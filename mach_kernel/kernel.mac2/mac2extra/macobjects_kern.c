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
 * $Log:	macobjects_kern.c,v $
 * Revision 2.2  91/09/12  16:49:00  bohman
 * 	Created.
 * 	[91/09/11  16:41:26  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2extra/macobjects_kern.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <mach/mach_types.h>

#include <ipc/ipc_port.h>
#include <ipc/ipc_space.h>

#include <device/device_port.h>

#include <vm/vm_kern.h>
#include <vm/vm_page.h>

/*
 * Routines dealing with the
 * creation of the memory objects
 * which contain copies of the
 * system heap and the high memory.
 */

static ipc_space_t	kern_space;

void
macobjects_kern_space_init()
{
    if (kern_space == (ipc_space_t)0)
	kern_space = current_space();
}

void
macobjects_device_port(port)
ipc_port_t	*port;
{
    *port = ipc_port_make_send(master_device_port);
}

boolean_t
macobject_copyin(object, port)
register memory_object_t	object;
register ipc_port_t		*port;
{
    return (ipc_object_copyin(kern_space,
			      object,
			      MACH_MSG_TYPE_COPY_SEND,
			      (ipc_object_t *) port) == KERN_SUCCESS);
}

boolean_t
macobject_kern_copy(memory_object, kern, size)
register memory_object_t	memory_object;
register vm_offset_t		kern;
register vm_size_t		size;
{
    register vm_object_t	object;
    register vm_page_t		page;
    register vm_offset_t	offset = 0;
    ipc_port_t			port;
    vm_offset_t			buffer;

    kern = trunc_page(kern);
    size = round_page(size);

    if (!macobject_copyin(memory_object, &port))
	return (FALSE);

    object = vm_object_enter(port, size, FALSE);
    if (IP_VALID(port))
	ipc_port_release_send(port);

    if (object == VM_OBJECT_NULL)
	return (FALSE);

    if (vm_allocate(kernel_map, &buffer, PAGE_SIZE, TRUE) != KERN_SUCCESS)
	goto destroy;

    vm_object_lock(object);

    for (; size > 0; offset += PAGE_SIZE, size -= PAGE_SIZE) {
	if ((page = vm_page_alloc(object, offset)) == VM_PAGE_NULL) {
	    vm_object_unlock(object);
	    goto dealloc_and_destroy;
	}

	PMAP_ENTER(kernel_pmap,
		   buffer,
		   page,
		   VM_PROT_READ|VM_PROT_WRITE,
		   FALSE);

	bcopy(kern + offset,
	      buffer,
	      PAGE_SIZE);

	page->busy = FALSE;

	page->dirty = TRUE;

	PMAP_ENTER(kernel_pmap,
		   buffer,
		   page,
		   VM_PROT_NONE,
		   FALSE);
    }

    (void) vm_object_deactivate_pages(object);

    vm_object_unlock(object);

    (void) vm_deallocate(kernel_map, buffer, PAGE_SIZE);

    return (TRUE);

dealloc_and_destroy:
    (void) vm_deallocate(kernel_map, buffer, PAGE_SIZE);

destroy:
    (void) memory_object_destroy(object, KERN_FAILURE);

    return (TRUE);
}
