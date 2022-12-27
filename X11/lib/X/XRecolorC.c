#include "copyright.h"

/* $XConsortium: XRecolorC.c,v 11.7 88/09/06 16:10:13 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

XRecolorCursor(dpy, cursor, foreground, background)
     register Display *dpy;
     Cursor cursor;
     XColor *foreground, *background;
{       
    register xRecolorCursorReq *req;

    LockDisplay(dpy);
    GetReq(RecolorCursor, req);
    req->cursor = cursor;
    req->foreRed = foreground->red;
    req->foreGreen = foreground->green;
    req->foreBlue = foreground->blue;
    req->backRed = background->red;
    req->backGreen = background->green;
    req->backBlue = background->blue;
    UnlockDisplay(dpy);
    SyncHandle();
}

