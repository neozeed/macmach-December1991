#include "copyright.h"

/* $XConsortium: XQuExt.c,v 11.15 89/12/11 19:10:04 rws Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#define NEED_REPLIES
#include "Xlibint.h"

#if NeedFunctionPrototypes
Bool XQueryExtension(
    register Display *dpy,
    const char *name,
    int *major_opcode,  /* RETURN */
    int *first_event,   /* RETURN */
    int *first_error)	/* RETURN */
#else
Bool XQueryExtension(dpy, name, major_opcode, first_event, first_error)
    register Display *dpy;
    char *name;
    int *major_opcode;  /* RETURN */
    int *first_event;   /* RETURN */
    int *first_error;	/* RETURN */
#endif
{       
    xQueryExtensionReply rep;
    register xQueryExtensionReq *req;

    LockDisplay(dpy);
    GetReq(QueryExtension, req);
    req->nbytes = name ? strlen(name) : 0;
    req->length += (req->nbytes+3)>>2;
    _XSend(dpy, name, (long)req->nbytes);
    (void) _XReply (dpy, (xReply *)&rep, 0, xTrue);
    *major_opcode = rep.major_opcode;
    *first_event = rep.first_event;
    *first_error = rep.first_error;
    UnlockDisplay(dpy);
    SyncHandle();
    return (rep.present);
}

