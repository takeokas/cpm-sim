/*
	80.h
	8080 Machine Interpreter
		   by
		S.Takeoka

	Aug/18/1987
	Aug/15/1987
*/

typedef	unsigned char	BYTE;
typedef	unsigned short	BYTE2;
typedef	unsigned int	BYTE4;

#define	CODESIZ	65536

/*	Program CodeArea	*/
extern BYTE	space[CODESIZ],
		*p_c,
		*s_p;

/*	8080 Register	*/
#ifdef	B_ENDIAN
extern	BYTE2	preg[4];
#define	wreg(x)	preg[x]
#define	reg(x)	(*((BYTE *)preg+(x)))
#endif

#ifdef	L_ENDIAN
extern	BYTE2	preg[4];
#define	wreg(x)	preg[x]
#define	reg(x)	(*(((BYTE *)preg)+((x) ^ 1)))
#endif

/*	Flags	*/
extern	unsigned int	c_y;

#define	z_flg_test(x) ( !((x)&0xFF))
#define	s_flg_test(x) ((x)&0x80)

/* BYTE4 xx4 must be decleared !! parity-odd */
#define	po_flg_test(x) \
	(xx4=(x),xx4=(xx4>>4)^xx4,xx4=(xx4>>2)^xx4,(((xx4>>1)^xx4)&1)  )



extern	BYTE4	lst_acc;
extern	BYTE	opecode;

#define	BC	0
#define	DE	1
#define	HL	2
#define	SP	3
#define	AF	3

#define	B	0
#define	C	1
#define	D	2
#define	E	3
#define	H	4
#define	L	5
#define	M space[wreg(2)]	/* M register */
#define	A	7

/*	varible for Monitor	*/

extern	int	itrace;
extern	jmp_buf	jbuf;

