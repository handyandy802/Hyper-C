;
; mover.a -- mover function...
;
    .entry  _mover          ;   mover(src,dest,len)
_mover:
	ldy	#5
	jsr	setup
	iny
$1:	cpy	4
	bne	$3
	lda	5
	bne	$2
	rts
$2:	dec	5
$3:	lda	[0],y
	sta	[2],y
	iny
	bne	$1
	inc	1
	inc	3
	jmp	$1

