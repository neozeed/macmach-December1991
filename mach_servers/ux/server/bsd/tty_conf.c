/* 
 * Mach Operating System
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 **********************************************************************
 * HISTORY
 *  2-Feb-89  David Golub (dbg) at Carnegie-Mellon University
 *	Always include select entry in linesw.
 *
 * $Log:	tty_conf.c,v $
 * Revision 2.1  89/08/04  14:06:16  rwd
 * Created.
 * 
 * Revision 2.2  88/11/14  14:31:46  gm0w
 * 	Added CSNET DialupIP support.
 * 	[88/11/14            gm0w]
 * 
 *  9-Apr-87  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Handle ROMP_TB.
 *
 * 24-Dec-86  Bill Bolosky (bolosky) at Carnegie-Mellon University
 *	romp: Added definitions for the l_select field used by X on the
 *	RT.  Also removed now defunct aed_tty entries (on line 7).
 *
 *  5-Aug-86  Jonathan J. Chew (jjc) at Carnegie-Mellon University
 *	Added support for Sun mouse and keyboard.
 *
 * 16-Feb-86  Bill Bolosky (bolosky) at Carnegie-Mellon University
 *	Added support for Sailboat MS and AED devices under switch romp.
 *
 * 25-Jan-86  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Upgraded to 4.3.
 *
 * 30-Jul-85  Mike Accetta (mja) at Carnegie-Mellon University
 *	NFE:  Added Front End line discipline.
 *	[V1(1)]
 *
 **********************************************************************
 */
 

#include "fe.h"
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)tty_conf.c	7.1 (Berkeley) 6/5/86
 */

#include "sys/param.h"
#include "sys/systm.h"
#include "sys/buf.h"
#include "sys/ioctl.h"
#include "sys/tty.h"
#include "sys/conf.h"

int	nodev();
int	nulldev();

int	ttyopen(),ttylclose(),ttread(),ttwrite(),nullioctl(),ttstart();
int	ttymodem(), nullmodem(), ttyinput();
int	ttselect();

#include "bk.h"
#if NBK > 0
int	bkopen(),bkclose(),bkread(),bkinput(),bkioctl();
#endif

#ifdef	romp
#include "romp_tb.h"
#undef	NTB
#define	NTB	NROMP_TB
#else	romp
#include "tb.h"
#endif	romp
#if NTB > 0 || NROMP_TB > 0
int	tbopen(),tbclose(),tbread(),tbinput(),tbioctl(),tbselect();
#endif
#ifdef	romp
/* This is a kludge; supposed to be machine independent -- sac */
#include "ms.h"
#if NMS > 0
int	msdopen(),msdclose(),msdread(),msdinput(),msdioctl(),msdselect();
#endif NMS
#endif	romp

#ifdef	sun
#include "ms.h"
#if NMS > 0
int     msopen(), msclose(), msread(), msioctl(), msinput();
#endif

#include "kb.h"
#if NKB > 0
int     kbdopen(), kbdclose(), kbdread(), kbdioctl(), kbdinput();
#endif
#endif	sun

#include "sl.h"
#if NSL > 0
int	slopen(),slclose(),slinput(),sltioctl(),slstart();
#endif

#ifdef	vax
#include "du.h"
#if NDU > 0
int	dutopen(),dutclose(),dutinput(),dutioctl(),dutstart(),dutmodem();
#endif
#endif


struct	linesw linesw[] =
{
    {							/* 0- OTTYDISC */
	ttyopen, ttylclose, ttread, ttwrite, nullioctl,
	ttyinput, nodev, nulldev, ttstart, ttymodem,
	ttselect
    },
    {							/* 1- NETLDISC */
#if NBK > 0
	bkopen, bkclose, bkread, ttwrite, bkioctl,
	bkinput, nodev, nulldev, ttstart, nullmodem,
	ttselect
#else
	nodev, nodev, nodev, nodev, nodev,
	nodev, nodev, nodev, nodev, nodev,
	nodev
#endif
    },
    {							/* 2- NTTYDISC */
	ttyopen, ttylclose, ttread, ttwrite, nullioctl,
	ttyinput, nodev, nulldev, ttstart, ttymodem,
	ttselect
    },
    {							/* 3- TABLDISC */
#if NTB > 0
	tbopen, tbclose, tbread, nodev, tbioctl,
	tbinput, nodev, nulldev, ttstart, nullmodem,
#ifdef	romp
	tbselect
#else	romp
	ttselect
#endif	romp
#else
	nodev, nodev, nodev, nodev, nodev,
	nodev, nodev, nodev, nodev, nodev,
	nodev
#endif
    },
    {							/* 4- SLIPDISC */
#if NSL > 0
	slopen, slclose, nodev, nodev, sltioctl,
	slinput, nodev, nulldev, slstart, nulldev,
	nodev
#else
	nodev, nodev, nodev, nodev, nodev,
	nodev, nodev, nodev, nodev, nodev,
	nodev
#endif
    },
    {							/* 5- MOUSELDISC */
#if	defined(romp) && NMS > 0
	msdopen, msdclose, msdread, nodev, msdioctl,
	msdinput, nodev, nulldev, ttstart, nulldev,
	msdselect
#else	defined(romp) && NMS > 0
#if	defined(sun) && NMS > 0
	msopen, msclose, msread, nodev, msioctl,
	msinput, nodev, nulldev, nulldev, nulldev,
	ttselect
#else	defined(sun) && NMS > 0
	nodev, nodev, nodev, nodev, nodev,
	nodev, nodev, nodev, nodev, nodev,
	nodev
#endif	defined(sun) && NMS > 0
#endif	defined(romp) && NMS > 0
    },
    {							/* 6- KBDLDISC */
#if	defined(sun) && NKB > 0
        kbdopen, kbdclose, kbdread, ttwrite, kbdioctl,
	kbdinput, nodev, nulldev, ttstart, nulldev,
	ttselect
#else	defined(sun) && NKB > 0
	nodev, nodev, nodev, nodev, nodev,
	nodev, nodev, nodev, nodev, nodev,
	nodev
#endif	defined(sun) && NKB > 0
    },
    {							/* 7- DUDISC */
#if NDU > 0
	dutopen, dutclose, nodev, nodev, dutioctl,
	dutinput, nodev, nulldev, dutstart, dutmodem,
	nodev
#else
	nodev, nodev, nodev, nodev, nodev,
	nodev, nodev, nodev, nodev, nodev,
	nodev
#endif
    }
};

int	nldisp = sizeof (linesw) / sizeof (linesw[0]);

/*
 * Do nothing specific version of line
 * discipline specific ioctl command.
 */
/*ARGSUSED*/
nullioctl(tp, cmd, data, flags)
	struct tty *tp;
	char *data;
	int flags;
{

#ifdef lint
	tp = tp; data = data; flags = flags;
#endif
	return (-1);
}
