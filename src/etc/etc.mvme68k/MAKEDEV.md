vers(__file__,
	{-$OpenBSD: MAKEDEV.md,v 1.3 2002/04/17 22:53:37 miod Exp $-},
etc.MACHINE)dnl
dnl
dnl Copyright (c) 2002, Miodrag Vallat.
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
dnl *** mvme68k-specific devices
dnl
__devitem(mvme_tzs, ttya-d, standard serial ports)dnl
__devitem(mvme_czs, cuaa-d, standard call-up devices)dnl
_mkdev(mvme_tzs, {-tty[a-z]-}, {-u=${i#tty*}
	case $u in
	a) n=0 ;;
	b) n=1 ;;
	c) n=2 ;;
	d) n=3 ;;
	*) echo unknown tty device $i ;;
	esac
	M tty$u c major_mvme_tzs_c $n 660 dialer uucp-})dnl
_mkdev(mvme_czs, cua[a-z], {-u=${i#cua*}
	case $u in
	a) n=0 ;;
	b) n=1 ;;
	c) n=2 ;;
	d) n=3 ;;
	*) echo unknown cua device $i ;;
	esac
	M cua$u c major_mvme_czs_c Add($n, 128) 660 dialer uucp-})dnl
dnl tty00 not tty0 to prevent description conflict
__devitem(tty00, tty0*, CL-CD2400 serial ports)dnl
_mkdev(tty00, {-tty0*-}, {-u=${i#tty0*}
	case $u in
	0|1|2|3)
		M tty0$u c major_tty0_c $u 660 dialer uucp
		M cua0$u c major_tty0_c Add($u, 128) 660 dialer uucp
		;;
	*) echo unknown tty device $i ;;
	esac-})dnl
__devitem(ttyw, ttyw*, WG CL-CD2400 serial ports)dnl
_mkdev(ttyw, {-ttyw*-}, {-u=${i#ttyw*}
	case $u in
	0|1|2|3)
		M ttyw$u c major_ttyw_c $u 660 dialer uucp
		M cuaw$u c major_ttyw_c Add($u, 128) 660 dialer uucp
		;;
	*) echo unknown tty device $i ;;
	esac-})dnl
__devitem(lp, par0, Printer port)dnl
_mkdev(lp, {-lp*-}, {-u=${i#lp*}
	case $u in
	0) M par$u c major_lp_c $u 600;;
	*) echo unknown lp device $i ;;
	esac-})dnl
__devitem(sram, sram0, static memory access)dnl
_mkdev(sram, sram0, {-M sram0 c major_sram_c 0 640 kmem-})dnl
__devitem(nvram, nvram0, non-volatile memory access)dnl
_mkdev(nvram, nvram0, {-M nvram0 c major_nvram_c 0 640 kmem-})dnl
__devitem(flash, flash0, flash memory access)dnl
_mkdev(flash, flash0, {-M flash0 c major_flash_c 0 640 kmem-})dnl
__devitem(vmes, vmes0, VMEbus access)dnl
_mkdev(vmes, vmes0, {-M vmes0 c major_vmes_c 0 640 kmem-})dnl
__devitem(vmel, vmel0, VMEbus access)dnl
_mkdev(vmel, vmel0, {-M vmel0 c major_vmel_c 0 640 kmem-})dnl
dnl
dnl *** MAKEDEV itself
dnl
_TITLE(make)
dnl
dnl all)
dnl
target(all, sram, 0)dnl
target(all, nvram, 0)dnl
target(all, flash, 0)dnl
target(all, vmes, 0)dnl
target(all, vmel, 0)dnl
dnl
target(all, ses, 0)dnl
target(all, ch, 0)dnl
target(all, ss, 0, 1)dnl
target(all, xfs, 0)dnl
target(all, pty, 0, 1, 2)dnl
target(all, bpf, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9)dnl
target(all, tun, 0, 1, 2, 3)dnl
target(all, rd, 0)dnl
target(all, cd, 0, 1)dnl
target(all, sd, 0, 1, 2, 3, 4)dnl
target(all, ss, 0, 1)dnl
target(all, uk, 0)dnl
target(all, vnd, 0, 1, 2, 3)dnl
target(all, ccd, 0, 1, 2, 3)dnl
twrget(all, mvme_tzs, tty, a, b, c, d)dnl
twrget(all, mvme_czs, cua, a, b, c, d)dnl
target(all, tty00, 0, 1, 2, 3)dnl
target(all, ttyw, 0, 1, 2, 3)dnl
dnl target(all, lp, 0)dnl
_DEV(all)
dnl
dnl ramdisk)
dnl
target(ramd, sd, 0, 1, 2, 3, 4)dnl
target(ramd, st, 0, 1)dnl
target(ramd, cd, 0, 1)dnl)dnl
target(ramd, rd, 0)dnl
twrget(ramd, mvme_tzs, tty, a)dnl
target(ramd, pty, 0)dnl
_DEV(ramd)
dnl
_DEV(std)
_DEV(loc)
dnl
_TITLE(tap)
_DEV(st, 20, 7)
_TITLE(dis)
_DEV(sd, 8, 4)
_DEV(cd, 9, 8)
_DEV(ccd, 7, 5)
_DEV(vnd, 19, 6)
_DEV(rd, 18, 9)
_TITLE(term)
_DEV(mvme_tzs, 12)
_DEV(mvme_czs, 12)
_DEV(tty00, 13)
_DEV(ttyw, 30)
_TITLE(pty)
_DEV(tty, 4)
_DEV(pty, 5)
_TITLE(prn)
_DEV(lp, 28)
_TITLE(spec)
_DEV(uk, 41)
_DEV(ss, 42)
_DEV(fdesc, 21)
_DEV(bpf, 22)
_DEV(tun, 23)
_DEV(pf, 39)
_DEV(lkm, 24)
_DEV(rnd, 40)
_DEV(altq, 52)
_DEV(xfs, 51)
_DEV(sram, 7)
_DEV(nvram, 10)
_DEV(flash, 11)
_DEV(vmes, 32)
_DEV(vmel, 31)
dnl
divert(7)dnl
dnl
_std(1, 2, 43, 3, 6)
	;;

