;
; rindex.a -- return highest address of char in string, or NIL...
;

#include <regs65.ah>

	.entry	_rindex
_rindex:
	ldy	#3
	jsr	setup
	iny
	sty	r1
	sty	r1h
$1:	lda	[0],y
	beq	$3
	cmp	2
	bne	$2
	clc
	tya
	adc	0
	sta	r1
	lda	1
	adc	#0
	sta	r1h
$2:	iny
	bne	$1
	inc	1
	jmp	$1
$3:	rts

