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
 * $Log:	clock.c,v $
 * Revision 2.2  91/09/12  16:39:08  bohman
 * 	Created Mach 3.0 version from 2.5 version.
 * 	[91/09/11  14:24:23  bohman]
 * 
 * Revision 2.2.1.1  90/09/07  00:55:48  bohman
 * 	Final cleanup.
 * 	[90/09/07            bohman]
 * 
 * Revision 2.2  90/09/04  17:17:26  bohman
 * 	Created.
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/clock.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <mach/mach_types.h>

#include <sys/time.h>

#include <mach/mac2/frame.h>

#include <mac2/clock.h>
#include <mac2/act.h>
#include <mac2/psl.h>

#include <mac2os/Types.h>
#include <mac2os/Errors.h>
#include <mac2os/Retrace.h>

/*
 * Machine-dependent clock routines.
 */

/*
 * The 60.15 Hz clock activity list
 */
struct actlist	actclock;

VBLTask		VBLClock;

/*
 * Start the real-time clock.
 */
startrtclock()
{
    register VBLTaskPtr	v = &VBLClock;
    extern void		clocktask();

    v->qLink = 0;
    v->qType = 1;
    v->vblAddr = clocktask;
    v->vblCount = 1;
    v->vblPhase = 0;
    (void) VInstall(v);
}

/*
 * Initialze the 'time' variable.
 */
inittodr()
{
    unsigned long	t;

    (void) GetDateTime(&t);

    /*
     * The Macintosh Operating System
     * stores the time as local time,
     * and the Mach kernel the uses U**X
     * style GMT.  Here we just set the
     * kernel time based on the MacOS value,
     * so set the RTC time to the notion of
     * GMT for your local time zone.
     * Also, the T0 values for MacOS and
     * U**X are different, so we also add
     * in the correct offset here.
     */
    time.seconds = t - T0_DELTA;
    time.microseconds = 0;
}

/*
 * Called to set the time of day clock
 * when the system time is adjusted.  MacMach
 * does not ever set the RTC time, the user
 * is expected to set it from MacOS to standard time
 * (i.e. NOT daylight savings time) for the current time zone.
 */
resettodr()
{
}

/*
 * Handle 60.15 Hz clock ticks.
 */
clock_intr(frame)
register generic_exception_frame_t *frame;
{
    /*
     * Run activities in the
     * clock list.
     */
    if (CHECKACTLIST(actclock))
	doactlist(&actclock);

    /*
     * Pass clock interrupt to MI
     * code.
     */
    clock_interrupt(16625,	/* 60.15 HZ */
		    ((frame->f_normal.f_sr&SR_SUPR) == 0),
		    ((frame->f_normal.f_sr&SR_IPL) == SR_IPL0));

    VBLClock.vblCount = 1;
}

void
setsoftclock()
{
    softclock();
}
