/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	allocator.c,v $
 * Revision 2.4  90/06/02  15:20:11  rpd
 * 	Removed vm_allocate_with_pager.
 * 	Added reply_port argument to vm_map.
 * 	[90/04/08            rpd]
 * 
 * Revision 2.3  90/05/29  20:22:22  rwd
 * 	if anywhere in vm_allocate set addr to EMULATOR_BASE
 * 	[90/05/29            rwd]
 * 
 * Revision 2.2  89/11/29  15:26:01  af
 * 	RCS-ed.
 * 	[89/11/29            af]
 * 
 */
/*
 * Replacement for vm_allocate, to keep memory within emulator address space.
 */

#ifdef mac2 /* don't want the vm_map definition in mach_interface.h */
#define vm_map xvm_map
#include <mach/mach.h>
#include <machine/vmparam.h>
#undef vm_map
#else
#include <mach/mach.h>
#include <machine/vmparam.h>
#endif

#ifdef	mips
#define VM_PROT_DEFAULT (VM_PROT_READ|VM_PROT_WRITE)
#endif	mips

extern mach_port_t mig_get_reply_port();

kern_return_t
vm_allocate(task, addr, size, anywhere)
	task_t		task;
	vm_offset_t	*addr;
	vm_size_t	size;
	boolean_t	anywhere;
{
	if (anywhere)
	    *addr = EMULATOR_BASE;

	return (htg_vm_map(task, mig_get_reply_port(),
			addr, size, (vm_offset_t)0, anywhere,
			MEMORY_OBJECT_NULL, (vm_offset_t)0, FALSE,
			VM_PROT_DEFAULT, VM_PROT_ALL, VM_INHERIT_COPY));
}

kern_return_t
vm_map(task, reply_port, address, size, mask, anywhere,
	memory_object, offset, copy,
	cur_protection, max_protection, inheritance)

	vm_task_t	task;
	mach_port_t	reply_port;
	vm_address_t	*address;
	vm_size_t	size;
	vm_address_t	mask;
	boolean_t	anywhere;
	memory_object_t	memory_object;
	vm_offset_t	offset;
	boolean_t	copy;
	vm_prot_t	cur_protection;
	vm_prot_t	max_protection;
	vm_inherit_t	inheritance;
{
	if (anywhere && *address < EMULATOR_BASE)
	    *address = EMULATOR_BASE;

	return (htg_vm_map(task, reply_port, address, size, mask, anywhere,
			memory_object, offset, copy,
			cur_protection, max_protection, inheritance));
}
