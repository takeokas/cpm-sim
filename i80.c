/*
	i80.c
	8080 Machine Interpreter
	8080 Machine Executer
		   by
		S.Takeoka

	Ver.1.1
	Jan/27/1992 (daa debug)
	Jan/10/1992 (Parity)
	Aug/18/1987
	Aug/15/1987
*/
#include "copying.h"

#define VERSION "v1.11"

#include <stdio.h>
#include <setjmp.h>
#include <ctype.h>
#ifdef LINUX
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <termio.h>
#include <errno.h>
#endif /*LINUX*/

#include "i80.h"

#ifdef B_ENDIAN
BYTE2	preg[4];
#endif

#ifdef L_ENDIAN
BYTE2	preg[4];
#endif

extern	BYTE *break_point;

/*	Flags	*/
unsigned int c_y;

/*	Program CodeArea	*/
BYTE	space[CODESIZ],
	*p_c,
	*s_p;

/*	Last Values	*/
BYTE	opecode;
BYTE4	lst_acc;

/*
	Macros
*/

#define	c_flg_test(x) (cy=(x)&0x100 ? 1 : 0)

#define	brk(x)	{ \
	opecode= *(pc-x); \
	p_c=pc; s_p=sp; \
	lst_acc=last_acc; \
	c_y=cy; \
	longjmp(jbuf,opecode); \
}

#define	EMT(x)	{ \
	opecode= *(pc-1); \
	p_c=pc; s_p=sp; \
	lst_acc=last_acc; \
	c_y=cy; \
	emt(x); \
}

/*	8080 instruction	*/

#define	nop()	/* this is NOP ! */

/*
#define	undef()	{ \
	fprintf(stderr,"\nundef'd instruction %04x:%02x\n",pc-space,*(pc-1)); \
	pc= space+ 0xFFF8; \
}
*/

#define	undef()	{ \
	nputs(" undef"); \
	brk(1); \
}

#define	hlt()	{ \
	nputs(" hlt"); \
	brk(1); \
}

#define	mov(d,s)	reg(d) = reg(s)

#define	movrm(d)	reg(d) = M

#define	movmr(s)	M = reg(s)

#define	mvi(r)		reg(r)= *pc++

#define	mvim()		M = *pc++

#define	inr(r)		last_acc= ++reg(r)

#define	inrm()		last_acc= ++M

#define	dcr(r)		last_acc= --reg(r)

#define	dcrm()		last_acc= --M

#define	add(r) \
{ \
 reg(7)=last_acc=(last_d=reg(7))+(last_s=reg(r)); \
 c_flg_test(last_acc); \
}

#define	addm() \
{ \
 reg(7)=last_acc=(last_d=reg(7))+(last_s=M); \
 c_flg_test(last_acc); \
}

#define	adi() \
{ \
 reg(7)=last_acc=(last_d=reg(7))+(last_s= *pc++); \
 c_flg_test(last_acc); \
}

#define	adc(r) \
{ \
 reg(7)=last_acc=(last_d=reg(7))+(last_s=reg(r)+cy); \
 c_flg_test(last_acc); \
}

#define	adcm() \
{ \
 reg(7)=last_acc=(last_d=reg(7))+(last_s=M+cy); \
 c_flg_test(last_acc); \
}

#define	aci() \
{ \
 reg(7)=last_acc=(last_d=reg(7))+(last_s= *pc++ +cy); \
 c_flg_test(last_acc); \
}

#define	sub(r) \
{ \
 reg(7)=last_acc=reg(7)-reg(r); \
 c_flg_test(last_acc); \
}

#define	subm() \
{ \
 reg(7)=last_acc=reg(7)-M; \
 c_flg_test(last_acc); \
}

#define	sui() \
{ \
 reg(7)=last_acc=reg(7)- *pc++; \
 c_flg_test(last_acc); \
}

#define	sbb(r) \
{ \
 reg(7)=last_acc=reg(7)-reg(r)-cy; \
 c_flg_test(last_acc); \
}

#define	sbbm() \
{ \
 reg(7)=last_acc=reg(7)-M-cy; \
 c_flg_test(last_acc); \
}

#define	sbi() \
{ \
 reg(7)=last_acc=reg(7)- *pc++ -cy; \
 c_flg_test(last_acc); \
}

#define	ana(r) \
{ \
 reg(7)=last_acc=reg(7) & reg(r); \
 cy=0; \
}

#define	anam() \
{ \
 reg(7)=last_acc=reg(7) & M; \
 cy=0; \
}

#define	ani() \
{ \
 reg(7)=last_acc=reg(7) & *pc++; \
 cy=0; \
}


#define	xra(r) \
{ \
 reg(7)=last_acc=reg(7) ^ reg(r); \
 cy=0; \
}

#define	xram() \
{ \
 reg(7)=last_acc=reg(7) ^ M; \
 cy=0; \
}

#define	xri() \
{ \
 reg(7)=last_acc=reg(7) ^ *pc++; \
 cy=0; \
}

#define	ora(r) \
{ \
 reg(7)=last_acc=reg(7) | reg(r); \
 cy=0; \
}

#define	oram() \
{ \
 reg(7)=last_acc=reg(7) | M; \
 cy=0; \
}

#define	ori() \
{ \
 reg(7)=last_acc=reg(7) | *pc++; \
 cy=0; \
}

#define	cmp(r) \
{ \
 last_acc=reg(7) - reg(r); \
 c_flg_test(last_acc); \
}

#define	cmpm() \
{ \
 last_acc=reg(7) - M; \
 c_flg_test(last_acc); \
}

#define	cpi() \
{ \
 last_acc=reg(7) - *pc++; \
 c_flg_test(last_acc); \
}

#define	rlc() \
{ \
 xxx=reg(7); \
 reg(7)= (xxx<<1) | (cy=xxx>>7); \
}

#define	rrc() \
{ \
 xxx=reg(7); \
 reg(7)= (xxx>>1) | (xxx<<7)&0x80; \
 cy= xxx & 1 ? 1 : 0; \
}

#define	ral() \
{ \
 reg(7) =xw1= (reg(7)<<1) | cy ; \
 cy= xw1 & 0x0100 ? 1 : 0 ; \
}

#define	rar() \
{ \
 xxx=reg(7); \
 reg(7)= (xxx>>1) | (cy ? 0x80 : 0); \
 cy=xxx & 1; \
}

#ifndef OLD
#define	daa() \
{ \
 xxx = reg(7); \
 if((((last_s & 0xF)+(last_d & 0xF)) & 0x10) || ((xxx & 0xf) > 9)) \
			xxx += 0x6; \
 if(cy || (xxx & 0xf0) > 0x90 ){ \
	reg(7) = last_acc = xxx + 0x60; \
	cy=1; \
 }else{ \
 	reg(7) = last_acc = xxx; \
	c_flg_test(last_acc); \
 } \
}
#else /*OLD*/
#define	daa() \
{ \
 xxx = reg(7); \
 if((((last_s & 0xF)+(last_d & 0xF)) & 0x10) || ((xxx & 0xf) > 9)) \
			xxx += 0x6; \
 if(cy || (xxx & 0x1f0) > 0x90 ){ \
	reg(7) = last_acc = xxx + 0x60; \
	cy=1; \
 }else{ \
 	reg(7) = last_acc = xxx; \
	c_flg_test(last_acc); \
 } \
}
#endif

#define	cma()		reg(7) = ~reg(7)

#define	stc()		cy=1

#define	cmc()		cy= !cy

#define	lxi(wr) \
{ \
 reg(wr*2+1)= *pc++; reg(wr*2)= *pc++; \
}

#define	lxisp() \
{ \
 xxx= *pc++; \
 sp= space+((*pc++ << 8) | xxx); \
}

#define	dad(wr) \
{ \
 wreg(2)=xx4=wreg(2)+wreg(wr); \
 cy= (xx4 & 0x10000) ? 1 : 0; \
}

#define	dadsp() \
{ \
 wreg(2)=xx4=wreg(2)+ (sp -space); \
 cy= (xx4 & 0x10000) ? 1 : 0; \
}

#define	stax(wr) \
{ \
 space[wreg(wr)]=reg(7); \
}

#define	ldax(wr) \
{ \
 reg(7)=space[wreg(wr)]; \
}

#define	shld() \
{ \
 xxx= *pc++; \
 p= &space[(*pc++ << 8)|xxx]; \
 *p++= reg(5); \
 *p  = reg(4); \
}

#define	lhld() \
{ \
 xxx= *pc++; \
 p= &space[(*pc++ << 8)|xxx]; \
 reg(5)= *p++; \
 reg(4)= *p; \
}

#define	sta() \
{ \
 xxx= *pc++; \
 space[(*pc++ << 8)|xxx]=reg(7); \
}

#define	lda() \
{ \
 xxx= *pc++; \
 reg(7)=space[(*pc++ << 8)|xxx]; \
}

#define	inx(wr)		wreg(wr)++

#define	inxsp()		sp++

#define	dcx(wr)		wreg(wr)--

#define	dcxsp()		sp--

#define	out() \
{ \
	nputs(" out"); \
	brk(1); \
}

#define	in() \
{ \
	nputs(" in"); \
	brk(1); \
}

#define	xthl() \
{ \
 xxx= *sp; \
 *sp= reg(5); \
 reg(5)= xxx; \
 xxx= *(sp+1); \
 *(sp+1)= reg(4); \
 reg(4)= xxx; \
}

#define	xchg() \
{ \
 xw1=wreg(2); \
 wreg(2)=wreg(1); \
 wreg(1)=xw1; \
}

#define	di() \
{ \
}

#define	ei() \
{ \
}

#define	push(wr) \
{ \
 xw1= wreg(wr); \
 *(--sp) = xw1>>8; \
 *(--sp) = xw1; \
}

#define	pop(wr) \
{ \
 xxx= *sp++; \
 wreg(wr)= (*sp++ <<8) | xxx; \
}


#ifdef OLD
#define	pushpsw() \
{ \
 *(--sp) = reg(7); \
 *(--sp) = (s_flg_test(last_acc) ? 0x80 : 0) \
	 | (z_flg_test(last_acc) ? 0x40 : 0) \
	 | (cy ? 1 : 0) \
	 | (((last_s & 0xF)  + (last_d & 0xF))&0x10) ; \
}
#endif /*OLD*/

#ifdef PARITY
#define	pushpsw() \
{ \
 *(--sp) = reg(7); \
 *(--sp) = (s_flg_test(last_acc)) \
	 | (z_flg_test(last_acc) << 6) \
	 | (cy ? 1 : 0) \
	 | ((po_flg_test(last_acc)^1) <<2) \
	 | (((last_s & 0xF)  + (last_d & 0xF))&0x10) ; \
}

#define	poppsw() \
{ \
 xx4= *sp++;	/* F reg pop */ \
 reg(7)= *sp++; \
 cy= xx4 & 1; \
 last_acc= xx4 & 0xc0 ^ 0x40; \
 if(((last_acc<<1) ^ last_acc)& 0x80){ /* now Odd */ \
		if((xx4 & 0x4)) last_acc++; \
 }else{ /* now Even */ \
		if(!(xx4 & 0x4)) last_acc++; \
 } \
 if(xx4 & 0x10){ last_s=last_d=9;} \
 else{last_s=last_d=0;} \
}


#else /*PARITY*/

#define	pushpsw() \
{ \
 *(--sp) = reg(7); \
 *(--sp) = (s_flg_test(last_acc)) \
	 | (z_flg_test(last_acc) << 6) \
	 | (cy ? 1 : 0) \
	 | (((last_s & 0xF)  + (last_d & 0xF))&0x10) ; \
}

#define	poppsw() \
{ \
 xxx= *sp++;	/* F reg pop */ \
 reg(7)= *sp++; \
 cy= xxx & 1; \
 last_acc= xxx & 0xc0 ^ 0x40; \
 if(xxx &0x10){ last_s=last_d=9;} \
 else{last_s=last_d=0;} \
}

#endif /*!PARITY*/



#define	pchl()		pc= space+wreg(2)

#define	sphl()		sp= space+wreg(2)

#define	jmp() \
{ \
 xxx= *pc++; \
 pc= space+((*pc << 8) | xxx); \
}

#define	jnz() \
{ \
 if(! z_flg_test(last_acc)){ \
	xxx= *pc++; \
	pc= space+((*pc << 8) | xxx); \
 }else{ \
	pc+=2; \
 } \
}

#define	jz() \
{ \
 if(z_flg_test(last_acc)){ \
	xxx= *pc++; \
	pc= space+((*pc << 8) | xxx); \
 }else{ \
	pc+=2; \
 } \
}

#define	jnc() \
{ \
 if(!cy){ \
	xxx= *pc++; \
	pc= space+((*pc << 8) | xxx); \
 }else{ \
	pc+=2; \
 } \
}

#define	jc() \
{ \
 if(cy){ \
	xxx= *pc++; \
	pc= space+((*pc << 8) | xxx); \
 }else{ \
	pc+=2; \
 } \
}


#ifdef PARITY

#define	jpo() \
{ \
 if(po_flg_test(last_acc)){ \
	xxx= *pc++; \
	pc= space+((*pc << 8) | xxx); \
 }else{ \
	pc+=2; \
 } \
}

#define	jpe() \
{ \
 if(!po_flg_test(last_acc)){ \
	xxx= *pc++; \
	pc= space+((*pc << 8) | xxx); \
 }else{ \
	pc+=2; \
 } \
}

#else /*!PARITY*/

#define	jpo(){ \
	nputs(" jpo"); \
	brk(1); \
}

#define	jpe(){ \
	nputs(" jpe"); \
	brk(1); \
}

#endif /*!PARITY*/


#define	jp() \
{ \
 if(! s_flg_test(last_acc)){ \
	xxx= *pc++; \
	pc= space+((*pc << 8) | xxx); \
 }else{ \
	pc+=2; \
 } \
}

#define	jm() \
{ \
 if(s_flg_test(last_acc)){ \
	xxx= *pc++; \
	pc= space+((*pc << 8) | xxx); \
 }else{ \
	pc+=2; \
 } \
}


#define	call() \
{ \
 xw1= pc-space +2; \
 *(--sp) = xw1>>8; \
 *(--sp) = xw1; \
 xxx= *pc++; \
 pc= space+((*pc << 8) | xxx); \
}

#define	cnz() \
{ \
 if(! z_flg_test(last_acc)){ \
	xw1= pc-space +2; \
	*(--sp) = xw1>>8; \
	*(--sp) = xw1; \
	xxx= *pc++; \
	pc= space+((*pc << 8) | xxx); \
 }else{ \
	pc+=2; \
 } \
}

#define	cz() \
{ \
 if(z_flg_test(last_acc)){ \
	xw1= pc-space +2; \
	*(--sp) = xw1>>8; \
	*(--sp) = xw1; \
	xxx= *pc++; \
	pc= space+((*pc << 8) | xxx); \
 }else{ \
	pc+=2; \
 } \
}

#define	cnc() \
{ \
 if(!cy){ \
	xw1= pc-space +2; \
	*(--sp) = xw1>>8; \
	*(--sp) = xw1; \
	xxx= *pc++; \
	pc= space+((*pc << 8) | xxx); \
 }else{ \
	pc+=2; \
 } \
}

#define	cc() \
{ \
 if(cy){ \
	xw1= pc-space +2; \
	*(--sp) = xw1>>8; \
	*(--sp) = xw1; \
	xxx= *pc++; \
	pc= space+((*pc << 8) | xxx); \
 }else{ \
	pc+=2; \
 } \
}

#ifdef PARITY

#define	cpo() \
{ \
 if(po_flg_test(last_acc)){ \
	xw1= pc-space +2; \
	*(--sp) = xw1>>8; \
	*(--sp) = xw1; \
	xxx= *pc++; \
	pc= space+((*pc << 8) | xxx); \
 }else{ \
	pc+=2; \
 } \
}

#define	cpe() \
{ \
 if(!po_flg_test(last_acc)){ \
	xw1= pc-space +2; \
	*(--sp) = xw1>>8; \
	*(--sp) = xw1; \
	xxx= *pc++; \
	pc= space+((*pc << 8) | xxx); \
 }else{ \
	pc+=2; \
 } \
}

#else /*!PARITY*/

#define	cpo(){ \
	nputs(" cpo"); \
	brk(1); \
}

#define	cpe(){ \
	nputs(" cpe"); \
	brk(1); \
}

#endif /*!PARITY*/

#define	cp() \
{ \
 if(! s_flg_test(last_acc)){ \
	xw1= pc-space +2; \
	*(--sp) = xw1>>8; \
	*(--sp) = xw1; \
	xxx= *pc++; \
	pc= space+((*pc << 8) | xxx); \
 }else{ \
	pc+=2; \
 } \
}

#define	cm() \
{ \
 if(s_flg_test(last_acc)){ \
	xw1= pc-space +2; \
	*(--sp) = xw1>>8; \
	*(--sp) = xw1; \
	xxx= *pc++; \
	pc= space+((*pc << 8) | xxx); \
 }else{ \
	pc+=2; \
 } \
}

#define	ret() \
{ \
 xxx= *sp++; \
 pc= space+ (((*sp++)<<8)|xxx); \
}

#define	rnz() \
{ \
 if(! z_flg_test(last_acc)){ \
	xxx= *sp++; \
	pc= space+ (((*sp++)<<8)|xxx); \
 } \
}

#define	rz() \
{ \
 if(z_flg_test(last_acc)){ \
	xxx= *sp++; \
	pc= space+ (((*sp++)<<8)|xxx); \
 } \
}

#define	rnc() \
{ \
 if(! cy){ \
	xxx= *sp++; \
	pc= space+ (((*sp++)<<8)|xxx); \
 } \
}

#define	rc() \
{ \
 if(cy){ \
	xxx= *sp++; \
	pc= space+ (((*sp++)<<8)|xxx); \
 } \
}

#ifdef PARITY

#define	rpo() \
{ \
 if(po_flg_test(last_acc)){ \
	xxx= *sp++; \
	pc= space+ (((*sp++)<<8)|xxx); \
 } \
}

#define	rpe() \
{ \
 if(!po_flg_test(last_acc)){ \
	xxx= *sp++; \
	pc= space+ (((*sp++)<<8)|xxx); \
 } \
}

#else /*!PARITY*/

#define	rpo(){ \
	nputs(" rpo"); \
	brk(1); \
}

#define	rpe(){ \
	nputs(" rpe"); \
	brk(1); \
}

#endif /*!PARITY*/

#define	rp() \
{ \
 if(! s_flg_test(last_acc)){ \
	xxx= *sp++; \
	pc= space+ (((*sp++)<<8)|xxx); \
 } \
}

#define	rm() \
{ \
 if(s_flg_test(last_acc)){ \
	xxx= *sp++; \
	pc= space+ (((*sp++)<<8)|xxx); \
 } \
}

#define	rst(adr) \
{ \
 xw1= pc-space; \
 *(--sp) = xw1>>8; \
 *(--sp) = xw1; \
 pc= space + (adr*8); \
}


/*
*/

exqt(pc,sp)
register BYTE *pc,*sp;
{
 register BYTE xxx,cy;
 register BYTE2 xw1;
 register BYTE4 xx4;
 register BYTE *p;
 register BYTE4	last_acc;
 register BYTE last_s, last_d;

 for(;;){
   switch(*pc++){
	case	0:
		nop();break;
	case	1:
		lxi(BC);break;
	case	2:
		stax(BC);break;
	case	3:
		inx(BC);break;
	case	4:
		inr(B);break;
	case	5:
		dcr(B);break;
	case	6:
		mvi(B);break;
	case	7:
		rlc();break;
	case	8:
		undef();break;
	case	9:
		dad(BC);break;
	case	0xa:
		ldax(BC);break;
	case	0xb:
		dcx(BC);break;
	case	0xc:
		inr(C);break;
	case	0xd:
		dcr(C);break;
	case	0xe:
		mvi(C);break;
	case	0xf:
		rrc();break;

	case	0x10:
		undef();break;
	case	0x11:
		lxi(DE);break;
	case	0x12:
		stax(DE);break;
	case	0x13:
		inx(DE);break;
	case	0x14:
		inr(D);break;
	case	0x15:
		dcr(D);break;
	case	0x16:
		mvi(D);break;
	case	0x17:
		ral();break;
	case	0x18:
		undef();break;
	case	0x19:
		dad(DE);break;
	case	0x1a:
		ldax(DE);break;
	case	0x1b:
		dcx(DE);break;
	case	0x1c:
		inr(E);break;
	case	0x1d:
		dcr(E);break;
	case	0x1e:
		mvi(E);break;
	case	0x1f:
		rar();break;

	case	0x20:
		undef();break;
	case	0x21:
		lxi(HL);break;
	case	0x22:
		shld();break;
	case	0x23:
		inx(HL);break;
	case	0x24:
		inr(H);break;
	case	0x25:
		dcr(H);break;
	case	0x26:
		mvi(H);break;
	case	0x27:
		daa();break;
	case	0x28:
		undef();break;
	case	0x29:
		dad(HL);break;
	case	0x2a:
		lhld();break;
	case	0x2b:
		dcx(HL);break;
	case	0x2c:
		inr(L);break;
	case	0x2d:
		dcr(L);break;
	case	0x2e:
		mvi(L);break;
	case	0x2f:
		cma();break;

	case	0x30:
		undef();break;
	case	0x31:
		lxisp();break;
	case	0x32:
		sta();break;
	case	0x33:
		inxsp();break;
	case	0x34:
		inrm();break;
	case	0x35:
		dcrm();break;
	case	0x36:
		mvim();break;
	case	0x37:
		stc();break;
	case	0x38:
		undef();break;
	case	0x39:
		dadsp();break;
	case	0x3a:
		lda();break;
	case	0x3b:
		dcxsp();break;
	case	0x3c:
		inr(A);break;
	case	0x3d:
		dcr(A);break;
	case	0x3e:
		mvi(A);break;
	case	0x3f:
		cmc();break;

	case	0x40:
		mov(B,B);break;
	case	0x41:
		mov(B,C);break;
	case	0x42:
		mov(B,D);break;
	case	0x43:
		mov(B,E);break;
	case	0x44:
		mov(B,H);break;
	case	0x45:
		mov(B,L);break;
	case	0x46:
		movrm(B);break;
	case	0x47:
		mov(B,A);break;
	case	0x48:
		mov(C,B);break;
	case	0x49:
		mov(C,C);break;
	case	0x4a:
		mov(C,D);break;
	case	0x4b:
		mov(C,E);break;
	case	0x4c:
		mov(C,H);break;
	case	0x4d:
		mov(C,L);break;
	case	0x4e:
		movrm(C);break;
	case	0x4f:
		mov(C,A);break;
	case	0x50:
		mov(D,B);break;
	case	0x51:
		mov(D,C);break;
	case	0x52:
		mov(D,D);break;
	case	0x53:
		mov(D,E);break;
	case	0x54:
		mov(D,H);break;
	case	0x55:
		mov(D,L);break;
	case	0x56:
		movrm(D);break;
	case	0x57:
		mov(D,A);break;
	case	0x58:
		mov(E,B);break;
	case	0x59:
		mov(E,C);break;
	case	0x5a:
		mov(E,D);break;
	case	0x5b:
		mov(E,E);break;
	case	0x5c:
		mov(E,H);break;
	case	0x5d:
		mov(E,L);break;
	case	0x5e:
		movrm(E);break;
	case	0x5f:
		mov(E,A);break;
	case	0x60:
		mov(H,B);break;
	case	0x61:
		mov(H,C);break;
	case	0x62:
		mov(H,D);break;
	case	0x63:
		mov(H,E);break;
	case	0x64:
		mov(H,H);break;
	case	0x65:
		mov(H,L);break;
	case	0x66:
		movrm(H);break;
	case	0x67:
		mov(H,A);break;
	case	0x68:
		mov(L,B);break;
	case	0x69:
		mov(L,C);break;
	case	0x6a:
		mov(L,D);break;
	case	0x6b:
		mov(L,E);break;
	case	0x6c:
		mov(L,H);break;
	case	0x6d:
		mov(L,L);break;
	case	0x6e:
		movrm(L);break;
	case	0x6f:
		mov(L,A);break;
	case	0x70:
		movmr(B);break;
	case	0x71:
		movmr(C);break;
	case	0x72:
		movmr(D);break;
	case	0x73:
		movmr(E);break;
	case	0x74:
		movmr(H);break;
	case	0x75:
		movmr(L);break;
	case	0x76:
		EMT(*pc++);break;
	/*	hlt();break;	*/
	case	0x77:
		movmr(A);break;
	case	0x78:
		mov(A,B);break;
	case	0x79:
		mov(A,C);break;
	case	0x7a:
		mov(A,D);break;
	case	0x7b:
		mov(A,E);break;
	case	0x7c:
		mov(A,H);break;
	case	0x7d:
		mov(A,L);break;
	case	0x7e:
		movrm(A);break;
	case	0x7f:
		mov(A,A);break;
	case	0x80:
		add(B);break;
	case	0x81:
		add(C);break;
	case	0x82:
		add(D);break;
	case	0x83:
		add(E);break;
	case	0x84:
		add(H);break;
	case	0x85:
		add(L);break;
	case	0x86:
		addm();break;
	case	0x87:
		add(A);break;
	case	0x88:
		adc(B);break;
	case	0x89:
		adc(C);break;
	case	0x8a:
		adc(D);break;
	case	0x8b:
		adc(E);break;
	case	0x8c:
		adc(H);break;
	case	0x8d:
		adc(L);break;
	case	0x8e:
		adcm();break;
	case	0x8f:
		adc(A);break;
	case	0x90:
		sub(B);break;
	case	0x91:
		sub(C);break;
	case	0x92:
		sub(D);break;
	case	0x93:
		sub(E);break;
	case	0x94:
		sub(H);break;
	case	0x95:
		sub(L);break;
	case	0x96:
		subm();break;
	case	0x97:
		sub(A);break;
	case	0x98:
		sbb(B);break;
	case	0x99:
		sbb(C);break;
	case	0x9a:
		sbb(D);break;
	case	0x9b:
		sbb(E);break;
	case	0x9c:
		sbb(H);break;
	case	0x9d:
		sbb(L);break;
	case	0x9e:
		sbbm();break;
	case	0x9f:
		sbb(A);break;
	case	0xa0:
		ana(B);break;
	case	0xa1:
		ana(C);break;
	case	0xa2:
		ana(D);break;
	case	0xa3:
		ana(E);break;
	case	0xa4:
		ana(H);break;
	case	0xa5:
		ana(L);break;
	case	0xa6:
		anam();break;
	case	0xa7:
		ana(A);break;
	case	0xa8:
		xra(B);break;
	case	0xa9:
		xra(C);break;
	case	0xaa:
		xra(D);break;
	case	0xab:
		xra(E);break;
	case	0xac:
		xra(H);break;
	case	0xad:
		xra(L);break;
	case	0xae:
		xram();break;
	case	0xaf:
		xra(A);break;
	case	0xb0:
		ora(B);break;
	case	0xb1:
		ora(C);break;
	case	0xb2:
		ora(D);break;
	case	0xb3:
		ora(E);break;
	case	0xb4:
		ora(H);break;
	case	0xb5:
		ora(L);break;
	case	0xb6:
		oram();break;
	case	0xb7:
		ora(A);break;
	case	0xb8:
		cmp(B);break;
	case	0xb9:
		cmp(C);break;
	case	0xba:
		cmp(D);break;
	case	0xbb:
		cmp(E);break;
	case	0xbc:
		cmp(H);break;
	case	0xbd:
		cmp(L);break;
	case	0xbe:
		cmpm();break;
	case	0xbf:
		cmp(A);break;

	case	0xc0:
		rnz();break;
	case	0xc1:
		pop(BC);break;
	case	0xc2:
		jnz();break;
	case	0xc3:
		jmp();break;
	case	0xc4:
		cnz();break;
	case	0xc5:
		push(BC);break;
	case	0xc6:
		adi();break;
	case	0xc7:
		rst(0);break;
	case	0xc8:
		rz();break;
	case	0xc9:
		ret();break;
	case	0xca:
		jz();break;
	case	0xcb:
		undef();break;
	case	0xcc:
		cz();break;
	case	0xcd:
		call();break;
	case	0xce:
		aci();break;
	case	0xcf:
		rst(1);break;

	case	0xd0:
		rnc();break;
	case	0xd1:
		pop(DE);break;
	case	0xd2:
		jnc();break;
	case	0xd3:
		out();break;
	case	0xd4:
		cnc();break;
	case	0xd5:
		push(DE);break;
	case	0xd6:
		sui();break;
	case	0xd7:
		rst(2);break;
	case	0xd8:
		rc();break;
	case	0xd9:
		undef();break;
	case	0xda:
		jc();break;
	case	0xdb:
		in();break;
	case	0xdc:
		cc();break;
	case	0xdd:
		undef();break;
	case	0xde:
		sbi();break;
	case	0xdf:
		rst(3);break;

	case	0xe0:
		rpo();break;
	case	0xe1:
		pop(HL);break;
	case	0xe2:
		jpo();break;
	case	0xe3:
		xthl();break;
	case	0xe4:
		cpo();break;
	case	0xe5:
		push(HL);break;
	case	0xe6:
		ani();break;
	case	0xe7:
		rst(4);break;
	case	0xe8:
		rpe();break;
	case	0xe9:
		pchl();break;
	case	0xea:
		jpe();break;
	case	0xeb:
		xchg();break;
	case	0xec:
		cpe();break;
	case	0xed:
		undef();break;
	case	0xee:
		xri();break;
	case	0xef:
		rst(5);break;

	case	0xf0:
		rp();break;
	case	0xf1:
		poppsw();break;
	case	0xf2:
		jp();break;
	case	0xf3:
		di();break;
	case	0xf4:
		cp();break;
	case	0xf5:
		pushpsw();break;
	case	0xf6:
		ori();break;
	case	0xf7:
		rst(6);break;
	case	0xf8:
		rm();break;
	case	0xf9:
		sphl();break;
	case	0xfa:
		jm();break;
	case	0xfb:
		ei();break;
	case	0xfc:
		cm();break;
	case	0xfd:
		undef();break;
	case	0xfe:
		cpi();break;
	case	0xff:
		rst(7);break;
   }
#ifdef	TRACE
   if(itrace || pc==break_point) brk(0);
#endif
 }
}

disp_80_version()
{
 printf("8080 %s",VERSION);
#ifdef PARITY
 printf(" (parity)");
#endif
#ifdef TRACE
 printf(" (tracer)");
#endif
}

