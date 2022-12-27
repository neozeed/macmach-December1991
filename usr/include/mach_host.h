#ifndef	_mach_host_user_
#define	_mach_host_user_

/* Module mach_host */

#include <mach/kern_return.h>
#if	(defined(__STDC__) || defined(c_plusplus)) || defined(LINTLIBRARY)
#include <mach/port.h>
#include <mach/message.h>
#endif

#include <mach/std_types.h>
#include <mach/mach_types.h>

/* Routine host_processors */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t host_processors
#if	defined(LINTLIBRARY)
    (host_priv, processor_list, processor_listCnt)
	mach_port_t host_priv;
	processor_array_t *processor_list;
	mach_msg_type_number_t *processor_listCnt;
{ return host_processors(host_priv, processor_list, processor_listCnt); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t host_priv,
	processor_array_t *processor_list,
	mach_msg_type_number_t *processor_listCnt
);
#else
    ();
#endif
#endif

/* Routine host_info */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t host_info
#if	defined(LINTLIBRARY)
    (host, flavor, host_info_out, host_info_outCnt)
	mach_port_t host;
	int flavor;
	host_info_t host_info_out;
	mach_msg_type_number_t *host_info_outCnt;
{ return host_info(host, flavor, host_info_out, host_info_outCnt); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t host,
	int flavor,
	host_info_t host_info_out,
	mach_msg_type_number_t *host_info_outCnt
);
#else
    ();
#endif
#endif

/* Routine processor_info */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t processor_info
#if	defined(LINTLIBRARY)
    (processor, flavor, host, processor_info_out, processor_info_outCnt)
	mach_port_t processor;
	int flavor;
	mach_port_t *host;
	processor_info_t processor_info_out;
	mach_msg_type_number_t *processor_info_outCnt;
{ return processor_info(processor, flavor, host, processor_info_out, processor_info_outCnt); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t processor,
	int flavor,
	mach_port_t *host,
	processor_info_t processor_info_out,
	mach_msg_type_number_t *processor_info_outCnt
);
#else
    ();
#endif
#endif

/* Routine processor_start */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t processor_start
#if	defined(LINTLIBRARY)
    (processor)
	mach_port_t processor;
{ return processor_start(processor); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t processor
);
#else
    ();
#endif
#endif

/* Routine processor_exit */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t processor_exit
#if	defined(LINTLIBRARY)
    (processor)
	mach_port_t processor;
{ return processor_exit(processor); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t processor
);
#else
    ();
#endif
#endif

/* Routine processor_control */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t processor_control
#if	defined(LINTLIBRARY)
    (processor, processor_cmd, processor_cmdCnt)
	mach_port_t processor;
	processor_info_t processor_cmd;
	mach_msg_type_number_t processor_cmdCnt;
{ return processor_control(processor, processor_cmd, processor_cmdCnt); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t processor,
	processor_info_t processor_cmd,
	mach_msg_type_number_t processor_cmdCnt
);
#else
    ();
#endif
#endif

/* Routine processor_set_default */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t processor_set_default
#if	defined(LINTLIBRARY)
    (host, default_set)
	mach_port_t host;
	mach_port_t *default_set;
{ return processor_set_default(host, default_set); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t host,
	mach_port_t *default_set
);
#else
    ();
#endif
#endif

/* Routine xxx_processor_set_default_priv */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t xxx_processor_set_default_priv
#if	defined(LINTLIBRARY)
    (host, default_set)
	mach_port_t host;
	mach_port_t *default_set;
{ return xxx_processor_set_default_priv(host, default_set); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t host,
	mach_port_t *default_set
);
#else
    ();
#endif
#endif

/* Routine processor_set_create */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t processor_set_create
#if	defined(LINTLIBRARY)
    (host, new_set, new_name)
	mach_port_t host;
	mach_port_t *new_set;
	mach_port_t *new_name;
{ return processor_set_create(host, new_set, new_name); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t host,
	mach_port_t *new_set,
	mach_port_t *new_name
);
#else
    ();
#endif
#endif

/* Routine processor_set_destroy */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t processor_set_destroy
#if	defined(LINTLIBRARY)
    (set)
	mach_port_t set;
{ return processor_set_destroy(set); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t set
);
#else
    ();
#endif
#endif

/* Routine processor_set_info */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t processor_set_info
#if	defined(LINTLIBRARY)
    (set_name, flavor, host, info_out, info_outCnt)
	mach_port_t set_name;
	int flavor;
	mach_port_t *host;
	processor_set_info_t info_out;
	mach_msg_type_number_t *info_outCnt;
{ return processor_set_info(set_name, flavor, host, info_out, info_outCnt); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t set_name,
	int flavor,
	mach_port_t *host,
	processor_set_info_t info_out,
	mach_msg_type_number_t *info_outCnt
);
#else
    ();
#endif
#endif

/* Routine processor_assign */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t processor_assign
#if	defined(LINTLIBRARY)
    (processor, new_set, wait)
	mach_port_t processor;
	mach_port_t new_set;
	boolean_t wait;
{ return processor_assign(processor, new_set, wait); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t processor,
	mach_port_t new_set,
	boolean_t wait
);
#else
    ();
#endif
#endif

/* Routine processor_get_assignment */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t processor_get_assignment
#if	defined(LINTLIBRARY)
    (processor, assigned_set)
	mach_port_t processor;
	mach_port_t *assigned_set;
{ return processor_get_assignment(processor, assigned_set); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t processor,
	mach_port_t *assigned_set
);
#else
    ();
#endif
#endif

/* Routine thread_assign */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t thread_assign
#if	defined(LINTLIBRARY)
    (thread, new_set)
	mach_port_t thread;
	mach_port_t new_set;
{ return thread_assign(thread, new_set); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t thread,
	mach_port_t new_set
);
#else
    ();
#endif
#endif

/* Routine thread_assign_default */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t thread_assign_default
#if	defined(LINTLIBRARY)
    (thread)
	mach_port_t thread;
{ return thread_assign_default(thread); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t thread
);
#else
    ();
#endif
#endif

/* Routine thread_get_assignment */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t thread_get_assignment
#if	defined(LINTLIBRARY)
    (thread, assigned_set)
	mach_port_t thread;
	mach_port_t *assigned_set;
{ return thread_get_assignment(thread, assigned_set); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t thread,
	mach_port_t *assigned_set
);
#else
    ();
#endif
#endif

/* Routine task_assign */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t task_assign
#if	defined(LINTLIBRARY)
    (task, new_set, assign_threads)
	mach_port_t task;
	mach_port_t new_set;
	boolean_t assign_threads;
{ return task_assign(task, new_set, assign_threads); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t task,
	mach_port_t new_set,
	boolean_t assign_threads
);
#else
    ();
#endif
#endif

/* Routine task_assign_default */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t task_assign_default
#if	defined(LINTLIBRARY)
    (task, assign_threads)
	mach_port_t task;
	boolean_t assign_threads;
{ return task_assign_default(task, assign_threads); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t task,
	boolean_t assign_threads
);
#else
    ();
#endif
#endif

/* Routine task_get_assignment */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t task_get_assignment
#if	defined(LINTLIBRARY)
    (task, assigned_set)
	mach_port_t task;
	mach_port_t *assigned_set;
{ return task_get_assignment(task, assigned_set); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t task,
	mach_port_t *assigned_set
);
#else
    ();
#endif
#endif

/* Routine host_kernel_version */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t host_kernel_version
#if	defined(LINTLIBRARY)
    (host, kernel_version)
	mach_port_t host;
	kernel_version_t kernel_version;
{ return host_kernel_version(host, kernel_version); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t host,
	kernel_version_t kernel_version
);
#else
    ();
#endif
#endif

/* Routine thread_priority */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t thread_priority
#if	defined(LINTLIBRARY)
    (thread, priority, set_max)
	mach_port_t thread;
	int priority;
	boolean_t set_max;
{ return thread_priority(thread, priority, set_max); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t thread,
	int priority,
	boolean_t set_max
);
#else
    ();
#endif
#endif

/* Routine thread_max_priority */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t thread_max_priority
#if	defined(LINTLIBRARY)
    (thread, processor_set, max_priority)
	mach_port_t thread;
	mach_port_t processor_set;
	int max_priority;
{ return thread_max_priority(thread, processor_set, max_priority); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t thread,
	mach_port_t processor_set,
	int max_priority
);
#else
    ();
#endif
#endif

/* Routine task_priority */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t task_priority
#if	defined(LINTLIBRARY)
    (task, priority, change_threads)
	mach_port_t task;
	int priority;
	boolean_t change_threads;
{ return task_priority(task, priority, change_threads); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t task,
	int priority,
	boolean_t change_threads
);
#else
    ();
#endif
#endif

/* Routine processor_set_max_priority */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t processor_set_max_priority
#if	defined(LINTLIBRARY)
    (processor_set, max_priority, change_threads)
	mach_port_t processor_set;
	int max_priority;
	boolean_t change_threads;
{ return processor_set_max_priority(processor_set, max_priority, change_threads); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t processor_set,
	int max_priority,
	boolean_t change_threads
);
#else
    ();
#endif
#endif

/* Routine thread_policy */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t thread_policy
#if	defined(LINTLIBRARY)
    (thread, policy, data)
	mach_port_t thread;
	int policy;
	int data;
{ return thread_policy(thread, policy, data); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t thread,
	int policy,
	int data
);
#else
    ();
#endif
#endif

/* Routine processor_set_policy_enable */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t processor_set_policy_enable
#if	defined(LINTLIBRARY)
    (processor_set, policy)
	mach_port_t processor_set;
	int policy;
{ return processor_set_policy_enable(processor_set, policy); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t processor_set,
	int policy
);
#else
    ();
#endif
#endif

/* Routine processor_set_policy_disable */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t processor_set_policy_disable
#if	defined(LINTLIBRARY)
    (processor_set, policy, change_threads)
	mach_port_t processor_set;
	int policy;
	boolean_t change_threads;
{ return processor_set_policy_disable(processor_set, policy, change_threads); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t processor_set,
	int policy,
	boolean_t change_threads
);
#else
    ();
#endif
#endif

/* Routine processor_set_tasks */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t processor_set_tasks
#if	defined(LINTLIBRARY)
    (processor_set, task_list, task_listCnt)
	mach_port_t processor_set;
	task_array_t *task_list;
	mach_msg_type_number_t *task_listCnt;
{ return processor_set_tasks(processor_set, task_list, task_listCnt); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t processor_set,
	task_array_t *task_list,
	mach_msg_type_number_t *task_listCnt
);
#else
    ();
#endif
#endif

/* Routine processor_set_threads */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t processor_set_threads
#if	defined(LINTLIBRARY)
    (processor_set, thread_list, thread_listCnt)
	mach_port_t processor_set;
	thread_array_t *thread_list;
	mach_msg_type_number_t *thread_listCnt;
{ return processor_set_threads(processor_set, thread_list, thread_listCnt); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t processor_set,
	thread_array_t *thread_list,
	mach_msg_type_number_t *thread_listCnt
);
#else
    ();
#endif
#endif

/* Routine host_processor_sets */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t host_processor_sets
#if	defined(LINTLIBRARY)
    (host, processor_sets, processor_setsCnt)
	mach_port_t host;
	processor_set_name_array_t *processor_sets;
	mach_msg_type_number_t *processor_setsCnt;
{ return host_processor_sets(host, processor_sets, processor_setsCnt); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t host,
	processor_set_name_array_t *processor_sets,
	mach_msg_type_number_t *processor_setsCnt
);
#else
    ();
#endif
#endif

/* Routine host_processor_set_priv */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t host_processor_set_priv
#if	defined(LINTLIBRARY)
    (host_priv, set_name, set)
	mach_port_t host_priv;
	mach_port_t set_name;
	mach_port_t *set;
{ return host_processor_set_priv(host_priv, set_name, set); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t host_priv,
	mach_port_t set_name,
	mach_port_t *set
);
#else
    ();
#endif
#endif

/* Routine host_set_time */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t host_set_time
#if	defined(LINTLIBRARY)
    (host_priv, new_time)
	mach_port_t host_priv;
	time_value_t new_time;
{ return host_set_time(host_priv, new_time); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t host_priv,
	time_value_t new_time
);
#else
    ();
#endif
#endif

/* Routine host_adjust_time */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t host_adjust_time
#if	defined(LINTLIBRARY)
    (host_priv, new_adjustment, old_adjustment)
	mach_port_t host_priv;
	time_value_t new_adjustment;
	time_value_t *old_adjustment;
{ return host_adjust_time(host_priv, new_adjustment, old_adjustment); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t host_priv,
	time_value_t new_adjustment,
	time_value_t *old_adjustment
);
#else
    ();
#endif
#endif

/* Routine host_get_time */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t host_get_time
#if	defined(LINTLIBRARY)
    (host, current_time)
	mach_port_t host;
	time_value_t *current_time;
{ return host_get_time(host, current_time); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t host,
	time_value_t *current_time
);
#else
    ();
#endif
#endif

/* Routine host_reboot */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t host_reboot
#if	defined(LINTLIBRARY)
    (host_priv, options)
	mach_port_t host_priv;
	int options;
{ return host_reboot(host_priv, options); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t host_priv,
	int options
);
#else
    ();
#endif
#endif

/* Routine vm_wire */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t vm_wire
#if	defined(LINTLIBRARY)
    (host_priv, task, address, size, access)
	mach_port_t host_priv;
	mach_port_t task;
	vm_address_t address;
	vm_size_t size;
	vm_prot_t access;
{ return vm_wire(host_priv, task, address, size, access); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t host_priv,
	mach_port_t task,
	vm_address_t address,
	vm_size_t size,
	vm_prot_t access
);
#else
    ();
#endif
#endif

/* Routine thread_wire */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t thread_wire
#if	defined(LINTLIBRARY)
    (host_priv, thread, wired)
	mach_port_t host_priv;
	mach_port_t thread;
	boolean_t wired;
{ return thread_wire(host_priv, thread, wired); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t host_priv,
	mach_port_t thread,
	boolean_t wired
);
#else
    ();
#endif
#endif

#endif	_mach_host_user_
