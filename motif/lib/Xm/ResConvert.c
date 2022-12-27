#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)ResConvert.c	3.26 91/01/10";
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

#include <stdio.h>
#include <ctype.h>
#include <Xm/XmP.h>
#include <Xm/ExtObjectP.h>

extern XtAppContext _XtDefaultAppContext();

static void _XmCvtStringToArrowDirection();
static void _XmCvtStringToPacking();
static void _XmCvtStringToOrientation();
static void _XmCvtStringToAttachment();
static void _XmCvtStringToAlignment();
static void _XmCvtStringToLabelType();
static void _XmCvtStringToIndicatorType();
static void _XmCvtStringToEditMode();
static void _XmCvtStringToChar();
static void _XmCvtStringToScrollPolicy();
static void _XmCvtStringToVisualPolicy();
static void _XmCvtStringToSBDisplayPolicy();
static void _XmCvtStringToSBPlacement();
static void _XmCvtStringToSelectionPolicy();
static void _XmCvtStringToListSizePolicy();
static void _XmCvtStringToCmdWindowLoc();
static void _XmCvtStringToProcessingDirection();
static void _XmCvtStringToStringDirection();
static void _XmCvtStringToSeparatorType();
static void _XmCvtStringToShadowType();
static void _XmCvtStringToXmString ();
static Boolean _XmCvtStringToXmStringTable ();
static void _XmCvtStringToXmFontList ();
static void _XmCvtStringToKeySym ();
static void _XmCvtStringToWhichButton ();
static void _XmCvtStringToDefaultButtonType ();
static void _XmCvtStringToDialogType ();
static void _XmCvtStringToDialogStyle ();
static void _XmCvtStringToResizePolicy ();
static void _XmCvtStringToRowColumnType ();
static void _XmCvtStringToHorizontalPosition ();
static void _XmCvtStringToVerticalPosition ();
static void _XmCvtStringToHorizontalDimension ();
static void _XmCvtStringToVerticalDimension ();
static void _XmCvtStringToCharSetTable();
static void _XmCvtStringToKeySymTable();
static void _XmConvertStringToButtonType();
static void _XmCvtStringToFileTypeMask() ;
static void _XmCvtStringToBooleanDimension() ;
static void _XmXmStringCvtDestroy();

static void FetchUnitType(widget, size, value)
    Widget widget;
    Cardinal *size;
    XrmValue* value;
{
    XmExtObject	extObject;

    if (widget == NULL) {
	XtErrorMsg("missingWidget", "fetchUnitType", "XtToolkitError",
                   "FetchUnitType called without a widget to reference",
                   (String*)NULL, (Cardinal*)NULL);
    }

    if (XmIsExtObject(widget))
      {
	  widget = ((XmExtObject)widget)->ext.logicalParent;
      }

    if (XmIsGadget(widget))
      {
	  XmGadget	gadget = (XmGadget) widget;

	  value->addr = (caddr_t)&(gadget->gadget.unit_type);
      }

    else if (XmIsManager(widget))
      {
	  XmManagerWidget	manager = (XmManagerWidget) widget;

	  value->addr = (caddr_t)&(manager->manager.unit_type);
      }
    else if (XmIsPrimitive(widget))
      {
	  XmPrimitiveWidget	primitive = (XmPrimitiveWidget) widget;

	  value->addr = (caddr_t)&(primitive->primitive.unit_type);
      }
    else 
      XtWarning("FetchUnitType: bad widget class");

    value->size = sizeof(unsigned char);
}

static XtConvertArgRec resIndConvertArgs[] = {
    { XtProcedureArg, 
      (XtPointer)FetchUnitType, 
      0
    },
#ifdef notdef
    { XtResourceQuark,
        NULL, /* filled in later */
        sizeof(unsigned char)
    },
#endif /* notdef */
    { XtWidgetBaseOffset,
        (caddr_t) XtOffset(Widget, core.screen),
        sizeof (Screen*)
    }
};

static XtConvertArgRec XmStringArgs[] =
{
   { XtWidgetBaseOffset, (caddr_t) XtOffset(Widget, core.screen), sizeof (Screen*) }
};


/* Motif widget set version number.  Accessable by application - externed   */
/* in Xm.h.  Set to the value returned by XmVersion when RegisterConverters */
/* is called.                                                               */

int xmUseVersion;



/************************************************************************
 *
 *  XmRegisterConverters
 *	Register all of the Xm resource type converters.  Retain a
 *	flag indicating whether the converters have already been
 *	registered.
 *
 ************************************************************************/

void XmRegisterConverters ()
{
   static Boolean registered = False;
   static XrmQuark unitQ;

   if (!registered)
   {
      xmUseVersion = XmVersion;

#ifdef notdef
      unitQ = XrmStringToQuark(XmNunitType);

      /*
       * Because the quark value can't be loaded statically,
       * we have to compute it and load it here -- just before
       * installing the converters that use it.
       */

      resIndConvertArgs[0].address_id = (caddr_t) unitQ;
#endif /* notdef */ 

      XtAddConverter (XmRString, XmRArrowDirection,
                      _XmCvtStringToArrowDirection, NULL, 0);
      XtAddConverter (XmRString, XmROrientation,
                      _XmCvtStringToOrientation, NULL, 0);
      XtAddConverter (XmRString, XmRAttachment,
                      _XmCvtStringToAttachment, NULL, 0);
      XtAddConverter (XmRString, XmRPacking,
                      _XmCvtStringToPacking, NULL, 0);
      XtAddConverter (XmRString, XmRAlignment,
                      _XmCvtStringToAlignment, NULL, 0);
      XtAddConverter (XmRString, XmRLabelType,
                      _XmCvtStringToLabelType, NULL, 0);
      XtAddConverter (XmRString, XmRIndicatorType,
                      _XmCvtStringToIndicatorType, NULL, 0);
      XtAddConverter (XmRString, XmREditMode,
                      _XmCvtStringToEditMode, NULL, 0);
      XtAddConverter (XmRString, XmRScrollingPolicy,
      		       _XmCvtStringToScrollPolicy, NULL, 0);
      XtAddConverter (XmRString, XmRVisualPolicy,
      		       _XmCvtStringToVisualPolicy, NULL, 0);
      XtAddConverter (XmRString, XmRScrollBarDisplayPolicy,
      		       _XmCvtStringToSBDisplayPolicy, NULL, 0);
      XtAddConverter (XmRString, XmRScrollBarPlacement,
      		       _XmCvtStringToSBPlacement, NULL, 0);
      XtAddConverter (XmRString, XmRChar, 
                       _XmCvtStringToChar, NULL, 0);
      XtAddConverter (XmRString, XmRSelectionPolicy, 
		       _XmCvtStringToSelectionPolicy, NULL, 0);
      XtAddConverter (XmRString, XmRListSizePolicy,
      		       _XmCvtStringToListSizePolicy, NULL, 0);
      XtAddConverter (XmRString, XmRProcessingDirection,
      		       _XmCvtStringToProcessingDirection, NULL, 0);
      XtAddConverter (XmRString, XmRStringDirection,
      		       _XmCvtStringToStringDirection, NULL, 0);		       
      XtAddConverter (XmRString, XmRSeparatorType,
                      _XmCvtStringToSeparatorType, NULL, 0);
      XtAddConverter (XmRString, XmRShadowType,
                      _XmCvtStringToShadowType, NULL, 0);
      XtAddConverter (XmRString, XmRWhichButton,
                      _XmCvtStringToWhichButton, NULL, 0);
      XtAddConverter (XmRString, XmRFontList,
                      _XmCvtStringToXmFontList, XmStringArgs, 1);
      XtAddConverter (XmRString, XmRXmString,
                      _XmCvtStringToXmString, NULL, 0);
      XtAddConverter (XmRString, XmRDefaultButtonType,
                      _XmCvtStringToDefaultButtonType, NULL, 0);
      XtAddConverter (XmRString, XmRDialogType,
                      _XmCvtStringToDialogType, NULL, 0);
      XtAddConverter (XmRString, XmRDialogStyle,
                      _XmCvtStringToDialogStyle, NULL, 0);
      XtAddConverter (XmRString, XmRResizePolicy,
                      _XmCvtStringToResizePolicy, NULL, 0);
      XtAddConverter (XmRString, XmRCommandWindowLocation,
                      _XmCvtStringToCmdWindowLoc, NULL, 0);
      XtAddConverter (XmRString, XmRRowColumnType,
                      _XmCvtStringToRowColumnType, NULL, 0);
      XtAddConverter (XmRString, XmRKeySym, _XmCvtStringToKeySym, NULL, 0);
      XtAddConverter (XmRString, XmRHorizontalPosition,
                      _XmCvtStringToHorizontalPosition,
                      resIndConvertArgs,
                      XtNumber(resIndConvertArgs));
      XtAddConverter (XmRString, XmRHorizontalDimension,
                      _XmCvtStringToHorizontalDimension,
                      resIndConvertArgs,
                      XtNumber(resIndConvertArgs));
      XtAddConverter (XmRString, XmRVerticalPosition,
                      _XmCvtStringToVerticalPosition,
                      resIndConvertArgs,
                      XtNumber(resIndConvertArgs));
      XtAddConverter (XmRString, XmRVerticalDimension,
                      _XmCvtStringToVerticalDimension,
                      resIndConvertArgs,
                      XtNumber(resIndConvertArgs));
      XtAddConverter (XmRCompoundText, XmRXmString,
                      XmCvtTextToXmString,
                      resIndConvertArgs,
                      XtNumber(resIndConvertArgs));
      XtAddConverter (XmRXmString, XmRCompoundText,
                      XmCvtXmStringToText,
                      resIndConvertArgs,
                      XtNumber(resIndConvertArgs));

      XtAddConverter (XmRString, XmRCharSetTable,
                      _XmCvtStringToCharSetTable, NULL, 0);
      XtAddConverter (XmRString, XmRKeySymTable,
          			_XmCvtStringToKeySymTable, NULL, 0);
      XtAddConverter (XmRString, XmRButtonType,
          			_XmConvertStringToButtonType, NULL, 0);
      XtAddConverter (XmRString, XmRFileTypeMask,
                                          _XmCvtStringToFileTypeMask, NULL, 0);
      XtAddConverter (XmRString, XmRBooleanDimension,
                             _XmCvtStringToBooleanDimension, 
                             resIndConvertArgs,
                             XtNumber(resIndConvertArgs));

      XtSetTypeConverter (XmRString, XmRXmStringTable,
                      _XmCvtStringToXmStringTable, NULL, 0,
                      (XtCacheNone | XtCacheRefCount), 
                      _XmXmStringCvtDestroy);


   }

   registered = True;
}




/************************************************************************
 *
 *  _XmWarning
 *	Build up a warning message and call XtWarning to get it displayed.
 *
 ************************************************************************/

void _XmWarning (w, message)
Widget w;
char * message;

{
   char buf[1024];
   register int pos;
   char * newline_pos;


   pos = 0;

   if (w != NULL)
   {
      strcpy (&buf[pos], "\n    Name: ");
      pos += 11;
      strcpy (&buf[pos], XrmQuarkToString (w->core.xrm_name));
      pos += strlen (XrmQuarkToString (w->core.xrm_name));
      strcpy (&buf[pos], "\n    Class: ");
      pos += 12;
      strcpy (&buf[pos], w->core.widget_class->core_class.class_name);
      pos += strlen (w->core.widget_class->core_class.class_name);
   }

   strcpy (&buf[pos], "\n");
   pos++;

   do
   {
      strcpy (&buf[pos], "    ");
      pos += 4;

      newline_pos = (char*)strchr (message, '\n');

      if (newline_pos == NULL)
      {
         strcpy (&buf[pos], message);
         pos += strlen (message);
         break;
      }
      else
      {
         strncpy (&buf[pos], message, (int) (newline_pos - message + 1));
         pos +=  (int) (newline_pos - message + 1);
         message +=  (int) (newline_pos - message + 1);
      }
   }
   while (newline_pos != NULL);

   strcpy (&buf[pos], "\n");

   XtWarning (buf);
}



/************************************************************************
 *
 *  StringsAreEqual
 *	Compare two strings and return true if equal.
 *	The comparison is on lower cased strings.  It is the callers
 *	responsibility to ensure that test_str is already lower cased.
 *
 ************************************************************************/

static Boolean StringsAreEqual (in_str, test_str)
register char * in_str;
register char * test_str;

{
   register int i;
   register int j;
   i = *in_str;
   if (((in_str[0] == 'X') || (in_str[0] == 'x')) &&
       ((in_str[1] == 'M') || (in_str[1] == 'm')))
        in_str +=2;

   for (;;)
   {
      i = *in_str;
      j = *test_str;

      if (isupper (i)) i = tolower (i);
      if (i != j) return (False);
      if (i == 0) return (True);

      in_str++;
      test_str++;
   }
}
/************************************************************************
 *                                                                      *
 * _XmStringsAreEqual - entry point for people wanting to compare       *
 * strings.                                                             *
 *                                                                      *
 ************************************************************************/
#ifdef _NO_PROTO
Boolean _XmStringsAreEqual (in_str, test_str)
char * in_str;
char * test_str;
#else  /* _NO_PROTO */
Boolean _XmStringsAreEqual (char *in_str, char *test_str)
#endif /* _NO_PROTO */
{
    return (StringsAreEqual (in_str, test_str));
}

/************************************************************************
 *
 *  _XmCvtStringToArrowDirection
 *	Convert a string to an arrow direction.
 *
 ************************************************************************/

/* ARGSUSED */
static void _XmCvtStringToArrowDirection (args, num_args, from_val, to_val)
XrmValue * args;
Cardinal   num_args;
XrmValue * from_val;
XrmValue * to_val;

{
   char * in_str = (char *) (from_val->addr);
   static unsigned char i;

   to_val->size = sizeof (unsigned char);
   to_val->addr = (caddr_t) &i;

   if (StringsAreEqual (in_str, "arrow_up"))
      i = XmARROW_UP;
   else if (StringsAreEqual (in_str, "arrow_down"))
      i = XmARROW_DOWN;
   else if (StringsAreEqual (in_str, "arrow_left")) 
      i = XmARROW_LEFT;
   else if (StringsAreEqual (in_str, "arrow_right")) 
      i = XmARROW_RIGHT;
   else
   {
      to_val->size = 0;
      to_val->addr = NULL;
      XtStringConversionWarning ((char *)from_val->addr, XmRArrowDirection);
   }
}


/************************************************************************
 *
 *  _XmCvtStringToPacking
 *	Convert string to row column packing type.
 *
 ************************************************************************/

/* ARGSUSED */
static void _XmCvtStringToPacking (args, num_args, from_val, to_val)
XrmValuePtr args;
Cardinal * num_args;
XrmValue * from_val;
XrmValue * to_val;

{
   char *in_str = (char *) (from_val->addr);
   static unsigned char i;

   to_val->size = sizeof (unsigned char);
   to_val->addr = (caddr_t) &i;

   if (StringsAreEqual (in_str, "pack_none")) 
      i = XmPACK_NONE;
   else if (StringsAreEqual (in_str, "pack_tight")) 
      i = XmPACK_TIGHT;
   else if (StringsAreEqual (in_str, "pack_column")) 
      i = XmPACK_COLUMN;
   else
   {
      to_val->size = 0;
      to_val->addr = NULL;
      XtStringConversionWarning ((char *)from_val->addr, XmRPacking);
   }
}


/************************************************************************
 *
 *  _XmCvtStringToChar
 *	Convert string to a single character (a mnemonic)
 *
 ************************************************************************/

/* ARGSUSED */
static void _XmCvtStringToChar (args, num_args, from_val, to_val)
XrmValuePtr args;
Cardinal * num_args;
XrmValue * from_val;
XrmValue * to_val;

{
   char *in_str = (char *) (from_val->addr);
   static unsigned char i;

   to_val->size = sizeof (unsigned char);
   to_val->addr = (caddr_t) &i;

   i = *in_str;
}


/************************************************************************
 *
 *  _XmCvtStringToOrientation
 *	Convert string to orientation.
 *
 ************************************************************************/

/* ARGSUSED */
static void _XmCvtStringToOrientation (args, num_args, from_val, to_val)
XrmValuePtr args;
Cardinal * num_args;
XrmValue * from_val;
XrmValue * to_val;

{
   char * in_str = (char *)(from_val->addr);
   static unsigned char i;

   to_val->size = sizeof (unsigned char);
   to_val->addr = (caddr_t) &i;

   if (StringsAreEqual (in_str, "vertical")) 
      i = XmVERTICAL;
   else if (StringsAreEqual (in_str, "horizontal")) 
      i = XmHORIZONTAL;
   else
   {
      to_val->size = 0;
      to_val->addr = NULL;
      XtStringConversionWarning ((char *)from_val->addr, XmROrientation);
   }
}


/************************************************************************
 *
 *  _XmCvtStringToAttachment
 *	Convert a string to a form attacment
 *
 ************************************************************************/

/* ARGSUSED */
static void _XmCvtStringToAttachment (args, num_args, from_val, to_val)
XrmValue * args;
Cardinal   num_args;
XrmValue * from_val;
XrmValue * to_val;

{
   char * in_str = (char *) (from_val->addr);
   static unsigned char i;

   to_val->size = sizeof (unsigned char);
   to_val->addr = (caddr_t) &i;

   if (StringsAreEqual (in_str, "attach_none"))
      i = XmATTACH_NONE;
   else if (StringsAreEqual (in_str, "attach_form"))
      i = XmATTACH_FORM;
   else if (StringsAreEqual (in_str, "attach_opposite_form"))
      i = XmATTACH_OPPOSITE_FORM;
   else if (StringsAreEqual (in_str, "attach_widget"))
      i = XmATTACH_WIDGET;
   else if (StringsAreEqual (in_str, "attach_opposite_widget"))
      i = XmATTACH_OPPOSITE_WIDGET;
   else if (StringsAreEqual (in_str, "attach_position"))
      i = XmATTACH_POSITION;
   else if (StringsAreEqual (in_str, "attach_self"))
      i = XmATTACH_SELF;
   else
   {
      to_val->size = 0;
      to_val->addr = NULL;
      XtStringConversionWarning ((char *)from_val->addr, XmRAttachment);
   }
}


/************************************************************************
 *
 *  _XmCvtStrintToAlignment
 *	Convert a string to Label's alignment type.
 *
 ************************************************************************/

/* ARGSUSED */
static void _XmCvtStringToAlignment (args, num_args, from_val, to_val)
XrmValuePtr args;
Cardinal * num_args;
XrmValue * from_val;
XrmValue * to_val;

{
   char *in_str = (char *) (from_val->addr);
   static unsigned char i;

   to_val->size = sizeof (unsigned char);
   to_val->addr = (caddr_t) &i;

   if (StringsAreEqual (in_str, "alignment_center")) 
      i = XmALIGNMENT_CENTER;
   else if (StringsAreEqual (in_str, "alignment_beginning")) 
      i = XmALIGNMENT_BEGINNING;
   else if (StringsAreEqual (in_str, "alignment_end")) 
      i = XmALIGNMENT_END;
   else
   {
      to_val->size = 0;
      to_val->addr = NULL;
      XtStringConversionWarning ((char *)from_val->addr, XmRAlignment);
   }
}


/************************************************************************
 *
 *  _XmCvtStringToLabelType
 *	Convert a string to a label type.
 *
 ************************************************************************/

/* ARGSUSED */
static void _XmCvtStringToLabelType (args, num_args, from_val, to_val)
XrmValuePtr args;
Cardinal * num_args;
XrmValue * from_val;
XrmValue * to_val;

{
   char *in_str = (char *) (from_val->addr);
   static unsigned char i;

   to_val->size = sizeof (unsigned char);
   to_val->addr = (caddr_t) &i;

   if (StringsAreEqual (in_str, "string")) 
      i = XmSTRING;
   else if (StringsAreEqual (in_str, "pixmap")) 
      i = XmPIXMAP;
   else
   {
      to_val->size = 0;
      to_val->addr = NULL;
      XtStringConversionWarning ((char *)from_val->addr, XmRLabelType);
   }
}


/************************************************************************
 *
 *  _XmCvtStringToIndicatorType
 *	Convert a string to the toggle indicator type
 *
 ************************************************************************/

/* ARGSUSED */
static void _XmCvtStringToIndicatorType (args, num_args, from_val, to_val)
XrmValuePtr args;
Cardinal * num_args;
XrmValue * from_val;
XrmValue * to_val;

{
   char *in_str = (char *) (from_val->addr);
   static unsigned char i;

   to_val->size = sizeof (unsigned char);
   to_val->addr = (caddr_t) &i;

   if (StringsAreEqual (in_str, "n_of_many")) 
      i = XmN_OF_MANY;
   else if (StringsAreEqual (in_str, "one_of_many")) 
      i = XmONE_OF_MANY;
   else
   {
      to_val->size = 0;
      to_val->addr = NULL;
      XtStringConversionWarning ((char *)from_val->addr, XmRIndicatorType);
   }
}


/************************************************************************
 *
 *  _XmCvtStringToEditMode
 *	Convert a string to an edit mode.
 *
 ************************************************************************/

/* ARGSUSED */
static void _XmCvtStringToEditMode (args, num_args, from_val, to_val)
XrmValue * args;
Cardinal   num_args;
XrmValue * from_val;
XrmValue * to_val;

{
   char * in_str = (char *)(from_val->addr);
   static int i;

   to_val->size = sizeof (int);
   to_val->addr = (caddr_t) &i;

   if (StringsAreEqual (in_str, "multi_line_edit"))
      i = XmMULTI_LINE_EDIT;
   else if (StringsAreEqual (in_str, "single_line_edit"))
      i = XmSINGLE_LINE_EDIT;
   else
   {
      to_val->size = 0;
      to_val->addr = NULL;
      XtStringConversionWarning ((char *)from_val->addr, XmREditMode);
   }
}


/************************************************************************
 *
 *  _XmCvtStringToScrollPolicy
 *	Convert a string to a scrolling policy.
 *
 ************************************************************************/

static void  _XmCvtStringToScrollPolicy (args, num_args, from_val, to_val)
XrmValuePtr args;
Cardinal * num_args;
XrmValue * from_val;
XrmValue * to_val;

{
   char *in_str = (char *) (from_val->addr);
   static unsigned char i;

   to_val->size = sizeof (unsigned char);
   to_val->addr = (caddr_t) &i;

   if (StringsAreEqual (in_str, "automatic")) 
      i = XmAUTOMATIC;
   else if (StringsAreEqual (in_str, "application_defined")) 
      i = XmAPPLICATION_DEFINED;
   else
   {
      to_val->size = 0;
      to_val->addr = NULL;
      XtStringConversionWarning ((char *)from_val->addr, XmRScrollingPolicy);
   }
}


/************************************************************************
 *
 *  _XmCvtStringToVisualPolicy
 *	Convert a string to a visual policy.
 *
 ************************************************************************/

static void  _XmCvtStringToVisualPolicy (args, num_args, from_val, to_val)
XrmValuePtr args;
Cardinal * num_args;
XrmValue * from_val;
XrmValue * to_val;

{
   char *in_str = (char *) (from_val->addr);
   static unsigned char i;

   to_val->size = sizeof (unsigned char);
   to_val->addr = (caddr_t) &i;

   if (StringsAreEqual (in_str, "variable")) 
      i = XmVARIABLE;
   else if (StringsAreEqual (in_str, "constant")) 
      i = XmCONSTANT;
   else
   {
      to_val->size = 0;
      to_val->addr = NULL;
      XtStringConversionWarning ((char *)from_val->addr, XmRVisualPolicy);
   }
}


/************************************************************************
 *
 *  _XmCvtStringToSBDisplayPolicy
 *	Convert a string to a scroll bar display policy.
 *
 ************************************************************************/

static void  _XmCvtStringToSBDisplayPolicy (args, num_args, from_val, to_val)
XrmValuePtr args;
Cardinal * num_args;
XrmValue * from_val;
XrmValue * to_val;

{
   char *in_str = (char *) (from_val->addr);
   static unsigned char i;

   to_val->size = sizeof (unsigned char);
   to_val->addr = (caddr_t) &i;

   if (StringsAreEqual (in_str, "as_needed")) 
      i = XmAS_NEEDED;
   else if (StringsAreEqual (in_str, "static")) 
      i = XmSTATIC;
   else
   {
      to_val->size = 0;
      to_val->addr = NULL;
      XtStringConversionWarning ((char *)from_val->addr, XmRScrollBarDisplayPolicy);
   }
}


/************************************************************************
 *
 *  _XmCvtStringToSBPlacement
 *	Convert a string to a scrollbar placement
 *
 ************************************************************************/

static void  _XmCvtStringToSBPlacement (args, num_args, from_val, to_val)
XrmValuePtr args;
Cardinal * num_args;
XrmValue * from_val;
XrmValue * to_val;

{
   char *in_str = (char *) (from_val->addr);
   static unsigned char i;

   to_val->size = sizeof (unsigned char);
   to_val->addr = (caddr_t) &i;

   if (StringsAreEqual (in_str, "top_left")) 
      i = XmTOP_LEFT;
   else if (StringsAreEqual (in_str, "top_right")) 
      i = XmTOP_RIGHT;
   else if (StringsAreEqual (in_str, "bottom_left")) 
      i = XmBOTTOM_LEFT;
   else if (StringsAreEqual (in_str, "bottom_right")) 
      i = XmBOTTOM_RIGHT;
   else
   {
      to_val->size = 0;
      to_val->addr = NULL;
      XtStringConversionWarning ((char *)from_val->addr, XmRScrollBarPlacement);
   }
}


/************************************************************************
 *
 *  _XmCvtStringToSelectionPolicy
 *	Convert a string to a selection policy.
 *
 ************************************************************************/

static void  _XmCvtStringToSelectionPolicy (args, num_args, from_val, to_val)
XrmValuePtr args;
Cardinal * num_args;
XrmValue * from_val;
XrmValue * to_val;

{
   char *in_str = (char *) (from_val->addr);
   static unsigned char i;

   to_val->size = sizeof (unsigned char);
   to_val->addr = (caddr_t) &i;

   if (StringsAreEqual (in_str, "single_select")) 
      i = XmSINGLE_SELECT;
   else if (StringsAreEqual (in_str, "multiple_select")) 
      i = XmMULTIPLE_SELECT;
   else if (StringsAreEqual (in_str, "extended_select")) 
      i = XmEXTENDED_SELECT;
   else if (StringsAreEqual (in_str, "browse_select")) 
      i = XmBROWSE_SELECT;
   else
   {
      to_val->size = 0;
      to_val->addr = NULL;
      XtStringConversionWarning ((char *)from_val->addr, XmRSelectionPolicy);
   }
}


/************************************************************************
 *
 *  _XmCvtStringToListSizePolicy
 *	Convert a string to a list size policy.
 *
 ************************************************************************/

static void  _XmCvtStringToListSizePolicy (args, num_args, from_val, to_val)
XrmValuePtr args;
Cardinal * num_args;
XrmValue * from_val;
XrmValue * to_val;

{
   char *in_str = (char *) (from_val->addr);
   static unsigned char i;

   to_val->size = sizeof (unsigned char);
   to_val->addr = (caddr_t) &i;

   if (StringsAreEqual (in_str, "constant")) 
      i = XmCONSTANT;
   else if (StringsAreEqual (in_str, "variable")) 
      i = XmVARIABLE;
   else if (StringsAreEqual (in_str, "resize_if_possible")) 
      i = XmRESIZE_IF_POSSIBLE;
   else
   {
      to_val->size = 0;
      to_val->addr = NULL;
      XtStringConversionWarning ((char *)from_val->addr, XmRListSizePolicy);
   }
}
/************************************************************************
 *
 *  _XmCvtStringToCmdWindowLoc
 *	Convert a string to a command window location
 *
 ************************************************************************/

static void  _XmCvtStringToCmdWindowLoc (args, num_args, from_val, to_val)
XrmValuePtr args;
Cardinal * num_args;
XrmValue * from_val;
XrmValue * to_val;

{
   char *in_str = (char *) (from_val->addr);
   static unsigned char i;

   to_val->size = sizeof (unsigned char);
   to_val->addr = (caddr_t) &i;

   if (StringsAreEqual (in_str, "command_above_workspace")) 
      i = XmCOMMAND_ABOVE_WORKSPACE;
   else if (StringsAreEqual (in_str, "command_below_workspace")) 
      i = XmCOMMAND_BELOW_WORKSPACE;
   else
   {
      to_val->size = 0;
      to_val->addr = NULL;
      XtStringConversionWarning ((char *)from_val->addr, XmRCommandWindowLocation);
   }
}


/************************************************************************
 *
 *  XmCvtStringToUnitType
 *	Convert a string to resolution independent unit type.
 *
 ************************************************************************/

void XmCvtStringToUnitType (args, num_args, from_val, to_val)
XrmValuePtr args;
Cardinal * num_args;
XrmValue * from_val;
XrmValue * to_val;

{
   char *in_str = (char *) (from_val->addr);
   static unsigned char i;

   to_val->size = sizeof (unsigned char);
   to_val->addr = (caddr_t) &i;

   if (StringsAreEqual (in_str, "pixels")) 
      i = XmPIXELS;
   else if (StringsAreEqual (in_str, "100th_millimeters")) 
      i = Xm100TH_MILLIMETERS;
   else if (StringsAreEqual (in_str, "1000th_inches")) 
      i = Xm1000TH_INCHES;
   else if (StringsAreEqual (in_str, "100th_points")) 
      i = Xm100TH_POINTS;
   else if (StringsAreEqual (in_str, "100th_font_units")) 
      i = Xm100TH_FONT_UNITS;
   else
   {
      to_val->size = 0;
      to_val->addr = NULL;
      XtStringConversionWarning ((char *)from_val->addr, XmRUnitType);
   }
}


/************************************************************************
 *
 *  _XmCvtStringToProcessingDirection
 *	Convert a string to a scrollbar processing direction.
 *
 ************************************************************************/

/* ARGSUSED */
static void _XmCvtStringToProcessingDirection (args, num_args, from_val, to_val)
XrmValue * args;
Cardinal   num_args;
XrmValue * from_val;
XrmValue * to_val;

{
   char * in_str = (char *) (from_val->addr);
   static unsigned char i;

   to_val->size = sizeof (unsigned char);
   to_val->addr = (caddr_t) &i;

   if (StringsAreEqual (in_str, "max_on_top")) 
      i = XmMAX_ON_TOP;
   else if (StringsAreEqual (in_str, "max_on_bottom")) 
      i = XmMAX_ON_BOTTOM;
   else if (StringsAreEqual (in_str, "max_on_left")) 
      i = XmMAX_ON_LEFT;
   else if (StringsAreEqual (in_str, "max_on_right")) 
      i = XmMAX_ON_RIGHT;
   else
   {
      to_val->size = 0;
      to_val->addr = NULL;
      XtStringConversionWarning ((char *)from_val->addr, XmRProcessingDirection);
   }
}

/************************************************************************
 *
 *  _XmCvtStringToStringDirection
 *	Convert a string to a string direction.
 *
 ************************************************************************/

/* ARGSUSED */
static void _XmCvtStringToStringDirection (args, num_args, from_val, to_val)
XrmValue * args;
Cardinal   num_args;
XrmValue * from_val;
XrmValue * to_val;

{
   char * in_str = (char *) (from_val->addr);
   static unsigned char i;

   to_val->size = sizeof (unsigned char);
   to_val->addr = (caddr_t) &i;

   if (StringsAreEqual (in_str, "string_direction_l_to_r")) 
       i = XmSTRING_DIRECTION_L_TO_R;
   else if (StringsAreEqual (in_str, "string_direction_r_to_l")) 
       i = XmSTRING_DIRECTION_R_TO_L;
   else
   {
      to_val->size = 0;
      to_val->addr = NULL;
      XtStringConversionWarning ((char *)from_val->addr, XmRStringDirection);
   }
}

/************************************************************************
 *
 *   _XmCvtStringToKeySym
 *	Convert a string to a KeySym
 *
 ************************************************************************/

static void  _XmCvtStringToKeySym (args, num_args, from_val, to_val)
XrmValue * args;
Cardinal   num_args;
XrmValue * from_val;
XrmValue * to_val;

{
   char * in_str = (char *) (from_val->addr);
   static KeySym i;

   to_val->size = sizeof (KeySym);
   to_val->addr = (caddr_t) &i;

   i = XStringToKeysym(in_str);
   if (i == NoSymbol)
   {
      to_val->size = 0;
      to_val->addr = NULL;
      XtStringConversionWarning ((char *)from_val->addr, XmRStringDirection);
   }
}


/************************************************************************
 *
 *  _XmCvtStringToSeparatorType
 *       Convert a string to a separator type  
 *
 ************************************************************************/

/* ARGSUSED */
static void _XmCvtStringToSeparatorType (args, num_args, from_val, to_val)
XrmValue * args;
Cardinal   num_args;
XrmValue * from_val;
XrmValue * to_val;

{
   char * in_str = (char *) (from_val->addr);
   static unsigned char i;

   to_val->size = sizeof (unsigned char);
   to_val->addr = (caddr_t) &i;


   if (StringsAreEqual (in_str, "no_line"))                 
      i = XmNO_LINE;
   else if (StringsAreEqual (in_str, "single_line"))   
      i = XmSINGLE_LINE;
   else if (StringsAreEqual (in_str, "double_line"))        
      i = XmDOUBLE_LINE;
   else if (StringsAreEqual (in_str, "single_dashed_line")) 
      i = XmSINGLE_DASHED_LINE;
   else if (StringsAreEqual (in_str, "double_dashed_line")) 
      i = XmDOUBLE_DASHED_LINE;
   else if (StringsAreEqual (in_str, "shadow_etched_out"))  
      i = XmSHADOW_ETCHED_OUT;
   else if (StringsAreEqual (in_str, "shadow_etched_in"))  
      i = XmSHADOW_ETCHED_IN;
   else
   {
      to_val->size = 0;
      to_val->addr = NULL;
      XtStringConversionWarning ((char *)from_val->addr, XmRSeparatorType);
   }
}


/************************************************************************
 *
 *  _XmCvtStringToShadowType
 *       Convert a string to a shadow type (DrawnButton)
 *
 ************************************************************************/

/* ARGSUSED */
static void _XmCvtStringToShadowType (args, num_args, from_val, to_val)
XrmValue * args;
Cardinal   num_args;
XrmValue * from_val;
XrmValue * to_val;

{
   char * in_str = (char *) (from_val->addr);
   static unsigned char i;

   to_val->size = sizeof (unsigned char);
   to_val->addr = (caddr_t) &i;

   if (StringsAreEqual (in_str, "shadow_etched_in"))
      i = XmSHADOW_ETCHED_IN;
   else if (StringsAreEqual (in_str, "shadow_etched_out"))  
      i = XmSHADOW_ETCHED_OUT;
   else if (StringsAreEqual (in_str, "shadow_in"))                 
      i = XmSHADOW_IN;
   else if (StringsAreEqual (in_str, "shadow_out"))   
      i = XmSHADOW_OUT;
   else
   {
      to_val->size = 0;
      to_val->addr = NULL;
      XtStringConversionWarning ((char *)from_val->addr, XmRShadowType);
   }
}


/************************************************************************
 *
 *  _XmCvtStringToXmString
 *	Convert an ASCII string to a XmString.
 *
 ************************************************************************/

/* ARGSUSED */
static void _XmCvtStringToXmString (args, num_args, from_val, to_val)
XrmValuePtr args;
Cardinal * num_args;
XrmValue * from_val;
XrmValue * to_val;

{
   char  *s;
   static XmString i;

   if (from_val->addr == NULL)
      return;

   to_val->addr = (caddr_t) &i;
   to_val->size = sizeof(caddr_t);

   s = from_val->addr;

   i = XmStringCreateLtoR (s, XmSTRING_DEFAULT_CHARSET);

   if (i == NULL)
   {
      to_val->addr = NULL;
      to_val->size = 0;
      XtStringConversionWarning (s, XmRXmString);
   }
}


/************************************************************************
 *									*
 * GetNextFontList - parse a string into a font name and a character	*
 * set.  Modifies the pointers passed in.				*
 *									*
 ************************************************************************/
static Boolean GetNextFontList(s,fs,cs)
char **s, **fs, **cs;

{
    if (**s == '\0')
        return(FALSE);

/****************
 *
 * Skip any leading whitespace. If we find a valid char, point the 
 * fontstruct parameter at it.
 *
 ****************/
    while(isspace(**s) && **s != '\0') (*s)++;

    if (**s == '\0')
        return(FALSE);
    *fs = *s;
/****************
 *
 * Now look for the '='. If we don't find it, set the charset to the
 * default and return. NOTE that we can quote stuff with a pair of ""
 *
 ****************/
    if (**s == '"')
    {
        (*fs)++; (*s)++;
        while(**s != '"')
            (*s)++;
        **s = '\0';
        (*s)++;
    }
    else
        while((**s != '=') && (**s != ',') && (**s != '\0'))
            (*s)++;


    if (**s != '=') 
    {
	if (**s == ',' || **s == '\0')
	{
	    *cs = (char *)XmSTRING_DEFAULT_CHARSET;
            if (**s != '\0')
            {
                **s = '\0';
                (*s)++;
            }
            return(TRUE);
        }
        else
            return(FALSE);
    }
    **s = '\0'; (*s)++;                 /*  Else replace the = with a null */

/****************
 *
 * And look for the character set. Quotes work here, too.
 *
 ****************/
    while(isspace(**s) && **s != '\0') (*s)++;

    if (**s == '\0')
        return(FALSE);
    *cs = *s;

    if (**s == '"')
    {
        (*cs)++; (*s)++;
        while(**s != '"')
            (*s)++;
        **s = '\0';
        (*s)++;
    }
    else
        while(!isspace(**s) && (**s != ',') && **s != '\0') (*s)++;

    **s = '\0'; (*s)++;
    return(TRUE);
}


/************************************************************************
 *
 *  _XmCvtStringToXmFontList
 *	Convert a string to a fontlist.  This is in the form :
 *
 *       FontName [=charset] [,FontName [=charset] ]+".
 *
 ************************************************************************/

/* ARGSUSED */
static void _XmCvtStringToXmFontList (args, num_args, from_val, to_val)
XrmValuePtr args;
Cardinal * num_args;
XrmValue * from_val;
XrmValue * to_val;

{
   char lowercase[128];
   char *s, *fs, *cs;
   Screen *screen;	
   Boolean got_it; 
   Display *display;
   XFontStruct *font_struct;
   static XmFontList font_list;

   if (from_val->addr == NULL)
      return;

   s = (char *) from_val->addr;
   screen = * ((Screen **) args[0].addr);
   display = DisplayOfScreen(screen);
   got_it = FALSE;


   /* Look for fontlists in the args. If we can't open any, then open     */
   /* "fixed" and punt. Ya know, I should check for errors and put out a  */
   /* nice message like "can't open font %s", but oh well...              */

   if (!GetNextFontList(&s,&fs,&cs))
   {
      font_struct = XLoadQueryFont(display, "fixed");

      /* alternative try, it should work this time! */
      if (!font_struct) font_struct = XLoadQueryFont(display, "*");

      if (font_struct)
      {
         got_it = TRUE;	    
         font_list = XmFontListCreate (font_struct, XmSTRING_DEFAULT_CHARSET);
      } else {
	  XtWarning("Can't open one single font! exiting...");
	  exit(1);
      }
   }
   else
   {
      LowerCase (fs, lowercase);
      font_struct = XLoadQueryFont (display, lowercase);

      if (font_struct)
      {
         got_it = TRUE;
         font_list = XmFontListCreate (font_struct, cs);
      }

      while(GetNextFontList (&s, &fs, &cs))
      {
         LowerCase (fs, lowercase);
         font_struct = XLoadQueryFont (display, lowercase);    

         if (font_struct)
         {
            got_it = TRUE;	    
            font_list = XmFontListAdd (font_list, font_struct, cs);      
         }
      }
   }

   if (got_it)
   {
      to_val->addr = (caddr_t ) &font_list;
      to_val->size = sizeof (caddr_t);
   }
   else
   {
      to_val->addr = NULL;
      to_val->size = 0;
      XtStringConversionWarning ((char *)from_val->addr, XmRFontList);
   }
}


/************************************************************************
 *									*
 * GetNextXmString - return a pointer to a null-terminated string.	*
 *                   The pointer is passed in cs. Up to the caller to   *
 *    		     free that puppy. Returns FALSE if end of string.	*
 *									*
 ************************************************************************/

static Boolean GetNextXmString(s,cs)
char **s, **cs;

{
   char *tmp;


   if (**s == '\0')
      return(FALSE);


   /*  Skip any leading whitespace.  */

   while(isspace(**s) && **s != '\0') (*s)++;

   if (**s == '\0')
      return(FALSE);
  

   /* Found something. Allocate some space (ugh!) and start copying  */
   /* the next string                                                */

   *cs = XtMalloc(strlen(*s));
   tmp = *cs;

   while((**s) != '\0') 
   {
      if ((**s) == '\\' && *((*s)+1) == ',')	/* Quoted comma */
      {
         (*s)+=2;
         *tmp = ',';
         tmp++;
      }
      else
      {
         if((**s) == ',')			/* End of a string */
         {
            *tmp = '\0';
            (*s)++;
            return(TRUE);
         }
         else
         {
            *tmp = **s;
            tmp++; 
            (*s)++;
         }
       }
    }

    *tmp = '\0';
    return(TRUE);
}


/************************************************************************
 *
 * _XmCvtStringToXmStringTable
 *
 * Convert a string table to an array of XmStrings.This is in the form :
 *
 *       		String [, String2]*
 *
 * The comma delimeter can be  quoted by a \
 *
 ************************************************************************/

/* ARGSUSED */
static Boolean _XmCvtStringToXmStringTable (dpy, args, num_args, from_val, to_val, data)
Display *dpy;
XrmValuePtr args;
Cardinal * num_args;
XrmValue * from_val;
XrmValue * to_val;
XtPointer *data;
{
   char  *s, *cs;
   XmString *table;
   static  XmString *tblptr;
   int TableCount = 0;
   int	  i, j;

   if (from_val->addr == NULL)
      return(FALSE);

   s = (char *) from_val->addr;
   j = 100;
   table = (XmString *) XtMalloc(sizeof(XmString) * j);
   for (i = 0; GetNextXmString (&s, &cs); i++)
   {
      if (i >= j)
       {
           j *= 2;
           table = (XmString *)XtRealloc(table, (sizeof(XmString) * j));
       }
      table[i] = XmStringCreateLtoR (cs, XmSTRING_DEFAULT_CHARSET);
      XtFree(cs);
   }
/****************
 *
 * NULL terminate the array...
 *
 ****************/
   i ++;
   table =  (XmString *)XtRealloc(table, (sizeof(XmString) * i));
   table[i-1] = (XmString ) NULL;
   tblptr = table;

   if (to_val->addr != NULL) 
   {
	if (to_val->size < sizeof(caddr_t)) 
        {
	    to_val->size = sizeof(caddr_t);	
	    return False;
	}		
        *(XmString **)(to_val->addr) = table;
    }
    else 
    {
	to_val->addr = (XtPointer)&tblptr;
    }				
    to_val->size = sizeof(caddr_t);
    return(TRUE);
}

/****************
 *
 * _XmXmStringCvtDestroy - free up the space allocated by the converter
 *
 ****************/
static void _XmXmStringCvtDestroy(app, to, data, args, num_args)
XtAppContext app;
XrmValue * to;
XtPointer  data;
XrmValue * args;
Cardinal * num_args;
{
   int i;
   XmString *table = *(XmString **)(to->addr);
   for (i = 0; table[i] != NULL; i++)
       XmStringFree(table[i]);       
}
 
 
/************************************************************************
 *
 *  _XmCvtStringToWhichButton
 *       Convert a string to a Button constant
 *
 ************************************************************************/

/* ARGSUSED */
static void _XmCvtStringToWhichButton (args, num_args, from_val, to_val)
XrmValue * args;
Cardinal   num_args;
XrmValue * from_val;
XrmValue * to_val;

{
   char * in_str = (char *) (from_val->addr);
   static unsigned int i;

   to_val->size = sizeof (unsigned int);
   to_val->addr = (caddr_t) &i;

   if (StringsAreEqual (in_str, "button1"))                 
      i = Button1;
   else if (StringsAreEqual (in_str, "button2"))   
      i = Button2;
   else if (StringsAreEqual (in_str, "button3"))        
      i = Button3;
   else if (StringsAreEqual (in_str, "button4")) 
      i = Button4;
   else if (StringsAreEqual (in_str, "button5")) 
      i = Button5;
   else
   {
      to_val->size = 0;
      to_val->addr = NULL;
      XtStringConversionWarning ((char *)from_val->addr, XmRWhichButton);
   }
}


/************************************************************************
 *
 *  _XmCvtStringToResizePolicy
 *
 ************************************************************************/

/* ARGSUSED */
static void _XmCvtStringToResizePolicy (args, num_args, from_val, to_val)
XrmValue * args;
Cardinal   num_args;
XrmValue * from_val;
XrmValue * to_val;

{
   char * in_str = (char *) (from_val->addr);
   static unsigned char i;

   to_val->size = sizeof (unsigned char);
   to_val->addr = (caddr_t) &i;

   if (StringsAreEqual (in_str, "resize_none"))
      i = XmRESIZE_NONE;
   else if (StringsAreEqual (in_str, "resize_grow")) 
      i = XmRESIZE_GROW;
   else if (StringsAreEqual (in_str, "resize_any")) 
      i = XmRESIZE_ANY;
   else
   {
      to_val->size = 0;
      to_val->addr = NULL;
      XtStringConversionWarning ((char *)from_val->addr, XmRResizePolicy);
   }
}


/************************************************************************
 *
 *  _XmCvtStringToDialogStyle
 *
 ************************************************************************/

/* ARGSUSED */
static void _XmCvtStringToDialogStyle (args, num_args, from_val, to_val)
XrmValue * args;
Cardinal   num_args;
XrmValue * from_val;
XrmValue * to_val;

{
   char * in_str = (char *) (from_val->addr);
   static unsigned char i;

   to_val->size = sizeof (unsigned char);
   to_val->addr = (caddr_t) &i;

   if (StringsAreEqual (in_str, "dialog_work_area"))
       i = XmDIALOG_WORK_AREA;
   else if (StringsAreEqual (in_str, "dialog_modeless")) 
       i = XmDIALOG_MODELESS;
   else if (StringsAreEqual (in_str, "dialog_application_modal"))
       i = XmDIALOG_APPLICATION_MODAL;
   else if (StringsAreEqual (in_str, "dialog_full_application_modal"))
       i = XmDIALOG_FULL_APPLICATION_MODAL;
   else if (StringsAreEqual (in_str, "dialog_primary_application_modal"))
       i = XmDIALOG_PRIMARY_APPLICATION_MODAL;
   else if (StringsAreEqual (in_str, "dialog_system_modal"))
       i = XmDIALOG_SYSTEM_MODAL;
   else
   {
      to_val->size = 0;
      to_val->addr = NULL;
      XtStringConversionWarning ((char *)from_val->addr, XmRDialogStyle);
   }
}


/************************************************************************
 *
 *  _XmCvtStringToDialogType
 *
 ************************************************************************/

/* ARGSUSED */
static void _XmCvtStringToDialogType (args, num_args, from_val, to_val)
XrmValue * args;
Cardinal   num_args;
XrmValue * from_val;
XrmValue * to_val;

{
   char * in_str = (char *) (from_val->addr);
   static unsigned char i;

   to_val->size = sizeof (unsigned char);
   to_val->addr = (caddr_t) &i;

   if (StringsAreEqual (in_str, "dialog_error"))
       i = XmDIALOG_ERROR;
   else if (StringsAreEqual (in_str, "dialog_information"))
       i = XmDIALOG_INFORMATION;
   else if (StringsAreEqual (in_str, "dialog_message"))
       i = XmDIALOG_MESSAGE;
   else if (StringsAreEqual (in_str, "dialog_question"))
       i = XmDIALOG_QUESTION;
   else if (StringsAreEqual (in_str, "dialog_warning"))
       i = XmDIALOG_WARNING;
   else if (StringsAreEqual (in_str, "dialog_working"))
       i = XmDIALOG_WORKING;
   else if (StringsAreEqual (in_str, "dialog_prompt"))
       i = XmDIALOG_PROMPT;
   else if (StringsAreEqual (in_str, "dialog_selection"))
       i = XmDIALOG_SELECTION;
   else if (StringsAreEqual (in_str, "dialog_command"))
       i = XmDIALOG_COMMAND;
   else
   {
      to_val->size = 0;
      to_val->addr = NULL;
      XtStringConversionWarning ((char *)from_val->addr, XmRDialogType);
   }
}


/************************************************************************
 *
 *  _XmCvtStringToDefaultButtonType
 *
 ************************************************************************/

/* ARGSUSED */
static void _XmCvtStringToDefaultButtonType (args, num_args, from_val, to_val)
XrmValue * args;
Cardinal   num_args;
XrmValue * from_val;
XrmValue * to_val;

{
   char * in_str = (char *) (from_val->addr);
   static unsigned char i;

   to_val->size = sizeof (unsigned char);
   to_val->addr = (caddr_t) &i;

   if (StringsAreEqual (in_str, "dialog_ok_button"))
       i = XmDIALOG_OK_BUTTON;
   else if (StringsAreEqual (in_str, "dialog_cancel_button"))
       i = XmDIALOG_CANCEL_BUTTON;
   else if (StringsAreEqual (in_str, "dialog_help_button"))
       i = XmDIALOG_HELP_BUTTON;
   else
   {
      to_val->size = 0;
      to_val->addr = NULL;
      XtStringConversionWarning ((char *)from_val->addr, XmRDefaultButtonType);
   }
}


/************************************************************************
 *
 *  _XmCvtStringToRowColumnType
 *
 ************************************************************************/

/* ARGSUSED */
static void _XmCvtStringToRowColumnType (args, num_args, from_val, to_val)
XrmValue * args;
Cardinal   num_args;
XrmValue * from_val;
XrmValue * to_val;

{
   char * in_str = (char *) (from_val->addr);
   static unsigned char i;

   to_val->size = sizeof (unsigned char);
   to_val->addr = (caddr_t) &i;

   if (StringsAreEqual (in_str, "work_area"))
       i = XmWORK_AREA;
   else if (StringsAreEqual (in_str, "menu_bar"))
       i = XmMENU_BAR;
   else if (StringsAreEqual (in_str, "menu_popup"))
       i = XmMENU_POPUP;
   else if (StringsAreEqual (in_str, "menu_pulldown"))
       i = XmMENU_PULLDOWN;
   else if (StringsAreEqual (in_str, "menu_option"))
       i = XmMENU_OPTION;
   else
   {
      to_val->size = 0;
      to_val->addr = NULL;
      XtStringConversionWarning ((char *)from_val->addr, XmRRowColumnType);
   }
}

/* ARGSUSED */
static void _XmCvtStringToHorizontalPosition (args, num_args, from_val, to_val)
XrmValue * args;
Cardinal * num_args;
XrmValue * from_val;
XrmValue * to_val;
{
    unsigned char unitType = (unsigned char) *(args[0].addr);
    Screen * screen = * ((Screen **) args[1].addr);
    static Position pixels;
    int intermediate;

    intermediate = (int) atoi(from_val->addr);

    to_val->size = sizeof(Position);
    to_val->addr = (caddr_t) &pixels;

    pixels = (Position)
        _XmConvertUnits(screen,XmHORIZONTAL,(int) unitType,
            intermediate,XmPIXELS);
}

/* ARGSUSED */
static void _XmCvtStringToHorizontalDimension (args, num_args, from_val, to_val)
XrmValue * args;
Cardinal * num_args;
XrmValue * from_val;
XrmValue * to_val;
{
    unsigned char unitType = (unsigned char) *(args[0].addr);
    Screen * screen = * ((Screen **) args[1].addr);
    static Dimension pixels;
    int intermediate;

    intermediate = (int) atoi(from_val->addr);

    to_val->size = sizeof(Dimension);
    to_val->addr = (caddr_t) &pixels;

    pixels = (Dimension)
        _XmConvertUnits(screen,XmHORIZONTAL,(int) unitType,
            intermediate,XmPIXELS);
}

/* ARGSUSED */
static void _XmCvtStringToVerticalPosition (args, num_args, from_val, to_val)
XrmValue * args;
Cardinal * num_args;
XrmValue * from_val;
XrmValue * to_val;
{
    unsigned char unitType = (unsigned char) *(args[0].addr);
    Screen * screen = * ((Screen **) args[1].addr);
    static Position pixels;
    int intermediate;

    intermediate = (int) atoi(from_val->addr);

    to_val->size = sizeof(Position);
    to_val->addr = (caddr_t) &pixels;

    pixels = (Position)
        _XmConvertUnits(screen,XmVERTICAL,(int) unitType,
            intermediate,XmPIXELS);
}

/* ARGSUSED */
static void _XmCvtStringToVerticalDimension (args, num_args, from_val, to_val)
XrmValue * args;
Cardinal * num_args;
XrmValue * from_val;
XrmValue * to_val;
{
    unsigned char unitType = (unsigned char) *(args[0].addr);
    Screen * screen = * ((Screen **) args[1].addr);
    static Dimension pixels;
    int intermediate;

    intermediate = (int) atoi(from_val->addr);

    to_val->size = sizeof(Dimension);
    to_val->addr = (caddr_t) &pixels;

    pixels = (Dimension)
        _XmConvertUnits(screen,XmVERTICAL,(int) unitType,
            intermediate,XmPIXELS);
}


/************************************************************************
 *
 *  _XmGetDefaultFontList
 *       This function is called by a widget to initialize it's fontlist
 *   resource with a default, when it is NULL. This is done by checking to
 *   see if any of the widgets, in the widget's parent hierarchy is a
 *   subclass of BulletinBoard or the VendorShell widget class, and if it
 *   is, returning the BulletinBoard or VendorShell fontlist. 
 *
 *************************************************************************/

XmFontList _XmGetDefaultFontList (w, fontListType)
Widget w;
unsigned char fontListType;

{
 Arg al[1];
 XmFontList data;

   while (w = XtParent(w))
   {
      if (XmIsBulletinBoard(w))
      {
         if (fontListType == XmLABEL_FONTLIST)
             XtSetArg(al[0], XmNlabelFontList, & data);
	 else if (fontListType == XmTEXT_FONTLIST)
	     XtSetArg(al[0], XmNtextFontList, & data);
	 else if (fontListType == XmBUTTON_FONTLIST)
	     XtSetArg(al[0], XmNbuttonFontList, & data);

	 XtGetValues (w, al, 1);
         return (data);
      }

      else
      {
         if (XmIsVendorShell (w) ||
	     XmIsMenuShell (w))
         {
           XtSetArg(al[0], XmNdefaultFontList, & data);
           XtGetValues (w, al, 1);
           return (data);
         } 
      }
   }
   return (NULL);
} 

/* ARGSUSED */
static void _XmConvertStringToButtonType(args, num_args, from_val, to_val)
XrmValuePtr args;
Cardinal num_args;
XrmValuePtr from_val, to_val;
{
    int i, comma_count;
    static int size = 0;
    static XmButtonTypeTable buttonTable;
    String in_str = (String) from_val->addr;
    String work_str, btype_str;
    
    for (comma_count = i = 0; i < strlen(in_str); i++)
        if (in_str[i] == ',')
            comma_count++;

    /* 
     * This ONLY works if there is NEVER more than one converted button
     * table in existence at any time
     */
    if (++comma_count > size)
    {
        XtFree(buttonTable);
        buttonTable = (XmButtonTypeTable) 
            XtMalloc(sizeof(XmButtonType) * comma_count);
    }

    work_str = (String) XtMalloc(strlen(in_str));

    strcpy(work_str, in_str);

    for (btype_str = (char*)strtok(work_str,","), i = 0;
        btype_str; btype_str = (char*)strtok(NULL,","), i++)
    {
        if (StringsAreEqual(btype_str, "pushbutton"))
            buttonTable[i] = XmPUSHBUTTON;
        else if (StringsAreEqual(btype_str, "togglebutton"))
            buttonTable[i] = XmTOGGLEBUTTON;
        else if (StringsAreEqual(btype_str, "cascadebutton"))
            buttonTable[i] = XmCASCADEBUTTON;
        else if (StringsAreEqual(btype_str, "separator"))
            buttonTable[i] = XmSEPARATOR;
        else if (StringsAreEqual(btype_str, "double_separator"))
            buttonTable[i] = XmDOUBLE_SEPARATOR;
        else if (StringsAreEqual(btype_str, "title"))
            buttonTable[i] = XmTITLE;
        else
        {
            XtStringConversionWarning((char *)btype_str,
                XmRButtonType);
            buttonTable[i] = XmNONE;
        }
    }
    XtFree(work_str);

    to_val->addr = (caddr_t) &buttonTable;
    to_val->size = sizeof(buttonTable);
}

/* ARGSUSED */
static void _XmCvtStringToKeySymTable(args, num_args, from_val, to_val)
XrmValuePtr args;
Cardinal num_args;
XrmValuePtr from_val, to_val;
{
    static XmKeySymTable keySymTable;
    int i, comma_count;
	static int size = 0;
    String in_str = (String) from_val->addr;
    String work_str, ks_str;
    XrmValue single_string, single_keysym;
    KeySym ks;

    for (comma_count = i = 0; i < strlen(in_str); i++)
        if (in_str[i] == ',')
            comma_count++;

    /* 
     * This ONLY works if there is NEVER more than one converted keysym
     * table in existence at any time
     */
    if (++comma_count > size)
    {
        XtFree(keySymTable);
        keySymTable = (XmKeySymTable) 
            XtMalloc(sizeof(KeySym) * comma_count);
    }

    work_str = (String) XtMalloc(strlen(in_str));

    strcpy(work_str, in_str);

    for (ks_str = (char*)strtok(work_str,","), i = 0;
        ks_str; ks_str = (char*)strtok(NULL,","), i++)
    {
        if (strlen(ks_str) == 0)
            keySymTable[i] = NoSymbol;
        else
        {
            if ((ks = XStringToKeysym(ks_str)) == NoSymbol)
                XtStringConversionWarning(ks_str, XmRKeySym);
            keySymTable[i] = ks;
        }

    }

    to_val->addr = (caddr_t) &keySymTable;
    to_val->size = sizeof(keySymTable);
}

/* ARGSUSED */
static void _XmCvtStringToCharSetTable(args, num_args, from_val, to_val)
XrmValuePtr args;
Cardinal num_args;
XrmValuePtr from_val, to_val;
{
    static XmStringCharSetTable charsetTable;
    int i, comma_count;
    String in_str = (String) from_val->addr;
    String work_str, cs_str;
    XrmValue single_string, single_charset;
    XmStringCharSet cs;

    for (comma_count = i = 0; i < strlen(in_str); i++)
        if (in_str[i] == ',')
            comma_count++;

    charsetTable = (XmStringCharSetTable) 
        XtMalloc(sizeof(XmStringCharSet) * comma_count);

    work_str = (String) XtMalloc(strlen(in_str));

    strcpy(work_str, in_str);

    for (cs_str = (char*)strtok(work_str,","), i = 0;
        cs_str; cs_str = (char*)strtok(NULL,","), i++)
    {
        if (strlen(cs_str) > 0)
        {
            cs = (XmStringCharSet) XtMalloc(strlen(cs_str));
            strcpy(cs, cs_str);
            charsetTable[i] = cs;
        }
        else
            charsetTable[i] = NULL;
    }

    to_val->addr = (caddr_t) &charsetTable;
    to_val->size = sizeof(charsetTable);
}

/************************************************************************
 *
 *  _XmCvtStringToFileTypeMask
 *
 ************************************************************************/

/* ARGSUSED */
static void _XmCvtStringToFileTypeMask (args, num_args, from_val, to_val)
XrmValue * args;
Cardinal   num_args;
XrmValue * from_val;
XrmValue * to_val;

{
   char * in_str = (char *) (from_val->addr);
   static unsigned char i;

   to_val->size = sizeof (unsigned char);
   to_val->addr = (caddr_t) &i;

   if (StringsAreEqual (in_str, "file_directory"))
      i = XmFILE_DIRECTORY ;
   else if (StringsAreEqual (in_str, "file_regular")) 
      i = XmFILE_REGULAR ;
   else if (StringsAreEqual (in_str, "file_any_type")) 
      i = XmFILE_ANY_TYPE ;
   else
   {
      to_val->size = 0;
      to_val->addr = NULL;
      XtStringConversionWarning ((char *)from_val->addr, XmRFileTypeMask);
   }
}

/************************************************************************
 *
 *  _XmCvtStringToBooleanDimension
 *
 ************************************************************************/

/* ARGSUSED */
static void _XmCvtStringToBooleanDimension (args, num_args, from_val, to_val)
XrmValue * args;
Cardinal   num_args;
XrmValue * from_val;
XrmValue * to_val;
{
    static  Dimension       val ;
            char *          in_str = (char *) (from_val->addr) ;

    if(    ('0' <= in_str[0])  &&  (in_str[0] <= '9')    )
    {   
        /* Is numeric argument, so convert to horizontal dimension.  This is
        *   to preserve 1.0 compatibility (the resource actually behaves like
        *   a boolean in version 1.1).
        */
        _XmCvtStringToHorizontalDimension( args, num_args, from_val, to_val) ;
        } 
    else
    {   /* Presume Boolean (version 1.1).
        */
        to_val->size = sizeof( Dimension) ;
        to_val->addr = (caddr_t) &val ;
                
        if(    StringsAreEqual( in_str, "true")    )
        {   val = (Dimension) 1 ;
            } 
        else
        {   if(    StringsAreEqual( in_str, "false")    )
            {   val = (Dimension) 0 ;
                } 
            else
            {   to_val->size = 0 ;
                to_val->addr = NULL ;
                XtStringConversionWarning( in_str, XmRBooleanDimension) ;
                } 
            } 
        } 
    return ;
    }

/******************************************************************************
*	Compound text <-> compound string conversion routines
******************************************************************************/

/*
** Define standard character set strings
*/
#define CS_ISO8859_1		"ISO8859-1"
#define CS_ISO8859_2		"ISO8859-2"
#define CS_ISO8859_3		"ISO8859-3"
#define CS_ISO8859_4		"ISO8859-4"
#define CS_ISO8859_5		"ISO8859-5"
#define CS_ISO8859_6		"ISO8859-6"
#define CS_ISO8859_7		"ISO8859-7"
#define CS_ISO8859_8		"ISO8859-8"
#define CS_ISO8859_9		"ISO8859-9"
#define CS_JISX0201		"JISX0201.1976-0"
#define CS_GB2312_0		"GB2312.1980-0"
#define CS_GB2312_1		"GB2312.1980-1"
#define CS_JISX0208_0		"JISX0208.1983-0"
#define CS_JISX0208_1		"JISX0208.1983-1"
#define CS_KSC5601_0		"KSC5601.1987-0"
#define CS_KSC5601_1		"KSC5601.1987-1"


/* Define handy macros (note: these constants are in OCTAL) */
#define EOS	00
#define STX	02
#define HT	011
#define NL	012
#define ESC	033
#define CSI	0233

#define NEWLINESTRING			"\012"
#define NEWLINESTRING_LEN		sizeof(NEWLINESTRING)-1

#define CTEXT_L_TO_R			"\233\061\135"
#define CTEXT_L_TO_R_LEN		sizeof(CTEXT_L_TO_R)-1

#define CTEXT_R_TO_L			"\233\062\135"
#define CTEXT_R_TO_L_LEN		sizeof(CTEXT_R_TO_L)-1

#define CTEXT_SET_ISO8859_1		"\033\050\102\033\055\101"
#define CTEXT_SET_ISO8859_1_LEN		sizeof(CTEXT_SET_ISO8859_1)-1

#define CTEXT_SET_ISO8859_2		"\033\050\102\033\055\102"
#define CTEXT_SET_ISO8859_2_LEN		sizeof(CTEXT_SET_ISO8859_2)-1

#define CTEXT_SET_ISO8859_3		"\033\050\102\033\055\103"
#define CTEXT_SET_ISO8859_3_LEN		sizeof(CTEXT_SET_ISO8859_3)-1

#define CTEXT_SET_ISO8859_4		"\033\050\102\033\055\104"
#define CTEXT_SET_ISO8859_4_LEN		sizeof(CTEXT_SET_ISO8859_4)-1

#define CTEXT_SET_ISO8859_5		"\033\050\102\033\055\114"
#define CTEXT_SET_ISO8859_5_LEN		sizeof(CTEXT_SET_ISO8859_5)-1

#define CTEXT_SET_ISO8859_6		"\033\050\102\033\055\107"
#define CTEXT_SET_ISO8859_6_LEN		sizeof(CTEXT_SET_ISO8859_6)-1

#define CTEXT_SET_ISO8859_7		"\033\050\102\033\055\106"
#define CTEXT_SET_ISO8859_7_LEN		sizeof(CTEXT_SET_ISO8859_7)-1

#define CTEXT_SET_ISO8859_8		"\033\050\102\033\055\110"
#define CTEXT_SET_ISO8859_8_LEN		sizeof(CTEXT_SET_ISO8859_8)-1

#define CTEXT_SET_ISO8859_9		"\033\050\102\033\055\115"
#define CTEXT_SET_ISO8859_9_LEN		sizeof(CTEXT_SET_ISO8859_9)-1

#define CTEXT_SET_JISX0201		"\033\050\112\033\051\111"
#define CTEXT_SET_JISX0201_LEN		sizeof(CTEXT_SET_JISX0201)-1

#define CTEXT_SET_GB2312_0		"\033\044\050\101\033\044\051\101"
#define CTEXT_SET_GB2312_0_LEN		sizeof(CTEXT_SET_GB2312_0)-1

#define CTEXT_SET_JISX0208_0		"\033\044\050\102\033\044\051\102"
#define CTEXT_SET_JISX0208_0_LEN	sizeof(CTEXT_SET_JISX0208_0)-1

#define CTEXT_SET_KSC5601_0		"\033\044\050\103\033\044\051\103"
#define CTEXT_SET_KSC5601_0_LEN		sizeof(CTEXT_SET_KSC5601_0)-1


#define CTVERSION 1
#define _IsValidC0(ctx, c)	(((c) == HT) || ((c) == NL) || ((ctx)->version > CTVERSION)) 
#define _IsValidC1(ctx, c)	((ctx)->version > CTVERSION)
 
#define _IsValidESCFinal(c)	(((c) >= 0x30) && ((c) <= 0x7e))
#define _IsValidCSIFinal(c)	(((c) >= 0x40) && ((c) <= 0x7e))

#define _IsInC0Set(c)		((c) <= 0x1f)
#define _IsInC1Set(c)		(((c) >= 0x80) && ((c) <= 0x9f))
#define _IsInGLSet(c)		(((c) >= 0x20) && ((c) <= 0x7f))
#define _IsInGRSet(c)		((c) >= 0xa0)
#define _IsInColumn2(c)		(((c) >= 0x20) && ((c) <= 0x2f))
#define _IsInColumn3(c)		(((c) >= 0x30) && ((c) <= 0x3f))
#define _IsInColumn4(c)		(((c) >= 0x40) && ((c) <= 0x4f))
#define _IsInColumn5(c)		(((c) >= 0x50) && ((c) <= 0x5f))
#define _IsInColumn6(c)		(((c) >= 0x60) && ((c) <= 0x6f))
#define _IsInColumn7(c)		(((c) >= 0x70) && ((c) <= 0x7f))
#define _IsInColumn4or5(c)	(((c) >= 0x40) && ((c) <= 0x5f))

typedef unsigned char Octet;
typedef unsigned char *OctetPtr;

typedef enum {
    ct_Dir_StackEmpty,
    ct_Dir_Undefined,
    ct_Dir_LeftToRight,
    ct_Dir_RightToLeft
} ct_Direction;

/*
** ct_Charset is used in the xmstring_to_text conversion to keep track
** of the prevous character set.  The order is not important.
*/
typedef enum {
    cs_none,
    cs_Hanzi,
    cs_JapaneseGCS,
    cs_Katakana,
    cs_KoreanGCS,
    cs_Latin1,
    cs_Latin2,
    cs_Latin3,
    cs_Latin4,
    cs_Latin5,
    cs_LatinArabic,
    cs_LatinCyrillic,
    cs_LatinGreek,
    cs_LatinHebrew,
    cs_NonStandard
} ct_Charset; 

/* Internal context block */
typedef struct _ct_context {
    OctetPtr	    octet;		/* octet ptr into compound text stream */
    OctetPtr	    lastoctet;		/* ptr to last octet in stream */
    struct {				/* flags */
	unsigned    dircs	: 1;	/*   direction control seq encountered */
	unsigned    gchar	: 1;	/*   graphic characters encountered */
	unsigned    ignext	: 1;	/*   ignore extensions */
	unsigned    gl		: 1;	/*   text is for gl */
	unsigned    text	: 1;	/*   current item is a text seq */
    } flags;
    ct_Direction    *dirstack;		/* direction stack pointer */
    unsigned int    dirsp;		/* current dir stack index */
    unsigned int    dirstacksize;	/* size of direction stack */
    OctetPtr	    item;		/* ptr to current item */
    unsigned int    itemlen;		/* length of current item */
    unsigned int    version;		/* version of compound text */
    String	    gl_charset;	/* ptr to GL character set */
    String	    gr_charset;	/* ptr to GR character set */
    unsigned char   gl_charset_size;	/* # of chars in GL charset */
    unsigned char   gr_charset_size;	/* # of chars in GR charset */
    unsigned char   gl_octets_per_char;	/* # of octets per GL char */
    unsigned char   gr_octets_per_char;	/* # of octets per GR char */
    XmString	    xmstring;		/* compound string to be returned */
    XmString	    xmsep;		/* compound string separator segment */
} ct_context;


#define _SetGL(ctx, charset, size, octets)\
    (ctx)->flags.gl = True;\
    (ctx)->gl_charset = (charset);\
    (ctx)->gl_charset_size = (size);\
    (ctx)->gl_octets_per_char = (octets)

#define _SetGR(ctx, charset, size, octets)\
    (ctx)->flags.gl = False;\
    (ctx)->gr_charset = (charset);\
    (ctx)->gr_charset_size = (size);\
    (ctx)->gr_octets_per_char = (octets)

#define _PushDir(ctx, dir)\
    if ( (ctx)->dirsp == ((ctx)->dirstacksize - 1) ) {\
	(ctx)->dirstacksize += 8;\
	(ctx)->dirstack = \
	    (ct_Direction *)XtRealloc((char *)(ctx)->dirstack,\
				(ctx)->dirstacksize * sizeof(ct_Direction));\
    }\
    (ctx)->dirstack[++((ctx)->dirsp)] = dir;\
    (ctx)->flags.dircs = True

#define _PopDir(ctx)	((ctx)->dirsp)--

#define _CurDir(ctx)	(ctx)->dirstack[(ctx)->dirsp]


/* 
 * forward references
 */

static Boolean cvtTextToXmString ();
static void outputXmString  ();
static XmString concatStringToXmString ();
static Boolean processESC  ();
static Boolean processESCHack  ();
static Boolean processCSI  ();
static Boolean processExtendedSegments ();
static Boolean processExtendedSegmentsHack ();
static Boolean process94n  ();
static Boolean process94GL  ();
static Boolean process94GR  ();
static Boolean process96GR  ();
static Boolean cvtXmStringToText ();
static OctetPtr ctextConcat  ();

typedef unsigned char UChar;
typedef unsigned short UShort;



/************************************************************************
 *
 *  XmCvtCTToXmString
 *	Convert a compound text string to a XmString.  This is the public
 *	version which takes only a compound text string as an argument.
 *	Note: processESC and processExtendedSegments have to be hacked
 *	for this to work.
 *
 ************************************************************************/

XmString XmCvtCTToXmString (text)
     char	*text;
{
    ct_context	    *ctx;		/* compound text context block */
    Boolean	    ok = True;
    Octet	    c;

    ctx = (ct_context *) XtMalloc(sizeof(ct_context));

/* initialize the context block */
    ctx->octet = (OctetPtr)text;
    ctx->flags.dircs = False;
    ctx->flags.gchar = False;
    ctx->flags.ignext = False;
    ctx->flags.gl = False;
    ctx->flags.text = False;
    ctx->dirstacksize = 8;
    ctx->dirstack = (ct_Direction *)
            XtMalloc(ctx->dirstacksize*sizeof(ct_Direction));
    ctx->dirstack[0] = ct_Dir_StackEmpty;
    ctx->dirstack[1] = ct_Dir_LeftToRight;
    ctx->dirsp = 1;
    ctx->item = NULL;
    ctx->itemlen = 0;
    ctx->version = CTVERSION;
    ctx->gl_charset = CS_ISO8859_1;
    ctx->gl_charset_size = 94;
    ctx->gl_octets_per_char = 1;
    ctx->gr_charset = CS_ISO8859_1;
    ctx->gr_charset_size = 96;
    ctx->gr_octets_per_char = 1;
    ctx->xmstring = NULL;
    ctx->xmsep = NULL;

/*
** check for version/ignore extensions sequence (must be first if present)
**  Format is:	ESC 02/03 V 03/00   ignoring extensions is OK
**		ESC 02/03 V 03/01   ignoring extensions is not OK
**  where V is in the range 02/00 thru 02/15 and represents versions 1 thru 16
*/
    if (    (ctx->octet[0] == ESC)
	&&  (ctx->octet[1] == 0x23)
	&&  (_IsInColumn2(ctx->octet[2])
	&&  ((ctx->octet[3] == 0x30) || ctx->octet[3] == 0x31))
       ) {
	ctx->version = ctx->octet[2] - 0x1f;	/* 0x20-0x2f => version 1-16 */
	if (ctx->octet[3] == 0x30)		/* 0x30 == can ignore extensions */
	    ctx->flags.ignext = True;
	ctx->octet += 4;			/* advance ptr to next seq */
    }


    while (ctx->octet[0] != NULL) {
    switch (*ctx->octet) {			/* look at next octet in seq */
	case ESC:
	    /* %%% TEMP
	    ** if we have any text to output, do it
	    ** this section needs to be optimized so that it handles
	    ** paired character sets without outputting a new segment.
	    */
	    if (ctx->flags.text) {
		outputXmString(ctx, False);	/* with no separator */
	    }
	    ctx->flags.text = False;
	    ctx->item = ctx->octet;		/* remember start of this item */
	    ctx->itemlen = 0;

	    ctx->octet++; ctx->itemlen++;	/* advance ptr to next char */

	    /* scan for final char */
	    while (	(ctx->octet[0] != NULL)
		     && (_IsInColumn2(*ctx->octet)) ) {
		ctx->octet++; ctx->itemlen++;	/* advance ptr to next char */
	    }

	    if (ctx->octet[0] == NULL) {	/* if nothing after this, it's an error */
		ok = False;
		break;
	    }

	    c = *ctx->octet;			/* get next char in seq */
	    ctx->octet++; ctx->itemlen++;	/* advance ptr to next char */
	    if (_IsValidESCFinal(c)) {
		/* we have a valid ESC sequence - handle it */
		ok = processESCHack(ctx, c);
	    } else {
		ok = False;
	    }
	    break;

	case CSI:
	    /*
	    ** CSI format is:	CSI P I F   where
	    **	    03/00 <= P <= 03/15
	    **	    02/00 <= I <= 02/15
	    **	    04/00 <= F <= 07/14
	    */
	    /* %%% TEMP
	    ** if we have any text to output, do it
	    ** This may need optimization.
	    */
	    if (ctx->flags.text) {
		outputXmString(ctx, True);	/* with a separator */
	    }
	    ctx->flags.text = False;
	    ctx->item = ctx->octet;		/* remember start of this item */
	    ctx->itemlen = 0;

	    ctx->octet++; ctx->itemlen++;	/* advance ptr to next char */

	    /* scan for final char */
	    while (	(ctx->octet[0] != NULL)
		    &&	_IsInColumn3(*ctx->octet)  ) {
		ctx->octet++; ctx->itemlen++;	/* advance ptr to next char */
	    }
	    while (	(ctx->octet[0] != NULL)
		    && _IsInColumn2(*ctx->octet)   ) {
		ctx->octet++; ctx->itemlen++;	/* advance ptr to next char */
	    }

	    /* if nothing after this, it's an error */
	    if (ctx->octet[0] == NULL) {
		ok = False;
		break;
	    }

	    c = *ctx->octet;			/* get next char in seq */
	    ctx->octet++; ctx->itemlen++;	/* advance ptr to next char */
	    if (_IsValidCSIFinal(c)) {
		/* we have a valid CSI sequence - handle it */
		ok = processCSI(ctx, c);
	    } else {
		ok = False;
	    }
	    break;

	case NL:			    /* new line */
	    /* if we have any text to output, do it */
	    if (ctx->flags.text) {
		outputXmString(ctx, True);	/* with a separator */
		ctx->flags.text = False;
	    } else {
		XmString    save;
		if (ctx->xmsep == NULL) {
		    ctx->xmsep = XmStringSeparatorCreate();
		}
		save = ctx->xmstring;
		ctx->xmstring = XmStringConcat(ctx->xmstring,
						ctx->xmsep);
		XmStringFree(save);		/* free original xmstring */
	    }
	    ctx->octet++;			/* advance ptr to next char */
	    break;

	case HT:
	    /* Tab currently has no meaning in XmString, so just ignore it. */

	    /* if we have any text to output, do it */
	    if (ctx->flags.text) {
	      outputXmString(ctx, False);	/* with no separator */
	    }
	    ctx->flags.text = False;

	    ctx->octet++;			/* advance ptr to next char */
	    break;

	default:			    /* just 'normal' text */
	    ctx->item = ctx->octet;		/* remember start of this item */
	    ctx->itemlen = 0;
	    ctx->flags.text = True;
	    while (ctx->octet[0] != NULL) {
	      c = *ctx->octet;
	      if ((c == ESC) || (c == CSI) || (c == NL) || (c == HT)) {
		break;
	      }	
	      
		if (	(_IsInC0Set(c) && (!_IsValidC0(ctx, c)))
		    ||	(_IsInC1Set(c) && (!_IsValidC1(ctx, c))) ) {
		    ok = False;
		    break;
		}
		ctx->flags.gchar = True;	/* We have a character! */
		if (ctx->flags.gl) {
		    ctx->octet += ctx->gl_octets_per_char;
		    ctx->itemlen += ctx->gl_octets_per_char;
		} else {
		    ctx->octet += ctx->gr_octets_per_char;
		    ctx->itemlen += ctx->gr_octets_per_char;
		}
	    } /* end while */
	    break;
	} /* end switch */
    if (!ok) break;
    } /* end while */

/* if we have any text left to output, do it */
    if (ctx->flags.text) {
	outputXmString(ctx, False);		/* with no separator */
    }

    XtFree(ctx->dirstack);
    if (ctx->xmsep != NULL) XmStringFree(ctx->xmsep);

    if (ok)
      return ( (XmString)ctx->xmstring );
    else 
	return ( (XmString)NULL );
    
}


/***********************************************************************
 *
 * Hacked procedures to work with XmCvtCTToXmString.
 *
 ***********************************************************************/

/* processESCHack - handle valid ESC sequences */

static Boolean processESCHack(ctx, final)
    ct_context	    *ctx;
    Octet	    final;
{
    Boolean	    ok;

    switch (ctx->item[1]) {
    case 0x24:			/* 02/04 - invoke 94(n) charset into GL or GR */
	ok = process94n(ctx, final);
	break;
    case 0x25:			/* 02/05 - extended segments */
	/* if we have any text to output, do it */
	if (ctx->flags.text) {
	    outputXmString(ctx, False);	/* with no separator */
	    ctx->flags.text = False;
	}
	ok = processExtendedSegmentsHack(ctx, final);
	break;
    case 0x28:			/* 02/08 - invoke 94 charset into GL */
	ok = process94GL(ctx, final);
	break;
    case 0x29:			/* 02/09 - invoke 94 charset into GR */
	ok = process94GR(ctx, final);
	break;
    case 0x2d:			/* 02/13 - invoke 96 charset into GR */
	ok =  process96GR(ctx, final);
	break;
    default:
	ok = False;
	break;
    }
    return(ok);
}


static Boolean processExtendedSegmentsHack(ctx, final)
    ct_context	    *ctx;
    Octet	    final;
{
    OctetPtr	    esptr;			/* ptr into ext seg */
    unsigned int    seglen;			/* length of ext seg */
    unsigned int    len;			/* length */
    String	    charset_copy;		/* ptr to NULL-terminated copy of ext seg charset */
    OctetPtr	    text_copy;			/* ptr to NULL-terminated copy of ext seg text */
    XmString	    tempxm1, tempxm2;
    Boolean	    ok = True;

    /* Extended segments
    **  01/11 02/05 02/15 03/00 M L	    variable # of octets/char
    **  01/11 02/05 02/15 03/01 M L	    1 octet/char
    **  01/11 02/05 02/15 03/02 M L	    2 octets/char
    **  01/11 02/05 02/15 03/03 M L	    3 octets/char
    **  01/11 02/05 02/15 03/04 M L	    4 octets/char
    */
    if (	(ctx->itemlen == 4)
	&&	(ctx->item[2] == 0x2f)
	&&	(_IsInColumn3(final))
       ) 
      {
	if (    (ctx->octet[0] < 0x80)
	    ||  (ctx->octet[1] < 0x80)
	   )	
	  {
	    return(False);
	  }

	/*
	** The most significant bit of M and L are always set to 1
	** The number is computed as ((M-128)*128)+(L-128)
	*/
	seglen = *ctx->octet - 0x80;
	ctx->octet++; ctx->itemlen++;		/* advance pointer */
	seglen = (seglen << 7) + (*ctx->octet - 0x80);
	ctx->octet++; ctx->itemlen++;		/* advance pointer */
	
	/* Check for premature end. */
	for (esptr = ctx->octet; esptr < (ctx->octet + seglen); esptr++) 
	  {
	    if (*esptr == NULL) 
	      {
		return(False);
	      }
	  }	

        esptr = ctx->octet;			/* point to charset */
	ctx->itemlen += seglen;			/* advance pointer over segment */
	ctx->octet += seglen;

	switch (final) {
	case 0x30:				/* variable # of octets per char */
	case 0x31:				/* 1 octet per char */
	case 0x32:				/* 2 octets per char */
	    /* scan for STX separator between charset and text */
	    len = 0;
	    while (esptr[len] != STX)
		len++;
	    if (len > ctx->itemlen) {		/* if we ran off the end, error */
		ok = False;
		break;
	    }
	    charset_copy = XtMalloc(len + 1);
	    strncpy(charset_copy, esptr, len);
	    charset_copy[len] = EOS;
	    esptr += len + 1;			/* point to text part */
	    len = seglen - len - 1;		/* calc length of text part */
	    text_copy = (unsigned char *) XtMalloc(len + 1);
	    bcopy (esptr, text_copy, len);
	    text_copy[len] = EOS;
	    tempxm1 = 
	      XmStringSegmentCreate((char *) text_copy,
				    charset_copy,
				    (unsigned char)(_CurDir(ctx) == 
						    ct_Dir_LeftToRight ?
						    XmSTRING_DIRECTION_L_TO_R :
						    XmSTRING_DIRECTION_R_TO_L),
				    False );
	    tempxm2 = ctx->xmstring;
	    ctx->xmstring = XmStringConcat(ctx->xmstring, tempxm1);
	    XtFree(text_copy);
	    XtFree(charset_copy);
	    XmStringFree(tempxm1);		/* free xm string */
	    XmStringFree(tempxm2);		/* free original xm string */
	    ok = True;
	    break;
	    
	case 0x33:				/* 3 octets per char */
	case 0x34:				/* 4 octets per char */
	    /* not supported */
	    ok = False;
	    break;

	default:
	    /* reserved for future use */
	    ok = False;
	    break;
	} /* end switch */
    } /* end if */

    return(ok);
}
  

/************************************************************************
 *
 *  XmCvtTextToXmString
 *	Convert a compound text string to a XmString.
 *
 ************************************************************************/

Boolean XmCvtTextToXmString (display, args, num_args, from_val, to_val, converter_data)
    Display	*display;
    XrmValuePtr	args;
    Cardinal	*num_args;
    XrmValue	*from_val;
    XrmValue	*to_val;
    XtPointer	*converter_data;
{
    Boolean		ok;

    if (from_val->addr == NULL)
	return(False);

    ok = cvtTextToXmString(from_val, to_val);

    if (!ok)
    {
	to_val->addr = NULL;
	to_val->size = 0;
	XtAppWarningMsg(
			XtDisplayToApplicationContext(display),
			"conversionError","compoundText", "XtToolkitError",
			"Cannot convert compound text string to type XmString",
			(String *)NULL, (Cardinal *)NULL);
    }
    return(ok);
}


static Boolean cvtTextToXmString(from, to)
    XrmValue	    *from;
    XrmValue	    *to;
{
    ct_context	    *ctx;		/* compound text context block */
    Boolean	    ok = True;
    Octet	    c;

    ctx = (ct_context *) XtMalloc(sizeof(ct_context));

/* initialize the context block */
    ctx->octet = (OctetPtr)from->addr;
    ctx->lastoctet = ctx->octet + from->size;
    ctx->flags.dircs = False;
    ctx->flags.gchar = False;
    ctx->flags.ignext = False;
    ctx->flags.gl = False;
    ctx->flags.text = False;
    ctx->dirstacksize = 8;
    ctx->dirstack = (ct_Direction *)
            XtMalloc(ctx->dirstacksize*sizeof(ct_Direction));
    ctx->dirstack[0] = ct_Dir_StackEmpty;
    ctx->dirstack[1] = ct_Dir_LeftToRight;
    ctx->dirsp = 1;
    ctx->item = NULL;
    ctx->itemlen = 0;
    ctx->version = CTVERSION;
    ctx->gl_charset = CS_ISO8859_1;
    ctx->gl_charset_size = 94;
    ctx->gl_octets_per_char = 1;
    ctx->gr_charset = CS_ISO8859_1;
    ctx->gr_charset_size = 96;
    ctx->gr_octets_per_char = 1;
    ctx->xmstring = NULL;
    ctx->xmsep = NULL;

/*
** check for version/ignore extensions sequence (must be first if present)
**  Format is:	ESC 02/03 V 03/00   ignoring extensions is OK
**		ESC 02/03 V 03/01   ignoring extensions is not OK
**  where V is in the range 02/00 thru 02/15 and represents versions 1 thru 16
*/
    if (    (from->size >= 4)
	&&  (ctx->octet[0] == ESC)
	&&  (ctx->octet[1] == 0x23)
	&&  (_IsInColumn2(ctx->octet[2])
	&&  ((ctx->octet[3] == 0x30) || ctx->octet[3] == 0x31))
       ) {
	ctx->version = ctx->octet[2] - 0x1f;	/* 0x20-0x2f => version 1-16 */
	if (ctx->octet[3] == 0x30)		/* 0x30 == can ignore extensions */
	    ctx->flags.ignext = True;
	ctx->octet += 4;			/* advance ptr to next seq */
    }


    while (ctx->octet < ctx->lastoctet) {
    switch (*ctx->octet) {			/* look at next octet in seq */
	case ESC:
	    /* %%% TEMP
	    ** if we have any text to output, do it
	    ** this section needs to be optimized so that it handles
	    ** paired character sets without outputting a new segment.
	    */
	    if (ctx->flags.text) {
		outputXmString(ctx, False);	/* with no separator */
	    }
	    ctx->flags.text = False;
	    ctx->item = ctx->octet;		/* remember start of this item */
	    ctx->itemlen = 0;

	    ctx->octet++; ctx->itemlen++;	/* advance ptr to next char */

	    /* scan for final char */
	    while (	(ctx->octet != ctx->lastoctet)
		     && (_IsInColumn2(*ctx->octet)) ) {
		ctx->octet++; ctx->itemlen++;	/* advance ptr to next char */
	    }

	    if (ctx->octet == ctx->lastoctet) {	/* if nothing after this, it's an error */
		ok = False;
		break;
	    }

	    c = *ctx->octet;			/* get next char in seq */
	    ctx->octet++; ctx->itemlen++;	/* advance ptr to next char */
	    if (_IsValidESCFinal(c)) {
		/* we have a valid ESC sequence - handle it */
		ok = processESC(ctx, c);
	    } else {
		ok = False;
	    }
	    break;

	case CSI:
	    /*
	    ** CSI format is:	CSI P I F   where
	    **	    03/00 <= P <= 03/15
	    **	    02/00 <= I <= 02/15
	    **	    04/00 <= F <= 07/14
	    */
	    /* %%% TEMP
	    ** if we have any text to output, do it
	    ** This may need optimization.
	    */
	    if (ctx->flags.text) {
		outputXmString(ctx, True);	/* with a separator */
	    }
	    ctx->flags.text = False;
	    ctx->item = ctx->octet;		/* remember start of this item */
	    ctx->itemlen = 0;

	    ctx->octet++; ctx->itemlen++;	/* advance ptr to next char */

	    /* scan for final char */
	    while (	(ctx->octet != ctx->lastoctet)
		    &&	_IsInColumn3(*ctx->octet)  ) {
		ctx->octet++; ctx->itemlen++;	/* advance ptr to next char */
	    }
	    while (	(ctx->octet != ctx->lastoctet)
		    && _IsInColumn2(*ctx->octet)   ) {
		ctx->octet++; ctx->itemlen++;	/* advance ptr to next char */
	    }

	    /* if nothing after this, it's an error */
	    if (ctx->octet == ctx->lastoctet) {
		ok = False;
		break;
	    }

	    c = *ctx->octet;			/* get next char in seq */
	    ctx->octet++; ctx->itemlen++;	/* advance ptr to next char */
	    if (_IsValidCSIFinal(c)) {
		/* we have a valid CSI sequence - handle it */
		ok = processCSI(ctx, c);
	    } else {
		ok = False;
	    }
	    break;

	case NL:			    /* new line */
	    /* if we have any text to output, do it */
	    if (ctx->flags.text) {
		outputXmString(ctx, True);	/* with a separator */
		ctx->flags.text = False;
	    } else {
		XmString    save;
		if (ctx->xmsep == NULL) {
		    ctx->xmsep = XmStringSeparatorCreate();
		}
		save = ctx->xmstring;
		ctx->xmstring = XmStringConcat(ctx->xmstring,
						ctx->xmsep);
		XmStringFree(save);		/* free original xmstring */
	    }
	    ctx->octet++;			/* advance ptr to next char */
	    break;

	case HT:
	    /* Tab currently has no meaning in an XmString, ignore it. */

	    /* if we have any text to output, do it */
	    if (ctx->flags.text) {
	      outputXmString(ctx, False);	/* with no separator */
	    }
	    ctx->flags.text = False;

	    ctx->octet++;			/* advance ptr to next char */
	    break;

	default:			    /* just 'normal' text */
	    ctx->item = ctx->octet;		/* remember start of this item */
	    ctx->itemlen = 0;
	    ctx->flags.text = True;
	    while (ctx->octet < ctx->lastoctet) {
		c = *ctx->octet;
		if ((c == ESC) || (c == CSI) || (c == NL) || (c == HT)) {
		    break;
		}
		if (	(_IsInC0Set(c) && (!_IsValidC0(ctx, c)))
		    ||	(_IsInC1Set(c) && (!_IsValidC1(ctx, c))) ) {
		    ok = False;
		    break;
		}
		ctx->flags.gchar = True;	/* We have a character! */
		if (ctx->flags.gl) {
		    ctx->octet += ctx->gl_octets_per_char;
		    ctx->itemlen += ctx->gl_octets_per_char;
		} else {
		    ctx->octet += ctx->gr_octets_per_char;
		    ctx->itemlen += ctx->gr_octets_per_char;
		}
		if (ctx->octet > ctx->lastoctet) {
		    ok = False;
		    break;
		}
	    } /* end while */
	    break;
	} /* end switch */
    if (!ok) break;
    } /* end while */

/* if we have any text left to output, do it */
    if (ctx->flags.text) {
	outputXmString(ctx, False);		/* with no separator */
    }

    XtFree(ctx->dirstack);
    if (ctx->xmstring != NULL) {
	to->addr = (char*)ctx->xmstring;
	to->size = XmStringLength(ctx->xmstring);
    }
    if (ctx->xmsep != NULL) XmStringFree(ctx->xmsep);
    return (ok);
}

/* outputXmString */

static void outputXmString(ctx, separator)
    ct_context		*ctx;
    Boolean		separator;
{
    OctetPtr		tempstring;
    XmString		savedxm;

/*
**  If the GL charset is ISO8859-1, and the GR charset is any ISO8859
**  charset, then they're a pair, so we can create a single segment using
**  just the GR charset.
**
**  If GL and GR are multibyte charsets and they match (both GB2312, both
**  JISX0208 or both KSC5601, then we can create a single segment using
**  just the GR charset.  If GL and GR are multibyte charsets and they DON'T
**  match, or if GL or GR is multibyte and the other is singlebyte, then
**  there's no way to tell which characters belong to GL and which to GR,
**  so treat it like a non-Latin1 in GL - 7 bit characters go to GL, 8 bit
**  characters to to GR.  *** THIS APPEARS TO BE A HOLE IN THE COMPOUND
**  TEXT SPEC ***.
**
**  Otherwise the charsets are not a pair and we will switch between GL
**  and GR segments each time the high bit changes.
*/
    if (    (	!strcmp(ctx->gl_charset, CS_ISO8859_1)
	        &&	
	        (   !strcmp(ctx->gr_charset, CS_ISO8859_1)
		||  !strcmp(ctx->gr_charset, CS_ISO8859_2)
		||  !strcmp(ctx->gr_charset, CS_ISO8859_3)
		||  !strcmp(ctx->gr_charset, CS_ISO8859_4)
		||  !strcmp(ctx->gr_charset, CS_ISO8859_5)
		||  !strcmp(ctx->gr_charset, CS_ISO8859_6)
		||  !strcmp(ctx->gr_charset, CS_ISO8859_7)
		||  !strcmp(ctx->gr_charset, CS_ISO8859_8)
		||  !strcmp(ctx->gr_charset, CS_ISO8859_9)
		)
	    )
	    ||
	    (	!strcmp(ctx->gl_charset, CS_GB2312_0) 
	     && !strcmp(ctx->gr_charset, CS_GB2312_1))
	    ||
	    (	!strcmp(ctx->gl_charset, CS_JISX0208_0)
	     && !strcmp(ctx->gr_charset, CS_JISX0208_1))
	    ||
	    (	!strcmp(ctx->gl_charset, CS_KSC5601_0) 
	     && !strcmp(ctx->gr_charset, CS_KSC5601_1))
	)
	{
	/* OK to do single segment output but always use GR charset */
	tempstring = (unsigned char *) XtMalloc(ctx->itemlen+1);
	strncpy(tempstring, ctx->item, ctx->itemlen);
	tempstring[ctx->itemlen] = EOS;
	ctx->xmstring = concatStringToXmString
			    (	ctx->xmstring,
				(char *) tempstring,
				(char *) ctx->gr_charset,
				(XmStringDirection)
				    ((_CurDir(ctx) == ct_Dir_LeftToRight) ?
					XmSTRING_DIRECTION_L_TO_R :
					XmSTRING_DIRECTION_R_TO_L ),
				separator );
	XtFree(tempstring);			/* free text version */
	}
    else
	{
	/* have to create a new segment everytime the highbit changes */
	unsigned int	i = 0, j = 0;
	Octet		c;
	Boolean		curseg_is_gl;

	curseg_is_gl = isascii(ctx->item[0]);

	tempstring = (unsigned char *) XtMalloc(ctx->itemlen+1);

	while (j < ctx->itemlen)
	    {
	    c = ctx->item[j];
	    if (isascii(c))
		{
		if (!curseg_is_gl)
		    {
		    /* output gr string */
		    tempstring[i] = EOS;
		    ctx->xmstring = concatStringToXmString
					(   ctx->xmstring,
					    (char *) tempstring,
					    (char *) ctx->gr_charset,
					    (XmStringDirection)
						((_CurDir(ctx) == ct_Dir_LeftToRight) ?
						    XmSTRING_DIRECTION_L_TO_R :
						    XmSTRING_DIRECTION_R_TO_L ),
					    False );
		    i = 0;			/* reset tempstring ptr */
		    curseg_is_gl = True;	/* start gl segment */
		    };
		tempstring[i++] = c;		/* copy octet to temp */
		j++;
		}
	    else
		{
		if (curseg_is_gl)
		    {
		    /* output gl string */
		    tempstring[i] = EOS;
		    ctx->xmstring = concatStringToXmString
					(   ctx->xmstring,
					    (char *) tempstring,
					    (char *) ctx->gl_charset,
					    (XmStringDirection)
						((_CurDir(ctx) == ct_Dir_LeftToRight) ?
						    XmSTRING_DIRECTION_L_TO_R :
						    XmSTRING_DIRECTION_R_TO_L ),
					    False );
		    i = 0;			/* reset tempstring ptr */
		    curseg_is_gl = False;	/* start gr segment */
		    };
		tempstring[i++] = c;		/* copy octet to temp */
		j++;
		}; /* end if */
	    }; /* end while */

	/* output last segment */
	tempstring[i] = EOS;
	ctx->xmstring = concatStringToXmString
				(   ctx->xmstring,
				    (char *) tempstring,
				    (char *)
					((curseg_is_gl) ?
					    ctx->gl_charset :
					    ctx->gr_charset ),
				    (XmStringDirection)
					((_CurDir(ctx) == ct_Dir_LeftToRight) ?
					    XmSTRING_DIRECTION_L_TO_R :
					    XmSTRING_DIRECTION_R_TO_L ),
				    False );

	if (separator)
	    {
	    if (ctx->xmsep == NULL)
		{
		ctx->xmsep = XmStringSeparatorCreate();
		};
	    savedxm = ctx->xmstring;
	    ctx->xmstring = XmStringConcat(ctx->xmstring, ctx->xmsep);
	    XmStringFree(savedxm);		/* free original xmstring */
	    };
	}; /* end if paired */
    return;
}

static XmString concatStringToXmString (compoundstring, textstring,
					charset, direction, separator)
    XmString		compoundstring;
    char *		textstring;
    char *		charset;
    XmStringDirection	direction;
    Boolean		separator;
{
    XmString	tempxm1, tempxm2;

    tempxm1 = XmStringSegmentCreate (   textstring,
					charset,
					direction,
					separator );
    tempxm2 = compoundstring;
    compoundstring = XmStringConcat(compoundstring, tempxm1);
    XmStringFree(tempxm1);			/* free xm version */
    XmStringFree(tempxm2);			/* free original xm string */
    return (compoundstring);
}


/* processESC - handle valid ESC sequences */

static Boolean processESC(ctx, final)
    ct_context	    *ctx;
    Octet	    final;
{
    Boolean	    ok;

    switch (ctx->item[1]) {
    case 0x24:			/* 02/04 - invoke 94(n) charset into GL or GR */
	ok = process94n(ctx, final);
	break;
    case 0x25:			/* 02/05 - extended segments */
	/* if we have any text to output, do it */
	if (ctx->flags.text) {
	    outputXmString(ctx, False);	/* with no separator */
	    ctx->flags.text = False;
	}
	ok = processExtendedSegments(ctx, final);
	break;
    case 0x28:			/* 02/08 - invoke 94 charset into GL */
	ok = process94GL(ctx, final);
	break;
    case 0x29:			/* 02/09 - invoke 94 charset into GR */
	ok = process94GR(ctx, final);
	break;
    case 0x2d:			/* 02/13 - invoke 96 charset into GR */
	ok =  process96GR(ctx, final);
	break;
    default:
	ok = False;
	break;
    }
    return(ok);
}


/*
**  processCSI - handle valid CSI sequences
**	CSI sequences
**	09/11 03/01 05/13   begin left-to-right text
**	09/11 03/02 05/13   begin right-to-left text
**	09/11 05/13	    end of string
**	09/11 P I F	    reserved for use in future extensions
*/

static Boolean processCSI(ctx, final)
    ct_context	    *ctx;
    Octet	    final;
{
    Boolean	    ok = True;

    switch (final) {
    case 0x5d:				/* end of direction sequence */
	switch (ctx->item[1]) {
	case 0x31:			/* start left to right */
	    if (ctx->flags.gchar && ctx->dirsp == 0) {
		ok = False;
	    } else {
		_PushDir(ctx, ct_Dir_LeftToRight);
	    }
	    break;
	case 0x32:			/* start right to left */
	    if (ctx->flags.gchar && ctx->dirsp == 0) {
		ok = False;
	    } else {
		_PushDir(ctx, ct_Dir_RightToLeft);
	    }
	    break;
	case 0x5d:			/* Just CSI EOS - revert */
	    if (ctx->dirsp > 0) {
		_PopDir(ctx);
		
	    } else {
		ok = False;
	    }
	    break;
	default:			/* anything else is an error */
	    ok = False;
	}
	break;

    default:				/* reserved for future extensions */
	ok = False;
	break;
    }
    return(ok);
}



static Boolean processExtendedSegments(ctx, final)
    ct_context	    *ctx;
    Octet	    final;
{
    OctetPtr	    esptr;			/* ptr into ext seg */
    unsigned int    seglen;			/* length of ext seg */
    unsigned int    len;			/* length */
    String	    charset_copy;		/* ptr to NULL-terminated copy of ext seg charset */
    OctetPtr	    text_copy;			/* ptr to NULL-terminated copy of ext seg text */
    XmString	    tempxm1, tempxm2;
    Boolean	    ok = True;

    /* Extended segments
    **  01/11 02/05 02/15 03/00 M L	    variable # of octets/char
    **  01/11 02/05 02/15 03/01 M L	    1 octet/char
    **  01/11 02/05 02/15 03/02 M L	    2 octets/char
    **  01/11 02/05 02/15 03/03 M L	    3 octets/char
    **  01/11 02/05 02/15 03/04 M L	    4 octets/char
    */
    if (	(ctx->itemlen == 4)
	&&	(ctx->item[2] == 0x2f)
	&&	(_IsInColumn3(final))
       ) {
	if (    ((ctx->lastoctet - ctx->octet) < 2)
	    ||  (ctx->octet[0] < 0x80)
	    ||  (ctx->octet[1] < 0x80)
	   ) {
	    return(False);
	}

	/*
	** The most significant bit of M and L are always set to 1
	** The number is computed as ((M-128)*128)+(L-128)
	*/
	seglen = *ctx->octet - 0x80;
	ctx->octet++; ctx->itemlen++;		/* advance pointer */
	seglen = (seglen << 7) + (*ctx->octet - 0x80);
	ctx->octet++; ctx->itemlen++;		/* advance pointer */
	if ((ctx->lastoctet - ctx->octet) < seglen) {
	    return(False);
	}
	esptr = ctx->octet;			/* point to charset */
	ctx->itemlen += seglen;			/* advance pointer over segment */
	ctx->octet += seglen;

	switch (final) {
	case 0x30:				/* variable # of octets per char */
	case 0x31:				/* 1 octet per char */
	case 0x32:				/* 2 octets per char */
	    /* scan for STX separator between charset and text */
	    len = 0;
	    while (esptr[len] != STX)
		len++;
	    if (len > ctx->itemlen) {		/* if we ran off the end, error */
		ok = False;
		break;
	    }
	    charset_copy = XtMalloc(len + 1);
	    strncpy(charset_copy, esptr, len);
	    charset_copy[len] = EOS;
	    esptr += len + 1;			/* point to text part */
	    len = seglen - len - 1;		/* calc length of text part */
	    text_copy = (unsigned char *) XtMalloc(len + 1);
	    bcopy (esptr, text_copy, len);
	    text_copy[len] = EOS;
	    tempxm1 = 
	      XmStringSegmentCreate((char *) text_copy,
				    charset_copy,
				    (unsigned char)(_CurDir(ctx) == 
						    ct_Dir_LeftToRight ?
						    XmSTRING_DIRECTION_L_TO_R :
						    XmSTRING_DIRECTION_R_TO_L),
				    False );
	    tempxm2 = ctx->xmstring;
	    ctx->xmstring = XmStringConcat(ctx->xmstring, tempxm1);
	    XtFree(text_copy);
	    XtFree(charset_copy);
	    XmStringFree(tempxm1);		/* free xm string */
	    XmStringFree(tempxm2);		/* free original xm string */
	    ok = True;
	    break;
	    
	case 0x33:				/* 3 octets per char */
	case 0x34:				/* 4 octets per char */
	    /* not supported */
	    ok = False;
	    break;

	default:
	    /* reserved for future use */
	    ok = False;
	    break;
	} /* end switch */
    } /* end if */

    return(ok);
}


static Boolean process94n(ctx, final)
    ct_context	    *ctx;
    Octet	    final;
{
    if (ctx->itemlen > 3) {
	switch (ctx->item[2]) {
	case 0x28:				/* into GL */
	    switch (final) {
	    case 0x41:				/* 04/01 - China (PRC) Hanzi */
		_SetGL(ctx, CS_GB2312_0, 94, 2);
		break;
	    case 0x42:				/* 04/02 - Japanese GCS, level 2 */
		_SetGL(ctx, CS_JISX0208_0, 94, 2);
		break;
	    case 0x43:				/* 04/03 - Korean GCS */
		_SetGL(ctx, CS_KSC5601_0, 94, 2);
		break;
	    default:
		/* other character sets are not supported */
		return False;
		break;
	    } /* end switch (final) */
	    break;

	case 0x29:				/* into GR */
	    switch (final) {
	    case 0x41:				/* 04/01 - China (PRC) Hanzi */
		_SetGR(ctx, CS_GB2312_1, 94, 2);
		break;
	    case 0x42:				/* 04/02 - Japanese GCS, level 2 */
		_SetGR(ctx, CS_JISX0208_1, 94, 2);
		break;
	    case 0x43:				/* 04/03 - Korean GCS */
		_SetGR(ctx, CS_KSC5601_1, 94, 2);
		break;
	    default:
		/* other character sets are not supported */
		return False;
		break;
	    } /* end switch (final) */
	    break;

	default:
	    /* error */
	    return False;
	    break;
	} /* end switch item[2] */
    }
    else {
	/* error */
	return False;
    } /* end if */
    return True;
}



static Boolean process94GL(ctx, final)
    ct_context	    *ctx;
    Octet	    final;
{
    switch (final) {
    case 0x42:				/* 04/02 - Left half, ISO8859* (ASCII) */
	_SetGL(ctx, CS_ISO8859_1,  94, 1);
	break;
    case 0x4a:				/* 04/10 - Left half, Katakana */
	_SetGL(ctx, CS_JISX0201, 94, 1);
	break;
    default:
	return False;
    }

    return(True);
}


static Boolean process94GR(ctx, final)
    ct_context	    *ctx;
    Octet	    final;
{
    switch (final) {
    case 0x49:				/* 04/09 - Right half, Katakana */
	_SetGR(ctx, CS_JISX0201, 94, 1);
	break;
    default:
	return False;
    }

    return(True);
}



static Boolean process96GR(ctx, final)
    ct_context	    *ctx;
    Octet	    final;
{
    switch (final) {
    case 0x41:				/* 04/01 - Right half, Latin 1 */
	_SetGR(ctx, CS_ISO8859_1, 96, 1);
	break;
    case 0x42:				/* 04/02 - Right half, Latin 2 */
	_SetGR(ctx, CS_ISO8859_2, 96, 1);
	break;
    case 0x43:				/* 04/03 - Right half, Latin 3 */
	_SetGR(ctx, CS_ISO8859_3, 96, 1);
	break;
    case 0x44:				/* 04/04 - Right half, Latin 4 */
	_SetGR(ctx, CS_ISO8859_4, 96, 1);
	break;
    case 0x46:				/* 04/06 - Right half, Latin/Greek */
	_SetGR(ctx, CS_ISO8859_7, 96, 1);
	break;
    case 0x47:				/* 04/07 - Right half, Latin/Arabic */
	_SetGR(ctx, CS_ISO8859_6, 96, 1);
	break;
    case 0x48:				/* 04/08 - Right half, Latin/Hebrew */
	_SetGR(ctx, CS_ISO8859_8, 96, 1);
	break;
    case 0x4c:				/* 04/12 - Right half, Latin/Cyrillic */
	_SetGR(ctx, CS_ISO8859_5, 96, 1);
	break;
    case 0x4d:				/* 04/13 - Right half, Latin 5 */
	_SetGR(ctx, CS_ISO8859_9, 96, 1);
	break;
    default:
	return False;
    }

    return(True);
}


/************************************************************************
 *
 *  XmCvtXmStringToCT
 *	Convert an XmString to a compound text string directly.
 *	This is the public version of the resource converter and only
 *	requires the XmString as an argument.
 *
 ************************************************************************/

#ifdef _NO_PROTO
char *XmCvtXmStringToCT (string)
       XmString	string;
#else
char *XmCvtXmStringToCT (XmString string)
#endif
{
  Boolean	ok;
  /* Dummy up some XrmValues to pass to cvtXmStringToText. */
  XrmValue	from_val;
  XrmValue	to_val;
  
  if (string == NULL)
    return ( (char *) NULL );
  
  from_val.addr = (char *)string;
  
  ok = cvtXmStringToText(&from_val, &to_val);
  
  if (!ok)
    {
	XtAppWarningMsg(
			_XtDefaultAppContext(),
			"conversionError","compoundText", "XtToolkitError",
			"Cannot convert XmString to type compound text string",
			(String *)NULL, (Cardinal *)NULL);
	return ( (char *) NULL );
    }

  return ( (char *) to_val.addr );
}

/************************************************************************
 *
 *  XmCvtXmStringToText
 *	Convert an XmString to an ASCII string.
 *
 ************************************************************************/

Boolean XmCvtXmStringToText (display, args, num_args, from_val, to_val, converter_data)
    Display	*display;
    XrmValuePtr	args;
    Cardinal	*num_args;
    XrmValue	*from_val;
    XrmValue	*to_val;
    XtPointer	*converter_data;
{
    Boolean		ok;

    if (from_val->addr == NULL)
	return(False);

    ok = cvtXmStringToText(from_val, to_val);

    if (!ok)
    {
	to_val->addr = NULL;
	to_val->size = 0;
	XtAppWarningMsg(
			XtDisplayToApplicationContext(display),
			"conversionError","compoundText", "XtToolkitError",
			"Cannot convert XmString to type compound text string",
			(String *)NULL, (Cardinal *)NULL);
    }
    return(ok);
}



static Boolean cvtXmStringToText (from, to)
    XrmValue	    *from;
    XrmValue	    *to;
{
    Boolean			ok = True;
    OctetPtr			outc = NULL;
    unsigned int		outlen = 0;
    OctetPtr			ctext = NULL;
    XmStringContext		context = NULL;
    XmStringCharSet		charset = NULL;
    XmStringDirection		direction;
    Boolean			separator;
    ct_Direction		prev_direction = ct_Dir_Undefined;
    ct_Charset			prev_charset = cs_none;
    unsigned int		len;
    unsigned int		ctlen;

    ok = XmStringInitContext(&context, (XmString)from->addr);
    if (!ok) return(False);

    while (ok = XmStringGetNextSegment (context, (char **) &ctext, &charset,
					&direction, &separator)) {

	/* First output the direction, if changed */
	if (direction == XmSTRING_DIRECTION_L_TO_R) {
	    if (prev_direction != ct_Dir_LeftToRight) {
		outc = ctextConcat(outc, outlen, 
                         (UChar *) CTEXT_L_TO_R, (unsigned int)CTEXT_L_TO_R_LEN);
		outlen += CTEXT_L_TO_R_LEN;
		prev_direction = ct_Dir_LeftToRight;
	    }
	}
	else {
	    if (prev_direction != ct_Dir_RightToLeft) {
		outc = ctextConcat(outc, outlen, 
                         (UChar *) CTEXT_R_TO_L, (unsigned int)CTEXT_R_TO_L_LEN);
		outlen += CTEXT_R_TO_L_LEN;
		prev_direction = ct_Dir_RightToLeft;
	    }
	};

	ctlen = strlen(ctext);			/* get length of text */

	/* Next output the charset */
	if (strcmp(charset, CS_ISO8859_1) == 0) {
	    if (prev_charset != cs_Latin1) {
		outc = ctextConcat(outc, outlen, 
                      (UChar *)CTEXT_SET_ISO8859_1, 
                      (unsigned int)CTEXT_SET_ISO8859_1_LEN);
		outlen += CTEXT_SET_ISO8859_1_LEN;
		prev_charset = cs_Latin1;
	    };
	}
	else
	if (strcmp(charset, CS_ISO8859_2) == 0) {
	    if (prev_charset != cs_Latin2) {
		outc = ctextConcat(outc, outlen, 
                    (UChar *)CTEXT_SET_ISO8859_2, 
                    (unsigned int)CTEXT_SET_ISO8859_2_LEN);
		outlen += CTEXT_SET_ISO8859_2_LEN;
		prev_charset = cs_Latin2;
	    };
	}
	else
	if (strcmp(charset, CS_ISO8859_3) == 0) {
	    if (prev_charset != cs_Latin3) {
		outc = ctextConcat(outc, outlen, 
                    (UChar *)CTEXT_SET_ISO8859_3, 
                    (unsigned int)CTEXT_SET_ISO8859_3_LEN);
		outlen += CTEXT_SET_ISO8859_3_LEN;
		prev_charset = cs_Latin3;
	    };
	}
	else
	if (strcmp(charset, CS_ISO8859_4) == 0) {
	    if (prev_charset != cs_Latin4) {
		outc = ctextConcat(outc, outlen, 
                    (UChar *)CTEXT_SET_ISO8859_4, 
                    (unsigned int)CTEXT_SET_ISO8859_4_LEN);
		outlen += CTEXT_SET_ISO8859_4_LEN;
		prev_charset = cs_Latin4;
	    };
	}
	else
	if (strcmp(charset, CS_ISO8859_5) == 0) {
	    if (prev_charset != cs_LatinCyrillic) {
		outc = ctextConcat(outc, outlen, 
                    (UChar *)CTEXT_SET_ISO8859_5, 
                    (unsigned int)CTEXT_SET_ISO8859_5_LEN);
		outlen += CTEXT_SET_ISO8859_5_LEN;
		prev_charset = cs_LatinCyrillic;
	    };
	}
	else
	if (strcmp(charset, CS_ISO8859_6) == 0) {
	    if (prev_charset != cs_LatinArabic) {
		outc = ctextConcat(outc, outlen, 
                    (UChar *)CTEXT_SET_ISO8859_6, 
                    (unsigned int)CTEXT_SET_ISO8859_6_LEN);
		outlen += CTEXT_SET_ISO8859_6_LEN;
		prev_charset = cs_LatinArabic;
	    };
	}
	else
	if (strcmp(charset, CS_ISO8859_7) == 0) {
	    if (prev_charset != cs_LatinGreek) {
		outc = ctextConcat(outc, outlen, 
                    (UChar *)CTEXT_SET_ISO8859_7, 
                    (unsigned int)CTEXT_SET_ISO8859_7_LEN);
		outlen += CTEXT_SET_ISO8859_7_LEN;
		prev_charset = cs_LatinGreek;
	    };
	}
	else
	if (strcmp(charset, CS_ISO8859_8) == 0) {
	    if (prev_charset != cs_LatinHebrew) {
		outc = ctextConcat(outc, outlen, 
                    (UChar *)CTEXT_SET_ISO8859_8, 
                    (unsigned int)CTEXT_SET_ISO8859_8_LEN);
		outlen += CTEXT_SET_ISO8859_8_LEN;
		prev_charset = cs_LatinHebrew;
	    };
	}
	else
	if (strcmp(charset, CS_ISO8859_9) == 0) {
	    if (prev_charset != cs_Latin5) {
		outc = ctextConcat(outc, outlen, 
                    (UChar *)CTEXT_SET_ISO8859_9, 
                    (unsigned int)CTEXT_SET_ISO8859_9_LEN);
		outlen += CTEXT_SET_ISO8859_9_LEN;
		prev_charset = cs_Latin5;
	    };
	}
	else
	if (strcmp(charset, CS_JISX0201) == 0) {
	    if (prev_charset != cs_Katakana) {
		outc = ctextConcat(outc, outlen, 
                    (UChar *)CTEXT_SET_JISX0201, 
                    (unsigned int)CTEXT_SET_JISX0201_LEN);
		outlen += CTEXT_SET_JISX0201_LEN;
		prev_charset = cs_Katakana;
	    };
	}
	else
	if (	(strcmp(charset, CS_GB2312_0) == 0)
	    ||	(strcmp(charset, CS_GB2312_1) == 0)	) {
	    if (prev_charset != cs_Hanzi) {
		outc = ctextConcat(outc, outlen, 
                    (UChar *)CTEXT_SET_GB2312_0, 
                    (unsigned int)CTEXT_SET_GB2312_0_LEN);
		outlen += CTEXT_SET_GB2312_0_LEN;
		prev_charset = cs_Hanzi;
	    };
	}
	else
	if (	(strcmp(charset, CS_JISX0208_0) == 0)
	    ||	(strcmp(charset, CS_JISX0208_1) == 0)	) {
	    if (prev_charset != cs_JapaneseGCS) {
		outc = ctextConcat(outc, outlen, 
                    (UChar *)CTEXT_SET_JISX0208_0, 
                    (unsigned int)CTEXT_SET_JISX0208_0_LEN);
		outlen += CTEXT_SET_JISX0208_0_LEN;
		prev_charset = cs_JapaneseGCS;
	    };
	}
	else
	if (	(strcmp(charset, CS_KSC5601_0) == 0)
	    ||	(strcmp(charset, CS_KSC5601_1) == 0)	) {
	    if (prev_charset != cs_KoreanGCS) {
		outc = ctextConcat(outc, outlen, 
                    (UChar *)CTEXT_SET_KSC5601_0, 
                    (unsigned int)CTEXT_SET_KSC5601_0_LEN);
		outlen += CTEXT_SET_KSC5601_0_LEN;
		prev_charset = cs_KoreanGCS;
	    };
	}
	else {
	    /* oh s**t - must be a non-standard character set! */
	    OctetPtr	temp;

	    len = strlen(charset);
	    temp = (unsigned char *) XtMalloc(outlen + 6 + len + 2);
				/* orig + header + charset + STX + EOS */
	    bcopy(outc, temp, outlen);
	    XtFree(outc);
	    outc = temp;
	    temp = &(outc[outlen]);
	    /*
	    ** Format is:
	    **	01/11 02/05 02/15 03/nn M L charset 00/02 text
	    */
	    *temp++ = 0x1b;
	    *temp++ = 0x25;
	    *temp++ = 0x2f;
	    /*
	    ** HACK!  The next octet in the sequence is the # of octets/char.
	    ** XmStrings don't have this information, so just set it to be
	    ** variable # of octets/char, and hope the caller knows what to do.
	    */
	    *temp++ = 0x30;
	    *temp++ = 0x80 + (len+ctlen+1)/128;	    /* encode len in next 2 octets */
	    *temp++ = 0x80 + (len+ctlen+1)%128;
	    strcpy(temp, charset);
	    temp += len;
	    *temp++ = STX;
	    *temp = EOS;			    /* make sure there's a \0 on the end */
	    prev_charset = cs_NonStandard;
	    outlen += 6 + len + 1;
	};
	XtFree(charset);

	/* Now copy in the text */
	if (ctlen > 0) {
	    outc = ctextConcat(outc, outlen, ctext, ctlen);
	    outlen += ctlen;
	};

	/* Finally, add the separator if any */
	if (separator) {
	    outc = ctextConcat(outc, outlen, 
                (UChar *)NEWLINESTRING, (unsigned int)NEWLINESTRING_LEN);
	    outlen++;
	};

    }; /* end while */

    if (outc != NULL) {
	to->addr = (char *) outc;
	to->size = outlen;
    };

    XmStringFreeContext(context);

    return(True);
} /* end of cvtTextToXmString */



static OctetPtr ctextConcat(str1, str1len, str2, str2len)
	OctetPtr	str1;
	unsigned int	str1len;
	OctetPtr	str2;
	unsigned int	str2len;
{

	str1 = (OctetPtr)XtRealloc((void *)str1, (str1len + str2len + 1));
	bcopy(str2, &str1[str1len], str2len);
	str1[str1len+str2len] = EOS;
	return(str1);
}
