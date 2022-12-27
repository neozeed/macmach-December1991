#ifdef REV_INFO 
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)Label.c	3.19 91/01/10";
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
#include <Xm/LabelP.h>
#include <Xm/Xm.h>


#define Pix(w)			((w)->label.pixmap)
#define Pix_insen(w)		((w)->label.pixmap_insen)

/* Warning Messages */

#define LABEL_TYPE_MESSAGE "Invalid XmNlabeType "

#define ALIGNMENT_MESSAGE "Invalid value in XmNalignment"

#define STR_DIR_MESSAGE "Invalid value in XmNstringDirection"

#define CS_STRING_MESSAGE " Invalid XmNlabelString - must be a compound string "

#define ACC_MESSAGE " Invalid XmNacceleratorText - must be a compound string "


/* Core Class Functions */

static void ClassInitialize();
static void ClassPartInitialize();
static void Initialize();
static void SetSize();
static void Enter();
static void Leave();
static void Destroy();
static Boolean SetValues();
static void Redisplay();
static XtGeometryResult QueryGeometry  ();
static void SetValuesAlmost();

/* Help Action Procedure */

static void Help		();

/* Label Class Functions */

static void SetOverrideCallback();
static XmStringCharSet _XmStringCharSetCreate ();


/* get values hook functions */

static void GetLabelString();
static void GetAccelerator();
static void GetAcceleratorText();
static void GetMnemonicCharSet();


/* default translations and action recs */

static char DefaultTranslation[] =
      "<EnterWindow>:	Enter()\n\
       <LeaveWindow>:	Leave()\n\
       <Key>osfHelp:	Help()";


static XtActionsRec ActionsList[] = {
       {"Enter",	(XtActionProc)Enter},
       {"Leave",	(XtActionProc)Leave},
       {"Help",		(XtActionProc)Help},
};


/* Here are the translations used by the subclasses for menu traversal */
/* The matching actions are defined in RowColumn.c                     */

#ifndef MCCABE
static  char menu_traversal_events[] =
    "<Unmap>:		Unmap()\n\
     <FocusOut>:	FocusOut()\n\
     <FocusIn>:		FocusIn()\n\
     <Key>osfCancel:	MenuEscape()\n\
     <Key>osfLeft:	MenuTraverseLeft()\n\
     <Key>osfRight:	MenuTraverseRight()\n\
     <Key>osfUp:	MenuTraverseUp()\n\
     <Key>osfDown:	MenuTraverseDown()";
#else
static char menu_traversal_events[];
#endif



/* here are the resources that this widget adds */


static XtResource resources[] = 
{
 
    {
        XmNshadowThickness,
        XmCShadowThickness,
        XmRHorizontalDimension,
        sizeof (Dimension),
        XtOffset (XmLabelWidget, primitive.shadow_thickness),
        XmRImmediate,
        (caddr_t) 0
    },

    {
         XmNalignment,
         XmCAlignment,
         XmRAlignment,
         sizeof(unsigned char),
	 XtOffset(XmLabelWidget,label.alignment),
         XmRImmediate, 
	 (caddr_t) XmALIGNMENT_CENTER
    },

    {
         XmNlabelType,
         XmCLabelType,
         XmRLabelType,
         sizeof(unsigned char),
	 XtOffset(XmLabelWidget,label.label_type),
         XmRImmediate,
         (caddr_t) XmSTRING
    },

    {
 	XmNmarginWidth, 
	XmCMarginWidth, 
	XmRHorizontalDimension, 
	sizeof (Dimension),
	XtOffset (XmLabelWidget, label.margin_width), 
	XmRImmediate,
	(caddr_t) 2
    },

    {
	XmNmarginHeight, 
	XmCMarginHeight, 
	XmRVerticalDimension, 
	sizeof (Dimension),
	XtOffset (XmLabelWidget, label.margin_height),
	XmRImmediate,
	(caddr_t) 2
    },

    {
	XmNmarginLeft, 
	XmCMarginLeft, 
	XmRHorizontalDimension, 
	sizeof (Dimension),
	XtOffset (XmLabelWidget, label.margin_left), 
	XmRImmediate,
	(caddr_t) 0
    },

    {
	XmNmarginRight, 
	XmCMarginRight, 
	XmRHorizontalDimension, 
	sizeof (Dimension),
	XtOffset (XmLabelWidget, label.margin_right), 
	XmRImmediate,
	(caddr_t) 0
    },

    {
	XmNmarginTop, 
	XmCMarginTop, 
	XmRVerticalDimension, 
	sizeof (Dimension),
	XtOffset (XmLabelWidget, label.margin_top), 
	XmRImmediate,
	(caddr_t) 0
    },

    {
	XmNmarginBottom, 
	XmCMarginBottom, 
	XmRVerticalDimension, 
	sizeof (Dimension),
	XtOffset (XmLabelWidget, label.margin_bottom), 
	XmRImmediate,
	(caddr_t) 0
    },


    {
         XmNfontList,
         XmCFontList,
         XmRFontList,
         sizeof(XmFontList),
         XtOffset(XmLabelWidget,label.font),
         XmRImmediate,
	 (caddr_t) NULL
    },

    {
         XmNlabelPixmap,
         XmCLabelPixmap,
         XmRPrimForegroundPixmap,
         sizeof(Pixmap),
         XtOffset(XmLabelWidget,label.pixmap),
         XmRImmediate,
         (caddr_t) XmUNSPECIFIED_PIXMAP
    },

    {
         XmNlabelInsensitivePixmap,
         XmCLabelInsensitivePixmap,
         XmRPixmap,
         sizeof(Pixmap),
         XtOffset(XmLabelWidget,label.pixmap_insen),
         XmRImmediate,
   	 (caddr_t) XmUNSPECIFIED_PIXMAP 
    },


    {    
         XmNlabelString,
         XmCXmString,
         XmRXmString,
         sizeof(_XmString),
         XtOffset(XmLabelWidget, label._label),
         XmRImmediate,
         (caddr_t) NULL
    },

    {
         XmNmnemonic,
         XmCMnemonic,
         XmRKeySym,
         sizeof(KeySym),
         XtOffset(XmLabelWidget,label.mnemonic),
         XmRImmediate,
         (caddr_t) NULL
    },

     {
          XmNmnemonicCharSet,
          XmCMnemonicCharSet,
          XmRString,
          sizeof(XmStringCharSet),
          XtOffset(XmLabelWidget,label.mnemonicCharset),
          XmRImmediate,
	  (caddr_t) XmSTRING_DEFAULT_CHARSET    
     },

    {
         XmNaccelerator,
         XmCAccelerator,
         XmRString,
         sizeof(char *),
         XtOffset(XmLabelWidget,label.accelerator),
         XmRImmediate,
         (caddr_t) NULL
    },

    {
         XmNacceleratorText,
         XmCAcceleratorText,
         XmRXmString,
         sizeof(_XmString),
         XtOffset(XmLabelWidget,label._acc_text),
         XmRImmediate,
         (caddr_t) NULL
    },

   { 
        XmNrecomputeSize,
        XmCRecomputeSize,
        XmRBoolean,
        sizeof(Boolean),
        XtOffset(XmLabelWidget,label.recompute_size),
        XmRImmediate,
        (caddr_t) True
   },

   { 
        XmNstringDirection,
        XmCStringDirection,
        XmRStringDirection,
        sizeof(unsigned char),
        XtOffset(XmLabelWidget,label.string_direction),
        XmRImmediate,
        (caddr_t) XmSTRING_DIRECTION_DEFAULT 
   },

   {
        XmNtraversalOn,
        XmCTraversalOn,
        XmRBoolean,
        sizeof (Boolean),
        XtOffset (XmPrimitiveWidget, primitive.traversal_on),
        XmRImmediate, 
        (caddr_t) False
    },

    {
        XmNhighlightThickness,
        XmCHighlightThickness,
        XmRHorizontalDimension,
        sizeof (Dimension),
        XtOffset (XmPrimitiveWidget, primitive.highlight_thickness),
        XmRImmediate, 
        (caddr_t) 0
    },

};



/*  Definition for resources that need special processing in get values  */

static XmSyntheticResource syn_resources[] =
{
   { 
     XmNmarginWidth, 
     sizeof (Dimension),
     XtOffset (XmLabelWidget, label.margin_width), 
     _XmFromHorizontalPixels,
     _XmToHorizontalPixels 
     },

   { 
     XmNmarginHeight, 
     sizeof (Dimension),
     XtOffset (XmLabelWidget, label.margin_height),
     _XmFromVerticalPixels, 
     _XmToVerticalPixels 
     },

   { 
     XmNmarginLeft, 
     sizeof (Dimension),
     XtOffset (XmLabelWidget, label.margin_left), 
     _XmFromHorizontalPixels, 
     _XmToHorizontalPixels 
     },

   { 
     XmNmarginRight, 
     sizeof (Dimension),
     XtOffset (XmLabelWidget, label.margin_right), 
     _XmFromHorizontalPixels,
     _XmToHorizontalPixels
     },

   { 
     XmNmarginTop, 
     sizeof (Dimension),
     XtOffset (XmLabelWidget, label.margin_top), 
     _XmFromVerticalPixels,
     _XmToVerticalPixels
     },

   { 
     XmNmarginBottom, 
     sizeof (Dimension),
     XtOffset (XmLabelWidget, label.margin_bottom),
     _XmFromVerticalPixels,
     _XmToVerticalPixels
     },

   { XmNlabelString, 
     sizeof (_XmString),
     XtOffset (XmLabelWidget, label._label),
     GetLabelString,
     NULL
     },

   { XmNmnemonicCharSet,
     sizeof (XmStringCharSet),
     XtOffset (XmLabelWidget, label.mnemonicCharset),
     GetMnemonicCharSet,
     NULL
     },

   { XmNaccelerator,
     sizeof(String),
     XtOffset(XmLabelWidget,label.accelerator),
     GetAccelerator,
     NULL
     },

   { XmNacceleratorText, 
     sizeof (_XmString),
     XtOffset (XmLabelWidget, label._acc_text),
     GetAcceleratorText,
     NULL
     },
};


/* this is the class record that gets set at compile/link time */
/* this is what is passed to the widgetcreate routine as the   */
/* the class.  All fields must be inited at compile time       */


externaldef ( xmlabelclassrec) XmLabelClassRec xmLabelClassRec = {
  {
    /* superclass	  */	(WidgetClass) &xmPrimitiveClassRec,
    /* class_name	  */	"XmLabel",
    /* widget_size	  */	sizeof(XmLabelRec),
    /* class_initialize   */    ClassInitialize,
    /* chained class init */	ClassPartInitialize,
    /* class_inited       */	FALSE,
    /* initialize	  */	Initialize,
    /* initialize hook    */    NULL,
    /* realize		  */	_XtInherit,
    /* actions		  */	ActionsList,
    /* num_actions	  */	XtNumber(ActionsList),
    /* resources	  */	resources,
    /* num_resources	  */	XtNumber(resources),
    /* xrm_class	  */	NULLQUARK,
    /* compress_motion	  */	TRUE,
    /* compress_exposure  */	XtExposeCompressMaximal,  
    /* compress enter/exit*/    TRUE,
    /* visible_interest	  */	FALSE,
    /* destroy		  */	Destroy,
    /* resize		  */	SetSize,
    /* expose		  */	Redisplay,
    /* set_values	  */	(XtSetValuesFunc)SetValues,
    /* set values hook    */    NULL,
    /* set values almost  */    (XtAlmostProc) (SetValuesAlmost),
									/*  XtInheritSetValuesAlmost,  */
    /* get values hook    */    NULL,
    /* accept_focus	  */	NULL,
    /* version            */    XtVersion,
    /* callback offsetlst */    NULL,
    /* default trans      */    DefaultTranslation,
    /* query geo proc	  */	QueryGeometry,
    /* display accelerator*/	NULL,
    /* reserved           */    NULL,	    /* extension record      */
  },

  {				        /* Xmprimitive        */
      _XtInherit,                       /* border_highlight   */
      _XtInherit,                       /* border_unhighlight */
      XtInheritTranslations,       /* translations       */
      NULL,                             /* arm_and_activate   */
      syn_resources,   	    		/* syn resources      */
      XtNumber(syn_resources),		/* num syn_resources  */
      NULL,                             /* extension          */
  },

  {					   /* XmLabel */
        SetOverrideCallback,               /* override_callback             */
	NULL,				   /* menu procedure interface      */
        NULL,				   /* translations                  */
	NULL,				   /* extension record              */

  }
};

externaldef( xmlabelwidgetclass) WidgetClass xmLabelWidgetClass =  
				(WidgetClass) &xmLabelClassRec;


/*********************************************************************
 *
 * ClassInitialize
 *       This is the class initialization routine.  It is called only
 *       the first time a widget of this class is initialized.
 *
 ********************************************************************/         
/* ARGSUSED */
static void ClassInitialize(w)
    Widget w;
{
   xmLabelClassRec.label_class.translations =
     (String) (XtParseTranslationTable(menu_traversal_events));
}

/*********************************************************************
 *
 *  ClassPartInitialize
 *      Processes the class fields which need to be inherited.
 *
 ************************************************************************/

static void ClassPartInitialize (wc)

  register XmLabelWidgetClass wc;

{
  XmLabelWidgetClass super = (XmLabelWidgetClass)wc->core_class.superclass;

  if (wc->label_class.setOverrideCallback == XmInheritSetOverrideCallback)
      wc->label_class.setOverrideCallback = 
             super->label_class.setOverrideCallback;

  if (wc->core_class.resize == XmInheritResize)
      wc->core_class.resize = super->core_class.resize;

  if (wc->core_class.realize == XmInheritRealize)
      wc->core_class.realize = super->core_class.realize;

  if (wc->label_class.translations == XtInheritTranslations )
      wc->label_class.translations = super->label_class.translations;

  _XmFastSubclassInit (wc, XmLABEL_BIT);

}


/************************************************************************
 *
 *  SetNormalGC
 *      Create the normal and insensitive GC's for the gadget.
 *
 ************************************************************************/

static void SetNormalGC(lw)
        XmLabelWidget      lw;
{
        XGCValues       values;
        XtGCMask        valueMask;
        XFontStruct     *fs;

        valueMask = GCForeground | GCBackground | GCFont | GCGraphicsExposures;

       _XmFontListGetDefaultFont(lw->label.font, &fs);
        values.foreground = lw->primitive.foreground;
        values.background = lw->core.background_pixel;
	values.graphics_exposures = False;

	if (fs==NULL)
	  valueMask &= ~GCFont;
	else
          values.font     = fs->fid;

        lw->label.normal_GC = XtGetGC(lw,valueMask,&values);

        valueMask |= GCFillStyle | GCTile | GCGraphicsExposures;
        values.fill_style = FillTiled;
        values.tile = XmGetPixmap (XtScreen((Widget)(lw)), "50_foreground",
                                   lw->primitive.foreground,
                                   lw->core.background_pixel);
        lw->label.insensitive_GC = XtGetGC(lw, valueMask, &values);

}


 /************************************************************************
 *
 *  SetSize
 *      Sets new width, new height, and new label.TextRect
 *      appropriately. This routine is called by Initialize and
 *      SetValues.
 *
 ************************************************************************/
static void SetSize(newlw)
XmLabelWidget      newlw;
{
   XmLabelPart        *lp;

   lp = &(newlw->label);

   /* initialize TextRect width and height to 0, change later if needed */
   lp->TextRect.width = 0;
   lp->TextRect.height = 0;
   lp->acc_TextRect.width = 0;
   lp->acc_TextRect.height = 0;

   if (Lab_IsPixmap(newlw))  /* is a pixmap so find out how */
   {                         /* how big it is */

      if ((newlw->core.sensitive) && (newlw->core.ancestor_sensitive))
      {
         if (Pix(newlw) != XmUNSPECIFIED_PIXMAP)
         {
	    unsigned int junk;
	    unsigned int  w = 0 , h = 0, d;

	    XGetGeometry (XtDisplay(newlw),
			  Pix(newlw),
			  (Window*)&junk,      /* returned root window */   
			  (int*)&junk, (int*)&junk,   /* x, y of pixmap */  
			  &w, &h,        /* width, height of pixmap */
			  &junk,          /* border width */ 
			  &d);          /* depth */

	    lp->TextRect.width = (unsigned short) w;
	    lp->TextRect.height = (unsigned short) h;
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

	    lp->TextRect.width = (unsigned short) w;
	    lp->TextRect.height = (unsigned short) h;
	 }
      }
      if (lp->_acc_text != NULL)
      {
	 Dimension w,h ;
	 
	 /*
	  * If we have a string then size it.
	  */
	 if (!_XmStringEmpty (lp->_acc_text))
	 {
	    _XmStringExtent(lp->font, lp->_acc_text, &w, &h);
	    lp->acc_TextRect.width = (unsigned short)w;
	    lp->acc_TextRect.height = (unsigned short)h;
	 }
      }
   } 
   else     
       if (Lab_IsText(newlw))
       { 
	  Dimension w, h;
	  
	  if (!_XmStringEmpty (lp->_label)) {
	     
	     /*
	      * If we have a string then size it.
	      */
	     _XmStringExtent(lp->font, lp->_label, &w, &h);
	     lp->TextRect.width = (unsigned short)w;
	     lp->TextRect.height = (unsigned short)h;
	  }

	  if (lp->_acc_text != NULL) {
	     /*
	      * If we have a string then size it.
	      */
	     if (!_XmStringEmpty (lp->_acc_text))
	     {
		_XmStringExtent(lp->font, lp->_acc_text, &w, &h);
		lp->acc_TextRect.width = (unsigned short)w;
		lp->acc_TextRect.height = (unsigned short)h;
	     }
	  }
       }  /* else */

   /* increase margin width if necessary to accomadate accelerator text */
   if (lp->_acc_text != NULL) 
       if (lp->margin_right < lp->acc_TextRect.width + LABEL_ACC_PAD)
       {
	  lp->margin_right = lp->acc_TextRect.width + LABEL_ACC_PAD;
       }

   /* Has a width been specified?  */
   
   if (newlw->core.width == 0)
       newlw->core.width = (Dimension)
	   lp->TextRect.width + 
	       lp->margin_left + lp->margin_right +
		   (2 * (lp->margin_width 
			 + newlw->primitive.highlight_thickness
			 + newlw->primitive.shadow_thickness));
                               
   switch (lp -> alignment)
   {
    case XmALIGNMENT_BEGINNING:
      lp->TextRect.x = (short) lp->margin_width +
	  lp->margin_left +
	      newlw->primitive.highlight_thickness +
		  newlw->primitive.shadow_thickness;

      break;

    case XmALIGNMENT_END:
      lp->TextRect.x = (short) newlw->core.width - 
	  (newlw->primitive.highlight_thickness +
	   newlw->primitive.shadow_thickness +	
	   lp->margin_width + lp->margin_right +
	   lp->TextRect.width);
      break;

    default:
      lp->TextRect.x =  (short) newlw->primitive.highlight_thickness
	  + newlw->primitive.shadow_thickness
	      + lp->margin_width + lp->margin_left +
		  ((newlw->core.width - lp->margin_left
		    - lp->margin_right
		    - (2 * (lp->margin_width
			    + newlw->primitive.highlight_thickness
			    + newlw->primitive.shadow_thickness))
		    - lp->TextRect.width) / 2);

      break;
   }

   /* Has a height been specified? */
   
   if (newlw->core.height == 0)
       newlw->core.height = (Dimension)
	   Max(lp->TextRect.height, lp->acc_TextRect.height) +
	       lp->margin_top +
		   lp->margin_bottom
		       + (2 * (lp->margin_height
			       + newlw->primitive.highlight_thickness
			       + newlw->primitive.shadow_thickness));

   lp->TextRect.y =  (short) newlw->primitive.highlight_thickness
       + newlw->primitive.shadow_thickness
	   + lp->margin_height + lp->margin_top +
	       ((newlw->core.height - lp->margin_top
		 - lp->margin_bottom
		 - (2 * (lp->margin_height
			 + newlw->primitive.highlight_thickness
			 + newlw->primitive.shadow_thickness))
		 - lp->TextRect.height) / 2);

   if (lp->_acc_text != NULL)
   {
      Dimension  base_label, base_accText, diff;

      lp->acc_TextRect.x = (short) newlw->core.width - 
	  newlw->primitive.highlight_thickness -
	      newlw->primitive.shadow_thickness -
		  newlw->label.margin_width -
		      newlw->label.margin_right +
			  LABEL_ACC_PAD;

      lp->acc_TextRect.y =  (short) newlw->primitive.highlight_thickness
	  + newlw->primitive.shadow_thickness
	      + lp->margin_height + lp->margin_top +
		  ((newlw->core.height - lp->margin_top
		    - lp->margin_bottom
		    - (2 * (lp->margin_height
			    + newlw->primitive.highlight_thickness
			    + newlw->primitive.shadow_thickness))
		    - lp->acc_TextRect.height) / 2);

      /* make sure the label and accelerator text line up*/
      /* when the fonts are different */

      if (Lab_IsText(newlw))
      {
	 base_label = _XmStringBaseline (lp->font, lp->_label);
	 base_accText = _XmStringBaseline (lp->font, lp->_acc_text);
	 
	 if (base_label > base_accText)
	 {
	    diff = base_label - base_accText;
	    lp->acc_TextRect.y = (short) lp->TextRect.y + diff - 1;
	 }
	 else if (base_label < base_accText)
	 {
	    diff = base_accText - base_label;
	    lp->TextRect.y = (short) lp->acc_TextRect.y + diff - 1;
	 }
      }
   }

   if (newlw->core.width == 0)    /* set core width and height to a */
       newlw->core.width = 1;       /* default value so that it doesn't */
   if (newlw->core.height == 0)   /* generate a Toolkit Error */
       newlw->core.height = 1;
}

/************************************************************
 *
 * Initialize
 *    This is the widget's instance initialize routine.  It is 
 *    called once for each widget                              
 *
 ************************************************************/

static void Initialize(req, new)
    Widget req,				/* as built from arglist */
	   new;   			/* as modified by superclasses */
{
    XmLabelWidget lw = (XmLabelWidget) new;
    unsigned char  stringDirection;
    Arg           l_args[1];
    int	n;

    /* if menuProcs is not set up yet, try again */
    if (xmLabelClassRec.label_class.menuProcs == NULL)
	xmLabelClassRec.label_class.menuProcs =
	    (XmMenuProc) _XmGetMenuProcContext();

    /* Check for Invalid enumerated types */

    if ((!Lab_IsPixmap(lw)) && (!Lab_IsText(lw)))
    {
      lw->label.label_type = XmSTRING;
      _XmWarning (lw, LABEL_TYPE_MESSAGE);
      
    }

    if ((lw->label.alignment != XmALIGNMENT_BEGINNING) &&
        (lw->label.alignment != XmALIGNMENT_END) &&
        (lw->label.alignment != XmALIGNMENT_CENTER))
    {
      lw->label.alignment = XmALIGNMENT_CENTER;
      _XmWarning (lw, ALIGNMENT_MESSAGE);
    }
    
/*
 * Default string behavior : is same as that of parent.
 *  If string direction is not set then borrow it from the parent.
 *  Should we still check for it.
 */

   if (lw->label.string_direction == (unsigned char) XmSTRING_DIRECTION_DEFAULT)
    { if ( XmIsManager ( XtParent(lw))) 
	  { n = 0;
        XtSetArg( l_args[n], XmNstringDirection, &stringDirection); n++;
        XtGetValues ( XtParent(lw), l_args, n);
        lw->label.string_direction = stringDirection;
	  }
      else
	   lw->label.string_direction = XmSTRING_DIRECTION_L_TO_R;
    }

    if ((lw->label.string_direction != XmSTRING_DIRECTION_L_TO_R) &&
        (lw->label.string_direction != XmSTRING_DIRECTION_R_TO_L))
    {
      lw->label.string_direction = XmSTRING_DIRECTION_L_TO_R;
      _XmWarning (lw, STR_DIR_MESSAGE);
    }

    if (lw->label.font == NULL)
    {
      XFontStruct  *fs;
      XmStringCharSet cset = (XmStringCharSet) XmSTRING_DEFAULT_CHARSET;

      XmFontList defaultFont;

      if (XtClass(lw) == xmLabelWidgetClass)
        defaultFont = _XmGetDefaultFontList(lw, XmLABEL_FONTLIST);
      else
        defaultFont = _XmGetDefaultFontList(lw, XmBUTTON_FONTLIST);

      lw->label.font = XmFontListCopy (defaultFont);

    }
    /* Make a local copy of the font list */
    else
    {
      lw->label.font = XmFontListCopy( lw->label.font);
    }

    /* get menu type and which button */
    if (XmIsRowColumn(XtParent(new)))
    {
       Arg arg[1];
       XtSetArg (arg[0], XmNrowColumnType, &lw->label.menu_type);
       XtGetValues (XtParent(new), arg, 1);
    }
    else
	lw->label.menu_type = XmWORK_AREA;

    /*  Handle the label string :
     *   If no label string is given accept widget's name as default.
     *     convert the widgets name to an XmString before storing;
     *   else
     *     save a copy of the given string.
     *     If the given string is not an XmString issue an warning.
     */
   if ( lw->label._label == NULL)
        {
          XmString string;
          XmStringCharSet cset = (XmStringCharSet) XmSTRING_DEFAULT_CHARSET;

          string   =  XmStringLtoRCreate(lw->core.name, cset);
          lw->label._label =  _XmStringCreate(string);
          XtFree(string);
        }
    else

      {  if (_XmStringIsXmString((XmString)lw->label._label))
           lw->label._label= _XmStringCreate((XmString)lw->label._label);
         else
         {
           XmString string;
           XmStringCharSet cset = (XmStringCharSet) XmSTRING_DEFAULT_CHARSET;

            _XmWarning (lw, CS_STRING_MESSAGE);

           string   =  XmStringLtoRCreate(lw->core.name, cset);
           lw->label._label =  _XmStringCreate(string);
           XtFree(string);

         }
    }

     /*
      * Convert the given mnemonicCharset to the internal Xm-form.
      */
    if  (lw->label.mnemonicCharset != NULL )
             lw->label.mnemonicCharset =
		_XmStringCharSetCreate (lw->label.mnemonicCharset );
    else
             lw->label.mnemonicCharset =
		_XmStringCharSetCreate (XmSTRING_DEFAULT_CHARSET );

    /* Accelerators are currently only supported in menus */

    if ((lw->label._acc_text != NULL) &&
	((lw->label.menu_type == XmMENU_POPUP) ||
	 (lw->label.menu_type == XmMENU_PULLDOWN)))
     {
        if (_XmStringIsXmString((XmString)lw->label._acc_text))
        {
                /*
                 * Copy the input string into local space, if
                 * not a Cascade Button
                 */
           if ( XmIsCascadeButton(lw))
              lw->label._acc_text = NULL;
           else
              lw->label._acc_text= _XmStringCreate((XmString)lw->label._acc_text);
        }
        else
        {
           _XmWarning(lw, ACC_MESSAGE);
           lw->label._acc_text = NULL;
        }
    }
    else
        lw->label._acc_text = NULL;


    if ((lw->label.accelerator != NULL) &&
        ((lw->label.menu_type == XmMENU_POPUP) ||
	 (lw->label.menu_type == XmMENU_PULLDOWN)))
    {
      char *s;

      /* Copy the accelerator into local space */

      s = XtMalloc (XmStrlen (lw->label.accelerator) + 1);
      strcpy (s, lw->label.accelerator);
      lw->label.accelerator = s;
    }
    else
         lw->label.accelerator = NULL;

    lw->label.skipCallback = FALSE;

   /*  If zero width and height was requested by the application,  */
   /*  reset new's width and height to zero to allow SetSize()     */
   /*  to operate properly.                                        */

   if (req->core.width == 0)
      lw->core.width = 0;  

   if (req->core.height == 0)
      lw->core.height = 0;

   SetSize(lw);
   SetNormalGC(lw);

   /*  Force the label traversal flag when in a menu  */

   if ((XtClass(lw) == xmLabelWidgetClass) &&
       ((lw->label.menu_type == XmMENU_POPUP) ||
        (lw->label.menu_type == XmMENU_PULLDOWN) ||
        (lw->label.menu_type == XmMENU_OPTION)))
   {
      lw->primitive.traversal_on = FALSE;
      lw->primitive.highlight_on_enter = FALSE;

      XtOverrideTranslations(lw,
	((XmLabelClassRec *)XtClass(lw))->label_class.translations);
   }

   if ((lw->label.menu_type == XmMENU_POPUP) ||
       (lw->label.menu_type == XmMENU_PULLDOWN) ||
       (lw->label.menu_type == XmMENU_BAR))
       lw->primitive.highlight_thickness = 0;



}

/************************************************************************
 *
 *  QueryGeometry
 *
 ************************************************************************/

static XtGeometryResult QueryGeometry (lw, intended, reply)
XmLabelWidget lw;
XtWidgetGeometry *intended, *reply;
{
    reply->request_mode = 0;			/* set up fields I care about */

     /* Don't really know what to do with queries about x,y,border,stacking.
     * Since we are interpreting unset bits as a request for information
     * (asking about neither height or width does the old 0-0 request)
     * a caller asking about x,y should not get back width and height,
     * especially since it is an expensive operation.  So x, y, border, stack
     * all return No, this indicates we'd prefer to remain as is.  Parent
     * is free to change it anyway...
     */

    if (GMode (intended) & ( ~ (CWWidth | CWHeight)))
	return (XtGeometryNo);

    if (lw->label.recompute_size == FALSE)
        return (XtGeometryNo);


     /* pre-load the reply with input values */
    
     reply->request_mode = (CWWidth | CWHeight);

     reply->width = (Dimension) lw->label.TextRect.width +
                    (2 * (lw->label.margin_width +
                    lw->primitive.highlight_thickness +
                    lw->primitive.shadow_thickness)) +
                    lw->label.margin_left +
                    lw->label.margin_right;

      if (reply->width == 0)
          reply->width = 1;

      reply->height = (Dimension) Max(lw->label.TextRect.height,
                          lw->label.acc_TextRect.height)
                      + (2 * (lw->label.margin_height +
                      lw->primitive.highlight_thickness +
                      lw->primitive.shadow_thickness)) +
                      lw->label.margin_top +
                      lw->label.margin_bottom;

     if (reply->height == 0)
          reply->height = 1;

     if ((reply->width != intended->width) ||
         (reply->height != intended->height) || 
         (GMode (intended) != GMode (reply)))
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
 *      Free up the label gadget allocated space.  This includes
 *      the label, and GC's.
 *
 ************************************************************************/

static void Destroy(w)
Widget w;
{
    XmLabelWidget lw = (XmLabelWidget) w;

    if (lw->label._label != NULL) _XmStringFree (lw->label._label);
    if (lw->label._acc_text != NULL) _XmStringFree (lw->label._acc_text);
    if (lw->label.accelerator != NULL) XtFree (lw->label.accelerator);
    if (lw->label.font  != NULL) XmFontListFree (lw->label.font);
    if (lw->label.mnemonicCharset !=  NULL )
	XtFree (lw->label.mnemonicCharset);
    XtReleaseGC (lw, lw->label.normal_GC);
    XtReleaseGC (lw, lw->label.insensitive_GC);

}




/************************************************************************
 *
 *  Redisplay
 *

 ***********************************************************************/
static void Redisplay(lw)
XmLabelWidget      lw;

{
   GC		    gc;
   XmLabelPart        *lp;
   Dimension availW, availH, needed_width, needed_height;

   lp = &(lw->label);

   if (Lab_IsPixmap(lw))
   {
      if ((lw->core.sensitive) && (lw->core.ancestor_sensitive))
      {
	 if (Pix (lw) != XmUNSPECIFIED_PIXMAP)
	 {
	    gc = lp->normal_GC;
	    XCopyArea (XtDisplay(lw), Pix(lw), XtWindow(lw),gc, 0, 0, 
                    lp->TextRect.width, lp->TextRect.height,
                    lp->TextRect.x, lp->TextRect.y); 
	 }
      }
      else
      {
	 if (Pix_insen (lw) != XmUNSPECIFIED_PIXMAP)
	 {
        gc = lp->insensitive_GC;
	    XCopyArea (XtDisplay(lw), Pix_insen(lw), XtWindow(lw),gc, 0, 0, 
                    lp->TextRect.width, lp->TextRect.height,
                    lp->TextRect.x, lp->TextRect.y); 

	 }
      }
   }

   else if ( (Lab_IsText (lw)) && (lp->_label != NULL))
   {
      if (lp->mnemonic != NULL)
/*
 * When the DEC  routine comes call  the following routine.
 * - Call it now
 */
      { /*
         * A hack to use keysym as the mnemonic.
	 */
	char tmp[2];
	tmp[0] = ((lp->mnemonic) & ( (long) (0xFF)));
	tmp[1] = '\0';

        _XmStringDrawMnemonic(XtDisplay(lw), XtWindow(lw),
                                lp->font, lp->_label,
                                (((lw->core.sensitive) &&
                                  (lw->core.ancestor_sensitive)) ?
                                lp->normal_GC :
                                lp->insensitive_GC),
                                lp->TextRect.x, lp->TextRect.y,
                                lp->TextRect.width, lp->alignment,
                                lp->string_direction, NULL,
                                tmp, /* lp->mnemonic, */ lp->mnemonicCharset);
	}
/*
 * but for now keep the original code.
 *
 *
 *        _XmStringDrawUnderline(XtDisplay(lw), XtWindow(lw),
 *                               lp->font, lp->_label,
 *                               (((lw->core.sensitive) && 
 *                                 (lw->core.ancestor_sensitive)) ?
 *                               lp->normal_GC :
 *                               lp->insensitive_GC),
 *                               lp->TextRect.x, lp->TextRect.y,
 *                               lp->TextRect.width, lp->alignment,
 *                               lp->string_direction, NULL,
 *                               lp->_mnemonic);
 *
 */

      else
          _XmStringDraw (XtDisplay(lw), XtWindow(lw),
                        lp->font, lp->_label,
                        (((lw->core.sensitive) &&
                          (lw->core.ancestor_sensitive)) ?
                        lp->normal_GC :
                        lp->insensitive_GC),
                        lp->TextRect.x, lp->TextRect.y,
                        lp->TextRect.width, lp->alignment,
                        lp->string_direction, NULL);

   }

   if (lp->_acc_text != NULL)
   {
      /* since accelerator text  is drawn by moving in from the right,
	 it is possible to overwrite label text when there is clipping,
	 Therefore draw accelerator text only if there is enough
	 room for everything */

      if ((lw->core.width) >= (2 * (lw->primitive.highlight_thickness +
                                       lw->primitive.shadow_thickness +
                                       lp->margin_width) +
                                       lp->margin_left + lp->TextRect.width +
                                       lp->margin_right))
	  _XmStringDraw (XtDisplay(lw), XtWindow(lw),
                              lp->font, lp->_acc_text,
                              (((lw->core.sensitive) &&
                              (lw->core.ancestor_sensitive)) ?
                              lp->normal_GC :
                              lp->insensitive_GC),
                              lp->acc_TextRect.x, lp->acc_TextRect.y,
                              lp->acc_TextRect.width, XmALIGNMENT_END,
                              lp->string_direction, NULL);
   }

   availH = lw->core.height;
   availW = lw->core.width;

   needed_width = lp->TextRect.width + lp->acc_TextRect.width +
                  lp->margin_left + lp->margin_right +
                  (2 * (lw->primitive.highlight_thickness +
                        lw->primitive.shadow_thickness +
                        lp->margin_width));

   needed_height = lp->TextRect.height + lp->acc_TextRect.height +
                  lp->margin_top + lp->margin_bottom +
                  (2 * (lw->primitive.highlight_thickness +
                        lw->primitive.shadow_thickness +
                        lp->margin_height));

   if ((availH < needed_height  || availW < needed_width) &&
      (lw->primitive.highlight_thickness > 0 ||
       lw->primitive.shadow_thickness > 0))
   {
      _XmEraseShadow (XtDisplay(lw), XtWindow(lw),
                      lw->primitive.highlight_thickness +
                      lw->primitive.shadow_thickness,
                      0, 0,
                      lw->core.width,
                      lw->core.height);
   }

    /* Redraw the proper highlight  */


    if (lw->primitive.highlighted)
         _XmHighlightBorder((Widget)lw);
    else if (_XmDifferentBackground ((Widget)lw, XtParent (lw)))
         _XmUnhighlightBorder((Widget)lw);


}


/**********************************************************************
 *
 * Enter
 *
 *********************************************************************/
static void Enter(w, event)
XmLabelWidget w;
XEvent *event;

{
  if (w->primitive.highlight_on_enter)
   _XmPrimitiveEnter ((XmPrimitiveWidget)w, event);
}


/**********************************************************************
 *
 * Leave
 *
 *********************************************************************/
static void Leave(w, event)
XmLabelWidget w;
XEvent *event;

{
  if (w->primitive.highlight_on_enter)
   _XmPrimitiveLeave ((XmPrimitiveWidget)w, event);
}


/************************************************************************
 *
 *  SetValues
 *      This routine will take care of any changes that have been made
 *
 ************************************************************************/

static Boolean SetValues (current, req, new)
XmLabelWidget current,req,new;
{
    Boolean flag = FALSE;
    Boolean newstring = FALSE;
    XmLabelPart        *newlp, *curlp, *reqlp;
    Boolean ProcessFlag = FALSE;
	Boolean SizeChanged = False;

	int newCoreWidth, newCoreHeight, delta;
    char  tmpchar[2];
    /* Get pointers to the label parts  */

    newlp = &(new->label);
    curlp = &(current->label);
    reqlp = &(req->label);
    

    /*  If the label has changed, make a copy of the new label,  */
    /*  and free the old label.                                  */

    if (newlp->_label!= curlp->_label)
    {   
        newstring = TRUE;
        if (newlp->_label == NULL)
	{ XmString string;
          XmStringCharSet cset = (XmStringCharSet) XmSTRING_DEFAULT_CHARSET;
   
          string   =  XmStringLtoRCreate(new->core.name, cset);
          newlp->_label =  _XmStringCreate(string);
          XtFree(string);
        }
       else
	{ if (_XmStringIsXmString((XmString)newlp->_label))
              newlp->_label = _XmStringCreate((XmString)newlp->_label);     
	  else
          {
            XmString string;
            XmStringCharSet cset = (XmStringCharSet) XmSTRING_DEFAULT_CHARSET;

            _XmWarning (new, CS_STRING_MESSAGE);

            string   =  XmStringLtoRCreate(new->core.name, cset);
            newlp->_label =  _XmStringCreate(string);
            XtFree(string);

          }
        }

       _XmStringFree(curlp->_label);
        curlp->_label= NULL;
        reqlp->_label= NULL;
     }


    if ((newlp->_acc_text!= curlp->_acc_text) &&
	((newlp->menu_type == XmMENU_POPUP) ||
	 (newlp->menu_type == XmMENU_PULLDOWN)))
    {
      if (newlp->_acc_text != NULL)
      {
        if (_XmStringIsXmString((XmString)newlp->_acc_text))
        {
          newstring = TRUE;
	  if ((XmIsCascadeButton (new)) &&
	      (newlp->_acc_text != NULL))
	      newlp->_acc_text = NULL;

	  else
            newlp->_acc_text = _XmStringCreate((XmString)newlp->_acc_text);
          _XmStringFree(curlp->_acc_text);
          curlp->_acc_text= NULL;
          reqlp->_acc_text= NULL;
        }
        else
        {
           _XmWarning (new, ACC_MESSAGE);
            newlp->_acc_text = NULL;
        }
      }
    }
    else
        newlp->_acc_text = curlp->_acc_text;


    if (newlp->font != curlp->font)
    {
      if (newlp->font == NULL)
      {
        XFontStruct *fs;
        XmStringCharSet cset = (XmStringCharSet) XmSTRING_DEFAULT_CHARSET;

        fs = XLoadQueryFont(XtDisplay(new), "fixed");
        newlp->font = XmFontListCreate (fs, cset);
      }
      else
        newlp->font = XmFontListCopy (newlp->font);

     if (Lab_IsText(new))
     {
       _XmStringUpdate (newlp->font, newlp->_label);
     }
    }

   if ((new->label.menu_type == XmMENU_POPUP) ||
       (new->label.menu_type == XmMENU_PULLDOWN) ||
       (new->label.menu_type == XmMENU_BAR))
       new->primitive.highlight_thickness = 0;


    if ((!Lab_IsPixmap(new)) && (!Lab_IsText(new)))
    {
      new->label.label_type = current->label.label_type;
      _XmWarning (new, LABEL_TYPE_MESSAGE);
    }

    /* ValidateInputs(new); */

    if ((Lab_IsText(new) && 
	  ((newstring) ||
           (newlp->font != curlp->font))) ||
	(Lab_IsPixmap(new) &&
          ((newlp->pixmap != curlp->pixmap) ||
           (newlp->pixmap_insen  != curlp->pixmap_insen))) ||
        (newlp->label_type != curlp->label_type) ||
        (new->primitive.shadow_thickness !=
         current->primitive.shadow_thickness) ||
        (newlp->margin_height != curlp->margin_height) ||
        (newlp->margin_width != curlp->margin_width) ||
        (newlp->margin_left != curlp->margin_left) ||
        (newlp->margin_right != curlp->margin_right) ||
        (newlp->margin_top != curlp->margin_top) ||
        (newlp->margin_bottom != curlp->margin_bottom)) 
     {
	    flag = TRUE;
        if (newlp->recompute_size)
        {
          if (req->core.width == current->core.width)
              new->core.width = 0;
          if (req->core.height == current->core.height)
              new->core.height = 0;
          SetSize(new);
	      SizeChanged = True;
        }
     }


     if((newlp->alignment != curlp->alignment) ||
        (newlp->string_direction != curlp->string_direction) ||
        (new->primitive.shadow_thickness !=
         current->primitive.shadow_thickness) ||
        (new->primitive.highlight_thickness !=
         current->primitive.highlight_thickness) ||
        ((new->core.width <= 0) || (new->core.height <= 0)) ||
        (req->core.width != current->core.width) ||
        (req->core.height != current->core.height))
    {

        if ((new->label.alignment != XmALIGNMENT_BEGINNING) &&
        (new->label.alignment != XmALIGNMENT_END) &&
        (new->label.alignment != XmALIGNMENT_CENTER))
        {
          new->label.alignment = current->label.alignment;
          _XmWarning (new, ALIGNMENT_MESSAGE);
        }

        if ((new->label.string_direction != XmSTRING_DIRECTION_L_TO_R) &&
            (new->label.string_direction != XmSTRING_DIRECTION_R_TO_L))
        {
          new->label.string_direction = current->label.string_direction;
          _XmWarning (new, STR_DIR_MESSAGE);
        }

       /* Call SetSize to get the new size. */
	   /* SetSize will recompute width and height if it finds core.width
	    * and /or core.height to be 0.
	    */
         if (!(SizeChanged)) SetSize(new);
        flag = TRUE;
    }


/*
 * If sensitivity of the label has changed then we must redisplay
 *  the label.
 */
	if ((new->core.sensitive != current->core.sensitive) ||
        (new->core.ancestor_sensitive != current->core.ancestor_sensitive)) 
	  {
	    flag = TRUE;
	  }

    if ((new->primitive.foreground !=
        current->primitive.foreground) ||
        (new->core.background_pixel !=
        current->core.background_pixel) ||
        (newlp->font != curlp->font))
    {
        XtReleaseGC(current, current->label.normal_GC);
        XtReleaseGC(current, current->label.insensitive_GC);
        SetNormalGC(new);
        flag = TRUE;
    }

    /*  Force the traversal flag when in a menu.  */

    if ((XtClass(new) == xmLabelWidgetClass) &&
	((new->label.menu_type == XmMENU_POPUP) ||
	 (new->label.menu_type == XmMENU_PULLDOWN) ||
	 (new->label.menu_type == XmMENU_OPTION)))
    {
       new->primitive.traversal_on = FALSE;
       new->primitive.highlight_on_enter = FALSE;
    }


    if ((new->label.menu_type != XmWORK_AREA) &&
        (new->label.mnemonic != current->label.mnemonic))
    {
       /* New grabs only required if mnemonic changes */
       ProcessFlag = TRUE;
       if (new->label.label_type == XmSTRING)
	   flag = TRUE;
    }

    if (new->label.mnemonicCharset != current->label.mnemonicCharset)
    {
       if (new->label.mnemonicCharset)
           new->label.mnemonicCharset =
    	      _XmStringCharSetCreate(new->label.mnemonicCharset);
       else
           new->label.mnemonicCharset =
    	      _XmStringCharSetCreate(XmSTRING_DEFAULT_CHARSET);

       if (current->label.mnemonicCharset != NULL)
	  XtFree (current->label.mnemonicCharset);

       if (new->label.label_type == XmSTRING)
	  flag = TRUE;
    }

    if (((new->label.menu_type == XmMENU_POPUP) ||
	 (new->label.menu_type == XmMENU_PULLDOWN)) &&
        (new->label.accelerator != current->label.accelerator))
    {
      if (newlp->accelerator != NULL)
      {
         char *s;

         /* Copy the accelerator into local space */

         s = XtMalloc (XmStrlen (newlp->accelerator) + 1);
         strcpy (s, newlp->accelerator);
         newlp->accelerator = s;
      }
      if (curlp->accelerator != NULL)
        XtFree(curlp->accelerator);
      curlp->accelerator = NULL;
      reqlp->accelerator = NULL;
      ProcessFlag = TRUE;
    }
    else
      newlp->accelerator = curlp->accelerator;
  
    if (ProcessFlag)
	(* xmLabelClassRec.label_class.menuProcs) (XmMENU_PROCESS_TREE, (Widget)new,
						   NULL, NULL, NULL);

    return(flag);
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

static void SetOverrideCallback (w)
XmLabelWidget w;
{
   w->label.skipCallback = True;
}


/************************************************************************
 *
 *  Help
 *      This routine is called if the user made a help selection
 *      on the widget.
 *
 ************************************************************************/

static void Help (w, event)
Widget w;
XEvent *event;

{
    XmLabelWidget lw = (XmLabelWidget) w;
    XmAnyCallbackStruct temp;
    Widget parent = XtParent(lw);

    if (lw->label.menu_type == XmMENU_POPUP ||
	lw->label.menu_type == XmMENU_PULLDOWN)
	(* xmLabelClassRec.label_class.menuProcs)
	    (XmMENU_POPDOWN, parent, NULL, (caddr_t)event, NULL);

	_XmPrimitiveHelp((XmPrimitiveWidget)w, event);
}

/************************************************************************
 *
 *  GetLabelString
 *     This is a get values hook function that returns the external
 *     form of the label string from the internal form.
 *
 ***********************************************************************/
/* ARGSUSED */
static void GetLabelString (lw, resource, value)
XmLabelWidget lw;
XrmQuark      resource;
XtArgVal      *value;

{
  XmString string;
 
  string = _XmStringCreateExternal (lw->label.font, lw->label._label);

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
XmLabelWidget lw;
XrmQuark      resource;
XtArgVal      *value;

{
  String string;

  string = XtNewString( Lab_Accelerator(lw));

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
static void GetAcceleratorText(lw, resource, value)
XmLabelWidget lw;
XrmQuark      resource;
XtArgVal      *value;

{
  XmString string;

  string = _XmStringCreateExternal (lw->label.font, lw->label._acc_text);

  *value = (XtArgVal) string;

}
/************************************************************************
 *
 *  XmCreateLabelWidget
 *      Externally accessable function for creating a label gadget.
 *
 ************************************************************************/


#ifdef _NO_PROTO
Widget XmCreateLabel (parent,name,arglist,argCount)
       Widget parent;
       char   *name;
       Arg    *arglist;
       Cardinal    argCount;
#else /* _NO_PROTO */
Widget XmCreateLabel (Widget parent, char *name, Arg *arglist, Cardinal argCount)
#endif /* _NO_PROTO */
{
    return (XtCreateWidget(name,xmLabelWidgetClass,parent,arglist,argCount));
}


static  XmStringCharSet
_XmStringCharSetCreate ( stringcharset)
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
 *  GetMnemonicCharSet
 *     This is a get values hook function that returns the external
 *     form of the mnemonicCharSet from the internal form.
 *  : Returns a string containg the mnemonicCharSet.
 *    Caller must free the string .
 ***********************************************************************/
/* ARGSUSED */
static void GetMnemonicCharSet (lw, resource, value)
XmLabelWidget lw;
XrmQuark      resource;
XtArgVal      *value;

{
  char *cset;
  int   size = 0;

  cset = NULL;
  if (lw->label.mnemonicCharset)
    { size = strlen (lw->label.mnemonicCharset);
      if (size > 0)
     cset = (char *) (_XmStringCharSetCreate(lw->label.mnemonicCharset));
    }

  *value = (XtArgVal) cset;

}

static void SetValuesAlmost (old, new, request, reply)
XmLabelWidget old, new;
XtWidgetGeometry  *request, *reply;
{  WidgetClass     wc;
	 XmLabelPart        *newlp, *oldlp;


		SetSize ( new);;
		*request = *reply;

}

