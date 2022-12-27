/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	tcp_defs.h,v $
 * Revision 2.1  90/10/27  20:46:45  dpj
 * Moving under MACH3 source control
 * 
 * Revision 1.5  89/05/02  11:18:20  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * 26-Mar-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Converted to use the new memory management module.
 *
 *  3-Dec-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Created.
 */
/*
 * tcp_defs.h
 *
 *
 * $Header: /afs/cs.cmu.edu/project/mach-2/rcs/pkg/netmsg/server/tcp_defs.h,v 2.1 90/10/27 20:46:45 dpj Exp $
 *
 */

/*
 * Definitions for the TCP module.
 */


#ifndef	_TCP_DEFS_
#define	_TCP_DEFS_

#include	"mem.h"
#include	"sbuf.h"
#include	"sys_queue.h"

/*
 * Transaction records.
 */
typedef struct tcp_trans {
	int			state;	/* see defines below */
	unsigned long		trid;
	int			client_id;
	sbuf_ptr_t		data;
	int			crypt_level;
	int			(*reply_proc)();
	sys_queue_chain_t	transq;	/* list of pending/waiting transactions */
} tcp_trans_t, *tcp_trans_ptr_t;

#define	TCP_TR_INVALID	0
#define	TCP_TR_PENDING	1	/* awaiting a reply */
#define	TCP_TR_WAITING	2	/* awaiting transmission */


/*
 * Memory management definitions.
 */
extern mem_objrec_t		MEM_TCPTRANS;


#endif	_TCP_DEFS_
