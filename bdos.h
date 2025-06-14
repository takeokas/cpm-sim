/*
	bdos.h
	8080 Machine Interpreter
		   by
		S.Takeoka
*/

/* CP/M System allocated area */
/* #define BDOS_TOP	0xEC06  /* 64K CP/M */
#define BDOS_TOP	0xF906  /*  */

#define BIOS_TOP	0xFA00
#define BIOS_ENT	(BIOS_TOP+3)
#define DEF_USR_STK	(BIOS_TOP-2)
/*#define DEF_USR_STK	0xF9FE */

#define CPM_MAX_FILES	10

#define DEFDMA	0x0080

extern int dma_adr;
extern int bdos_logvec;

struct	drive_t	{
	int	rw;
	char	name[MAXPATHLEN];
}; 

extern	struct	drive_t	drv[16];
