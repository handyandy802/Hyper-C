/*
 * opsysl.c -- 6502 operating system support...
 */

/*
 * Version 1.3 -- added _pcrlf byte at 0x3a for no linefeed on print...
 *						dm 10/23/85
 * Version 1.2 -- fixed problem with '%' followed by any non-control
 *					character in xprintf.c  10/11/85
 * Version 1.1 -- fixed problem with variable width fields in xprintf
 *					Opsys relinked -- 9/26/85
 * Version 1.0 -- initial release 9/15/85
 */
 
#include <std.h>
#include <errors.h>
#include <hdr.h>
#include <prgtbl.h>
#include <setjmp.h>

#define AARS

/*----------------------------------------------------------------------
 * Program Loader...
 */

EXTERN TEXT		*_pname;
EXTERN VOID		(*_errHdlr)();
EXTERN UBYTE	_ioresult;
EXTERN WORD		_exitVal;
EXTERN BYTE		_sysfd;
EXTERN TEXT 	_sysline[128];

LOCAL JMPVECT 	_exitvector;
LOCAL INT		_oldPrt;

LOCAL OBJHDR	_progHdr;
LOCAL BYTE		_fileLevel	@0xbf94;
LOCAL TEXT		proPath[] 	@0x0280;
LOCAL TEXT		shellName[] = "shell";

/*
 * dkError -- record and attempt to report a disk error...
 */
VOID dkError(errCode, str)
INT errCode;
TEXT *str;
{
	if((_ioresult = errCode) && _errHdlr)
		(*_errHdlr)(errCode, str);
}

INT *load(name)
TEXT *name;
{
	if((_sysfd = open(name)) <= 0)
	{
		dkError(_ioresult, name);
		return(NIL);
	}
	return(ovload(_sysfd));
}

INT ovload(fd, name)
INT fd;
TEXT *name;
{
	INT  readLen;
	INT	 *uprog;
	
	if(sizeof(_progHdr) != read(fd, _progHdr, sizeof(_progHdr)) ||
		_progHdr.h_magic != 0x100)
	{
badprog:
		dkError(NOTPROG, name);
		return(NIL);
	}
	if((uprog = _progHdr.h_tloc) < 0x2000)
		goto badprog;
	readLen = _progHdr.h_text + _progHdr.h_data;
	if(readLen != read(fd, (INT)uprog, readLen))
		goto badprog;
	clrbuf((INT)uprog + readLen, _progHdr.h_bss);
	return(uprog);
}
	
INT exec(name, ac, av)
TEXT *name;
INT  ac;
TEXT **av;
{
	VOID (*uprog)();
	
	if(!(uprog = load(name)))
		return(_ioresult);
	initMem((INT)uprog + _progHdr.h_text + _progHdr.h_data + _progHdr.h_bss);
	_pname = av[0];
	return((*uprog)(ac, av));
}
	
LOCAL VOID reportErr(errCode, msg)
INT  errCode;
TEXT *msg;
{
	printf("\r\7%s: error #0x%02x", _pname, errCode);
	if(msg)
		printf(" --- %s", msg);
	cr();
	exit(-errCode);
}

VOID exit(val, str)
{
	close(0);
	_exitVal = val;
	mover(str, _sysline, 128);
	longjmp(&_exitvector, 1);
}
	
VOID _usrAbort()
{
	dkError(USRABORT, NIL);
}

VOID _commandLoop()
{
	EXTERN WORD _prtHook;
	VOID (*shell)();

	_vidinit();
	_oldPrt = _prtHook;
	_exitVal = 9999;
	setjmp(&_exitvector);
	_pname = "boot";
	_prtHook = _oldPrt;
	_errHdlr = NIL;
loop:
	if(shell = load(shellName))
		goto gotIt;
	mover(shellName, &proPath[proPath[0]+1], strlen(shellName)+1);
	if(shell = load(&proPath[1]))
	{
gotIt:
		_errHdlr = &reportErr;
		(*shell)();
	}
	printf("\r\r\7\7\7Place %s online...\r", &proPath[1]);
	putstr("Hit any key to continue...\r");
	getkey(YES);
	goto loop;
}

/* --- end of opsysl.c --- */


