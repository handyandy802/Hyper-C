;
; vidio.a -- get key/ put chars to screen...
;

#include <regs65.ah>

csw	= 0x36
ksw	= 0x38
kbd	= 0xc000
kbdstrb	= 0xc010
	
;
; vidinit -- initialize screen i/o card and indirect vectors...
;

	.entry	__vidinit
__vidinit:
	jsr	__vidonly
	lda	kbdstrb
	lda	0xcfff
	lda	0xc30d
	sta	$1+1
$1:	jsr	0xc300
	lda	0xc30e
	sta	cinjsr+1
	lda	0xc30f
	sta	coutjsr+1
	rts
	
	.entry	__prtinit
__prtinit:
	ldy	#0
	lda	[sp],y
	and	#7
	ora	#0xc0
prtinit:
	sta	csw+1
	ldx	#0
	stx	csw
	stx	ksw+1
	cmp	#0xc1		;is it slot #1?
	bne	basic		;no -- can't use Pascal protocol
	ldy	#0x0b		;chk generic signiture byte
	lda	[csw],y
	cmp	#1		;is it Pascal protocol?
	bne	basic		;no -- do things like BASIC
	iny
	lda	[csw],y		;get ID byte
	and	#0xf0		;get hi nibble
	cmp	#0x10		;is it a Pascal card?
	bne	basic		;no!
pascal:
	iny
	lda	[csw],y		;get Pascal init vector
	sta	$3+1
	iny
	iny
	lda	[csw],y		;get Pascal output vector
	sta	pasout+1
	iny
	lda	[csw],y		;get Pascal I/O check vector
	sta	pasiock+1
	jsr	regset		;setup regs for Pascal protocol
$3:	jsr	0xc100		;perform Pascal init
	ldy	#pasEnd-pasStart
$4:	lda	pasStart,y	;now transfer driver to patch area
	sta	__putprt,y
	dey
	bpl	$4
	rts
	
basic:
	lda	csw+1
	sta	$1+2
$1:	jsr	0xc100		;perform "PR#x" for slot x
	ldy	#patchEnd-patchStart
$2:	lda	patchStart,y
	sta	__putprt,y
	dey
	bpl	$2
	rts
	
regset:
	bit	0xcfff		;switch out expansion ROM at 0xc800
	ldx	#0xc1
	ldy	#0x10
	rts
	
patchStart:
	ldx	0xc082		;switch ROM back in...
	ldx	#0x4c		;make csw into a jmp
	stx	csw-1
	jsr	csw-1		;call the output routine
	ldx	0xc083		;re-enable RAM...
	ldx	0xc083
	rts
patchEnd:

pasStart:
	pha			;save output char
patch:
	jsr	regset
	lda	#0		;request status
pasiock:
	jsr	0xc100
	bcc	patch		;device not ready
	jsr	regset
	pla			;get char
pasout:
	jmp	0xc100		;send it now!
pasEnd:

;
; getkey -- get a keyboard character...
; Parameter specifies whether pending keys should be discarded or not...
;

	.entry	_getkey
_getkey:
	ldy	#0
	sty	r1h
	lda	[sp],y
	beq	$1
	bit	kbdstrb
$1:	lda	0xcfff
cinjsr:
	jsr	0xc300
	sta	r1
	rts

	.entry	_clrkbd
_clrkbd:
	bit	kbdstrb
	rts
	
	.entry	_keypress
_keypress:
	lda	kbd
	and	#0x80
	sta	r1
	lda	#0
	sta	r1h
	rts
;
; putstr -- output a null terminated string to the screen...
;
	.entry	_putstr
_putstr:
	ldy	#1
	jsr	setup
$1:	ldy	#0
	lda	[0],y
	beq	$2
	jsr	putchr
	inc	0
	bne	$1
	inc	1
	jmp	$1
$2:	rts
	
;
; putchr -- put a character to the screen...
;

	.entry	_putchr
_putchr:
	ldy	#0
	lda	[sp],y
putchr:
	jmp	[__prtHook]

	.entry	__vidonly
__vidonly:
	lda	#<putvid
	ldy	#>putvid
setHook:
	sta	__prtHook
	sty	__prtHook+1
	rts
	
	.entry	__prtonly
__prtonly:
	lda	#<putprt
	ldy	#>putprt
	bne	setHook
	
	.entry	__prtvid
__prtvid:
	lda	#<putboth
	ldy	#>putboth
	bne	setHook

putboth:
	pha
	jsr	putprt
	pla

putvid:
	cmp	#0x09
	bne	$2
	lda	__xcurs
	and	#3
	sbc	#4
	sta	2
$1:	lda	#0x20
	jsr	$3
	inc	2
	bne	$1
	rts
$2:	cmp	#0x0d
	bne	$3
	jsr	__chkAbort
	lda	#0x0d
	jsr	$3
	lda	#0x0a
$3:	bit	0xcfff
coutjsr:
	jmp	0xc300

	.entry	__chkAbort
__chkAbort:
	lda	kbd
	cmp	#0x93	;ctl-s
	bne	$2
	bit	kbdstrb
$1:	lda	kbd
	bpl	$1
	bit	kbdstrb
$2:	cmp	#0x80	;ctl-@
	beq	abort
	rts
abort:	bit	kbdstrb
	jsr	__usrAbort

;
; putprt -- put a character to the printer...
;

	.entry	_putprt
_putprt:
	ldy	#0
	lda	[sp],y
putprt:
	cmp	#0x0d
	bne	$1
	jsr	__chkAbort
	lda	#0
	sta	__prtpos
	lda	#0x0d
	jsr	$2
	lda	__pcrlf
	bne	$11
	rts
$11:
	lda	#0x0a
	bne	$2
$1:	cmp	#0x20
	bcc	$2
	inc	__prtpos
$2:	jmp	__putprt

;
; getcurs -- return screen cursor position ((y * 256) + x)
;
	.entry	_getcurs
_getcurs:
	lda	__ycurs
	sta	r1h
	lda	__xcurs
	sta	r1
	rts
	
;
; setcurs -- set screen cursor position ((y * 256) + x)
;
	.entry	_setcurs
_setcurs:
	lda	#0x1e
	jsr	putchr
	ldy	#0
	jsr	$1
	ldy	#1
$1:	lda	[sp],y
	clc
	adc	#0x20
	jmp	putchr
	
; --- end of vidio.a ---

