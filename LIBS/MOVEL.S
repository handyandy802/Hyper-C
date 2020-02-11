;
; movel.a -- move left function...
;
	.entry	_movel
_movel:
	ldy	#5
	jsr	setup
	iny
	clc
	lda	0
	adc	4
	sta	0
	lda	1
	adc	5
	sta	1
	clc
	lda	2
	adc	4
	sta	2
	lda	3
	adc	5
	sta	3
$1:	lda	4
	bne	$3
	lda	5
	bne	$2
	rts
$2:	dec	5
$3:	dec	4
	lda	0
	bne	*+4
	dec	1
	dec	0
	lda	2
	bne	*+4
	dec	3
	dec	2
	lda	[0],y
	sta	[2],y
	jmp	$1

