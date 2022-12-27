/*
 * Mach Operating System
 * Copyright (c) 1991,1990,1989,1988,1987,1986 Carnegie Mellon University
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
 * $Log:	mach_traps.cs,v $
 * Revision 2.4  91/05/14  17:53:36  mrt
 * 	Correcting copyright
 * 
 * Revision 2.3  91/02/14  14:17:58  mrt
 * 	Added new Mach copyright
 * 	[91/02/13  12:45:30  mrt]
 * 
 * Revision 2.2  89/08/04  15:08:51  rwd
 * 	Created.  All the work gets done in "syscall_sw.h" now.
 * 	[86/09/01            mwyoung]
 * 
 * Revision 2.1  89/08/04  15:08:34  rwd
 * Created.
 * 
 */

#include <mach/syscall_sw.h>
