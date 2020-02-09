/*
 * getArg.c --- get blank separated args from text buffer...
 */

#include <std.h>

INT getArgs(cmd, av, nargs)
TEXT *cmd;
TEXT **av;
INT nargs;
{
	INT i;
	
	for(i = 0; i < nargs-1; ++i)
	{
		av[i] = getArg(&cmd);
		if(!*av[i])
			break;
	}
	av[i] = NIL;
	return(i);
}

TEXT *getArg(sp)
VAR TEXT *sp;
{
	TEXT *wp;
	
	while(*sp && *sp <= ' ')
		++sp;
	if(*sp == '"')
	{
		wp = ++sp;
		while(*sp && *sp != '"')
		{
			if(*sp == '\\')
				++sp;
			if(*sp)
				++sp;
		}
	}
	else
	{
		wp = sp;
		while(*sp > ' ')
			++sp;
	}
	if(*sp)
		*sp++ = '\0';
	return(wp);
}

/* --- end of getArg.c --- */

