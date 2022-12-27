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
 * $Log:	xmm_server.c,v $
 * Revision 2.5  91/08/28  11:16:24  jsb
 * 	Added temporary definition for memory_object_change_completed.
 * 	[91/08/16  14:21:20  jsb]
 * 
 * 	Added comment to xmm_lookup about read-only pagers.
 * 	[91/08/15  10:12:12  jsb]
 * 
 * Revision 2.4  91/08/03  18:19:40  jsb
 * 	Changed mach_port_t to ipc_port_t whereever appropriate.
 * 	[91/07/17  14:07:08  jsb]
 * 
 * Revision 2.3  91/07/01  08:26:29  jsb
 * 	Added support for memory_object_create.
 * 	Now export normal memory_object_init with standard arguments.
 * 	Improved object initialization logic.
 * 	Added garbage collection.
 * 	[91/06/29  15:39:01  jsb]
 * 
 * Revision 2.2  91/06/17  15:48:33  jsb
 * 	First checkin.
 * 	[91/06/17  11:05:10  jsb]
 * 
 */
/*
 *	File:	norma/xmm_server.c
 *	Author:	Joseph S. Barrera III
 *	Date:	1991
 *
 *	Interface between kernel and xmm system.
 */

#include <norma/xmm_obj.h>
#include <norma/xmm_server_rename.h>
#include <ipc/ipc_space.h>
#include <ipc/ipc_port.h>
#include <vm/memory_object.h>
#include <vm/vm_fault.h>
#include <vm/vm_map.h>
#include <vm/vm_object.h>
#include <vm/vm_page.h>
#include <vm/vm_pageout.h>
#include <kern/ipc_kobject.h>

struct mobj {
	struct xmm_obj	obj;
	ipc_port_t	memory_object;
	vm_object_t	vm_object;
	xmm_obj_t	svm_mobj;
};

#undef  KOBJ
#define KOBJ    ((struct mobj *) kobj)

kern_return_t k_server_data_provided();
kern_return_t k_server_data_unavailable();
kern_return_t k_server_get_attributes();
kern_return_t k_server_lock_request();
kern_return_t k_server_data_error();
kern_return_t k_server_set_attributes();
kern_return_t k_server_destroy();
kern_return_t k_server_data_supply();

xmm_decl(server_class,
	/* m_init		*/	m_invalid_init,
	/* m_terminate		*/	m_invalid_terminate,
	/* m_copy		*/	m_invalid_copy,
	/* m_data_request	*/	m_invalid_data_request,
	/* m_data_unlock	*/	m_invalid_data_unlock,
	/* m_data_write		*/	m_invalid_data_write,
	/* m_lock_completed	*/	m_invalid_lock_completed,
	/* m_supply_completed	*/	m_invalid_supply_completed,
	/* m_data_return	*/	m_invalid_data_return,

	/* k_data_provided	*/	k_server_data_provided,
	/* k_data_unavailable	*/	k_server_data_unavailable,
	/* k_get_attributes	*/	k_server_get_attributes,
	/* k_lock_request	*/	k_server_lock_request,
	/* k_data_error		*/	k_server_data_error,
	/* k_set_attributes	*/	k_server_set_attributes,
	/* k_destroy		*/	k_server_destroy,
	/* k_data_supply	*/	k_server_data_supply,

	/* name			*/	"server",
	/* size			*/	sizeof(struct mobj)
);

k_server_data_provided(kobj, offset, data, length, lock_value)
	xmm_obj_t kobj;
	vm_offset_t offset;
	vm_offset_t data;
	vm_size_t length;
	vm_prot_t lock_value;
{
	kern_return_t kr;

	vm_object_incr(KOBJ->vm_object);
	kr = memory_object_data_provided(KOBJ->vm_object, offset, data,
					 length, lock_value);
	return kr;
}

k_server_data_unavailable(kobj, offset, length)
	xmm_obj_t kobj;
	vm_offset_t offset;
	vm_size_t length;
{
	kern_return_t kr;

	vm_object_incr(KOBJ->vm_object);
	kr = memory_object_data_unavailable(KOBJ->vm_object, offset,
					    length);
	return kr;
}

k_server_get_attributes(kobj, object_ready, may_cache, copy_strategy)
	xmm_obj_t kobj;
	boolean_t *object_ready;
	boolean_t *may_cache;
	memory_object_copy_strategy_t *copy_strategy;
{
	kern_return_t kr;

	vm_object_incr(KOBJ->vm_object);
	kr = memory_object_get_attributes(KOBJ->vm_object, object_ready,
					  may_cache, copy_strategy);
	return kr;
}

k_server_lock_request(kobj, offset, length, should_clean, should_flush,
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
	vm_object_incr(KOBJ->vm_object);
	kr = memory_object_lock_request(KOBJ->vm_object, offset, length,
					should_clean, should_flush, lock_value,
					(mobj
					 ? MOBJ->memory_object
					 : MACH_PORT_NULL),
					0 /* XXX reply_to_type */);
	return kr;
}

k_server_data_error(kobj, offset, length, error_value)
	xmm_obj_t kobj;
	vm_offset_t offset;
	vm_size_t length;
	kern_return_t error_value;
{
	kern_return_t kr;

	vm_object_incr(KOBJ->vm_object);
	kr = memory_object_data_error(KOBJ->vm_object, offset, length,
				      error_value);
	return kr;
}

k_server_set_attributes(kobj, object_ready, may_cache, copy_strategy)
	xmm_obj_t kobj;
	boolean_t object_ready;
	boolean_t may_cache;
	memory_object_copy_strategy_t copy_strategy;
{
	kern_return_t kr;

	if (KOBJ->vm_object->internal) {
		/*
		 * Simply a side-effect of using memory_object_init
		 * in place of memory_object_create. We don't want
		 * to let the kernel see this.
		 */
		return KERN_SUCCESS;
	}
	vm_object_incr(KOBJ->vm_object);
	kr = memory_object_set_attributes(KOBJ->vm_object, object_ready,
					  may_cache, copy_strategy);
	return kr;
}

k_server_destroy(kobj, reason)
	xmm_obj_t kobj;
	kern_return_t reason;
{
	kern_return_t kr;

	vm_object_incr(KOBJ->vm_object);
	kr = memory_object_destroy(KOBJ->vm_object, reason);
	return kr;
}

k_server_data_supply(kobj, offset, data, length, dealloc_data, lock_value,
		     precious, reply_to)
	xmm_obj_t kobj;
	vm_offset_t offset;
	vm_offset_t data;
	vm_size_t length;
	boolean_t dealloc_data;
	vm_prot_t lock_value;
	boolean_t precious;
	ipc_port_t reply_to;
{
	return k_invalid_data_supply(kobj, offset, data, length, dealloc_data,
				     lock_value, precious, reply_to);
}

#define	SAE	1
#if	SAE

/*
 * XXX
 * Should either use hash table,
 * or better yet use kobject technology.
 */

typedef struct xmm_exception *xmm_exception_t;

#define	XMM_EXCEPTION_NULL	((xmm_exception_t) 0)

struct xmm_exception {
	ipc_port_t	port;
	xmm_exception_t	next;
};

xmm_exception_t xmm_exception_list = XMM_EXCEPTION_NULL;

xmm_is_exception(port)
	ipc_port_t port;
{
	xmm_exception_t sx;

	for (sx = xmm_exception_list; sx; sx = sx->next) {
		if (sx->port == port) {
			return TRUE;
		}
	}
	return FALSE;
}

xmm_add_exception(port)
	ipc_port_t port;
{
	xmm_exception_t sx;

	printf("add exception 0x%x\n", port);
	if (xmm_is_exception(port)) {
		printf("(already was there)\n");
		return;
	}
	sx = (xmm_exception_t) kalloc(sizeof(*sx));
	sx->port = port;
	sx->next = xmm_exception_list;
	xmm_exception_list = sx;
}
#endif	SAE

/*
 * Find or create svm mobj associated with memory object.
 * This will be called on canonical node for memory object.
 *
 * Always called by xmm_export_create;
 * called by xmm_server_create when not mapped locally.
 */
kern_return_t
xmm_lookup(memory_object, new_mobj)
	ipc_port_t memory_object;
	xmm_obj_t *new_mobj;
{
	xmm_obj_t mobj;
	kern_return_t kr;
	vm_object_t vm_object;
	int kotype;

	/*
	 * If there is an mobj associated with memory_object, return it.
	 * XXX should hold appropriate lock XXX
	 *
	 * XXX should sleep if transitioning
	 */
	kotype = ip_kotype(memory_object);
	if (kotype == IKOT_PAGER) {
		/*
		 * If the memory_object has already been mapped locally,
		 * retrieve kobj via pager_request field in vm_object.
		 * If this field is null, then we are only now initializing
		 * the vm_object in vm_object_enter.
		 *
		 * XXX or...
		 * pager_request may be svm mobj.
		 */
		vm_object = (vm_object_t) memory_object->ip_kobject;
		if (vm_object->pager_request) {
			/*
			 * We have already initialized object.
			 * Obtain xmm_server flavor mobj.
			 */
			mobj = (xmm_obj_t) vm_object->pager_request;
			/*
			 * Return xmm_svm flavor mobj.
			 * XXX this starts to look ugly.
			 */
			if (mobj->class == &server_class) {
				*new_mobj = MOBJ->svm_mobj;
			} else {
				*new_mobj = mobj;
			}
			return KERN_SUCCESS;
		}
	} else if (kotype == IKOT_XMM_PAGER) {
		/*
		 * If the memory_object has been mapped externally,
		 * retrieve kobj directly.
		 */
		*new_mobj = (xmm_obj_t) memory_object->ip_kobject;
		return KERN_SUCCESS;
	}

	/*
	 * We need to create an svm mobj.
	 *
	 * XXX
	 * In some cases we will be using an internal, read-only pager,
	 * in which case we don't need the svm layer.
	 * XXX
	 * Currently, internal pagers are checked for in xmm_user_create;
	 * this is somewhat bogus; we should do it here, checking for
	 * a special kotype.
	 */
	kr = xmm_user_create(memory_object, &mobj);
	if (kr != KERN_SUCCESS) {
		printf("xmm_mo_create: xmm_user_create: %x\n", kr);
		return kr;
	}
#if 1
	kr = xmm_debug_create(mobj, &mobj);
	if (kr != KERN_SUCCESS) {
		printf("xmm_mo_create: xmm_debug_create.1: %x\n", kr);
		return kr;
	}
	kr = xmm_split_create(mobj, &mobj);
	if (kr != KERN_SUCCESS) {
		printf("xmm_mo_create: xmm_split_create: %x\n", kr);
		return kr;
	}
	kr = xmm_debug_create(mobj, &mobj);
	if (kr != KERN_SUCCESS) {
		printf("xmm_mo_create: xmm_debug_create.x: %x\n", kr);
		return kr;
	}
	kr = xmm_svm_create(mobj, &mobj);
	if (kr != KERN_SUCCESS) {
		printf("xmm_mo_create: xmm_svm_create: %x\n", kr);
		return kr;
	}
#endif
	/*
	 * Set up IKOT_XMM_PAGER association if appropriate.
	 */
	if (kotype == IKOT_NONE) {
		ipc_kobject_set(memory_object,
				(ipc_kobject_t) mobj,
				IKOT_XMM_PAGER);
	}
	*new_mobj = mobj;
	return KERN_SUCCESS;
}

memory_object_init(memory_object, memory_control, memory_object_name,
		   page_size)
	ipc_port_t memory_object;
	ipc_port_t memory_control;
	ipc_port_t memory_object_name;
	vm_size_t page_size;
{
	kern_return_t kr;
	xmm_obj_t mobj, svm_mobj;
	vm_object_t vm_object;
	int po;

	if (ip_kotype(memory_object) != IKOT_PAGER) {
		panic("memory_object_init: not ikot_pager\n");
	}
	vm_object = (vm_object_t) memory_object->ip_kobject;
	/*
	 * XXX
	 * Check for multiple inits and/or reuse of memory_object
	 */
	if (ipc_port_node(memory_object) == node_self()) {
		kr = xmm_lookup(memory_object, &mobj);
		if (kr != KERN_SUCCESS) {
			printf("xmm_mo_create: xmm_lookup: %x\n", kr);
			return kr;
		}
	} else {
		/*
		 * Svm_is_exception check will need to be done by xmm_export.
		 * This means we should pass back error in set attributes?
		 * Or perhaps provide fake pager which always faults,
		 * probably using xmm_invalid routines.
		 *
		 * XXX In this case svm_mobj meaningless?!
		 */
		kr = xmm_import_create(memory_object, &mobj);
		if (kr != KERN_SUCCESS) {
			printf("xmm_mo_create: xmm_import_create: %x\n", kr);
			return kr;
		}
	}

	svm_mobj = mobj;

	kr = xmm_debug_create(mobj, &mobj);
	if (kr != KERN_SUCCESS) {
		printf("xmm_mo_create: xmm_debug_create.2: %x\n", kr);
		return kr;
	}

	kr = xmm_obj_allocate(&server_class, mobj, &mobj);
	if (kr != KERN_SUCCESS) {
		printf("xmm_mo_create: xmm_server_create: %x\n", kr);
		return kr;
	}

	MOBJ->vm_object = vm_object;
	MOBJ->memory_object = memory_object;
	MOBJ->svm_mobj = svm_mobj;
	
	M_INIT(mobj, mobj, memory_object_name, page_size);
	
	vm_object->pager_request = (ipc_port_t) mobj;
	return KERN_SUCCESS;
}

memory_object_create(default_pager_server, memory_object, size, memory_control,
		     memory_object_name, page_size)
	ipc_port_t default_pager_server;
	ipc_port_t memory_object;
	vm_size_t size;
	ipc_port_t memory_control;
	ipc_port_t memory_object_name;
	vm_size_t page_size;
{
	kern_return_t kr;
	xmm_obj_t mobj, svm_mobj;
	vm_object_t vm_object;
	int po;

	kr = k_memory_object_create(default_pager_server, memory_object, size,
				    memory_control, memory_object_name,
				    page_size);
	if (kr != KERN_SUCCESS) {
		return kr;
	}

	if (ip_kotype(memory_object) != IKOT_PAGER) {
		panic("memory_object_create: not ikot_pager\n");
	}
	vm_object = (vm_object_t) memory_object->ip_kobject;

	if (ipc_port_node(memory_object) != node_self()) {
		panic("memory_object_create: not node_self\n");
	}
	kr = xmm_lookup(memory_object, &mobj);
	if (kr != KERN_SUCCESS) {
		printf("xmm_mo_create: xmm_lookup: %x\n", kr);
		return kr;
	}

	svm_mobj = mobj;

	kr = xmm_debug_create(mobj, &mobj);
	if (kr != KERN_SUCCESS) {
		printf("xmm_mo_create: xmm_debug_create.2: %x\n", kr);
		return kr;
	}

	kr = xmm_obj_allocate(&server_class, mobj, &mobj);
	if (kr != KERN_SUCCESS) {
		printf("xmm_mo_create: xmm_obj_allocate: %x\n", kr);
		return kr;
	}

	MOBJ->vm_object = vm_object;
	MOBJ->memory_object = memory_object;
	MOBJ->svm_mobj = svm_mobj;
	
	/* will go all the way down and up */
	M_INIT(mobj, mobj, memory_object_name, page_size);
	
	vm_object->pager_request = (ipc_port_t) mobj;
	return KERN_SUCCESS;
}

memory_object_terminate(mobj, kobj, memory_object_name)
	xmm_obj_t mobj;
	xmm_obj_t kobj;
	ipc_port_t memory_object_name;
{
	kern_return_t kr;

	kr = M_TERMINATE(kobj, kobj, memory_object_name);
	xmm_obj_deallocate(kobj);
	return kr;
}

/* ARGSUSED */
memory_object_copy(mobj, kobj, offset, length, new_memory_object)
	xmm_obj_t mobj;
	xmm_obj_t kobj;
	vm_offset_t offset;
	vm_size_t length;
	memory_object_t new_memory_object;
{
	/* XXX ? */
	panic("xmm_server: memory_object_copy\n");
	return KERN_FAILURE;
}

memory_object_data_request(mobj, kobj, offset, length, desired_access)
	xmm_obj_t mobj;
	xmm_obj_t kobj;
	vm_offset_t offset;
	vm_size_t length;
	vm_prot_t desired_access;
{
	return M_DATA_REQUEST(kobj, kobj, offset, length, desired_access);
}

memory_object_data_unlock(mobj, kobj, offset, length, desired_access)
	xmm_obj_t mobj;
	xmm_obj_t kobj;
	vm_offset_t offset;
	vm_size_t length;
	vm_prot_t desired_access;
{
	return M_DATA_UNLOCK(kobj, kobj, offset, length, desired_access);
}

memory_object_data_write(mobj, kobj, offset, data, length)
	xmm_obj_t mobj;
	xmm_obj_t kobj;
	vm_offset_t offset;
	char *data;
	int length;
{
	return M_DATA_WRITE(kobj, kobj, offset, data, length);
}

#ifdef	KERNEL
memory_object_data_initialize(mobj, kobj, offset, data, length)
	xmm_obj_t mobj;
	xmm_obj_t kobj;
	vm_offset_t offset;
	char *data;
	int length;
{
	/*
	 * Probably need to add M_DATA_INITIALIZE, or perhaps
	 * an 'initial' parameter to memory_object_data_write.
	 */
	panic("memory_object_data_initialize");
}
#endif	KERNEL

/*
 * XXX Consumed rights, etc.
 */
memory_object_lock_completed(reply_to, reply_to_type, kobj, offset, length)
	ipc_port_t reply_to;
	mach_msg_type_name_t reply_to_type;
	xmm_obj_t kobj;
	vm_offset_t offset;
	vm_size_t length;
{
	return M_LOCK_COMPLETED(kobj, kobj, offset, length);
}

memory_object_supply_completed(mobj, kobj, offset, length, result,
			       error_offset)
	xmm_obj_t mobj;
	xmm_obj_t kobj;
	vm_offset_t offset;
	vm_size_t length;
	kern_return_t result;
	vm_offset_t error_offset;
{
	return M_SUPPLY_COMPLETED(kobj, kobj, offset, length, result,
				  error_offset);
}

memory_object_data_return(mobj, kobj, offset, data, length)
	xmm_obj_t mobj;
	xmm_obj_t kobj;
	vm_offset_t offset;
	vm_offset_t data;
	vm_size_t length;
{
	return M_DATA_RETURN(kobj, kobj, offset, data, length);
}

memory_object_change_completed()
{
	panic("xmm_server: memory_object_change_completed called");
}

xmm_object_set(kobj, vm_object)
	xmm_obj_t kobj;
	vm_object_t vm_object;
{
	KOBJ->vm_object = vm_object;
}
