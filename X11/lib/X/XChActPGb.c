#include "copyright.h"

/* $XConsortium: XChActPGb.c,v 11.7 88/09/06 16:04:21 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

XChangeActivePointerGrab(dpy, event_mask, curs, time)
register Display *dpy;
unsigned int event_mask; /* CARD16 */
Cursor curs;
Time time;
{
    register xChangeActivePointerGrabReq *req;

    LockDisplay(dpy);
    GetReq(ChangeActivePointerGrab, req);
    req->eventMask = event_mask;
    req->cursor = curs;
    req->time = time;
    UnlockDisplay(dpy);
    SyncHandle();
}
