/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	emul_stack.h,v $
 * Revision 2.2  90/06/02  15:20:39  rpd
 * 	Created for new IPC.
 * 	[90/03/26  20:32:04  rpd]
 * 
 */

#ifndef	_EMUL_STACK_H_
#define	_EMUL_STACK_H_

#ifdef mac2 /* fault jmp_buf */
#include <setjmp.h>
#endif

/*
 * Top of emulator stack holds link and reply port.
 */
struct emul_stack_top {
	struct emul_stack_top	*link;
#ifdef mac2 /* fault jmp_buf */
	jmp_buf			fault;
#endif
	mach_port_t		reply_port;
};
typedef	struct emul_stack_top	*emul_stack_t;

extern emul_stack_t emul_stack_init();
extern emul_stack_t emul_stack_alloc();

#ifdef mac2 /* fault jmp_buf */
extern int *emul_stack_fault();
#endif

#endif	_EMUL_STACK_H_
