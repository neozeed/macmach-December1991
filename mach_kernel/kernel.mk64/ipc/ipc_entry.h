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
 * $Log:	ipc_entry.h,v $
 * Revision 2.5  91/05/14  16:31:54  mrt
 * 	Correcting copyright
 * 
 * Revision 2.4  91/02/05  17:21:24  mrt
 * 	Changed to new Mach copyright
 * 	[91/02/01  15:44:29  mrt]
 * 
 * Revision 2.3  91/01/08  15:13:06  rpd
 * 	Removed MACH_IPC_GENNOS, IE_BITS_UNUSEDC, IE_BITS_UNUSEDG.
 * 	[90/11/08            rpd]
 * 
 * Revision 2.2  90/06/02  14:49:41  rpd
 * 	Created for new IPC.
 * 	[90/03/26  20:54:40  rpd]
 * 
 */
/*
 *	File:	ipc/ipc_entry.h
 *	Author:	Rich Draves
 *	Date:	1989
 *
 *	Definitions for translation entries, which represent
 *	tasks' capabilities for ports and port sets.
 */

#ifndef	_IPC_IPC_ENTRY_H_
#define _IPC_IPC_ENTRY_H_

#include <mach/port.h>
#include <mach/kern_return.h>
#include <kern/zalloc.h>
#include <ipc/port.h>
#include <ipc/ipc_table.h>
#include <ipc/ipc_port.h>

/*
 *	Spaces hold capabilities for ipc_object_t's (ports and port sets).
 *	Each ipc_entry_t records a capability.  Most capabilities have
 *	small names, and the entries are elements of a table.
 *	Capabilities can have large names, and a splay tree holds
 *	those entries.  The cutoff point between the table and the tree
 *	is adjusted dynamically to minimize memory consumption.
 *
 *	The ie_index field of entries in the table implements
 *	a ordered hash table with open addressing and linear probing.
 *	This hash table converts (space, object) -> name.
 *	It is used independently of the other fields.
 *
 *	Free (unallocated) entries in the table have null ie_object
 *	fields.  The ie_bits field is zero except for IE_BITS_GEN.
 *	The ie_next (ie_request) field links free entries into a free list.
 *
 *	The first entry in the table (index 0) is always free.
 *	It is used as the head of the free list.
 */

typedef unsigned int ipc_entry_bits_t;
typedef ipc_table_elems_t ipc_entry_num_t;	/* number of entries */

typedef struct ipc_entry {
	ipc_entry_bits_t ie_bits;
	struct ipc_object *ie_object;
	union {
		mach_port_index_t next;
		ipc_port_request_index_t request;
	} index;
	union {
		mach_port_index_t table;
		struct ipc_tree_entry *tree;
	} hash;
} *ipc_entry_t;

#define	IE_NULL		((ipc_entry_t) 0)

#define	ie_request	index.request
#define	ie_next		index.next
#define	ie_index	hash.table

#define	IE_BITS_UREFS_MASK	0x0000ffff	/* 16 bits of user-reference */
#define	IE_BITS_UREFS(bits)	((bits) & IE_BITS_UREFS_MASK)

#define	IE_BITS_TYPE_MASK	0x001f0000	/* 5 bits of capability type */
#define	IE_BITS_TYPE(bits)	((bits) & IE_BITS_TYPE_MASK)

#define	IE_BITS_MAREQUEST	0x00200000	/* 1 bit for msg-accepted */

#define	IE_BITS_COMPAT		0x00400000	/* 1 bit for compatibility */

#define	IE_BITS_COLLISION	0x00800000	/* 1 bit for collisions */
#define	IE_BITS_GEN_MASK	0xff000000	/* 8 bits for generation */
#define	IE_BITS_GEN(bits)	((bits) & IE_BITS_GEN_MASK)
#define	IE_BITS_GEN_ONE		0x01000000	/* low bit of generation */
#define	IE_BITS_RIGHT_MASK	0x007fffff	/* relevant to the right */


typedef struct ipc_tree_entry {
	struct ipc_entry ite_entry;
	mach_port_t ite_name;
	struct ipc_space *ite_space;
	struct ipc_tree_entry *ite_lchild;
	struct ipc_tree_entry *ite_rchild;
} *ipc_tree_entry_t;

#define	ITE_NULL	((ipc_tree_entry_t) 0)

#define	ite_bits	ite_entry.ie_bits
#define	ite_object	ite_entry.ie_object
#define	ite_request	ite_entry.ie_request
#define	ite_next	ite_entry.hash.tree

extern zone_t ipc_tree_entry_zone;

#define ite_alloc()	((ipc_tree_entry_t) zalloc(ipc_tree_entry_zone))
#define	ite_free(ite)	zfree(ipc_tree_entry_zone, (vm_offset_t) (ite))


extern ipc_entry_t
ipc_entry_lookup(/* ipc_space_t space, mach_port_t name */);

extern kern_return_t
ipc_entry_get(/* ipc_space_t space,
		 mach_port_t *namep, ipc_entry_t *entryp */);

extern kern_return_t
ipc_entry_alloc(/* ipc_space_t space,
		   mach_port_t *namep, ipc_entry_t *entryp */);

extern kern_return_t
ipc_entry_alloc_name(/* ipc_space_t space, mach_port_t name,
			ipc_entry_t *entryp */);

extern void
ipc_entry_dealloc(/* ipc_space_t space, mach_port_t name,
		     ipc_entry_t entry */);

extern kern_return_t
ipc_entry_grow_table(/* ipc_space_t space */);

#endif	_IPC_IPC_ENTRY_H_
