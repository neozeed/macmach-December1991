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
 * $Log:	mach_debug.c,v $
 * Revision 2.5  91/05/14  16:38:28  mrt
 * 	Correcting copyright
 * 
 * Revision 2.4  91/02/05  17:24:30  mrt
 * 	Changed to new Mach copyright
 * 	[91/02/01  15:52:50  mrt]
 * 
 * Revision 2.3  91/01/08  15:14:55  rpd
 * 	Changed ipc_info_bucket_t to hash_info_bucket_t.
 * 	[91/01/02            rpd]
 * 	Removed MACH_IPC_GENNOS.
 * 	[90/11/08            rpd]
 * 
 * Revision 2.2  90/06/02  14:52:15  rpd
 * 	Created for new IPC.
 * 	[90/03/26  21:05:22  rpd]
 * 
 */
/*
 *	File:	ipc/mach_debug.c
 *	Author:	Rich Draves
 *	Date:	1989
 *
 *	Exported kernel calls.  See mach_debug/mach_debug.defs.
 */

#include <mach_ipc_compat.h>

#include <mach/kern_return.h>
#include <mach/port.h>
#include <mach/machine/vm_types.h>
#include <mach/vm_param.h>
#include <mach_debug/ipc_info.h>
#include <mach_debug/hash_info.h>
#include <kern/host.h>
#include <vm/vm_map.h>
#include <vm/vm_kern.h>
#include <ipc/ipc_space.h>
#include <ipc/ipc_port.h>
#include <ipc/ipc_hash.h>
#include <ipc/ipc_marequest.h>
#include <ipc/ipc_table.h>

/*
 *	Routine:	mach_port_get_srights [kernel call]
 *	Purpose:
 *		Retrieve the number of extant send rights
 *		that a receive right has.
 *	Conditions:
 *		Nothing locked.
 *	Returns:
 *		KERN_SUCCESS		Retrieved number of send rights.
 *		KERN_INVALID_TASK	The space is null.
 *		KERN_INVALID_TASK	The space is dead.
 *		KERN_INVALID_NAME	The name doesn't denote a right.
 *		KERN_INVALID_RIGHT	Name doesn't denote receive rights.
 */

kern_return_t
mach_port_get_srights(space, name, srightsp)
	ipc_space_t space;
	mach_port_t name;
	mach_port_rights_t *srightsp;
{
	ipc_port_t port;
	kern_return_t kr;
	mach_port_rights_t srights;

	if (space == IS_NULL)
		return KERN_INVALID_TASK;

	kr = ipc_port_translate_receive(space, name, &port);
	if (kr != KERN_SUCCESS)
		return kr;
	/* port is locked and active */

	srights = port->ip_srights;
	ip_unlock(port);

	*srightsp = srights;
	return KERN_SUCCESS;
}

/*
 *	Routine:	host_ipc_hash_info
 *	Purpose:
 *		Return information about the global reverse hash table.
 *	Conditions:
 *		Nothing locked.  If successful, returns a copy object
 *		which the caller must consume.
 *	Returns:
 *		KERN_SUCCESS		Returned information.
 *		KERN_INVALID_HOST	The host is null.
 *		KERN_RESOURCE_SHORTAGE	Couldn't allocate memory.
 */

kern_return_t
host_ipc_hash_info(host, copyp, countp)
	host_t host;
	vm_map_copy_t *copyp;
	mach_msg_type_number_t *countp;
{
	hash_info_bucket_t *info;
	mach_msg_type_number_t count;
	vm_map_copy_t copy;
	kern_return_t kr;

	if (host == HOST_NULL)
		return KERN_INVALID_HOST;

	kr = ipc_hash_info(&info, &count);
	if (kr != KERN_SUCCESS)
		return kr;

	kr = vm_map_copyin(ipc_kernel_map, (vm_offset_t) info,
			   (vm_size_t) (count * sizeof *info),
			   TRUE, FALSE, &copy);
	assert(kr == KERN_SUCCESS);

	*copyp = copy;
	*countp = count;
	return KERN_SUCCESS;
}

/*
 *	Routine:	host_ipc_marequest_info
 *	Purpose:
 *		Return information about the marequest hash table.
 *	Conditions:
 *		Nothing locked.  If successful, returns a copy object
 *		which the caller must consume.
 *	Returns:
 *		KERN_SUCCESS		Returned information.
 *		KERN_INVALID_HOST	The host is null.
 *		KERN_RESOURCE_SHORTAGE	Couldn't allocate memory.
 */

kern_return_t
host_ipc_marequest_info(host, maxp, copyp, countp)
	host_t host;
	unsigned int *maxp;
	vm_map_copy_t *copyp;
	mach_msg_type_number_t *countp;
{
	unsigned int max;
	hash_info_bucket_t *info;
	mach_msg_type_number_t count;
	vm_map_copy_t copy;
	kern_return_t kr;

	if (host == HOST_NULL)
		return KERN_INVALID_HOST;

	kr = ipc_marequest_info(&max, &info, &count);
	if (kr != KERN_SUCCESS)
		return kr;

	kr = vm_map_copyin(ipc_kernel_map, (vm_offset_t) info,
			   (vm_size_t) (count * sizeof *info),
			   TRUE, FALSE, &copy);
	assert(kr == KERN_SUCCESS);

	*maxp = max;
	*copyp = copy;
	*countp = count;
	return KERN_SUCCESS;
}

/*
 *	Routine:	mach_port_space_info
 *	Purpose:
 *	Conditions:
 *		Nothing locked.  If successful, returns a copy object
 *		which the caller must consume.
 *	Returns:
 *		KERN_SUCCESS		Returned information.
 *		KERN_INVALID_TASK	The space is null.
 *		KERN_INVALID_TASK	The space is dead.
 *		KERN_RESOURCE_SHORTAGE	Couldn't allocate memory.
 */

kern_return_t
mach_port_space_info(space, infop, tablep, tableCnt, treep, treeCnt)
	ipc_space_t space;
	ipc_info_space_t *infop;
	ipc_info_name_array_t *tablep;
	mach_msg_type_number_t *tableCnt;
	ipc_info_tree_name_array_t *treep;
	mach_msg_type_number_t *treeCnt;
{
	ipc_info_space_t info;
	ipc_info_name_t *table_info;
	ipc_info_tree_name_t *tree_info;
	ipc_tree_entry_t tentry;
	ipc_entry_t table;
	ipc_entry_num_t tsize;
	mach_port_index_t index;
	kern_return_t kr;

	vm_size_t size1;	/* size of allocated memory for table */
	vm_size_t size1_needed;	/* actual amount needed for table */
	vm_offset_t addr1;	/* allocated memory, for table */
	vm_map_copy_t memory1;	/* copied-in memory, for names */
	vm_size_t size2;	/* size of allocated memory for tree */
	vm_size_t size2_needed;	/* actual amount needed for tree */
	vm_offset_t addr2;	/* allocated memory, for tree */
	vm_map_copy_t memory2;	/* copied-in memory, for tree */

	if (space == IS_NULL)
		return KERN_INVALID_TASK;

	size1 = size2 = 0;

	for (;;) {
		is_read_lock(space);
		if (!space->is_active) {
			is_read_unlock(space);
			if (size1 != 0)
				kmem_free(ipc_kernel_map, addr1, size1);
			if (size2 != 0)
				kmem_free(ipc_kernel_map, addr2, size2);
			return KERN_INVALID_TASK;
		}

		size1_needed = round_page(space->is_table_size *
					  sizeof(ipc_info_name_t));
		size2_needed = round_page(space->is_tree_total *
					  sizeof(ipc_info_tree_name_t));

		if ((size1_needed <= size1) &&
		    (size2_needed <= size2))
			break;

		is_read_unlock(space);

		if (size1_needed > size1) {
			if (size1 != 0)
				kmem_free(ipc_kernel_map, addr1, size1);
			size1 = size1_needed;

			kr = vm_allocate(ipc_kernel_map, &addr1, size1, TRUE);
			if (kr != KERN_SUCCESS) {
				if (size2 != 0)
					kmem_free(ipc_kernel_map,
						  addr2, size2);

				return KERN_RESOURCE_SHORTAGE;
			}

			kr = vm_map_pageable(ipc_kernel_map,
					     addr1, addr1 + size1,
					     VM_PROT_READ|VM_PROT_WRITE);
			assert(kr == KERN_SUCCESS);
		}

		if (size2_needed > size2) {
			if (size2 != 0)
				kmem_free(ipc_kernel_map, addr2, size2);
			size2 = size2_needed;

			kr = vm_allocate(ipc_kernel_map, &addr2, size2, TRUE);
			if (kr != KERN_SUCCESS) {
				if (size1 != 0)
					kmem_free(ipc_kernel_map,
						  addr1, size1);

				return KERN_RESOURCE_SHORTAGE;
			}

			kr = vm_map_pageable(ipc_kernel_map,
					     addr2, addr2 + size2,
					     VM_PROT_READ|VM_PROT_WRITE);
			assert(kr == KERN_SUCCESS);
		}
	}
	/* space is read-locked and active; we have enough wired memory */

	info.iis_genno_mask = MACH_PORT_NGEN(MACH_PORT_DEAD);
	info.iis_table_size = space->is_table_size;
	info.iis_table_next = space->is_table_next->its_size;
	info.iis_tree_size = space->is_tree_total;
	info.iis_tree_small = space->is_tree_small;
	info.iis_tree_hash = space->is_tree_hash;

	table_info = (ipc_info_name_t *) addr1;
	tree_info = (ipc_info_tree_name_t *) addr2;

	table = space->is_table;
	tsize = space->is_table_size;

	for (index = 0; index < tsize; index++) {
		ipc_info_name_t *iin = &table_info[index];
		ipc_entry_t entry = &table[index];
		ipc_entry_bits_t bits = entry->ie_bits;

		iin->iin_name = MACH_PORT_MAKEB(index, bits);
		iin->iin_collision = (bits & IE_BITS_COLLISION) ? TRUE : FALSE;
#if	MACH_IPC_COMPAT
		iin->iin_compat = (bits & IE_BITS_COMPAT) ? TRUE : FALSE;
#else	MACH_IPC_COMPAT
		iin->iin_compat = FALSE;
#endif	MACH_IPC_COMPAT
		iin->iin_marequest = (bits & IE_BITS_MAREQUEST) ? TRUE : FALSE;
		iin->iin_type = IE_BITS_TYPE(bits);
		iin->iin_urefs = IE_BITS_UREFS(bits);
		iin->iin_object = (vm_offset_t) entry->ie_object;
		iin->iin_next = entry->ie_next;
		iin->iin_hash = entry->ie_index;
	}

	for (tentry = ipc_splay_traverse_start(&space->is_tree), index = 0;
	     tentry != ITE_NULL;
	     tentry = ipc_splay_traverse_next(&space->is_tree, FALSE)) {
		ipc_info_tree_name_t *iitn = &tree_info[index++];
		ipc_info_name_t *iin = &iitn->iitn_name;
		ipc_entry_t entry = &tentry->ite_entry;
		ipc_entry_bits_t bits = entry->ie_bits;

		assert(IE_BITS_TYPE(bits) != MACH_PORT_TYPE_NONE);

		iin->iin_name = tentry->ite_name;
		iin->iin_collision = (bits & IE_BITS_COLLISION) ? TRUE : FALSE;
#if	MACH_IPC_COMPAT
		iin->iin_compat = (bits & IE_BITS_COMPAT) ? TRUE : FALSE;
#else	MACH_IPC_COMPAT
		iin->iin_compat = FALSE;
#endif	MACH_IPC_COMPAT
		iin->iin_marequest = (bits & IE_BITS_MAREQUEST) ? TRUE : FALSE;
		iin->iin_type = IE_BITS_TYPE(bits);
		iin->iin_urefs = IE_BITS_UREFS(bits);
		iin->iin_object = (vm_offset_t) entry->ie_object;
		iin->iin_next = entry->ie_next;
		iin->iin_hash = entry->ie_index;

		if (tentry->ite_lchild == ITE_NULL)
			iitn->iitn_lchild = MACH_PORT_NULL;
		else
			iitn->iitn_lchild = tentry->ite_lchild->ite_name;

		if (tentry->ite_rchild == ITE_NULL)
			iitn->iitn_rchild = MACH_PORT_NULL;
		else
			iitn->iitn_rchild = tentry->ite_rchild->ite_name;

	}
	ipc_splay_traverse_finish(&space->is_tree);
	is_read_unlock(space);

	/*
	 *	Make used memory pageable and get it into
	 *	copy objects.  Free any unused memory.
	 */

	if (size1_needed == 0) {
		memory1 = VM_MAP_COPY_NULL;

		if (size1 != 0)
			kmem_free(ipc_kernel_map, addr1, size1);
	} else {
		kr = vm_map_pageable(ipc_kernel_map,
				     addr1, addr1 + size1_needed,
				     VM_PROT_NONE);
		assert(kr == KERN_SUCCESS);

		kr = vm_map_copyin(ipc_kernel_map, addr1, size1_needed,
				   TRUE, FALSE, &memory1);
		assert(kr == KERN_SUCCESS);

		if (size1 != size1_needed)
			kmem_free(ipc_kernel_map,
				  addr1 + size1_needed, size1 - size1_needed);
	}

	if (size2_needed == 0) {
		memory2 = VM_MAP_COPY_NULL;

		if (size2 != 0)
			kmem_free(ipc_kernel_map, addr2, size2);
	} else {
		kr = vm_map_pageable(ipc_kernel_map,
				     addr2, addr2 + size2_needed,
				     VM_PROT_NONE);
		assert(kr == KERN_SUCCESS);

		kr = vm_map_copyin(ipc_kernel_map, addr2, size2_needed,
				   TRUE, FALSE, &memory2);
		assert(kr == KERN_SUCCESS);

		if (size2 != size2_needed)
			kmem_free(ipc_kernel_map,
				  addr2 + size2_needed, size2 - size2_needed);
	}

	*infop = info;
	*tablep = (ipc_info_name_array_t) memory1;
	*tableCnt = info.iis_table_size;
	*treep = (ipc_info_tree_name_array_t) memory2;
	*treeCnt = info.iis_tree_size;
	return KERN_SUCCESS;
}

/*
 *	Routine:	mach_port_dnrequest_info
 *	Purpose:
 *		Returns information about the dead-name requests
 *		registered with the named receive right.
 *	Conditions:
 *		Nothing locked.
 *	Returns:
 *		KERN_SUCCESS		Retrieved information.
 *		KERN_INVALID_TASK	The space is null.
 *		KERN_INVALID_TASK	The space is dead.
 *		KERN_INVALID_NAME	The name doesn't denote a right.
 *		KERN_INVALID_RIGHT	Name doesn't denote receive rights.
 */

kern_return_t
mach_port_dnrequest_info(space, name, totalp, usedp)
	ipc_space_t space;
	mach_port_t name;
	unsigned int *totalp, *usedp;
{
	unsigned int total, used;
	ipc_port_t port;
	kern_return_t kr;

	if (space == IS_NULL)
		return KERN_INVALID_TASK;

	kr = ipc_port_translate_receive(space, name, &port);
	if (kr != KERN_SUCCESS)
		return kr;
	/* port is locked and active */

	if (port->ip_dnrequests == IPR_NULL) {
		total = 0;
		used = 0;
	} else {
		ipc_port_request_t dnrequests = port->ip_dnrequests;
		ipc_port_request_index_t index;

		total = dnrequests->ipr_size->its_size;

		for (index = 1, used = 0;
		     index < total; index++) {
			ipc_port_request_t ipr = &dnrequests[index];

			if (ipr->ipr_name != MACH_PORT_NULL)
				used++;
		}
	}
	ip_unlock(port);

	*totalp = total;
	*usedp = used;
	return KERN_SUCCESS;
}
