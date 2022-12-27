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
 * $Log:	serial_console.c,v $
 * Revision 2.2  91/08/24  11:53:18  af
 * 	Created, splitting out most code from dz_tty.c file.
 * 	[91/07/07            af]
 * 
 */
/*
 *	File: serial_console.c
 * 	Author: Alessandro Forin, Carnegie Mellon University
 *	Date:	7/91
 *
 *	Console driver for serial-line based consoles.
 */

#include <constty.h>
#if	NCONSTTY > 0
#include <bm.h>
#include <platforms.h>

#include <mach_kdb.h>

#include <machine/machparam.h>		/* spl definitions */
#include <sys/types.h>
#include <device/io_req.h>
#include <device/tty.h>
#include <sys/syslog.h>

#include <chips/busses.h>
#include <chips/screen_defs.h>
#include <chips/serial_defs.h>

#ifdef	DECSTATION
#include <mips/prom_interface.h>
#define	CONSOLE_SERIAL_LINE_NO	3
#endif	/*DECSTATION*/

#ifdef	VAXSTATION
#define	CONSOLE_SERIAL_LINE_NO	3

#define cnputc ser_putc
#define cngetc ser_getc
#define cnpollc ser_pollc
#define cnmaygetc ser_maygetc

#endif	/*VAXSTATION*/

#ifndef	CONSOLE_SERIAL_LINE_NO
#define	CONSOLE_SERIAL_LINE_NO	0
#endif

/* Size this as max possible number of lines in any serial chip we might use */
static struct tty 	console_tty_data[NCONSTTY];
struct tty		*console_tty[NCONSTTY];	/* exported */

#define	DEFAULT_SPEED	B9600
#define	DEFAULT_FLAGS	(TF_EVENP|TF_ODDP|TF_ECHO)


/*
 * A machine MUST have a console.  In our case
 * things are a little complicated by the graphic
 * display: people expect it to be their "console",
 * but we'd like to be able to live without it.
 * This is not to be confused with the "rconsole" thing:
 * that just duplicates the console I/O to
 * another place (for debugging/logging purposes).
 *
 * There is then another historical kludge: if
 * there is a graphic display it is assumed that
 * the minor "1" is the mouse, with some more
 * magic attached to it.  And again, one might like to
 * use the serial line 1 as a regular one.
 *
 */
#define	user_console	makedev(0,0)

dev_t	console = makedev(0,0);

int	(*console_probe)(),
	(*console_param)(),
	(*console_start)(),
	(*console_putc)(),
	(*console_getc)(),
	(*console_pollc)(),
	(*console_mctl)(),
	(*console_softCAR)();

/*
 * Lower-level (internal) interfaces, for printf and gets
 */
int cnunit = 0;		/* which unit owns the 'console' */
int cnline = 0;		/* which line of that unit */
int rcline = 3;		/* alternate, "remote console" line */

rcputc(c)
{
	if (rcline)
		(*console_putc)( cnunit, rcline, c);
}

cnputc(c)
{
#if	NBM > 0
	if (SCREEN_ISA_CONSOLE()) {
		/* this does its own rcputc */
		screen_blitc(SCREEN_CONS_UNIT(), c);
	} else
#endif	NBM > 0
	       {
		rcputc(c);
		(*console_putc)( cnunit, cnline, c);/* insist on a console still */
	}
	if (c == '\n')
		cnputc('\r');
}

cngetc()
{
	return (*console_getc)( cnunit, cnline, TRUE, FALSE);
}

cnpollc(bool)
{
	(*console_pollc)(cnunit, bool);
}

 
/* Debugger support */
cnmaygetc()
{
	return (*console_getc)( cnunit, cnline, FALSE, FALSE);
}


boolean_t
screen_captures(line)
	register int	line;
{
/* in screen.c ? */
	return (SCREEN_ISA_CONSOLE() &&
		((line == SCREEN_LINE_KEYBOARD) ||
		 (line == SCREEN_LINE_POINTER)));
}


/*
 * Higher level (external) interface, for GP use
 */


/*
 * This is basically a special form of autoconf,
 * to get printf() going before true autoconf.
 */
cons_find(tube)
int tube;
{
	static struct bus_device d;
	register int		 i;
	struct tty		*tp;

	for (i = 0; i < NCONSTTY; i++)
		console_tty[i] = &console_tty_data[i];
	/* the hardware device will set tp->t_addr for valid ttys */

	d.unit = 0;
	if (!(*console_probe)(0, &d)) {
		/* we have no console, but maybe that's ok */
#ifdef	DECSTATION
		/* no, it is not */
		dprintf("%s", "no console!\n");
		halt();
#endif	/*DECSTATION*/
		return 0;
	}

#ifdef	DECSTATION
	/*
	 * Remote console line
	 */
	{
		char *cp = prom_getenv("osconsole");
		if (cp && (string_to_int(cp) & 0x8))
			rcline = 3;
	}

#endif	/*DECSTATION*/

	/*
	 * Console always on unit 0. Fix if you need to
	 */
	cnunit = 0;

#if	NBM > 0
	if (tube && screen_probe(0)) {

		/* associate screen to console iff */
		if (console == user_console)
			screen_console = cnunit | SCREEN_CONS_ENBL;
		cnline = SCREEN_LINE_KEYBOARD;

		/* mouse and keybd */
		tp = console_tty[SCREEN_LINE_KEYBOARD];
		tp->t_ispeed = B4800;
		tp->t_ospeed = B4800;
		tp->t_flags = TF_LITOUT|TF_EVENP|TF_ECHO|TF_XTABS|TF_CRMOD;
		tp->t_dev = makedev(cnunit, SCREEN_LINE_KEYBOARD);
		(*console_param)(tp, SCREEN_LINE_KEYBOARD);

		tp = console_tty[SCREEN_LINE_POINTER];
		tp->t_ispeed = B4800;
		tp->t_ospeed = B4800;
		tp->t_flags = TF_LITOUT|TF_ODDP;
		tp->t_dev = makedev(cnunit, SCREEN_LINE_POINTER);
		(*console_param)(tp, SCREEN_LINE_POINTER);
		/* console_scan will turn on carrier */

	} else {
#endif	NBM > 0
		/* use non-graphic console as console */
		cnline = CONSOLE_SERIAL_LINE_NO;

		tp = console_tty[cnline];
		tp->t_ispeed = B9600;
		tp->t_ospeed = B9600;
		tp->t_flags = TF_LITOUT|TF_EVENP|TF_ECHO|TF_XTABS|TF_CRMOD;
		(*console_softCAR)(cnunit, cnline, TRUE);
		console = makedev(cnunit, cnline);
		tp->t_dev = console;
		(*console_param)(tp, SCREEN_LINE_OTHER);
#if	NBM > 0
	}

	/*
	 * Enable rconsole interrupts for KDB
	 */
	if (tube && rcline != cnline) {
		tp = console_tty[rcline];
		tp->t_ispeed = B9600;
		tp->t_ospeed = B9600;
		tp->t_flags = TF_LITOUT|TF_EVENP|TF_ECHO|TF_XTABS|TF_CRMOD;
		tp->t_dev = makedev(cnunit, rcline);
		(*console_softCAR)(cnunit, rcline, TRUE);
		(*console_param)(tp, SCREEN_LINE_OTHER);
	} else
	 	rcline = 0;
#endif	NBM > 0
}

/*
 * Open routine
 */
cons_open(dev, flag, ior)
	dev_t	dev;
	int	flag;
	io_req_t ior;
{
	register struct tty	*tp;
	register int		ttyno;
	extern			cons_start(), cons_stop();	/* forward */
 
	if (dev == user_console)
		dev = console;

	ttyno = minor(dev);
	if (ttyno >= NCONSTTY)
		return D_NO_SUCH_DEVICE;
	tp = console_tty[ttyno];
	tp->t_start	= cons_start;
	tp->t_stop	= cons_stop;

#if	NBM > 0
	if (screen_captures(ttyno))
		screen_open(SCREEN_CONS_UNIT(), ttyno==SCREEN_LINE_KEYBOARD);
#endif	NBM > 0

	if ((tp->t_state & TS_ISOPEN) == 0) {
		if (tp->t_ispeed == 0) {
			tp->t_ispeed = DEFAULT_SPEED;
			tp->t_ospeed = DEFAULT_SPEED;
			tp->t_flags = DEFAULT_FLAGS;
		}
		tp->t_dev = dev;
		(*console_param)(tp, ttyno);
	}
	(void) (*console_mctl)(dev, TM_DTR, DMSET);

	return (char_open(dev, tp, flag, ior));
}


/*
 * Close routine
 */
cons_close(dev, flag)
	dev_t dev;
{
	register struct tty	*tp;
	register int		ttyno;
	int s;
 
	if (dev == user_console)
		dev = console;

	ttyno = minor(dev);

#if	NBM > 0
	if (screen_captures(ttyno))
		screen_close(SCREEN_CONS_UNIT(), ttyno==SCREEN_LINE_KEYBOARD);
#endif	NBM > 0

	tp = console_tty[ttyno];

	s = spltty();
	simple_lock(&tp->t_lock);

	(void) (*console_mctl)(dev, TM_BRK, DMBIC);

	if ((tp->t_state&(TS_HUPCLS|TS_WOPEN)) || (tp->t_state&TS_ISOPEN)==0)
		(void) (*console_mctl)(dev, TM_HUP, DMSET);
	ttyclose(tp);

	simple_unlock(&tp->t_lock);
	splx(s);
}

cons_read(dev, ior)
	dev_t			dev;
	register io_req_t	ior;
{
	register struct tty	*tp;
	register 		ttyno;
 
	if (dev == user_console)
		dev = console;

	ttyno = minor(dev);
#if	NBM > 0
	if (SCREEN_ISA_CONSOLE() && (ttyno == SCREEN_LINE_POINTER))
		return screen_read(SCREEN_CONS_UNIT(), ior);
#endif	NBM > 0

	tp = console_tty[ttyno];
	return char_read(tp, ior);
}
 

cons_write(dev, ior)
	dev_t			dev;
	register io_req_t	ior;
{
	register struct tty	*tp;
	register		ttyno;

	if (dev == user_console)
		dev = console;

	ttyno = minor(dev);
#if	NBM > 0
	if (screen_captures(ttyno))
		return screen_write(SCREEN_CONS_UNIT(), ior);
#endif	NBM > 0

	tp = console_tty[ttyno];
	return char_write(tp, ior);
}

/*
 * Start output on a line
 */
cons_start(tp)
	register struct tty *tp;
{
	int			s;
 
	s = spltty();
	if (tp->t_state & (TS_TIMEOUT|TS_BUSY|TS_TTSTOP))
		goto out;

	if (tp->t_outq.c_cc == 0)
		goto out;

	tp->t_state |= TS_BUSY;

	(*console_start)(tp);

out:
	splx(s);
}

/*
 * Stop output on a line.
 */
cons_stop(tp, flag)
	register struct tty *tp;
{
	register int s;

	s = spltty();
	if (tp->t_state & TS_BUSY) {
		if ((tp->t_state&TS_TTSTOP)==0)
			tp->t_state |= TS_FLUSH;
	}
	splx(s);
}
 

cons_portdeath(dev, port)
	dev_t	dev;
	mach_port_t port;
{
	if (dev == user_console)
		dev = console;
	return (tty_portdeath(console_tty[minor(dev)], port));
}

io_return_t
cons_get_status(dev, flavor, data, status_count)
	dev_t		dev;
	int		flavor;
	int *		data;		/* pointer to OUT array */
	unsigned int	*status_count;		/* out */
{
	register struct tty *tp;
	register int	ttyno;

	if (dev == user_console)
		dev = console;

	ttyno = minor(dev);

#if	NBM > 0
	if (screen_captures(ttyno) &&
	    (screen_get_status(SCREEN_CONS_UNIT(),
			flavor, data, status_count) == D_SUCCESS))
		return D_SUCCESS;
#endif	NBM > 0

	tp = console_tty[ttyno];

	switch (flavor) {
	    case TTY_MODEM:
		*data = (*console_mctl)(dev, 0, DMGET);
		*status_count = 1;
		break;
	    default:
		return (tty_get_status(tp, flavor, data, status_count));
	}
	return (D_SUCCESS);
}

io_return_t
cons_set_status(dev, flavor, data, status_count)
	dev_t		dev;
	int		flavor;
	int *		data;
	unsigned int	status_count;
{
	register struct tty *tp;
	register int	ttyno;

	if (dev == user_console)
		dev = console;

	ttyno = minor(dev);

#if	NBM > 0
	if (screen_captures(ttyno) &&
	    (screen_set_status(SCREEN_CONS_UNIT(),
			flavor, data, status_count) == D_SUCCESS))
		return D_SUCCESS;
#endif	NBM > 0

	tp  = console_tty[ttyno];

	switch (flavor) {
	    case TTY_MODEM:
		if (status_count < TTY_MODEM_COUNT)
		    return (D_INVALID_OPERATION);
		(void) (*console_mctl)(dev, *data, DMSET);
		break;

	    case TTY_SET_BREAK:
		(void) (*console_mctl)(dev, TM_BRK, DMBIS);
		break;

	    case TTY_CLEAR_BREAK:
		(void) (*console_mctl)(dev, TM_BRK, DMBIC);
		break;

	    case TTY_STATUS:
	    {
		register int error;
		error = tty_set_status(tp, flavor, data, status_count);
		if (error == 0)
		    (*console_param)(tp, ttyno);
		return (error);
	    }
	    default:
		return (tty_set_status(tp, flavor, data, status_count));
	}
	return (D_SUCCESS);
}


/*
 * A simple scheme to dispatch interrupts.
 *
 * This deals with the fairly common case where we get an
 * interrupt on each rx/tx character.  A more elaborate
 * scheme [someday here too..] would handle instead many
 * characters per interrupt, perhaps using a DMA controller
 * or a large SILO.  Note that it is also possible to simulate
 * a DMA chip with 'pseudo-dma' code that runs directly down
 * in the interrupt routine.
 */
 
/*
 * We just received a character, ship it up for further processing.
 * Arguments are the tty number for which it is meant, a flag that
 * indicates a keyboard or mouse is potentially attached to that
 * tty (-1 if not), the character proper stripped down to 8 bits,
 * and an indication of any error conditions associated with the
 * receipt of the character.
 * We deal here with rconsole input handling and dispatching to
 * mouse or keyboard translation routines. cons_input() does
 * the rest.
 */
#if	MACH_KDB
int	l3break = 0x10;		/* dear old ^P, we miss you so bad. */
#endif	MACH_KDB

cons_simple_rint(ttyno, line, c, err)
	int		line;
	int		c;
{
	/*
	 * Rconsole. Drop in the debugger on break or ^P.
	 * Otherwise pretend input came from keyboard.
	 */
	if (rcline && ttyno == rcline) {
#if	MACH_KDB
		if ((err & CONS_ERR_BREAK) ||
		    ((c & 0x7f) == l3break))
			return gimmeabreak();
#endif	MACH_KDB
		ttyno = minor(console);
		goto process_it;
	}

#if	NBM > 0
	if (screen_captures(line)) {
		if (line == SCREEN_LINE_POINTER)
			return mouse_input(SCREEN_CONS_UNIT(), c);
		if (line == SCREEN_LINE_KEYBOARD) {
			c = lk201_rint(SCREEN_CONS_UNIT(), c, FALSE, FALSE);
			if (c == -1)
				return; /* shift or bad char */
		}
	}
#endif	NBM > 0
process_it:
	cons_input(ttyno, c, err);
}

/*
 * Send along a character on a tty.  If we were waiting for
 * this char to complete the open procedure do so; check
 * for errors; if all is well proceed to ttyinput().
 */
cons_input(ttyno, c, err)
{
	register struct tty *tp;

	tp  = console_tty[ttyno];

	if ((tp->t_state & TS_ISOPEN) == 0) {
		tt_open_wakeup(tp);
		return;
	}
	if (err & CONS_ERR_OVERRUN)
		log(LOG_WARNING, "sl%d: silo overflow\n", ttyno);

	if (err & CONS_ERR_PARITY)	
		if (((tp->t_flags & (TF_EVENP|TF_ODDP)) == TF_EVENP)
		  || ((tp->t_flags & (TF_EVENP|TF_ODDP)) == TF_ODDP))
			return;
	if (err & CONS_ERR_BREAK)	/* XXX autobaud XXX */
		c = tp->t_breakc;

	ttyinput(c & 0xff, tp);
}
 
/*
 * Transmission of a character is complete.
 * Return the next character or -1 if none.
 */
cons_simple_tint(ttyno)
{
	register struct tty *tp;
 
	tp = console_tty[ttyno];
	if (tp->t_addr == 0)	/* not probed --> stray */
		return -1;
	tp->t_state &= ~TS_BUSY;
	if (tp->t_state & TS_FLUSH)
		tp->t_state &= ~TS_FLUSH;

	cons_start(tp);
	if (tp->t_outq.c_cc == 0 || (tp->t_state&TS_BUSY)==0)
		return -1;

	return getc(&tp->t_outq);
}

 



#endif	/*NCONSTTY > 0*/
