#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)Desktop.c	3.12.1.8 91/04/03";
#endif /* lint */
#endif /* REV_INFO */
/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
*  (c) Copyright 1989, 1990  DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
*  (c) Copyright 1987, 1988, 1989, 1990, HEWLETT-PACKARD COMPANY
*  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
*  (c) Copyright 1988 MICROSOFT CORPORATION
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
/* Make sure all wm properties can make it out of the resource manager */

#include <Xm/XmP.h>
#include <X11/ShellP.h>
#include <Xm/VendorEP.h>
#include "BaseClassI.h"


static XtResource desktopResources[] =
{
#ifdef notdef
/* this should be ok, but we're working around the XtNpersistent bug */
    {
	XmNlogicalParent,
	XmCLogicalParent, XmRWidget, sizeof (Widget),
	XtOffset (XmDesktopObject, ext.logicalParent),
	XmRImmediate, (XtPointer)NULL,
    },
#endif /* notdef */
    {
	XmNdesktopParent,
	XmCDesktopParent, XmRWidget, sizeof (Widget),
	XtOffset(XmDesktopObject, desktop.parent),
	XmRImmediate, (XtPointer)NULL,
    },
    {
	XmNextensionType,
	XmCExtensionType, XmRExtensionType, sizeof (unsigned char),
	XtOffset (XmDesktopObject, ext.extensionType),
	XmRImmediate, (XtPointer)XmDESKTOP_EXTENSION,
    },
};

/********    Static Function Declarations    ********/
#ifdef _NO_PROTO

static void DesktopClassPartInitialize() ;
static void ResParentDestroyed() ;
static void DesktopDestroy() ;
static void DesktopInsertChild() ;
static void DesktopDeleteChild() ;
static void DesktopInitialize() ;
static void DisplayInitialize() ;
static void DisplayDestroy() ;
static void ScreenInitialize() ;

#else

static void DesktopClassPartInitialize( 
                        WidgetClass widgetClass) ;
static void ResParentDestroyed( 
                        Widget resParent,
                        XmExtObject me,
                        XtPointer callData) ;
static void DesktopDestroy( 
                        XmDesktopObject w) ;
static void DesktopInsertChild( 
                        XmDesktopObject w) ;
static void DesktopDeleteChild( 
                        XmDesktopObject w) ;
static void DesktopInitialize( 
                        Widget requested_widget,
                        Widget new_widget,
                        ArgList args,
                        Cardinal *num_args) ;
static void DisplayInitialize( 
                        Widget requested_widget,
                        Widget new_widget,
                        ArgList args,
                        Cardinal *num_args) ;
static void DisplayDestroy( 
                        XmDisplayObject w) ;
static void ScreenInitialize( 
                        Widget requested_widget,
                        Widget new_widget,
                        ArgList args,
                        Cardinal *num_args) ;

#endif /* _NO_PROTO */
/********    End Static Function Declarations    ********/
    
XmDesktopClassRec xmDesktopClassRec = {
    {	
	(WidgetClass) &xmExtClassRec,	/* superclass	*/   
	"Desktop",			/* class_name 		*/   
	sizeof(XmDesktopRec), 		/* size 		*/   
	NULL,				/* Class Initializer 	*/   
	DesktopClassPartInitialize,	/* class_part_init 	*/ 
	FALSE, 				/* Class init'ed ? 	*/   
	DesktopInitialize,		/* initialize         	*/   
	NULL, 				/* initialize_notify    */ 
	NULL,	 			/* realize            	*/   
	NULL,	 			/* actions            	*/   
	0,				/* num_actions        	*/   
	desktopResources,		/* resources          	*/   
	XtNumber(desktopResources),	/* resource_count     	*/   
	NULLQUARK, 			/* xrm_class          	*/   
	FALSE, 				/* compress_motion    	*/   
	FALSE, 				/* compress_exposure  	*/   
	FALSE, 				/* compress_enterleave	*/   
	FALSE, 				/* visible_interest   	*/   
	DesktopDestroy,			/* destroy            	*/   
	NULL,		 		/* resize             	*/   
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
    {					/* ext			*/
	NULL,				/* synthetic resources	*/
	NULL,				/* num syn resources	*/
	NULL,				/* extension		*/
    },
    {					/* desktop		*/
	NULL,				/* child_class		*/
	DesktopInsertChild,		/* insert_child		*/
	DesktopDeleteChild,		/* delete_child		*/
	NULL,				/* extension		*/
    },
};

XmWorldClassRec xmWorldClassRec = {
    {	
	(WidgetClass) &xmDesktopClassRec,/* superclass		*/   
	"World",			/* class_name 		*/   
	sizeof(XmWorldRec), 		/* size 		*/   
	NULL,				/* Class Initializer 	*/   
	NULL,				/* class_part_init 	*/ 
	FALSE, 				/* Class init'ed ? 	*/   
	NULL,				/* initialize         	*/   
	NULL, 				/* initialize_notify    */ 
	NULL,	 			/* realize            	*/   
	NULL,	 			/* actions            	*/   
	0,				/* num_actions        	*/   
	NULL,				/* resources          	*/   
	0,				/* resource_count     	*/   
	NULLQUARK, 			/* xrm_class          	*/   
	FALSE, 				/* compress_motion    	*/   
	FALSE, 				/* compress_exposure  	*/   
	FALSE, 				/* compress_enterleave	*/   
	FALSE, 				/* visible_interest   	*/   
	NULL,				/* destroy            	*/   
	NULL,		 		/* resize             	*/   
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
	NULL, 				/* world_accelerator  */ 
	NULL, 				/* extension            */ 
    },	
    {					/* ext			*/
	NULL,				/* synthetic resources	*/
	NULL,				/* num syn resources	*/
	NULL,				/* extension		*/
    },
    {					/* desktop		*/
	(WidgetClass)&xmDisplayClassRec,/* child_class		*/
	XtInheritInsertChild,		/* insert_child		*/
	XtInheritDeleteChild,		/* delete_child		*/
	NULL,				/* extension		*/
    }
};


XmDisplayClassRec xmDisplayClassRec = {
    {	
	(WidgetClass) &xmDesktopClassRec,/* superclass		*/   
	"Display",			/* class_name 		*/   
	sizeof(XmDisplayRec), 		/* size 		*/   
	NULL,				/* Class Initializer 	*/   
	NULL,				/* class_part_init 	*/ 
	FALSE, 				/* Class init'ed ? 	*/   
	DisplayInitialize,		/* initialize         	*/   
	NULL, 				/* initialize_notify    */ 
	NULL,	 			/* realize            	*/   
	NULL,	 			/* actions            	*/   
	0,				/* num_actions        	*/   
	NULL,				/* resources          	*/   
	0,				/* resource_count     	*/   
	NULLQUARK, 			/* xrm_class          	*/   
	FALSE, 				/* compress_motion    	*/   
	FALSE, 				/* compress_exposure  	*/   
	FALSE, 				/* compress_enterleave	*/   
	FALSE, 				/* visible_interest   	*/   
	DisplayDestroy,			/* destroy            	*/   
	NULL,		 		/* resize             	*/   
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
    {					/* ext			*/
	NULL,				/* synthetic resources	*/
	NULL,				/* num syn resources	*/
	NULL,				/* extension		*/
    },
    {					/* desktop		*/
	(WidgetClass)&xmScreenClassRec,	/* child_class		*/
	XtInheritInsertChild,		/* insert_child		*/
	XtInheritDeleteChild,		/* delete_child		*/
	NULL,				/* extension		*/
    }
};

    
    
#ifdef notdef
    static void		DisplayClassInitialize();
    static void		DisplayClassPartInitialize();
    static void 		DisplayInitialize();
    static Boolean 		DisplaySetValues();
    static void		DisplayDestroy();
    static Widget		DisplayCreateChild();
    static void		DisplayInsertChild();
    static void		DisplayDeleteChild();
#endif
    

XmScreenClassRec xmScreenClassRec = {
    {	
	(WidgetClass) &xmDesktopClassRec,/* superclass		*/   
	"Screen",			/* class_name 		*/   
	sizeof(XmScreenRec), 		/* size 		*/   
	NULL,				/* Class Initializer 	*/   
	NULL,				/* class_part_init 	*/ 
	FALSE, 				/* Class init'ed ? 	*/   
	ScreenInitialize,		/* initialize         	*/   
	NULL, 				/* initialize_notify    */ 
	NULL,	 			/* realize            	*/   
	NULL,	 			/* actions            	*/   
	0,				/* num_actions        	*/   
	NULL,				/* resources          	*/   
	0,				/* resource_count     	*/   
	NULLQUARK, 			/* xrm_class          	*/   
	FALSE, 				/* compress_motion    	*/   
	FALSE, 				/* compress_exposure  	*/   
	FALSE, 				/* compress_enterleave	*/   
	FALSE, 				/* visible_interest   	*/   
	NULL,				/* destroy            	*/   
	NULL,		 		/* resize             	*/   
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
	NULL, 				/* screen_accelerator  */ 
	NULL, 				/* extension            */ 
    },	
    {					/* ext			*/
	NULL,				/* synthetic resources	*/
	NULL,				/* num syn resources	*/
	NULL,				/* extension		*/
    },
    {					/* desktop		*/
	NULL,				/* child_class		*/
	XtInheritInsertChild,		/* insert_child		*/
	XtInheritDeleteChild,		/* delete_child		*/
	NULL,				/* extension		*/
    }
};
    
externaldef(desktopobjectclass) WidgetClass 
      xmDesktopObjectClass = (WidgetClass) &xmDesktopClassRec;
externaldef(worldobjectclass) WidgetClass 
      xmWorldObjectClass = (WidgetClass) (&xmWorldClassRec);
externaldef(displayobjectclass) WidgetClass 
      xmDisplayObjectClass = (WidgetClass) (&xmDisplayClassRec);
externaldef(screenobjectclass) WidgetClass 
      xmScreenObjectClass = (WidgetClass) (&xmScreenClassRec);
    
#ifdef EMACSBADELECTRIC_C
;
#endif
    
static void DesktopClassPartInitialize(widgetClass)
    WidgetClass widgetClass;
{
    register XmDesktopClassPartPtr wcPtr;
    register XmDesktopClassPartPtr superPtr;
    
    wcPtr = (XmDesktopClassPartPtr)
      &(((XmDesktopObjectClass)widgetClass)->desktop_class);
    
    if (widgetClass != xmDesktopObjectClass)
      /* don't compute possible bogus pointer */
      superPtr = (XmDesktopClassPartPtr)&(((XmDesktopObjectClass)widgetClass
					   ->core_class.superclass)->desktop_class);
#ifdef lint
    else
      superPtr = NULL;
#endif
    
    /* We don't need to check for null super since we'll get to xmDesktop
       eventually, and it had better define them!  */
    
#ifdef notdef
    if (wcPtr->create_child == XmInheritCreateChild) {
	wcPtr->create_child =
	  superPtr->create_child;
    }
#endif
    if (wcPtr->child_class == XmInheritClass) {
	wcPtr->child_class = 
	  superPtr->child_class;
    }
    if (wcPtr->insert_child == XtInheritInsertChild) {
	wcPtr->insert_child = superPtr->insert_child;
    }
    
    if (wcPtr->delete_child == XtInheritDeleteChild) {
	wcPtr->delete_child = superPtr->delete_child;
    }
    
}

static void ResParentDestroyed(resParent, me, callData)
    Widget	resParent;
    XmExtObject	me;
    XtPointer	callData;
{
    if (!me->object.being_destroyed)
      XtDestroyWidget(me);
    
}


static void DesktopDestroy(w)
    XmDesktopObject	w;
{
    XmDesktopObject		deskObj = (XmDesktopObject)w;
    Widget			deskParent;
    Widget			resParent = w->ext.logicalParent;
    XmBaseClassExt		*bcePtr;
    XmWidgetExtData		extData;

    /*
     * we should free this in ExtObject's destroy proc, but since all
     * gadgets would need to change to not free it in thier code we'll
     * do it here. |||
     */
    extData = _XmGetWidgetExtData(resParent,w->ext.extensionType);
    if (extData->reqWidget)
      XtFree((char *) extData->reqWidget);

    bcePtr = _XmGetBaseClassExtPtr(XtClass(deskObj), XmQmotif);
   
    if (deskParent = deskObj->desktop.parent)
      {
	  XmDesktopObjectClass	deskParentClass;

	  deskParentClass = (XmDesktopObjectClass)
	    XtClass(deskParent);
	  (*(deskParentClass->desktop_class.delete_child)) (deskObj);
      }

    /*
     * if we were created as a sibling of our primary then we have a
     * destroy callback on them.
     */
    if (resParent && 
	!resParent->core.being_destroyed)
      XtRemoveCallback(resParent, 
		       XmNdestroyCallback,
		       ResParentDestroyed,
		       (XtPointer)w);
    XtFree((char *) w->desktop.children);
}

static void DesktopInsertChild(w)
    XmDesktopObject	w;
{
    register Cardinal	     	position;
    register Cardinal        	i;
    register XmDesktopObject 	cw;
    register WidgetList      	children;
    
    cw = (XmDesktopObject) w->desktop.parent;
    children = cw->desktop.children;
    
    position = cw->desktop.num_children;
    
    if (cw->desktop.num_children == cw->desktop.num_slots) {
	/* Allocate more space */
	cw->desktop.num_slots +=  (cw->desktop.num_slots / 2) + 2;
	cw->desktop.children = children = 
	  (WidgetList) XtRealloc((XtPointer) children,
				 (unsigned) (cw->desktop.num_slots) * sizeof(Widget));
    }
    /* Ripple children up one space from "position" */
    for (i = cw->desktop.num_children; i > position; i--) {
	children[i] = children[i-1];
    }
    children[position] = (Widget)w;
    cw->desktop.num_children++;
}

static void DesktopDeleteChild(w)
    XmDesktopObject	w;
{
    register Cardinal	     	position;
    register Cardinal	     	i;
    register XmDesktopObject 	cw;
    
    cw = (XmDesktopObject) w->desktop.parent;
    
    for (position = 0; position < cw->desktop.num_children; position++) {
        if (cw->desktop.children[position] == (Widget)w) {
	    break;
	}
    }
    if (position == cw->desktop.num_children) return;
    
    /* Ripple children down one space from "position" */
    cw->desktop.num_children--;
    for (i = position; i < cw->desktop.num_children; i++) {
        cw->desktop.children[i] = cw->desktop.children[i+1];
    }
}

/************************************************************************
 *
 *  DesktopInitialize
 *
 ************************************************************************/
/* ARGSUSED */
static void DesktopInitialize(requested_widget, new_widget, args, num_args)
    Widget	new_widget, requested_widget;
    ArgList	args;
    Cardinal	*num_args;
{
    XmDesktopObject		deskObj = (XmDesktopObject)new_widget;
    Widget			deskParent;
    Widget			resParent = deskObj->ext.logicalParent;
    XmBaseClassExt		*bcePtr;

    bcePtr = _XmGetBaseClassExtPtr(XtClass(deskObj), XmQmotif);

    deskObj->desktop.num_children = 0;
    deskObj->desktop.children = NULL;
    deskObj->desktop.num_slots = 0;
    
    if (deskParent = deskObj->desktop.parent)
      {
	  XmDesktopObjectClass	deskParentClass;
	  deskParentClass = (XmDesktopObjectClass)
	    XtClass(deskParent);
	  (*(deskParentClass->desktop_class.insert_child)) (deskObj);
      }

    if (resParent)
      XtAddCallback(resParent, 
		    XmNdestroyCallback, 
		    ResParentDestroyed, 
		    (XtPointer)deskObj);
}

static XContext	actualClassContext = NULL;

/************************************************************************
 *
 *  _XmGetActualClass
 *
 ************************************************************************/
/* ARGSUSED */
#ifdef _NO_PROTO
WidgetClass _XmGetActualClass(class)
    WidgetClass	class;
#else /* _NO_PROTO */
    WidgetClass _XmGetActualClass(WidgetClass class)
#endif /* _NO_PROTO */
{
	  WidgetClass		actualClass;

	  if (actualClassContext == NULL)
	    actualClassContext = XUniqueContext();
	  
	  /*
	   * see if a non-default class has been specified for the
	   * class
	   */
	  if (XFindContext(NULL,
			   (Window) class,
			   actualClassContext,
			   (caddr_t*)&actualClass))
	    {
		return class;
	    }
	  else
	    return actualClass;
}

/************************************************************************
 *
 *  _XmSetActualClass
 *
 ************************************************************************/
/* ARGSUSED */
#ifdef _NO_PROTO
void _XmSetActualClass(class, actualClass)
    WidgetClass	class;
    WidgetClass actualClass;
#else /* _NO_PROTO */
    void _XmSetActualClass(WidgetClass class,  WidgetClass actualClass)
#endif /* _NO_PROTO */
{
    if (actualClassContext == NULL)
      actualClassContext = XUniqueContext();
    
    /*
     * see if a non-default class has been specified for the
     * class
     */
    XSaveContext(NULL,
		 (Window) class,
		 actualClassContext,
		 actualClass);
}

/************************************************************************
 *
 *  _XmGetWorldObject
 *
 ************************************************************************/
/* ARGSUSED */
#ifdef _NO_PROTO
XmWorldObject _XmGetWorldObject(shell, args, num_args)
    Widget	shell;
    ArgList	args;
    Cardinal	*num_args;
#else /* _NO_PROTO */
    XmWorldObject _XmGetWorldObject(Widget shell, ArgList args, Cardinal *num_args)
#endif /* _NO_PROTO */
{
    XmWorldObject	worldObject;
    static XContext	worldObjectContext = NULL;
    
    if (worldObjectContext == NULL)
      worldObjectContext = XUniqueContext();
    
    if (XFindContext(NULL,
		     (Window) NULL,
		     worldObjectContext,
		     (caddr_t*)&worldObject))
      {
	  WidgetClass		worldClass;
	  Widget		appShell = shell;

	  worldClass = _XmGetActualClass(xmWorldObjectClass);
	  
	  while (XtParent(appShell)) 
	    appShell = XtParent(appShell);
	  
	  worldObject = (XmWorldObject)
	    XtCreateWidget("world",
			   worldClass,
			   appShell,
			   args,
			   num_args ? *num_args: NULL);
	  
	  XSaveContext(NULL,
		       (Window) NULL,
		       worldObjectContext,
		       worldObject);
      }
    return 
      worldObject;
}

/************************************************************************
 *
 *  DisplayInitialize
 *
 ************************************************************************/
/* ARGSUSED */
static void DisplayInitialize(requested_widget, new_widget, args, num_args)
    Widget	new_widget, requested_widget;
    ArgList	args;
    Cardinal	*num_args;
{
    XmDisplayObject		displayObj = (XmDisplayObject)new_widget;

    displayObj->display.numModals = 0;
    displayObj->display.modals = NULL;
    displayObj->display.maxModals = 0;
}

static void DisplayDestroy(w)
    XmDisplayObject	w;
{
    XtFree((char *) w->display.modals);
}

/************************************************************************
 *
 *  _XmGetDisplayObject
 *
 ************************************************************************/
/* ARGSUSED */
#ifdef _NO_PROTO
XmDisplayObject _XmGetDisplayObject(shell, args, num_args)
    Widget	shell;
    ArgList	args;
    Cardinal	*num_args;
#else /* _NO_PROTO */
XmDisplayObject _XmGetDisplayObject(Widget shell, ArgList args, Cardinal *num_args)
#endif /* _NO_PROTO */
{
    XmDisplayObject	displayObject;
    static XContext	displayObjectContext = NULL;

    if (displayObjectContext == NULL)
      displayObjectContext = XUniqueContext();
    
    if (XFindContext(XtDisplay(shell),
		     (Window) NULL,
		     displayObjectContext,
		     (caddr_t*)&displayObject))
      {
	  XmWorldObject		worldObject;
	  XmWorldObjectClass 	woc;
	  Arg			myArgs[4];
	  Cardinal		numMyArgs = 0;
	  ArgList		mergedArgs;
	  
	  worldObject = _XmGetWorldObject(shell,
					  NULL,
					  NULL);
	  
	  woc =(XmWorldObjectClass)XtClass(worldObject);

	  XtSetArg(myArgs[numMyArgs] ,XmNdesktopParent, worldObject);
	  numMyArgs++;
	  XtSetArg(myArgs[numMyArgs] ,XmNlogicalParent, worldObject);
	  numMyArgs++;
	  
	  if (num_args)
	    mergedArgs = XtMergeArgLists(args, *num_args, myArgs, numMyArgs);
	  else
	    mergedArgs = myArgs;
 
	  /*
	   * if the display object isn't there then this is the first
	   * shell on this display
	   */
	  displayObject = (XmDisplayObject)
	    XtCreateWidget("display",
			   woc->desktop_class.child_class,
			   shell,
			   mergedArgs,
			   (num_args ? *num_args : 0) + numMyArgs);
	  
	  XSaveContext(XtDisplay(shell),
		       (Window) NULL,
		       displayObjectContext,
		       displayObject);

	  if (myArgs != mergedArgs)
	    XtFree(mergedArgs);
      }
    return 
      displayObject;
}

/************************************************************************
 *
 *  ScreenInitialize
 *
 ************************************************************************/
/* ARGSUSED */
static void ScreenInitialize(requested_widget, new_widget, args, num_args)
    Widget	new_widget, requested_widget;
    ArgList	args;
    Cardinal	*num_args;
{
    XmScreenObject		screenObj = (XmScreenObject)new_widget;

    screenObj->screen.mwmPresent = FALSE;
    screenObj->screen.shellMapped = FALSE;
}

/************************************************************************
 *
 *  _XmGetScreenObject
 *
 ************************************************************************/
/* ARGSUSED */
#ifdef _NO_PROTO
XmScreenObject _XmGetScreenObject(shell, args, num_args)
    Widget	shell;
    ArgList	args;
    Cardinal	*num_args;
#else /* _NO_PROTO */
XmScreenObject _XmGetScreenObject(Widget shell, ArgList args, Cardinal *num_args)
#endif /* _NO_PROTO */
{ 
    XmScreenObject	screenObject;
    static XContext	screenObjectContext = NULL;

    if (screenObjectContext == NULL)
      screenObjectContext = XUniqueContext();

    if (XFindContext(XtDisplay(shell),
		     (Window) XtScreen(shell),
		      screenObjectContext,
		      (caddr_t*)&screenObject))
      {
	  XmDisplayObject	displayObject;
	  XmDesktopObjectClass 	doc;
	  Arg			myArgs[4];
	  Cardinal		numMyArgs = 0;
	  ArgList		mergedArgs;
	  
	  
	  displayObject = _XmGetDisplayObject(shell,
					    NULL,
					    NULL);

	  doc =(XmDesktopObjectClass)XtClass(displayObject);

	  XtSetArg(myArgs[numMyArgs] ,XmNdesktopParent, displayObject);
	  numMyArgs++;
	  XtSetArg(myArgs[numMyArgs] ,XmNlogicalParent, displayObject);
	  numMyArgs++;
	  
	  if (num_args)
	    mergedArgs = XtMergeArgLists(args, *num_args, myArgs, numMyArgs);
	  else
	    mergedArgs = myArgs;
 
	  /*
	   * if the display object isn't there then this is the first
	   * shell on this display
	   */
	  screenObject = (XmScreenObject)
	    XtCreateWidget("screen",
			   doc->desktop_class.child_class,
			   shell,
			   mergedArgs,
			   (num_args ? *num_args : 0) + numMyArgs);
	  
	  XSaveContext(XtDisplay(shell),
		      (Window) XtScreen(shell),
		      screenObjectContext,
		      screenObject);

	  if (myArgs != mergedArgs)
	    XtFree(mergedArgs);
      }
    return 
      screenObject;
}

