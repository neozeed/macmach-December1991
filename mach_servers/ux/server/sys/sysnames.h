/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */

/*
 * HISTORY
 * $Log:	sysnames.h,v $
 * Revision 2.4  91/09/03  11:13:28  jsb
 * 	From Intel SSD: Added i860 case.
 * 	[91/09/02  14:18:28  jsb]
 * 
 * Revision 2.3  90/05/21  13:59:20  dbg
 * 	Add i386.
 * 	[90/03/14            dbg]
 * 
 * Revision 2.2  90/01/23  00:05:27  af
 * 	Created.
 * 	[89/12/15            af]
 * 
 */

/*
 *	Definitions for macros that namei expands,
 *	strictly machine-dependent.
 */

#define	ATSYS	"@sys"		/* expands to SYSNAME */
#define	ATCPU	"@cpu"		/* expands to CPUNAME */

#ifdef	vax
#define	CPUNAME	"vax"
#define	SYSNAME	"vax_mach"
#endif	vax

#ifdef	sun3
#define	CPUNAME	"mc68000"
#define	SYSNAME	"sun3_mach"
#endif	sun3

#ifdef	mips
#define	CPUNAME	"mips"
#ifdef	PMAX
#define	SYSNAME	"pmax_mach"
#endif	PMAX
#ifdef	MSERIES
#define	SYSNAME	"mseries_mach"
#endif	MSERIES
#endif	mips

#ifdef	i386
#define	CPUNAME	"i386"
#define	SYSNAME	"i386_mach"
#endif	i386

#ifdef	i860
#define	CPUNAME	"i860"
#define	SYSNAME	"i860_mach"
#endif	i860

#ifdef	mac2
#define CPUNAME "mac2"
#define SYSNAME "mac2_mach"
#endif	mac2

#define SYSNAMELEN	(sizeof SYSNAME -1)
#define CPUNAMELEN	(sizeof CPUNAME -1)
