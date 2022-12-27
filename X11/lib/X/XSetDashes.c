#include "copyright.h"

/* $XConsortium: XSetDashes.c,v 11.10 89/12/11 19:10:15 rws Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

#if NeedFunctionPrototypes
XSetDashes (
    register Display *dpy,
    GC gc,
    int dash_offset,
    const char *list,
    int n)
#else
XSetDashes (dpy, gc, dash_offset, list, n)
    register Display *dpy;
    GC gc;
    int dash_offset;
    char *list;
    int n;
#endif
    {
    register xSetDashesReq *req;

    LockDisplay(dpy);
    GetReq (SetDashes,req);
    req->gc = gc->gid;
    req->dashOffset = gc->values.dash_offset = dash_offset;
    req->nDashes = n;
    req->length += (n+3)>>2;
    gc->dashes = 1;
    gc->dirty &= ~(GCDashList | GCDashOffset);
    Data (dpy, list, (long)n);
    UnlockDisplay(dpy);
    SyncHandle();
    }
    
