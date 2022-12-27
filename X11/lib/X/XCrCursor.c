#include "copyright.h"

/* $XConsortium: XCrCursor.c,v 11.7 88/09/06 16:05:43 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

Cursor XCreatePixmapCursor(dpy, source, mask, foreground, background, x, y)
     register Display *dpy;
     Pixmap source, mask;
     XColor *foreground, *background;
     unsigned int  x, y;

{       
    register xCreateCursorReq *req;
    Cursor cid;

    LockDisplay(dpy);
    GetReq(CreateCursor, req);
    req->cid = cid = XAllocID(dpy);
    req->source = source;
    req->mask = mask;
    req->foreRed = foreground->red;
    req->foreGreen = foreground->green;
    req->foreBlue = foreground->blue;
    req->backRed = background->red;
    req->backGreen = background->green;
    req->backBlue = background->blue;
    req->x = x;
    req->y = y;
    UnlockDisplay(dpy);
    SyncHandle();
    return (cid);
}

