/* 
 * Mach Operating System
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	ufs_inode.c,v $
 * Revision 2.8  90/08/06  15:32:28  rwd
 * 	Turn itrunc block zeroing back on for MAP_UAREA NBC
 * 	implimentation.
 * 	[90/08/03            rwd]
 * 
 * Revision 2.7  90/06/19  23:09:44  rpd
 * 	Purged MACH, MACH_NO_KERNEL, MACH_XP, CMUCS conditionals.
 * 	Made itrunc always use mfs_trunc, never user_trunc.
 * 	Removed special handling in itrunc for zero i_size/length case.
 * 	[90/06/12            rpd]
 * 
 * Revision 2.6  90/06/02  15:22:36  rpd
 * 	Purged inode_pager_active.
 * 	[90/03/28            rpd]
 * 
 * Revision 2.5  90/05/29  20:23:41  rwd
 * 	Added rfr's inode writeback changes.
 * 	[90/04/12            rwd]
 * 
 * 	Added user_trunc call for MACH_NBC and MAP_UAREA.
 * 	[90/03/27            rwd]
 * 
 * Revision 2.4  90/01/23  00:04:01  af
 * 	Applied gm0w's correction to iflush fix.
 * 	[90/01/20  23:14:13  af]
 * 
 * Revision 2.3  90/01/19  14:35:16  rwd
 * 	Get new NBC version from rfr
 * 	[90/01/15            rwd]
 * 
 * Revision 2.2  89/11/29  15:27:59  af
 * 	From mainline: in iflush() clear the references that the inode
 * 	pager keeps for cached files (by calling inode_uncache_try).
 * 	[89/11/27            af]
 * 
 * 	Out-of-kernel version.  Changed free() to free_block() to
 * 	avoid conflict with C library.
 * 	[89/01/05            dbg]
 * 
 * Revision 2.1  89/08/04  14:14:53  rwd
 * Created.
 * 
 * Revision 2.4  88/07/17  17:28:06  mwyoung
 * Use new memory object types.
 * 
 * Revision 2.3.1.1  88/07/04  15:00:46  mwyoung
 * See below.
 * 
 *
 * 21-Apr-88  Mike Accetta (mja) at Carnegie-Mellon University
 *	Installed bug fix from Jay Kistler to avoid problems when the
 *	iget() caller is not expecting to transfer into VICE (e.g. from
 *	the bind() call).
 *	[ V5.1(XF23) ]
 *
 * 17-Mar-88  Joseph Boykin (boykin) at Encore Computer Corporation
 *	Fixed a bug in itrunc for fast symbolic links.  The inode size
 *	was not set to zero; fsck tended to complain.
 *
 * 17-Feb-88  Joseph Boykin (boykin) at Encore Computer Corporation
 *	Fixed a bug in fast symbolic links -- attempts to truncate the
 *	the file would result in errors and/or panics.  Put code
 *	in 'itrunc' to not attempt to truncate such inodes.  Removed
 *	test from 'irele' since it is no longer necessary.
 *	(MULTIMAX code only).
 *
 * 14-Dec-87  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Changed how irele() checks for active use by the inode pager.
 *	Reduced history.  Participants so far: avie, mwyoung, dbg,
 *	bolosky, jjk, dlb, mja.
 *
 *  2-Dec-86  Jay Kistler (jjk) at Carnegie-Mellon University
 *	VICE:  added hook for ITC/Andrew remote file system--identifies
 *	entry points into rfs by checking a mount entry's m_remote field.
 *
 * 01-Mar-86  Mike Accetta (mja) at Carnegie-Mellon University
 *	CMUCS: suppress inode time updates if "iupdnot"
 *	variable is non-zero (TEMP).
 *
 * 26-Aug-85  Mike Accetta (mja) at Carnegie-Mellon University
 *	CMUCS:  Added inode count consistency check macros.
 *	[V1(1)].
 *
 **********************************************************************
 */
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)ufs_inode.c	7.1 (Berkeley) 6/5/86
 */
 
#include <mach_nbc.h>
#include <mach_xp.h>
#include <quota.h>
#include <vice.h>

#include <uxkern/import_mach.h>
#include <vm/inode_pager.h>
#include <sys/param.h>
#include <sys/systm.h>
#include <sys/mount.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/inode.h>
#include <sys/fs.h>
#include <sys/buf.h>
#if	MACH_NBC
#include <sys/mfs.h>
#endif	MACH_NBC
#if	QUOTA
#include <sys/quota.h>
#endif	QUOTA
#include <sys/kernel.h>

#define	INOHSZ	512
#if	((INOHSZ&(INOHSZ-1)) == 0)
#define	INOHASH(dev,ino)	(((dev)+(ino))&(INOHSZ-1))
#else
#define	INOHASH(dev,ino)	(((unsigned)((dev)+(ino)))%INOHSZ)
#endif

union ihead {				/* inode LRU cache, Chris Maltby */
	union  ihead *ih_head[2];
	struct inode *ih_chain[2];
} ihead[INOHSZ];

struct inode *ifreeh, **ifreet;

/* 
 *  Define these in one place to avoid string constant replication.
 */
char *PANICMSG_IINCR = "iincr";
char *PANICMSG_IDECR = "idecr";

/*
 * Initialize hash links for inodes
 * and build inode free list.
 */
ihinit()
{
	register int i;
	register struct inode *ip = inode;
	register union  ihead *ih = ihead;

	for (i = INOHSZ; --i >= 0; ih++) {
		ih->ih_head[0] = ih;
		ih->ih_head[1] = ih;
	}
	ifreeh = ip;
	ifreet = &ip->i_freef;
	ip->i_freeb = &ifreeh;
	ip->i_forw = ip;
	ip->i_back = ip;
	for (i = ninode; --i > 0; ) {
		++ip;
		ip->i_forw = ip;
		ip->i_back = ip;
		*ifreet = ip;
		ip->i_freeb = ifreet;
		ifreet = &ip->i_freef;
	}
	ip->i_freef = NULL;
}

#ifdef notdef
/*
 * Find an inode if it is incore.
 * This is the equivalent, for inodes,
 * of ``incore'' in bio.c or ``pfind'' in subr.c.
 */
struct inode *
ifind(dev, ino)
	dev_t dev;
	ino_t ino;
{
	register struct inode *ip;
	register union  ihead *ih;

	ih = &ihead[INOHASH(dev, ino)];
	for (ip = ih->ih_chain[0]; ip != (struct inode *)ih; ip = ip->i_forw)
		if (ino==ip->i_number && dev==ip->i_dev)
			return (ip);
	return ((struct inode *)0);
}
#endif notdef

/*
 * Look up an inode by device,inumber.
 * If it is in core (in the inode structure),
 * honor the locking protocol.
 * If it is not in core, read it in from the
 * specified device.
 * If the inode is mounted on, perform
 * the indicated indirection.
 * In all cases, a pointer to a locked
 * inode structure is returned.
 *
 * panic: no imt -- if the mounted file
 *	system is not in the mount table.
 *	"cannot happen"
 */
struct inode *
iget(dev, fs, ino)
	dev_t dev;
	register struct fs *fs;
	ino_t ino;
{
	register struct inode *ip;
	register union  ihead *ih;
	register struct mount *mp;
	register struct buf *bp;
	register struct dinode *dp;
	register struct inode *iq;

loop:
	ih = &ihead[INOHASH(dev, ino)];
	for (ip = ih->ih_chain[0]; ip != (struct inode *)ih; ip = ip->i_forw)
		if (ino == ip->i_number && dev == ip->i_dev) {
			/*
			 * Following is essentially an inline expanded
			 * copy of igrab(), expanded inline for speed,
			 * and so that the test for a mounted on inode
			 * can be deferred until after we are sure that
			 * the inode isn't busy.
			 */
			if ((ip->i_flag&ILOCKED) != 0) {
				ip->i_flag |= IWANT;
				sleep((caddr_t)ip, PINOD);
				goto loop;
			}
			if ((ip->i_flag&IMOUNT) != 0) {
				for (mp = &mount[0]; mp < &mount[NMOUNT]; mp++)
					if(mp->m_inodp == ip) {
#if	VICE			
					    if (mp->m_remote) {
						    u.u_rmt_dev = mp->m_dev;
						    if (!u.u_rmt_requested)
							u.u_error = EINVAL;
						    return NULL;
					    } else {
#endif	VICE
						dev = mp->m_dev;
						fs = mp->m_bufp->b_un.b_fs;
						ino = ROOTINO;
						goto loop;
#if	VICE
					    }
#endif	VICE
					}
				panic("no imt");
			}
			if (ip->i_count == 0) {		/* ino on free list */
				if (iq = ip->i_freef)
					iq->i_freeb = ip->i_freeb;
				else
					ifreet = ip->i_freeb;
				*ip->i_freeb = iq;
				ip->i_freef = NULL;
				ip->i_freeb = NULL;
#if	VICE
				ip->i_rmt_dev = NODEV;
#endif	VICE
				ip->pager = MEMORY_OBJECT_NULL;
			}
			iincr_chk(ip);
			ip->i_flag |= ILOCKED;
			return(ip);
		}

	if ((ip = ifreeh) == NULL) {
		tablefull("inode");
		u.u_error = ENFILE;
		return(NULL);
	}
	if (ip->i_count)
		panic("free inode isn't");
	if (iq = ip->i_freef)
		iq->i_freeb = &ifreeh;
	ifreeh = iq;
	ip->i_freef = NULL;
	ip->i_freeb = NULL;
#if	MACH_NBC
	/*
	 *	Flush the inode from the file map cache.
	 */
	mfs_uncache(ip);
#endif	MACH_NBC
	/*
	 * Now to take inode off the hash chain it was on
	 * (initially, or after an iflush, it is on a "hash chain"
	 * consisting entirely of itself, and pointed to by no-one,
	 * but that doesn't matter), and put it on the chain for
	 * its new (ino, dev) pair
	 */
	remque(ip);
	insque(ip, ih);
	ip->i_dev = dev;
	ip->i_fs = fs;
	ip->i_number = ino;
	cacheinval(ip);
	ip->i_flag = ILOCKED;
	iincr_chk(ip);
	ip->i_lastr = 0;
#if	VICE
	/* ? Do we still need this???? */
	ip->i_rmt_dev = NODEV;
#endif	VICE
#if	QUOTA
	dqrele(ip->i_dquot);
#endif	QUOTA
	bp = bread(dev, fsbtodb(fs, itod(fs, ino)), (int)fs->fs_bsize);
	/*
	 * Check I/O errors
	 */
	if ((bp->b_flags&B_ERROR) != 0) {
		brelse(bp);
		/*
		 * the inode doesn't contain anything useful, so it would
		 * be misleading to leave it on its hash chain.
		 * 'iput' will take care of putting it back on the free list.
		 */
		remque(ip);
		ip->i_forw = ip;
		ip->i_back = ip;
		/*
		 * we also loose its inumber, just in case (as iput
		 * doesn't do that any more) - but as it isn't on its
		 * hash chain, I doubt if this is really necessary .. kre
		 * (probably the two methods are interchangable)
		 */
		ip->i_number = 0;
#if	QUOTA
		ip->i_dquot = NODQUOT;
#endif	QUOTA
		iput(ip);
		return(NULL);
	}
	dp = bp->b_un.b_dino;
	dp += itoo(fs, ino);
	ip->i_ic = dp->di_ic;
	brelse(bp);
#if	QUOTA
	if (ip->i_mode == 0)
		ip->i_dquot = NODQUOT;
	else
		ip->i_dquot = inoquota(ip);
#endif	QUOTA
#if	MACH_NBC
	ip->inode_size = ip->i_size;
#endif	MACH_NBC
	ip->pager = MEMORY_OBJECT_NULL;
	return (ip);
}

/*
 * Convert a pointer to an inode into a reference to an inode.
 *
 * This is basically the internal piece of iget (after the
 * inode pointer is located) but without the test for mounted
 * filesystems.  It is caller's responsibility to check that
 * the inode pointer is valid.
 */
igrab(ip)
	register struct inode *ip;
{
	while ((ip->i_flag&ILOCKED) != 0) {
		ip->i_flag |= IWANT;
		sleep((caddr_t)ip, PINOD);
	}
	if (ip->i_count == 0) {		/* ino on free list */
		register struct inode *iq;

		if (iq = ip->i_freef)
			iq->i_freeb = ip->i_freeb;
		else
			ifreet = ip->i_freeb;
		*ip->i_freeb = iq;
		ip->i_freef = NULL;
		ip->i_freeb = NULL;
	}
	ip->i_count++;
	ip->i_flag |= ILOCKED;
}

/*
 * Decrement reference count of
 * an inode structure.
 * On the last reference,
 * write the inode out and if necessary,
 * truncate and deallocate the file.
 */
iput(ip)
	register struct inode *ip;
{

	if ((ip->i_flag & ILOCKED) == 0)
		panic("iput");
	IUNLOCK(ip);
	irele(ip);
}

irele(ip)
	register struct inode *ip;
{
	int mode;

	if (ip->i_count == 1) {
		ip->i_flag |= ILOCKED;
		if (ip->i_nlink <= 0 && ip->i_fs->fs_ronly == 0) {
			itrunc(ip, (u_long)0);
			mode = ip->i_mode;
			ip->i_mode = 0;
			ip->i_rdev = 0;
			ip->i_flag |= IUPD|ICHG;
			ifree(ip, ip->i_number, mode);
#if	QUOTA
			(void) chkiq(ip->i_dev, ip, ip->i_uid, 0);
			dqrele(ip->i_dquot);
			ip->i_dquot = NODQUOT;
#endif	QUOTA
		}
		{
		    struct timeval time;
		    get_time(&time);
		    IUPDAT(ip, &time, &time, 0);
		}
		IUNLOCK(ip);
		ip->i_flag = 0;
		/*
		 * Put the inode on the end of the free list.
		 * Possibly in some cases it would be better to
		 * put the inode at the head of the free list,
		 * (eg: where i_mode == 0 || i_number == 0)
		 * but I will think about that later .. kre
		 * (i_number is rarely 0 - only after an i/o error in iget,
		 * where i_mode == 0, the inode will probably be wanted
		 * again soon for an ialloc, so possibly we should keep it)
		 */
		if (ifreeh) {
			*ifreet = ip;
			ip->i_freeb = ifreet;
		} else {
			ifreeh = ip;
			ip->i_freeb = &ifreeh;
		}
		ip->i_freef = NULL;
		ifreet = &ip->i_freef;
	} else if (!(ip->i_flag & ILOCKED))
		{
		    struct timeval time;
		    get_time(&time);
		    ITIMES(ip, &time, &time);
		}
	idecr_chk(ip);
}

#if	VICE
/*
 * Really horrible fudge to allow us to drop an inode we got with iget but
 * which isn't really allocated.
 */
iforget(ip)
struct inode *ip;
{
	ip->i_count = 0;
	ip->i_flag = 0;
	if (ifreeh) {
		*ifreet = ip;
		ip->i_freeb = ifreet;
	} else {
		ifreeh = ip;
		ip->i_freeb = &ifreeh;
	}
	ip->i_freef = NULL;
	ifreet = &ip->i_freef;
}
#endif	VICE

int iupdnot = 0;	/* TEMP: suppress time updates if true */

/*
 * Check accessed and update flags on
 * an inode structure.
 * If any is on, update the inode
 * with the current time.
 * If waitfor is given, then must insure
 * i/o order so wait for write to complete.
 */
iupdat(ip, ta, tm, waitfor)
	register struct inode *ip;
	struct timeval *ta, *tm;
	int waitfor;
{
	register struct buf *bp;
	struct dinode *dp;
	register struct fs *fp;

	fp = ip->i_fs;
	if ((ip->i_flag & (IUPD|IACC|ICHG|IMOD)) != 0) {
		if (fp->fs_ronly)
			return;
		bp = bread(ip->i_dev, fsbtodb(fp, itod(fp, ip->i_number)),
			(int)fp->fs_bsize);
		if (bp->b_flags & B_ERROR) {
			brelse(bp);
			return;
		}
		if (iupdnot)
			ip->i_flag &= ~(ICHG);
		if (ip->i_flag&IACC)
			ip->i_atime = ta->tv_sec;
		if (ip->i_flag&IUPD)
			ip->i_mtime = tm->tv_sec;
		if (ip->i_flag&ICHG)
			{
			    struct timeval time;
			    get_time(&time);
			    ip->i_ctime = time.tv_sec;
			}
		ip->i_flag &= ~(IUPD|IACC|ICHG|IMOD);
		dp = bp->b_un.b_dino + itoo(fp, ip->i_number);
		dp->di_ic = ip->i_ic;
		if (waitfor == 2)
			baforce(bp);
		else if (waitfor == 1)
			bwrite(bp);
		else
			bdwrite(bp);
	}
}

#define	SINGLE	0	/* index of single indirect block */
#define	DOUBLE	1	/* index of double indirect block */
#define	TRIPLE	2	/* index of triple indirect block */
/*
 * Truncate the inode ip to at most
 * length size.  Free affected disk
 * blocks -- the blocks of the file
 * are removed in reverse order.
 *
 * NB: triple indirect blocks are untested.
 */
itrunc(oip, length)
	register struct inode *oip;
	u_long length;
{
	register daddr_t lastblock;
	daddr_t bn, lbn, lastiblock[NIADDR];
	register struct fs *fs;
	register struct inode *ip;
	struct buf *bp;
	u_long osize, size;
	int offset, level;
	long nblocks, blocksreleased = 0;
	register int i;
	dev_t dev;
	struct inode tip;
	extern long indirtrunc();

#ifdef	multimax
	/*
	 * Don't try to truncate fast symbolic link inodes since they
	 *  don't have any real storage.
	 */
	if((oip->i_mode & IFMT) == IFLNK && (oip->i_icflags & IC_FASTLINK)) {
		for(i=NDADDR+NIADDR-1; i >= 0; i--)
			oip->i_db[i] = 0;
		oip->i_icflags = 0;
		oip->i_size = 0;
		oip->i_flag |= ICHG|IUPD;
		{
		    struct timeval time;
		    get_time(&time);
		    iupdat(oip, &time, &time, 1);
		}
		return;
	}
#endif multimax

#if	MACH_NBC
	iunlock(oip);	/* ARGH!!! */
	mfs_trunc(oip, length);
	ilock(oip);
#endif	MACH_NBC

	if (oip->i_size <= length) {
		oip->i_flag |= ICHG|IUPD;
		{
		    struct timeval time;
		    get_time(&time);
		    /*
		     * In BSD4.3, iupdat is called with waitfor == 1.
		     * We make it 0 to get better times on the AIM benchmark.
		     */
		    iupdat(oip, &time, &time, 0);
		}
		return;
	}
	/*
	 * Calculate index into inode's block list of
	 * last direct and indirect blocks (if any)
	 * which we want to keep.  Lastblock is -1 when
	 * the file is truncated to 0.
	 */
	fs = oip->i_fs;
	lastblock = lblkno(fs, length + fs->fs_bsize - 1) - 1;
	lastiblock[SINGLE] = lastblock - NDADDR;
	lastiblock[DOUBLE] = lastiblock[SINGLE] - NINDIR(fs);
	lastiblock[TRIPLE] = lastiblock[DOUBLE] - NINDIR(fs) * NINDIR(fs);
	nblocks = btodb(fs->fs_bsize);
	/*
	 * Update the size of the file. If the file is not being
	 * truncated to a block boundry, the contents of the
	 * partial block following the end of the file must be
	 * zero'ed in case it ever become accessable again because
	 * of subsequent file growth.
	 */
	osize = oip->i_size;
	offset = blkoff(fs, length);
	if (offset == 0) {
		oip->i_size = length;
	} else {
		lbn = lblkno(fs, length);
		bn = fsbtodb(fs, bmap(oip, lbn, B_WRITE, offset));
		if (u.u_error || (long)bn < 0)
			return;
		oip->i_size = length;
		size = blksize(fs, oip, lbn);
		dev = oip->i_dev;
		if (oip->pager != MEMORY_OBJECT_NULL)
			inode_uncache(oip);
#if	MACH_NBC && !MAP_UAREA
		/* data flushed in mfs_trunc above */
#else	MACH_NBC && !MAP_UAREA
		bp = bread(dev, bn, size);
		if (bp->b_flags & B_ERROR) {
			u.u_error = EIO;
			oip->i_size = osize;
			brelse(bp);
			return;
		}
		bzero(bp->b_un.b_addr + offset, (unsigned)(size - offset));
		bdwrite(bp);
#endif	MACH_NBC && !MAP_UAREA
	}
	/*
	 * Update file and block pointers
	 * on disk before we start freeing blocks.
	 * If we crash before free'ing blocks below,
	 * the blocks will be returned to the free list.
	 * lastiblock values are also normalized to -1
	 * for calls to indirtrunc below.
	 */
	tip = *oip;
	tip.i_size = osize;
	for (level = TRIPLE; level >= SINGLE; level--)
		if (lastiblock[level] < 0) {
			oip->i_ib[level] = 0;
			lastiblock[level] = -1;
		}
	for (i = NDADDR - 1; i > lastblock; i--)
		oip->i_db[i] = 0;
	oip->i_flag |= ICHG|IUPD;
	syncip(oip);

	/*
	 * Indirect blocks first.
	 */
	ip = &tip;
	for (level = TRIPLE; level >= SINGLE; level--) {
		bn = ip->i_ib[level];
		if (bn != 0) {
			blocksreleased +=
			    indirtrunc(ip, bn, lastiblock[level], level);
			if (lastiblock[level] < 0) {
				ip->i_ib[level] = 0;
				free_block(ip, bn, (off_t)fs->fs_bsize);
				blocksreleased += nblocks;
			}
		}
		if (lastiblock[level] >= 0)
			goto done;
	}

	/*
	 * All whole direct blocks or frags.
	 */
	for (i = NDADDR - 1; i > lastblock; i--) {
		register off_t bsize;

		bn = ip->i_db[i];
		if (bn == 0)
			continue;
		ip->i_db[i] = 0;
		bsize = (off_t)blksize(fs, ip, i);
		free_block(ip, bn, bsize);
		blocksreleased += btodb(bsize);
	}
	if (lastblock < 0)
		goto done;

	/*
	 * Finally, look for a change in size of the
	 * last direct block; release any frags.
	 */
	bn = ip->i_db[lastblock];
	if (bn != 0) {
		off_t oldspace, newspace;

		/*
		 * Calculate amount of space we're giving
		 * back as old block size minus new block size.
		 */
		oldspace = blksize(fs, ip, lastblock);
		ip->i_size = length;
		newspace = blksize(fs, ip, lastblock);
		if (newspace == 0)
			panic("itrunc: newspace");
		if (oldspace - newspace > 0) {
			/*
			 * Block number of space to be free'd is
			 * the old block # plus the number of frags
			 * required for the storage we're keeping.
			 */
			bn += numfrags(fs, newspace);
			free_block(ip, bn, oldspace - newspace);
			blocksreleased += btodb(oldspace - newspace);
		}
	}
done:
/* BEGIN PARANOIA */
	for (level = SINGLE; level <= TRIPLE; level++)
		if (ip->i_ib[level] != oip->i_ib[level])
			panic("itrunc1");
	for (i = 0; i < NDADDR; i++)
		if (ip->i_db[i] != oip->i_db[i])
			panic("itrunc2");
/* END PARANOIA */
	oip->i_blocks -= blocksreleased;
	if (oip->i_blocks < 0)			/* sanity */
		oip->i_blocks = 0;
	oip->i_flag |= ICHG;
#if	QUOTA
	(void) chkdq(oip, -blocksreleased, 0);
#endif	QUOTA
}

/*
 * Release blocks associated with the inode ip and
 * stored in the indirect block bn.  Blocks are free'd
 * in LIFO order up to (but not including) lastbn.  If
 * level is greater than SINGLE, the block is an indirect
 * block and recursive calls to indirtrunc must be used to
 * cleanse other indirect blocks.
 *
 * NB: triple indirect blocks are untested.
 */
long
indirtrunc(ip, bn, lastbn, level)
	register struct inode *ip;
	daddr_t bn, lastbn;
	int level;
{
	register int i;
	struct buf *bp, *copy;
	register daddr_t *bap;
	register struct fs *fs = ip->i_fs;
	daddr_t nb, last;
	long factor;
	int blocksreleased = 0, nblocks;

	/*
	 * Calculate index in current block of last
	 * block to be kept.  -1 indicates the entire
	 * block so we need not calculate the index.
	 */
	factor = 1;
	for (i = SINGLE; i < level; i++)
		factor *= NINDIR(fs);
	last = lastbn;
	if (lastbn > 0)
		last /= factor;
	nblocks = btodb(fs->fs_bsize);
	/*
	 * Get buffer of block pointers, zero those 
	 * entries corresponding to blocks to be free'd,
	 * and update on disk copy first.
	 */
	copy = geteblk((int)fs->fs_bsize);
	bp = bread(ip->i_dev, fsbtodb(fs, bn), (int)fs->fs_bsize);
	if (bp->b_flags&B_ERROR) {
		brelse(copy);
		brelse(bp);
		return (0);
	}
	bap = bp->b_un.b_daddr;
	bcopy((caddr_t)bap, (caddr_t)copy->b_un.b_daddr, (u_int)fs->fs_bsize);
	bzero((caddr_t)&bap[last + 1],
	  (u_int)(NINDIR(fs) - (last + 1)) * sizeof (daddr_t));
	baforce(bp);
#if	MACH_NBC
	/* indirect blocks don't need flushing */
#endif	MACH_NBC
	bp = copy, bap = bp->b_un.b_daddr;

	/*
	 * Recursively free totally unused blocks.
	 */
	for (i = NINDIR(fs) - 1; i > last; i--) {
		nb = bap[i];
		if (nb == 0)
			continue;
		if (level > SINGLE)
			blocksreleased +=
			    indirtrunc(ip, nb, (daddr_t)-1, level - 1);
		free_block(ip, nb, (off_t)fs->fs_bsize);
		blocksreleased += nblocks;
	}

	/*
	 * Recursively free last partial block.
	 */
	if (level > SINGLE && lastbn >= 0) {
		last = lastbn % factor;
		nb = bap[i];
		if (nb != 0)
			blocksreleased += indirtrunc(ip, nb, last, level - 1);
	}
	brelse(bp);
	return (blocksreleased);
}

/*
 * remove any inodes in the inode cache belonging to dev
 *
 * There should not be any active ones, return error if any are found
 * (nb: this is a user error, not a system err)
 *
 * Also, count the references to dev by block devices - this really
 * has nothing to do with the object of the procedure, but as we have
 * to scan the inode table here anyway, we might as well get the
 * extra benefit.
 *
 * this is called from sumount()/sys3.c when dev is being unmounted
 */
#if	QUOTA
iflush(dev, iq)
	dev_t dev;
	struct inode *iq;
#else	QUOTA
iflush(dev)
	dev_t dev;
#endif	QUOTA
{
	register struct inode *ip;
	register open = 0;

	for (ip = inode; ip < inodeNINODE; ip++) {
#if	QUOTA
		if (ip != iq && ip->i_dev == dev) {
#else	QUOTA
		if (ip->i_dev == dev) {
#endif	QUOTA
			if (ip->i_count)
			    inode_uncache_try(ip);
			if (ip->i_count)
				return(-1);
			else {
				remque(ip);
				ip->i_forw = ip;
				ip->i_back = ip;
				/*
				 * as i_count == 0, the inode was on the free
				 * list already, just leave it there, it will
				 * fall off the bottom eventually. We could
				 * perhaps move it to the head of the free
				 * list, but as umounts are done so
				 * infrequently, we would gain very little,
				 * while making the code bigger.
				 */
#if	QUOTA
				dqrele(ip->i_dquot);
				ip->i_dquot = NODQUOT;
#endif	QUOTA
			}
		} else if (ip->i_count && (ip->i_mode&IFMT)==IFBLK &&
		    ip->i_rdev == dev)
			open++;
	}
	return (open);
}

/*
 * Lock an inode. If its already locked, set the WANT bit and sleep.
 */
ilock(ip)
	register struct inode *ip;
{

	ILOCK(ip);
}

/*
 * Unlock an inode.  If WANT bit is on, wakeup.
 */
iunlock(ip)
	register struct inode *ip;
{

	IUNLOCK(ip);
}
