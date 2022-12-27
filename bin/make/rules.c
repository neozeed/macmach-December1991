#ifndef lint
static char *RCSid = "$Id: rules.c,v 1.10 90/05/08 17:57:47 bww Exp $";
#endif

/*
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 ************************************************************************
 * HISTORY
 * $Log:	rules.c,v $
 *      Add more backwards compatability for mac2 in CMUCS environment.
 *      Add support for GNU .S files.
 * 	[91/04/24            zon]
 *
 * Revision 5.5  90/10/24  12:12:52  mrt
 * 	Define TARGET_MACHINE to be SUN3 for sun 3 to be consistent
 * 	with stump. The -m flag will now look in directories named
 * 	SUN3 for files and not SUN. The source trees should be
 * 	changed.
 * 	[90/10/24            mrt]
 * 
 * Revision 5.4  90/08/09  16:33:18  mrt
 * 	Changed TARGET_MACHINE on sun3 back to SUN at CMUCS for
 * 	backwards compatiblity with our source tree.
 * 	[90/08/09            mrt]
 * 
 * Revision 5.3  90/08/06  18:24:06  mrt
 * 	Added definitions of machine/MACHINE for compatibility with old-style 
 * 	Makefiles
 * 	[90/07/11            mrt]
 * 
 * Revision 1.10  90/05/08  17:57:47  bww
 * 	Purged "pwb" conditionals.
 * 	[90/05/08  17:57:24  bww]
 * 
 * Revision 1.9  90/02/22  17:39:27  bww
 * 	Modified i386 TARGET_MACHINE definitions.
 * 	[90/02/22  17:35:45  bww]
 * 
 * Revision 1.8  90/02/01  20:49:09  bww
 * 	Delinted.
 * 	[90/02/01  20:45:11  bww]
 * 
 * Revision 1.7  90/01/17  23:07:00  bww
 * 	Added definitions for i386 and mac2.
 * 	[90/01/17  23:06:39  bww]
 * 
 * Revision 1.6  89/12/19  16:05:31  bww
 * 	Disable old MACHINE/machine compatibility.
 * 	[89/12/19  16:04:27  bww]
 * 
 * Revision 1.5  89/12/02  23:00:00  bww
 * 	Use TARGET_MACHINE/target_machine instead of MACHINE/machine
 * 	(MACHINE clashed with the Tahoe definition).  However, retain
 * 	previous MACHINE/machine values as a temporary transition aid.
 * 	Also purged CPU.
 * 	[89/12/02  22:59:42  bww]
 * 
 * Revision 1.4  89/08/15  18:26:31  bww
 * 	Add PMAX (mips) symbol definitions.
 * 	From "[89/05/06            mja]" at CMU.
 * 	[89/08/15            bww]
 * 
 * Revision 1.3  89/05/26  17:06:35  bww
 * 	Removed CMU conditional definition of MACHINE/machine
 * 	on SUN3.  Renamed MACHINE variable to _MACHINE to avoid
 * 	clash with tahoe #define.
 * 	[89/05/26  17:06:16  bww]
 * 
 * Revision 1.2  89/05/26  10:05:23  bww
 * 	CMU CS as of 89/05/15
 * 	[89/05/26  09:46:50  bww]
 * 
 * Revision 5.1  89/05/12  19:11:07  bww
 * 	Converted the CPU, MACHINE, machine, CPUTYPE, and cputype
 * 	definitions into global fixed size arrays so that they could
 * 	be easily patched.
 * 	[89/05/12  19:10:51  bww]
 * 
 * Revision 5.0  89/03/01  01:41:08  bww
 * 	Version 5.
 * 	[89/03/01  01:33:21  bww]
 * 
 */

#ifndef lint
static	char *sccsid = "@(#)rules.c	3.1";
#endif

/*
 * DEFAULT RULES FOR UNIX
 *
 * These are the internal rules that "make" trucks around with it at
 * all times. One could completely delete this entire list and just
 * conventionally define a global "include" makefile which had these
 * rules in it. That would make the rules dynamically changeable
 * without recompiling make. This file may be modified to local
 * needs.
 */

#include "defs.h"

#ifdef pdp11
#	define target_MACHINE	"TARGET_MACHINE=PDP11"
#	define target_machine	"target_machine=pdp11"
#ifdef CMUCS
#	define this_MACHINE	"MACHINE=PDP11"
#	define this_machine	"machine=pdp11"
#endif /* CMUCS */
#	define this_CPUTYPE	"CPUTYPE=PDP11"
#	define this_cputype	"cputype=pdp11"
#endif
#ifdef vax
#	define target_MACHINE	"TARGET_MACHINE=VAX"
#	define target_machine	"target_machine=vax"
#ifdef CMUCS
#	define this_MACHINE	"MACHINE=VAX"
#	define this_machine	"machine=vax"
#endif /* CMUCS */
#	define this_CPUTYPE	"CPUTYPE=VAX"
#	define this_cputype	"cputype=vax"
#endif
#ifdef sun
#ifdef sparc
#	define target_MACHINE	"TARGET_MACHINE=SUN4"
#	define target_machine	"target_machine=sun4"
#ifdef CMUCS
#	define this_MACHINE	"MACHINE=SUN4"
#	define this_machine	"machine=sun4"
#endif /* CMUCS */
#	define this_CPUTYPE	"CPUTYPE=SPARC"
#	define this_cputype	"cputype=sparc"
#else
#ifdef mc68020
#ifdef CMUCS
#	define this_MACHINE	"MACHINE=SUN"  /* SUN3  old CMU way */
#	define this_machine	"machine=sun"  /* sun3 */
#endif /* CMUCS */
#	define target_MACHINE	"TARGET_MACHINE=SUN3"
#	define target_machine	"target_machine=sun3"
#	define this_CPUTYPE	"CPUTYPE=MC68020"
#	define this_cputype	"cputype=mc68020"
#else
#	define target_MACHINE	"TARGET_MACHINE=SUN2"
#	define target_machine	"target_machine=sun2"
#	define this_CPUTYPE	"CPUTYPE=MC68000"
#	define this_cputype	"cputype=mc68000"
#endif
#endif
#endif
#ifdef ibm032
#	define target_MACHINE	"TARGET_MACHINE=IBMRT"
#	define target_machine	"target_machine=ibmrt"
#ifdef CMUCS
#	define this_MACHINE	"MACHINE=IBMRT"
#	define this_machine	"machine=ibmrt"
#endif /* CMUCS */
#	define this_CPUTYPE	"CPUTYPE=IBM032"
#	define this_cputype	"cputype=ibm032"
#endif
#ifdef ibm370
#	define target_MACHINE	"TARGET_MACHINE=IBM"
#	define target_machine	"target_machine=ibm"
#	define this_CPUTYPE	"CPUTYPE=IBM370"
#	define this_cputype	"cputype=ibm370"
#endif
#ifdef ns32000
#ifdef balance
#	define target_MACHINE	"TARGET_MACHINE=BALANCE"
#	define target_machine	"target_machine=balance"
#	define this_CPUTYPE	"CPUTYPE=NS32032"
#	define this_cputype	"cputype=ns32032"
#endif
#ifdef MULTIMAX
#	define target_MACHINE	"TARGET_MACHINE=MMAX"
#	define target_machine	"target_machine=mmax"
#ifdef CMUCS
#	define this_MACHINE	"MACHINE=MMAX"
#	define this_machine	"machine=mmax"
#endif /* CMUCS */
#	define this_CPUTYPE	"CPUTYPE=NS32032"
#	define this_cputype	"cputype=ns32032"
#endif
#endif
#ifdef mips
#	define target_MACHINE	"TARGET_MACHINE=PMAX"
#	define target_machine	"target_machine=pmax"

#ifdef CMUCS
#	define this_MACHINE	"MACHINE=PMAX"
#	define this_machine	"machine=pmax"
#endif /* CMUCS */
#	define this_CPUTYPE	"CPUTYPE=MIPS"
#	define this_cputype	"cputype=mips"
#endif
#ifdef i386
#	define target_MACHINE	"TARGET_MACHINE=I386"
#	define target_machine	"target_machine=i386"
#ifdef CMUCS
#	define this_MACHINE	"MACHINE=I386"
#	define this_machine	"machine=i386"
#endif /* CMUCS */
#	define this_CPUTYPE	"CPUTYPE=I386"
#	define this_cputype	"cputype=i386"
#endif
#ifdef mac2
#	define target_MACHINE	"TARGET_MACHINE=MAC2"
#	define target_machine	"target_machine=mac2"
#ifdef CMUCS
#	define this_MACHINE	"MACHINE=mac2"
#	define this_machine	"machine=mac2"
#endif /* CMUCS */
#	define this_CPUTYPE	"CPUTYPE=mc68020"
#	define this_cputype	"cputype=mc68020"
#endif

char _TARGET_MACHINE[32] = target_MACHINE;	/* patch me */
char _target_machine[32] = target_machine;	/* patch me */
#ifdef CMUCS
char O_MACHINE[32]	= this_MACHINE;	/* patch me */
char o_machine[32]	= this_machine;	/* patch me */
#endif /* CMUCS */
char _CPUTYPE[32]	= this_CPUTYPE;	/* patch me */
char _cputype[32]	= this_cputype;	/* patch me */


char *builtin[] = {

	".SUFFIXES : .out .o .s .S .c .F .f .e .r .y .yr .ye .l .p .sh .csh .h",

	/*
	 * PRESET VARIABLES
	 */
	"MAKE=make",
	"AR=ar",
	"ARFLAGS=",
	"RANLIB=ranlib",
	"LD=ld",
	"LDFLAGS=",
	"LINT=lint",
	"LINTFLAGS=",
	"CO=co",
	"COFLAGS=-q",
	"CP=cp",
	"CPFLAGS=",
	"MV=mv",
	"MVFLAGS=",
	"RM=rm",
	"RMFLAGS=-f",
	"YACC=yacc",
	"YACCR=yacc -r",
	"YACCE=yacc -e",
	"YFLAGS=",
	"LEX=lex",
	"LFLAGS=",
	"CC=cc",
	"CFLAGS=",
	"AS=as",
	"ASFLAGS=",
	"PC=pc",
	"PFLAGS=",
	"RC=f77",
	"RFLAGS=",
	"EC=efl",
	"EFLAGS=",
	"FC=f77",
	"FFLAGS=",
	"LOADLIBES=",

	_TARGET_MACHINE,
	_target_machine,
#if	CMUCS
	O_MACHINE,
	o_machine,
#endif
	_CPUTYPE,
	_cputype,

	/*
	 * SINGLE SUFFIX RULES
	 */
	".s :",
	"\t$(AS) $(ASFLAGS) -o $@ $<",

	".S :",
	"\t$(CC) $(LDFLAGS) $(CFLAGS) $(ASFLAGS) $< $(LOADLIBES) -o $@",

	".c :",
	"\t$(CC) $(LDFLAGS) $(CFLAGS) $< $(LOADLIBES) -o $@",

	".F .f :",
	"\t$(FC) $(LDFLAGS) $(FFLAGS) $< $(LOADLIBES) -o $@",

	".e :",
	"\t$(EC) $(LDFLAGS) $(EFLAGS) $< $(LOADLIBES) -o $@",

	".r :",
	"\t$(RC) $(LDFLAGS) $(RFLAGS) $< $(LOADLIBES) -o $@",

	".p :",
	"\t$(PC) $(LDFLAGS) $(PFLAGS) $< $(LOADLIBES) -o $@",

	".y :",
	"\t$(YACC) $(YFLAGS) $<",
	"\t$(CC) $(LDFLAGS) $(CFLAGS) y.tab.c $(LOADLIBES) -ly -o $@",
	"\t$(RM) $(RMFLAGS) y.tab.c",

	".l :",
	"\t$(LEX) $(LFLAGS) $<",
	"\t$(CC) $(LDFLAGS) $(CFLAGS) lex.yy.c $(LOADLIBES) -ll -o $@",
	"\t$(RM) $(RMFLAGS) lex.yy.c",

	".sh :",
	"\t$(CP) $(CPFLAGS) $< $@",
	"\tchmod +x $@",

	".csh :",
	"\t$(CP) $(CPFLAGS) $< $@",
	"\tchmod +x $@",

	".CO :",
	"\t$(CO) $(COFLAGS) $< $@",

	".CLEANUP :",
	"\t$(RM) $(RMFLAGS) $?",

	/*
	 * DOUBLE SUFFIX RULES
	 */
	".s.o :",
	"\t$(AS) -o $@ $<",

	".S.o :",
	"\t$(CC) $(CFLAGS) $(ASFLAGS) -c $<",

	".c.o :",
	"\t$(CC) $(CFLAGS) -c $<",

	".F.o .f.o :",
	"\t$(FC) $(FFLAGS) -c $<",

	".e.o :",
	"\t$(EC) $(EFLAGS) -c $<",

	".r.o :",
	"\t$(RC) $(RFLAGS) -c $<",

	".y.o :",
	"\t$(YACC) $(YFLAGS) $<",
	"\t$(CC) $(CFLAGS) -c y.tab.c",
	"\t$(RM) $(RMFLAGS) y.tab.c",
	"\t$(MV) $(MVFLAGS) y.tab.o $@",

	".yr.o:",
	"\t$(YACCR) $(YFLAGS) $<",
	"\t$(RC) $(RFLAGS) -c y.tab.r",
	"\t$(RM) $(RMFLAGS) y.tab.r",
	"\t$(MV) $(MVFLAGS) y.tab.o $@",

	".ye.o :",
	"\t$(YACCE) $(YFLAGS) $<",
	"\t$(EC) $(EFLAGS) -c y.tab.e",
	"\t$(RM) $(RMFLAGS) y.tab.e",
	"\t$(MV) $(MVFLAGS) y.tab.o $@",

	".l.o :",
	"\t$(LEX) $(LFLAGS) $<",
	"\t$(CC) $(CFLAGS) -c lex.yy.c",
	"\t$(RM) $(RMFLAGS) lex.yy.c",
	"\t$(MV) $(MVFLAGS) lex.yy.o $@",

	".p.o :",
	"\t$(PC) $(PFLAGS) -c $<",

	".y.c :",
	"\t$(YACC) $(YFLAGS) $<",
	"\t$(MV) $(MVFLAGS) y.tab.c $@",

	".yr.r:",
	"\t$(YACCR) $(YFLAGS) $<",
	"\t$(MV) $(MVFLAGS) y.tab.r $@",

	".ye.e :",
	"\t$(YACCE) $(YFLAGS) $<",
	"\t$(MV) $(MVFLAGS) y.tab.e $@",

	".l.c :",
	"\t$(LEX) $(LFLAGS) $<",
	"\t$(MV) $(MVFLAGS) lex.yy.c $@",

	".o.out .s.out .S.out .c.out :",
	"\t$(CC) $(LDFLAGS) $(CFLAGS) $< $(LOADLIBES) -o $@",

	".F.out .f.out :",
	"\t$(FC) $(LDFLAGS) $(FFLAGS) $< $(LOADLIBES) -o $@",

	".e.out :",
	"\t$(EC) $(LDFLAGS) $(EFLAGS) $< $(LOADLIBES) -o $@",

	".r.out :",
	"\t$(RC) $(LDFLAGS) $(RFLAGS) $< $(LOADLIBES) -o $@",

	".y.out :",
	"\t$(YACC) $(YFLAGS) $<",
	"\t$(CC) $(LDFLAGS) $(CFLAGS) y.tab.c $(LOADLIBES) -ly -o $@",
	"\t$(RM) $(RMFLAGS) y.tab.c",

	".l.out :",
	"\t$(LEX) $(LFLAGS) $<",
	"\t$(CC) $(LDFLAGS) $(CFLAGS) lex.yy.c $(LOADLIBES) -ll -o $@",
	"\t$(RM) $(RMFLAGS) lex.yy.c",

	".p.out :",
	"\t$(PC) $(LDFLAGS) $(PFLAGS) $< $(LOADLIBES) -o $@",

	0
};
