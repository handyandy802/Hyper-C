;
; clrbuf.a -- clrbuf function...
;
    .entry  _clrbuf         ;   clrbuf(buf,len)
_clrbuf:
	ldy	#3
	jsr	setup
	iny
	tya
$1:	cpy	2
	bne	$3
	ldx	3
	bne	$2
	rts
$2:	dec	3
$3:	sta	[0],y
	iny
	bne	$1
	inc	1
	jmp	$1

