/*
 * mustOpen.c --- Code to open files, requesting user to place online
 * if necessary...
 */

#include <std.h>

INT mustOpen(file, altPaths, myError)
TEXT *file;
TEXT *altPaths;
VOID (*myError)();
{
	TEXT namBuf[64];
	
	return(srchOpen(namBuf, file, altPaths, myError));
}

INT srchOpen(namBuf, file, altPaths, myError)
TEXT *namBuf;
TEXT *file;
TEXT *altPaths;
VOID (*myError)();
{
	INT  fd;
	CHAR ch;
	TEXT *path;
	TEXT *cp;
	EXTERN VOID error();
	
loop:
	path = altPaths;
	do {
		cp = namBuf;
		if(path && *path)
		{
			while(*path && *path != '|')
				*cp++ = *path++;
			if(*path)
				++path;
		}
		mover(file, cp, strlen(file) + 1);
		if(0 < (fd = open(namBuf)))
			return(fd);
	} while(path && *path);
	sync();
	printf("\n\7Place %s online,\n", file);
	putstr("Hit any key to continue, <Esc> aborts... ");
	ch = getkey(YES);
	putchr('\n');
	if(ch == 0x1b)
		doError(myError, "Can't open", file);
	goto loop;
}

LOCAL VOID doError(myError, str1, str2)
VOID (*myError)();
TEXT *str1, *str2;
{
	EXTERN VOID error();
	
	if(!myError)
		myError = &error;
	(*myError)(str1, str2);
}

INT mustCreate(fname, myError)
TEXT *fname;
VOID (*myError)();
{
	INT fd;
	
	if(0 >= (fd = create(fname)))
		doError(myError, "Can't create", fname);
	return(fd);
}

#define EOFERR	0x4c	/* ioresult of EOF read */

INT mustRead(fd, buf, len, myError, fname)
INT fd;
TEXT *buf;
INT len;
VOID (*myError)();
TEXT *fname;
{
	EXTERN BYTE _ioresult;
	
	len = read(fd, buf, len);
	if(_ioresult && _ioresult != EOFERR)
		doError(myError, "Error reading", fname);
	return(len);
}

INT mustWrite(fd, buf, len, myError, fname)
INT fd;
TEXT *buf;
INT len;
VOID (*myError)();
TEXT *fname;
{
	EXTERN BYTE _ioresult;
	
	len = write(fd, buf, len);
	if(_ioresult)
		doError(myError, "Error writing", fname);
	return(len);
}

INT mustGetl(fd, buf, maxlen, myError, fname)
INT fd;
TEXT *buf;
INT maxlen;
VOID (*myError)();
TEXT *fname;
{
	INT len;
	EXTERN BYTE _ioresult;

	len = getl(fd, buf, maxlen);
	if(_ioresult && _ioresult != EOFERR)
		doError(myError, "Error reading", fname);
	return(len);
}

/* --- end of mustOpen.c --- */

