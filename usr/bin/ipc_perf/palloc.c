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
 * $Log:	palloc.c,v $
 * Revision 2.4  91/03/27  17:21:59  mrt
 * 	Changed mach.h include
 * 
 * Revision 2.3  91/03/19  11:38:19  mrt
 * 	Changed to new copyright
 * 
 * Revision 2.2  90/09/12  16:29:41  rpd
 * 	First check-in.
 * 	[90/09/11            rpd]
 * 
 */

#include <mach.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>

#define streql(a, b)	(strcmp((a), (b)) == 0)

double
periteration(before, after, iterations)
	struct timeval *before, *after;
	int iterations;
{
	/* usecs/iteration */

	return (((after->tv_sec - before->tv_sec) * 1000000 +
		 (after->tv_usec - before->tv_usec)) /
		(double) iterations);
}

static void
usage()
{
	quit(1, "usage: palloc [-i iterations] [rpc|trap|syscall]\n");
}

main(argc, argv)
	int argc;
	char *argv[];
{
	struct rusage rbefore, rafter;
	struct timeval tbefore, tafter;

	enum { Trap, Syscall, RPC } test;
	int iterations = 1000;

	int i;

	for (i = 1; i < argc; i++)
		if (streql(argv[i], "-i") && (i < argc-1))
			iterations = atoi(argv[++i]);
		else
			break;

	switch (argc - i)
	    case 1:
		if (streql(argv[i], "rpc"))
	    case 0:
			test = RPC;
		else if (streql(argv[i], "trap"))
			test = Trap;
		else if (streql(argv[i], "syscall"))
			test = Syscall;
		else
	    default:
			usage();

	switch (test) {
	    case Trap:
		printf("%d iterations of mach_reply_port/mach_port_destroy:\n",
		       iterations);
		break;

	    case Syscall:
		printf("%d iterations of syscall_mach_port_allocate/mach_port_destroy:\n",
		       iterations);
		break;

	    case RPC:
		printf("%d iterations of mach_port_allocate/mach_port_destroy:\n",
		       iterations);
		break;
	}

	(void) gettimeofday(&tbefore, (struct timezone *) NULL);
	(void) getrusage(RUSAGE_SELF, &rbefore);

	for (i = 0; i < iterations; i++) {
		mach_port_t port;
		kern_return_t kr;

		switch (test) {
		    case Trap:
			port = mach_reply_port();
			if (port == MACH_PORT_NULL)
				quit(1, "palloc: mach_reply_port\n");
			break;

		    case Syscall:
			kr = syscall_mach_port_allocate(mach_task_self(),
					MACH_PORT_RIGHT_RECEIVE, &port);
			if (kr != KERN_SUCCESS)
				quit(1, "palloc: syscall_mach_port_allocate\n");
			break;

		    case RPC:
			kr = mach_port_allocate(mach_task_self(),
					MACH_PORT_RIGHT_RECEIVE, &port);
			if (kr != KERN_SUCCESS)
				quit(1, "palloc: mach_port_allocate\n");
		}

		kr = mach_port_destroy(mach_task_self(), port);
		if (kr != KERN_SUCCESS)
			quit(1, "palloc: mach_port_destroy\n");
	}

	(void) getrusage(RUSAGE_SELF, &rafter);
	(void) gettimeofday(&tafter, (struct timezone *) NULL);

	printf("Elapsed usecs/iteration: %7.3f\n",
	       periteration(&tbefore, &tafter, iterations));
	printf("User    usecs/iteration: %7.3f\n",
	       periteration(&rbefore.ru_utime, &rafter.ru_utime, iterations));
	printf("System  usecs/iteration: %7.3f\n",
	       periteration(&rbefore.ru_stime, &rafter.ru_stime, iterations));

	exit(0);
}
