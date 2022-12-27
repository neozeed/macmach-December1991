/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	nmserver.c,v $
 * Revision 2.1  90/10/27  20:45:28  dpj
 * Moving under MACH3 source control
 * 
 * Revision 1.16.1.1.1.5  90/10/21  21:55:15  dpj
 * 	Removed automatic aggregate initialization of time_value.
 * 
 * Revision 1.16.1.1.1.4  90/10/07  13:54:16  dpj
 * 	Re-worked to deal with the various MACH3-related configurations.
 * 	New arguments parsing.
 * 
 * Revision 1.16.1.1.1.3  90/08/15  14:59:24  dpj
 * 	Updated for MACH3_SA.
 * 
 * Revision 1.16.1.1.1.2  90/08/02  20:23:28  dpj
 * 	Use explicit port set instead of port_enable/port_disable.
 * 	MACH3_DEBUG: added "-I" and "-P" switches to operate in the
 * 	Mach 3.0 environment.
 * 	[90/06/03  17:40:08  dpj]
 * 
 * Revision 1.16.1.1.1.1  90/07/30  14:04:18  dpj
 * 	Added "-i" switch for own IP address.
 * 	Handle errors with "-I" and "-i" switches.
 * 
 * Revision 1.16  89/05/02  11:14:31  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * Revision 1.15  89/04/24  20:41:13  dpj
 * 	Changes from NeXT as of 88/09/30
 * 	[89/04/19  17:55:58  dpj]
 * 
 * Revision 1.14  88/10/08  22:30:41  dpj
 * 	Modified the declaration of cthread_debug to work with a threads
 * 	library with no debugging.
 * 
 *  5-Sep-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Modified the declaration of cthread_debug to work with a threads
 *	library with no debugging.
 *
 * 21-Jul-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	NOTIFY: allocate the notify port for the compatibility netmsgserver.
 *
 * 23-Jun-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Added printout of a timestamp on startup.
 *
 *  5-Apr-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Added code to exec the old netmsgserver if the kernel is not good
 *	enough to support the new netmsgserver. (COMPAT)
 *
 * 29-Mar-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Modified logstat initialization so that tracing can be
 *	turned on very early.
 *
 * 14-Mar-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Added code to read a configuration file.
 *
 * 27-Feb-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Added "-C" switch to control compatibility mode. (COMPAT)
 *
 * 21-Jul-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Added call to cthread_exit() at the end of main.
 *
 * 20-May-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Allow print_level to be set from command line.
 *
 * 16-Apr-87  Daniel Julin (dpj) at Carnegie Mellon University
 *	Fixed to use standard logstat technology for messages.
 *
 *  2-Jan-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Started.
 *
 */
/*
 * nmserver.c
 *
 *
 */

#ifndef	lint
char nmserver_rcsid[] = "$Header: /afs/cs.cmu.edu/project/mach-2/rcs/pkg/netmsg/server/nmserver.c,v 2.1 90/10/27 20:45:28 dpj Exp $";
#endif not lint

/*
 * Main program for the network server - implements full IPC service.
 */


#include <cthreads.h>
#include <stdio.h>
#include <mach.h>
#include <mach/message.h>
#include <sys/signal.h>
#if	MACH3_SA || MACH3_US
#include <mach/time_value.h>
#endif	MACH3_SA || MACH3_US

#undef	NET_DEBUG
#define	NET_DEBUG	1
#undef	NET_PRINT
#define	NET_PRINT	1

#include "debug.h"
#include "ls_defs.h"
#include "netmsg.h"
#include "network.h"
#include "nm_init.h"

int				cthread_debug;


#if	USE_NETIPC_MACH3
/*
 * Global variables set from the command line.
 */
char				*mach3_if_name = NULL;
netaddr_t			mach3_ip_addr = 0;
char				mach3_net_server_name_data[100];
char				*mach3_net_server_name;
#endif	USE_NETIPC_MACH3


#if	COMPAT
extern char			compat_server[];
extern int			errno;
extern int			exit_handler();
#endif	COMPAT


/*
 * netmsg_usage
 */
netmsg_usage(program)
	char			*program;
{
	fprintf(stderr,"Usage: %s \n",program);
#if	MACH3
#if	USE_NETIPC_MACH3
	fprintf(stderr,
"        -I <interface name> -i <IP address>\n");
	fprintf(stderr,
"        [-netserver <net server name>]\n");
#endif	USE_NETIPC_MACH3
#if	MACH3_UNIX
	fprintf(stderr,
"        [-unix]                                -- output channel\n");
#endif	MACH3_UNIX
#if	MACH3_VUS
	fprintf(stderr,
"        [-console|-unix]                       -- output channel\n");
#endif	MACH3_VUS
#if	MACH3_US || MACH3_SA
	fprintf(stderr,
"        [-console]                             -- output channel\n");
#endif	MACH3_US || MACH3_SA
	fprintf(stderr,
"        [-stop]                                -- stop after startup\n");
#endif	MACH3
	fprintf(stderr,
"        [-p <print level>]\n");
#if	(! defined(MACH3)) || MACH3_UNIX || MACH3_VUS
	fprintf(stderr,
"        [-f <configuration file>]\n");
#endif	(! defined(MACH3)) || MACH3_UNIX || MACH3_VUS
	fprintf(stderr,
"        [-c]                                   -- cthread debugging\n");
	fprintf(stderr,
"        [-t]                                   -- tracing\n");
#if	COMPAT
	fprintf(stderr,
"        [-C]                                   -- compatibility mode\n");
#endif	COMPAT

	_exit(1);
}


/*
 * main
 *
 */
main(argc, argv)
	int			argc;
	char			**argv;
{
	char			*program = argv[0];
	int			print_level = -1;
	char			*config_file = NULL;
#if	COMPAT
	int			compat_mode = -1;
#endif	COMPAT
#if	MACH3_SA || MACH3_US
	time_value_t		tv;
#else	MACH3_SA || MACH3_US
	long			clock;
#endif	MACH3_SA || MACH3_US
#if	MACH3
#if	USE_NETIPC_MACH3
	int			have_mach3_if_name = 0;
	int			have_mach3_ip_addr = 0;
#endif	USE_NETIPC_MACH3
	int			stop = 0;
#endif	MACH3

#if	MACH3_SA || MACH3_US
	/*
	 * This program may be needed to access the Diag server over
	 * the network. Always start with output on the console to
	 * avoid deadlocks.
	 */
	mach3_output_console();
#endif	MACH3_SA || MACH3_US

	/*
	 * Random setup.
	 */
#if	USE_NETIPC_MACH3
	mach3_net_server_name = mach3_net_server_name_data;
	strcpy(mach3_net_server_name,"net_server-mig");
#endif	USE_NETIPC_MACH3
#if	NeXT
	signal(SIGHUP, SIG_IGN);
#else	NeXT

	/*
	 * Initialise the cthreads package.
	 */
	cthread_init();
#endif	NeXT

	/*
	 * Initialise the stuff needed for debugging.
	 */
	if (!(ls_init_1())) {
		panic("ls_init_1 failed.");
	}

	/*
	 * Parse arguments.
	 */
	argc--;
	argv++;
	for (; argc > 0; argc--, argv++) {
#if	MACH3
#if	USE_NETIPC_MACH3
		if (!strcmp(argv[0],"-I")) {
			argc--;
			argv++;
			if (argc <= 0) netmsg_usage(program);
			mach3_if_name = argv[0];
			have_mach3_if_name = 1;
			continue;
		}

		if (!strcmp(argv[0],"-i")) {
			argc--;
			argv++;
			if (argc <= 0) netmsg_usage(program);
			mach3_ip_addr = (netaddr_t) inet_addr(argv[0]);
			have_mach3_ip_addr = 1;
			continue;
		}

		if (!strcmp(argv[0],"-netserver")) {
			argc--;
			argv++;
			if (argc <= 0) netmsg_usage(program);
			mach3_net_server_name = argv[0];
			continue;
		}
#endif	USE_NETIPC_MACH3

#if	MACH3_VUS || MACH3_US || MACH3_SA
		if (!strcmp(argv[0],"-console")) {
			mach3_output_console();
			continue;
		}
#endif	MACH3_VUS || MACH3_US || MACH3_SA

#if	MACH3_UNIX || MACH3_VUS
		if (!strcmp(argv[0],"-unix")) {
			mach3_output_unix();
			continue;
		}
#endif	MACH3_UNIX || MACH3_VUS

		if (!strcmp(argv[0],"-stop")) {
			stop = 1;
			continue;
		}
#endif	MACH3

#if	(! defined(MACH3)) || MACH3_UNIX || MACH3_VUS
		if (!strcmp(argv[0],"-f")) {
			argc--;
			argv++;
			if (argc <= 0) netmsg_usage(program);
			config_file = argv[0];
			continue;
		}
#endif	(! defined(MACH3)) || MACH3_UNIX || MACH3_VUS

		if (!strcmp(argv[0],"-c")) {
			cthread_debug = 1;
			continue;
		}

		if (!strcmp(argv[0],"-t")) {
			tracing_on = 1;
			argc--;
			argv++;
			if (argc <= 0) netmsg_usage(program);
			continue;
		}

		if (!strcmp(argv[0],"-p")) {
			argc--;
			argv++;
			if (argc <= 0) netmsg_usage(program);
			print_level = atoi(argv[0]);
			continue;
		}

#if	COMPAT
		if (!strcmp(argv[0],"-C")) {
			compat_mode = 1;
			continue;
		}
#endif	COMPAT

		/*
		 * Unknown argument.
		 */
		netmsg_usage(program);
		break;
	}

#if	MACH3
#if	USE_NETIPC_MACH3
	if ((! have_mach3_ip_addr) || (!(have_mach3_if_name))) {
		netmsg_usage(program);
	}
#endif	USE_NETIPC_MACH3

	/*
	 * Pause to allow to attach a debugger.
	 */
	if (stop) {
		fprintf(stderr,
		"netmsgserver suspended. Awaiting restart from debugger.\n");
#if	MACH3_UNIX
		task_suspend(task_self());
#endif	MACH3_UNIX
#if	MACH3_VUS || MACH3_US || MACH3_SA
		task_suspend(mach_task_self());
#endif	MACH3_VUS || MACH3_US || MACH3_SA
	}
#endif	MACH3

	if (ls_read_config_file(config_file) != TRUE) {
		fprintf(stderr,
			"%s: Invalid configuration file - Exiting\n",program);
		(void)fflush(stderr);
		_exit(-1);
	}
	if (print_level != -1)
		debug.print_level = print_level;
#if	COMPAT
	if (compat_mode != -1)
		param.compat = compat_mode;
#endif	COMPAT

#if	MACH3_SA || MACH3_US
#else	MACH3_SA || MACH3_US
	/*
	 * Initialize syslog if appropriate.
	 */
	if (param.syslog) {
		openlog("netmsgserver", LOG_PID | LOG_NOWAIT, LOG_USER);
	}
#endif	MACH3_SA || MACH3_US

	/*
	 * Put out a timestamp when starting
	 */
#if	MACH3_SA || MACH3_US
	tv.seconds = 0;
	tv.microseconds = 0;
	(void) mach_get_time(&tv);
	fprintf(stdout, "Started %s , timestamp=(%d,%d)\n", program,
						tv.seconds,tv.microseconds);
#else	MACH3_SA || MACH3_US
	clock = time(0);
	fprintf(stdout, "Started %s at %24.24s \n", program, ctime(&clock));
	if(param.syslog) {
		syslog(LOG_INFO, "Started %s at %24.24s ",
						program, ctime(&clock));
	}
#endif	MACH3_SA || MACH3_US

	fprintf(stdout, "%s: %s %s print_level = %d.\n", program,
				(tracing_on ? "tracing" : ""),
				(cthread_debug ? "cthread_debug" : ""),
				debug.print_level);
	(void)fflush(stdout);

#if	COMPAT
	/*
	 * This ugly code to allow the network server to start up on
	 * machines that do not have an up-to-date Mach kernel.
	 */
	{
		struct thread_basic_info	ti;
		int				count;
		kern_return_t			kr;

		count = sizeof(struct thread_basic_info);
		kr = thread_info(thread_self(),THREAD_BASIC_INFO,&ti,&count);
		if (kr != KERN_SUCCESS) {
			ERROR((msg,
	"*** This machine does not run a good Mach kernel ***"));
			ERROR((msg,
	"*** Starting the old netmsgserver standalone ***"));
			execl(compat_server,"old_netmsgserver",0);
			ERROR((msg,
	"execl of old netmsgserver failed, errno=%d", errno));
			_exit(2);
		}
	}

	/*
	 * Fork a process for the old netmsgserver in compatibility mode.
	 * We fork here before we are multi-threaded, but the child will wait
	 * until we are ready by looking at a flag.
	 */
	if (param.compat) {
		extern int	compat_pid;

		/*
		 * First establish signal handlers to kill the child
		 * if we have to exit.
		 */
#define	HANDLER(sig)	{						\
	if (signal(sig,exit_handler) == (int (*)()) -1) {		\
		ERROR((msg,"netname_init.signal(sig) failed, errno=%d"));\
		panic("netname_init.signal");				\
	}								\
}
		HANDLER(SIGHUP);
		HANDLER(SIGINT);
		HANDLER(SIGQUIT);
		HANDLER(SIGILL);
		HANDLER(SIGIOT);
		HANDLER(SIGEMT);
		HANDLER(SIGFPE);
		HANDLER(SIGBUS);
		HANDLER(SIGSEGV);
		HANDLER(SIGSYS);
		HANDLER(SIGTERM);
		HANDLER(SIGURG);
		HANDLER(SIGIO);
		HANDLER(SIGCHLD);
#undef	HANDLER

		/*
		 * Fork the old server.
		 */
		compat_pid = fork();
		if (compat_pid < 0) {
			ERROR((msg,
	"netname_init.fork: cannot start an old network server (COMPAT),
							errno=%d",errno));
			panic("cannot fork");
		}
		if (compat_pid == 0) {
			msg_header_t	sleep_msg;
			kern_return_t	kr;
			int		fd;
			port_t		notify_port;

			/*
			 * Child. Close all non-essential file descriptors,
			 * wait for the go signal,
			 * then exec the old network server.
			 */
			for (fd = getdtablesize(); fd > 2; fd--)
				close(fd);

#if	NOTIFY
			kr = port_allocate(task_self(),&notify_port);
			if (kr != KERN_SUCCESS) {
				ERROR((msg,
	"[COMPAT,NOTIFY] port_allocate(notify_port) failed,kr=%d",kr));
				_exit(0);
			}
			kr = task_set_special_port(task_self(),
						TASK_NOTIFY_PORT,notify_port);
			if (kr != KERN_SUCCESS) {
				ERROR((msg,
	"[COMPAT,NOTIFY] task_set_special_port(notify_port) failed,kr=%d",kr));
				_exit(0);
			}
#else	NOTIFY
			notify_port = task_notify();
#endif	NOTIFY

			sleep_msg.msg_size = sizeof(msg_header_t);
			sleep_msg.msg_local_port = notify_port;
			kr = msg_receive(&sleep_msg, RCV_TIMEOUT, 60000);
			if (kr == KERN_SUCCESS) {
				execl(compat_server,"old_netmsgserver",0);
				ERROR((msg,
			"execl of old netmsgserver failed, errno=%d", errno));
				_exit(0);
			} else {
				ERROR((msg,
	"old netmsgserver timed-out while waiting for the GO signal, kr=%d\n",
									kr));
				_exit(0);
			}
		} else {
			/*
			 * Parent.
			 */
			ERROR((msg,
	"Started old netmsgserver process for compatibility mode, pid=%d",
								compat_pid));
		}
	}
#endif	COMPAT

	if (nm_init()) {
		ERROR((msg,"Network Server initialised."));
		DEBUG0(TRUE, 3, 1260);
		DEBUG_NETADDR(TRUE, 3, my_host_id);
	} else {
		panic("Network Server initialisation failed.");
	}

#if	PROF
	{
		/*
		 * Wait for ever so that we do not drop off the end of main.
		 */
		port_t		prof_port;
		msg_header_t	prof_msg;
		(void)port_allocate(task_self(), &prof_port);
		_netname_check_in(PORT_NULL,"PROF_EXIT",task_self(),prof_port);
		prof_msg.msg_size = sizeof(prof_msg);
		prof_msg.msg_local_port = prof_port;
		msg_receive(&prof_msg, MSG_OPTION_NONE, 0);
		fprintf(stderr, "Received profile exit message - exitting.\n");
		_exit(1);
	}
#endif	PROF

	cthread_exit();
}
