#ifndef	_default_pager_object_user_
#define	_default_pager_object_user_

/* Module default_pager_object */

#include <mach/kern_return.h>
#if	(defined(__STDC__) || defined(c_plusplus)) || defined(LINTLIBRARY)
#include <mach/port.h>
#include <mach/message.h>
#endif

#include <mach/std_types.h>
#include <mach/mach_types.h>

/* Routine default_pager_object_create */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t default_pager_object_create
#if	defined(LINTLIBRARY)
    (default_pager, memory_object, object_size)
	mach_port_t default_pager;
	memory_object_t *memory_object;
	vm_size_t object_size;
{ return default_pager_object_create(default_pager, memory_object, object_size); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t default_pager,
	memory_object_t *memory_object,
	vm_size_t object_size
);
#else
    ();
#endif
#endif

/* Routine default_pager_info */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t default_pager_info
#if	defined(LINTLIBRARY)
    (default_pager, total_space, free_space)
	mach_port_t default_pager;
	vm_size_t *total_space;
	vm_size_t *free_space;
{ return default_pager_info(default_pager, total_space, free_space); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t default_pager,
	vm_size_t *total_space,
	vm_size_t *free_space
);
#else
    ();
#endif
#endif

#endif	_default_pager_object_user_
