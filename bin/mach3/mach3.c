/* 
 * Mach Operating System
 * Copyright (c) 1991,1990 Carnegie Mellon University
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 * 
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS 
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
 * any improvements or extensions that they make and grant Carnegie the
 * rights to redistribute these changes.
 */
/*
 * HISTORY
 * $Log:	mach3.c,v $
 * Revision 2.4  91/03/27  17:24:41  mrt
 * 	Changed mach.h include
 * 
 * Revision 2.3  91/03/19  12:23:50  mrt
 * 	Changed to new copyright
 * 
 * Revision 2.2  90/11/05  23:33:33  rpd
 * 	Created.
 * 	[90/11/05            rpd]
 * 
 */

main(int argc, char **argv)
{
  int i;

  if (argc != 1) {
    printf( "usage: mach3\n"); 
    exit(1);
  }
  /* On Mach 3.0, this should return MACH_MSG_SUCCESS (0).
   * On Mach 2.5, this should return KERN_INVALID_ARGUMENT (4).
   * Convert to a shell return code: true (zero) when running on Mach 3.0.
   */
  exit(mach_msg(0, 0, 0, 0, 0, 0, 0) ? 1 : 0);
}
