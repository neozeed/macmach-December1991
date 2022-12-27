#include "copyright.h"

/* $XConsortium: XMaskEvent.c,v 11.18 88/09/06 16:11:31 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#define NEED_EVENTS
#include "Xlibint.h"

extern _XQEvent *_qfree;
extern long _event_to_mask[];
#define AllPointers (PointerMotionMask|PointerMotionHintMask|ButtonMotionMask)
#define AllButtons (Button1MotionMask|Button2MotionMask|Button3MotionMask|\
		    Button4MotionMask|Button5MotionMask)

/* 
 * return the next event in the queue matching one of the events in the mask.
 * If no event, flush output, and wait until match succeeds.
 * Events earlier in the queue are not discarded.
 */

XMaskEvent (dpy, mask, event)
	register Display *dpy;
	long mask;		/* Selected event mask. */
	register XEvent *event;	/* XEvent to be filled in. */
{
	register _XQEvent *prev, *qelt;

        LockDisplay(dpy);
	prev = NULL;
	while (1) {
	    for (qelt = prev ? prev->next : dpy->head;
		 qelt;
		 prev = qelt, qelt = qelt->next) {
		if ((_event_to_mask[qelt->event.type] & mask) &&
		    ((qelt->event.type != MotionNotify) ||
		     (mask & AllPointers) ||
		     (mask & AllButtons & qelt->event.xmotion.state))) {
		    *event = qelt->event;
		    if (prev) {
			if ((prev->next = qelt->next) == NULL)
			    dpy->tail = prev;
		    } else {
			if ((dpy->head = qelt->next) == NULL)
			dpy->tail = NULL;
		    }
		    qelt->next = _qfree;
		    _qfree = qelt;
		    dpy->qlen--;
		    UnlockDisplay(dpy);
		    return;
		}
	    }
	    _XReadEvents(dpy);
	}
}
