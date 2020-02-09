/*
 * sysops.cc -- system entry points...
 */

#include <std.h>
#include <syslib.h>

INT clrbuf()		opsys(0x08);
INT mover()			opsys(0x1a);
INT strlen()		opsys(0x2c);

/* --- end of strings.cc --- */

