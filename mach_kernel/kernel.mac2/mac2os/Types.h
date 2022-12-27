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
 * $Log:	Types.h,v $
 * Revision 2.2  91/09/12  16:51:49  bohman
 * 	Created Mach 3.0 version from 2.5 version.
 * 	[91/09/11  16:36:35  bohman]
 * 
 * Revision 2.2  90/08/30  11:46:57  bohman
 * 	Created.
 * 	[90/08/29  13:00:32  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2os/Types.h
 */

/*
	Types.h -- Common Defines and Types

	C Interface to the Macintosh Libraries
	Copyright Apple Computer,Inc. 1985-1987
	All rights reserved.
*/

#ifndef __TYPES__
#define __TYPES__

#define nil 0
#define NULL 0

enum {false,true};

typedef unsigned char Boolean;
typedef unsigned char *Ptr;
typedef Ptr *Handle;
typedef void (*ProcPtr)();
typedef ProcPtr *ProcHandle;
typedef long Fixed;
typedef long Fract;
typedef unsigned long ResType;
typedef long OSType;
typedef short OSErr;
typedef unsigned char Style;

typedef struct Point {
    short v;
    short h;
} Point;

typedef struct Rect {
    short top;
    short left;
    short bottom;
    short right;
} Rect;

#define String(size)		\
struct {			\
    unsigned char length;	\
    unsigned char text[size];	\
}

typedef String(255) Str255,*StringPtr,**StringHandle;
typedef String(27) Str27;
typedef String(63) Str63;

#endif
