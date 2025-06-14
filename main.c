/*
	main.c
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

#include <fcntl.h>
#include <sgtty.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/param.h>


#ifdef TERMCAP
# include <curses.h>
# include <term.h>
#endif
#ifdef CURSES_
# include <curses.h>
#endif

#include "i80.h"
#include "default.h"
#include "bios.h"
#include "bdos.h"
#include "env.h"

extern	char	*getenv();
extern	void init_termcap();
extern	void cons_cursor();
extern	void cons_cls();
extern	void make_autoexec();
extern	void do_opt();
extern	void make_autoexec();


#ifdef LINUX
static struct  termio  old_tty;
static struct  termio  new_tty;
#else
struct sgttyb oldt, newt;
#endif /*!LINUX*/

extern int optind;
extern char *optarg;

/*	parameters	*/
char	*bios_file,
	*bios_= NULL;
char	bbb[256];
char	*conf_file;
int	smart_term,
	smart_;

char	cwd[MAXPATHLEN];

int verbose = VERBOSE;
int ccp_upper_f =CCP_LINE_UPPER;
int fname_lo_f =FNAME_LO;
int bdos_lib_srch =LIB_SRCH;


static char startup_cmd[256] = {0};

char *version = "\t%dK cpm v0.11 + ";

/**/
char termcap[1024];
char *t_CM, *t_CLS, *t_HOME,  *t_UP, *t_DOWN;
char *tc,tcbuf[1024];




void
terminate()
{
 the_exit("\nCP/M terminated\n");
}

#ifdef TERMCAP
void
init_termcap()
{
 char *termtyp=getenv("TERM");
 if(termtyp ==NULL){
   printf("can not get TERM\n");
   return;
 }
 if(1!=tgetent(termcap, termtyp)){
   printf("cannot get TERMCAP entry, cannot use screen editor\n");
   t_CM=t_CLS=t_UP=t_HOME=t_DOWN=NULL;
   return;
 }
/* printf("tgetent=%s\n", termcap ); /**/
 tc=tcbuf;
 t_CM=tgetstr("cm", &tc);
 t_CLS=tgetstr("cl", &tc);
 t_UP=tgetstr("up", &tc);
 t_HOME=tgetstr("ho", &tc);
 t_DOWN=tgetstr("do", &tc);
 //printf("t_CM=| %s |  ,  t_CLS=| %s |,  t_CLS=| %s |\n", t_CM, t_CLS, t_UP);

 if(t_CM == NULL || t_CLS==NULL){
   printf("your terminal's capabirity is too poor, cannot use WordMaster screen editor\n");
 }
 if(t_UP == NULL || t_HOME==NULL /* || t_DOWN==NULL */ ){
   printf("your terminal's  capabirity is too poor, cannot use MuSTAR\n");
 }
}
#elif CURSES_
void
init_termcap()
{
  initscr(); // cbreak(); noecho();
  raw();
}
#endif

#ifdef TERMCAP
xputc(c)
{
 putchar(c);
}

void
cons_cursor(x,y)
{
	if(t_CM==NULL) return;
	tputs( tgoto(t_CM, x, y), 25, xputc);
}
#elif CURSES_
void
cons_cursor(x,y)
{
 move(y, x);
 //printf("*** CURSOR %d,%d ***",x,y);
}
#endif

dmp(s)
char *s;
{
 for(;*s;){
	printf("%02x ", *s++);
 }
 printf("\n");
}

#ifdef TERMCAP
void
cons_cls()
{
	if(t_CLS==NULL) return;
	tputs( t_CLS , 25, xputc);
}

void
cons_up()
{
	if(t_UP==NULL) return;
	tputs( t_UP , 25, xputc);
}

void
cons_home()
{
	if(t_HOME==NULL) return;
	tputs( t_HOME , 25, xputc);
}
void
cons_down()
{
	if(t_DOWN==NULL) return;
	tputs( t_DOWN , 25, xputc);
}

#elif CURSES_
void
cons_cls()
{
  erase();
}
#endif


main(argc,argv)int argc;char **argv;
{

 disp_version();
#ifdef ENDIAN_CHECK
 reg(B)=1; reg(C)=2;
 reg(D)=3; reg(E)=4;
 reg(H)=5; reg(L)=6;
 reg(A)=7; 

 printf("\r\n %04x\r\n", wreg(BC) );
 printf(" %04x\r\n", wreg(DE) );
 printf(" %04x\r\n", wreg(HL) );
 printf(" %04x\r\n", wreg(AF) );
#endif

 init_termcap();

 set_subfile();
 if(getwd(cwd) == NULL) {
	the_exit("can not getwd");
 }

 def_value();
 do_opt(argc,argv);
 set_conf();
 if(drv[0].rw == -1) the_exit(" drive A: not assigned to UNIX-path\n");

 if(smart_>=0) smart_term=smart_;
 if(bios_!= NULL) bios_file=bios_;

 signal(SIGTERM, terminate);
 signal(SIGHUP, terminate);

 fclose(stderr); /**/

 if(*startup_cmd !=0) make_autoexec(startup_cmd);

 go_cpm();
}

go_cpm(dr)char *dr;
{int x;
 if(-1==load_bios(bios_file)) exit(0);
 init_cpm(dr);
 load_sys();
 setup_tty(1);
 ccp();
}

void
make_autoexec(s)
char *s;
{FILE *f;
 char b[256];

 if(NULL==(f=fopen(subfile,"w"))) return;
 b[0]=strlen(s);
 strcpy(b+1,s);
 fwrite(b,128,1,f);
 fclose(f);
}


/*	misc	subroutines	*/
disp_version()
{
 printf(version,(BDOS_TOP - 0xEC06)/1024 + 64);
 disp_80_version();
}

setup_tty(fd)
{
#ifdef LINUX
 if(ioctl(1, TCGETA, &old_tty) < 0){
   printf("ioctl failed\n");
   return -1;
 }

 new_tty = old_tty;
 new_tty.c_iflag = 0;
 new_tty.c_oflag = 0;
 new_tty.c_cflag = old_tty.c_cflag;
 new_tty.c_lflag = 0;
 new_tty.c_line = old_tty.c_line;
 new_tty.c_cc[VMIN] = 1;
 new_tty.c_cc[VTIME] = 0;
 if(ioctl(fd, TCSETAW, &new_tty) < 0){
   printf("ioctl failed\n");
   return -1;
 }

 /* if(fcntl(0, F_SETFL, O_NONBLOCK) < 0){*/
 if(fcntl(0, F_SETFL, FNDELAY) < 0){
   printf("fcntl failed\n");
   return -1;
 }
#else
	gtty(1,&oldt);
	newt=oldt;
/*	newt.sg_flags |= CBREAK; */
	newt.sg_flags |= RAW;
	newt.sg_flags &= ~ECHO;
	stty(fd,&newt);
#endif
#ifdef CURSES_
 raw();
#endif

#ifdef VT_VT
	vt100_vt52(52);
#endif /*VT_VT*/
}

setdown_tty(fd)
{
#ifdef LINUX
  ioctl(fd,TCSETAW, &old_tty);
#else
	if(!smart_term) sleep(1);
	stty(fd,&oldt);
#endif
#ifdef CURSES_
	endwin();
#endif
#ifdef VT_VT
	vt100_vt52(100);
#endif /*VT_VT*/
}

#ifdef VT_VT
vt100_vt52(vt)
{char *b;
	if(!(b=getenv("TERM"))) return;
	if(strncmp("vt1",b,3) && strncmp("vt2",b,3)) return;
	if(vt==52){printf("\033[?2l");fflush(stdout); }
	else{	printf("\033<");fflush(stdout); }
}
#endif /*VT_VT*/

nputs(s)char *s;
{printf("%s",s);}

set_subfile()
{
	sprintf(subfile,"%s.%d",SUBFILE,getpid());
	sprintf(subfile2,"%s.%d",SUBFILE2,getpid());
}
	

/*
	set up subroutine
*/

def_value()
{char *c;
 strcpy(com_path,"");
 strcpy(lib_path,"");
 bios_file= CPM_BIOS;
 smart_term=1;
 if((c=getenv(CPM_ENV))!=NULL) conf_file= c;
 else conf_file= DEF_CONF;
}

void
do_opt(argc,argv) int argc;char **argv;
{
 smart_= -1;
 for(;;){
	switch(getopt(argc,argv,"mts:b:f:")){
		case 't' : smart_=0; break;
		case 'b' : bios_=optarg; break;
		case 'f' : conf_file=optarg; break;
		case EOF : return;
		default  :
		 printf("%s [-t][-b BIOS_FILE][-f CONF_FILE]\n", argv[0]);
			exit(0);
	}
 }
}

set_conf()
{FILE *f;
 int x,ty;
 char buf[MAXPATHLEN+100];
 char b1[MAXPATHLEN],b2[MAXPATHLEN],b3[MAXPATHLEN],*p,*p2,*index();
 if((f=fopen(conf_file,"r"))==NULL){
	sprintf(buf," can not open config-file: %s\n",conf_file);
	the_exit(buf);
 }
 clear_drive();
 for(;;){
	if(fgets(buf,MAXPATHLEN,f)==NULL) break;
	if(1>sscanf(buf,"%s ",b1)) continue;
	if(*buf==';') continue;	/* comment */
	if(!strcmp(b1,"loose_term")){
		smart_term=0;
		continue;
	}
	if(0==strcmp(b1,"startup")){
		/* startup */
		if(NULL==(p=index(buf,'\''))){
			printf("startup:missing open '''\n"); exit(1);
		}
		p++;
		if(NULL==(p2=index(p,'\''))){
			printf("startup:missing close '''\n"); exit(1);
		}
		*p2= '\0';
		strcpy(startup_cmd,p);
		continue;
	}

	x=sscanf(buf,"%s %s %s",b1,b2,b3);
	if(x!=2){
		printf("\nConfig.File: %s ???\n",b1);
		continue;
	}
	if(0==strcmp(b1,"com_path")){
		/* com_path */
		strcpy(com_path,b2);
		strcat(com_path,"/");
		continue;
	}else if(0==strcmp(b1,"lib_path")){
		/* lib_path */
		strcpy(lib_path,b2);
		strcat(lib_path,"/");
		continue;
	}else if(0==strcmp(b1,"bios")){
		/* bios_file */
		strcpy(bios_file=bbb,b2);
		continue;
	}else if(0==strcmp(b1,"verbose")){
		/* verbose */
		verbose = atoi(b2);
		continue;
	}else if(0==strcmp(b1,"ccp_line_upper")){
		/* ccp_line_upper */
		ccp_upper_f =atoi(b2);
		continue;
	}else if(0==strcmp(b1,"file_name_low")){
		/* fname_lo */
		fname_lo_f =atoi(b2);
		continue;
	}else if(0==strcmp(b1,"lib_srch")){
		/* bdos_lib_srch */
		bdos_lib_srch =atoi(b2);
		continue;
	}else{
		/* drive */
		x=sscanf(buf,"%s %s ",b1,b2);
		if(x!=2) continue;
		if(b1[1]!=':'){ printf("\nConfig.File: %s ???\n",b1); continue;}
		set_drive(b1,b2);
		continue;
	}
 }
}


set_drive(dr,nm)
char *dr,*nm;
{int i;
	i=(islower(*dr) ? toupper(*dr) : *dr)-'A';
	if( *nm == '/' ){ /* absolute path */
		strcpy(drv[i].name,nm);
	}else{
		strcpy(drv[i].name,cwd);
		strcat(drv[i].name,"/");
		strcat(drv[i].name,nm);
	}
	drv[i].rw=1;
	bdos_logvec |= 1<<i;
}

clear_drive()
{int i;
 for(i=0;i<16;i++) drv[i].rw= -1;	/* clear drives */
}


the_exit(s)
char *s;
{
	closeAll();
	finish_cpm();
#ifdef CURSES_
	endwin();
#endif
	setdown_tty(1);
	printf("%s",s);
	exit(0);
}

/* EOF */
