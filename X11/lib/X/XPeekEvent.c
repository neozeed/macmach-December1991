#include "copyright.h"

/* $XConsortium: XPeekEvent.c,v 11.13 88/09/06 16:09:31 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#define NEED_EVENTS
#include "Xlibint.h"

extern _XQEvent *_qfree;

/* 
 * Return the next event in the queue,
 * BUT do not remove it from the queue.
 * If none found, flush and wait until there is an event to peek.
 */

XPeekEvent (dpy, event)
	register Display *dpy;
	register XEvent *event;
{
	LockDisplay(dpy);
	if (dpy->head == NULL)
	    _XReadEvents(dpy);
	*event = (dpy->head)->event;
	UnlockDisplay(dpy);
}

