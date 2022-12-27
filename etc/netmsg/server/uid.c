/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	uid.c,v $
 * Revision 2.1  90/10/27  20:46:59  dpj
 * Moving under MACH3 source control
 * 
 * Revision 1.8.1.1  90/08/15  15:00:41  dpj
 * 	Updated for MACH3_SA.
 * 
 * Revision 1.8  89/05/02  11:19:06  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 *  3-Nov-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Started.
 *
 */
/*
 * uid.c
 *
 *
 */
#ifndef	lint
char uid_rcsid[] = "$Header: /afs/cs.cmu.edu/project/mach-2/rcs/pkg/netmsg/server/uid.c,v 2.1 90/10/27 20:46:59 dpj Exp $";
#endif not lint
/*
 * Generates locally unique identifiers.
 */


#define DEBUGOFF 1

#include <cthreads.h>
#include <mach/boolean.h>
#if	MACH3_SA || MACH3_US
#include <mach/time_value.h>
#else	MACH3_SA || MACH3_US
#include <sys/time.h>
#endif	MACH3_SA || MACH3_US
#include <sys/types.h>

#include "netmsg.h"
#include "uid.h"

#if	MACH3_SA || MACH3_US
unsigned int	uid_counter;
#else	MACH3_SA || MACH3_US
extern long	random();
extern char	*initstate();

#define STATE_SIZE	256
static char		random_state[STATE_SIZE];
#endif	MACH3_SA || MACH3_US



/*
 * uid_init
 *
 * Initialises random number generator in order to produce unique identifiers.
 *
 */
EXPORT boolean_t uid_init()
BEGIN("uid_init")
#if	MACH3_SA || MACH3_US
    time_value_t tp;
#else	MACH3_SA || MACH3_US
    struct timeval tp;
    struct timezone tzp;
#endif	MACH3_SA || MACH3_US

#if	MACH3_SA || MACH3_US
    (void)mach_get_time(&tp);
    uid_counter = (tp.seconds & 0x003fffff) << 10;
#else	MACH3_SA || MACH3_US
    (void)gettimeofday(&tp, &tzp);

    (void)initstate((unsigned int)tp.tv_usec, (char *)random_state, STATE_SIZE);
#endif	MACH3_SA || MACH3_US

    
    RETURN(TRUE);
END


/*
 * uid_get_new_uid
 *
 * Returns a new unique identifier from the random number generator.
 *
 */
EXPORT long uid_get_new_uid()
BEGIN("uid_get_new_uid")
    long new_uid;
#if	MACH3_SA || MACH3_US
    new_uid = ++uid_counter;
#else	MACH3_SA || MACH3_US
    new_uid = (long)random();
#endif	MACH3_SA || MACH3_US
    RETURN(new_uid);
END
