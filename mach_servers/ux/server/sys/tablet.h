/* 
 * Mach Operating System
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 **********************************************************************
 * HISTORY
 * $Log:	tablet.h,v $
 * Revision 2.2  91/03/20  15:04:22  dbg
 * 	Fixed IOCTLs for ANSI C.
 * 	[91/02/21            dbg]
 * 
 * Revision 2.1  89/08/04  14:48:46  rwd
 * Created.
 * 
 * Revision 2.4  88/10/27  10:51:37  rpd
 * 	romp:  Added the TBIOGETC and TBIOSETC ioctls.
 * 	[88/10/26  14:50:08  rpd]
 * 
 * Revision 2.3  88/10/01  21:58:51  rpd
 * 	Added ROMP_TBCOMPAT support.
 * 	[88/09/28  17:05:27  rpd]
 * 
 * Revision 2.2  88/08/24  02:46:04  mwyoung
 * 	Adjusted include file references.
 * 	[88/08/17  02:23:56  mwyoung]
 * 
 *
 * 15-Jul-87  Bill Bolosky (bolosky) at Carnegie-Mellon University
 *	Renamed struct tbpos to struct tbmspos in accordance w/new acis
 *	code.
 *
 * 22-Jan-87  Richard Sanzi (sanzi) at Carnegie-Mellon University
 *	Merged in 4.2 ACIS files h/tbdefs.h and h/tbioctl.h.
 *	Changed 'rets' field in "struct tb" to be an array, under switch romp.
 *	Added other fields to support the queue also under switch romp.
 *	Added several tablet numbers and new ioctl numbers to support X.
 **********************************************************************
 */
 
/*
 * Copyright (c) 1985, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)tablet.h	7.1 (Berkeley) 6/4/86
 */

#ifndef _TABLET_
/*
 * Tablet line discipline.
 */

#ifdef	KERNEL
#ifdef	romp
#include <romp_tbcompat.h>
#endif	romp
#else	KERNEL
#ifdef	romp
#include <sys/features.h>
#endif	romp
#endif	KERNEL

#ifdef	KERNEL
#include <sys/ioctl.h>
#endif	KERNEL

/*
 * Reads on the tablet return one of the following
 * structures, depending on the underlying tablet type.
 * The first two are defined such that a read of
 * sizeof (gtcopos) on a non-gtco tablet will return
 * meaningful info.  The in-proximity bit is simulated
 * where the tablet does not directly provide the information.
 */
struct	tbmspos {
	int	xpos, ypos;	/* raw x-y coordinates */
	short	status;		/* buttons/pen down */
#define	TBINPROX	0100000		/* pen in proximity of tablet */
	short	scount;		/* sample count */
};

struct	gtcopos {
	int	xpos, ypos;	/* raw x-y coordinates */
	short	status;		/* as above */
	short	scount;		/* sample count */
	short	xtilt, ytilt;	/* raw tilt */
	short	pressure;
	short	pad;		/* pad to longword boundary */
};

struct	polpos {
	short	p_x, p_y, p_z;	/* raw 3-space coordinates */
	short	p_azi, p_pit, p_rol;	/* azimuth, pitch, and roll */
	short	p_stat;		/* status, as above */
	char	p_key;		/* calculator input keyboard */
};

/*
 * Tablet state
 */

#define NTBS		(16)
#define	TBMAXREC	(17)	/* max input record size */
#define TBQUEUESIZE	(5)

struct tb {
	int	tbflags;		/* mode & type bits */
	char	cbuf[TBMAXREC];		/* input buffer */
#ifdef	romp
	short	lastindex;
	short	curindex;
#endif	romp
	union {
		struct	tbmspos tbpos;
		struct	gtcopos gtcopos;
		struct	polpos polpos;
#ifdef	romp		
	} rets[TBQUEUESIZE];				/* processed state */
#else	romp
        } rets;
#endif	romp	
} tb[NTBS];


#define BIOSMODE	_IOW('b', 1, int)	/* set mode bit(s) */
#define BIOGMODE	_IOR('b', 2, int)	/* get mode bit(s) */
#define	TBMODE		0xfff0		/* mode bits: */
#define		TBPOINT		0x0010		/* single point */
#define		TBRUN		0x0000		/* runs contin. */
#define		TBSTOP		0x0020		/* shut-up */
#define		TBGO		0x0000		/* ~TBSTOP */
#define		TBBINDATA	0x0040	/* Mask off high bit of in data? */
#define	TBTYPE		0x000f		/* tablet type: */
#if	defined(romp) && ROMP_TBCOMPAT
#define		HITACHI_DISC	0
#define		GTCO_DISC	1
#define		CALCOMP_DISC	2
#define		PCMS_DISC	3
#define		PLANMS_DISC3	4
#define		PLANMS_DISC2	5
#else	defined(romp) && ROMP_TBCOMPAT
#define		TBUNUSED	0x0000
#define		TBHITACHI	0x0001		/* hitachi tablet */
#define		TBTIGER		0x0002		/* hitachi tiger */
#define		TBGTCO		0x0003		/* gtco */
#define		TBPOL		0x0004		/* polhemus 3space */
#define		TBHDG		0x0005		/* hdg-1111b, low res */
#define		TBHDGHIRES	0x0006		/* hdg-1111b, high res */
#define 	CALCOMP_DISC	0x0007		
#define 	PCMS_DISC	0x0008
#define 	PLANMS_DISC3	0x0009
#define 	PLANMS_DISC2	0x000a
#ifdef	romp
#define		HITACHI_DISC	TBHITACHI	/* For compatability */
#define		GTCO_DISC	TBGTCO
#endif	romp
#endif	defined(romp) && ROMP_TBCOMPAT

#define TBIOGETD	_IOR('b',0,int)
#define TBIOSETD	_IOWR('b',1,int)
#define BIOSTYPE	_IOW('b', 3, int)	/* set tablet type */
#define BIOGTYPE	_IOR('b', 4, int)	/* get tablet type*/
#if	defined(romp)
#define TBIOGETC	_IOR('b',5,int)		/* get compat mode flag */
#define TBIOSETC	_IOW('b',6,int)		/* set compat mode flag */
#endif	defined(romp)

#endif	_TABLET_
