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
 * $Log:	DeskBus.h,v $
 * Revision 2.2  91/09/12  16:49:51  bohman
 * 	Created.
 * 	[91/09/11  16:25:31  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2os/DeskBus.h
 */

/************************************************************

Created: Thursday, September 7, 1989 at 3:33 PM
	DeskBus.h
	C Interface to the Macintosh Libraries


	Copyright Apple Computer, Inc.	1987 -1989
	All rights reserved

************************************************************/


#ifndef __DESKBUS__
#define __DESKBUS__

typedef struct ADBOpBlock {
    Ptr 	dataBuffPtr;		/*address of data buffer*/
    ProcPtr	opServiceRtPtr;	 	/*service routine pointer*/
    Ptr 	opDataAreaPtr;		/*optional data area address*/
} ADBOpBlock, *ADBOpBPtr;

typedef struct ADBDataBlock {
    char	devType;			/*device type*/
    char	origADBAddr;		/*original ADB Address*/
    Ptr		dbServiceRtPtr;	 	/*service routine pointer*/
    Ptr		dbDataAreaAddr; 		/*data area address*/
} ADBDataBlock, *ADBDBlkPtr;

typedef struct ADBSetInfoBlock {
    ProcPtr	siServiceRtPtr;	 	/*service routine pointer*/
    Ptr		siDataAreaAddr; 		/*data area address*/
} ADBSetInfoBlock, *ADBSInfoPtr;

#endif
