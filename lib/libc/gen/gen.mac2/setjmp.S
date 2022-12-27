#if defined(LIBC_SCCS)
	.asciz "@(#)setjmp.s MacMach 5.1"
	.even
#endif /* LIBC_SCCS and not lint */

#include "DEFS.h"

/* longjmp(a, v) causes a "return(v)" from the last call to setjmp(a)
 * by restoring d2-d7,a2-a7 (all the register variables) and adjusting
 * the stack.
 * NOTE: define NOSIGMASK to make a version that does not save/restore
 * the signal mask, named _longjmp() and _setjmp().
 * NOTE: bad things will happen if the stack frame context of the setjmp()
 * does not exist when longjmp() is done...
 */

/* the jmp_buf looks like:
struct {
  int pc;
  int sigmask;
  int d2, d3, d4, d5, d6, d7;
  int a2, a3, a4, a5, a6, a7;
};
typedef int jmp_buf[14];
*/

	.text
#ifndef NOSIGMASK
	.globl	_sigblock
#endif

/*
setjmp(buf)
jmp_buf buf;
{
}
*/

#ifdef NOSIGMASK
ENTRY(_setjmp)
#else
ENTRY(setjmp)
#endif
	link	a6,#0		/* trivial stack frame */
	movl	a6@(8),a0	/* arg1 is pointer to jmp_buf */
	movl	a6@(4),a0@	/* save return from setjmp */
#ifdef NOSIGMASK
	clrl	a0@(4)		/* sigmask -- not used */
#else
	pea	0		/* zero argument */
	jbsr	_sigblock	/* sigblock(0); returns sigmask in D0 */
	addql	#4,sp		/* remove argument */
	movl	sp@(8),a0	/* pointer to jmp_buf */
	movl	d0,a0@(4)	/* save sigmask */
#endif
	unlk	a6		/* remove stack frame before saving registers */
	moveml	#0xFCFC,a0@(8)	/* d2-d7, a2-a7 */
	clrl	d0		/* return 0 */
	rts

/*
longjmp(buf, val)
jmp_buf buf;
int val;
{
}
*/

#ifdef NOSIGMASK
ENTRY2(_longjmp)
#else
ENTRY2(longjmp)
#endif
	link	a6,#0		/* trivial stack frame */
#ifndef NOSIGMASK
	movl	a6@(8),a0	/* arg1 is pointer to jmp_buf */
	movl	a0@(4),sp@-	/* get saved mask for argument */
	jbsr	_sigsetmask	/* sigsetmask(mask) */
	addql	#4,sp		/* remove argument */
#endif
	movl	a6@(12),d0	/* arg2 is value to be returned */
	bne	skip		/* if not zero, skip */
	moveq	#1,d0		/* return 1 if tried to return 0 */
skip:	movl	a6@(8),a0	/* arg1 is pointer to jmp_buf */
	unlk	a6		/* remove stack frame before restoring registers */
	moveml	a0@(8),#0xFCFC	/* restore d2-d7, a2-a7 */
	movl	a0@,sp@		/* set return from setjmp */
	rts			/* return from setjmp */
