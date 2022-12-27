/* 
 * Mach Operating System
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	exec.h,v $
 * Revision 2.2  89/11/29  15:29:34  af
 * 	Support coff images.  I did it for mips, but should work
 * 	for all.
 * 	[89/11/16  17:04:41  af]
 * 
 * Revision 2.1  89/08/04  14:45:11  rwd
 * Created.
 * 
 * 12-Sep-86  Jonathan J. Chew (jjc) at Carnegie-Mellon University
 *	Changed first long word to two shorts for the Sun to give
 *	the machine type along with the magic number.
 *	Added machine types for Sun.
 *	Made file includable more than once.
 *
 */
 
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)exec.h	7.1 (Berkeley) 6/4/86
 */

#ifndef	_EXEC_
#define	_EXEC_	1

/*
 * Header prepended to each a.out file.
 */
#ifndef	mips

struct exec {
#if	defined(sun) || defined(mac2)
unsigned short  a_machtype;     /* machine type */
unsigned short  a_magic;        /* magic number */
#else
	long	a_magic;	/* magic number */
#endif	defined(sun) || defined(mac2)
unsigned long	a_text;		/* size of text segment */
unsigned long	a_data;		/* size of initialized data */
unsigned long	a_bss;		/* size of uninitialized data */
unsigned long	a_syms;		/* size of symbol table */
unsigned long	a_entry;	/* entry point */
unsigned long	a_trsize;	/* size of text relocation */
unsigned long	a_drsize;	/* size of data relocation */
};

#else	!mips
#include <sys/coff.h>

struct exec {
	struct filehdr f;
	struct aouthdr a;
#define a_magic	a.magic		/* magic number */
#define a_text	a.tsize		/* size of text segment */
#define a_data	a.dsize		/* size of initialized data */
#define a_bss	a.bsize		/* size of uninitialized data */
#define a_syms	f.f_nsyms	/* size of symbol table */
#define a_entry	a.entry		/* entry point */
};

#endif	mips


#define	OMAGIC	0407		/* old impure format */
#define	NMAGIC	0410		/* read-only text */
#define	ZMAGIC	0413		/* demand load format */

#ifdef sun
/* Sun machine types */

#define M_OLDSUN2	0	/* old sun-2 executable files */
#define M_68010		1	/* runs on either 68010 or 68020 */
#define M_68020		2	/* runs only on 68020 */
#endif sun

#ifdef	mac2
#define M_68020		2	/* sun compatable: 68020/30 */
#define M_68040		3	/* runs only on 68040 */
#endif

#endif	_EXEC_
