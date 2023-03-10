/* 
 * Mach Operating System
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 **********************************************************************
 * HISTORY
 * $Log:	tty.h,v $
 * Revision 2.3  90/06/02  15:26:02  rpd
 * 	Cleaned up conditionals; removed MACH, CMU, CMUCS, MACH_NO_KERNEL.
 * 	[90/04/28            rpd]
 * 	Converted to new IPC.
 * 	[90/03/26  20:04:21  rpd]
 * 
 * Revision 2.2  89/08/31  16:29:09  rwd
 * 	Fix if/endifs
 * 	[89/08/28            rwd]
 * 	Added TS_RQUEUED to specify whether a read has been queued.
 * 	This is in case someone opens the device for WRONLY and then
 * 	someone opens for read.
 * 	[89/08/28            rwd]
 * 
 * 	Out-of-kernel version.  Add t_device_port, t_reply_port; remove
 * 	t_addr.
 * 	[89/01/11            dbg]
 * 
 * Revision 2.1  89/08/04  14:47:21  rwd
 * Created.
 * 
 * Revision 2.3  88/08/24  02:50:13  mwyoung
 * 	Adjusted include file references.
 * 	[88/08/17  02:26:18  mwyoung]
 * 
 *
 * 09-Apr-88  Mike Accetta (mja) at Carnegie-Mellon University
 *	CS_TTYLOC,CS_TTY => CS_GENERIC.
 *	[ V5.1(XF23) ]
 *
 * 30-Jan-87  Mike Accetta (mja) at Carnegie-Mellon University
 *	CS_GENERIC:  Added TS_LOGGEDIN and TS_LOGGEDOUT bit
 *	definitions.
 *	[ V5.1(F1) ]
 *
 * 21-Oct-86  Jonathan J. Chew (jjc) at Carnegie-Mellon University
 *	Added definition of "TS_OUT" for dialout tty on Sun, but
 *	didn't define it to be the same number as Sun Unix does.
 *
 * 26-Aug-86  Robert V Baron (rvb) at Carnegie-Mellon University
 *	Added TS_ONDELAY for X.
 *
 * 25-Jan-86  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Upgraded to 4.3.
 *
 * 09-May-85  Mike Accetta (mja) at Carnegie-Mellon University
 *	Upgraded from 4.1BSD.  Carried over changes below.
 *	[V1(1)]
 *
 * 28-Mar-83  Mike Accetta (mja) at Carnegie-Mellon University
 *	CS_GENERIC:  Added t_ttyloc field to tty structure for
 *	recording terminal location information (V3.06h).
 *
 **********************************************************************
 */
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)tty.h	7.1 (Berkeley) 6/4/86
 */

#ifndef	_SYS_TTY_H_
#define	_SYS_TTY_H_

#ifdef	KERNEL
#include <uxkern/import_mach.h>
#endif	KERNEL

#include <sys/ttychars.h>
#include <sys/ttydev.h>
#include <sys/ttyloc.h>

/*
 * A clist structure is the head of a linked list queue
 * of characters.  The characters are stored in blocks
 * containing a link and CBSIZE (param.h) characters. 
 * The routines in tty_subr.c manipulate these structures.
 */
struct clist {
	int	c_cc;		/* character count */
	char	*c_cf;		/* pointer to first char */
	char	*c_cl;		/* pointer to last char */
};

/*
 * Per-tty structure.
 *
 * Should be split in two, into device and tty drivers.
 * Glue could be masks of what to echo and circular buffer
 * (low, high, timeout).
 */
struct tty {
	union {
		struct {
			struct	clist T_rawq;
			struct	clist T_canq;
		} t_t;
#define	t_rawq	t_nu.t_t.T_rawq		/* raw characters or partial line */
#define	t_canq	t_nu.t_t.T_canq		/* raw characters or partial line */
		struct {
			struct	buf *T_bufp;
			char	*T_cp;
			int	T_inbuf;
			int	T_rec;
		} t_n;
#define	t_bufp	t_nu.t_n.T_bufp		/* buffer allocated to protocol */
#define	t_cp	t_nu.t_n.T_cp		/* pointer into the ripped off buffer */
#define	t_inbuf	t_nu.t_n.T_inbuf	/* number chars in the buffer */
#define	t_rec	t_nu.t_n.T_rec		/* have a complete record */
	} t_nu;
	struct	clist t_outq;		/* device */
	int	(*t_oproc)();		/* device */
	struct	proc *t_rsel;		/* tty */
	struct	proc *t_wsel;
				caddr_t	T_LINEP;	/* ### */
	mach_port_t	t_device_port;
	mach_port_t	t_reply_port;
	dev_t	t_dev;			/* device */
	int	t_flags;		/* some of both */
	int	t_state;		/* some of both */
	short	t_pgrp;			/* tty */
	char	t_delct;		/* tty */
	char	t_line;			/* glue */
	char	t_col;			/* tty */
	char	t_ispeed, t_ospeed;	/* device */
	char	t_rocount, t_rocol;	/* tty */
	struct	ttychars t_chars;	/* tty */
	struct	winsize t_winsize;	/* window size */
/* be careful of tchars & co. */
#define	t_erase		t_chars.tc_erase
#define	t_kill		t_chars.tc_kill
#define	t_intrc		t_chars.tc_intrc
#define	t_quitc		t_chars.tc_quitc
#define	t_startc	t_chars.tc_startc
#define	t_stopc		t_chars.tc_stopc
#define	t_eofc		t_chars.tc_eofc
#define	t_brkc		t_chars.tc_brkc
#define	t_suspc		t_chars.tc_suspc
#define	t_dsuspc	t_chars.tc_dsuspc
#define	t_rprntc	t_chars.tc_rprntc
#define	t_flushc	t_chars.tc_flushc
#define	t_werasc	t_chars.tc_werasc
#define	t_lnextc	t_chars.tc_lnextc
	struct ttyloc
		t_ttyloc;		/* terminal location (CMU) */
};

#define	TTIPRI	28
#define	TTOPRI	29

/* limits */
#define	NSPEEDS	16
#define	TTMASK	15
#define	OBUFSIZ	100
#define	TTYHOG	255
#ifdef	KERNEL
short	tthiwat[NSPEEDS], ttlowat[NSPEEDS];
#define	TTHIWAT(tp)	tthiwat[(tp)->t_ospeed&TTMASK]
#define	TTLOWAT(tp)	ttlowat[(tp)->t_ospeed&TTMASK]
extern	struct ttychars ttydefaults;
#endif	KERNEL

/* internal state bits */
#define	TS_TIMEOUT	0x000001	/* delay timeout in progress */
#define	TS_WOPEN	0x000002	/* waiting for open to complete */
#define	TS_ISOPEN	0x000004	/* device is open */
#define	TS_FLUSH	0x000008	/* outq has been flushed during DMA */
#define	TS_CARR_ON	0x000010	/* software copy of carrier-present */
#define	TS_BUSY		0x000020	/* output in progress */
#define	TS_ASLEEP	0x000040	/* wakeup when output done */
#define	TS_XCLUDE	0x000080	/* exclusive-use flag against open */
#define	TS_TTSTOP	0x000100	/* output stopped by ctl-s */
#define	TS_HUPCLS	0x000200	/* hang up upon last close */
#define	TS_TBLOCK	0x000400	/* tandem queue blocked */
#define	TS_RCOLL	0x000800	/* collision in read select */
#define	TS_WCOLL	0x001000	/* collision in write select */
#define	TS_NBIO		0x002000	/* tty in non-blocking mode */
#define	TS_ASYNC	0x004000	/* tty in async i/o mode */
#define	TS_ONDELAY	0x008000	/* device is open; software copy of 
 					 * carrier is not present */
#ifdef	sun
#define TS_OUT          0x010000        /* tty in use for dialout only */
					/* NOTE: This was 0x008000 in 
						 Sun Unix */
#endif	sun
/* state for intra-line fancy editing work */
#define	TS_BKSL		0x010000	/* state for lowercase \ work */
#define	TS_QUOT		0x020000	/* last character input was \ */
#define	TS_ERASE	0x040000	/* within a \.../ for PRTRUB */
#define	TS_LNCH		0x080000	/* next character is literal */
#define	TS_TYPEN	0x100000	/* retyping suspended input (PENDIN) */
#define	TS_CNTTB	0x200000	/* counting tab width; leave FLUSHO alone */

/* special CMU local state */
#define	TS_LOGGEDIN	0x400000	/* terminal is "logged-in" */
#define	TS_LOGGEDOUT	0x800000	/* login process has exited */

#define TS_RQUEUED	0x1000000	/* has a read been queued */

#define	TS_LOCAL	(TS_BKSL|TS_QUOT|TS_ERASE|TS_LNCH|TS_TYPEN|TS_CNTTB)

/* define partab character types */
#define	ORDINARY	0
#define	CONTROL		1
#define	BACKSPACE	2
#define	NEWLINE		3
#define	TAB		4
#define	VTAB		5
#define	RETURN		6

#endif	_SYS_TTY_H_
