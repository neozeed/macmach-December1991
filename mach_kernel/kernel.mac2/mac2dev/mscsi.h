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
 * $Log:	mscsi.h,v $
 * Revision 2.2  91/09/12  16:47:19  bohman
 * 	Created.
 * 	[91/09/11  16:00:42  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2dev/mscsi.h
 *	Author: David E. Bohman II (CMU macmach)
 */

#define SCSI_NCTLR	7

#define SCSI_NBBLK	512
#define SCSI_NBBLKLOG2	9

#define SCSI_MAXPHYS	(32 * 1024)

#define SCSI_TSTUNITRDY	0x00
#define SCSI_READ	0x08
#define SCSI_WRITE	0x0a

#define scsiMSBlk(n)	(((n) >> 16) & 0x1f)
#define scsiBlk(n)	(((n) >> 8) & 0xff)
#define scsiLSBlk(n)	((n) & 0xff)

#define scsiMAXNBlk	256

#define scsiNBlk(n)	(((n) < scsiMAXNBlk)? (n): 0)

#define SCSI_CMDLEN	6	/* group 0 commands only */

typedef union {
    unsigned char	op;
    struct {
	unsigned char	op;
	unsigned char	lun:3,
			msblk:5;
	unsigned char	blk;
	unsigned char	lsblk;
	unsigned char	nblk;
    } rw;
    unsigned char	xxx[SCSI_CMDLEN];
} SCSICmdBlk;

#define SCSI_X_READ	0x01
#define SCSI_X_WRITE	0x02
#define SCSI_X_BLIND	0x04

#define scsiBranch(n)	((n) * sizeof (SCSITransferInstr))

#define SCSI_STAT_REQ	0x20
