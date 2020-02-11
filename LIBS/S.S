;
; s.s -- startup code for HyperC ProDOS programs...
;

#include <regs65.ah>

	pla
	pla
	jsr	_main
	lda	#0
	tay
	sta	[sp],y
	iny
	sta	[sp],y
	jsr	_exit
	
;
; --- end of s.s
;

