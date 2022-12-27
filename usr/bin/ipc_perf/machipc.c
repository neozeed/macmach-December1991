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
 * $Log:	machipc.c,v $
 * Revision 2.5  91/03/27  17:21:50  mrt
 * 	Changed mach.h include
 * 
 * Revision 2.4  91/03/19  11:38:13  mrt
 * 	Changed to new copyright
 * 
 * Revision 2.3  91/03/10  13:40:15  rpd
 * 	Streamlined the simple RPC test to reduce user space overhead.
 * 	[91/03/09  17:39:36  rpd]
 * 
 * Revision 2.2  90/09/12  16:29:36  rpd
 * 	First check-in.
 * 	[90/09/11            rpd]
 * 
 */

#include <mach.h>
#include <mach/msg_type.h>
#include <mach_error.h>
#include <mach/message.h>
#include <servers/netname.h>
#include <stdio.h>
#include <strings.h>
#include <sys/time.h>
#include <sys/resource.h>

#define streql(a, b)	(strcmp((a), (b)) == 0)

static void client_ool(), client_simple();
static void server_ool(), server_simple();
static void null_trap();

struct msg_ool {
	mach_msg_header_t h;
	mach_msg_type_long_t t;
	vm_offset_t d;
};

usage()
{
	fprintf(stderr, "usage: machipc -s [-ool] [-n service-name]\n");
	fprintf(stderr, "\t-n: specify name registered with netname service\n");
	fprintf(stderr, "\t-ool: be prepared for OOL data\n");
	fprintf(stderr, "usage: machipc -c [-i iterations] [-ool size] [-n service-name]\n");
	fprintf(stderr, "\t-n: specify name registered with netname service\n");
	fprintf(stderr, "\t-i: number of RPCs measured\n");
	fprintf(stderr, "\t-ool: size (bytes, in hex) of OOL data in request\n");
	fprintf(stderr, "usage: machipc -null [-i iterations]\n");
	fprintf(stderr, "\t-i: number of null traps measured\n");
	exit(1);
}

main(argc, argv)
	int argc;
	char *argv[];
{
	enum { Nothing, Client, Server, NullTrap } mode = Nothing;
	boolean_t AmClient = FALSE, AmServer = FALSE;
	boolean_t ServerOOL = FALSE;
	int iterations = 1000;
	vm_size_t oolsize = -1;
	char *servicename = "MachIPCPerformance";

	mach_port_t port;
	kern_return_t kr;
	int i;

	for (i = 1; i < argc; i++)
		if ((mode == Nothing) && streql(argv[i], "-c"))
			mode = Client;
		else if ((mode == Nothing) && streql(argv[i], "-s"))
			mode = Server;
		else if ((mode == Nothing) && streql(argv[i], "-null"))
			mode = NullTrap;
		else if (((mode == Client) || (mode == NullTrap)) &&
			 streql(argv[i], "-i") && (i < argc-1))
			iterations = atoi(argv[++i]);
		else if ((mode == Client) &&
			 streql(argv[i], "-ool") && (i < argc-1))
			oolsize = atoh(argv[++i]);
		else if ((mode == Server) && streql(argv[i], "-ool"))
			ServerOOL = TRUE;
		else if (((mode == Client) || (mode == Server)) &&
			 streql(argv[i], "-n") && (i < argc-1))
			servicename = argv[++i];
		else
			usage();

	switch (mode) {
	    case Nothing:
		usage();

	    case Client:
		kr = netname_look_up(name_server_port, "*",
				     servicename, &port);
		if (kr != KERN_SUCCESS)
			quit(1, "machipc: netname_look_up: %s\n",
			     mach_error_string(kr));

		if (oolsize == -1)
			client_simple(port, iterations);
		else
			client_ool(port, iterations, oolsize);
		break;

	    case Server:
		kr = mach_port_allocate(mach_task_self(),
					MACH_PORT_RIGHT_RECEIVE, &port);
		if (kr != KERN_SUCCESS)
			quit(1, "machipc: mach_port_allocate: %s\n",
			     mach_error_string(kr));

		kr = netname_check_in(name_server_port, servicename,
				      mach_task_self(), port);
		if (kr != KERN_SUCCESS)
			quit(1, "machipc: netname_check_in: %s\n",
			     mach_error_string(kr));

		if (ServerOOL)
			server_ool(port);
		else
			server_simple(port);
		break;

	    case NullTrap:
		null_trap(iterations);
		break;
	}

	exit(0);
}

double
periter(before, after, iterations)
	struct timeval *before, *after;
	int iterations;
{
	/* usecs/RPC */

	return (((after->tv_sec - before->tv_sec) * 1000000 +
		 (after->tv_usec - before->tv_usec)) /
		(double) iterations);
}

double
throughput(before, after, iterations, size)
	struct timeval *before, *after;
	int iterations;
	vm_size_t size;
{
	/* bytes/sec */

	return (((double)iterations * size) /
		((after->tv_sec - before->tv_sec) +
		 (after->tv_usec - before->tv_usec) / 1000000.0));
}

static void
client_simple(port, iterations)
	mach_port_t port;
	int iterations;
{
	mach_msg_header_t msg;
	struct rusage rbefore, rafter;
	struct timeval tbefore, tafter;
	mach_port_t reply;
	int i;
	kern_return_t kr;

	reply = mach_reply_port();

	printf("%d RPCs, no OOL data:\n", iterations);

	msg.msgh_kind = MSG_TYPE_RPC;
	msg.msgh_id = 0;
	msg.msgh_local_port = reply;
	msg.msgh_remote_port = port;
	msg.msgh_bits = MACH_MSGH_BITS(MACH_MSG_TYPE_COPY_SEND,
				       MACH_MSG_TYPE_MAKE_SEND_ONCE);

	kr = mach_msg(&msg, MACH_SEND_MSG|MACH_RCV_MSG,
		      sizeof msg, sizeof msg, reply,
		      MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
	if (kr != MACH_MSG_SUCCESS)
		quit(1, "machipc: mach_msg: %s\n", mach_error_string(kr));

	(void) gettimeofday(&tbefore, (struct timezone *) NULL);
	(void) getrusage(RUSAGE_SELF, &rbefore);

	for (i = 0; i < iterations; i++) {
		/*
		 *	The reply message will leave the correct values
		 *	for msgh_kind, msgh_id, msgh_local_port.
		 */

		msg.msgh_remote_port = port;
		msg.msgh_bits = MACH_MSGH_BITS(MACH_MSG_TYPE_COPY_SEND,
					       MACH_MSG_TYPE_MAKE_SEND_ONCE);

		(void) mach_msg_trap(&msg, MACH_SEND_MSG|MACH_RCV_MSG,
				     sizeof msg, sizeof msg, reply,
				     MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
	}

	(void) getrusage(RUSAGE_SELF, &rafter);
	(void) gettimeofday(&tafter, (struct timezone *) NULL);

	printf("Elapsed usecs/RPC: %7.3f\n",
	       periter(&tbefore, &tafter, iterations));
	printf("User    usecs/RPC: %7.3f\n",
	       periter(&rbefore.ru_utime, &rafter.ru_utime, iterations));
	printf("System  usecs/RPC: %7.3f\n",
	       periter(&rbefore.ru_stime, &rafter.ru_stime, iterations));
}

static void
server_simple(port)
	mach_port_t port;
{
	mach_msg_header_t msg;
	mach_port_t pset;
	kern_return_t kr;

	kr = mach_port_allocate(mach_task_self(),
				MACH_PORT_RIGHT_PORT_SET, &pset);
	if (kr != KERN_SUCCESS)
		quit(1, "machipc: mach_port_allocate: %s\n",
		     mach_error_string(kr));

	kr = mach_port_move_member(mach_task_self(), port, pset);
	if (kr != KERN_SUCCESS)
		quit(1, "machipc: mach_port_move_member: %s\n",
		     mach_error_string(kr));

	kr = mach_msg(&msg, MACH_RCV_MSG, 0, sizeof msg, pset,
		      MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
	if (kr != MACH_MSG_SUCCESS)
		quit(1, "machipc: mach_msg: %s\n",
		     mach_error_string(kr));

	for (;;) {
		msg.msgh_bits =
			MACH_MSGH_BITS(MACH_MSG_TYPE_MOVE_SEND_ONCE, 0);
		msg.msgh_local_port = MACH_PORT_NULL;

		(void) mach_msg_trap(&msg, MACH_SEND_MSG|MACH_RCV_MSG,
				     sizeof msg, sizeof msg, pset,
				     MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
	}
}

static void
client_ool(port, iterations, oolsize)
	mach_port_t port;
	int iterations;
	vm_size_t oolsize;
{
	struct msg_ool msg;
	struct rusage rbefore, rafter;
	struct timeval tbefore, tafter;
	mach_port_t reply;
	vm_offset_t data;
	double tput;
	kern_return_t kr;
	int i;

	reply = mach_reply_port();

	msg.t.msgtl_name = MACH_MSG_TYPE_INTEGER_8;
	msg.t.msgtl_size = 8;
	msg.t.msgtl_number = oolsize;
	msg.t.msgtl_header.msgt_name = 0;
	msg.t.msgtl_header.msgt_size = 0;
	msg.t.msgtl_header.msgt_number = 0;
	msg.t.msgtl_header.msgt_inline = FALSE;
	msg.t.msgtl_header.msgt_longform = TRUE;
	msg.t.msgtl_header.msgt_deallocate = FALSE;
	msg.t.msgtl_header.msgt_unused = 0;

	if (oolsize > 0) {
		kr = vm_allocate(mach_task_self(), &data,
				 oolsize, TRUE);
		if (kr != KERN_SUCCESS)
		    quit(1, "client: vm_allocate: %s\n",
			 mach_error_string(kr));

		for (i = 0; i < oolsize/sizeof(int); i++)
		    ((int *) data)[i] = i;
	} else
		data = 0;

	msg.d = data;
	printf("%d RPCs, 0x%x bytes OOL data:\n",
	       iterations, oolsize);

	(void) gettimeofday(&tbefore, (struct timezone *) NULL);
	(void) getrusage(RUSAGE_SELF, &rbefore);

	for (i = 0; i < iterations; i++) {
		msg.h.msgh_kind = MSG_TYPE_RPC;
		msg.h.msgh_id = 0;
		msg.h.msgh_remote_port = port;
		msg.h.msgh_local_port = reply;
		msg.h.msgh_size = sizeof msg;
		msg.h.msgh_bits =
			(MACH_MSGH_BITS(MACH_MSG_TYPE_COPY_SEND,
					MACH_MSG_TYPE_MAKE_SEND_ONCE) |
			 MACH_MSGH_BITS_COMPLEX);

		kr = mach_msg(&msg.h, MACH_SEND_MSG|MACH_RCV_MSG,
			      sizeof msg, sizeof msg.h, reply,
			      MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
		if (kr != MACH_MSG_SUCCESS)
			quit(1, "machipc: mach_msg: %s\n",
			     mach_error_string(kr));
	}

	(void) getrusage(RUSAGE_SELF, &rafter);
	(void) gettimeofday(&tafter, (struct timezone *) NULL);

	printf("Elapsed usecs/RPC: %7.3f\n",
	       periter(&tbefore, &tafter, iterations));
	printf("User    usecs/RPC: %7.3f\n",
	       periter(&rbefore.ru_utime, &rafter.ru_utime, iterations));
	printf("System  usecs/RPC: %7.3f\n",
	       periter(&rbefore.ru_stime, &rafter.ru_stime, iterations));

	tput = throughput(&tbefore, &tafter, iterations, oolsize);

	printf("\n");
	printf("Throughput bytes/sec: %d (%dK)\n",
	       (int) tput, (int) (tput / 1024.0));
}

static void
server_ool(port)
	mach_port_t port;
{
	struct msg_ool msg;
	mach_port_t pset;
	kern_return_t kr;

	kr = mach_port_allocate(mach_task_self(),
				MACH_PORT_RIGHT_PORT_SET, &pset);
	if (kr != KERN_SUCCESS)
		quit(1, "machipc: mach_port_allocate: %s\n",
		     mach_error_string(kr));

	kr = mach_port_move_member(mach_task_self(), port, pset);
	if (kr != KERN_SUCCESS)
		quit(1, "machipc: mach_port_move_member: %s\n",
		     mach_error_string(kr));

	kr = mach_msg(&msg.h, MACH_RCV_MSG, 0, sizeof msg, pset,
		      MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
	if (kr != MACH_MSG_SUCCESS)
		quit(1, "machipc: mach_msg: %s\n",
		     mach_error_string(kr));

	for (;;) {
		if (msg.h.msgh_bits & MACH_MSGH_BITS_COMPLEX) {
			vm_offset_t data = msg.d;
			vm_size_t oolsize = msg.t.msgtl_number;

			if (oolsize > 0) {
				kr = vm_deallocate(mach_task_self(),
						   data, oolsize);
				if (kr != KERN_SUCCESS)
					quit(1, "machipc: vm_deallocate: %s\n",
					     mach_error_string(kr));
			}
		}

		msg.h.msgh_bits =
			MACH_MSGH_BITS(MACH_MSG_TYPE_MOVE_SEND_ONCE, 0);
		msg.h.msgh_local_port = MACH_PORT_NULL;

		kr = mach_msg(&msg.h, MACH_SEND_MSG|MACH_RCV_MSG,
			      sizeof msg.h, sizeof msg, pset,
			      MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
		if (kr != MACH_MSG_SUCCESS)
			quit(1, "machipc: mach_msg: %s\n",
			     mach_error_string(kr));
	}
}

static void
null_trap(iterations)
	int iterations;
{
	struct rusage rbefore, rafter;
	struct timeval tbefore, tafter;
	int i;

	printf("%d iterations:\n", iterations);

	(void) getrusage(RUSAGE_SELF, &rbefore);
	(void) gettimeofday(&tbefore, (struct timezone *) NULL);

	for (i = 0; i < iterations; i++)
		(void) mach_msg_trap((mach_msg_header_t *) 0,
				     MACH_MSG_OPTION_NONE,
				     0, 0, MACH_PORT_NULL,
				     MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);

	(void) getrusage(RUSAGE_SELF, &rafter);
	(void) gettimeofday(&tafter, (struct timezone *) NULL);

	printf("Elapsed usecs/trap: %7.3f\n",
	       periter(&tbefore, &tafter, iterations));
	printf("User    usecs/trap: %7.3f\n",
	       periter(&rbefore.ru_utime, &rafter.ru_utime, iterations));
	printf("System  usecs/trap: %7.3f\n",
	       periter(&rbefore.ru_stime, &rafter.ru_stime, iterations));
}
