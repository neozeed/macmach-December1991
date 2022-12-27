/* 
 * Mach Operating System
 * Copyright (c) 1991,1990 Carnegie Mellon University
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 * 
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS 
 * CONDITION.  CARNEGIE MELLON DISCLAIMS ANY LIABILITY OF ANY KIND FOR
 * ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
 * 
 * Carnegie Mellon requests users of this software to return to
 * 
 *  Software Distribution Coordinator  or  Software.Distribution@CS.CMU.EDU
 *  School of Computer Science
 *  Carnegie Mellon University
 *  Pittsburgh PA 15213-3890
 * 
 * any improvements or extensions that they make and grant Carnegie the
 * rights to redistribute these changes.
 */
/*
 * HISTORY
 * $Log:	xstrip.c,v $
 * Revision 2.4  91/08/29  16:54:32  jsb
 * 	Moved mips functionality to xstrip_mips.c.
 * 	This is now a cover file that includes the right
 * 	file, or provides a failure stub if there is no
 * 	right file (e.g., unsupported machine type).
 * 
 */
/*
 *	File:	xstrip.c
 *	Author:	Joseph S. Barrera III
 *	Date:	1991
 */

#if	mips
#define	XSTRIP_IMPLEMENTED
#include "xstrip_mips.c"
#endif

#if	i386 || vax || sun3 || mac2
#define	XSTRIP_IMPLEMENTED
#include "xstrip_aout.c"
#endif

#ifndef	XSTRIP_IMPLEMENTED
main()
{
	printf("xstrip: not implemented for this architecture\n");
	exit(0);
}
#endif
