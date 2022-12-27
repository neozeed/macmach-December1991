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
 * $Log:	machparam.h,v $
 * Revision 2.2  91/09/12  16:41:50  bohman
 * 	Created Mach 3.0 version from 2.5 version.
 * 	[91/09/11  14:53:31  bohman]
 * 
 * Revision 2.2  90/08/30  18:23:58  bohman
 * 	Created.
 * 
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/machparam.h
 */

#ifndef _MACHINE_MACH_PARAM_H_
#define _MACHINE_MACH_PARAM_H_

#if	defined(KERNEL) && !defined(ASSEMBLER)
#include <machine/cpu_inline.c>
#endif

#include <machine/endian.h>

#endif	_MACHINE_MACH_PARAM_H_
