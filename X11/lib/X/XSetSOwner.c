#include "copyright.h"

/* $XConsortium: XSetSOwner.c,v 11.8 88/09/06 16:10:42 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

XSetSelectionOwner(dpy, selection, owner, time)
register Display *dpy;
Atom selection;
Window owner;
Time time;
{
    register xSetSelectionOwnerReq *req;

    LockDisplay(dpy);
    GetReq(SetSelectionOwner,req);
    req->selection = selection;
    req->window = owner;
    req->time = time;
    UnlockDisplay(dpy);
    SyncHandle();
}
