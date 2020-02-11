/*
 * atoi.c -- convert string to number...
 * Returns the converted number value, and updates the string pointer
 * to point at first char beyond the number string...
 */

#include <std.h>
 
INT atoi(sptr)
TEXT **sptr;
{
	INT  base;
	INT  val;
	TEXT *cp;
	CHAR ch;
	CHAR sign;
	
	cp = *sptr;
	val = 0;
	while(isspace(*cp))
		++cp;
	if((sign = *cp) == '-' || sign == '+')
		++cp;
	if(*cp == '0')
	{
		++cp;
		if((*cp == 'x') || (*cp == 'X'))
		{	
			base = 16;
			++cp;
		}
		else
			base = 8;
	}
	else
		base = 10;
	while(isdigit(*cp) || ((base == 16) && ishex(*cp)))
	{
		if(*cp >= 'A')
		{
			if(*cp >= 'a')
				ch = *cp - 'a';
			else
				ch = *cp - 'A';
			ch += 10;
		}
		else
			ch = *cp - '0';
		if(ch < base)
		{
			++cp;
			val = val * base + ch;
		}
		else
			break;
	}
	*sptr = cp;
	return((sign == '-') ? -val : val);
}

/* --- end of atoi.c --- */

