;
; strncmp.a -- strncmp function
;

#include <regs65.ah>

	.entry	_strncmp
_strncmp:
	ldy	#5
	jsr	setup
	iny
	sty	r1
	sty	r1h
$1:	cpy	4
	bne	$4
	lda	5
	bne	$3
$2:	rts
$3:	dec	5
$4:	lda	[0],y
	cmp	[2],y
	bne	$5
	tax
	beq	$2
	iny
	bne	$1
	inc	1
	inc	3
	jmp	$1
$5:	sec
	sbc	[2],y
	sta	r1
	bcs	$6
	dec	r1h
$6:	rts

