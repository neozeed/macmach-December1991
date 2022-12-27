#include "copyright.h"

/* $XConsortium: XGetSOwner.c,v 11.13 88/09/06 16:08:17 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#define NEED_REPLIES
#include "Xlibint.h"

Window XGetSelectionOwner(dpy, selection)
register Display *dpy;
Atom selection;
{
    xGetSelectionOwnerReply rep;
    register xResourceReq *req;
    LockDisplay(dpy);
    GetResReq(GetSelectionOwner, selection, req);

    if (_XReply(dpy, (xReply *)&rep, 0, xTrue) == 0) rep.owner = None;
    UnlockDisplay(dpy);
    SyncHandle();
    return(rep.owner);
}
