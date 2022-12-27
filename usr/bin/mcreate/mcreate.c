/* 
 * Mach Operating System
 * Copyright (c) 1991,1990 Carnegie Mellon University
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
 * $Log:	mcreate.c,v $
 * Revision 2.5  91/08/29  15:48:29  rpd
 * 	Moved machid include files into the standard include directory.
 * 	[91/08/29            rpd]
 * 
 * Revision 2.4  91/03/27  17:27:06  mrt
 * 	Changed mach.h include
 * 
 * Revision 2.3  91/03/19  12:31:21  mrt
 * 	Changed to new copyright
 * 
 * Revision 2.2  90/09/12  16:32:24  rpd
 * 	Created.
 * 	[90/08/30            rpd]
 * 
 */

#include <stdio.h>
#include <strings.h>
#include <mach.h>
#include <mach_error.h>
#include <servers/netname.h>
#include <servers/machid.h>

#define streql(a, b)	(strcmp((a), (b)) == 0)

static mach_port_t server;
static mach_port_t auth;

static void
usage()
{
    quit(1, "usage: mcreate [-host machine] [-inherit] [-pset|-task|-thread] id\n");
}

main(argc, argv)
    int argc;
    char *argv[];
{
    char *hostname = "";
    enum { None, PSet, Inherit, Task, TaskInherit, Thread } action = None;
    mach_id_t name;
    kern_return_t kr;
    int i;

    for (i = 1; i < argc; i++)
	if (streql(argv[i], "-host") && (i < argc-1))
	    hostname = argv[++i];
	else if (streql(argv[i], "-pset") &&
		 (action == None))
	    action = PSet;
	else if (streql(argv[i], "-inherit") &&
		 (action == None))
	    action = Inherit;
	else if (streql(argv[i], "-inherit") &&
		 (action == Task))
	    action = TaskInherit;
	else if (streql(argv[i], "-task") &&
		 (action == None))
	    action = Task;
	else if (streql(argv[i], "-task") &&
		 (action == Inherit))
	    action = TaskInherit;
	else if (streql(argv[i], "-thread") &&
		 (action == None))
	    action = Thread;
	else if (streql(argv[i], "--")) {
	    i++;
	    break;
	} else if (argv[i][0] == '-')
	    usage();
	else
	    break;

    argv += i;
    argc -= i;

    switch (action)
      default:
	if (argc != 1)
      case None:
      case Inherit:
	    usage();

    name = atoi(argv[0]);

    kr = netname_look_up(name_server_port, hostname, "MachID", &server);
    if (kr != KERN_SUCCESS)
	quit(1, "mcreate: netname_lookup_up(MachID): %s\n",
	     mach_error_string(kr));

    auth = mach_host_priv_self();
    if (auth == MACH_PORT_NULL)
	auth = mach_task_self();

    switch (action) {
      case PSet: {
	mprocessor_set_t pset;
	mprocessor_set_name_t pset_name;

	kr = machid_processor_set_create(server, auth, name,
					 &pset, &pset_name);
	if (kr != KERN_SUCCESS)
	    quit(1, "mcreate: machid_processor_set_create(%u): %s\n",
		 name, mach_error_string(kr));

	printf("Created pset %u, pset name %u.\n", pset, pset_name);
	break;
      }

      case Task:
      case TaskInherit: {
	mtask_t task;

	kr = machid_task_create(server, auth, name,
				action == TaskInherit, &task);
	if (kr != KERN_SUCCESS)
	    quit(1, "mcreate: machid_task_create(%u): %s\n",
		 name, mach_error_string(kr));

	printf("Created task %u.\n", task);
	break;
      }

      case Thread: {
	mthread_t thread;

	kr = machid_thread_create(server, auth, name, &thread);
	if (kr != KERN_SUCCESS)
	    quit(1, "mcreate: machid_thread_create(%u): %s\n",
		 name, mach_error_string(kr));

	printf("Created thread %u.\n", thread);
	break;
      }
    }

    exit(0);
}
