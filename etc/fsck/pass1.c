/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */
/* 3-Feb-89  Zon Williamson (zon) at Carnegie Mellon University
 *	Added AFS compatability for mac2, clear MACH paging inodes.
 */

#ifndef lint
static char sccsid[] = "@(#)pass1.c	5.4 (Berkeley) 4/9/87";
#endif not lint

#include <sys/param.h>
#include <sys/inode.h>
#include <sys/fs.h>
#include "fsck.h"

static daddr_t badblk;
static daddr_t dupblk;
int pass1check();

pass1()
{
	register int c, i, j;
	register DINODE *dp;
	struct zlncnt *zlnp;
	int ndb, partial, cgd;
	struct inodesc idesc;
	ino_t inumber;
#ifdef mac2
	int pagingi;

	pagingi = 0;
#endif

	/*
	 * Set file system reserved blocks in used block map.
	 */
	for (c = 0; c < sblock.fs_ncg; c++) {
		cgd = cgdmin(&sblock, c);
		if (c == 0) {
			i = cgbase(&sblock, c);
			cgd += howmany(sblock.fs_cssize, sblock.fs_fsize);
		} else
			i = cgsblock(&sblock, c);
		for (; i < cgd; i++)
			setbmap(i);
	}
	/*
	 * Find all allocated blocks.
	 */
	bzero((char *)&idesc, sizeof(struct inodesc));
	idesc.id_type = ADDR;
	idesc.id_func = pass1check;
	inumber = 0;
#ifdef mac2
	n_files = nViceFiles = n_blks = 0;
#else
	n_files = n_blks = 0;
#endif
	for (c = 0; c < sblock.fs_ncg; c++) {
		for (i = 0; i < sblock.fs_ipg; i++, inumber++) {
			if (inumber < ROOTINO)
				continue;
			dp = ginode(inumber);
#ifdef mac2
			/* ignore garbage inodes created by MACH system */
			if (inumber > ROOTINO && dp->di_mode == IFREG && dp->di_nlink == 0) {
				if (debug) {
					pwarn("PAGING I=%u ",inumber);
					pinode(inumber);
					printf("\n");
				}
				pagingi++;
				zapino(dp);
				inodirty();
			}
#endif
			if (!ALLOC(dp)) {
				if (bcmp((char *)dp->di_db, (char *)zino.di_db,
					NDADDR * sizeof(daddr_t)) ||
				    bcmp((char *)dp->di_ib, (char *)zino.di_ib,
					NIADDR * sizeof(daddr_t)) ||
				    dp->di_mode || dp->di_size) {
					pfatal("PARTIALLY ALLOCATED INODE I=%u",
						inumber);
					if (reply("CLEAR") == 1) {
						zapino(dp);
						inodirty();
					}
				}
				statemap[inumber] = USTATE;
				continue;
			}
#ifdef mac2
			/* done below */
#else
			lastino = inumber;
#endif
			if (dp->di_size < 0 ||
			    dp->di_size + sblock.fs_bsize - 1 < 0) {
				if (debug)
					printf("bad size %d:", dp->di_size);
				goto unknown;
			}
#ifdef mac2
			/* print out the bad block inode and number */
			if ((dp->di_mode & IFMT) == IFMT)
				pfatal("BAD BLOCK I=%d, BLK=%d\n",
					inumber, dp->di_db[0]);
#endif
			if (!preen && (dp->di_mode & IFMT) == IFMT &&
			    reply("HOLD BAD BLOCK") == 1) {
#ifdef mac2
				/* don't let users or dump read it */
				dp->di_size = 0;
#else
				dp->di_size = sblock.fs_fsize;
#endif
				dp->di_mode = IFREG|0600;
				inodirty();
			}
#ifdef mac2
/* We use special bad block files which have a zero length but
 * have the first disk block pointer pointing to a bad spot. This
 * mechanism allows dump to run normally and skip reading the bad
 * block but avoids putting the bad block in the free list. The bad
 * block file should really have a special file type but that is a
 * bigger change.
 */
#else

			ndb = howmany(dp->di_size, sblock.fs_bsize);
			if (SPECIAL(dp))
				ndb++;
			for (j = ndb; j < NDADDR; j++)
				if (dp->di_db[j] != 0) {
					if (debug)
						printf("bad direct addr: %d\n",
							dp->di_db[j]);
					goto unknown;
				}
			for (j = 0, ndb -= NDADDR; ndb > 0; j++)
				ndb /= NINDIR(&sblock);
			for (; j < NIADDR; j++)
				if (dp->di_ib[j] != 0) {
					if (debug)
						printf("bad indirect addr: %d\n",
							dp->di_ib[j]);
					goto unknown;
				}
#endif
			if (ftypeok(dp) == 0)
				goto unknown;
#ifdef mac2
			/* count the inode as being used */
			lastino = inumber;
#endif
			n_files++;
			lncntp[inumber] = dp->di_nlink;
			if (dp->di_nlink <= 0) {
				zlnp = (struct zlncnt *)malloc(sizeof *zlnp);
				if (zlnp == NULL) {
					pfatal("LINK COUNT TABLE OVERFLOW");
					if (reply("CONTINUE") == 0)
						errexit("");
				} else {
					zlnp->zlncnt = inumber;
					zlnp->next = zlnhead;
					zlnhead = zlnp;
				}
			}
#ifdef mac2
			statemap[inumber] = DIRCT(dp) ? DSTATE : VICEINODE(dp) ? VSTATE: FSTATE;
#else
			statemap[inumber] = DIRCT(dp) ? DSTATE : FSTATE;
#endif
			badblk = dupblk = 0; maxblk = 0;
			idesc.id_number = inumber;
			(void)ckinode(dp, &idesc);
			idesc.id_entryno *= btodb(sblock.fs_fsize);
			if (dp->di_blocks != idesc.id_entryno) {
				pwarn("INCORRECT BLOCK COUNT I=%u (%ld should be %ld)",
				    inumber, dp->di_blocks, idesc.id_entryno);
				if (preen)
					printf(" (CORRECTED)\n");
				else if (reply("CORRECT") == 0)
					continue;
				dp->di_blocks = idesc.id_entryno;
				inodirty();
			}
			continue;
	unknown:
			pfatal("UNKNOWN FILE TYPE I=%u", inumber);
			statemap[inumber] = FCLEAR;
			if (reply("CLEAR") == 1) {
				statemap[inumber] = USTATE;
				zapino(dp);
				inodirty();
			}
		}
	}
#ifdef mac2
	if (pagingi > 0)
		pwarn("PAGING INODES = %d (ITOTAL %d)\n",pagingi,
		      sblock.fs_ncg * sblock.fs_ipg);
#endif
}

pass1check(idesc)
	register struct inodesc *idesc;
{
	int res = KEEPON;
	int anyout, nfrags;
	daddr_t blkno = idesc->id_blkno;
	register struct dups *dlp;
	struct dups *new;

	if ((anyout = outrange(blkno, idesc->id_numfrags)) != 0) {
		blkerr(idesc->id_number, "BAD", blkno);
		if (++badblk >= MAXBAD) {
			pwarn("EXCESSIVE BAD BLKS I=%u",
				idesc->id_number);
			if (preen)
				printf(" (SKIPPING)\n");
			else if (reply("CONTINUE") == 0)
				errexit("");
			return (STOP);
		}
	}
	for (nfrags = idesc->id_numfrags; nfrags > 0; blkno++, nfrags--) {
		if (anyout && outrange(blkno, 1)) {
			res = SKIP;
		} else if (!getbmap(blkno)) {
			n_blks++;
			setbmap(blkno);
		} else {
			blkerr(idesc->id_number, "DUP", blkno);
			if (++dupblk >= MAXDUP) {
				pwarn("EXCESSIVE DUP BLKS I=%u",
					idesc->id_number);
				if (preen)
					printf(" (SKIPPING)\n");
				else if (reply("CONTINUE") == 0)
					errexit("");
				return (STOP);
			}
			new = (struct dups *)malloc(sizeof(struct dups));
			if (new == NULL) {
				pfatal("DUP TABLE OVERFLOW.");
				if (reply("CONTINUE") == 0)
					errexit("");
				return (STOP);
			}
			new->dup = blkno;
			if (muldup == 0) {
				duplist = muldup = new;
				new->next = 0;
			} else {
				new->next = muldup->next;
				muldup->next = new;
			}
			for (dlp = duplist; dlp != muldup; dlp = dlp->next)
				if (dlp->dup == blkno)
					break;
			if (dlp == muldup && dlp->dup != blkno)
				muldup = new;
		}
		/*
		 * count the number of blocks found in id_entryno
		 */
		idesc->id_entryno++;
	}
	return (res);
}
