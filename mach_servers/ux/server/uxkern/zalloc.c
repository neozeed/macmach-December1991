/* 
 * Mach Operating System
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */

/*
 *	File:	kern/zalloc.c
 *	Author:	Avadis Tevanian, Jr.
 *
 *	Zone-based memory allocator.  A zone is a collection of fixed size
 *	data blocks for which quick allocation/deallocation is possible.
 *
 * HISTORY
 * $Log:	zalloc.c,v $
 * Revision 2.3  91/08/12  22:38:33  rvb
 * 	Removed calls to vm_pageable.
 * 	[91/08/06            rpd]
 * 
 * Revision 2.2  90/06/02  15:29:06  rpd
 * 	Converted to new IPC.
 * 
 * 	Modified for out-of-kernel use.
 * 	[89/01/17            dbg]
 * 
 * Revision 2.1  89/08/04  14:51:18  rwd
 * Created.
 * 
 * Revision 2.2  88/12/19  02:48:41  mwyoung
 * 	Fix include file references.
 * 	[88/12/19  00:33:03  mwyoung]
 * 	
 * 	Add and use zone_ignore_overflow.
 * 	[88/12/14            mwyoung]
 * 
 *  8-Jan-88  Rick Rashid (rfr) at Carnegie-Mellon University
 *	Made pageable zones really pageable.  Turned spin locks to sleep
 *	locks for pageable zones.
 *
 * 30-Dec-87  David Golub (dbg) at Carnegie-Mellon University
 *	Delinted.
 *
 * 20-Oct-87  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Allocate zone memory from a separate kernel submap, to avoid
 *	sleeping with the kernel_map locked.
 *
 *  1-Oct-87  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Added zchange().
 *
 * 30-Sep-87  Richard Sanzi (sanzi) at Carnegie-Mellon University
 *	Deleted the printf() in zinit() which is called when zinit is
 *	creating a pageable zone.
 *
 * 12-Sep-87  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Modified to use list of elements instead of queues.  Actually,
 *	this package now uses macros defined in zalloc.h which define
 *	the list semantics.
 *
 * 30-Mar-87  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Update zone's cur_size field when it is crammed (zcram).
 *
 * 23-Mar-87  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Only define zfree if there is no macro version.
 *
 * 17-Mar-87  David Golub (dbg) at Carnegie-Mellon University
 *	De-linted.
 *
 * 12-Feb-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Added zget - no waiting version of zalloc.
 *
 * 22-Jan-87  Michael Young (mwyoung) at Carnegie-Mellon University
 *	De-linted.
 *
 * 12-Jan-87  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Eliminated use of the old interlocked queuing package;
 *	random other cleanups.
 *
 *  9-Jun-85  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Created.
 */

#include <sys/zalloc.h>
#include <uxkern/import_mach.h>

zone_t		zone_zone;	/* this is the zone containing other zones */

boolean_t	zone_ignore_overflow = FALSE;

vm_offset_t	zdata;
vm_size_t	zdata_size;

#define	lock_zone(zone)		mutex_lock(&zone->lock)

#define	unlock_zone(zone)	mutex_unlock(&zone->lock)

#define	lock_zone_init(zone)	mutex_init(&zone->lock)

/*
 *	Initialize the "zone of zones."
 */
void zone_init()
{
	vm_offset_t	p;

	zdata_size = round_page(64 * sizeof(struct zone));
	(void) vm_allocate(mach_task_self(), &zdata, zdata_size, TRUE);
#if	0
	(void) vm_pageable(mach_task_self(), zdata, zdata_size,
			   VM_PROT_READ|VM_PROT_WRITE);
#endif

	zone_zone = ZONE_NULL;
	zone_zone = zinit(sizeof(struct zone), sizeof(struct zone), 0,
					FALSE, "zones");
	p = (vm_offset_t)(zone_zone + 1);
	zcram(zone_zone, p, (zdata + zdata_size) - p);
}

/*
 *	zinit initializes a new zone.  The zone data structures themselves
 *	are stored in a zone, which is initially a static structure that
 *	is initialized by zone_init.
 */
zone_t zinit(size, max, alloc, pageable, name)
	vm_size_t	size;		/* the size of an element */
	vm_size_t	max;		/* maximum memory to use */
	vm_size_t	alloc;		/* allocation size */
	boolean_t	pageable;	/* is this zone pageable? */
	char		*name;		/* a name for the zone */
{
	register zone_t		z;

	if (zone_zone == ZONE_NULL)
		z = (zone_t) zdata;
	else if ((z = (zone_t) zalloc(zone_zone)) == ZONE_NULL)
		return(ZONE_NULL);

	/*
	 *	Round off all the parameters appropriately.
	 */

	if ((max = round_page(max)) < (alloc = round_page(alloc)))
		max = alloc;

	z->free_elements = 0;
	z->cur_size = 0;
	z->max_size = max;
	z->elem_size = size;
	z->alloc_size = alloc;
	z->pageable = pageable;
	z->zone_name = name;
	z->count = 0;
	z->doing_alloc = FALSE;
	z->exhaustible = z->sleepable = FALSE;
	lock_zone_init(z);
	return(z);
}

/*
 *	Cram the given memory into the specified zone.
 */
void zcram(zone, newmem, size)
	register zone_t		zone;
	vm_offset_t		newmem;
	vm_size_t		size;
{
	register vm_size_t	elem_size;

	elem_size = zone->elem_size;

	lock_zone(zone);
	zone->cur_size += size;
	while (size >= elem_size) {
		ADD_TO_ZONE(zone, newmem);
		zone->count++;	/* compensate for ADD_TO_ZONE */
		size -= elem_size;
		newmem += elem_size;
	}
	unlock_zone(zone);
}

/*
 *	zalloc returns an element from the specified zone.
 */
vm_offset_t zalloc(zone)
	register zone_t	zone;
{
	register vm_offset_t	addr;

	if (zone == ZONE_NULL)
		panic ("zalloc: null zone");

	lock_zone(zone);
	REMOVE_FROM_ZONE(zone, addr, vm_offset_t);
	while (addr == 0) {
		/*
		 *	If nothing was there, try to get more
		 */
		vm_offset_t	alloc_addr;

		if ((zone->cur_size + zone->alloc_size) > zone->max_size) {
			if (zone->exhaustible)
				break;

			if (!zone_ignore_overflow) {
				printf("zone \"%s\" empty.\n", zone->zone_name);
				panic("zalloc");
			}
		}
		unlock_zone(zone);

		if (vm_allocate(mach_task_self(),
				&alloc_addr,
				zone->alloc_size,
				TRUE)
			!= KERN_SUCCESS) {
		    if (zone->exhaustible)
			break;
		    panic("zalloc");
		}
#if	0
		if (!zone->pageable) {
		    (void) vm_pageable(mach_task_self(),
					alloc_addr,
					zone->alloc_size,
					VM_PROT_READ|VM_PROT_WRITE);
		}
#endif
		zcram(zone, alloc_addr, zone->alloc_size);

		lock_zone(zone);

		REMOVE_FROM_ZONE(zone, addr, vm_offset_t);
	}

	unlock_zone(zone);
	return(addr);
}

/*
 *	zget returns an element from the specified zone
 *	and immediately returns nothing if there is nothing there.
 *
 *	This form should be used when you can not block (like when
 *	processing an interrupt).
 */
vm_offset_t zget(zone)
	register zone_t	zone;
{
	register vm_offset_t	addr;

	if (zone == ZONE_NULL)
		panic ("zalloc: null zone");

	lock_zone(zone);
	REMOVE_FROM_ZONE(zone, addr, vm_offset_t);
	unlock_zone(zone);

	return(addr);
}

void zfree(zone, elem)
	register zone_t	zone;
	vm_offset_t	elem;
{
	lock_zone(zone);
	ADD_TO_ZONE(zone, elem);
	unlock_zone(zone);
}


void zchange(zone, pageable, sleepable, exhaustible)
	zone_t		zone;
	boolean_t	pageable;
	boolean_t	sleepable;
	boolean_t	exhaustible;
{
	zone->pageable = pageable;
	zone->sleepable = sleepable;
	zone->exhaustible = exhaustible;
}
