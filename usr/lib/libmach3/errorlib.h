
/* 
 * Mach Operating System
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * File:	errrolib.h
 * Purpose:
 *	Generic error code interface
 *
 * HISTORY:
 * $Log:	errorlib.h,v $
 * Revision 2.2  90/08/13  15:44:25  jjc
 * 	Picked up from Mark Stevenson for including in error_codes.c.
 * 	[90/08/07            jjc]
 * 
 * Revision 2.2  90/07/26  12:44:36  dpj
 * 	First version.
 * 	[90/07/24  16:40:33  dpj]
 * 
 * Revision 1.4  90/03/21  17:22:02  jms
 * 	Comment Changes
 * 	[90/03/16  16:52:20  jms]
 * 
 * 	[89/12/19  16:16:56  jms]
 * 
 *	new taskmaster changes
 *
 * Revision 1.3  90/01/02  22:04:20  dorr
 * 	no change.
 * 
 * Revision 1.2.1.1  89/12/19  17:06:35  dorr
 * 	16-May-88	Mary R. Thompson (mrt) at Carnegie Mellon
 *	Corrected the definitions of IPC_RCV_MOD and IPC_SEND_MOD
 *
 * 09-Mar-88	Douglas Orr (dorr) at Carnegie-Mellon University
 *	created.
 */

#include <mach/error.h>

#define	IPC_SEND_MOD		(err_ipc|err_sub(0))
#define	IPC_RCV_MOD		(err_ipc|err_sub(1))
#define	IPC_MIG_MOD		(err_ipc|err_sub(2))

#define	SERV_NETNAME_MOD	(err_server|err_sub(0))
#define	SERV_ENV_MOD		(err_server|err_sub(1))
#define	SERV_EXECD_MOD		(err_server|err_sub(2))


#define	NO_SUCH_ERROR		"unknown error code"

struct error_subsystem {
	char			* subsys_name;
	int			max_code;
	char			* * codes;
};

struct error_system {
	int			max_sub;
	char			* bad_sub;
	struct error_subsystem	* subsystem;
};

extern	struct error_system 	errors[err_max_system+1];

#define	errlib_count(s)		(sizeof(s)/sizeof(s[0]))



