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
 * $Log:	xmm_import.c,v $
 * Revision 2.3  91/07/01  08:26:12  jsb
 * 	Fixed object importation protocol. Return valid return values.
 * 	[91/06/29  15:30:10  jsb]
 * 
 * Revision 2.2  91/06/17  15:48:18  jsb
 * 	First checkin.
 * 	[91/06/17  11:03:28  jsb]
 * 
 */
/*
 *	File:	norma/xmm_import.c
 *	Author:	Joseph S. Barrera III
 *	Date:	1991
 *
 *	Xmm layer for mapping a remote object.
 */

#include <norma/xmm_obj.h>
#include <ipc/ipc_space.h>
#include <ipc/ipc_port.h>

struct mobj {
	struct xmm_obj	obj;
	mach_port_t	memory_object;
	mach_port_t	export_master;
	xmm_obj_t	export_kobj;
};

#undef  KOBJ
#define KOBJ    ((struct mobj *) kobj)

kern_return_t m_import_init();
kern_return_t m_import_terminate();
kern_return_t m_import_copy();
kern_return_t m_import_data_request();
kern_return_t m_import_data_unlock();
kern_return_t m_import_data_write();
kern_return_t m_import_lock_completed();
kern_return_t m_import_supply_completed();
kern_return_t m_import_data_return();

xmm_decl(import_class,
	/* m_init		*/	m_import_init,
	/* m_terminate		*/	m_import_terminate,
	/* m_copy		*/	m_import_copy,
	/* m_data_request	*/	m_import_data_request,
	/* m_data_unlock	*/	m_import_data_unlock,
	/* m_data_write		*/	m_import_data_write,
	/* m_lock_completed	*/	m_import_lock_completed,
	/* m_supply_completed	*/	m_import_supply_completed,
	/* m_data_return	*/	m_import_data_return,

	/* k_data_provided	*/	k_invalid_data_provided,
	/* k_data_unavailable	*/	k_invalid_data_unavailable,
	/* k_get_attributes	*/	k_invalid_get_attributes,
	/* k_lock_request	*/	k_invalid_lock_request,
	/* k_data_error		*/	k_invalid_data_error,
	/* k_set_attributes	*/	k_invalid_set_attributes,
	/* k_destroy		*/	k_invalid_destroy,
	/* k_data_supply	*/	k_invalid_data_supply,

	/* name			*/	"import",
	/* size			*/	sizeof(struct mobj)
);

kern_return_t
xmm_import_create(memory_object, new_mobj)
	mach_port_t memory_object;
	xmm_obj_t *new_mobj;
{
	xmm_obj_t mobj;
	kern_return_t kr;
	
	kr = xmm_obj_allocate(&import_class, XMM_OBJ_NULL, &mobj);
	if (kr != KERN_SUCCESS) {
		return kr;
	}

	MOBJ->memory_object = memory_object;	/* needed? */

	*new_mobj = mobj;
	return KERN_SUCCESS;
}

mach_port_t
xmm_guess_export_master(memory_object)
	mach_port_t memory_object;
{
	return master_device_port_at_node(ipc_port_node(memory_object));
}

m_import_init(mobj, k_kobj, memory_object_name, page_size)
	xmm_obj_t mobj;
	xmm_obj_t k_kobj;
	mach_port_t memory_object_name;
	vm_size_t page_size;
{
	kern_return_t kr;
	xmm_obj_t kobj = mobj;
	extern ipc_port_t master_device_port;
	
	/*
	 * XXX
	 * Should check initialization, here and elsewhere, remembering
	 * that initialization has three states:
	 *	Created, but m_import_init not called
	 *	m_import_init called, but set_attributes not called
	 *	set_attributes called
	 */

	k_kobj->m_kobj = kobj;
	kobj->k_kobj = k_kobj;

	kr = proxy_init(xmm_guess_export_master(MOBJ->memory_object),
			master_device_port,
			kobj,
			MOBJ->memory_object,
			page_size);
#if 1
	if (kr) {
		panic("m_import_init: moi returns %x\n", kr);
	}
#endif
	return KERN_SUCCESS;
}

m_import_terminate(mobj, kobj, memory_object_name)
	xmm_obj_t mobj;
	xmm_obj_t kobj;
	mach_port_t memory_object_name;
{
	proxy_terminate(MOBJ->export_master,
			MOBJ->export_kobj);
	/* XXX deallocate memory_object_name ??? */
	return KERN_SUCCESS;
}

m_import_copy(mobj, kobj, offset, length, new_memory_object)
	xmm_obj_t mobj;
	xmm_obj_t kobj;
	vm_offset_t offset;
	vm_size_t length;
	mach_port_t new_memory_object;
{
	panic("m_import_copy\n");
}

m_import_data_request(mobj, kobj, offset, length, desired_access)
	xmm_obj_t mobj;
	xmm_obj_t kobj;
	vm_offset_t offset;
	vm_size_t length;
	vm_prot_t desired_access;
{
	return proxy_data_request(MOBJ->export_master,
				  MOBJ->export_kobj,
				  offset,
				  length,
				  desired_access);
}

m_import_data_unlock(mobj, kobj, offset, length, desired_access)
	xmm_obj_t mobj;
	xmm_obj_t kobj;
	vm_offset_t offset;
	vm_size_t length;
	vm_prot_t desired_access;
{
	return proxy_data_unlock(MOBJ->export_master,
				 MOBJ->export_kobj,
				 offset,
				 length,
				 desired_access);
}

m_import_data_write(mobj, kobj, offset, data, length)
	xmm_obj_t mobj;
	xmm_obj_t kobj;
	vm_offset_t offset;
	vm_offset_t data;
	int length;
{
	return proxy_data_write(MOBJ->export_master,
				MOBJ->export_kobj,
				offset,
				data,
				length);
}

m_import_lock_completed(mobj, kobj, offset, length)
	xmm_obj_t mobj;
	xmm_obj_t kobj;
	vm_offset_t offset;
	vm_size_t length;
{
	return proxy_lock_completed(MOBJ->export_master,
				    MOBJ->export_kobj,
				    offset,
				    length);
}

m_import_supply_completed(mobj, kobj, offset, length, result, error_offset)
	xmm_obj_t mobj;
	xmm_obj_t kobj;
	vm_offset_t offset;
	vm_size_t length;
	kern_return_t result;
	vm_offset_t error_offset;
{
#if 1
	panic("m_import_supply_completed\n");
#else
	return proxy_supply_completed(MOBJ->export_master,
				      MOBJ->export_kobj,
				      offset,
				      length,
				      result,
				      error_offset);
#endif
}

m_import_data_return(mobj, kobj, offset, data, length)
	xmm_obj_t mobj;
	xmm_obj_t kobj;
	vm_offset_t offset;
	vm_offset_t data;
	vm_size_t length;
{
#if 1
	panic("m_import_data_return\n");
#else
	return proxy_data_return(MOBJ->export_master,
				 MOBJ->export_kobj,
				 offset,
				 data,
				 length);
#endif
}

_proxy_data_provided(import_master, kobj, offset, data, length, lock_value)
	mach_port_t	import_master;
	xmm_obj_t	kobj;
	vm_offset_t	offset;
	pointer_t	data;
	unsigned int	length;
	vm_prot_t	lock_value;
{
	return K_DATA_PROVIDED(kobj, offset, data, length, lock_value);
}

_proxy_data_unavailable(import_master, kobj, offset, length)
	mach_port_t	import_master;
	xmm_obj_t	kobj;
	vm_offset_t	offset;
	vm_size_t	length;
{
	return K_DATA_UNAVAILABLE(kobj, offset, length);
}

_proxy_get_attributes(import_master, kobj, object_ready, may_cache,
		      copy_strategy)
	mach_port_t	import_master;
	xmm_obj_t	kobj;
	boolean_t	*object_ready;
	boolean_t	*may_cache;
	memory_object_copy_strategy_t *copy_strategy;
{
	return K_GET_ATTRIBUTES(kobj, object_ready, may_cache, copy_strategy);
}

_proxy_lock_request(import_master, kobj, offset, length, should_clean,
		    should_flush, prot, reply_to, reply_to_type)
	mach_port_t	import_master;
	xmm_obj_t	kobj;
	vm_offset_t	offset;
	vm_size_t	length;
	boolean_t	should_clean;
	boolean_t	should_flush;
	vm_prot_t	prot;
	mach_port_t	reply_to;
	/*mach_msg_type_name_t*/int	reply_to_type;
{
	xmm_obj_t robj;
	
#if     lint
	reply_to_type++;
#endif  lint
	if (reply_to == MACH_PORT_NULL) {
		robj = XMM_OBJ_NULL;
	} else if (reply_to == KOBJ->memory_object) {
		robj = kobj;
	} else {
		/* XXX should cons up a new obj */
		panic("xmm lock_request\n");
	}
	return K_LOCK_REQUEST(kobj, offset, length, should_clean,
			      should_flush, prot, robj);
}

_proxy_data_error(import_master, kobj, offset, length, error_value)
	mach_port_t	import_master;
	xmm_obj_t	kobj;
	vm_offset_t	offset;
	vm_size_t	length;
	kern_return_t	error_value;
{
	return K_DATA_ERROR(kobj, offset, length, error_value);
}

_proxy_set_attributes(import_master, kobj, export_master, export_kobj,
		      object_ready, may_cache, copy_strategy, error_value)
	mach_port_t	import_master;
	xmm_obj_t	kobj;
	mach_port_t	export_master;
	xmm_obj_t	export_kobj;
	boolean_t	object_ready;
	boolean_t	may_cache;
	memory_object_copy_strategy_t copy_strategy;
	kern_return_t	error_value;
{
	if (error_value) {
		/* destroy? or should export have done that? */
		printf("proxy_set_attributes loses\n");
	}
	KOBJ->export_master = export_master;
	KOBJ->export_kobj = export_kobj;
	return K_SET_ATTRIBUTES(kobj, object_ready, may_cache, copy_strategy);
}

_proxy_destroy(import_master, kobj, reason)
	mach_port_t	import_master;
	xmm_obj_t	kobj;
	kern_return_t	reason;
{
	return K_DESTROY(kobj, reason);
}
