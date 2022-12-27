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
 * $Log:	call_frame.h,v $
 * Revision 2.2  91/09/12  16:39:00  bohman
 * 	Created Mach 3.0 version from 2.5 version.
 * 	[91/09/11  14:22:53  bohman]
 * 
 * Revision 2.2  90/08/30  11:00:50  bohman
 * 	Created.
 * 	[90/08/29  11:29:55  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/call_frame.h
 *	Author: David E. Bohman II (CMU macmach)
 */

#ifndef	_MACHINE_CALL_FRAME_H_
#define _MACHINE_CALL_FRAME_H_

/*
 * C function call frame
 * definition (actually compiler dependent)
 */
struct call_frame {
    struct call_frame *f_fp;
    unsigned long f_pc;
    unsigned long f_param[1];
};

#endif	_MACHINE_CALL_FRAME_H_
