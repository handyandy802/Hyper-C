/*
 * getflags.c -- collect flags from command line
 *
 * Copyright (c) 1984 by The WSM Group.
 * All rights reserved.
 */

#include <std.h>

#define ARGA	struct arga
ARGA
{
	INT		ntop;
	TEXT	*val[1];
};

TEXT *getflags(ac, av, fmt, args)
VAR INT ac;
VAR TEXT **av;
TEXT *fmt;
INT *args;
{
	TEXT *cmd;
	INT **argp;
	TEXT *cmd2;
	TEXT *flags;
	CHAR ch;

	++av;
	if(--ac <= 0)
		return(NIL);
	while(ac > 0)
	{
		cmd = *av;
		ch = *cmd++;
		if(ch != '+' && ch != '-')
			return(NIL);
		if(ch == '-')
			ch = *cmd++;
bigloop:
		argp = &args;
		flags = fmt;
		while(*flags++ != ch)
		{
loop:
			flags += instr(flags, "\0:,>");
			if(*flags == '\0')
				return(cmd-1);
			if(*flags++ == ':')
			{
				showuse(flags, fmt);
				exit(ERR);
			}
			++argp;
		}
		cmd2 = cmd;
		while(isalnum(*flags))
		{
			if(*flags == *cmd2)
			{
				++flags;
				++cmd2;
			}
			else
				goto loop;
		}
		cmd = cmd2;
		switch(*flags)
		{

			case ',':
			case ':':
			case '\0':
				*(CHAR *)(*argp) = YES;
				break;

			case '?':
				*(CHAR *)(*argp) = (*cmd == '\\') ? doesc(&cmd) : *cmd;
				++cmd;
				break;

			case '#':
			case '*':
				if(*cmd == '\0')
				{
					--ac;
					++av;
					if(ac)
						cmd = *av;
					else
					{
						flagerr(fmt);
						return(cmd);
					}
				}
				if(*flags == '#')
				{
					if(!convert(cmd, *argp))
					{
						flagerr(fmt);
						return(cmd);
					}
				}
				else
					**argp = (INT)cmd;
				cmd = "";
				break;
		}
		if(ch = *cmd++)
			goto bigloop;
		--ac;
		++av;
	}
	return(NIL);
}

LOCAL BOOL convert(str, val)
TEXT *str;
VAR INT val;
{
	val = atoi(&str);
	return(!*str);
}

LOCAL VOID flagerr(fmt)
TEXT *fmt;
{
	TEXT *str;

	if(str = index(fmt, ':'))
	{
		showuse(str+1, fmt);
		exit(ERR);
	}
}

LOCAL VOID showuse(str, fmt)
TEXT *str;
TEXT *fmt;
{
	EXTERN TEXT *_pname;

	printf("usage: %s ", _pname);
	while(*str)
	{
		if(*str == 'F')
			showflags(fmt);
		else
			putchr(*str);
		++str;
	}
	putchr('\n');
}

LOCAL VOID showflags(fmt)
REG TEXT *fmt;
{
	putstr("-[");
	while(*fmt != ':')
	{
		if(*fmt == ',')
			putchr(' ');
		else
			putchr(*fmt);
		++fmt;
	}
	putchr(']');
}

/* --- end of getflags.c --- */

