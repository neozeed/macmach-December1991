#include "copyright.h"

/* $XConsortium: XAutoRep.c,v 11.8 88/09/06 16:04:07 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1985	*/

#include "Xlibint.h"
XAutoRepeatOn (dpy)
register Display *dpy;
{
	XKeyboardControl values;
	values.auto_repeat_mode = AutoRepeatModeOn;
	XChangeKeyboardControl (dpy, KBAutoRepeatMode, &values);
}

XAutoRepeatOff (dpy)
register Display *dpy;
{
	XKeyboardControl values;
	values.auto_repeat_mode = AutoRepeatModeOff;
	XChangeKeyboardControl (dpy, KBAutoRepeatMode, &values);
}


