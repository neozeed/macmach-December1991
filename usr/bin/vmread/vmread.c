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
 * $Log:	vmread.c,v $
 * Revision 2.5  91/08/29  15:49:45  rpd
 * 	Moved machid include files into the standard include directory.
 * 	[91/08/29            rpd]
 * 
 * Revision 2.4  91/03/27  17:28:02  mrt
 * 	Changed mach.h include
 * 
 * Revision 2.3  91/03/19  12:32:54  mrt
 * 	Changed to new copyright
 * 
 * Revision 2.2  90/09/12  16:33:20  rpd
 * 	Created.
 * 	[90/06/18            rpd]
 * 
 */

#include <mach.h>
#include <mach/message.h>
#include <mach_error.h>
#include <servers/netname.h>
#include <servers/machid.h>

#define streql(a, b)	(strcmp((a), (b)) == 0)

static mach_port_t server;
static mach_port_t auth;

static void
usage()
{
	quit(1, "usage: vmread [-host machine] taskid addr words\n");
}

main(argc, argv)
    int argc;
    char *argv[];
{
    char *hostname = "";
    vm_offset_t addr, first, last, memory;
    vm_size_t size, memsize;
    mtask_t task;
    int i;
    kern_return_t kr;

    for (i = 1; i < argc; i++)
	if (streql(argv[i], "-host") && (i < argc-1))
	    hostname = argv[++i];
	else if (streql(argv[i], "--")) {
	    i++;
	    break;
	} else if (argv[i][0] == '-')
	    usage();
	else
	    break;

    argc -= i;
    argv += i;

    if (argc != 3)
	usage();

    kr = netname_look_up(name_server_port, hostname, "MachID", &server);
    if (kr != KERN_SUCCESS)
	quit(1, "vmread: netname_lookup_up(MachID): %s\n",
	     mach_error_string(kr));

    auth = mach_host_priv_self();
    if (auth == MACH_PORT_NULL)
	auth = mach_task_self();

    task = atoi(argv[0]);
    addr = atoh(argv[1]);
    size = atoh(argv[2]);

    first = trunc_page(addr);
    last = round_page(addr + size);

    kr = machid_vm_read(server, auth, task, first, last - first,
			&memory, &memsize);
    if (kr != KERN_SUCCESS)
	quit(1, "vmread: machid_vm_read: %s\n", mach_error_string(kr));

    memory += addr - first;

    for (i = 0; i < size/4; i++) {
	if (i % 4 == 0)
	    printf("%08x: ", addr + 4 * i);
	
	printf("%08x", ((unsigned int *) memory)[i]);
	
	if (i % 4 == 3)
	    printf("\n");
	else
	    printf(" ");
    }
    if (i % 4 != 0)
	printf("\n");

    exit(0);
}
