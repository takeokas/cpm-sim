# cpm-sim , CP/M and i8080 CPU Simulator  
# i8080 CPU シミュレータ + CP/M エミュレータ  
##		1998/Jun/03 竹岡 尚三(Shozo TAKEOKA)
## このシステムは8080 CPUをソフトウェアでシミュレートするプログラムと、CP/M Ver2.2をUNIX下でシミュレートするプログラムの組み合わせです。

## このソフトウェアは、基本的にCP/Mの下で動くソフトウェアを、動作させる事を目的としています。が、モニターから機械語を入力して遊ぶ事も可能です。

 このシミュレータでは、CP/Mのドライブを一つのUNIXディレクトリに割り当てます。  
 ドライブ名とUNIXディレクトリの関係は、コンフィギュレーション・ファイルに記述します。詳しくは cpm.man のコンフィギュレーション・ファイルの項を見て下さい。  

  8080シミュレータ + CP/Mは cpm.man  
  CP/Mのインストールは install.doc  
  CP/M上のユーティリティは omake.man  

  にそれぞれ説明してあります。  

##  なお、CP/Mの標準コマンドやCP/Mそのもののソースは
	The unofficial CP/M WEB SITE  
	(http://cdl.uta.edu/cpm/)  
  に存在します。  

###  では、CP/M 80を久しぶりに満喫して下さい。  
			竹岡尚三 ( https://www.takeoka.org/~take/ )
