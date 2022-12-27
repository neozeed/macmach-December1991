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
 *      Created this version for MacMach.
 *
 **********************************************************************
 */

#if defined(SYSLIBC_SCCS)
	.asciz "@(#)sigvec.s MacMach 5.1 (from Berkeley 5.5) *sun hack*"
	.even
#endif /* SYSLIBC_SCCS and not lint */

#include "SYS.h"

/*
#include <sys/syscall.h>
#include <signal.h>
#include <errno.h>

int (*_sigfunc[NSIG])();
extern	int _sigtramp();
extern	int errno;

sigvec(sig, vec, ovec)
int sig;
register struct sigvec *vec, *ovec;
{
  struct sigvec avec;
  int (*osig)();

  if (sig <= 0 || sig >= NSIG) {
    errno = EINVAL;
    return (-1);
  }
  osig = _sigfunc[sig];
  if (vec) {
    _sigfunc[sig] = vec->sv_handler;
    avec = *vec;
    vec = &avec;
    if (vec->sv_handler != SIG_DFL && vec->sv_handler != SIG_IGN)
      vec->sv_handler = _sigtramp;
  }
  if (syscall(SYS_sigvec, sig, vec, ovec) < 0) {
    _sigfunc[sig] = osig;
    return (-1);
  }
  if (ovec && osig) ovec->sv_handler = osig;
  return (0);
}
*/

/* from gnu cc */
.text
	.even
.globl _sigvec
_sigvec:
	link a6,#-16
	moveml #0x30,sp@-
	movel a6@(12),a3
	movel a6@(16),a2
	tstl a6@(8)
	jle L3
	moveq #31,d1
	cmpl a6@(8),d1
	jlt L3
	jra L2
L3:
	moveq #22,d1
	movel d1,_errno
	moveq #-1,d0
	jra L1
L2:
	movel a6@(8),d0
	asll #2,d0
	lea __sigfunc,a0
	movel a0@(d0:l),a6@(-16)
	tstl a3
	jeq L4
	movel a6@(8),d0
	asll #2,d0
	lea __sigfunc,a0
	movel a3@,a0@(d0:l)
	movel a3,a0
	lea a6@(-12),a1
	movel a0@,a1@
	addqw #4,a1
	addqw #4,a0
	movel a0@,a1@
	addqw #4,a1
	addqw #4,a0
	movel a0@,a1@
	addqw #4,a1
	addqw #4,a0
	lea a6@(-12),a3
	tstl a3@
	jeq L5
	moveq #1,d1
	cmpl a3@,d1
	jeq L5
	movel #__sigtramp,a3@
L5:
L4:
	movel a2,sp@-
	movel a3,sp@-
	movel a6@(8),sp@-
	pea 108:w
	jbsr _syscall
	movel d0,d0
	addw #16,sp
	tstl d0
	jge L6
	movel a6@(8),d0
	asll #2,d0
	lea __sigfunc,a0
	movel a6@(-16),a0@(d0:l)
	moveq #-1,d0
	jra L1
L6:
	tstl a2
	jeq L7
	tstl a6@(-16)
	jeq L7
	movel a6@(-16),a2@
L7:
	clrl d0
	jra L1
L1:
	moveml a6@(-24),#0xc00
	unlk a6
	rts
.comm __sigfunc,128

#define SYS_osigcleanup 139
__sigtramp:
	moveml	#0xC0C0,sp@-		/* save C scratch regs */
	subl	#256-4,sp		/* Allocate block on stack. */
	movel	sp@(256-4),sp@-		/* Save proper return address. */
	lea	sp@(4),a0		/* a0 gets address of block. */
	movel	a0@(256+16),d0		/* d0 gets signal number.	 */
	movel	d0,a0@+			/* Save signal number. */
	cmpw	#8,d0	
	jeq	8f			/* If SIGFPE, don't save/restore. */
	fmovem	fp0/fp1,a0@		/* save 68881 registers */
	fmovem	fpc/fps/fpi,a0@(24)	/* save 68881 registers */
8:	movl	sp@(16+256),d0		/* get signal number */
	lsll	#2,d0			/* scale for index */
	movl	#__sigfunc,a0		/* get array of func ptrs */
	movl	a0@(0,d0:l),a0		/* get func */
	movl	sp@(0+24+256),sp@-	/* push scp address */
	movl	sp@(4+20+256),sp@-	/* push code */
	movl	sp@(8+16+256),sp@-	/* push signal number */
	jsr	a0@			/* call handler */
	addl	#12,sp			/* pop args */
	lea	sp@(4),a0		/* a0 gets address of block. */
	movel	a0@+,d0
	cmpw	#8,d0
	beqs	9f			/* Branch if no restore required. */
	fmovem	a0@+,fp0/fp1		/* restore 68881 registers */
	fmovem	a0@ ,fpc/fps/fpi	/* restore 68881 registers */
9:	movel	sp@,sp@(256)		/* Move proper return address. */
	addl	#256,sp			/* Deallocate block on stack. */
	moveml	sp@+,#0x0303		/* restore regs */
	addl	#8,sp			/* pop signo and code */
	pea	SYS_osigcleanup
	trap	#0
	/*NOTREACHED */
