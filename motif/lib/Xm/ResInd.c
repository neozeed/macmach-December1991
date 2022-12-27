#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)ResInd.c	3.14 91/01/10";
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



#include <X11/Xatom.h>

#include <Xm/XmP.h>
#include <Xm/ExtObjectP.h>
#include <Xm/VendorEP.h>
#include <Xm/AtomMgr.h>

/**********************************************************************
 *
 *  Font unit processing structures and data.
 *
 **********************************************************************/

#define DEFAULT_QUAD_WIDTH 10

typedef struct
{
	Display * display;
	int h_unit, v_unit;
} XmFontUnitData;

static XmFontUnitData * font_unit_list = NULL;
static int list_size = 0;
static int list_count = 0;

static int GetFontUnit();
static int _XmGetFontUnit();


/**********************************************************************
 *
 *  BuildResources
 *	Build up a new synthetic resource list based on a combination
 *	the the widget's class and super class resource list.
 *
 **********************************************************************/

static void BuildResources (wc_resources_ptr, wc_num_resources_ptr,
	sc_resources, sc_num_resources)
XmSyntheticResource ** wc_resources_ptr;
int                *  wc_num_resources_ptr;
XmSyntheticResource *  sc_resources;
int                   sc_num_resources;
{
	XmSyntheticResource * wc_resources;
	int                  wc_num_resources;
	XmSyntheticResource * new_resources;
	int                  new_num_resources;
	register int i, j;
	Boolean override;


	wc_resources = (XmSyntheticResource *) *wc_resources_ptr;
	wc_num_resources = (int) *wc_num_resources_ptr;


	/*  If there are no new resources, just use the super class data  */

	if (wc_num_resources == 0)
	{
		*wc_resources_ptr = sc_resources;
		*wc_num_resources_ptr = sc_num_resources;
		return;
	}


	/*
	 * Allocate a new resource list to contain the combined set of
	 * resources from the class and super class.  This allocation
	 * may create to much space if there are overrides in the new
	 * resource list.  Copy sc's resources into the space.
	 */

	new_resources = (XmSyntheticResource *) 
		XtMalloc(sizeof (XmSyntheticResource) *
		(wc_num_resources + sc_num_resources));
	bcopy ((char *) sc_resources, (char *) new_resources, 
		sc_num_resources * sizeof (XmSyntheticResource));


	/*
	 * Loop through the wc resources and copy 
	 * them into the new resources
	 */

	new_num_resources = sc_num_resources;

	for (i = 0; i < wc_num_resources; i++)
	{

		/*  First check to see if this is an override  */

		override = False;
		for (j = 0; j < sc_num_resources; j++)
		{
			if (new_resources[j].resource_name == 
				wc_resources[i].resource_name)
			{
				override = True;
				new_resources[j].export_proc = 
					wc_resources[i].export_proc;
				break;
			}
		}


		/*  If it wasn't an override stuff it into the list  */

		if (override == False)
			new_resources[new_num_resources++] = wc_resources[i];
	}


	/*  Replace the resource list and count will the new one.  */

	*wc_resources_ptr = new_resources;
	*wc_num_resources_ptr = new_num_resources;
}




/**********************************************************************
 *
 *  InitializeSyntheticResources
 *	Initialize a synthetic resource list.  This is called befor
 *	Primitive, Manager and Gadgets build resources to convert the
 *	resource names to quarks in the class' synthetic processing
 *	lists.
 *
 **********************************************************************/

static void InitializeSyntheticResources (resources, num_resources)
XmSyntheticResource * resources;
int num_resources;
{
	register int i;

	for (i = 0; i < num_resources; i++)
		resources[i].resource_name = 
			(String) XrmStringToQuark (resources[i].resource_name);
}




/**********************************************************************
 *
 *  _XmBuildPrimitiveResources
 *	Build up the primitives synthetic resource processing list 
 *	by combining the super classes with this class.
 *
 **********************************************************************/

#ifdef _NO_PROTO
void _XmBuildPrimitiveResources (wc)
XmPrimitiveWidgetClass wc;
#else /* _NO_PROTO */
void _XmBuildPrimitiveResources (XmPrimitiveWidgetClass wc)
#endif /* _NO_PROTO */
{
	XmPrimitiveWidgetClass sc;

	sc = (XmPrimitiveWidgetClass) wc->core_class.superclass;

	InitializeSyntheticResources(wc->primitive_class.syn_resources,
		wc->primitive_class.num_syn_resources);

	if (sc == (XmPrimitiveWidgetClass) widgetClass) return;

	BuildResources (&(wc->primitive_class.syn_resources),
		&(wc->primitive_class.num_syn_resources),
		sc->primitive_class.syn_resources,
		sc->primitive_class.num_syn_resources);
}




/**********************************************************************
 *
 *  _XmBuildGadgetResources
 *	Build up the gadget's synthetic resource processing list 
 *	by combining the super classes with this class.
 *
 **********************************************************************/

#ifdef _NO_PROTO
void _XmBuildGadgetResources (wc)
XmGadgetClass wc;
#else /* _NO_PROTO */
void _XmBuildGadgetResources (XmGadgetClass wc)
#endif /* _NO_PROTO */
{
	XmGadgetClass sc;

	sc = (XmGadgetClass) wc->rect_class.superclass;

	InitializeSyntheticResources(wc->gadget_class.syn_resources,
		wc->gadget_class.num_syn_resources);

	if (sc == (XmGadgetClass) rectObjClass) return;

	BuildResources (&(wc->gadget_class.syn_resources),
		&(wc->gadget_class.num_syn_resources),
		sc->gadget_class.syn_resources,
		sc->gadget_class.num_syn_resources);
}




/**********************************************************************
 *
 *  _XmBuildManagerResources
 *	Build up the manager's synthetic and constraint synthetic
 *	resource processing list by combining the super classes with 
 *	this class.
 *
 **********************************************************************/

#ifdef _NO_PROTO
void _XmBuildManagerResources (wc)
XmManagerWidgetClass wc;
#else /* _NO_PROTO */
void _XmBuildManagerResources (XmManagerWidgetClass wc)
#endif /* _NO_PROTO */
{
	XmManagerWidgetClass sc;

	sc = (XmManagerWidgetClass) wc->core_class.superclass;

	InitializeSyntheticResources(wc->manager_class.syn_resources,
		wc->manager_class.num_syn_resources);

	InitializeSyntheticResources(
		wc->manager_class.syn_constraint_resources,
		wc->manager_class.num_syn_constraint_resources);

	if (sc == (XmManagerWidgetClass) constraintWidgetClass) return;

	BuildResources (&(wc->manager_class.syn_resources),
		&(wc->manager_class.num_syn_resources),
		sc->manager_class.syn_resources,
		sc->manager_class.num_syn_resources);

	BuildResources (&(wc->manager_class.syn_constraint_resources),
		&(wc->manager_class.num_syn_constraint_resources),
		sc->manager_class.syn_constraint_resources,
		sc->manager_class.num_syn_constraint_resources);
}


/**********************************************************************
 *
 *  _XmBuildExtResources
 *	Build up the ext's synthetic 
 *	resource processing list by combining the super classes with 
 *	this class.
 *
 **********************************************************************/

#ifdef _NO_PROTO
void _XmBuildExtResources (wc)
    XmExtObjectClass	wc;
#else /* _NO_PROTO */
void _XmBuildExtResources (XmExtObjectClass wc)
#endif /* _NO_PROTO */
{
	XmExtObjectClass 		sc;

	InitializeSyntheticResources(wc->ext_class.syn_resources,
				     wc->ext_class.num_syn_resources);

	if (wc != (XmExtObjectClass) xmExtObjectClass)
	  {
	      sc = (XmExtObjectClass) wc->object_class.superclass;

	      BuildResources (&(wc->ext_class.syn_resources),
			      &(wc->ext_class.num_syn_resources),
			      sc->ext_class.syn_resources,
			      sc->ext_class.num_syn_resources);
	  }
}




/**********************************************************************
 *
 *  GetValuesHook
 *	This procedure is used as the synthetic hook in Primitive,
 *	Manager, and Gadget.  It uses the synthetic resource list
 *	attached to the class, comparing it to the input resource list,
 *	and for each match, calling the function to process the get
 *	value data.
 *
 **********************************************************************/

static void GetValuesHook (w, base, resources, num_resources,
	args, num_args)
Widget w;
caddr_t base;
XmSyntheticResource * resources;
int num_resources;
ArgList args;
Cardinal num_args; /* Note that this is not a pointer */
{
	register int i, j;
	register XrmQuark quark;
	XtArgVal value;
	XtArgVal orig_value;
	int value_size;
	caddr_t value_ptr;
	
	
	/*  Loop through each argument, quarkifing the name.  Then loop  */
	/*  through each synthetic resource to see if there is a match.  */
	
	for (i = 0; i < num_args; i++)
	{
		quark = XrmStringToQuark (args[i].name);
	
		for (j = 0; j < num_resources; j++)
		{
			if ((resources[j].export_proc) &&
				(XrmQuark)(resources[j].resource_name) == quark)
			{
				value_size = resources[j].resource_size;
				value_ptr = base + resources[j].resource_offset;
	
				if (value_size == 1)
					value = (XtArgVal)(*(char *)value_ptr);
				else if (value_size == 2)
					value = (XtArgVal)(*(short *)value_ptr);
				else if (value_size == 4)
					value = (XtArgVal)(*(long *)value_ptr);

				orig_value = value;
	
				(*(resources[j].export_proc))(w, 
				  resources[j].resource_offset, &value);
	
				if (orig_value == args[i].value)
				{
					args[i].value = value;
				}
				else
				{
					value_ptr = (caddr_t) args[i].value;
	
					if (value_size == 1)
						*(char *) value_ptr = (char) value;
					else if (value_size == 2)
						*(short *) value_ptr = (short) value;
					else if (value_size == 4)
						*(long *) value_ptr = (long) value;
				}

				break;
			}
		}
	}
}




/**********************************************************************
 *
 *  ConstraintGetValuesHook
 *	When a widget is a child of a constraint manager, this function
 *	processes the synthetic arg list with for any constraint based
 *	resource processing that needs to be done.
 *
 **********************************************************************/

static void ConstraintGetValuesHook (w, args, num_args)
Widget w;
ArgList args;
Cardinal * num_args;
{
	XmManagerWidgetClass parent_wc = 
		(XmManagerWidgetClass) w->core.parent->core.widget_class;
	
	if (!XmIsManager(w->core.parent)) return;
	
	if (parent_wc->manager_class.num_syn_constraint_resources == 0)
		return;
	
	GetValuesHook (w, w->core.constraints,
		parent_wc->manager_class.syn_constraint_resources,
		parent_wc->manager_class.num_syn_constraint_resources,
		args, *num_args);
}




/**********************************************************************
 *
 *  _XmPrimitiveGetValuesHook
 *	Process the synthetic resources that need to be synthesized
 *
 **********************************************************************/

#ifdef _NO_PROTO
void _XmPrimitiveGetValuesHook (w, args, num_args)
Widget w;
ArgList args;
Cardinal * num_args;
#else /* _NO_PROTO */
void _XmPrimitiveGetValuesHook (Widget w, ArgList args, Cardinal *num_args)
#endif /* _NO_PROTO */
{
	XmPrimitiveWidgetClass wc = 
		(XmPrimitiveWidgetClass) w->core.widget_class;
	
	if (wc->primitive_class.num_syn_resources != 0)
		GetValuesHook (w, (caddr_t)w, wc->primitive_class.syn_resources,
		wc->primitive_class.num_syn_resources, args, *num_args);
	
	if (w->core.constraints != NULL) 
		ConstraintGetValuesHook (w, args, num_args);
}




/**********************************************************************
 *
 *  _XmGadgetGetValuesHook
 *	Process the synthetic resources that need to be synthesized
 *
 **********************************************************************/

#ifdef _NO_PROTO
void _XmGadgetGetValuesHook (w, args, num_args)
Widget w;
ArgList args;
Cardinal * num_args;
#else /* _NO_PROTO */
void _XmGadgetGetValuesHook (Widget w, ArgList args, Cardinal *num_args)
#endif /* _NO_PROTO */
{
	XmGadgetClass wc = (XmGadgetClass) w->core.widget_class;
	
	if (wc->gadget_class.num_syn_resources != 0)
		GetValuesHook (w, (caddr_t) w, wc->gadget_class.syn_resources,
			wc->gadget_class.num_syn_resources, args, *num_args);
	
	if (w->core.constraints != NULL)
		ConstraintGetValuesHook (w, args, num_args);
}




/**********************************************************************
 *
 *  _XmManagerGetValuesHook
 *	Process the synthetic resources that need to be synthesized
 *
 **********************************************************************/

#ifdef _NO_PROTO
void _XmManagerGetValuesHook (w, args, num_args)
Widget w;
ArgList args;
Cardinal * num_args;
#else /* _NO_PROTO */
void _XmManagerGetValuesHook (Widget w, ArgList args, Cardinal *num_args)
#endif /* _NO_PROTO */
{
	XmManagerWidgetClass wc =
		(XmManagerWidgetClass) w->core.widget_class;
	
	if (wc->manager_class.num_syn_resources != 0)
		GetValuesHook (w, (caddr_t) w, wc->manager_class.syn_resources,
		wc->manager_class.num_syn_resources, args, *num_args);
	
	if (w->core.constraints != NULL)
		ConstraintGetValuesHook (w, args, num_args);
}


/**********************************************************************
 *
 *  _XmExtGetValuesHook
 *	Process the synthetic resources that need to be synthesized
 *
 **********************************************************************/

#ifdef _NO_PROTO
void _XmExtGetValuesHook (w, args, num_args)
    Widget w;
    ArgList args;
    Cardinal * num_args;
#else /* _NO_PROTO */
void _XmExtGetValuesHook (Widget w, ArgList args, Cardinal *num_args)
#endif /* _NO_PROTO */
{
    XmExtObjectClass	 		wc;

    wc = (XmExtObjectClass)XtClass(w);
    
    if (wc->ext_class.num_syn_resources != 0)
      GetValuesHook(w, (caddr_t) w, 
		    wc->ext_class.syn_resources,
		    wc->ext_class.num_syn_resources, 
		    args, *num_args);
}



/**********************************************************************
 *
 * ImportArgs
 * Convert the value in the arg list from the application type to the 
 * appropriate internal representation by calling the import_proc 
 * specified for the given resource.
 *
 **********************************************************************/

static void ImportArgs (w, base, resources, num_resources,
	args, num_args)
Widget w;
caddr_t base;
XmSyntheticResource * resources;
int num_resources;
ArgList args;
Cardinal num_args; /* Note that this is not a pointer */
{
	register int i, j;
	register XrmQuark quark;
	XtArgVal value;
	int value_size;
	caddr_t value_ptr;
	
	
	/*  Loop through each argument, quarkifing the name.  Then loop  */
	/*  through each synthetic resource to see if there is a match.  */
	
	for (i = 0; i < num_args; i++)
	{
		quark = XrmStringToQuark (args[i].name);
	
		for (j = 0; j < num_resources; j++)
		{
			if ((resources[j].import_proc) &&
				(XrmQuark)(resources[j].resource_name) == quark)
			{
				value = args[i].value;
	
				if ((*(resources[j].import_proc))(w, 
				    resources[j].resource_offset, &value) ==
				    XmSYNTHETIC_LOAD)
				{
					value_size = resources[j].resource_size;
					value_ptr = base + resources[j].resource_offset;

					/* Load the converted value into the structure */

					if (value_size == 1)
						*(char *) value_ptr = (char) value;
					else if (value_size == 2)
						*(short *) value_ptr = (short) value;
					else if (value_size == 4)
						*(long *) value_ptr = (long) value;
				}

				break;
			}
		}
	}
}



/**********************************************************************
 *
 * _XmExtImportArgs
 * Does arg importing for sub-classes of VendorExt.
 *
 **********************************************************************/

#ifdef _NO_PROTO
void _XmExtImportArgs (w, args, num_args)
    Widget w;
    ArgList args;
    Cardinal * num_args;
#else /* _NO_PROTO */
void _XmExtImportArgs (Widget w, ArgList args, Cardinal *num_args)
#endif /* _NO_PROTO */
{
    XmExtObjectClass	 		wc;

    wc = (XmExtObjectClass)XtClass(w);
    
    if (wc->ext_class.num_syn_resources != 0)
      ImportArgs (w, (caddr_t) w, 
		  wc->ext_class.syn_resources,
		  wc->ext_class.num_syn_resources, 
		  args, *num_args);
}




/**********************************************************************
 *
 *  ImportConstraintArgs
 *	When a widget is a child of a constraint manager, this function
 *	processes the synthetic arg list with for any constraint based
 *	resource processing that needs to be done.
 *
 **********************************************************************/

static void ImportConstraintArgs (w, args, num_args)
Widget w;
ArgList args;
Cardinal * num_args;
{
	XmManagerWidgetClass parent_wc = 
		(XmManagerWidgetClass) w->core.parent->core.widget_class;
	
	if (!XmIsManager(w->core.parent)) return;
	
	if (parent_wc->manager_class.num_syn_constraint_resources == 0)
		return;
	
	ImportArgs (w, w->core.constraints,
		parent_wc->manager_class.syn_constraint_resources,
		parent_wc->manager_class.num_syn_constraint_resources,
		args, *num_args);
}




/**********************************************************************
 *
 * _XmPrimitiveImportArgs
 * Does arg importing for sub-classes of XmPrimitive.
 *
 **********************************************************************/

#ifdef _NO_PROTO
void _XmPrimitiveImportArgs (w, args, num_args)
Widget w;
ArgList args;
Cardinal * num_args;
#else /* _NO_PROTO */
void _XmPrimitiveImportArgs (Widget w, ArgList args, Cardinal *num_args)
#endif /* _NO_PROTO */
{
	XmPrimitiveWidgetClass wc =
		(XmPrimitiveWidgetClass) w->core.widget_class;
	
	if (wc->primitive_class.num_syn_resources != 0)
		ImportArgs (w, (caddr_t) w, wc->primitive_class.syn_resources,
			wc->primitive_class.num_syn_resources, args, *num_args);
	
	if (w->core.constraints != NULL) 
		ImportConstraintArgs (w, args, num_args);
}



/**********************************************************************
 *
 *  _XmGadgetImportArgs
 * Does arg importing for sub-classes of XmGadget.
 *
 **********************************************************************/

#ifdef _NO_PROTO
void _XmGadgetImportArgs (w, args, num_args)
Widget w;
ArgList args;
Cardinal * num_args;
#else /* _NO_PROTO */
void _XmGadgetImportArgs (Widget w, ArgList args, Cardinal *num_args)
#endif /* _NO_PROTO */
{
	XmGadgetClass wc = (XmGadgetClass) w->core.widget_class;
	
	if (wc->gadget_class.num_syn_resources != 0)
		ImportArgs (w, (caddr_t) w, wc->gadget_class.syn_resources,
			wc->gadget_class.num_syn_resources, args, *num_args);
	
	if (w->core.constraints != NULL)
		ImportConstraintArgs (w, args, num_args);
}




/**********************************************************************
 *
 *  _XmManagerImportArgs
 * Does arg importing for sub-classes of XmManager.
 *
 **********************************************************************/

#ifdef _NO_PROTO
void _XmManagerImportArgs (w, args, num_args)
Widget w;
ArgList args;
Cardinal * num_args;
#else /* _NO_PROTO */
void _XmManagerImportArgs (Widget w, ArgList args, Cardinal *num_args)
#endif /* _NO_PROTO */
{
	XmManagerWidgetClass wc = 
		(XmManagerWidgetClass) w->core.widget_class;
	
	if (wc->manager_class.num_syn_resources != 0)
		ImportArgs (w, (caddr_t) w, wc->manager_class.syn_resources,
			wc->manager_class.num_syn_resources, args, *num_args);
	
	if (w->core.constraints != NULL)
		ImportConstraintArgs (w, args, num_args);
}




/**********************************************************************
 *
 * _XmConvertUnits
 * Does the real work of conversion.
 * 
 **********************************************************************/

#ifdef _NO_PROTO
int _XmConvertUnits (screen, dimension, from_type, from_val, to_type)
Screen * screen;
int dimension;
register int from_type;
register int from_val;
register int to_type;
#else /* _NO_PROTO */
int _XmConvertUnits (Screen *screen, int dimension, int from_type, int from_val, int to_type)
#endif /* _NO_PROTO */
{
	register int from_val_in_mm;
	register int mm_per_pixel;
	int font_unit;
	
	
	/*  Do error checking  */
	
	if (dimension != XmHORIZONTAL && dimension != XmVERTICAL)
		return (0);
	
	if ((from_type != XmPIXELS) && 
		(from_type != Xm100TH_MILLIMETERS) &&
		(from_type != Xm1000TH_INCHES) && 
		(from_type != Xm100TH_POINTS) &&
		(from_type != Xm100TH_FONT_UNITS))
		return (0);
	
	if ((to_type != XmPIXELS) && 
		(to_type != Xm100TH_MILLIMETERS) &&
		(to_type != Xm1000TH_INCHES) && 
		(to_type != Xm100TH_POINTS) &&
		(to_type != Xm100TH_FONT_UNITS))
		return (0);
	
	/*  Check for type to same type conversions  */
	
	if (from_type == to_type)
		return (from_val);
	
	/*  Get the screen dimensional data  */
	
	if (dimension == XmHORIZONTAL)
		mm_per_pixel = 
			(WidthMMOfScreen(screen) * 100) / WidthOfScreen(screen);
	else
		mm_per_pixel = 
			(HeightMMOfScreen(screen) * 100) / HeightOfScreen(screen);
	
	if (from_type == XmPIXELS)
		from_val_in_mm = from_val * mm_per_pixel;
	else if (from_type == Xm100TH_POINTS)
		from_val_in_mm = (from_val * 353) / 1000;
	else if (from_type == Xm1000TH_INCHES)
		from_val_in_mm = (from_val * 254) / 100;
	else if (from_type == Xm100TH_MILLIMETERS)
		from_val_in_mm = from_val;
	else if (from_type == Xm100TH_FONT_UNITS)
	{
		font_unit = _XmGetFontUnit (screen, dimension);
		from_val_in_mm = from_val * font_unit * mm_per_pixel / 100;
	}
	
	
	if (to_type == XmPIXELS)
		return (from_val_in_mm / mm_per_pixel);
	else if (to_type == Xm100TH_POINTS)
		return ((from_val_in_mm * 1000) / 353);
	else if (to_type == Xm1000TH_INCHES)
		return ((from_val_in_mm * 100) / 254);
	else if (to_type == Xm100TH_MILLIMETERS)
		return (from_val_in_mm);
	else if (to_type == Xm100TH_FONT_UNITS)
	{
		font_unit = _XmGetFontUnit (screen, dimension);
		return ((from_val_in_mm * 100) / (mm_per_pixel * font_unit));
	}
}




/**********************************************************************
 *
 *  XmConvertUnits
 *	Convert a value in from_type representation to a value in
 *	to_type representation using the screen to look up the screen
 *	resolution and the dimension to denote whether to use the
 *	horizontal or vertical resolution data.
 *
 **********************************************************************/

#ifdef _NO_PROTO
int XmConvertUnits (widget, dimension, from_type, from_val, to_type)
Widget widget;
int dimension;
register int from_type;
register int from_val;
register int to_type;
#else /* _NO_PROTO */
int XmConvertUnits (Widget widget, int dimension, int from_type, int from_val, int to_type)
#endif /* _NO_PROTO */
{
	
	Screen *screen = XtScreen(widget);
	register int from_val_in_mm;
	register int mm_per_pixel;
	
	/*  Do error checking  */
	
	if (screen == NULL)
		return (0);
	if (dimension != XmHORIZONTAL && dimension != XmVERTICAL)
		return (0);
	
	return(_XmConvertUnits(screen, dimension,
		from_type, from_val, to_type));
}



/*********************************************************************
 *
 *  XmCvtToVerticalPixels
 *      Convert from a specified unit type to pixel type using
 *      the vertical resolution of the screen.
 *
 *********************************************************************/
#ifdef _NO_PROTO
int XmCvtToHorizontalPixels(screen, from_val, from_type)
Screen * screen;
register int from_val;
register int from_type;
#else /* _NO_PROTO */
int XmCvtToHorizontalPixels (Screen * screen, int from_val, 
	int from_type)
#endif /* _NO_PROTO */
{
	 /*  Do error checking  */
	 if (screen == NULL)
		return (0);
	
	return(_XmConvertUnits(screen, XmHORIZONTAL,
		from_type, from_val, XmPIXELS));
}



/**********************************************************************
 *
 *  _XmToHorizontalPixels
 *	Convert from a non-pixel unit type to pixels using the 
 *	horizontal resolution of the screen.  This function is
 *	accessed from a widget.
 *
 **********************************************************************/

#ifdef _NO_PROTO
XmImportOperator _XmToHorizontalPixels (widget, offset, value)
register Widget widget;
int offset;
XtArgVal * value;
#else /* _NO_PROTO */
XmImportOperator _XmToHorizontalPixels (Widget widget, int offset, XtArgVal *value)
#endif /* _NO_PROTO */
{
	static Screen * screen = NULL;
	static int mm_per_pixel;
	register unsigned char unit_type;
	register int value_in_mm;
	int font_unit;
	
	
	/*  Get the unit type of the widget  */
	
	if (XmIsGadget (widget))
		unit_type = ((XmGadget)(widget))->gadget.unit_type;
	else if (XmIsPrimitive (widget))
		unit_type = ((XmPrimitiveWidget)(widget))->primitive.unit_type;
	else if (XmIsManager (widget))
		unit_type = ((XmManagerWidget)(widget))->manager.unit_type;
	else if (XmIsExtObject(widget))
	  {
	      XmExtObject	extObj = (XmExtObject)widget;
	      Widget		parent = extObj->ext.logicalParent;

	      if (XmIsVendorShell(parent))
		{
		    unit_type = ((XmVendorShellExtObject)widget)->vendor.unit_type;
		}
	      else if (XmIsGadget(parent))
		{
		    unit_type =
		      ((XmGadget)(parent))->gadget.unit_type;
		}
	  }
	
	/*  Check for type to same type conversions  */
	
	if (unit_type == XmPIXELS)
		return XmSYNTHETIC_LOAD;
	/*  Pre-process FontUnit types  */
	
	if (unit_type == Xm100TH_FONT_UNITS)
	{
		font_unit = GetFontUnit (widget, XmHORIZONTAL);
		*value = (XtArgVal) ((int)*value * font_unit / 100);
		return XmSYNTHETIC_LOAD;
	}
	
	
	/*  Get the screen dimensional data  */
	
	if (screen != XtScreen (widget))
	{
		screen = XtScreen (widget);
		mm_per_pixel = 
			(WidthMMOfScreen(screen) * 100) / WidthOfScreen(screen);
	}
	
	
	/*  Convert the data  */
	
	if (unit_type == Xm100TH_POINTS)
		value_in_mm = ((int)*value * 353) / 1000;
	else if (unit_type == Xm1000TH_INCHES)
		value_in_mm = ((int)*value * 254) / 100;
	else if (unit_type == Xm100TH_MILLIMETERS)
		value_in_mm = (int)*value;
	*value = (XtArgVal)(value_in_mm / mm_per_pixel);
	return XmSYNTHETIC_LOAD;
}

/*********************************************************************
 *
 *  XmCvtToVerticalPixels
 *      Convert from a specified unit type to pixel type using
 *      the vertical resolution of the screen.
 *
 *********************************************************************/
#ifdef _NO_PROTO
int XmCvtToVerticalPixels(screen, from_val, from_type)
Screen * screen;
register int from_val;
register int from_type;
#else /* _NO_PROTO */
int XmCvtToVerticalPixels (Screen * screen, int from_val, int from_type)
#endif /* _NO_PROTO */
{
	 /*  Do error checking  */
	 if (screen == NULL)
		return (0);
	
	return(_XmConvertUnits(screen, XmVERTICAL,
		from_type, from_val, XmPIXELS));
}



/********************************************************************
 *
 *  _XmToVerticalPixels
 *	Convert from non-pixel unit type to pixels using the 
 *	vertical resolution of the screen.  This function is
 *	accessed from a widget.
 *
 **********************************************************************/

#ifdef _NO_PROTO
XmImportOperator _XmToVerticalPixels (widget, offset, value)
register Widget widget;
int offset;
XtArgVal * value;
#else /* _NO_PROTO */
XmImportOperator _XmToVerticalPixels (Widget widget, int offset, XtArgVal *value)
#endif /* _NO_PROTO */
{
	static Screen * screen = NULL;
	static int mm_per_pixel;
	register unsigned char unit_type;
	register int value_in_mm;
	int font_unit;
	
	
	/*  Get the unit type of the widget  */
	
	if (XmIsGadget (widget))
		unit_type = ((XmGadget)(widget))->gadget.unit_type;
	else if (XmIsPrimitive (widget))
		unit_type = ((XmPrimitiveWidget)(widget))->primitive.unit_type;
	else if (XmIsManager (widget))
		unit_type = ((XmManagerWidget)(widget))->manager.unit_type;
	else if (XmIsExtObject(widget))
	  {
	      XmExtObject	extObj = (XmExtObject)widget;
	      Widget		parent = extObj->ext.logicalParent;

	      if (XmIsVendorShell(parent))
		{
		    unit_type = ((XmVendorShellExtObject)widget)->vendor.unit_type;
		}
	      else if (XmIsGadget(parent))
		{
		    unit_type =
		      ((XmGadget)(parent))->gadget.unit_type;
		}
	  }
	
	/*  Check for type to same type conversions  */
	
	if (unit_type == XmPIXELS)
		return XmSYNTHETIC_LOAD;
	
	/*  Pre-process FontUnit types  */
	
	if (unit_type == Xm100TH_FONT_UNITS)
	{
		font_unit = GetFontUnit (widget, XmVERTICAL);
		*value = (XtArgVal)((int)*value * font_unit / 100);
		return XmSYNTHETIC_LOAD;
	}
	
	
	/*  Get the screen dimensional data  */
	
	if (screen != XtScreen (widget))
	{
		screen = XtScreen (widget);
		mm_per_pixel = 
			(HeightMMOfScreen(screen) * 100) / HeightOfScreen(screen);
	}
	
	/*  Convert the data  */
	
	if (unit_type == Xm100TH_POINTS)
		value_in_mm = ((int)*value * 353) / 1000;
	else if (unit_type == Xm1000TH_INCHES)
		value_in_mm = ((int)*value * 254) / 100;
	else if (unit_type == Xm100TH_MILLIMETERS)
		value_in_mm = (int)*value;
	
	*value = (XtArgVal)(value_in_mm / mm_per_pixel);
	return XmSYNTHETIC_LOAD;
}


/*********************************************************************
*
*
*  XmCvtFromHorizontalPixels
*      Convert from a pixel unit type to specified type using
*      the horizontal resolution of the screen.
*
 **********************************************************************/
#ifdef _NO_PROTO
int XmCvtFromHorizontalPixels(screen, from_val, to_type)
Screen * screen;
register int from_val;
register int to_type;
#else /* _NO_PROTO */
int XmCvtFromHorizontalPixels (Screen * screen, int from_val, 
	int to_type)
#endif /* _NO_PROTO */
{
												
	/*  Do error checking  */

	if (screen == NULL)
		return (0);
	
	return(_XmConvertUnits(screen, 
		XmHORIZONTAL, XmPIXELS, from_val, to_type));
}


/**********************************************************************
 *
 *  _XmFromHorizontalPixels
 *	Convert from a pixel unit type to a non-pixels using the 
 *	horizontal resolution of the screen.  This function is
 *	accessed from a getvalues hook table.
 *
 **********************************************************************/

#ifdef _NO_PROTO
void _XmFromHorizontalPixels (widget, offset, value)
register Widget widget;
int offset;
XtArgVal * value;
#else /* _NO_PROTO */
void _XmFromHorizontalPixels (Widget widget, int offset, XtArgVal *value)
#endif /* _NO_PROTO */
{
	static Screen * screen = NULL;
	static int mm_per_pixel;
	register unsigned char unit_type;
	register int value_in_mm;
	int font_unit;
	
	/*  Get the unit type of the widget  */
	
	if (XmIsGadget (widget))
		unit_type = ((XmGadget)(widget))->gadget.unit_type;
	else if (XmIsPrimitive (widget))
		unit_type = ((XmPrimitiveWidget)(widget))->primitive.unit_type;
	else if (XmIsManager (widget))
		unit_type = ((XmManagerWidget)(widget))->manager.unit_type;
	else if (XmIsExtObject(widget))
	  {
	      XmExtObject	extObj = (XmExtObject)widget;
	      Widget		parent = extObj->ext.logicalParent;

	      if (XmIsVendorShell(parent))
		{
		    unit_type = ((XmVendorShellExtObject)widget)->vendor.unit_type;
		}
	      else if (XmIsGadget(parent))
		{
		    unit_type =
		      ((XmGadget)(parent))->gadget.unit_type;
		}
	  }
	
	
	/*  Check for type to same type conversions  */
	
	if (unit_type == XmPIXELS)
		return;
	
	/*  Pre-process FontUnit types  */
	
	if (unit_type == Xm100TH_FONT_UNITS)
	{
		font_unit = GetFontUnit (widget, XmHORIZONTAL);
		*value = (XtArgVal)(*value) * 100 / font_unit;
	}
	
	
	/*  Get the screen dimensional data  */
	
	if (screen != XtScreen (widget))
	{
		screen = XtScreen (widget);
		mm_per_pixel =
			(WidthMMOfScreen(screen) * 100) / WidthOfScreen(screen);
	}
	
	
	/*  Convert the data  */
	
	value_in_mm = ((int)*value) * mm_per_pixel;
	
	if (unit_type == Xm100TH_POINTS)
		*value = (XtArgVal) ((value_in_mm * 1000) / 353);
	else if (unit_type == Xm1000TH_INCHES)
		*value = (XtArgVal) ((value_in_mm * 100) / 254);
	else if (unit_type == Xm100TH_MILLIMETERS)
		*value = (XtArgVal) (value_in_mm);
}



/*********************************************************************
*
*
*  XmCvtFromVerticalPixels
*      Convert from a pixel unit type to specified type using
*      the horizontal resolution of the screen.
*
 **********************************************************************/
#ifdef _NO_PROTO
int XmCvtFromVerticalPixels(screen, from_val, to_type)
Screen * screen;
register int from_val;
register int to_type;
#else /* _NO_PROTO */
int XmCvtFromVerticalPixels (Screen * screen, int from_val, 
	int to_type)
#endif /* _NO_PROTO */
{
												
	/*  Do error checking  */

	if (screen == NULL)
		return (0);
	
	return(_XmConvertUnits(screen, 
		XmVERTICAL, XmPIXELS, from_val, to_type));
}



/**********************************************************************
 *
 *  _XmFromVerticalPixels
 *	Convert from pixel unit type to non-pixels using the 
 *	vertical resolution of the screen.  This function is
 *	accessed from a getvalues hook table.
 *
 **********************************************************************/

#ifdef _NO_PROTO
void _XmFromVerticalPixels (widget, offset, value)
register Widget widget;
int offset;
XtArgVal * value;
#else /* _NO_PROTO */
void _XmFromVerticalPixels (Widget widget, int offset, XtArgVal *value)
#endif /* _NO_PROTO */
{
	static Screen * screen = NULL;
	static int mm_per_pixel;
	register unsigned char unit_type;
	register int value_in_mm;
	int font_unit;
	
	
	/*  Get the unit type of the widget  */
	
	if (XmIsGadget (widget))
		unit_type = ((XmGadget)(widget))->gadget.unit_type;
	else if (XmIsPrimitive (widget))
		unit_type = ((XmPrimitiveWidget)(widget))->primitive.unit_type;
	else if (XmIsManager (widget))
		unit_type = ((XmManagerWidget)(widget))->manager.unit_type;
	else if (XmIsExtObject(widget))
	  {
	      XmExtObject	extObj = (XmExtObject)widget;
	      Widget		parent = extObj->ext.logicalParent;

	      if (XmIsVendorShell(parent))
		{
		    unit_type = ((XmVendorShellExtObject)widget)->vendor.unit_type;
		}
	      else if (XmIsGadget(parent))
		{
		    unit_type =
		      ((XmGadget)(parent))->gadget.unit_type;
		}
	  }
	
	
	/*  Check for type to same type conversions  */
	
	if (unit_type == XmPIXELS)
		return;
	
	/*  Pre-process FontUnit types  */
	
	if (unit_type == Xm100TH_FONT_UNITS)
	{
		font_unit = GetFontUnit (widget, XmVERTICAL);
		*value = (XtArgVal)((int)*value) * 100 / font_unit;
	}
	
	
	/*  Get the screen dimensional data  */
	
	if (screen != XtScreen (widget))
	{
		screen = XtScreen (widget);
		mm_per_pixel =
			(HeightMMOfScreen(screen) * 100) / HeightOfScreen(screen);
	}
	
	
	/*  Convert the data  */
	
	value_in_mm = ((int)*value) * mm_per_pixel;
	
	if (unit_type == Xm100TH_POINTS)
		*value = (XtArgVal) ((value_in_mm * 1000) / 353);
	else if (unit_type == Xm1000TH_INCHES)
		*value = (XtArgVal) ((value_in_mm * 100) / 254);
	else if (unit_type == Xm100TH_MILLIMETERS)
		*value = (XtArgVal) (value_in_mm);
}



/**********************************************************************
 *
 * _XmSortResourceList
 * This procedure ensures the XmNunitType occurs first in the class 
 * resource list.  By doing this, the coreClass resource converters 
 * for x,y,width,height,and borderWidth are guaranteed to have a valid 
 * unitType.
 **********************************************************************/

#ifdef _NO_PROTO
void _XmSortResourceList(list,len)
XrmResource *list[];
Cardinal len;
#else /* _NO_PROTO */
void _XmSortResourceList (XrmResource *list [], Cardinal len)
#endif /* _NO_PROTO */
{
	static Boolean first_time = TRUE;
	static XrmQuark unitQ;
	int n,i;
	XrmResource *p;

	if (first_time)
	{
		unitQ = XrmStringToQuark(XmNunitType);
		first_time = FALSE;
	}

	for (n=0; n < len; n++)
		if (list[n]->xrm_name == unitQ)
		{
			p = list[n];
			break;
		}
	
	for (i=n; i > 0; i--)
		list[i] = list[i-1];
	
	list[0] = p;
}




/**********************************************************************
 *
 * _XmUnitTypeDefault
 * This procedure is called as the resource default XtRCallProc
 * to default the unit type resource.  This procedure supports 
 * the propagation of unit type from parent to child.
 *
 **********************************************************************/

#ifdef _NO_PROTO
void _XmUnitTypeDefault (widget, offset, value)
Widget widget;
int offset;
XrmValue * value;
#else /* _NO_PROTO */
void _XmUnitTypeDefault (Widget widget, int offset, XrmValue *value)
#endif /* _NO_PROTO */
{
	static unsigned char unit_type;

	value->size = sizeof(unit_type);
	value->addr = (caddr_t) &unit_type;

	if (XmIsManager(widget->core.parent))
		unit_type = 
			((XmManagerWidget)(widget->core.parent))->manager.unit_type;
	else
		unit_type = XmPIXELS;
}



/**********************************************************************
 **********************************************************************

	Font unit handling functions

 **********************************************************************
 **********************************************************************/

/**********************************************************************
 *
 *  XmSetFontUnits
 *	Set the font_unit value for the display.  These values can
 *	then be used later to process the font unit conversions.
 *
 **********************************************************************/
#ifdef _NO_PROTO
void XmSetFontUnits (display, h_value, v_value)
Display * display;
int h_value;
int v_value;
#else /* _NO_PROTO */
void XmSetFontUnits (Display *display, int h_value, int v_value)
#endif /* _NO_PROTO */
{
	register int i;
	
	/*  See if the display is already in the list  */
	
	for (i = 0; i < list_count; i++)
		if ((font_unit_list + i)->display == display)
		{
			(font_unit_list + i)->h_unit = h_value;
			(font_unit_list + i)->v_unit = v_value;
			return;
		}
	
	/*  See if more space is needed  */
	
	if (list_size == list_count)
	{
		list_size += 10;
		font_unit_list =
			(XmFontUnitData *) XtRealloc (font_unit_list, 
			sizeof(XmFontUnitData) * list_size);
	}
	
	
	/*  Add the new display and value to the font list  */
	
	(font_unit_list + list_count)->display = display;
	(font_unit_list + list_count)->h_unit = h_value;
	(font_unit_list + list_count)->v_unit = v_value;
	list_count++;
}

/* DEPRECATED */
#ifdef _NO_PROTO
void XmSetFontUnit (display, value)
Display * display;
int value;
#else /* _NO_PROTO */
void XmSetFontUnit (Display *display, int value)
#endif /* _NO_PROTO */
{
	XmSetFontUnits(display, value, value);
}




/**********************************************************************
 *
 *  GetFontUnit
 *	Get the font unit for the provided display.  If one is not
 *	found get the default for the display by accessing the default
 *	font in the resource data base or by using the default font name.
 *
 **********************************************************************/

static int GetFontUnit (widget, dimension)
Widget widget;
int dimension;
{
	Display * display = XtDisplay (widget);
	register int i;
	XFontStruct * font_struct;
	unsigned long font_unit_return;
	int h_unit, v_unit;
	
	static XtResource resources[] =
	{
		{ 
			XmNfont, XmCFont, XmRFontStruct, 
			sizeof (XFontStruct *), 0, XmRString, "Fixed" 
		}
	};
	
	/*  See if the display is already in the list  */
	
	for (i = 0; i < list_count; i++)
		if ((font_unit_list + i)->display == display)
		{
			if (dimension == XmHORIZONTAL)
				return((font_unit_list + i)->h_unit);
			else
				return((font_unit_list + i)->v_unit);
		}
	
	
	/*  Get the global font or default font by making a resource      */
	/*  data base query.  If we get a font struct, get the value for  */
	/*  the QUAD_WIDTH otherwise set the font_unit to a default.     */
	
	XtGetApplicationResources (widget, &font_struct,
		resources, 1, NULL, 0);
	
	if (font_struct != NULL)
	{
		unsigned long pixel_s, avg_w, point_s, resolution_y;
		Atom xa_average_width, xa_pixel_size, xa_resolution_y;

		xa_average_width = XmInternAtom(display, "AVERAGE_WIDTH",
			TRUE);
		xa_pixel_size = XmInternAtom(display, "PIXEL_SIZE",
			TRUE);
		xa_resolution_y = XmInternAtom(display, "RESOLUTION_Y",
			TRUE);
		
		/* Horizontal units */

		if (xa_average_width && XGetFontProperty(font_struct,
			xa_average_width, &avg_w))
		{
			h_unit = ((float) (avg_w / 10) + 0.5);
		}
		else if (XGetFontProperty (font_struct, XA_QUAD_WIDTH, 
			&font_unit_return))
		{
			h_unit = font_unit_return;
		}
		else
		{
			h_unit = ((font_struct->min_bounds.width
				+ font_struct->max_bounds.width) / 2.3) + 0.5;
		}

		/* Vertical units */
		if (XGetFontProperty(font_struct, xa_pixel_size, &pixel_s))
		{
			v_unit = (((float) pixel_s) / 1.8) + 0.5;
		}
		else if ((XGetFontProperty(font_struct,
				XA_POINT_SIZE, &point_s))
			&&
			(XGetFontProperty(font_struct, xa_resolution_y,
				&resolution_y)))
		{
			float ps, ry, tmp;

			ps = point_s;
			ry = resolution_y;

			tmp = (ps * ry) / 1400.0;

			v_unit = (int) (tmp + 0.5);
		}
		else
		{
			v_unit = ((float) (font_struct->max_bounds.ascent 
				+ font_struct->max_bounds.descent) / 2.2) + 0.5;
		}
	
		XFreeFont (display, font_struct);
	}
	else
	{
		h_unit = DEFAULT_QUAD_WIDTH;
		v_unit = DEFAULT_QUAD_WIDTH;
	}
	
	XmSetFontUnits (display, h_unit, v_unit);

	if (dimension == XmHORIZONTAL)
		return(h_unit);
	else
		return(v_unit);
}




/**********************************************************************
 *
 *  _XmGetFontUnit
 * Same as above, but this one does it the hard way.  The reason
 * for doing it the hard way is so that Resolution Independent
 * data can be dealt with in resource converters.
 *
 **********************************************************************/

static int _XmGetFontUnit (screen, dimension)
Screen * screen;
int dimension;
{
	Display * display = screen->display;
	register int i;
	XFontStruct * font_struct = NULL;
	unsigned long font_unit_return;
	XrmQuark names[3], classes[3];
	String app_name, app_class;
	static Boolean first_time = TRUE;
	static XrmQuark appNameQ;
	static XrmQuark appClassQ;
	static XrmQuark fontNameQ;
	static XrmQuark fontClassQ;
	static XrmQuark fontRepQ;
	XrmRepresentation rep;
	XrmValue db_value;
	int h_unit, v_unit;
	
	if (first_time)
	{
		fontNameQ = XrmStringToQuark(XmNfont);
		fontClassQ = XrmStringToQuark(XmCFont);
		fontRepQ = XrmStringToQuark(XmRFontStruct);
		first_time = FALSE;
	}

	/*  See if the display is already in the list  */
	
	for (i = 0; i < list_count; i++)
		if ((font_unit_list + i)->display == display)
		{
			if (dimension == XmHORIZONTAL)
				return((font_unit_list + i)->h_unit);
			else
				return((font_unit_list + i)->v_unit);
		}
	
	XtGetApplicationNameAndClass(display, &app_name, &app_class);
	appNameQ = XrmStringToQuark(app_name);
	appClassQ = XrmStringToQuark(app_class);

	names[0] = appNameQ;
	names[1] = fontNameQ;
	names[2] = NULL;
	
	classes[0] = appClassQ;
	classes[1] = fontClassQ;
	classes[2] = NULL;

	if (XrmQGetResource(XtDatabase(display), names, classes,
		&rep, &db_value))
	{
		if (rep == fontRepQ)
			bcopy(db_value.addr, &font_struct, sizeof(font_struct));
		else
		{
			XrmValue convert_to;

			/* To hack in the footsteps of Xt Initialize.c for R4 */
			WidgetRec junk_w;
			bzero(&junk_w, sizeof(junk_w));
			junk_w.core.self = &junk_w;
			junk_w.core.widget_class = coreWidgetClass;
			junk_w.core.screen = screen;
			XtInitializeWidgetClass(coreWidgetClass);

			convert_to.addr = (caddr_t) &font_struct;
			convert_to.size = sizeof(font_struct);
			if (!XtConvertAndStore(&junk_w, XrmQuarkToString(rep),
				&db_value, XmRFontStruct, &convert_to))
			{
				font_struct = NULL;
			}
		}
	}

	if (font_struct != NULL)
	{
		unsigned long avg_w, point_s, pixel_s, resolution_y;
		Atom xa_average_width, xa_pixel_size, xa_resolution_y;

		xa_average_width = XmInternAtom(display, "AVERAGE_WIDTH",
			TRUE);
		xa_pixel_size = XmInternAtom(display, "PIXEL_SIZE",
			TRUE);
		xa_resolution_y = XmInternAtom(display, "RESOLUTION_Y",
			TRUE);
		
		/* Horizontal units */

		if (xa_average_width && XGetFontProperty(font_struct,
			xa_average_width, &avg_w))
		{
			h_unit = ((float) (avg_w / 10) + 0.5);
		}
		else if (XGetFontProperty (font_struct, XA_QUAD_WIDTH, 
			&font_unit_return))
		{
			h_unit = font_unit_return;
		}
		else
		{
			h_unit = ((font_struct->min_bounds.width
				+ font_struct->max_bounds.width) / 2.3) + 0.5;
		}

		/* Vertical units */
		if (XGetFontProperty(font_struct, xa_pixel_size, &pixel_s))
		{
			v_unit = (((float) pixel_s) / 1.8) + 0.5;
		}
		else if ((XGetFontProperty(font_struct,
				XA_POINT_SIZE, &point_s))
			&&
			(XGetFontProperty(font_struct, xa_resolution_y,
				&resolution_y)))
		{
			float ps, ry, tmp;

			ps = point_s;
			ry = resolution_y;

			tmp = (ps * ry) / 1400.0;

			v_unit = (int) (tmp + 0.5);
		}
		else
		{
			v_unit = ((float) (font_struct->max_bounds.ascent 
				+ font_struct->max_bounds.descent) / 2.2) + 0.5;
		}
	
		XFreeFont (display, font_struct);
	}
	else
	{
		h_unit = DEFAULT_QUAD_WIDTH;
		v_unit = DEFAULT_QUAD_WIDTH;
	}
	
	XmSetFontUnits (display, h_unit, v_unit);

	if (dimension == XmHORIZONTAL)
		return(h_unit);
	else
		return(v_unit);
}
