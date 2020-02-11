;
; index.a -- return index of char in string, or NIL...
;

#include <regs65.ah>

	.entry	_index
_index:
	ldy	#3
	jsr	setup
	iny
	sty	r1
	sty	r1h
$1:	lda	[0],y
	beq	$2
	cmp	2
	bne	$3
	clc
	tya
	adc	0
	sta	r1
	lda	1
	adc	#0
	sta	r1h
$2:	rts
$3:	iny
	bne	$1
	inc	1
	jmp	$1

