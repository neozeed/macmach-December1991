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
 * $Log:	emul_trap.s,v $
 * Revision 2.2  91/09/12  16:39:58  bohman
 * 	Created.
 * 	[91/09/11  14:29:51  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/emul_trap.s
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <mac2/asm.h>
#include <mac2/cpu.h>

#include <assym.s>

/*
 * Emulate A-line traps in
 * user mode.
 */
ENTRY(_ATRAP_EMULATION)
	movl	sp@+,a0@(PCB_A0)
	movl	d0,a0@(PCB_D0)
	movl	a1,a0@(PCB_A1)
	movl	usp,a1
	movl	a1,a0@(PCB_SP)

/*
 * Simulate the exception in user mode.
 *
 * Push the exception frame on the
 * user stack.
 */
	movl	#FC_UD,d0
	movc	d0,dfc

	movl	a0@(PCB_THREAD),a1
	movl	#Afault,a1@(THREAD_RECOVER)

	movl	a0@(PCB_SP),a1
	movw	sp@(F_VOR),d0
	movsw	d0,a1@-
	movl	sp@(F_PC),d0
	movsl	d0,a1@-
	movw	sp@(F_SR),d0
	movsw	d0,a1@-
	movl	a1,a0@(PCB_SP)

	movl	a0@(PCB_THREAD),a1
	clrl	a1@(THREAD_RECOVER)

/*
 * Change the PC in the return
 * frame to return to the trap handler.
 */
	movsl	0x28,d0
	movl	d0,sp@(F_PC)

	movl	a0@(PCB_SP),a1
	movl	a1,usp
	movl	a0@(PCB_D0),d0
	moveml	a0@(PCB_A0),a0-a1
	rte

/*
 * A fault occurred while pushing
 * the exception frame onto the user
 * stack.  Restore the user registers,
 * set the low order bit on the user PC
 * and return.
 */
Afault:
	bclr	#SR_TRACE_BIT,sp@(F_SR)
	movl	sp@(F_PC),d0
	bset	#0,d0
	movl	d0,sp@(F_PC)
	movc	msp,a0
	movl	a0@(PCB_SP),a1
	movl	a1,usp
	movl	a0@(PCB_D0),d0
	moveml	a0@(PCB_A0),a0-a1
	rte

/*
 * Emulate TRAP1 traps in
 * user mode.
 */
ENTRY(_TRAP1_EMULATION)
	movl	sp@+,a0@(PCB_A0)
	movl	d0,a0@(PCB_D0)
	movl	a1,a0@(PCB_A1)
	movl	usp,a1
	movl	a1,a0@(PCB_SP)

/*
 * Simulate the exception in user mode.
 *
 * Push the exception frame on the
 * user stack.
 */
	movl	#FC_UD,d0
	movc	d0,dfc

	movl	a0@(PCB_THREAD),a1
	movl	#Afault,a1@(THREAD_RECOVER)

	movl	a0@(PCB_SP),a1
	movw	sp@(F_VOR),d0
	movsw	d0,a1@-
	movl	sp@(F_PC),d0
	movsl	d0,a1@-
	movw	sp@(F_SR),d0
	movsw	d0,a1@-
	movl	a1,a0@(PCB_SP)

	movl	a0@(PCB_THREAD),a1
	clrl	a1@(THREAD_RECOVER)

/*
 * Change the PC in the return
 * frame to return to the trap handler.
 */
	movsl	0x84,d0
	movl	d0,sp@(F_PC)
	movl	a0@(PCB_SP),a1
	movl	a1,usp
	movl	a0@(PCB_D0),d0
	moveml	a0@(PCB_A0),a0-a1
	rte
