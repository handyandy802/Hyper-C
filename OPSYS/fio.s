;
; fio.s -- file i/o interface...
;

#include <regs65.ah>

		.entry	__filePath
__filePath:
		ds	64
		.entry	__newPath
__newPath:
		ds	64

getBuf:
		lda	#0xc0
		sta	bufAddr+1
$1:		lda	bufAddr+1
		beq	$2
		jsr	tstavail
		bne $1
		jsr	tstavail
		bne	$1
		jsr	tstavail
		bne	$1
		jsr	tstavail
		bne	$1
$2:		rts
		
bitMap	= 0xbf58

tstavail:
		dec	bufAddr+1
		lda	bufAddr+1
		lsr	a
		lsr	a
		lsr	a
		tax
		lda	bufAddr+1
		and	#7
		tay
		lda	bitMap,x
		and	bitmask,y
		rts
		
bitmask:
		.byte	128,64,32,16,8,4,2,1
		
openParms:
		.byte	3
		.word	__filePath
bufAddr:
		.word	0xf900
refnum:	.byte	0
				
		.entry	__open
__open:
		jsr		getBuf
		jsr		0xbf00
		.byte	0xc8
		.word	openParms
		sta		__ioresult
		bcs		badopen
		lda		refnum
		sta		r1
		lda		#0
		sta		r1h
		rts
badopen:
		lda		#0xff
		sta		r1
		sta		r1h
		rts
		
closeParms:
		.byte	1
closeFD:
		.byte	0
		
		.entry	_close
_close:
		ldy		#0
		lda		[sp],y
		sta		closeFD
		jsr		0xbf00
		.byte	0xcc
		.word	closeParms
		sta		__ioresult
		rts
		
nlParms:
		.byte	3
nlFD:	.byte	0
nlMask:	.byte	0
nlChar:	.byte	0x0d

		.entry	_getl
_getl:
		lda		#0x7f
		ldx		#0x0d
		bne		read1

		.entry	_read
_read:
		lda		#0
		tax
read1:
		sta		nlMask
		stx		nlChar
		jsr		rwSetup
		lda		rwFD
		sta		nlFD
		jsr		0xbf00
		.byte	0xc9	;newline
		.word	nlParms
		bcs		badopen
		jsr		0xbf00
		.byte	0xca	;read
		.word	rwParms
rwfinis:
		sta		__ioresult
		lda		rwAct
		sta		r1
		lda		rwAct+1
		sta		r1h
		rts

		.entry	_write
_write:
		jsr		rwSetup
		jsr		0xbf00
		.byte	0xcb
		.word	rwParms
		jmp		rwfinis

rwParms:
		.byte	4
rwFD:	.byte	0
rwBuf:	.word	0
rwLen:	.word	0
rwAct:	.word	0
		
rwSetup:
		ldy		#0
		lda		[sp],y
		sta		rwFD
		ldy		#2
		lda		[sp],y
		sta		rwBuf
		iny
		lda		[sp],y
		sta		rwBuf+1
		iny
		lda		[sp],y
		sta		rwLen
		iny
		lda		[sp],y
		sta		rwLen+1
		rts
		
destroyParms:
		.byte	1
		.word	__filePath
		
		.entry	__destroy
__destroy:
		jsr		0xbf00
		.byte	0xc1
		.word	destroyParms
		sta		__ioresult
		rts
		
createParms:
		.byte	7
		.word	__filePath
		.byte	0xc3		;full priveleges
		.byte	4			;text file
		.word	0			;recsize
		.byte	1			;seedling file
		.word	0			;creation date
		.word	0			;creation time
		
		.entry	__create
__create:
		jsr		0xbf00
		.byte	0xc0
		.word	createParms
		sta		__ioresult
		rts

renameParms:
		.byte	2
renOld:
		.word	__filePath
renNew:
		.word	__newPath
		
		.entry	__rename
__rename:
		jsr		0xbf00
		.byte	0xc2
		.word	renameParms
		sta		__ioresult
		rts

flushParms:
		.byte	1
flushFD:
		.byte	0
		
		.entry	__flush
__flush:
		jsr		0xbf00
		.byte	0xcd
		.word	flushParms
		sta		__ioresult
		rts
		
prefParms:
		.byte	1
		.word	__filePath
				
		.entry	__setPrefix
__setPrefix:
		jsr		0xbf00
		.byte	0xc6
		.word	prefParms
		sta		__ioresult
		rts
		
posParms:
		.byte	2
posFD:	.byte	0
posPos:	.byte	0
		.word	0
		
setpos:
		ldy		#3
		jsr		setup
		lda		0
		sta		posFD
		ldy		#0
		lda		[2],y
		sta		posPos+2
		iny
		iny
		lda		[2],y
		sta		posPos
		iny
		lda		[2],y
		sta		posPos+1
		rts

		.entry	_setFPos
_setFPos:
		jsr		setpos
		jsr		0xbf00
		.byte	0xce
		.word	posParms
		sta		__ioresult
		rts

		.entry	_setEOF
_setEOF:
		jsr		setpos
		jsr		0xbf00
		.byte	0xd0
		.word	posParms
		sta		__ioresult
		rts
		
		.entry	_getEOF
_getEOF:
		ldy		#0
		lda		[sp],y
		sta		posFD
		jsr		0xbf00
		.byte	0xd1
		.word	posParms
		jmp		getPos
		
		.entry	_getFPos
_getFPos:
		ldy		#0
		lda		[sp],y
		sta		posFD
		jsr		0xbf00
		.byte	0xcf
		.word	posParms
getPos:
		sta		__ioresult
		ldy		#3
		jsr		setup
		ldy		#1
		lda		#0
		sta		[2],y
		dey
		lda		posPos+2
		sta		[2],y
		ldy		#3
		lda		posPos+1
		sta		[2],y
		dey
		lda		posPos
		sta		[2],y
		rts

getPrefParms:
		.byte	1
getPrefBuf:
		.word	0
				
		.entry	__getPrefix
__getPrefix:
		ldy		#0
		lda		[sp],y
		sta		getPrefBuf
		iny
		lda		[sp],y
		sta		getPrefBuf+1
		jsr		0xbf00
		.byte	0xc7
		.word	getPrefParms
		rts
		
; --- end of fio.s ---

