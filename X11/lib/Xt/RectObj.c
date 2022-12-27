/* LINTLIBRARY */
#ifndef lint
static char Xrcsid[] = "$XConsortium: RectObj.c,v 1.10 89/09/12 16:48:15 swick Exp $";
/* $oHeader: RectObj.c,v 1.2 88/08/18 15:51:21 asente Exp $ */
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

#define RECTOBJ
#include "IntrinsicI.h"
#include "StringDefs.h"
/******************************************************************
 *
 * Rectangle Object Resources
 *
 ******************************************************************/

static XtResource resources[] = {

    {XtNancestorSensitive, XtCSensitive, XtRBoolean, sizeof(Boolean),
      XtOffset(RectObj,rectangle.ancestor_sensitive),XtRCallProc,
      (XtPointer)XtCopyAncestorSensitive},
    {XtNx, XtCPosition, XtRPosition, sizeof(Position),
         XtOffset(RectObj,rectangle.x), XtRImmediate, (XtPointer)0},
    {XtNy, XtCPosition, XtRPosition, sizeof(Position),
         XtOffset(RectObj,rectangle.y), XtRImmediate, (XtPointer)0},
    {XtNwidth, XtCWidth, XtRDimension, sizeof(Dimension),
         XtOffset(RectObj,rectangle.width), XtRImmediate, (XtPointer)0},
    {XtNheight, XtCHeight, XtRDimension, sizeof(Dimension),
         XtOffset(RectObj,rectangle.height), XtRImmediate, (XtPointer)0},
    {XtNborderWidth, XtCBorderWidth, XtRDimension, sizeof(Dimension),
         XtOffset(RectObj,rectangle.border_width), XtRImmediate, (XtPointer)1},
    {XtNsensitive, XtCSensitive, XtRBoolean, sizeof(Boolean),
         XtOffset(RectObj,rectangle.sensitive), XtRImmediate, (XtPointer)True}
    };

static void RectObjInitialize();
static void RectClassPartInitialize();
static void RectSetValuesAlmost();

externaldef(rectobjclassrec) RectObjClassRec rectObjClassRec = {
  {
    /* superclass	  */	(WidgetClass)&objectClassRec,
    /* class_name	  */	"Rect",
    /* widget_size	  */	sizeof(RectObjRec),
    /* class_initialize   */    NULL,
    /* class_part_initialize*/	RectClassPartInitialize,
    /* class_inited       */	FALSE,
    /* initialize	  */	RectObjInitialize,
    /* initialize_hook    */	NULL,		
    /* realize		  */	NULL,
    /* actions		  */	NULL,
    /* num_actions	  */	0,
    /* resources	  */	resources,
    /* num_resources	  */	XtNumber(resources),
    /* xrm_class	  */	NULLQUARK,
    /* compress_motion	  */	FALSE,
    /* compress_exposure  */	TRUE,
    /* compress_enterleave*/ 	FALSE,
    /* visible_interest	  */	FALSE,
    /* destroy		  */	NULL,
    /* resize		  */	NULL,
    /* expose		  */	NULL,
    /* set_values	  */	NULL,
    /* set_values_hook    */	NULL,			
    /* set_values_almost  */	RectSetValuesAlmost,  
    /* get_values_hook    */	NULL,			
    /* accept_focus	  */	NULL,
    /* version		  */	XtVersion,
    /* callback_offsets   */    NULL,
    /* tm_table           */    NULL,
    /* query_geometry	    */  NULL,
    /* display_accelerator  */	NULL,
    /* extension	    */  NULL
  }
};

externaldef(rectObjClass)
WidgetClass rectObjClass = (WidgetClass)&rectObjClassRec;

/*
 * Start of rectangle object methods
 */


static void RectClassPartInitialize(wc)
    register WidgetClass wc;
{
    register RectObjClass roc = (RectObjClass)wc;
    register RectObjClass super = ((RectObjClass)roc->rect_class.superclass);

    /* We don't need to check for null super since we'll get to object
       eventually, and it had better define them!  */


    if (roc->rect_class.resize == XtInheritResize) {
	roc->rect_class.resize = super->rect_class.resize;
    }

    if (roc->rect_class.expose == XtInheritExpose) {
	roc->rect_class.expose = super->rect_class.expose;
    }

    if (roc->rect_class.set_values_almost == XtInheritSetValuesAlmost) {
	roc->rect_class.set_values_almost = super->rect_class.set_values_almost;
    }


    if (roc->rect_class.query_geometry == XtInheritQueryGeometry) {
	roc->rect_class.query_geometry = super->rect_class.query_geometry;
    }
}

/* ARGSUSED */
static void RectObjInitialize(requested_widget, new_widget)
    Widget   requested_widget;
    register Widget new_widget;
{
    ((RectObj)new_widget)->rectangle.managed = FALSE;
}

/*ARGSUSED*/
static void RectSetValuesAlmost(old, new, request, reply)
    Widget		old;
    Widget		new;
    XtWidgetGeometry    *request;
    XtWidgetGeometry    *reply;
{
    *request = *reply;
}
