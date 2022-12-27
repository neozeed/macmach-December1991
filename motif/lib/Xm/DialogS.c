#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)DialogS.c	3.14 91/01/10";
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
#include <Xm/XmP.h>
#include <Xm/DialogSP.h>

#include <Xm/BulletinB.h>
#include <Xm/BulletinBP.h>
#include "BaseClassI.h"

extern void _XtShellGetCoordinates();


#define BIGSIZE ((Dimension)32123)
#define MAGIC_VAL ((Position)~0L)

#ifndef Abs
#define Abs(x) ( (x) >= 0 ? (x) : -(x))
#endif 

#define TotalWidth(w)   (XtWidth  (w) + (2 * (XtBorderWidth (w))))
#define TotalHeight(w)  (XtHeight (w) + (2 *(XtBorderWidth (w))))

#define CALLBACK(w,which,why,evnt)		\
{						\
 if (XmIsBulletinBoard(w))	\
   {						\
 XmAnyCallbackStruct temp;		\
   temp.reason = why;			\
     temp.event  = evnt;			\
       XtCallCallbacks (w, which, &temp);	\
     }						\
   }

/* forward declarations of class procedures */

static void	Initialize();
static void	Realize();
static void	ChangeManaged();
static void	InsertChild();
static Boolean 	SetValues();
static Boolean 	Check_for_window_map();
static void	ClassInitialize();
static void	ClassPartInit();
static XtGeometryResult	GeometryManager();

static XtCallbackProc	DeleteWindowHandler();

/* redefine default for delete_response */

static XtResource extResources[]= {	
    {
	XmNdeleteResponse, XmCDeleteResponse, 
	XmRDeleteResponse, sizeof(unsigned char),
	XtOffset(XmDialogShellExtObject, vendor.delete_response), 
	XmRImmediate, (caddr_t) XmUNMAP,
    },
};


/* ext rec static initialization */
XmDialogShellExtClassRec xmDialogShellExtClassRec = {
    {	
	(WidgetClass) &xmVendorShellExtClassRec, /* superclass	*/   
	"XmDialogShell",		/* class_name 		*/   
	sizeof(XmDialogShellExtRec), 	/* size 		*/   
	NULL,		 		/* Class Initializer 	*/   
	NULL,		 		/* class_part_init 	*/ 
	FALSE, 				/* Class init'ed ? 	*/   
	NULL,	 			/* initialize         	*/   
	NULL, 				/* initialize_notify    */ 
	NULL,	 			/* realize            	*/   
	NULL,	 			/* actions            	*/   
	0,				/* num_actions        	*/   
	extResources, 			/* resources          	*/   
	XtNumber(extResources),		/* resource_count     	*/   
	NULLQUARK, 			/* xrm_class          	*/   
	FALSE, 				/* compress_motion    	*/   
	FALSE, 				/* compress_exposure  	*/   
	FALSE, 				/* compress_enterleave	*/   
	FALSE, 				/* visible_interest   	*/   
	NULL,				/* destroy            	*/   
	XtInheritResize, 		/* resize             	*/   
	NULL, 				/* expose             	*/   
	NULL,		 		/* set_values         	*/   
	NULL, 				/* set_values_hook      */ 
	NULL,			 	/* set_values_almost    */ 
	NULL,				/* get_values_hook      */ 
	NULL, 				/* accept_focus       	*/   
	XtVersion, 			/* intrinsics version 	*/   
	NULL, 				/* callback offsets   	*/   
	NULL,				/* tm_table           	*/   
	NULL, 				/* query_geometry       */ 
	NULL, 				/* display_accelerator  */ 
	NULL, 				/* extension            */ 
    },	
    {
	NULL,				/* synthetic resources	*/
	NULL,				/* num syn resources	*/
	NULL,				/* extension		*/
    },
    {					/* desktop		*/
	NULL,				/* child_class		*/
	XtInheritInsertChild,		/* insert_child		*/
	XtInheritDeleteChild,		/* delete_child		*/
	NULL,				/* extension		*/
    },
    {					/* shell extension	*/
	XmInheritEventHandler,		/* structureNotify	*/
	NULL,				/* extension		*/
    },
    {					/* vendor ext		*/
	(XtCallbackProc)DeleteWindowHandler,/* delete window handler*/
	XmInheritProtocolHandler,	/* offset_handler	*/
	NULL,				/* extension		*/
    }
};

externaldef(xmdialogshellextobjectclass) 
    WidgetClass xmDialogShellExtObjectClass = (WidgetClass) &xmDialogShellExtClassRec;

#ifdef FULL_EXT
XmBaseClassExtRec	myBaseClassExtRec = {
    NULL,				/* Next extension	*/
    NULLQUARK,				/* record type XmQmotif	*/
    XmBaseClassExtVersion,		/* version		*/
    sizeof(XmBaseClassExtRec),		/* size			*/
    XmInheritInitializeSetup,		/* initialize setup	*/
    NULL,				/* initialize prehook	*/
    NULL,				/* initialize posthook	*/
    XmInheritInitializeCleanup,		/* initialize cleanup	*/
    XmInheritSetValuesSetup,		/* setValues setup	*/
    NULL,				/* setValues prehook	*/
    NULL,				/* setValues posthook	*/
    XmInheritSetValuesCleanup,		/* setValues cleanup	*/
    XmInheritGetValuesSetup,		/* getValues setup	*/
    NULL,				/* getValues prehook	*/
    NULL,				/* getValues posthook	*/
    XmInheritGetValuesCleanup,		/* getValues cleanup	*/
    (WidgetClass)&xmDialogShellExtClassRec,/* secondary class	*/
    XmInheritCreate,			/* secondary create	*/
    {0},				/* fast subclass	*/
};
#else
XmBaseClassExtRec	myBaseClassExtRec = {
    NULL,				/* Next extension	*/
    NULLQUARK,				/* record type XmQmotif	*/
    XmBaseClassExtVersion,		/* version		*/
    sizeof(XmBaseClassExtRec),		/* size			*/
    XmInheritInitializePrehook,		/* initialize prehook	*/
    XmInheritSetValuesPrehook,		/* set_values prehook	*/
    XmInheritInitializePosthook,	/* initialize posthook	*/
    XmInheritSetValuesPosthook,		/* set_values posthook	*/
    (WidgetClass)&xmDialogShellExtClassRec,/* secondary class	*/
    XmInheritCreate,			/* secondary create	*/
    NULL,				/* getSecRes data	*/
    {0}				/* fast subclass	*/
};
#endif


XmDialogShellClassRec xmDialogShellClassRec = {
    {					    /* core class record */
	
	(WidgetClass) & transientShellClassRec,	/* superclass */
	XmCDialogShell, 		/* class_name */
	sizeof(XmDialogShellWidgetRec), /* widget_size */
	ClassInitialize,		/* class_initialize proc */
	ClassPartInit,			/* class_part_initialize proc */
	FALSE, 				/* class_inited flag */
	Initialize, 			/* instance initialize proc */
	NULL, 				/* init_hook proc */
	XtInheritRealize,		/* realize widget proc */
	NULL, 				/* action table for class */
	0, 				/* num_actions */
	NULL,	 			/* resource list of class */
	0,		 		/* num_resources in list */
	NULLQUARK, 			/* xrm_class ? */
	FALSE, 				/* don't compress_motion */
	TRUE, 				/* do compress_exposure */
	FALSE, 				/* do compress enter-leave */
	FALSE, 				/* do have visible_interest */
	NULL, 				/* destroy widget proc */
	XtInheritResize, 		/* resize widget proc */
	NULL, 				/* expose proc */
	SetValues, 			/* set_values proc */
	NULL, 				/* set_values_hook proc */
	XtInheritSetValuesAlmost, 	/* set_values_almost proc */
	NULL, 				/* get_values_hook */
	NULL, 				/* accept_focus proc */
	XtVersion, 			/* current version */
	NULL, 				/* callback offset    */
	XtInheritTranslations, 		/* default translation table */
	XtInheritQueryGeometry, 	/* query geometry widget proc */
	NULL, 				/* display accelerator    */
	(XtPointer)&myBaseClassExtRec,	/* extension record      */
    },
    { 					/* composite class record */
	(XtGeometryHandler)GeometryManager,/* geometry_manager */
	ChangeManaged, 			/* change_managed		*/
	InsertChild,			/* insert_child			*/
	XtInheritDeleteChild, 		/* from the shell */
	NULL, 				/* extension record      */
    },
    { 					/* shell class record */
	NULL, 				/* extension record      */
    },
    { 					/* wm shell class record */
	NULL, 				/* extension record      */
    },
    { 					/* vendor shell class record */
	NULL,				/* extension record      */
    },
    { 					/* transient class record */
	NULL, 				/* extension record      */
    },
    { 					/* our class record */
	NULL, 				/* extension record      */
    },
};


/*
 * now make a public symbol that points to this class record
 */

externaldef(xmdialogshellwidgetclass)
    WidgetClass xmDialogShellWidgetClass = (WidgetClass)&xmDialogShellClassRec;
    
static void ClassInitialize()
{
    myBaseClassExtRec.record_type = XmQmotif;
}
    


/************************************************************************
 *
 *  ClassPartInit
 *    Set up the fast subclassing for the widget.
 *
 ************************************************************************/

static void ClassPartInit(wc)
WidgetClass wc;
{
   _XmFastSubclassInit(wc, XmDIALOG_SHELL_BIT);
}

static Widget GetRectObjKid(p)
    CompositeWidget	p;
{
    Cardinal	numRects, i;
    Widget	*currKid;
    
    for (i = 0, currKid = p->composite.children;
	 i < p->composite.num_children;
	 i++, currKid++)
      {
	  if (XtIsRectObj(*currKid))
	    return (*currKid);
      }
    return NULL;
}

static Widget GetManagedKid(p)
    CompositeWidget	p;
{
    Cardinal	numRects, i;
    Widget	*currKid;
    
    for (i = 0, currKid = p->composite.children;
	 i < p->composite.num_children;
	 i++, currKid++)
      {
	  if (XtIsManaged(*currKid))
	    return (*currKid);
      }
    return NULL;
}

/************************************************************************
 *
 *  DeleteWindowHandler
 *
 ************************************************************************/
static XtCallbackProc DeleteWindowHandler(w, closure, call_data)
    VendorShellWidget	w;
    caddr_t		closure, call_data;
{
    XmVendorShellExtObject ve = (XmVendorShellExtObject) closure;

    switch(ve->vendor.delete_response)
      {
	case XmUNMAP:
	  {
	      Widget managedKid;
	      
	      if (managedKid = GetManagedKid(w))
		XtUnmanageChild(managedKid);
	      break;
	  }
	case XmDESTROY:
	  XtDestroyWidget(w);
	  break;
	  
	case XmDO_NOTHING:
	default:
	  break;
      }
}    

/************************************************************************
 *
 *  Initialize
 *
 ************************************************************************/
static void Initialize(request, new)
    Widget request, new;
{
    XmDialogShellWidget	d_shell = (XmDialogShellWidget)new;

    if (XtWidth  (new) <= 0)  XtWidth  (new) = 5;
    if (XtHeight (new) <= 0)  XtHeight (new) = 5;
}




/************************************************************************
 *
 *  SetValues
 *
 ************************************************************************/
static Boolean SetValues(current, request, new)
    Widget current, request, new;
{
    XmDialogShellWidget	d_shell = (XmDialogShellWidget)new;

    return (FALSE);
}

static void InsertChild (w)
    Widget w;
{
    CompositeWidget p = (CompositeWidget) XtParent (w);
   

    /*
     * make sure we only have a rectObj and a VendorObject as children
     */
    if (XtIsRectObj(w))
	{
	    if (GetRectObjKid(p))
	      {
		  XtError("DialogShell widget only supports one rectObj child");
	      }
	    else
	      {
		  /*
		   * make sure we're realized so people won't core dump when doing
		   * incorrect managing prior to realize
		   */
		  XtRealizeWidget(p);
	      }
	}

    (* ((CompositeWidgetClass)compositeWidgetClass)
     ->composite_class.insert_child) 
      (w);
}

#define HALFDIFF(a, b) ((((Position)a) - ((Position)b))/2)

static void GetDefaultPosition(child, parent, xRtn, yRtn)
    XmBulletinBoardWidget	child;
    Widget			parent;
    Position			*xRtn, *yRtn;
{
    Window 	win;
    Display 	*disp;
    int 	max_w, max_h;
    Position 	x, y;

    x = HALFDIFF(XtWidth(parent), XtWidth(child));
    y = HALFDIFF(XtHeight(parent), XtHeight(child));
    
    /* 
     * find root co-ords of the parent's center
     */
    if (XtIsRealized (parent))
      XtTranslateCoords(parent, x, y, &x, &y);
    
    /*
     * try to keep the popup from dribbling off the display
     */
    disp = XtDisplay (child);
    max_w = DisplayWidth  (disp, DefaultScreen (disp));
    max_h = DisplayHeight (disp, DefaultScreen (disp));
    
    if ((x + (int)TotalWidth  (child)) > max_w) 
      x = max_w - TotalWidth  (child);
    if ((y + (int)TotalHeight (child)) > max_h) 
      y = max_h - TotalHeight (child);
    if (x < 0) x = 0;
    if (y < 0) y = 0;

    *xRtn = x;
    *yRtn = y;
}
#undef HALFDIFF







/*
 * border width and size and location are ty...
 *
 * 1. We allow the border width of a XmDialogShell child to change
 *    size arbitrarily.
 *
 * 2. The border width of the shell widget tracks the child's
 *    at all times, exactly.
 *
 * 3. The width of the shell is kept exactly the same as the
 *    width of the child at all times.
 *
 * 4. The child is always positioned at the location
 *    (- child_border, - child_border).
 *
 * the net result is the child has a border width which is always
 * what the user asked for;  but none of it is ever seen, it's all
 * clipped by the shell (parent).  The user sees the border
 * of the shell which is the size he set the child's border to.
 *
 * In the DEC window manager world the window manager does
 * exactly the same thing with the window it puts around the shell.
 * Hence the shell and child have a border width just as the user
 * set but the window manager overrides that and only a single
 * pixel border is displayed.  In a non-wm environment the child 
 * appears to have a border width, in reality this is the shell
 * widget border.  You wanted to know...
 */
static void   ChangeManaged (shell)
    XmDialogShellWidget shell;
{
    /*
     *  If the child went to unmanaged, call XtPopdown.
     *  If the child went to managed, call XtPopup.
     */
    
    int		 		mask = CWStackMode;
    XWindowChanges	 	value;
    XmBulletinBoardWidget	 child;
    int				i;
    XmWidgetExtData		extData = _XmGetWidgetExtData((Widget)shell, XmSHELL_EXTENSION);
    XmVendorShellExtObject	ve = (XmVendorShellExtObject)extData->widget;
    Boolean			childIsBB;

    if (((child = (XmBulletinBoardWidget) GetRectObjKid(shell)) == NULL) ||
	(child->core.being_destroyed))
      return;
    
    childIsBB = XmIsBulletinBoard(child);
    
    if (child->core.managed) 
      {
	  XtWidgetGeometry	request;
	  Position		kidX, kidY;
	  Dimension		kidBW;
	  Boolean		defaultPosition = True;

	  /*
	   * temporary workaround for setkeyboard focus |||
	   */
	  if (((Widget)child != ve->vendor.old_managed)
#ifdef notdef
	      &&(_XmGetFocusPolicy((Widget)child) == XmEXPLICIT)
#endif /* notdef */
	      )
	    {
		XtSetKeyboardFocus((Widget)shell, (Widget)child);
		ve->vendor.old_managed = (Widget)child;
	    }

	  /* 
	   * if the child isn't realized, then we need to realize it
	   * so we have a valid size. It will get created as a result
	   * so we  zero out it's position info so it'll
	   * be okay and then restore it.
	   */
	  if (!XtIsRealized(child))
	    {
		kidX = XtX(child);
		kidY = XtY(child);
		kidBW = XtBorderWidth(child);

		XtX(child) = 0;
		XtY(child) = 0;
		XtBorderWidth(child) = 0;

		XtRealizeWidget(child);

		XtX(child) = kidX;
		XtY(child) = kidY;
		XtBorderWidth(child) = kidBW;
	    }
	  else if (childIsBB)
	    {
		/*  
		 *  Move the window to 0,0
		 *  but don't tell the widget.  It thinks it's where
		 *  the shell is...
		 */
		if ((XtX(child) != 0) || (XtY(child) != 0))
		  XMoveWindow (XtDisplay(child), 
			       XtWindow(child), 
			       0, 0);
	    }

	  /*
	   * TRY TO FIX 1.0 BUG ALERT!
	   *
	   * map callback should occur BEFORE bulletinBoard class default positioning
	   * otherwise, widgets such as fileselection using map callback for
	   * correct sizing have default positioning done before the widget 
	   * grows to its correct dimensions
	   */
	  CALLBACK (child, XmNmapCallback, XmCR_MAP, NULL);	

	  /* 
	   * Make sure that the shell has the same common parameters as 
	   * its child.  Then move the child so that the shell will 
	   * correctly surround it.
	   */
	  request.request_mode = 0;

	  if (childIsBB)
	    {
		defaultPosition =
		  child->bulletin_board.default_position;
		if (defaultPosition && (ve->vendor.externalReposition))
		  defaultPosition = 
		    child->bulletin_board.default_position = 
		      False;
	    }
	  /*
	   * we always keep the bb position at 0.0. we do this before
	   * realizing the child since we dont want the position
	   * information to be flushed to the server for the BBoard.
	   */
	  if (XtX(child) && childIsBB)
	    {
		kidX = XtX(child);
		XtX(child) = 0;
	    }
	  else
	    kidX = XtX(shell);

	  if (XtY(child) && childIsBB)
	    {
		kidY = XtY(child);
		XtY(child) = 0;
	    }
	  else
	    kidY = XtY(shell);

	  if (XtBorderWidth(child) && childIsBB)
	    {
		kidBW = XtBorderWidth(child);
		XtBorderWidth(child) = 0;
	    }
	  else
	    kidBW = XtBorderWidth(shell);
	  
	  if (XtWidth (child) != XtWidth (shell))
	    {
		request.request_mode |= CWWidth;
		request.width = XtWidth(child);
	    }
	  if (XtHeight (child) != XtHeight (shell))
	    {
		request.request_mode |= CWHeight;
		request.height = XtHeight(child);
	    }

	  if (childIsBB)
	    {
		if (defaultPosition)
		  {
		      GetDefaultPosition(child,
					 XtParent(shell),
					 &request.x,
					 &request.y);
		      if (request.x != kidX)
			request.request_mode |= CWX;
		      if (request.y != kidY)
			request.request_mode |= CWY;
		  }
		else
		  {
		      if (kidX != XtX(shell))
			{
			    request.request_mode |= CWX;
			    if (kidX == MAGIC_VAL)
			      request.x = 0;
			    else
			      request.x = kidX;
			}
		      if (kidY != XtY(shell))
			{
			    request.request_mode |= CWY;
			    if (kidY == MAGIC_VAL)
			      request.y = 0;
			    else
			      request.y = kidY;
			}
		  }
	    }
	  else
	    {
		if (kidX != XtX(shell))
		  {
		      request.request_mode |= CWX;
		      request.x = kidX;
		  }
		if (kidY != XtY(shell))
		  {
		      request.request_mode |= CWY;
		      request.y = kidY;
		  }
		if (kidBW != XtBorderWidth(shell))
		  {
		      request.request_mode |= CWBorderWidth;
		      request.border_width = kidBW;
		  }
	    }
	  if (request.request_mode)
	    (void)XtMakeGeometryRequest(shell, &request, &request);

	  /*
	   * Set the mapStyle to manage so that if we are externally
	   * unmapped by the wm we will be able to recover on reciept
	   * of the unmap notify and unmanage ourselves
	   */
	  ve->vendor.mapStyle = _XmMANAGE_MAP;

	  /*
	   * the grab_kind is handled in the popup_callback
	   */
	  XtPopup  (shell, XtGrabNone);
      }
    /*
     * CHILD BEING UNMANAGED
     */
    else
      {
	  Position	x, y;

	  _XtShellGetCoordinates(shell, &x, &y);

#ifdef notdef
	  if (XmIsBulletinBoard(child))
	    {
		XtX(child) = x;
		XtY(child) = y;
	    }
	  /*
	   * update normal_hints even though we shouldn't need to
	   */
	  SetWMOffset(shell);
#endif
	  /*
	   * take it down and then tell user
	   */
	  
	  XtPopdown(shell);
	  
	  CALLBACK (child, XmNunmapCallback, XmCR_UNMAP, NULL);	
      }
}                       


/************************************************************************
 *
 *  ConfigureDialog
 * 	
 * 		Don't move the position of dialog but update the fields
 *
 ************************************************************************/
static void ConfigureDialog(w, geom)
    Widget 		w;
    XtWidgetGeometry	*geom;
{
    XWindowChanges changes, old;
    Cardinal mask = 0;

    if (geom->request_mode & XtCWQueryOnly)
      return;
#ifdef notdef
    if (geom->request_mode & CWX)
	w->core.x = geom->x;

    if (geom->request_mode & CWY)
	w->core.y = geom->y;

    if (geom->request_mode & CWBorderWidth)
      {
	  w->core.border_width = geom->border_width;
      }
#endif
    if ((geom->request_mode & CWWidth) &&
	(w->core.width != geom->width))
      {
	  changes.width = w->core.width = geom->width;
	  mask |= CWWidth;
      }

    if ((geom->request_mode & CWHeight) &&
	(w->core.height != geom->height))
      {
	  changes.height = w->core.height = geom->height;
	  mask |= CWHeight;
      }

    if (mask != 0) 
      {
	  if (XtIsRealized(w)) 
	    {
		if (XtIsWidget(w))
		  XConfigureWindow(XtDisplay(w), XtWindow(w), mask, &changes);
#ifdef DEBUG	    
		else
		  XtError("gadgets aren't allowed in shell");
#endif /* DEBUG */	    
	    }
      }
}

/************************************************************************
 *
 *  GeometryManager
 *
 ************************************************************************/
static XtGeometryResult GeometryManager( wid, request, reply )
    Widget wid;
    XtWidgetGeometry *request;
    XtWidgetGeometry *reply;
{
    ShellWidget 	shell = (ShellWidget)(wid->core.parent);
    XtWidgetGeometry 	my_request;

    if(!(shell->shell.allow_shell_resize) && XtIsRealized(wid) &&
       (request->request_mode & (CWWidth | CWHeight | CWBorderWidth)))
      return(XtGeometryNo);
    /*
     * because of our klutzy API we mimic position requests on the
     * dialog to ourselves
     */
    my_request.request_mode = 0;
    /* %%% worry about XtCWQueryOnly */
    if (request->request_mode & XtCWQueryOnly)
      my_request.request_mode |= XtCWQueryOnly;
    if (request->request_mode & CWX) {
	if (request->x == MAGIC_VAL)
	  my_request.x = 0;
	else
	  my_request.x = request->x;
	my_request.request_mode |= CWX;
    }
    if (request->request_mode & CWY) {
	if (request->y == MAGIC_VAL)
	  my_request.y = 0;
	else
	  my_request.y = request->y;
	my_request.request_mode |= CWY;
    }
    if (request->request_mode & CWWidth) {
	my_request.width = request->width;
	my_request.request_mode |= CWWidth;
    }
    if (request->request_mode & CWHeight) {
	my_request.height = request->height;
	my_request.request_mode |= CWHeight;
    }
    if (request->request_mode & CWBorderWidth) {
	my_request.border_width = request->border_width;
	my_request.request_mode |= CWBorderWidth;
    }
    if (XtMakeGeometryRequest((Widget)shell, &my_request, NULL)
	== XtGeometryYes)
      {
          ConfigureDialog(wid, &my_request);

	  if (XmIsBulletinBoard(wid))
	      return XtGeometryDone;
	  else
	      return XtGeometryYes;
      } 
    else 
      return XtGeometryNo;
}


/*
 *************************************************************************
 *
 * Public creation entry points
 *
 *************************************************************************
 */


/*
 * low level create entry points
 */


#ifdef _NO_PROTO
Widget  XmCreateDialogShell (p, name, al, ac)
Widget  p;				/* parent widget */
char    *name;			/* dialog widget name */
ArgList al;
Cardinal   ac;    
#else /* _NO_PROTO */
Widget XmCreateDialogShell (Widget p, char *name, ArgList al, Cardinal ac)
#endif /* _NO_PROTO */
{
    return (XtCreatePopupShell(name, xmDialogShellWidgetClass, p, al, ac));
}

