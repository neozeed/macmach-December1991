#include "copyright.h"

/* $XConsortium: XLiProps.c,v 11.19 89/11/08 17:07:19 converse Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#define NEED_REPLIES
#include "Xlibint.h"

Atom *XListProperties(dpy, window, n_props)
register Display *dpy;
Window window;
int *n_props;  /* RETURN */
{
    long nbytes;
    xListPropertiesReply rep;
    Atom *properties;
    register xResourceReq *req;

    LockDisplay(dpy);
    GetResReq(ListProperties, window, req);
    if (!_XReply(dpy, (xReply *)&rep, 0, xFalse)) {
	*n_props = 0;
	UnlockDisplay(dpy);
        SyncHandle();
	return ((Atom *) NULL);
    }

    if (rep.nProperties) {
	nbytes = rep.nProperties * sizeof(Atom);
	properties = (Atom *) Xmalloc ((unsigned) nbytes);
	nbytes = rep.nProperties << 2;
	if (! properties) {
	    _XEatData(dpy, (unsigned long) nbytes);
	    UnlockDisplay(dpy);
	    SyncHandle();
	    return (Atom *) NULL;
	}
	_XRead32 (dpy, (char *) properties, nbytes);
    }
    else properties = (Atom *) NULL;

    *n_props = rep.nProperties;
    UnlockDisplay(dpy);
    SyncHandle();
    return (properties);
}
