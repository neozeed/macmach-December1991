#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "@(#)LabelG.c	3.23 91/01/10";
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
#include <ctype.h>
#include <X11/keysymdef.h>
#include <Xm/XmP.h>
#include <X11/ShellP.h>
#include <Xm/LabelGP.h>
#include <Xm/CacheP.h>
#include <Xm/ExtObjectP.h>
#include <Xm/BaseClassI.h>

#define Pix(w)		LabG_Pixmap(w)
#define Pix_insen(w)	LabG_PixmapInsensitive(w)


/* Warning Messages */

#define LABEL_TYPE_MESSAGE "Invalid XmNlabelType "

#define ALIGNMENT_MESSAGE "Invalid value in XmNalignment"

#define STR_DIR_MESSAGE "Invalid value in XmNstringDirection"

#define CS_STRING_MESSAGE " Invalid XmNlabelString - must be a compound string "

#define ACC_MESSAGE " Invalid XmNacceleratorText - must be a compound string "


/* Core Class functions  */

static void    ClassInitialize();
static void    ClassPartInitialize();
static void    Initialize();
static void    SetSize();
static void    Redisplay();
static Boolean SetValues();
static void    Destroy();
static XtGeometryResult QueryGeometry();
static void SetValuesAlmost();

/* Get/Set Values Hook Functions */

static void GetLabelString();
static void GetAccelerator();
static void GetAcceleratorText();
static void GetMnemonicCharset();

/* Initialize and SetValues Pre and Post hooks */

static void InitializePosthook();
static Boolean SetValuesPrehook();
static Boolean SetValuesPosthook();
static void GetValuesPrehook();
static void GetValuesPosthook();

/*  Gadget Class functions  */

static void     InputDispatch();
static Boolean  VisualChange();
static XmStringCharSet _XmStringCharsetCreate ();
    /* Should be in XmStrings - here on a trial basis. Sankar 1/10/90 */


/*  Help action procedure  */

static void Help();


/*  Label Class functions  */

static void SetOverrideCallback();

/*
 * Procedures for manipulating cached_resources as secondary data.
 */
static Cardinal GetLabelBGClassSecResData (); 
static XtPointer GetLabelClassResBase ( );
extern Cardinal _XmSecondaryResourceData ();


/*  Label gadget resources  */
/*********
 * Uncached resources for Label Gadget
 */

static XtResource resources[] = 
{
    {
        XmNshadowThickness,
        XmCShadowThickness,
        XmRHorizontalDimension,
        sizeof (Dimension),
        XtOffset (XmLabelGadget, gadget.shadow_thickness),
        XmRImmediate, (caddr_t) 0
    },

    {   XmNlabelPixmap,
        XmCLabelPixmap,
        XmRGadgetPixmap,
        sizeof(Pixmap),
        XtOffset(XmLabelGadget,label.pixmap),
        XmRImmediate,
        (caddr_t) XmUNSPECIFIED_PIXMAP
    },

    {
         XmNlabelInsensitivePixmap,
         XmCLabelInsensitivePixmap,
         XmRGadgetPixmap,
         sizeof(Pixmap),
         XtOffset(XmLabelGadget,label.pixmap_insen),
         XmRImmediate,
         (caddr_t) XmUNSPECIFIED_PIXMAP
    },


    {   XmNlabelString,
        XmCXmString,
        XmRXmString,
        sizeof(_XmString),
        XtOffset(XmLabelGadget, label._label),
        XmRImmediate,
        (caddr_t) NULL
    },

    {   XmNfontList,
        XmCFontList,
        XmRFontList,
        sizeof(XmFontList),
        XtOffset(XmLabelGadget, label.font),
        XmRImmediate,
        (caddr_t) NULL
    },

    {   XmNmnemonic,
        XmCMnemonic,
        XmRKeySym,
        sizeof(KeySym),
        XtOffset(XmLabelGadget,label.mnemonic),
        XmRImmediate,
        (caddr_t) NULL
    },

    {   XmNmnemonicCharSet,
        XmCMnemonicCharSet,
        XmRString,
        sizeof(XmStringCharSet),
        XtOffset(XmLabelGadget,label.mnemonicCharset),
        XmRImmediate,
        (caddr_t)  XmSTRING_DEFAULT_CHARSET
    },

    {   XmNaccelerator,
        XmCAccelerator,
        XmRString,
        sizeof(char *),
        XtOffset(XmLabelGadget,label.accelerator),
        XmRImmediate,
        (caddr_t) NULL
    },

    {   XmNacceleratorText,
        XmCAcceleratorText,
        XmRXmString,
        sizeof(_XmString),
        XtOffset(XmLabelGadget,label._acc_text),
        XmRImmediate,
        (caddr_t) NULL 
    },
    {
        XmNtraversalOn, 
        XmCTraversalOn, 
        XmRBoolean, 
        sizeof (Boolean),
        XtOffset (XmGadget, gadget.traversal_on),
        XmRImmediate, (caddr_t) False
    },
   {
        XmNhighlightThickness, 
        XmCHighlightThickness, 
        XmRHorizontalDimension,
        sizeof (Dimension), 
        XtOffset (XmGadget, gadget.highlight_thickness),
        XmRImmediate, (caddr_t) 0
   },

};
/*********
 * Cached resources for Label Gadget
 */
static XtResource cache_resources[] = 
{
    {
        XmNlabelType,
        XmCLabelType,
        XmRLabelType,
        sizeof(unsigned char),
	XtOffset(XmLabelGCacheObject, label_cache.label_type), XmRImmediate,
        (caddr_t) XmSTRING
    },

    {   XmNalignment,
        XmCAlignment,
        XmRAlignment,
        sizeof(unsigned char),
	XtOffset(XmLabelGCacheObject, label_cache.alignment), XmRImmediate, 
	(caddr_t) XmALIGNMENT_CENTER
    },

    {	XmNmarginWidth, 
	XmCMarginWidth, 
	XmRHorizontalDimension,
	sizeof (Dimension),
	XtOffset (XmLabelGCacheObject, label_cache.margin_width), XmRImmediate,
	(caddr_t) 2
    },

    {	XmNmarginHeight, 
	XmCMarginHeight, 
	XmRVerticalDimension, 
	sizeof (Dimension),
	XtOffset (XmLabelGCacheObject, label_cache.margin_height), XmRImmediate,
	(caddr_t) 2
    },

    {	XmNmarginLeft, 
	XmCMarginLeft, 
	XmRHorizontalDimension, 
	sizeof (Dimension),
	XtOffset (XmLabelGCacheObject, label_cache.margin_left), XmRImmediate,
	(caddr_t) 0
    },

    {	XmNmarginRight, 
	XmCMarginRight, 
	XmRHorizontalDimension, 
	sizeof (Dimension),
	XtOffset (XmLabelGCacheObject, label_cache.margin_right), XmRImmediate,
	(caddr_t) 0
    },

    {	XmNmarginTop, 
	XmCMarginTop, 
	XmRVerticalDimension, 
	sizeof (Dimension),
	XtOffset (XmLabelGCacheObject, label_cache.margin_top), XmRImmediate,
	(caddr_t) 0
    },

    {	XmNmarginBottom, 
	XmCMarginBottom, 
	XmRVerticalDimension, 
	sizeof (Dimension),
	XtOffset (XmLabelGCacheObject, label_cache.margin_bottom), XmRImmediate,
	(caddr_t) 0
    },

    {   XmNrecomputeSize,
        XmCRecomputeSize,
        XmRBoolean,
        sizeof(Boolean),
        XtOffset(XmLabelGCacheObject, label_cache.recompute_size), XmRImmediate,
        (caddr_t) True
    },

    {
        XmNstringDirection,
        XmCStringDirection,
        XmRStringDirection,
        sizeof(unsigned char),
        XtOffset(XmLabelGCacheObject, label_cache.string_direction), XmRImmediate,
        (caddr_t) XmSTRING_DIRECTION_DEFAULT  
   },
};

/*  Definition for resources that need special processing in get values  */

static XmSyntheticResource syn_resources[] =
{
   { XmNlabelString,
     sizeof (_XmString),
     XtOffset (XmLabelGadget, label._label),
     GetLabelString,
     NULL},

   { XmNaccelerator,
     sizeof(String),
     XtOffset(XmLabelGadget,label.accelerator),
     GetAccelerator,
     NULL},

   { XmNacceleratorText,
     sizeof (_XmString),
     XtOffset (XmLabelGadget, label._acc_text),
     GetAcceleratorText,
     NULL},

   { XmNmnemonicCharSet,
     sizeof (XmStringCharSet),
     XtOffset (XmLabelGadget, label.mnemonicCharset),
     GetMnemonicCharset,
     NULL},
};

static XmSyntheticResource cache_syn_resources[] =
{

   { XmNmarginWidth, 
     sizeof (Dimension),
     XtOffset (XmLabelGCacheObject, label_cache.margin_width), 
     _XmFromHorizontalPixels, 
     _XmToHorizontalPixels    
   },

   { XmNmarginHeight, 
     sizeof (Dimension),
     XtOffset (XmLabelGCacheObject, label_cache.margin_height),
     _XmFromVerticalPixels, 
     _XmToVerticalPixels    
   },

   { XmNmarginLeft, 
     sizeof (Dimension),
     XtOffset (XmLabelGCacheObject, label_cache.margin_left), 
     _XmFromHorizontalPixels, 
     _XmToHorizontalPixels    
   },

   { XmNmarginRight, 
     sizeof (Dimension),
     XtOffset (XmLabelGCacheObject, label_cache.margin_right), 
     _XmFromHorizontalPixels, 
     _XmToHorizontalPixels    
   },

   { XmNmarginTop, 
     sizeof (Dimension),
     XtOffset (XmLabelGCacheObject, label_cache.margin_top), 
     _XmFromVerticalPixels, 
     _XmToVerticalPixels    
   },

   { XmNmarginBottom, 
     sizeof (Dimension),
     XtOffset (XmLabelGCacheObject, label_cache.margin_bottom),
     _XmFromVerticalPixels, 
     _XmToVerticalPixels    
   },
};


static XmCacheClassPart LabelClassCachePart = {
	{NULL, 0, 0},		 /* head of class cache list */
	_XmCacheCopy,		/* Copy routine		*/
	_XmCacheDelete,		/* Delete routine	*/
	_XmLabelCacheCompare,	/* Comparison routine 	*/
};


static void SecondaryObjectCreate();

static XmBaseClassExtRec       labelBaseClassExtRec = {
    NULL,                                     /* Next extension       */
    NULLQUARK,                                /* record type XmQmotif */
    XmBaseClassExtVersion,                    /* version              */
    sizeof(XmBaseClassExtRec),                /* size                 */
    XmInheritInitializePrehook,               /* initialize prehook   */
    SetValuesPrehook,                         /* set_values prehook   */
    InitializePosthook,                       /* initialize posthook  */
    SetValuesPosthook,                        /* set_values posthook  */
    (WidgetClass)&xmLabelGCacheObjClassRec,   /* secondary class      */
    (XtInitProc)SecondaryObjectCreate,        /* creation proc        */
    (XmGetSecResDataFunc) GetLabelBGClassSecResData,    /* getSecResData */
    {NULL},                                     /* fast subclass        */
    GetValuesPrehook,                         /* get_values prehook   */
    GetValuesPosthook,                        /* get_values posthook  */
    
};

/* ext rec static initialization */
externaldef (xmlabelgcacheobjclassrec)
XmLabelGCacheObjClassRec xmLabelGCacheObjClassRec =
{
  {
      /* superclass         */    (WidgetClass) &xmExtClassRec,
      /* class_name         */    "XmLabelGadget",
      /* widget_size        */    sizeof(XmLabelGCacheObjRec),
      /* class_initialize   */    NULL,
      /* chained class init */    NULL,
      /* class_inited       */    False,
      /* initialize         */    NULL,
      /* initialize hook    */    NULL,
      /* realize            */    NULL,
      /* actions            */    NULL,
      /* num_actions        */    0,
      /* resources          */    cache_resources,
      /* num_resources      */    XtNumber(cache_resources),
      /* xrm_class          */    NULLQUARK,
      /* compress_motion    */    False,
      /* compress_exposure  */    False,
      /* compress enter/exit*/    False,
      /* visible_interest   */    False,
      /* destroy            */    NULL,
      /* resize             */    NULL,
      /* expose             */    NULL,
      /* set_values         */    NULL,
      /* set values hook    */    NULL,
      /* set values almost  */    NULL,
      /* get values hook    */    NULL,
      /* accept_focus       */    NULL,
      /* version            */    XtVersion,
      /* callback offsetlst */    NULL,
      /* default trans      */    NULL,
      /* query geo proc     */    NULL,
      /* display accelerator*/    NULL,
      /* extension record   */    NULL,
   },

   {
      /* synthetic resources */   cache_syn_resources,
      /* num_syn_resources   */   XtNumber(cache_syn_resources),
      /* extension           */   NULL,
   }
};


/* This is the class record that gets set at compile/link time  */
/* this is what is passed to the widget create routine as the   */
/* the class.  All fields must be inited at compile time.       */

externaldef ( xmlabelgadgetclassrec) 
		XmLabelGadgetClassRec xmLabelGadgetClassRec = {
{
    /* superclass	  */	(WidgetClass) &xmGadgetClassRec,
    /* class_name	  */	"XmLabelGadget",
    /* widget_size	  */	sizeof(XmLabelGadgetRec),
    /* class_initialize   */    ClassInitialize,
    /* chained class init */	ClassPartInitialize,
    /* class_inited       */	False,
    /* initialize	  */	Initialize,
    /* initialize hook    */    NULL,
    /* realize		  */	NULL,
    /* actions		  */	NULL,
    /* num_actions	  */	0,
    /* resources	  */	resources,
    /* num_resources	  */	XtNumber(resources),
    /* xrm_class	  */	NULLQUARK,
    /* compress_motion	  */	True,
    /* compress_exposure  */	XtExposeCompressMaximal,
    /* compress enter/exit*/    True,
    /* visible_interest	  */	False,
    /* destroy		  */	Destroy,
    /* resize		  */	SetSize,
    /* expose		  */	Redisplay,
    /* set_values	  */	(XtSetValuesFunc)SetValues,
    /* set values hook    */    NULL,
    /* set values almost  */    (XtAlmostProc) (SetValuesAlmost),
    /* get values hook    */    NULL,
    /* accept_focus	  */	NULL,
    /* version            */    XtVersion,
    /* callback offsetlst */    NULL,
    /* default trans      */    NULL,
    /* query geo proc	  */	QueryGeometry,
    /* display accelerator*/	NULL,
    /* extension record   */    (XtPointer)&labelBaseClassExtRec,	      
  },

  {				        /* XmGadget           */
      _XtInherit,                       /* border_highlight   */
      _XtInherit,                       /* border_unhighlight */
      NULL,                             /* arm_and_activate   */
      InputDispatch,			/* input dispatch     */
      VisualChange,			/* visual_change      */
      syn_resources,   	    		/* syn resources      */
      XtNumber(syn_resources),		/* num syn_resources  */
      &LabelClassCachePart,		/* class cache part   */
      NULL,                             /* extension          */
  },

  {					/* Label */
      SetOverrideCallback,              /* override_callback             */
      NULL,				/* menu procedure interface	 */
      NULL,                             /* extension record         */
  }
};

externaldef(xmlabelgadgetclass) WidgetClass xmLabelGadgetClass =  (WidgetClass) &xmLabelGadgetClassRec;

/*******************************************************************
 *
 *  _XmLabelCacheCompare
 *
 *******************************************************************/
#ifdef _NO_PROTO
int _XmLabelCacheCompare (label_inst, label_cache_inst)
XmLabelGCacheObjPart *label_inst, *label_cache_inst;
#else /* _NO_PROTO */
int _XmLabelCacheCompare (XmLabelGCacheObjPart *label_inst, XmLabelGCacheObjPart *label_cache_inst)
#endif /* _NO_PROTO */
{
   if ((label_inst->label_type == label_cache_inst->label_type) &&
       (label_inst->alignment == label_cache_inst->alignment) &&
       (label_inst->string_direction== label_cache_inst->string_direction) &&
       (label_inst->margin_height== label_cache_inst->margin_height) &&
       (label_inst->margin_width== label_cache_inst->margin_width) &&
       (label_inst->margin_left== label_cache_inst->margin_left) &&
       (label_inst->margin_right== label_cache_inst->margin_right) &&
       (label_inst->margin_top== label_cache_inst->margin_top) &&
       (label_inst->margin_bottom== label_cache_inst->margin_bottom) &&
       (label_inst->recompute_size== label_cache_inst->recompute_size) &&
       (label_inst->skipCallback== label_cache_inst->skipCallback) &&
       (label_inst->menu_type== label_cache_inst->menu_type)) 
       return 1;
   else
       return 0;
 
}
/***********************************************************
*
*  ClassInitialize
*
************************************************************/

static void ClassInitialize()
{
  labelBaseClassExtRec.record_type = XmQmotif;
} 


/************************************************************************
 *
 *  ClassPartInitialize
 *	Processes the class fields which need to be inherited.
 *
 ************************************************************************/

static void ClassPartInitialize (wc)
register XmLabelGadgetClass wc;

{
   if (wc->label_class.setOverrideCallback == XmInheritSetOverrideCallback)
       wc->label_class.setOverrideCallback = SetOverrideCallback;

   if (wc->rect_class.resize == XmInheritResize)
       wc->rect_class.resize = SetSize;

  
   _XmFastSubclassInit (((WidgetClass)wc), XmLABEL_GADGET_BIT);
}

/************************************************************************
*
*  SecondaryObjectCreate
*
************************************************************************/
/* ARGSUSED */
static void SecondaryObjectCreate(req, new, args, num_args)
Widget      req, new;
ArgList     args;
Cardinal    *num_args;
{
  XmBaseClassExt              *cePtr;
  Arg                         myArgs[2];
  ArgList                     mergedArgs;
  XmWidgetExtData             extData;

  XtSetArg(myArgs[0] ,XmNlogicalParent, new);
  XtSetArg(myArgs[1] ,XmNextensionType, XmCACHE_EXTENSION);

    if (*num_args)
      mergedArgs = XtMergeArgLists(args, *num_args, myArgs, XtNumber(myArgs));
    else
      mergedArgs = myArgs;

    cePtr = _XmGetBaseClassExtPtr(XtClass(new), XmQmotif);

    (void) XtCreateWidget(XtName(new),
                         (*cePtr)->secondaryObjectClass,
		         XtParent(new),
		         mergedArgs, *num_args + 2);

    extData = _XmGetWidgetExtData(new, XmCACHE_EXTENSION);

    if (mergedArgs != myArgs)
      XtFree (mergedArgs);

    /*
     * fill out cache pointers
     */
    LabG_Cache(new) = &(((XmLabelGCacheObject)extData->widget)->label_cache);
    LabG_Cache(req) = &(((XmLabelGCacheObject)extData->reqWidget)->label_cache);

}


/************************************************************************
 *
 *  InitializePosthook
 *
 ************************************************************************/
/* ARGSUSED */
static void InitializePosthook(req, new, args, num_args)
Widget      req, new;
ArgList     args;
Cardinal    *num_args;
{
    XmWidgetExtData     ext;
    XmLabelGadget lw = (XmLabelGadget)new;
    XmLabelGadget rw = (XmLabelGadget)req;

    /*
     * - register parts in cache.
     * - update cache pointers
     * - and free req
     */

     LabG_Cache(lw) = (XmLabelGCacheObjPart *)
	   _XmCachePart( LabG_ClassCachePart(lw),
		         (caddr_t)LabG_Cache(lw),
                         sizeof(XmLabelGCacheObjPart));

    /*
     * might want to break up into per-class work that gets explicitly
     * chained. For right now, each class has to replicate all
     * superclass logic in hook routine
     */

    /*
     * free the req subobject used for comparisons
     */
     ext = _XmGetWidgetExtData((Widget)lw, XmCACHE_EXTENSION);
     XtFree(ext->reqWidget);
     XtDestroyWidget(ext->widget);
     /* extData gets freed at destroy */
}


/************************************************************************
 *
 *  SetValuesPrehook
 *
 ************************************************************************/
/* ARGSUSED */
static Boolean SetValuesPrehook(oldParent,refParent,newParent, args, num_args)
Widget      oldParent, refParent, newParent;
ArgList     args;
Cardinal    *num_args;
{
    XmLabelGadget  lg = (XmLabelGadget)newParent;
    XmLabelGadget  rg = (XmLabelGadget)refParent;

    XmWidgetExtData             extData;
    XmBaseClassExt              *cePtr;
    WidgetClass                 ec;
    XmLabelGCacheObject         new;

    cePtr = _XmGetBaseClassExtPtr(XtClass(newParent), XmQmotif);
    ec = (*cePtr)->secondaryObjectClass;

    /* allocate copies and fill from cache */
    new = (XmLabelGCacheObject) XtMalloc(ec->core_class.widget_size);

    new->object.self = (Widget)new;
    new->object.widget_class = ec;
    new->object.parent = XtParent(newParent);
    new->object.xrm_name = newParent->core.xrm_name;
    new->object.being_destroyed = False;
    new->object.destroy_callbacks = NULL;
    new->object.constraints = NULL;

    new->ext.logicalParent = newParent;
    new->ext.extensionType = XmCACHE_EXTENSION;

    bcopy((char *)LabG_Cache(newParent),
	  (char *)&(new->label_cache),
          sizeof(XmLabelGCacheObjPart));

    extData = (XmWidgetExtData) XtCalloc(sizeof(XmWidgetExtDataRec), 1);
    _XmPushWidgetExtData(newParent, extData, XmCACHE_EXTENSION);

    XtSetValues(new, args, *num_args);

    LabG_Cache(newParent) = &(((XmLabelGCacheObject)new)->label_cache);
    LabG_Cache(refParent) = &(((XmLabelGCacheObject)extData->reqWidget)->label_cache);

    return FALSE;
}



/************************************************************************
 *
 *  GetValuesPrehook
 *
 ************************************************************************/
/* ARGSUSED */
static void GetValuesPrehook(newParent, args, num_args)
Widget      newParent;
ArgList     args;
Cardinal    *num_args;
{
    XmLabelGadget  lg = (XmLabelGadget)newParent;

    XmWidgetExtData             extData;
    XmBaseClassExt              *cePtr;
    WidgetClass                 ec;
    XmLabelGCacheObject         new;

    cePtr = _XmGetBaseClassExtPtr(XtClass(newParent), XmQmotif);
    ec = (*cePtr)->secondaryObjectClass;

    /* allocate copies and fill from cache */
    new = (XmLabelGCacheObject) XtMalloc(ec->core_class.widget_size);

    new->object.self = (Widget)new;
    new->object.widget_class = ec;
    new->object.parent = XtParent(newParent);
    new->object.xrm_name = newParent->core.xrm_name;
    new->object.being_destroyed = False;
    new->object.destroy_callbacks = NULL;
    new->object.constraints = NULL;

    new->ext.logicalParent = newParent;
    new->ext.extensionType = XmCACHE_EXTENSION;

    bcopy((char *)LabG_Cache(newParent),
	  (char *)&(new->label_cache),
          sizeof(XmLabelGCacheObjPart));

    extData = (XmWidgetExtData) XtCalloc(sizeof(XmWidgetExtDataRec), 1);
    _XmPushWidgetExtData(newParent, extData, XmCACHE_EXTENSION);

    XtGetValues(new, args, *num_args);

}

/************************************************************************
 *
 *  GetValuesPosthook
 *
 ************************************************************************/
/* ARGSUSED */
static void GetValuesPosthook(new, args, num_args)
Widget      new;
ArgList     args;
Cardinal    *num_args;
{
 XmLabelGadget  lg = (XmLabelGadget)new;
 XmWidgetExtData             ext;

 _XmPopWidgetExtData(new, &ext, XmCACHE_EXTENSION);

 XtFree(ext->widget);
 XtFree(ext);
}

/************************************************************************
 *
 *  SetValuesPosthook
 *
 ************************************************************************/
/* ARGSUSED */
static Boolean SetValuesPosthook(current, req, new, args, num_args)
Widget      current, req, new;
ArgList     args;
Cardinal    *num_args;
{
    XmLabelGadget lg = (XmLabelGadget)new;
    XmLabelGadget cg = (XmLabelGadget)current;

    XmWidgetExtData             ext;
    XmLabelGCacheObjPart        *oldCachePtr, *newCachePtr;

    /*
     * - register parts in cache.
     * - update cache pointers
     * - and free req
     */


      /* assign if changed! */
      if (!_XmLabelCacheCompare(LabG_Cache(new),
			  LabG_Cache(current)))
			  
      {
         _XmCacheDelete((caddr_t)LabG_Cache(current));  /* delete the old one */
	  LabG_Cache(new) = (XmLabelGCacheObjPart *)
	      _XmCachePart(LabG_ClassCachePart(new),
	                   (caddr_t)LabG_Cache(new),
			   sizeof(XmLabelGCacheObjPart));
      } else
          LabG_Cache(new) = LabG_Cache(current);

      _XmPopWidgetExtData(new, &ext, XmCACHE_EXTENSION);

      XtFree(ext->widget);
      XtFree(ext->reqWidget);
      XtFree(ext->oldWidget);
      XtFree(ext);

      return FALSE;
}






/************************************************************************
 *
 *  SetNormalGC
 *	Create the normal and insensitive GC's for the gadget.
 *
 ************************************************************************/

static void SetNormalGC (lw)
XmLabelGadget lw;

{
   XGCValues       values;
   XtGCMask        valueMask;
   XmManagerWidget mw;
   XFontStruct     *fs;


   mw = (XmManagerWidget) XtParent(lw);

   valueMask = GCForeground | GCBackground | GCFont | GCGraphicsExposures;

    _XmFontListGetDefaultFont(LabG_Font(lw), &fs);
   values.foreground = mw->manager.foreground;
   values.background = mw->core.background_pixel;
   values.graphics_exposures = FALSE;

   if (fs==NULL)
     valueMask &= ~GCFont;
   else
     values.font       = fs->fid;

   LabG_NormalGC(lw) = XtGetGC (mw,valueMask,&values);

   valueMask |= GCFillStyle | GCTile;
   values.fill_style = FillTiled;
   values.tile = XmGetPixmap (XtScreen((Widget)(lw)), "50_foreground",
                                   mw->manager.foreground,
                                   mw->core.background_pixel);
   LabG_InsensitiveGC(lw) = XtGetGC(mw, valueMask, &values);

}




/************************************************************************
 *
 *  SetSize
 *	Sets new width, new height, and new label.TextRect 
 *	appropriately. It is called by Initialize and SetValues.
 *
 ************************************************************************/

static void SetSize(newlw)
XmLabelGadget newlw;

{  int leftx, rightx, dispx;

   /* initialize TextRect width and height to 0, reset if needed */
   LabG_TextRect(newlw).width = 0;
   LabG_TextRect(newlw).height = 0;
   LabG_AccTextRect(newlw).width = 0;
   LabG_AccTextRect(newlw).height = 0;

   if (LabG_IsPixmap (newlw))     /* is a pixmap so find out how */
   {                         /* how big it is */

      if ((newlw->rectangle.sensitive) &&
	  (newlw->rectangle.ancestor_sensitive))
      {
	 if (Pix (newlw) != XmUNSPECIFIED_PIXMAP)
	 {
	    unsigned int junk;
	    unsigned int  w = 0 , h = 0, d;

	    XGetGeometry (XtDisplay(newlw),
			  Pix(newlw),
			  (Window*)&junk,    /* returned root window */   
			  (int*)&junk, (int*)&junk,  /* x, y of pixmap */  
			  &w, &h,              /* width, height of pixmap */
			  &junk,               /* border width */ 
			  &d);              /* depth */

 
	    LabG_TextRect(newlw).width = (unsigned short) w;
	    LabG_TextRect(newlw).height = (unsigned short) h;
 	 }
      }
      else
      {
	 if (Pix_insen(newlw) != XmUNSPECIFIED_PIXMAP)
	 {
	    unsigned int junk;
	    unsigned int  w = 0 , h = 0, d;

	    XGetGeometry (XtDisplay(newlw),
			  Pix_insen(newlw),
			  (Window*)&junk,          /* returned root window */
			  (int*)&junk, (int*)&junk,   /* x, y of pixmap */
			  &w, &h,        /* width, height of pixmap */
			  &junk,          /* border width */
			  &d);          /* depth */
	    
	    LabG_TextRect(newlw).width = (unsigned short) w;
	    LabG_TextRect(newlw).height = (unsigned short) h;
	 }
      } 
      
      if (LabG__acceleratorText(newlw) != NULL)
      {
	 Dimension w,h ;
	 
	 /*
	  * If we have a string then size it.
	  */
	 if (!_XmStringEmpty (LabG__acceleratorText(newlw)))
	 {
	    _XmStringExtent(LabG_Font(newlw), 
			    LabG__acceleratorText(newlw), &w, &h);
	    LabG_AccTextRect(newlw).width = (unsigned short)w;
	    LabG_AccTextRect(newlw).height = (unsigned short)h;
	 }
      }
   } 
   
   else if (LabG_IsText (newlw))
   { 
      Dimension w, h;

      /* If we have a string then size it.  */

      if (!_XmStringEmpty (LabG__label(newlw)))
      {
         _XmStringExtent (LabG_Font(newlw), LabG__label(newlw), &w, &h);
	 LabG_TextRect(newlw).width = (unsigned short)w;
	 LabG_TextRect(newlw).height = (unsigned short)h;
      }

      if (LabG__acceleratorText(newlw) != NULL)
      {
	 /*
	  * If we have a string then size it.
	  */
	 if (!_XmStringEmpty (LabG__acceleratorText(newlw)))
	 {
	    _XmStringExtent(LabG_Font(newlw), LabG__acceleratorText(newlw), 
			    &w, &h);
	    LabG_AccTextRect(newlw).width = w;
	    LabG_AccTextRect(newlw).height = h;
	 }
      }
   }

   /* increase margin width if necessary to accomadate accelerator text */
   if (LabG__acceleratorText(newlw) != NULL) 
       
       if (LabG_MarginRight(newlw) < 
	   LabG_AccTextRect(newlw).width + LABELG_ACC_PAD)
       {
	  LabG_MarginRight(newlw) = 
	      LabG_AccTextRect(newlw).width + LABELG_ACC_PAD;
       }

   /* Has a width been specified?  */

   if (newlw->rectangle.width == 0)
       newlw->rectangle.width =
           LabG_TextRect(newlw).width +
	       LabG_MarginLeft(newlw) + LabG_MarginRight(newlw) +
		   (2 * (LabG_MarginWidth(newlw) +
			 newlw->gadget.highlight_thickness
				+ newlw->gadget.shadow_thickness));

   leftx = newlw->rectangle.x + newlw->gadget.highlight_thickness +
	      newlw->gadget.shadow_thickness + LabG_MarginWidth(newlw) +
	      LabG_MarginLeft(newlw);

   rightx = newlw->rectangle.width - newlw->gadget.highlight_thickness -
	      newlw->gadget.shadow_thickness - LabG_MarginWidth(newlw) -
	      LabG_MarginRight(newlw);
   rightx += newlw->rectangle.x;
		           
                    
   switch (LabG_Alignment(newlw))
   {
    case XmALIGNMENT_BEGINNING:
	  LabG_TextRect(newlw).x = leftx;
      break;

    case XmALIGNMENT_END:
      LabG_TextRect(newlw).x = rightx - LabG_TextRect(newlw).width;
      break;

    default:
	/* XmALIGNMENT_CENTER */
      dispx = ( (rightx -leftx) - LabG_TextRect(newlw).width)/2;
      LabG_TextRect(newlw).x = leftx + dispx;

      break;
   }

   /*  Has a height been specified?  */
   if (newlw->rectangle.height == 0)
       newlw->rectangle.height = Max(LabG_TextRect(newlw).height,
				     LabG_AccTextRect(newlw).height) 
	   + LabG_MarginTop(newlw) 
	       + LabG_MarginBottom(newlw)
		   + (2 * (LabG_MarginHeight(newlw)
			   + newlw->gadget.highlight_thickness
			   + newlw->gadget.shadow_thickness));

   LabG_TextRect(newlw).y = newlw->rectangle.y +
       newlw->gadget.highlight_thickness
	   + newlw->gadget.shadow_thickness
	       + LabG_MarginHeight(newlw) + LabG_MarginTop(newlw) +
		   ((newlw->rectangle.height - LabG_MarginTop(newlw)
		     - LabG_MarginBottom(newlw)
		     - (2 * (LabG_MarginHeight(newlw)
			     + newlw->gadget.highlight_thickness
			     + newlw->gadget.shadow_thickness))
		     - LabG_TextRect(newlw).height) / 2);

   if (LabG__acceleratorText(newlw) != NULL)
   {
      Dimension  base_label, base_accText, diff;

      LabG_AccTextRect(newlw).x =  newlw->rectangle .x + 
	  (newlw->rectangle.width -
	   newlw->gadget.highlight_thickness -
	   newlw->gadget.shadow_thickness -
	   LabG_MarginWidth(newlw) -
	   LabG_MarginRight(newlw) +
	   LABELG_ACC_PAD);

      LabG_AccTextRect(newlw).y = newlw->rectangle.y +
	  newlw->gadget.highlight_thickness
	      + newlw->gadget.shadow_thickness
		  + LabG_MarginHeight(newlw) + LabG_MarginTop(newlw) +
		      ((newlw->rectangle.height - LabG_MarginTop(newlw)
			- LabG_MarginBottom(newlw)
			- (2 * (LabG_MarginHeight(newlw)
				+ newlw->gadget.highlight_thickness
				+ newlw->gadget.shadow_thickness))
			- LabG_AccTextRect(newlw).height) / 2);

      /* make sure the label and accelerator text line up*/
      /* when the fonts are different */

      if (LabG_IsText (newlw))
      { 
	 base_label = 
	     _XmStringBaseline (LabG_Font(newlw), LabG__label(newlw));
	 base_accText = 
	     _XmStringBaseline (LabG_Font(newlw),
				LabG__acceleratorText(newlw));
	 
	 if (base_label > base_accText)
	 {
	    diff = base_label - base_accText;
	    LabG_AccTextRect(newlw).y = LabG_TextRect(newlw).y + diff - 1;
	 }
	 else if (base_label < base_accText)
	 {
	    diff = base_accText - base_label;
	    LabG_TextRect(newlw).y = LabG_AccTextRect(newlw).y + diff - 1;
	 }
      }
   }

   if (newlw->rectangle.width == 0)    /* set core width and height to a */
       newlw->rectangle.width = 1;     /* default value so that it doesn't */
   if (newlw->rectangle.height == 0)   /* a Toolkit Error */
       newlw->rectangle.height = 1;
}




/************************************************************************
 *
 *  Initialize
 *	This is the widget's instance initialize routine.  It is
 *	called once for each widget.
 *  Changes: Treat label, pixmap, labeltype, mnemoniccharset as independedntly
 *	settable resource.
 ************************************************************************/
/* ARGSUSED */
static void Initialize(req, new, args, num_args)
Widget req;				/* as built from arglist */
Widget new;   				/* as modified by superclasses */
ArgList args;
Cardinal *num_args;

{
   unsigned char  stringDirection;
    Arg           myargs[1];
    int n;

    XmLabelGadget lw = (XmLabelGadget) new;
    XmLabelGadget rw = (XmLabelGadget) req;

    /* if menuProcs is not set up yet, try again */
    if (xmLabelGadgetClassRec.label_class.menuProcs == NULL)
	xmLabelGadgetClassRec.label_class.menuProcs =
	    (XmMenuProc) _XmGetMenuProcContext();

    LabG_SkipCallback(new) = NULL;

    /* Check for Invalid enumerated types */

    if ((!LabG_IsPixmap(new)) && (!LabG_IsText(lw)))
    {
      LabG_LabelType(new) = XmSTRING;
      _XmWarning (lw, LABEL_TYPE_MESSAGE);

    }

    if ((LabG_Alignment(new) != XmALIGNMENT_BEGINNING) &&
        (LabG_Alignment(new) != XmALIGNMENT_END) &&
        (LabG_Alignment(new) != XmALIGNMENT_CENTER))
    {
      LabG_Alignment(new) = XmALIGNMENT_CENTER;
      _XmWarning (lw, ALIGNMENT_MESSAGE);
    }
/*
 * Default string behavior : is same as that of parent.
 *  If string direction is not set then borrow it from the parent.
 *  Should we still check for it.
 */

   if (LabG_StringDirection(new) == XmSTRING_DIRECTION_DEFAULT)
    { if ( XmIsManager( XtParent(new)))
	  { n = 0;
        XtSetArg( myargs[n], XmNstringDirection, &stringDirection); n++;
        XtGetValues ( XtParent(lw), myargs, n);
        LabG_StringDirection(new) = stringDirection;
	  }
      else
       LabG_StringDirection(new) = XmSTRING_DIRECTION_L_TO_R;
    }

    if ((LabG_StringDirection(new) != XmSTRING_DIRECTION_L_TO_R) &&
        (LabG_StringDirection(new) != XmSTRING_DIRECTION_R_TO_L))
    {
      LabG_StringDirection(new) = XmSTRING_DIRECTION_L_TO_R;
      _XmWarning (lw, STR_DIR_MESSAGE);
    }


    if (LabG_Font(new) == NULL)
    {
      XFontStruct  *fs;
      XmStringCharSet cset = (XmStringCharSet) XmSTRING_DEFAULT_CHARSET;

      XmFontList defaultFont;

      if (XtClass(lw) == xmLabelGadgetClass)
         defaultFont = _XmGetDefaultFontList(lw, XmLABEL_FONTLIST);
      else
	 defaultFont = _XmGetDefaultFontList(lw, XmBUTTON_FONTLIST);

      lw->label.font = XmFontListCopy (defaultFont);

    }
    /* Make a local copy of the font list */
    else
    {
      LabG_Font(new) = XmFontListCopy( LabG_Font(new));
    }

    if (XmIsRowColumn(XtParent(new)))
    {
       Arg arg[1];
       XtSetArg (arg[0], XmNrowColumnType, &LabG_MenuType(new));
       XtGetValues (XtParent(new), arg, 1);
    }
    else
	LabG_MenuType(new) = XmWORK_AREA;
	

    /*  Handle the label string :
     *   If no label string is given accept widget's name as default.
     *     convert the widgets name to an XmString before storing;
     *   else
     *     save a copy of the given string.
     *     If the given string is not an XmString issue an warning.
     */

   if (LabG__label(new) == NULL)
    {
          XmString string;
          XmStringCharSet cset = (XmStringCharSet) XmSTRING_DEFAULT_CHARSET;

          string   =  XmStringLtoRCreate(XrmQuarkToString(lw->object.xrm_name),
	 				 cset);
          LabG__label(new) =  _XmStringCreate(string);
          XtFree(string);
     }
   else
    { if (_XmStringIsXmString((XmString)LabG__label(new)))
           LabG__label(new) = _XmStringCreate((XmString)LabG__label(new));       
      else
       {  XmString string;
          XmStringCharSet cset = (XmStringCharSet) XmSTRING_DEFAULT_CHARSET;

         _XmWarning (lw, CS_STRING_MESSAGE);		
          string   =  XmStringLtoRCreate(XrmQuarkToString(lw->object.xrm_name),
				cset);
         LabG__label(new) =  _XmStringCreate(string);
         XtFree(string);
       }
    }

     /*
      * Convert the given mnemonicCharset to the internal Xm-form.
      */
     if (LabG_MnemonicCharset(new) != NULL)
     {
         LabG_MnemonicCharset (new) =
            _XmStringCharsetCreate (LabG_MnemonicCharset (new) );
     }
     else
         LabG_MnemonicCharset (new) =
            _XmStringCharsetCreate (XmSTRING_DEFAULT_CHARSET );

    /* Accelerators are currently only supported in menus */

    if ((LabG__acceleratorText(new) != NULL) &&
	((LabG_MenuType(new) == XmMENU_POPUP) ||
	 (LabG_MenuType(new) == XmMENU_PULLDOWN)))
     {
        if (_XmStringIsXmString((XmString)LabG__acceleratorText(new)))
        {
	   /*
	    * Copy the input string into local space, if
	    * not a Cascade Button
	    */
           if ( XmIsCascadeButtonGadget(new))
              LabG__acceleratorText(new) = NULL;
           else
              LabG__acceleratorText(new)= 
		 _XmStringCreate((XmString)LabG__acceleratorText(new));
        }
        else
        {
           _XmWarning(lw, ACC_MESSAGE);
           LabG__acceleratorText(new) = NULL;
        }
    }

    else
        LabG__acceleratorText(new) = NULL;

    if ((LabG_Accelerator(new) != NULL) &&
        ((LabG_MenuType(new) == XmMENU_POPUP) ||
	 (LabG_MenuType(new) == XmMENU_PULLDOWN)))
    {
      char *s;

      /* Copy the accelerator into local space */

      s = XtMalloc (XmStrlen (LabG_Accelerator(new)) + 1);
      strcpy (s, LabG_Accelerator(new));
      LabG_Accelerator(lw) = s;
    }
    else
        LabG_Accelerator(lw) = NULL;

    LabG_SkipCallback(lw) = FALSE;

    /*  If zero width and height was requested by the application,  */
    /*  reset new's width and height to zero to allow SetSize()     */
    /*  to operate properly.                                        */

    if (rw->rectangle.width == 0) 
        lw->rectangle.width = 0;
    
    if (rw->rectangle.height == 0) 
        lw->rectangle.height = 0;
    

    SetSize(lw);
    SetNormalGC(lw);


    /*  Force the label traversal flag when in a menu  */

    if ((XtClass(lw) == xmLabelGadgetClass) &&
       ((LabG_MenuType(new) == XmMENU_POPUP) ||
        (LabG_MenuType(new) == XmMENU_PULLDOWN) ||
        (LabG_MenuType(new) == XmMENU_OPTION)))
    {
       lw->gadget.traversal_on = False;
       lw->gadget.highlight_on_enter = False;
    }

   if ((LabG_MenuType(new) == XmMENU_POPUP) ||
       (LabG_MenuType(new) == XmMENU_PULLDOWN) ||
       (LabG_MenuType(new) == XmMENU_BAR))
       lw->gadget.highlight_thickness = 0;

   /*  Initialize the interesting input types.  */

   lw->gadget.event_mask = XmHELP_EVENT | XmFOCUS_IN_EVENT | XmFOCUS_OUT_EVENT
                           | XmENTER_EVENT | XmLEAVE_EVENT;

}


/************************************************************************
 *
 *  QueryGeometry
 *
 ************************************************************************/

static XtGeometryResult QueryGeometry (lg, intended, reply)
XmLabelGadget lg;
XtWidgetGeometry *intended, *reply;
{
    reply->request_mode = 0;		/* set up fields I care about */

     /* Don't really know what to do with queries about x,y,border,stacking.
     * Since we are interpreting unset bits as a request for information
     * (asking about neither height or width does the old 0-0 request)
     * a caller asking about x,y should not get back width and height,
     * especially since it is an expensive operation.  So x, y, border, stack
     * all return No, this indicates we'd prefer to remain as is.  Parent
     * is free to change it anyway...
     *
     */

    if (GMode (intended) & ( ~ (CWWidth | CWHeight))) 
	  return (XtGeometryNo);

    if (LabG_RecomputeSize(lg) == FALSE)
          return (XtGeometryNo);


     /* pre-load the reply with input values */

     reply->request_mode = (CWWidth | CWHeight);

     reply->width = LabG_TextRect(lg).width +
                    (2 * (LabG_MarginWidth(lg) +
                    lg->gadget.highlight_thickness +
                    lg->gadget.shadow_thickness)) +
                    LabG_MarginLeft(lg) +
                    LabG_MarginRight(lg);

      if (reply->width == 0)
          reply->width = 1;

      reply->height = Max(LabG_TextRect(lg).height,
                          LabG_AccTextRect(lg).height)
                      + (2 * (LabG_MarginHeight(lg) +
                      lg->gadget.highlight_thickness +
                      lg->gadget.shadow_thickness)) +
                      LabG_MarginTop(lg) +
                      LabG_MarginBottom(lg);

      if (reply->height == 0)
          reply->height = 1;

     if ((reply->width != intended->width) ||
         (reply->height != intended->height) ||
         (GMode (intended) !=  GMode (reply)))
           return (XtGeometryAlmost);
     else
     {
         reply->request_mode = 0;
         return (XtGeometryYes);
     }

}


/************************************************************************
 *
 *  Destroy
 *	Free up the label gadget allocated space.  This includes
 *	the label, and GC's.
 *
 ************************************************************************/

static void Destroy(w)
Widget w;
{
    if (LabG__label(w) != NULL) _XmStringFree (LabG__label(w));
    if (LabG__acceleratorText(w) != NULL) _XmStringFree (LabG__acceleratorText(w));
    if (LabG_Accelerator(w) != NULL) XtFree (LabG_Accelerator(w));
    if (LabG_Font(w)  != NULL) XmFontListFree (LabG_Font(w));
    if (LabG_MnemonicCharset(w) != NULL ) XtFree (LabG_MnemonicCharset (w));

    _XmCacheDelete((caddr_t)LabG_Cache(w));

    XtReleaseGC (XtParent(w), LabG_NormalGC(w));
    XtReleaseGC (XtParent(w), LabG_InsensitiveGC(w));

}




/************************************************************************
 *
 *  Redisplay
 *
 ************************************************************************/

static void Redisplay (lw)
XmLabelGadget lw;

{
   GC gc;
   GC clipgc;
   XRectangle clip_rect;
   Dimension availW, availH, needed_width, needed_height;
   Boolean clip_set = False;

   if (LabG_MenuType(lw) == XmMENU_POPUP ||
       LabG_MenuType(lw) == XmMENU_PULLDOWN)
   {
      ShellWidget mshell = (ShellWidget) XtParent(XtParent(lw));
      if (! mshell->shell.popped_up)
	  return;
   }

   availH = lw->rectangle.height;
   availW = lw->rectangle.width;

   needed_width = LabG_TextRect(lw).width + LabG_AccTextRect(lw).width +
                  LabG_MarginLeft(lw) + LabG_MarginRight(lw) +
                  (2 * (lw->gadget.highlight_thickness +
                        lw->gadget.shadow_thickness +
                        LabG_MarginWidth(lw)));

   needed_height = LabG_TextRect(lw).height + LabG_AccTextRect(lw).height +
                  LabG_MarginTop(lw) + LabG_MarginBottom(lw) +
                  (2 * (lw->gadget.highlight_thickness +
                        lw->gadget.shadow_thickness +
                        LabG_MarginHeight(lw)));

   if (availH < needed_height  || availW < needed_width)
   {
      clip_rect.x = lw->rectangle.x + lw->gadget.highlight_thickness +
                    lw->gadget.shadow_thickness;
      clip_rect.y = lw->rectangle.y + lw->gadget.highlight_thickness +
                    lw->gadget.shadow_thickness;
      clip_rect.width = availW - 2 * (lw->gadget.highlight_thickness +
                                      lw->gadget.shadow_thickness);
      clip_rect.height = availH - 2 * (lw->gadget.highlight_thickness +
                                       lw->gadget.shadow_thickness);
      if ((lw->rectangle.sensitive) && (lw->rectangle.ancestor_sensitive))
      {
        XSetClipRectangles (XtDisplay (lw),
                          LabG_NormalGC(lw), 0,0, &clip_rect,1,Unsorted);
        clipgc = LabG_NormalGC(lw);
      }
      else
      {
        XSetClipRectangles (XtDisplay (lw),
                          LabG_InsensitiveGC(lw), 0,0, &clip_rect,1,Unsorted);
        clipgc = LabG_InsensitiveGC(lw);
      }
      clip_set = True;
   }


   /*  Draw the pixmap or text  */

   if (LabG_IsPixmap(lw))
   {
     if ((lw->rectangle.sensitive) && (lw->rectangle.ancestor_sensitive))
     {
       if (Pix (lw) != XmUNSPECIFIED_PIXMAP)
       {
         gc = LabG_NormalGC(lw);
         XCopyArea (XtDisplay(lw), Pix(lw), XtWindow(lw),gc, 0, 0, 
                    LabG_TextRect(lw).width, LabG_TextRect(lw).height, 
                    LabG_TextRect(lw).x, LabG_TextRect(lw).y); 
       }
     }
     else
     {
       if (Pix_insen (lw) != XmUNSPECIFIED_PIXMAP)
       {
         gc = LabG_InsensitiveGC(lw);
         XCopyArea (XtDisplay(lw), Pix_insen(lw), XtWindow(lw),gc, 0, 0,
                   LabG_TextRect(lw).width, LabG_TextRect(lw).height,
                   LabG_TextRect(lw).x, LabG_TextRect(lw).y);

       }

     }

   }

   else if ( (LabG_IsText (lw)) && (LabG__label(lw) != NULL))
   {
      if (LabG_Mnemonic(lw) != NULL)
      { /*
         * A hack to use keysym as the mnemonic.
         */
          char tmp[2];
          tmp[0] = (( LabG_Mnemonic(lw)) & ( (long) (0xFF)));
          tmp[1] = '\0';

        _XmStringDrawMnemonic(XtDisplay(lw), XtWindow(lw),
                                 LabG_Font(lw), LabG__label(lw),
                                 (((lw->rectangle.sensitive) &&
                                   (lw->rectangle.ancestor_sensitive)) ?
                                 LabG_NormalGC(lw) :
                                 LabG_InsensitiveGC(lw)),
                                 LabG_TextRect(lw).x, LabG_TextRect(lw).y,
                                 LabG_TextRect(lw).width, LabG_Alignment(lw),
                                 XmSTRING_DIRECTION_L_TO_R, NULL,
                                 tmp,   /* LabG__mnemonic(lw), */ 
				 LabG_MnemonicCharset(lw));
		 }

     else
      _XmStringDraw (XtDisplay(lw), XtWindow(lw), 
                     LabG_Font(lw), LabG__label(lw),
                     (((lw->rectangle.sensitive) &&
                       (lw->rectangle.ancestor_sensitive)) ?
                     LabG_NormalGC(lw) :
                     LabG_InsensitiveGC(lw)),
                     LabG_TextRect(lw).x,
                     LabG_TextRect(lw).y,
                     LabG_TextRect(lw).width,
                     LabG_Alignment(lw), XmSTRING_DIRECTION_L_TO_R, NULL);
   }

   if (LabG__acceleratorText(lw) != NULL)
   {
        /* since accelerator text  is drawn by moving in from the right,
         it is possible to overwrite label text when there is clipping,
         Therefore draw accelerator text only if there is enough
         room for everything */

        if ((lw->rectangle.width) >= (2 * (lw->gadget.highlight_thickness +
                                           lw->gadget.shadow_thickness +
                                           LabG_MarginWidth(lw)) +
                                      LabG_MarginLeft(lw) + LabG_TextRect(lw).width +
                                      LabG_MarginRight(lw)))
           _XmStringDraw (XtDisplay(lw), XtWindow(lw),
                         LabG_Font(lw), LabG__acceleratorText(lw),
                         (((lw->rectangle.sensitive) &&
                         (lw->rectangle.ancestor_sensitive)) ?
                         LabG_NormalGC(lw) :
                         LabG_InsensitiveGC(lw)),
                         LabG_AccTextRect(lw).x,
                         LabG_AccTextRect(lw).y,
                         LabG_AccTextRect(lw).width, XmALIGNMENT_END,
                         XmSTRING_DIRECTION_L_TO_R, NULL);
   }


   /*  If set, reset the clipping rectangle to none  */

   if (clip_set)
      XSetClipMask (XtDisplay (lw), clipgc, None);


   /* Redraw the proper highlight  */

   if (lw->gadget.highlighted)
      _XmHighlightBorder((Widget)lw);
   else if (_XmDifferentBackground ((Widget)lw, XtParent (lw)))
      _XmUnhighlightBorder((Widget)lw);
}


/************************************************************************
 *
 *  SetValues
 *	This routine will take care of any changes that have been made
 *
 ************************************************************************/


static Boolean SetValues (current, req, new)
XmLabelGadget current,req,new;

{
   Boolean flag = False;
   Boolean newstring = False;
   Boolean ProcessFlag = FALSE;
#ifdef PinheadCC
   Boolean interum = False;
#endif /* PinheadCC */

   /*  If the label has changed, make a copy of the new label,  */
   /*  and free the old label.                                  */ 

   if (LabG__label(new)!= LabG__label(current))
   { newstring = True;
     if (LabG__label(new) == NULL)
      {
          XmString string;
          XmStringCharSet cset = (XmStringCharSet) XmSTRING_DEFAULT_CHARSET;

          string   =  XmStringLtoRCreate(XrmQuarkToString(current->
						object.xrm_name), cset);
          LabG__label(new) =  _XmStringCreate(string);
          XtFree(string);
      }
     else
      { if (_XmStringIsXmString((XmString)LabG__label(new)))
           LabG__label(new) = _XmStringCreate((XmString)LabG__label(new));
        else
        {  XmString string;
           XmStringCharSet cset = (XmStringCharSet) XmSTRING_DEFAULT_CHARSET;

           _XmWarning (new, CS_STRING_MESSAGE);
            string = XmStringLtoRCreate(XrmQuarkToString(new->object.xrm_name),
                                        cset);
           LabG__label(new) =  _XmStringCreate(string);
           XtFree(string);
        }
     }

      _XmStringFree(LabG__label(current));
      LabG__label(current)= NULL;
      LabG__label(req)= NULL;
   }


   if ((LabG__acceleratorText(new)!= LabG__acceleratorText(current)) &&
	((LabG_MenuType(new) == XmMENU_POPUP) ||
	 (LabG_MenuType(new) == XmMENU_PULLDOWN)))
   {
      if (LabG__acceleratorText(new) != NULL)
      {
	 if (_XmStringIsXmString((XmString)LabG__acceleratorText(new)))
	 {
	    newstring = TRUE;
	    if ((XmIsCascadeButtonGadget(new)) &&
		(LabG__acceleratorText(new) != NULL))
		LabG__acceleratorText(new) = NULL;
	    else
		LabG__acceleratorText(new) = _XmStringCreate((XmString)LabG__acceleratorText(new));
	    _XmStringFree(LabG__acceleratorText(current));
	    LabG__acceleratorText(current)= NULL;
	    LabG__acceleratorText(req)= NULL;
	 }
	 else
	 {
	    _XmWarning (new, ACC_MESSAGE);
	    LabG__acceleratorText(new) = NULL;
	 }
      }
   }
   else
       LabG__acceleratorText(new) = LabG__acceleratorText(current);

   if (LabG_Font(new) != LabG_Font(current))
   {
      if (LabG_Font(new) == NULL)
      {
        XFontStruct *fs;
        XmStringCharSet cset = (XmStringCharSet) XmSTRING_DEFAULT_CHARSET;

        fs = XLoadQueryFont(XtDisplay(new), "fixed");
        LabG_Font(new) = XmFontListCreate (fs, cset);
      }
      else
        LabG_Font(new) = XmFontListCopy (LabG_Font(new));

      if (LabG_IsText(new))
      {
        _XmStringUpdate (LabG_Font(new), LabG__label(new));
      }
   }


   /*  ReInitialize the interesting input types.  */

   new->gadget.event_mask = XmHELP_EVENT;

   new->gadget.event_mask |= 
        XmFOCUS_IN_EVENT | XmFOCUS_OUT_EVENT | XmENTER_EVENT | XmLEAVE_EVENT;

   if ((LabG_MenuType(new) == XmMENU_POPUP) ||
       (LabG_MenuType(new) == XmMENU_PULLDOWN))
       new->gadget.highlight_thickness = 0;

   if ((!LabG_IsPixmap(new)) && (!LabG_IsText(new)))
   {
       LabG_LabelType(new) = LabG_LabelType(current);
       _XmWarning (new, LABEL_TYPE_MESSAGE);
   }

    /* ValidateInputs(new); */

#ifdef PinheadCC
   interum = (
#else
   if (
#endif /* PinheadCC */
       (LabG_IsText(new) &&
         ((newstring) ||
         (LabG_Font(new) != LabG_Font(current)))) ||
       (LabG_IsPixmap(new) &&
         ((LabG_Pixmap(new) != LabG_Pixmap(current)) ||
          (LabG_PixmapInsensitive(new) != LabG_PixmapInsensitive(current)))) ||
       (LabG_LabelType(new) != LabG_LabelType(current)) ||
       (LabG_Alignment(new)!= LabG_Alignment(current)) ||
       (LabG_StringDirection(new) != LabG_StringDirection(current))
#ifdef PinheadCC
       );
   interum = interum || (
#else
       ||
#endif /* PinheadCC */
       (LabG_MarginHeight(new) != LabG_MarginHeight(current)) ||
       (LabG_MarginWidth(new) != LabG_MarginWidth(current)) ||
       (LabG_MarginRight(new) != LabG_MarginRight(current)) ||
       (LabG_MarginLeft(new)!= LabG_MarginLeft(current)) ||
       (LabG_MarginTop(new)!= LabG_MarginTop(current)) ||
       (LabG_MarginBottom(new)!= LabG_MarginBottom(current))
#ifdef PinheadCC
       );
   if (interum ||
#else
       ||
#endif /* PinheadCC */
       (new->gadget.shadow_thickness !=
        current->gadget.shadow_thickness) ||
       (new->gadget.highlight_thickness !=
        current->gadget.highlight_thickness) ||

       ((new->rectangle.width <= 0) || (new->rectangle.height <= 0)) ||
       (req->rectangle.x != current->rectangle.x) ||
       (req->rectangle.y != current->rectangle.y) ||
       (req->rectangle.width != current->rectangle.width) ||
       (req->rectangle.height != current->rectangle.height) )
	  
   {

      if ((LabG_Alignment(new) != XmALIGNMENT_BEGINNING) &&
         (LabG_Alignment(new) != XmALIGNMENT_END) &&
         (LabG_Alignment(new) != XmALIGNMENT_CENTER))
      {
          LabG_Alignment(new) = LabG_Alignment(current);
          _XmWarning (new, ALIGNMENT_MESSAGE);
      }

      if ((LabG_StringDirection(new) != XmSTRING_DIRECTION_L_TO_R) &&
          (LabG_StringDirection(new) != XmSTRING_DIRECTION_R_TO_L))
      {
          LabG_StringDirection(new) = LabG_StringDirection(current);
          _XmWarning (new, STR_DIR_MESSAGE);
      }
   
	 if (LabG_RecomputeSize(new))   
      {
         if (req->rectangle.width == current->rectangle.width)
            new->rectangle.width = 0;
         if (req->rectangle.height == current->rectangle.height)
            new->rectangle.height = 0;
      }
/** I wonder why not the following  - Sankar:
    if (LabG_RecomputeSize(new))
     {
         if (new->rectangle.width != current->rectangle.width)
            new->rectangle.width = 0;
         if (new->rectangle.height != current->rectangle.height)
            new->rectangle.height = 0;
      }
***/

      /*  Call SetSize to get the new size.  */

      SetSize(new);

      flag = True;
   }

/*
 * If the sensitivity has changed then we must redisplay.
 */
	if( (new->rectangle.sensitive != current->rectangle.sensitive) ||
        (new->rectangle.ancestor_sensitive !=
        current->rectangle.ancestor_sensitive) )
      {
	    flag = True;
	  }


   /*  Force the traversal flag when in a menu.  */

   if ((XtClass(new) == xmLabelGadgetClass) &&
       ((LabG_MenuType(new) == XmMENU_POPUP) ||
	(LabG_MenuType(new) == XmMENU_PULLDOWN) ||
	(LabG_MenuType(new) == XmMENU_OPTION)))
   {
      new->gadget.traversal_on = False;
      new->gadget.highlight_on_enter = False;
   }


   /*  Recreate the GC's if the font has been changed  */

   if (LabG_Font(new) != LabG_Font(current))
   {
      XtReleaseGC (XtParent (current), LabG_NormalGC(current));
      XtReleaseGC (XtParent (current), LabG_InsensitiveGC(current));
      SetNormalGC(new);
      flag = True;
   }

   if ((LabG_MenuType(new) != XmWORK_AREA) &&
       (LabG_Mnemonic(new) != LabG_Mnemonic(current)))
   {
      /* New grabs only required if mnemonic changes */
      ProcessFlag = TRUE;
      if (LabG_LabelType(new) == XmSTRING)
	 flag = TRUE;
   }

   if (LabG_MnemonicCharset(new) != LabG_MnemonicCharset(current)) 
   {
      if (LabG_MnemonicCharset(new))
        LabG_MnemonicCharset(new) = 
	    _XmStringCharsetCreate(LabG_MnemonicCharset (new));
      else
        LabG_MnemonicCharset(new) = 
	    _XmStringCharsetCreate(XmSTRING_DEFAULT_CHARSET);

      if ( LabG_MnemonicCharset (current) != NULL)
	 XtFree ( LabG_MnemonicCharset (current));

      if (LabG_LabelType(new) == XmSTRING)
	 flag = TRUE;
   }

   if (((LabG_MenuType(new) == XmMENU_POPUP) ||
	(LabG_MenuType(new) == XmMENU_PULLDOWN)) &&
       (LabG_Accelerator(new) != LabG_Accelerator(current)))
    {
      if (LabG_Accelerator(new) != NULL)
      {
         char *s;

         /* Copy the accelerator into local space */

         s = XtMalloc (XmStrlen (LabG_Accelerator(new)) + 1);
         strcpy (s, LabG_Accelerator(new));
         LabG_Accelerator(new) = s;
      }

      if (LabG_Accelerator(current) != NULL)
        XtFree(LabG_Accelerator(current));

      LabG_Accelerator(current) = NULL;
      LabG_Accelerator(req) = NULL;
      ProcessFlag = TRUE;
    }
    else
      LabG_Accelerator(new) = LabG_Accelerator(current);

    if (ProcessFlag)
	(* xmLabelGadgetClassRec.label_class.menuProcs)
	    (XmMENU_PROCESS_TREE, (Widget)new, NULL, NULL, NULL);

   return (flag);
}


/************************************************************************
 *
 *  VisualChange
 *	This function is called from XmManagerClass set values when
 *	the managers visuals have changed.  The gadget regenerates any
 *	GC based on the visual changes and returns True indicating a
 *	redraw is needed.  Otherwize, False is returned.
 *
 ************************************************************************/

static Boolean VisualChange (gw, curmw, newmw)
XmGadget gw;
XmManagerWidget curmw;
XmManagerWidget newmw;

{
   if (curmw->manager.foreground != newmw->manager.foreground ||
       curmw->core.background_pixel != newmw->core.background_pixel)
   {
      XtReleaseGC (XtParent (gw), LabG_NormalGC(gw));
      XtReleaseGC (XtParent (gw), LabG_InsensitiveGC(gw));
      SetNormalGC(gw);
      return (True);
   }

   return (False);
}




/************************************************************************
 *
 *  InputDispatch
 *     This function catches input sent by a manager and dispatches it
 *     to the individual routines.
 *
 ************************************************************************/

static void InputDispatch (lg, event, event_mask)
XmLabelGadget lg;
XEvent * event;
Mask event_mask;

{
   if (event_mask & XmHELP_EVENT) Help (lg, event);
   else if (event_mask & XmENTER_EVENT) _XmEnterGadget ((XmGadget)lg, event);
   else if (event_mask & XmLEAVE_EVENT) _XmLeaveGadget ((XmGadget)lg, event);
   else if (event_mask & XmFOCUS_IN_EVENT) _XmFocusInGadget ((XmGadget)lg, event);
   else if (event_mask & XmFOCUS_OUT_EVENT) _XmFocusOutGadget ((XmGadget)lg, event);
}




/************************************************************************
 *
 *  Help
 *	This routine is called if the user made a help selection 
 *      on the widget.
 *
 ************************************************************************/

static void Help (w, event)
Widget w;
XEvent *event;

{
    XmLabelGadget lg = (XmLabelGadget) w;
    XmAnyCallbackStruct temp;
    Widget parent = XtParent(lg);

    if (LabG_MenuType(lg) == XmMENU_POPUP ||
	LabG_MenuType(lg) == XmMENU_PULLDOWN)
    {
      (* xmLabelGadgetClassRec.label_class.menuProcs)
	  (XmMENU_POPDOWN, XtParent(lg), NULL, (caddr_t)event, NULL);
    }
    
    temp.reason = XmCR_HELP;
    temp.event  = event;

    _XmSocorro(w, event);
}


/************************************************************************
 *
 *  GetLabelString
 *     This is a get values hook function that returns the external
 *     form of the label string from the internal form.
 *
 ***********************************************************************/
/* ARGSUSED */
static void GetLabelString (lw, offset, value)
XmLabelGadget 	lw;
int      	offset;
XtArgVal      	*value;

{
  XmString string;

  string = _XmStringCreateExternal (LabG_Font(lw), LabG__label(lw));

  *value = (XtArgVal) string;

}

/************************************************************************
 *
 *  GetAccelerator
 *     This is a get values hook function that returns a copy
 *     of the accelerator string.
 *
 ***********************************************************************/
/* ARGSUSED */
static void GetAccelerator(lw, resource, value)
XmLabelGadget lw;
XrmQuark      resource;
XtArgVal      *value;

{
  String string;

  string = XtNewString(LabG_Accelerator(lw));

  *value = (XtArgVal) string;

}

/************************************************************************
 *
 *  GetAcceleratorText
 *     This is a get values hook function that returns the external
 *     form of the accelerator text from the internal form.
 *
 ***********************************************************************/
/* ARGSUSED */
static void GetAcceleratorText(lw, offset, value)
XmLabelGadget 	lw;
int		offset;
XtArgVal      	*value;

{
  XmString string;

  string = _XmStringCreateExternal (LabG_Font(lw), LabG__acceleratorText(lw));

  *value = (XtArgVal) string;

}

/************************************************************************
 *
 *  _XmStringCharsetCreate
 *
 ************************************************************************/
static XmStringCharSet _XmStringCharsetCreate (stringcharset)
XmStringCharSet stringcharset;
{
    char    *cset;
    char    *string;
    int size;

    string = (char *) (stringcharset);
    size = strlen ( string);
    cset = XtMalloc (size +1);
    if (cset != NULL)
    strcpy( cset, string);
    return ( (XmStringCharSet) cset);
}


/************************************************************************
 *
 *  GetMnemonicCharset
 *     This is a get values hook function that returns the external
 *     form of the mnemonicCharset from the internal form.
 *  : Returns a string containg the mnemonicCharset.
 *    Caller must free the string .
 *
 ***********************************************************************/
/* ARGSUSED */
static void GetMnemonicCharset (lw, resource, value)
XmLabelGadget lw;
XrmQuark      resource;
XtArgVal      *value;

{
  char *cset;
  int   size = 0;

  cset = NULL;
  if (LabG_MnemonicCharset (lw))
    { size = strlen (LabG_MnemonicCharset (lw));
      if (size > 0)
     cset = (char *) (_XmStringCharsetCreate(LabG_MnemonicCharset (lw)));
    }

  *value = (XtArgVal) cset;

}


/************************************************************************
 *
 *  Caching Assignment help
 *     These routines are for manager widgets that go into Label's
 *     fields and set them, instead of doing a SetValues.
 *
 ************************************************************************/
static XmLabelGCacheObjPart local_cache; 
static Boolean local_cache_inited = FALSE;

/*
 * QualifyLabelLocalCache
 *  Checks to see if local cache is set up
 */
static Boolean QualifyLabelLocalCache(w) 
XmLabelGadget w;
{
    if (!local_cache_inited) 
    { 
        local_cache_inited = TRUE; 
        ClassCacheCopy(LabG_ClassCachePart(w))((caddr_t)LabG_Cache(w), 
					       (caddr_t)&local_cache, 
					       sizeof(local_cache)); 
    }
}


/************************************************************************
 *
 * _XmReCacheLabG()
 * Check to see if ReCaching is necessary as a result of fields having
 * been set by a mananger widget. This routine is called by the
 * manager widget in their SetValues after a change is made to any
 * of Label's cached fields.
 *
 ************************************************************************/

#ifdef _NO_PROTO
void _XmReCacheLabG(lw)
XmLabelGadget   lw;
#else /* _NO_PROTO */
void _XmReCacheLabG (XmLabelGadget lw)
#endif /* _NO_PROTO */
{
     if (local_cache_inited &&
        (!_XmLabelCacheCompare( &local_cache, LabG_Cache(lw)))) 
     {	
           _XmCacheDelete((caddr_t)LabG_Cache(lw));	/* delete the old one */
	   LabG_Cache(lw) = (XmLabelGCacheObjPart *)_XmCachePart(
	       LabG_ClassCachePart(lw), (caddr_t)&local_cache, sizeof(local_cache));
     } 
     local_cache_inited = FALSE;
}

#ifdef _NO_PROTO
void _XmAssignLabG_MarginHeight(lw, value)
XmLabelGadget   lw;
Dimension       value;
#else /* _NO_PROTO */
void _XmAssignLabG_MarginHeight (XmLabelGadget lw, Dimension value)
#endif /* _NO_PROTO */
{
       QualifyLabelLocalCache(lw);
       local_cache.margin_height = value;
}

#ifdef _NO_PROTO
void _XmAssignLabG_MarginWidth(lw, value)
XmLabelGadget   lw;
Dimension       value;
#else /* _NO_PROTO */
void _XmAssignLabG_MarginWidth (XmLabelGadget lw, Dimension value)
#endif /* _NO_PROTO */
{
       QualifyLabelLocalCache(lw);
       local_cache.margin_width = value;
}

#ifdef _NO_PROTO
void _XmAssignLabG_MarginLeft(lw, value)
XmLabelGadget   lw;
Dimension       value;
#else /* _NO_PROTO */
void _XmAssignLabG_MarginLeft (XmLabelGadget lw, Dimension value)
#endif /* _NO_PROTO */
{
       QualifyLabelLocalCache(lw);
       local_cache.margin_left = value;
}

#ifdef _NO_PROTO
void _XmAssignLabG_MarginRight(lw, value)
XmLabelGadget   lw;
Dimension       value;
#else /* _NO_PROTO */
void _XmAssignLabG_MarginRight (XmLabelGadget lw, Dimension value)
#endif /* _NO_PROTO */
{
       QualifyLabelLocalCache(lw);
       local_cache.margin_right = value;
}

#ifdef _NO_PROTO
void _XmAssignLabG_MarginTop(lw, value)
XmLabelGadget   lw;
Dimension       value;
#else /* _NO_PROTO */
void _XmAssignLabG_MarginTop (XmLabelGadget lw, Dimension value)
#endif /* _NO_PROTO */
{
       QualifyLabelLocalCache(lw);
       local_cache.margin_top = value;
}

#ifdef _NO_PROTO
void _XmAssignLabG_MarginBottom(lw, value)
XmLabelGadget   lw;
Dimension       value;
#else /* _NO_PROTO */
void _XmAssignLabG_MarginBottom (XmLabelGadget lw, Dimension value)
#endif /* _NO_PROTO */
{
       QualifyLabelLocalCache(lw);
       local_cache.margin_bottom = value;
}

/************************************************************************
 *
 *  SetOverrideCallback
 *
 * Used by subclasses.  If this is set true, then there is a RowColumn
 * parent with the entryCallback resource set.  The subclasses do not
 * do their activate callbacks, instead the RowColumn callbacks are called
 * by RowColumn.
 ************************************************************************/
/* ARGSUSED */
static void SetOverrideCallback (w, function)
XmLabelGadget w;
XtProc * function;

{
   QualifyLabelLocalCache(w);
   local_cache.skipCallback= True;
   _XmReCacheLabG(w);
}


/************************************************************************
 *
 *  XmCreateLabelGadget
 *	Externally accessable function for creating a label gadget.
 *
 ************************************************************************/

#ifdef _NO_PROTO
Widget XmCreateLabelGadget (parent, name, arglist, argCount)
Widget  parent;
char  * name;
Arg   * arglist;
Cardinal     argCount;

#else /* _NO_PROTO */
Widget XmCreateLabelGadget (Widget parent, char *name, Arg *arglist, Cardinal argCount)
#endif /* _NO_PROTO */
{
   return (XtCreateWidget(name, xmLabelGadgetClass, parent, arglist, argCount));
}

/*
 *  GetLabelBGClassSecResData ( ) 
 *    Class function to be called to copy secondary resource for external
 *  use.  i.e. copy the cached resources and send it back.
 */
/* ARGSUSED */
static Cardinal GetLabelBGClassSecResData ( class, data_rtn)
WidgetClass class;
XmSecondaryResourceData  **data_rtn;
{   int arrayCount = 0;
    int resNum;
    XmSecondaryResourceData secData, *sd;
    XmBaseClassExt  bcePtr;
	String  resource_class, resource_name;
	XtPointer  client_data;

    bcePtr = &( labelBaseClassExtRec);
    client_data = NULL;
    resource_class = NULL;
    resource_name = NULL;
    arrayCount =
      _XmSecondaryResourceData ( bcePtr, data_rtn, client_data,  
				resource_name, resource_class,
			    (XmResourceBaseProc) (GetLabelClassResBase));

    return (arrayCount);

}

/*
 * GetLabelClassResBase ()
 *   retrun the address of the base of resources.
 *   - Not yet implemented.
 */
static XtPointer
GetLabelClassResBase ( widget, client_data)
Widget  widget;
XtPointer client_data;
{   XtPointer  widgetSecdataPtr;
    int  labg_cache_size = sizeof (XmLabelGCacheObjPart);
	char *cp;

	widgetSecdataPtr = (XtPointer) (XtMalloc ( labg_cache_size +1));

    if (widgetSecdataPtr)
      { cp = (char *) widgetSecdataPtr;
        bcopy ( (char *) ( LabG_Cache(widget)), (char *) cp, labg_cache_size);
	  }

	return (widgetSecdataPtr);

}

static void SetValuesAlmost (old, new, request, reply)
XmLabelGadget old, new;
XtWidgetGeometry  *request, *reply;
{  WidgetClass     wc;

   SetSize (new);
   *request = *reply;

}

