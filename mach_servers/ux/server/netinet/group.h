/* 
 **********************************************************************
 * Mach Operating System
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 **********************************************************************
 * HISTORY
 * $Log:	group.h,v $
 * Revision 2.1  89/08/04  14:20:58  rwd
 * Created.
 * 
 **********************************************************************
 */ 

/*
 * group.h
 */

#include "igmproto.h"

#if	IGMPROTO

#include "igmp_var.h"

struct GroupDescriptor {
    u_char		inuse;
    int			confirm_timer;
    struct igmpcb 	pcb;
};

#define GROUP_POOL_SIZE	128	/* total no. of descriptors  */


extern struct GroupDescriptor	GroupDescriptorPool[];

extern struct GroupDescriptor	*InstallGroupDescriptor();
extern struct GroupDescriptor	*AllocGroupDescriptor();

#endif	IGMPROTO
