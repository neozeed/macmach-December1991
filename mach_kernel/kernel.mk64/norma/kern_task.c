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
 * $Log:	kern_task.c,v $
 * Revision 2.4  91/08/28  11:16:18  jsb
 * 	Turned off remaining printf.
 * 	[91/08/26  11:16:11  jsb]
 * 
 * 	Added support for remote task creation with inherited memory.
 * 	Remove task creation is accessible either via task_create_remote,
 * 	or task_create with task_server_node patched to some reasonable value.
 * 	[91/08/15  13:55:54  jsb]
 * 
 * Revision 2.3  91/06/17  15:48:07  jsb
 * 	Moved routines here from kern/ipc_tt.c and kern/task.c.
 * 	[91/06/17  11:01:51  jsb]
 * 
 * Revision 2.2  91/06/06  17:08:15  jsb
 * 	First checkin.
 * 	[91/05/25  11:47:39  jsb]
 * 
 */
/*
 *	File:	norma/kern_task.c
 *	Author:	Joseph S. Barrera III
 *
 *	NORMA task support.
 */

#include <norma_task.h>

#include <mach/machine/vm_types.h>
#include <mach/vm_param.h>
#include <mach/task_info.h>
#include <mach/task_special_ports.h>
#include <ipc/ipc_space.h>
#include <kern/mach_param.h>
#include <kern/task.h>
#include <kern/host.h>
#include <kern/thread.h>
#include <kern/zalloc.h>
#include <kern/kalloc.h>
#include <kern/processor.h>
#include <kern/ipc_tt.h>

ipc_port_t norma_task_server;
decl_simple_lock_data(,norma_task_server_lock)

extern zone_t task_zone;

int task_server_node = -1;

/*
 * XXX This definition should be elsewhere
 */
norma_node_self(host, node)
	host_t host;
	int *node;
{
	*node = node_self();
	return KERN_SUCCESS;
}

int mmm = 0;
int fff = 1;
int bbb = 0;

#if	NORMA_TASK

mumble(a,b,c,d,e,f,g,h,i,j,k,l,m,n)
{
	if (mmm) {
		printf(a,b,c,d,e,f,g,h,i,j,k,l,m,n);
	}
}

fret(a,b,c,d,e,f,g,h,i,j,k,l,m,n)
{
	if (fff) {
		printf(a,b,c,d,e,f,g,h,i,j,k,l,m,n);
	}
}

babble(a,b,c,d,e,f,g,h,i,j,k,l,m,n)
{
	if (bbb) {
		printf(a,b,c,d,e,f,g,h,i,j,k,l,m,n);
	}
}

kern_return_t task_create(parent_task, inherit_memory, child_task)
	task_t		parent_task;
	boolean_t	inherit_memory;
	task_t		*child_task;		/* OUT */
{
	if (task_server_node == -1) {
		return task_create_local(parent_task, inherit_memory,
					 child_task);
	} else {
		return task_create_remote(parent_task, inherit_memory,
					  task_server_node, child_task);
	}
}

/*
 * This allows us to create a task without providing a parent.
 */
kern_return_t norma_task_allocate(host, task)
	host_t	host;
	task_t	*task;		/* OUT */
{
	babble("norma_task_allocate: called...\n");
	return task_create_local(TASK_NULL, FALSE, task);
}

void
ipc_task_reinit(task, parent)
	task_t task;
	task_t parent;
{
	int i;

	if (parent == TASK_NULL) {
		task->itk_exception = IP_NULL;
		task->itk_bootstrap = IP_NULL;
		for (i = 0; i < TASK_PORT_REGISTER_MAX; i++)
			task->itk_registered[i] = IP_NULL;
	} else {
		itk_lock(parent);
		assert(parent->itk_self != IP_NULL);

		/* inherit registered ports */

		for (i = 0; i < TASK_PORT_REGISTER_MAX; i++)
			task->itk_registered[i] =
				ipc_port_copy_send(parent->itk_registered[i]);

		/* inherit exception and bootstrap ports */

		task->itk_exception =
			ipc_port_copy_send(parent->itk_exception);
		task->itk_bootstrap =
			ipc_port_copy_send(parent->itk_bootstrap);

		itk_unlock(parent);
	}
}

kern_return_t
task_get_inherited_ports(task, r0, r1, r2, r3, exception, bootstrap)
	task_t task;
	ipc_port_t *r0;
	ipc_port_t *r1;
	ipc_port_t *r2;
	ipc_port_t *r3;
	ipc_port_t *exception;
	ipc_port_t *bootstrap;
{
	if (task == TASK_NULL) {
		return KERN_INVALID_ARGUMENT;
	}
	itk_lock(task);
	*r0 = ipc_port_copy_send(task->itk_registered[0]);
	*r1 = ipc_port_copy_send(task->itk_registered[1]);
	*r2 = ipc_port_copy_send(task->itk_registered[2]);
	*r3 = ipc_port_copy_send(task->itk_registered[3]);
	*exception = ipc_port_copy_send(task->itk_exception);
	*bootstrap = ipc_port_copy_send(task->itk_bootstrap);
	itk_unlock(task);
	return KERN_SUCCESS;
}

kern_return_t
task_set_inherited_ports(task, r0, r1, r2, r3, exception, bootstrap)
	task_t task;
	ipc_port_t r0;
	ipc_port_t r1;
	ipc_port_t r2;
	ipc_port_t r3;
	ipc_port_t exception;
	ipc_port_t bootstrap;
{
	int i;

	if (task == TASK_NULL) {
		return KERN_INVALID_ARGUMENT;
	}
	itk_lock(task);
	for (i = 0; i < 4; i++) {
		if (IP_VALID(task->itk_registered[i])) {
			ipc_port_release_send(task->itk_registered[i]);
		}
	}
	if (IP_VALID(task->itk_exception)) {
		ipc_port_release_send(task->itk_exception);
	}
	if (IP_VALID(task->itk_bootstrap)) {
		ipc_port_release_send(task->itk_bootstrap);
	}
	task->itk_registered[0] = r0;
	task->itk_registered[1] = r1;
	task->itk_registered[2] = r2;
	task->itk_registered[3] = r3;
	task->itk_exception = exception;
	task->itk_bootstrap = bootstrap;
	itk_unlock(task);
	return KERN_SUCCESS;
}

kern_return_t task_create_remote(parent_task, inherit_memory, child_node,
				 child_task)
	task_t		parent_task;
	boolean_t	inherit_memory;
	int		child_node;
	task_t		*child_task;		/* OUT */
{
	ipc_port_t remote_task;
	task_t new_task;
	kern_return_t kr;
	int vector_start;
	unsigned int entry_vector_count;
	mach_port_t r0, r1, r2, r3, exception, bootstrap;
	vm_offset_t *entry_vector;

	babble("task_create: doing %d -> %d\n", node_self(), child_node);
	kr = r_norma_task_allocate(host_port_at_node(child_node),
				   &remote_task);
	if (kr != KERN_SUCCESS) {
		return kr;
	}

	entry_vector = (vm_offset_t *) kalloc(1024 * sizeof(entry_vector));
	entry_vector_count = 1024;

	kr = task_get_emulation_vector(parent_task, &vector_start,
				       entry_vector, &entry_vector_count);
	if (kr != KERN_SUCCESS) {
		printf("task_get_emulation_vector failed: kr %d %x\n", kr, kr);
		return kr;
	}

	kr = r_task_set_emulation_vector(remote_task, vector_start,
					 entry_vector, entry_vector_count);
	if (kr != KERN_SUCCESS) {
		printf("task_set_emulation_vector failed: kr %d %x\n", kr, kr);
		return kr;
	}

	kfree(entry_vector, 1024 * sizeof(entry_vector));

	kr = task_get_inherited_ports(parent_task, &r0, &r1, &r2, &r3,
				      &exception, &bootstrap);
	if (kr != KERN_SUCCESS) {
		printf("task_get_inherited_ports failed: kr %d %x\n", kr, kr);
		return kr;
	}

	mumble("%x %x %x %x %x %x\n", r0, r1, r2, r3, exception, bootstrap);
	kr = r_task_set_inherited_ports(remote_task, r0, r1, r2, r3,
					exception, bootstrap);
	if (kr != KERN_SUCCESS) {
		printf("task_set_inherited_ports failed: kr %d %x\n", kr, kr);
		return kr;
	}

	if (inherit_memory) {
		kr = task_copy_vm(parent_task->map, remote_task);
		mumble("task_create: task_copy_vm: (%x)\n", kr);
		if (kr != KERN_SUCCESS) {
			return kr;
		}
	}

	/*
	 * Create a placeholder task for the benefit of convert_task_to_port.
	 * Set new_task->map to VM_MAP_NULL so that task_deallocate will
	 * know that this is only a placeholder task.
	 * XXX decr send-right count?
	 */
	new_task = (task_t) zalloc(task_zone);
	if (new_task == TASK_NULL) {
		panic("task_create: no memory for task structure");
	}

	/* only one ref, for our caller */
	new_task->ref_count = 1;

	new_task->map = VM_MAP_NULL;
	new_task->itk_self = remote_task;
	simple_lock_init(&new_task->lock);

	babble("task_create: did   %d -> %d\n", node_self(), child_node);

	*child_task = new_task;
	return(KERN_SUCCESS);
}

task_copy_vm(from0, to)
	vm_map_t from0;
	ipc_port_t to;
{
	vm_offset_t address;
	vm_size_t size;
	vm_prot_t protection, max_protection;
	vm_inherit_t inheritance;
	boolean_t shared;
	port_t object_name;
	vm_offset_t offset;
	kern_return_t kr;
	vm_map_t from;
	ipc_port_t memory_object;
	
	from = vm_map_fork(from0);
	if (from == VM_MAP_NULL) {
		panic("task_copy_vm: vm_map_fork\n");
	}
	for (address = 0;; address += size) {
		kr = vm_region(from, &address, &size, &protection,
			       &max_protection, &inheritance, &shared,
			       &object_name, &offset);
		if (kr == KERN_NO_SPACE) {
			return KERN_SUCCESS;
		}
		switch (inheritance) {
			case VM_INHERIT_NONE:
			break;

			case VM_INHERIT_SHARE:
			fret("task_copy_vm: VM_INHERIT_SHARE!\n");
			return KERN_FAILURE;

			case VM_INHERIT_COPY:
			kr = norma_copy_create(from, address, size,
					       &memory_object);
			if (kr != KERN_SUCCESS) {
				fret("task_cv: copy_create: %d 0x%x\n",
				     kr, kr);
				return kr;
			}
			kr = r_vm_map(to, &address, size, 0, FALSE,
				      memory_object, 0, FALSE,
				      protection, max_protection,
				      inheritance);
			if (kr != KERN_SUCCESS) {
				fret("task_cv: vm_map: %d 0x%x\n",
				     kr, kr);
				return kr;
			}
			break;

			default:
			panic("task_copy_vm: inheritance=%d!\n",
			      inheritance);
		}
	}
}
#else	NORMA_TASK
task_create_local()
{
}

norma_task_allocate()
{
}

task_get_inherited_ports()
{
}

task_set_inherited_ports()
{
}

task_create_remote()
{
}

norma_copy_create()
{
}

#endif	NORMA_TASK
