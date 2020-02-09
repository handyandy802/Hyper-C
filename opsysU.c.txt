/*
 * opsysu.cc -- 6502 operating system support...
 */

#include <std.h>

/*----------------------------------------------------------------------
 * Device Routines...
 */

INT conWrite(buf, len)
TEXT *buf;
INT    len;
{
	INT actlen;
	
	actlen = 0;
	while(actlen < len)
	{
		putchr(*buf++);
		++actlen;
	}
	return(actlen);
}

INT prtWrite(buf, len)
TEXT *buf;
INT    len;
{
	INT actlen;
	
	actlen = 0;
	while(actlen < len)
	{
		putprt(*buf++);
		++actlen;
	}
	return(actlen);
}

INT conRead(buf, len)
TEXT *buf;
INT len;
{
	return(conkbdRead(YES, buf, len));
}

INT kbdRead(buf, len)
TEXT *buf;
INT len;
{
	return(conkbdRead(NO, buf, len));
}

LOCAL INT conkbdRead(echo, buf, len)
BOOL echo;
TEXT *buf;
INT len;
{
	INT actlen;
	INT conpos;
	LOCAL TEXT BackStr[] = "\b \b";
	LOCAL BOOL prtState = NO;
	CHAR ch;
	
	actlen = 0;
	len -= 2;
	for(;;)
	{
		switch(ch = getkey(NO))
		{
			case 0:
				_usrAbort();
				
			case '\r':
				++actlen;
				*buf++ = '\r';
				*buf = 0;
				if(echo)
					cr();
				return(actlen);
				
			case 0x7f:
			case '\b':
				if(actlen)
				{
					--actlen;
					--buf;
					if(echo)
						putstr(BackStr);
				}
				break;
			
			case 0x18:	/* ^X */
				while(actlen > 0)
				{
					bs();
					--actlen;
					--buf;
				}
				clreol();
				break;
				
			case 0x10:	/* ^P */
				if(prtState)
				{
					_vidonly();
					prtState = NO;
				}
				else
				{
					_prtvid();
					prtState = YES;
				}
				break;
				
			case '\t':
				ch = ' ';
			default:
				if(actlen < len && ch >= ' ')
				{
					++actlen;
					*buf++ = ch;
					if(echo)
						putchr(ch);
				}
				break;
		}
	}
}

/* --- end of opsysu.cc --- */

