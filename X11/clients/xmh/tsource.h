/*
 * $XConsortium: tsource.h,v 1.1 89/09/01 17:36:45 kit Exp $
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

#ifndef _tsource_h
#define _tsource_h

/***********************************************************************
 *
 * TextSrc Widget
 *
 ***********************************************************************/

#include <X11/Xaw/TextSrc.h>

/* Resources:

 Name		     Class		RepType		Default Value
 ----		     -----		-------		-------------
 toc                 Toc                Pointer         NULL

*/

#define XtCToc "Toc"

#define XtNtoc "toc"

/* Class record constants */

extern WidgetClass tocSourceWidgetClass;

typedef struct _TocSourceClassRec *TocSourceWidgetClass;
typedef struct _TocSourceRec      *TocSourceWidget;

#endif /* _XawTextSrc_h */
/* DON'T ADD STUFF AFTER THIS #endif */
