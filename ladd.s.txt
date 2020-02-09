;
; _ladd -- long integer add...
;
;	void _ladd(lint1, lint2)
;	int *lint1;
;	int *lint2;
;
;	performs:
;		lint1 + lint2 -> lint1
;
;	long integers are stored as an array of two 16-bit integers
;	with the hi-word first.  (Remember that bytes within a word
;	are stored lo-byte first).
;

#include <regs65.ah>

	.entry	__ladd
__ladd:
	ldy	#3
	jsr	setup
	clc
	ldy	#2
	lda	[0],y
	adc	[2],y
	sta	[0],y
	iny
	lda	[0],y
	adc	[2],y
	sta	[0],y
	ldy	#0
	lda	[0],y
	adc	[2],y
	sta	[0],y
	iny
	lda	[0],y
	adc	[2],y
	sta	[0],y
	rts
;
; -- end of _ladd
;

