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
 * $Log:	exit.c,v $
 * Revision 2.4  91/05/14  17:52:00  mrt
 * 	Correcting copyright
 * 
 * Revision 2.3  91/02/14  14:16:59  mrt
 * 	Added new Mach copyright
 * 	[91/02/13  12:43:58  mrt]
 * 
 * Revision 2.2  90/11/05  14:35:34  rpd
 * 	Created.
 * 	[90/10/30            rpd]
 * 
 */

#include <mach/mach.h>

void _exit(code)
    int code;
{
    for (;;) {
	/* shouldn't return, but just in case... */
	(void) task_terminate(mach_task_self());
    }
}

void exit(code)
    int code;
{
    _exit(code);
}
