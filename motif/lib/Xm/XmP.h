#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)XmP.h	3.34 91/01/10";
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
/************************************<+>*************************************
 ****************************************************************************
 **
 **   File:        XmP.h
 **
 **   Description: This include file contains the class and instance record
 **                definitions for all meta classes.  It also contains externs
 **                for internally shared functions and defines for internally 
 **                shared values.
 **
 ****************************************************************************
 ************************************<+>*************************************/

#ifndef _XmP_h
#define _XmP_h

#include <Xm/Xm.h>
#include <X11/IntrinsicP.h>
#include <Xm/Traversal.h>
#include <Xm/VendorE.h>

/***************************************************************************
 *  Macros replacing toolkit macros so that gadgets are handled properly. 
 ***************************************************************************/

#ifdef XtName
#undef XtName
#endif
#define XtName(widget) XrmQuarkToString (((Object)(widget))->object.xrm_name)

#ifdef XtDisplay
#undef XtDisplay
#endif
#define XtDisplay(widget)						\
   (XtIsWidget(widget)         ?					\
      ((Widget)(widget))->core.screen->display :			\
      ((Object)(widget))->object.parent->core.screen->display)

#ifdef XtScreen
#undef XtScreen
#endif
#define XtScreen(widget) (XtIsWidget(widget)  ?		\
			 ((Widget)(widget))->core.screen   :	\
			 ((Object)(widget))->object.parent->core.screen)

#ifdef XtWindow
#undef XtWindow
#endif
#define XtWindow(widget) (XtIsWidget(widget)  ?			\
			 ((Widget)(widget))->core.window   :		\
			 ((Object)(widget))->object.parent->core.window)

#ifdef XtClass
#undef XtClass
#endif
#define XtClass(widget)	(((Object)(widget))->object.widget_class)

#ifdef XtSuperclass
#undef XtSuperclass
#endif
#define XtSuperclass(widget) (XtClass(widget)->core_class.superclass)


#ifdef XtIsRealized
#undef XtIsRealized
#endif
#define XtIsRealized(widget) 					\
   (XtIsWidget(widget)  ?					\
      ((Widget)(widget))->core.window != NULL  :		\
      ((Object)(widget))->object.parent->core.window != NULL)

#ifdef XtIsManaged
#undef XtIsManaged
#endif
#define XtIsManaged(widget) 					\
  (XtIsRectObj(widget) ? (((RectObj)(widget))->rectangle.managed) : False)


#ifdef XtIsSensitive
#undef XtIsSensitive
#endif
#define XtIsSensitive(widget) 					\
  ((!XtIsRectObj(widget)) ? False : 				\
   (((RectObj)(widget))->rectangle.sensitive && 		\
    ((RectObj)(widget))->rectangle.ancestor_sensitive))

#ifdef XtParent
#undef XtParent
#endif
#define XtParent(widget) (((Object)(widget))->object.parent)

/***********************************************************************
 *
 *  Extern and defines for setting up a default FontList
 *
 ***********************************************************************/
#define XmLABEL_FONTLIST     1
#define XmBUTTON_FONTLIST    2
#define XmTEXT_FONTLIST      3

extern XmFontList _XmGetDefaultFontList();


#ifndef _ARGUMENTS
#ifdef _NO_PROTO
#define _ARGUMENTS(arglist) ()
#else
#define _ARGUMENTS(arglist) arglist
#endif
#endif

extern Boolean _XmFontListGetDefaultFont _ARGUMENTS((
 	XmFontList	fontlist,
 	XFontStruct	**font_struct));
extern Boolean _XmFontListSearch  _ARGUMENTS(( XmFontList fontlist , XmStringCharSet charset , short *index , XFontStruct **font_struct ));
extern Boolean _XmStringIsXmString  _ARGUMENTS((XmString string ));
extern Boolean _XmStringInitContext  _ARGUMENTS(( _XmStringContext *context , _XmString string ));
extern Boolean _XmStringGetNextSegment  _ARGUMENTS(( _XmStringContext context , XmStringCharSet *charset , XmStringDirection *direction , char **text , short *char_count , Boolean *separator ));
extern void _XmStringFreeContext  _ARGUMENTS(( _XmStringContext context ));
extern Dimension _XmStringWidth  _ARGUMENTS(( XmFontList fontlist , _XmString string ));
extern Dimension _XmStringHeight  _ARGUMENTS(( XmFontList fontlist , _XmString string ));
extern void _XmStringExtent  _ARGUMENTS(( XmFontList fontlist , _XmString string , Dimension *width , Dimension *height ));
extern Boolean _XmStringEmpty  _ARGUMENTS(( _XmString string ));
extern void _XmStringDraw  _ARGUMENTS(( Display *d , Window w , XmFontList fontlist , _XmString string , GC gc , Position x , Position y , Dimension width , unsigned char align , unsigned char lay_dir , XRectangle *clip ));
extern void _XmStringDrawImage  _ARGUMENTS(( Display *d , Window w , XmFontList fontlist , _XmString string , GC gc , Position x , Position y , Dimension width , unsigned char align , unsigned char lay_dir , XRectangle *clip ));
extern void _XmStringDrawUnderline  _ARGUMENTS(( 
Display *d , Window w , XmFontList f , _XmString s , GC gc , 
Position x , Position y , Dimension width , unsigned char align , unsigned char lay_dir , 
XRectangle *clip , _XmString u ));
extern void _XmStringDrawMnemonic  _ARGUMENTS(( 
Display *d , Window w , XmFontList fontlist , _XmString string , GC gc , 
Position x , Position y , Dimension width , unsigned char align , unsigned char lay_dir , 
XRectangle *clip , String mnemonic , XmStringCharSet charset ));
extern _XmString _XmStringCreate  _ARGUMENTS(( XmString cs ));
extern void _XmStringFree  _ARGUMENTS(( _XmString string ));
extern void _XmStringUpdate  _ARGUMENTS(( XmFontList fontlist , _XmString string ));
extern _XmString _XmStringCopy  _ARGUMENTS(( _XmString string ));
extern Boolean _XmStringByteCompare  _ARGUMENTS(( _XmString a , _XmString b ));
extern Boolean _XmStringHasSubstring  _ARGUMENTS(( _XmString string , _XmString substring ));
extern XmString _XmStringCreateExternal  _ARGUMENTS(( XmFontList fontlist , _XmString cs ));
extern Dimension _XmStringBaseline  _ARGUMENTS(( XmFontList fontlist , _XmString string ));
extern int _XmStringLineCount  _ARGUMENTS(( _XmString string ));
#ifdef DEBUG
extern void dump_fontlist  _ARGUMENTS(( XmFontList f ));
extern void dump_fontlist_cache  _ARGUMENTS(( void ));
extern void dump_external  _ARGUMENTS(( XmString cs ));
extern void dump_internal  _ARGUMENTS(( _XmString string ));
#endif
extern char *_XmStringGetCurrentCharset _ARGUMENTS(( void ));
extern char *_XmCharsetCanonicalize _ARGUMENTS(( char *charset ));

/************************************************************************
 *
 *  Extern for XmWarning
 *
 ************************************************************************/

extern void _XmWarning();
extern void XmRegisterConverters ();

/***********************************************************************
 *
 * Miscellaneous Private Defines
 *
 ***********************************************************************/

/* For the default color calculation and caching */

#define LOOK_AT_SCREEN          (1<<0)
#define LOOK_AT_CMAP            (1<<1)
#define LOOK_AT_BACKGROUND      (1<<2)
#define LOOK_AT_FOREGROUND      (1<<3)
#define LOOK_AT_TOP_SHADOW      (1<<4)
#define LOOK_AT_BOTTOM_SHADOW   (1<<5)
#define LOOK_AT_SELECT          (1<<6)

#define XmBACKGROUND     ((unsigned char) (1<<0))
#define XmFOREGROUND     ((unsigned char) (1<<1))
#define XmTOP_SHADOW     ((unsigned char) (1<<2))
#define XmBOTTOM_SHADOW  ((unsigned char) (1<<3))
#define XmSELECT         ((unsigned char) (1<<4))

/*  Structure used to hold color schemes  */
typedef struct _XmColorData
{  Screen * screen;
   Colormap color_map;
   unsigned char allocated;
   XColor background;
   XColor foreground;
   XColor top_shadow;
   XColor bottom_shadow;
   XColor select;
} XmColorData;


#ifdef _NO_PROTO
String        _XmGetDefaultBackgroundColorSpec();
void          _XmSetDefaultBackgroundColorSpec();
XmColorData * _XmGetDefaultColors();
XmColorData * _XmGetColors();
Boolean       _XmSearchColorCache();
XmColorData * _XmAddToColorCache();
Pixel         _XmAccessColorData();
#else /* _NO_PROTO */
String _XmGetDefaultBackgroundColorSpec (void);
void _XmSetDefaultBackgroundColorSpec (String new_color_spec);
XmColorData * _XmGetDefaultColors(Screen *screen, Colormap color_map);
XmColorData * _XmGetColors(Screen * screen, Colormap color_map,
	Pixel background);
Boolean _XmSearchColorCache(unsigned int which, XmColorData *values, 
	XmColorData **ret);
XmColorData * _XmAddToColorCache(XmColorData *new_rec);
Pixel _XmAccessColorData(XmColorData *cd, unsigned char which);
#endif /* _NO_PROTO */



#define XmStrlen(s)      ((s) ? strlen(s) : 0)

#define XmPRIMITIVE	0
#define XmMANAGER	1


/* Defines used by geometry manager utilities */

#define XmGET_ACTUAL_SIZE	1
#define XmGET_PREFERRED_SIZE	2
#define XmGEO_PRE_SET		3
#define XmGEO_POST_SET		4

/* Defaults for Geometry Utility defines are always 0.
*/
#define XmGEO_EXPAND		0
#define XmGEO_CENTER		1
#define XmGEO_PACK		2

#define XmGEO_PROPORTIONAL	0
#define XmGEO_AVERAGING		1
#define XmGEO_WRAP		2

#define XmGEO_ROW_MAJOR		0
#define XmGEO_COLUMN_MAJOR	1


/* Defines used for menu/button communication */
#define XmMENU_POPDOWN 		0
#define XmMENU_PROCESS_TREE 	1
#define XmMENU_TRAVERSAL 	2
#define XmMENU_SHELL_POPDOWN 	3
#define XmMENU_CALLBACK		4
#define XmMENU_BUTTON		5
#define XmMENU_CASCADING	6
#define XmMENU_SUBMENU		7
#define XmMENU_ARM		8
#define XmMENU_DISARM		9
#define XmMENU_BAR_CLEANUP	10


/* For MapEvent: _XmMatchBtnEvent */
#define XmIGNORE_EVENTTYPE      -1

/* Default minimum Toggle indicator dimension */
#define DEFAULT_INDICATOR_DIM   9

/* General purpose macros */

#ifndef MAX
#define MAX(x,y)	((x) > (y) ? (x) : (y))
#endif

#define XtX(w)		 ((w)->core.x)
#define XtY(w)		 ((w)->core.y)
#define XtWidth(w)	 ((w)->core.width)
#define XtHeight(w)	 ((w)->core.height)
#define XtBorderWidth(w) ((w)->core.border_width)
#define XtBackground(w)	 ((w)->core.background_pixel)
#define XtSensitive(w)	 ((w)->core.sensitive && (w)->core.ancestor_sensitive) 

#define RX(r)		    (((RectObj) r)->rectangle.x)
#define RY(r)		    (((RectObj) r)->rectangle.y)
#define RWidth(r)	    (((RectObj) r)->rectangle.width)
#define RHeight(r)	    (((RectObj) r)->rectangle.height)
#define RBorder(r)	    (((RectObj) r)->rectangle.border_width)

#define GMode(g)	    ((g)->request_mode)
#define IsX(g)		    (GMode (g) & CWX)
#define IsY(g)		    (GMode (g) & CWY)
#define IsWidth(g)	    (GMode (g) & CWWidth)
#define IsHeight(g)	    (GMode (g) & CWHeight)
#define IsBorder(g)	    (GMode (g) & CWBorderWidth)
#define IsWidthHeight(g)    ((GMode (g) & CWWidth) || (GMode (g) & CWHeight))

#ifndef XtParent
#define XtParent(w)	 ((w)->core.parent)
#endif

#define XtCoreProc(w,proc) ((w)->core.widget_class->core_class.proc)


/************************************************************************
 *
 *  Resource definition function and structure used by Primitive, Gadget
 *  and Manager to define there get values hook processing lists
 *
 ************************************************************************/

typedef enum { 
   XmSYNTHETIC_NONE, XmSYNTHETIC_LOAD 
} XmImportOperator;

typedef void (*XmExportProc)();
/* Widget widget         */
/* int resource_offset   */
/* XtArgVal value        */

typedef XmImportOperator (*XmImportProc)();
/* Widget widget         */
/* int resource_offset   */
/* XtArgVal value        */

typedef struct _XmSyntheticResource
{
   String   resource_name;
   Cardinal resource_size;
   Cardinal resource_offset;
   XmExportProc export_proc;
   XmImportProc import_proc;
} XmSyntheticResource;


/***********************************************************************
 *
 * Resolution independence functions
 *
 ***********************************************************************/
#ifdef NOCODE
extern void _XmSortResourceList();
extern void _XmUnitTypeDefault();

extern XmImportOperator _XmToHorizontalPixels();
extern XmImportOperator _XmToVerticalPixels();
extern void _XmFromHorizontalPixels();
extern void _XmFromVerticalPixels();
extern int _XmConvertUnits();

extern void _XmBuildPrimitiveResources();
extern void _XmBuildGadgetResources();
extern void _XmBuildManagerResources();

extern void _XmPrimitiveImportArgs();
extern void _XmGadgetImportArgs();
extern void _XmManagerImportArgs();
extern void _XmShellImportArgs();

extern void _XmPrimitiveGetValuesHook();
extern void _XmGadgetGetValuesHook();
extern void _XmManagerGetValuesHook();
extern void _XmShellGetValuesHook();
#endif /* NOCODE */
#ifdef _NO_PROTO
extern void _XmBuildPrimitiveResources ();
extern void _XmBuildGadgetResources ();
extern void _XmBuildManagerResources ();
extern void _XmPrimitiveGetValuesHook ();
extern void _XmGadgetGetValuesHook ();
extern void _XmManagerGetValuesHook ();
extern void _XmShellGetValuesHook ();
extern void _XmShellImportArgs ();
extern void _XmPrimitiveImportArgs ();
extern void _XmGadgetImportArgs ();
extern void _XmManagerImportArgs ();
extern int _XmConvertUnits ();
extern XmImportOperator _XmToHorizontalPixels ();
extern XmImportOperator _XmToVerticalPixels ();
extern void _XmFromHorizontalPixels ();
extern void _XmFromVerticalPixels ();
extern void _XmSortResourceList ();
extern void _XmUnitTypeDefault ();
extern void XmSetFontUnit ();
#else /* _NO_PROTO */
extern void _XmBuildPrimitiveResources (XmPrimitiveWidgetClass wc);
extern void _XmBuildGadgetResources (XmGadgetClass wc);
extern void _XmBuildManagerResources (XmManagerWidgetClass wc);
extern void _XmPrimitiveGetValuesHook (Widget w, ArgList args, Cardinal *num_args);
extern void _XmGadgetGetValuesHook (Widget w, ArgList args, Cardinal *num_args);
extern void _XmManagerGetValuesHook (Widget w, ArgList args, Cardinal *num_args);
extern void _XmShellGetValuesHook (Widget w, ArgList args, Cardinal *num_args);
extern void _XmShellImportArgs (Widget w, ArgList args, Cardinal *num_args);
extern void _XmPrimitiveImportArgs (Widget w, ArgList args, Cardinal *num_args);
extern void _XmGadgetImportArgs (Widget w, ArgList args, Cardinal *num_args);
extern void _XmManagerImportArgs (Widget w, ArgList args, Cardinal *num_args);
extern int _XmConvertUnits (Screen *screen, int dimension, int from_type, int from_val, int to_type);
extern XmImportOperator _XmToHorizontalPixels (Widget widget, int offset, XtArgVal *value);
extern XmImportOperator _XmToVerticalPixels (Widget widget, int offset, XtArgVal *value);
extern void _XmFromHorizontalPixels (Widget widget, int offset, XtArgVal *value);
extern void _XmFromVerticalPixels (Widget widget, int offset, XtArgVal *value);
extern void _XmSortResourceList (XrmResource *list [], Cardinal len);
extern void _XmUnitTypeDefault (Widget widget, int offset, XrmValue *value);
extern void XmSetFontUnit (Display *display, int value);
#endif /* _NO_PROTO */

#define XmINVALID_DIMENSION 0xFFFF

/************************************************************************
 *
 *  Primitive class and instance definitions.  Primitive #defines
 *  and internally used exported functions.
 *
 ************************************************************************/
typedef void (*XmMenuProc)(
#ifndef _NO_PROTO
int proc,
Widget widget,
Boolean flag,
caddr_t data,
caddr_t data2
#endif
);

typedef void (*XmMenuTrav)(
#ifndef _NO_PROTO
Widget rc,
Widget pw,
int direction
#endif
);

typedef void (*XmPopFunc)(
#ifndef _NO_PROTO
    Widget 		/* widget */,
    XEvent*		/* event */
#endif
);

typedef void (*XmArmAndActivate)(
#ifndef _NO_PROTO
    Widget 		/* widget */,
    XEvent*		/* event */
#endif
);

typedef void (*XmInputDisProc)(
#ifndef _NO_PROTO
    Widget 		/* widget */,
    XEvent*		/* event */,
    Mask			     
#endif
);

typedef struct _XmPrimitiveClassPart
{
   XtWidgetProc         border_highlight;
   XtWidgetProc         border_unhighlight;
   String               translations;
   XmArmAndActivate     arm_and_activate;
   XmSyntheticResource * syn_resources;   
   int                  num_syn_resources;   
   caddr_t              extension;
} XmPrimitiveClassPart;

typedef struct _XmPrimitiveClassRec
{
    CoreClassPart        core_class;
    XmPrimitiveClassPart primitive_class;
} XmPrimitiveClassRec;

#ifndef PRIMITIVE
externalref XmPrimitiveClassRec xmPrimitiveClassRec;
#endif


/*  The Primitive instance record  */

typedef struct _XmPrimitivePart
{
   Pixel   foreground;

   Dimension   shadow_thickness;
   Pixel   top_shadow_color;
   Pixmap  top_shadow_pixmap;
   Pixel   bottom_shadow_color;
   Pixmap  bottom_shadow_pixmap;

   Dimension   highlight_thickness;
   Pixel   highlight_color;
   Pixmap  highlight_pixmap;

   XtCallbackList help_callback;
   caddr_t        user_data;

   Boolean traversal_on;
   Boolean highlight_on_enter;
   Boolean have_traversal;

   unsigned char unit_type;
   unsigned char navigation_type;

   Boolean highlight_drawn;
   Boolean highlighted;

   GC      highlight_GC;
   GC      top_shadow_GC;
   GC      bottom_shadow_GC;
} XmPrimitivePart;

typedef struct _XmPrimitiveRec
{
   CorePart        core;
   XmPrimitivePart primitive;
} XmPrimitiveRec;

/* Access Macros */
#define Prim_ShadowThickness(w) (((XmPrimitiveWidget)(w))->primitive.shadow_thickness)
#define Prim_HaveTraversal(w) (((XmPrimitiveWidget)(w))->primitive.have_traversal)

/*  Structure used for terminating keyboard actions  */

typedef struct 
{
   Boolean * keyIsPressed;
   void (*handler)();
} _XmTerminateData;





/************************************************************************
 *
 *  Gadget class and instance definitions.  Gadget #defines
 *  and internally used exported functions.
 *
 ************************************************************************/


/* Gadget cache header for each gadget's Cache Part */

typedef struct _XmGadgetCache
{
   struct _XmGadgetCache	*next;
   struct _XmGadgetCache	*prev;
   int				ref_count;
} XmGadgetCache, *XmGadgetCachePtr;

/* A cache entry for each class which desires gadget caching */

typedef void (*XmCacheCopyProc)(
#ifndef _NO_PROTO
caddr_t  src, 
caddr_t  dest,
int      size
#endif
);

typedef void (*XmCacheDeleteProc)(
#ifndef _NO_PROTO
caddr_t data
#endif
);

typedef struct _XmCacheClassPart 
{
   XmGadgetCache	cache_head;
   XmCacheCopyProc	cache_copy;
   XmCacheDeleteProc	cache_delete;
   int			(*cache_compare)();
} XmCacheClassPart, *XmCacheClassPartPtr;

#define XmInheritCachePart	((XmCacheClassPartPtr) _XtInherit)

typedef struct _XmGadgetCacheRef
{
   struct _XmGadgetCache	*next;
   struct _XmGadgetCache	*prev;
   int				ref_count;
   int				data;
} XmGadgetCacheRef, *XmGadgetCacheRefPtr;


/*  Gadget class structure  */

typedef Boolean (*XmVisualChangeProc)();  /* returns TRUE if redisplay */
/* XmGadget child           */
/* XmManagerWidget current  */
/* XmManagerWidget new      */

typedef struct _XmGadgetClassPart
{
   XtWidgetProc         border_highlight;
   XtWidgetProc         border_unhighlight;
   XmArmAndActivate     arm_and_activate;
   XmInputDisProc       input_dispatch;
   XmVisualChangeProc   visual_change;
   XmSyntheticResource * syn_resources;   
   int                  num_syn_resources;   
   XmCacheClassPartPtr	cache_part;
   caddr_t		extension;
} XmGadgetClassPart;


/*  Full class record declaration for Gadget class  */

typedef struct _XmGadgetClassRec
{
   RectObjClassPart  rect_class;
   XmGadgetClassPart gadget_class;
} XmGadgetClassRec;

#ifndef GADGET
externalref XmGadgetClassRec xmGadgetClassRec;
#endif


/*  The Gadget instance record  */

typedef struct _XmGadgetPart
{
   Dimension shadow_thickness;
   Dimension highlight_thickness;

   XtCallbackList help_callback;
   caddr_t        user_data;

   Boolean traversal_on;
   Boolean highlight_on_enter;
   Boolean have_traversal;

   unsigned char unit_type;
   unsigned char navigation_type;

   Boolean highlight_drawn;
   Boolean highlighted;
   Boolean visible;

   Mask event_mask;
} XmGadgetPart;

/*  Full instance record declaration  */

typedef struct _XmGadgetRec
{
   ObjectPart   object;
   RectObjPart  rectangle;
   XmGadgetPart gadget;
} XmGadgetRec;


/*  Masks to define input the gadget is interested in  */

#define XmNO_EVENT              0x000
#define XmENTER_EVENT           0x001
#define XmLEAVE_EVENT           0x002
#define XmFOCUS_IN_EVENT        0x004
#define XmFOCUS_OUT_EVENT       0x008
#define XmMOTION_EVENT          0x010
#define XmARM_EVENT             0x020
#define XmACTIVATE_EVENT        0x040
#define XmHELP_EVENT            0x080
#define XmKEY_EVENT             0x100
#define XmMULTI_ARM_EVENT       0x200
#define XmMULTI_ACTIVATE_EVENT  0x400
#define XmALL_EVENT             0x4FF


/* Gadget access Macros */
#define G_ShadowThickness(g) \
	(((XmGadget)(g))->gadget.shadow_thickness)
#define G_HighlightThickness(g) \
	(((XmGadget)(g))->gadget.highlight_thickness)



/************************************************************************
 *
 *  Manager class, instance and constraint definitions.  Manager #defines
 *  and internally used exported functions.
 *
 ************************************************************************/

/*  Traversal procedure definition  */

typedef void (*XmTraversalProc)();
/* Widget widget */
/* XPoint ul, lr */
/* int    dir    */

/*  Parent process procedure definition  */

typedef Boolean (*XmParentProcessProc)();
/* Widget widget */
/* XmParentProcessData data */

/*  Structure and defines for parent process data */

typedef struct
{ 
   XEvent *     event ;
   int          action ;
   String *     params ;
   Cardinal     num_params ;
} XmInputActionRec ;

typedef struct
{ 
   int          process_type ;
   union
   {  XmInputActionRec input_action ;
   }            data ;
} XmParentProcessDataRec, * XmParentProcessData ;

#define XmINPUT_ACTION	1

#define XmRETURN	1
#define XmCANCEL	2

/* Parent process entry point */

#ifdef _NO_PROTO
extern Boolean _XmParentProcess() ;
#else /* _NO_PROTO */
extern Boolean _XmParentProcess( Widget widget, XmParentProcessData data) ;
#endif /* _NO_PROTO */

/*  Structure used for storing accelerator and mnemonic information.  */

typedef struct 
{
   unsigned int eventType;
   KeySym       keysym;
   KeyCode      key;
   unsigned int modifiers;
   Widget       component;
   Boolean      needGrab;
   Boolean      isMnemonic;
} XmKeyboardData;


/*  The class definition  */

typedef struct _XmManagerClassPart
{
   String               translations;
   XmSyntheticResource * syn_resources;   
   int                  num_syn_resources;   
   XmSyntheticResource * syn_constraint_resources;   
   int                  num_syn_constraint_resources;   
   XmParentProcessProc  parent_process;
   caddr_t              extension;
} XmManagerClassPart;

typedef struct _XmManagerClassRec
{
    CoreClassPart       core_class;
    CompositeClassPart  composite_class;
    ConstraintClassPart constraint_class;
    XmManagerClassPart  manager_class;
} XmManagerClassRec;

#ifndef MANAGER
externalref XmManagerClassRec xmManagerClassRec;
#endif

/*  The instance definition  */

typedef struct _XmManagerPart
{
   Pixel   foreground;

   Dimension   shadow_thickness;
   Pixel   top_shadow_color;
   Pixmap  top_shadow_pixmap;
   Pixel   bottom_shadow_color;
   Pixmap  bottom_shadow_pixmap;

   Pixel   highlight_color;
   Pixmap  highlight_pixmap;

   XtCallbackList help_callback;
   caddr_t        user_data;

   Boolean traversal_on;
   unsigned char unit_type;
   unsigned char navigation_type;
   
   Boolean event_handler_added;
   Widget  active_child;
   Widget  highlighted_widget;
   Widget  accelerator_widget;

   Boolean has_focus;

   XmStringDirection string_direction;

   XmKeyboardData * keyboard_list;
   short num_keyboard_entries;
   short size_keyboard_list;

   XmGadget selected_gadget;
   XmGadget eligible_for_multi_button_event;

   GC      background_GC;
   GC      highlight_GC;
   GC      top_shadow_GC;
   GC      bottom_shadow_GC;
} XmManagerPart;

typedef struct _XmManagerRec
{
   CorePart       core;
   CompositePart  composite;
   ConstraintPart constraint;
   XmManagerPart  manager;
} XmManagerRec;


/*  The constraint definition  */

typedef struct _XmManagerConstraintPart
{
   int unused;
} XmManagerConstraintPart;

typedef struct _XmManagerConstraintRec
{
   XmManagerConstraintPart manager;
} XmManagerConstraintRec, * XmManagerConstraintPtr;


/*  Defines for the traversal and parent process procedure inheritance  */

#define XtInheritTraversalProc ((XmTraversalProc) _XtInherit)
#define XmInheritParentProcess ((XmParentProcessProc) _XtInherit)


/*  Manager exported functions  */

#ifdef NOCODE
extern void     _XmManagerEnter();         /* start traversal               */
extern void     _XmGadgetTraversePrevTabGroup() ;
extern void     _XmGadgetTraverseNextTabGroup() ;
extern void     _XmGadgetTraversePrev() ;
extern void     _XmGadgetTraverseNext() ;
extern void     _XmGadgetTraverseHome() ;
extern void     _XmManagerLeave();         /* end traversal                 */
extern void     _XmManagerFocusIn();       /* focus management              */
extern void     _XmManagerFocusOut();      /* focus management              */
extern void     _XmManagerVisibility();	   /* more focus management         */
extern void     _XmManagerUnmap();         /* even more focus management    */
extern Boolean  _XmFindTraversablePrim();  /* traversal checking routine    */
extern void     _XmReManageChildren();     /* rebuild Managed Children List */
extern Boolean  _XmGetFocusResetFlag();
extern void     _XmSetFocusResetFlag();
extern Boolean  _XmGetTransientFlag();
extern void     _XmSetTransientFlag();
extern void 	_XmDrawShadowType();
extern void 	_XmClearShadowType();
extern void 	_XmDestroyParentCallback();

#endif  /* NOCODE */

/* Access macros */

#define MGR_KeyboardList(m)        (((XmManagerRec *) (m))->manager.keyboard_list)
#define MGR_NumKeyboardEntries(m)  (((XmManagerRec *) (m))->manager.num_keyboard_entries)
#define MGR_SizeKeyboardList(m)    (((XmManagerRec *) (m))->manager.size_keyboard_list)
#define MGR_ShadowThickness(m)    (((XmManagerRec *) (m))->manager.shadow_thickness)


/*  GC access macros  */

#define XmParentTopShadowGC(w) (((XmManagerWidget)			\
	(((XmGadget)(w))->object.parent))->manager.top_shadow_GC)

#define XmParentBottomShadowGC(w) (((XmManagerWidget)			\
	(((XmGadget)(w))->object.parent))->manager.bottom_shadow_GC)

#define XmParentHighlightGC(w) (((XmManagerWidget)			\
	(((XmGadget)(w))->object.parent))->manager.highlight_GC)

#define XmParentBackgroundGC(w) (((XmManagerWidget)                     \
        (((XmGadget)(w))->object.parent))->manager.background_GC)


/***********************************************************************
 *
 * Simple Menu Structure
 *
 ***********************************************************************/

typedef struct _XmSimpleMenuRec {
	int count;
	int post_from_button;
	XtCallbackProc callback;
	XmStringTable label_string;
	String *accelerator;
	XmStringTable accelerator_text;
	XmKeySymTable mnemonic;
	XmStringCharSetTable mnemonic_charset;
	XmButtonTypeTable button_type;
	int button_set;
	XmString option_label;
        KeySym option_mnemonic;
} XmSimpleMenuRec, * XmSimpleMenu;

/****************
 *
 * Data structure for building a real translation table out of a 
 * virtual string.
 *
 ****************/
 typedef struct {
     Modifiers mod;
     char      *key;
     char      *action;
  } _XmBuildVirtualKeyStruct;
              
/***********************************************************************
 *
 * Types and functions for Geometry Utilities
 *
 ***********************************************************************/

typedef struct _XmKidGeometryRec
{
    Widget   kid;				/* ptr to kid */
    XtWidgetGeometry	box;			/* kid geo box */
} XmKidGeometryRec, *XmKidGeometry;

typedef struct
{   Boolean         end ;        /* Flag to mark end of rows.                */
    void            (*fix_up)() ;/* Used for non-ordinary layouts.           */
    Dimension       even_width ; /* If non-zero, set all boxes to same width.*/
    Dimension       even_height ;/* If non-zero, set all boxes to same height*/
    Dimension       min_height ; /* Minimum height, if stretch_height TRUE.  */
    Boolean         stretch_height ;/* Stretch height to fill vertically.    */
    Boolean         uniform_border ;/* Enforce on all kids this row, dflt F. */
    Dimension       border ;        /* Value to use if uniform_border set.   */
    unsigned char   fill_mode ; /* Possible values: XmGEO_PACK, XmGEO_CENTER,*/
				/*   or XmGEO_EXPAND (default).              */
    unsigned char   fit_mode ;  /* Method for fitting boxes into space,      */
                /* XmGEO_PROPORTIONAL (dflt), XmGEO_AVERAGING, or XmGEO_WRAP.*/
    Boolean         sticky_end ;  /* Last box in row sticks to edge, dflt F. */
    Dimension       space_above ; /* Between-line spacing, default 0.        */
    Dimension       space_end ;   /* End spacing (XmGEO_CENTER), default 0.  */
    Dimension       space_between ; /* Internal spacing, default 0.          */
    Dimension       max_box_height ;/* Set during arrange routine.           */
    Dimension       boxes_width ;   /* Set during arrange routine.           */
    Dimension       fill_width ;    /* Set during arrange routine.           */
    Dimension       box_count ;     /* Set during arrange routine.           */
    } XmGeoRowLayoutRec, * XmGeoRowLayout ;

typedef struct
{   Widget          composite ;     /* Widget managing layout.               */
    Widget          instigator ;    /* Widget initiating re-layout.          */
    XtWidgetGeometry instig_request ;/* Geometry layout request of instigatr.*/
    XtWidgetGeometry parent_request ;/* Subsequent layout request to parent. */
    XtWidgetGeometry * in_layout ;  /* Geo. of instig. in layout (after Get).*/
    XmKidGeometry   boxes ;/* Array of boxes, lines separated by NULL record.*/
    XmGeoRowLayout  layouts ;       /* Array of row format information.      */
    Dimension       margin_w ;/*Sum of margin, highlight, & shadow thickness.*/
    Dimension       margin_h ;/*Sum of margin, highlight, & shadow thickness.*/
    Boolean         stretch_boxes ; /* Set during arrange routine.           */
    Boolean         uniform_border ;/* Enforce on all kids, default FALSE.   */
    Dimension       border ;	    /* Value to use if uniform_border TRUE.  */
    Dimension       max_width ;     /* Set during arrange routine.           */
    Dimension       boxes_height ;  /* Set during arrange routine.           */
    Dimension       fill_height ;   /* Set during arrange routine.           */
    Dimension       width ;         /* Set during arrange routine.           */
    Dimension       height ;        /* Set during arrange routine.           */
    Boolean         (*set_except)() ;
    Boolean         (*almost_except)() ;
    Boolean         (*no_geo_request)() ;
    caddr_t         extension ;
    } XmGeoMatrixRec, *XmGeoMatrix ;

typedef XmGeoMatrix (*XmGeoCreateProc)() ;

#define XtInheritGeoMatrixCreate ((XmGeoCreateProc) _XtInherit)


typedef void (*XmFocusMovedProc)() ;

#define XtInheritFocusMovedProc ((XmFocusMovedProc) _XtInherit)

#ifdef _NO_PROTO

extern XtGeometryResult _XmHandleQueryGeometry() ;
extern XtGeometryResult _XmHandleGeometryManager() ;
extern void             _XmHandleSizeUpdate() ;
extern XmGeoMatrix _XmGeoMatrixAlloc() ;
extern Boolean _XmGeoSetupKid() ;
extern void _XmGeoMatrixGet() ;
extern void _XmGeoMatrixSet() ;
extern void _XmGeoAdjustBoxes() ;
extern void _XmGeoGetDimensions() ;
extern void _XmGeoArrangeBoxes() ;
extern Dimension _XmGeoBoxesSameWidth() ;
extern Dimension _XmGeoBoxesSameHeight() ;
extern void _XmSeparatorFix() ;
extern XmKidGeometry _XmGetKidGeo() ;
extern void _XmSetKidGeo() ;
extern Boolean _XmGeometryEqual() ;
extern Boolean _XmGeoReplyYes() ;
extern XtGeometryResult _XmMakeGeometryRequest() ;

#else /* _NO_PROTO */

extern XtGeometryResult _XmHandleQueryGeometry( Widget wid,
                         XtWidgetGeometry *intended, XtWidgetGeometry *desired,
			  unsigned char policy, XmGeoMatrix(*createMatrix)()) ;
extern XtGeometryResult _XmHandleGeometryManager( Widget wid,
			     Widget instigator, XtWidgetGeometry *desired,
			       XtWidgetGeometry *allowed, unsigned char policy,
		        XmGeoMatrix *cachePtr, XmGeoMatrix (*createMatrix)()) ;
extern void _XmHandleSizeUpdate( Widget wid, unsigned char policy, 
					     XmGeoMatrix (*createMatrix)()) ;
extern XmGeoMatrix _XmGeoMatrixAlloc( unsigned int numRows,
				 unsigned int numBoxes, unsigned int extSize) ;
extern Boolean _XmGeoSetupKid( XmKidGeometry geo, Widget kidWid) ;
extern void _XmGeoMatrixGet( XmGeoMatrix geoSpec, int geoType) ;
extern void _XmGeoMatrixSet( XmGeoMatrix geoSpec) ;
extern void _XmGeoAdjustBoxes( XmGeoMatrix geoSpec) ;
extern void _XmGeoGetDimensions( XmGeoMatrix geoSpec) ;
extern void _XmGeoArrangeBoxes( XmGeoMatrix geoSpec, Position x, Position y, 
                                                Dimension *pW, Dimension *pH) ;
extern Dimension _XmGeoBoxesSameWidth( XmKidGeometry rowPtr, Dimension width) ;
extern Dimension _XmGeoBoxesSameHeight( XmKidGeometry rowPtr,
                                                            Dimension height) ;
extern void _XmSeparatorFix( XmGeoMatrix geoSpec, int action,
                              XmGeoRowLayout layoutPtr, XmKidGeometry rowPtr) ;
extern XmKidGeometry _XmGetKidGeo( CompositeWidget c, Widget instigator,
               XtWidgetGeometry *request, int uniform_border, Dimension border,
                         int uniform_width_margins, int uniform_height_margins,
                                                   Widget help, int geo_type) ;
extern void _XmSetKidGeo( XmKidGeometry kg, Widget instigator) ;
extern Boolean _XmGeometryEqual( Widget wid, XtWidgetGeometry * geoA,
                                                     XtWidgetGeometry * geoB) ;
extern Boolean _XmGeoReplyYes( Widget wid, XtWidgetGeometry * desired,
					         XtWidgetGeometry * response) ;
extern XtGeometryResult _XmMakeGeometryRequest( Widget w,
                                                      XtWidgetGeometry *geom) ;
#endif /* _NO_PROTO */

/***********************************************************************
 *
 * Resolution independence functions
 *
 ***********************************************************************/


typedef void (*XmVoidProc)();


/* Various useful constant and macro definitions */
/* from /usr/include/X11/Misc.h */

/* LAST(Dimension) */
#define MAXDIMENSION	((1 << 31)-1)

#define Max(x, y)	(((x) > (y)) ? (x) : (y))
#define Min(x, y)	(((x) < (y)) ? (x) : (y))
#define AssignMax(x, y)	if ((y) > (x)) x = (y)
#define AssignMin(x, y)	if ((y) < (x)) x = (y)

/************************************************************************
 *
 *  Fast subclassing macros and definitions
 *
 ************************************************************************/
#include <Xm/BaseClassI.h>

#ifndef NO_FAST_EXT
# define _XmFastSubclassInit(wc, bit_field) \
{ XmBaseClassExt * _Xm_fastPtr;\
  if (_Xm_fastPtr = _XmGetBaseClassExtPtr(wc, XmQmotif)) \
    _XmSetFlagsBit((*_Xm_fastPtr)->flags, bit_field, 1); \
  }
# ifndef NO_FUNC_WORKAROUND
#   define _XmIsFastSubclass(wc, bit) _XmIsSlowSubclass(wc, bit)
# else
#   define _XmIsFastSubclass(wc, bit) \
      (_XmGetFlagsBit( ((*_XmGetBaseClassExtPtr((wc),XmQmotif))->flags), bit) == 1)
# endif /* NO_FUNC_WORKAROUND */

#else
# define _XmFastSubclassInit(wc, bit_field) \
   (wc->core_class.extension = (caddr_t)   \
    (((unsigned int) wc->core_class.extension) | bit_field))

# define _XmIsFastSubclass(wc, bit)  \
  ((wc)->core_class.extension & bit)
#endif

#define XmARROW_BUTTON_BIT			(32)
#define XmARROW_BUTTON_GADGET_BIT		(33)
#define XmBULLETIN_BOARD_BIT			(34)
#define XmCASCADE_BUTTON_BIT		(1)
#define XmCASCADE_BUTTON_GADGET_BIT	(2)
#define XmCOMMAND_BOX_BIT		(3)
#define XmDIALOG_SHELL_BIT		(4)
#define XmDRAWING_AREA_BIT			(35)
#define XmDRAWN_BUTTON_BIT		(14)
#define XmFILE_SELECTION_BOX_BIT		(36)
#define XmFORM_BIT			(6)
#define XmFRAME_BIT				(37)
#define XmGADGET_BIT			(8)
#define XmLABEL_BIT			(9)
#define XmLABEL_GADGET_BIT		(10)
#define XmLIST_BIT			(5)
#define XmMAIN_WINDOW_BIT		(11)
#define XmMANAGER_BIT			(12)
#define XmMENU_SHELL_BIT		(13)
#define XmMESSAGE_BOX_BIT			(38)
#define XmPRIMITIVE_BIT			(15)
#define XmPUSH_BUTTON_BIT		(16)
#define XmPUSH_BUTTON_GADGET_BIT	(17)
#define XmROW_COLUMN_BIT		(18)
#define XmSASH_BIT				(39)
#define XmSCALE_BIT				(40)
#define XmSCROLL_BAR_BIT			(19)
#define XmSCROLLED_WINDOW_BIT		(20)
#define XmSELECTION_BOX_BIT		(21)
#define XmSEPARATOR_BIT			(22)
#define XmSEPARATOR_GADGET_BIT		(23)
#define XmTEXT_BIT			(24)
#define XmTEXT_FIELD_BIT		(7)
#define XmTOGGLE_BUTTON_BIT		(25)
#define XmTOGGLE_BUTTON_GADGET_BIT	(26)
#define XmVPANED_BIT				(41)
#define XmVENDOR_SHELL_BIT			(42)

#undef XmIsBulletinBoard
#define XmIsBulletinBoard(w)  \
  (_XmIsFastSubclass(XtClass(w), XmBULLETIN_BOARD_BIT))

#undef XmIsCascadeButton
#define XmIsCascadeButton(w)  \
  (_XmIsFastSubclass(XtClass(w), XmCASCADE_BUTTON_BIT))

#undef XmIsCascadeButtonGadget
#define XmIsCascadeButtonGadget(w)  \
  (_XmIsFastSubclass(XtClass(w), XmCASCADE_BUTTON_GADGET_BIT))

#undef XmIsCommandBox
#define XmIsCommandBox(w)  \
  (_XmIsFastSubclass(XtClass(w), XmCOMMAND_BOX_BIT))

#undef XmIsDialogShell
#define XmIsDialogShell(w)  \
  (_XmIsFastSubclass(XtClass(w), XmDIALOG_SHELL_BIT))

#undef XmIsDrawnButton
#define XmIsDrawnButton(w)  \
  (_XmIsFastSubclass(XtClass(w), XmDRAWN_BUTTON_BIT))

#undef XmIsForm
#define XmIsForm(w)  \
  (_XmIsFastSubclass(XtClass(w), XmFORM_BIT))

#undef XmIsGadget
#define XmIsGadget(w)  \
  (_XmIsFastSubclass(XtClass(w), XmGADGET_BIT))

#undef XmIsLabel
#define XmIsLabel(w)  \
  (_XmIsFastSubclass(XtClass(w), XmLABEL_BIT))

#undef XmIsLabelGadget
#define XmIsLabelGadget(w)  \
  (_XmIsFastSubclass(XtClass(w), XmLABEL_GADGET_BIT))

#undef XmIsList
#define XmIsList(w)  \
  (_XmIsFastSubclass(XtClass(w), XmLIST_BIT))

#undef XmIsMainWindow
#define XmIsMainWindow(w)  \
  (_XmIsFastSubclass(XtClass(w), XmMAIN_WINDOW_BIT))

#undef XmIsManager
#define XmIsManager(w)  \
  (_XmIsFastSubclass(XtClass(w), XmMANAGER_BIT))

#undef XmIsMenuShell
#define XmIsMenuShell(w)  \
  (_XmIsFastSubclass(XtClass(w), XmMENU_SHELL_BIT))

#undef XmIsPrimitive
#define XmIsPrimitive(w)  \
  (_XmIsFastSubclass(XtClass(w), XmPRIMITIVE_BIT))

#undef XmIsPushButton
#define XmIsPushButton(w)  \
  (_XmIsFastSubclass(XtClass(w), XmPUSH_BUTTON_BIT))

#undef XmIsPushButtonGadget
#define XmIsPushButtonGadget(w)  \
  (_XmIsFastSubclass(XtClass(w), XmPUSH_BUTTON_GADGET_BIT))

#undef XmIsRowColumn
#define XmIsRowColumn(w)  \
  (_XmIsFastSubclass(XtClass(w), XmROW_COLUMN_BIT))

#undef XmIsScrolledWindow
#define XmIsScrolledWindow(w)  \
  (_XmIsFastSubclass(XtClass(w), XmSCROLLED_WINDOW_BIT))

#undef XmIsScrollBar
#define XmIsScrollBar(w)  \
  (_XmIsFastSubclass(XtClass(w), XmSCROLL_BAR_BIT))

#undef XmIsSelectionBox
#define XmIsSelectionBox(w)  \
  (_XmIsFastSubclass(XtClass(w), XmSELECTION_BOX_BIT))

#undef XmIsSeparator
#define XmIsSeparator(w)  \
  (_XmIsFastSubclass(XtClass(w), XmSEPARATOR_BIT))

#undef XmIsSeparatorGadget
#define XmIsSeparatorGadget(w)  \
  (_XmIsFastSubclass(XtClass(w), XmSEPARATOR_GADGET_BIT))

#undef XmIsText
#define XmIsText(w)  \
  (_XmIsFastSubclass(XtClass(w), XmTEXT_BIT))

#undef XmIsTextField
#define XmIsTextField(w)  \
  (_XmIsFastSubclass(XtClass(w), XmTEXT_FIELD_BIT))

#undef XmIsToggleButton
#define XmIsToggleButton(w)  \
  (_XmIsFastSubclass(XtClass(w), XmTOGGLE_BUTTON_BIT))

#undef XmIsToggleButtonGadget
#define XmIsToggleButtonGadget(w)  \
  (_XmIsFastSubclass(XtClass(w), XmTOGGLE_BUTTON_GADGET_BIT))



/************************************************************************
 *
 *  Defines and macros for the XmResolvePart function
 *
 ************************************************************************/

/*  Widget class indices used with XmPartOffset and XmField macros  */

#define XmObjectIndex 		0
#define ObjectIndex 		XmObjectIndex
#define XmRectObjIndex		(XmObjectIndex + 1)
#define RectObjIndex		XmRectObjIndex
#define XmWindowObjIndex	(XmRectObjIndex + 1)
#define WindowObjIndex		XmWindowObjIndex
#define XmCoreIndex 		0
#define CoreIndex 		XmCoreIndex
#define XmCompositeIndex 	(XmWindowObjIndex + 2)
#define CompositeIndex 		XmCompositeIndex
#define XmConstraintIndex 	(XmCompositeIndex + 1)
#define ConstraintIndex 	XmConstraintIndex
#define XmGadgetIndex	 	(XmRectObjIndex + 1)
#define XmPrimitiveIndex 	(XmWindowObjIndex + 2)
#define XmManagerIndex	 	(XmConstraintIndex + 1)

#define XmArrowBIndex		(XmPrimitiveIndex + 1)
#define XmArrowButtonIndex	XmArrowBIndex
#define XmLabelIndex		(XmPrimitiveIndex + 1)
#define XmListIndex		(XmPrimitiveIndex + 1)
#define XmScrollBarIndex	(XmPrimitiveIndex + 1)
#define XmSeparatorIndex	(XmPrimitiveIndex + 1)
#define XmTextIndex		(XmPrimitiveIndex + 1)

#define XmCascadeBIndex		(XmLabelIndex + 1)
#define XmCascadeButtonIndex	XmCascadeBIndex
#define XmDrawnBIndex		(XmLabelIndex + 1)
#define XmDrawnButtonIndex	XmDrawnBIndex
#define XmPushBIndex		(XmLabelIndex + 1)
#define XmPushButtonIndex	XmPushBIndex
#define XmToggleBIndex		(XmLabelIndex + 1)
#define XmToggleButtonIndex	XmToggleBIndex

#define XmArrowBGIndex		(XmGadgetIndex + 1)
#define XmArrowButtonGadgetIndex XmArrowBGIndex
#define XmLabelGIndex		(XmGadgetIndex + 1)
#define XmLabelGadgetIndex	XmLabelGIndex
#define XmSeparatoGIndex	(XmGadgetIndex + 1)
#define XmSeparatorGadgetIndex	XmSeparatoGIndex

#define XmCascadeBGIndex	(XmLabelGIndex + 1)
#define XmCascadeButtonGadgetIndex XmCascadeBGIndex
#define XmPushBGIndex		(XmLabelGIndex + 1)
#define XmPushButtonGadgetIndex	XmPushBGIndex
#define XmToggleBGIndex		(XmLabelGIndex + 1)
#define XmToggleButtonGadgetIndex XmToggleBGIndex

#define XmBulletinBIndex	(XmManagerIndex + 1)
#define XmBulletinBoardIndex	XmBulletinBIndex
#define XmDrawingAIndex		(XmManagerIndex + 1)
#define XmDrawingAreaIndex	XmDrawingAIndex
#define XmFrameIndex		(XmManagerIndex + 1)
#define XmPanedWIndex		(XmManagerIndex + 1)
#define XmPanedWindowIndex	XmPanedWIndex
#define XmRowColumnIndex	(XmManagerIndex + 1)
#define XmScaleIndex		(XmManagerIndex + 1)
#define XmScrolledWIndex	(XmManagerIndex + 1)
#define XmScrolledWindowIndex	XmScrolledWIndex

#define XmFormIndex		(XmBulletinBIndex + 1)
#define XmMessageBIndex		(XmBulletinBIndex + 1)
#define XmMessageBoxIndex	XmMessageBIndex
#define XmSelectioBIndex	(XmBulletinBIndex + 1)
#define XmSelectionBoxIndex	XmSelectioBIndex

#define XmMainWIndex		(XmScrolledWIndex + 1)
#define XmMainWindowIndex	XmMainWIndex

#define XmCommandIndex		(XmSelectioBIndex + 1)
#define XmFileSBIndex		(XmSelectioBIndex + 1)
#define XmFileSelectionBoxIndex	XmFileSBIndex

#define XmShellIndex 		(XmCompositeIndex + 1)
#define ShellIndex 		XmShellIndex
#define XmOverrideShellIndex 	(XmShellIndex + 1)
#define OverrideShellIndex 	XmOverrideShellIndex
#define XmWMShellIndex	 	(XmShellIndex + 1)
#define WMShellIndex	 	XmWMShellIndex
#define XmVendorShellIndex 	(XmWMShellIndex + 1)
#define VendorShellIndex 	XmVendorShellIndex
#define XmTransientShellIndex	(XmVendorShellIndex + 1)
#define TransientShellIndex	XmTransientShellIndex
#define XmTopLevelShellIndex 	(XmVendorShellIndex + 1)
#define TopLevelShellIndex 	XmTopLevelShellIndex
#define XmApplicationShellIndex (XmTopLevelShellIndex + 1)
#define ApplicationShellIndex 	XmApplicationShellIndex

#define XmDialogSIndex		(XmTransientShellIndex + 1)
#define XmDialogShellIndex	XmDialogSIndex
#define XmMenuShellIndex	(XmOverrideShellIndex + 1)


#define XmLONGBITS (sizeof(long) * 8)
#define XmHALFLONGBITS (XmLONGBITS/2)

typedef struct _XmPartResource {
    String     resource_name;	/* Resource name			    */
    String     resource_class;	/* Resource class			    */
    String     resource_type;	/* Representation type desired		    */
    Cardinal   resource_size;	/* Size in bytes of representation	    */
    Cardinal   resource_offset;	/* Index within & offset within part 	    */
    String     default_type;	/* representation type of specified default */
    caddr_t    default_addr;   	/* Address of default resource		    */
} XmPartResource;


#if defined(__STDC__) && !defined(UNIXCPP)
# define XmPartOffset(part, variable) \
        ((part##Index) << XmHALFLONGBITS) + XtOffset(part##Part *, variable)

# define XmConstraintPartOffset(part, variable) \
        ((part##Index) << XmHALFLONGBITS) + \
	XtOffset(part##ConstraintPart *, variable)

# define XmGetPartOffset(r, offset) \
       ((r)->resource_offset & 0xffff) + \
	(*(offset))[(r)->resource_offset >> XmHALFLONGBITS];

# define XmField(widget, offsetrecord, part, variable, type) \
	(*(type *)(((char *) (widget)) + offsetrecord[part##Index] + \
		XtOffset(part##Part *, variable)))

# define XmConstraintField(widget, offsetrecord, part, variable, type) \
	(*(type *)(((char *) (widget)->core.constraints) + \
	offsetrecord[part##Index] + \
	XtOffset(part##ConstraintPart *, variable)))
#else
# define XmPartOffset(part, variable) \
        ((part/**/Index) << XmHALFLONGBITS) + XtOffset(part/**/Part *, variable)

# define XmConstraintPartOffset(part, variable) \
        ((part/**/Index) << XmHALFLONGBITS) + \
	XtOffset(part/**/ConstraintPart *, variable)

# define XmGetPartOffset(r, offset) \
       ((r)->resource_offset & 0xffff) + \
	(*(offset))[(r)->resource_offset >> XmHALFLONGBITS];

# define XmField(widget, offsetrecord, part, variable, type) \
	(*(type *)(((char *) (widget)) + offsetrecord[part/**/Index] + \
	XtOffset(part/**/Part *, variable)))

# define XmConstraintField(widget, offsetrecord, part, variable, type) \
	(*(type *)(((char *) (widget)->core.constraints) + \
	offsetrecord[part/**/Index] + \
	XtOffset(part/**/ConstraintPart *, variable)))
#endif

/****************************************************************************
 * this suffix is added to dialog shells created by Xm convenience routines *
 * so that, for example, a call to create a form dialog named f generates a *
 * dialog shell named f_popup in addition to a form named f                 *
 ****************************************************************************/

#define DIALOG_SUFFIX "_popup"
#define DIALOG_SUFFIX_SIZE 6


/************************************************************************
 *                                                                      *
 * Defines for function protos...                                       *
 *                                                                      *
 ************************************************************************/

#ifdef _NO_PROTO
extern void _XmDoGadgetTraversal ();
extern void _XmGadgetTraversePrevTabGroup ();
extern void _XmGadgetTraverseNextTabGroup ();
extern void _XmGadgetTraverseLeft ();
extern void _XmGadgetTraverseRight ();
extern void _XmGadgetTraverseUp ();
extern void _XmGadgetTraverseDown ();
extern void _XmGadgetTraverseNext ();
extern void _XmGadgetTraversePrev ();
extern void _XmGadgetTraverseHome ();
extern void _XmGadgetSelect ();
extern void _XmGadgetButtonMotion ();
extern void _XmGadgetKeyInput ();
extern void _XmGadgetArm ();
extern void _XmGadgetActivate ();
extern void _XmManagerHelp ();
extern void _XmPrimitiveHelp ();
extern void _XmSocorro ();
extern void _XmGadgetMultiArm ();
extern void _XmGadgetMultiActivate ();
extern Boolean _XmGetTransientFlag ();
extern void _XmSetTransientFlag ();
extern Boolean _XmGetFocusResetFlag ();
extern void _XmSetFocusResetFlag ();
extern void _XmDrawShadowType ();
extern void _XmClearShadowType ();
extern void _XmDestroyParentCallback ();
extern void _XmTraverseLeft ();
extern void _XmTraverseRight ();
extern void _XmTraverseUp ();
extern void _XmTraverseDown ();
extern void _XmTraverseNext ();
extern void _XmTraversePrev ();
extern void _XmTraverseHome ();
extern void _XmTraverseNextTabGroup ();
extern void _XmTraversePrevTabGroup ();
extern Boolean _XmDifferentBackground ();
extern void _XmHighlightBorder ();
extern void _XmUnhighlightBorder ();
extern void _XmDrawBorder();
extern void _XmDrawShadow ();
extern void _XmEraseShadow ();
extern void _XmEnterGadget ();
extern void _XmLeaveGadget ();
extern void _XmFocusInGadget ();
extern void _XmFocusOutGadget ();
extern XmGadget _XmInputInGadget ();
extern XmGadget _XmInputForGadget ();
extern void _XmConfigureObject ();
extern void _XmResizeObject ();
extern void _XmMoveObject ();
extern void _XmRedisplayGadgets ();
extern void _XmDispatchGadgetInput ();
extern void _XmVirtualToActualKeysym();
extern Boolean _XmStringsAreEqual ();
extern char *_XmGetRealXlations();
extern void _XmNavigChangeManaged ();
extern void _XmSetDestination ();
#else /* _NO_PROTO */
extern void _XmDoGadgetTraversal (XmManagerWidget mw, XEvent *event, int direction);
extern void _XmGadgetTraversePrevTabGroup (XmManagerWidget mw, XEvent *event);
extern void _XmGadgetTraverseNextTabGroup (XmManagerWidget mw, XEvent *event);
extern void _XmGadgetTraverseLeft (XmManagerWidget mw, XEvent *event);
extern void _XmGadgetTraverseRight (XmManagerWidget mw, XEvent *event);
extern void _XmGadgetTraverseUp (XmManagerWidget mw, XEvent *event);
extern void _XmGadgetTraverseDown (XmManagerWidget mw, XEvent *event);
extern void _XmGadgetTraverseNext (XmManagerWidget mw, XEvent *event);
extern void _XmGadgetTraversePrev (XmManagerWidget mw, XEvent *event);
extern void _XmGadgetTraverseHome (XmManagerWidget mw, XEvent *event);
extern void _XmGadgetSelect (XmManagerWidget mw, XEvent *event);
extern void _XmGadgetButtonMotion (XmManagerWidget mw, XEvent *event);
extern void _XmGadgetKeyInput (XmManagerWidget mw, XEvent *event);
extern void _XmGadgetArm (XmManagerWidget mw, XButtonPressedEvent *event);
extern void _XmGadgetActivate (XmManagerWidget mw, XButtonPressedEvent *event);
extern void _XmManagerHelp (XmManagerWidget mw, XEvent *event);
extern void _XmPrimitiveHelp (XmPrimitiveWidget pw, XEvent *event);
extern void _XmSocorro (Widget w, XEvent *event);
extern void _XmGadgetMultiArm (XmManagerWidget mw, XButtonPressedEvent *event);
extern void _XmGadgetMultiActivate (XmManagerWidget mw, XButtonPressedEvent *event);
extern Boolean _XmGetTransientFlag (Widget w);
extern void _XmSetTransientFlag (Widget w, Boolean value);
extern Boolean _XmGetFocusResetFlag (Widget w);
extern void _XmSetFocusResetFlag (Widget w, Boolean value);
extern void _XmDrawShadowType (Widget w, unsigned int shadow_type, Dimension core_width, Dimension core_height, Dimension shadow_thickness, Dimension highlight_thickness, GC top_shadow_GC, GC bottom_shadow_GC);
extern void _XmClearShadowType (Widget w, Dimension old_width, Dimension old_height, Dimension old_shadow_thickness, Dimension old_highlight_thickness);
extern void _XmDestroyParentCallback (Widget w, caddr_t client_data, caddr_t call_data);
extern void _XmTraverseLeft (Widget w, XEvent *event);
extern void _XmTraverseRight (Widget w, XEvent *event);
extern void _XmTraverseUp (Widget w, XEvent *event);
extern void _XmTraverseDown (Widget w, XEvent *event);
extern void _XmTraverseNext (Widget w, XEvent *event);
extern void _XmTraversePrev (Widget w, XEvent *event);
extern void _XmTraverseHome (Widget w, XEvent *event);
extern void _XmTraverseNextTabGroup (Widget w, XEvent *event);
extern void _XmTraversePrevTabGroup (Widget w, XEvent *event);
extern Boolean _XmDifferentBackground (Widget w, Widget parent);
extern void _XmHighlightBorder (Widget w);
extern void _XmUnhighlightBorder (Widget w);
extern void _XmDrawBorder(Widget w, GC gc, Position x, Position y, 
	Dimension width, Dimension height, Dimension highlight_width);
extern void _XmDrawShadow (Display *display, Drawable d, GC top_GC, GC bottom_GC, int size, int x, int y, int width, int height);
extern void _XmEraseShadow (Display *display, Drawable d, int size, int x, int y, int width, int height);
extern void _XmEnterGadget (XmGadget gadget, XEvent *event);
extern void _XmLeaveGadget (XmGadget gadget, XEvent *event);
extern void _XmFocusInGadget (XmGadget gadget, XEvent *event);
extern void _XmFocusOutGadget (XmGadget gadget, XEvent *event);
extern XmGadget _XmInputInGadget (CompositeWidget cw, int x, int y);
extern XmGadget _XmInputForGadget (CompositeWidget cw, int x, int y);
extern void _XmConfigureObject (RectObj g, int x, int y, int width, int height, int border_width);
extern void _XmResizeObject (RectObj g, int width, int height, int border_width);
extern void _XmMoveObject (RectObj g, int x, int y);
extern void _XmRedisplayGadgets (CompositeWidget mw, XExposeEvent *event, Region region);
extern void _XmDispatchGadgetInput (XmGadget g, XEvent *event, Mask mask);
extern void _XmVirtualToActualKeysym(Display * dpy,
                                     KeySym virtKeysym, 
                                     KeySym *actualKeysymRtn, 
                                     Modifiers * modifiersRtn);
extern Boolean _XmStringsAreEqual (char *in_str, char *test_str);
extern char *  _XmGetRealXlations(Display *dpy, 
                                  _XmBuildVirtualKeyStruct *keys,
                                  int      num_keys);
extern void _XmNavigChangeManaged(Widget w);
extern void _XmSetDestination (Display *dpy, Widget w);
#endif /* _NO_PROTO */

/************************************************************************
 * defines needed for 3D visual enhancement of defaultButtonshadow and
 *  implementation of ToggleButton Indicatorsize.
 ************************************************************************/
#define XM_3D_ENHANCE_PIXEL 2
#define XM_DEFAULT_TOP_MARGIN 0
#define XM_DEFAULT_BOTTOM_MARGIN 0

#ifdef _NO_PROTO
void _XmDrawSquareButton ();
void _XmDrawDiamondButton ();
#else /* _NO_PROTO */
void _XmDrawSquareButton (Widget w, int x, int y, int size, GC topGC, GC bottomGC, GC centerGC, Boolean fill);
void _XmDrawDiamondButton (Widget tw, int x, int y, int size, GC topGC, GC bottomGC, GC centerGC, Boolean fill);
#endif /* _NO_PROTO */

#ifdef _NO_PROTO
void _XmRecordEvent ();
Boolean _XmIsEventUnique ();
#else /* _NO_PROTO */
void _XmRecordEvent (XEvent *event);
Boolean _XmIsEventUnique (XEvent *event);
#endif /* _NO_PROTO */



#ifdef _NO_PROTO
/*  Arrow rectangle generation function  */
extern void _XmGetArrowDrawRects ();
/*  Arrow rectangle manipulation function  */
extern void _XmOffsetArrow ();
#else /* _NO_PROTO */
/*  Arrow rectangle generation function  */

extern void _XmGetArrowDrawRects (int highlight_thickness, 
				  int shadow_thickness,
				  unsigned int direction, int core_width, 
				  int core_height, short *top_count, 
				  short *cent_count, short *bot_count, 
				  XRectangle **top, XRectangle **cent, 
				  XRectangle **bot);

/*  Arrow rectangle manipulation function  */

extern void _XmOffsetArrow (int diff_x, int diff_y, XRectangle *top, 
			    XRectangle *cent, XRectangle *bot, int top_count, 
			    int cent_count, int bot_count);
#endif /* _NO_PROTO */



#ifdef _NO_PROTO
Boolean _XmMatchBtnEvent ();
Boolean _XmMatchKeyEvent ();
#else /* _NO_PROTO */
Boolean _XmMatchBtnEvent (XButtonEvent *event, unsigned int eventType, unsigned int button, unsigned int modifiers);
Boolean _XmMatchKeyEvent (XKeyEvent *event, unsigned int eventType, unsigned int key, unsigned int modifiers);
#endif /* _NO_PROTO */

#ifdef _NO_PROTO
caddr_t _XmGetMenuProcContext();
#else /* _NO_PROTO */
caddr_t _XmGetMenuProcContext (void);
#endif /* _NO_PROTO */


#ifdef _NO_PROTO
Boolean _XmMapBtnEvent ();
Boolean _XmMapKeyEvent ();
#else /* _NO_PROTO */
Boolean _XmMapKeyEvent (String str, unsigned int *eventType, unsigned int *key, unsigned int *modifiers);
Boolean _XmMapBtnEvent (String str, unsigned int *eventType, unsigned int *button, unsigned int *modifiers);
#endif /* _NO_PROTO */

#ifdef _NO_PROTO
void _XmCreateMenuCursor ();
void _XmInitializeMenuCursor();
void _XmSaveMenuProcContext ();
#else /* _NO_PROTO */
void _XmCreateMenuCursor (Widget m);
void _XmInitializeMenuCursor (void);
void _XmSaveMenuProcContext (caddr_t address);
#endif /* _NO_PROTO */


#ifdef _NO_PROTO
Boolean _XmInstallPixmap ();
#else /* _NO_PROTO */
Boolean _XmInstallPixmap (Pixmap pixmap, Screen *screen, char *image_name, Pixel foreground, Pixel background);
#endif /* _NO_PROTO */

#ifdef _NO_PROTO
void _XmExtImportArgs ();
void _XmBuildExtResources ();
#else /* _NO_PROTO */
void _XmExtImportArgs (Widget w, ArgList args, Cardinal *num_args);
void _XmBuildExtResources (XmExtObjectClass wc);
#endif /* _NO_PROTO */

#endif /* _XmP_h */
/* DON'T ADD STUFF AFTER THIS #endif */
