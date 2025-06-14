/*
	lined.c
	line edit routine
		   by
		S.Takeoka
	1999/Jul/11 DELC at endOfLine debug
*/
#include "copying.h"


#include <stdio.h>
#include <setjmp.h>
#include <signal.h>
#ifdef LINUX
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <termio.h>
#include <errno.h>
#endif /*LINUX*/

#include "env.h"


/* #define WORDSTAR /**/
/* #define EMACS   /**/

static void del_ __P( (char *b, int x));
static void ins_ __P( (char *b, int x));
static void eraEol_ __P( (char *b, int x));
static void cback_ __P( ( char *s) );
static void linTop __P( ( char *s, int x) );
static void zputs __P( ( char *x) );
static void zputc __P( ( int) );
static void zbs __P( ( int) );

#ifdef CURSES
#define putchar(x) do{printw("%c",x);refresh();}while(0);
#endif

#define Ctrl(x) ((x)& 0x1F)

#define CR      '\r'
#define DEL     0x7F
#define BS      Ctrl('H')
#define CTRLC   Ctrl('C')
#define CTRLZ   Ctrl('Z')
#define CAN	Ctrl('X')
#define CAN2	Ctrl('U')


#ifdef WORDSTAR
#define UP      Ctrl('E')
#define DOWN	Ctrl('X')
#define RIGHT   Ctrl('D')
#define LEFT    Ctrl('S')
#define KILL    Ctrl('K')
#define DELC    Ctrl('G')
#define SUSP	Ctrl('Q')
#define EOL	Ctrl('\\')
#define TOL	Ctrl('\\')
#endif

#ifdef CPM3
#define UP      Ctrl('R')
#define DOWN	Ctrl('X')
#define RIGHT   Ctrl('F')
#define LEFT    Ctrl('A')
#define KILL    Ctrl('K')
#define DELC    Ctrl('G')
#define SUSP	Ctrl('D')
#define EOL	Ctrl('\\')
#define TOL	Ctrl('\\')
#endif

#ifdef EMACS
#define UP	Ctrl('P')
#define DOWN	Ctrl('N')
#define RIGHT	Ctrl('F')
#define LEFT	Ctrl('B')
#define KILL	Ctrl('K')
#define DELC	Ctrl('D')
#define SUSP	Ctrl('Q')
#define EOL	Ctrl('E')
#define TOL	Ctrl('A')
#endif




#define Kill() \
{\
	eraEol_(b,cx); \
	b[cx]=0; \
	len=strlen(b); \
}


linEd(b,blen)
char *b;
int blen;
{
 int cx,len;
 int c;

 blen--;

 zputs(b);      /* display the line */
 len=cx=strlen(b);

 for(;;){
        c= bios_conin();
        switch(c){
        case SUSP :
		if(len!=0) continue;
		setdown_tty(1);
		kill(getpid(), SIGTSTP);
		setup_tty(1);
		continue;
        case CTRLC :
		if(len!=0) break; /* charcter insert */
                zputs("^C");
		longjmp(ccpWbootEntry,-1);
		/* return -1; */
        case CR :
                return 0;
        case UP :
		linTop(b,cx);
		cx=0;
		Kill();
                return 1;
/*
        case DOWN :
                return 2;
*/
        case CAN :
        case CAN2 :
		linTop(b,cx);
		cx=0;
		Kill();
		continue;
        case LEFT:
                if(cx <=0) continue;
                zbs(b[cx-1]);
                cx--;
                continue;
        case RIGHT:
                if(b[cx]== '\0') continue;
                zputc(b[cx]);
                cx++;
                continue;

        case KILL :
		Kill();
                continue;

        case DEL:
        case BS:
                if(cx <=0) continue;
                zbs(b[cx-1]);
                cx--;
                /* fall into DELC */
        case DELC :
		c=b[cx];
		if(c=='\0') continue;
		del_(b,cx);
		zputs(&b[cx]); /* move cursor to line end */
		if(c<' ')putchar(' ');
		putchar(' ');
		zbs(c);
		cback_(&b[cx]); /* move cursor to insertion point */
		len--;
                continue;
        case EOL :
		c=b[cx];
		if(c=='\0') continue;
		zputs(&b[cx]); /* move cursor to line end */
		cx=len;
                continue;
        case TOL :
                for(;cx;){
			cx--;
			zbs(b[cx]);
		}
                continue;
	default:;
        }
/*        if(c<' ') continue; */

	if(len>=blen) continue;
        ins_(b,cx);
        b[cx]=c;
        zputs(&b[cx]);
        cx++;len++;

        cback_(&b[cx]);

 }
}

static void
del_(char *b, int x)
{
 char *p;
 p=b+x;
 for(;*p;){
        *p= *(p+1);
        p++;
 }
}


static void
ins_(char *b,int x)
{
 char *p,*q;
 int save,a;

 p=b+x;
 if(*p == 0){ *(p+1)=0; return;}

 for(;*p;p++)
        ;
 save=b[x];
 b[x]=0;

 q=p+1;
 a= *p;
 for(;;){
        *q= a;
        p--;q--;
        if((a= *p) ==0) break;
 }
 *q= save;
}


static void
eraEol_(char *b,int cx)
{char *s;
 int c;
 s=b+cx;
        for(c=0;*s;s++){
                if(*s<' '){putchar(' '); c++;}
                putchar(' '); c++;
        }
        for(;c;c--) putchar('\b');
}



static void
cback_(char *s)
{
        for(;*s;s++){
		zbs(*s);
	}
}

static void
linTop(char *s,int x)
{int save;
 save=s[x];
 s[x]=0;
 cback_(s);
 s[x]=save;
}


static void
zputs(char *x)
{
 char ss[256],*s;

 for(s=ss;;){
	if(*x == 0){ *s =0; break;}
	if(*x <' '){ *s++= '^'; *s++= *x | 0x40; }
	else	   { *s++= *x; }
	x++;
 }

 printf("%s",ss);
}


static void
zputc(int c)
{
	if(c< ' '){ putchar('^'); putchar(c | 0x40);}
	else		{putchar(c); }
}

static void
zbs(int c)
{
	if(c==0){ putchar('\b'); return;}
	if(c<' ') putchar('\b');
	putchar('\b');
}
