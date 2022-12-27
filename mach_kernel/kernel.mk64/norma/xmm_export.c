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
 * $Log:	xmm_export.c,v $
 * Revision 2.3  91/07/01  08:26:07  jsb
 * 	Fixed object importation protocol.
 * 	Corrected declaration of _proxy_lock_completed.
 * 	[91/06/29  15:28:46  jsb]
 * 
 * Revision 2.2  91/06/17  15:48:15  jsb
 * 	First checkin.
 * 	[91/06/17  11:06:11  jsb]
 * 
 */
/*
 *	File:	norma/xmm_export.c
 *	Author:	Joseph S. Barrera III
 *	Date:	1991
 *
 *	Xmm layer for allowing others to map a local object.
 */

#include <norma/xmm_obj.h>
#include <ipc/ipc_space.h>
#include <ipc/ipc_port.h>

struct mobj {
	struct xmm_obj	obj;
	mach_port_t	memory_object;
	mach_port_t	import_master;
	xmm_obj_t	import_kobj;
};

#undef  KOBJ
#define KOBJ    ((struct mobj *) kobj)

kern_return_t k_export_data_provided();
kern_return_t k_export_data_unavailable();
kern_return_t k_export_get_attributes();
kern_return_t k_export_lock_request();
kern_return_t k_export_data_error();
kern_return_t k_export_set_attributes();
kern_return_t k_export_destroy();
kern_return_t k_export_data_supply();

xmm_decl(export_class,
	/* m_init		*/	m_invalid_init,
	/* m_terminate		*/	m_invalid_terminate,
	/* m_copy		*/	m_invalid_copy,
	/* m_data_request	*/	m_invalid_data_request,
	/* m_data_unlock	*/	m_invalid_data_unlock,
	/* m_data_write		*/	m_invalid_data_write,
	/* m_lock_completed	*/	m_invalid_lock_completed,
	/* m_supply_completed	*/	m_invalid_supply_completed,
	/* m_data_return	*/	m_invalid_data_return,

	/* k_data_provided	*/	k_export_data_provided,
	/* k_data_unavailable	*/	k_export_data_unavailable,
	/* k_get_attributes	*/	k_export_get_attributes,
	/* k_lock_request	*/	k_export_lock_request,
	/* k_data_error		*/	k_export_data_error,
	/* k_set_attributes	*/	k_export_set_attributes,
	/* k_destroy		*/	k_export_destroy,
	/* k_data_supply	*/	k_export_data_supply,

	/* name			*/	"export",
	/* size			*/	sizeof(struct mobj)
);

k_export_data_provided(kobj, offset, data, length, lock_value)
	xmm_obj_t kobj;
	vm_offset_t offset;
	vm_offset_t data;
	vm_size_t length;
	vm_prot_t lock_value;
{
	kern_return_t kr;

	kr = proxy_data_provided(KOBJ->import_master, KOBJ->import_kobj,
				 offset, data, length, lock_value);
	return kr;
}

k_export_data_unavailable(kobj, offset, length)
	xmm_obj_t kobj;
	vm_offset_t offset;
	vm_size_t length;
{
	kern_return_t kr;

	kr = proxy_data_unavailable(KOBJ->import_master, KOBJ->import_kobj,
				    offset, length);
	return kr;
}

k_export_get_attributes(kobj, object_ready, may_cache, copy_strategy)
	xmm_obj_t kobj;
	boolean_t *object_ready;
	boolean_t *may_cache;
	memory_object_copy_strategy_t *copy_strategy;
{
	kern_return_t kr;

	kr = proxy_get_attributes(KOBJ->import_master,
				  KOBJ->import_kobj, object_ready,
				  may_cache, copy_strategy);
	return kr;
}

k_export_lock_request(kobj, offset, length, should_clean, should_flush,
		      lock_value, mobj)
	xmm_obj_t kobj;
	vm_offset_t offset;
	vm_size_t length;
	boolean_t should_clean;
	boolean_t should_flush;
	vm_prot_t lock_value;
	xmm_obj_t mobj;
{
	kern_return_t kr;

	/*
	 * XXX There is a general problem with K_LOCK_REQUEST.
	 * XXX So far, it looks like memory managers can only specify
	 * XXX their own memory objects (mobjs) as reply objects.
	 */
	kr = proxy_lock_request(KOBJ->import_master, KOBJ->import_kobj,
				offset, length, should_clean,
				should_flush, lock_value,
				(mobj
				 ? MOBJ->memory_object
				 : MACH_PORT_NULL),
				0 /* XXX reply_to_type */);
	return kr;
}

k_export_data_error(kobj, offset, length, error_value)
	xmm_obj_t kobj;
	vm_offset_t offset;
	vm_size_t length;
	kern_return_t error_value;
{
	kern_return_t kr;

	kr = proxy_data_error(KOBJ->import_master, KOBJ->import_kobj,
			      offset, length, error_value);
	return kr;
}

k_export_set_attributes(kobj, object_ready, may_cache, copy_strategy)
	xmm_obj_t kobj;
	boolean_t object_ready;
	boolean_t may_cache;
	memory_object_copy_strategy_t copy_strategy;
{
	kern_return_t kr;
	extern ipc_port_t master_device_port;

	kr = proxy_set_attributes(KOBJ->import_master, KOBJ->import_kobj,
				  master_device_port, kobj, object_ready,
				  may_cache, copy_strategy, KERN_SUCCESS);
	return kr;
}

k_export_destroy(kobj, reason)
	xmm_obj_t kobj;
	kern_return_t reason;
{
	kern_return_t kr;

	kr = proxy_destroy(KOBJ->import_master, KOBJ->import_kobj, reason);
	return kr;
}

k_export_data_supply(kobj, offset, data, length, dealloc_data, lock_value,
		     precious, reply_to)
	xmm_obj_t kobj;
	vm_offset_t offset;
	vm_offset_t data;
	vm_size_t length;
	boolean_t dealloc_data;
	vm_prot_t lock_value;
	boolean_t precious;
	mach_port_t reply_to;
{
	kern_return_t kr;

#if 0
	kr = proxy_data_supply(KOBJ->import_master, KOBJ->import_kobj,
			       offset, data, length, dealloc_data,
			       lock_value, precious, reply_to);
#endif
	return kr;
}

_proxy_init(export_master, import_master, import_kobj, memory_object,
	    page_size)
	mach_port_t export_master;
	mach_port_t import_master;
	xmm_obj_t import_kobj;
	mach_port_t memory_object;
	vm_size_t page_size;
{
	xmm_obj_t mobj;
	kern_return_t kr;
	
	/*
	 * XXX
	 * Check for multiple inits and/or reuse of memory_object
	 */
	if (ipc_port_node(memory_object) != node_self()) {
		printf("_proxy_init: bad guess, shithead\n");
		return KERN_FAILURE;
	}

	kr = xmm_lookup(memory_object, &mobj);
	if (kr != KERN_SUCCESS) {
		printf("_proxy_init: xmm_lookup: %x\n", kr);
		return kr;
	}
#if 1
	kr = xmm_debug_create(mobj, &mobj);
	if (kr != KERN_SUCCESS) {
		printf("_proxy_init: xmm_debug_create.1: %x\n", kr);
		return kr;
	}
#endif
	kr = xmm_obj_allocate(&export_class, mobj, &mobj);
	if (kr != KERN_SUCCESS) {
		printf("_proxy_init: xmm_obj_allocate: %x\n", kr);
		return kr;
	}

	MOBJ->memory_object = memory_object;
	MOBJ->import_master = import_master;
	MOBJ->import_kobj = import_kobj;

	M_INIT(mobj, mobj, 0, page_size);

	return KERN_FAILURE;
}

_proxy_terminate(export_master, kobj, memory_object_name)
	mach_port_t export_master;
	xmm_obj_t kobj;
	mach_port_t memory_object_name;
{
	return M_TERMINATE(kobj, kobj, memory_object_name);
}

/* ARGSUSED */
_proxy_copy(export_master, kobj, offset, length, new_memory_object)
	mach_port_t export_master;
	xmm_obj_t kobj;
	vm_offset_t offset;
	vm_size_t length;
	memory_object_t new_memory_object;
{
	/* XXX ? */
	panic("xmm_export: memory_object_copy\n");
	return KERN_FAILURE;
}

_proxy_data_request(export_master, kobj, offset, length, desired_access)
	mach_port_t export_master;
	xmm_obj_t kobj;
	vm_offset_t offset;
	vm_size_t length;
	vm_prot_t desired_access;
{
	return M_DATA_REQUEST(kobj, kobj, offset, length, desired_access);
}

_proxy_data_unlock(export_master, kobj, offset, length, desired_access)
	mach_port_t export_master;
	xmm_obj_t kobj;
	vm_offset_t offset;
	vm_size_t length;
	vm_prot_t desired_access;
{
	return M_DATA_UNLOCK(kobj, kobj, offset, length, desired_access);
}

_proxy_data_write(export_master, kobj, offset, data, length)
	mach_port_t export_master;
	xmm_obj_t kobj;
	vm_offset_t offset;
	char *data;
	int length;
{
	return M_DATA_WRITE(kobj, kobj, offset, data, length);
}

/*
 * XXX Consumed rights, etc.
 */
_proxy_lock_completed(export_master, kobj, offset, length)
	mach_port_t export_master;
	xmm_obj_t kobj;
	vm_offset_t offset;
	vm_size_t length;
{
	return M_LOCK_COMPLETED(kobj, kobj, offset, length);
}

_proxy_supply_completed(export_master, kobj, offset, length, result,
			       error_offset)
	mach_port_t export_master;
	xmm_obj_t kobj;
	vm_offset_t offset;
	vm_size_t length;
	kern_return_t result;
	vm_offset_t error_offset;
{
	return M_SUPPLY_COMPLETED(kobj, kobj, offset, length, result,
				  error_offset);
}

_proxy_data_return(export_master, kobj, offset, data, length)
	mach_port_t export_master;
	xmm_obj_t kobj;
	vm_offset_t offset;
	vm_offset_t data;
	vm_size_t length;
{
	return M_DATA_RETURN(kobj, kobj, offset, data, length);
}
