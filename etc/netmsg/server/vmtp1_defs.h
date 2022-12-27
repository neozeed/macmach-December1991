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
 * $Log:	vmtp1_defs.h,v $
 * Revision 2.1  90/10/27  20:47:05  dpj
 * Moving under MACH3 source control
 * 
 * Revision 1.6  89/05/02  11:19:28  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * 23-Jun-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Added support for encrypted data.
 *
 *  8-Jun-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Created.
 */
/*
 * vmtp1_defs.h
 *
 *
 * $Header: /afs/cs.cmu.edu/project/mach-2/rcs/pkg/netmsg/server/vmtp1_defs.h,v 2.1 90/10/27 20:47:05 dpj Exp $
 *
 */

/*
 * Definitions for the VMTP module.
 */


#ifndef	_VMTP1_DEFS_
#define	_VMTP1_DEFS_

#define KERNEL_FEATURES	1

#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<netinet/vmtp_so.h>
#include	<netinet/vmtp.h>

#include	"crypt.h"
#include	"sbuf.h"


/*
 * Maximum VMTP segment size, preallocated in the transport records.
 */
#define	VMTP_SEGSIZE	16384
#define VMTP_MAX_DATA	(VMTP_SEGSIZE - sizeof(crypt_header_t) - sizeof(long))

typedef struct vmtp_segment {
	crypt_header_t	crypt_header;
	char		data[VMTP_MAX_DATA];
	long		pad;
} vmtp_segment_t, *vmtp_segment_ptr_t;

/*
 * VMTP transport record.
 */
typedef struct vmtp_rec {
	vmtp_segment_t	segment;	/* maximum-size VMTP segment */
	struct vmtp_rec	*next;		/* next in free queue */
	int		socket;		/* associated socket (if any) */
	struct vmtpmcb	mcb;		/* VMTP message control block */
	sbuf_t		sbuf;		/* sbuf for data */
	sbuf_seg_t	sbuf_seg;	/* segment for sbuf */
	int		client_id;	/* higher-level (ipc) ID */
	int		(*callback)();	/* client callback procedure */
} vmtp_rec_t, *vmtp_rec_ptr_t;


#endif	_VMTP1_DEFS_
