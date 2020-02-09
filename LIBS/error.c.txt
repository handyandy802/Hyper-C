/*
 * error.cc -- error routine...
 */

#include <std.h>

VOID error(str1, str2)
TEXT *str1;
TEXT *str2;
{
	EXTERN TEXT *_pname;
	
	if(!str2)
		str2 = "";
	printf("\n%s: %s %s\n", _pname, str1, str2);
	exit(ERR);
}

