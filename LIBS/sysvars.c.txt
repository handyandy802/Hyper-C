/*
 * sysvars.cc -- system global variables in Page 2...
 */

#include <std.h>

TEXT  _sysline[128] @0xbe00;    /* command line buffer */
TEXT  _cmdline[128] @0xbe80;    /* exec command line buffer */
TEXT  *_argv[32]    @0x300;		/* vector of command args */
TEXT  *_cargv[32]	@0x340;		/* vector of exec command args */
UBYTE _ioresult		@0x380;		/* I/O return code */
BYTE  _sysfd		@0x381;		/* system file descriptor */
UBYTE _prtpos		@0x382;		/* printer line position */
BOOL  _pcrlf		@0x383;		/* nonzero means print linefeeds */
WORD  _exitVal		@0x384;		/* last program's exit value */
TEXT  *_pname		@0x386;		/* name of current program */
VOID  (*_errHdlr)()	@0x388;		/* system error handler */
UBYTE *_heapBase	@0x38a;		/* base addr of current heap */
UBYTE *_heapTop		@0x38c;		/* current top of heap */
WORD  _prtHook		@0x38e;		/* printer hook vector */
BYTE  _cmdfd		@0x390;		/* exec file descriptor */
BYTE  _cargc		@0x391;		/* count of exec commands */
UBYTE _putprt		@0x392;		/* patch prog for printer driver */

UBYTE _xcurs		@0x57b;		/* screen column */
UBYTE _ycurs		@0x5fb;		/* screen row */

/* --- end of sysvars.c --- */

