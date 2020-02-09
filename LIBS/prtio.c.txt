/*
 * prtio.cc -- system entry points...
 */

#include <std.h>
#include <syslib.h>

INT prtWrite()		opsys(0x1e);
INT _prtinit()		opsys(0x02);
INT putprt()		opsys(0x21);
INT _vidinit()		opsys(0x05);
INT _vidonly()		opsys(0x06);
INT _prtonly()		opsys(0x03);
INT _prtvid()		opsys(0x04);

/* --- end of prtio.cc --- */

