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
 * $Log:	old_ns_port.c,v $
 * Revision 2.1  90/10/27  20:47:24  dpj
 * Moving under MACH3 source control
 * 
 * Revision 1.4  89/05/02  11:21:03  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * 11-Aug-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Started.
 *
 */
/*
 * old_ns_port.c
 *
 *
 */

#ifndef	lint
static char     rcsid[] = "$Header: /afs/cs.cmu.edu/project/mach-2/rcs/pkg/netmsg/utils/old_ns_port.c,v 2.1 90/10/27 20:47:24 dpj Exp $";
#endif not lint

/*
 * Replaces the new name server port by the old name server port
 * in the registered ports of our parent.
 */


#define DEBUG		1
#define MACH_INIT_SLOTS	1

#include <mach.h>
#include <mach_init.h>
#include <stdio.h>

main()
{
    kern_return_t	kr;
    port_t		old_ns_port;
    port_t		parent_task;
    unsigned int	reg_ports_cnt;
    port_array_t	reg_ports;
    int			parent_pid;

    /*
     * Look up the registered ports.
     */
    if ((kr = mach_ports_lookup(task_self(), &reg_ports, &reg_ports_cnt)) != KERN_SUCCESS) {
	fprintf(stderr, "mach_ports_lookup fails, kr = %d.\n", kr);
	exit(-1);
    }

    /*
     * Get the kernel port for our parent.
     */
    parent_pid = getppid();
    if ((parent_task = task_by_pid(parent_pid)) == PORT_NULL) {
	fprintf(stderr, "task_by_pid fails.\n");
	exit(-1);
    }
#if	DEBUG
    printf("Parent pid = %d, parent port = %x.\n", parent_pid, parent_task);
#endif	DEBUG

    /*
     * Get the old name_server_port from the unused registered slot
     * and place it back in the old name server slot.
     */
    reg_ports[NAME_SERVER_SLOT] = reg_ports[MACH_PORTS_SLOTS_USED];
#if	DEBUG
    printf("reg_ports[%d] = %x.\n", NAME_SERVER_SLOT, reg_ports[NAME_SERVER_SLOT]);
#endif	DEBUG

    /*
     * Register the changed ports with our parent.
     */
    if ((kr = mach_ports_register(parent_task, reg_ports, reg_ports_cnt)) != KERN_SUCCESS) {
	fprintf(stderr, "mach_ports_register fails, kr = %d.\n", kr);
	exit(-1);
    }
#if	DEBUG
    printf("done\n");
#endif	DEBUG

}
