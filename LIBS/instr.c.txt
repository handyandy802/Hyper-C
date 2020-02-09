/*
 * instr -- scan the null terminated string str for a character in
 * the null terminated character string chrset.  Return index of
 * match, or the index of the terminating null in str.
 *
 * Copyright (c) 1984 by The WSM Group.
 * All rights reserved.
 */

#include <std.h>

INT instr(str, chrset)
TEXT *str;
TEXT *chrset;
{
	INT index = 0;
	CHAR ch;
		
	while((ch = *str++) && !inset(ch, chrset))
		++index;
	return(index);
}

