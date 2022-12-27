/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	emul_cache.c,v $
 * Revision 2.2  89/10/17  11:23:57  rwd
 * 	Removed reference to ERROR macro.
 * 	[89/09/26            rwd]
 * 
 */
/*
 * Routines which can cache values for performance.
 */

#ifdef ECACHE
#include <uxkern/bsd_msg.h>

#define ROUTINE(routine) int routine(serv_port, syscode, argp, rvalp)\
	int serv_port;\
	int syscode;\
	int *argp;\
	int *rvalp;\
{\
	register int rv=0;\
	if (emul_cache.routine[0] == 0) {\
	    rv = emul_generic(serv_port, syscode, argp, rvalp);\
	    if (rv) return (rv);\
	    emul_cache.routine[0] = rvalp[0];\
	    emul_cache.routine[1] = rvalp[1];\
	} else {\
	    rvalp[0] = emul_cache.routine[0];\
	    rvalp[1] = emul_cache.routine[1];\
	}\
	return (rv);\
}

struct emul_cache_struct {
	int e_getpid[2];
	int e_getdtablesize[2];
	int e_getpagesize[2];
} emul_cache;

emul_cache_init()
{
	bzero(&emul_cache,sizeof(struct emul_cache_struct));
}

ROUTINE(e_getpid)
ROUTINE(e_getdtablesize)
ROUTINE(e_getpagesize)
#endif ECACHE
