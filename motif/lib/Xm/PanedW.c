#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)PanedW.c	3.14 91/01/10";
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
#include <ctype.h>
#include <X11/cursorfont.h>
#include <Xm/PanedWP.h>
#include <Xm/SeparatoG.h>
#include <Xm/SashP.h>

typedef enum {UpperPane='U', LowerPane='L'} Direction;

#define MESSAGE4 "Invalid minimum value, must be > 0."
#define MESSAGE5 "Invalid maximum value, must be > 0."
#define MESSAGE6 "Invalid minimum/maximum value, minimum must be < maximum."
#define MESSAGE7 "Constraints do not allow appropriate sizing."
#define MESSAGE8 "Too few parameters."
#define MESSAGE9 "Invalid 1st parameter."

#define PaneInfo(w)	((XmPanedWindowConstraintPtr)(w)->core.constraints)
#define IsPane(w)	(PaneInfo(w)->panedw.isPane)
#define PaneIndex(w)	(PaneInfo(w)->panedw.position)
#define offset(field) XtOffset(XmPanedWindowWidget, paned_window.field)

#define XmBLOCK	10

/* Resolution independence conversion functions */

extern void _XmIntializeGetValuesResources();

/* Default action list */

static XtActionsRec actionsList[] =
{
   { "Help",     (XtActionProc) _XmManagerHelp },
   { "arm",      (XtActionProc) _XmGadgetArm  },
   { "activate", (XtActionProc) _XmGadgetActivate },
};

/****************************************************************
 *
 * Paned Window Resources
 *
 ****************************************************************/

static XtResource resources[] = {
    {XmNmarginWidth, XmCMarginWidth, XmRHorizontalDimension, sizeof(Dimension),
       offset(margin_width), XmRImmediate, (caddr_t) 3},

    {XmNmarginHeight, XmCMarginHeight, XmRVerticalDimension, sizeof(Dimension),
       offset(margin_height), XmRImmediate, (caddr_t) 3},

    {XmNspacing, XmCSpacing, XmRVerticalDimension, sizeof(Dimension),
       offset(spacing), XmRImmediate, (caddr_t) 8},

    {XmNrefigureMode, XmCBoolean, XmRBoolean, sizeof(Boolean),
       offset(refiguremode), XmRImmediate, (caddr_t) TRUE},

    {XmNseparatorOn, XmCSeparatorOn, XmRBoolean, sizeof(Boolean),
       offset(separator_on), XmRImmediate, (caddr_t) TRUE},

    {XmNsashIndent, XmCSashIndent, XmRHorizontalPosition, sizeof(Position),
       offset(sash_indent), XmRImmediate, (caddr_t) -10},

    {XmNsashWidth, XmCSashWidth, XmRHorizontalDimension, sizeof(Dimension),
       offset(sash_width), XmRImmediate, (caddr_t) 10},

    {XmNsashHeight, XmCSashHeight, XmRVerticalDimension, sizeof(Dimension),
       offset(sash_height), XmRImmediate, (caddr_t) 10},

    {XmNsashShadowThickness, XmCShadowThickness, XmRHorizontalDimension,
       sizeof(Dimension), offset(sash_shadow_thickness), XmRImmediate,
       (caddr_t) 2},
};

/* Definition for resources that need special processing in get values */

static XmSyntheticResource get_resources[] =
{
   { XmNmarginWidth, 
     sizeof(Dimension),
     offset(margin_width),
     _XmFromHorizontalPixels,
     _XmToHorizontalPixels
   },

   { XmNmarginHeight,
     sizeof(Dimension),
     offset(margin_height),
     _XmFromVerticalPixels,
     _XmToVerticalPixels
   },

   { XmNspacing,
     sizeof(Dimension),
     offset(spacing),
     _XmFromVerticalPixels,
     _XmToVerticalPixels
   },

   { XmNsashIndent,
     sizeof(Position),
     offset(sash_indent),
     _XmFromHorizontalPixels,
     _XmToHorizontalPixels
   },

   { XmNsashWidth,
     sizeof(Dimension),
     offset(sash_width),
     _XmFromHorizontalPixels,
     _XmToHorizontalPixels
   },

   { XmNsashHeight,
     sizeof(Dimension),
     offset(sash_height),
     _XmFromVerticalPixels,
     _XmToVerticalPixels
   },

   { XmNsashShadowThickness,
     sizeof(Dimension),
     offset(sash_shadow_thickness),
     _XmFromHorizontalPixels,
     _XmToHorizontalPixels
   },
};

#undef offset



/****************************************************************
 *
 * Paned Window Constraint Resources For Its Constraint Record
 *
 ****************************************************************/

#define offset(field) XtOffset(XmPanedWindowConstraintPtr, panedw.field)

static XtResource constraint_resources[] = {
    {XmNallowResize, XmCBoolean, XmRBoolean, sizeof(Boolean),
	 offset(allow_resize), XmRImmediate, (caddr_t) FALSE},
    {XmNpaneMinimum, XmCPaneMinimum, XmRVerticalDimension, sizeof(Dimension),
         offset(min), XmRImmediate, (caddr_t) 1},
    {XmNpaneMaximum, XmCPaneMaximum, XmRVerticalDimension, sizeof(Dimension),
         offset(max), XmRImmediate, (caddr_t) 1000},
    {XmNskipAdjust, XmCBoolean, XmRBoolean, sizeof(Boolean),
         offset(skip_adjust), XmRImmediate, (caddr_t) FALSE},
};

/* Definition for constraint resources that need special */
/* processing in get values                              */

static XmSyntheticResource get_constraint_resources[] =
{
    {  XmNpaneMinimum,
       sizeof(Dimension),
       offset(min),
       _XmFromVerticalPixels,
       _XmToVerticalPixels
    },

    {  XmNpaneMaximum,
       sizeof(Dimension),
       offset(max),
       _XmFromVerticalPixels,
       _XmToVerticalPixels
    },
};

#undef offset

static void AdjustGC();
static void GetFlipGC();
static void ConstraintInit();
static void ClassPartInitialize();
static void Initialize();
static void Realize();
static void Destroy();
static void ConstraintDestroy();
static void Resize();
static Boolean SetValues();
static XtGeometryResult GeometryManager();
static void ChangeManaged();
static Cardinal InsertOrder();
static void InsertChild();
static Boolean PaneSetValues();


/*************************************<->*************************************
 *
 *
 *   Description:  PanedWindow full class record
 *   -----------
 *************************************<->***********************************/

externaldef(xmpanedwindowclassrec) XmPanedWindowClassRec xmPanedWindowClassRec =
{
   {	 					/* core class fields   */
      (WidgetClass) &xmManagerClassRec,		/* superclass          */
      "XmPanedWindow",				/* class name          */
      sizeof(XmPanedWindowRec),			/* size                */
      NULL,					/* class initialize    */
      ClassPartInitialize,			/* class_part_inite    */
      FALSE,					/* class_inited        */
      Initialize,				/* initialize          */
      NULL,					/* initialize_hook     */
      Realize,					/* realize             */
      actionsList,				/* actions             */
      XtNumber(actionsList),			/* num_actions         */
      resources,				/* resourses           */
      XtNumber(resources),			/* resource_count      */
      NULLQUARK,				/* xrm_class           */
      TRUE,					/* compress_motion     */
      XtExposeCompressMaximal,			/* compress_exposure   */
      TRUE,					/* compress_enter/lv   */
      FALSE,					/* visible_interest    */
      Destroy,					/* destroy             */
      Resize,					/* resize              */
      _XmRedisplayGadgets,			/* expose              */
      SetValues,				/* set_values          */
      NULL,					/* set_values_hook     */
      XtInheritSetValuesAlmost,			/* set_values_almost   */
      NULL,					/* get_values_hook     */
      NULL,					/* accept_focus        */
      XtVersion,				/* version             */
      NULL, 					/* callback_private    */
      XtInheritTranslations,			/* tm_table            */
      NULL,					/* Query Geometry proc */
      NULL,					/* display accelerator */
      NULL,					/* extension           */
   },

   {						/* composite class fields */
      GeometryManager,				/* geometry_manager       */
      ChangeManaged,				/* change_managed         */
      InsertChild,				/* insert_child           */
      XtInheritDeleteChild,			/* delete_child           */
      NULL,					/* extension              */
   },

   {						/* constraint class fields */
      constraint_resources,			/* subresourses            */
      XtNumber(constraint_resources),		/* subresource_count       */
      sizeof(XmPanedWindowConstraintRec),	/* constraint_size         */
      ConstraintInit,				/* initialize              */
      ConstraintDestroy,			/* destroy                 */
      PaneSetValues,				/* set_values              */
      NULL,					/* extension               */
  }, 

  {						/* manager_class fields   */
      XtInheritTranslations,	 		/* translations      	  */
      get_resources,				/* get resources      	  */
      XtNumber(get_resources),			/* num get_resources 	  */
      get_constraint_resources,			/* get_cont_resources     */
      XtNumber(get_constraint_resources),	/* num_get_cont_resources */
      XmInheritParentProcess,                   /* parent_process         */
      NULL, 					/* extension              */
  }, 

  {						/* paned_window_class fields */
      NULL, 					/* extension                 */
  }
};

externaldef(xmpanedwindowwidgetclass) WidgetClass xmPanedWindowWidgetClass =
					   (WidgetClass) &xmPanedWindowClassRec;


/**********************************************************************
 *
 *  ReManageChildren
 *	This procedure will be called by the ChangeManged procedure 
 *	It will reassemble the currently managed children into the 
 *      "paned_window.managed_children" list.
 *
 ********************************************************************/

static void ReManageChildren(pw)
XmPanedWindowWidget pw;

{
   int i;

   pw->paned_window.num_managed_children = 0;

   for (i = 0; i < pw->composite.num_children; i++)
   {
       if (XtIsManaged(pw->composite.children[i]))
       {
	    if ((pw->paned_window.num_managed_children+1) >
				 pw->paned_window.num_slots)
            {
	       pw->paned_window.num_slots += XmBLOCK;
	       pw->paned_window.managed_children = (WidgetList)
		  XtRealloc ((caddr_t) pw->paned_window.managed_children,
			     (pw->paned_window.num_slots * sizeof(Widget)));
            }
	    pw->paned_window.managed_children
	            [pw->paned_window.num_managed_children++] =
		       pw->composite.children[i];
       }
   }
}

/*************************************<->*************************************
 *
 *  NeedsAdjusting (pw)
 *
 *   Description:
 *   -----------
 *     Calculate the height needed to fully display this paned window.
 *
 *************************************<->***********************************/

static int NeedsAdjusting(pw)
 register XmPanedWindowWidget pw;
{
   int needed, i;

   needed = 0;
   for (i = 0; i < pw->paned_window.pane_count; i++) {
     needed += PaneInfo(pw->paned_window.managed_children[i])->panedw.dheight +
                 2 * pw->paned_window.managed_children[i]->core.border_width +
                 pw->paned_window.spacing;
   }

  /*
   * Get rid of extra spacing from previous 'for' loop and add in
   * margin height at the top and bottom of the panedw window
   */
   if (pw->paned_window.pane_count > 0)
       needed += 2*pw->paned_window.margin_height - pw->paned_window.spacing;

   return (needed != pw->core.height) ? needed : 0 ;
}


/*************************************<->*************************************
 *
 *  AdjustPanedWindowHeight
 *
 *   Description:
 *   -----------
 *     Request a new size for the vertical paned window from its parent.
 *     If the requested new height is less than 1, then ask for size of 1.
 *     Since we don't 
 *
 *************************************<->***********************************/
static XtGeometryResult AdjustPanedWindowHeight(pw, newheight, reply_height)
  XmPanedWindowWidget pw;
  Dimension newheight;
  Dimension *reply_height;
{
    Dimension replyWidth, replyHeight;
    XtGeometryResult result = XtGeometryNo;

    if (newheight < 1) newheight = 1;
    switch (XtMakeResizeRequest((Widget)pw, pw->core.width, newheight,
 			        &replyWidth, &replyHeight))
    {
      case XtGeometryYes:
          *reply_height = newheight;
          result = XtGeometryYes;
          break;

      case XtGeometryAlmost:
          XtMakeResizeRequest((Widget)pw, replyWidth, replyHeight, NULL, NULL);
          *reply_height = replyHeight;
          result = XtGeometryAlmost;
          break;

      case XtGeometryNo:
          *reply_height = pw->core.height;
          result = XtGeometryNo;
          break;
    }
    return(result);
}


/*************************************<->*************************************
 *
 *  ResetDHeights
 *
 *   Description:
 *   -----------
 *     Set the desired height field in the constraint record for each
 *     pane.
 *
 *************************************<->***********************************/
static void ResetDHeights(pw)
  XmPanedWindowWidget pw;
{
    Widget *childP;
    int i;

    for (i=0, childP = pw->paned_window.managed_children; 
          i < pw->paned_window.pane_count;  childP++, i++)
             PaneInfo(*childP)->panedw.dheight = (*childP)->core.height;
}


/*************************************<->*************************************
 *
 *  RefigureLocations
 *
 *   Description:
 *   -----------
 *   This is the workhorse routine which actually computes where the children
 *   are going to be placed.  It honors any Min/Max constraints placed on a
 *   pane as well as honoring the direction to do the refiguring.
 *
 *************************************<->***********************************/
static void RefigureLocations(pw, c_index, dir, rflag)
  register XmPanedWindowWidget pw;
  int c_index;
  Direction dir;
  Boolean rflag;
{
    WidgetList children = pw->paned_window.managed_children;
    int num_panes = pw->paned_window.pane_count;
    int _dir = (dir == UpperPane) ? 1 : -1;
    int spacing;
    XmPanedWindowConstraintPart * pane;
    register Widget *childP;
    Position y;
    int heightused;
    int cdir, i, adjustment;
    Widget top;
    int pass;

    if (num_panes == 0 || !pw->paned_window.refiguremode)
	return;

    spacing = pw->paned_window.spacing;

   /*
    * ENFORCE THE MIN/MAX CONSTRAINTS; ALSO KEEP TRACK OF THE 
    * TOTAL HEIGHT NEEDED TO DISPLAY VPANED WINDOW BASED ON 
    * DESIRED HEIGHTS OF PANES.
    */
   heightused = 0;
   for (childP = children, i = 0; i < num_panes; childP++, i++) 
     {
       pane = &(PaneInfo(*childP)->panedw);

       if (pane->dheight < pane->min) pane->dheight = pane->min;
       else if (pane->dheight > pane->max) pane->dheight = pane->max;

       heightused += pane->dheight + spacing + 2 * (*childP)->core.border_width;

     }
   /*
    * Get rid of extra spacing from previous 'for' loop and add in
    * margin height at the top and bottom of the paned window
    */
    heightused += 2*pw->paned_window.margin_height - spacing;

    childP = children + c_index;
    if (dir == UpperPane && c_index != num_panes - 1) childP++;
    cdir = _dir;
   /* allow at most 3 passes through the panes to adjust the heights */
    for (pass = 0; heightused != pw->core.height &&
		             pass < (9 * num_panes); pass++) {
	pane = &(PaneInfo(*childP)->panedw);
	if (!pane->skip_adjust || cdir != _dir) {
	    int old = pane->dheight;
	    if (heightused < pw->core.height)
	        pane->dheight+= pw->core.height - heightused;
	    else
	        if (heightused - pw->core.height < pane->dheight &&
                    pane->dheight - (heightused - pw->core.height) > 1)
		    pane->dheight-= heightused - pw->core.height;
	        else
		    pane->dheight = 1;
	    if (pane->dheight < pane->min) pane->dheight = pane->min;
	    if (pane->dheight > pane->max) pane->dheight = pane->max;
	    heightused += (pane->dheight - old);
	}
	childP+= cdir;
 /*
  * WE GET INTO THE NEXT WHILE LOOP WHEN WE HAVE EXHAUSTED OUR
  * LIST OF CHILDREN AND WE STILL NEED TO REDISTRIBUTE A CHANGE IN
  * HEIGHT, NOW WE WILL TRY TO CHANGE DIRECTION AND SEE IF THERE
  * IS A PANE BACK WHERE WE STARTED FROM THAT CAN ABSORB THE
  * SHORTAGE/OVERAGE
  */
	while ((childP < children) || ((childP - children) >= num_panes)) {
	    cdir = -cdir;
	    if (cdir == _dir) {
    		y = pw->paned_window.margin_height;
    		for (childP = children, i = 0; i < num_panes; childP++, i++) {
       			PaneInfo(*childP)->panedw.dy = y;
       			y += PaneInfo(*childP)->panedw.dheight + spacing +
                             2 * (*childP)->core.border_width;
    		}
    		y += pw->paned_window.margin_height - spacing;
                /* if not resizing, make sure the sum of the pane 
                   heights are not greater than the vpane height */
                if (!rflag){
                   if (y > pw->core.height) {
                       childP = children + c_index;
                       pane = &(PaneInfo(*childP)->panedw);
                       if (pane->dheight > (y - pw->core.height))
                         pane->dheight = (pane->dheight - 
                                               (y - pw->core.height)); 
                       else
                         pane->dheight = 1;
                        
                   } else {
                       return;
                   }
                } else
                   return;
             }
	     childP = children + c_index + cdir;

       /* HANDLE SPECIAL CASE */
           if ((c_index == 0) && (cdir < 0)) childP++;
	}
    }
    y = pw->paned_window.margin_height;
    for (childP = children, i = 0; i < num_panes; childP++, i++) {
       PaneInfo(*childP)->panedw.dy = y;
       y += PaneInfo(*childP)->panedw.dheight + spacing +
            2 * (*childP)->core.border_width;
    }
    y += pw->paned_window.margin_height - spacing;

    if (y > pw->core.height)
      _XmWarning (pw, MESSAGE7);
}


/*************************************<->*************************************
 *
 *  CommitNewLocations
 *
 *   Description:
 *   -----------
 *    Use the core width/height,  It also raises the sash for the
 *    pane, and prevents the sash from disappearing from the screen.
 *
 *************************************<->***********************************/

static void CommitNewLocations(pw)
XmPanedWindowWidget pw;
{
    WidgetList children = pw->paned_window.managed_children;
    int num_panes = pw->paned_window.pane_count;
    register Widget *childP;
    XWindowChanges changes;
    int i, xoffset, sepY;

    changes.stack_mode = Above;

    xoffset = pw->paned_window.margin_width;

    for (childP = children, i = 0; i < num_panes; childP++, i++) {
	register XmPanedWindowConstraintPart * pane =
                                 &(PaneInfo(*childP)->panedw);
	register Widget sash = pane->sash;
	register Widget separator = pane->separator;

        if (sash)  /* IF THIS IS NOT NULL */
         {
	   _XmMoveObject((RectObj)*childP, xoffset, pane->dy);
	   _XmResizeObject((RectObj)*childP, pw->core.width - 2 *
                          ((*childP)->core.border_width +
                          pw->paned_window.margin_width),
                          pane->dheight, (*childP)->core.border_width);             
           if (separator)
           {
              sepY = (*childP)->core.y + (*childP)->core.height + 
                        2 * (*childP)->core.border_width +
                        pw->paned_window.spacing/2 - separator->core.height/2 -
                        separator->core.border_width;

              _XmMoveObject((RectObj)separator, separator->core.x,  sepY);
	      _XmResizeObject((RectObj)separator, pw->core.width,
                              separator->core.height,
                              separator->core.border_width);
           }

	   /* Move and Display the Sash */
           if (pw->paned_window.sash_indent < 0)
  	        changes.x = pw->core.width + pw->paned_window.sash_indent
		           - sash->core.width - sash->core.border_width*2;
           else
                changes.x = pw->paned_window.sash_indent;


            /* PREVENT SASH FROM DISAPPEARING FROM SCREEN */
           if ((changes.x > (pw->core.width - sash->core.width)) || 
              (changes.x < 0))
                changes.x = 0;
   
           changes.y = (*childP)->core.y + (*childP)->core.height + 
                       2 * (*childP)->core.border_width +
                       pw->paned_window.spacing/2 - sash->core.height/2 - 
		       sash->core.border_width;

	   /* This should match XtMoveWidget, except that we're
	    * also insuring the sash is Raised in the same request */

	   sash->core.x = changes.x;
	   sash->core.y = changes.y;

	   if (XtIsRealized(pane->sash))
	       XConfigureWindow( XtDisplay(pane->sash), XtWindow(pane->sash),
			         CWX | CWY | CWStackMode, &changes );
         } else {
	    _XmMoveObject((RectObj)*childP, xoffset, pane->dy);
	    _XmResizeObject((RectObj)*childP, pw->core.width -
                           2*(pw->core.border_width +
                           pw->paned_window.margin_width),
                           pane->dheight, (*childP)->core.border_width);             
         }
    }

    if (pw->paned_window.flipgc != NULL)
	AdjustGC(pw);
}


/*************************************<->*************************************
 *
 *  RefigureLocationsAndCommit 
 *
 *   Description:
 *   -----------
 *    A utility call that does the call to calculate the pane layout 
 *    and then move the panes to their new locations.
 *
 *************************************<->***********************************/

static void RefigureLocationsAndCommit(pw, c_index, dir, rflag)
  XmPanedWindowWidget pw;
  Direction dir;
  Boolean rflag;
{
    if (pw->paned_window.refiguremode) {
	RefigureLocations(pw, c_index, dir, rflag);
	CommitNewLocations(pw);
    }
}


/*************************************<->*************************************
 *
 *  DrawTrackLines
 *
 *   Description:
 *   -----------
 *     Erase any old track lines (point are kept in the pane's constraint
 *     record--olddy field) and then draw new track lines across the top
 *     of all panes (except the first).  These lines will be interactively 
 *     moved (by other routines) to respond to the user's request to resize 
 *     panes within the VPane Manager.
 *
 *************************************<->***********************************/

static void DrawTrackLines(pw)
  XmPanedWindowWidget pw;
{
    Widget *childP;
    XmPanedWindowConstraintPart * pane;
    Widget *children = pw->paned_window.managed_children;
    int num_panes = pw->paned_window.pane_count;
    Dimension separator_height;
    int x1, x2;

    x1= 0;
    x2= pw->core.width - x1;

    for (childP = children + 1; childP - children < num_panes; childP++) {
        pane = &(PaneInfo(*childP)->panedw);
        separator_height =
		 pane->separator ? pane->separator->core.height: (Dimension)2;

	if (pane->olddy != pane->dy) {
            XDrawLine(XtDisplay(pw), XtWindow(pw), pw->paned_window.flipgc, x1,
                      pane->olddy - (pw->paned_window.spacing +
                      separator_height) / 2, x2,
                      pane->olddy - (pw->paned_window.spacing +
                      separator_height) / 2);
            XDrawLine(XtDisplay(pw), XtWindow(pw), pw->paned_window.flipgc,
                      x1, pane->dy - (pw->paned_window.spacing +
                      separator_height) / 2, x2,
                      pane->dy - (pw->paned_window.spacing +
                      separator_height) / 2);
	    pane->olddy = pane->dy;
	}
    }
}

/*************************************<->*************************************
 *
 *  EraseTrackLines
 *
 *   Description:
 *   -----------
 *   After the user has stopped adjusting the pane sizes, erase the last
 *   set of track lines (remember that DrawTrackLines erases old track
 *   lines before drawing new ones).
 *
 *************************************<->***********************************/

static void EraseTrackLines(pw)
  XmPanedWindowWidget pw;
{
    Widget *childP;
    XmPanedWindowConstraintPart * pane;
    Widget *children = pw->paned_window.managed_children;
    int num_panes = pw->paned_window.pane_count;
    Dimension separator_height;
    int x1, x2;

    x1= 0;
    x2= pw->core.width - x1;

    for (childP = children + 1; childP - children < num_panes; childP++) {
        pane = &(PaneInfo(*childP)->panedw);
        separator_height =
		 pane->separator ? pane->separator->core.height: (Dimension)2;

	XDrawLine(XtDisplay(pw), XtWindow(pw), pw->paned_window.flipgc, x1,
		  pane->olddy - (pw->paned_window.spacing +
		  separator_height) / 2, x2,
		  pane->olddy - (pw->paned_window.spacing +
                  separator_height) / 2);
    }
}


/*************************************<->*************************************
 *
 *  ProcessKeyEvent
 *
 *   Description:
 *   -----------
 *    This function processes a batch of key pressed events
 *    so that a sash movement action via the keyboard doesn't
 *    get too far behind the key event actions. 
 *
 *************************************<->***********************************/

/* ARGSUSED */
static XtTimerCallbackProc ProcessKeyEvent(client_data, id)
caddr_t client_data;
XtIntervalId *id;
{
    Widget w = (Widget) client_data;
    register XmPanedWindowWidget pw = (XmPanedWindowWidget)w->core.parent;
    register WidgetList children = pw->paned_window.managed_children;
    int num_panes = pw->paned_window.pane_count;
    Widget *childP;
    XmPanedWindowConstraintPart * pane;
    short c_index;
    int diff;

    pw->paned_window.top_pane = pw->paned_window.bottom_pane = NULL;

    if (pw->paned_window.increment_count < 0) {
        /* NOTE THAT w IS A SASH, TO GET POSITION WE HAVE
           TO GET INDEX OF PANE ASSOCIATED WITH THIS SASH */
	c_index = PaneIndex(PaneInfo(w)->panedw.sash);
        if (c_index < (num_panes-1)) c_index++;

	pane = &(PaneInfo(children[c_index])->panedw);
	while (pane->max == pane->min && c_index < num_panes - 1)
	     pane = &(PaneInfo(children[++c_index])->panedw);
 	pw->paned_window.bottom_pane = PaneInfo(children[c_index]);
    } else {
        /* NOTE THAT w IS A SASH, TO GET POSITION WE HAVE
           TO GET INDEX OF PANE ASSOCIATED WITH THIS SASH */
 	c_index = PaneIndex(PaneInfo(w)->panedw.sash);

	pane = &(PaneInfo(children[c_index])->panedw);
	while (pane->max == pane->min && c_index > 0)
             pane = &(PaneInfo(children[--c_index])->panedw);
	pw->paned_window.top_pane = PaneInfo(children[c_index]);
    }

    for (childP = children; childP - children < num_panes; childP++)
       PaneInfo(*childP)->panedw.olddy = -99;

    ResetDHeights( pw );
    diff = pw->paned_window.increment_count;
    if (pw->paned_window.top_pane) {
       /* make sure heights don't go negative */
        if ((-diff) >= (int)pw->paned_window.top_pane->panedw.dheight) {
           /* can't add as much to other pane */
            diff = -pw->paned_window.top_pane->panedw.dheight + 1;
            pw->paned_window.top_pane->panedw.dheight = 1;
        }
        else
            pw->paned_window.top_pane->panedw.dheight += diff;
        RefigureLocationsAndCommit(pw, PaneIndex(PaneInfo(w)->panedw.sash),
                                   UpperPane, False);
    } else {
        if (diff >= (int)pw->paned_window.bottom_pane->panedw.dheight) {
            pw->paned_window.bottom_pane->panedw.dheight = 1;
        } else
            pw->paned_window.bottom_pane->panedw.dheight -= diff;
        RefigureLocationsAndCommit(pw, PaneIndex(PaneInfo(w)->panedw.sash),
                                   LowerPane, False);
    }
    pw->paned_window.increment_count = 0;
}


/*************************************<->*************************************
 *
 *  HandleSash
 *
 *   Description:
 *   -----------
 *    Selection Events on the sashes invoke this routine through 
 *    callbacks.  An adjustment of the size of the panes always involves
 *    2 panes (one to shrink and one to grow) if the type of sash grab is 
 *    "This Window Only" then both a top and bottom pane are selected (these
 *    are the panes which will be adjusted); if the grab type is "Upper Pane"
 *    then only a "top" pane is chosen and the correct "bottom"
 *    pane will be determined by the "RefigureLocations" routine.  If
 *    the grab type is "Lower" then only a "bottom" pane is chosen and the
 *    the correct "top" pane is found by the "RefigureLocations" routine.
 *
 *************************************<->***********************************/

/* ARGSUSED */
static void HandleSash(w, closure, callData)
  Widget	w;
  caddr_t	closure;	/* (paned) Widget (unused) */
  caddr_t	callData;	/* SashCallData */
{
    SashCallData call_data = (SashCallData)callData;
    register XmPanedWindowWidget pw = (XmPanedWindowWidget)w->core.parent;
    register WidgetList children = pw->paned_window.managed_children;
    int num_panes = pw->paned_window.pane_count;
    short increment;
    short c_index;
    int diff, y;
    Widget *childP;
    XmPanedWindowConstraintPart * pane;
    char ActionType;

    if (call_data->num_params == 0) {
        _XmWarning (pw, MESSAGE8);
        return;
    }

    if (call_data->event->xany.type == KeyPress) {

    }

    switch (call_data->event->xany.type) {
        case ButtonPress:
	case ButtonRelease: y = call_data->event->xbutton.y_root;
			    break;

        case KeyRelease: return;
			 
        case KeyPress:  if (call_data->num_params < 3) {
             		   _XmWarning (pw, MESSAGE8);
             		   return;
    			}
        		if (*call_data->params[2] == 'U') {
            		   if (*call_data->params[1] == 'L')
                	      increment = -10;
	    		   else
	        	      increment = -1;
        		} else if (*call_data->params[2] == 'D') {
            		   if (*call_data->params[1] == 'L')
	        	      increment = 10;
	    		   else
	        	      increment = 1;
        		}
			if (!pw->paned_window.increment_count) {
	    		   pw->paned_window.increment_count = increment;
            		   XtAppAddTimeOut(XtWidgetToApplicationContext(pw),
			  		  XtGetMultiClickTime(XtDisplay(pw)), 
			  		  (XtTimerCallbackProc) ProcessKeyEvent,
					  (caddr_t) w);
        		} else
	    		   pw->paned_window.increment_count += increment;
	    		return;

        case MotionNotify:  y = call_data->event->xmotion.y_root;
			    break;

	default:	    y = pw->paned_window.starty;
    }

    ActionType = *call_data->params[0];
    if (islower(ActionType)) ActionType = toupper(ActionType);

    switch (ActionType) {
	case 'S':		/* Start adjustment */
	    pw->paned_window.top_pane = pw->paned_window.bottom_pane = NULL;

            /* NOTE THAT w IS A SASH, TO GET POSITION WE HAVE
               TO GET INDEX OF PANE ASSOCIATED WITH THIS SASH */
            c_index = PaneIndex(PaneInfo(w)->panedw.sash);
            if (c_index < (num_panes-1)) c_index++;

	    pane = &(PaneInfo(children[c_index])->panedw);
	    while (pane->max == pane->min && c_index < num_panes - 1)
	          pane = &(PaneInfo(children[++c_index])->panedw);
	    pw->paned_window.bottom_pane = PaneInfo(children[c_index]);

            /* NOTE THAT w IS A SASH, TO GET POSITION WE HAVE
               TO GET INDEX OF PANE ASSOCIATED WITH THIS SASH */
	    c_index = PaneIndex(PaneInfo(w)->panedw.sash);

	    pane = &(PaneInfo(children[c_index])->panedw);
  	    while (pane->max == pane->min && c_index > 0)
                  pane = &(PaneInfo(children[--c_index])->panedw);
   	    pw->paned_window.top_pane = PaneInfo(children[c_index]);

	    pw->paned_window.starty = y;

	    for (childP = children; childP - children < num_panes; childP++)
		PaneInfo(*childP)->panedw.olddy = -99;
	    break;

	case 'M': 
	    ResetDHeights( pw );
	    diff = y - pw->paned_window.starty;
            if (diff > 0) {
		/* make sure heights don't go negative */
		if ((-diff) >= (int)pw->paned_window.top_pane->panedw.dheight) {
		    /* can't add as much to other pane */
		    diff = -pw->paned_window.top_pane->panedw.dheight + 1;
		    pw->paned_window.top_pane->panedw.dheight = 1;
		}
		else {
		    pw->paned_window.top_pane->panedw.dheight += diff;
		}
	       RefigureLocations(pw, PaneIndex(PaneInfo(w)->panedw.sash),
                                 UpperPane, False);
	    } else {
		if (diff >= (int)pw->paned_window.bottom_pane->panedw.dheight) {
		  pw->paned_window.bottom_pane->panedw.dheight = 1;
		} else {
		  pw->paned_window.bottom_pane->panedw.dheight -= diff;
                }
	       RefigureLocations(pw, PaneIndex(PaneInfo(w)->panedw.sash),
                                 LowerPane, False);
	    }

	    DrawTrackLines(pw);
	    break;


	case 'C':
	    EraseTrackLines(pw);
	    CommitNewLocations(pw);
	    break;

	default:
            _XmWarning (pw, MESSAGE9);
     }
}



/*************************************<->*************************************
 *
 * GeometryManager
 *
 *   Description:
 *   -----------
 *  The Geometry Manager only allows changes after Realize if
 *  allow_resize is True in the constraints record.  It
 *  only allows height changes, but offers the requested height
 *  as a compromise if both width and height changes were requested.
 *  As all good Geometry Managers should, we will return No if the
 *  request will have no effect; i.e. when the requestor is already
 *  of the desired geometry.
 *
 *************************************<->***********************************/

static XtGeometryResult GeometryManager(w, request, reply)
Widget w;
XtWidgetGeometry *request, *reply;
{
    XmPanedWindowWidget pw = (XmPanedWindowWidget) w->core.parent;
    XtGeometryMask mask = request->request_mode;
    int replyWidth, replyHeight;
    int old_dheight;
    Dimension new_height, reply_height;
    XtWidgetGeometry allowed;
    XmPanedWindowConstraintPart * pane;
    Boolean request_altered = FALSE;
    register Widget *children;
    int i, childWidth, childBorderWidth;
    int num_panes =0;
    register Widget *childP;


    if (XmIsSash(w)) {
        if ((mask & CWX) && request->x >= 0) w->core.x = request->x;
        if ((mask & CWY) && request->y >= 0) w->core.y = request->y;
        if ((mask & CWHeight) && request->height > 0)
	   w->core.height = request->height;
        if ((mask & CWWidth) && request->width > 0)
	   w->core.width = request->width;
	return XtGeometryYes;
    }

    pane = &(PaneInfo(w)->panedw);

/* DISALLOW RESIZES IF FLAG FOR THIS PANE SAYS NO */    
    if (XtIsRealized((Widget)pw) && !pane->allow_resize)
      return XtGeometryNo;

/* REJECT ATTEMPTS THAT DO NOT WANT TO ADJUST HEIGHT, OR REQUESTS WHERE
   WIDGET IS ALREADY EQUAL TO SIZE THEY ARE REQUESTING  */
    if (mask & ~(CWWidth | CWHeight))
        return XtGeometryNo;

/* MAKE SURE THAT MIN/MAX CONSTRAINTS ARE HONORED. AND IF WE NEED TO
   ENFORCE IT, RETURN XtGeometryAlmost */
   if (request->height < pane->min && (CWHeight & mask))
    {  request->height = pane->min; request_altered = TRUE; }
   if (request->height > pane->max && (CWHeight & mask))
    {  request->height = pane->max;  request_altered = TRUE; }

    allowed.request_mode = mask;

   if ((mask & CWWidth)) {
      /* 
       * NOW RECOMPUTE THE LIST OF MANAGED CHILDREN.
       */
      ReManageChildren((XmManagerWidget)pw);
      children = pw->paned_window.managed_children;

      /*
       * COUNT THE NUMBER OF PANES THAT ARE MANAGED.
       */
      childP = children;
      while ( (num_panes < pw->paned_window.num_managed_children) &&
                XtIsRectObj(*childP) &&
                (*childP)->core.widget_class != xmSashWidgetClass &&
                (*childP++)->core.widget_class != xmSeparatorGadgetClass)
                     num_panes++;

      pw->paned_window.pane_count = num_panes;
      /* 
       * SET WIDTH OF PANED WINDOW EQUAL TO THAT OF WIDEST CHILD
       */

       childWidth = 0;
       childBorderWidth = 0;
       for (childP = children, i = 0; i < num_panes; childP++, i++)
        {
           if (((*childP)->core.width + (*childP)->core.border_width) >
                 childWidth + childBorderWidth) 
               childWidth = (*childP)->core.width;
               childBorderWidth = (*childP)->core.border_width;
        }
       if (childWidth < 1) childWidth = 1;
       allowed.width = childWidth;
       if (request->width > allowed.width) {
          switch (XtMakeResizeRequest(pw, request->width +
                                      2 * (childBorderWidth +
                                      pw->paned_window.margin_width),
                                      pw->core.height, &replyWidth, 
                                      &replyHeight))
          {
            case XtGeometryYes:           
               allowed.width = request->width;
               break;

            case XtGeometryAlmost:
	       XtMakeResizeRequest(pw, replyWidth, replyHeight, NULL, NULL);
               allowed.width = replyWidth - 2 * (childBorderWidth +
                                                pw->paned_window.margin_width);
               break;

            case XtGeometryNo:
               allowed.width = pw->core.width;
               break;
          }
       }
   }

   if ((mask & CWHeight)) {
      /* try out the new size */
      ResetDHeights( pw );
      old_dheight = pane->dheight;
      pane->dheight = request->height;

      if (new_height = NeedsAdjusting(pw)) {
/* ASSUMES THAT THE CHILD WILL HONOR THE VALUES IN THE XtGeometryAlmost
   ANSWER, BECAUSE WE HAVE ALREADY ALTERED THE SIZE OF VPANE. WHAT IF THE
   CHILD CHOOSES NOT TO CHANGE?*/

          switch(AdjustPanedWindowHeight(pw, new_height, &reply_height))
          {
            case XtGeometryYes:           
              allowed.height = request->height;
              break;

            case XtGeometryAlmost:
              allowed.height = request->height - (new_height - reply_height); 
              if (allowed.height < pane->min) allowed.height = pane->min;
              break;

            case XtGeometryNo:
              return(XtGeometryNo);
          }

      }
      else
          allowed.height = request->height;

   }

   if (((request->height != allowed.height) && (mask & CWHeight)) || 
       ((request->width != allowed.width) && (mask & CWWidth)) || 
         request_altered) {
        if (mask & CWHeight) pane->dheight = old_dheight;
        *reply = allowed;
        return XtGeometryAlmost;
      }

   w->core.height = request->height;
   w->core.width = request->width;
   _XmResizeObject((RectObj)w, request->width, request->height, w->core.border_width);
   RefigureLocations(pw, PaneIndex(w), UpperPane, False);

   if (XtIsRealized((Widget)pw))
       CommitNewLocations( pw );

   return XtGeometryYes;
}

/************************************************************************
 *
 *  ClassPartInitialize
 *     Set up the fast subclassing for the widget.
 *
 ************************************************************************/

static void ClassPartInitialize(wc)
WidgetClass wc;
{
  _XmFastSubclassInit (wc, XmVPANED_BIT);
}



/************************************************************************
 *
 *  Initialize
 *     The main widget instance initialization routine.
 *
 ************************************************************************/

/* ARGSUSED */
static void Initialize(request, new, args, num_args)
  Widget request, new;
  ArgList args;
  Cardinal num_args;
{
  XmPanedWindowWidget pw = (XmPanedWindowWidget) new;

 /* Protect against empty widgets */
  pw->paned_window.pane_count = 0;
  pw->paned_window.managed_children =
                          (WidgetList) XtMalloc(XmBLOCK*sizeof(Widget));
  pw->paned_window.num_slots = XmBLOCK;
  pw->paned_window.num_managed_children = 0;
  pw->paned_window.starty = 0;
  pw->paned_window.top_pane = NULL;
  pw->paned_window.bottom_pane = NULL;
  pw->paned_window.flipgc = NULL;
  pw->paned_window.increment_count = 0;

/*************************************************************
 * NOTE: that sash_indent is made to conform to a correct size
 * during changed managed time/layout time.  Since the size of
 * the window may change we won't require that 
 * abs(sash_indent) <= width of a pane.
 ************************************************************/

  if (pw->composite.insert_position == NULL)
     pw -> composite.insert_position = (XtOrderProc) InsertOrder;
  pw -> paned_window.recursively_called = FALSE;

/* DON'T ALLOW HEIGHT/WIDTH TO BE 0, OR X DOESN'T HANDLE THIS WELL */
 
  if (pw->core.width == 0) pw->core.width = 10;
  if (pw->core.height == 0) pw->core.height = 10;

}


/************************************************************************
 *
 *  Constraint Initialize
 *
 *  This routine is called to initialize the constraint record for
 *  every child inserted into the paned_window window.
 *
 ************************************************************************/

/* ARGSUSED */
static void ConstraintInit(request, new, args, num_args)
  Widget request, new;
  ArgList args;
  Cardinal num_args;
{
  XmPanedWindowWidget pw = (XmPanedWindowWidget) XtParent(new);
  XmPanedWindowConstraintPart * pane = &(PaneInfo(new)->panedw);
  int height;
  unsigned char unit_type;

  if (!XtIsRectObj(new)) return;

  height = new->core.height;
  pane->position = 0;

  if (pane->min == 0)
    {
        _XmWarning (pw, MESSAGE4);
     	pane->min = 1;
    }

  if (pane->max == 0)
    {
        _XmWarning (pw, MESSAGE5);
     	pane->max = pane->min + 1;
    }

  if (pane->min > pane->max)
    {
        _XmWarning (pw, MESSAGE6);
     	pane->max = pane->min + 1;
    }

  /* ENFORCE MIN/MAX */
  if (height < pane->min) height = pane->min;
  if (height > pane->max) height = pane->max;

  if (height != new->core.height)
    	_XmResizeObject((RectObj)new, new->core.width, height, new->core.border_width);
}


/*************************************<->*************************************
 *
 *  Realize
 *
 *   Description:
 *   -----------
 *    Create our window, set NW gravity (Realize), set up
 *
 *
 *************************************<->***********************************/
static void Realize(w, p_valueMask, attributes)
Widget w;
Mask *p_valueMask;
XSetWindowAttributes *attributes;
{
    register XmPanedWindowWidget pw = (XmPanedWindowWidget)w;
    WidgetList children = pw->paned_window.managed_children;
    int num_children = pw->paned_window.num_managed_children;
    Widget *childP;
    Mask valueMask = *p_valueMask;

    valueMask |= CWBitGravity | CWDontPropagate;
    attributes->bit_gravity = NorthWestGravity;
    attributes->do_not_propagate_mask = ButtonPressMask|
	ButtonReleaseMask|KeyPressMask|KeyReleaseMask|PointerMotionMask;

    XtCreateWindow (w, InputOutput, CopyFromParent, valueMask, attributes);

    GetFlipGC(pw);

    /* one last time, in case we grew to try to return an
     * XtGeometryAlmost for a child, but the child decided not to grow 
     * or in case some child grew itself and we didn't hear about it.
     */
    Resize( pw );

    /* now we have to make sure all the sashs are on above their
     * panes, which means that we have to realize all our children
     * here and now.  If we realize from the beginning of the list,
     * then the sashs (which are at the end) will be Above by default. */

    for (childP = children; childP - children < num_children; childP++)
	    XtRealizeWidget( *childP );

} /* Realize */


/*************************************<->*************************************
 *
 *  Destroy
 *
 *************************************<->***********************************/

static void Destroy(w)
Widget w;
{
    XmPanedWindowWidget pw = (XmPanedWindowWidget)w;

    if (pw->paned_window.flipgc != NULL) {
       XFreeGC(XtDisplay(pw), pw->paned_window.flipgc);
       pw->paned_window.flipgc = NULL;
    }

    XtFree(pw->paned_window.managed_children);

} /* Destroy */


/***************************************************************************
 *
 * static Cardinal
 * InsertOrder (cw, args, p_num_args)
 *
 *   This function searches through the composite widget's list of children
 *   to find the correct insertion position for the new child.  If the
 *   new child is an ordinary child (not a subclass of XmSashWidget)
 *   the position returned will cause it to be inserted after the other
 *   ordinary children but before any Sashs; if the new child is a 
 *   sash the position returned will cause it to be inserted at the
 *   end of the list.  This procedure does not examine the arglist.
 *
 ************************************************************************/

static Cardinal InsertOrder (w)
Widget w;         /* Composite widget being inserted into */
{
   CompositeWidget cw = (CompositeWidget) XtParent(w);
   Cardinal i=0;

   while ((i < cw->composite.num_children) &&
          (! XmIsSash(cw->composite.children[i])) &&
          (! XmIsSeparatorGadget(cw->composite.children[i])))
                i++;
   return (i);
}



/*************************************<->*************************************
 *
 *  InsertChild()
 *
 *************************************<->***********************************/

static void InsertChild(w)
register Widget w;
{
   XmPanedWindowWidget pw = (XmPanedWindowWidget)w->core.parent;
   XmPanedWindowConstraintPart * pane = &(PaneInfo(w)->panedw);
   static XtCallbackRec SashCallback[] = { {HandleSash, NULL}, {NULL, NULL} };
   Arg args[10];
   int n;

   if (!XtIsRectObj(w)) return;

   /* 
    * Insert the child widget in the composite children list with an
    * insertion procedure exported from Manager.c, it, in turn, will
    * make use of the special insert procedure, InsertOrder, defined
    * above.  Essentially, ordinary panes are grouped together at the
    * beginning of the list of composite children, sashs are always 
    * put at the end of the list.
    */

   (*((XmManagerWidgetClass)xmManagerWidgetClass)->composite_class.insert_child)
							    (w);
  /*
   * If we are creating a sash for an ordinary pane, then just
   * return here. However, before we do, set its "isPane" flag
   * to false, meaning that this is NOT a pane; if it is a 
   * pane then set its "isPane" flag to TRUE.
   */
   if (w->core.widget_class == xmSashWidgetClass ||
       w->core.widget_class == xmSeparatorGadgetClass ||
       !XtIsRectObj(w)) {
      pane->isPane = FALSE;
      pane->separator = NULL;
      pane->sash = NULL;
      return;
    }
    pane->isPane = TRUE;

    n = 0;
    XtSetArg(args[n], XmNwidth, pw->core.width); n++;
    XtSetArg(args[n], XmNborderWidth, 0); n++;
    XtSetArg(args[n], XmNhighlightThickness, 0); n++;
    XtSetArg(args[n], XmNseparatorType, XmSHADOW_ETCHED_IN); n++;
    XtSetArg(args[n], XmNmargin, 0); n++;
    XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
    XtSetArg(args[n], XmNnavigationType, (XtArgVal) XmNONE); n++;
    pane->separator = XtCreateWidget("separator", xmSeparatorGadgetClass,
                                     (Widget)pw, args, n);
    PaneInfo(pane->separator)->panedw.separator = w;

   /* If we create a sash then have the pane's constraint rec point
    * to the sash, ignore the constraint rec of the sash (Yes, it 
    * gets one).  If we don't create a sash for the pane, just set
    * that field to NULL.   
    */
    SashCallback[0].closure = (caddr_t)w;

    n = 0;
    XtSetArg(args[n], XmNwidth, pw->paned_window.sash_width); n++;
    XtSetArg(args[n], XmNheight, pw->paned_window.sash_height); n++;
    XtSetArg(args[n], XmNshadowThickness, 
                pw->paned_window.sash_shadow_thickness); n++;
    XtSetArg(args[n], XmNcallback, (XtArgVal) SashCallback); n++;
    XtSetArg(args[n], XmNunitType, (XtArgVal) XmPIXELS); n++;
    XtSetArg(args[n], XmNtraversalOn, (XtArgVal) True); n++;
    XtSetArg(args[n], XmNnavigationType, (XtArgVal) XmTAB_GROUP); n++;
    pane->sash = XtCreateWidget("sash", xmSashWidgetClass, (Widget)pw, args, n);

    n = 0;
    XtSetArg(args[n], XmNnavigationType, (XtArgVal) XmTAB_GROUP); n++;
    XtSetValues(w, args, n);
      
    PaneInfo(pane->sash)->panedw.sash = w;

} /* InsertChild */




/*************************************<->*************************************
 *
 * ChangeManaged
 *
 *************************************<->***********************************/

static void ChangeManaged(w)
   Widget w;
{
   register XmPanedWindowWidget pw = (XmPanedWindowWidget)w;
   register Widget *childP;
   register int i;
   Widget *children;
   int num_children = pw->composite.num_children;
   Widget sash, separator;
   Dimension width = 0, height = 0;
   Dimension childWidth = 0, childBorderWidth = 0, newWidth = 0;
   Dimension reply_width, reply_height, needed;
   int num_panes = 0;
   XmPanedWindowConstraintPart * pane;
   XtGeometryResult result;

  /* 
   * THIS PREVENTS US FROM RE-ENTERING THIS CODE AS WE MANAGE/UNMANAGE
   * THE SASHES
   */
   if (pw->paned_window.recursively_called++)  return;

  /* 
   * NOW RECOMPUTE THE LIST OF MANAGED CHILDREN.
   */
   ReManageChildren((XmManagerWidget)pw);
   children = pw->paned_window.managed_children;

  /*
   * COUNT THE NUMBER OF PANES THAT ARE MANAGED.
   */
   childP = children;
   while ((num_panes < pw->paned_window.num_managed_children) &&
             XtIsRectObj(*childP) &&
             (*childP)->core.widget_class != xmSashWidgetClass &&
             (*childP++)->core.widget_class != xmSeparatorGadgetClass)
                  num_panes++;

   pw->paned_window.pane_count = num_panes;


  /* 
   * SET WIDTH OF PANED WINDOW EQUAL TO THAT OF WIDEST CHILD
   */

   childWidth = 0;
   childBorderWidth = 0;
   height = 0;
   for (childP = children, i = 0; i < num_panes; childP++, i++)
   {
       pane = &(PaneInfo(*childP)->panedw);
       if ((*childP)->core.width + (*childP)->core.border_width > 
            childWidth + childBorderWidth) 
          childWidth = (*childP)->core.width;
          childBorderWidth = (*childP)->core.border_width;
       if ((*childP)->core.height < pane->min)
          _XmResizeObject((RectObj)*childP, (*childP)->core.width,
                          pane->min, (*childP)->core.border_width);
       if ((*childP)->core.height > pane->max)
          _XmResizeObject((RectObj)*childP, (*childP)->core.width,
                          pane->max, (*childP)->core.border_width);
       height += (*childP)->core.height + 2*(*childP)->core.border_width;
   }

   if (childWidth < 1) childWidth = 1;

  /*
   * NOW SCAN THE COMPOSITE LIST OF CHILDREN, AND MAKE SURE
   * THAT THEIR MANAGEMENT SETTING REFLECTS THAT OF THEIR PANE.
   */

   for (childP = pw->composite.children, i = 0; 
                     i < num_children; childP++, i++) 
    {
      if (! IsPane(*childP)) break;  /* jump out of loop */
      sash = PaneInfo(*childP)->panedw.sash;
      separator = PaneInfo(*childP)->panedw.separator;

      /* Realize child now so it won't get realized and put on the 
         top of the stack, above the sash, when it is realized later */
      if (XtIsRealized(pw) && XtIsManaged(*childP))
          XtRealizeWidget(*childP);

      /* KEEP SOME RECORD OF DESIRED HEIGHT */
      PaneInfo(*childP)->panedw.dheight = (*childP)->core.height;

      newWidth = childWidth + 2*(childBorderWidth -
                                 (*childP)->core.border_width);

      _XmResizeObject((RectObj)*childP, newWidth, (*childP)->core.height,
		      (*childP)->core.border_width);
       if ((XtIsManaged(*childP)) && (*childP != children[num_panes-1])) {
              if (separator && pw->paned_window.separator_on) {
                  if (!XtIsManaged(separator)) XtManageChild(separator);
  	          if (XtIsRealized(separator))
		    XRaiseWindow(XtDisplay(separator), XtWindow(separator));
	      }

              if (sash) {
                 if (PaneInfo(*childP)->panedw.min !=
	 	     PaneInfo(*childP)->panedw.max) {
                    if (!XtIsManaged(sash)) XtManageChild(sash);
  	            if (XtIsRealized(sash))
		       XRaiseWindow( XtDisplay(sash), XtWindow(sash) );
	         } else
                    if (XtIsManaged(sash)) XtUnmanageChild(sash);
              }
	} else
            if (sash)
               if (XtIsManaged(sash)) XtUnmanageChild(sash);
     }

  /* NOW CHANGEMANAGED CAN BE ENTERED NORMALLY */
   pw->paned_window.recursively_called = False;

   /*
    * TRAVERSE MANAGED PANES AND SET THE POSITION FIELD IN THE CONSTRAINT
    * RECORD TO 0, 1, 2, 3, 4 ETC.
    */
   
   childP = pw->paned_window.managed_children;
   for (i = 0; i < pw->paned_window.pane_count; childP++)
      (PaneInfo(*childP))->panedw.position = i++;
        

   width = childWidth;

   if (height < 1) height = 1;

   width += 2*(pw->paned_window.margin_width + childBorderWidth);
   height += pw->paned_window.spacing*(num_panes-1) +
             2*pw->paned_window.margin_height;

   result = XtMakeResizeRequest(w, width, height,
                                  &reply_width, &reply_height);

   if (result == XtGeometryAlmost)
      XtMakeResizeRequest(w, reply_width, reply_height, &width, &height);

   if (result == XtGeometryYes || result == XtGeometryAlmost) {
       pw->core.width = width;
       pw->core.height = height;
       
      /* see if the height of the Paned Window
         needs to be adjusted to fit all the panes */
      if (needed = NeedsAdjusting(pw))
         AdjustPanedWindowHeight(pw, needed, &reply_height);

   }

   ResetDHeights( pw );
   
   if (XtIsRealized((Widget)pw))
      RefigureLocationsAndCommit(pw, 0, UpperPane, False);

   _XmNavigChangeManaged((Widget)pw);

} /* ChangeManaged */



/*************************************<->*************************************
 *
 *  Resize
 *
 *************************************<->***********************************/
static void Resize(pw)
    XmPanedWindowWidget pw;
{
    RefigureLocationsAndCommit( pw, pw->paned_window.pane_count - 1, 
                                                LowerPane, True);
} /* Resize */



/*************************************<->*************************************
 *
 *  SetValues
 *   -----------------
 *
 *************************************<->***********************************/
static Boolean SetValues(oldpw, requestpw, newpw)
    XmPanedWindowWidget oldpw, requestpw, newpw;
{
   Boolean returnFlag = False;
   WidgetList children = newpw->composite.children;
   register Widget *childP;
   int num_children = newpw->composite.num_children;
   static Arg sashargs[3];
   int i, x, y;
   int n = 0;

   if (oldpw->core.background_pixel != newpw->core.background_pixel)
     {
       if (newpw->paned_window.flipgc != NULL) {
          XFreeGC(XtDisplay(newpw), newpw->paned_window.flipgc);
       }
       GetFlipGC(newpw);
       returnFlag = True;
     }        

   if (newpw->paned_window.sash_width == 0)
      newpw->paned_window.sash_width = oldpw->paned_window.sash_width;

   if (oldpw->paned_window.sash_width != newpw->paned_window.sash_width)
    {
       XtSetArg(sashargs[n], XmNwidth, newpw->paned_window.sash_width); n++;
    }

   if (newpw->paned_window.sash_height == 0)
      newpw->paned_window.sash_height = oldpw->paned_window.sash_height;

   if (oldpw->paned_window.sash_height != newpw->paned_window.sash_height)
    {
       XtSetArg(sashargs[n], XmNheight, newpw->paned_window.sash_height); n++;
    }

   if (oldpw->paned_window.sash_shadow_thickness != 
           newpw->paned_window.sash_shadow_thickness)
    {
       XtSetArg(sashargs[n], XmNshadowThickness, 
                newpw->paned_window.sash_shadow_thickness); n++;
    }

   if (oldpw->paned_window.separator_on != newpw->paned_window.separator_on &&
       num_children > 2) {
      WidgetList sep_children;
      Cardinal num_separators = 0;

     /* This should be more than enough space */
      sep_children = (WidgetList) XtMalloc((num_children/3) * sizeof(Widget));

      for (childP = children, i = 0; i < num_children; childP++, i++) {
          if (IsPane(*childP)) {
             Widget separator = PaneInfo(*childP)->panedw.separator;
             if (separator) {
                sep_children[num_separators] = separator;
                num_separators++;
             }
          }
       }
      if (num_separators != 0) {
         if (newpw->paned_window.separator_on)
            XtManageChildren((WidgetList) sep_children, num_separators);
         else
            XtUnmanageChildren((WidgetList) sep_children, num_separators);
      }
         
      XtFree((char *)sep_children);
   }


   if (oldpw->paned_window.sash_indent != newpw->paned_window.sash_indent ||
       oldpw->paned_window.margin_width != newpw->paned_window.margin_width ||
       oldpw->paned_window.margin_height != newpw->paned_window.margin_height ||
       oldpw->paned_window.sash_width != newpw->paned_window.sash_width ||
       oldpw->paned_window.sash_height != newpw->paned_window.sash_height ||
       oldpw->paned_window.sash_shadow_thickness !=
				 newpw->paned_window.sash_shadow_thickness ||
       oldpw->paned_window.spacing != newpw->paned_window.spacing) {
      for (childP = children, i = 0; i < num_children; childP++, i++) {
          if (!XmIsSash(*childP) && !XmIsSeparatorGadget(*childP)) {
             register XmPanedWindowConstraintPart * pane =
                                    &(PaneInfo(*childP)->panedw);
             register Widget sash = pane->sash;

             if (sash)  /* IF THIS IS NOT NULL */
             {
                /* Send Down Changes to Sash */ 
                if (n != 0)  /* something is in the arglist */
                   XtSetValues((Widget)sash, sashargs, n); 

	        /* Move and Display the Sash */
                if (newpw->paned_window.sash_indent < 0)
  	           x = newpw->core.width + newpw->paned_window.sash_indent
	               - sash->core.width - sash->core.border_width*2;
                 else
                   x = newpw->paned_window.sash_indent;


                 /* PREVENT SASH FROM DISAPPEARING FROM SCREEN */
                 if ((x > (newpw->core.width - sash->core.width)) || (x < 0))
                    x = 0;
   
	         y = (*childP)->core.y + (*childP)->core.height +
	             2 * (*childP)->core.border_width +
		     newpw->paned_window.spacing/2 - sash->core.height/2 -
		     sash->core.border_width;

                 _XmMoveObject((RectObj)sash, x, y);
              }
          }
     }
   }
   if (oldpw->paned_window.margin_width != newpw->paned_window.margin_width) {
      newpw->core.width = newpw->core.width + 
                          ((2 * newpw->paned_window.margin_width) -
                           (2 * oldpw->paned_window.margin_width));
      returnFlag = True;
   }
   if ((oldpw->paned_window.spacing != newpw->paned_window.spacing ||
       oldpw->paned_window.margin_height != newpw->paned_window.margin_height ||
       (requestpw->paned_window.refiguremode &&
				 !(oldpw->paned_window.refiguremode))) &&
       XtIsRealized(newpw)) {
      Dimension needed;

      if (needed = NeedsAdjusting(newpw)) {
         newpw->core.height = needed;
         returnFlag = True;
      } 
   }

   return(returnFlag);
} /* SetValues */



/*************************************<->*************************************
 *
 *  PaneSetValues
 *
 *
 *************************************<->***********************************/
/* ARGSUSED */
static Boolean PaneSetValues(old, request, new)
    Widget old, request, new;
{
   XmPanedWindowWidget pw = (XmPanedWindowWidget)new->core.parent;
   static XtCallbackRec SashCallback[] = { {HandleSash, NULL}, {NULL, NULL} };
   static Arg sashargs[5];
   int count;
   unsigned char unit_type;
   XmPanedWindowConstraintPart * old_pane = &(PaneInfo(old)->panedw);
   XmPanedWindowConstraintPart * new_pane = &(PaneInfo(new)->panedw);

   if (!XtIsRectObj(new)) return(FALSE);

   if (old_pane->min != new_pane->min || old_pane->max != new_pane->max)
    {
        
       if (new_pane->min < 1)
	{
           _XmWarning (pw, MESSAGE4);
	   new_pane->min = old_pane->min;
	}

       if (new_pane->max < 1)
	{
           _XmWarning (pw, MESSAGE5);
	   new_pane->max = old_pane->max;
	}

       if (new_pane->min > new_pane->max)
	{
           _XmWarning (pw, MESSAGE6);
	   new_pane->min = old_pane->min;
	   new_pane->max = old_pane->max;
	}

       if ((new_pane->min == new_pane->max) && 
            (new_pane->sash != NULL))
      	{
           XtUnmanageChild(new_pane->sash);
      	} 
       else
      	{
           count =pw->paned_window.pane_count -1;
           if (new_pane->position != count) { 
              if (new_pane->separator == NULL) {
	         new_pane->separator = XtCreateWidget("separator", 
                                                     xmSeparatorGadgetClass,
                                                     (Widget)pw, NULL, 0);

                 if (XtIsRealized(new)) XtRealizeWidget(new_pane->separator);
         
	      }
              if (pw->paned_window.separator_on)
                       XtManageChild(new_pane->separator);
              if (new_pane->min != new_pane->max) {
                 if (new_pane->sash == NULL) {
	            SashCallback[0].closure = (caddr_t)new;
                  
                    XtSetArg(sashargs[0], XmNwidth,
			     pw->paned_window.sash_width);
                    XtSetArg(sashargs[1], XmNheight,
			     pw->paned_window.sash_height);
                    XtSetArg(sashargs[2], XmNshadowThickness, 
                             pw->paned_window.sash_shadow_thickness);
                    XtSetArg(sashargs[3], XmNcallback, (XtArgVal) SashCallback);
                    XtSetArg(sashargs[4], XmNunitType, (XtArgVal) XmPIXELS);
	            new_pane->sash = XtCreateManagedWidget("sash", 
                                                  xmSashWidgetClass, (Widget)pw,
                                                  sashargs, XtNumber(sashargs));

                    if (XtIsRealized(new))  XtRealizeWidget(new_pane->sash);
          
	          } else
                    XtManageChild(new_pane->sash);
               }
	    }
	}

	if (new_pane->min > new->core.height) new->core.height = new_pane->min;
	if (new_pane->max < new->core.height) new->core.height = new_pane->max;
	old_pane->min = new_pane->min;
	old_pane->max = new_pane->max;

        return(True);
   }

   return False;
} 



/*************************************<->*************************************
 *
 *  ConstraintDestroy
 *
 *   Description:
 *   -----------
 *    Destroy the sash of any pane which is being destroyed.
 *
 *
 *************************************<->***********************************/
static void ConstraintDestroy(w)
Widget w;
{
   if (!XtIsRectObj(w)) return;

   /* 
    * If this is an ordinary pane,  delete its sash (if it has one)
    * and separator (if it has one) and then invoke the standard
    * inherited delete child routines.
    */
   if ((w->core.widget_class != xmSashWidgetClass) &&
       (w->core.widget_class != xmSeparatorGadgetClass) &&
       XtIsRectObj(w))
   {
        if (PaneInfo(w)->panedw.sash != NULL)
          XtDestroyWidget(PaneInfo(w)->panedw.sash);

        if (PaneInfo(w)->panedw.separator != NULL)
          XtDestroyWidget(PaneInfo(w)->panedw.separator);
   }
}


/*************************************<->*************************************
 *
 *  void AdjustGC(pw)
 *          XmPanedWindowWidget  pw;
 *
 *   Description:
 *   -----------
 *   Set up the clip regions so the track lines do not draw on
 *   top of the sashes.
 *
 *************************************<->***********************************/

static void AdjustGC(pw)
    XmPanedWindowWidget pw;
{
    XRectangle clip_rect;
    register int i;
    Region sash_region, clip_region;
    if (pw->composite.num_children > 0) {
    	sash_region = XCreateRegion();
    	clip_region = XCreateRegion();

    	/* find all the managed sashes and add their area to the sash region */
    	for (i = 0; i < pw->composite.num_children; i++)
    	{
        	if (XmIsSash(pw->composite.children[i]) &&
            		XtIsManaged(pw->composite.children[i])) {
    	   	clip_rect.width = pw->composite.children[i]->core.width;
    	   	clip_rect.height = pw->composite.children[i]->core.height;
    	   	clip_rect.x = pw->composite.children[i]->core.x;
    	   	clip_rect.y = pw->composite.children[i]->core.y;
	   	XUnionRectWithRegion(&clip_rect, sash_region, sash_region);
        	}
    	}

    	/* set up the initial clip region */
    	clip_rect.width = pw->core.width;
    	clip_rect.height = pw->core.height;
    	clip_rect.x = 0;
    	clip_rect.y = 0;
    	XUnionRectWithRegion(&clip_rect, clip_region, clip_region);

    	/* remove the sash regions from the clip region */
    	XSubtractRegion(clip_region, sash_region, clip_region);
	
    	/* set the clip region, so the track
	   lines won't be draw on the sashes */
    	XSetRegion(XtDisplay(pw), pw->paned_window.flipgc, clip_region);

    	/* remove the clip regions */
    	XDestroyRegion(sash_region);
    	XDestroyRegion(clip_region);
   }
}


/*************************************<->*************************************
 *
 *  void GetFlipGC(pw)
 *          XmPanedWindowWidget  pw;
 *
 *   Description:
 *   -----------
 *   Create a GC which can be used to draw/erase track lines when the
 *   the size of the panes is being changed.
 *
 *************************************<->***********************************/

static void GetFlipGC(pw)
    XmPanedWindowWidget pw;
{
    unsigned long valuemask = GCForeground | GCSubwindowMode | GCFunction;
    XGCValues	values;
    Drawable drawable = RootWindowOfScreen(XtScreen(pw));

    values.foreground	= pw->core.background_pixel;
    values.subwindow_mode = IncludeInferiors;
    values.function = GXinvert;
    pw->paned_window.flipgc = XCreateGC(XtDisplay(pw), drawable,
					valuemask, &values);
    AdjustGC(pw);

}

/*************************************<->*************************************
 *
 *  Widget XmCreatePanedWindow(parent, name, args, argCount)
 *      Widget 	parent;
 *      char 	*name;
 *      ArgList	args;
 *      int	argCount;
 *
 *   Description:
 *   -----------
 *   A convenience routine to be used in creating a new PanedWindow
 *   manager widget.
 *
 *************************************<->***********************************/


#ifdef _NO_PROTO
Widget XmCreatePanedWindow(parent, name, args, argCount)
   Widget 	parent;
   char 	*name;
   ArgList	args;
   Cardinal	argCount;
#else /* _NO_PROTO */
Widget XmCreatePanedWindow (Widget parent, char *name, ArgList args, Cardinal argCount)
#endif /* _NO_PROTO */
{
    return (XtCreateWidget(name, xmPanedWindowWidgetClass,
                                   parent, args, argCount));
}

