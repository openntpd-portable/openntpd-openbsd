vers(__file__,
	{-$OpenBSD: MAKEDEV.md,v 1.8 2002/05/16 21:11:20 miod Exp $-},
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
_DEV(local)
_TITLE(tap)
_DEV(ct, 7, 0)
_DEV(mt, 16, 1)
_DEV(st_hp300, 20, 6)
_TITLE(dis)
_DEV(ccd, 17, 5)
_DEV(hd, 9, 2)
_DEV(sd, 8, 4)
_DEV(vnd, 19, 6)
_DEV(rd, 34, 8)
_TITLE(termp)
_DEV(dca, 12)
_DEV(dcm, 15)
_DEV(apci)
_TITLE(pty)
_DEV(tty, 4)
_DEV(pty, 5)
_TITLE(prn)
_DEV(ppi, 11)
_TITLE(call)
_TITLE(spec)
_DEV(fdesc, 21)
_DEV(grf, 10)
_DEV(ite)
_DEV(hil, 14)
_DEV(bpf, 22)
_DEV(tun, 23)
_DEV(pf, 33)
_DEV(lkm, 24)
_DEV(rnd, 32)
_DEV(xfs, 51)
_DEV(altq, 52)
_DEV(systrace, 50)
dnl
divert(7)dnl
dnl
ramdisk)
	_recurse std ct0 ct1 st0 st1 hd0 hd1 hd2 hd3 hd4
	_recurse sd0 sd1 sd2 sd3 sd4 rd0 pty0
	_recurse hil grf0 apci0 ite0 dca0 dcm0 dcm1
	_recurse bpf0 bpf1 tun0 tun1 lkm random
	;;

_std(1, 2, 36, 3, 6)
	;;

dca*)
	case $U in
	0|1|2|3)
		M tty$U c major_dca_c $U 660 dialer uucp
		M cua$U c major_dca_c Add($U, 128) 660 dialer uucp
		;;
	*)
		echo bad unit for dca in: $i
		;;
	esac
	;;

dcm*)
	case $U in
	0|1|2|3)
		u="$(( $U * 4 ))"
		i=0
		while [ $i -lt 4 ]
		do
			n="$(( $u + $i ))"
			ext=`hex $n`

			M tty0${ext} c 15 ${n} 660 dialer uucp
			M cua0${ext} c 15 "$(( $n + 128 ))" 660 dialer uucp

			i="$(( $i + 1 ))"
		done
		;;
	*)
		echo bad unit for dcm in: $i
		;;
	esac
	;;

apci*)
	# There exists only one Frodo ASIC per HP9000/400 SPU.
	case $U in
	0)
		for i in 0 1 2 3; do
			M ttya${i} c 35 ${i} 660 dialer uucp
			M cuaa${i} c 35 Add($i, 128) 660 dialer uucp
		done
		;;
	*)
		echo bad unit for apci in: $i
		;;
	esac
	;;

ppi*)
	case $U in
	0|1|2|3)
		M ppi$U c major_ppi_c $U 600
		;;
	*)
		echo bad unit for ppi in: $i
		;;
	esac
	;;

ite*)
	case $U in
	0|1|2|3)
		M ttye$U c 13 $U 600
		;;
	*)
		echo bad unit for ite in: $i
		;;
	esac
	;;

grf*)
	case $U in
	0|1|2|3)
		M grf$U c major_grf_c $U
#		echo "grf$U: use MAKEDEV.hpux to make HP-UX names for device"
		;;
	*)
		echo bad unit for grf in: $i
		;;
	esac
	;;

hil*)
	for U in 0 1 2 3 4 5 6 7
	do
		M hil$U c 14 $U
	done
	MKlist="$MKlist;ln hil1 keyboard"
	MKlist="$MKlist;ln hil3 locator"
	RMlist="$RMlist keyboard locator"
	;;
