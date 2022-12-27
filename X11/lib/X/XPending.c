#include "copyright.h"

/* $XConsortium: XPending.c,v 11.13 88/09/06 16:09:35 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

/* Read in pending events if needed and return the number of queued events. */

int XEventsQueued (dpy, mode)
    register Display *dpy;
    int mode;
{
    int ret_val;
    LockDisplay(dpy);
    if (dpy->qlen || (mode == QueuedAlready))
	ret_val = dpy->qlen;
    else
	ret_val = _XEventsQueued (dpy, mode);
    UnlockDisplay(dpy);
    return ret_val;
}

int XPending (dpy)
    register Display *dpy;
{
    int ret_val;
    LockDisplay(dpy);
    if (dpy->qlen)
	ret_val = dpy->qlen;
    else
	ret_val = _XEventsQueued (dpy, QueuedAfterFlush);
    UnlockDisplay(dpy);
    return ret_val;
}
