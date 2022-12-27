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
 * $Log:	xmm_user.c,v $
 * Revision 2.4  91/08/28  11:16:30  jsb
 * 	Added definition for xxx_memory_object_lock_request, and temporary
 * 	stubs for data_supply, object_ready, and change_attributes.
 * 	[91/08/16  14:22:37  jsb]
 * 
 * 	Added check for internal memory objects to xmm_user_create.
 * 	[91/08/15  10:14:19  jsb]
 * 
 * Revision 2.3  91/07/01  08:26:46  jsb
 * 	Collect garbage. Support memory_object_create.
 * 	Disassociate kobj from memory_control before calling
 * 	memory_object_terminate to prevent upcalls on terminated kobj.
 * 	[91/06/29  15:51:50  jsb]
 * 
 * Revision 2.2  91/06/17  15:48:48  jsb
 * 	Renamed xmm_vm_object_lookup.
 * 	[91/06/17  13:20:06  jsb]
 * 
 * 	First checkin.
 * 	[91/06/17  11:02:47  jsb]
 * 
 */
/*
 *	File:	norma/xmm_user.c
 *	Author:	Joseph S. Barrera III
 *	Date:	1991
 *
 *	Interface between memory managers and xmm system.
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

/*
 * Since we ALWAYS have an SVM module above us,
 * we NEVER have more than one memory_control per memory_object.
 * Thus we can combine mobj and kobj.
 */

struct mobj {
	struct xmm_obj	obj;
	mach_port_t	memory_object;
	mach_port_t	memory_control;
	boolean_t	initialized;
};

#undef  KOBJ
#define KOBJ    ((struct mobj *) kobj)

kern_return_t m_user_init();
kern_return_t m_user_terminate();
kern_return_t m_user_copy();
kern_return_t m_user_data_request();
kern_return_t m_user_data_unlock();
kern_return_t m_user_data_write();
kern_return_t m_user_lock_completed();
kern_return_t m_user_supply_completed();
kern_return_t m_user_data_return();

xmm_decl(user_class,
	/* m_init		*/	m_user_init,
	/* m_terminate		*/	m_user_terminate,
	/* m_copy		*/	m_user_copy,
	/* m_data_request	*/	m_user_data_request,
	/* m_data_unlock	*/	m_user_data_unlock,
	/* m_data_write		*/	m_user_data_write,
	/* m_lock_completed	*/	m_user_lock_completed,
	/* m_supply_completed	*/	m_user_supply_completed,
	/* m_data_return	*/	m_user_data_return,

	/* k_data_provided	*/	k_invalid_data_provided,
	/* k_data_unavailable	*/	k_invalid_data_unavailable,
	/* k_get_attributes	*/	k_invalid_get_attributes,
	/* k_lock_request	*/	k_invalid_lock_request,
	/* k_data_error		*/	k_invalid_data_error,
	/* k_set_attributes	*/	k_invalid_set_attributes,
	/* k_destroy		*/	k_invalid_destroy,
	/* k_data_supply	*/	k_invalid_data_supply,

	/* name			*/	"user",
	/* size			*/	sizeof(struct mobj)
);

xmm_obj_t
user_kobj_by_memory_control(memory_control)
	mach_port_t memory_control;
{
	if (ip_kotype((ipc_port_t) memory_control) == IKOT_NONE) {
		printf("null kobj for memory_control\n");
		return XMM_OBJ_NULL;
	}
	if (ip_kotype((ipc_port_t) memory_control) != IKOT_PAGING_REQUEST) {
		panic("xx type = %x\n", memory_control);
		panic("xx type = %d\n", ip_kotype((ipc_port_t)memory_control));
	}
	return (xmm_obj_t) ((ipc_port_t) memory_control)->ip_kobject;
}

vm_object_t
xmm_vm_object_lookup(memory_control)
	mach_port_t memory_control;
{
	xmm_obj_t kobj;
	ipc_port_t memory_object_port;
	vm_object_t object;

	kobj = user_kobj_by_memory_control(memory_control);
	if (kobj == XMM_OBJ_NULL) {
		return VM_OBJECT_NULL;
	}
	memory_object_port = (ipc_port_t) KOBJ->memory_object;
	if (ip_kotype(memory_object_port) != IKOT_PAGER) {
		return VM_OBJECT_NULL;
	}
	object = (vm_object_t) memory_object_port->ip_kobject;
	vm_object_reference(object);
	return object;
}

/*
 * XXX should first home be original mapper?
 */
kern_return_t
xmm_user_create(memory_object, new_mobj)
	mach_port_t memory_object;
	xmm_obj_t *new_mobj;
{
	xmm_obj_t mobj;
	kern_return_t kr;
	vm_object_t object;
#if 3
	mach_port_t memory_control;
#endif
#if 2
	/*
	 * XXX
	 * See note in xmm_lookup. Check for internal objects
	 * should be done elsewhere.
	 */
	if (is_internal_memory_object(memory_object, new_mobj)) {
		return KERN_SUCCESS;
	}
#endif
#if 3
	/*
	 *	Allocate request port.
	 */
	memory_control = (memory_object_t) ipc_port_alloc_kernel();
	if (memory_control == MACH_PORT_NULL) {
		panic("vm_object_enter: pager request alloc");
	}
#endif
	kr = xmm_obj_allocate(&user_class, XMM_OBJ_NULL, &mobj);
	if (kr != KERN_SUCCESS) {
		return kr;
	}
	MOBJ->memory_object = memory_object;
	MOBJ->memory_control = memory_control;
	MOBJ->initialized = FALSE;
	ipc_kobject_set((ipc_port_t) memory_control,
			(ipc_kobject_t) mobj,
			IKOT_PAGING_REQUEST);
#if 0
printf("m_user_create: >> 0x%x, 0x%x\n", memory_object, memory_control);
#endif
	*new_mobj = mobj;
	return KERN_SUCCESS;
}

m_user_init(mobj, k_kobj, memory_object_name, page_size)
	xmm_obj_t mobj;
	xmm_obj_t k_kobj;
	mach_port_t memory_object_name;
	vm_size_t page_size;
{
	kern_return_t kr;
	xmm_obj_t kobj = mobj;
	
	if (MOBJ->initialized) {
		panic("m_user_init: initialized!\n");
	}
	MOBJ->initialized = TRUE;

	k_kobj->m_kobj = kobj;
	kobj->k_kobj = k_kobj;

#if 777
{
	ipc_port_t memory_object_port = (ipc_port_t) MOBJ->memory_object;
	vm_object_t object;

	if (ip_kotype(memory_object_port) == IKOT_PAGER) {
		object = (vm_object_t) memory_object_port->ip_kobject;
		if (object && object->internal) {
			return K_SET_ATTRIBUTES(mobj, TRUE, TRUE, 0);
		}
	}
}	
#endif
	
#if 666
	if (! MOBJ->memory_object) {
		/* XXX A hack for xmm_map_copyin */
		K_SET_ATTRIBUTES(mobj, TRUE, TRUE, 0);
		return;
	}
#endif
	kr = memory_object_init(MOBJ->memory_object,
			   MOBJ->memory_control,
#if 0
			   memory_object_name,
#else
			   MOBJ->memory_control,	/* XXX */
#endif
			   page_size);
	if (kr) {
		panic("m_user_init: moi returns %x\n", kr);
	}
	return kr;
}

m_user_terminate(mobj, kobj, memory_object_name)
	xmm_obj_t mobj;
	xmm_obj_t kobj;
	mach_port_t memory_object_name;
{
	kern_return_t kr;

	ipc_kobject_set(KOBJ->memory_control, IKO_NULL, IKOT_NONE);
	kr = memory_object_terminate(MOBJ->memory_object,
				     KOBJ->memory_control,
				     memory_object_name);
	xmm_obj_deallocate(mobj);
	return kr;
}

m_user_copy(mobj, kobj, offset, length, new_memory_object)
	xmm_obj_t mobj;
	xmm_obj_t kobj;
	vm_offset_t offset;
	vm_size_t length;
	mach_port_t new_memory_object;
{
	return memory_object_copy(MOBJ->memory_object,
				  KOBJ->memory_control,
				  offset,
				  length,
				  new_memory_object);
}

m_user_data_request(mobj, kobj, offset, length, desired_access)
	xmm_obj_t mobj;
	xmm_obj_t kobj;
	vm_offset_t offset;
	vm_size_t length;
	vm_prot_t desired_access;
{
	return memory_object_data_request(MOBJ->memory_object,
					  KOBJ->memory_control,
					  offset,
					  length,
					  desired_access);
}

m_user_data_unlock(mobj, kobj, offset, length, desired_access)
	xmm_obj_t mobj;
	xmm_obj_t kobj;
	vm_offset_t offset;
	vm_size_t length;
	vm_prot_t desired_access;
{
	return memory_object_data_unlock(MOBJ->memory_object,
					 KOBJ->memory_control,
					 offset,
					 length,
					 desired_access);
}

m_user_data_write(mobj, kobj, offset, data, length)
	xmm_obj_t mobj;
	xmm_obj_t kobj;
	vm_offset_t offset;
	vm_offset_t data;
	int length;
{
	return memory_object_data_write(MOBJ->memory_object,
					KOBJ->memory_control,
					offset,
					data,
					length);
}

m_user_lock_completed(mobj, kobj, offset, length)
	xmm_obj_t mobj;
	xmm_obj_t kobj;
	vm_offset_t offset;
	vm_size_t length;
{
	return memory_object_lock_completed(MOBJ->memory_object,
					    MACH_MSG_TYPE_MOVE_SEND_ONCE,
					    KOBJ->memory_control,
					    offset,
					    length);
}

m_user_supply_completed(mobj, kobj, offset, length, result, error_offset)
	xmm_obj_t mobj;
	xmm_obj_t kobj;
	vm_offset_t offset;
	vm_size_t length;
	kern_return_t result;
	vm_offset_t error_offset;
{
	return memory_object_supply_completed(MOBJ->memory_object,
					      KOBJ->memory_control,
					      offset,
					      length,
					      result,
					      error_offset);
}

m_user_data_return(mobj, kobj, offset, data, length)
	xmm_obj_t mobj;
	xmm_obj_t kobj;
	vm_offset_t offset;
	vm_offset_t data;
	vm_size_t length;
{
	return memory_object_data_return(MOBJ->memory_object,
					 KOBJ->memory_control,
					 offset,
					 data,
					 length);
}

memory_object_data_provided(memory_control, offset, data, length, lock_value)
	mach_port_t	memory_control;
	vm_offset_t	offset;
	pointer_t	data;
	unsigned int	length;
	vm_prot_t	lock_value;
{
	xmm_obj_t kobj;
	
	kobj = user_kobj_by_memory_control(memory_control);
	if (kobj == XMM_OBJ_NULL) {
		return KERN_INVALID_ARGUMENT;
	}
	return K_DATA_PROVIDED(kobj, offset, data, length, lock_value);
}

memory_object_data_unavailable(memory_control, offset, length)
	mach_port_t	memory_control;
	vm_offset_t	offset;
	vm_size_t	length;
{
	xmm_obj_t kobj;
	
	kobj = user_kobj_by_memory_control(memory_control);
	if (kobj == XMM_OBJ_NULL) {
		return KERN_INVALID_ARGUMENT;
	}
	return K_DATA_UNAVAILABLE(kobj, offset, length);
}

memory_object_get_attributes(memory_control, object_ready, may_cache,
			     copy_strategy)
	mach_port_t	memory_control;
	boolean_t	*object_ready;
	boolean_t	*may_cache;
	memory_object_copy_strategy_t *copy_strategy;
{
	xmm_obj_t kobj;
	
	kobj = user_kobj_by_memory_control(memory_control);
	if (kobj == XMM_OBJ_NULL) {
		return KERN_INVALID_ARGUMENT;
	}
	return K_GET_ATTRIBUTES(kobj, object_ready, may_cache, copy_strategy);
}

xxx_memory_object_lock_request(memory_control, offset, length, should_clean,
			       should_flush, prot, reply_to, reply_to_type)
	mach_port_t	memory_control;
	vm_offset_t	offset;
	vm_size_t	length;
	boolean_t	should_clean;
	boolean_t	should_flush;
	vm_prot_t	prot;
	mach_port_t	reply_to;
	/*mach_msg_type_name_t*/int	reply_to_type;
{
	return memory_object_lock_request(memory_control, offset, length,
					  should_clean, should_flush, prot,
					  reply_to, reply_to_type);
}

/*
 * XXX
 * What we really need here, and xmm_svm could use it as well,
 * is a routine which takes a kobj and a (reply_to, reply_to_type) pair
 * and returns an robj, which may be mobj and which may be a new robj.
 */

memory_object_lock_request(memory_control, offset, length, should_reply,
			   should_flush, prot, reply_to, reply_to_type)
	mach_port_t	memory_control;
	vm_offset_t	offset;
	vm_size_t	length;
	int		should_reply;
	boolean_t	should_flush;
	vm_prot_t	prot;
	mach_port_t	reply_to;
	/*mach_msg_type_name_t*/int	reply_to_type;
{
	xmm_obj_t kobj;
	xmm_obj_t robj;
	
#if     lint
	reply_to_type++;
#endif  lint
	kobj = user_kobj_by_memory_control(memory_control);
	if (kobj == XMM_OBJ_NULL) {
		return KERN_INVALID_ARGUMENT;
	}
	if (reply_to == MACH_PORT_NULL) {
		robj = XMM_OBJ_NULL;
	} else if (reply_to == KOBJ->memory_object) {
		if (reply_to_type != MACH_MSG_TYPE_MOVE_SEND_ONCE) {
			panic("xmm lock_request: unknown reply_to_type %d\n",
			      reply_to_type);
		}
		robj = kobj;
	} else {
		/* XXX should cons up a new obj */
		panic("xmm lock_request\n");
	}
	return K_LOCK_REQUEST(kobj, offset, length, should_reply, should_flush,
			      prot, robj);
}

memory_object_data_error(memory_control, offset, length, error_value)
	mach_port_t	memory_control;
	vm_offset_t	offset;
	vm_size_t	length;
	kern_return_t	error_value;
{
	xmm_obj_t kobj;
	
	kobj = user_kobj_by_memory_control(memory_control);
	if (kobj == XMM_OBJ_NULL) {
		return KERN_INVALID_ARGUMENT;
	}
	return K_DATA_ERROR(kobj, offset, length, error_value);
}

memory_object_set_attributes(memory_control, object_ready, may_cache,
			     copy_strategy)
	mach_port_t	memory_control;
	boolean_t	object_ready;
	boolean_t	may_cache;
	memory_object_copy_strategy_t copy_strategy;
{
	xmm_obj_t kobj;
	
	kobj = user_kobj_by_memory_control(memory_control);
	if (kobj == XMM_OBJ_NULL) {
		return KERN_INVALID_ARGUMENT;
	}
	return K_SET_ATTRIBUTES(kobj, object_ready, may_cache, copy_strategy);
}

memory_object_destroy(memory_control, reason)
	mach_port_t	memory_control;
	kern_return_t	reason;
{
	xmm_obj_t kobj;
	
	kobj = user_kobj_by_memory_control(memory_control);
	if (kobj == XMM_OBJ_NULL) {
		return KERN_INVALID_ARGUMENT;
	}
	return K_DESTROY(kobj, reason);
}

memory_object_data_supply()
{
	panic("memory_object_data_supply: called!\n");
}

memory_object_ready()
{
	panic("memory_object_ready: called!\n");
}

memory_object_change_attributes()
{
	panic("memory_object_change_attributes: called!\n");
}
