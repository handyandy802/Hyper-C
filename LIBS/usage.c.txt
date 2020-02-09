/*
 * usage.cc -- print a usage message...
 * If message ends in newline, then abort the program as well...
 */

#include <std.h>

VOID usage(str)
TEXT *str;
{
	EXTERN TEXT *_pname;
	
	printf("usage: %s %s", _pname, str);
	if(str[strlen(str)-1] == '\n')
		exit(ERR);
	else
		putchr('\n');
}

