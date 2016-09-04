define(MACHINE,sgi)dnl
vers(__file__,
	{-$OpenBSD: MAKEDEV.md,v 1.48 2016/09/04 15:38:59 naddy Exp $-},
etc.MACHINE)dnl
dnl
dnl Copyright (c) 2001-2006 Todd T. Fries <todd@OpenBSD.org>
dnl All rights reserved.
dnl
dnl Redistribution and use in source and binary forms, with or without
dnl modification, are permitted provided that the following conditions
dnl are met:
dnl 1. Redistributions of source code must retain the above copyright
dnl    notice, this list of conditions and the following disclaimer.
dnl 2. The name of the author may not be used to endorse or promote products
dnl    derived from this software without specific prior written permission.
dnl
dnl THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
dnl INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
dnl AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL
dnl THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
dnl EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
dnl PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
dnl OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
dnl WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
dnl OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
dnl ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
dnl
dnl
__devitem(zs, tty[a-b]*, Zilog 8530 serial port,zs)dnl
_mkdev(zs, {-tty[a-b]-}, {-u=${i#tty*}
	case $u in
	a) n=0 ;;
	b) n=1 ;;
	*) echo unknown tty device $i ;;
	esac
	M tty$u c major_zs_c $n 660 dialer uucp
	M cua$u c major_zs_c Add($n, 128) 660 dialer uucp-})dnl
_TITLE(make)
_DEV(all)
_DEV(ramd)
_DEV(std)
_DEV(local)
_TITLE(dis)
_DEV(cd, 8, 3)
_DEV(rd, 22, 8)
_DEV(sd, 9, 0)
_DEV(vnd, 11, 2)
_DEV(wd, 18, 4)
_TITLE(tap)
_DEV(ch, 36)
_DEV(st, 10)
_TITLE(term)
_DEV(com, 17)
_DEV(zs, 19)
_TITLE(pty)
_DEV(ptm, 52)
_DEV(pty, 5)
_DEV(tty, 4)
_TITLE(cons)
_DEV(wsdisp, 25)
_DEV(wscons)
_DEV(wskbd, 26)
_DEV(wsmux, 28)
_TITLE(point)
_DEV(wsmouse, 27)
_TITLE(usb)
_DEV(ttyU, 66)
_DEV(uall)
_DEV(ugen, 63)
_DEV(uhid, 62)
_DEV(ulpt, 64)
_DEV(usb, 61)
_TITLE(spec)
_DEV(au, 44)
_DEV(bio, 49)
_DEV(bpf, 12)
_DEV(diskmap, 69)
_DEV(fdesc, 7)
_DEV(fuse, 73)
_DEV(hotplug, 67)
_DEV(pci, 29)
_DEV(pf, 31)
_DEV(pppx, 70)
_DEV(rnd, 33)
_DEV(tun, 13)
_DEV(tap, 74)
_DEV(uk, 32)
_DEV(vi, 45)
_DEV(vscsi, 68)
_DEV(switch, 75)
dnl
divert(__mddivert)dnl
dnl
_std(2, 3, 35, 6)
	;;
dnl
dnl *** sgi specific targets
dnl
twrget(all, au, audio, 0, 1, 2)dnl
target(all, ch, 0)dnl
target(all, vscsi, 0)dnl
target(all, diskmap)dnl
target(all, pty, 0, 1, 2)dnl
target(all, bio)dnl
target(all, tun, 0, 1, 2, 3)dnl
target(all, tap, 0, 1, 2, 3)dnl
target(all, rd, 0)dnl
target(all, cd, 0, 1)dnl
target(all, sd, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9)dnl
target(all, vnd, 0, 1, 2, 3)dnl
target(all, switch, 0, 1, 2, 3)dnl
twrget(all, zs, tty, a, b)dnl
twrget(wscons, wscons, ttyD, cfg, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, a, b)dnl
twrget(wscons, wscons, ttyE, cfg, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, a, b)dnl
twrget(wscons, wscons, ttyF, cfg, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, a, b)dnl
target(ramd, pty, 0)dnl
target(ramd, bio)dnl
target(ramd, diskmap)dnl
target(ramd, random)dnl
