/* 
 * Mach Operating System
 * Copyright (c) 1991,1990 Carnegie Mellon University
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
 * $Log:	scsi_optical.c,v $
 * Revision 2.6  91/06/19  11:57:57  rvb
 * 	File moved here from mips/PMAX since it is now "MI" code, also
 * 	used by Vax3100 and soon -- the omron luna88k.
 * 	[91/06/04            rvb]
 * 
 * Revision 2.5  91/05/14  17:30:53  mrt
 * 	Correcting copyright
 * 
 * Revision 2.4  91/02/05  17:46:03  mrt
 * 	Added author notices
 * 	[91/02/04  11:19:52  mrt]
 * 
 * 	Changed to use new Mach copyright
 * 	[91/02/02  12:18:38  mrt]
 * 
 * Revision 2.3  90/12/05  23:35:26  af
 * 
 * 
 * Revision 2.1.1.1  90/11/01  03:40:10  af
 * 	Created, from the SCSI specs:
 * 	"Small Computer Systems Interface (SCSI)", ANSI Draft
 * 	X3T9.2/82-2 - Rev 17B December 1985
 * 	"Small Computer System Interface - 2 (SCSI-II)", ANSI Draft
 * 	X3T9.2/86-109 -  Rev 10C March 1990
 * 	[90/10/11            af]
 */
/*
 *	File: scsi_optical.c
 * 	Author: Alessandro Forin, Carnegie Mellon University
 *	Date:	10/90
 *
 *	Middle layer of the SCSI driver: SCSI protocol implementation
 *
 * This file contains code for SCSI commands for OPTICAL MEMORY devices.
 */

#ifdef	SCSI2

#include <mach/std_types.h>

char *scmem_name(internal)
	boolean_t	internal;
{
	return internal ? "mz" : "opti";
}

scmem_optimize()
{}

#if 0
scsi_erase_long
scsi_medium_scan
scsi_read_defect_data
scsi_read_generation
scsi_read_updated_block
scsi_update_block
scsi_write_verify_long
#endif

#endif	SCSI2
