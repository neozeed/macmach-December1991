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
 * $Log:	db_print.c,v $
 * Revision 2.9  91/08/03  18:17:19  jsb
 * 	In db_print_thread, if the thread is swapped and there is a
 * 	continuation function, print the function name in parentheses
 * 	instead of '(swapped)'.
 * 	[91/07/04  09:59:27  jsb]
 * 
 * Revision 2.8  91/07/31  17:30:43  dbg
 * 	Revise scheduling state machine.
 * 	[91/07/30  16:43:42  dbg]
 * 
 * Revision 2.7  91/07/09  23:15:57  danner
 * 	Fixed a few printf that should be db_printfs. 
 * 	[91/07/08            danner]
 * 
 * Revision 2.6  91/05/14  15:35:25  mrt
 * 	Correcting copyright
 * 
 * Revision 2.5  91/02/05  17:06:53  mrt
 * 	Changed to new Mach copyright
 * 	[91/01/31  16:18:56  mrt]
 * 
 * Revision 2.4  90/10/25  14:43:54  rwd
 * 	Changed db_show_regs to print unsigned.
 * 	[90/10/19            rpd]
 * 	Generalized the watchpoint support.
 * 	[90/10/16            rwd]
 * 
 * Revision 2.3  90/09/09  23:19:52  rpd
 * 	Avoid totally incorrect guesses of symbol names for small values.
 * 	[90/08/30  17:39:08  af]
 * 
 * Revision 2.2  90/08/27  21:51:49  dbg
 * 	Insist that 'show thread' be called with an explicit address.
 * 	[90/08/22            dbg]
 * 
 * 	Fix type for db_maxoff.
 * 	[90/08/20            dbg]
 * 
 * 	Do not dereference the "valuep" field of a variable directly,
 * 	call the new db_read/write_variable functions instead.
 * 	Reflected changes in symbol lookup functions.
 * 	[90/08/20            af]
 * 	Reduce lint.
 * 	[90/08/10  14:33:44  dbg]
 * 
 * 	Created.
 * 	[90/07/25            dbg]
 * 
 */
/*
 * 	Author: David B. Golub, Carnegie Mellon University
 *	Date:	7/90
 */

/*
 * Miscellaneous printing.
 */
#include <machine/db_machdep.h>

#include <ddb/db_lex.h>
#include <ddb/db_variables.h>
#include <ddb/db_sym.h>

#include <kern/task.h>
#include <kern/thread.h>
#include <kern/queue.h>

extern unsigned int	db_maxoff;

void
db_show_regs()
{
	int	(*func)();
	register struct db_variable *regp;
	db_expr_t	value, offset;
	char *		name;

	for (regp = db_regs; regp < db_eregs; regp++) {
	    db_read_variable(regp, &value);
	    db_printf("%-12s%#8n", regp->name, value);
	    db_find_xtrn_sym_and_offset((db_addr_t)value, &name, &offset);
	    if (name != 0 && offset <= db_maxoff && offset != value) {
		db_printf("\t%s", name);
		if (offset != 0)
		    db_printf("+0x%08x", offset);
	    }
	    db_printf("\n");
	}
	db_print_loc_and_inst(PC_REGS(DDB_REGS));
}

void
db_print_thread(thread)
	thread_t	thread;
{
	time_value_t	user_time;
	time_value_t	system_time;

	db_printf(" %c%c%c%c",
		(thread->state & TH_RUN)  ? 'R' : ' ',
		(thread->state & TH_WAIT) ? 'W' : ' ',
		(thread->state & TH_SUSP) ? 'S' : ' ',
		(thread->state & TH_UNINT)? 'N' : ' ');
	if (thread->state & TH_SWAPPED) {
	    if (thread->swap_func) {
		db_printf("(");
		db_printsym((db_addr_t)thread->swap_func, DB_STGY_ANY);
		db_printf(")");
	    } else {
		db_printf("(swapped)");
	    }
	}
	if (thread->state & TH_WAIT) {
	    db_printf(" ");
	    db_printsym((db_addr_t)thread->wait_event, DB_STGY_ANY);
	}
	else {
	    thread_read_times(thread, &user_time, &system_time);
	    db_printf(" pri = %d, %du %ds %dc",
		thread->sched_pri,
		user_time.seconds,
		system_time.seconds,
		thread->cpu_usage);
	}
	db_printf("\n");
}

void
db_print_task_long(task, thread_print_routine)
	task_t	task;
	void	(*thread_print_routine)();
{
	thread_t	thread;

	db_printf("task 0x%x: ", task);
	if (task->thread_count == 0) {
	    db_printf("no threads\n");
	    return;
	}
	if (task->thread_count > 1) {
	    db_printf("%d threads: \n", task->thread_count);
	}
	queue_iterate(&task->thread_list, thread, thread_t, thread_list) {
	    db_printf("thread 0x%x ", thread);
	    (*thread_print_routine)(thread);
	}
}

db_print_all_tasks(task_print_routine, thread_print_routine)
	void	(*task_print_routine)();
	void	(*thread_print_routine)();
{
	register task_t		 task;
	register processor_set_t pset;

	if (all_psets.next == 0)
	    return;	/* not initialized */

	queue_iterate(&all_psets, pset, processor_set_t, all_psets) {
	    queue_iterate(&pset->tasks, task, task_t, pset_tasks) {
		(*task_print_routine)(task, thread_print_routine);
	    }
	}
}

void
db_show_all_threads()
{
	db_print_all_tasks(db_print_task_long, db_print_thread);
}

/*ARGSUSED*/
void
db_show_one_thread(addr, have_addr, count, modif)
	db_expr_t	addr;
	boolean_t	have_addr;
	db_expr_t	count;
	char *		modif;
{
	if (!have_addr) {
	    db_error("Which thread?\n");
	    /*NOTREACHED*/
	}
	db_print_thread((thread_t)addr);
}
