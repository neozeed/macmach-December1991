#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)ExtObject.c	3.7 91/01/10";
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

#include <Xm/ExtObjectP.h>
#include "BaseClassI.h"

/* forward declarations for internal functions */



/***************************************************************************
 *
 * ExtObject Resources
 *
 ***************************************************************************/


static void UseParent(w, offset, value)
    Widget 	w;
    int		offset;
    XrmValue	*value;
{
    value->addr = (XtPointer) &(w->core.parent);
}

#define Offset(field) XtOffset(XmExtObject, ext.field)

static XtResource extResources[] =
{
    {
	XmNlogicalParent,
	XmCLogicalParent, XmRWidget, sizeof (Widget),
	Offset (logicalParent),
	XmRCallProc, (XtPointer)UseParent,
    },
    {
	XmNextensionType,
	XmCExtensionType, XmRExtensionType, sizeof (unsigned char),
	Offset (extensionType),
	XmRImmediate, (XtPointer)XmDEFAULT_EXTENSION,
    },
};
#undef Offset

/* Foward reference for class routines */

static void		ClassInitialize();
static void		ClassPartInitialize();
static void 		Initialize();
static Boolean 		SetValues();
static void 		Realize();
static void		GetValuesHook();
static void		Destroy();
static void 		ChangeManaged();

/* forward reference for utility routines */

static void		InitializePrehook();
static Boolean		SetValuesPrehook();
static void 		GetValuesPrehook();
#ifndef NO_CLASS_PART_INIT_HOOK
static void		ClassPartInitPrehook();
static void		ClassPartInitPosthook();
#endif /* NO_CLASS_PART_INIT_HOOK */

static XmBaseClassExtRec       myBaseClassExtRec = {
    NULL,                                     /* Next extension       */
    NULLQUARK,                                /* record type XmQmotif */
    XmBaseClassExtVersion,                    /* version              */
    sizeof(XmBaseClassExtRec),                /* size                 */
    InitializePrehook,		              /* initialize prehook   */
    SetValuesPrehook,		              /* set_values prehook   */
    NULL,			              /* initialize posthook  */
    NULL,			              /* set_values posthook  */
    NULL,				      /* secondary class      */
    NULL,			              /* creation proc        */
    NULL,	                              /* getSecRes data       */
    {NULL},                                   /* fast subclass        */
    GetValuesPrehook,		              /* get_values prehook   */
    NULL,			              /* get_values posthook  */
#ifndef NO_CLASS_PART_INIT_HOOK
    ClassPartInitPrehook,		      /* class_part_prehook   */
    ClassPartInitPosthook,		      /* class_part_posthook  */
    NULL,	 			      /* compiled_ext_resources*/   
    NULL,	 			      /* ext_resources       	*/   
    0,					      /* resource_count     	*/   
    FALSE,				      /* use_sub_resources	*/
#endif /* NO_CLASS_PART_INIT_HOOK */
};

XmExtClassRec xmExtClassRec = {
    {	
	(WidgetClass) &objectClassRec, /* superclass 		*/   
	"dynamic",			/* class_name 		*/   
	sizeof(XmExtRec),	 	/* size 		*/   
	ClassInitialize, 		/* Class Initializer 	*/   
	ClassPartInitialize, 		/* class_part_init 	*/ 
	FALSE, 				/* Class init'ed ? 	*/   
	Initialize, 			/* initialize         	*/   
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
	Destroy,			/* destroy            	*/   
	NULL,		 		/* resize             	*/   
	NULL, 				/* expose             	*/   
	SetValues,	 		/* set_values         	*/   
	NULL, 				/* set_values_hook      */ 
	NULL,			 	/* set_values_almost    */ 
	GetValuesHook,			/* get_values_hook      */ 
	NULL, 				/* accept_focus       	*/   
	XtVersion, 			/* intrinsics version 	*/   
	NULL, 				/* callback offsets   	*/   
	NULL,				/* tm_table           	*/   
	NULL, 				/* query_geometry       */ 
	NULL, 				/* display_accelerator  */ 
	(XtPointer)&myBaseClassExtRec,/* extension         */ 
    },	
    {
	NULL,				/* synthetic resources	*/
	0,				/* num syn resources	*/
    },
};

externaldef(xmextobjectclass) WidgetClass 
  xmExtObjectClass = (WidgetClass) (&xmExtClassRec);

/************************************************************************
 *
 *  ClassInitialize
 *
 ************************************************************************/

static void ClassInitialize()
{
    myBaseClassExtRec.record_type = XmQmotif;
}

#ifndef NO_CLASS_PART_INIT_HOOK
/************************************************************************
 *
 *  ClassPartInitPrehook
 *
 ************************************************************************/
static void ClassPartInitPrehook (w)
    WidgetClass w;
{
    XmExtObjectClass wc = (XmExtObjectClass) w;
    
    if ((WidgetClass)wc != xmExtObjectClass)
      {
	  XmBaseClassExt	*scePtr;
	  XmExtObjectClass sc = (XmExtObjectClass) w->core_class.superclass;

	  scePtr = _XmGetBaseClassExtPtr(sc, XmQmotif);
	  /*
	   * if our superclass uses sub resources, then we need to
	   * temporarily fill it's core resource fields so that objectClass
	   * classPartInit will be able to find them for merging. We
	   * assume that we only need to set things up for the
	   * superclass and not any deeper ancestors
	   */
	  if ((*scePtr)->use_sub_resources)
	    {
		sc->object_class.resources = 
		  (*scePtr)->compiled_ext_resources;
		sc->object_class.num_resources = 
		  (*scePtr)->num_ext_resources;
	    }
      }
}
/************************************************************************
 *
 *  ClassPartInitPosthook
 *
 ************************************************************************/
static void ClassPartInitPosthook (w)
    WidgetClass w;
{
    XmExtObjectClass 	wc = (XmExtObjectClass) w;
    Cardinal		junk;
    XmBaseClassExt	*wcePtr;

    wcePtr = _XmGetBaseClassExtPtr(wc, XmQmotif);
    
    if ((WidgetClass)wc != xmExtObjectClass)
      {
	  XmExtObjectClass sc = (XmExtObjectClass) w->core_class.superclass;
	  XmBaseClassExt	*scePtr;

	  scePtr = _XmGetBaseClassExtPtr(sc, XmQmotif);

	  if ((*scePtr) && (*scePtr)->use_sub_resources)
	    {
#ifdef CALLBACKS_USE_RES_LIST
		sc->object_class.resources = NULL;
		sc->object_class.num_resources = 0;
#endif
	    }
      }
    if ((*wcePtr) && (*wcePtr)->use_sub_resources)
      {
	  /*
	   * put our compiled resources back and zero out oject class so
	   * it's invisible to object class create processing
	   */
	  (*wcePtr)->compiled_ext_resources =
	    wc->object_class.resources;
	  (*wcePtr)->num_ext_resources = 
	    wc->object_class.num_resources;
#ifdef CALLBACKS_USE_RES_LIST
	  /*
	   * Xt currently uses the offset table to do callback
	   * resource processing. This means that we can't use this
	   * trick of nulling out the resource list in order to avoid
	   * an extra round of resource processing
	   */
	  wc->object_class.resources = NULL;
	  wc->object_class.num_resources = 0;
#endif
      }
}
#endif /* NO_CLASS_PART_INIT_HOOK */

/************************************************************************
 *
 *  ClassPartInitialize
 *    Set up the inheritance mechanism for the routines exported by
 *    vendorShells class part.
 *
 ************************************************************************/

static void ClassPartInitialize (w)
    WidgetClass w;
    
{
    XmExtObjectClass wc = (XmExtObjectClass) w;
    XmExtObjectClass sc =
      (XmExtObjectClass) wc->object_class.superclass;
    
    if (wc == (XmExtObjectClass)xmExtObjectClass)
      return;
    _XmBaseClassPartInitialize(w);
    _XmBuildExtResources(wc);
}

static void InitializePrehook(req, new, args, num_args)
    Widget	req, new;
    ArgList	args;
    Cardinal	*num_args;
{
#ifndef NO_CLASS_PART_INIT_HOOK
    XmBaseClassExt		*wcePtr;
    XmExtObject			ne = (XmExtObject) new;
    XmExtObject			re = (XmExtObject) req;
    XmExtObjectClass		ec = (XmExtObjectClass) XtClass(new);
    Widget			resParent = ne->ext.logicalParent;
    XmWidgetExtData		extData;


    wcePtr = _XmGetBaseClassExtPtr(ec, XmQmotif);

    if ((*wcePtr)->use_sub_resources)
      {
	  /*
	   * get a uncompiled resource list to use with
	   * XtGetSubresources. We can't do this in
	   * ClassPartInitPosthook because Xt doesn't set class_inited at
	   * the right place and thereby mishandles the
	   * XtGetResourceList call
	   */
	  if ((*wcePtr)->ext_resources == NULL)
	    {
		ec->object_class.resources =
		  (*wcePtr)->compiled_ext_resources;
		ec->object_class.num_resources =		
		  (*wcePtr)->num_ext_resources;

		XtGetResourceList(ec,
				  &((*wcePtr)->ext_resources),
				  &((*wcePtr)->num_ext_resources));

#ifdef CALLBACKS_USE_RES_LIST
		ec->object_class.resources = NULL;
		ec->object_class.num_resources = 0;
#endif /* CALLBACKS_USE_RES_LIST */
	    }
	  XtGetSubresources(XtParent(new),
			    (XtPointer)new,
			    NULL, NULL,
			    (*wcePtr)->ext_resources,
			    (*wcePtr)->num_ext_resources,
			    args, *num_args);
#endif /* NO_CLASS_PART_INIT_HOOK */
      }
}


static void Initialize(req, new, args, num_args)
    Widget	req, new;
    ArgList	args;
    Cardinal	*num_args;
{
#ifndef NO_CLASS_PART_INIT_HOOK
    XmBaseClassExt		*wcePtr;
    XmExtObject			ne = (XmExtObject) new;
    XmExtObject			re = (XmExtObject) req;
    XmExtObjectClass		ec = (XmExtObjectClass) XtClass(new);
    Widget			resParent = ne->ext.logicalParent;
    XmWidgetExtData		extData;


    wcePtr = _XmGetBaseClassExtPtr(ec, XmQmotif);

    if (!(*wcePtr)->use_sub_resources)
      {
	  if (resParent)
	    {
		extData = (XmWidgetExtData) XtCalloc(sizeof(XmWidgetExtDataRec), 1);
		_XmPushWidgetExtData(resParent, extData, ne->ext.extensionType);
		
		extData->widget = new;
		extData->reqWidget = (Widget)
		  XtMalloc(XtClass(new)->core_class.widget_size);
		bcopy((char *)req, (char *)extData->reqWidget,
		      XtClass(new)->core_class.widget_size);
		
		/*  Convert the fields from unit values to pixel values  */
		
		_XmExtImportArgs(new, args, num_args);
	    }
      }
#endif /* NO_CLASS_PART_INIT_HOOK */
}

static Boolean SetValuesPrehook(req, curr, new, args, num_args)
    Widget	req, curr, new;
    ArgList	args;
    Cardinal	*num_args;
{
#ifndef NO_CLASS_PART_INIT_HOOK
    XmExtObjectClass		ec = (XmExtObjectClass) XtClass(new);
    XmBaseClassExt		*wcePtr;

    wcePtr = _XmGetBaseClassExtPtr(ec, XmQmotif);

    if ((*wcePtr)->use_sub_resources)
      {
	  XtSetSubvalues((XtPointer)new,
			 (*wcePtr)->ext_resources,
			 (*wcePtr)->num_ext_resources,
			 args, *num_args);
      }
#endif /* NO_CLASS_PART_INIT_HOOK */
    return False;
}

static void GetValuesPrehook(new, args, num_args)
    Widget	new;
    ArgList	args;
    Cardinal	*num_args;
{
#ifndef NO_CLASS_PART_INIT_HOOK
    XmExtObjectClass		ec = (XmExtObjectClass) XtClass(new);
    XmBaseClassExt		*wcePtr;

    wcePtr = _XmGetBaseClassExtPtr(ec, XmQmotif);

    if ((*wcePtr)->use_sub_resources)
      {
	  XtGetSubvalues((XtPointer)new,
			 (*wcePtr)->ext_resources,
			 (*wcePtr)->num_ext_resources,
			 args, *num_args);
      }
#endif /* NO_CLASS_PART_INIT_HOOK */
}

/************************************************************************
 *
 *  SetValues
 *
 ************************************************************************/
/* ARGSUSED */
static Boolean SetValues(old,ref,new, args, num_args)
    Widget	old, ref, new;
    ArgList	args;
    Cardinal	*num_args;
{
    XmExtObject			ne = (XmExtObject) new;
    XmExtObject			re = (XmExtObject) ref;
    XmExtObject			oe = (XmExtObject) old;
    XmExtObjectClass		ec = (XmExtObjectClass) XtClass(new);
    Widget			resParent = ne->ext.logicalParent;
    XmWidgetExtData		ext = _XmGetWidgetExtData(resParent, ne->ext.extensionType);
    Cardinal			extSize;

    if (resParent)
      {
	  extSize = XtClass(new)->core_class.widget_size;
	  
	  ext->widget = new;
	  
	  ext->oldWidget = (Widget) XtMalloc(extSize);
	  bcopy((char *)old, (char *)ext->oldWidget, extSize); 
	  
	  ext->reqWidget = (Widget) XtMalloc(extSize);
	  bcopy((char *)ref, (char *)ext->reqWidget, extSize); 
	  
	  /*  Convert the necessary fields from unit values to pixel values  */
	  
	  _XmExtImportArgs(new, args, num_args);
      }
    return FALSE;
}

/************************************************************************
 *
 *  SetValues
 *
 ************************************************************************/
/* ARGSUSED */
static void GetValuesHook(new, args, num_args)
    Widget	new;
    ArgList	args;
    Cardinal	*num_args;
{
    XmExtObject			ne = (XmExtObject) new;
    Widget			resParent = ne->ext.logicalParent;
    
    if (resParent)
      {
	  XmWidgetExtData		ext =
	    _XmGetWidgetExtData(resParent, ne->ext.extensionType);
	  extern void			_XmExtGetValuesHook();
	  
	  ext->widget = new;
	  
	  _XmExtGetValuesHook(new, args, num_args);
      }
}

/************************************************************************
 *
 *  Destroy
 *
 ************************************************************************/
static void Destroy(extObj)
    XmExtObject	extObj;
{
    Widget			resParent = extObj->ext.logicalParent;
 
    if (resParent)
      {
	  XmWidgetExtData		extData;
	  
	  _XmPopWidgetExtData(resParent, 
			      &extData,
			      extObj->ext.extensionType);

#ifdef notdef
	  /*
	   * we can't do this here cause the gadgets have already
	   * freed it |||
	   */
	  XtFree(extData->reqWidget);
#endif /* notdef */
	  XtFree(extData);
      }
}


