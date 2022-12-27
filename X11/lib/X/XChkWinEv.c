#include "copyright.h"

/* $XConsortium: XChkWinEv.c,v 11.15 88/09/06 16:04:41 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1985, 1987	*/
#define NEED_EVENTS
#include "Xlibint.h"

extern _XQEvent *_qfree;
extern long _event_to_mask[];
#define AllPointers (PointerMotionMask|PointerMotionHintMask|ButtonMotionMask)
#define AllButtons (Button1MotionMask|Button2MotionMask|Button3MotionMask|\
		    Button4MotionMask|Button5MotionMask)

/* 
 * Check existing events in queue to find if any match.  If so, return.
 * If not, flush buffer and see if any more events are readable. If one
 * matches, return.  If all else fails, tell the user no events found.
 */

Bool XCheckWindowEvent (dpy, w, mask, event)
        register Display *dpy;
	Window w;		/* Selected window. */
	long mask;		/* Selected event mask. */
	register XEvent *event;	/* XEvent to be filled in. */
{
 	register _XQEvent *prev, *qelt;
	int n;              /* time through count */

        LockDisplay(dpy);
	prev = NULL;
	for (n = 3; --n >= 0;) {
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
		    return True;
		}
	    }
	    switch (n) {
	      case 2:
		_XEventsQueued(dpy, QueuedAfterReading);
		break;
	      case 1:
		_XFlush(dpy);
		break;
	    }
	}
	UnlockDisplay(dpy);
	return False;
}
