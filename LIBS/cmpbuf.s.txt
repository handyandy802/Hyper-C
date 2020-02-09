;
; cmpbuf.a -- compare two buffers for a specified length
;
;	int cmpbuf(buf1, buf2, len);
;
;	result is zero if they are equal...
;	else the result is the difference in the sense buf1 - buf2.
;

#include <regs65.ah>

	.entry	_cmpbuf
_cmpbuf:
	ldy	#5
	jsr	setup
	iny
	sty	r1
	sty	r1h
$1:	cpy	4
	bne	$3
	lda	5
	bne	$2
	rts
$2:	dec	5
$3:	lda	[0],y
	cmp	[2],y
	bne	$4
	iny
	bne	$1
	inc	1
	inc	3
	jmp	$1
$4:	sec
	sbc	[2],y
	sta	r1
	bcs	$5
	dec	r1h
$5:	rts

