/*
 * fprintf.cc -- fprintf function...
 *
 * Copyright (c) 1984 by The WSM Group.
 * All rights reserved.
 */
 
#include <std.h>

LOCAL FILE *myfp;

LOCAL VOID myputc(ch)
CHAR ch;
{
	putc(myfp, ch);
}

INT fprintf(fp, fmt, args)
FILE *fp;
TEXT *fmt;
INT  args;
{
	myfp = fp;
	return(xprintf(fmt, &args, &myputc));
}

