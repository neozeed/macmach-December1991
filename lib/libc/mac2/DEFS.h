/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)DEFS.h	5.1 (Berkeley) 5/30/85
 */

#ifdef __STDC__

#ifdef PROF
		    .globl  mcount
#define _MCOUNT	    link a6,\#0; lea mcntr,a0; \
		    .data; .even; mcntr: .long 0; .text; jsr mcount; unlk a6
#define _MCOUNT2    link a6,\#0; lea mcntr2,a0; \
		    .data; .even; mcntr2: .long 0; .text; jsr mcount; unlk a6
#else
#define _MCOUNT
#define _MCOUNT2
#endif

#define	ENTRY(x)    .text; .even; .globl _##x; _##x: _MCOUNT
#define	ENTRY2(x)    .text; .even; .globl _##x; _##x: _MCOUNT2

#else

#ifdef PROF
		    .globl  mcount
#define _MCOUNT	    link a6,#0; lea mcntr,a0; \
		    .data; .even; mcntr: .long 0; .text; jsr mcount; unlk a6
#define _MCOUNT2    link a6,#0; lea mcntr2,a0; \
		    .data; .even; mcntr2: .long 0; .text; jsr mcount; unlk a6
#else
#define _MCOUNT
#define _MCOUNT2
#endif

#define	ENTRY(x)    .text; .even; .globl _/**/x; _/**/x: _MCOUNT
#define	ENTRY2(x)    .text; .even; .globl _/**/x; _/**/x: _MCOUNT2

#endif
