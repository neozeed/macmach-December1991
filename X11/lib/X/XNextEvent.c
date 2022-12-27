#include "copyright.h"

/* $XConsortium: XNextEvent.c,v 11.15 88/09/06 16:09:22 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#define NEED_EVENTS
#include "Xlibint.h"

extern _XQEvent *_qfree;

/* 
 * Return next event in queue, or if none, flush output and wait for
 * events.
 */

XNextEvent (dpy, event)
	register Display *dpy;
	register XEvent *event;
{
	register _XQEvent *qelt;
	
	LockDisplay(dpy);
	
	if (dpy->head == NULL)
	    _XReadEvents(dpy);
	qelt = dpy->head;
	*event = qelt->event;

	/* move the head of the queue to the free list */
	if ((dpy->head = qelt->next) == NULL)
	    dpy->tail = NULL;
	qelt->next = _qfree;
	_qfree = qelt;
	dpy->qlen--;
	UnlockDisplay(dpy);
	return;
}

