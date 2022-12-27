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
 * $Log:	Slots.h,v $
 * Revision 2.2  91/09/12  16:51:20  bohman
 * 	Created Mach 3.0 version from 2.5 version.
 * 	[91/09/11  16:33:48  bohman]
 * 
 * Revision 2.2  90/08/30  11:45:45  bohman
 * 	Created.
 * 	[90/08/29  13:00:18  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2os/Slots.h
 */

/*
	Slots.h -- new Operating System Interfaces

	C Interface to the Macintosh Libraries
	Copyright Apple Computer,Inc. 1986-1987
	All rights reserved.
*/


#ifndef __NOSINTF__
#define __NOSINTF__

				/* StatusFlags constants */
/* Card is Changed field in StatusFlags field of sInfoArray */
#define fCardIsChanged	1

/* For SearchSRT. Flag to check for SAME sResource in the table.  */
#define fCkForSame	0

/* For SearchSRT. Flag to check for NEXT sResource in the table.  */
#define fCkForNext	1

/* If this bit is set then warm start, else cold start. */
#define fWarmStart	2

/* For ReadPBSize. If set, check for 0 in reserved field of block size */
#define fCkReserved	2

/* For CalcStep. If set calculate step for consecutive bytes, else ID's */
#define fConsecBytes	32

				/* State constants */ 
#define stateNil	0		/* Nil */
#define stateSDMInit	1		/* Slot declaration manager Init */
#define statePRAMInit	2		/* sPRAM record init */
#define statePInit	3		/* Primary init */
#define stateSInit	4		/* Secondary init */

				/* Device Manager Slot Support */
typedef struct SlotIntQElement {
    Ptr			sqLink;		/* ptr to next element */
    unsigned short	sqType;		/* queue type ID for validity */
    short		sqPrio;		/* priority */
    ProcPtr		sqAddr;		/* interrupt service routine */
    unsigned long	sqParm;		/* optional A1 parameter */
} SlotIntQElement, *SQElemPtr;
				  
				/* Slot Declaration Manager */
typedef struct SpBlock {
    unsigned long	spResult;	/* FUNCTION Result */
    Ptr			spsPointer;	/* Structure pointer */
    unsigned long	spSize;		/* Size of structure */
    unsigned long	spOffsetData;	/* Offset/Data field */
    Ptr			spIOFileName;	/* Pointer to IOFile name */
    Ptr			spsExecPBlk;	/* Pointer to sExec parameter block */
    Ptr			spStackPtr;	/* Old Stack pointer */
    unsigned long	spMisc;		/* Misc field for SDM */
    unsigned long	spReserved;	/* Reserved for future expansion */
    unsigned short	spIOReserved;	/* Reserved field of Slot Resource Table */
    short		spRefNum;	/* RefNum */
    unsigned short	spCategory;	/* sType: Category	 */
    unsigned short	spCType;	/* 	  Type		 */
    unsigned short	spDrvrSW; 	/*	  DrvrSW	 */
    unsigned short	spDrvrHW;	/*	  DrvrHW	 */
    unsigned char	spTBMask;	/* Type bit mask
					 * (Bits 0..3 determine
					 * which words 0..3 to mask) */
    unsigned char	spSlot;		/* Slot number */
    unsigned char	spID;		/* Structure ID */
    unsigned char	spExtDev;	/* Id of the external device. */
    unsigned char	spHwDev;	/* Id of the hardware device. */
    unsigned char	spByteLanes;	/* Bytelanes value from FHeader */
    unsigned char	spFlags;	/* Flags passed to routines */
    unsigned char	spKey;		/* Internal use only */
} SpBlock, *SpBlockPtr;

typedef struct SInfoRecord {
    Ptr			siDirPtr;	/* Pointer to directory */
    short		siInitStatusA;	/* initialization error */
    short		siInitStatusV;	/* status returned by prim init code */
    unsigned char	siState;	/* initialization state */
    unsigned char	siCPUByteLanes;	/* 0=[d0..d7], 1=[d8..d15], ... */
    unsigned char	siTopOfROM;	/* Top of ROM = $FssFFFFx,
					 * where x is TopOfROM */
    unsigned char	siStatusFlags;	/* bit 0 - card is changed */
    unsigned short	siTOConst;	/* Time Out Constant for BusErr */
    unsigned char	siReserved[2];	/* reserved */
} SInfoRecord, *SInfoRecPtr;

typedef struct SPRAMRecord {
    unsigned short	boardID;
    unsigned char	vendorUse1;
    unsigned char	vendorUse2;
    unsigned char	vendorUse3;
    unsigned char	vendorUse4;
    unsigned char	vendorUse5;
    unsigned char	vendorUse6;
} SPRAMRecord, *SPRAMRecPtr;

typedef struct FHeaderRec {
    unsigned long	fhDirOffset;		/* offset to directory */
    unsigned long	fhLength;		/* length of ROM */
    unsigned long	fhCRC;			/* CRC */
    unsigned char	fhROMRev;		/* revision of ROM */
    unsigned char	fhFormat;		/* format - 2 */
    unsigned long	fhTstPat;		/* test pattern */
    unsigned char	fhReserved;		/* reserved */
    unsigned char	fhByteLanes;		/* ByteLanes */
} FHeaderRec, *FHeaderRecPtr;

typedef struct SEBlock {
    unsigned char	seSlot;		/* Slot number */
    unsigned char	sesRsrcId;	/* sResource Id */
    short		seStatus;	/* Status of code executed by sExec */
    unsigned char	seFlags;	/* Flags */
    unsigned char	seFiller0;	/* Filler */
    unsigned char	seFiller1;	/* Filler */
    unsigned char	seFiller2;	/* Filler */

			/*  extensions for sLoad + sBoot  */
    long		seResult;	/* Result of sLoad */
    long		seIOFileName;	/* Pointer to IOFile name */
    unsigned char	seDevice;	/* Which device to read from */
    unsigned char	sePartition;	/* The partition */
    unsigned char	seOSType;	/* Type of OS */
    unsigned char	seReserved;	/* Reserved field */
    unsigned char	seRefNum;	/*  RefNum of the driver */
			/*  extensions for sBoot  */
    unsigned char	seNumDevices;	/* Number of devices to load */
    unsigned char	seBootState;	/* State of StartBoot code */
} SEBlock;

#endif
