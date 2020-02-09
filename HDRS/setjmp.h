/*
 * setjmp.h -- jmpvect structure for setjmp/longjmp...
 */
 
#define JMPVECT		struct jmpvect
JMPVECT
{
	WORD	dsply[16];
	WORD	*pc;
	WORD	*fp;
	WORD	*sp;
	WORD	rp;
	WORD	syspc;
};

/* --- end of setjmp.h --- */

