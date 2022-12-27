#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)Sash.c	3.11 91/01/10";
#endif /* lint */
#endif /* REV_INFO */
/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
*  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
*  (c) Copyright 1987, 1988, 1989, 1990, HEWLETT-PACKARD COMPANY
*  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
*  ALL RIGHTS RESERVED
*  
*  	THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED
*  AND COPIED ONLY IN ACCORDANCE WITH THE TERMS OF SUCH LICENSE AND
*  WITH THE INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR
*  ANY OTHER COPIES THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE
*  AVAILABLE TO ANY OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF THE
*  SOFTWARE IS HEREBY TRANSFERRED.
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
*  Notice:  Notwithstanding any other lease or license that may pertain to,
*  or accompany the delivery of, this computer software, the rights of the
*  Government regarding its use, reproduction and disclosure are as set
*  forth in Section 52.227-19 of the FARS Computer Software-Restricted
*  Rights clause.
*  
*  (c) Copyright 1989, 1990, 1991 Open Software Foundation, Inc.  Unpublished - all
*  rights reserved under the Copyright laws of the United States.
*  
*  RESTRICTED RIGHTS NOTICE:  Use, duplication, or disclosure by the
*  Government is subject to the restrictions as set forth in subparagraph
*  (c)(1)(ii) of the Rights in Technical Data and Computer Software clause
*  at DFARS 52.227-7013.
*  
*  Open Software Foundation, Inc.
*  11 Cambridge Center
*  Cambridge, MA   02142
*  (617)621-8700
*  
*  RESTRICTED RIGHTS LEGEND:  This computer software is submitted with
*  "restricted rights."  Use, duplication or disclosure is subject to the
*  restrictions as set forth in NASA FAR SUP 18-52.227-79 (April 1985)
*  "Commercial Computer Software- Restricted Rights (April 1985)."  Open
*  Software Foundation, Inc., 11 Cambridge Center, Cambridge, MA  02142.  If
*  the contract contains the Clause at 18-52.227-74 "Rights in Data General"
*  then the "Alternate III" clause applies.
*  
*  (c) Copyright 1989, 1990, 1991 Open Software Foundation, Inc.
*  ALL RIGHTS RESERVED 
*  
*  
* Open Software Foundation is a trademark of The Open Software Foundation, Inc.
* OSF is a trademark of Open Software Foundation, Inc.
* OSF/Motif is a trademark of Open Software Foundation, Inc.
* Motif is a trademark of Open Software Foundation, Inc.
* DEC is a registered trademark of Digital Equipment Corporation
* DIGITAL is a registered trademark of Digital Equipment Corporation
* X Window System is a trademark of the Massachusetts Institute of Technology
*
*******************************************************************************
******************************************************************************/
#include <X11/cursorfont.h>
#include <Xm/SashP.h>

static void Redisplay();
static void ClassPartInitialize();
static void ClassInitialize();
static void Initialize();
static void Realize();


#ifndef MCCABE
static char defTranslations[] =
"<Key>osfHelp:	Help()\n\
 Ctrl ~Shift <Key>osfUp:	SashAction(Key, LargeIncr, Up)\n\
 ~Ctrl ~Shift <Key>osfUp:	SashAction(Key, DefaultIncr, Up)\n\
 Ctrl ~Shift <Key>osfDown:	SashAction(Key, LargeIncr, Down)\n\
 ~Ctrl ~Shift <Key>osfDown:	SashAction(Key, DefaultIncr, Down)\n\
 Shift ~Meta ~Alt <Key>Tab:	PrevTabGroup()\n\
 ~Meta ~Alt <Key>Tab:		NextTabGroup()\n\
 ~Ctrl ~Shift ~Meta ~Alt<Btn1Down>:	SashAction(Start)\n\
 ~Ctrl ~Shift ~Meta ~Alt<Btn1Motion>:	SashAction(Move)\n\
 ~Ctrl ~Shift ~Meta ~Alt<Btn1Up>:	SashAction(Commit)\n\
 ~Ctrl ~Shift ~Meta ~Alt<Btn2Down>:	SashAction(Start)\n\
 ~Ctrl ~Shift ~Meta ~Alt<Btn2Motion>:	SashAction(Move)\n\
 ~Ctrl ~Shift ~Meta ~Alt<Btn2Up>:	SashAction(Commit)\n\
 <FocusIn>:	SashFocusIn()\n\
 <FocusOut>:	SashFocusOut()\n\
 <Unmap>:	PrimitiveUnmap()\n\
 <EnterWindow>:	enter()\n\
 <LeaveWindow>:	leave()";
#else
static char defTranslations[];
#endif


static XtResource resources[] = {
   {XmNborderWidth, XmCBorderWidth, XmRHorizontalDimension, sizeof(Dimension),
      XtOffset(XmSashWidget, core.border_width), XmRImmediate, (caddr_t) 0},

   {XmNcallback, XmCCallback, XmRCallback, sizeof(XtCallbackList), 
      XtOffset(XmSashWidget, sash.sash_action), XmRPointer, NULL},
};

static void SashAction( /* Widget, XEvent*, String*, Cardinal */ );
static void SashFocusIn( /* Widget, XEvent*, String*, Cardinal */ );
static void SashFocusOut( /* Widget, XEvent*, String*, Cardinal */ );
static void PrevTabGroup( /* Widget, XEvent*, String*, Cardinal */ );
static void NextTabGroup( /* Widget, XEvent*, String*, Cardinal */ );

static XtActionsRec actionsList[] =
{
  {"Help",		_XmPrimitiveHelp},
  {"SashAction",	SashAction},
  {"SashFocusIn",	SashFocusIn},
  {"SashFocusOut",	SashFocusOut},
  {"PrevTabGroup",	_XmTraversePrevTabGroup},
  {"NextTabGroup",	_XmTraverseNextTabGroup},
  {"enter", (XtActionProc) _XmPrimitiveEnter},
  {"leave", (XtActionProc) _XmPrimitiveLeave},
};


externaldef(xmsashclassrec) XmSashClassRec xmSashClassRec = {
   {
/* core class fields */
    /* superclass         */   (WidgetClass) &xmPrimitiveClassRec,
    /* class name         */   "XmSash",
    /* size               */   sizeof(XmSashRec),
    /* class initialize   */   ClassInitialize,
    /* class_part_init    */   ClassPartInitialize,
    /* class_inited       */   FALSE,
    /* initialize         */   Initialize,
    /* initialize_hook    */   NULL,
    /* realize            */   Realize,
    /* actions            */   actionsList,
    /* num_actions        */   XtNumber(actionsList),
    /* resourses          */   resources,
    /* resource_count     */   XtNumber(resources),
    /* xrm_class          */   NULLQUARK,
    /* compress_motion    */   TRUE,
    /* compress_exposure  */   XtExposeCompressMaximal,
    /* compress_enter/lv  */   TRUE,
    /* visible_interest   */   FALSE,
    /* destroy            */   NULL,
    /* resize             */   NULL,
    /* expose             */   Redisplay,
    /* set_values         */   NULL,
    /* set_values_hook    */   NULL,
    /* set_values_almost  */   XtInheritSetValuesAlmost,
    /* get_values_hook    */   NULL,
    /* accept_focus       */   NULL,
    /* version            */   XtVersion,
    /* callback_private   */   NULL,
    /* tm_table           */   defTranslations,
    /* query_geometry     */   NULL,
    NULL,                             /* display_accelerator   */
    NULL,                             /* extension             */
   },

   {
      _XtInherit,   /* Primitive border_highlight   */
      _XtInherit,   /* Primitive border_unhighlight */
      NULL,         /* translations                 */
      NULL,         /* arm_and_activate             */
      NULL,	    /* get resources                */
      0,	    /* num get_resources            */
      NULL,         /* extension                    */
   }
};

externaldef(xmsashwidgetclass) WidgetClass xmSashWidgetClass =
					         (WidgetClass) &xmSashClassRec;

#define SASHSIZE 10


/************************************************************************
 *
 *  ClassPartInitialize
 *    Set up the fast subclassing for the widget.
 *
 ************************************************************************/

static void ClassPartInitialize (wc)
WidgetClass wc;
{
   _XmFastSubclassInit(wc, XmSASH_BIT);
}


/************************************************************************
 *
 *  ClassInitialize
 *    Initialize the primitive part of class structure with 
 *    routines to do special highlight & unhighlight for Sash.
 *
 ************************************************************************/

static void HighlightSash(sash)
XmSashWidget sash;
{
  int x, y;
  
  x = y = sash->primitive.shadow_thickness;
  
  XFillRectangle (XtDisplay ((Widget) sash), XtWindow ((Widget) sash),
                   sash->primitive.highlight_GC,
                   x,y, sash->core.width-(2*x), sash->core.height-(2*y));

}


static void UnhighlightSash(sash)
XmSashWidget sash;
{
  int x, y;
  
  x = y = sash->primitive.shadow_thickness;

  XClearArea (XtDisplay ((Widget) sash), XtWindow ((Widget) sash),
                   x,y, sash->core.width-(2*x), sash->core.height-(2*y),
	           FALSE);
}


/* ARGSUSED */
static void SashFocusIn(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    register XmSashWidget sash = (XmSashWidget) w;

    if (_XmGetFocusPolicy((Widget)sash) == XmEXPLICIT)
       HighlightSash(sash);


    _XmDrawShadow (XtDisplay (w), XtWindow (w),
                     sash->primitive.top_shadow_GC,
                     sash->primitive.bottom_shadow_GC,
                     sash->primitive.shadow_thickness,
                     0,0,w->core.width, w->core.height);

    sash->sash.has_focus = True;
}


/* ARGSUSED */
static void SashFocusOut(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    register XmSashWidget sash = (XmSashWidget) w;

    if (_XmGetFocusPolicy((Widget)sash) == XmEXPLICIT)
       UnhighlightSash(sash);

    _XmDrawShadow (XtDisplay (w), XtWindow (w),
                     sash->primitive.top_shadow_GC,
                     sash->primitive.bottom_shadow_GC,
                     sash->primitive.shadow_thickness,
                     0,0,w->core.width, w->core.height);

    sash->sash.has_focus = False;
}


static void ClassInitialize()
{
   xmSashClassRec.primitive_class.border_highlight =
                  (XtWidgetProc) HighlightSash;
   xmSashClassRec.primitive_class.border_unhighlight = 
                  (XtWidgetProc) UnhighlightSash;
}


static void Initialize(request, new)
  XmSashWidget request, new;
{
  if (request->core.width == 0)
     new->core.width += SASHSIZE;
  if (request->core.height == 0)
     new->core.height += SASHSIZE;
  new->sash.has_focus = False;
}



static void SashAction( widget, event, params, num_params )
    Widget widget;
    XEvent *event;
    String *params;
    Cardinal *num_params;
{
    SashCallDataRec call_data;

    call_data.event = event;
    call_data.params = params;
    call_data.num_params = *num_params;

    XtCallCallbacks(widget, XmNcallback, (caddr_t)&call_data );
}


static Cursor SashCursor=NULL;

static void Realize(w, p_valueMask, attributes)
    register Widget w;
    Mask *p_valueMask;
    XSetWindowAttributes *attributes;
{
   Mask valueMask = *p_valueMask;

   if (SashCursor==NULL)
         SashCursor=XCreateFontCursor(XtDisplay(w), XC_crosshair);
    attributes->cursor = SashCursor;
    valueMask |= CWCursor;

    XtCreateWindow (w, InputOutput, CopyFromParent, valueMask, attributes);

    XDefineCursor(XtDisplay(w), XtWindow(w), SashCursor);
}





/*************************************<->*************************************
 *
 *  Redisplay (w, event)
 *
 *   Description:
 *   -----------
 *     Cause the widget, identified by w, to be redisplayed.
 *
 *
 *   Inputs:
 *   ------
 *     w = widget to be redisplayed;
 *     event = event structure identifying need for redisplay on this
 *             widget.
 * 
 *   Outputs:
 *   -------
 *
 *   Procedures Called
 *   -----------------
 *   DrawToggle()
 *   XDrawString()
 *************************************<->***********************************/

/* ARGSUSED */
static void Redisplay(w, event)
    Widget w;
    XEvent *event;
{
   register XmSashWidget sash = (XmSashWidget) w;

     _XmDrawShadow (XtDisplay (w), XtWindow (w), 
                     sash->primitive.top_shadow_GC,
                     sash->primitive.bottom_shadow_GC, 
                     sash->primitive.shadow_thickness,
		     0,0,w->core.width, w->core.height);

     if (sash->sash.has_focus) HighlightSash(w);
}
