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
 * $Log:	locore.s,v $
 * Revision 2.2  91/09/12  16:40:59  bohman
 * 	Created Mach 3.0 version from 2.5 version.
 * 	[91/09/11  14:45:54  bohman]
 * 
 * Revision 2.2  90/09/04  17:21:14  bohman
 * 	Created.
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/locore.s
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <mach/mac2/vm_param.h>

#include <mac2/asm.h>
#include <mac2/cpu.h>
#include <mac2/psl.h>
#include <mac2/trap.h>

#include <mac2os/Globals.h>

#include <assym.s>

#define halt	trap \#15

/*
 * locore for MACMACH
 */

	.data

/*
 * This is not really the interrupt
 * stack.  It is called this to
 * satisfy an external reference to
 * intstack (bsd/init_main.c).  Actually,
 * it is the first thing in the data segment,
 * and serves as a buffer so that none
 * of the data variables end up in a
 * read only text page.  The application
 * stack is used as the interrupt stack.
 */
	.globl	_intstack
_intstack:
	.space	MAC2_PGBYTES

/*
 * Some globals.
 */
	.globl	_mem_size
	.globl	_is68030
#ifdef MODE24
	.globl	_mac32bit
#endif /* MODE24 */
	.globl	_cache
_mem_size:	.long	0
_is68030:	.long	0
#ifdef MODE24
_mac32bit:	.long	0
#endif /* MODE24 */
_cache:		.long	0

	.globl	_bootdev
	.globl	_boothowto
	.globl	_ramdisk_image
	.globl	_ramdisk_size
_bootdev:	.long	0
_boothowto:	.long	0
_ramdisk_image:	.long	0
_ramdisk_size:	.long	0

/*
 * Macro used for entering
 * the trap handler.
 */
#define TRAP(type)	\
	.globl	__##type;	\
__##type: ;	\
	btst	\#SR_SUPR_BIT,sp@;	\
	bne	1f;	\
	movl	a0,sp@-;	\
	movc	msp,a0;	\
	moveml	d0-d7/a0-a6,a0@(PCB_REGS);	\
	movl	sp@+,a0@(PCB_A0);	\
	movl	usp,a1;	\
	movl	a1,a0@(PCB_SP);	\
	pea	sp@;	\
	movl	\#TRAP_##type,sp@-;	\
	jsr	_user_trap;	\
	addqw	\#8,sp;	\
	jmp	rte_user;	\
\
1:	moveml	d0-d7/a0-a7,sp@-;	\
	lea	sp@(R_SP+4),a0;	\
	pea	sp@;	\
	movl	a0,sp@-;	\
	movl	\#TRAP_##type,sp@-;	\
	jsr	_kernel_trap;	\
	addw	\#12,sp;	\
	jmp	rte_kernel

	.globl	_start
	.text
/*
 * The new application zone
 * ends right before the kernel
 * image begins.
 */
	.globl	applstack
applstack:

/*
 * Program entry point
 */
_start:	clrl	d0
	movl	d0,a0
	movl	#_start,a0@

/*
 * We check to see if
 * the kernel is running at
 * the correct location.  If
 * not, we use bcopy to copy
 * ourselves to the right place.
 */
	lea	pc@(2),a0
loc:	cmpl	#loc,a0
	beq	0f
	movl	a0,d0
	movl	#_edata,a0
	subl	#_start,a0
	movl	a0,sp@-
	movl	#_start,sp@-
	subl	#loc-_start,d0
	movl	d0,sp@-
	bsr	_bcopy
	lea	sp@(12),sp
	jmp	_start:l
0:

/*
 * OK.  Now we are running in
 * the right place.  Fetch bootdev
 * and boothowto from the stack and
 * store in variables.
 */
	lea	sp@(4),sp	    | pop return addr
	movl	sp@+,_bootdev:l
	movl	sp@+,_boothowto:l
	movl	sp@+,_ramdisk_image:l
	movl	sp@+,_ramdisk_size:l

/*
 * Reinitialize the application heap.
 */
	lea	applstack,sp
	.word	0xa02c			| InitApplZone

/*
 * Switch to 32 bit mode
 */
	moveq	#1,d0
	.word	0xa05d			| SwapMMUMode

#ifdef MODE24
	extbl	d0
	movl	d0,_mac32bit		| Save boot MMU mode
#else /* MODE24 */
	/* SHOULD CRASH HERE IF KERNEL BOOTED IN 24-BIT MODE */
#endif /* MODE24 */
	
	movw	#SR_HIGH,sr		| mask interrupts

/*
 * Reset FPP
 */
	btst	#G_hwCbFPU,G_HWCfgFlags
	beq	0f
	clrl	sp@-
	frestore	sp@+
0:

/*
 * Setup CPU cache(s)/Test for 68030
 */
	movc	cacr,d0
	movl	d0,_cache

	movl	#CPU_DCACHE_WRTALLOC,d0
	movc	d0,cacr
	movc	cacr,d0
	andw	#CPU_DCACHE_WRTALLOC,d0
	beq	0f
	movl	#1,_is68030
0:
	movl	_cache,d0
	orl	#CPU_CACHE_CLR,d0
	movc	d0,cacr

/*
 * Zero BSS segment
 */
	lea	_end,a0
	subl	#_edata,a0
	movl	a0,sp@-
	pea	_edata
	jsr	_bzero

/*
 * Calculate available physical
 * memory and store it.
 */
/*
 * avail_start is end of kernel bss
 * rounded to next page.
 */
	movl	#MAC2_PGBYTES-1,d1
	movl	#_end,d0
	addl	d1,d0
	notl	d1
	andl	d1,d0
	movl	d0,_avail_start

/*
 * avail_end is the addr of the last
 * available memory page.
 */
	movl	G_BufPtr,d0
	subql	#1,d0
	andl	d1,d0
	movl	d0,_avail_end

/*
 * mem_size is the total amount of
 * physical memory in the system.
 * memory is defined to start at 0.
 */
	movl	G_MemTop,_mem_size

	jsr	_mac2_init
	jsr	_machine_startup
	halt

/*
 * The TRAP #8 instruction is
 * used to simulate a macintosh
 * interrupt from a mach interrupt
 * handler.
 */
ENTRY(_trap8)
	btst	#SR_SUPR_BIT,sp@
	bne	1f
	movl	a0,sp@-
	movc	msp,a0
	moveml	d0-d7/a0-a6,a0@(PCB_REGS)
	movl	sp@+,a0@(PCB_A0)
	movl	usp,a1
	movl	a1,a0@(PCB_SP)
	pea	sp@
	movl	#TRAP_ERROR,sp@-
	jsr	_user_trap
	addqw	#8,sp
	jmp	rte_user
1:
	movw	a6@(4),d0
	orw	#SR_SUPR,d0
	movw	d0,sp@
	jmp	a0@(0)@(0)

/*
 * This routine is installed
 * as a VBL task to provide
 * clock interrupts to the
 * mach kernel.
 */
ENTRY(clocktask)
#ifdef MODE24
	movb	G_MMU32bit,sp@-
	bne	0f
	moveq	#1,d0
	movl	G_JSwapMMU,a0
	jsr	a0@
0:
#endif /* MODE24 */
	pea	a6@(4)	
	jsr	_clock_intr
	addqw	#4,sp

#ifdef MODE24
	movb	sp@+,d0
	bne	0f
	movl	G_JSwapMMU,a0
	jsr	a0@
0:
#endif /* MODE24 */
	rts

ENTRY(timeread_task)
#ifdef MODE24
	movb	G_MMU32bit,sp@-
	moveml	a1,sp@-
	bne	0f
	moveq	#1,d0
	movl	G_JSwapMMU,a0
	jsr	a0@
0:
#else /* MODE24 */
	moveml	a1,sp@-
#endif /* MODE24 */
	jsr	_timeread_intr
	addqw	#4,sp

#ifdef MODE24
	movb	sp@+,d0
	bne	0f
	movl	G_JSwapMMU,a0
	jsr	a0@
0:
#endif /* MODE24 */
	rts

/*
 * These routines are installed
 * as slot VBL tasks.
 */
#ifdef MODE24
#define SLOT_TASK(slot) \
	.globl	_macslot##slot##_task;	\
_macslot##slot##_task: ;		\
	movb	G_MMU32bit,sp@-;	\
	bne	0f;			\
	moveq	\#1,d0;			\
	movl	G_JSwapMMU,a0;		\
	jsr	a0@;			\
0: ;					\
	movl	\#0x##slot,sp@-;	\
	jsr	_macslot_intr;		\
	addqw	\#4,sp;			\
\
	movb	sp@+,d0;		\
	bne	0f;			\
	movl	G_JSwapMMU,a0;		\
	jsr	a0@;			\
0: ;					\
	rts
#else /* MODE24 */
#define SLOT_TASK(slot) \
	.globl	_macslot##slot##_task;	\
_macslot##slot##_task: ;		\
	movl	\#0x##slot,sp@-;	\
	jsr	_macslot_intr;		\
	addqw	\#4,sp;			\
	rts
#endif /* MODE24 */

SLOT_TASK(9)
SLOT_TASK(A)
SLOT_TASK(B)
SLOT_TASK(C)
SLOT_TASK(D)
SLOT_TASK(E)

/*
 * This is the service routine
 * for ADB devices.
 */
ENTRY(adb_service)
	movb	a0@+,d1
	movl	a0,sp@-
	lslw	#8,d0
	orb	d1,d0
	movl	d0,sp@-
	jsr	_adb_input
	addqw	#8,sp
	rts

/*
 * Device Interrupt handlers
 */
ENTRY(_level1intr)
	link	a6,#0
	cmpl	#applstack,sp
	bls	0f
	lea	applstack,sp
0:
	moveml	d0-d1/a0-a1,sp@-
#ifdef MODE24
	movb	G_MMU32bit,sp@-
	bne	0f
	moveq	#1,d0
	movl	G_JSwapMMU,a0
	jsr	a0@
0:
#endif /* MODE24 */
	lea	0x64,a0
	trap	#8
	jmp	rte_intr

ENTRY(_level2intr)
	link	a6,#0
	cmpl	#applstack,sp
	bls	0f
	lea	applstack,sp
0:
	moveml	d0-d1/a0-a1,sp@-
#ifdef MODE24
	movb	G_MMU32bit,sp@-
	bne	0f
	moveq	#1,d0
	movl	G_JSwapMMU,a0
	jsr	a0@
0:
#endif /* MODE24 */
	lea	0x68,a0
	trap	#8
	jmp	rte_intr

ENTRY(_level4intr)
	link	a6,#0
	cmpl	#applstack,sp
	bls	0f
	lea	applstack,sp
0:
	moveml	d0-d1/a0-a1,sp@-
#ifdef MODE24
	movb	G_MMU32bit,sp@-
	bne	0f
	moveq	#1,d0
	movl	G_JSwapMMU,a0
	jsr	a0@
0:
#endif /* MODE24 */
	lea	0x70,a0
	trap	#8
	jmp	rte_intr

	.data
nmi_lock:
	.long	0
	.text

ENTRY(_nmi)
	bset	#0,nmi_lock
	beq	0f
	rte
0:
	link	a6,#0
	cmpl	#applstack,sp
	bls	0f
	lea	applstack,sp
0:
	moveml	d0-d1/a0-a1,sp@-
#ifdef MODE24
	movl	_mac32bit,d0
	movl	G_JSwapMMU,a0
	jsr	a0@
	movb	d0,sp@-
#endif
/*0:*/
	lea	0x7c,a0
	trap	#8

#ifdef MODE24
	movb	sp@+,d0
	movl	G_JSwapMMU,a0
	jsr	a0@
#endif /* MODE24 */

	moveml	sp@+,d0-d1/a0-a1
	unlk	a6

	bclr	#0,nmi_lock
	rte

/*
 * System Call handler
 */
ENTRY(_syscall)
	btst	#SR_SUPR_BIT,sp@
	bne	syscall_supr
/*
 * Save registers a0-a1/d0-d2 and usp
 */
	movl	a0,sp@-
	movc	msp,a0
	movl	sp@+,a0@(PCB_A0)
	movl	a1,a0@(PCB_A1)
	movl	usp,a1
	movl	a1,a0@(PCB_SP)
	moveml	d0-d2,a0@(PCB_D0)
/*
 * Retrieve syscall number into d2
 */
	movl	a0@(PCB_THREAD),a1
	movl	#syscall_fault,a1@(THREAD_RECOVER)
	movl	#FC_UD,d0
	movc	d0,sfc
	movl	a0@(PCB_SP),a1
	movsl	a1@+,d2
	movl	a1,a0@(PCB_SP)
	movl	a0@(PCB_THREAD),a1
	clrl	a1@(THREAD_RECOVER)
/*
 * Calculate emulation vector into d1
 */
	movl	d2,d1
	subl	#EML_MIN_SYSCALL,d1
	blt	syscall_native
	movl	a1@(THREAD_TASK),a1
	tstl	a1@(TASK_EMUL)
	beq	syscall_native
	movl	a1@(TASK_EMUL),a1
	cmpl	a1@(EMUL_DISP_COUNT),d1
	bgt	syscall_native
	movl	a1@(EMUL_DISP_VECTOR,d1:L:4),d1
	bne	syscall_emul
	/* FALL THROUGH */
/*
 * Native system call
 * NB: This code pops the syscall number
 * off the user stack before calling _syscall!!
 */
syscall_native:
	moveml	d3-d7,a0@(PCB_D3)
	moveml	a2-a6,a0@(PCB_A2)
	pea	sp@
	negl	d2
	movl	d2,sp@-
	jsr	_syscall
	addqw	#8,sp
	jmp	rte_user

/*
 * Emulated system call
 *	d1	emulation vector
 *	d2	syscall number
 */
syscall_emul:
/*
 * Setup call frame on user stack
 */
	movl	a0@(PCB_THREAD),a1
	movl	#syscall_fault,a1@(THREAD_RECOVER)
	movl	#FC_UD,d0
	movc	d0,sfc
	movl	a0@(PCB_SP),a1
/*
 * Push return PC on user
 * stack
 */
	movl	sp@(F_PC),d0
	movsl	d0,a1@-
/*
 * Push CCR on user stack
 */
	movw	sp@(F_SR),d0
	movsw	d0,a1@-
/*
 * Push syscall number on
 * user stack
 */
	movsl	d2,a1@-
	movl	a1,a0@(PCB_SP)
	movl	a0@(PCB_THREAD),a1
	clrl	a1@(THREAD_RECOVER)
/*
 * Replace current frame PC w/emulation
 * vector
 */
	movl	d1,sp@(F_PC)
/*
 * Restore registers a0-a1/d0-d2 and usp
 */
	movl	a0@(PCB_SP),a1
	movl	a1,usp
	moveml	a0@(PCB_D0),d0-d2
	moveml	a0@(PCB_A0),a0-a1
	rte

/*
 * Fault occurred during user
 * stack manipulation
 */
syscall_fault:
/*
 * Set low order bit on current
 * frame PC
 */
	movl	sp@(F_PC),d0
	bset	#0,d0
	movl	d0,sp@(F_PC)
/*
 * Restore registers a0-a1/d0-d2 and usp
 */
	movc	msp,a0
	movl	a0@(PCB_SP),a1
	movl	a1,usp
	moveml	a0@(PCB_D0),d0-d2
	moveml	a0@(PCB_A0),a0-a1
	rte

/*
 * System call from supervisor mode
 * (an error)
 */
syscall_supr:
	moveml	d0-d7/a0-a7,sp@-
	lea	sp@(R_SP+4),a0
	pea	sp@
	movl	a0,sp@-
	movl	#TRAP_ERROR,sp@-
	jsr	_kernel_trap
	addw	#12,sp
	jmp	rte_kernel

/*
 * 1010 Line handler
 */
ENTRY(_EMULATION_1010)
	btst	#SR_SUPR_BIT,sp@
	bne	1f
	movl	a0,sp@-
	movc	msp,a0
	btst	#MAC_EMULATION_BIT,a0@(PCB_FLAGS)
	bne	2f

	moveml	d0-d7/a0-a6,a0@(PCB_REGS)
	movl	sp@+,a0@(PCB_A0)
	movl	usp,a1
	movl	a1,a0@(PCB_SP)
	pea	sp@
	movl	#TRAP_EMULATION_1010,sp@-
	jsr	_user_trap
	addqw	#8,sp
	jmp	rte_user

1:	jmp	@(0x28)@(0)
2:	jmp	__ATRAP_EMULATION

/*
 * TRAP1 handler
 */
ENTRY(_trap1)
	btst	#SR_SUPR_BIT,sp@
	bne	1f
	movl	a0,sp@-
	movc	msp,a0
	btst	#MAC_EMULATION_BIT,a0@(PCB_FLAGS)
	bne	2f

	moveml	d0-d7/a0-a6,a0@(PCB_REGS)
	movl	sp@+,a0@(PCB_A0)
	movl	usp,a1
	movl	a1,a0@(PCB_SP)
	pea	sp@
	movl	#TRAP_BAD_INSTRUCTION,sp@-
	jsr	_user_trap
	addqw	#8,sp
	jmp	rte_user

1:	moveml	d0-d7/a0-a7,sp@-
	lea	sp@(R_SP+4),a0
	pea	sp@
	movl	a0,sp@-
	movl	#TRAP_BAD_INSTRUCTION,sp@-
	jsr	_kernel_trap
	addw	#12,sp
	jmp	rte_kernel

2:	jmp	__TRAP1_EMULATION

/*
 * Trap handlers
 */

TRAP(BAD_ACCESS)

TRAP(TRACE)

TRAP(EMULATION_1111)

TRAP(BAD_INSTRUCTION)

TRAP(PRIV_INSTRUCTION)

TRAP(BREAKPOINT)

TRAP(ARITHMETIC)

TRAP(SOFTWARE)

TRAP(ERROR)

/*
 * Called to restart the machine.  Jumps
 * to the ROM entry point after turning off
 * the PMMU on machines earlier than the ci.
 */
ENTRY(boot_inst)
	movw	#SR_HIGH,sr
	lea	applstack,sp
	cmpl	#0xffffffff,G_BootGlobPtr
	bne	0f
	clrl	sp@-
	pmove	sp@,TC
0:
	lea	0x40800000,a0
	movl	a0@(4),a0
	jmp	a0@

/*
 * Routines to copy data to and from
 * user space.
 */

/*
 * copyout(from, to, count)
 *
 * copy a block of data from kernel space
 * to user space.
 */
ENTRY(copyout)
ENTRY2(copyoutmsg)
	moveml	d2/a2,sp@-
	movl	#FC_UD,d0
	movc	d0,dfc			| load destination function code (user xxx)
	movl	_active_threads,a2	| get active thread
	movl	#cperr,a2@(THREAD_RECOVER)	| catch any faults
	movl	sp@(12),a0		| get src address
	movl	sp@(16),a1		| get dst address
	movl	sp@(20),d1		| get count
	
	/*
	 * get count of long
	 * words into d0
	 */
	movl	d1,d0
	lsrl	#2,d0
	
	/*
	 * check for count == 0
	 */
	beq	2f
	bra	1f
	
	/*
	 * loop for moving lower 16 bits
	 * worth of count
	 */
0:	movl	a0@+,d2			| fetch data
	movsl	d2,a1@+			| store it
1:	dbf	d0,0b
	
	/*
	 * check upper 16 bits
	 * of count for more to do
	 */
	swap	d0
	tstw	d0
	beq	2f
	subqw	#1,d0
	swap	d0
	bra	0b
	
	/*
	 * check for residue bytes
	 */
2:	andl	#3,d1
	bra	1f

	/*
	 * move residue
	 */
0:	movb	a0@+,d2			| fetch data
	movsb	d2,a1@+			| store it
1:	dbf	d1,0b

	moveq	#0,d0			| return success
	clrl	a2@(THREAD_RECOVER)	| clear fault address
	moveml	sp@+,d2/a2
	rts

/*
 * copyin(from, to, count)
 *
 * copy a block of data from user space
 * to kernel space.
 */
ENTRY(copyin)
ENTRY2(copyinmsg)
	moveml	d2/a2,sp@-
	movl	#FC_UD,d0
	movc	d0,sfc			| load source function code (user xxx)
	movl	_active_threads,a2	| get active thread
	movl	#cperr,a2@(THREAD_RECOVER)	| catch any faults
	movl	sp@(12),a0		| get src address
	movl	sp@(16),a1		| get dst address
	movl	sp@(20),d1		| get count
	
	/*
	 * get count of long
	 * words into d0
	 */
	movl	d1,d0
	lsrl	#2,d0
	
	/*
	 * check for count == 0
	 */
	beq	2f
	bra	1f
	
	/*
	 * loop for moving lower 16 bits
	 * worth of count
	 */
0:	movsl	a0@+,d2			| fetch data
	movl	d2,a1@+			| store it
1:	dbf	d0,0b
	
	/*
	 * check upper 16 bits
	 * of count for more to do
	 */
	swap	d0
	tstw	d0
	beq	2f
	subqw	#1,d0
	swap	d0
	bra	0b
	
	/*
	 * check for residue bytes
	 */
2:	andl	#3,d1
	bra	1f

	/*
	 * move residue
	 */
0:	movsb	a0@+,d2			| fetch data
	movb	d2,a1@+			| store it
1:	dbf	d1,0b

	moveq	#0,d0			| return success
	clrl	a2@(THREAD_RECOVER)	| clear fault address
	moveml	sp@+,d2/a2
	rts

cperr:
	moveq	#1,d0		| return error
	clrl	a2@(THREAD_RECOVER)	| clear fault address
	moveml	sp@+,d2/a2
	rts

#ifdef notdef
/* Context Switching Primitives from MACH 2.5 */
/*
 * switch_thread_context()
 * switch_task_context()
 *
 * Switch context to a different thread.
 */
ENTRY(switch_thread_context)
ENTRY2(switch_task_context)
	movl	sp@(4),a0

/*
 * MACH scheduler glue
 */
	cmpl	#TH_RUN,a0@(THREAD_STATE)
	bne	0f
	clrl	sp@-
	movl	a0,sp@-
	jsr	_thread_setrun
	addqw	#8,sp
	bra	1f

0:
	cmpl	#TH_RUN+TH_WAIT,a0@(THREAD_STATE)
	bne	0f
	andl	#~TH_RUN,a0@(THREAD_STATE)
	bra	1f

0:
	cmpl	#TH_RUN+TH_IDLE,a0@(THREAD_STATE)
	beq	1f

	movl	a0,sp@-
	jsr	_thread_continue
	addqw	#4,sp
1:

/*
 * Save context of old thread
 */
	movl	sp@(4),a0
	movl	a0@(THREAD_PCB),a0

/*
 * Save FP state
 */
	btst	#G_hwCbFPU,G_HWCfgFlags
	beq	0f
	fsave	a0@(PCB_FP_FRAME)
	tstw	a0@(PCB_FP_FRAME)
	beq	0f
	fmovem	fpc/fps/fpi,a0@(PCB_FPS_CTRL)
	fmovem	fp0-fp7,a0@(PCB_FPS_REGS)
0:
/*
 * Save thread kernel context
 */
	movl	sp@,a0@(PCB_K_PC)
	movw	sr,a0@(PCB_K_SR)
	orw	#SR_IPL7,sr
	moveml	d2-d7/a2-a7,a0@(PCB_K_REGS)

/*
 * Restore context of new thread
 */
	movl	sp@(8),a0
	movl	a0@(THREAD_PCB),a1

/*
 * Forced restore of FP state
 */
	btst	#G_hwCbFPU,G_HWCfgFlags
	beq	2f
	bclr	#FP_RESTORE_BIT,a1@(PCB_FLAGS)
	beq	0f
	frestore a1@(PCB_FP_FRAME)
	fmovem	a1@(PCB_FPS_REGS),fp0-fp7
	fmovem	a1@(PCB_FPS_CTRL),fpc/fps/fpi
	bra	2f	

/*
 * Normal restore of FP state
 */
0:
	tstw	a1@(PCB_FP_FRAME)
	beq	1f

	fmovem	a1@(PCB_FPS_REGS),fp0-fp7
	fmovem	a1@(PCB_FPS_CTRL),fpc/fps/fpi
1:
	frestore a1@(PCB_FP_FRAME)
2:
/*
 * Restore thread kernel context
 */
	moveml	a1@(PCB_K_REGS),d2-d7/a2-a7
	movl	a0,_active_threads
	movc	a1,msp
	movw	a1@(PCB_K_SR),sr
	movl	a1@(PCB_K_PC),sp@

	rts
#endif

/*
 * Context Switching Primitives
 *
 * switch_context_{save,discard,handoff}(old_thread, new_thread)
 *	and
 * load_context(thread)
 */

/*
 * Saves the old thread's kernel context before
 * restoring that of the new thread.  Returns the
 * old thread (thread_t).
 */
ENTRY(switch_context_save)
/*
 * Save context of old thread
 */
	movl	sp@(4),a0
	movl	a0@(THREAD_PCB),a1

/*
 * Save FP state
 */
	btst	#G_hwCbFPU,G_HWCfgFlags
	beq	0f
	fsave	a1@(PCB_FP_FRAME)
	tstw	a1@(PCB_FP_FRAME)
	beq	0f
	fmovem	fpc/fps/fpi,a1@(PCB_FPS_CTRL)
	fmovem	fp0-fp7,a1@(PCB_FPS_REGS)
0:
/*
 * Save thread kernel context
 */
	movl	sp@,a1@(PCB_K_PC)
	movw	sr,a1@(PCB_K_SR)
	moveml	d2-d7/a2-a7,a1@(PCB_K_REGS)

/*
 * Return old thread
 */
	movl	a0,d0

/*
 * Restore context of new thread
 */
	movl	sp@(8),a0
	movl	a0@(THREAD_PCB),a1

/*
 * Forced restore of FP state
 */
	btst	#G_hwCbFPU,G_HWCfgFlags
	beq	2f
	bclr	#FP_RESTORE_BIT,a1@(PCB_FLAGS)
	beq	0f
	frestore a1@(PCB_FP_FRAME)
	fmovem	a1@(PCB_FPS_REGS),fp0-fp7
	fmovem	a1@(PCB_FPS_CTRL),fpc/fps/fpi
	bra	2f	

/*
 * Normal restore of FP state
 */
0:
	tstw	a1@(PCB_FP_FRAME)
	beq	1f

	fmovem	a1@(PCB_FPS_REGS),fp0-fp7
	fmovem	a1@(PCB_FPS_CTRL),fpc/fps/fpi
1:
	frestore a1@(PCB_FP_FRAME)
2:
/*
 * Restore thread kernel context
 */
	moveml	a1@(PCB_K_REGS),d2-d7/a2-a7
	movc	a1,msp
	movw	a1@(PCB_K_SR),sr
	movl	a1@(PCB_K_PC),sp@

	rts

/*
 * Restores new thread's kernel context,
 * discarding that of the old thread.
 * Returns the old thread (thread_t).
 */
ENTRY(switch_context_discard)
/*
 * Save context of old thread
 */
	movl	sp@(4),a0
	movl	a0@(THREAD_PCB),a1

/*
 * Save FP state
 */
	btst	#G_hwCbFPU,G_HWCfgFlags
	beq	0f
	fsave	a1@(PCB_FP_FRAME)
	tstw	a1@(PCB_FP_FRAME)
	beq	0f
	fmovem	fpc/fps/fpi,a1@(PCB_FPS_CTRL)
	fmovem	fp0-fp7,a1@(PCB_FPS_REGS)
0:
/*
 * Return old thread
 */
	movl	a0,d0

/*
 * Restore context of new thread
 */
	movl	sp@(8),a0
	movl	a0@(THREAD_PCB),a1

/*
 * Forced restore of FP state
 */
	btst	#G_hwCbFPU,G_HWCfgFlags
	beq	2f
	bclr	#FP_RESTORE_BIT,a1@(PCB_FLAGS)
	beq	0f
	frestore a1@(PCB_FP_FRAME)
	fmovem	a1@(PCB_FPS_REGS),fp0-fp7
	fmovem	a1@(PCB_FPS_CTRL),fpc/fps/fpi
	bra	2f	

/*
 * Normal restore of FP state
 */
0:
	tstw	a1@(PCB_FP_FRAME)
	beq	1f

	fmovem	a1@(PCB_FPS_REGS),fp0-fp7
	fmovem	a1@(PCB_FPS_CTRL),fpc/fps/fpi
1:
	frestore a1@(PCB_FP_FRAME)
2:
/*
 * Restore thread kernel context
 */
	moveml	a1@(PCB_K_REGS),d2-d7/a2-a7
	movc	a1,msp
	movw	a1@(PCB_K_SR),sr
	movl	a1@(PCB_K_PC),sp@

	rts

/*
 * Called when new thread is stealing
 * the old thread's stack.
 * Returns void.
 */
ENTRY(switch_context_handoff)
	movl	sp@(4),a0
	movl	a0@(THREAD_PCB),a0

/*
 * Save FP state
 */
	btst	#G_hwCbFPU,G_HWCfgFlags
	beq	0f
	fsave	a0@(PCB_FP_FRAME)
	tstw	a0@(PCB_FP_FRAME)
	beq	0f
	fmovem	fpc/fps/fpi,a0@(PCB_FPS_CTRL)
	fmovem	fp0-fp7,a0@(PCB_FPS_REGS)
0:
/*
 * Restore context of new thread
 */
	movl	sp@(8),a0
	movl	a0@(THREAD_PCB),a1

/*
 * Forced restore of FP state
 */
	btst	#G_hwCbFPU,G_HWCfgFlags
	beq	2f
	bclr	#FP_RESTORE_BIT,a1@(PCB_FLAGS)
	beq	0f
	frestore a1@(PCB_FP_FRAME)
	fmovem	a1@(PCB_FPS_REGS),fp0-fp7
	fmovem	a1@(PCB_FPS_CTRL),fpc/fps/fpi
	bra	2f	

/*
 * Normal restore of FP state
 */
0:
	tstw	a1@(PCB_FP_FRAME)
	beq	1f

	fmovem	a1@(PCB_FPS_REGS),fp0-fp7
	fmovem	a1@(PCB_FPS_CTRL),fpc/fps/fpi
1:
	frestore a1@(PCB_FP_FRAME)
2:
	movc	a1,msp

	rts

/*
 * Called to load the first context
 * onto a cpu.
 * Returns THREAD_NULL.
 */
ENTRY(load_context)
	movl	sp@(4),a0
	movl	a0@(THREAD_PCB),a1

/*
 * Forced restore of FP state
 */
	btst	#G_hwCbFPU,G_HWCfgFlags
	beq	2f
	bclr	#FP_RESTORE_BIT,a1@(PCB_FLAGS)
	beq	0f
	frestore a1@(PCB_FP_FRAME)
	fmovem	a1@(PCB_FPS_REGS),fp0-fp7
	fmovem	a1@(PCB_FPS_CTRL),fpc/fps/fpi
	bra	2f	

/*
 * Normal restore of FP state
 */
0:
	tstw	a1@(PCB_FP_FRAME)
	beq	1f

	fmovem	a1@(PCB_FPS_REGS),fp0-fp7
	fmovem	a1@(PCB_FPS_CTRL),fpc/fps/fpi
1:
	frestore a1@(PCB_FP_FRAME)
2:
/*
 * Restore thread kernel context
 */
	moveml	a1@(PCB_K_REGS),d2-d7/a2-a7
	movc	a1,msp
	movw	a1@(PCB_K_SR),sr
	movl	a1@(PCB_K_PC),sp@

	moveq	#0,d0

	rts

/*
 * Call a normal thread continuation
 * on a clean stack.
 */
ENTRY(call_continuation)
	movl	sp@(4),a0
	movc	msp,a1
	btst	#RET_FRAME_BIT,a1@(PCB_FLAGS)
	bne	0f
/*
 * Case 1:
 * Return frame is NOT already saved
 * in the PCB.  This can happen if a
 * syscall which uses a continuation
 * does not block for some reason.  In
 * this case we do not unwind the stack
 * all the way because that would trash
 * the return frame.
 */
	movl	a1@(PCB_FRAME),sp
	subl	a6,a6
	jmp	a0@
0:
/*
 * Case 2:
 * Return frame has already been saved
 * in the PCB.  Unwind the stack all the
 * way.
 */
	movl	a1@(PCB_STACK),sp
	subl	a6,a6
	jmp	a0@

/*
 * Call a thread continuation which was
 * set up by stack_attach().  Stack is
 * already clean.
 */
ENTRY(stack_attach_continue)
	movl	sp@,a0
	movl	d0,sp@
	jsr	a0@
/* SHOULDN'T RETURN */
	halt

/*
 * Return directly to user mode.
 * Don't unwind the stack if
 * the return frame is in the PCB
 * because rte_user() will do it.
 */
ENTRY(thread_exception_return)
ENTRY(thread_bootstrap_return)
	movc	msp,a0
	btst	#RET_FRAME_BIT,a0@(PCB_FLAGS)
	bne	0f
	movl	a0@(PCB_FRAME),sp
0:	jmp	rte_user

/*
 * Return directly to user mode,
 * returning a value in D0. Don't
 * unwind the stack if the return
 * frame is in the PCB because
 * rte_user() will do it.
 */
ENTRY(thread_syscall_return)
	movc	msp,a0
	movl	sp@(4),a0@(PCB_D0)
	btst	#RET_FRAME_BIT,a0@(PCB_FLAGS)
	bne	0f
	movl	a0@(PCB_FRAME),sp
0:	jmp	rte_user

/*
 * Return following a user trap.
 */
rte_user:
	orw	#SR_IPL,sr
	movc	msp,a1
	bclr	#RET_FRAME_BIT,a1@(PCB_FLAGS)
	jeq	2f
/*
 * Restore special return frame
 * from PCB
 */
	movl	a1@(PCB_STACK),a0
	movl	a1@(PCB_RETURN_FRAME_SIZE),d0
	subl	d0,a0
	movl	a0,sp
	lsrl	#1,d0
	lea	a1@(PCB_RETURN_FRAME_DATA),a2
	bra	1f
0:	movw	a2@+,a0@+
1:	dbf	d0,0b
	clrl	a1@(PCB_RETURN_FRAME_SIZE)
2:
/*
 * AST/Trace bit handling
 */
	btst	#TRACE_AST_BIT,a1@(PCB_AST)
	bne	3f
	btst	#TRACE_USER_BIT,a1@(PCB_FLAGS)
	beq	4f
3:	bset	#SR_TRACE_BIT,sp@(F_SR)
4:

/*
 * Run activities in
 * 'software' list
 */
	jsr	_softint

/*
 * Restore user registers
 */
	movc	msp,a1
	movl	a1@(PCB_SP),a0
	movl	a0,usp
	moveml	a1@,d0-d7/a0-a6
	rte

/*
 * Return following a kernel trap.
 */
rte_kernel:
	tstl	d0
	beq	0f

/*
 * Return with normal frame,
 * discarding existing frame
 */
	lea	sp@(R_SP+4),a0
	movl	a0,a1
	addw	d0,a1
	clrw	a1@(F_VOR)
	movl	a0@(F_PC),a1@(F_PC)
	movw	a0@(F_SR),a1@(F_SR)
	movl	a1,sp@(R_SP)
	moveml	sp@,d0-d7/a0-a7
	rte

/*
 * Return with existing frame
 */
0:
	moveml	sp@+,d0-d7/a0-a6
	addqw	#4,sp
	rte


/*
 * Return following a device interrupt.
 */
	.globl	rte_intr
rte_intr:
	orw	#SR_IPL,sr
	btst	#SR_SUPR_BIT,a6@(4)
	bne	1f
/*
 * AST handling
 */
	movc	msp,a0
	btst	#TRACE_AST_BIT,a0@(PCB_AST)
	beq	1f
	bset	#SR_TRACE_BIT,a6@(4)
1:

/*
 * If returning to IPL 0 run
 * activities in 'software' list
 */
	movw	a6@(4),d0
	andw	#SR_IPL,d0
	bne	2f
	jsr	_softint
/*
 * Restore saved registers
 */
2:
#ifdef MODE24
	movb	sp@+,d0
	bne	0f
	movl	G_JSwapMMU,a0
	jsr	a0@
0:
#endif /* MODE24 */
	moveml	sp@+,d0-d1/a0-a1
	unlk	a6
	rte

/*
 * Save FPU registers into thread pcb.
 */
ENTRY(fp_sync)
	btst	#G_hwCbFPU,G_HWCfgFlags
	beq	0f
	movc	msp,a0			| get thread PCB
	fsave	a0@(PCB_FP_FRAME)	| save internal state
	tstw	a0@(PCB_FP_FRAME)	| is there any?
	beq	0f			| bail out if not
	fmovem	fpc/fps/fpi,a0@(PCB_FPS_CTRL)	| save control regs
	fmovem	fp0-fp7,a0@(PCB_FPS_REGS)	| and data regs
	frestore a0@(PCB_FP_FRAME)	| restore internal state
0:
	rts

/*
 * Stop FPU and save internal state in thread pcb.
 */
ENTRY(fp_stop)
	btst	#G_hwCbFPU,G_HWCfgFlags
	beq	0f
	movc	msp,a0
	fsave	a0@(PCB_FP_FRAME)
0:
	rts

/*
 * Restore FPU internal state from thread pcb.
 */
ENTRY(fp_continue)
	btst	#G_hwCbFPU,G_HWCfgFlags
	beq	0f
	movc	msp,a0
	frestore a0@(PCB_FP_FRAME)
0:
	rts
