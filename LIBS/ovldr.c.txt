/*
 * ovldr.cc -- overlay loader module...
 */
 
#include <std.h>

#define JMPTBL		struct jmptbl
JMPTBL
{
	UBYTE	xseg[4];	/* first 3 bytes exec code, 4th byte is seg# */
	UBYTE	xaddr[2];	/* addr of exec routine */
};

#define SEGTBL		struct segtbl
SEGTBL
{
	INT		*ldaddr;	/* load address of segment */
	FPOS	fpos;		/* file position of code segment in file */
};

/*
 * _OVLDRX -- the main guts of the overlay loader...
 * Called by the low-level routine _OVLDR with a pointer to the
 * current segment information...
 */
INT *_OVLDRX(jp)
JMPTBL *jp;
{
	EXTERN SEGTBL _OVSEGTBL[];
	EXTERN JMPTBL _OVJMPTBL[];
    EXTERN VOID _OVLDR();
    EXTERN UBYTE _sysfd;
    SEGTBL *sp;
	UBYTE  seg;
	UBYTE  seg2;
	INT    *ldaddr;
	JMPTBL *jp2;
	
	seg = jp->xseg[3];
	sp = &_OVSEGTBL[seg];
	ldaddr = sp->ldaddr;
	for(jp2 = _OVJMPTBL; jp2 < _OVSEGTBL; ++jp2)
	{
		seg2 = jp2->xseg[3];
		if(seg2 == seg)
		{
			/*
			 * mark all routines in this new segment as loaded...
			 */
			jp2->xseg[0] = 0x4c;	/* 6502 jmp instruction */
			jp2->xseg[1] = jp2->xaddr[0];
			jp2->xseg[2] = jp2->xaddr[1];
		}
		else if(_OVSEGTBL[seg2].ldaddr >= ldaddr)
		{
			/*
			 * mark sibling segments and their sub-overlays as unloaded...
			 */
			jp2->xseg[0] = 0x20;	/* 6502 jsr instruction */
            jp2->xseg[1] = (INT)&_OVLDR;
            jp2->xseg[2] = (INT)&_OVLDR >> 8;
		}
	}
    setFPos(_sysfd, sp->fpos);
    ovload(_sysfd, NIL);
	return(jp);			/* return original jmptbl entry address */
}

/* --- end of ovldr.cc --- */

