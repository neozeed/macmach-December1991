#include "copyright.h"

/* $XConsortium: XFreePix.c,v 11.6 88/09/06 16:07:34 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

XFreePixmap(dpy, pixmap)
    register Display *dpy;
    Pixmap pixmap;

{   
    register xResourceReq *req;    
    LockDisplay(dpy);
    GetResReq(FreePixmap, pixmap, req);
    UnlockDisplay(dpy);
    SyncHandle();
}

