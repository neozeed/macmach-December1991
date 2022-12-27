#ifdef REV_INFO
#ifndef lint
static char SCCSID[] ="OSF/Motif: @(#)Dog.c	1.11 91/01/10";
#endif /* lint */
#endif /* REV_INFO */
/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
*  (c) Copyright 1990 Alastair Gourlay
*  ALL RIGHTS RESERVED
*
*  	THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE WITHOUT
*  NOTICE AND SHOULD NOT BE CONSTRUED AS A COMMITMENT BY OPEN SOFTWARE
*  FOUNDATION, INC. OR ITS THIRD PARTY SUPPLIERS  
*  
*  	OPEN SOFTWARE FOUNDATION, INC. AND ITS THIRD PARTY SUPPLIERS,
*  ASSUME NO RESPONSIBILITY FOR THE USE OR INABILITY TO USE ANY OF ITS
*  SOFTWARE .   OSF SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
*  KIND, AND OSF EXPRESSLY DISCLAIMS ALL IMPLIED WARRANTIES, INCLUDING
*  BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
*  FITNESS FOR A PARTICULAR PURPOSE.
*  
* Open Software Foundation is a trademark of The Open Software Foundation, Inc.
* OSF is a trademark of Open Software Foundation, Inc.
* OSF/Motif is a trademark of Open Software Foundation, Inc.
* Motif is a trademark of Open Software Foundation, Inc.
* X Window System is a trademark of the Massachusetts Institute of Technology
*
*******************************************************************************
******************************************************************************/

/*****************************************************************************
*
*  Dog.c - Dog widget source file
*  
******************************************************************************/
#include <stdio.h>
#include <X11/StringDefs.h>
#include <Xm/Xm.h>
#include <Mrm/MrmPublic.h>
#include "DogP.h"

#include "up.bm"
#include "down.bm"
#include "bark.bm"

#define WagTime(w) XmField(w,offsets,Dog,wag_time, int)
#define BarkTime(w) XmField(w,offsets,Dog,bark_time, int)
#define BarkCallback(w) XmField(w,offsets,Dog,bark_callback,XtCallbackList)
#define UpPixmap(w) XmField(w,offsets,Dog,up_pixmap,Pixmap)
#define DownPixmap(w) XmField(w,offsets,Dog,down_pixmap,Pixmap)
#define BarkPixmap(w) XmField(w,offsets,Dog,bark_pixmap,Pixmap)
#define CurrPixmap(w) XmField(w,offsets,Dog,curr_pixmap,Pixmap)
#define CurrPx(w) XmField(w,offsets,Dog,curr_px,int)
#define Wagging(w) XmField(w,offsets,Dog,wagging,Boolean)
#define Barking(w) XmField(w,offsets,Dog,barking,Boolean)
#define DrawGC(w) XmField(w,offsets,Dog,draw_GC,GC)
#define PixmapX(w) XmField(w,offsets,Dog,pixmap_x,Position)
#define PixmapY(w) XmField(w,offsets,Dog,pixmap_y,Position)
#define DrawX(w) XmField(w,offsets,Dog,draw_x,Position)
#define DrawY(w) XmField(w,offsets,Dog,draw_y,Position)
#define DrawWidth(w) XmField(w,offsets,Dog,draw_width,Dimension)
#define DrawHeight(w) XmField(w,offsets,Dog,draw_height,Dimension)
#define CurrWidth(w) XmField(w,offsets,Dog,curr_width,Dimension)
#define CurrHeight(w) XmField(w,offsets,Dog,curr_height,Dimension)
#define HighlightThickness(w) \
		XmField(w,offsets,XmPrimitive,highlight_thickness,Dimension)
#define ShadowThickness(w) \
		XmField(w,offsets,XmPrimitive,shadow_thickness,Dimension)
#define Foreground(w)	XmField(w,offsets,XmPrimitive,foreground,Pixel)
#define Highlighted(w) XmField(w,offsets,XmPrimitive,highlighted,Boolean)
#define TopShadowGC(w) XmField(w,offsets,XmPrimitive,top_shadow_GC,GC)
#define BottomShadowGC(w) XmField(w,offsets,XmPrimitive,bottom_shadow_GC,GC)
#define BackgroundPixel(w) XmField(w,offsets,Core,background_pixel,Pixel)
#define Width(w) XmField(w,offsets,Core,width,Dimension)
#define Height(w) XmField(w,offsets,Core,height,Dimension)

#define SetPixmap(w, px, pixmap, width, height) \
    CurrPx(w) = px; CurrPixmap(w) = pixmap; \
    CurrWidth(w) = width; CurrHeight(w) = height

#define MakePixmap(b,wd,ht) \
    XCreatePixmapFromBitmapData(XtDisplay(w),RootWindowOfScreen(XtScreen(w)), \
    (b), (wd), (ht), Foreground(w), BackgroundPixel(w), \
    DefaultDepthOfScreen(XtScreen(w)))

static void ClassInitialize();
static void Initialize();
static void Redisplay();
static void Resize();
static void Destroy();
static Boolean SetValues();
static XtGeometryResult QueryGeometry();

static void bark_dog();
static void end_bark();
static void start_wag();
static void stop_wag();
static void do_wag();

static char defaultTranslations[] =
   "<Btn1Down>:         Bark()\n\
    ~Shift<Btn2Down>:   StartWag()\n\
    Shift<Btn2Down>:    StopWag()\n\
    <Key>Return:        Bark()\n\
    Ctrl <Key>Return:   Bark()\n\
    <Key>osfActivate:   Bark()\n\
    <Key>space:         Bark()\n\
    Ctrl <Key>space:    Bark()\n\
    <Key>osfSelect:     Bark()\n\
    <Key>W:             StartWag()\n\
    <Key>S:             StopWag()\n\
    <Key>osfHelp:       PrimitiveHelp()";

static XtActionsRec actionsList[] = {
    { "Bark", (XtActionProc) bark_dog},
    { "StartWag", (XtActionProc) start_wag},
    { "StopWag", (XtActionProc) stop_wag}
};

static XmPartResource resources[] = {
    {DogNwagTime, DogCWagTime, XtRInt, sizeof(int),
	XmPartOffset(Dog,wag_time), XmRImmediate, (caddr_t)100},
    {DogNbarkTime, DogCBarkTime, XtRInt, sizeof(int),
	XmPartOffset(Dog,bark_time), XmRImmediate, (caddr_t)1000},
    {DogNbarkCallback, XtCCallback, XtRCallback, sizeof(caddr_t),
	XmPartOffset(Dog,bark_callback), XtRCallback, NULL}
};

DogClassRec dogClassRec = {
    {                                   /* core_class fields    */
    (WidgetClass) &xmPrimitiveClassRec, /* superclass           */
    "Dog",                              /* class_name           */
    sizeof(DogPart),                    /* widget_size          */
    ClassInitialize,                    /* class_initialize     */
    NULL,                               /* class_part_initialize*/
    False,                              /* class_inited         */
    Initialize,                         /* initialize           */
    NULL,                               /* initialize_notify    */
    XtInheritRealize,                   /* realize              */
    actionsList,                        /* actions              */
    XtNumber(actionsList),              /* num_actions          */
    (XtResourceList)resources,          /* resources            */
    XtNumber(resources),                /* num_resources        */
    NULLQUARK,                          /* xrm_class            */
    True,                               /* compress_motion      */
    True,                               /* compress_exposure    */
    True,                               /* compress_enterleave  */
    False,                              /* visible_interest     */
    Destroy,                            /* destroy              */
    Resize,                             /* resize               */
    Redisplay,                          /* expose               */
    SetValues,                          /* set_values           */
    NULL,                               /* set_values_hook      */
    XtInheritSetValuesAlmost,           /* set_values_almost    */
    NULL,                               /* get_values_hook      */
    NULL,                               /* accept_focus         */
    XtVersionDontCheck,                 /* version              */
    NULL,                               /* callback_private     */
    defaultTranslations,                /* tm_table             */
    QueryGeometry,                      /* query_geometry       */
    NULL,                               /* disp accelerator     */
    NULL                                /* extension            */
    },
    {				        /* primitive_class record */
    _XtInherit,                       	/* border_highlight     */
    _XtInherit,                       	/* border_unhighlight   */
    XtInheritTranslations,       	/* translations         */
    bark_dog,                           /* arm_and_activate     */
    NULL,   	    			/* syn resources        */
    0,					/* num syn_resources    */
    NULL,                             	/* extension            */
    },
    {					/* dog_class record     */
    NULL,                             	/* extension            */
    }
};

externaldef(dogwidgetclass) WidgetClass dogWidgetClass =
						(WidgetClass) &dogClassRec;

static XmOffsetPtr offsets; /* Part Offset table for XmResolvePartOffsets */

/**********************************************************************
 *
 * DogCreate - Convenience routine, used by Uil/Mrm.
 *
 *********************************************************************/

Widget DogCreate(parent, name, arglist, nargs)
    Widget parent;
    char *name;
    Arg *arglist;
    int nargs;
{
    return(XtCreateWidget (name, dogWidgetClass, parent, arglist, nargs));
}

/**********************************************************************
 *
 * DogMrmInitialize - register Dog widget class with Mrm
 *
 *********************************************************************/

int DogMrmInitialize()
{
    return(MrmRegisterClass (MrmwcUnknown, "Dog" , "DogCreate",	DogCreate,
				&dogClassRec));
}

/**********************************************************************
 *
 * _DogDrawPixmap - draw the current pixmap
 *
 *********************************************************************/

void _DogDrawPixmap(w)
    DogWidget w;
{
    if (XtIsRealized(w)) {
	XCopyArea(XtDisplay(w),CurrPixmap(w),
		XtWindow(w),DrawGC(w),
		PixmapX(w), PixmapY(w),
		DrawWidth(w), DrawHeight(w),
		DrawX(w), DrawY(w));
    }
}

/**********************************************************************
 *
 * _DogPosition(w) - position the current pixmap
 *
 *********************************************************************/

void _DogPosition(w)
    DogWidget w;
{
    Dimension margin = ShadowThickness(w) + HighlightThickness(w);

    if (CurrWidth(w) < Max(Width(w) - 2 * margin,0)) {
	PixmapX(w) = 0;
	DrawX(w) = Width(w)/2 - CurrWidth(w)/2;
	DrawWidth(w) = CurrWidth(w);
    }
    else {
	PixmapX(w) = (CurrWidth(w) - (Width(w) - 2 * margin))/2;
	DrawX(w) = margin;
	DrawWidth(w) = Width(w) - 2 * margin;
    }

    if (CurrHeight(w) < Max(Height(w) - 2 * margin,0)) {
	PixmapY(w)= 0;
	DrawY(w) = Height(w)/2 - CurrHeight(w)/2;
	DrawHeight(w) = CurrHeight(w);
    }
    else {
	PixmapY(w) = (CurrHeight(w) - (Height(w) - 2 * margin))/2;
	DrawY(w) = margin;
	DrawHeight(w) = Height(w) - 2 * margin;
    }
}

/**********************************************************************
 *
 * Class methods
 *
 *********************************************************************/

static void ClassInitialize()
{
    XmResolvePartOffsets(dogWidgetClass, &offsets);
}

static void Initialize(request, new)
    DogWidget request;
    DogWidget new;
{
    if (Width(request) == 0)
	Width(new) = Max(Max(up_width, down_width),bark_width) +
		2*(ShadowThickness(new)+HighlightThickness(new));
    if (Height(request) == 0)
	Height(new) = Max(Max(up_height, down_height),bark_height) +
		2*(ShadowThickness(new)+HighlightThickness(new));
    create_GC(new);
    create_pixmaps(new);
    SetPixmap(new, DownPx, DownPixmap(new), down_width, down_height);
    Wagging(new) = False;
    Barking(new) = False;
    Resize(new);
}

static create_pixmaps(w)
    DogWidget w;
{
    UpPixmap(w) = MakePixmap(up_bits, up_width, up_height);
    DownPixmap(w) = MakePixmap(down_bits, down_width, down_height);
    BarkPixmap(w) = MakePixmap(bark_bits, bark_width, bark_height);
}

static destroy_pixmaps(w)
    DogWidget w;
{
    XFreePixmap (XtDisplay(w), UpPixmap(w));
    XFreePixmap (XtDisplay(w), DownPixmap(w));
    XFreePixmap (XtDisplay(w), BarkPixmap(w));
}

static create_GC(w)
    DogWidget w;
{
    XGCValues       values;
    XtGCMask        valueMask;

    valueMask = GCForeground | GCBackground | GCGraphicsExposures;
    values.foreground = Foreground(w);
    values.background = BackgroundPixel(w);
    values.graphics_exposures = False;
    DrawGC(w) = XtGetGC(w,valueMask,&values);
}

static void Destroy(w)
    DogWidget w;
{
    XtReleaseGC (w, DrawGC(w));
    destroy_pixmaps(w);
    XtRemoveAllCallbacks (w, DogNbarkCallback);
}

static void Resize(w)
    DogWidget w;
{
    _DogPosition(w);
}

static void Redisplay(w, event, region)
    DogWidget w;
    XEvent *event;
    Region region;
{
    if (XtIsRealized(w)) {
	if (ShadowThickness(w) > 0)
	    _XmDrawShadow (XtDisplay (w), XtWindow (w),
		TopShadowGC(w), BottomShadowGC(w), ShadowThickness(w),
		HighlightThickness(w), HighlightThickness(w),
		Width(w) - 2 * HighlightThickness(w),
		Height(w) - 2 * HighlightThickness(w));

	if (Highlighted(w))
	    _XmHighlightBorder (w);
	else if (_XmDifferentBackground (w, XtParent (w)))
	    _XmUnhighlightBorder (w);

	_DogDrawPixmap(w);
    }
}

static Boolean SetValues(current, request, new)
    DogWidget current;
    DogWidget request;
    DogWidget new;

{
    Boolean redraw = False;

    if (ShadowThickness(new) != ShadowThickness(current) ||
	HighlightThickness(new) != HighlightThickness(current)) {
	_DogPosition(new);
	redraw = True;
    }
    if (Foreground(new) != Foreground(current) ||
        BackgroundPixel(new) != BackgroundPixel(current)) {
	XtReleaseGC (current, DrawGC(current));
        create_GC(new);
	destroy_pixmaps(new);
	create_pixmaps(new);
	switch (CurrPx(new)) {
	    case(UpPx) : 
		SetPixmap(new,UpPx,UpPixmap(new),up_width,up_height);
		break;
	    case(DownPx) : 
		SetPixmap(new,DownPx,DownPixmap(new),down_width,down_height);
		break;
	    case(BarkPx) : 
		SetPixmap(new,BarkPx,BarkPixmap(new),bark_width,bark_height);
		break;
	}
	redraw = True;
    }
    return (redraw);
}

static XtGeometryResult QueryGeometry (w, intended, reply)
    DogWidget w;
    XtWidgetGeometry *intended;
    XtWidgetGeometry *reply;
{
    reply->request_mode = 0;

    if (intended->request_mode & (~(CWWidth | CWHeight)) != 0)
        return (XtGeometryNo);

    reply->request_mode = (CWWidth | CWHeight);
    reply->width = Max(Max(down_width,up_width),bark_width) +
			2*(ShadowThickness(w)+HighlightThickness(w));
    reply->height = Max(Max(down_height,up_height),bark_height) +
			2*(ShadowThickness(w)+HighlightThickness(w));

    if (reply->width != intended->width ||
	reply->height != intended->height ||
	intended->request_mode != reply->request_mode)
	return (XtGeometryAlmost);
    else {
	reply->request_mode = 0;
	return (XtGeometryYes);
    }
}
/**********************************************************************
 *
 * Widget actions
 *
 *********************************************************************/

static void bark_dog(w, event)
    DogWidget w;
    XEvent *event;
{
    XmProcessTraversal(w, XmTRAVERSE_CURRENT);
    XtCallCallbacks (w, DogNbarkCallback, NULL);
    if (Barking(w) == True) return;
    Barking(w) = True;
    SetPixmap(w,BarkPx,BarkPixmap(w),bark_width,bark_height);
    _DogPosition(w);
    _DogDrawPixmap(w);
    XtAppAddTimeOut (XtWidgetToApplicationContext(w), BarkTime(w), end_bark, w);
}

static void end_bark(w, t)
    DogWidget w;
    XtIntervalId *t;
{
    SetPixmap(w,DownPx,DownPixmap(w),down_width,down_height);
    _DogPosition(w);
    _DogDrawPixmap(w);
    Barking(w) = False;
    if (Wagging(w) == True)
	XtAppAddTimeOut(XtWidgetToApplicationContext(w), WagTime(w), do_wag, w);
}

static void start_wag(w, event)
    DogWidget w;
    XEvent *event;
{
    XmProcessTraversal(w, XmTRAVERSE_CURRENT);
    if (Wagging(w) == True) return;
    Wagging(w) = True;
    if (Barking(w) == True) return;
    XtAppAddTimeOut (XtWidgetToApplicationContext(w), WagTime(w), do_wag, w);
}

static void stop_wag(w, event)
    DogWidget w;
    XEvent *event;
{
    XmProcessTraversal(w, XmTRAVERSE_CURRENT);
    Wagging(w) = False;
}

static void do_wag(w, t)
    DogWidget w;
    XtIntervalId *t;
{
    if (Barking(w) == True) return;
    if (Wagging(w) == False) return;
    switch(CurrPx(w)) {
	case(UpPx):
	    SetPixmap(w,DownPx,DownPixmap(w),down_width,down_height);
	    break;
	case(DownPx):
	    SetPixmap(w,UpPx,UpPixmap(w),up_width,up_height);
	    break;
    }
    _DogPosition(w);
    _DogDrawPixmap(w);
    XtAppAddTimeOut (XtWidgetToApplicationContext(w), WagTime(w), do_wag, w);
}

