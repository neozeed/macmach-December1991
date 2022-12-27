%{
/* 
 * Mach Operating System
 * Copyright (c) 1991,1990,1989,1988,1987 Carnegie Mellon University
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 * 
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS 
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
 * HISTORY
 * $Log:	config.l,v $
 * Revision 2.6  91/07/08  16:59:17  danner
 * 	Luna88k support.
 * 	[91/06/26            danner]
 * 
 * Revision 2.5  91/06/19  11:58:26  rvb
 * 	cputypes.h->platforms.h
 * 	[91/06/12  13:45:50  rvb]
 * 
 * Revision 2.4  91/02/05  17:52:50  mrt
 * 	Changed to new Mach copyright
 * 	[91/02/01  17:51:15  mrt]
 * 
 * Revision 2.3  90/08/27  22:09:18  dbg
 * 	Merge CMU changes into Tahoe release.
 * 	[90/07/18            dbg]
 * 
 * Revision 2.2  90/06/02  15:03:52  rpd
 * 	Removed dst, maxdsiz, maxusers, timezone.
 * 	[90/03/26  22:56:38  rpd]
 * 
 * Revision 2.1  89/08/03  18:11:48  rwd
 * Created.
 * 
 * Revision 2.6  89/02/25  19:21:27  gm0w
 * 	Changes for cleanup.
 * 
 * Revision 2.5  89/02/07  22:56:08  mwyoung
 * Code cleanup cataclysm.
 * 
 * Revision 2.4  89/01/23  22:21:52  af
 * 	Add mips keywords. Also make makevariables an alias for
 * 	makeoptions.
 * 	[89/01/09            rvb]
 * 
 * Revision 2.3  88/11/23  16:41:24  rpd
 * 	Changed the definition of WORD, to allow numbers
 * 	if the identifier starts with a capital letter.
 * 	[88/11/04  17:58:13  rpd]
 * 
 * 08-Jan-87  Robert Beck (beck) at Sequent Computer Systems, Inc.
 *	Added "bin" keyword.
 *
 * 27-Oct-86  Jonathan J. Chew (jjc) at Carnegie-Mellon University
 *	Merged in David Black's changes for the Multimax.
 *
 * 02-Jun-86  Jonathan J. Chew (jjc) at Carnegie-Mellon University
 *	Added rules for Sun VMEbus.
 *
 */

/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 *	@(#)config.l	5.5 (Berkeley) 6/18/88
 */

#include <ctype.h>
#include "y.tab.h"
#include "config.h"

#define tprintf if (do_trace) printf

/*
 * Key word table
 */

struct kt {
	char *kt_name;
	int kt_val;
} key_words[] = {
	{ "and",	AND },
	{ "args",	ARGS },
	{ "at",		AT },
	{ "config",	CONFIG },
	{ "controller",	CONTROLLER },
	{ "conftype",	CONF },
	{ "csr",	CSR },
	{ "device",	DEVICE },
	{ "disk",	DISK },
	{ "drive",	DRIVE },
	{ "dumps",	DUMPS },
	{ "flags",	FLAGS },
	{ "hz",		HZ },
	{ "ident",	IDENT },
	{ "major",	MAJOR },
	{ "makeoptions", MAKEOPTIONS },
	{ "makevariables", MAKEOPTIONS },
	{ "master",	MASTER },
	{ "mba",	MBA },
	{ "minor",	MINOR },
	{ "nexus",	NEXUS },
	{ "on",		ON },
	{ "options",	OPTIONS },
	{ "platform",	PLATFORM },
	{ "priority",	PRIORITY },
	{ "pseudo-device",PSEUDO_DEVICE },
	{ "root",	ROOT },
	{ "size",	SIZE },
	{ "slave",	SLAVE },
	{ "swap",	SWAP },
	{ "tape",	DEVICE },
	{ "trace",	TRACE },
	{ "uba",	UBA },
	{ "vector",	VECTOR },
	{ "lun",	LUN },			/* MMAX only */
	{ "slot",	SLOT },			/* MMAX only */
	{ "tape",	TAPE },			/* MMAX only */
	{ "bin",	BIN },			/* SQT ONLY */
#ifdef	mips
	{ "am",		ADDRMOD },		/* MIPS */
	{ "mbii",	MBII },			/* MIPS */
 	{ "vme",	VME },			/* MIPS */
#endif	mips
	{ 0, 0 },
};
%}
WORD	([A-Za-z_][-A-Za-z_]*|[A-Z][-A-Za-z_0-9]*)
%%
{WORD}		{
			int i;

			if ((i = kw_lookup(yytext)) == -1)
			{
				yylval.str = yytext;
				tprintf("id(%s) ", yytext);
				return ID;
			}
			tprintf("(%s) ", yytext);
			return i;
		}
\"[^"]+\"	{
			yytext[strlen(yytext)-1] = '\0';
			yylval.str = yytext + 1;
			return ID;
		}
0[0-7]*		{
			yylval.val = octal(yytext);
			tprintf("#O:%o ", yylval.val);
			return NUMBER;
		}
0x[0-9a-fA-F]+	{
			yylval.val = hex(yytext);
			tprintf("#X:%x ", yylval.val);
			return NUMBER;
		}
[1-9][0-9]*	{
			yylval.val = atoi(yytext);
			tprintf("#D:%d ", yylval.val);
			return NUMBER;
		}
[0-9]"."[0-9]*	{
			double atof();
			yylval.val = (int) (60 * atof(yytext) + 0.5);
			return FPNUMBER;
		}
"-"		{
			return MINUS;
		}
"?"		{
			yylval.val = -1;
			tprintf("? ");
			return NUMBER;
		}
\n/[ \t]	{
			yyline++;
			tprintf("\n... ");
		}
\n		{
			yyline++;
			tprintf("\n");
			return SEMICOLON;
		}
#.*		{	/* Ignored (comment) */;	}
[ \t]*		{	/* Ignored (white space) */;	}
";"		{	return SEMICOLON;		}
","		{	return COMMA;			}
"="		{	return EQUALS;			}
"@"		{	return AT;			}
.		{	return yytext[0];		}

vme16           {       return VME16D16;                }
vme24           {       return VME24D16;                }
vme32           {       return VME32D32;                }

vme16d16        {       return VME16D16;                }
vme24d16        {       return VME24D16;                }
vme32d16        {       return VME32D16;                }
vme16d32        {       return VME16D32;                }
vme24d32        {       return VME24D32;                }
vme32d32        {       return VME32D32;                }

%%
/*
 * kw_lookup
 *	Look up a string in the keyword table.  Returns a -1 if the
 *	string is not a keyword otherwise it returns the keyword number
 */

kw_lookup(word)
register char *word;
{
	register struct kt *kp;

	for (kp = key_words; kp->kt_name != 0; kp++)
		if (eq(word, kp->kt_name))
			return kp->kt_val;
	return -1;
}

/*
 * Number conversion routines
 */

octal(str)
char *str;
{
	int num;

	(void) sscanf(str, "%o", &num);
	return num;
}

hex(str)
char *str;
{
	int num;

	(void) sscanf(str+2, "%x", &num);
	return num;
}
