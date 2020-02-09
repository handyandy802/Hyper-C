/*
 * sysops.cc -- system entry points...
 */

#include <std.h>
#include <syslib.h>

INT _chkAbort()		opsys(0x01);
INT clrkbd()		opsys(0x09);
INT conRead()		opsys(0x0a);
INT conWrite()		opsys(0x0b);
INT getcurs()		opsys(0x13);
INT keypress()		opsys(0x17);
INT getkey()		opsys(0x14);
INT kbdRead()		opsys(0x16);
INT printf()		opsys(0x1d);
INT putchr()		opsys(0x20);
INT putstr()		opsys(0x23);
INT setcurs()		opsys(0x2a);

/* --- end of stdio.cc --- */

