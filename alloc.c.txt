/*
 * alloc.c -- nalloc, alloc, free
 *
 * Usage:
 *
 *		int nbytes;
 *		char *p;
 *
 *		p = nalloc(nbytes, link);
 *		p = alloc(nbytes, link);
 *		free(p);
 *
 * Returns:
 *	p = address of array if available.
 *
 * Bugs:
 *	don't attempt to free bogus memory regions.
 *
 * Copyright (c) 1984 by The WSM Group.
 * All rights reserved.
 */

#include <std.h>
#include <errors.h>

#define BRKSIZ		512		/* sbreak request size */
#define STKSZ		1024	/* reserve at least 1024 bytes from stack */

#define HEADER struct header
HEADER
{
	UWORD	size;
	HEADER	*ptr;
};

LOCAL HEADER base = {0, &base};
EXTERN CHAR *_heapBase;
EXTERN CHAR *_heapTop;

VOID initMem(mem)
CHAR *mem;
{
	_heapTop = _heapBase = mem;
	base.ptr = &base;
}
	
CHAR *sbreak(size)
UWORD size;
{
	CHAR *p;
		
	size += (size & 1);
	p = _heapTop;
	_heapTop = (INT)p + size;
	if(_heapTop > (INT)&p - STKSZ || _heapTop < _heapBase)
	{
		_heapTop = p;
		return(NIL);
	}
	return(p);
}

CHAR *nalloc(nbytes, link)
UWORD nbytes;
INT   link;
{
	HEADER *p, *q;
	UWORD brksiz;

	nbytes += (nbytes & 1) + sizeof(UWORD);
loop:
	for(q = &base, p = q->ptr; p != &base; q = p, p = p->ptr)
		if(p->size >= nbytes)
		{
			if(p->size - nbytes < sizeof(HEADER))
				q->ptr = p->ptr;
			else
			{
				q->ptr = (INT)p + nbytes;
				q->ptr->size = p->size - nbytes;
				q->ptr->ptr = p->ptr;
				p->size = nbytes;
			}
			*(INT *)((INT)p + sizeof(UWORD)) = link;
			return((INT)p + sizeof(UWORD));
		}
	brksiz = ~(BRKSIZ-1) & (nbytes + BRKSIZ - 1);
	if(!(p = sbreak(brksiz)))
		return(NIL);
	p->size = brksiz;
	free((INT)p+sizeof(UWORD));
	goto loop;
}

VOID free(cp)
CHAR *cp;
{
	HEADER *p, *q;

	if(!cp)
		return;
	p = cp - sizeof(UWORD);	/* point to size */
	for(q = &base; !(p > q && p < q->ptr); q = q->ptr)
		if(q >= q->ptr && (p > q || p < q->ptr))
			break;	/* at one end or other */

	if((INT)p+p->size == q->ptr)
	{	/* join to upper nbr */
		p->size += q->ptr->size;
		p->ptr = q->ptr->ptr;
	}
	else
		p->ptr = q->ptr;
	if((INT)q + q->size == p)
	{	/* join to lower nbr */
		q->size += p->size;
		q->ptr = p->ptr;
	}
	else
		q->ptr = p;
}

CHAR *alloc(size, link)
INT size;
INT link;
{
	CHAR *cp;

	if(!(cp = nalloc(size, link)))
		dkError(MEMERR, NIL);
	return(cp);
}

/* --- end of alloc.c --- */

