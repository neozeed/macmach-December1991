#ifndef lint
static char Xrcsid[] = "$XConsortium: Alloc.c,v 1.29 89/11/03 07:58:12 swick Exp $";
/* $oHeader: Alloc.c,v 1.2 88/08/18 15:33:53 asente Exp $ */
#endif /* lint */

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

/*
 * X Toolkit Memory Allocation Routines
 *
 * Uses Xlib memory management, which is spec'd to be re-entrant.
 */

#include <X11/Xlib.h>
#include <X11/Xlibos.h>
#include "IntrinsicI.h"

extern void exit();

void _XtAllocError(type)
    String type;
{
    Cardinal num_params = 1;
    if (type == NULL) type = "local memory allocation";
    XtErrorMsg("allocError", type ,"XtToolkitError",
	       "Cannot perform %s", &type, &num_params);
}

char *XtMalloc(size)
    unsigned size;
{
    char *ptr;
    if ((ptr = Xmalloc(size)) == NULL)
        _XtAllocError("malloc");

    return(ptr);
}

char *XtRealloc(ptr, size)
    char     *ptr;
    unsigned size;
{
   if (ptr == NULL) return(XtMalloc(size));
   else if ((ptr = Xrealloc(ptr, size)) == NULL)
	_XtAllocError("realloc");

   return(ptr);
}

char *XtCalloc(num, size)
    unsigned num, size;
{
    char *ptr;

    if ((ptr = Xcalloc(num, size)) == NULL)
	_XtAllocError("calloc");

    return(ptr);
}

void XtFree(ptr)
    char *ptr;
{
   if (ptr != NULL) Xfree(ptr);
}


