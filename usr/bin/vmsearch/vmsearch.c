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
 * $Log:	vmsearch.c,v $
 * Revision 2.5  91/08/29  15:49:52  rpd
 * 	Moved machid include files into the standard include directory.
 * 	[91/08/29            rpd]
 * 
 * Revision 2.4  91/03/27  17:28:11  mrt
 * 	Changed mach.h include
 * 
 * Revision 2.3  91/03/19  12:33:05  mrt
 * 	Changed to new copyright
 * 
 * Revision 2.2  90/09/12  16:33:27  rpd
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

#define streql(a, b)	(strcmp((a), (b)) == 0)

static mach_port_t server;
static mach_port_t auth;

static vm_offset_t search_vm_region();
static void search_memory();

static void
usage()
{
    quit(1, "usage: vmsearch [-host machine] taskid value\n");
}

void
main(argc, argv)
    int argc;
    char *argv[];
{
    char *hostname = "";
    mtask_t task;
    unsigned int value;
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

    if (argc != 2)
	usage();

    kr = netname_look_up(name_server_port, hostname, "MachID", &server);
    if (kr != KERN_SUCCESS)
	quit(1, "vmread: netname_lookup_up(MachID): %s\n",
	     mach_error_string(kr));

    auth = mach_host_priv_self();
    if (auth == MACH_PORT_NULL)
	auth = mach_task_self();

    task = atoi(argv[0]);
    value = atoh(argv[1]);

    search_memory(task, value);

    exit(0);
}

static void
search_chunk(addr, value, memory, size)
    vm_offset_t addr;
    unsigned int value;
    vm_offset_t memory;
    vm_size_t size;
{
    unsigned int i;

    for (i = 0; i < size; i += sizeof(unsigned int))
	if (* (unsigned int *) (memory + i) == value)
	    printf("%08x\n", addr + i);
}

static vm_offset_t
search_vm_region(task, addr, value)
    mtask_t task;
    vm_offset_t addr;
    unsigned int value;
{
    vm_offset_t memory;
    unsigned int count;
    vm_region_info_t info;
    kern_return_t kr;

    kr = machid_vm_region(server, auth, task, addr, &info);
    if (kr == KERN_NO_SPACE)
	return 0;
    else if (kr != KERN_SUCCESS)
	quit(1, "vmsearch: machid_vm_region: %s\n", mach_error_string(kr));

    kr = machid_vm_read(server, auth, task, info.vr_address, info.vr_size,
			&memory, &count);
    if (kr != KERN_SUCCESS)
	quit(1, "vmsearch: machid_vm_read: %s\n", mach_error_string(kr));
    else if (count != info.vr_size)
	quit(1, "vmsearch: machid_vm_read: bad count (%u != %u)\n",
	     count, info.vr_size);

    search_chunk(info.vr_address, value, memory, info.vr_size);

    kr = vm_deallocate(mach_task_self(), memory, info.vr_size);
    if (kr != KERN_SUCCESS)
	quit(1, "vmsearch: vm_deallocate: %s\n", mach_error_string(kr));

    return info.vr_address + info.vr_size;
}

static void
search_memory(task, value)
    mtask_t task;
    unsigned int value;
{
    vm_offset_t addr;

    for (addr = search_vm_region(task, 0, value);
	 addr != 0;
	 addr = search_vm_region(task, addr, value))
	continue;
}
