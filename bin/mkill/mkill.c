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
 * $Log:	mkill.c,v $
 * Revision 2.5  91/08/29  15:48:36  rpd
 * 	Moved machid include files into the standard include directory.
 * 	[91/08/29            rpd]
 * 
 * Revision 2.4  91/03/27  17:27:10  mrt
 * 	Changed mach.h include
 * 
 * Revision 2.3  91/03/19  12:31:28  mrt
 * 	Changed to new copyright
 * 
 * Revision 2.2  90/09/12  16:32:31  rpd
 * 	Removed thread_depress_abort.
 * 	[90/08/07            rpd]
 * 
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

extern char *malloc();

#define streql(a, b)	(strcmp((a), (b)) == 0)

static mach_port_t server;
static mach_port_t auth;

enum action { Default, Kill, Stop, Continue, Abort };

static void perform_action();

static void
usage()
{
    quit(1, "usage: mkill [-host machine] [-KILL|-STOP|-CONT|-ABORT] ids...\n");
}

main(argc, argv)
    int argc;
    char *argv[];
{
    char *hostname = "";
    enum action action = Default;
    mach_id_t *ids;
    unsigned int numids;
    kern_return_t kr;
    int i;

    for (i = 1; i < argc; i++)
	if (streql(argv[i], "-host") && (i < argc-1))
	    hostname = argv[++i];
	else if ((streql(argv[i], "-K") ||
		  streql(argv[i], "-KILL") ||
		  streql(argv[i], "-9")) && (action == Default))
	    action = Kill;
	else if ((streql(argv[i], "-S") ||
		  streql(argv[i], "-STOP") ||
		  streql(argv[i], "-17")) && (action == Default))
	    action = Stop;
	else if ((streql(argv[i], "-C") ||
		  streql(argv[i], "-CONT") ||
		  streql(argv[i], "-19")) && (action == Default))
	    action = Continue;
	else if ((streql(argv[i], "-A") ||
		  streql(argv[i], "-ABORT")) && (action == Default))
	    action = Abort;
	else if (streql(argv[i], "--")) {
	    i++;
	    break;
	} else if (argv[i][0] == '-')
	    usage();
	else
	    break;

    argv += i;
    argc -= i;

    numids = argc;
    ids = (mach_id_t *) malloc(numids * sizeof *ids);

    for (i = 0; i < argc; i++)
	ids[i] = atoi(argv[i]);

    kr = netname_look_up(name_server_port, hostname, "MachID", &server);
    if (kr != KERN_SUCCESS)
	quit(1, "mkill: netname_lookup_up(MachID): %s\n",
	     mach_error_string(kr));

    auth = mach_host_priv_self();
    if (auth == MACH_PORT_NULL)
	auth = mach_task_self();

    perform_action(action, ids, numids);
    exit(0);
}

static void
perform_action(action, ids, numids)
    enum action action;
    mach_id_t *ids;
    unsigned int numids;
{
    mach_type_t type;
    unsigned int i;
    kern_return_t kr;

    for (i = 0; i < numids; i++) {
	mach_id_t id = ids[i];

	kr = machid_mach_type(server, auth, id, &type);
	if (kr != KERN_SUCCESS)
	    quit(1, "mkill: machid_type: %s\n", mach_error_string(kr));

	switch (type) {
	  case MACH_TYPE_NONE:
	    fprintf(stderr, "mkill: bad id %u\n", id);
	    continue;

	  default:
	  badaction:
	    fprintf(stderr, "mkill: %u has type %s\n",
		    ids[i], mach_type_string(type));
	    continue;

	  case MACH_TYPE_TASK:
	    switch (action) {
	      case Default:
	      case Kill:
		kr = machid_task_terminate(server, auth, id);
		break;
	      case Stop:
		kr = machid_task_suspend(server, auth, id);
		break;
	      case Continue:
		kr = machid_task_resume(server, auth, id);
		break;
	      default:
		goto badaction;
	    }
	    break;

	  case MACH_TYPE_THREAD:
	    switch (action) {
	      case Default:
	      case Kill:
		kr = machid_thread_terminate(server, auth, id);
		break;
	      case Stop:
		kr = machid_thread_suspend(server, auth, id);
		break;
	      case Continue:
		kr = machid_thread_resume(server, auth, id);
		break;
	      case Abort:
		kr = machid_thread_abort(server, auth, id);
		break;
	      default:
		goto badaction;
	    }
	    break;

	  case MACH_TYPE_PROCESSOR_SET_NAME:
	    kr = machid_mach_lookup(server, auth, id,
				    MACH_TYPE_PROCESSOR_SET, &id);
	    if (kr != KERN_SUCCESS)
		quit(1, "mkill: machid_mach_lookup: %s\n",
		     mach_error_string(kr));

	    if (id == 0)
		goto badaction;
	    /* fall-through */

	  case MACH_TYPE_PROCESSOR_SET:
	    switch (action) {
	      case Default:
	      case Kill:
		kr = machid_processor_set_destroy(server, auth, id);
		break;
	      default:
		goto badaction;
	    }
	    break;

	  case MACH_TYPE_PROCESSOR:
	    switch (action) {
	      case Default:
	      case Stop:
		kr = machid_processor_exit(server, auth, id);
		break;
	      case Continue:
		kr = machid_processor_start(server, auth, id);
		break;
	      default:
		goto badaction;
	    }
	    break;
	}

	if (kr != KERN_SUCCESS)
	    fprintf(stderr, "mkill: %s %u: %s\n",
		    mach_type_string(type), ids[i], mach_error_string(kr));
    }
}
