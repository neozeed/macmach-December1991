#include "copyright.h"

/* $XConsortium: XWinEvent.c,v 11.15 88/09/06 16:11:24 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1985	*/

#define NEED_EVENTS
#include "Xlibint.h"

extern _XQEvent *_qfree;
extern long _event_to_mask[];
#define AllPointers (PointerMotionMask|PointerMotionHintMask|ButtonMotionMask)
#define AllButtons (Button1MotionMask|Button2MotionMask|Button3MotionMask|\
		    Button4MotionMask|Button5MotionMask)

/* 
 * Return the next event in the queue
 * for the given window matching one of the events in the mask.
 * Events earlier in the queue are not discarded.
 * If none found, flush, and then wait until an event arrives which
 * matches.
 */

XWindowEvent (dpy, w, mask, event)
        register Display *dpy;
	Window w;		/* Selected window. */
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
		if ((qelt->event.xany.window == w) &&
		    (_event_to_mask[qelt->event.type] & mask) &&
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
