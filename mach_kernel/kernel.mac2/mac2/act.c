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
 * $Log:	act.c,v $
 * Revision 2.2  91/09/12  16:38:11  bohman
 * 	Created Mach 3.0 version from 2.5 version.
 * 	[91/09/11  14:16:15  bohman]
 * 
 * Revision 2.2  90/09/04  17:16:08  bohman
 * 	Created.
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/act.c
 *	Author: David E. Bohman II (CMU macmach)
 */

/*
 * activity schedule routines
 */
#include <mach/mach_types.h>

#include <machine/machparam.h>

#include <mac2/act.h>

/*
 * Zones to allocate activities
 * from.
 */
zone_t	    actzones[ACT_MAX_NENTRIES];

/*
 * initactzone(n)
 *	n - number of list entries
 *
 * Initialize an activity zone for
 * activites with n list entries.
 */
static
initactzone(n)
register int	n;
{
    register zone_t	zone;
#define	SIZEOF_ACT(n)	\
    (sizeof (struct act) + ((n) * sizeof (struct actentry)))

    zone = zinit(SIZEOF_ACT(n),
		 16*SIZEOF_ACT(n),
		 0,
		 FALSE,
		 "activities");

    if (zone == ZONE_NULL)
	panic("initactzone: zone init failed");

    ACTZONE(n) = zone;

#undef SIZEOF_ACT
}

/*
 * makeact(func, ipl, n)
 *	func - ptr to function to execute
 *	ipl - ipl to run function at
 *	n - number of activity list entries
 *	returns - ptr to act struct for act created
 *		  or 0 if no memory
 *
 * Makeact creates an activity.  Note that the activity
 * is not put on an activity list (scheduled), runact()
 * is used to do this.
 */
struct act *
makeact(func, ipl, n)
register int	(*func)(), ipl, n;
{
    register struct act		*ap;
    register struct actentry	*ae;

    if (n > ACT_MAX_NENTRIES)
	panic("makeact: too many lists");
    
    if (ACTZONE(n) == ZONE_NULL)
	initactzone(n);

    ap = (struct act *)zalloc(ACTZONE(n));
    if (ap == 0)
    	return (0);

    ap->a_func = func;
    ap->a_ipl = ipl;
    ap->a_flags = 0;
    ap->a_nentries = n;

    for (ae = ap->a_alist; ae < &ap->a_alist[n]; ae++) {
	ae->ae_flags = 0;
	ae->ae_act = ap;
    }

    return (ap);
}

/*
 * addact(entry, ap, al)
 *	entry - activity entry index
 *	ap - ptr to activity
 *	al - ptr to activity list
 *
 * Addact sets up an activity entry to
 * use the specified activity list.
 */
addact(entry, ap, al)
register int		entry;
register struct act	*ap;
register struct actlist *al;
{
    register struct actentry	*ae = &ap->a_alist[entry];
    register			s = splhigh();

    if (entry < ap->a_nentries && !(ae->ae_flags&ACT_Q_OK)) {
	ae->ae_alist = al;
	ae->ae_flags |= ACT_Q_OK;
    }

    splx(s);
}    

/*
 * runact(entry, ap, arg, t)
 *	entry - activity entry index
 *	ap - ptr to act struct
 *	arg - argument to pass to function
 *	t - number of calls to skip
 *
 * Runact schedules an activity to be run by
 * placing the entry on its activity list.
 */
runact(entry, ap, arg, t)
register int		entry;
register struct act	*ap;
unsigned long		t;
{
    register struct actentry	*ae = &ap->a_alist[entry];
    register			s = splhigh();

    if (entry < ap->a_nentries && (ae->ae_flags&ACT_Q_OK)) {
	if ((ae->ae_flags&ACT_Q_QUE) == 0)	/* not queued, put on list */
	    putact(ae);

	if ((ae->ae_flags&ACT_Q_RUN) == 0) {	/* schedule it */
	    ae->ae_arg = arg;
	    ae->ae_time = t;
	    ae->ae_flags |= ACT_Q_RUN;
	}
    }

    splx(s);
}

/*
 * Purgeactlist removes all queued, but
 * not scheduled activities from an
 * activity list.
 */
static
void
purgeactlist(a)
register struct actlist		*a;
{
    register struct actentry	*ae, **ppn;

    if (a->al_front == a->al_back) {
	ae = a->al_front;
	if (ae && (ae->ae_flags & ACT_Q_RUN) == 0)
	    a->al_front = a->al_back = 0;
    }
    else {
	ppn = &a->al_front;

	while (ae = *ppn) {
	    if ((ae->ae_flags&ACT_Q_RUN) == 0) {
		ae->ae_flags &= ~ACT_Q_QUE;
		*ppn = ae->ae_next;
		if (a->al_back == ae)
		    a->al_back = (struct actentry *)ppn;
	    }
	    else
		ppn = &ae->ae_next;
	}
    }
}

/*
 * Endact removes an activity from all
 * of its activity lists and then
 * deallocates the data structure.
 */
endact(ap)
register struct act	*ap;
{
    register struct actentry	*ae, *am = &ap->a_alist[ap->a_nentries];
    register			s = splhigh();

    for (ae = ap->a_alist; ae < am; ae++) {
	ae->ae_flags &= ~ACT_Q_RUN;
	ae->ae_time = 0;
	if (ae->ae_flags&ACT_Q_QUE)
	    purgeactlist(ae->ae_alist);
    }

    splx(s);

    zfree(ACTZONE(ap->a_nentries), ap);
}

/*
 * Doactlist() is a function call
 * version of the macro of the same name.
 * The calling sequence is different; the macro
 * takes an activity list as a argument, and the
 * function takes a ptr to an activity list.
 */
doactlist(al)
register struct actlist		*al;
{
    DOACTLIST(*al);
}
