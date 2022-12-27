/* 
 * Mach Operating System
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 **********************************************************************
 * HISTORY
 * $Log:	systm.h,v $
 * Revision 2.2  90/06/02  15:25:54  rpd
 * 	Cleaned up conditionals; removed MACH, CMU, CMUCS, MACH_NO_KERNEL.
 * 	[90/04/28            rpd]
 * 
 * 	Out-of-kernel version.  Remove include of cpus.h.
 * 	[89/01/05            dbg]
 * 
 * Revision 2.1  89/08/04  14:46:22  rwd
 * Created.
 * 
 * Revision 2.4  88/08/24  02:45:36  mwyoung
 * 	Adjusted include file references.
 * 	[88/08/17  02:23:41  mwyoung]
 * 
 *
 * 29-Mar-88  Michael Young (mwyoung) at Carnegie-Mellon University
 *	MACH: Eliminated unused variables.
 *
 * 19-Mar-87  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Make redefinitions of insque/remque always ifdef'ed on lint.
 *
 *  1-Feb-87  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Moved runrun and curpri definitions to sched.h for MACH.
 *
 * 19-Jan-86  David L. Black (dlb) at Carnegie-Mellon University
 *	MACH: runrun and curpri are now per cpu.  Removed
 *	runin, runout, wantin, and noproc.
 *
 * 17-Dec-86  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Removed declaration of icode under MACH.
 *
 * 12-Nov-86  David L. Black (dlb) at Carnegie-Mellon University
 *	ns32000: #ifdef lint'ed insque and remque casts.  These bogosities
 *	ought to be permanently #ifdef lint'ed for all systems.
 *
 *  7-Oct-86  David L. Black (dlb) at Carnegie-Mellon University
 *	Merged Multimax changes.
 *
 * 24-Sep-86  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Changed to directly import declaration of boolean.
 *
 * 29-Aug-86  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Converted from "bool" type to "boolean_t" where necessary.
 *
 * 16-Jul-86  Bill Bolosky (bolosky) at Carnegie-Mellon University
 *	MACH: Removed definitions of swapdev and argdev.
 *
 * 22-Mar-86  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Merged VM and Romp versions.
 *
 * 18-Feb-86  Bill Bolosky (bolosky) at Carnegie-Mellon University
 *	romp: Added new definition of icode[].
 *
 * 25-Jan-86  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Upgraded to 4.3.
 *
 *  4-Nov-85  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Add sy_parallel flag to the system call entries to specify
 *	whether or not the system call can be executed in parallel.
 *
 * 03-Aug-85  Mike Accetta (mja) at Carnegie-Mellon University
 *	CS_RPAUSE:  Added rpause() and fspause() declarations.
 *
 * 20-Jun-85  Mike Accetta (mja) at Carnegie-Mellon University
 *	CMUCS:  Added bootdev definition.
 *	[V1(1)]
 *
 **********************************************************************
 */
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)systm.h	7.1 (Berkeley) 6/4/86
 */

#ifndef	_SYS_SYSTM_H_
#define	_SYS_SYSTM_H_

#ifdef	KERNEL
#include <uxkern/import_mach.h>
#endif	KERNEL

#ifdef mac2 /* need mach boolean definitions */
#ifndef KERNEL
#include <mach/boolean.h>
#endif
#endif

/*
 * Random set of variables
 * used by more than one
 * routine.
 */
extern	char version[];		/* system version */

/*
 * Nblkdev is the number of entries
 * (rows) in the block switch. It is
 * set in binit/bio.c by making
 * a pass over the switch.
 * Used in bounds checking on major
 * device numbers.
 */
int	nblkdev;

/*
 * Number of character switch entries.
 * Set by cinit/prim.c
 */
int	nchrdev;

int	mpid;			/* generic for unique process id's */
char	kmapwnt;		/* Make #if cleaner */

int	maxmem;			/* actual max memory per process */
int	physmem;		/* physical memory on this CPU */

int	updlock;		/* lock for sync */
daddr_t	rablock;		/* block to be read ahead */
int	rasize;			/* size of block in rablock */
extern	int intstack[];		/* stack for interrupts */
dev_t	rootdev;		/* device of the root */
dev_t	dumpdev;		/* device to take dumps on */
long	dumplo;			/* offset into dumpdev */

daddr_t	bmap();
#ifndef mac2 /* calloc() definition conflicts with that in cthreads.h */
caddr_t	calloc();
#endif
boolean_t	rpause();
boolean_t	fspause();

/*
 * Structure of the system-entry table
 */
extern struct sysent
{
	short	sy_narg;		/* total number of arguments */
	short	sy_parallel;		/* can execute in parallel */
	int	(*sy_call)();		/* handler */
} sysent[];

char	*panicstr;
int	boothowto;		/* reboot flags, from console subsystem */
int	show_space;
u_long	bootdev;		/* boot device, from bootstrap subsystem */
int	selwait;

#ifdef	lint
/* casts to keep lint happy */
#define	insque(q,p)	_insque((caddr_t)q,(caddr_t)p)
#define	remque(q)	_remque((caddr_t)q)
#endif	lint

#endif	_SYS_SYSTM_H_
