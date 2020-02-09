/*
 * inset -- test chr for membership in the null terminated string
 * of characters chrset.  Return YES or NO.
 * If the null character '\0' is to be among the set,
 * then it must be first...
 *
 * Copyright (c) 1984 by The WSM Group.
 * All rights reserved.
 */

#include <std.h>

BOOL inset(chr, chrset)
CHAR chr;
TEXT *chrset;
{
	do {
		if(*chrset == chr)
			return(YES);
	} while(*++chrset);
	return(NO);
}

