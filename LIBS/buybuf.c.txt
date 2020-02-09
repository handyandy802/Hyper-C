/*
 * buybuf.cc -- buybuf function...
 */

#include <std.h>

TEXT *buybuf(str, len)
TEXT *str;
INT  len;
{
	TEXT *sp;
	
	if(sp = alloc(len))
		mover(str, sp, len);
	return(sp);
}

