#ifndef	_machid_debug_user_
#define	_machid_debug_user_

/* Module machid_debug */

#include <mach/kern_return.h>
#if	(defined(__STDC__) || defined(c_plusplus)) || defined(LINTLIBRARY)
#include <mach/port.h>
#include <mach/message.h>
#endif

#include <mach/std_types.h>
#include <mach/mach_types.h>
#include <mach_debug/mach_debug_types.h>
#include <servers/machid_types.h>

/* Routine port_get_srights */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t machid_port_get_srights
#if	defined(LINTLIBRARY)
    (server, auth, task, name, srights)
	mach_port_t server;
	mach_port_t auth;
	mtask_t task;
	mach_port_t name;
	mach_port_rights_t *srights;
{ return machid_port_get_srights(server, auth, task, name, srights); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mtask_t task,
	mach_port_t name,
	mach_port_rights_t *srights
);
#else
    ();
#endif
#endif

/* Routine port_space_info */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t machid_port_space_info
#if	defined(LINTLIBRARY)
    (server, auth, task, info, table_info, table_infoCnt, tree_info, tree_infoCnt)
	mach_port_t server;
	mach_port_t auth;
	mtask_t task;
	ipc_info_space_t *info;
	ipc_info_name_array_t *table_info;
	mach_msg_type_number_t *table_infoCnt;
	ipc_info_tree_name_array_t *tree_info;
	mach_msg_type_number_t *tree_infoCnt;
{ return machid_port_space_info(server, auth, task, info, table_info, table_infoCnt, tree_info, tree_infoCnt); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mtask_t task,
	ipc_info_space_t *info,
	ipc_info_name_array_t *table_info,
	mach_msg_type_number_t *table_infoCnt,
	ipc_info_tree_name_array_t *tree_info,
	mach_msg_type_number_t *tree_infoCnt
);
#else
    ();
#endif
#endif

/* Routine port_dnrequest_info */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t machid_port_dnrequest_info
#if	defined(LINTLIBRARY)
    (server, auth, task, name, total, used)
	mach_port_t server;
	mach_port_t auth;
	mtask_t task;
	mach_port_t name;
	unsigned *total;
	unsigned *used;
{ return machid_port_dnrequest_info(server, auth, task, name, total, used); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mtask_t task,
	mach_port_t name,
	unsigned *total,
	unsigned *used
);
#else
    ();
#endif
#endif

/* Routine vm_region_info */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t machid_vm_region_info
#if	defined(LINTLIBRARY)
    (server, auth, task, addr, region, objects, objectsCnt)
	mach_port_t server;
	mach_port_t auth;
	mtask_t task;
	vm_offset_t addr;
	vm_info_region_t *region;
	vm_info_object_array_t *objects;
	mach_msg_type_number_t *objectsCnt;
{ return machid_vm_region_info(server, auth, task, addr, region, objects, objectsCnt); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mtask_t task,
	vm_offset_t addr,
	vm_info_region_t *region,
	vm_info_object_array_t *objects,
	mach_msg_type_number_t *objectsCnt
);
#else
    ();
#endif
#endif

/* Routine host_stack_usage */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t machid_host_stack_usage
#if	defined(LINTLIBRARY)
    (server, auth, host, reserved, total, space, resident, maxusage, maxstack)
	mach_port_t server;
	mach_port_t auth;
	mhost_t host;
	vm_size_t *reserved;
	unsigned *total;
	vm_size_t *space;
	vm_size_t *resident;
	vm_size_t *maxusage;
	vm_offset_t *maxstack;
{ return machid_host_stack_usage(server, auth, host, reserved, total, space, resident, maxusage, maxstack); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mhost_t host,
	vm_size_t *reserved,
	unsigned *total,
	vm_size_t *space,
	vm_size_t *resident,
	vm_size_t *maxusage,
	vm_offset_t *maxstack
);
#else
    ();
#endif
#endif

/* Routine processor_set_stack_usage */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t machid_processor_set_stack_usage
#if	defined(LINTLIBRARY)
    (server, auth, pset, total, space, resident, maxusage, maxstack)
	mach_port_t server;
	mach_port_t auth;
	mprocessor_set_t pset;
	unsigned *total;
	vm_size_t *space;
	vm_size_t *resident;
	vm_size_t *maxusage;
	vm_offset_t *maxstack;
{ return machid_processor_set_stack_usage(server, auth, pset, total, space, resident, maxusage, maxstack); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mprocessor_set_t pset,
	unsigned *total,
	vm_size_t *space,
	vm_size_t *resident,
	vm_size_t *maxusage,
	vm_offset_t *maxstack
);
#else
    ();
#endif
#endif

/* Routine host_zone_info */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t machid_host_zone_info
#if	defined(LINTLIBRARY)
    (server, auth, host, names, namesCnt, info, infoCnt)
	mach_port_t server;
	mach_port_t auth;
	mhost_t host;
	zone_name_array_t *names;
	mach_msg_type_number_t *namesCnt;
	zone_info_array_t *info;
	mach_msg_type_number_t *infoCnt;
{ return machid_host_zone_info(server, auth, host, names, namesCnt, info, infoCnt); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mhost_t host,
	zone_name_array_t *names,
	mach_msg_type_number_t *namesCnt,
	zone_info_array_t *info,
	mach_msg_type_number_t *infoCnt
);
#else
    ();
#endif
#endif

#endif	_machid_debug_user_
