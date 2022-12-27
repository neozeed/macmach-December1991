/* 
 **********************************************************************
 * Mach Operating System
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 **********************************************************************
 * HISTORY
 * $Log:	ufs_subr.c,v $
 * Revision 2.4  91/01/08  15:00:13  rpd
 * 	Use non synchronizing version of notavail inside a
 * 	larger section of spl-protected code.  This makes 
 * 	its munging with freelists a bit safer.
 * 	[91/01/04  13:10:12  af]
 * 
 * Revision 2.3  90/05/29  20:23:46  rwd
 * 	Rfr limited inode writeback changes.
 * 	[90/04/12  16:12:21  rwd]
 * 
 * Revision 2.2  90/01/19  14:35:26  rwd
 * 	Add #if MACH_NBC's around mfs_cache_sync
 * 	[90/01/16            rwd]
 * 	Call mfs_cache_sync
 * 	[90/01/15            rwd]
 * 
 * Revision 2.1  89/08/04  14:07:16  rwd
 * Created.
 * 
 *  6-Jan-89  David Golub (dbg) at Carnegie-Mellon University
 *	Out-of-kernel version.
 *
 * 10-Oct-87  Stephen Schwab (schwab) at Carnegie-Mellon University
 *	Changes for 650.  Left in c routine for scanc.
 *
 *  2-Dec-86  Jay Kistler (jjk) at Carnegie-Mellon University
 *	VICE:  added igetfs() routine to support ITC/Andrew rfs.
 *
 * 28-Oct-86  Jonathan J. Chew (jjc) at Carnegie-Mellon University
 *	Make this compile outside the kernel (for use in fsck).
 *
 * 13-Aug-86  Jonathan J. Chew (jjc) at Carnegie-Mellon University
 *	Don't use "scanc" for Sun because Sun has its own version
 *	already.
 *
 * 27-Jul-86  Bill Bolosky (bolosky) at Carnegie-Mellon University
 *	MACH: Removed check for swapdev in getfsx.
 *
 * 20-May-86  Mike Accetta (mja) at Carnegie-Mellon University
 *	CMUCS:  changed syncip() to use IMOD rather than ICHG
 *	flag for iudpat() (although I'm not sure even that is really
 *	necessary).
 *	
 * 26-Jan-86  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Upgraded to 4.3.  [Why does this file use "#ifdef KERNEL"?]
 *
 * 20-May-85  Glenn Marcy (gm0w) at Carnegie-Mellon University
 *	Upgraded to 4.2BSD.  Carried over changes below [V1(1)].
 *
 *	CMUCS:  Changed inode reference count modification to use
 *	incr/decr macros to check for consistency.
 *
 **********************************************************************
 */
 
#ifdef	KERNEL
#include <mach_no_kernel.h>

#include "cputypes.h"

#include "cmucs.h"
#include "mach.h"
#else	KERNEL
#include <sys/features.h>
#endif	KERNEL
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)ufs_subr.c	7.1 (Berkeley) 6/5/86
 */

#ifdef KERNEL
#include "sys/param.h"
#include "sys/systm.h"
#include "sys/mount.h"
#include "sys/fs.h"
#include "sys/buf.h"
#include "sys/inode.h"
#include "sys/dir.h"
#include "sys/user.h"
#include "sys/quota.h"
#include "sys/kernel.h"
#else
#include <sys/param.h>
#include <sys/systm.h>
#include <sys/mount.h>
#include <sys/fs.h>
#include <sys/buf.h>
#include <sys/inode.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/quota.h>
#endif

#ifdef KERNEL
int	syncprt = 0;

/*
 * Update is the internal name of 'sync'.  It goes through the disk
 * queues to initiate sandbagged IO; goes through the inodes to write
 * modified nodes; and it goes through the mount table to initiate
 * the writing of the modified super blocks.
 */
update()
{
	register struct inode *ip;
	register struct mount *mp;
	struct fs *fs;

	if (syncprt)
		bufstats();
	if (updlock)
		return;
	updlock++;

#if	MACH_NBC
	mfs_cache_sync();
#endif	MACH_NBC

	/*
	 * Write back modified superblocks.
	 * Consistency check that the superblock
	 * of each file system is still in the buffer cache.
	 */
	for (mp = &mount[0]; mp < &mount[NMOUNT]; mp++) {
		if (mp->m_bufp == NULL || mp->m_dev == NODEV)
			continue;
		fs = mp->m_bufp->b_un.b_fs;
		if (fs->fs_fmod == 0)
			continue;
		if (fs->fs_ronly != 0) {		/* XXX */
			printf("fs = %s\n", fs->fs_fsmnt);
			panic("update: rofs mod");
		}
		fs->fs_fmod = 0;
#if	MACH_NO_KERNEL
		{
		    struct timeval time;
		    get_time(&time);
		    fs->fs_time = time.tv_sec;
		}
#else	MACH_NO_KERNEL
		fs->fs_time = time.tv_sec;
#endif	MACH_NO_KERNEL
		sbupdate(mp);
	}
	/*
	 * Write back each (modified) inode.
	 */
	for (ip = inode; ip < inodeNINODE; ip++) {
		if ((ip->i_flag & ILOCKED) != 0 || ip->i_count == 0 ||
		    (ip->i_flag & (IMOD|IACC|IUPD|ICHG)) == 0)
			continue;
		ip->i_flag |= ILOCKED;
#if	CMUCS
		iincr_chk(ip);
#else	CMUCS
		ip->i_count++;
#endif	CMUCS
#if	MACH_NO_KERNEL
		{
		    struct timeval time;
		    get_time(&time);
		    iupdat(ip, &time, &time, 0);
		}
#else	MACH_NO_KERNEL
		iupdat(ip, &time, &time, 0);
#endif	MACH_NO_KERNEL
		iput(ip);
	}
	updlock = 0;
	/*
	 * Force stale buffer cache information to be flushed,
	 * for all devices.
	 */
	bflush(NODEV);
}

/*
 * Flush all the blocks associated with an inode.
 * There are two strategies based on the size of the file;
 * large files are those with more than (nbuf / 2) blocks.
 * Large files
 * 	Walk through the buffer pool and push any dirty pages
 *	associated with the device on which the file resides.
 * Small files
 *	Look up each block in the file to see if it is in the 
 *	buffer pool writing any that are found to disk.
 *	Note that we make a more stringent check of
 *	writing out any block in the buffer pool that may
 *	overlap the inode. This brings the inode up to
 *	date with recent mods to the cooked device.
 */
syncip(ip)
	register struct inode *ip;
{
	register struct fs *fs;
	register struct buf *bp;
	struct buf *lastbufp;
	long lbn, lastlbn;
	int s;
	daddr_t blkno;

	fs = ip->i_fs;
	lastlbn = howmany(ip->i_size, fs->fs_bsize);
	if (lastlbn < nbuf / 2) {
		for (lbn = 0; lbn < lastlbn; lbn++) {
			blkno = fsbtodb(fs, bmap(ip, lbn, B_READ));
			blkflush(ip->i_dev, blkno, blksize(fs, ip, lbn));
		}
	} else {
		lastbufp = &buf[nbuf];
		for (bp = buf; bp < lastbufp; bp++) {
			if (bp->b_dev != ip->i_dev ||
			    (bp->b_flags & B_DELWRI) == 0)
				continue;
			s = splbio();
			if (bp->b_flags & B_BUSY) {
				bp->b_flags |= B_WANTED;
				sleep((caddr_t)bp, PRIBIO+1);
				splx(s);
				bp--;
				continue;
			}
			s_notavail(bp);
			splx(s);
			bwrite(bp);
		}
	}
#if	CMUCS
	ip->i_flag |= IMOD;
#else	CMUCS
	ip->i_flag |= ICHG;
#endif	CMUCS
#if	MACH_NO_KERNEL
	{
	    struct timeval time;
	    get_time(&time);
	    iupdat(ip, &time, &time, 2 /* 1 */);
	}
#else	MACH_NO_KERNEL
	iupdat(ip, &time, &time, 1);
#endif	MACH_NO_KERNEL
}
#endif

extern	int around[9];
extern	int inside[9];
extern	u_char *fragtbl[];

/*
 * Update the frsum fields to reflect addition or deletion 
 * of some frags.
 */
fragacct(fs, fragmap, fraglist, cnt)
	struct fs *fs;
	int fragmap;
	long fraglist[];
	int cnt;
{
	int inblk;
	register int field, subfield;
	register int siz, pos;

	inblk = (int)(fragtbl[fs->fs_frag][fragmap]) << 1;
	fragmap <<= 1;
	for (siz = 1; siz < fs->fs_frag; siz++) {
		if ((inblk & (1 << (siz + (fs->fs_frag % NBBY)))) == 0)
			continue;
		field = around[siz];
		subfield = inside[siz];
		for (pos = siz; pos <= fs->fs_frag; pos++) {
			if ((fragmap & field) == subfield) {
				fraglist[siz] += cnt;
				pos += siz;
				field <<= siz;
				subfield <<= siz;
			}
			field <<= 1;
			subfield <<= 1;
		}
	}
}

#ifdef KERNEL
/*
 * Check that a specified block number is in range.
 */
badblock(fs, bn)
	register struct fs *fs;
	daddr_t bn;
{

	if ((unsigned)bn >= fs->fs_size) {
		printf("bad block %d, ", bn);
		fserr(fs, "bad block");
		return (1);
	}
	return (0);
}
#endif

/*
 * block operations
 *
 * check if a block is available
 */
isblock(fs, cp, h)
	struct fs *fs;
	unsigned char *cp;
	daddr_t h;
{
	unsigned char mask;

	switch ((int)fs->fs_frag) {
	case 8:
		return (cp[h] == 0xff);
	case 4:
		mask = 0x0f << ((h & 0x1) << 2);
		return ((cp[h >> 1] & mask) == mask);
	case 2:
		mask = 0x03 << ((h & 0x3) << 1);
		return ((cp[h >> 2] & mask) == mask);
	case 1:
		mask = 0x01 << (h & 0x7);
		return ((cp[h >> 3] & mask) == mask);
	default:
		panic("isblock");
		return (NULL);
	}
}

/*
 * take a block out of the map
 */
clrblock(fs, cp, h)
	struct fs *fs;
	u_char *cp;
	daddr_t h;
{

	switch ((int)fs->fs_frag) {
	case 8:
		cp[h] = 0;
		return;
	case 4:
		cp[h >> 1] &= ~(0x0f << ((h & 0x1) << 2));
		return;
	case 2:
		cp[h >> 2] &= ~(0x03 << ((h & 0x3) << 1));
		return;
	case 1:
		cp[h >> 3] &= ~(0x01 << (h & 0x7));
		return;
	default:
		panic("clrblock");
	}
}

/*
 * put a block into the map
 */
setblock(fs, cp, h)
	struct fs *fs;
	unsigned char *cp;
	daddr_t h;
{

	switch ((int)fs->fs_frag) {

	case 8:
		cp[h] = 0xff;
		return;
	case 4:
		cp[h >> 1] |= (0x0f << ((h & 0x1) << 2));
		return;
	case 2:
		cp[h >> 2] |= (0x03 << ((h & 0x3) << 1));
		return;
	case 1:
		cp[h >> 3] |= (0x01 << (h & 0x7));
		return;
	default:
		panic("setblock");
	}
}

#ifdef KERNEL
#ifdef VICE
/* The routine igetfs, below, has been added for the ITC */

/* igetfs is identical to getfs, except that an error is returned rather than
 * panicing.  Used by icreate, etc.
 */
struct fs *
igetfs(dev)
	dev_t dev;
{
	register struct mount *mp;
	register struct fs *fs;

	for (mp = &mount[0]; mp < &mount[NMOUNT]; mp++) {
		if (mp->m_bufp == NULL || mp->m_dev != dev)
			continue;
		fs = mp->m_bufp->b_un.b_fs;
		if (fs->fs_magic != FS_MAGIC) {
			printf("dev = 0x%x, fs = %s\n", dev, fs->fs_fsmnt);
			panic("igetfs: bad magic");
		}
		return (fs);
	}
	u.u_error = ENXIO;
	return (NULL);
}
#endif	VICE

/*
 * Getfs maps a device number into a pointer to the incore super block.
 *
 * The algorithm is a linear search through the mount table. A
 * consistency check of the super block magic number is performed.
 *
 * panic: no fs -- the device is not mounted.
 *	this "cannot happen"
 */
struct fs *
getfs(dev)
	dev_t dev;
{
	register struct mount *mp;
	register struct fs *fs;

	for (mp = &mount[0]; mp < &mount[NMOUNT]; mp++) {
		if (mp->m_bufp == NULL || mp->m_dev != dev)
			continue;
		fs = mp->m_bufp->b_un.b_fs;
		if (fs->fs_magic != FS_MAGIC) {
			printf("dev = 0x%x, fs = %s\n", dev, fs->fs_fsmnt);
			panic("getfs: bad magic");
		}
		return (fs);
	}
	printf("dev = 0x%x\n", dev);
	panic("getfs: no fs");
	return (NULL);
}

/*
 * Getfsx returns the index in the file system
 * table of the specified device.  The swap device
 * is also assigned a pseudo-index.  The index may
 * be used as a compressed indication of the location
 * of a block, recording
 *	<getfsx(dev),blkno>
 * rather than
 *	<dev, blkno>
 * provided the information need remain valid only
 * as long as the file system is mounted.
 */
getfsx(dev)
	dev_t dev;
{
	register struct mount *mp;

#if	MACH
#else	MACH
	if (dev == swapdev)
		return (MSWAPX);
#endif	MACH
	for(mp = &mount[0]; mp < &mount[NMOUNT]; mp++)
		if (mp->m_dev == dev)
			return (mp - &mount[0]);
	return (-1);
}

/*
 * Print out statistics on the current allocation of the buffer pool.
 * Can be enabled to print out on every ``sync'' by setting "syncprt"
 * above.
 */
bufstats()
{
	int s, i, j, count;
	register struct buf *bp, *dp;
	int counts[MAXBSIZE/CLBYTES+1];
	static char *bname[BQUEUES] = { "LOCKED", "LRU", "AGE", "EMPTY" };

	for (bp = bfreelist, i = 0; bp < &bfreelist[BQUEUES]; bp++, i++) {
		count = 0;
		for (j = 0; j <= MAXBSIZE/CLBYTES; j++)
			counts[j] = 0;
		s = splbio();
		for (dp = bp->av_forw; dp != bp; dp = dp->av_forw) {
			counts[dp->b_bufsize/CLBYTES]++;
			count++;
		}
		splx(s);
		printf("%s: total-%d", bname[i], count);
		for (j = 0; j <= MAXBSIZE/CLBYTES; j++)
			if (counts[j] != 0)
				printf(", %d-%d", j * CLBYTES, counts[j]);
		printf("\n");
	}
}
#endif

#if !defined(vax) || defined(VAX630) || defined(VAX650)
			       /*
				* Scanc has mircocode assist on 650,
				* but we can do it this way in
				* case the emulation is not there.
				*/
/*
 * C definitions of special vax instructions.
 */
#ifdef	sun
#else	sun
scanc(size, cp, table, mask)
	u_int size;
	register u_char *cp, table[];
	register u_char mask;
{
	register u_char *end = &cp[size];

	while (cp < end && (table[*cp] & mask) == 0)
		cp++;
	return (end - cp);
}
#endif	sun

#endif
#if !defined(vax)

skpc(mask, size, cp)
	register u_char mask;
	u_int size;
	register u_char *cp;
{
	register u_char *end = &cp[size];

	while (cp < end && *cp == mask)
		cp++;
	return (end - cp);
}

locc(mask, size, cp)
	register u_char mask;
	u_int size;
	register u_char *cp;
{
	register u_char *end = &cp[size];

	while (cp < end && *cp != mask)
		cp++;
	return (end - cp);
}
#endif vax
