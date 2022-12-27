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
 * $Log:	mach_init.c,v $
 * Revision 2.2  91/03/27  16:02:17  mrt
 * 	First checkin
 * 
 */
/*
 *	File:	mach_init.c
 *	Author:	Michael Young, Carnegie Mellon University
 *	Date:	Aug. 1986
 *
 *	This file is called by crt0 and fork. It initializes
 *	values for the standard server ports and calls
 *	mig_init to initialize a mig reply port.
 *
 */

#define	MACH_INIT_SLOTS		1
#include <mach.h>

#ifdef mac2
#include <stdio.h>
#include <mach/message.h>
#include <assert.h>
#endif /* mac2 */

extern void mig_init();

mach_port_t	mach_task_self_ = MACH_PORT_NULL;

mach_port_t	name_server_port = MACH_PORT_NULL;
mach_port_t	environment_port = MACH_PORT_NULL;
mach_port_t	service_port = MACH_PORT_NULL;

vm_size_t	vm_page_size;

int		mach_init()
{
	mach_port_array_t	mach_init_ports;
	unsigned int		mach_init_ports_count;
	vm_statistics_data_t	vm_stat;
	kern_return_t		kr;

#ifdef mac2
	/* Abort if not running on a Mach 3.0 system.
	 * On Mach 3.0, this should return MACH_MSG_SUCCESS (0).
	 * On Mach 2.5, this should return KERN_INVALID_ARGUMENT (4).
	 */
        if (mach_msg((mach_msg_header_t *)0, MACH_MSG_OPTION_NONE, 0, 0,
	  MACH_PORT_NULL, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL)
	  != MACH_MSG_SUCCESS) {
	    fprintf(stderr, "Attempt to use 3.0 libmach on a 2.5 system.\n");
	    abort();
	}
#endif /* mac2 */

#undef	mach_task_self

	/*
	 *	Get the important ports into the cached values,
	 *	as required by "mach_init.h".
	 */
	 
	mach_task_self_ = mach_task_self();

	/*
	 *	Initialize the single mig reply port
	 */

	mig_init(0);

	/*
	 *	Cache some other valuable system constants
	 */

	kr = vm_statistics(mach_task_self_, &vm_stat);
	if (kr != KERN_SUCCESS)
		quit(2, "mach_init: vm_statistics: %s (%d)\n",
		     mach_error_string(kr), kr);
	vm_page_size = vm_stat.pagesize;

	/*
	 *	Find those ports important to every task.
	 */

	kr = mach_ports_lookup(mach_task_self_, &mach_init_ports,
			       &mach_init_ports_count);
	if (kr != KERN_SUCCESS)
		quit(2, "mach_init: mach_ports_lookup: %s\n",
		     mach_error_string(kr));
	if (mach_init_ports_count < MACH_PORTS_SLOTS_USED)
		quit(2, "mach_init: mach_ports_lookup: count=%d\n",
		     mach_init_ports_count);

	name_server_port = mach_init_ports[NAME_SERVER_SLOT];
	environment_port = mach_init_ports[ENVIRONMENT_SLOT];
	service_port     = mach_init_ports[SERVICE_SLOT];

	/* get rid of out-of-line data so brk has a chance of working */
	kr = vm_deallocate(mach_task_self_,
			   (vm_address_t) mach_init_ports,
			   (vm_size_t) (mach_init_ports_count *
					sizeof(mach_port_t)));
	if (kr != KERN_SUCCESS)
		quit(2, "mach_init: vm_deallocate: %s (%d)\n",
		     mach_error_string(kr), kr);

	return(0);
}

int		(*mach_init_routine)() = mach_init;

#ifndef	lint
/*
 *	Routines which our library must suck in, to avoid
 *	a later library from referencing them and getting
 *	the wrong version.
 */
_replacements()
{
	sbrk(0);
	fork();
}
#endif	lint
