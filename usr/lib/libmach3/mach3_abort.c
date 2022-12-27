/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * File: $Source: /afs/cs.cmu.edu/project/mach-2/rcs/lib/mach3/mach3/mach3_abort.c,v $
 *
 * Purpose:
 *
 * HISTORY
 * $Log:	mach3_abort.c,v $
 * Revision 2.3  90/10/29  17:27:28  dpj
 * 	Merged-up to U25
 * 	[90/09/02  20:00:37  dpj]
 * 
 * 	First version.
 * 	[90/08/02  10:19:59  dpj]
 * 
 * Revision 2.2  90/08/22  18:09:12  roy
 * 	No change.
 * 	[90/08/14  17:03:58  roy]
 * 
 * Revision 2.1  90/08/02  10:19:40  dpj
 * Created.
 * 
 */

#ifndef lint
char * mach3_abort_rcsid = "$Header: /afs/cs.cmu.edu/project/mach-2/rcs/lib/mach3/mach3/mach3_abort.c,v 2.3 90/10/29 17:27:28 dpj Exp $";
#endif	lint

#if	MACH3_US || MACH3_SA

#include	<stdio.h>

abort()
{
	fprintf(stderr,"***** Aborting *****\n");
	exit(2001);
}

#endif	MACH3_US || MACH3_SA

