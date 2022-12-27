/* 
 * Mach Operating System
 * Copyright (c) 1991,1990,1989 Carnegie Mellon University
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 * 
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS 
 * CONDITION.  CARNEGIE MELLON DISCLAIMS ANY LIABILITY OF ANY KIND FOR
 * ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
 * 
 * Carnegie Mellon requests users of this software to return to
 * 
 *  Software Distribution Coordinator  or  Software.Distribution@CS.CMU.EDU
 *  School of Computer Science
 *  Carnegie Mellon University
 *  Pittsburgh PA 15213-3890
 * 
 * any improvements or extensions that they make and grant Carnegie the
 * rights to redistribute these changes.
 */
/*
 * HISTORY
 * $Log:	mach_privileged_ports.h,v $
 * Revision 2.2  91/03/27  15:39:36  mrt
 * 	First checkin
 * 
 * Revision 2.3  90/10/29  18:12:11  dpj
 * 	Added set_mach_privileged_host_port(), set_mach_device_server_port().
 * 	[90/08/15  15:01:01  dpj]
 * 
 * Revision 2.2  90/07/26  12:44:43  dpj
 * 	First version.
 * 	[90/07/24  16:53:16  dpj]
 * 
 */
/*
 *	File:	mach_privileged_ports.h
 *	Author:	Dan Julin, Carnegie Mellon University
 *	Date:	July 1990
 *
 * 	Privileged ports exported by the Mach kernel.
 */


#ifndef	_MACH_PRIVILEGED_PORTS_
#define	_MACH_PRIVILEGED_PORTS_

#include	<mach.h>
#include	<mach/kern_return.h>

extern mach_port_t	mach_privileged_host_port();
extern mach_port_t	mach_device_server_port();

extern void		set_mach_privileged_host_port();
extern void		set_mach_device_server_port();


#endif	_MACH_PRIVILEGED_PORTS_
