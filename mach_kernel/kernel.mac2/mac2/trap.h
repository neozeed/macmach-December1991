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
 * $Log:	trap.h,v $
 * Revision 2.2  91/09/12  16:45:00  bohman
 * 	Created Mach 3.0 version from 2.5 version.
 * 	[91/09/11  15:12:15  bohman]
 * 
 * Revision 2.2  90/08/30  11:03:32  bohman
 * 	Created.
 * 	[90/08/29  12:00:35  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/trap.h
 *	Author: David E. Bohman II (CMU macmach)
 */

#ifndef _MAC2_TRAP_H_
#define _MAC2_TRAP_H_

/*
 * Trap type values
 */
#define TRAP_BAD_ACCESS		0
#define TRAP_TRACE		1
#define TRAP_EMULATION_1010	2
#define TRAP_EMULATION_1111	3
#define TRAP_BAD_INSTRUCTION	4
#define TRAP_PRIV_INSTRUCTION	5
#define TRAP_BREAKPOINT		6
#define TRAP_ARITHMETIC		7
#define TRAP_SOFTWARE		8
#define TRAP_ERROR		9

#endif	_MAC2_TRAP_H_
