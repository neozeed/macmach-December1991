#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)MainW.c	3.10 91/01/10";
#endif /* lint */
#endif /* REV_INFO */
/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
*  (c) Copyright 1987, 1988, 1989, 1990, HEWLETT-PACKARD COMPANY
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
/* #define CDB */

#include <X11/Intrinsic.h>
#include <X11/IntrinsicP.h>
#include <X11/Xutil.h>
#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/SeparatoGP.h>
#include <Xm/SeparatoG.h>
#include <Xm/ScrolledWP.h>
#include <Xm/Command.h>
#include <Xm/RowColumn.h>
#include <Xm/Label.h>
#include <Xm/MainW.h>
#include <Xm/MainWP.h>

static void Resize();
static void InsertChild();
static XtCallbackProc KidKilled();
static Boolean SetValues();
static XtGeometryResult GeometryManager();
static void ChangeManaged();
static void ClassPartInitialize();
static void Initialize();
static void LayMeOut();

static Arg Args[20];

#define MWMessage1 "The Menu Bar cannot be changed to NULL."
#define MWMessage2 "The Command Window cannot be changed to NULL."
#define MWMessage3 "Negative margin value ignored."

static XtActionsRec MainWActions[] =
{
  {"Help",		        (XtActionProc) _XmManagerHelp}  
};


/************************************************************************
 *									*
 * Main Window Resources						*
 *									*
 ************************************************************************/

static XtResource resources[] = 
{
    {
	XmNcommandWindow, XmCCommandWindow, XmRWindow, sizeof(Widget),
        XtOffset (XmMainWindowWidget, mwindow.CommandWindow),
	XmRImmediate, NULL
    },
    {
	XmNcommandWindowLocation, XmCCommandWindowLocation, 
        XmRCommandWindowLocation, sizeof(char),
        XtOffset (XmMainWindowWidget, mwindow.CommandLoc),
	XmRImmediate, XmCOMMAND_ABOVE_WORKSPACE
    },
    {
	XmNmenuBar, XmCMenuBar, XmRWindow, sizeof(Widget),
        XtOffset (XmMainWindowWidget, mwindow.MenuBar),
	XmRImmediate, NULL
    },
    {
	XmNmessageWindow, XmCMessageWindow, XmRWindow, sizeof(Widget),
        XtOffset (XmMainWindowWidget, mwindow.Message),
	XmRImmediate, NULL
    },
    {
        XmNmainWindowMarginWidth, XmCMainWindowMarginWidth,
        XmRHorizontalDimension, sizeof (Dimension),
        XtOffset (XmMainWindowWidget, mwindow.margin_width), 
	XmRImmediate, (caddr_t) 0
    },
    {   
        XmNmainWindowMarginHeight, XmCMainWindowMarginHeight,
        XmRVerticalDimension, sizeof (Dimension),
        XtOffset (XmMainWindowWidget, mwindow.margin_height), 
	XmRImmediate, (caddr_t) 0
    },
    {
	XmNshowSeparator, XmCShowSeparator, XmRBoolean, sizeof(Boolean),
        XtOffset (XmMainWindowWidget, mwindow.ShowSep),
	XmRImmediate, FALSE
    },
    {
        XmNnavigationType, XmCNavigationType, XmRNavigationType, 
        sizeof (unsigned char), XtOffset(XmManagerWidget, manager.navigation_type),
        XmRImmediate, (caddr_t) XmTAB_GROUP,
    }
};

/****************
 *
 * Resolution independent resources
 *
 ****************/

static XmSyntheticResource get_resources[] =
{
   { XmNmainWindowMarginWidth, 
     sizeof (Dimension),
     XtOffset (XmMainWindowWidget, mwindow.margin_width), 
     _XmFromHorizontalPixels,
     _XmToHorizontalPixels },

   { XmNmainWindowMarginHeight, 
     sizeof (Dimension),
     XtOffset (XmMainWindowWidget, mwindow.margin_height),
     _XmFromVerticalPixels,
     _XmToVerticalPixels },

};


/****************************************************************
 *
 * Full class record constant
 *
 ****************************************************************/

externaldef(xmmainwindowclassrec) XmMainWindowClassRec
             xmMainWindowClassRec = {
  {
/* core_class fields      */
    /* superclass         */    (WidgetClass) &xmScrolledWindowClassRec,
    /* class_name         */    "XmMainWindow",
    /* widget_size        */    sizeof(XmMainWindowRec),
    /* class_initialize   */    NULL,
    /* class_partinit     */    ClassPartInitialize,
    /* class_inited       */	FALSE,
    /* initialize         */    Initialize,
    /* Init hook	  */    NULL,
    /* realize            */    _XtInherit,
    /* actions		  */	MainWActions,
    /* num_actions	  */	XtNumber(MainWActions),
    /* resources          */    resources,
    /* num_resources      */    XtNumber(resources),
    /* xrm_class          */    NULLQUARK,
    /* compress_motion	  */	TRUE,
    /* compress_exposure  */	TRUE,
    /* compress_enterleave*/	TRUE,
    /* visible_interest   */    FALSE,
    /* destroy            */    NULL,
    /* resize             */    Resize,
    /* expose             */    _XtInherit,
    /* set_values         */    (XtSetValuesFunc )SetValues,
    /* set values hook    */    NULL,
    /* set values almost  */    XtInheritSetValuesAlmost,
    /* get values hook    */    NULL,
    /* accept_focus       */    NULL,
    /* Version            */    XtVersion,
    /* PRIVATE cb list    */    NULL,
    /* tm_table		  */    XtInheritTranslations,
    /* query_geometry     */    NULL,
    /* display_accelerator*/    NULL,
    /* extension          */    NULL,
  },
  {
/* composite_class fields */
    /* geometry_manager   */    (XtGeometryHandler )GeometryManager,
    /* change_managed     */    (XtWidgetProc) ChangeManaged,
    /* insert_child	  */	InsertChild,
    /* delete_child	  */	XtInheritDeleteChild,	/* Inherit from superclass */
    /* Extension          */    NULL,
  },{
/* Constraint class Init */
    NULL,
    0,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
      
  },
/* Manager Class */
   {		
      XtInheritTranslations,    		/* translations        */    
      get_resources,				/* get resources      	  */
      XtNumber(get_resources),			/* num get_resources 	  */
      NULL,					/* get_cont_resources     */
      0,					/* num_get_cont_resources */
      XmInheritParentProcess,                   /* parent_process         */
      NULL,					/* extension           */    
   },

 {
/* Scrolled Window class - none */     
     /* mumble */               0
 },
 {
/* Main Window class - none */     
     /* mumble */               0
 }	
};

externaldef(xmmainwindowwidgetclass) WidgetClass
             xmMainWindowWidgetClass = (WidgetClass)&xmMainWindowClassRec;




/************************************************************************
 *									*
 *  ClassPartInitialize - Set up the fast subclassing.			*
 *									*
 ************************************************************************/
static void ClassPartInitialize(wc)
WidgetClass wc;

{
   _XmFastSubclassInit (wc, XmMAIN_WINDOW_BIT);
}


/************************************************************************
 *									*
 *  Initialize								*
 *									*
 ************************************************************************/
/* ARGSUSED */
static void Initialize(request, new, args, num_args)
    XmMainWindowWidget request, new;
    ArgList args;
    Cardinal num_args;
{
    int   k;
    
/****************
 *
 *  Bounds check the size.
 *
 ****************/
    if (request->core.width==0 || (request->core.height==0))
    {
        if (request->core.width==0)
	    new->core.width = 50;
	if (request->core.height==0)
	    new->core.height = 50;
    }

    new->swindow.GivenWidth = request->core.width;
    new->swindow.GivenHeight = request->core.height;

    XtAugmentTranslations(new, ((XmManagerClassRec *)XtClass(new))->
    			       manager_class.translations);

    if ((new->mwindow.CommandLoc != XmCOMMAND_ABOVE_WORKSPACE) &&
        (new->mwindow.CommandLoc != XmCOMMAND_BELOW_WORKSPACE))
        new->mwindow.CommandLoc = XmCOMMAND_ABOVE_WORKSPACE;

    new->swindow.InInit = TRUE;

    k = 0;
    XtSetArg (Args[k],XmNorientation,(XtArgVal) XmHORIZONTAL); k++;
    XtSetArg (Args[k],XmNseparatorType,(XtArgVal) XmSHADOW_ETCHED_IN); k++;
    XtSetArg (Args[k],XmNhighlightThickness,(XtArgVal) 0); k++;
    new->mwindow.Sep1 = (XmSeparatorGadget) XtCreateManagedWidget("MainWinSep1", 
				  xmSeparatorGadgetClass,(Widget) new,Args, k);
    
    k = 0;
    XtSetArg (Args[k],XmNorientation,(XtArgVal) XmHORIZONTAL); k++;
    XtSetArg (Args[k],XmNseparatorType,(XtArgVal) XmSHADOW_ETCHED_IN); k++;
    XtSetArg (Args[k],XmNhighlightThickness,(XtArgVal) 0); k++;
    new->mwindow.Sep2 = (XmSeparatorGadget) XtCreateManagedWidget("MainWinSep2", 
				  xmSeparatorGadgetClass,(Widget) new,Args, k);
    k = 0;
    XtSetArg (Args[k],XmNorientation,(XtArgVal) XmHORIZONTAL); k++;
    XtSetArg (Args[k],XmNseparatorType,(XtArgVal) XmSHADOW_ETCHED_IN); k++;
    XtSetArg (Args[k],XmNhighlightThickness,(XtArgVal) 0); k++;
    new->mwindow.Sep3 = (XmSeparatorGadget) XtCreateManagedWidget("MainWinSep3", 
				  xmSeparatorGadgetClass,(Widget) new,Args, k);

    new->mwindow.ManagingSep = FALSE;    
    new->swindow.InInit = FALSE;
    new->swindow.XOffset = new->mwindow.margin_width;    
    new->swindow.YOffset = new->mwindow.margin_height;    
    new->swindow.WidthPad = new->mwindow.margin_width;    
    new->swindow.HeightPad = new->mwindow.margin_height;    
}

/************************************************************************
 *									*
 *  KidKilled								*
 *  Destroy callback for the kid widgets.				*
 *									*
 ************************************************************************/
/* ARGSUSED */
 static XtCallbackProc KidKilled(w,closure,call_data)
    Widget w;
    caddr_t  closure, call_data;
{
    XmMainWindowWidget mw;
    
    mw = (XmMainWindowWidget )(w->core.parent);
    if (w == mw->mwindow.CommandWindow)
        mw->mwindow.CommandWindow = NULL;
    if (w == mw->mwindow.MenuBar)
        mw->mwindow.MenuBar = NULL;
    if (w == mw->mwindow.Message)
        mw->mwindow.Message = NULL;
}


/************************************************************************
 *									*
 *  InsertChild								*
 *									*
 ************************************************************************/
static void InsertChild(w)
    Widget	w;
{
    XmScrolledWindowWidgetClass     superclass;
    XmManagerWidgetClass     superduperclass;    
    XmMainWindowWidget   mom;
    XmRowColumnWidget    rc;
    Boolean 		 punt;

   if (!XtIsRectObj(w))
	return;

/****************
 *
 * If the kid is a command window or a menubar, or if the scrolled window
 * is in Variable layout mode, just insert it.
 *
 ****************/
    mom = (XmMainWindowWidget )w->core.parent;

    superclass = (XmScrolledWindowWidgetClass)xmScrolledWindowWidgetClass;
    superduperclass = (XmManagerWidgetClass)xmManagerWidgetClass;
    
    punt = FALSE;
    if (XmIsRowColumn(w))
    {
        Arg arg[1];
        unsigned char menutype;
        XtSetArg (arg[0], XmNrowColumnType, &menutype );
        XtGetValues (w, arg, 1);

	punt = (menutype == XmMENU_BAR);
	if (punt && !mom->mwindow.MenuBar)         /* If it's a menubar, and */
        {
  	    mom->mwindow.MenuBar = w;		   /* we don't have one yet, use it. */
        }
    }

    if (XmIsCommandBox(w))
    {
	if (!mom->mwindow.CommandWindow)   	   /* If it's a command, and */
	{					   /* we don't have one, get it */
	    punt = TRUE;
	    mom->mwindow.CommandWindow = w;
	}
	
    }
    if ((mom->swindow.InInit)                   ||
        (punt))
    {
        XtAddCallback(w, XmNdestroyCallback,KidKilled,NULL);
	(*superduperclass->composite_class.insert_child)(w);
	return;
    }
/****************
 *
 *  Else let the scrolled window have a crack at it.
 *
 ****************/
    else
	(*superclass->composite_class.insert_child)(w);

}



/************************************************************************
 *									*
 * WidgetVisible - return TRUE if the widget is there and managed.	*
 *									*
 ************************************************************************/
static Boolean WidgetVisible(w)
    Widget w;
{
    return(w && XtIsManaged(w));
}

/************************************************************************
 *									*
 * LayMeOut - Layout the main window.					*
 *                                                                      *
 * This code should be razed and re-written asap.                       *
 *									*
 ************************************************************************/
static void LayMeOut(mw)
    XmMainWindowWidget	mw;
{
    Position mbx,mby, cwx,cwy, swy, mwx, mwy, sepy, sep2y;
    Dimension mbwidth, mbheight, cwwidth, cwheight;
    Dimension MyXpad, MyYpad, mwwidth, mwheight;
    Dimension	bw, sep2h, sep3h;
    XtWidgetGeometry  desired, preferred;

/****************
 *
 * Query the kids - and we have definite preferences as to their sizes.
 * The Menubar gets top billing - we tell it it how wide it is going to be ,
 * and let it have whatever height it wants. The command box gets to stay
 * it's current height, but has to go to the new width. The scrolled window 
 * gets the leftovers.
 *
 ****************/
    MyXpad = mw->mwindow.margin_width;
    MyYpad = mw->mwindow.margin_height;
    mw->swindow.HeightPad = MyYpad;
    
    cwx = MyXpad;

    cwy = swy = MyYpad;
    
    mw->mwindow.ManagingSep = TRUE;    
	
    if (WidgetVisible(mw->mwindow.MenuBar))
    {
	bw = mw->mwindow.MenuBar->core.border_width;
	mbx = MyXpad;
	mby = MyYpad;
	mbwidth = mw->core.width - (2 * (MyXpad + bw));
	if (mbwidth <= 0) mbwidth = 10;
	mbheight = mw->mwindow.MenuBar->core.height;

	desired.x = mbx;	
	desired.y = mby;
	desired.border_width = bw;
        desired.width = mbwidth;
        desired.height = mbheight;
        desired.request_mode = (CWWidth);
        if (XtQueryGeometry(mw->mwindow.MenuBar, &desired, &preferred) != XtGeometryYes)
        {
   	    bw = preferred.border_width;
	    mbheight = preferred.height;
        }
        _XmConfigureObject((RectObj)mw->mwindow.MenuBar, mbx, mby, mbwidth, mbheight,bw);

        if (mw->mwindow.ShowSep)
        {
	    XtManageChild(mw->mwindow.Sep1);
            _XmConfigureObject((RectObj)mw->mwindow.Sep1, 0, mby + mbheight + (2 * bw),
	        	       mw->core.width,  mw->mwindow.Sep1->rectangle.height, 0);
            cwy = swy = mw->mwindow.Sep1->rectangle.height + 
                        mw->mwindow.Sep1->rectangle.y;
        }
        else
        {
            XtUnmanageChild(mw->mwindow.Sep1);
            cwy = swy = mby + mbheight + (2 * bw);
        }
    }
    else
    {
	XtUnmanageChild(mw->mwindow.Sep1);
    }

    if (WidgetVisible(mw->mwindow.CommandWindow))
    {
        bw = mw->mwindow.CommandWindow->core.border_width;
	cwx = MyXpad;
	cwwidth = mw->core.width - (2 * (MyXpad + bw));
	if (cwwidth <= 0) cwwidth = 10;
	cwheight = mw->mwindow.CommandWindow->core.height;

	desired.x = cwx;	
	desired.y = cwy;
	desired.border_width = bw;
        desired.width = cwwidth;
        desired.height = cwheight;
        desired.request_mode = (CWWidth);
        if (XtQueryGeometry(mw->mwindow.CommandWindow, &desired, &preferred) 
            != XtGeometryYes)
        {
   	    bw = preferred.border_width;
	    cwheight = preferred.height;
        }

        if ((cwheight + cwy + (2 * bw)) > (mw->core.height - (2 * MyYpad )))
            cwheight = mw->core.height - (2 * bw) - MyYpad - cwy;
        if (mw->mwindow.ShowSep)
            sep2h = mw->mwindow.Sep2->rectangle.height;
        else
            sep2h = 0;

        sep2y = (cwheight +  cwy) + 2 * bw;
        swy = sep2h + (cwheight +  cwy) + 2 * bw;
        if (mw->mwindow.CommandLoc == XmCOMMAND_BELOW_WORKSPACE)
        {
            mby = swy; 
            sep2y = cwy + (mw->core.height - swy - MyYpad);
            swy = cwy;
            mw->swindow.HeightPad = sep2h + cwheight;
            if (mw->mwindow.ShowSep)
                cwy = sep2y + mw->mwindow.Sep2->rectangle.height;
            else
                cwy = sep2y;
        }
    }    
    else
    {
	XtUnmanageChild(mw->mwindow.Sep2);
        sep2h = 0;
        cwheight = 0;
    }

    if (WidgetVisible(mw->mwindow.Message))
    {
        bw = mw->mwindow.Message->core.border_width;
	mwx = MyXpad;
	mwwidth = mw->core.width - (2 * (MyXpad + bw));
	mwheight = mw->mwindow.Message->core.height;

	desired.x = mwx;	
	desired.y = swy;
	desired.border_width = bw;
        desired.width = mwwidth;
        desired.height = mwheight;
        desired.request_mode = (CWWidth);
        if (XtQueryGeometry(mw->mwindow.Message, &desired, &preferred) 
            != XtGeometryYes)
        {
   	    bw = preferred.border_width;
	    mwheight = preferred.height;
        }
        if (mw->mwindow.ShowSep)
            sep3h = mw->mwindow.Sep3->rectangle.height;
        else
            sep3h = 0;

        sepy = mw->core.height - mwheight - (2 * bw) - MyYpad - sep3h;

        if (mw->mwindow.ShowSep)
            mwy = sepy + sep3h;
        else
            mwy = sepy;

        if (mw->mwindow.CommandLoc == XmCOMMAND_BELOW_WORKSPACE)
        {
            mw->swindow.HeightPad = sep2h + cwheight + sep3h + mwheight;
            sep2y -= (sep3h + mwheight);
            cwy -= (sep3h + mwheight);
        }
        else
            mw->swindow.HeightPad = sep3h + mwheight;

        _XmConfigureObject((RectObj)mw->mwindow.Message, mwx, mwy, mwwidth, mwheight, bw);
        if (mw->mwindow.ShowSep)
        {
	    XtManageChild(mw->mwindow.Sep3);
            _XmConfigureObject((RectObj)mw->mwindow.Sep3, 0, sepy, mw->core.width,  
                               mw->mwindow.Sep3->rectangle.height, 0);
        }
        else
            XtUnmanageChild(mw->mwindow.Sep3);
    }    
    else
    {
	XtUnmanageChild(mw->mwindow.Sep3);
    }
    if (WidgetVisible(mw->mwindow.CommandWindow))
    {
        _XmConfigureObject((RectObj)mw->mwindow.CommandWindow, cwx, cwy, cwwidth, cwheight, bw);
        if (mw->mwindow.ShowSep)
        {
	    XtManageChild(mw->mwindow.Sep2);
            _XmConfigureObject((RectObj)mw->mwindow.Sep2, 0, sep2y, mw->core.width,  
                               mw->mwindow.Sep2->rectangle.height, 0);
        }
        else
            XtUnmanageChild(mw->mwindow.Sep2);
    }

    mw->swindow.XOffset = MyXpad;    
    mw->swindow.YOffset = swy;    
    mw->swindow.WidthPad = MyXpad;
    mw->mwindow.ManagingSep = FALSE;    
}

/************************************************************************
 *                                                                      *
 *  Recompute the size of the main window.				* 
 *									*
 ************************************************************************/
static void Resize(mw)
    XmMainWindowWidget	mw;
{
    XmScrolledWindowWidgetClass superclass;

    superclass = (XmScrolledWindowWidgetClass) xmScrolledWindowWidgetClass;
    LayMeOut(mw);
    (*superclass->core_class.resize)(mw);
}



/************************************************************************
 *									*
 * SetBoxSize - set the size of the Main window to enclose all the	*
 * visible widgets.							*
 *									*
 ************************************************************************/
static void SetBoxSize(mw)
    XmMainWindowWidget mw;
{
    Dimension	    newWidth,newHeight;
    XmScrollBarWidget	    hsb, vsb;
    Widget 	    w;
    Dimension	    hsheight,vmwidth,ht,hsbht,vsbht;
    Dimension	    width, MyXpad, MyYpad;
    XtWidgetGeometry  preferred;

    ht = mw->manager.shadow_thickness  * 2;
    hsbht = vsbht = 0;
    MyXpad = mw->mwindow.margin_width * 2;
    MyYpad = mw->mwindow.margin_height * 2;

    hsb =  mw->swindow.hScrollBar;
    vsb =  mw->swindow.vScrollBar;
    
    if (mw->swindow.ScrollPolicy == XmAPPLICATION_DEFINED)
        w = mw->swindow.WorkWindow;
    else
        w = (Widget )mw->swindow.ClipWindow;
	
    if (WidgetVisible(vsb) &&
        (vsb->core.x < mw->core.width)) 
    {
       	vsbht = 2 * vsb->primitive.highlight_thickness;
	vmwidth = vsb->core.width + mw->swindow.pad +
	          (2 * vsb->primitive.highlight_thickness);
    }
    else
	vmwidth = 0;

    if (WidgetVisible(hsb) &&
        (hsb->core.y < mw->core.height)) 
    {
       	hsbht = 2 * hsb->primitive.highlight_thickness;
	hsheight = hsb->core.height + mw->swindow.pad +
		   (2 * hsb->primitive.highlight_thickness);
    }
    else
	hsheight = 0;
/****************
 *
 * Use the work window as the basis for our height. If the mode is
 * constant, and we are not realized, use the areawidth and areaheight
 * variables instead of the clipwindow width and height, since they are a
 * match for the workspace until the swindow is realized.
 *
 ****************/
    if (WidgetVisible(w)) 
    {
        if ((mw->swindow.ScrollPolicy == XmAUTOMATIC) &&
	    !XtIsRealized(mw))
	{
  	    newWidth = mw->swindow.AreaWidth + (w->core.border_width * 2) + 
		       hsbht + vmwidth + ht + MyXpad;
            newHeight = mw->swindow.AreaHeight + (w->core.border_width * 2) + 
		        vsbht + hsheight + ht + MyYpad;
        }
	else
	{
            XtQueryGeometry(w, NULL, &preferred);
	    newWidth = preferred.width + (w->core.border_width * 2) + 
		       hsbht + vmwidth + ht + MyXpad;
            newHeight = preferred.height  + (w->core.border_width * 2) + 
		        vsbht + hsheight + ht + MyYpad;
	}
    }
    else
    {
	newWidth = mw->core.width + MyXpad;
        newHeight = mw->core.height + MyYpad;
    }
    
    
    if (WidgetVisible(mw->mwindow.CommandWindow))
    {   
        XtQueryGeometry(mw->mwindow.CommandWindow, NULL, &preferred);
        width = preferred.width + 
	        (2 * mw->mwindow.CommandWindow->core.border_width);
    	if (newWidth < width) newWidth = width;
	newHeight += preferred.height + 
  	            (2 * mw->mwindow.CommandWindow->core.border_width);
        if (mw->mwindow.Sep2 && mw->mwindow.ShowSep) 
	    newHeight += mw->mwindow.Sep2->rectangle.height;

    }

    if (WidgetVisible(mw->mwindow.MenuBar))
    {   
        XtQueryGeometry(mw->mwindow.MenuBar, NULL, &preferred);
        width = preferred.width +
	        (2 * mw->mwindow.MenuBar->core.border_width);
    	if (newWidth < width) newWidth = width;
	newHeight += preferred.height +
  	            (2 * mw->mwindow.MenuBar->core.border_width);
        if (mw->mwindow.Sep1  && mw->mwindow.ShowSep) 
	    newHeight += mw->mwindow.Sep1->rectangle.height;
    }

    if (WidgetVisible(mw->mwindow.Message))
    {   
        XtQueryGeometry(mw->mwindow.Message, NULL, &preferred);
        width = preferred.width + 
	        (2 * mw->mwindow.Message->core.border_width);
    	if (newWidth < width) newWidth = width;
	newHeight += preferred.height + 
  	            (2 * mw->mwindow.Message->core.border_width);
        if (mw->mwindow.Sep3 && mw->mwindow.ShowSep) 
	    newHeight += mw->mwindow.Sep3->rectangle.height;

    }
    
/****************
 *
 *
 * If we're not realized, and we have a width and height, use it.
 *
 ******************/
    if (!XtIsRealized(mw))
    {
        if (mw->swindow.GivenWidth)
            newWidth = mw->swindow.GivenWidth;
        if (mw->swindow.GivenHeight)
            newHeight = mw->swindow.GivenHeight;
    }
    if (XtMakeResizeRequest(mw,newWidth,newHeight,NULL,NULL) == XtGeometryYes)
    	    Resize(mw);	    
}

/************************************************************************
 *									*
 *  GeometryManager							*
 *									*
 ************************************************************************/
static XtGeometryResult GeometryManager (w, request, reply)
Widget w;
XtWidgetGeometry * request;
XtWidgetGeometry * reply;

{
    XmMainWindowWidget mw;
    XtGeometryResult res;
    Widget	    mb;
    Dimension	    newWidth,newHeight, OldHeight;
    Dimension	    bw;
    XmScrolledWindowWidgetClass superclass;
    XtWidgetGeometry  desired, preferred;

    superclass = (XmScrolledWindowWidgetClass) xmScrolledWindowWidgetClass;

    mw = (XmMainWindowWidget ) w->core.parent;
    while (!XmIsMainWindow(mw))
    mw = (XmMainWindowWidget )mw->core.parent;

/****************
 *
 * Disallow any X or Y changes.
 * 
 ****************/
    if ((request -> request_mode & CWX || request -> request_mode & CWY))
	if (request->request_mode & CWWidth || 
	    request->request_mode & CWHeight)
	{
	    reply->x = w->core.x;
	    reply->y = w->core.y;
	    if (request->request_mode & CWWidth)
	        reply->width = request->width;
	    if (request->request_mode & CWHeight)
	        reply->height = request->height;
	    reply->request_mode = request->request_mode & ~(CWX | CWY);
	    return(XtGeometryAlmost);
	}
	else
	    return(XtGeometryNo);


    mb = mw->mwindow.MenuBar;


/****************
 *
 * If it's not a mainwindow kid, let the scrolled window deal with it.
 * If it's from the workwindow, and the width changed, resize the menubar
 * and ask for a new height so my layout routine doesn't clip the workwindow.
 *
 ****************/
    if (w != mw->mwindow.MenuBar && 
        w != mw->mwindow.Message &&
        w != mw->mwindow.CommandWindow &&
        w != (Widget )mw->mwindow.Sep1 && 
        w != (Widget) mw->mwindow.Sep2 &&
        w != (Widget) mw->mwindow.Sep3)
    {
        res = (*superclass->composite_class.geometry_manager)(w, request, reply);
        if (res == XtGeometryYes)
        {
            if ((w == mw->swindow.WorkWindow) && 
                (request->request_mode & CWWidth) && 
                WidgetVisible(mb))
            {
                desired.x = mb->core.x;	
	        desired.y = mb->core.y;
	        desired.border_width = mb->core.border_width;
                desired.width = mw->core.width - 
                                (2 * mw->mwindow.margin_width);
                desired.height = mb->core.height;
                desired.request_mode = (CWWidth);
                XtQueryGeometry(mw->mwindow.MenuBar, &desired, &preferred);
                if (preferred.height != mb->core.height)
                {
                    newHeight = mw->core.height - 
	                        (mb->core.height - (2 * mb->core.border_width)) +
	    	                preferred.height + (2 *preferred.border_width);
                    if (XtMakeResizeRequest(mw,mw->core.width,newHeight,NULL,NULL)
                        == XtGeometryYes)
                        _XmResizeObject((RectObj)mw->mwindow.MenuBar, preferred.width, 
                                    preferred.height,preferred.border_width);
                }
            }
          
            Resize(mw);
        }
        return(res);
    }
    
/****************
 *
 * The following code does not handle a BorderWidth only
 * request correctly!
 *
 ****************/
    if(request->request_mode & CWBorderWidth)
	bw = request->border_width;
    else
        bw = w->core.border_width;

    if(request->request_mode & CWWidth) 
	newWidth = request->width + 2 * (bw + mw->mwindow.margin_width);
    else
        newWidth = mw->core.width;

     if (newWidth < mw->core.width) newWidth = mw->core.width;
     
/****************
 *
 * Margins are already included in the old width & height
 *
 ****************/
     if(request->request_mode & CWHeight)
         newHeight = mw->core.height - 
	             (w->core.height - (2 * w->core.border_width)) +
	    	     request->height + 2 * bw;
    else 
         newHeight = mw->core.height;

    OldHeight = mw->core.height;
        

    res = XtMakeResizeRequest(mw,newWidth,newHeight,NULL,NULL);
    if (res == XtGeometryYes)
	{
  	    if(request->request_mode & CWWidth)
	        w->core.width = request->width;
	    if(request->request_mode & CWHeight)
	        w->core.height = request->height;
            mw->swindow.YOffset = mw->swindow.YOffset + (newHeight - OldHeight);
    	    Resize(mw);	    
	}
    return(res);
}



/************************************************************************
 *									*
 *  ChangeManaged - called whenever there is a change in the managed	*
 *		    set.						*
 *									*
 ************************************************************************/
static void ChangeManaged(mw)
    XmMainWindowWidget mw;
{
    XmScrolledWindowWidgetClass superclass;
    CompositeWidget cw = (CompositeWidget) mw->swindow.ClipWindow;
    Widget   w;
    register int i;

    if (mw->mwindow.ManagingSep) return;
    superclass = (XmScrolledWindowWidgetClass) xmScrolledWindowWidgetClass;
/****************
 *
 * This is an ugly bit of work... It's possible for the clip window to get
 * "extra" kids that really want to be mainwindow widgets. So, we fix
 * that... Who says the intrinsics can't reparent widgets? :-)
 *
 ****************/
    if (mw->swindow.ScrollPolicy == XmAUTOMATIC)
    {
        if ((cw->composite.num_children > 1) &&
            (mw->swindow.WorkWindow != NULL))
        {
            for (i = 0; i < cw->composite.num_children; i++)
                if (cw->composite.children[i] != mw->swindow.WorkWindow)
                {
                    w = cw->composite.children[i];
                    if (mw->composite.num_children == mw->composite.num_slots) 
                    {
                        mw->composite.num_slots +=  (mw->composite.num_slots / 2) + 2;
                        mw->composite.children = (WidgetList) XtRealloc(
                                                 (XtPointer) mw->composite.children,
                                                 (unsigned) (mw->composite.num_slots) 
                                                     * sizeof(Widget));
                    }
                    mw->composite.children[mw->composite.num_children++] = w;
                    w->core.parent = (Widget )mw;
                }
            cw->composite.num_children = 1;
            cw->composite.children[0] = mw->swindow.WorkWindow;
        }
                
                
    }
    if (!XtIsRealized(mw)) SetBoxSize(mw);
    (*superclass->composite_class.change_managed)(mw);	
    if (XtIsRealized(mw)) SetBoxSize(mw);
    InitializeScrollBars((Widget)mw);
}
     


/************************************************************************
 *									*
 *  SetValues								*
 *									*
 ************************************************************************/
static Boolean SetValues (current, request, new)
    XmMainWindowWidget current, request, new;
{
    Boolean ret = FALSE;
/****************
 *
 * Visual attributes
 *
 ****************/

    if ((new->mwindow.margin_width != current->mwindow.margin_width) ||
       (new->mwindow.margin_height != current->mwindow.margin_height))
            ret = TRUE;

    if ((new->mwindow.CommandLoc != current->mwindow.CommandLoc) &&
        ((new->mwindow.CommandLoc == XmCOMMAND_ABOVE_WORKSPACE) ||
         (new->mwindow.CommandLoc == XmCOMMAND_BELOW_WORKSPACE)))
            ret = TRUE;
    else
        new->mwindow.CommandLoc = current->mwindow.CommandLoc;

    if ((new->mwindow.MenuBar != current->mwindow.MenuBar) &&
        (new->mwindow.MenuBar == NULL))
    {
        _XmWarning(new, MWMessage1);
	new->mwindow.MenuBar = current->mwindow.MenuBar;
	request->mwindow.MenuBar = current->mwindow.MenuBar;
    }
    if ((new->mwindow.CommandWindow != current->mwindow.CommandWindow) &&
        (new->mwindow.CommandWindow == NULL))
    {
        _XmWarning(new, MWMessage2);    
	new->mwindow.CommandWindow = current->mwindow.CommandWindow;
	request->mwindow.CommandWindow = current->mwindow.CommandWindow;
    }

    if (request->mwindow.MenuBar != current->mwindow.MenuBar) 
    {
        if (request->mwindow.MenuBar == current->mwindow.CommandWindow)
            new->mwindow.CommandWindow = NULL;
        if (request->mwindow.MenuBar == current->swindow.WorkWindow)
            new->swindow.WorkWindow = NULL;
        if (request->mwindow.MenuBar == (Widget) current->swindow.vScrollBar)
            new->swindow.vScrollBar= NULL;
        if (request->mwindow.MenuBar == (Widget) current->swindow.hScrollBar)
            new->swindow.hScrollBar= NULL;
    }

    if (request->mwindow.CommandWindow != current->mwindow.CommandWindow) 
    {
        if (request->mwindow.CommandWindow == current->mwindow.MenuBar)
            new->mwindow.MenuBar = NULL;
        if (request->mwindow.CommandWindow == current->swindow.WorkWindow)
            new->swindow.WorkWindow = NULL;
        if (request->mwindow.CommandWindow == (Widget) current->swindow.vScrollBar)
            new->swindow.vScrollBar= NULL;
        if (request->mwindow.CommandWindow == (Widget) current->swindow.hScrollBar)
            new->swindow.hScrollBar= NULL;
    }

    if (request->mwindow.Message != current->mwindow.Message) 
    {
        if (request->mwindow.Message == current->mwindow.MenuBar)
            new->mwindow.MenuBar = NULL;
        if (request->mwindow.Message == current->swindow.WorkWindow)
            new->swindow.WorkWindow = NULL;
        if (request->mwindow.Message == (Widget) current->swindow.vScrollBar)
            new->swindow.vScrollBar= NULL;
        if (request->mwindow.Message == (Widget) current->swindow.hScrollBar)
            new->swindow.hScrollBar= NULL;
    }

    if (request->swindow.WorkWindow != current->swindow.WorkWindow) 
    {
        if (request->swindow.WorkWindow == current->mwindow.CommandWindow)
            new->mwindow.CommandWindow = NULL;
        if (request->swindow.WorkWindow == current->mwindow.MenuBar)
            new->mwindow.MenuBar = NULL;
        if (request->swindow.WorkWindow == (Widget) current->swindow.vScrollBar)
            new->swindow.vScrollBar= NULL;
        if (request->swindow.WorkWindow == (Widget) current->swindow.hScrollBar)
            new->swindow.hScrollBar= NULL;
    }

    if ((new->mwindow.MenuBar != current->mwindow.MenuBar) ||
        (new->mwindow.Message != current->mwindow.Message) ||
        (new->mwindow.CommandWindow != current->mwindow.CommandWindow ) ||
        (new->mwindow.ShowSep != current->mwindow.ShowSep ) ||
        (ret))
    {
	SetBoxSize(new);
	InitializeScrollBars((Widget)new);
	Resize(new);
    }
           
    return (FALSE);
 }



/************************************************************************
 *									*
 * Spiffy API Functions							*
 *									*
 ************************************************************************/

/************************************************************************
 *									*
 * XmMainWindowSetAreas - set a new widget set.				*
 *									*
 ************************************************************************/
#ifdef _NO_PROTO
void XmMainWindowSetAreas(w, menu, command, hscroll, vscroll, wregion)
    Widget w;
    Widget menu;
    Widget command;
    Widget hscroll;
    Widget vscroll;
    Widget wregion;
#else /* _NO_PROTO */
void XmMainWindowSetAreas (Widget w, Widget menu, Widget command, Widget hscroll, Widget vscroll, Widget wregion)
#endif /* _NO_PROTO */
{
    XmMainWindowWidget mw = (XmMainWindowWidget ) w;    
    mw->mwindow.MenuBar = menu;
    mw->mwindow.CommandWindow = command;
    XmScrolledWindowSetAreas((Widget)mw, hscroll, vscroll, wregion);
    if (XtIsRealized(mw))
    {
        SetBoxSize(mw);
        InitializeScrollBars((Widget)mw);
        LayMeOut(mw);
    }
}


/************************************************************************
 *									*
 * XmMainWindowSep1 - return the id of the top seperator widget.	*
 *									*
 ************************************************************************/
#ifdef _NO_PROTO
Widget XmMainWindowSep1(w)
    Widget w;
#else /* _NO_PROTO */
Widget XmMainWindowSep1 (Widget w)
#endif /* _NO_PROTO */
{
    XmMainWindowWidget   mw = (XmMainWindowWidget) w;
    return ((Widget) mw->mwindow.Sep1);
}

/************************************************************************
 *									*
 * XmMainWindowSep2 - return the id of the bottom seperator widget.	*
 *									*
 ************************************************************************/
#ifdef _NO_PROTO
Widget XmMainWindowSep2(w)
    Widget w;
#else /* _NO_PROTO */
Widget XmMainWindowSep2 (Widget w)
#endif /* _NO_PROTO */
{
    XmMainWindowWidget   mw = (XmMainWindowWidget) w;
    return ((Widget) mw->mwindow.Sep2);
}


/************************************************************************
 *									*
 * XmMainWindowSep3 - return the id of the bottom seperator widget.	*
 *									*
 ************************************************************************/
#ifdef _NO_PROTO
Widget XmMainWindowSep3(w)
    Widget w;
#else /* _NO_PROTO */
Widget XmMainWindowSep3 (Widget w)
#endif /* _NO_PROTO */
{
    XmMainWindowWidget   mw = (XmMainWindowWidget) w;
    return ((Widget) mw->mwindow.Sep3);
}


/************************************************************************
 *									*
 * XmCreateMainWindow - hokey interface to XtCreateWidget.		*
 *									*
 ************************************************************************/
#ifdef _NO_PROTO
Widget XmCreateMainWindow(parent, name, args, argCount)
Widget   parent;
char     *name;
ArgList  args;
Cardinal      argCount;
#else /* _NO_PROTO */
Widget XmCreateMainWindow (Widget parent, char *name, ArgList args, Cardinal argCount)
#endif /* _NO_PROTO */
{

    return ( XtCreateWidget( name, 
			     xmMainWindowWidgetClass, 
			     parent, 
			     args, 
			     argCount ) );
}
