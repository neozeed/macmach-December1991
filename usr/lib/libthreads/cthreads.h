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
 * $Log:	cthreads.h,v $
 * Revision 2.11  91/08/03  18:20:15  jsb
 * 	Removed the infamous line 122.
 * 	[91/08/01  22:40:24  jsb]
 * 
 * Revision 2.10  91/07/31  18:35:42  dbg
 * 	Fix the standard-C conditional: it's __STDC__.
 * 
 * 	Allow for macro-redefinition of cthread_sp, spin_try_lock,
 * 	spin_unlock (from machine/cthreads.h).
 * 	[91/07/30  17:34:28  dbg]
 * 
 * Revision 2.9  91/05/14  17:56:42  mrt
 * 	Correcting copyright
 * 
 * Revision 2.8  91/02/14  14:19:52  mrt
 * 	Added new Mach copyright
 * 	[91/02/13  12:41:15  mrt]
 * 
 * Revision 2.7  90/11/05  14:37:12  rpd
 * 	Include machine/cthreads.h.  Added spin_lock_t.
 * 	[90/10/31            rwd]
 * 
 * Revision 2.6  90/10/12  13:07:24  rpd
 * 	Channge to allow for positive stack growth.
 * 	[90/10/10            rwd]
 * 
 * Revision 2.5  90/09/09  14:34:56  rpd
 * 	Remove mutex_special and debug_mutex.
 * 	[90/08/24            rwd]
 * 
 * Revision 2.4  90/08/07  14:31:14  rpd
 * 	Removed RCS keyword nonsense.
 * 
 * Revision 2.3  90/01/19  14:37:18  rwd
 * 	Add back pointer to cthread structure.
 * 	[90/01/03            rwd]
 * 	Change definition of cthread_init and change ur_cthread_self macro
 * 	to reflect movement of self pointer on stack.
 * 	[89/12/18  19:18:34  rwd]
 * 
 * Revision 2.2  89/12/08  19:53:49  rwd
 * 	Change spin_try_lock to int.
 * 	[89/11/30            rwd]
 * 	Changed mutex macros to deal with special mutexs
 * 	[89/11/26            rwd]
 * 	Make mutex_{set,clear}_special routines instead of macros.
 * 	[89/11/25            rwd]
 * 	Added mutex_special to specify a need to context switch on this
 * 	mutex.
 * 	[89/11/21            rwd]
 * 
 * 	Made mutex_lock a macro trying to grab the spin_lock first.
 * 	[89/11/13            rwd]
 * 	Removed conditionals.  Mutexes are more like conditions now.
 * 	Changed for limited kernel thread version.
 * 	[89/10/23            rwd]
 * 
 * Revision 2.1  89/08/03  17:09:40  rwd
 * Created.
 * 
 *
 * 28-Oct-88  Eric Cooper (ecc) at Carnegie Mellon University
 *	Implemented spin_lock() as test and test-and-set logic
 *	(using mutex_try_lock()) in sync.c.  Changed ((char *) 0)
 *	to 0, at Mike Jones's suggestion, and turned on ANSI-style
 *	declarations in either C++ or _STDC_.
 *
 * 29-Sep-88  Eric Cooper (ecc) at Carnegie Mellon University
 *	Changed NULL to ((char *) 0) to avoid dependency on <stdio.h>,
 *	at Alessandro Forin's suggestion.
 *
 * 08-Sep-88  Alessandro Forin (af) at Carnegie Mellon University
 *	Changed queue_t to cthread_queue_t and string_t to char *
 *	to avoid conflicts.
 *
 * 01-Apr-88  Eric Cooper (ecc) at Carnegie Mellon University
 *	Changed compound statement macros to use the
 *	do { ... } while (0) trick, so that they work
 *	in all statement contexts.
 *
 * 19-Feb-88  Eric Cooper (ecc) at Carnegie Mellon University
 *	Made spin_unlock() and mutex_unlock() into procedure calls
 *	rather than macros, so that even smart compilers can't reorder
 *	the clearing of the lock.  Suggested by Jeff Eppinger.
 *	Removed the now empty <machine>/cthreads.h.
 * 
 * 01-Dec-87  Eric Cooper (ecc) at Carnegie Mellon University
 *	Changed cthread_self() to mask the current SP to find
 *	the self pointer stored at the base of the stack.
 *
 * 22-Jul-87  Eric Cooper (ecc) at Carnegie Mellon University
 *	Fixed bugs in mutex_set_name and condition_set_name
 *	due to bad choice of macro formal parameter name.
 *
 * 21-Jul-87  Eric Cooper (ecc) at Carnegie Mellon University
 *	Moved #include <machine/cthreads.h> to avoid referring
 *	to types before they are declared (required by C++).
 *
 *  9-Jul-87  Michael Jones (mbj) at Carnegie Mellon University
 *	Added conditional type declarations for C++.
 *	Added _cthread_init_routine and _cthread_exit_routine variables
 *	for automatic initialization and finalization by crt0.
 */
/*
 * 	File: 	cthread.h
 *	Author: Eric Cooper, Carnegie Mellon University
 *	Date:	Jul, 1987
 *
 * 	Definitions for the C Threads package.
 *
 */


#ifndef	_CTHREADS_
#define	_CTHREADS_ 1

#include <machine/cthreads.h>

#if	c_plusplus || __STDC__

#ifndef	C_ARG_DECLS
#define	C_ARG_DECLS(arglist)	arglist
#endif	/* not C_ARG_DECLS */

typedef void *any_t;

#else	/* not (c_plusplus || __STDC__) */

#ifndef	C_ARG_DECLS
#define	C_ARG_DECLS(arglist)	()
#endif	/* not C_ARG_DECLS */

typedef char *any_t;

#endif	/* not (c_plusplus || __STDC__) */

#include <mach/mach.h>
#include <mach/machine/vm_param.h>

#ifndef	TRUE
#define	TRUE	1
#define	FALSE	0
#endif	/* TRUE */

/*
 * C Threads package initialization.
 */

extern int cthread_init();
extern any_t calloc C_ARG_DECLS((unsigned n, unsigned size));

/*
 * Queues.
 */
typedef struct cthread_queue {
	struct cthread_queue_item *head;
	struct cthread_queue_item *tail;
} *cthread_queue_t;

typedef struct cthread_queue_item {
	struct cthread_queue_item *next;
} *cthread_queue_item_t;

#define	NO_QUEUE_ITEM	((cthread_queue_item_t) 0)

#define	QUEUE_INITIALIZER	{ NO_QUEUE_ITEM, NO_QUEUE_ITEM }

#define	cthread_queue_alloc()	((cthread_queue_t) calloc(1, sizeof(struct cthread_queue)))
#define	cthread_queue_init(q)	((q)->head = (q)->tail = 0)
#define	cthread_queue_free(q)	free((any_t) (q))

#define	cthread_queue_enq(q, x) \
	do { \
		(x)->next = 0; \
		if ((q)->tail == 0) \
			(q)->head = (cthread_queue_item_t) (x); \
		else \
			(q)->tail->next = (cthread_queue_item_t) (x); \
		(q)->tail = (cthread_queue_item_t) (x); \
	} while (0)

#define	cthread_queue_preq(q, x) \
	do { \
		if ((q)->tail == 0) \
			(q)->tail = (cthread_queue_item_t) (x); \
		((cthread_queue_item_t) (x))->next = (q)->head; \
		(q)->head = (cthread_queue_item_t) (x); \
	} while (0)

#define	cthread_queue_head(q, t)	((t) ((q)->head))

#define	cthread_queue_deq(q, t, x) \
	if (((x) = (t) ((q)->head)) != 0 && \
	    ((q)->head = (cthread_queue_item_t) ((x)->next)) == 0) \
		(q)->tail = 0; \
	else

#define	cthread_queue_map(q, t, f) \
	do { \
		register cthread_queue_item_t x, next; \
		for (x = (cthread_queue_item_t) ((q)->head); x != 0; x = next) { \
			next = x->next; \
			(*(f))((t) x); \
		} \
	} while (0)

/*
 * Spin locks.
 */
extern void
spin_lock_solid C_ARG_DECLS((spin_lock_t *p));

#ifndef	spin_unlock
extern void
spin_unlock C_ARG_DECLS((spin_lock_t *p));
#endif

#ifndef	spin_try_lock
extern int
spin_try_lock C_ARG_DECLS((spin_lock_t *p));
#endif

#define spin_lock(p) if (!spin_try_lock(p)) spin_lock_solid(p); else

/*
 * Mutex objects.
 */
typedef struct mutex {
	spin_lock_t lock;
	char *name;
	struct cthread_queue queue;
	spin_lock_t held;
} *mutex_t;

#define	MUTEX_INITIALIZER	{ SPIN_LOCK_INITIALIZER, 0, QUEUE_INITIALIZER, SPIN_LOCK_INITIALIZER}

#define	mutex_alloc()		((mutex_t) calloc(1, sizeof(struct mutex)))
#define	mutex_init(m)		do {spin_lock_init(&(m)->lock); cthread_queue_init(&(m)->queue); spin_lock_init(&(m)->held);} while (0)
#define	mutex_set_name(m, x)	((m)->name = (x))
#define	mutex_name(m)		((m)->name != 0 ? (m)->name : "?")
#define	mutex_clear(m)		/* nop */???
#define	mutex_free(m)		free((any_t) (m))

extern void
mutex_lock_solid C_ARG_DECLS((mutex_t m));		/* blocking */

extern void
mutex_unlock_solid C_ARG_DECLS((mutex_t m));

#define mutex_try_lock(m) spin_try_lock(&(m)->held)
#define mutex_lock(m)	if (!spin_try_lock(&(m)->held)) mutex_lock_solid(m);else
#define mutex_unlock(m) if (spin_unlock(&(m)->held),cthread_queue_head(&(m)->queue, int) != 0)mutex_unlock_solid(m); else

/*
 * Condition variables.
 */
typedef struct condition {
	spin_lock_t lock;
	struct cthread_queue queue;
	char *name;
} *condition_t;

#define	CONDITION_INITIALIZER		{ SPIN_LOCK_INITIALIZER, QUEUE_INITIALIZER, 0 }

#define	condition_alloc()		((condition_t) calloc(1, sizeof(struct condition)))
#define	condition_init(c)		do { spin_lock_init(&(c)->lock); cthread_queue_init(&(c)->queue); } while (0)
#define	condition_set_name(c, x)	((c)->name = (x))
#define	condition_name(c)		((c)->name != 0 ? (c)->name : "?")
#define	condition_clear(c)		do { condition_broadcast(c); spin_lock(&(c)->lock); } while (0)
#define	condition_free(c)		do { condition_clear(c); free((any_t) (c)); } while (0)

#define	condition_signal(c)		if ((c)->queue.head) \
						cond_signal(c); \
					else

#define	condition_broadcast(c)		if ((c)->queue.head) \
						cond_broadcast(c); \
					else

extern void
cond_signal C_ARG_DECLS((condition_t c));

extern void
cond_broadcast C_ARG_DECLS((condition_t c));

extern void
condition_wait C_ARG_DECLS((condition_t c, mutex_t m));

/*
 * Threads.
 */

typedef any_t (*cthread_fn_t) C_ARG_DECLS((any_t arg));

#include <setjmp.h>

typedef struct cthread {
	struct cthread *next;
	struct mutex lock;
	struct condition done;
	int state;
	jmp_buf catch;
	cthread_fn_t func;
	any_t arg;
	any_t result;
	char *name;
	any_t data;
	struct ur_cthread *ur;
} *cthread_t;

#define	NO_CTHREAD	((cthread_t) 0)

extern cthread_t
cthread_fork C_ARG_DECLS((cthread_fn_t func, any_t arg));

extern void
cthread_detach C_ARG_DECLS((cthread_t t));

extern any_t
cthread_join C_ARG_DECLS((cthread_t t));

extern void
cthread_yield();

extern void
cthread_exit C_ARG_DECLS((any_t result));

/*
 * This structure must agree with struct cproc in cthread_internals.h
 */
typedef struct ur_cthread {
	struct ur_cthread *next;
	cthread_t incarnation;
} *ur_cthread_t;

#ifndef	cthread_sp
extern int
cthread_sp();
#endif

extern int cthread_stack_mask;

#ifdef	STACK_GROWTH_UP
#define	ur_cthread_ptr(sp) \
	(* (ur_cthread_t *) ((sp) & cthread_stack_mask))
#else	STACK_GROWTH_UP
#define	ur_cthread_ptr(sp) \
	(* (ur_cthread_t *) ( ((sp) | cthread_stack_mask) + 1 \
			      - sizeof(ur_cthread_t *)) )
#endif	STACK_GROWTH_UP

#define	ur_cthread_self()	(ur_cthread_ptr(cthread_sp()))

#define	cthread_assoc(id, t)	((((ur_cthread_t) (id))->incarnation = (t)), \
				((t) ? ((t)->ur = (ur_cthread_t)(id)) : 0))
#define	cthread_self()		(ur_cthread_self()->incarnation)

extern void
cthread_set_name C_ARG_DECLS((cthread_t t, char *name));

extern char *
cthread_name C_ARG_DECLS((cthread_t t));

extern int
cthread_count();

extern void
cthread_set_limit C_ARG_DECLS((int n));

extern int
cthread_limit();

#define	cthread_set_data(t, x)	((t)->data = (x))
#define	cthread_data(t)		((t)->data)

/*
 * Debugging support.
 */
#ifdef	DEBUG

#ifndef	ASSERT
/*
 * Assertion macro, similar to <assert.h>
 */
#include <stdio.h>
#define	ASSERT(p)	if (!(p)) { \
				fprintf(stderr, \
					"File %s, line %d: assertion p failed.\n", \
					__FILE__, __LINE__); \
				abort(); \
			} else
#endif	ASSERT

#define	SHOULDNT_HAPPEN	0

extern int cthread_debug;

#else	DEBUG

#ifndef	ASSERT
#define	ASSERT(p)
#endif	ASSERT

#endif	DEBUG

#endif	_CTHREADS_
