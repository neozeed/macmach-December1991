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
 * $Log:	mig_support.c,v $
 * Revision 2.4  91/05/14  17:53:52  mrt
 * 	Correcting copyright
 * 
 * Revision 2.3  91/02/14  14:18:05  mrt
 * 	Added new Mach copyright
 * 	[91/02/13  12:44:38  mrt]
 * 
 * Revision 2.2  90/06/02  15:12:42  rpd
 * 	Converted to new IPC.
 * 	[90/03/26  23:27:00  rpd]
 * 
 * Revision 2.1  89/08/03  17:05:48  rwd
 * Created.
 * 
 * 31-Aug-88  David Golub (dbg) at Carnegie-Mellon University
 *	Standalone version - no stdio assumed.
 *
 *
 *  12-May-88	Mary Thompson (mrt) at Carnegie Mellon
 *	included mach_error.h to remove lint
 *  30-Jul-87	Mary Thompson (mrt) at Carnegie Mellon
 *	started
 */
/*
 *  Abstract:
 *	Routines to set and deallocate the mig reply port.
 *	They are called from mig generated interfaces.
 *
 */

#include <mach/mach.h>

static mach_port_t	mig_reply_port = MACH_PORT_NULL;

/*****************************************************
 *  Called by mach_init. This is necessary after
 *  a fork to get rid of bogus port number.
 ****************************************************/

void mig_init(first)
    int	first;
{
	if (first == 0)
		mig_reply_port = MACH_PORT_NULL;
}

/********************************************************
 *  Called by mig interfaces whenever they  need a reply port.
 *  Used to provide the same interface as multi-threaded tasks need.
 ********************************************************/

mach_port_t
mig_get_reply_port()
{
	if (mig_reply_port == MACH_PORT_NULL)
		mig_reply_port = mach_reply_port();

	return mig_reply_port;
}

/*************************************************************
 *  Called by mig interfaces after a timeout on the port.
 *  Could be called by user.
 ***********************************************************/

void
mig_dealloc_reply_port()
{
	mach_port_t port;

	port = mig_reply_port;
	mig_reply_port = MACH_PORT_NULL;

	(void) mach_port_mod_refs(mach_task_self(), port,
				  MACH_PORT_RIGHT_RECEIVE, -1);
}
