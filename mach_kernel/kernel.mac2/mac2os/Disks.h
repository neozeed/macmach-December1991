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
 * $Log:	Disks.h,v $
 * Revision 2.2  91/09/12  16:49:59  bohman
 * 	Created.
 * 	[91/09/11  16:26:26  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2os/Disks.h
 */

/************************************************************

Created: Thursday, September 7, 1989 at 3:51 PM
	Disks.h
	C Interface to the Macintosh Libraries


	Copyright Apple Computer, Inc.	 1985-1989
	All rights reserved

************************************************************/


#ifndef __DISKS__
#define __DISKS__

struct DrvSts {
	short track;
	char writeProt;
	char diskInPlace;
	char installed;
	char sides;
	struct QElem *qLink;
	short qType;
	short dQDrive;
	short dQRefNum;
	short dQFSID;
	char twoSideFmt;
	char needsFlush;
	short diskErrs;
};

typedef struct DrvSts DrvSts;
struct DrvSts2 {
	short track;
	char writeProt;
	char diskInPlace;
	char installed;
	char sides;
	struct QElem *qLink;
	short qType;
	short dQDrive;
	short dQRefNum;
	short dQFSID;
	short driveSize;
	short driveS1;
	short driveType;
	short driveManf;
	short driveChar;
	char driveMisc;
};

typedef struct DrvSts2 DrvSts2;

#endif
