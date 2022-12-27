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
 * $Log:	mnice.c,v $
 * Revision 2.5  91/08/29  15:48:42  rpd
 * 	Moved machid include files into the standard include directory.
 * 	[91/08/29            rpd]
 * 
 * Revision 2.4  91/03/27  17:27:15  mrt
 * 	Changed mach.h include
 * 
 * Revision 2.3  91/03/19  12:31:36  mrt
 * 	Changed to new copyright
 * 
 * Revision 2.2  90/09/12  16:32:40  rpd
 * 	Created.
 * 	[90/06/18            rpd]
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
    quit(1, "usage: mnice [-host machine] [-change|-setmax|-maxpri pset] [-<priority>] ids...\n");
}

main(argc, argv)
    int argc;
    char *argv[];
{
    char *hostname = "";
    enum { Default, ChangeThreads, MaxPriority, SetMax } action = Default;
    mprocessor_set_t pset;
    int priority = 12;
    kern_return_t kr;
    int i;

    for (i = 1; i < argc; i++)
	if (streql(argv[i], "-host") && (i < argc-1))
	    hostname = argv[++i];
	else if (streql(argv[i], "-change") &&
		 (action == Default))
	    action = ChangeThreads;
	else if (streql(argv[i], "-setmax") &&
		 (action == Default))
	    action = SetMax;
	else if (streql(argv[i], "-maxpri") &&
		 (action == Default) && (i < argc-1)) {
	    pset = atoi(argv[++i]);
	    action = MaxPriority;
	} else if ((argv[i][0] == '-') &&
		   ('0' <= argv[i][1]) && (argv[i][1] <= '9'))
	    priority = atoi(&argv[i][1]);
	else if (streql(argv[i], "--")) {
	    i++;
	    break;
	} else if (argv[i][0] == '-')
	    usage();
	else
	    break;

    argv += i;
    argc -= i;

    kr = netname_look_up(name_server_port, hostname, "MachID", &server);
    if (kr != KERN_SUCCESS)
	quit(1, "mnice: netname_lookup_up(MachID): %s\n",
	     mach_error_string(kr));

    auth = mach_host_priv_self();
    if (auth == MACH_PORT_NULL)
	auth = mach_task_self();

    for (i = 0; i < argc; i++) {
	mach_id_t id = atoi(argv[i]), origid = id;
	mach_type_t type;

	kr = machid_mach_type(server, auth, id, &type);
	if (kr != KERN_SUCCESS)
	    quit(1, "mnice: machid_mach_type: %s\n", mach_error_string(kr));

	switch (type) {
	  case MACH_TYPE_NONE:
	    fprintf(stderr, "mnice: bad id %u\n", id);
	    break;

	  case MACH_TYPE_THREAD:
	    switch (action) {
	      case Default:
		kr = machid_thread_priority(server, auth, id, priority, FALSE);
		break;

	      case SetMax:
		kr = machid_thread_priority(server, auth, id, priority, TRUE);
		break;

	      case MaxPriority:
		kr = machid_thread_max_priority(server, auth, id,
						pset, priority);
		break;

	      default:
		goto badaction;
	    }
	    break;

	  case MACH_TYPE_TASK:
	    switch (action) {
	      case Default:
		kr = machid_task_priority(server, auth, id, priority, FALSE);
		break;

	      case ChangeThreads:
		kr = machid_task_priority(server, auth, id, priority, TRUE);
		break;

	      default:
		goto badaction;
	    }
	    break;

	  case MACH_TYPE_PROCESSOR_SET_NAME:
	    kr = machid_mach_lookup(server, auth, id,
				    MACH_TYPE_PROCESSOR_SET, &id);
	    if (kr != KERN_SUCCESS)
		quit(1, "mnice: machid_mach_lookup: %s\n",
		     mach_error_string(kr));

	    if (id == 0)
		goto badaction;
	    /* fall-through */

	  case MACH_TYPE_PROCESSOR_SET:
	    switch (action) {
	      case Default:
		kr = machid_processor_set_max_priority(server, auth, id,
						       priority, FALSE);
		break;

	      case ChangeThreads:
		kr = machid_processor_set_max_priority(server, auth, id,
						       priority, TRUE);
		break;

	      default:
		goto badaction;
	    }
	    break;

	  default:
	  badaction:
	    fprintf(stderr, "mnice: %u has type %s\n",
		    origid, mach_type_string(type));
	    continue;
	}

	if (kr != KERN_SUCCESS)
	    fprintf(stderr, "mnice: %s %u: %s\n",
		    mach_type_string(type), origid, mach_error_string(kr));
    }

    exit(0);
}
