#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)Form.c	3.21 91/01/10";
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
#include <stdio.h>
#include <Xm/XmP.h>
#include <Xm/BulletinBP.h>
#include <Xm/DialogS.h>
#include <Xm/FormP.h>


#define MESSAGE1 "Fraction base cannot be zero."
#define MESSAGE2 "Incorrect form attachment type."
#define MESSAGE3 "Cannot set constraints for non-resizable widget."
#define MESSAGE4 "Attachment widget must not be null."
#define MESSAGE5 "Circular dependency in Form children."
#define MESSAGE6 "Edge attached to a widget but no widget specified."
#define MESSAGE7 "Bailed out of edge synchronization after 10,000 iterations.\nCheck for contradictory constraints on the children of this form."
#define MESSAGE8 "Attachment widget must be have same parent as widget."
	

/*  Useful macros  */

#define Value(a) (really ? (a)->value : (a)->tempValue)

#define AssignValue(a, v) (really ? (a->value = (v)) : (a->tempValue = (v)))

#define GetFormConstraint(w) \
	(&((XmFormConstraintPtr) (w)->core.constraints)->form)

#define NE(x) (oldc->x != newc->x)

#define ANY(x) (NE(att[LEFT].x) || NE(att[RIGHT].x) || \
		NE(att[TOP].x)  || NE(att[BOTTOM].x))


/*  Class and internal functions  */

static void    ClassPartInitialize();
static void    Initialize();
static void    Redisplay();
static void    Resize();
static void    Realize();
static Boolean SetValues();
static void SetValuesAlmost();

static XtGeometryResult GeometryManager();
static XtGeometryResult QueryGeometry();
static void             ChangeManaged();
static void             DeleteChild();

static void    ConstraintInitialize();
static Boolean ConstraintSetValues();

static void PlaceChildren();
static void CalcFormSize();
static void CheckConstraints();
static void CalcEdgeValues();
static void CalcEdgeValue();
static void ComputeAttachment();
static void SortChildren();
static Boolean ChangeIfNeeded();
static int GetFormOffset();
static void MarginWidthOut();
static void MarginHeightOut();
static void _FromTopOffset();
static void _FromBottomOffset();
static void _FromLeftOffset();
static void _FromRightOffset();


/* convenient magic numbers */

#define RESOURCE_DEFAULT  (Dimension) -1
#define MAX_LOOP 10000

#define LEFT	0
#define RIGHT	1
#define TOP	2
#define BOTTOM	3


static XtResource resources[] = 
{
   {
	XmNmarginWidth, XmCMarginWidth, XmRHorizontalDimension,
	sizeof(Dimension),
	XtOffset(XmFormWidget, bulletin_board.margin_width),
	XmRImmediate, (caddr_t) RESOURCE_DEFAULT
   },
   {
	XmNmarginHeight, XmCMarginHeight, XmRVerticalDimension,
	sizeof(Dimension),
	XtOffset(XmFormWidget, bulletin_board.margin_height),
	XmRImmediate, (caddr_t) RESOURCE_DEFAULT
   },
   {
      XmNhorizontalSpacing, XmCSpacing, XmRHorizontalDimension,
	  sizeof(Dimension),
      XtOffset (XmFormWidget, form.horizontal_spacing), 
      XmRImmediate, (caddr_t) 0
   },
   {
      XmNverticalSpacing, XmCSpacing, XmRVerticalDimension,
	  sizeof(Dimension),
      XtOffset (XmFormWidget, form.vertical_spacing), 
      XmRImmediate, (caddr_t) 0
   },

   {
      XmNfractionBase, XmCMaxValue, XmRInt, sizeof(int),
      XtOffset (XmFormWidget, form.fraction_base), 
      XmRImmediate, (caddr_t) 100
   },

   {
      XmNrubberPositioning, XmCRubberPositioning, XmRBoolean, 
	  sizeof(Boolean),
      XtOffset (XmFormWidget, form.rubber_positioning), 
      XmRImmediate, (caddr_t) False
   },

};


/*  Definition for resources that need special processing in get values  */

static XmSyntheticResource syn_resources[] =
{
   { XmNmarginWidth,
     sizeof (Dimension),
     XtOffset (XmFormWidget, bulletin_board.margin_width), 
	 MarginWidthOut,
	 _XmToHorizontalPixels },

   { XmNmarginHeight,
     sizeof (Dimension),
     XtOffset (XmFormWidget, bulletin_board.margin_height), 
	 MarginHeightOut,
	 _XmToVerticalPixels },

   { XmNhorizontalSpacing,
     sizeof (Dimension),
     XtOffset (XmFormWidget, form.horizontal_spacing), 
     _XmFromHorizontalPixels,
	 _XmToHorizontalPixels },

   { XmNverticalSpacing,
     sizeof (Dimension),
     XtOffset (XmFormWidget, form.vertical_spacing), 
     _XmFromVerticalPixels,
	 _XmToVerticalPixels },
};


/*  The constraint resource list  */

static XtResource constraints[] =
{
   {
      XmNtopAttachment, XmCAttachment, XmRAttachment, sizeof(unsigned char),
      XtOffset(XmFormConstraintPtr, form.att[TOP].type),
      XmRImmediate, (caddr_t) XmATTACH_NONE
   },

   {
      XmNbottomAttachment, XmCAttachment, XmRAttachment,
      sizeof(unsigned char),
      XtOffset(XmFormConstraintPtr, form.att[BOTTOM].type),
      XmRImmediate, (caddr_t) XmATTACH_NONE
   },

   {
      XmNleftAttachment, XmCAttachment, XmRAttachment,
      sizeof(unsigned char),
      XtOffset(XmFormConstraintPtr, form.att[LEFT].type),
      XmRImmediate, (caddr_t) XmATTACH_NONE
   },

   {
      XmNrightAttachment, XmCAttachment, XmRAttachment,
      sizeof(unsigned char),
      XtOffset(XmFormConstraintPtr, form.att[RIGHT].type),
      XmRImmediate, (caddr_t) XmATTACH_NONE
   },

   {
      XmNtopWidget, XmCWidget, XmRWindow, sizeof(Widget),
      XtOffset(XmFormConstraintPtr, form.att[TOP].w),
      XmRWindow, (caddr_t) NULL
   },

   {
      XmNbottomWidget, XmCWidget, XmRWindow, sizeof(Widget),
      XtOffset(XmFormConstraintPtr, form.att[BOTTOM].w),
      XmRWindow, (caddr_t) NULL
   },

   {
      XmNleftWidget, XmCWidget, XmRWindow, sizeof(Widget),
      XtOffset(XmFormConstraintPtr, form.att[LEFT].w),
      XmRWindow, (caddr_t) NULL
   },

   {
      XmNrightWidget, XmCWidget, XmRWindow, sizeof(Widget),
      XtOffset(XmFormConstraintPtr, form.att[RIGHT].w),
      XmRWindow, (caddr_t) NULL
   },

   {
      XmNtopPosition, XmCAttachment, XmRInt, sizeof(int),
      XtOffset(XmFormConstraintPtr, form.att[TOP].percent),
      XmRImmediate, (caddr_t) 0
   },

   {
      XmNbottomPosition, XmCAttachment, XmRInt, sizeof(int),
      XtOffset(XmFormConstraintPtr, form.att[BOTTOM].percent),
      XmRImmediate, (caddr_t) 0
   },

   {
      XmNleftPosition, XmCAttachment, XmRInt, sizeof(int),
      XtOffset(XmFormConstraintPtr, form.att[LEFT].percent),
      XmRImmediate, (caddr_t) 0
   },

   {
      XmNrightPosition, XmCAttachment, XmRInt, sizeof(int),
      XtOffset(XmFormConstraintPtr, form.att[RIGHT].percent),
      XmRImmediate, (caddr_t) 0
   },

   {
      XmNtopOffset, XmCOffset, XmRInt, sizeof(int),
      XtOffset(XmFormConstraintPtr, form.att[TOP].offset),
      XmRImmediate, (caddr_t) RESOURCE_DEFAULT
   },

   {
      XmNbottomOffset, XmCOffset, XmRInt, sizeof(int),
      XtOffset(XmFormConstraintPtr, form.att[BOTTOM].offset),
      XmRImmediate, (caddr_t) RESOURCE_DEFAULT
   },

   {
      XmNleftOffset, XmCOffset, XmRInt, sizeof(int),
      XtOffset(XmFormConstraintPtr, form.att[LEFT].offset),
      XmRImmediate, (caddr_t) RESOURCE_DEFAULT
   },

   {
      XmNrightOffset, XmCOffset, XmRInt, sizeof(int),
      XtOffset(XmFormConstraintPtr, form.att[RIGHT].offset),
      XmRImmediate, (caddr_t) RESOURCE_DEFAULT
   },

   {
      XmNresizable, XmCBoolean, XmRBoolean, sizeof(Boolean),
      XtOffset(XmFormConstraintPtr, form.resizable),
      XmRImmediate, (caddr_t) True
   }
};


/*  Definition for constraint resources that need special  */
/*  processing in get values                               */

static XmSyntheticResource syn_constraint_resources[] =
{

   { XmNtopOffset,
     sizeof (int),
     XtOffset(XmFormConstraintPtr, form.att[TOP].offset),
     _FromTopOffset,
	 _XmToVerticalPixels },

   {  XmNbottomOffset,
      sizeof (int),
      XtOffset(XmFormConstraintPtr, form.att[BOTTOM].offset),
     _FromBottomOffset,
	 _XmToVerticalPixels },

   {  XmNleftOffset,
      sizeof (int),
      XtOffset(XmFormConstraintPtr, form.att[LEFT].offset),
     _FromLeftOffset,
	 _XmToHorizontalPixels },

   {  XmNrightOffset,
      sizeof (int),
      XtOffset(XmFormConstraintPtr, form.att[RIGHT].offset),
     _FromRightOffset,
	 _XmToHorizontalPixels },
};



/*  Static initialization of the attached dialog box widget class record.  */

externaldef(xmformclassrec) XmFormClassRec xmFormClassRec =
{
   {                    /* core_class fields    */
      (WidgetClass) &xmBulletinBoardClassRec,   /* superclass   */
      "XmForm",                 /* class_name           */
      sizeof(XmFormRec),        /* widget_size          */
      NULL,                     /* class_initialize */
      ClassPartInitialize,      /* class init part proc */
      False,                    /* class_inited         */
      Initialize,               /* initialize           */
      NULL,                     /* initialize_notify    */
      Realize,                  /* realize              */
      NULL,                     /* actions              */
      0,                        /* num_actions          */
      resources,                /* resources            */
      XtNumber(resources),      /* num_resources        */
      NULLQUARK,                /* xrm_class            */
      False,                    /* compress_motion  */
      XtExposeCompressMaximal,  /* compress_exposure    */
      False,                    /* compress_enterleave  */
      False,                    /* visible_interest     */
      NULL,                     /* destroy              */
      Resize,                   /* resize               */
      Redisplay,                /* expose               */
      SetValues,                /* set_values           */
      NULL,                     /* set_values_hook      */
      SetValuesAlmost,          /* set_values_almost    */
      NULL,                     /* get_values_hook      */
      NULL,                     /* accept_focus         */
      XtVersion,                /* version      */
      NULL,                     /* callback_private */
      XtInheritTranslations,    /* tm_table             */
      QueryGeometry,            /* Query Geometry proc  */
      NULL,                     /* disp accelerator     */
      NULL,                     /* extension            */    
   },

   {                    /* composite_class fields */
      GeometryManager,      /* geometry_manager       */
      ChangeManaged,        /* change_managed         */
      XtInheritInsertChild, /* insert_child           */
      DeleteChild,          /* delete_child           */
      NULL,                 /* extension              */
   },

   {                    /* constraint_class fields */
      constraints,                  /* constraint resource     */
      XtNumber(constraints),        /* number of constraints   */
      sizeof(XmFormConstraintRec),  /* size of constraint      */
      ConstraintInitialize,         /* initialization          */
      NULL,                         /* destroy proc            */
      ConstraintSetValues,          /* set_values proc         */
      NULL,                         /* extension               */
   },

   {                        /* manager_class fields   */
      XtInheritTranslations,                /* translations           */
      syn_resources,                        /* syn_resources          */
      XtNumber(syn_resources),              /* num_syn_resources      */
      syn_constraint_resources,             /* syn_cont_resources     */
      XtNumber(syn_constraint_resources),   /* num_syn_cont_resources */
      XmInheritParentProcess,               /* parent_process         */
      NULL,                                 /* extension              */
   },

   {                        /* bulletin_board_class fields */
      FALSE,                                /* always_install_accelerators */
      NULL,                                 /* geo_matrix_create  */
      XtInheritFocusMovedProc,              /* focus_moved_proc   */
      NULL,                                 /* extension          */
   },

   {                        /* form_class fields  */
      NULL,                 /* extension          */
   }
};


externaldef(xmformwidgetclass) WidgetClass 
	xmFormWidgetClass = (WidgetClass) &xmFormClassRec;


static void _FromTopOffset (w, offset, value)
Widget w;
int offset;
XtArgVal *value;
        {
                XmFormConstraint c;

                c = GetFormConstraint(w);

                if (c->att[TOP].offset == RESOURCE_DEFAULT)
                        *value = 0;
                else
                        _XmFromVerticalPixels(w, offset, value);
        }

static void _FromBottomOffset (w, offset, value)
Widget w;
int offset;
XtArgVal *value;
        {
                XmFormConstraint c;

                c = GetFormConstraint(w);

                if (c->att[BOTTOM].offset == RESOURCE_DEFAULT)
                        *value = 0;
                else
                        _XmFromVerticalPixels(w, offset, value);
        }

static void _FromLeftOffset (w, offset, value)
Widget w;
int offset;
XtArgVal *value;
        {
                XmFormConstraint c;

                c = GetFormConstraint(w);

                if (c->att[LEFT].offset == RESOURCE_DEFAULT)
                        *value = 0;
                else
                        _XmFromHorizontalPixels(w, offset, value);
        }

static void _FromRightOffset (w, offset, value)
Widget w;
int offset;
XtArgVal *value;
        {
                XmFormConstraint c;

                c = GetFormConstraint(w);

                if (c->att[RIGHT].offset == RESOURCE_DEFAULT)
                        *value = 0;
                else
                        _XmFromHorizontalPixels(w, offset, value);
        }


static void MarginWidthOut (fw, offset, value)
XmFormWidget fw;
int offset;
XtArgVal *value;
{
	if (fw->bulletin_board.margin_width == RESOURCE_DEFAULT)
		*value = 0;
	else
		_XmFromHorizontalPixels((Widget)fw, offset, value);
}


static void MarginHeightOut (fw, offset, value)
XmFormWidget fw;
int offset;
XtArgVal *value;
{
	if (fw->bulletin_board.margin_height == RESOURCE_DEFAULT)
		*value = 0;
	else
		_XmFromVerticalPixels((Widget)fw, offset, value);
}

/************************************************************************
 *
 *  ClassPartInitialize
 *	Set up the fast subclassing.
 *
 ************************************************************************/

static void ClassPartInitialize(wc)
WidgetClass wc;

{
   _XmFastSubclassInit (wc, XmFORM_BIT);
}




/************************************************************************
 *
 *  CalcFormSizeWithChange
 *	Find size of a bounding box which will include all of the 
 *	children, including the child which may change
 *
 ************************************************************************/

static Boolean SyncEdges(fw, last_child, form_width, form_height,
	instigator, geometry)
XmFormWidget fw;
Widget last_child;
Dimension *form_width, *form_height;
Widget instigator;
XtWidgetGeometry *geometry;
{
	register Widget child;
	register XmFormConstraint c;
	long int loop_count;
	Dimension tmp_w = *form_width, tmp_h = *form_height;
	Dimension sav_w, sav_h;
	Boolean settled = FALSE;
	Boolean finished = TRUE;

	sav_w = tmp_w;
	sav_h = tmp_h;

	loop_count = 0;
	while (!settled)
	{
		/*
		 * Contradictory constraints can cause the constraint
		 * processing to go into endless oscillation.  This means that
		 * proper exit condition for this loop is never satisfied.
		 * But, infinite loops are a bad thing, even if is the result
		 * of a careless user.  We therefore have added a loop counter
		 * to ensure that this loop will terminate.
		 *
		 * There are problems with this however.  In the worst case
		 * this procedure could need to loop fw->composite.num_children!
		 * times.  Unfortunately, numbers like 100! don't fit integer
		 * space well; neither will current architectures complete that
		 * many loops before the sun burns out.
		 *
		 * Soooo, we will wait for an arbitrarily large number of
		 * iterations to go by before we give up.  This allows us to
		 * claim that the procedure will always complete, and the number
		 * is large enough to accomodate all but the very large and
		 * very pathological Form widget configurations.
		 *
		 * This is gross, but it's either do this or risk truly
		 * infinite loops.
		 */

		if (loop_count++ > MAX_LOOP)
			break;

		for (child = fw->form.first_child;
			child != NULL;
			child = c->next_sibling) 
		{
			if (!XtIsManaged (child))
				break;

			c = GetFormConstraint(child);

			CalcEdgeValues(child, FALSE, instigator, geometry, 
				&tmp_w, &tmp_h);

			if (child == last_child)
				break;
		}

		if ((sav_w == tmp_w) && (sav_h == tmp_h))
			settled = TRUE;
		else
		{
			sav_w = tmp_w;
			sav_h = tmp_h;
		}
	}


	if (loop_count > MAX_LOOP)
	{
		_XmWarning(fw, MESSAGE7);
		finished = FALSE;
	}

	*form_width = sav_w;
	*form_height = sav_h;

	return(finished);
}



static Boolean CalcFormSizeWithChange (fw, w, h, c, g)
XmFormWidget fw;
Dimension * w;
Dimension * h;
Widget c;
XtWidgetGeometry *g;

{
	Dimension junkh = fw->core.height;
	Dimension junkw = fw->core.width;
	Widget child;
	XmFormConstraint fc;
	int tmp;

	if (h == NULL) h = &junkh;
	if (w == NULL) w = &junkw;

	/* Place children, but don't do it for real--just get new size */

	for(child = fw->form.first_child; 
		child != NULL;
		child = fc->next_sibling)
	{
		if (!XtIsManaged (child))
			break;

		fc = GetFormConstraint(child);

		CalcEdgeValues(child, False, c, g, w, h);
		if (!SyncEdges(fw, child, w, h, c, g))
			return(False);
	}

	for(child = fw->form.first_child; 
		child != NULL;
		child = fc->next_sibling)
	{
		if (!XtIsManaged (child)) 
			break;

		fc = GetFormConstraint(child);

		tmp = fc->att[RIGHT].tempValue;
		if (fc->att[RIGHT].type == XmATTACH_FORM)
			tmp += GetFormOffset(fw, RIGHT, fc->att);
		if (tmp > 0) 
			AssignMax (*w, tmp);
		
		tmp = fc->att[BOTTOM].tempValue ;
		if (fc->att[BOTTOM].type == XmATTACH_FORM)
			tmp += GetFormOffset(fw, BOTTOM, fc->att);
		if (tmp > 0) 
			AssignMax (*h, tmp);
	}

	if (!(*w))
		*w = 1;
	if (!(*h))
		*h = 1;

	if (*w != XtWidth(fw) || *h != XtHeight(fw)) 
		return True;
	else
		return False;
}




/************************************************************************
 *
 *  CalcFormSize
 *	Find size of a bounding box which will include all of the children.
 *
 ************************************************************************/
static void CalcFormSize (fw, w, h)
XmFormWidget fw;
Dimension * w;
Dimension * h;
{
	Widget child;
	Dimension junkh = fw->core.height;
	Dimension junkw = fw->core.width;
	XmFormConstraint fc;
	int tmp;

	if (h == NULL) h = &junkh;
	if (w == NULL) w = &junkw;

	for (child = fw->form.first_child;
		child != NULL;
		child = fc->next_sibling)
	{
		if (!XtIsManaged (child))
			break;

		fc = GetFormConstraint(child);

		CalcEdgeValues(child, False, NULL, NULL, w, h);
		if (!SyncEdges(fw, child, w, h, NULL, NULL))
			break;
	}

	for(child = fw->form.first_child; 
		child != NULL;
		child = fc->next_sibling)
	{
		if (!XtIsManaged (child)) 
			break;

		fc = GetFormConstraint(child);

		tmp = fc->att[RIGHT].tempValue;
		if (fc->att[RIGHT].type == XmATTACH_FORM)
			tmp += GetFormOffset(fw, RIGHT, fc->att);
		if (tmp > 0) 
			AssignMax (*w, tmp);
		
		tmp = fc->att[BOTTOM].tempValue ;
		if (fc->att[BOTTOM].type == XmATTACH_FORM)
			tmp += GetFormOffset(fw, BOTTOM, fc->att);
		if (tmp > 0) 
			AssignMax (*h, tmp);
	}

	if (!(*w))
		*w = 1;
	if (!(*h))
		*h = 1;
}






/************************************************************************
 *
 *  CheckSizeRequest
 *	Decide what to do about a resize request of a form entry.
 *
 ************************************************************************/

static XtGeometryResult CheckSizeRequest (w)
Widget w;

{
   XmFormConstraint c = GetFormConstraint(w);

   if (c->resizable) return XtGeometryYes;
   else	return XtGeometryNo;
}




/************************************************************************
 *
 *  GeometryManager
 *
 ************************************************************************/

static XtGeometryResult GeometryManager (w, desired, allowed)
Widget w;
XtWidgetGeometry * desired;
XtWidgetGeometry * allowed;
{
	int size_req, stack_req;
	XtWidgetGeometry original;
	XtGeometryResult size_a = XtGeometryNo;
	XmFormWidget fw = (XmFormWidget) XtParent(w);
	XmFormConstraint c = GetFormConstraint(w);

	if (fw->form.processing_constraints)
	{
		fw->form.processing_constraints = FALSE;
		PlaceChildren (fw, w, desired );
		return(XtGeometryDone);
	}

	if (desired->request_mode & CWWidth)
		c->preferred_width = desired->width;
	if (desired->request_mode & CWHeight)
		c->preferred_height = desired->height;

	if (desired->request_mode == (CWX | CWY))
		return(XtGeometryNo);

	original.x = w->core.x;
	original.y = w->core.y;
	original.width = w->core.width;
	original.height = w->core.height;
	original.border_width = w->core.border_width;


	/*  Decide what to do based on the request mask  */

	size_req = desired->request_mode & 
		(CWX | CWY | CWWidth | CWHeight | CWBorderWidth);
	stack_req = desired->request_mode & (CWSibling | CWStackMode);

	if (size_req) 
	{
		/* 
		 * All this routine does is check 
		 * that child widget is resizable
		 */

		size_a = CheckSizeRequest (w);

		if (size_a == XtGeometryYes) 
		{


		/*
		 * The childs request was OK'd by us, so see if our geo mgr
		 *  will allow us to accomodate the child resize.  If so, place
		 *  children again.
		 */

			if (ChangeIfNeeded (fw, w, desired))
			{
				if (desired->request_mode & XtCWQueryOnly)
					/* Do not change the widget */
					size_a = XtGeometryYes;
				else
				{
					PlaceChildren (fw, w, desired);


					/*
					 * if new geometry still equals desired after call
					 * to PlaceChildren then return done.  otherwise,
					 * attachment constaints have caused another
					 * geometry change.  note: this is real sicko
					 */

					if ((desired->request_mode & CWX && 
						desired->x != w->core.x) ||
						(desired->request_mode & CWY && 
						desired->y != w->core.y) ||
						(desired->request_mode & CWWidth && 
						desired->width != w->core.width) ||
						(desired->request_mode & CWHeight && 
						desired->height != w->core.height) ||
						(desired->request_mode & CWBorderWidth && 
						desired->border_width != w->core.border_width))
					{
						size_a = XtGeometryAlmost;

						allowed->request_mode = desired->request_mode;
						allowed->x = w->core.x;
						allowed->y = w->core.y;
						allowed->width = w->core.width;
						allowed->height = w->core.height;
						allowed->border_width = w->core.border_width;


						/*
						 * We've changed the geometry but are
						 * about to return "almost" implying we haven't
						 * so to avoid lying about it, change geometry
						 * back to original
						 */

						PlaceChildren (fw, w, &original);
					}
					else
					{
						size_a = XtGeometryDone;
					}
				}
			} 
			else
				size_a = XtGeometryNo;
		}
	}

	if (stack_req)
	{
		allowed->sibling = desired->sibling;
		allowed->stack_mode = desired->stack_mode;

		if (!size_req)
			size_a = XtGeometryYes;
		else if (size_a != XtGeometryYes)
			size_a = XtGeometryAlmost;
	}

	return (size_a);
}

static XtGeometryResult QueryGeometry(fw, req, ret)
XmFormWidget fw;
XtWidgetGeometry *req, *ret;
{
	Dimension w = 0, h = 0;
	Boolean good_width = FALSE, good_height = FALSE;

	SortChildren(fw);

	if (!XtIsRealized(fw))
	{
		int i;
		Widget child;
		XmFormConstraint c;

		for (i = 0; i < fw->composite.num_children; i++)
		{
			child = fw->composite.children[i];
			c = GetFormConstraint(child);
			c->preferred_width = XtWidth(child);
			c->preferred_height = XtHeight(child);
		}
	}

	CalcFormSize(fw, &w, &h);

	ret->request_mode = 0;

	/*
	 * Set the return sizes appropriately.
	 */
	switch (fw->bulletin_board.resize_policy)
	{
		case XmRESIZE_NONE:
			ret->width = fw->core.width;
			ret->height = fw->core.height;
		break;
		case XmRESIZE_GROW:
			ret->width = MAX(w, fw->core.width);
			ret->height = MAX(h, fw->core.height);
		break;
		case XmRESIZE_ANY:
			ret->width = w;
			ret->height = h;
		break;
		default:
			/*This is an error condition*/ ;
		break;
	}

	/*
	 * Handle the request.
	 */
	if (req->request_mode & CWWidth)
	{
		switch (fw->bulletin_board.resize_policy)
		{
			case XmRESIZE_NONE:
				if (req->width == fw->core.width)
					good_width = TRUE;
				else
					ret->request_mode |= CWWidth;
			break;
			case XmRESIZE_GROW:
				if (req->width >= fw->core.width)
					good_width = TRUE;
				else
					ret->request_mode |= CWWidth;
			break;
			case XmRESIZE_ANY:
				if (req->width >= w)
					good_width = TRUE;
				else
					ret->request_mode |= CWWidth;
			break;
		}
	}
	if (req->request_mode & CWHeight)
	{
		switch (fw->bulletin_board.resize_policy)
		{
			case XmRESIZE_NONE:
				if (req->height == fw->core.height)
					good_height = TRUE;
				else
					ret->request_mode |= CWHeight;
			break;
			case XmRESIZE_GROW:
				if (req->height >= fw->core.height)
					good_height = TRUE;
				else
					ret->request_mode |= CWHeight;
			break;
			case XmRESIZE_ANY:
				if (req->height >= h)
					good_height = TRUE;
				else
					ret->request_mode |= CWHeight;
			break;
		}
	}

	/*
	 * Set the return value.
	 */
	if (good_width && good_height)
		return(XtGeometryYes);
	else if (!good_width && !good_height)
		return(XtGeometryNo);
	else
		return(XtGeometryAlmost);
}




/************************************************************************
 *
 *  Resize
 *	This routine is called by the parent's geometry manager after it
 *	has ok'd the resize request AND resized the form window.  This 
 *	routine is responsible for implementing the size given.
 *
 ************************************************************************/

static void Resize (fw)
XmFormWidget fw;

{
   Boolean      draw_shadow = False;

	/* clear the shadow if its needed (will check if its now larger) */
	_XmClearShadowType ((Widget )fw, fw->bulletin_board.old_width,
		fw->bulletin_board.old_height,
		fw->bulletin_board.old_shadow_thickness, 0);

	/*
	 * if it is now smaller, redraw the shadow since there may not be a
	 * redisplay
	 */
	if ((fw->bulletin_board.old_height > fw->core.height) ||
		(fw->bulletin_board.old_width > fw->core.width))
		draw_shadow = True;

	fw->bulletin_board.old_width = fw->core.width;
	fw->bulletin_board.old_height = fw->core.height;
	fw->bulletin_board.old_shadow_thickness = 
	fw->manager.shadow_thickness;


	PlaceChildren (fw, NULL, NULL, NULL, NULL);

	if ((draw_shadow) && 
		(XtIsRealized(fw)) && 
		(fw->manager.shadow_thickness))
		/* pop-out not pop-in */
		_XmDrawShadow (XtDisplay (fw), XtWindow (fw),
			fw->manager.top_shadow_GC,
			fw->manager.bottom_shadow_GC,
			fw->manager.shadow_thickness,
			0, 0, fw->core.width, fw->core.height);
}


static void Realize(fw, m, a)
XmFormWidget fw;
Mask *m;
XSetWindowAttributes *a;
{
	if (!XtWidth(fw))
		XtWidth(fw) = 10;
	if (!XtHeight(fw))
		XtHeight(fw) = 10;
	
	(*(xmBulletinBoardClassRec.core_class.realize)) (fw, (XtValueMask*)m, a);
}

static void Redisplay(fw, event, region)
XmFormWidget fw;
XEvent *event;
Region region;
{
	_XmRedisplayGadgets((CompositeWidget)fw, (XExposeEvent*)event, region);

	if (fw->manager.shadow_thickness)
		_XmDrawShadowType((Widget)fw, fw->bulletin_board.shadow_type,
			XtWidth(fw), XtHeight(fw),
			fw->manager.shadow_thickness, 0,
			fw->manager.top_shadow_GC,
			fw->manager.bottom_shadow_GC);
}



/************************************************************************
 *
 *  PlaceChildren
 *	Position all children according to their constraints.  
 *      Return desired width and height.
 *
 ************************************************************************/

static void PlaceChildren (fw, instigator, inst_geometry)
XmFormWidget fw;
Widget instigator;
XtWidgetGeometry *inst_geometry;
{
	XtWidgetGeometry g;
	Widget child;
	register XmFormConstraint c;
	int height, width;

	for (child = fw->form.first_child;
	child != NULL;
	child = c->next_sibling) 
	{
		if (!XtIsManaged(child))
			break;

		c = GetFormConstraint(child);

		CalcEdgeValues(child, TRUE, instigator, inst_geometry, 
			NULL, NULL);

		width = c->att[RIGHT].value - c->att[LEFT].value
			- (2 * ((RectObj) child)->rectangle.border_width);
		height = c->att[BOTTOM].value - c->att[TOP].value
			- (2 * ((RectObj) child)->rectangle.border_width);

		if (width <= 0)
			width = 1;
		if (height <= 0)
			height = 1;

		g.x = c->att[LEFT].value;
		g.y = c->att[TOP].value;
		g.width = width;
		g.height = height;
		g.request_mode = 0;

		if (g.x != ((RectObj) child)->rectangle.x) 
			g.request_mode |= CWX;
		if (g.y != ((RectObj) child)->rectangle.y) 
			g.request_mode |= CWY;
		if (g.width != ((RectObj) child)->rectangle.width) 
			g.request_mode |= CWWidth;
		if (g.height != ((RectObj) child)->rectangle.height) 
			g.request_mode |= CWHeight;


		if (g.request_mode)
			_XmConfigureObject((RectObj)child, g.x, g.y, g.width, 
				g.height, child->core.border_width);
	}
}


/************************************************************************
 *
 *  ChangeManaged
 *	Something changed in the set of managed children, so place 
 *	the children and change the form widget size to reflect new size, 
 *	if possible.
 *
 ************************************************************************/

static void ChangeManaged (fw)
XmFormWidget fw;
{
	XtWidgetGeometry g;
	int i, j, k, grow_ok, shrink_ok;
	register XmFormConstraint c;
	register Widget w, child;

	/*
	 * The following code works around a bug in the intrinsics
	 * destroy processing.  The child is unmanaged before anything
	 * else (destroy callbacks) so we have to handle the destroy
	 * inside of changemanaged instead of in a destroy callback
	 */
	for (k = 0; k < fw->composite.num_children; k++)
	{
		child = fw->composite.children[k];

		if (child->core.being_destroyed)
		{
			/*  If anyone depends on this child,
			make into a dependency on form  */


			for (i = 0; i < fw->composite.num_children; i++)
			{
				w = fw->composite.children[i];
				c = GetFormConstraint(w);

				for (j = 0; j < 4; j++) 
				{
					if (((c->att[j].type == XmATTACH_WIDGET) &&
						(c->att[j].w == child))
						||
						((c->att[j].type == XmATTACH_OPPOSITE_WIDGET) &&
						(c->att[j].w == child)))
					{
						switch (j)
						{
							case LEFT:
								c->att[j].type = XmATTACH_FORM;
								c->att[j].offset = w->core.x;
							break;
							case TOP:
								c->att[j].type = XmATTACH_FORM;
								c->att[j].offset = w->core.y;
							break;
							default:
								c->att[j].type = XmATTACH_NONE;
							break;
						}
						c->att[j].w = NULL;
					}
				}
			}
		}
	}

	SortChildren (fw);

	if (!XtIsRealized(fw))
	{
		/* First time through */
		Dimension w = 0, h = 0;
		Widget child;
		int i;

		for (i = 0; i < fw->composite.num_children; i++)
		{
			child = fw->composite.children[i];
			c = GetFormConstraint(child);
			c->preferred_width = XtWidth(child);
			c->preferred_height = XtHeight(child);
		}

		g.request_mode = 0;
		g.width = (fw->core.width ? fw->core.width : 1);
		g.height = (fw->core.height ? fw->core.height : 1);

		if (!fw->form.initial_width && fw->form.initial_height)
		{
			CalcFormSize(fw, &w, NULL);
			g.width = w;
			g.request_mode |= CWWidth;
		}
		else if (fw->form.initial_width && !fw->form.initial_height)
		{
			CalcFormSize(fw, NULL, &h);
			g.height = h;
			g.request_mode |= CWHeight;
		}
		else if (!fw->form.initial_width && !fw->form.initial_height)
		{
			CalcFormSize(fw, &w, &h);
			g.width = w;
			g.height = h;
			g.request_mode |= (CWWidth | CWHeight);
		}

		if (g.request_mode != 0) 
			_XmMakeGeometryRequest((Widget)fw, &g);

		PlaceChildren (fw, NULL, NULL);
	}
	else
	{
		ChangeIfNeeded(fw, NULL, NULL);
		PlaceChildren (fw, NULL, NULL);
	}

	fw->bulletin_board.old_width = fw->core.width;
	fw->bulletin_board.old_height = fw->core.height;
	fw->bulletin_board.old_shadow_thickness =
	fw->manager.shadow_thickness;

	_XmNavigChangeManaged((Widget)fw);
}                       




/************************************************************************
 *
 *  ChangeIfNeeded
 *	Returns whether to honor widget w's resize request; only returns 
 *      False if resize would require form to change size but it cannot.
 *	Form changes size as a side effect.
 *
 ************************************************************************/

static Boolean ChangeIfNeeded (fw, w, desired)
XmFormWidget fw;
Widget w;
XtWidgetGeometry * desired;

{
	XtWidgetGeometry g;
	Boolean answer = TRUE;
	int grow_ok, shrink_ok;

	g.request_mode = 0;
	g.width = 0;
	g.height = 0;

	grow_ok   = fw->bulletin_board.resize_policy != XmRESIZE_NONE;
	shrink_ok = fw->bulletin_board.resize_policy != XmRESIZE_NONE && 
		fw->bulletin_board.resize_policy != XmRESIZE_GROW;

	if (CalcFormSizeWithChange(fw, &g.width, &g.height, w, desired)) 
	{
		if ((g.width > fw->core.width && grow_ok) ||
			(g.width < fw->core.width && shrink_ok)) 
			g.request_mode |= CWWidth;

		if ((g.height > fw->core.height && grow_ok) ||
			(g.height < fw->core.height && shrink_ok)) 
			g.request_mode |= CWHeight;

		if ((g.request_mode) && 
			(_XmMakeGeometryRequest((Widget)fw, &g) == XtGeometryNo))
			answer = FALSE;
	}

	if ( fw->bulletin_board.old_shadow_thickness &&
		(fw->bulletin_board.old_width != fw->core.width ||
		fw->bulletin_board.old_height != fw->core.height) )
	{
		_XmClearShadowType ((Widget )fw, fw->bulletin_board.old_width,
		fw->bulletin_board.old_height,
		fw->bulletin_board.old_shadow_thickness, 0);
	}

	fw->bulletin_board.old_width = fw->core.width;
	fw->bulletin_board.old_height = fw->core.height;
	fw->bulletin_board.old_shadow_thickness =
	fw->manager.shadow_thickness;

	return (answer);
}




/************************************************************************
 *
 *  DeleteChild
 *	Delete a single widget from a parent widget
 *
 ************************************************************************/

static void DeleteChild (child)
Widget child;

{
   XmFormWidget fw = (XmFormWidget) XtParent(child);
   Widget w;
   register XmFormConstraint c;
   int i;
   register int j;
   WidgetClass class;


   if (!XtIsRectObj(child))
	return;

   /*  Use our superclass's composite delete proc */
	class = XtClass(fw);
	XtClass(fw) = XtSuperclass(fw);
	(* (((CompositeWidgetClass) XtClass(fw))->
		composite_class.delete_child)) (child);
	XtClass(fw) = class;

	SortChildren(fw);
}




/************************************************************************
 *
 *  SetValues
 *
 ************************************************************************/

static Boolean SetValues (old, req, new)
XmFormWidget old, req, new;
{
	Dimension w = 0;
	Dimension h = 0;


	/*  Check for invalid fraction base  */

	if (new->form.fraction_base == 0)
	{
		_XmWarning (new, MESSAGE1);
		new->form.fraction_base = old->form.fraction_base;
	}

	/* Undo any help we got from our superclass */
	new->core.width = req->core.width;
	new->core.height = req->core.height;

	if (XtIsRealized(new))
	{
		/* SetValues of width and/or height = 0, signals the form to  */
		/* recompute its bounding box and grow if it needs.           */

		if ((XtWidth(new) != XtWidth(old)) ||
			(XtHeight(new) != XtHeight(old))) 
		{
			if ((XtWidth(new) == 0) || (XtHeight(new) == 0))
			{
				CalcFormSize (new, &w, &h);
				if (XtWidth(new) == 0) XtWidth(new) = w;
				if (XtHeight(new) == 0) XtHeight(new) = h;

			} 
			else
			{
				w = XtWidth(new);
				h = XtHeight(new);
			}
		}


		/*  If default distance has changed, or the
			fraction base has changed, recalculate size.  */

		if ((new->form.horizontal_spacing !=
				old->form.horizontal_spacing)   ||
			(new->form.vertical_spacing != 
				old->form.vertical_spacing)     ||
			(new->form.fraction_base != 
				old->form.fraction_base))
		{
			CalcFormSize(new, &w, &h);
			XtWidth(new) = w;
			XtHeight(new) = h;
		}
	}
	else if ((XtWidth(new) != XtWidth(old)) ||
		(XtHeight(new) != XtHeight(old)))
	{
		if (XtWidth(new) != XtWidth(old))
			new->form.initial_width = TRUE;
		if (XtHeight(new) != XtHeight(old))
			new->form.initial_height = TRUE;
	}

	return False;
}

static void SetValuesAlmost(old, new, req, rep)
XmFormWidget old, new;
XtWidgetGeometry *req, *rep;
{
	if (!rep->request_mode)
		PlaceChildren(new, NULL, NULL);

	*req = *rep;
}




/************************************************************************
 *
 *  ConstraintSetValues
 *	If any values change, what we do is place everything again.
 *
 ************************************************************************/

static Boolean ConstraintSetValues (old, ref, new)
register Widget old, ref, new;

{
	XmFormWidget fw = (XmFormWidget) XtParent(new);
	register XmFormConstraint oldc, newc;
	register int i;

	if (!XtIsRectObj(new)) return (False);

	oldc = GetFormConstraint(old),
	newc = GetFormConstraint(new);

	if (XtWidth(new) != XtWidth(old))
		newc->preferred_width = XtWidth(new);

	if (XtHeight(new) != XtHeight(old))
		newc->preferred_height = XtHeight(new);

	/*  Validate the attachement type.  */

	for (i = 0; i < 4; i++)
	{
		if (newc->att[i].type != oldc->att[i].type)
		{
			switch (newc->att[i].type)
			{
				case XmATTACH_NONE:
				case XmATTACH_FORM:
				case XmATTACH_OPPOSITE_FORM:
				case XmATTACH_WIDGET:
				case XmATTACH_OPPOSITE_WIDGET:
				case XmATTACH_POSITION:
				case XmATTACH_SELF:
				break;

				default:
				_XmWarning(new, MESSAGE2);
				newc->att[i].type = oldc->att[i].type;
				break;
			}
		}
		if ((newc->att[i].type == XmATTACH_WIDGET) ||
			(newc->att[i].type == XmATTACH_OPPOSITE_WIDGET))
		{
			if ((newc->att[i].w) && 
				(XtParent(newc->att[i].w) != XtParent(new)))
			{
				Widget att_w = newc->att[i].w;
				if ((XmIsText(att_w) || XmIsList(att_w))
					&&
					((XmIsScrolledWindow(XtParent(att_w))) &&
					(XtParent(XtParent(att_w)) == (Widget) fw)))
					newc->att[i].w = XtParent(att_w);
				else
					_XmWarning (new, MESSAGE8);
			}
		}
	}

	/* Re do the layout only if we have to */
	if ((XtIsRealized(fw)) && (XtIsManaged(new)) &&
		(ANY(type) || ANY(w) || ANY(percent) || ANY(offset)))
	{
		XtWidgetGeometry g;

		g.request_mode = 0;

		if (XtWidth(new) != XtWidth(old))
		{
			g.request_mode |= CWWidth;
			g.width = XtWidth(new);
		}

		if (XtHeight(new) != XtHeight(old))
		{
			g.request_mode |= CWHeight;
			g.height = XtHeight(new);
		}

		if (XtBorderWidth(new) != XtBorderWidth(old))
		{
			g.request_mode |= CWBorderWidth;
			g.border_width = XtBorderWidth(new);
		}

		fw->form.processing_constraints = TRUE;
		SortChildren(fw);
		ChangeIfNeeded(fw, new, &g);
		PlaceChildren(fw, new, &g);
	}

	return (False);
}




/************************************************************************
 *
 *  Initialize
 *	The form widget specific initialization.
 *
 ************************************************************************/

static void Initialize (req, new)
XmFormWidget req, new;
{
	new->form.first_child = NULL;


	if (new->form.fraction_base == 0)
	{
		new->form.fraction_base = 100;
		_XmWarning(new, MESSAGE1);
	}

	new->form.processing_constraints = FALSE;

	/* Undo the stuff that our superclass did */

	if (req->core.width)
		new->form.initial_width = TRUE;
	else
		new->form.initial_width = FALSE;

	if (req->core.height)
		new->form.initial_height = TRUE;
	else
		new->form.initial_height = FALSE;

	/* Set up for shadow drawing */

	new->bulletin_board.old_width = XtWidth(new);
	new->bulletin_board.old_height = XtHeight(new);
	new->bulletin_board.old_shadow_thickness =
		new->manager.shadow_thickness;
}



/************************************************************************
 *
 *  ConstraintInitialize
 *
 ************************************************************************/

static void ConstraintInitialize (req, new)
Widget req, new;

{
   XmFormWidget fw = (XmFormWidget) XtParent(new);
   XmFormConstraint nc;
   register int i;


   if (!XtIsRectObj(new)) return;

   nc = GetFormConstraint(new);

   /*  Validate the attachement type.  */

   for (i = 0; i < 4; i++)
   {
      switch (nc->att[i].type)
      {
         case XmATTACH_NONE:
         case XmATTACH_FORM:
         case XmATTACH_OPPOSITE_FORM:
         case XmATTACH_WIDGET:
         case XmATTACH_OPPOSITE_WIDGET:
         case XmATTACH_POSITION:
         case XmATTACH_SELF:
         break;

         default:
            _XmWarning (new, MESSAGE2);
            nc->att[i].type = XmATTACH_NONE;
         break;
      }
	  if ((nc->att[i].type == XmATTACH_WIDGET) ||
	  	(nc->att[i].type == XmATTACH_OPPOSITE_WIDGET))
		{
			if ((nc->att[i].w) &&
				(XtParent(nc->att[i].w) != XtParent(new)))
			{
				Widget att_w = nc->att[i].w;
				if ((XmIsText(att_w) || XmIsList(att_w))
					&&
					((XmIsScrolledWindow(XtParent(att_w))) &&
					(XtParent(XtParent(att_w)) == (Widget) fw)))
					nc->att[i].w = XtParent(att_w);
				else
					_XmWarning (new, MESSAGE8);
			}
		}
   }

   nc->preferred_width = XtWidth(new);
   nc->preferred_height = XtHeight(new);
}




/************************************************************************
 *
 *  CheckConstraints
 *
 ************************************************************************/

static void CheckConstraints (w)
Widget w;

{
	XmFormConstraint c;
	XmFormWidget fw = (XmFormWidget) XtParent(w);
	XmFormAttachment left, right, top, bottom;
	XmFormAttachment a;
	int which;
	int wid, ht;

	if (!XtIsRectObj(w)) return;

	c = GetFormConstraint(w);

	left = &c->att[LEFT]; 
	right = &c->att[RIGHT];
	top = &c->att[TOP]; 
	bottom = &c->att[BOTTOM];

	if (left->type == XmATTACH_NONE && right->type == XmATTACH_NONE) 
	{
		if (fw->form.rubber_positioning) 
			left->type = right->type = XmATTACH_SELF;
		else
		{
			left->type = XmATTACH_FORM;
			left->offset = w->core.x;
		}
	}

	if (top->type == XmATTACH_NONE && bottom->type == XmATTACH_NONE)
	{
		if (fw->form.rubber_positioning) 
			top->type = bottom->type = XmATTACH_SELF;
		else 
		{
			top->type = XmATTACH_FORM;
			top->offset = w->core.y;
		}
	}

	for (which = 0; which < 4; which++) 
	{
		a = &c->att[which];

		switch (a->type) 
		{
			case XmATTACH_NONE:
			case XmATTACH_FORM:
			case XmATTACH_OPPOSITE_FORM:
				a->w = NULL;
				a->percent = 0;
			break;

			case XmATTACH_SELF:
				a->offset = 0;
				a->w = NULL;
				a->type = XmATTACH_POSITION;

				wid = w->core.x + w->core.width
					+ (2 * w->core.border_width);
				ht = w->core.y + w->core.height
					+ (2 * w->core.border_width);

				if (wid < fw->core.width)
					wid = fw->core.width;
				if (ht  < fw->core.height)
					ht  = fw->core.height;

				switch (which)
				{
					case LEFT:
						a->percent = (w->core.x
							* fw->form.fraction_base) / wid;
					break;

					case TOP:
						a->percent = (w->core.y
							* fw->form.fraction_base) / ht;
					break;

					case RIGHT:
						a->percent = ((w->core.x + w->core.width +
						2 * w->core.border_width) *
						fw->form.fraction_base) / wid;
					break;

					case BOTTOM:
						a->percent = ((w->core.y + w->core.height +
						2 * w->core.border_width) *
						fw->form.fraction_base) / ht;
					break;
				}
			break;

			case XmATTACH_POSITION:
				a->w = 0;
			break;

			case XmATTACH_WIDGET:
			case XmATTACH_OPPOSITE_WIDGET:
				a->percent = 0;
			break;
		}
	}
}




/************************************************************************
 *
 *  SortChildren
 *
 ************************************************************************/

static void SortChildren (fw)
register XmFormWidget fw;
{
   int i, j;
   Widget child;
   register XmFormConstraint c, c1;
   int sortedCount = 0;
   Widget last_child, att_widget;
   Boolean sortable;


   fw->form.first_child = NULL;

   for (i = 0; i < fw->composite.num_children; i++)
   {
      child = fw->composite.children[i];
      c = GetFormConstraint(child);

      if (XtIsManaged (child))
      {
         c->sorted = False;
         c->next_sibling = NULL;
      }
      else
      {	
         c->next_sibling = fw->form.first_child;
         fw->form.first_child = child;
         c->sorted = True;
         sortedCount++;
      }

      CheckConstraints(child);
   }


   /* THIS IS PROBABLY WRONG AND SHOULD BE FIXED SOMEDAY             */
   /* WHY SHOULD UNMANAGED CHILDREN BE ALLOWED AS ATTACHMENT POINTS  */
   /* FOR MANAGED CHILDREN???                                        */

   /* While there are unsorted children, find one with only sorted  */
   /* predecessors and put it in the list.  This algorithm works    */
   /* particularly well if the order is already correct             */

   last_child = NULL;
		
   for ( ; sortedCount != fw->composite.num_children; sortedCount++) 
   {
      sortable = False;

      for (i = 0; !sortable && i < fw->composite.num_children; i++) 
      {
         child = fw->composite.children[i];
         c = GetFormConstraint(child);

         if (c->sorted) continue;

         sortable = True;

         for (j = 0; j < 4; j++) 
         {
            if ((c->att[j].type == XmATTACH_WIDGET) ||
				(c->att[j].type == XmATTACH_OPPOSITE_WIDGET))
            {	
               att_widget = c->att[j].w;

               if ((att_widget != NULL) &&
				(XtParent(att_widget) == XtParent(child)))
				{
				   c1 = GetFormConstraint (att_widget);
				   if (!c1->sorted) sortable = False;
				}
				else
					c->att[j].type = XmATTACH_FORM;
            }
          }
      }

      if (sortable)
      {
         /*  We have found a sortable child...add to sorted list.  */

         if (last_child == NULL) 
         {
            c->next_sibling = fw->form.first_child;
            fw->form.first_child = child;
         }
         else
         {
            c1 = GetFormConstraint(last_child);
            c->next_sibling = c1->next_sibling;
            c1->next_sibling = child;
         }

         last_child = child;
         c->sorted = True;
      }

      else
      {
         /*  We failed to find a sortable child, there must be  */
         /*  a circular dependency.                             */ 

         _XmWarning (fw, MESSAGE5);
         return;
      }
   }
}




/************************************************************************
 *
 *  CalcEdgeValues
 *
 ************************************************************************/

static void CalcEdgeValues (w, really, instigator, inst_geometry,
	form_width, form_height)
Widget w;
Boolean really;
Widget instigator;
XtWidgetGeometry *inst_geometry;
Dimension *form_width, *form_height;
{
	XmFormConstraint c = GetFormConstraint (w);
	XmFormWidget fw = (XmFormWidget) XtParent (w);
	XmFormAttachment left = &c->att[LEFT], right = &c->att[RIGHT],
		top = &c->att[TOP], bottom = &c->att[BOTTOM] ;
	Dimension width, height, border_width;

	if (w == instigator) 
	{ 
		if (inst_geometry->request_mode & CWWidth)
			width = inst_geometry->width; 
		else
			width = w->core.width;
		if (inst_geometry->request_mode & CWHeight)
			height = inst_geometry->height; 
		else
			height = w->core.height;
		if (inst_geometry->request_mode & CWBorderWidth)
			border_width = inst_geometry->border_width;
		else
			border_width = w->core.border_width;
	}
	else if (!fw->form.processing_constraints)
	{
		/*
		* If we just use the widget's current geometry we will
		* effectively be grow only.  That would not be correct.
		*
		* Instead we will use our idea of the child's preferred
		* size.
		*/
		width = c->preferred_width;
		height = c->preferred_height;
		border_width = w->core.border_width;
	}
	else
	{
		width = w->core.width;
		height = w->core.height;
		border_width = w->core.border_width;
	}

	width += border_width * 2;
	height += border_width * 2;


	if (width <= 0) width = 1;
	if (height <= 0) height = 1;

	if (left->type != XmATTACH_NONE)
	{
		if (right->type != XmATTACH_NONE)	    /* LEFT and right are attached */
		{
			CalcEdgeValue(fw, w, width, border_width, 
				LEFT, really, form_width, form_height);
			CalcEdgeValue(fw, w, width, border_width, 
				RIGHT, really, form_width, form_height);
		}

		else 	/*  LEFT attached, compute right  */
		{
			CalcEdgeValue(fw, w, width, border_width, 
				LEFT, really, form_width, form_height);
			ComputeAttachment(w, width, border_width, RIGHT, really,
			form_width, form_height);
		}
	} 
	else
	{
		if (right->type != XmATTACH_NONE)    /* RIGHT attached, compute left */
		{
		CalcEdgeValue(fw, w, width, border_width, 
			RIGHT, really, form_width, form_height);
		ComputeAttachment(w, width, border_width, 
			LEFT, really, form_width, form_height);
		}
	}

	if (top->type != XmATTACH_NONE) 
	{
		if (bottom->type != XmATTACH_NONE)   /* TOP and bottom are attached */
		{
			CalcEdgeValue(fw, w, height, border_width, 
				TOP, really, form_width, form_height);
			CalcEdgeValue(fw, w, height, border_width, 
				BOTTOM, really, form_width, form_height);
		}
		else	/* TOP attached, compute bottom */
		{
			CalcEdgeValue(fw, w, height, border_width, 
				TOP, really, form_width, form_height);
			ComputeAttachment(w, height, border_width, 
				BOTTOM, really, form_width, form_height);
		}
	}
	else
	{
		if (bottom->type != XmATTACH_NONE)   /* BOTTOM attached, compute top */
		{
			CalcEdgeValue(fw, w, height, border_width, 
				BOTTOM, really, form_width, form_height);
			ComputeAttachment(w, height, border_width, 
				TOP, really, form_width, form_height);
		} 
	}
}



/*********************************************************************
 *
 * CheckBottomBase
 *
 *********************************************************************/
static float CheckBottomBase(sibling, opposite)
Widget sibling;
Boolean opposite;
{
	XmFormWidget fw = (XmFormWidget) sibling->core.parent;
	XmFormConstraint c = GetFormConstraint(sibling);
	XmFormConstraint ca;
	Boolean flag = FALSE;
	float return_val;

	if (!opposite)
	{
		switch (c->att[TOP].type)
		{
			case XmATTACH_POSITION:
				return_val = (float) ((float) fw->form.fraction_base
					/ (float) c->att[TOP].percent);
			break;
			case XmATTACH_NONE:
				switch (c->att[BOTTOM].type)
				{
					case XmATTACH_FORM:
						return_val = 1.0;
					break;
					case XmATTACH_POSITION:
						return_val = (float)
							((float) fw->form.fraction_base 
							/ (float) c->att[BOTTOM].percent);
					break;
					case XmATTACH_OPPOSITE_WIDGET:
						flag = TRUE;
					case XmATTACH_WIDGET:
						return_val = 
							CheckBottomBase(c->att[BOTTOM].w, flag);
					break;
					default:
						return_val = 0.0;
					break;
				}
			break;
			default:
				return_val = 0.0;
			break;
		}
	}
	else
	{
		switch(c->att[BOTTOM].type)
		{
			case XmATTACH_NONE:
				if (c->att[TOP].type == XmATTACH_POSITION)
					return_val = (float)
						((float) fw->form.fraction_base 
						/ (float) c->att[TOP].percent);
				else
					return_val = 0.0;
			break;
			case XmATTACH_POSITION:
				return_val = (float) ((float) fw->form.fraction_base
					/ (float) c->att[BOTTOM].percent);
			break;
			case XmATTACH_OPPOSITE_WIDGET:
				flag = TRUE;
			case XmATTACH_WIDGET:
				return_val = CheckBottomBase(c->att[BOTTOM].w, flag);
			break;
			default:
				return_val = 0.0;
			break;
		}
	}

	return(return_val);
}


/*********************************************************************
 *
 * CheckRightBase
 *
 *********************************************************************/
static float CheckRightBase(sibling, opposite)
Widget sibling;
Boolean opposite;
{
	XmFormWidget fw = (XmFormWidget) sibling->core.parent;
	XmFormConstraint c = GetFormConstraint(sibling);
	XmFormConstraint ca;
	Boolean flag = FALSE;
	float return_val;

	if (!opposite)
	{
		switch (c->att[LEFT].type)
		{
			case XmATTACH_POSITION:
				return_val =  (float) ((float) fw->form.fraction_base
					/ (float) c->att[LEFT].percent);
			break;
			case XmATTACH_NONE:
				switch (c->att[RIGHT].type)
				{
					case XmATTACH_FORM:
						return_val = 1.0;
					break;
					case XmATTACH_POSITION:
						return_val = (float) 
							((float) fw->form.fraction_base 
							/  (float) c->att[RIGHT].percent);
					break;
					case XmATTACH_OPPOSITE_WIDGET:
						flag = TRUE;
					case XmATTACH_WIDGET:
						return_val = 
							CheckRightBase(c->att[RIGHT].w, flag);
					break;
					default:
						return_val =  0.0;
					break;
				}
			break;
			default:
				return_val = 0.0;
			break;
		}
	}
	else
	{
		switch(c->att[RIGHT].type)
		{
			case XmATTACH_NONE:
				if (c->att[LEFT].type == XmATTACH_POSITION)
					return_val = (float) 
						((float) fw->form.fraction_base 
						/ (float)c->att[LEFT].percent);
				else
					return_val = 0.0;
			break;
			case XmATTACH_POSITION:
				return_val = (float) ((float) fw->form.fraction_base
					/ (float) c->att[RIGHT].percent);
			break;
			case XmATTACH_OPPOSITE_WIDGET:
				flag = TRUE;
			case XmATTACH_WIDGET:
				return_val = CheckRightBase(c->att[RIGHT].w, flag);
			break;
			default:
				return_val = 0.0;
			break;
		}
	}

	return(return_val);
}

/*********************************************************************
 *
 *  CalcEdgeValue
 *
 *********************************************************************/

static void CalcEdgeValue (fw, w, size, border_width, which, really,
	fwidth, fheight)
XmFormWidget fw;
Widget w;
Dimension size;
Dimension border_width;
int which;
Boolean really;
Dimension *fwidth, *fheight;
{
   float scale;
   XmFormAttachment att = GetFormConstraint(w)->att;
   XmFormAttachment a = att + which;
   XmFormConstraint c;
   XmFormAttachment ref;
   float factor;
   int temp1, temp2, temp3;
   int ctype;


   ctype = a->type;

   if ((ctype == XmATTACH_WIDGET)
	&& 
	((!a->w) || (XtParent(a->w) != XtParent(w))))
      ctype = XmATTACH_FORM;

   if ((ctype == XmATTACH_OPPOSITE_WIDGET) 
	&&
	((!a->w) || (XtParent(a->w) != XtParent(w))))
      ctype = XmATTACH_OPPOSITE_FORM;
    
   switch (ctype) 
   {
      case XmATTACH_FORM:
         a->w = NULL;
         a->percent = 0;

         switch (which) 
         {
            case LEFT:
            case TOP:
               AssignValue(a, 
				GetFormOffset(fw, which, att));
            break;

            case RIGHT:
				if (fwidth != NULL)
					if ((att + LEFT)->type == XmATTACH_OPPOSITE_FORM)
					{
						/* 
						 * the top will already have been attached
						 * we will cause the widget to dissappear
						 */
						AssignValue(a, Value((att + LEFT)) + size
							+ GetFormOffset(fw,which,att));
					}
					else if ((att + LEFT)->type == XmATTACH_NONE)
					{
						temp1 = *fwidth 
							- (GetFormOffset(fw, which, att));
						temp2 = temp1 - size;
						if (temp2 < 0)
						{
							*fwidth += abs(temp2);
							AssignValue(a, (temp1 + abs(temp2)));
						}
						else
							AssignValue(a, temp1);
					}
					else
					{
						temp1 = *fwidth 
							- (GetFormOffset(fw, which, att));
						temp2 = Value(att + LEFT);
						temp3 = temp1 - temp2 - size;
						if (temp3 < 0)
						{
							*fwidth += abs(temp3);
							AssignValue(a, (*fwidth - size
								- GetFormOffset(fw, which, att)));
						}
						else
							AssignValue(a, temp1);
					}
				else
					if ((att + LEFT)->type == XmATTACH_OPPOSITE_FORM)
					{
						/* 
						 * the top will already have been attached
						 * we will cause the widget to dissappear
						 */
						AssignValue(a, Value((att + LEFT)) + size
							+ GetFormOffset(fw,which,att));
					}
					else
						AssignValue(a, fw->core.width 
							- GetFormOffset(fw,which,att));
			break;
            case BOTTOM:
				if (fheight != NULL)
					if ((att + TOP)->type == XmATTACH_OPPOSITE_FORM)
					{
						/* 
						 * the top will already have been attached
						 * we will cause the widget to dissappear
						 */
						AssignValue(a, Value((att + TOP)) + size
							+ GetFormOffset(fw,which,att));
					}
					else if ((att + TOP)->type == XmATTACH_NONE)
					{
						temp1 = *fheight 
							- GetFormOffset(fw, which, att);
						temp2 = temp1 - size;
						if (temp2 < 0)
						{
							*fheight += abs(temp2);
							AssignValue(a, (temp1 + abs(temp2)));
						}
						else
							AssignValue(a, temp1);
					}
					else
					{
						temp1 = *fheight 
							- GetFormOffset(fw, which, att);
						temp2 = Value(att + TOP);
						temp3 = temp1 - temp2 - size;
						if (temp3 < 0)
						{
							*fheight += abs(temp3);
							AssignValue(a, (*fheight - size
								- GetFormOffset(fw, which, att)));
						}
						else
							AssignValue(a, temp1);
					}
				else
					if ((att + TOP)->type == XmATTACH_OPPOSITE_FORM)
					{
						/* 
						 * the top will already have been attached
						 * we will cause the widget to dissappear
						 */
						AssignValue(a, Value((att + TOP)) + size
							+ GetFormOffset(fw,which,att));
					}
					else
						AssignValue(a, fw->core.height 
							- GetFormOffset(fw,which,att));
            break;
         }
         return;
    
      case XmATTACH_OPPOSITE_FORM:
         a->w = NULL;
         a->percent = 0;
    
         switch (which) 
         {
            case LEFT:
			   if (fwidth)
				   AssignValue(a, 
					*fwidth + GetFormOffset(fw, which, att));
			   else
				   AssignValue(a,
					fw->core.width + GetFormOffset(fw, which, att));
            break;

            case TOP:
				if (fheight)
				   AssignValue(a,
					*fheight + GetFormOffset(fw, which, att));
				else
				   AssignValue(a,
					fw->core.height + GetFormOffset(fw, which, att));
            break;

            case RIGHT:
            case BOTTOM:
					AssignValue(a, -(GetFormOffset(fw, which, att)));
            break;
         }
         return;

      case XmATTACH_POSITION:
         scale = ((float) a->percent) / fw->form.fraction_base;
         a->w = 0;
    
         switch (which) 
         {
            case LEFT:
			   if (fwidth != NULL)
				   AssignValue(a, (*fwidth * scale + 0.5) 
					+ GetFormOffset(fw, which, att));
			   else
				   AssignValue(a, (fw->core.width * scale + 0.5)
					+ GetFormOffset(fw, which, att));
            break;
            case TOP:
			   if (fheight != NULL)
				   AssignValue(a, (*fheight * scale + 0.5) + 
								  GetFormOffset(fw, which, att));
				else
				   AssignValue(a, (fw->core.height * scale + 0.5) + 
								  GetFormOffset(fw, which, att));
            break;
            case RIGHT:
				if (fwidth != NULL)
					if ((att + LEFT)->type != XmATTACH_NONE)
					{
						temp1 = ((*fwidth  * scale) + 0.5) 
							- GetFormOffset(fw, which, att);
						temp2 = Value(att + LEFT);
						temp3 = temp1 - temp2 - size;
						if (temp3 < 0)
						{
							if (!scale)
								scale = 1.0;

							*fwidth += (((1.0 / scale) * abs(temp3)) + 0.5);
						}
						AssignValue(a, (temp2 + size));
					}
					else
					   AssignValue(a, (*fwidth * scale + 0.5) - 
									  GetFormOffset(fw, which, att));
				else
					   AssignValue(a, 
						(fw->core.width * scale + 0.5) - 
									  GetFormOffset(fw, which, att));
			break;
            case BOTTOM:
				if (fheight != NULL)
					if ((att + TOP)->type != XmATTACH_NONE)
					{
						temp1 = ((*fheight  * scale) + 0.5) 
							- GetFormOffset(fw, which, att);
						temp2 = Value(att + TOP);
						temp3 = temp1 - temp2 - size;
						if (temp3 < 0)
						{
							if (!scale)
								scale = 1.0;

							*fheight += (((1.0 / scale) * abs(temp3)) + 0.5);
						}
						AssignValue(a, (temp2 + size));
					}
					else
					   AssignValue(a, (*fheight * scale + 0.5) - 
									  GetFormOffset(fw, which, att));
				else
					AssignValue(a, 
						(fw->core.height * scale + 0.5) - 
							  GetFormOffset(fw, which, att));
         }
         return;

      case XmATTACH_WIDGET:
         a->percent = 0;
         c = GetFormConstraint(a->w);

         switch (which) 
         {
            case LEFT:
               ref = &c->att[RIGHT];
               AssignValue(a, Value(ref) + GetFormOffset(fw, which, att));
            break;

            case TOP:
               ref = &c->att[BOTTOM];
               AssignValue(a, Value(ref) + GetFormOffset(fw, which, att));
            break;

            case RIGHT:
               ref = &c->att[LEFT];
			   if (att[LEFT].type != XmATTACH_NONE)
			   {
				   temp1 = Value(ref) - GetFormOffset(fw, which, att);
				   temp2 = temp1 - Value(&(att[LEFT]));
				   temp3 = temp2 - size;
				   if ((fwidth) && (temp3 < 0))
				   {
					   factor = CheckRightBase(a->w, FALSE);
					   *fwidth += ((factor * abs(temp3)) + 0.5);
					   temp1 = Value(&(att[LEFT])) + size;
				   }
				   AssignValue(a, temp1);
			   }
			   else
				{
				   temp1 = Value(ref) - GetFormOffset(fw, which, att);
				   AssignValue(a, temp1);
				}
            break;

            case BOTTOM:
               ref = &c->att[TOP];
			   if (att[TOP].type != XmATTACH_NONE)
			   {
				   temp1 = Value(ref) - GetFormOffset(fw, which, att);
				   temp2 = temp1 - Value((&att[TOP]));
				   temp3 = temp2 - size;
				   if ((fwidth) && (temp3 < 0))
				   {
					   factor = CheckBottomBase(a->w, FALSE);
					   *fheight += ((factor * abs(temp3)) + 0.5);
					   temp1 = Value((&att[TOP])) + size;
				   }
				   AssignValue(a, temp1);
			   }
			   else
				{
				   temp1 = Value(ref) - GetFormOffset(fw, which, att);
				   AssignValue(a, temp1);
				}
            break;
         }
         return;

      case XmATTACH_OPPOSITE_WIDGET:
         a->percent = 0;
         c = GetFormConstraint(a->w);
    
         switch (which) 
         {
            case LEFT:
               ref = &c->att[LEFT];
               AssignValue(a, Value(ref) + GetFormOffset(fw, which, att));
            break;

            case TOP:
               ref = &c->att[TOP];
               AssignValue(a, Value(ref) + GetFormOffset(fw, which, att));
            break;

            case RIGHT:
               ref = &c->att[RIGHT];
               AssignValue(a, Value(ref) - GetFormOffset(fw, which, att));
            break;

            case BOTTOM:
               ref = &c->att[BOTTOM];
               AssignValue(a, Value(ref) - GetFormOffset(fw, which, att));
            break;
         }
         return;
   }
}




/************************************************************************
 *
 *  ComputeAttachment
 *
 ************************************************************************/

static void ComputeAttachment(w, size, border_width, which, really,
	fwidth, fheight)
Widget w;
Dimension size;
Dimension border_width;
int which;
Boolean really;
Dimension *fwidth, *fheight;
{
   XmFormWidget fw = (XmFormWidget) w->core.parent;
   XmFormConstraint c = GetFormConstraint(w);
   XmFormAttachment a = &c->att[which];
   int temp;

   switch (which) 
   {
      case LEFT:
		 temp =  Value(&c->att[RIGHT]) - size;
		 if ((fwidth != NULL) && (temp < 0))
		 {
			*fwidth += abs(temp);
			temp = 0;
		 }
         AssignValue(a,temp);
      break;

      case RIGHT:
		 temp = Value(&c->att[LEFT]) + size;
		 if ((fwidth != NULL) && (temp > *fwidth))
			*fwidth += (temp - *fwidth);
         AssignValue(a,temp);
      break;

      case TOP:
		 temp = Value(&c->att[BOTTOM]) - size;
		 if ((fheight != NULL) && (temp < 0))
		 {
			*fheight += abs(temp);
			temp = 0;
		 }
         AssignValue(a, temp);
      break;

      case BOTTOM:
		 temp = Value(&c->att[TOP]) + size;
		 if ((fheight != NULL) && (temp > *fheight))
			*fheight += (temp - *fheight);
         AssignValue(a, temp);
      break;
   }
}




/************************************************************************
 *
 *  GetFormOffset
 *
 ************************************************************************/

static int GetFormOffset (fw, which, a)
XmFormWidget fw;
int which;
XmFormAttachment a;
{
	int o;

	a += which;
	o = a->offset;

	if (o == RESOURCE_DEFAULT) 
	{
		switch (a->type)
		{
			case XmATTACH_NONE:
			case XmATTACH_SELF:
			case XmATTACH_POSITION:
				o = 0;
			break;

			case XmATTACH_FORM:
			case XmATTACH_OPPOSITE_FORM:
				if ((which == LEFT) || (which == RIGHT))
				{
					if (fw->bulletin_board.margin_width
							== RESOURCE_DEFAULT)
						o = fw->form.horizontal_spacing;
					else
						o = fw->bulletin_board.margin_width;
				}
				else
				{
					if (fw->bulletin_board.margin_height 
							== RESOURCE_DEFAULT)
						o = fw->form.vertical_spacing;
					else
						o = fw->bulletin_board.margin_height;
				}
			break;

			case XmATTACH_WIDGET:
			case XmATTACH_OPPOSITE_WIDGET:
				if ((which == LEFT) || (which == RIGHT))
					o = fw->form.horizontal_spacing;
				else
					o = fw->form.vertical_spacing;
			break;
		}
	}

	return o;
}




/************************************************************************
 *
 *		Application Accessible External Functions
 *
 ************************************************************************/


/************************************************************************
 *
 *  XmCreateForm
 *	Create an instance of a form and return the widget id.
 *
 ************************************************************************/

#ifdef _NO_PROTO
Widget XmCreateForm (parent, name, arglist, argcount)
Widget parent;
char * name;
ArgList arglist;
Cardinal argcount;

#else /* _NO_PROTO */
Widget XmCreateForm (Widget parent, char *name, ArgList arglist, Cardinal argcount)
#endif /* _NO_PROTO */
{
   return(XtCreateWidget(name, xmFormWidgetClass, parent, arglist, argcount));
}




/************************************************************************
 *
 *  XmCreateFormDialog
 *	Create an instance of a form dialog and return the widget id.
 *
 ************************************************************************/

#ifdef _NO_PROTO
Widget XmCreateFormDialog (parent, name, arglist, argcount)
Widget parent;
char * name;
ArgList arglist;
Cardinal argcount;

#else /* _NO_PROTO */
Widget XmCreateFormDialog (Widget parent, char *name, ArgList arglist, Cardinal argcount)
#endif /* _NO_PROTO */
{
   Widget form_shell;
   Widget form;
   Arg shell_arglist[1];
   char *ds_name;

   /*  Create a pop-up shell for the form box.  */

   /* cobble up a name */
   ds_name = XtCalloc(strlen(name)+DIALOG_SUFFIX_SIZE+1,sizeof(char));
   strcpy(ds_name,name);
   strcat(ds_name,DIALOG_SUFFIX);

   /* create the widget, allow it to change sizes */
   XtSetArg (shell_arglist[0], XmNallowShellResize, True);
   form_shell = XmCreateDialogShell (parent, ds_name, shell_arglist, 1);

   XtFree(ds_name);


   /*  Create the widget  */

   form = XtCreateWidget (name, xmFormWidgetClass, 
                          form_shell, arglist, argcount);
   XtAddCallback (form, XmNdestroyCallback, _XmDestroyParentCallback, NULL);
   return (form);

}
