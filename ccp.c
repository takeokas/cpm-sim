/*
	ccp.c
	8080 Machine Interpreter
		   by
		S.Takeoka

	Jan/16/1992
	Jan/04/1992
*/
#include "copying.h"

#include <stdio.h>
#include <setjmp.h>
#include <ctype.h>
#include <sys/param.h>

#ifdef MACOSX
#include <stdlib.h>
#endif /* MACOSX */

#ifdef LINUX
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <termio.h>
#endif /* LINUX*/

#include "i80.h"
#include "bdos.h"
#include "env.h"

extern char *index();

extern void ccp_save();
extern void ccp_ren();
extern void ccp_era();
extern void ccp_save();
extern void ccp_user();
extern void ccp_chdir();
extern void ccp_path();
extern void setargs();
extern void def_fcb_set();
extern void def_fcb_set();
extern void def_fcb_set();


#define FCB1 0x5C
#define FCB2 0x6C

#define CcpDefDrv space[4]


jmp_buf	ccpWbootEntry;
/*---*/
static jmp_buf	ccpEntry;
static jmp_buf	machineTrap;

char com_path[MAXPATHLEN] = {0};

/* ccp */
static int ccp_curUser;


/* ccp args*/
char	Arg[4][256];
char	arg[4][256];
int Argco;
int argco;

#ifdef AHO
/* CCP Variables */
#define Var_Max 10
struct  {
  char *name,
  int v}
var[Var_Max];

#define UPPER 0

#endif /*AHO*/


/*
	EMT	return ccp without wboot
*/
emt_return_ccp()
{
	longjmp(ccpEntry,-1);
}

/*
	ccp
*/

ccp()
{char *s,cmdlin[128],cmd[128];

 int x,i;

 xsub=0;
 reg(E)=ccp_curUser=0;bdos_user(); /*set user*/

 if(x=setjmp(machineTrap)){
	/* if(x!=-1)pr_stat(); */
	pr_stat();
 }
 bcopy(machineTrap,jbuf,sizeof(jbuf));
 /* machine Trap causes Wboot */

 /* wboot restarts here!!! */
 setjmp(ccpWbootEntry);

 wboot_sub();
 if(xsub) printf("(xsub active)");

 /* return CCP without wbooting */
 if(setjmp(ccpEntry)){
	fprintf(stderr,"(CCP Hot start)\r\n");
 }

 closeAll();

 printf("\r\n");

 for(;;){
	reg(E)=CcpDefDrv; bdos_setcurdrv();
	reg(E)=ccp_curUser;bdos_user(); /*set user*/
	wreg(DE)=DEFDMA; bdos_setdma(); /* set DEFDMA */

	if(ccp_curUser)	printf("%d%c>", ccp_curUser , ('A'+CcpDefDrv));
	else		printf("%c>", ('A'+CcpDefDrv));
	getcmd(cmdlin);
	s=cmdlin;
	printf("\r\n");
	if(strlen(s)==0) continue;

	/* raw args */
	argco=sscanf(s,"%s %s %s %s",arg[0],arg[1],arg[2],arg[3]);

	if(ccp_upper_f)toupperStr(s);
	/* Modifyed args */
	Argco=sscanf(s,"%s %s %s %s",Arg[0],Arg[1],Arg[2],Arg[3]);
#ifdef AHO
	fprintf(stderr,"Argco=%d,",Argco);
	for(i=0;i<Argco;i++){
	  fprintf(stderr,"%s ",Arg[i]);
	}
	fprintf(stderr,"\n");fflush(stderr);
#endif /*AHO*/


	if(Argco <=0) continue;
	if(strlen(Arg[0])==2 && Arg[0][1]== ':'){ /* def.drive */
		x= Arg[0][0] - 'A';
		if(x<0 || 15<x){printf("%s?\r\n",Arg[0]); continue;}
		CcpDefDrv= x;
		reg(E)=CcpDefDrv; bdos_setcurdrv();
		continue;
	}


	setargs(s);

	/*builtin */
	strcpy(cmd,Arg[0]);
	toupperStr(cmd);
	if(strcmp(cmd,"UNIX")==0){
		the_exit("\nCP/M Halted\n");
	}
	if(strcmp(cmd,"MON")==0){
		monitor();
		bcopy(machineTrap,jbuf,sizeof(jbuf));
		continue;
	}
	if(strcmp(cmd,"PWD")==0){
		if(Argco!=1) continue;
		ccp_pwd();
		continue;
	}
	if(strcmp(cmd,"CD")==0){
		ccp_chdir();
		continue;
	}
	if(strcmp(cmd,"PATH")==0){
		ccp_path();
		continue;
	}
#ifdef AHO
	if(strcmp(cmd,"SET")==0){
		ccp_set();
		continue;
	}
#endif /*AHO*/
	if(strcmp(cmd,"TYPE")==0){
		if(Argco!=2) continue;
		ccp_type();
		continue;
	}
	if(strcmp(cmd,"DIR")==0){
		ccp_dir();
		continue;
	}
	if(strcmp(cmd,"SAVE")==0){
		ccp_save();
		continue;
	}
	if(strcmp(cmd,"REN")==0){
		ccp_ren();
		continue;
	}
	if(strcmp(cmd,"ERA")==0){
		ccp_era();
		continue;
	}
	if(strcmp(cmd,"USER")==0){
		ccp_user();
		continue;
	}

	/* trangent command */
	load_exec_prog();
 }
}

static char ccp_gets_hist[256]={0};

getcmd(s)
char *s;
{int x;
	x=submit(s);
	if(NULL==x){
		/* no $$$.sub file */
		xsub=0;
		strcpy(s,"");
		for(;;){
		  x=console_gets(s,126);
		  if(x==0){
		    if(strlen(s)) strcpy(ccp_gets_hist,s);
		    break;
		  }
		  strcpy(s,ccp_gets_hist);
		}
		return 1;
	}
	return 0;
}

load_exec_prog()
{
	int p;

	if(Argco <1) return -1;
	if(-1==load_com()) return -1;

	wreg(DE)=DEFDMA; bdos_setdma(); /* set DEFDMA */
	p= set_ccp_ret_on_stack();
	exqt(space+0x0100,space+p);
}

load_com()
{
	char fn[MAXPATHLEN];
	char fnL[MAXPATHLEN];
	char AA[MAXPATHLEN];

	strcpy(AA,Arg[0]);
	if(Arg[0][1] == ':'){
		strcpy(AA,&Arg[0][2]);
		append_unix_path(AA,Arg[0][0]-'A'+1);
	}

	strcpy(fn,AA);strcat(fn,".COM");
	if(-1!=load_file(fn)){ return 1;}

	strcpy(fnL,fn);
	tolowerStr(fnL);
	if(-1!=load_file(fnL)){ return 1;}

	strcpy(fn,com_path); strcat(fn,AA); strcat(fn,".COM");
	if(-1!=load_file(fn)){ return 1;}

	strcpy(fn,com_path); strcat(fn,fnL);
	if(-1!=load_file(fn)){ return 1;}

	printf("%s?\r\n",Arg[0]);
	return -1;
}





/*
	builtin Commands
*/
char ccp_cmd_buf[256];

char *
bltin_cmd_fname(s)
char *s;
{
 if(fname_lo_f) tolowerStr(s);
 if(s[1] == ':'){
	reg(E)= s[0]<'a' ? s[0]-'A' : s[0]-'a'; bdos_setcurdrv();
	bcopy(s+2,s,strlen(s)-2+1);
 }
 if(NULL !=index(s,'.')) return s;
 strcat(s,".");
 return s;
}


ccp_type()
{char *p;
 bltin_cmd_fname(arg[1]);
 if(NULL==(p= (char*)getenv("PAGER"))){
   p="more";
 }
 strcpy(ccp_cmd_buf,p);
 strcat(ccp_cmd_buf," ");
 if(256==system1(strcat(ccp_cmd_buf,arg[1]))){/* printf("%s ?\r\n",arg[1]);*/}
}

ccp_dir()
{
 strcpy(ccp_cmd_buf,"ls -F ");
 if(argco==2){strcat(ccp_cmd_buf, bltin_cmd_fname(arg[1]));}
 if(argco==3){
   strcat(ccp_cmd_buf, arg[1]); 
   strcat(ccp_cmd_buf, " "); strcat(ccp_cmd_buf, bltin_cmd_fname(arg[2]));
 }
 system1(ccp_cmd_buf);
}

void
ccp_save()
{int n;
 if(argco!=3) {printf("?"); return;}
 n=atoi(arg[1]);
 bltin_cmd_fname(arg[2]);
 save_file(arg[2],n);
}

void
ccp_ren()
{
 char *s;
 if(argco!=2){printf("?"); return;}

 s=index(arg[1],'=');
 if(s==NULL){printf("?"); return;}

 *s++ = '\0';

 strcpy(ccp_cmd_buf,"mv ");
 strcat(ccp_cmd_buf, bltin_cmd_fname(s));
 strcat(ccp_cmd_buf, " ");
 strcat(ccp_cmd_buf, bltin_cmd_fname(arg[1]));
/* xputs(ccp_cmd_buf); */
 if(256== system1(ccp_cmd_buf)){ /* printf("%s ?\r\n",s); */}
}

void
ccp_era()
{char cmd[128];
 if(argco!=2){printf("?"); return;}
 strcpy(ccp_cmd_buf,"rm ");
 strcat(ccp_cmd_buf, bltin_cmd_fname(arg[1]));
 if(!strcmp("*.*",arg[1])){
	printf("ALL (Y/N)?");
	getcmd(cmd);
	printf("\r\n");
	if(*cmd != 'y' && *cmd != 'Y') return;
 }
 if(256== system1(ccp_cmd_buf)){ /* printf("%s ?\r\n",arg[1]); */}
}

void
ccp_user()
{int n;
 if(argco==1) {printf("%d\r\n",ccp_curUser); return;}
 if(argco!=2) {printf("?"); return;}
 reg(E)= ccp_curUser=atoi(arg[1]);
 bdos_user();
}

ccp_pwd()
{
 char s[MAXPATHLEN];
 strcpy(s,"pwd");
 system1(s);
}


void
ccp_chdir()
{char s[MAXPATHLEN];
 if(argco!=2) {printf("?"); return;}
 if(-1==chdir(arg[1])){printf("cannot chdir:%s\r\n",arg[1]); return;}
 /*getwd(s);*/
 getcwd(s,MAXPATHLEN);
 strcpy(drv[CcpDefDrv].name,s);
}

void
ccp_path()
{
 if(argco==1) {printf("path=%s\r\n",com_path); return;}
 if(argco!=2) {printf("?"); return;}
 strcpy(com_path,arg[1]);
 strcat(com_path,"/");
}


system1(s)
char *s;
{int x;
 FILE *f;
	if(NULL==(f=fopen("/dev/tty","r+"))){
		return 256;
	}
	bcopy(f,stderr,sizeof(FILE));
	setdown_tty(1);
	dollar(s);
	x=system(s);
	setup_tty(1);
	fclose(f);
	if(x==127){printf("\r\n??? UNIX command cannot be invoked:%s\r\r",s);}
	return x;
}

/*
	set Args
	deffcb,defdma
*/
void
setargs(s)
char *s;
{int i;
 for(i=FCB1;i<DEFDMA;i++){
   space[i]=0;
 }
 def_dma_set(s);
 if(Argco <=1){ /* none arg */
   def_fcb_clr(FCB1);
   def_fcb_clr(FCB2);
   return;
 }
 if(Argco == 2){ /* 1 arg */
   def_fcb_set(FCB1,Arg[1]);
   def_fcb_clr(FCB2);
   return;
 }
 if(Argco >= 3){ /* 2 arg */
   def_fcb_set(FCB1,Arg[1]);
   def_fcb_set(FCB2,Arg[2]);
   return;
 }
}

void
def_fcb_set(fcb,ar)
int fcb;
char *ar;
{BYTE *f;
 int i, dot_found=0;
 char *p;

 def_fcb_clr(fcb);

 if(NULL!= (p=index(ar,'='))) *p = '\0'; /* '=' */

 if(ar[1]==':'){ /* drive */
   space[fcb]= ar[0]-'A'+1;
   ar+=2;
 }
 /* file name */
 for(f=(space+fcb+1),i=0;i<8;i++){
   if(*ar == '\0')return;
   if(*ar == '.'){ dot_found=1; break;}
   *f++ = *ar++;
 }

 if(dot_found==0){
   for(;;ar++){
     if(*ar == '\0')return;
     if(*ar == '.') break;
   }
 }

 /* file type */
 ar++; /* skip '.' */
 for(f=(space+fcb+1+8),i=0;i<3;i++){
   if(*ar == '\0')return;
   *f++ = *ar++;
 }
}

def_fcb_clr(fcb)
int fcb;
{int i;

  space[fcb]= 0;
  for(i=0;i<(8+3);i++){
    space[fcb+1+i]=' ';
  }
}

def_dma_set(s)
char *s;
{int l;
 for(;;s++){ /* skip blank at top */
   if(*s !=' ') break;
 }
 for(;;s++){ /* skip command (Arg0) */
   if(*s =='\0') break;
   if(*s ==' ') break;
 }
 l=strlen(s);
 strcpy(space+DEFDMA+1,s); /*must be NULL terminated!bcopy(s,(space+DEFDMA+1),l);*/
 space[DEFDMA]=l;
}


/*
	misc routines
*/
#define Upper(x) (('a' <= (x) && (x) <='z') ? ((x)-'a'+'A') : (x))

toupperStr(s)
char *s;
{
  for(;*s;s++){
    *s = Upper(*s);
  }
}


#define Lower(x) (('A' <= (x) && (x) <='Z') ? ((x)-'A'+'a') : (x))

tolowerStr(s)
char *s;
{
  for(;*s;s++){
    *s = Lower(*s);
  }
}





#ifdef AHO
ccp_set()
{
 if(argco<2){
   for(i=0;i<Var_Max;i++){
     printf("%s=%d\n"var[i].name,var[i].v);
   }
   return;
 }
 if(argco<3){
 }
}
#endif /*AHO*/
