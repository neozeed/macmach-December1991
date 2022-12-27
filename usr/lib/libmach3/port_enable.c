/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	port_enable.c,v $
 * Revision 2.2  90/07/26  12:37:56  dpj
 * 	First version
 * 	[90/07/24  14:29:30  dpj]
 * 
 *
 */

/*
 * Compatibility code for port_enable()/port_disable functions.
 */

#if	!defined(MACH_IPC_COMPAT) || MACH_IPC_COMPAT

#define	PORT_ENABLED_IS_A_VARIABLE	1

#include <mach/mach_types.h>

port_t		PORT_ENABLED = (-1);

kern_return_t	port_enable(target_task, port)
	task_t		target_task;
	port_t		port;
{
	extern
	kern_return_t	port_set_add();
#ifdef	XXX
	extern
	kern_return_t	xxx_port_enable();
#endif	XXX

#if	PORT_ENABLED_IS_A_VARIABLE
	extern
	kern_return_t	port_set_allocate();

	if (PORT_ENABLED == (-1))
		(void) port_set_allocate(target_task, &PORT_ENABLED);
#endif	PORT_ENABLED_IS_A_VARIABLE

#ifdef	XXX
	return((PORT_ENABLED == (-1)) ?
		xxx_port_enable(target_task, port) :
	 	port_set_add(target_task, PORT_ENABLED, port)
		);
#else	XXX
	return(port_set_add(target_task, PORT_ENABLED, port));
#endif	XXX
}

kern_return_t	port_disable(target_task, port)
	task_t		target_task;
	port_t		port;
{
	kern_return_t	kr;
	extern
	kern_return_t	port_set_remove();
#ifdef	XXX
	extern
	kern_return_t	xxx_port_disable();
#endif	XXX

#if	PORT_ENABLED_IS_A_VARIABLE
	extern
	kern_return_t	port_set_allocate();

	if (PORT_ENABLED == (-1))
		(void) port_set_allocate(target_task, &PORT_ENABLED);
#endif	PORT_ENABLED_IS_A_VARIABLE

#ifdef	XXX
	return((PORT_ENABLED == (-1)) ?
		xxx_port_disable(target_task, port) :
	 	(((kr = port_set_remove(target_task, port)) ==
							KERN_NOT_IN_SET) ?
			KERN_SUCCESS : kr)
		);
#else	XXX
	kr = port_set_remove(target_task, port);
	if (kr == KERN_NOT_IN_SET)
		return(KERN_SUCCESS);
	else
		return(kr);
#endif	XXX
}

#endif	!defined(MACH_IPC_COMPAT) || MACH_IPC_COMPAT

