/*
	bios.c
	8080 Machine Interpreter
		   by
		S.Takeoka

	Jan/04/1992
	Aug/18/1987
	Aug/15/1987
*/
#include "copying.h"

#include <stdio.h>
#include <sys/ioctl.h>
#ifdef SYSV
#	include <sys/types.h>
#	include <fcntl.h>
#else
#	include <sys/file.h>
#endif
#include <setjmp.h>
#include <ctype.h>
#include <sys/param.h>

#ifdef MACOSX
#include <sys/select.h>
#endif

#ifdef LINUX
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <termio.h>
#include <errno.h>
#else /*!LINUX*/
extern	int	errno;
#endif /*!LINUX*/

#include "i80.h"
#include "env.h"
#include "bdos.h"
#include "bios.h"
#include "emt.h"

#define	DPH_BASE	0xfb00
#define	DPB_BASE	0xfe00

extern void bios_conout();
extern void bios_pun_open();
extern void bios_rdr_open();




int		drive,
		track,
		secter,
		dph;

char	con_buf[256];
int	con_b;

FILE	*pun,
	*rdr;


BYTE	*s=space;	/* for GDB */



/*	dispatch	*/
/*	emulate machine trap	*/
emt(x)
{
	switch(x){
	case Bios_cboot:
		bios_cboot();break;
	case Bios_wboot:
		bios_wboot();break;
	case Bios_const:
		bios_const();break;
	case Bios_conin:
		bios_conin();break;

	case Bios_conout:
		bios_conout();break; 
	case Bios_list:
		bios_list();break; 
	case Bios_punch:
		bios_punch();break; 
	case Bios_reader:
		bios_reader();break;

	case Bios_home:
		bios_home();break;
	case Bios_seldsk:
		bios_seldsk();break;
	case Bios_settrk:
		bios_settrk();break;
	case Bios_setsec:
		bios_setsec();break;

	case Bios_setdma:
		bios_setdma();break;
	case Bios_cpm_read:
		bios_cpm_read();break;
	case Bios_cpm_write:
		bios_cpm_write();break;
	case Bios_listst:
		bios_listst();break;
	/* 10 */
	case Bios_sectran:
		bios_sectran();break;
	/* 20 */
	case Bios_pun_open:
		bios_pun_open();break;
	case Bios_pun_close:
		bios_pun_close();break;
	/* 24 */
	case Bios_rdr_open:
		bios_rdr_open();break;
	case Bios_rdr_close:
		bios_rdr_close();break;
	/* 80 */
	case Bios_unix_open:
		bios_unix_open();break;
	case Bios_unix_close:
		bios_unix_close();break;
	case Bios_unix_read:
		bios_unix_read();break;
	case Bios_unix_write:
		bios_unix_write();break;
	/* 81 */
	case Bios_unix_system:
		bios_unix_system();break;
	/* FE,FF */
	case Emt_return_ccp:
		emt_return_ccp();break;
	case Emt_bdos:
		emt_bdos();break;
	}
}


/*
	cold boot loader
*/

init_cpm()
{int i,x,dph,dpb;
 space[0]=0xc3;	/* jmp 0fa03h */
 space[1]= BIOS_ENT & 0xFF;
 space[2]= BIOS_ENT >>8;

 space[3]=0;	/* iobyte	*/
 space[4]=0;	/* drive = A: */

 space[5]=0xc3;	/* jmp 0ec06h */
 space[6]= BDOS_TOP & 0xFF;;
 space[7]= BDOS_TOP>>8;

 space[0xFFF8]=0xC3;	/* for 'Undef' trap */
 space[0xFFF9]=0x00;
 space[0xFFFA]=0x00;

 s_p=space+0x100;
 dma_adr= DEFDMA;
 
 init_bdos();

}

finish_cpm()
{
}

/*	bios Functions	*/

bios_cboot()
{
	printf("HALT (bios_cboot)");
	longjmp(jbuf,-1);
}

bios_wboot()
{
	longjmp(ccpWbootEntry,-1);
}

wboot_sub()
{
 space[0]=0xc3;	/* jmp 0fa03h */
 space[1]= BIOS_ENT & 0xFF;
 space[2]= BIOS_ENT>>8;

 space[5]=0xc3;	/* jmp 0ec06h */
 space[6]= BDOS_TOP & 0xFF;
 space[7]= BDOS_TOP>>8;

 load_sys();
 reg(1)=space[4];	/* C reg = current Drive */

 /* for 'Undef' trap */
/*
 space[0xFFF8]=0xC3;
 space[0xFFF9]=0x00;
 space[0xFFFA]=0x00;
*/
}

/*
	load sys
*/
static int ccpEnt;

load_sys()
{int p= BDOS_TOP;

 space[p-6]= 0x00;
 space[p-5]= 0x16;	/* hidden Version No.(Decimal 22) */
 space[p-4]= SERNO & 0xFF;	/* Serial No. */
 space[p-3]= (SERNO >> 8) & 0xFF;;
 space[p-2]= (SERNO >> 16) & 0xFF;;
 space[p-1]= (SERNO >> 24) & 0xFF;;

 space[p++]=0x76;	/* emt_bdos */
 space[p++]=0xFF;
 space[p++]=0xC9;	/* ret */

 ccpEnt=p;
 space[p++]=0x76;	/* emt_ret_ccp */
 space[p++]=0xFE;

 return 0;
}

set_ccp_ret_on_stack()
{
	int p= DEF_USR_STK;
	space[p]= ccpEnt;
	space[p+1]= ccpEnt >> 8;
	return p;
}



load_bios(fn)char *fn;
{FILE *f; int c,x;char b[255];
 int i=BIOS_TOP;
 if((f=fopen(fn,"r"))==NULL){printf("can not open %s\n",fn);return -1;}
 fgets(b,255,f);

 for(;;){	/* load BIOS */
	if((c=getc(f))== EOF || i>=0x10000) break;
	if(c<' ') continue;
	space[i++]= (bin(c)<<4) | bin(getc(f));
 }
 fclose(f);
 return i;
}

bin(x)
{
return	(((x)<'a')? (x)-'0' : (x)-'a'+10);
}


/*
	bios routines
*/

bios_const()
{long n;
#if defined(LINUX) || defined(MACOSX)
 fd_set rfds;
 struct timeval to;

 fflush(stdout);
 /* if(ioctl(0,FIONREAD,&n) < 0) return -1; */

 FD_ZERO(&rfds);
 FD_SET(0, &rfds);
 to.tv_sec = 0; to.tv_usec = 0;
 n = select(FD_SETSIZE, &rfds, NULL, NULL, &to);
 if(n == -1){
   puts("select() error\r\n"); /*return -1;*/
 }
 if(FD_ISSET(0,&rfds)){ /*xputs("."); fflush(stdout);/**/ }
 n=FD_ISSET(0,&rfds);
#else
 fflush(stdout);
 ioctl(0,FIONREAD,&n);
#endif
 reg(A)= n ? 0xFF : 0 ;
 /*printf(" %d,%d ",n, reg(A));fflush(stdout);*/
}


#ifdef LINUX
static
waittty()
{int n;
 fd_set rfds;

 FD_ZERO(&rfds);
 FD_SET(0, &rfds);
 n = select(FD_SETSIZE, &rfds, NULL, NULL, NULL);
 if(n == -1){
	xputs("select() error\r\n"); return -1;
 }
 return FD_ISSET(0,&rfds);
}
#endif

bios_conin()
{unsigned char c;
 fflush(stdout);
#ifdef LINUX
 waittty();
#endif
#ifdef CURSES
 refresh();
#endif
 con_b=read(0,&c,1);
 //printf(" %02x ",c);fflush(stdout);
 reg(A)=c;
 return c;
}

void
bios_conout()
{
 int c;
 static int esc=0, y=0;
 c=reg(C);
 /* write(2,&reg(C),1); */
 /* putchar(reg(C));*/
 //if(c== 0x00){ esc=0; return;}
 if(c== 0x1A){ cons_cls();esc=0; return;} /* ^Z */
 if(c== 0x0B){ cons_up();esc=0; return;} /* ^K */
 if(c== 0x1E){ cons_home();esc=0; return;} /* ^^ */
 //if(c== '\r'){ cons_down();esc=0; return;}
 if(c== 0x1B){ esc=1; return;}
#ifdef NEVER
 if(c== 7){ putchar(7); return;}
 if(c== '\n'){ putchar('\n'); return;}
 if(c<' '){ esc=0; return;}
#endif /*NEVER*/
 switch(esc){
 case 0 :
#ifdef CURSES
   printw("%c",c);refresh(); return;
#else
   putchar(c); return;
#endif
 case 1 : switch(c){
	  case '*' :  cons_cls(); esc=0; return;
	  case '=' :  esc=2; return;
	  default  :  esc=0; putchar(c); return;
	 }
 case 2 : y= c-' '; esc=3; return;
 case 3 : cons_cursor(c-' ',y); esc=0; return;
 }
}

/*	disk I/O	*/
/******---******/

bios_setdma()
{
 dma_adr= wreg(BC);
/* if(verbose) verbose_msg("bios_setdma called"); */
}


bios_home()
{
 if(verbose) verbose_msg("bios_home called");
}

bios_seldsk()
{
 drive= reg(C);
 wreg(HL)= DPH_BASE+ reg(C)*16;
/* if(verbose) verbose_msg("bios_seldsk called"); */
}

bios_settrk()
{
 if(verbose) verbose_msg("bios_settrk called");
}

bios_setsec()
{
 if(verbose) verbose_msg("bios_setsec called");
}

bios_sectran()
{
 if(verbose) verbose_msg("bios_sectran called");
}


bios_cpm_read()
{
 if(verbose) verbose_msg("bios_cpm_read called");
}

bios_cpm_write()
{
 if(verbose) verbose_msg("bios_cpm_write called");
}



/*
	charcter I/O
*/

bios_listst()
{
 reg(7)= 0xFF;
}

bios_list()
{
}




/*	extend CP/M	*/

/* UNIX FILE ACCESS BIOS CALLS */

/* open unix file.
	in
	BC: mode
	DE: pointer to the unix file name string.(NULL TERMINATED!! NOT $)
	HL: flags 
	out
	HL: fd (unix like)
 */
bios_unix_open()
{
	unsigned char *filename = space + wreg(DE);
	int fd;

	wreg(HL)=fd= open(filename, wreg(HL),wreg(BC));
	if(fd ==-1) wreg(BC) = errno;
}

/* close unix file.
	in
	A : fd.
	out
	HL: result
 */
bios_unix_close()
{
	if(wreg(HL)=close(reg(A))) wreg(BC)= errno;
}

/* read unix file.
	in
	A : fd (unix like)
	DE: buf
	BC: nbytes
	out
	HL: read bytes
 */
bios_unix_read()
{
	unsigned char *buf = space + wreg(DE);
	int x;
	wreg(HL) =x= read(reg(A),buf, wreg(BC));
	if(x==-1) wreg(BC)= errno;
}

/* write unix file.
	in:
	A : fd (unix like)
	DE: buf
	BC: nbytes
	out:
	HL: written bytes
 */
bios_unix_write()
{
	unsigned char *buf = space + wreg(DE);
	int x;
	wreg(HL) =x= write(reg(A),buf, wreg(BC));
	if(x==-1) wreg(BC)= errno;
}

/*
	system
	in
	DE: pointer of string
	out
	HL: exit status
*/
bios_unix_system()
{
/*	setdown_tty(1); */
	wreg(HL)=system1(space+wreg(DE));
/*	setup_tty(1); */
}

/*	rdr, pun open close unix functions	*/

/*	puncher	*/
/*
	in
	DE: file name
	out
	HL: result -1/0
*/
void
bios_pun_open()
{
 if((pun=fopen(space+wreg(DE),"w"))==NULL){
	wreg(HL)= -1;return;
 }
 wreg(HL)=0;
}

bios_pun_close()
{
 fclose(pun);
}

/*
	in
	C: char
*/
bios_punch()
{
 putc(reg(C),pun);
}

/*	reader	*/
/*
	in
	DE: file name
	out
	HL: result -1/0
*/
void
bios_rdr_open()
{
 if((rdr=fopen(space+wreg(DE),"r"))==NULL){
	wreg(HL)= -1;return;
 }
 wreg(HL)=0;
}

bios_rdr_close()
{
 fclose(rdr);
}

/*
	out
	A: getc
	HL: getc
*/
bios_reader()
{
 reg(A)=wreg(HL)=getc(rdr);
}


