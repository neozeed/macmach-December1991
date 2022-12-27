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
 * $Log:	default_pager.c,v $
 * Revision 2.17  91/08/29  13:44:27  jsb
 * 	A couple quick changes for NORMA_VM. Will be fixed later.
 * 
 * Revision 2.16  91/08/28  16:59:29  jsb
 * 	Fixed the default values of default_pager_internal_count and
 * 	default_pager_external_count.
 * 	[91/08/28            rpd]
 * 
 * Revision 2.15  91/08/28  11:09:32  jsb
 * 	Added seqnos_memory_object_change_completed.
 * 	From dlb: use memory_object_data_supply for pagein when buffer is
 * 	going to be deallocated.
 * 	From me: don't use data_supply under NORMA_VM (will be fixed).
 * 	[91/08/26  14:30:07  jsb]
 * 
 * 	Changed to process requests in parallel when possible.
 * 
 * 	Don't bother keeping track of mscount.
 * 	[91/08/16            rpd]
 * 	Added default_pager_info.
 * 	[91/08/15            rpd]
 * 
 * 	Added sequence numbers to the memory object interface.
 * 	Changed to use no-senders notifications.
 * 	Changed to keep track of port rights and not use mach_port_destroy.
 * 	Added dummy supply-completed and data-return stubs.
 * 	[91/08/13            rpd]
 * 
 * Revision 2.14  91/05/18  14:28:32  rpd
 * 	Don't give privileges to threads handling external objects.
 * 	[91/04/06            rpd]
 * 	Enhanced to use multiple threads, for performance and to avoid
 * 	a deadlock caused by default_pager_object_create.
 * 	Added locking to partitions.
 * 	Added locking to pager_port_hashtable.
 * 	Changed pager_port_hash to something reasonable.
 * 	[91/04/03            rpd]
 * 
 * Revision 2.13  91/05/14  15:21:41  mrt
 * 	Correcting copyright
 * 
 * Revision 2.12  91/03/16  14:41:26  rpd
 * 	Updated for new kmem_alloc interface.
 * 	Fixed memory_object_create to zero the new pager structure.
 * 	[91/03/03            rpd]
 * 	Removed thread_swappable.
 * 	[91/01/18            rpd]
 * 
 * Revision 2.11  91/02/05  17:00:49  mrt
 * 	Changed to new copyright
 * 	[91/01/28  14:54:31  mrt]
 * 
 * Revision 2.10  90/09/09  14:31:01  rpd
 * 	Use decl_simple_lock_data.
 * 	[90/08/30            rpd]
 * 
 * Revision 2.9  90/08/27  21:44:51  dbg
 * 	Add definitions of NBBY, howmany.
 * 	[90/07/16            dbg]
 * 
 * Revision 2.8  90/06/02  14:45:22  rpd
 * 	Changed default_pager_object_create so the out argument
 * 	is a poly send right.
 * 	[90/05/03            rpd]
 * 	Removed references to keep_wired_memory.
 * 	[90/04/29            rpd]
 * 	Converted to new IPC.
 * 	Removed data-request queue.
 * 	[90/03/26  21:30:57  rpd]
 * 
 * Revision 2.7  90/03/14  21:09:58  rwd
 * 	Call default_pager_object_server and add
 * 	default_pager_object_create
 * 	[90/01/22            rwd]
 * 
 * Revision 2.6  90/01/11  11:41:08  dbg
 * 	Use bootstrap-task print routines.
 * 	[89/12/20            dbg]
 * 
 * 	De-lint.
 * 	[89/12/06            dbg]
 * 
 * Revision 2.5  89/12/08  19:52:03  rwd
 * 	Turn off CHECKSUM
 * 	[89/12/06            rwd]
 * 
 * Revision 2.4  89/10/23  12:01:54  dbg
 * 	Change pager_read_offset and pager_write_offset to return block
 * 	number as function result.  default_read()'s caller must now
 * 	deallocate data if not the same as the data buffer passed in.
 * 	Add register declarations and clean up loops a bit.
 * 	[89/10/19            dbg]
 * 
 * 	Oops - nothing like having your debugging code introduce bugs...
 * 	[89/10/17            dbg]
 * 
 * Revision 2.3  89/10/16  15:21:59  rwd
 * 	debugging: checksum pages in each object.
 * 	[89/10/04            dbg]
 * 
 * Revision 2.2  89/09/08  11:22:06  dbg
 * 	Wait for default_partition to be set.
 * 	[89/09/01            dbg]
 * 
 * 	Modified to call outside routines for read and write.
 * 	Removed disk structure.  Added part_create.
 * 	Reorganized code.
 * 	[89/07/11            dbg]
 * 
 */
/*
 * Default pager.  Pages to paging partition.
 *
 * MUST BE ABLE TO ALLOCATE WIRED-DOWN MEMORY!!!
 */

#include <cpus.h>
#include <norma_vm.h>

#include <mach/boolean.h>
#include <mach/kern_return.h>
#include <mach/memory_object.h>
#include <mach/message.h>
#include <mach/notify.h>
#include <mach/port.h>
#include <mach/time_value.h>
#include <mach/vm_inherit.h>
#include <mach/vm_param.h>
#include <mach/vm_prot.h>

#include <kern/queue.h>
#include <kern/zalloc.h>
#include <kern/kalloc.h>

#include <kern/task.h>
#include <kern/thread.h>
#include <kern/sched_prim.h>

#include <vm/vm_kern.h>

#include <device/device_types.h>
#include <device/device.h>

#include <boot_ufs/boot_printf.h>

#if	0
#define	CHECKSUM	1
#endif

/*
 * 'Partition' structure for each paging area.
 * Controls allocation of blocks within paging area.
 */
struct part {
	decl_simple_lock_data(, p_lock)	/* for bitmap/free */
	vm_size_t	total_size;	/* total number of blocks */
	vm_size_t	free;		/* number of blocks free */
	unsigned char	*bitmap;	/* allocation map */
	int		(*p_read)();	/* Read block from partition */
	int		(*p_write)();	/* Write block to partition */
	char		*p_private;	/* Pointer to private data for
					   read/write routines. */
};
typedef	struct part	*partition_t;

/*
 * Bitmap allocation.
 */
#define	NBBY		8
#define	BYTEMASK	0xff

#define	howmany(a,b)	(((a) + (b) - 1)/(b))

/*
 * Value to indicate no block assigned
 */
#define	NO_BLOCK	((vm_offset_t)-1)

/*
 * Create a partition descriptor.
 * size is in BYTES.
 */
partition_t
part_create(size, p_read, p_write, p_private)
	vm_offset_t	size;		/* size in pages */
	int		(*p_read)();	/* read routine */
	int		(*p_write)();	/* write routine */
	char *		p_private;	/* structure used by read/write */
{
	register partition_t	part;

	size = atop(size);

	part = (partition_t) kalloc(sizeof(struct part));
	simple_lock_init(&part->p_lock);
	part->total_size = size;
	part->free	= size;
	part->bitmap	= (unsigned char *)kalloc(howmany(size, NBBY));
	part->p_read	= p_read;
	part->p_write	= p_write;
	part->p_private	= p_private;

	bzero((char *)part->bitmap, howmany(size, NBBY));

	return (part);
}

/*
 * Allocate a page in a paging file
 */
vm_offset_t
pager_alloc_page(part)
	register partition_t	part;
{
	register int	byte;
	register int	bit;
	register int	limit;

	simple_lock(&part->p_lock);

	if (part->free == 0) {
	    /* out of paging space */

	    simple_unlock(&part->p_lock);
	    return (NO_BLOCK);
	}

	limit = howmany(part->total_size, NBBY);
	for (byte = 0; byte < limit; byte++)
	    if (part->bitmap[byte] != BYTEMASK)
		break;

	if (byte == limit)
	    panic("pager_alloc_page");

	for (bit = 0; bit < NBBY; bit++)
	    if ((part->bitmap[byte] & (1<<bit)) == 0)
		break;
	if (bit == NBBY)
	    panic("pager_alloc_page");

	part->bitmap[byte] |= (1<<bit);
	part->free--;

	simple_unlock(&part->p_lock);
	return (byte*NBBY+bit);
}

/*
 * Deallocate a page in a paging file
 */
void
pager_dealloc_page(part, page)
	register partition_t	part;
	register vm_offset_t	page;
{
	register int	bit, byte;

	if (page >= part->total_size)
	    panic("dealloc_page");

	byte = page / NBBY;
	bit  = page % NBBY;

	simple_lock(&part->p_lock);
	part->bitmap[byte] &= ~(1<<bit);
	part->free++;
	simple_unlock(&part->p_lock);
}

void
no_paging_space()
{
	panic("*** PAGING SPACE EXHAUSTED ***");
}

/*
 * Allocation info for each paging object.
 *
 * Most operations, even pager_write_offset and pager_put_checksum,
 * just need a read lock.  Higher-level considerations prevent
 * conflicting operations on a single page.  The lock really protects
 * the underlying size and block map memory, so pager_extend needs a
 * write lock.
 */
struct dpager {
	lock_data_t	lock;		/* lock for extending block map */
	partition_t	partition;	/* allocation area for pager */
	vm_offset_t	*map;		/* block map */
	vm_size_t	size;		/* size of paging object, in pages */
#ifdef	CHECKSUM
	vm_offset_t	*checksum;	/* checksum - parallel to block map */
#define	NO_CHECKSUM	((vm_offset_t)-1)
#endif	CHECKSUM
};
typedef struct dpager	*dpager_t;

/*
 * A paging object uses either a one- or a two-level map of offsets
 * into the associated paging partition.
 */
#define	PAGEMAP_ENTRIES		128
				/* number of pages in a second-level map */
#define	PAGEMAP_SIZE(npgs)	((npgs)*sizeof(vm_offset_t))

#define	INDIRECT_PAGEMAP_ENTRIES(npgs) \
		((((npgs)-1)/PAGEMAP_ENTRIES) + 1)
#define	INDIRECT_PAGEMAP_SIZE(npgs) \
		(INDIRECT_PAGEMAP_ENTRIES(npgs) * sizeof(vm_offset_t *))
#define	INDIRECT_PAGEMAP(size)	\
		(size > PAGEMAP_ENTRIES)

/*
 * Attach a new paging object to a paging partition
 */
void
pager_alloc(pager, part, size)
	register dpager_t	pager;
	partition_t		part;
	register vm_size_t	size;
{
	register int		i;
	register vm_offset_t *	mapptr;

	lock_init(&pager->lock, TRUE);
	pager->partition = part;

	/*
	 * Convert byte size to number of pages,
	 * then increase to the nearest power of 2.
	 */
	size = atop(size);
	i = 1;
	while (i < size)
	    i <<= 1;
	size = i;

	if (INDIRECT_PAGEMAP(size)) {
	    mapptr = (vm_offset_t *)
				kalloc(INDIRECT_PAGEMAP_SIZE(size));
	    for (i = INDIRECT_PAGEMAP_ENTRIES(size); --i >= 0; )
		mapptr[i] = 0;
	}
	else {
	    mapptr = (vm_offset_t *)kalloc(PAGEMAP_SIZE(size));
	    for (i = 0; i < size; i++)
		mapptr[i] = NO_BLOCK;
	}
	pager->map = mapptr;
	pager->size = size;
#ifdef	CHECKSUM
	if (INDIRECT_PAGEMAP(size)) {
	    mapptr = (vm_offset_t *)
				kalloc(INDIRECT_PAGEMAP_SIZE(size));
	    for (i = INDIRECT_PAGEMAP_ENTRIES(size); --i >= 0; )
		mapptr[i] = 0;
	}
	else {
	    mapptr = (vm_offset_t *)kalloc(PAGEMAP_SIZE(size));
	    for (i = 0; i < size; i++)
		mapptr[i] = NO_CHECKSUM;
	}
	pager->checksum = mapptr;
#endif	CHECKSUM
}

/*
 * Extend the map for a paging object.
 *
 * XXX This implementation can allocate an arbitrary large amount
 * of wired memory when extending a big block map.  Because vm-privileged
 * threads call pager_extend, this can crash the system by exhausting
 * system memory.
 */
void
pager_extend(pager, new_size)
	register dpager_t	pager;
	register vm_size_t	new_size;	/* in pages */
{
	register vm_offset_t *	new_mapptr;
	register vm_offset_t *	old_mapptr;
	register int		i;
	register vm_size_t	old_size;

	lock_write(&pager->lock);
	/*
	 * Double current size until we cover new size.
	 */
	old_size = pager->size;

	i = old_size;
	while (i < new_size)
	    i <<= 1;
	new_size = i;

	if (INDIRECT_PAGEMAP(old_size)) {
	    /*
	     * Pager already uses two levels.  Allocate
	     * a larger indirect block.
	     */
	    new_mapptr = (vm_offset_t *)
			kalloc(INDIRECT_PAGEMAP_SIZE(new_size));
	    old_mapptr = pager->map;
	    for (i = 0; i < INDIRECT_PAGEMAP_ENTRIES(old_size); i++)
		new_mapptr[i] = old_mapptr[i];
	    for (; i < INDIRECT_PAGEMAP_ENTRIES(new_size); i++)
		new_mapptr[i] = 0;
	    kfree((char *)old_mapptr, INDIRECT_PAGEMAP_SIZE(old_size));
	    pager->map = new_mapptr;
	    pager->size = new_size;
#ifdef	CHECKSUM
	    new_mapptr = (vm_offset_t *)
			kalloc(INDIRECT_PAGEMAP_SIZE(new_size));
	    old_mapptr = pager->checksum;
	    for (i = 0; i < INDIRECT_PAGEMAP_ENTRIES(old_size); i++)
		new_mapptr[i] = old_mapptr[i];
	    for (; i < INDIRECT_PAGEMAP_ENTRIES(new_size); i++)
		new_mapptr[i] = 0;
	    kfree((char *)old_mapptr, INDIRECT_PAGEMAP_SIZE(old_size));
	    pager->checksum = new_mapptr;
#endif	CHECKSUM
	    lock_done(&pager->lock);
	    return;
	}

	if (INDIRECT_PAGEMAP(new_size)) {
	    /*
	     * Changing from direct map to indirect map.
	     * Allocate both indirect and direct map blocks,
	     * since second-level (direct) block must be
	     * full size (PAGEMAP_SIZE(PAGEMAP_ENTRIES)).
	     */

	    /*
	     * Allocate new second-level map first.
	     */
	    new_mapptr = (vm_offset_t *)kalloc(PAGEMAP_SIZE(PAGEMAP_ENTRIES));
	    old_mapptr = pager->map;
	    for (i = 0; i < old_size; i++)
		new_mapptr[i] = old_mapptr[i];
	    for (; i < PAGEMAP_ENTRIES; i++)
		new_mapptr[i] = NO_BLOCK;
	    kfree((char *)old_mapptr, PAGEMAP_SIZE(old_size));
	    old_mapptr = new_mapptr;

	    /*
	     * Now allocate indirect map.
	     */
	    new_mapptr = (vm_offset_t *)
			kalloc(INDIRECT_PAGEMAP_SIZE(new_size));
	    new_mapptr[0] = (vm_offset_t) old_mapptr;
	    for (i = 1; i < INDIRECT_PAGEMAP_ENTRIES(new_size); i++)
		new_mapptr[i] = 0;
	    pager->map = new_mapptr;
	    pager->size = new_size;
#ifdef	CHECKSUM
	    /*
	     * Allocate new second-level map first.
	     */
	    new_mapptr = (vm_offset_t *)kalloc(PAGEMAP_SIZE(PAGEMAP_ENTRIES));
	    old_mapptr = pager->checksum;
	    for (i = 0; i < old_size; i++)
		new_mapptr[i] = old_mapptr[i];
	    for (; i < PAGEMAP_ENTRIES; i++)
		new_mapptr[i] = NO_CHECKSUM;
	    kfree((char *)old_mapptr, PAGEMAP_SIZE(old_size));
	    old_mapptr = new_mapptr;

	    /*
	     * Now allocate indirect map.
	     */
	    new_mapptr = (vm_offset_t *)
			kalloc(INDIRECT_PAGEMAP_SIZE(new_size));
	    new_mapptr[0] = (vm_offset_t) old_mapptr;
	    for (i = 1; i < INDIRECT_PAGEMAP_ENTRIES(new_size); i++)
		new_mapptr[i] = 0;
	    pager->checksum = new_mapptr;
#endif	CHECKSUM
	    lock_done(&pager->lock);
	    return;
	}
	/*
	 * Enlarging a direct block.
	 */
	new_mapptr = (vm_offset_t *)
		kalloc(PAGEMAP_SIZE(new_size));
	old_mapptr = pager->map;
	for (i = 0; i < old_size; i++)
	    new_mapptr[i] = old_mapptr[i];
	for (; i < new_size; i++)
	    new_mapptr[i] = NO_BLOCK;
	kfree((char *)old_mapptr, PAGEMAP_SIZE(old_size));
	pager->map = new_mapptr;
	pager->size = new_size;
#ifdef	CHECKSUM
	new_mapptr = (vm_offset_t *)
		kalloc(PAGEMAP_SIZE(new_size));
	old_mapptr = pager->checksum;
	for (i = 0; i < old_size; i++)
	    new_mapptr[i] = old_mapptr[i];
	for (; i < new_size; i++)
	    new_mapptr[i] = NO_CHECKSUM;
	kfree((char *)old_mapptr, PAGEMAP_SIZE(old_size));
	pager->checksum = new_mapptr;
#endif	CHECKSUM
	lock_done(&pager->lock);
}

/*
 * Given an offset within a paging object, find the
 * corresponding block within the paging partition.
 * Return NO_BLOCK if none allocated.
 */
vm_offset_t
pager_read_offset(pager, offset)
	register dpager_t	pager;
	vm_offset_t		offset;
{
	register vm_offset_t	f_page;
	vm_offset_t		pager_offset;

	f_page = atop(offset);

	lock_read(&pager->lock);
	if (f_page >= pager->size)
	    panic("pager_read_offset");

	if (INDIRECT_PAGEMAP(pager->size)) {
	    register vm_offset_t *mapptr;

	    mapptr = (vm_offset_t *)pager->map[f_page/PAGEMAP_ENTRIES];
	    if (mapptr == 0)
		pager_offset = NO_BLOCK;
	    else
		pager_offset = mapptr[f_page%PAGEMAP_ENTRIES];
	}
	else {
	    pager_offset = pager->map[f_page];
	}

	lock_done(&pager->lock);
	return (pager_offset);
}

#ifdef	CHECKSUM
/*
 * Return the checksum for a block.
 */
int
pager_get_checksum(pager, offset)
	register dpager_t	pager;
	vm_offset_t		offset;
{
	register vm_offset_t	f_page;
	int checksum;

	f_page = atop(offset);

	lock_read(&pager->lock);
	if (f_page >= pager->size)
	    panic("pager_get_checksum");

	if (INDIRECT_PAGEMAP(pager->size)) {
	    register vm_offset_t *mapptr;

	    mapptr = (vm_offset_t *)pager->checksum[f_page/PAGEMAP_ENTRIES];
	    if (mapptr == 0)
		checksum = NO_CHECKSUM;
	    else
		checksum = mapptr[f_page%PAGEMAP_ENTRIES];
	}
	else {
	    checksum = pager->checksum[f_page];
	}

	lock_done(&pager->lock);
	return (checksum);
}

/*
 * Remember the checksum for a block.
 */
int
pager_put_checksum(pager, offset, checksum)
	register dpager_t	pager;
	vm_offset_t		offset;
	int			checksum;
{
	register vm_offset_t	f_page;

	f_page = atop(offset);

	lock_read(&pager->lock);
	if (f_page >= pager->size)
	    panic("pager_put_checksum");

	if (INDIRECT_PAGEMAP(pager->size)) {
	    register vm_offset_t *mapptr;

	    mapptr = (vm_offset_t *)pager->checksum[f_page/PAGEMAP_ENTRIES];
	    if (mapptr == 0)
		panic("pager_put_checksum");

	    mapptr[f_page%PAGEMAP_ENTRIES] = checksum;
	}
	else {
	    pager->checksum[f_page] = checksum;
	}
	lock_done(&pager->lock);
}

/*
 * Compute a checksum - XOR each 32-bit word.
 */
int
compute_checksum(addr, size)
	vm_offset_t	addr;
	vm_size_t	size;
{
	register int	checksum = NO_CHECKSUM;
	register int	*ptr;
	register int	count;

	ptr = (int *)addr;
	count = size / sizeof(int);

	while (--count >= 0)
	    checksum ^= *ptr++;

	return (checksum);
}
#endif	CHECKSUM

/*
 * Given an offset within a paging object, find the
 * corresponding block within the paging partition.
 * Allocate a new block if necessary.
 *
 * WARNING: paging objects apparently may be extended
 * without notice!
 */
vm_offset_t
pager_write_offset(pager, offset)
	register dpager_t	pager;
	vm_offset_t		offset;
{
	register vm_offset_t	block, f_page;
	register vm_offset_t	*mapptr;

	f_page = atop(offset);

	lock_read(&pager->lock);
	while (f_page >= pager->size) {
	    /*
	     * Paging object must be extended.
	     * Remember that offset is 0-based, but size is 1-based.
	     */
	    lock_done(&pager->lock);
	    pager_extend(pager, f_page + 1);
	    lock_read(&pager->lock);
	}

	if (INDIRECT_PAGEMAP(pager->size)) {

	    mapptr = (vm_offset_t *)pager->map[f_page/PAGEMAP_ENTRIES];
	    if (mapptr == 0) {
		/*
		 * Allocate the indirect block
		 */
		register int i;

		mapptr = (vm_offset_t *)kalloc(PAGEMAP_SIZE(PAGEMAP_ENTRIES));
		if (mapptr == 0) {
		    lock_done(&pager->lock);
		    /* out of space! */
		    printf("*** PAGER ERROR: out of memory\n");
		    no_paging_space();
		    return (NO_BLOCK);
		}
		pager->map[f_page/PAGEMAP_ENTRIES] = (vm_offset_t)mapptr;
		for (i = 0; i < PAGEMAP_ENTRIES; i++)
		    mapptr[i] = NO_BLOCK;
#ifdef	CHECKSUM
		{
		    register vm_offset_t *cksumptr;
		    register int j;

		    cksumptr = (vm_offset_t *)
				kalloc(PAGEMAP_SIZE(PAGEMAP_ENTRIES));
		    if (cksumptr == 0) {
			lock_done(&pager->lock);
			/* out of space! */
			printf("*** PAGER ERROR: out of memory\n");
			no_paging_space();
			return (FALSE);
		    }
		    pager->checksum[f_page/PAGEMAP_ENTRIES]
			= (vm_offset_t)cksumptr;
		    for (j = 0; j < PAGEMAP_ENTRIES; j++)
			cksumptr[j] = NO_CHECKSUM;
		}
#endif	CHECKSUM
	    }
	    f_page %= PAGEMAP_ENTRIES;
	}
	else {
	    mapptr = pager->map;
	}

	block = mapptr[f_page];
	if (block == NO_BLOCK) {
	    block = pager_alloc_page(pager->partition);
	    if (block == NO_BLOCK) {
		lock_done(&pager->lock);
		no_paging_space();
		return (NO_BLOCK);	/* out of space */
	    }
	    mapptr[f_page] = block;
	}

	lock_done(&pager->lock);
	return (block);
}

/*
 * Deallocate all of the blocks belonging to a paging object.
 * No locking needed because no other operations can be in progress.
 */
void
pager_dealloc(pager)
	register dpager_t	pager;
{
	register int i, j;
	register vm_offset_t	block;
	register vm_offset_t	*mapptr;

	if (INDIRECT_PAGEMAP(pager->size)) {
	    for (i = INDIRECT_PAGEMAP_ENTRIES(pager->size); --i >= 0; ) {
		mapptr = (vm_offset_t *)pager->map[i];
		if (mapptr) {
		    for (j = 0; j < PAGEMAP_ENTRIES; j++) {
			block = mapptr[j];
			if (block != NO_BLOCK)
			    pager_dealloc_page(pager->partition, block);
		    }
		    kfree((char *)mapptr, PAGEMAP_SIZE(PAGEMAP_ENTRIES));
		}
	    }
	    kfree((char *)pager->map, INDIRECT_PAGEMAP_SIZE(pager->size));
#ifdef	CHECKSUM
	    for (i = INDIRECT_PAGEMAP_ENTRIES(pager->size); --i >= 0; ) {
		mapptr = (vm_offset_t *)pager->checksum[i];
		if (mapptr) {
		    kfree((char *)mapptr, PAGEMAP_SIZE(PAGEMAP_ENTRIES));
		}
	    }
	    kfree((char *)pager->checksum,
		  INDIRECT_PAGEMAP_SIZE(pager->size));
#endif	CHECKSUM
	}
	else {
	    mapptr = pager->map;
	    for (i = 0; i < pager->size; i++ ) {
		block = mapptr[i];
		if (block != NO_BLOCK)
		    pager_dealloc_page(pager->partition, block);
	    }
	    kfree((char *)pager->map, PAGEMAP_SIZE(pager->size));
#ifdef	CHECKSUM
	    kfree((char *)pager->checksum, PAGEMAP_SIZE(pager->size));
#endif	CHECKSUM
	}
}
/*

 */

/*
 * Read/write routines.
 */
#define	PAGER_SUCCESS	0
#define	PAGER_ABSENT	1
#define	PAGER_ERROR	2

/*
 * Read data from a default pager.  Addr is the address of a buffer
 * to fill.  Out_addr returns the buffer that contains the data;
 * if it is different from <addr>, it must be deallocated after use.
 */
int
default_read(ds, addr, size, offset, out_addr)
	register dpager_t	ds;
	vm_offset_t		addr;	/* pointer to block to fill */
	register vm_size_t	size;
	register vm_offset_t	offset;
	vm_offset_t		*out_addr;
				/* returns pointer to data */
{
	register vm_offset_t	block;
	vm_offset_t	raddr;
	vm_size_t	rsize;
	register int	rc;
	boolean_t	first_time;
	register partition_t	part = ds->partition;
#ifdef	CHECKSUM
	vm_size_t	original_size = size;
	vm_offset_t	original_offset = offset;
#endif	CHECKSUM
	/*
	 * Find the block in the paging partition
	 */
	block = pager_read_offset(ds, offset);
	if (block == NO_BLOCK)
	    return (PAGER_ABSENT);

	/*
	 * Read it, trying for the entire page.
	 */
	offset = ptoa(block);
	first_time = TRUE;
	*out_addr = addr;

	do {
	    rc = (*part->p_read)(
		part->p_private,
		offset,
		size,
		&raddr,
		&rsize);
	    if (rc != 0)
		return (PAGER_ERROR);

	    /*
	     * If we got the entire page on the first read, return it.
	     */
	    if (first_time && rsize == size) {
		*out_addr = raddr;
		break;
	    }
	    /*
	     * Otherwise, copy the data into the
	     * buffer we were passed, and try for
	     * the next piece.
	     */
	    first_time = FALSE;
	    bcopy((char *)raddr, (char *)addr, rsize);
	    addr += rsize;
	    offset += rsize;
	    size -= rsize;
	} while (size != 0);

#ifdef	CHECKSUM
	{
	    int	write_checksum,
		read_checksum;

	    write_checksum = pager_get_checksum(ds, original_offset);
	    read_checksum = compute_checksum(*out_addr, original_size);
	    if (write_checksum != read_checksum) {
		panic(
  "PAGER CHECKSUM ERROR: offset 0x%x, written 0x%x, read 0x%x",
		    original_offset, write_checksum, read_checksum);
	    }
	}
#endif	CHECKSUM
	return (PAGER_SUCCESS);
}

int
default_write(ds, addr, size, offset)
	register dpager_t	ds;
	register vm_offset_t	addr;
	register vm_size_t	size;
	register vm_offset_t	offset;
{
	vm_offset_t	block;
	vm_size_t	wsize;
	register int	rc;

	/*
	 * Find block in paging partition
	 */
	block = pager_write_offset(ds, offset);
	if (block == NO_BLOCK)
	    return (PAGER_ERROR);

#ifdef	CHECKSUM
	/*
	 * Save checksum
	 */
	{
	    int	checksum;

	    checksum = compute_checksum(addr, size);
	    pager_put_checksum(ds, offset, checksum);
	}
#endif	CHECKSUM
	offset = ptoa(block);

	do {
	    rc = (*ds->partition->p_write)(
		ds->partition->p_private,
		offset,
		addr,
		size,
		&wsize);
	    if (rc != 0) {
		printf("*** PAGER ERROR: default_write: ");
		printf("ds=0x%x addr=0x%x size=0x%x offset=0x%x resid=0x%x\n",
			ds, addr, size, offset, wsize);
		return (PAGER_ERROR);
	    }
	    addr += wsize;
	    offset += wsize;
	    size -= wsize;
	} while (size != 0);
	return (PAGER_SUCCESS);
}

boolean_t
default_has_page(ds, offset)
	dpager_t	ds;
	vm_offset_t	offset;
{
	return (pager_read_offset(ds, offset) != NO_BLOCK);
}

/*

 */

/*
 * Mapping between pager port and paging object.
 */
struct dstruct {
	decl_simple_lock_data(, lock)	/* Lock for the structure */
	boolean_t	waiting_seqno:1,  /* someone waiting on seqno */
			waiting_read:1,	  /* someone waiting on readers */
			waiting_write:1;  /* someone waiting on writers */

	queue_chain_t	links;		/* Link in pager-port hash list */

	memory_object_t	pager;		/* Pager port */
	mach_port_seqno_t seqno;	/* Pager port sequence number */
	mach_port_t	pager_request;	/* Request port */
	mach_port_urefs_t urefs;	/* Request port user-refs */

	unsigned int	readers;	/* Reads in progress */
	unsigned int	writers;	/* Writes in progress */

	unsigned int	errors;		/* Pageout error count */
	struct dpager	dpager;		/* Actual pager */
};
typedef struct dstruct *	default_pager_t;
#define	DEFAULT_PAGER_NULL	((default_pager_t)0)

zone_t	dstruct_zone;

#define	PAGER_PORT_HASH_COUNT	127

struct pager_port {
	decl_simple_lock_data(, lock)
	queue_head_t queue;
} pager_port_hashtable[PAGER_PORT_HASH_COUNT];

/*
 *	Normally the low 8 bits of port names don't carry much
 *	information, but we can't ignore them entirely because
 *	this is implementation-dependent.  We will tend to get
 *	alternating numbers in the high 24 bits, because we
 *	receive two ports per memory_object_create.
 */

#define	pager_port_hash(port) \
	((((port) >> 9) + ((port) & 0x1ff)) % PAGER_PORT_HASH_COUNT)

void pager_port_hash_init()
{
	register int	i;

	for (i = 0; i < PAGER_PORT_HASH_COUNT; i++) {
	    struct pager_port *pp = &pager_port_hashtable[i];

	    simple_lock_init(&pp->lock);
	    queue_init(&pp->queue);
	}
}

void pager_port_hash_insert(port, ds)
	mach_port_t port;
	default_pager_t	ds;
{
	register struct pager_port *pp =
		&pager_port_hashtable[pager_port_hash(port)];

	simple_lock(&pp->lock);
	queue_enter(&pp->queue, ds, default_pager_t, links);
	simple_unlock(&pp->lock);
}

default_pager_t pager_port_lookup(port)
	mach_port_t port;
{
	register struct pager_port *pp;
	register default_pager_t entry;

	pp = &pager_port_hashtable[pager_port_hash(port)];

	simple_lock(&pp->lock);

	queue_iterate(&pp->queue, entry, default_pager_t, links)
	    if (entry->pager == port) {
		simple_unlock(&pp->lock);
		return (entry);
	    }

	simple_unlock(&pp->lock);
	return (DEFAULT_PAGER_NULL);
}

void pager_port_hash_delete(ds)
	default_pager_t ds;
{
	register struct pager_port *pp;
	register default_pager_t entry;

	pp = &pager_port_hashtable[pager_port_hash(ds->pager)];

	simple_lock(&pp->lock);
	queue_remove(&pp->queue, ds, default_pager_t, links);
	simple_unlock(&pp->lock);
}

/*
 *	We use the sequence numbers on requests to regulate
 *	our parallelism.  In general, we allow multiple reads and writes
 *	to proceed in parallel, with the exception that reads must
 *	wait for previous writes to finish.  (Because the kernel might
 *	generate a data-request for a page on the heels of a data-write
 *	for the same page, and we must avoid returning stale data.)
 *	terminate requests wait for proceeding reads and writes to finish.
 */

unsigned int default_pager_total = 0;		/* debugging */
unsigned int default_pager_wait_seqno = 0;	/* debugging */
unsigned int default_pager_wait_read = 0;	/* debugging */
unsigned int default_pager_wait_write = 0;	/* debugging */

void pager_port_lock(ds, seqno)
	default_pager_t ds;
	mach_port_seqno_t seqno;
{
	default_pager_total++;
	simple_lock(&ds->lock);
	while (ds->seqno != seqno) {
		default_pager_wait_seqno++;
		ds->waiting_seqno = TRUE;
		thread_sleep((int) &ds->seqno,
			     simple_lock_addr(ds->lock),
			     FALSE);
		simple_lock(&ds->lock);
	}
	simple_unlock(&ds->lock);
}

void pager_port_unlock(ds)
	default_pager_t ds;
{
	simple_lock(&ds->lock);
	ds->seqno++;
	if (ds->waiting_seqno) {
		thread_wakeup((int) &ds->seqno);
		ds->waiting_seqno = FALSE;
	}
	simple_unlock(&ds->lock);
}

void pager_port_start_read(ds)
	default_pager_t ds;
{
	simple_lock(&ds->lock);
	ds->readers++;
	simple_unlock(&ds->lock);
}

void pager_port_wait_for_readers(ds)
	default_pager_t ds;
{
	simple_lock(&ds->lock);
	while (ds->readers != 0) {
		default_pager_wait_read++;
		ds->waiting_read = TRUE;
		thread_sleep((int) &ds->readers,
			     simple_lock_addr(ds->lock),
			     FALSE);
		simple_lock(&ds->lock);
	}
	simple_unlock(&ds->lock);
}

void pager_port_finish_read(ds)
	default_pager_t ds;
{
	simple_lock(&ds->lock);
	if ((--ds->readers == 0) && ds->waiting_read) {
		thread_wakeup((int) &ds->readers);
		ds->waiting_read = FALSE;
	}
	simple_unlock(&ds->lock);
}

void pager_port_start_write(ds)
	default_pager_t ds;
{
	simple_lock(&ds->lock);
	ds->writers++;
	simple_unlock(&ds->lock);
}

void pager_port_wait_for_writers(ds)
	default_pager_t ds;
{
	simple_lock(&ds->lock);
	while (ds->writers != 0) {
		default_pager_wait_write++;
		ds->waiting_write = TRUE;
		thread_sleep((int) &ds->writers,
			     simple_lock_addr(ds->lock),
			     FALSE);
		simple_lock(&ds->lock);
	}
	simple_unlock(&ds->lock);
}

void pager_port_finish_write(ds)
	default_pager_t ds;
{
	simple_lock(&ds->lock);
	if ((--ds->writers == 0) && ds->waiting_write) {
		thread_wakeup((int) &ds->writers);
		ds->waiting_write = FALSE;
	}
	simple_unlock(&ds->lock);
}

/*
 *	Default pager.
 *	Wired into kernel.
 */
partition_t	default_partition = 0;	/* Where to allocate new pagers */
decl_simple_lock_data(,
		default_partition_lock);
					/* lock to set it */

extern task_t bootstrap_task;		/* Our task. */
mach_port_t default_pager_self;		/* Our task port. */

mach_port_t default_pager_default_port;	/* Port for memory_object_create. */
thread_t default_pager_default_thread; /* Thread for default_port. */

mach_port_t default_pager_internal_set;	/* Port set for internal objects. */
mach_port_t default_pager_external_set;	/* Port set for external objects. */

typedef struct default_pager_thread {
	thread_t dpt_thread;		/* Server thread. */
	vm_offset_t dpt_buffer;		/* Read buffer. */
	boolean_t dpt_internal;		/* Do we handle internal objects? */
} default_pager_thread_t;


/* Random computation to get more parallelism on multiprocessors. */
#if	NCPUS > 32
#define DEFAULT_PAGER_INTERNAL_COUNT	(32/4 + 3)
#else
#define DEFAULT_PAGER_INTERNAL_COUNT	(NCPUS/4 + 3)
#endif

/* Memory created by default_pager_object_create should mostly be resident. */
#define DEFAULT_PAGER_EXTERNAL_COUNT	(1)

unsigned int default_pager_internal_count = DEFAULT_PAGER_INTERNAL_COUNT;
					/* Number of "internal" threads. */
unsigned int default_pager_external_count = DEFAULT_PAGER_EXTERNAL_COUNT;
					/* Number of "external" threads. */

default_pager_t pager_port_alloc(size)
	vm_size_t size;
{
	default_pager_t ds;

	ds = (default_pager_t) zalloc(dstruct_zone);
	if (ds == DEFAULT_PAGER_NULL)
	    panic("pager_port_alloc");
	bzero((char *) ds, sizeof *ds);

	simple_lock_init(&ds->lock);
	pager_alloc(&ds->dpager, default_partition, size);

	return ds;
}

/*
 *	Make all calls involving the kernel interface go through IPC.
 */

#include <mach/mach_user_internal.h>
#include <mach/mach_port_internal.h>

/*
 *	Rename all of the functions in the pager interface,
 *	to avoid confusing them with the kernel interface
 */

#define	seqnos_memory_object_init		default_pager_init
#define	seqnos_memory_object_terminate		default_pager_terminate
#define	seqnos_memory_object_data_request	default_pager_data_request
#define	seqnos_memory_object_data_unlock	default_pager_data_unlock
#define	seqnos_memory_object_data_write		default_pager_data_write
#define	seqnos_memory_object_data_initialize	default_pager_data_initialize
#define	seqnos_memory_object_create		default_pager_create
#define	seqnos_memory_object_copy		default_pager_copy
#define	seqnos_memory_object_lock_completed	default_pager_lock_completed
#define seqnos_memory_object_supply_completed	default_pager_supply_completed
#define seqnos_memory_object_data_return	default_pager_data_return
#define seqnos_memory_object_change_completed	default_pager_change_completed

mach_port_urefs_t default_pager_max_urefs = 10000;

void pager_port_check_request(ds, pager_request)
	default_pager_t ds;
	mach_port_t pager_request;
{
	mach_port_delta_t delta;
	kern_return_t kr;

	simple_lock(&ds->lock);

#if	NORMA_VM
	if (ds->pager_request == MACH_PORT_NULL) {
		ds->pager_request = pager_request;
	};
#endif	/* NORMA_VM */

	assert(ds->pager_request == pager_request);

	if (++ds->urefs > default_pager_max_urefs) {
		delta = 1 - ds->urefs;
		ds->urefs = 1;
	} else
		delta = 0;
	simple_unlock(&ds->lock);

	if (delta != 0) {
		/*
		 *	Deallocate excess user references.
		 */

		kr = mach_port_mod_refs(default_pager_self, pager_request,
					MACH_PORT_RIGHT_SEND, delta);
		if (kr != KERN_SUCCESS)
			panic("pager_port_check_request");
	}
}

void default_pager_add(ds, internal)
	default_pager_t ds;
	boolean_t internal;
{
	mach_port_t pager = ds->pager;
	mach_port_t pset;
	mach_port_mscount_t sync;
	mach_port_t previous;
	kern_return_t kr;

	/*
	 *	The port currently has a make-send count of zero,
	 *	because either we just created the port or we just
	 *	received the port in a memory_object_create request.
	 */

	if (internal) {
		/* possibly generate an immediate no-senders notification */
		sync = 0;
		pset = default_pager_internal_set;
	} else {
		/* delay notification till send right is created */
		sync = 1;
		pset = default_pager_external_set;
	}

	kr = mach_port_request_notification(default_pager_self, pager,
				MACH_NOTIFY_NO_SENDERS, sync,
				pager, MACH_MSG_TYPE_MAKE_SEND_ONCE,
				&previous);
	if ((kr != KERN_SUCCESS) || (previous != MACH_PORT_NULL))
		panic("default_pager_add");

	kr = mach_port_move_member(default_pager_self, pager, pset);
	if (kr != KERN_SUCCESS)
		panic("default_pager_add");
}

/*
 *	Routine:	memory_object_create
 *	Purpose:
 *		Handle requests for memory objects from the
 *		kernel.
 *	Notes:
 *		Because we only give out the default memory
 *		manager port to the kernel, we don't have to
 *		be so paranoid about the contents.
 */
kern_return_t
seqnos_memory_object_create(old_pager, seqno, new_pager, new_size,
			    new_pager_request, new_pager_name, new_page_size)
	mach_port_t	old_pager;
	mach_port_seqno_t seqno;
	mach_port_t	new_pager;
	vm_size_t	new_size;
	mach_port_t	new_pager_request;
	mach_port_t	new_pager_name;
	vm_size_t	new_page_size;
{
	register default_pager_t	ds;
	kern_return_t kr;

	assert(old_pager == default_pager_default_port);
#if	NORMA_VM
#else	/* NORMA_VM */
	assert(MACH_PORT_VALID(new_pager_request));
#endif	/* NORMA_VM */
	assert(new_pager_name == MACH_PORT_NULL);
	assert(new_page_size == PAGE_SIZE);

	ds = pager_port_alloc(new_size);

	/*
	 *	Set up associations between these ports
	 *	and this default_pager structure
	 */

	ds->pager = new_pager;
	ds->pager_request = new_pager_request;
	ds->urefs = 1; /* for the send right we just received */
	pager_port_hash_insert(new_pager, ds);

	/*
	 *	After this, other threads might start receiving requests
	 *	for this memory object.
	 */

	default_pager_add(ds, TRUE);

	return(KERN_SUCCESS);
}

memory_object_copy_strategy_t default_pager_copy_strategy =
					MEMORY_OBJECT_COPY_DELAY;

kern_return_t
seqnos_memory_object_init(pager, seqno, pager_request, pager_name,
			  pager_page_size)
	mach_port_t	pager;
	mach_port_seqno_t seqno;
	mach_port_t	pager_request;
	mach_port_t	pager_name;
	vm_size_t	pager_page_size;
{
	register default_pager_t	ds;
	kern_return_t	kr;

	assert(MACH_PORT_VALID(pager_request));
	assert(MACH_PORT_VALID(pager_name));
	assert(pager_page_size == PAGE_SIZE);

	ds = pager_port_lookup(pager);
	if (ds == DEFAULT_PAGER_NULL)
	    panic("(default_pager)init");
	pager_port_lock(ds, seqno);

	if (ds->pager_request != MACH_PORT_NULL)
	    panic("(default_pager)init");

	ds->pager_request = pager_request;
	ds->urefs = 1; /* for the send right we just received */

	/*
	 *	Even if the kernel immediately terminates the object,
	 *	the pager_request port won't be destroyed until
	 *	we process the terminate request, which won't happen
	 *	until we unlock the object.
	 */

	kr = memory_object_set_attributes(pager_request,
					  TRUE,
					  FALSE,	/* do not cache */
					  default_pager_copy_strategy);
	if (kr != KERN_SUCCESS)
	    panic("(default_pager)init");

	pager_port_unlock(ds);

	/*
	 *	We have no use for the pager_name port.
	 */

	kr = mach_port_deallocate(mach_task_self(), pager_name);
	if (kr != KERN_SUCCESS)
	    panic("(default_pager)init");

	return(KERN_SUCCESS);
}

kern_return_t
seqnos_memory_object_terminate(pager, seqno, pager_request, pager_name)
	mach_port_t	pager;
	mach_port_seqno_t seqno;
	mach_port_t	pager_request;
	mach_port_t	pager_name;
{
	register default_pager_t	ds;
	mach_port_urefs_t urefs;
	kern_return_t kr;

	/*
	 *	pager_request and pager_name are receive rights,
	 *	not send rights.
	 */

	ds = pager_port_lookup(pager);
	if (ds == DEFAULT_PAGER_NULL)
		panic("(default_pager)terminate");
	pager_port_lock(ds, seqno);

	/*
	 *	Wait for read and write requests to terminate.
	 */

	pager_port_wait_for_readers(ds);
	pager_port_wait_for_writers(ds);

	/*
	 *	After memory_object_terminate both memory_object_init
	 *	and a no-senders notification are possible.
	 */

	ds->pager_request = MACH_PORT_NULL;
	urefs = ds->urefs;
	ds->urefs = 0;
	pager_port_unlock(ds);

	/*
	 *	Now we deallocate our various port rights.
	 */

	kr = mach_port_mod_refs(default_pager_self, pager_request,
				MACH_PORT_RIGHT_SEND, -urefs);
#if	NORMA_VM
	if (kr != KERN_SUCCESS && kr != KERN_INVALID_VALUE)
	    panic("(default_pager)terminate");
#else	/* NORMA_VM */
	if (kr != KERN_SUCCESS)
	    panic("(default_pager)terminate");
#endif	/* NORMA_VM */

	kr = mach_port_mod_refs(default_pager_self, pager_request,
				MACH_PORT_RIGHT_RECEIVE, -1);
	if (kr != KERN_SUCCESS)
	    panic("(default_pager)terminate");

	if (MACH_PORT_VALID(pager_name)) {
	    kr = mach_port_mod_refs(default_pager_self, pager_name,
				    MACH_PORT_RIGHT_RECEIVE, -1);
	    if (kr != KERN_SUCCESS)
		panic("(default_pager)terminate");
	}

	return (KERN_SUCCESS);
}

void default_pager_no_senders(pager, seqno, mscount)
	memory_object_t pager;
	mach_port_seqno_t seqno;
	mach_port_mscount_t mscount;
{
	register default_pager_t ds;
	kern_return_t kr;

	/*
	 *	Because we don't give out multiple send rights
	 *	for a memory object, there can't be a race
	 *	between getting a no-senders notification
	 *	and creating a new send right for the object.
	 *	Hence we don't keep track of mscount.
	 */

	ds = pager_port_lookup(pager);
	if (ds == DEFAULT_PAGER_NULL)
		panic("(default_pager)no_senders");
	pager_port_lock(ds, seqno);

	/*
	 *	We shouldn't get a no-senders notification
	 *	when the kernel has the object cached.
	 */

	if (ds->pager_request != MACH_PORT_NULL)
		panic("(default_pager)no_senders");

	/*
	 *	Remove the memory object port association, and then
	 *	the destroy the port itself.
	 */

	pager_port_hash_delete(ds);
	pager_dealloc(&ds->dpager);
	zfree(dstruct_zone, (vm_offset_t) ds);

	kr = mach_port_mod_refs(default_pager_self, pager,
				MACH_PORT_RIGHT_RECEIVE, -1);
	if (kr != KERN_SUCCESS)
		panic("(default_pager)no_senders");
}

int		default_pager_pagein_count = 0;
int		default_pager_pageout_count = 0;

kern_return_t
seqnos_memory_object_data_request(pager, seqno, reply_to, offset,
				  length, protection_required)
	memory_object_t	pager;
	mach_port_seqno_t seqno;
	mach_port_t	reply_to;
	vm_offset_t	offset;
	vm_size_t	length;
	vm_prot_t	protection_required;
{
	default_pager_thread_t *dpt;
	default_pager_t	ds;
	vm_offset_t addr;
	unsigned int errors;
	kern_return_t rc;

	dpt = (default_pager_thread_t *) current_thread()->ith_other;
	assert(current_thread() == dpt->dpt_thread);

	if (length != PAGE_SIZE)
	    panic("(default_pager)data_request");

	ds = pager_port_lookup(pager);
	if (ds == DEFAULT_PAGER_NULL)
	    panic("(default_pager)data_request");
	pager_port_lock(ds, seqno);
	pager_port_check_request(ds, reply_to);
	pager_port_wait_for_writers(ds);
	pager_port_start_read(ds);
	pager_port_unlock(ds);

	simple_lock(&ds->lock);
	errors = ds->errors;
	simple_unlock(&ds->lock);

	if (errors) {
	    printf("(default_pager)data_request: dropping request because of");
	    printf(" previous paging errors\n");
	    (void) memory_object_data_error(reply_to,
				offset, PAGE_SIZE,
				KERN_FAILURE);
	    goto done;
	}

	rc = default_read(&ds->dpager, dpt->dpt_buffer,
			  PAGE_SIZE, offset,
			  &addr);
	switch (rc) {
	    case PAGER_SUCCESS:
#if	NORMA_VM
		(void) memory_object_data_provided(
			reply_to, offset,
			addr, PAGE_SIZE,
			VM_PROT_NONE);
		if (addr != dpt->dpt_buffer)
		    (void) vm_deallocate(default_pager_self, addr, PAGE_SIZE);
#else	/* NORMA_VM */
		if (addr != dpt->dpt_buffer) {
		    /*
		     *	Deallocates data buffer
		     */
		    (void) memory_object_data_supply(
		        reply_to, offset,
			addr, PAGE_SIZE, TRUE,
			VM_PROT_NONE,
			FALSE, MACH_PORT_NULL);
		} else {
		    (void) memory_object_data_provided(
			reply_to, offset,
			addr, PAGE_SIZE,
			VM_PROT_NONE);
		}
#endif	/* NORMA_VM */
		break;

	    case PAGER_ABSENT:
		(void) memory_object_data_unavailable(
			reply_to,
			offset,
			PAGE_SIZE);
		break;

	    case PAGER_ERROR:
		(void) memory_object_data_error(
			reply_to,
			offset,
			PAGE_SIZE,
			KERN_FAILURE);
		break;
	}

	default_pager_pagein_count++;

    done:
	pager_port_finish_read(ds);
	return(KERN_SUCCESS);
}

/*
 * memory_object_data_initialize: check whether we already have each page, and
 * write it if we do not.  The implementation is far from optimized, and
 * also assumes that the default_pager is single-threaded.
 */
kern_return_t
seqnos_memory_object_data_initialize(pager, seqno, pager_request,
				     offset, addr, data_cnt)
	memory_object_t	pager;
	mach_port_seqno_t seqno;
	mach_port_t	pager_request;
	register
	vm_offset_t	offset;
	register
	pointer_t	addr;
	vm_size_t	data_cnt;
{
	vm_offset_t	amount_sent;
	default_pager_t	ds;

#ifdef	lint
	pager_request++;
#endif	lint

	ds = pager_port_lookup(pager);
	if (ds == DEFAULT_PAGER_NULL)
	    panic("(default_pager)data_intialize");
	pager_port_lock(ds, seqno);
	pager_port_check_request(ds, pager_request);
	pager_port_start_write(ds);
	pager_port_unlock(ds);

	for (amount_sent = 0;
	     amount_sent < data_cnt;
	     amount_sent += PAGE_SIZE) {

	     if (!default_has_page(&ds->dpager, offset + amount_sent)) {
		if (default_write(&ds->dpager,
				  addr + amount_sent,
				  PAGE_SIZE,
				  offset + amount_sent)
			 != PAGER_SUCCESS) {
		    printf("(default_pager)data_initialize: write error\n");
		    simple_lock(&ds->lock);
		    ds->errors++;
		    simple_unlock(&ds->lock);
		}
	     }
	}

	pager_port_finish_write(ds);
	if (vm_deallocate(default_pager_self, addr, data_cnt) != KERN_SUCCESS)
	    panic("default_initialize: deallocate failed");
	return(KERN_SUCCESS);
}

/*
 * memory_object_data_write: split up the stuff coming in from
 * a memory_object_data_write call
 * into individual pages and pass them off to default_write.
 */
kern_return_t
seqnos_memory_object_data_write(pager, seqno, pager_request,
				offset, addr, data_cnt)
	memory_object_t	pager;
	mach_port_seqno_t seqno;
	mach_port_t	pager_request;
	register
	vm_offset_t	offset;
	register
	pointer_t	addr;
	vm_size_t	data_cnt;
{
	register
	vm_size_t	amount_sent;
	default_pager_t	ds;

#ifdef	lint
	pager_request++;
#endif	lint

	if ((data_cnt % PAGE_SIZE) != 0)
	    panic("(default_pager)data_write");

	ds = pager_port_lookup(pager);
	if (ds == DEFAULT_PAGER_NULL)
	    panic("(default_pager)data_write");
	pager_port_lock(ds, seqno);
	pager_port_check_request(ds, pager_request);
	pager_port_start_write(ds);
	pager_port_unlock(ds);

	for (amount_sent = 0;
	     amount_sent < data_cnt;
	     amount_sent += PAGE_SIZE) {

	    register int result;

	    result = default_write(&ds->dpager,
			      addr + amount_sent,
			      PAGE_SIZE,
			      offset + amount_sent);
	    if (result != KERN_SUCCESS) {
		printf("*** WRITE ERROR on default_pageout:");
		printf(" pager=%d, offset=0x%x, length=0x%x, result=%d\n",
			pager, offset+amount_sent, PAGE_SIZE, result);
		simple_lock(&ds->lock);
		ds->errors++;
		simple_unlock(&ds->lock);
	    }
	    default_pager_pageout_count++;
	}

	pager_port_finish_write(ds);
	if (vm_deallocate(default_pager_self, addr, data_cnt) != KERN_SUCCESS)
		panic("default_data_write: deallocate failed");

	return(KERN_SUCCESS);
}

/*ARGSUSED*/
kern_return_t
seqnos_memory_object_copy(old_memory_object, seqno, old_memory_control,
			  offset, length, new_memory_object)
	memory_object_t	old_memory_object;
	mach_port_seqno_t seqno;
	memory_object_control_t
			old_memory_control;
	vm_offset_t	offset;
	vm_size_t	length;
	memory_object_t	new_memory_object;
{
	panic("(default_pager)copy");
	return KERN_FAILURE;
}

kern_return_t
seqnos_memory_object_lock_completed(pager, seqno, pager_request,
				    offset, length)
	memory_object_t	pager;
	mach_port_seqno_t seqno;
	mach_port_t	pager_request;
	vm_offset_t	offset;
	vm_size_t	length;
{
#ifdef	lint
	pager++; seqno++; pager_request++; offset++; length++;
#endif	lint

	panic("(default_pager)lock_completed");
	return(KERN_FAILURE);
}

kern_return_t
seqnos_memory_object_data_unlock(pager, seqno, pager_request,
				 offset, addr, data_cnt)
	memory_object_t	pager;
	mach_port_seqno_t seqno;
	mach_port_t	pager_request;
	vm_offset_t	offset;
	pointer_t	addr;
	vm_size_t	data_cnt;
{
	panic("(default_pager)data_unlock");
	return(KERN_FAILURE);
}

kern_return_t
seqnos_memory_object_supply_completed(pager, seqno, pager_request,
				      offset, length,
				      result, error_offset)
	memory_object_t	pager;
	mach_port_seqno_t seqno;
	mach_port_t	pager_request;
	vm_offset_t	offset;
	vm_size_t	length;
	kern_return_t	result;
	vm_offset_t	error_offset;
{
	panic("(default_pager)supply_completed");
	return(KERN_FAILURE);
}

kern_return_t
seqnos_memory_object_data_return(pager, seqno, pager_request,
				 offset, addr, data_cnt,
				 dirty, kernel_copy)
	memory_object_t	pager;
	mach_port_seqno_t seqno;
	mach_port_t	pager_request;
	vm_offset_t	offset;
	pointer_t	addr;
	vm_size_t	data_cnt;
	boolean_t	dirty;
	boolean_t	kernel_copy;
{
	panic("(default_pager)data_return");
	return(KERN_FAILURE);
}

kern_return_t
seqnos_memory_object_change_completed(pager, seqno, may_cache, copy_strategy)
	memory_object_t	pager;
	mach_port_seqno_t seqno;
	boolean_t	may_cache;
	memory_object_copy_strategy_t copy_strategy;
{
	panic("(default_pager)change_completed");
	return(KERN_FAILURE);
}

/*
 *	Include the server loop
 */

boolean_t default_pager_notify_server(in, out)
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

	assert(n->not_type.msgt_name == MACH_MSG_TYPE_INTEGER_32);
	assert(n->not_type.msgt_size == 32);
	assert(n->not_type.msgt_number == 1);
	assert(n->not_type.msgt_inline);
	assert(! n->not_type.msgt_longform);

	default_pager_no_senders(n->not_header.msgh_local_port,
				 n->not_header.msgh_seqno, n->not_count);

	out->msgh_remote_port = MACH_PORT_NULL;
	return TRUE;
}

#define	SERVER_LOOP		default_pager_server_loop
#define	SERVER_NAME		"default_pager"
#define	SERVER_DISPATCH(in,out)	\
		(default_pager_server(in, out) || \
		 default_pager_default_server(in, out) || \
		 default_pager_object_server(in, out) || \
		 default_pager_notify_server(in, out))

#include <kern/server_loop.c>

#define	seqnos_memory_object_server		default_pager_server
#include <mach/memory_object_server.c>
#define	seqnos_memory_object_default_server	default_pager_default_server
#include <mach/memory_object_default_server.c>
#include <mach/default_pager_object_server.c>

int	dstruct_max	= 1000;		/* sb same as number of vm_objects */

/*
 *	We use multiple threads, for two reasons.
 *
 *	First, memory objects created by default_pager_object_create
 *	are "external", instead of "internal".  This means the kernel
 *	sends data (memory_object_data_write) to the object pageable.
 *	To prevent deadlocks, the external and internal objects must
 *	be managed by different threads.
 *
 *	Second, the default pager uses synchronous IO operations.
 *	Some write operations require read operations on indirect blocks.
 *	Spreading requests across multiple threads should
 *	recover some of the performance loss from synchronous IO.
 *
 *	We have 3+ threads.
 *	One receives memory_object_create and
 *	default_pager_object_create requests.
 *	One or more manage internal objects.
 *	One or more manage external objects.
 */

void
default_pager_thread_privileges()
{
	/*
	 *	Set thread privileges.  We don't need stack privileges,
	 *	because default pager threads never block
	 *	with an explicit continuation.
	 */

	current_thread()->vm_privilege = TRUE;
}

mach_msg_size_t default_pager_msg_size = 128;

void
default_pager_thread()
{
	default_pager_thread_t *dpt;
	mach_port_t pset;

	dpt = (default_pager_thread_t *) current_thread()->ith_other;
	assert(current_thread() == dpt->dpt_thread);

	/*
	 *	Threads handling external objects can't have
	 *	privileges.  Otherwise a burst of data-requests for an
	 *	external object could empty the free-page queue,
	 *	because the fault code only reserves real pages for
	 *	requests sent to internal objects.
	 */

	if (dpt->dpt_internal) {
		default_pager_thread_privileges();
		pset = default_pager_internal_set;
	} else {
		pset = default_pager_external_set;
	}

	SERVER_LOOP(pset, default_pager_msg_size);
}

void
start_default_pager_thread(internal)
	boolean_t internal;
{
	default_pager_thread_t *dpt;
	kern_return_t kr;

	dpt = (default_pager_thread_t *) kalloc(sizeof *dpt);
	if (dpt == 0)
		panic("default pager");

	dpt->dpt_internal = internal;

	kr = kmem_alloc_wired(kernel_map, &dpt->dpt_buffer, PAGE_SIZE);
	if (kr != KERN_SUCCESS)
		panic("default pager");

	/*
	 *	kernel_thread will stick dpt in the new thread's
	 *	ith_other field.  Because the default pager threads
	 *	never block with a continuation, that field won't
	 *	get stepped on.
	 */

	dpt->dpt_thread = kernel_thread(bootstrap_task,
					default_pager_thread,
					(char *) dpt);
}

void
default_pager(host_port)
	mach_port_t host_port;
{
	kern_return_t kr;
	int i;

	default_pager_thread_privileges();

	/*
	 * Wait for at least the default paging partition to be
	 * set up.
	 */
	simple_lock(&default_partition_lock);
	while (default_partition == (partition_t)0) {
	    thread_sleep((int)&default_partition,
			 simple_lock_addr(default_partition_lock),
			 FALSE);
	    simple_lock(&default_partition_lock);
	}
	simple_unlock(&default_partition_lock);

	default_pager_self = mach_task_self();

	/*
	 *	Initialize the name port hashing stuff.
	 */
	pager_port_hash_init();

	dstruct_zone = zinit(sizeof(struct dstruct),
			(vm_size_t) (sizeof(struct dstruct) * dstruct_max),
			sizeof(struct dstruct),
			FALSE,
			"default pager structures");

	/*
	 *	This thread will receive memory_object_create
	 *	requests from the kernel and default_pager_object_create
	 *	requests from the user via default_pager_default_port.
	 */

	default_pager_default_thread = current_thread();

	kr = mach_port_allocate(default_pager_self, MACH_PORT_RIGHT_RECEIVE,
				&default_pager_default_port);
	if (kr != KERN_SUCCESS)
		panic("default pager");

	kr = mach_port_allocate(default_pager_self, MACH_PORT_RIGHT_PORT_SET,
				&default_pager_internal_set);
	if (kr != KERN_SUCCESS)
		panic("default pager");

	kr = mach_port_allocate(default_pager_self, MACH_PORT_RIGHT_PORT_SET,
				&default_pager_external_set);
	if (kr != KERN_SUCCESS)
		panic("default pager");

    {
	memory_object_t DMM = default_pager_default_port;
	kr = vm_set_default_memory_manager(host_port, &DMM);
	if ((kr != KERN_SUCCESS) || (DMM != MACH_PORT_NULL))
		panic("default pager");
    }

	/*
	 *	Now we create the threads that will actually
	 *	manage objects.
	 */

	for (i = 0; i < default_pager_internal_count; i++)
		start_default_pager_thread(TRUE);

	for (i = 0; i < default_pager_external_count; i++)
		start_default_pager_thread(FALSE);

	SERVER_LOOP(default_pager_default_port, default_pager_msg_size);
}

/*
 * Set up the default paging partition.
 */
void
create_default_partition(size, p_read, p_write, p_private)
	vm_size_t	size;
	int		(*p_read)();
	int		(*p_write)();
	char		*p_private;
{
	partition_t	part;

	part = part_create(size, p_read, p_write, p_private);
	simple_lock(&default_partition_lock);
	default_partition = part;
	thread_wakeup((int)&default_partition);
	simple_unlock(&default_partition_lock);
}

kern_return_t default_pager_object_create(pager, mem_obj, type, size)
	mach_port_t pager;
	mach_port_t *mem_obj;
	mach_msg_type_name_t *type;
	vm_size_t size;
{
	default_pager_t ds;
	mach_port_t port;
	kern_return_t result;

	result = mach_port_allocate(default_pager_self,
				    MACH_PORT_RIGHT_RECEIVE, &port);
	if (result != KERN_SUCCESS) return (result);

	ds = pager_port_alloc(size);

	/*
	 *	Set up associations between these ports
	 *	and this default_pager structure
	 */

	ds->pager = port;
	pager_port_hash_insert(port, ds);
	default_pager_add(ds, FALSE);

	*mem_obj = port;
	*type = MACH_MSG_TYPE_MAKE_SEND;
	return (KERN_SUCCESS);
}

kern_return_t default_pager_info(pager, totalp, freep)
	mach_port_t pager;
	vm_size_t *totalp, *freep;
{
	register partition_t part;
	vm_size_t total, free;

	simple_lock(&default_partition_lock);
	part = default_partition;
	simple_unlock(&default_partition_lock);

	if (part != 0) {
		total = ptoa(part->total_size);
		simple_lock(&part->p_lock);
		free = ptoa(part->free);
		simple_unlock(&part->p_lock);
	} else {
		total = 0;
		free = 0;
	}

	*totalp = total;
	*freep = free;
	return KERN_SUCCESS;
}
