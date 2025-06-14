/*
	bdosfunc.h
	8080 Machine Interpreter
		   by
		S.Takeoka

	Jan/04/1992
*/

/* Bdos function No. */
#define WBOOT	0
#define CONIN	1
#define CONOUT	2
#define RDRIN	3		/* nop */
#define PUNOUT	4		/* nop */
#define LSTOUT	5		/* nop */
#define DIRIO	6
#define GETIOB	7
#define SETIOB	8
#define PUTSTR	9
#define GETSTR	10
#define CONST	11
#define VER	12
#define RESET	13
#define SELDSK	14
#define OPEN	15
#define CLOSE	16
#define SRC1ST	17
#define SRCNXT	18
#define FERA	19
#define READ	20
#define WRITE	21
#define FCREA	22
#define FREN	23
#define LOGVEC	24		/* fake */
#define CURDRV	25
#define SETDMA	26
#define ALLCVEC	27		/* not supported (fake)*/
#define PROTECT	28		/* not supported (nop) */
#define ROVEC	29		/* not supported (fake) */
#define FATR	30		/* not supported (nop) */
#define DPBA	31		/* fake */
#define USER	32
#define RDRND	33
#define WRRND	34
#define FSIZE	35
#define SETRREC	36

/*  Extended for CP/sim  */
#define XSUB	255

