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
 * $Log:	bootstrap.c,v $
 * Revision 2.18  91/09/12  16:37:49  bohman
 * 	Made bootstrap task call mac2 machine dependent code before running
 * 	'startup', which is loaded from the UX file system.  This needs to
 * 	be handled more generally in the future.
 * 	[91/09/11  17:07:59  bohman]
 * 
 * Revision 2.17  91/08/28  11:14:22  jsb
 * 	Changed msgh_kind to msgh_seqno.
 * 	[91/08/10            rpd]
 * 
 * Revision 2.16  91/08/03  18:18:45  jsb
 * 	Moved bootstrap query earlier. Removed all NORMA specific code.
 * 	[91/07/25  18:25:35  jsb]
 * 
 * Revision 2.15  91/07/31  17:44:14  dbg
 * 	Pass host port to boot_load_program and read_emulator_symbols.
 * 	[91/07/30  17:02:40  dbg]
 * 
 * Revision 2.14  91/07/01  08:24:54  jsb
 * 	Removed notion of master/slave. Instead, refuse to start up
 * 	a bootstrap task whenever startup_name is null.
 * 	[91/06/29  16:48:14  jsb]
 * 
 * Revision 2.13  91/06/19  11:55:57  rvb
 * 	Ask for startup program to override default.
 * 	[91/06/18  21:39:17  rvb]
 * 
 * Revision 2.12  91/06/17  15:46:51  jsb
 * 	Renamed NORMA conditionals.
 * 	[91/06/17  10:49:04  jsb]
 * 
 * Revision 2.11  91/06/06  17:06:53  jsb
 * 	Allow slaves to act as masters (for now).
 * 	[91/05/13  17:36:17  jsb]
 * 
 * Revision 2.10  91/05/18  14:31:32  rpd
 * 	Added argument to kernel_thread.
 * 	[91/04/03            rpd]
 * 
 * Revision 2.9  91/05/14  16:40:06  mrt
 * 	Correcting copyright
 * 
 * Revision 2.8  91/02/05  17:25:42  mrt
 * 	Changed to new Mach copyright
 * 	[91/02/01  16:11:22  mrt]
 * 
 * Revision 2.7  90/12/14  11:01:58  jsb
 * 	Changes to NORMA_BOOT support. Use real device port, not a proxy;
 * 	new device forwarding code handles forwarding of requests.
 * 	Have slave not bother starting bootstrap task if there is nothing
 * 	for it to run.
 * 	[90/12/13  21:37:57  jsb]
 * 
 * Revision 2.6  90/09/28  16:55:30  jsb
 * 	Added NORMA_BOOT support.
 * 	[90/09/28  14:04:43  jsb]
 * 
 * Revision 2.5  90/06/02  14:53:39  rpd
 * 	Load emulator symbols.
 * 	[90/05/11  16:58:37  rpd]
 * 
 * 	Made bootstrap_task available externally.
 * 	[90/04/05            rpd]
 * 	Converted to new IPC.
 * 	[90/03/26  22:03:39  rpd]
 * 
 * Revision 2.4  90/01/11  11:43:02  dbg
 * 	Initialize bootstrap print routines.  Remove port number
 * 	printout.
 * 	[89/12/20            dbg]
 * 
 * Revision 2.3  89/11/29  14:09:01  af
 * 	Enlarged the bootstrap task's map to accomodate some unnamed
 * 	greedy RISC box.  Sigh.
 * 	[89/11/07            af]
 * 	Made root_name and startup_name non-preallocated, so that
 * 	they can be changed at boot time on those machines like
 * 	mips and Sun where the boot loader passes command line
 * 	arguments to the kernel.
 * 	[89/10/28            af]
 * 
 * Revision 2.2  89/09/08  11:25:02  dbg
 * 	Pass root partition name to default_pager_setup.
 * 	[89/08/31            dbg]
 * 
 * 	Assume that device service has already been created.
 * 	Create bootstrap task here and give it the host and
 * 	device ports.
 * 	[89/08/01            dbg]
 * 
 * 	Call default_pager_setup.
 * 	[89/07/11            dbg]
 * 
 * 12-Apr-89  David Golub (dbg) at Carnegie-Mellon University
 *	Removed console_port.
 *
 */
/*
 * Bootstrap the various built-in servers.
 */
#include <mach_kdb.h>

#include <mach/port.h>
#include <mach/message.h>
#include <mach/mach_interface.h>
#include <mach/vm_param.h>
#include <ipc/ipc_port.h>
#include <kern/host.h>
#include <kern/task.h>
#include <kern/thread.h>
#include <device/device_port.h>
#include <sys/reboot.h>

mach_port_t	bootstrap_master_device_port;	/* local name */
mach_port_t	bootstrap_master_host_port;	/* local name */

int	boot_load_program();

extern char	*root_name;

char	*startup_name = "/mach_servers/startup";
char	 new_startup[64];
char	*emulator_name = "/mach_servers/emulator";

extern void	default_pager();
extern void	default_pager_setup();

void		bootstrap();	/* forward */

task_t	bootstrap_task;

void bootstrap_create()
{
	ipc_port_t port;

	/*
	 * Create the bootstrap task.
	 */
#define	BOOTSTRAP_MAP_SIZE	(256 * PAGE_SIZE)

	if ((boothowto>>RB_SHIFT) & RB_ASKNAME) {
		printf("bootstrap server? [%s] ", startup_name);
		gets(new_startup, sizeof(new_startup));
		if (new_startup[0] == 'n' && new_startup[1] == '\0') {
			printf("Not starting bootstrap task.\n");
			return;
		}
		if (new_startup[0]) {
			startup_name = new_startup;
		}
	}
	bootstrap_task = kernel_task_create(TASK_NULL, BOOTSTRAP_MAP_SIZE);

	/*
	 * Insert send rights to the master host and device ports.
	 */

	port = ipc_port_make_send(realhost.host_priv_self);
	bootstrap_master_host_port = 1;
	for (;;) {
		kern_return_t kr;

		kr = mach_port_insert_right(bootstrap_task->itk_space,
					    bootstrap_master_host_port,
					    port, MACH_MSG_TYPE_PORT_SEND);
		if (kr == KERN_SUCCESS)
			break;
		assert(kr == KERN_NAME_EXISTS);

		bootstrap_master_host_port++;
	}

	port = ipc_port_make_send(master_device_port);
	bootstrap_master_device_port = 1;
	for (;;) {
		kern_return_t kr;

		kr = mach_port_insert_right(bootstrap_task->itk_space,
					    bootstrap_master_device_port,
					    port, MACH_MSG_TYPE_PORT_SEND);
		if (kr == KERN_SUCCESS)
			break;
		assert(kr == KERN_NAME_EXISTS);

		bootstrap_master_device_port++;
	}

	/*
	 * Start the bootstrap thread.
	 */
	(void) kernel_thread(bootstrap_task, bootstrap, (char *) 0);
}

/*
 * From this point on, the bootstrap task uses the external
 * versions of the kernel interface routines.
 */
#include <mach/mach_user_internal.h>
#include <mach/mach_port_internal.h>
#include <boot_ufs/boot_printf.h>

extern void	boot_printf_init();

void bootstrap()
{
	register kern_return_t	result;
	mach_port_t		user_task;
	mach_port_t		user_thread;
	mach_port_t		bootstrap_port;

	mach_port_t		my_task = mach_task_self();

	char			flag_string[12];

	boot_printf_init();

#ifdef	mac2
	(void) macobjects_bootstrap(bootstrap_master_host_port);
#endif

	/*
	 * Allocate a port to pass initial server ports back and forth.
	 */
	result = mach_port_allocate(my_task, MACH_PORT_RIGHT_RECEIVE,
				    &bootstrap_port);
	assert(result == KERN_SUCCESS);

	result = mach_port_insert_right(my_task, bootstrap_port,
					bootstrap_port,
					MACH_MSG_TYPE_MAKE_SEND);
	assert(result == KERN_SUCCESS);

	result = task_set_bootstrap_port(my_task, bootstrap_port);
	assert(result == KERN_SUCCESS);

	/*
	 * Set up the temporary in-kernel default pager.
	 */
	default_pager_setup(bootstrap_master_host_port,
			    bootstrap_master_device_port,
			    root_name);

	/*
	 * Create the user task and thread to run the startup file.
	 */
	result = task_create(my_task, FALSE, &user_task);
	if (result != KERN_SUCCESS)
	    panic("task_create %d", result);

	result = thread_create(user_task, &user_thread);
	if (result != KERN_SUCCESS)
	    panic("thread_create %d", result);

	/*
	 * Load the startup file.
	 * Pass it a command line of
	 * "startup -boot_flags root_name"
	 */
	get_boot_flags(flag_string);
	result = boot_load_program(bootstrap_master_host_port,
				   bootstrap_master_device_port,
				   user_task,
				   user_thread,
				   root_name,
				   startup_name,
				   flag_string,
				   root_name,
				   (char *)0);
	if (result != 0)
	    panic("boot_load_program %d", result);

	/*
	 * Read emulator symbol table.
	 * Startup symbol table was read inside boot_load_program.
	 */
	read_emulator_symbols(bootstrap_master_host_port,
				bootstrap_master_device_port,
				   root_name,
				   emulator_name);

	/*
	 * Start up the thread
	 */
	result = thread_resume(user_thread);
	if (result != KERN_SUCCESS)
	    panic("thread_resume %d", result);

	{
	    struct imsg {
		mach_msg_header_t	hdr;
		mach_msg_type_t		port_desc_1;
		mach_port_t		port_1;
		mach_msg_type_t		port_desc_2;
		mach_port_t		port_2;
	    } imsg;

	    /*
	     * Wait for the port-request message
	     */
	    while (mach_msg(&imsg.hdr, MACH_RCV_MSG,
			    0, sizeof imsg.hdr, bootstrap_port,
			    MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL)
						!= MACH_MSG_SUCCESS)
		continue;

	    /*
	     * Send back the host and device ports
	     */

	    imsg.hdr.msgh_bits = MACH_MSGH_BITS_COMPLEX |
		MACH_MSGH_BITS(MACH_MSGH_BITS_REMOTE(imsg.hdr.msgh_bits), 0);
	    /* msgh_size doesn't need to be initialized */
	    /* leave msgh_remote_port untouched */
	    imsg.hdr.msgh_local_port = MACH_PORT_NULL;
	    /* leave msgh_seqno untouched */
	    imsg.hdr.msgh_id += 100;	/* this is a reply msg */

	    imsg.port_desc_1.msgt_name = MACH_MSG_TYPE_COPY_SEND;
	    imsg.port_desc_1.msgt_size = 32;
	    imsg.port_desc_1.msgt_number = 1;
	    imsg.port_desc_1.msgt_inline = TRUE;
	    imsg.port_desc_1.msgt_longform = FALSE;
	    imsg.port_desc_1.msgt_deallocate = FALSE;
	    imsg.port_desc_1.msgt_unused = 0;

	    imsg.port_1	= bootstrap_master_host_port;

	    imsg.port_desc_2 = imsg.port_desc_1;

	    imsg.port_2	= bootstrap_master_device_port;

	    result = mach_msg(&imsg.hdr, MACH_SEND_MSG,
			      sizeof imsg, 0, MACH_PORT_NULL,
			      MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
	    if (result != MACH_MSG_SUCCESS)
		panic("mach_msg %x", result);
	}

	/*
	 * Become the default pager
	 */
	default_pager(bootstrap_master_host_port);
	/*NOTREACHED*/
}

/*
 * Parse the boot flags into an argument string.
 * Format as a standard flag argument: '-asd...'
 */

get_boot_flags(str)
	char	str[];	/* OUT */
{
	register char *cp;
	register int	bflag;
#ifdef	mips
	extern char *machine_get_boot_flags();
	cp = machine_get_boot_flags(str);
#else	mips
	cp = str;
	*cp++ = '-';
#endif	mips

	bflag = boothowto;

	if (bflag & RB_ASKNAME)
	    *cp++ = 'a';
	if (bflag & RB_SINGLE)
	    *cp++ = 's';
#if	MACH_KDB
	if (bflag & RB_KDB)
	    *cp++ = 'd';
#endif	MACH_KDB
	if (bflag & RB_HALT)
	    *cp++ = 'h';
	if (bflag & RB_INITNAME)
	    *cp++ = 'n';

	if (cp == &str[1])	/* no flags */
	    *cp++ = 'x';
	*cp = '\0';
}
