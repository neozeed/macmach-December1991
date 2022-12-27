/* 
 * Mach Operating System
 * Copyright (c) 1988 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/* 
 * HISTORY
 * $Log:	kern_descrip.c,v $
 * Revision 2.5  91/03/20  14:59:57  dbg
 * 	Fix reference to user_request_it
 * 	[91/03/12            rwd]
 * 
 * Revision 2.4  90/11/05  15:33:04  rpd
 * 	Fix for not MAP_UAREA & MACH_NBC.
 * 	[90/11/01            rwd]
 * 
 * Revision 2.3  90/08/06  15:31:16  rwd
 * 	Init file_info share locks in share_dupit.
 * 	[90/07/12            rwd]
 * 
 * Revision 2.2  90/05/29  20:22:53  rwd
 * 	Convert mu_lock references to new lock routines.
 * 	[90/04/18            rwd]
 * 	Add override parameter to user_request_it.
 * 	[90/04/14            rwd]
 * 	Update file_info on dup.  Added close_file.
 * 	[90/03/28            rwd]
 * 
 * Revision 2.1  89/08/04  14:05:55  rwd
 * Created.
 * 
 * Revision 2.2  88/08/24  01:18:06  mwyoung
 * 	Corrected include file references.
 * 	[88/08/22  23:10:14  mwyoung]
 * 
 *
 *  7-Feb-87  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Merge VICE changes -- include vice.h and change to #if VICE.
 *
 * 28-Jan-87  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Added check for bogus file reference count in closef (to
 *	hopefully catch RFS bugs).
 *
 *  2-Dec-86  Jay Kistler (jjk) at Carnegie-Mellon University
 *	VICE:  added hooks for ITC/Andrew remote file system.
 *
 * 17-May-86  Mike Accetta (mja) at Carnegie-Mellon University
 *	CS_RFS:  Added explicit hook in flock() into rfs_finode()
 *	for a remote file.
 *	
 * 25-Jan-86  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Upgraded to 4.3.
 *
 * 18-Jul-85  Mike Accetta (mja) at Carnegie-Mellon University
 *	CS_RFS:  Added explicit hook in fstat() into remote system
 *	call handling.
 *	[V1(1)]
 *
 **********************************************************************
 */
 
#include <cmucs.h>
#include <cmucs_rfs.h>
#include <vice.h>
#include <mach_nbc.h>
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)kern_descrip.c	7.1 (Berkeley) 6/5/86
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/kernel.h>
#include <sys/inode.h>
#include <sys/proc.h>
#include <sys/file.h>
#include <sys/socket.h>
#include <sys/socketvar.h>
#include <sys/mount.h>
#include <sys/stat.h>

#include <sys/ioctl.h>

/*
 * Descriptor management.
 */

/*
 * TODO:
 *	eliminate u.u_error side effects
 */

/*
 * System calls on descriptors.
 */
getdtablesize()
{

	u.u_r.r_val1 = NOFILE;
}

getdopt()
{

}

setdopt()
{

}

dup()
{
	register struct a {
		int	i;
	} *uap = (struct a *) u.u_ap;
	struct file *fp;
	int j;

	if (uap->i &~ 077) { uap->i &= 077; dup2(); return; }	/* XXX */

	GETF(fp, uap->i);
	j = ufalloc(0);
	if (j < 0)
		return;
	dupit(j, fp, u.u_pofile[uap->i] &~ UF_EXCLOSE);
#if	MACH_NBC && MAP_UAREA
	share_dupit(fp, uap->i, j);
#endif	MACH_NBC && MAP_UAREA
}

dup2()
{
	register struct a {
		int	i, j;
	} *uap = (struct a *) u.u_ap;
	register struct file *fp;

	GETF(fp, uap->i);
	if (uap->j < 0 || uap->j >= NOFILE) {
		u.u_error = EBADF;
		return;
	}
	u.u_r.r_val1 = uap->j;
	if (uap->i == uap->j)
		return;
	if (u.u_ofile[uap->j]) {
		if (u.u_pofile[uap->j] & UF_MAPPED)
			munmapfd(uap->j);
		close_file(uap->j);
		if (u.u_error)
			return;
	}
	dupit(uap->j, fp, u.u_pofile[uap->i] &~ UF_EXCLOSE);
#if	MACH_NBC && MAP_UAREA
	share_dupit(fp, uap->i, uap->j);
#endif	MACH_NBC && MAP_UAREA
}

#if	MACH_NBC && MAP_UAREA
share_dupit(fp, i, j)
	register struct file *fp;
	register int i;
	register int j;
{
	register struct inode *ip = (struct inode *)fp->f_data;
	register struct file_info *fi = &u.u_shared_rw->us_file_info[i];
	register struct file_info *nfi = &u.u_shared_rw->us_file_info[j];

	if (fi->mapped) {
	    mu_lock(ip);
	    ip->count++;
	    mu_unlock(ip);
	    u.u_fmap[(u.u_findx[j]=u.u_findx[i])]++;
	    bcopy(fi, nfi, sizeof(struct file_info));
	    nfi->inuse = nfi->control = FALSE;
	    share_lock_init(&nfi->lock);
	}
}
#endif	MACH_NBC && MAP_UAREA
dupit(fd, fp, flags)
	int fd;
	register struct file *fp;
	register int flags;
{

	u.u_ofile[fd] = fp;
	u.u_pofile[fd] = flags;
	fp->f_count++;
	if (fd > u.u_lastfile)
		u.u_lastfile = fd;
}

/*
 * The file control system call.
 */
fcntl()
{
	register struct file *fp;
	register struct a {
		int	fdes;
		int	cmd;
		int	arg;
	} *uap;
	register i;
	register char *pop;

	uap = (struct a *)u.u_ap;
	GETF(fp, uap->fdes);
	pop = &u.u_pofile[uap->fdes];
	switch(uap->cmd) {
	case F_DUPFD:
		i = uap->arg;
		if (i < 0 || i >= NOFILE) {
			u.u_error = EINVAL;
			return;
		}
		if ((i = ufalloc(i)) < 0)
			return;
#if	MACH_NBC && MAP_UAREA
		share_dupit(fp, uap->fdes, i);
#endif	MACH_NBC && MAP_UAREA
		dupit(i, fp, *pop &~ UF_EXCLOSE);
		break;

	case F_GETFD:
		u.u_r.r_val1 = *pop & 1;
		break;

	case F_SETFD:
		*pop = (*pop &~ 1) | (uap->arg & 1);
		break;

	case F_GETFL:
		u.u_r.r_val1 = fp->f_flag+FOPEN;
		break;

	case F_SETFL:
		fp->f_flag &= FCNTLCANT;
		fp->f_flag |= (uap->arg-FOPEN) &~ FCNTLCANT;
#if	MAP_UAREA
		if (fp->f_flag & FAPPEND) u.u_shared_rw->
			us_file_info[uap->fdes].append = TRUE;
#endif	MAP_UAREA
		u.u_error = fset(fp, FNDELAY, fp->f_flag & FNDELAY);
		if (u.u_error)
			break;
		u.u_error = fset(fp, FASYNC, fp->f_flag & FASYNC);
		if (u.u_error)
			(void) fset(fp, FNDELAY, 0);
		break;

	case F_GETOWN:
		u.u_error = fgetown(fp, &u.u_r.r_val1);
		break;

	case F_SETOWN:
		u.u_error = fsetown(fp, uap->arg);
		break;

	default:
		u.u_error = EINVAL;
	}
}

fset(fp, bit, value)
	struct file *fp;
	int bit, value;
{

	if (value)
		fp->f_flag |= bit;
	else
		fp->f_flag &= ~bit;
	return (fioctl(fp, (int)(bit == FNDELAY ? FIONBIO : FIOASYNC),
	    (caddr_t)&value));
}

fgetown(fp, valuep)
	struct file *fp;
	int *valuep;
{
	int error;

	switch (fp->f_type) {

	case DTYPE_SOCKET:
		*valuep = ((struct socket *)fp->f_data)->so_pgrp;
		return (0);

	default:
		error = fioctl(fp, (int)TIOCGPGRP, (caddr_t)valuep);
		*valuep = -*valuep;
		return (error);
	}
}

fsetown(fp, value)
	struct file *fp;
	int value;
{

	if (fp->f_type == DTYPE_SOCKET) {
		((struct socket *)fp->f_data)->so_pgrp = value;
		return (0);
	}
	if (value > 0) {
		struct proc *p = pfind(value);
		if (p == 0)
			return (ESRCH);
		value = p->p_pgrp;
	} else
		value = -value;
	return (fioctl(fp, (int)TIOCSPGRP, (caddr_t)&value));
}

fioctl(fp, cmd, value)
	struct file *fp;
	int cmd;
	caddr_t value;
{

	return ((*fp->f_ops->fo_ioctl)(fp, cmd, value));
}

close()
{
	struct a {
		int	i;
	} *uap = (struct a *)u.u_ap;
	register int i = uap->i;
	register struct file *fp;
	register u_char *pf;

	GETF(fp, i);
	pf = (u_char *)&u.u_pofile[i];
	if (*pf & UF_MAPPED)
		munmapfd(i);
	close_file(uap->i);
	while (u.u_lastfile >= 0 && u.u_ofile[u.u_lastfile] == NULL)
		u.u_lastfile--;
	*pf = 0;
	/* WHAT IF u.u_error ? */
}

fstat()
{
	register struct file *fp;
	register struct a {
		int	fdes;
		struct	stat *sb;
	} *uap;
	struct stat ub;

	uap = (struct a *)u.u_ap;
	GETF(fp, uap->fdes);
	switch (fp->f_type) {

#if	CMUCS_RFS
	/*
	 *  The original code probably should have included fstat() in the file
	 *  ops structure in which case these explicit hooks could have been
	 *  avoided but ...
	 */
	case DTYPE_RFSINO:
		u.u_error = rfs_fstat(fp);
		return;
	case DTYPE_RFSCTL:
		u.u_error = rfsC_fstat(fp);
		return;
#endif	CMUCS_RFS
#if	VICE
	case DTYPE_VICEINO:
		u.u_error = rmt_fstat_fop(fp, uap->sb);
		return;
#endif	VICE
	case DTYPE_INODE:
		u.u_error = ino_stat((struct inode *)fp->f_data, &ub);
		break;

	case DTYPE_SOCKET:
		u.u_error = soo_stat((struct socket *)fp->f_data, &ub);
		break;

	default:
		panic("fstat");
		/*NOTREACHED*/
	}
	if (u.u_error == 0)
		u.u_error = copyout((caddr_t)&ub, (caddr_t)uap->sb,
		    sizeof (ub));
}

/*
 * Allocate a user file descriptor.
 */
ufalloc(i)
	register int i;
{

	for (; i < NOFILE; i++)
		if (u.u_ofile[i] == NULL) {
			u.u_r.r_val1 = i;
			u.u_pofile[i] = 0;
			if (i > u.u_lastfile)
				u.u_lastfile = i;
			return (i);
		}
	u.u_error = EMFILE;
	return (-1);
}

ufavail()
{
	register int i, avail = 0;

	for (i = 0; i < NOFILE; i++)
		if (u.u_ofile[i] == NULL)
			avail++;
	return (avail);
}

struct	file *lastf;
/*
 * Allocate a user file descriptor
 * and a file structure.
 * Initialize the descriptor
 * to point at the file structure.
 */
struct file *
falloc()
{
	register struct file *fp;
	register i;

	i = ufalloc(0);
	if (i < 0)
		return (NULL);
	if (lastf == 0)
		lastf = file;
	for (fp = lastf; fp < fileNFILE; fp++)
		if (fp->f_count == 0)
			goto slot;
	for (fp = file; fp < lastf; fp++)
		if (fp->f_count == 0)
			goto slot;
	tablefull("file");
	u.u_error = ENFILE;
	return (NULL);
slot:
	u.u_ofile[i] = fp;
	fp->f_count = 1;
	fp->f_data = 0;
	fp->f_offset = 0;
	lastf = fp + 1;
	return (fp);
}

/*
 * Convert a user supplied file descriptor into a pointer
 * to a file structure.  Only task is to check range of the descriptor.
 * Critical paths should use the GETF macro.
 */
struct file *
getf(f)
	register int f;
{
	register struct file *fp;

	if ((unsigned)f >= NOFILE || (fp = u.u_ofile[f]) == NULL) {
		u.u_error = EBADF;
		return (NULL);
	}
	return (fp);
}

/*
 * Internal form of close.
 * Decrement reference count on file structure.
 */
closef(fp)
	register struct file *fp;
{

	if (fp == NULL)
		return;
	if (fp->f_count > 1) {
		fp->f_count--;
		return;
	}
#if	CMUCS
	if (fp->f_count != 1) {
		/* panic("closef: f_count not 1"); */
		printf("closef: f_count not 1, f_count = %d\n",fp->f_count);
		return;
	}
#endif	CMUCS
	(*fp->f_ops->fo_close)(fp);
	fp->f_count = 0;
}

close_file(i)
int i;
{
	register struct file *fp = u.u_ofile[i];
	register struct inode *ip = (struct inode *)fp->f_data;
	u.u_ofile[i] = NULL;
	u.u_pofile[i] = 0;
#if	MACH_NBC && MAP_UAREA
	if (u.u_shared_rw->us_file_info[i].mapped) {
	    user_request_it(ip, i, 1, fp);
	    user_unmap_inode(ip);
	}
#endif	MACH_NBC && MAP_UAREA
	closef(fp);
}


/*
 * Apply an advisory lock on a file descriptor.
 */
flock()
{
	register struct a {
		int	fd;
		int	how;
	} *uap = (struct a *)u.u_ap;
	register struct file *fp;

	GETF(fp, uap->fd);
	if (fp->f_type != DTYPE_INODE) {
#if	CMUCS_RFS
		/*
		 *  Perform remote system call handling if necessary.
		 */
		if (fp->f_type == DTYPE_RFSINO)
		{
			extern struct file *rfs_finode();

			(void) rfs_finode(fp);
		}
		else
#endif	CMUCS_RFS
#if	VICE
		/*
		 *  Perform ITC/Andrew remote system call handling if necessary.
		 */
		if (fp->f_type == DTYPE_VICEINO)
		{
			extern struct file *rmt_fdes();

			(void) rmt_fdes(fp);
		}
		else
#endif	VICE
		u.u_error = EOPNOTSUPP;
		return;
	}
	if (uap->how & LOCK_UN) {
		ino_unlock(fp, FSHLOCK|FEXLOCK);
		return;
	}
	if ((uap->how & (LOCK_SH | LOCK_EX)) == 0)
		return;					/* error? */
	if (uap->how & LOCK_EX)
		uap->how &= ~LOCK_SH;
	/* avoid work... */
	if ((fp->f_flag & FEXLOCK) && (uap->how & LOCK_EX) ||
	    (fp->f_flag & FSHLOCK) && (uap->how & LOCK_SH))
		return;
	u.u_error = ino_lock(fp, uap->how);
}
