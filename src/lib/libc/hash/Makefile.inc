#	$OpenBSD: Makefile.inc,v 1.1 1996/09/29 16:15:04 millert Exp $

# hash functions
.PATH: ${.CURDIR}/hash

SRCS+=	sha1.c
MAN+=	sha1.3
MLINKS+=sha1.3 SHA1Init.3
MLINKS+=sha1.3 SHA1Update.3
MLINKS+=sha1.3 SHA1Final.3
MLINKS+=sha1.3 SHA1Transform.3
