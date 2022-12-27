#include "copyright.h"

/* $XConsortium: XLoadFont.c,v 11.9 89/12/11 19:09:43 rws Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

#if NeedFunctionPrototypes
Font XLoadFont (
    register Display *dpy,
    const char *name)
#else
Font XLoadFont (dpy, name)
    register Display *dpy;
    char *name;
#endif
{
    register long nbytes;
    Font fid;
    register xOpenFontReq *req;
    LockDisplay(dpy);
    GetReq(OpenFont, req);
    nbytes = req->nbytes = name ? strlen(name) : 0;
    req->fid = fid = XAllocID(dpy);
    req->length += (nbytes+3)>>2;
    Data (dpy, name, nbytes);
    UnlockDisplay(dpy);
    SyncHandle();
    return (fid); 
       /* can't return (req->fid) since request may have already been sent */
}

