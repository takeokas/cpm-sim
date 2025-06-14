# cpm-sim , CP/M and i8080 CPU Simulator  
# i8080 CPU シミュレータ + CP/M エミュレータ  
## 1998/Jun/03 竹岡 尚三(Shozo TAKEOKA, https://www.takeoka.org/~take/ )
## CP/M 80を久しぶりに満喫して下さい。  

## このシステムは8080 CPUをソフトウェアでシミュレートするプログラムと、CP/M Ver2.2をUNIX下でシミュレートするプログラムの組み合わせです。
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


# 8080シミュレータ + CP/M の使い方
 このシミュレータでは、CP/Mのドライブを一つのUNIXディレクトリに割り当てます。  
 ドライブ名とUNIXディレクトリの関係は、コンフィギュレーション・ファイルに記述します。詳しくは cpm.man のコンフィギュレーション・ファイルの項を見て下さい。  

 
