/*	$OpenBSD: control.c,v 1.1 2013/10/04 14:28:16 phessler Exp $ */

/*
 * Copyright (c) 2003, 2004 Henning Brauer <henning@openbsd.org>
 * Copyright (c) 2012 Mike Miller <mmiller@mgm51.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>

#include "ntpd.h"

#define	CONTROL_BACKLOG	5

int
control_init(char *path)
{
	struct sockaddr_un	 sun;
	int			 fd;
	mode_t			 old_umask;

	if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		log_warn("control_init: socket");
		return (-1);
	}

	bzero(&sun, sizeof(sun));
	sun.sun_family = AF_UNIX;
	if (strlcpy(sun.sun_path, path, sizeof(sun.sun_path)) >=
	    sizeof(sun.sun_path))
		errx(1, "ctl socket name too long");

	if (unlink(path) == -1)
		if (errno != ENOENT) {
			log_warn("control_init: unlink %s", path);
			close(fd);
			return (-1);
		}

	old_umask = umask(S_IXUSR|S_IXGRP|S_IWOTH|S_IROTH|S_IXOTH);
	if (bind(fd, (struct sockaddr *)&sun, sizeof(sun)) == -1) {
		log_warn("control_init: bind: %s", path);
		close(fd);
		umask(old_umask);
		return (-1);
	}
	umask(old_umask);

	if (chmod(path, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP) == -1) {
		log_warn("control_init: chmod");
		close(fd);
		(void)unlink(path);
		return (-1);
	}

	session_socket_blockmode(fd, BM_NONBLOCK);

	return (fd);
}

int
control_listen(int fd)
{
	if (fd != -1 && listen(fd, CONTROL_BACKLOG) == -1) {
		log_warn("control_listen: listen");
		return (-1);
	}

	return (0);
}

void
control_shutdown(int fd)
{
	close(fd);
}

void
control_cleanup(const char *path)
{
	if (path)
		unlink(path);
}

int
control_accept(int listenfd)
{
	int			 connfd;
	socklen_t		 len;
	struct sockaddr_un	 sun;
	struct ctl_conn		*ctl_conn;

	len = sizeof(sun);
	if ((connfd = accept(listenfd,
	    (struct sockaddr *)&sun, &len)) == -1) {
		if (errno != EWOULDBLOCK && errno != EINTR)
			log_warn("control_accept: accept");
		return (0);
	}

	session_socket_blockmode(connfd, BM_NONBLOCK);

	if ((ctl_conn = calloc(1, sizeof(struct ctl_conn))) == NULL) {
		log_warn("control_accept");
		close(connfd);
		return (0);
	}

	imsg_init(&ctl_conn->ibuf, connfd);

	TAILQ_INSERT_TAIL(&ctl_conns, ctl_conn, entry);

	return (1);
}

struct ctl_conn *
control_connbyfd(int fd)
{
	struct ctl_conn	*c;

	for (c = TAILQ_FIRST(&ctl_conns); c != NULL && c->ibuf.fd != fd;
	    c = TAILQ_NEXT(c, entry))
		;	/* nothing */

	return (c);
}

int
control_close(int fd)
{
	struct ctl_conn	*c;

	if ((c = control_connbyfd(fd)) == NULL) {
		log_warn("control_close: fd %d: not found", fd);
		return (0);
	}

	msgbuf_clear(&c->ibuf.w);
	TAILQ_REMOVE(&ctl_conns, c, entry);

	close(c->ibuf.fd);
	free(c);

	return (1);
}

int
control_dispatch_msg(struct pollfd *pfd, u_int *ctl_cnt)
{
	struct imsg		 imsg;
	struct ctl_conn		*c;
	struct ntp_peer 	*p;
	struct ntp_sensor 	*s;
	struct ctl_show_status	 c_status;
	struct ctl_show_peer	 c_peer;
	struct ctl_show_sensor	 c_sensor;
	int			 cnt;
	ssize_t			 n;

	if ((c = control_connbyfd(pfd->fd)) == NULL) {
		log_warn("control_dispatch_msg: fd %d: not found", pfd->fd);
		return (0);
	}

	if (pfd->revents & POLLOUT)
		if (msgbuf_write(&c->ibuf.w) < 0) {
			*ctl_cnt -= control_close(pfd->fd);
			return (1);
		}

	if (!(pfd->revents & POLLIN))
		return (0);

	if ((n = imsg_read(&c->ibuf)) == -1 || n == 0) {
		*ctl_cnt -= control_close(pfd->fd);
		return (1);
	}

	for (;;) {
		if ((n = imsg_get(&c->ibuf, &imsg)) == -1) {
			*ctl_cnt -= control_close(pfd->fd);
			return (1);
		}
		if (n == 0)
			break;

		switch (imsg.hdr.type) {
		case IMSG_CTL_SHOW_STATUS:
			build_show_status(&c_status);
			imsg_compose(&c->ibuf, IMSG_CTL_SHOW_STATUS, 0, 0, -1,
			    &c_status, sizeof (c_status));
			break;
		case IMSG_CTL_SHOW_PEERS:
			cnt = 0;
			TAILQ_FOREACH(p, &conf->ntp_peers, entry) {
				build_show_peer(&c_peer, p);
				imsg_compose(&c->ibuf, IMSG_CTL_SHOW_PEERS,
				    0, 0, -1, &c_peer, sizeof(c_peer));
				cnt++;
			}
			imsg_compose(&c->ibuf, IMSG_CTL_SHOW_PEERS_END,
			    0, 0, -1, &cnt, sizeof(cnt));
			break;
		case IMSG_CTL_SHOW_SENSORS:
			cnt = 0;
			TAILQ_FOREACH(s, &conf->ntp_sensors, entry) {
				build_show_sensor(&c_sensor, s);
				imsg_compose(&c->ibuf, IMSG_CTL_SHOW_SENSORS,
				    0, 0, -1, &c_sensor, sizeof(c_sensor));
			cnt++;
			}
			imsg_compose(&c->ibuf, IMSG_CTL_SHOW_SENSORS_END,
			    0, 0, -1, &cnt, sizeof(cnt));
			break;
		case IMSG_CTL_SHOW_ALL:
			build_show_status(&c_status);
			imsg_compose(&c->ibuf, IMSG_CTL_SHOW_STATUS, 0, 0, -1,
			    &c_status, sizeof (c_status));

			cnt = 0;
			TAILQ_FOREACH(p, &conf->ntp_peers, entry) {
				build_show_peer(&c_peer, p);
				imsg_compose(&c->ibuf, IMSG_CTL_SHOW_PEERS,
				    0, 0, -1, &c_peer, sizeof(c_peer));
				cnt++;
			}
			imsg_compose(&c->ibuf, IMSG_CTL_SHOW_PEERS_END,
			    0, 0, -1, &cnt, sizeof(cnt));

			cnt = 0;
			TAILQ_FOREACH(s, &conf->ntp_sensors, entry) {
				build_show_sensor(&c_sensor, s);
				imsg_compose(&c->ibuf, IMSG_CTL_SHOW_SENSORS,
				    0, 0, -1, &c_sensor, sizeof(c_sensor));
			cnt++;
			}
			imsg_compose(&c->ibuf, IMSG_CTL_SHOW_SENSORS_END,
			    0, 0, -1, &cnt, sizeof(cnt));

			imsg_compose(&c->ibuf, IMSG_CTL_SHOW_ALL_END,
			    0, 0, -1, NULL, 0);
			break;
		default:
			break;
		}
		imsg_free(&imsg);
	}
	return (0);
}

void
session_socket_blockmode(int fd, enum blockmodes bm)
{
	int	flags;

	if ((flags = fcntl(fd, F_GETFL, 0)) == -1)
		fatal("fcntl F_GETFL");

	if (bm == BM_NONBLOCK)
		flags |= O_NONBLOCK;
	else
		flags &= ~O_NONBLOCK;

	if ((flags = fcntl(fd, F_SETFL, flags)) == -1)
		fatal("fcntl F_SETFL");
}

void
build_show_status(struct ctl_show_status *cs)
{
	struct ntp_peer 	*p;
	struct ntp_sensor 	*s;

	cs->peercnt = cs->valid_peers = 0;
	cs->sensorcnt = cs->valid_sensors = 0;

	TAILQ_FOREACH(p, &conf->ntp_peers, entry) {
		cs->peercnt++;
		if (p->trustlevel >= TRUSTLEVEL_BADPEER)
			cs->valid_peers++;
	}
	TAILQ_FOREACH(s, &conf->ntp_sensors, entry) {
		cs->sensorcnt++;
		if (s->update.good)
			cs->valid_sensors++;
	}

	cs->synced = conf->status.synced;
	cs->stratum = conf->status.stratum;
	cs->clock_offset = getoffset() * 1000.0;
}

void
build_show_peer(struct ctl_show_peer *cp, struct ntp_peer *p)
{
	const char 	*a = "not resolved";
	const char 	*pool = "", *addr_head_name = "";
	u_int8_t 	 shift, best, validdelaycnt, jittercnt;
	time_t		 now;

	now = getmonotime();

	if (p->addr)
		a = log_sockaddr((struct sockaddr *)&p->addr->ss);
	if (p->addr_head.pool)
		pool = "from pool ";

	if (0 != strcmp(a, p->addr_head.name))
		addr_head_name = p->addr_head.name;

	snprintf(cp->peer_desc, sizeof(cp->peer_desc),
	    "%s %s%s %s", a, pool, addr_head_name,
	    print_rtable(p->rtable) );

	validdelaycnt = best = 0;
	cp->offset = cp->delay = 0.0;
	for (shift = 0; shift < OFFSET_ARRAY_SIZE; shift++) {
		if (p->reply[shift].delay > 0.0) {
			cp->offset += p->reply[shift].offset;
			cp->delay += p->reply[shift].delay;

			if (p->reply[shift].delay < p->reply[best].delay)
				best = shift;

			validdelaycnt++;
		}
	}

	if (validdelaycnt > 1) {
		cp->offset /= validdelaycnt;
		cp->delay /= validdelaycnt;
	}

	/*
	 *  use simple average for jitter calculation, as the
	 *  RFC5905-recommended RMS average needs the math library
	 */
	jittercnt = 0;
	cp->jitter = 0.0;
	for (shift = 0; shift < OFFSET_ARRAY_SIZE; shift++) {
		if (p->reply[shift].delay > 0.0 && shift != best) {
			cp->jitter += p->reply[shift].delay -
			    p->reply[best].delay;
			jittercnt++;
		}
	}
	if (jittercnt > 1)
		cp->jitter /= jittercnt;

	if (p->shift == 0)
		shift = OFFSET_ARRAY_SIZE - 1;
	else
		shift = p->shift - 1;

	if (conf->status.synced == 1 &&
	    p->reply[shift].status.send_refid == conf->status.refid)
		cp->syncedto = 1;
	else
		cp->syncedto = 0;

	/* milliseconds to reduce number of leading zeroes */
	cp->offset *= 1000.0;
	cp->delay *= 1000.0;
	cp->jitter *= 1000.0;

	cp->weight = p->weight;
	cp->trustlevel = p->trustlevel;
	cp->stratum = p->reply[shift].status.stratum;
	cp->next = p->next - now < 0 ? 0 : p->next - now;
	cp->poll = p->poll;
}

void
build_show_sensor(struct ctl_show_sensor *cs, struct ntp_sensor *s)
{
	time_t		 now;
	u_int8_t 	 shift;
	u_int32_t	 refid;

	now = getmonotime();

	memcpy(&refid, SENSOR_DEFAULT_REFID, sizeof(refid));
	refid = refid == s->refid ? 0 : s->refid;

	snprintf(cs->sensor_desc, sizeof(cs->sensor_desc),
	    "%s  %.4s", s->device, (char *)&refid);

	if (s->shift == 0)
		shift = SENSOR_OFFSETS - 1;
	else
		shift = s->shift - 1;

	if (conf->status.synced == 1 &&
	    s->offsets[shift].status.send_refid == conf->status.refid)
		cs->syncedto = 1;
	else
		cs->syncedto = 0;

	cs->weight = s->weight;
	cs->good = s->update.good;
	cs->stratum = s->offsets[shift].status.stratum;
	cs->next = s->next - now < 0 ? 0 : s->next - now;
	cs->poll = SENSOR_QUERY_INTERVAL;
	cs->offset = s->offsets[shift].offset * 1000.0;
	cs->correction = (double)s->correction / 1000.0;
}
