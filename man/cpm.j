
			c p m

		8080 CPU シミュレータ + CP/M シミュレータ

				1998/Jul/03 竹岡 尚三@(株)アックス

 0. はじめに

  cpmは8080 CPU実行のシミュレーションと、CP/M Ver2.2の機能のシミュレート
を行ないます。
  このシステムは、基本的に CP/M の下で動くソフトウェアを、動作させる事を
目的としています。が、モニターから機械語を入力して遊ぶ事も可能です。

  このシミュレータでは、CP/M のドライブを一つの UNIX ディレクトリに割り
当てます。
  ドライブ名と UNIX ディレクトリの関係は、コンフィギュレーション・ファイ
ルに記述します。
  ただし、ディレクトリはccpの組み込みコマンド「cd」で任意に移動可能です。

  Submit は CP/M Ver2.2 と完全コンパチブルです。CP/M Ver2.2の submit.com
コマンドを使用して下さい。
  ただし、xsub のメカニズムが違うので、xsub.com はおまけについているもの
を必ず使用してください。




 1. 使い方

	cpm [-t][-b BIOS_FILE][-f CONF_FILE]

  オプション

	t : ターミナルがルーズである指定。config_file の "loose_term" と同じ。

	b : bios のファイル名を指定します。

	f : config_file を指定する。
	    config_file 名は、オプション f の指定が無い時は、
	    環境変数 CPM_CONFIG から得ようとし、それもない場合は、デフォー
	    ルトである "/usr/local/lib/cpmconf" が使われます。




 2.  コンフィギュレーション・ファイル

  コンフィギュレーション・ファイルにより、CP/M のドライブ名とUNIXディレ
クトリの関係を指定します。(ドライブ1台を1つのUNIXディレクトリとしていま
す。)
  また、コンフィギュレーション・ファイルには CP/M の以下のような環境設定
が記述できます。
  コンフィギュレーションの各エントリは１行につき一つです。
  各エントリ行の先頭にはスペースを開けないで下さい。
  先頭が ";" で始まる行はコメントになります。

1) ドライブ指定

	ドライブ名:	ディレクトリ・パス名

   と、いうぐあいに書きます。パス名はフルパスで書いて下さい。


2) 環境設定


 (a) loose_term
	「ターミナルがルーズ」である、指定。
	この指定があると、sleep(1) を実行してから tty のモードを切り替え
	ます。(rlogin などが、ルーズな時に指定して下さい)
	コマンド・ラインで -t が指定された場合、そちらが優先されます。

 (b) bios  パス名
	BIOS ファイルの指定。
	UNIX のファイル名を指定します。
	指定がない場合は、デフォールトである "bios64.sys" が使われます。
	コマンド・ラインで -b が指定された場合、そちらが優先されます。

 (c) com_path	パス名
	CCP が COM ファイルを捜すための、ディレクトリ指定。
	CCP がカレント・ドライブを捜して、COMファイルが見つからない場合、
	次に自動的に捜しに行くディレクトリを指定します。指定は１つのみで
	す。
	指定がない場合は、どこにも捜しに行きません。
	CCP の path コマンドで設定を、参照、変更できます。

 (d) lib_path	パス名
	BDOS Function#15 のファイルのopenに失敗した時、自動的にファイル
	を捜しに行くディレクトリの指定。指定は１つのみです。

 (e) start_up	'コマンド・ライン'
	cpm 起動時に、コマンド・ラインを、実行する。
	（必ず"'"で囲んで下さい）

 (f) lib_srch  0/1
	BDOS Function#15 に失敗した時、lib_path を使って、
	ファイルを捜す(1)、否(0)、を設定する。

 (g) verbose	0/1
	サポートされていない、BDOS Function, BIOS Function が呼び出され
	た時に、コンソールに通知する(1)、否(0)、を設定する。

 (h) ccp_line_upper	0/1
	CCPで入力された、コマンド・ラインをすべて大文字に変換する(1)、
	否(0)、を設定する。

 (i) file_name_low	0/1
	BDOS Functionのファイル操作時、ファイル名中のすべての英文字を、
	小文字に変換してから、BDOS Functionを実行する(1)、否(0)、を設定
	する。

3) コンフィギュレーション・ファイル記述例

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


4)  端末について

  ターミナル(実際は rlogin)がルーズであると、tty のモード切り替え時に
tty への出力がフッラシュされない事があります。それを、防ぐため、
loose_term を指定すると、tty のモード切り替え時に sleep(1) を実行します。

  cpmはadm3相当の端末エミュレーション機能があります。UNIXでいかなる端末
を使用していても、 cpm下のCP/Mアプリケーションが出力するadm3のエスケープ
シーケンスをcpmが解釈して、UNIX端末の制御に変換してくれます。

  cpmが解釈可能なadm3の制御シーケンスは

	^Z (CTRL-Z): 画面消去
	^]* (ESC *): 画面消去
	^]=yx (ESC = y x) : カーソル移動(x,yは0x20のオフセットが加わります)

  のみです。

  Word Masterなどのソフトウェアは端末コードを「adm3」としてコンフィギュ
レーションして使用してください。




 3. 8080 CPU シミュレータ (Ver.1.1)

  8080 シミュレータは、Cで記述し、なおかつ高速で動くように、考えてコーディ
ングされています。
  Make 時にいくつかの選択を行なうことが可能です。

1) Make 時に指定する要素

 (a) -DBIG_ENDIAN  /  -DLITTLE_ENDIAN

  必ず、どちらかを指定して下さい。
  自分のマシンが BIG_ENDIAN か、LITTLE_ENDIAN かを確認して、どちらかを
  -D で指定して下さい。
	BIG_ENDIAN    は 680?0, Sparc
	LITTLE_ENDIAN は VAX, 32?32, 80?86, PDP11, 8080
  です。

 (b) -DTRACE

  機械語モニターで、トレース、BreakPoint 機能を使うか否か。
  機械語モニターでトレースを使いたい人は -DTRACE として下さい。ただし、
トレース可能にすると、Break Point と Trace Flag のチェックが入り、オーバー
ヘッド多少増えます。

 (c) -DPARITY

  パリティ・フラグの機能を使うか否か。
  Parity Flag を使いたい人は、-DPARITY として下さい。パリティ・フラグは
psw の push, pop に、オーバーヘッドを増やします。CP/M下で稼働するアプリ
ケーションのほとんどは、i8080,Z80 自動判定の用途以外、パリティ・フラグを
使用しません。


2)  機械語モニター

  シミュレータのデバッグ用に非力で使い難いモニターがあります。:-)
  CCP のコマンド・ラインで mon と、入力する事で、モニターに入れます。

 (a)  モニター・コマンド表

	g: Go
		指定番地へジャンプして実行を始める。
	q: Quit
		モニターを終わり、CCP へもどる。(0と同じ）
	d: Dump memory
		メモリの内容をダンプする。
	m: Modify memory
		メモリの内容を変える。
	x: Dump register
		レジスタをダンプする。
	R: read Hex file into memory at (space+0x100)
		hex.com フォーマットのファイルを１００Ｈ番地から読み込む。
	r: read raw file into memory at (space+0x100)
		ファイルをそのまま、ファイルを１００Ｈ番地から読み込む。
	0: continue CP/M
		CP/MをWarmBootして続ける。（qと同じ）
	?: help
		ヘルプを表示する。

  ＊以下のコマンドはトレースつきでmakeした時のみ使えます。

	t: Toggle tarce
		トレースを on/off します。
	b: Set break-pointer
		BreakPoint のセット。(BP は一つだけ)
	n: continue
		Breakの続きから再開


3)  8080 CPU シミュレーションに関する制限

 (a) PC, SP

  この 8080 シミュレータは速さを増すため、プログラム・カウンタとスタック・
ポインタのオーバ(アンダー)ランの検出をしていません。よって、自殺プログラ
ムなどは動かない事があります。

 (b) Parity Flag

  8080 Ver1.0では、パリティー・フラグは使われないであろうと考え、インプ
リメントしていませんでした。
  8080 Ver1.1では、パリティー・フラグもサポートしました。
  Make時に、PARITY を #define すると、パリティー・フラグが使えます。ただ
し、 psw の push, pop 時にオーバーヘッドが増えますから、もし、pswの
push, popを少しでも、速くしたい場合には、PARITY なしで、make されること
を、お勧めします。

 (c) EI, DI, OUT, IN 命令

  意味を持ちません。(NOPと同じです）

 (d) HLT 命令

  8080 シミュレータでは、emt命令(pdp11における、ソフトウェア・インタラ
プト命令の事)としています。
  0x76 直後の 1Byte で、emtの種類を判断します。
  emtの種類は、emt.hを見て下さい。

4)速度

  SUN-3/260 では、4MHz の Z80 よりも、少し速い程度です。
  くれぐれも Dhrystone値 3000 以下の機械で実行しないようにして下さい。:-)




4. CP/M シミュレータ

0)
  CP/M シミュレータは、BDOS 部と、CCP 部に分かれます。
  CCP 部は、非常にいい加減な作りです。誰かの書き換えを期待します。
  BDOS 部はCP/M Ver2.2 とのコンパチビリティを、かなり実現しましたが、駄
目な部分も残っています。


1) ファイル名
  cpm ではファイル名に "." が、含まれていないと、扱えません。
 （"foo" というようなファイル名は、"foo." として、操作されます）
  また、ワイルドカードの扱いが不完全です。"?"が UNIX 流（どれかの１文字。
０文字にはならない）に解釈されます。したがって、"????????.???"が、何かの
ファイル名にマッチするのは、非常にまれなことになっています。;-< が、実用
上、あまり、差し支えはないようです。


2) CCP ビルトイン・コマンド

  CCP には、ビルトイン・コマンドがあります。

 (a)	UNIX
	cpm を終了する。

 (b)	MON
	機械語モニターへ入る。

 (c)	PWD
	現在の UNIX における、ワーキング・ディレクトリを表示。

 (d)	PATH	[new-path]
	COM ファイルを捜して、見つからない時に、自動的に見に行くディレク
	トリを new-path とする。
	new-path が指定されていない時は、現在のサーチ・パスを表示する。

 (e)	TYPE	file-name
	環境変数 PAGER を捜して、それがあればその内容を UNIXコマンドとし
	て呼び出す。その引数に、file-name を渡す。

 (f)	DIR	[arg1] [arg2]
	UNIXコマンド ls を呼び出す。;-P :-)

 (g)	SAVE	num  file-name
	CP/M の save と同じ。

 (h)	REN	file1=file2
	CP/M の ren と同じ。ただし、UNIX の mv を呼び出すので、file1 と
	file2 のドライブ指定が、違っても mv と同様に動作する。

 (i)	ERA	file1
	CP/M の era と同じ。

 (j)	USER	num
	CP/M の user とほぼ同じ。ただし、BDOS が、user を有効にサポート
	していないので、ほとんど意味がない。

 (k)	stop
	CCP コマンド・ライン入力時、入力文字がない状態で、"^Q"を入力する
	事によって、cpm を stop(suspend) させることが、できます。


3) CCP の環境

  CCP のコマンド・ラインには、BDOS Function#10 と同じ、行編集機能がつい
ています。
  CCP のコマンド・ラインを、入力直後にすべて大文字にするか、何もしないか
をコンフィギュレーション・ファイルの ccp_line_upper で指定できます。
CP/M アプリケーションの中には、コマンド・ラインからの引数が大文字でなけ
れば、正しく動作しないものも、いくつかあります。
  ビルトイン・コマンドがファイル名を解釈する時、UNIX の shell に解釈させ
ますから、 "?" の扱いが、違っています。

4) BDOS

  BDOS シミュレータは、CP/M Ver2.2 の BDOS Function を UNIX のシステム・
コールに変換します。

 (a) ファイル・システムについて

  各ドライブは UNIX のディレクトリに割り当てられます。

  ファイル名の中の "?" は CP/M と扱いが異なります。（"?" は何かの1文字と
マッチし、0文字とはマッチしない）
  BDOS Functionのファイル操作時(#23,#19,#15,#22,#17)、ファイル名中のすべ
ての英文字を、小文字に変換してから、BDOS Functionを実行するかどうかを、
コンフィギュレーション・ファイルの file_name_low で、指定できます。

  また、現在、ファイルは同時に CPM_MAX_FILES 個しかオープンできません（
CPM_MAX_FILES は bdos.h 中で、 #define されています）。
  CP/M のコンベンションでは、 read のみのファイルはクローズしなくてよい
ことに、なっているので、１つのアプリケーションを長い間使っていると、ファ
イルがオープンできなくなることがあります。そうなった、場合は
ResetDiskSystem(BDOS Function#13)を行なうか、アプリケーションを終るしか
ありません。
  ResetDiskSystem を行なうか、CCP へ戻ると、オープンされていたファイルは
すべてクローズされます。


 (b) GetString (BDOS Function#10) について

  BDOS Function#10 に行編集機能がついています。TAB はサポートしていませ
ん（^Iと表示されます）。
  Make 時、キーバインドを、次のように指定してください。
	-DEMACS	   : Emacs 風
	-DWORDSTAR : WordStar 風
	-DCPM3	   : CP/M3 風

  編集機能とキーバインド一覧

	編集機能: Emacs	: WordStar : CP/M3
	Forward : ^F	: ^D	   : ^F
	Backward: ^B	: ^S	   : ^A
	前行	: ^P	: ^E	   : ^R
	DelChar : ^D	: ^G	   : ^G
	Kill	: ^K	: ^K	   : ^K
	CAN	:	^X , ^U

	行頭での ^C は、reboot。行頭での ^Q は、Stop。


 (c) Open File (BDOS Function#15) について

  ファイルのオープンに失敗した場合、コンフィギュレーション・ファイルで指
定された、lib_path を自動的にサーチして、オープンする機能がついています。
lib_srch が 0 の場合、この機能は、働きません。

 (d) User (BDOS Function#32) について

  Userは正しく動きますが、ファイル・システム上では、なんの意味も持たせて
いないので、ほとんど無意味です。

 (e) サポートしていない BDOS Function
	#24 GetLoginVector	: fake. 現在使用できるドライブ全部を1とする
	#27 GetAllocationVector	: fake. BIOS 中の、偽情報領域を返す
	#28 Protect Disk	: NOP. なにもしない
	#29 Get RO Vector	: fake. 0 を返す
	#30 FileAttribute	: NOP. なにもしない
	#31 Get DPB Area	: fake. BIOS より偽の情報を得て、それを返す

 (f) 拡張した BDOS Function
	#255 set xsub flag	: E-reg の値を xsub flag にセットする
				  1: xsub 有効 / 0: xsub 無効。

5) BIOS

  サポートしていない BIOS Function
	#0	cboot	: Trap されて、CCP にもどる
	#5	list	: NOP
	#8	home	: NOP
	#9	seldsk	: fake. with bios64.sys
	#10	settrk	: NOP
	#11	setsec	: NOP
	#13	read	: NOP
	#14	write	: NOP
	#15	listst	: fake. always ready
	#16	sectran	: NOP

  ただし、seldsk は bios64.sys 中の偽のディスク情報を返します。
   bios64.sys では、すべてのドライブについて、8M Bytes のディスクの情報
を返します。また、Allocation 情報を格納する領域は 1ドライブ分しか用意し
ていないので、注意が必要です。


6) SUBMIT について

  Submit は CP/M Ver2.2 と完全コンパチブルです。CP/Mの submit.com コマン
ドを使用して下さい。
  ただし、xsub のメカニズムが違うので、xsub.com はおまけについているもの
を使用してください。
  このシステムでは、submitファイルは、/tmp/$$$.sub.PID（PID はプロセス番
号）となりますが、BDOS Function のなかで、すべての $$$.sub 操作を、
/tmp/$$$.sub.PID に対する操作に置き換えているので、BDOS Call を使用する
プログラムなら、すべてうまく動作するはずです。


7) CP/M のシリアル番号について

  bios.h の中で SERNO に CP/M のシリアル番号を設定しています。
  MOVCPM.COMは、このシリアル番号を参照して誤っている場合は動かなくなりま
す。MOVCPM.COMを使用する場合は、お持ちのCP/M のシリアル番号を SERNO に設
定した後、bios.c を make しなおして下さい。
  なお、MOVCPM.COM にパッチをあてて動かす方法もあるようです。



--- End of cpm.man
