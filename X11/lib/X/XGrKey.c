#include "copyright.h"

/* $XConsortium: XGrKey.c,v 11.7 88/09/06 16:08:28 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

XGrabKey(dpy, key, modifiers, grab_window, owner_events, 
	 pointer_mode, keyboard_mode)
    register Display *dpy;
    int key;
    unsigned int modifiers;
    Window grab_window;
    Bool owner_events;
    int pointer_mode, keyboard_mode;

{
    register xGrabKeyReq *req;
    LockDisplay(dpy);
    GetReq(GrabKey, req);
    req->ownerEvents = owner_events;
    req->grabWindow = grab_window;
    req->modifiers = modifiers;
    req->key = key;
    req->pointerMode = pointer_mode;
    req->keyboardMode = keyboard_mode;
    UnlockDisplay(dpy);
    SyncHandle();
}



