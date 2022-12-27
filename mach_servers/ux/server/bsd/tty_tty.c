#if	CMU
/* 
 * Mach Operating System
 * Copyright (c) 1988 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/* 
 * HISTORY
 * $Log:	tty_tty.c,v $
 * Revision 2.2  89/08/09  14:35:37  rwd
 * 	Moved include of sys/conf.h to end for struct tty
 * 	[89/08/05  14:20:08  rwd]
 * 
 * Revision 2.3  88/08/24  01:31:35  mwyoung
 * 	Corrected include file references.
 * 	[88/08/22  22:05:09  mwyoung]
 * 
 *
 * 09-Apr-88  Mike Accetta (mja) at Carnegie-Mellon University
 *	Changed to require that the device number of the controlling
 *	terminal in the U area differ from that of the calling device
 *	before permitting access (other than to open).  The controlling
 *	terminal is now changed to this driver when a terminal is
 *	disconnected from all processes at "logout" time.
 *	[ V5.1(XF23) ]
 *
 ***********************************************************************;
 */ 

#include <cmucs.h>
#endif	CMU
 
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)tty_tty.c	7.1 (Berkeley) 6/5/86
 */

/*
 * Indirect driver for controlling tty.
 *
 * THIS IS GARBAGE: MUST SOON BE DONE WITH struct inode * IN PROC TABLE.
 */
#include <sys/param.h>
#include <sys/systm.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/ioctl.h>
#include <sys/tty.h>
#include <sys/proc.h>
#include <sys/uio.h>
#include <sys/conf.h>
#if	CMUCS

/*
 *  We use this as the tty structure for processes which have been disconnected
 *  from their original controlling terminal.  The only thing such processes
 *  can do with it is to open /dev/tty and issue a TIOCNOTTY to disconnect
 *  themselves from it (presumably to establish a real controlling terminal).
 */
struct tty sytty[1];
#endif	CMUCS

/*ARGSUSED*/
syopen(dev, flag)
	dev_t dev;
	int flag;
{
	if (u.u_ttyp == NULL)
		return (ENXIO);
#if	CMUCS
	if (u.u_ttyd == dev)
		return(0);
#endif	CMUCS
	return ((*cdevsw[major(u.u_ttyd)].d_open)(u.u_ttyd, flag));
}

/*ARGSUSED*/
syread(dev, uio)
	dev_t dev;
	struct uio *uio;
{

#if	CMUCS
	if (u.u_ttyp == NULL || u.u_ttyd == dev)
#else	CMUCS
	if (u.u_ttyp == NULL)
#endif	CMUCS
		return (ENXIO);
	return ((*cdevsw[major(u.u_ttyd)].d_read)(u.u_ttyd, uio));
}

/*ARGSUSED*/
sywrite(dev, uio)
	dev_t dev;
	struct uio *uio;
{

#if	CMUCS
	if (u.u_ttyp == NULL || u.u_ttyd == dev)
#else	CMUCS
	if (u.u_ttyp == NULL)
#endif	CMUCS
		return (ENXIO);
	return ((*cdevsw[major(u.u_ttyd)].d_write)(u.u_ttyd, uio));
}

/*ARGSUSED*/
syioctl(dev, cmd, addr, flag)
	dev_t dev;
	int cmd;
	caddr_t addr;
	int flag;
{

	if (cmd == TIOCNOTTY) {
		u.u_ttyp = 0;
		u.u_ttyd = 0;
		u.u_procp->p_pgrp = 0;
		return (0);
	}
#if	CMUCS
	if (u.u_ttyp == NULL || u.u_ttyd == dev)
#else	CMUCS
	if (u.u_ttyp == NULL)
#endif	CMUCS
		return (ENXIO);
	return ((*cdevsw[major(u.u_ttyd)].d_ioctl)(u.u_ttyd,cmd,addr,flag));
}

/*ARGSUSED*/
syselect(dev, flag)
	dev_t dev;
	int flag;
{

#if	CMUCS
	if (u.u_ttyp == NULL || u.u_ttyd == dev) {
#else	CMUCS
	if (u.u_ttyp == NULL) {
#endif	CMUCS
		u.u_error = ENXIO;
		return (0);
	}
	return ((*cdevsw[major(u.u_ttyd)].d_select)(u.u_ttyd, flag));
}
