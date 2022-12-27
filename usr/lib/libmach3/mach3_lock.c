/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * File: $Source: /afs/cs.cmu.edu/project/mach-2/rcs/lib/mach3/mach3/mach3_lock.c,v $
 *
 * Purpose: simple spin locks
 *
 * HISTORY
 * $Log:	mach3_lock.c,v $
 * Revision 2.2  90/10/29  17:27:52  dpj
 * 	Created.
 * 	[90/10/27  17:49:02  dpj]
 * 
 * 	Created to replace lock.c, to avoid conflicts with the cthreads
 * 	version of spin locks.
 * 	[90/10/21  21:20:52  dpj]
 * 
 *
 */

#ifndef lint
char * skel_rcsid = "$Header: /afs/cs.cmu.edu/project/mach-2/rcs/lib/mach3/mach3/mach3_lock.c,v 2.2 90/10/29 17:27:52 dpj Exp $";
#endif	lint


/*
 * Note: most of the functions are machine-dependent, and found in
 * ${MACHINE}/lock.s.
 */


void
mach3_spin_lock(p)
	register int *p;
{
	while (*p != 0 || !mach3_spin_try_lock(p))
		;	/* spin */
}



