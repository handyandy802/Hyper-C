;
; strlen.a -- strlen function...
;

#include <regs65.ah>

    .entry  _strlen         ;   strlen(char * str)
_strlen:
	ldy	#1
	jsr	setup
	iny
	sty	r1h
$1:	lda	[0],y
	beq	$2
	iny
	bne	$1
	inc	r1h
	bne	$1
$2:	sty	r1
	rts

