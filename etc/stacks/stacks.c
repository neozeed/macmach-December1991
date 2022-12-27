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
 * $Log:	stacks.c,v $
 * Revision 2.5  91/08/29  15:49:23  rpd
 * 	Moved machid include files into the standard include directory.
 * 	[91/08/29            rpd]
 * 
 * Revision 2.4  91/03/27  17:27:47  mrt
 * 	Changed mach.h include
 * 
 * Revision 2.3  91/03/19  12:32:27  mrt
 * 	Changed to new copyright
 * 
 * Revision 2.2  90/11/05  23:34:07  rpd
 * 	Created.
 * 	[90/10/22            rpd]
 * 
 */

#include <mach.h>
#include <mach/message.h>
#include <mach_error.h>
#include <servers/netname.h>
#include <servers/machid.h>
#include <servers/machid_debug.h>

#define streql(a, b)	(strcmp((a), (b)) == 0)

static mach_port_t server;
static mach_port_t auth;

static void
usage()
{
    quit(1, "usage: stacks [-host machine] [pset]\n");
}

main(argc, argv)
    int argc;
    char *argv[];
{
    char *hostname = "";
    mhost_t host;
    mprocessor_set_t pset;
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

    if (argc > 1)
	usage();

    kr = netname_look_up(name_server_port, hostname, "MachID", &server);
    if (kr != KERN_SUCCESS)
	quit(1, "stacks: netname_lookup_up(MachID): %s\n",
	     mach_error_string(kr));

    auth = mach_host_priv_self();
    if (auth == MACH_PORT_NULL)
	auth = mach_task_self();

    switch (argc) {
	mhost_t phost;

      case 0:
	kr = machid_host_ports(server, auth, &host, &phost);
	if (kr != KERN_SUCCESS)
	    quit(1, "stacks: machid_host_ports: %s\n", mach_error_string(kr));

	kr = machid_processor_set_default(server, auth, host, &pset);
	if (kr != KERN_SUCCESS)
	    quit(1, "stacks: machid_processor_set_default: %s\n",
		 mach_error_string(kr));
	break;

      case 1:
	pset = atoi(argv[0]);

	kr = machid_mach_lookup(server, auth, pset, MACH_TYPE_HOST, &host);
	if (kr != KERN_SUCCESS)
	    quit(1, "stacks: machid_mach_lookup: %s\n", mach_error_string(kr));
	break;
    }

{
    vm_size_t reserved;
    unsigned int htotal, ptotal, total;
    vm_size_t hspace, pspace, space;
    vm_size_t hresident, president, resident;
    vm_size_t husage, pusage, usage;
    vm_offset_t hstack, pstack, stack;

    kr = machid_host_stack_usage(server, auth, host, &reserved, &htotal,
				 &hspace, &hresident, &husage, &hstack);
    if (kr != KERN_SUCCESS)
	quit(1, "stacks: machid_host_stack_usage: %s\n",
	     mach_error_string(kr));

    kr = machid_processor_set_stack_usage(server, auth, pset, &ptotal,
					  &pspace, &president,
					  &pusage, &pstack);
    if (kr != KERN_SUCCESS)
	quit(1, "stacks: machid_processor_set_stack_usage: %s\n",
	     mach_error_string(kr));

    total = htotal + ptotal;
    space = hspace + pspace;
    resident = hresident + president;
    if (husage > pusage) {
	usage = husage;
	stack = hstack;
    } else {
	usage = pusage;
	stack = pstack;
    }

    printf("%d kernel stacks:\n", total);
    printf("0x%x resident memory (%d of %d pages)\n",
	   resident, resident / vm_page_size, space / vm_page_size);
    if (usage != 0)
	printf("0x%x + 0x%x = 0x%x maximum usage (stack 0x%x)\n",
	       reserved, usage, reserved + usage, stack);
}
    exit(0);
}
