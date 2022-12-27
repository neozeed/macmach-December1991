/* 
 * Mach Operating System
 * Copyright (c) 1991 Carnegie Mellon University
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 * 
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS "AS IS"
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
 * any improvements or extensions that they make and grant Carnegie Mellon
 * the rights to redistribute these changes.
 */
/*
 * HISTORY
 * $Log:	ipc_output.c,v $
 * Revision 2.4  91/08/03  18:19:30  jsb
 * 	Merged into norma/ipc_net.c. At some point I will move all output
 * 	related functions from norma/ipc_net.c back into here.
 * 	[91/08/01  22:04:59  jsb]
 * 
 * 	Cleaned up and corrected handling of out-of-line data.
 * 	Eliminated remaining old-style page list code.
 * 	[91/07/27  18:51:30  jsb]
 * 
 * 	Moved MACH_MSGH_BITS_COMPLEX_{PORTS,DATA} to mach/message.h.
 * 	[91/07/04  13:11:39  jsb]
 * 
 * 	Confined complex_data_hint garbage to just this file.
 * 	[91/07/04  10:22:22  jsb]
 * 
 * Revision 2.3  91/07/01  08:25:39  jsb
 * 	Changes for new vm_map_copy_t definition.
 * 	Locking changes in ipc_clport_send.
 * 	[91/06/29  15:06:45  jsb]
 * 
 * Revision 2.2  91/06/17  15:47:48  jsb
 * 	Moved here from ipc/ipc_cloutput.c.
 * 	[91/06/17  11:06:38  jsb]
 * 
 * Revision 2.2  91/06/06  17:05:27  jsb
 * 	Fixed copyright.
 * 	[91/05/24  13:19:37  jsb]
 * 
 * 	First checkin.
 * 	[91/05/14  13:30:09  jsb]
 * 
 */
/*
 *	File:	norma/ipc_output.c
 *	Author:	Joseph S. Barrera III
 *	Date:	1990
 *
 *	Functions to support ipc between nodes in a single Mach cluster.
 */

/*
 *	For now, see norma/ipc_net.c.
 */
