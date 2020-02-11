;
; setup.a -- utility routine for library functions...
;
; On entry:
;	y = nbr bytes - 1 to move from stack to page zero starting at 0.
;	Moves parameter bytes to page zero working space.

#include <regs65.ah>

	.entry	setup
setup:
	lda	[sp],y
	sta	0,y
	dey
	bpl	setup
	rts

