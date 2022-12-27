/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * File: $Source: /afs/cs.cmu.edu/project/mach-2/rcs/pkg/netmsg/utils/nmprotocol.c,v $
 *
 * Purpose: dynamically change the IPC transport protocol
 *		used by a netmsgserver.
 *
 * HISTORY
 * $Log:	nmprotocol.c,v $
 * Revision 2.1  90/10/27  20:47:17  dpj
 * Moving under MACH3 source control
 * 
 * Revision 2.1.1.1  90/10/21  21:55:59  dpj
 * 	First working version.
 * 
 *
 */

#include	<stdio.h>
#include	<mach.h>
#include	"ls_defs.h"
#include	"logstat.h"
#include	"transport.h"
#include	<servers/netname.h>

#define NETNAME_NAME	"NEW_NETWORK_NAME_SERVER"


nmprotocol_usage()
{
	fprintf(stderr,"Usage: nmprotocol\n");
#if	MACH3
	fprintf(stderr,"                 [-stop]\n");
#if	MACH3_VUS || MACH3_US || MACH3_SA
	fprintf(stderr,"                 [-console]\n");
#endif	MACH3_VUS || MACH3_US || MACH3_SA
#endif	MACH3
	fprintf(stderr,"                 [-host <hostname>]\n");
	fprintf(stderr,"                 tcp|deltat|<protocol-num>\n");

	exit(1);
}


main(argc, argv)
	int			argc;
	char			**argv;
{
	int			stop = 0;
	char			*host = "127.0.0.1";
	int			new_protocol = 0;
	int			old_protocol;
	kern_return_t		kr;
	port_t			logstat_port;
	param_ptr_t		param_ptr;
	unsigned int		param_ptr_size;

	/*
	 * Parse arguments.
	 */
	argc--;
	argv++;
	for (; argc > 0; argc--, argv++) {
#if	MACH3
		if (!strcmp(argv[0],"-stop")) {
			stop = 1;
			continue;
		}

#if	MACH3_VUS || MACH3_US || MACH3_SA
		if (!strcmp(argv[0],"-console")) {
			mach3_output_console();
			continue;
		}
#endif	MACH3_VUS || MACH3_US || MACH3_SA
#endif	MACH3

		if (!strcmp(argv[0],"-host")) {
			argc--;
			argv++;
			if (argc <= 0) nmprotocol_usage();
			host = argv[0];
			continue;
		}

		if (!strcmp(argv[0],"tcp")) {
			if (new_protocol != 0) nmprotocol_usage();
			new_protocol = TR_TCP_ENTRY;
			continue;
		}

		if (!strcmp(argv[0],"deltat")) {
			if (new_protocol != 0) nmprotocol_usage();
			new_protocol = TR_DELTAT_ENTRY;
			continue;
		}

		/*
		 * Non-switch args.
		 */
		if ((new_protocol != 0) ||
				((new_protocol = atoi(argv[0])) == 0)) {
			nmprotocol_usage();
			break;
		}
	}

#if	MACH3
	/*
	 * Pause to allow to attach a debugger.
	 */
	if (stop) {
		fprintf(stderr,
		"nmprotocol suspended. Awaiting restart from debugger.\n");
#if	MACH3_UNIX
		task_suspend(task_self());
#endif	MACH3_UNIX
#if	MACH3_VUS || MACH3_US || MACH3_SA
		task_suspend(mach_task_self());
#endif	MACH3_VUS || MACH3_US || MACH3_SA
	}
#endif	MACH3

	/*
	 * Look-up the logstat port.
	 */
	kr = netname_look_up(name_server_port,host,"NM_LOGSTAT",&logstat_port);
	if (kr != KERN_SUCCESS) {
		mach_error("Cannot find netmsgserver control port",kr);
		exit(1);
	}

	/*
	 * Get the current parameters.
	 */
	kr = ls_sendparam(logstat_port,&param_ptr,&param_ptr_size);
	if (kr != KERN_SUCCESS) {
		mach_error("Cannot get current operating parameters",kr);
		exit(1);
	}
	old_protocol = param_ptr->transport_default;

	if (new_protocol != 0) {
		/*
		 * Change the transport protocol.
		 */
		param_ptr->transport_default = new_protocol;
		kr = ls_setparam(logstat_port,param_ptr,param_ptr_size);
		if (kr != KERN_SUCCESS) {
			mach_error("Cannot set new operating parameters",kr);
			exit(1);
		}
		fprintf(stdout,
			"Netmsgserver IPC transport protocol is %d (was %d)\n",
						new_protocol,old_protocol);
	} else {
		fprintf(stdout,"Netmsgserver IPC transport protocol is %d\n",
								old_protocol);
	}
}
