/* 
 * Mach Operating System
 * Copyright (c) 1991,1990,1989,1988 Carnegie Mellon University
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
 * $Log:	processor.c,v $
 * Revision 2.9  91/05/14  16:45:27  mrt
 * 	Correcting copyright
 * 
 * Revision 2.8  91/05/08  12:48:02  dbg
 * 	Changed pset_sys_init to give each processor a control port,
 * 	even when it is not running.  Without a control port, there is
 * 	no way to start an inactive processor.
 * 	[91/04/26  14:42:59  dbg]
 * 
 * Revision 2.7  91/02/05  17:28:27  mrt
 * 	Changed to new Mach copyright
 * 	[91/02/01  16:15:59  mrt]
 * 
 * Revision 2.6  90/09/09  14:32:26  rpd
 * 	Removed pset_is_garbage, do_pset_scan.
 * 	Changed processor_set_create to return the actual processor set.
 * 	[90/08/30            rpd]
 * 
 * Revision 2.5  90/08/27  22:03:17  dbg
 * 	Fixed processor_set_info to return the correct count.
 * 	[90/08/23            rpd]
 * 
 * Revision 2.4  90/08/27  11:52:07  dbg
 * 	Fix type mismatch in processor_set_create.
 * 	[90/07/18            dbg]
 * 
 * Revision 2.3  90/06/19  22:59:17  rpd
 * 	Fixed bug in processor_set_things.
 * 	[90/06/14            rpd]
 * 
 * Revision 2.2  90/06/02  14:55:32  rpd
 * 	Created for new host/processor technology.
 * 	[90/03/26  23:49:46  rpd]
 * 
 * 	Move includes
 * 	[89/08/02            dlb]
 * 	Eliminate interrupt protection for pset locks.
 * 	Add init for quantum_adj_lock.
 * 	[89/06/14            dlb]
 * 
 * 	Add processor_set_{tasks,threads}.  Use common internals.
 * 	Maintain all_psets_count for host_processor_sets.
 * 	[89/06/09            dlb]
 * 
 * 	Add processor_set_policy, sched flavor of processor_set_info.
 * 	[89/05/18            dlb]
 * 
 * 	Add processor_set_max_priority.
 * 	[89/05/12            dlb]
 * 
 * 	Add wait argument to processor_assign call.
 * 	[89/05/10            dlb]
 * 	Move processor reassignment to processor_set_destroy from
 * 	pset_deallocate.
 * 	[89/03/13            dlb]
 * 
 * 	Fix interrupt protection in processor_set_{create,destroy}.
 * 	[89/03/09            dlb]
 * 	Remove reference count decrements from pset_remove_{task,thread}.
 * 	Callers must explicitly call pset_deallocate().
 * 	[89/02/17            dlb]
 * 	Add load factor/average inits.  Make info available to users.
 * 	[89/02/09            dlb]
 * 
 * 	24-Sep-1988  David Black (dlb) at Carnegie-Mellon University
 * 
 * Revision 2.5.2.2  90/02/22  23:20:24  rpd
 * 	Changed to use kalloc/kfree instead of ipc_kernel_map.
 * 	Fixed calls to convert_task_to_port/convert_thread_to_port.
 * 
 * Revision 2.5.2.1  90/02/20  22:21:47  rpd
 * 	Revised for new IPC.
 * 	[90/02/19  23:36:11  rpd]
 * 
 * Revision 2.5  89/12/22  15:52:54  rpd
 * 	Changes to implement pset garbage collection:
 * 	1.  Add pset_is_garbage to detect abandoned processor sets.
 * 		Add do_pset_scan to look for them.
 * 	2.  Pass back the actual ports from processor_set_create, so they
 * 	        will always have extra references; this way a newly created
 * 		processor set never looks like garbage.
 * 
 * 	Also optimized processor_set_destroy.
 * 	[89/12/15            dlb]
 * 	Put all fixed priority support code under MACH_FIXPRI switch.
 * 	Add thread_change_psets for use by thread_assign.
 * 	[89/11/10            dlb]
 * 	Check for null processor set in pset_deallocate.
 * 	[89/11/06            dlb]
 * 
 * Revision 2.4  89/11/20  11:23:45  mja
 * 	Put all fixed priority support code under MACH_FIXPRI switch.
 * 	Add thread_change_psets for use by thread_assign.
 * 	[89/11/10            dlb]
 * 	Check for null processor set in pset_deallocate.
 * 	[89/11/06            dlb]
 * 
 * Revision 2.3  89/10/15  02:05:04  rpd
 * 	Minor cleanups.
 * 
 * Revision 2.2  89/10/11  14:20:11  dlb
 * 	Add processor_set_{tasks,threads}.  Use common internals.
 * 	Maintain all_psets_count for host_processor_sets.
 * 	Add processor_set_policy, sched flavor of processor_set_info.
 * 	Add processor_set_max_priority.
 * 	Remove reference count decrements from pset_remove_{task,thread}.
 * 	Callers must explicitly call pset_deallocate().
 * 	Add load factor/average inits.  Make info available to users.
 * 
 *	Created
 */

/*
 *	processor.c: processor and processor_set manipulation routines.
 */

#include <cpus.h>
#include <mach_fixpri.h>
#include <mach_host.h>

#include <mach/boolean.h>
#include <mach/policy.h>
#include <mach/processor_info.h>
#include <mach/vm_param.h>
#include <kern/cpu_number.h>
#include <kern/lock.h>
#include <kern/host.h>
#include <kern/processor.h>
#include <kern/sched.h>
#include <kern/thread.h>
#include <kern/ipc_host.h>
#include <ipc/ipc_port.h>

#if	MACH_HOST
#include <kern/zalloc.h>
zone_t	pset_zone;
#endif	MACH_HOST

/*
 *	Exported variables.
 */
struct processor_set default_pset;
struct processor processor_array[NCPUS];

queue_head_t		all_psets;
int			all_psets_count;
decl_simple_lock_data(, all_psets_lock);

processor_t	master_processor;
processor_t	processor_ptr[NCPUS];

/*
 *	Bootstrap the processor/pset system so the scheduler can run.
 */
void pset_sys_bootstrap()
{
	register int	i;
	void pset_init(), processor_init();

	pset_init(&default_pset);
	default_pset.empty = FALSE;
	for (i = 0; i < NCPUS; i++) {
		/*
		 *	Initialize processor data structures.
		 *	Note that cpu_to_processor(i) is processor_ptr[i].
		 */
		processor_ptr[i] = &processor_array[i];
		processor_init(processor_ptr[i], i);
	}
	master_processor = cpu_to_processor(master_cpu);
	queue_init(&all_psets);
	simple_lock_init(&all_psets_lock);
	queue_enter(&all_psets, &default_pset, processor_set_t, all_psets);
	all_psets_count = 1;
	default_pset.active = TRUE;
	default_pset.empty = FALSE;

	/*
	 *	Note: the default_pset has a max_priority of BASEPRI_USER.
	 *	Internal kernel threads override this in kernel_thread.
	 */
}

#if	MACH_HOST
/*
 *	Rest of pset system initializations.
 */
void pset_sys_init()
{
	register int	i;
	register processor_t	processor;

	/*
	 * Allocate the zone for processor sets.
	 */
	pset_zone = zinit(sizeof(struct processor_set), 128*PAGE_SIZE,
		PAGE_SIZE, FALSE, "processor sets");

	/*
	 * Give each processor a control port.
	 * The master processor already has one.
	 */
	for (i = 0; i < NCPUS; i++) {
	    processor = cpu_to_processor(i);
	    if (processor != master_processor &&
		machine_slot[i].is_cpu)
	    {
		ipc_processor_init(processor);
		ipc_processor_enable(processor);
	    }
	}
}
#endif	MACH_HOST

/*
 *	Initialize the given processor_set structure.
 */

void pset_init(pset)
register processor_set_t	pset;
{
	int	i;

	simple_lock_init(&pset->runq.lock);
	pset->runq.low = 0;
	pset->runq.count = 0;
	for (i = 0; i < NRQS; i++) {
	    queue_init(&(pset->runq.runq[i]));
	}
	queue_init(&pset->idle_queue);
	pset->idle_count = 0;
	simple_lock_init(&pset->idle_lock);
	queue_init(&pset->processors);
	pset->processor_count = 0;
	pset->empty = TRUE;
	queue_init(&pset->tasks);
	pset->task_count = 0;
	queue_init(&pset->threads);
	pset->thread_count = 0;
	pset->ref_count = 1;
	queue_init(&pset->all_psets);
	pset->active = FALSE;
	simple_lock_init(&pset->lock);
	pset->pset_self = IP_NULL;
	pset->pset_name_self = IP_NULL;
	pset->max_priority = BASEPRI_USER;
#if	MACH_FIXPRI
	pset->policies = POLICY_TIMESHARE;
#endif	MACH_FIXPRI
	pset->set_quantum = min_quantum;
#if	NCPUS > 1
	pset->quantum_adj_index = 0;
	simple_lock_init(&pset->quantum_adj_lock);

	for (i = 0; i <= NCPUS; i++) {
	    pset->machine_quantum[i] = min_quantum;
	}
#endif	NCPUS > 1
	pset->mach_factor = 0;
	pset->load_average = 0;
	pset->sched_load = SCHED_SCALE;		/* i.e. 1 */
}

/*
 *	Initialize the given processor structure for the processor in
 *	the slot specified by slot_num.
 */

void processor_init(pr, slot_num)
register processor_t	pr;
int	slot_num;
{
	int	i;

	simple_lock_init(&pr->runq.lock);
	pr->runq.low = 0;
	pr->runq.count = 0;
	for (i = 0; i < NRQS; i++) {
	    queue_init(&(pr->runq.runq[i]));
	}
	queue_init(&pr->processor_queue);
	pr->state = PROCESSOR_OFF_LINE;
	pr->next_thread = THREAD_NULL;
	pr->idle_thread = THREAD_NULL;
	pr->quantum = 0;
	pr->first_quantum = FALSE;
	pr->last_quantum = 0;
	pr->processor_set = PROCESSOR_SET_NULL;
	pr->processor_set_next = PROCESSOR_SET_NULL;
	queue_init(&pr->processors);
	simple_lock_init(&pr->lock);
	pr->processor_self = IP_NULL;
	pr->slot_num = slot_num;
}

/*
 *	pset_remove_processor() removes a processor from a processor_set.
 *	It can only be called on the current processor.  Caller must
 *	hold lock on current processor and processor set.
 */

pset_remove_processor(pset, processor)
processor_set_t	pset;
processor_t	processor;
{
	if (pset != processor->processor_set)
		panic("pset_remove_processor: wrong pset");

	queue_remove(&pset->processors, processor, processor_t, processors);
	processor->processor_set = PROCESSOR_SET_NULL;
	pset->processor_count--;
	quantum_set(pset);
}

/*
 *	pset_add_processor() adds a  processor to a processor_set.
 *	It can only be called on the current processor.  Caller must
 *	hold lock on curent processor and on pset.  No reference counting on
 *	processors.  Processor reference to pset is implicit.
 */

pset_add_processor(pset, processor)
processor_set_t	pset;
processor_t	processor;
{
	queue_enter(&pset->processors, processor, processor_t, processors);
	processor->processor_set = pset;
	pset->processor_count++;
	quantum_set(pset);
}

/*
 *	pset_remove_task() removes a task from a processor_set.
 *	Caller must hold locks on pset and task.  Pset reference count
 *	is not decremented; caller must explicitly pset_deallocate.
 */

pset_remove_task(pset, task)
processor_set_t	pset;
task_t	task;
{
	if (pset != task->processor_set)
		return;

	queue_remove(&pset->tasks, task, task_t, pset_tasks);
	task->processor_set = PROCESSOR_SET_NULL;
	pset->task_count--;
}

/*
 *	pset_add_task() adds a  task to a processor_set.
 *	Caller must hold locks on pset and task.  Pset references to
 *	tasks are implicit.
 */

pset_add_task(pset, task)
processor_set_t	pset;
task_t	task;
{
	queue_enter(&pset->tasks, task, task_t, pset_tasks);
	task->processor_set = pset;
	pset->task_count++;
	pset->ref_count++;
}

/*
 *	pset_remove_thread() removes a thread from a processor_set.
 *	Caller must hold locks on pset and thread.  Pset reference count
 *	is not decremented; caller must explicitly pset_deallocate.
 */

pset_remove_thread(pset, thread)
processor_set_t	pset;
thread_t	thread;
{
	queue_remove(&pset->threads, thread, thread_t, pset_threads);
	thread->processor_set = PROCESSOR_SET_NULL;
	pset->thread_count--;
}

/*
 *	pset_add_thread() adds a  thread to a processor_set.
 *	Caller must hold locks on pset and thread.  Pset references to
 *	threads are implicit.
 */

pset_add_thread(pset, thread)
processor_set_t	pset;
thread_t	thread;
{
	queue_enter(&pset->threads, thread, thread_t, pset_threads);
	thread->processor_set = pset;
	pset->thread_count++;
	pset->ref_count++;
}

/*
 *	thread_change_psets() changes the pset of a thread.  Caller must
 *	hold locks on both psets and thread.  The old pset must be
 *	explicitly pset_deallocat()'ed by caller.
 */

thread_change_psets(thread, old_pset, new_pset)
thread_t	thread;
processor_set_t old_pset, new_pset;
{
	queue_remove(&old_pset->threads, thread, thread_t, pset_threads);
	old_pset->thread_count--;
	queue_enter(&new_pset->threads, thread, thread_t, pset_threads);
	thread->processor_set = new_pset;
	new_pset->thread_count++;
	new_pset->ref_count++;
}	

/*
 *	pset_deallocate:
 *
 *	Remove one reference to the processor set.  Destroy processor_set
 *	if this was the last reference.
 */
pset_deallocate(pset)
processor_set_t	pset;
{
	if (pset == PROCESSOR_SET_NULL)
		return;

	pset_lock(pset);
	if (--pset->ref_count > 0) {
		pset_unlock(pset);
		return;
	}
#if	!MACH_HOST
	panic("pset_deallocate: default_pset destroyed");
#endif	!MACH_HOST
#if	MACH_HOST
	/*
	 *	Reference count is zero, however the all_psets list
	 *	holds an implicit reference and may make new ones.
	 *	Its lock also dominates the pset lock.  To check for this,
	 *	temporarily restore one reference, and then lock the
	 *	other structures in the right order.
	 */
	pset->ref_count = 1;
	pset_unlock(pset);
	
	simple_lock(&all_psets_lock);
	pset_lock(pset);
	if (--pset->ref_count > 0) {
		/*
		 *	Made an extra reference.
		 */
		pset_unlock(pset);
		simple_unlock(&all_psets_lock);
		return;
	}

	/*
	 *	Ok to destroy pset.  Make a few paranoia checks.
	 */

	if ((pset == &default_pset) || (pset->thread_count > 0) ||
	    (pset->task_count > 0) || pset->processor_count > 0) {
		panic("pset_deallocate: destroy default or active pset");
	}
	/*
	 *	Remove from all_psets queue.
	 */
	queue_remove(&all_psets, pset, processor_set_t, all_psets);
	all_psets_count--;

	pset_unlock(pset);
	simple_unlock(&all_psets_lock);

	/*
	 *	That's it, free data structure.
	 */
	zfree(pset_zone, pset);
#endif	MACH_HOST
}

/*
 *	pset_reference:
 *
 *	Add one reference to the processor set.
 */
pset_reference(pset)
processor_set_t	pset;
{
	pset_lock(pset);
	pset->ref_count++;
	pset_unlock(pset);
}

kern_return_t processor_info(processor, flavor, host, info, count)
register processor_t	processor;
int			flavor;
host_t			*host;
processor_info_t	info;
unsigned int		*count;
{
	register int	slot_num, state;
	register processor_basic_info_t		basic_info;

	if (processor == PROCESSOR_NULL)
		return(KERN_INVALID_ARGUMENT);

	if (flavor != PROCESSOR_BASIC_INFO ||
		*count < PROCESSOR_BASIC_INFO_COUNT)
			return(KERN_FAILURE);

	basic_info = (processor_basic_info_t) info;

	slot_num = processor->slot_num;
	basic_info->cpu_type = machine_slot[slot_num].cpu_type;
	basic_info->cpu_subtype = machine_slot[slot_num].cpu_subtype;
	state = processor->state;
	if (state == PROCESSOR_SHUTDOWN || state == PROCESSOR_OFF_LINE)
		basic_info->running = FALSE;
	else
		basic_info->running = TRUE;
	basic_info->slot_num = slot_num;
	if (processor == master_processor) 
		basic_info->is_master = TRUE;
	else
		basic_info->is_master = FALSE;

	*count = PROCESSOR_BASIC_INFO_COUNT;
	*host = &realhost;
	return(KERN_SUCCESS);
}

kern_return_t processor_start(processor)
processor_t	processor;
{
    	if (processor == PROCESSOR_NULL)
		return(KERN_INVALID_ARGUMENT);
#if	NCPUS > 1
	return(cpu_start(processor->slot_num));
#else	NCPUS > 1
	return(KERN_FAILURE);
#endif	NCPUS > 1
}

kern_return_t processor_exit(processor)
processor_t	processor;
{
	if (processor == PROCESSOR_NULL)
		return(KERN_INVALID_ARGUMENT);

#if	NCPUS > 1
	return(processor_shutdown(processor));
#else	NCPUS > 1
	return(KERN_FAILURE);
#endif	NCPUS > 1
}

kern_return_t processor_control(processor, info, count)
processor_t		processor;
processor_info_t	info;
long			*count;
{
	if (processor == PROCESSOR_NULL)
		return(KERN_INVALID_ARGUMENT);

#if	NCPUS > 1
	return(cpu_control(processor->slot_num, (int *)info, *count));
#else	NCPUS > 1
	return(KERN_FAILURE);
#endif	NCPUS > 1
}

/*
 *	Precalculate the appropriate system quanta based on load.  The
 *	index into machine_quantum is the number of threads on the
 *	processor set queue.  It is limited to the number of processors in
 *	the set.
 */

quantum_set(pset)
processor_set_t	pset;
{
#if	NCPUS > 1
	register int	i,ncpus;

	ncpus = pset->processor_count;

	for ( i=1 ; i <= ncpus ; i++) {
		pset->machine_quantum[i] =
			((min_quantum * ncpus) + (i/2)) / i ;
	}
	pset->machine_quantum[0] = 2 * pset->machine_quantum[1];

	i = ((pset->runq.count > pset->processor_count) ?
		pset->processor_count : pset->runq.count);
	pset->set_quantum = pset->machine_quantum[i];
#else	NCPUS > 1
	default_pset.set_quantum = min_quantum;
#endif	NCPUS > 1
}

#if	MACH_HOST
/*
 *	processor_set_create:
 *
 *	Create and return a new processor set.
 */

kern_return_t	processor_set_create(host, new_set, new_name)
host_t	host;
processor_set_t *new_set, *new_name;
{
	processor_set_t	pset;

	if (host == HOST_NULL)
		return(KERN_INVALID_ARGUMENT);
	pset = (processor_set_t) zalloc(pset_zone);
	pset_init(pset);
	pset_reference(pset);	/* for new_set out argument */
	pset_reference(pset);	/* for new_name out argument */
	ipc_pset_init(pset);
	pset->active = TRUE;

	simple_lock(&all_psets_lock);
	queue_enter(&all_psets, pset, processor_set_t, all_psets);
	all_psets_count++;
	simple_unlock(&all_psets_lock);

	ipc_pset_enable(pset);

	*new_set = pset;
	*new_name = pset;
	return(KERN_SUCCESS);
}

/*
 *	processor_set_destroy:
 *
 *	destroy a processor set.  Any tasks, threads or processors
 *	currently assigned to it are reassigned to the default pset.
 */
kern_return_t processor_set_destroy(pset)
processor_set_t	pset;
{
	register queue_entry_t	elem;
	register queue_head_t	*list;

	if (pset == PROCESSOR_SET_NULL || pset == &default_pset)
		return(KERN_INVALID_ARGUMENT);

	/*
	 *	Handle multiple termination race.  First one through sets
	 *	active to FALSE and disables ipc access.
	 */
	pset_lock(pset);
	if (!(pset->active)) {
		pset_unlock(pset);
		return(KERN_FAILURE);
	}

	pset->active = FALSE;
	ipc_pset_disable(pset);


	/*
	 *	Now reassign everything in this set to the default set.
	 */

	if (pset->task_count > 0) {
	    list = &pset->tasks;
	    while (!queue_empty(list)) {
		elem = queue_first(list);
		task_reference((task_t) elem);
		pset_unlock(pset);
		task_assign((task_t) elem, &default_pset, FALSE);
		task_deallocate((task_t) elem);
		pset_lock(pset);
	    }
	}

	if (pset->thread_count > 0) {
	    list = &pset->threads;
	    while (!queue_empty(list)) {
		elem = queue_first(list);
		thread_reference((thread_t) elem);
		pset_unlock(pset);
		thread_assign((thread_t) elem, &default_pset);
		thread_deallocate((thread_t) elem);
		pset_lock(pset);
	    }
	}
	
	if (pset->processor_count > 0) {
	    list = &pset->processors;
	    while(!queue_empty(list)) {
		elem = queue_first(list);
		pset_unlock(pset);
		processor_assign((processor_t) elem, &default_pset, TRUE);
		pset_lock(pset);
	    }
	}

	pset_unlock(pset);

	/*
	 *	Destroy ipc state.
	 */
	ipc_pset_terminate(pset);

	/*
	 *	Deallocate pset's reference to itself.
	 */
	pset_deallocate(pset);
	return(KERN_SUCCESS);
}

#else	MACH_HOST
	    
kern_return_t	processor_set_create(host, new_set, new_name)
host_t	host;
processor_set_t	*new_set, *new_name;
{
#ifdef	lint
	host++; new_set++; new_name++;
#endif	lint
	return(KERN_FAILURE);
}

kern_return_t processor_set_destroy(pset)
processor_set_t	pset;
{
#ifdef	lint
	pset++;
#endif	lint
	return(KERN_FAILURE);
}

#endif	MACH_HOST

kern_return_t processor_get_assignment(processor, pset)
processor_t	processor;
processor_set_t	*pset;
{
    	int state;

	state = processor->state;
	if (state == PROCESSOR_SHUTDOWN || state == PROCESSOR_OFF_LINE)
		return(KERN_FAILURE);

	*pset = processor->processor_set;
	pset_reference(*pset);
	return(KERN_SUCCESS);
}

kern_return_t processor_set_info(pset, flavor, host, info, count)
processor_set_t		pset;
int			flavor;
host_t			*host;
processor_set_info_t	info;
unsigned int		*count;
{
	if (pset == PROCESSOR_SET_NULL)
		return(KERN_INVALID_ARGUMENT);

	if (flavor == PROCESSOR_SET_BASIC_INFO) {
		register processor_set_basic_info_t	basic_info;

		if (*count < PROCESSOR_SET_BASIC_INFO_COUNT)
			return(KERN_FAILURE);

		basic_info = (processor_set_basic_info_t) info;

		pset_lock(pset);
		basic_info->processor_count = pset->processor_count;
		basic_info->task_count = pset->task_count;
		basic_info->thread_count = pset->thread_count;
		basic_info->mach_factor = pset->mach_factor;
		basic_info->load_average = pset->load_average;
		pset_unlock(pset);

		*count = PROCESSOR_SET_BASIC_INFO_COUNT;
		*host = &realhost;
		return(KERN_SUCCESS);
	}
	else if (flavor == PROCESSOR_SET_SCHED_INFO) {
		register processor_set_sched_info_t	sched_info;

		if (*count < PROCESSOR_SET_SCHED_INFO_COUNT)
			return(KERN_FAILURE);

		sched_info = (processor_set_sched_info_t) info;

		pset_lock(pset);
#if	MACH_FIXPRI
		sched_info->policies = pset->policies;
#else	MACH_FIXPRI
		sched_info->policies = POLICY_TIMESHARE;
#endif	MACH_FIXPRI
		sched_info->max_priority = pset->max_priority;
		pset_unlock(pset);

		*count = PROCESSOR_SET_SCHED_INFO_COUNT;
		*host = &realhost;
		return(KERN_SUCCESS);
	}

	*host = HOST_NULL;
	return(KERN_INVALID_ARGUMENT);
}

/*
 *	processor_set_max_priority:
 *
 *	Specify max priority permitted on processor set.  This affects
 *	newly created and assigned threads.  Optionally change existing
 * 	ones.
 */
kern_return_t
processor_set_max_priority(pset, max_priority, change_threads)
processor_set_t	pset;
int		max_priority;
boolean_t	change_threads;
{
	if (pset == PROCESSOR_SET_NULL || invalid_pri(max_priority))
		return(KERN_INVALID_ARGUMENT);

	pset_lock(pset);
	pset->max_priority = max_priority;

	if (change_threads) {
	    register queue_head_t *list;
	    register thread_t	thread;

	    list = &pset->threads;
	    thread = (thread_t) queue_first(list);
	    while (!queue_end(list, (queue_entry_t) thread)) {
		if (thread->max_priority < max_priority)
			thread_max_priority(thread, pset, max_priority);

		thread = (thread_t) queue_next(&thread->pset_threads);
	    }
	}

	pset_unlock(pset);

	return(KERN_SUCCESS);
}

/*
 *	processor_set_policy_enable:
 *
 *	Allow indicated policy on processor set.
 */

kern_return_t
processor_set_policy_enable(pset, policy)
processor_set_t	pset;
int	policy;
{
	if ((pset == PROCESSOR_SET_NULL) || invalid_policy(policy))
		return(KERN_INVALID_ARGUMENT);

#if	MACH_FIXPRI
	pset_lock(pset);
	pset->policies |= policy;
	pset_unlock(pset);
	
    	return(KERN_SUCCESS);
#else	MACH_FIXPRI
	if (policy == POLICY_TIMESHARE)
		return(KERN_SUCCESS);
	else
		return(KERN_FAILURE);
#endif	MACH_FIXPRI
}

/*
 *	processor_set_policy_disable:
 *
 *	Forbid indicated policy on processor set.  Time sharing cannot
 *	be forbidden.
 */

kern_return_t
processor_set_policy_disable(pset, policy, change_threads)
processor_set_t	pset;
int	policy;
boolean_t	change_threads;
{
	if ((pset == PROCESSOR_SET_NULL) || policy == POLICY_TIMESHARE ||
	    invalid_policy(policy))
		return(KERN_INVALID_ARGUMENT);

#if	MACH_FIXPRI
	pset_lock(pset);

	/*
	 *	Check if policy enabled.  Disable if so, then handle
	 *	change_threads.
	 */
	if (pset->policies & policy) {
	    pset->policies &= ~policy;

	    if (change_threads) {
		register queue_head_t	*list;
		register thread_t	thread;

		list = &pset->threads;
		thread = (thread_t) queue_first(list);
		while (!queue_end(list, (queue_entry_t) thread)) {
		    if (thread->policy == policy)
			thread_policy(thread, POLICY_TIMESHARE, 0);

		    thread = (thread_t) queue_next(&thread->pset_threads);
		}
	    }
	}
	pset_unlock(pset);
#endif	MACH_FIXPRI

	return(KERN_SUCCESS);
}

#define THING_TASK	0
#define THING_THREAD	1

/*
 *	processor_set_things:
 *
 *	Common internals for processor_set_{threads,tasks}
 */
kern_return_t
processor_set_things(pset, thing_list, count, type)
processor_set_t	pset;
mach_port_t	**thing_list;
unsigned int	*count;
int		type;
{
	unsigned int actual;	/* this many things */
	int i;

	vm_size_t size, size_needed;
	vm_offset_t addr;

	if (pset == PROCESSOR_SET_NULL)
		return KERN_INVALID_ARGUMENT;

	size = 0; addr = 0;

	for (;;) {
		pset_lock(pset);
		if (!pset->active) {
			pset_unlock(pset);
			return KERN_FAILURE;
		}

		if (type == THING_TASK)
			actual = pset->task_count;
		else
			actual = pset->thread_count;

		/* do we have the memory we need? */

		size_needed = actual * sizeof(mach_port_t);
		if (size_needed <= size)
			break;

		/* unlock the pset and allocate more memory */
		pset_unlock(pset);

		if (size != 0)
			kfree(addr, size);

		assert(size_needed > 0);
		size = size_needed;

		addr = kalloc(size);
		if (addr == 0)
			return KERN_RESOURCE_SHORTAGE;
	}

	/* OK, have memory and the processor_set is locked & active */

	switch (type) {
	    case THING_TASK: {
		task_t *tasks = (task_t *) addr;
		task_t task;

		for (i = 0, task = (task_t) queue_first(&pset->tasks);
		     i < actual;
		     i++, task = (task_t) queue_next(&task->pset_tasks)) {
			/* take ref for convert_task_to_port */
			task_reference(task);
			tasks[i] = task;
		}
		assert(queue_end(&pset->tasks, (queue_entry_t) task));
		break;
	    }

	    case THING_THREAD: {
		thread_t *threads = (thread_t *) addr;
		thread_t thread;

		for (i = 0, thread = (thread_t) queue_first(&pset->threads);
		     i < actual;
		     i++,
		     thread = (thread_t) queue_next(&thread->pset_threads)) {
			/* take ref for convert_thread_to_port */
			thread_reference(thread);
			threads[i] = thread;
		}
		assert(queue_end(&pset->threads, (queue_entry_t) thread));
		break;
	    }
	}

	/* can unlock processor set now that we have the task/thread refs */
	pset_unlock(pset);

	if (actual == 0) {
		/* no things, so return null pointer and deallocate memory */
		*thing_list = 0;
		*count = 0;

		if (size != 0)
			kfree(addr, size);
	} else {
		/* if we allocated too much, must copy */

		if (size_needed < size) {
			vm_offset_t newaddr;

			newaddr = kalloc(size_needed);
			if (newaddr == 0) {
				switch (type) {
				    case THING_TASK: {
					task_t *tasks = (task_t *) addr;

					for (i = 0; i < actual; i++)
						task_deallocate(tasks[i]);
					break;
				    }

				    case THING_THREAD: {
					thread_t *threads = (thread_t *) addr;

					for (i = 0; i < actual; i++)
						thread_deallocate(threads[i]);
					break;
				    }
				}
				kfree(addr, size);
				return KERN_RESOURCE_SHORTAGE;
			}

			bcopy((char *) addr, (char *) newaddr, size_needed);
			kfree(addr, size);
			addr = newaddr;
		}

		*thing_list = (mach_port_t *) addr;
		*count = actual;

		/* do the conversion that Mig should handle */

		switch (type) {
		    case THING_TASK: {
			task_t *tasks = (task_t *) addr;

			for (i = 0; i < actual; i++)
				((mach_port_t *) tasks)[i] =
					convert_task_to_port(tasks[i]);
			break;
		    }

		    case THING_THREAD: {
			thread_t *threads = (thread_t *) addr;

			for (i = 0; i < actual; i++)
				((mach_port_t *) threads)[i] =
					convert_thread_to_port(threads[i]);
			break;
		    }
		}
	}

	return(KERN_SUCCESS);
}


/*
 *	processor_set_tasks:
 *
 *	List all tasks in the processor set.
 */
kern_return_t
processor_set_tasks(pset, task_list, count)
processor_set_t	pset;
task_array_t	*task_list;
unsigned int	*count;
{
    return(processor_set_things(pset, task_list, count, THING_TASK));
}

/*
 *	processor_set_threads:
 *
 *	List all threads in the processor set.
 */
kern_return_t
processor_set_threads(pset, thread_list, count)
processor_set_t	pset;
thread_array_t	*thread_list;
unsigned int	*count;
{
    return(processor_set_things(pset, thread_list, count, THING_THREAD));
}
