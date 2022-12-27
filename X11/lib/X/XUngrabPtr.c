#include "copyright.h"

/* $XConsortium: XUngrabPtr.c,v 11.8 88/09/06 16:11:12 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

XUngrabPointer(dpy, time)
register Display *dpy;
Time time;
{
    register xResourceReq *req;

    LockDisplay(dpy);
    GetResReq(UngrabPointer, time, req);
    UnlockDisplay(dpy);
    SyncHandle();
}
