/* 
 * Mach Operating System
 * Copyright (c) 1988 Carnegie-Mellon University
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 *	File:	queue.h
 *
 *	Type definitions for generic queues.
 */
/*
 * HISTORY
 * $Log:	queue.h,v $
 * Revision 2.2  90/06/02  15:25:42  rpd
 * 	Cleaned up conditionals; removed MACH, CMU, CMUCS, MACH_NO_KERNEL.
 * 	[90/04/28            rpd]
 * 
 * 	Out-of-kernel version - eliminate mpqueue.
 * 	[89/01/05            dbg]
 * 
 * Revision 2.1  89/08/04  14:46:28  rwd
 * Created.
 * 
 * Revision 2.4  88/12/19  02:51:30  mwyoung
 * 	Eliminate round_queue.
 * 	[88/11/22  01:22:22  mwyoung]
 * 	
 * 	Add queue_remove_last.
 * 	[88/11/18            mwyoung]
 * 
 * Revision 2.3  88/08/24  02:40:43  mwyoung
 * 	Adjusted include file references.
 * 	[88/08/17  02:20:58  mwyoung]
 * 
 *
 * 17-Jan-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Added queue_enter_first, queue_last and queue_prev for use by
 *	the TCP netport code.
 *
 * 17-Mar-87  David Golub (dbg) at Carnegie-Mellon University
 *	Made queue package return queue_entry_t instead of vm_offset_t.
 *
 * 27-Feb-87  David Golub (dbg) at Carnegie-Mellon University
 *	Made remqueue a real routine on all machines.  Defined old
 *	Queue routines as macros that invoke new queue routines.
 *
 * 25-Feb-87  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Fixed non-KERNEL compilation.
 *
 * 24-Feb-87  David L. Black (dlb) at Carnegie-Mellon University
 *	MULTIMAX: remqueue is now a real routine, removed define.
 *	This is done to mask a compiler bug.
 *
 * 12-Jun-85  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Created.
 */

#ifndef	_QUEUE_
#define	_QUEUE_

/*
 *	Queue of abstract objects.  Queue is maintained
 *	within that object.
 *
 *	Supports fast removal from within the queue.
 *
 *	How to declare a queue of elements of type "foo_t":
 *		In the "*foo_t" type, you must have a field of
 *		type "queue_chain_t" to hold together this queue.
 *		There may be more than one chain through a
 *		"foo_t", for use by different queues.
 *
 *		Declare the queue as a "queue_t" type.
 *
 *		Elements of the queue (of type "foo_t", that is)
 *		are referred to by reference, and cast to type
 *		"queue_entry_t" within this module.
 */

/*
 *	A generic doubly-linked list (queue).
 */

struct queue_entry {
	struct queue_entry	*next;		/* next element */
	struct queue_entry	*prev;		/* previous element */
};

typedef struct queue_entry	*queue_t;
typedef	struct queue_entry	queue_head_t;
typedef	struct queue_entry	queue_chain_t;
typedef	struct queue_entry	*queue_entry_t;

/*
 *	enqueue puts "elt" on the "queue".
 *	dequeue returns the first element in the "queue".
 *	remqueue removes the specified "elt" from the specified "queue".
 */

#define enqueue(queue,elt)	enqueue_tail(queue, elt)
#define	dequeue(queue)		dequeue_head(queue)

void		enqueue_head();
void		enqueue_tail();
queue_entry_t	dequeue_head();
queue_entry_t	dequeue_tail();
void		remqueue();

/*
 *	Macro:		queue_init
 *	Function:
 *		Initialize the given queue.
 *	Header:
 *		void queue_init(q)
 *			queue_t		q;	/* MODIFIED *\
 */
#define	queue_init(q)	((q)->next = (q)->prev = q)

/*
 *	Macro:		queue_first
 *	Function:
 *		Returns the first entry in the queue,
 *	Header:
 *		queue_entry_t queue_first(q)
 *			queue_t	q;		/* IN *\
 */
#define	queue_first(q)	((q)->next)

/*
 *	Macro:		queue_next
 *	Header:
 *		queue_entry_t queue_next(qc)
 *			queue_t qc;
 */
#define	queue_next(qc)	((qc)->next)

/*
 *	Macro:		queue_end
 *	Header:
 *		boolean_t queue_end(q, qe)
 *			queue_t q;
 *			queue_entry_t qe;
 */
#define	queue_end(q, qe)	((q) == (qe))

#define	queue_empty(q)		queue_end((q), queue_first(q))

/*
 *	Macro:		queue_enter
 *	Header:
 *		void queue_enter(q, elt, type, field)
 *			queue_t q;
 *			<type> elt;
 *			<type> is what's in our queue
 *			<field> is the chain field in (*<type>)
 */
#define queue_enter(head, elt, type, field)			\
{ 								\
	if (queue_empty((head))) {				\
		(head)->next = (queue_entry_t) elt;		\
		(head)->prev = (queue_entry_t) elt;		\
		(elt)->field.next = head;			\
		(elt)->field.prev = head;			\
	}							\
	else {							\
		register queue_entry_t prev;			\
								\
		prev = (head)->prev;				\
		(elt)->field.prev = prev;			\
		(elt)->field.next = head;			\
		(head)->prev = (queue_entry_t)(elt);		\
		((type)prev)->field.next = (queue_entry_t)(elt);\
	}							\
}

/*
 *	Macro:		queue_field [internal use only]
 *	Function:
 *		Find the queue_chain_t (or queue_t) for the
 *		given element (thing) in the given queue (head)
 */
#define	queue_field(head, thing, type, field)			\
		(((head) == (thing)) ? (head) : &((type)(thing))->field)

/*
 *	Macro:		queue_remove
 *	Header:
 *		void queue_remove(q, qe, type, field)
 *			arguments as in queue_enter
 */
#define	queue_remove(head, elt, type, field)			\
{								\
	register queue_entry_t	next, prev;			\
								\
	next = (elt)->field.next;				\
	prev = (elt)->field.prev;				\
								\
	queue_field((head), next, type, field)->prev = prev;	\
	queue_field((head), prev, type, field)->next = next;	\
}

/*
 *	Macro:		queue_assign
 */
#define	queue_assign(to, from, type, field)			\
{								\
	((type)((from)->prev))->field.next = (to);		\
	((type)((from)->next))->field.prev = (to);		\
	*to = *from;						\
}

#define	queue_remove_first(h, e, t, f)				\
{								\
	e = (t) queue_first((h));				\
	queue_remove((h), (e), t, f);				\
}

#define	queue_remove_last(h, e, t, f)				\
{								\
	e = (t) queue_last((h));				\
	queue_remove((h), (e), t, f);				\
}

/*
 *	Macro:		queue_enter_first
 *	Header:
 *		void queue_enter_first(q, elt, type, field)
 *			queue_t q;
 *			<type> elt;
 *			<type> is what's in our queue
 *			<field> is the chain field in (*<type>)
 */
#define queue_enter_first(head, elt, type, field)			\
{ 								\
	if (queue_empty((head))) {				\
		(head)->next = (queue_entry_t) elt;		\
		(head)->prev = (queue_entry_t) elt;		\
		(elt)->field.next = head;			\
		(elt)->field.prev = head;			\
	}							\
	else {							\
		register queue_entry_t next;			\
								\
		next = (head)->next;				\
		(elt)->field.prev = head;			\
		(elt)->field.next = next;			\
		(head)->next = (queue_entry_t)(elt);		\
		((type)next)->field.prev = (queue_entry_t)(elt);\
	}							\
}

/*
 *	Macro:		queue_last
 *	Function:
 *		Returns the last entry in the queue,
 *	Header:
 *		queue_entry_t queue_last(q)
 *			queue_t	q;		/* IN *\
 */
#define	queue_last(q)	((q)->prev)

/*
 *	Macro:		queue_prev
 *	Header:
 *		queue_entry_t queue_prev(qc)
 *			queue_t qc;
 */
#define	queue_prev(qc)	((qc)->prev)

/*
 *	Old queue stuff, will go away soon.
 */

/*
 **********************************************************************
 * HISTORY
 * 21-May-85  Mike Accetta (mja) at Carnegie-Mellon University
 *	Upgraded to 4.2BSD.  Changed to enQueue()/deQueue() names
 *	to avoid conflicts with 4.2 dequeue().
 *	[V1(1)]
 *
 * 06-Mar-80  Rick Rashid (rfr) at Carnegie-Mellon University
 *	Created (V1.05).
 *
 **********************************************************************
 */

/*
 * General purpose structure to define circular queues.
 *  Both the queue header and the queue elements have this
 *  structure.
 */

struct Queue
{
    struct Queue * F;
    struct Queue * B;
};

#define	initQueue(q)	(queue_init((queue_t)(q)))
#define	enQueue(q,elt)	(enqueue_tail((queue_t)(q),(queue_entry_t)(elt)))
#define	deQueue(q)	((struct Queue *)dequeue_head((queue_t)(q)))
#define	remQueue(q,elt)	(remqueue((queue_t)(q),(queue_entry_t)(elt)))
#define	Queueempty(q)	(queue_empty((queue_t)(q)))

#endif	_QUEUE_
