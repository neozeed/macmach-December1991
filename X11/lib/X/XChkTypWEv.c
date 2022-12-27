#include "copyright.h"

/* $XConsortium: XChkTypWEv.c,v 11.6 88/09/06 16:09:13 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1985, 1987	*/
#define NEED_EVENTS
#include "Xlibint.h"

extern _XQEvent *_qfree;
/* 
 * Check existing events in queue to find if any match.  If so, return.
 * If not, flush buffer and see if any more events are readable. If one
 * matches, return.  If all else fails, tell the user no events found.
 */

Bool XCheckTypedWindowEvent (dpy, w, type, event)
        register Display *dpy;
	Window w;		/* Selected window. */
	int type;		/* Selected event type. */
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
		    (qelt->event.type == type)) {
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
