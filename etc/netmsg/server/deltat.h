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
 * $Log:	deltat.h,v $
 * Revision 2.1  90/10/27  20:43:57  dpj
 * Moving under MACH3 source control
 * 
 * Revision 1.7  89/05/02  11:07:56  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 *  1-Oct-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Modified for new request-reply transport interface. (RPCMOD)
 *
 *  4-May-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Removed retry characteristic constants - they are now in the param record.
 *
 *  4-Dec-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Removed deltat_abort.
 *
 * 13-Nov-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Started.
 *
 */
/*
 * delta.h
 *
 *
 * $Header: /afs/cs.cmu.edu/project/mach-2/rcs/pkg/netmsg/server/deltat.h,v 2.1 90/10/27 20:43:57 dpj Exp $
 *
 */

/*
 * Public definitions for delta-t transport protocol.
 */


#ifndef	_DELTAT_
#define	_DELTAT_

#include <mach/boolean.h>

#include "transport.h"

/*
 * Delta-t specific failure codes.
 */
#define DELTAT_SUCCESS		TR_SUCCESS
#define DELTAT_ERROR_BASE	(-(TR_DELTAT_ENTRY * 16))
#define DELTAT_FAILURE		(1 + DELTAT_ERROR_BASE)
#define DELTAT_REMOTE_ABORT	(2 + DELTAT_ERROR_BASE)

/*
 * Exported functions.
 */
extern boolean_t deltat_init();
extern int deltat_retry();
#if	RPCMOD
extern int deltat_sendrequest();
#else	RPCMOD
extern int deltat_send();
#endif	RPCMOD

#endif	_DELTAT_
