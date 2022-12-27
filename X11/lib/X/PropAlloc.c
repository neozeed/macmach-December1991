/* Copyright 1989 Massachusetts Institute of Technology */

#include "copyright.h"
#include <stdio.h>
#include "Xlibint.h"
#include "Xutil.h"


/*
 * Routines for allocating space for structures that are expected to get 
 * longer at some point.
 */

XSizeHints *XAllocSizeHints ()
{
    return ((XSizeHints *) Xcalloc (1, (unsigned) sizeof (XSizeHints)));
}


XStandardColormap *XAllocStandardColormap ()
{
    return ((XStandardColormap *)
	    Xcalloc (1, (unsigned) sizeof (XStandardColormap)));
}


XWMHints *XAllocWMHints ()
{
    return ((XWMHints *) Xcalloc (1, (unsigned) sizeof (XWMHints)));
}


XClassHint *XAllocClassHint ()
{
    register XClassHint *h;

    if (h = (XClassHint *) Xcalloc (1, (unsigned) sizeof (XClassHint))) 
      h->res_name = h->res_class = NULL;

    return h;
}


XIconSize *XAllocIconSize ()
{
    return ((XIconSize *) Xcalloc (1, (unsigned) sizeof (XIconSize)));
}


