/*
	bdos.c
	8080 Machine Interpreter
		   by
		S.Takeoka

	Feb/03/1992
	Jan/04/1992
*/
/* #define DEBUG /**/

/*
	--- memo---

fileName=== xxxx???? -> xxxx*   (search '?' from end of FileName.)

fileType=== filename.???, fileName.___   ->   fileName.*  , fileName*

#define SELDSK	14 OK?
#define LOGVEC	24
#define USER	32 not yet

*/
#include "copying.h"


#include <stdio.h>
#include <setjmp.h>
#include <ctype.h>
#ifdef __FreeBSD__
# include <unistd.h>
#endif __FreeBSD__


#ifdef SYSV
#	include <sys/types.h>
#	include <sys/stat.h>
#endif

#include <fcntl.h>
#include <sys/file.h>
#include <sys/param.h>

#ifdef LINUX
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <termio.h>
#include <errno.h>
#endif /* LINUX*/

#include "i80.h"
#include "env.h"
#include "bdos.h"
#include "bdosfunc.h"

extern void emt_bdos();
extern void init_bdos();
extern void closeAll();
extern void make_lib_file_name(char*,BYTE*);
extern void make_file_name();
extern void dollar();
extern void bdos_rename();
extern void bdos_open();
extern void bdos_creat();
extern void bdos_close();
extern void bdos_read();
extern void bdos_write();
extern void bdos_rdrnd();
extern void bdos_wrrnd();
extern void bdos_fsize();

extern void bdos_src1st();
extern void bdos_srcnxt();
extern void bdos_user();
extern void bdos_const();
extern void bdos_dirio();
extern void bdos_putstr();
//extern void bdos_chdir();
extern void ch_dir();




int xsub;
char	subfile[256];
char	subfile2[256];

char	lib_path[256];

int dma_adr;

int bdos_logvec;

static int c_buf = -1; /* conin Buffer */

static int bdos_cur_user=0;
static int bdos_cur_drv=0;


struct	drive_t	drv[16];




struct	cpmFile {
	int fd;
	int opened;
	char name[256];
}; 

static struct	cpmFile	cpmfil[CPM_MAX_FILES];

#define FCREATE_MODE 00644

/* fcb offset */
#define CPD	16
#define CurBlkLo	12
#define CurBlkHi	14
#define CurRec	32


/* DEBUG */


xputs(s)char *s;
{
#ifdef DEBUG
 fputs(s,stdout);
 fflush(stdout); /* SUN is foolish */
#endif DEBUG
}

xprintf(s,x,y,z)
{
#ifdef DEBUG
  printf(s,x,y,z);
#endif !DEBUG
}



/*
	--- submit ---
*/
/*
	bdos_xsub
*/
bdos_xsub()
{
	xsub = reg(E);
	reg(A)=wreg(HL)=0;
}

submit(s)
char *s;
{FILE *f1,*f2;
 long ep;
 unsigned char w[128],work[128];
 int x,i;

 bdos_const();
 if(reg(A)){ /* abort submit */
	c_buf = -1;
	unlink(subfile);
	return 0;
 }
 if(NULL==(f1=fopen(subfile,"r"))){
	return 0; /* Not submitting */
 }
 fseek(f1,-128L,2);
 ep = ftell(f1);
 x= fread(work,128,1,f1);
 if(ep!=0){
/* xprintf("\r\nsubmit:ep=%d  ",ep); */
	if(NULL==(f2=fopen(subfile2,"w"))){
		printf("\r\nBDOS:Submit workFile cannot open\r\n");
		unlink(subfile);
		return 0;
	}
	rewind(f1);
	for(i=0;ep>0;ep-=128,i++){
		x= fread(w,128,1,f1);
		if(x==NULL) break;
		x=fwrite(w,128,1,f2);
	}
	fclose(f2);
	fclose(f1);
	unlink(subfile);
	rename(subfile2,subfile);
 }else{
	fclose(f1);
	unlink(subfile);
 }
 bzero(s,128);
 strncpy(s,work+1,*work);
 printf("%s",s);
 return 1;
}

/*
	--- EMT bdos ---
*/
void
emt_bdos()
{
xprintf("bdos=%d\r\n",reg(C)); /**/
 switch(reg(C)){
	case WBOOT : bdos_wboot(); /* longjmp to ccpWbootEntry */
	case CONIN : bdos_conin(); return;
	case CONOUT: bdos_conout();return;
	case RDRIN : bdos_rdrin(); return;
	case PUNOUT :bdos_punout();return;
	case LSTOUT :bdos_lstout();return;
	case DIRIO : bdos_dirio(); return;
	case GETIOB :bdos_getiob(); return;
	case SETIOB :bdos_setiob(); return;
	case PUTSTR :bdos_putstr(); return;
	case GETSTR :bdos_getstr(); return;
	case CONST :bdos_const(); return;
	case VER : bdos_getver(); return;
	case RESET : bdos_reset();return;
	case SELDSK :bdos_setcurdrv();return;
	case OPEN :bdos_open();return;
	case CLOSE :bdos_close();return;
	case SRC1ST :bdos_src1st();return;
	case SRCNXT :bdos_srcnxt();return;

	case FERA : bdos_erase();return;

	case READ :bdos_read();return;
	case WRITE :bdos_write();return;
	case FCREA :bdos_creat();return;

	case FREN : bdos_rename();return;
	case LOGVEC :bdos_getlogvec();return;
	case CURDRV :bdos_curdrv();return;
	case SETDMA : bdos_setdma();return;
	case ALLCVEC :bdos_allcvec();return;
	case PROTECT :bdos_protect();return;
	case ROVEC :bdos_rovec();return;
	case FATR :bdos_fatr();return;
	case DPBA :bdos_dpba();return;
	case USER :bdos_user();return;

	case RDRND :bdos_rdrnd();return;
	case WRRND :bdos_wrrnd();return;

	case FSIZE :bdos_fsize();return;

	case SETRREC :bdos_setrrec();return;
	/* CP/sin Only */
	case XSUB : bdos_xsub();return;

	default :
	if(verbose){
		printf("\r\n*** wrong Bdos function (%d) ",reg(C));
		pr_stat1();
		printf(" ***\r\n");
	}
	reg(A)=wreg(HL)=0xFF; return;
 }
}


/*

*/
void
init_bdos()
{int i;
xputs(" init_bdos ");

 for(i=0;i<CPM_MAX_FILES;i++){
	cpmfil[i].opened =0;
 }
}

void
closeAll()
{int i;
xputs(" closeAll ");

 for(i=0;i<CPM_MAX_FILES;i++){
   if(cpmfil[i].opened){
	close(cpmfil[i].fd);
	cpmfil[i].opened =0;
xprintf("opened=%d ",i);
   }
 }
}

new_fd()
{int i;
xputs(" new_fd ");

 for(i=0;i<CPM_MAX_FILES;i++){
   if(cpmfil[i].opened ==0){
xprintf("newfd=%d ",i);
     return i;
   }
 }
 printf("\r\nBDOS:CPM_MAX_FILES too less(%d)\r\n",CPM_MAX_FILES);
 return -1;
}

check_opened(fn)
char *fn;
{int i;
xputs(" check_opened ");

 for(i=0;i<CPM_MAX_FILES;i++){
   if(cpmfil[i].opened && !strcmp( cpmfil[i].name , fn) ){
	return i;
   }
 }
 return -1;
}


/*
  bdos function subroutines
*/
/*
 */
void
make_lib_file_name(fn,fcb)
char *fn;
BYTE *fcb;
{BYTE *pp;
 int d;
 char f[256];

 pp=fcb + 1;

 bzero(fn,9);
 strncpy(fn,pp,8);
 cut_spaces(fn);
 strcat(fn,".");
 /* type */
 strncat(fn,pp+8,3);
 cut_spaces(fn);

 if(strcmp(fn,"$$$.sub")==0 || strcmp(fn,"$$$.SUB")==0){
	strcpy(fn,subfile);
	return;
 }

 if(fname_lo_f)tolowerStr(fn);


 /* append UNIX-path */
 strcpy(f,fn);
 strcpy(fn,lib_path);
 strcat(fn,"/");
 strcat(fn,f);
/* printf(" make_lib_file_name=%s ",fn);/* */

}

void
make_file_name(fn,fcb)
char *fn;
BYTE *fcb;
{BYTE *pp;
 if(make_short_file_name(fn,fcb)) return; /* submit file */
 append_unix_path(fn,*fcb);
/*printf(" make_file_name=%s ",fn); /**/
}

make_short_file_name(fn,fcb)
char *fn;
BYTE *fcb;
{BYTE *pp;

 pp=fcb + 1;

 bzero(fn,9);
 strncpy(fn,pp,8);
 cut_spaces(fn);
 strcat(fn,".");
 /* type */
 strncat(fn,pp+8,3);
 cut_spaces(fn);

 if(strcmp(fn,"$$$.sub")==0 || strcmp(fn,"$$$.SUB")==0){
	strcpy(fn,subfile);
	return 1;
 }

 if(fname_lo_f)tolowerStr(fn);

 return 0;
}

/* append UNIX-path */
append_unix_path(fn,fcbd)
char *fn;
{
 int d;
 char f[256];
 ch_drv(fcbd);
 if(fcbd == 0){ d= bdos_cur_drv;}
 else{ d= fcbd -1;}
 strcpy(f,fn);
 strcpy(fn,drv[d].name);
 strcat(fn,"/");
 strcat(fn,f);
}

void
dollar(fn)
char *fn;
{char ss[256],*s,c;
 strcpy(ss,fn);
 for(s=ss;;){
	c= *s++;
	if(c=='\0'){*fn = '\0'; return;}
	if(c=='$') *fn++ = '\\';
	*fn++ =c;
 }
}







cut_spaces(fn)
char *fn;
{char *p;
 p=fn+strlen(fn);
 for(p--;;){
   if(*p != ' ') break;
   *p-- = '\0';
 }
}

ch_drv(fcb_drv)
{
 int d;
 if(fcb_drv == 0){ d= bdos_cur_drv;}
 else if(fcb_drv == '?'){ d= bdos_cur_drv;}
 else{ d= fcb_drv -1;}
 ch_dir(d);
}

/*
	--- File maintnance ---
*/
void
bdos_rename()
{
 char fn1[256],fn2[256];
 BYTE *fcb,*fcb2;

xputs(" bdos_rename ");

 fcb = space+wreg(DE);
 fcb2= space+wreg(DE)+16;
 make_file_name(fn1,fcb);
 make_file_name(fn2,fcb2);

 if(access(fn2,F_OK)==0){ /* file exists */
   reg(A)=wreg(HL)= 0xFF;
   return;
 }

 if(-1 == rename(fn1,fn2)){
   /* fail */
   reg(A)=wreg(HL)= 0xFF;
 }else{
   reg(A)=wreg(HL)= 0;
 }

}

bdos_erase()
{char fn1[256];
 BYTE *fcb;

xputs(" bdos_erase ");

 fcb = space+wreg(DE);
 make_file_name(fn1,fcb);

 if(-1 == unlink(fn1)){
   reg(A)=wreg(HL)= 0xFF;
 }else{
   reg(A)=wreg(HL)= 0;
 }
}

/*
	File I/O
 */
void
bdos_open()
{char fn[256];
 int cpd,fd;
 BYTE *fcb;

xputs(" bdos_open ");

 fcb = space+wreg(DE);
 make_file_name(fn,fcb);

#ifdef AHO
 if((cpd=check_opened(fn))!= -1){
xputs(" bdos_open:opened ");
	reg(A)=wreg(HL)=0x00;
	return;
 }
#endif AHO

 if((cpd=new_fd())== -1){
	/* no file-table */
	reg(A)=wreg(HL)=0xFF;
	return;
 }

 fd=open(fn,O_RDWR);
 if(fd==-1){
xputs(" openFailed ");
	if(bdos_lib_srch==0){ /* lib search OFF */
		reg(A)=wreg(HL)=0xFF; /* fail */
		return ;
	}
	make_lib_file_name(fn,fcb);
	fd=open(fn,O_RDWR);
	if(fd==-1){
		reg(A)=wreg(HL)=0xFF; /* fail */
		return ;
	}
 }
 cpmfil[cpd].fd=fd;
 cpmfil[cpd].opened=1;
 strcpy(cpmfil[cpd].name, fn);

 space[wreg(DE)+CPD] = cpd;
 space[wreg(DE)+CurBlkLo] = 0; /* current Block */
 space[wreg(DE)+CurBlkHi] = 0; /* current Block */
/* space[wreg(DE)+CurRec] = 0; /* current record */
 reg(A)=wreg(HL)=0x00;
}


void
bdos_creat()
{char fn[256];
 int cpd,fd;
 BYTE *fcb;

xputs(" bdos_creat ");
 fcb = space+wreg(DE);
 make_file_name(fn,fcb);

#ifdef AHO
 if((cpd=check_opened(fn))!= -1){
xputs(" bdos_open:opened ");
	close(cpmfil[cpd].fd);
 }
#endif AHO

 if((cpd=new_fd())== -1){
	/* no file-table */
	reg(A)=wreg(HL)=0xFF; /*fail*/
	return;
 }

 fd=open(fn,O_RDWR | O_CREAT, FCREATE_MODE);
 if(fd==-1){
xputs(" creOpenFailed ");
	reg(A)=wreg(HL)=0xFF; /*fail*/
	return ;
 }
 cpmfil[cpd].fd=fd;
 cpmfil[cpd].opened=1;
 strcpy(cpmfil[cpd].name, fn);

 space[wreg(DE)+CPD] = cpd;
 space[wreg(DE)+CurBlkLo] = 0; /* current Block */
 space[wreg(DE)+CurBlkHi] = 0; /* current Block */
/* space[wreg(DE)+CurRec] = 0; /* current record */
 reg(A)=wreg(HL)=0;
}


void
bdos_close()
{int cpd;
xputs(" bdos_close ");
 cpd=space[wreg(DE)+CPD];
 if(0== cpmfil[cpd].opened){
	reg(A)=wreg(HL)=0xFF; /*fail*/
	return ;
 }

 cpmfil[cpd].opened=0;
 if(-1==close(cpmfil[cpd].fd)){
xputs(" closeError ");
	reg(A)=wreg(HL)=0xFF; /*fail*/
	return ;
 }
 reg(A)=wreg(HL)=0;
}


void
bdos_read()
{int cpd,x;
 BYTE *fcb;
 long cur_rec;

 fcb = space+wreg(DE);
 cpd= *(fcb+CPD);
/*pr_stat();*/
 if(0== cpmfil[cpd].opened){
	reg(A)=wreg(HL)=0xFF;
	return ;
 }

 cur_rec=(long)((*(fcb+CurBlkHi)<<15)|(*(fcb+CurBlkLo)<<7))+*(fcb+CurRec);
 if(-1L ==(x=lseek(cpmfil[cpd].fd, (off_t)cur_rec*128, L_SET))){
	reg(A)=wreg(HL)=0xFF;
	return;
 }

 if(0>=(x=read(cpmfil[cpd].fd, &space[dma_adr],128))){
	reg(A)=wreg(HL)=0x01; /* EOF */
	return;
 }
xprintf(" bdos_read:(%d)x=%d ",cpd,x); /**/

 space[wreg(DE)+CurBlkHi] = cur_rec>>15;
 space[wreg(DE)+CurBlkLo] = cur_rec>>7;
 x=cur_rec &0x7F;x++;
 space[wreg(DE)+CurRec] = x;
 reg(A)=wreg(HL)=0x00; /* success */
}

void
bdos_write()
{int cpd,x;
 BYTE *fcb;
 long cur_rec;

 fcb = space+wreg(DE);
 cpd= *(fcb+CPD);
/*pr_stat();*/
 if(0== cpmfil[cpd].opened){
	reg(A)=wreg(HL)=0xFF;
	return ;
 }

 cur_rec=(long)((*(fcb+CurBlkHi)<<15)|(*(fcb+CurBlkLo)<<7))+*(fcb+CurRec);
 if(-1L ==(x=lseek(cpmfil[cpd].fd, (off_t)cur_rec*128, L_SET))){
	reg(A)=wreg(HL)=0xFF;
	return;
 }

 if(0>=(x= write(cpmfil[cpd].fd, &space[dma_adr],128))){
	reg(A)=wreg(HL)=0x01; /* EOF */
	return;
 }
xprintf(" bdos_write:(%d)x=%d ",cpd,x); /**/

 space[wreg(DE)+CurBlkHi] = cur_rec>>15;
 space[wreg(DE)+CurBlkLo] = cur_rec>>7;
 x=cur_rec &0x7F;x++;
 space[wreg(DE)+CurRec] = x;
 reg(A)=wreg(HL)=0x00; /* success */
}

/* random access */
void
bdos_rdrnd()
{
 int cpd,x;
 BYTE *fcb;
 int cur_rec;

xputs(" bdos_rdrnd ");

 fcb = space+wreg(DE);
 cpd= *(fcb+CPD);
/*pr_stat();*/
 if(0== cpmfil[cpd].opened){
	reg(A)=wreg(HL)=0xFF;
	return ;
 }

 cur_rec= (*(fcb+35)<<16) | (*(fcb+34)<< 8) | *(fcb+33);

 if(-1L ==(x=lseek(cpmfil[cpd].fd, (off_t)cur_rec*128, L_SET))){
	reg(A)=wreg(HL)=0xFF;
	return;
 }

 if(0>=(x=read(cpmfil[cpd].fd, &space[dma_adr],128))){
	reg(A)=wreg(HL)=0x01; /* EOF */
	return;
 }
xprintf(" bdos_rdrnd:(%d)x=%d ",cpd,x); /**/

 space[wreg(DE)+CurBlkHi] = cur_rec>>15;
 space[wreg(DE)+CurBlkLo] = cur_rec>>7;
 x=cur_rec &0x7F;
 space[wreg(DE)+CurRec] = (x==0)?128:x;
 reg(A)=wreg(HL)=0x00; /* success */
}




void
bdos_wrrnd()
{
 int cpd,x;
 BYTE *fcb;
 int cur_rec;

xputs(" bdos_wrrnd ");

 fcb = space+wreg(DE);
 cpd= *(fcb+CPD);
/*pr_stat();*/
 if(0== cpmfil[cpd].opened){
	reg(A)=wreg(HL)=0xFF;
	return ;
 }

 cur_rec= (*(fcb+35)<<16) | (*(fcb+34)<< 8) | *(fcb+33);

 if(-1L ==(x=lseek(cpmfil[cpd].fd, (off_t)cur_rec*128, L_SET))){
	reg(A)=wreg(HL)=0xFF;
	return;
 }

 if(0>=(x= write(cpmfil[cpd].fd, &space[dma_adr],128))){
	reg(A)=wreg(HL)=0x01; /* EOF */
	return;
 }
xprintf(" bdos_wrrnd:(%d)x=%d ",cpd,x); /**/

 space[wreg(DE)+CurBlkHi] = cur_rec>>15;
 space[wreg(DE)+CurBlkLo] = cur_rec>>7;
 x=cur_rec &0x7F;
 space[wreg(DE)+CurRec] = (x==0)?128:x;
 reg(A)=wreg(HL)=0x00; /* success */
}

bdos_setrrec()
{
 int cpd,x;
 BYTE *fcb;
 int cur_rec;

xputs(" bdos_setrrec ");

 fcb = space+wreg(DE);

 cur_rec= (long)(*(fcb+CurBlkHi)<<15) | (*(fcb+CurBlkLo)<<7) | *(fcb+CurRec);

 space[wreg(DE)+33] = cur_rec;
 space[wreg(DE)+34] = cur_rec>>8;
 space[wreg(DE)+35] = cur_rec>>16;
 reg(A)=wreg(HL)=0x00; /* success */
}

void
bdos_fsize()
{
int cpd;
 BYTE *fcb;
 int rec,x;


 fcb = space+wreg(DE);
 cpd= *(fcb+CPD);
/*pr_stat();*/
 if(0== cpmfil[cpd].opened){
	reg(A)=wreg(HL)=0xFF;
	return ;
 }

 if(-1L ==(x=lseek(cpmfil[cpd].fd, 0L , L_XTND))){
	reg(A)=wreg(HL)=0xFF;
	return;
 }
 
 rec = (x+1)/128; /* next record */
xprintf(" bdos_fsize:(%d,%d)",x,rec);
 space[wreg(DE)+33] = rec;
 space[wreg(DE)+34] = rec>>8;
 space[wreg(DE)+35] = rec>>16;
 reg(A)=wreg(HL)=0x00; /* success */
}






/*
	Directory
*/
#define DIR_MAX 500
static char dir[DIR_MAX][256];
static int dir_n, dir_p;

void
bdos_src1st()
{
 FILE *f;
 int i,x;
 char cmd[256],fn[256];
 char gazonk[256],gazonk1[256],gazonk2[256],*p;
 BYTE *fcb;

 xputs(" bdos_src1st ");
 
 strcpy(cmd,"/bin/ls ");
 fcb = space+wreg(DE);
 ch_drv(*fcb);
 make_short_file_name(fn,fcb);
 dollar(fn);
 strcat(cmd,fn);
 if(NULL==(f=popen(cmd , "r"))){
   reg(A)=wreg(HL)=0xFF; /*fail */
   return;
 }

 for(i=0;i<DIR_MAX;i++){
	if(NULL==fgets(dir[i],256,f)) break;
xprintf("\r\nd[%d]=",i); xprintf("%s",dir[i]);
 }
 x=pclose(f);
xprintf("pclose=%d\r\n", x);
 dir_n=i;

 if(dir_n<1){ dir_p= dir_n=0; reg(A)=wreg(HL)=0xFF;  return; }
#ifdef AHO
 if(dir_n==1 && (3==sscanf(dir[0],"%s %s %s",gazonk,gazonk1,gazonk2))){
   /* file not found */
   dir_p= dir_n=0; reg(A)=wreg(HL)=0xFF; return;
 }
#endif

 /* file 1st entry to DMA*/
 set_dir_to_dma(dir[0]);

 space[wreg(DE)+CurBlkLo] = 0; /* current Block */
 space[wreg(DE)+CurBlkHi] = 0; /* current Block */
/* space[wreg(DE)+CurRec] = 0; /* current record */
 reg(A)=wreg(HL)=0;
 dir_p=1;
}

set_dir_to_dma(dir)
char *dir;
{
 BYTE *dma;
 char fn[256],*p;

 dma= &space[dma_adr];
 bzero(dma,32);
 dma[0]= 0; /* user id */

 if(NULL!=(p= (char*)index(dir,'\n'))){ *p = '\0';}

 p= (char*)index(dir,'.');
 *p++ = '\0';

 strcpy(fn,dir); strcat(fn,"        "); strncpy(dma+1,fn,8); /* file name */

 strcpy(fn,p); strcat(fn,"   "); strncpy(dma+9,fn,3);  /* file type */
}


void
bdos_srcnxt()
{
 xputs(" bdos_srcnxt ");
 if(dir_n <=0 ){ reg(A)=wreg(HL)= 0xFF; return; /* not found */ }
 if(dir_n <= dir_p){ reg(A)=wreg(HL)= 0xFF; return; /* all done */ }
 set_dir_to_dma(dir[dir_p]);

 space[wreg(DE)+CurBlkLo] = 0; /* current Block */
 space[wreg(DE)+CurBlkHi] = 0; /* current Block */
/* space[wreg(DE)+CurRec] = 0; /* current record */
 reg(A)=wreg(HL)=0;
 dir_p++;
}

/*
	Disk System
*/

bdos_fatr()
{
 /* Not Supported */
 reg(A)=wreg(HL)= 0;
 if(verbose) verbose_msg("bdos_setFileAttr called");
}

bdos_setdma()
{
  wreg(BC)=wreg(DE);
  bios_setdma();
}

bdos_reset()
{
 closeAll();
 wreg(DE)= DEFDMA;
 bdos_setdma();
}

/* */


bdos_setcurdrv()
{
 ch_dir(reg(E));
 bdos_cur_drv=reg(E);
}

bdos_curdrv()
{
 reg(A)= wreg(HL)= bdos_cur_drv;
}

bdos_getlogvec()
{
 reg(A)=wreg(HL)= bdos_logvec;
}


bdos_protect()
{
 /* Not Supported */
 reg(A)=wreg(HL)= 0;
 if(verbose) verbose_msg("bdos_protect called");
}

bdos_rovec()
{
 /* Not Supported */
 wreg(HL)= 0;
 if(verbose) verbose_msg("bdos_protectDisk called");
}


/* not yet  IIKAGEN */
bdos_allcvec()
{
 int dph;
 if(verbose)verbose_msg("bdos_allcvec called");

 reg(C)= bdos_cur_drv;
 bios_seldsk();
 dph=wreg(HL);
 wreg(HL)= (space[dph+15] <<8) | space[dph+14]; /* alv */
}

bdos_dpba()
{
 int dph;
 if(verbose)verbose_msg("bdos_dpbAdrs called");

 reg(C)= bdos_cur_drv;
 bios_seldsk();
 dph=wreg(HL);
 wreg(HL)= (space[dph+11] <<8) | space[dph+10]; /* dpb */
}




/*
	wboot
*/
bdos_wboot()
{
	longjmp(ccpWbootEntry,-1);
}

/*
	get version
*/
bdos_getver()
{
  reg(A)=wreg(HL)= 0x0022;
}

/*
	user
*/
void
bdos_user()
{
 if(reg(E) == 0xFF){ reg(A)= wreg(HL)= bdos_cur_user; return; }
 bdos_cur_user = reg(E);
 reg(A)=wreg(HL)= 0;
 bdos_chdir(bdos_cur_drv,bdos_cur_user);
}



/*
	console I/O
*/
void
bdos_const()
{int c;
 if(c_buf != -1){
	reg(A)= wreg(HL)= 1; /* exist char */
	return;
 }

 bios_const();
 if(!reg(A)){  wreg(HL)= 0; return;  /* reg(A)=0;  No char*/ }
 /* pause check*/
 c=bios_conin();
 if(c == 0x13){ /* ^S */
	c=bios_conin();
	c_buf= -1;
	reg(A)= wreg(HL)= 0; /* No char */
	if(c == 0x03) longjmp(ccpWbootEntry,-1);
 }else{
	reg(A)= wreg(HL)= 1; /* exist char */
	c_buf=c;
 }
}


bdos_conin()
{int c;

 if(c_buf != -1){
	reg(E)=c= c_buf;
	c_buf= -1;
 }else{
	bios_conin();
	reg(E)=c=reg(A);
 }

 if(c >=' '){
   bdos_conout();
 }else if(c =='\r'){
   bdos_conout();
 }else if(c =='\n'){
   bdos_conout();
 }else if(c =='\b'){
   bdos_conout();
 }else if(c =='\t'){ /* ^I */
   bdos_conout();
 }
 reg(A)=wreg(HL)=c;
}

bdos_conout()
{static int cc=0;
 int c,x;

 bdos_const(); /* pause check */

 c=reg(E);
/* printf(" bdos_conout(%02x) ",c); */

 if(c =='\t'){ /* ^I */
   x=((cc/8)+1)*8;
   for(;x>cc;cc++){
     reg(C)=' ';
     bios_conout();
   }
 }else{
   reg(C)=c;
   bios_conout();
   if(c >=' '){
     cc++;
   }else if(c =='\r'){
     cc=0;
   }else if(c =='\n'){
     cc=0;
   }else if(c =='\b'){
     cc--;
   }
 }
}

void
bdos_dirio()
{
  if(reg(E)==0xFF){ /* getc */
    bios_const();
    if(reg(A)==0){
      reg(A)=wreg(HL)=0; return;
    }else{
      bios_conin(); return;
    }
  }else{
    reg(C)=reg(E);
    bios_conout();
  }
}


/* reader/puncher, list */

bdos_rdrin()
{
  bios_reader();
}

bdos_punout()
{
  reg(C)=reg(E);
  bios_punch();
}

bdos_lstout()
{
  reg(C)=reg(E);
  bios_list();
}


/* iobyte */
bdos_getiob()
{
 reg(A)=wreg(HL)=space[3];
}

bdos_setiob()
{
 space[3] = reg(E);
}

/* string I/O */
void
bdos_putstr()
{BYTE *p;
 int x;
 p= space+ wreg(DE);
 for(;;){
   x = *p++;
   if(x=='$') return;
   reg(E)= x;
   bdos_conout();
 }
}

/*---*/

console_gets(s,max)
char *s;
int max;
{
 return linEd(s,max);
}


static char bdos_gets_hist[256];

bdos_getstr()
{int x;
 char buf[256];
 int max,l;

 max=space[wreg(DE)];
 x=NULL;

 if(xsub){
	x=submit(buf);
 }

 if(x==0){
	c_buf= -1; /* for conin() and const() */
	strcpy(buf,"");
	for(;;){
		x=console_gets(buf,max+1);
		if(x==0){ strcpy(bdos_gets_hist,buf);break;}
		strcpy(buf,bdos_gets_hist);
	}
 }

 space[wreg(DE)+1]=l=strlen(buf);
 bcopy(buf,space+wreg(DE)+2,MIN(max,l));
 reg(E)= '\r'; bdos_conout();
}


/*
*/
verbose_msg(s)
char *s;
{
	printf("\r\n*** %s ",s);
	pr_stat1();
	printf(" ***\r\n");
}

/*
static int aho=0;
if(aho++ > 5) the_exit("");
*/
void
ch_dir(d)
{
 if(15<d){
	printf("\r\nBdos Err On Drive Select\r\n");
	space[4]=0;
	bdos_conin();
	longjmp(ccpWbootEntry,-1);
	return;
 }
 if(0==((1 <<d) & bdos_logvec)){
    printf("\r\nBdos Err On %c: Select(not assigned to UNIX-path)",'A'+d);
	space[4]=0;
	bdos_conin();
	longjmp(ccpWbootEntry,-1);
	d=0xFF;
	return;
 }
 if(-1==bdos_chdir(d,bdos_cur_user)){
    printf("\r\nBdos Err On %c: Select(cannot chdir )(^C:exit CP/sim)",
		'A'+d);
	space[4]=0;
	bdos_conin();
	if(reg(A)== 0x03) the_exit("\r\nBye Bye.\r\n");
	longjmp(ccpWbootEntry,-1);
 }
}

int
bdos_chdir(d,usr)
int d,usr;
{
 char fn[256];
 strcpy(fn,drv[d].name);
 return chdir(fn);
}

