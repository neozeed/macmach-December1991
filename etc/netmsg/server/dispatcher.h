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
 * $Log:	dispatcher.h,v $
 * Revision 2.1  90/10/27  20:44:05  dpj
 * Moving under MACH3 source control
 * 
 * Revision 1.8  89/05/02  11:08:50  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 *  3-Sep-86  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Modified for RPCMOD.
 *
 * 14-Dec-86  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Revised the declaration for disp_inmsg for new set of
 *	parameters. Added a declaration for disp_inprobe.
 *	Added disp_inprobe to dispatcher_switch_t.
 *
 *  3-Dec-86  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Changed DISP_MAX_TYPE into DISP_TYPE_MAX.
 *
 * 26-Nov-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Started.
 *
 */
/*
 * dispatcher.h
 *
 *
 * $Header: /afs/cs.cmu.edu/project/mach-2/rcs/pkg/netmsg/server/dispatcher.h,v 2.1 90/10/27 20:44:05 dpj Exp $
 *
 */

/*
 * Functions exported by the dispatcher module.
 */


#ifndef	_DISPATCHER_
#define	_DISPATCHER_

#include "disp_hdr.h"

/*
 * Dispatcher switch.
 */
typedef struct {
    int		(*disp_indata)();
    int		(*disp_inprobe)();
    int		(*disp_indata_simple)();
    int		(*disp_rr_simple)();
#if	RPCMOD
    int		(*disp_in_request)();
#endif	RPCMOD
} dispatcher_switch_t;

extern dispatcher_switch_t	dispatcher_switch[DISP_TYPE_MAX];


/*
 * Functions exported by the dispatcher module.
 */

extern boolean_t disp_init();
/*
*/

extern int disp_indata();
/*
int		trid;
sbuf_ptr_t	data;
netaddr_t	from;
int		(*tr_cleanup)();
int		trmod;
int		client_id;
int		crypt_level;
boolean_t	broadcast;
*/

extern int disp_inprobe();
/*
int		trid;
sbuf_ptr_t	pkt;
netaddr_t	from;
int		(*(*cancel))();
int		trmod;
int		*client_id;
int		crypt_level;
boolean_t	broadcast;
*/

extern int disp_indata_simple();
/*
int		client_id;
sbuf_ptr_t	data;
netaddr_t	from;
boolean_t	broadcast;
int		crypt_level;
*/

extern int disp_rr_simple();
/*
sbuf_ptr_t	data;
netaddr_t	from;
boolean_t	broadcast;
int		crypt_level;
*/

#if	RPCMOD
extern int disp_in_request();
/*
int		trmod;
int		trid;
sbuf_ptr_t	data_ptr;
netaddr_t	from;
int		crypt_level;
boolean_t	broadcast;
*/
#endif	RPCMOD

#endif	_DISPATCHER_
