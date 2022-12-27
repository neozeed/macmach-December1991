#include "copyright.h"

/* $XConsortium: XUnmapWin.c,v 11.6 88/09/06 16:11:21 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"
XUnmapWindow (dpy, w)
        register Display *dpy;
	Window w;
{
        register xResourceReq *req;

        LockDisplay(dpy);
        GetResReq(UnmapWindow, w, req);
	UnlockDisplay(dpy);
	SyncHandle();
}

