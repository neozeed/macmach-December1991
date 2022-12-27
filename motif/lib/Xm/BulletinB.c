#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)BulletinB.c	3.30.1.3 91/03/21";
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

/* Include files:
*/
#include <Xm/BulletinBP.h>

#include <Xm/DialogS.h>
#include <Xm/MwmUtil.h>
#include <Xm/PushBG.h>
#include <Xm/PushB.h>
#include <Xm/LabelG.h>
#include <Xm/Text.h>
#include <Xm/AtomMgr.h>

#include "TraversalI.h"

/****************************************************************/
/* Local defines:
*/
#define STRING_CHARSET          "ISO8859-1"
#define	MARGIN_DEFAULT		10

#define WARN_RESIZE_POLICY	"Incorrect resize policy."
#define WARN_DIALOG_STYLE	"Incorrect dialog style."
#define WARN_SHADOW_TYPE	"Incorrect shadow type."
#define WARN_NO_DEFAULT_FONT    "Null font list (no vendor shell default)"

/*  External functions:
*/

extern void _XmClearBGCompatibility();
extern void _XmClearBCompatibility();

/*  Internal functions:
*/
#ifdef _NO_PROTO

static void             ClassPartInitialize() ;
static void             DialogStyleDefault() ;
static void             Initialize() ;
static void             Destroy() ;
static Boolean          SetValues() ;
static Boolean          SetValuesHook() ;
static void             HandleChangeManaged() ;
static void             HandleResize() ;
static XtGeometryResult HandleGeometryManager() ;
static void             CalcSize() ;
static Boolean          DoLayout() ;
static void             ChangeManaged() ;
static void	        UnmanageCallback() ;
static void             InsertChild() ;
static void             DeleteChild() ;
static Boolean          Overlap() ;
static XtGeometryResult GeometryManager() ;
static XtGeometryResult QueryGeometry() ;
static void             Resize() ;
static void             Redisplay() ;
static Boolean          BulletinBoardParentProcess() ;
static void             GetXFromShell() ;
static void             GetYFromShell() ;
static void             GetBorderWidthFromShell() ;
static void             UpdateWMShellTitle() ;
static Boolean          _XmStringSingleSegment() ;

#else /* _NO_PROTO */

static void             ClassPartInitialize( XmBulletinBoardWidgetClass class);
static void             DialogStyleDefault( Widget widget, int offset,
                                                            XrmValue * value) ;
static void             Initialize( XmBulletinBoardWidget request,
                                                   XmBulletinBoardWidget new) ;
static void             Destroy( XmBulletinBoardWidget bb) ;
static Boolean          SetValues( XmBulletinBoardWidget current, 
                                                 XmBulletinBoardWidget request,
                                                   XmBulletinBoardWidget new) ;
static Boolean          SetValuesHook( XmBulletinBoardWidget bb, ArgList args,
                                                         Cardinal * num_args) ;
static void             HandleChangeManaged( XmBulletinBoardWidget bbWid,
                                            XmGeoMatrix (*geoMatrixCreate)()) ;
static void             HandleResize( XmBulletinBoardWidget bbWid,
                                            XmGeoMatrix (*geoMatrixCreate)()) ;
static XtGeometryResult HandleGeometryManager( Widget instigator,
                          XtWidgetGeometry *desired, XtWidgetGeometry *allowed,
                                            XmGeoMatrix (*geoMatrixCreate)()) ;
static void             CalcSize( XmBulletinBoardWidget bb, 
                             Dimension * replyWidth, Dimension * replyHeight) ;
static Boolean          DoLayout( XmBulletinBoardWidget bb) ;
static void             ChangeManaged( XmBulletinBoardWidget bb) ;
static void	        UnmanageCallback( Widget w, caddr_t client_data,
                                                           caddr_t call_data) ;
static void             InsertChild( Widget child) ;
static void             DeleteChild( Widget child) ;
static Boolean          Overlap( XmBulletinBoardWidget bb, Widget w) ;
static XtGeometryResult GeometryManager( Widget w, XtWidgetGeometry * request,
                                                    XtWidgetGeometry * reply) ;
static XtGeometryResult QueryGeometry( XmBulletinBoardWidget bb,
                                                   XtWidgetGeometry * intended,
                                                  XtWidgetGeometry * desired) ;
static void             Resize( XmBulletinBoardWidget bb) ;
static void             Redisplay( XmBulletinBoardWidget bb, XEvent * event,
                                                               Region region) ;
static Boolean          BulletinBoardParentProcess( XmBulletinBoardWidget bb,
                                                   XmParentProcessData event) ;
static void             GetXFromShell( XmBulletinBoardWidget bb,
                                         XrmQuark resource, XtArgVal * value) ;
static void             GetYFromShell( XmBulletinBoardWidget bb, 
                                         XrmQuark resource, XtArgVal * value) ;
static void             GetBorderWidthFromShell( XmBulletinBoardWidget bb,
                                         XrmQuark resource, XtArgVal * value) ;
static void             UpdateWMShellTitle( XmBulletinBoardWidget wid) ;
static Boolean          _XmStringSingleSegment( XmString str,
                            char * * pTextOut, XmStringCharSet * pCharsetOut) ;
#endif /* _NO_PROTO */


/*  default translation table and action list  */

#ifndef MCCABE
static char defaultTranslations[] =
       "<Key>osfSelect:		ManagerGadgetSelect()\n\
	<Key>osfActivate: 	BulletinBoardReturn()\n\
        <Key>osfHelp:		ManagerGadgetHelp()\n\
	<Key>osfCancel:		BulletinBoardCancel()\n\
	~Shift ~Meta ~Alt <Key>Return:	BulletinBoardReturn()\n\
	~Shift ~Meta ~Alt <Key>space:	ManagerGadgetSelect()\n\
	<Key>:			ManagerGadgetKeyInput()\n\
	<BtnMotion>:	ManagerGadgetButtonMotion()\n\
	<Btn1Down>:	ManagerGadgetArm()\n\
	<Btn1Down>,<Btn1Up>:	ManagerGadgetActivate()\n\
	<Btn1Up>:	ManagerGadgetActivate()\n\
	<Btn1Down>(2+):	ManagerGadgetMultiArm()\n\
	<Btn1Up>(2+):	ManagerGadgetMultiActivate()";

static char defaultAccelerators[] =
	"\043override\n\
	<Key>osfActivate:	BulletinBoardReturn()\n\
	<Key>osfCancel:		BulletinBoardCancel()\n\
	~Shift ~Meta ~Alt <Key>Return:	BulletinBoardReturn()";
#else
static char defaultTranslations[] ;
static char defaultAccelerators[] ;
#endif

static XtActionsRec actionsList[] =
{
	{ "Enter",	(XtActionProc)	_XmManagerEnter },      /* Motif 1.0 */
	{ "FocusIn",	(XtActionProc)	_XmManagerFocusIn },    /* Motif 1.0 */
	{ "Arm",	(XtActionProc)	_XmGadgetArm  },        /* Motif 1.0 */
	{ "Activate",	(XtActionProc)	_XmGadgetActivate },    /* Motif 1.0 */
	{ "Help",	(XtActionProc)	_XmManagerHelp },       /* Motif 1.0 */
	{ "Return",	(XtActionProc)	_XmBulletinBoardReturn },/*Motif 1.0 */
	{ "BulletinBoardReturn", (XtActionProc)	_XmBulletinBoardReturn },
        { "BulletinBoardCancel", (XtActionProc)	_XmBulletinBoardCancel },
};



/*  Resource definitions for BulletinBoard
 */

static XmSyntheticResource syn_resources[] =
{
	{	XmNdialogTitle,
		sizeof (XmString),
		XtOffset (XmBulletinBoardWidget, bulletin_board.dialog_title),
		_XmBB_GetDialogTitle,
		NULL
	},
	{	XmNmarginWidth,
		sizeof (Dimension),
		XtOffset (XmBulletinBoardWidget, bulletin_board.margin_width),
		_XmFromHorizontalPixels,
		_XmToHorizontalPixels
	},

	{	XmNmarginHeight,
		sizeof (Dimension),
		XtOffset (XmBulletinBoardWidget, bulletin_board.margin_height),
		_XmFromVerticalPixels,
		_XmToVerticalPixels
	},

	{	XmNx,
		sizeof (Position),
		XtOffset (XmBulletinBoardWidget, core.x),
		GetXFromShell,
		NULL
	},

	{	XmNy,
		sizeof (Position),
		XtOffset (XmBulletinBoardWidget, core.y),
		GetYFromShell,
		NULL
	},

	{	XmNborderWidth,
		sizeof (Dimension),
		XtOffset (XmBulletinBoardWidget, core.border_width),
		GetBorderWidthFromShell,
		NULL
	},
};



static XtResource resources[] =
{
	{	XmNshadowType,
		XmCShadowType, XmRShadowType, sizeof (unsigned char),
		XtOffset (XmBulletinBoardWidget, bulletin_board.shadow_type),
		XmRImmediate, (caddr_t) XmSHADOW_OUT
	},

	{
		XmNshadowThickness,
		XmCShadowThickness, XmRHorizontalDimension, sizeof (Dimension),
		XtOffset (XmBulletinBoardWidget, manager.shadow_thickness),
		XmRImmediate, (caddr_t) 0
	},

	{	XmNmarginWidth,
		XmCMarginWidth, XmRHorizontalDimension, sizeof (Dimension),
		XtOffset (XmBulletinBoardWidget, bulletin_board.margin_width),
		XmRImmediate, (caddr_t) MARGIN_DEFAULT
	},

	{	XmNmarginHeight,
		XmCMarginHeight, XmRVerticalDimension, sizeof (Dimension),
		XtOffset (XmBulletinBoardWidget, bulletin_board.margin_height),
		XmRImmediate, (caddr_t) MARGIN_DEFAULT
	},

	{	XmNdefaultButton,
		XmCWidget, XmRWindow, sizeof (Widget), 
		XtOffset (XmBulletinBoardWidget, bulletin_board.default_button),
		XmRImmediate, (caddr_t) NULL
	},

	{	XmNcancelButton,
		XmCWidget, XmRWindow, sizeof (Widget),
		XtOffset (XmBulletinBoardWidget, bulletin_board.cancel_button),
		XmRImmediate, (caddr_t) NULL
	},

	{	XmNfocusCallback,
		XmCCallback, XmRCallback, sizeof (XtCallbackList),
		XtOffset (XmBulletinBoardWidget, bulletin_board.focus_callback),
		XmRImmediate, (caddr_t) NULL
	},
#ifdef BB_HAS_LOSING_FOCUS_CB
	{	XmNlosingFocusCallback,
		XmCCallback, XmRCallback, sizeof (XtCallbackList),
		XtOffset (XmBulletinBoardWidget, 
                                         bulletin_board.losing_focus_callback),
		XmRImmediate, (caddr_t) NULL
	},
#endif
	{	XmNmapCallback,
		XmCCallback, XmRCallback, sizeof (XtCallbackList),
		XtOffset (XmBulletinBoardWidget, bulletin_board.map_callback),
		XmRImmediate, (caddr_t) NULL
	},

	{	XmNunmapCallback,
		XmCCallback, XmRCallback, sizeof (XtCallbackList),
		XtOffset (XmBulletinBoardWidget, bulletin_board.unmap_callback),
		XmRImmediate, (caddr_t) NULL
	},

	{	XmNbuttonFontList,
		XmCButtonFontList, XmRFontList, sizeof (XmFontList),
		XtOffset (XmBulletinBoardWidget, bulletin_board.button_font_list),
		XmRFontList, (caddr_t) NULL
	},

	{	XmNlabelFontList,
		XmCLabelFontList, XmRFontList, sizeof (XmFontList),
		XtOffset (XmBulletinBoardWidget, bulletin_board.label_font_list),
		XmRFontList, (caddr_t) NULL
	},

	{	XmNtextFontList,
		XmCTextFontList, XmRFontList, sizeof (XmFontList),
		XtOffset (XmBulletinBoardWidget, bulletin_board.text_font_list),
		XmRFontList, (caddr_t) NULL
	},

        {       XmNaccelerators,
                XmCAccelerators, XmRAcceleratorTable, sizeof(XtAccelerators), 
                XtOffset( XmBulletinBoardWidget, core.accelerators),
                XmRString, (XtPointer) defaultAccelerators
        },

	{	XmNtextTranslations,
		XmCTranslations, XmRTranslationTable, sizeof (XtTranslations),
		XtOffset (XmBulletinBoardWidget, bulletin_board.text_translations),
		XmRImmediate, (caddr_t) NULL
	},

	{	XmNallowOverlap,
		XmCAllowOverlap, XmRBoolean, sizeof (Boolean),
		XtOffset (XmBulletinBoardWidget, bulletin_board.allow_overlap),
		XmRImmediate, (caddr_t) True
	},

	{	XmNautoUnmanage,
		XmCAutoUnmanage, XmRBoolean, sizeof (Boolean),
		XtOffset (XmBulletinBoardWidget, bulletin_board.auto_unmanage),
		XmRImmediate, (caddr_t) True
	},

	{	XmNdefaultPosition,
		XmCDefaultPosition, XmRBoolean, sizeof (Boolean),
		XtOffset (XmBulletinBoardWidget, bulletin_board.default_position),
		XmRImmediate, (caddr_t) True
	},

	{	XmNresizePolicy,
		XmCResizePolicy, XmRResizePolicy, sizeof (unsigned char),
		XtOffset (XmBulletinBoardWidget, bulletin_board.resize_policy),
		XmRImmediate, (caddr_t) XmRESIZE_ANY
	},

	{	XmNnoResize,
		XmCNoResize, XmRBoolean, sizeof (Boolean),
		XtOffset (XmBulletinBoardWidget, bulletin_board.no_resize),
		XmRImmediate, (caddr_t) False
	},

	{	XmNdialogStyle,
		XmCDialogStyle, XmRDialogStyle, sizeof (unsigned char),
		XtOffset (XmBulletinBoardWidget, bulletin_board.dialog_style),
		XmRCallProc, (caddr_t) DialogStyleDefault
	},

	{	XmNdialogTitle,
		XmCDialogTitle, XmRXmString, sizeof (XmString),
		XtOffset (XmBulletinBoardWidget, bulletin_board.dialog_title),
		XmRString, (caddr_t) NULL
	},
};



/****************************************************************
 *
 *	BulletinBoard class record
 *
 ****************************************************************/

externaldef( xmbulletinboardclassrec) XmBulletinBoardClassRec
                                                      xmBulletinBoardClassRec =
{
   {			/* core_class fields      */
      (WidgetClass) &xmManagerClassRec,		/* superclass         */
      "XmBulletinBoard",			/* class_name         */
      sizeof(XmBulletinBoardRec),		/* widget_size        */
      NULL,					/* class_initialize   */
      ClassPartInitialize,			/* class_part_init    */
      FALSE,					/* class_inited       */
      Initialize,				/* initialize         */
      NULL,					/* initialize_hook    */
      _XtInherit,				/* realize            */
      actionsList,				/* actions	      */
      XtNumber(actionsList),			/* num_actions	      */
      resources,				/* resources          */
      XtNumber(resources),			/* num_resources      */
      NULLQUARK,				/* xrm_class          */
      TRUE,					/* compress_motion    */
      XtExposeCompressMaximal,			/* compress_exposure  */
      TRUE,					/* compress_enterlv   */
      FALSE,					/* visible_interest   */
      (XtWidgetProc) Destroy,                   /* destroy            */
      Resize,					/* resize             */
      (XtExposeProc) Redisplay,			/* expose             */
      (XtSetValuesFunc) SetValues,		/* set_values         */
      SetValuesHook,				/* set_values_hook    */
      _XtInherit,				/* set_values_almost  */
      NULL,					/* get_values_hook    */
      NULL,					/* accept_focus       */
      XtVersion,				/* version            */
      NULL,					/* callback_private   */
      defaultTranslations,                      /* tm_table           */
      (XtGeometryHandler) QueryGeometry,	/* query_geometry     */
      NULL,             	                /* display_accelerator*/
      NULL,		                        /* extension          */
   },

   {		/* composite_class fields */
      (XtGeometryHandler) GeometryManager,    	/* geometry_manager   */
      (XtWidgetProc) ChangeManaged,		/* change_managed     */
      InsertChild,				/* insert_child       */
      DeleteChild,				/* delete_child       */
      NULL,                                     /* extension          */
   },

   {		/* constraint_class fields */
      NULL,					/* resource list        */   
      0,					/* num resources        */   
      0,					/* constraint size      */   
      NULL,					/* init proc            */   
      NULL,             			/* destroy proc         */   
      NULL,					/* set values proc      */   
      NULL,                                     /* extension            */
   },

   {		/* manager_class fields */
      XtInheritTranslations,			/* default translations   */
      syn_resources,				/* syn_resources      	  */
      XtNumber (syn_resources),			/* num_syn_resources 	  */
      NULL,					/* syn_cont_resources     */
      0,					/* num_syn_cont_resources */
      BulletinBoardParentProcess,               /* parent_process         */
      NULL,					/* extension		  */
   },

   {		/* bulletinBoard class fields */     
      FALSE,                                    /*always_install_accelerators*/
      NULL,                                     /* geo_matrix_create */
      _XmBulletinBoardFocusMoved,               /* focus_moved_proc */
      NULL					/* extension */
   }
};

externaldef( xmbulletinboardwidgetclass) WidgetClass xmBulletinBoardWidgetClass
                                     = (WidgetClass) &xmBulletinBoardClassRec ;


/****************************************************************/
static void
#ifdef _NO_PROTO
ClassPartInitialize( class)
            XmBulletinBoardWidgetClass class ;
#else
ClassPartInitialize(
            XmBulletinBoardWidgetClass class)
#endif
/****************
 * 
 ****************/
{
            XmBulletinBoardWidgetClass super
                  = (XmBulletinBoardWidgetClass) class->core_class.superclass ;
/****************/

    _XmFastSubclassInit (class, XmBULLETIN_BOARD_BIT);

    if(    class->bulletin_board_class.geo_matrix_create
                                               == XtInheritGeoMatrixCreate    )
    {   class->bulletin_board_class.geo_matrix_create
                              = super->bulletin_board_class.geo_matrix_create ;
        } 
    if(    class->bulletin_board_class.focus_moved_proc
                                                == XtInheritFocusMovedProc    )
    {   class->bulletin_board_class.focus_moved_proc
                               = super->bulletin_board_class.focus_moved_proc ;
        } 
    return ;
    }

/****************************************************************/
static void
#ifdef _NO_PROTO
DialogStyleDefault( widget, offset, value)
            Widget          widget ;
            int             offset ;
            XrmValue *      value ;
#else
DialogStyleDefault(
            Widget          widget,
            int             offset,
            XrmValue *      value)
#endif
/****************           ARGSUSED
 * Set the default style to modeless if the parent is a 
 *   DialogShell, otherwise set to workarea.
 ****************/
{   
    static unsigned char    style ;
/****************/

    style = XmDIALOG_WORK_AREA ;	

    if(    XmIsDialogShell (XtParent (widget))    )
    {   style = XmDIALOG_MODELESS ;
        } 
    value->addr = (caddr_t) (&style) ;

    return ;
    }

/****************************************************************/
static void
#ifdef _NO_PROTO
Initialize( request, new)
            XmBulletinBoardWidget request ;
            XmBulletinBoardWidget new ;
#else
Initialize(
            XmBulletinBoardWidget request,
            XmBulletinBoardWidget new)
#endif
/****************
 * Initialize a BulletinBoard instance.
 ****************/
{   
            Arg             al[5] ;
    register Cardinal       ac ;
            unsigned int    mwm_functions ;
            char *          text_value ;
            XmFontList      defaultFL ;
            int             mwmStyle ;
            Dimension       shadowTh ;
            Dimension       dbShadowTh ;
            Widget          ancestor ;
            XmWidgetExtData extData ;
            XmBulletinBoardWidgetClass bbClass ;
/****************/

    new->bulletin_board.in_set_values = False ;
    new->bulletin_board.descendant_focus = (Widget) NULL ;
    new->bulletin_board.geo_cache = (XmGeoMatrix) NULL ;
    new->bulletin_board.initial_focus = TRUE ;

    /*	Copy font list data.
    */
    defaultFL = BB_ButtonFontList( new) ;
    if(    !defaultFL    )
    {   defaultFL = _XmGetDefaultFontList( (Widget) new, XmBUTTON_FONTLIST) ;
        } 
    if(    defaultFL    )
    {   BB_ButtonFontList( new) = XmFontListCopy( defaultFL) ;
        } 
    else
    {   _XmWarning( (Widget) new, WARN_NO_DEFAULT_FONT) ;
        } 

    defaultFL = BB_LabelFontList( new) ;
    if(    !defaultFL    )
    {   defaultFL = _XmGetDefaultFontList( (Widget) new, XmLABEL_FONTLIST) ;
        } 
    if(    defaultFL    )
    {   BB_LabelFontList( new) = XmFontListCopy( defaultFL) ;
        } 
    else
    {   _XmWarning( (Widget) new, WARN_NO_DEFAULT_FONT) ;
        } 

    defaultFL = BB_TextFontList( new) ;
    if(    !defaultFL    )
    {   defaultFL = _XmGetDefaultFontList( (Widget) new, XmTEXT_FONTLIST) ;
        } 
    if(    defaultFL    )
    {   BB_TextFontList( new) = XmFontListCopy( defaultFL) ;
        } 
    else
    {   _XmWarning( (Widget) new, WARN_NO_DEFAULT_FONT) ;
        } 

    if(    (request->manager.shadow_thickness == 0)
        && XtIsShell( XtParent( request))    )
    {   new->manager.shadow_thickness = 1 ;
        }
    /* Default and Cancel buttons are Set/Get resources only.  The
    *   DefaultButton field is used as a marker for the state of buttons
    *   in the child list.  As such, it is essential that it is NULL at
    *   the end of Initialize.
    */
    BB_DefaultButton( new) = NULL ;
    BB_CancelButton( new) = NULL ;

    /* The DynamicButton fields are managed and will be set appropriately
    *   in the focus moved callback routine.
    */
    BB_DynamicDefaultButton( new) = NULL ;
    BB_DynamicCancelButton( new) = NULL ;

    /*	Make sure height and width are not zero.
    */
    if(    new->core.width == 0    )
    {   new->core.width = 1 ;
        } 
    if(    new->core.height == 0    )
    {   new->core.height = 1 ;
        } 
    new->bulletin_board.old_shadow_thickness = 0 ;

    if(    request->bulletin_board.dialog_title    )
    {   
        new->bulletin_board.dialog_title = XmStringCopy( 
                                        request->bulletin_board.dialog_title) ;
        UpdateWMShellTitle( new) ;
        }
    /*	Set parent attributes.
    */
    ac = 0 ;
    text_value = NULL ;

    /*	Turn off window manager resize.
    */
    if(    request->bulletin_board.no_resize
        && XmIsVendorShell( XtParent( request))    ) 
    {   
        mwm_functions = (XmIsDialogShell (XtParent( request)))
                                   ? (MWM_FUNC_MOVE | MWM_FUNC_CLOSE)
                                       : (MWM_FUNC_MAXIMIZE | MWM_FUNC_MINIMIZE
                                            | MWM_FUNC_MOVE | MWM_FUNC_CLOSE) ;
        XtSetArg( al[ac], XmNmwmFunctions, mwm_functions) ;  ac++ ;
        }
    /* If parent is DialogShell, set dialog attributes and realize.
    */
    if(    XmIsDialogShell (XtParent (request))    )
    {   
        new->bulletin_board.shell = XtParent( request) ;

        switch(    request->bulletin_board.dialog_style    )
        {   
            case XmDIALOG_PRIMARY_APPLICATION_MODAL:
            {   mwmStyle = MWM_INPUT_PRIMARY_APPLICATION_MODAL ;
                break ;
                } 
            case XmDIALOG_FULL_APPLICATION_MODAL:
            {   mwmStyle = MWM_INPUT_FULL_APPLICATION_MODAL ;
                break; 
                } 
            case XmDIALOG_SYSTEM_MODAL:
            {   mwmStyle = MWM_INPUT_SYSTEM_MODAL ;
                break ;
                } 
            case XmDIALOG_MODELESS:
            default:
            {   mwmStyle = MWM_INPUT_MODELESS ;
                break ;
                } 
            } 
        XtSetArg( al[ac], XmNmwmInputMode, mwmStyle) ;  ac++ ;
        XtSetValues( new->bulletin_board.shell, al, ac) ;
        XtRealizeWidget( new->bulletin_board.shell) ;
        } 
    else
    {   new->bulletin_board.shell = NULL ;
        if(    ac    )
        {   XtSetValues( XtParent (request), al, (Cardinal) 1) ;
            } 
        }
    XtFree( text_value) ;

    /*	Validate shadow type
    */
    if(    (request->bulletin_board.shadow_type != XmSHADOW_IN)
        && (request->bulletin_board.shadow_type != XmSHADOW_OUT)
        && (request->bulletin_board.shadow_type != XmSHADOW_ETCHED_IN)
        && (request->bulletin_board.shadow_type != XmSHADOW_ETCHED_OUT)    ) 
    {   
        _XmWarning( (Widget) new, WARN_SHADOW_TYPE) ;
        new->bulletin_board.shadow_type = XmSHADOW_OUT ;
        }
    /*	Validate resize policy.
    */
    if(    (request->bulletin_board.resize_policy != XmRESIZE_NONE)
        && (request->bulletin_board.resize_policy != XmRESIZE_ANY)
        && (request->bulletin_board.resize_policy != XmRESIZE_GROW)    ) 
    {   
        _XmWarning( (Widget) new, WARN_RESIZE_POLICY) ;		
        new->bulletin_board.resize_policy = XmRESIZE_ANY ;
        }
    /*	Validate dialog style.
    */
    if(    !new->bulletin_board.shell
        && (request->bulletin_board.dialog_style != XmDIALOG_WORK_AREA)    ) 
    {   
        _XmWarning( (Widget) new, WARN_DIALOG_STYLE) ;
        new->bulletin_board.dialog_style = XmDIALOG_WORK_AREA ;
        }
    else
    {   if(    new->bulletin_board.shell
            && (request->bulletin_board.dialog_style != XmDIALOG_MODELESS)
            && (request->bulletin_board.dialog_style != XmDIALOG_SYSTEM_MODAL)
            && (request->bulletin_board.dialog_style
                                         != XmDIALOG_PRIMARY_APPLICATION_MODAL)
            && (request->bulletin_board.dialog_style
                                       != XmDIALOG_FULL_APPLICATION_MODAL)    )
        {   _XmWarning( (Widget) new, WARN_DIALOG_STYLE) ;		
            new->bulletin_board.dialog_style = XmDIALOG_MODELESS ;
            }
        } 
    /* Set source widget for accelerators used 
    *   by Manager in ConstraintInitialize.
    */
    if(    new->core.accelerators    )
    {   new->manager.accelerator_widget = (Widget) new ;
        } 

    bbClass = (XmBulletinBoardWidgetClass) new->core.widget_class ;

    if(    bbClass->bulletin_board_class.focus_moved_proc    )
    {   
        /* Walk up hierarchy to find vendor shell.  Then setup moved focus
        *   callback so default button can be maintained.
        */
        ancestor = XtParent( new) ;
        while(    ancestor  &&  !XmIsVendorShell( ancestor)    )
        {   ancestor = XtParent( ancestor) ;
            } 
        if(    ancestor
            && (extData = _XmGetWidgetExtData( ancestor, XmSHELL_EXTENSION))  )
        {   
            XtAddCallback( extData->widget, XmNfocusMovedCallback,
               (XtCallbackProc) bbClass->bulletin_board_class.focus_moved_proc,
                                                             (XtPointer) new) ;
            } 
        } 
    return ;
    }

/****************************************************************/
static void
#ifdef _NO_PROTO
Destroy( bb)
            XmBulletinBoardWidget bb ;
#else
Destroy(
            XmBulletinBoardWidget bb)
#endif
/****************
 * Free fontlists, remove callbacks.
 ****************/
{   
            Widget          ancestor ;
            XmWidgetExtData extData ;
            XmBulletinBoardWidgetClass bbClass ;
/****************/

    XmStringFree( bb->bulletin_board.dialog_title) ;

    /*  Free geometry cache.
    */
    if(    bb->bulletin_board.geo_cache    )
    {   XtFree( bb->bulletin_board.geo_cache) ;
        } 
    /*	Free fontlists.
    */
    if(    bb->bulletin_board.button_font_list    )
    {   XmFontListFree( bb->bulletin_board.button_font_list) ;
        } 
    if(    bb->bulletin_board.label_font_list    )
    {   XmFontListFree( bb->bulletin_board.label_font_list) ;
        } 
    if(    bb->bulletin_board.text_font_list    )
    {   XmFontListFree( bb->bulletin_board.text_font_list) ;
        } 

    bbClass = (XmBulletinBoardWidgetClass) bb->core.widget_class ;

    if(    bbClass->bulletin_board_class.focus_moved_proc    )
    {   
        /* Walk up hierarchy to find vendor shell.  Then remove focus moved
        *   callback.
        */
        ancestor = XtParent( bb) ;
        while(    ancestor  &&  !XmIsVendorShell( ancestor)    )
        {   ancestor = XtParent( ancestor) ;
            } 
        if(    ancestor
            && !ancestor->core.being_destroyed
            && (extData = _XmGetWidgetExtData( ancestor, XmSHELL_EXTENSION))  )
        {   
            XtRemoveCallback( extData->widget, XmNfocusMovedCallback,
               (XtCallbackProc) bbClass->bulletin_board_class.focus_moved_proc,
                                                              (XtPointer) bb) ;
            } 
        } 
    return ;
    }

/****************************************************************/
static Boolean
#ifdef _NO_PROTO
SetValues( current, request, new)
            XmBulletinBoardWidget current ;
            XmBulletinBoardWidget request ;
            XmBulletinBoardWidget new ;
#else
SetValues(
            XmBulletinBoardWidget current,
            XmBulletinBoardWidget request,
            XmBulletinBoardWidget new)
#endif
/****************
 * Modify attributes of a BulletinBoard instance.
 ****************/
{   
            Boolean	    newbfonts = False ;
            Boolean	    newlfonts = False ;
            Boolean	    newtfonts = False ;
            XmFontList	    bfonts, lfonts, tfonts ;
    register int            i, max ;
            Widget *        kid ;
            Arg		    al[10] ;
    register Cardinal       ac ;
            unsigned int    mwm_functions ;
            int             mwmStyle ;
            unsigned int    numChildren ;
            unsigned int    childIndex ;
            Widget *        childList ;
/****************/

    current->bulletin_board.in_set_values = True ;

    /*	Make sure height and width are not zero.
    */
    if(    new->core.width == 0    )
    {   new->core.width = 1 ;
        } 
    if(    new->core.height == 0    )
    {   new->core.height = 1 ;
        } 

    /*	Validate shadow type
    */
    if(    request->bulletin_board.shadow_type != XmSHADOW_IN &&
           request->bulletin_board.shadow_type != XmSHADOW_OUT &&
           request->bulletin_board.shadow_type != XmSHADOW_ETCHED_IN &&
	   request->bulletin_board.shadow_type != XmSHADOW_ETCHED_OUT    ) 
    {   
        _XmWarning( (Widget) new, WARN_SHADOW_TYPE) ;		
        new->bulletin_board.shadow_type = current->bulletin_board.shadow_type ;
        }
    /*	Validate resize policy.
    */
    if(    request->bulletin_board.resize_policy != XmRESIZE_NONE &&
	   request->bulletin_board.resize_policy != XmRESIZE_ANY &&
	   request->bulletin_board.resize_policy != XmRESIZE_GROW    ) 
    {   
        _XmWarning( (Widget) new, WARN_RESIZE_POLICY) ;		
        new->bulletin_board.resize_policy
                                      = current->bulletin_board.resize_policy ;
        }
    /*	Validate dialog style.
    */
    if(    !new->bulletin_board.shell &&
	   request->bulletin_board.dialog_style != XmDIALOG_WORK_AREA    ) 
    {   
        _XmWarning( (Widget) new, WARN_DIALOG_STYLE) ;
        new->bulletin_board.dialog_style 
                                       = current->bulletin_board.dialog_style ;
        }
    else
    {   if(    new->bulletin_board.shell
            && (request->bulletin_board.dialog_style != XmDIALOG_MODELESS)
            && (request->bulletin_board.dialog_style != XmDIALOG_SYSTEM_MODAL)
	    && (request->bulletin_board.dialog_style
                                         != XmDIALOG_PRIMARY_APPLICATION_MODAL)
	    && (request->bulletin_board.dialog_style
                                       != XmDIALOG_FULL_APPLICATION_MODAL)    )
        {   _XmWarning( (Widget) new, WARN_DIALOG_STYLE) ;
            new->bulletin_board.dialog_style 
                                       = current->bulletin_board.dialog_style ;
            }
        } 
    /*	Update shell attributes.
    */
    if(    new->bulletin_board.dialog_title !=
                                      current->bulletin_board.dialog_title    )
    {   /* Update window manager title.
        */
        XmStringFree( current->bulletin_board.dialog_title) ;
        new->bulletin_board.dialog_title = XmStringCopy(
                                        request->bulletin_board.dialog_title) ;
        UpdateWMShellTitle( new) ;
        }
    if(    new->bulletin_board.shell    )
    {   
        ac = 0 ;
        /*	Turn window manager resize on or off.
        */
        if(    new->bulletin_board.no_resize != current->bulletin_board.no_resize    )
        {   
            if(    new->bulletin_board.no_resize    )
            {   mwm_functions = (XmIsDialogShell (XtParent (request)))
                                   ? (MWM_FUNC_MOVE | MWM_FUNC_CLOSE)
                                       : (MWM_FUNC_MAXIMIZE | MWM_FUNC_MINIMIZE
                                            | MWM_FUNC_MOVE | MWM_FUNC_CLOSE) ;
                } 
            else
            {   mwm_functions = (XmIsDialogShell (XtParent (request)))
                           ? (MWM_FUNC_MOVE | MWM_FUNC_CLOSE | MWM_FUNC_RESIZE)
                                       : (MWM_FUNC_MAXIMIZE | MWM_FUNC_MINIMIZE
                                               | MWM_FUNC_MOVE | MWM_FUNC_CLOSE
                                                           | MWM_FUNC_RESIZE) ;
                } 
            XtSetArg( al[ac], XmNmwmFunctions, mwm_functions) ;  ac++ ;
            }
        if(    new->bulletin_board.dialog_style
                                   != current->bulletin_board.dialog_style    )
        {   switch(    request->bulletin_board.dialog_style    )
            {   
                case XmDIALOG_PRIMARY_APPLICATION_MODAL:
                {   mwmStyle = MWM_INPUT_PRIMARY_APPLICATION_MODAL ;
                    break ;
                    } 
                case XmDIALOG_FULL_APPLICATION_MODAL:
                {   mwmStyle = MWM_INPUT_FULL_APPLICATION_MODAL ;
                    break ; 
                    } 
                case XmDIALOG_SYSTEM_MODAL:
                {   mwmStyle = MWM_INPUT_SYSTEM_MODAL ;
                    break ;
                    } 
                case XmDIALOG_MODELESS:
                default:
                {   mwmStyle = MWM_INPUT_MODELESS ;
                    break ;
                    } 
                } 
            XtSetArg( al[ac], XmNmwmInputMode, mwmStyle) ; ac++ ;
            }
        if(    ac    )
        {   
            XtSetValues( XtParent (new), al, ac) ;
            }
        }
    /*	Copy new font list data, free previous.
    */
    if(    request->bulletin_board.button_font_list
                               != current->bulletin_board.button_font_list    )
    {   newbfonts = True ;
        if(    current->bulletin_board.button_font_list    )
        {   XmFontListFree( current->bulletin_board.button_font_list) ;
            } 
        if(    new->bulletin_board.button_font_list    )
        {   new->bulletin_board.button_font_list = XmFontListCopy( 
                                    request->bulletin_board.button_font_list) ;
            } 
        if(    !new->bulletin_board.button_font_list    )
        {   new->bulletin_board.button_font_list = XmFontListCopy( 
                     _XmGetDefaultFontList( (Widget) new, XmBUTTON_FONTLIST)) ;
            } 
        }
    if(    request->bulletin_board.label_font_list
                                != current->bulletin_board.label_font_list    )
    {   newlfonts = True ;
        if(    current->bulletin_board.label_font_list    )
        {   XmFontListFree( current->bulletin_board.label_font_list) ;
            } 
        if(    new->bulletin_board.label_font_list    )
        {   new->bulletin_board.label_font_list = XmFontListCopy( 
                                     request->bulletin_board.label_font_list) ;
            } 
        if(    !new->bulletin_board.label_font_list    )
        {   new->bulletin_board.label_font_list = XmFontListCopy( 
                      _XmGetDefaultFontList( (Widget) new, XmLABEL_FONTLIST)) ;
            } 
        }
    if(    request->bulletin_board.text_font_list
                                 != current->bulletin_board.text_font_list    )
    {   newtfonts = True ;
        if(    current->bulletin_board.text_font_list    )
        {   XmFontListFree( current->bulletin_board.text_font_list) ;
            } 
        if(    new->bulletin_board.text_font_list    )
        {   new->bulletin_board.text_font_list = XmFontListCopy( 
                                      request->bulletin_board.text_font_list) ;
            } 
        if(    !new->bulletin_board.text_font_list    )
        {   new->bulletin_board.text_font_list = XmFontListCopy( 
                       _XmGetDefaultFontList( (Widget) new, XmTEXT_FONTLIST)) ;
            } 
        }
    /* 	Update children if font list changed.
    */
    if(    newbfonts || newlfonts || newtfonts    )
    {   
        bfonts = new->bulletin_board.button_font_list ;
        lfonts = new->bulletin_board.label_font_list ;
        tfonts = new->bulletin_board.text_font_list ;

        kid = current->composite.children ;
        max = current->composite.num_children ;
        for(    i = 0 ;  i < max ;  i++    )
        {   
            if(    (newbfonts || newlfonts)  &&  
			(XmIsLabel (kid[i]) || XmIsLabelGadget (kid[i]))    )
            {   
                if(    newbfonts
                    && (XmIsPushButton( kid[i])
                         || XmIsPushButtonGadget( kid[i])
                         || XmIsToggleButtonGadget( kid[i])
                         || XmIsToggleButton( kid[i]))    )
                {   
                    XtSetArg( al[0], XmNfontList, bfonts) ;
                    XtSetValues( kid[i], al, 1) ;
                    }
                else
                {   if(    newlfonts
                        && !(   XmIsPushButton (kid[i])
                             || XmIsPushButtonGadget (kid[i])
                             || XmIsToggleButtonGadget (kid[i])
                             || XmIsToggleButton (kid[i]))    )
                    {   
                        XtSetArg( al[0], XmNfontList, lfonts) ;
                        XtSetValues( kid[i], al, 1) ;
                        }
                    } 
                }
            else
            {   if(    newtfonts
                    && (   XmIsText (kid[i])
                        || XmIsList (kid[i]))    )
                {   
                    XtSetArg( al[0], XmNfontList, tfonts) ;
                    XtSetValues( kid[i], al, 1) ;
                    }
                }
            }
        }
    if(    BB_DefaultButton( new) != BB_DefaultButton( current)    )
    {   
        if(    !BB_DefaultButton( current)    )
        {   
            /* This is the a new default button.  To avoid children generating
            *   geometry requests every time they get the default button
            *   shadow, be sure that the compatibility bit is turned off.
            *   This may generate geometry requests at this time, but they
            *   will never occur subsequently because the insert child
            *   routine will hereafter assure that the compatibility bit is
            *   turned off.
            */
            numChildren = new->composite.num_children ;
            childList = new->composite.children ;
            childIndex = 0 ;
            while(    childIndex < numChildren    )
            {   
                if(    XmIsPushButtonGadget( childList[childIndex])
                    || XmIsPushButton( childList[childIndex])    )
                {   
                    _XmBulletinBoardSetDefaultShadow( childList[childIndex]) ;
                    } 
                ++childIndex ;
                } 
            }
        if(    !BB_DefaultButton( new)    )
        {   
            /* Existing default button is being turned off.
            */
            _XmBulletinBoardSetDynDefaultButton( (Widget) new, NULL) ;
            } 
        else
        {   if(    BB_DescendantFocus( new)    )
            {   
                /* If a pushbutton has the focus, then always give it the
                *   default button visuals and behavior.  Otherwise,
                *   set the default button as specified.  Never display
                *   the default button unless a descendant has the focus.
                */
                if(    XmIsPushButtonGadget( BB_DescendantFocus( new))
                    || XmIsPushButton( BB_DescendantFocus( new))    )
                {   
                    _XmBulletinBoardSetDynDefaultButton( (Widget) new,
                                                    BB_DescendantFocus( new)) ;
                    } 
                else
                {   _XmBulletinBoardSetDynDefaultButton( (Widget) new,
                                                      BB_DefaultButton( new)) ;
                    } 
                } 
            } 
        } 
    current->bulletin_board.in_set_values = False ;

    return( False) ;
    }

/****************************************************************/
static Boolean
#ifdef _NO_PROTO
SetValuesHook( bb, args, num_args)
            XmBulletinBoardWidget bb ;
            ArgList         args ;
            Cardinal *      num_args ;
#else
SetValuesHook(
            XmBulletinBoardWidget bb,
            ArgList         args,
            Cardinal *      num_args)
#endif
/****************
 * 
 ****************/
{
	int			i;
	Widget			shell = bb->bulletin_board.shell;
	XtWidgetGeometry	request;
/****************/

/* shared with DialogS.c */    
#define MAGIC_VAL ((Position)~0L)
    
    if (!shell)
      return (False);
    
    for (i=0; i<*num_args; i++)
      {
	  if (strcmp (args[i].name, XmNx) == 0)
	    {
		if ((args[i].value == 0) && (XtX (bb) == 0))
		  {
		      XtX (bb) = MAGIC_VAL;
		  }
	    } 
	  if (strcmp (args[i].name, XmNy) == 0)
	    {
		if ((args[i].value == 0) && (XtY (bb) == 0))
		  {
		      XtY (bb) = MAGIC_VAL;
		  }
	    }
      }
    return (False);
}


/****************************************************************/
Widget
#ifdef _NO_PROTO
_XmBB_CreateButtonG( bb, l_string, name)
            Widget          bb ;
            XmString        l_string ;
            char *          name ;
#else
_XmBB_CreateButtonG(
            Widget          bb,
            XmString        l_string,
            char *          name)
#endif
/****************
 * 
 ****************/
{
            Arg		    al[10] ;
    register Cardinal       ac = 0 ;
            Widget          button ;
/****************/

    if(    l_string    )
    {
	XtSetArg( al[ac], XmNlabelString, l_string) ; ac++ ;
        }
    XtSetArg( al[ac], XmNstringDirection, BB_StringDirection( bb)) ; ac++ ;

    button = XmCreatePushButtonGadget( (Widget) bb, name, al, ac) ;

    _XmBulletinBoardSetDefaultShadow( button) ;

    return( button) ;
    }

/****************************************************************/
Widget
#ifdef _NO_PROTO
_XmBB_CreateLabelG( bb, l_string, name)
            Widget          bb ;
            XmString        l_string ;
            char *          name ;
#else
_XmBB_CreateLabelG(
            Widget          bb,
            XmString        l_string,
            char *          name)
#endif
/****************
 * 
 ****************/
{
            Arg		    al[10] ;
    register int            ac = 0 ;
/****************/

    if(    l_string    )
    {
	XtSetArg( al[ac], XmNlabelString, l_string) ; ac++ ;
        }
    XtSetArg( al[ac], XmNstringDirection, BB_StringDirection( bb)) ; ac++ ;
    XtSetArg( al[ac], XmNhighlightThickness, 0) ; ac++ ;
    XtSetArg( al[ac], XmNtraversalOn, False) ; ac++ ;
    XtSetArg( al[ac], XmNalignment, XmALIGNMENT_BEGINNING) ; ac++ ;

    return( XmCreateLabelGadget( bb, name, al, ac)) ;
    }

/****************************************************************/
static void
#ifdef _NO_PROTO
HandleChangeManaged( bbWid, geoMatrixCreate)
            XmBulletinBoardWidget bbWid ;
            XmGeoMatrix     (*geoMatrixCreate)() ;
#else
HandleChangeManaged(
            XmBulletinBoardWidget bbWid,
            XmGeoMatrix     (*geoMatrixCreate)())
#endif
/****************
 * Arrange widgets and make geometry request if necessary.
 ****************/
{
            Dimension       desired_w = 0 ;
            Dimension       desired_h = 0 ;
            Dimension       allowed_w ;
            Dimension       allowed_h ;
            XmGeoMatrix     geoSpec ;
/****************/

    /*	Use actuals if resize_none unless first time with actuals <= 1.
    */
    if(    BB_ResizePolicy( bbWid) == XmRESIZE_NONE    )
    {   
        if(    XtIsRealized( bbWid)  ||  (XtWidth( bbWid) > (Dimension) 1)    )
        {   desired_w = XtWidth( bbWid) ;
            } 
        if(    XtIsRealized( bbWid)  ||  (XtHeight( bbWid) > (Dimension) 1)   )
        {   desired_h = XtHeight( bbWid) ;
            } 
        } 
    geoSpec = (*geoMatrixCreate)( bbWid, NULL, NULL) ;

    _XmGeoMatrixGet( geoSpec, XmGET_PREFERRED_SIZE) ;

    _XmGeoArrangeBoxes( geoSpec, (Position) 0, (Position) 0,
                                                      &desired_w, &desired_h) ;
    /*	Adjust desired width and height if resize_grow.
    */
    if(    BB_ResizePolicy( bbWid) == XmRESIZE_GROW    )
    {
	Boolean		reset_w = False ;
        Boolean         reset_h = False ;

        if(    desired_w < XtWidth( bbWid)    )
	{
	    desired_w = XtWidth( bbWid) ;
	    reset_w = True ;
	    }
	if(    desired_h < XtHeight( bbWid)    )
	{
	    desired_h = XtHeight( bbWid) ;
	    reset_h = True ;
	    }
	if(    reset_w  ||  reset_h    )
	{   
            _XmGeoArrangeBoxes( geoSpec, (Position) 0, (Position) 0,
                                                      &desired_w, &desired_h) ;
            } 
	}

    /*	Request new geometry from parent.
    */
    if(    (desired_h != XtHeight( bbWid))
        || (desired_w != XtWidth( bbWid))    )
    {   
        switch(    XtMakeResizeRequest( (Widget) bbWid, desired_w, desired_h, 
                                                   &allowed_w, &allowed_h)    )
        {   
            case XtGeometryYes:
            {   
                allowed_w = desired_w ;
                allowed_h = desired_h ;
                break ;
                } 
            case XtGeometryAlmost:
            {   
                XtMakeResizeRequest( (Widget) bbWid, allowed_w, allowed_h, 
                                                                  NULL, NULL) ;
                _XmGeoArrangeBoxes( geoSpec, (Position) 0, (Position) 0, 
                                                      &allowed_w, &allowed_h) ;
                break ;
                } 
            case XtGeometryNo:
                    default:
            {   allowed_w = XtWidth( bbWid) ;
                allowed_h = XtHeight( bbWid) ;
                _XmGeoArrangeBoxes( geoSpec, (Position) 0, (Position) 0, 
                                                      &allowed_w, &allowed_h) ;
                break ;
                } 
            }
        /*	Clear shadow if necessary.
        */
        if(    bbWid->bulletin_board.old_shadow_thickness
            && (   (allowed_w > bbWid->bulletin_board.old_width)
                || (allowed_h > bbWid->bulletin_board.old_height))    )
        {   
            _XmClearShadowType( (Widget) bbWid,
                              bbWid->bulletin_board.old_width,
                                 bbWid->bulletin_board.old_height,
                                    bbWid->bulletin_board.old_shadow_thickness, 
                                                               (Dimension) 0) ;
            bbWid->bulletin_board.old_shadow_thickness = 0 ;
            }
        }
    /*	Update children.
    */
    _XmGeoMatrixSet( geoSpec) ;

    XtFree( (char *) geoSpec) ;

    if(    bbWid->manager.shadow_thickness
        && (XtWidth( bbWid) <= bbWid->bulletin_board.old_width)
        && (XtHeight( bbWid) <= bbWid->bulletin_board.old_height)    )
    {   
        _XmDrawShadowType( (Widget) bbWid, bbWid->bulletin_board.shadow_type,
                                    bbWid->core.width, bbWid->core.height,
                                       bbWid->manager.shadow_thickness,
                                          0, bbWid->manager.top_shadow_GC,
                                             bbWid->manager.bottom_shadow_GC) ;
        bbWid->bulletin_board.old_shadow_thickness 
                                            = bbWid->manager.shadow_thickness ;
        }
    bbWid->bulletin_board.old_width = bbWid->core.width ;
    bbWid->bulletin_board.old_height = bbWid->core.height ;

    _XmNavigChangeManaged( (Widget) bbWid) ;

    return ;
    }
/****************************************************************/
static void
#ifdef _NO_PROTO
HandleResize( bbWid, geoMatrixCreate)
            XmBulletinBoardWidget bbWid ;
            XmGeoMatrix     (*geoMatrixCreate)() ;
#else
HandleResize(
            XmBulletinBoardWidget bbWid,
            XmGeoMatrix     (*geoMatrixCreate)())
#endif
/****************
 * Arrange widgets based on new size.
 ****************/
{
            Dimension       new_w = XtWidth( bbWid) ;
            Dimension       new_h = XtHeight( bbWid) ;
            XmGeoMatrix     geoSpec ;
            Boolean         draw_shadow = False ;
/****************/
    
    /*	Clear shadow.
    */
    if(    bbWid->bulletin_board.old_shadow_thickness
        && (   (bbWid->bulletin_board.old_width != bbWid->core.width)
            || (bbWid->bulletin_board.old_height != bbWid->core.height))    )
    {
	_XmClearShadowType( (Widget) bbWid, bbWid->bulletin_board.old_width,
                              bbWid->bulletin_board.old_height, 
                               bbWid->bulletin_board.old_shadow_thickness, 
                                                               (Dimension) 0) ;
        bbWid->bulletin_board.old_shadow_thickness = 0 ;
	}

    geoSpec = (*geoMatrixCreate)( bbWid, NULL, NULL) ;

    _XmGeoMatrixGet( geoSpec, XmGET_PREFERRED_SIZE) ;

    _XmGeoArrangeBoxes( geoSpec, (Position) 0, (Position) 0, &new_w, &new_h) ;

    _XmGeoMatrixSet( geoSpec) ;

    if(    bbWid->manager.shadow_thickness
        && (XtWidth( bbWid) <= bbWid->bulletin_board.old_width)
        && (XtHeight( bbWid) <= bbWid->bulletin_board.old_height)    )
    {   
        _XmDrawShadowType( (Widget) bbWid, bbWid->bulletin_board.shadow_type,
                                    bbWid->core.width, bbWid->core.height,
                                       bbWid->manager.shadow_thickness,
                                          0, bbWid->manager.top_shadow_GC,
                                             bbWid->manager.bottom_shadow_GC) ;
        bbWid->bulletin_board.old_shadow_thickness 
                                            = bbWid->manager.shadow_thickness ;
        } 
    bbWid->bulletin_board.old_width = bbWid->core.width ;
    bbWid->bulletin_board.old_height = bbWid->core.height ;

    XtFree( (char *) geoSpec) ;
    return ;
    }
/****************************************************************/
static XtGeometryResult
#ifdef _NO_PROTO
HandleGeometryManager( instigator, desired, allowed, geoMatrixCreate)
            Widget          instigator ;
            XtWidgetGeometry * desired ;
            XtWidgetGeometry * allowed ;
            XmGeoMatrix     (*geoMatrixCreate)() ;
#else
HandleGeometryManager(
            Widget          instigator,
            XtWidgetGeometry * desired,
            XtWidgetGeometry * allowed,
            XmGeoMatrix     (*geoMatrixCreate)())
#endif
/****************
 * Handle geometry requests from children.
 ****************/
{
            XmBulletinBoardWidget bbWid ;
            XtGeometryResult result ;
/****************/

    bbWid = (XmBulletinBoardWidget) XtParent( instigator) ;

    if(    !(desired->request_mode & (CWWidth | CWHeight))    )
    {   return (XtGeometryYes) ;
        } 

    /*	Clear shadow if necessary.
    */
    if(    bbWid->bulletin_board.old_shadow_thickness
        && (BB_ResizePolicy( bbWid) != XmRESIZE_NONE)    )
    {
        _XmClearShadowType( (Widget) bbWid, bbWid->bulletin_board.old_width, 
                              bbWid->bulletin_board.old_height, 
                               bbWid->bulletin_board.old_shadow_thickness, 0) ;
        bbWid->bulletin_board.old_shadow_thickness = 0 ;
	}

    result = _XmHandleGeometryManager( (Widget) bbWid,
                         instigator, desired, allowed, BB_ResizePolicy( bbWid),
                           &bbWid->bulletin_board.geo_cache, geoMatrixCreate) ;

    if(    !bbWid->bulletin_board.in_set_values
        && bbWid->manager.shadow_thickness
        && (XtWidth( bbWid) <= bbWid->bulletin_board.old_width)
        && (XtHeight( bbWid) <= bbWid->bulletin_board.old_height)    )
    {   
        _XmDrawShadowType( (Widget) bbWid, bbWid->bulletin_board.shadow_type,
                                    bbWid->core.width, bbWid->core.height,
                                       bbWid->manager.shadow_thickness,
                                          0, bbWid->manager.top_shadow_GC,
                                             bbWid->manager.bottom_shadow_GC) ;
        bbWid->bulletin_board.old_shadow_thickness
                                            = bbWid->manager.shadow_thickness ;
        } 
    bbWid->bulletin_board.old_width = bbWid->core.width ;
    bbWid->bulletin_board.old_height = bbWid->core.height ;

    return( result) ;
    }

/****************************************************************/
void
#ifdef _NO_PROTO
_XmBulletinBoardSizeUpdate( bbWid)
            XmBulletinBoardWidget bbWid ;
#else
_XmBulletinBoardSizeUpdate(
            XmBulletinBoardWidget bbWid)
#endif
/****************
 * 
 ****************/
{
            XmBulletinBoardWidgetClass classPtr = 
                        (XmBulletinBoardWidgetClass) bbWid->core.widget_class ;
/****************/

    if(    !XtIsRealized( bbWid)    )
    {   return ;
        } 

    if(    classPtr->bulletin_board_class.geo_matrix_create    )
    {   
        /*	Clear shadow if necessary.
        */
        if(    bbWid->bulletin_board.old_shadow_thickness
            && (BB_ResizePolicy( bbWid) != XmRESIZE_NONE)    )
        {
            _XmClearShadowType( (Widget) bbWid,
                         bbWid->bulletin_board.old_width,
                            bbWid->bulletin_board.old_height,
                               bbWid->bulletin_board.old_shadow_thickness, 0) ;
            bbWid->bulletin_board.old_shadow_thickness = 0 ;
            }

        _XmHandleSizeUpdate((Widget) bbWid, BB_ResizePolicy( bbWid),
                            classPtr->bulletin_board_class.geo_matrix_create) ;

        if(    bbWid->manager.shadow_thickness
            && (XtWidth( bbWid) <= bbWid->bulletin_board.old_width)
            && (XtHeight( bbWid) <= bbWid->bulletin_board.old_height)    )
        {   
            _XmDrawShadowType( (Widget) bbWid,
                                 bbWid->bulletin_board.shadow_type,
                                    bbWid->core.width, bbWid->core.height,
                                       bbWid->manager.shadow_thickness,
                                          0, bbWid->manager.top_shadow_GC,
                                             bbWid->manager.bottom_shadow_GC) ;
            bbWid->bulletin_board.old_shadow_thickness
                                            = bbWid->manager.shadow_thickness ;
            }
        } 
    bbWid->bulletin_board.old_width = bbWid->core.width ;
    bbWid->bulletin_board.old_height = bbWid->core.height ;

    return ;
    }


/****************************************************************/
static void
#ifdef _NO_PROTO
CalcSize( bb, replyWidth, replyHeight)
            XmBulletinBoardWidget bb ;
            Dimension *     replyWidth ;
            Dimension *     replyHeight ;
#else
CalcSize(
            XmBulletinBoardWidget bb,
            Dimension *     replyWidth,
            Dimension *     replyHeight)
#endif
/****************
 * Figure out how much size we need.
 ****************/
{   
            Cardinal  i, numItems ;
            Dimension minWidth, minHeight ;
            Widget box ;   /* Current item */
/****************/
    /*
    * If there are no items then any space will work 
    */
    if(    (numItems=bb->composite.num_children) <= 0    )
    {   
        if(    (replyWidth != NULL) && (replyHeight != NULL)    )
        {   
            *replyWidth = (bb->bulletin_board.margin_width +
                                           bb->manager.shadow_thickness) << 1 ;
            if(    ! *replyWidth    )
            {   *replyWidth = 10 ;
                } 
            *replyHeight = (bb->bulletin_board.margin_height +
                                           bb->manager.shadow_thickness) << 1 ;
            if(    ! *replyHeight    )
            {   *replyHeight = 10 ;
                } 
            }
        return ;
        }
    /*
    * Compute the minimum width & height for this box
    * by summing the rectangles of all children.
    */
    minWidth = minHeight = 0 ;
    for(    i=0 ; i < numItems ; i++    )
    {   
        box = (Widget) bb->composite.children[i] ;
        if(    XtIsManaged (box)    )
        {   
            minWidth = Max( minWidth, (box->core.x + 2*box->core.border_width
                                                          + box->core.width)) ;
            minHeight = Max( minHeight, (box->core.y + 2*box->core.border_width
                                                         + box->core.height)) ;
            }
        }
    /*
    *  add in margins on right and bottom
    */
    minWidth += bb->bulletin_board.margin_width
                                               + bb->manager.shadow_thickness ;
    minHeight += bb->bulletin_board.margin_height
                                               + bb->manager.shadow_thickness ;
    if(    !minWidth    )
    {   minWidth = 10 ;
        } 
    if(    !minHeight    )
    {   minHeight = 10 ;
        } 
    *replyWidth = minWidth ;
    *replyHeight = minHeight ;
    return ;
    }
   
/****************************************************************/
static Boolean
#ifdef _NO_PROTO
DoLayout( bb)
            XmBulletinBoardWidget bb ;
#else
DoLayout(
            XmBulletinBoardWidget bb)
#endif
/****************
 * Calculate the size needed for the BulletinBoard.
 * Request a change in size if needed from parent.
 ****************/
{   
            Dimension		width, height ;
            Dimension		ok_width, ok_height ;
/****************/

    CalcSize( bb, &width, &height) ;

    if(    (bb->core.width == width) && (bb->core.height == height)    )
    {   return( True) ;
        } 
    if(    (    (bb->bulletin_board.resize_policy == XmRESIZE_GROW)
             || (bb->bulletin_board.resize_policy == XmRESIZE_NONE))
        && ((bb->core.width >= width) && (bb->core.height >= height))    )
    {   return( True) ;
        } 
    if(    bb->bulletin_board.resize_policy == XmRESIZE_NONE    )
    {   return( False) ;
        } 
    if(    bb->bulletin_board.resize_policy == XmRESIZE_GROW    )
    {   if(    width < bb->core.width    )
        {   width = bb->core.width ;
            } 
        if(    height < bb->core.height    )
        {   height = bb->core.height ;
            } 
        } 
    switch(    XtMakeResizeRequest( (Widget) bb, width, height, 
                                                    &ok_width, &ok_height)    )
    {   case XtGeometryYes:
        {   return( True) ;
            } 
        case XtGeometryAlmost:
        {   if(    (bb->bulletin_board.resize_policy == XmRESIZE_GROW)
                && (    (ok_width < bb->core.width)
                     || (ok_height < bb->core.height))    )
            {   /* If allowed is smaller than current in either dimension,
                *   must forget it.
                */
                break ;
                } 
            /* Since RESIZE_ANY or RESIZE_GROW with larger dimensions,
	    *    take what we can get.
            */
            XtMakeResizeRequest( (Widget) bb, ok_width, ok_height,
                                                                  NULL, NULL) ;
            return( True) ;
            } 
        case XtGeometryNo:
        default:
        {   break ;
            } 
        }
    return( False) ;
    }
   
/****************************************************************/
static void
#ifdef _NO_PROTO
ChangeManaged( bb)
            XmBulletinBoardWidget bb ;
#else
ChangeManaged(
            XmBulletinBoardWidget bb)
#endif
/****************
 * Layout children of the BulletinBoard.
 ****************/
{   
            Cardinal	i, numItems ;
            Widget		box ;   /* Current item */
            Position	newx, newy ;
            Boolean		do_move ;
            XmBulletinBoardWidgetClass classPtr = 
                           (XmBulletinBoardWidgetClass) bb->core.widget_class ;
/****************/

    if(    classPtr->bulletin_board_class.geo_matrix_create    )
    {   HandleChangeManaged( bb, 
                            classPtr->bulletin_board_class.geo_matrix_create) ;
        return ;
        } 
    /*	Enforce margins for children if margins are non-zero
    */
    numItems = bb->composite.num_children ;
    for(    i=0 ; i < numItems ; i++    )
    {   
        do_move = False ;
        box = (Widget) bb->composite.children[i] ;
        if(    XtIsManaged (box)    )
        {   
            if(    (BB_MarginWidth( bb) != 0)
		&& (box->core.x < bb->bulletin_board.margin_width)    )
            {   
                do_move = True ;
                newx = bb->bulletin_board.margin_width ;
                }
            else
            {   newx = box->core.x ;
                } 
            if(    (BB_MarginHeight( bb) != 0)
	        && (box->core.y < bb->bulletin_board.margin_height)    )
            {   
                do_move = True ;
                newy = bb->bulletin_board.margin_height ;
                }
            else
            {   newy = box->core.y ;
                } 
            }
        if(    do_move    )
        {   _XmMoveObject( (RectObj) box, newx, newy) ;
            } 
        }
    /*	Clear shadow if necessary.
    */
    if(    bb->bulletin_board.old_shadow_thickness    )
    {   
        _XmClearShadowType( (Widget) bb, bb->bulletin_board.old_width, 
				bb->bulletin_board.old_height, 
				bb->bulletin_board.old_shadow_thickness, 0) ;
        bb->bulletin_board.old_shadow_thickness = 0 ;
        }
    /*	Reconfigure the BulletinBoard.
    */
    DoLayout( bb) ;

    if(    bb->manager.shadow_thickness
        && (XtWidth( bb) <= bb->bulletin_board.old_width)
        && (XtHeight( bb) <= bb->bulletin_board.old_height)    )
    {   
        _XmDrawShadowType( (Widget) bb, bb->bulletin_board.shadow_type, 
               bb->core.width, bb->core.height, bb->manager.shadow_thickness,
                  0, bb->manager.top_shadow_GC, bb->manager.bottom_shadow_GC) ;
        bb->bulletin_board.old_shadow_thickness 
                                               = bb->manager.shadow_thickness ;
        }
    bb->bulletin_board.old_width = bb->core.width ;
    bb->bulletin_board.old_height = bb->core.height ;

    _XmNavigChangeManaged( (Widget) bb) ;

    return ;
    }
   
/****************************************************************/
static void
#ifdef _NO_PROTO
UnmanageCallback( w, client_data, call_data)
            Widget          w ;
            caddr_t         client_data ;
            caddr_t         call_data ;
#else
UnmanageCallback(
            Widget          w,
            caddr_t         client_data,
            caddr_t         call_data)
#endif
/****************           ARGSUSED
 * Unmanage BulletinBoard after button is activated.
 ****************/
{
            Widget          bb = (Widget) client_data ;
/****************/

    XtUnmanageChild( bb) ;

    return ;
    }

/****************************************************************/
static void
#ifdef _NO_PROTO
InsertChild( child)
            Widget          child ;
#else
InsertChild(
            Widget          child)
#endif
/****************
 * Add a child to the BulletinBoard.
 ****************/
{   
            XmBulletinBoardWidget bb = (XmBulletinBoardWidget)
                                                             XtParent( child) ;
            Boolean         is_button = False ;
            Arg             args[5] ;
            Cardinal        Nargs ;
            Dimension       shadowTh ;
/****************/

    /* call manager InsertChild to update BB child info
    */
    (*((XmManagerWidgetClass) xmManagerWidgetClass)
                                      ->composite_class.insert_child)( child) ;
    if(    !XtIsRectObj( child)    )
    {   return ;
        }

    if(    XmIsPushButtonGadget( child)  ||  XmIsPushButton( child)    )
    {   is_button = TRUE ;

        if(    BB_DefaultButton( bb)    )
        {   
            /* Turn off compatibility mode behavior of Default PushButtons and
            *   set the default button shadow thickness.
            */
            _XmBulletinBoardSetDefaultShadow( child) ;
            } 
        } 
    if(    XmIsDrawnButton( child)    )
    {   is_button = True ;
        } 
    if(    is_button && bb->bulletin_board.shell
                                       && bb->bulletin_board.auto_unmanage    )
    {   
        XtAddCallback( child, XmNactivateCallback,
                           (XtCallbackProc) UnmanageCallback, (XtPointer) bb) ;
        }
    if(    XmIsText (child)    )
    {   
        if(    bb->bulletin_board.text_translations    )
        {   XtOverrideTranslations( child, 
				bb->bulletin_board.text_translations) ;
            } 
        }
    return ;
    }

/****************************************************************/
static void
#ifdef _NO_PROTO
DeleteChild( child)
            Widget          child ;
#else
DeleteChild(
            Widget          child)
#endif
/****************
 * Clear widget id in instance record
 ****************/
{   
            XmBulletinBoardWidget bb ;
/****************/

    if(    XtIsRectObj( child)    )
    {   
        bb = (XmBulletinBoardWidget) XtParent( child) ;

        if(    child == BB_DefaultButton( bb)    )
        {   BB_DefaultButton( bb) = NULL ;
            } 
        else
        {   if(    child == BB_DynamicDefaultButton( bb)    )
            {   BB_DynamicDefaultButton( bb) = NULL ;
                } 
            else 
            {   if(    child == BB_CancelButton( bb)    )
                {   BB_CancelButton( bb) = NULL ;
                    } 
                else
                {   if(    child == BB_DynamicCancelButton( bb)    )
                    {   BB_DynamicCancelButton( bb) = NULL ;
                        } 
                    } 
                } 
            }
        } 
    (*((XmManagerWidgetClass) xmManagerWidgetClass)
                                      ->composite_class.delete_child)( child) ;
    return ;
    }	

/****************************************************************/
static Boolean
#ifdef _NO_PROTO
Overlap( bb, w)
            XmBulletinBoardWidget bb ;
            Widget          w ;
#else
Overlap(
            XmBulletinBoardWidget bb,
            Widget          w)
#endif
/****************
 * Return True if a child of the BulletinBoard intersects any other child.
 ****************/
{   
    register int	i ;
            Position	left1 = w->core.x ;
            Position	top1 = w->core.y ;
            Dimension	right1 = w->core.x + w->core.width +
                                                       2*w->core.border_width ;
            Dimension	bottom1 = w->core.y + w->core.height +
                                                       2*w->core.border_width ;
/****************/

    for(    i=0 ; i<bb->composite.num_children ; i++    )
    {   
                Widget		kid = bb->composite.children[i] ;
                Position	left2 = kid->core.x ;
                Position	top2 = kid->core.y ;
                Dimension	right2 = kid->core.x + kid->core.width +
                                                     2*kid->core.border_width ;
                Dimension	bottom2 = kid->core.y + kid->core.height +
                                                     2*kid->core.border_width ;
        if(    w != kid && 
		    (((left1 >= left2) && (left1 <= right2)) ||
		     ((left2 >= left1) && (left2 <= right1))) &&
		    (((top1 >= top2) && (top1 <= bottom2)) ||
		     ((top2 >= top1) && (top2 <= bottom1)))    )
        {   return( True) ;
            } 
        }
    return( False) ;
    }

/****************************************************************/
static XtGeometryResult
#ifdef _NO_PROTO
GeometryManager( w, request, reply)
            Widget          w ;
            XtWidgetGeometry * request ;
            XtWidgetGeometry * reply ;
#else
GeometryManager(
            Widget          w,
            XtWidgetGeometry * request,
            XtWidgetGeometry * reply)
#endif
/****************
 * 
 ****************/
{   
            Dimension	width, height, borderWidth ;
            Position	x, y ;
            XmBulletinBoardWidget bb ;
            Boolean geoFlag = False ;
            XtGeometryResult returnCode = XtGeometryNo ;
            XmBulletinBoardWidgetClass classPtr ;
            XtWidgetGeometry localReply ;
/****************/

    bb = (XmBulletinBoardWidget) w->core.parent ;
    classPtr = (XmBulletinBoardWidgetClass) bb->core.widget_class ;

    if(    classPtr->bulletin_board_class.geo_matrix_create    )
    {   return( HandleGeometryManager( w, request, reply,
                           classPtr->bulletin_board_class.geo_matrix_create)) ;
        } 
    /* 
     * Since this is a bulletin board, most position requests are accepted.
     * If requested position is in top or left margin, then return almost.
     */
    /* Fill out entire reply record
    */
    localReply = *request ;
    localReply.request_mode = CWX | CWY | CWWidth | CWHeight | CWBorderWidth ;

    if(    !(request->request_mode & CWX)    )
    {   localReply.x = w->core.x ;
        } 
    if(    !(request->request_mode & CWY)    )
    {   localReply.y = w->core.y ;
        } 
    if(    !(request->request_mode & CWWidth)    )
    {   localReply.width = w->core.width ;
        } 
    if(    !(request->request_mode & CWHeight)    )
    {   localReply.height = w->core.height ;
        } 
    if(    !(request->request_mode & CWBorderWidth)    )
    {   localReply.border_width = w->core.border_width ;
        } 

    /*  check for x less than margin width
    */
    if(    (request->request_mode & CWX)
        && (BB_MarginWidth( bb) != 0)
        && (request->x < bb->bulletin_board.margin_width)    )
    {   
        localReply.x = (Position) bb->bulletin_board.margin_width ;
        returnCode = XtGeometryAlmost ;
        }
    /*  check for y less than margin height  */
    if(    (request->request_mode & CWY)
        && (BB_MarginHeight( bb) != 0)
        && (request->y < bb->bulletin_board.margin_height)    )
    {   
        localReply.y = (Position) bb->bulletin_board.margin_height ;
        returnCode = XtGeometryAlmost ;
        }
    /* Save current size and set to new size
    */
    x = w->core.x ;
    y = w->core.y ;
    width = w->core.width ;
    height = w->core.height ;
    borderWidth = w->core.border_width ;

    w->core.x = localReply.x ;
    w->core.y = localReply.y ;
    w->core.width = localReply.width ;
    w->core.height = localReply.height ;
    w->core.border_width = localReply.border_width ;

    if(    !bb->bulletin_board.allow_overlap && Overlap (bb, w)    )
    {   
        returnCode = XtGeometryNo ;
        } 
    else
    {   geoFlag = DoLayout( bb) ;
        if(    !geoFlag
            && ((localReply.x + localReply.width
                                              + (localReply.border_width << 1))
                         <= (bb->core.width - bb->bulletin_board.margin_width))
            && ((localReply.y + localReply.height 
                                              + (localReply.border_width << 1))
                  <= (bb->core.height - bb->bulletin_board.margin_height))    )
        {   geoFlag = True ;
            } 
        if(    geoFlag    )
        {   
            if(    returnCode != XtGeometryAlmost    )
            {   returnCode = XtGeometryYes ;
                } 
            }
        else
        {   returnCode = XtGeometryNo ;
            } 
        }
    if(    returnCode == XtGeometryAlmost    )
    {   if(    reply    )
        {   *reply = localReply ;
            } 
        else
        {   returnCode = XtGeometryNo ;
            } 
        }
    if(    returnCode != XtGeometryYes    )
    {   /* Can't oblige, so restore previous values.
        */
        w->core.x = x ;
        w->core.y = y ;
        w->core.width = width ;
        w->core.height = height ;
        w->core.border_width = borderWidth ;
        } 

    /*	Clear shadow if necessary.
    */
    if(    bb->bulletin_board.old_shadow_thickness
        && (    (bb->bulletin_board.old_width != bb->core.width)
            ||  (bb->bulletin_board.old_height != bb->core.height))    )
    {   
        _XmClearShadowType( (Widget) bb, bb->bulletin_board.old_width, 
				bb->bulletin_board.old_height, 
				bb->bulletin_board.old_shadow_thickness, 0) ;
        bb->bulletin_board.old_shadow_thickness = 0 ;
        }
    bb->bulletin_board.old_width = XtWidth( bb) ;
    bb->bulletin_board.old_height = XtHeight( bb) ;

    return( returnCode) ;
    }
   
/****************************************************************/
static XtGeometryResult
#ifdef _NO_PROTO
QueryGeometry( bb, intended, desired)
            XmBulletinBoardWidget bb ;
            XtWidgetGeometry * intended ;
            XtWidgetGeometry * desired ;
#else
QueryGeometry(
            XmBulletinBoardWidget bb,
            XtWidgetGeometry * intended,
            XtWidgetGeometry * desired)
#endif
/****************
 * Handle query geometry requests
 ****************/
{   
            Dimension       a_w = 0, a_h = 0 ;
            XmBulletinBoardWidgetClass classPtr = 
                           (XmBulletinBoardWidgetClass) bb->core.widget_class ;
/****************/

    if(    classPtr->bulletin_board_class.geo_matrix_create    )
    {   
        return( _XmHandleQueryGeometry( (Widget) bb,
                        intended, desired, BB_ResizePolicy( bb),
                           classPtr->bulletin_board_class.geo_matrix_create)) ;
        } 
    if(    GMode (intended) & ~ (CWWidth | CWHeight)    )
    {   return( XtGeometryYes) ;
        } 
    if(    (GMode (intended) & CWWidth) &&
		 (GMode (intended) & ~CWHeight)    )
    {   a_w = intended->width ;
        } 
    if(    (GMode (intended) & CWHeight) &&
		 (GMode (intended) & ~CWWidth)    )
    {   a_h = intended->height ;
        } 
    CalcSize( bb, &a_w, &a_h) ;

    if(    (a_w == XtWidth (bb)) && (a_h == XtHeight (bb))    )
    {   return( XtGeometryNo) ;
        } 
    GMode( desired) = 0 ;
    desired->width = a_w ;
    desired->height = a_h ;
    if(    !GMode (intended)    )
    {   
        GMode( desired) |= CWWidth ;
        GMode( desired) |= CWHeight ;
        return( XtGeometryAlmost) ;
        }
    if(    (GMode (intended) & (CWWidth | CWHeight))
		&& ((a_w != intended->width) || (a_h != intended->height))    )
    {   
        GMode( desired) |= CWWidth ;
        GMode( desired) |= CWHeight ;
        return( XtGeometryAlmost) ;
        }
    if(    (GMode (intended) & CWWidth) && 
		 (GMode (intended) & ~CWHeight)    )
    {   
        GMode( desired) |= CWHeight ;
        return( XtGeometryAlmost) ;
        }
    if(  (GMode (intended) & CWHeight) && (GMode (intended) & ~CWWidth)  )
    {   
        GMode( desired) |= CWWidth ;
        return( XtGeometryAlmost) ;
        }
    return( XtGeometryYes) ;
    }

/****************************************************************/
static void
#ifdef _NO_PROTO
Resize( bb)
            XmBulletinBoardWidget bb ;
#else
Resize(
            XmBulletinBoardWidget bb)
#endif
/****************
 * Conform to new size.
 ****************/
{   
            XmBulletinBoardWidgetClass classPtr = 
                           (XmBulletinBoardWidgetClass) bb->core.widget_class ;
/****************/

    if(    classPtr->bulletin_board_class.geo_matrix_create    )
    {   
        HandleResize( bb, classPtr->bulletin_board_class.geo_matrix_create) ;
        return ;
        } 
    /*	Clear shadow.
    */
    if(    bb->bulletin_board.old_shadow_thickness    )
    {   
        _XmClearShadowType( (Widget) bb, bb->bulletin_board.old_width, 
				bb->bulletin_board.old_height, 
				bb->bulletin_board.old_shadow_thickness, 0) ;
        bb->bulletin_board.old_shadow_thickness = 0 ;
        }
    if(    bb->manager.shadow_thickness
        && (XtWidth( bb) <= bb->bulletin_board.old_width)
        && (XtHeight( bb) <= bb->bulletin_board.old_height)    )
    {   
        _XmDrawShadowType( (Widget) bb, bb->bulletin_board.shadow_type, 
               bb->core.width, bb->core.height, bb->manager.shadow_thickness,
                  0, bb->manager.top_shadow_GC, bb->manager.bottom_shadow_GC) ;
        bb->bulletin_board.old_shadow_thickness 
                                               = bb->manager.shadow_thickness ;
        }
    bb->bulletin_board.old_width = bb->core.width ;
    bb->bulletin_board.old_height = bb->core.height ;

    return ;
    }

/****************************************************************/
static void
#ifdef _NO_PROTO
Redisplay( bb, event, region)
            XmBulletinBoardWidget bb ;
            XEvent *        event ;
            Region          region ;
#else
Redisplay(
            XmBulletinBoardWidget bb,
            XEvent *        event,
            Region          region)
#endif
/****************
 * Redisplay gadgets and draw shadow.
 ****************/
{   
/****************/
    Widget          topMostShell, startWidget, dbutton = NULL ;
    static Widget   currTopMostShell = NULL, firstTravChild = NULL;

    /*	Redisplay gadgets.
    */
    _XmRedisplayGadgets( (CompositeWidget) bb, &event->xexpose, region) ;

    /*	Draw shadow.
    */
    if(    bb->manager.shadow_thickness    )
    {   
        _XmDrawShadowType( (Widget) bb, bb->bulletin_board.shadow_type, 
               bb->core.width, bb->core.height, bb->manager.shadow_thickness,
                  0, bb->manager.top_shadow_GC, bb->manager.bottom_shadow_GC) ;
        bb->bulletin_board.old_shadow_thickness 
                                               = bb->manager.shadow_thickness ;
        } 
    bb->bulletin_board.old_width = bb->core.width ;
    bb->bulletin_board.old_height = bb->core.height ;

#ifndef DBUTTON_FOCUS_BOGOUSITY
    /*
     * Set the dynamic default button when displayed for the first time.
     * This code should set BB_InitialFocus to false (so that it will be
     * executed only once), but doing so will disable the call to
     * XmProcessTraversal in _XmBulletinBoardFocusMoved.
     */
    if (BB_InitialFocus( bb)
        && BB_DefaultButton( bb)
        && (_XmGetFocusPolicy((Widget)bb) == XmEXPLICIT))
    {
	if (XtIsShell( XtParent( bb)))
	{
	    dbutton = BB_DefaultButton( bb);
	}
	else
	{
    /*
     * Set default button when manager is bootstrapping the traversal, and
     * traversal is set to the default button (see _XmMgrTraversal()).
     */
	    topMostShell = _XmFindTopMostShell((Widget)bb);

	    if ((firstTravChild == NULL)
		|| (topMostShell != currTopMostShell))
	    {
		if (startWidget = _XmFindNextTabGroup(topMostShell))
		{
		    firstTravChild = (Widget) _XmNavigate(startWidget, XmTRAVERSE_DOWN);
		}
		currTopMostShell = topMostShell;
	    }

	    if (firstTravChild == BB_DefaultButton( bb))
	      dbutton = firstTravChild;
	}

	if ((dbutton != NULL)
	    && (XmIsPushButton( dbutton)
		|| XmIsPushButtonGadget( dbutton))
	    && XtIsManaged( dbutton)
	    && XtIsSensitive( dbutton))
	{
	    _XmBulletinBoardSetDynDefaultButton( (Widget) bb, dbutton ) ;
	}
    }
#endif
    return ;
    }

/****************************************************************/
void
#ifdef _NO_PROTO
_XmBulletinBoardFocusMoved(  wid, client_data, call_data)
            Widget          wid ;
            XtPointer       client_data ;
            XmFocusMovedCallbackStruct * call_data ;
#else
_XmBulletinBoardFocusMoved(
            Widget          wid,
            XtPointer       client_data,
            XmFocusMovedCallbackStruct * call_data)
#endif
/****************           ARGSUSED
 * 
 ****************/
{   
            Widget          ancestor ;
	    XmBulletinBoardWidget bb = (XmBulletinBoardWidget) client_data ;
            XmAnyCallbackStruct	cb ;
            Boolean         BBHasFocus = FALSE ;
            Boolean         BBHadFocus = FALSE ;
            Widget          dbutton = NULL ;
            Widget          cbutton = NULL ;
/****************/

    if(    !call_data->cont    )
    {   /* Preceding callback routine wants focus-moved processing
        *   to be discontinued.
        */
        return ;
        } 

    /* Walk the heirarchy above the widget that is getting the focus to
    *   determine the correct default and cancel buttons for the current 
    *   context.
    *  Note that no changes are made to the Bulletin Board instance record
    *   until after the possibility of a forced traversal is evaluated.
    */
    ancestor = call_data->new ;
    while(    ancestor  &&  !XtIsShell( ancestor)    )
    {   
        if(    ancestor == (Widget) bb    )
        {   BBHasFocus = TRUE ;
            break ;
            } 
        if(    XmIsBulletinBoard( ancestor)    )
        {   
            if(    !dbutton    )
            {   dbutton = BB_DefaultButton( ancestor) ;
                } 
            if(    !cbutton    )
            {   cbutton = BB_CancelButton( ancestor) ;
                } 
            } 
        ancestor = XtParent( ancestor) ;
        } 
    ancestor = call_data->old ;
    while(    ancestor  &&  !XtIsShell( ancestor)    )
    {   
        if(    ancestor == (Widget) bb    )
        {   BBHadFocus = TRUE ;
            break ;
            } 
        ancestor = XtParent( ancestor) ;
        }

    if(    BBHasFocus    )
    {   
        /* The widget getting the input focus is a descendent of
        *   or is this Bulletin Board.
        * If there were no descendent Bulletin Boards with default or cancel
        *   buttons, use our own.
        */
        if(    !dbutton    )
        {   dbutton = BB_DefaultButton( bb) ;
            } 
        if(    !cbutton    )
        {   cbutton = BB_CancelButton( bb) ;
            } 

        if(    dbutton    )
        {   
            if(    (call_data->focus_policy == XmEXPLICIT)
                && BB_InitialFocus( bb)
                && (dbutton != call_data->new)
                && XtIsManaged( dbutton)
                && XtIsSensitive( dbutton)    )
            {   
                /* Have just entered this hierarchy and the button
                *   selected by the traversal code is not the default
                *   button, so force the focus to the default button.
                * Do this only if the default button is not from a
                *   descendent Bulletin Board to avoid contention 
                *   between another Bulletin Board trying to move 
                *   the focus to its own default button.
                */
                BB_InitialFocus( bb) = FALSE ;

                XmProcessTraversal((Widget) dbutton, XmTRAVERSE_CURRENT) ;

                call_data->cont = FALSE ; /* Do not continue. */
                return ;
                }        

            if(    XmIsPushButton( call_data->new)
                || XmIsPushButtonGadget( call_data->new)    )
            {   
                /* Any pushbutton which gets the focus
                *   gets the default visuals and behavior.
                */
                _XmBulletinBoardSetDynDefaultButton( (Widget) bb,
                                                              call_data->new) ;
                } 
            else /* call_data->new is not a push button */
            {   
                if(    (call_data->focus_policy == XmEXPLICIT)
                    || !XmIsManager( call_data->new)
                    || !call_data->old
                    || (    !XmIsPushButtonGadget( call_data->old)
                         && !XmIsPushButton( call_data->old))    )
                {   
                    /* Avoid setting the default button when in pointer mode,
                    *   leaving a button, and entering the background area.
                    *   The appropriate default button will be set when the
                    *   next focus moved event occurs.
                    *   This will avoid unnecessary flashing of default
                    *   button shadow when moving pointer between buttons.
                    */
                    if(    XtIsManaged( dbutton)    )
                    {   
                        _XmBulletinBoardSetDynDefaultButton( (Widget) bb,
                                                                     dbutton) ;
                        } 
                    } 
                } 
            }
        else /* dbutton == NULL */
        {   if(    BB_DynamicDefaultButton( bb)    )
            {   _XmBulletinBoardSetDynDefaultButton( (Widget) bb, NULL) ;
                } 
            } 
        BB_DynamicCancelButton( bb) = cbutton ;
        BB_DescendantFocus( bb) = call_data->new ;

        if(    !BBHadFocus    )
        {   
            cb.reason = XmCR_FOCUS ;
            cb.event = NULL ;
            XtCallCallbackList ((Widget) bb,
                                      bb->bulletin_board.focus_callback, &cb) ;

            } 
        } 
    else /* Bulletin Board does not have focus */
    {   
#ifdef BB_HAS_LOSING_FOCUS_CB

        if(    BBHadFocus    )
        {   
            cb.reason = XmCR_LOSING_FOCUS ;
            cb.event = NULL ;
            XtCallCallbackList ((Widget) bb,
                               bb->bulletin_board.losing_focus_callback, &cb) ;

            } 
#endif

#ifdef DBUTTON_FOCUS_BOGOUSITY
        _XmBulletinBoardSetDynDefaultButton( (Widget) bb, (Widget) NULL) ;
        BB_DynamicCancelButton( bb) = (Widget) NULL ;
        BB_DescendantFocus( bb) = (Widget) NULL ;

#else
	/* If a Bulletin Board had focus and new widget is not NULL,
	*   then remove default button graphic from previous Bulletin Board.
	*   This is so focus events on the shell does not clear the default
	*   button indicator.
	*/
        if(    BBHadFocus  &&  call_data->new  )
        {   
	    _XmBulletinBoardSetDynDefaultButton( (Widget) bb, (Widget) NULL) ;
	    BB_DynamicCancelButton( bb) = (Widget) NULL ;
	    BB_DescendantFocus( bb) = (Widget) NULL ;
        }
#endif

        }
    BB_InitialFocus( bb) = FALSE ;

    return ;
    }

/****************************************************************/
static Boolean
#ifdef _NO_PROTO
BulletinBoardParentProcess( bb, event)
            XmBulletinBoardWidget bb ;
            XmParentProcessData event ;
#else /* _NO_PROTO */
BulletinBoardParentProcess(
            XmBulletinBoardWidget bb,
            XmParentProcessData event)
#endif /* _NO_PROTO */

/****************
 * 
 ****************/
{
/****************/

    if(    (event->process_type == XmINPUT_ACTION)
        && (   (    (event->data.input_action.action == XmRETURN)
                 && BB_DynamicDefaultButton( bb))
            || (    (event->data.input_action.action == XmCANCEL)
                 && BB_DynamicCancelButton( bb)))    )
    {   
        if(    event->data.input_action.action == XmRETURN    )
        {   
            _XmBulletinBoardReturn( bb, event->data.input_action.event,
                                      event->data.input_action.params,
                                         event->data.input_action.num_params) ;
            } 
        else
        {   _XmBulletinBoardCancel( bb, event->data.input_action.event,
                                      event->data.input_action.params,
                                         event->data.input_action.num_params) ;
            } 
        return( TRUE) ;
        } 

    return( _XmParentProcess( XtParent( bb), event)) ;
    }

/****************************************************************/
void
#ifdef _NO_PROTO
_XmBulletinBoardReturn( bb, event, params, numParams)
            XmBulletinBoardWidget bb ;
            XEvent *        event ;
            String *        params ;
            Cardinal        numParams ;
#else
_XmBulletinBoardReturn(
            XmBulletinBoardWidget bb,
            XEvent *        event,
            String *        params,
            Cardinal        numParams)
#endif
/****************           ARGSUSED
 * Process Return and Enter key events in the BulletinBoard.
 *   If there is a default button, call its Activate callbacks.
 ****************/
{   
            XmAnyCallbackStruct cb ;
            XmGadgetClass   gadget_class ;
            XmPrimitiveWidgetClass primitive_class ;
            Widget          dbutton = BB_DynamicDefaultButton( bb) ;
            XmParentProcessDataRec parentEvent ;
            Widget          eventWidget ;
/****************/

    if(    !dbutton    )
    {   
        parentEvent.process_type = XmINPUT_ACTION ;
        parentEvent.data.input_action.action = XmRETURN ;
        parentEvent.data.input_action.event = event ;
        parentEvent.data.input_action.params = params ;
        parentEvent.data.input_action.num_params = numParams ;

        if(    _XmParentProcess( XtParent( bb), &parentEvent)    )
        {   return ;
            }
        dbutton = BB_DescendantFocus( bb) ;
        } 
    if(    dbutton    )
    {   
        if(    XmIsGadget( dbutton) && XtIsManaged( dbutton)    )
        {   
            gadget_class = (XmGadgetClass) XtClass( dbutton) ;
            if(    gadget_class->gadget_class.arm_and_activate
                && XtIsSensitive( dbutton)    )
            {   (*(gadget_class->gadget_class.arm_and_activate))( dbutton,
                                                                       event) ;
                } 
            }
        else
        {   if(    XmIsPrimitive( dbutton) && XtIsManaged( dbutton)    )
            {   
                primitive_class = (XmPrimitiveWidgetClass) XtClass( dbutton) ;
                if(    primitive_class->primitive_class.arm_and_activate
                    && XtIsSensitive( dbutton)    )
                {   (*(primitive_class->primitive_class.arm_and_activate))(
                                                              dbutton, event) ;
                    } 
                }
            else
            {   if(    XtIsSensitive( dbutton)
                    && (XtHasCallbacks( dbutton, XmNactivateCallback)
                                                     == XtCallbackHasSome)    )
                {   cb.reason = XmCR_ACTIVATE ;
                    cb.event = event ;
                    XtCallCallbacks( dbutton, XmNactivateCallback, 
                                                             (XtPointer) &cb) ;
                    } 
                }
            }
        } 
    return ;
    }

/****************************************************************/
void
#ifdef _NO_PROTO
_XmBulletinBoardCancel( bb, event, params, numParams)
            XmBulletinBoardWidget bb ;
            XEvent *        event ;
            String *        params ;
            Cardinal        numParams ;
#else
_XmBulletinBoardCancel(
            XmBulletinBoardWidget bb,
            XEvent *        event,
            String *        params,
            Cardinal        numParams)
#endif
/****************           ARGSUSED
 * Process Cancel key events in the BulletinBoard.
 * If there is a cancel button, call its Activate callbacks.
 ****************/
{   
            XmAnyCallbackStruct cb ;
            XmGadgetClass   gadget_class ;
            XmPrimitiveWidgetClass primitive_class ;
            Widget          cbutton = BB_DynamicCancelButton( bb) ;
            XmParentProcessDataRec parentEvent ;
/****************/

    if(    !cbutton    )
    {   
        parentEvent.process_type = XmINPUT_ACTION ;
        parentEvent.data.input_action.action = XmCANCEL ;
        parentEvent.data.input_action.event = event ;
        parentEvent.data.input_action.params = params ;
        parentEvent.data.input_action.num_params = numParams ;

        _XmParentProcess( XtParent( bb), &parentEvent) ;

        return ;
        } 
    if(    XmIsGadget( cbutton) && XtIsManaged( cbutton)    )
    {   
        gadget_class = (XmGadgetClass) XtClass( cbutton) ;
        if(    gadget_class->gadget_class.arm_and_activate
            && XtIsSensitive( cbutton)    )
        {   (*(gadget_class->gadget_class.arm_and_activate))( cbutton, event) ;
            } 
        }
    else
    {   if(    XmIsPrimitive( cbutton) && XtIsManaged( cbutton)    )
        {   
            primitive_class = (XmPrimitiveWidgetClass) XtClass( cbutton) ;
            if(    primitive_class->primitive_class.arm_and_activate
                && XtIsSensitive( cbutton)    )
            {   (*(primitive_class->primitive_class.arm_and_activate))(
                                                              cbutton, event) ;
                } 
            }
        else
        {   if(    XtIsSensitive( cbutton)
                && (XtHasCallbacks( cbutton, XmNactivateCallback)
                                                     == XtCallbackHasSome)    )
            {   cb.reason = XmCR_ACTIVATE ;
                cb.event = event ;
                XtCallCallbacks( cbutton, XmNactivateCallback,
                                                             (XtPointer) &cb) ;
                } 
            }
        }
    return ;
    }

/*
 * Copy the XmString in XmNdialogTitle before returning it to the user.
 */
void
#ifdef _NO_PROTO
_XmBB_GetDialogTitle( bb, resource, value)
            XmBulletinBoardWidget bb ;
            XrmQuark        resource ;
            XtArgVal *      value ;
#else
_XmBB_GetDialogTitle(
            XmBulletinBoardWidget bb,
            XrmQuark        resource,
            XtArgVal *      value)
#endif
/****************           ARGSUSED  ****************/
{
  XmString      data ;
  Arg           al[1] ;
/****************/
  
  data = XmStringCopy((XmString)bb->bulletin_board.dialog_title);
  *value = (XtArgVal) data ;

  return ;
}


/****************************************************************/
static void
#ifdef _NO_PROTO
GetXFromShell( bb, resource, value)
            XmBulletinBoardWidget bb ;
            XrmQuark        resource ;
            XtArgVal *      value ;
#else
GetXFromShell(
            XmBulletinBoardWidget bb,
            XrmQuark        resource,
            XtArgVal *      value)
#endif
/****************           ARGSUSED
 * 
 ****************/
{   
            Widget		shell = bb->bulletin_board.shell ;
            Position	data, junk ;
/****************/

    if(    shell  )
    {   
        XtTranslateCoords( (Widget) bb, (Position) 0, (Position) 0, 
                                                                &data, &junk) ;
        *value = (XtArgVal) data ;
        }
    else
    {   *value = (XtArgVal) bb->core.x ;
        } 
    return ;
    }
/****************************************************************/
static void
#ifdef _NO_PROTO
GetYFromShell( bb, resource, value)
            XmBulletinBoardWidget bb ;
            XrmQuark        resource ;
            XtArgVal *      value ;
#else
GetYFromShell(
            XmBulletinBoardWidget bb,
            XrmQuark        resource,
            XtArgVal *      value)
#endif
/****************           ARGSUSED
 * 
 ****************/
{   
            Widget		shell = bb->bulletin_board.shell ;
            Position	data, junk ;
/****************/

    if(    shell   )
    {   
        XtTranslateCoords( (Widget) bb, (Position) 0, (Position) 0, 
                                                                &junk, &data) ;
        *value = (XtArgVal)data ;
        }
    else
    {   *value = (XtArgVal)bb->core.y ;
        } 
    return ;
    }
/****************************************************************/
static void
#ifdef _NO_PROTO
GetBorderWidthFromShell( bb, resource, value)
            XmBulletinBoardWidget bb ;
            XrmQuark        resource ;
            XtArgVal *      value ;
#else
GetBorderWidthFromShell(
            XmBulletinBoardWidget bb,
            XrmQuark        resource,
            XtArgVal *      value)
#endif
/****************           ARGSUSED
 * 
 ****************/
{   
            Widget		shell = bb->bulletin_board.shell ;
/****************/

    if(    shell && XtIsManaged (bb)    )
    {   
        *value = (XtArgVal)XtBorderWidth( shell) ;
        }
    else
    {   *value = (XtArgVal)bb->core.border_width ;
        } 
    return ;
    }

/****************************************************************/
void
#ifdef _NO_PROTO
_XmBulletinBoardSetDefaultShadow( button)
            Widget          button ;
#else /* _NO_PROTO */
_XmBulletinBoardSetDefaultShadow(
            Widget          button)
#endif /* _NO_PROTO */
/****************
 * 
 ****************/
{   
            Arg             argv[2] ;
            Cardinal        argc ;
            Dimension       dbShadowTh ;
            Dimension       shadowTh ;
/****************/

    if(    XmIsPushButtonGadget( button)    )
    {   _XmClearBGCompatibility( (XmPushButtonGadget) button) ;
        } 
    else
    {   if(    XmIsPushButton( button)    )
        {   _XmClearBCompatibility( (XmPushButtonWidget) button) ;
            } 
        } 
    argc = 0 ;
    XtSetArg( argv[argc], XmNshadowThickness, &shadowTh) ; ++argc ;
    XtSetArg( argv[argc], XmNdefaultButtonShadowThickness, 
                                                        &dbShadowTh) ; ++argc ;
    XtGetValues( button, argv, argc) ;
    
    if(    !dbShadowTh    )
    {   if(    shadowTh > 1    )
        {   dbShadowTh = shadowTh >> 1 ;
            }
        else
        {   dbShadowTh = shadowTh ;
            } 
        argc = 0 ;
        XtSetArg( argv[argc], XmNdefaultButtonShadowThickness, 
                                                         dbShadowTh) ; ++argc ;
        XtSetValues( button, argv, argc) ;
        } 
    return ;
    }

/****************************************************************/
void
#ifdef _NO_PROTO
_XmBulletinBoardSetDynDefaultButton( wid, newDefaultButton)
            Widget          wid ;
            Widget          newDefaultButton ;
#else
_XmBulletinBoardSetDynDefaultButton(
            Widget          wid,
            Widget          newDefaultButton)
#endif
/****************
 * If the default button of the bulletin board widget (wid) is not equal to
 *   the specified default button (newDefaultButton), then SetValues is called
 *   to turn off/on the appropriate default borders and the default button
 *   field of the bulletin board instance record is updated appropriately.
  ****************/
{
            XmBulletinBoardWidget bbWid = (XmBulletinBoardWidget) wid ;
            Arg             args[1] ;
/****************/

    if(    newDefaultButton != BB_DynamicDefaultButton( bbWid)    )
    {   
        if(    BB_DynamicDefaultButton( bbWid)    )
        {   XtSetArg( args[0], XmNshowAsDefault, FALSE) ;
            XtSetValues( BB_DynamicDefaultButton( bbWid), args, 1) ;
            } 
        BB_DynamicDefaultButton( bbWid) = newDefaultButton ;

        if(    newDefaultButton    )
        {   
            if(    XtParent( newDefaultButton) != (Widget) bbWid    )
            {   
                if(    XmIsPushButtonGadget( newDefaultButton)    )
                {   _XmClearBGCompatibility(
                                       (XmPushButtonGadget) newDefaultButton) ;
                    } 
                else
                {   if(    XmIsPushButton( newDefaultButton)    )
                    {   _XmClearBCompatibility(
                                       (XmPushButtonWidget) newDefaultButton) ;
                        } 
                    } 
                } 
            XtSetArg( args[0], XmNshowAsDefault, TRUE) ;
            XtSetValues( newDefaultButton, args, 1) ;
            } 
        } 
    return ;
    }

/****************************************************************/
static void
#ifdef _NO_PROTO
UpdateWMShellTitle( bbWid)
            XmBulletinBoardWidget bbWid ;
#else
UpdateWMShellTitle(
            XmBulletinBoardWidget bbWid)
#endif
/****************
 * If the parent is a WMShell, this routine sets the title and title 
 *   encoding resources of the WMShell according to the dialog_title
 *   resource of the bulletin board widget
 ****************/
{   
            Widget          parent ;
            char *          text ;
            XmStringCharSet charset ;
            Arg             al[10] ;
            Cardinal        ac ;
            XrmValue        from ;
            XrmValue        to ;
/****************/

    /* Set WMShell title (if present).
    */
    parent = XtParent( bbWid) ;

    if(    XtIsWMShell( parent)    )
    {   
        /* Parent is a Window Manager Shell, so set WMShell title
        *   from XmNdialogTitle.
        */
        text = NULL ;
        ac = 0 ;
        if(    _XmStringSingleSegment( bbWid->bulletin_board.dialog_title, 
                                                          &text, &charset)    )
        {   if(    !strcmp( STRING_CHARSET, charset)
        /* Remove default charset comparison when Motif is internationalized.*/
                || !strcmp( XmSTRING_DEFAULT_CHARSET, charset)    )
            {   
                /* dialog_title is a single segment of charset STRING_CHARSET,
                *   so use atom of "STRING".  Otherwise, convert to compound
                *   text and use atom of "COMPOUND_TEXT".
                */
                XtFree( (char *) charset) ;
                charset = "STRING" ;
                } 
            else
            {   /* Don't need this, since dialog_title will be converted from
                *   original XmString to compound text.
                */
                XtFree( (char *) charset) ;
                XtFree( (char *) text) ;
                text = NULL ;
                } 
            } 
        if(    !text    )
        {   from.addr = (caddr_t)bbWid->bulletin_board.dialog_title ;
            if(    XmCvtXmStringToText( XtDisplay( bbWid), NULL, NULL, 
                                                         &from, &to, NULL)    )
            {   text = to.addr ;
                charset = "COMPOUND_TEXT" ;
                } 
            } 
        if(    text    )
        {   
            XtSetArg( al[ac], XtNtitle, text) ;  ++ac ;
            XtSetArg( al[ac], XtNtitleEncoding, XmInternAtom( 
                                   XtDisplay( bbWid), charset, FALSE)) ; ++ac ;
            XtSetValues( parent, al, ac) ;
            XtFree( (char *) text) ;
            } 
        } 
    return ;
    }

/****************************************************************/
static Boolean
#ifdef _NO_PROTO
_XmStringSingleSegment( str, pTextOut, pCharsetOut)
            XmString        str ;
            char * *        pTextOut ;
            XmStringCharSet * pCharsetOut ;
#else
_XmStringSingleSegment(
            XmString        str,
            char * *        pTextOut,
            XmStringCharSet * pCharsetOut)
#endif
/****************
 * Allocates a copy of the text and character set of the specified XmString 
 *   if the XmString is composed of a single segment.
 * Returns TRUE if str is a single segment, FALSE otherwise.
 ****************/
{
            Boolean         retVal ;
            XmStringContext context ;
            char *          text ;
            XmStringCharSet charset ;
            char *          text2 ;
            XmStringCharSet charset2 ;
            XmStringDirection direction ;
            Boolean         separator ;
/****************/

    retVal = FALSE ;

    if(    XmStringInitContext( &context, str)    )
    {   
        if(    XmStringGetNextSegment( context, &text, &charset,
                                                   &direction, &separator)    )
        {   if(    !XmStringGetNextSegment( context, &text2, &charset2,
                                                   &direction, &separator)    )
            {   retVal = TRUE ;
                *pTextOut = text ;
                *pCharsetOut = charset ;
                } 
            else
            {   XtFree( text) ;
                XtFree( (char *) charset) ;
                XtFree( text2) ;
                XtFree( (char *) charset2) ;
                } 
            } 
        XmStringFreeContext( context) ;
        } 
    return( retVal) ;
    }

/****************************************************************/
Widget
#ifdef _NO_PROTO
XmCreateBulletinBoard( p, name, args, n)
            Widget          p ;         /*  parent widget	*/
            String          name ;	/*  widget name		*/
            ArgList         args ;	/*  arg list	*/
            Cardinal        n ;         /*  arg count	*/
#else
XmCreateBulletinBoard(
            Widget          p,          /*  parent widget	*/
            String          name,       /*  widget name		*/
            ArgList         args,       /*  arg list	*/
            Cardinal        n)          /*  arg count	*/
#endif
/****************
 * This function creates and returns a BulletinBoard widget.
 ****************/
{   
/****************/

    return( XtCreateWidget( name, xmBulletinBoardWidgetClass, p, args, n)) ;
    }
/****************************************************************/
Widget
#ifdef _NO_PROTO
XmCreateBulletinBoardDialog( ds_p, name, bb_args, bb_n)
            Widget          ds_p ;      /*  parent for shell	*/
            String          name ;      /*  widget name		*/
            ArgList         bb_args ;   /*  arglist for bb	*/
            Cardinal        bb_n ;      /*  arg count for bb	*/
#else
XmCreateBulletinBoardDialog(
            Widget          ds_p,       /*  parent for shell	*/
            String          name,       /*  widget name		*/
            ArgList         bb_args,    /*  arglist for bb	*/
            Cardinal        bb_n)       /*  arg count for bb	*/
#endif
/****************
 * This convenience function creates a DialogShell and a BulletinBoard
 *   child of the shell; returns the BulletinBoard widget.
 ****************/
{   
            Widget		bb ;		/*  BulletinBoard	*/
            Widget		ds ;		/*  DialogShell		*/
            Arg		ds_args[1] ;	/*  arglist for shell	*/
            char           *ds_name ;
/****************/

    /*	Create DialogShell parent.
    */
    ds_name = XtCalloc( strlen( name)+DIALOG_SUFFIX_SIZE+1, sizeof( char)) ;
    strcpy( ds_name, name) ;
    strcat( ds_name, DIALOG_SUFFIX) ;

    XtSetArg( ds_args[0], XmNallowShellResize, True) ; 
    ds = XmCreateDialogShell( ds_p, ds_name, ds_args, 1) ;

    XtFree( ds_name) ;
    /*	Create BulletinBoard.
    */
    bb = XtCreateWidget( name, xmBulletinBoardWidgetClass, ds, bb_args, bb_n) ;
    /*	Add callback to destroy DialogShell parent.
    */
    XtAddCallback( bb, XmNdestroyCallback, _XmDestroyParentCallback, 
                                                            (XtPointer) NULL) ;
    /*	Return BulletinBoard.
    */
    return( bb) ;
    }
/****************************************************************/
