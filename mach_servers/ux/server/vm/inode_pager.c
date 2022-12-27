/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 *	File:	inode_pager.c
 *
 *	"Swap" pager that pages to/from Unix inodes.  Also
 *	handles demand paging from files.
 *
 * HISTORY
 * $Log:	inode_pager.c,v $
 * Revision 2.11  91/08/30  12:57:04  jsb
 * 	Stub out one more upcall: memory_object_change_completed()
 * 
 * Revision 2.10  91/08/12  22:38:38  rvb
 * 	Removed calls to vm_pageable.
 * 	[91/08/06            rpd]
 * 
 * Revision 2.9  91/07/30  15:35:38  rvb
 * 	From jsb: stub out two more upcalls: memory_object_data_return()
 * 	and memory_object_supply_completed()
 * 	[91/07/29            rvb]
 * 
 * Revision 2.8  90/08/06  15:35:50  rwd
 * 	Move memory_object_set_attributes in memory_object_init to
 * 	before letting the outside world know about the pager_request
 * 	port.
 * 	[90/08/05            rwd]
 * 	Make sure pager_wait_lock is conditionalized to MACH_NBC.
 * 	[90/07/31            rwd]
 * 	ip->pager_wait_lock now governs ip->pager,pager_request and
 * 	waiting.
 * 	[90/07/27            rwd]
 * 	Add debugging code.
 * 	[90/07/10            rwd]
 * 
 * Revision 2.7  90/06/19  23:16:58  rpd
 * 	Backed out the inode_write/bmap optimization that avoids
 * 	clearing and overwriting blocks, pending a real fix.
 * 	[90/06/19            rpd]
 * 
 * 	Fixed inode_pager_data_write to ignore writes that are entirely
 * 	past EOF, and correctly deallocate memory for writes that
 * 	overlap EOF by a page or more.
 * 	[90/06/11            rpd]
 * 
 * 	Fixed unix_release bug in inode_pager_no_senders.
 * 	[90/06/10            rpd]
 * 
 * Revision 2.6  90/06/02  15:29:24  rpd
 * 	Fixed calls to memory_object_set_attributes so the master lock
 * 	isn't held.  They might block, leading to deadlock.
 * 
 * 	Added missing unix_release in memory_object_data_request.
 * 	[90/04/04            rpd]
 * 
 * 	Condensed history.  Purged default-pager support:
 * 	inode_pager_iput, inode_pager_iget, inode_pager_active,
 * 	memory_object_create, memory_object_data_initialize,
 * 	inode_has_page, inode_swap_preference, etc.  Eliminated
 * 	inode_port_entry zone; merged hash links into the inode_pager
 * 	structure.  Added no-senders detection to do cleanup.
 * 	Fixed unix_master/unix_release bugs.
 * 	[90/03/28            rpd]
 * 
 * 	Fixed inode_uncache and inode_uncache_try
 * 	to do something reasonable.
 * 	[90/03/27            rpd]
 * 	Converted to new IPC.
 * 	Removed MACH_NO_KERNEL conditionals.
 * 	Removed data-request queue.
 * 	[90/03/26  20:30:56  rpd]
 * 
 * Revision 2.5  90/05/29  20:25:36  rwd
 * 	Inode_size logic update for server mapped files.
 * 	[90/05/04            rwd]
 * 	Added rfr'r multiple page writeback code.
 * 	[90/04/12            rwd]
 * 
 * 	Get rpd's fix for inode_uncache*.
 * 	[90/04/08            rwd]
 * 
 * 	Get inode_size from user shared area in MAP_UAREA case.
 * 	[90/04/05            rwd]
 * 
 *
 * 	Condensed history:
 *	NBC changes (rfr).
 *	Wired cthread to a kernel thread (rwd).
 *	Out-of-kernel-version (dbg).
 *	Fake no-senders detections with a "dead" ref count.  (mwyoung)
 *	Allow multiple clients of a memory object.  (mwyoung)
 *	Use memory_object_data_error.  (mwyoung)
 *	Remember errors on a per-object basis. (mwyoung)
 *	Added inode_pager_release(). (mwyoung)
 *	Try to avoid deadlock when allocating data structures  (avie, mwyoung).
 *	Try to printf rather than panic when faced with errors (avie).
 *	"No buffer code" enhancements. (avie)
 *	External paging version. (mwyoung, bolosky)
 *	Allow pageout to ask whether a page has been written out.  (dbg)
 *	Keep only a pool of in-core inodes.  (avie)
 *	Use readahead when able. (avie)
 *	Require that inode operations occur
 *	 on the master processor (avie, rvb, dbg).
 *	Combine both "program text" and "normal file" handling
 *	 into one. (avie, mwyoung)
 *	Allocate paging inodes on mounted filesystems (mja);
 *	 allow preferences to be supplied (mwyoung).
 *
 * 12-Mar-86  David Golub (dbg) at Carnegie-Mellon University
 *	Created.
 */

#include <mach_nbc.h>

#include <sys/types.h>
#include <sys/param.h>
#include <sys/buf.h>
#include <sys/dir.h>
#include <sys/fs.h>
#include <sys/inode.h>
#include <sys/map.h>
#include <sys/mfs.h>
#include <sys/mount.h>
#include <sys/parallel.h>
#include <sys/queue.h>
#include <sys/systm.h>
#include <sys/uio.h>
#include <sys/user.h>
#include <sys/zalloc.h>
#include <uxkern/import_mach.h>
#include <vm/inode_pager.h>

/*
 * Hack around missing XPR debugging stuff
 */
#define	XPR(a,b)

#define	assert(ex)	ASSERT(ex)

/*
 * Temporary extern...
 */
extern mach_port_t	privileged_host_port;

#ifndef	private
#define	private
#endif	private

boolean_t	inode_pager_debug = FALSE;


/*
 *	Inode_pager data structure protocols:
 *
 *	The basic data structure is the inode_pager_t, which
 *	represents a particular (on-disk) inode, and thus a
 *	particular memory object.
 *
 *	Inode_pager objects can be created through one mechanism,
 *	a user mapping request for a particular inode.
 *
 *	To satisfy a user request, the inode_pager must first find
 *	the memory_object associated with that inode if it has
 *	already been created.  [The inode must be resident, because
 *	the requestor must have a reference.]  Thus, the inode
 *	structure has been modified to contain the memory_object port.
 *
 *	This field in the inode is also used to prevent the
 *	destruction/truncation of inodes used strictly for paging.
 *
 *	Currently, the routines in the inode_pager are called
 *	from either of two sources:
 *		Inode_pager task:
 *			A separate singly-threaded task is created
 *			to service the memory management interface.
 *		Kernel-state user threads:
 *			Mapping and cache control requests are made
 *			from the kernel context of the client thread.
 *	The kernel-context threads, as well as outside use of the inode
 *	data structure, requires the inode_pager to synchronize as
 *	though it were multi-threaded.
 *
 *	The routines that act within the inode_pager task make all
 *	Mach kernel function requests through the RPC interface, and
 *	thus use the non-KERNEL data types for data objects (tasks,
 *	memory objects, ports).  Currently, the value of task_self()
 *	for the inode_pager task is cached in the inode_pager_self
 *	variable.
 *
 *	Despite being a separate task, the inode_pager runs within
 *	the kernel virtual address space, so that it may directly access
 *	inode data structures.  Memory allocation may also be done
 *	using the internal kernel functions.
 *
 *	The kernel-context threads also complicate port management.
 *	In addition to maintaining the port names known to the inode_pager
 *	task (including conversion between port names and inode_pager_t's),
 *	the data structures must contain the global names for the
 *	memory_object and memory_object_control ports.
 *
 *	Port rights and references are maintained as follows:
 *		Memory object port:
 *			The inode_pager task has all rights, and
 *			keeps one reference for the global name stored
 *			with the inode_pager_t structure.  [The port
 *			recorded in the inode is the global name, and is
 *			copied from the	inode_pager_t, sharing that
 *			reference.]
 *		Memory object control port:
 *			The inode_pager task has only send rights,
 *			and keeps one reference for the global name
 *			it stores.  [As with the memory_object port,
 *			the global name is copied into the inode itself,
 *			so that control functions can be instigated by
 *			kernel-context client threads.]
 *		Memory object name port:
 *			The inode_pager task has only send rights.
 *			The name port is not even recorded.
 *	Regardless how the object is created, the control and name
 *	ports are created by the kernel and passed through the memory
 *	management interface.
 *
 *	The inode_pager assumes that access to its memory objects
 *	will not be propagated to more that one host, and therefore
 *	provides no consistency guarantees beyond those made by the
 *	kernel.
 *
 *	In the event that more than one host attempts to use an inode
 *	memory object, the inode_pager will only record the last set
 *	of port names.  [This can happen with only one host if a new
 *	mapping is being established while termination of all previous
 *	mappings is taking place.]  Currently, the inode_pager assumes
 *	that its clients adhere to the initialization and termination
 *	protocols in the memory management interface; otherwise, port
 *	rights or out-of-line memory from erroneous messages may be
 *	allowed to accumulate.
 *
 *	As mentioned above, the inode_pager can also provide the backing
 *	storage for temporary memory objects.  Thus, it must adhere to
 *	the restrictions placed on default memory managers for those
 *	temporary objects (and currently, for other objects as well).
 *	
 *	[The phrase "currently" has been used above to denote aspects of
 *	the implementation that could be altered without changing the rest
 *	of the basic documentation.]
 */

/*
 *	Basic inode pager structure
 */

typedef struct inode_pager {
	queue_chain_t	links;		/* for hash table */

	int		client_count;	/* a sanity check */

	memory_object_t	pager;		/* Pager port */
	mach_port_t	pager_request;	/* Known request port */
	boolean_t	cached;		/* Can be cached? */

	mach_port_mscount_t mscount;	/* Number of send rights created */
	mach_port_urefs_t urefs;	/* Number of urefs for request port */

	struct inode	*ip;		/* in memory inode */
	int		errors;		/* Pageout error count */
} *inode_pager_t;

#define	INODE_PAGER_NULL	((inode_pager_t) 0)


/*
 *	Basic inode pager data structures
 */

zone_t		inode_pager_zone;

mach_port_t	inode_pager_self;
mach_port_t	inode_pager_enabled_set;
mach_port_t	inode_pager_default;

mach_port_urefs_t inode_pager_max_urefs = 10000;

inode_pager_t inode_pager_check_request();

/*
 *	Stuff for keeping a hash table of ports for inode_pager
 *	backed objects.
 */

#define	INODE_PORT_HASH_COUNT	256

private
queue_head_t	inode_port_hashtable[INODE_PORT_HASH_COUNT];

/*
 *	We add the low 8 bits to the high 24 bits, because
 *	this interacts well with the way the IPC implementation
 *	allocates port names.
 */

#define inode_port_hash(port)					\
		((((port) & 0xff) + ((port) >> 8)) &		\
		 (INODE_PORT_HASH_COUNT - 1))

private
void inode_port_hash_insert(rec)
	inode_pager_t	rec;
{
	mach_port_t name_port = rec->pager;

	queue_enter(&inode_port_hashtable[inode_port_hash(name_port)],
		    rec, inode_pager_t, links);
}

private
void inode_port_hash_init()
{
	register int i;

	for (i = 0; i < INODE_PORT_HASH_COUNT; i++) 
		queue_init(&inode_port_hashtable[i]);
}

private
inode_pager_t inode_pager_lookup(name_port)
	register mach_port_t name_port;
{
	register queue_t bucket =
		&inode_port_hashtable[inode_port_hash(name_port)];

	register inode_pager_t entry;

	for (entry = (inode_pager_t) queue_first(bucket);
	     !queue_end(bucket, &entry->links);
	     entry = (inode_pager_t) queue_next(&entry->links))
		if (entry->pager == name_port)
			return entry;

	panic("inode_pager_lookup");
}

private
void inode_port_hash_delete(is)
	register inode_pager_t is;
{
	mach_port_t name_port = is->pager;
	register queue_t bucket =
		&inode_port_hashtable[inode_port_hash(name_port)];

	queue_remove(bucket, is, inode_pager_t, links);
}

#if	MACH_NBC
#else	MACH_NBC
#define	mfs_uncache()
#endif	MACH_NBC

void inode_uncache(ip)
	struct inode	*ip;
{
	mach_port_t request;

	mfs_uncache(ip);
	if ((request = ip->pager_request) != MACH_PORT_NULL) {
		/*
		 * To avoid deadlocking with the inode-pager thread,
		 * we must release the master lock.  Note set-attributes
		 * might block if the pager is just being terminated.
		 */

		if (u.u_master_lock)
			master_unlock();

		(void) memory_object_set_attributes(request,
				TRUE, FALSE,	/* do not cache */
				MEMORY_OBJECT_COPY_DELAY);

		if (u.u_master_lock)
			master_lock();
	}
}

boolean_t inode_uncache_try(ip)
	struct inode	*ip;
{
	extern mach_port_t mig_get_reply_port();
	mig_reply_header_t msg;
	mach_msg_return_t mr;
	mach_port_t request;

	mfs_uncache(ip);
	if ((request = ip->pager_request) == MACH_PORT_NULL)
	    return (FALSE);

	/*
	 * To avoid deadlocking with the inode-pager thread,
	 * we must release the master lock.  Note set-attributes
	 * might block if the pager is just being terminated.
	 */

	if (u.u_master_lock)
		master_unlock();

	/*
	 * Uncache the object, then see whether that has killed it.
	 * Note this has the unfortunate attribute of marking the
	 * object as not cacheable, if we didn't kill it.
	 */

	(void) memory_object_set_attributes(request,
				TRUE, FALSE,	/* do not cache */
				MEMORY_OBJECT_COPY_DELAY);

	/*
	 * Make an RPC to inode_pager_default, to make sure that any
	 * messages generated by the set-attributes have been received.
	 */

	msg.Head.msgh_bits = MACH_MSGH_BITS(MACH_MSG_TYPE_MAKE_SEND,
					    MACH_MSG_TYPE_MAKE_SEND_ONCE);
	msg.Head.msgh_size = 0;
	msg.Head.msgh_remote_port = inode_pager_default;
	msg.Head.msgh_local_port = mig_get_reply_port();
	msg.Head.msgh_kind = MACH_MSGH_KIND_NORMAL;
	msg.Head.msgh_id = (mach_msg_id_t) inode_uncache_try;

	mr = mach_msg(&msg.Head, MACH_SEND_MSG|MACH_RCV_MSG,
		      sizeof msg.Head, sizeof msg, msg.Head.msgh_local_port,
		      MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
	if (mr != MACH_MSG_SUCCESS)
		panic("inode_uncache_try");

	if (u.u_master_lock)
		master_lock();

	/*
	 * Now we can check if the object has been terminated.
	 */

	return (ip->pager == MACH_PORT_NULL);
}

/*
 *	inode_pager_create
 *
 *	Create an istruct corresponding to the given ip.
 *
 *	This may potentially cause other incore inodes to be
 *	released (but remembered by the istruct).
 *
 *	Must hold master_lock.
 */
private
inode_pager_t inode_pager_create(ip)
	register struct inode	*ip;
{
	register inode_pager_t	is;

	is = (inode_pager_t) zalloc(inode_pager_zone);
	assert(is != INODE_PAGER_NULL);

	is->client_count = 0;

	is->pager = MEMORY_OBJECT_NULL;
	is->pager_request = MACH_PORT_NULL;
	is->cached = FALSE;

	is->mscount = 0;
	is->urefs = 0;

	is->ip = ip;
	is->errors = 0;

	iincr_chk(ip);

	XPR(XPR_INODE_PAGER, ("inode_pager_create: returning %x", (int) is));
	return(is);
}

/*
 *	Routine:	inode_pager_setup
 *	Purpose:
 *		Returns a memory object (that may be used in
 *		a call to vm_map) representing the given inode.
 *	Side effects:
 *		When the memory object returned by this call
 *		is no longer needed (e.g., it has been mapped
 *		into the desired address space), it should be
 *		deallocated using inode_pager_release.
 */
memory_object_t	inode_pager_setup(ip, is_text, can_cache)
	struct inode	*ip;
	boolean_t	is_text;
	boolean_t	can_cache;
{
	inode_pager_t is;
	mach_port_t pager;
	kern_return_t kr;

	if (ip == (struct inode *) 0)
		return MEMORY_OBJECT_NULL;

	unix_master();

	if (is_text)
		ip->i_flag |= ITEXT;

	if ((pager = ip->pager) == MEMORY_OBJECT_NULL) {
		mach_port_t previous;

		is = inode_pager_create(ip);

		kr = mach_port_allocate(inode_pager_self,
					MACH_PORT_RIGHT_RECEIVE, &pager);
		if (kr != KERN_SUCCESS)
			panic("inode_pager_setup: can't allocate port");

		kr = mach_port_request_notification(inode_pager_self, pager,
					MACH_NOTIFY_NO_SENDERS, 1,
					pager, MACH_MSG_TYPE_MAKE_SEND_ONCE,
					&previous);
		if ((kr != KERN_SUCCESS) || (previous != MACH_PORT_NULL))
			panic("inode_pager_setup: request_notify");

		(void) mach_port_move_member(inode_pager_self,
					     pager, inode_pager_enabled_set);

		ip->pager = is->pager = pager;
		is->cached = can_cache;

		inode_port_hash_insert(is);
	} else {
		is = inode_pager_lookup(pager);
	}

	/*
	 *	Create a send right to the memory object for our caller.
	 *	Our caller should eventually dispose of the send right
	 *	by calling inode_pager_release.
	 */

	kr = mach_port_insert_right(inode_pager_self,
				    pager, pager,
				    MACH_MSG_TYPE_MAKE_SEND);
	if (kr != KERN_SUCCESS)
	    panic("inode_pager_setup: can't acquire send rights");

	is->mscount++;		/* created a send right */

	unix_release();
	return pager;
}

/*
 *	Routine:	inode_pager_release
 *	Purpose:
 *		Relinquish any references or rights that were
 *		associated with the result of a call to
 *		inode_pager_setup.
 *	NOTE:
 *		This call, like inode_pager_setup, does not run
 *		in the context of the inode_pager.
 */
void inode_pager_release(object)
	memory_object_t	object;
{
	(void) mach_port_deallocate(inode_pager_self, object);
}

#define	PAGER_SUCCESS		0	/* page read or written */
#define	PAGER_ABSENT		1	/* pager does not have page */
#define	PAGER_ERROR		2	/* pager unable to read or write page */
#define	PAGER_DONE		3	/* operation already performed */

typedef	int		pager_return_t;

pager_return_t	inode_read();	/* forward */
pager_return_t	inode_write();	/* forward */


/*
 *	Rename all of the functions in the pager interface,
 *	to avoid confusing them with the kernel interface
 */

#define	memory_object_init		inode_pager_init
#define	memory_object_terminate		inode_pager_terminate
#define	memory_object_data_request	inode_pager_data_request
#define	memory_object_data_unlock	inode_pager_data_unlock
#define	memory_object_data_write	inode_pager_data_write
#define	memory_object_copy		inode_pager_copy
#define	memory_object_lock_completed	inode_pager_lock_completed


int		inode_pager_pagein_count = 0;
int		inode_pager_pageout_count = 0;
vm_offset_t	inode_pager_input_buffer;

kern_return_t	memory_object_data_request(pager, reply_to, offset,
				   length, protection_required)
	memory_object_t	pager;
	mach_port_t	reply_to;
	vm_offset_t	offset;
	vm_size_t	length;
	vm_prot_t	protection_required;
{
	inode_pager_t is;
	struct inode *ip;
	pager_return_t pr;
	kern_return_t result;

	if (inode_pager_debug)
		printf("%s: pager=%d, offset=0x%x, length=0x%x\n",
			"(inode_pager)data_request",
			pager, offset, length);

	if (length != vm_page_size)
		panic("(inode_pager)data_request: bad length");

	unix_master();
	is = inode_pager_check_request(pager, reply_to);

	if (is->errors) {
		unix_release();
		printf("(inode_pager)data_request: previous errors\n");
		(void) memory_object_data_error(reply_to,
						offset, vm_page_size,
						KERN_FAILURE);

		return (KERN_SUCCESS);
	}

	ip = is->ip;
	ilock(ip);

	pr = inode_read(ip, reply_to, offset,
			inode_pager_input_buffer, offset);

	if (inode_pager_debug)
		printf("%s: ip=%x, offset=0x%x, return=%x\n",
			"(inode_pager)inode_read", ip, offset, pr);

	iunlock(ip);
	inode_pager_pagein_count++;
	unix_release();

	switch (pr) {
	    case PAGER_DONE:
		/* inode_read did the data-provided call */
		break;

	    case PAGER_SUCCESS:
		result = memory_object_data_provided(
				reply_to, offset,
				inode_pager_input_buffer, vm_page_size,
				VM_PROT_NONE);
		break;

	    case PAGER_ABSENT:
		result = memory_object_data_unavailable(
				reply_to, offset,
				vm_page_size);
		break;

	    case PAGER_ERROR:
		result = memory_object_data_error(
				reply_to, offset,
				vm_page_size, KERN_FAILURE);
		break;

	    default:
		panic("inode_pagein: bogus return from inode_read");
	}

	return(KERN_SUCCESS);
}

/*
 * memory_object_data_write: split up the stuff coming in from
 * a memory_object_data_write call into individual pages and
 * pass them off to inode_write.
 */
kern_return_t memory_object_data_write(pager, pager_request,
					offset, addr, data_cnt)
	memory_object_t	pager;
	mach_port_t	pager_request;
	register
	vm_offset_t	offset;
	register
	pointer_t	addr;
	vm_size_t	data_cnt;
{
	vm_size_t	size;
	register struct inode	*ip;
	inode_pager_t	is;
	pager_return_t	ret;

	XPR(XPR_INODE_PAGER_DATA, ("memory_object_data_write(inode_pager): pager 0x%x, offset 0x%x",
				pager, offset));

	if (inode_pager_debug)
		printf("%s: pager=%d, offset=0x%x, length=0x%x\n",
			"pager_data_write(inode_pager)",
			pager, offset, data_cnt);

	if ((data_cnt % vm_page_size) != 0) {
	    printf("(inode_pager)memory_object_data_write: not a multiple of a page");
	    data_cnt = round_page(data_cnt);
	}

	unix_master();

	is = inode_pager_check_request(pager, pager_request);
	ip = is->ip;

	size = data_cnt;

	ilock(ip);

#if	MACH_NBC
    {
	u_long inode_size;

	if (ip->mapped)
		inode_size = mfs_inode_size(ip);
	else {
		inode_size = ip->inode_size;	/* not i_size */

		/* XXX Something bad is happening... */
		printf("(inode_pager)data_write: not mapped\n");
		printf("inode_size = %x i_size = %x offset = %x size = %x\n",
		       inode_size, ip->i_size, offset, size);
	}

	/*
	 *	Ensure that a paging operation doesn't
	 *	accidently extend a "mapped" file.
	 */

	if (offset >= inode_size) {
	    /* entire write is past EOF */

	    iunlock(ip);
	    unix_release();
	    (void) vm_deallocate(mach_task_self(), addr, data_cnt);
	    return KERN_SUCCESS;
	} else if (offset + size > inode_size) {
	    vm_size_t rounded_size;

	    /* part of the write is past EOF */

	    size = inode_size - offset;
	    if ((rounded_size = round_page(size)) != data_cnt)
		(void) vm_deallocate(mach_task_self(), addr + rounded_size,
				     data_cnt - rounded_size);
	}
    }
#endif	MACH_NBC

	ret = inode_write(ip, addr, size, offset);
	iunlock(ip);

	switch (ret) {
	    case PAGER_SUCCESS:
		(void) vm_deallocate(inode_pager_self, addr, size);
		break;

	    case PAGER_DONE:
		/* inode_write is keeping the data */
		break;

	    default:
		(void) vm_deallocate(inode_pager_self, addr, size);

		printf("(inode_pager)data_write: error = %d\n",
			u.u_error);
		u.u_error = 0;
		is->errors++;
		break;
	}

	inode_pager_pageout_count += size/vm_page_size;
	unix_release();

	if (inode_pager_debug)
		printf("%s: pager=%d, offset=0x%x, length=0x%x ret = %d\n",
			"pager_data_write(inode_pager) done",
			pager, offset, data_cnt, ret);

	return(KERN_SUCCESS);
}

kern_return_t	memory_object_data_unlock(pager, reply_to, offset,
				   length, protection_required)
	memory_object_t	pager;
	mach_port_t	reply_to;
	vm_offset_t	offset;
	vm_size_t	length;
	vm_prot_t	protection_required;
{
#ifdef	lint
	pager++; reply_to++; offset++; length++; protection_required++;
#endif	lint
	panic("(inode_pager)data_unlock: called");
}

kern_return_t	memory_object_copy(old_memory_object, old_memory_control,
					offset, length,
					new_memory_object)
	memory_object_t	old_memory_object;
	memory_object_control_t
			old_memory_control;
	vm_offset_t	offset;
	vm_size_t	length;
	memory_object_t	new_memory_object;
{
#ifdef	lint
	old_memory_object++; old_memory_control++; offset++; length++; new_memory_object++;
#endif	lint
	panic("(inode_pager)copy: called");
}

kern_return_t
memory_object_data_return(memory_object, memory_control, offset, data, length,
			  dirty, kernel_copy)
	memory_object_t	memory_object;
	memory_object_control_t
			memory_control;
	vm_offset_t	offset;
	vm_offset_t	data;
	vm_size_t	length;
	boolean_t	dirty;
	boolean_t	kernel_copy;
{
#ifdef	lint
	memory_object++; memory_control++; offset++;
	data++; length++; dirty++; kernel_copy++;
#endif	lint
	panic("(inode_pager)data_return: called");
}

kern_return_t
memory_object_supply_completed(memory_object, memory_control, offset, length,
			       result, error_offset)
	memory_object_t	memory_object;
	memory_object_control_t
			memory_control;
	vm_offset_t	offset;
	vm_size_t	length;
	kern_return_t	result;
	vm_offset_t	error_offset;
{
#ifdef	lint
	memory_object++; memory_control++; offset++;
	length++; result++; error_offset++;
#endif	lint
	panic("(inode_pager)supply_completed: called");
}

kern_return_t	memory_object_lock_completed(pager, pager_request,
					     offset, length)
	memory_object_t	pager;
	mach_port_t	pager_request;
	vm_offset_t	offset;
	vm_size_t	length;
{
	inode_pager_t is;
	struct inode *ip;

	unix_master();
	is = inode_pager_check_request(pager, pager_request);
	ip = is->ip;

	if (inode_pager_debug&0x10)
		printf("Memory object lock request %x, %x, %x\n",
		       pager, is, ip->i_id);

#if	MACH_NBC
	mutex_lock(&ip->pager_wait_lock);
	if (ip->waiting) {
		ip->waiting = FALSE;
		condition_broadcast(&ip->pager_wait);

		if (inode_pager_debug&0x10)
			printf("condition broadcast\n");
	}
	mutex_unlock(&ip->pager_wait_lock);
#endif	MACH_NBC

	unix_release();
	return(KERN_SUCCESS);
}

kern_return_t
memory_object_change_completed(memory_object, may_cache, copy_strategy)
	memory_object_t	memory_object;
	boolean_t	may_cache;
	int		copy_strategy;
{
#ifdef	lint
	memory_object++; may_cache++; copy_strategy++;
#endif	lint
	panic("(inode_pager)change_completed: called");
}

int		inode_read_aheads = 0;
int		inode_read_individuals = 0;

pager_return_t	inode_read(ip, reply_to, r_offset, buffer, f_offset)
	register struct inode	*ip;
	mach_port_t		reply_to;
	vm_offset_t		r_offset;
	vm_offset_t		buffer;
	vm_offset_t		f_offset;
{
	vm_offset_t	offset;		/* byte offset within object*/
	vm_offset_t	p_offset;	/* byte offset within physical page */
	dev_t		dev;
	register struct fs	*fs;
	daddr_t		lbn, bn;
	int		size;
	long		bsize;
	int		csize, on, n, save_error, err;
	u_long		diff;
	struct buf	*bp;


	/*
	 *	Get the inode and the offset within it to read from.
	 */

	p_offset = 0;

	dev = ip->i_dev;
	fs = ip->i_fs;
	bsize = fs->fs_bsize;
	csize = vm_page_size;

	/*
	 * Be sure that data not in the file is zero filled.
	 * The easiest way to do this is to zero the entire
	 * page now.
	 */

	/*
	if (ip->i_size < (f_offset + csize)) {
	    bzero((caddr_t) buffer, vm_page_size);
	}
	*/

	/*
	 *	Read from the inode until we've filled the page.
	 */
	do {
	    /*
	     *	Find block and offset within it for our data.
	     */
	    lbn = lblkno(fs, f_offset);	/* logical block number */
	    on  = blkoff(fs, f_offset);	/* byte offset within block */

	    /*
	     *	Find the size we can read - don't go beyond the
	     *	end of a block.
	     */
	    n = MIN((unsigned)(bsize - on), csize);
	    if (ip->i_size <= f_offset) {
		if (p_offset == 0) {
		    /*
		     * entire block beyond end of file -
		     * doesn't exist
		     */
		    return(PAGER_ABSENT);
		 }
		/*
		 * block partially there - zero the rest of it
		 */
		break;
	    }
	    diff = ip->i_size - f_offset;
	    if (diff < n)
		n = diff;

	    /*
	     *	Read the index to find the disk block to read
	     *	from.  If there is no block, report that we don't
	     *	have this data.
	     *
	     *	!!! Assumes that:
	     *		1) Any offset will be on a fragment boundary
	     *		2) The inode has whole page
	     */
	    save_error = u.u_error;
	    u.u_error = 0;
	    /* changes u.u_error! */
	    bn = fsbtodb(fs, bmap(ip, lbn, B_READ, (int)(on+n)));
	    err = u.u_error;
	    u.u_error = save_error;

	    if (err) {
		printf("IO error on pagein: error = %d.\n",err);
		return(PAGER_ERROR);
	    }

	    if ((long)bn < 0)
		return(PAGER_ABSENT);

	    size = blksize(fs, ip, lbn);

	    /*
	     *	Read the block through the buffer pool,
	     *	then copy it to the physical memory already
	     *	allocated for this page.
	     */

	    if ((ip->i_lastr + 1) == lbn) {
		inode_read_aheads++;
		bp = breada(dev, bn, size, rablock, rasize);
	    } else {
		inode_read_individuals++;
		bp = bread(dev, bn, size);
	    }
	    ip->i_lastr = lbn;

	    if (bp->b_flags & B_ERROR) {
		brelse(bp);
		printf("IO error on pagein (bread)\n");
		return(PAGER_ERROR);
	    }

	    n = MIN(n, size - bp->b_resid);
	    if ((unsigned int) n == vm_page_size &&
	        on == 0 && p_offset == 0) {
		(void) memory_object_data_provided(
				reply_to, r_offset,
				bp->b_un.b_addr, vm_page_size,
				VM_PROT_NONE);

		bp->b_flags |= B_AGE;
		bp->b_flags |= B_PAGET;
	    	brelse(bp);
		return (PAGER_DONE);
	    }

	    bcopy(bp->b_un.b_addr+on, (caddr_t) buffer + p_offset,
		  (unsigned int) n);

	    /*
	     *	Account for how much we've read this time
	     *	around.
	     */
	    csize -= n;
	    p_offset += n;
	    f_offset += n;

	    if (n + on == bsize || f_offset == ip->i_size) {
		bp->b_flags |= B_AGE;
		bp->b_flags |= B_PAGET;
	    }
	    brelse(bp);

	} while (csize > 0 && n != 0);

	/*
	 * Be sure that data not in the file is zero filled.
	 */
	if (csize > 0)
	    bzero((caddr_t) buffer + (vm_page_size-csize), csize);

	return(PAGER_SUCCESS);
}

pager_return_t	inode_write(ip, addr, csize, f_offset)
	register struct inode	*ip;
	vm_offset_t	addr;
	vm_size_t	csize;
	vm_offset_t	f_offset;	/* byte offset within file block */
{
	vm_offset_t	p_offset;	/* byte offset within physical page */
	dev_t		dev;
	register struct fs	*fs;
	daddr_t		lbn, bn;
	int		size;
	long		bsize;
	int		on, n, save_error, err;
	struct buf	*bp;
#if	MACH_NBC
	extern int	nbc_debug;
#endif	MACH_NBC

	p_offset = 0;

	dev = ip->i_dev;
	fs = ip->i_fs;
	bsize = fs->fs_bsize;

#if	MACH_NBC
	if ((nbc_debug & 0x8) && ip->pager_request) {
		uprintf("inode_write: ip 0x%x, f_offset = %d, size = %d.\n",
				ip, f_offset, csize);
	}
#endif	MACH_NBC

	do {
	    lbn = lblkno(fs, f_offset);	/* logical block number */
	    on  = blkoff(fs, f_offset);	/* byte offset within block */

	    n   = MIN((unsigned)(bsize - on), csize);

	    save_error = u.u_error;
	    u.u_error = 0;
	    /* changes u.u_error! */

	    /*
	     *	The B_XXX argument to the bmap() call is used
	     *	by the NBC system to direct inode flushing.
	     */

	    bn = fsbtodb(fs, bmap(ip, lbn, B_WRITE | B_XXX, (int)(on+n) ));
	    err = u.u_error;
	    u.u_error = save_error;

	    if (err || (long) bn < 0) {
		printf("IO error on pageout: error = %d.\n",err);
		return(PAGER_ERROR);
	    }

	    if (f_offset + n > ip->i_size) {
		ip->i_size = f_offset + n;
#if	MACH_NBC
			if (nbc_debug & 0x8) {
				uprintf("inode extended to %d bytes\n",
					ip->i_size);
			}
#endif	MACH_NBC
	    }

	    size = blksize(fs, ip, lbn);

	    if (n == bsize) 
		bp = getblk(dev, bn, size);
	    else
		bp = bread(dev, bn, size);

	    if (bp->b_flags & B_ERROR) {
		brelse(bp);

		printf("IO error on pageout (bread)\n");
		return(PAGER_ERROR);
	    }

	    n = MIN(n, size - bp->b_resid);
	    if ((n == bsize) && (p_offset == 0) &&
		(csize == n) && (on == 0)) {
		(void) vm_deallocate(inode_pager_self,
				     (vm_offset_t)bp->b_un.b_addr,
				     (vm_size_t)bp->b_bufsize);
		bp->b_un.b_addr = (caddr_t) addr;
		bp->b_bufsize = n;
	    	bp->b_flags |= B_AGE;
		bp->b_flags |= B_PAGET;
		bawrite(bp);
		ip->i_flag |= IUPD|ICHG;
		return (PAGER_DONE);
	    }

	    bcopy((caddr_t) (addr + p_offset),
			      bp->b_un.b_addr+on,
			      (unsigned int) n);
#if	0
	    if ((on+n) < bp->b_bcount) {
		    blkclr(bp->b_un.b_addr+on+n, 
			   (unsigned int)bp->b_bcount - (on + n));
	    }
#endif

	    csize -= n;
	    p_offset += n;
	    f_offset += n;

	    if (n + on == bsize) {
		bp->b_flags |= B_AGE;
		bp->b_flags |= B_PAGET;
		bawrite(bp);
	    } else
		bdwrite(bp);

	    ip->i_flag |= IUPD|ICHG;

	} while (csize != 0 && n != 0);

	return(PAGER_SUCCESS);
}

memory_object_copy_strategy_t inode_copy_strategy = MEMORY_OBJECT_COPY_DELAY;

kern_return_t
memory_object_init(pager, pager_request, pager_name, pager_page_size)
	mach_port_t	pager;
	mach_port_t	pager_request;
	mach_port_t	pager_name;
	vm_size_t	pager_page_size;
{
	register inode_pager_t	is;
	register struct inode	*ip;
	boolean_t	cached;

	if (inode_pager_debug)
		printf("(inode_pager)init: pager=%d, request=%d, name=%d\n",
		       pager, pager_request, pager_name);

	if (pager_page_size != vm_page_size)
		panic("(inode_pager)init: wrong page size");

	assert(MACH_PORT_VALID(pager_request));
	assert(MACH_PORT_VALID(pager_name));

	unix_master();
	is = inode_pager_lookup(pager);
	ip = is->ip;

	if (is->client_count++ > 0)
		panic("(inode_pager)init: multiple clients");

	assert(is->pager_request == MACH_PORT_NULL);
	cached = is->cached;

	/*
	 *	Reply to the kernel:  the memory object is ready.
	 *	This set-attributes should never block since it is
	 *	the first message sent to the request port.
	 */

	(void) memory_object_set_attributes(pager_request, TRUE,
					    cached, inode_copy_strategy);

#if	MACH_NBC
	mutex_lock(&ip->pager_wait_lock);
#endif	MACH_NBC
	is->pager_request = pager_request;
	ip->pager_request = pager_request;
#if	MACH_NBC
	if (ip->waiting) {
		ip->waiting = FALSE;
		condition_broadcast(&ip->pager_wait);
	}
	mutex_unlock(&ip->pager_wait_lock);
#endif	MACH_NBC

	assert(is->urefs == 0);
	is->urefs = 1;

	unix_release();

	/*
	 *	We have no use for the name port.
	 *	Deallocate our send rights immediately.
	 */

	(void) mach_port_deallocate(inode_pager_self, pager_name);

	return(KERN_SUCCESS);
}

kern_return_t	memory_object_terminate(pager, pager_request, pager_name)
	mach_port_t	pager;
	mach_port_t	pager_request;
	mach_port_t	pager_name;
{
	inode_pager_t	is;
	mach_port_urefs_t urefs;

	assert(MACH_PORT_VALID(pager_request));
	assert(MACH_PORT_VALID(pager_name));

	if (inode_pager_debug)
		printf("%s: pager=%d, request=%d, name=%d\n",
			"memory_object_terminate(inode_pager)",
			pager, pager_request, pager_name);

	/*
	 *	Deallocate the name port; we don't need it.
	 */

	(void) mach_port_mod_refs(inode_pager_self, pager_name,
				  MACH_PORT_RIGHT_RECEIVE, -1);

	/*
	 *	The kernel can call memory_object_init on this memory_object
	 *	and reuse it before the last send right disappears.
	 *	So, when we are done the object has to be ready for
	 *	either memory_object_init or inode_pager_no_senders.
	 */

	unix_master();

	is = inode_pager_lookup(pager);

	urefs = is->urefs;

	assert(is->client_count == 1);
	assert(is->pager_request == pager_request);
	assert(urefs > 0);

	is->client_count--;
	is->urefs = 0;

	/*
	 *	Clear the request port.
	 *	Do this before deallocating it.
	 */

#if	MACH_NBC
	mutex_lock(&is->ip->pager_wait_lock);
#endif	MACH_NBC
	is->pager_request = MACH_PORT_NULL;
	is->ip->pager_request = MACH_PORT_NULL;
#if	MACH_NBC
	if (is->ip->waiting) {
		is->ip->waiting = FALSE;
		condition_broadcast(&is->ip->pager_wait);
	}
	mutex_unlock(&is->ip->pager_wait_lock);
#endif	MACH_NBC

	unix_release();

	(void) mach_port_mod_refs(inode_pager_self, pager_request,
				  MACH_PORT_RIGHT_SEND, -urefs);

	(void) mach_port_mod_refs(inode_pager_self, pager_request,
				  MACH_PORT_RIGHT_RECEIVE, -1);

	return KERN_SUCCESS;
}

/*
 *	inode_pager_check_request:
 *
 *	Called by those functions (data_request, data_write, etc)
 *	which are receiving send rights for the pager_request port.
 */

inode_pager_t
inode_pager_check_request(pager, pager_request)
	mach_port_t	pager;
	mach_port_t	pager_request;
{
	register inode_pager_t is;
	mach_port_urefs_t urefs;

	is = inode_pager_lookup(pager);

	urefs = is->urefs;

	assert(is->client_count > 0);
	assert(is->pager_request == pager_request);
	assert(urefs > 0);

	if (++urefs > inode_pager_max_urefs) {
		/*
		 *	Deallocate excess user references.
		 */

		(void) mach_port_mod_refs(inode_pager_self, pager_request,
					  MACH_PORT_RIGHT_SEND, -urefs+1);
		urefs = 1;
	}

	is->urefs = urefs;
	return is;
}

void inode_pager_no_senders(pager, mscount)
	memory_object_t	pager;
	mach_port_mscount_t mscount;
{
	register inode_pager_t	is;
	struct inode *ip;
	mach_port_mscount_t ourcount;

	unix_master();
	is = inode_pager_lookup(pager);

	/*
	 *	We might not have processed the no-senders notification
	 *	in time to keep inode_pager_setup from creating more
	 *	send rights for this memory_object.  Hence, use the mscount
	 *	to determine if we should just register for another
	 *	no-senders notification.
	 */

	if (is->client_count != 0)
		panic("inode_pager_no_senders: client count");

	ourcount = is->mscount;
	if (mscount < ourcount) {
		mach_port_t previous;

		/*
		 *	This should be quite rare.
		 *	We can release the master lock immediately,
		 *	because we don't need to use the inode_pager_t.
		 */

		unix_release();

		if ((mach_port_request_notification(
				inode_pager_self, pager,
				MACH_NOTIFY_NO_SENDERS, ourcount,
				pager, MACH_MSG_TYPE_MAKE_SEND_ONCE,
				&previous) != KERN_SUCCESS) ||
		    (previous != MACH_PORT_NULL))
			panic("inode_pager_no_senders: request_notify");

		return;
	} else if (mscount > ourcount)
		panic("inode_pager_no_senders: mscount too big");

	assert(is->pager_request == MACH_PORT_NULL);
	assert(is->urefs == 0);

	/*
	 *	Release the inode reference and clear
	 *	the pager field.  Do this before destroying the port.
	 */

	ip = is->ip;
	ip->i_flag &= ~ITEXT;
	ip->pager = MEMORY_OBJECT_NULL;
	irele(ip);

	/*
	 *	Remove the memory object port association, and then
	 *	the destroy the port itself.
	 */

	inode_port_hash_delete(is);
	unix_release();

	(void) mach_port_mod_refs(inode_pager_self, pager,
				  MACH_PORT_RIGHT_RECEIVE, -1);

	zfree(inode_pager_zone, (vm_offset_t) is);
}

boolean_t	inode_pager_notify_server(in, out)
	mach_msg_header_t *in, *out;
{
	register mach_no_senders_notification_t *n =
			(mach_no_senders_notification_t *) in;

	/*
	 *	The only send-once rights we create are for
	 *	receiving no-more-senders notifications.
	 *	Hence, if we receive a message directed to
	 *	a send-once right, we can assume it is
	 *	a genuine no-senders notification from the kernel.
	 */

	if ((n->not_header.msgh_bits !=
			MACH_MSGH_BITS(0, MACH_MSG_TYPE_PORT_SEND_ONCE)) ||
	    (n->not_header.msgh_id != MACH_NOTIFY_NO_SENDERS))
		return FALSE;

	assert(n->not_header.msgh_size == sizeof *n);
	assert(n->not_header.msgh_remote_port == MACH_PORT_NULL);
	assert(n->not_header.msgh_kind == MACH_MSGH_KIND_NOTIFICATION);

	assert(n->not_type.msgt_name == MACH_MSG_TYPE_INTEGER_32);
	assert(n->not_type.msgt_size == 32);
	assert(n->not_type.msgt_number == 1);
	assert(n->not_type.msgt_inline);
	assert(! n->not_type.msgt_longform);

	inode_pager_no_senders(n->not_header.msgh_local_port, n->not_count);

	out->msgh_remote_port = MACH_PORT_NULL;
	return TRUE;
}

any_t
inode_pager_server_loop(arg)
	any_t	arg;
{
	mach_msg_return_t	r;
	mach_port_t	my_self;
	vm_offset_t	messages;
	register mach_msg_header_t *in_msg;
	register mach_msg_header_t *out_msg;

	cthread_set_name(cthread_self(), "inode_pager");

	cthread_wire();

	u.u_procp = &proc[0];	/* XXX - see mach_init */

	my_self = mach_task_self();

	if (vm_allocate(my_self, &messages, 2 * 8192, TRUE) != KERN_SUCCESS)
	    panic("inode_pager: can't allocate message buffers");

#if	0
	(void) vm_pageable(my_self, messages, 2 * 8192,
			   VM_PROT_READ|VM_PROT_WRITE);
#endif
	in_msg  = (mach_msg_header_t *)messages;
	out_msg = (mach_msg_header_t *)(messages + 8192);

	for (;;) {
	    r = mach_msg(in_msg, MACH_RCV_MSG,
			 0, 8192, inode_pager_enabled_set,
			 MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
	    if (r != MACH_MSG_SUCCESS) {
		printf("inode_pager: receive failed, %d\n", r);
		continue;
	    }

	    (void) (inode_pager_server(in_msg, out_msg) ||
		    inode_pager_notify_server(in_msg, out_msg));

	    if (MACH_PORT_VALID(out_msg->msgh_remote_port)) {
		(void) mach_msg(out_msg, MACH_SEND_MSG,
				out_msg->msgh_size, 0, MACH_PORT_NULL,
				MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
	    }
	}
}


#define	memory_object_server		inode_pager_server
#include <mach/memory_object_server.c>

void		inode_pager()
{
	kern_return_t kr;

	/*
	 *	Initialize the name port hashing stuff.
	 */

	inode_port_hash_init();

	inode_pager_self = mach_task_self();

	/*
	 *	We are the default pager.
	 *	Initialize the "default pager" port.
	 */

	kr = mach_port_allocate(inode_pager_self, MACH_PORT_RIGHT_RECEIVE,
				&inode_pager_default);
	if (kr != KERN_SUCCESS)
	    panic("inode_pager: can't create default pager port");

	kr = mach_port_allocate(inode_pager_self, MACH_PORT_RIGHT_PORT_SET,
				&inode_pager_enabled_set);
	if (kr != KERN_SUCCESS)
		panic("inode_pager: cannot create enabled port set");

	kr = mach_port_move_member(inode_pager_self,
				   inode_pager_default,
				   inode_pager_enabled_set);
	if (kr != KERN_SUCCESS)
		panic("inode_pager: cannot enable default port");

	/*
	 *	Allocate the buffer for pagein.
	 *	[We wire down the buffer for now.]
	 */

	if (vm_allocate(inode_pager_self, &inode_pager_input_buffer,
			vm_page_size, TRUE) != KERN_SUCCESS)
		panic("inode_pagein: cannot allocate a buffer!");
#if	0
	(void) vm_pageable(inode_pager_self, inode_pager_input_buffer,
			   vm_page_size, VM_PROT_READ|VM_PROT_WRITE);
#endif

	/*
	 * Start a thread for the server loop.
	 */
	{
	    extern void ux_create_thread();

	    ux_create_thread(inode_pager_server_loop);
	}
}


void 	inode_pager_bootstrap()
{
	register vm_size_t	size;
	register int		i;
	/*
	 *	Initialize zone of paging structures.
	 */
	size = (vm_size_t) sizeof(struct inode_pager);
	inode_pager_zone = zinit(size,
				 (vm_size_t)size * ninode * 10,
				 vm_page_size,
				 FALSE,
				 "inode_pager structures");
}
