/*
 * setjmp.cc -- system entry points...
 */

#include <std.h>
#include <syslib.h>

INT longjmp()		opsys(0x19);
INT setjmp()		opsys(0x2b);
	
/* --- end of setjmp.cc --- */

