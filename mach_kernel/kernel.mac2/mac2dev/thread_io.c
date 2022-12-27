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
 * $Log:	thread_io.c,v $
 * Revision 2.2  91/09/12  16:48:08  bohman
 * 	Created.
 * 	[91/09/11  16:06:16  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2dev/thread_io.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <mach/mach_types.h>

#include <kern/sched_prim.h>

#include <mac2os/Types.h>
#include <mac2os/Errors.h>
#include <mac2os/Files.h>

extern void	thread_io_done();

thread_io(func, pb)
register OSErr		(*func)();
register ParmBlkPtr	pb;
{

    pb->ioParam.ioCompletion = thread_io_done;
    (void) (*func)(pb, TRUE);

    while (pb->ioParam.ioResult > 0) {
	assert_wait(&pb->ioParam.ioResult, FALSE);
	thread_block((void (*)()) 0);
    }

    return (pb->ioParam.ioResult);
}

void
thread_io_complete(pb)
register ParmBlkPtr	pb;
{
    thread_wakeup_one(&pb->ioParam.ioResult);
}
