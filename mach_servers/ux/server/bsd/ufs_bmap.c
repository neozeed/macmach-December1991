/* 
 * Mach Operating System
 * Copyright (c) 1988 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/* 
 * HISTORY
 * $Log:	ufs_bmap.c,v $
 * Revision 2.4  90/11/05  15:34:02  rpd
 * 	Fixed to work w/o MACH_NBC.
 * 	[90/11/01            rwd]
 * 
 * Revision 2.3  90/06/19  23:09:39  rpd
 * 	Purged MACH and CMUCS conditionals.
 * 	Pending a better understanding of this mess,
 * 	changed bmap to ignore B_XXX.
 * 	[90/06/19            rpd]
 * 
 * Revision 2.2  90/05/29  20:23:38  rwd
 * 	Rfr code form limited inode writeback.
 * 	[90/04/12  16:11:50  rwd]
 * 
 * Revision 2.1  89/08/04  14:14:40  rwd
 * Created.
 * 
 * Revision 2.3  88/12/19  02:34:35  mwyoung
 * 	Always turn off B_XXX in the read/write flag argument to bmap().
 * 	[88/12/06            mwyoung]
 * 
 * Revision 2.2  88/08/24  01:34:23  mwyoung
 * 	Corrected include file references.
 * 	[88/08/22  22:05:32  mwyoung]
 * 
 *
 * 22-Apr-87  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	MACH_NBC:  Be sure to flush any pages potentially cached in the
 *	VM system when allocating new blocks to a file.  Actually,
 *	this is probably only necessary when a frag is extended, this
 *	can be verified later.
 *
 * 16-Jul-86  Bill Bolosky (bolosky) at Carnegie-Mellon University
 *	CS_GENERIC: changed size, nsize and osize variables in bmap from
 *	type into to u_long.
 *
 * 25-Jan-86  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Upgraded to 4.3.
 *
 */
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)ufs_bmap.c	7.1 (Berkeley) 6/5/86
 */

#include <mach_nbc.h>

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/inode.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/buf.h>
#include <sys/proc.h>
#include <sys/fs.h>

/*
 * Bmap defines the structure of file system storage
 * by returning the physical block number on a device given the
 * inode and the logical block number in a file.
 * When convenient, it also leaves the physical
 * block number of the next block of the file in rablock
 * for use in read-ahead.
 */
/*VARARGS3*/
daddr_t
bmap(ip, bn, rwflg, size)
	register struct inode *ip;
	daddr_t bn;
	int rwflg;
	u_long size;	/* supplied only when rwflg == B_WRITE */
{
	register int i;
	u_long osize, nsize;
	struct buf *bp, *nbp;
	struct fs *fs;
	int j, sh;
	daddr_t nb, lbn, *bap, pref, blkpref();
#if	MACH_NBC
	int	flush;
	boolean_t	clearbuf = TRUE;
#else	MACH_NBC
	boolean_t	clearbuf = FALSE;
#endif	MACH_NBC

	if (bn < 0) {
		u.u_error = EFBIG;
		return ((daddr_t)0);
	}
	fs = ip->i_fs;
	rablock = 0;
	rasize = 0;		/* conservative */

#if	MACH_NBC
#if	0
	/*
	 *	Pick up the flush indication that has been hacked into the 
	 *	read/write flag.
	 */
	flush = ((rwflg & B_XXX) != 0);
	if (flush && ((rwflg & B_WRITE) != 0)) clearbuf = TRUE;
#else	0
	flush = FALSE;
	clearbuf = TRUE;
#endif	0
#endif	MACH_NBC
	/* Turn off the bit, regardless whether we use it */
	rwflg &= ~B_XXX;

	/*
	 * If the next write will extend the file into a new block,
	 * and the file is currently composed of a fragment
	 * this fragment has to be extended to be a full block.
	 */
	nb = lblkno(fs, ip->i_size);
	if (rwflg == B_WRITE && nb < NDADDR && nb < bn) {
		osize = blksize(fs, ip, nb);
		if (osize < fs->fs_bsize && osize > 0) {
			bp = realloccg(ip, ip->i_db[nb],
				blkpref(ip, nb, (int)nb, &ip->i_db[0]),
				osize, (int)fs->fs_bsize);
			if (bp == NULL)
				return ((daddr_t)-1);
			ip->i_size = (nb + 1) * fs->fs_bsize;
			ip->i_db[nb] = dbtofsb(fs, bp->b_blkno);
			ip->i_flag |= IUPD|ICHG;
			bdwrite(bp);
#if	MACH_NBC
			if (flush)
				ino_flush(ip, nb * fs->fs_bsize,
					fs->fs_bsize);
#endif	MACH_NBC
		}
	}
	/*
	 * The first NDADDR blocks are direct blocks
	 */
	if (bn < NDADDR) {
		nb = ip->i_db[bn];
		if (rwflg == B_READ) {
			if (nb == 0)
				return ((daddr_t)-1);
			goto gotit;
		}
		if (nb == 0 || ip->i_size < (bn + 1) * fs->fs_bsize) {
			if (nb != 0) {
				/* consider need to reallocate a frag */
				osize = fragroundup(fs, blkoff(fs, ip->i_size));
				nsize = fragroundup(fs, size);
				if (nsize <= osize)
					goto gotit;
				bp = realloccg(ip, nb,
					blkpref(ip, bn, (int)bn, &ip->i_db[0]),
					osize, nsize);
			} else {
				if (ip->i_size < (bn + 1) * fs->fs_bsize)
					nsize = fragroundup(fs, size);
				else
					nsize = fs->fs_bsize;
				bp = alloc(ip,
					blkpref(ip, bn, (int)bn, &ip->i_db[0]),
					nsize, clearbuf);
			}
			if (bp == NULL)
				return ((daddr_t)-1);
			nb = dbtofsb(fs, bp->b_blkno);
			if ((ip->i_mode&IFMT) == IFDIR)
				/*
				 * Write directory blocks synchronously
				 * so they never appear with garbage in
				 * them on the disk.
				 */
				bwrite(bp);
			else
				bdwrite(bp);
#if	MACH_NBC
			if (flush)
				ino_flush(ip, bn*fs->fs_bsize, fs->fs_bsize);
#endif	MACH_NBC
			ip->i_db[bn] = nb;
			ip->i_flag |= IUPD|ICHG;
		}
gotit:
		if (bn < NDADDR - 1) {
			rablock = fsbtodb(fs, ip->i_db[bn + 1]);
			rasize = blksize(fs, ip, bn + 1);
		}
		return (nb);
	}

	/*
	 * Determine how many levels of indirection.
	 */
	pref = 0;
	sh = 1;
	lbn = bn;
	bn -= NDADDR;
	for (j = NIADDR; j>0; j--) {
		sh *= NINDIR(fs);
		if (bn < sh)
			break;
		bn -= sh;
	}
	if (j == 0) {
		u.u_error = EFBIG;
		return ((daddr_t)0);
	}

	/*
	 * fetch the first indirect block
	 */
	nb = ip->i_ib[NIADDR - j];
	if (nb == 0) {
		if (rwflg == B_READ)
			return ((daddr_t)-1);
		pref = blkpref(ip, lbn, 0, (daddr_t *)0);
	        bp = alloc(ip, pref, (int)fs->fs_bsize, TRUE);
		if (bp == NULL)
			return ((daddr_t)-1);
		nb = dbtofsb(fs, bp->b_blkno);
		/*
		 * Write synchronously so that indirect blocks
		 * never point at garbage.
		 */
		bwrite(bp);
#if	MACH_NBC
		/* no need to VM flush indirect blocks */
#endif	MACH_NBC
		ip->i_ib[NIADDR - j] = nb;
		ip->i_flag |= IUPD|ICHG;
	}

	/*
	 * fetch through the indirect blocks
	 */
	for (; j <= NIADDR; j++) {
		bp = bread(ip->i_dev, fsbtodb(fs, nb), (int)fs->fs_bsize);
		if (bp->b_flags & B_ERROR) {
			brelse(bp);
			return ((daddr_t)0);
		}
		bap = bp->b_un.b_daddr;
		sh /= NINDIR(fs);
		i = (bn / sh) % NINDIR(fs);
		nb = bap[i];
		if (nb == 0) {
			if (rwflg==B_READ) {
				brelse(bp);
				return ((daddr_t)-1);
			}
			if (pref == 0)
				if (j < NIADDR)
					pref = blkpref(ip, lbn, 0,
						(daddr_t *)0);
				else
					pref = blkpref(ip, lbn, i, &bap[0]);
		        nbp = alloc(ip, pref, (int)fs->fs_bsize, clearbuf);
			if (nbp == NULL) {
				brelse(bp);
				return ((daddr_t)-1);
			}
			nb = dbtofsb(fs, nbp->b_blkno);
			if (j < NIADDR || (ip->i_mode&IFMT) == IFDIR)
				/*
				 * Write synchronously so indirect blocks
				 * never point at garbage and blocks
				 * in directories never contain garbage.
				 */
				bwrite(nbp);
			else
				bdwrite(nbp);
			bap[i] = nb;
			bdwrite(bp);
#if	MACH_NBC
			if (flush)
				ino_flush(ip, lbn*fs->fs_bsize, fs->fs_bsize);
#endif	MACH_NBC
		} else
			brelse(bp);
	}

	/*
	 * calculate read-ahead.
	 */
	if (i < NINDIR(fs) - 1) {
		rablock = fsbtodb(fs, bap[i+1]);
		rasize = fs->fs_bsize;
	}
	return (nb);
}
