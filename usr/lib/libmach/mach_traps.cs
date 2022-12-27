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
 * $Log:	mach_traps.cs,v $
 * Revision 2.2  91/03/27  16:03:43  mrt
 * 	First checkin
 * 
 */
/*	
 *	File:	mach_traps.cs
 *	Author: Michael Young, Carnegie Mellon University
 *	Date:	Aug. 1989
 *
 *	This file exports the Mach kernel trap interface.
 *	The file mach/syscall_sw.h comes from the kernel
 *	sources and defines the trap numbers and includes
 *	mach/machine/syscall_sw.h which defines the trap
 *	code.
 */
#include <mach/syscall_sw.h>
