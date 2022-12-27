#include "copyright.h"

/* $XConsortium: XGetIFocus.c,v 11.9 88/09/06 16:08:01 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#define NEED_REPLIES
#include "Xlibint.h"

XGetInputFocus(dpy, focus, revert_to)
     register Display *dpy;
     Window *focus;
     int *revert_to;
{       
    xGetInputFocusReply rep;
    register xReq *req;
    LockDisplay(dpy);
    GetEmptyReq(GetInputFocus, req);
    (void) _XReply (dpy, (xReply *)&rep, 0, xTrue);
    *focus = rep.focus;
    *revert_to = rep.revertTo;
    UnlockDisplay(dpy);
    SyncHandle();
}

