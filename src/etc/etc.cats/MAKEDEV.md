vers(__file__,
	{-$OpenBSD: MAKEDEV.md,v 1.4 2004/02/13 23:34:21 miod Exp $-},
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
_DEV(fdesc, 7)
_DEV(local)
_DEV(ramd)
_TITLE(tap)
_DEV(st, 25, 25)
_TITLE(dis)
_DEV(wd, 16, 16)
_DEV(sd, 24, 24)
_DEV(cd, 26, 26)
_DEV(ch, 27)
_DEV(rd, 18, 18)
_DEV(vnd, 19, 19)
_DEV(raid, 71, 71)
_DEV(ccd, 21, 21)
_TITLE(cons)
_DEV(wscons)
_DEV(wsdisp, 60)
_DEV(wskbd, 61)
_DEV(wsmouse, 62)
_DEV(wsmux, 63)
_TITLE(term)
dnl _DEV(fcom, 54)
dnl _DEV(com, 12)
_DEV(com, 54)
_TITLE(pty)
_DEV(tty, 4)
_DEV(pty, 5)
_TITLE(usb)
_DEV(usb, 64)
_DEV(uhid, 65)
_DEV(ulpt, 66)
_DEV(urio, 67)
_DEV(ttyU, 68)
_DEV(uscan, 69)
_DEV(ugen, 70)
_TITLE(spec)
_DEV(au, 36)
_DEV(apm, 25)
_DEV(bpf, 22)
_DEV(tun, 33)
_DEV(lkm, 35)
_DEV(rnd, 40)
_DEV(uk, 28)
_DEV(ss, 29)
_DEV(xfs, 51)
_DEV(iop, 73)
_DEV(pf, 39)
_DEV(cry, 47)
_DEV(usbs)
_DEV(pci, 71)
_DEV(systrace, 50)
_DEV(bktr, 75)
_DEV(tuner, 75)
_DEV(radio, 97)
_DEV(ptm, 98)
dnl
divert(7)dnl
dnl
_std(1, 2, 43, 3, 6)
	M xf86		c 2 4 600
	M reload	c 2 20 640 kmem
	;;
dnl
dnl *** cats specific targets
dnl
target(all, ses, 0)dnl
target(all, ch, 0)dnl
target(all, ss, 0, 1)dnl
target(all, xfs, 0)dnl
twrget(all, flo, fd, 0, 0B, 0C, 0D, 0E, 0F, 0G, 0H)dnl
twrget(all, flo, fd, 1, 1B, 1C, 1D, 1E, 1F, 1G, 1H)dnl
target(all, pty, 0, 1, 2)dnl
target(all, bpf, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9)dnl
target(all, tun, 0, 1, 2, 3)dnl
target(all, xy, 0, 1, 2, 3)dnl
target(all, rd, 0)dnl
target(all, cd, 0, 1)dnl
target(all, sd, 0, 1, 2, 3, 4)dnl
target(all, vnd, 0, 1, 2, 3)dnl
target(all, ccd, 0, 1, 2, 3)dnl
target(ramd, sd, 0, 1, 2, 3, 4)dnl
target(ramd, wd, 0, 1, 2, 3, 4)dnl
target(ramd, st, 0, 1)dnl
target(ramd, cd, 0, 1)dnl)dnl
target(ramd, rd, 0)dnl
target(ramd, tty0, 0, 1)dnl
target(ramd, pty, 0)dnl
