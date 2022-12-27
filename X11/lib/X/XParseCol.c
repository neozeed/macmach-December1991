#include "copyright.h"

/* $XConsortium: XParseCol.c,v 11.18 89/12/11 19:09:55 rws Exp $ */
/* Copyright    Massachusetts Institute of Technology    1985	*/

#define NEED_REPLIES
#include "Xlibint.h"

#if NeedFunctionPrototypes
Status XParseColor (
	register Display *dpy,
        Colormap cmap,
	register const char *spec,
	XColor *def)
#else
Status XParseColor (dpy, cmap, spec, def)
	register Display *dpy;
        Colormap cmap;
	register char *spec;
	XColor *def;
#endif
{
	register int n, i;
	int r, g, b;
	char c;

        if (!spec) return(0);
	n = strlen (spec);
	if (*spec != '#') {
	    xLookupColorReply reply;
	    register xLookupColorReq *req;
	    LockDisplay(dpy);
	    GetReq (LookupColor, req);
	    req->cmap = cmap;
	    req->nbytes = n;
	    req->length += (n + 3) >> 2;
	    Data (dpy, spec, (long)n);
	    if (!_XReply (dpy, (xReply *) &reply, 0, xTrue)) {
		UnlockDisplay(dpy);
		SyncHandle();
	    	return (0);
		}
	    def->red = reply.exactRed;
	    def->green = reply.exactGreen;
	    def->blue = reply.exactBlue;
	    def->flags = DoRed | DoGreen | DoBlue;
	    UnlockDisplay(dpy);
	    SyncHandle();
	    return (1);
	}
	spec++;
	n--;
	if (n != 3 && n != 6 && n != 9 && n != 12)
	    return (0);
	n /= 3;
	g = b = 0;
	do {
	    r = g;
	    g = b;
	    b = 0;
	    for (i = n; --i >= 0; ) {
		c = *spec++;
		b <<= 4;
		if (c >= '0' && c <= '9')
		    b |= c - '0';
		else if (c >= 'A' && c <= 'F')
		    b |= c - ('A' - 10);
		else if (c >= 'a' && c <= 'f')
		    b |= c - ('a' - 10);
		else return (0);
	    }
	} while (*spec != '\0');
	n <<= 2;
	n = 16 - n;
	def->red = r << n;
	def->green = g << n;
	def->blue = b << n;
	def->flags = DoRed | DoGreen | DoBlue;
	return (1);
}
