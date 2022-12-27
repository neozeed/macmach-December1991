/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	console.c,v $
 * Revision 2.3  90/10/29  17:27:14  dpj
 * 	Added mach_console_port(), set_mach_console_port().
 * 	[90/08/15  14:23:10  dpj]
 * 
 * Revision 2.2  90/07/26  12:36:49  dpj
 * 	First version
 * 	[90/07/24  14:26:09  dpj]
 * 
 *
 */

/*
 * Simple console output facility for standalone Mach programs.
 */

#include	<mach.h>
#include	<mach_privileged_ports.h>
#include	<device/device_types.h>

#ifdef	DEBUG && (MACH3_UNIX || MACH3_VUS || MACH3_US)
#include	<stdio.h>
#endif	DEBUG && (MACH3_UNIX || MACH3_VUS || MACH3_US)


static mach_port_t	mach_console_port_internal = MACH_PORT_NULL;
static int		tried_open = 0;


mach_port_t mach_console_port()
{
	kern_return_t		result;

	if (mach_console_port_internal != MACH_PORT_NULL) {
		return(mach_console_port_internal);
	}

	if (tried_open == 1) {
		return(MACH_PORT_NULL);
	}

	tried_open = 1;
	result = device_open(mach_device_server_port(),
					D_READ|D_WRITE,
					"console",
					&mach_console_port_internal);
#ifdef	DEBUG && (MACH3_UNIX || MACH3_VUS || MACH3_US)
		fprintf(stderr,"mach_console_port.device_open(): 0x%x\n",result);
#endif	DEBUG && (MACH3_UNIX || MACH3_VUS || MACH3_US)
	if (result != KERN_SUCCESS) {
		mach_console_port_internal = MACH_PORT_NULL;
	}

	return(mach_console_port_internal);
}

void set_mach_console_port(port)
	mach_port_t		port;
{
	mach_console_port_internal = port;
}


kern_return_t	console_write(str,len)
	char		*str;
	int		len;
{
	kern_return_t		result;
	int			count;

	if (mach_console_port_internal == MACH_PORT_NULL) {
		/*
		 * Force initialization.
		 */
		(void) mach_console_port();
	}

	if (len <= 0) return(KERN_SUCCESS);

	result = device_write_inband(mach_console_port_internal,0,0,
							str,len,&count);
#ifdef	DEBUG && (MACH3_UNIX || MACH3_VUS || MACH3_US)
	fprintf(stderr,"console_write.device_write_inband(): 0x%x %d\n",
							result,count);
#endif	DEBUG && (MACH3_UNIX || MACH3_VUS || MACH3_US)

	if (str[len - 1] == '\n') {
		(void) device_write_inband(mach_console_port_internal,0,0,
							"\r",1,&count);
	}

	return(result);
}


