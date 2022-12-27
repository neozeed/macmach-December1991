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
 * $Log:	mpolicy.c,v $
 * Revision 2.5  91/08/29  15:48:49  rpd
 * 	Moved machid include files into the standard include directory.
 * 	[91/08/29            rpd]
 * 
 * Revision 2.4  91/03/27  17:27:20  mrt
 * 	Changed mach.h include
 * 
 * Revision 2.3  91/03/19  12:31:43  mrt
 * 	Changed to new copyright
 * 
 * Revision 2.2  90/10/08  13:15:48  rpd
 * 	Created.
 * 	[90/10/08  13:12:33  rpd]
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
    quit(1, "usage: mpolicy [-host machine] [-policy policy data] [-enable policy] [-disable policy] [-threads] ids...\n");
}

static int
parse_policy(str)
    char *str;
{
    if (streql(str, "tshare"))
	return POLICY_TIMESHARE;
    else if (streql(str, "fixpri"))
	return POLICY_FIXEDPRI;
    else
	usage();
}

static int
parse_data(policy, str)
    int policy;
    char *str;
{
    return atoi(str);
}

main(argc, argv)
    int argc;
    char *argv[];
{
    char *hostname = "";
    enum { None, ThreadPolicy, PolicyEnable, PolicyDisable } action = None;
    boolean_t ChangeThreads = FALSE;
    int policy, data;
    kern_return_t kr;
    int i, j;

    for (i = 1; i < argc; i++)
	if (streql(argv[i], "-host") && (i < argc-1))
	    hostname = argv[++i];
	else if (streql(argv[i], "-policy") &&
		 (action == None) && (i < argc-2)) {
	    action = ThreadPolicy;
	    policy = parse_policy(argv[++i]);
	    data = parse_data(policy, argv[++i]);
	} else if (streql(argv[i], "-enable") &&
		   (action == None) && (i < argc-1)) {
	    action = PolicyEnable;
	    policy = parse_policy(argv[++i]);
	} else if (streql(argv[i], "-disable") &&
		   (action == None) && (i < argc-1)) {
	    action = PolicyDisable;
	    policy = parse_policy(argv[++i]);
	} else if (streql(argv[i], "-threads"))
	    ChangeThreads = TRUE;
	else if (streql(argv[i], "--")) {
	    i++;
	    break;
	} else if (argv[i][0] == '-')
	    usage();
	else
	    break;

    argv += i;
    argc -= i;

    if ((ChangeThreads && (action != PolicyDisable)) ||
	(action == None))
	usage();

    kr = netname_look_up(name_server_port, hostname, "MachID", &server);
    if (kr != KERN_SUCCESS)
	quit(1, "mpolicy: netname_lookup_up(MachID): %s\n",
	     mach_error_string(kr));

    auth = mach_host_priv_self();
    if (auth == MACH_PORT_NULL)
	auth = mach_task_self();

    for (i = 0; i < argc; i++) {
	mach_id_t id = atoi(argv[i]), origid = id;
	mach_type_t type;

	kr = machid_mach_type(server, auth, id, &type);
	if (kr != KERN_SUCCESS)
	    quit(1, "mpolicy: machid_mach_type: %s\n", mach_error_string(kr));

	if (action == ThreadPolicy) {
	    mthread_t *threads;
	    unsigned int threadCnt;

	    switch (type) {
	      case MACH_TYPE_NONE:
		continue;

	      default:
		kr = machid_mach_lookup(server, auth, id,
					MACH_TYPE_THREAD, &id);
		if (kr != KERN_SUCCESS)
		    quit(1, "mpolicy: machid_mach_lookup: %s\n",
			 mach_error_string(kr));

		if (id == 0)
		    goto badtype;
		/* fall-through */

	      case MACH_TYPE_THREAD:
		kr = machid_thread_policy(server, auth, id, policy, data);
		if (kr != KERN_SUCCESS)
		    quit(1, "mpolicy: machid_thread_policy: %s\n",
			 mach_error_string(kr));
		continue;

	      case MACH_TYPE_TASK:
		kr = machid_task_threads(server, auth, id,
					 &threads, &threadCnt);
		break;

	      case MACH_TYPE_PROCESSOR_SET_NAME:
		kr = machid_mach_lookup(server, auth, id,
					MACH_TYPE_PROCESSOR_SET, &id);
		if (kr != KERN_SUCCESS)
		    quit(1, "mpolicy: machid_mach_lookup: %s\n",
			 mach_error_string(kr));

		if (id == 0)
		    goto badtype;
		/* fall-through */

	      case MACH_TYPE_PROCESSOR_SET:
		kr = machid_processor_set_threads(server, auth, id,
						  &threads, &threadCnt);
		break;

	      case MACH_TYPE_HOST:
		kr = machid_mach_lookup(server, auth, id,
					MACH_TYPE_HOST_PRIV, &id);
		if (kr != KERN_SUCCESS)
		    quit(1, "mpolicy: machid_mach_lookup: %s\n",
			 mach_error_string(kr));

		if (id == 0)
		    goto badtype;
		/* fall-through */

	      case MACH_TYPE_HOST_PRIV:
		kr = machid_host_threads(server,auth, id,
					 &threads, &threadCnt);
		break;

	      badtype:
		fprintf(stderr, "mpolicy: %u has type %s\n",
			origid, mach_type_string(type));
		continue;
	    }
	    if (kr != KERN_SUCCESS)
		continue;

	    for (j = 0; j < threadCnt; j++) {
		kr = machid_thread_policy(server, auth,
					  threads[j], policy, data);
		if (kr != KERN_SUCCESS)
		    quit(1, "mpolicy: machid_thread_policy: %s\n",
			 mach_error_string(kr));
	    }

	    kr = vm_deallocate(mach_task_self(), (vm_offset_t) threads,
			       (vm_size_t) (threadCnt * sizeof *threads));
	    if (kr != KERN_SUCCESS)
		quit(1, "mpolicy: vm_deallocate: %s\n", mach_error_string(kr));
	} else {
	    switch (type) {
	      case MACH_TYPE_NONE:
		continue;

	      default:
	      case MACH_TYPE_PROCESSOR_SET_NAME:
		kr = machid_mach_lookup(server, auth, id,
					MACH_TYPE_PROCESSOR_SET, &id);
		if (kr != KERN_SUCCESS)
		    quit(1, "mpolicy: machid_mach_lookup: %s\n",
			 mach_error_string(kr));

		if (id == 0)
		    goto badtype;
		/* fall-through */

	      case MACH_TYPE_PROCESSOR_SET:
		switch (action) {
		  case PolicyEnable:
		    kr = machid_processor_set_policy_enable(server, auth,
						id, policy);
		    if (kr != KERN_SUCCESS)
			quit(1, "mpolicy: machid_processor_set_policy_enable: %s\n",
			     mach_error_string(kr));
		    break;

		  case PolicyDisable:
		    kr = machid_processor_set_policy_disable(server, auth,
						id, policy, ChangeThreads);
		    if (kr != KERN_SUCCESS)
			quit(1, "mpolicy: machid_processor_set_policy_disable: %s\n",
			     mach_error_string(kr));
		    break;
		}
		break;
	    }
	}
    }

    exit(0);
}
