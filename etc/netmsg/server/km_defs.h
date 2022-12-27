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
 * $Log:	km_defs.h,v $
 * Revision 2.1  90/10/27  20:44:28  dpj
 * Moving under MACH3 source control
 * 
 * Revision 1.6  89/05/02  11:10:52  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 *  6-Jul-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Added a timer to a key request record in order to repeat key
 *	exchange requests if we get no response to them.
 *
 *  5-Jun-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Store host byte order key in key record for multperm encryption algorithm.
 *
 *  9-Jan-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Started.
 *
 */
/*
 * km_defs.h
 *
 *
 * $Header: /afs/cs.cmu.edu/project/mach-2/rcs/pkg/netmsg/server/km_defs.h,v 2.1 90/10/27 20:44:28 dpj Exp $
 *
 */

/*
 * Internal definitions for the Key Management module.
 */


#ifndef	_KM_DEFS_
#define	_KM_DEFS_

#include "key_defs.h"
#include "nm_defs.h"
#include "timer.h"

typedef struct {
    netaddr_t	kr_host_id;
    key_t	kr_key;
    key_t	kr_mpkey;
    key_t	kr_mpikey;
} key_rec_t, *key_rec_ptr_t;

#define KEY_REC_NULL	((key_rec_ptr_t)0)

/*
 * Definition of queue holding pending key exchange requests.
 */
typedef struct kmq_entry {
    struct kmq_entry	*next;
    struct timer	kmq_timer;
    netaddr_t		kmq_host_id;
    int			kmq_client_id;
    int			(*kmq_client_retry)();
} kmq_entry_t, *kmq_entry_ptr_t;


#define KM_RETRY_INTERVAL	60


/*
 * External definitions for functions implemented by km_utils.c
 */

extern key_rec_ptr_t km_host_enter();
/*
netaddr_t	host_id;
*/

extern key_rec_ptr_t km_host_lookup();
/*
netaddr_t	host_id;
*/

extern void km_utils_init();
/*
*/

extern void km_procs_init();
/*
*/

#endif	_KM_DEFS_
