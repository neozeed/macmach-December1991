/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	subr_log.c,v $
 * Revision 2.3  90/09/09  22:31:23  rpd
 * 	Made it to compile and work properly:
 * 		- new select technology
 * 		- resurvive MACH conditional for procp/thread pointers
 * 		- fixed a couple of race conditions
 * 	[90/08/14  11:43:24  af]
 * 
 * Revision 2.2  90/06/19  23:08:27  rpd
 * 	Changed gsignal in logwakeup to gsignal3.
 * 	Purged CMU, MACH conditionals.
 * 	[90/06/10            rpd]
 * 
 * Revision 2.1  89/08/04  14:10:18  rwd
 * Created.
 * 
 * Revision 2.3  88/07/15  15:34:17  mja
 * Add include for "sys/thread.h" on advice from DLB.
 * 
 * 15-Jun-88  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Use a thread (cast to a struct proc * :-() when doing selwakeup.
 *	Problem discovered by David Black.
 */
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)subr_log.c	7.1 (Berkeley) 6/5/86
 */

#include <mach_no_kernel.h>
#include <mach.h>

/*
 * Error log buffer for kernel printf's.
 */

#include <sys/param.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/proc.h>
#include <sys/ioctl.h>
#include <sys/msgbuf.h>
#include <sys/file.h>
#include <sys/errno.h>

#define LOG_RDPRI	(PZERO + 1)

#define LOG_NBIO	0x02
#define LOG_ASYNC	0x04
#define LOG_RDWAIT	0x08

struct logsoftc {
	int	sc_state;		/* see above for possibilities */
	struct	proc *sc_selp;		/* process waiting on select call */
	int	sc_pgrp;		/* process group for async I/O */
} logsoftc;

int	log_open;			/* also used in log() */

/*ARGSUSED*/
logopen(dev)
	dev_t dev;
{

	if (log_open)
		return (EBUSY);
	log_open = 1;
	logsoftc.sc_selp = 0;
	logsoftc.sc_pgrp = u.u_procp->p_pgrp;
	/*
	 * Potential race here with putchar() but since putchar should be
	 * called by autoconf, msg_magic should be initialized by the time
	 * we get here.
	 */
	if (msgbuf.msg_magic != MSG_MAGIC) {
		register int i;

		msgbuf.msg_magic = MSG_MAGIC;
		msgbuf.msg_bufx = msgbuf.msg_bufr = 0;
		for (i=0; i < MSG_BSIZE; i++)
			msgbuf.msg_bufc[i] = 0;
	}
	return (0);
}

/*ARGSUSED*/
logclose(dev, flag)
	dev_t dev;
{
#if	MACH_NO_KERNEL
	selwakeup((caddr_t)&logsoftc.sc_selp);
#else	MACH_NO_KERNEL
	logsoftc.sc_selp = 0;
#endif	MACH_NO_KERNEL
	logsoftc.sc_state = 0;
	logsoftc.sc_pgrp = 0;
	log_open = 0;
}

/*ARGSUSED*/
logread(dev, uio)
	dev_t dev;
	struct uio *uio;
{
	register long l;
	register int s;
	int error = 0;

	s = splhigh();
	while (msgbuf.msg_bufr == msgbuf.msg_bufx) {
		if (logsoftc.sc_state & LOG_NBIO) {
			splx(s);
			return (EWOULDBLOCK);
		}
		logsoftc.sc_state |= LOG_RDWAIT;
		sleep((caddr_t)&msgbuf, LOG_RDPRI);
	}
	splx(s);

	while (uio->uio_resid > 0) {
		l = msgbuf.msg_bufx - msgbuf.msg_bufr;
		if (l < 0)
			l = MSG_BSIZE - msgbuf.msg_bufr;
		l = MIN(l, uio->uio_resid);
		if (l == 0)
			break;
		error = uiomove((caddr_t)&msgbuf.msg_bufc[msgbuf.msg_bufr],
			(int)l, UIO_READ, uio);
		if (error)
			break;
		msgbuf.msg_bufr += l;
		if (msgbuf.msg_bufr < 0 || msgbuf.msg_bufr >= MSG_BSIZE)
			msgbuf.msg_bufr = 0;
	}
	return (error);
}

/*ARGSUSED*/
logselect(dev, rw)
	dev_t dev;
	int rw;
{
	int s = splhigh();

	switch (rw) {

	case FREAD:
		if (msgbuf.msg_bufr != msgbuf.msg_bufx) {
			splx(s);
			return (1);
		}
#if	MACH_NO_KERNEL
		selenter((caddr_t)&logsoftc.sc_selp);
#else	MACH_NO_KERNEL
#if	MACH
		logsoftc.sc_selp = (struct proc *) current_thread();
#else	MACH
		logsoftc.sc_selp = u.u_procp;
#endif	MACH
#endif	MACH_NO_KERNEL
		break;
	}
	splx(s);
	return (0);
}

logwakeup()
{

	if (!log_open)
		return;
#if	MACH_NO_KERNEL
	selwakeup((caddr_t)&logsoftc.sc_selp);
#else	MACH_NO_KERNEL
	if (logsoftc.sc_selp) {
		selwakeup(logsoftc.sc_selp, 0);
		logsoftc.sc_selp = 0;
	}
#endif	MACH_NO_KERNEL
	if (logsoftc.sc_state & LOG_ASYNC)
		gsignal3(logsoftc.sc_pgrp, SIGIO, FALSE); 
	if (logsoftc.sc_state & LOG_RDWAIT) {
		wakeup((caddr_t)&msgbuf);
		logsoftc.sc_state &= ~LOG_RDWAIT;
	}
}

/*ARGSUSED*/
logioctl(com, data, flag)
	caddr_t data;
{
	long l;
	int s;

	switch (com) {

	/* return number of characters immediately available */
	case FIONREAD:
		s = splhigh();
		l = msgbuf.msg_bufx - msgbuf.msg_bufr;
		splx(s);
		if (l < 0)
			l += MSG_BSIZE;
		*(off_t *)data = l;
		break;

	case FIONBIO:
		if (*(int *)data)
			logsoftc.sc_state |= LOG_NBIO;
		else
			logsoftc.sc_state &= ~LOG_NBIO;
		break;

	case FIOASYNC:
		if (*(int *)data)
			logsoftc.sc_state |= LOG_ASYNC;
		else
			logsoftc.sc_state &= ~LOG_ASYNC;
		break;

	case TIOCSPGRP:
		logsoftc.sc_pgrp = *(int *)data;
		break;

	case TIOCGPGRP:
		*(int *)data = logsoftc.sc_pgrp;
		break;

	default:
		return (-1);
	}
	return (0);
}
