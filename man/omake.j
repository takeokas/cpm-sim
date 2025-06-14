
○ CP/Mのおまけソフトウェア

  xsub
	このCP/Mエミュレータには、専用のxsubが必要です。
	標準のxsub.comと入れ換えて使って下さい。
	(submit は標準のものが使えます)

  sh [UNIX-COMMAND]
	引数に与えられたUNIXコマンドを実行します。引数は全て小文字に
	変換されます。引数がない場合は sh が起きます。


  bios64.asm
	bios64.sysのソース・ファイル。
	ディスクの情報が重要。
	ここでは、8MByteという最大級のディスクの情報を保持している。

  make64.sub
	bios64.sysをつくり出すsubmitファイル。
	hexというコマンドは付属しないが、ただ16進ダンプを作るだけのコマ
	ンド。


  pbas.asm
	PaloAlto Tiny BASICのソース。

  pbas.com
	超有名なPaloAlto Tiny BASIC。
	命令はすべて大文字で入力すること。
	BASICの終了はコマンドラインで SYSTEM と入力。
