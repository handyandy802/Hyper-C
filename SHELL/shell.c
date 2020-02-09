/*
 * shell.c -- 6502 operating system shell support...
 */

/*
 * Version 2.0 -- made separate from opsys for ProDOS...
 *						dm 12/01/85
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

/*----------------------------------------------------------------------
 * Program Loader...
 */

#define MAXARGS		32
#define CMDSIZ		128

EXTERN PRGTBL 	_resprog[];
EXTERN TEXT		*_pname;
EXTERN VOID		(*_errHdlr)();
EXTERN UBYTE	_ioresult;
EXTERN WORD		_exitVal;
EXTERN BOOL		_pcrlf;
EXTERN TEXT 	_sysline[128];
EXTERN TEXT		_cmdline[128];
EXTERN TEXT 	*_argv[32];
EXTERN TEXT		*_cargv[32];
EXTERN BYTE		_cmdfd;
EXTERN BYTE		_cargc;

LOCAL OBJHDR	_progHdr;
LOCAL INT 		_argc;
LOCAL JMPVECT 	_exitVector;
LOCAL INT		_oldPrt;

LOCAL BYTE		_fileLevel	@0xbf94;
EXTERN BYTE		_memory;
LOCAL TEXT 		autocmd[] = "autoexec";
TEXT			execPath[]  @0x2c0;
TEXT			myPath[] = "bin/";
LOCAL TEXT		proPath[]   @0x280;

VOID main()
{
	PRGTBL *pp;
	TEXT   **av;
	EXTERN WORD _prtHook;
	INT    *oldHdlr;
	EXTERN VOID reportErr();
	TEXT   theFile[64];
	
	_oldPrt = _prtHook;
	if(_exitVal == 9999)
	{
		TEXT  *cp;
		
		banner();
		_exitVal = 99;
		_cmdfd = 0;
		_fileLevel = 0;
		mover(autocmd, _sysline, strlen(autocmd)+1);
		cp = &execPath;
		*cp++ = '|';
		mover(&proPath[1], cp, proPath[0]);
		cp += proPath[0];
		if(cp[-1] != '/')
			*cp++ = '/';
		mover(myPath, cp, strlen(myPath) + 1);
	}
	oldHdlr = _errHdlr;
	_errHdlr = &reportErr;
	setjmp(&_exitVector);
	for(;;)
	{
loop:
		close(0);
		_prtHook = _oldPrt;
		_pname = "opsys";
		if(_exitVal != 99)
			getCmd();
		if(!(_argc = getArgs(_sysline, _argv, MAXARGS)))
			goto loop;
		av = _argv;
		if(*av[0] == ';')
			goto loop;
		if(streq(*av, "print"))
		{
			_prtonly();
			if(*av[1] == '#' && isdigit(av[1][1]))
			{
				_prtinit(av[1][1]-'0');
				_pcrlf = (av[1][2] - '-');
				++av;
				--_argc;
			}
			--_argc;
			++av;
		}
		if(_argc > 0)
		{
			for(pp = &_resprog[0]; pp->name; ++pp)
				if(streq(*av, pp->name))
				{
					_pname = *av;
					_exitVal = (*pp->prog)(_argc, av);
					goto loop;
				}
			if(!binFile(&theFile, *av))
			{
				getCmdFile(&theFile, _argc, av);
				goto loop;
			}
			_errHdlr = oldHdlr;
			exec(theFile, _argc, av);
		}
	}
}
		
BOOL binFile(theFile, fname)
TEXT *theFile;
TEXT *fname;
{
	INT _sysfd;
	EXTERN VOID error();
	
	_sysfd = srchOpen(theFile, fname, execPath, &error);
	if(sizeof(_progHdr) !=
		mustRead(_sysfd, _progHdr, sizeof(_progHdr), &error, theFile) ||
		_progHdr.h_magic != 0x100)
	{
		close(_sysfd);
		return(NO);
	}
	close(_sysfd);
	return(YES);
}

VOID error(msg1, msg2)
TEXT *msg1, *msg2;
{
	beep();
	putstr(msg1);
	if(msg2)
	{
		putchr(' ');
		putstr(msg2);
	}
	cr();
	_exitVal = ERR;
	longjmp(&_exitVector, 1);
}
	
LOCAL VOID reportErr(errCode, msg)
INT  errCode;
TEXT *msg;
{
	printf("\r\7%s: error #0x%02x", _pname, errCode);
	if(msg)
		printf(" --- %s", msg);
	cr();
	_exitVal = -errCode;
	longjmp(&_exitVector, 1);
}

LOCAL VOID getCmdFile(theFile, ac, av)
TEXT *theFile;
INT  ac;
TEXT **av;
{
	EXTERN VOID error();
	
	if(_cmdfd)
		close(_cmdfd);
	_cmdfd = 0;
	_cmdline = _sysline;
	for(_cargc = 0; _cargc < ac; ++_cargc)
		_cargv[_cargc] = (INT)*av++ + (INT)_cmdline - (INT)_sysline;
	_cmdfd = mustOpen(theFile, NIL, &error);
	_fileLevel = 1;
	_exitVal = 0;
}

LOCAL VOID getCmd()
{
	INT len;
	INT i;
	INT j;
	INT n;
	TEXT *cp;
	TEXT buf[CMDSIZ];
	EXTERN WORD _prtHook;
				
	putstr("> ");
	if(_exitVal == 0 && _cmdfd > 0 &&
		(len = getl(_cmdfd, buf, sizeof(buf))) > 0)
	{
		buf[len] = 0;
		for(i = 0, j = 0; i < len;)
		{
			if(buf[i] == '$')
			{
				++i;
				if(isdigit(n = buf[i++]) && (n -= '0') < _cargc)
				{
					for(cp = _cargv[n]; *cp;)
						stuff(*cp++, &j);
				}
			}
			else
				stuff(buf[i++], &j);
		}
		_sysline[j] = 0;
		putstr(_sysline);
	}
	else
	{
		if(_cmdfd)
			close(_cmdfd);
		_cmdfd = 0;
		_fileLevel = 0;
		conRead(_sysline, sizeof(_sysline));
		_oldPrt = _prtHook;
	}
}

LOCAL VOID stuff(ch, ix)
CHAR ch;
VAR INT ix;
{
	if(ix < sizeof(_sysline) - 1)
		_sysline[ix++] = ch;
}

TEXT *_banner[] = {
"\rIf you have a copy of this program and are not a registered owner,\r",
"we ask you to become one today, showing your support of our efforts to\r",
"produce low cost, high quality software.  You can become a registered\r",
"owner by ordering the disks and a manual for $150.  Copies of the manual\r",
"may be purchased for $35.00.  Send your order to:\r\r",
"\t\t\t\t\tThe WSM Group, Inc.\r",
"\t\t\t\t\t  P.O. Box 32005\r",
"\t\t\t\t\tTucson, AZ  85751\r",
"\t\t\t\t\t  602/298-7910\r\r",
"We know you will enjoy using this system!  Thank you for your support.\r\r\r",
"\t\t\t\t---< Welcome to WSM Shell [3.0] >---\r\r\r",
NIL
};

VOID banner()
{
	TEXT **cp;
	
	for(cp = &_banner[0]; *cp; ++cp)
		putstr(*cp);
}

/* --- end of shell.c --- */

