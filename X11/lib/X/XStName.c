#include "copyright.h"

/* $XConsortium: XStName.c,v 11.12 89/12/11 19:10:32 rws Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"
#include "Xatom.h"

#if NeedFunctionPrototypes
XStoreName (
    register Display *dpy,
    Window w,
    const char *name)
#else
XStoreName (dpy, w, name)
    register Display *dpy;
    Window w;
    char *name;
#endif
{
    XChangeProperty(dpy, w, XA_WM_NAME, XA_STRING, 
		8, PropModeReplace, (unsigned char *)name,
                name ? strlen(name) : 0);
}

#if NeedFunctionPrototypes
XSetIconName (
    register Display *dpy,
    Window w,
    const char *icon_name)
#else
XSetIconName (dpy, w, icon_name)
    register Display *dpy;
    Window w;
    char *icon_name;
#endif
{
    XChangeProperty(dpy, w, XA_WM_ICON_NAME, XA_STRING, 
		8, PropModeReplace, (unsigned char *)icon_name,
		icon_name ? strlen(icon_name) : 0);
}
