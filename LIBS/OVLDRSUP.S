;
; ovldrsup.a -- support code for system level access in ovldr.cc...
;

#include <regs65.ah>

	.entry	__OVLDR
__OVLDR:	;overlay loader entry point...
	sec			;make room for seginfo ptr
	lda	sp
	sbc	#2
	sta	sp
	bcs	*+4
	dec	sph
	ldy	#0		;push seginfo ptr
	sec
	pla			;adjust return addr to seginfo block
	sbc	#2
	sta	[sp],y
	iny
	pla
	sbc	#0
	sta	[sp],y
	jsr	__OVLDRX	;call guts of ovldr -- returns addr in r1
	clc			;drop seginfo ptr
	lda	sp
	adc	#2
	sta	sp
	bcc	*+4
	inc	sph
	jmp	[r1]		;now call the routine...
;
; --- end of ovldrsup.a ---
;

