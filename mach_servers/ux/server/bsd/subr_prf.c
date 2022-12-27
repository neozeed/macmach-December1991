/* 
 * Mach Operating System
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 **********************************************************************
 * HISTORY
 * $Log:	subr_prf.c,v $
 * Revision 2.5  90/09/09  22:31:31  rpd
 * 	Removed redef of logwakeup.
 * 	[90/08/14            af]
 * 
 * Revision 2.4  90/05/29  20:23:26  rwd
 * 	Stop recursive panics.
 * 	[90/05/10            rwd]
 * 
 * Revision 2.3  89/10/17  11:26:27  rwd
 * 	Delay before Debugger call so print makes it to console.
 * 	[89/09/27            rwd]
 * 
 * Revision 2.2  89/09/15  15:28:37  rwd
 * 	Include uxkern/device.h instead of device/device.h
 * 	[89/09/11            rwd]
 * 
 * 	Untie console output from message buffer.
 * 	[89/05/11            dbg]
 * 
 * 	Added max-length format control to printf.
 * 	[89/02/03            dbg]
 * 
 * 	Out-of-kernel version.  NO CONDITIONALS!
 * 	[89/01/12            dbg]
 * 
 * Revision 2.1  89/08/04  14:07:32  rwd
 * Created.
 * 
 * Revision 2.6  88/12/19  02:34:23  mwyoung
 * 	Fix include file references.
 * 	[88/12/19            mwyoung]
 * 	
 * 	romp: Remove lint.
 * 	[88/12/17            mwyoung]
 * 
 * Revision 2.5  88/10/18  03:15:29  mwyoung
 * 	Start printing "<cpu_number>" before printf messages only once
 * 	a non-master processor has tried to print.
 * 	[88/09/13            mwyoung]
 * 
 * Revision 2.4  88/08/09  17:51:36  rvb
 * Use printf_lock so that multiple processors sharing the same
 * physical terminal don't garble their output.
 * Also: printf_cpu controls whether to printout the cpu number
 * before the print string and at each internal \n.
 * Bug fixes to %b printf mode for handling of "fields".
 * 
 *
 * 29-Mar-88  Michael Young (mwyoung) at Carnegie-Mellon University
 *	MACH: Removed use of "sys/vm.h".
 *
 * 13-Mar-88  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Reduce the number of errors flagged by HC.
 *
 * 29-Feb-88  Robert Baron (rvb) at Carnegie-Mellon University
 *	On sun3, panic early in boot just halts.
 *
 * 10-Nov-87  Mike Accetta (mja) at Carnegie-Mellon University
 *	Redirect all console output through putchar() to output through
 *	the `xcons_tp' terminal when it is defined rather than to the
 *	physical console.
 *	[ V5.1(XF21) ]
 *
 * 30-Sep-87  Richard Sanzi (sanzi) at Carnegie-Mellon University
 *	Added include of "machine/cpu.h" for the definition of
 *	cpu_number() (for non-multiprocessor systems).
 *
 * 31-Aug-87  David Black (dlb) at Carnegie-Mellon University
 *	MACH: multiprocessor panic logic.  If a second panic occurs
 *	on a different cpu, immediately stop that cpu.
 *
 * 27-Apr-87  Bill Bolosky (bolosky) at Carnegie-Mellon University
 *	Changed dependency on command-line defined symbol RDB to
 *	include file symbol ROMP_RDB.  Include romp_rdb.h if compiling
 *	for romp.
 *
 * 27-Mar-87  Robert Baron (rvb) at Carnegie-Mellon University
 *	allow a field size spec after the % for numeric fields
 *
 * 30-Jan-87  Mike Accetta (mja) at Carnegie-Mellon University
 *	CMUCS:  Changed log() in the absence of a syslog daemon to
 *	only log messages on the console which are LOG_WARNING or
 *	higher priority (this requires a companion change to the syslog
 *	configuration file in order to have the same effect when syslog
 *	is running).
 *	[ V5.1(F1) ]
 *
 * 08-Jan-87  Robert Beck (beck) at Sequent Computer Systems, Inc.
 *	Add BALANCE case to panic().
 *
 * 21-Oct-86  Jonathan J. Chew (jjc) at Carnegie-Mellon University
 *	Merged in change for Sun to trap into kdb in panic().
 *
 *  7-Oct-86  David L. Black (dlb) at Carnegie-Mellon University
 *	Merged in Multimax changes; mostly due to register parameter passing.
 *
 * 13-Apr-86  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	CMUCS:  print table full messages on console in addition to
 *	loging them.
 *
 * 15-Feb-86  Bill Bolosky (bolosky) at Carnegie-Mellon University
 *	Added different definitions of the parameter list for printf and
 *	uprintf under the assumption that the Sailboat c compiler needs
 *	them.  Switched on by romp.
 *
 * 25-Jan-86  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Upgraded to 4.3.
 *
 *  8-Sep-85  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	On a panic, issue a break point trap if kernel debugger present
 *	(and active).
 *
 * 26-Jun-85  Mike Accetta (mja) at Carnegie-Mellon University
 *	CMUCS:  Modified panic() to generate a breakpoint trap
 *	before rebooting.
 *	[V1(1)]
 *
 * 14-Jun-85  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Added (and modified) mprintf from Ultrix.
 *
 * 15-Sep-84  Robert V Baron (rvb) at Carnegie-Mellon University
 *	Spiffed up printf b format. you can also give an entry of the form
 *	\#1\#2Title
 *	will print Title then extract the bits from #1 to #2 as a field and
 *	print it -- some devices have fields in their csr. consider
 *	vaxmpm/mpmreg.h as an example.
 **********************************************************************
 */

/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)subr_prf.c	7.1 (Berkeley) 6/5/86
 */

#include <sys/varargs.h>

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/buf.h>
#include <sys/conf.h>
#include <sys/reboot.h>
#include <sys/msgbuf.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/proc.h>
#include <sys/ioctl.h>
#include <sys/tty.h>
#include <sys/syslog.h>

#include <uxkern/import_mach.h>

#include <uxkern/device.h>

#define TOCONS	0x1
#define TOTTY	0x2
#define TOLOG	0x4
#define	TOSTR	0x8

/*
 * In case console is off,
 * panicstr contains argument to last
 * call to panic.
 */
char	*panicstr;
int	paniced = 0;

/*
 * Lock for print buffer.
 */
struct mutex	printf_lock = MUTEX_INITIALIZER;

/*
 * Scaled down version of C Library printf.
 * Used to print diagnostic information directly on console tty.
 * Since it is not interrupt driven, all system activities are
 * suspended.  Printf should not be used for chit-chat.
 *
 * One additional format: %b is supported to decode error registers.
 * Usage is:
 *	printf("reg=%b\n", regval, "<base><arg>*");
 * Where <base> is the output base expressed as a control character,
 * e.g. \10 gives octal; \20 gives hex.  Each arg is a sequence of
 * characters, the first of which gives the bit number to be inspected
 * (origin 1), and the next characters (up to a control character, i.e.
 * a character <= 32), give the name of the register.  Thus
 *	printf("reg=%b\n", 3, "\10\2BITTWO\1BITONE\n");
 * would produce output:
 *	reg=3<BITTWO,BITONE>
 */
/*VARARGS1*/
printf(fmt, va_alist)
	char *fmt;
	va_dcl
{
	va_list	listp;

	mutex_lock(&printf_lock);

	va_start(listp);
	prf(fmt, &listp, TOCONS | TOLOG, (struct tty *)0);
	va_end(listp);
	logwakeup();
	cnflush();

	mutex_unlock(&printf_lock);
}

/*
 * Uprintf prints to the current user's terminal.
 * It may block if the tty queue is overfull.
 * Should determine whether current terminal user is related
 * to this process.
 */
/*VARARGS1*/
uprintf(fmt, va_alist)
	char *fmt;
	va_dcl
{
#ifdef notdef
	register struct proc *p;
#endif
	register struct tty *tp;

	va_list	listp;

	if ((tp = u.u_ttyp) == NULL)
		return;
#ifdef notdef
	if (tp->t_pgrp && (p = pfind(tp->t_pgrp)))
		if (p->p_uid != u.u_uid)	/* doesn't account for setuid */
			return;
#endif
	(void)ttycheckoutq(tp, 1);
	va_start(listp);
	prf(fmt, &listp, TOTTY, tp);
	va_end(listp);
}

/*
 * Log writes to the log buffer,
 * and guarantees not to sleep (so can be called by interrupt routines).
 * If there is no process reading the log yet, it writes to the console also.
 */
int	log_open;

/*VARARGS2*/
log(level, fmt, va_alist)
	char *fmt;
	va_dcl
{
	va_list	listp;

	mutex_lock(&printf_lock);

	logpri(level);

	va_start(listp);
	prf(fmt, &listp, TOLOG, (struct tty *)0);
	va_end(listp);

	logwakeup();

	if (!log_open) {
	    if ((level&LOG_PRIMASK) <= LOG_WARNING) {
		va_start(listp);
		prf(fmt, &listp, TOCONS, (struct tty *)0);
		va_end(listp);
		cnflush();
	    }
	}

	mutex_unlock(&printf_lock);
}

logpri(level)
	int level;
{

	putchar('<', TOLOG, (struct tty *)0);
	printn((u_long)level, 10, TOLOG, (struct tty *)0, 0, 0, 0);
	putchar('>', TOLOG, (struct tty *)0);
}

/*
 * Sprintf prints to a string buffer.
 */
/*VARARGS2*/
sprintf(s, fmt, va_alist)
	char s[];	/* out */
	char *fmt;
	va_dcl
{
	va_list	listp;
	char	*cp;

	cp = s;
	va_start(listp);
	prf(fmt, &listp, TOSTR, (struct tty *)&cp);
	va_end(listp);

	*cp = '\0';	/* end string */
}


prf(fmt, adx, flags, ttyp)
	register char *fmt;
	struct tty *ttyp;
	va_list	*adx;
{
	register int b, c, i;
	char *s;
	int any;
	int base;
	u_long	value;

	int zf, fld_size, precision, left_adjust;

loop:
	while ((c = *fmt++) != '%') {
	    if (c == '\0') {
		return;
	    }
	    putchar(c, flags, ttyp);
	}
again:
	c = *fmt++;

	zf = 0, fld_size = 0, precision = -1, left_adjust = 0;

	if (c == '-') {
	    left_adjust = 1;
	    c = *fmt++;
	}
	if (c == '0')
	    zf = '0';
	for (; c >= '0' && c <= '9'; c = *fmt++)
	    fld_size = fld_size * 10 + c - '0';

	if (c == '.') {
	    c = *fmt++;
	    if (c >= '0' && c <= '9') {
		precision = 0;
		for (; c >= '0' && c <= '9'; c = *fmt++)
		    precision = precision * 10 + c - '0';
	    }
	}

	/* THIS CODE IS VAX DEPENDENT IN HANDLING %l? AND %c */
	switch (c) {

	    case 'l':
		goto again;
	    case 'x': case 'X':
		b = 16;
		goto number;
	    case 'd': case 'D':
	    case 'u':		/* what a joke */
		b = 10;
		goto number;
	    case 'o': case 'O':
		b = 8;
number:
		value = va_arg(*adx, u_long);
		printn(value, b, flags, ttyp, zf, fld_size, left_adjust);
		break;
	    case 'c':
		value = va_arg(*adx, u_long);
		for (i = 24; i >= 0; i -= 8)
			if (c = (value >> i) & 0x7f)
				putchar(c, flags, ttyp);
		break;
	    case 'b':
		b = va_arg(*adx, int);
		s = va_arg(*adx, char *);

		base = *s++;
		printn((u_long)b, base, flags, ttyp, 0, 0, 0);
		any = 0;
		if (b) {
		    while (i = *s++) {
			if (*s <= 32) {
			    register int j;
			    register int fld;

			    j = *s++;
			    fld = ( (b >> (j-1)) & ( (2 << (i-j)) -1));
			    if (fld) {
				putchar((any++) ? ',' : '<', flags, ttyp);
				for (; (c = *s) > 32 ; s++)
				    putchar(c, flags, ttyp);
				putchar('=', flags, ttyp);
				printn( (u_long) fld, base, flags, ttyp,
					 0, 0, 0);
			    }
			    else
				for (; (c = *s) > 32 ; s++) /* skip name */;
			}
			else if (b & (1 << (i-1))) {
			    putchar( (any) ? ',' : '<', flags, ttyp);
			    any = 1;
			    for (; (c = *s) > 32; s++)
				putchar(c, flags, ttyp);
			}
			else
			    for (; *s > 32; s++)
				;
		    }
		    putchar('>', flags, ttyp);
		}
		break;

	    case 's':
		s = va_arg(*adx, char *);
		if (fld_size > 0 && !left_adjust) {
		    i = strlen(s);
		    if (i > precision)
			i = precision;
		    while (i < fld_size) {
			putchar(' ', flags, ttyp);
			i++;
		    }
		}
		i = 0;
		while (c = *s++) {
		    if (++i > precision && precision != -1)
			break;
		    putchar(c, flags, ttyp);
		}
		if (i < fld_size && left_adjust) {
		    while (i < fld_size) {
			putchar(' ', flags, ttyp);
			i++;
		    }
		}
		break;

	    case '%':
		putchar('%', flags, ttyp);
		break;
	}
	goto loop;
}

/*
 * Printn prints a number n in base b.
 * We don't use recursion to avoid deep kernel stacks.
 */
printn(n, b, flags, ttyp, zf, fld_size, left_adjust)
	u_long n;
	struct tty *ttyp;
{
	char prbuf[11];
	register char *cp;
	register int	size;

	if (b == 10 && (int)n < 0) {
	    putchar('-', flags, ttyp);
	    n = (unsigned)(-(int)n);
	}
	cp = prbuf;
	do {
	    *cp++ = "0123456789abcdef"[n%b];
	    n /= b;
	} while (n);
	size = cp - prbuf;

	if (size < fld_size && !left_adjust) {
	    for (; size < fld_size; fld_size--) {
		putchar((zf) ? '0' : ' ', flags, ttyp);
	    }
	}
	do
	    putchar(*--cp, flags, ttyp);
	while (cp > prbuf);

	if (left_adjust && size < fld_size) {
	    for (; size < fld_size; fld_size--) {
		putchar((zf) ? '0' : ' ', flags, ttyp);
	    }
	}
}

/*
 * Panic is called on unresolvable fatal errors.
 * It prints "panic: mesg", and then reboots.
 * If we are called twice, then we avoid trying to
 * sync the disks as this often leads to recursive panics.
 */
panic(s)
	char *s;
{
	int bootopt = RB_AUTOBOOT;

	if (panicstr)
		bootopt |= RB_NOSYNC;
	else {
		panicstr = s;
	}
	if (!paniced) {
	    paniced = 1;
	    printf("panic: %s\n", s);
	    paniced = 0;
	}

#ifdef mac2 /* cause system to hang after a panic */
	while (1);
#endif

	if (boothowto&RB_KDB) {
	    int i;
	    for(i=1;i<20000;i++);
	    Debugger("panic");
	} else
	    boot(RB_PANIC, bootopt);
}

/*
 * Warn that a system table is full.
 */
tablefull(tab)
	char *tab;
{

	printf("%s: table is full\n", (unsigned) tab);
	log(LOG_ERR, "%s: table is full\n", (unsigned) tab);
}

/*
 * Print a character on console or users terminal.
 * If destination is console then the last MSGBUFS characters
 * are saved in msgbuf for inspection later.
 */
/*ARGSUSED*/
putchar(c, flags, tp)
	register int c;
	struct tty *tp;
{

	if (flags & TOSTR) {
	    register char **cpp = (char **)tp;

	    *(*cpp)++ = c;
	    return;
	}

totty:
	if (flags & TOTTY) {
	    register s = spltty();

	    if (tp && (tp->t_state & (TS_CARR_ON | TS_ISOPEN)) ==
		    (TS_CARR_ON | TS_ISOPEN)) {
		if (c == '\n')
		    (void) ttyoutput('\r', tp);
		(void) ttyoutput(c, tp);
		ttstart(tp);
	    }
	    splx(s);
	}

	if ((flags & TOLOG) && c != '\0' && c != '\r' && c != 0177) {
	    if (msgbuf.msg_magic != MSG_MAGIC) {
		register int i;

		msgbuf.msg_magic = MSG_MAGIC;
		msgbuf.msg_bufx = msgbuf.msg_bufr = 0;
		for (i=0; i < MSG_BSIZE; i++)
		    msgbuf.msg_bufc[i] = 0;
	    }
	    msgbuf.msg_bufc[msgbuf.msg_bufx++] = c;
	    if (msgbuf.msg_bufx < 0 || msgbuf.msg_bufx >= MSG_BSIZE)
		msgbuf.msg_bufx = 0;

	}

	if ((flags & TOCONS) && c != '\0') {
	    extern struct tty *xcons_tp;

	    if (xcons_tp == 0) {
		cnputc(c);
	    }
	    else {
		flags = TOTTY;
		tp = xcons_tp;
		goto totty;
	    }
	}
}

