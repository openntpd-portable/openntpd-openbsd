vers(__file__,
	{-$OpenBSD: MAKEDEV.md,v 1.4 2002/02/14 13:29:20 todd Exp $-},
etc.MACHINE)dnl
dnl
dnl Copyright (c) 2001 Todd T. Fries <todd@OpenBSD.org>
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
_TITLE(make)
_DEV(all)
_DEV(std)
_DEV(fdesc, 21)
_DEV(local)
_DEV(ramd)
_TITLE(tap)
_DEV(st, 20, 5)
_TITLE(dis)
_DEV(wd, 11, 0)
_DEV(sd, 8, 2)
_DEV(cd, 9, 3)
_DEV(ch, 10)
_DEV(vnd, 19, 14)
_DEV(rd, 17, 17)
_DEV(raid, 54, 19)
_DEV(ccd, 18, 16)
_TITLE(cons)
_DEV(wscons)
_DEV(wsdisp, 67)
_DEV(wskbd, 68)
_DEV(wsmouse, 69)
_DEV(wsmux, 70)
_TITLE(term)
_DEV(ttyE)
_DEV(com, 7)
_TITLE(pty)
_DEV(tty, 4)
_DEV(pty, 5)
_TITLE(usb)
_DEV(usb, 61)
_DEV(uhid, 62)
_DEV(ugen, 63)
_DEV(ulpt, 64)
_DEV(urio, 65)
_DEV(utty, 66)
_DEV(uscan, 74)
_TITLE(spec)
_DEV(au, 44)
_DEV(apm, 25)
_DEV(bpf, 22)
_DEV(tun, 23)
_DEV(lkm, 24)
_DEV(rnd, 40)
_DEV(uk, 41)
_DEV(ss, 42)
_DEV(xfs, 51)
_DEV(altq, 72)
_DEV(iop, 73)
_DEV(pf, 39)
_DEV(cry, 47)
_DEV(usbs)
_DEV(pci, 71)
dnl
divert(7)dnl
dnl
_std(1, 2, 43, 3, 6)
	M reload	c 2 20 640 kmem
	;;

ttyE*)
	type=E
	case $type in
	E)	major=25; minor=$unit ;;
	esac
	M tty$type$unit c $major $minor
	;;
