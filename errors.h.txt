/*
 * errors.h -- _ioresult error codes...
 */
 
#define IOERR	enum ioError
IOERR (NOERR, DRVERR, WRPROT, FMTERR, BADFD, BADFCB, NAMERR,
		DIRFULL, DSKFULL, NOTFND, NOTPROG, MEMERR, USRABORT);

/*
 NOTE: Under ProDOS the errors related to disk & file I/O are not used.
 Rather, the ProDOS error codes are used -- see available ProDOS technical
 literature...
*/

/* --- end of errors.h --- */

