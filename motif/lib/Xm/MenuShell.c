#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)MenuShell.c	3.24.1.2 91/03/20";
#endif /* lint */
#endif /* REV_INFO */
/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
*  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
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
#include <X11/ShellP.h>
#include <X11/VendorP.h>
#include <Xm/MenuShellP.h>
#include <Xm/RowColumnP.h>
#include <Xm/LabelP.h>
#include <Xm/CascadeBP.h>
#include <Xm/LabelGP.h>
#include <Xm/CascadeBGP.h>

extern XContext _XmMenuCursorContext;
extern XmButtonEventStatusRec _XmButtonEventStatus;
extern int _XmOnPostFromList();
extern void _XmGetActiveTopLevelMenu();


static Boolean SetValues ();
static void Resize();
static void PopdownOne ();
static void PopdownEveryone ();
static void PopdownDone ();
static void ClassInitialize ();
static void ClassPartInitialize ();
static void Initialize ();
/* BEGIN OSF fix pir 2131 */
static XtGeometryResult GeometryManager();
/* END OSF fix pir 2131 */
static void managed_set_changed ();
static void InsertChild ();
static void DeleteChild ();
static void Destroy ();
static void ClearTraversal ();
static void PopupSharedMenupane();
#ifdef _NO_PROTO
void _XmEnterRowColumn ();
#else /* _NO_PROTO */
void _XmEnterRowColumn (Widget widget, caddr_t closure, XEvent *event);
#endif /* _NO_PROTO */

#define Events ButtonPressMask|ButtonReleaseMask|EnterWindowMask|LeaveWindowMask

/* Warning messages */
#define ChildMsg "MenuShell widgets must have a xmRowColumnWidgetClass child"
#define ManageMsg "Attempting to manage an incomplete menu."

static XtTranslations translations_parsed ;

#ifndef MCCABE
static char translations [] = 
    "<BtnDown>:   ClearTraversal() \n\
     <BtnUp>:     MenuShellPopdownDone()";
#else
static char translations [];
#endif

static XtActionsRec actionsList[] = 
{
    {"MenuShellPopdownOne", (XtActionProc) PopdownOne},
    {"MenuShellPopdownDone", (XtActionProc) PopdownDone},
    {"ClearTraversal",       (XtActionProc) ClearTraversal}
};
static XtResource resources[] =
{
    {
         XmNdefaultFontList,
         XmCDefaultFontList,
         XmRFontList,
         sizeof(XmFontList),
         XtOffset(XmMenuShellWidget, menu_shell.default_font_list),
         XmRString,
         (caddr_t) "fixed"
    }
};


static XmBaseClassExtRec baseClassExtRec = {
    NULL,
    NULLQUARK,
    XmBaseClassExtVersion,
    sizeof(XmBaseClassExtRec),
    NULL,				/* InitializePrehook	*/
    NULL,				/* SetValuesPrehook	*/
    NULL,				/* InitializePosthook	*/
    NULL,				/* SetValuesPosthook	*/
    NULL,				/* secondaryObjectClass	*/
    NULL,				/* secondaryCreate	*/
    (XmGetSecResDataFunc)NULL,		/* getSecRes data	*/
    0,					/* fastSubclass flags	*/
    NULL,				/* get_values_prehook	*/
    NULL,				/* get_values_posthook	*/
};



externaldef(xmmenushellclassrec) XmMenuShellClassRec xmMenuShellClassRec = 
{
    {
        (WidgetClass) & overrideShellClassRec,  /* superclass */
        "XmMenuShell",                  /* class_name */
        sizeof (XmMenuShellWidgetRec),  /* widget size */
        ClassInitialize,                /* Class Initializer */
        ClassPartInitialize,            /* chained class init */
        FALSE,
        Initialize,                     /* instance init proc */
        NULL,                           /* init_hook proc */
        XtInheritRealize,               /* instance realize proc */
        actionsList,                    /* actions */
        XtNumber(actionsList),          /* num_actions */
        resources,                      /* resource list */
        XtNumber(resources),            /* resource_count */
        NULLQUARK,                      /* xrm_class */
        TRUE,                           /* do compress_motion */
        XtExposeCompressMaximal,	/* do compress_exposure */
        TRUE,                           /* do compress enter-leave */
        FALSE,                          /* do have visible_interest */
        Destroy,                           /* instance widget destroyb */
        Resize,                         /* resize */
        XtInheritExpose,                /* expose */
        SetValues,                      /* set values proc */
        NULL,                           /* set values hook proc */
        XtInheritSetValuesAlmost,       /* set_values_almost proc */
        NULL,                           /* get_values_hook */
        NULL,                           /* accept_focus proc */
        XtVersion,                      /* current version */
        NULL,                           /* callback offset    */
        NULL,                           /* translation table */
        NULL,                           /* query geo proc */
        NULL,                           /* disp accelerator */
	(XtPointer)&baseClassExtRec,    /* extension */
    },
    {                       /* composite class record */
/* BEGIN OSF fix pir 2131 */
        GeometryManager,       		/* geo mgr */
/* END OSF fix pir 2131 */
        managed_set_changed,            /* have our own */
        InsertChild,                    /* and this too */
        DeleteChild,
	NULL,                           /* Extension */
    },
    {                       /* shell class record */
        NULL,                           /* extension */
    },
    {                       /* override shell class record */
        NULL,                           /* extension */
    },
    {                       /* menushell class record */
        PopdownOne,
	PopdownEveryone,
        PopdownDone,
        PopupSharedMenupane,
        NULL,                           /* extension */
    },
};

externaldef(xmmenushellwidgetclass) WidgetClass xmMenuShellWidgetClass = 
   (WidgetClass) &xmMenuShellClassRec;


static Time lastManagedMenuTime = NULL;

/*
 * When using an override redirect window, it is safe to draw to the
 * window as soon as you have mapped it; you need not wait for exposure
 * events to arrive.  So ... to force menupanes to post quickly, we will
 * redraw all of the items now, and ignore the exposure events we receive
 * later.
 */

static void _XmFastExpose (rowcol)
register XmManagerWidget rowcol;
{
   register int i;
   register Widget child;

   /* Process the menupane */
   (*(XtClass(rowcol)->core_class.expose))(rowcol, NULL, NULL);

   /* Process each windowed child */
   for (i = 0; i < rowcol->composite.num_children; i++)
   {
      child = rowcol->composite.children[i];

      if (XtIsWidget(child) && 
          XtIsManaged(child))
      {
         (*(XtClass(child)->core_class.expose))(child, NULL, NULL);
      }
   }

   XFlush(XtDisplay(rowcol));

   /* Set one-shot, so menupane will ignore next expose */
   RC_SetExpose(rowcol, False);
}



/*
 * When unposting a group of cascading menupanes, we will first unmap all
 * of the MenuShell widgets, and then take care of the other work needed
 * to get the job done.  We do it this way, so that we can give the user
 * the fastest possible feedback.
 */

static void _XmFastPopdown (shell)
XmMenuShellWidget shell;
{
   if (RC_PopupPosted(shell->composite.children[0]))
       _XmFastPopdown(RC_PopupPosted(shell->composite.children[0]));
   
   XtUnmapWidget(shell);
}



/*
 * Post the requested menu shell widget,
 */

static void PostMenuShell(menuShell, grab_kind, spring_loaded)

   XmMenuShellWidget menuShell;
   XtGrabKind grab_kind;
   Boolean spring_loaded;

{
   if (spring_loaded)
       XtPopupSpringLoaded (menuShell);
   else
       XtPopup (menuShell, grab_kind);

   /* mark the row column as NOT popping down */
   RC_SetPoppingDown(menuShell->composite.children[0], False);
}


/*
 * Class Initialize.  Parse the translation tables and set up fast subclassing.
 */
static void ClassInitialize ()
{
   translations_parsed = XtParseTranslationTable (translations);
   _XmInitializeExtensions();
   baseClassExtRec.record_type = XmQmotif;
}


static void ClassPartInitialize (wc)
WidgetClass wc;
{
   _XmBaseClassPartInitialize(wc);
   _XmFastSubclassInit (wc, XmMENU_SHELL_BIT);
}


/*
 * Initialize routine
 */
/* ARGSUSED */
static void Initialize (req, new)
Widget req, new;
{
    XmMenuShellWidget ms = (XmMenuShellWidget) new;

    new->core.background_pixmap = None;

    XtBorderWidth (new) = 0;            /* Style Guide says so */

    /* set up translations before realizing */
    if (new->core.tm.translations == NULL)
	new->core.tm.translations = translations_parsed;

    XtRealizeWidget (new);

    ms->menu_shell.focus_data = (XmFocusData)_XmCreateFocusData();

    ms->menu_shell.focus_policy = XmPOINTER;

    ms->shell.allow_shell_resize = TRUE;

    /* Assume, for now, that the application created this shell */
    ms->menu_shell.private_shell = False;
}


/*
 * set values
 *
 * Don't allow the allowShellResize flag to be set false
 */
/* ARGSUSED */
static Boolean SetValues (old, requested, new)
XmMenuShellWidget old, requested, new;
{
   new->shell.allow_shell_resize = TRUE;
   return (TRUE);
}


/*
 * Resize our first child
 *
 * The statement pertaining to shared menupanes is commented out because
 * if the user is rapidly moving thru the menubar, an old configure notify
 * event may come in, cause the shell to resize the wrong child.
 */

static void Resize (ms)

   XmMenuShellWidget ms;

{
   if (((ms->composite.num_children == 1) && 
        (XtIsManaged(ms->composite.children[0]))))
/*
     || ((ms->composite.num_children > 1) && (ms->shell.popped_up)))
*/
   {
      Widget child = ms->composite.children[0];

      XtConfigureWidget(child, -ms->core.border_width, -ms->core.border_width,
		       ms->core.width, ms->core.height, ms->core.border_width);
   }
}


/*
 * DeleteChild routine
 */
static void DeleteChild (widget)
Widget widget;
{
   XmMenuShellWidget parent = (XmMenuShellWidget)XtParent(widget);

   /* Remove the child as our tab group */
   XmRemoveTabGroup(widget);

   /* Let composite class finish the work */
   (*(((CompositeWidgetClass)compositeWidgetClass)->composite_class.
          delete_child))(widget);

   /*
    * If we had multiple children, but we've now dropped down to 1 child,
    * then we need to unmanage the child.
    */
   if (parent->composite.num_children == 1)
   { 
       if (!parent->shell.popped_up)
           (parent->composite.children[0])->core.managed = False;
   }
   else if ((parent->composite.num_children == 0) && 
            (parent->menu_shell.private_shell) &&
            (!parent->core.being_destroyed))
   {
       /* Destroy private shells, when they have no more children */
       XtDestroyWidget(parent);
   }
}

/*
 * InsertChild routine
 */
static void InsertChild (widget)
Widget widget;
{
    CompositeWidget parent = (CompositeWidget) widget->core.parent;

    if (XmIsRowColumn (widget))
    {
       if ((RC_Type(widget) == XmMENU_PULLDOWN) ||
	   (RC_Type(widget) == XmMENU_POPUP))
       {
	  XtAddEventHandler (widget, EnterWindowMask, FALSE,
			     _XmEnterRowColumn, FALSE);
       }

       /*
        * add to composite list
        */

       (*((CompositeWidgetClass)compositeWidgetClass)->composite_class.
              insert_child) (widget);

       /* Add the new child as our tab group */
       XmAddTabGroup(widget);

       XtRealizeWidget(widget);

       /*
        * If only one child, then we will manage and unmanage it when we
        * want it to post or unpost.  If there are more than one child, then
        * we will have them always managed, and we will use a different
        * technique for posting and unposting.
        */
       if (parent->composite.num_children == 1)
       {
          XtSetKeyboardFocus(parent, widget);
       }
       else if (parent->composite.num_children == 2)
       {
          XtManageChildren(parent->composite.children, 2);
       }
       else
       {
          XtManageChild(widget);
       }
       if (parent->composite.num_children == 1)
       {
	  XMapWindow(XtDisplay(widget), XtWindow(widget));
       }
    }
    else
       _XmWarning(widget, ChildMsg);
}


static void ForceMenuPaneOnScreen (rowcol, x, y)
register XmRowColumnWidget rowcol;
register Position * x;
register Position * y;
{
   Position rightEdgeOfMenu, bottomEdgeOfMenu;
   Dimension dispWidth, dispHeight;
   Position old_x;
   Widget pulldown_button = RC_CascadeBtn(rowcol);
   Dimension RowColBorderWidth = rowcol->core.border_width << 1;
   Dimension CascadeBorderWidth;
   if (pulldown_button)
       CascadeBorderWidth = pulldown_button->core.border_width << 1;

   /* Force the rowcol to be completely visible */

   old_x = *x;
   rightEdgeOfMenu = *x + RowColBorderWidth + rowcol->core.width;
   bottomEdgeOfMenu = *y + RowColBorderWidth + rowcol->core.height;
   dispWidth = WidthOfScreen (XtScreen(rowcol));
   dispHeight = HeightOfScreen (XtScreen(rowcol));

   /*
    * For OPTION menus, if the submenu is [partially] offscreen, offset it
    * off the button.
    */
/* BEGIN OSF Fix pir 2548 */
   if (pulldown_button && XtParent(pulldown_button) && 
       (RC_Type(XtParent(pulldown_button)) == XmMENU_OPTION))
/* END OSF Fix pir 2548 */
   {
      if (*y < 0)
      {
          *y = 0;

	  /* Consider CascadeBtn as well as RowCol width to allow multi
	   * column RowColumn
	   */
          *x += pulldown_button->core.width + CascadeBorderWidth;
	  rightEdgeOfMenu = *x + RowColBorderWidth + rowcol->core.width;
          bottomEdgeOfMenu = *y + RowColBorderWidth + rowcol->core.height;
      }

      if (bottomEdgeOfMenu >= (Position)dispHeight)
      {
          *y = dispHeight - rowcol->core.height - RowColBorderWidth - 1;
          *x += pulldown_button->core.width + CascadeBorderWidth;
	  rightEdgeOfMenu = *x + RowColBorderWidth + rowcol->core.width;
          bottomEdgeOfMenu = *y + RowColBorderWidth + rowcol->core.height;
      }

      if (*x < 0)
      {
          *x += pulldown_button->core.width + CascadeBorderWidth;
	  rightEdgeOfMenu = *x + RowColBorderWidth + rowcol->core.width;
      }

      if (rightEdgeOfMenu >= (Position)dispWidth)
      {
	  *x = old_x - rowcol->core.width + RowColBorderWidth;
	  rightEdgeOfMenu = *x + RowColBorderWidth + rowcol->core.width;
      }
   }

   /*
    * If the submenu is offscreen force it completely on.
    */
   if (*x < 0)
       *x = 0;

   if (*y < 0)
       *y = 0;

   if (rightEdgeOfMenu >= (Position)dispWidth)
       *x -= (rightEdgeOfMenu - dispWidth + 1);

   if (bottomEdgeOfMenu >= (Position)dispHeight)
   {
/* BEGIN OSF Fix pir 2548 */
      if (pulldown_button && XtParent(pulldown_button) &&
	  (RC_Type(XtParent(pulldown_button)) == XmMENU_BAR))
/* END OSF Fix pir 2548 */
      {
	 Position y_temp = *y;

	 /* this menu pane is being pulled down from a bar */
	 /* it doesn't fit, so we have to place it above   */
	 /* the bar -- if it will fit                      */

	 y_temp -= (CascadeBorderWidth
		    + pulldown_button->core.height
		    + RowColBorderWidth
		    + rowcol->core.height + 1);
			
	 if (y_temp > 0)
	     *y = y_temp;
      }
      else
	  *y -= (bottomEdgeOfMenu - dispHeight + 1);
   }
}

/*
 * Method for posting a multi-paned menushell
 */
static void PopupSharedMenupane (submenu)
register XmRowColumnWidget submenu;
{
   register XmMenuShellWidget popup = (XmMenuShellWidget) XtParent(submenu);
   XmRowColumnWidget parent_menu;
   Position x, y;
   Dimension height, width;
   XmCascadeButtonWidget cascadebtn;
   XmRowColumnCallbackStruct callback;
   int _index;
   register int i;
   Boolean popped_up = popup->shell.popped_up;
   XmRowColumnWidget old_rowcol = NULL;
   Cursor cursor;

   callback.reason = XmCR_MAP;
   callback.event  = NULL;
   callback.widget = NULL;
   callback.data   = NULL;
   callback.callbackstruct = NULL;

   /* Find out which child is trying to get posted */
   for (i = 0; i < popup->composite.num_children; i++)
   {
      if (popup->composite.children[i] == (Widget)submenu)
      {
	 _index = i;
	 break;
      }
   }

   /* Swap only if the submenu is not already child[0] */
   if (_index != 0)
   {
      /* Swap places in the children list */
      old_rowcol = (XmRowColumnWidget) popup->composite.children[0];
      RC_SetPoppingDown(old_rowcol, True);
      popup->composite.children[_index] = (Widget)old_rowcol;
      popup->composite.children[0] = (Widget)submenu;
      RC_SetPoppingDown(submenu, False);
      XUnmapWindow(XtDisplay(old_rowcol), XtWindow(old_rowcol));
   }

   if (submenu->core.being_destroyed)
       return;

   /*
    * Set us as the active tab group for our parent
    */
   _XmSetActiveTabGroup(popup->menu_shell.focus_data, (Widget)submenu);

   /*
    * get the shell widget in sync with the menu widget
    * We keep the menu at 0,0 and shell at the menu's position.
    * the menu's position is just used as a place to keep shell's loc
    */

   if (RC_WidgetHasMoved (submenu))     /* but make sure it didn't */
   {                                    /* just move to where it */
                                        /* is now.  Happens with */
                                        /* pulldowns a lot */
      x = XtX(submenu);
      y = XtY(submenu);
      
      ForceMenuPaneOnScreen(submenu,&x,&y);
      
      XtX (submenu) = XtY (submenu) = (-1 * XtBorderWidth(submenu));

      if (RC_WindowHasMoved(submenu))
      {
	 XMoveWindow (XtDisplay(submenu), XtWindow(submenu), 
		      XtX(submenu), XtY(submenu));
	 RC_SetWindowMoved (submenu, FALSE);
      }

      RC_SetWidgetMoved (submenu, FALSE);
   }
   else
   {
      x = XtX(popup);
      y = XtY(popup);
   }

   /* Always adjust to the size of the menupane */
   width = XtWidth(submenu);
   height = XtHeight(submenu);

   XtConfigureWidget(popup, x, y, width, height, popup->core.border_width);

   XMapWindow(XtDisplay(submenu), XtWindow(submenu));

   if (popped_up && (old_rowcol != submenu))
   {
      XmRowColumnCallbackStruct callback2;

      callback2.reason = XmCR_UNMAP;
      callback2.event  = NULL;
      callback2.widget = NULL;
      callback2.data   = NULL;
      callback2.callbackstruct = NULL;
      XtCallCallbackList ((Widget) old_rowcol, old_rowcol->row_column.unmap_callback, &callback2);
   }

   XtCallCallbackList ((Widget) submenu, submenu->row_column.map_callback, &callback);

   (*(((XmRowColumnClassRec *)XtClass(submenu))->row_column_class.
      menuProcedures)) (XmMENU_ARM, (Widget)submenu, NULL, NULL, NULL);

   if (popped_up)
       _XmFastExpose(submenu);

   cascadebtn = (XmCascadeButtonWidget) RC_CascadeBtn(submenu);
   parent_menu = (XmRowColumnWidget) XtParent (cascadebtn);

   /* Make sure the first item gets the focus */
   if (submenu->manager.traversal_on)
   {
      submenu->manager.active_child = NULL;
      
      if ((parent_menu->manager.active_child != (Widget)cascadebtn) && 
	  ((RC_Type(parent_menu) == XmMENU_POPUP) ||
	   (RC_Type(parent_menu) == XmMENU_PULLDOWN)))
      {
	 /* Move the focus to the child */
	 _XmGrabTheFocus((Widget)cascadebtn, NULL);
      }
   }

   /*  
    * we are the root of the chain and better do the exclusive grab
    */

   if (!popped_up)
   {
      _XmSetTransientFlag((Widget)popup, TRUE);
      PostMenuShell(popup, XtGrabNonexclusive, False);
      _XmFastExpose(submenu);

      if ((RC_Type(parent_menu) == XmMENU_BAR) &&
	  (RC_BeingArmed(parent_menu)))
      {
	 XFindContext(XtDisplay(popup), NULL, _XmMenuCursorContext,
		      (caddr_t*)&cursor);
	 XtGrabPointer(parent_menu, True,
		       Events, GrabModeAsync, GrabModeAsync, None,
		       cursor, CurrentTime);
	 RC_SetBeingArmed(parent_menu, False);
      }

      /* We always must move the focuse for the leaf menu pane */
      _XmMenuFocus(XtParent(submenu), XmMENU_MIDDLE, CurrentTime);
   }

   /*
    * now highlight the pulldown entry widget that
    * pulled down the menu
    */
   XmCascadeButtonHighlight ((Widget)cascadebtn, TRUE);

   if (popped_up)
   {
      if (old_rowcol->manager.traversal_on)
      {
	 XmGadget activeChild;
	 
	 activeChild = (XmGadget)old_rowcol->manager.active_child;
	 _XmClearFocusPath((Widget)old_rowcol);
	 
	 /*
	  * If the active child was a gadget, then we need to 
	  * handle the sending of the focus out ourselves.
	  */
	 if (activeChild && XmIsGadget(activeChild))
	 {
	    _XmDispatchGadgetInput((XmGadget)activeChild, NULL, 
				   XmFOCUS_OUT_EVENT);
	    activeChild->gadget.have_traversal = False;
	 }
      }
      XmCascadeButtonHighlight (RC_CascadeBtn(old_rowcol), False);
      old_rowcol->manager.active_child = NULL;
   }

   if (old_rowcol)
       XtSetKeyboardFocus(XtParent(submenu), submenu);
}                       

/* BEGIN OSF fix pir 2131 */
/* This is ripped off from Xt/Shell.c and modified to also deal with X and Y 
 * requests.
 */
/*
 * This is gross, I can't wait to see if the change happened so I will ask
 * the window manager to change my size and do the appropriate X work.
 * I will then tell the requester that he can.  Care must be taken because
 * it is possible that some time in the future the request will be
 * asynchronusly denied and the window reverted to it's old size/shape.
 */
 
static XtGeometryResult GeometryManager( wid, request, reply )
     Widget wid;
     XtWidgetGeometry *request;
     XtWidgetGeometry *reply;
{
  ShellWidget shell = (ShellWidget)(wid->core.parent);
  XtWidgetGeometry my_request;
  
  if(shell->shell.allow_shell_resize == FALSE && XtIsRealized(wid))
    return(XtGeometryNo);

  if(!XtIsRealized((Widget)shell)){
    if (request->request_mode & (CWX | CWY)) {
      return(XtGeometryNo);
    }
    *reply = *request;
    if(request->request_mode & CWWidth)
      wid->core.width = shell->core.width = request->width;
    if(request->request_mode & CWHeight) 
      wid->core.height = shell->core.height = request->height;
    if(request->request_mode & CWBorderWidth)
      wid->core.border_width = shell->core.border_width =
	request->border_width;
    return(XtGeometryYes);
  }

  /* %%% worry about XtCWQueryOnly */
  my_request.request_mode = 0;
  if (request->request_mode & CWX) {
    my_request.x = request->x;
    my_request.request_mode |= CWX;
  }
  if (request->request_mode & CWY) {
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
      == XtGeometryYes) {
    if (request->request_mode & CWX) {
      wid->core.x = request->x;
    }
    if (request->request_mode & CWY) {
      wid->core.y = request->y;
    }
    if (request->request_mode & CWWidth) {
      wid->core.width = request->width;
    }
    if (request->request_mode & CWHeight) {
      wid->core.height = request->height;
    }
    if (request->request_mode & CWBorderWidth) {
      wid->core.border_width = request->border_width;
      wid->core.x = wid->core.y = -request->border_width;
    }	
    return XtGeometryYes;
  } else return XtGeometryNo;
}	
/* END OSF fix pir 2131 */

/*
 * managed_set_changed
 */
static void managed_set_changed (w)
Widget w;
{
   register XmMenuShellWidget popup = (XmMenuShellWidget) w;
   XmRowColumnWidget parent_menu;
   Position x, y;
   Dimension height, width;
   XmCascadeButtonWidget cascadebtn;
   XmRowColumnCallbackStruct callback;
   register Widget child;
   register XmRowColumnWidget rowcol = 
       (XmRowColumnWidget)popup->composite.children[0];
   Cursor cursor;

   _XmButtonEventStatus.waiting_to_be_managed = FALSE;

   /* Don't handle multi-paned shells here */
   if (popup->composite.num_children > 1)
       return;

   callback.reason = XmCR_MAP;
   callback.event  = NULL;
   callback.widget = NULL;
   callback.data   = NULL;
   callback.callbackstruct = NULL;

   if (rowcol->core.being_destroyed)
       return;

   if (XtIsManaged (rowcol)) 
   {
      if  ((RC_Type(rowcol) == XmMENU_PULLDOWN) &&
	   (! RC_CascadeBtn(rowcol)))
      {
	 /*
	  * this pulldown does not have a complete path to
	  * a toplevel pane, so it can not be managed
	  */
	 _XmWarning(w, ManageMsg);
	 XtUnmanageChild (rowcol);
	 return;
      } 
      else  
         if (RC_Type(rowcol) == XmMENU_POPUP) 
         {
	    /* Verify MenuPost/WhichButton */
	    if ((_XmButtonEventStatus.time ==
		    XtLastTimestampProcessed(XtDisplay(rowcol))) &&
		 !_XmButtonEventStatus.verified)
	    {
	       XtUnmanageChild (rowcol);
	       return;
	    }
	    /* Remember the time in case this is just a BMenu Click */
	    lastManagedMenuTime = _XmButtonEventStatus.time;
	 }

      /* Set us as the active tab group for our parent */
      _XmSetActiveTabGroup(popup->menu_shell.focus_data, (Widget)rowcol);
	 
      /*
       * get the shell widget in sync with the menu widget
       * We keep the menu at 0,0 and shell at the menu's position.
       * the menu's position is just used as a place to keep shell's loc
       */
      if (RC_WidgetHasMoved (rowcol))   
      {                                 
	 x = XtX(rowcol);
	 y = XtY(rowcol);
	      
         ForceMenuPaneOnScreen(rowcol,&x,&y);
	 
	 XtX (rowcol) = XtY (rowcol) = (-1 * XtBorderWidth(rowcol));
	      
	 if (RC_WindowHasMoved(rowcol))
	 {
	    XMoveWindow (XtDisplay(rowcol), XtWindow(rowcol), 
			 XtX(rowcol), XtY(rowcol));
	    RC_SetWindowMoved (rowcol, FALSE);
	 }
	      
	 RC_SetWidgetMoved (rowcol, FALSE);
      }
      else
      {
	 x = XtX(popup);
	 y = XtY(popup);
      }
	 
      width = XtWidth(rowcol);
      height = XtHeight(rowcol);
	 
      XtConfigureWidget(popup, x, y, width, height,
			popup->core.border_width);
	 
      XtCallCallbackList ((Widget) rowcol, rowcol->row_column.map_callback, &callback);
      
      (*(((XmRowColumnClassRec *)XtClass(rowcol))->row_column_class.
	 menuProcedures)) (XmMENU_ARM, (Widget)rowcol, NULL, NULL, NULL);

      /*
       * now figure out exactly how to pop it up
       */
	 
      switch (RC_Type(rowcol))
      {
       case XmMENU_POPUP:
	 _XmSetTransientFlag((Widget)w, True);
	 PostMenuShell(w, XtGrabExclusive, True);
	 _XmFastExpose(rowcol);
	      
	 /* For popups, put pointer grab on root menu pane */
	 XFindContext(XtDisplay(w), NULL, _XmMenuCursorContext, 
		      (caddr_t*)&cursor);

	 _XmMenuFocus(XtParent(rowcol), XmMENU_BEGIN, CurrentTime);
	 XtGrabPointer(rowcol, True,
		       Events, GrabModeAsync, GrabModeAsync,
		       None, cursor, CurrentTime);
	 break;

       case XmMENU_PULLDOWN:
	 cascadebtn = (XmCascadeButtonWidget) RC_CascadeBtn(rowcol);
	 parent_menu = (XmRowColumnWidget) XtParent (cascadebtn);

	 /* Make sure the correct item gets the focus */
	 if (rowcol->manager.traversal_on)
	 {
	    if (RC_MemWidget(rowcol) && 
		(RC_Type(parent_menu) == XmMENU_OPTION))
	    {
	       if (XtParent(RC_MemWidget(rowcol)) == (Widget)rowcol)
		   rowcol->manager.active_child = RC_MemWidget(rowcol);
	       else
	       {
		  /* Find our link to the memory widget */
		  child = (Widget)RC_MemWidget(rowcol);
		  while (child)
		  {
		     if (XtParent(child) == (Widget)rowcol)
			 break;
		     
		     child = (Widget)RC_CascadeBtn(XtParent(child));
		  }
		  rowcol->manager.active_child =  child;
	       }
	    }
	    else
		rowcol->manager.active_child = NULL;
	    
	    if ((parent_menu->manager.active_child != (Widget)cascadebtn) &&
		((RC_Type(parent_menu) == XmMENU_POPUP) ||
		 (RC_Type(parent_menu) == XmMENU_PULLDOWN)))
	    {
	       /* Move the focus to the child */
	       _XmGrabTheFocus((Widget)cascadebtn, NULL);
	    }
	 }
	 
	 if (RC_Type(parent_menu) == XmMENU_OPTION)
	 {
	    /* Remember the time in case this is just a BSelect Click */
	    lastManagedMenuTime = _XmButtonEventStatus.time;

	    _XmSetTransientFlag((Widget)w, True);
	    PostMenuShell(w, XtGrabExclusive, True);
	    _XmFastExpose(rowcol);
	      
	    /* For popups, put pointer grab on root menu pane */
	    XFindContext(XtDisplay(w), NULL, _XmMenuCursorContext,
			 (caddr_t*)&cursor);

	    _XmMenuFocus(XtParent(rowcol), XmMENU_BEGIN, CurrentTime);
	    XtGrabPointer(rowcol, True,
			  Events, GrabModeAsync, GrabModeAsync,
			  None, cursor, CurrentTime);
	 }
	 else
	 {
	    _XmSetTransientFlag(w, TRUE);
	    PostMenuShell(w, XtGrabNonexclusive, False);
	    _XmFastExpose(rowcol);

	    /* For a menubar, put the pointer grab on the menubar. */
	    if ((RC_Type(parent_menu) == XmMENU_BAR) &&
		(RC_BeingArmed(parent_menu)))
	    {
	       XFindContext(XtDisplay(popup), NULL, 
			    _XmMenuCursorContext, (caddr_t*)&cursor);
	       XtGrabPointer(parent_menu, True, Events, GrabModeAsync,
			     GrabModeAsync, None, cursor, CurrentTime);
	       RC_SetBeingArmed(parent_menu, False);
	    }

	    /* We always must grab the keyboard for the leaf menu pane */
	    _XmMenuFocus(XtParent(rowcol), XmMENU_MIDDLE, CurrentTime);
	    XtSetKeyboardFocus(XtParent(rowcol), rowcol);
	    
	    /*
	     * now highlight the pulldown entry widget that
	     * pulled down the menu
	     */
	    XmCascadeButtonHighlight ((Widget)cascadebtn, TRUE);
	    break;
	 }
      }
   }
   else
   {
      /* popdown anything that is still up */
      _XmMenuFocus(w, XmMENU_END, CurrentTime);
      
      PopdownEveryone (popup, NULL);
      if (RC_Type(rowcol) == XmMENU_POPUP)
      {
	 _XmSetTransientFlag((Widget)w, False);
	 XtUngrabKeyboard(w, CurrentTime);
	 XtUngrabPointer(w, CurrentTime);

	 (*(((XmRowColumnClassRec *)XtClass(rowcol))->
	    row_column_class.menuProcedures))
	     (XmMENU_TRAVERSAL, (Widget)rowcol, False, NULL, NULL);
      }
   }
}                       


/*
 * actually popdown a menuShell widget, also flag as unmanaged
 * its menu child and unhighlight the cascadebutton that popped up this
 * widget
 */

static void Popdown (menushell, event)
XmMenuShellWidget menushell;
XEvent           *event;
{
    XmGadget activeChild;
    XmRowColumnCallbackStruct   callback;
    XmRowColumnWidget  rowcol = (XmRowColumnWidget) 
                                 menushell->composite.children[0];

    if (menushell->shell.popped_up)
    {
        /* mark the row column as popping down */
        RC_SetPoppingDown (rowcol, True);
	
        XtPopdown (menushell);

        /* Clean up traversal */
        if (rowcol->manager.traversal_on)
        {
            activeChild = (XmGadget)rowcol->manager.active_child;
            _XmClearFocusPath((Widget)rowcol);
            _XmSetFocusResetFlag((Widget)menushell, True);
            XtSetKeyboardFocus(menushell, rowcol);
            _XmSetFocusResetFlag((Widget)menushell, False);

            /*
             * If the active child was a gadget, then we need to handle the
             * sending of the focus out ourselves.
             */

            if (activeChild && XmIsGadget(activeChild))
            {
                _XmDispatchGadgetInput((XmGadget)activeChild, event, XmFOCUS_OUT_EVENT);
                activeChild->gadget.have_traversal = False;
            }
        }

        rowcol->manager.active_child = NULL;

        /* Don't unmanage if we're sharing the menu shell */
        if (menushell->composite.num_children == 1)
           rowcol->core.managed = FALSE;

        if ((RC_Type(rowcol) != XmMENU_POPUP) && RC_CascadeBtn(rowcol))
        {
           XmCascadeButtonHighlight (RC_CascadeBtn(rowcol), FALSE);
	   RC_PopupPosted (XtParent(RC_CascadeBtn(rowcol))) = FALSE;
        }
    
        callback.reason = XmCR_UNMAP;
        callback.event  = event;
	callback.widget = NULL;
	callback.data   = NULL;
	callback.callbackstruct = NULL;
        XtCallCallbackList ((Widget) rowcol, rowcol->row_column.unmap_callback, &callback);
    }
}


/*
 * Popdown all the popup kids of this widget, do bottom to top popdown.
 */
    
static void PopdownKids (menushell, event)
XmMenuShellWidget   menushell;
XEvent            * event;
{
   ShellWidget subShell;

   if (subShell = (ShellWidget)
       RC_PopupPosted(menushell->composite.children[0]))
   {
      /* mark this row colum as popping down */
      RC_SetPoppingDown (subShell->composite.children[0], True);
    
      PopdownKids (subShell, event);
      Popdown (subShell, event);
   }
}


/*
 * This routine determines if there is an enter event pending which will
 * just pop back up the row column that is about to be popped down.
 */
static int SkipPopdown (cascade)
XmCascadeButtonWidget cascade;
{
    XEvent  event;

    /*
     * check if an enter event is pending.  
     */
    if (XPending (XtDisplay (cascade)))
    {
        XPeekEvent (XtDisplay (cascade), &event);

        if (event.type == EnterNotify)
        {
            XEnterWindowEvent * enterevent = (XEnterWindowEvent *) &event;

            if (XtWindow (cascade) == enterevent->window)
                     return (TRUE);
        }
    }

    return (FALSE);
}


/* 
 * event handler for entering on a row column widget.  The widget must
 * be either a pulldown or popup menu.
 */
/* ARGSUSED */
#ifdef _NO_PROTO
void _XmEnterRowColumn (widget, closure, event)
Widget widget;
caddr_t closure;
XEvent *event;
#else /* _NO_PROTO */
void _XmEnterRowColumn (Widget widget, caddr_t closure, XEvent *event)
#endif /* _NO_PROTO */
{
    XmRowColumnWidget cascade;
    Widget cascadebtn;
    
    XmRowColumnWidget rowcol = (XmRowColumnWidget) widget;
    XmMenuShellWidget menushell = (XmMenuShellWidget) XtParent(rowcol);
    XEnterWindowEvent *enterevent = (XEnterWindowEvent *) event;

    /* 
     * Ignore popdown requests if traversal is on, or we're no longer
     * visible, or we don't have any submenus up.
     */
    if ((rowcol->manager.traversal_on)    ||
	(!menushell->shell.popped_up)     ||
	(! RC_PopupPosted(rowcol)))
	return;

    cascade = (XmRowColumnWidget)
	((CompositeWidget) RC_PopupPosted(rowcol))->composite.children[0];
    
    cascadebtn = RC_CascadeBtn (cascade);

    /*
     * Make sure that popdown should not be skipped.  It should be skipped
     * if the mouse has moved into the cascadebutton (widget or gadget)
     * that will just pop it back up again.
     */
    if ((XmIsCascadeButtonGadget(cascadebtn)) && 
             (cascadebtn == (Widget) _XmInputInGadget((CompositeWidget)rowcol, enterevent->x, 
						      enterevent->y)))
    {
       return;
    }

    else if ((XmIsCascadeButton(cascadebtn)) &&
	     ((enterevent->subwindow == XtWindow(cascadebtn)) || 
	      (SkipPopdown (cascadebtn))))
       {
          return;
    }

    _XmMenuFocus(XtParent(rowcol), XmMENU_MIDDLE, CurrentTime);
    PopdownKids (menushell, event);
}


static void PopdownOne (widget, event)
Widget widget;
XEvent *event;
{
    XmMenuShellWidget ms = (XmMenuShellWidget)widget;
    XmMenuShellWidget tms;
    XmRowColumnWidget rowcol;

    /* Find a menu shell, this may be a menushell, rowcol or button */
    while (ms && !XmIsMenuShell(ms))
         ms = (XmMenuShellWidget)XtParent(ms);

    if (ms == NULL)
        return;

    /* Find the toplevel MenuShell 
     */
    _XmGetActiveTopLevelMenu (ms->composite.children[0], &rowcol);
    tms = ms;
    if (RC_Type(rowcol) == XmMENU_BAR)
        tms = (XmMenuShellWidget) RC_PopupPosted(rowcol);
    else if (RC_Type(rowcol) == XmMENU_POPUP)
        tms = (XmMenuShellWidget) XtParent(rowcol);

    if (ms == tms)
       PopdownDone(widget, event);
    else
       PopdownEveryone(ms, event);

    /* 2nd part to kludge that fixes F10 / popdown core dump so that
     * MenuFocusIn is not called to leave the cascade button highlighted 
     */
    if (RC_Type(rowcol) == XmMENU_BAR)
       (void)XmProcessTraversal((Widget)rowcol, XmTRAVERSE_CURRENT);

    if (event)
       _XmRecordEvent(event);
}
    

/*
 * Class function used to unpost all menupanes
 */

/*
 * popdown this all the popup children, and then me, ie. bottom to top
 *
 * called from managed_set_changed and PopdownDone
 */

static void PopdownEveryone (widget, event)
Widget widget;
XEvent *event;
{
    Widget rowcol;
    XmMenuShellWidget shell = (XmMenuShellWidget) widget;

    /* Ignore this event, if already processed */
    if ( event && !_XmIsEventUnique(event))
       return;
    /* 
     * If only a portion of the hierarchy is being popped down, then
     * make sure the keyboard grab gets reset to the right window.
     */
    rowcol = shell->composite.children[0];

    if ((RC_Type(rowcol) != XmMENU_POPUP) && RC_CascadeBtn(rowcol) &&
        XmIsMenuShell(XtParent(XtParent(RC_CascadeBtn(rowcol)))))
    {
       _XmMenuFocus(XtParent(XtParent(RC_CascadeBtn(rowcol))),
		    XmMENU_MIDDLE, CurrentTime);
    }

    /* mark this row colum as popping down */
    RC_SetPoppingDown (rowcol, True);
    
    if (shell->shell.popped_up) 
    {
       _XmFastPopdown(widget);
       XFlush(XtDisplay(widget));
       PopdownKids (widget, event);           /* do our kids */
       Popdown (widget ,event);                /* do us */
    }
}


/*
 * Class function used to unpost all menupanes
 */

static void PopdownDone (widget, event)
Widget widget;
XButtonEvent *event;
{
    XmMenuShellWidget ms = (XmMenuShellWidget)widget;
    XmRowColumnWidget  rowcol;

    /* Ignore this event, if already processed */
    if (event && !_XmIsEventUnique((XEvent*)event))
       return;

    /* Find a menu shell, this may be a menushell, rowcol or button */
    while (ms && !XmIsMenuShell(ms))
         ms = (XmMenuShellWidget)XtParent(ms);

    if (ms == NULL)
	return;
    _XmGetActiveTopLevelMenu (ms->composite.children[0], &rowcol);

    if (event && ((event->type==ButtonPress) || (event->type==ButtonRelease)))
    {
        if ( !_XmMatchBtnEvent( event, XmIGNORE_EVENTTYPE,
              RC_PostButton(rowcol), RC_PostModifiers(rowcol)) )
           return;
    }

    /* This is to enable Option and Popup Menus to post on BMenu/BSelect Click.
     * By this time we know that the button is valid.  We need to check
     * if the post event occured within a short time span.  Next we force 
     * the menu into traversal mode.  And, of course, we return immediately
     * without popping down the pane.
     */
     if (event && ((event->time - lastManagedMenuTime) < 
             XtGetMultiClickTime(XtDisplay(ms))))	/* or 150 ms? */
     {
       /* Also check rowcol->manager.highlighted_widget - if it's set, it means
	* the user has quickly dragged and selected an entry during 'click' 
	* time.  This is an edge case.  If the guy has dragged to a selection 
	* (highlighted), take it as an activation and don't go into traversal 
	* mode.
	*/
	if ( ((RC_Type(rowcol) == XmMENU_OPTION) && 
		 !(((XmManagerWidget)RC_OptionSubMenu(rowcol))->
		    manager.highlighted_widget)) ||
	     ((RC_Type(rowcol) != XmMENU_OPTION) &&
		 !rowcol->manager.highlighted_widget) ) 
	{
	   /* Force traversal on */
	   (*(((XmRowColumnClassRec *)XtClass(rowcol))->
	      row_column_class.menuProcedures))
	       (XmMENU_TRAVERSAL, (Widget)rowcol, True, NULL, NULL);
	   return;
	}
     }


    /* if in a menubar or popup, get the toplevel menushell */
    if (RC_Type(rowcol) == XmMENU_BAR)
	ms = (XmMenuShellWidget) RC_PopupPosted(rowcol);

    else if (RC_Type(rowcol) == XmMENU_POPUP)
	ms = (XmMenuShellWidget) XtParent(rowcol);

    _XmMenuFocus((Widget)ms, XmMENU_END, CurrentTime);
    
    PopdownEveryone (ms, event);

    /* if its a menubar, clean it up and disarm it */
    if (RC_Type(rowcol) == XmMENU_BAR)
    {
        (*(((XmRowColumnClassRec *)XtClass(rowcol))->row_column_class.
            menuProcedures)) (XmMENU_BAR_CLEANUP, (Widget)rowcol, NULL, NULL, NULL);

	(*(((XmRowColumnClassRec *)XtClass(rowcol))->row_column_class.
	   menuProcedures)) (XmMENU_DISARM, (Widget)rowcol, NULL, NULL, NULL); 
    }

    /* Cleanup grabs */
    _XmSetTransientFlag((Widget)ms, False);
    XtUngrabKeyboard(ms, CurrentTime);
    XtUngrabPointer(ms, CurrentTime);

    (*(((XmRowColumnClassRec *)XtClass(rowcol))->
       row_column_class.menuProcedures))
	(XmMENU_TRAVERSAL, (Widget)rowcol, False, NULL, NULL);
}


/*
 * Clear traversal in the associated menu hierarchy
 */

static void ClearTraversal (widget, event)
XmMenuShellWidget widget;
XButtonEvent *event;
{
   Widget parent = NULL;
   Widget eventWidget;
   Widget rowcol;
   
   if (!_XmIsEventUnique((XEvent*)event))
      return;

   /* Get our row column child */
   if (widget->composite.num_children == 0)
      return;

   rowcol = widget->composite.children[0];
   if ( !_XmMatchBtnEvent( event, RC_PostEventType(rowcol),
        RC_PostButton(rowcol), RC_PostModifiers(rowcol)) )
      return;

   if (((XmManagerWidget)rowcol)->manager.traversal_on)
   {
      /* Let rowcolumn class clear the traversal */
      (*(((XmRowColumnClassRec *)XtClass(rowcol))->
	 row_column_class.menuProcedures))
	  (XmMENU_TRAVERSAL, rowcol, False, NULL, NULL);
   }

   /* Unpost any unneeded submenus */
   if ((eventWidget = XtWindowToWidget(event->display, event->window)) == NULL)
      return;

   /*
    * If the event occurred in a menupane, or a child of a menupane, then
    * since they did not process the event and mark it as being used, we
    * must assume the intended widget was insensitive or something.  So,
    * we will pop down all panes hanging off of this one.
    */
   if ((XmIsRowColumn(eventWidget) &&
	(RC_Type(eventWidget) == XmMENU_POPUP ||
	 RC_Type(eventWidget) == XmMENU_PULLDOWN)) ||
       ((parent = XtParent(eventWidget)) && XmIsRowColumn(parent) &&
	(RC_Type(parent) == XmMENU_POPUP ||
	 RC_Type(parent) == XmMENU_PULLDOWN)))
   {
      if (parent)
         _XmButtonPopdownChildren((XmRowColumnWidget)parent);
      else
         _XmButtonPopdownChildren((XmRowColumnWidget)eventWidget);
   }
}


/*
 * Public Routine
 */

#ifdef _NO_PROTO
Widget XmCreateMenuShell (parent, name, al, ac)
Widget  parent;
char *  name;
ArgList al;
Cardinal ac;
#else /* _NO_PROTO */
Widget XmCreateMenuShell (Widget parent, char *name, ArgList al, Cardinal ac)
#endif /* _NO_PROTO */
{
   return (XtCreatePopupShell (name, xmMenuShellWidgetClass, parent, al, ac));
}

static void Destroy(ms)
    XmMenuShellWidget	ms;
{
    _XmDestroyFocusData(ms->menu_shell.focus_data);
}
