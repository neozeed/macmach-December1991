/* 
 * Mach Operating System
 * Copyright (c) 1991,1990,1989 Carnegie Mellon University
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 * 
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS "AS IS"
 * CONDITION.  CARNEGIE MELLON DISCLAIMS ANY LIABILITY OF ANY KIND FOR
 * ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
 * 
 * Carnegie Mellon requests users of this software to return to
 * 
 *  Software Distribution Coordinator  or  Software.Distribution@CS.CMU.EDU
 *  School of Computer Science
 *  Carnegie Mellon University
 *  Pittsburgh PA 15213-3890
 * 
 * any improvements or extensions that they make and grant Carnegie Mellon
 * the rights to redistribute these changes.
 */


/*
 * HISTORY
 * $Log:	cpu.h,v $
 * Revision 2.2  91/09/12  16:39:38  bohman
 * 	Created Mach 3.0 version from 2.5 version.
 * 	[91/09/11  14:27:30  bohman]
 * 
 * Revision 2.2  90/08/30  11:01:06  bohman
 * 	Created.
 * 	[90/08/29  11:31:40  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/cpu.h
 *	Author: David E. Bohman II (CMU macmach)
 */

#ifndef	_MACHINE_CPU_H_
#define	_MACHINE_CPU_H_

/*
 * cpu architecture defines
 */

#ifndef	ASSEMBLER
/*
 * An interrupt vector
 */
struct ivect {
    void	(*vector)();
};
#endif	ASSEMBLER

/*
 * Cache Control
 */
#define	CPU_ICACHE_ENABLE   0x0001
#define	CPU_ICACHE_FREEZE   0x0002
#define	CPU_ICACHE_CLRENTRY 0x0004
#define	CPU_ICACHE_CLEAR    0x0008
#define CPU_ICACHE_BURSTEN  0x0010
#define CPU_DCACHE_ENABLE   0x0100
#define CPU_DCACHE_FREEZE   0x0200
#define CPU_DCACHE_CLRENTRY 0x0400
#define CPU_DCACHE_CLEAR    0x0800
#define CPU_DCACHE_BURSTEN  0x1000
#define CPU_DCACHE_WRTALLOC 0x2000

/*
 * Value or'd with CACR to flush
 * both the instruction and data caches.
 */
#define CPU_CACHE_CLR	(CPU_DCACHE_CLEAR+CPU_ICACHE_CLEAR)

/*
 * Function code register values.
 */
#define	FC_UD	1		/* user data */
#define	FC_UP	2		/* user program */
#define	FC_SD	5		/* supervisor data */
#define	FC_SP	6		/* supervisor program */
#define	FC_CPU	7		/* cpu space */

#ifndef	ASSEMBLER
#define	set_cpu_number()

#define	cpu_number()	(0)

int	master_cpu;

extern struct ivect	*ivect_tbl, ivect_tbl_prototype[];
#endif	ASSEMBLER
#endif	_MACHINE_CPU_H_
