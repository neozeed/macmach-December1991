/* 
 * Mach Operating System
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 **************************************************************************
 * HISTORY
 * $Log:	signal.h,v $
 * Revision 2.5  91/09/03  11:13:17  jsb
 * 	From Intel SSD: Added i860 case.
 * 	[91/09/02  14:18:02  jsb]
 * 
 * Revision 2.4  90/06/02  15:25:48  rpd
 * 	Cleaned up conditionals; removed MACH, CMU, CMUCS, MACH_NO_KERNEL.
 * 	[90/04/28            rpd]
 * 	Add cantmask and stopsigmask macros.
 * 	[90/02/23            rwd]
 * 
 * Revision 2.3  90/03/14  21:29:23  rwd
 * 	Add cantmask and stopsigmask macros.
 * 	[90/02/23            rwd]
 * 
 * Revision 2.2  89/11/29  15:29:53  af
 * 	Mips additions.
 * 	[89/11/16  17:03:45  af]
 * 
 * Revision 2.1  89/08/04  14:48:15  rwd
 * Created.
 * 
 * Revision 2.8  88/12/20  13:53:53  rpd
 * 	Added SC_RTFL define.  Changed sc_ignore to sc_saveiar.
 * 	[88/12/08  10:41:47  rpd]
 * 
 * Revision 2.7  88/10/18  00:37:54  mwyoung
 * 	Don't define sc_fp and sc_ap fields in sigcontext structure
 * 	for 68000.
 * 	[88/10/17            jjc]
 * 
 * Revision 2.6  88/09/29  15:32:11  mrt
 * 	29-Sep-88 Mary Thompson (mrt) at Carnegie-Mellon
 * 	Added some parens to definition of signal so that hc could
 * 	compile it.
 * 	[88/09/29  15:25:35  mrt]
 * 
 * Revision 2.5  88/08/24  02:44:09  mwyoung
 * 	Adjusted include file references.
 * 	[88/08/17  02:22:51  mwyoung]
 * 
 *
 * 25-Apr-88  Richard Sanzi (sanzi) at Carnegie-Mellon University
 *	romp: Changed value of MAXSIGREGS to 33.  We want to give the
 *	users (lisp) more of the exception information, especially the
 *	info value from trap. 
 *
 * 14-Mar-88  Joseph Boykin (boykin) at Encore Computer Corporation
 *	Added definition for floating point Operand error.
 *
 * 02-Mar-88  Jonathan J. Chew (jjc) at Carnegie-Mellon University
 *	Added FPA exceptions for Sun.
 *
 * 05-Feb-88  Mary Thompson (mrt) at Carnegie Mellon
 *	Removed extraneous ; from end of #define sc_pc sc_iar
 *
 * 06-Jan-88  Jay Kistler (jjk) at Carnegie Mellon University
 *	Added declarations for __STDC__.
 *
 * 11-Nov-87  Jonathan J. Chew (jjc) at Carnegie-Mellon University
 *	Added FPA exceptions for Sun.
 *
 * 11-Sep-87  Richard Sanzi (sanzi) at Carnegie-Mellon University
 *	For romp and new rompc, define the sigcontext in a more rational
 *	way which retains compatability with older non-apc kernels.
 *
 * 22-Jul-87  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Removed MACH_ACC references.
 *
 * 13-Jul-87  David Black (dlb) at Carnegie-Mellon University
 *	MACH: added threadmask definition for KERNEL only.
 *
 *  4-Jul-87  Bill Bolosky (bolosky) at Carnegie-Mellon University
 *	Changed around rt sigcontext to support plopping APC exception
 *	packets in the sigcontext.  Also eliminate ROMP_FPA.
 *
 * 4-Jun-86   Mary Thompson at Carnegie Mellon
 *	removed  non-kernel include of sys/features and removed
 *	conditionals on ROMP_FPA and MACH_ACC || MACH
 *
 * 08-Jan-87  Robert Beck (beck) at Sequent Computer Systems, Inc.
 *	Add include of cputypes.h for BALANCE definition.
 *	Changed various #ifdef ns32000 to MULTIMAX.
 *	Add BALANCE flavor of struct sigcontext.
 *
 *  5-Dec-86  David L. Black (dlb) at Carnegie-Mellon University
 *	ns32000: added FLAG_TRAP code to SIGILL.
 *
 * 22-Oct-86  Jonathan J. Chew (jjc) at Carnegie-Mellon University
 *	Added 68000 dependent code and put back "#ifndef ASSEMBLER"s
 *	that someone took out.
 *
 *  7-Oct-86  David L. Black (dlb) at Carnegie-Mellon University
 *	Merged Multimax changes.
 *
 *  7-Sep-86  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Added conditional on MACH for SIGMSG things.
 *
 *  8-Jul-86  Bill Bolosky (bolosky) at Carnegie-Mellon University
 *	romp: Added include of ieeetrap.h and definition of fpvmach
 *
 * 25-Jan-86  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Upgraded to 4.3.  Note that the IPC signals conflict with the
 *	"user defined" signals.
 *
 * 18-Feb-86  Bill Bolosky (bolosky) at Carnegie-Mellon University
 *	Added fields sc_iar and sc_icscs in sigcontext for Sailboat
 *	under switch romp from IBM code.
 *
 * 29-Sep-84  Robert V Baron (rvb) at Carnegie-Mellon University
 *	Define SIGEMSG and SIGMSG for the IPC message delivery "interrupts"
 *
 **************************************************************************
 */
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)signal.h	7.1 (Berkeley) 6/4/86
 */

#ifndef	_SYS_SIGNAL_H_
#define	_SYS_SIGNAL_H_

#define NSIG	32

#define	SIGHUP	1	/* hangup */
#define	SIGINT	2	/* interrupt */
#define	SIGQUIT	3	/* quit */
#define	SIGILL	4	/* illegal instruction (not reset when caught) */
#ifdef	mc68000
#define     ILL_ILLINSTR_FAULT  0x10    /* illegal instruction fault */
#define     ILL_PRIVVIO_FAULT   0x20    /* privilege violation fault */
#define     ILL_COPROCERR_FAULT 0x34    /* [coprocesser protocol error fault] */
#define     ILL_TRAP1_FAULT     0x84    /* trap #1 fault */
#define     ILL_TRAP2_FAULT     0x88    /* trap #2 fault */
#define     ILL_TRAP3_FAULT     0x8c    /* trap #3 fault */
#define     ILL_TRAP4_FAULT     0x90    /* trap #4 fault */
#define     ILL_TRAP5_FAULT     0x94    /* trap #5 fault */
#define     ILL_TRAP6_FAULT     0x98    /* trap #6 fault */
#define     ILL_TRAP7_FAULT     0x9c    /* trap #7 fault */
#define     ILL_TRAP8_FAULT     0xa0    /* trap #8 fault */
#define     ILL_TRAP9_FAULT     0xa4    /* trap #9 fault */
#define     ILL_TRAP10_FAULT    0xa8    /* trap #10 fault */
#define     ILL_TRAP11_FAULT    0xac    /* trap #11 fault */
#define     ILL_TRAP12_FAULT    0xb0    /* trap #12 fault */
#define     ILL_TRAP13_FAULT    0xb4    /* trap #13 fault */
#define     ILL_TRAP14_FAULT    0xb8    /* trap #14 fault */
#else	mc68000
#define	    ILL_RESAD_FAULT	0x0	/* reserved addressing fault */
#define	    ILL_PRIVIN_FAULT	0x1	/* privileged instruction fault */
#define	    ILL_RESOP_FAULT	0x2	/* reserved operand fault */
#endif	mc68000
#ifdef	multimax
#define     ILL_FLAG_TRAP	0x3	/* flag trap taken */
#endif	multimax
#ifdef	mips
#define	    ILL_COPR_UNUSABLE	0x3	/* coprocessor unusable */
#endif	mips
/* CHME, CHMS, CHMU are not yet given back to users reasonably */
#define	SIGTRAP	5	/* trace trap (not reset when caught) */
#define	SIGIOT	6	/* IOT instruction */
#define	SIGABRT	SIGIOT	/* compatibility */
#define	SIGEMT	7	/* EMT instruction */
#define	SIGFPE	8	/* floating point exception */
#ifdef mc68000
#define     EMT_EMU1010         0x28    /* line 1010 emulator trap */
#define     EMT_EMU1111         0x2c    /* line 1111 emulator trap */

#define     FPE_INTDIV_TRAP     0x14    /* integer divide by zero */
#define     FPE_CHKINST_TRAP    0x18    /* CHK [CHK2] instruction */
#define     FPE_TRAPV_TRAP      0x1c    /* TRAPV [cpTRAPcc TRAPcc] instr */
#define     FPE_FLTBSUN_TRAP    0xc0    /* [branch or set on unordered cond] */
#define     FPE_FLTINEX_TRAP    0xc4    /* [floating inexact result] */
#define     FPE_FLTDIV_TRAP     0xc8    /* [floating divide by zero] */
#define     FPE_FLTUND_TRAP     0xcc    /* [floating underflow] */
#define     FPE_FLTOPERR_TRAP   0xd0    /* [floating operand error] */
#define     FPE_FLTOVF_TRAP     0xd4    /* [floating overflow] */
#define     FPE_FLTNAN_TRAP     0xd8    /* [floating Not-A-Number] */
#else	mc68000
#define	    FPE_INTOVF_TRAP	0x1	/* integer overflow */
#define	    FPE_INTDIV_TRAP	0x2	/* integer divide by zero */
#define	    FPE_FLTOVF_TRAP	0x3	/* floating overflow */
#define	    FPE_FLTDIV_TRAP	0x4	/* floating/decimal divide by zero */
#define	    FPE_FLTUND_TRAP	0x5	/* floating underflow */
#define	    FPE_DECOVF_TRAP	0x6	/* decimal overflow */
#define	    FPE_SUBRNG_TRAP	0x7	/* subscript out of range */
#define	    FPE_FLTOVF_FAULT	0x8	/* floating overflow fault */
#define	    FPE_FLTDIV_FAULT	0x9	/* divide by zero floating fault */
#define	    FPE_FLTUND_FAULT	0xa	/* floating underflow fault */
#endif	mc68000
#if	sun
#define     FPE_FPA_ENABLE	0x400	/* [FPA not enabled] */
#define     FPE_FPA_ERROR	0x404	/* [FPA arithmetic exception] */
#endif	sun
#ifdef	multimax
#define	    FPE_ILLINST_FAULT	0xb	/* Illegal FPU instruction */
#define     FPE_INVLOP_FAULT	0xc	/* Invalid operation */
#define	    FPE_INEXACT_FAULT	0xd	/* Inexact result */
#define	    FPE_OPERAND_FAULT	0xe	/* Operand fault	*/
#endif	multimax
#ifdef	mips
#define	    FPE_UNIMP_FAULT	0xb	/* Unimplemented FPU instruction */
#define	    FPE_INVALID_FAULT	0xc	/* Invalid operation */
#define	    FPE_INEXACT_FAULT	0xd	/* Inexact result */
#endif	mips
#define	SIGKILL	9	/* kill (cannot be caught or ignored) */
#define	SIGBUS	10	/* bus error */
#define	SIGSEGV	11	/* segmentation violation */
#define	SIGSYS	12	/* bad argument to system call */
#define	SIGPIPE	13	/* write on a pipe with no one to read it */
#define	SIGALRM	14	/* alarm clock */
#define	SIGTERM	15	/* software termination signal from kill */
#define	SIGURG	16	/* urgent condition on IO channel */
#define	SIGSTOP	17	/* sendable stop signal not from tty */
#define	SIGTSTP	18	/* stop signal from tty */
#define	SIGCONT	19	/* continue a stopped process */
#define	SIGCHLD	20	/* to parent on child stop or exit */
#define	SIGCLD	SIGCHLD	/* compatibility */
#define	SIGTTIN	21	/* to readers pgrp upon background tty read */
#define	SIGTTOU	22	/* like TTIN for output if (tp->t_local&LTOSTOP) */
#define	SIGIO	23	/* input/output possible signal */
#define	SIGXCPU	24	/* exceeded CPU time limit */
#define	SIGXFSZ	25	/* exceeded file size limit */
#define	SIGVTALRM 26	/* virtual time alarm */
#define	SIGPROF	27	/* profiling time alarm */
#define SIGWINCH 28	/* window size changes */
#define SIGUSR1 30	/* user defined signal 1 */
#define SIGUSR2 31	/* user defined signal 2 */
/* used by MACH kernels */
#define SIGEMSG 30	/* process received an emergency message */
#define	SIGMSG	31	/* process received normal message */


#ifdef	multimax
/*
 * Special signal number for intermediate signal handler.
 */
#define SIGCATCHALL 0x400
#endif	multimax
#ifdef	mips
/*
 * Codes for the mips break instruction.
 */
#define BRK_USERBP	0	/* user bp (used by debuggers) */
#define BRK_KERNELBP	1	/* kernel bp (used by prom) */
#define BRK_ABORT	2	/* no longer used */
#define BRK_BD_TAKEN	3	/* for taken bd emulation */
#define BRK_BD_NOTTAKEN	4	/* for not taken bd emulation */
#define BRK_SSTEPBP	5	/* user bp (used by debuggers) */
#define BRK_OVERFLOW	6	/* overflow check */
#define BRK_DIVZERO	7	/* divide by zero check */
#define BRK_RANGE	8	/* range error check */
#endif	mips

#if	!defined(__STDC__) && !defined(KERNEL)
int	(*signal())();
#endif	!defined(__STDC__) && !defined(KERNEL)

/*
 * Signal vector "template" used in sigvec call.
 */
#ifndef	ASSEMBLER
struct	sigvec {
	int	(*sv_handler)();	/* signal handler */
	int	sv_mask;		/* signal mask to apply */
	int	sv_flags;		/* see signal options below */
};
#endif	ASSEMBLER
#define SV_ONSTACK	0x0001	/* take signal on signal stack */
#define SV_INTERRUPT	0x0002	/* do not restart system on signal return */
#define sv_onstack sv_flags	/* isn't compatibility wonderful! */

/*
 * Structure used in sigstack call.
 */
#ifndef	ASSEMBLER
struct	sigstack {
	char	*ss_sp;			/* signal stack pointer */
	int	ss_onstack;		/* current status */
};

/*
 * Information pushed on stack when a signal is delivered.
 * This is used by the kernel to restore state following
 * execution of the signal handler.  It is also made available
 * to the handler to allow it to properly restore state if
 * a non-standard exit is performed.
 */
struct	sigcontext {
	int	sc_onstack;		/* sigstack state to restore */
	int	sc_mask;		/* signal mask to restore */
#ifdef	vax
	int	sc_sp;			/* sp to restore */
	int	sc_fp;			/* fp to restore */
	int	sc_ap;			/* ap to restore */
	int	sc_pc;			/* pc to restore */
	int	sc_ps;			/* psl to restore */
#endif	vax
#ifdef	mc68000
	int	sc_sp;			/* sp to restore */
	int	sc_pc;			/* pc to restore */
	int	sc_ps;			/* psl to restore */
#endif	mc68000
#ifdef	mac2
#define	sc_flags	sc_onstack
#define SC_ONSTACK	SV_ONSTACK	/* on signal stack */
#define SC_FRAMESAVE	0x8000		/* exception frame is above sigcontext */
#endif	mac2
#ifdef	multimax
	int	sc_r0;			/* r0 to restore */
	int	sc_r1;			/* r1 to restore */
	int	sc_r2;			/* r2 to restore */
	int	sc_sp;			/* sp to restore */
	int	sc_pc;			/* pc to restore */
	int	sc_ps;			/* psl to restore */
#endif	multimax
#ifdef	balance
	int	sc_sp;			/* sp to restore */
	int	sc_modpsr;		/* mod/psr to restore */
	int	sc_sp;			/* sp to restore */
	int	sc_pc;			/* pc to restore */
	int	sc_ps;			/* psl to restore */
#endif	balance
#ifdef	i386
#define MAXSIGREGS	19
	int	sc_gs;
	int	sc_fs;
	int	sc_es;
	int	sc_ds;
	int	sc_edi;
	int	sc_esi;
	int	sc_ebp;
	int	sc_esp;
	int	sc_ebx;
	int	sc_edx;
	int	sc_ecx;
	int	sc_eax;
	int	sc_trapno;	/* err & trapno keep the context */
	int	sc_err;		/* switching code happy */
	int	sc_eip;
	int	sc_cs;
	int	sc_efl;
	int	sc_uesp;
	int	sc_ss;
#endif	i386
#ifdef	mips
	int	sc_pc;			/* pc at time of signal */
	/*
	 * General purpose registers
	 */
	int	sc_regs[32];	/* processor regs 0 to 31 */
#define sc_sp sc_regs[29]
#define sc_fp sc_regs[30]
	int	sc_mdlo;	/* mul/div low */
	int	sc_mdhi;	/* mul/div high */
	/*
	 * Floating point coprocessor state
	 */
	int	sc_ownedfp;	/* fp has been used */
	int	sc_fpregs[32];	/* fp regs 0 to 31 */
	int	sc_fpc_csr;	/* floating point control and status reg */
	int	sc_fpc_eir;	/* floating point exception instruction reg */
	/*
	 * System coprocessor registers at time of signal
	 */
	int	sc_cause;	/* cp0 cause register */
	int	sc_badvaddr;	/* cp0 bad virtual address */
	int	sc_badpaddr;	/* cp0 bad physical address */
#endif	mips
#ifdef sparc
        int     sc_sp;                  /* sp to restore */
        int     sc_pc;                  /* pc to retore */
        int     sc_npc;                 /* next pc to restore */
        int     sc_psr;                 /* psr to restore */
        int     sc_g1;                  /* register that must be restored */
        int     sc_o0;
#endif sparc
#ifdef i860
	/*
	 * DON'T EVEN THINK ABOUT REARRANGING THIS WITHOUT CONSULTING
	 * emul_vector.s and sys/signal.h !!
	 */
	int	sc_regs[32];	/* integer registers 0 thru 31 */
#define sc_r0	sc_regs[0]	/* always 0 */
#define sc_r1	sc_regs[1]	/* user's return address */
#define sc_sp	sc_regs[2]	/* stack pointer */
#define sc_fp	sc_regs[3]	/* frame pointer */
#define sc_r4	sc_regs[4]	/* non-volatile */
#define sc_r5	sc_regs[5]	/* non-volatile */
#define sc_r6	sc_regs[6]	/* non-volatile */
#define sc_r7	sc_regs[7]	/* non-volatile */
#define sc_r8	sc_regs[8]	/* non-volatile */
#define sc_r9	sc_regs[9]	/* non-volatile */
#define sc_r10	sc_regs[10]	/* non-volatile */
#define sc_r11	sc_regs[11]	/* non-volatile */
#define sc_r12	sc_regs[12]	/* non-volatile */
#define sc_r13	sc_regs[13]	/* non-volatile */
#define sc_r14	sc_regs[14]	/* non-volatile */
#define sc_r15	sc_regs[15]	/* non-volatile */
#define sc_r16	sc_regs[16]	/* syscall param  1 */
#define sc_r17	sc_regs[17]	/* syscall param  2 */
#define sc_r18	sc_regs[18]	/* syscall param  3 */
#define sc_r19	sc_regs[19]	/* syscall param  4 */
#define sc_r20	sc_regs[20]	/* syscall param  5 */
#define sc_r21	sc_regs[21]	/* syscall param  6 */
#define sc_r22	sc_regs[22]	/* syscall param  7 */
#define sc_r23	sc_regs[23]	/* syscall param  8 */
#define sc_r24	sc_regs[24]	/* syscall param  9 */
#define sc_r25	sc_regs[25]	/* syscall param 10 */
#define sc_r26	sc_regs[26]	/* syscall param 11 */
#define sc_r27	sc_regs[27]	/* syscall param 12 */
#define sc_r28	sc_regs[28]	/* for structure passing or spills */
#define sc_r29	sc_regs[29]	/* emulator will return thru here */
#define sc_r30	sc_regs[30]	/* volatile */
#define sc_r31	sc_regs[31]	/* system call number */
	int	sc_fpregs[8];	/* 8 non-volatile floating point regs */
#define sc_f0	sc_fpregs[0]	/* always 0 */
#define sc_f1	sc_fpregs[1]	/* always 0 */
#define sc_f2	sc_fpregs[2]	/* non-volatile */
#define sc_f3	sc_fpregs[3]	/* non-volatile */
#define sc_f4	sc_fpregs[4]	/* non-volatile */
#define sc_f5	sc_fpregs[5]	/* non-volatile */
#define sc_f6	sc_fpregs[6]	/* non-volatile */
#define sc_f7	sc_fpregs[7]	/* non-volatile */
	int	sc_psr;		/* processor status reg */
	int	sc_fsr;		/* floating-point status reg */
	int	sc_epsr;	/* extended processor status reg */
	int	sc_fir;		/* pc, sometimes... */ 
#endif i860
};
#endif	ASSEMBLER

#define	BADSIG		(int (*)())-1
#define	SIG_DFL		(int (*)())0
#define	SIG_IGN		(int (*)())1

#ifdef	KERNEL
#define	SIG_CATCH	(int (*)())2
#define	SIG_HOLD	(int (*)())3
#endif	KERNEL

#if	defined(__STDC__) && !defined(KERNEL)
/* man(2) declarations */
extern int sigblock(int);
extern int sigpause(int);
extern int sigreturn(struct sigcontext *);
extern int sigsetmask(int);
extern int sigstack(struct sigstack *, struct sigstack *);
extern int sigvec(int, struct sigvec *, struct sigvec *);
/* man(3) declarations */
extern int siginterrupt(int, int);
extern int (*signal(int, int (*)()))();
#endif	defined(__STDC__) && !defined(KERNEL)

/*
 * Macro for converting signal number to a mask suitable for
 * sigblock().
 */
#define sigmask(m)	(1 << ((m)-1))

#define	cantmask	(sigmask(SIGKILL)|sigmask(SIGCONT)|sigmask(SIGSTOP))
#define	stopsigmask	(sigmask(SIGSTOP)|sigmask(SIGTSTP)| \
			sigmask(SIGTTIN)|sigmask(SIGTTOU))

#define	cantmask	(sigmask(SIGKILL)|sigmask(SIGCONT)|sigmask(SIGSTOP))
#define	stopsigmask	(sigmask(SIGSTOP)|sigmask(SIGTSTP)| \
			sigmask(SIGTTIN)|sigmask(SIGTTOU))

#ifdef	KERNEL
/*
 *	signals delivered on a per-thread basis.
 */
#define threadmask	(sigmask(SIGILL)|sigmask(SIGTRAP)|sigmask(SIGIOT)| \
			sigmask(SIGEMT)|sigmask(SIGFPE)|sigmask(SIGBUS)| \
			sigmask(SIGSEGV)|sigmask(SIGSYS)|sigmask(SIGPIPE))
#endif	KERNEL
#endif	_SYS_SIGNAL_H_
