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
 * $Log:	snames.c,v $
 * Revision 2.4  91/03/27  17:29:44  mrt
 * 	Changed mach.h include
 * 
 * Revision 2.3  91/03/19  12:40:09  mrt
 * 	Changed to new copyright
 * 
 * Revision 2.2  90/09/12  16:33:55  rpd
 * 	Initial check-in.
 * 	[90/09/12  15:53:26  rpd]
 * 
 */

#include <stdio.h>
#include <strings.h>
#include <errno.h>
#include <mach.h>
#include <mach/message.h>
#include <mach/notify.h>

#define streql(a, b)	(strcmp((a), (b)) == 0)

extern int errno;
extern char *sys_errlist[];
extern int sys_nerr;

void netname_init();

char *program = NULL;
boolean_t Debug = FALSE;

mach_port_t notify;
mach_port_t service;
mach_port_t child;

static boolean_t snames_demux();

static void
usage()
{
    quit(1, "usage: %s [-d] command [args ...]\n", program);
}

static char *
unix_error_string(errno)
    int errno;
{
    if ((0 <= errno) && (errno < sys_nerr))
	return sys_errlist[errno];
    else
	return "unknown errno";
}

void
main(argc, argv)
    int argc;
    char *argv[];
{
    mach_port_t pset;
    mach_port_t *rports;
    unsigned int rportCnt;
    int i, pid;
    kern_return_t kr;

    program = rindex(argv[0], '/');
    if (program == NULL)
	program = argv[0];
    else
	program++;

    for (i = 1; i < argc; i++)
	if (streql(argv[i], "-d"))
	    Debug = TRUE;
	else if (streql(argv[i], "--")) {
	    i++;
	    break;
	} else if (argv[i][0] == '-')
	    usage();
	else
	    break;

    argv += i;
    argc -= i;

    /* Allocate our notification port. */

    kr = mach_port_allocate(mach_task_self(),
			    MACH_PORT_RIGHT_RECEIVE, &notify);
    if (kr != KERN_SUCCESS)
	quit(1, "%s: mach_port_allocate: %s\n",
	     program, mach_error_string(kr));

    kr = service_checkin(service_port, name_server_port, &service);
    if (kr == KERN_SUCCESS) {
	/* We are the official name server. */

	if (argc != 0)
	    usage();

	child = MACH_PORT_NULL;
    } else {
	mach_port_t previous;

	if (Debug)
	    printf("%s: service_checkin: %s\n",
		   program, mach_error_string(kr));

	if (argc < 1)
	    usage();

	/* Allocate the netname service port. */

	kr = mach_port_allocate(mach_task_self(),
				MACH_PORT_RIGHT_RECEIVE, &service);
	if (kr != KERN_SUCCESS)
	    quit(1, "%s: mach_port_allocate: %s\n",
		 program, mach_error_string(kr));

	kr = mach_port_insert_right(mach_task_self(), service,
				    service, MACH_MSG_TYPE_MAKE_SEND);
	if (kr != KERN_SUCCESS)
	    quit(1, "%s: mach_port_insert_right: %s\n",
		 program, mach_error_string(kr));

	/* Register the new service port. */

	kr = mach_ports_lookup(mach_task_self(), &rports, &rportCnt);
	if (kr != KERN_SUCCESS)
	    quit(1, "%s: mach_ports_lookup: %s\n",
		 program, mach_error_string(kr));

	if (rportCnt == 0) {
	    kr = mach_ports_register(mach_task_self(), &service, 1);
	    if (kr != KERN_SUCCESS)
		quit(1, "%s: mach_ports_register: %s\n",
		     program, mach_error_string(kr));
	} else {
	    rports[0] = service;

	    kr = mach_ports_register(mach_task_self(), rports, rportCnt);
	    if (kr != KERN_SUCCESS)
		quit(1, "%s: mach_ports_register: %s\n",
		     program, mach_error_string(kr));

	    /* too bad we can't use the deallocate bit on the register call */

	    kr = vm_deallocate(mach_task_self(), (vm_address_t) rports,
			       (vm_size_t) (rportCnt * sizeof(mach_port_t)));
	    if (kr != KERN_SUCCESS)
		quit(1, "%s: vm_deallocate: %s\n",
		     program, mach_error_string(kr));
	}

	/* Fork a child; exec the user's command. */

	pid = fork();
	if (pid == -1)
	    quit(1, "%s: fork: %s\n",
		 program, unix_error_string(errno));
	else if (pid == 0) {
	    (void) execvp(argv[0], argv);
	    quit(1, "%s: execvp: %s\n",
		 program, unix_error_string(errno));
	}

	child = task_by_pid(pid);
	if (!MACH_PORT_VALID(child))
	    quit(1, "%s: task_by_pid(%d)\n", program, pid);

	/* Find out when our child dies. */

	kr = mach_port_request_notification(mach_task_self(), child,
				MACH_NOTIFY_DEAD_NAME, TRUE,
				notify, MACH_MSG_TYPE_MAKE_SEND_ONCE,
				&previous);
	if ((kr != KERN_SUCCESS) || (previous != MACH_PORT_NULL))
	    quit(1, "%s: mach_port_request_notification: %s\n",
		 program, mach_error_string(kr));
    }

    /*
     *	Prepare the name service.
     *	The three do_netname_check_in calls will consume user-refs
     *	for their port args, so we have to generate the refs.
     *	Note that mach_task_self() is just a macro;
     *	it doesn't return a ref.
     */

    netname_init();

    kr = mach_port_mod_refs(mach_task_self(), mach_task_self(),
			    MACH_PORT_RIGHT_SEND, 3);
    if (kr != KERN_SUCCESS)
	quit(1, "%s: mach_port_mod_refs: %s\n",
	     program, mach_error_string(kr));

    kr = mach_port_mod_refs(mach_task_self(), service,
			    MACH_PORT_RIGHT_SEND, 3);
    if (kr != KERN_SUCCESS)
	quit(1, "%s: mach_port_mod_refs: %s\n",
	     program, mach_error_string(kr));

    kr = do_netname_check_in(service, "NameServer",
			     mach_task_self(), service);
    if (kr != KERN_SUCCESS)
	quit(1, "%s: netname_check_in: %s\n",
	     program, mach_error_string(kr));

    kr = do_netname_check_in(service, "NMMonitor",
			     mach_task_self(), service);
    if (kr != KERN_SUCCESS)
	quit(1, "%s: netname_check_in: %s\n",
	     program, mach_error_string(kr));

    kr = do_netname_check_in(service, "NMControl",
			     mach_task_self(), service);
    if (kr != KERN_SUCCESS)
	quit(1, "%s: netname_check_in: %s\n",
	     program, mach_error_string(kr));

    /* Prepare our port set. */

    kr = mach_port_allocate(mach_task_self(),
			    MACH_PORT_RIGHT_PORT_SET, &pset);
    if (kr != KERN_SUCCESS)
	quit(1, "%s: mach_port_allocate: %s\n",
	     program, mach_error_string(kr));

    kr = mach_port_move_member(mach_task_self(), service, pset);
    if (kr != KERN_SUCCESS)
	quit(1, "%s: mach_port_move_member: %s\n",
	     program, mach_error_string(kr));

    kr = mach_port_move_member(mach_task_self(), notify, pset);
    if (kr != KERN_SUCCESS)
	quit(1, "%s: mach_port_move_member: %s\n",
	     program, mach_error_string(kr));

    /* Enter service loop. */

    kr = mach_msg_server(snames_demux, 256, pset);
    quit(1, "%s: mach_msg_server: %s\n", program, mach_error_string(kr));
}

static boolean_t
snames_demux(request, reply)
    mach_msg_header_t *request, *reply;
{
    if (request->msgh_local_port == service)
	return netname_server(request, reply);
    else if (request->msgh_local_port == notify)
	return notify_server(request, reply);
    else
	quit(1, "%s: snames_demux: port = %x\n",
	     program, request->msgh_local_port);
}
