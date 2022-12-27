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
 * $Log:	sm_init_defs.h,v $
 * Revision 2.1  90/10/27  20:46:18  dpj
 * Moving under MACH3 source control
 * 
 * Revision 1.4  89/05/02  11:17:31  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 *  7-Jul-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Started.
 *
 */
/*
 * sm_init_defs.h
 *
 *
 * $Header: /afs/cs.cmu.edu/project/mach-2/rcs/pkg/netmsg/server/sm_init_defs.h,v 2.1 90/10/27 20:46:18 dpj Exp $
 *
 */

/*
 * Definitions for secure Mach initialisation.
 */


#ifndef	_SM_INIT_DEFS_
#define	_SM_INIT_DEFS_

#include <mach/mach_param.h>

#define NAME_SERVER_INDEX	0
#define AUTH_SERVER_INDEX	1
#define SM_INIT_INDEX_MAX	1

#undef NAME_SERVER_SLOT
#undef ENVIRONMENT_SLOT
#undef SERVICE_SLOT

#define OLD_NAME_SERVER_SLOT	0
#define NETNAME_SLOT		0
#define SM_INIT_SLOT		1
#define AUTH_PRIVATE_SLOT	1
#define OLD_ENV_SERVER_SLOT	1
#define AUTH_SERVER_SLOT	2
#define OLD_SERVICE_SLOT	2
#define KM_SERVICE_SLOT		2
#define NAME_SERVER_SLOT	3
#define NAME_PRIVATE_SLOT	3
#define SM_INIT_SLOTS_USED	4

#if (SM_INIT_SLOTS_USED > TASK_PORT_REGISTER_MAX)
Things are not going to work!
#endif


#define SM_INIT_FAILURE		11881

#endif	_SM_INIT_DEFS_
