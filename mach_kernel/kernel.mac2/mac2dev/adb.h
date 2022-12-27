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
 * $Log:	adb.h,v $
 * Revision 2.2  91/09/12  16:45:48  bohman
 * 	Created.
 * 	[91/09/11  15:24:40  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2dev/adb.h
 *	Author: David E. Bohman II (CMU macmach)
 */

#ifndef	_MAC2DEV_ADB_H_
#define _MAC2DEV_ADB_H_

#define ADB_INFO	0

typedef struct {
    unsigned char	addr;	/* original address */
    unsigned char	type;
} adb_info_t;

#define ADB_INFO_COUNT	(sizeof (adb_info_t) / sizeof (int))

#define ADB_FLUSH	1

typedef union {
    struct {
	unsigned char	cmd;
	unsigned char	len;
    } cmd;
    struct {
	unsigned char	addr:4,
			 cmd:4;
#define ADB_reset	0
#define ADB_flush	1
    } gen;
    struct {
	unsigned char	addr:4,
			 cmd:2,
#define ADB_listen	2
#define ADB_talk	3
			 reg:2;
    } reg;
} adb_cmd_t;

#define ADB_ADDR_KEYBOARD	2
#define ADB_ADDR_MOUSE		3

#endif	_MAC2DEV_ADB_H_
