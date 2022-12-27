#ifndef	_memory_object_default_user_
#define	_memory_object_default_user_

/* Module memory_object_default */

#include <mach/kern_return.h>
#if	(defined(__STDC__) || defined(c_plusplus)) || defined(LINTLIBRARY)
#include <mach/port.h>
#include <mach/message.h>
#endif

#include <mach/std_types.h>
#include <mach/mach_types.h>

/* SimpleRoutine memory_object_create */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t memory_object_create
#if	defined(LINTLIBRARY)
    (old_memory_object, new_memory_object, new_object_size, new_control_port, new_name, new_page_size)
	mach_port_t old_memory_object;
	mach_port_t new_memory_object;
	vm_size_t new_object_size;
	mach_port_t new_control_port;
	mach_port_t new_name;
	vm_size_t new_page_size;
{ return memory_object_create(old_memory_object, new_memory_object, new_object_size, new_control_port, new_name, new_page_size); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t old_memory_object,
	mach_port_t new_memory_object,
	vm_size_t new_object_size,
	mach_port_t new_control_port,
	mach_port_t new_name,
	vm_size_t new_page_size
);
#else
    ();
#endif
#endif

/* SimpleRoutine memory_object_data_initialize */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t memory_object_data_initialize
#if	defined(LINTLIBRARY)
    (memory_object, memory_control_port, offset, data, dataCnt)
	mach_port_t memory_object;
	mach_port_t memory_control_port;
	vm_offset_t offset;
	pointer_t data;
	mach_msg_type_number_t dataCnt;
{ return memory_object_data_initialize(memory_object, memory_control_port, offset, data, dataCnt); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t memory_object,
	mach_port_t memory_control_port,
	vm_offset_t offset,
	pointer_t data,
	mach_msg_type_number_t dataCnt
);
#else
    ();
#endif
#endif

#endif	_memory_object_default_user_
