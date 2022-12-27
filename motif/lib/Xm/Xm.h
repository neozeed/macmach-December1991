#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)Xm.h	3.67 91/01/10";
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

#ifndef _Xm_h
#define _Xm_h


#include <X11/Intrinsic.h>
#include <X11/Vendor.h>
#include <Xm/VirtKeys.h>


#define XmVERSION	1
#define XmREVISION	1
#define XmVersion (XmVERSION * 1000 + XmREVISION)

extern int xmUseVersion;


/* define used to denote an unspecified pixmap  */

#define XmUNSPECIFIED_PIXMAP	2

/* define charset constants */
#define XmSTRING_ISO8859_1  "ISO8859-1"
#define XmSTRING_OS_CHARSET "ISO8859-1"
#ifndef XmFALLBACK_CHARSET
#define XmFALLBACK_CHARSET "ISO8859-1"
#endif

/****************
 *
 * XmString structure defines. These must be here (at the start of the file) 
 * becaused they are used later on.
 *
 ****************/

#define XmSTRING_DIRECTION_L_TO_R	0
#define XmSTRING_DIRECTION_R_TO_L	1
#define XmSTRING_DIRECTION_DEFAULT      255

typedef unsigned char *	XmString;		/* opaque to outside */
typedef XmString *	XmStringTable;		/* opaque to outside */
typedef char *		XmStringCharSet;	/* Null term string */
typedef unsigned char	XmStringComponentType;	/* component tags */
typedef unsigned char 	XmStringDirection;

typedef struct _XmFontListRec       *XmFontList;       /* opaque to outside */
typedef struct __XmStringContextRec *_XmStringContext; /* opaque to outside */
typedef struct __XmStringRec        *_XmString;        /* opaque to outside */
typedef struct _XmtStringContextRec *XmStringContext;  /* opaque to outside */
typedef struct _XmFontListContextRec *XmFontContext;   /* opaque to outside */

#define XmSTRING_COMPONENT_UNKNOWN	0
#define XmSTRING_COMPONENT_CHARSET	1
#define XmSTRING_COMPONENT_TEXT		2
#define XmSTRING_COMPONENT_DIRECTION	3
#define XmSTRING_COMPONENT_SEPARATOR	4	/* 5-125 reserved */

#define XmSTRING_COMPONENT_END		126	/* no more comp in string */

#define XmSTRING_COMPONENT_USER_BEGIN	128	/* 128-255 are user tags */
#define XmSTRING_COMPONENT_USER_END	255
#define XmSTRING_DEFAULT_CHARSET	""


/************************************************************************
 *  Resource names
 *	Taken from StringDefs, with Xt replaced by Xm.
 ************************************************************************/

#define XmNaccelerator          "accelerator"
#define XmNaccelerators         "accelerators"
#define XmNacceleratorText      "acceleratorText"
#define XmNadjustLast		"adjustLast"
#define XmNadjustMargin		"adjustMargin"
#define XmNalignment		"alignment"
#define XmNancestorSensitive	"ancestorSensitive"
#define XmNarmCallback	 	"armCallback"
#define XmNbackground		"background"
#define XmNbackgroundPixmap     "backgroundPixmap"
#define XmNbitmap		"bitmap"
#define XmNborderColor		"borderColor"
#define XmNborder		"borderColor"
#define XmNborderPixmap		"borderPixmap"
#define XmNborderWidth		"borderWidth"
#define XmNbuttonAccelerators	"buttonAccelerators"
#define XmNbuttonAcceleratorText	"buttonAcceleratorText"
#define XmNbuttonCount		"buttonCount"
#define XmNbuttonMnemonicCharSets	"buttonMnemonicCharSets"
#define XmNbuttonMnemonics	"buttonMnemonics"
#define XmNbuttons		"buttons"
#define XmNbuttonSet		"buttonSet"
#define XmNbuttonType		"buttonType"
#define XmNcascadePixmap	"cascadePixmap"
#define XmNcascadingCallback	"cascadingCallback"
#define XmNchildren             "children"
#define XmNcolormap		"colormap"
#define XmNcommandWindowLocation "commandWindowLocation"
#define XmNdefaultFontList	"defaultFontList"
#define XmNdepth		"depth"
#define XmNdestroyCallback      "destroyCallback"
#define XmNdisarmCallback       "disarmCallback"
#define XmNeditType		"editType"
#define XmNentryAlignment	"entryAlignment"
#define XmNentryBorder		"entryBorder"
#define XmNentryClass		"entryClass"
#define XmNentryCallback	"entryCallback"
#define XmNexposeCallback       "exposeCallback"
#define XmNfile			"file"
#define XmNfillOnSelect		"fillOnSelect"
#define XmNfilterLabelString	"filterLabelString"
#define XmNfont			"font"
#define XmNfontList		"fontList"
#define XmNforceBars		"forceBars"
#define XmNforeground		"foreground"
#define XmNfunction		"function"
#define XmNheight		"height"
#define XmNhighlight		"highlight"
#define XmNindex		"index"
#define XmNindicatorOn		"indicatorOn"
#define XmNindicatorSize	"indicatorSize"
#define XmNindicatorType	"indicatorType"
#define XmNinitialResourcesPersistent   "initialResourcesPersistent"
#define XmNinnerHeight		"innerHeight"
#define XmNinnerWidth		"innerWidth"
#define XmNinnerWindow		"innerWindow"
#define XmNinsertPosition       "insertPosition"
#define XmNinternalHeight	"internalHeight"
#define XmNinternalWidth	"internalWidth"
#define XmNisAligned		"isAligned"
#define XmNisHomogeneous	"isHomogeneous"
#define XmNjumpProc		"jumpProc"
#define XmNjustify		"justify"
#define XmNlabelInsensitivePixmap "labelInsensitivePixmap"
#define XmNlabelPixmap		"labelPixmap"
#define XmNlabelString		"labelString"
#define XmNlabelType		"labelType"
#define XmNlength		"length"
#define XmNlistUpdated      "listUpdated"
#define XmNlowerRight		"lowerRight"
#define XmNmapCallback		"mapCallback"
#define XmNmappedWhenManaged	"mappedWhenManaged"
#define XmNmappingDelay	        "mappingDelay"
#define XmNmarginHeight		"marginHeight"
#define XmNmarginTop		"marginTop"
#define XmNmarginBottom		"marginBottom"
#define XmNmarginWidth		"marginWidth"
#define XmNmarginRight		"marginRight"
#define XmNmarginLeft		"marginLeft"
#define XmNmenuAccelerator	"menuAccelerator"
#define XmNmenuCursor		"menuCursor"
#define XmNmenuEntry		"menuEntry"
#define XmNmenuHelpWidget	"menuHelpWidget"
#define XmNmenuHistory		"menuHistory"
#define XmNmenuPost		"menuPost"
#define XmNmnemonic		"mnemonic"
#define XmNmnemonicCharSet  	"mnemonicCharSet"
#define XmNname         	"name"
#define XmNnavigationType	"navigationType"
#define XmNnotify		"notify"
#define XmNnumChildren		"numChildren"
#define XmNnumColumns		"numColumns"
#define XmNoptionLabel		"optionLabel"
#define XmNoptionMnemonic       "optionMnemonic"
#define XmNorientation		"orientation"
#define XmNpacking		"packing"
#define XmNparameter		"parameter"
#define XmNpopdownCallback	"popdownCallback"
#define XmNpopupCallback	"popupCallback"
#define XmNpopupEnabled		"popupEnabled"
#define XmNpostFromButton	"postFromButton"
#define XmNpostFromCount	"postFromCount"
#define XmNpostFromList		"postFromList"
#define XmNradioAlwaysOne	"radioAlwaysOne"
#define XmNradioBehavior	"radioBehavior"
#define XmNrecomputeSize	"recomputeSize"
#define XmNresize		"resize"
#define XmNresizeCallback       "resizeCallback"
#define XmNreverseVideo		"reverseVideo"
#define XmNrowColumnType	"rowColumnType"
#define XmNscaleMultiple     "scaleMultiple"
#define XmNscreen               "screen"
#define XmNscrollProc		"scrollProc"
#define XmNscrollDCursor	"scrollDCursor"
#define XmNscrollHCursor	"scrollHCursor"
#define XmNscrollLCursor	"scrollLCursor"
#define XmNscrollRCursor	"scrollRCursor"
#define XmNscrollUCursor	"scrollUCursor"
#define XmNscrollVCursor	"scrollVCursor"
#define XmNselectColor		"selectColor"
#define XmNselection		"selection"
#define XmNselectionArray	"selectionArray"
#define XmNselectInsensitivePixmap "selectInsensitivePixmap"
#define XmNselectPixmap	 	"selectPixmap"
#define XmNsensitive		"sensitive"
#define XmNset			"set"
#define XmNshadow		"shadow"
#define XmNshown		"shown"
#define XmNsimpleCallback	"simpleCallback"
#define XmNspace		"space"
#define XmNspacing		"spacing"
#define XmNstring		"string"
#define XmNstringDirection	"stringDirection"
#define XmNsubMenuId		"subMenuId"
#define XmNtextOptions		"textOptions"
#define XmNtextSink		"textSink"
#define XmNtextSource		"textSource"
#define XmNthickness		"thickness"
#define XmNthumb		"thumb"
#define XmNthumbProc		"thumbProc"
#define XmNtop			"top"
#define XmNtranslations		"translations"
#define XmNtraversalType    "traversalType"
#define XmNtroughColor		"troughColor"
#define XmNunmapCallback	"unmapCallback"
#define XmNunselectPixmap 	"unselectPixmap"
#define XmNupdate		"update"
#define XmNuseBottom		"useBottom"
#define XmNuseRight		"useRight"
#define XmNvalue		"value"
#define XmNvisibleWhenOff	"visibleWhenOff"
#define XmNwhichButton	"whichButton"
#define XmNwidth		"width"
#define XmNwindow		"window"
#define XmNx			"x"
#define XmNy			"y"

/* Class types */ 

#define XmCAccelerator          "Accelerator"
#define XmCAccelerators         "Accelerators"
#define XmCAcceleratorText      "AcceleratorText"
#define XmCAdjustLast		"AdjustLast"
#define XmCAdjustMargin		"AdjustMargin"
#define XmCAlignment		"Alignment"
#define XmCArmCallback		"ArmCallback"
#define XmCBackground		"Background"
#define XmCBitmap		"Bitmap"
#define XmCBoolean		"Boolean"
#define XmCBorderColor		"BorderColor"
#define XmCBorderWidth		"BorderWidth"
#define XmCButtonAccelerators	"ButtonAccelerators"
#define XmCButtonAcceleratorText	"ButtonAcceleratorText"
#define XmCButtonCount		"ButtonCount"
#define XmCButtonMnemonicCharSets	"ButtonMnemonicCharSets"
#define XmCButtonMnemonics	"ButtonMnemonics"
#define XmCButtons		"Buttons"
#define XmCButtonSet		"ButtonSet"
#define XmCButtonType		"ButtonType"
#define XmCCallback             "Callback"
#define XmCChildren             "Children"
#define XmCColormap		"Colormap"
#define XmCColor		"Color"
#define XmCCommandWindowLocation "CommandWindowLocation"
#define XmCCursor		"Cursor"
#define XmCDefaultFontList	"DefaultFontList"
#define XmCDepth		"Depth"
#define XmCDimension		"Dimension"
#define XmCDisarmCallback	"DisarmCallback"
#define XmCEditType		"EditType"
#define XmCEntryBorder		"EntryBorder"
#define XmCEntryClass		"EntryClass"
#define XmCEventBindings	"EventBindings"
#define XmCExposeCallback       "ExposeCallback"
#define XmCFile			"File"
#define XmCFillOnSelect		"FillOnSelect"
#define XmCFilterLabelString	"FilterLabelString"
#define XmCFont			"Font"
#define XmCFontList		"FontList"
#define XmCForeground		"Foreground"
#define XmCFraction		"Fraction"
#define XmCFunction		"Function"
#define XmCHeight		"Height"
#define XmCHSpace		"HSpace"
#define XmCIndex		"Index"
#define XmCIndicatorOn		"IndicatorOn"
#define XmCIndicatorSize	"IndicatorSize"
#define XmCIndicatorType	"IndicatorType"
#define XmCInitialResourcesPersistent   "InitialResourcesPersistent"
#define XmCInsertPosition       "InsertPosition"
#define XmCInterval		"Interval"
#define XmCIsAligned		"IsAligned"
#define XmCIsHomogeneous	"IsHomogeneous"
#define XmCJustify		"Justify"
#define XmCLabel		"Label"
#define XmCLabelInsensitivePixmap "LabelInsensitivePixmap"
#define XmCLabelPixmap		"LabelPixmap"
#define XmCLabelString		"LabelString"
#define XmCLabelType		"LabelType"
#define XmCLength		"Length"
#define XmCListUpdated       "ListUpdated"
#define XmCMappedWhenManaged	"MappedWhenManaged"
#define XmCMappingDelay         "MappingDelay"
#define XmCMargin		"Margin"
#define XmCMarginHeight		"MarginHeight"
#define XmCMarginWidth		"MarginWidth"
#define XmCMarginLeft		"MarginLeft"
#define XmCMarginRight		"MarginRight"
#define XmCMarginTop		"MarginTop"
#define XmCMarginBottom		"MarginBottom"
#define XmCMenuEntry		"MenuEntry"
#define XmCMenuPost		"MenuPost"
#define XmCMenuWidget           "MenuWidget"
#define XmCMnemonic		"Mnemonic"
#define XmCMnemonicCharSet      "MnemonicCharSet"
#define XmCNavigationType	"NavigationType"
#define XmCNotify		"Notify"
#define XmCNumChildren		"NumChildren"
#define	XmCNumColumns		"NumColumns"
#define XmCOptionLabel		"OptionLabel"
#define XmCOptionMnemonic       "OptionMnemonic"
#define XmCOrientation		"Orientation"
#define XmCPacking		"Packing"
#define XmCParameter		"Parameter"
#define XmCPixmap		"Pixmap"
#define XmCPopupEnabled		"PopupEnabled"
#define XmCPosition		"Position"
#define XmCPostFromButton	"PostFromButton"
#define XmCPostFromCount	"PostFromCount"
#define XmCPostFromList		"PostFromList"
#define XmCRadioAlwaysOne	"RadioAlwaysOne"
#define XmCRadioBehavior	"RadioBehavior"
#define XmCRecomputeSize	"RecomputeSize"
#define XmCResize		"Resize"
#define XmCResizeCallback       "ResizeCallback"
#define XmCReverseVideo		"ReverseVideo"
#define XmCRowColumnType	"RowColumnType"
#define XmCScaleMultiple        "ScaleMultiple"
#define XmCScreen               "Screen"
#define XmCScrollProc		"ScrollProc"
#define XmCScrollDCursor	"ScrollDCursor"
#define XmCScrollHCursor	"ScrollHCursor"
#define XmCScrollLCursor	"ScrollLCursor"
#define XmCScrollRCursor	"ScrollRCursor"
#define XmCScrollUCursor	"ScrollUCursor"
#define XmCScrollVCursor	"ScrollVCursor"
#define XmCSelectColor		"SelectColor"
#define XmCSelection		"Selection"
#define XmCSensitive		"Sensitive"
#define XmCSelectionArray	"SelectionArray"
#define XmCSelectInsensitivePixmap "SelectInsensitivePixmap"
#define XmCSelectPixmap	 	"SelectPixmap"
#define XmCSet			"Set"
#define XmCSimpleCheckBox	"SimpleCheckBox"
#define XmCSimpleMenuBar	"SimpleMenuBar"
#define XmCSimplePopupMenu	"SimplePopupMenu"
#define XmCSimplePulldownMenu	"SimplePulldownMenu"
#define XmCSimpleOptionMenu	"SimpleOptionMenu"
#define XmCSimpleRadioBox	"SimpleRadioBox"
#define XmCSpace		"Space"
#define XmCSpacing		"Spacing"
#define XmCString		"String"
#define XmCStringDirection	"StringDirection"
#define XmCTextOptions		"TextOptions"
#define XmCTextPosition		"TextPosition"
#define XmCTextSink		"TextSink"
#define XmCTextSource		"TextSource"
#define XmCThickness		"Thickness"
#define XmCThumb		"Thumb"
#define XmCTroughColor		"TroughColor"
#define XmCTranslations		"Translations"
#define XmCTraversalType        "TraversalType"
#define XmCUnselectPixmap 	"UnselectPixmap"
#define XmCValue		"Value"
#define XmCValueChangedCallback	"ValueChangedCallback"
#define XmCVisibleWhenOff	"VisibleWhenOff"
#define XmCVSpace		"VSpace"
#define XmCWhichButton	        "WhichButton"
#define XmCWidth		"Width"
#define XmCWindow		"Window"
#define XmCXmString             "XmString"
#define XmCX			"X"
#define XmCY			"Y"


/* Representation types */

#define XmRAcceleratorTable     "AcceleratorTable"
#define XmRAlignment		"Alignment"
#define XmRXmBackgroundPixmap			"XmBackgroundPixmap"
#define XmRBool		"Bool"
#define XmRBoolean		"Boolean"
#define XmRButtonType		"ButtonType"
#define XmRCallback             "Callback"
#define XmRCallbackProc             "CallbackProc"
#define XmRCallProc             "CallProc"
#define XmRChar			"Char"
#define XmRCharSetTable			"CharSetTable"
#define XmRColor		"Color"
#define XmRCommandWindowLocation "CommandWindowLocation"
#define XmRCompoundText		"CompoundText"
#define XmRCursor		"Cursor"
#define XmRDimension		"Dimension"
#define XmRDisplay		"Display"
#define XmREditMode		"EditMode"
#define XmRFile			"File"
#define XmRFont			"Font"
#define XmRFontList		"FontList"
#define XmRFontStruct		"FontStruct"
#define XmRFunction		"Function"
#define XmRGeometry		"Geometry"
#define XmRHorizontalDimension	"HorizontalDimension"
#define XmRHorizontalPosition	"HorizontalPosition"
#define XmRImmediate		"Immediate"
#define XmRIndicatorType	"IndicatorType"
#define XmRInt			"Int"
#define XmRJustify		"Justify"
#define XmRKeySym               "KeySym"
#define XmRKeySymTable               "KeySymTable"
#define XmRLabelType		"LabelType"
#define XmRLongBoolean		XmRBool		/* Compatibility */
#define XmRMenuWidget           "MenuWidget"
#define XmRMnemonic		"Mnemonic"
#define XmRNavigationType	"NavigationType"
#define XmROrientation		"Orientation"
#define XmRPacking		"Packing"
#define XmRPixel		"Pixel"
#define XmRPixmap		"Pixmap"
#define XmRGadgetPixmap		"GadgetPixmap"
#define XmRPointer		"Pointer"
#define XmRPosition		"Position"
#define XmRProc			"Proc"
#define XmRRowColumnType	"RowColumnType"
#define XmRShort                "Short"
#define XmRString		"String"
#define XmRStringDirection	"StringDirection"
#define XmRStringTable		"StringTable"
#define XmRTextPosition         "TextPosition"
#define XmRUnsignedChar         "UnsignedChar"
#define XmRVerticalDimension	"VerticalDimension"
#define XmRVerticalPosition	"VerticalPosition"
#define XmRTranslationTable     "TranslationTable"
#define XmRTraversalType     	"TraversalType"
#define XmRWhichButton		"WhichButton"
#define XmRWidget		"Widget"
#define XmRWidgetList		"WidgetList"
#define XmRWidgetClass		"WidgetClass"
#define XmRWindow		"Window"
#define XmRXmString		"XmString"
#define XmRXmStringTable	"XmStringTable"
#define XmRStringCharSet    	"XmStringCharSet"

/************************************************************************
 *  Resource names
 *	Taken from Shell.h, Xt replaced by Xm
 ************************************************************************/
#define XmNiconName	"iconName"
#define XmCIconName	"IconName"
#define XmNiconPixmap	"iconPixmap"
#define XmCIconPixmap	"IconPixmap"
#define XmNiconWindow	"iconWindow"
#define XmCIconWindow	"IconWindow"
#define XmNiconMask	"iconMask"
#define XmCIconMask	"IconMask"
#define XmNwindowGroup	"windowGroup"
#define XmCWindowGroup	"WindowGroup"
 
#define XmNsaveUnder		"saveUnder"
#define XmCSaveUnder		"SaveUnder"
#define XmNtransient		"transient"
#define XmCTransient		"Transient"
#define XmNoverrideRedirect	"overrideRedirect"
#define XmCOverrideRedirect	"OverrideRedirect"

#define XmNallowShellResize	"allowShellResize"
#define XmCAllowShellResize	"AllowShellResize"
#define XmNcreatePopupChildProc	"createPopupChildProc"
#define XmCCreatePopupChildProc	"CreatePopupChildProc"

#define XmNtitle	"title"
#define XmCTitle	"Title"

/* 
 * The following are only used at creation and can not be changed via 
 * SetValues.
 */

#define XmNargc		"argc"
#define XmCArgc		"Argc"
#define XmNargv		"argv"
#define XmCArgv		"Argv"
#define XmNiconX	"iconX"
#define XmCIconX	"IconX"
#define XmNiconY	"iconY"
#define XmCIconY	"IconY"
#define XmNinput	"input"
#define XmCInput	"Input"
#define XmNiconic	"iconic"
#define XmCIconic	"Iconic"
#define XmNinitialState	"initialState"
#define XmCInitialState	"InitialState"
#define XmNgeometry	"geometry"
#define XmCGeometry	"Geometry"
#define XmNminWidth	"minWidth"
#define XmCMinWidth	"MinWidth"
#define XmNminHeight	"minHeight"
#define XmCMinHeight	"MinHeight"
#define XmNmaxWidth	"maxWidth"
#define XmCMaxWidth	"MaxWidth"
#define XmNmaxHeight	"maxHeight"
#define XmCMaxHeight	"MaxHeight"
#define XmNwidthInc	"widthInc"
#define XmCWidthInc	"WidthInc"
#define XmNheightInc	"heightInc"
#define XmCHeightInc	"HeightInc"
#define XmNminAspectY	"minAspectY"
#define XmCMinAspectY	"MinAspectY"
#define XmNmaxAspectY	"maxAspectY"
#define XmCMaxAspectY	"MaxAspectY"
#define XmNminAspectX		"minAspectX"
#define XmCMinAspectX		"MinAspectX"
#define XmNmaxAspectX		"maxAspectX"
#define XmCMaxAspectX		"MaxAspectX"
#define XmNwmTimeout		"wmTimeout"
#define XmCWmTimeout		"WmTimeout"
#define XmNwaitForWm		"waitforwm"
#define XmCWaitForWm		"Waitforwm"


/****************
 *
 * New R4 psudo defines
 *
 ****************/
#define XmCReadOnly     XtCReadOnly
#define XmNvisual       XtNvisual
#define XmCVisual       XtCVisual
#define XmNiconNameEncoding     XtNiconNameEncoding
#define XmCIconNameEncoding     XtCIconNameEncoding
#define XmNtransientFor     XtNtransientFor
#define XmCTransientFor     XtCTransientFor
#define XmNbaseHeight       XtNbaseHeight
#define XmCBaseHeight       XtCBaseHeight
#define XmNbaseWidth        XtNbaseWidth
#define XmCBaseWidth        XtCBaseWidth
#define XmNtitleEncoding        XtNtitleEncoding
#define XmCTitleEndcoding       XtCTitleEndcoding
#define XmNwinGravity       XtNwinGravity
#define XmCWinGravity       XtCWinGravity

/************************************************************************
 *  Include VendorE.h in order to get it's resources, so a caller
 *  doesn't have to include it explicitly
 ************************************************************************/
#ifndef _VendorE_h
#include <Xm/VendorE.h>
#endif /* _VendorE_h */

#ifndef XmEXPLICIT
#define XmEXPLICIT 0
#define XmPOINTER 1
#endif


/************************************************************************
 *
 *  Base widget class and record definitions.
 *	Included are the definitions for XmPrimitive, XmManager,
 *      and XmGadget.
 *
 ************************************************************************/

/*  Primitive widget class and record definitions  */

#ifndef PRIMITIVE
externalref WidgetClass xmPrimitiveWidgetClass;
#endif

typedef struct _XmPrimitiveClassRec * XmPrimitiveWidgetClass;
typedef struct _XmPrimitiveRec      * XmPrimitiveWidget;


/*  Gadget widget class and record definitions  */

#ifndef GADGET
externalref WidgetClass xmGadgetClass;
#endif

typedef struct _XmGadgetClassRec * XmGadgetClass;
typedef struct _XmGadgetRec      * XmGadget;


/*  Manager widger class and record definitions  */


#ifndef MANAGER
externalref WidgetClass xmManagerWidgetClass;
#endif

typedef struct _XmManagerClassRec * XmManagerWidgetClass;
typedef struct _XmManagerRec      * XmManagerWidget;


/************************************************************************
 *  Fast subclassing -- just do XtIsSubclass now.  To be replaced.
 ************************************************************************/

#ifndef XmIsPrimitive
#define XmIsPrimitive(w)	XtIsSubclass(w, xmPrimitiveWidgetClass)
#endif  /* XmIsPrimitive */

#ifndef XmIsGadget
#define XmIsGadget(w)	XtIsSubclass(w, xmGadgetClass)
#endif  /* XmIsGadget */

#ifndef XmIsManager
#define XmIsManager(w)	XtIsSubclass(w, xmManagerWidgetClass)
#endif  /* XmIsManager */


/************************************************************************
 *  Primitive Resources and define values
 ************************************************************************/

#define XmNforeground		"foreground"
#define XmCForeground		"Foreground"

#define XmRPrimForegroundPixmap	"PrimForegroundPixmap"
#define XmRManForegroundPixmap	"ManForegroundPixmap"

#define XmCBackgroundPixmap	"BackgroundPixmap"
#define XmRBackgroundPixmap	"BackgroundPixmap"

#define XmNtraversalOn		"traversalOn"
#define XmCTraversalOn		"TraversalOn"

#define XmNhighlightOnEnter	"highlightOnEnter"
#define XmCHighlightOnEnter	"HighlightOnEnter"

#define XmNsizePolicy		"sizePolicy"
#define XmCSizePolicy		"SizePolicy"
#define XmRSizePolicy		"SizePolicy"

#define XmNhighlightThickness	"highlightThickness"
#define XmCHighlightThickness	"HighlightThickness"

#define XmNhighlightColor	"highlightColor"
#define XmCHighlightColor	"HighlightColor"

#define XmNhighlightPixmap	"highlightPixmap"
#define XmCHighlightPixmap	"HighlightPixmap"
#define XmRPrimHighlightPixmap	"HighlightPixmap"

#define XmNshadowThickness	"shadowThickness"
#define XmCShadowThickness	"ShadowThickness"

#define XmNtopShadowColor	"topShadowColor"
#define XmCTopShadowColor	"TopShadowColor"

#define XmNtopShadowPixmap	"topShadowPixmap"
#define XmCTopShadowPixmap	"TopShadowPixmap"
#define XmRPrimTopShadowPixmap	"TopShadowPixmap"

#define XmNbottomShadowColor	"bottomShadowColor"
#define XmCBottomShadowColor	"BottomShadowColor"

#define XmNbottomShadowPixmap		"bottomShadowPixmap"
#define XmCBottomShadowPixmap		"BottomShadowPixmap"
#define XmRPrimBottomShadowPixmap	"BottomShadowPixmap"

#define XmNunitType		"unitType"
#define XmCUnitType		"UnitType"
#define XmRUnitType		"UnitType"

#define XmNhelpCallback		"helpCallback"
#define XmNuserData		"userData"
#define XmCUserData		"UserData"


/* size policy values  */

#define XmCHANGE_ALL	0
#define XmCHANGE_NONE	1
#define XmCHANGE_WIDTH	2
#define XmCHANGE_HEIGHT	3


/*  unit type values  */

#define XmPIXELS		0
#define Xm100TH_MILLIMETERS	1
#define Xm1000TH_INCHES		2
#define Xm100TH_POINTS		3
#define Xm100TH_FONT_UNITS	4


/************************************************************************
 *  Navigation defines 
 ************************************************************************/
#ifndef XmNONE
#define XmNONE	 		0
#endif /* XmNONE */
#define XmTAB_GROUP		1
#define XmSTICKY_TAB_GROUP	2
#define XmEXCLUSIVE_TAB_GROUP	3
#define XmDYNAMIC_DEFAULT_TAB_GROUP	255

/************************************************************************
 *  Manager Resources and define values
 ************************************************************************/

#define XmRManTopShadowPixmap		"ManTopShadowPixmap"
#define XmRManBottomShadowPixmap	"ManBottomShadowPixmap"
#define XmRManHighlightPixmap		"ManHighlightPixmap"

#define XmNchildPosition	"childPosition"



/************************************************************************
 *  Menu defines
 ************************************************************************/

#define XmNO_ORIENTATION       0
#define XmVERTICAL	       1
#define XmHORIZONTAL           2

#define XmWORK_AREA	       0
#define XmMENU_BAR	       1
#define XmMENU_PULLDOWN        2
#define XmMENU_POPUP           3
#define XmMENU_OPTION          4

#define XmNO_PACKING           0
#define XmPACK_TIGHT	       1
#define XmPACK_COLUMN	       2
#define XmPACK_NONE	       3


/************************************************************************
 *  Label defines
 ************************************************************************/

#define XmALIGNMENT_BEGINNING		0
#define XmALIGNMENT_CENTER		1
#define XmALIGNMENT_END			2


/************************************************************************
 *  ToggleButton  defines
 ************************************************************************/

#define XmN_OF_MANY			1
#define XmONE_OF_MANY			2


/************************************************************************
 *  Form resources and defines
 ************************************************************************/

/*  Form resources  */

#define XmNhorizontalSpacing	"horizontalSpacing"
#define XmNverticalSpacing	"verticalSpacing"

#define XmNfractionBase		"fractionBase"
#define XmCMaxValue		"MaxValue"

#define XmNrubberPositioning	"rubberPositioning"
#define XmCRubberPositioning	"RubberPositioning"

#define XmNresizePolicy		"resizePolicy"
#define XmCResizePolicy		"ResizePolicy"
#define XmRResizePolicy		"ResizePolicy"


/*  Form constraint resources  */

#define XmNtopAttachment	"topAttachment"
#define XmNbottomAttachment	"bottomAttachment"
#define XmNleftAttachment	"leftAttachment"
#define XmNrightAttachment	"rightAttachment"
#define XmCAttachment		"Attachment"
#define XmRAttachment		"Attachment"

#define XmNtopWidget		"topWidget"
#define XmNbottomWidget		"bottomWidget"
#define XmNleftWidget		"leftWidget"
#define XmNrightWidget		"rightWidget"
#define XmCWidget		"Widget"

#define XmNtopPosition		"topPosition"
#define XmNbottomPosition	"bottomPosition"
#define XmNleftPosition		"leftPosition"
#define XmNrightPosition	"rightPosition"
#define XmNtopOffset		"topOffset"
#define XmNbottomOffset		"bottomOffset"
#define XmNleftOffset		"leftOffset"
#define XmNrightOffset		"rightOffset"
#define XmCOffset		"Offset"

#define XmNresizable		"resizable"


/*  Form defines  */

#define XmATTACH_NONE			0
#define XmATTACH_FORM			1
#define XmATTACH_OPPOSITE_FORM		2
#define XmATTACH_WIDGET			3
#define XmATTACH_OPPOSITE_WIDGET	4
#define XmATTACH_POSITION		5
#define XmATTACH_SELF			6

#define XmRESIZE_NONE	0
#define XmRESIZE_GROW	1
#define XmRESIZE_ANY	2	/*  for BulletinBoard, DrawingArea  */ 



/****************************************************************************
 *  Callback reasons 
 ****************************************************************************/

#define XmCR_NONE            		0
#define XmCR_HELP            		1
#define XmCR_VALUE_CHANGED		2
#define XmCR_INCREMENT			3
#define XmCR_DECREMENT			4
#define XmCR_PAGE_INCREMENT		5
#define XmCR_PAGE_DECREMENT		6
#define XmCR_TO_TOP			7
#define XmCR_TO_BOTTOM			8
#define XmCR_DRAG			9
#define XmCR_ACTIVATE			10
#define XmCR_ARM			11
#define XmCR_DISARM			12
#define XmCR_MAP       			16
#define XmCR_UNMAP  			17
#define XmCR_FOCUS			18
#define XmCR_LOSING_FOCUS		19
#define XmCR_MODIFYING_TEXT_VALUE	20
#define XmCR_MOVING_INSERT_CURSOR	21
#define XmCR_EXECUTE			22
#define	XmCR_SINGLE_SELECT		23
#define	XmCR_MULTIPLE_SELECT		24
#define	XmCR_EXTENDED_SELECT		25
#define	XmCR_BROWSE_SELECT		26
#define	XmCR_DEFAULT_ACTION		27
#define XmCR_CLIPBOARD_DATA_REQUEST	28
#define XmCR_CLIPBOARD_DATA_DELETE	29
#define XmCR_CASCADING			30
#define XmCR_OK                         31
#define XmCR_CANCEL                     32
#define XmCR_APPLY                      34
#define XmCR_NO_MATCH                   35
#define XmCR_COMMAND_ENTERED            36
#define XmCR_COMMAND_CHANGED            37
#define XmCR_EXPOSE			38
#define XmCR_RESIZE			39
#define XmCR_INPUT			40
#define XmCR_GAIN_PRIMARY		41
#define XmCR_LOSE_PRIMARY		42


/************************************************************************
 *  Callback structures 
 ************************************************************************/

typedef struct
{
    int     reason;
    XEvent  *event;
} XmAnyCallbackStruct;

typedef struct
{
    int     reason;
    XEvent  *event;
    int	    click_count;
} XmArrowButtonCallbackStruct;

typedef struct
{
    int     reason;
    XEvent  *event;
    Window  window;
} XmDrawingAreaCallbackStruct;

typedef struct
{
    int     reason;
    XEvent  *event;
    Window  window;
    int	    click_count;
} XmDrawnButtonCallbackStruct;

typedef struct
{
    int     reason;
    XEvent  *event;
    int	    click_count;
} XmPushButtonCallbackStruct;

typedef struct
{
    int     reason;
    XEvent  *event;
    Widget  widget;
    char    *data;
    char    *callbackstruct;
} XmRowColumnCallbackStruct;

typedef struct
{
   int reason;
   XEvent * event;
   int value;
   int pixel;
} XmScrollBarCallbackStruct;

typedef struct
{
   int reason;
   XEvent * event;
   int set;
} XmToggleButtonCallbackStruct;

typedef struct
{
   int 	     reason;
   XEvent    *event;
   XmString  item;
   int       item_length;
   int       item_position;
   XmString  *selected_items;
   int       selected_item_count;
   int       *selected_item_positions;
   char      selection_type;
} XmListCallbackStruct;

typedef struct
{
    int reason;
    XEvent	*event;
    XmString	value;
    int		length;
} XmSelectionBoxCallbackStruct;

typedef struct
{
    int reason;
    XEvent	*event;
    XmString	value;
    int		length;
} XmCommandCallbackStruct;

typedef struct
{
    int 	reason;
    XEvent	*event;
    XmString	value;
    int		length;
    XmString	mask;
    int		mask_length;
    XmString	dir ;
    int		dir_length ;
    XmString    pattern ;
    int		pattern_length ;
} XmFileSelectionBoxCallbackStruct;


typedef struct 
{
   int reason;
   XEvent * event;
   int value;
} XmScaleCallbackStruct;



/************************************************************************
 *  PushButton defines
 ************************************************************************/

#define XmNfillOnArm		"fillOnArm"
#define XmCFillOnArm		"FillOnArm"

#define XmNarmColor		"armColor"
#define XmCArmColor		"ArmColor"

#define XmNarmPixmap		"armPixmap"
#define XmCArmPixmap		"ArmPixmap"

#define XmNshowAsDefault		"showAsDefault"
#define XmCShowAsDefault		"ShowAsDefault"
#define XmRBooleanDimension		"BooleanDimension"

#define XmNdefaultButtonShadowThickness    "defaultButtonShadowThickness"
#define XmCDefaultButtonShadowThickness    "DefaultButtonShadowThickness"

#define XmNmultiClick	"multiClick"
#define XmCMultiClick	"MultiClick"
#define XmRMultiClick   "MultiClick"

#define XmMULTICLICK_DISCARD 0
#define XmMULTICLICK_KEEP 1

/************************************************************************
 *  DrawnButton defines
 ************************************************************************/

#define XmNpushButtonEnabled	"pushButtonEnabled"
#define XmCPushButtonEnabled	"PushButtonEnabled"

#define XmNshadowType	"shadowType"
#define XmCShadowType	"ShadowType"
#define XmRShadowType   "ShadowType"

#define XmSHADOW_IN		7
#define XmSHADOW_OUT		8

/************************************************************************
 *  Arrow defines
 ************************************************************************/

#define XmNarrowDirection	"arrowDirection"
#define XmCArrowDirection	"ArrowDirection"
#define XmRArrowDirection	"ArrowDirection"

#define XmNactivateCallback	"activateCallback"
#define XmNhelpCallback		"helpCallback"

#define XmARROW_UP	0
#define XmARROW_DOWN	1
#define XmARROW_LEFT	2
#define XmARROW_RIGHT	3



/************************************************************************
 *  Separator defines
 ************************************************************************/

#define XmNmargin 	"margin"
#define XmRMargin	"Margin"

#define XmNseparatorType  "separatorType"
#define XmCSeparatorType  "SeparatorType"
#define XmRSeparatorType  "SeparatorType"

#define XmNO_LINE               0
#define XmSINGLE_LINE           1
#define XmDOUBLE_LINE           2
#define XmSINGLE_DASHED_LINE    3
#define XmDOUBLE_DASHED_LINE    4
#define XmSHADOW_ETCHED_IN	5
#define XmSHADOW_ETCHED_OUT	6

#define XmPIXMAP	1
#define XmSTRING	2



/************************************************************************
 *  ScrollBar resource names and #defines
 ************************************************************************/

/*  Resources for scrollbar  */

#define XmNminimum	"minimum"
#define XmCMinimum	"Minimum"

#define XmNmaximum	"maximum"
#define XmCMaximum	"Maximum"

#define XmNsliderSize	"sliderSize"
#define XmCSliderSize	"SliderSize"

#define XmNshowArrows	"showArrows"
#define XmCShowArrows	"ShowArrows"

#define XmNprocessingDirection	"processingDirection"
#define XmCProcessingDirection	"ProcessingDirection"
#define XmRProcessingDirection	"ProcessingDirection"

#define XmNincrement		"increment"
#define XmCIncrement		"Increment"
#define XmNpageIncrement	"pageIncrement"
#define XmCPageIncrement	"PageIncrement"

#define XmNinitialDelay		"initialDelay"
#define XmCInitialDelay		"InitialDelay"
#define XmNrepeatDelay		"repeatDelay"
#define XmCRepeatDelay		"RepeatDelay"

#define XmNvalueChangedCallback		"valueChangedCallback"
#define XmNincrementCallback		"incrementCallback"
#define XmNdecrementCallback		"decrementCallback"
#define XmNpageIncrementCallback	"pageIncrementCallback"
#define XmNpageDecrementCallback	"pageDecrementCallback"
#define XmNtoTopCallback		"toTopCallback"
#define XmNtoBottomCallback		"toBottomCallback"
#define XmNdragCallback			"dragCallback"


/*  Defined values for scrollbar  */

#define XmMAX_ON_TOP	0
#define XmMAX_ON_BOTTOM	1
#define XmMAX_ON_LEFT	2
#define XmMAX_ON_RIGHT	3


/************************************************************************
 *									*
 * List Widget defines							*
 *									*
 ************************************************************************/


#define XmNlistSpacing 	"listSpacing"
#define XmCListSpacing	"ListSpacing"
#define XmRListSpacing	"ListSpacing"

#define XmNlistMarginWidth 	"listMarginWidth"
#define XmCListMarginWidth	"ListMarginWidth"
#define XmRListMarginWidth	"ListMarginWidth"

#define XmNlistMarginHeight 	"listMarginHeight"
#define XmCListMarginHeight	"ListMarginHeight"
#define XmRListMarginHeight	"ListMarginHeight"

#define XmNitems 	"items"
#define XmCItems	"Items"
#define XmRItems	"Items"

#define XmNitemCount 	"itemCount"
#define XmCItemCount	"ItemCount"
#define XmRItemCount	"ItemCount"

#define XmNselectedItems 	"selectedItems"
#define XmCSelectedItems	"SelectedItems"
#define XmRSelectedItems	"SelectedItems"

#define XmNselectedItemCount 	"selectedItemCount"
#define XmCSelectedItemCount	"SelectedItemCount"
#define XmRSelectedItemCount	"SelectedItemCount"

#define XmNvisibleItemCount 	"visibleItemCount"
#define XmCVisibleItemCount	"VisibleItemCount"
#define XmRVisibleItemCount	"VisibleItemCount"

#define XmNselectionPolicy 	"selectionPolicy"
#define XmCSelectionPolicy	"SelectionPolicy"
#define XmRSelectionPolicy	"SelectionPolicy"

#define XmNlistSizePolicy 	"listSizePolicy"
#define XmCListSizePolicy	"ListSizePolicy"
#define XmRListSizePolicy	"ListSizePolicy"

#define XmNdoubleClickInterval 	"doubleClickInterval"
#define XmCDoubleClickInterval	"DoubleClickInterval"
#define XmRDoubleClickInterval	"DoubleClickInterval"

#define XmNsingleSelectionCallback 	"singleSelectionCallback"
#define XmNmultipleSelectionCallback 	"multipleSelectionCallback"
#define XmNextendedSelectionCallback 	"extendedSelectionCallback"
#define XmNbrowseSelectionCallback 	"browseSelectionCallback"
#define XmNdefaultActionCallback 	"defaultActionCallback"

#define XmNautomaticSelection	"automaticSelection"
#define XmCAutomaticSelection	"AutomaticSelection"

#define XmNtopItemPosition      "topItemPosition"
#define XmCTopItemPosition      "TopItemPosition"

/****************
 *
 * Selection types
 *
 ****************/
#define	XmSINGLE_SELECT		0
#define	XmMULTIPLE_SELECT	1
#define	XmEXTENDED_SELECT	2
#define	XmBROWSE_SELECT		3

#define	XmSTATIC	0
#define	XmDYNAMIC	1

/************************************************************************
 *									*
 * Scrolled Window defines.						*
 *									*
 ************************************************************************/
#define XmNhorizontalScrollBar	"horizontalScrollBar"
#define XmCHorizontalScrollBar	"HorizontalScrollBar"

#define XmNverticalScrollBar	"verticalScrollBar"
#define XmCVerticalScrollBar	"VerticalScrollBar"

#define XmNworkWindow	"workWindow"
#define XmCWorkWindow	"WorkWindow"

#define XmNclipWindow	"clipWindow"
#define XmCClipWindow	"ClipWindow"

#define XmNscrollingPolicy	"scrollingPolicy"
#define XmCScrollingPolicy	"ScrollingPolicy"
#define XmRScrollingPolicy	"ScrollingPolicy"

#define  XmNvisualPolicy	"visualPolicy"
#define  XmCVisualPolicy	"VisualPolicy"
#define  XmRVisualPolicy	"VisualPolicy"

#define XmNscrollBarDisplayPolicy	"scrollBarDisplayPolicy"
#define XmCScrollBarDisplayPolicy	"ScrollBarDisplayPolicy"
#define XmRScrollBarDisplayPolicy	"ScrollBarDisplayPolicy"

#define XmNscrollBarPlacement	"scrollBarPlacement"
#define XmCScrollBarPlacement	"ScrollBarPlacement"
#define XmRScrollBarPlacement	"ScrollBarPlacement"

#define XmNupdateSliderSize	"updateSliderSize"
#define XmCUpdateSliderSize	"UpdateSliderSize"

#define XmNscrolledWindowMarginHeight	"scrolledWindowMarginHeight"
#define XmCScrolledWindowMarginHeight	"ScrolledWindowMarginHeight"

#define XmNscrolledWindowMarginWidth	"scrolledWindowMarginWidth"
#define XmCScrolledWindowMarginWidth	"ScrolledWindowMarginWidth"


#define XmVARIABLE	        0
#define XmCONSTANT	        1
#define XmRESIZE_IF_POSSIBLE	2

#define XmAUTOMATIC		0
#define XmAPPLICATION_DEFINED	1

/* #define	XmSTATIC	0     ** This is already defined by List ***/
#define	XmAS_NEEDED	1

#define SW_TOP		1
#define SW_BOTTOM	0
#define SW_LEFT		2
#define SW_RIGHT	0

#define XmTOP_LEFT	(SW_TOP | SW_LEFT)
#define XmBOTTOM_LEFT	(SW_BOTTOM  | SW_LEFT)
#define XmTOP_RIGHT	(SW_TOP | SW_RIGHT)
#define XmBOTTOM_RIGHT	(SW_BOTTOM  | SW_RIGHT)

/************************************************************************
 *									*
 * MainWindow Resources                                                 *
 *									*
 ************************************************************************/


#define XmNcommandWindow		"commandWindow"
#define XmCCommandWindow		"CommandWindow"
#define XmNmenuBar			"menuBar"
#define XmCMenuBar			"MenuBar"
#define XmNmessageWindow		"messageWindow"
#define XmCMessageWindow		"MessageWindow"
#define XmNmainWindowMarginHeight	"mainWindowMarginHeight"
#define XmCMainWindowMarginHeight	"MainWindowMarginHeight"
#define XmNmainWindowMarginWidth	"mainWindowMarginWidth"
#define XmCMainWindowMarginWidth	"MainWindowMarginWidth"
#define XmNshowSeparator    "showSeparator"
#define XmCShowSeparator    "ShowSeparator"

#define XmCOMMAND_ABOVE_WORKSPACE 0
#define XmCOMMAND_BELOW_WORKSPACE 1

/************************************************************************
 *									*
 * Text Widget defines							*
 *									*
 ************************************************************************/

#define XmNsource		"source"
#define XmCSource		"Source"

#define XmNoutputCreate		"outputCreate"
#define XmCOutputCreate		"OutputCreate"

#define XmNinputCreate		"inputCreate"
#define XmCInputCreate		"InputCreate"

#define XmNautoShowCursorPosition	"autoShowCursorPosition"
#define XmCAutoShowCursorPosition	"AutoShowCursorPosition"

#define XmNcursorPosition	"cursorPosition"
#define XmCCursorPosition	"CursorPosition"

#define XmNeditable		"editable"
#define XmCEditable		"Editable"

#define XmNmaxLength		"maxLength"
#define XmCMaxLength		"MaxLength"

#define XmNfocusCallback	 "focusCallback"
#define XmNlosingFocusCallback	 "losingFocusCallback"
#define XmNmodifyVerifyCallback  "modifyVerifyCallback"
#define XmNmotionVerifyCallback  "motionVerifyCallback"
#define XmNgainPrimaryCallback  "gainPrimaryCallback"
#define XmNlosePrimaryCallback  "losePrimaryCallback"

#define XmNverifyBell                   "verifyBell"
#define XmCVerifyBell                   "VerifyBell"

#define XmNwordWrap		"wordWrap"
#define XmCWordWrap		"WordWrap"

#define XmNblinkRate		"blinkRate"
#define XmCBlinkRate		"BlinkRate"

#define XmNresizeWidth		"resizeWidth"
#define XmCResizeWidth		"ResizeWidth"

#define XmNresizeHeight	"resizeHeight"
#define XmCResizeHeight	"ResizeHeight"

#define XmNscrollHorizontal	"scrollHorizontal"
#define XmNscrollVertical	"scrollVertical"
#define XmCScroll		"Scroll"

#define XmNscrollLeftSide	"scrollLeftSide"
#define XmNscrollTopSide	"scrollTopSide"
#define XmCScrollSide		"ScrollSide"

#define XmNcursorPositionVisible	"cursorPositionVisible"
#define XmCCursorPositionVisible	"CursorPositionVisible"

#define XmNtoPositionCallback	"toPositionCallback"

#define XmNcolumns	"columns"
#define XmCColumns	"Columns"

#define XmNrows		"rows"
#define XmCRows		"Rows"

#define XmNselectThreshold		"selectThreshold"
#define XmCSelectThreshold		"SelectThreshold"

#define XmNselectionArrayCount		"selectionArrayCount"
#define XmCSelectionArrayCount		"SelectionArrayCount"

#define XmNpendingDelete		"pendingDelete"
#define XmCPendingDelete		"PendingDelete"

#define XmNeditMode			"editMode"
#define XmCEditMode			"EditMode"

#define XmMULTI_LINE_EDIT	0
#define XmSINGLE_LINE_EDIT	1

typedef long XmTextPosition;
typedef Atom XmTextFormat;

#define FMT8BIT         ((XmTextFormat) XA_STRING)      /* 8-bit text. */
#define FMT16BIT        ((XmTextFormat)2)               /* 16-bit text. */

typedef enum { XmSELECT_POSITION, XmSELECT_WHITESPACE, XmSELECT_WORD,
               XmSELECT_LINE, XmSELECT_ALL, XmSELECT_PARAGRAPH } XmTextScanType;

typedef enum {XmHIGHLIGHT_NORMAL, XmHIGHLIGHT_SELECTED,
	      XmHIGHLIGHT_SECONDARY_SELECTED} XmHighlightMode;

/* XmTextBlock's are used to pass text around. */

typedef struct {
    char *ptr;                  /* Pointer to data. */
    int length;                 /* Number of bytes of data. */
    XmTextFormat format;       /* Representations format */
} XmTextBlockRec, *XmTextBlock;

typedef struct
{
    int reason;
    XEvent  *event;
    Boolean doit;
    long currInsert, newInsert;
    long startPos, endPos;
    XmTextBlock text;
} XmTextVerifyCallbackStruct, *XmTextVerifyPtr;

#define XmNtopCharacter                 "topCharacter"
#define XmCtopCharacter                 "TopCharacter"

/* functions renamed after 1.0 release due to resource name overlap */
#define XmGetTopPosition                XmGetTopCharacter
#define XmSetTopPosition                XmSetTopCharacter

/************************************************************************
 *									*
 * VPaned Widget defines						*
 *									*
 ************************************************************************/

/*  Resources for Vertical Paned Window  */

#define XmNrefigureMode		"refigureMode"

#define XmNseparatorOn		"separatorOn"
#define XmCSeparatorOn		"SeparatorOn"

#define XmNsashIndent		"sashIndent"
#define XmCSashIndent		"SashIndent"

#define XmNsashWidth		"sashWidth"
#define XmCSashWidth		"SashWidth"

#define XmNsashHeight		"sashHeight"
#define XmCSashHeight		"SashHeight"

#define XmNsashShadowThickness	"sashShadowThickness"


/* Constraint Resources for Vertical Paned Window */

#define XmNallowResize		"allowResize"
#define XmNskipAdjust		"skipAdjust"
#define XmNpaneMinimum          "paneMinimum"
#define XmNpaneMaximum          "paneMaximum"

#define XmCPaneMinimum          "PaneMinimum"
#define XmCPaneMaximum          "PaneMaximum"



/************************************************************************
 *									*
 * 	DrawingArea defines
 *									*
 ************************************************************************/

#define XmNinputCallback	"inputCallback"


/************************************************************************
 *									*
 *  DIALOG defines..  BulletinBoard and things common to its subclasses *
 *          CommandBox    MessageBox    Selection    FileSelection      *
 *									*
 ************************************************************************/

/* child type defines for Xm...GetChild() */
#define XmDIALOG_NONE	          0       /* a valid default button type */
#define XmDIALOG_APPLY_BUTTON	  1
#define XmDIALOG_CANCEL_BUTTON    2
#define XmDIALOG_DEFAULT_BUTTON   3
#define XmDIALOG_OK_BUTTON        4
#define XmDIALOG_FILTER_LABEL     5
#define XmDIALOG_FILTER_TEXT      6
#define XmDIALOG_HELP_BUTTON      7
#define XmDIALOG_LIST		  8
#define XmDIALOG_HISTORY_LIST     XmDIALOG_LIST
#define XmDIALOG_LIST_LABEL	  9
#define XmDIALOG_MESSAGE_LABEL    10
#define XmDIALOG_SELECTION_LABEL  11
#define XmDIALOG_PROMPT_LABEL     XmDIALOG_SELECTION_LABEL
#define XmDIALOG_SYMBOL_LABEL     12
#define XmDIALOG_TEXT	    	  13
#define XmDIALOG_VALUE_TEXT       XmDIALOG_TEXT
#define XmDIALOG_COMMAND_TEXT     XmDIALOG_TEXT
#define XmDIALOG_SEPARATOR    	  14
#define XmDIALOG_DIR_LIST         15
#define XmDIALOG_DIR_LIST_LABEL   16
#define XmDIALOG_FILE_LIST        XmDIALOG_LIST
#define XmDIALOG_FILE_LIST_LABEL  XmDIALOG_LIST_LABEL

/* defines for callbacks */
#define XmNokCallback             "okCallback"
#define XmNcancelCallback         "cancelCallback"
#define XmNapplyCallback	  "applyCallback"
#define XmNnoMatchCallback	  "noMatchCallback"
#define XmNcommandEnteredCallback "commandEnteredCallback"
#define XmNcommandChangedCallback "commandChangedCallback"


/* XmN defines for subwidgets */
#define XmNokLabelString         "okLabelString"
#define XmNcancelLabelString     "cancelLabelString"
#define XmNhelpLabelString       "helpLabelString"
#define XmNapplyLabelString	 "applyLabelString"
#define XmNselectionLabelString	 "selectionLabelString"
#define XmNlistLabelString	 "listLabelString"
#define XmNpromptString          "promptString"


/* XmC defines for subwidgets */
#define XmCOkLabelString	 "OkLabelString"
#define XmCCancelLabelString	 "CancelLabelString"
#define XmCHelpLabelString	 "HelpLabelString"
#define XmCApplyLabelString	 "ApplyLabelString"
#define XmCSelectionLabelString	 "SelectionLabelString"
#define XmCListLabelString	 "ListLabelString"
#define XmCPromptString          "PromptString"
#define XmCMessageString         "MessageString"

#define XmNdefaultButton	"defaultButton"
#define XmNcancelButton		"cancelButton"

#define XmNbuttonFontList	"buttonFontList"
#define XmCButtonFontList	"ButtonFontList"
#define XmNlabelFontList	"labelFontList"
#define XmCLabelFontList	"LabelFontList"
#define XmNtextFontList		"textFontList"
#define XmCTextFontList		"TextFontList"

#define XmNtextTranslations	"textTranslations"

#define XmNallowOverlap		"allowOverlap"
#define XmCAllowOverlap		"AllowOverlap"

#define XmNdefaultPosition	"defaultPosition"
#define XmCDefaultPosition	"DefaultPosition"

#define XmNautoUnmanage		"autoUnmanage"
#define XmCAutoUnmanage		"AutoUnmanage"

#define XmNallowShellResize	"allowShellResize"

#define XmNdialogTitle		"dialogTitle"
#define XmCDialogTitle		"DialogTitle"

#define XmNnoResize		"noResize"
#define XmCNoResize		"NoResize"

#define XmNdialogStyle		"dialogStyle"
#define XmCDialogStyle		"DialogStyle"
#define XmRDialogStyle		"DialogStyle"

/*  dialog style defines  */
#define XmDIALOG_MODELESS		0
#define XmDIALOG_PRIMARY_APPLICATION_MODAL 1
#define XmDIALOG_FULL_APPLICATION_MODAL	2
#define XmDIALOG_SYSTEM_MODAL		3

/* The following is for compatibility only. Its use is deprecated.
 */
#define XmDIALOG_APPLICATION_MODAL	XmDIALOG_PRIMARY_APPLICATION_MODAL

/************************************************************************
 * XmSelectionBox, XmFileSelectionBox and XmCommand - misc. stuff       *
 ***********************************************************************/

#define XmNmustMatch		   "mustMatch"
#define XmCMustMatch		   "MustMatch"
#define XmNnoMatchString	   "noMatchString"
#define XmCNoMatchString	   "NoMatchString"
#define XmNdirectory		   "directory"
#define XmCDirectory		   "Directory"
#define XmNpattern		   "pattern"
#define XmCPattern		   "Pattern"
#define XmNdirSpec                 "dirSpec"
#define XmCDirSpec                 "DirSpec"
#define XmNdirMask                 "dirMask"
#define XmCDirMask                 "DirMask"
#define XmNfileTypeMask            "fileTypeMask"
#define XmCFileTypeMask            "FileTypeMask"
#define XmRFileTypeMask            "FileTypeMask"
#define XmNdirectoryValid          "directoryValid"
#define XmCDirectoryValid          "DirectoryValid"
#define XmNdirListItems		   "dirListItems"
#define XmCDirListItems		   "DirListItems"
#define XmNdirListItemCount	   "dirListItemCount"
#define XmCDirListItemCount	   "DirListItemCount"
#define XmNdirListLabelString      "dirListLabelString"
#define XmCDirListLabelString      "DirListLabelString"
#define XmNfileListItems	   "fileListItems"
#define XmCFileListItems	   "FileListItems"
#define XmNfileListItemCount	   "fileListItemCount"
#define XmCFileListItemCount	   "FileListItemCount"
#define XmNfileListLabelString     "fileListLabelString"
#define XmCFileListLabelString     "FileListLabelString"
#define XmNqualifySearchDataProc   "qualifySearchDataProc"
#define XmCQualifySearchDataProc   "QualifySearchDataProc"
#define XmNdirSearchProc           "dirSearchProc"
#define XmCDirSearchProc           "DirSearchProc"
#define XmNfileSearchProc          "fileSearchProc"
#define XmCFileSearchProc          "FileSearchProc"
#define XmNlistItems		   "listItems"
#define XmNlistItemCount	   "listItemCount"
#define XmNlistVisibleItemCount	   "listVisibleItemCount"
#define XmNhistoryItems		   "historyItems"
#define XmNhistoryItemCount	   "historyItemCount"
#define XmNhistoryVisibleItemCount "historyVisibleItemCount"
#define XmNhistoryMaxItems         "historyMaxItems"
#define XmCMaxItems                "MaxItems"

#define XmNtextAccelerators	"textAccelerators"
#define XmCTextValue		"TextValue"          /* used in text widget */
#define XmNtextValue		"textValue"          /* used in text widget */
#define XmCTextString	        "TextString"
#define XmNtextString	        "textString"
#define XmNtextColumns		"textColumns"
#define XmNcommand		"command"

/* Defines for file type mask:
*/
#define XmFILE_DIRECTORY (1 << 0)
#define XmFILE_REGULAR   (1 << 1)
#define XmFILE_ANY_TYPE  (XmFILE_DIRECTORY | XmFILE_REGULAR)

/* Defines for selection dialog type:
*/
#define XmDIALOG_WORK_AREA      0
#define XmDIALOG_PROMPT         1
#define XmDIALOG_SELECTION      2
#define XmDIALOG_COMMAND        3
#define XmDIALOG_FILE_SELECTION 4


/************************************************************************
 *  XmMessageBox           stuff not common to other dialogs            *
 ***********************************************************************/

#define XmNdefaultButtonType     "defaultButtonType"
#define XmCDefaultButtonType     "DefaultButtonType"
#define XmRDefaultButtonType     "DefaultButtonType"

#define XmNminimizeButtons       "minimizeButtons"
#define XmCMinimizeButtons       "MinimizeButtons"

#define XmNmessageString         "messageString"
#define XmNmessageAlignment      "messageAlignment"

#define XmNsymbolPixmap          "symbolPixmap"

#define XmNdialogType            "dialogType"
#define XmCDialogType            "DialogType"
#define XmRDialogType            "DialogType"

/* defines for dialog type */
#define XmDIALOG_ERROR        1
#define XmDIALOG_INFORMATION  2
#define XmDIALOG_MESSAGE      3
#define XmDIALOG_QUESTION     4
#define XmDIALOG_WARNING      5
#define XmDIALOG_WORKING      6


/************************************************************************
 *	Resource names used by XmScale
 ************************************************************************/

#define XmNscaleWidth		"scaleWidth"
#define XmCScaleWidth		"ScaleWidth"

#define XmNscaleHeight		"scaleHeight"
#define XmCScaleHeight		"ScaleHeight"

#define XmNdecimalPoints	"decimalPoints"
#define XmCDecimalPoints	"DecimalPoints"

#define XmNshowValue		"showValue"
#define XmCShowValue		"ShowValue"

#define XmNtitleString		"titleString"
#define XmCTitleString		"TitleString"

/*  Traversal direction defines  */

#define XmTRAVERSE_CURRENT            0
#define XmTRAVERSE_NEXT               1
#define XmTRAVERSE_PREV               2
#define XmTRAVERSE_HOME               3
#define XmTRAVERSE_NEXT_TAB_GROUP     4
#define XmTRAVERSE_PREV_TAB_GROUP     5
#define XmTRAVERSE_UP                 6
#define XmTRAVERSE_DOWN               7
#define XmTRAVERSE_LEFT               8
#define XmTRAVERSE_RIGHT              9


/**********************************************************************
 *
 *  Color generation function.
 *
 **********************************************************************/

#ifdef _NO_PROTO
typedef void (*XmColorProc) ();
void XmGetColors();
XmColorProc  XmSetColorCalculation();
XmColorProc  XmGetColorCalculation();
#else /* _NO_PROTO */

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

typedef void (*XmColorProc) (
 	XColor *bg_color,
 	XColor *fg_color,
 	XColor *sel_color,
 	XColor *ts_color,
 	XColor *bs_color
     );
XmColorProc XmSetColorCalculation(XmColorProc proc);
XmColorProc XmGetColorCalculation(void);
void XmGetColors (Screen * screen, Colormap color_map, Pixel background,
	Pixel *foreground_ret, Pixel *top_shadow_ret,
	Pixel *bottom_shadow_ret, Pixel *select_ret);
#endif /* _NO_PROTO */

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

/**********************************************************************
 *
 *  extern for the string to unit type converter.
 *
 **********************************************************************/

#ifdef _NO_PROTO
extern void XmCvtStringToUnitType();
extern void XmSetFontUnit();
extern void XmSetFontUnits();
#else /* _NO_PROTO */

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif
extern void XmCvtStringToUnitType(XrmValuePtr args, Cardinal * num_args,
				  XrmValue * from_val, XrmValue * to_val);

extern void XmSetFontUnit (Display *display, int value);
extern void XmSetFontUnits(Display *display, int hvalue, int vvalue);
#endif /* _NO_PROTO */

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

/****************
 *
 *  Public functions with prototypes defined in XmString.c
 *
 ****************/

#ifndef _ARGUMENTS
#ifdef _NO_PROTO
#define _ARGUMENTS(arglist) ()
#else
#define _ARGUMENTS(arglist) arglist
#endif
#endif

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#if NeedWidePrototypes

extern XmString XmStringDirectionCreate  _ARGUMENTS((int direction));
extern XmString XmStringSegmentCreate  _ARGUMENTS(( char *text , XmStringCharSet charset , int direction , int separator ));
extern void XmStringDraw  _ARGUMENTS(( Display *d , Window w , XmFontList fontlist , XmString string , GC gc , int x , int y , int width , unsigned int align , unsigned int lay_dir ,XRectangle *clip ));
extern void XmStringDrawImage  _ARGUMENTS(( Display *d , Window w , XmFontList fontlist , XmString string , GC gc ,int x , int y , int width , unsigned int align , unsigned int lay_dir , XRectangle *clip ));
extern void XmStringDrawUnderline  _ARGUMENTS(( Display *d , Window w , XmFontList fntlst , XmString str , GC gc , int x , int y , int width , unsigned int align , unsigned int lay_dir , XRectangle *clip, XmString under ));
#else
extern XmString XmStringDirectionCreate  _ARGUMENTS((XmStringDirection direction ));
extern XmString XmStringSegmentCreate  _ARGUMENTS(( char *text , XmStringCharSet charset ,XmStringDirection direction , Boolean separator ));
extern void XmStringDraw  _ARGUMENTS(( Display *d , Window w , XmFontList fontlist , XmString string , GC gc ,Position x , Position y , Dimension width , unsigned char align , unsigned char lay_dir , XRectangle *clip ));
extern void XmStringDrawImage  _ARGUMENTS(( Display *d , Window w , XmFontList fontlist , XmString string , GC gc ,Position x , Position y , Dimension width , unsigned char align , unsigned char lay_dir , XRectangle *clip ));
extern void XmStringDrawUnderline  _ARGUMENTS(( Display *d , Window w , XmFontList fntlst , XmString str , GC gc , Position x , Position y , Dimension width , unsigned char align , unsigned char lay_dir , XRectangle *clip , XmString under ));

#endif /* NeedWidePrototypes */

extern XmString XmStringCreate  _ARGUMENTS(( char *text , XmStringCharSet charset ));
extern XmString XmStringCreateSimple  _ARGUMENTS(( char *text ));
extern XmString XmStringSeparatorCreate  _ARGUMENTS(( void ));
extern XmString XmStringLtoRCreate  _ARGUMENTS(( char *text , XmStringCharSet charset ));
extern XmString XmStringCreateLtoR  _ARGUMENTS(( char *text , XmStringCharSet charset ));
extern Boolean XmStringInitContext  _ARGUMENTS(( XmStringContext *context , XmString string ));
extern void XmStringFreeContext  _ARGUMENTS(( XmStringContext context ));
extern XmStringComponentType XmStringGetNextComponent  _ARGUMENTS(( XmStringContext context , char **text , XmStringCharSet *charset , XmStringDirection *direction , XmStringComponentType *unknown_tag , unsigned short *unknown_length , unsigned char **unknown_value ));
extern XmStringComponentType XmStringPeekNextComponent  _ARGUMENTS(( XmStringContext context ));
extern Boolean XmStringGetNextSegment  _ARGUMENTS(( XmStringContext context , char **text , XmStringCharSet *charset , XmStringDirection *direction , Boolean *separator ));
extern Boolean XmStringGetLtoR  _ARGUMENTS(( XmString string , XmStringCharSet charset , char **text ));
extern XmFontList XmFontListCreate  _ARGUMENTS(( XFontStruct *font , XmStringCharSet charset ));
extern XmFontList XmStringCreateFontList  _ARGUMENTS(( XFontStruct *font , XmStringCharSet charset ));
extern void XmFontListFree  _ARGUMENTS(( XmFontList fontlist ));
extern XmFontList XmFontListAdd  _ARGUMENTS(( XmFontList old , XFontStruct *font , XmStringCharSet charset ));
extern XmFontList XmFontListCopy  _ARGUMENTS(( XmFontList fontlist ));
extern Boolean XmFontListInitFontContext  _ARGUMENTS(( XmFontContext *context , XmFontList fontlist ));
extern Boolean XmFontListGetNextFont  _ARGUMENTS(( XmFontContext context , XmStringCharSet *charset , XFontStruct **font ));
extern void XmFontListFreeFontContext  _ARGUMENTS(( XmFontContext context ));
extern XmString XmStringConcat  _ARGUMENTS(( XmString a , XmString b ));
extern XmString XmStringNConcat  _ARGUMENTS(( XmString first , XmString second , int n ));
extern XmString XmStringCopy  _ARGUMENTS(( XmString string ));
extern XmString XmStringNCopy  _ARGUMENTS(( XmString str , int n ));
extern Boolean XmStringByteCompare  _ARGUMENTS(( XmString a1 , XmString b1 ));
extern Boolean XmStringCompare  _ARGUMENTS(( XmString a , XmString b ));
extern int XmStringLength  _ARGUMENTS(( XmString string ));
extern Boolean XmStringEmpty  _ARGUMENTS(( XmString string ));
extern Boolean XmStringHasSubstring  _ARGUMENTS(( XmString string , XmString substring ));
extern void XmStringFree  _ARGUMENTS(( XmString string ));
extern Dimension XmStringBaseline  _ARGUMENTS(( XmFontList fontlist , XmString string ));
extern Dimension XmStringWidth  _ARGUMENTS(( XmFontList fontlist , XmString string ));
extern Dimension XmStringHeight  _ARGUMENTS(( XmFontList fontlist , XmString string ));
extern void XmStringExtent  _ARGUMENTS(( XmFontList fontlist , XmString string , Dimension *width , Dimension *height ));
extern int XmStringLineCount  _ARGUMENTS(( XmString string ));

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

/***********************************************************************
 *
 * SimpleMenu declarations and definitions.
 *
 ***********************************************************************/
typedef unsigned char XmButtonType;
typedef XmButtonType * XmButtonTypeTable;
typedef KeySym * XmKeySymTable;
typedef XmStringCharSet * XmStringCharSetTable;

#define XmPUSHBUTTON 1
#define XmTOGGLEBUTTON 2
#define XmCHECKBUTTON 2
#define XmRADIOBUTTON 3
#define XmCASCADEBUTTON 4
#define XmSEPARATOR 5
#define XmDOUBLE_SEPARATOR 6
#define XmTITLE 7

#define XmVaPUSHBUTTON                "pushButton"
#define XmVaTOGGLEBUTTON        "checkButton"
#define XmVaCHECKBUTTON         "checkButton"
#define XmVaRADIOBUTTON         "radioButton"
#define XmVaCASCADEBUTTON     "cascadeButton"
#define XmVaSEPARATOR         "separator"
#define XmVaSINGLE_SEPARATOR  "singleSeparator"
#define XmVaDOUBLE_SEPARATOR  "doubleSeparator"
#define XmVaTITLE             "title"

#ifdef _NO_PROTO
Widget XmCreateSimpleMenuBar();
Widget XmCreateSimplePopupMenu();
Widget XmCreateSimplePulldownMenu();
Widget XmCreateSimpleOptionMenu();
Widget XmCreateSimpleRadioBox();
Widget XmCreateSimpleCheckBox();
#else /* _NO_PROTO */

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

Widget XmCreateSimpleMenuBar (Widget parent, String name, ArgList args, Cardinal arg_count);
Widget XmCreateSimplePopupMenu (Widget parent, String name, ArgList args, Cardinal arg_count);
Widget XmCreateSimplePulldownMenu (Widget parent, String name, ArgList args, Cardinal arg_count);
Widget XmCreateSimpleOptionMenu (Widget parent, String name, ArgList args, Cardinal arg_count);
Widget XmCreateSimpleRadioBox (Widget parent, String name, ArgList args, Cardinal arg_count);
Widget XmCreateSimpleCheckBox (Widget parent, String name, ArgList args, Cardinal arg_count);
#endif /* _NO_PROTO */

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#ifdef _NO_PROTO
extern Widget XmVaCreateSimpleMenuBar();
extern Widget XmVaCreateSimplePopupMenu();
extern Widget XmVaCreateSimplePulldownMenu();
extern Widget XmVaCreateSimpleOptionMenu();
extern Widget XmVaCreateSimpleRadioBox();
extern Widget XmVaCreateSimpleCheckBox();
#else /* _NO_PROTO */

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

extern Widget XmVaCreateSimpleMenuBar (Widget parent, String name, ...);
extern Widget XmVaCreateSimplePopupMenu (Widget parent, String name,
                                  XtCallbackProc callback, ...);
extern Widget XmVaCreateSimplePulldownMenu (Widget parent, String name,
                                     int post_from_button,
                                     XtCallbackProc callback, ...);
extern Widget XmVaCreateSimpleOptionMenu (Widget parent, String name,
                                   XmString option_label,
                                   KeySym option_mnemonic,
                                   int button_set,
                                   XtCallbackProc callback, ...);
extern Widget XmVaCreateSimpleRadioBox (Widget parent, String name,
                                 int button_set, XtCallbackProc callback, ...);
extern Widget XmVaCreateSimpleCheckBox (Widget parent, String name,
                                 XtCallbackProc callback, ...);
#endif /* _NO_PROTO */

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

/***********************************************************************
 *
 * 	Misc Declarations
 * 
 ***********************************************************************/

#ifdef _NO_PROTO
typedef XtPointer	(*XmResourceBaseProc)();
#else /* _NO_PROTO */
#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif
typedef XtPointer	(*XmResourceBaseProc)(
 	Widget  widget,
 	XtPointer client_data
     );
#if defined(__cplusplus) || defined(c_plusplus)
}
#endif
#endif /* else _NO_PROTO */

typedef struct _XmSecondaryResourceDataRec{
    XmResourceBaseProc	base_proc;
    XtPointer		client_data;
    String		name;
    String		res_class;
    XtResourceList	resources;
    Cardinal		num_resources;
}XmSecondaryResourceDataRec, *XmSecondaryResourceData;

#ifdef _NO_PROTO
extern Cardinal XmGetSecondaryResourceData ();
extern Widget XmTrackingLocate ();
extern int XmConvertUnits ();
extern int XmCvtFromHorizontalPixels ();
extern int XmCvtFromVerticalPixels ();
extern int XmCvtToHorizontalPixels ();
extern int XmCvtToVerticalPixels ();
#else /* _NO_PROTO */

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

extern Cardinal XmGetSecondaryResourceData (WidgetClass wclass,
                                    XmSecondaryResourceData **secondaryDataRtn);
extern Widget XmTrackingLocate (Widget widget, Cursor cursor, 
#if NeedWidePrototypes
    int confineTo
#else
    Boolean confineTo
#endif 
);
extern int XmConvertUnits (Widget widget, int dimension, int from_type, 
                            int from_val, int to_type);
extern int XmCvtFromHorizontalPixels (Screen * screen, int from_val, int to_type);
extern int XmCvtFromVerticalPixels (Screen * screen, int from_val, int to_type);
extern int XmCvtToHorizontalPixels (Screen * screen, int from_val, int from_type);
extern int XmCvtToVerticalPixels (Screen * screen, int from_val, int from_type);
#endif /* _NO_PROTO */

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif


#ifdef _NO_PROTO
extern XmString XmCvtCTToXmString();
extern char * XmCvtXmStringToCT();
extern Boolean XmCvtTextToXmString();
extern Boolean XmCvtXmStringToText();
#else /* _NO_PROTO */

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

extern XmString XmCvtCTToXmString( char *text );
extern char * XmCvtXmStringToCT( XmString string );
extern Boolean XmCvtTextToXmString  ( Display *display , XrmValuePtr args , Cardinal *num_args , XrmValue *from_val , XrmValue *to_val , XtPointer *converter_data );
extern Boolean XmCvtXmStringToText  ( Display *display , XrmValuePtr args , Cardinal *num_args , XrmValue *from_val , XrmValue *to_val , XtPointer *converter_data );

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif /* _NO_PROTO */

typedef unsigned char   XmNavigationType;

#ifdef _NO_PROTO
extern void XmAddTabGroup ();
extern void XmRemoveTabGroup ();
extern Boolean XmProcessTraversal ();
#else /* _NO_PROTO */

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

extern void XmAddTabGroup (Widget tabGroup);
extern void XmRemoveTabGroup (Widget w);
extern Boolean XmProcessTraversal (Widget w, int dir);
#endif /* _NO_PROTO */

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#ifdef _NO_PROTO
extern Boolean XmDestroyPixmap ();
extern Boolean XmUninstallImage ();
extern Boolean XmInstallImage ();
extern Pixmap XmGetPixmap ();
#else /* _NO_PROTO */

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

extern Boolean XmUninstallImage (XImage *image);
extern Boolean XmDestroyPixmap (Screen *screen, Pixmap pixmap);
extern Boolean XmInstallImage (XImage *image, char *image_name);
extern Pixmap XmGetPixmap (Screen *screen, char *image_name, Pixel foreground, Pixel background);
#endif /* _NO_PROTO */

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#ifdef _NO_PROTO
extern Cursor XmGetMenuCursor ();
extern void XmSetMenuCursor ();     
extern Widget XmGetDestination ();
#else /* _NO_PROTO */

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

extern Cursor XmGetMenuCursor (Display *display);
extern void XmSetMenuCursor (Display *display, Cursor cursorId);     
extern Widget XmGetDestination (Display *display);
#endif /* _NO_PROTO */

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

typedef long XmOffset;
typedef XmOffset *XmOffsetPtr;

#ifdef _NO_PROTO
extern void XmUpdateDisplay ();
extern void XmResolvePartOffsets ();
extern void XmResolveAllPartOffsets ();
#else /* _NO_PROTO */

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

extern void XmUpdateDisplay (Widget w);
extern void XmResolvePartOffsets (WidgetClass wclass, XmOffsetPtr *offset);
extern void XmResolveAllPartOffsets (WidgetClass wclass, XmOffsetPtr *offset,
					XmOffsetPtr *constraint_offset);
#endif /* _NO_PROTO */

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

/***********************************************************************

  The following workarounds have been suggested by licensees and have
  not been thoroughly tested. 

***********************************************************************/

#ifdef USE_NO_GETCWD
char *getcwd(buf,size)
char *buf;
int size;
{
extern char *malloc();
if (buf==(char*) NULL)
        buf=(char *) malloc (size);
return (char *)getwd(buf);
}
#endif

#ifdef NO_VFORK
#define vfork fork
#endif



#endif /* _Xm_h */
 /* DON'T ADD STUFF AFTER THIS #endif */
