#!/bin/sh
# $OpenBSD: install.sh,v 1.3 2002/06/09 06:15:15 todd Exp $
umask 0
[ "$TARDIR" ] || { echo "$0: set TARDIR first" ; exit 1; }

while read f
do
	gzip -d < $TARDIR/$f | tar xvpf -
done << \END_LIST
etc.tar.gz
dev.tar.gz
var.tar.gz
bin.tar.gz
sbin.tar.gz
usr.bin.tar.gz
usr.games.tar.gz
usr.include.tar.gz
usr.lib.tar.gz
usr.libexec.tar.gz
usr.misc.tar.gz
usr.sbin.tar.gz
usr.share.tar.gz
END_LIST
