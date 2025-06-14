/*
	mon.c
	8080 Machine Interpreter
		   by
		S.Takeoka

	Jan/04/1992
	Aug/18/1987
	Aug/15/1987
*/
#include "copying.h"

#include <stdio.h>
#include <setjmp.h>
#include <ctype.h>

#ifdef LINUX
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <termio.h>
#include <errno.h>
#endif /* LINUX*/


#include "i80.h"
#include "bios.h"

extern void mon();
extern void mem();
extern void read_file();
extern void read_n_file();

/*	*/

jmp_buf	jbuf;

int	itrace;

BYTE	*break_point;


/*	print CPU status	*/

pr_stat()
{
	printf(" %02x",opecode);
	printf(" PC=%04X,SP=%04X,[SP]=%04X",
		p_c-space,s_p-space,*s_p | (*(s_p+1)<<8));
	dmp_reg();
	dmp_flg();
#ifdef	TRACE
	pr_tr();
	pr_bp();
#endif	/*TRACE*/
}

pr_tr(){nputs(" trace=");if(itrace) nputs("ON"); else nputs("OFF");}

pr_bp()
{nputs(" bp=");
 if(break_point==0) nputs("NO");
 else printf("%04X",break_point-space);
}

pr_stat1()
{
	printf(" PC=%04X,SP=%04X,[SP]=%04X",
	       (p_c-space),(s_p-space),*s_p | (*(s_p+1)<<8));
}

/*	Debug Monitor	*/
monitor()
{
	setdown_tty(1);
	mon();
	setup_tty(1);
}

void
mon()
{char b[256];int x;
 itrace=1;
 p_c= space+0x100;  s_p= space+0x100;
 break_point = 0;
 if(x=setjmp(jbuf)){
	setdown_tty(1);
	nputs("\nBreaked");
	printf(" exception=%02x\n",x);
	pr_stat();
 }
 for(;;){
	nputs("\n?");gets(b);
	switch(*b){
		case 'g' : go();break;
		case '0' :
		case 'q' : return;
		case 'd' : dump();break;
		case 'm' : mem();break;
		case 'x' : pr_stat();break;
#ifdef	TRACE
		case 't' : itrace= !itrace; pr_tr();break;
		case 'b' : set_bp();break;
		case 'n' : exqt(p_c,s_p);break;
#endif

		case 'l' : load_sys(0xE380); break;

		case 'R' : read_file();break;
		case 'r' : read_n_file();break;
		case '?' : help(); break;
		default  : ;
	}
 }
}

help()
{
 disp_version();
 nputs(" by Shozo TAKEOKA\n");
 nputs("\n\tg: Go,\tq: Quit(return to CCP),\t\n");
 nputs("\td: Dump mem.\tm: Modify mem.\tx: Dump reg.\n");
#ifdef	TRACE
 nputs("\tt: Toggle tarce,\tb: Set break-pointer,\tn: continue\n");
#endif
 nputs("\tR: read Hex file into memory at (space+0x100)\n");
 nputs("\tr: read raw file into memory at (space+0x100)\n");
 nputs("\t0: continue CP/M (like as 'G0')\n");
 nputs("\t?: display this\n");
}


dump()
{int adr,i,j;char b[256];
 nputs("dump adrs ?");gets(b);
 sscanf(b,"%x",&adr);
 for(i=0;i<128;i+=16){
   printf("\n%04x  : ",adr+i);
   for(j=0;j<16;j++){
	printf("%02x ",space[adr+i+j]);
   }
   for(j=0;j<16;j++){
	printf("%c",isprint(space[adr+i+j]) ? space[adr+i+j]:'.');
   }
 }
}

void
mem()
{char b[256];int a,n;
 printf("mem adrs ?");gets(b);
 sscanf(b,"%x",&a);
 for(;;){
   printf("%04x : ",a);
   gets(b);
   if(strcmp(b,"q")==0) return;
   sscanf(b,"%x",&n);
   space[a++]=n;
 }
}

set_bp()
{char b[256];int a;
 printf("break adrs (0=clear BP) ?");gets(b);
 sscanf(b,"%x",&a);
 if(a==-1) break_point= 0;
 break_point= &space[a];
}
 
go()
{char b[256];int a;
 printf("Go adrs ?");gets(b);
 sscanf(b,"%x",&a);
 setup_tty(1);
 exqt(space+a,space+0x100);
}
 
dmp_reg()
{
 printf("   bc=%04x ",wreg(0));
 printf("de=%04x ",wreg(1));
 printf("hl=%04x ",wreg(2));
 printf(".a=%04x ",wreg(3));
}

dmp_flg()
{
 printf(" %c",c_y? 'C' : 'c');
 printf("%c",s_flg_test(lst_acc)? 'M' : 'P');
 printf("%c",z_flg_test(lst_acc)? 'Z' : 'z');
}

/*	read Hex file (Hex is NOT Intel format)	*/

void
read_file()
{FILE *f; int i,c;char fn[256],b[255];
 nputs("file name ?"); gets(fn);
 if((f=fopen(fn,"r"))==NULL){ nputs("can not read"); return;}
 fgets(b,255,f);
 i= 0x100;
 for(;;){
	if((c=getc(f))==-1 || i >= 0x10000) break;
	if(c<' ')continue;
	space[i++]= (bin(c)<<4) | bin(getc(f));
 }
 fclose(f);
 printf("save %d %s\n",(i-1)/0x100,b);
} 

/*	read raw file	*/
 
void
read_n_file()
{char fn[256];int i;
 nputs("raw file name ?"); gets(fn);
 if((i=load_file(fn))== -1){ nputs("can not read"); return;}
 space[i++] = 0x1a;
 printf("save %d\n",((i-2)/0x100)-1);
}

load_file(fn)
char *fn;
{FILE *f; int i,c;
 if((f=fopen(fn,"r"))==NULL){ return -1;}
 i= 0x100;
 for(;;){
	if((c=getc(f))==-1 ) break;
	space[i++]= c;
 }
 fclose(f);
 return i;
} 
 

/* save file */
save_file(fn,nrec)
char *fn;
int nrec;
{FILE *f; int i;

 if((f=fopen(fn,"w"))==NULL){ return -1;}
 i= 0x100;
 for(i=0x100;i<nrec*256+0x100;){
	if(fputc(space[i++],f)==-1 ) break;
 }
 fclose(f);
 return i;
} 
 
