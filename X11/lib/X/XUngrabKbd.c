#include "copyright.h"

/* $XConsortium: XUngrabKbd.c,v 11.9 88/09/06 16:11:10 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"
XUngrabKeyboard (dpy, time)
        register Display *dpy;
	Time time;
{
        register xResourceReq *req;

	LockDisplay(dpy);
        GetResReq(UngrabKeyboard, time, req);
	UnlockDisplay(dpy);
	SyncHandle();
}

