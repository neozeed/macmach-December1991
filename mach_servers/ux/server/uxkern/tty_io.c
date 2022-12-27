/* 
 * Mach Operating System
 * Copyright (c) 1991 Carnegie-Mellon University
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	tty_io.c,v $
 * Revision 2.9  91/03/12  21:12:05  dbg
 * 	Use new macros to distinguish block and character devices.
 * 	Fix tty_open to allocate a reply port for ttys opened only
 * 	for write (from Jonathan Chew).
 * 	[91/02/21            dbg]
 * 
 * Revision 2.8  90/10/25  15:07:37  rwd
 * 	Fix tty_{read,write}_reply to better handle error and data=0 conditions.
 * 	[90/10/21            rwd]
 * 	Check status on device_write.
 * 	[90/10/17            rwd]
 * 	Initialize tty structure at creation.
 * 	[90/10/15            rwd]
 * 
 * Revision 2.7  90/09/09  14:13:59  rpd
 * 	Deallocate the device port after closing it.
 * 	[90/08/23            rpd]
 * 
 * Revision 2.6  90/06/02  15:28:37  rpd
 * 	Converted to new IPC.
 * 	[90/03/26  20:23:58  rpd]
 * 
 * Revision 2.5  90/05/21  14:02:45  dbg
 * 	Fix bug in tty_select: pass device number to l_select, not tty
 * 	pointer.
 * 	[90/03/22            dbg]
 * 
 * Revision 2.4  89/09/15  15:29:44  rwd
 * 	Change includes
 * 	[89/09/11            rwd]
 * 	LITOUT is passed to device based on LITOUT & RAW here.
 * 	[89/09/01            rwd]
 * 
 * Revision 2.3  89/08/31  16:29:25  rwd
 * 	Open flags are FREAD,FWRITE etc when we get here!
 * 	[89/08/30            rwd]
 * 	Set terminal flags to 0 when grabbing a new tty struct
 * 	[89/08/29            rwd]
 * 
 * 		Only queue read to kernel if device is opened for READ.
 * 	[89/08/28            rwd]
 * 	Changed to use inband read
 * 	[89/08/15            rwd]
 * 
 * Revision 2.2  89/08/09  14:46:13  rwd
 * 	Added copyright to file by dbg.  Change to use inband write
 * 	requests.
 * 	[89/07/17            rwd]
 * 
 */
/*
 * Interface between MACH tty devices and BSD ttys.
 */
#include <cmucs.h>

#include <sys/param.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/ttyloc.h>
#include <sys/tty.h>
#include <sys/conf.h>
#include <sys/errno.h>
#include <sys/user.h>
#include <sys/file.h>
#include <sys/synch.h>

#include <uxkern/device.h>
#include <device/tty_status.h>

#include <uxkern/device_reply_hdlr.h>
#include <uxkern/device_utils.h>

/*
 * For tty, we store the tty structure pointer in the hash table.
 */
#define	tty_hash_enter(dev, tp)	\
		dev_number_hash_enter(XDEV_CHAR(dev), (char *)(tp))
#define	tty_hash_remove(dev)	\
		dev_number_hash_remove(XDEV_CHAR(dev))
#define	tty_hash_lookup(dev)	\
		((struct tty *)dev_number_hash_lookup(XDEV_CHAR(dev)))

/*
 * We cannot deallocate the tty when it is closed, since other
 * structures have handles that are not reference-counted (p->p_ttyp).
 * Instead, we just close the device port.
 */

/*
 * Exported version of tty_hash_lookup.
 */
struct tty *
tty_find_tty(dev)
	dev_t	dev;
{
	return (tty_hash_lookup(dev));
}

kern_return_t	tty_read_reply();	/* forward */
kern_return_t	tty_write_reply();	/* forward */
int		tty_start();		/* forward */

extern struct tty *	cons_tp;	/* console TTY */

#ifdef mac2
/* Note the non-obious use of TTY_SET_BREAK and TTY_CLEAR_BREAK to reset the
 * console.  The console device will reset when set status TTY_SET_BREAK.
 * Access to this is provide in two ways.
 * First, ioctl() can be used to TIOCSBRK and TIOCCBRK the console tty.
 * The TTY_SET_BREAK will not be issued until just before the TTY_CLEAR_BREAK.
 * The second way is to do only an ioctl() TIOCSBRK.  Then, just before the
 * next console tty read() or write(), the TTY_SET_BREAK will be issued.
 * The first way allows a simple reset of the console to be done with the
 * usual combination of TIOCSBRK and TIOCCBRK.
 * The second way allows the console reset to be delayed until the next
 * time an attempt is made to read or write to the console tty.
 * Note that select() can not cause the console device to reset.
 */
int console_break;
#endif

/*
 * Open tty.
 */
tty_open(dev, flag)
	dev_t	dev;
	int	flag;
{
	register struct tty *tp;
	boolean_t	new_tty;
	static int first=1;

	/*
	 * Check whether tty is already open.
	 */
	tp = tty_hash_lookup(dev);
	if (tp == 0) {
	    /*
	     * Create new TTY structure.
	     */
	    tp = (struct tty *)malloc(sizeof(struct tty));
	    bzero(tp, sizeof(struct tty));
	    tty_hash_enter(dev, tp);
	    tp->t_flags = 0;
	    tp->t_device_port = MACH_PORT_NULL;
	    tp->t_reply_port = MACH_PORT_NULL;

	    new_tty = TRUE;	/* may deallocate if open fails */
	}
	else
	    new_tty = FALSE;	/* old structure - may be pointers to it */
	    			/* cannot deallocate if open fails */

	if (tp->t_device_port == MACH_PORT_NULL) {
	    /*
	     * Device is closed - try to open it.
	     */
	    kern_return_t	rc;
	    mach_port_t	device_port;
	    dev_mode_t	mode;
	    char	name[32];

	    /* get string name from device number */
	    rc = cdev_name_string(dev, name);
	    if (rc != 0)
		return (rc);	/* bad name */

	    /* fix modes */
	    mode = 0;	/* XXX */
	    /* open device */
	    rc = device_open(device_server_port,
			     mode,
			     name,
			     &device_port);
	    if (rc != D_SUCCESS) {
		/*
		 * Deallocate tty structure, if newly created.
		 */
		if (new_tty) {
		    tty_hash_remove(dev);
		    free((char *)tp);
		}
		return (dev_error_to_errno(rc));
	    }

	    /*
	     * Check for alias of console.
	     */
	    if (cons_tp != 0 /* we use this to create cons_tp */
		    && device_port == cons_tp->t_device_port
		    && new_tty) {
		/*
		 * Opened console - use its tty
		 */
		tty_hash_remove(dev);
		free((char *)tp);

		tp = cons_tp;
		tty_hash_enter(dev, tp);
	    }
	    else {
		/*
		 * Save device-port and tty-structure for device number.
		 */
		tp->t_device_port = device_port;
	    }

	}

	tp->t_oproc = tty_start;

	if ((tp->t_state & TS_ISOPEN) == 0) {

	    struct tty_status	ttstat;
	    unsigned int	ttstat_count;

	    /*
	     * Set initial characters
	     */
	    ttychars(tp);

	    /*
	     * Get configuration parameters from device, put in tty structure
	     */
	    ttstat_count = TTY_STATUS_COUNT;
	    (void) device_get_status(tp->t_device_port,
				     TTY_STATUS,
				     (int *)&ttstat,
				     &ttstat_count);

	    tp->t_ispeed = ttstat.tt_ispeed;
	    tp->t_ospeed = ttstat.tt_ospeed;
	    if (ttstat.tt_flags & TF_EVENP)
		tp->t_flags |= EVENP;
	    if (ttstat.tt_flags & TF_ODDP)
		tp->t_flags |= ODDP;
	    if (ttstat.tt_flags & TF_ECHO)
		tp->t_flags |= ECHO;
	    if (ttstat.tt_flags & TF_CRMOD)
		tp->t_flags |= CRMOD;
	    if (ttstat.tt_flags & XTABS)
		tp->t_flags |= XTABS;

	    /*
	     * Pretend that carrier is always on, until I figure out
	     * how to do it right.
	     */
	    tp->t_state |= TS_CARR_ON; /* should get from TTY_STATUS */

	}
	else if (tp->t_state & TS_XCLUDE && u.u_uid != 0)
	    return (EBUSY);

	if (tp->t_reply_port == MACH_PORT_NULL) {
	    /*
	     * Create reply port for device read/write messages.
	     * Hook it up to device and tty.
	     */
	    tp->t_reply_port = mach_reply_port();
	    reply_hash_enter(tp->t_reply_port,
			     (char *)tp,
			     tty_read_reply,
			     tty_write_reply);
	}

	if ((flag & FREAD) && !(tp->t_state & TS_RQUEUED)) {
	    /*
	     * Post initial read.
	     */
	    (void) device_read_request_inband(tp->t_device_port,
					      tp->t_reply_port,
					      0,
					      0,	/* recnum */
					      TTHIWAT(tp));

	    tp->t_state |= TS_RQUEUED;
	}
	/*
	 * Wait for CARR_ON
	 */
	if (flag & O_NDELAY) {
	    tp->t_state |= TS_ONDELAY;
	}
	else {
	    while ((tp->t_state & TS_CARR_ON) == 0) {
		tp->t_state |= TS_WOPEN;
		sleep((caddr_t)&tp->t_rawq, TTIPRI);
		/*
		 * some devices sleep on t_state...	XXX
		 */
	    }
	}

	return ((*linesw[tp->t_line].l_open)(dev, tp));
}

tty_close(dev, flag)
	dev_t	dev;
	int	flag;
{
	register struct tty *tp;

	/* get tty structure and port from dev */
	tp = tty_hash_lookup(dev);

	(*linesw[tp->t_line].l_close)(tp);

	/*
	 * Do not close the console (special case HACK HACK)
	 */
	if (tp != cons_tp) {
	    /*
	     * Remove the reply port
	     */
	    reply_hash_remove(tp->t_reply_port);
	    (void) mach_port_mod_refs(mach_task_self(), tp->t_reply_port,
				      MACH_PORT_RIGHT_RECEIVE, -1);
	    tp->t_reply_port = MACH_PORT_NULL;

	    /*
	     * And close the device
	     */
	    (void) device_close(tp->t_device_port);
	    (void) mach_port_deallocate(mach_task_self(), tp->t_device_port);
	    tp->t_device_port = MACH_PORT_NULL;

	    /*
	     * Disable output
	     */
	    tp->t_oproc = 0;
	}
	/*
	 * Leave tty structure, but mark it closed.
	 */
	ttyclose(tp);
}

tty_read(dev, uio)
	dev_t	dev;
	struct uio *uio;
{
	register struct tty *tp;

	/* get tty from device */
	tp = tty_hash_lookup(dev);
#ifdef mac2
        if ((tp == cons_tp) && console_break) {
          int word;
          console_break = 0;
          (void)device_set_status(tp->t_device_port, TTY_SET_BREAK, &word, 0);
        }
#endif
	return ((*linesw[tp->t_line].l_read)(tp, uio));
}

tty_write(dev, uio)
	dev_t	dev;
	struct uio *uio;
{
	register struct tty *tp;

	/* get tty from device */
	tp = tty_hash_lookup(dev);
#ifdef mac2
/* disabled until macintosh emulator exit is fixed */
/*
        if ((tp == cons_tp) && console_break) {
          int word;
          console_break = 0;
          (void)device_set_status(tp->t_device_port, TTY_SET_BREAK, &word, 0);
        }
*/
#endif
	return ((*linesw[tp->t_line].l_write)(tp, uio));
}

tty_select(dev, rw)
	dev_t	dev;
	int	rw;
{
	register struct tty *tp;

	/* get tty from device */
	tp = tty_hash_lookup(dev);
	return ((*linesw[tp->t_line].l_select)(dev, rw));
}

tty_ioctl(dev, cmd, data, flag)
	dev_t	dev;
	int	cmd;
	caddr_t	data;
	int	flag;
{
	register struct tty *tp;
	int	error;

	struct tty_status	ttstat;
	unsigned int		ttstat_count;
	int			word;

	/* get tty from device */
	tp = tty_hash_lookup(dev);

	error = (*linesw[tp->t_line].l_ioctl)(tp, cmd, data, flag);
	if (error >= 0)
	    return (error);

	error = ttioctl(tp, cmd, data, flag);
	if (error >= 0) {
	    if (cmd == TIOCSETP || cmd == TIOCSETN || cmd == TIOCLBIS ||
		cmd == TIOCLBIC || cmd == TIOCLSET || cmd == TIOCHPCL) {
		/*
		 * set device parameters
		 */
		ttstat_count = TTY_STATUS_COUNT;
		(void) device_get_status(tp->t_device_port,
					 TTY_STATUS,
					 (int *)&ttstat,
					 &ttstat_count);

		switch (cmd) {
		    case TIOCSETP:
		    case TIOCSETN:
		    {
			register struct sgttyb *sg = (struct sgttyb *)data;
			ttstat.tt_ispeed = sg->sg_ispeed;
			ttstat.tt_ospeed = sg->sg_ospeed;
			if (sg->sg_flags & RAW) {
			    ttstat.tt_breakc = 0;
			    ttstat.tt_flags |= TF_LITOUT;
			} else {
			    ttstat.tt_breakc = tp->t_intrc;
			    if (!(tp->t_flags & LITOUT))
				ttstat.tt_flags &= ~TF_LITOUT;
			}
			if (sg->sg_flags & ODDP)
			    ttstat.tt_flags |= TF_ODDP;
			else
			    ttstat.tt_flags &= ~TF_ODDP;
			if (sg->sg_flags & EVENP)
			    ttstat.tt_flags |= TF_EVENP;
			else
			    ttstat.tt_flags &= ~TF_EVENP;

		    }
		    case TIOCLBIS:
		    {
			word = *(int *)data;

			if (word & (MDMBUF>>16))
			    ttstat.tt_flags |= TF_MDMBUF;
			if (word & (LITOUT>>16))
			    ttstat.tt_flags |= TF_LITOUT;
			if (word & (NOHANG>>16))
			    ttstat.tt_flags |= TF_NOHANG;
			break;
		    }
		    case TIOCLBIC:
		    {
			word = *(int *)data;

			if (word & (MDMBUF>>16))
			    ttstat.tt_flags &= ~TF_MDMBUF;
			if (word & (LITOUT>>16) && !(tp->t_flags & RAW))
			    ttstat.tt_flags &= ~TF_LITOUT;
			if (word & (NOHANG>>16))
			    ttstat.tt_flags &= ~TF_NOHANG;
			break;
		    }
		    case TIOCLSET:
		    {
			word = *(int *)data;

			if (word & (MDMBUF>>16))
			    ttstat.tt_flags |= TF_MDMBUF;
			else
			    ttstat.tt_flags &= ~TF_MDMBUF;
			if (word & (LITOUT>>16))
			    ttstat.tt_flags |= TF_LITOUT;
			else
			    if (!(tp->t_flags & RAW))
				ttstat.tt_flags &= ~TF_LITOUT;
			if (word & (NOHANG>>16))
			    ttstat.tt_flags |= TF_NOHANG;
			else
			    ttstat.tt_flags &= ~TF_NOHANG;
			break;
		    }
		    case TIOCHPCL:
			ttstat.tt_flags |= TF_HUPCLS;
			break;
		}
		(void) device_set_status(tp->t_device_port,
					 TTY_STATUS,
					 (int *)&ttstat,
					 ttstat_count);
	    }
	    return (error);
	}
	/* if command is one meant for device,
	   translate it into a device_set_status() and issue it.
	 */
	switch (cmd) {
	    case TIOCMODG:
	    case TIOCMGET:
		ttstat_count = TTY_MODEM_COUNT;
		(void) device_get_status(tp->t_device_port,
					 TTY_MODEM,
					 &word,
					 &ttstat_count);
		*(int *)data = word;
		break;
	    case TIOCMODS:
	    case TIOCMSET:
		word = *(int *)data;
		(void) device_set_status(tp->t_device_port,
					 TTY_MODEM,
					 &word,
					 TTY_MODEM_COUNT);
		break;
	    case TIOCMBIS:
	    case TIOCMBIC:
	    case TIOCSDTR:
	    case TIOCCDTR:
		ttstat_count = TTY_MODEM_COUNT;
		(void) device_get_status(tp->t_device_port,
					 TTY_MODEM,
					 &word,
					 &ttstat_count);
		switch (cmd) {
		    case TIOCMBIS:
			word |= *(int *)data;
			break;
		    case TIOCMBIC:
			word &= ~*(int *)data;
			break;
		    case TIOCSDTR:
			word |= TM_DTR;
			break;
		    case TIOCCDTR:
			word &= ~TM_DTR;
			break;
		}
		(void) device_set_status(tp->t_device_port,
					 TTY_MODEM,
					 &word,
					 TTY_MODEM_COUNT);
		break;
	    case TIOCSBRK:
#ifdef mac2
                if (tp == cons_tp) {
                  console_break++;
                  break;
                }
#endif
		(void) device_set_status(tp->t_device_port,
					 TTY_SET_BREAK,
					 &word,
					 0);
		break;
	    case TIOCCBRK:
#ifdef mac2
                if ((tp == cons_tp) && console_break) {
                  console_break = 0;
                  (void)device_set_status(tp->t_device_port,
                                          TTY_SET_BREAK,
                                          &word,
                                          0);
                }
#endif
		(void) device_set_status(tp->t_device_port,
					 TTY_CLEAR_BREAK,
					 &word,
					 0);
		break;
	    default:
	    {
		/*
		 * Not one of the TTY ioctls - try sending
		 * the code to the device, and see what happens.
		 */
		unsigned int	count;

		count = (cmd & ~(IOC_INOUT|IOC_VOID)) >> 16; /* bytes */
		count = (count + 3) >> 2;		     /* ints */
		if (count == 0)
		    count = 1;

		if (cmd & (IOC_VOID|IOC_IN)) {
		    error = device_set_status(tp->t_device_port,
					      cmd,
					      (int *)data,
					      count);
		    if (error)
			return (ENOTTY);
		}
		if (cmd & IOC_OUT) {
		    error = device_get_status(tp->t_device_port,
					      cmd,
					      (int *)data,
					      &count);
		    if (error)
			return (ENOTTY);
		break;
		}
	    }
	}

	return (0);
}

tty_stop(tp, rw)
	register struct tty *tp;
	int	rw;
{
	int	s;

	s = spltty();

	if (tp->t_state & TS_BUSY) {
	    (void) device_set_status(tp->t_device_port,
				     (rw) ? TTY_FLUSH : TTY_STOP,
				     (int *)&rw,
				     1);
	    if ((tp->t_state & TS_TTSTOP) == 0)
		tp->t_state |= TS_FLUSH;
	}
	splx(s);
}

tty_read_reply(tp_ptr, error, data, data_count)
	char *		tp_ptr;
	int		error;
	char		data[];
	unsigned int	data_count;
{
	register struct tty *tp = (struct tty *)tp_ptr;
	register int	i;
	int		s;

	if (!error && data_count > 0) {
	    interrupt_enter(SPLTTY);
	    for (i = 0; i < data_count; i++)
		(*linesw[tp->t_line].l_rint)(data[i], tp);
	    interrupt_exit(SPLTTY);
	} else if (error) {
	    printf("tty_read_reply: error = %x\n",error);
	    panic("tty_read_reply");
	}

	(void) device_read_request_inband(tp->t_device_port,
				   tp->t_reply_port,
				   0,		/* mode */
				   0,		/* recnum */
				   TTHIWAT(tp));
}

tty_write_reply(tp_ptr, error, bytes_written)
	char *		tp_ptr;
	int		error;
	int		bytes_written;
{
	register struct tty *tp = (struct tty *)tp_ptr;
	int		s;

	if (error) {
	    printf("tty_write_reply: error = %x\n",error);
	    panic("tty_write_reply");
	    bytes_written = 0;
	}

	interrupt_enter(SPLTTY);

	tp->t_state &= ~TS_BUSY;
	if (tp->t_state & TS_FLUSH)
	    tp->t_state &= ~TS_FLUSH;
	else
	    ndflush(&tp->t_outq, bytes_written);

	if (tp->t_line)
	    (*linesw[tp->t_line].l_start)(tp);
	else
	    tty_start(tp);

	interrupt_exit(SPLTTY);

}

tty_start(tp)
	register struct tty *tp;
{
	int	cc, s;
	kern_return_t result;

	s = spltty();
	if ((tp->t_state & (TS_TIMEOUT|TS_BUSY|TS_TTSTOP)) == 0) {

	    if (tp->t_outq.c_cc <= TTLOWAT(tp)) {
		if (tp->t_state & TS_ASLEEP) {
		    tp->t_state &= ~TS_ASLEEP;
		    wakeup((caddr_t)&tp->t_outq);
		}
		selwakeup((caddr_t)&tp->t_wsel);
	    }

	    /* get characters from tp->t_outq,
	       send to device */
	    if (tp->t_outq.c_cc != 0) {

		cc = ndqb(&tp->t_outq, 0);	/* device handles timeouts! */
		/* This assumes inband data size is large enough. */
		result = device_write_request_inband(tp->t_device_port,
						     tp->t_reply_port,
						     0,	/* mode */
						     0,	/* recnum */
						     tp->t_outq.c_cf,
						     cc);
		if (result != KERN_SUCCESS) {
		    printf("tty_start: device_write_request result = %x\n",
			   result);
		    Debugger("tty_start");
		} else 
		    tp->t_state |= TS_BUSY;
	    }
	}
	splx(s);
}

/*ARGSUSED*/
struct tty *
nulltty(dev)
	dev_t	dev;
{
#if	CMUCS
	extern struct tty sytty[1];
	return &sytty[0];
#else	CMUCS
	return ((struct tty *)0);
#endif	CMUCS
}

