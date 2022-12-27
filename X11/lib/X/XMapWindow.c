#include "copyright.h"

/* $XConsortium: XMapWindow.c,v 11.6 88/09/06 16:09:15 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"
XMapWindow (dpy, w)
	Window w;
	register Display *dpy;
{
	register xResourceReq *req;
	LockDisplay (dpy);
        GetResReq(MapWindow, w, req);
	UnlockDisplay (dpy);
	SyncHandle();
}

