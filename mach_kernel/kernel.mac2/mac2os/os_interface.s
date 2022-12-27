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
 * $Log:	os_interface.s,v $
 * Revision 2.2  91/09/12  16:52:10  bohman
 * 	Created.
 * 	[91/09/11  16:38:07  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2os/os_interface.s
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <mac2/asm.h>

#include <mac2os/Globals.h>
#include <mac2os/Traps.h>

/*
 * Interface code to the
 * Macintosh Operating System
 */

#define NEWOS	0x0200

#define IMMED	0x0200
#define ASYNC	0x0400

#define	ATrap(name) \
	.word	name

	.globl	_os_interface_init
_os_interface_init:
#ifdef MODE24
	movl	_mac32bit,d0
	jsr	trapSwapMMUMode
	movb	d0,sp@-

	tstl	_mac32bit
	bne	0f
	lea	trapSwapMMUMode,a0
	movl	a0,G_JSwapMMU
	movw	#SwapMMUMode,d0
	ATrap(SetTrapAddress+NEWOS)
0:
	movb	sp@+,d0
	jsr	trapSwapMMUMode
#endif /* MODE24 */
	rts

/*
 * The trap code for SwapMMUMode.
 * trashes d1/a0-a1
 */
trapSwapMMUMode:
	cmpb	G_MMU32bit,d0
	beq	0f
#ifdef MODE24
	movw	sr,sp@-
	orw	#0x700,sr
	extbl	d0
	movl	d0,sp@-
	jsr	_pmap_set_kern_addr_mode
	movl	sp@+,d1
	movb	G_MMU32bit,d0
	movb	d1,G_MMU32bit
	movw	sp@+,sr
#else /* MODE24 */
	/* !!! SOMEONE IS TRYING TO SWITCH TO 24-BIT MODE !!! */
#endif /* MODE24 */
0:
	rts

ENTRY(GetBootDrive)
	movl	sp@(4),a0
	movw	G_BootDrive,a0@
	rts

ENTRY(GetDateTime)
	movl	sp@(4),a0
	movl	G_Time,a0@
	rts

ENTRY(MemError)
	moveq	#0,d0
	movw	G_MemErr,d0
	rts

ENTRY(SwapMMUMode)
	movl	sp@(4),d0
	movl	G_JSwapMMU,a0
	jsr	a0@
	rts

/*
 * Entry code for an A trap call.
 * trashes d0-d1/a0-a1
 */
#ifdef MODE24
#define	enterATrapCall(name, n) \
	.globl	_##name;			\
_##name: ;					\
	link	a6,\#0;				\
	cmpl	\#applstack,sp;			\
	bls	0f;				\
	lea	applstack,sp;			\
0: 	STASH_##n;				\
	movl	_mac32bit,d0;			\
	cmpb	G_MMU32bit,d0;			\
	beq	0f;				\
	movw	sr,sp@-;			\
	orw	\#0x700,sr;			\
	movl	d0,sp@-;			\
	jsr	_pmap_set_kern_addr_mode;	\
	movl	sp@+,d1;			\
	movb	G_MMU32bit,d0;			\
	movb	d1,G_MMU32bit;			\
	movw	sp@+,sr;			\
0: ;						\
	movb	d0,sp@-
#else /* MODE24 */
#define	enterATrapCall(name, n) \
	.globl	_##name;			\
_##name: ;					\
	link	a6,\#0;				\
	cmpl	\#applstack,sp;			\
	bls	0f;				\
	lea	applstack,sp;			\
0: 	STASH_##n;				\
	movb	d0,sp@-
/* need to sort out this extra push */
#endif /* MODE24 */

/*
 * Exit code for an A trap call.
 * trashes d1/a0-a1
 */
#ifdef MODE24
#define exitATrapCall \
	bra	exitATrapCall_
exitATrapCall_:
	movb	sp@+,d1;			\
	cmpb	G_MMU32bit,d1;			\
	beq	0f;				\
	movl	d0,sp@-;			\
	movw	sr,sp@-;			\
	orw	#0x700,sr;			\
	extbl	d1;				\
	movl	d1,sp@-;			\
	jsr	_pmap_set_kern_addr_mode;	\
	movl	sp@+,d1;			\
	movb	d1,G_MMU32bit;			\
	movw	sp@+,sr;			\
	movl	sp@+,d0;			\
0: ;						\
	unlk	a6;				\
	rts
#else /* MODE24 */
#define exitATrapCall \
	unlk	a6;				\
	rts
#endif /* MODE24 */

#define STASH_0

#define STASH_1 \
	movl	a6@(8),sp@-

#define STASH_2 \
	movl	a6@(12),sp@-;	\
	movl	a6@(8),sp@-

#define STASH_3 \
	movl	a6@(16),sp@-;	\
	movl	a6@(12),sp@-;	\
	movl	a6@(8),sp@-

#ifdef MODE24
#define REG_ARG(x, reg) \
	movl sp@(2+(4*x)),reg
#else /* MODE24 */
#define REG_ARG(x, reg) \
	movl sp@(2+(4*x)),reg
#endif /* MODE24 */

#define STACK_RET_ALLOC_LONG \
	lea	sp@,a0;		\
	clrl	sp@-

#define STACK_RET_ALLOC_WORD \
	lea	sp@,a0;		\
	clrw	sp@-

#define STACK_RET_ALLOC_NONE \
	lea	sp@,a0

#define STACK_RET_LONG \
	movl	sp@+,d0

#define STACK_RET_WORD \
	movw	sp@+,d0

#define STACK_RET_NONE

#define STACK_ARG_LONG(x) \
	movl	a0@(2+(4*x)),sp@-

#define STACK_ARG_WORD(x) \
	movw	a0@(4+(4*x)),sp@-

#define PBCall(name) \
	enterATrapCall(PB##name, 2);	\
		REG_ARG(0, a0);		\
		REG_ARG(1, d0);		\
		bne	0f;		\
		ATrap(name);		\
		bra	1f;		\
0: ;					\
		ATrap(name+ASYNC);	\
1: ;					\
	exitATrapCall

enterATrapCall(InsTime, 1)
	REG_ARG(0, a0)
	ATrap(InsTime)
exitATrapCall

enterATrapCall(PrimeTime, 2)
	REG_ARG(0, a0)
	REG_ARG(1, d0)
	ATrap(PrimeTime)
exitATrapCall

enterATrapCall(RmvTime, 1)
	REG_ARG(0, a0)
	ATrap(RmvTime)
exitATrapCall

enterATrapCall(PBOffLine, 1)
	REG_ARG(0, a0)
	ATrap(OffLine)
exitATrapCall

enterATrapCall(OpenSlot, 2)
	REG_ARG(0, a0)
	REG_ARG(1, d0)
	bne	0f
	ATrap(Open+IMMED)
	bra	1f
0:
	ATrap(Open+ASYNC+IMMED)
1:
exitATrapCall

enterATrapCall(SlotVInstall, 2)
	REG_ARG(0, a0)
	REG_ARG(1, d0)
	ATrap(SlotVInstall)
exitATrapCall

enterATrapCall(SCSIReset, 0)
	STACK_RET_ALLOC_WORD
	movw	#0,sp@-
	ATrap(SCSIDispatch)
	STACK_RET_WORD
exitATrapCall

enterATrapCall(SCSIGet, 0)
	STACK_RET_ALLOC_WORD
	movw	#1,sp@-
	ATrap(SCSIDispatch)
	STACK_RET_WORD
exitATrapCall

enterATrapCall(SCSISelect, 1)
	STACK_RET_ALLOC_WORD
	STACK_ARG_WORD(0)
	movw	#2,sp@-
	ATrap(SCSIDispatch)
	STACK_RET_WORD
exitATrapCall

enterATrapCall(SCSICmd, 2)
	STACK_RET_ALLOC_WORD
	STACK_ARG_LONG(0)
	STACK_ARG_WORD(1)
	movw	#3,sp@-
	ATrap(SCSIDispatch)
	STACK_RET_WORD
exitATrapCall

enterATrapCall(SCSIRead, 1)
	STACK_RET_ALLOC_WORD
	STACK_ARG_LONG(0)
	movw	#5,sp@-
	ATrap(SCSIDispatch)
	STACK_RET_WORD
exitATrapCall

enterATrapCall(SCSIRBlind, 1)
	STACK_RET_ALLOC_WORD
	STACK_ARG_LONG(0)
	movw	#8,sp@-
	ATrap(SCSIDispatch)
	STACK_RET_WORD
exitATrapCall

enterATrapCall(SCSIWrite, 1)
	STACK_RET_ALLOC_WORD
	STACK_ARG_LONG(0)
	movw	#6,sp@-
	ATrap(SCSIDispatch)
	STACK_RET_WORD
exitATrapCall

enterATrapCall(SCSIWBlind, 1)
	STACK_RET_ALLOC_WORD
	STACK_ARG_LONG(0)
	movw	#9,sp@-
	ATrap(SCSIDispatch)
	STACK_RET_WORD
exitATrapCall

enterATrapCall(SCSIComplete, 3)
	STACK_RET_ALLOC_WORD
	STACK_ARG_LONG(0)
	STACK_ARG_LONG(1)
	STACK_ARG_LONG(2)
	movw	#4,sp@-
	ATrap(SCSIDispatch)
	STACK_RET_WORD
exitATrapCall

enterATrapCall(SCSIStat, 0)
	STACK_RET_ALLOC_WORD
	movw	#10,sp@-
	ATrap(SCSIDispatch)
	STACK_RET_WORD
exitATrapCall

enterATrapCall(ReadXPRam, 2)
	REG_ARG(0, a0)
	REG_ARG(1, d0)
	ATrap(ReadXPRam)
exitATrapCall

enterATrapCall(WriteXPRam, 2)
	REG_ARG(0, a0)
	REG_ARG(1, d0)
	ATrap(WriteXPRam)
exitATrapCall

enterATrapCall(KeyTrans, 3)
	STACK_RET_ALLOC_LONG
	STACK_ARG_LONG(0)
	STACK_ARG_WORD(1)
	STACK_ARG_LONG(2)
	ATrap(KeyTrans)
	STACK_RET_LONG
exitATrapCall

enterATrapCall(GetResource, 2)
	STACK_RET_ALLOC_LONG
	STACK_ARG_LONG(0)
	STACK_ARG_WORD(1)
	ATrap(GetResource)
	STACK_RET_LONG
exitATrapCall

enterATrapCall(DetachResource, 1)
	STACK_RET_ALLOC_NONE
	STACK_ARG_LONG(0)
	ATrap(DetachResource)
	STACK_RET_NONE
exitATrapCall	

enterATrapCall(HLock, 1)
	REG_ARG(0, a0)
	ATrap(HLock)
exitATrapCall

enterATrapCall(CountADBs, 0)
	ATrap(CountADBs)
exitATrapCall

enterATrapCall(GetIndADB, 2)
	REG_ARG(0, a0)
	REG_ARG(1, d0)
	ATrap(GetIndADB)
exitATrapCall

enterATrapCall(SetADBInfo, 2)
	REG_ARG(0, a0)
	REG_ARG(1, d0)
	ATrap(SetADBInfo)
exitATrapCall

enterATrapCall(VInstall, 1)
	REG_ARG(0, a0)
	ATrap(VInstall)
exitATrapCall

PBCall(Open)

PBCall(Close)

PBCall(Read)

PBCall(Write)

PBCall(Control)

PBCall(Status)

PBCall(KillIO)

enterATrapCall(SIntInstall, 2)
	REG_ARG(0, a0)
	REG_ARG(1, d0)
	ATrap(SIntInstall)
exitATrapCall

enterATrapCall(SRsrcInfo, 1)
	REG_ARG(0, a0)
	movl	#22,d0
	ATrap(SlotManager)
exitATrapCall

enterATrapCall(SFindStruct, 1)
	REG_ARG(0, a0)
	movl	#6,d0
	ATrap(SlotManager)
exitATrapCall

enterATrapCall(SGetBlock, 1)
	REG_ARG(0, a0)
	movl	#5,d0
	ATrap(SlotManager)
exitATrapCall

enterATrapCall(SFindDevBase, 1)
	REG_ARG(0, a0)
	movl	#27,d0
	ATrap(SlotManager)
exitATrapCall

enterATrapCall(SReadLong, 1)
	REG_ARG(0, a0)
	movl	#2,d0
	ATrap(SlotManager)
exitATrapCall

enterATrapCall(SNextsRsrc, 1)
	REG_ARG(0, a0)
	movl	#20,d0
	ATrap(SlotManager)
exitATrapCall

enterATrapCall(SReadStruct, 1)
	REG_ARG(0, a0)
	movl	#7,d0
	ATrap(SlotManager)
exitATrapCall

enterATrapCall(NewPtr, 1)
	REG_ARG(0, d0)
	ATrap(NewPtr)
	movw	d0,G_MemErr
	movl	a0,d0
exitATrapCall

enterATrapCall(DisposPtr, 1)
	REG_ARG(0,a0)
	ATrap(DisposPtr)
exitATrapCall

enterATrapCall(NewHandle, 1)
	REG_ARG(0, d0)
	ATrap(NewHandle)
	movw	d0,G_MemErr
	movl	a0,d0
exitATrapCall

enterATrapCall(DisposHandle, 1)
	REG_ARG(0, a0)
	ATrap(DisposHandle)
exitATrapCall
