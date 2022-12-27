/* 
 * Mach Operating System
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 **********************************************************************
 * HISTORY
 * $Log:	uio.h,v $
 * Revision 2.1  89/08/04  14:49:15  rwd
 * Created.
 * 
 * Revision 2.3  88/08/24  02:51:09  mwyoung
 * 	Adjusted include file references.
 * 	[88/08/17  02:26:52  mwyoung]
 * 
 *
 * 26-Feb-88  David Kirschen (kirschen) at Encore Computer Corporation
 *      Added #include of types.h to get caddr_t.
 *
 * 06-Jan-88  Jay Kistler (jjk) at Carnegie Mellon University
 *	Added declarations for __STDC__.
 *
 **********************************************************************
 */
 
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)uio.h	7.1 (Berkeley) 6/4/86
 */

#include <sys/types.h>

#ifndef _UIO_
#define	_UIO_	1

struct iovec {
	caddr_t	iov_base;
	int	iov_len;
};

struct uio {
	struct	iovec *uio_iov;
	int	uio_iovcnt;
	off_t	uio_offset;
	int	uio_segflg;
	int	uio_resid;
};

enum	uio_rw { UIO_READ, UIO_WRITE };

/*
 * Segment flag values (should be enum).
 */
#define UIO_USERSPACE	0		/* from user data space */
#define UIO_SYSSPACE	1		/* from system space */
#define UIO_USERISPACE	2		/* from user I space */

#if	defined(CMUCS) && defined(__STDC__) && !defined(KERNEL)
extern int readv(int, struct iovec *, int);
extern int writev(int, struct iovec *, int);
#endif	defined(CMUCS) && defined(__STDC__) && !defined(KERNEL)
#endif _UIO_
