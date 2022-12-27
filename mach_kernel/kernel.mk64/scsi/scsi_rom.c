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
 * $Log:	scsi_rom.c,v $
 * Revision 2.7  91/06/19  11:58:05  rvb
 * 	File moved here from mips/PMAX since it is now "MI" code, also
 * 	used by Vax3100 and soon -- the omron luna88k.
 * 	[91/06/04            rvb]
 * 
 * Revision 2.6  91/05/14  17:31:08  mrt
 * 	Correcting copyright
 * 
 * Revision 2.5  91/05/13  06:05:46  af
 * 	Removed unused code, added sccdrom_name().
 * 	[91/05/12  16:18:49  af]
 * 
 * Revision 2.4  91/02/05  17:46:14  mrt
 * 	Added author notices
 * 	[91/02/04  11:20:04  mrt]
 * 
 * 	Changed to use new Mach copyright
 * 	[91/02/02  12:18:53  mrt]
 * 
 * Revision 2.3  90/12/05  23:35:39  af
 * 
 * 
 * Revision 2.1.1.1  90/11/01  03:40:19  af
 * 	Created, from the SCSI specs:
 * 	"Small Computer Systems Interface (SCSI)", ANSI Draft
 * 	X3T9.2/82-2 - Rev 17B December 1985
 * 	"Small Computer System Interface - 2 (SCSI-II)", ANSI Draft
 * 	X3T9.2/86-109 -  Rev 10C March 1990
 * 	[90/10/11            af]
 */
/*
 *	File: scsi_rom.c
 * 	Author: Alessandro Forin, Carnegie Mellon University
 *	Date:	10/90
 *
 *	Middle layer of the SCSI driver: SCSI protocol implementation
 *
 * This file contains code for SCSI commands for CD-ROM devices.
 */

#include <mach/std_types.h>

char *sccdrom_name(internal)
	boolean_t	internal;
{
	return internal ? "rz" : "CD-ROM";
}

#ifdef	SCSI2
scsi_pause
scsi_play
scsi_play_long
scsi_play_msf
scsi_play_track_index
scsi_play_track_relative
scsi_play_track_relative_long
scsi_read_header
scsi_read_subchannel
scsi_read_toc
#endif	SCSI2
