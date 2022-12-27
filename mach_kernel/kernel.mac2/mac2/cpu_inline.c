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
 * $Log:	cpu_inline.c,v $
 * Revision 2.2  91/09/12  16:39:47  bohman
 * 	Created Mach 3.0 version from 2.5 version.
 * 	[91/09/11  14:28:18  bohman]
 * 
 * Revision 2.2  90/08/30  11:01:09  bohman
 * 	Created.
 * 	[90/08/29  11:32:19  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/cpu_inline.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#ifndef	_MACHINE_CPU_INLINE_C_
#define	_MACHINE_CPU_INLINE_C_

#include <machine/cpu.h>
#include <machine/psl.h>

/*
 * Inline functions dealing with
 * CPU details.
 */

/*
 * Return the address of the PCB for
 * the current thread.
 */
static inline
struct pcb *
current_thread_pcb()
{
    struct pcb *pcb;

    asm("movc	msp,%0" : "=r" (pcb) : );

    return (pcb);
}

/*
 * Flush both the instruction
 * and data caches.
 */
static inline
void
flush_cpu_caches()
{
    extern unsigned long	cache;

    asm("movc	%0,cacr" : : "d" (CPU_CACHE_CLR | cache));
}

/*
 * Flush cpu caches by option.
 */
static inline
void
flush_cpu_cache_opt(which)
unsigned long			which;
{
    extern unsigned long	cache;

    asm("movc	%0,cacr" : : "d" ((which & CPU_CACHE_CLR) | cache));
}

/*
 * Set the base of the interrupt
 * vector table
 */
static inline
void
set_vector_base(a)
unsigned long	a;
{
    asm("movc	%0,vbr" : : "r" (a));
}

/*
 * Functions for manipulating the
 * processor priority.
 */

/*
 * Canned  functions to set the
 * processor priority to a
 * compiled-in value.
 */
#define	define_spl(n, v)				\
static inline						\
int							\
spl##n()						\
{							\
    unsigned short r;					\
\
    asm("movw	sr,%0" : "=dm" (r) : );			\
\
    asm("movw	%0,sr" : :				\
	"i" ((unsigned short)(SR_SUPR | SR_IPL##v)));	\
\
    return (r & SR_IPL);				\
}

define_spl(7,		7)
define_spl(high,	7)
define_spl(hi,		7)
define_spl(vm,		7)
define_spl(imp,		7)
define_spl(sched,	7)

define_spl(6,		6)

define_spl(5,		5)

define_spl(4,		4)
define_spl(bio,		4)
define_spl(net,		4)
define_spl(tty,		4)

define_spl(3,		3)

define_spl(2,		2)

define_spl(1,		1)
define_spl(clock,	1)
define_spl(softclock,	1)

define_spl(0,		0)

/*
 * Set the processor priority
 * to the value passed in s.
 */
static inline
int
splx(s)
int s;
{
    unsigned short r;

    asm("movw	sr,%0" : "=dm" (r) : );

    asm("movw	%0,sr" : : "dm" ((unsigned short)(s|SR_SUPR)));

    return (r & SR_IPL);
}

#endif	_MACHINE_CPU_INLINE_C_
