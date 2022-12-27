#include "copyright.h"

/* $XConsortium: XUngrabBut.c,v 11.6 88/09/06 16:11:09 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

XUngrabButton(dpy, button, modifiers, grab_window)
register Display *dpy;
unsigned int button; /* CARD8 */
unsigned int modifiers; /* CARD16 */
Window grab_window;
{
    register xUngrabButtonReq *req;

    LockDisplay(dpy);
    GetReq(UngrabButton, req);
    req->button = button;
    req->modifiers = modifiers;
    req->grabWindow = grab_window;
    UnlockDisplay(dpy);
    SyncHandle();
}
