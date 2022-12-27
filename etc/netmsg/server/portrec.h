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
 * $Log:	portrec.h,v $
 * Revision 2.1  90/10/27  20:45:55  dpj
 * Moving under MACH3 source control
 * 
 * Revision 1.8  89/05/02  11:16:29  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * 27-Jul-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Modified to use lock_queue macros.
 *
 *  5-Feb-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Added external definition of pr_np_puid_lookup.
 *	Removed external definition of pr_create.
 *
 *  5-Nov-86  Robert Sansom (rds) at Carnegie-Mellon University
 *	Started.
 *
 */
/*
 * portrec.h
 *
 *
 * $Header: /afs/cs.cmu.edu/project/mach-2/rcs/pkg/netmsg/server/portrec.h,v 2.1 90/10/27 20:45:55 dpj Exp $
 *
 */

/*
 * External definitions for the Port Records module.
 */


#ifndef	_PORTREC_
#define	_PORT_REC_

#include <mach/boolean.h>

#include "lock_queue.h"
#include "lock_queue_macros.h"
#include "port_defs.h"

/*
 * The null network port.
 */
extern network_port_t		null_network_port;


/*
 * Functions exported.
 */

extern boolean_t pr_init();
/*
*/

extern void pr_destroy();
/*
port_rec_ptr_t		port_rec_ptr;
*/

extern port_rec_ptr_t pr_np_puid_lookup();
/*
np_uid_t		np_puid;
*/

extern port_rec_ptr_t pr_nportlookup();
/*
network_port_ptr_t	nport_ptr;
*/

extern port_rec_ptr_t pr_ntran();
/*
network_port_ptr_t	nport_ptr;
*/

extern port_rec_ptr_t pr_lportlookup();
/*
port_t			lport;
*/

extern port_rec_ptr_t pr_ltran();
/*
port_t			lport;
*/

extern boolean_t pr_nport_equal();
/*
network_port_ptr_t	nport_ptr_1, nport_ptr_2;
*/

extern void pr_nporttostring();
/*
char			*nport_str;
network_port_ptr_t	nport_ptr;
*/

extern lock_queue_t pr_list();
/*
*/


#endif	_PORT_REC_
