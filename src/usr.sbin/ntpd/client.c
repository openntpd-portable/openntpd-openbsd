/*	$OpenBSD: client.c,v 1.28 2004/07/20 16:47:55 henning Exp $ */

/*
 * Copyright (c) 2003, 2004 Henning Brauer <henning@openbsd.org>
 * Copyright (c) 2004 Alexander Guy <alexander.guy@andern.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF MIND, USE, DATA OR PROFITS, WHETHER
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING
 * OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <sys/param.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "ntpd.h"

int	client_update(struct ntp_peer *);

int
client_peer_init(struct ntp_peer *p)
{
	struct sockaddr_in	*sa_in;
	struct sockaddr_in6	*sa_in6;
	struct ntp_addr		*h;

	if ((p->query = calloc(1, sizeof(struct ntp_query))) == NULL)
		fatal("client_query calloc");

	for (h = p->addr; h != NULL; h = h->next) {
		switch (h->ss.ss_family) {
		case AF_INET:
			sa_in = (struct sockaddr_in *)&h->ss;
			if (ntohs(sa_in->sin_port) == 0)
				sa_in->sin_port = htons(123);
			break;
		case AF_INET6:
			sa_in6 = (struct sockaddr_in6 *)&h->ss;
			if (ntohs(sa_in6->sin6_port) == 0)
				sa_in6->sin6_port = htons(123);
			break;
		default:
			fatal("king bula sez: wrong AF in client_peer_init");
			/* not reached */
		}
	}

	if ((p->query->fd = socket(p->addr->ss.ss_family, SOCK_DGRAM, 0)) == -1)
		fatal("client_query socket");

	p->query->msg.status = MODE_CLIENT | (NTP_VERSION << 3);
	p->state = STATE_NONE;
	p->next = time(NULL);
	p->shift = 0;
	p->trustlevel = TRUSTLEVEL_PATHETIC;

	return (0);
}

int
client_nextaddr(struct ntp_peer *p)
{
	close(p->query->fd);

	if ((p->addr = p->addr->next) == NULL)
		p->addr = p->addr_head.a;

	if ((p->query->fd = socket(p->addr->ss.ss_family, SOCK_DGRAM, 0)) == -1)
		fatal("client_query socket");

	p->shift = 0;
	p->trustlevel = TRUSTLEVEL_PATHETIC;

	return (0);
}

int
client_query(struct ntp_peer *p)
{
	/*
	 * Send out a random 64-bit number as our transmit time.  The NTP
	 * server will copy said number into the originate field on the
	 * response that it sends us.  This is totally legal per the SNTP spec.
	 *
	 * The impact of this is two fold: we no longer send out the current
	 * system time for the world to see (which may aid an attacker), and
	 * it gives us a (not very secure) way of knowing that we're not
	 * getting spoofed by an attacker that can't capture our traffic
	 * but can spoof packets from the NTP server we're communicating with.
	 *
	 * Save the real transmit timestamp locally.
	 */

	p->query->msg.xmttime.int_part = arc4random();
	p->query->msg.xmttime.fraction = arc4random();
	p->query->xmttime = gettime();

	if (ntp_sendmsg(p->query->fd, (struct sockaddr *)&p->addr->ss,
	    &p->query->msg, NTP_MSGSIZE_NOAUTH, 0) == -1) {
		p->next = time(NULL) + INTERVAL_QUERY_PATHETIC;
		return (-1);
	}

	p->state = STATE_QUERY_SENT;
	p->next = 0;
	p->deadline = time(NULL) + QUERYTIME_MAX;

	return (0);
}

int
client_dispatch(struct ntp_peer *p)
{
	struct sockaddr_storage	 fsa;
	socklen_t		 fsa_len;
	char			 buf[NTP_MSGSIZE];
	ssize_t			 size;
	struct ntp_msg		 msg;
	double			 T1, T2, T3, T4;
	double			 abs_offset;
	time_t			 interval;

	fsa_len = sizeof(fsa);
	if ((size = recvfrom(p->query->fd, &buf, sizeof(buf), 0,
	    (struct sockaddr *)&fsa, &fsa_len)) == -1) {
		if (errno == EHOSTUNREACH || errno == EHOSTDOWN ||
		    errno == ENETDOWN) {
			log_warn("recvfrom %s",
			    log_sockaddr((struct sockaddr *)&fsa));
			return (0);
		} else
			fatal("recvfrom");
	}

	T4 = gettime();

	ntp_getmsg(buf, size, &msg);

	if (msg.orgtime.int_part != p->query->msg.xmttime.int_part ||
	    msg.orgtime.fraction != p->query->msg.xmttime.fraction)
		return (0);

	/*
	 * From RFC 2030 (with a correction to the delay math):
	 *
	 *      Timestamp Name          ID   When Generated
	 *     ------------------------------------------------------------
	 *     Originate Timestamp     T1   time request sent by client
	 *     Receive Timestamp       T2   time request received by server
	 *     Transmit Timestamp      T3   time reply sent by server
	 *     Destination Timestamp   T4   time reply received by client
	 *
	 *  The roundtrip delay d and local clock offset t are defined as
	 *
	 *    d = (T4 - T1) - (T3 - T2)     t = ((T2 - T1) + (T3 - T4)) / 2.
	 */

	T1 = p->query->xmttime;
	T2 = lfp_to_d(msg.rectime);
	T3 = lfp_to_d(msg.xmttime);

	p->reply[p->shift].offset = ((T2 - T1) + (T3 - T4)) / 2;
	p->reply[p->shift].delay = (T4 - T1) - (T3 - T2);
	p->reply[p->shift].error = (T2 - T1) - (T3 - T4);
	p->reply[p->shift].rcvd = time(NULL);
	p->reply[p->shift].good = 1;

	p->reply[p->shift].status.leap = (msg.status & LIMASK) >> 6;
	p->reply[p->shift].status.precision = msg.precision;
	p->reply[p->shift].status.rootdelay = sfp_to_d(msg.distance);
	p->reply[p->shift].status.rootdispersion = sfp_to_d(msg.dispersion);
	p->reply[p->shift].status.refid = htonl(msg.refid);
	p->reply[p->shift].status.reftime = lfp_to_d(msg.reftime);
	p->reply[p->shift].status.poll = msg.ppoll;

	if (p->trustlevel < TRUSTLEVEL_PATHETIC)
		interval = INTERVAL_QUERY_PATHETIC;
	else if (p->trustlevel < TRUSTLEVEL_AGRESSIVE)
		interval = INTERVAL_QUERY_AGRESSIVE;
	else {
		if (p->reply[p->shift].offset < 0)
			abs_offset = -p->reply[p->shift].offset;
		else
			abs_offset = p->reply[p->shift].offset;

		if (abs_offset > QSCALE_OFF_MAX)
			interval = INTERVAL_QUERY_NORMAL;
		else if (abs_offset < QSCALE_OFF_MIN)
			interval = INTERVAL_QUERY_NORMAL *
			    (QSCALE_OFF_MAX / QSCALE_OFF_MIN);
		else
			interval = INTERVAL_QUERY_NORMAL *
			    (QSCALE_OFF_MAX / abs_offset);
	}

	p->next = time(NULL) + interval;
	p->deadline = 0;
	p->state = STATE_REPLY_RECEIVED;

	/* every received reply which we do not discard increases trust */
	if (p->trustlevel < 10) {
		if (p->trustlevel < TRUSTLEVEL_BADPEER &&
		    p->trustlevel + 1 >= TRUSTLEVEL_BADPEER)
			log_info("peer %s now valid",
			    log_sockaddr((struct sockaddr *)&fsa));
		p->trustlevel++;
	}

	client_update(p);

	log_debug("reply from %s: offset %f delay %f, "
	    "next query %ds", log_sockaddr((struct sockaddr *)&fsa),
	    p->reply[p->shift].offset, p->reply[p->shift].delay, interval);

	if (++p->shift >= OFFSET_ARRAY_SIZE)
		p->shift = 0;

	return (0);
}

int
client_update(struct ntp_peer *p)
{
	int	i, best = 0, good = 0;

	/*
	 * clock filter
	 * find the offset which arrived with the lowest delay
	 * use that as the peer update
	 * invalidate it and all older ones
	 */

	for (i = 0; good == 0 && i < OFFSET_ARRAY_SIZE; i++)
		if (p->reply[i].good) {
			good++;
			best = i;
		}

	for (; i < OFFSET_ARRAY_SIZE; i++)
		if (p->reply[i].good) {
			good++;
			if (p->reply[i].delay < p->reply[best].delay)
				best = i;
		}

	if (good < 8)
		return (-1);

	memcpy(&p->update, &p->reply[best], sizeof(p->update));
	ntp_adjtime();

	for (i = 0; i < OFFSET_ARRAY_SIZE; i++)
		if (p->reply[i].rcvd <= p->reply[best].rcvd)
			p->reply[i].good = 0;

	return (0);
}
