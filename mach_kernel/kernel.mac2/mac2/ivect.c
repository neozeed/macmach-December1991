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
 * $Log:	ivect.c,v $
 * Revision 2.2  91/09/12  16:40:48  bohman
 * 	Created Mach 3.0 version from 2.5 version.
 * 	[91/09/11  14:44:57  bohman]
 * 
 * Revision 2.2  90/08/30  11:01:36  bohman
 * 	Created.
 * 	[90/08/29  11:36:27  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/ivect.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <machine/cpu.h>

extern void	_BAD_ACCESS(), _TRACE(), _EMULATION_1010(), _EMULATION_1111();
extern void	_BAD_INSTRUCTION(), _PRIV_INSTRUCTION();
extern void	_BREAKPOINT(), _ARITHMETIC(), _SOFTWARE(), _ERROR();

extern void	_syscall();
extern void	_trap8();
extern void	_trap1();

extern void	_nmi();
extern void	_level1intr(), _level2intr(), _level4intr();

#define ACCESS	_BAD_ACCESS
#define TRACE	_TRACE
#define EMU1010 _EMULATION_1010
#define EMU1111	_EMULATION_1111
#define INSTR	_BAD_INSTRUCTION
#define PRIV	_PRIV_INSTRUCTION
#define BRKPT	_BREAKPOINT
#define ARITH	_ARITHMETIC
#define SOFTW	_SOFTWARE
#define ERR	_ERROR

#define COPY	((void(*)()) 1)

struct ivect	*ivect_tbl;

struct ivect	ivect_tbl_prototype[] = {
         ERR,         ERR,      ACCESS,       INSTR,	/* 0x000 - 0x00c */
       INSTR,       ARITH,       SOFTW,       SOFTW,	/* 0x010 - 0x01c */
        PRIV,       TRACE,     EMU1010,     EMU1111,	/* 0x020 - 0x02c */
         ERR,       INSTR,       INSTR,         ERR,	/* 0x030 - 0x03c */
         ERR,         ERR,         ERR,         ERR,	/* 0x040 - 0x04c */
         ERR,         ERR,         ERR,         ERR,	/* 0x050 - 0x05c */
         ERR, _level1intr, _level2intr,         ERR,	/* 0x060 - 0x06c */
 _level4intr,         ERR,	  COPY,        _nmi,	/* 0x070 - 0x07c */
    _syscall,	   _trap1,       INSTR,       INSTR,	/* 0x080 - 0x08c */
       INSTR,       INSTR,       INSTR,       INSTR,	/* 0x090 - 0x09c */
      _trap8,       INSTR,       INSTR,       INSTR,	/* 0x0a0 - 0x0ac */
       INSTR,       INSTR,       INSTR,       BRKPT,	/* 0x0b0 - 0x0bc */
       ARITH,       ARITH,       ARITH,       ARITH,	/* 0x0c0 - 0x0cc */
       ARITH,       ARITH,       ARITH,         ERR,	/* 0x0d0 - 0x0dc */
         ERR,         ERR,         ERR,         ERR,	/* 0x0e0 - 0x0ec */
         ERR,         ERR,         ERR,         ERR,	/* 0x0f0 - 0x0fc */
         ERR,         ERR,         ERR,         ERR,	/* 0x100 - 0x10c */
         ERR,         ERR,         ERR,         ERR,	/* 0x110 - 0x11c */
         ERR,         ERR,         ERR,         ERR,	/* 0x120 - 0x12c */
         ERR,         ERR,         ERR,         ERR,	/* 0x130 - 0x13c */
         ERR,         ERR,         ERR,         ERR,	/* 0x140 - 0x14c */
         ERR,         ERR,         ERR,         ERR,	/* 0x150 - 0x15c */
         ERR,         ERR,         ERR,         ERR,	/* 0x160 - 0x16c */
         ERR,         ERR,         ERR,         ERR,	/* 0x170 - 0x17c */
         ERR,         ERR,         ERR,         ERR,	/* 0x180 - 0x18c */
         ERR,         ERR,         ERR,         ERR,	/* 0x190 - 0x19c */
         ERR,         ERR,         ERR,         ERR,	/* 0x1a0 - 0x1ac */
         ERR,         ERR,         ERR,         ERR,	/* 0x1b0 - 0x1bc */
         ERR,         ERR,         ERR,         ERR,	/* 0x1c0 - 0x1cc */
         ERR,         ERR,         ERR,         ERR,	/* 0x1d0 - 0x1dc */
         ERR,         ERR,         ERR,         ERR,	/* 0x1e0 - 0x1ec */
         ERR,         ERR,         ERR,         ERR,	/* 0x1f0 - 0x1fc */
         ERR,         ERR,         ERR,         ERR,	/* 0x200 - 0x20c */
         ERR,         ERR,         ERR,         ERR,	/* 0x210 - 0x21c */
         ERR,         ERR,         ERR,         ERR,	/* 0x220 - 0x22c */
         ERR,         ERR,         ERR,         ERR,	/* 0x230 - 0x23c */
         ERR,         ERR,         ERR,         ERR,	/* 0x240 - 0x24c */
         ERR,         ERR,         ERR,         ERR,	/* 0x250 - 0x25c */
         ERR,         ERR,         ERR,         ERR,	/* 0x260 - 0x26c */
         ERR,         ERR,         ERR,         ERR,	/* 0x270 - 0x27c */
         ERR,         ERR,         ERR,         ERR,	/* 0x280 - 0x28c */
         ERR,         ERR,         ERR,         ERR,	/* 0x290 - 0x29c */
         ERR,         ERR,         ERR,         ERR,	/* 0x2a0 - 0x2ac */
         ERR,         ERR,         ERR,         ERR,	/* 0x2b0 - 0x2bc */
         ERR,         ERR,         ERR,         ERR,	/* 0x2c0 - 0x2cc */
         ERR,         ERR,         ERR,         ERR,	/* 0x2d0 - 0x2dc */
         ERR,         ERR,         ERR,         ERR,	/* 0x2e0 - 0x2ec */
         ERR,         ERR,         ERR,         ERR,	/* 0x2f0 - 0x2fc */
         ERR,         ERR,         ERR,         ERR,	/* 0x300 - 0x30c */
         ERR,         ERR,         ERR,         ERR,	/* 0x310 - 0x31c */
         ERR,         ERR,         ERR,         ERR,	/* 0x320 - 0x32c */
         ERR,         ERR,         ERR,         ERR,	/* 0x330 - 0x33c */
         ERR,         ERR,         ERR,         ERR,	/* 0x340 - 0x34c */
         ERR,         ERR,         ERR,         ERR,	/* 0x350 - 0x35c */
         ERR,         ERR,         ERR,         ERR,	/* 0x360 - 0x36c */
         ERR,         ERR,         ERR,         ERR,	/* 0x370 - 0x37c */
         ERR,         ERR,         ERR,         ERR,	/* 0x380 - 0x38c */
         ERR,         ERR,         ERR,         ERR,	/* 0x390 - 0x39c */
         ERR,         ERR,         ERR,         ERR,	/* 0x3a0 - 0x3ac */
         ERR,         ERR,         ERR,         ERR,	/* 0x3b0 - 0x3bc */
         ERR,         ERR,         ERR,         ERR,	/* 0x3c0 - 0x3cc */
         ERR,         ERR,         ERR,         ERR,	/* 0x3d0 - 0x3dc */
         ERR,         ERR,         ERR,         ERR,	/* 0x3e0 - 0x3ec */
         ERR,         ERR,         ERR,         ERR,	/* 0x3f0 - 0x3fc */
};

/*
 * Initialize the interrupt vector table.  This has
 * to be here because this is where the size of the
 * table is defined.
 */
ivect_init()
{
    register struct ivect	*itbl, *ptbl, *mtbl;
    register			i;

    itbl = ivect_tbl; ptbl = ivect_tbl_prototype; mtbl = 0;

    for (i = 0; i < (sizeof (ivect_tbl_prototype) /
		     sizeof (struct ivect)); i++) {
	if (ptbl[i].vector == COPY)
	    itbl[i] = mtbl[i];
	else
	    itbl[i] = ptbl[i];
    }
}
