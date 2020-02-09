;
; streq.a -- streq function
;

#include <regs65.ah>

	.entry	_streq
_streq:
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
	inc	r1
$3:	rts

