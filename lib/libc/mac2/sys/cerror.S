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
 *      Saves error number in _errno then returns -1 OK.
 *
 **********************************************************************
 */

#if defined(SYSLIBC_SCCS)
	.asciz "@(#)cerror.s MacMach 5.1 (from Berkeley 5.5)"
	.even
#endif /* SYSLIBC_SCCS and not lint */

#include "SYS.h"

	.data
	.globl	_errno
_errno:	.long 0
	.text
	.globl cerror
cerror: /* cerror(errno) */
	movl	d0,_errno
	moveq	#-1,d0
	rts
