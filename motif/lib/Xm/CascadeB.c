#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)CascadeB.c	3.28.1.1 91/02/21";
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

#include <X11/keysymdef.h>
#include <Xm/XmP.h>
#include <X11/ShellP.h>
#include <Xm/MenuShellP.h>
#include <Xm/RowColumnP.h>
#include <Xm/LabelP.h>
#include <Xm/CascadeBP.h>
#include <Xm/LabelGP.h>
#include <Xm/CascadeBGP.h>
#include <Xm/CacheP.h>

#define CASCADE_PIX_SPACE    4	 /* pixels between label and bit map */
#define MAP_DELAY_DEFAULT   180
#define EVENTS              ButtonPressMask | ButtonReleaseMask | \
                            EnterWindowMask | LeaveWindowMask


#define WRONGPARENT "XmCascadeButton must have correct type of \
                     xmRowColumnWidgetClass parent."
#define WRONGSUBMENU "Only XmMENU_PULLDOWN XmRowColumnWidgets can be submenus."
#define WRONGMAPDELAY "MapDelay must be >= 0."

extern XContext _XmMenuCursorContext;

/*
 * forward declarations
 */

static Boolean SetValues ();
static void ClassInitialize();
static void ClassPartInitialize();
static void Initialize ();
static void Destroy ();
static void Resize ();
static void Redisplay ();
static void BorderHighlight ();
static void BorderUnhighlight ();
static void ArmAndActivate ();
static void Arm ();
static void Select ();
static void Disarm ();
static void Cascading ();
static void Popup ();

/* 
 * action procs 
 */
static void DelayedArm ();
static void CheckDisarm ();
static void StartDrag ();
static void DoSelect ();
static void KeySelect ();
static void MenuBarSelect ();
static void MenuBarEnter ();
static void MenuBarLeave ();
static void CleanupMenuBar ();
static void Help  ();

static int _XmArrowPixmapCacheCompare();

/*
 * event translation tables for cascadebutton.  There are different
 * ones for the different menus a cascadebutton widget can appear in
 */

static XtTranslations menubar_events_parsed;

#ifndef MCCABE
static char menubar_events [] = 
    "<BtnDown>:			MenuBarSelect()\n\
     <EnterWindow>Normal:	MenuBarEnter()\n\
     <LeaveWindow>Normal:	MenuBarLeave()\n\
     <BtnUp>:			DoSelect()\n\
     <Key>osfSelect:		KeySelect()\n\
     <Key>osfActivate:	        KeySelect()\n\
     <Key>osfHelp:		Help()\n\
     <Key>osfCancel:		CleanupMenuBar()\n\
     ~Shift ~Meta ~Alt <Key>Return:	KeySelect()\n\
     ~Shift ~Meta ~Alt <Key>space:	KeySelect()";
#else
static char menubar_events [];
#endif

static XtTranslations p_events_parsed;

#ifndef MCCABE
static char p_events [] = 
    "<BtnDown>:			StartDrag()\n\
     <EnterWindow>:		DelayedArm()\n\
     <LeaveWindow>:		CheckDisarm()\n\
     <BtnUp>:			DoSelect()\n\
     <Key>osfSelect:		KeySelect()\n\
     <Key>osfActivate:	        KeySelect()\n\
     <Key>osfHelp:		Help()\n\
     <Key>osfCancel:		CleanupMenuBar()\n\
     ~Shift ~Meta ~Alt<Key>Return:	KeySelect()\n\
     ~Shift ~Meta ~Alt<Key>space:	KeySelect()";
#else
static char p_events [];
#endif
  
static XtActionsRec action_table [] =
{
    {"DelayedArm",	(XtActionProc) DelayedArm},
    {"CheckDisarm",	(XtActionProc) CheckDisarm},
    {"StartDrag",	(XtActionProc) StartDrag},
    {"DoSelect", 	(XtActionProc) DoSelect},
    {"KeySelect",	(XtActionProc) KeySelect},
    {"MenuBarSelect",	(XtActionProc) MenuBarSelect},
    {"MenuBarEnter",	(XtActionProc) MenuBarEnter},
    {"MenuBarLeave",	(XtActionProc) MenuBarLeave},
    {"CleanupMenuBar",	(XtActionProc) CleanupMenuBar},
    {"Help",		(XtActionProc) Help},
    { NULL, NULL}
};


static XtResource resources[] = 
{
    {	XmNactivateCallback, 
	XmCCallback, 
	XmRCallback,
	sizeof (XtCallbackList),
	XtOffset (XmCascadeButtonWidget, cascade_button.activate_callback), 
	XmRCallback,
	NULL
    },
    {	XmNcascadingCallback, 
	XmCCallback, 
	XmRCallback,
	sizeof (XtCallbackList),
	XtOffset (XmCascadeButtonWidget, cascade_button.cascade_callback), 
	XmRCallback,
	NULL
    },
    {	XmNsubMenuId, 
	XmCMenuWidget,				/* submenu */
	XmRMenuWidget, 
	sizeof (Widget),
	XtOffset (XmCascadeButtonWidget, cascade_button.submenu), 
	XmRMenuWidget, 
	(caddr_t) NULL
    },
    {	XmNcascadePixmap, 
	XmCPixmap, 
	XmRPrimForegroundPixmap,
	sizeof(Pixmap),
	XtOffset (XmCascadeButtonWidget, cascade_button.cascade_pixmap), 
	XmRImmediate,
	(caddr_t) XmUNSPECIFIED_PIXMAP
    },
    {   XmNmappingDelay,
	XmCMappingDelay,
	XmRInt,
	sizeof (int),
	XtOffset (XmCascadeButtonWidget, cascade_button.map_delay),
	XmRImmediate,
	(caddr_t) MAP_DELAY_DEFAULT
    },
    {
        XmNshadowThickness,
        XmCShadowThickness,
        XmRHorizontalDimension,
        sizeof (Dimension),
        XtOffset (XmCascadeButtonWidget, primitive.shadow_thickness),
        XmRImmediate,
        (caddr_t) 2
    },
    {
	XmNtraversalOn,
	XmCTraversalOn,
	XmRBoolean,
	sizeof(Boolean),
	XtOffset(XmPrimitiveWidget, primitive.traversal_on),
	XmRImmediate,
	(caddr_t) True
    },
    {
        XmNhighlightThickness,
        XmCHighlightThickness,
        XmRHorizontalDimension,
        sizeof (Dimension),
        XtOffset (XmPrimitiveWidget, primitive.highlight_thickness),
        XmRImmediate,
        (caddr_t) 2
    },
};       

/*
 * static initialization of the cascade button widget class record, 
 * must do each field
 */

externaldef(xmcascadebuttonclassrec) XmCascadeButtonClassRec xmCascadeButtonClassRec = 
{
    {			/* core class record */
	(WidgetClass) &xmLabelClassRec,		/* superclass ptr	*/
	"XmCascadeButton",			/* class_name	*/
	sizeof (XmCascadeButtonWidgetRec),	/* size of Pulldown widget */
	ClassInitialize,			/* class init proc */
	ClassPartInitialize,			/* chained class init */
	FALSE,					/* class is not init'ed */
	Initialize,				/* widget init proc */
	NULL,					/* init_hook proc */
    	_XtInherit,				/* widget realize proc */
    	action_table,				/* class action table */
    	XtNumber (action_table),
	resources,				/* this class's resource list*/
	XtNumber (resources),			/*  "	  " resource_count */
    	NULLQUARK,				/* xrm_class	        */
    	TRUE,					/* compress motion */
    	XtExposeCompressMaximal,		/* compress exposure */
	TRUE,					/* compress enter-leave */
   	FALSE,					/* no VisibilityNotify */
	Destroy,				/* class destroy proc */
	Resize,					/* class resize proc */
	Redisplay,				/* expose proc */
	SetValues,				/* set_value proc */
	NULL,					/* set_value_hook proc */
	XtInheritSetValuesAlmost,		/* set_value_almost proc */
	NULL,					/* get_values_hook */
	NULL,					/* class accept focus proc */
	XtVersion,				/* current version */
    	NULL,					/* callback offset list */
    	NULL,					/* default translation table */
						  /* this is manually set */
	XtInheritQueryGeometry,			/* query geo proc */
	NULL,				        /* display accelerator*/
	NULL,					/* extension */
    },
	{
			/* Primitive Class record */
	BorderHighlight,			/* border_highlight */
	BorderUnhighlight,			/* border_uhighlight */
	XtInheritTranslations,		        /* translations */
	ArmAndActivate,	                        /* arm & activate */
	NULL,					/* get resources */
	0,					/* num get_resources */
	NULL					/* extension */
	},
    {			/* Label Class record */
	_XtInherit,		                /* set override callback */
	(XmMenuProc)_XtInherit,			/* menu procedures       */
	XtInheritTranslations,		        /* menu traversal xlation */
	NULL,					/* extension */
    },
    {			/* cascade_button class record */
        NULL,					/* extension */  
    }
};

/*
 * now make a public symbol that points to this class record
 */

externaldef(xmcascadebuttonwidgetclass) WidgetClass xmCascadeButtonWidgetClass = (WidgetClass) &xmCascadeButtonClassRec;

/*
 * use caching to share arrow data
 */
static XmCacheClassPart	ArrowPixmapCache = {
   {NULL, 0, 0},		/* head of class cache list */
   _XmCacheCopy,		/* Copy routine     */
   (XmCacheDeleteProc) _XmArrowPixmapCacheDelete,	/* Delete routine   */
   _XmArrowPixmapCacheCompare,	/* Comparison routine   */
};


/*
 * parse the translation tables for the different menutypes
 */
static void ClassInitialize()
{
    menubar_events_parsed  = XtParseTranslationTable (menubar_events);
    p_events_parsed	   = XtParseTranslationTable (p_events);
}



/*
 * set up fast subclassing
 */
static void ClassPartInitialize(wc)
WidgetClass wc;
{
    _XmFastSubclassInit (wc, XmCASCADE_BUTTON_BIT);
}


/* 
 * _XmArrowPixmapCacheCompare()
 */
static int _XmArrowPixmapCacheCompare(arrowpix_rec, arrowpix_cache_rec)
XmArrowPixmap *arrowpix_rec, *arrowpix_cache_rec;
{
   if ((arrowpix_rec->height == arrowpix_cache_rec->height) &&
       (arrowpix_rec->width == arrowpix_cache_rec->width) &&
       (arrowpix_rec->depth == arrowpix_cache_rec->depth) &&
       (arrowpix_rec->top_shadow_color == 
          arrowpix_cache_rec->top_shadow_color) &&
       (arrowpix_rec->bottom_shadow_color == 
	  arrowpix_cache_rec->bottom_shadow_color) &&
       (arrowpix_rec->foreground_color == 
	  arrowpix_cache_rec->foreground_color))
      return 1;
   else
      return 0;
}

/* 
 * _XmArrowPixmapCacheCompare()
 */
#ifdef _NO_PROTO
void _XmArrowPixmapCacheDelete( pixmap )
Pixmap pixmap;
#else /* _NO_PROTO */
void _XmArrowPixmapCacheDelete(Pixmap  pixmap )
#endif /* _NO_PROTO */
{
   XmGadgetCachePtr ptr;
   XmArrowPixmap *arrowpix_rec;

   ptr =  (XmGadgetCachePtr)(ClassCacheHead(&ArrowPixmapCache)).next;
   while (ptr)
   {
     arrowpix_rec = (XmArrowPixmap *)(CacheDataPtr(ptr));
     if (pixmap == arrowpix_rec->pixmap)
     {
	if (--ptr->ref_count <= 0) 
	{
	  (ptr->prev)->next = ptr->next;
	  if (ptr->next)                    /* not the last record */
	    (ptr->next)->prev = ptr->prev;
	  XFreePixmap(arrowpix_rec->display, arrowpix_rec->pixmap);
	  XtFree( ptr );
	}
	return;
     }
     else
        ptr = (XmGadgetCachePtr)ptr->next;
   } 
}


/*
 * The button is armed (does not pop up submenus).
 */
static void BorderHighlight (cb)
XmCascadeButtonWidget cb;
{
      Arm (cb);
}


/*
 * The button is disarmed (does not pop down submenus).
 */
static void BorderUnhighlight (cb)
XmCascadeButtonWidget cb;
{
      Disarm (cb, False);
}


/*
 * Draw the 3D shadow around the widget if its is armed.
 */
static void DrawShadow (cb)
register XmCascadeButtonWidget cb;
{
   if (cb->primitive.shadow_thickness > 0 &&
      (cb->cascade_button.armed))
   {
      if (XtIsRealized(cb))
      {
	 _XmDrawShadow (XtDisplay (cb), XtWindow (cb),
			cb->primitive.top_shadow_GC,
			cb->primitive.bottom_shadow_GC,
			cb->primitive.shadow_thickness,
			cb->primitive.highlight_thickness,
			cb->primitive.highlight_thickness,
			cb->core.width - 2 * 
			cb->primitive.highlight_thickness,
			cb->core.height - 2 * 
			cb->primitive.highlight_thickness);
      }
   }
}


static void DrawCascade(cb)
register XmCascadeButtonWidget cb;
{
   if ((CB_HasCascade(cb)) && (CB_Cascade_width(cb) != 0))
   {
      /* draw the casacade */
      XCopyArea (XtDisplay(cb), 
	 cb->cascade_button.armed && 
	    (CB_ArmedPixmap(cb) != XmUNSPECIFIED_PIXMAP) ? 
	    CB_ArmedPixmap(cb) : CB_CascadePixmap(cb), 
	 XtWindow(cb),
	 cb->label.normal_GC, 0, 0, 
	 CB_Cascade_width(cb), CB_Cascade_height(cb),
	 CB_Cascade_x(cb), CB_Cascade_y(cb));
   }
}

/*
 * redisplay the widget
 */
static void Redisplay (cb)
register XmCascadeButtonWidget cb;
{
    if (XtIsRealized (cb)) 
    {
	/* Label class does most of the work */
	(* xmLabelClassRec.core_class.expose)(cb, NULL, NULL);
       
	DrawCascade(cb);
	DrawShadow (cb);
    }
}


/*
 * Arming the cascadebutton consists of setting the armed bit
 * and drawing the 3D shadow.
 */
static void Arm (cb)
register XmCascadeButtonWidget cb;
{
   if (!cb->cascade_button.armed)
   {
      cb->cascade_button.armed = TRUE;
      DrawCascade(cb);
      DrawShadow (cb);
   }
   if (Lab_MenuType (cb) != XmMENU_BAR)
      XmProcessTraversal((Widget)cb, XmTRAVERSE_CURRENT);
}



/*
 * Post the submenu and then arm the button.  The arming is done
 * second so that the post can be quickly as possible.
 */
static void ArmAndPost (cb, event)
XmCascadeButtonWidget cb;
XEvent *event;
{
   if (!cb->cascade_button.armed)
   {
      _XmCascadingPopup ((Widget)cb, event, TRUE);
      Arm (cb);
   }
}




/*
 * class function to cause the cascade button to be armed and selected
 */
static void ArmAndActivate (cb, event)
XmCascadeButtonWidget cb;
XEvent * event;
{
    XmRowColumnWidget parent = (XmRowColumnWidget) XtParent(cb);

    switch (Lab_MenuType (cb))
    {
       case XmMENU_BAR:
       {
          ShellWidget myShell = NULL;

          /* Shared menupanes require some additional checks */
          if (CB_Submenu(cb))
              myShell = (ShellWidget)XtParent(CB_Submenu(cb));

          if (myShell && (myShell->shell.popped_up) &&
              (myShell->composite.children[0] == CB_Submenu(cb)))
          {
	     (* xmLabelClassRec.label_class.menuProcs)
		 (XmMENU_POPDOWN, (Widget)parent, NULL, (caddr_t)event, NULL);

	     Disarm (cb, FALSE);
          }
          else 
          {
             /* call the cascading callbacks first thing */
             Cascading (cb, event);

             if (! RC_IsArmed (parent))
	     {
		_XmMenuFocus((Widget)parent, XmMENU_BEGIN, CurrentTime);

		if (CB_Submenu (cb))
		    (* xmLabelClassRec.label_class.menuProcs)
			(XmMENU_ARM, (Widget)parent, NULL, NULL, NULL);
	     }

             else
		 (* xmLabelClassRec.label_class.menuProcs)
		     (XmMENU_BAR_CLEANUP, (Widget)parent, NULL, NULL, NULL);

             /* do the select without calling the cascading callbacks again */
             Select (cb, event, FALSE);

             if (CB_Submenu(cb))
             {
		(* xmLabelClassRec.label_class.menuProcs) (XmMENU_TRAVERSAL,
							   (Widget)parent, TRUE,
							   NULL, NULL);
             }
             else
             {
		(* xmLabelClassRec.label_class.menuProcs) (XmMENU_TRAVERSAL,
							   (Widget)parent, FALSE,
							   NULL, NULL);

		(* xmLabelClassRec.label_class.menuProcs) (XmMENU_DISARM,
							   (Widget)parent, NULL,
							   NULL, NULL);

                _XmSetTransientFlag((Widget)cb, False);
		_XmMenuFocus(XtParent(cb), XmMENU_END, CurrentTime);
                XtUngrabPointer((Widget) cb, CurrentTime);
             }
	     /*
 	      * The previous call to Select() may set the active_child.  So
 	      * we'll just force it in case there is no submenu (PM mode).
 	      */
 	     parent->manager.active_child = 
	       CB_Submenu(cb) ? NULL : (Widget)cb;
          }
	  
          break;
       }

       case XmMENU_PULLDOWN:
       case XmMENU_POPUP:
       {
          Select (cb, event, TRUE);
	  if (CB_Submenu(cb))
	  {
	     (* xmLabelClassRec.label_class.menuProcs) (XmMENU_TRAVERSAL,
							(Widget)parent, TRUE,
							NULL, NULL);
	  }
          break;
       }
    }
}


/*
 * disarm the menu.  This may include popping down any submenu that is up or
 * removing the timeout to post a submenu
 */
static void Disarm (cb, unpost)
register XmCascadeButtonWidget cb;
Boolean unpost;
{
   if (cb->cascade_button.armed)
   {
      cb->cascade_button.armed = FALSE;

      /* popdown any posted submenus */
      if (unpost)
         _XmButtonPopdownChildren ((XmRowColumnWidget)XtParent(cb));

      /* if a delayed arm is pending, remove it */
      if (cb->cascade_button.timer)
      {
         XtRemoveTimeOut (cb->cascade_button.timer);
         cb->cascade_button.timer = 0;
      }

      /* if the shadow is drawn and the menupane is not going down, erase it */
      if ((cb->primitive.shadow_thickness != 0) &&
	  (! RC_PoppingDown(XtParent(cb))))
      {
	 if (XtIsRealized (cb))
	 {
	    _XmEraseShadow (XtDisplay (cb), XtWindow (cb),
			    cb->primitive.shadow_thickness,
			    cb->primitive.highlight_thickness,
			    cb->primitive.highlight_thickness,
			    cb->core.width - 2 * 
			     cb->primitive.highlight_thickness,
			    cb->core.height - 2 * 
 			     cb->primitive.highlight_thickness);
	 }
      }
      DrawCascade(cb);
   }
}


/*
 * called when the post delay timeout occurs.
 */
static XtTimerCallbackProc PostTimeout (cb)
XmCascadeButtonWidget cb;
{
   if (cb->cascade_button.timer)
   {
      cb->cascade_button.timer = 0;
    
      _XmCascadingPopup ((Widget)cb, NULL, TRUE);
   }
}


/*
 * set the timer to post the submenu if a leave event does
 * not occur first.
 */
/*ARGSUSED*/
static void DelayedArm (cb, event, param, num_param)
XmCascadeButtonWidget cb;
XEvent	*event;
char   **param;
int     *num_param;
{
   if ((! cb->cascade_button.armed) &&
       (((XmMenuShellWidget) XtParent(XtParent(cb)))->shell.popped_up) &&
       (! ((XmRowColumnWidget) XtParent(cb))->manager.traversal_on))

   {
      if (cb->cascade_button.map_delay <= 0)
	 ArmAndPost (cb, event);
  
      else
      {
         cb->cascade_button.timer = 
               XtAppAddTimeOut(XtWidgetToApplicationContext(cb), 
			       (unsigned long) cb->cascade_button.map_delay,
			       (XtTimerCallbackProc) PostTimeout, (caddr_t)cb);
         Arm(cb);
      }
   }
}


/*
 * if traversal is not on and the mouse
 * has not entered its cascading submenu, disarm the
 * cascadebutton.
 */
/*ARGSUSED*/
static void CheckDisarm (cb, event, param, num_param)
XmCascadeButtonWidget cb;
XEvent	*event;
char   **param;
int     *num_param;
{
   register XmMenuShellWidget submenushell;
   XEnterWindowEvent * entEvent = (XEnterWindowEvent *) event;

   if (! ((XmRowColumnWidget) XtParent(cb))->manager.traversal_on)
   {
      if ((cb->cascade_button.armed) && 
          (CB_Submenu(cb)))
      {
         submenushell = (XmMenuShellWidget) XtParent (CB_Submenu(cb));
   
         if (submenushell->shell.popped_up)
         {
            if ((entEvent->x_root >= submenushell->core.x) &&
                (entEvent->x_root <  submenushell->core.x + 
                                     submenushell->core.width +
                                     (submenushell->core.border_width << 1)) &&
                (entEvent->y_root >= submenushell->core.y) &&
                (entEvent->y_root <  submenushell->core.y + 
                                     submenushell->core.height +
	   			     (submenushell->core.border_width << 1)))

  	        /* then we are in the cascading submenu, don't disarm */
 	        return;
         }
      }
      Disarm (cb, TRUE);
   }
}


/*
 * post submenu and disable menu's traversal.  The order of these 
 * function calls is critical.
 */
/*ARGSUSED*/
static void StartDrag (cb, event, param, num_param)
XmCascadeButtonWidget cb;
XEvent	*event;
char   **param;
int     *num_param;
{
   Boolean validButton;

   /*
    * make sure the shell is popped up, this takes care of a corner case
    * that can occur with rapid pressing of the mouse button
    */
   if (((Lab_MenuType(cb) == XmMENU_PULLDOWN) ||
	(Lab_MenuType(cb) == XmMENU_POPUP)) &&
       (!((XmMenuShellWidget) XtParent(XtParent(cb)))->shell.popped_up))
   {
      return;
   }

   (* xmLabelClassRec.label_class.menuProcs) (XmMENU_BUTTON,
					      XtParent(cb), NULL, (caddr_t)event,
					      &validButton);
   
   if (validButton)
   {
      _XmCascadingPopup ((Widget)cb, event, TRUE);
      Arm (cb);

      (* xmLabelClassRec.label_class.menuProcs) (XmMENU_TRAVERSAL,
						 XtParent(cb), FALSE,
						 NULL, NULL);

      /* record event so MenuShell does not process it */
      _XmRecordEvent (event);
   }
}


/*
 * do the popup (either w/ or w/o the cascade callbacks).
 * If there is not a submenu, bring down the menu system.
 */
static void Select (cb, event, doCascade)
register XmCascadeButtonWidget cb;
XEvent * event;
Boolean  doCascade;
{
   XmAnyCallbackStruct cback;

   _XmCascadingPopup ((Widget)cb, event, doCascade);

   /*
    * check if there is a submenu here in case this changed during 
    * the cascading callbacks
    */
   if (CB_Submenu(cb) == NULL)
   {
      (* xmLabelClassRec.label_class.menuProcs)
	  (XmMENU_POPDOWN, XtParent(cb), NULL, (caddr_t)event, NULL);

      Disarm (cb, FALSE);

      cback.event = event;
      cback.reason = XmCR_ACTIVATE;

      if (XmIsRowColumn(XtParent(cb)))
      {
	 (* xmLabelClassRec.label_class.menuProcs) (XmMENU_CALLBACK,
						    XtParent(cb), FALSE,(caddr_t) cb,
						    (caddr_t)&cback);
      }

      if ((! cb->label.skipCallback) &&
	  (cb->cascade_button.activate_callback))
      {
         XtCallCallbackList ((Widget) cb, cb->cascade_button.activate_callback, &cback);
      }
   }
   else 
   {
      Arm(cb);
   }
}

/*
 * if there is a submenu, enable traversal.
 * call select to do the work
 */
/*ARGSUSED*/
static void DoSelect (cb, event, param, num_param)
register XmCascadeButtonWidget cb;
XEvent	*event;
char   **param;
int     *num_param;
{
   Boolean validButton;

   if (cb->cascade_button.armed == False)
      return;

   if ((Lab_MenuType(cb) == XmMENU_BAR) && !RC_IsArmed (XtParent(cb)))
      return;

   /*
    * make sure the shell is popped up, this takes care of a corner case
    * that can occur with rapid pressing of the mouse button
    */
   if (((Lab_MenuType(cb) == XmMENU_PULLDOWN) ||
	(Lab_MenuType(cb) == XmMENU_POPUP)) &&
       (!((XmMenuShellWidget) XtParent(XtParent(cb)))->shell.popped_up))
   {
      return;
   }

   (* xmLabelClassRec.label_class.menuProcs) (XmMENU_BUTTON,
					      XtParent(cb), NULL,
					      (caddr_t)event, &validButton);
   
   if (validButton)
   {
      Select (cb, event, (Boolean) CB_Submenu(cb) != 0);

      /* don't let the menu shell widget process this event */
      _XmRecordEvent (event);

      if (CB_Submenu(cb))
	  (* xmLabelClassRec.label_class.menuProcs) (XmMENU_TRAVERSAL,
						     XtParent(cb), TRUE,
						     NULL, NULL);
      else
      {
	 (* xmLabelClassRec.label_class.menuProcs) (XmMENU_DISARM,
						    XtParent(cb), NULL,
						    NULL, NULL);

         if (Lab_MenuType(cb) == XmMENU_BAR)
         {
            _XmSetTransientFlag((Widget)cb, False);
	    _XmMenuFocus(XtParent(cb), XmMENU_END, CurrentTime);
            XtUngrabPointer(cb, CurrentTime);
         }
      }
   }
}

/*
 * if the menu system traversal is enabled, do a select
 */
/*ARGSUSED*/
static void KeySelect (cb, event, param, num_param)
XmCascadeButtonWidget cb;
XEvent	*event;
char   **param;
int     *num_param;
{
   /* check if event has been processed */
   if (!_XmIsEventUnique(event))
      return;

   if (((XmRowColumnWidget) XtParent(cb))->manager.traversal_on)
   {
      if (Lab_MenuType(cb) == XmMENU_BAR)
	 (* xmLabelClassRec.label_class.menuProcs) (XmMENU_BAR_CLEANUP,
						    XtParent(cb), NULL,
						    NULL, NULL);

      Select (cb, event, TRUE);

      if (CB_Submenu(cb))
      {
	 (* xmLabelClassRec.label_class.menuProcs) (XmMENU_TRAVERSAL,
						    XtParent(cb), TRUE,
						    NULL, NULL);
      }
   }

   /* record so that menuShell does not process this event */
   _XmRecordEvent(event);
}


/*
 * If the menu system is not active, arm it and arm this cascadebutton
 * else start the drag mode
 */
static void MenuBarSelect (cb, event, param, num_param)
XmCascadeButtonWidget cb;
XEvent	*event;
char   **param;
int     *num_param;
{
   Boolean validButton;

   if (RC_IsArmed ((XmRowColumnWidget) XtParent(cb)))
   {
      /* Cleanup the PM menubar mode, if enabled */
      (* xmLabelClassRec.label_class.menuProcs) (XmMENU_BAR_CLEANUP,
						 XtParent(cb), NULL,
						 NULL, NULL);

      if (!CB_Submenu(cb))
      {
	 _XmMenuFocus(XtParent(XtParent(cb)), XmMENU_MIDDLE, CurrentTime);
      }

      StartDrag (cb, event, param, num_param);
   }

   else
   {
      (* xmLabelClassRec.label_class.menuProcs) (XmMENU_BUTTON,
						 XtParent(cb), NULL, (caddr_t)event,
						 &validButton);
   
      if (validButton)
      {
	 (* xmLabelClassRec.label_class.menuProcs) (XmMENU_ARM,
						    XtParent(cb), NULL,
						    NULL, NULL);
 
	 _XmMenuFocus(XtParent(cb), XmMENU_BEGIN, CurrentTime);

	 ArmAndPost (cb, event);
	 
	 if (!CB_Submenu(cb))
	 {  
	    /*
	     * since no submenu is posted, check if the grab has occured
	     * and if not, do the pointer grab now.
	     */
	    if (RC_BeingArmed(XtParent(cb)))
	    {
	       Cursor cursor;

	       XFindContext(XtDisplay(cb), NULL, _XmMenuCursorContext,
			    (caddr_t*)&cursor);
	       XtGrabPointer(XtParent(cb), True,
			     EVENTS, GrabModeAsync, GrabModeAsync, None,
			     cursor, CurrentTime);
	       RC_SetBeingArmed(XtParent(cb), False);
	    }
	 }
	 
	 /* record so that menuShell doesn't process this event */
	 _XmRecordEvent (event);
      }
   }
}


/* 
 * If the menu is active, post submenu and arm.
 */
/*ARGSUSED*/
static void MenuBarEnter (cb, event, param, num_param)
register XmCascadeButtonWidget cb;
XEvent	*event;
char   **param;
int     *num_param;
{
   if ((RC_IsArmed (XtParent(cb))) &&
       (! cb->cascade_button.armed) && 
       (! ((XmRowColumnWidget) XtParent(cb))->manager.traversal_on))
   {
      if (!CB_Submenu(cb))
      {
	 _XmMenuFocus(XtParent(cb), XmMENU_MIDDLE, CurrentTime);
      }

      _XmCascadingPopup ((Widget)cb, event, TRUE);
      Arm(cb);
   }
}


/*
 * unless our submenu is posted or traversal is on, disarm
 */
/*ARGSUSED*/
static void MenuBarLeave (cb, event, param, num_param)
register XmCascadeButtonWidget cb;
XEvent	*event;
char   **param;
int     *num_param;
{
   XmMenuShellWidget submenuShell;

   if (RC_IsArmed (XtParent (cb)))
   {
      if (CB_Submenu(cb))
      {
         submenuShell = (XmMenuShellWidget) XtParent(CB_Submenu(cb));

         if (submenuShell->shell.popped_up)
            return;
      }  
   
      if (! ((XmRowColumnWidget) XtParent(cb))->manager.traversal_on)
         Disarm (cb, TRUE);   
   }
}

/*
 * Cleanup the menubar, if its in the PM traversal mode
 */
/*ARGSUSED*/
static void CleanupMenuBar (cb, event, param, num_param)
XmCascadeButtonWidget cb;
XEvent	*event;
char   **param;
int     *num_param;
{
    if (RC_IsArmed(XtParent(cb)))
    {
        (* ((XmRowColumnWidgetClass) XtClass(XtParent(cb)))->
             row_column_class.armAndActivate) (XtParent(cb), NULL);
	_XmRecordEvent(event);
    }
}


/*
 * CascadeButton help action 
 */
/*ARGSUSED*/
static void Help  (cb, event, param, num_param)
XmCascadeButtonWidget cb;
XEvent	*event;
char   **param;
int     *num_param;
{
   _XmCBHelp ((Widget)cb, event);
}


/*
 * CascadeButton Widget and Gadget help routine - first bring down the
 * menu and then do the help callback.
 */
#ifdef _NO_PROTO
void _XmCBHelp (w, event)
Widget w;
XEvent *event;
#else /* _NO_PROTO */
void _XmCBHelp (Widget w, XEvent *event)
#endif /* _NO_PROTO */
{
   XmAnyCallbackStruct cback;
   XmRowColumnWidget parent = (XmRowColumnWidget) XtParent(w);

   if (RC_Type(parent) == XmMENU_BAR)
   {
      int num_param = 0;
      CleanupMenuBar (w, event, NULL, &num_param); 
   }

   else if ((RC_Type(parent) == XmMENU_PULLDOWN) ||
            (RC_Type(parent) == XmMENU_POPUP))
   {
      (*(((XmMenuShellClassRec *) xmMenuShellWidgetClass)->
          menu_shell_class.popdownDone)) (XtParent(parent), NULL); 
   }

   if (XmIsGadget(w))
      _XmSocorro(w, event);
   else
      _XmPrimitiveHelp((XmPrimitiveWidget)w, event);
}


/*
 * pop down any popup children of the passed row column widget.  This
 * routine is called by all menu buttons (widgets & gadgets).
 */
#ifdef _NO_PROTO
void _XmButtonPopdownChildren (rowcol)
XmRowColumnWidget rowcol;
#else /* _NO_PROTO */
void _XmButtonPopdownChildren (XmRowColumnWidget rowcol)
#endif /* _NO_PROTO */
{
   if (RC_PopupPosted(rowcol))
   {
      (*(((XmMenuShellClassRec *)xmMenuShellWidgetClass)->
	 menu_shell_class.popdownEveryone))(RC_PopupPosted(rowcol),NULL);
   }
}


/*
 * When moving between a shared menupane, we only want to unpost the
 * descendant panes, not the shared one.
 * We only need to check the first popup child, since the menushell
 * has always forced the popped up shell to be the first child.
 */
static void PopdownGrandChildren (rowcol)
XmRowColumnWidget rowcol;

{
   CompositeWidget menuShell;

   if ((menuShell = (CompositeWidget) RC_PopupPosted(rowcol)) == NULL)
       return;

   if (menuShell =
       (CompositeWidget) RC_PopupPosted (menuShell->composite.children[0]))
   {
      (*(((XmMenuShellClassRec *)xmMenuShellWidgetClass)->
	 menu_shell_class.popdownEveryone))((Widget)menuShell,NULL);
   }
}


/*
 * call the cascading callbacks.  The cb parameter can be either a 
 * cascadebutton widget or gadget.
 */
static void Cascading (w, event)
Widget w;
XEvent *event;
{
    XmAnyCallbackStruct cback;

    cback.reason = XmCR_CASCADING;
    cback.event = event;

    if (XmIsCascadeButton(w))
    {
       XmCascadeButtonWidget cb = (XmCascadeButtonWidget)w;
       XmRowColumnWidget submenu = (XmRowColumnWidget) CB_Submenu(cb);

       /* if the submenu is already up or button is armed, just return */
       if ((submenu) && 
	   (((XmMenuShellWidget) XtParent(submenu))->shell.popped_up) &&
	   (((XmMenuShellWidget) XtParent(submenu))->
	    composite.children[0] == (Widget) submenu))
       {
	  return;
       }
       
       XtCallCallbackList ((Widget) cb, cb->cascade_button.cascade_callback, &cback);
    }
    else
    {
       XmCascadeButtonGadget cb = (XmCascadeButtonGadget)w;
       XmRowColumnWidget submenu = (XmRowColumnWidget) CBG_Submenu(cb);

       /* if the submenu is already up or button is armed, just return */
       if ((submenu) && 
	   (((XmMenuShellWidget) XtParent(submenu))->shell.popped_up) &&
	   (((XmMenuShellWidget) XtParent(submenu))->
	    composite.children[0] == (Widget) submenu))
       {
	  return;
       }
       
       XtCallCallbackList ((Widget) cb, cb->cascade_button.cascade_callback, &cback);
    }
}


/*
 * call the cascading callbacks and the popup any submenu.  This is called
 * by both the cascadebutton widget and gadget.
 */
#ifdef _NO_PROTO
void _XmCascadingPopup (cb, event, doCascade)
Widget cb;
XEvent *event;
Boolean doCascade;
#else /* _NO_PROTO */
void _XmCascadingPopup (Widget cb, XEvent *event, Boolean doCascade)
#endif /* _NO_PROTO */
{
   if (doCascade)
       Cascading (cb, event);
   Popup (cb, event);
}

/*
 * pop up the pulldown menu associated with this cascadebutton
 */

static void Popup (cb, event)
Widget  cb;
XEvent *event;
{
    Widget oldActiveChild;
    Boolean popped_up = False;
    register XmRowColumnWidget   submenu;
    register XmRowColumnWidget	parent   = (XmRowColumnWidget) XtParent (cb);

    if (XmIsCascadeButtonGadget(cb))
       submenu = (XmRowColumnWidget) CBG_Submenu(cb);
    else
       submenu = (XmRowColumnWidget) CB_Submenu(cb);

    /* if its already up, popdown submenus and then return */
    if ((submenu) &&
        (popped_up = ((XmMenuShellWidget) XtParent(submenu))->shell.popped_up))
    {
        XmMenuShellWidget shell = (XmMenuShellWidget)XtParent(submenu);

        /* Just in case the menu shell is being shared */
        if (shell->composite.children[0] == (Widget)submenu)
        {
           _XmButtonPopdownChildren(submenu);
           return;
        }
    }

    if (RC_IsArmed (parent) && XtIsManaged (parent))
    {
        /*
         * If the old active child for the menupane was a cascadeB gadget, 
         * and it did not have its submenu posted, then
         * we need to manually send it FocusOut notification, since
         * when we managed our submenu, the active_child field for
         * our parent was set to us, and the parent now no longer knows
         * who previously had the focus.
         */
        oldActiveChild = parent->manager.active_child;
        if (oldActiveChild && 
            (oldActiveChild != (Widget)cb) &&
            XmIsCascadeButtonGadget(oldActiveChild) &&
            CBG_Submenu(oldActiveChild) &&
            (((XmMenuShellWidget)XtParent(CBG_Submenu(oldActiveChild)))->
               shell.popped_up == False))
        {
            parent->manager.active_child = NULL;
            _XmDispatchGadgetInput((XmGadget)oldActiveChild, NULL, XmFOCUS_OUT_EVENT);
            ((XmGadget)oldActiveChild)->gadget.have_traversal = False;
        }
        else if (!submenu || !popped_up)
        {
           /*
            * When moving to a button without a submenu, or between 
            * non-shared panes, popdown all visible subpanes of this parent.
            */
           _XmButtonPopdownChildren (parent);
        }
        else
        {
           /*
            * Handle shared menupanes */
           PopdownGrandChildren (parent);
        }

	if (submenu)
	{
	   if (XmIsCascadeButtonGadget(cb))
	       (* xmLabelGadgetClassRec.label_class.menuProcs)
		   (XmMENU_CASCADING, cb, NULL, (caddr_t)submenu, (caddr_t)event);

	   else
	       (* xmLabelClassRec.label_class.menuProcs)
		   (XmMENU_CASCADING, cb, NULL, (caddr_t)submenu, (caddr_t)event);

           if (((ShellWidget)XtParent(submenu))->composite.num_children == 1)
	      XtManageChild (submenu);
           else
           {
              (*(((XmMenuShellClassRec *)xmMenuShellWidgetClass)->
                   menu_shell_class.popupSharedMenupane))(submenu);
           }

	}
    }
}


/*
 * Create the CB and CBG 3d arrows.
 * The Pixmaps must be unspecified going into this routine.  This helps to
 * make sure arrow cache is sync'd up as well as not accidentally overwriting
 * application's pixmap arrow if set.
 */
void _XmCreateArrowPixmaps(cb)
XmCascadeButtonWidget cb;
{
   XmArrowPixmap cpart;
   XmArrowPixmap *armed_arrow, *unarmed_arrow;
   GC gc, tsGC, bsGC;
   XGCValues values;
   Pixmap pixmap;
   short arrow_top_count, arrow_cent_count, arrow_bot_count;
   int ht, st; 
   Pixel tsc, bsc, bg;
   Dimension side;
   Screen *screen;
   int depth;
   unsigned char arrow_direction;
   unsigned short text_height;

   XRectangle *arrow_top = NULL;
   XRectangle *arrow_cent = NULL;
   XRectangle *arrow_bot = NULL;

   screen = XtScreen(cb);
   if (XmIsGadget(cb))
   {
      if ((CBG_CascadePixmap(cb) != XmUNSPECIFIED_PIXMAP) && 
	  (CBG_ArmedPixmap(cb) != XmUNSPECIFIED_PIXMAP))
	return;

      ht = G_HighlightThickness(cb);
      st = G_ShadowThickness(cb); 
      tsc = ((XmManagerWidget)XtParent(cb))->manager.top_shadow_color;
      bsc = ((XmManagerWidget)XtParent(cb))->manager.bottom_shadow_color;
      bg = ((XmManagerWidget)XtParent(cb))->core.background_pixel;
      tsGC = ((XmManagerWidget)XtParent(cb))->manager.top_shadow_GC;
      bsGC = ((XmManagerWidget)XtParent(cb))->manager.bottom_shadow_GC;
      arrow_direction = 
	 (LabG_StringDirection(cb) == XmSTRING_DIRECTION_L_TO_R) ?
	 XmARROW_RIGHT : XmARROW_LEFT;
      text_height = LabG_TextRect_height(cb);
      depth = XtParent(cb)->core.depth;
   }
   else
   {
      if ((CB_CascadePixmap(cb) != XmUNSPECIFIED_PIXMAP) && 
	  (CB_ArmedPixmap(cb) != XmUNSPECIFIED_PIXMAP))
	return;

      ht = cb->primitive.highlight_thickness;
      st = cb->primitive.shadow_thickness;
      tsc =  cb->primitive.top_shadow_color;
      bsc =  cb->primitive.bottom_shadow_color;
      bg = cb->core.background_pixel;
      tsGC = cb->primitive.top_shadow_GC;
      bsGC = cb->primitive.bottom_shadow_GC;
      arrow_direction = 
	 (cb->label.string_direction == XmSTRING_DIRECTION_L_TO_R) ?
	 XmARROW_RIGHT : XmARROW_LEFT;
      text_height = Lab_TextRect_height(cb);
      depth = cb->core.depth;
   }
   

   side = Max( (text_height * 2 / 3) + 2 * (ht + st),
	       (2*(ht + (st-1) +1)) +1 );	/* see _XmGetArrowDrawRects() */

   cpart.height = cpart.width = side;
   cpart.depth = DefaultDepth(XtDisplay(cb), DefaultScreen(XtDisplay(cb)));
   cpart.top_shadow_color = tsc;
   cpart.bottom_shadow_color = bsc;
   cpart.foreground_color = bg;
   cpart.display = XtDisplay(cb);
   cpart.pixmap = XmUNSPECIFIED_PIXMAP;


   /*
    * Create or get an existing arrow cache record for the unarmed arrow
    */
   unarmed_arrow = (XmArrowPixmap *) 
      _XmCachePart(&ArrowPixmapCache, (caddr_t)&cpart, sizeof(XmArrowPixmap));

   /*
    * Create or get an existing arrow cache record for the armed arrow
    */
   cpart.top_shadow_color = bsc;
   cpart.bottom_shadow_color = tsc;

   armed_arrow =  (XmArrowPixmap *)
      _XmCachePart(&ArrowPixmapCache, (caddr_t)&cpart, sizeof(XmArrowPixmap));

   if ((unarmed_arrow->pixmap == XmUNSPECIFIED_PIXMAP) || 
       (armed_arrow->pixmap == XmUNSPECIFIED_PIXMAP))
   {
      _XmGetArrowDrawRects(ht, st-1, arrow_direction,
	 side, side,
	 &arrow_top_count, &arrow_cent_count, &arrow_bot_count,
	 &arrow_top, &arrow_cent, &arrow_bot);

      values.foreground = values.background = bg;
      values.graphics_exposures = False;
      gc = XtGetGC (cb, GCForeground | GCBackground | GCGraphicsExposures,
	 &values);

      /* armed arrow */
      if (armed_arrow->pixmap == XmUNSPECIFIED_PIXMAP)
      {
	 pixmap = XCreatePixmap(XtDisplay(cb), RootWindowOfScreen(screen),
	    side, side, depth);

	 armed_arrow->pixmap = pixmap;

	 XFillRectangle(XtDisplay(cb), pixmap, gc, 0, 0, side, side);
	 XFillRectangles(XtDisplay((Widget)cb), pixmap,
	    bsGC,
	    arrow_top, arrow_top_count);
	 XFillRectangles(XtDisplay((Widget)cb), pixmap,
	    tsGC,
	    arrow_bot, arrow_bot_count);
	 XFillRectangles(XtDisplay((Widget)cb), pixmap,
	    gc,
	    arrow_cent, arrow_cent_count);
      }

      /* standard (unarmed) pixmap */
      if (unarmed_arrow->pixmap == XmUNSPECIFIED_PIXMAP)
      {
	 pixmap = XCreatePixmap(XtDisplay(cb),  RootWindowOfScreen(screen),
	    side, side, depth);

	 unarmed_arrow->pixmap = pixmap;

	 XFillRectangle(XtDisplay(cb), pixmap, gc, 0, 0, side, side);
	 XFillRectangles(XtDisplay((Widget)cb), pixmap,
	    tsGC,
	    arrow_top, arrow_top_count);
	 XFillRectangles(XtDisplay((Widget)cb), pixmap,
	    bsGC,
	    arrow_bot, arrow_bot_count);
	 XFillRectangles(XtDisplay((Widget)cb), pixmap,
	    gc,
	    arrow_cent, arrow_cent_count);
      }
      XtReleaseGC(cb, gc);
   }
   if (XmIsGadget(cb))
   {
      CBG_ArmedPixmap(cb) = armed_arrow->pixmap;
      CBG_CascadePixmap(cb) = unarmed_arrow->pixmap;
   }
   else
   {
      CB_ArmedPixmap(cb) = armed_arrow->pixmap;
      CB_CascadePixmap(cb) = unarmed_arrow->pixmap;
   }
}

/*
 * get the cascade size set up
 */
static int size_cascade (cascadebtn)
XmCascadeButtonWidget cascadebtn;
{
    Window rootwin;
    int x,y;					       /* must be int */
    unsigned int width, height, border, depth;	       /* must be int */

    if (CB_CascadePixmap(cascadebtn) != XmUNSPECIFIED_PIXMAP)
    {
       XGetGeometry(XtDisplay(cascadebtn), CB_CascadePixmap(cascadebtn),
		    &rootwin, &x, &y, &width, &height,
		    &border, &depth);

       CB_Cascade_width(cascadebtn) = (Dimension) width;
       CB_Cascade_height(cascadebtn) = (Dimension) height;
    }
    else
    {
       CB_Cascade_width(cascadebtn) = 0;
       CB_Cascade_height(cascadebtn) = 0;
    }
}


/*
 * set up the cascade position.  
 */

static int position_cascade (cascadebtn)
XmCascadeButtonWidget cascadebtn;
{
   Dimension buffer;

   if (CB_HasCascade(cascadebtn))
   { 
      CB_Cascade_x(cascadebtn) = XtWidth (cascadebtn) -
                               cascadebtn->primitive.highlight_thickness -
                               cascadebtn->primitive.shadow_thickness -
			       Lab_MarginWidth(cascadebtn) -
                               CB_Cascade_width(cascadebtn);

      buffer = cascadebtn->primitive.highlight_thickness +
             cascadebtn->primitive.shadow_thickness +
             Lab_MarginHeight(cascadebtn);

      CB_Cascade_y(cascadebtn) = buffer +
                               ((XtHeight(cascadebtn) -  2*buffer) -
                                CB_Cascade_height(cascadebtn)) / 2;
   }
   else
   {
      CB_Cascade_y(cascadebtn) = 0;
      CB_Cascade_x(cascadebtn) = 0;
   }
}


/*
 * set up the cascade size and location
 */
static void setup_cascade (cascadebtn, adjustWidth, adjustHeight)
XmCascadeButtonWidget cascadebtn;
Boolean adjustWidth;
Boolean adjustHeight;
{
   Dimension delta;

   if (CB_HasCascade(cascadebtn))
   {
      /*
       *  modify the size of the cascadebutton to acommadate the cascade, if
       *  needed.  The cascade should fit inside MarginRight.
       */
      if ((CB_Cascade_width(cascadebtn) + CASCADE_PIX_SPACE) >
	  Lab_MarginRight(cascadebtn))
      {
	 delta = CB_Cascade_width(cascadebtn) + CASCADE_PIX_SPACE -
	     Lab_MarginRight(cascadebtn);
	 Lab_MarginRight(cascadebtn) += delta;

	 if (adjustWidth)
	     XtWidth(cascadebtn) += delta;
	 
	 else
	 {
	    if (cascadebtn->label.alignment == XmALIGNMENT_END)
		Lab_TextRect_x(cascadebtn) -= delta;
	    else if (cascadebtn->label.alignment == XmALIGNMENT_CENTER)
		Lab_TextRect_x(cascadebtn) -= delta/2;
	 }
      }
	
      /*
       * the cascade height should fit inside of 
       * TextRect + marginTop + marginBottom
       */
      delta = CB_Cascade_height(cascadebtn) +
	  2 * (Lab_MarginHeight(cascadebtn) +
	       cascadebtn->primitive.shadow_thickness +
	       cascadebtn->primitive.highlight_thickness);
		
      if (delta > XtHeight(cascadebtn))
      {
	 delta -= XtHeight(cascadebtn);
	 Lab_MarginTop(cascadebtn) += delta/2;
	 Lab_TextRect_y(cascadebtn) += delta/2;
	 Lab_MarginBottom(cascadebtn) += delta - (delta/2);
	 
	 if (adjustHeight)
	     XtHeight(cascadebtn) += delta;
      }
   }

   position_cascade(cascadebtn);
}


/*
 * Destroy the widget
 */

static void Destroy (cb)
XmCascadeButtonWidget cb;
{
    XmRowColumnWidget submenu = (XmRowColumnWidget) CB_Submenu(cb);

    /*
     * If the armed pixmap exists, both pixmaps must be cached arrows
     */
    if (CB_ArmedPixmap(cb))
    {
       _XmArrowPixmapCacheDelete(CB_ArmedPixmap(cb));
       _XmArrowPixmapCacheDelete(CB_CascadePixmap(cb));
    }
    /*
     * break the submenu link
     */
    if (submenu != NULL)
	(* xmLabelClassRec.label_class.menuProcs) (XmMENU_SUBMENU,
						   (Widget)submenu, FALSE, (caddr_t)cb, NULL);

    if (cb->cascade_button.timer)
         XtRemoveTimeOut (cb->cascade_button.timer);
    
    XtRemoveAllCallbacks (cb, XmNactivateCallback);
    XtRemoveAllCallbacks (cb, XmNcascadingCallback);
}
                         

/*
 * routine to resize a cascade button, called by the parent
 * geometery manager
 */

static void Resize (cb)
XmCascadeButtonWidget cb;
{
    /*
     */
     if (cb)
     {
	/* Label class does it's work */
	(* xmLabelClassRec.core_class.resize) (cb);

	/* move the cascade too */
	position_cascade (cb);
     }
}


/*
 * Attach the submenu to the cascadebtn.  If it is already attached to
 * something, first break that link
 */
static void AttachSubmenu (cb)
XmCascadeButtonWidget cb;
{
   XmRowColumnWidget submenu = (XmRowColumnWidget) CB_Submenu(cb);
   Arg args[1];

   /* if its already attached somewhere, break that link */
   if (RC_CascadeBtn(submenu))
   {
      XtSetArg (args[0], XmNsubMenuId, NULL);
      XtSetValues (RC_CascadeBtn(submenu), args, 1);
   }
       
   (* xmLabelClassRec.label_class.menuProcs) (XmMENU_SUBMENU,
					      CB_Submenu(cb), TRUE, (caddr_t)cb, NULL);
}


/*
 * Set Values
 */
static Boolean SetValues (old, requested, new)
XmCascadeButtonWidget  old, requested, new;
{
    Boolean flag = FALSE;
    Boolean adjustWidth = FALSE;
    Boolean adjustHeight = FALSE;

    if ((CB_Submenu(new)) &&
	((! XmIsRowColumn(CB_Submenu(new))) ||
	 (RC_Type(CB_Submenu(new)) != XmMENU_PULLDOWN)))
    {
       CB_Submenu(new) = NULL;
       _XmWarning (new, WRONGSUBMENU);
    }

    if (new->cascade_button.map_delay < 0) 
    {
       new->cascade_button.map_delay = old->cascade_button.map_delay;
       _XmWarning (new, WRONGMAPDELAY);
    }

    /* if there is a change to submenu, notify menu system */
    if (CB_Submenu(old) != CB_Submenu(new))
    {
       if (CB_Submenu(old))
	   (* xmLabelClassRec.label_class.menuProcs) (XmMENU_SUBMENU,
						      CB_Submenu(old), FALSE,
						      (caddr_t)old, NULL);

       if (CB_Submenu(new))
	   AttachSubmenu (new);
       
    }

    /* don't let traversal be changed */
    if (Lab_MenuType(new) == XmMENU_BAR)
	new->primitive.traversal_on = TRUE;

    /* handle the cascade pixmap indicator */
    else if ((Lab_MenuType(new) == XmMENU_PULLDOWN)	||
	     (Lab_MenuType(new) == XmMENU_POPUP))
    {
       /* don't let traversal be changed */
       new->primitive.traversal_on = TRUE;

       if ((new->label.recompute_size)  || (requested->core.width <= 0))
	  adjustWidth = TRUE;

       if ((new->label.recompute_size)  || (requested->core.height <= 0))
	  adjustHeight = TRUE;

       /* get new pixmap size */
       if (CB_CascadePixmap(old) != CB_CascadePixmap (new))
       {
	  if (CB_ArmedPixmap(old) != XmUNSPECIFIED_PIXMAP)
	  {
	     _XmArrowPixmapCacheDelete(CB_ArmedPixmap(old));
	     _XmArrowPixmapCacheDelete(CB_CascadePixmap(old));
	  }
	  CB_ArmedPixmap(new) = XmUNSPECIFIED_PIXMAP;
	  size_cascade (new);
       } else
          if ( ((CB_CascadePixmap(new) ==  XmUNSPECIFIED_PIXMAP) &&
                  (!CB_Submenu(old) && CB_Submenu(new))) ||
               ((CB_ArmedPixmap(old) != XmUNSPECIFIED_PIXMAP) &&
                  (Lab_TextRect_height(old) != Lab_TextRect_height(new))) )
	  {
	     _XmArrowPixmapCacheDelete(CB_ArmedPixmap(old));
	     _XmArrowPixmapCacheDelete(CB_CascadePixmap(old));
	     CB_ArmedPixmap(new) = XmUNSPECIFIED_PIXMAP;
	     CB_CascadePixmap(new) = XmUNSPECIFIED_PIXMAP;
	     _XmCreateArrowPixmaps(new);
	     size_cascade (new);
	  }

       /*
        * resize widget if cascade appeared or disappeared, or if the
	* cascade pixmap changed size.
	*/
       if ((CB_CascadePixmap (old) != CB_CascadePixmap (new))  ||
	    (old->cascade_button.submenu != new->cascade_button.submenu))
       {
	  setup_cascade (new, adjustWidth, adjustHeight);

	  /* if there wasn't a cascade, and still isn't, don't redraw */
	  if (old->cascade_button.submenu || new->cascade_button.submenu)
	      flag = TRUE;
       }

       /* make sure that other changes did not scrunch our pixmap */
       else if (new->cascade_button.submenu)
       {
	  if ((new->primitive.highlight_thickness !=
	       old->primitive.highlight_thickness)               ||
	      (new->primitive.shadow_thickness !=
	       old->primitive.shadow_thickness)                  ||
	      (Lab_MarginRight (new) != Lab_MarginRight (old))   ||
	      (Lab_MarginHeight (new) != Lab_MarginHeight (old)) ||
	      (Lab_MarginTop (new) != Lab_MarginTop (old))	 ||
	      (Lab_MarginBottom (new) != Lab_MarginBottom (old)))
	  {
	     setup_cascade (new,adjustWidth, adjustHeight);
	     flag = TRUE;
	  }

	  else if ((Lab_MarginWidth(new) != Lab_MarginWidth(old)) ||
		   (new->core.width != old->core.width)           ||
		   (new->core.height != old->core.height))

	  {
	     position_cascade (new);
	     flag = TRUE;
	  }
       }
    }

    return (flag);
}


/*
 * Initialize
 */
static void Initialize (req, new)
XmCascadeButtonWidget  req;
XmCascadeButtonWidget  new;
{
    Boolean adjustWidth = FALSE;
    Boolean adjustHeight = FALSE;

    XmRowColumnWidget    submenu = (XmRowColumnWidget) CB_Submenu(new);
    XmRowColumnWidget    parent = (XmRowColumnWidget) XtParent(new);

    if ((! XmIsRowColumn (parent)) &&
	((Lab_MenuType(new) == XmMENU_PULLDOWN) ||
	 (Lab_MenuType(new) == XmMENU_POPUP)    ||
	 (Lab_MenuType(new) == XmMENU_BAR)))
    {
       _XmWarning (new, WRONGPARENT);
    }

    /* if menuProcs is not set up yet, try again */
    if (xmLabelClassRec.label_class.menuProcs == NULL)
	xmLabelClassRec.label_class.menuProcs =
	    (XmMenuProc) _XmGetMenuProcContext();

    new->cascade_button.armed = FALSE;
    new->cascade_button.timer = 0;
    CB_ArmedPixmap(new) = XmUNSPECIFIED_PIXMAP;

    /*
     * if the user did not specify a margin width, and we are
     * in a menuBar, set up the default
     */
    if (Lab_MarginWidth(req) == XmINVALID_DIMENSION)
       if (Lab_MenuType(new) == XmMENU_BAR)
          Lab_MarginWidth(new) = 6;

    if (submenu &&
	(! XmIsRowColumn(submenu) ||
	 (RC_Type(submenu) != XmMENU_PULLDOWN)))
    {
       submenu = NULL;
       _XmWarning (new, WRONGSUBMENU);
    }

    if (new->cascade_button.map_delay < 0) 
    {
       new->cascade_button.map_delay = MAP_DELAY_DEFAULT;
       _XmWarning (new, WRONGMAPDELAY);
    }

    /* call submenu's class function to set the link */
    if (submenu != NULL)
	AttachSubmenu (new);
    
   if (submenu && (CB_CascadePixmap(new) == XmUNSPECIFIED_PIXMAP))
      _XmCreateArrowPixmaps(new);
	 
    /*
     * get the right translations established
     */
   if (new->core.tm.translations == NULL)
   {
      if (Lab_MenuType(new) == XmMENU_PULLDOWN ||
          Lab_MenuType(new) == XmMENU_POPUP)
      {
         new->core.tm.translations = p_events_parsed;
       
	 if (req->core.width <= 0)
	     adjustWidth = TRUE;

	 if (req->core.height <= 0)
	     adjustHeight = TRUE;

	 /* get pixmap size and set up widget to allow room for it */
	 size_cascade (new);
	 setup_cascade (new, adjustWidth, adjustHeight);
      }

      else if (Lab_MenuType(new) == XmMENU_BAR)
         new->core.tm.translations = menubar_events_parsed;

   }

   new->primitive.traversal_on = TRUE;
   XtOverrideTranslations(new,
			  ((XmLabelClassRec *)
			   XtClass(new))->label_class.translations); 
}


/*
 *************************************************************************
 *
 * Public Routines                                                        
 *
 *************************************************************************
 */

#ifdef _NO_PROTO
Widget XmCreateCascadeButton (parent, name, al, ac)
Widget   parent;
char    *name;
ArgList  al;
Cardinal ac;
#else /* _NO_PROTO */
Widget XmCreateCascadeButton (Widget parent, char *name, ArgList al, Cardinal ac)
#endif /* _NO_PROTO */
{
    Widget cb;

    cb = XtCreateWidget(name, xmCascadeButtonWidgetClass, parent, al, ac);

    return (cb);
}


/*
 * This routine is called for both cascadebutton gadgets and widgets.
 * The button is armed or disarmed but it does not pop up or down submenus.
 */
#ifdef _NO_PROTO
void XmCascadeButtonHighlight (cb, highlight)
Widget cb;
Boolean highlight;
#else /* _NO_PROTO */
void XmCascadeButtonHighlight (Widget cb, 
#if NeedWidePrototypes
int highlight
#else
Boolean highlight
#endif 
)
#endif /* _NO_PROTO */
{
   if ((cb) && XmIsCascadeButton(cb))
   {
      if (highlight)
         Arm (cb);

      else
         Disarm (cb, FALSE);
   }

   else if ((cb) && XmIsCascadeButtonGadget(cb))
      XmCascadeButtonGadgetHighlight (cb, highlight);
}
