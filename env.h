/*
	env.h
	8080 Machine Interpreter
		   by
		S.Takeoka
*/


extern jmp_buf	ccpWbootEntry;

/* submit */
extern int xsub;
extern int submit();

#define SUBFILE		"/tmp/$$$.sub"
#define SUBFILE2	"/tmp/$$$.$$$"

extern char	subfile[];
extern char	subfile2[];

/*
  environment Flags
*/
extern int verbose;
extern int fname_lo_f;
extern int bdos_lib_srch;
/*
  ccp envirroment
*/
extern char com_path[];
extern char lib_path[];
extern int ccp_upper_f;

#ifdef SYSV
# define MAXPATHLEN	1024
char *getcwd();
# define getwd(x)	getcwd((x), MAXPATHLEN)
#else
char *getwd();
#endif
