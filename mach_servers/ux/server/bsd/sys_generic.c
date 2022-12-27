/* 
 * Mach Operating System
 * Copyright (c) 1988 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/* 
 * HISTORY
 * $Log:	sys_generic.c,v $
 * Revision 2.7  91/03/20  14:58:35  dbg
 * 	Fixed/simplified unix_master locking in rwuio.
 * 	[91/02/02            rpd]
 * 
 * Revision 2.6  90/09/09  22:31:38  rpd
 * 	Just cleaning spots.
 * 	[90/08/14  11:44:53  af]
 * 
 * Revision 2.5  90/08/06  15:32:20  rwd
 * 	Return -1 on error in select.
 * 	[90/06/22            rwd]
 * 	Change select locking to use select_lock instead of splhigh().
 * 	[90/06/11            rwd]
 * 
 * 	Remove some excess copying in select().
 * 	[90/06/11            rwd]
 * 	Remove Conditionals.  Make select two pass.  Add debuging
 * 	code.  Convert references to u. to uth-> with uth as a
 * 	register variable in select() and ioctl().
 * 	[90/06/07            rwd]
 * 
 * Revision 2.4  90/06/19  23:08:37  rpd
 * 	Purged CMUCS, MACH, MACH_NO_KERNEL conditionals.
 * 	Made on_master in rwuio volatile.
 * 	[90/06/06            rpd]
 * 
 * Revision 2.3  89/11/29  15:27:49  af
 * 	Ioctl tweaks for Mips&Suntools .
 * 	[89/11/16  16:59:50  af]
 * 
 * Revision 2.2  89/08/31  16:28:45  rwd
 * 	In select timeout occurs when hzto is 0 instead of doing a timer
 * 	compare
 * 	[89/08/18            rwd]
 * 
 * 	Out-of-kernel version.  Use new select mechanism.
 * 	[89/01/10            dbg]
 * 
 * Revision 2.1  89/08/04  14:07:09  rwd
 * Created.
 * 
 * Revision 2.3  88/08/24  01:29:03  mwyoung
 * 	Corrected include file references.
 * 	[88/08/23  00:38:11  mwyoung]
 * 
 * 27-Oct-87  Robert Baron (rvb) at Carnegie-Mellon University
 *	Syscall now does read/write readv/writev in parallel and rwuio
 *	does unix_master call.
 *
 * 30-Sep-87  David Golub (dbg) at Carnegie-Mellon University
 *	New scheduling state machine.
 *
 * 31-Jan-87  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Support for multiple threads.
 *
 * 27-Jan-87  Mike Accetta (mja) at Carnegie-Mellon University
 *	Modified to allow new FNOSPC flag bit in file table to prohibit
 *	any disk space resource pauses on a per-file basis.
 *	[ V5.1(F1) ]
 *
 * 25-Jan-86  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Upgraded to 4.3.
 *
 * 10-Oct-85  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Apply bug fix for selwakeup.
 *
 * 03-Aug-85  Mike Accetta (mja) at Carnegie-Mellon University
 *	CS_RPAUSE:  added resource pause hook in rwuio() for
 *	disk space exhaustion.
 *
 **********************************************************************
 */
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)sys_generic.c	7.1 (Berkeley) 6/5/86
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/ioctl.h>
#include <sys/file.h>
#include <sys/proc.h>
#include <sys/uio.h>
#include <sys/kernel.h>
#include <sys/stat.h>

#include <uxkern/import_mach.h>
#include <sys/parallel.h>

/*
 * Read system call.
 */
read()
{
	register struct a {
		int	fdes;
		char	*cbuf;
		unsigned count;
	} *uap = (struct a *)u.u_ap;
	struct uio auio;
	struct iovec aiov;

	aiov.iov_base = (caddr_t)uap->cbuf;
	aiov.iov_len = uap->count;
	auio.uio_iov = &aiov;
	auio.uio_iovcnt = 1;
	rwuio(&auio, UIO_READ);
}

readv()
{
	register struct a {
		int	fdes;
		struct	iovec *iovp;
		unsigned iovcnt;
	} *uap = (struct a *)u.u_ap;
	struct uio auio;
	struct iovec aiov[16];		/* XXX */

	if (uap->iovcnt > sizeof(aiov)/sizeof(aiov[0])) {
		u.u_error = EINVAL;
		return;
	}
	auio.uio_iov = aiov;
	auio.uio_iovcnt = uap->iovcnt;
	u.u_error = copyin((caddr_t)uap->iovp, (caddr_t)aiov,
	    uap->iovcnt * sizeof (struct iovec));
	if (u.u_error)
		return;
	rwuio(&auio, UIO_READ);
}

/*
 * Write system call
 */
write()
{
	register struct a {
		int	fdes;
		char	*cbuf;
		unsigned count;
	} *uap = (struct a *)u.u_ap;
	struct uio auio;
	struct iovec aiov;

	auio.uio_iov = &aiov;
	auio.uio_iovcnt = 1;
	aiov.iov_base = uap->cbuf;
	aiov.iov_len = uap->count;
	rwuio(&auio, UIO_WRITE);
}

writev()
{
	register struct a {
		int	fdes;
		struct	iovec *iovp;
		unsigned iovcnt;
	} *uap = (struct a *)u.u_ap;
	struct uio auio;
	struct iovec aiov[16];		/* XXX */

	if (uap->iovcnt > sizeof(aiov)/sizeof(aiov[0])) {
		u.u_error = EINVAL;
		return;
	}
	auio.uio_iov = aiov;
	auio.uio_iovcnt = uap->iovcnt;
	u.u_error = copyin((caddr_t)uap->iovp, (caddr_t)aiov,
	    uap->iovcnt * sizeof (struct iovec));
	if (u.u_error)
		return;
	rwuio(&auio, UIO_WRITE);
}

rwuio(uio, rw)
	register struct uio *uio;
	enum uio_rw rw;
{
	struct a {
		int	fdes;
	};
	register struct file *fp;
	register struct iovec *iov;
	int i, count;
	int rcount;	/* resume count */

	GETF(fp, ((struct a *)u.u_ap)->fdes);
	if ((fp->f_flag&(rw==UIO_READ ? FREAD : FWRITE)) == 0) {
		u.u_error = EBADF;
		return;
	}
	uio->uio_resid = 0;
	uio->uio_segflg = UIO_USERSPACE;
	iov = uio->uio_iov;
	for (i = 0; i < uio->uio_iovcnt; i++) {
		if (iov->iov_len < 0) {
			u.u_error = EINVAL;
			return;
		}
		uio->uio_resid += iov->iov_len;
		if (uio->uio_resid < 0) {
			u.u_error = EINVAL;
			return;
		}
		iov++;
	}
	count = uio->uio_resid;
	unix_master();
resume:
	rcount = uio->uio_resid;
	uio->uio_offset = fp->f_offset;
	if (setjmp(&u.u_qsave)) {
		if (uio->uio_resid == count) {
			if ((u.u_sigintr & sigmask(u.u_procp->p_cursig)) != 0)
				u.u_error = EINTR;
			else
				u.u_eosys = RESTARTSYS;
		}
	} else {
		u.u_error = (*fp->f_ops->fo_rw)(fp, rw, uio);
	}
	u.u_r.r_val1 = count - uio->uio_resid;
	fp->f_offset += (rcount - uio->uio_resid);
	if (u.u_error && fspause(fp->f_flag&FNOSPC)) goto resume;
	unix_release();
}

/*
 * Ioctl system call
 */
ioctl()
{
	register struct file *fp;
	struct a {
		int	fdes;
		int	cmd;
		caddr_t	cmarg;
	} *uap;
	register int com;
	register u_int size;
	char data[IOCPARM_MASK+1];
	struct uthread *uth = &u;

	uap = (struct a *)uth->u_ap;
	GETF(fp, uap->fdes);
	if ((fp->f_flag & (FREAD|FWRITE)) == 0) {
		uth->u_error = EBADF;
		return;
	}
	com = uap->cmd;

#if defined(vax) && defined(COMPAT)
	/*
	 * Map old style ioctl's into new for the
	 * sake of backwards compatibility (sigh).
	 */
	if ((com&~0xffff) == 0) {
		com = mapioctl(com);
		if (com == 0) {
			uth->u_error = EINVAL;
			return;
		}
	}
#endif
	if (com == FIOCLEX) {
		uth->u_pofile[uap->fdes] |= UF_EXCLOSE;
		return;
	}
	if (com == FIONCLEX) {
		uth->u_pofile[uap->fdes] &= ~UF_EXCLOSE;
		return;
	}

	/*
	 * Interpret high order word to find
	 * amount of data to be copied to/from the
	 * user's address space.
	 */
	size = (com &~ (IOC_INOUT|IOC_VOID)) >> 16;
	if (size > sizeof (data)) {
		uth->u_error = EFAULT;
		return;
	}
	if (com&IOC_IN) {
#if	mips || sun	/* Needed on Sun's to make Suntools happy */
		if (size == sizeof (int) && uap->cmarg == NULL)
			*(int *)data = 0;
		else
#endif	mips || sun
		if (size) {
			uth->u_error =
			    copyin(uap->cmarg, (caddr_t)data, (u_int)size);
			if (uth->u_error)
				return;
		} else
			*(caddr_t *)data = uap->cmarg;
	} else if ((com&IOC_OUT) && size)
		/*
		 * Zero the buffer on the stack so the user
		 * always gets back something deterministic.
		 */
		bzero((caddr_t)data, size);
	else if (com&IOC_VOID)
		*(caddr_t *)data = uap->cmarg;

	switch (com) {

	case FIONBIO:
		uth->u_error = fset(fp, FNDELAY, *(int *)data);
		return;

	case FIOASYNC:
		uth->u_error = fset(fp, FASYNC, *(int *)data);
		return;

	case FIOSETOWN:
		uth->u_error = fsetown(fp, *(int *)data);
		return;

	case FIOGETOWN:
		uth->u_error = fgetown(fp, (int *)data);
		return;
	}
	uth->u_error = (*fp->f_ops->fo_ioctl)(fp, com, data);
	/*
	 * Copy any data to user, size was
	 * already set and checked above.
	 */
	if (uth->u_error == 0 && (com&IOC_OUT) && size)
		uth->u_error = copyout(data, uap->cmarg, (u_int)size);
}

#ifdef	DEBUG
#include <cthreads.h>
int		select_debug = FALSE;
struct mutex	select_mutex = MUTEX_INITIALIZER;
int		select_count = 0;
int		select_timeout = 0;
int		select_first = 0;
int		select_later = 0;
int		select_zftime = 0;
int		select_ztime = 0;
int		select_during = 0;
int		select_sig = 0;
int		select_none = 0;
#endif	DEBUG
struct selbuf {
	int	s_count;	/* number of items in select list */
	int	s_state;	/* state of select buffer: */
#define	SB_CLEAR	0		/* wait condition cleared */
#define	SB_WILLWAIT	1		/* will wait if not cleared */
#define	SB_WAITING	2		/* waiting - wake up */
	struct selitem {
		struct	selitem *si_next;	/* next selbuf in item chain */
		struct	selitem	**si_dev;	/* head of item chain */
		struct	selbuf	*si_selbuf;	/* 'thread' waiting */
	} s_item[NOFILE];
};

struct mutex select_lock = MUTEX_INITIALIZER;
int	unselect();

/*
 * Select system call.
 */
select()
{
	register struct uthread *uth = &u;
	register struct uap  {
		int	nd;
		fd_set	*in, *ou, *ex;
		struct	timeval *tv;
	} *uap = (struct uap *)uth->u_ap;
	fd_set obits[3];
	struct timeval atv;
	int	result;
	struct	selbuf	selbuf;
	struct	timeval	time;
	register int timeout_hzto;
#ifdef	DEBUG
	int	times = -1;
#endif	DEBUG

#ifdef	DEBUG
	if (select_debug) {
		mutex_lock(&select_mutex);
		select_count++;
		if (uap->tv) select_timeout++;
		mutex_unlock(&select_mutex);
	}
#endif	DEBUG
	bzero((caddr_t)obits, sizeof(obits));
	if (uap->nd > NOFILE)
		uap->nd = NOFILE;	/* forgiving, if slightly wrong */

	if (uap->tv) {
		bcopy((caddr_t)uap->tv, (caddr_t)&atv, sizeof (atv));
		if (itimerfix(&atv)) {
			uth->u_error = EINVAL;
			goto done;
		}
		get_time(&time);
		timevaladd(&atv, &time);
	}
	uth->uu_sb = &selbuf;
	selbuf.s_count = 0;

	while (TRUE) {
#ifdef	DEBUG
	    if (select_debug)
		times++;
#endif	DEBUG
	    if (uth->uu_sb) {
		uth->uu_sb = 0;
	    } else {
		uth->uu_sb = &selbuf;
		/*
		 * Initialize the select buffer.  Set state without lock,
		 * since not pointed to by any devices yet.
		 */
		selbuf.s_state = SB_WILLWAIT;

	    }
	    /*
	     * Scan for conditions.  If state is SB_CLEAR at end,
	     * one of the conditions became true.
	     */
	    uth->u_r.r_val1 = selscan(uap->in, uap->ou, uap->ex,
				      obits, uap->nd);
	    if (uth->u_error || uth->u_r.r_val1) {
#ifdef	DEBUG
		if (select_debug) {
			mutex_lock(&select_mutex);
			if (times) {
			    select_later++;
			} else {
			    select_first++;
			}
			mutex_unlock(&select_mutex);
		}
#endif	DEBUG
		break;
	    }

	    if (uap->tv) {
		timeout_hzto = hzto(&atv);
		if (timeout_hzto == 0) {
#ifdef	DEBUG
		    if (select_debug) {
			mutex_lock(&select_mutex);
			if (times) {
			    select_ztime++;
			} else {
			    select_zftime++;
			}
			mutex_unlock(&select_mutex);
		    }
#endif	DEBUG
		    break;
		}
	    }

#ifdef	DEBUG
	    if (select_debug && times && !uth->uu_sb) {
		mutex_lock(&select_mutex);
		select_none++;
		mutex_unlock(&select_mutex);
	    }
#endif	DEBUG

	    if (!uth->uu_sb) continue;

	    mutex_lock(&select_lock);
	    if (selbuf.s_state == SB_CLEAR) {
		/*
		 * Condition may have come true.  Clear the rest
		 * of the select items and try again.
		 */
		unselect(&selbuf);
		mutex_unlock(&select_lock);
#ifdef	DEBUG
		if (select_debug) {
		    mutex_lock(&select_mutex);
		    select_during++;
		    mutex_unlock(&select_mutex);
		}
#endif	DEBUG
		continue;
	    }

	    selbuf.s_state = SB_WAITING;
	    tsleep_enter((caddr_t)&selbuf,
			 PZERO+1,
			 (uap->tv) ? timeout_hzto : 0);
	    mutex_unlock(&select_lock);
	    result = tsleep_main((caddr_t)&selbuf,
				 PZERO+1,
				 (uap->tv) ? timeout_hzto : 0);
	    if (result == TS_SIG) {
		uth->u_error = EINTR;
#ifdef	DEBUG
		if (select_debug) {
		    mutex_lock(&select_mutex);
		    select_sig++;
		    mutex_unlock(&select_mutex);
		}
#endif	DEBUG
		break;
	    }
	    mutex_lock(&select_lock);
	    unselect(&selbuf);	/* clean up the rest of the conditions */
	    mutex_unlock(&select_lock);
	}
	if (uth->uu_sb) {
	    mutex_lock(&select_lock);
	    unselect(&selbuf);
	    mutex_unlock(&select_lock);
	}
done:
#define	putbits(name, x) \
	if (uap->name) { \
		bcopy((caddr_t)&obits[x], (caddr_t)uap->name, \
		    (unsigned)(sizeof(fd_set))); \
	}
	if (uth->u_error == 0) {
		putbits(in, 0);
		putbits(ou, 1);
		putbits(ex, 2);
#undef putbits
	} else {
	    uth->u_r.r_val1 = -1;
	}
}

/*
 * Unchain all active select events from this select
 * buffer.  Clear the wait condition.  This assumes select_lock
 * is held.
 */
unselect(sb)
	register struct selbuf *sb;
{
	register struct selitem *si;
	register struct selitem **sip;
	register int	i;
	int	s;

	/*
	 * Check each select item that is still
	 * active...
	 */
	for (i = 0, si = &sb->s_item[0];
	     i < sb->s_count;
	     i++, si++) {
	    if ((sip = si->si_dev) != 0) {
		/*
		 * Find the select event
		 * and unchain it.
		 */
		register struct selitem *sic;

		while ((sic = *sip) != 0) {
		    if (sic == si) {
			*sip = si->si_next;
			si->si_next = 0;
			break;
		    }
		    sip = &sic->si_next;
		}
		if (sic == 0)
		    panic("unselect: not on device list");
	    }
	    else {
		if (si->si_next != 0)
		    panic("unselect: selwakeup incomplete?");
	    }
	}
	sb->s_count = 0;

	/*
	 * Clear the select buffer and the wait condition.
	 */
	{
	    register int old_state;

	    old_state = sb->s_state;
	    sb->s_state = SB_CLEAR;
	    if (old_state == SB_WAITING)
		wakeup((caddr_t)sb);
	}
}

selscan(in, ou, ex, obits, nfd)
	fd_set *in, *ou, *ex, *obits;
{
	register int which, i, j;
	register fd_mask bits;
	register fd_set *cbits;
	int flag;
	struct file *fp;
	int n = 0;

	for (which = 0; which < 3; which++) {
		switch (which) {

		case 0:
			flag = FREAD;
			cbits = in;
			break;
		case 1:
			flag = FWRITE;
			cbits = ou;
			break;
		case 2:
			flag = 0;
			cbits = ex;
			break;
		}
		if (cbits != 0) {
		  for (i = 0; i < nfd; i += NFDBITS) {
			bits = cbits->fds_bits[i/NFDBITS];
			while ((j = ffs(bits)) && i + --j < nfd) {
				bits &= ~(1 << j);
				fp = u.u_ofile[i + j];
				if (fp == NULL) {
					u.u_error = EBADF;
					break;
				}
				if ((*fp->f_ops->fo_select)(fp, flag)) {
					FD_SET(i + j, &obits[which]);
					n++;
				}
			}
		  }
		}
	}
	return (n);
}

/*ARGSUSED*/
seltrue(dev, flag)
	dev_t dev;
	int flag;
{

	return (1);
}

/*
 * Add a select event to the current select buffer.
 * Chain all select buffers that are waiting for
 * the same event.
 */
selenter(hdr)
	caddr_t	hdr;
{
	register struct selitem **sip = (struct selitem **)hdr;
	register struct selbuf *sb = u.uu_sb;
	register struct selitem *si;

	if (sb) {
	    mutex_lock(&select_lock);

	    si = &sb->s_item[sb->s_count++];
	    si->si_selbuf = sb;
	    si->si_dev = sip;
	    si->si_next = *sip;
	    *sip = si;

	    mutex_unlock(&select_lock);
	}
}

/*
 * Wakeup all threads waiting on the same select event.
 * Do not clear the other select events that each thread
 * is waiting on; thread will do that itself.
 */
selwakeup(hdr)
	caddr_t	hdr;
{
	register struct selitem **sip = (struct selitem **)hdr;
	register struct selitem *si, *next;
	register struct selbuf *sb;
	register int old_state;

	mutex_lock(&select_lock);

	si = *sip;		/* get the select item chain */
	*sip = 0;		/* clear it out of device */

	while (si != 0) {
	    next = si->si_next;

	    si->si_next = 0;
	    si->si_dev = 0;	/* inactivate the select item */
	    sb = si->si_selbuf;
	    old_state = sb->s_state;
	    sb->s_state = SB_CLEAR;
	    if (old_state == SB_WAITING)
		wakeup((caddr_t)sb);

	    si = next;
	}

	mutex_unlock(&select_lock);
}
