/*
 * sprintf.cc -- sprintf function
 *
 * Copyright (c) 1984 by The WSM Group.
 * All rights reserved.
 */

#include <std.h>

LOCAL TEXT *bufp;

INT sprintf(buf, fmt, args)
TEXT *buf;
TEXT *fmt;
INT  args;
{
	EXTERN VOID _stuff();
	INT	count;

	bufp = buf;
	count = xprintf(fmt, &args, &_stuff);
	*bufp = '\0';
	return(count);
}

LOCAL VOID _stuff(ch)
CHAR ch;
{
	*bufp++ = ch;
}

