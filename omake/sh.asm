;
;	system
;

unx$sys	equ	0084h
;
	org	100h
;
	lxi	h,80h

	mov	a,m	; length
	ora	a
	jnz	l1
	lxi	d,sh
	jmp	system

l1:	mov	c,a
	inx	h

	mov	e,l	; de := pointer
	mov	d,h

	mvi	b,0
	dad	b
	mvi	m,0	;put NULL at the end of str.
;
;	tolower string
	mov	l,e
	mov	h,d
tl2:
	mov	a,m
	ora	a
	jz	tl3
	cpi	'A'
	jc	tl1
	cpi	'Z'+1
	jnc	tl1
	adi	'a'-'A'
	mov	m,a
tl1:
	inx	h
	jmp	tl2
tl3:
;
system:	hlt
	dw	unx$sys
	jmp	0

sh:	db	'exec sh',0

	end
