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
 * $Log:	vminfo.c,v $
 * Revision 2.7  91/08/30  15:41:35  rpd
 * 	Updated from vio_single_use to vio_use_old_pageout.
 * 	[91/08/30            rpd]
 * 
 * Revision 2.6  91/08/29  15:49:37  rpd
 * 	Moved machid include files into the standard include directory.
 * 	[91/08/29            rpd]
 * 
 * Revision 2.5  91/03/27  17:27:57  mrt
 * 	Changed mach.h include
 * 
 * Revision 2.4  91/03/19  12:32:44  mrt
 * 	Changed to new copyright
 * 
 * Revision 2.3  90/11/05  23:34:20  rpd
 * 	Removed vir_copy_on_write.
 * 	[90/10/29            rpd]
 * 
 * Revision 2.2  90/09/12  16:33:13  rpd
 * 	Created.
 * 	[90/06/18            rpd]
 * 
 */

#include <stdio.h>
#include <strings.h>
#include <mach.h>
#include <mach_debug/mach_debug.h>
#include <mach_error.h>
#include <servers/netname.h>
#include <servers/machid.h>
#include <servers/machid_debug.h>

#define streql(a, b)	(strcmp((a), (b)) == 0)

static void print_vm_info();
static vm_offset_t print_vm_region(), print_vm_region_info();

static mach_port_t server;
static mach_port_t auth;

static boolean_t TaskHold = FALSE;
static boolean_t RegionInfo = FALSE;
static boolean_t Verbose = FALSE;

static void
usage()
{
    quit(1, "usage: vminfo [-host machine] [-d] [-v] [-z] [taskid]\n");
}

static void
parse_args(argc, argv, taskp)
    int argc;
    char *argv[];
    mtask_t *taskp;
{
    char *hostname = "";
    mtask_t task;
    kern_return_t kr;
    int i;

    for (i = 1; i < argc; i++)
	if (streql(argv[i], "-host") && (i < argc-1))
	    hostname = argv[++i];
	else if (streql(argv[i], "-z"))
	    TaskHold = TRUE;
	else if (streql(argv[i], "-Z"))
	    TaskHold = FALSE;
	else if (streql(argv[i], "-d"))
	    RegionInfo = TRUE;
	else if (streql(argv[i], "-D"))
	    RegionInfo = FALSE;
	else if (streql(argv[i], "-v"))
	    Verbose = TRUE;
	else if (streql(argv[i], "-V"))
	    Verbose = FALSE;
	else if (streql(argv[i], "--")) {
	    i++;
	    break;
	} else if (argv[i][0] == '-')
	    usage();
	else
	    break;

    kr = netname_look_up(name_server_port, hostname, "MachID", &server);
    if (kr != KERN_SUCCESS)
	quit(1, "vminfo: netname_lookup_up(MachID): %s\n",
	     mach_error_string(kr));

    auth = mach_host_priv_self();
    if (auth == MACH_PORT_NULL)
	auth = mach_task_self();

    switch (argc - i) {
      case 0:
	kr = machid_mach_register(server, auth, mach_task_self(),
				  MACH_TYPE_TASK, &task);
	if (kr != KERN_SUCCESS)
	    quit(1, "vminfo: machid_mach_register: %s\n",
		 mach_error_string(kr));
	break;

      case 1:
	task = atoi(argv[i]);
	break;

      default:
	usage();
    }

    *taskp = task;
}

void
main(argc, argv)
    int argc;
    char *argv[];
{
    mtask_t task;
    kern_return_t kr;

    parse_args(argc, argv, &task);

    if (TaskHold) {
	kr = machid_task_suspend(server, auth, task);
	if (kr != KERN_SUCCESS)
	    quit(1, "vminfo: machid_task_suspend: %s\n",
		 mach_error_string(kr));
    }

    print_vm_info(task, RegionInfo ? print_vm_region_info : print_vm_region);

    if (TaskHold) {
	kr = machid_task_resume(server, auth, task);
	if (kr != KERN_SUCCESS)
	    quit(1, "vminfo: machid_task_resume: %s\n",
		 mach_error_string(kr));
    }

    exit(0);
}

static char *inherit_name[] =
	{ "both", "copy", "none", "move" };
static char *prot_name[] =
	{ "   ", "R  ", " W ", "RW ", "  X", "R X", " WX", "RWX" };
static char *copy_name[] =
	{ "none", "call", "delay" };

static vm_offset_t
print_vm_region(task, addr)
    mtask_t task;
    vm_offset_t addr;
{
    vm_region_info_t info;
    kern_return_t kr;

    kr = machid_vm_region(server, auth, task, addr, &info);
    if (kr == KERN_NO_SPACE)
	return 0;
    else if (kr != KERN_SUCCESS)
	quit(1, "vminfo: machid_vm_region: %s\n",
	     mach_error_string(kr));

    printf("Region %08x-%08x: ", info.vr_address,
	   info.vr_address + info.vr_size);
    printf("prot=%s/%s/%s, ",
	   prot_name[info.vr_prot],
	   prot_name[info.vr_max_prot],
	   inherit_name[info.vr_inherit]);
    printf("name=%4d, offset=%x\n", info.vr_name, info.vr_offset);

    return info.vr_address + info.vr_size;
}

static void
print_region(region)
    vm_info_region_t region;
{
    printf("Region %08x-%08x: ",
	   region.vir_start, region.vir_end);
    printf("prot=%s/%s/%s",
	   prot_name[region.vir_protection],
	   prot_name[region.vir_max_protection],
	   inherit_name[region.vir_inheritance]);
    if ((region.vir_wired_count != 0) ||
	(region.vir_user_wired_count != 0))
	printf(", wired=%u/%u",
	       region.vir_wired_count, region.vir_user_wired_count);
    if (region.vir_object != 0)
	printf(", offset=%x", region.vir_offset);
    if (region.vir_needs_copy)
	printf(", copy needed");
}

static void
print_object(object)
    vm_info_object_t object;
{
    printf("    Object %08x: size = %x, %u refs, %u pages",
	   object.vio_object, object.vio_size,
	   object.vio_ref_count, object.vio_resident_page_count);
    printf(", %u absent, %u paging ops\n",
	   object.vio_absent_count, object.vio_paging_in_progress);

    if (object.vio_temporary)
	printf("      temp");
    else
	printf("      perm");
    if (object.vio_internal)
	printf(", internal");
    else
	printf(", external");
    if (object.vio_can_persist)
	printf(", cache");
    if (object.vio_pager_created) {
	printf(", created");
	if (!object.vio_pager_ready)
	    printf(", not ready");
	if (!object.vio_pager_initialized)
	    printf(", uninitialized");
    } else {
	if (object.vio_pager_ready)
	    printf(", ready");
	if (object.vio_pager_initialized)
	    printf(", initialized");
    }
    if (object.vio_lock_in_progress)
	printf(", locking");
    if (object.vio_lock_restart)
	printf(", restart");
    if (object.vio_use_old_pageout)
	printf(", old pageout");
    if (!object.vio_alive)
	printf(", dead");

    if (object.vio_copy_strategy != 0)
	printf(", copy %s", copy_name[object.vio_copy_strategy]);
    if (object.vio_paging_offset != 0)
	printf(", paging offset = %x", object.vio_paging_offset);
    if (object.vio_shadow != 0)
	printf(", shadow = %08x (%x)",
	       object.vio_shadow, object.vio_shadow_offset);
    if (object.vio_copy != 0)
	printf(", copy = %08x", object.vio_copy);
    printf(", alloc = %x", object.vio_last_alloc);
    printf("\n");
}

static vm_offset_t
print_vm_region_info(task, addr)
    mtask_t task;
    vm_offset_t addr;
{
    vm_info_region_t region;
    vm_info_object_t *objects;
    unsigned int objectCnt;
    kern_return_t kr;

    kr = machid_vm_region_info(server, auth, task, addr,
			       &region, &objects, &objectCnt);
    if (kr == KERN_NO_SPACE)
	return 0;
    else if (kr != KERN_SUCCESS)
	quit(1, "vminfo: machid_vm_region_info: %s\n",
	     mach_error_string(kr));

    print_region(region);

    if (Verbose) {
	int i;

	printf("\n");
	for (i = 0; i < objectCnt; i++)
	    print_object(objects[i]);
    } else {
	if (objectCnt > 0)
	    printf(", %u objects", objectCnt);
	printf("\n");
    }

    kr = vm_deallocate(mach_task_self(), (vm_offset_t) objects,
		       (vm_size_t) (objectCnt * sizeof *objects));
    if (kr != KERN_SUCCESS)
	quit(1, "vminfo: vm_deallocate: %s\n",
	     mach_error_string(kr));

    return region.vir_end;
}

static void
print_vm_info(task, func)
    mtask_t task;
    vm_offset_t (*func)();
{
    vm_offset_t addr;

    for (addr = (*func)(task, 0); addr != 0; addr = (*func)(task, addr))
	continue;
}
