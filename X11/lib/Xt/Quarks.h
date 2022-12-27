/* $XConsortium: Quarks.h,v 1.12 89/11/10 17:39:09 swick Exp $ */
/* $oHeader: Quarks.h,v 1.3 88/08/19 16:43:21 asente Exp $ */

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

#ifndef _XtQuarks_h_
#define _XtQuarks_h_


/* Representation types */

extern	XrmQuark  XtQBool;
extern	XrmQuark  XtQBoolean;
extern	XrmQuark  XtQColor;
extern	XrmQuark  XtQCursor;
extern	XrmQuark  XtQDimension;
extern	XrmQuark  XtQDisplay;
extern	XrmQuark  XtQFile;
extern	XrmQuark  XtQFont;
extern	XrmQuark  XtQFontStruct;
extern	XrmQuark  XtQInt;
extern	XrmQuark  XtQPixel;
extern	XrmQuark  XtQPixmap;
extern	XrmQuark  XtQPointer;
extern  XrmQuark  XtQPosition;
extern  XrmQuark  XtQShort;
extern	XrmQuark  XtQString;
extern  XrmQuark  XtQUnsignedChar;
extern	XrmQuark  XtQWindow;

/* Boolean enumeration constants */

extern	XrmQuark  XtQEoff;
extern	XrmQuark  XtQEfalse;
extern	XrmQuark  XtQEno;
extern	XrmQuark  XtQEon;
extern	XrmQuark  XtQEtrue;
extern	XrmQuark  XtQEyes;

/* color enumeration constants */

extern XrmQuark XtQExtdefaultbackground;
extern XrmQuark XtQExtdefaultforeground;

/* font constant */

extern XrmQuark XtQExtdefaultfont;

#endif /* _XrmQuarks_h_ */
