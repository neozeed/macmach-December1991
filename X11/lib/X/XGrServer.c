#include "copyright.h"

/* $XConsortium: XGrServer.c,v 11.6 88/09/06 16:08:36 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"
XGrabServer (dpy)
register Display *dpy;
{
	register xReq *req;
	LockDisplay(dpy);
        GetEmptyReq(GrabServer, req);
	UnlockDisplay(dpy);
	SyncHandle();
}

