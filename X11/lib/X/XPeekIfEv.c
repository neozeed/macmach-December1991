#include "copyright.h"

/* $XConsortium: XPeekIfEv.c,v 11.11 88/09/06 16:09:33 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#define NEED_EVENTS
#include "Xlibint.h"

extern _XQEvent *_qfree;

/*
 * return the next event in the queue that satisfies the predicate.
 * BUT do not remove it from the queue.
 * If none found, flush, and then wait until one satisfies the predicate.
 */

XPeekIfEvent (dpy, event, predicate, arg)
	register Display *dpy;
	register XEvent *event;
	Bool (*predicate)();
	char *arg;
{
	register _XQEvent *prev, *qelt;

	LockDisplay(dpy);
	prev = NULL;
	while (1) {
	    for (qelt = prev ? prev->next : dpy->head;
		 qelt;
		 prev = qelt, qelt = qelt->next) {
		if ((*predicate)(dpy, &qelt->event, arg)) {
		    *event = qelt->event;
		    UnlockDisplay(dpy);
		    return;
		}
	    }
	    _XReadEvents(dpy);
	}
}

