/* 
 * Mach Operating System
 * Copyright (c) 1991,1990,1989,1988 Carnegie Mellon University
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
 * $Log:	printf.c,v $
 * Revision 2.4  91/05/14  17:54:45  mrt
 * 	Correcting copyright
 * 
 * Revision 2.3  91/02/14  14:18:37  mrt
 * 	Added new Mach copyright
 * 	[91/02/13  12:44:48  mrt]
 * 
 * Revision 2.2  89/11/29  14:18:47  af
 * 	Characters are passed as ints, on Mips %c broke.
 * 	[89/10/28  10:28:25  af]
 * 
 * Revision 2.1  89/08/03  17:05:57  rwd
 * Created.
 * 
 *  8-Aug-88  David Golub (dbg) at Carnegie-Mellon University
 *	Converted for MACH kernel use.  Removed %r, %R, %b; added %b
 *	from Berkeley's kernel to print bit fields in device registers;
 *	changed to use varargs.
 *
 */

/*
 *  Common code for printf et al.
 *
 *  The calling routine typically takes a variable number of arguments,
 *  and passes the address of the first one.  This implementation
 *  assumes a straightforward, stack implementation, aligned to the
 *  machine's wordsize.  Increasing addresses are assumed to point to
 *  successive arguments (left-to-right), as is the case for a machine
 *  with a downward-growing stack with arguments pushed right-to-left.
 *
 *  To write, for example, fprintf() using this routine, the code
 *
 *	fprintf(fd, format, args)
 *	FILE *fd;
 *	char *format;
 *	{
 *	_doprnt(format, &args, fd);
 *	}
 *
 *  would suffice.  (This example does not handle the fprintf's "return
 *  value" correctly, but who looks at the return value of fprintf
 *  anyway?)
 *
 *  This version implements the following printf features:
 *
 *	%d	decimal conversion
 *	%u	unsigned conversion
 *	%x	hexadecimal conversion
 *	%X	hexadecimal conversion with capital letters
 *	%o	octal conversion
 *	%c	character
 *	%s	string
 *	%m.n	field width, precision
 *	%-m.n	left adjustment
 *	%0m.n	zero-padding
 *	%*.*	width and precision taken from arguments
 *
 *  This version does not implement %f, %e, or %g.  It accepts, but
 *  ignores, an `l' as in %ld, %lo, %lx, and %lu, and therefore will not
 *  work correctly on machines for which sizeof(long) != sizeof(int).
 *  It does not even parse %D, %O, or %U; you should be using %ld, %o and
 *  %lu if you mean long conversion.
 *
 *  This version implements the following nonstandard features:
 *
 *	%b	binary conversion
 *	%r	roman numeral conversion
 *	%R	roman numeral conversion with capital letters
 *
 *  As mentioned, this version does not return any reasonable value.
 *
 *  Permission is granted to use, modify, or propagate this code as
 *  long as this notice is incorporated.
 *
 *  Steve Summit 3/25/87
 */

/*
 * Added formats for decoding device registers:
 *
 * printf("reg = %b", regval, "<base><arg>*")
 *
 * where <base> is the output base expressed as a control character:
 * i.e. '\10' gives octal, '\20' gives hex.  Each <arg> is a sequence of
 * characters, the first of which gives the bit number to be inspected
 * (origin 1), and the rest (up to a control character (<= 32)) give the
 * name of the register.  Thus
 *	printf("reg = %b\n", 3, "\10\2BITTWO\1BITONE")
 * would produce
 *	reg = 3<BITTWO,BITONE>
 *
 * If the second character in <arg> is also a control character, it
 * indicates the last bit of a bit field.  In this case, printf will extract
 * bits <1> to <2> and print it.  Characters following the second control
 * character are printed before the bit field.
 *	printf("reg = %b\n", 0xb, "\10\4\3FIELD1=\2BITTWO\1BITONE")
 * would produce
 *	reg = b<FIELD1=2,BITONE>
 */

#include <mach/boolean.h>
#include <sys/varargs.h>

#define isdigit(d) ((d) >= '0' && (d) <= '9')
#define Ctod(c) ((c) - '0')

#define MAXBUF (sizeof(long int) * 8)		 /* enough for binary */

_doprnt(fmt, argp, fd)
	register char *fmt;
	va_list *argp;
	char *fd;		/* generic pointer for output */
{
	register char *p;
	char *p2;
	int length;
	int prec;
	int ladjust;
	char padc;
	int n;
	unsigned int u;
	int base;
	int negflag;
	char *s;
	int i, any;
	char c;

	while (*fmt != '\0') {
	    if (*fmt != '%') {
		putchar(*fmt++, fd);
		continue;
	    }

	    fmt++;

	    if (*fmt == 'l')
		fmt++;	     /* need to use it if sizeof(int) < sizeof(long) */

	    length = 0;
	    prec = -1;
	    ladjust = FALSE;
	    padc = ' ';

	    if (*fmt == '-') {
		ladjust = TRUE;
		fmt++;
	    }

	    if (*fmt == '0') {
		padc = '0';
		fmt++;
	    }

	    if (isdigit(*fmt)) {
		while(isdigit(*fmt))
		    length = 10 * length + Ctod(*fmt++);
	    }
	    else if (*fmt == '*') {
		length = va_arg(*argp, int);
		fmt++;
		if (length < 0) {
		    ladjust = !ladjust;
		    length = -length;
		}
	    }

	    if (*fmt == '.') {
		fmt++;
		if (isdigit(*fmt)) {
		    prec = 0;
		    while(isdigit(*fmt))
			prec = 10 * prec + Ctod(*fmt++);
		}
		else if (*fmt == '*') {
		    prec = va_arg(*argp, int);
		    fmt++;
		}
	    }

	    negflag = FALSE;

	    switch(*fmt) {
		case 'b':
		case 'B':
		    u = va_arg(*argp, unsigned int);
		    p = va_arg(*argp, char *);
		    base = *p++;
		    printnum(u, base, FALSE, 0, FALSE, ' ', fd);

		    if (u == 0)
			break;

		    any = FALSE;
		    while (i = *p++) {
			if (*p <= 32) {
			    /*
			     * Bit field
			     */
			    register int j;
			    if (any)
				putchar(',', fd);
			    else {
				putchar('<', fd);
				any = TRUE;
			    }
			    j = *p++;
			    for (; (c = *p) > 32; p++)
				putchar(c, fd);
			    printnum((unsigned)( (u>>(j-1)) & ((2<<(i-j))-1)),
					base, FALSE, 0, FALSE, ' ', fd);
			}
			else if (u & (1<<(i-1))) {
			    if (any)
				putchar(',', fd);
			    else {
				putchar('<', fd);
				any = TRUE;
			    }
			    for (; (c = *p) > 32; p++)
				putchar(c, fd);
			}
			else {
			    for (; *p > 32; p++)
				continue;
			}
		    }
		    if (any)
			putchar('>', fd);
		    break;

		case 'c':
		    c = va_arg(*argp, int);
		    putchar(c, fd);
		    break;

		case 'd':
		case 'D':
		    n = va_arg(*argp, int);

		    if (n >= 0)
			u = n;
		    else {
			u = -n;
			negflag = TRUE;
		    }
		    printnum(u, 10, negflag, length, ladjust, padc, fd);
		    break;

		case 'o':
		case 'O':
		    u = va_arg(*argp, unsigned int);
		    printnum(u, 8, FALSE, length, ladjust, padc, fd);
		    break;

		case 's':
		    p = va_arg(*argp, char *);

		    if (p == (char *)0)
			p = "(NULL)";

		    if (length > 0 && !ladjust) {
			n = 0;
			p2 = p;

			for (; *p != '\0' && (prec == -1 || n < prec); p++)
			    n++;

			p = p2;

			while (n < length) {
			    putchar(' ', fd);
			    n++;
			}
		    }

		    n = 0;

		    while (*p != '\0') {
			if (++n > prec && prec != -1)
			    break;

			putchar(*p++, fd);
		    }

		    if (n < length && ladjust) {
			while (n < length) {
			    putchar(' ', fd);
			    n++;
			}
		    }

		    break;

		case 'u':
		case 'U':
		    u = va_arg(*argp, unsigned int);
		    printnum(u, 10, FALSE, length, ladjust, padc, fd);
		    break;

		case 'x':
		    u = va_arg(*argp, unsigned int);
		    printnum(u, 16, FALSE, length, ladjust, padc, fd);
		    break;

		case '\0':
		    fmt--;
		    break;

		default:
		    putchar(*fmt, fd);
	    }
	fmt++;
	}
}

printnum(u, base, negflag, length, ladjust, padc, fd)
	register unsigned int	u;	/* number to print */
	register int		base;
	boolean_t		negflag;
	register int		length;
	boolean_t		ladjust;
	char			padc;
	char *			fd;
{
	char	buf[MAXBUF];	/* build number here */
	register char *	p = &buf[MAXBUF-1];
	register int	size;
	static char digs[] = "0123456789abcdef";

	do {
	    *p-- = digs[u % base];
	    u /= base;
	} while (u != 0);

	if (negflag)
	    putchar('-', fd);

	size = &buf[MAXBUF - 1] - p;

	if (size < length && !ladjust) {
	    while (length > size) {
		putchar(padc, fd);
		length--;
	    }
	}

	while (++p != &buf[MAXBUF])
	    putchar(*p, fd);

	if (size < length) {
	    /* must be ladjust */
	    while (length > size) {
		putchar(padc, fd);
		length--;
	    }
	}
}

/*
 * Printing (to console)
 */
/*VARARGS1*/
printf(fmt, va_alist)
	char *	fmt;
	va_dcl
{
	va_list	listp;
	va_start(listp);
	_doprnt(fmt, &listp, (char *)0);
	va_end(listp);
}

/*VARARGS2*/
char *
sprintf_x(s, fmt, va_alist)
	char *	s;		/* output string */
	char *	fmt;
	va_dcl
{
	va_list	listp;
	va_start(listp);
	_doprnt(fmt, &listp, s);
	va_end(listp);
	*s = 0;
	return (s);
}

/*
 *	Output routine.
 */
putchar(c, fd)
	char	c;		/* character to output */
	char	*fd;		/* output parameter */
{
	*fd++ = c;
}
