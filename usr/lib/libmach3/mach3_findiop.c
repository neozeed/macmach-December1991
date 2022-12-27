/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * File: $Source: /afs/cs.cmu.edu/project/mach-2/rcs/lib/mach3/mach3/mach3_findiop.c,v $
 *
 * Purpose: Output functions for libc stdio, suitable for a
 *	standalone environment.
 *
 * This file is a replacement for findiop.c, but it is
 * currently reusing some of the original UNIX code:
 *
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 * HISTORY
 * $Log:	mach3_findiop.c,v $
 * Revision 2.4  90/10/29  17:27:45  dpj
 * 	Merged-up to U25
 * 	[90/09/02  20:00:48  dpj]
 * 
 * Revision 2.3  90/08/22  18:10:12  roy
 * 	Allocate static bufs for stdout, stderr.
 * 	[90/08/14  12:31:01  roy]
 * 
 * Revision 2.2  90/07/26  12:37:20  dpj
 * 	First version
 * 	[90/07/24  14:28:26  dpj]
 * 
 */

#ifndef lint
char * mach3_findiop_rcsid = "$Header: /afs/cs.cmu.edu/project/mach-2/rcs/lib/mach3/mach3/mach3_findiop.c,v 2.4 90/10/29 17:27:45 dpj Exp $";
#endif	lint

#if	MACH3_US || MACH3_SA

#include <stdio.h>

#define NSTATIC	20	/* stdin + stdout + stderr + the usual */

char	stdout_buf[BUFSIZ];
char	stderr_buf[BUFSIZ];

FILE _iob[NSTATIC] = {
	{ 0, NULL,       NULL,       0,      _IOREAD,       0 }, /* stdin  */
	{ 0, stdout_buf, stdout_buf, BUFSIZ, _IOWRT|_IOLBF, 1 }, /* stdout */
	{ 0, stderr_buf, stderr_buf, BUFSIZ, _IOWRT|_IOLBF, 2 }, /* stderr */
};

#endif	MACH3_US || MACH3_SA
