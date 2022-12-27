#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)Visual.c	3.24.1.2 91/03/14";
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
#include <X11/ShellP.h>

/* Warning and Error messages */

#define MESSAGE1 "Invalid color requested from _XmAccessColorData"

/*  Defines and functions for processing dynamic defaults  */


#define XmMAX_SHORT 65535


/* Contributions of each primary to overall luminosity, sum to 1.0 */

#define XmRED_LUMINOSITY 0.30
#define XmGREEN_LUMINOSITY 0.59
#define XmBLUE_LUMINOSITY 0.11

/* Percent effect of intensity, light, and luminosity & on brightness,
   sum to 100 */

#define XmINTENSITY_FACTOR  25
#define XmLIGHT_FACTOR       0
#define XmLUMINOSITY_FACTOR 75

/* Thresholds for brightness
   above LITE threshold, LITE color model is used
   below DARK threshold, DARK color model is be used
   use STD color model in between */

#define XmCOLOR_LITE_THRESHOLD (0.77 * XmMAX_SHORT)

#define XmFOREGROUND_THRESHOLD (0.35 * XmMAX_SHORT)

#define XmCOLOR_DARK_THRESHOLD (0.15 * XmMAX_SHORT)

/* LITE color model
   percent to interpolate RGB towards black for SEL, BS, TS */

#define XmCOLOR_LITE_SEL_FACTOR   15
#define XmCOLOR_LITE_BS_FACTOR   45
#define XmCOLOR_LITE_TS_FACTOR   20

/* DARK color model
   percent to interpolate RGB towards white for SEL, BS, TS */

#define XmCOLOR_DARK_SEL_FACTOR   15
#define XmCOLOR_DARK_BS_FACTOR   30
#define XmCOLOR_DARK_TS_FACTOR   50

/* STD color model
   percent to interpolate RGB towards black for SEL, BS
   percent to interpolate RGB towards white for TS
   HI values used for high brightness (within STD)
   LO values used for low brightness (within STD)
   Interpolate factors between HI & LO values based on brightness */

#define XmCOLOR_HI_SEL_FACTOR   15
#define XmCOLOR_HI_BS_FACTOR   35
#define XmCOLOR_HI_TS_FACTOR   70

#define XmCOLOR_LO_SEL_FACTOR   15
#define XmCOLOR_LO_BS_FACTOR    60
#define XmCOLOR_LO_TS_FACTOR   40

/*  Forward declarations for the dynamic default function  */

static void _XmGetDynamicDefault();

/*  Name set by background pixmap converter, used and cleared  */
/*  Primitive and Manager Initialize procedure.                */

static char * background_pixmap_name = NULL;


/*  Foreward declarations for the primitive and manager pixmap  */
/*  resource converter functions.                               */

static void _XmCvtStringToBackgroundPixmap();
static void _XmCvtStringToPrimForegroundPixmap();
static void _XmCvtStringToPrimHighlightPixmap();
static void _XmCvtStringToPrimTopShadowPixmap();
static void _XmCvtStringToPrimBottomShadowPixmap();
static void _XmCvtStringToManForegroundPixmap();
static void _XmCvtStringToManHighlightPixmap();
static void _XmCvtStringToManTopShadowPixmap();
static void _XmCvtStringToManBottomShadowPixmap();
static void _XmCvtStringToGadgetPixmap();

#ifdef _NO_PROTO
static int _XmBrightness() ;
#else /* _NO_PROTO */
static int _XmBrightness(XColor *color);
#endif /* _NO_PROTO */

/*  Argument lists sent down to the pixmap converter functions  */

static XtConvertArgRec backgroundArgs[] =
{
   { XtBaseOffset, (caddr_t) 0, sizeof (int) },
};

static XtConvertArgRec primForegroundArgs[] =
{
   { XtBaseOffset, (caddr_t) XtOffset(Widget, core.screen), sizeof (Screen*) },
   { XtBaseOffset, (caddr_t) XtOffset(XmPrimitiveWidget,
primitive.foreground), sizeof (Pixel) },
   { XtBaseOffset, (caddr_t) XtOffset(Widget, core.background_pixel),
sizeof (Pixel) }
};

static XtConvertArgRec primHighlightArgs[] =
{
   { XtBaseOffset, (caddr_t) XtOffset(Widget, core.screen), sizeof (Screen*) },
   { XtBaseOffset, (caddr_t) XtOffset(XmPrimitiveWidget,
primitive.highlight_color), sizeof (Pixel) },
   { XtBaseOffset, (caddr_t) XtOffset(Widget, core.background_pixel),
sizeof (Pixel) }
};

static XtConvertArgRec primTopShadowArgs[] =
{
   { XtBaseOffset, (caddr_t) XtOffset(Widget, core.screen), sizeof (Screen*) },
   { XtBaseOffset, (caddr_t) XtOffset (XmPrimitiveWidget,
primitive.top_shadow_color), sizeof (Pixel) },
   { XtBaseOffset, (caddr_t) XtOffset (Widget, core.background_pixel),
sizeof (Pixel) }
};

static XtConvertArgRec primBottomShadowArgs[] =
{
   { XtBaseOffset, (caddr_t) XtOffset(Widget, core.screen), sizeof (Screen*) },
   { XtBaseOffset, (caddr_t) XtOffset (XmPrimitiveWidget,
primitive.bottom_shadow_color), sizeof (Pixel) },
   { XtBaseOffset, (caddr_t) XtOffset (Widget, core.background_pixel),
sizeof (Pixel) }
};

static XtConvertArgRec manForegroundArgs[] =
{
   { XtWidgetBaseOffset, (caddr_t) XtOffset(Widget, core.screen),
sizeof (Screen*) },
   { XtWidgetBaseOffset, (caddr_t) XtOffset(XmManagerWidget,
manager.foreground), sizeof (Pixel) },
   { XtWidgetBaseOffset, (caddr_t) XtOffset(Widget,
core.background_pixel), sizeof (Pixel) }
};

static XtConvertArgRec manHighlightArgs[] =
{
   { XtWidgetBaseOffset, (caddr_t) XtOffset(Widget, core.screen),
sizeof (Screen*) },
   { XtWidgetBaseOffset, (caddr_t) XtOffset(XmManagerWidget,
manager.highlight_color), sizeof (Pixel) },
   { XtWidgetBaseOffset, (caddr_t) XtOffset(Widget,
core.background_pixel), sizeof (Pixel) }
};

static XtConvertArgRec manTopShadowArgs[] =
{
   { XtWidgetBaseOffset, (caddr_t)XtOffset(Widget, core.screen), sizeof (Screen*) },
   { XtWidgetBaseOffset, (caddr_t) XtOffset (XmManagerWidget,
manager.top_shadow_color), sizeof (Pixel) },
   { XtWidgetBaseOffset, (caddr_t) XtOffset (Widget,
core.background_pixel), sizeof (Pixel) }
};

static XtConvertArgRec manBottomShadowArgs[] =
{
   { XtWidgetBaseOffset, (caddr_t) XtOffset(Widget, core.screen),
sizeof (Screen*) },
   { XtWidgetBaseOffset, (caddr_t) XtOffset (XmManagerWidget,
manager.bottom_shadow_color), sizeof (Pixel) },
   { XtWidgetBaseOffset, (caddr_t) XtOffset (Widget,
core.background_pixel), sizeof (Pixel) }
};

static XtConvertArgRec gadgetPixmapArgs[] =
{
   { XtWidgetBaseOffset, (caddr_t) XtOffset(Widget, core.parent),
sizeof (Widget ) }
};


/************************************************************************
 *
 *  _XmRegisterPixmapConverters
 *	Register the pixmap converters used exclusively by primitive
 *
 ************************************************************************/

#ifdef _NO_PROTO
void _XmRegisterPixmapConverters()

#else /* _NO_PROTO */
void _XmRegisterPixmapConverters (void)
#endif /* _NO_PROTO */
{
   static Boolean inited = False;


   if (inited == False)
   {
	  inited = True;

      XtAddConverter (XmRString, XmRXmBackgroundPixmap,
                      _XmCvtStringToBackgroundPixmap, 
                      backgroundArgs, XtNumber(backgroundArgs));

      XtAddConverter (XmRString, XmRPrimForegroundPixmap,
                      _XmCvtStringToPrimForegroundPixmap, 
                      primForegroundArgs, XtNumber(primForegroundArgs));

      XtAddConverter (XmRString, XmRPrimHighlightPixmap,
                      _XmCvtStringToPrimHighlightPixmap, 
                      primHighlightArgs, XtNumber(primHighlightArgs));

      XtAddConverter (XmRString, XmRPrimTopShadowPixmap,
                      _XmCvtStringToPrimTopShadowPixmap, 
                      primTopShadowArgs, XtNumber(primTopShadowArgs));

      XtAddConverter (XmRString, XmRPrimBottomShadowPixmap,
                      _XmCvtStringToPrimBottomShadowPixmap, 
                      primBottomShadowArgs, XtNumber(primBottomShadowArgs));

      XtAddConverter (XmRString, XmRManForegroundPixmap,
                      _XmCvtStringToManForegroundPixmap, 
                      manForegroundArgs, XtNumber(manForegroundArgs));

      XtAddConverter (XmRString, XmRManHighlightPixmap,
                      _XmCvtStringToManHighlightPixmap, 
                      manHighlightArgs, XtNumber(manHighlightArgs));

      XtAddConverter (XmRString, XmRManTopShadowPixmap,
                      _XmCvtStringToManTopShadowPixmap, 
                      manTopShadowArgs, XtNumber(manTopShadowArgs));

      XtAddConverter (XmRString, XmRManBottomShadowPixmap,
                      _XmCvtStringToManBottomShadowPixmap, 
                      manBottomShadowArgs, XtNumber(manBottomShadowArgs));

      XtAddConverter (XmRString, XmRGadgetPixmap,
                      _XmCvtStringToGadgetPixmap, 
                      gadgetPixmapArgs, XtNumber(gadgetPixmapArgs));
   }
}




/************************************************************************
 *
 *  _XmGetBGPixmapName
 *	Return the background pixmap name set by the string to background
 *	resource converter.  This is used by primitive and manager.
 *
 ************************************************************************/

char * _XmGetBGPixmapName()

{
   return (background_pixmap_name);
}




/************************************************************************
 *
 *  _XmClearBGPixmapName
 *	Clear the background pixmap name set by the string to background
 *	resource converter.  This is used by primitive and manager.
 *
 ************************************************************************/

#ifdef _NO_PROTO
void _XmClearBGPixmapName()

#else /* _NO_PROTO */
void _XmClearBGPixmapName (void)
#endif /* _NO_PROTO */
{
   background_pixmap_name = NULL;
}



/************************************************************************
 *
 *  Primitive, gadget and manager resource converters
 *
 ************************************************************************/

/************************************************************************
 *
 *  _XmCvtStringToBackgroundPixmap()
 *	Convert a string to a background pixmap.  Used by both
 *	primitive and manager.
 *
 ************************************************************************/

static void _XmCvtStringToBackgroundPixmap (args, numArgs, fromVal, toVal)
XrmValue * args;
Cardinal   numArgs;
XrmValue * fromVal;
XrmValue * toVal;

{
   static Pixmap pixmap;
   Widget widget;

   widget = *((Widget *) args[0].addr);

   if (!XtIsShell(widget))
      background_pixmap_name = (char *) (fromVal->addr);


   /*  Always send back a NULL pixmap  */

   pixmap = XmUNSPECIFIED_PIXMAP;
   (*toVal).size = sizeof (Pixmap);
   (*toVal).addr = (caddr_t) &pixmap;
}



/************************************************************************
 *
 *  CvtStringToPixmap
 *	Convert a string to a pixmap.  This is the general conversion
 *	routine referenced by many of the specific string to pixmap
 *	converter functions.
 *
 ************************************************************************/

static void CvtStringToPixmap (args, numArgs, fromVal, toVal)
XrmValue * args;
Cardinal   numArgs;
XrmValue * fromVal;
XrmValue * toVal;

{
   static Pixmap pixmap;
   char   * image_name = (char *) (fromVal->addr);
   Screen * screen;
   Pixel    foreground;
   Pixel    background;

   if (_XmStringsAreEqual(image_name, "unspecified_pixmap"))
   {
       pixmap = XmUNSPECIFIED_PIXMAP;
   }
   else
   {
       screen = *((Screen **) args[0].addr);
       foreground = *((Pixel *) args[1].addr);
       background = *((Pixel *) args[2].addr);
       pixmap = XmGetPixmap (screen, image_name, foreground, background);
   }
   (*toVal).size = sizeof (Pixmap);
   (*toVal).addr = (caddr_t) &pixmap;
}

/************************************************************************
 *
 *  _XmCvtStringToGadgetPixmap
 *	Convert a string to a pixmap as used by a gadget.
 *
 ************************************************************************/

static void _XmCvtStringToGadgetPixmap (args, numArgs, fromVal, toVal)
XrmValue * args;
Cardinal   numArgs;
XrmValue * fromVal;
XrmValue * toVal;

{
   static Pixmap pixmap;
   char   * image_name = (char *) (fromVal->addr);
   XmManagerWidget mom;
   Screen * screen;
   Pixel    foreground;
   Pixel    background;

   if (_XmStringsAreEqual(image_name, "unspecified_pixmap"))
   {
       pixmap = XmUNSPECIFIED_PIXMAP;
   }
   else
   {
       mom = *((XmManagerWidget *) args[0].addr);
       screen = XtScreen(mom);
       foreground = mom->manager.foreground;
       background = mom->core.background_pixel;
       pixmap = XmGetPixmap (screen, image_name, foreground, background);
   }
   (*toVal).size = sizeof (Pixmap);
   (*toVal).addr = (caddr_t) &pixmap;
}




/************************************************************************
 *
 *  The following set of converter functions call the general converter
 *  to do all of the work.
 *
 ************************************************************************/

static void _XmCvtStringToPrimForegroundPixmap (args, numArgs, fromVal, toVal)
XrmValue * args;
Cardinal   numArgs;
XrmValue * fromVal;
XrmValue * toVal;

{
   CvtStringToPixmap (args, numArgs, fromVal, toVal);
}


static void _XmCvtStringToPrimHighlightPixmap (args, numArgs, fromVal, toVal)
XrmValue * args;
Cardinal   numArgs;
XrmValue * fromVal;
XrmValue * toVal;

{
   CvtStringToPixmap (args, numArgs, fromVal, toVal);
}


static void _XmCvtStringToPrimTopShadowPixmap (args, numArgs, fromVal, toVal)
XrmValue * args;
Cardinal   numArgs;
XrmValue * fromVal;
XrmValue * toVal;

{
   CvtStringToPixmap (args, numArgs, fromVal, toVal);
}


static void 
_XmCvtStringToPrimBottomShadowPixmap (args, numArgs, fromVal, toVal)
XrmValue * args;
Cardinal   numArgs;
XrmValue * fromVal;
XrmValue * toVal;

{
   CvtStringToPixmap (args, numArgs, fromVal, toVal);
}


static void _XmCvtStringToManForegroundPixmap (args, numArgs, fromVal, toVal)
XrmValue * args;
Cardinal   numArgs;
XrmValue * fromVal;
XrmValue * toVal;

{
   CvtStringToPixmap (args, numArgs, fromVal, toVal);
}


static void _XmCvtStringToManHighlightPixmap (args, numArgs, fromVal, toVal)
XrmValue * args;
Cardinal   numArgs;
XrmValue * fromVal;
XrmValue * toVal;

{
   CvtStringToPixmap (args, numArgs, fromVal, toVal);
}


static void _XmCvtStringToManTopShadowPixmap (args, numArgs, fromVal, toVal)
XrmValue * args;
Cardinal   numArgs;
XrmValue * fromVal;
XrmValue * toVal;

{
   CvtStringToPixmap (args, numArgs, fromVal, toVal);
}


static void 
_XmCvtStringToManBottomShadowPixmap (args, numArgs, fromVal, toVal)
XrmValue * args;
Cardinal   numArgs;
XrmValue * fromVal;
XrmValue * toVal;

{
   CvtStringToPixmap (args, numArgs, fromVal, toVal);
}




/************************************************************************
 *
 *  Dynamic defaulting color and pixmap functions
 *
 ************************************************************************/

#ifdef _NO_PROTO
void _XmForegroundColorDefault (widget, offset, value)
Widget   widget;
int      offset;
XrmValue * value;

#else /* _NO_PROTO */
void _XmForegroundColorDefault (Widget widget, int offset, XrmValue *value)
#endif /* _NO_PROTO */
{
   _XmGetDynamicDefault (widget, XmFOREGROUND, offset, value);
}


#ifdef _NO_PROTO
void _XmBackgroundColorDefault (widget, offset, value)
Widget   widget;
int      offset;
XrmValue * value;

#else /* _NO_PROTO */
void _XmBackgroundColorDefault (Widget widget, int offset, XrmValue *value)
#endif /* _NO_PROTO */
{
   _XmGetDynamicDefault (widget, XmBACKGROUND, offset, value);
}


#ifdef _NO_PROTO
void _XmTopShadowColorDefault (widget, offset, value)
Widget   widget;
int      offset;
XrmValue * value;

#else /* _NO_PROTO */
void _XmTopShadowColorDefault (Widget widget, int offset, XrmValue *value)
#endif /* _NO_PROTO */
{
   _XmGetDynamicDefault (widget, XmTOP_SHADOW, offset, value);
}


#ifdef _NO_PROTO
void _XmBottomShadowColorDefault (widget, offset, value)
Widget   widget;
int      offset;
XrmValue * value;

#else /* _NO_PROTO */
void _XmBottomShadowColorDefault (Widget widget, int offset, XrmValue *value)
#endif /* _NO_PROTO */
{
   _XmGetDynamicDefault (widget, XmBOTTOM_SHADOW, offset, value);
}


#ifdef _NO_PROTO
void _XmSelectColorDefault (widget, offset, value)
Widget   widget;
int      offset;
XrmValue * value;

#else /* _NO_PROTO */
void _XmSelectColorDefault (Widget widget, int offset, XrmValue *value)
#endif /* _NO_PROTO */
{
   _XmGetDynamicDefault (widget, XmSELECT, offset, value);
}



#ifdef _NO_PROTO
void _XmPrimitiveTopShadowPixmapDefault (widget, offset, value)
Widget   widget;
int      offset;
XrmValue * value;

#else /* _NO_PROTO */
void _XmPrimitiveTopShadowPixmapDefault (Widget widget, int offset, XrmValue *value)
#endif /* _NO_PROTO */
{
   XmPrimitiveWidget pw = (XmPrimitiveWidget) widget;
   static Pixmap pixmap;

   pixmap = XmUNSPECIFIED_PIXMAP;

   value->addr = (char *) &pixmap;
   value->size = sizeof (Pixmap);

   if (pw->primitive.top_shadow_color == pw->core.background_pixel)
      pixmap = XmGetPixmap (XtScreen (pw), "50_foreground",
                            pw->primitive.top_shadow_color,
                            pw->primitive.foreground);

   else if (DefaultDepthOfScreen (XtScreen (widget)) == 1)
      pixmap = XmGetPixmap (XtScreen (pw), "50_foreground",
                            pw->primitive.top_shadow_color,
                            pw->core.background_pixel);
}



#ifdef _NO_PROTO
void _XmManagerTopShadowPixmapDefault (widget, offset, value)
Widget   widget;
int      offset;
XrmValue * value;

#else /* _NO_PROTO */
void _XmManagerTopShadowPixmapDefault (Widget widget, int offset, XrmValue *value)
#endif /* _NO_PROTO */
{
   XmManagerWidget mw = (XmManagerWidget) widget;
   static Pixmap pixmap;

   pixmap = XmUNSPECIFIED_PIXMAP;

   value->addr = (char *) &pixmap;
   value->size = sizeof (Pixmap);

   if (mw->manager.top_shadow_color == mw->core.background_pixel)
      pixmap = XmGetPixmap (XtScreen (mw), "50_foreground",
                            mw->manager.top_shadow_color,
                            mw->manager.foreground);

   else if (DefaultDepthOfScreen (XtScreen (widget)) == 1)
      pixmap = XmGetPixmap (XtScreen (mw), "50_foreground",
                            mw->manager.top_shadow_color,
                            mw->core.background_pixel);
}



#ifdef _NO_PROTO
void _XmPrimitiveHighlightPixmapDefault (widget, offset, value)
Widget   widget;
int      offset;
XrmValue * value;

#else /* _NO_PROTO */
void _XmPrimitiveHighlightPixmapDefault (Widget widget, int offset, XrmValue *value)
#endif /* _NO_PROTO */
{
   XmPrimitiveWidget pw = (XmPrimitiveWidget) widget;
   static Pixmap pixmap;

   pixmap = XmUNSPECIFIED_PIXMAP;

   value->addr = (char *) &pixmap;
   value->size = sizeof (Pixmap);

   if (pw->primitive.highlight_color == pw->core.background_pixel)
      pixmap = XmGetPixmap (XtScreen (pw), "50_foreground",
                            pw->primitive.highlight_color,
                            pw->primitive.foreground);

   else if (DefaultDepthOfScreen (XtScreen (widget)) == 1)
      pixmap = XmGetPixmap (XtScreen (pw), "50_foreground",
                            pw->primitive.highlight_color,
                            pw->core.background_pixel);
}



#ifdef _NO_PROTO
void _XmManagerHighlightPixmapDefault (widget, offset, value)
Widget   widget;
int      offset;
XrmValue * value;

#else /* _NO_PROTO */
void _XmManagerHighlightPixmapDefault (Widget widget, int offset, XrmValue *value)
#endif /* _NO_PROTO */
{
   XmManagerWidget mw = (XmManagerWidget) widget;
   static Pixmap pixmap;

   pixmap = XmUNSPECIFIED_PIXMAP;

   value->addr = (char *) &pixmap;
   value->size = sizeof (Pixmap);

   if (mw->manager.highlight_color == mw->core.background_pixel)
      pixmap = XmGetPixmap (XtScreen (mw), "50_foreground",
                            mw->manager.highlight_color,
                            mw->manager.foreground);

   if (DefaultDepthOfScreen (XtScreen (widget)) == 1)
      pixmap = XmGetPixmap (XtScreen (mw), "50_foreground",
                            mw->manager.highlight_color,
                            mw->core.background_pixel);
}




/*********************************************************************
 *
 *  _XmGetDynamicDefault
 *	Given the widget and the requested type of default, generate the
 *	default and store it in the value structure to be returned.
 *
 *********************************************************************/
 
static void _XmGetDynamicDefault (widget, type, offset, value)
Widget widget;
int type;
int offset;
XrmValue * value;

{
	Screen *screen;
	Colormap color_map;
	static Pixel new_value;
	XmColorData *color_data;

	value->size = sizeof(new_value);
	value->addr = (char *) &new_value;

	if (!XtIsWidget(widget))
		widget = widget->core.parent;

	screen = XtScreen(widget);
	color_map = widget->core.colormap;

	if (type == XmBACKGROUND)
	{
		color_data = _XmGetDefaultColors(screen, color_map);
	}
	else
	{
		color_data = _XmGetColors(screen, color_map,
			widget->core.background_pixel);
	}

	new_value = _XmAccessColorData(color_data, type);
}




/*********************************************************************
 *
 *  _XmGetDefalaultColors
 *
 *********************************************************************/
static String default_background_color_spec = "#729FFF";
static Boolean app_defined = FALSE;

#ifdef _NO_PROTO
String _XmGetDefaultBackgroundColorSpec()
#else /* _NO_PROTO */
String _XmGetDefaultBackgroundColorSpec (void)
#endif /* _NO_PROTO */
{
	return(default_background_color_spec);
}

#ifdef _NO_PROTO
void _XmSetDefaultBackgroundColorSpec(new_color_spec)
String new_color_spec;
#else /* _NO_PROTO */
void _XmSetDefaultBackgroundColorSpec (String new_color_spec)
#endif /* _NO_PROTO */
{
	if (app_defined)
	{
		XtFree(default_background_color_spec);
	}

	default_background_color_spec = (String)
		XtMalloc(strlen(new_color_spec) + 1);
	strcpy(default_background_color_spec, new_color_spec);

	app_defined = TRUE;
}

#ifdef _NO_PROTO
XmColorData * _XmGetDefaultColors(screen, color_map)
Screen * screen;
Colormap color_map;
#else /* _NO_PROTO */
XmColorData * _XmGetDefaultColors( Screen * screen, Colormap color_map)
#endif /* _NO_PROTO */
{
	static XmColorData ** default_set = NULL;
	static int default_set_count = 0;
	static int default_set_size = 0;
	register int i;
	XColor color_def;
	Pixel background;


	/*  Look through  a set of screen / background pairs to see  */
	/*  if the default is already in the table.                  */

	for (i = 0; i < default_set_count; i++)
	{
	if ((default_set[i]->screen == screen) &&
		(default_set[i]->color_map == color_map))
		return (default_set[i]);
	}

	/*  See if more space is needed in the array  */

	if (default_set == NULL)
	{
		default_set_size = 10;
		default_set = (XmColorData **) XtRealloc(default_set, 
			(sizeof(XmColorData *) * default_set_size));
		
	}
	else if (default_set_count == default_set_size)
	{
		default_set_size += 10;
		default_set = (XmColorData **) XtRealloc(default_set, 
			sizeof(XmColorData *) * default_set_size);
	}

	/* Find the background based on the depth of the screen */
	if (DefaultDepthOfScreen(screen) == 1)
		background = WhitePixelOfScreen(screen);
	else
	  {
		/*  Parse out a color for the default background  */

		if  (XParseColor(DisplayOfScreen(screen), color_map,
			_XmGetDefaultBackgroundColorSpec(), &color_def) != 0)
		  {
		         if (XAllocColor(DisplayOfScreen(screen), color_map, 
                                     &color_def) != 0)
			   background = color_def.pixel;
                         else
			   { 
		             XtWarning("Cannot allocate colormap entry for background, setting background to white ");
		             background = WhitePixelOfScreen(screen);
			   }                
	         }
                 else 
		   { 
		     XtWarning("Cannot parse given background color, setting background to white");
		         background = WhitePixelOfScreen(screen);
		       }                
	      }
	/*
	 * Get the color data generated and save it in the next open
	 * slot in the default set array.  default_set points to a subset
	 * of the data pointed to by color_set (defined in _XmGetColors).
	 */

	default_set[default_set_count] = 
		_XmGetColors(screen, color_map, background);
	default_set_count++;

	return (default_set[default_set_count - 1]);
}


/*
 * GLOBAL VARIABLES    (really only visible to the last routines
 *                      in this file.)
 *
 * These variables define the color cache.
 */
static int Set_Count=0, Set_Size=0;
static XmColorData *Color_Set=NULL;

#ifdef _NO_PROTO
Boolean _XmSearchColorCache(which, values, ret)
unsigned int which;
XmColorData *values;
XmColorData **ret;
#else
Boolean _XmSearchColorCache(unsigned int which, XmColorData *values,
	XmColorData **ret)
#endif /* _NO_PROTO */
{
	register int i;

	/* 
	 * Look through  a set of screen, color_map, background triplets 
	 * to see if these colors have already been generated.
	 */

	for (i = 0; i < Set_Count; i++)
	{
		if ( (!(which & LOOK_AT_SCREEN) ||
				((Color_Set + i)->screen == values->screen))
			&&
			(!(which & LOOK_AT_CMAP) ||
				((Color_Set + i)->color_map == values->color_map))
			&&
			(!(which & LOOK_AT_BACKGROUND) ||
				(((Color_Set + i)->allocated & XmBACKGROUND) &&
				((Color_Set + i)->background.pixel == 
					values->background.pixel)))
			&&
			(!(which & LOOK_AT_FOREGROUND) ||
				(((Color_Set + i)->allocated & XmFOREGROUND) &&
				((Color_Set + i)->foreground.pixel ==
					values->foreground.pixel)))
			&&
			(!(which & LOOK_AT_TOP_SHADOW) ||
				(((Color_Set + i)->allocated & XmTOP_SHADOW) &&
				((Color_Set + i)->top_shadow.pixel == 
					values->top_shadow.pixel)))
			&&
			(!(which & LOOK_AT_BOTTOM_SHADOW) ||
				(((Color_Set + i)->allocated & XmBOTTOM_SHADOW) &&
				((Color_Set+ i)->bottom_shadow.pixel == 
					values->bottom_shadow.pixel)))
			&&
			(!(which & LOOK_AT_SELECT) ||
				(((Color_Set + i)->allocated & XmSELECT) &&
				((Color_Set + i)->select.pixel ==
					values->select.pixel))))
			{
				*ret = (Color_Set + i);
				return (TRUE);
			}
	}

	*ret = NULL;
	return (FALSE);
}

#ifdef _NO_PROTO
XmColorData * _XmAddToColorCache(new_rec)
XmColorData *new_rec;
#else
XmColorData * _XmAddToColorCache(XmColorData *new_rec)
#endif /* _NO_PROTO */
{
	/*  See if more space is needed */

	if (Set_Count == Set_Size)
	{
		Set_Size += 10;
		Color_Set = 
		(XmColorData *)XtRealloc(Color_Set, 
			sizeof(XmColorData) * Set_Size);
	}

	*(Color_Set + Set_Count) = *new_rec;
	Set_Count++;

	return(Color_Set + (Set_Count - 1));
}

#ifdef _NO_PROTO
Pixel _XmAccessColorData(cd, which)
XmColorData *cd;
unsigned char which;
#else
Pixel _XmAccessColorData(XmColorData *cd, unsigned char which)
#endif
{
	Pixel p;

	switch(which)
	{
		case XmBACKGROUND:
			if (!(cd->allocated & which) && 
				(XAllocColor(cd->screen->display,
				cd->color_map, &(cd->background)) == 0))
				cd->background.pixel = BlackPixelOfScreen(cd->screen);
			p = cd->background.pixel;
			cd->allocated |= which;
		break;
		case XmFOREGROUND:
			if (cd->allocated & which)
				p = cd->foreground.pixel;
			else if ((cd->foreground.red == 0) &&
				(cd->foreground.green == 0) &&
				(cd->foreground.blue == 0))
				p = cd->foreground.pixel =
					BlackPixelOfScreen(cd->screen);
			else if ((cd->foreground.red == XmMAX_SHORT) &&
				(cd->foreground.green == XmMAX_SHORT) &&
				(cd->foreground.blue == XmMAX_SHORT))
				p = cd->foreground.pixel =
					WhitePixelOfScreen(cd->screen);
			else if ((p = XAllocColor(cd->screen->display,
					cd->color_map, &(cd->foreground))) == 0)
				p = cd->foreground.pixel = 
					BlackPixelOfScreen(cd->screen);
			cd->allocated |= which;
		break;
		case XmTOP_SHADOW:
			if (!(cd->allocated & which) &&
				(XAllocColor(cd->screen->display,
				cd->color_map, &(cd->top_shadow)) == 0))
			{
				if (_XmBrightness(&(cd->background))
						> XmCOLOR_LITE_THRESHOLD)
					cd->top_shadow.pixel = 
						BlackPixelOfScreen(cd->screen);
				else
					cd->top_shadow.pixel =
						WhitePixelOfScreen(cd->screen);
			}
			p = cd->top_shadow.pixel;
			cd->allocated |= which;
		break;
		case XmBOTTOM_SHADOW:
			if (!(cd->allocated & which) &&
				(XAllocColor(cd->screen->display,
					cd->color_map, &(cd->bottom_shadow)) == 0))
			{
				if (_XmBrightness(&(cd->background))
						< XmCOLOR_DARK_THRESHOLD)
					cd->bottom_shadow.pixel =  
						_XmAccessColorData(cd, XmTOP_SHADOW);
				else
					cd->bottom_shadow.pixel = 
						BlackPixelOfScreen(cd->screen);
			}
			p = cd->bottom_shadow.pixel;
			cd->allocated |= which;
		break;
		case XmSELECT:
			if (!(cd->allocated & which) &&
				(XAllocColor(cd->screen->display,
				cd->color_map, &(cd->select)) == 0))
			{
				if (_XmBrightness(&(cd->background)) 
						< XmCOLOR_DARK_THRESHOLD)
					cd->select.pixel = WhitePixelOfScreen(cd->screen);
				else
					cd->select.pixel = BlackPixelOfScreen(cd->screen);
			}
			p = cd->select.pixel;
			cd->allocated |= which;
		break;
		default:
			XtWarning(MESSAGE1);
			p = BlackPixelOfScreen(cd->screen);
		break;
	}

	return(p);
}

static void SetMonochromeColors(colors)
XmColorData *colors;
{
	Screen *screen = colors->screen;
	Pixel background = colors->background.pixel;

	if (background == BlackPixelOfScreen(screen))
	{
		colors->foreground.pixel = WhitePixelOfScreen (screen);
		colors->foreground.red = colors->foreground.green = 
			colors->foreground.blue = XmMAX_SHORT;

		colors->bottom_shadow.pixel = WhitePixelOfScreen(screen);
		colors->bottom_shadow.red = colors->bottom_shadow.green = 
			colors->bottom_shadow.blue = XmMAX_SHORT;

		colors->select.pixel = WhitePixelOfScreen(screen);
		colors->select.red = colors->select.green = 
			colors->select.blue = XmMAX_SHORT;

		colors->top_shadow.pixel = BlackPixelOfScreen(screen);
		colors->top_shadow.red = colors->top_shadow.green = 
			colors->top_shadow.blue = 0;
	}
	else if (background == WhitePixelOfScreen(screen))
	{
		colors->foreground.pixel = BlackPixelOfScreen(screen);
		colors->foreground.red = colors->foreground.green = 
			colors->foreground.blue = 0;

		colors->top_shadow.pixel = WhitePixelOfScreen(screen);
		colors->top_shadow.red = colors->top_shadow.green = 
			colors->top_shadow.blue = XmMAX_SHORT;

		colors->bottom_shadow.pixel = BlackPixelOfScreen(screen);
		colors->bottom_shadow.red = colors->bottom_shadow.green = 
			colors->bottom_shadow.blue = 0;

		colors->select.pixel = BlackPixelOfScreen(screen);
		colors->select.red = colors->select.green = 
			colors->select.blue = 0;
	}

	colors->allocated |= (XmFOREGROUND | XmTOP_SHADOW 
		| XmBOTTOM_SHADOW | XmSELECT);
}

#ifdef _NO_PROTO
static int _XmBrightness(color)
XColor *color;
#else /* _NO_PROTO */
static int _XmBrightness(XColor *color)
#endif /* _NO_PROTO */
{
	int brightness;
	int intensity;
	int light;
	int luminosity, maxprimary, minprimary;
	int red = color->red;
	int green = color->green;
	int blue = color->blue;

	intensity = (red + green + blue) / 3;

	/* 
	 * The casting nonsense below is to try to control the point at
	 * the truncation occurs.
	 */

	luminosity = (int) ((XmRED_LUMINOSITY * (float) red)
		+ (XmGREEN_LUMINOSITY * (float) green)
		+ (XmBLUE_LUMINOSITY * (float) blue));

	maxprimary = ( (red > green) ?
					( (red > blue) ? red : blue ) :
					( (green > blue) ? green : blue ) );

	minprimary = ( (red < green) ?
					( (red < blue) ? red : blue ) :
					( (green < blue) ? green : blue ) );

	light = (minprimary + maxprimary) / 2;

	brightness = ( (intensity * XmINTENSITY_FACTOR) +
				   (light * XmLIGHT_FACTOR) +
				   (luminosity * XmLUMINOSITY_FACTOR) ) / 100;
	return(brightness);
}

#define BoundColor(value)\
	((value < 0) ? 0 : (((value > XmMAX_SHORT) ? XmMAX_SHORT : value)))

static void CalculateColorsForLightBackground(bg_color,
	fg_color, sel_color, ts_color, bs_color)
XColor *bg_color, *fg_color, *sel_color, *ts_color, *bs_color;
{
	int color_value;

	if (fg_color)
	{
		fg_color->red = 0;
		fg_color->green = 0;
		fg_color->blue = 0;
	}

	if (sel_color)
	{
		color_value = bg_color->red;
		color_value -= (color_value * XmCOLOR_LITE_SEL_FACTOR) / 100;
		sel_color->red = color_value;

		color_value = bg_color->green;
		color_value -= (color_value * XmCOLOR_LITE_SEL_FACTOR) / 100;
		sel_color->green = color_value;

		color_value = bg_color->blue;
		color_value -= (color_value * XmCOLOR_LITE_SEL_FACTOR) / 100;
		sel_color->blue = color_value;
	}

	if (bs_color)
	{
		color_value = bg_color->red;
		color_value -= (color_value * XmCOLOR_LITE_BS_FACTOR) / 100;
		bs_color->red = color_value;

		color_value = bg_color->green;
		color_value -= (color_value * XmCOLOR_LITE_BS_FACTOR) / 100;
		bs_color->green = color_value;

		color_value = bg_color->blue;
		color_value -= (color_value * XmCOLOR_LITE_BS_FACTOR) / 100;
		bs_color->blue = color_value;
	}

	if (ts_color)
	{
		color_value = bg_color->red;
		color_value -= (color_value * XmCOLOR_LITE_TS_FACTOR) / 100;
		ts_color->red = color_value;

		color_value = bg_color->green;
		color_value -= (color_value * XmCOLOR_LITE_TS_FACTOR) / 100;
		ts_color->green = color_value;

		color_value = bg_color->blue;
		color_value -= (color_value * XmCOLOR_LITE_TS_FACTOR) / 100;
		ts_color->blue = color_value;
	}
}
	
static void CalculateColorsForDarkBackground(bg_color, fg_color,
	sel_color, ts_color, bs_color)
XColor *bg_color, *fg_color, *sel_color, *ts_color, *bs_color;
{
	int color_value;

	if (fg_color)
	{
		fg_color->red = XmMAX_SHORT;
		fg_color->green = XmMAX_SHORT;
		fg_color->blue = XmMAX_SHORT;
	}

	if (sel_color)
	{
		color_value = bg_color->red;
		color_value += XmCOLOR_DARK_SEL_FACTOR *
			(XmMAX_SHORT - color_value) / 100;
		sel_color->red = color_value;

		color_value = bg_color->green;
		color_value += XmCOLOR_DARK_SEL_FACTOR *
			(XmMAX_SHORT - color_value) / 100;
		sel_color->green = color_value;

		color_value = bg_color->blue;
		color_value += XmCOLOR_DARK_SEL_FACTOR *
			(XmMAX_SHORT - color_value) / 100;
		sel_color->blue = color_value;
	}

	if (bs_color)
	{
		color_value = bg_color->red;
		color_value += XmCOLOR_DARK_BS_FACTOR *
			(XmMAX_SHORT - color_value) / 100;
		bs_color->red = color_value;

		color_value = bg_color->green;
		color_value += XmCOLOR_DARK_BS_FACTOR *
			(XmMAX_SHORT - color_value) / 100;
		bs_color->green = color_value;

		color_value = bg_color->blue;
		color_value += XmCOLOR_DARK_BS_FACTOR *
			(XmMAX_SHORT - color_value) / 100;
		bs_color->blue = color_value;
	}

	if (ts_color)
	{
		color_value = bg_color->red;
		color_value += XmCOLOR_DARK_TS_FACTOR *
			(XmMAX_SHORT - color_value) / 100;
		ts_color->red = color_value;

		color_value = bg_color->green;
		color_value += XmCOLOR_DARK_TS_FACTOR *
			(XmMAX_SHORT - color_value) / 100;
		ts_color->green = color_value;

		color_value = bg_color->blue;
		color_value += XmCOLOR_DARK_TS_FACTOR *
			(XmMAX_SHORT - color_value) / 100;
		ts_color->blue = color_value;
	}
}

static void CalculateColorsForMediumBackground(bg_color, fg_color,
	sel_color, ts_color, bs_color)
XColor *bg_color,  *fg_color, *sel_color, *ts_color, *bs_color;
{
	int brightness = _XmBrightness(bg_color);
	int color_value, f;

        if (brightness > XmFOREGROUND_THRESHOLD)
	{
	   fg_color->red = 0;
	   fg_color->green = 0;
	   fg_color->blue = 0;
	}
	else
	{
	   fg_color->red = XmMAX_SHORT;
	   fg_color->green = XmMAX_SHORT;
	   fg_color->blue = XmMAX_SHORT;
	 }
      
	if (sel_color)
	{
		f = XmCOLOR_LO_SEL_FACTOR + ( brightness
			* ( XmCOLOR_HI_SEL_FACTOR - XmCOLOR_LO_SEL_FACTOR )
			/ XmMAX_SHORT );

		color_value = bg_color->red;
		color_value -= (color_value * f) / 100;
		sel_color->red = color_value;

		color_value = bg_color->green;
		color_value -= (color_value * f) / 100;
		sel_color->green = color_value;

		color_value = bg_color->blue;
		color_value -= (color_value * f) / 100;
		sel_color->blue = color_value;
	      }

	if (bs_color)
	{
		f = XmCOLOR_LO_BS_FACTOR - (brightness 
			* ( XmCOLOR_LO_BS_FACTOR - XmCOLOR_HI_BS_FACTOR )
			/ XmMAX_SHORT);

		color_value = bg_color->red;
		color_value -= (color_value * f) / 100;
		bs_color->red = color_value;

		color_value = bg_color->green;
		color_value -= (color_value * f) / 100;
		bs_color->green = color_value;

		color_value = bg_color->blue;
		color_value -= (color_value * f) / 100;
		bs_color->blue = color_value;
	      }

	if (ts_color)
	{
		f = XmCOLOR_LO_TS_FACTOR + (brightness
			* ( XmCOLOR_HI_TS_FACTOR - XmCOLOR_LO_TS_FACTOR )
			/ XmMAX_SHORT);

		color_value = bg_color->red;
		color_value += f * ( XmMAX_SHORT - color_value ) / 100;
		ts_color->red = color_value;

		color_value = bg_color->green;
		color_value += f * ( XmMAX_SHORT - color_value ) / 100;
		ts_color->green = color_value;

		color_value = bg_color->blue;
		color_value += f * ( XmMAX_SHORT - color_value ) / 100;
		ts_color->blue = color_value;
	}      
}      
   

static void _XmCalculateColorsRGB(bg_color, fg_color,
        sel_color, ts_color, bs_color)
XColor *bg_color, *fg_color, *sel_color, *ts_color, *bs_color;
{
	int brightness = _XmBrightness(bg_color);

	if (brightness < XmCOLOR_DARK_THRESHOLD)
		CalculateColorsForDarkBackground(bg_color, fg_color,
			sel_color, ts_color, bs_color);
	else if (brightness > XmCOLOR_LITE_THRESHOLD)
		CalculateColorsForLightBackground(bg_color, fg_color,
			sel_color, ts_color, bs_color);
	else
		CalculateColorsForMediumBackground(bg_color, fg_color,
			sel_color, ts_color, bs_color);
}


static XmColorProc ColorRGBCalcProc = _XmCalculateColorsRGB;

#ifdef _NO_PROTO
XmColorProc XmSetColorCalculation(proc)
XmColorProc proc;
#else
XmColorProc XmSetColorCalculation(XmColorProc proc)
#endif /* _NO_PROTO */
{
	XmColorProc a = ColorRGBCalcProc;

	if (proc != NULL)
		ColorRGBCalcProc = proc;
	else
		ColorRGBCalcProc = _XmCalculateColorsRGB;
	
	return(a);
}

#ifdef _NO_PROTO
XmColorProc XmGetColorCalculation()
#else
XmColorProc XmGetColorCalculation(void)
#endif /* _NO_PROTO */
{
	return(ColorRGBCalcProc);
}


/*********************************************************************
 *
 *  XmGetColors
 *
 *********************************************************************/

#ifdef _NO_PROTO
XmColorData * _XmGetColors (screen, color_map, background)
Screen * screen;
Colormap color_map;
Pixel background;
#else /* _NO_PROTO */
XmColorData * _XmGetColors (Screen * screen, Colormap color_map,
	Pixel background)
#endif /* _NO_PROTO */
{
	Display * display = DisplayOfScreen (screen);
	XColor bg_color;
	XColor new_color;
	unsigned int color_value;
	XmColorData *old_colors;
	XmColorData new_colors;


	new_colors.screen = screen;
	new_colors.color_map = color_map;
	new_colors.background.pixel = background;

	if (_XmSearchColorCache(
		(LOOK_AT_SCREEN | LOOK_AT_CMAP | LOOK_AT_BACKGROUND),
			&new_colors, &old_colors))
		return(old_colors);

	XQueryColor (display, color_map, &(new_colors.background));
	new_colors.allocated = XmBACKGROUND;

	/*
	 * Just in case somebody looks at these before they're ready,
	 * initialize them to a value that is always valid (for most
	 * implementations of X).
	 */
	new_colors.foreground.pixel = new_colors.top_shadow.pixel = 
		new_colors.top_shadow.pixel = new_colors.select.pixel = 0;

	/*  Generate the foreground, top_shadow, and bottom_shadow based  */
	/*  on the background                                             */

	if (DefaultDepthOfScreen(screen) == 1)
		SetMonochromeColors(&new_colors);
	else
		(*ColorRGBCalcProc)(&(new_colors.background),
			&(new_colors.foreground), &(new_colors.select),
			&(new_colors.top_shadow), &(new_colors.bottom_shadow));

	return (_XmAddToColorCache(&new_colors));
}


#ifdef _NO_PROTO
void XmGetColors (screen, color_map, background,
	foreground_ret, top_shadow_ret, bottom_shadow_ret, select_ret)
Screen * screen;
Colormap color_map;
Pixel background;
Pixel *foreground_ret, *top_shadow_ret, *bottom_shadow_ret, *select_ret;
#else /* _NO_PROTO */
void XmGetColors (Screen * screen, Colormap color_map, 
	Pixel background, Pixel *foreground_ret, Pixel *top_shadow_ret, 
	Pixel *bottom_shadow_ret, Pixel *select_ret)
#endif /* _NO_PROTO */
{
	XmColorData *cd;

	cd = _XmGetColors(screen, color_map, background);

	if (foreground_ret)
		*foreground_ret = _XmAccessColorData(cd, XmFOREGROUND);
	if (top_shadow_ret)
		*top_shadow_ret = _XmAccessColorData(cd, XmTOP_SHADOW);
	if (bottom_shadow_ret)
		*bottom_shadow_ret = _XmAccessColorData(cd, XmBOTTOM_SHADOW);
	if (select_ret)
		*select_ret = _XmAccessColorData(cd, XmSELECT);
}
