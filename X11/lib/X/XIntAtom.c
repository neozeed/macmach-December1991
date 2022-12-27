#include "copyright.h"

/* $XConsortium: XIntAtom.c,v 11.13 89/12/11 19:09:36 rws Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#define NEED_REPLIES
#include "Xlibint.h"

#if NeedFunctionPrototypes
Atom XInternAtom (
    register Display *dpy,
    const char *name,
    Bool onlyIfExists)
#else
Atom XInternAtom (dpy, name, onlyIfExists)
    register Display *dpy;
    char *name;
    Bool onlyIfExists;
#endif
{
    register long nbytes;
    xInternAtomReply rep;
    register xInternAtomReq *req;
    LockDisplay(dpy);
    GetReq(InternAtom, req);
    nbytes = req->nbytes = name ? strlen(name) : 0;
    req->onlyIfExists = onlyIfExists;
    req->length += (nbytes+3)>>2;
    _XSend (dpy, name, nbytes);
    	/* use _XSend instead of Data, since the following _XReply
           will always flush the buffer anyway */
    if(_XReply (dpy, (xReply *)&rep, 0, xTrue) == 0) rep.atom = None;
    UnlockDisplay(dpy);
    SyncHandle();
    return (rep.atom);
}

