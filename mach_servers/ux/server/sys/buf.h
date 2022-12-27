/* 
 * Mach Operating System
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	buf.h,v $
 * Revision 2.3  91/01/08  15:00:45  rpd
 * 	Added s_notavail(), a non-synchronizing version of notavail.
 * 	Kept the (now unused) old notavail() for reference.
 * 	[91/01/04  13:07:56  af]
 * 
 * Revision 2.2  90/06/02  15:25:00  rpd
 * 	Cleaned up conditionals; removed MACH, CMU, CMUCS, MACH_NO_KERNEL.
 * 	[90/04/28            rpd]
 * 	Converted to new IPC.
 * 
 * 	Out-of-kernel version.  Add b_reply_port; remove b_proc.
 * 	[89/01/11            dbg]
 * 
 * Revision 2.1  89/08/04  14:44:07  rwd
 * Created.
 * 
 * Revision 2.5  88/09/30  14:10:13  mrt
 * 	30-Sep-88 Mary R. Thompson (mrt) at Carnegie Mellon
 * 	made #include <mach.h> conditional on KERNEL
 * 	else #include <sys/features.h>
 * 	[88/09/30  13:37:16  mrt]
 * 
 * Revision 2.4  88/08/24  02:23:33  mwyoung
 * 	Adjusted include file references.
 * 	[88/08/17  02:10:04  mwyoung]
 * 
 *
 * 29-Mar-88  Michael Young (mwyoung) at Carnegie-Mellon University
 *	MACH: Removed unused variables.
 *
 */
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)buf.h	7.1 (Berkeley) 6/4/86
 */

#ifndef	_BUF_
#define	_BUF_

#ifdef mac2 /* need mach definitions */
#ifndef KERNEL
#include <mach.h>
#endif
#endif

#ifdef	KERNEL
#include <uxkern/import_mach.h>
#endif	KERNEL

/*
 * The header for buffers in the buffer pool and otherwise used
 * to describe a block i/o request is given here.  The routines
 * which manipulate these things are given in bio.c.
 *
 * Each buffer in the pool is usually doubly linked into 2 lists:
 * hashed into a chain by <dev,blkno> so it can be located in the cache,
 * and (usually) on (one of several) queues.  These lists are circular and
 * doubly linked for easy removal.
 *
 * There are currently three queues for buffers:
 *	one for buffers which must be kept permanently (super blocks)
 * 	one for buffers containing ``useful'' information (the cache)
 *	one for buffers containing ``non-useful'' information
 *		(and empty buffers, pushed onto the front)
 * The latter two queues contain the buffers which are available for
 * reallocation, are kept in lru order.  When not on one of these queues,
 * the buffers are ``checked out'' to drivers which use the available list
 * pointers to keep track of them in their i/o active queues.
 */

/*
 * Bufhd structures used at the head of the hashed buffer queues.
 * We only need three words for these, so this abbreviated
 * definition saves some space.
 */
struct bufhd
{
	long	b_flags;		/* see defines below */
	struct	buf *b_forw, *b_back;	/* fwd/bkwd pointer in chain */
};
struct buf
{
	long	b_flags;		/* too much goes here to describe */
	struct	buf *b_forw, *b_back;	/* hash chain (2 way street) */
	struct	buf *av_forw, *av_back;	/* position on free list if not BUSY */
#define	b_actf	av_forw			/* alternate names for driver queue */
#define	b_actl	av_back			/*    head - isn't history wonderful */
	long	b_bcount;		/* transfer count */
	long	b_bufsize;		/* size of allocated buffer */
#define	b_active b_bcount		/* driver queue head: drive active */
	short	b_error;		/* returned after I/O */
	dev_t	b_dev;			/* major+minor device name */
	union {
	    caddr_t b_addr;		/* low order core address */
	    int	*b_words;		/* words for clearing */
	    struct fs *b_fs;		/* superblocks */
	    struct csum *b_cs;		/* superblock summary information */
	    struct cg *b_cg;		/* cylinder group block */
	    struct dinode *b_dino;	/* ilist */
	    daddr_t *b_daddr;		/* indirect block */
	} b_un;
	daddr_t	b_blkno;		/* block # on device */
	long	b_resid;		/* words not transferred after error */
#define	b_errcnt b_resid		/* while i/o in progress: # retries */
	mach_port_t b_reply_port;	/* reply port for IO */
	int	(*b_iodone)();		/* function called by iodone */
	int	b_pfcent;		/* center page when swapping cluster */
};

#define	BQUEUES		4		/* number of free buffer queues */

#define	BQ_LOCKED	0		/* super-blocks &c */
#define	BQ_LRU		1		/* lru, useful buffers */
#define	BQ_AGE		2		/* rubbish */
#define	BQ_EMPTY	3		/* buffer headers with no memory */

#ifdef	KERNEL
#define	BUFHSZ	512
#define RND	(MAXBSIZE/DEV_BSIZE)
#if	((BUFHSZ&(BUFHSZ-1)) == 0)
#define	BUFHASH(dev, dblkno)	\
	((struct buf *)&bufhash[((int)(dev)+(((int)(dblkno))/RND))&(BUFHSZ-1)])
#else
#define	BUFHASH(dev, dblkno)	\
	((struct buf *)&bufhash[((int)(dev)+(((int)(dblkno))/RND)) % BUFHSZ])
#endif

struct	buf *buf;		/* the buffer pool itself */
char	*buffers;
int	nbuf;			/* number of buffer headers */
int	bufpages;		/* number of memory pages in the buffer pool */
struct	bufhd bufhash[BUFHSZ];	/* heads of hash lists */
struct	buf bfreelist[BQUEUES];	/* heads of available lists */

struct	buf *alloc();
struct	buf *realloccg();
struct	buf *baddr();
struct	buf *getblk();
struct	buf *geteblk();
struct	buf *getnewbuf();
struct	buf *bread();
struct	buf *breada();

unsigned minphys();
#endif	KERNEL

/*
 * These flags are kept in b_flags.
 */
#define	B_WRITE		0x000000	/* non-read pseudo-flag */
#define	B_READ		0x000001	/* read when I/O occurs */
#define	B_DONE		0x000002	/* transaction finished */
#define	B_ERROR		0x000004	/* transaction aborted */
#define	B_BUSY		0x000008	/* not on av_forw/back list */
#define	B_PHYS		0x000010	/* physical IO */
#define	B_XXX		0x000020	/* was B_MAP, alloc UNIBUS on pdp-11 */
#define	B_WANTED	0x000040	/* issue wakeup when BUSY goes off */
#define	B_AGE		0x000080	/* delayed write for correct aging */
#define	B_ASYNC		0x000100	/* don't wait for I/O completion */
#define	B_DELWRI	0x000200	/* write at exit of avail list */
#define	B_TAPE		0x000400	/* this is a magtape (no bdwrite) */
#define	B_UAREA		0x000800	/* add u-area to a swap operation */
#define	B_PAGET		0x001000	/* page in/out of page table space */
#define	B_DIRTY		0x002000	/* dirty page to be pushed out async */
#define	B_PGIN		0x004000	/* pagein op, so swap() can count it */
#define	B_CACHE		0x008000	/* did bread find us in the cache ? */
#define	B_INVAL		0x010000	/* does not contain valid info  */
#define	B_LOCKED	0x020000	/* locked in core (not reusable) */
#define	B_HEAD		0x040000	/* a buffer header, not a buffer */
#define	B_BAD		0x100000	/* bad block revectoring in progress */
#define	B_CALL		0x200000	/* call b_iodone from iodone */

/*
 * Insq/Remq for the buffer hash lists.
 */
#define	bremhash(bp) { \
	(bp)->b_back->b_forw = (bp)->b_forw; \
	(bp)->b_forw->b_back = (bp)->b_back; \
}
#define	binshash(bp, dp) { \
	(bp)->b_forw = (dp)->b_forw; \
	(bp)->b_back = (dp); \
	(dp)->b_forw->b_back = (bp); \
	(dp)->b_forw = (bp); \
}

/*
 * Insq/Remq for the buffer free lists.
 */
#define	bremfree(bp) { \
	(bp)->av_back->av_forw = (bp)->av_forw; \
	(bp)->av_forw->av_back = (bp)->av_back; \
}
#define	binsheadfree(bp, dp) { \
	(dp)->av_forw->av_back = (bp); \
	(bp)->av_forw = (dp)->av_forw; \
	(dp)->av_forw = (bp); \
	(bp)->av_back = (dp); \
}
#define	binstailfree(bp, dp) { \
	(dp)->av_back->av_forw = (bp); \
	(bp)->av_back = (dp)->av_back; \
	(dp)->av_back = (bp); \
	(bp)->av_forw = (dp); \
}

/*
 * Take a buffer off the free list it's on and
 * mark it as being use (B_BUSY) by a device.
 */
#define	s_notavail(bp) { \
	bremfree(bp); \
	(bp)->b_flags |= B_BUSY; \
}	

#define	notavail(bp) { \
	int x = splbio(); \
	s_notavail(bp); \
	splx(x); \
}

#define	iodone	biodone
#define	iowait	biowait

/*
 * Zero out a buffer's data portion.
 */
#define	clrbuf(bp) { \
	blkclr((bp)->b_un.b_addr, (unsigned)(bp)->b_bcount); \
	(bp)->b_resid = 0; \
}

#endif	_BUF_
