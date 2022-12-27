/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	cons.c,v $
 * Revision 2.7  91/03/20  15:04:52  dbg
 * 	Pay attention to amount written in cnflush - it might not be the
 * 	entire buffer.
 * 	[90/12/20            dbg]
 * 
 * Revision 2.6  90/06/02  15:27:11  rpd
 * 	Converted to new IPC.
 * 	[90/03/26  20:10:13  rpd]
 * 
 * Revision 2.5  90/05/21  14:02:08  dbg
 * 	Console is not always device (0,0).  Look for "console" in
 * 	cdevsw.  Still assumes that minor is 0.
 * 	[90/03/15            dbg]
 * 
 * Revision 2.4  89/12/08  20:18:34  rwd
 * 	Added cons_port().
 * 	[89/12/05  02:34:13  af]
 * 
 * Revision 2.3  89/09/15  15:29:16  rwd
 * 	Change includes
 * 	[89/09/11            rwd]
 * 
 * Revision 2.2  89/08/31  16:29:14  rwd
 * 	Have open use flags passed to it to open device.
 * 	[89/08/30            rwd]
 * 	Changed to use device_write_inband
 * 	[89/08/15            rwd]
 * 	Added spltty's around references to cons_buf.
 * 	[89/08/11            rwd]
 * 
 *
 */
/*
 * Console output.  Handles aliasing other ttys to console.
 */

#include <sys/param.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/ttyloc.h>
#include <sys/tty.h>
#include <sys/conf.h>
#include <sys/systm.h>
#include <sys/errno.h>
#include <sys/file.h>

#include <uxkern/device_utils.h>

struct tty	*cons_tp;

dev_t			cons_dev_number = NODEV;
mach_port_t		console_port;
extern mach_port_t	device_server_port;

extern struct tty *tty_find_tty();


cons_open(dev, flag)
	dev_t	dev;
	int	flag;
{
	register int error;

	if (cons_dev_number == NODEV) {
	    register int	m;
	    /*
	     * Look for console.
	     */
	    for (m = 0; m < nchrdev; m++) {
		if (!strcmp(cdevsw[m].d_name, "console")) {
		    cons_dev_number = makedev(m, 0);
		    break;
		}
	    }
	    if (cons_dev_number == NODEV) {
		panic("no console configured");
	    }
	}

	error = tty_open(cons_dev_number, flag);

	cons_tp = tty_find_tty(cons_dev_number);

	/* TTYLOC */
	if (cons_tp->t_ttyloc.tlc_hostid == 0) {
	    cons_tp->t_ttyloc.tlc_hostid = TLC_MYHOST;
	    cons_tp->t_ttyloc.tlc_ttyid  = TLC_CONSOLE;
	}

	return (error);
}

/* to satisfy console redirection */
cons_write(dev, uio)
	dev_t	dev;
	struct uio *uio;
{
	return (tty_write(dev, uio));
}

/* to satisfy console redirection */
cons_ioctl(dev, cmd, data, flag)
	dev_t	dev;
	int	cmd;
	caddr_t	data;
	int	flag;
{
	return (tty_ioctl(dev, cmd, data, flag));
}

mach_port_t
cons_port(dev)
	dev_t	dev;
{
	return cons_tp->t_device_port;
}

#define	CONS_BUF_SIZE	1024
char	cons_buf[CONS_BUF_SIZE];
int	cons_buf_pos = 0;

cnputc(c)
	int	c;
{
	int s;

	s = spltty();
	cons_buf[cons_buf_pos++] = c;
	if (cons_buf_pos >= CONS_BUF_SIZE)
	    cnflush();

	if (c == '\n')
	    cnputc('\r');
	(void) splx(s);
}

cnflush()
{
	unsigned int cw;
	int start_pos;
	int s;

	s = spltty();
	start_pos = 0;
	while (start_pos < cons_buf_pos) {
	    (void) device_write_inband(cons_tp->t_device_port,
			0, 0,
			&cons_buf[start_pos], cons_buf_pos - start_pos,
			&cw);
	    start_pos += cw;
	}
	cons_buf_pos = 0;
	(void) splx(s);
}

console_init()
{
	kern_return_t	rc;

	/*
	 * Open the console
	 */
	rc = device_open(device_server_port,
			 D_READ|D_WRITE,
			 "console",
			 &console_port);
	if (rc != KERN_SUCCESS)
	    panic("console_init");

}

