/* 
 * Mach Operating System
 * Copyright (c) 1991,1990,1989 Carnegie Mellon University
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
 * $Log:	sbrk.c,v $
 * Revision 2.2  91/03/26  17:46:40  mrt
 * 	First checkin
 * 
 */
/*
 *	File:	sbrk.c
 *	Author: Avadis Tevanian, Carnegie Mellon University
 *	Date:	June 1986
 *
 *	Unix compatibility for sbrk system call.
 */

#define  EXPORT_BOOLEAN
#include <mach.h>		/* for vm_allocate, vm_offset_t */
#include <stdio.h>		/* for stderr */
#include <sys/types.h>		/* for caddr_t */
#include <mach_init.h>		/* for vm_page_size */

#if	(defined(vax) || defined(ibmrt) || defined(ns32000) || defined(sun) || defined(i386)) || defined(mac2)
#if	multimax
DEF_FUNC()
{
#endif	multimax
asm(".data");
asm(".globl	curbrk");
asm(".globl	minbrk");
asm(".globl	_curbrk");
asm(".globl	_minbrk");
asm(".globl	_end");
#if	multimax
asm("_minbrk:");
asm("minbrk:	.double	_end");
asm("_curbrk:");
asm("curbrk:	.double	_end");
asm(".text");
}
#else	multimax
asm("_minbrk:");
asm("minbrk:	.long	_end");
asm("_curbrk:");
asm("curbrk:	.long	_end");
asm(".text");
#endif	multimax
#else	(defined(vax) || defined(ibmrt) || defined(ns32000) || defined(sun) || defined (i386))

/* Will not find get "assembler" forms of cubrk, minbrk. */

#ifdef	mips
extern char end;
#define curbrk _curbrk
#define minbrk _minbrk
caddr_t curbrk = &end;
caddr_t minbrk = &end;
#endif	mips

#endif	(defined(vax) || defined(ibmrt) || defined(ns32000) || defined(sun) || defined(i386))

#ifdef lint
   /* lint doesn't see asm stuff */
caddr_t	curbrk;
caddr_t	minbrk;
#else lint
extern caddr_t curbrk;
extern caddr_t minbrk;
#endif lint

#define	roundup(a,b)	((((a) + (b) - 1) / (b)) * (b))

static int sbrk_needs_init = FALSE;

caddr_t sbrk(size)
	int	size;
{
	vm_offset_t	addr;
	kern_return_t	ret;
	caddr_t		ocurbrk;

	if (sbrk_needs_init) {
		sbrk_needs_init = FALSE;
		/*
		 *	Get "curbrk"
		 */

	}
	
	if (size <= 0)
		return(curbrk);
	addr = (vm_offset_t) roundup((int)curbrk,vm_page_size);
	ocurbrk = curbrk;
	if (((int)curbrk+size) > addr)
	{	ret = vm_allocate(mach_task_self(), &addr, 
			    (vm_size_t) size -((int)addr-(int)curbrk), FALSE);
		if (ret == KERN_NO_SPACE) {
			ret = vm_allocate(mach_task_self(), &addr, (vm_size_t) size, TRUE);
			ocurbrk = (caddr_t)addr;
		}
		if (ret != KERN_SUCCESS) 
			return((caddr_t) -1);
	}

	curbrk = (caddr_t)ocurbrk + size;
	return(ocurbrk);

}

void brk(x)
	caddr_t x;
{
	fprintf(stderr, "brk: not implemented\n");
}
