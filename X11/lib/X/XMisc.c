#include "copyright.h"

/* $XConsortium: XMisc.c,v 1.3 88/09/06 16:10:30 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1988	*/

#include "Xlibint.h"

long XMaxRequestSize(dpy)
    Display *dpy;
{
    return dpy->max_request_size;
}

char *XResourceManagerString(dpy)
    Display *dpy;
{
    return dpy->xdefaults;
}

unsigned long XDisplayMotionBufferSize(dpy)
    Display *dpy;
{
    return dpy->motion_buffer;
}

XDisplayKeycodes(dpy, min_keycode_return, max_keycode_return)
    Display *dpy;
    int *min_keycode_return, *max_keycode_return;
{
    *min_keycode_return = dpy->min_keycode;
    *max_keycode_return = dpy->max_keycode;
}

VisualID XVisualIDFromVisual(visual)
    Visual *visual;
{
    return visual->visualid;
}
