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
 * $Log:	mach_syscalls.c,v $
 * Revision 2.8  91/06/25  10:34:48  rpd
 * 	Check against MACH_SEND_INTERRUPTED instead of KERN_SUCCESS.
 * 	[91/05/20            rpd]
 * 
 * Revision 2.7  91/05/14  17:53:28  mrt
 * 	Correcting copyright
 * 
 * Revision 2.6  91/02/14  14:17:54  mrt
 * 	Added new Mach copyright
 * 	[91/02/13  12:44:29  mrt]
 * 
 * Revision 2.5  90/09/09  14:34:31  rpd
 * 	Added mach_port_allocate_name since it didn't exist because of
 * 	the makefile sed.
 * 	[90/09/03            rwd]
 * 
 * Revision 2.4  90/06/19  23:03:50  rpd
 * 	Added mach_port_allocate, mach_port_deallocate, mach_port_insert_right.
 * 	[90/06/02            rpd]
 * 
 * Revision 2.3  90/06/02  15:12:36  rpd
 * 	Try kernel call whenever syscall fails.
 * 	[90/05/31            rpd]
 * 
 * 	Updated for new IPC.
 * 	Removed vm_allocate_with_pager.
 * 	[90/05/31            rpd]
 * 
 * Revision 2.2  90/05/29  18:40:03  rwd
 * 	New file from rfr to try traps then mig.
 * 	[90/04/20            rwd]
 * 
 */

#include <mach/mach.h>
#include <mach/message.h>

kern_return_t task_create(target_task, inherit_memory, child_task)
	task_t target_task;
	boolean_t inherit_memory;
	task_t *child_task;
{
	kern_return_t result;

	result = syscall_task_create(target_task, inherit_memory, child_task);
	if (result == MACH_SEND_INTERRUPTED)
		result = mig_task_create(target_task, inherit_memory,
					 child_task);
	return(result);
}

kern_return_t task_terminate(target_task)
	task_t target_task;
{
	kern_return_t result;

	result = syscall_task_terminate(target_task);
	if (result == MACH_SEND_INTERRUPTED)
		result = mig_task_terminate(target_task);
	return(result);
}

kern_return_t task_suspend(target_task)
	task_t target_task;
{
	kern_return_t result;

	result = syscall_task_suspend(target_task);
	if (result == MACH_SEND_INTERRUPTED)
		result = mig_task_suspend(target_task);
	return(result);
}

kern_return_t task_set_special_port(task, which_port, special_port)
	task_t task;
	int which_port;
	mach_port_t special_port;
{
	kern_return_t result;

	result = syscall_task_set_special_port(task, which_port, special_port);
	if (result == MACH_SEND_INTERRUPTED)
		result = mig_task_set_special_port(task, which_port, 
					           special_port);
	return(result);
}

kern_return_t vm_allocate(target_task, address, size, anywhere)
	vm_task_t target_task;
	vm_address_t *address;
	vm_size_t size;
	boolean_t anywhere;
{
	kern_return_t result;

	result = syscall_vm_allocate(target_task, address, size, anywhere);
	if (result == MACH_SEND_INTERRUPTED)
		result = mig_vm_allocate(target_task,address, size, anywhere);
	return(result);
}

kern_return_t vm_deallocate(target_task, address, size)
	vm_task_t target_task;
	vm_address_t address;
	vm_size_t size;
{
	kern_return_t result;

	result = syscall_vm_deallocate(target_task, address, size);
	if (result == MACH_SEND_INTERRUPTED)
		result = mig_vm_deallocate(target_task,address, size);
	return(result);
}

kern_return_t vm_map
	(target_task, address, size, mask, anywhere, memory_object, offset, copy, cur_protection, max_protection, inheritance)
	vm_task_t target_task;
	vm_address_t *address;
	vm_size_t size;
	vm_address_t mask;
	boolean_t anywhere;
	memory_object_t memory_object;
	vm_offset_t offset;
	boolean_t copy;
	vm_prot_t cur_protection;
	vm_prot_t max_protection;
	vm_inherit_t inheritance;
{
	kern_return_t result;

	result = syscall_vm_map(target_task, address, size, mask, anywhere,
				memory_object, offset, copy,
				cur_protection, max_protection, inheritance);
	if (result == MACH_SEND_INTERRUPTED)
		result = mig_vm_map(target_task, address, size, mask, anywhere,
				memory_object, offset, copy,
				cur_protection, max_protection, inheritance);
	return(result);
}

kern_return_t
mach_port_allocate(task, right, namep)
	task_t task;
	mach_port_right_t right;
	mach_port_t *namep;
{
	kern_return_t kr;

	kr = syscall_mach_port_allocate(task, right, namep);
	if (kr == MACH_SEND_INTERRUPTED)
		kr = mig_mach_port_allocate(task, right, namep);

	return kr;
}

kern_return_t
mach_port_allocate_name(task, right, namep)
	task_t task;
	mach_port_right_t right;
	mach_port_t namep;
{
	kern_return_t kr;

	kr = syscall_mach_port_allocate_name(task, right, namep);
	if (kr == MACH_SEND_INTERRUPTED)
		kr = mig_mach_port_allocate_name(task, right, namep);

	return kr;
}

kern_return_t
mach_port_deallocate(task, name)
	task_t task;
	mach_port_t name;
{
	kern_return_t kr;

	kr = syscall_mach_port_deallocate(task, name);
	if (kr == MACH_SEND_INTERRUPTED)
		kr = mig_mach_port_deallocate(task, name);

	return kr;
}

kern_return_t
mach_port_insert_right(task, name, right, rightType)
	task_t task;
	mach_port_t name;
	mach_port_t right;
	mach_msg_type_name_t rightType;
{
	kern_return_t kr;

	kr = syscall_mach_port_insert_right(task, name, right, rightType);
	if (kr == MACH_SEND_INTERRUPTED)
		kr = mig_mach_port_insert_right(task, name, right, rightType);

	return kr;
}
