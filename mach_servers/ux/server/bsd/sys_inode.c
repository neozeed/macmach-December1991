/* 
 * Mach Operating System
 * Copyright (c) 1991 Carnegie-Mellon University
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/* 
 * HISTORY
 * $Log:	sys_inode.c,v $
 * Revision 2.7  91/03/20  15:01:47  dbg
 * 	Fixed IOCTLs for ANSI C.
 * 	[91/02/21            dbg]
 * 
 * Revision 2.6  90/06/19  23:08:45  rpd
 * 	Added missing ILOCK to rwip.
 * 	[90/06/05            rpd]
 * 
 * Revision 2.5  90/06/02  15:22:29  rpd
 * 	Removed include of uxkern/mfs_prim.c.
 * 	[90/06/01            rpd]
 * 
 * Revision 2.4  90/05/29  20:23:29  rwd
 * 	Put back forgotten ILOCK at end of server rwip call.
 * 	[90/05/12            rwd]
 * 	Remove non MACH code.
 * 	[90/05/01            rwd]
 * 
 * 	Allow server to read from mapped files also.
 * 	[90/05/01            rwd]
 * 	Remove ip->hack_address grossity.
 * 	[90/04/18            rwd]
 * 	Added MACH_NBC && MAP_UAREA code.
 * 	[90/03/19            rwd]
 * 
 * Revision 2.3  90/05/21  13:50:48  dbg
 * 	In rwip, for MACH_NBC: use unix_release and unix_master to
 * 	maintain the count on the master lock, so that copyout (called
 * 	from uiomove) can correctly release and restore the master lock
 * 	depending on its caller.
 * 	[90/03/22            dbg]
 * 
 * Revision 2.2  90/01/19  14:35:09  rwd
 * 	Moved include of sys/mfs.h
 * 	[90/01/16            rwd]
 * 	New version for NBC from rfr
 * 	[90/01/15            rwd]
 * 
 * 	Out-of-kernel version.
 * 	[89/01/05            dbg]
 * 
 * Revision 2.1  89/08/04  14:06:10  rwd
 * Created.
 * 
 * Revision 2.4  88/08/24  01:29:54  mwyoung
 * 	Corrected include file references.
 * 	[88/08/23  00:39:52  mwyoung]
 * 
 *
 * 09-Apr-88  Mike Accetta (mja) at Carnegie-Mellon University
 *	CMUCS: Moved terminal close down code from vhangup() into new
 *	ttydetach() routine which is called from here as well as from
 *	from ttylogout() and whenever the control side of a pseudo-
 *	terminal is closed down.
 *	[ V5.1(XF23) ]
 *
 * 26-Sep-87  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Minor bug fixes for NBC.
 *
 * 25-May-87  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Coalese NBC code into normal VM based code.
 *
 *  4-Jun-87  William Bolosky (bolosky) at Carnegie-Mellon University
 *	Eliminated pager_id's, and thus all dependance on MACH_XP and
 *	the include of mach_xp.h.
 *
 *  1-Jun-87  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Added include of "vm/inode_pager.h" to get inode_uncache.
 *
 *  7-Feb-87  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Merge VICE changes -- include vice.h and change to #if VICE.
 *
 *  3-Feb-87  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Use inode_uncache routine to properly maintain inode-object
 *	cache.
 *
 * 27-Jan-87  Mike Accetta (mja) at Carnegie-Mellon University
 *	Added FIOCNOSPC call to prohibit/allow disk space resource
 *	pausing on a per-file basis.
 *	[ V5.1(F1) ]
 *
 *  2-Dec-86  Jay Kistler (jjk) at Carnegie-Mellon University
 *	VICE:  Changed the test for open files when closing a
 *	device in ino_close to look at DTYPE_VICEINO inodes as
 *	well as DTYPE_INODE types.
 *
 * 19-Nov-86  Mike Accetta (mja) at Carnegie-Mellon University
 *	Added FIOCFSPARAM call to retrieve file system parameters of an
 *	open file.
 *	[ V5.1(F1) ]
 *
 *  9-Nov-86  Michael Young (mwyoung) at Carnegie-Mellon University
 *	De-linted some things.
 *
 * 31-May-86  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Use the new pager_id field of the inode to call
 *	vm_object_uncache.
 *
 *  1-May-86  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Remove inode from the object cache if it is truncated.
 *
 * 16-Feb-86  Bill Bolosky (bolosky) at Carnegie-Mellon University
 *	Added RT compatibility under switch romp.  Changes due to
 *	the fact that the RT character-special drivers take an inode
 *	pointer as an extra parameter.
 *
 * 25-Feb-86  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Installed VM changes.
 *
 * 25-Jan-86  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Upgraded to 4.3.
 *
 * 05-Aug-85  Mike Accetta (mja) at Carnegie-Mellon University
 *	CMUCS:  Fix bug in rdwri() so that it only sets error EIO
 *	on a residual count when no other error was indicated.
 *	[V1(1)]
 *	
 */
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)sys_inode.c	7.1 (Berkeley) 6/5/86
 */

#include <mach_nbc.h>
#include <map_uarea.h>
#include <vice.h>

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/inode.h>
#include <sys/proc.h>
#include <sys/fs.h>
#include <sys/conf.h>
#include <sys/buf.h>
#include <sys/mount.h>
#include <sys/file.h>
#include <sys/uio.h>
#include <sys/ioctl.h>
#include <sys/tty.h>
#include <sys/mfs.h>
#include <uxkern/import_mach.h>
#include <vm/inode_pager.h>
#if	MACH_NBC
#include <sys/parallel.h>
extern int nbc_debug;
#endif	MACH_NBC
#include <sys/stat.h>
#include <sys/kernel.h>

int	ino_rw(), ino_ioctl(), ino_select(), ino_close();
struct 	fileops inodeops =
	{ ino_rw, ino_ioctl, ino_select, ino_close };

ino_rw(fp, rw, uio)
	struct file *fp;
	enum uio_rw rw;
	struct uio *uio;
{
	register struct inode *ip = (struct inode *)fp->f_data;
	int error;

	if ((ip->i_mode&IFMT) == IFREG) {
		ILOCK(ip);
		if (fp->f_flag&FAPPEND && rw == UIO_WRITE)
			uio->uio_offset = fp->f_offset = mfs_inode_size(ip);
		error = rwip(ip, uio, rw);
		IUNLOCK(ip);
	} else
		error = rwip(ip, uio, rw);
	return (error);
}

rdwri(rw, ip, base, len, offset, segflg, aresid)
	struct inode *ip;
	caddr_t base;
	int len, segflg;
	off_t offset;
	int *aresid;
	enum uio_rw rw;
{
	struct uio auio;
	struct iovec aiov;
	int error;

	auio.uio_iov = &aiov;
	auio.uio_iovcnt = 1;
	aiov.iov_base = base;
	aiov.iov_len = len;
	auio.uio_resid = len;
	auio.uio_offset = offset;
	auio.uio_segflg = segflg;
	error = rwip(ip, &auio, rw);
	if (aresid)
		*aresid = auio.uio_resid;
	else
		if (auio.uio_resid)
		    if (error == 0)
			error = EIO;
	return (error);
}

rwip(ip, uio, rw)
	register struct inode *ip;
	register struct uio *uio;
	enum uio_rw rw;
{
	dev_t dev = (dev_t)ip->i_rdev;
	struct buf *bp;
	struct fs *fs;
	daddr_t lbn, bn;
	register int n, on, type;
	int size;
	long bsize;
	extern int mem_no;
	int error = 0;

	if (rw != UIO_READ && rw != UIO_WRITE)
		panic("rwip");
	if (rw == UIO_READ && uio->uio_resid == 0)
		return (0);
#if	0
	/* XXX Here's where it's a bad idea to have off_t be unsigned */
	if (uio->uio_offset < 0 &&
	    ((ip->i_mode&IFMT) != IFCHR || mem_no != major(dev)))
		return (EINVAL);
#endif	0
	if (rw == UIO_READ)
		ip->i_flag |= IACC;
	type = ip->i_mode&IFMT;
	if (type == IFCHR) {
		if (rw == UIO_READ)
			error = (*cdevsw[major(dev)].d_read)(dev, uio);
		else {
			ip->i_flag |= IUPD|ICHG;
			error = (*cdevsw[major(dev)].d_write)(dev, uio);
		}
		return (error);
	}
	if (uio->uio_resid == 0)
		return (0);
	if (rw == UIO_WRITE && type == IFREG &&
	    uio->uio_offset + uio->uio_resid >
	      u.u_rlimit[RLIMIT_FSIZE].rlim_cur) {
		psignal(u.u_procp, SIGXFSZ);
		return (EFBIG);
	}
	if (type != IFBLK) {
		dev = ip->i_dev;
		fs = ip->i_fs;
		bsize = fs->fs_bsize;
	} else
		bsize = BLKDEV_IOSIZE;
#if	MACH_NBC
#if	MAP_UAREA
	if (ip->mapped) {
		register vm_offset_t	va;

		if (nbc_debug & 4) {
		   uprintf("rwip(%c): ip 0x%x, id 0x%x, offset %d, size %d\n",
				rw == UIO_READ ? 'R' : 'W',
				ip,ip->i_id, uio->uio_offset, uio->uio_resid);
		}
		IUNLOCK(ip);
		mfs_get(ip);
		do {
			register struct file_info *fi = &ip->file_info;
			register struct map_info *mi = &fi->map_info;
			n = MIN((unsigned)bsize, uio->uio_resid);
			if (rw == UIO_READ) {
				int diff = mi->inode_size - uio->uio_offset;
				if (diff <= 0) {
					mfs_put(ip);
					ILOCK(ip);
					return (0);
				}
				if (diff < n)
					n = diff;
			}
			if ((rw == UIO_WRITE) &&
			   (uio->uio_offset + n > mi->inode_size) &&
			   (type == IFDIR || type == IFREG || type == IFLNK))
				mi->inode_size = uio->uio_offset + n;
			/*
			 *	Check to be sure we have a valid window
			 *	for the mapped file.
			 */
			if ((uio->uio_offset < mi->offset) ||
			   ((uio->uio_offset + n) > (mi->offset + mi->size))){
				user_remap_inode(ip, uio->uio_offset, n);
			}
			va = mi->address + uio->uio_offset - mi->offset;
			if (nbc_debug & 4) {
				uprintf("uiomove: va = 0x%x, n = %d.\n",
					va, n);
			}
			u.u_error = uiomove(va, n, rw, uio);
			if (rw == UIO_WRITE) {
				if (u.u_ruid != 0)
					ip->i_mode &= ~(ISUID|ISGID);
				/* put it on disk */
				fi->dirty = TRUE;
			}
		} while (u.u_error == 0 && uio->uio_resid > 0 && n != 0);
		mfs_put(ip);
		ILOCK(ip);
		return(u.u_error);
	}
#else	MAP_UAREA
	if (ip->mapped) {
		register vm_offset_t	va;

		if (nbc_debug & 4) {
		   uprintf("rwip(%c): ip 0x%x, id 0x%x, offset %d, size %d\n",
				rw == UIO_READ ? 'R' : 'W',
				ip,ip->i_id, uio->uio_offset, uio->uio_resid);
		}
		IUNLOCK(ip);
		mfs_get(ip, uio->uio_offset, uio->uio_resid);
		ILOCK(ip);
		do {
			n = MIN((unsigned)bsize, uio->uio_resid);
			if (rw == UIO_READ) {
				int diff = ip->inode_size - uio->uio_offset;
				if (diff <= 0) {
					mfs_put(ip);
					return (0);
				}
				if (diff < n)
					n = diff;
			}
			if ((rw == UIO_WRITE) &&
			   (uio->uio_offset + n > ip->inode_size) &&
			   (type == IFDIR || type == IFREG || type == IFLNK))
				ip->inode_size = uio->uio_offset + n;
			/*
			 *	Check to be sure we have a valid window
			 *	for the mapped file.
			 */
			IUNLOCK(ip);
			if ((uio->uio_offset < ip->offset) ||
			   ((uio->uio_offset + n) > (ip->offset + ip->size))){
				remap_inode(ip, uio->uio_offset, n);
			}
			va = ip->va + uio->uio_offset - ip->offset;
			if (nbc_debug & 4) {
				uprintf("uiomove: va = 0x%x, n = %d.\n",
					va, n);
			}
			unix_release();
			u.u_error = uiomove(va, n, rw, uio);
			unix_master();
			ILOCK(ip);
			if (rw == UIO_WRITE) {
				ip->i_flag |= IUPD|ICHG;
				if (u.u_ruid != 0)
					ip->i_mode &= ~(ISUID|ISGID);
				/* put it on disk */
				ip->dirty = TRUE;
			}
		} while (u.u_error == 0 && uio->uio_resid > 0 && n != 0);
		mfs_put(ip);
		return(u.u_error);
	}
#endif	MAP_UAREA
#endif	MACH_NBC
	do {
		lbn = uio->uio_offset / bsize;
		on = uio->uio_offset % bsize;
		n = MIN((unsigned)(bsize - on), uio->uio_resid);
		if (type != IFBLK) {
			if (rw == UIO_READ) {
				int diff = ip->i_size - uio->uio_offset;
				if (diff <= 0)
					return (0);
				if (diff < n)
					n = diff;
			}
			bn = fsbtodb(fs,
			    bmap(ip, lbn, rw == UIO_WRITE ? B_WRITE: B_READ, (int)(on+n)));
			if (u.u_error || rw == UIO_WRITE && (long)bn<0)
				return (u.u_error);
			if (rw == UIO_WRITE && uio->uio_offset + n > ip->i_size &&
			   (type == IFDIR || type == IFREG || type == IFLNK))
				ip->i_size = uio->uio_offset + n;
			size = blksize(fs, ip, lbn);
		} else {
			bn = lbn * (BLKDEV_IOSIZE/DEV_BSIZE);
			rablock = bn + (BLKDEV_IOSIZE/DEV_BSIZE);
			rasize = size = bsize;
		}
		if (rw == UIO_READ) {
			if ((long)bn<0) {
				bp = geteblk(size);
				clrbuf(bp);
			} else if (ip->i_lastr + 1 == lbn)
				bp = breada(dev, bn, size, rablock, rasize);
			else
				bp = bread(dev, bn, size);
			ip->i_lastr = lbn;
		} else {
			if (ip->pager != MEMORY_OBJECT_NULL)
				inode_uncache(ip);
			if (n == bsize) 
				bp = getblk(dev, bn, size);
			else
				bp = bread(dev, bn, size);
		}
		n = MIN(n, size - bp->b_resid);
		if (bp->b_flags & B_ERROR) {
			error = EIO;
			brelse(bp);
			goto bad;
		}
		u.u_error =
		    uiomove(bp->b_un.b_addr+on, n, rw, uio);
		if (rw == UIO_READ) {
			if (n + on == bsize || uio->uio_offset == ip->i_size)
				bp->b_flags |= B_AGE;
			brelse(bp);
		} else {
			if ((ip->i_mode&IFMT) == IFDIR)
				bwrite(bp);
			else if (n + on == bsize) {
				bp->b_flags |= B_AGE;
				bawrite(bp);
			} else
				bdwrite(bp);
			ip->i_flag |= IUPD|ICHG;
			if (u.u_ruid != 0)
				ip->i_mode &= ~(ISUID|ISGID);
		}
	} while (u.u_error == 0 && uio->uio_resid > 0 && n != 0);
	if (error == 0)				/* XXX */
		error = u.u_error;		/* XXX */
bad:
	return (error);
}

ino_ioctl(fp, com, data)
	struct file *fp;
	register int com;
	caddr_t data;
{
	register struct inode *ip = ((struct inode *)fp->f_data);
	register int fmt = ip->i_mode & IFMT;
	dev_t dev;

	if (com == _IOW('f', 100, int))	/* FIOCXMOD */
		return(0);
	if (com == FIOCFSPARAM)
	{
		struct fs *fs = ip->i_fs;
		struct fsparam *fsp = (struct fsparam *)data;
		int i;

		fsp->fsp_free = freefrags(fs);
		fsp->fsp_size = fs->fs_dsize;
		if (fs->fs_fsize > 1024)
		{
			i = (fs->fs_fsize/1024);
			fsp->fsp_free /= i;
			fsp->fsp_size /= i;
		}
		else if (fs->fs_fsize < 1024)
		{
			i = (1024/fs->fs_fsize);
			fsp->fsp_free *= i;
			fsp->fsp_size *= i;
		}
		fsp->fsp_ifree   = freeinodes(fs);
		fsp->fsp_isize   = fs->fs_ncg*fs->fs_ipg;
		fsp->fsp_minfree = fs->fs_minfree;
		bzero((caddr_t)fsp->fsp_unused, sizeof(fsp->fsp_unused));
		return(0);
	}
	switch (fmt) {

	case IFREG:
		if (com == FIOCNOSPC)
		{
		    int i = (fp->f_flag&FNOSPC);

		    switch (*(int *)data)
		    {
			case FIOCNOSPC_ERROR:
			    fp->f_flag |=  FNOSPC;
			    break;
			case FIOCNOSPC_PAUSE:
			    fp->f_flag &= ~FNOSPC;
			    break;
			case FIOCNOSPC_SAME:
			    break;
			default:
			    return(EINVAL);
		    }
		    *(int *)data = i?FIOCNOSPC_ERROR:FIOCNOSPC_PAUSE;
		    return(0);
		}
	case IFDIR:
		if (com == FIONREAD) {
			*(off_t *)data = ip->i_size - fp->f_offset;
			return (0);
		}
		if (com == FIONBIO || com == FIOASYNC)	/* XXX */
			return (0);			/* XXX */
		/* fall into ... */

	default:
		return (ENOTTY);

	case IFCHR:
		dev = ip->i_rdev;
		u.u_r.r_val1 = 0;
		if (setjmp(&u.u_qsave)) {
			if ((u.u_sigintr & sigmask(u.u_procp->p_cursig)) != 0)
				return(EINTR);
			u.u_eosys = RESTARTSYS;
			return (0);
		}
		return ((*cdevsw[major(dev)].d_ioctl)(dev, com, data,
		    fp->f_flag));
	}
}

ino_select(fp, which)
	struct file *fp;
	int which;
{
	register struct inode *ip = (struct inode *)fp->f_data;
	register dev_t dev;

	switch (ip->i_mode & IFMT) {

	default:
		return (1);		/* XXX */

	case IFCHR:
		dev = ip->i_rdev;
		return (*cdevsw[major(dev)].d_select)(dev, which);
	}
}

#ifdef notdef
ino_clone()
{

	return (EOPNOTSUPP);
}
#endif

ino_stat(ip, sb)
	register struct inode *ip;
	register struct stat *sb;
{
	{
	    struct timeval time;
	    get_time(&time);
	    ITIMES(ip, &time, &time);
	}

	/*
	 * Copy from inode table
	 */
	sb->st_dev = ip->i_dev;
	sb->st_ino = ip->i_number;
	sb->st_mode = ip->i_mode;
	sb->st_nlink = ip->i_nlink;
	sb->st_uid = ip->i_uid;
	sb->st_gid = ip->i_gid;
	sb->st_rdev = (dev_t)ip->i_rdev;
	sb->st_size = mfs_inode_size(ip);
	sb->st_atime = ip->i_atime;
	sb->st_spare1 = 0;
	sb->st_mtime = ip->i_mtime;
	sb->st_spare2 = 0;
	sb->st_ctime = ip->i_ctime;
	sb->st_spare3 = 0;
	/* this doesn't belong here */
	if ((ip->i_mode&IFMT) == IFBLK)
		sb->st_blksize = BLKDEV_IOSIZE;
	else if ((ip->i_mode&IFMT) == IFCHR)
		sb->st_blksize = MAXBSIZE;
	else
		sb->st_blksize = ip->i_fs->fs_bsize;
	sb->st_blocks = ip->i_blocks;
	sb->st_spare4[0] = sb->st_spare4[1] = 0;
	return (0);
}

ino_close(fp)
	register struct file *fp;
{
	register struct inode *ip = (struct inode *)fp->f_data;
	register struct mount *mp;
	int flag, mode;
	dev_t dev;
	register int (*cfunc)();

	if (fp->f_flag & (FSHLOCK|FEXLOCK))
		ino_unlock(fp, FSHLOCK|FEXLOCK);
	flag = fp->f_flag;
	dev = (dev_t)ip->i_rdev;
	mode = ip->i_mode & IFMT;
#if	MACH_NBC
	if (mode == IFREG) {
#if	MAP_UAREA
	    if (!u.u_shared_rw->us_map_files)
#endif	MAP_UAREA
		unmap_inode(ip);
	}
#endif	MACH_NBC
	ilock(ip);
	iput(ip);
	fp->f_data = (caddr_t) 0;		/* XXX */
	switch (mode) {

	case IFCHR:
		cfunc = cdevsw[major(dev)].d_close;
		break;

	case IFBLK:
		/*
		 * We don't want to really close the device if it is mounted
		 */
/* MOUNT TABLE SHOULD HOLD INODE */
		for (mp = mount; mp < &mount[NMOUNT]; mp++)
			if (mp->m_bufp != NULL && mp->m_dev == dev)
				return;
		cfunc = bdevsw[major(dev)].d_close;
		break;

	default:
		return;
	}

	/*
	 * Check that another inode for the same device isn't active.
	 * This is because the same device can be referenced by
	 * two different inodes.
	 */
	for (fp = file; fp < fileNFILE; fp++) {
#if	VICE
		if (fp->f_type != DTYPE_INODE && fp->f_type != DTYPE_VICEINO)
#else	VICE
		if (fp->f_type != DTYPE_INODE)		/* XXX */
#endif	VICE
			continue;
		if (fp->f_count && (ip = (struct inode *)fp->f_data) &&
		    ip->i_rdev == dev && (ip->i_mode&IFMT) == mode)
			return;
	}
	if (mode == IFBLK) {
		/*
		 * On last close of a block device (that isn't mounted)
		 * we must invalidate any in core blocks, so that
		 * we can, for instance, change floppy disks.
		 */
		bflush(dev);
		binval(dev);
	}
	if (setjmp(&u.u_qsave)) {
		/*
		 * If device close routine is interrupted,
		 * must return so closef can clean up.
		 */
		if (u.u_error == 0)
			u.u_error = EINTR;	/* ??? */
		return;
	}
	(*cfunc)(dev, flag);
}

/*
 * Place an advisory lock on an inode.
 */
ino_lock(fp, cmd)
	register struct file *fp;
	int cmd;
{
	register int priority = PLOCK;
	register struct inode *ip = (struct inode *)fp->f_data;

	if ((cmd & LOCK_EX) == 0)
		priority += 4;
	if (setjmp(&u.u_qsave)) {
		if ((u.u_sigintr & sigmask(u.u_procp->p_cursig)) != 0)
			return(EINTR);
		u.u_eosys = RESTARTSYS;
		return (0);
	}
	/*
	 * If there's a exclusive lock currently applied
	 * to the file, then we've gotta wait for the
	 * lock with everyone else.
	 */
again:
	while (ip->i_flag & IEXLOCK) {
		/*
		 * If we're holding an exclusive
		 * lock, then release it.
		 */
		if (fp->f_flag & FEXLOCK) {
			ino_unlock(fp, FEXLOCK);
			continue;
		}
		if (cmd & LOCK_NB)
			return (EWOULDBLOCK);
		ip->i_flag |= ILWAIT;
		sleep((caddr_t)&ip->i_exlockc, priority);
	}
	if ((cmd & LOCK_EX) && (ip->i_flag & ISHLOCK)) {
		/*
		 * Must wait for any shared locks to finish
		 * before we try to apply a exclusive lock.
		 *
		 * If we're holding a shared
		 * lock, then release it.
		 */
		if (fp->f_flag & FSHLOCK) {
			ino_unlock(fp, FSHLOCK);
			goto again;
		}
		if (cmd & LOCK_NB)
			return (EWOULDBLOCK);
		ip->i_flag |= ILWAIT;
		sleep((caddr_t)&ip->i_shlockc, PLOCK);
		goto again;
	}
	if (fp->f_flag & FEXLOCK)
		panic("ino_lock");
	if (cmd & LOCK_EX) {
		cmd &= ~LOCK_SH;
		ip->i_exlockc++;
		ip->i_flag |= IEXLOCK;
		fp->f_flag |= FEXLOCK;
	}
	if ((cmd & LOCK_SH) && (fp->f_flag & FSHLOCK) == 0) {
		ip->i_shlockc++;
		ip->i_flag |= ISHLOCK;
		fp->f_flag |= FSHLOCK;
	}
	return (0);
}

/*
 * Unlock a file.
 */
ino_unlock(fp, kind)
	register struct file *fp;
	int kind;
{
	register struct inode *ip = (struct inode *)fp->f_data;
	int flags;

	kind &= fp->f_flag;
	if (ip == NULL || kind == 0)
		return;
	flags = ip->i_flag;
	if (kind & FSHLOCK) {
		if ((flags & ISHLOCK) == 0)
			panic("ino_unlock: SHLOCK");
		if (--ip->i_shlockc == 0) {
			ip->i_flag &= ~ISHLOCK;
			if (flags & ILWAIT)
				wakeup((caddr_t)&ip->i_shlockc);
		}
		fp->f_flag &= ~FSHLOCK;
	}
	if (kind & FEXLOCK) {
		if ((flags & IEXLOCK) == 0)
			panic("ino_unlock: EXLOCK");
		if (--ip->i_exlockc == 0) {
			ip->i_flag &= ~(IEXLOCK|ILWAIT);
			if (flags & ILWAIT)
				wakeup((caddr_t)&ip->i_exlockc);
		}
		fp->f_flag &= ~FEXLOCK;
	}
}

/*
 * Openi called to allow handler
 * of special files to initialize and
 * validate before actual IO.
 */
#if	MAP_UAREA
openi(ip, mode, indx)
#else	MAP_UAREA
openi(ip, mode)
#endif	MAP_UAREA
	register struct inode *ip;
{
	dev_t dev = (dev_t)ip->i_rdev;
	register int maj = major(dev);

	switch (ip->i_mode&IFMT) {

	case IFCHR:
		if ((u_int)maj >= nchrdev)
			return (ENXIO);
		return ((*cdevsw[maj].d_open)(dev, mode));

	case IFBLK:
		if ((u_int)maj >= nblkdev)
			return (ENXIO);
		return ((*bdevsw[maj].d_open)(dev, mode));
#if	MACH_NBC
	case IFREG:
#if	MAP_UAREA
	    if (u.u_shared_rw->us_map_files) {
		user_map_inode(ip, mode, indx);
	    } else {
#endif	MAP_UAREA
		map_inode(ip);
#if	MAP_UAREA
	    }
#endif	MAP_UAREA
#endif	MACH_NBC
	}
	return (0);
}

/*
 * Revoke access the current tty by all processes.
 * Used only by the super-user in init
 * to give ``clean'' terminals at login.
 */
vhangup()
{

	if (!suser())
		return;
	if (u.u_ttyp == NULL)
		return;
	ttydetach(u.u_ttyd, u.u_ttyp);
}

forceclose(dev)
	dev_t dev;
{
	register struct file *fp;
	register struct inode *ip;

	for (fp = file; fp < fileNFILE; fp++) {
		if (fp->f_count == 0)
			continue;
		if (fp->f_type != DTYPE_INODE)
			continue;
		ip = (struct inode *)fp->f_data;
		if (ip == 0)
			continue;
		if ((ip->i_mode & IFMT) != IFCHR)
			continue;
		if (ip->i_rdev != dev)
			continue;
		fp->f_flag &= ~(FREAD|FWRITE);
	}
}
