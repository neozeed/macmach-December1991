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
 * $Log:	srr.h,v $
 * Revision 2.1  90/10/27  20:46:25  dpj
 * Moving under MACH3 source control
 * 
 * Revision 1.7  89/05/02  11:17:43  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * 23-Apr-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Removed retry characteristic constants - they are now in the param record.
 *
 *  4-Dec-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Added srr_max_data_size.  Removed srr_abort.
 *
 *  5-Nov-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Started.
 *
 */
/*
 * srr.h
 *
 *
 * $Header: /afs/cs.cmu.edu/project/mach-2/rcs/pkg/netmsg/server/srr.h,v 2.1 90/10/27 20:46:25 dpj Exp $
 *
 */

/*
 * Definitions for the Simple request-response transport protocol.
 */


#ifndef	_SRR_
#define	_SRR_

#include <mach/boolean.h>

#include "transport.h"

/*
 * srr specific failure codes.
 */
#define SRR_SUCCESS		(0)
#define SRR_ERROR_BASE		(-(TR_SRR_ENTRY * 16))
#define SRR_TOO_LARGE		(1 + SRR_ERROR_BASE)
#define SRR_FAILURE		(2 + SRR_ERROR_BASE)
#define SRR_ENCRYPT_FAILURE	(3 + SRR_ERROR_BASE)


/*
 * The maximum amount of data that can be placed in a request or a response.
 */
extern int srr_max_data_size;

/*
 * Exported functions.
 */
extern boolean_t srr_init();
extern int srr_send();

#endif	_SRR_
