/* 
 **********************************************************************
 * Mach Operating System
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 **********************************************************************
 * HISTORY
 * $Log:	subr_xxx.c,v $
 * Revision 2.1  89/08/04  14:10:49  rwd
 * Created.
 * 
 *  5-Jan-89  David Golub (dbg) at Carnegie-Mellon University
 *	Out-of-kernel version.
 *
 *  3-Apr-88  Michael Young (mwyoung) at Carnegie-Mellon University
 *	MACH: Don't include "sys/vm.h".
 *
 * 30-Dec-87  David Golub (dbg) at Carnegie-Mellon University
 *	Delinted.
 *
 * 07-Oct-87  Mike Accetta (mja) at Carnegie-Mellon University
 *	Change to panic on strlen(NULL) until this is determined to be
 *	a feature instead of a bug.
 *	[ V5.1(XF18) ]
 *
 * 05-May-87  Jonathan J. Chew (jjc) at Carnegie-Mellon University
 *	Fixed strlen() so it won't memory fault if it is passed
 *	a null pointer.
 *
 * 12-Jan-87  David Golub (dbg) at Carnegie-Mellon University
 *
 * 08-Jan-87  Robert Beck (beck) at Sequent Computer Systems, Inc.
 *	Include cputypes.h.
 *	Don't compile calloc() if BALANCE -- this is used differently.
 *	Don't compile ffs(), bcmp(), and strlen() for BALANCE.
 *
 * 23-Oct-86  Jonathan J. Chew (jjc) at Carnegie-Mellon University
 *	Added Sun to list of machines that need definition of imin(),
 *	min(), etc..
 *
 *  7-Jul-86  Bill Bolosky (bolosky) at Carnegie-Mellon University
 *	MACH: (negatively) Conditionalized include of pte.h
 *
 * 25-Feb-86  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	New calloc routine using VM routines.
 *
 * 25-Jan-86  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Upgraded to 4.3.
 *
 * 16-Nov-85  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	CMUCS:  Fixed off by one error in ffs.
 *
 **********************************************************************
 */
#include <mach_no_kernel.h>

#include "cputypes.h"

#include "cmucs.h"
#include "mach.h"
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)subr_xxx.c	7.1 (Berkeley) 6/5/86
 */

#if	MACH
#if	MACH_NO_KERNEL
#include <uxkern/import_mach.h>
#else	MACH_NO_KERNEL
#include "vm/vm_map.h"
#include "vm/vm_kern.h"
#endif	MACH_NO_KERNEL
#else	MACH
#include "machine/pte.h"
#endif	MACH

#include "sys/param.h"
#include "sys/systm.h"
#include "sys/conf.h"
#include "sys/inode.h"
#include "sys/dir.h"
#include "sys/user.h"
#include "sys/buf.h"
#include "sys/proc.h"
#include "sys/fs.h"
#if	MACH
#else	MACH
#include "sys/vm.h"
#include "sys/cmap.h"
#endif	MACH
#include "sys/uio.h"

/*
 * Routine placed in illegal entries in the bdevsw and cdevsw tables.
 */
nodev()
{

	return (ENODEV);
}

/*
 * Null routine; placed in insignificant entries
 * in the bdevsw and cdevsw tables.
 */
nulldev()
{

	return (0);
}

#if defined(romp) || defined(ns32000) || defined(sun)
#define	notdef
#endif	romp || ns32000 || sun
#ifdef notdef
imin(a, b)
{

	return (a < b ? a : b);
}

imax(a, b)
{

	return (a > b ? a : b);
}

unsigned
min(a, b)
	u_int a, b;
{

	return (a < b ? a : b);
}

unsigned
max(a, b)
	u_int a, b;
{

	return (a > b ? a : b);
}
#endif notdef
#if defined(romp) || defined(ns32000) || defined(sun)
#undef	notdef
#endif	romp || ns32000 || sun

#if	MACH_NO_KERNEL
#else	MACH_NO_KERNEL
#if	MACH
#if	BALANCE
/*
 * Sequent Balance kernel uses calloc() differently.  See sqt/startup.c.
 */
#else	BALANCE
caddr_t calloc(size)
	int size;
{
	return((caddr_t)kmem_alloc(kernel_map, (vm_offset_t)size));
}
#endif	BALANCE
#else	MACH
extern	cabase, calimit;
extern	struct pte camap[];

caddr_t	cacur = (caddr_t)&cabase;
caddr_t	camax = (caddr_t)&cabase;
int	cax = 0;
/*
 * This is a kernel-mode storage allocator.
 * It is very primitive, currently, in that
 * there is no way to give space back.
 * It serves, for the time being, the needs of
 * auto-configuration code and the like which
 * need to allocate some stuff at boot time.
 */
caddr_t
calloc(size)
	int size;
{
	register caddr_t res;
	register int i;

	if (cacur+size >= (caddr_t)&calimit)
		panic("calloc");
	while (cacur+size > camax) {
		(void) vmemall(&camap[cax], CLSIZE, &proc[0], CSYS);
		vmaccess(&camap[cax], camax, CLSIZE);
		for (i = 0; i < CLSIZE; i++)
			clearseg(camap[cax++].pg_pfnum);
		camax += NBPG * CLSIZE;
	}
	res = cacur;
	cacur += size;
	return (res);
}
#endif	MACH
#endif	MACH_NO_KERNEL

#ifdef GPROF
/*
 * Stub routine in case it is ever possible to free space.
 */
cfreemem(cp, size)
	caddr_t cp;
	int size;
{
	printf("freeing %x, size %d\n", cp, size);
}
#endif

#ifndef vax
#if	BALANCE
#else	BALANCE
ffs(mask)
	register long mask;
{
	register int i;

#if	CMUCS
	for(i = 1; i <= NSIG; i++) {
#else	CMUCS
	for(i = 1; i < NSIG; i++) {
#endif	CMUCS
		if (mask & 1)
			return (i);
		mask >>= 1;
	}
	return (0);
}

bcmp(s1, s2, len)
	register char *s1, *s2;
	register int len;
{

	while (len--)
		if (*s1++ != *s2++)
			return (1);
	return (0);
}

strlen(s1)
	register char *s1;
{
	register int len;

	if (s1 == 0)
		panic("strlen(NULL)");
	for (len = 0; *s1++ != '\0'; len++)
		/* void */;
	return (len);
}
#endif	BALANCE
#endif	vax
