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
 * $Log:	nmprofexit.c,v $
 * Revision 2.1  90/10/27  20:47:16  dpj
 * Moving under MACH3 source control
 * 
 * Revision 1.4  89/05/02  11:20:29  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * 22-May-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Started.
 *
 */
/*
 * nmprofexit.c
 *
 *
 */

#ifndef	lint
static char     rcsid[] = "$Header: /afs/cs.cmu.edu/project/mach-2/rcs/pkg/netmsg/utils/nmprofexit.c,v 2.1 90/10/27 20:47:16 dpj Exp $";
#endif not lint

/*
 * Program to make a profiling network server exit.
 */


#include <mach.h>
#include <stdio.h>
#include <mach/message.h>

#define NETNAME_NAME	"NEW_NETWORK_NAME_SERVER"

main() {
    port_t		prof_port, network_server_port;
    msg_header_t	prof_msg;
    kern_return_t	kr;

    /*
     * Look up the network server.
     */
    if ((kr = netname_look_up(name_server_port, "", NETNAME_NAME, &network_server_port)) == KERN_SUCCESS) {
	printf("Look up of new network server succeeds, port = %d.\n", network_server_port);
    }
    else {
	network_server_port = name_server_port;
	printf("Look up of new network server fails, using standard server, port = %d.\n",
			network_server_port);
    }
    if ((kr = netname_look_up(network_server_port, "", "PROF_EXIT", &prof_port)) == KERN_SUCCESS) {
	printf("Look up of profiling port succeeds - sending message.\n");
	prof_msg.msg_type = MSG_TYPE_NORMAL;
	prof_msg.msg_simple = TRUE;
	prof_msg.msg_local_port = PORT_NULL;
	prof_msg.msg_remote_port = prof_port;
	prof_msg.msg_size = sizeof(msg_header_t);
	if ((kr = msg_send(&prof_msg, MSG_OPTION_NONE, 0)) == KERN_SUCCESS) {
	    printf("Profiling exit message sent successfully.\n");
	}
	else {
	    fprintf(stderr, "msg_send fails, kr = %d.\n", kr);
	}
    }
}

