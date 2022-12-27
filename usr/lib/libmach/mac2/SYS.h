/*
 * Copyright (c) 1983 Regents of the University of California.
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
 **********************************************************************
 * HISTORY
 *
 * 21-Nov-88  Zon Williamson (zon) at Carnegie-Mellon University
 *      Created this version for the mac2.
 *
 **********************************************************************
 *
 *	@(#)SYS.h	5.4 (Berkeley) 6/27/88
 */

#include <sys/syscall.h>

/* macros to profile assembly functions */

#ifdef	__STDC__
#ifdef	PROF
		    .globl  mcount
#define _MCOUNT	    link a6,\#0; lea mcntr,a0; \
		    .data; .even; mcntr: .long 0; \
		    .text; jsr mcount; unlk a6
#define _MCOUNT2    link a6,\#0; lea mcntr2,a0; \
		    .data; .even; mcntr2: .long 0; \
		    .text; jsr mcount; unlk a6
#else	/* PROF */
#define _MCOUNT
#define _MCOUNT2
#endif	/* PROF */
#define	ENTRY(x)    .globl _##x; _##x: _MCOUNT
#define	ENTRY2(x)    .globl _##x; _##x: _MCOUNT2
#else	/* __STDC__ */
#ifdef PROF
		    .globl  mcount
#define _MCOUNT	    link a6,#0; lea mcntr,a0; \
		    .data; .even; mcntr: .long 0; \
		    .text; jsr mcount; unlk a6
#define _MCOUNT2    link a6,#0; lea mcntr2,a0; \
		    .data; .even; mcntr2: .long 0; \
		    .text; jsr mcount; unlk a6
#else	/* PROF */
#define _MCOUNT
#define _MCOUNT2
#endif	/* PROF */
#define	ENTRY(x)    .globl _/**/x; _/**/x: _MCOUNT
#define	ENTRY2(x)   .globl _/**/x; _/**/x: _MCOUNT2
#endif	/* __STDC__ */

/* macros to make system calls with error returns */
#ifdef	__STDC__
#define	SYSCALL(x)  .globl cerror; err: jmp cerror; \
		    ENTRY(x); pea SYS_##x; trap \#0; jcs err
#define	PSEUDO(x,y) ENTRY(x); pea SYS_##y; trap \#0
#else	/* __STDC__ */
#define	SYSCALL(x)  .globl cerror; err: jmp cerror; \
		    ENTRY(x); pea SYS_/**/x; trap #0; jcs err
#define	PSEUDO(x,y) ENTRY(x); pea SYS_/**/y; trap #0
#endif	/* __STDC__ */

