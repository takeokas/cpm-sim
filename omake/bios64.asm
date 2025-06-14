;
;	bios for 64K CP/M
;	for CP/SIM
;	08/Jan/1992
;
;
NULL		equ	0
bios64		equ	0fa00h
dph$base	equ	0fb00h	;This Address sensitive for "cpmbios.c"
dpb$base	equ	0fe00h
;
;
;	Emulate Machine Trap
emt	macro	num
	hlt
	dw	num
	endm
;
	org	bios64
;
	jmp	cboot
	jmp	wboot
	jmp	const
	jmp	conin
	jmp	conout
	jmp	list
	jmp	punch
	jmp	reader
	jmp	home
	jmp	seldsk
	jmp	settrk
	jmp	setsec
	jmp	setdma
	jmp	read
	jmp	write
	jmp	listst
	jmp	sectran
;
cboot:	emt	0		;will be breaked
;
;
wboot:	emt	1
	jmp	0e400h
;
const:
	emt	2
	ret
;
conin:
	emt	3
	ret
;
conout:
	emt	4
	ret
;
list:
	emt	5
	ret
;
punch:
	emt	6
	ret
;
reader:
	emt	7
	ret
;
home:
	emt	8
	ret
;
seldsk:
	lxi	h,dph$base
	emt	9
	ret
;
settrk:
	emt	0ah
	ret
;
setsec:
	emt	0bh
	ret
;
setdma:
	emt	0ch
	ret
;
read:
	emt	0dh
	ret
;
write:
	emt	0eh
	ret
;
listst:
	emt	0fh
	ret
;
sectran:
	emt	10h
	ret
;
;	dirBuf
;
dirbuf:	ds	128
;
	org	dph$base		;This Address sensitive for "cpmbios.c"
;
;	DPH
;
csv$alv	macro	no
	dw	csv&no,	alv&no	;csv,alv
	endm
;
dno	set	0
	rept	16
	dw	NULL,	0000h	;xlt, 0
	dw	0000h,	0000h	;0, 0
	dw	dirbuf,	dpb$8M	;dirbuf,dpb=8M
	csv$alv	%dno
dno	set	dno+1
	endm
;
;
;	CS(check Scratch) AL(alloc area)
;
cs$al	macro	ano
csv&ano:	ds	1	;(16)
alv&ano:	ds	31
	endm
;
dno	set	0
	rept	16
	cs$al	%dno
dno	set	dno+1
	endm
;
;
	org	dpb$base
;
;	DPB
;
dpb$8M:			;;; 8M
	dw	100*8	;spt	100KB/track
	db	7	;bsh	16384 Bytes/Block
	db	127	;blm
	db	7	;exm
	dw	511	;dsm
	dw	511	;drm
	db	80h	;al0
	db	0	;al1
	dw	1	;cks
	dw	1	;off
;
	end

