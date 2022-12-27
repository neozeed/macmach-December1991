static char rcsid[] =
	"$XConsortium: StrToJust.c,v 1.2 89/12/10 10:36:16 rws Exp $";

/* 
 * Copyright 1988 by the Massachusetts Institute of Technology
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided 
 * that the above copyright notice appear in all copies and that both that 
 * copyright notice and this permission notice appear in supporting 
 * documentation, and that the name of M.I.T. not be used in advertising
 * or publicity pertaining to distribution of the software without specific, 
 * written prior permission. M.I.T. makes no representations about the 
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 */

#include	<X11/Intrinsic.h>
#include	"Xmu.h"

/* ARGSUSED */
void
XmuCvtStringToJustify(args, num_args, fromVal, toVal)
    XrmValuePtr *args;		/* unused */
    Cardinal	*num_args;	/* unused */
    XrmValuePtr fromVal;
    XrmValuePtr toVal;
{
    static XtJustify	e;
    static XrmQuark	XrmQEleft;
    static XrmQuark	XrmQEcenter;
    static XrmQuark	XrmQEright;
    static int		haveQuarks;
    XrmQuark    q;
    char	*s = (char *) fromVal->addr;
    char        lowerName[1000];

    if (s == NULL) return;

    if (!haveQuarks) {
	XrmQEleft   = XrmStringToQuark(XtEleft);
	XrmQEcenter = XrmStringToQuark(XtEcenter);
	XrmQEright  = XrmStringToQuark(XtEright);
	haveQuarks = 1;
    }

    XmuCopyISOLatin1Lowered(lowerName, s);

    q = XrmStringToQuark(lowerName);

    toVal->size = sizeof(XtJustify);
    toVal->addr = (caddr_t) &e;

    if (q == XrmQEleft)   { e = XtJustifyLeft;   return; }
    if (q == XrmQEcenter) { e = XtJustifyCenter; return; }
    if (q == XrmQEright)  { e = XtJustifyRight;  return; }

    toVal->size = 0;
    toVal->addr = NULL;
}
