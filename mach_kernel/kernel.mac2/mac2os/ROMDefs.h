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
 * $Log:	ROMDefs.h,v $
 * Revision 2.2  91/09/12  16:50:49  bohman
 * 	Created Mach 3.0 version from 2.5 version.
 * 	[91/09/11  16:30:36  bohman]
 * 
 * Revision 2.2  90/08/30  11:09:26  bohman
 * 	Created.
 * 	[90/08/29  13:00:00  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2os/ROMDefs.h
 */

/*
	ROMDefs.h -- ROM Definition interface 

	C Interface to the Macintosh Libraries
	Copyright Apple Computer,Inc. 1986-1987
	All rights reserved.
*/

#ifndef __ROMDEFS__
#define __ROMDEFS__

				/* Format-Header */
#define appleFormat	0x01		/* Format of Declaration Data (IEEE will assign real value) */
#define romRevision	0x01		/* Revision of Declaration Data Format */
#define testPattern	0x5A932BC7	/* FHeader long word test pattern */

				/* sExec constants */
#define sCodeRev	2			/* Revision of code (For sExec) */
#define sCPU68000	1			/* CPU type = 68000 */
#define sCPU68020	2			/* CPU type = 68020 */

				/* sDRVR directory constants */
#define sMacOS68000	1			/* Mac OS, CPU type = 68000 */
#define sMacOS68020	2			/* Mac OS, CPU type = 68020 */


				/* sResource types */	
	/* Board sResource - Required on all boards */
#define board				0x00000000
	/* Video with Apple parameters for TFB card. */
#define displayVideoAppleTFB		0x01010101
	/* Video with Apple parameters for GM card. */
#define displayVideoAppleGM		0x01010102
		/* Ethernet with apple parameters for 3-Comm card. */
#define networkEtherNetApple3Com	0x02010101
		/* A simple test sResource. */
#define testSimpleAppleAny		0x80010100

	
				/* Misc */
#define endOfList	0xFF		/* End of list */
#define defaultTO       100		/*100 retries */
  
			/* sResource List. Category: All */
		/* The following Id's are common to all sResources. */
#define sRsrcType	1		/* Type of sResource */
#define sRsrcName	2		/* Name of sResource */
#define sRsrcIcon	3		/* Icon */
#define sRsrcDrvrDir	4		/* Driver directory */
#define sRsrcLoadDir	5		/* Load directory */
#define sRsrcBootRec	6		/* sBoot record */
#define sRsrcFlags	7		/* sResource Flags */
#define sRsrcHWDevId	8		/* Hardware Device Id */

#define minorBaseOS	10		/* Offset to base of sResource in minor space. */
#define minorLength	11
#define majorBaseOS	12		/* Offset to base of sResource in Major space. */
#define majorLength	13

#define sDRVRDir	16		/* sDriver directory */

#define drSwApple     	1 
#define drHwTFB       	1
#define drHwGM        	2 
#define drHw3Com      	1 

			/* sResource List. Category: Board */
		/* The following Id's are common to all Board sResources. */
#define catBoard        1
#define catTest         2
#define catDisplay     	3
#define catNetwork     	4
#define boardId		32		/* Board Id */
#define pRAMInitData	33		/* sPRAM init data */
#define primaryInit	34		/* Primary init record */
#define timeOutConst	35		/* Time out constant */
#define vendorInfo	36		/* Vendor information List. See Vendor List, below */
#define boardFlags	37		/* Board Flags */


			/* Vendor List */		
		/* The following Id's are associated with the VendorInfo id */
#define vendorId	1		/* Vendor Id */
#define serialNum	2		/* Serial number */
#define revLevel	3		/* Revision level */
#define partNum		4		/* Part number */


			/* sResource List. Category_Type: Test_One */
		/* The following Id's are common to all Test_One_x sResources. */
#define typeBoard	0 
#define typeApple	1
#define typeVideo	1   
#define typeEtherNet	1  
#define testByte	32		/* Test byte. */
#define testWord	33		/* Test Word. */
#define testLong	34		/* Test Long. */
#define testString	35		/* Test String. */
#endif
