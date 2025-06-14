# cpm-sim , CP/M and i8080 CPU Simulator  
# i8080 CPU シミュレータ + CP/M エミュレータ  
## 1998/Jun/03 竹岡 尚三(Shozo TAKEOKA, https://www.takeoka.org/~take/ )
### CP/M 80を久しぶりに満喫して下さい。  

## このシステムは8080 CPUをソフトウェアでシミュレートするプログラムと、CP/M Ver2.2をLinux/UNIX下でシミュレートするプログラムの組み合わせです。
### Windows WSL Ubuntuでも動作します。
## このソフトウェアは、基本的にCP/Mの下で動くソフトウェアを、動作させる事を目的としています。が、モニターから機械語を入力して遊ぶ事も可能です。

 
##  なお、CP/Mの標準コマンドやCP/Mそのもののソースは
	The unofficial CP/M WEB SITE  
	(http://cdl.uta.edu/cpm/)  
  に存在します。  

 8080シミュレータ + CP/Mは man/cpm.man  
  CP/Mのインストールは man/install.txt  
  CP/M上のユーティリティは man/omake.man  
  Windows WSL Ubuntuのメモ wsl-memo.txt  
  にそれぞれ説明してあります。  


# 8080シミュレータ + CP/M の終了方法
  A> UNIX  
  で、シミュレータは終了します。(Linuxのときも「UNIX」と入力すること)  

# フラグの遅延評価
  本8080CPU シミュレータは。下記のスライドの、P.12 に書いた、フラグの値が必要になるまで、計算を遅延させるような実装を行っています。  
  https://www.slideshare.net/slideshow/flag-iroiroa150605c1/49064393
