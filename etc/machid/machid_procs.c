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
 * $Log:	machid_procs.c,v $
 * Revision 2.7  91/08/30  14:52:56  rpd
 * 	Moved machid include files into the standard include directory.
 * 	[91/08/29            rpd]
 * 
 * 	Added vm_statistics.
 * 	[91/08/19            rpd]
 * 	Added host_default_pager, default_pager_info.
 * 	[91/08/15            rpd]
 * 
 * Revision 2.6  91/03/27  17:26:48  mrt
 * 	Changed mach.h include
 * 
 * Revision 2.5  91/03/19  12:30:54  mrt
 * 	Changed to new copyright
 * 
 * Revision 2.4  90/10/08  13:15:40  rpd
 * 	Added thread_policy, processor_set_policy_enable,
 * 	and processor_set_policy_disable.
 * 	[90/10/07            rpd]
 * 
 * Revision 2.3  90/09/23  14:30:48  rpd
 * 	Fixed do_host_threads to use MACH_TYPE_THREAD.
 * 	[90/09/18            rpd]
 * 
 * Revision 2.2  90/09/12  16:31:53  rpd
 * 	Added processor_set_create, task_create, thread_create,
 * 	processor_assign, thread_assign, thread_assign_default,
 * 	task_assign, task_assign_default.
 * 	[90/08/31            rpd]
 * 
 * 	Created.
 * 	[90/06/18            rpd]
 * 
 */

#include <stdio.h>
#include <mach.h>
#include <mach_error.h>
#include <servers/machid_types.h>
#include "machid_internal.h"

kern_return_t
do_mach_type(server, auth, id, typep)
    mach_port_t server;
    mach_port_t auth;
    mach_id_t id;
    mach_type_t *typep;
{
    *typep = type_lookup(id);
    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_mach_register(server, auth, port, type, idp)
    mach_port_t server;
    mach_port_t auth;
    mach_port_t port;
    mach_type_t type;
    mach_id_t *idp;
{
    *idp = name_lookup(port, type);
    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_mach_lookup(server, auth, name, atype, anamep)
    mach_port_t server;
    mach_port_t auth;
    mach_id_t name;
    mach_type_t atype;
    mach_id_t *anamep;
{
    *anamep = assoc_lookup(name, atype);
    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_mach_port(server, auth, name, portp)
    mach_port_t server;
    mach_port_t auth;
    mach_id_t name;
    mach_port_t *portp;
{
    mach_port_t port;
    kern_return_t kr;

    kr = port_lookup(name, auth, mo_Port, &port);
    if (kr != KERN_SUCCESS)
	return kr;

    *portp = port;
    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_host_ports(server, auth, hostp, phostp)
    mach_port_t server;
    mach_port_t auth;
    mhost_t *hostp;
    mhost_priv_t *phostp;
{
    mhost_t host;
    mhost_priv_t phost;

    host = name_lookup(mach_host_self(), MACH_TYPE_HOST);
    phost = name_lookup(mach_host_priv_self(), MACH_TYPE_HOST_PRIV);

    assoc_create(host, MACH_TYPE_HOST_PRIV, phost);
    assoc_create(phost, MACH_TYPE_HOST, host);
    assoc_create(phost, MACH_TYPE_HOST_PRIV, phost);

    *hostp = host;
    *phostp = phost;
    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_host_processor_sets(server, auth, host, mproc_sets, mproc_setsCnt)
    mach_port_t server;
    mach_port_t auth;
    mhost_priv_t host;
    mprocessor_set_array_t *mproc_sets;
    unsigned int *mproc_setsCnt;
{
    processor_set_name_t *proc_sets;
    unsigned int proc_setsCnt, i;
    mach_port_t port;
    kern_return_t kr;

    kr = port_lookup(host, auth, mo_Info, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_HOST_PRIV;
	return kr;
    }

    kr = host_processor_sets(port, &proc_sets, &proc_setsCnt);
    if (kr != KERN_SUCCESS) {
	port_consume(port);
	return kr;
    }

    for (i = 0; i < proc_setsCnt; i++) {
	mprocessor_set_name_t mproc_set_name;
	mprocessor_set_t mproc_set;
	processor_set_t proc_set;

	kr = host_processor_set_priv(port, proc_sets[i], &proc_set);
	if (kr != KERN_SUCCESS) {
	    kern_return_t kr2;

	    port_consume(port);
	    for (; i < proc_setsCnt; i++)
		(void) name_lookup(proc_sets[i], MACH_TYPE_PROCESSOR_SET_NAME);

	    kr2 = vm_deallocate(mach_task_self(), (vm_offset_t) proc_sets,
				(vm_size_t)(proc_setsCnt * sizeof *proc_sets));
	    if (kr2 != KERN_SUCCESS)
		quit(1, "machid: vm_deallocate: %s\n", mach_error_string(kr2));

	    return kr;
	}

	mproc_set_name = name_lookup(proc_sets[i],
				     MACH_TYPE_PROCESSOR_SET_NAME);
	mproc_set = name_lookup(proc_set, MACH_TYPE_PROCESSOR_SET);

	assoc_create(mproc_set_name, MACH_TYPE_HOST_PRIV, host);
	assoc_create(mproc_set, MACH_TYPE_HOST_PRIV, host);
	assoc_create(mproc_set_name, MACH_TYPE_PROCESSOR_SET, mproc_set);
	assoc_create(mproc_set, MACH_TYPE_PROCESSOR_SET_NAME, mproc_set_name);

	proc_sets[i] = mproc_set;
    }

    *mproc_sets = proc_sets;
    *mproc_setsCnt = proc_setsCnt;
    port_consume(port);
    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_host_processor_set_names(server, auth, host, mproc_sets, mproc_setsCnt)
    mach_port_t server;
    mach_port_t auth;
    mhost_t host;
    mprocessor_set_name_array_t *mproc_sets;
    unsigned int *mproc_setsCnt;
{
    processor_set_name_t *proc_sets;
    unsigned int proc_setsCnt, i;
    mach_port_t port;
    kern_return_t kr;

    kr = port_lookup(host, auth, mo_Info, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_HOST;
	return kr;
    }

    kr = host_processor_sets(port, &proc_sets, &proc_setsCnt);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    for (i = 0; i < proc_setsCnt; i++)
	proc_sets[i] = name_lookup(proc_sets[i], MACH_TYPE_PROCESSOR_SET_NAME);

    *mproc_sets = proc_sets;
    *mproc_setsCnt = proc_setsCnt;
    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_host_tasks(server, auth, host, mtasks, mtasksCnt)
    mach_port_t server;
    mach_port_t auth;
    mhost_priv_t host;
    mtask_array_t *mtasks;
    unsigned int *mtasksCnt;
{
    processor_set_name_t *proc_set_names;
    processor_set_t *proc_sets;
    unsigned int proc_setsCnt;
    task_t **tasks_list;
    unsigned int *tasksCnts;
    unsigned int total_tasks, i, j, k;
    mtask_t *tasks;
    mach_port_t port;
    kern_return_t kr;

    kr = port_lookup(host, auth, mo_Info, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_HOST_PRIV;
	return kr;
    }

    /* get a list of the processor sets */

    kr = host_processor_sets(port, &proc_set_names, &proc_setsCnt);
    if (kr != KERN_SUCCESS) {
	port_consume(port);
	return kr;
    }

    /* allocate space for the list of privileged ports */

    proc_sets = (processor_set_t *) malloc(proc_setsCnt * sizeof *proc_sets);
    if (proc_sets == NULL)
	quit(1, "machid: malloc failed\n");

    /* convert from name ports to privileged ports */

    for (i = 0; i < proc_setsCnt; i++) {
	kr = host_processor_set_priv(port, proc_set_names[i], &proc_sets[i]);
	if (kr != KERN_SUCCESS) {
	    kern_return_t kr2;
	    int j;

	    port_consume(port);
	    for (j = 0; j < i; j++)
		(void) name_lookup(proc_sets[i], MACH_TYPE_PROCESSOR_SET);
	    for (; i < proc_setsCnt; i++)
		(void) name_lookup(proc_set_names[i],
				   MACH_TYPE_PROCESSOR_SET_NAME);

	    kr2 = vm_deallocate(mach_task_self(), (vm_offset_t) proc_set_names,
			(vm_size_t) (proc_setsCnt * sizeof *proc_set_names));
	    if (kr2 != KERN_SUCCESS)
		quit(1, "machid: vm_deallocate: %s\n", mach_error_string(kr2));

	    kr2 = vm_deallocate(mach_task_self(), (vm_offset_t) proc_sets,
			(vm_size_t) (proc_setsCnt * sizeof *proc_sets));
	    if (kr2 != KERN_SUCCESS)
		quit(1, "machid: vm_deallocate: %s\n", mach_error_string(kr2));

	    return kr;
	}

	proc_set_names[i] = name_lookup(proc_set_names[i],
					MACH_TYPE_PROCESSOR_SET_NAME);
    }

    /* no longer need the privileged host port */

    port_consume(port);

    /* allocate space for lists of the tasks in each processor set */

    tasks_list = (task_t **) malloc(proc_setsCnt * sizeof *tasks_list);
    if (tasks_list == NULL)
	quit(1, "machid: malloc failed\n");

    tasksCnts = (unsigned int *) malloc(proc_setsCnt * sizeof *tasksCnts);
    if (tasksCnts == NULL)
	quit(1, "machid: malloc failed\n");

    /* get the lists of tasks */

    for (total_tasks = i = 0; i < proc_setsCnt; i++) {
	tasks_list[i] = NULL;
	tasksCnts[i] = 0;

	if (!MACH_PORT_VALID(proc_sets[i]))
	    continue;

	kr = processor_set_tasks(proc_sets[i],
				 &tasks_list[i], &tasksCnts[i]);

	proc_sets[i] = name_lookup(proc_sets[i], MACH_TYPE_PROCESSOR_SET);
	assoc_create(proc_sets[i], MACH_TYPE_HOST_PRIV, host);
	assoc_create(proc_set_names[i], MACH_TYPE_HOST_PRIV, host);
	assoc_create(proc_sets[i], MACH_TYPE_PROCESSOR_SET_NAME,
		     proc_set_names[i]);
	assoc_create(proc_set_names[i], MACH_TYPE_PROCESSOR_SET, proc_sets[i]);

	total_tasks += tasksCnts[i];
    }

    /* we don't need the processor sets anymore */

    free((char *) proc_sets);
    kr = vm_deallocate(mach_task_self(), (vm_offset_t) proc_set_names,
		       (vm_size_t) (proc_setsCnt * sizeof *proc_set_names));
    if (kr != KERN_SUCCESS)
	quit(1, "machid: vm_deallocate: %s\n", mach_error_string(kr));

    /* get space for the combined task list */

    kr = vm_allocate(mach_task_self(), (vm_offset_t *) &tasks,
		     (vm_size_t) (total_tasks * sizeof(mtask_t)), TRUE);
    if (kr != KERN_SUCCESS)
	quit(1, "machid: vm_allocate: %s\n", mach_error_string(kr));

    for (k = i = 0; i < proc_setsCnt; i++)
	for (j = 0; j < tasksCnts[i]; j++)
	    assoc_create(tasks[k++] = name_lookup(tasks_list[i][j],
						  MACH_TYPE_TASK),
			 MACH_TYPE_HOST_PRIV, host);

    /* clean up memory */

    for (i = 0; i < proc_setsCnt; i++) {
	if (tasksCnts[i] == 0)
	    continue;

	kr = vm_deallocate(mach_task_self(), (vm_offset_t) tasks_list[i],
			   (vm_size_t) (tasksCnts[i] * sizeof(task_t)));
	if (kr != KERN_SUCCESS)
	    quit(1, "machid: vm_deallocate: %s\n", mach_error_string(kr));
    }

    free((char *) tasks_list);
    free((char *) tasksCnts);

    *mtasks = tasks;
    *mtasksCnt = total_tasks;
    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_host_threads(server, auth, host, mthreads, mthreadsCnt)
    mach_port_t server;
    mach_port_t auth;
    mhost_priv_t host;
    mthread_array_t *mthreads;
    unsigned int *mthreadsCnt;
{
    processor_set_name_t *proc_set_names;
    processor_set_t *proc_sets;
    unsigned int proc_setsCnt;
    thread_t **threads_list;
    unsigned int *threadsCnts;
    unsigned int total_threads, i, j, k;
    mthread_t *threads;
    mach_port_t port;
    kern_return_t kr;

    kr = port_lookup(host, auth, mo_Info, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_HOST_PRIV;
	return kr;
    }

    /* get a list of the processor sets */

    kr = host_processor_sets(port, &proc_set_names, &proc_setsCnt);
    if (kr != KERN_SUCCESS) {
	port_consume(port);
	return kr;
    }

    /* allocate space for the list of privileged ports */

    proc_sets = (processor_set_t *) malloc(proc_setsCnt * sizeof *proc_sets);
    if (proc_sets == NULL)
	quit(1, "machid: malloc failed\n");

    /* convert from name ports to privileged ports */

    for (i = 0; i < proc_setsCnt; i++) {
	kr = host_processor_set_priv(port, proc_set_names[i], &proc_sets[i]);
	if (kr != KERN_SUCCESS) {
	    kern_return_t kr2;
	    int j;

	    port_consume(port);
	    for (j = 0; j < i; j++)
		(void) name_lookup(proc_sets[i], MACH_TYPE_PROCESSOR_SET);
	    for (; i < proc_setsCnt; i++)
		(void) name_lookup(proc_set_names[i],
				   MACH_TYPE_PROCESSOR_SET_NAME);

	    kr2 = vm_deallocate(mach_task_self(), (vm_offset_t) proc_set_names,
			(vm_size_t) (proc_setsCnt * sizeof *proc_set_names));
	    if (kr2 != KERN_SUCCESS)
		quit(1, "machid: vm_deallocate: %s\n", mach_error_string(kr2));

	    kr2 = vm_deallocate(mach_task_self(), (vm_offset_t) proc_sets,
			(vm_size_t) (proc_setsCnt * sizeof *proc_sets));
	    if (kr2 != KERN_SUCCESS)
		quit(1, "machid: vm_deallocate: %s\n", mach_error_string(kr2));

	    return kr;
	}

	proc_set_names[i] = name_lookup(proc_set_names[i],
					MACH_TYPE_PROCESSOR_SET_NAME);
    }

    /* no longer need the privileged host port */

    port_consume(port);

    /* allocate space for lists of the threads in each processor set */

    threads_list = (thread_t **) malloc(proc_setsCnt * sizeof *threads_list);
    if (threads_list == NULL)
	quit(1, "machid: malloc failed\n");

    threadsCnts = (unsigned int *) malloc(proc_setsCnt * sizeof *threadsCnts);
    if (threadsCnts == NULL)
	quit(1, "machid: malloc failed\n");

    /* get the lists of threads */

    for (total_threads = i = 0; i < proc_setsCnt; i++) {
	threads_list[i] = NULL;
	threadsCnts[i] = 0;

	if (!MACH_PORT_VALID(proc_sets[i]))
	    continue;

	kr = processor_set_threads(proc_sets[i],
				 &threads_list[i], &threadsCnts[i]);

	proc_sets[i] = name_lookup(proc_sets[i], MACH_TYPE_PROCESSOR_SET);
	assoc_create(proc_sets[i], MACH_TYPE_HOST_PRIV, host);
	assoc_create(proc_set_names[i], MACH_TYPE_HOST_PRIV, host);
	assoc_create(proc_sets[i], MACH_TYPE_PROCESSOR_SET_NAME,
		     proc_set_names[i]);
	assoc_create(proc_set_names[i], MACH_TYPE_PROCESSOR_SET, proc_sets[i]);

	total_threads += threadsCnts[i];
    }

    /* we don't need the processor sets anymore */

    free((char *) proc_sets);
    kr = vm_deallocate(mach_task_self(), (vm_offset_t) proc_set_names,
		       (vm_size_t) (proc_setsCnt * sizeof *proc_set_names));
    if (kr != KERN_SUCCESS)
	quit(1, "machid: vm_deallocate: %s\n", mach_error_string(kr));

    /* get space for the combined thread list */

    kr = vm_allocate(mach_task_self(), (vm_offset_t *) &threads,
		     (vm_size_t) (total_threads * sizeof(mthread_t)), TRUE);
    if (kr != KERN_SUCCESS)
	quit(1, "machid: vm_allocate: %s\n", mach_error_string(kr));

    for (k = i = 0; i < proc_setsCnt; i++)
	for (j = 0; j < threadsCnts[i]; j++)
	    assoc_create(threads[k++] = name_lookup(threads_list[i][j],
						    MACH_TYPE_THREAD),
			 MACH_TYPE_HOST_PRIV, host);

    /* clean up memory */

    for (i = 0; i < proc_setsCnt; i++) {
	if (threadsCnts[i] == 0)
	    continue;

	kr = vm_deallocate(mach_task_self(), (vm_offset_t) threads_list[i],
			   (vm_size_t) (threadsCnts[i] * sizeof(thread_t)));
	if (kr != KERN_SUCCESS)
	    quit(1, "machid: vm_deallocate: %s\n", mach_error_string(kr));
    }

    free((char *) threads_list);
    free((char *) threadsCnts);

    *mthreads = threads;
    *mthreadsCnt = total_threads;
    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_host_processors(server, auth, host, procsp, procsCntp)
    mach_port_t server;
    mach_port_t auth;
    mhost_priv_t host;
    mprocessor_array_t *procsp;
    unsigned int *procsCntp;
{
    processor_t *procs;
    unsigned int procsCnt, i;
    mach_port_t port;
    kern_return_t kr;

    kr = port_lookup(host, auth, mo_Info, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_HOST_PRIV;
	return kr;
    }

    kr = host_processors(port, &procs, &procsCnt);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    for (i = 0; i < procsCnt; i++)
	assoc_create(procs[i] = name_lookup(procs[i], MACH_TYPE_PROCESSOR),
		     MACH_TYPE_HOST_PRIV, host);

    *procsp = procs;
    *procsCntp = procsCnt;
    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_task_threads(server, auth, task, threadsp, threadsCntp)
    mach_port_t server;
    mach_port_t auth;
    mtask_t task;
    mthread_array_t *threadsp;
    unsigned int *threadsCntp;
{
    mhost_priv_t host;
    thread_t *threads;
    unsigned int threadsCnt, i;
    mach_port_t port;
    kern_return_t kr;

    kr = port_lookup(task, auth, mo_Info, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_TASK;
	return kr;
    }

    kr = task_threads(port, &threads, &threadsCnt);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    /* propogate host from task to threads */
    host = assoc_lookup(task, MACH_TYPE_HOST_PRIV);

    for (i = 0; i < threadsCnt; i++) {
	mthread_t thread = name_lookup(threads[i], MACH_TYPE_THREAD);

	threads[i] = thread;
	assoc_create(thread, MACH_TYPE_TASK, task);
	assoc_create(thread, MACH_TYPE_HOST_PRIV, host);
    }

    *threadsp = threads;
    *threadsCntp = threadsCnt;
    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_processor_set_threads(server, auth, pset, threadsp, threadsCntp)
    mach_port_t server;
    mach_port_t auth;
    mprocessor_set_t pset;
    mthread_array_t *threadsp;
    unsigned int *threadsCntp;
{
    mhost_priv_t host;
    thread_t *threads;
    unsigned int threadsCnt, i;
    mach_port_t port;
    kern_return_t kr;

    kr = port_lookup(pset, auth, mo_Info, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_PROCESSOR_SET;
	return kr;
    }

    kr = processor_set_threads(port, &threads, &threadsCnt);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    /* propogate host from pset to threads */
    host = assoc_lookup(pset, MACH_TYPE_HOST_PRIV);

    for (i = 0; i < threadsCnt; i++) {
	mthread_t thread = name_lookup(threads[i], MACH_TYPE_THREAD);

	threads[i] = thread;
	assoc_create(thread, MACH_TYPE_PROCESSOR_SET, pset);
	assoc_create(thread, MACH_TYPE_HOST_PRIV, host);
    }

    *threadsp = threads;
    *threadsCntp = threadsCnt;
    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_processor_set_tasks(server, auth, pset, tasksp, tasksCntp)
    mach_port_t server;
    mach_port_t auth;
    mprocessor_set_t pset;
    mtask_array_t *tasksp;
    unsigned int *tasksCntp;
{
    mhost_priv_t host;
    task_t *tasks;
    unsigned int tasksCnt, i;
    mach_port_t port;
    kern_return_t kr;

    kr = port_lookup(pset, auth, mo_Info, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_PROCESSOR_SET;
	return kr;
    }

    kr = processor_set_tasks(port, &tasks, &tasksCnt);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    /* propogate host from pset to tasks */
    host = assoc_lookup(pset, MACH_TYPE_HOST_PRIV);

    for (i = 0; i < tasksCnt; i++) {
	mtask_t task = name_lookup(tasks[i], MACH_TYPE_TASK);

	tasks[i] = task;
	assoc_create(task, MACH_TYPE_PROCESSOR_SET, pset);
	assoc_create(task, MACH_TYPE_HOST_PRIV, host);
    }

    *tasksp = tasks;
    *tasksCntp = tasksCnt;
    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_host_basic_info(server, auth, host, infop)
    mach_port_t server;
    mach_port_t auth;
    mhost_t host;
    host_basic_info_data_t *infop;
{
    mach_port_t port;
    unsigned int count;
    kern_return_t kr;

    kr = port_lookup(host, auth, mo_Info, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_HOST;
	return kr;
    }

    count = sizeof *infop / sizeof(int);
    kr = host_info(port, HOST_BASIC_INFO, infop, &count);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_host_sched_info(server, auth, host, infop)
    mach_port_t server;
    mach_port_t auth;
    mhost_t host;
    host_sched_info_data_t *infop;
{
    mach_port_t port;
    unsigned int count;
    kern_return_t kr;

    kr = port_lookup(host, auth, mo_Info, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_HOST;
	return kr;
    }

    count = sizeof *infop / sizeof(int);
    kr = host_info(port, HOST_SCHED_INFO, infop, &count);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_host_load_info(server, auth, host, infop)
    mach_port_t server;
    mach_port_t auth;
    mhost_t host;
    host_load_info_data_t *infop;
{
    mach_port_t port;
    unsigned int count;
    kern_return_t kr;

    kr = port_lookup(host, auth, mo_Info, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_HOST;
	return kr;
    }

    count = sizeof *infop / sizeof(int);
    kr = host_info(port, HOST_LOAD_INFO, infop, &count);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_processor_set_default(server, auth, host, psetp)
    mach_port_t server;
    mach_port_t auth;
    mhost_t host;
    mprocessor_set_name_t *psetp;
{
    processor_set_name_t pset;
    mach_port_t port;
    kern_return_t kr;

    kr = port_lookup(host, auth, mo_Info, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_HOST;
	return kr;
    }

    kr = processor_set_default(port, &pset);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    *psetp = name_lookup(pset, MACH_TYPE_PROCESSOR_SET_NAME);
    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_host_kernel_version(server, auth, host, version)
    mach_port_t server;
    mach_port_t auth;
    mhost_t host;
    kernel_version_t version;
{
    mach_port_t port;
    kern_return_t kr;

    kr = port_lookup(host, auth, mo_Info, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_HOST;
	return kr;
    }

    kr = host_kernel_version(port, version);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_processor_basic_info(server, auth, proc, hostp, infop)
    mach_port_t server;
    mach_port_t auth;
    mprocessor_t proc;
    mhost_t *hostp;
    processor_basic_info_data_t *infop;
{
    host_t host;
    mach_port_t port;
    unsigned int count;
    kern_return_t kr;

    kr = port_lookup(proc, auth, mo_Info, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_PROCESSOR;
	return kr;
    }

    count = sizeof *infop / sizeof(int);
    kr = processor_info(port, PROCESSOR_BASIC_INFO, &host, infop, &count);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    assoc_create(proc, MACH_TYPE_HOST,
		 *hostp = name_lookup(host, MACH_TYPE_HOST));

    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_processor_set_basic_info(server, auth, pset, hostp, infop)
    mach_port_t server;
    mach_port_t auth;
    mprocessor_set_name_t pset;
    mhost_t *hostp;
    processor_set_basic_info_data_t *infop;
{
    host_t host;
    mach_port_t port;
    unsigned int count;
    kern_return_t kr;

    kr = port_lookup(pset, auth, mo_Info, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_PROCESSOR_SET_NAME;
	return kr;
    }

    count = sizeof *infop / sizeof(int);
    kr = processor_set_info(port, PROCESSOR_SET_BASIC_INFO,
			    &host, infop, &count);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    assoc_create(pset, MACH_TYPE_HOST,
		 *hostp = name_lookup(host, MACH_TYPE_HOST));

    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_processor_set_sched_info(server, auth, pset, hostp, infop)
    mach_port_t server;
    mach_port_t auth;
    mprocessor_set_name_t pset;
    mhost_t *hostp;
    processor_set_sched_info_data_t *infop;
{
    host_t host;
    mach_port_t port;
    unsigned int count;
    kern_return_t kr;

    kr = port_lookup(pset, auth, mo_Info, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_PROCESSOR_SET_NAME;
	return kr;
    }

    count = sizeof *infop / sizeof(int);
    kr = processor_set_info(port, PROCESSOR_SET_SCHED_INFO,
			    &host, infop, &count);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    assoc_create(pset, MACH_TYPE_HOST,
		 *hostp = name_lookup(host, MACH_TYPE_HOST));

    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_task_unix_info(server, auth, task, pidp, comm, commCntp)
    mach_port_t server;
    mach_port_t auth;
    mtask_t task;
    unix_pid_t *pidp;
    unix_command_t comm;
    unsigned int *commCntp;
{
    mach_port_t port;
    kern_return_t kr;

    kr = port_lookup(task, auth, mo_Info, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_TASK;
	return kr;
    }

    if (pid_by_task(port, pidp, comm, commCntp) != 0) {
	*pidp = 0;
	*commCntp = 0;
    }

    port_consume(port);
    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_task_basic_info(server, auth, task, infop)
    mach_port_t server;
    mach_port_t auth;
    mtask_t task;
    task_basic_info_data_t *infop;
{
    mach_port_t port;
    unsigned int count;
    kern_return_t kr;

    kr = port_lookup(task, auth, mo_Info, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_TASK;
	return kr;
    }

    count = sizeof *infop / sizeof(int);
    kr = task_info(port, TASK_BASIC_INFO, infop, &count);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_task_thread_times_info(server, auth, task, timesp)
    mach_port_t server;
    mach_port_t auth;
    mtask_t task;
    task_thread_times_info_data_t *timesp;
{
    mach_port_t port;
    unsigned int count;
    kern_return_t kr;

    kr = port_lookup(task, auth, mo_Info, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_TASK;
	return kr;
    }

    count = sizeof *timesp / sizeof(int);
    kr = task_info(port, TASK_THREAD_TIMES_INFO, timesp, &count);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_thread_basic_info(server, auth, thread, infop)
    mach_port_t server;
    mach_port_t auth;
    mthread_t thread;
    thread_basic_info_data_t *infop;
{
    mach_port_t port;
    unsigned int count;
    kern_return_t kr;

    kr = port_lookup(thread, auth, mo_Info, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_THREAD;
	return kr;
    }

    count = sizeof *infop / sizeof(int);
    kr = thread_info(port, THREAD_BASIC_INFO, infop, &count);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_thread_sched_info(server, auth, thread, infop)
    mach_port_t server;
    mach_port_t auth;
    mthread_t thread;
    thread_sched_info_data_t *infop;
{
    mach_port_t port;
    unsigned int count;
    kern_return_t kr;

    kr = port_lookup(thread, auth, mo_Info, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_THREAD;
	return kr;
    }

    count = sizeof *infop / sizeof(int);
    kr = thread_info(port, THREAD_SCHED_INFO, infop, &count);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    auth_consume(auth);
    return KERN_SUCCESS;
}

#ifdef	mips
kern_return_t
do_mips_thread_state(server, auth, thread, statep)
    mach_port_t server;
    mach_port_t auth;
    mthread_t thread;
    mips_thread_state_t *statep;
{
    mach_port_t port;
    unsigned int count;
    kern_return_t kr;

    kr = port_lookup(thread, auth, mo_Read, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_THREAD;
	return kr;
    }

    count = sizeof *statep / sizeof(int);
    kr = thread_get_state(port, MIPS_THREAD_STATE, statep, &count);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    auth_consume(auth);
    return KERN_SUCCESS;
}
#endif	mips

#ifdef	sun3
kern_return_t
do_sun3_thread_state(server, auth, thread, statep)
    mach_port_t server;
    mach_port_t auth;
    mthread_t thread;
    sun3_thread_state_t *statep;
{
    mach_port_t port;
    unsigned int count;
    kern_return_t kr;

    kr = port_lookup(thread, auth, mo_Read, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_THREAD;
	return kr;
    }

    count = sizeof *statep / sizeof(int);
    kr = thread_get_state(port, SUN_THREAD_STATE_REGS, statep, &count);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    auth_consume(auth);
    return KERN_SUCCESS;
}
#endif	sun3

#ifdef	vax
kern_return_t
do_vax_thread_state(server, auth, thread, statep)
    mach_port_t server;
    mach_port_t auth;
    mthread_t thread;
    vax_thread_state_t *statep;
{
    mach_port_t port;
    unsigned int count;
    kern_return_t kr;

    kr = port_lookup(thread, auth, mo_Read, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_THREAD;
	return kr;
    }

    count = sizeof *statep / sizeof(int);
    kr = thread_get_state(port, VAX_THREAD_STATE, statep, &count);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    auth_consume(auth);
    return KERN_SUCCESS;
}
#endif	vax

#ifdef	i386
kern_return_t
do_i386_thread_state(server, auth, thread, statep)
    mach_port_t server;
    mach_port_t auth;
    mthread_t thread;
    i386_thread_state_t *statep;
{
    mach_port_t port;
    unsigned int count;
    kern_return_t kr;

    kr = port_lookup(thread, auth, mo_Read, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_THREAD;
	return kr;
    }

    count = sizeof *statep / sizeof(int);
    kr = thread_get_state(port, i386_THREAD_STATE, statep, &count);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    auth_consume(auth);
    return KERN_SUCCESS;
}
#endif	i386

kern_return_t
do_task_terminate(server, auth, task)
    mach_port_t server;
    mach_port_t auth;
    mtask_t task;
{
    mach_port_t port;
    kern_return_t kr;

    kr = port_lookup(task, auth, mo_Write, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_TASK;
	return kr;
    }

    kr = task_terminate(port);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_task_suspend(server, auth, task)
    mach_port_t server;
    mach_port_t auth;
    mtask_t task;
{
    mach_port_t port;
    kern_return_t kr;

    kr = port_lookup(task, auth, mo_Write, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_TASK;
	return kr;
    }

    kr = task_suspend(port);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_task_resume(server, auth, task)
    mach_port_t server;
    mach_port_t auth;
    mtask_t task;
{
    mach_port_t port;
    kern_return_t kr;

    kr = port_lookup(task, auth, mo_Write, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_TASK;
	return kr;
    }

    kr = task_resume(port);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_thread_terminate(server, auth, thread)
    mach_port_t server;
    mach_port_t auth;
    mthread_t thread;
{
    mach_port_t port;
    kern_return_t kr;

    kr = port_lookup(thread, auth, mo_Write, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_THREAD;
	return kr;
    }

    kr = thread_terminate(port);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_thread_suspend(server, auth, thread)
    mach_port_t server;
    mach_port_t auth;
    mthread_t thread;
{
    mach_port_t port;
    kern_return_t kr;

    kr = port_lookup(thread, auth, mo_Write, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_THREAD;
	return kr;
    }

    kr = thread_suspend(port);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_thread_resume(server, auth, thread)
    mach_port_t server;
    mach_port_t auth;
    mthread_t thread;
{
    mach_port_t port;
    kern_return_t kr;

    kr = port_lookup(thread, auth, mo_Write, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_THREAD;
	return kr;
    }

    kr = thread_resume(port);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_thread_abort(server, auth, thread)
    mach_port_t server;
    mach_port_t auth;
    mthread_t thread;
{
    mach_port_t port;
    kern_return_t kr;

    kr = port_lookup(thread, auth, mo_Write, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_THREAD;
	return kr;
    }

    kr = thread_abort(port);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_processor_set_destroy(server, auth, pset)
    mach_port_t server;
    mach_port_t auth;
    mprocessor_set_t pset;
{
    mach_port_t port;
    kern_return_t kr;

    kr = port_lookup(pset, auth, mo_Write, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_PROCESSOR_SET;
	return kr;
    }

    kr = processor_set_destroy(port);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_processor_start(server, auth, processor)
    mach_port_t server;
    mach_port_t auth;
    mprocessor_t processor;
{
    mach_port_t port;
    kern_return_t kr;

    kr = port_lookup(processor, auth, mo_Write, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_PROCESSOR;
	return kr;
    }

    kr = processor_start(port);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_processor_exit(server, auth, processor)
    mach_port_t server;
    mach_port_t auth;
    mprocessor_t processor;
{
    mach_port_t port;
    kern_return_t kr;

    kr = port_lookup(processor, auth, mo_Write, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_PROCESSOR;
	return kr;
    }

    kr = processor_exit(port);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_vm_region(server, auth, task, addr, infop)
    mach_port_t server;
    mach_port_t auth;
    mtask_t task;
    vm_offset_t addr;
    vm_region_info_t *infop;
{
    vm_region_info_t info;
    memory_object_name_t name;
    mach_port_t port;
    kern_return_t kr;

    kr = port_lookup(task, auth, mo_Info, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_TASK;
	return kr;
    }

    kr = vm_region(port, &addr, &info.vr_size,
		   &info.vr_prot, &info.vr_max_prot, &info.vr_inherit,
		   &info.vr_shared, &name, &info.vr_offset);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    info.vr_address = addr;
    info.vr_name = name_lookup(name, MACH_TYPE_OBJECT_NAME);

    *infop = info;
    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_vm_read(server, auth, task, addr, size, datap, dataCntp)
    mach_port_t server;
    mach_port_t auth;
    mtask_t task;
    vm_offset_t addr;
    vm_size_t size;
    pointer_t *datap;
    unsigned int *dataCntp;
{
    mach_port_t port;
    kern_return_t kr;

    kr = port_lookup(task, auth, mo_Read, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_TASK;
	return kr;
    }

    kr = vm_read(port, addr, size, datap, dataCntp);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_thread_priority(server, auth, thread, priority, set_max)
    mach_port_t server;
    mach_port_t auth;
    mthread_t thread;
    int priority;
    boolean_t set_max;
{
    mach_port_t port;
    kern_return_t kr;

    kr = port_lookup(thread, auth, mo_Write, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_THREAD;
	return kr;
    }

    kr = thread_priority(port, priority, set_max);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_thread_max_priority(server, auth, thread, pset, max_pri)
    mach_port_t server;
    mach_port_t auth;
    mthread_t thread;
    mprocessor_set_t pset;
    int max_pri;
{
    mach_port_t tport, psport;
    kern_return_t kr;

    kr = port_lookup(thread, auth, mo_Write, &tport);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_THREAD;
	return kr;
    }

    kr = port_lookup(pset, auth, mo_Write, &psport);
    if (kr != KERN_SUCCESS) {
	port_consume(tport);
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_PROCESSOR_SET;
	return kr;
    }

    kr = thread_max_priority(tport, psport, max_pri);
    port_consume(tport);
    port_consume(psport);
    if (kr != KERN_SUCCESS)
	return kr;

    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_task_priority(server, auth, task, priority, change_threads)
    mach_port_t server;
    mach_port_t auth;
    mtask_t task;
    int priority;
    boolean_t change_threads;
{
    mach_port_t port;
    kern_return_t kr;

    kr = port_lookup(task, auth, mo_Write, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_TASK;
	return kr;
    }

    kr = task_priority(port, priority, change_threads);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_processor_set_max_priority(server, auth, pset, max_pri, change_threads)
    mach_port_t server;
    mach_port_t auth;
    mprocessor_set_t pset;
    int max_pri;
    boolean_t change_threads;
{
    mach_port_t port;
    kern_return_t kr;

    kr = port_lookup(pset, auth, mo_Write, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_PROCESSOR_SET;
	return kr;
    }

    kr = processor_set_max_priority(port, max_pri, change_threads);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_port_names(server, auth, task, names, namesCnt, types, typesCnt)
    mach_port_t server;
    mach_port_t auth;
    mtask_t task;
    mach_port_array_t *names;
    unsigned int *namesCnt;
    mach_port_type_array_t *types;
    unsigned int *typesCnt;
{
    mach_port_t port;
    kern_return_t kr;

    kr = port_lookup(task, auth, mo_Info, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_TASK;
	return kr;
    }

    kr = mach_port_names(port, names, namesCnt, types, typesCnt);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_port_type(server, auth, task, name, typep)
    mach_port_t server;
    mach_port_t auth;
    mtask_t task;
    mach_port_t name;
    mach_port_type_t *typep;
{
    mach_port_t port;
    kern_return_t kr;

    kr = port_lookup(task, auth, mo_Info, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_TASK;
	return kr;
    }

    kr = mach_port_type(port, name, typep);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_port_get_refs(server, auth, task, name, right, refsp)
    mach_port_t server;
    mach_port_t auth;
    mtask_t task;
    mach_port_t name;
    mach_port_right_t right;
    mach_port_urefs_t *refsp;
{
    mach_port_t port;
    kern_return_t kr;

    kr = port_lookup(task, auth, mo_Info, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_TASK;
	return kr;
    }

    kr = mach_port_get_refs(port, name, right, refsp);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_port_get_receive_status(server, auth, task, name, statusp)
    mach_port_t server;
    mach_port_t auth;
    mtask_t task;
    mach_port_t name;
    mach_port_status_t *statusp;
{
    mach_port_t port;
    kern_return_t kr;

    kr = port_lookup(task, auth, mo_Info, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_TASK;
	return kr;
    }

    kr = mach_port_get_receive_status(port, name, statusp);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_port_get_set_status(server, auth, task, name, membersp, membersCntp)
    mach_port_t server;
    mach_port_t auth;
    mtask_t task;
    mach_port_t name;
    mach_port_array_t *membersp;
    unsigned int *membersCntp;
{
    mach_port_t port;
    kern_return_t kr;

    kr = port_lookup(task, auth, mo_Info, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_TASK;
	return kr;
    }

    kr = mach_port_get_set_status(port, name, membersp, membersCntp);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_processor_get_assignment(server, auth, proc, psetp)
    mach_port_t server;
    mach_port_t auth;
    mprocessor_t proc;
    mprocessor_set_name_t *psetp;
{
    processor_set_name_t pset;
    mach_port_t port;
    kern_return_t kr;

    kr = port_lookup(proc, auth, mo_Info, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_PROCESSOR;
	return kr;
    }

    kr = processor_get_assignment(port, &pset);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    assoc_create(proc, MACH_TYPE_PROCESSOR_SET_NAME,
		 *psetp = name_lookup(pset, MACH_TYPE_PROCESSOR_SET_NAME));

    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_thread_get_assignment(server, auth, thread, psetp)
    mach_port_t server;
    mach_port_t auth;
    mthread_t thread;
    mprocessor_set_name_t *psetp;
{
    processor_set_name_t pset;
    mach_port_t port;
    kern_return_t kr;

    kr = port_lookup(thread, auth, mo_Info, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_THREAD;
	return kr;
    }

    kr = thread_get_assignment(port, &pset);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    assoc_create(thread, MACH_TYPE_PROCESSOR_SET_NAME,
		 *psetp = name_lookup(pset, MACH_TYPE_PROCESSOR_SET_NAME));

    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_task_get_assignment(server, auth, task, psetp)
    mach_port_t server;
    mach_port_t auth;
    mtask_t task;
    mprocessor_set_name_t *psetp;
{
    processor_set_name_t pset;
    mach_port_t port;
    kern_return_t kr;

    kr = port_lookup(task, auth, mo_Info, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_TASK;
	return kr;
    }

    kr = task_get_assignment(port, &pset);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    assoc_create(task, MACH_TYPE_PROCESSOR_SET_NAME,
		 *psetp = name_lookup(pset, MACH_TYPE_PROCESSOR_SET_NAME));

    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_host_processor_set_priv(server, auth, host, psetn, psetp)
    mach_port_t server;
    mach_port_t auth;
    mhost_priv_t host;
    mprocessor_set_name_t psetn;
    mprocessor_set_t *psetp;
{
    mprocessor_set_t name;
    processor_set_t pset;
    mach_port_t hport, psport;
    kern_return_t kr;

    kr = port_lookup(host, auth, mo_Info, &hport);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_HOST_PRIV;
	return kr;
    }

    kr = port_lookup(psetn, auth, mo_Info, &psport);
    if (kr != KERN_SUCCESS) {
	port_consume(hport);
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_PROCESSOR_SET_NAME;
	return kr;
    }

    kr = host_processor_set_priv(hport, psport, &pset);
    port_consume(hport);
    port_consume(psport);
    if (kr != KERN_SUCCESS)
	return kr;

    name = name_lookup(pset, MACH_TYPE_PROCESSOR_SET);
    assoc_create(psetn, MACH_TYPE_HOST_PRIV, host);
    assoc_create(name, MACH_TYPE_HOST_PRIV, host);
    assoc_create(psetn, MACH_TYPE_PROCESSOR_SET, name);
    assoc_create(name, MACH_TYPE_PROCESSOR_SET_NAME, psetn);

    *psetp = name;
    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_processor_set_create(server, auth, host, psetp, psetnp)
    mach_port_t server;
    mach_port_t auth;
    mhost_t host;
    mprocessor_set_t *psetp;
    mprocessor_set_name_t *psetnp;
{
    processor_set_t pset;
    processor_set_name_t psetn;
    mach_port_t port;
    kern_return_t kr;

    kr = port_lookup(host, auth, mo_Write, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_HOST;
	return kr;
    }

    kr = processor_set_create(port, &pset, &psetn);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    assoc_create(*psetp = name_lookup(pset, MACH_TYPE_PROCESSOR_SET),
		 MACH_TYPE_HOST_PRIV,
		 assoc_lookup(host, MACH_TYPE_HOST_PRIV));
    assoc_create(*psetnp = name_lookup(psetn, MACH_TYPE_PROCESSOR_SET_NAME),
		 MACH_TYPE_HOST_PRIV,
		 assoc_lookup(host, MACH_TYPE_HOST_PRIV));

    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_task_create(server, auth, parent, inherit, taskp)
    mach_port_t server;
    mach_port_t auth;
    mtask_t parent;
    boolean_t inherit;
    mtask_t *taskp;
{
    task_t task;
    mach_port_t port;
    kern_return_t kr;

    kr = port_lookup(parent, auth, mo_Write, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_TASK;
	return kr;
    }

    kr = task_create(port, inherit, &task);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    assoc_create(*taskp = name_lookup(task, MACH_TYPE_TASK),
		 MACH_TYPE_HOST_PRIV,
		 assoc_lookup(parent, MACH_TYPE_HOST_PRIV));

    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_thread_create(server, auth, task, threadp)
    mach_port_t server;
    mach_port_t auth;
    mtask_t task;
    mthread_t *threadp;
{
    thread_t thread;
    mach_port_t port;
    kern_return_t kr;

    kr = port_lookup(task, auth, mo_Write, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_TASK;
	return kr;
    }

    kr = thread_create(port, &thread);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    assoc_create(*threadp = name_lookup(thread, MACH_TYPE_THREAD),
		 MACH_TYPE_HOST_PRIV,
		 assoc_lookup(task, MACH_TYPE_HOST_PRIV));

    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_processor_assign(server, auth, processor, pset, wait)
    mach_port_t server;
    mach_port_t auth;
    mprocessor_t processor;
    mprocessor_set_t pset;
    boolean_t wait;
{
    mach_port_t pport, psport;
    kern_return_t kr;

    kr = port_lookup(processor, auth, mo_Write, &pport);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_PROCESSOR;
	return kr;
    }

    kr = port_lookup(pset, auth, mo_Write, &psport);
    if (kr != KERN_SUCCESS) {
	port_consume(pport);
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_PROCESSOR_SET;
	return kr;
    }

    kr = processor_assign(pport, psport, wait);
    port_consume(pport);
    port_consume(psport);
    if (kr != KERN_SUCCESS)
	return kr;

    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_thread_assign(server, auth, thread, pset)
    mach_port_t server;
    mach_port_t auth;
    mthread_t thread;
    mprocessor_set_t pset;
{
    mach_port_t thport, psport;
    kern_return_t kr;

    kr = port_lookup(thread, auth, mo_Write, &thport);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_THREAD;
	return kr;
    }

    kr = port_lookup(pset, auth, mo_Write, &psport);
    if (kr != KERN_SUCCESS) {
	port_consume(thport);
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_PROCESSOR_SET;
	return kr;
    }

    kr = thread_assign(thport, psport);
    port_consume(thport);
    port_consume(psport);
    if (kr != KERN_SUCCESS)
	return kr;

    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_thread_assign_default(server, auth, thread)
    mach_port_t server;
    mach_port_t auth;
    mthread_t thread;
{
    mach_port_t thport;
    kern_return_t kr;

    kr = port_lookup(thread, auth, mo_Write, &thport);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_THREAD;
	return kr;
    }

    kr = thread_assign_default(thport);
    port_consume(thport);
    if (kr != KERN_SUCCESS)
	return kr;

    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_task_assign(server, auth, task, pset, assign_threads)
    mach_port_t server;
    mach_port_t auth;
    mtask_t task;
    mprocessor_set_t pset;
    boolean_t assign_threads;
{
    mach_port_t tport, psport;
    kern_return_t kr;

    kr = port_lookup(task, auth, mo_Write, &tport);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_TASK;
	return kr;
    }

    kr = port_lookup(pset, auth, mo_Write, &psport);
    if (kr != KERN_SUCCESS) {
	port_consume(tport);
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_PROCESSOR_SET;
	return kr;
    }

    kr = task_assign(tport, psport, assign_threads);
    port_consume(tport);
    port_consume(psport);
    if (kr != KERN_SUCCESS)
	return kr;

    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_task_assign_default(server, auth, task, assign_threads)
    mach_port_t server;
    mach_port_t auth;
    mtask_t task;
    boolean_t assign_threads;
{
    mach_port_t tport;
    kern_return_t kr;

    kr = port_lookup(task, auth, mo_Write, &tport);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_TASK;
	return kr;
    }

    kr = task_assign_default(tport, assign_threads);
    port_consume(tport);
    if (kr != KERN_SUCCESS)
	return kr;

    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_thread_policy(server, auth, thread, policy, data)
    mach_port_t server;
    mach_port_t auth;
    mthread_t thread;
    int policy;
    int data;
{
    mach_port_t port;
    kern_return_t kr;

    kr = port_lookup(thread, auth, mo_Write, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_THREAD;
	return kr;
    }

    kr = thread_policy(port, policy, data);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_processor_set_policy_enable(server, auth, pset, policy)
    mach_port_t server;
    mach_port_t auth;
    mprocessor_set_t pset;
    int policy;
{
    mach_port_t port;
    kern_return_t kr;

    kr = port_lookup(pset, auth, mo_Write, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_PROCESSOR_SET;
	return kr;
    }

    kr = processor_set_policy_enable(port, policy);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_processor_set_policy_disable(server, auth, pset, policy, change_threads)
    mach_port_t server;
    mach_port_t auth;
    mprocessor_set_t pset;
    int policy;
    boolean_t change_threads;
{
    mach_port_t port;
    kern_return_t kr;

    kr = port_lookup(pset, auth, mo_Write, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_PROCESSOR_SET;
	return kr;
    }

    kr = processor_set_policy_disable(port, policy, change_threads);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_host_default_pager(server, auth, host, default_pagerp)
    mach_port_t server;
    mach_port_t auth;
    mhost_priv_t host;
    mdefault_pager_t *default_pagerp;
{
    mach_port_t default_pager_port;
    mach_port_t port;
    kern_return_t kr;

    kr = port_lookup(host, auth, mo_Info, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_HOST_PRIV;
	return kr;
    }

    default_pager_port = MACH_PORT_NULL;
    kr = vm_set_default_memory_manager(port, &default_pager_port);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    *default_pagerp = name_lookup(default_pager_port, MACH_TYPE_DEFAULT_PAGER);
    assoc_create(*default_pagerp, MACH_TYPE_HOST_PRIV, host);
    assoc_create(host, MACH_TYPE_DEFAULT_PAGER, *default_pagerp);
    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_default_pager_info(server, auth, default_pager, totalp, freep)
    mach_port_t server;
    mach_port_t auth;
    mdefault_pager_t default_pager;
    vm_size_t *totalp;
    vm_size_t *freep;
{
    vm_size_t total, free;
    mach_port_t port;
    kern_return_t kr;

    kr = port_lookup(default_pager, auth, mo_Info, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_DEFAULT_PAGER;
	return kr;
    }

    kr = default_pager_info(port, &total, &free);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    *totalp = total;
    *freep = free;
    auth_consume(auth);
    return KERN_SUCCESS;
}

kern_return_t
do_vm_statistics(server, auth, task, datap)
    mach_port_t server;
    mach_port_t auth;
    mtask_t task;
    vm_statistics_data_t *datap;
{
    vm_statistics_data_t data;
    mach_port_t port;
    kern_return_t kr;

    kr = port_lookup(task, auth, mo_Info, &port);
    if (kr != KERN_SUCCESS) {
	if (kr == KERN_INVALID_NAME)
	    kr = KERN_INVALID_TASK;
	return kr;
    }

    kr = vm_statistics(port, &data);
    port_consume(port);
    if (kr != KERN_SUCCESS)
	return kr;

    *datap = data;
    auth_consume(auth);
    return KERN_SUCCESS;
}
