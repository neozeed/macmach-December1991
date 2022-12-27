/* 
 * Mach Operating System
 * Copyright (c) 1991,1990,1989 Carnegie Mellon University
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 * 
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS "AS IS"
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
 * any improvements or extensions that they make and grant Carnegie Mellon
 * the rights to redistribute these changes.
 */


/*
 * HISTORY
 * $Log:	softint.c,v $
 * Revision 2.2  91/09/12  16:44:10  bohman
 * 	Created Mach 3.0 version from 2.5 version.
 * 	[91/09/11  15:05:07  bohman]
 * 
 * Revision 2.2  90/08/30  11:03:03  bohman
 * 	Created.
 * 	[90/08/29  11:45:59  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/softint.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <mach/mach_types.h>

#include <machine/machparam.h>

#include <mac2/act.h>

struct actlist	actsoft;

#define	SOFTACT_SOFT_LIST	0

struct act *
makesoftact(func, ipl)
register int (*func)(), ipl;
{
    register struct act *ap;

    ap = makeact(func, ipl, 1);
    if (ap)
	addact(SOFTACT_SOFT_LIST, ap, &actsoft);

    return (ap);
}

/*
 * Call activity ap with argument arg
 * at some later time as a software interrupt.
 */
void
softact(ap, arg)
register struct act	*ap;
{
    runact(SOFTACT_SOFT_LIST, ap, arg, 0);
}

/*
 * Called from IPL7 to process
 * software interrupts.
 */
void
softint()
{
    if (CHECKACTLIST(actsoft))
	DOACTLIST(actsoft);
}
