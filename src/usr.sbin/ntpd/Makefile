#	$OpenBSD$

.PATH:		${.CURDIR}/..

PROG=	ntpd
SRCS=	ntpd.c buffer.c log.c imsg.c ntp.c
CFLAGS+= -Wall -I${.CURDIR}
CFLAGS+= -Wstrict-prototypes -Wmissing-prototypes
CFLAGS+= -Wmissing-declarations
CFLAGS+= -Wshadow -Wpointer-arith -Wcast-qual
CFLAGS+= -Wsign-compare
YFLAGS=
NOMAN=

.include <bsd.prog.mk>
