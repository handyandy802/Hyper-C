/*
 * hdr.h -- cp-Code file header
 */

#define OBJHDR		struct objhdr
OBJHDR
{
	UWORD	h_magic;	/* 0x100 or 0x200 */
	UWORD	h_symsz;	/* size of symbol table */
	UWORD	h_tloc;		/* text location */
	UWORD	h_text;		/* text size */
	UWORD	h_dloc;		/* data location */
	UWORD	h_data;		/* data size */
	UWORD	h_bloc;		/* bss location */
	UWORD	h_bss;		/* bss size */
	UWORD	h_relsz;	/* size of relocation tables */
	UWORD	h_jtbl;		/* jump table file offset */
};

#define LIBHDR	struct libhdr
LIBHDR
{
	TEXT	h_fname[14];		/* file name null padded */
	UWORD	h_fsize;			/* file size */
};

/* --- end of hdr.h --- */

