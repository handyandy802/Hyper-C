/*
 * files.c -- system entry points...
 */

#include <std.h>
#include <syslib.h>

INT create()		opsys(0x0c);
INT getFPos()		opsys(0x11);
INT getc()			opsys(0x12);
INT getl()			opsys(0x15);
INT open()			opsys(0x1c);
INT putc()			opsys(0x1f);
INT puts()			opsys(0x22);
INT read()			opsys(0x24);
INT seek()			opsys(0x28);
INT setFPos()		opsys(0x29);
INT write()			opsys(0x2e);
INT close()			opsys(0x34);
INT _setPath()		opsys(0x10);
INT _setPrefix()	opsys(0x35);
INT getEOF()		opsys(0x36);
INT setEOF()		opsys(0x37);

/* --- end of files.c --- */

