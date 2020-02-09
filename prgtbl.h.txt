/*
 * prgtbl.h -- header for resident program table...
 */
 
#define PRGTBL	struct prgtbl
PRGTBL
{
	TEXT	*name;
	VOID	(*prog)();
};

