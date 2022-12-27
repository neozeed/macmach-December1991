#if defined(LIBC_SCCS)
	.asciz "@(#)_setjmp.s MacMach 5.1"
	.even
#endif /* LIBC_SCCS and not lint */

/* just like setjmp() but doesn't save/restore signal mask */

#undef LIBC_SCCS
#define NOSIGMASK

#include "setjmp.S"
