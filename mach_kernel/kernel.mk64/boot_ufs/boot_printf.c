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
 * $Log:	boot_printf.c,v $
 * Revision 2.8  91/05/14  15:20:47  mrt
 * 	Correcting copyright
 * 
 * Revision 2.7  91/03/13  16:50:19  rpd
 * 	Should initialize the "count" before a device_read_inband().
 * 	Bug found by laverne@sumitomo.com.
 * 	[91/03/13  15:03:15  af]
 * 
 * Revision 2.6  91/02/05  17:00:25  mrt
 * 	Changed to new copyright
 * 	[91/01/28  14:54:14  mrt]
 * 
 * Revision 2.5  90/10/25  14:41:34  rwd
 * 	Modified boot_gets to allocate arrays from heap, not stack.
 * 	[90/10/23            rpd]
 * 
 * Revision 2.4  90/08/27  21:44:20  dbg
 * 	Pass extra argument to _doprnt.
 * 	[90/08/21            dbg]
 * 
 * Revision 2.3  90/06/02  14:45:13  rpd
 * 	Converted to new IPC and new host port mechanism.
 * 	[90/03/26  21:28:27  rpd]
 * 
 * Revision 2.2  90/01/11  11:40:53  dbg
 * 	Created.
 * 	[89/12/20            dbg]
 * 
 */
/*
 * Printf from bootstrap task - use device interface.
 */
#include <mach/port.h>
#include <mach/mach_user_internal.h>
#include <mach/mach_port_internal.h>
#include <sys/varargs.h>
#include <sys/reboot.h>

#include <kern/host.h>
#include <kern/kalloc.h>

#include <device/device_types.h>

mach_port_t		console_port;
extern mach_port_t	bootstrap_master_device_port;

void
boot_printf_init()
{
	(void) device_open(bootstrap_master_device_port,
			   0,
			   "console",
			   &console_port);
}

#define	BOOT_PRINTF_BUFMAX	128
char	boot_printf_buf[BOOT_PRINTF_BUFMAX + 1]; /* extra for '\r\n' */
unsigned int	boot_printf_index = 0;

boot_putchar(c)
	int	c;
{
	boot_printf_buf[boot_printf_index] = c;
	boot_printf_index++;
	if (c == '\n') {
	    boot_printf_buf[boot_printf_index] = '\r';
	    boot_printf_index++;
	}

	if (boot_printf_index >= BOOT_PRINTF_BUFMAX) {
	    int	amt;
	    (void) device_write_inband(console_port, 0, 0,
			boot_printf_buf, boot_printf_index, &amt);
	    boot_printf_index = 0;
	}
}

/*VARARGS1*/
boot_printf(fmt, va_alist)
	char *	fmt;
	va_dcl
{
	va_list	listp;

	va_start(listp);
	_doprnt(fmt, &listp, boot_putchar, 0);
	va_end(listp);

	if (boot_printf_index != 0) {
	    int	amt;
	    (void) device_write_inband(console_port, 0, 0,
			boot_printf_buf, boot_printf_index, &amt);
	    boot_printf_index = 0;
	}
}

boot_gets(str, maxlen)
	char *str;
	int  maxlen;
{
	register char *lp;
	register int c;

	char *inbuf;
	unsigned int count;
	register char *ip;
	char *strmax = str + maxlen - 1; /* allow space for trailing 0 */

	inbuf = (char *) kalloc(IO_INBAND_MAX);
	lp = str;
	for (;;) {
	    count = IO_INBAND_MAX;
	    (void) device_read_inband(console_port, 0, 0,
				      IO_INBAND_MAX, inbuf, &count);
	    for (ip = inbuf; ip < &inbuf[count]; ip++) {
		c = *ip;
		switch (c) {
		    case '\n':
		    case '\r':
			boot_printf("\n");
			*lp++ = 0;
			kfree((vm_offset_t)inbuf, IO_INBAND_MAX);
			return;

		    case '\b':
		    case '#':
		    case '\177':
			if (lp > str) {
			    boot_printf("\b \b");
			    lp--;
			}
			continue;
		    case '@':
		    case 'u'&037:
			lp = str;
			boot_printf("\n\r");
			continue;
		    default:
			if (c >= ' ' && c < '\177') {
			    if (lp < strmax) {
				*lp++ = c;
				boot_printf("%c", c);
			    }
			    else {
				boot_printf("%c", '\007'); /* beep */
			    }
			}
		}
	    }
	}
}

/*VARARGS1*/
boot_panic(s, va_alist)
	char *s;
	va_dcl
{
	va_list listp;

	boot_printf("panic: ");
	va_start(listp);
	_doprnt(s, &listp, boot_putchar, 0);
	va_end(listp);
	boot_printf("\n");

	/*
	 * XXX - use internal form of host_reboot,
	 * since I am not exporting a 'host_internal'
	 * file yet.
	 */
	(void) host_reboot(realhost.host_priv_self, RB_DEBUGGER);
}
