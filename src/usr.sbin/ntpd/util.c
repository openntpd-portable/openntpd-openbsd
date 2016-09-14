/*	$OpenBSD: util.c,v 1.22 2016/09/14 13:20:16 rzalamena Exp $ */

/*
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
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "ntpd.h"

double
gettime_corrected(void)
{
	return (gettime() + getoffset());
}

double
getoffset(void)
{
	struct timeval	tv;
	if (adjtime(NULL, &tv) == -1)
		return (0.0);
	return (tv.tv_sec + 1.0e-6 * tv.tv_usec);
}

double
gettime(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		fatal("gettimeofday");

	return (gettime_from_timeval(&tv));
}

double
gettime_from_timeval(struct timeval *tv)
{
	/*
	 * Account for overflow on OSes that have a 32-bit time_t.
	 */
	return ((uint64_t)tv->tv_sec + JAN_1970 + 1.0e-6 * tv->tv_usec);
}

time_t
getmonotime(void)
{
	struct timespec	ts;

	if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0)
		fatal("clock_gettime");

	return (ts.tv_sec);
}


void
d_to_tv(double d, struct timeval *tv)
{
	tv->tv_sec = d;
	tv->tv_usec = (d - tv->tv_sec) * 1000000;
	while (tv->tv_usec < 0) {
		tv->tv_usec += 1000000;
		tv->tv_sec -= 1;
	}
}

double
lfp_to_d(struct l_fixedpt lfp)
{
	double	ret;

	lfp.int_partl = ntohl(lfp.int_partl);
	lfp.fractionl = ntohl(lfp.fractionl);

	ret = (double)(lfp.int_partl) + ((double)lfp.fractionl / UINT_MAX);

	return (ret);
}

struct l_fixedpt
d_to_lfp(double d)
{
	struct l_fixedpt	lfp;

	lfp.int_partl = htonl((u_int32_t)d);
	lfp.fractionl = htonl((u_int32_t)((d - (u_int32_t)d) * UINT_MAX));

	return (lfp);
}

double
sfp_to_d(struct s_fixedpt sfp)
{
	double	ret;

	sfp.int_parts = ntohs(sfp.int_parts);
	sfp.fractions = ntohs(sfp.fractions);

	ret = (double)(sfp.int_parts) + ((double)sfp.fractions / USHRT_MAX);

	return (ret);
}

struct s_fixedpt
d_to_sfp(double d)
{
	struct s_fixedpt	sfp;

	sfp.int_parts = htons((u_int16_t)d);
	sfp.fractions = htons((u_int16_t)((d - (u_int16_t)d) * USHRT_MAX));

	return (sfp);
}

char *
print_rtable(int r)
{
	static char b[11];

	b[0] = 0;
	if (r > 0)
		snprintf(b, sizeof(b), "rtable %d", r);

	return (b);
}

const char *
log_sockaddr(struct sockaddr *sa)
{
	static char	buf[NI_MAXHOST];

	if (getnameinfo(sa, SA_LEN(sa), buf, sizeof(buf), NULL, 0,
	    NI_NUMERICHOST))
		return ("(unknown)");
	else
		return (buf);
}

pid_t
start_child(char *pname, int cfd, int argc, char **argv)
{
	char		**nargv;
	int		  nargc, i;
	pid_t		  pid;

	/* Prepare the child process new argv. */
	nargv = calloc(argc + 3, sizeof(char *));
	if (nargv == NULL)
		fatal("%s: calloc", __func__);

	/* Copy the program name first. */
	nargc = 0;
	nargv[nargc++] = argv[0];

	/* Set the process name and copy the original args. */
	nargv[nargc++] = "-P";
	nargv[nargc++] = pname;
	for (i = 1; i < argc; i++)
		nargv[nargc++] = argv[i];

	nargv[nargc] = 0;

	switch (pid = fork()) {
	case -1:
		fatal("%s: fork", __func__);
		break;
	case 0:
		/* Prepare the parent socket and execute. */
		dup2(cfd, PARENT_SOCK_FILENO);

		execvp(argv[0], nargv);
		fatal("%s: execvp", __func__);
		break;

	default:
		/* Close child's socket end. */
		close(cfd);
		break;
	}

	free(nargv);
	return (pid);
}

int
sanitize_argv(int *argc, char ***argv)
{
	char		**nargv;
	int		  nargc;
	int		  i;

	/*
	 * We need at least three arguments:
	 * Example: '/usr/sbin/ntpd' '-P' 'foobar'.
	 */
	if (*argc < 3)
		return (-1);

	*argc -= 2;

	/* Allocate new arguments vector and copy pointers. */
	nargv = calloc((*argc) + 1, sizeof(char *));
	if (nargv == NULL)
		return (-1);

	nargc = 0;
	nargv[nargc++] = (*argv)[0];
	for (i = 1; i < *argc; i++)
		nargv[nargc++] = (*argv)[i + 2];

	nargv[nargc] = NULL;
	*argv = nargv;
	return (0);
}
