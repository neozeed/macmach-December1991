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
 * $Log:	setroot.c,v $
 * Revision 2.2  91/09/12  16:43:42  bohman
 * 	Created.
 * 	[91/09/11  15:02:41  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/setroot.c
 */

char *root_name;

get_root_device()
{
    extern int bootdev; /* from locore.s */
    int minor;
    int major;

/*    minor = bootdev & 0xFF;*/
    minor = (bootdev >> 5) & 0x7;
    major = (bootdev >> 8) & 0xFF;

    if (major == 0) {
      root_name = "hdXa";
      root_name[2] = '0' + minor;
    }
    else if (major == 8) root_name = "rd0";
    else root_name = "   ";
}
