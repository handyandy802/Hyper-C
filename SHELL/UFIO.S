;
; ufio.s -- PRODOS interface for utilities...
;

#include <regs65.ah>

#define PRODOS	0xbf00

mkdirParms:
		.byte	7
		.word	__filePath
		.byte	0xc3		;full priveleges
		.byte	0x0f		;directory file
		.word	0			;recsize
		.byte	0x0d		;linked subdirectory file
		.word	0			;creation date
		.word	0			;creation time
		
		.entry	__mkdir
__mkdir:
		jsr		0xbf00
		.byte	0xc0
		.word	mkdirParms
		sta		__ioresult
		rts

		.entry	__getVols
__getVols:
		ldy		#0
		lda		[sp],y
		sta		volBuf
		iny
		lda		[sp],y
		sta		volBuf+1
		jsr		PRODOS
		.byte	0xc5
		.word	volParms
		sta		__ioresult
		rts

volParms:
		.byte	2
		.byte	0
volBuf:
		.word	0
		
		.entry	__getPrefix
__getPrefix:
		ldy		#0
		lda		[sp],y
		sta		prefBuf
		iny
		lda		[sp],y
		sta		prefBuf+1
		jsr		PRODOS
		.byte	0xc7
		.word	prefParms
		sta		__ioresult
		rts
		
prefParms:
		.byte	1
prefBuf:
		.word	0
		
		.entry	__getFInfo
__getFInfo:
		lda		#0x0a
		sta		__infoParms
		jsr		PRODOS
		.byte	0xc4
		.word	__infoParms
		sta		__ioresult
		rts
		
		.entry	__infoParms
__infoParms:
		.byte	0x0a
		.word	__filePath
		.entry	__infoAccess
__infoAccess:
		.byte	0
		.entry	__infoType
__infoType:
		.byte	0
		.entry	__infoAuxType
__infoAuxType:
		.word	0
		.byte	0
		.entry	__infoSize
__infoSize:
		.word	0
		.word	0
		.word	0
		.word	0
		.word	0

		.entry	__setFInfo
__setFInfo:
		lda		#7
		sta		__infoParms
		jsr		PRODOS
		.byte	0xc3
		.word	__infoParms
		sta		__ioresult
		rts
		
		.entry	__filePath
__filePath:
		.ds		64
		
;
; --- end of ufio.s --- 
;

