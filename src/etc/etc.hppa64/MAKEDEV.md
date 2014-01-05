define(MACHINE,hppa64)dnl
vers(__file__,
	{-$OpenBSD: MAKEDEV.md,v 1.28 2014/01/05 01:16:52 deraadt Exp $-},
etc.MACHINE)dnl
dnl
dnl Copyright (c) 2001-2006 Todd T. Fries <todd@OpenBSD.org>
dnl
dnl Permission to use, copy, modify, and distribute this software for any
dnl purpose with or without fee is hereby granted, provided that the above
dnl copyright notice and this permission notice appear in all copies.
dnl
dnl THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
dnl WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
dnl MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
dnl ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
dnl WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
dnl ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
dnl OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
dnl
dnl
_TITLE(make)
_DEV(all)
_DEV(ramdisk)
_DEV(std)
_DEV(local)
_TITLE(dis)
_DEV(cd, 12, 6)
_DEV(flo, 24, 7)
_DEV(sd, 10, 4)
_DEV(rd, 9, 3)
_DEV(vnd, 8, 2)
_DEV(wd, 39, 8)
_TITLE(tap)
_DEV(ch, 13)
_DEV(st, 11, 5)
_TITLE(term)
_DEV(com, 23)
dnl _TITLE(call)
_TITLE(pty)
_DEV(ptm, 38)
_DEV(pty, 5)
_DEV(tty, 4)
_TITLE(cons)
_DEV(wsdisp, 27)
_DEV(wscons)
_DEV(wskbd, 28)
_DEV(wsmux, 30)
_TITLE(point)
_DEV(wsmouse, 29)
_TITLE(prn)
_DEV(lpt, 26)
_TITLE(usb)
_DEV(uall)
_DEV(usb, 40)
_DEV(uhid, 41)
_DEV(ugen, 42)
_DEV(ulpt, 43)
_DEV(urio, 44)
_DEV(ttyU, 45)
_TITLE(spec)
_DEV(au, 35)
_DEV(bio, 37)
_DEV(bpf, 17)
_DEV(cry, 36)
_DEV(diskmap,56)
_DEV(fdesc, 16)
_DEV(fuse, 58)
_DEV(lkm, 19)
_DEV(pci, 31)
_DEV(pdc, 22)
_DEV(pf, 21)
_DEV(pppx,57)
_DEV(rnd, 20)
_DEV(systrace, 34)
_DEV(tun, 18)
_DEV(uk, 15)
_DEV(vi, 33)
_DEV(vscsi, 54)
dnl
divert(__mddivert)dnl
dnl
ramdisk)
	_recurse std fd st0 st1 sd0 sd1 sd2 sd3 rd0 random
	_recurse pty0 bpf0 bpf1 tun0 tun1 lkm bio diskmap
	;;

_std(1, 2, 25, 6)
	M pdc		c 22 0 640 operator
	;;
dnl
dnl *** hppa64 specific devices
dnl
twrget(all, au, audio, 0, 1, 2)dnl
target(all, ch, 0)dnl
target(all, vscsi, 0)dnl
target(all, diskmap)dnl
twrget(all, flo, fd, 0, 0B, 0C, 0D, 0E, 0F, 0G, 0H)dnl
twrget(all, flo, fd, 1, 1B, 1C, 1D, 1E, 1F, 1G, 1H)dnl
target(all, pty, 0)dnl
target(all, bpf, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9)dnl
target(all, bio)dnl
target(all, tun, 0, 1, 2, 3)dnl
target(all, xy, 0, 1, 2, 3)dnl
target(all, rd, 0)dnl
target(all, cd, 0, 1)dnl
target(all, sd, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9)dnl
target(all, vnd, 0, 1, 2, 3)dnl
target(ramd, pty, 0)dnl
target(ramd, hil)dnl
target(ramd, tun, 0, 1)dnl
