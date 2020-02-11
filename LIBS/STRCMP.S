;
; strcmp.a -- strcmp function
;

#include <regs65.ah>

	.entry	_strcmp
_strcmp:
	ldy	#3
	jsr	setup
	iny
	sty	r1
	sty	r1h
$1:	lda	[0],y
	cmp	[2],y
	bne	$3
	iny
	bne	$2
	inc	1
	inc	3
$2:	tax
	bne	$1
	rts
$3:	sec
	sbc	[2],y
	sta	r1
	bpl	$4
	dec	r1h
$4:	rts

