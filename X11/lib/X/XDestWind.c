#include "copyright.h"

/* $XConsortium: XDestWind.c,v 11.6 88/09/06 16:06:26 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"
XDestroyWindow (dpy, w)
	register Display *dpy;
	Window w;
{
        register xResourceReq *req;

	LockDisplay(dpy);
        GetResReq(DestroyWindow, w, req);
	UnlockDisplay(dpy);
	SyncHandle();
}

