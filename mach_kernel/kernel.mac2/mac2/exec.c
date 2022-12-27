/* 
 * Mach Operating System
 * Copyright (c) 1991,1990,1989 Carnegie Mellon University
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
 * $Log:	exec.c,v $
 * Revision 2.2  91/09/12  16:40:17  bohman
 * 	Created from vax version.
 * 	[91/09/11  14:41:46  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/exec.c
 */

#include <mach/mach_types.h>

#include <mach/mach_interface.h>
#include <mach/mach_user_internal.h>

#include <boot_ufs/file_io.h>
#include <boot_ufs/loader_info.h>

#include <mac2/exec.h>

#define NBPG			MAC2_PGBYTES

#define USRTEXT			NBPG
#define USRSTACK		(0-NBPG)

#define NBSG			131072
#define SGOFSET			(NBSG-1)

#define LOADER_PAGE_SIZE	MAC2_PGBYTES
#define loader_round_page(x)	((vm_offset_t)((((vm_offset_t)(x)) \
						+ LOADER_PAGE_SIZE - 1) \
					& ~(LOADER_PAGE_SIZE-1)))

#define loader_round_segment(x)	((vm_offset_t)((((vm_offset_t)(x)) \
						+ SGOFSET) \
					       & ~SGOFSET))

int
ex_get_header(fp, lp)
struct file			*fp;
register struct loader_info	*lp;
{
    struct exec	x;
    register int	result;
    vm_size_t	resid;
    
    result = read_file(fp, 0, (vm_offset_t)&x, sizeof(x), &resid);
    if (result)
	return (result);
    if (resid)
	return (EX_NOT_EXECUTABLE);

    switch ((int)x.a_magic) {
	
      case 0407:
	lp->format = EX_READIN;
	lp->text_start  = 0;
	lp->text_size   = 0;
	lp->text_offset = 0;
	lp->data_start  = USRTEXT;
	lp->data_size   = x.a_text + x.a_data;
	lp->data_offset = sizeof(struct exec);
	lp->bss_size    = x.a_bss;
	break;
	
      case 0410:
	if (x.a_text == 0) {
	    return(EX_NOT_EXECUTABLE);
	}
	lp->format = EX_SHAREABLE;
	lp->text_start  = USRTEXT;
	lp->text_size   = loader_round_page(x.a_text);
	lp->text_offset = sizeof(struct exec);
	lp->data_start  =
	    loader_round_segment(LOADER_PAGE_SIZE
				 + lp->text_size);
	lp->data_size   = loader_round_page(x.a_data);
	lp->data_offset = lp->text_offset + lp->text_size;
	lp->bss_size    = loader_round_page(x.a_bss);
	break;
	
      case 0413:
	if (x.a_text == 0) {
	    return(EX_NOT_EXECUTABLE);
	}
	lp->format = EX_PAGEABLE;
	lp->text_start  = USRTEXT;
	lp->text_size   = loader_round_page(x.a_text);
	lp->text_offset = 0;
	lp->data_start  = loader_round_segment(USRTEXT + lp->text_size);
	lp->data_size   = loader_round_page(x.a_data);
	lp->data_offset = lp->text_offset + lp->text_size;
	lp->bss_size    = loader_round_page(x.a_bss);
	break;
      default:
	return (EX_NOT_EXECUTABLE);
    }
    lp->entry_1 = x.a_entry;
    lp->entry_2 = 0;

    return(0);
}

#define	STACK_SIZE	(32*1024)

char *
set_regs(user_task, user_thread, lp, arg_size)
mach_port_t		user_task;
mach_port_t		user_thread;
struct loader_info	*lp;
int			arg_size;
{
    vm_offset_t			stack_start;
    vm_offset_t			stack_end;
    thread_state_regs_t		regs;
    thread_state_frame_t	frame;
    unsigned int		reg_size;
    
    /*
     * Allocate stack.
     */
    stack_end = USRSTACK;
    stack_start = stack_end - STACK_SIZE;
    (void) vm_allocate(user_task,
		       &stack_start,
		       (vm_size_t)(stack_end - stack_start),
		       FALSE);
    
    reg_size = THREAD_STATE_REGS_COUNT;
    (void) thread_get_state_KERNEL(user_thread,
				   THREAD_STATE_REGS,
				   (thread_state_regs_t *)&regs,
				   &reg_size);
    
    regs.r_sp = (int)((stack_end - arg_size) & ~(sizeof(int)-1));
    
    (void) thread_set_state_KERNEL(user_thread,
				   THREAD_STATE_REGS,
				   (thread_state_regs_t *)&regs,
				   reg_size);

    frame.f_normal.f_pc = lp->entry_1;
    frame.f_normal.f_sr = 0;
    frame.f_normal.f_fmt = STKFMT_NORMAL;

    (void) thread_set_state_KERNEL(user_thread,
				   THREAD_STATE_FRAME,
				   (thread_state_frame_t *)&frame,
				   THREAD_STATE_FRAME_COUNT);
    
    return ((char *)regs.r_sp);
}
