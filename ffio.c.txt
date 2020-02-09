/*
 * create.c -- file create...
 */

#include <sys.h>

EXTERN TEXT _filePath[];
EXTERN TEXT _newPath[];
LOCAL BYTE _fileLevel @0xbf94;
	
VOID _setPath(fname)
{
	__setPath(fname, &_filePath);
}

LOCAL VOID __setPath(fname, buf)
TEXT *fname;
TEXT *buf;
{
	buf[0] = strlen(fname);
	mover(fname, &buf[1], buf[0]);
}

INT create(fname)
TEXT *fname;
{
	__setPath(fname, &_filePath);
	_destroy();
	_create();
	return(_open());
}

INT open(fname)
TEXT *fname;
{
	__setPath(fname, &_filePath);
	return(_open());
}

VOID remove(fname)
TEXT *fname;
{
	__setPath(fname, &_filePath);
	_destroy();
}

VOID rename(oldname, newname)
TEXT *oldname, *newname;
{
	__setPath(oldname, &_filePath);
	__setPath(newname, &_newPath);
	_rename();
}
	
VOID sync()
{
	INT oldLev;
	
	oldLev = _fileLevel;
	_fileLevel = 0;
	_flush();
	_fileLevel = oldLev;
}

INT getc(fd)
INT fd;
{
	CHAR ch;
	
	if(1 != read(fd, &ch, 1))	
		return(-1);
	return(ch);
}

VOID putc(fd, ch)
INT fd;
CHAR ch;
{
	CHAR ch2;
	
	ch2 = ch;
	write(fd, &ch2, 1);
}

VOID puts(fd, str)
INT fd;
TEXT *str;
{
	INT len;
	
	len = strlen(str);
	write(fd, str, len);
}

VOID seek(fd, posh, posl, mode)
INT fd;
INT posh, posl;
INT mode;
{
	INT fpos[2];
	
	switch(mode)
	{
		case 0:
			setFPos(fd, &posh);
			break;
		
		case 1:
			getFPos(fd, &fpos);
			_ladd(&fpos, &posh);
			setFPos(fd, &fpos);
			break;
			
		case 2:
			getEOF(fd, &fpos);
			_ladd(&fpos, &posh);
			setFPos(fd, &fpos);
			break;
	}
}

/* --- end of ffio.c --- */

