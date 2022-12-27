#include "copyright.h"

/***********************************************************
Copyright 1988 by Wyse Technology, Inc., San Jose, Ca.,
and the Massachusetts Institute of Technology, Cambridge, Massachusetts.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the names of Wyse or MIT not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  

WYSE DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/

#include <stdio.h>
#define NEED_EVENTS
#include "Xlibint.h"
#include "Xatom.h"
#include <X11/Xos.h>
#include "Xutil.h"

/*
 * This function instructs the window manager to change this window from
 * NormalState to IconicState.
 */
Status XIconifyWindow (dpy, w, screen)
    Display *dpy;
    Window w;
    int screen;
{
    XClientMessageEvent ev;
    Window root = RootWindow (dpy, screen);

    if (dpy->atoms->wm_change_state == None) {
	Atom a = XInternAtom (dpy, "WM_CHANGE_STATE", False);

	if (a == None) return False;
	LockDisplay (dpy);
	dpy->atoms->wm_change_state = a;
	UnlockDisplay (dpy);
    }

    ev.type = ClientMessage;
    ev.window = w;
    ev.message_type = dpy->atoms->wm_change_state;
    ev.format = 32;
    ev.data.l[0] = IconicState;
    return (XSendEvent (dpy, root, False,
			SubstructureRedirectMask|SubstructureNotifyMask,
			(XEvent *)&ev));
}
