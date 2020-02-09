;
; boot.s -- PRODOS bootup of C Environment...
;

mli		=	0xbf00		;prodos machine language interface
buffer	=	0x4000		;our local buffer space
length	= 	0x1800		;expected length of opsys
opsys	=	0x0800		;start address of opsys
powerup	=	0x03f4		;reset powerup byte
myName  =	0x0280		;address of my program name
setPref	=	0xc6		;mli set prefix call
getPref	=	0xc7		;mli get prefix call
open	=	0xc8		;mli open call
read	=	0xca		;mli read call
close	=	0xcc		;mli close call
quit	=	0x65		;mli quit call

csys:
	lda		#0			;minimum acceptable prodos version
	sta		0xbffc		;  number for this system file
	lda		#3			;version number of this system file
	sta		0xbffd		;let prodos know about it
	ldx		#0xff		;initialize the stack pointer
	txs

	ldx		myName		;null terminate my name
	lda		#0
	sta		myName+1,x
	tax					;init rindex
	ldy		#1			;start index
loop:					;scan and convert my name to lowercase
	lda		myName,y
	beq		setPrefix
	and		#0x7f
	sta		myName,y
	cmp		#'/'		;is it a '/' character?
	bne		noslash
	tya					;yes - remember its position
	tax
noslash:
	iny					;no - point to next character
	bne		loop
setPrefix:
	txa					;was '/' seen?
	beq		getPrefix	;no -- get our current prefix
	stx		myName		;save prefix count at front of string
	jsr		mli			;set new prefix
	.byte	setPref
	.word	prefixParms
	bcs		error
	bcc		openIt

getPrefix:				;get existing prefix
	jsr		mli
	.byte	getPref
	.word	prefixParms
	
openIt:
	ldx		myName		;null terminate prefix for opsys
	lda		#0
	sta		myName+1,x

	jsr		mli			;open opsys file
	.byte	open
	.word	openParms
	bcs		error

	lda		refnum		;init file refnums
	sta		readref
	sta		closeref

	jsr		mli			;read opsys file into memory
	.byte	read
	.word	readParms
	bcs 	error

	jsr		mli			;now close the file
	.byte	close
	.word	closeParms
	bcs		error

	inc		powerup		;force cold-start on reset
	jmp		opsys		;enter the operating system

error:
	jsr		mli			;quit if error
	.byte	quit
	.word	quitParms
	
quitParms:
	.byte	4
	.byte	0
	.word	0
	.byte	0
	.word	0

openParms:
	.byte	3
	.word	path		;ptr to pathname for open
	.word	buffer		;ptr to file buffer
refnum:
	.byte	0
	
path:
	.byte	5,"opsys"	;name of opsys file
	
prefixParms:
	.byte	1
	.word	myName
	
readParms:
	.byte	4
readref:
	.byte	0			;refnum of file to read
	.word	opsys		;ptr to read-to location
	.word	length		;nbr bytes to read
	.word	0

closeParms:
	.byte	1			;refnum of file to close
closeref:
	.byte	0
	
; -- end of boot.s ---

