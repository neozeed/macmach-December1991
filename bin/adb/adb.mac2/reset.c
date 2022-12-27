/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)reset.c MacMach 5.1 (from Berkeley 5.4)";
#endif /* LIBC_SCCS and not lint */

/*
 * C library -- reset, setexit
 *
 *	reset(x)
 * will generate a "return" from
 * the last call to
 *	setexit()
 * by restoring r6 - r12, ap, fp
 * and doing a return.
 * The returned value is x; on the original
 * call the returned value is 0.
 *
 * useful for going back to the main loop
 * after a horrible error in a lowlevel
 * routine.
 */
#include <setjmp.h>

static jmp_buf there;

setexit()
{
  setjmp(there);
}

reset()
{
  longjmp(there, 0);
}
