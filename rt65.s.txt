;
;	rt65.a -- 6502 p-Code runtime support...
;

#include <regs65.ah>

	.entry	Alnkv
Alnkv:	sta	0	;save local size
	sty	1
	stx	2

	ldy	2
	lda	dsply,y
	ldx	dsply+1,y
	jsr	Apshxa
	
	lda	fp
	ldx	fph
	jsr	Apshxa
	
	ldx	2
	lda	sp
	sta	fp
	sta	dsply,x
	lda	sph
	sta	fph
	sta	dsply+1,x

	sec
	lda	sp
	sbc	0
	sta	sp
	lda	sph
	sbc	1
	sta	sph
	
	pla
	clc
	adc	#1
	sta	0
	pla
	adc	#0
	sta	1
	
	sec
	lda	sp
	sbc	#2
	sta	sp
	bcs	*+4
	dec	sph
	
	ldy	#0
	pla
	sta	[sp],y
	pla
	iny
	sta	[sp],y
	
	jmp	[0]
	
	.entry	Artnv
Artnv:	ldy	#1
	lda	[sp],y
	pha
	dey
	lda	[sp],y
	pha
	
	stx	0
	lda	fp
	sta	sp
	lda	fph
	sta	sph
	ldx	#fp
	jsr	Apopx
	clc
	lda	#dsply
	adc	0
	tax
	bne	Apopx

	.entry	Alnk
Alnk:	sta	0	;save local size
	sty	1

	lda	fp
	ldx	fph
	jsr	Apshxa
	
	lda	sp
	sta	fp
	lda	sph
	sta	fph

	sec
	lda	sp
	sbc	0
	sta	sp
	lda	sph
	sbc	1
	sta	sph
	
	pla
	clc
	adc	#1
	sta	0
	pla
	adc	#0
	sta	1
	
	sec
	lda	sp
	sbc	#2
	sta	sp
	bcs	*+4
	dec	sph
	
	ldy	#0
	pla
	sta	[sp],y
	pla
	iny
	sta	[sp],y
	
	jmp	[0]
	
	.entry	Artn
Artn:	ldy	#1
	lda	[sp],y
	pha
	dey
	lda	[sp],y
	pha
	
	lda	fp
	sta	sp
	lda	fph
	sta	sph
	ldx	#fp		;fall into Apopx

	.entry	Apopx
Apopx:	ldy	#0
	lda	[sp],y
	sta	0,x
	iny
	lda	[sp],y
	sta	1,x
	clc
	lda	sp
	adc	#2
	sta	sp
	bcc	*+4
	inc	sph
	rts
	
	.entry	Apshxa
Apshxa:	tay
	sec
	lda	sp
	sbc	#2
	sta	sp
	bcs	*+4
	dec	sph
	tya
	ldy	#0
	sta	[sp],y
	txa
	iny
	sta	[sp],y
	rts

	.entry	Alla
Alla:	ldy	#0

	.entry	Allan
Allan:	clc
	adc	fp
	sta	r1
	tya
	adc	fph
	sta	r1h
	rts
	
	.entry	Alva
Alva:	ldy	#0

	.entry	Alvan
Alvan:	clc
	adc	dsply,x
	sta	r1
	tya
	adc	dsply+1,x
	sta	r1h
	rts
	
	.entry	Apla
Apla:	ldy	#0

	.entry	Aplan
Aplan:	clc
	adc	fp
	pha
	tya
	adc	fph
	tax
	pla
	jmp	Apshxa

	.entry	Apva
Apva:	ldy	#0

	.entry	Apvan
Apvan:	clc
	adc	dsply,x
	pha
	tya
	adc	dsply+1,x
	tax
	pla
	jmp	Apshxa

	.entry	Atab
Atab:	lda	r1
	sta	r2
	lda	r1h
	sta	r2h
	rts
	
Axtojp:	clc
	adc	0,x
	sta	jp
	tya
	adc	1,x
	sta	jph
	rts
	
Adsptojp:
	clc
	adc	dsply,x
	sta	jp
	tya
	adc	dsply+1,x
	sta	jph
	rts

	.entry	Alur1
Alur1:	ldx	#0
	lda	[r1],y
	sta	r1
	stx	r1h
	rts
	
	.entry	Alwr1
Alwr1:	lda	[r1],y
	tax
	iny
	lda	[r1],y
	sta	r1h
	stx	r1
	rts
	
	.entry	Apur1
Apur1:	ldx	#0
	lda	[r1],y
	jmp	Apshxa
	
	.entry	Apwr1
Apwr1:	lda	[r1],y
	tax
	dey
	lda	[r1],y
	jmp	Apshxa
	
	.entry	Asbr2
Asbr2:	lda	r1
	sta	[r2],y
	rts
	
	.entry	Asbbr2
Asbbr2:
	lda	[r2],y
	and	smask
	ora	r3
	sta	[r2],y
	rts
	
	.entry	Aswr2
Aswr2:	lda	r1
	sta	[r2],y
	iny
	lda	r1h
	sta	[r2],y
	rts
	
	.entry	Aswbr2
Aswbr2:
	lda	[r2],y
	and	smask
	ora	r3
	sta	[r2],y
	iny
	lda	[r2],y
	and	smaskh
	ora	r3h
	sta	[r2],y
	rts
	
	.entry	Alufp
Alufp:	ldx	#0
	lda	[fp],y
	sta	r1
	stx	r1h
	rts
	
	.entry	Alwfp
Alwfp:	lda	[fp],y
	sta	r1
	iny
	lda	[fp],y
	sta	r1h
	rts
	
	.entry	Apufp
Apufp:	ldx	#0
	lda	[fp],y
	jmp	Apshxa
	
	.entry	Apwfp
Apwfp:	lda	[fp],y
	tax
	dey
	lda	[fp],y
	jmp	Apshxa
	
	.entry	Asbfp
Asbfp:	lda	r1
	sta	[fp],y
	rts
	
	.entry	Asbbfp
Asbbfp:
	lda	[fp],y
	and	smask
	ora	r3
	sta	[fp],y
	rts
	
	.entry	Aswfp
Aswfp:	lda	r1
	sta	[fp],y
	iny
	lda	r1h
	sta	[fp],y
	rts
	
	.entry	Aswbfp
Aswbfp:
	lda	[fp],y
	and	smask
	ora	r3
	sta	[fp],y
	iny
	lda	[fp],y
	and	smaskh
	ora	r3h
	sta	[fp],y
	rts
	
	.entry	Aluvp,Aluvpn
Aluvp:	ldy	#0
Aluvpn:	jsr	Adsptojp
	jmp	Alujp
	
	.entry	Alur1n
Alur1n:	ldx	#r1
	bne	Alujp0
	
	.entry	Alufpn
Alufpn:	ldx	#fp	;fall into Alujp
Alujp0:	jsr	Axtojp

	.entry	Alujp
Alujp:	ldx	#0
	lda	[jp,x]
	sta	r1
	stx	r1h
	rts
	
	.entry	Alwvp,Alwvpn
Alwvp:	ldy	#0
Alwvpn:	jsr	Adsptojp
	jmp	Alwjp
	
	.entry	Alwr1n
Alwr1n:	ldx	#r1
	bne	Alwjp0
	
	.entry	Alwfpn
Alwfpn:	ldx	#fp	;fall into Alwjp
Alwjp0:	jsr	Axtojp

	.entry	Alwjp
Alwjp:	ldy	#0
	lda	[jp],y
	sta	r1
	iny
	lda	[jp],y
	sta	r1h
	rts
	
	.entry	Apuvp,Apuvpn
Apuvp:	ldy	#0
Apuvpn:	jsr	Adsptojp
	jmp	Apujp
	
	.entry	Apur1n
Apur1n:	ldx	#r1
	bne	Apujp0
	
	.entry	Apufpn
Apufpn:	ldx	#fp	;fall into Apujp
Apujp0:	jsr	Axtojp

	.entry	Apujp
Apujp:	ldx	#0
	lda	[jp,x]
	jmp	Apshxa
	
	.entry	Apwvp,Apwvpn
Apwvp:	ldy	#0
Apwvpn:	jsr	Adsptojp
	jmp	Apwjp
	
	.entry	Apwr1n
Apwr1n:	ldx	#r1
	bne	Apwjp0
	
	.entry	Apwfpn
Apwfpn:	ldx	#fp	;fall into Apwjp
Apwjp0:	jsr	Axtojp

	.entry	Apwjp
Apwjp:	ldy	#1
	lda	[jp],y
	tax
	dey
	lda	[jp],y
	jmp	Apshxa
	
	.entry	Asbvp,Asbvpn
Asbvp:	ldy	#0
Asbvpn:	jsr	Adsptojp
	jmp	Asbjp
	
	.entry	Asbr2n
Asbr2n:	ldx	#r2
	bne	Asbjp0
	
	.entry	Asbfpn
Asbfpn:	ldx	#fp	;fall into Asbjp
Asbjp0:	jsr	Axtojp
	
	.entry	Asbjp
Asbjp:	ldy	#0
	lda	r1
	sta	[jp],y
	rts
	
	.entry	Aswvp,Aswvpn
Aswvp:	ldy	#0
Aswvpn:	jsr	Adsptojp
	jmp	Aswjp
	
	.entry	Aswr2n
Aswr2n:	ldx	#r2
	bne	Aswjp0
	
	.entry	Aswfpn
Aswfpn:	ldx	#fp	;fall into Aswjp
Aswjp0:	jsr	Axtojp
	
	.entry	Aswjp
Aswjp:	ldy	#0
	lda	r1
	sta	[jp],y
	iny
	lda	r1h
	sta	[jp],y
	rts
	
	.entry	Asbbvp,Asbbvpn
Asbbvp:	ldy	#0
Asbbvpn:
	jsr	Adsptojp
	jmp	Asbbjp
	
	.entry	Asbbr2n
Asbbr2n:
	ldx	#r2
	bne	Asbbjp0
	
	.entry	Asbbfpn
Asbbfpn:
	ldx	#fp	;fall into Asbbfpn
Asbbjp0:
	jsr	Axtojp
	
	.entry	Asbbjp
Asbbjp:	ldy	#0
	lda	[jp],y
	and	smask
	ora	r3
	sta	[jp],y
	rts
	
	.entry	Aswbvp,Aswbvpn
Aswbvp:	ldy	#0
Aswbvpn:
	jsr	Adsptojp
	jmp	Aswbjp
	
	.entry	Aswbr2n
Aswbr2n:
	ldx	#r2
	bne	Aswbjp0
	
	.entry	Aswbfpn
Aswbfpn:
	ldx	#fp	;fall into Aswbfpn
Aswbjp0:
	jsr	Axtojp
	
	.entry	Aswbjp
Aswbjp:	ldy	#0
	lda	[jp],y
	and	smask
	ora	r3
	sta	[jp],y
	iny
	lda	[jp],y
	and	smaskh
	ora	r3h
	sta	[jp],y
	rts
	
	.entry	Ainc1
Ainc1:	inc	r1
	bne	*+4
	inc	r1h
	rts
	
	.entry	Ainc
Ainc:	clc
	adc	r1
	sta	r1
	bcc	*+4
	inc	r1h
	rts
	
	.entry	Aincn
Aincn:	clc
	adc	r1
	sta	r1
	tya
	adc	r1h
	sta	r1h
	rts
	
	.entry	Adec1
Adec1:	lda	r1
	bne	*+4
	dec	r1h
	dec	r1
	rts
	
	.entry	Aisp
Aisp:	clc
	adc	sp
	sta	sp
	bcc	*+4
	inc	sph
	rts
	
	.entry	Aneg
Aneg:	sec
	lda	#0
	sbc	r1
	sta	r1
	lda	#0
	sbc	r1h
	sta	r1h
	rts
	
	.entry	Anot
Anot:	lda	r1
	eor	#0xff
	sta	r1
	lda	r1h
	eor	#0xff
	sta	r1h
	rts
	
	.entry	Aadd
Aadd:	clc
	lda	r1
	adc	r2
	sta	r1
	lda	r1h
	adc	r2h
	sta	r1h
	rts
	
	.entry	Asub
Asub:	sec
	lda	r1
	sbc	r2
	sta	r1
	lda	r1h
	sbc	r2h
	sta	r1h
	rts
	
	.entry	Aand
Aand:	lda	r1
	and	r2
	sta	r1
	lda	r1h
	and	r2h
	sta	r1h
	rts
	
	.entry	Aor
Aor:	lda	r1
	ora	r2
	sta	r1
	lda	r1h
	ora	r2h
	sta	r1h
	rts
	
	.entry	Axor
Axor:	lda	r1
	eor	r2
	sta	r1
	lda	r1h
	eor	r2h
	sta	r1h
	rts

	.entry	Amul
Amul:	lda	r2
	ldy	r2h	;fall thru to Ampi
	
	.entry	Ampi
Ampi:	sta	r3
	sty	r3h
	lda	r1
	sta	r4
	lda	r1h
	sta	r4h
	lda	#0
	sta	r1
	sta	r1h
	ldy	#16
$1:	asl	r1
	rol	r1h
	rol	r3
	rol	r3h
	bcc	$2
	clc
	lda	r1
	adc	r4
	sta	r1
	lda	r1h
	adc	r4h
	sta	r1h
$2:	dey
	bne	$1
	rts
	
	.entry	Ain
Ain:	lda	r1		;swap r1 and r2
	ldx	r2		;to get base addr in r2
	sta	r2		;and bit number in r1
	stx	r1
	lda	r1h
	ldx	r2h
	sta	r2h
	stx	r1h
	jsr	_getBits	;get the offset and bit
	and	[r2],y		;test membership (y = 0)
	sty	r1h
	beq	*+3
	iny
	sty	r1
	rts
	
	.entry	Aincl
Aincl:	jsr	_getBits
	ora	[r2],y		;or in the set element	(y = 0)
	sta	[r2],y
	rts

	.entry	Aexcl
Aexcl:	jsr	_getBits
	eor	#0xff		;complement mask
	and	[r2],y		;turn off set element	(y = 0)
	sta	[r2],y
	rts
		
_getBits:
	lda	#0
	tay			;clear y
	ldx	#3		;shift count
$1:	lsr	r2h		;divide bit nbr by 8
	ror	r2
	rol	a		;remember remainder (bit reversed)
	dex
	bne	$1
	tax			;save bit reversed remainder
	lda	r2		;form byte addr in r2
	adc	r1
	sta	r2
	lda	r2h
	adc	r1h
	sta	r2h
	lda	_bitpat,x	;get the bit element
	rts

_bitpat:
	.byte	0x01,0x10,0x04,0x40	;remember that the offset is
	.byte	0x02,0x20,0x08,0x80	;the bit reversed value...

	.entry	Adivu
Adivu:	lda	r2
	ldy	r2h	;fall into Advi
	
	.entry	Advi
Advi:	sta	r3	;save copy of divisor in r3
	sty	r3h
_doDiv:
	lda	#0
	sta	r4		;quotient in r1, remainder in r4
	sta	r4h		;clear initial remainder
	ldy	#16		;shif count
$1:	lda	r1
	asl	a		;shift dividend and remainder left
	rol	r1h
	rol	r4
	rol	r4h
	sec			;trial subtraction of divisor
	lda	r4
	sbc	r3
	tax
	lda	r4h
	sbc	r3h
	bcc	$2		;branch if borrow
	sta	r4h		;else save new remainder
	stx	r4
$2:	rol	r1		;shift result into quotient
	dey			;repeat till done
	bne	$1
	rts
	
	.entry	Adiv
Adiv:	jsr	_getSign
	jsr	_doDiv
	lsr	smask
	bcc	$1		;test result sign
	jmp	Aneg
$1:	rts
	
	
	.entry	Amodu
Amodu:	jsr	Adivu
_modu1:
	lda	r4		;move remainder to r1
	sta	r1
	lda	r4h
	sta	r1h
	rts
	
	.entry	Amod
Amod:	jsr	_getSign
	jsr	_doDiv
	bit	smask		;test result sign
	bpl	_modu1		;positive -- move remainder to r1
	tya			;clear a (y = 0)
	sec
	sbc	r4		;negate result while storing in r1
	sta	r1
	tya
	sbc	r4h
	sta	r1h
	rts
	
;
; ensure divisor and dividend are positive...
; remember dividend and result sign...
; sign code stored in smask:
;	smask < 0 -- dividend was < 0
;	smask odd -- result is < 0
;
_getSign:
	lda	#0
	sta	smask		;clear result sign code
	lda	r1h		;test dividend sign
	bpl	$1
	lda	#0x81		;save sign code
	sta	smask
	jsr	Aneg
$1:	lda	r2h		;test sign of divisor
	bpl	$2
	inc	smask		;remember result sign (odd = negative)
	sec			;negate divisor so it's positive
	lda	#0
	sbc	r2
	sta	r3
	lda	#0
	sbc	r2h
	sta	r3h
	rts
$2:	sta	r3h
	lda	r2
	sta	r3
	rts
	
	.entry	Ashl
Ashl:	ldy	r2
	beq	$2
$1:	asl	r1
	rol	r1h
	dey
	bne	$1
$2:	rts
	
	.entry	Ashr
Ashr:	ldy	r2
	beq	$2
$1:	lda	r1h
	asl	a
	ror	r1h
	ror	r1
	dey
	bne	$1
$2:	rts
	
	.entry	Ashru
Ashru:	ldy	r2
	beq	$2
$1:	lsr	r1h
	ror	r1
	dey
	bne	$1
$2:	rts
	
	.entry	Acmp
Acmp:	sec
	lda	r1
	sbc	r2
	sta	r2
	lda	r1h
	sbc	r2h
	rts
	
	.entry	Aeq
Aeq:	jsr	Acmp
	ora	r2
	bne	_set0
	beq	_set1
	
	.entry	Aeqz
Aeqz:	lda	r1
	ora	r1h
	bne	_set0
	beq	_set1

	.entry	Ane
Ane:	jsr	Acmp
	ora	r2
	beq	_set0
	bne	_set1
	
	.entry	Anez
Anez:	lda	r1
	ora	r1h
	bne	_set1
	beq	_set0
	
	.entry	Alt
Alt:	jsr	Acmp
	bmi	_set1
	bpl	_set0
	
	.entry	Altz
Altz:	lda	r1h
	bmi	_set1
	bpl	_set0
	
	.entry	Ale
Ale:	jsr	Acmp
	bmi	_set1
	ora	r2
	beq	_set1
	bne	_set0
	
	.entry	Alez
Alez:	lda	r1h
	bmi	_set1
	ora	r1
	beq	_set1
	bne	_set0
	
	.entry	Age
Age:	jsr	Acmp
	bmi	_set0
	bpl	_set1
	
	.entry	Agez
Agez:	lda	r1h
	bmi	_set0
	bpl	_set1

_set0:
	lda	#0
	sta	r1
	sta	r1h
	rts
_set1:
	lda	#0
	sta	r1h
	lda	#1
	sta	r1
	rts
	
	.entry	Agt
Agt:	jsr	Acmp
	bmi	_set0
	ora	r2
	beq	_set0
	bne	_set1

	.entry	Agtz
Agtz:	lda	r1h
	bmi	_set0
	ora	r1
	beq	_set0
	bne	_set1
	
	.entry	Ault
Ault:	jsr	Acmp
	bcc	_set1
	bcs	_set0

	.entry	Aule
Aule:	jsr	Acmp
	bcc	_set1
	ora	r2
	beq	_set1
	bne	_set0

	.entry	Auge
Auge:	jsr	Acmp
	bcs	_set1
	bcc	_set0
	
	.entry	Augt
Augt:	jsr	Acmp
	bcc	_set0
	ora	r2
	beq	_set0
	bne	_set1

	.entry	Ajsi
Ajsi:	jmp	[r1]
	
	.entry	Aswt
Aswt:	sta	jp
	sty	jph
$1:	ldy	#0
	lda	[jp],y
	sta	r3
	iny
	lda	[jp],y
	sta	r3h
	iny
	ora	r3
	beq	$3
	sec
	lda	r1
	sbc	[jp],y
	bne	$2
	lda	r1h
	iny
	sbc	[jp],y
	bne	$2
	jmp	[r3]
$2:	clc
	lda	jp
	adc	#4
	sta	jp
	bcc	*+4
	inc	jph
	jmp	$1
$3:	lda	[jp],y
	sta	r3
	iny
	lda	[jp],y
	sta	r3h
	jmp	[r3]
	
	.entry	Albit
Albit:	beq	$2
$1:	lsr	r1h
	ror	r1
	dex
	bne	$1
$2:	lda	_mask,y
	and	r1
	sta	r1
	lda	_mask+1,y
	and	r1h
	sta	r1h
	rts

	.entry	Asbit
Asbit:	lda	_mask,x
	and	r1
	sta	r3
	lda	_mask,x
	eor	#0xff
	sta	smask
	lda	_mask+1,x
	and	r1h
	sta	r3h
	lda	_mask+1,x
	eor	#0xff
	sta	smaskh
	tya
	beq	$2
$1:	lda	smaskh
	asl	a
	rol	smask
	rol	smaskh
	asl	r3
	rol	r3h
	dey
	bne	$1
$2:	rts

_mask:
	.word	0x0000,0x0001,0x0003,0x0007
	.word	0x000f,0x001f,0x003f,0x007f
	.word	0x00ff,0x01ff,0x03ff,0x07ff
	.word	0x0fff,0x1fff,0x3fff,0x7fff
	.word	0xffff

	.entry	Amov
Amov:	sta	r3
	sty	r3h
	lda	r1		;put orignal dst addr in r4
	sta	r4
	lda	r1h
	sta	r4h
	ldy	#0
$1:	lda	[r2],y
	sta	[r4],y
	cpy	r3		;test low count
	bne	$3		;not there so branch
	lda	r3h		;there -- test hi count
	bne	$2		;no there -- decrement and continue
	rts			;there -- return
$2:	dec	r3h		;decrement hi count
$3:	iny
	bne	$1
	inc	r2h
	inc	r4h
	jmp	$1
	
; --- end of rt65.a ---

