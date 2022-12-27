/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	bsd_types_gen.c,v $
 * Revision 2.2  90/05/21  14:01:44  dbg
 * 	Return 0 to shell.
 * 	[90/03/14            dbg]
 * 
 */
/*
 * Generate definitions for Mig interfaces.  MiG can't handle random
 * C definitions or expressions any better than the assembler.
 */

#include <sys/types.h>
#include <sys/param.h>

main()
{
	printf("#define\tPATH_LENGTH %d\n",
			roundup(MAXPATHLEN,sizeof(int)));
	printf("#define\tSMALL_ARRAY_LIMIT %d\n",
			4096);
	printf("#define\tFD_SET_LIMIT %d\n",
			howmany(FD_SETSIZE, NFDBITS));
	printf("#define\tGROUPS_LIMIT %d\n",
			NGROUPS);
	printf("#define\tHOST_NAME_LIMIT %d\n",
			MAXHOSTNAMELEN);

	return (0);
};
