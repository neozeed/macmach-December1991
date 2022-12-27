#include "copyright.h"

/* $XConsortium: XUnldFont.c,v 11.6 88/09/06 16:11:16 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

XUnloadFont(dpy, font)
     register Display *dpy;
     Font font;

{       
    register xResourceReq *req;

    LockDisplay(dpy);
    GetResReq(CloseFont, font, req);
    UnlockDisplay(dpy);
    SyncHandle();
}

