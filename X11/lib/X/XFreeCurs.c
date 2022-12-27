#include "copyright.h"

/* $XConsortium: XFreeCurs.c,v 11.6 88/09/06 16:07:29 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

XFreeCursor(dpy, cursor)
    register Display *dpy;
    Cursor cursor;
{
    register xResourceReq *req;
    LockDisplay(dpy);
    GetResReq(FreeCursor, cursor, req);
    UnlockDisplay(dpy);
    SyncHandle();
}

