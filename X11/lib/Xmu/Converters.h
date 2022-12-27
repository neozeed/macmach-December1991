/*
 * $XConsortium: Converters.h,v 1.3 89/10/09 16:25:30 jim Exp $
 *
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
 * The X Window System is a Trademark of MIT.
 *
 * The interfaces described by this header file are for miscellaneous utilities
 * and are not part of the Xlib standard.
 */

#ifndef _XMU_STRCONVERT_H_
#define _XMU_STRCONVERT_H_

/*
 * Converters - insert in alphabetical order
 */

/******************************************************************************
 * XmuCvtFunctionToCallback
 */
extern void XmuCvtFunctionToCallback();


/******************************************************************************
 * XmuCvtStringToBackingStore
 */
#define XtNbackingStore "backingStore"
#define XtCBackingStore "BackingStore"
#define XtRBackingStore "BackingStore"
#define XtEnotUseful "notUseful"
#define XtEwhenMapped "whenMapped"
#define XtEalways "always"
#define XtEdefault "default"
extern void XmuCvtStringToBackingStore();


/******************************************************************************
 * XmuCvtStringToCursor
 */
extern void XmuCvtStringToCursor();


/******************************************************************************
 * XmuCvtStringToJustify
 */
typedef enum {
    XtJustifyLeft,       /* justify text to left side of button   */
    XtJustifyCenter,     /* justify text in center of button      */
    XtJustifyRight       /* justify text to right side of button  */
} XtJustify;
#define XtEleft "left"
#define XtEcenter "center"
#define XtEright "right"
extern void XmuCvtStringToJustify();


/******************************************************************************
 * XmuCvtStringToLong
 */
#define XtRLong "Long"
extern void XmuCvtStringToLong();


/******************************************************************************
 * XmuCvtStringToOrientation
 */
typedef enum {XtorientHorizontal, XtorientVertical} XtOrientation;
extern void XmuCvtStringToOrientation();


/******************************************************************************
 * XmuCvtStringToBitmap
 */
extern void XmuCvtStringToBitmap();


/******************************************************************************
 * XmuCvtStringToShapeStyle; is XtTypeConverter (i.e. new style)
 * no conversion arguments, not particularly useful to cache the results.
 */

#define XtRShapeStyle "ShapeStyle"
#define XtERectangle "Rectangle"
#define XtEOval "Oval"
#define XtEEllipse "Ellipse"
#define XtERoundedRectangle "RoundedRectangle"

#define XmuShapeRectangle 1
#define XmuShapeOval 2
#define XmuShapeEllipse 3
#define XmuShapeRoundedRectangle 4

extern Boolean XmuCvtStringToShapeStyle();
extern Boolean XmuReshapeWidget( /*
    Widget w,
    int shape_style,
    int corner_width,
    int corner_height
    */ );

/******************************************************************************
 * XmuCvtStringToWidget
 */
extern void XmuCvtStringToWidget();


#endif /* _XMU_STRCONVERT_H_ */
