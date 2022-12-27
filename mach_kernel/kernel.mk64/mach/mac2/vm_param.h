/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	vm_param.h,v $
 * Revision 2.2  91/09/12  16:53:29  bohman
 * 	Created Mach 3.0 version from 2.5 version.
 * 	[91/09/11  17:16:38  bohman]
 * 
 * Revision 2.2  90/08/30  17:52:05  bohman
 * 	Created.
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mach/mac2/vm_param.h
 *	Author: David E. Bohman II (CMU macmach)
 */

#ifndef _MACH_MAC2_VM_PARAM_H_
#define _MACH_MAC2_VM_PARAM_H_

#include <mac2/machparam.h>

#define BYTE_SIZE	8	/* byte size in bits */

#define MAC2_PGBYTES	8192	/* bytes per page */
#define MAC2_PGSHIFT	13	/* number of bits to shift for pages */

/*
 *	Convert bytes to pages and convert pages to bytes.
 *	No rounding is used.
 */

#define	mac2_btop(x)		(((unsigned)(x)) >> MAC2_PGSHIFT)
#define	mac2_ptob(x)		(((unsigned)(x)) << MAC2_PGSHIFT)

/*
 *	Round off or truncate to the nearest page.  These will work
 *	for either addresses or counts.  (i.e. 1 byte rounds to 1 page
 *	bytes.
 */

#define mac2_round_page(x)	((((unsigned)(x)) + MAC2_PGBYTES - 1) & \
					~(MAC2_PGBYTES-1))
#define mac2_trunc_page(x)	(((unsigned)(x)) & ~(MAC2_PGBYTES-1))

/*
 * Macintosh II implementation
 * uses separate user and
 * kernel address spaces.
 *
 */
#define	VM_MIN_ADDRESS	((vm_offset_t) 0)
#define	VM_MAX_ADDRESS	((vm_offset_t) 0xffffffff)

#define VM_MIN_KERNEL_ADDRESS	((vm_offset_t) 0x0)
#define VM_MAX_KERNEL_ADDRESS	((vm_offset_t) 0xffffffff)

#define	KERNEL_STACK_SIZE	MAC2_PGBYTES

#define INTSTACK_SIZE		MAC2_PGBYTES

/*
 *	Conversion between mac2 pages and VM pages
 */

#define trunc_mac2_to_vm(p)	(atop(trunc_page(mac2_ptob(p))))
#define round_mac2_to_vm(p)	(atop(round_page(mac2_ptob(p))))
#define vm_to_mac2(p)		(mac2_btop(ptoa(p)))

#endif	_MACH_MAC2_VM_PARAM_H_
