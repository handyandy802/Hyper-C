/*
 * exec.c -- system entry points...
 */

#include <std.h>
#include <syslib.h>

INT exec()			opsys(0x0d);
INT exit()			opsys(0x0e);
INT _quit()			opsys(0x33);
INT load()			opsys(0x18);
INT ovload()		opsys(0x31);
	
/* --- end of exec.c --- */

