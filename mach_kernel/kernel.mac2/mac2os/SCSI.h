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
 * $Log:	SCSI.h,v $
 * Revision 2.2  91/09/12  16:51:08  bohman
 * 	Created.
 * 	[91/09/11  16:32:34  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2os/SCSI.h
 */

/************************************************************

Created: Thursday, September 7, 1989 at 7:18 PM
	SCSI.h
	C Interface to the Macintosh Libraries


	Copyright Apple Computer, Inc.	1986-1989
	All rights reserved

************************************************************/


#ifndef __SCSI__
#define __SCSI__

/*
 * Result codes
 */
#define scCommErr	2
#define scBadParamsErr	4
#define scPhaseErr	5
#define scCompareErr	6

/*
 * SCSI Manager transfer opcodes
 */
#define scInc		1
#define scNoInc		2
#define scAdd		3
#define scMove		4
#define scLoop		5
#define scNOp		6
#define scStop		7
#define scComp		8

typedef struct {
    unsigned short	scOpcode;
    unsigned long	scParam1;
    unsigned long	scParam2;
} SCSITransferInstr;

/*
 * SCSI drive partitioning
 */
#define sbSIGWord	0x4552
#define pMapSIG		0x504D

typedef struct Block0 {
    unsigned short	sbSig;		/*unique value for SCSI block 0*/
    unsigned short	sbBlkSize;	/*block size of device*/
    unsigned long	sbBlkCount;	/*number of blocks on device*/
    unsigned short	sbDevType;	/*device type*/
    unsigned short	sbDevId; 	/*device id*/
    unsigned long	sbData;		/*not used*/
    unsigned short	sbDrvrCount; 	/*driver descriptor count*/
    unsigned long	ddBlock;	/*1st driver's starting block*/
    unsigned short	ddSize;		/*size of 1st driver (512-byte blks)*/
    unsigned short	ddType;		/*system type (1 for Mac+)*/
} Block0;

typedef struct Partition {
    unsigned short	pmSig;		/*unique value for map entry blk*/
    unsigned short	pmSigPad;	/*currently unused*/
    unsigned long	pmMapBlkCnt;	/*# of blks in partition map*/
    unsigned long	pmPyPartStart;	/*physical start blk of partition*/
    unsigned long	pmPartBlkCnt; 	/*# of blks in this partition*/
    unsigned char	pmPartName[32];	/*ASCII partition name*/
    unsigned char	pmPartType[32];	/*ASCII partition type*/
    unsigned long	pmLgDataStart;	/*log. # of partition's 1st data blk*/
    unsigned long	pmDataCnt;	/*# of blks in partition's data area*/
    unsigned long	pmPartStatus; 	/*bit field for partition status*/
    unsigned long	pmLgBootStart;	/*log. blk of partition's boot code*/
    unsigned long	pmBootSize;	/*number of bytes in boot code*/
    unsigned long	pmBootAddr;	/*memory load address of boot code*/
    unsigned long	pmBootAddr2;	/*currently unused*/
    unsigned long	pmBootEntry;	/*entry point of boot code*/
    unsigned long	pmBootEntry2; 	/*currently unused*/
    unsigned long	pmBootCksum;	/*checksum of boot code*/
    unsigned char	pmProcessor[16];/*ASCII for the processor type*/
} Partition;

#endif
