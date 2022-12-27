/* $XConsortium: ResourceI.h,v 1.6 89/11/14 14:23:06 swick Exp $ */
/* $oHeader: ResourceI.h,v 1.2 88/08/18 15:56:00 asente Exp $ */
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

/****************************************************************
 *
 * Resources
 *
 ****************************************************************/

#define StringToQuark(string) XrmStringToQuark(string)
#define StringToName(string) XrmStringToName(string)
#define StringToClass(string) XrmStringToClass(string)

typedef struct _XtTypedArg* XtTypedArgList;

extern void XtGetResources(
#if NeedFunctionPrototypes
    Widget       /* widget */,
    ArgList      /* args */,
    int          /* num_args */
#endif
);

extern void _XtResourceDependencies(
#if NeedFunctionPrototypes
    WidgetClass  /* wc */
#endif
);

extern void _XtConstraintResDependencies(
#if NeedFunctionPrototypes
    ConstraintWidgetClass  /* wc */
#endif
);

extern XtCacheRef* _XtGetResources(
#if NeedFunctionPrototypes
    Widget	    /* w */,
    ArgList	    /* args */,
    Cardinal	    /* num_args */,
    XtTypedArgList  /* typed_args */,
    Cardinal*	    /* num_typed_args */
#endif
);
