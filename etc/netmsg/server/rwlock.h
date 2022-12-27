/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	rwlock.h,v $
 * Revision 2.1  90/10/27  20:46:04  dpj
 * Moving under MACH3 source control
 * 
 * Revision 1.9  89/05/02  11:17:02  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * 27-Jul-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	VAX_FAST_LOCK: use rfr's inline locking package.
 *	Available only on the Vax.
 *
 *  2-Jun-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Conditionally treat rw_locks as exclusive locks - all rwlock
 *	calls are macros which directly call the mutex functions.
 *
 * 27-Apr-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Made the mutex and condition inline in the lock structure.
 *	Added extern definition of lk_init and lk_clear.
 *
 * 25-Jan-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Changed block_t to rw_block_t and perm_t to rw_perm_t.
 *
 */
/*
 * rwlock.h
 *
 *
 * $Header: /afs/cs.cmu.edu/project/mach-2/rcs/pkg/netmsg/server/rwlock.h,v 2.1 90/10/27 20:46:04 dpj Exp $
 *
 */

/*
 * Definitions of structures for read/write locks
 */


#ifndef	_RWLOCK_
#define	_RWLOCK_

#ifdef	vax
#define	VAX_FAST_LOCK	1
#else	vax
#define	VAX_FAST_LOCK	0
#endif	vax

#include <cthreads.h>

#if	VAX_FAST_LOCK
#include "vax_fast_lock.h"
#endif	VAX_FAST_LOCK

typedef enum {
	PERM_READ,
	PERM_READWRITE,
} rw_perm_t;

typedef enum {
	NOBLOCK = 0,
	BLOCK = 1,
} rw_block_t;

typedef struct lock {
	rw_perm_t		lk_permission;
	struct mutex		lk_mutex;
	struct condition	lk_condition;
	short			lk_users;
} *lock_t;

#define	X_LOCKS	1

#if	X_LOCKS

#define lk_init(lock) mutex_init(&(lock)->lk_mutex)

#define	lk_lock(lock, perm, block) mutex_lock(&(lock)->lk_mutex)

#define lk_unlock(lock) mutex_unlock(&(lock)->lk_mutex)

#define lk_clear(lock) mutex_clear(&(lock)->lk_mutex)

#else	X_LOCKS

extern lock_t	lk_alloc();

extern void	lk_init();
/*
lock_t		lock;
*/

extern void	lk_free();
/*
lock_t		lock;
*/

extern void	lk_clear();
/*
lock_t		lock;
*/


extern int	lk_lock();
/*
lock_t		lock;
rw_perm_t	perm;
rw_block_t	block;
*/

extern void	lk_unlock();
/*
lock_t	lock;
*/

#endif	X_LOCKS

#endif	_RWLOCK_
