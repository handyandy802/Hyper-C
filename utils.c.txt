/*
 * utils.cc -- resident utilities...
 */
 
#include <std.h>
#include <errors.h>
#include <prgtbl.h>

EXTERN BYTE _ioresult;
EXTERN VOID error();
EXTERN TEXT execPath[];

/*
 * In the following routines, we have assumed that a call to dkError will
 * never return...  This is fair, since these routines should only be called
 * from the shell... His error handler does a longjmp back to the beginning
 * of the shell loop...
 */
 
PRGTBL _resprog[] = {
	{"cp", &_docp},
	{"mv", &_domv},
	{"ls", &_dols},
	{"rm", &_dorm},
	{"ren", &_doren},
	{"cls", &_docls},
	{"cd", &_docd},
	{"dm", &_dodump},
	{"bye", &_quit},
	{"vols", &_dovols},
	{"mkdir", &_domkdir},
	{"mksys", &_domksys},
	{"path", &_dopath},
	{"pwd", &_dopwd},
	{"?", &_dohelp},
	NIL
};

LOCAL TEXT *helpText[] =
{
	"?                      ;this help info",
	"bye                    ;exit csystem",
	"cd <path>              ;change dir",
	"cls                    ;clear screen",
	"cp <from> <to>         ;copy files",
	"dm [addr]              ;dump memory",
	"ls [<files>]           ;list files",
	"mkdir <paths>          ;make directories",
	"mksys <files>          ;make into SYS files",
	"mv <from> <to>         ;move files",
	"path [<path>]          ;print/set exec path",
	"pwd                    ;print working directory",
	"ren <path> <newName>   ;rename file",
	"rm <files>             ;remove files",
	"vols                   ;print volumes online",
};

LOCAL VOID _dopath(ac, av)
INT ac;
TEXT **av;
{
	if(ac > 1)
		mover(av[1], execPath, 64);
	else
	{
		putstr(execPath);
		putchr('\n');
	}
	return(OK);
}

LOCAL VOID _dohelp()
{
	TEXT **cp;
	
	for(cp = helpText; *cp; ++cp)
	{
		putstr(*cp);
		putchr('\n');
	}
	return(OK);
}

LOCAL VOID _dopwd()
{
	TEXT fbuf[65];
	
	_getPrefix(&fbuf);
	fbuf[fbuf[0]] = 0;
	putstr(&fbuf[1]);
	putchr('\n');
	return(OK);
}

LOCAL VOID _domksys(ac, av)
INT ac;
TEXT **av;
{
	EXTERN BYTE _infoType;
	EXTERN WORD	_infoAuxType;
	
	if(ac < 1)
		error("usage: mksys <files>", NIL);
	while(--ac > 0)
	{
		setPath(*++av);
		_getFInfo();
		_infoType = 0xff;
		_infoAuxType = 0x2000;
		_setFInfo();
		printf("%s:\n", *av);
	}
	return(OK);
}
		
LOCAL VOID chkerr(name)
TEXT *name;
{
	if(_ioresult)
		printf("%s: error #0x%02x\n\7", name, _ioresult);
}

LOCAL VOID _domkdir(ac, av)
INT ac;
TEXT **av;
{
	while(--ac > 0)
	{
		setPath(*++av);
		_mkdir();
		chkerr(*av);
	}
	return(OK);
}
		
LOCAL VOID _docls()
{
	page();
	return(OK);
}

LOCAL VOID _docd(ac, av)
INT ac;
TEXT **av;
{
	if(ac > 1)
	{
		_setPath(av[1]);
		_setPrefix();
	}
	return(OK);
}

LOCAL VOID _dodump(ac, av)
INT ac;
TEXT **av;
{
	TEXT *addrStr;
	CHAR ch;
	INT i, j;
	LOCAL UBYTE *addr = 0;
	
	if(addrStr = av[1])
	{
		addr = 0;
		while(ishex(ch = *addrStr++))
		{
			if(ch > '9')
			{
				if(ch >= 'a')
					ch -= 0x20;
				ch -= 7;
			}
			addr = 16 * (INT)addr + ch - '0';
		}
	}
	for(i = 0; i < 16; ++i)
	{
		printf("%04x  ", addr);
		for(j = 0; j < 16; j += 2)
			printf("%02x%02x ", addr[j], addr[j+1]);
		putchr(' ');
		for(j = 0; j < 16; ++j)
		{
			ch = addr[j] & 0x7f;
			if(0x20 <= ch && ch < 0x7f)
				putchr(ch);
			else
				putchr('.');
		}
		addr += 16;
		putchr('\n');
	}
	return(OK);
}
		
LOCAL VOID _docp(ac, av)
INT ac;
TEXT **av;
{
	return(__docp(ac, av, NO));
}

LOCAL VOID _domv(ac, av)
INT ac;
TEXT **av;
{
	return(__docp(ac, av, YES));
}

LOCAL VOID __docp(ac, av, ismove)
INT ac;
TEXT **av;
BOOL ismove;
{
	INT i;
	INT len;
	INT  src;
	INT  dst;
	TEXT *prefix;
	TEXT *fname;
	TEXT toName[64];
	TEXT *buf;
	UINT  buflen;
	EXTERN BYTE _infoType;
	EXTERN INT _memory;
	
	if(ac < 3)
		error("usage: cp <from> <to>\n", NIL);
	++av;
	ac -= 2;
	setPath(av[ac]);
	_getFInfo();
	if(_ioresult)
		prefix = NIL;
	else
	{
		if(ac > 1 && _infoType != 0x0f)
			error(av[ac], "not a directory");
		if(_infoType == 0x0f)
			prefix = av[ac];
		else
			prefix = NIL;
	}
	buf = _memory;
	buflen = 8192;
/*
	while(((UINT)buf + buflen) > ((UINT)&buflen - 0x200))
		buflen >>= 1;
*/
	for(i = 0; i < ac; ++i)
	{
		setPath(av[i]);
		_getFInfo();
		if(_ioresult == 0 && _infoType == 0x0f)
			error(av[i], "is a directory");
		printf("%s:\n", av[i]);
		if(!prefix && streq(av[i], av[ac]))
			continue;
		src = mustOpen(av[i], NIL, &error);
		if(prefix)
		{
			if(fname = rindex(av[i], '/'))
				++fname;
			else
				fname = av[i];
			sprintf(toName, "%s/%s", prefix, fname);
		}
		else
			mover(av[ac], toName, strlen(av[ac])+1);
/*
		dst = mustCreate(toName);
		while(0 < (len = mustRead(src, buf, buflen, &error, av[i])))
			mustWrite(dst, buf, len, &error, toName);
*/
		dst = create(toName);
		while(0 < (len = read(src, &_memory, 8192)))
			write(dst, &_memory, len);
			
		close(src);
		close(dst);
		setPath(av[i]);
		_getFInfo();
		setPath(toName);
		_setFInfo();
		if(ismove)
			remove(av[i]);
	}
	return(OK);
}
	
LOCAL VOID _dorm(ac, av)
INT ac;
TEXT **av;
{
	TEXT *fileName;
	
	while(fileName = *++av)
	{
		printf("%s:\n", fileName);
		remove(fileName);
		chkerr(fileName);
	}
	return(OK);
}

LOCAL VOID _doren(ac, av)
INT ac;
TEXT **av;
{
	rename(av[1], av[2]);
	chkerr(av[1]);
	return(_ioresult);
}

LOCAL VOID _dovols()
{
	INT  nameLen;
	TEXT *cp;
	TEXT vbuf[256];
	
	_getVols(vbuf);
	cp = &vbuf;
	while(*cp)
	{
		nameLen = *cp & 15;
		if(nameLen)
			printf("%-15.*s\n", nameLen, &cp[1]);
		cp += 16;
	}
	return(OK);
}
	
LOCAL VOID _dols(ac, av)
INT ac;
TEXT **av;
{
	TEXT fbuf[64];
	
	if(ac < 2)
	{
		_getPrefix(&fbuf);
		fbuf[fbuf[0]] = 0;
		__dols(&fbuf[1]);
	}
	else while(ac > 1)
		__dols(av[--ac]);
	return(OK);
}
		
LOCAL VOID setPath(name)
TEXT *name;
{
	EXTERN TEXT _filePath[];
	
	_filePath[0] = strlen(name);
	mover(name, &_filePath[1], _filePath[0]);
}

LOCAL VOID __dols(name)
TEXT *name;
{
	INT fd;
	INT i, j;
	INT len;
	INT *ip;
	TEXT dirBuf[0x27];
	EXTERN WORD _infoSize;
	EXTERN BYTE _infoType;
	
	fd = mustOpen(name, NIL, &error);
	setPath(name);
	_getFInfo();
	printf("%3d %s:", _infoSize, name);
	if(_infoType == 0x0f)
	{
		seek(fd, 0, 4, 0);
		putchr('\n');
		for(i = j = 0;; ++i)
		{
			if(i && (i % 13 == 0))
				seek(fd, 0, 5, 1);
			if(0x27 != mustRead(fd, &dirBuf, 0x27, &error, name))
				break;
			if(dirBuf[0])
			{
				len = dirBuf[0] & 15;
                if(dirBuf[0] > 0xdf)
					ip = &dirBuf[0x21];
				else
					ip = &dirBuf[0x13];
				printf("%3d %-.*s", *ip, len, &dirBuf[1]);
                if(dirBuf[0] > 0x3f)
				{
					putchr('/');
					++len;
				}
				if(++j % 4 == 0)
					putchr('\n');
				else
					while(len++ < 17)
						putchr(' ');
			}
		}
	}
	if(j % 4)
		putchr('\n');
	close(fd);
}				

/* --- end of utils.cc --- */

