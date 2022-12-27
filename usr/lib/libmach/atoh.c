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
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS  "AS IS"
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
 * any improvements or extensions that they make and grant Carnegie the
 * rights to redistribute these changes.
 */
/*
 *  HISTORY
 * $Log:	atoh.c,v $
 * Revision 2.2  91/04/11  11:07:11  mrt
 * 	Copied from libcs
 * 
 * Revision 1.2  90/12/11  17:50:07  mja
 * 	Add copyright/disclaimer for distribution.
 * 
 * 20-Nov-79  Steven Shafer (sas) at Carnegie-Mellon University
 *	Created for VAX.
 *
 */
/*
 *  File:	atoh  --  convert ascii to hexidecimal
 *  Author: 	Steven Shafer, Carnegie Mellon University
 *  Date:	Nov. 1979
 *
 *  Usage:  i = atoh (string);
 *	unsigned int i;
 *	char *string;
 *
 *  Atoh converts the value contained in "string" into an
 *  unsigned integer, assuming that the value represents
 *  a hexidecimal number.
 *
 */

unsigned int atoh(ap)
char *ap;
{
	register char *p;
	register unsigned int n;
	register int digit,lcase;

	p = ap;
	n = 0;
	while(*p == ' ' || *p == '	')
		p++;
	while ((digit = (*p >= '0' && *p <= '9')) ||
		(lcase = (*p >= 'a' && *p <= 'f')) ||
		(*p >= 'A' && *p <= 'F')) {
		n *= 16;
		if (digit)	n += *p++ - '0';
		else if (lcase)	n += 10 + (*p++ - 'a');
		else		n += 10 + (*p++ - 'A');
	}
	return(n);
}
