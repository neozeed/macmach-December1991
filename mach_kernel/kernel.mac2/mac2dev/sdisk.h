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
 * $Log:	sdisk.h,v $
 * Revision 2.2  91/09/12  16:47:39  bohman
 * 	Created.
 * 	[91/09/11  16:02:51  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2dev/sdisk.h
 *	Author: David E. Bohman II (CMU macmach)
 */

#define SD_PART_INFO	0

typedef struct {
    unsigned		parts;		/* num of partitions from map */
    unsigned		length;		/* of partition in blocks */
    unsigned char	name[32];
    unsigned char	type[32];
} sd_part_info_t;

#define SD_PART_INFO_COUNT	(sizeof (sd_part_info_t) / sizeof (int))

#define SD_REC_SIZE		512
#define SD_REC_SIZE_LOG2	9
