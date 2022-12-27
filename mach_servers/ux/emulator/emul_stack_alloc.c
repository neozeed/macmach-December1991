/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	emul_stack_alloc.c,v $
 * Revision 2.5  90/11/05  15:31:50  rpd
 * 	Remove stack limit problems.
 * 	[90/11/01            rwd]
 * 
 * Revision 2.4  90/10/25  15:06:31  rwd
 * 	Add code to support STACK_GROWTH_UP.
 * 	[90/10/12            rwd]
 * 
 * Revision 2.3  90/06/02  15:20:44  rpd
 * 	Revised reply port allocation and added an explicit reply port
 * 	argument to vm_map.  (It is called before we are on an emulator stack.)
 * 	[90/04/08            rpd]
 * 	Converted to new IPC.
 * 	[90/03/26  19:28:56  rpd]
 * 
 * Revision 2.2  89/11/29  15:26:46  af
 * 	RCS-ed.
 * 	[89/11/29            af]
 * 
 */
/*
 * Maintain a separate stack for the user thread
 * to use while in the emulator.
 */

#ifdef mac2 /* don't want the vm_map definition in mach_interface.h */
#define vm_map xvm_map
#include <mach/mach.h>
#include <machine/vmparam.h>
#include "emul_stack.h"
#undef vm_map
#else
#include <mach/mach.h>
#include <machine/vmparam.h>
#include "emul_stack.h"
#endif

extern	char	end[];

extern void	emul_panic();

vm_size_t	emul_stack_size = 4096*4;	/* 16 K */
					/* size of one stack */
vm_offset_t	emul_stack_mask;	/* mask these bits to get to bottom */
vm_offset_t	emul_stack_next;
emul_stack_t	emul_stack_list = (emul_stack_t)0;

/*
 * Flag to indicate that we can use emulator stacks.
 */
boolean_t	stack_init_done = FALSE;
/*
 * Global reply port; used before the emulator stacks are initialized.
 */
mach_port_t	mig_reply_port = MACH_PORT_NULL;

extern mach_port_t mig_get_reply_port();

/*
 * Clear the stack list on fork; children do NOT inherit the list
 * of emulator stacks.
 */
emul_stack_t
emul_stack_init()
{
	emul_stack_mask = ~(emul_stack_size - 1);
	emul_stack_next = ((vm_offset_t)end + emul_stack_size - 1)
				& emul_stack_mask;

	emul_stack_list = emul_stack_alloc();
#ifndef mac2
	emul_stack_next = USRTEXT;
#endif
	return emul_stack_list;
}

/*
 * Allocate a stack for the emulator.  Called when
 * the stack list is empty.  Can be called on user's stack.
 */
emul_stack_t
emul_stack_alloc()
{
        vm_offset_t     	base;
        register emul_stack_t   new_stack;
        mach_port_t    		reply_port;
	kern_return_t		ret;

        /*
         * Allocate a new reply port for the stack;
         * or take the global port if this is the first stack.
         */

        if (stack_init_done)
                reply_port = mach_reply_port();
        else
                reply_port = mig_get_reply_port();
        if (reply_port == MACH_PORT_NULL)
                emul_panic("emul_stack_alloc: no reply port");

        /*
         * Look for the next free region at the correct alignment.
         * We must pass in the reply port explicitly, because
         * we aren't running on the new stack yet, so mig_get_reply_port()
         * can't find an appropriate reply port.
         */
	base = emul_stack_next;
	ret = vm_map(mach_task_self(), reply_port,
		     &base, emul_stack_size, (emul_stack_size - 1),
		     TRUE, MEMORY_OBJECT_NULL, (vm_offset_t)0, FALSE,
		     VM_PROT_DEFAULT, VM_PROT_ALL, VM_INHERIT_NONE);

	if (ret != KERN_SUCCESS)
	    emul_panic("emul_stack_alloc");

        emul_stack_next = base + emul_stack_size;

        /*
         * Set up top-of-stack structure with new reply port.
         */
#ifdef  STACK_GROWTH_UP
        new_stack = (emul_stack_t) (base);
        new_stack->link = 0;
        new_stack->reply_port = reply_port;
        new_stack++;
#else   STACK_GROWTH_UP
        new_stack = (emul_stack_t) (base + emul_stack_size);
        new_stack--;
        new_stack->link = 0;
        new_stack->reply_port = reply_port;
#endif  STACK_GROWTH_UP

        return (new_stack);
}


/*
 * Use reply port at top of emulator stack.
 */
mach_port_t
mig_get_reply_port()
{
        register mach_port_t port;

        if (stack_init_done) {
                int     x;
                register vm_offset_t    stack_addr = (vm_offset_t)&x; /* XXX */                register emul_stack_t   stack;

                stack_addr &= emul_stack_mask;  /* get base of stack */
#ifdef  STACK_GROWTH_UP
                stack = (emul_stack_t)stack_addr;
#else   STACK_GROWTH_UP
                stack_addr += emul_stack_size;  /* get top of stack */
                stack = ((emul_stack_t)stack_addr) - 1;
                                        /* point to structure at stack top */
#endif  STACK_GROWTH_UP
                if ((port = stack->reply_port) == MACH_PORT_NULL)
                        stack->reply_port = port = mach_reply_port();
        } else {
                if ((port = mig_reply_port) == MACH_PORT_NULL)
                        mig_reply_port = port = mach_reply_port();
        }

        return port;
}

/*ARGSUSED*/
void
mig_dealloc_reply_port(port)
	mach_port_t port;
{
	emul_panic("mig_dealloc_reply_port");
}

void
mig_init(initial)
	register emul_stack_t initial;
{
	if (initial == 0) {
		stack_init_done = FALSE;
		mig_reply_port = MACH_PORT_NULL;
	} else {
		/* recycle global reply port as this cthread's reply port */

		stack_init_done = TRUE;
		if (initial->reply_port != mig_reply_port)
			emul_panic("mig_init");
	}
}
