/*	$OpenBSD: inet.h,v 1.13 2012/07/10 11:49:42 guenther Exp $	*/

/*
 * ++Copyright++ 1983, 1993
 * -
 * Copyright (c) 1983, 1993
 *    The Regents of the University of California.  All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * -
 * Portions Copyright (c) 1993 by Digital Equipment Corporation.
 * 
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies, and that
 * the name of Digital Equipment Corporation not be used in advertising or
 * publicity pertaining to distribution of the document or software without
 * specific, written prior permission.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND DIGITAL EQUIPMENT CORP. DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS.   IN NO EVENT SHALL DIGITAL EQUIPMENT
 * CORPORATION BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 * -
 * --Copyright--
 */

/*
 *	@(#)inet.h	8.1 (Berkeley) 6/2/93
 *	$From: inet.h,v 8.6 1996/08/08 06:54:29 vixie Exp $
 */

#ifndef _INET_H_
#define	_INET_H_

/* External definitions for functions in inet(3) */

#include <sys/cdefs.h>
#include <sys/types.h>
#include <machine/endian.h>

/*
 * Buffer lengths for strings containing printable IP addresses
 */
#ifndef INET_ADDRSTRLEN
#define INET_ADDRSTRLEN		16
#endif
#ifndef INET6_ADDRSTRLEN
#define INET6_ADDRSTRLEN	46
#endif

#ifndef _IN_ADDR_DECLARED
#define _IN_ADDR_DECLARED
/*
 * IP Version 4 Internet address (a structure for historical reasons)
 */
struct in_addr {
	in_addr_t s_addr;
};
#endif


__BEGIN_DECLS
in_addr_t	 inet_addr(const char *);
char		*inet_ntoa(struct in_addr);
const char	*inet_ntop(int, const void *__restrict, char *__restrict,
		    socklen_t) __attribute__ ((__bounded__(__string__,3,4)));
int		 inet_pton(int, const char *__restrict, void *__restrict);

#if __BSD_VISIBLE
int		 inet_aton(const char *, struct in_addr *);
in_addr_t	 inet_lnaof(struct in_addr);
struct in_addr	 inet_makeaddr(in_addr_t , in_addr_t);
char *		 inet_neta(in_addr_t, char *, size_t)
			__attribute__((__bounded__(__string__,2,3)));
in_addr_t	 inet_netof(struct in_addr);
in_addr_t	 inet_network(const char *);
char		*inet_net_ntop(int, const void *, int, char *, size_t)
			__attribute__((__bounded__(__string__,4,5)));
int		 inet_net_pton(int, const char *, void *, size_t)
			__attribute__((__bounded__(__string__,3,4)));
unsigned int	 inet_nsap_addr(const char *, unsigned char *, int);
char		*inet_nsap_ntoa(int, const unsigned char *, char *);
#endif /* __BSD_VISIBLE */
__END_DECLS

#endif /* !_INET_H_ */
