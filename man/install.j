
		cpm $@%$%s%9%H!<%k%,%$%I(J

					1998/Jul/03 $@C]2,>0;0!w(J($@3t(J)$@%"%C%/%9(J


 1. Make $@$9$kA0$K(J

 1) Endian $@$N3NG'(J

  $@<+J,$N%^%7%s$,(J BIG_ENDIAN $@$+!"(JLITTLE_ENDIAN $@$+$r3NG'$7$F!"$I$A$i$+$r(J
  -D $@$G;XDj$7$F2<$5$$!#(J
	B_ENDIAN    $@$O(J 680x0, Sparc
	L_ENDIAN $@$O(J VAX, 32x32, 80x86, 8080
  $@$G$9!#(J


 2) $@5!3#8l%b%K%?!<$G!"%H%l!<%95!G=$r;H$&$+H]$+(J

  $@5!3#8l%b%K%?!<$G%H%l!<%9$r;H$$$?$$?M$O(J -DTRACE $@$H$7$F$*$$$F2<$5$$!#(J
$@$?$@$7!"%H%l!<%92DG=$K$9$k$H!"(JBreak Point $@$H(J Trace Flag$@$N%A%'%C%/$,F~(J
$@$j!"%*!<%P!<%X%C%IB?>/A}$($^$9!#(J


 3) $@%Q%j%F%#!&%U%i%0$N5!G=$r;H$&$+H]$+(J

  Parity Flag $@$r;H$$$?$$?M$O!"(J-DPARITY $@$H$7$F2<$5$$!#%Q%j%F%#!&%U%i%0(J
$@$O(Jpsw$@$N(Jpush,pop $@$K!"%*!<%P!<%X%C%I$rA}$d$7$^$9!#(JCP/M $@2<$G2TF/$9$k%"%W%j(J
$@%1!<%7%g%s$N$[$H$s$I$O!"(Ji8080,Z80$@<+F0H=Dj$NMQES0J30!"%Q%j%F%#!&%U%i%0(J
$@$r;HMQ$7$^$;$s!#(J

 4) GetString $@$N%-!<%P%$%s%I@_Dj(J

  BDOS Function#10 $@$N9TJT=85!G=$N%-!<%P%$%s%I$r;XDj$G$-$^$9!#(J
  $@%-!<%P%$%s%I$r!"<!$N$h$&$K;XDj$7$F$/$@$5$$!#(J
	-DEMACS	   : Emacs $@Iw(J
	-DWORDSTAR : WordStar $@Iw(J
	-DCPM3	   : CP/M3 $@Iw(J



 2.$@%7%9%F%`$NF0$+$7J}(J

 1) $@E83+$7$?%G%#%l%/%H%j$G(J
	% make
   $@$H$9$l$P!"(Jcpm $@$,$G$-$k!#(J

 2) BIOS$@$O4{$KMQ0U$7$F$"$k$N$G(J("bios64.sys")$@!"8f?4G[$J$/!#(J

 3) $@;n83E*$KF0$+$9$K$O!"(Jmake$@$7$?%G%#%l%/%H%j$G(J

	% cpm -b bios64.sys -f xconf

    $@$H$9$l$P!"(Jcpm$@$,5/F0$7$^$9!#(J
    $@=*N;$O(J
	A> unix
    $@$G$9!#(J

 4) $@%3%s%U%#%.%e%l!<%7%g%s!&%U%!%$%k!V(Jcpmconf$@!W$N%I%i%$%V$N%Q%9$r!"E,Ev(J
    $@$K@_Dj$7$F2<$5$$!#(J


 5) $@K\3JE*$K%$%s%9%H!<%k$9$k$K$O!"(Jroot$@$K$J$C$F!"(J
	# make install
    $@$H$7$^$9!#(J
    $@3F%U%!%$%k$,(J
	cpm	: /usr/local/bin/
	cpmconf : /usr/local/lib/
	bios64.sys : /usr/local/lib/
    $@%3%T!<$5$l$^$9!#(J

 6) $@0J9_(J
	% cpm
   $@$H$d$l$P!"(JCP/M $@$,5/F0$9$kH&$G$9!#(J

 7) $@3+H/$K;HMQ$9$k8@8l$d%W%m%8%'%/%H$4$H$K%3%s%U%#%.%e%l!<%7%g%s!&%U%!%$(J
   $@%k$r:n$C$F!"(Jcpm$@5/F0;~$K!V(J-f$@!W%*%W%7%g%s$G$=$l$r;XDj$9$k$H!"$+$J$jJXMx(J
   $@$G$9!#(J
   ($@%I%i%$%V;XDj$OI,$:%U%k%Q%9$G=q$$$F2<$5$$(J)


 8) $@4D6-JQ?t(J CPM_CONFIG $@$G%3%s%U%#%.%e%l!<%7%g%s!&%U%!%$%k$r;XDj$9$k$3$H(J
$@$b$G$-$^$9!#(J




 3.$@0\?"@-$K$D$$$F(J

  $@9bB.2=$N$?$a$K%;%3%$5;$r;H$$<c430\?"@-$rB;$M$F$$$^$9$,!"8=B8$9$k(J
 16/32 bit $@$N%W%m%;%C%5$J$iBgDqF0$/$h$&$K:n$C$F$"$j$^$9!#(J
  $@$^$?(J BIG_ENDIAN $@$G$b!"(J LITTLE_ENDIAN $@$G$b!"F0$/$h$&$K:n$C$F$"$j$^$9$+(J
$@$i!"%3%s%Q%$%k;~$K(J Makefile $@Cf$G;XDj$7$F2<$5$$!#(J

  $@;]$/F0:n$7$J$$;~$O(J BYTE,BYTE2,BYTE4 $@$N(J typedef $@$r3NG'$7$F2<$5$$!#(J
  ($@3NG'$7$F!"@dK>$9$k;v$K$J$C$F$b;d$O4XCN$7$^$;$s$,!D(J :-)

  $@$G$O!"(JCP/M $@$r0&MQ$7$F2<$5$$!#(J

			$@C]2,>0;0!w(J($@3t(J)$@%"%C%/%9(J

