/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	mach_time.c,v $
 * Revision 2.3  90/08/22  18:11:22  roy
 * 	Added MACH3_UNIX impl.
 * 	[90/08/14  12:14:14  roy]
 * 
 * Revision 2.2  90/07/26  12:37:47  dpj
 * 	First version
 * 	[90/07/24  14:29:19  dpj]
 * 
 *
 */

#include	<mach.h>
#include	<mach/time_value.h>
#include 	<mach_error.h>


#if !defined(MACH3_UNIX)
/*
 * Simple time-of-day facility for standalone Mach programs.
 */

#include	<mach_privileged_ports.h>

kern_return_t	mach_get_time(time_value)
	time_value_t		*time_value;	/* OUT */
{
	return(host_get_time(mach_privileged_host_port(),time_value));
}


kern_return_t	mach_set_time(time_value)
	time_value_t		*time_value;
{
	return(host_set_time(mach_privileged_host_port(),*time_value));
}


#else !defined(MACH3_UNIX)
/*
 * Simple time-of-day facility for Unix programs
 */

#include <sys/time.h>

kern_return_t	mach_get_time(time_value)
	time_value_t		*time_value;	/* OUT */
{
	struct timeval time;
	extern int errno;

	errno = 0;
	if (gettimeofday(&time, 0) < 0)
		return(unix_err(errno));
	else {
		time_value->seconds = time.tv_sec;
		time_value->microseconds = time.tv_usec;
	}
	
	return(KERN_SUCCESS);
}


#endif !defined(MACH3_UNIX)

