/*
 * printf.cc -- printf function
 *
 * Copyright (c) 1984 by The WSM Group.
 * All rights reserved.
 */

#include <std.h>

INT printf(fmt, args)
TEXT *fmt;
INT  args;
{
	EXTERN VOID putchr();
	
	return(xprintf(fmt, &args, &putchr));
}


