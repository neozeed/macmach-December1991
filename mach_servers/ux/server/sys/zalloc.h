/* 
 * Mach Operating System
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 *	File:	zalloc.h
 *	Author:	Avadis Tevanian, Jr.
 *
 *	Copyright (C) 1985, Avadis Tevanian, Jr.
 *
 * HISTORY
 * 17-Jan-89  David Golub (dbg) at Carnegie-Mellon University
 *	Modified for out-of-kernel use.
 *
 * 19-Dec-88  David Golub (dbg) at Carnegie-Mellon University
 *	Added register copy of zone in ZFREE macro.
 *
 * $Log:	zalloc.h,v $
 * Revision 2.1  89/08/04  14:46:15  rwd
 * Created.
 * 
 * Revision 2.3  88/12/19  02:52:11  mwyoung
 * 	Use MACRO_BEGIN and MACRO_END.  This corrects a problem
 * 	in the ZGET macro under lint.
 * 	[88/12/09            mwyoung]
 * 
 * Revision 2.2  88/08/24  02:56:21  mwyoung
 * 	Adjusted include file references.
 * 	[88/08/17  02:30:22  mwyoung]
 * 
 *
 *  8-Jan-88  Rick Rashid (rfr) at Carnegie-Mellon University
 *	Pageable zone lock added. NOTE ZALLOC and ZFREE macros assume
 *	non-pageable zones.
 *
 * 30-Sep-87  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Make ZALLOC, ZFREE not macros for lint.
 *
 * 12-Sep-87  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Modified to use a list instead of a queue - no need for expense
 *	of queue.  Also removed warning about assembly language hacks as
 *	there are none left that I know of.
 *
 *  1-Sep-87  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Added zchange() declaration; added sleepable, exhaustible flags.
 *
 * 18-Apr-87  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Added ZALLOC and ZGET macros, note that the arguments are
 *	different that zalloc and zget due to language restrictions.
 *	For consistency, also made a ZFREE macro, zfree is now always
 *	a procedure call.
 *
 * 15-Apr-87  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Added warning about implementations that inline expand zalloc
 *	and zget.
 *
 * 23-Mar-87  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Simplified zfree macro.
 *
 *  9-Mar-87  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Make zfree a macro: a big win on a register-based machine
 *	with expensive procedure call; smaller change elsewhere.
 *
 *  3-Mar-87  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Reduce include of "../h/types.h" to "../machine/vm_types.h".
 *
 * 12-Feb-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Added zget.
 *
 * 12-Jan-87  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Eliminated use of the interlocked queue package.
 *
 *  9-Jun-85  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Created.
 */

#ifndef	_ZALLOC_
#define	_ZALLOC_

#include <uxkern/import_mach.h>

#include <sys/macro_help.h>

/*
 *	A zone is a collection of fixed size blocks for which there
 *	is fast allocation/deallocation access.  Kernel routines can
 *	use zones to manage data structures dynamically, creating a zone
 *	for each type of data structure to be managed.
 *
 */

typedef struct zone {
	struct mutex	lock;		/* generic lock */
	int		count;		/* Number of elements used now */
	vm_offset_t	free_elements;
	vm_size_t	cur_size;	/* current memory utilization */
	vm_size_t	max_size;	/* how large can this zone grow */
	vm_size_t	elem_size;	/* size of an element */
	vm_size_t	alloc_size;	/* size used for more memory */
	boolean_t	doing_alloc;	/* is zone expanding now? */
	char		*zone_name;	/* a name for the zone */
	unsigned int
	/* boolean_t */	pageable :1,	/* zone pageable? */
	/* boolean_t */	sleepable :1,	/* sleep if empty? */
	/* boolean_t */ exhaustible :1;	/* merely return if empty? */
} *zone_t;

#define		ZONE_NULL	((zone_t) 0)

vm_offset_t	zalloc();
vm_offset_t	zget();
zone_t		zinit();
void		zfree();
void		zchange();

#define ADD_TO_ZONE(zone, element) \
	MACRO_BEGIN							\
		*((vm_offset_t *)(element)) = (zone)->free_elements;	\
		(zone)->free_elements = (vm_offset_t) (element);	\
		(zone)->count--;					\
	MACRO_END

#define REMOVE_FROM_ZONE(zone, ret, type)				\
	MACRO_BEGIN							\
	(ret) = (type) (zone)->free_elements;				\
	if ((ret) != (type) 0) {					\
		(zone)->count++;					\
		(zone)->free_elements = *((vm_offset_t *)(ret));	\
	}								\
	MACRO_END

#define ZFREE(zone, element)		\
	MACRO_BEGIN			\
	register zone_t	z = (zone);	\
					\
	mutex_lock(&z->lock);		\
	ADD_TO_ZONE(z, element);	\
	mutex_unlock(&z->lock);		\
	MACRO_END

#define	ZALLOC(zone, ret, type)			\
	MACRO_BEGIN				\
	register zone_t	z = (zone);		\
						\
	mutex_lock(&z->lock);			\
	REMOVE_FROM_ZONE(zone, ret, type);	\
	mutex_unlock(&z->lock);			\
	if ((ret) == (type)0)			\
		(ret) = (type)zalloc(z);	\
	MACRO_END

#define	ZGET(zone, ret, type)			\
	MACRO_BEGIN				\
	register zone_t	z = (zone);		\
						\
	mutex_lock(&z->lock);			\
	REMOVE_FROM_ZONE(zone, ret, type);	\
	mutex_unlock(&z->lock);			\
	MACRO_END

void		zcram();
void		zone_init();

#endif	_ZALLOC_
