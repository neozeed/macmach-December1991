/* 
 * Mach Operating System
 * Copyright (c) 1991,1990,1989,1988,1987 Carnegie Mellon University
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 * 
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS "AS IS"
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
 * any improvements or extensions that they make and grant Carnegie Mellon
 * the rights to redistribute these changes.
 */
/*
 * HISTORY
 * $Log:	xpr.c,v $
 * Revision 2.8  91/08/28  11:14:56  jsb
 * 	Fixed xprbootstrap to zero the allocate memory.
 * 	[91/08/18            rpd]
 * 
 * Revision 2.7  91/05/18  14:34:37  rpd
 * 	Added xprenable and other minor changes so that the xpr buffer
 * 	may be examined after a spontaneous reboot.
 * 	[91/05/03            rpd]
 * 	Fixed the initialization check in xpr.
 * 	Fixed xpr_dump.
 * 	[91/04/02            rpd]
 * 
 * Revision 2.6  91/05/14  16:50:09  mrt
 * 	Correcting copyright
 * 
 * Revision 2.5  91/03/16  14:53:24  rpd
 * 	Updated for new kmem_alloc interface.
 * 	[91/03/03            rpd]
 * 
 * Revision 2.4  91/02/05  17:31:13  mrt
 * 	Changed to new Mach copyright
 * 	[91/02/01  16:21:17  mrt]
 * 
 * Revision 2.3  90/09/09  14:33:04  rpd
 * 	Use decl_simple_lock_data.
 * 	[90/08/30            rpd]
 * 
 * Revision 2.2  89/11/29  14:09:21  af
 * 	Added xpr_dump() to print on console the content of the buffer,
 * 	only valid for KDB usage.
 * 	[89/11/12            af]
 * 
 * 	MACH_KERNEL: include sys/cpu_number.h instead of machine/cpu.h.
 * 	Clean up comments.
 * 	[88/12/19            dbg]
 * 
 * Revision 2.1  89/08/03  15:49:11  rwd
 * Created.
 * 
 * Revision 2.2  88/12/19  02:48:30  mwyoung
 * 	Fix include file references.
 * 	[88/11/22  02:17:01  mwyoung]
 * 	
 * 	Separate initialization into two phases.
 * 	[88/11/22  01:13:11  mwyoung]
 * 
 *  6-Jan-88  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Eliminate use of arg6 in order to allow a more shapely event structure.
 *
 * 30-Dec-87  David Golub (dbg) at Carnegie-Mellon University
 *	Delinted.
 *
 *  7-Dec-87  Richard Sanzi (sanzi) at Carnegie-Mellon University
 *	Added xpr_save() routine.
 *
 */ 
#include <mach_kdb.h>
/*
 * xpr silent tracing circular buffer.
 */
#include <kern/xpr.h>
#include <kern/lock.h>
#include <kern/cpu_number.h>
#include <machine/machparam.h>
#include <vm/vm_kern.h>

/*
 *	After a spontaneous reboot, it is desirable to look
 *	at the old xpr buffer.  Assuming xprbootstrap allocates
 *	the buffer in the same place in physical memory and
 *	the reboot doesn't clear memory, this should work.
 *	xprptr will be reset, but the saved value should be OK.
 *	Just set xprenable false so the buffer isn't overwritten.
 */

decl_simple_lock_data(,	xprlock)

boolean_t xprenable = FALSE;	/* Enable xpr tracing */
int nxprbufs = 0;	/* Number of contiguous xprbufs allocated */
int xprflags = 0;	/* Bit mask of xpr flags enabled */
struct xprbuf *xprbase;	/* Pointer to circular buffer nxprbufs*sizeof(xprbuf)*/
struct xprbuf *xprptr;	/* Currently allocated xprbuf */
struct xprbuf *xprlast;	/* Pointer to end of circular buffer */

/*VARARGS1*/
xpr(msg, arg1, arg2, arg3, arg4, arg5)
	char *msg;
{
	register s;
	register struct xprbuf *x;

	/* If we aren't initialized, ignore trace request */
	if (!xprenable || (xprbase == 0))
		return;
	/* Guard against all interrupts and allocate next buffer. */
	s = splhigh();
	simple_lock(&xprlock);
	x = xprptr++;
	if (xprptr >= xprlast) {
		/* wrap around */
		xprptr = xprbase;
	}
	/* Save xprptr in allocated memory. */
	*(struct xprbuf **)xprlast = xprptr;
	simple_unlock(&xprlock);
	splx(s);
	x->msg = msg;
	x->arg1 = arg1;
	x->arg2 = arg2;
	x->arg3 = arg3;
	x->arg4 = arg4;
	x->arg5 = arg5;
	x->timestamp = XPR_TIMESTAMP;
	x->cpuinfo = cpu_number();
}

xprbootstrap()
{
	vm_size_t size;
	kern_return_t kr;

	simple_lock_init(&xprlock);
	if (nxprbufs == 0)
		return;	/* assume XPR support not desired */

	/* leave room at the end for a saved copy of xprptr */
	size = nxprbufs * sizeof(struct xprbuf) + sizeof xprptr;
	kr = kmem_alloc_wired(kernel_map, (vm_offset_t *) &xprbase, size);
	if (kr != KERN_SUCCESS)
		panic("xprbootstrap");

	/* xpr_dump checks the msg field, but we just clear everything */
	bzero((char *) xprbase, size);

	xprlast = &xprbase[nxprbufs];
	xprptr = xprbase;
}

int		xprinitial = 0;

xprinit()
{
	xprflags |= xprinitial;
}

/*
 *	Save 'number' of xpr buffers to the area provided by buffer.
 */

xpr_save(buffer,number)
struct xprbuf *buffer;
int number;
{
    int i,s;
    struct xprbuf *x;
    s = splhigh();
    simple_lock(&xprlock);
    if (number > nxprbufs) number = nxprbufs;
    x = *(struct xprbuf **)xprlast;
    
    for (i = number - 1; i >= 0 ; i-- ) {
	if (--x < xprbase) {
	    /* wrap around */
	    x = xprlast - 1;
	}
	bcopy((char *)x, (char *)&buffer[i], sizeof(struct xprbuf));
    }
    simple_unlock(&xprlock);
    splx(s);
}


#if	MACH_KDB
#include <machine/setjmp.h>

extern void db_printf();
extern jmp_buf_t *db_recover;

/*
 *	Print current content of xpr buffers (KDB's sake)
 *	Use stack order to make it understandable
 */
xpr_dump()
{
	jmp_buf_t db_jmpbuf;
	jmp_buf_t *prev;
	register struct xprbuf *x;
	int i, s;

	if (!nxprbufs)
		return;

	s = splhigh();
	simple_lock(&xprlock);

	prev = db_recover;
	if (_setjmp(db_recover = &db_jmpbuf) == 0)
	    for (x = *(struct xprbuf **)xprlast, i = 0; i < nxprbufs; i++) {
		if (--x < xprbase)
			x = xprlast - 1;

		if (x->msg == 0)
			break;

		db_printf("<%d:%d:%x> ", x - xprbase,
			  x->cpuinfo, x->timestamp);
		db_printf(x->msg, x->arg1,x->arg2,x->arg3,x->arg4,x->arg5);
	    }
	db_recover = prev;

	simple_unlock(&xprlock);
	(void) splx(s);

}
#endif	MACH_KDB
