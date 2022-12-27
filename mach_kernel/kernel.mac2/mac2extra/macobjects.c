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
 * $Log:	macobjects.c,v $
 * Revision 2.2  91/09/12  16:48:49  bohman
 * 	Created.
 * 	[91/09/11  16:40:33  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2extra/macobjects.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <mach/mach_types.h>
#include <mach/vm_param.h>

#include <kern/macro_help.h>
#include <kern/task.h>
#include <kern/thread.h>

#include <mac2os/Globals.h>
#include <mac2os/Types.h>
#include <mac2os/Errors.h>

#include <mach/mach_user_internal.h>
#include <mach/mach_port_internal.h>

#define default_pager_object_create \
    default_pager_object_create_EXTERNAL

/*
 * Executed by the bootstrap to create
 * the memory objects used to run the
 * Macintosh Operating System inside
 * a user task.
 *
 * The user mode macintosh emulator
 * is currently started from within
 * the UX server.  Eventually, it should
 * be a stand-alone Mach application that
 * is started directly when the kernel is
 * booted.  An extra task is created here
 * which initializes the memory objects and then
 * goes to sleep.  In the new scheme, this
 * task would actually run the macintosh server.
 */

void
macobjects_bootstrap(bootstrap_host_priv_port)
mach_port_t	bootstrap_host_priv_port;
{
    register task_t	new_task;
    kern_return_t	result;
    void		macobjects_main();

    result = task_set_bootstrap_port(mach_task_self(),
				     bootstrap_host_priv_port);
    if (result != KERN_SUCCESS) {
	printf("macobject_bootstrap: Cannot set bootstrap port %x\n", result);
	return;
    }

    new_task = kernel_task_create(current_task(), (vm_size_t)0);
    if (new_task == TASK_NULL) {
	printf("macobject_bootstrap: Cannot create task\n");
	return;
    }

    (void) kernel_thread(new_task, macobjects_main, 0);
}

static mach_port_t	host_priv_port;

static memory_object_t	kern_low, kern_high;
static vm_offset_t	kern_low_offset, kern_high_offset;
static vm_size_t	kern_low_size, kern_high_size;

#define stop_thread() \
MACRO_BEGIN				\
    int		forever;		\
\
    assert_wait((int)&forever, FALSE);	\
    thread_block((void (*)()) 0);	\
MACRO_END

#define swtch_thread() \
MACRO_BEGIN						\
    thread_block((void (*)()) 0);			\
MACRO_END

void
macobjects_create();

void
macobjects_main()
{
    register kern_return_t	result;
    mach_port_t			default_pager_port;
    register			again = 0;
    
    result = task_get_bootstrap_port(mach_task_self(),
				     &host_priv_port);
    if (result != KERN_SUCCESS) {
	printf("macobjects_main: Cannot get bootstrap port %x\n", result);
	stop_thread();
    }

    do {
	if (again++ > 0) swtch_thread();
	default_pager_port = MACH_PORT_NULL;
	result = vm_set_default_memory_manager(host_priv_port,
					       &default_pager_port);
	if (result != KERN_SUCCESS) {
	    printf("macobjects_main: Cannot get default pager port %x\n",
		   result);
	    stop_thread();
	}
    } while (default_pager_port == MACH_PORT_NULL);

    (void) macobjects_create(default_pager_port);

    /*
     * Setup the ADB device table.
     */
    adb_setup();

    /*
     * Install the slot VBL tasks.
     */
    slot_tasks_install();

    stop_thread();
}

void
macobjects_create(pager)
mach_port_t	pager;
{
    register kern_return_t	result;
    register vm_offset_t	offset;
    register vm_size_t		size;
#ifdef MODE24
    extern boolean_t		mac32bit;
#endif /* MODE24 */

    (void) macobjects_kern_space_init();

    offset = 0;
    size = round_page(ApplZone);
    result = default_pager_object_create(pager,
					 &kern_low,
					 size);
    if (result == KERN_SUCCESS
	&& macobject_kern_copy(kern_low, offset, size)) {

	printf("kern_low: offset %x size %x\n", offset, size);
	kern_low_offset = offset;
	kern_low_size = size;
    }

    offset = trunc_page(BufPtr);
#ifdef MODE24
    if (mac32bit)
	size = MemTop - offset;
    else {
	if (MemTop > 0x800000)
	    size = 0x800000 - offset;
	else
	    size = MemTop - offset;
    }
#else /* MODE24 */
    size = MemTop - offset;
#endif /* MODE24 */
    size = trunc_page(size);	/* XXX */
    result = default_pager_object_create(pager,
					 &kern_high,
					 size);
    if (result == KERN_SUCCESS
	&& macobject_kern_copy(kern_high, offset, size)) {

	printf("kern_high: offset %x size %x\n", offset, size);
	kern_high_offset = offset;
	kern_high_size = size;
    }
}

kern_return_t
macserver_KernelObjects(mach_port_t	request_port,
			boolean_t	*_mac32bit,
			memory_object_t	*low_object,
			vm_offset_t	*low_offset,
			vm_size_t	*low_size,
			memory_object_t	*high_object,
			vm_offset_t	*high_offset,
			vm_size_t	*high_size)
{
#ifdef MODE24
    extern boolean_t	mac32bit;
#else /* MODE24 */
    static boolean_t	mac32bit = TRUE;
#endif /* MODE24 */

    *_mac32bit = mac32bit;

    if (macobject_copyin(kern_low, low_object)) {
	*low_offset = kern_low_offset;
	*low_size = kern_low_size;
    }
    else {
	*low_offset = 0;
	*low_size = 0;
    }

    if (macobject_copyin(kern_high, high_object)) {
	*high_offset = kern_high_offset;
	*high_size = kern_high_size;
    }
    else {
	*high_offset = 0;
	*high_size = 0;
    }

    return (KERN_SUCCESS);
}

kern_return_t
macserver_DeviceMaster(mach_port_t	request_port,
		       mach_port_t	*master_port)
{
    macobjects_device_port(master_port);

    return (KERN_SUCCESS);
}

kern_return_t
macserver_HostPriv(mach_port_t		request_port,
		   mach_port_t		*host_port)
{
    if (!macobject_copyin(host_priv_port, host_port))
	return (KERN_FAILURE);

    return (KERN_SUCCESS);
}

kern_return_t
macserver_Emulate(mach_port_t		request_port)
{
    current_thread_pcb()->pcb_flags |= MAC_EMULATION;

    return (KERN_SUCCESS);
}
