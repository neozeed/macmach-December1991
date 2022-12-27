/* 
 * Mach Operating System
 * Copyright (c) 1988 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/* 
 * HISTORY
 * $Log:	ufs_bio.c,v $
 * Revision 2.4  91/01/08  15:00:03  rpd
 * 	Shuffled spl around a bit to make the buffer cache less vulnerable
 * 	to deadlocks due to its parallelization.  Since this code will
 * 	soon change significantly (NFS/OFS/4.4.. upgrades) I am not
 * 	going to waste time to replace spls with locks, Encore has done
 * 	that already.
 * 	[91/01/04  13:14:32  af]
 * 
 * Revision 2.3  90/06/19  23:09:31  rpd
 * 	Purged CMUCS, MACH conditionals.
 * 	[90/06/19            rpd]
 * 
 * Revision 2.2  90/05/29  20:23:36  rwd
 * 	Rfr code for limited inode writeback.
 * 	[90/04/12  16:11:33  rwd]
 * 
 * Revision 2.1  89/08/04  14:14:30  rwd
 * Created.
 * 
 * Revision 2.3  88/08/24  01:33:31  mwyoung
 * 	Corrected include file references.
 * 	[88/08/22  22:05:21  mwyoung]
 * 
 *
 * 14-Dec-87  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Added measurements in getblk()... eventually these can be removed.
 *
 * 04-Aug-87  Mike Accetta (mja) at Carnegie-Mellon University
 *	Installed fix from Rick Rashid to avoid using the BQ_AGE queue
 *	for delayed writes (to prevent the cache from filling up mostly
 *	with status information at the expense of file data).  This
 *	prevents discrimination against file data blocks which existed
 *	in the original Berkeley buffer management. This
 *	change is enabled by the BQ_AGE_DISABLE flag which is on by
 *	default with the expectation that the majority of the systems
 *	(e.g. single-user workstations) will benefit.  Large multi-user
 *	systems may want to patch it off.
 *	[ V5.1(XF15) ]
 *
 * 16-Jul-86  Bill Bolosky (bolosky) at Carnegie-Mellon University
 *	CS_GENERIC: Changed "size" parameter to blkflush to be u_long
 *	instead of long.  
 *
 *  7-Jul-86  Bill Bolosky (bolosky) at Carnegie-Mellon University
 *	Removed Unnecessary include of pte.h
 *
 */
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)ufs_bio.c	7.1 (Berkeley) 6/5/86
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/buf.h>
#include <sys/conf.h>
#include <sys/proc.h>
#include <sys/trace.h>

int	BQ_AGE_DISABLE = 1;

int bio_count = 0;
#define BIO_COUNT_MAX 16

#undef 	BUFHASH
#define	BUFHASH(dev, dblkno)	\
	((struct buf *)&bufhash[((((int)(dblkno >> 4))))&(BUFHSZ-1)])

/*
 * Read in (if necessary) the block and return a buffer pointer.
 */
struct buf *
bread(dev, blkno, size)
	dev_t dev;
	daddr_t blkno;
	int size;
{
	register struct buf *bp;

	if (size == 0)
		panic("bread: size 0");
	bp = getblk(dev, blkno, size);
	if (bp->b_flags&B_DONE) {
		trace(TR_BREADHIT, pack(dev, size), blkno);
		return (bp);
	}
	bp->b_flags |= B_READ;
	if (bp->b_bcount > bp->b_bufsize)
		panic("bread");
	(*bdevsw[major(dev)].d_strategy)(bp);
	bio_count++;
	trace(TR_BREADMISS, pack(dev, size), blkno);
	u.u_ru.ru_inblock++;		/* pay for read */
	biowait(bp);
	return (bp);
}

/*
 * Read in the block, like bread, but also start I/O on the
 * read-ahead block (which is not allocated to the caller)
 */
struct buf *
breada(dev, blkno, size, rablkno, rabsize)
	dev_t dev;
	daddr_t blkno; int size;
	daddr_t rablkno; int rabsize;
{
	register struct buf *bp, *rabp;

	bp = NULL;
	/*
	 * If the block isn't in core, then allocate
	 * a buffer and initiate i/o (getblk checks
	 * for a cache hit).
	 */
	if (!incore(dev, blkno)) {
		bp = getblk(dev, blkno, size);
		if ((bp->b_flags&B_DONE) == 0) {
			bp->b_flags |= B_READ;
			if (bp->b_bcount > bp->b_bufsize)
				panic("breada");
			(*bdevsw[major(dev)].d_strategy)(bp);
			bio_count++;
			trace(TR_BREADMISS, pack(dev, size), blkno);
			u.u_ru.ru_inblock++;		/* pay for read */
		} else
			trace(TR_BREADHIT, pack(dev, size), blkno);
	}

	/*
	 * If there's a read-ahead block, start i/o
	 * on it also (as above).
	 */
	if (rablkno && !incore(dev, rablkno)) {
		rabp = getblk(dev, rablkno, rabsize);
		if (rabp->b_flags & B_DONE) {
			brelse(rabp);
			trace(TR_BREADHITRA, pack(dev, rabsize), blkno);
		} else {
			rabp->b_flags |= B_READ|B_ASYNC;
			if (rabp->b_bcount > rabp->b_bufsize)
				panic("breadrabp");
			(*bdevsw[major(dev)].d_strategy)(rabp);
			bio_count++;
			trace(TR_BREADMISSRA, pack(dev, rabsize), rablock);
			u.u_ru.ru_inblock++;		/* pay in advance */
		}
	}

	/*
	 * If block was in core, let bread get it.
	 * If block wasn't in core, then the read was started
	 * above, and just wait for it.
	 */
	if (bp == NULL)
		return (bread(dev, blkno, size));
	biowait(bp);
	return (bp);
}

/*
 * Write the buffer, waiting for completion.
 * Then release the buffer.
 */
bforce(bp)
	register struct buf *bp;
{
	register flag;

	flag = bp->b_flags;
	bp->b_flags &= ~(B_READ | B_DONE | B_ERROR | B_DELWRI);
	if ((flag&B_DELWRI) == 0)
		u.u_ru.ru_oublock++;		/* noone paid yet */
	trace(TR_BWRITE, pack(bp->b_dev, bp->b_bcount), bp->b_blkno);
	if (bp->b_bcount > bp->b_bufsize)
		panic("bwrite");

	(*bdevsw[major(bp->b_dev)].d_strategy)(bp);
	bio_count++;

	if (bio_count > BIO_COUNT_MAX) {
		bp->b_flags &= ~B_ASYNC;
		flag &= ~B_ASYNC;
	}		

	/*
	 * If the write was synchronous, then await i/o completion.
	 * If the write was "delayed", then we put the buffer on
	 * the q of blocks awaiting i/o completion status.
	 */
	if ((flag&B_ASYNC) == 0) {
		biowait(bp);
		brelse(bp);
	} else if (flag & B_DELWRI) 
		bp->b_flags |= B_AGE;
}

int	bio_force = 0;
int	bio_async = 1;

bwrite(bp)
	register struct buf *bp;
{
	if (bio_force) {
		if (bio_async) {
			bp->b_flags |= B_ASYNC;
		}
		bforce(bp);
	} else 
		bdwrite(bp);
}

baforce(bp)
	register struct buf *bp;
{
	bp->b_flags |= B_ASYNC;
	bforce(bp);
}

/*
 * Release the buffer, marking it so that if it is grabbed
 * for another purpose it will be written out before being
 * given up (e.g. when writing a partial block where it is
 * assumed that another write for the same block will soon follow).
 * This can't be done for magtape, since writes must be done
 * in the same order as requested.
 */
bdwrite(bp)
	register struct buf *bp;
{
	register int flags;

	if ((bp->b_flags&B_DELWRI) == 0)
		u.u_ru.ru_oublock++;		/* noone paid yet */
	flags = bdevsw[major(bp->b_dev)].d_flags;
	if(flags & B_TAPE)
		bawrite(bp);
	else {
		bp->b_flags |= B_DELWRI | B_DONE;
		brelse(bp);
	}
}

/*
 * Release the buffer, start I/O on it, but don't wait for completion.
 */
bawrite(bp)
	register struct buf *bp;
{

	bp->b_flags |= B_ASYNC;
	bwrite(bp);
}

/*
 * Release the buffer, with no I/O implied.
 */
brelse(bp)
	register struct buf *bp;
{
	register struct buf *flist;
	register s, flags;
	int athead, awakem;

	trace(TR_BRELSE, pack(bp->b_dev, bp->b_bufsize), bp->b_blkno);

	if (bp->b_flags&B_ERROR)
		if (bp->b_flags & B_LOCKED)
			bp->b_flags &= ~B_ERROR;	/* try again later */
		else
			bp->b_dev = NODEV;  		/* no assoc */

	/*
	 * Stick the buffer back on a free list.
	 */
	athead = 1;
	flags = bp->b_flags;
	if (bp->b_bufsize <= 0) {
		/* block has no buffer ... put at front of unused buffer list */
		flist = &bfreelist[BQ_EMPTY];
	} else if (flags & (B_ERROR|B_INVAL|B_PAGET)) {
		/* block has no info ... put at front of most free list */
		flist = &bfreelist[BQ_AGE];
	} else {
		if (flags & B_LOCKED)
			flist = &bfreelist[BQ_LOCKED];
		else if ((flags & B_AGE) && (BQ_AGE_DISABLE == 0))
			flist = &bfreelist[BQ_AGE];
		else
			flist = &bfreelist[BQ_LRU];
		athead = 0;
	}

	s = splbio();

	bp->b_flags &= ~(B_WANTED|B_BUSY|B_ASYNC|B_AGE|B_PAGET);
	if (athead) {
		binsheadfree(bp, flist);
	} else {
		binstailfree(bp, flist);
	}

	if (bfreelist[0].b_flags&B_WANTED) {
		bfreelist[0].b_flags &= ~B_WANTED;
		awakem = 1;
	} else
		awakem = 0;

	splx(s);

	/*
	 * If someone's waiting for the buffer, or
	 * is waiting for a buffer wake 'em up.
	 */
	if (flags&B_WANTED)
		wakeup((caddr_t)bp);
	if (awakem)
		wakeup((caddr_t)bfreelist);
}

/*
 * See if the block is associated with some buffer
 * (mainly to avoid getting hung up on a wait in breada)
 */
incore(dev, blkno)
	dev_t dev;
	daddr_t blkno;
{
	register struct buf *bp;
	register struct buf *dp;

	/* xxx strictly speaking this would need spls too */
	dp = BUFHASH(dev, blkno);
	for (bp = dp->b_forw; bp != dp; bp = bp->b_forw)
		if (bp->b_blkno == blkno && bp->b_dev == dev &&
		    (bp->b_flags & B_INVAL) == 0)
			return (1);
	return (0);
}

#if	0
struct buf *
baddr(dev, blkno, size)
	dev_t dev;
	daddr_t blkno;
	int size;
{

	if (incore(dev, blkno))
		return (bread(dev, blkno, size));
	return (0);
}
#endif

struct {
	int	hits;
	int	misses;
} getblk_stats = { 0, 0 };

/*
 * Assign a buffer for the given block.  If the appropriate
 * block is already associated, return it; otherwise search
 * for the oldest non-busy buffer and reassign it.
 *
 * We use splx here because this routine may be called
 * on the interrupt stack during a dump, and we don't
 * want to lower the ipl back to 0.
 */
struct buf *
getblk(dev, blkno, size)
	dev_t dev;
	daddr_t blkno;
	int size;
{
	register struct buf *bp, *dp;
	int s;

	if (size > MAXBSIZE)
		panic("getblk: size too big");
	/*
	 * To prevent overflow of 32-bit ints when converting block
	 * numbers to byte offsets, blknos > 2^32 / DEV_BSIZE are set
	 * to the maximum number that can be converted to a byte offset
	 * without overflow. This is historic code; what bug it fixed,
	 * or whether it is still a reasonable thing to do is open to
	 * dispute. mkm 9/85
	 */
	if ((unsigned)blkno >= 1 << (sizeof(int)*NBBY-DEV_BSHIFT))
		blkno = 1 << ((sizeof(int)*NBBY-DEV_BSHIFT) + 1);
	/*
	 * Search the cache for the block.  If we hit, but
	 * the buffer is in use for i/o, then we wait until
	 * the i/o has completed.
	 */
	dp = BUFHASH(dev, blkno);
loop:
	s = splbio();
	for (bp = dp->b_forw; bp != dp; bp = bp->b_forw) {
		if (bp->b_blkno != blkno || bp->b_dev != dev ||
		    bp->b_flags&B_INVAL)
			continue;
		if (bp->b_flags&B_BUSY) {
			bp->b_flags |= B_WANTED;
			sleep((caddr_t)bp, PRIBIO+1);
			splx(s);
			goto loop;
		}
		s_notavail(bp);
		splx(s);
		if (bp->b_bcount != size && brealloc(bp, size) == 0)
			goto loop;
		getblk_stats.hits++;
		return (bp);
	}
	if (major(dev) >= nblkdev)
		panic("blkdev");
	getblk_stats.misses++;
	splx(s);

	bp = getnewbuf();
	bfree(bp);
	bp->b_dev = dev;
	bp->b_blkno = blkno;
	bp->b_error = 0;

	s = splbio();
	bremhash(bp);
	binshash(bp, dp);
	splx(s);

	if (brealloc(bp, size) == 0)
		goto loop;
	return (bp);
}

/*
 * get an empty block,
 * not assigned to any particular device
 */
struct buf *
geteblk(size)
	int size;
{
	register struct buf *bp, *flist;
	register s;

	if (size > MAXBSIZE)
		panic("geteblk: size too big");
loop:
	bp = getnewbuf();
	bp->b_flags |= B_INVAL;
	bfree(bp);
	bp->b_dev = (dev_t)NODEV;
	bp->b_error = 0;
	flist = &bfreelist[BQ_AGE];

	s = splbio();
	bremhash(bp);
	binshash(bp, flist);
	splx(s);

	if (brealloc(bp, size) == 0)
		goto loop;
	return (bp);
}

/*
 * Allocate space associated with a buffer.
 * If can't get space, buffer is released
 */
brealloc(bp, size)
	register struct buf *bp;
	int size;
{
	daddr_t start, last;
	register struct buf *ep;
	struct buf *dp;
	int s;

	/*
	 * First need to make sure that all overlaping previous I/O
	 * is dispatched with.
	 */
	if (size == bp->b_bcount)
		return (1);
	if (size < bp->b_bcount) { 
		if (bp->b_flags & B_DELWRI) {
			bforce(bp);
			return (0);
		}
		if (bp->b_flags & B_LOCKED)
			panic("brealloc");
		return (allocbuf(bp, size));
	}
	bp->b_flags &= ~B_DONE;
	if (bp->b_dev == NODEV)
		return (allocbuf(bp, size));

	trace(TR_BREALLOC, pack(bp->b_dev, size), bp->b_blkno);
	/*
	 * Search cache for any buffers that overlap the one that we
	 * are trying to allocate. Overlapping buffers must be marked
	 * invalid, after being written out if they are dirty. (indicated
	 * by B_DELWRI) A disk block must be mapped by at most one buffer
	 * at any point in time. Care must be taken to avoid deadlocking
	 * when two buffer are trying to get the same set of disk blocks.
	 */
	start = bp->b_blkno;
	last = start + btodb(size) - 1;
	dp = BUFHASH(bp->b_dev, bp->b_blkno);
loop:
	s = splbio();
	for (ep = dp->b_forw; ep != dp; ep = ep->b_forw) {
		if (ep == bp || ep->b_dev != bp->b_dev || (ep->b_flags&B_INVAL))
			continue;
		/* look for overlap */
		if (ep->b_bcount == 0 || ep->b_blkno > last ||
		    ep->b_blkno + btodb(ep->b_bcount) <= start)
			continue;
		if (ep->b_flags&B_BUSY) {
			ep->b_flags |= B_WANTED;
			sleep((caddr_t)ep, PRIBIO+1);
			splx(s);
			goto loop;
		}
		s_notavail(ep);

		if (ep->b_flags & B_DELWRI) {
			splx(s);
			bforce(ep);
			goto loop;
		}
		ep->b_flags |= B_INVAL;
		brelse(ep);
	}
	splx(s);
	return (allocbuf(bp, size));
}

/*
 * Find a buffer which is available for use.
 * Select something from a free list.
 * Preference is to AGE list, then LRU list.
 */
struct buf *
getnewbuf()
{
	register struct buf *bp, *dp;
	int s;

loop:
	s = splbio();
	for (dp = &bfreelist[BQ_AGE]; dp > bfreelist; dp--)
		if (dp->av_forw != dp)
			break;
	if (dp == bfreelist) {		/* no free blocks */
		dp->b_flags |= B_WANTED;
		sleep((caddr_t)dp, PRIBIO+1);
		splx(s);
		goto loop;
	}
	bp = dp->av_forw;
	s_notavail(bp);
	if (bp->b_flags & B_DELWRI) {
		baforce(bp);
		splx(s);
		goto loop;
	}
	bp->b_flags = B_BUSY;
	splx(s);
	trace(TR_BRELSE, pack(bp->b_dev, bp->b_bufsize), bp->b_blkno);
	return (bp);
}

/*
 * Wait for I/O completion on the buffer; return errors
 * to the user.
 */
biowait(bp)
	register struct buf *bp;
{
	int s;

	s = splbio();
	while ((bp->b_flags&B_DONE)==0)
		sleep((caddr_t)bp, PRIBIO);
	splx(s);
	if (u.u_error == 0)			/* XXX */
		u.u_error = geterror(bp);
}

/*
 * Mark I/O complete on a buffer.
 * If someone should be called, e.g. the pageout
 * daemon, do so.  Otherwise, wake up anyone
 * waiting for it.
 */
biodone(bp)
	register struct buf *bp;
{

	if (bp->b_flags & B_DONE)
		panic("dup biodone");
	bp->b_flags |= B_DONE;
	if (bp->b_flags & B_CALL) {
		bp->b_flags &= ~B_CALL;
		(*bp->b_iodone)(bp);
		return;
	}

	bio_count--;
	
	if (bp->b_flags&B_ASYNC)
		brelse(bp);
	else {
		bp->b_flags &= ~B_WANTED;
		wakeup((caddr_t)bp);
	}
}

/*
 * Insure that no part of a specified block is in an incore buffer.
 */
blkflush(dev, blkno, size)
	dev_t dev;
	daddr_t blkno;
	u_long size;
{
	register struct buf *ep;
	struct buf *dp;
	daddr_t start, last;
	int s;

	start = blkno;
	last = start + btodb(size) - 1;
	dp = BUFHASH(dev, blkno);
loop:
	s = splbio();
	for (ep = dp->b_forw; ep != dp; ep = ep->b_forw) {
		if (ep->b_dev != dev || (ep->b_flags&B_INVAL))
			continue;
		/* look for overlap */
		if (ep->b_bcount == 0 || ep->b_blkno > last ||
		    ep->b_blkno + btodb(ep->b_bcount) <= start)
			continue;
		if (ep->b_flags&B_BUSY) {
			ep->b_flags |= B_WANTED;
			sleep((caddr_t)ep, PRIBIO+1);
			splx(s);
			goto loop;
		}
		if (ep->b_flags & B_DELWRI) {
			s_notavail(ep);
			splx(s);
			bforce(ep);
			goto loop;
		}
	}
	splx(s);
}

/*
 * Make sure all write-behind blocks
 * on dev (or NODEV for all)
 * are flushed out.
 * (from umount and update)
 */
bflush(dev)
	dev_t dev;
{
	register struct buf *bp;
	register struct buf *flist;
	int s;

loop:
	s = splbio();
	for (flist = bfreelist; flist < &bfreelist[BQ_EMPTY]; flist++)
	for (bp = flist->av_forw; bp != flist; bp = bp->av_forw) {
		if ((bp->b_flags & B_DELWRI) == 0)
			continue;
		if (dev == NODEV || dev == bp->b_dev) {
			bp->b_flags |= B_ASYNC;
			s_notavail(bp);
			bforce(bp);
			splx(s);
			goto loop;
		}
	}
	splx(s);
}

/*
 * Pick up the device's error number and pass it to the user;
 * if there is an error but the number is 0 set a generalized code.
 */
geterror(bp)
	register struct buf *bp;
{
	int error = 0;

	if (bp->b_flags&B_ERROR)
		if ((error = bp->b_error)==0)
			return (EIO);
	return (error);
}

/*
 * Invalidate in core blocks belonging to closed or umounted filesystem
 *
 * This is not nicely done at all - the buffer ought to be removed from the
 * hash chains & have its dev/blkno fields clobbered, but unfortunately we
 * can't do that here, as it is quite possible that the block is still
 * being used for i/o. Eventually, all disc drivers should be forced to
 * have a close routine, which ought ensure that the queue is empty, then
 * properly flush the queues. Until that happy day, this suffices for
 * correctness.						... kre
 */
binval(dev)
	dev_t dev;
{
	register struct buf *bp;
	register struct bufhd *hp;
#define dp ((struct buf *)hp)
	register s;

	s = splbio();
	for (hp = bufhash; hp < &bufhash[BUFHSZ]; hp++)
		for (bp = dp->b_forw; bp != dp; bp = bp->b_forw)
			if (bp->b_dev == dev)
				bp->b_flags |= B_INVAL;
	splx(s);
}
