
			c p m

		8080 CPU $@%7%_%e%l!<%?(J + CP/M $@%7%_%e%l!<%?(J

				1998/Jul/03 $@C]2,(J $@>0;0(J@($@3t(J)$@%"%C%/%9(J

 0. $@$O$8$a$K(J

  cpm$@$O(J8080 CPU$@<B9T$N%7%_%e%l!<%7%g%s$H!"(JCP/M Ver2.2$@$N5!G=$N%7%_%e%l!<%H(J
$@$r9T$J$$$^$9!#(J
  $@$3$N%7%9%F%`$O!"4pK\E*$K(J CP/M $@$N2<$GF0$/%=%U%H%&%'%"$r!"F0:n$5$;$k;v$r(J
$@L\E*$H$7$F$$$^$9!#$,!"%b%K%?!<$+$i5!3#8l$rF~NO$7$FM7$V;v$b2DG=$G$9!#(J

  $@$3$N%7%_%e%l!<%?$G$O!"(JCP/M $@$N%I%i%$%V$r0l$D$N(J UNIX $@%G%#%l%/%H%j$K3d$j(J
$@Ev$F$^$9!#(J
  $@%I%i%$%VL>$H(J UNIX $@%G%#%l%/%H%j$N4X78$O!"%3%s%U%#%.%e%l!<%7%g%s!&%U%!%$(J
$@%k$K5-=R$7$^$9!#(J
  $@$?$@$7!"%G%#%l%/%H%j$O(Jccp$@$NAH$_9~$_%3%^%s%I!V(Jcd$@!W$GG$0U$K0\F02DG=$G$9!#(J

  Submit $@$O(J CP/M Ver2.2 $@$H40A4%3%s%Q%A%V%k$G$9!#(JCP/M Ver2.2$@$N(J submit.com
$@%3%^%s%I$r;HMQ$7$F2<$5$$!#(J
  $@$?$@$7!"(Jxsub $@$N%a%+%K%:%`$,0c$&$N$G!"(Jxsub.com $@$O$*$^$1$K$D$$$F$$$k$b$N(J
$@$rI,$:;HMQ$7$F$/$@$5$$!#(J




 1. $@;H$$J}(J

	cpm [-t][-b BIOS_FILE][-f CONF_FILE]

  $@%*%W%7%g%s(J

	t : $@%?!<%_%J%k$,%k!<%:$G$"$k;XDj!#(Jconfig_file $@$N(J "loose_term" $@$HF1$8!#(J

	b : bios $@$N%U%!%$%kL>$r;XDj$7$^$9!#(J

	f : config_file $@$r;XDj$9$k!#(J
	    config_file $@L>$O!"%*%W%7%g%s(J f $@$N;XDj$,L5$$;~$O!"(J
	    $@4D6-JQ?t(J CPM_CONFIG $@$+$iF@$h$&$H$7!"$=$l$b$J$$>l9g$O!"%G%U%)!<(J
	    $@%k%H$G$"$k(J "/usr/local/lib/cpmconf" $@$,;H$o$l$^$9!#(J




 2.  $@%3%s%U%#%.%e%l!<%7%g%s!&%U%!%$%k(J

  $@%3%s%U%#%.%e%l!<%7%g%s!&%U%!%$%k$K$h$j!"(JCP/M $@$N%I%i%$%VL>$H(JUNIX$@%G%#%l(J
$@%/%H%j$N4X78$r;XDj$7$^$9!#(J($@%I%i%$%V(J1$@Bf$r(J1$@$D$N(JUNIX$@%G%#%l%/%H%j$H$7$F$$$^(J
$@$9!#(J)
  $@$^$?!"%3%s%U%#%.%e%l!<%7%g%s!&%U%!%$%k$K$O(J CP/M $@$N0J2<$N$h$&$J4D6-@_Dj(J
$@$,5-=R$G$-$^$9!#(J
  $@%3%s%U%#%.%e%l!<%7%g%s$N3F%(%s%H%j$O#19T$K$D$-0l$D$G$9!#(J
  $@3F%(%s%H%j9T$N@hF,$K$O%9%Z!<%9$r3+$1$J$$$G2<$5$$!#(J
  $@@hF,$,(J ";" $@$G;O$^$k9T$O%3%a%s%H$K$J$j$^$9!#(J

1) $@%I%i%$%V;XDj(J

	$@%I%i%$%VL>(J:	$@%G%#%l%/%H%j!&%Q%9L>(J

   $@$H!"$$$&$0$"$$$K=q$-$^$9!#%Q%9L>$O%U%k%Q%9$G=q$$$F2<$5$$!#(J


2) $@4D6-@_Dj(J


 (a) loose_term
	$@!V%?!<%_%J%k$,%k!<%:!W$G$"$k!";XDj!#(J
	$@$3$N;XDj$,$"$k$H!"(Jsleep(1) $@$r<B9T$7$F$+$i(J tty $@$N%b!<%I$r@Z$jBX$((J
	$@$^$9!#(J(rlogin $@$J$I$,!"%k!<%:$J;~$K;XDj$7$F2<$5$$(J)
	$@%3%^%s%I!&%i%$%s$G(J -t $@$,;XDj$5$l$?>l9g!"$=$A$i$,M%@h$5$l$^$9!#(J

 (b) bios  $@%Q%9L>(J
	BIOS $@%U%!%$%k$N;XDj!#(J
	UNIX $@$N%U%!%$%kL>$r;XDj$7$^$9!#(J
	$@;XDj$,$J$$>l9g$O!"%G%U%)!<%k%H$G$"$k(J "bios64.sys" $@$,;H$o$l$^$9!#(J
	$@%3%^%s%I!&%i%$%s$G(J -b $@$,;XDj$5$l$?>l9g!"$=$A$i$,M%@h$5$l$^$9!#(J

 (c) com_path	$@%Q%9L>(J
	CCP $@$,(J COM $@%U%!%$%k$rA\$9$?$a$N!"%G%#%l%/%H%j;XDj!#(J
	CCP $@$,%+%l%s%H!&%I%i%$%V$rA\$7$F!"(JCOM$@%U%!%$%k$,8+$D$+$i$J$$>l9g!"(J
	$@<!$K<+F0E*$KA\$7$K9T$/%G%#%l%/%H%j$r;XDj$7$^$9!#;XDj$O#1$D$N$_$G(J
	$@$9!#(J
	$@;XDj$,$J$$>l9g$O!"$I$3$K$bA\$7$K9T$-$^$;$s!#(J
	CCP $@$N(J path $@%3%^%s%I$G@_Dj$r!";2>H!"JQ99$G$-$^$9!#(J

 (d) lib_path	$@%Q%9L>(J
	BDOS Function#15 $@$N%U%!%$%k$N(Jopen$@$K<:GT$7$?;~!"<+F0E*$K%U%!%$%k(J
	$@$rA\$7$K9T$/%G%#%l%/%H%j$N;XDj!#;XDj$O#1$D$N$_$G$9!#(J

 (e) start_up	'$@%3%^%s%I!&%i%$%s(J'
	cpm $@5/F0;~$K!"%3%^%s%I!&%i%$%s$r!"<B9T$9$k!#(J
	$@!JI,$:(J"'"$@$G0O$s$G2<$5$$!K(J

 (f) lib_srch  0/1
	BDOS Function#15 $@$K<:GT$7$?;~!"(Jlib_path $@$r;H$C$F!"(J
	$@%U%!%$%k$rA\$9(J(1)$@!"H](J(0)$@!"$r@_Dj$9$k!#(J

 (g) verbose	0/1
	$@%5%]!<%H$5$l$F$$$J$$!"(JBDOS Function, BIOS Function $@$,8F$S=P$5$l(J
	$@$?;~$K!"%3%s%=!<%k$KDLCN$9$k(J(1)$@!"H](J(0)$@!"$r@_Dj$9$k!#(J

 (h) ccp_line_upper	0/1
	CCP$@$GF~NO$5$l$?!"%3%^%s%I!&%i%$%s$r$9$Y$FBgJ8;z$KJQ49$9$k(J(1)$@!"(J
	$@H](J(0)$@!"$r@_Dj$9$k!#(J

 (i) file_name_low	0/1
	BDOS Function$@$N%U%!%$%kA`:n;~!"%U%!%$%kL>Cf$N$9$Y$F$N1QJ8;z$r!"(J
	$@>.J8;z$KJQ49$7$F$+$i!"(JBDOS Function$@$r<B9T$9$k(J(1)$@!"H](J(0)$@!"$r@_Dj(J
	$@$9$k!#(J

3) $@%3%s%U%#%.%e%l!<%7%g%s!&%U%!%$%k5-=RNc(J

;
;	cpm config. file
;

;loose_term

bios	/usr/take/src/cpm/bios64.sys

com_path /usr/local/lib/cpm/com/
lib_path /usr/local/lib/cpm/lib/

; environment flags
lib_srch	1
verbose		0
ccp_line_upper	1
file_name_low	1


; auto execute command on cold start
startup  'submit test'

; Drive setting
;d:	path_name

A:	.
B:	/home/take/cpm/

;--- end of Config.


4)  $@C<Kv$K$D$$$F(J

  $@%?!<%_%J%k(J($@<B:]$O(J rlogin)$@$,%k!<%:$G$"$k$H!"(Jtty $@$N%b!<%I@Z$jBX$(;~$K(J
tty $@$X$N=PNO$,%U%C%i%7%e$5$l$J$$;v$,$"$j$^$9!#$=$l$r!"KI$0$?$a!"(J
loose_term $@$r;XDj$9$k$H!"(Jtty $@$N%b!<%I@Z$jBX$(;~$K(J sleep(1) $@$r<B9T$7$^$9!#(J

  cpm$@$O(Jadm3$@AjEv$NC<Kv%(%_%e%l!<%7%g%s5!G=$,$"$j$^$9!#(JUNIX$@$G$$$+$J$kC<Kv(J
$@$r;HMQ$7$F$$$F$b!"(J cpm$@2<$N(JCP/M$@%"%W%j%1!<%7%g%s$,=PNO$9$k(Jadm3$@$N%(%9%1!<%W(J
$@%7!<%1%s%9$r(Jcpm$@$,2r<a$7$F!"(JUNIX$@C<Kv$N@)8f$KJQ49$7$F$/$l$^$9!#(J

  cpm$@$,2r<a2DG=$J(Jadm3$@$N@)8f%7!<%1%s%9$O(J

	^Z (CTRL-Z): $@2hLL>C5n(J
	^]* (ESC *): $@2hLL>C5n(J
	^]=yx (ESC = y x) : $@%+!<%=%k0\F0(J(x,y$@$O(J0x20$@$N%*%U%;%C%H$,2C$o$j$^$9(J)

  $@$N$_$G$9!#(J

  Word Master$@$J$I$N%=%U%H%&%'%"$OC<Kv%3!<%I$r!V(Jadm3$@!W$H$7$F%3%s%U%#%.%e(J
$@%l!<%7%g%s$7$F;HMQ$7$F$/$@$5$$!#(J




 3. 8080 CPU $@%7%_%e%l!<%?(J (Ver.1.1)

  8080 $@%7%_%e%l!<%?$O!"(JC$@$G5-=R$7!"$J$*$+$D9bB.$GF0$/$h$&$K!"9M$($F%3!<%G%#(J
$@%s%0$5$l$F$$$^$9!#(J
  Make $@;~$K$$$/$D$+$NA*Br$r9T$J$&$3$H$,2DG=$G$9!#(J

1) Make $@;~$K;XDj$9$kMWAG(J

 (a) -DBIG_ENDIAN  /  -DLITTLE_ENDIAN

  $@I,$:!"$I$A$i$+$r;XDj$7$F2<$5$$!#(J
  $@<+J,$N%^%7%s$,(J BIG_ENDIAN $@$+!"(JLITTLE_ENDIAN $@$+$r3NG'$7$F!"$I$A$i$+$r(J
  -D $@$G;XDj$7$F2<$5$$!#(J
	BIG_ENDIAN    $@$O(J 680?0, Sparc
	LITTLE_ENDIAN $@$O(J VAX, 32?32, 80?86, PDP11, 8080
  $@$G$9!#(J

 (b) -DTRACE

  $@5!3#8l%b%K%?!<$G!"%H%l!<%9!"(JBreakPoint $@5!G=$r;H$&$+H]$+!#(J
  $@5!3#8l%b%K%?!<$G%H%l!<%9$r;H$$$?$$?M$O(J -DTRACE $@$H$7$F2<$5$$!#$?$@$7!"(J
$@%H%l!<%92DG=$K$9$k$H!"(JBreak Point $@$H(J Trace Flag $@$N%A%'%C%/$,F~$j!"%*!<%P!<(J
$@%X%C%IB?>/A}$($^$9!#(J

 (c) -DPARITY

  $@%Q%j%F%#!&%U%i%0$N5!G=$r;H$&$+H]$+!#(J
  Parity Flag $@$r;H$$$?$$?M$O!"(J-DPARITY $@$H$7$F2<$5$$!#%Q%j%F%#!&%U%i%0$O(J
psw $@$N(J push, pop $@$K!"%*!<%P!<%X%C%I$rA}$d$7$^$9!#(JCP/M$@2<$G2TF/$9$k%"%W%j(J
$@%1!<%7%g%s$N$[$H$s$I$O!"(Ji8080,Z80 $@<+F0H=Dj$NMQES0J30!"%Q%j%F%#!&%U%i%0$r(J
$@;HMQ$7$^$;$s!#(J


2)  $@5!3#8l%b%K%?!<(J

  $@%7%_%e%l!<%?$N%G%P%C%0MQ$KHsNO$G;H$$Fq$$%b%K%?!<$,$"$j$^$9!#(J:-)
  CCP $@$N%3%^%s%I!&%i%$%s$G(J mon $@$H!"F~NO$9$k;v$G!"%b%K%?!<$KF~$l$^$9!#(J

 (a)  $@%b%K%?!<!&%3%^%s%II=(J

	g: Go
		$@;XDjHVCO$X%8%c%s%W$7$F<B9T$r;O$a$k!#(J
	q: Quit
		$@%b%K%?!<$r=*$o$j!"(JCCP $@$X$b$I$k!#(J(0$@$HF1$8!K(J
	d: Dump memory
		$@%a%b%j$NFbMF$r%@%s%W$9$k!#(J
	m: Modify memory
		$@%a%b%j$NFbMF$rJQ$($k!#(J
	x: Dump register
		$@%l%8%9%?$r%@%s%W$9$k!#(J
	R: read Hex file into memory at (space+0x100)
		hex.com $@%U%)!<%^%C%H$N%U%!%$%k$r#1#0#0#HHVCO$+$iFI$_9~$`!#(J
	r: read raw file into memory at (space+0x100)
		$@%U%!%$%k$r$=$N$^$^!"%U%!%$%k$r#1#0#0#HHVCO$+$iFI$_9~$`!#(J
	0: continue CP/M
		CP/M$@$r(JWarmBoot$@$7$FB3$1$k!#!J(Jq$@$HF1$8!K(J
	?: help
		$@%X%k%W$rI=<($9$k!#(J

  $@!v0J2<$N%3%^%s%I$O%H%l!<%9$D$-$G(Jmake$@$7$?;~$N$_;H$($^$9!#(J

	t: Toggle tarce
		$@%H%l!<%9$r(J on/off $@$7$^$9!#(J
	b: Set break-pointer
		BreakPoint $@$N%;%C%H!#(J(BP $@$O0l$D$@$1(J)
	n: continue
		Break$@$NB3$-$+$i:F3+(J


3)  8080 CPU $@%7%_%e%l!<%7%g%s$K4X$9$k@)8B(J

 (a) PC, SP

  $@$3$N(J 8080 $@%7%_%e%l!<%?$OB.$5$rA}$9$?$a!"%W%m%0%i%`!&%+%&%s%?$H%9%?%C%/!&(J
$@%]%$%s%?$N%*!<%P(J($@%"%s%@!<(J)$@%i%s$N8!=P$r$7$F$$$^$;$s!#$h$C$F!"<+;&%W%m%0%i(J
$@%`$J$I$OF0$+$J$$;v$,$"$j$^$9!#(J

 (b) Parity Flag

  8080 Ver1.0$@$G$O!"%Q%j%F%#!<!&%U%i%0$O;H$o$l$J$$$G$"$m$&$H9M$(!"%$%s%W(J
$@%j%a%s%H$7$F$$$^$;$s$G$7$?!#(J
  8080 Ver1.1$@$G$O!"%Q%j%F%#!<!&%U%i%0$b%5%]!<%H$7$^$7$?!#(J
  Make$@;~$K!"(JPARITY $@$r(J #define $@$9$k$H!"%Q%j%F%#!<!&%U%i%0$,;H$($^$9!#$?$@(J
$@$7!"(J psw $@$N(J push, pop $@;~$K%*!<%P!<%X%C%I$,A}$($^$9$+$i!"$b$7!"(Jpsw$@$N(J
push, pop$@$r>/$7$G$b!"B.$/$7$?$$>l9g$K$O!"(JPARITY $@$J$7$G!"(Jmake $@$5$l$k$3$H(J
$@$r!"$*4+$a$7$^$9!#(J

 (c) EI, DI, OUT, IN $@L?Na(J

  $@0UL#$r;}$A$^$;$s!#(J(NOP$@$HF1$8$G$9!K(J

 (d) HLT $@L?Na(J

  8080 $@%7%_%e%l!<%?$G$O!"(Jemt$@L?Na(J(pdp11$@$K$*$1$k!"%=%U%H%&%'%"!&%$%s%?%i(J
$@%W%HL?Na$N;v(J)$@$H$7$F$$$^$9!#(J
  0x76 $@D>8e$N(J 1Byte $@$G!"(Jemt$@$N<oN`$rH=CG$7$^$9!#(J
  emt$@$N<oN`$O!"(Jemt.h$@$r8+$F2<$5$$!#(J

4)$@B.EY(J

  SUN-3/260 $@$G$O!"(J4MHz $@$N(J Z80 $@$h$j$b!">/$7B.$$DxEY$G$9!#(J
  $@$/$l$0$l$b(J Dhrystone$@CM(J 3000 $@0J2<$N5!3#$G<B9T$7$J$$$h$&$K$7$F2<$5$$!#(J:-)




4. CP/M $@%7%_%e%l!<%?(J

0)
  CP/M $@%7%_%e%l!<%?$O!"(JBDOS $@It$H!"(JCCP $@It$KJ,$+$l$^$9!#(J
  CCP $@It$O!"Hs>o$K$$$$2C8:$J:n$j$G$9!#C/$+$N=q$-49$($r4|BT$7$^$9!#(J
  BDOS $@It$O(JCP/M Ver2.2 $@$H$N%3%s%Q%A%S%j%F%#$r!"$+$J$j<B8=$7$^$7$?$,!"BL(J
$@L\$JItJ,$b;D$C$F$$$^$9!#(J


1) $@%U%!%$%kL>(J
  cpm $@$G$O%U%!%$%kL>$K(J "." $@$,!"4^$^$l$F$$$J$$$H!"07$($^$;$s!#(J
 $@!J(J"foo" $@$H$$$&$h$&$J%U%!%$%kL>$O!"(J"foo." $@$H$7$F!"A`:n$5$l$^$9!K(J
  $@$^$?!"%o%$%k%I%+!<%I$N07$$$,IT40A4$G$9!#(J"?"$@$,(J UNIX $@N.!J$I$l$+$N#1J8;z!#(J
$@#0J8;z$K$O$J$i$J$$!K$K2r<a$5$l$^$9!#$7$?$,$C$F!"(J"????????.???"$@$,!"2?$+$N(J
$@%U%!%$%kL>$K%^%C%A$9$k$N$O!"Hs>o$K$^$l$J$3$H$K$J$C$F$$$^$9!#(J;-< $@$,!"<BMQ(J
$@>e!"$"$^$j!":9$7;Y$($O$J$$$h$&$G$9!#(J


2) CCP $@%S%k%H%$%s!&%3%^%s%I(J

  CCP $@$K$O!"%S%k%H%$%s!&%3%^%s%I$,$"$j$^$9!#(J

 (a)	UNIX
	cpm $@$r=*N;$9$k!#(J

 (b)	MON
	$@5!3#8l%b%K%?!<$XF~$k!#(J

 (c)	PWD
	$@8=:_$N(J UNIX $@$K$*$1$k!"%o!<%-%s%0!&%G%#%l%/%H%j$rI=<(!#(J

 (d)	PATH	[new-path]
	COM $@%U%!%$%k$rA\$7$F!"8+$D$+$i$J$$;~$K!"<+F0E*$K8+$K9T$/%G%#%l%/(J
	$@%H%j$r(J new-path $@$H$9$k!#(J
	new-path $@$,;XDj$5$l$F$$$J$$;~$O!"8=:_$N%5!<%A!&%Q%9$rI=<($9$k!#(J

 (e)	TYPE	file-name
	$@4D6-JQ?t(J PAGER $@$rA\$7$F!"$=$l$,$"$l$P$=$NFbMF$r(J UNIX$@%3%^%s%I$H$7(J
	$@$F8F$S=P$9!#$=$N0z?t$K!"(Jfile-name $@$rEO$9!#(J

 (f)	DIR	[arg1] [arg2]
	UNIX$@%3%^%s%I(J ls $@$r8F$S=P$9!#(J;-P :-)

 (g)	SAVE	num  file-name
	CP/M $@$N(J save $@$HF1$8!#(J

 (h)	REN	file1=file2
	CP/M $@$N(J ren $@$HF1$8!#$?$@$7!"(JUNIX $@$N(J mv $@$r8F$S=P$9$N$G!"(Jfile1 $@$H(J
	file2 $@$N%I%i%$%V;XDj$,!"0c$C$F$b(J mv $@$HF1MM$KF0:n$9$k!#(J

 (i)	ERA	file1
	CP/M $@$N(J era $@$HF1$8!#(J

 (j)	USER	num
	CP/M $@$N(J user $@$H$[$\F1$8!#$?$@$7!"(JBDOS $@$,!"(Juser $@$rM-8z$K%5%]!<%H(J
	$@$7$F$$$J$$$N$G!"$[$H$s$I0UL#$,$J$$!#(J

 (k)	stop
	CCP $@%3%^%s%I!&%i%$%sF~NO;~!"F~NOJ8;z$,$J$$>uBV$G!"(J"^Q"$@$rF~NO$9$k(J
	$@;v$K$h$C$F!"(Jcpm $@$r(J stop(suspend) $@$5$;$k$3$H$,!"$G$-$^$9!#(J


3) CCP $@$N4D6-(J

  CCP $@$N%3%^%s%I!&%i%$%s$K$O!"(JBDOS Function#10 $@$HF1$8!"9TJT=85!G=$,$D$$(J
$@$F$$$^$9!#(J
  CCP $@$N%3%^%s%I!&%i%$%s$r!"F~NOD>8e$K$9$Y$FBgJ8;z$K$9$k$+!"2?$b$7$J$$$+(J
$@$r%3%s%U%#%.%e%l!<%7%g%s!&%U%!%$%k$N(J ccp_line_upper $@$G;XDj$G$-$^$9!#(J
CP/M $@%"%W%j%1!<%7%g%s$NCf$K$O!"%3%^%s%I!&%i%$%s$+$i$N0z?t$,BgJ8;z$G$J$1(J
$@$l$P!"@5$7$/F0:n$7$J$$$b$N$b!"$$$/$D$+$"$j$^$9!#(J
  $@%S%k%H%$%s!&%3%^%s%I$,%U%!%$%kL>$r2r<a$9$k;~!"(JUNIX $@$N(J shell $@$K2r<a$5$;(J
$@$^$9$+$i!"(J "?" $@$N07$$$,!"0c$C$F$$$^$9!#(J

4) BDOS

  BDOS $@%7%_%e%l!<%?$O!"(JCP/M Ver2.2 $@$N(J BDOS Function $@$r(J UNIX $@$N%7%9%F%`!&(J
$@%3!<%k$KJQ49$7$^$9!#(J

 (a) $@%U%!%$%k!&%7%9%F%`$K$D$$$F(J

  $@3F%I%i%$%V$O(J UNIX $@$N%G%#%l%/%H%j$K3d$jEv$F$i$l$^$9!#(J

  $@%U%!%$%kL>$NCf$N(J "?" $@$O(J CP/M $@$H07$$$,0[$J$j$^$9!#!J(J"?" $@$O2?$+$N(J1$@J8;z$H(J
$@%^%C%A$7!"(J0$@J8;z$H$O%^%C%A$7$J$$!K(J
  BDOS Function$@$N%U%!%$%kA`:n;~(J(#23,#19,#15,#22,#17)$@!"%U%!%$%kL>Cf$N$9$Y(J
$@$F$N1QJ8;z$r!">.J8;z$KJQ49$7$F$+$i!"(JBDOS Function$@$r<B9T$9$k$+$I$&$+$r!"(J
$@%3%s%U%#%.%e%l!<%7%g%s!&%U%!%$%k$N(J file_name_low $@$G!";XDj$G$-$^$9!#(J

  $@$^$?!"8=:_!"%U%!%$%k$OF1;~$K(J CPM_MAX_FILES $@8D$7$+%*!<%W%s$G$-$^$;$s!J(J
CPM_MAX_FILES $@$O(J bdos.h $@Cf$G!"(J #define $@$5$l$F$$$^$9!K!#(J
  CP/M $@$N%3%s%Y%s%7%g%s$G$O!"(J read $@$N$_$N%U%!%$%k$O%/%m!<%:$7$J$/$F$h$$(J
$@$3$H$K!"$J$C$F$$$k$N$G!"#1$D$N%"%W%j%1!<%7%g%s$rD9$$4V;H$C$F$$$k$H!"%U%!(J
$@%$%k$,%*!<%W%s$G$-$J$/$J$k$3$H$,$"$j$^$9!#$=$&$J$C$?!">l9g$O(J
ResetDiskSystem(BDOS Function#13)$@$r9T$J$&$+!"%"%W%j%1!<%7%g%s$r=*$k$7$+(J
$@$"$j$^$;$s!#(J
  ResetDiskSystem $@$r9T$J$&$+!"(JCCP $@$XLa$k$H!"%*!<%W%s$5$l$F$$$?%U%!%$%k$O(J
$@$9$Y$F%/%m!<%:$5$l$^$9!#(J


 (b) GetString (BDOS Function#10) $@$K$D$$$F(J

  BDOS Function#10 $@$K9TJT=85!G=$,$D$$$F$$$^$9!#(JTAB $@$O%5%]!<%H$7$F$$$^$;(J
$@$s!J(J^I$@$HI=<($5$l$^$9!K!#(J
  Make $@;~!"%-!<%P%$%s%I$r!"<!$N$h$&$K;XDj$7$F$/$@$5$$!#(J
	-DEMACS	   : Emacs $@Iw(J
	-DWORDSTAR : WordStar $@Iw(J
	-DCPM3	   : CP/M3 $@Iw(J

  $@JT=85!G=$H%-!<%P%$%s%I0lMw(J

	$@JT=85!G=(J: Emacs	: WordStar : CP/M3
	Forward : ^F	: ^D	   : ^F
	Backward: ^B	: ^S	   : ^A
	$@A09T(J	: ^P	: ^E	   : ^R
	DelChar : ^D	: ^G	   : ^G
	Kill	: ^K	: ^K	   : ^K
	CAN	:	^X , ^U

	$@9TF,$G$N(J ^C $@$O!"(Jreboot$@!#9TF,$G$N(J ^Q $@$O!"(JStop$@!#(J


 (c) Open File (BDOS Function#15) $@$K$D$$$F(J

  $@%U%!%$%k$N%*!<%W%s$K<:GT$7$?>l9g!"%3%s%U%#%.%e%l!<%7%g%s!&%U%!%$%k$G;X(J
$@Dj$5$l$?!"(Jlib_path $@$r<+F0E*$K%5!<%A$7$F!"%*!<%W%s$9$k5!G=$,$D$$$F$$$^$9!#(J
lib_srch $@$,(J 0 $@$N>l9g!"$3$N5!G=$O!"F/$-$^$;$s!#(J

 (d) User (BDOS Function#32) $@$K$D$$$F(J

  User$@$O@5$7$/F0$-$^$9$,!"%U%!%$%k!&%7%9%F%`>e$G$O!"$J$s$N0UL#$b;}$?$;$F(J
$@$$$J$$$N$G!"$[$H$s$IL50UL#$G$9!#(J

 (e) $@%5%]!<%H$7$F$$$J$$(J BDOS Function
	#24 GetLoginVector	: fake. $@8=:_;HMQ$G$-$k%I%i%$%VA4It$r(J1$@$H$9$k(J
	#27 GetAllocationVector	: fake. BIOS $@Cf$N!"56>pJsNN0h$rJV$9(J
	#28 Protect Disk	: NOP. $@$J$K$b$7$J$$(J
	#29 Get RO Vector	: fake. 0 $@$rJV$9(J
	#30 FileAttribute	: NOP. $@$J$K$b$7$J$$(J
	#31 Get DPB Area	: fake. BIOS $@$h$j56$N>pJs$rF@$F!"$=$l$rJV$9(J

 (f) $@3HD%$7$?(J BDOS Function
	#255 set xsub flag	: E-reg $@$NCM$r(J xsub flag $@$K%;%C%H$9$k(J
				  1: xsub $@M-8z(J / 0: xsub $@L58z!#(J

5) BIOS

  $@%5%]!<%H$7$F$$$J$$(J BIOS Function
	#0	cboot	: Trap $@$5$l$F!"(JCCP $@$K$b$I$k(J
	#5	list	: NOP
	#8	home	: NOP
	#9	seldsk	: fake. with bios64.sys
	#10	settrk	: NOP
	#11	setsec	: NOP
	#13	read	: NOP
	#14	write	: NOP
	#15	listst	: fake. always ready
	#16	sectran	: NOP

  $@$?$@$7!"(Jseldsk $@$O(J bios64.sys $@Cf$N56$N%G%#%9%/>pJs$rJV$7$^$9!#(J
   bios64.sys $@$G$O!"$9$Y$F$N%I%i%$%V$K$D$$$F!"(J8M Bytes $@$N%G%#%9%/$N>pJs(J
$@$rJV$7$^$9!#$^$?!"(JAllocation $@>pJs$r3JG<$9$kNN0h$O(J 1$@%I%i%$%VJ,$7$+MQ0U$7(J
$@$F$$$J$$$N$G!"Cm0U$,I,MW$G$9!#(J


6) SUBMIT $@$K$D$$$F(J

  Submit $@$O(J CP/M Ver2.2 $@$H40A4%3%s%Q%A%V%k$G$9!#(JCP/M$@$N(J submit.com $@%3%^%s(J
$@%I$r;HMQ$7$F2<$5$$!#(J
  $@$?$@$7!"(Jxsub $@$N%a%+%K%:%`$,0c$&$N$G!"(Jxsub.com $@$O$*$^$1$K$D$$$F$$$k$b$N(J
$@$r;HMQ$7$F$/$@$5$$!#(J
  $@$3$N%7%9%F%`$G$O!"(Jsubmit$@%U%!%$%k$O!"(J/tmp/$$$.sub.PID$@!J(JPID $@$O%W%m%;%9HV(J
$@9f!K$H$J$j$^$9$,!"(JBDOS Function $@$N$J$+$G!"$9$Y$F$N(J $$$.sub $@A`:n$r!"(J
/tmp/$$$.sub.PID $@$KBP$9$kA`:n$KCV$-49$($F$$$k$N$G!"(JBDOS Call $@$r;HMQ$9$k(J
$@%W%m%0%i%`$J$i!"$9$Y$F$&$^$/F0:n$9$k$O$:$G$9!#(J


7) CP/M $@$N%7%j%"%kHV9f$K$D$$$F(J

  bios.h $@$NCf$G(J SERNO $@$K(J CP/M $@$N%7%j%"%kHV9f$r@_Dj$7$F$$$^$9!#(J
  MOVCPM.COM$@$O!"$3$N%7%j%"%kHV9f$r;2>H$7$F8m$C$F$$$k>l9g$OF0$+$J$/$J$j$^(J
$@$9!#(JMOVCPM.COM$@$r;HMQ$9$k>l9g$O!"$*;}$A$N(JCP/M $@$N%7%j%"%kHV9f$r(J SERNO $@$K@_(J
$@Dj$7$?8e!"(Jbios.c $@$r(J make $@$7$J$*$7$F2<$5$$!#(J
  $@$J$*!"(JMOVCPM.COM $@$K%Q%C%A$r$"$F$FF0$+$9J}K!$b$"$k$h$&$G$9!#(J



--- End of cpm.man
