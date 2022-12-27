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
 * $Log:	waitfor.c,v $
 * Revision 2.2  91/08/29  16:09:31  rpd
 * 	Created.
 * 	[91/08/23            rpd]
 * 
 */

#include <stdio.h>
#include <strings.h>
#define MACH_INIT_SLOTS		1	/* we want NAME_SERVER_SLOT, etc. */
#include <mach.h>
#include <mach_error.h>
#include <servers/service.h>

#define streql(a, b)	(strcmp((a), (b)) == 0)

static void
usage()
{
    quit(1, "usage: waitfor [netname|service|envmgr|slot slot-num]\n");
}

main(argc, argv)
    int argc;
    char *argv[];
{
    mach_port_t *ports;
    unsigned int portsCnt;
    mach_port_t port;
    kern_return_t kr;
    int slot;

    if (argc < 2)
	usage();

    if (streql(argv[1], "slot")) {
	if (argc != 3)
	    usage();

	slot = atoi(argv[2]);
    } else if (streql(argv[1], "netname")) {
	if (argc != 2)
	    usage();

	slot = NAME_SERVER_SLOT;
    } else if (streql(argv[1], "service")) {
	if (argc != 2)
	    usage();

	slot = SERVICE_SLOT;
    } else if (streql(argv[1], "envmgr")) {
	if (argc != 2)
	    usage();

	slot = ENVIRONMENT_SLOT;
    } else
	usage();

    kr = mach_ports_lookup(mach_task_self(), &ports, &portsCnt);
    if (kr != KERN_SUCCESS)
	quit(1, "waitfor: mach_ports_lookup: %s\n", mach_error_string(kr));

    if ((slot < 0) || (slot >= portsCnt))
	quit(1, "waitfor: bad slot number: %d\n", slot);

    port = ports[slot];
    if (!MACH_PORT_VALID(port))
	quit(1, "waitfor: no port at slot %d\n", slot);

    kr = service_waitfor(service_port, port);
    if (kr != KERN_SUCCESS)
	quit(1, "waitfor: service_waitfor: %s\n", mach_error_string(kr));

    exit(0);
}
