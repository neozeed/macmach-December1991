/* 
 * Mach Operating System
 * Copyright (c) 1991 Carnegie Mellon University
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
 * $Log:	netmemory.c,v $
 * Revision 2.3  91/07/06  16:56:47  jsb
 * 	Some compilers don't understand void.
 * 
 * Revision 2.2  91/07/06  14:55:35  jsb
 * 	First checkin.
 * 
 */
/*
 *	File:	netmemory.c
 *	Author:	Joseph S. Barrera III
 *	Date:	1991
 *
 *	Server which provides shared memory objects using xmm library routines.
 */

#include <mach.h>
#include <stdio.h>
#include <cthreads.h>
#include <mach/message.h>
#include <mach/mach_param.h>
#include <mig_errors.h>
#include <servers/netname.h>
#include <sys/file.h>

#ifndef	__STDC__
#define	void	int
#endif	__STDC__

typedef void *xmm_obj_t;

kern_return_t	xmm_svm_create();
kern_return_t	xmm_debug_create();
kern_return_t	xmm_flush_create();

boolean_t	verbose = FALSE;
boolean_t	debug = FALSE;
boolean_t	debug_more = FALSE;
boolean_t	needs_flush = FALSE;
boolean_t	always_write = FALSE;
boolean_t	interpose = FALSE;

void *		paging_buffer;
void *		paging_partition;

usage()
{
	fprintf(stderr, "usage: netmemoryserver [-v] [-d] [-D] [-pf file]\n");
	fprintf(stderr, "\t-v: verbose startup\n");
	fprintf(stderr, "\t-d: debug output\n");
	fprintf(stderr, "\t-D: more debug output\n");
	fprintf(stderr, "\t-pf <file>: specify a paging file\n");
	exit(1);
}

main(argc, argv)
	int argc;
	char **argv;
{
	kern_return_t kr;
	char *paging_file_name = (char *) 0;
	int i;

	for (i = 1; i < argc; i++) {
		if (! strcmp(argv[i], "-d")) {
			debug = TRUE;
			verbose = TRUE;
		} else if (! strcmp(argv[i], "-D")) {
			debug = TRUE;
			verbose = TRUE;
			debug_more = TRUE;
		} else if (! strcmp(argv[i], "-v")) {
			verbose = TRUE;
		} else if (! strcmp(argv[i], "-pf")) {
			if (i == argc - 1) {
				fprintf(stderr, "-pf requires file arg\n");
				exit(1);
			}
			paging_file_name = argv[++i];
		} else if (! strcmp(argv[i], "-needs_flush")) {
			needs_flush = TRUE;
		} else if (! strcmp(argv[i], "-always_write")) {
			needs_flush = FALSE;
		} else if (! strcmp(argv[i], "-q")) {
			extern int xmm_debug_allowed;
			xmm_debug_allowed = FALSE;
		} else if (! strcmp(argv[i], "-interpose")) {
			interpose = TRUE;
		} else {
			usage();
		}
	}
	open_paging_file(paging_file_name);
	netmemory_init();
	xmm_user_init();
	kr = netmemory_service();
	mach_error("netmemory_service", kr);
	exit(1);
}

/*
 * Create paging_partition and paging_buffer,
 * both used for all netmemory objects.
 */
open_paging_file(paging_file_name)
	char *paging_file_name;
{
	int fd;
	kern_return_t kr;
	xmm_obj_t mobj;

	/*
	 * Get a filename and open it
	 */
	if (paging_file_name == (char *) 0) {
		paging_file_name = "/usr/tmp/nm_paging_file";
	}
	fd = open(paging_file_name, O_CREAT | O_TRUNC | O_RDWR, 0600);
	if (fd < 0) {
		unlink(paging_file_name);
		fd = open(paging_file_name, O_CREAT | O_TRUNC | O_RDWR, 0600);
	}
	if (fd < 0) {
		perror(paging_file_name);
		exit(1);
	}

	/*
	 * Create a pagingfile object for the file
	 */
	kr = xmm_pagingfile_create(fd, &mobj);
	if (kr != KERN_SUCCESS) {
		return kr;
	}
	if (debug_more) {
		kr = xmm_debug_create(mobj, &mobj);
		if (kr != KERN_SUCCESS) {
			return kr;
		}
	}

	/*
	 * Create a paging partition, using the pagingfile object
	 */
	kr = xmm_partition_allocate(mobj, 128*1024*1024, &paging_partition);
	if (kr != KERN_SUCCESS) {
		return kr;
	}

	/*
	 * And create a buffer for data_writes of data in transit
	 * through the svm system
	 */
	kr = xmm_buffer_allocate(1, &paging_buffer);
	if (kr != KERN_SUCCESS) {
		return kr;
	}
}

kern_return_t
netmemory_create(server, size, netmemory_object, netmemory_control)
	port_t server;
	vm_size_t size;
	port_t *netmemory_object;
	port_t *netmemory_control;
{
	int fd;
	kern_return_t kr;
	xmm_obj_t mobj;

	/*
	 * Create a private paging object for this netmemory object,
	 * allocated from the shared paging partition
	 */
	xmm_multiplex_create(paging_partition, &mobj);
	if (debug_more) {
		kr = xmm_debug_create(mobj, &mobj);
		if (kr != KERN_SUCCESS) {
			return kr;
		}
	}

	/*
	 * Create a buffer for data_writes underneath svm layer
	 */
	kr = xmm_buffer_create(mobj, paging_buffer, &mobj);
	if (kr != KERN_SUCCESS) {
		return kr;
	}
	if (debug_more) {
		kr = xmm_debug_create(mobj, &mobj);
		if (kr != KERN_SUCCESS) {
			return kr;
		}
	}

	/*
	 * Create the svm object. This is the object that handles
	 * interkernel object consistency.
	 */
	kr = xmm_svm_create(mobj, &mobj);
	if (kr != KERN_SUCCESS) {
		return kr;
	}
	if (debug) {
		kr = xmm_debug_create(mobj, &mobj);
		if (kr != KERN_SUCCESS) {
			return kr;
		}
	}

	/*
	 * Export the object so that other netmemory servers can find it
	 */
	kr = xmm_net_export(mobj, netmemory_object);
	if (kr != KERN_SUCCESS) {
		return kr;
	}

	*netmemory_control = *netmemory_object;
	return KERN_SUCCESS;
}

kern_return_t
netmemory_destroy(netmemory_control)
	port_t netmemory_control;
{
	/* XXX gc */
	return KERN_SUCCESS;
}

kern_return_t
netmemory_cache(server, netmemory_object, memory_object)
	port_t server;
	port_t netmemory_object;
	port_t *memory_object;
{
	kern_return_t kr;
	xmm_obj_t mobj;

	/*
	 * Find the object corresponding to the netmemory port.
	 * This may be a local object, or a proxy for a remote object.
	 */
	kr = xmm_net_import(netmemory_object, &mobj);
	if (kr != KERN_SUCCESS) {
		return kr;
	}

	/*
	 * Just a testing hack
	 */
	if (needs_flush) {
		if (debug_more) {
			kr = xmm_debug_create(mobj, &mobj);
			if (kr != KERN_SUCCESS) {
				return kr;
			}
		}
		kr = xmm_flush_create(mobj, &mobj);
		if (kr != KERN_SUCCESS) {
			return kr;
		}
	}

	/*
	 * Just a testing hack
	 */
	if (always_write) {
		if (debug_more) {
			kr = xmm_debug_create(mobj, &mobj);
			if (kr != KERN_SUCCESS) {
				return kr;
			}
		}
		kr = xmm_write_create(mobj, &mobj);
		if (kr != KERN_SUCCESS) {
			return kr;
		}
	}

	/*
	 * Finish off with the layer that translates kernel
	 * memory_object messages into object messages.
	 */
	if (debug_more) {
		kr = xmm_debug_create(mobj, &mobj);
		if (kr != KERN_SUCCESS) {
			return kr;
		}
	}
	kr = xmm_memory_object_create(mobj, memory_object);
	if (kr != KERN_SUCCESS) {
		return kr;
	}

	return KERN_SUCCESS;
}

char netmemory_server_name[80] = "netmemoryserver";
port_t netmemory_server_port;

kern_return_t
netmemory_init()
{
	kern_return_t kr;
	char *getenv();

	/*
	 * Use a nonstandard service name for debugging if one is supplied
	 */
	if (getenv("NETMEMORYSERVER_NAME")) {
		strcpy(netmemory_server_name, getenv("NETMEMORYSERVER_NAME"));
	}

	/*
	 * Find old netmemoryserver, if any, and remove it
	 */
	kr = netname_look_up(name_server_port, "", netmemory_server_name,
			     &netmemory_server_port);
	if (kr == NETNAME_SUCCESS) {
		kr = netname_check_out(name_server_port, netmemory_server_name,
				       netmemory_server_port);
		printf("Netmemoryserver: removed old netmemoryserver\n");
	}

	/*
	 * Create a port for incoming netmemory requests.
	 */
	kr = mach_port_allocate(mach_task_self(), MACH_PORT_RIGHT_RECEIVE,
				&netmemory_server_port);
	if (kr != KERN_SUCCESS) {
		return kr;
	}
	kr = mach_port_insert_right(mach_task_self(), netmemory_server_port,
				    netmemory_server_port,
				    MACH_MSG_TYPE_MAKE_SEND);
	if (kr != KERN_SUCCESS) {
		return kr;
	}

	/*
	 * Check in our service port
	 * XXX should use better signature
	 */
	kr = netname_check_in(name_server_port, netmemory_server_name,
			      netmemory_server_port, netmemory_server_port);
	if (kr != NETNAME_SUCCESS) {
		return kr;
	}
	return KERN_SUCCESS;
}

typedef struct max_msg {
	msg_header_t header;
	char data[MSG_SIZE_MAX - sizeof(msg_header_t)];
} *max_msg_t;

kern_return_t
netmemory_service()
{
	struct max_msg in_msg, out_msg;
	kern_return_t kr;
	
	for (;;) {
		in_msg.header.msg_local_port = netmemory_server_port;
		in_msg.header.msg_size = sizeof(in_msg);
		kr = mach_msg_receive(&in_msg.header, MSG_OPTION_NONE, 0);
		if (kr != RCV_SUCCESS) {
			return kr;
		}

		if (! netmemory_server(&in_msg.header, &out_msg.header)) {
			out_msg.header.msg_size = sizeof(out_msg.header);
			out_msg.header.msg_id = 0;
			out_msg.header.msg_local_port = MACH_PORT_NULL;
			kr = mach_msg_send(&out_msg.header, MSG_OPTION_NONE,0);
			printf("netmemoryserver mig loop: invalid msg id %d\n",
			       in_msg.header.msg_id);
			mach_error("death pill", kr);
			continue;
		}
		if (out_msg.header.msg_remote_port != MACH_PORT_NULL) {
			kr = mach_msg_send(&out_msg.header, MSG_OPTION_NONE,
					   0);
			if (kr != SEND_SUCCESS) {
				mach_error("mach_msg_send", kr);
				printf("netmemoryserver mig loop: msg_id %d\n",
				       in_msg.header.msg_id);
			}
		}
	}
}

