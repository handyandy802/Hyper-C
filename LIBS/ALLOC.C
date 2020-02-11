/*
 * alloc.cc -- system entry points...
 */

#include <std.h>
#include <syslib.h>

INT alloc()			opsys(0x07);
INT free()			opsys(0x0f);
INT nalloc()		opsys(0x1b);
INT sbreak()		opsys(0x27);

/* --- end of alloc.cc --- */

