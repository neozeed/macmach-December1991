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
 * $Log:	page_info.h,v $
 * Revision 2.4  91/05/14  17:04:01  mrt
 * 	Correcting copyright
 * 
 * Revision 2.3  91/02/05  17:38:10  mrt
 * 	Changed to new Mach copyright
 * 	[91/02/01  17:29:22  mrt]
 * 
 * Revision 2.2  90/05/03  15:48:58  dbg
 * 	Created.
 * 	[90/04/06            dbg]
 * 
 */

#include <mach/machine/vm_types.h>

typedef	vm_offset_t	*page_address_array_t;
