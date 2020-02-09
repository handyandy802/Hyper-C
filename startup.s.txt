;
; startup.a -- opsys startup file...
;

#include <regs65.ah>

BRKVect		= 0x03f0
StackBottom = 0x9e00

	jmp	startup		;0x00
	jmp	__chkAbort
	jmp	__prtinit
	jmp	__prtonly
	jmp	__prtvid
	jmp	__vidinit
	jmp	__vidonly
	jmp	_alloc

	jmp	_clrbuf		;0x08
	jmp	_clrkbd
	jmp	_conRead
	jmp	_conWrite
	jmp	_create
	jmp	_exec
	jmp	_exit
	jmp	_free

	jmp	__setPath	;0x10
	jmp	_getFPos
	jmp	_getc
	jmp	_getcurs
	jmp	_getkey
	jmp	_getl
	jmp	_kbdRead
	jmp	_keypress

	jmp	_load		;0x18
	jmp	_longjmp
	jmp	_mover
	jmp	_nalloc
	jmp	_open
	jmp	_printf
	jmp	_prtWrite
	jmp	_putc

	jmp	_putchr		;0x20
	jmp	_putprt
	jmp	_puts
	jmp	_putstr
	jmp	_read
	jmp	_remove
	jmp	_rename
	jmp	_sbreak

	jmp	_seek		;0x28
	jmp	_setFPos
	jmp	_setcurs
	jmp	_setjmp
	jmp	_strlen
	jmp	_sync
	jmp	_write
	jmp	_xprintf

	jmp	setup		;0x30
	jmp	_ovload
	jmp	__ladd
	jmp	__quit
	jmp	_close
	jmp	__setPrefix
	jmp	_getEOF
	jmp	_setEOF

__pcrlf = 0x3a
_memMap = 0xbf58

	.entry	startup
startup:
	cld
	lda	#<__brk
	sta	BRKVect
	lda	#>__brk
	sta	BRKVect+1
	ldx	#0xff
	stx	__pcrlf
	txs
	lda	#<StackBottom
	sta	sp
	lda	#>StackBottom
	sta	sph
	ldx	#23
_memloop:
	lda	_mymap,x
	sta	_memMap,x 	;mark out entire memory map...
	dex
	bpl	_memloop
	jsr	__commandLoop

_mymap:
	.byte	0xff,0,0,0		;0x00-0x1f
	.byte	0,0,0,0			;0x20-0x3f
	.byte	0,0,0,0			;0x40-0x5f
	.byte	0,0,0,0			;0x60-0x7f
	.byte	0,0,0,0			;0x80-0x9f
    .byte   0,0,0,3         ;0xa0-0xbf

	.entry	__quit
__quit:
	lda	#0
	sta	0xbf94	;set file level to zero
	jsr	0xbf00	;perform global file close call
	.byte	0xcc
	.word	glbclose
	jsr	0xbf00	;perform quit call...
	.byte	0x65
	.word	quitparms

glbclose:
	.byte	1,0

quitparms:
	.byte	4,0
	.word	0
	.byte	0
	.word	0
	
; --- end of startup.a ---

