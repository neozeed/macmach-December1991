/*
 * $XConsortium: XErrDes.c,v 11.38 89/12/11 19:09:02 rws Exp $
 */

/***********************************************************
Copyright 1987, 1988 by Digital Equipment Corporation, Maynard, Massachusetts,
and the Massachusetts Institute of Technology, Cambridge, Massachusetts.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the names of Digital or MIT not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  

DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/

#include <stdio.h>
#include <X11/Xos.h>
#include "Xlibint.h"
#include "Xresource.h"

#ifndef ERRORDB
#define ERRORDB "/usr/lib/X11/XErrorDB"
#endif

/*
 * descriptions of errors in Section 4 of Protocol doc (pp. 350-351); more
 * verbose descriptions are given in the error database
 */
static char *_XErrorList[] = {
    /* No error	*/		"no error",
    /* BadRequest */		"BadRequest",
    /* BadValue	*/		"BadValue",
    /* BadWindow */		"BadWindow",
    /* BadPixmap */		"BadPixmap",
    /* BadAtom */		"BadAtom",
    /* BadCursor */		"BadCursor",
    /* BadFont */		"BadFont",
    /* BadMatch	*/		"BadMatch",
    /* BadDrawable */		"BadDrawable",
    /* BadAccess */		"BadAccess",
    /* BadAlloc	*/		"BadAlloc",
    /* BadColor */  		"BadColor",
    /* BadGC */  		"BadGC",
    /* BadIDChoice */		"BadIDChoice",
    /* BadName */		"BadName",
    /* BadLength */		"BadLength",
    /* BadImplementation */	"BadImplementation",
};
static int _XErrorListSize = sizeof(_XErrorList);


XGetErrorText(dpy, code, buffer, nbytes)
    register int code;
    register Display *dpy;
    char *buffer;
    int nbytes;
{
    char buf[32];
    register _XExtension *ext;

    if (nbytes == 0) return;
    sprintf(buf, "%d", code);
    if (code <= (_XErrorListSize/ sizeof (char *)) && code > 0) {
	XGetErrorDatabaseText(dpy, "XProtoError", buf, _XErrorList[code],
			      buffer, nbytes);
	}
    ext = dpy->ext_procs;
    while (ext) {		/* call out to any extensions interested */
 	if (ext->error_string != NULL) 
 	    (*ext->error_string)(dpy, code, &ext->codes, buffer, nbytes);
 	ext = ext->next;
    }    
    return;
}

#if NeedFunctionPrototypes
/*ARGSUSED*/
XGetErrorDatabaseText(
    Display *dpy,
    register const char *name,
    register const char *type,
    const char *defaultp,
    char *buffer,
    int nbytes)
#else
/*ARGSUSED*/
XGetErrorDatabaseText(dpy, name, type, defaultp, buffer, nbytes)
    Display *dpy;
    register char *name, *type;
    char *defaultp;
    char *buffer;
    int nbytes;
#endif
{

    static XrmDatabase db;
    static int initialized = False;
    XrmString type_str;
    XrmValue result;
    char temp[BUFSIZ];

    if (nbytes == 0) return;
    if (!initialized) {
	XrmInitialize();
	db = XrmGetFileDatabase(ERRORDB);
	initialized = True;
    }
    if (db)
    {
	sprintf(temp, "%s.%s", name, type);
	XrmGetResource(db, temp, "ErrorType.ErrorNumber", &type_str, &result);
    }
    else
	result.addr = (caddr_t)NULL;
    if (!result.addr) {
	result.addr = (caddr_t) defaultp;
	result.size = strlen(defaultp) + 1;
    }
    (void) strncpy (buffer, (char *) result.addr, nbytes);
    if (result.size > nbytes) buffer[nbytes-1] = '\0';
}
