/*
 * doesc -- encode the sequence of characters beginning at *tp, on the
 * assumption that (*tp)[0] is an escape character.  Update the pointer
 * at tp to point to the last character of the (variable length) escape
 * sequence.
 *
 * If (*tp)[1] is null, then the code value is (*tp)[0], i.e., the
 * escape character itself; this is the only escape sequence of length one.
 * If (*tp)[1] is a character in the set "bfnrte" in either case, the code
 * is the corresponding member of the set (backspace, formfeed, newline,
 * return, tab, escape).
 * If (*tp)[1] is a digit, then up to three digits are taken as the
 * octal value of the escape sequence.
 *
 * Copyright (c) 1984 by The WSM Group.
 * All rights reserved.
 */

#include <std.h>

CHAR doesc(tp)
VAR TEXT *tp;
{
	CHAR ch;
	UBYTE val;
		
	if(tp[1] == '\0')
		return(*tp);
	ch = *++tp;
	if(!isdigit(ch))
	{
		switch(ch)
		{
			case 'f':
				ch = 0x0c;
				break;

			case 'n':
				ch = 0x0a;
				break;

			case 'r':
				ch = 0x0d;
				break;

			case 'b':
				ch = 0x08;
				break;

			case 't':
				ch = 0x09;
				break;

			case 'e':
				ch = 0x1b;
				break;
		}				
		return(ch);
	}
	if(ch > '7')
		return(ch);
	val = ch - '0';
	ch = tp[1];
	if(isdigit(ch) && ch < '8')
	{
		val = 8 * val + ch - '0';
		++tp;
		ch = tp[1];
		if(isdigit(ch) && ch < '8')
		{
			++tp;
			val = 8 * val + ch - '0';
		}
	}
	return(val);
}

