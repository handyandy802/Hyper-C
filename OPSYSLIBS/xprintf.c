/*
 * xprintf.cc -- generalized formatted output routine...
 *
 * WARNING: This module is very compiler dependent.
 * 			It has been coded for the GRM 'C' compiler.
 *
 *			These routines depend heavily on the fact that all
 *			stacked function parameters are 1 word in width.
 *
 * Copyright (c) 1984 by The WSM Group.
 * All rights reserved.
 */
 
#include <std.h>

#define SIGNED		1
#define UNSIGNED	0

INT xprintf(fmt, args, pfn)
TEXT *fmt;
INT  *args;
VOID (*pfn)();
{
	TEXT  *sptr;
	TEXT  cbuf[32];
	BOOL  ljust;
	INT	  width;
	INT	  dpl;
	INT	  len;
	INT	  nchrs;
	CHAR  filler;
	CHAR  fctl;
		
	LOCAL INT convert()
	{
		INT	val;
	
		val = 0;
		if(*fmt == '*')
		{
			++fmt;
			val = *args++;
		}
		else
			while(isdigit(*fmt))
				val = 10 * val + *fmt++ - '0';
		return(val);
	}

	LOCAL VOID itoa(base, signed)
	INT 	base;
	BOOL	signed;
	{
		CHAR	*cp;
		INT     val;
		CHAR	*cp2;
		TEXT	dbuf[32];
	
		val = *args;
		cp = &dbuf[0];
		cp2 = &cbuf[0];
		if(!signed)
		{
			do {
				*cp++ = (unsigned)val % base + '0';
                *(unsigned *)&val /= base;
				if(--dpl == 0)
					*cp++ = '.';
			} while(val);
		}
		else
		{
			if(val < 0)
				*cp2++ = '-';
			else
				val = -(val);
			do {
				*cp++ = '0' - val % base;
				val /= base;
				if(--dpl == 0)
					*cp++ = '.';
			} while(val);
		}
		while(cp-- > &dbuf[0]) 
			*cp2++ = (*cp > '9') ? (*cp + 'a' - '9' - 1) : *cp;
		*cp2 = '\0';
	}

	nchrs = 0;
	while(*fmt)
	{
		if(*fmt != '%')
		{
			++nchrs;
			(*pfn)(*fmt++);
		}
		else
		{
			if(ljust = (*++fmt == '-'))
				++fmt;
			if(*fmt == '0')
				filler = *fmt++;
			else
				filler = ' ';
			width = convert(&fmt, &args);
			dpl = (*fmt == '.') ? (++fmt, convert()) : 0;
			sptr = &cbuf;
			switch(fctl = *fmt++)
			{
				case 'd':
				case 'D':
					itoa(10, SIGNED);
					break;

				case 'u':
				case 'U':
					itoa(10, UNSIGNED);
					break;

				case 'x':
				case 'X':
					itoa(16, UNSIGNED);
					break;

				case 'o':
				case 'O':
					itoa(8, UNSIGNED);
					break;

				case 'c':
				case 'C':
					cbuf[0] = *args;
					cbuf[1] = '\0';
					break;

				case 's':
				case 'S':
					sptr = *args;
					break;

				default:
					cbuf[0] = fctl;
					cbuf[1] = '\0';
					break;
			}
			++args;
			len = sptr ? strlen(sptr) : 0;
			if((fctl == 's' || fctl == 'S') && dpl)
				len = min(dpl, len);
			if(!ljust)
			{
				while(width-- > len)
				{
					++nchrs;
					(*pfn)(filler);
				}
			}
			while(len--)
			{
				--width;
				++nchrs;
				(*pfn)(*sptr++);
			}
			while(width-- > 0)
			{
				++nchrs;
				(*pfn)(' ');
			}
		}
	}
	return(nchrs);
}
	
/* --- end of xprintf.cc --- */

