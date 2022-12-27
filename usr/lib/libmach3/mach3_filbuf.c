/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * File: $Source: /afs/cs.cmu.edu/project/mach-2/rcs/lib/mach3/mach3/mach3_filbuf.c,v $
 *
 * Purpose: Output functions for libc stdio, suitable for a
 *	standalone environment.
 *
 * HISTORY
 * $Log:	mach3_filbuf.c,v $
 * Revision 2.4  90/10/29  17:27:41  dpj
 * 	Merged-up to U25
 * 	[90/09/02  20:00:45  dpj]
 * 
 * 	Added checks for write-only and string-based streams, which
 * 	should always return EOF.
 * 	[90/08/02  10:20:51  dpj]
 * 
 * Revision 2.3  90/08/22  18:09:43  roy
 * 	Prevent reading beyond eof.
 * 	[90/08/14  12:32:13  roy]
 * 
 * Revision 2.2  90/07/26  12:37:17  dpj
 * 	First version
 * 	[90/07/24  14:28:20  dpj]
 * 
 */

#ifndef lint
char * mach3_filbuf_rcsid = "$Header: /afs/cs.cmu.edu/project/mach-2/rcs/lib/mach3/mach3/mach3_filbuf.c,v 2.4 90/10/29 17:27:41 dpj Exp $";
#endif	lint

#if	MACH3_US || MACH3_SA

#include	<stdio.h>

_filbuf(iop)
	FILE	*iop;
{
	if (iop->_flag & _IORW)
		iop->_flag |= _IOREAD;

	if ((iop->_flag&_IOREAD) == 0)
		return(EOF);
	if (iop->_flag&(_IOSTRG|_IOEOF))
		return(EOF);

	fprintf(stderr,"Error: attempting to read on file descriptor %d\n",
								fileno(iop));
	return(EOF);
}

#endif	MACH3_US || MACH3_SA
