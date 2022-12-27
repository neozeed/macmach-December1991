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
 * any improvements or extensions that they make and grant Carnegie Mellon rights
 * to redistribute these changes.
 */
/*
 * HISTORY
 * $Log:	types.h,v $
 * Revision 2.6  91/05/14  17:40:39  mrt
 * 	Correcting copyright
 * 
 * Revision 2.5  91/02/05  17:57:07  mrt
 * 	Changed to new Mach copyright
 * 	[91/02/01  17:49:41  mrt]
 * 
 * Revision 2.4  90/08/27  22:13:03  dbg
 * 	Created.
 * 	[90/07/16            dbg]
 * 
 */
#ifndef	_SYS_TYPES_H_
#define	_SYS_TYPES_H_

/*
 * Common type definitions that lots of old files seem to want.
 */

typedef	unsigned char	u_char;		/* unsigned char */
typedef	unsigned short	u_short;	/* unsigned short */
typedef	unsigned int	u_int;		/* unsigned int */
typedef	unsigned long	u_long;		/* unsigned long */

typedef struct _quad_ {
	unsigned long	val[2];		/* 2 long values make... */
} quad;					/* an 8-byte item */

typedef	char *		caddr_t;	/* address of a (signed) char */

typedef	unsigned long	time_t;		/* an unsigned long */
typedef unsigned long	daddr_t;	/* an unsigned long */
typedef	unsigned long	off_t;		/* another unsigned long */

typedef	unsigned short	dev_t;		/* another unsigned short */
#define	NODEV		((dev_t)-1)	/* and a null value for it */

#define	major(i)	((i) >> 8)
#define	minor(i)	((i) & 0xFF)
#define	makedev(i,j)	(((i) << 8) | (j))

#ifndef	NULL
#define	NULL		(0)
					/* the null pointer */
#endif

#endif	/* _SYS_TYPES_H_ */
