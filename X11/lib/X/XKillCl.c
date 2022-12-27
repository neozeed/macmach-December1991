#include "copyright.h"

/* $XConsortium: XKillCl.c,v 11.8 88/09/06 16:08:52 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"
XKillClient(dpy, resource)
	register Display *dpy;
	XID resource;
{
	register xResourceReq *req;
	LockDisplay(dpy);
        GetResReq(KillClient, resource, req);
	UnlockDisplay(dpy);
	SyncHandle();
}

