/* 
 * Mach Operating System
 * Copyright (c) 1991 Carnegie Mellon University
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 * 
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS 
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
 * any improvements or extensions that they make and grant Carnegie the
 * rights to redistribute these changes.
 */
/*
 * HISTORY
 * $Log:	xmm_copy.c,v $
 * Revision 2.2  91/08/28  11:16:22  jsb
 * 	In m_copy_data_request: removed dead code, and added missing
 * 	is_continuation parameter to vm_map_copyin_page_list.
 * 	[91/08/16  14:25:04  jsb]
 * 
 * 	First checkin.
 * 	[91/08/15  13:03:06  jsb]
 * 
 */
/*
 *	File:	norma/xmm_copy.c
 *	Author:	Joseph S. Barrera III
 *	Date:	1991
 */

#include <norma/xmm_obj.h>
#include <norma/xmm_user_rename.h>
#include <ipc/ipc_space.h>
#include <ipc/ipc_port.h>
#include <vm/memory_object.h>
#include <vm/vm_fault.h>
#include <vm/vm_map.h>
#include <vm/vm_object.h>
#include <vm/vm_page.h>
#include <vm/vm_pageout.h>

struct mobj {
	struct xmm_obj	obj;
	ipc_port_t	memory_object;	/* must be second field */
	xmm_obj_t	next;		/* must be third field */
	vm_map_t	map;
	vm_offset_t	start;
	vm_size_t	size;
	boolean_t	*shadowed;	/* which pages are in shadow obj */
};

#undef  KOBJ
#define KOBJ    ((struct mobj *) kobj)

kern_return_t m_copy_init();
kern_return_t m_copy_terminate();
kern_return_t m_copy_data_request();
kern_return_t m_copy_data_write();

xmm_decl(copy_class,
	/* m_init		*/	m_copy_init,
	/* m_terminate		*/	m_copy_terminate,
	/* m_copy		*/	m_invalid_copy,
	/* m_data_request	*/	m_copy_data_request,
	/* m_data_unlock	*/	m_invalid_data_unlock,
	/* m_data_write		*/	m_copy_data_write,
	/* m_lock_completed	*/	m_invalid_lock_completed,
	/* m_supply_completed	*/	m_invalid_supply_completed,
	/* m_data_return	*/	m_invalid_data_return,

	/* k_data_provided	*/	k_invalid_data_provided,
	/* k_data_unavailable	*/	k_invalid_data_unavailable,
	/* k_get_attributes	*/	k_invalid_get_attributes,
	/* k_lock_request	*/	k_invalid_lock_request,
	/* k_data_error		*/	k_invalid_data_error,
	/* k_set_attributes	*/	k_invalid_set_attributes,
	/* k_destroy		*/	k_invalid_destroy,
	/* k_data_supply	*/	k_invalid_data_supply,

	/* name			*/	"copy",
	/* size			*/	sizeof(struct mobj)
);

#if	666
/*
 * This should live somewhere else
 */
xmm_obj_t	internal_mobj_list;

boolean_t
is_internal_memory_object(memory_object, new_mobj)
	ipc_port_t memory_object;
	xmm_obj_t *new_mobj;
{
	xmm_obj_t mobj;

	for (mobj = internal_mobj_list; mobj; mobj = MOBJ->next) {
		if (MOBJ->memory_object == memory_object) {
			*new_mobj = mobj;
			return TRUE;
		}
	}
	return FALSE;
}

add_internal_memory_object(mobj)
	xmm_obj_t mobj;
{
	MOBJ->next = internal_mobj_list;
	internal_mobj_list = mobj;
}
#endif	666

kern_return_t
norma_copy_create(map, start, size, memory_object_p)
	vm_map_t map;
	vm_offset_t start;
	vm_size_t size;
	ipc_port_t *memory_object_p;
{
	ipc_port_t memory_object;
	xmm_obj_t mobj;
	kern_return_t kr;
	vm_object_t object;

mumble("xmm_copy_create\n");
	memory_object = (ipc_port_t) ipc_port_alloc_kernel();
	if (memory_object == IP_NULL) {
		panic("xmm_copy_create: ipc_port_alloc_kernel");
	}
	kr = xmm_obj_allocate(&copy_class, XMM_OBJ_NULL, &mobj);
	if (kr != KERN_SUCCESS) {
		return kr;
	}
	MOBJ->memory_object = memory_object;
	add_internal_memory_object(mobj);

	MOBJ->map = map;
	MOBJ->start = start;
	MOBJ->size = size;
	*memory_object_p = memory_object;
mumble("xmm_copy_create returning\n");
	return KERN_SUCCESS;
}

m_copy_init(mobj, k_kobj, memory_object_name, page_size)
	xmm_obj_t mobj;
	xmm_obj_t k_kobj;
	ipc_port_t memory_object_name;
	vm_size_t page_size;
{
	kern_return_t kr;
	xmm_obj_t kobj = mobj;
	
mumble("xmm_copy_init\n");

	k_kobj->m_kobj = kobj;
	kobj->k_kobj = k_kobj;

	return K_SET_ATTRIBUTES(kobj, TRUE, FALSE, MEMORY_OBJECT_COPY_DELAY);
}

/*
 * Should instead use no-more-senders for termination
 */
m_copy_terminate(mobj, kobj, memory_object_name)
	xmm_obj_t mobj;
	xmm_obj_t kobj;
	ipc_port_t memory_object_name;
{
	/* XXX */
	return KERN_SUCCESS;
}

m_copy_data_request(mobj, kobj, offset, length, desired_access)
	xmm_obj_t mobj;
	xmm_obj_t kobj;
	vm_offset_t offset;
	vm_size_t length;
	vm_prot_t desired_access;
{
	extern zone_t vm_map_copy_zone;
	vm_map_copy_t copy;
	vm_page_t m;
	vm_offset_t kvaddr;
	int i;
	kern_return_t kr;
	
mumble("xmm_copy_data_request\n");
	if (length != PAGE_SIZE) {
		panic("m_copy_data_request: big length %d\n", length);
	}
	if (offset + length > MOBJ->size) {
		panic("m_copy_data_request: big offset %d\n", offset);
	}
	kr = vm_map_copyin_page_list(MOBJ->map,
				     MOBJ->start + offset,
				     PAGE_SIZE,
				     FALSE,/* src_destroy */
				     TRUE,/* steal pages */
				     &copy,
				     FALSE/* is continuation */);
	if (kr) {
		fret("xmm_copy_data_request 0x%x 0x%x 0x%x: %x\n",
		     MOBJ->start, offset, length, kr);
		return K_DATA_ERROR(kobj, offset, length, kr);
	}
mumble("xmm_copy_data_request returns\n");
	return K_DATA_PROVIDED(kobj, offset, copy, length, VM_PROT_NONE);
}

int Right = 0;

kern_return_t
m_copy_data_write(mobj, kobj, offset, data, length)
	xmm_obj_t	mobj;
	xmm_obj_t	kobj;
	vm_offset_t	offset;
	vm_offset_t	data;
	vm_size_t	length;
{
	if (Right) panic("m_copy_data_write");
	return KERN_SUCCESS;
}
