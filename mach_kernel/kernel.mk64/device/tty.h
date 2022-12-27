/* 
 * Mach Operating System
 * Copyright (c) 1991,1990 Carnegie Mellon University
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
 * $Log:	tty.h,v $
 * Revision 2.6  91/07/09  23:16:12  danner
 * 	   Added omron tty specific flags; conditionalized under luna88k.
 * 	[91/05/25            danner]
 * 
 * Revision 2.5  91/05/14  16:02:00  mrt
 * 	Correcting copyright
 * 
 * Revision 2.4  91/02/05  17:10:26  mrt
 * 	Changed to new Mach copyright
 * 	[91/01/31  17:30:52  mrt]
 * 
 * Revision 2.3  90/08/27  21:55:44  dbg
 * 	Re-created to avoid ownership problems.
 * 	[90/07/09            dbg]
 * 
 */
/*
 *	Author: David B. Golub, Carnegie Mellon University
 *	Date: 	7/90
 *
 * 	Compatibility TTY structure for existing TTY device drivers.
 */

#ifndef	_DEVICE_TTY_H_
#define	_DEVICE_TTY_H_

#include <kern/lock.h>
#include <kern/queue.h>
#include <mach/port.h>

#include <device/device_types.h>
#ifdef luna88k
#include <luna88k/jtermio.h>
#endif
#include <device/tty_status.h>
#include <device/cirbuf.h>

struct tty {
	decl_simple_lock_data(,t_lock)
	struct cirbuf	t_inq;		/* input buffer */
	struct cirbuf	t_outq;		/* output buffer */
	char *		t_addr;		/* device pointer */
	int		t_dev;		/* device number */
	int		(*t_start)();	/* routine to start output */
#define	t_oproc	t_start
	int		(*t_stop)();	/* routine to stop output */
	char		t_ispeed;	/* input speed */
	char		t_ospeed;	/* output speed */
	char		t_breakc;	/* character to deliver when 'break'
					   condition received */
	int		t_flags;	/* mode flags */
	int		t_state;	/* current state */
	int		t_line;		/* fake line discipline number,
					   for old drivers - always 0 */
	queue_head_t	t_delayed_read;	/* pending read requests */
	queue_head_t	t_delayed_write;/* pending write requests */
	queue_head_t	t_delayed_open;	/* pending open requests */

	int		(*t_getstat)();	/* routine to get status */
#ifdef luna
      struct jterm    t_jt;
      short           t_jstate;
      char            t_term;         /* terminal type */
      char            t_tmflag;       /* t_tmflag */
      unsigned short  t_omron;        /* OMRON extended flags */
      int             *t_kfptr;
#endif luna
	int		(*t_setstat)();	/* routine to set status */
	dev_ops_t	t_tops;		/* another device to possibly
					   push through */
};
typedef struct tty	*tty_t;

/*
 * Common TTY service routines
 */
io_return_t	char_open();
io_return_t	char_read();
io_return_t	char_write();
void		tty_queue_completion();
boolean_t	tty_queueempty();

#define	tt_open_wakeup(tp) \
	(tty_queue_completion(&(tp)->t_delayed_open))
#define	tt_write_wakeup(tp) \
	(tty_queue_completion(&(tp)->t_delayed_write))


#define	TTMASK	(NSPEEDS-1)	/* assume power of 2! */
#define	OBUFSIZ	100
#define	TTYHOG	255

short	tthiwat[NSPEEDS], ttlowat[NSPEEDS];
#define	TTHIWAT(tp)	tthiwat[(tp)->t_ospeed&TTMASK]
#define	TTLOWAT(tp)	ttlowat[(tp)->t_ospeed&TTMASK]

/* internal state bits */
#define	TS_INIT		0x00000001	/* tty structure initialized */
#define	TS_TIMEOUT	0x00000002	/* delay timeout in progress */
#define	TS_WOPEN	0x00000004	/* waiting for open to complete */
#define	TS_ISOPEN	0x00000008	/* device is open */
#define	TS_FLUSH	0x00000010	/* outq has been flushed during DMA */
#define	TS_CARR_ON	0x00000020	/* software copy of carrier-present */
#define	TS_BUSY		0x00000040	/* output in progress */
#define	TS_ASLEEP	0x00000080	/* wakeup when output done */

#define	TS_TTSTOP	0x00000100	/* output stopped by ctl-s */
#define	TS_HUPCLS	0x00000200	/* hang up upon last close */
#define	TS_TBLOCK	0x00000400	/* tandem queue blocked */

#define	TS_NBIO		0x00001000	/* tty in non-blocking mode */
#define	TS_ONDELAY	0x00002000	/* device is open; software copy of 
 					 * carrier is not present */
#define TS_OUT          0x00010000	/* tty in use for dialout only */

#define TS_TRANSLATE	0x00100000	/* translation device enabled */
#define TS_KDB		0x00200000	/* should enter kdb on ALT */
/* flags - old names defined in terms of new ones */

#define	TANDEM		TF_TANDEM
#define	ODDP		TF_ODDP
#define	EVENP		TF_EVENP
#define	ANYP		(ODDP|EVENP)
#define	MDMBUF		TF_MDMBUF
#define	LITOUT		TF_LITOUT
#define	NOHANG		TF_NOHANG

#define	ECHO		TF_ECHO
#define	CRMOD		TF_CRMOD
#define	XTABS		TF_XTABS

/* these are here only to let old code compile - they are never set */
#define	RAW		LITOUT
#define	PASS8		LITOUT

/*
 * Hardware bits.
 * SHOULD NOT BE HERE.
 */
#define	DONE	0200
#define	IENABLE	0100

/*
 * Modem control commands.
 */
#define	DMSET		0
#define	DMBIS		1
#define	DMBIC		2
#define	DMGET		3

/*
 * Fake 'line discipline' switch, for the benefit of old code
 * that wants to call through it.
 */
struct ldisc_switch {
	int	(*l_read)();	/* read */
	int	(*l_write)();	/* write */
	int	(*l_rint)();	/* single character input */
	int	(*l_modem)();	/* modem change */
	int	(*l_start)();	/* start output */
};

extern struct ldisc_switch	linesw[];

#endif	/* _DEVICE_TTY_H_ */
