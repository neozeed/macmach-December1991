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
 * $Log:	slots.h,v $
 * Revision 2.2  91/09/12  16:44:00  bohman
 * 	Created.
 * 	[91/09/11  15:04:27  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/slots.h
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <mac2/act.h>

#include <mac2os/Retrace.h>

/*
 * slot manager defines.
 */

typedef struct slotdata {
    VBLTaskPtr		STaskPtr;
    struct actlist	SActList;
    int			SFlags;
#define SLOT_MAPPABLE	0x01
} slot_t, *slot_ptr_t;

/*
 * Slot number range
 */
#define	SLOT_NUM_LOW	0x9
#define SLOT_NUM_HIGH	0xe

/*
 * Perform range check on
 * a slot number
 */
#define slot_in_range(slot)	\
    ((slot) >= SLOT_NUM_LOW && (slot) <= SLOT_NUM_HIGH)

/*
 * Convert a slot number into a
 * table index
 */
#define slot_to_index(slot)	((slot) - SLOT_NUM_LOW)

/*
 * Convert a slot number into a slot
 * base address (slot space)
 */
#define slot_to_ptr(slot)	\
    ((vm_offset_t)(0xf0000000 | ((slot) << 24)))

/*
 * Convert a (pointer to a slot) to (an offset
 * within the slot)
 */
#define slot_ptr_to_offset(p)	\
    ((vm_offset_t)(p) & 0x00ffffff)

/*
 * Convert (an offset within a slot) to
 * (a pointer to the slot)
 */
#define slot_offset_to_ptr(slot, offset)	\
    (slot_to_ptr(slot) | ((vm_offset_t)(offset) & 0x00ffffff)

/*
 * Convert a slot number into a slot
 * base address (super slot space)
 */
#define slot_to_super_ptr(slot)	\
    ((vm_offset_t)((slot) << 28))

/*
 * Convert a slot number into a
 * slotdata ptr
 */
#define slot_to_slotdata_ptr(slot)	(&slots[slot_to_index(slot)])

extern struct slotdata slots[];
