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
 * $Log:	vmstat.c,v $
 * Revision 2.2  91/08/29  16:08:26  rpd
 * 	Created.
 * 	[91/08/19            rpd]
 * 
 */
/*
 * vmstat is derived from
 *	File:	vm_stat.c
 *	Author:	Avadis Tevanian, Jr.
 *
 *	Copyright (C) 1986, Avadis Tevanian, Jr.
 *
 *	Display Mach VM statistics.
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

static mtask_t task = 0;
static mdefault_pager_t default_pager = 0;

static void snapshot();
static void print_stats();

static void usage()
{
    quit(1, "usage: vmstat [-host machine] [-task id] [-dpager id] [repeat-interval]\n");
}

main(argc, argv)
    int	argc;
    char *argv[];
{
    char *hostname = "";
    mhost_t host;
    mhost_priv_t phost;
    int delay;
    int i;
    kern_return_t kr;

    for (i = 1; i < argc; i++)
	if (streql(argv[i], "-host") && (i < argc-1))
	    hostname = argv[++i];
	else if (streql(argv[i], "-task") && (i < argc-1))
	    task = atoi(argv[++i]);
	else if (streql(argv[i], "-dpager") && (i < argc-1))
	    default_pager = atoi(argv[++i]);
	else if (streql(argv[i], "--")) {
	    i++;
	    break;
	} else if (argv[i][0] == '-')
	    usage();
	else
	    break;

    argv += i;
    argc -= i;

    switch (argc) {
      case 0:
	delay = 0;
	break;

      case 1:
	delay = atoi(argv[0]);
	if (delay <= 0)
	    usage();
	break;

      default:
	usage();
    }

    kr = netname_look_up(name_server_port, hostname, "MachID", &server);
    if (kr != KERN_SUCCESS)
	quit(1, "vmstat: netname_lookup_up(MachID): %s\n",
	     mach_error_string(kr));

    auth = mach_host_priv_self();
    if (auth == MACH_PORT_NULL)
	auth = mach_task_self();

    /*
     *	We need a task and a default pager.
     *	We pick the first task on the host, which in practice
     *	will be the kernel task.  In any case, we just need
     *	a long-lived task.
     */

    kr = machid_host_ports(server, auth, &host, &phost);
    if (kr != KERN_SUCCESS)
	quit(1, "vmstat: machid_host_ports: %s\n",
	     mach_error_string(kr));

    if (default_pager == 0) {
	kr = machid_host_default_pager(server, auth, phost, &default_pager);
	if (kr != KERN_SUCCESS)
	    quit(1, "vmstat: machid_host_default_pager: %s\n",
		 mach_error_string(kr));
    }

    if (task == 0) {
	mtask_t *tasks;
	unsigned int tasksCnt;

	kr = machid_host_tasks(server, auth, phost, &tasks, &tasksCnt);
	if (kr != KERN_SUCCESS)
	    quit(1, "vmstat: machid_host_tasks: %s\n",
		 mach_error_string(kr));

	if (tasksCnt == 0)
	    quit(1, "vmstat: no tasks on host\n");
	task = tasks[0];

	kr = vm_deallocate(mach_task_self(), (vm_offset_t) tasks,
			   (vm_size_t) (tasksCnt * sizeof *tasks));
	if (kr != KERN_SUCCESS)
	    quit(1, "vmstat: vm_deallocate: %s\n",
		 mach_error_string(kr));
    }

    if (delay == 0)
	snapshot();
    else for (;;) {
	print_stats();
	sleep(delay);
    }

    exit(0);
}

static void get_stats(statsp, percentp, totalp, freep)
    struct vm_statistics *statsp;
    int *percentp;
    vm_size_t *totalp;
    vm_size_t *freep;
{
    kern_return_t kr;

    kr = machid_vm_statistics(server, auth, task, statsp);
    if (kr != KERN_SUCCESS)
	quit(1, "vmstat: machid_vm_statistics: %s\n",
	     mach_error_string(kr));

    if (statsp->lookups == 0)
	*percentp = 0;
    else
	*percentp = (100 * statsp->hits) / statsp->lookups;

    kr = machid_default_pager_info(server, auth, default_pager,
				   totalp, freep);
    if (kr != KERN_SUCCESS)
	quit(1, "vmstat: machid_default_pager_info: %s\n",
	     mach_error_string(kr));
}

static void
digits(buffer, number)
    char *buffer;
    double number;
{
    if (number < 10.0)
	sprintf(buffer, "%4.2f", number);
    else if (number < 100.0)
	sprintf(buffer, "%4.1f", number);
    else
	sprintf(buffer, "%4.0f", number);
}

static void
mem_to_string(buffer, size)
    char *buffer;
    vm_size_t size;
{
    char digitsbuf[100];

    if (size > 1024*1024*1024) {
	digits(digitsbuf, size/(1024.0*1024.0*1024.0));
	sprintf(buffer, "%sG", digitsbuf);
    } else if (size > 1024*1024) {
	digits(digitsbuf, size/(1024.0*1024.0));
	sprintf(buffer, "%sM", digitsbuf);
    } else
	sprintf(buffer, "%dK", size/1024);
}

static void pstat(str, n)
	char	*str;
	int	n;
{
	printf("%-25s %10d.\n", str, n);
}

static void snapshot()
{
    struct vm_statistics stats;
    int percent;
    vm_size_t total, free;
    char free_buffer[100], total_buffer[100];

    get_stats(&stats, &percent, &total, &free);
    mem_to_string(free_buffer, free);
    mem_to_string(total_buffer, total);

    printf("Mach Virtual Memory Statistics: (page size of %d bytes)\n",
	   stats.pagesize);
    pstat("Pages free:", stats.free_count);
    pstat("Pages active:", stats.active_count);
    pstat("Pages inactive:", stats.inactive_count);
    pstat("Pages wired down:", stats.wire_count);
    pstat("\"Translation faults\":", stats.faults);
    pstat("Pages copy-on-write:", stats.cow_faults);
    pstat("Pages zero filled:", stats.zero_fill_count);
    pstat("Pages reactivated:", stats.reactivations);
    pstat("Pageins:", stats.pageins);
    pstat("Pageouts:", stats.pageouts);
    printf("Object cache: %d hits of %d lookups (%d%% hit rate)\n",
	   stats.hits, stats.lookups, percent);
    printf("Default pager backing store: %s free of %s total space\n",
	   free_buffer, total_buffer);
}

static struct vm_statistics last_stats;

static void banner()
{
    struct vm_statistics stats;
    int percent;
    vm_size_t total, free;
    char total_buffer[100];

    get_stats(&stats, &percent, &total, &free);
    mem_to_string(total_buffer, total);

    printf("Mach VM Stats: ");
    printf("(page size %d bytes, cache hits %d%%, backing store %s)\n",
	   stats.pagesize, percent, total_buffer);
    printf("  free active inac wire  faults  copied  zeroed reactive pagein pageout   space\n");

    bzero(&last_stats, sizeof last_stats);
}

static void print_stats()
{
    static count = 0;

    struct vm_statistics stats;
    int percent;
    vm_size_t total, free;
    char free_buffer[100];

    if (count++ == 0)
	banner();

    if (count > 20)
	count = 0;

    get_stats(&stats, &percent, &total, &free);
    mem_to_string(free_buffer, free);

    printf("%6d %6d %4d %4d %7d %7d %7d %7d %7d %7d %7s\n",
	   stats.free_count,
	   stats.active_count,
	   stats.inactive_count,
	   stats.wire_count,
	   stats.faults - last_stats.faults,
	   stats.cow_faults - last_stats.cow_faults,
	   stats.zero_fill_count - last_stats.zero_fill_count,
	   stats.reactivations - last_stats.reactivations,
	   stats.pageins - last_stats.pageins,
	   stats.pageouts - last_stats.pageouts,
	   free_buffer);
    last_stats = stats;
}
