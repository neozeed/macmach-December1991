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
 * $Log:	lock.h,v $
 * Revision 2.7  91/05/18  14:32:17  rpd
 * 	Added check_simple_locks.
 * 	[91/03/31            rpd]
 * 
 * Revision 2.6  91/05/14  16:43:51  mrt
 * 	Correcting copyright
 * 
 * Revision 2.5  91/05/08  12:47:17  dbg
 * 	When actually using the locks (on multiprocessors), import the
 * 	machine-dependent simple_lock routines from machine/lock.h.
 * 	[91/04/26  14:42:23  dbg]
 * 
 * Revision 2.4  91/02/05  17:27:37  mrt
 * 	Changed to new Mach copyright
 * 	[91/02/01  16:14:39  mrt]
 * 
 * Revision 2.3  90/11/05  14:31:18  rpd
 * 	Added simple_lock_taken.
 * 	[90/11/04            rpd]
 * 
 * Revision 2.2  90/01/11  11:43:26  dbg
 * 	Upgraded to match mainline:
 * 	 	Added decl_simple_lock_data, simple_lock_addr macros.
 * 	 	Rearranged complex lock structure to use decl_simple_lock_data
 * 	 	for the interlock field and put it last (except on ns32000).
 * 	 	[89/01/15  15:16:47  rpd]
 * 
 * 	Made all machines use the compact field layout.
 * 
 * Revision 2.1  89/08/03  15:49:42  rwd
 * Created.
 * 
 * Revision 2.2  88/07/20  16:49:35  rpd
 * Allow for sanity-checking of simple locking on uniprocessors,
 * controlled by new option MACH_LDEBUG.  Define composite
 * MACH_SLOCKS, which is true iff simple locking calls expand
 * to code.  It can be used to #if-out declarations, etc, that
 * are only used when simple locking calls are real.
 * 
 *  3-Nov-87  David Black (dlb) at Carnegie-Mellon University
 *	Use optimized lock structure for multimax also.
 *
 * 27-Oct-87  Robert Baron (rvb) at Carnegie-Mellon University
 *	Use optimized lock "structure" for balance now that locks are
 *	done inline.
 *
 * 26-Jan-87  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Invert logic of no_sleep to can_sleep.
 *
 * 29-Dec-86  David Golub (dbg) at Carnegie-Mellon University
 *	Removed BUSYP, BUSYV, adawi, mpinc, mpdec.  Defined the
 *	interlock field of the lock structure to be a simple-lock.
 *
 *  9-Nov-86  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Added "unsigned" to fields in vax case, for lint.
 *
 * 21-Oct-86  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Added fields for sleep/recursive locks.
 *
 *  7-Oct-86  David L. Black (dlb) at Carnegie-Mellon University
 *	Merged Multimax changes.
 *
 * 26-Sep-86  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Removed reference to "caddr_t" from BUSYV/P.  I really
 *	wish we could get rid of these things entirely.
 *
 * 24-Sep-86  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Changed to directly import boolean declaration.
 *
 *  1-Aug-86  David Golub (dbg) at Carnegie-Mellon University
 *	Added simple_lock_try, sleep locks, recursive locking.
 *
 * 11-Jun-86  Bill Bolosky (bolosky) at Carnegie-Mellon University
 *	Removed ';' from definitions of locking macros (defined only
 *	when NCPU < 2). so as to make things compile.
 *
 * 28-Feb-86  Bill Bolosky (bolosky) at Carnegie-Mellon University
 *	Defined adawi to be add when not on a vax.
 *
 * 07-Nov-85  Michael Wayne Young (mwyoung) at Carnegie-Mellon University
 *	Overhauled from previous version.
 */
/*
 *	File:	kern/lock.h
 *	Author:	Avadis Tevanian, Jr., Michael Wayne Young
 *	Date:	1985
 *
 *	Locking primitives definitions
 */

#ifndef	_KERN_LOCK_H_
#define	_KERN_LOCK_H_

#include <cpus.h>
#include <mach_ldebug.h>

#include <mach/boolean.h>

#define MACH_SLOCKS	((NCPUS > 1) || MACH_LDEBUG)

/*
 *	A simple spin lock.
 */

struct slock {
	int		lock_data;	/* in general 1 bit is sufficient */
};

typedef struct slock	simple_lock_data_t;
typedef struct slock	*simple_lock_t;

#if	MACH_SLOCKS
/*
 *	Use the locks.
 */

#if	NCPUS > 1
/*
 *	Import the definitions from machine-dependent code.
 */

#include <machine/lock.h>

#else	NCPUS > 1
/*
 *	Use our single-CPU locking test routines.
 */

extern void		simple_lock_init();
extern void		simple_lock();
extern void		simple_unlock();
extern boolean_t	simple_lock_try();

#endif	NCPUS > 1

#define	decl_simple_lock_data(class,name) \
class	simple_lock_data_t	name;

#define	simple_lock_addr(lock)		(&(lock))

#if	NCPUS == 1
#define simple_lock_taken(lock)		((lock)->lock_data)
extern void		check_simple_locks();
#else	NCPUS == 1
#define	simple_lock_taken(lock)		(1)	/* always succeeds */
#define check_simple_locks()
#endif	NCPUS == 1

#else	MACH_SLOCKS
/*
 *	No multiprocessor locking is necessary.
 */
#define simple_lock_init(l)
#define simple_lock(l)
#define simple_unlock(l)
#define simple_lock_try(l)	(1)	/* always succeeds */
#define simple_lock_taken(l)	(1)	/* always succeeds */
#define check_simple_locks()

/*
 * Do not allocate storage for locks if not needed.
 */
#define	decl_simple_lock_data(class,name)
#define	simple_lock_addr(lock)		((simple_lock_t)0)

#endif	MACH_SLOCKS

/*
 *	The general lock structure.  Provides for multiple readers,
 *	upgrading from read to write, and sleeping until the lock
 *	can be gained.
 *
 *	On some architectures, assembly language code in the 'inline'
 *	program fiddles the lock structures.  It must be changed in
 *	concert with the structure layout.
 *
 *	Only the "interlock" field is used for hardware exclusion;
 *	other fields are modified with normal instructions after
 *	acquiring the interlock bit.
 */
struct lock {
	unsigned int	read_count:16,	/* Number of accepted readers */
			want_upgrade:1,	/* Read-to-write upgrade waiting */
			want_write:1,	/* Writer is waiting, or
					   locked for write */
			waiting:1,	/* Someone is sleeping on lock */
			can_sleep:1,	/* Can attempts to lock go to sleep? */
			:0;
	char		*thread;	/* Thread that has lock, if
					   recursive locking allowed
					   (Not thread_t to avoid
					   recursive definitions) */
	int		recursion_depth;/* Depth of recursion */

	decl_simple_lock_data(,interlock)
					/* Hardware interlock field.
					   Last in the structure so that
					   field offsets are the same whether
					   or not it is present. */
};

typedef struct lock	lock_data_t;
typedef struct lock	*lock_t;

/* Sleep locks must work even if no multiprocessing */

extern void		lock_init();
extern void		lock_sleepable();
extern void		lock_write();
extern void		lock_read();
extern void		lock_done();
extern boolean_t	lock_read_to_write();
extern void		lock_write_to_read();
extern boolean_t	lock_try_write();
extern boolean_t	lock_try_read();
extern boolean_t	lock_try_read_to_write();

#define	lock_read_done(l)	lock_done(l)
#define	lock_write_done(l)	lock_done(l)

extern void		lock_set_recursive();
extern void		lock_clear_recursive();

#endif	_KERN_LOCK_H_
