/* 
 * Mach Operating System
 * Copyright (c) 1991,1990 Carnegie Mellon University
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 * 
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS 
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
 * any improvements or extensions that they make and grant Carnegie the
 * rights to redistribute these changes.
 */
/*
 * HISTORY
 * $Log:	machid_types.h,v $
 * Revision 2.2  91/08/29  15:05:27  rpd
 * 	Moved to the include directory.
 * 	[91/08/29            rpd]
 * 
 * 	Added mdefault_pager_t, MACH_TYPE_DEFAULT_PAGER,
 * 	KERN_INVALID_DEFAULT_PAGER.
 * 	[91/08/15            rpd]
 * 
 * Revision 2.3  91/03/19  12:31:06  mrt
 * 	Changed to new copyright
 * 
 * Revision 2.2  90/09/12  16:32:03  rpd
 * 	Added mprocessor_set_name_array_t.
 * 	[90/08/07            rpd]
 * 
 * 	Created.
 * 	[90/06/18            rpd]
 * 
 */

#ifndef	_MACHID_TYPES_H_
#define	_MACHID_TYPES_H_

#include <mach/boolean.h>
#include <mach/kern_return.h>
#include <mach/port.h>
#include <mach/task_info.h>
#include <mach/machine/vm_types.h>
#include <mach/vm_prot.h>
#include <mach/vm_inherit.h>

typedef unsigned int mach_id_t;
typedef unsigned int mach_type_t;

#define MACH_TYPE_NONE			0
#define	MACH_TYPE_TASK			1
#define MACH_TYPE_THREAD		2
#define MACH_TYPE_PROCESSOR_SET		3
#define MACH_TYPE_PROCESSOR_SET_NAME	4
#define MACH_TYPE_PROCESSOR		5
#define MACH_TYPE_HOST			6
#define MACH_TYPE_HOST_PRIV		7
#define MACH_TYPE_OBJECT		8
#define MACH_TYPE_OBJECT_CONTROL	9
#define MACH_TYPE_OBJECT_NAME		10
#define MACH_TYPE_MASTER_DEVICE		11
#define MACH_TYPE_DEFAULT_PAGER		12

extern char *mach_type_string(/* type */);

typedef mach_id_t mhost_t;
typedef mach_id_t mhost_priv_t;

typedef mach_id_t mdefault_pager_t;

typedef mach_id_t mprocessor_t;
typedef mprocessor_t *mprocessor_array_t;

typedef mach_id_t mprocessor_set_t;
typedef mprocessor_set_t *mprocessor_set_array_t;
typedef mach_id_t mprocessor_set_name_t;
typedef mprocessor_set_name_t *mprocessor_set_name_array_t;

typedef mach_id_t mtask_t;
typedef mtask_t *mtask_array_t;

typedef mach_id_t mthread_t;
typedef mthread_t *mthread_array_t;

typedef mach_id_t mobject_t;
typedef mach_id_t mobject_control_t;
typedef mach_id_t mobject_name_t;

typedef struct vm_region_info {
    vm_offset_t vr_address;
    vm_size_t vr_size;
    vm_prot_t vr_prot;
    vm_prot_t vr_max_prot;
    vm_inherit_t vr_inherit;
    boolean_t vr_shared;
    mobject_name_t vr_name;
    vm_offset_t vr_offset;
} vm_region_info_t;

#ifdef	mips
#include <mach/mips/thread_status.h>

typedef struct mips_thread_state mips_thread_state_t;
#endif	mips

#ifdef	sun3
#include <mach/sun3/thread_status.h>

typedef struct sun_thread_state sun3_thread_state_t;
#endif	sun3

#ifdef	vax
#include <mach/vax/thread_status.h>

typedef struct vax_thread_state vax_thread_state_t;
#endif	vax

#ifdef	i386
#include <mach/i386/thread_status.h>

typedef struct i386_thread_state i386_thread_state_t;
#endif	i386

typedef int unix_pid_t;
typedef char *unix_command_t;

#define	KERN_INVALID_THREAD		KERN_INVALID_ARGUMENT
#define	KERN_INVALID_PROCESSOR_SET	KERN_INVALID_ARGUMENT
#define	KERN_INVALID_PROCESSOR_SET_NAME	KERN_INVALID_ARGUMENT
#define KERN_INVALID_HOST_PRIV		KERN_INVALID_HOST
#define KERN_INVALID_PROCESSOR		KERN_INVALID_ARGUMENT
#define KERN_INVALID_DEFAULT_PAGER	KERN_INVALID_ARGUMENT

#endif	_MACHID_TYPES_H_
