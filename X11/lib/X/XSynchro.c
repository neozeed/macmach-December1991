#include "copyright.h"

/* $XConsortium: XSynchro.c,v 11.7 89/07/23 17:52:55 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"


int _XSyncFunction(dpy)
register Display *dpy;
{
	XSync(dpy,0);
	return 0;
}

int (*XSynchronize(dpy,onoff))()
     register Display *dpy;
     int onoff;
{
        int (*temp)();

	LockDisplay(dpy);
	temp = dpy->synchandler;
	if (onoff) dpy->synchandler = _XSyncFunction;
	else dpy->synchandler = NULL;
	UnlockDisplay(dpy);
	return (temp);
}

int (*XSetAfterFunction(dpy,func))()
     register Display *dpy;
     int (*func)();
{
        int (*temp)();

	LockDisplay(dpy);
	temp = dpy->synchandler;
	dpy->synchandler = func;
	UnlockDisplay(dpy);
	return (temp);
}

