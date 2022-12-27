#ifndef	_machid_user_
#define	_machid_user_

/* Module machid */

#include <mach/kern_return.h>
#if	(defined(__STDC__) || defined(c_plusplus)) || defined(LINTLIBRARY)
#include <mach/port.h>
#include <mach/message.h>
#endif

#include <mach/std_types.h>
#include <mach/mach_types.h>
#include <servers/machid_types.h>

/* Routine mach_type */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t machid_mach_type
#if	defined(LINTLIBRARY)
    (server, auth, id, mtype)
	mach_port_t server;
	mach_port_t auth;
	mach_id_t id;
	mach_type_t *mtype;
{ return machid_mach_type(server, auth, id, mtype); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mach_id_t id,
	mach_type_t *mtype
);
#else
    ();
#endif
#endif

/* Routine mach_register */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t machid_mach_register
#if	defined(LINTLIBRARY)
    (server, auth, port, mtype, id)
	mach_port_t server;
	mach_port_t auth;
	mach_port_t port;
	mach_type_t mtype;
	mach_id_t *id;
{ return machid_mach_register(server, auth, port, mtype, id); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mach_port_t port,
	mach_type_t mtype,
	mach_id_t *id
);
#else
    ();
#endif
#endif

/* Routine mach_lookup */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t machid_mach_lookup
#if	defined(LINTLIBRARY)
    (server, auth, name, atype, aname)
	mach_port_t server;
	mach_port_t auth;
	mach_id_t name;
	mach_type_t atype;
	mach_id_t *aname;
{ return machid_mach_lookup(server, auth, name, atype, aname); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mach_id_t name,
	mach_type_t atype,
	mach_id_t *aname
);
#else
    ();
#endif
#endif

/* Routine mach_port */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t machid_mach_port
#if	defined(LINTLIBRARY)
    (server, auth, name, port)
	mach_port_t server;
	mach_port_t auth;
	mach_id_t name;
	mach_port_t *port;
{ return machid_mach_port(server, auth, name, port); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mach_id_t name,
	mach_port_t *port
);
#else
    ();
#endif
#endif

/* Routine host_ports */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t machid_host_ports
#if	defined(LINTLIBRARY)
    (server, auth, host, phost)
	mach_port_t server;
	mach_port_t auth;
	mhost_t *host;
	mhost_priv_t *phost;
{ return machid_host_ports(server, auth, host, phost); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mhost_t *host,
	mhost_priv_t *phost
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
kern_return_t machid_host_processor_sets
#if	defined(LINTLIBRARY)
    (server, auth, host, sets, setsCnt)
	mach_port_t server;
	mach_port_t auth;
	mhost_priv_t host;
	mprocessor_set_array_t *sets;
	mach_msg_type_number_t *setsCnt;
{ return machid_host_processor_sets(server, auth, host, sets, setsCnt); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mhost_priv_t host,
	mprocessor_set_array_t *sets,
	mach_msg_type_number_t *setsCnt
);
#else
    ();
#endif
#endif

/* Routine host_tasks */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t machid_host_tasks
#if	defined(LINTLIBRARY)
    (server, auth, host, tasks, tasksCnt)
	mach_port_t server;
	mach_port_t auth;
	mhost_priv_t host;
	mtask_array_t *tasks;
	mach_msg_type_number_t *tasksCnt;
{ return machid_host_tasks(server, auth, host, tasks, tasksCnt); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mhost_priv_t host,
	mtask_array_t *tasks,
	mach_msg_type_number_t *tasksCnt
);
#else
    ();
#endif
#endif

/* Routine host_threads */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t machid_host_threads
#if	defined(LINTLIBRARY)
    (server, auth, host, threads, threadsCnt)
	mach_port_t server;
	mach_port_t auth;
	mhost_priv_t host;
	mthread_array_t *threads;
	mach_msg_type_number_t *threadsCnt;
{ return machid_host_threads(server, auth, host, threads, threadsCnt); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mhost_priv_t host,
	mthread_array_t *threads,
	mach_msg_type_number_t *threadsCnt
);
#else
    ();
#endif
#endif

/* Routine host_processors */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t machid_host_processors
#if	defined(LINTLIBRARY)
    (server, auth, host, procs, procsCnt)
	mach_port_t server;
	mach_port_t auth;
	mhost_priv_t host;
	mprocessor_array_t *procs;
	mach_msg_type_number_t *procsCnt;
{ return machid_host_processors(server, auth, host, procs, procsCnt); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mhost_priv_t host,
	mprocessor_array_t *procs,
	mach_msg_type_number_t *procsCnt
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
kern_return_t machid_processor_set_threads
#if	defined(LINTLIBRARY)
    (server, auth, pset, threads, threadsCnt)
	mach_port_t server;
	mach_port_t auth;
	mprocessor_set_t pset;
	mthread_array_t *threads;
	mach_msg_type_number_t *threadsCnt;
{ return machid_processor_set_threads(server, auth, pset, threads, threadsCnt); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mprocessor_set_t pset,
	mthread_array_t *threads,
	mach_msg_type_number_t *threadsCnt
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
kern_return_t machid_processor_set_tasks
#if	defined(LINTLIBRARY)
    (server, auth, pset, tasks, tasksCnt)
	mach_port_t server;
	mach_port_t auth;
	mprocessor_set_t pset;
	mtask_array_t *tasks;
	mach_msg_type_number_t *tasksCnt;
{ return machid_processor_set_tasks(server, auth, pset, tasks, tasksCnt); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mprocessor_set_t pset,
	mtask_array_t *tasks,
	mach_msg_type_number_t *tasksCnt
);
#else
    ();
#endif
#endif

/* Routine task_threads */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t machid_task_threads
#if	defined(LINTLIBRARY)
    (server, auth, task, threads, threadsCnt)
	mach_port_t server;
	mach_port_t auth;
	mtask_t task;
	mthread_array_t *threads;
	mach_msg_type_number_t *threadsCnt;
{ return machid_task_threads(server, auth, task, threads, threadsCnt); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mtask_t task,
	mthread_array_t *threads,
	mach_msg_type_number_t *threadsCnt
);
#else
    ();
#endif
#endif

/* Routine host_basic_info */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t machid_host_basic_info
#if	defined(LINTLIBRARY)
    (server, auth, host, info)
	mach_port_t server;
	mach_port_t auth;
	mhost_t host;
	host_basic_info_data_t *info;
{ return machid_host_basic_info(server, auth, host, info); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mhost_t host,
	host_basic_info_data_t *info
);
#else
    ();
#endif
#endif

/* Routine host_sched_info */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t machid_host_sched_info
#if	defined(LINTLIBRARY)
    (server, auth, host, info)
	mach_port_t server;
	mach_port_t auth;
	mhost_t host;
	host_sched_info_data_t *info;
{ return machid_host_sched_info(server, auth, host, info); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mhost_t host,
	host_sched_info_data_t *info
);
#else
    ();
#endif
#endif

/* Routine host_load_info */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t machid_host_load_info
#if	defined(LINTLIBRARY)
    (server, auth, host, info)
	mach_port_t server;
	mach_port_t auth;
	mhost_t host;
	host_load_info_data_t *info;
{ return machid_host_load_info(server, auth, host, info); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mhost_t host,
	host_load_info_data_t *info
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
kern_return_t machid_processor_set_default
#if	defined(LINTLIBRARY)
    (server, auth, host, pset)
	mach_port_t server;
	mach_port_t auth;
	mhost_t host;
	mprocessor_set_name_t *pset;
{ return machid_processor_set_default(server, auth, host, pset); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mhost_t host,
	mprocessor_set_name_t *pset
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
kern_return_t machid_host_kernel_version
#if	defined(LINTLIBRARY)
    (server, auth, host, kernel_version)
	mach_port_t server;
	mach_port_t auth;
	mhost_t host;
	kernel_version_t kernel_version;
{ return machid_host_kernel_version(server, auth, host, kernel_version); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mhost_t host,
	kernel_version_t kernel_version
);
#else
    ();
#endif
#endif

/* Routine processor_basic_info */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t machid_processor_basic_info
#if	defined(LINTLIBRARY)
    (server, auth, proc, host, info)
	mach_port_t server;
	mach_port_t auth;
	mprocessor_t proc;
	mhost_t *host;
	processor_basic_info_data_t *info;
{ return machid_processor_basic_info(server, auth, proc, host, info); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mprocessor_t proc,
	mhost_t *host,
	processor_basic_info_data_t *info
);
#else
    ();
#endif
#endif

/* Routine processor_set_basic_info */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t machid_processor_set_basic_info
#if	defined(LINTLIBRARY)
    (server, auth, pset, host, info)
	mach_port_t server;
	mach_port_t auth;
	mprocessor_set_name_t pset;
	mhost_t *host;
	processor_set_basic_info_data_t *info;
{ return machid_processor_set_basic_info(server, auth, pset, host, info); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mprocessor_set_name_t pset,
	mhost_t *host,
	processor_set_basic_info_data_t *info
);
#else
    ();
#endif
#endif

/* Routine processor_set_sched_info */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t machid_processor_set_sched_info
#if	defined(LINTLIBRARY)
    (server, auth, pset, host, info)
	mach_port_t server;
	mach_port_t auth;
	mprocessor_set_name_t pset;
	mhost_t *host;
	processor_set_sched_info_data_t *info;
{ return machid_processor_set_sched_info(server, auth, pset, host, info); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mprocessor_set_name_t pset,
	mhost_t *host,
	processor_set_sched_info_data_t *info
);
#else
    ();
#endif
#endif

/* Routine task_unix_info */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t machid_task_unix_info
#if	defined(LINTLIBRARY)
    (server, auth, task, pid, comm, commCnt)
	mach_port_t server;
	mach_port_t auth;
	mtask_t task;
	unix_pid_t *pid;
	unix_command_t comm;
	mach_msg_type_number_t *commCnt;
{ return machid_task_unix_info(server, auth, task, pid, comm, commCnt); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mtask_t task,
	unix_pid_t *pid,
	unix_command_t comm,
	mach_msg_type_number_t *commCnt
);
#else
    ();
#endif
#endif

/* Routine task_basic_info */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t machid_task_basic_info
#if	defined(LINTLIBRARY)
    (server, auth, task, info)
	mach_port_t server;
	mach_port_t auth;
	mtask_t task;
	task_basic_info_data_t *info;
{ return machid_task_basic_info(server, auth, task, info); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mtask_t task,
	task_basic_info_data_t *info
);
#else
    ();
#endif
#endif

/* Routine task_thread_times_info */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t machid_task_thread_times_info
#if	defined(LINTLIBRARY)
    (server, auth, task, times)
	mach_port_t server;
	mach_port_t auth;
	mtask_t task;
	task_thread_times_info_data_t *times;
{ return machid_task_thread_times_info(server, auth, task, times); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mtask_t task,
	task_thread_times_info_data_t *times
);
#else
    ();
#endif
#endif

/* Routine thread_basic_info */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t machid_thread_basic_info
#if	defined(LINTLIBRARY)
    (server, auth, thread, info)
	mach_port_t server;
	mach_port_t auth;
	mthread_t thread;
	thread_basic_info_data_t *info;
{ return machid_thread_basic_info(server, auth, thread, info); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mthread_t thread,
	thread_basic_info_data_t *info
);
#else
    ();
#endif
#endif

/* Routine thread_sched_info */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t machid_thread_sched_info
#if	defined(LINTLIBRARY)
    (server, auth, thread, info)
	mach_port_t server;
	mach_port_t auth;
	mthread_t thread;
	thread_sched_info_data_t *info;
{ return machid_thread_sched_info(server, auth, thread, info); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mthread_t thread,
	thread_sched_info_data_t *info
);
#else
    ();
#endif
#endif

/* Routine task_terminate */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t machid_task_terminate
#if	defined(LINTLIBRARY)
    (server, auth, task)
	mach_port_t server;
	mach_port_t auth;
	mtask_t task;
{ return machid_task_terminate(server, auth, task); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mtask_t task
);
#else
    ();
#endif
#endif

/* Routine task_suspend */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t machid_task_suspend
#if	defined(LINTLIBRARY)
    (server, auth, task)
	mach_port_t server;
	mach_port_t auth;
	mtask_t task;
{ return machid_task_suspend(server, auth, task); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mtask_t task
);
#else
    ();
#endif
#endif

/* Routine task_resume */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t machid_task_resume
#if	defined(LINTLIBRARY)
    (server, auth, task)
	mach_port_t server;
	mach_port_t auth;
	mtask_t task;
{ return machid_task_resume(server, auth, task); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mtask_t task
);
#else
    ();
#endif
#endif

/* Routine thread_terminate */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t machid_thread_terminate
#if	defined(LINTLIBRARY)
    (server, auth, thread)
	mach_port_t server;
	mach_port_t auth;
	mthread_t thread;
{ return machid_thread_terminate(server, auth, thread); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mthread_t thread
);
#else
    ();
#endif
#endif

/* Routine thread_suspend */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t machid_thread_suspend
#if	defined(LINTLIBRARY)
    (server, auth, thread)
	mach_port_t server;
	mach_port_t auth;
	mthread_t thread;
{ return machid_thread_suspend(server, auth, thread); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mthread_t thread
);
#else
    ();
#endif
#endif

/* Routine thread_resume */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t machid_thread_resume
#if	defined(LINTLIBRARY)
    (server, auth, thread)
	mach_port_t server;
	mach_port_t auth;
	mthread_t thread;
{ return machid_thread_resume(server, auth, thread); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mthread_t thread
);
#else
    ();
#endif
#endif

/* Routine thread_abort */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t machid_thread_abort
#if	defined(LINTLIBRARY)
    (server, auth, thread)
	mach_port_t server;
	mach_port_t auth;
	mthread_t thread;
{ return machid_thread_abort(server, auth, thread); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mthread_t thread
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
kern_return_t machid_processor_set_destroy
#if	defined(LINTLIBRARY)
    (server, auth, pset)
	mach_port_t server;
	mach_port_t auth;
	mprocessor_set_t pset;
{ return machid_processor_set_destroy(server, auth, pset); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mprocessor_set_t pset
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
kern_return_t machid_processor_start
#if	defined(LINTLIBRARY)
    (server, auth, processor)
	mach_port_t server;
	mach_port_t auth;
	mprocessor_t processor;
{ return machid_processor_start(server, auth, processor); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mprocessor_t processor
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
kern_return_t machid_processor_exit
#if	defined(LINTLIBRARY)
    (server, auth, processor)
	mach_port_t server;
	mach_port_t auth;
	mprocessor_t processor;
{ return machid_processor_exit(server, auth, processor); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mprocessor_t processor
);
#else
    ();
#endif
#endif

/* Routine vm_region */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t machid_vm_region
#if	defined(LINTLIBRARY)
    (server, auth, task, addr, info)
	mach_port_t server;
	mach_port_t auth;
	mtask_t task;
	vm_offset_t addr;
	vm_region_info_t *info;
{ return machid_vm_region(server, auth, task, addr, info); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mtask_t task,
	vm_offset_t addr,
	vm_region_info_t *info
);
#else
    ();
#endif
#endif

/* Routine vm_read */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t machid_vm_read
#if	defined(LINTLIBRARY)
    (server, auth, task, addr, size, data, dataCnt)
	mach_port_t server;
	mach_port_t auth;
	mtask_t task;
	vm_offset_t addr;
	vm_size_t size;
	pointer_t *data;
	mach_msg_type_number_t *dataCnt;
{ return machid_vm_read(server, auth, task, addr, size, data, dataCnt); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mtask_t task,
	vm_offset_t addr,
	vm_size_t size,
	pointer_t *data,
	mach_msg_type_number_t *dataCnt
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
kern_return_t machid_thread_priority
#if	defined(LINTLIBRARY)
    (server, auth, thread, priority, set_max)
	mach_port_t server;
	mach_port_t auth;
	mthread_t thread;
	int priority;
	boolean_t set_max;
{ return machid_thread_priority(server, auth, thread, priority, set_max); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mthread_t thread,
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
kern_return_t machid_thread_max_priority
#if	defined(LINTLIBRARY)
    (server, auth, thread, pset, max_pri)
	mach_port_t server;
	mach_port_t auth;
	mthread_t thread;
	mprocessor_set_t pset;
	int max_pri;
{ return machid_thread_max_priority(server, auth, thread, pset, max_pri); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mthread_t thread,
	mprocessor_set_t pset,
	int max_pri
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
kern_return_t machid_task_priority
#if	defined(LINTLIBRARY)
    (server, auth, task, priority, change_threads)
	mach_port_t server;
	mach_port_t auth;
	mtask_t task;
	int priority;
	boolean_t change_threads;
{ return machid_task_priority(server, auth, task, priority, change_threads); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mtask_t task,
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
kern_return_t machid_processor_set_max_priority
#if	defined(LINTLIBRARY)
    (server, auth, pset, max_pri, change_threads)
	mach_port_t server;
	mach_port_t auth;
	mprocessor_set_t pset;
	int max_pri;
	boolean_t change_threads;
{ return machid_processor_set_max_priority(server, auth, pset, max_pri, change_threads); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mprocessor_set_t pset,
	int max_pri,
	boolean_t change_threads
);
#else
    ();
#endif
#endif

/* Routine port_names */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t machid_port_names
#if	defined(LINTLIBRARY)
    (server, auth, task, names, namesCnt, types, typesCnt)
	mach_port_t server;
	mach_port_t auth;
	mtask_t task;
	mach_port_array_t *names;
	mach_msg_type_number_t *namesCnt;
	mach_port_type_array_t *types;
	mach_msg_type_number_t *typesCnt;
{ return machid_port_names(server, auth, task, names, namesCnt, types, typesCnt); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mtask_t task,
	mach_port_array_t *names,
	mach_msg_type_number_t *namesCnt,
	mach_port_type_array_t *types,
	mach_msg_type_number_t *typesCnt
);
#else
    ();
#endif
#endif

/* Routine port_type */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t machid_port_type
#if	defined(LINTLIBRARY)
    (server, auth, task, name, ptype)
	mach_port_t server;
	mach_port_t auth;
	mtask_t task;
	mach_port_t name;
	mach_port_type_t *ptype;
{ return machid_port_type(server, auth, task, name, ptype); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mtask_t task,
	mach_port_t name,
	mach_port_type_t *ptype
);
#else
    ();
#endif
#endif

/* Routine port_get_refs */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t machid_port_get_refs
#if	defined(LINTLIBRARY)
    (server, auth, task, name, right, refs)
	mach_port_t server;
	mach_port_t auth;
	mtask_t task;
	mach_port_t name;
	mach_port_right_t right;
	mach_port_urefs_t *refs;
{ return machid_port_get_refs(server, auth, task, name, right, refs); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mtask_t task,
	mach_port_t name,
	mach_port_right_t right,
	mach_port_urefs_t *refs
);
#else
    ();
#endif
#endif

/* Routine port_get_receive_status */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t machid_port_get_receive_status
#if	defined(LINTLIBRARY)
    (server, auth, task, name, status)
	mach_port_t server;
	mach_port_t auth;
	mtask_t task;
	mach_port_t name;
	mach_port_status_t *status;
{ return machid_port_get_receive_status(server, auth, task, name, status); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mtask_t task,
	mach_port_t name,
	mach_port_status_t *status
);
#else
    ();
#endif
#endif

/* Routine port_get_set_status */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t machid_port_get_set_status
#if	defined(LINTLIBRARY)
    (server, auth, task, name, members, membersCnt)
	mach_port_t server;
	mach_port_t auth;
	mtask_t task;
	mach_port_t name;
	mach_port_array_t *members;
	mach_msg_type_number_t *membersCnt;
{ return machid_port_get_set_status(server, auth, task, name, members, membersCnt); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mtask_t task,
	mach_port_t name,
	mach_port_array_t *members,
	mach_msg_type_number_t *membersCnt
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
kern_return_t machid_processor_get_assignment
#if	defined(LINTLIBRARY)
    (server, auth, proc, pset)
	mach_port_t server;
	mach_port_t auth;
	mprocessor_t proc;
	mprocessor_set_name_t *pset;
{ return machid_processor_get_assignment(server, auth, proc, pset); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mprocessor_t proc,
	mprocessor_set_name_t *pset
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
kern_return_t machid_thread_get_assignment
#if	defined(LINTLIBRARY)
    (server, auth, thread, pset)
	mach_port_t server;
	mach_port_t auth;
	mthread_t thread;
	mprocessor_set_name_t *pset;
{ return machid_thread_get_assignment(server, auth, thread, pset); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mthread_t thread,
	mprocessor_set_name_t *pset
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
kern_return_t machid_task_get_assignment
#if	defined(LINTLIBRARY)
    (server, auth, task, pset)
	mach_port_t server;
	mach_port_t auth;
	mtask_t task;
	mprocessor_set_name_t *pset;
{ return machid_task_get_assignment(server, auth, task, pset); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mtask_t task,
	mprocessor_set_name_t *pset
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
kern_return_t machid_host_processor_set_priv
#if	defined(LINTLIBRARY)
    (server, auth, host, psetn, pset)
	mach_port_t server;
	mach_port_t auth;
	mhost_priv_t host;
	mprocessor_set_name_t psetn;
	mprocessor_set_t *pset;
{ return machid_host_processor_set_priv(server, auth, host, psetn, pset); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mhost_priv_t host,
	mprocessor_set_name_t psetn,
	mprocessor_set_t *pset
);
#else
    ();
#endif
#endif

/* Routine host_processor_set_names */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t machid_host_processor_set_names
#if	defined(LINTLIBRARY)
    (server, auth, host, sets, setsCnt)
	mach_port_t server;
	mach_port_t auth;
	mhost_t host;
	mprocessor_set_name_array_t *sets;
	mach_msg_type_number_t *setsCnt;
{ return machid_host_processor_set_names(server, auth, host, sets, setsCnt); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mhost_t host,
	mprocessor_set_name_array_t *sets,
	mach_msg_type_number_t *setsCnt
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
kern_return_t machid_processor_set_create
#if	defined(LINTLIBRARY)
    (server, auth, host, pset, psetn)
	mach_port_t server;
	mach_port_t auth;
	mhost_t host;
	mprocessor_set_t *pset;
	mprocessor_set_name_t *psetn;
{ return machid_processor_set_create(server, auth, host, pset, psetn); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mhost_t host,
	mprocessor_set_t *pset,
	mprocessor_set_name_t *psetn
);
#else
    ();
#endif
#endif

/* Routine task_create */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t machid_task_create
#if	defined(LINTLIBRARY)
    (server, auth, parent, inherit, task)
	mach_port_t server;
	mach_port_t auth;
	mtask_t parent;
	boolean_t inherit;
	mtask_t *task;
{ return machid_task_create(server, auth, parent, inherit, task); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mtask_t parent,
	boolean_t inherit,
	mtask_t *task
);
#else
    ();
#endif
#endif

/* Routine thread_create */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t machid_thread_create
#if	defined(LINTLIBRARY)
    (server, auth, task, thread)
	mach_port_t server;
	mach_port_t auth;
	mtask_t task;
	mthread_t *thread;
{ return machid_thread_create(server, auth, task, thread); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mtask_t task,
	mthread_t *thread
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
kern_return_t machid_processor_assign
#if	defined(LINTLIBRARY)
    (server, auth, processor, pset, wait)
	mach_port_t server;
	mach_port_t auth;
	mprocessor_t processor;
	mprocessor_set_t pset;
	boolean_t wait;
{ return machid_processor_assign(server, auth, processor, pset, wait); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mprocessor_t processor,
	mprocessor_set_t pset,
	boolean_t wait
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
kern_return_t machid_thread_assign
#if	defined(LINTLIBRARY)
    (server, auth, thread, pset)
	mach_port_t server;
	mach_port_t auth;
	mthread_t thread;
	mprocessor_set_t pset;
{ return machid_thread_assign(server, auth, thread, pset); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mthread_t thread,
	mprocessor_set_t pset
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
kern_return_t machid_thread_assign_default
#if	defined(LINTLIBRARY)
    (server, auth, thread)
	mach_port_t server;
	mach_port_t auth;
	mthread_t thread;
{ return machid_thread_assign_default(server, auth, thread); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mthread_t thread
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
kern_return_t machid_task_assign
#if	defined(LINTLIBRARY)
    (server, auth, task, pset, assign_threads)
	mach_port_t server;
	mach_port_t auth;
	mtask_t task;
	mprocessor_set_t pset;
	boolean_t assign_threads;
{ return machid_task_assign(server, auth, task, pset, assign_threads); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mtask_t task,
	mprocessor_set_t pset,
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
kern_return_t machid_task_assign_default
#if	defined(LINTLIBRARY)
    (server, auth, task, assign_threads)
	mach_port_t server;
	mach_port_t auth;
	mtask_t task;
	boolean_t assign_threads;
{ return machid_task_assign_default(server, auth, task, assign_threads); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mtask_t task,
	boolean_t assign_threads
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
kern_return_t machid_thread_policy
#if	defined(LINTLIBRARY)
    (server, auth, thread, policy, data)
	mach_port_t server;
	mach_port_t auth;
	mthread_t thread;
	int policy;
	int data;
{ return machid_thread_policy(server, auth, thread, policy, data); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mthread_t thread,
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
kern_return_t machid_processor_set_policy_enable
#if	defined(LINTLIBRARY)
    (server, auth, processor_set, policy)
	mach_port_t server;
	mach_port_t auth;
	mprocessor_set_t processor_set;
	int policy;
{ return machid_processor_set_policy_enable(server, auth, processor_set, policy); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mprocessor_set_t processor_set,
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
kern_return_t machid_processor_set_policy_disable
#if	defined(LINTLIBRARY)
    (server, auth, processor_set, policy, change_threads)
	mach_port_t server;
	mach_port_t auth;
	mprocessor_set_t processor_set;
	int policy;
	boolean_t change_threads;
{ return machid_processor_set_policy_disable(server, auth, processor_set, policy, change_threads); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mprocessor_set_t processor_set,
	int policy,
	boolean_t change_threads
);
#else
    ();
#endif
#endif

/* Routine host_default_pager */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t machid_host_default_pager
#if	defined(LINTLIBRARY)
    (server, auth, host, default_pager)
	mach_port_t server;
	mach_port_t auth;
	mhost_priv_t host;
	mdefault_pager_t *default_pager;
{ return machid_host_default_pager(server, auth, host, default_pager); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mhost_priv_t host,
	mdefault_pager_t *default_pager
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
kern_return_t machid_default_pager_info
#if	defined(LINTLIBRARY)
    (server, auth, default_pager, total_size, free_size)
	mach_port_t server;
	mach_port_t auth;
	mdefault_pager_t default_pager;
	vm_size_t *total_size;
	vm_size_t *free_size;
{ return machid_default_pager_info(server, auth, default_pager, total_size, free_size); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mdefault_pager_t default_pager,
	vm_size_t *total_size,
	vm_size_t *free_size
);
#else
    ();
#endif
#endif

/* Routine vm_statistics */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t machid_vm_statistics
#if	defined(LINTLIBRARY)
    (server, auth, task, data)
	mach_port_t server;
	mach_port_t auth;
	mtask_t task;
	vm_statistics_data_t *data;
{ return machid_vm_statistics(server, auth, task, data); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	mach_port_t server,
	mach_port_t auth,
	mtask_t task,
	vm_statistics_data_t *data
);
#else
    ();
#endif
#endif

#endif	_machid_user_
