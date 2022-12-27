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
 * $Log:	pinfo.c,v $
 * Revision 2.5  91/08/29  15:49:14  rpd
 * 	Moved machid include files into the standard include directory.
 * 	[91/08/29            rpd]
 * 
 * 	Updated for sequence numbers.
 * 	[91/08/11            rpd]
 * 
 * Revision 2.4  91/03/27  17:27:39  mrt
 * 	Changed mach.h include
 * 
 * Revision 2.3  91/03/19  12:32:15  mrt
 * 	Changed to new copyright
 * 
 * Revision 2.2  90/09/12  16:32:56  rpd
 * 	Created.
 * 	[90/06/18            rpd]
 * 
 */

#include <stdio.h>
#include <strings.h>
#include <math.h>
#include <mach.h>
#include <mach_debug/mach_debug.h>
#include <mach_error.h>
#include <servers/netname.h>
#include <servers/machid.h>
#include <servers/machid_debug.h>

#define streql(a, b)	(strcmp((a), (b)) == 0)

static char *port_special_string();
static char *port_type_string();
static void print_status_info();
static void print_info_dn();
static void print_info();

static void print_space_info();
static void print_task_info();
static void print_task_name_info();

static mach_port_t server;
static mach_port_t auth;

static boolean_t Verbose = FALSE;
static boolean_t TaskHold = FALSE;
static boolean_t CheckSpecialName = FALSE;
static boolean_t SpaceInfo = FALSE;

static void
usage()
{
    quit(1, "usage: pinfo [-host machine] [-v] [-d] [-z] [taskid [port-name]]\n");
}

static void
parse_args(argc, argv, taskp, name)
    int argc;
    char *argv[];
    mtask_t *taskp;
    mach_port_t *name;
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
	    SpaceInfo = TRUE;
	else if (streql(argv[i], "-D"))
	    SpaceInfo = FALSE;
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
	quit(1, "pinfo: netname_lookup_up(MachID): %s\n",
	     mach_error_string(kr));

    auth = mach_host_priv_self();
    if (auth == MACH_PORT_NULL)
	auth = mach_task_self();

    *name = MACH_PORT_NULL;

    switch (argc - i)
    {
      case 0:
	kr = machid_mach_register(server, auth, mach_task_self(),
				  MACH_TYPE_TASK, &task);
	if (kr != KERN_SUCCESS)
	    quit(1, "pinfo: machid_mach_register: %s\n",
		 mach_error_string(kr));
	CheckSpecialName = TRUE;
	break;

      case 2:
	if (SpaceInfo)
	    usage();

	*name = atoh(argv[i+1]);
	/* fall-through */

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
    mach_port_t name;

    parse_args(argc, argv, &task, &name);

    if (TaskHold) {
	kern_return_t kr;

	kr = machid_task_suspend(server, auth, task);
	if (kr != KERN_SUCCESS)
	    quit(1, "pinfo: machid_task_suspend: %s\n",
		 mach_error_string(kr));
    }

    if (SpaceInfo)
	print_space_info(task);
    else if (name == MACH_PORT_NULL)
	print_task_info(task);
    else
	print_task_name_info(task, name);

    if (TaskHold) {
	kern_return_t kr;

	kr = machid_task_resume(server, auth, task);
	if (kr != KERN_SUCCESS)
	    quit(1, "pinfo: machid_task_resume: %s\n",
		 mach_error_string(kr));
    }

    exit(0);
}

static char *
port_special_string(name)
    mach_port_t name;
{
    if (name == mach_task_self())
	return "mach_task_self";
    else if (name == mig_get_reply_port())
	return "mig reply port";
    else if (name == name_server_port)
	return "name_server_port";
    else if (name == environment_port)
	return "environment_port";
    else if (name == service_port)
	return "service_port";
    else
	return NULL;
}

static char *
port_type_string(type)
    mach_port_type_t type;
{
    static char buffer[100];
    char *rights;

    switch (type & MACH_PORT_TYPE_ALL_RIGHTS)
    {
      case MACH_PORT_TYPE_NONE:
	rights = "no rights";
	break;
      case MACH_PORT_TYPE_SEND:
	rights = "send rights";
	break;
      case MACH_PORT_TYPE_RECEIVE:
	rights = "receive rights";
	break;
      case MACH_PORT_TYPE_SEND_RECEIVE:
	rights = "send/receive rights";
	break;
      case MACH_PORT_TYPE_SEND_ONCE:
	rights = "send-once rights";
	break;
      case MACH_PORT_TYPE_DEAD_NAME:
	rights = "dead name";
	break;
      case MACH_PORT_TYPE_PORT_SET:
	rights = "port set";
	break;
      default:
	rights = "unknown rights";
	break;
    }

    sprintf(buffer, "%s%s%s%s", rights,
	   (type & MACH_PORT_TYPE_DNREQUEST) ? ",dnrequest" : "",
	   (type & MACH_PORT_TYPE_MAREQUEST) ? ",marequest" : "",
	   (type & MACH_PORT_TYPE_COMPAT) ? ",compat" : "");
    return buffer;
}

static void
print_status_info(task, name, type)
    mtask_t task;
    mach_port_t name;
    mach_port_type_t type;
{
    switch (type & MACH_PORT_TYPE_ALL_RIGHTS) {
      default:
      case MACH_PORT_TYPE_SEND_ONCE:
	printf("%4x %s", name, port_type_string(type));
	break;

      case MACH_PORT_TYPE_SEND:
      case MACH_PORT_TYPE_DEAD_NAME: {
	mach_port_urefs_t refs;
	kern_return_t kr;

	kr = machid_port_get_refs(server, auth, task, name,
				  (type & MACH_PORT_TYPE_SEND ?
				   MACH_PORT_RIGHT_SEND :
				   MACH_PORT_RIGHT_DEAD_NAME),
				  &refs);
	if (kr != KERN_SUCCESS)
	    quit(1, "pinfo: machid_port_get_refs(%x): %s\n",
		 name, mach_error_string(kr));

	printf("%4x %s [refs=%u]", name, port_type_string(type), refs);
	break;
    }

      case MACH_PORT_TYPE_RECEIVE:
      case MACH_PORT_TYPE_SEND_RECEIVE: {
	mach_port_status_t status;
	mach_port_urefs_t refs;
	mach_port_rights_t srights;
	unsigned int dntotal, dnused;
	kern_return_t kr;

	kr = machid_port_get_refs(server, auth, task, name,
				  MACH_PORT_RIGHT_SEND, &refs);
	if (kr != KERN_SUCCESS)
	    quit(1, "pinfo: machid_port_get_refs(%x): %s\n",
		 name, mach_error_string(kr));

	kr = machid_port_get_receive_status(server, auth, task, name, &status);
	if (kr != KERN_SUCCESS)
	    quit(1, "pinfo: machid_port_get_receive_status(%x): %s\n",
		 name, mach_error_string(kr));

	printf("%4x %s [refs=%u]", name, port_type_string(type), refs);

	printf("[seqno=%u", status.mps_seqno);

	if (status.mps_pset != MACH_PORT_NULL)
	    printf(", pset=%x", status.mps_pset);

	if (status.mps_mscount != 0)
	    printf(", mscount=%u", status.mps_mscount);

	if (status.mps_qlimit != MACH_PORT_QLIMIT_DEFAULT)
	    printf(", qlimit=%u", status.mps_qlimit);

	if (status.mps_msgcount != 0)
	    printf(", msgs=%u", status.mps_msgcount);

	if (status.mps_sorights != 0)
	    printf(", sorights=%u", status.mps_sorights);

	if (status.mps_srights) {
	    kr = machid_port_get_srights(server, auth, task, name, &srights);
	    if (kr == KERN_SUCCESS)
		printf(", srights=%u", srights);
	    else
		printf(", srights");
	}

	if (status.mps_pdrequest)
	    printf(", pdrequest");

	if (status.mps_nsrequest)
	    printf(", nsrequest");

	kr = machid_port_dnrequest_info(server, auth, task, name,
					&dntotal, &dnused);
	if ((kr == KERN_SUCCESS) && (dntotal > 0))
	    printf(", dnrequest=%u/%u", dnused, dntotal);

	printf("]");
	break;
    }

      case MACH_PORT_TYPE_PORT_SET: {
	mach_port_t *members;
	unsigned int membersCnt;
	int i;
	kern_return_t kr;

	kr = machid_port_get_set_status(server, auth, task, name,
					&members, &membersCnt);
	if (kr != KERN_SUCCESS)
	    quit(1, "pinfo: machid_port_get_set_status(%x): %s\n",
		 name, mach_error_string(kr));

	printf("%4x %s [members:", name, port_type_string(type));
	for (i = 0; i < membersCnt; i++)
	    printf(" %x", members[i]);
	printf("]");

	kr = vm_deallocate(mach_task_self(),
			   (vm_offset_t) members,
			   (vm_size_t) (membersCnt * sizeof(mach_port_t)));
	if (kr != KERN_SUCCESS)
	    quit(1, "pinfo: vm_deallocate: %s\n",
		 mach_error_string(kr));

	break;
    }
    }
}

static void
print_info(task, name, type)
    mtask_t task;
    mach_port_t name;
    mach_port_type_t type;
{
    if (Verbose)
	print_status_info(task, name, type);
    else
	printf("%4x %s", name, port_type_string(type));
    if (CheckSpecialName) {
	char *special = port_special_string(name);

	if (special != NULL)
	    printf(" (%s)", special);
    }
    printf("\n");
}

unsigned int
find_index(size, tree, name)
    unsigned int size;
    ipc_info_tree_name_t *tree;
    mach_port_t name;
{
    unsigned int lbound = 0;
    unsigned int ubound = size - 1;
    unsigned int middle;

    for (;;) {
	mach_port_t mname;

	/* lbound <= desired index <= ubound */

	middle = lbound + (ubound - lbound) / 2;
	mname = tree[middle].iitn_name.iin_name;

	if (name == mname)
	    break;
	else if (name < mname)
	    ubound = middle - 1;
	else
	    lbound = middle + 1;
    }

    return middle;
}

static void
find_parents(size, tree, parents)
    unsigned int size;
    ipc_info_tree_name_t *tree;
    unsigned int *parents;
{
    unsigned int i;

    for (i = 0; i < size; i++) {
	mach_port_t name, lchild, rchild;

	name = tree[i].iitn_name.iin_name;
	lchild = tree[i].iitn_lchild;
	rchild = tree[i].iitn_rchild;

	if (lchild != MACH_PORT_NULL)
	    parents[find_index(size, tree, lchild)] = i + 1;
	if (rchild != MACH_PORT_NULL)
	    parents[find_index(size, tree, rchild)] = i + 1;
    }
}

static void
find_depths(size, parents, depths)
    unsigned int size;
    unsigned int *parents;
    unsigned int *depths;
{
    unsigned int i;

    for (i = 0; i < size; i++) {
	unsigned int depth;
	unsigned int pindex;

	for (depth = 0, pindex = i + 1;
	     pindex != 0;
	     depth++, pindex = parents[pindex - 1])
	    continue;

	depths[i] = depth;
    }
}

static unsigned int
find_mindepth(size, tree, depths)
    unsigned int size;
    ipc_info_tree_name_t *tree;
    unsigned int *depths;
{
    unsigned int mindepth = ~0;
    unsigned int i;

    for (i = 0; i < size; i++)
	if ((tree[i].iitn_lchild == MACH_PORT_NULL) &&
	    (tree[i].iitn_rchild == MACH_PORT_NULL) &&
	    (depths[i] < mindepth))
	    mindepth = depths[i];

    return mindepth;
}

static unsigned int
find_maxdepth(size, tree, depths)
    unsigned int size;
    ipc_info_tree_name_t *tree;
    unsigned int *depths;
{
    unsigned int maxdepth = 0;
    unsigned int i;

    for (i = 0; i < size; i++)
	if (depths[i] > maxdepth)
	    maxdepth = depths[i];

    return maxdepth;
}

static unsigned int
find_totaldepth(size, tree, depths)
    unsigned int size;
    ipc_info_tree_name_t *tree;
    unsigned int *depths;
{
    unsigned int totaldepth = 0;
    unsigned int i;

    for (i = 0; i < size; i++)
	totaldepth += depths[i];

    return totaldepth;
}

static unsigned int
find_numdepth(size, tree, depths, depth)
    unsigned int size;
    ipc_info_tree_name_t *tree;
    unsigned int *depths, depth;
{
    unsigned int total = 0;
    unsigned int i;

    for (i = 0; i < size; i++)
	if (depths[i] == depth)
	    total++;

    return total;
}

static void
print_name(name, shift)
    mach_port_t name;
    unsigned int shift;
{
    if (name == MACH_PORT_NULL)
	printf("         ");
    else
	printf("%6x.%02x", name >> shift, (name << 32-shift) >> 32-shift);
}

static void
print_name_info(iin, shift)
    ipc_info_name_t *iin;
    unsigned int shift;
{
    print_name(iin->iin_name, shift);
    printf(" ");

    if (iin->iin_compat)
	printf("c");
    else
	printf(" ");
    if (iin->iin_collision)
	printf("x");
    else
	printf(" ");
    if (iin->iin_marequest)
	printf("b");
    else
	printf(" ");
    printf(" ");

    switch(iin->iin_type)
    {
      case MACH_PORT_TYPE_NONE:
	printf("free    ");
	break;
      case MACH_PORT_TYPE_SEND:
	printf("send    ");
	break;
      case MACH_PORT_TYPE_DEAD_NAME:
	printf("dead    ");
	break;
      case MACH_PORT_TYPE_PORT_SET:
	printf("pset    ");
	break;
      case MACH_PORT_TYPE_SEND_ONCE:
	printf("sonce   ");
	break;
      case MACH_PORT_TYPE_RECEIVE:
	printf("receive ");
	break;
      case MACH_PORT_TYPE_SEND_RECEIVE:
	printf("send/rcv");
	break;
      default:
	printf("unknown ");
	break;
    }

    if (iin->iin_object == 0)
	printf("         ");
    else
	printf(" %08x", iin->iin_object);

    /* for next free column */

    if (iin->iin_type == MACH_PORT_TYPE_NONE)
	printf(" %4x", iin->iin_next);
    else
	printf("     ");

    /* for dead-name request column */

    if ((iin->iin_type != MACH_PORT_TYPE_NONE) &&
	(iin->iin_next != 0))
	printf(" %5x", iin->iin_next);
    else
	printf("      ");
}

static void
print_space_info(task)
    mtask_t task;
{
    ipc_info_space_t info;
    ipc_info_name_t *table;
    unsigned int tableCnt;
    ipc_info_tree_name_t *tree;
    unsigned int treeCnt;
    unsigned int *parents;
    unsigned int *depths;
    unsigned int i, shift;
    unsigned int used, hused;
    unsigned int leaves;
    double avebranching, logsize1, avedepth;
    unsigned int maxdepth, mindepth;
    kern_return_t kr;

    kr = machid_port_space_info(server, auth, task, &info,
				&table, &tableCnt, &tree, &treeCnt);
    if (kr != KERN_SUCCESS)
	quit(1, "pinfo: machid_port_space_info: %s\n",
	     mach_error_string(kr));

    if (info.iis_table_size != tableCnt)
	quit(1, "pinfo: machid_port_space_info: iis_table_size != tableCnt\n");

    if (info.iis_tree_size != treeCnt)
	quit(1, "pinfo: machid_port_space_info: iis_tree_size != treeCnt\n");

    shift = 0;
    while ((info.iis_genno_mask >> shift) != 0)
	shift++;

    for (used = 0, i = 0; i < info.iis_table_size; i++)
	if (table[i].iin_type != MACH_PORT_TYPE_NONE)
	    used++;

    for (hused = 0, i = 0; i < info.iis_table_size; i++)
	if (table[i].iin_hash != 0)
	    hused++;

    printf("Period:         %u (%u bits)\n", info.iis_genno_mask + 1, shift);
    printf("Table size:     %u\n", info.iis_table_size);
    printf("Table used:     %u (%d%%)\n",
	   used, 100*used/info.iis_table_size);
    printf("Hash used:      %u (%d%%)\n",
	   hused, 100*hused/info.iis_table_size);
    printf("Next size:      %u\n", info.iis_table_next);
    printf("Tree size:      %u\n", info.iis_tree_size);

    if (info.iis_tree_size > 0) {
	parents = (unsigned int *)
			malloc(info.iis_tree_size * sizeof *parents);
	if (parents == 0)
	    quit(1, "pinfo: malloc(parents): failed\n");
	bzero((char *) parents, info.iis_tree_size * sizeof *parents);

	depths = (unsigned int *) malloc(info.iis_tree_size * sizeof *depths);
	if (depths == 0)
	    quit(1, "pinfo: malloc(depths): failed\n");
	bzero((char *) depths, info.iis_tree_size * sizeof *depths);

	/*
	 *	The parents array contains the indices (in tree)
	 *	of the parent node, plus one, for each node.
	 *	This lets the root node be marked with index 0.
	 */

	find_parents(info.iis_tree_size, tree, parents);

	/*
	 *	The depths array contains the depth of each node.
	 *	The root node has depth one.
	 */

	find_depths(info.iis_tree_size, parents, depths);

	mindepth = find_mindepth(info.iis_tree_size, tree, depths);
	maxdepth = find_maxdepth(info.iis_tree_size, tree, depths);
	avedepth = (find_totaldepth(info.iis_tree_size, tree, depths) /
		    (double) info.iis_tree_size);

	for (i = 0, leaves = 0; i < info.iis_tree_size; i++)
	    if ((tree[i].iitn_lchild == MACH_PORT_NULL) &&
		(tree[i].iitn_rchild == MACH_PORT_NULL))
		leaves++;

	if (info.iis_tree_size == leaves)
	    avebranching = 2.0;
	else
	    avebranching = ((info.iis_tree_size - 1) /
			    (double) (info.iis_tree_size - leaves));

	logsize1 = log((double) (info.iis_tree_size + 1)) / log(2.0);

	printf("Small entries:  %u\n", info.iis_tree_small);
	printf("Hashed entries: %u\n", info.iis_tree_hash);
	printf("Leaf entries:   %u (%3.2f)\n", leaves, avebranching);
	printf("Average depth:  %3.2f (%3.2f)\n", avedepth, logsize1);
	printf("Max/min depth:  %u/%u\n", maxdepth, mindepth);
    }

    if (Verbose) {
	printf("\n");
	printf("  name        type       object next dnreq index\n");
	for (i = 0; i < info.iis_table_size; i++) {
	    ipc_info_name_t *iin = &table[i];

	    print_name_info(iin, shift);

	    if (iin->iin_hash == 0)
		printf("      ");
	    else
		printf(" %5x", iin->iin_hash);
	    printf("\n");
	}

	if (info.iis_tree_size > 0) {
	    printf("\n");
	    printf("  name        type       object next dnreq    lchild    rchild    parent depth\n");
	    for (i = 0; i < info.iis_tree_size; i++) {
		ipc_info_tree_name_t *iitn = &tree[i];

		print_name_info(&iitn->iitn_name, shift);

		printf(" ");
		print_name(iitn->iitn_lchild, shift);
		printf(" ");
		print_name(iitn->iitn_rchild, shift);
		printf(" ");
		print_name((parents[i] == 0 ? MACH_PORT_NULL :
			    tree[parents[i]-1].iitn_name.iin_name),
			   shift);

		printf(" %5u\n", depths[i]);
	    }

	    printf("\n");
	    for (i = 1; i <= maxdepth; i++)
		printf("At depth %3d:   %u\n",
		       i, find_numdepth(info.iis_tree_size, tree, depths, i));
	}
    }

    kr = vm_deallocate(mach_task_self(),
		       (vm_offset_t) table,
		       (vm_size_t) (tableCnt * sizeof *table));
    if (kr != KERN_SUCCESS)
	quit(1, "pinfo: vm_deallocate: %s\n", mach_error_string(kr));

    kr = vm_deallocate(mach_task_self(),
		       (vm_offset_t) tree,
		       (vm_size_t) (treeCnt * sizeof *tree));
    if (kr != KERN_SUCCESS)
	quit(1, "pinfo: vm_deallocate: %s\n", mach_error_string(kr));
}

static void
print_task_info(task)
    mtask_t task;
{
    mach_port_array_t names;
    mach_port_type_array_t types;
    unsigned int namesCnt, typesCnt, i;
    kern_return_t kr;

    kr = machid_port_names(server, auth, task,
			   &names, &namesCnt, &types, &typesCnt);
    if (kr != KERN_SUCCESS)
	quit(1, "pinfo: machid_port_names: %s\n", mach_error_string(kr));
    if (namesCnt != typesCnt)
	quit(1, "pinfo: machid_port_names: namesCnt != typesCnt\n");

    for (i = 0; i < namesCnt; i++)
	print_info(task, names[i], types[i]);

    kr = vm_deallocate(mach_task_self(),
		       (vm_offset_t) names,
		       (vm_size_t) (namesCnt * sizeof(mach_port_t)));
    if (kr != KERN_SUCCESS)
	quit(1, "pinfo: vm_deallocate: %s\n", mach_error_string(kr));

    kr = vm_deallocate(mach_task_self(),
		       (vm_offset_t) types,
		       (vm_size_t) (typesCnt * sizeof(mach_port_type_t)));
    if (kr != KERN_SUCCESS)
	quit(1, "pinfo: vm_deallocate: %s\n", mach_error_string(kr));
}

static void
print_task_name_info(task, name)
    mtask_t task;
    mach_port_t name;
{
    mach_port_type_t type;
    kern_return_t kr;

    kr = machid_port_type(server, auth, task, name, &type);
    if (kr != KERN_SUCCESS)
	quit(1, "pinfo: machid_port_type: %s\n", mach_error_string(kr));

    print_info(task, name, type);
}
