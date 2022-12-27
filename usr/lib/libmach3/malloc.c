/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * malloc.c
 *
 * $Source: /afs/cs.cmu.edu/project/mach-2/rcs/lib/mach3/mach3/malloc.c,v $
 *
 * Memory allocator for use with multiple threads.
 *
 * HISTORY
 * $Log:	malloc.c,v $
 * Revision 2.3  90/10/29  17:28:15  dpj
 * 	Added code to allow forking.
 * 	Added (compiled-out) various tracing macros, to track the
 * 	version of this file in ./pkg/malloc.
 * 	[90/10/27  17:51:14  dpj]
 * 
 * 	Added code to allow forking.
 * 	Added (compiled-out) various tracing macros, to track the
 * 	version of this file in ./pkg/malloc.
 * 	[90/10/21  21:22:42  dpj]
 * 
 * Revision 2.2  90/07/26  12:37:50  dpj
 * 	First version
 * 	[90/07/24  14:29:25  dpj]
 * 
 * Revision 2.3  90/06/02  15:14:00  rpd
 * 	Converted to new IPC.
 * 	[90/03/20  20:56:57  rpd]
 * 
 * Revision 2.2  89/12/08  19:53:59  rwd
 * 	Removed conditionals.
 * 	[89/10/23            rwd]
 * 
 * Revision 2.1  89/08/03  17:09:46  rwd
 * Created.
 * 
 *
 * 13-Sep-88  Eric Cooper (ecc) at Carnegie Mellon University
 *	Changed realloc() to copy min(old size, new size) bytes.
 *	Bug found by Mike Kupfer at Olivetti.
 */



#include <mach.h>
#include <stdio.h>

#define	ASSERT(p) {							\
	if (!(p)) {							\
		fprintf(stderr,"*** mach3.malloc: assertion failed ***\n");\
		exit(1);						\
	}								\
}

#define	MACH_CALL(expr, ret)	if (((ret) = (expr)) != KERN_SUCCESS) { \
					fprintf(stderr,			\
			"*** mach3.malloc: MACH_CALL failed ***\n");	\
					exit(1);			\
				} else


#ifdef	TRACE
extern void	malloc_trace_enter();
extern int	malloc_trace_enabled;
#define		MALLOC_TRACE_ENTER(_label_,_req_,_act_,_addr_) {	\
	if (malloc_trace_enabled) {					\
		malloc_trace_enter(_label_,_req_,_act_,_addr_);		\
	}								\
}
#else	TRACE
#define		MALLOC_TRACE_ENTER(_label_,_req_,_act_,_addr_)		/**/
#endif	TRACE

/*
 * C library imports:
 */
extern bcopy();

/*
 * Structure of memory block header.
 * When free, next points to next block on free list.
 * When allocated, fl points to free list.
 * Size of header is 4 bytes, so minimum usable block size is 8 bytes.
 */
typedef union header {
	union header *next;
	struct free_list *fl;
} *header_t;

#define MIN_SIZE	8	/* minimum block size */

typedef struct free_list {
	int lock;	/* spin lock for mutual exclusion */
	header_t head;	/* head of free list for this size */
#ifdef	DEBUG
	int in_use;	/* # mallocs - # frees */
#endif	DEBUG
} *free_list_t;

/*
 * Free list with index i contains blocks of size 2^(i+3) including header.
 * Smallest block size is 8, with 4 bytes available to user.
 * Size argument to malloc is a signed integer for sanity checking,
 * so largest block size is 2^31.
 */
#define NBUCKETS	29

static struct free_list free_list[NBUCKETS];

static void
more_memory(size, fl)
	int size;
	register free_list_t fl;
{
	register int amount;
	register int n;
	vm_address_t where;
	register header_t h;
	kern_return_t r;

	if (size <= vm_page_size) {
		amount = vm_page_size;
		n = vm_page_size / size;
		/*
		 * We lose vm_page_size - n*size bytes here.
		 */
	} else {
		amount = size;
		n = 1;
	}
#if	MACH3_UNIX
	MACH_CALL(vm_allocate(task_self(), &where, (vm_size_t) amount, TRUE), r);
#else	MACH3_UNIX
	MACH_CALL(vm_allocate(mach_task_self(), &where, (vm_size_t) amount, TRUE), r);
#endif	MACH3_UNIX
	h = (header_t) where;
	do {
		h->next = fl->head;
		fl->head = h;
		h = (header_t) ((char *) h + size);
	} while (--n != 0);
}

char *
malloc(size)
	register unsigned int size;
{
	register int i, n;
	register free_list_t fl;
	register header_t h;

	if ((int) size <= 0) {		/* sanity check */
		MALLOC_TRACE_ENTER("malloc",size,0,0);
		return 0;
	}
	size += sizeof(union header);
	/*
	 * Find smallest power-of-two block size
	 * big enough to hold requested size plus header.
	 */
	i = 0;
	n = MIN_SIZE;
	while (n < size) {
		i += 1;
		n <<= 1;
	}
	ASSERT(i < NBUCKETS);
	fl = &free_list[i];
	mach3_spin_lock(&fl->lock);
	h = fl->head;
	if (h == 0) {
		/*
		 * Free list is empty;
		 * allocate more blocks.
		 */
		more_memory(n, fl);
		h = fl->head;
		if (h == 0) {
			/*
			 * Allocation failed.
			 */
			mach3_spin_unlock(&fl->lock);
			MALLOC_TRACE_ENTER("malloc",
					size - sizeof(union header),0,0);
			return 0;
		}
	}
	/*
	 * Pop block from free list.
	 */
	fl->head = h->next;
#ifdef	DEBUG
	fl->in_use += 1;
#endif	DEBUG
	mach3_spin_unlock(&fl->lock);
	/*
	 * Store free list pointer in block header
	 * so we can figure out where it goes
	 * at free() time.
	 */
	h->fl = fl;
	/*
	 * Return pointer past the block header.
	 */
	MALLOC_TRACE_ENTER("malloc",size - sizeof(union header),
		n - sizeof(union header),((char *) h) + sizeof(union header));
	return ((char *) h) + sizeof(union header);
}

free(base)
	char *base;
{
	register header_t h;
	register free_list_t fl;
	register int i;

	if (base == 0)
		return;
	/*
	 * Find free list for block.
	 */
	h = (header_t) (base - sizeof(union header));
	fl = h->fl;
	i = fl - free_list;
	/*
	 * Sanity checks.
	 */
	if (i < 0 || i >= NBUCKETS) {
		ASSERT(0 <= i && i < NBUCKETS);
		return;
	}
	if (fl != &free_list[i]) {
		ASSERT(fl == &free_list[i]);
		return;
	}
	/*
	 * Push block on free list.
	 */
	mach3_spin_lock(&fl->lock);
	h->next = fl->head;
	fl->head = h;
#ifdef	DEBUG
	fl->in_use -= 1;
#endif	DEBUG
	mach3_spin_unlock(&fl->lock);
	MALLOC_TRACE_ENTER("free",(1 << (i+3)) - sizeof(union header),
				(1 << (i+3)) - sizeof(union header),base);
	return;
}

char *
realloc(old_base, new_size)
	char *old_base;
	unsigned int new_size;
{
	register header_t h;
	register free_list_t fl;
	register int i;
	unsigned int old_size;
	char *new_base;

	if (old_base == 0)
		return 0;
	/*
	 * Find size of old block.
	 */
	h = (header_t) (old_base - sizeof(union header));
	fl = h->fl;
	i = fl - free_list;
	/*
	 * Sanity checks.
	 */
	if (i < 0 || i >= NBUCKETS) {
		ASSERT(0 <= i && i < NBUCKETS);
		return 0;
	}
	if (fl != &free_list[i]) {
		ASSERT(fl == &free_list[i]);
		return 0;
	}
	/*
	 * Free list with index i contains blocks of size 2^(i+3) including header.
	 */
	old_size = (1 << (i+3)) - sizeof(union header);
	/*
	 * Allocate new block, copy old bytes, and free old block.
	 */
#ifdef	notdef
	MALLOC_TRACE_ENTER("realloc",new_size,0,old_base);
#endif	notdef
	new_base = malloc(new_size);
	if (new_base != 0)
		bcopy(old_base, new_base, (int) (old_size < new_size ? old_size : new_size));
	free(old_base);
	return new_base;
}

#ifdef	DEBUG
void
print_malloc_free_list()
{
  	register int i, size;
	register free_list_t fl;
	register int n;
  	register header_t h;
	int total_used = 0;
	int total_free = 0;

	fprintf(stderr, "      Size     In Use       Free      Total\n");
  	for (i = 0, size = MIN_SIZE, fl = free_list;
	     i < NBUCKETS;
	     i += 1, size <<= 1, fl += 1) {
		mach3_spin_lock(&fl->lock);
		if (fl->in_use != 0 || fl->head != 0) {
			total_used += fl->in_use * size;
			for (n = 0, h = fl->head; h != 0; h = h->next, n += 1)
				;
			total_free += n * size;
			fprintf(stderr, "%10d %10d %10d %10d\n",
				size, fl->in_use, n, fl->in_use + n);
		}
		mach3_spin_unlock(&fl->lock);
  	}
  	fprintf(stderr, " all sizes %10d %10d %10d\n",
		total_used, total_free, total_used + total_free);
}
#endif	DEBUG

void malloc_fork_prepare()
/*
 * Prepare the malloc module for a fork by insuring that no thread is in a
 * malloc critical section.
 */
{
	register int i;

	for (i = 0; i < NBUCKETS; i++) {
		mach3_spin_lock(&free_list[i].lock);
	}
}

void malloc_fork_parent()
/*
 * Called in the parent process after a fork() to resume normal operation.
 */
{
	register int i;

	for (i = NBUCKETS-1; i >= 0; i--) {
		mach3_spin_unlock(&free_list[i].lock);
	}
}

void malloc_fork_child()
/*
 * Called in the child process after a fork() to resume normal operation.
 * In the MTASK case we also have to change memory inheritance so that the
 * child does not share memory with the parent.
 */
{
	register int i;

	for (i = NBUCKETS-1; i >= 0; i--) {
		mach3_spin_unlock(&free_list[i].lock);
	}
}
