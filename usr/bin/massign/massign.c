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
 * $Log:	massign.c,v $
 * Revision 2.5  91/08/29  15:48:21  rpd
 * 	Moved machid include files into the standard include directory.
 * 	[91/08/29            rpd]
 * 
 * Revision 2.4  91/03/27  17:27:01  mrt
 * 	Changed mach.h include
 * 
 * Revision 2.3  91/03/19  12:31:09  mrt
 * 	Changed to new copyright
 * 
 * Revision 2.2  90/09/12  16:32:17  rpd
 * 	Created.
 * 	[90/08/31            rpd]
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
    quit(1, "usage: massign [-host machine] [-wait|-threads] [-pset id] ids...\n");
}

main(argc, argv)
    int argc;
    char *argv[];
{
    char *hostname = "";
    boolean_t Wait = FALSE;
    boolean_t AssignThreads = FALSE;
    mprocessor_set_t pset = 0;
    kern_return_t kr;
    int i;

    for (i = 1; i < argc; i++)
	if (streql(argv[i], "-host") && (i < argc-1))
	    hostname = argv[++i];
	else if (streql(argv[i], "-wait"))
	    Wait = TRUE;
	else if (streql(argv[i], "-threads"))
	    AssignThreads = TRUE;
	else if (streql(argv[i], "-pset") && (i < argc-1))
		 pset = atoi(argv[++i]);
	else if (streql(argv[i], "--")) {
	    i++;
	    break;
	} else if (argv[i][0] == '-')
	    usage();
	else
	    break;

    argv += i;
    argc -= i;

    if ((Wait && AssignThreads) ||
	(Wait && (pset == 0)))
	usage();

    kr = netname_look_up(name_server_port, hostname, "MachID", &server);
    if (kr != KERN_SUCCESS)
	quit(1, "massign: netname_lookup_up(MachID): %s\n",
	     mach_error_string(kr));

    auth = mach_host_priv_self();
    if (auth == MACH_PORT_NULL)
	auth = mach_task_self();

    for (i = 0; i < argc; i++) {
	mach_id_t id = atoi(argv[i]);
	mach_type_t type;

	kr = machid_mach_type(server, auth, id, &type);
	if (kr != KERN_SUCCESS)
	    quit(1, "massign: machid_mach_type: %s\n", mach_error_string(kr));

	switch (type) {
	  case MACH_TYPE_NONE:
	    fprintf(stderr, "massign: bad id %u\n", id);
	    break;

	  case MACH_TYPE_THREAD:
	    if (Wait || AssignThreads)
		goto badaction;

	    if (pset == 0)
		kr = machid_thread_assign_default(server, auth, id);
	    else
		kr = machid_thread_assign(server, auth, id, pset);
	    break;

	  case MACH_TYPE_TASK:
	    if (Wait)
		goto badaction;

	    if (pset == 0)
		kr = machid_task_assign_default(server, auth,
						id, AssignThreads);
	    else
		kr = machid_task_assign(server, auth,
					id, pset, AssignThreads);
	    break;

	  case MACH_TYPE_PROCESSOR:
	    if (AssignThreads || (pset == 0))
		goto badaction;

	    kr = machid_processor_assign(server, auth, id, pset, Wait);
	    break;

	  default:
	  badaction:
	    fprintf(stderr, "massign: %u has type %s\n",
		    id, mach_type_string(type));
	    continue;
	}

	if (kr != KERN_SUCCESS)
	    fprintf(stderr, "massign: %s %u: %s\n",
		    mach_type_string(type), id, mach_error_string(kr));
    }

    exit(0);
}
