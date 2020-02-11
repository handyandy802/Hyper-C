;
; setjmp.a -- setjmp and longjmp utilities
;

#include <regs65.ah>

	.entry	_longjmp
_longjmp:
	ldy	#3
	jsr	setup
	lda	2
	sta	r1
	lda	3
	sta	r1h
	ldy	#0
	ldx	#32
$1:	lda	[0],y		;restore the display...
	sta	dsply,y
	iny
	dex
	bne	$1
	ldx	#6
$2:	lda	[0],y		;restore pc, fp, and sp
	sta	pc-32,y
	iny
	dex
	bne	$2
	lda	[0],y		;restore the system stack ptr
	tax
	txs
	iny
	iny
	lda	[0],y		;restore the return addr
	sta	0x101,x
	iny
	lda	[0],y
	sta	0x102,x
	rts

	.entry	_setjmp
_setjmp:
	ldy	#1
	jsr	setup
	iny
	sty	r1		;return zero...
	sty	r1h
	ldx	#32
$1:	lda	dsply,y	;save the display...
	sta	[0],y
	iny
	dex
	bne	$1
	ldx	#6
$2:	lda	pc-32,y		;save pc, fp, and sp
	sta	[0],y
	iny
	dex
	bne	$2
	tsx			;save the system stack pointer
	txa
	sta	[0],y
	iny
	iny
	lda	0x101,x		;save the return addr
	sta	[0],y
	iny
	lda	0x102,x
	sta	[0],y
	rts
;
; -- end of setjmp/longjmp ---
;

