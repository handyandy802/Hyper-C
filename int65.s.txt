;
;	int65.a -- 6502 p-Code interpreter...
;

#include <regs65.ah>

	.entry	__brk
__brk:
	lda	pch
	ldx	pc
	jsr	_pushax
	ldx	0x3a		;get saved pc low
	bne	_brk1
	dec	0x3b		;saved pc high
_brk1:	dex
	stx	pc
	lda	0x3b
	sta	pch
	lda	0x48		;get saved status register
	pha
	plp
_next:
	ldy	#0		;fetch next opcode
	lda	[pc],y
	inc	pc
	bne	*+4
	inc	pch
	asl	a
	tax
	lda	_jtbl,x
	sta	jp
	lda	_jtbl+1,x
	sta	jph
	jmp	[jp]

_getImm:	; get signed immediate value in (a,x)
	lda	[pc],y
	tax
	bcs	_getImm1
	bpl	*+3
	dey
	tya
incpc:	inc	pc
	bne	*+4
	inc	pch
	rts
_getImm1:
	iny
	lda	[pc],y
incpc2:	jsr	incpc
	jmp	incpc

_getUImm:	; get unsigned immediate value in r3
	lda	[pc],y
	sta	r3
	bcs	$1
	sty	r3h
	jmp	incpc
$1:	iny
	lda	[pc],y
	sta	r3h
	jmp	incpc2
	
_getAddr:	; get immediate address in jp
	lda	[pc],y
	bcs	$1
	bpl	*+3
	dey
	adc	pc
	sta	jp
	tya
	adc	pch
	sta	jph
	jmp	incpc
$1:	sta	jp
	iny
	lda	[pc],y
	sta	jph
	jmp	incpc2

_getIAddr:
	ldx	#r1
	bne	_getIFAddr
	
_getI2Addr:
	ldx	#r2
	bne	_getIFAddr
	
_getVAddr:
	php
	lda	[pc],y
	jsr	incpc
	clc
	adc	#dsply
	tax
	plp
	bne	_getIFAddr
	
_getFAddr:
	ldx	#fp
_getIFAddr:		
	lda	[pc],y
	bcs	$1
	bpl	*+3
	dey
	adc	0,x
	sta	jp
	tya
	adc	1,x
	sta	jph
	jmp	incpc
$1:	clc
	adc	0,x
	sta	jp
	iny
	lda	[pc],y
	adc	1,x
	sta	jph
	jmp	incpc2
	
_ldc:
	jsr	_getImm
	sta	r1h
	stx	r1
	jmp	_next
	
_ldc0:
	sty	r1
	sty	r1h
	jmp	_next
	
_ldc1:
	sty	r1h
	iny
	sty	r1
	jmp	_next
	
_ldcb:
	jsr	_getImm
	sta	r2h
	stx	r2
	jmp	_next
	
_pdc0:
	tya
	tax
	beq	pushit
	
_pdc1:
	ldx	#1
	tya
	beq	pushit
	
_pdc:
	jsr	_getImm
pushit:	jsr	_pushax
	jmp	_next

_pushax:
	tay
	sec
	lda	sp
	sbc	#2
	sta	sp
	bcs	*+4
	dec	sph
	tya
	ldy	#1
	sta	[sp],y
	txa
	dey
	sta	[sp],y
	rts

_lea:
	jsr	_getAddr
_lea1:
	lda	jp
	sta	r1
	lda	jph
	sta	r1h
	jmp	_next
	
_lla:
	jsr	_getFAddr
	jmp	_lea1
	
_lva:	jsr	_getVAddr
	jmp	_lea1
	
_pea:
	jsr	_getAddr
_pea1:
	ldx	jp
	lda	jph
	jmp	pushit

_pla:
	jsr	_getFAddr
	jmp	_pea1
	
_pva:	jsr	_getVAddr
	jmp	_pea1
	
_tab:
	lda	r1
	sta	r2
	lda	r1h
	sta	r2h
	jmp	_next
	
_psh:
	ldx	r1
	lda	r1h
	jmp	pushit
	
_pop:
	lda	[sp],y
	sta	r2
	iny
	lda	[sp],y
	sta	r2h
	lda	sp		;carry already clear from next!
	adc	#2
	sta	sp
	bcc	*+4
	inc	sph
	jmp	_next
	
_ldu:
	jsr	_getAddr
	ldy	#0
	lda	[jp],y
	sta	r1
	sty	r1h
	jmp	_next
	
_pdu:
	jsr	_getAddr
	ldy	#0
	lda	[jp],y
	tax
	tya
	jmp	pushit
	
;_ldb:
;	jsr	_getAddr
;	ldy	#0
;	lda	[jp],y
;	sta	r1
;	bpl	*+3
;	dey
;	sty	r1h
;	jmp	_next
	
;_pdb:
;	jsr	_getAddr
;	ldy	#0
;	lda	[jp],y
;	bpl	*+3
;	dey
;	tax
;	tya
;	jmp	pushit
	
_ldw:
	jsr	_getAddr
	ldy	#0
	lda	[jp],y
	sta	r1
	iny
	lda	[jp],y
	sta	r1h
	jmp	_next
	
_pdw:
	jsr	_getAddr
	ldy	#0
	lda	[jp],y
	tax
	iny
	lda	[jp],y
	jmp	pushit
	
_lvu:	jsr	_getVAddr
	jmp	_llux
	
_llu:
	jsr	_getFAddr
_llux:
	ldy	#0
	lda	[jp],y
	sta	r1
	sty	r1h
	jmp	_next
	
_pvu:	jsr	_getVAddr
	jmp	_plux
	
_plu:
	jsr	_getFAddr
_plux:
	ldy	#0
	lda	[jp],y
	tax
	tya
	jmp	pushit
	
;_llb:
;	jsr	_getFAddr
;_llbx:
;	ldy	#0
;	lda	[jp],y
;	sta	r1
;	bpl	*+3
;	dey
;	sty	r1h
;	jmp	_next
	
;_plb:
;	jsr	_getFAddr
;_plbx:
;	ldy	#0
;	lda	[jp],y
;	bpl	*+3
;	dey
;	tax
;	tya
;	jmp	pushit

_lvw:	jsr	_getVAddr
	jmp	_llwx
	
_llw:
	jsr	_getFAddr
_llwx:
	ldy	#0
_llwxx:
	lda	[jp],y
	sta	r1
	iny
	lda	[jp],y
	sta	r1h
	jmp	_next
	
_llw4:
	ldy	#4
_llwn:
	lda	[fp],y
	sta	r1
	iny
	lda	[fp],y
	sta	r1h
	jmp	_next
	
_llw6:
	ldy	#6
	bne	_llwn
	
_llw_2:
	lda	#0xfe
_llw_n:
	adc	fp		;carry is already clear from next!
	sta	jp
	lda	#0xff
	adc	fph
	sta	jph
	jmp	_llwxx
	
_llw_4:
	lda	#0xfc
	bne	_llw_n
	
_pvw:	jsr	_getVAddr
	jmp	_plwx
_plw:
	jsr	_getFAddr
_plwx:
	ldy	#0
_plwxx:
	lda	[jp],y
	tax
	iny
	lda	[jp],y
	jmp	pushit

_plw4:
	ldy	#4
_plwn:
	lda	[fp],y
	tax
	iny
	lda	[fp],y
	jmp	pushit
	
_plw6:
	ldy	#6
	bne	_plwn

_plw_2:
	lda	#0xfe
_plw_n:
	adc	fp		;carry is already clear from next!
	sta	jp
	lda	#0xff
	adc	fph
	sta	jph
	jmp	_plwxx
	
_plw_4:
	lda	#0xfc
	bne	_plw_n
	
_liu:
	jsr	_getIAddr
	jmp	_llux
	
_piu:
	jsr	_getIAddr
	jmp	_plux
	
;_lib:
;	jsr	_getIAddr
;	jmp	_llbx
;	
;_pib:
;	jsr	_getIAddr
;	jmp	_plbx
	
_liw:
	jsr	_getIAddr
	jmp	_llwx
	
_piw:
	jsr	_getIAddr
	jmp	_plwx
	
_sib:
	jsr	_getI2Addr
	jmp	_bstore
	
_stb:
	jsr	_getAddr
	jmp	_bstore
	
_svb:	jsr	_getVAddr
	jmp	_bstore
_slb:
	jsr	_getFAddr
_bstore:
	ldy	#0
	lda	r1
	sta	[jp],y
	jmp	_next
_bfldStore:
	lda	smask
	and	[jp],y		;mask off dest old field bits
	ora	r3		;or in new field bits
	sta	[jp],y		;save new field bits
unpatch:
	ldy	#2
$1:	lda	patch1,y
	sta	_bstore,y
	sta	_wstore,y
	dey
	bpl	$1
	jmp	_next
	
patch1:
	ldy	#0
	lda	r1
bpatch2:
	jmp	_bfldStore
wpatch2:
	jmp	_wfldStore

_siw:
	jsr	_getI2Addr
	jmp	_wstore

_stw:
	jsr	_getAddr
	jmp	_wstore
		
_svw:	jsr	_getVAddr
	jmp	_wstore
_slw:
	jsr	_getFAddr
_wstore:
	ldy	#0
	lda	r1		;normal store
	sta	[jp],y
	iny
	lda	r1h
	sta	[jp],y
	jmp	_next
_wfldStore:
	lda	smask		;bitfield store
	and	[jp],y		;mask out old field
	ora	r3		;or in new field
	sta	[jp],y		;save result
	lda	smaskh
	iny
	and	[jp],y
	ora	r3h
	sta	[jp],y
	jmp	unpatch
	
_slw_4:
	lda	#0xfc
	bne	_slw_n
	
_slw_2:
	lda	#0xfe
_slw_n:
	adc	fp		;carry already clear from next!
	sta	jp
	lda	#0xff
	adc	fph
	sta	jph
	jmp	_wstore
	
_inc:
	jsr	_getUImm
	clc
	lda	r1
	adc	r3
	sta	r1
	lda	r1h
	adc	r3h
	sta	r1h
	jmp	_next
	
_inc1:
	inc	r1
	bne	*+4
	inc	r1h
	jmp	_next
	
_inc2:
	lda	#2
_incn:
	adc	r1		;carry already clear from next!
	sta	r1
	bcc	*+4
	inc	r1h
	jmp	_next

_inc4:
	lda	#4
	bne	_incn

_inc6:
	lda	#6
	bne	_incn
	
_inc8:
	lda	#8
	bne	_incn
	
_dec:
	jsr	_getUImm
	sec
	lda	r1
	sbc	r3
	sta	r1
	lda	r1h
	sbc	r3h
	sta	r1h
	jmp	_next
	
_dec1:
	lda	r1
	bne	*+4
	dec	r1h
	dec	r1
	jmp	_next
	
_dec2:
	sec
	lda	r1
	sbc	#2
	sta	r1
	bcs	*+4
	dec	r1h
	jmp	_next
	
_isp:	;assume we will always have fewer than 256 bytes to pop!
	lda	[pc],y
	adc	sp		;carry already clear from next!
	sta	sp
	bcc	*+4
	inc	sph
	jsr	incpc
	jmp	_next
	

_isp2:
	lda	#2
_ispn:
	adc	sp		;carry already clear from next!
	sta	sp
	bcc	*+4
	inc	sph
	jmp	_next

_isp4:
	lda	#4
	bne	_ispn
	
_neg:
	sec
	tya
	sbc	r1
	sta	r1
	tya
	sbc	r1h
	sta	r1h
	jmp	_next
	
_not:
	lda	r1
	eor	#0xff
	sta	r1
	lda	r1h
	eor	#0xff
	sta	r1h
	jmp	_next
	
_add:
	lda	r1		;carry already clear from next!
	adc	r2
	sta	r1
	lda	r1h
	adc	r2h
	sta	r1h
	jmp	_next
	
_sub:
	sec
	lda	r1
	sbc	r2
	sta	r1
	lda	r1h
	sbc	r2h
	sta	r1h
	jmp	_next
	
_and:
	lda	r1
	and	r2
	sta	r1
	lda	r1h
	and	r2h
	sta	r1h
	jmp	_next
	
_or:
	lda	r1
	ora	r2
	sta	r1
	lda	r1h
	ora	r2h
	sta	r1h
	jmp	_next
	
_xor:
	lda	r1
	eor	r2
	sta	r1
	lda	r1h
	eor	r2h
	sta	r1h
	jmp	_next

_mul:
	lda	r2
	sta	r3
	lda	r2h
	sta	r3h
	jsr	_mpy
	jmp	_next
	
_mpi:
	jsr	_getUImm
	jsr	_mpy
	jmp	_next
	
_mpy:
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
	
_in:
	lda	r1		;swap r1 and r2
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
	jmp	_next
	
_incl:
	jsr	_getBits
	ora	[r2],y		;or in the set element	(y = 0)
	sta	[r2],y
	jmp	_next

_excl:
	jsr	_getBits
	eor	#0xff		;complement mask
	and	[r2],y		;turn off set element	(y = 0)
	sta	[r2],y
	jmp	_next
		
_getBits:
	tya			;clear a
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

_divu:
	jsr	_doDiv
	jmp	_next
	
_modu:
	jsr	_doDiv
_modu1:
	lda	r4		;move remainder to r1
	sta	r1
	lda	r4h
	sta	r1h
	jmp	_next
	
_div:
	jsr	_getSign
	jsr	_doDiv
	lsr	jp
	bcc	$1		;test result sign
	tya			;is negative -- clear a (y = 0)
	sec
	sbc	r1		;negate result
	sta	r1
	tya
	sbc	r1h
	sta	r1h
$1:	jmp	_next
	
_mod:				;signed remainder -- same sign as dividend
	jsr	_getSign
	jsr	_doDiv
	bit	jp		;test result sign
	bpl	_modu1		;positive -- move remainder to r1
	tya			;clear a (y = 0)
	sec
	sbc	r4		;negate result while storing in r1
	sta	r1
	tya
	sbc	r4h
	sta	r1h
	jmp	_next
	
_doDiv:
	lda	r2		;save copy of divisor in r3
	sta	r3
	lda	r2h
	sta	r3h
_doDivR3:			;divide r1 by r3
	sty	r4		;quotient in r1, remainder in r4
	sty	r4h		;clear initial remainder
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
;
; ensure divisor and dividend are positive...
; remember dividend and result sign...
; sign code stored in jp:
;	jp < 0 -- dividend was < 0
;	jp odd -- result is < 0
;
_getSign:
	sty	jp		;clear result sign code
	bit	r1h		;test dividend sign
	bpl	$1
	lda	#0x81		;save sign code
	sta	jp
	sec			;negate dividend so it's positive
	tya			;clear a (y = 0)
	sbc	r1
	sta	r1
	tya
	sbc	r1h
	sta	r1h
$1:	bit	r2h		;test sign of divisor
	bpl	$2
	inc	jp		;remember result sign (odd = negative)
	sec			;negate divisor so it's positive
	tya
	sbc	r2
	sta	r2
	tya
	sbc	r2h
	sta	r2h
$2:	rts
	
_dvi:
	jsr	_getUImm	;get divisor into r3
	jsr	_doDivR3
	jmp	_next
	
_dvi2:
	lsr	r1h
	ror	r1
	jmp	_next
	
_mpi2:
	asl	r1
	rol	r1h
	jmp	_next
	
_shl:
	ldy	r2
	beq	$2
$1:	asl	r1
	rol	r1h
	dey
	bne	$1
$2:	jmp	_next
	
_shr:
	ldy	r2
	beq	$2
$1:	lda	r1h
	asl	a
	ror	r1h
	ror	r1
	dey
	bne	$1
$2:	jmp	_next
	
_shru:
	ldy	r2
	beq	$2
$1:	lsr	r1h
	ror	r1
	dey
	bne	$1
$2:	jmp	_next
	
_cmp:
	sec
	lda	r1
	sbc	r2
	sta	r2
	lda	r1h
	sbc	r2h
	rts
	
_jeq:
	jsr	_getAddr
	jsr	_cmp
	ora	r2
	beq	_jmp1
	jmp	_next

_jeqz:
	lda	r1
	ora	r1h
	bne	_skip
	beq	_jmp

_jne:
	jsr	_getAddr
	jsr	_cmp
	ora	r2
	bne	_jmp1
	jmp	_next
	
_jnez:
	lda	r1
	ora	r1h
	bne	_jmp
	beq	_skip
	
_jlt:
	jsr	_getAddr
	jsr	_cmp
	bmi	_jmp1
	jmp	_next

_jltz:
	lda	r1h
	bmi	_jmp
	bpl	_skip
	
_jle:
	jsr	_getAddr
	jsr	_cmp
	bmi	_jmp1
	ora	r2
	beq	_jmp1
	jmp	_next

_jlez:
	lda	r1h
	bmi	_jmp
	ora	r1
	beq	_jmp
	bne	_skip
	
_jmp:
	jsr	_getAddr
_jmp1:
	lda	jp
	sta	pc
	lda	jph
	sta	pch
	jmp	_next

_skip:
	lda	pc
	adc	#1		;carry set from next?
	sta	pc
	bcc	*+4
	inc	pch
	jmp	_next

_jge:
	jsr	_getAddr
	jsr	_cmp
	bpl	_jmp1
	jmp	_next
	
_jgez:
	lda	r1h
	bpl	_jmp
	bmi	_skip
	
_jgt:
	jsr	_getAddr
	jsr	_cmp
	bmi	$1
	ora	r2
	bne	_jmp1
$1:	jmp	_next
	
_jgtz:
	lda	r1h
	bmi	_skip
	ora	r1
	bne	_jmp
	beq	_skip
	
_jult:
	jsr	_getAddr
	jsr	_cmp
	bcc	_jmp1
	jmp	_next
	
_jule:
	jsr	_getAddr
	jsr	_cmp
	bcc	_jmp1
	ora	r2
	beq	_jmp1
	jmp	_next
	
_juge:
	jsr	_getAddr
	jsr	_cmp
	bcs	_jmp1
	jmp	_next

_jugt:
	jsr	_getAddr
	jsr	_cmp
	bcc	$1
	ora	r2
	bne	_jmp1
$1:	jmp	_next
	
_eq:
	jsr	_cmp
	ora	r2
	bne	_set0
	beq	_set1
	
_eqz:
	lda	r1
	ora	r1h
	bne	_set0
	beq	_set1

_ne:
	jsr	_cmp
	ora	r2
	beq	_set0
	bne	_set1
	
_nez:
	lda	r1
	ora	r1h
	bne	_set1
	beq	_set0
	
_lt:
	jsr	_cmp
	bmi	_set1
	bpl	_set0
	
_ltz:
	lda	r1h
	bmi	_set1
	bpl	_set0
	
_le:
	jsr	_cmp
	bmi	_set1
	ora	r2
	beq	_set1
	bne	_set0
	
_lez:
	lda	r1h
	bmi	_set1
	ora	r1
	beq	_set1
	bne	_set0
	
_ge:
	jsr	_cmp
	bmi	_set0
	bpl	_set1
	
_gez:
	lda	r1h
	bmi	_set0
	bpl	_set1

_set0:
	sty	r1
	sty	r1h
	jmp	_next
_set1:
	sty	r1h
	iny
	sty	r1
	jmp	_next
	
_gt:
	jsr	_cmp
	bmi	_set0
	ora	r2
	beq	_set0
	bne	_set1

_gtz:
	lda	r1h
	bmi	_set0
	ora	r1
	beq	_set0
	bne	_set1
	
_ult:
	jsr	_cmp
	bcc	_set1
	bcs	_set0

_ule:
	jsr	_cmp
	bcc	_set1
	ora	r2
	beq	_set1
	bne	_set0

_uge:
	jsr	_cmp
	bcs	_set1
	bcc	_set0
	
_ugt:
	jsr	_cmp
	bcc	_set0
	ora	r2
	beq	_set0
	bne	_set1

_lnkv:	php
	lda	[pc],y
	sta	0
	jsr	incpc
	tax
	lda	dsply+1,x
	tay
	lda	dsply,x
	tax
	tya
	jsr	_pushax
	lda	fph
	ldx	fp
	jsr	_pushax
	ldx	0
	lda	sp
	sta	fp
	sta	dsply,x
	lda	sph
	sta	fph
	sta	dsply+1,x
	jmp	_lnk2
	
_lnk:	php
	lda	fph
	ldx	fp
	jsr	_pushax
	lda	sp
	sta	fp
	lda	sph
	sta	fph
_lnk2:
	plp
	ldy	#0
	jsr	_getUImm
	sec
	lda	sp
	sbc	r3
	sta	sp
	lda	sph
	sbc	r3h
	sta	sph
	lda	sp
	sec
	sbc	#2
	sta	sp
	bcs	*+4
	dec	sph
	ldy	#0
	pla
	sta	[sp],y
	iny
	pla
	sta	[sp],y
	jmp	_next
	
_jpjmp:
	jmp	[jp]
_jsr:
	jsr	_getAddr
	jsr	_jpjmp
	jmp	_next
	
_r1jmp:
	jmp	[r1]
_jsi:
	jsr	_r1jmp
	jmp	_next
	
_rtn:
	iny
	lda	[sp],y
	pha
	dey
	lda	[sp],y
	pha
	lda	fp
	sta	sp
	lda	fph
	sta	sph
	lda	[sp],y
	sta	fp
	iny
	lda	[sp],y
	sta	fph
	iny
	lda	[sp],y
	sta	pc
	iny
	lda	[sp],y
	sta	pch
	lda	sp
	adc	#4		;carry already clear from next!
	sta	sp
	bcc	*+4
	inc	sph
	rts

_rtnv:
	iny
	lda	[sp],y
	pha
	dey
	lda	[sp],y
	pha
	lda	[pc],y
	tax
	lda	fp
	sta	sp
	lda	fph
	sta	sph
	lda	[sp],y
	sta	fp
	iny
	lda	[sp],y
	sta	fph
	iny
	lda	[sp],y
	sta	dsply,x
	iny
	lda	[sp],y
	sta	dsply+1,x
	iny
	lda	[sp],y
	sta	pc
	iny
	lda	[sp],y
	sta	pch
	lda	sp
	adc	#6		;carry already clear from next!
	sta	sp
	bcc	*+4
	inc	sph
	rts

_swt:
	jsr	_getAddr
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
	lda	r3
	sta	pc
	lda	r3h
	sta	pch
	jmp	_next
$2:	clc
	lda	jp
	adc	#4
	sta	jp
	bcc	$1
	inc	jph
	jmp	$1
$3:	lda	[jp],y
	sta	pc
	iny
	lda	[jp],y
	sta	pch
	jmp	_next
	
_lbit:
	lda	[pc],y
	tax
	beq	$2
$1:	lsr	r1h
	ror	r1
	dex
	bne	$1
$2:	iny
	lda	[pc],y
	tay
	lda	_mask,y
	and	r1
	sta	r1
	lda	_mask+1,y
	and	r1h
	sta	r1h
	jsr	incpc2
	jmp	_next

_sbit:
	lda	[pc],y
	tax
	lda	_mask,x
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
	iny
	lda	[pc],y
	beq	$2
	tay
$1:	lda	smaskh
	asl	a
	rol	smask
	rol	smaskh
	asl	r3
	rol	r3h
	dey
	bne	$1
$2:	ldy	#2
$3:	lda	wpatch2,y
	sta	_wstore,y
	lda	bpatch2,y
	sta	_bstore,y
	dey
	bpl	$3
	jsr	incpc2
	jmp	_next
	
_mask:
	.word	0x0000,0x0001,0x0003,0x0007
	.word	0x000f,0x001f,0x003f,0x007f
	.word	0x00ff,0x01ff,0x03ff,0x07ff
	.word	0x0fff,0x1fff,0x3fff,0x7fff
	.word	0xffff

_mov:
	lda	r1		;put orignal dst addr in r4
	sta	r4
	lda	r1h
	sta	r4h
	jsr	_getUImm	;get byte count-1
	ldy	#0
$1:	lda	[r2],y
	sta	[r4],y
	cpy	r3		;test low count
	bne	$3		;not there so branch
	lda	r3h		;there -- test hi count
	bne	$2		;no there -- decrement and continue
	jmp	_next		;there -- return
$2:	dec	r3h		;decrement hi count
$3:	iny
	bne	$1
	inc	r2h
	inc	r4h
	jmp	$1
	
_jtbl:
	.word	_next	;_csp	0
	.word	_add
	.word	_and
	.word	_dec
	.word	_dec1
	.word	_dec2
	.word	_div
	.word	_divu
	.word	_dvi	;0x08
	.word	_dvi2
	.word	_excl
	.word	_in
	.word	_inc
	.word	_inc1
	.word	_inc2
	.word	_inc4
	.word	_inc6	;0x10
	.word	_inc8
	.word	_incl
	.word	_isp
	.word	_isp2
	.word	_isp4
	.word	_jeq
	.word	_jne
	.word	_jlt	;0x18
	.word	_jle
	.word	_jge
	.word	_jgt
	.word	_jeqz
	.word	_jnez
	.word	_jltz
	.word	_jlez
	.word	_jgez	;0x20
	.word	_jgtz
	.word	_jult
	.word	_jule
	.word	_juge
	.word	_jugt
	.word	_jmp
	.word	_jsi
	.word	_jsr	;0x28
	.word	_ldc
	.word	_ldc0
	.word	_ldc1
	.word	_ldcb
	.word	_lea
	.word	_ldu
	.word	_ldw
	.word	_lbit	;0x30
	.word	_liu
	.word	_liw
	.word	_lla
	.word	_llu
	.word	_llw
	.word	_llw4
	.word	_llw6
	.word	_llw_2	;0x38
	.word	_llw_4
	.word	_lva
	.word	_lvu
	.word	_lvw
	.word	_lnk
	.word	_lnkv
	.word	_mod
	.word	_modu	;0x40
	.word	_mov
	.word	_mpi
	.word	_mpi2
	.word	_mul
	.word	_neg
	.word	_not
	.word	_or
	.word	_pdc	;0x48
	.word	_pdc0
	.word	_pdc1
	.word	_pea
	.word	_pdu
	.word	_pdw
	.word	_piu
	.word	_piw
	.word	_pla	;0x50
	.word	_plu
	.word	_plw
	.word	_plw4
	.word	_plw6
	.word	_plw_2
	.word	_plw_4
	.word	_pva
	.word	_pvu	;0x58
	.word	_pvw
	.word	_pop
	.word	_psh
	.word	_rtn
	.word	_rtnv
	.word	_sbit
	.word	_shl
	.word	_shr	;0x60
	.word	_shru
	.word	_sib
	.word	_siw
	.word	_slb
	.word	_slw
	.word	_slw_2
	.word	_slw_4
	.word	_stb	;0x68
	.word	_stw
	.word	_svb
	.word	_svw
	.word	_sub
	.word	_swt
	.word	_tab
	.word	_xor
	.word	_eq	;0x70
	.word	_ne
	.word	_lt
	.word	_le
	.word	_ge
	.word	_gt
	.word	_eqz
	.word	_nez
	.word	_ltz	;0x78
	.word	_lez
	.word	_gez
	.word	_gtz
	.word	_ult
	.word	_ule
	.word	_uge
	.word	_ugt

; --- end of interp65.a ---

