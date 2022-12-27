#ifdef  REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)Traversal.c	3.30.1.2 91/03/04";
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
#include <Xm/XmP.h>
#include <X11/Xutil.h>
#include "BaseClassI.h"

#include <Xm/VendorEP.h>

#include <Xm/MenuShell.h>
#include <Xm/MenuShellP.h>
#include <Xm/Label.h>
#include <Xm/LabelG.h>
#include <Xm/SashP.h> /* Yuk,,,,, */

#include "TraversalI.h"

static void MoveNext();
static void MovePrev();
static Boolean Start();
static void Home();
static void TraverseToChild ();

static  void	_XmChangeTabGroups();
static	void	AddTabGroup();
static  void	RemoveTabGroup();
static 	Widget	_XmFindGrabTab();
static  Widget  FindFirstTraversableChild();

typedef enum {
    XmUnrelated,
    XmMyAncestor,
    XmMyDescendant,
    XmMyCousin,
    XmMySelf
}XmGeneology;

typedef struct _XmFocusDataRec {
    Widget	active_tab_group;
    Widget	focus_item;
    Widget	old_focus_item;
    Widget	pointer_item;
    int		num_tab_groups;
    int		size_tab_group_list;
    Widget *	tab_groups;
    Cardinal	num_explicit;
    Boolean	needToFlush;
    XCrossingEvent lastCrossingEvent;
    XmGeneology focalPoint;
} XmFocusDataRec;

#ifdef _NO_PROTO
XmFocusData _XmCreateFocusData()
#else /* _NO_PROTO */
XmFocusData _XmCreateFocusData (void)
#endif /* _NO_PROTO */
{
    XmFocusData	focusData;
      
    focusData = (XmFocusData) XtCalloc(sizeof(XmFocusDataRec), 1);
    
    return focusData;
}

#ifdef _NO_PROTO
void _XmDestroyFocusData(focusData)
    XmFocusData	focusData;
#else /* _NO_PROTO */
void _XmDestroyFocusData (XmFocusData focusData)
#endif /* _NO_PROTO */
{
    if (focusData->tab_groups != NULL) XtFree(focusData->tab_groups);
    XtFree(focusData);
}

#ifdef _NO_PROTO
void _XmSetActiveTabGroup(focusData, tabGroup)
    XmFocusData	focusData;
    Widget	tabGroup;
#else /* _NO_PROTO */
void _XmSetActiveTabGroup (XmFocusData focusData, Widget tabGroup)
#endif /* _NO_PROTO */
{
    focusData->active_tab_group = tabGroup;
}

#ifdef _NO_PROTO
Widget _XmGetActiveItem(w)
    Widget	w;
#else /* _NO_PROTO */
Widget _XmGetActiveItem (Widget w)
#endif /* _NO_PROTO */
{
    XmFocusData		focus_data;

    if ((focus_data = _XmGetFocusData((ShellWidget)_XmFindTopMostShell(w))) == NULL)
      return NULL;
    else
      return focus_data->focus_item;
}



#ifdef _NO_PROTO
void GetWidgetNavigPtrs(widget, np)
    Widget		widget;
    WidgetNavigPtrs	np;
#else /* _NO_PROTO */
void GetWidgetNavigPtrs (Widget widget, WidgetNavigPtrs np)
#endif /* _NO_PROTO */
{
    np->sensitive 		= &(widget->core.sensitive);
    np->ancestor_sensitive	= &(widget->core.ancestor_sensitive);
    np->managed			= &(widget->core.managed);

    if (XmIsManager(widget))
      {
	  XmManagerWidget w = (XmManagerWidget) widget;

	  np->traversal_on 		= &(w->manager.traversal_on);
	  np->mapped_when_managed 	= &(w->core.mapped_when_managed);
	  np->navigation_type	 	= &(w->manager.navigation_type);
	  np->highlighted		= NULL;
	  np->have_traversal		= NULL;
      }
    else if (XmIsPrimitive(widget))
      {
	  XmPrimitiveWidget w = (XmPrimitiveWidget) widget;

	  np->traversal_on 		= &(w->primitive.traversal_on);
	  np->mapped_when_managed 	= &(w->core.mapped_when_managed);
	  np->navigation_type	 	= &(w->primitive.navigation_type);
	  np->highlighted		= &(w->primitive.highlighted);
	  np->have_traversal		= &(w->primitive.have_traversal);
      }
    else if (XmIsGadget(widget))
      {
	  XmGadget w = (XmGadget) widget;

	  np->traversal_on 		= &(w->gadget.traversal_on);
	  np->mapped_when_managed 	= NULL;
	  np->navigation_type	 	= &(w->gadget.navigation_type);
	  np->highlighted		= &(w->gadget.highlighted);
	  np->have_traversal		= &(w->gadget.have_traversal);
      }
    else /* it must be an object or foriegn widget */
      {
	  np->traversal_on 		= NULL;
	  np->mapped_when_managed 	= NULL;
	  np->navigation_type	 	= NULL;
	  np->highlighted		= NULL;
	  np->have_traversal		= NULL;
      }
}

static Boolean InTabSet(w, wNav)
    Widget		w;
    WidgetNavigPtrs	wNav;
{
    WidgetNavigPtrsRec	npRec;
    WidgetNavigPtrs 	np = &npRec;
    XmFocusData		focus_data;
    Widget		tabGroup;

    if (((focus_data = _XmGetFocusData((ShellWidget)_XmFindTopMostShell(w))) == NULL) ||
	!(tabGroup = _XmFindTabGroup(w)))
      return False;
    else
      {
	  GetWidgetNavigPtrs(tabGroup, np);
	  
	  if (NavigFieldEq(np,navigation_type, XmEXCLUSIVE_TAB_GROUP) ||
	      NavigFieldEq(np,navigation_type, XmSTICKY_TAB_GROUP) ||
	      NavigFieldEq(np,navigation_type, XmNONE) ||
	      (focus_data->num_explicit == 0))
	    return True;
	  else
	    return False;
      }
}

/************************************************************************
 *
 *  _XmNavigInitialize
 *
 ************************************************************************/

#ifdef _NO_PROTO
void _XmNavigInitialize (request, new, args, num_args)
    Widget request, new;
    ArgList args;
    Cardinal * num_args;
#else /* _NO_PROTO */
void _XmNavigInitialize (Widget request, Widget new, ArgList args, Cardinal *num_args)
#endif /* _NO_PROTO */
{
    WidgetNavigPtrsRec	npRec;
    WidgetNavigPtrs	np = &npRec;
    XtTranslations	translations = NULL;
    extern void		_XmVirtKeysHandler();
    GetWidgetNavigPtrs(new, np);

    if (XmIsManager(new))
      {
	  XmManagerWidget mw = (XmManagerWidget) new;
	  translations = (XtTranslations)
	    ((XmManagerClassRec *)XtClass(mw))->manager_class.translations;

	  XtInsertEventHandler(new,
			       (EventMask)KeyPressMask | KeyReleaseMask,
			       FALSE,
			       _XmVirtKeysHandler, 
			       (XtPointer)NULL,
			       XtListHead);

      }
    else if (XmIsPrimitive(new))
      {
	  XmPrimitiveWidget pw = (XmPrimitiveWidget) new;
	  translations = (XtTranslations) 
	    ((XmPrimitiveClassRec *)XtClass(new))->primitive_class.translations;

	  XtInsertEventHandler(new,
			       (EventMask)KeyPressMask | KeyReleaseMask,
			       FALSE,
			       _XmVirtKeysHandler, 
			       (XtPointer)NULL,
			       XtListHead);

      }

    NavigFieldSet(np, have_traversal, FALSE);
#ifdef notdef
/* this is now done in the defaults file of the widget code */
    NavigFieldSet(np, traversal_on, TRUE);
#endif
    switch(NavigFieldGet(np, navigation_type))
      {
	case XmEXCLUSIVE_TAB_GROUP:
	case XmSTICKY_TAB_GROUP:
	  AddTabGroup(new, NavigFieldGet(np, navigation_type));
	  break;
	case XmTAB_GROUP:
	case XmNONE:
	case XmDYNAMIC_DEFAULT_TAB_GROUP:
	  break;
	default:
	  XtWarning("invalid navigation type");
	  NavigFieldSet(np, navigation_type, XmNONE);
	  break;	
      }

    /*  If this widget is requesting traversal then augment its  */
    /*  translation table with some additional events.           */
    /* We will only augment translations for a widget which      */
    /* already has some translations defined; this allows widgets*/
    /* which want to set different translations (i.e. menus) to  */
    /* it at a later point in time.                              */
    
    if (NavigFieldEq(np,traversal_on, True) && 
	translations && new->core.tm.translations)
      XtOverrideTranslations((Widget)new, translations);
}


#ifdef _NO_PROTO
Boolean _XmChangeNavigationType(current, newNavType)
    Widget		current;
    XmNavigationType	newNavType;
#else /* _NO_PROTO */
Boolean _XmChangeNavigationType (Widget current, XmNavigationType newNavType)
#endif /* _NO_PROTO */
{
    WidgetNavigPtrsRec	currNavRec;
    WidgetNavigPtrs	currNav = &currNavRec;

    GetWidgetNavigPtrs(current, currNav);

    switch(newNavType)
      {
	case XmEXCLUSIVE_TAB_GROUP:
	case XmSTICKY_TAB_GROUP:
	case XmTAB_GROUP:
	case XmNONE:
	case XmDYNAMIC_DEFAULT_TAB_GROUP:
	  break;
	default:
	  XtWarning("invalid value for navigation_type");
	  return False;
	  break;	
      }

    if (NavigFieldEq(currNav, navigation_type, newNavType))
	return True;
    
    switch(NavigFieldGet(currNav, navigation_type))
	{
	case XmEXCLUSIVE_TAB_GROUP:
	case XmSTICKY_TAB_GROUP:
	    RemoveTabGroup(current->core.self, 
			   NavigFieldGet(currNav,navigation_type),
			   True);
	    break;
	}

    switch(newNavType)
	{
	case XmEXCLUSIVE_TAB_GROUP:
	case XmSTICKY_TAB_GROUP:
	    AddTabGroup(current->core.self, newNavType);
	    break; 
	}
    return True ;
}

/************************************************************************
 *
 *  _XmNavigSetValues
 *
 ************************************************************************/
  
#ifdef _NO_PROTO
Boolean _XmNavigSetValues (current, request, new, args, num_args)
    Widget current, request, new;
    ArgList args;
    Cardinal * num_args;
#else /* _NO_PROTO */
Boolean _XmNavigSetValues (Widget current, Widget request, Widget new, ArgList args, Cardinal *num_args)
#endif /* _NO_PROTO */
{
    Boolean	 	returnFlag = False;
    Boolean	 	visualFlag = False;
    Boolean 		tempTrav, tempSensitive;
    Boolean		tempAnSensitive, tempMapped, tempNavig;
    Boolean		*highlighted, *haveTrav;

    Boolean 		traversalIsHere = False;
    XRectangle 		visRect;
    XmManagerWidget 	parent;
    Boolean		falseVal = False, trueVal = True;

    WidgetNavigPtrsRec	currNavRec, newNavRec;
    WidgetNavigPtrs	currNav = &currNavRec, newNav = &newNavRec;


    GetWidgetNavigPtrs(current, currNav);
    GetWidgetNavigPtrs(new, newNav);

    if (!NavigFieldsEq(currNav, newNav, navigation_type))
      {
	  if (!_XmChangeNavigationType(current,
				       NavigFieldGet(newNav, 
						     navigation_type)))
	    NavigFieldSet(newNav,
			  navigation_type,
			  NavigFieldGet(currNav,
					navigation_type));
	  else
	    returnFlag |= True;
      }

    /*  Process the change in values */
    
    if (!NavigFieldsEq(currNav, newNav, mapped_when_managed) ||
	!NavigFieldsEq(currNav, newNav, sensitive) ||
	!NavigFieldsEq(currNav, newNav, ancestor_sensitive) ||
	!NavigFieldsEq(currNav, newNav, traversal_on) ||
	!NavigFieldsEq(currNav, newNav, navigation_type))
      {
	  returnFlag = True;
	  
	  if (_XmFocusIsHere(new) &&
	      (!_XmCreateVisibilityRect(new, &visRect) ||
	       !_XmTestTraversability(new, &visRect) ||
	       !InTabSet(new, newNav)))
	    {
		XmProcessTraversal ((Widget)new, XmTRAVERSE_RIGHT);
	  
		/*  We need to manually clear the 'have_traversal' flag,      */
		/*  since _XmPrimitiveFocusOut() will not do it because our  */
		/*  traversal flag will not be set by that time.              */
		
		NavigFieldSet(newNav, have_traversal, FALSE);
		NavigFieldSet(newNav, highlighted, FALSE);
		
		returnFlag = True;
	    }
	  else 
	    {
		/* Traversal code will catch this case */
	    }
      }

    if (XmIsManager(new))
      {
	  /* Traversal is always on for a manager widget */
#ifdef notdef
	  NavigFieldSet(newNav, traversal_on, TRUE);
	  NavigFieldSet(currNav, traversal_on, TRUE);
#endif
      }
    else if (XmIsPrimitive(new))
      {
	  XmPrimitiveWidget curpw = (XmPrimitiveWidget) current;
	  XmPrimitiveWidget newpw = (XmPrimitiveWidget) new;

	  /*  If traversal has been turned on, then augment the translations  */
	  /*  of the new widget.                                              */
	  
	  if (!NavigFieldsEq(newNav, currNav, traversal_on) &&
	      NavigFieldEq(newNav, traversal_on, True) && 
	      (newpw->core.tm.translations))
	    {
		if ((((XmPrimitiveClassRec *)
		      XtClass(newpw))->primitive_class.translations) != NULL)
		  XtOverrideTranslations
		    ((Widget) newpw,
		     ((XmPrimitiveClassRec *)XtClass(newpw))->primitive_class.translations);
	    }
      }
    return returnFlag;
}

/************************************************************************
 *
 *  _XmNavigDestroy
 *
 ************************************************************************/

#ifdef _NO_PROTO
void _XmNavigDestroy (w)
    Widget	w;
    
#else /* _NO_PROTO */
    void _XmNavigDestroy (Widget w)
#endif /* _NO_PROTO */
{
    WidgetNavigPtrsRec	navRec;
    WidgetNavigPtrs	np = &navRec;
    XmFocusData		focusData;
    Widget		shell = _XmFindTopMostShell(w);
    if (focusData = _XmGetFocusData((ShellWidget)shell))
      {
	  GetWidgetNavigPtrs(w, np);
	  
	  if (_XmFocusIsHere(w))
	    {
		/*
		 * should probably go to the next widget whatever that means
		 * but that's more complicated... so punt
		 */
		(void)XmProcessTraversal(w, XmTRAVERSE_RIGHT);
	    }
	  /*
	   * There is a bug in the Xt FocusDestroyCallback that will
	   * clear the focus after we have set it in the
	   * _XmNavigChangeManaged. This work around will make sure
	   * it's correctly set ||| 
	   */
	  else if (focusData->old_focus_item == w)
	    {
		XtSetKeyboardFocus(shell, focusData->focus_item);
	    }
	  /* If we're a tab group, then remove the tab group */
	  if (NavigIsTabGroup(np))
	    RemoveTabGroup(w, NavigFieldGet(np,navigation_type), True);
      }
}

/************************************************************************
 *
 *  _XmTrackShellFocus
 *
 *  This handler is added by ShellExt initialize to the front of the
 * queue
 *     
 ************************************************************************/
/* ARGSUSED */
void _XmTrackShellFocus(widget, client_data, event, dontSwallow)
    Widget widget;
    XtPointer client_data;
    XEvent *event;
    Boolean *dontSwallow;
{
    XmVendorShellExtObject	ve =
      (XmVendorShellExtObject)client_data;
    XmFocusData			focusData;
    XmGeneology			oldFocalPoint;
    XmGeneology			newFocalPoint;

    if (widget->core.being_destroyed)
      {
	  *dontSwallow = False;
	  return;
      }
    focusData = ve->vendor.focus_data;
    oldFocalPoint = newFocalPoint = focusData->focalPoint;

    switch( event->type ) {
      case EnterNotify:
      case LeaveNotify:
	
	/*
	 * If operating in a focus driven model, then enter and
	 * leave events do not affect the keyboard focus.
	 */
	if ((event->xcrossing.detail != NotifyInferior)
	    &&	(event->xcrossing.focus))
	  {	      
	      switch (oldFocalPoint)
		{
		  case XmMyAncestor:
		    if (event->type == LeaveNotify)
		      newFocalPoint = XmUnrelated;
		    break;
		  case XmUnrelated:
		    if (event->type == EnterNotify)
		      newFocalPoint = XmMyAncestor;
  	            break;
		  case XmMySelf:
		    break;
		  case XmMyDescendant:
		    break;
		    
		}	
	  }
	break;
      case FocusIn:
	switch (event->xfocus.detail)
	  {
	    case NotifyNonlinear:
	    case NotifyAncestor:
	    case NotifyInferior:
	      newFocalPoint = XmMySelf;
	      break;
	    case NotifyNonlinearVirtual:
	    case NotifyVirtual:
	      newFocalPoint = XmMyDescendant;
	      break;
	    case NotifyPointer:
	      newFocalPoint = XmMyAncestor;
	      break;
	  }
	break;
      case FocusOut:
	switch (event->xfocus.detail)
	  {
	    case NotifyPointer:
	    case NotifyNonlinear:
	    case NotifyAncestor:
	    case NotifyNonlinearVirtual:
	    case NotifyVirtual:
	      newFocalPoint = XmUnrelated;
	      break;
	    case NotifyInferior:
	      newFocalPoint = XmMyDescendant;
	      return;
	      break;
	  }
	break;
    }

    
    if ((_XmGetFocusPolicy(widget) == XmEXPLICIT) && 
	(oldFocalPoint != newFocalPoint) && 
	focusData->focus_item)
      {
	  if (oldFocalPoint == XmUnrelated)
	    {
		_XmCallFocusMoved(NULL, 
				  focusData->focus_item, 
				  event);
	    }
	  else if (newFocalPoint == XmUnrelated)
	    {
		_XmCallFocusMoved(focusData->focus_item, 
				  NULL,
				  event);
	    }
      }
    focusData->focalPoint = newFocalPoint;
}


#ifdef _NO_PROTO
Boolean	_XmCallFocusMoved(old, new, event)
    XEvent	*event;
    Widget	old, new;
#else /* _NO_PROTO */
Boolean _XmCallFocusMoved (Widget old, Widget new, XEvent *event)
#endif /* _NO_PROTO */
{
    Widget			w;
    Widget			topShell;
    
    if (old) 
      w = old;
    else if (new)
      w = new;
      
    topShell 	= (Widget) _XmFindTopMostShell(w);

    /*
     * make sure it's a shell that has a vendorExt object
     */
    if (XmIsVendorShell(topShell))
      {
	  XmWidgetExtData		extData;
	  XmVendorShellExtObject	vendorExt;
	  XtCallbackList		callbacks;

	  extData	= _XmGetWidgetExtData(topShell, XmSHELL_EXTENSION);

	  if (vendorExt = (XmVendorShellExtObject) extData->widget)
	    {
		if (callbacks = vendorExt->vendor.focus_moved_callback)
		  {
		      XmFocusMovedCallbackStruct	callData;
		      
		      callData.event		= event;
		      callData.cont		= True;
		      callData.old		= old;
		      callData.new		= new;
		      callData.focus_policy	= vendorExt->vendor.focus_policy;
		      
		      XtCallCallbackList(vendorExt, callbacks, &callData);
		      return callData.cont;
		  }
	    }
      }
    return True;
}

#define EVENTS_EQ(ev1, ev2) \
  ((((ev1)->type == (ev2)->type) &&\
    ((ev1)->serial == (ev2)->serial) &&\
    ((ev1)->time == (ev2)->time) &&\
    ((ev1)->x == (ev2)->x) &&\
    ((ev1)->y == (ev2)->y)) ? TRUE : FALSE)

static Boolean UpdatePointerData(w, event)
    Widget		w;
    XCrossingEvent	*event;
{
    XmFocusData		focusData;

    if (focusData = _XmGetFocusData((ShellWidget)_XmFindTopMostShell(w)))
      {
	  XCrossingEvent *lastEvent = &(focusData->lastCrossingEvent);

	  focusData->needToFlush = TRUE;

	  if (!EVENTS_EQ(lastEvent,event))
	    {
		focusData->pointer_item = w;
		focusData->lastCrossingEvent = *event;
		return TRUE;
	    }
      }
    return FALSE;
}

static void FlushPointerData(w, event)
    Widget		w;
    XCrossingEvent	*event;
{
    XmFocusData		focusData = _XmGetFocusData((ShellWidget)_XmFindTopMostShell(w));

    if (focusData && focusData->needToFlush)
      {
	  XCrossingEvent	lastEvent;

	  lastEvent = focusData->lastCrossingEvent;

	  focusData->needToFlush = FALSE;
	  /* 
	   * We are munging data into the event to fake out the focus
	   * code when Mwm is trying to catch up with the pointer.
	   * This event that we are munging might already have been
	   * munged by XmDispatchGadgetInput from a motion event to a
	   * crossing event !!!!!
	   */
	  
	  lastEvent.serial = event->serial;
	  lastEvent.time = event->time;
	  lastEvent.focus = True;
	  XtDispatchEvent(&lastEvent);
      }
}

/************************************************************************
 *
 *  Enter & Leave
 *      Enter and leave event processing routines.  Handle border
 *      highlighting and dehighlighting by invoking the class functions.
 *
 ************************************************************************/

#ifdef _NO_PROTO
void _XmPrimitiveEnter (pw, event)
    XmPrimitiveWidget pw;
    XEvent * event;
    
#else /* _NO_PROTO */
void _XmPrimitiveEnter (XmPrimitiveWidget pw, XEvent *event)
#endif /* _NO_PROTO */
{
    if (_XmGetFocusPolicy((Widget)pw) == XmPOINTER)
      {
	  if ((event->xcrossing.focus) &&
	      (_XmCallFocusMoved(XtParent(pw), (Widget)pw, event)))
	    {
		if (pw->primitive.highlight_on_enter &&
		    (((XmPrimitiveWidgetClass)(pw->core.widget_class))->
		     primitive_class.border_highlight != NULL))
		  (*(((XmPrimitiveWidgetClass)(pw->core.widget_class))->
		     primitive_class.border_highlight))(pw);
	    }
	  else
	    (void) UpdatePointerData(pw, event);
      }
}

#ifdef _NO_PROTO
void _XmPrimitiveLeave (pw, event)
    XmPrimitiveWidget pw;
    XEvent * event;
    
#else /* _NO_PROTO */
void _XmPrimitiveLeave (XmPrimitiveWidget pw, XEvent *event)
#endif /* _NO_PROTO */
{
    if (_XmGetFocusPolicy((Widget)pw) == XmPOINTER)
      {
	  Widget		parent = XtParent(pw);

	  if (event->xcrossing.focus && _XmCallFocusMoved((Widget)pw, parent, event))
	    {
		if (pw->primitive.highlight_on_enter &&
		    (((XmPrimitiveWidgetClass)(pw->core.widget_class))->
		     primitive_class.border_unhighlight != NULL))
		  (*(((XmPrimitiveWidgetClass)(pw->core.widget_class))->
		     primitive_class.border_unhighlight))(pw);
	    }
	  else
	    UpdatePointerData(pw, event);
      }	
}




/************************************************************************
 *
 *  Unmap
 *      Track whether a widget is visible.
 *
 ***********************************************************************/

#ifdef _NO_PROTO
void _XmPrimitiveUnmap (pw, event)
XmPrimitiveWidget pw;
XEvent * event;

#else /* _NO_PROTO */
void _XmPrimitiveUnmap (XmPrimitiveWidget pw, XEvent *event)
#endif /* _NO_PROTO */
{
   if (pw->primitive.have_traversal)
      _XmProcessTraversal ((Widget)pw, XmTRAVERSE_RIGHT, False);
}




/************************************************************************
 *
 *  Focus In & Out
 *      Handle border highlighting and dehighlighting.
 *
 ************************************************************************/

#ifdef _NO_PROTO
void _XmPrimitiveFocusInInternal (pw, event)
    XmPrimitiveWidget pw;
    XEvent * event;
#else /* _NO_PROTO */
void _XmPrimitiveFocusInInternal (XmPrimitiveWidget pw, XEvent *event)
#endif /* _NO_PROTO */
{
    XRectangle 		visRect;

    if (!event->xfocus.send_event)
      return;

    if (_XmGetFocusPolicy((Widget)pw) == XmPOINTER)
      {
	  /*
	   * Maybe Mwm trying to catch up  with us
	   */
	  if (XtIsShell(XtParent(pw)))
	    {
		FlushPointerData(pw, event);
	    }
      }
    else 
      {
	  /*
	   * we should only be recieving the focus from a traversal request.
	   * Otherwise bug out
	   */
	  if (!_XmGetTabGroup((Widget)pw))
	    {
		(void) XmProcessTraversal ((Widget)pw, XmTRAVERSE_NEXT_TAB_GROUP);
	    }
	  else /* if(_XmCallFocusMoved(NULL, pw, event)) */
	    {
		(*(((XmPrimitiveWidgetClass)(pw->core.widget_class))->
		   primitive_class.border_highlight))(pw);
		pw->primitive.have_traversal = True;
	    }
      }
}

#ifdef _NO_PROTO
void _XmPrimitiveFocusOut (pw, event)
XmPrimitiveWidget pw;
XEvent * event;

#else /* _NO_PROTO */
void _XmPrimitiveFocusOut (XmPrimitiveWidget pw, XEvent *event)
#endif /* _NO_PROTO */
{
   if (!event->xfocus.send_event || pw->core.being_destroyed)
      return;

    if (_XmGetFocusPolicy((Widget)pw) == XmEXPLICIT)
      {
	 /*  if(_XmCallFocusMoved(pw, NULL, event)) */
	    {
		if (pw->primitive.traversal_on)
		  {
		      (*(((XmPrimitiveWidgetClass)(pw->core.widget_class))->
			 primitive_class.border_unhighlight))(pw);
		      pw->primitive.have_traversal = False;
		  }
	    }								
      }
}

#ifdef _NO_PROTO
void _XmPrimitiveFocusIn (pw, event)
XmPrimitiveWidget pw;
XEvent * event;

#else /* _NO_PROTO */
void _XmPrimitiveFocusIn (XmPrimitiveWidget pw, XEvent *event)
#endif /* _NO_PROTO */
{
   if (!_XmGetTransientFlag((Widget)pw))
      _XmPrimitiveFocusInInternal(pw, event);
}



/************************************************************************
 *
 *  _XmEnterGadget
 *     This function processes enter window conditions occuring in a gadget
 *
 ************************************************************************/

#ifdef _NO_PROTO
void _XmEnterGadget (gadget, event)
XmGadget gadget;
XEvent * event;

#else /* _NO_PROTO */
void _XmEnterGadget (XmGadget gadget, XEvent *event)
#endif /* _NO_PROTO */
{
    if (_XmGetFocusPolicy((Widget)gadget) == XmPOINTER)
      {
	  XmFocusData		focusData;
	  
	  focusData = _XmGetFocusData((ShellWidget)_XmFindTopMostShell((Widget)gadget));

	  /*
	   * we may be getting called as a result of Mwm catching up
	   * with the pointer and setting input focus to the shell
	   * which then gets forwarded to us
	   */
	  if((focusData->focalPoint != XmUnrelated) &&
	     _XmCallFocusMoved(XtParent(gadget), (Widget)gadget, event))
	    {
		if (gadget->gadget.highlight_on_enter)
		  {
		      _XmHighlightBorder ((Widget)gadget);
		  }
	    }
      }
}

/************************************************************************
 *
 *  DispatchGadgetInput
 *	This routine is used instead of _XmDispatchGadgetInput due to
 *	the fact that it needs to dispatch to unmanaged gadgets
 *
 ************************************************************************/
static void DispatchGadgetInput (g, event, mask)
    XmGadget g;
    XEvent * event;
    Mask mask;
{
   if (g->gadget.event_mask & mask && XtIsSensitive (g))
     (*(((XmGadgetClass) (g->object.widget_class))->
	gadget_class.input_dispatch)) ((Widget)g, event, mask);
}

/************************************************************************
 *
 *  _XmLeaveGadget
 *     This function processes leave window conditions occuring in a gadget
 *
 ************************************************************************/

#ifdef _NO_PROTO
void _XmLeaveGadget (gadget, event)
XmGadget gadget;
XEvent * event;
#else /* _NO_PROTO */
void _XmLeaveGadget (XmGadget gadget, XEvent *event)
#endif /* _NO_PROTO */
{
    if (_XmGetFocusPolicy((Widget)gadget) == XmPOINTER)
      {
	  XmFocusData		focusData;
	  
	  focusData = _XmGetFocusData((ShellWidget)_XmFindTopMostShell((Widget)gadget));

	  if((focusData->focalPoint != XmUnrelated)
	     && _XmCallFocusMoved((Widget)gadget, XtParent(gadget), event)
	     )
	    {
		if (gadget->gadget.highlight_on_enter)
		  {
		      _XmUnhighlightBorder ((Widget)gadget);
		  }
	    }
      }
}


/************************************************************************
 *
 *  _XmFocusInGadget
 *     This function processes focusIn conditions occuring in a gadget
 *
 ************************************************************************/

#ifdef _NO_PROTO
void _XmFocusInGadget (gadget, event)
XmGadget gadget;
XEvent * event;

#else /* _NO_PROTO */
void _XmFocusInGadget (XmGadget gadget, XEvent *event)
#endif /* _NO_PROTO */
{
    if(_XmGetFocusPolicy((Widget)gadget) == XmEXPLICIT)
      {
	  if (((XmGadgetClass)XtClass(gadget))->gadget_class.border_highlight)
	    {
		(*(((XmGadgetClass)XtClass(gadget))->gadget_class.border_highlight))
		  (gadget);
	    }
      }
}


/************************************************************************
 *
 *  _XmFocusOutGadget
 *     This function processes FocusOut conditions occuring in a gadget
 *
 ************************************************************************/

#ifdef _NO_PROTO
void _XmFocusOutGadget (gadget, event)
XmGadget gadget;
XEvent * event;

#else /* _NO_PROTO */
void _XmFocusOutGadget (XmGadget gadget, XEvent *event)
#endif /* _NO_PROTO */
{
    if(_XmGetFocusPolicy((Widget)gadget) == XmEXPLICIT)
      {
	  if (((XmGadgetClass)XtClass(gadget))->gadget_class.border_unhighlight)
	    {
		(*(((XmGadgetClass)XtClass(gadget))->gadget_class.border_unhighlight))
		  (gadget);
	    }
      }
}

/************************************************************************
 *
 *  Enter, FocusIn and Leave Window procs
 *
 *     These two procedures handle traversal activation and deactivation
 *     for manager widgets. They are invoked directly throught the
 *     the action table of a widget.
 *
 ************************************************************************/

/************************************************************************
 *
 *  _XmManagerEnter
 *     This function handles both focusIn and Enter. Don't ask me why
 *     :-( 
 *
 ************************************************************************/

#ifdef _NO_PROTO
void _XmManagerEnter (mw, event)
    XmManagerWidget   mw;
    XCrossingEvent    * event;
    
#else /* _NO_PROTO */
void _XmManagerEnter (XmManagerWidget mw, XCrossingEvent *event)
#endif /* _NO_PROTO */
{
    Widget 		child;
    
    if (_XmGetFocusPolicy((Widget)mw) == XmPOINTER)
      {
	  if (UpdatePointerData(mw, event) && event->focus)
	    {
		Widget	old;
		
		if (event->detail == NotifyInferior)
		  old = XtWindowToWidget(event->display, 
					 event->subwindow);
		else
		  old = XtParent(mw);
		(void)_XmCallFocusMoved(old, (Widget)mw, (XEvent*)event);
	    }
      }
}

#ifdef _NO_PROTO
void _XmManagerLeave (mw, event)
    XmManagerWidget   mw;
    XCrossingEvent    * event;
    
#else /* _NO_PROTO */
void _XmManagerLeave (XmManagerWidget mw, XCrossingEvent *event)
#endif /* _NO_PROTO */
{
    Widget child;

    /*
     *  this code is inefficient since it is called twice for each
     * internal move in  the hierarchy |||
     */
    if (event->type == LeaveNotify)
      {
	  if (_XmGetFocusPolicy((Widget)mw) == XmPOINTER)
	    {
		Widget		new;
		XmFocusData	focusData;
		
		if (event->detail == NotifyInferior)
		  new = XtWindowToWidget(event->display, 
					 event->subwindow);
		else 
		  {
		      new = XtParent(mw);
		  }
		if (UpdatePointerData(mw, event) && event->focus)
		  {
		      (void)_XmCallFocusMoved((Widget)mw, new, (XEvent*)event);
		  }
	    }
      }
}

#ifdef _NO_PROTO
void _XmManagerFocusInInternal (mw, event)
    XmManagerWidget   mw;
    XEvent          * event;
    
#else /* _NO_PROTO */
void _XmManagerFocusInInternal (XmManagerWidget mw, XEvent *event)
#endif /* _NO_PROTO */
{
    Widget		child;
    XFocusChangeEvent	*focusEvent = (XFocusChangeEvent *)event;
    
    /*
     * Managers ignore all focus events which have been generated by the
     * window system; only those sent to us by a window manager or the
     * Xtk focus code is accepted.
     * Bail out if the focus policy is not set to explicit
     */
    if (_XmGetFocusResetFlag((Widget)mw)) 
      return;
    else if (_XmGetFocusPolicy((Widget)mw) == XmEXPLICIT)
      {
	  if ((focusEvent->send_event))
	    {
		/* if the heirarchy doesn't have an active tab group give it one */
		if (!_XmGetTabGroup((Widget)mw))
		  _XmMgrTraversal(_XmFindTopMostShell((Widget)mw), 
				  XmTRAVERSE_NEXT_TAB_GROUP);
		else
		  {
		      /* If focus went to a gadget, then force it to highlight */
		      if ((child = mw->manager.active_child) &&
			  XmIsGadget(child)
#ifdef OLD_FOCUS_MOVED
			  && _XmCallFocusMoved(NULL, child, event)
#endif /* OLD_FOCUS_MOVED */	     
			  )
			{
			    if (!(((XmGadget)child)->gadget.have_traversal))
			      {
				  DispatchGadgetInput(child, event, XmFOCUS_IN_EVENT);
				  ((XmGadget)child)->gadget.have_traversal = True;
			      }
			}
#ifdef OLD_FOCUS_MOVED
		      else
			(void)_XmCallFocusMoved(NULL,
						mw,
						event);
#endif /* OLD_FOCUS_MOVED */
		  }
	    }
      }
#ifdef FOLLOW_SERVER
    else if (focusEvent->detail == NotifyPointer)
      {
	  FlushPointerData(mw, event);
      }
#else
    else if (focusEvent->send_event)
      {
	  FlushPointerData(mw, event);
      }
#endif
}

/*
 * Non-menu widgets use this entry point, so that they will ignore focus
 * events during menu activities.
 */

#ifdef _NO_PROTO
void _XmManagerFocusIn (mw, event)
    XmManagerWidget   mw;
    XEvent          * event;
    
#else /* _NO_PROTO */
void _XmManagerFocusIn (XmManagerWidget mw, XEvent *event)
#endif /* _NO_PROTO */
{
    if (!_XmGetTransientFlag((Widget)mw))
      _XmManagerFocusInInternal(mw, event);
}


/*
 * If the manager widget received a FocusOut while it is processing its
 * FocusIn event, then it knows that the focus has been successfully moved
 * to one of its children.  However, if no FocusOut is received, then the
 * manager widget must manually force the child to take the focus.
 */

#ifdef _NO_PROTO
void _XmManagerFocusOut (mw, event)
    
    XmManagerWidget mw;
    XEvent * event;
#else /* _NO_PROTO */
void _XmManagerFocusOut (XmManagerWidget mw, XEvent *event)
#endif /* _NO_PROTO */
{
    Widget child;
    
    if (!event->xfocus.send_event)
      return;
    
    if (_XmGetFocusPolicy((Widget)mw) == XmEXPLICIT)
      {
	/* If focus is in a gadget, then force it to unhighlight */
	if ((child = mw->manager.active_child) && XmIsGadget(child)
#ifdef OLD_FOCUS_MOVED
	    && _XmCallFocusMoved(child, NULL, event)
#endif /* OLD_FOCUS_MOVED */	     
	    )
	  {
	      DispatchGadgetInput(child, event, XmFOCUS_OUT_EVENT);
	      ((XmGadget)child)->gadget.have_traversal = False;
	  }
#ifdef OLD_FOCUS_MOVED
	else
	  (void)_XmCallFocusMoved((Widget)mw, NULL, event);
#endif /* OLD_FOCUS_MOVED */	     
    }
}

/************************************************************************
 *
 * Unmap
 *      Track whether a widget is visible.
 *
 ***********************************************************************/
#ifdef _NO_PROTO
void _XmManagerUnmap (mw, event)
    XmManagerWidget mw;
    XEvent * event;
#else /* _NO_PROTO */
void _XmManagerUnmap (XmManagerWidget mw, XEvent *event)
#endif /* _NO_PROTO */
{
    if (_XmFocusIsHere((Widget)mw))
      _XmProcessTraversal ((Widget)mw, XmTRAVERSE_RIGHT, False);
}


/************************************************************************
 *
 * _XmNavigChangeManaged
 *
 ***********************************************************************/
#ifdef _NO_PROTO
void _XmNavigChangeManaged (w)
    Widget	w;
#else /* _NO_PROTO */
void _XmNavigChangeManaged(Widget w)
#endif /* _NO_PROTO */
{
    if (_XmFocusIsHere(w))
      {
	  XmFocusData	focusData;
	  Widget	shell, focusItem;
	  
	  if ((shell= _XmFindTopMostShell(w)) &&
	      (focusData = _XmGetFocusData((ShellWidget)shell)) &&
	      (focusItem = focusData->focus_item) &&
	      !_XmTestTraversability (focusItem, NULL))
	    {
		XmProcessTraversal(focusItem, XmTRAVERSE_RIGHT);
	    }
      }
}

/************************************************************************
 *
 * _XmMgrTraversal
 *
 ***********************************************************************/
#ifdef _NO_PROTO
Boolean _XmMgrTraversal (w, direction)
    Widget	w;
    int 	direction;
#else /* _NO_PROTO */
Boolean _XmMgrTraversal (Widget w, int direction)
#endif /* _NO_PROTO */
{
    Widget 	activeTab, requestTab, newTab, startWidget;
    XmFocusData	focusData;
    XRectangle 	visRect;
    Boolean	tabbing = FALSE;
    char	error[80];
    /*
     * Traversal only works in VendorShells or MenuShells; any other
     * shell class does not have the focus data information.
     */
    if ((focusData = _XmGetFocusData((ShellWidget)_XmFindTopMostShell(w))) == NULL)
      return False;
    
    if (_XmGetFocusPolicy(w) != XmEXPLICIT)
      {
	  return False;
      }
    
    activeTab = _XmGetTabGroup(w);
    requestTab = _XmFindTabGroup(w);
    startWidget = w;
    
    /*
     * We allow the requesting widget not to be in a tab_group so that
     * the manager can bootstrap the traversal by passing in itself and
     * XmTRAVERSE_NEXT_TAB_GROUP. Otherwise it's an warning.
     */
    if (
	(!activeTab && !requestTab &&
	 (direction != XmTRAVERSE_NEXT_TAB_GROUP))
	)
      {
#ifdef DEBUG	  
	  XtWarning("traversal bootstrap situation with bad parameters");
#endif
	  return False;
      }
    else 
      if (!requestTab || (direction == XmTRAVERSE_NEXT_TAB_GROUP))
      {
	  if ((newTab = _XmFindNextTabGroup(w)) == NULL)
	    {
		_XmClearKbdFocus(w);
		return False;
	    }
	  direction = XmTRAVERSE_CURRENT;
	  startWidget = newTab;
	  tabbing = TRUE;
      }
    else if (direction == XmTRAVERSE_PREV_TAB_GROUP)
      {
	  if ((newTab = _XmFindPrevTabGroup(w)) == NULL)
	    {
		_XmClearKbdFocus(w);
		return False;
	    }
	  direction = XmTRAVERSE_CURRENT;
	  startWidget = newTab;
	  tabbing = TRUE;
      }
    /*
     * we are guaranteed that both activeTab and requestTab are valid
     * candidates since otherwise next_tab_group would have caught
     * them
     */
    else if (activeTab != requestTab)
      {
	  if (direction != XmTRAVERSE_CURRENT)
	    {
#ifdef DEBUG	       
		XtWarning("attempt to traverse to new tab using bad params");
#endif /* DEBUG */
		return False;
	    }
	  else
	    {
		/* try to find a focus candidate for the request widget */
		if ((newTab = _XmFindGrabTab(startWidget)) == NULL)
		  return False;
		startWidget = w;
	    }
      }
    /*
     * since we're staying in same tab group check if it's still
     * active 
     */
    else 
      {
	  XRectangle 	visRect;
	  
	  if (!_XmCreateVisibilityRect((Widget)activeTab, &visRect) ||
	      (!_XmPathIsTraversable(activeTab, 
				     focusData->num_explicit
				     ? XmEXCLUSIVE_TAB_GROUP : XmTAB_GROUP,
				     AboveAndBelow,
				     &visRect)))
	    {
		if ((newTab = _XmFindNextTabGroup(requestTab)) == NULL)
		  {
		      _XmClearKbdFocus(w);
		      return False;
		  }
		direction = XmTRAVERSE_CURRENT;
		startWidget = newTab;
		tabbing = True;
	    }
	  else
	    newTab = requestTab;
      }
    
    /* There is a child to traverse to */
    switch(direction)
      {
	case XmTRAVERSE_NEXT:
	  MoveNext(focusData, newTab); 
          break;
	case XmTRAVERSE_PREV:
	  MovePrev(focusData, newTab); 
          break;
	case XmTRAVERSE_HOME:
	  Home(focusData, newTab);  
          break;
	case XmTRAVERSE_RIGHT:
	case XmTRAVERSE_DOWN:
	  if (focusData->num_explicit)
	    MoveNext(focusData, newTab);
	  else
	    {
		if (startWidget = _XmNavigate(startWidget, direction))
		  Start(focusData, 
			newTab, 
			startWidget);
		else
		  return XmProcessTraversal(newTab,
					    XmTRAVERSE_NEXT_TAB_GROUP);
	    }
	  break;
	case XmTRAVERSE_LEFT:
	case XmTRAVERSE_UP:
	  if (focusData->num_explicit)
	    MovePrev(focusData,
		     newTab);
	  else
	    {
		if (startWidget = _XmNavigate(startWidget, direction))
		  Start(focusData,
			newTab, 
			startWidget);
		else
		  {
		      _XmClearKbdFocus(w);
		      return False;
		  }
	    }
	  break;
	case XmTRAVERSE_CURRENT:
	  if (focusData->num_explicit)
	    {
		if (startWidget)
		  Start(focusData,
			newTab, 
			startWidget);
		else
		  return FALSE;
	    }
	  else 
	    {
		if (tabbing)
		  {
		      startWidget = _XmNavigate(newTab,
						XmTRAVERSE_DOWN);
		      if (!startWidget)
			startWidget = newTab;
		  }
		Start(focusData,
		      newTab, 
		      startWidget);
	    }
	  break;
      }
    return True;
}


/* 
 * startWidget is either a navigable control or a field or an element
 * in a field that has been explicitly boinked to move the focus.
 * We're guaranteed that there is a traversable item in the tab group 
 */
static Boolean Start (focusData, newTab, startWidget)
    XmFocusData	focusData;
    Widget	newTab;
    Widget 	startWidget;
{
    int 		i=0;
    Widget 		child;
    XRectangle 		visRect;
    Boolean		returnVal = FALSE;

    if (((Widget) newTab != startWidget) ||
	XmIsPrimitive(startWidget)|| 
	XmIsGadget(startWidget))
      {
	  /*
	   *  we are assuming that the startWidget is only one level
	   * down.
	   */
	  if (_XmTestTraversability(startWidget, NULL))
	    {
		TraverseToChild(focusData, newTab, startWidget);
		returnVal = TRUE;
	    }
	  else
	    returnVal = FALSE;
      }
    else
      {
	  /*
	   * If an active child is already defined, attempt to move to it.  If it is
	   * no longer accessible, look for another child.
	   */
	  Widget   	chosen;
	  
	  if (chosen = 
	      FindFirstTraversableChild(focusData,
					newTab,
					((XmManagerWidget)newTab)->manager.active_child,
					XmTRAVERSE_CURRENT))
	    {
		TraverseToChild(focusData, newTab, chosen);
		returnVal = TRUE;
	    }
	  else
	    returnVal =  FALSE;
      }
    return returnVal;
}


static Widget FindFirstTraversableChild(focusData, mw, startWidget, direction)
    XmFocusData		focusData;
    XmManagerWidget	mw;
    Widget		startWidget;
    int			direction;
{
    XRectangle		visRect;
    int			i, numKids = mw->composite.num_children;
    int			startIndex = -1, endIndex;
    Widget		travKid = NULL;
    Boolean		isExplicit = (focusData->num_explicit > 0);
    
    
    /*
     * special case for leaf manager tab groups
     */
    if (numKids == 0)
      return (Widget)mw;

    (void) _XmCreateVisibilityRect((Widget)mw, &visRect);
    
    if (startWidget
#ifdef notdef
	&& !(startWidget->core.being_destroyed)
#endif /* notdef */
	)
      {
	  for (i = 0; 
	       (startIndex < 0) && (i < numKids);
	       i++)
	    {
		if (startWidget == mw->composite.children[i])
		  startIndex = i;
	    }
#ifdef DEBUG
	  if (startIndex < 0)
	    {
		XtWarning("startWidget is not in child list");
		return NULL;
	    }
#endif /* DEBUG */	  
      }

    switch (direction)
      {
	case XmTRAVERSE_CURRENT:
	  /*
	   * make sure direction is reset after we sync up the indexes
	   */
	  direction = XmTRAVERSE_NEXT;
	  if (startIndex >= 0)
	    {
		i = startIndex - 1;
		endIndex = numKids;
		
		break;
	    }
	  /* else fall through to TRAVERSE_NEXT */
	case XmTRAVERSE_NEXT:
	  if (startIndex < 0) 
	    {
		startIndex = numKids;
		i = -1;
	    }
	  else 
	    i = startIndex;
	  endIndex = numKids;
	  break;
	case XmTRAVERSE_PREV:
	  if (startIndex < 0) 
	    {
		startIndex = -1;
		i = numKids;
	    }
	  else
	    i = startIndex;
	  endIndex = -1;
      }
    
    /* Determine if there are any traversable children after us */
    while (1)
      {
	  Widget	currKid;

	  if (direction == XmTRAVERSE_NEXT)
	    i++;
	  else
	    i--;
	  
	  if (i == endIndex)
	    {
		if ((endIndex == startIndex) ||
		    (mw->composite.num_children == 1))
		  return NULL;
		else
		  {
		      endIndex = startIndex;
		      if (direction == XmTRAVERSE_NEXT)
			i = 0;
		      else
			i = numKids - 1;
		  }
	    }	  

	  if ((currKid = mw->composite.children[i]) &&
	      (_XmFindTabGroup(currKid) != currKid || (XmIsCommandBox(mw) &&
		XmIsText(currKid))) &&
	      (!isExplicit || !XmIsManager(currKid)))
	    {
		WidgetNavigPtrsRec	npRec;
		WidgetNavigPtrs 	np = &npRec;
		
		GetWidgetNavigPtrs(currKid, np);
		if (_XmWidgetIsTraversable(currKid, 
					   np,
					   isExplicit ? XmNONE_OR_BC : XmTAB_ANY,
					   &visRect))
		  return currKid;
	    }
      }
}

/* We are guaranteed that there is a child to traverse to! */

static void MoveNext (focusData, tabGroup)
    XmFocusData		focusData;
    XmManagerWidget 	tabGroup;
{
    Widget   	chosen;
    
    /* Primitive widgets don't have a next or a previous */
    if(!XmIsManager(tabGroup))
      return;

    chosen = FindFirstTraversableChild(focusData,
				       tabGroup, 
				       tabGroup->manager.active_child,
				       XmTRAVERSE_NEXT);
    TraverseToChild(focusData,
		    tabGroup, 
		    chosen);
}




/* We are guaranteed that there is a child to traverse to! */

static void MovePrev (focusData, tabGroup)
    XmFocusData		focusData;
    XmManagerWidget	tabGroup;
{
    Widget   	chosen;
    
    /* Primitive widgets don't have a next or a previous */
    if(!XmIsManager(tabGroup))
      return;

    chosen = FindFirstTraversableChild(focusData,
				       tabGroup, 
				       tabGroup->manager.active_child,
				       XmTRAVERSE_PREV);
    TraverseToChild(focusData,
		    tabGroup, 
		    chosen);
}


static void Home (focusData, tabGroup)
    XmFocusData		focusData;
    XmManagerWidget 	tabGroup;
{
  Widget candidate, chosen;
  int i=(-1);
  XRectangle visRect;

   /* If this is a primitive widget, then simply traverse to it */
   if (XmIsPrimitive(tabGroup) || XmIsGadget(tabGroup))
   {
      TraverseToChild(focusData, tabGroup, tabGroup);
      return;
   }
  
   chosen = NULL;
   _XmCreateVisibilityRect((Widget)tabGroup, &visRect);

   /* search thru list of children to find the first traversable child */
  chosen = FindFirstTraversableChild(focusData,
				     tabGroup,
				     NULL,
				     XmTRAVERSE_NEXT);

   if (tabGroup->manager.active_child != chosen)
   {
#ifdef notdef
      _XmClearFocusPath(tabGroup->manager.active_child);
      tabGroup->manager.active_child = chosen;
#endif /* notdef */
      TraverseToChild(focusData, tabGroup, chosen);
   }
}



static void TraverseToChild (focus_data, tabGroup, chosen)
    XmFocusData		focus_data;
    XmManagerWidget 	tabGroup;
    Widget 		chosen;
{ 
    Boolean	isExplicit = (focus_data->num_explicit > 0);
    Widget	topmost_shell = _XmFindTopMostShell((Widget)tabGroup);

    if (!chosen || !tabGroup ||
	(XmIsManager(chosen) && isExplicit))
      {
#ifdef DEBUG
	  XtWarning("bad parameters to TraverseToChild");
#endif
	  return;
      }
    /*
     * special case composite tabGroups with no kids
     */
    else if (!isExplicit &&
	     XmIsManager(chosen) &&
	     ((chosen != (Widget)tabGroup) ||
	      (tabGroup->composite.num_children)))
      {
	  Widget oldChosen = chosen;
	  chosen = FindFirstTraversableChild(focus_data,
					     chosen,
					     NULL,
					     XmTRAVERSE_NEXT);
	  if (chosen != oldChosen)
	    {
		TraverseToChild(focus_data,
				tabGroup, 
				chosen);
		return;
	    }
      }
    /*
     * if the item already has the focus don't recycle it.
     */
    if ((chosen != focus_data->focus_item) &&
	_XmCallFocusMoved(focus_data->focus_item,
			  chosen,
			  NULL))
      {
	  XmManagerWidget mwParent = (XmManagerWidget) XtParent(chosen);

	  /* catch old before the focus_item gets cleared */
	  focus_data->old_focus_item = focus_data->focus_item;

	  if (focus_data->active_tab_group != (Widget)tabGroup)
	    {
		_XmChangeTabGroups(focus_data, (Widget)tabGroup);
	    }
	  

	  XtSetKeyboardFocus((Widget)topmost_shell, None);
	  
	  /* 
	   * bc logic: it sets both the tab_group active_child and the
	   * immediate parent's active_child since these may not be
	   * the same with nested managers for layout
	   */
	  if (XmIsManager(tabGroup) && ((Widget)tabGroup != chosen))
	    tabGroup->manager.active_child = chosen;
	  if (XmIsManager(mwParent))
	    mwParent->manager.active_child = chosen;

	  focus_data->focus_item = chosen;
	  XtSetKeyboardFocus(topmost_shell, chosen);
      }
}

#ifdef _NO_PROTO
void _XmClearKbdFocus (tabGroup)

   Widget tabGroup;

#else /* _NO_PROTO */
void _XmClearKbdFocus (Widget tabGroup)
#endif /* _NO_PROTO */
{
   ShellWidget topmost_shell;

#ifdef notdef
   /*
    * if were already cleared then there is no work to be done
    */
   if (!_XmGetTabGroup(tabGroup))
     return;
#endif /* notdef */
   /* 
    * To clear the keyoard focus, we need to reset the shell's focus
    * widget back to the child of the shell.
    */
   topmost_shell = (ShellWidget)_XmFindTopMostShell(tabGroup);

   /* Clear active tab group field in the shell */
   _XmClearFocusPath(tabGroup);
   _XmClearTabGroup(tabGroup);

   _XmSetFocusResetFlag((Widget)topmost_shell, True);
   if (!topmost_shell->core.being_destroyed)
     {
	Widget childwid = NULL;
	int i;
	for (i = 0; i < topmost_shell->composite.num_children; i++) 
	  {
	     if (XtIsManaged(topmost_shell->composite.children[i])) {
		childwid = topmost_shell->composite.children[i];
		break;
	     }
	  }
	if (childwid != tabGroup)
	  {
	      XtSetKeyboardFocus (topmost_shell, None);
	      XtSetKeyboardFocus (topmost_shell, childwid);
	  }
     }
   _XmSetFocusResetFlag((Widget)topmost_shell, False);

}

static void _XmChangeTabGroups (focusData, newTab)
    XmFocusData	focusData;
    Widget	newTab;
{
   ShellWidget 	topmost_shell;
   Widget	oldTab = focusData->active_tab_group;
   
   if (oldTab == newTab)
     return;

   topmost_shell = (ShellWidget)_XmFindTopMostShell(newTab);

   /* 
    * To clear the keyoard focus, we need to reset the shell's focus
    * widget back to the child of the shell.
    */
   if (oldTab)
     {
	 if (!topmost_shell->core.being_destroyed)
	   {
	       XtSetKeyboardFocus (topmost_shell, None);
	   }
	 /* Clear active tab group field in the shell */
	 _XmClearFocusPath(oldTab);
	 _XmClearTabGroup(oldTab);
     }
   if (newTab)
     {
	 focusData->active_tab_group = newTab;
     }
}


/*
 * Starting with the specified widget, clear the active_child field
 * in all manager widgets; then clear the have_traversal field in
 * the primitive widget.
 */

#ifdef _NO_PROTO
void _XmClearFocusPath (w)

   Widget w;

#else /* _NO_PROTO */
void _XmClearFocusPath (Widget w)
#endif /* _NO_PROTO */
{
   ShellWidget 	topmost_shell;
   XmFocusData	focus_data;

   XmManagerWidget mw;
   Boolean isPrimitive = False;
   Boolean isGadget = False;

   topmost_shell = (ShellWidget)_XmFindTopMostShell(w);
   if (!(focus_data = _XmGetFocusData(topmost_shell)))
     return;
#ifdef notdef
   focus_data->old_focus_item = focus_data->focus_item;
#endif
   focus_data->focus_item = NULL;

   
   for (;
	w && !XtIsShell(w);
	w = XtParent(w))
     {
	 if (XmIsPrimitive(w))
	   {
	       isPrimitive = True;
	       mw = (XmManagerWidget)XtParent(w);
	   }
	 else if (XmIsGadget(w))
	   {
	       isGadget = True;
	       mw = (XmManagerWidget)XtParent(w);
	   }
	 else
	   mw = (XmManagerWidget)w;
	 
	 if (mw && XmIsManager(mw))
	   {
	       if (mw->manager.active_child)
		 {
		     if (XmIsPrimitive(mw->manager.active_child))
		       {
			   ((XmPrimitiveWidget)mw->manager.active_child)->
			     primitive.have_traversal = False;
		       }
		     else if (XmIsGadget(mw->manager.active_child))
		       {
			   ((XmGadget)mw->manager.active_child)->
			     gadget.have_traversal = False;
		       }
		     mw->manager.active_child = NULL;
		 }
	   }
	 else if (isPrimitive)
	   ((XmPrimitiveWidget)w)->primitive.have_traversal = False;
	 else if (isGadget)
	   ((XmGadget)w)->gadget.have_traversal = False;
     }
}

/************************************************************************
 *
 *  Find a Traversable Primitive
 *     This functions is used to determine if there is a primitive in 
 *     tab group which can be traversed to.
 *
 ************************************************************************/

#ifdef _NO_PROTO
Boolean _XmFindTraversablePrim (tabGroup)
    CompositeWidget tabGroup;
#else /* _NO_PROTO */
Boolean _XmFindTraversablePrim (CompositeWidget tabGroup)
#endif /* _NO_PROTO */
{
   XRectangle visRect;
   XmFocusData	focusData =
     _XmGetFocusData((ShellWidget)_XmFindTopMostShell((Widget)tabGroup));

   if ((tabGroup->core.being_destroyed) ||
       !_XmCreateVisibilityRect((Widget)tabGroup, &visRect))
     return (False);

   return _XmPathIsTraversable((Widget)tabGroup, 
			       focusData->num_explicit
			       ? XmEXCLUSIVE_TAB_GROUP : XmTAB_GROUP, 
			       BelowOnly,
			       &visRect);
}





/************************************************************************
 *
 *  _XmTestTraversability
 *
 ************************************************************************/

#ifdef _NO_PROTO
Boolean _XmTestTraversability (widget, visRect)
    Widget widget;
    XRectangle * visRect;
#else /* _NO_PROTO */
Boolean _XmTestTraversability (Widget widget, XRectangle *visRect)
#endif /* _NO_PROTO */
{
    WidgetNavigPtrsRec	npRec;
    WidgetNavigPtrs 	np = &npRec;

    GetWidgetNavigPtrs(widget, np);
    return _XmWidgetIsTraversable(widget, np, XmTAB_ANY, visRect);
}
static Boolean BaseClassIsTraversable(widget, np, navType, visRect)
    Widget		widget;
    WidgetNavigPtrs	np;
    XmNavigationType	navType;
    XRectangle * 	visRect;
{
    XRectangle 		tmpRect;
    Boolean		returnVal = True;
    Boolean		sameNavType;

    if ((navType == XmTAB_ANY) 
	||
	((navType == XmNONE_OR_BC) && 
	 (NavigFieldEq(np, navigation_type, XmTAB_GROUP) ||
	  NavigFieldEq(np, navigation_type, XmNONE)))
	||
	(_XmIsTabGroup(navType) && NavigIsTabGroup(np)) 
	||
	((navType == XmNONE) && !NavigIsTabGroup(np)))
      {
	  sameNavType = True;
      }
    else
      {
	  sameNavType = False;
      }

    if (!sameNavType 					||
	NavigFieldEq(np, traversal_on, FALSE)	 	||
	NavigFieldEq(np, sensitive, FALSE)	 	||
	NavigFieldEq(np, ancestor_sensitive, FALSE)	||
	!XtIsManaged(widget)				||
	widget -> core.being_destroyed == True		||
	(visRect && !_XmIntersectRect(visRect, widget, &tmpRect)))
      {
	  returnVal = False;
      }
    /*
     * This next test is only useful if the app is both using
     * mapped_when_managed to allocate space for the widget and using
     * direct calls to map/unmap to get on/off the screen. 
     */
    else if(NavigFieldEq(np, mapped_when_managed, FALSE))
      {
	  XWindowAttributes xwa;
	  XGetWindowAttributes(XtDisplay(widget), XtWindow(widget), &xwa);
	  if(xwa.map_state==IsViewable) 
	    returnVal = True;
	  else
	    returnVal = False;
      }
    return returnVal;
}

static Boolean	RowColumnIsTraversable(widget, np, navType, visRect)
    Widget		widget;
    WidgetNavigPtrs	np;
    XmNavigationType	navType;
    XRectangle * 	visRect;
{
    XRectangle 		tmpRect;
    Boolean		trueVal = True;
    /* Transient menupanes don't have mapped_when_managed set */
    if (XmIsMenuShell(XtParent(widget)) &&
	!widget->core.mapped_when_managed)
      np->mapped_when_managed = &trueVal;

    return (BaseClassIsTraversable(widget, np, navType, visRect));
}	

static Boolean	BCPrimIsTraversable(widget, np, navType, visRect)
    Widget		widget;
    WidgetNavigPtrs	np;
    XmNavigationType	navType;
    XRectangle * 	visRect;
{
    XRectangle 		tmpRect;
    Boolean		trueVal = True;

    if (navType == XmNONE_OR_BC)
      return FALSE;
    else
      return (BaseClassIsTraversable(widget, np, navType, visRect));
}	



/************************************************************************
 *
 *  _XmWidgetIsTraversable
 *
 ************************************************************************/

#ifdef _NO_PROTO
Boolean _XmWidgetIsTraversable(widget, np, navType, visRect)
    Widget 		widget;
    WidgetNavigPtrs 	np;
    XmNavigationType	navType;
    XRectangle		*visRect;
#else /* _NO_PROTO */
Boolean _XmWidgetIsTraversable (Widget widget, WidgetNavigPtrs np, XmNavigationType navType, XRectangle *visRect)
#endif /* _NO_PROTO */
{
    WidgetClass		wc = XtClass(widget);

    if (!XtIsRectObj(widget))
      return False;
    else if (XmIsRowColumn(widget))
      return (RowColumnIsTraversable(widget, np, navType, visRect));
    else if (XmIsSash(widget))
      return (BCPrimIsTraversable(widget, np, navType, visRect));
#ifdef notdef
    else if ((wc == xmLabelGadgetClass) || (wc == xmLabelWidgetClass))
      return (BCPrimIsTraversable(widget, np, navType, visRect));
#endif /* notdef */
    else
      return (BaseClassIsTraversable(widget, np, navType, visRect));
}

/************************************************************************
 *
 *  _XmPathIsTraversable
 *
 ************************************************************************/
#ifdef _NO_PROTO
Boolean _XmPathIsTraversable(widget, navType, testType, visRect)
    Widget		widget;
    XmNavigationType	navType;
    XmNavigTestType	testType;
    XRectangle * 	visRect;
#else /* _NO_PROTO */
Boolean _XmPathIsTraversable (Widget widget, XmNavigationType navType, XmNavigTestType testType, XRectangle *visRect)
#endif /* _NO_PROTO */
{
    XRectangle 		tmpRect;
    Boolean		returnVal = True;
    WidgetNavigPtrsRec	npRec;
    WidgetNavigPtrs 	np = &npRec;

    if (!XtIsRectObj(widget))
      return False;

    GetWidgetNavigPtrs(widget, np);

    returnVal = _XmWidgetIsTraversable(widget, np, navType, visRect);

    if (returnVal &&
	((testType == BelowOnly) ||
	 (testType == AboveAndBelow)))
      {
	  if  (XmIsManager(widget))
	    {
		register int 	i;
		Widget		*currKid;
		XmManagerWidget	mw = (XmManagerWidget)widget;
		Boolean		found = False;
		unsigned int	tabType;

		switch(navType)
		  {
		    case XmEXCLUSIVE_TAB_GROUP:
		      tabType = XmNONE_OR_BC;
		      break;
		    default:
		      tabType = XmNONE;
		      break;
		  }
		for (i = 0, currKid = mw->composite.children;
		     i < mw->composite.num_children && !found;
		     i++, currKid++)
		  {
		      /*
		       * if it's a 1.0 hierarchy, then don't allow nested
		       * navigation
		       */
		      if (!(navType == XmEXCLUSIVE_TAB_GROUP) ||
			  !(XmIsManager(*currKid)))
			found = 
			  _XmPathIsTraversable(*currKid,
					       tabType,
					       BelowOnly,
					       visRect);
		  }
		if (i)
		  returnVal = found;
		else
		  {
		      /*
		       * we are allowing a leaf tab group with no kids
		       */
		  }
	    }
      }
    
    if (returnVal && 
	(widget = XtParent(widget)) &&
	!XtIsShell(widget) &&
	((testType == AboveOnly) ||
	 (testType == AboveAndBelow)))
      {
	  if (!visRect &&  !_XmCreateVisibilityRect(widget, &tmpRect))
	    returnVal = False;
	  else
	    returnVal = _XmPathIsTraversable(widget,
					     XmTAB_ANY,
					     AboveOnly,
					     visRect ? visRect : &tmpRect);
      }
    return (returnVal);
}




#ifdef _NO_PROTO
Boolean _XmFocusIsHere (w)
   Widget 	w;
#else /* _NO_PROTO */
Boolean _XmFocusIsHere (Widget w)
#endif /* _NO_PROTO */
{
    ShellWidget topmost_shell;
    XmFocusData focus_data;
    Widget	item;

    /* Find the topmost shell widget */
    topmost_shell = (ShellWidget)_XmFindTopMostShell(w);
    
    if (!(topmost_shell->core.being_destroyed) &&
	(focus_data = _XmGetFocusData(topmost_shell)) &&
	(item = focus_data->focus_item))
      {
	  for (; !XtIsShell(item); item = XtParent(item))
	    if (item == w)
	      return True;
      }
    return(False);
}




/************************************************************************
 *
 *  _XmProcessTraversal
 *     This function handles all of the directional traversal conditions.
 *     It first verifies that traversal is active, and then invokes the
 *     traversal code.
 *
 ************************************************************************/

#ifdef _NO_PROTO
void _XmProcessTraversal (w, dir, check)
Widget w;
int    dir;
Boolean check;    

#else /* _NO_PROTO */
void _XmProcessTraversal (Widget w, int dir, Boolean check)
#endif /* _NO_PROTO */
{
   if (check && (_XmGetFocusPolicy(w) != XmEXPLICIT))
      return;

   (void) _XmMgrTraversal(w, dir);
}

#ifdef _NO_PROTO
Boolean XmProcessTraversal(w, dir)
    Widget    	w;
    int		dir;
#else /* _NO_PROTO */
Boolean XmProcessTraversal (Widget w, int dir)
#endif /* _NO_PROTO */
{
    return _XmMgrTraversal(w, dir);
}




/*
 * Functions for accessing and manipulating the tab group lists.
 */




/* Find the next tab group which is accessible; return NULL if none found */

#ifdef _NO_PROTO
Widget _XmFindNextTabGroup (w)
    Widget w;
#else /* _NO_PROTO */
Widget _XmFindNextTabGroup (Widget w)
#endif /* _NO_PROTO */
{
    ShellWidget topmost_shell;
    XmFocusData focus_data;
    int i, j, upper_limit;
    Widget oldActiveGroup;
    
    if (w == NULL)
      return(NULL);
    
    if (_XmGetFocusPolicy(w) != XmEXPLICIT)
      {
	  _XmClearKbdFocus(w);
	  return(NULL);
      }
    
    /* Find the topmost shell widget */
    topmost_shell = (ShellWidget)_XmFindTopMostShell(w);
    focus_data = _XmGetFocusData(topmost_shell);
    
    /* Determine where to start our search */
    oldActiveGroup = focus_data->active_tab_group;

    if (!focus_data->num_explicit)
      {
	  /*
	   * in order to allow bootstrapping pass in w 
	   */
	  if (!oldActiveGroup)
	    oldActiveGroup = (Widget)topmost_shell;
	  return _XmNavigate(oldActiveGroup, 
			     XmTRAVERSE_NEXT_TAB_GROUP);
      }
    else
      {
	  upper_limit = focus_data->num_tab_groups;
	  
	  if (oldActiveGroup)
	    {
		for (i = 0; i < upper_limit; i++)
		  {
		      if (oldActiveGroup == focus_data->tab_groups[i])
			{
			    i++;
			    break;
			}
		  }
	    }
	  else
	    i = 0;
	  
	  /* Now, find the next available group */
	  for (j = 0; j < upper_limit; j++)
	    {
		/* Wrap, if necessary */
		if (i >= upper_limit)  
		  i = 0;
		
		/* Test accessibility */
		if (_XmFindTraversablePrim((CompositeWidget)focus_data->tab_groups[i]))
		  {
		      return(focus_data->tab_groups[i]);
		  }
		i++;
	    }
      }
    /* Nothing is accessible */
    _XmClearKbdFocus(w);
    return (NULL);
}


/* Find the previous tab group which is accessible; return NULL if none found */

#ifdef _NO_PROTO
Widget _XmFindPrevTabGroup (w)
    
    Widget w;
    
#else /* _NO_PROTO */
Widget _XmFindPrevTabGroup (Widget w)
#endif /* _NO_PROTO */
{
    ShellWidget topmost_shell;
    XmFocusData focus_data;
    int i, j, upper_limit;
    Widget oldActiveGroup;
    
    if (w == NULL)
      return(NULL);
    
    if (_XmGetFocusPolicy(w) != XmEXPLICIT)
      {
	  _XmClearKbdFocus(w);
	  return(NULL);
      }
    
    /* Find the topmost shell widget */
    topmost_shell = (ShellWidget)_XmFindTopMostShell(w);
    focus_data = _XmGetFocusData(topmost_shell);
    
    /* Determine where to start our search */
    oldActiveGroup = focus_data->active_tab_group;

    if (!focus_data->num_explicit)
      {
	  /*
	   * in order to allow bootstrapping pass in w 
	   */
	  if (!oldActiveGroup)
	    oldActiveGroup = (Widget)topmost_shell;
	  return _XmNavigate(oldActiveGroup, 
			     XmTRAVERSE_PREV_TAB_GROUP);
      }
    else
      {
	  upper_limit = focus_data->num_tab_groups;
	  
	  if (oldActiveGroup)
	    {
		for (i = 0; i < upper_limit; i++)
		  {
		      if (oldActiveGroup == focus_data->tab_groups[i])
			{
			    i--;
			    break;
			}
		  }
	    }
	  else
	    i = 0;
	  
	  /* In case the old group is not in the list */
	  if (i >= upper_limit)
	    i = upper_limit - 1;
	  
	  /* Now, find the next available group */
	  for (j = 0; j < upper_limit; j++)
	    {
		/* Wrap, if necessary */
		if (i < 0)  
		  i = upper_limit -1;
		
		/* Test accessibility */
		if (_XmFindTraversablePrim((CompositeWidget)focus_data->tab_groups[i]))
		  {
		      return(focus_data->tab_groups[i]);
		  }
		i--;
	    }
      }
    /* Nothing is accessible */
    _XmClearKbdFocus(w);
    return (NULL);
}


/* Function to query the current focus policy for this widget hierarchy */

unsigned char _XmGetFocusPolicy (w)

   Widget w;

{
   ShellWidget 		topmost_shell;

   /* Find the topmost shell widget */
   topmost_shell = (ShellWidget)_XmFindTopMostShell(w);

   if (XtIsVendorShell((Widget)topmost_shell))
     return (((XmVendorShellExtObject)
	      (_XmGetWidgetExtData((Widget)topmost_shell, XmSHELL_EXTENSION))->widget)
	     ->vendor.focus_policy);
   else if (XmIsMenuShell(topmost_shell))
     return ((XmMenuShellWidget)topmost_shell)->menu_shell.focus_policy;
   else
     return(XmPOINTER);
}


/* Clear the active tab group for the specified widget hierarchy */

#ifdef _NO_PROTO
void _XmClearTabGroup (w)
   Widget w;
#else /* _NO_PROTO */
void _XmClearTabGroup (Widget w)
#endif /* _NO_PROTO */
{
   ShellWidget topmost_shell;
   XmFocusData focus_data;
   Widget tabGroup;

   if (w == NULL)
      return;

   topmost_shell = (ShellWidget)_XmFindTopMostShell(w);
   if ((focus_data = _XmGetFocusData(topmost_shell)) == NULL)
      return;

   tabGroup = focus_data->active_tab_group;
   if (tabGroup && XmIsManager(tabGroup))
      ((XmManagerWidget)tabGroup)->manager.active_child = NULL;

   focus_data->active_tab_group = NULL;
#ifdef notdef
   focus_data->old_focus_item = focus_data->focus_item;
#endif
   focus_data->focus_item = NULL;
}

/*
 * Given a primitive widget, find the tab group (if any) that this widget 
 * is in.  If it is in a tab group, then either it, or its parent will be
 * on the tab group list.  If both the parent and the child are tab groups,
 * then we will return the parents tab group.
 */
#ifdef _NO_PROTO
Widget _XmFindTabGroup (widget)
    
    Widget widget;
    
#else /* _NO_PROTO */
    Widget _XmFindTabGroup (Widget widget)
#endif /* _NO_PROTO */
{
    WidgetNavigPtrsRec	npRec;
    WidgetNavigPtrs	np = &npRec;
    XmFocusData 	focusData;
    Widget		w = widget;

    if (w == NULL)
      return (NULL);
    
    if (_XmGetFocusPolicy(w) != XmEXPLICIT)
      {
	  _XmClearKbdFocus(w);
	  return(NULL);
      }
    
    focusData = _XmGetFocusData((ShellWidget)_XmFindTopMostShell(w));
    
    
    for (; w; w = XtParent(w))
      {
	  GetWidgetNavigPtrs(w, np);
	  
	  if (focusData->num_explicit)
	    {
		if ((NavigFieldEq(np, navigation_type,
				  XmEXCLUSIVE_TAB_GROUP) ||
		     NavigFieldEq(np, navigation_type,
				  XmSTICKY_TAB_GROUP)) &&
		    _XmFindTraversablePrim((CompositeWidget)w))
		  return w;
		else if ((w != widget) && (w != XtParent(widget)))
		  return NULL;
	    }
	  else
	    {
		if (NavigIsTabGroup(np))
		  return w;
	    }
      }
    return (NULL);
}


/* Return the active tab group for the specified widget hierarchy */

#ifdef _NO_PROTO
Widget _XmGetTabGroup (w)

   Widget w;

#else /* _NO_PROTO */
Widget _XmGetTabGroup (Widget w)
#endif /* _NO_PROTO */
{
    ShellWidget	topmost_shell;
    XmFocusData	focus_data;

   topmost_shell = (ShellWidget)_XmFindTopMostShell(w);
   if ((focus_data = _XmGetFocusData(topmost_shell)) == NULL)
      return(NULL);

   /*
    * MenuShell widgets ALWAYS have one tab group: their menu pane.
    */
   if (XmIsMenuShell(topmost_shell))
   {
      if ((topmost_shell->composite.num_children > 0) &&
          (!topmost_shell->core.being_destroyed))
      {
         focus_data->active_tab_group = topmost_shell->composite.children[0];
      }
   }

   return(focus_data->active_tab_group);
}


/* Find the topmost shell widget associated with the specified widget */

#ifdef _NO_PROTO
Widget _XmFindTopMostShell (w)
    Widget w;
#else /* _NO_PROTO */
Widget _XmFindTopMostShell (Widget w)
#endif /* _NO_PROTO */
{
    Widget topmost_shell = w;

   if (w == NULL)
      return(NULL);

   while (!XtIsShell(topmost_shell) && XtParent(topmost_shell))
      topmost_shell = XtParent(topmost_shell);

   return((Widget)topmost_shell);
}

static void AddTabGroup(tabGroup, newNavType)
    Widget	tabGroup;
    XmNavigationType newNavType;
{
   ShellWidget topmost_shell = (ShellWidget) _XmFindTopMostShell(tabGroup);
   XmFocusData focus_data;
   int i;

   if (((focus_data = _XmGetFocusData(topmost_shell)) == NULL) ||
       (!(newNavType == XmEXCLUSIVE_TAB_GROUP) &&
	!(newNavType == XmSTICKY_TAB_GROUP)))
     return;

   /* see if it's already in the tab list */
   for (i = 0; i < focus_data->num_tab_groups; i++)
     if (focus_data->tab_groups[i] == tabGroup)
       return;

   /* Grow the list, if necessary */
   if (++focus_data->num_tab_groups >= focus_data->size_tab_group_list)
   {
      focus_data->size_tab_group_list += 10;
      focus_data->tab_groups = (Widget *)
	XtRealloc((XtPointer)focus_data->tab_groups,
		  focus_data->size_tab_group_list * sizeof(Widget));
   }

   focus_data->tab_groups[focus_data->num_tab_groups-1] = tabGroup;

   if (newNavType == XmEXCLUSIVE_TAB_GROUP)
     focus_data->num_explicit++;
}


static void RemoveTabGroup (tabGroup, navType, move)
    Widget 		tabGroup;
    XmNavigationType	navType;
    Boolean move;
{
    ShellWidget topmost_shell = (ShellWidget) _XmFindTopMostShell(tabGroup);
    XmFocusData focus_data;
    int i, j;
    Boolean focusIsHere;
    
    if (topmost_shell->core.being_destroyed ||
	((focus_data = _XmGetFocusData(topmost_shell)) == NULL))
      return;
    
    focusIsHere = _XmFocusIsHere(tabGroup);
    
    
    if (focusIsHere)
      {
	  /*
	   * should probably go to the next widget whatever that means
	   * but that's more complicated... so punt
	   */

	  if (move) (void)XmProcessTraversal(tabGroup, XmTRAVERSE_NEXT_TAB_GROUP);

	  /*
	    dd: potentially remove this call, since it screw up the changenavigationtype
	    call and it should not append (navigdestroy already does something
	    that move the focus */
	
	  if (!XmIsManager(tabGroup))
	    {
		if (XmIsManager(XtParent(tabGroup)))
		  ((XmManagerWidget)XtParent(tabGroup))->manager.active_child = NULL;
	    }
	  else
	    ((XmManagerWidget)tabGroup)->manager.active_child = NULL;
      }

    for (i = 0; i < focus_data->num_tab_groups; i++)
      {
	  if (focus_data->tab_groups[i] == tabGroup)
	    {
		for (j = i; j < focus_data->num_tab_groups-1; j++)
		  focus_data->tab_groups[j] = focus_data->tab_groups[j+1];
		
		focus_data->num_tab_groups--;
		if (navType == XmEXCLUSIVE_TAB_GROUP) 
		  focus_data->num_explicit--;
		break;
	    }
      }
}


/*******************
 * Public procedures
 *******************/


#ifdef _NO_PROTO
void XmAddTabGroup (tabGroup)

   Widget tabGroup;

#else /* _NO_PROTO */
void XmAddTabGroup (Widget tabGroup)
#endif /* _NO_PROTO */
{
    Arg		arg;

    XtSetArg(arg, XmNnavigationType, XmEXCLUSIVE_TAB_GROUP);
    XtSetValues(tabGroup, &arg, 1);
}


#ifdef _NO_PROTO
void XmRemoveTabGroup(w)
    Widget	w;
#else /* _NO_PROTO */
void XmRemoveTabGroup (Widget w)
#endif /* _NO_PROTO */
{
    Arg		arg;

    XtSetArg(arg, XmNnavigationType, XmNONE);
    XtSetValues(w, &arg, 1);
}


/*
 * Invoked by the VendorShell widget, when the focus_policy changes.
 * Registered as a callback by both the Manager and Primitive classes,
 * when the parent is a VendorShell widget.
 */

#ifdef _NO_PROTO
void _XmFocusModelChanged (topmost_shell, client_data, call_data)
    ShellWidget topmost_shell;
    caddr_t client_data;
    caddr_t call_data;
#else /* _NO_PROTO */
void _XmFocusModelChanged (ShellWidget topmost_shell, caddr_t client_data, caddr_t call_data)
#endif /* _NO_PROTO */
{
    unsigned char focus_policy = (unsigned char)call_data;
    XmFocusData focus_data;
    Widget childwid = NULL;
    int i;
    
    for (i = 0; i < topmost_shell->composite.num_children; i++) 
      {
	  if (XtIsManaged(topmost_shell->composite.children[i])) {
	      childwid = topmost_shell->composite.children[i];
	      break;
	  }
      }
    
    if (focus_policy == XmPOINTER)
      {
	  focus_data = _XmGetFocusData(topmost_shell);
	  
	  if (focus_data->active_tab_group)
	    {
		if (!topmost_shell->core.being_destroyed)
		  {
#ifdef notdef
		      _XmSetFocusResetFlag((Widget)topmost_shell, True);
#endif
		      XtSetKeyboardFocus (topmost_shell, NULL);
		      XtSetKeyboardFocus(topmost_shell, 
					 childwid);
#ifdef notdef
		      _XmSetFocusResetFlag((Widget)topmost_shell, False);
#endif
		  }
		
		_XmClearKbdFocus(focus_data->active_tab_group);
	    }
      }
    else
      {
	  (void) _XmMgrTraversal(childwid, 
				 XmTRAVERSE_NEXT_TAB_GROUP);
      }
}


/*
 * Function used by TextEdit widgets to grab the focus; the widget must
 * be both traversable AND in a Tab Group to do this.  The request will
 * fail if either of these requirements are not met.
 */

#ifdef _NO_PROTO
Boolean _XmGrabTheFocus (w, event)

   Widget w;
   XEvent *event;

#else /* _NO_PROTO */
Boolean _XmGrabTheFocus (Widget w, XEvent *event)
#endif /* _NO_PROTO */
{
   _XmMgrTraversal(w, XmTRAVERSE_CURRENT);
}


static Widget _XmFindGrabTab(w)
    Widget	w;
{
   XRectangle 	visRect;
   Widget	tabGroup;

   if ((_XmGetFocusPolicy(w) != XmEXPLICIT) 	||
       !_XmCreateVisibilityRect(w, &visRect) 	||
       !_XmTestTraversability(w, &visRect))
     return(NULL);
   else
     return(_XmFindTabGroup(w));
}


/*
 * This function returns a pointer to the focus data associated with the
 * topmost shell.  This allows us to treat the location opaquely.
 */

#ifdef _NO_PROTO
XmFocusData _XmGetFocusData (topmost_shell)

   ShellWidget topmost_shell;

#else /* _NO_PROTO */
XmFocusData _XmGetFocusData (ShellWidget topmost_shell)
#endif /* _NO_PROTO */
{
   if ((topmost_shell == NULL) || topmost_shell->core.being_destroyed)
      return(NULL);

   if (XmIsVendorShell((Widget)topmost_shell))
   {
     return (((XmVendorShellExtObject)
	      (_XmGetWidgetExtData((Widget)topmost_shell, XmSHELL_EXTENSION))->widget)
	     ->vendor.focus_data);
   }
   else if (XmIsMenuShell(topmost_shell))
   {
      XmMenuShellWidget ms = (XmMenuShellWidget)topmost_shell;

      return (ms->menu_shell.focus_data);
   }
   else
      return(NULL);
}


/*
 * Depending upon the widget coming in, extract its mapped_when_managed flag
 * and its managed flag.
 */

#ifdef _NO_PROTO
Boolean _XmGetManagedInfo (w)
    
    Widget w;
    
#else /* _NO_PROTO */
Boolean _XmGetManagedInfo (Widget w)
#endif /* _NO_PROTO */
{
    if (XmIsPrimitive (w))
      return (w->core.managed && w->core.mapped_when_managed);
    else if (XmIsGadget (w))
      return (w->core.managed);
    else
      {
	  /* Treat menupanes specially */
	  if (XmIsRowColumn(w) && XmIsMenuShell(XtParent(w)))
	    {
		return (True);
	    }
	  else
	    return (w->core.managed && w->core.mapped_when_managed);
      }
}

/*
 * This function will generate a rectangle describing the portion of the
 * specified widget which is not clipped by any of its ancestors.  It also
 * verifies that the ancestors are both managed and mapped_when_managed.
 * It will return True if a non-empty intersection exists; it will return
 * False if no intersection occurs or one of the ancestors is not managed 
 * or mapped_when_managed.
 *
 * The passed-in widget must be a manager widget.
 */

#ifdef _NO_PROTO
Boolean _XmCreateVisibilityRect (w, rectPtr)

   Widget w;
   XRectangle * rectPtr;

#else /* _NO_PROTO */
Boolean _XmCreateVisibilityRect (Widget w, XRectangle *rectPtr)
#endif /* _NO_PROTO */
{
   if (!_XmGetManagedInfo(w))
   {
       _XmClearRect(rectPtr);
       return(False);
   }

   _XmSetRect(rectPtr, w);
   w = XtParent(w);

   /* Process all widgets, excluding the shell widget */
   while (!XtIsShell(w))
   {
      if (!_XmGetManagedInfo(w) ||
          !_XmIntersectRect(rectPtr, w, rectPtr))
      {
         _XmClearRect(rectPtr);
         return (False);
      }

      w = XtParent(w);
   }

   return (True);
}

/*
 * Initialize the rectangle structure to the specified values.
 */

_XmSetRect (rect, w)

   register XRectangle * rect;
   Widget w;

{
   Position x, y;

   XtTranslateCoords(XtParent(w), w->core.x, w->core.y, &x, &y);
   rect->x = x + w->core.border_width;
   rect->y = y + w->core.border_width;;
   rect->width = w->core.width;
   rect->height = w->core.height;
}

/*
 * Intersects the specified rectangle with the rectangle describing the
 * passed-in widget.  Returns True if they intersect, or False if they
 * do not.
 */

_XmIntersectRect (srcRectA, widget, dstRect)

   register XRectangle * srcRectA;
   register Widget  widget;
   register XRectangle * dstRect;

{
   register int srcABot, widgetBot;
   register int srcARight, widgetRight;
   Position x, y;
   int newHeight, newWidth;

   XtTranslateCoords(XtParent(widget), widget->core.x, widget->core.y, &x, &y);
   x += widget->core.border_width;
   y += widget->core.border_width;

   srcABot = srcRectA->y + srcRectA->height - 1;
   widgetBot = y + widget->core.height - 1;
   srcARight = srcRectA->x + srcRectA->width - 1;
   widgetRight = x + widget->core.width - 1;

   if (srcRectA->x >= x) 
       dstRect->x = srcRectA->x;
   else 
       dstRect->x = x;

   if (srcRectA->y > y) 
       dstRect->y = srcRectA->y;
   else 
       dstRect->y = y;

   if (srcARight >= widgetRight) 
   {
       newWidth = widgetRight - dstRect->x + 1;
       dstRect->width = (newWidth > 0) ? newWidth : 0;
   }
   else 
   {
       newWidth = srcARight - dstRect->x + 1;
       dstRect->width = (newWidth > 0) ? newWidth : 0;
   }

   if (srcABot > widgetBot) 
   {
       newHeight = widgetBot - dstRect->y + 1; 
       dstRect->height = (newHeight > 0) ? newHeight : 0;
   }
   else 
   {
       newHeight = srcABot - dstRect->y + 1;
       dstRect->height = (newHeight > 0) ? newHeight : 0;
   }

   if (_XmEmptyRect(dstRect))
      return(FALSE);

   return(TRUE);
}



_XmEmptyRect (r)

   register XRectangle * r;

{
   if (r->width <= 0 || r->height <= 0)
      return (TRUE);

   return (FALSE);
}

_XmClearRect (r)

   register XRectangle * r;

{
   r->x = 0;
   r->y = 0;
   r->width = 0;
   r->height = 0;
}
