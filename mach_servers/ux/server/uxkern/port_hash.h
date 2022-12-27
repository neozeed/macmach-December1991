/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	port_hash.h,v $
 * Revision 2.2  90/06/02  15:28:09  rpd
 * 	Converted to new IPC.
 * 	[90/03/26  20:21:33  rpd]
 * 
 */

/*
 * Port-to-pointer hash routines.
 */
#include <sys/queue.h>
#include <uxkern/import_mach.h>

struct port_hash_entry {
	queue_chain_t	chain;
	mach_port_t	port;
	char *		value;
};
typedef struct port_hash_entry *port_hash_entry_t;

struct port_hash_table {
	int		length;
	queue_head_t	head[1];	/* variable size */
};
typedef struct port_hash_table *port_hash_table_t;

#define	PORT_HASH(port)	(port)

port_hash_table_t	port_hash_init();
boolean_t		port_hash_enter();
char *			port_hash_lookup();
boolean_t		port_hash_remove();
