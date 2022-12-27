/*	filsys.h	4.3	81/03/03	*/

/*
 * Structure of the super-block
 */

#define	BSIZE	(1024)		/* 1024-byte block file system */
#define	NICFREE	178		/* 1024-byte block file system */
#define	NICINOD	100		/* 1024-byte block file system */
#define	SUPERB	((daddr_t)1)	/* block number of the super block */

struct	filsys
{
	unsigned short s_isize;		/* size in blocks of i-list */
	daddr_t	s_fsize;   		/* size in blocks of entire volume */
	short	s_nfree;   		/* number of addresses in s_free */
	daddr_t	s_free[NICFREE];	/* free block list */
	short	s_ninode;  		/* number of i-nodes in s_inode */
	u_short	s_inode[NICINOD];	/* free i-node list */
	char   	s_flock;   		/* lock during free list manipulation */
	char   	s_ilock;   		/* lock during i-list manipulation */
	char   	s_xfmod;    		/* (was) super block modified flag */
	char   	s_xronly;   		/* (was) mounted read-only flag */
	time_t 	s_time;    		/* last super block update */
	daddr_t	s_tfree;   		/* total free blocks*/
	u_short	s_tinode;  		/* total free inodes */
	short	s_dinfo[2];		/* interleave stuff */
#define	s_m	s_dinfo[0]
#define	s_n	s_dinfo[1]
	char   	s_fsmnt[12];		/* ordinary file mounted on */
	/* end not maintained */
	u_short	s_lasti;		/* start place for circular search */
	u_short	s_nbehind;		/* est # free inodes before s_lasti */
};

struct fblk
{
	int    	df_nfree;
	daddr_t	df_free[NICFREE];
};

typedef	struct fblk *FBLKP;
