;
; strneq.a -- strneq function
;

#include <regs65.ah>

	.entry	_strneq
_strneq:
	ldy	#5
	jsr	setup
	iny
	sty	r1
	sty	r1h
$1:	cpy	4
	bne	$5
	lda	5
	bne	$4
$2:	inc	r1
$3:	rts
$4:	dec	5
$5:	lda	[0],y
	cmp	[2],y
	bne	$3
	tax
	beq	$2
	iny
	bne	$1
	inc	1
	inc	3
	jmp	$1

