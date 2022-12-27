/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * File: $Source: /afs/cs.cmu.edu/project/mach-2/rcs/lib/mach3/mach3/mach3_exit.c,v $
 *
 * HISTORY
 * $Log:	mach3_exit.c,v $
 * Revision 2.4  90/11/10  00:38:11  dpj
 * 	Enabled definition of exit() for all architectures under
 * 	MACH3_US and MACH3_SA, to track changes in the pure kernel crt0.c.
 * 	[90/11/08  22:17:11  dpj]
 * 
 * Revision 2.3  90/10/29  17:27:35  dpj
 * 	Added _exit() routine.
 * 	[90/08/13  10:16:32  dpj]
 * 
 * Revision 2.2  90/07/26  12:37:13  dpj
 * 	First version
 * 	[90/07/24  14:28:16  dpj]
 * 
 *
 */

#ifndef lint
char * mach3_exit_rcsid = "$Header: /afs/cs.cmu.edu/project/mach-2/rcs/lib/mach3/mach3/mach3_exit.c,v 2.4 90/11/10 00:38:11 dpj Exp $";
#endif	lint

#if	MACH3_US || MACH3_SA

exit(code)
{
	(void) task_terminate(mach_task_self());
}

_exit(code)
{
	return(exit(code));
}

#endif	MACH3_US || MACH3_SA
