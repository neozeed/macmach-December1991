#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)TextF.c	3.37.1.8 91/03/21";
#endif /* lint */
#endif /* REV_INFO */
/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
*  (c) Copyright 1989, 1990, HEWLETT-PACKARD COMPANY
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
* X Window System is a trademark of the Massachusetts Institute of Technology
*
*******************************************************************************
******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <Xm/TextFP.h>
#include <Xm/CutPaste.h>
#include <X11/Xatom.h>
#include <X11/ShellP.h>
#include <X11/VendorP.h>
#include <X11/keysym.h>
#include <X11/AtomMgr.h>

#define MAXINT 2147483647

#define TEXT_MAX_INSERT_SIZE 32    /* Size of buffer for XLookupString. */




static XmTextScanType sarray[] = {
    XmSELECT_POSITION, XmSELECT_WORD, XmSELECT_LINE
};

static int sarraysize = XtNumber(sarray);

extern caddr_t _CompileActionTable();   /* From TMState.c. */

/* background visuals for dynamic defaulting */
extern _XmSelectColorDefault();

typedef struct {
    XmTextPosition position;
    int length;
    Time time;
} PrimSelect;

typedef struct {
    Boolean has_destination;
    XmTextPosition position;
    int replace_length;
    Boolean quick_key;
} TextFDestDataRec, *TextFDestData;

typedef struct {
    XmTextFieldWidget tf;
} TextFGCDataRec, *TextFGCData;

static XContext _XmTextFDestContext = NULL;
static XContext _XmTextFGCContext = NULL;


/* internal functions */
static void SetSel2();
static void LoseSel2();
static void MakeCopy();
static void XmTextFieldSetClipRect();
static void SetSelection();
static void StartSelection();
static Boolean SetDestination();
static void RedisplayText();
static XmTextPosition GetPosFromX();

/* Action Procedures */
static void InsertChar();
static void DeletePrevChar();
static void DeleteNextChar();
static void DeletePrevWord();
static void DeleteNextWord();
static void DeleteToStartOfLine();
static void DeleteToEndOfLine();
static void Activate();
static void ProcessCancel();
static void BackwardChar();
static void ForwardChar();
static void BackwardWord();
static void ForwardWord();
static void EndOfLine();
static void BeginningOfLine();
static void KeySelection();
static void StartPrimary();
static void MoveDestination();
static void ExtendPrimary();
static void ExtendEnd();
static void StartSecondary();
static void ExtendSecondary();
static void ClearSelection();
static void DeleteSelection();
static void ProcessCopy();
static void ProcessMove();
static void PageLeft();
static void PageRight();
static void CutPrimary();
static void CopyPrimary();
static void SetAnchor();
static void ToggleAddMode();
static void SelectAll();
static void DeselectAll();
static void QuickCutSet();
static void QuickCopySet();
static void DoQuickAction();
static void CutClipboard();
static void CopyClipboard();
static void PasteClipboard();
static void TraverseUp();
static void TraverseDown();
static void TraverseHome();
static void TraverseNextTabGroup();
static void TraversePrevTabGroup();
static void TextFocusIn();
static void TextFocusOut();
static void TextEnter();
static void TextLeave();

/* default translations and action recs */
static XtActionsRec text_actions[] = {
/* Text Replacing Bindings */
  {"self-insert",		(XtActionProc)InsertChar},
  {"delete-previous-character",	(XtActionProc)DeletePrevChar},
  {"delete-next-character",	(XtActionProc)DeleteNextChar},
  {"delete-previous-word",	(XtActionProc)DeletePrevWord},
  {"delete-next-word",		(XtActionProc)DeleteNextWord},
  {"delete-to-end-of-line",	(XtActionProc)DeleteToEndOfLine},
  {"delete-to-start-of-line",	(XtActionProc)DeleteToStartOfLine},
/* Miscellaneous Bindings */
  {"activate",			(XtActionProc)Activate},
  {"process-cancel",		(XtActionProc)ProcessCancel},
  {"Help",			(XtActionProc)_XmPrimitiveHelp},
/* Motion Bindings */
  {"backward-character",	(XtActionProc)BackwardChar},
  {"forward-character",		(XtActionProc)ForwardChar},
  {"backward-word",		(XtActionProc)BackwardWord},
  {"forward-word",		(XtActionProc)ForwardWord},
  {"end-of-line",		(XtActionProc)EndOfLine},
  {"beginning-of-line",		(XtActionProc)BeginningOfLine},
  {"page-left",			(XtActionProc)PageLeft},
  {"page-right",		(XtActionProc)PageRight},
/* Selection Bindings */
  {"key-select",		(XtActionProc)KeySelection},
  {"grab-focus",		(XtActionProc)StartPrimary},
  {"move-destination",		(XtActionProc)MoveDestination},
  {"extend-start",		(XtActionProc)ExtendPrimary},
  {"extend-adjust",		(XtActionProc)ExtendPrimary},
  {"extend-end",		(XtActionProc)ExtendEnd},
  {"delete-selection",		(XtActionProc)DeleteSelection},
  {"clear-selection",		(XtActionProc)ClearSelection},
  {"cut-primary",		(XtActionProc)CutPrimary},
  {"copy-primary",		(XtActionProc)CopyPrimary},
  {"set-anchor",		(XtActionProc)SetAnchor},
  {"toggle-add-mode",		(XtActionProc)ToggleAddMode},
  {"select-all",		(XtActionProc)SelectAll},
  {"deselect-all",		(XtActionProc)DeselectAll},
/* Quick Cut and Paste Bindings */
  {"secondary-start",		(XtActionProc)StartSecondary},
  {"secondary-adjust",		(XtActionProc)ExtendSecondary},
  {"copy-to",			(XtActionProc)ProcessCopy},
  {"move-to",			(XtActionProc)ProcessMove},
  {"quick-cut-set",		(XtActionProc)QuickCutSet},
  {"quick-copy-set",		(XtActionProc)QuickCopySet},
  {"do-quick-action",		(XtActionProc)DoQuickAction},
/* Clipboard Bindings */
  {"cut-clipboard",		(XtActionProc)CutClipboard},
  {"copy-clipboard",		(XtActionProc)CopyClipboard},
  {"paste-clipboard",		(XtActionProc)PasteClipboard},
/* Traversal */
  {"traverse-next",		(XtActionProc)TraverseDown},
  {"traverse-prev",		(XtActionProc)TraverseUp},
  {"traverse-home",		(XtActionProc)TraverseHome},
  {"next-tab-group",		(XtActionProc)TraverseNextTabGroup},
  {"prev-tab-group",		(XtActionProc)TraversePrevTabGroup},
/* Focus */
  {"focusIn",			(XtActionProc)TextFocusIn},
  {"focusOut",			(XtActionProc)TextFocusOut},
  {"enter",			(XtActionProc)TextEnter},
  {"leave",			(XtActionProc)TextLeave},
  {"unmap",			(XtActionProc)_XmPrimitiveUnmap},
};

static char EventBindings1[] = {
"Shift<Key>osfQuickPaste:	do-quick-action()\n\
 Meta <Key>osfQuickPaste:	quick-cut-set()\n\
 Alt <Key>osfQuickPaste:	quick-cut-set()\n\
 <Key>osfQuickPaste:		quick-copy-set()\n\
 Meta <Key>osfPrimaryPaste:	cut-primary()\n\
 Alt <Key>osfPrimaryPaste:	cut-primary()\n\
 <Key>osfPrimaryPaste:		copy-primary()\n\
 Meta <Key>osfCut:		cut-primary()\n\
 Alt <Key>osfCut:		cut-primary()\n\
 <Key>osfCut:			cut-clipboard()\n\
 <Key>osfPaste:			paste-clipboard()\n\
 Meta <Key>osfCopy:		copy-primary()\n\
 Alt <Key>osfCopy:		copy-primary()\n\
 <Key>osfCopy:			copy-clipboard()\n\
 Shift <Key>osfBeginLine:	beginning-of-line(extend)\n\
 <Key>osfBeginLine:		beginning-of-line()\n\
 Shift <Key>osfEndLine:		end-of-line(extend)\n\
 <Key>osfEndLine:		end-of-line()\n\
 Ctrl <Key>osfPageUp:		page-left()\n\
 Ctrl <Key>osfPageDown:		page-right()\n\
 <Key>osfClear:			clear-selection()\n\
 ~Meta ~Alt <Key>osfBackSpace:	delete-previous-character()\n\
 Shift Meta<Key>osfDelete:	cut-primary()\n\
 Shift Alt<Key>osfDelete:	cut-primary()\n\
 Shift ~Ctrl ~Meta ~Alt <Key>osfDelete:		cut-clipboard()\n\
 ~Shift Ctrl ~Meta ~Alt <Key>osfDelete:		delete-to-end-of-line()\n\
 ~Shift ~Ctrl ~Meta ~Alt <Key>osfDelete:	delete-next-character()\n"
};

static char EventBindings2[] = {
"Ctrl Meta<Key>osfInsert:	copy-primary()\n\
 Ctrl Alt<Key>osfInsert:	copy-primary()\n\
 Shift ~Ctrl ~Meta ~Alt <Key>osfInsert:		paste-clipboard()\n\
 ~Shift Ctrl ~Meta ~Alt <Key>osfInsert:		copy-clipboard()\n\
 ~Shift <Key>osfSelect:		set-anchor()\n\
 Shift ~Ctrl <Key>osfSelect:	key-select()\n\
 <Key>osfActivate:		activate()\n\
 <Key>osfAddMode:		toggle-add-mode()\n\
 <Key>osfHelp:			Help()\n\
 <Key>osfCancel:		process-cancel()\n\
 Shift Ctrl <Key>osfLeft:	backward-word(extend)\n\
 Ctrl <Key>osfLeft:		backward-word()\n\
 Shift <Key>osfLeft:		key-select(left)\n\
 <Key>osfLeft:			backward-character()\n\
 Shift Ctrl <Key>osfRight:	forward-word(extend)\n\
 Ctrl <Key>osfRight:		forward-word()\n\
 Shift <Key>osfRight:		key-select(right)\n\
 <Key>osfRight:			forward-character()\n\
 <Key>osfUp:			traverse-prev()\n\
 <Key>osfDown:			traverse-next()\n\
 Ctrl ~Meta ~Alt <Key>slash:	select-all()\n\
 Ctrl ~Meta ~Alt <Key>backslash:	deselect-all()\n\
 Shift ~Meta ~Alt <Key>Tab:	prev-tab-group()\n\
 ~Meta ~Alt <Key>Tab:		next-tab-group()\n\
 ~Shift ~Meta ~Alt <Key>Return:	activate()\n\
 Ctrl ~Shift ~Meta ~Alt <Key>space:	set-anchor()\n\
 Ctrl Shift ~Meta ~Alt <Key>space:	key-select()\n\
 ~Ctrl ~Meta ~Alt <Key>:	self-insert()\n"
};

static char EventBindings3[] = {
"~Ctrl  Shift ~Meta ~Alt<Btn1Down>:		extend-start()\n\
  Ctrl ~Shift ~Meta ~Alt<Btn1Down>:		move-destination()\n\
 ~Ctrl ~Shift ~Meta ~Alt<Btn1Down>:		grab-focus()\n\
 ~Ctrl ~Meta ~Alt<Btn1Motion>:			extend-adjust()\n\
 ~Ctrl ~Meta ~Alt<Btn1Up>:			extend-end()\n\
  Ctrl ~Shift ~Meta ~Alt<Btn2Down>:		secondary-start()\n\
 ~Ctrl ~Shift  Meta ~Alt<Btn2Down>:		secondary-start()\n\
 ~Ctrl ~Shift ~Meta  Alt<Btn2Down>:		secondary-start()\n\
 ~Ctrl ~Shift ~Meta ~Alt<Btn2Down>:		secondary-start()\n\
  Ctrl ~Shift ~Meta ~Alt<Btn2Motion>:		secondary-adjust()\n\
 ~Ctrl ~Shift  Meta ~Alt<Btn2Motion>:		secondary-adjust()\n\
 ~Ctrl ~Shift ~Meta  Alt<Btn2Motion>:		secondary-adjust()\n\
 ~Ctrl ~Shift ~Meta ~Alt<Btn2Motion>:		secondary-adjust()\n\
 ~Ctrl ~Shift  Meta ~Alt<Btn2Up>:		move-to()\n\
 ~Ctrl ~Shift ~Meta  Alt<Btn2Up>:		move-to()\n\
 ~Shift ~Meta ~Alt<Btn2Up>:			copy-to()\n\
 <FocusIn>:					focusIn()\n\
 <FocusOut>:					focusOut()\n\
 <Enter>:					enter()\n\
 <Leave>:					leave()\n\
 <Unmap>:					unmap()"
};

static XtResource resources[] =
{
    {
      XmNactivateCallback, XmCCallback, XmRCallback, sizeof(XtCallbackList),
      XtOffset (XmTextFieldWidget, text.activate_callback),
      XmRCallback, NULL
    },

    {
      XmNlosingFocusCallback, XmCCallback, XmRCallback, sizeof(XtCallbackList),
      XtOffset (XmTextFieldWidget, text.losing_focus_callback),
      XmRCallback, NULL
    },

    {
      XmNmodifyVerifyCallback, XmCCallback, XmRCallback, sizeof(XtCallbackList),
      XtOffset (XmTextFieldWidget, text.modify_verify_callback),
      XmRCallback, NULL
    },

    {
      XmNmotionVerifyCallback, XmCCallback, XmRCallback, sizeof(XtCallbackList),
      XtOffset (XmTextFieldWidget, text.motion_verify_callback),
      XmRCallback, NULL
    },

    {
      XmNgainPrimaryCallback, XmCCallback, XmRCallback, sizeof(XtCallbackList),
      XtOffset (XmTextFieldWidget, text.gain_primary_callback),
      XmRCallback, NULL
    },

    {
      XmNlosePrimaryCallback, XmCCallback, XmRCallback, sizeof(XtCallbackList),
      XtOffset (XmTextFieldWidget, text.lose_primary_callback),
      XmRCallback, NULL
    },

    {
      XmNvalueChangedCallback, XmCCallback, XmRCallback, sizeof(XtCallbackList),
      XtOffset (XmTextFieldWidget, text.value_changed_callback),
      XmRCallback, NULL
    },

    {
      XmNvalue, XmCValue, XmRString, sizeof(XmString),
      XtOffset(XmTextFieldWidget, text.value),
      XmRString, ""
    },

    {
      XmNmarginHeight, XmCMarginHeight, XmRVerticalDimension, sizeof(Dimension),
      XtOffset(XmTextFieldWidget, text.margin_height),
      XmRImmediate, (caddr_t) 5
    },

    {
      XmNmarginWidth, XmCMarginWidth, XmRHorizontalDimension, sizeof(Dimension),
      XtOffset(XmTextFieldWidget, text.margin_width),
      XmRImmediate, (caddr_t) 5
    },

    {
      XmNcursorPosition, XmCCursorPosition, XmRTextPosition,
      sizeof (XmTextPosition),
      XtOffset (XmTextFieldWidget, text.cursor_position),
      XmRImmediate, (caddr_t) 0
    },

    {
      XmNcolumns, XmCColumns, XmRShort, sizeof(short),
      XtOffset(XmTextFieldWidget, text.columns),
      XmRImmediate, (caddr_t) 20
    },

    {
      XmNmaxLength, XmCMaxLength, XmRInt, sizeof(int),
      XtOffset(XmTextFieldWidget, text.max_length),
      XmRImmediate, (caddr_t) MAXINT
    },

    {
      XmNblinkRate, XmCBlinkRate, XmRInt, sizeof(int),
      XtOffset(XmTextFieldWidget, text.blink_rate),
      XmRImmediate, (caddr_t) 500
    },

    {
      XmNfontList, XmCFontList, XmRFontList, sizeof(XmFontList),
      XtOffset(XmTextFieldWidget, text.font_list),
      XmRImmediate, NULL 
    },

    {
      XmNselectionArray, XmCSelectionArray, XmRPointer,
      sizeof(caddr_t),
      XtOffset(XmTextFieldWidget, text.selection_array),
      XmRImmediate, (caddr_t) sarray
    },

    {
      XmNselectionArrayCount, XmCSelectionArrayCount, XmRInt, sizeof(int),
      XtOffset(XmTextFieldWidget, text.selection_array_count),
      XmRInt, (caddr_t) &sarraysize
    },

    {
      XmNresizeWidth, XmCResizeWidth, XmRBoolean, sizeof(Boolean),
      XtOffset(XmTextFieldWidget, text.resize_width),
      XmRImmediate, (caddr_t) False
    },

    {
      XmNpendingDelete, XmCPendingDelete, XmRBoolean, sizeof(Boolean),
      XtOffset(XmTextFieldWidget, text.pending_delete),
      XmRImmediate, (caddr_t) True
    },

    {
      XmNeditable, XmCEditable, XmRBoolean, sizeof(Boolean),
      XtOffset(XmTextFieldWidget, text.editable),
      XmRImmediate, (caddr_t) True
    },

    {
      XmNcursorPositionVisible, XmCCursorPositionVisible, XmRBoolean,
      sizeof(Boolean),
      XtOffset(XmTextFieldWidget, text.cursor_position_visible),
      XmRImmediate, (caddr_t) True
    },

   {
     XmNverifyBell, XmCVerifyBell, XmRBoolean, sizeof(Boolean),
     XtOffset(XmTextFieldWidget, text.verify_bell),
     XmRImmediate, (caddr_t) True
   },

   {
     XmNselectThreshold, XmCSelectThreshold, XmRInt, sizeof(int),
     XtOffset(XmTextFieldWidget, text.threshold),
     XmRImmediate, (caddr_t) 5
   },

   {
     XmNnavigationType, XmCNavigationType, XmRNavigationType,
     sizeof (unsigned char),
     XtOffset (XmPrimitiveWidget, primitive.navigation_type),
     XmRImmediate, (caddr_t) XmTAB_GROUP
   },
};

/* Definition for resources that need special processing in get values */

static XmSyntheticResource syn_resources[] =
{
   {
     XmNmarginWidth,
     sizeof(Dimension),
     XtOffset(XmTextFieldWidget, text.margin_width),
     _XmFromHorizontalPixels,
     _XmToHorizontalPixels
   },

   {
     XmNmarginHeight,
     sizeof(Dimension),
     XtOffset(XmTextFieldWidget, text.margin_height),
     _XmFromVerticalPixels,
     _XmToVerticalPixels
   },

   {
     XmNvalue,
     sizeof(char *),
     XtOffset(XmTextFieldWidget, text.value),
     MakeCopy,
     NULL,
   },
};

/* Core Class Functions */

static void ClassPartInitialize();
static void Realize();
static void Initialize();
static void Destroy();
static void Resize();

static Boolean SetValues();
static void TextFieldExpose();
static XtGeometryResult QueryGeometry  ();

externaldef(xmtextfieldclassrec) XmTextFieldClassRec xmTextFieldClassRec =
{
   {
      (WidgetClass) &xmPrimitiveClassRec,	/* superclass         */
      "XmTextField",				/* class_name         */
      sizeof(XmTextFieldRec),		        /* widget_size        */
      NULL,					/* class_initialize   */
      ClassPartInitialize,			/* class_part_initiali*/
      FALSE,					/* class_inited       */
      Initialize,				/* initialize         */
      NULL,					/* initialize_hook    */
      Realize,					/* realize            */
      text_actions,				/* actions            */
      XtNumber(text_actions),			/* num_actions        */
      resources,				/* resources          */
      XtNumber(resources),			/* num_resources      */
      NULLQUARK,				/* xrm_class          */
      TRUE,					/* compress_motion    */
      XtExposeCompressMaximal,			/* compress_exposure  */
      TRUE,					/* compress_enterleave*/
      FALSE,					/* visible_interest   */
      Destroy,					/* destroy            */
      Resize,					/* resize             */
      TextFieldExpose,				/* expose             */
      SetValues,				/* set_values         */
      NULL,					/* set_values_hook    */
      XtInheritSetValuesAlmost,			/* set_values_almost  */
      NULL,					/* get_values_hook    */
      NULL,					/* accept_focus       */
      XtVersion,				/* version            */
      NULL,					/* callback_private   */
      NULL,					/* tm_table           */
      QueryGeometry,				/* query_geometry     */
      NULL,					/* display accel      */
      NULL,					/* extension          */
   },

   {  	                          		/* Xmprimitive        */
      _XmHighlightBorder,              		/* border_highlight   */
      _XmUnhighlightBorder,              	/* border_unhighlight */
      NULL,					/* translations	      */
      NULL,                     		/* arm_and_activate   */
      syn_resources,            		/* syn resources      */
      XtNumber(syn_resources),  		/* num syn_resources  */
      NULL,                     		/* extension          */
   },

   {                            		/* text class s */
      NULL,                     		/* extension              */
   }
};

externaldef(xmtextfieldwidgetclass) WidgetClass xmTextFieldWidgetClass =
					 (WidgetClass) &xmTextFieldClassRec;

static void MakeCopy(w, offset, value)
Widget w;
int offset;
XtArgVal * value;
{
    (*value) = (XtArgVal) XmTextFieldGetString (w);
}

static TextFDestData GetTextFDestData(w)
Widget w;
{
   TextFDestData dest_data;

   if (_XmTextFDestContext == NULL)
      _XmTextFDestContext = XUniqueContext();

   if (XFindContext(XtDisplay(w), (Window) XA_SECONDARY,
                    _XmTextFDestContext, (caddr_t *) &dest_data)) {
       dest_data = (TextFDestData)
		    XtCalloc((unsigned) sizeof(TextFDestDataRec), (unsigned) 1);

       XSaveContext(XtDisplay(w), (Window) XA_SECONDARY,
                    _XmTextFDestContext, dest_data);
   }

   return dest_data;
}

static TextFGCData GetTextFGCData(w)
Widget w;
{
   TextFGCData gc_data;

   if (_XmTextFGCContext == NULL)
      _XmTextFGCContext = XUniqueContext();

   if (XFindContext(XtDisplay(w), (Window) 0,
                    _XmTextFGCContext, (caddr_t*) &gc_data)) {
       gc_data = (TextFGCData)XtCalloc((unsigned)sizeof(TextFGCDataRec), 1);

       XSaveContext(XtDisplay(w), (Window) 0, _XmTextFGCContext, gc_data);
       gc_data->tf = (XmTextFieldWidget) w;
   }

   if (gc_data->tf == NULL) gc_data->tf = (XmTextFieldWidget) w;

   return gc_data;
}


/*
 * Find the highlight record corresponding to the given position.  Returns a
 * pointer to the record.  The third argument indicates whether we are probing
 * the left or right edge of a highlighting range.
 */

static HighlightRec *FindHighlight(w, position)
XmTextFieldWidget w;
XmTextPosition position;
{
    HighlightRec *l = w->text.highlight.list;
    int i;

    for (i=w->text.highlight.number - 1 ; i>=0 ; i--)
        if (position >= l[i].position) {
            l = l + i;
            break;
        }

    return(l);
}

static void InsertHighlight(w, position, mode)
XmTextFieldWidget w;
XmTextPosition position;
XmHighlightMode mode;
{
    HighlightRec *l1;
    HighlightRec *l = w->text.highlight.list;
    int i, j;

    l1 = FindHighlight(w, position);
    if (l1->position == position)
        l1->mode = mode;
    else {
        i = (l1 - l) + 1;
        w->text.highlight.number++;
        if (w->text.highlight.number > w->text.highlight.maximum) {
            w->text.highlight.maximum = w->text.highlight.number;
            l = w->text.highlight.list = (HighlightRec *) XtRealloc((char *) l,
                 (unsigned)(w->text.highlight.maximum * sizeof(HighlightRec)));
        }
        for (j=w->text.highlight.number-1 ; j>i ; j--)
            l[j] = l[j-1];
        l[i].position = position;
        l[i].mode = mode;
    }
}


static void TextFieldSetHighlight(tf, left, right, mode)
XmTextFieldWidget tf;
XmTextPosition left, right;
XmHighlightMode mode;
{
    HighlightRec *l;
    XmHighlightMode endmode;
    int i, j;

    if (left >= right || right <= 0) return;

    endmode = FindHighlight(tf, right)->mode;
    InsertHighlight(tf, left, mode);
    InsertHighlight(tf, right, endmode);
    l = tf->text.highlight.list;
    i = 1;
    while (i < tf->text.highlight.number) {
        if (l[i].position >= left && l[i].position < right)
            l[i].mode = mode;
        if (l[i].mode == l[i-1].mode) {
            tf->text.highlight.number--;
            for (j=i ; j<tf->text.highlight.number ; j++)
                l[j] = l[j+1];
        } else i++;
    }
}


/*
 * Get x and y based on position.
 */

/*--------------------------------------------------------------------------+*/
static Boolean GetXYFromPos(tf, position, x, y)
/*--------------------------------------------------------------------------+*/
XmTextFieldWidget tf;
XmTextPosition position;
Position *x, *y;
{
   /* initialize the x and y positions to zero */
    *x = 0;
    *y = 0;

    if (position > tf->text.string_length) return False;

    *x += XTextWidth(TextF_Font(tf), TextF_Value(tf), (int)position);

    *x += tf->text.h_offset - 1;
    *y += tf->primitive.highlight_thickness + tf->primitive.shadow_thickness +
          TextF_MarginHeight(tf) + TextF_Font(tf)->ascent;

    return True;
}


static Boolean CurrentCursorState(tf)
XmTextFieldWidget tf;
{
    if (!tf->text.cursor_on) return False;
    if (!tf->text.has_focus || tf->text.blink_on || !XtIsSensitive(tf))
        return True;
    return False;
}

/*
 * Draws the destination cursor.
 */

/*--------------------------------------------------------------------------+*/
static void DrawDestination(tf)
/*--------------------------------------------------------------------------+*/
XmTextFieldWidget tf;
{
    Position x, y;
    Position insertx, inserty;

    if (!XtIsRealized((Widget) tf) || !XtSensitive((Widget) tf) ||
        tf->text.dest_on ||  !tf->text.dest_visible ||
        !TextF_CursorPositionVisible(tf))
       return;

    if (!tf->text.has_rect) XmTextFieldSetClipRect(tf);

    if (GetXYFromPos(tf, tf->text.dest_position, &insertx, &inserty)) {

      /* offset half of the caret width minus 1 */
       x = insertx - ((CARET_WIDTH >> 1) - 1);

      /*
       * offset the font descent minus the cursor height plus 2 i.e.
       * the top of the cursor is 2 pixels below the base line
       */
       y = inserty + ((TextF_Font(tf)->descent - CARET_HEIGHT) + 2);

       if (x < 0 || y < 0) return;

       XCopyArea(XtDisplay(tf), tf->text.dest_cursor,
                 XtWindow(tf), tf->text.image_gc,
                 0, 0, CARET_WIDTH, CARET_HEIGHT, x, y);
    }

    tf->text.dest_on = True;
}

/*
 * Clear the destination cursor.
 */

/*--------------------------------------------------------------------------+*/
static void ClearDestination(tf, ignore_sens)
/*--------------------------------------------------------------------------+*/
XmTextFieldWidget tf;
Boolean ignore_sens;
{
    Position x, y;
    Position insertx, inserty;

    if (!XtIsRealized((Widget) tf) || 
        (!ignore_sens && !XtSensitive((Widget) tf)) ||
        !tf->text.dest_on ||  !tf->text.dest_visible ||
        !TextF_CursorPositionVisible(tf))
       return;

    if (!tf->text.has_rect) XmTextFieldSetClipRect(tf);

    if (GetXYFromPos(tf, tf->text.dest_position, &insertx, &inserty)) {

      /* offset half of the caret width minus 1 */
       x = insertx - ((CARET_WIDTH >> 1) - 1);
   
      /*
       * offset the font descent minus the cursor height plus 2 i.e.
       * the top of the cursor is 2 pixels below the base line
       */
       y = inserty + ((TextF_Font(tf)->descent - CARET_HEIGHT) + 2);
   
       if (x < 0 || y < 0) return;

       XCopyArea(XtDisplay(tf), tf->text.dest_cursor,
                 XtWindow(tf), tf->text.image_gc,
                 0, 0, CARET_WIDTH, CARET_HEIGHT, x, y);
   
    }

    tf->text.dest_on = False;
}


#ifdef _NO_PROTO
/*--------------------------------------------------------------------------+*/
void _XmTextFieldDestinationVisible(w, turn_on)
/*--------------------------------------------------------------------------+*/
Widget w;
Boolean turn_on;
#else /* _NO_PROTO */
void _XmTextFieldDestinationVisible(Widget w, Boolean turn_on)
#endif /* _NO_PROTO */
{
   XmTextFieldWidget tf = (XmTextFieldWidget) w;

   if (turn_on) {
      if (tf->text.dest_visible) return;
      tf->text.dest_visible = True;
      if (tf->text.has_destination &&
          (tf->text.dest_position != TextF_CursorPosition(tf) ||
           !tf->text.has_focus))
         DrawDestination(tf);
   } else {
      if (!tf->text.dest_visible) return;
      if (tf->text.has_destination &&
          (tf->text.dest_position != TextF_CursorPosition(tf) ||
          !tf->text.has_focus))
         ClearDestination(tf, False);
      tf->text.dest_visible = False;
   }
}


/*
 * Paint insert cursor
 */

/*--------------------------------------------------------------------------+*/
static void PaintCursor(tf)
/*--------------------------------------------------------------------------+*/
XmTextFieldWidget tf;
{
    Pixmap pixmap, cursor;
    Position x, y;

    if (!XtIsRealized(tf) || !TextF_CursorPositionVisible(tf)) return;

    (void) GetXYFromPos(tf, TextF_CursorPosition(tf), &x, &y);

    x -=(tf->text.cursor_width >> 1) - 1; 
    y = (y + (Position) TextF_Font(tf)->descent) -
	 (Position) tf->text.cursor_height;

    if (tf->text.add_mode)
       cursor = tf->text.add_mode_cursor;
    else
       cursor = tf->text.cursor;

    if (!tf->text.has_focus || !XtIsSensitive(tf)) {
        pixmap = tf->text.putback;
    } else {
        pixmap = (tf->text.cursor_on && tf->text.blink_on) ?
                 cursor : tf->text.putback;
    }

    if (tf->text.putback != NULL && pixmap != tf->text.putback)
        XCopyArea(XtDisplay(tf), tf->text.putback, XtWindow(tf),
		  tf->text.image_gc, 0, 0, tf->text.cursor_width,
		  tf->text.cursor_height, x, y);
    if (pixmap != NULL)
        XCopyArea(XtDisplay(tf), pixmap, XtWindow(tf), tf->text.image_gc,
                  0, 0, tf->text.cursor_width, tf->text.cursor_height, x, y);

    if (tf->text.putback == pixmap)
        tf->text.putback = NULL;
    else
        tf->text.putback = pixmap;

}

/*--------------------------------------------------------------------------+*/
static void DrawInsertionPoint(tf, turn_on)
/*--------------------------------------------------------------------------+*/
XmTextFieldWidget tf;
Boolean turn_on;
{

    if (tf->text.cursor_on == turn_on || !XtIsRealized((Widget) tf))
        return;
    tf->text.cursor_on = turn_on;
    PaintCursor(tf);
}

/*--------------------------------------------------------------------------+*/
static void BlinkInsertionPoint(tf)
/*--------------------------------------------------------------------------+*/
XmTextFieldWidget tf;
{
    if (tf->text.cursor_on && tf->text.blink_on == CurrentCursorState(tf)){
        if (tf->text.blink_on) tf->text.blink_on = False;
        else tf->text.blink_on = True;
        PaintCursor(tf);
    }
}



/*
 * Handle blink on and off
 */
/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void HandleTimer(closure, id)
/*--------------------------------------------------------------------------+*/
caddr_t closure;
XtIntervalId id;
{
    XmTextFieldWidget tf = (XmTextFieldWidget) closure;

    if (TextF_BlinkRate(tf) != 0)
        tf->text.timer_id =
		 XtAppAddTimeOut(XtWidgetToApplicationContext((Widget)tf),
				 (unsigned long)TextF_BlinkRate(tf),
                                 (XtTimerCallbackProc) HandleTimer,
                                 (caddr_t) closure);
    if (tf->text.has_focus && XtIsSensitive(tf)) BlinkInsertionPoint(tf);
}


/*
 * Change state of blinking insert cursor on and off
 */

/*--------------------------------------------------------------------------+*/
static void ChangeBlinkBehavior(tf, turn_on)
/*--------------------------------------------------------------------------+*/
XmTextFieldWidget tf;
Boolean turn_on;
{
    if (turn_on) {
        if (TextF_BlinkRate(tf) != 0 && tf->text.timer_id == NULL)
            tf->text.timer_id =
                XtAppAddTimeOut(XtWidgetToApplicationContext((Widget)tf),
			        (unsigned long)TextF_BlinkRate(tf),
                                (XtTimerCallbackProc) HandleTimer,
                                (caddr_t) tf);
        tf->text.blink_on = True;
    } else {
        if (tf->text.timer_id)
            XtRemoveTimeOut(tf->text.timer_id);
        tf->text.timer_id = NULL;
    }

    if (tf->text.cursor_on)
	PaintCursor(tf);
}


/*--------------------------------------------------------------------------+*/
static void GetRect(tf, rect)
/*--------------------------------------------------------------------------+*/
XmTextFieldWidget tf;
XRectangle *rect;
{
  Dimension margin_width = TextF_MarginWidth(tf) +
	                   tf->primitive.shadow_thickness +
			   tf->primitive.highlight_thickness;
  Dimension margin_height = TextF_MarginHeight(tf) +
	                   tf->primitive.shadow_thickness +
			   tf->primitive.highlight_thickness;

  if (margin_width < tf->core.width)
     rect->x = margin_width;
  else
     rect->x = tf->core.width;

  if (margin_height < tf->core.height)
     rect->y = margin_height;
  else
     rect->y = tf->core.height;

  if ((2 * margin_width) < tf->core.width)
     rect->width = (int) tf->core.width - (2 * margin_width);
  else
     rect->width = 0;

  if ((2 * margin_height) < tf->core.height)
     rect->height = (int) tf->core.height - (2 * margin_height);
  else
     rect->height = 0;
}



static void XmSetFullGC (tf, gc)
XmTextFieldWidget tf;
GC gc;
{
  XRectangle ClipRect;

 /* adjust clip rectangle to allow the cursor to paint into the margins */
  ClipRect.x = tf->primitive.shadow_thickness +
               tf->primitive.highlight_thickness;
  ClipRect.y = tf->primitive.shadow_thickness +
               tf->primitive.highlight_thickness;
  ClipRect.width = tf->core.width - (2 * (tf->primitive.shadow_thickness +
                                          tf->primitive.highlight_thickness));
  ClipRect.height = tf->core.height - (2 * (tf->primitive.shadow_thickness +
                                            tf->primitive.highlight_thickness));

  XSetClipRectangles(XtDisplay(tf), gc, 0, 0, &ClipRect, 1,
                     Unsorted);
 /*
  * Make sure the cached GC has the clipping rectangle
  * set to the current widget.
  */
  if (!tf->text.has_rect) {
     TextFGCData gc_data = GetTextFGCData((Widget)tf);
     gc_data->tf->text.has_rect = False;
     gc_data->tf = tf;
     tf->text.has_rect = True;
  }
}


static void XmSetMarginGC (tf, gc)
XmTextFieldWidget tf;
GC gc;
{
  XRectangle ClipRect;

  GetRect(tf, &ClipRect);
  XSetClipRectangles(XtDisplay(tf), gc, 0, 0, &ClipRect, 1,
                     Unsorted);
 /*
  * Make sure the cached GC has the clipping rectangle
  * set to the current widget.
  */
  if (!tf->text.has_rect) {
     TextFGCData gc_data = GetTextFGCData((Widget)tf);
     gc_data->tf->text.has_rect = False;
     gc_data->tf = tf;
     tf->text.has_rect = True;
  }
}



/*
 * Set new clipping rectangle for text field.  This is
 * done on each focus in event since the text field widgets
 * share the same GC.
 */

/*--------------------------------------------------------------------------+*/
static void XmTextFieldSetClipRect(tf)
/*--------------------------------------------------------------------------+*/
XmTextFieldWidget tf;
{

  XmSetMarginGC(tf, tf->text.gc);
  XmSetFullGC(tf, tf->text.image_gc);

}

static void XmSetNormGC (tf, gc)
XmTextFieldWidget tf;
GC gc;
{
    unsigned long valueMask = (GCForeground | GCBackground | GCFunction |
                               GCGraphicsExposures);
    XGCValues values;

    values.foreground = tf->primitive.foreground;
    values.background = tf->core.background_pixel;
    values.function = GXcopy;
    values.graphics_exposures = (Bool) TRUE;

    XChangeGC(XtDisplay(tf), gc, valueMask, &values);
}

static void XmSetInvGC (tf, gc)
XmTextFieldWidget tf;
GC gc;
{
    unsigned long valueMask = (GCForeground | GCBackground | GCFunction |
                               GCGraphicsExposures);
    XGCValues values;

    values.foreground = tf->core.background_pixel;
    values.background = tf->primitive.foreground;
    values.function = GXcopy;
    values.graphics_exposures = (Bool) TRUE;

    XChangeGC(XtDisplay(tf), gc, valueMask, &values);
}

static void XmSetXorGC (tf, gc)
XmTextFieldWidget tf;
GC gc;
{
    unsigned long valueMask = (GCForeground | GCBackground | GCFunction |
                               GCGraphicsExposures);
    XGCValues values;

    values.foreground = tf->primitive.foreground ^ tf->core.background_pixel;
    values.background = 0;
    values.function = GXxor;
    values.graphics_exposures = (Bool) FALSE;

    XChangeGC(XtDisplay(tf), gc, valueMask, &values);
}


/*
 * Redisplay the new adjustments that have been made the the text
 * field widget.
 */

/*--------------------------------------------------------------------------+*/
static void RedisplayText(tf, start, end)
/*--------------------------------------------------------------------------+*/
XmTextFieldWidget tf;
XmTextPosition start, end;
{
  HighlightRec *l = tf->text.highlight.list;
  XRectangle rect;
  int x, y, i;
  Dimension margin_width = TextF_MarginWidth(tf) +
	                   tf->primitive.shadow_thickness +
			   tf->primitive.highlight_thickness;
  Dimension margin_height = TextF_MarginHeight(tf) +
	                   tf->primitive.shadow_thickness +
			   tf->primitive.highlight_thickness;


  if (!XtIsRealized(tf)) return;

  if (tf->core.width - (2 * margin_width) <= 0)
    return;
  if (tf->core.height - (2 * margin_height) <= 0)
    return;

 /*
  * Make sure the cached GC has the clipping rectangle
  * set to the current widget.
  */
  if (!tf->text.has_rect) XmTextFieldSetClipRect(tf);

  if (tf->text.has_destination &&
      (tf->text.dest_position != TextF_CursorPosition(tf) ||
      !tf->text.has_focus))
     ClearDestination(tf, False);
  DrawInsertionPoint(tf, False);

 /* Get the current rectangle.
  */
  GetRect(tf, &rect);

  x = tf->text.h_offset;
  y = margin_height + TextF_Font(tf)->ascent;

 /* search through the highlight array and draw the text */
  for (i = 0; i + 1 < tf->text.highlight.number; i++) {

     /* make sure start is within current highlight */
      if (l[i].position <= start && start < l[i+1].position &&
          l[i].position < end) {
        /* update x position up to start position */
         x += XTextWidth (TextF_Font(tf), TextF_Value(tf) + l[i].position,
                          (int)(start - l[i].position));

         if (l[i].mode == XmHIGHLIGHT_SELECTED)
           /* Draw the selected text using an inverse gc */
            XmSetInvGC(tf, tf->text.gc);
         else
            XmSetNormGC(tf, tf->text.gc);

         if (end > l[i+1].position) {
           /* end position is outside current highlight */
            XDrawImageString (XtDisplay(tf), XtWindow(tf), tf->text.gc,
                              x, y, TextF_Value(tf) + start,
                              (int)l[i+1].position - (int)start);

            if (l[i].mode == XmHIGHLIGHT_SECONDARY_SELECTED)
               XDrawLine(XtDisplay(tf), XtWindow(tf), tf->text.gc, x, y,
                        (x + XTextWidth(TextF_Font(tf), TextF_Value(tf) + start,
                         (int)l[i+1].position - (int)start)) - 1, y);

           /* update x position up to the next highlight position */
            x += XTextWidth (TextF_Font(tf), TextF_Value(tf) + start,
                            (int)l[i+1].position - (int)start);
           /* update start position to the next highlight position */
            start = l[i+1].position;
         } else {
           /* end position is within current highlight */
            XDrawImageString (XtDisplay(tf), XtWindow(tf), tf->text.gc,
                              x, y, TextF_Value(tf) + start,
                              (int)end - (int)start);

            if (l[i].mode == XmHIGHLIGHT_SECONDARY_SELECTED)
               XDrawLine(XtDisplay(tf), XtWindow(tf), tf->text.gc, x, y,
                        (x + XTextWidth(TextF_Font(tf), TextF_Value(tf) + start,
		         (int)end - (int)start)) - 1, y);

           /* update x position up to the next highlight position */
            x += XTextWidth (TextF_Font(tf), TextF_Value(tf) + start,
                            (int)l[i+1].position - (int)start);
            start = end;
         }
      } else
         x += XTextWidth (TextF_Font(tf), TextF_Value(tf) + l[i].position,
                       (int)(l[i+1].position - l[i].position));
  }


 /* complete the drawing of the text to the end of the line */
  if (l[i].position < end) {
     if (l[i].mode == XmHIGHLIGHT_SELECTED)
       /* Draw the selected text using an inverse gc */
        XmSetInvGC(tf, tf->text.gc);
     else
        XmSetNormGC(tf, tf->text.gc);

     x += XTextWidth(TextF_Font(tf), TextF_Value(tf) + l[i].position,
		     ((int)start - (int)l[i].position));

     XDrawImageString (XtDisplay(tf), XtWindow(tf), tf->text.gc,
                       x, y, TextF_Value(tf) + start, ((int)end - (int)start));

     if (l[i].mode == XmHIGHLIGHT_SECONDARY_SELECTED)
         XDrawLine(XtDisplay(tf), XtWindow(tf), tf->text.gc, x, y,
                   (x + XTextWidth(TextF_Font(tf), TextF_Value(tf) + start,
                                   (int)end - (int)start)) - 1,
		   y);

     x += XTextWidth(TextF_Font(tf), TextF_Value(tf) + start,
		     tf->text.string_length - (int)start);
  } else
     x += XTextWidth(TextF_Font(tf), TextF_Value(tf) + l[i].position,
		  tf->text.string_length - (int)l[i].position);

  if (x < rect.x + rect.width) {
     XmSetInvGC(tf, tf->text.gc);
     XFillRectangle(XtDisplay(tf), XtWindow(tf), tf->text.gc, x, rect.y,
                    rect.x + rect.width - x, rect.height);
  }

  if (tf->text.has_destination &&
      tf->text.dest_position != TextF_CursorPosition(tf))
     DrawDestination(tf);
  DrawInsertionPoint(tf, True);
}


/*
 * Use the font along with the resources that have been set
 * to determine the height and width of the text field widget.
 */

/*--------------------------------------------------------------------------+*/
static void ComputeSize(tf, width, height)
/*--------------------------------------------------------------------------+*/
XmTextFieldWidget tf;
Dimension *width, *height;
{
    if (TextF_ResizeWidth(tf) &&
	TextF_Columns(tf) < tf->text.string_length)
       *width = XTextWidth(TextF_Font(tf),
			   TextF_Value(tf), 0) +
                2 * (TextF_MarginWidth(tf) + tf->primitive.shadow_thickness +
                tf->primitive.highlight_thickness);
    else
       *width = TextF_Columns(tf) * tf->text.average_char_width +
             2 * (TextF_MarginWidth(tf) + tf->primitive.shadow_thickness +
                  tf->primitive.highlight_thickness);

    if (height != NULL)
       *height = TextF_Font(tf)->descent + TextF_Font(tf)->ascent +
	         2 * (TextF_MarginHeight(tf) + tf->primitive.shadow_thickness +
	         tf->primitive.highlight_thickness);

}


/*
 * TryResize - Attempts to resize the width of the text field widget.
 * If the attempt fails or is ineffective, return GeometryNo.
 */

/*--------------------------------------------------------------------------+*/
static XtGeometryResult TryResize(tf, width, height)
/*--------------------------------------------------------------------------+*/
XmTextFieldWidget tf;
Dimension width, height;
{
    Dimension reswidth, resheight;
    Dimension origwidth = tf->core.width;
    XtGeometryResult result;

    result = XtMakeResizeRequest((Widget)tf, width, height,
                                 &reswidth, &resheight);

    if (result == XtGeometryAlmost) {
        result = XtMakeResizeRequest((Widget)tf, reswidth, resheight,
				     &reswidth, &resheight);

        if (reswidth == origwidth)
           result = XtGeometryNo;
        return result;
    }

   /*
    * Caution: Some geometry managers return XtGeometryYes
    *	        and don't change the widget's size.
    */
    if (tf->core.width != width && tf->core.height != height)
        result = XtGeometryNo;

    return result;
}


/*
 * Function AdjustText
 *
 * AdjustText ensures that the character at the given position is entirely
 * visible in the Text Field widget.  If the character is not already entirely
 * visible, AdjustText changes the Widget's h_offsetring appropriately.  If
 * the text must be redrawn, AdjustText calls RedisplayText.
 *
 */

/*--------------------------------------------------------------------------+*/
static Boolean AdjustText(tf, position, flag)
/*--------------------------------------------------------------------------+*/
XmTextFieldWidget tf;
XmTextPosition position;
Boolean flag; 		/* flag to redisplay text only when necessary. */
{
  int left_edge, diff;
  Dimension margin_width = TextF_MarginWidth(tf) +
			      tf->primitive.shadow_thickness +
                              tf->primitive.highlight_thickness;
  Dimension margin_height = TextF_MarginHeight(tf) +
                              tf->primitive.shadow_thickness +
                              tf->primitive.highlight_thickness;

  left_edge = XTextWidth(TextF_Font(tf), TextF_Value(tf),
                         (int)position) + tf->text.h_offset;

  if ((diff = left_edge - margin_width) < 0) {
    /*
     * Make sure the cached GC has the clipping rectangle
     * set to the current widget.
     */
     if (!tf->text.has_rect) XmTextFieldSetClipRect(tf);
     if (tf->text.has_destination &&
         (tf->text.dest_position != TextF_CursorPosition(tf) ||
         !tf->text.has_focus))
        ClearDestination(tf, False);
    /* We need to scroll the string to the right. */
     DrawInsertionPoint(tf, False);
     tf->text.h_offset -= diff;
     if (!XtIsRealized(tf)) return True;
     XmSetInvGC(tf, tf->text.gc);
     XmSetFullGC(tf, tf->text.gc);
     XFillRectangle(XtDisplay(tf), XtWindow(tf), tf->text.gc,
		    tf->primitive.shadow_thickness +
                    tf->primitive.highlight_thickness,
		    tf->primitive.shadow_thickness +
                    tf->primitive.highlight_thickness,
                    TextF_MarginWidth(tf),
                    tf->core.height - (2 * (tf->primitive.shadow_thickness +
                                           tf->primitive.highlight_thickness)));
     XmSetMarginGC(tf, tf->text.gc);
     RedisplayText(tf, 0, tf->text.string_length); 
     return True;
  } else if ((diff = (left_edge -
                      (tf->core.width - margin_width))) > 0) {
           /*
            * Make sure the cached GC has the clipping rectangle
            * set to the current widget.
            */
            if (!tf->text.has_rect) XmTextFieldSetClipRect(tf);
            if (tf->text.has_destination &&
                (tf->text.dest_position != TextF_CursorPosition(tf) ||
                !tf->text.has_focus))
               ClearDestination(tf, False);
            DrawInsertionPoint(tf, False);
           /* We need to scroll the string to the left. */
            tf->text.h_offset -= diff;
            if (!XtIsRealized(tf)) return True;
            XmSetInvGC(tf, tf->text.gc);
            XmSetFullGC(tf, tf->text.gc);
            XFillRectangle(XtDisplay(tf), XtWindow(tf), tf->text.gc,
                           tf->core.width - margin_width,
		           tf->primitive.shadow_thickness +
                           tf->primitive.highlight_thickness,
                           TextF_MarginWidth(tf),
                           tf->core.height -
                           (2 * (tf->primitive.shadow_thickness +
                                 tf->primitive.highlight_thickness)));
             XmSetMarginGC(tf, tf->text.gc);
             RedisplayText(tf, 0, tf->text.string_length); 
             return True;
  }

  if (flag) RedisplayText(tf, position, tf->text.string_length); 

  return False;
}

/*
 * AdjustSize
 *
 * Adjust size will resize the text to ensure that all the text is visible.
 * It will also adjust text that is shrunk.  Shrinkage is limited to the
 * size determined by the XmNcolumns resource.
 */
/*--------------------------------------------------------------------------+*/
static void AdjustSize(tf)
/*--------------------------------------------------------------------------+*/
XmTextFieldWidget tf;
{
  XtGeometryResult result = XtGeometryYes;
  int left_edge, diff;
  Boolean redisplay = False;
  Dimension margin_width = TextF_MarginWidth(tf) +
			      tf->primitive.shadow_thickness +
                              tf->primitive.highlight_thickness;

  left_edge = XTextWidth(TextF_Font(tf), TextF_Value(tf),
                         tf->text.string_length) + margin_width;

  if ((diff = (left_edge - (tf->core.width - (margin_width + 1)))) > 0) {
	  /* Attempt to resize.  If it doesn't succeed, do scrolling.  */
           result = TryResize(tf, tf->core.width + diff, tf->core.height);
	   if (result == XtGeometryYes) {
               Resize((Widget)tf);
	       return;
           } else
              /* We need to scroll the string to the left. */
               tf->text.h_offset -= diff;
  } else {
        Dimension width;

       /* If the new size is smaller than core size, we need
	* to shrink.  Note: new size will never be less than the
	* width determined by the columns resource.
	*/
	ComputeSize(tf, &width, NULL);
        if (width < tf->core.width) {
	   result = TryResize(tf, width, tf->core.height);
           if (result == XtGeometryYes) {
              Resize((Widget)tf);
              return;
           }
        }
  }

  if (result != XtGeometryYes)
     redisplay = AdjustText(tf, TextF_CursorPosition(tf), False);

  if (!redisplay)
     RedisplayText(tf, 0, tf->text.string_length);
}


static Boolean ModifyVerify(tf, event, replace_prev, replace_next,
			    insert, insert_length)
XmTextFieldWidget tf;
XEvent *event;
XmTextPosition replace_prev, replace_next;
char *insert;
int insert_length;
{
     XmTextVerifyCallbackStruct vcb;
     XmTextBlockRec newblock;

     newblock.length = insert_length;
     if (insert_length) {
        /* Allocate space for the character string to be inserted.
         * It is up to the application that uses the callback to
	 * free the allocated space.
         */
         newblock.ptr = (char *) XtMalloc((unsigned)insert_length);
         strncpy(newblock.ptr, insert, insert_length);
     } else newblock.ptr = NULL;
	
    /* Fill in the appropriate struct s */
     vcb.reason = XmCR_MODIFYING_TEXT_VALUE;
     vcb.event = (XEvent *) event;
     vcb.doit = True;
     vcb.currInsert = TextF_CursorPosition(tf);
     vcb.newInsert = TextF_CursorPosition(tf);
     vcb.text = &newblock;
     vcb.startPos = replace_prev;
     vcb.endPos = replace_next;

     /* Call the modify verify callbacks. */
     XtCallCallbackList ((Widget) tf, TextF_ModifyVerifyCallback(tf), (Opaque)&vcb);

     /* If doit becomes False, then don't allow the change. */
     return vcb.doit;
}


/*
 * Calls the motion verify callback.  If the doit flag is true,
 * then reset the cursor_position and call AdjustText() to
 * move the text if need be.
 */
/*--------------------------------------------------------------------------+*/
static void SetCursorPosition(tf, event, position, adjust_flag)
/*--------------------------------------------------------------------------+*/
XmTextFieldWidget tf;
XEvent *event;
XmTextPosition position;
Boolean adjust_flag;
{
    XmTextVerifyCallbackStruct cb;
    Boolean flag = False;

    if (position < 0) position = 0;

    if (position > tf->text.string_length)
       position = tf->text.string_length;

    if (TextF_CursorPosition(tf) != position && event != NULL) {
      /* Call Motion Verify Callback before Cursor Changes Positon */
       cb.reason = XmCR_MOVING_INSERT_CURSOR;
       cb.event  = event;
       cb.currInsert = TextF_CursorPosition(tf);
       cb.newInsert = position;
       cb.doit = True;
       XtCallCallbackList ((Widget) tf, TextF_MotionVerifyCallback(tf), &cb);

       if (!cb.doit) {
          if (tf->text.verify_bell) XBell(XtDisplay((Widget)tf), 0);
	  return;
       }
    }
    if (tf->text.cursor_on) DrawInsertionPoint(tf, False);

    TextF_CursorPosition(tf) = position;

    if (!tf->text.add_mode && tf->text.pending_off && tf->text.has_primary) {
       SetSelection(tf, position, position, True);
       flag = True;
    }

    if (tf->text.has_destination) {
       if (tf->text.dest_position == TextF_CursorPosition(tf))
          ClearDestination(tf, False);
       else
          DrawDestination(tf);
    }

    if (adjust_flag) (void) AdjustText(tf, position, flag);

    if (!tf->text.cursor_on) DrawInsertionPoint(tf, True);

}


/*
 * Function ReplaceText
 *
 * ReplaceText is a utility function for the text-modifying action procedures
 * below (InsertChar, DeletePrevChar, and so on).  ReplaceText does the real
 * work of editing the string, including:
 *
 *   (1) invoking the modify verify callbacks,
 *   (2) allocating more memory for the string if necessary,
 *   (3) doing the string manipulation,
 *   (4) moving the selection (the insertion point),
 *   (5) redrawing the text, and
 *   (6) invoking the value changed callbacks.
 */

/*--------------------------------------------------------------------------+*/
static Boolean ReplaceText(tf, event, replace_prev, replace_next, insert,
                        insert_length)
/*--------------------------------------------------------------------------+*/
XmTextFieldWidget tf;
XEvent *event;
XmTextPosition replace_prev, replace_next;
char *insert;
int insert_length;
{
  int replace_length, i;
  char *src, *dst;
  XmAnyCallbackStruct cb;
  int delta = 0;
  XmTextPosition old_position = replace_prev;
  XmTextPosition cursorPos;


  if (!TextF_Editable(tf)) {
     if (tf->text.verify_bell) XBell(XtDisplay((Widget)tf), 0);
     return False;
  }

  replace_length = replace_next - replace_prev;

 /* Truncate insertions that go beyond max length boundries.
  */
  if ((tf->text.string_length + insert_length - replace_length) - 
               (TextF_MaxLength(tf)) > 0) { 
      if (tf->text.verify_bell) XBell(XtDisplay((Widget)tf), 0);
      return False;
  }

 /* If there are modify verify callbacks, verify that we want to continue
  * the action.
  */
  if (TextF_ModifyVerifyCallback(tf)) {
    /* If the function ModifyVerify() returns false then don't
     * continue with the action.
     */
     if (!ModifyVerify(tf, event, replace_prev,
		       replace_next, insert, insert_length)) {
        if (tf->text.verify_bell) XBell(XtDisplay((Widget)tf), 0);
	return False;
     }
  }

 /* make sure selections are turned off prior to changeing text */
  if (tf->text.has_primary && tf->text.prim_pos_left != tf->text.prim_pos_right)
     XmTextFieldSetHighlight((Widget)tf, tf->text.prim_pos_left,
			     tf->text.prim_pos_right, XmHIGHLIGHT_NORMAL);

  if (tf->text.has_destination &&
      (tf->text.dest_position != TextF_CursorPosition(tf) ||
      !tf->text.has_focus))
     ClearDestination(tf, False);
  if (tf->text.cursor_on) DrawInsertionPoint(tf, False);

  /* Allocate more space if we need it.
   */
  if (tf->text.string_length + insert_length - replace_length >=
      tf->text.size_allocd)
    {
      tf->text.size_allocd += MAX(insert_length,
                                        (tf->text.size_allocd * 2));
      TextF_Value(tf) = XtRealloc(TextF_Value(tf), (unsigned) (sizeof(char) *
							 tf->text.size_allocd));
    }

  if (replace_length > insert_length)
    /* We need to shift the text at and after replace_next to the left. */
    for (src = TextF_Value(tf) + replace_next,
         dst = src + (insert_length - replace_length),
         i = (tf->text.string_length + 1) - replace_next;
         i > 0;
         ++src, ++dst, --i)
      *dst = *src;
  else if (replace_length < insert_length)
    /* We need to shift the text at and after replace_next to the right. */
    /* Need to add 1 to string_length to handle the NULL terminator on */
    /* the string. */
    for (src = TextF_Value(tf) + tf->text.string_length,
         dst = src + (insert_length - replace_length),
         i = (tf->text.string_length + 1) - replace_next;
         i > 0;
         --src, --dst, --i)
      *dst = *src;


 /* Update the string.
  */
  if (insert_length != 0) {
     for (src = insert,
          dst = TextF_Value(tf) + replace_prev,
          i = insert_length;
          i > 0;
          ++src, ++dst, --i)
       *dst = *src;
  }

  delta = insert_length - replace_length;

  if (tf->text.has_primary) {
     if (tf->text.prim_pos_left > replace_prev)
        tf->text.prim_pos_left += delta;
     if (tf->text.prim_pos_right > replace_prev)
        tf->text.prim_pos_right += delta;
     if (tf->text.prim_pos_left > tf->text.prim_pos_right)
	tf->text.prim_pos_right = tf->text.prim_pos_left;
  }

 /* make sure the selection are redisplay, since they were turned off earlier */
  if (tf->text.has_primary && tf->text.prim_pos_left != tf->text.prim_pos_right)
     XmTextFieldSetHighlight((Widget)tf, tf->text.prim_pos_left,
			     tf->text.prim_pos_right, XmHIGHLIGHT_SELECTED);

  tf->text.string_length += insert_length - replace_length;

  if (TextF_ResizeWidth(tf)) {
     cursorPos = replace_next + (insert_length - replace_length);
     if (event != NULL)
        (void) SetDestination((Widget)tf, cursorPos, False, event->xkey.time);
     SetCursorPosition(tf, event, cursorPos, False);
     AdjustSize(tf);
  } else {
     cursorPos = replace_next + (insert_length - replace_length);
     if (event != NULL)
        (void) SetDestination((Widget)tf, cursorPos, False, event->xkey.time);
     SetCursorPosition(tf, event, cursorPos, False);

     /* Call AdjustText twice: once to ensure the string is redrawn, and      PIR2492
      * the second to force it to be redisplayed in the right place.          PIR2492
      */                                                                   /* PIR2492 */
     if (insert_length >= replace_length) {                                /* PIR2492 */
        AdjustText(tf, old_position, True);                                /* PIR2492 */
     }                                                                     /* PIR2492 */
     AdjustText(tf, TextF_CursorPosition(tf), True);                       /* PIR2492 */
  }

  cb.reason = XmCR_VALUE_CHANGED;
  cb.event = event;
  XtCallCallbackList ((Widget) tf, TextF_ValueChangedCallback(tf), (Opaque) &cb);
  return True;
}


/*
 * Reset selection flag and selection positions and then display
 * the new settings.
 */
/*--------------------------------------------------------------------------+*/
static void DeselectSelection(w, disown, sel_time)
/*--------------------------------------------------------------------------+*/
Widget w;
Boolean disown;
Time sel_time;
{
  XmTextFieldWidget tf = (XmTextFieldWidget) w;

  if (disown)
    /*
     * Disown the primary selection (This function is a no-op if
     * this widget doesn't own the primary selection)
     */
     XtDisownSelection(w, XA_PRIMARY, sel_time);
  if (tf != NULL) {
     DrawInsertionPoint(tf, False);
     tf->text.has_primary = False;
     TextFieldSetHighlight(tf, tf->text.prim_pos_left,
		        tf->text.prim_pos_right, XmHIGHLIGHT_NORMAL);
     tf->text.prim_pos_left = tf->text.prim_pos_right =
	       tf->text.prim_anchor = TextF_CursorPosition(tf);
     XmTextFieldSetAddMode(w, False);
     DrawInsertionPoint(tf, True);
     RedisplayText(tf, 0, tf->text.string_length);
  }
}

/*
 * Finds the cursor position from the given X value.
 */
/*--------------------------------------------------------------------------+*/
static XmTextPosition GetPosFromX(tf, x)
/*--------------------------------------------------------------------------+*/
XmTextFieldWidget tf;
Position x;
{
    XmTextPosition position;
    unsigned char c;
    int temp_x = 0;
    char *ptr;
    int next_char_width;

   /* Decompose the x to equal the length of the text string */
    temp_x += tf->text.h_offset - 1;

   /* Next width is an offset allowing button presses on the left side 
    * of a character to select that character, while button presses
    * on the rigth side of the character select the  NEXT character.
    */

    if (tf->text.string_length > 0) {


          next_char_width = XTextWidth(TextF_Font(tf),TextF_Value(tf),1);
    }



    for (position = 0; temp_x + next_char_width/2 < (int) x &&
	               position < tf->text.string_length; position++){

	
       temp_x+=next_char_width;    /* 
				    * We still haven't reached the x pos.
				    * Add the width and find the next chars
				    * width. 
				    */

	/*
	 * If there is a next position, find its width.  Otherwise, use the
	 * current "next" width.
	 */

          c = (unsigned char) TextF_Value(tf)[position+1];
          if (TextF_Font(tf)->per_char && (c >= TextF_Font(tf)->min_char_or_byte2 && 
			         c <= TextF_Font(tf)->max_char_or_byte2)){
             next_char_width = 
		            TextF_Font(tf)->per_char[c - TextF_Font(tf)->min_char_or_byte2].width;
          } else
	      next_char_width = TextF_Font(tf)->min_bounds.width;


    } /* for */

    return position;
}


/* Replaces the destination selection, with the secondary selection */

/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void InsertDestSelection(w, closure, seltype, type, value,
				length, format)
/*--------------------------------------------------------------------------+*/
Widget w;
Opaque closure;
Atom *seltype;
Atom *type;
char *value;
int *length;
int *format;
{
    Boolean *select_done = (Boolean *)closure;
    XmTextFieldWidget tf = (XmTextFieldWidget) w;
    XmTextPosition cursorPos;
    XmTextPosition left = 0;
    XmTextPosition right = 0;
    XmTextPosition replace_left, replace_right;
    XmTextPosition old_cursorPos;
    Boolean dest_disjoint = True;

    if (!value) {
       select_done[0] = True;
       return;
    }

  /* Don't do replace if there is not text to add */
    if (*value == NULL || *length == 0){
       XtFree(value);
       select_done[0] = True;
       return;
    }

    replace_left = replace_right = tf->text.dest_position;

    if (XmTextFieldGetSelectionPosition (w, &left, &right) && left != right) {
       if (replace_left >= left && replace_right <= right &&
           tf->text.pending_delete) {
          replace_left = left;
          replace_right = right;
          dest_disjoint = False;
       }
    }

    old_cursorPos = TextF_CursorPosition(tf);

    if (!ReplaceText(tf, NULL, replace_left, replace_right, value, *length)) {
       select_done[1] = False;
    } else {
       SetCursorPosition(tf, NULL, old_cursorPos, False);
       select_done[1] = True;
       (void) SetDestination((Widget)tf, replace_left + *length,
                    	     False, CurrentTime);

        if (left != right) {
           if (!dest_disjoint) {
              tf->text.prim_anchor = tf->text.dest_position;
              StartSelection(tf, tf->text.prim_anchor,
                           tf->text.prim_anchor, CurrentTime);
           } else {
              if (!tf->text.add_mode) {
                 tf->text.prim_anchor = tf->text.dest_position;
                 StartSelection(tf, tf->text.prim_anchor,
                                tf->text.prim_anchor, CurrentTime);
              }
           }
        } else {
           tf->text.prim_anchor = tf->text.dest_position;
        }
    }

    XtFree(value);
    select_done[0] = True;
}
/* Replaces the primary selection, with the insert selection */

/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void InsertSelection(w, closure, seltype, type, value, length, format)
/*--------------------------------------------------------------------------+*/
Widget w;
Opaque closure;
Atom *seltype;
Atom *type;
char *value;
int *length;
int *format;
{
    Boolean *select_done = (Boolean *)closure;
    XmTextFieldWidget tf = (XmTextFieldWidget) w;
    XmTextPosition cursorPos;
    XmTextPosition left = 0;
    XmTextPosition right = 0;
    XmTextPosition old_cursorPos;

    if (!value) {
       select_done[0] = True;
       return;
    }

  /* Don't do replace if there is not text to add */
    if (*value == NULL || *length == 0){
       XtFree(value);
       select_done[0] = True;
       return;
    }

    if (!XmTextFieldGetSelectionPosition (w, &left, &right) && left == right) {
       XBell(XtDisplay(w), 0);
       XtFree(value);
       select_done[0] = True;
       select_done[1] = False;
       return;
    }

    old_cursorPos = TextF_CursorPosition(tf);

    if (!ReplaceText(tf, NULL, left, right, value, *length)) {
       select_done[1] = False;
    } else {
       select_done[1] = True;
       tf->text.pending_off = True;
       SetCursorPosition(tf, NULL, left + *length, False);
       (void) SetDestination((Widget)tf, TextF_CursorPosition(tf),
                    	     False, CurrentTime);
    }

    XtFree(value);
    select_done[0] = True;
}


/*
 * Converts requested target of insert selection.
 */
/*--------------------------------------------------------------------------+*/
static Boolean ConvertInsertSelection(w, selection, type, value,
				      length, format, dest)
/*--------------------------------------------------------------------------+*/
Widget w;
Atom *selection, *type;
caddr_t *value;
int *length, *format;
Boolean dest;
{
   XtAppContext app = XtWidgetToApplicationContext(w);
   XSelectionRequestEvent * req_event;
   static unsigned long old_serial= 0;
   Atom actual_type;
   int actual_format;
   unsigned long nitems;
   unsigned long bytes;
   unsigned char *prop;
   Boolean select_done[2];
   TextInsertPair *pair;

   select_done[0] = False;
   select_done[1] = False;

   req_event = XtGetSelectionRequest(w, *selection, NULL);
      
  /* Work around for intrinsics selection bug */
   if (old_serial != req_event->serial)
      old_serial = req_event->serial;
   else
      return False;

   XGetWindowProperty(req_event->display, req_event->requestor,
  		      req_event->property, 0L, 10000000, False,
		      AnyPropertyType, &actual_type, &actual_format,
		      &nitems, &bytes, &prop);

   pair = (TextInsertPair *)prop;

  /*
   * Make selection request to replace the primary selection
   * with the insert selection.
   */
   if (dest)
      XtGetSelectionValue(w, pair->selection, pair->target, InsertDestSelection,
		          (Opaque)select_done, CurrentTime);
   else
      XtGetSelectionValue(w, pair->selection, pair->target, InsertSelection,
		          (Opaque)select_done, CurrentTime);

  /*
   * Make sure the above selection request is completed
   * before returning from the convert proc.
   */
   for (;;) {
       XEvent event;

       if (select_done[0])
          break;
       XtAppNextEvent(app, &event);
       XtDispatchEvent(&event);
   }

   *type = XmInternAtom(XtDisplay(w), "INSERT_SELECTION", False);
   *format = 8;
   *value = NULL;
   *length = 0;

   XtFree((char *)prop);
   return (select_done[1]);
}

static Boolean ConvertDest(w, selection, target, type, value, length, format)
Widget w;
Atom *selection;
Atom *target;
Atom *type;
caddr_t *value;
int *length;
int *format;
{
    XmTextFieldWidget tf = (XmTextFieldWidget)w;
    Atom XA_TARGETS = XmInternAtom(XtDisplay(w), "TARGETS", False);
    Atom MOTIF_DESTINATION = XmInternAtom(XtDisplay(w),
                                        "MOTIF_DESTINATION", False);
    Atom XA_INSERT_SELECTION = XmInternAtom(XtDisplay(w),
                                           "INSERT_SELECTION", False);
    Atom XA_TIMESTAMP = XmInternAtom(XtDisplay(w), "TIMESTAMP", False);

    if (*selection != MOTIF_DESTINATION) return FALSE;

   /*
    * XA_TARGETS identifies what targets the text widget can
    * provide data for.
    */
    if (*target == XA_TARGETS) {
      Atom *targs = (Atom *)XtMalloc((unsigned) (3 * sizeof(Atom)));
    
      targs[0] = XA_TARGETS;
      targs[1] = XA_INSERT_SELECTION;
      targs[2] = XA_TIMESTAMP;
      *type = XA_TARGETS;
      *value = (caddr_t) targs;
      *length = (3*sizeof(Atom)) >> 2;
      *format = 32;
   } else if (*target == XA_TIMESTAMP) {
      Time *timestamp;
      timestamp = (Time *) XtMalloc((unsigned) sizeof(Time));
      *timestamp = tf->text.dest_time;
      *value = (char *) timestamp;
      *type = XA_TIMESTAMP;
      *length = sizeof(Time);
      *format = 32;
   /*
    * Provide data for XA_INSERT_SELECTION requests
    */
    } else if (*target == XA_INSERT_SELECTION)
        return (ConvertInsertSelection(w, selection, type, value,
				       length, format, True));

    return TRUE;
}


/* ARGSUSED */
static void LoseDestination(w, selection)
Widget w;
Atom *selection;
{
    XmTextFieldWidget tf = (XmTextFieldWidget) w;

    ClearDestination(tf, True);
    tf->text.has_destination = False;
}

static Boolean SetDestination(w, position, disown, set_time)
Widget w;
XmTextPosition position;
Boolean disown;
Time set_time;
{
    XmTextFieldWidget tf = (XmTextFieldWidget) w;
    Boolean result = TRUE;
    Atom MOTIF_DESTINATION = XmInternAtom(XtDisplay(w),
                                        "MOTIF_DESTINATION", False);

    if (!disown) {
        if (tf->text.has_destination) {
           if (tf->text.dest_position != TextF_CursorPosition(tf) ||
               !tf->text.has_focus)
              ClearDestination(tf, False);
        } else {
            result = XtOwnSelection(w, MOTIF_DESTINATION, set_time,
                                    ConvertDest, LoseDestination,
                                    (XtSelectionDoneProc) NULL);
            tf->text.dest_time = set_time;
            tf->text.has_destination = result;

	    if (result) _XmSetDestination(XtDisplay(w), w); 
	    /* Call XmGetDestination(dpy) to get widget that last had 
	       destination cursor. */
        }
        tf->text.dest_position = position;
        if (tf->text.has_destination &&
            (tf->text.dest_position != TextF_CursorPosition(tf) ||
               !tf->text.has_focus))
              DrawDestination(tf);
    } else {
        XtDisownSelection(w, MOTIF_DESTINATION, set_time);

	if (w == XmGetDestination(XtDisplay(w)))
	  _XmSetDestination(XtDisplay(w), (Widget)NULL);

        tf->text.has_destination = False;
    }

    return result;
}



Boolean _XmTextFieldSetDestination(w, position, set_time)
Widget w;
XmTextPosition position;
Time set_time;
{
   XmTextFieldWidget tf = (XmTextFieldWidget) w;

   if (tf->text.has_destination) {
      if (!tf->text.has_focus ||
          tf->text.dest_position != TextF_CursorPosition(tf))
         ClearDestination(tf, False);
   }
   SetDestination(w, position, False, set_time);
   if (!tf->text.has_focus ||
       tf->text.dest_position != TextF_CursorPosition(tf))
      DrawDestination(tf);
}


/*
 * Converts requests for the primary selection.
 */
/*--------------------------------------------------------------------------+*/
static Boolean Convert(w, selection, target, type, value, length, format)
/*--------------------------------------------------------------------------+*/
Widget w;
Atom *selection, *target, *type;
caddr_t *value;
int *length, *format;
{


   XmTextFieldWidget tf = (XmTextFieldWidget)w;
   Atom XA_TARGETS = XmInternAtom(XtDisplay(w), "TARGETS", False);
   Atom XA_INSERT_SELECTION = XmInternAtom(XtDisplay(w),
					  "INSERT_SELECTION", False);
   Atom XA_DELETE = XmInternAtom(XtDisplay(w), "DELETE", False);
   Atom XA_TIMESTAMP = XmInternAtom(XtDisplay(w), "TIMESTAMP", False);
   Atom XA_TEXT = XmInternAtom(XtDisplay(w), "TEXT", False);

   if (*selection != XA_PRIMARY) return False;

  /*
   * XA_TARGETS identifies what targets the text widget can
   * provide data for.
   */
   if (*target == XA_TARGETS) {
      Atom *targs = (Atom *)XtMalloc((unsigned) (6 * sizeof(Atom)));
    
      targs[0] = XA_STRING;
      targs[1] = XA_TARGETS;
      targs[2] = XA_INSERT_SELECTION;
      targs[3] = XA_DELETE;
      targs[4] = XA_TIMESTAMP;
      targs[5] = XA_TEXT;
      *type = XA_TARGETS;
      *value = (caddr_t) targs;
      *length = (6*sizeof(Atom)) >> 2;
      *format = 32;
  /* Provide data for XA_TIMESTAMP requests.  Work around for Intrinsics bug. */
   } else if (*target == XA_TIMESTAMP) {
      Time *timestamp;
      timestamp = (Time *) XtMalloc((unsigned) sizeof(Time));
      *timestamp = tf->text.prim_time;
      *value = (char *) timestamp;
      *type = XA_TIMESTAMP;
      *length = sizeof(Time);
      *format = 32;
  /* Provide data for XA_STRING requests */
   } else if (*target == XA_STRING || *target == XA_TEXT) {
      *type = XA_STRING;
      *format = 8;
      if (tf == NULL || 
	  tf->text.prim_pos_left == tf->text.prim_pos_right)
	 return False;
      *length = tf->text.prim_pos_right - tf->text.prim_pos_left;
      *value = XtMalloc((unsigned) *length + 1);
     /* get the selection value */
      (void) strncpy(*value,
 		      TextF_Value(tf) + tf->text.prim_pos_left,
		      *length);
  /*
   * Provide data for XA_INSERT_SELECTION requests, used in
   * swaping selections.
   */
   } else if (*target == XA_INSERT_SELECTION) {
      return (ConvertInsertSelection(w, selection, type, value,
				     length, format, False));
  /* Delete the selection */
    } else if (*target == XA_DELETE) {
       XmTextBlockRec block;

       block.length = 0;
       if (!ReplaceText(tf, NULL, tf->text.prim_pos_left,
			         tf->text.prim_pos_right, NULL, 0))
          return False;

       if (tf->text.has_destination)
          tf->text.prim_anchor = TextF_CursorPosition(tf);

       tf->text.prim_pos_right = tf->text.prim_pos_left = tf->text.prim_anchor;

       *type = XA_DELETE;
       *value = NULL;
       *length = 0;
       *format = 8;
   } else/* unknown selection type */
      return False;

   return True;
}


/*
 * Clears the primary selection upon reciept of a lose selection
 * notification.
 */
/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void LoseSelection(w, selection)
/*--------------------------------------------------------------------------+*/
Widget w;
Atom *selection;
{
   XmTextFieldWidget tf = (XmTextFieldWidget)w;
   XmAnyCallbackStruct cb;

   if (tf->text.has_primary) {
     /* Deselect the highlighted text, and reset the primary selection */
      DeselectSelection(w, False, 0);

      cb.reason = XmCR_LOSE_PRIMARY;
      cb.event = NULL;
      XtCallCallbackList ((Widget) tf, tf->text.lose_primary_callback, (Opaque) &cb);
   }
}


/*
 * Calls the losing focus verify callback to verify that the application
 * want to traverse out of the text field widget.  Returns the result.
 */
/*--------------------------------------------------------------------------+*/
static Boolean VerifyLeave(tf, event)
/*--------------------------------------------------------------------------+*/
XmTextFieldWidget  tf;
XEvent        *event;
{
    XmTextVerifyCallbackStruct  cbdata;

    cbdata.reason = XmCR_LOSING_FOCUS;
    cbdata.event = event;
    cbdata.doit = True;
    cbdata.currInsert = TextF_CursorPosition(tf);
    cbdata.newInsert = TextF_CursorPosition(tf);
    cbdata.startPos = TextF_CursorPosition(tf);
    cbdata.endPos = TextF_CursorPosition(tf);
    cbdata.text = NULL;
    XtCallCallbackList ((Widget) tf, TextF_LosingFocusCallback(tf), (Opaque) &cbdata);
    return(cbdata.doit);
}

/*--------------------------------------------------------------------------+*/
static void FindWord(tf, begin, left, right)
/*--------------------------------------------------------------------------+*/
XmTextFieldWidget tf;
int begin;
int *left;
int *right;
{
    int start, end;



    for (start = begin; start > 0; start--) {
	if (isspace((int)TextF_Value(tf)[start - 1])) {
	   break;
        }
    }
    *left = start;

    /* CHANGED <= TO < TO KEEP FROM IDENTIFYING THE NULL CHARACTER */
    for (end = begin; end <= tf->text.string_length; end++) {
	if (isspace((int)TextF_Value(tf)[end])) {
           end++;
	   break;
        }
    }
    *right = end - 1;
}


/*--------------------------------------------------------------------------+*/
static void FindPrevWord(tf, left, right)
/*--------------------------------------------------------------------------+*/
XmTextFieldWidget tf;
XmTextPosition *left;
XmTextPosition *right;
{

    XmTextPosition start = TextF_CursorPosition(tf);


       if ((start > 0) && (isspace((int)TextF_Value(tf)[start - 1]))) {
           for (; start > 0; start--) {
               if (!isspace((int)TextF_Value(tf)[start - 1])) {
                  start--;
                  break;
               }
           }
       }
       FindWord(tf, start, left, right);
}

/*--------------------------------------------------------------------------+*/
static void FindNextWord(tf, left, right)
/*--------------------------------------------------------------------------+*/
XmTextFieldWidget tf;
XmTextPosition *left;
XmTextPosition *right;
{

    XmTextPosition end = TextF_CursorPosition(tf);

       if (isspace((int)TextF_Value(tf)[end])) {
           for (end = TextF_CursorPosition(tf);
                end < tf->text.string_length; end++) {
               if (!isspace((int)TextF_Value(tf)[end])) {
                  break;
               }
           }
       }
       FindWord(tf, end, left, right);
      /*
       * Set right to the last witespace following the end of the
       * current word.
       */
       while (*right <= tf->text.string_length &&
              isspace((int)TextF_Value(tf)[(int)*right]))
             *right = *right + 1;
       *right = *right - 1;
}

static void CheckDisjointSelection(w, position, sel_time)
Widget w;
XmTextPosition position;
Time sel_time;
{
    XmTextFieldWidget tf = (XmTextFieldWidget) w;
    XmTextPosition left, right;

    if (!tf->text.add_mode && 
        XmTextFieldGetSelectionPosition ((Widget)tf, &left, &right) &&
        position >= left && position <= right)
       tf->text.pending_off = FALSE;
    else
       tf->text.pending_off = TRUE;

    if (left == right) {
       (void) SetDestination(w, position, False, sel_time);
       tf->text.prim_anchor = position;
    } else {
       (void) SetDestination(w, position, False, sel_time);
       if (!tf->text.add_mode) tf->text.prim_anchor = position;
    }
}

static Boolean NeedsPendingDelete(tf)
XmTextFieldWidget tf;
{
   XmTextPosition left, right;

   return (TextF_PendingDelete(tf) &&
           tf->text.has_primary &&
	   tf->text.prim_pos_left != tf->text.prim_pos_right &&
           tf->text.prim_pos_left <= TextF_CursorPosition(tf) &&
           tf->text.prim_pos_right >= TextF_CursorPosition(tf));
}

/****************************************************************
 *
 * Input functions defined in the action table.
 *
 ****************************************************************/

/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void InsertChar(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
  XmTextFieldWidget tf = (XmTextFieldWidget) w;
  XmTextPosition cursorPos, left, right;
  XComposeStatus text_compose_status;
  char insert_string[TEXT_MAX_INSERT_SIZE];
  int insert_length;

  _XmTextFieldDestinationVisible(w, False);

 /* Determine what was pressed.
  */
  insert_length = XLookupString((XKeyEvent*)event, 
				insert_string, sizeof(insert_string),
                                (KeySym *) NULL, &text_compose_status);

  if (insert_length > 0) {
   /* do not insert non-printing characters */
    if (!XTextWidth(TextF_Font(tf), insert_string, insert_length)) return;

    cursorPos = TextF_CursorPosition(tf);

    if (NeedsPendingDelete(tf)) (void) XmTextFieldRemove(w); 

    if (ReplaceText(tf, (XEvent *) event, TextF_CursorPosition(tf),
		    TextF_CursorPosition(tf), insert_string, insert_length)) {

        CheckDisjointSelection(w, TextF_CursorPosition(tf),
			       event->xkey.time);
        SetCursorPosition(tf, event, TextF_CursorPosition(tf), False);
    }
  }
  _XmTextFieldDestinationVisible(w, True);
}

/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void DeletePrevChar(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
  XmTextFieldWidget tf = (XmTextFieldWidget) w;

  _XmTextFieldDestinationVisible(w, False);

  /* if pending delete is on and there is a selection */
  if (NeedsPendingDelete(tf)) (void) XmTextFieldRemove(w);
  else { 
     if (tf->text.has_primary &&
         tf->text.prim_pos_left != tf->text.prim_pos_right) {
        if (TextF_CursorPosition(tf) - 1 >= 0)
           if (ReplaceText(tf, event, TextF_CursorPosition(tf) - 1,
                              TextF_CursorPosition(tf), NULL, 0)) {
              CheckDisjointSelection(w, TextF_CursorPosition(tf),
	               event->xkey.time);
              SetCursorPosition(tf, event, TextF_CursorPosition(tf), False);
           }
      } else if (TextF_CursorPosition(tf) - 1 >= 0) {
        if (ReplaceText(tf, event, TextF_CursorPosition(tf) - 1,
                           TextF_CursorPosition(tf), NULL, 0)) {
            CheckDisjointSelection(w, TextF_CursorPosition(tf),
	             event->xkey.time);
            SetCursorPosition(tf, event, TextF_CursorPosition(tf), False);
        }
      }  
  }
  _XmTextFieldDestinationVisible(w, True);
}

/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void DeleteNextChar(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
  XmTextFieldWidget tf = (XmTextFieldWidget) w;

  _XmTextFieldDestinationVisible(w, False);

 /* if pending delete is on and there is a selection */
  if (NeedsPendingDelete(tf)) (void) XmTextFieldRemove(w);
  else { 
      if (tf->text.has_primary &&
           tf->text.prim_pos_left != tf->text.prim_pos_right) {
          if (TextF_CursorPosition(tf) < tf->text.string_length)
             if (ReplaceText(tf, event, TextF_CursorPosition(tf),
                             TextF_CursorPosition(tf) + 1, NULL, 0)) {
                 CheckDisjointSelection(w, TextF_CursorPosition(tf),
			                event->xkey.time);
                 SetCursorPosition(tf, event, TextF_CursorPosition(tf), False);
             }
       } else if (TextF_CursorPosition(tf) < tf->text.string_length)
          if (ReplaceText(tf, event, TextF_CursorPosition(tf),
                             TextF_CursorPosition(tf) + 1, NULL, 0)) {
              CheckDisjointSelection(w, TextF_CursorPosition(tf),
			             event->xkey.time);
              SetCursorPosition(tf, event, TextF_CursorPosition(tf), False);
          }
  }
  _XmTextFieldDestinationVisible(w, True);
}

/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void DeletePrevWord(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
  XmTextFieldWidget tf = (XmTextFieldWidget) w;
  XmTextPosition left, right;

  _XmTextFieldDestinationVisible(w, False);

 /* if pending delete is on and there is a selection */
  if (NeedsPendingDelete(tf)) (void) XmTextFieldRemove(w);
  else { 
       FindPrevWord(tf, &left, &right);
       if (tf->text.has_primary &&
           tf->text.prim_pos_left != tf->text.prim_pos_right) {
          if (ReplaceText(tf, event, left, TextF_CursorPosition(tf), NULL, 0)) {
             CheckDisjointSelection(w, TextF_CursorPosition(tf),
                                    event->xkey.time);
             SetCursorPosition(tf, event, TextF_CursorPosition(tf), False);
          }
       } else if (TextF_CursorPosition(tf) - 1 >= 0)
          if (ReplaceText(tf, event, left, TextF_CursorPosition(tf), NULL, 0)) {
              CheckDisjointSelection(w, TextF_CursorPosition(tf),
			             event->xkey.time);
              SetCursorPosition(tf, event, TextF_CursorPosition(tf), False);
          }
  }
  _XmTextFieldDestinationVisible(w, True);
}

/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void DeleteNextWord(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
  XmTextFieldWidget tf = (XmTextFieldWidget) w;
  XmTextPosition left, right;

  _XmTextFieldDestinationVisible(w, False);

 /* if pending delete is on and there is a selection */
  if (NeedsPendingDelete(tf)) (void) XmTextFieldRemove(w);
  else { 
       FindNextWord(tf, &left, &right);
       if (tf->text.has_primary &&
           tf->text.prim_pos_left != tf->text.prim_pos_right) {
          if (ReplaceText(tf, event, TextF_CursorPosition(tf), right, NULL, 0)){
             CheckDisjointSelection(w, TextF_CursorPosition(tf),
				    event->xkey.time);
             SetCursorPosition(tf, event, TextF_CursorPosition(tf), False);
          }
       } else if (TextF_CursorPosition(tf) < tf->text.string_length)
          if (ReplaceText(tf, event, TextF_CursorPosition(tf), right, NULL, 0)){
              CheckDisjointSelection(w, TextF_CursorPosition(tf),
			             event->xkey.time);
              SetCursorPosition(tf, event, TextF_CursorPosition(tf), False);
          }
  }
  _XmTextFieldDestinationVisible(w, True);
}


/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void DeleteToEndOfLine(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
  XmTextFieldWidget tf = (XmTextFieldWidget) w;
    
  _XmTextFieldDestinationVisible(w, False);

 /* if pending delete is on and there is a selection */
  if (NeedsPendingDelete(tf)) (void) XmTextFieldRemove(w);
     else if (TextF_CursorPosition(tf) < tf->text.string_length) {
       if (ReplaceText(tf, event, TextF_CursorPosition(tf),
                          tf->text.string_length, NULL, 0)) {
           CheckDisjointSelection(w, TextF_CursorPosition(tf),
			          event->xkey.time);
           SetCursorPosition(tf, event, TextF_CursorPosition(tf), False);
       }
  }
  _XmTextFieldDestinationVisible(w, True);
}


/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void DeleteToStartOfLine(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
  XmTextFieldWidget tf = (XmTextFieldWidget) w;
    
  _XmTextFieldDestinationVisible(w, False);

 /* if pending delete is on and there is a selection */
  if (NeedsPendingDelete(tf)) (void) XmTextFieldRemove(w);
  else if (TextF_CursorPosition(tf) - 1 >= 0) {
       if (ReplaceText(tf, event, 0, TextF_CursorPosition(tf), NULL, 0)) {
           CheckDisjointSelection(w, TextF_CursorPosition(tf),
			          event->xkey.time);
           SetCursorPosition(tf, event, TextF_CursorPosition(tf), False);
       }
  }
  _XmTextFieldDestinationVisible(w, True);
}

/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void ProcessCancel(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextFieldWidget tf = (XmTextFieldWidget) w;

    XmParentProcessDataRec  p_event ;

    p_event.process_type = XmINPUT_ACTION ;
    p_event.data.input_action.action = XmCANCEL ;
    p_event.data.input_action.event = event ;/* Pointer to XEvent. */
    p_event.data.input_action.params = NULL ; /* Or use what you have if   */
    p_event.data.input_action.num_params = 0 ;/* input is from translation.*/

    if (tf->text.has_secondary) {
       tf->text.cancel = True;
       SetSel2(w, 0, 0, False, event->xkey.time);
       tf->text.has_secondary = False;
       XtUngrabKeyboard(w, CurrentTime);
    }

    if (tf->text.has_primary && tf->text.extending) {
       tf->text.cancel = True;
      /* reset orig_left and orig_right */
       XmTextFieldSetSelection((Widget)tf, tf->text.orig_left,
			       tf->text.orig_right, event->xkey.time);
    }

    if (!tf->text.cancel)
       (void) _XmParentProcess(XtParent(tf), &p_event);

}

/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void Activate(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmAnyCallbackStruct cb;
    XmTextFieldWidget tf = (XmTextFieldWidget) w;
    XmParentProcessDataRec  p_event ;

    p_event.process_type = XmINPUT_ACTION ;
    p_event.data.input_action.action = XmRETURN ;
    p_event.data.input_action.event = event ;/* Pointer to XEvent. */
    p_event.data.input_action.params = NULL ; /* Or use what you have if   */
    p_event.data.input_action.num_params = 0 ;/* input is from translation.*/

    cb.reason = XmCR_ACTIVATE;
    cb.event  = event;
    XtCallCallbackList ((Widget) tf, TextF_ActivateCallback(tf), (Opaque) &cb);

    (void) _XmParentProcess(XtParent(w), &p_event);
}


/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void SimpleMovement(w, event, cursorPos, position)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent *event;
XmTextPosition cursorPos;
XmTextPosition position;
{
  XmTextFieldWidget tf = (XmTextFieldWidget) w;
  XKeyEvent *k_event = (XKeyEvent *) event;

  _XmTextFieldDestinationVisible(w, False);

  if ((tf->text.add_mode && position >= tf->text.prim_pos_left &&
                                position <= tf->text.prim_pos_right) ||
        k_event->state & ShiftMask)
       tf->text.pending_off = FALSE;
  else
       tf->text.pending_off = TRUE;

  if (!tf->text.add_mode) {
       if (!(k_event->state & ShiftMask))
          tf->text.prim_anchor = position; 
       /*
        * if doing selection, reset the destination position
        * prior to calling KeySelection() to avoid having the
        * anchor getting reset to the wrong position.
        */
        if (k_event->state & ShiftMask)
           (void) SetDestination(w, cursorPos, False, event->xkey.time);
  }

  SetCursorPosition(tf, (XEvent *)event, position, True);

  if (k_event->state & ShiftMask) {
       Cardinal num = 0;
       KeySelection(w, event, NULL, &num);
  } else {
       _XmTextFieldDestinationVisible(w, True);
  }
}

/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void BackwardChar(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextFieldWidget tf = (XmTextFieldWidget) w;
    XmTextPosition cursorPos, position;

    cursorPos = TextF_CursorPosition(tf);

    if (cursorPos > 0) {
       position = cursorPos - 1;
       SimpleMovement(tf, event, cursorPos, position);
    }
}

/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void ForwardChar(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextFieldWidget tf = (XmTextFieldWidget) w;
    XmTextPosition cursorPos, position, last_position;

    last_position = tf->text.string_length;
    cursorPos = TextF_CursorPosition(tf);

    if (cursorPos < last_position) {
       position = cursorPos + 1;
       SimpleMovement(tf, event, cursorPos, position);
    }
}

/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void BackwardWord(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
   XmTextFieldWidget tf = (XmTextFieldWidget) w;
   XmTextPosition cursorPos, position, dummy;

   cursorPos = TextF_CursorPosition(tf);

   if (cursorPos > 0) {
      FindPrevWord(tf, &position, &dummy);
      SimpleMovement(tf, event, cursorPos, position);
   }
}

/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void ForwardWord(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
   XmTextFieldWidget tf = (XmTextFieldWidget) w;
   XmTextPosition cursorPos, position, dummy, last_position;

    last_position = tf->text.string_length;
    cursorPos = TextF_CursorPosition(tf);

    if (cursorPos < last_position) {
       FindNextWord(tf, &dummy, &position);
          if(isspace((int)TextF_Value(tf)[position])){
	     for (;position < tf->text.string_length; position++){
	        if (!isspace((int)TextF_Value(tf)[position]))
	        break;
             }
          }

       SimpleMovement(tf, event, cursorPos, position);
    }
}


/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void EndOfLine(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
   XmTextFieldWidget tf = (XmTextFieldWidget) w;
   XmTextPosition cursorPos, position, last_position;

   cursorPos = TextF_CursorPosition(tf);
   last_position = tf->text.string_length;

   if (cursorPos < last_position) {
      position = last_position;
      SimpleMovement(tf, event, cursorPos, position);
   }
}


/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void BeginningOfLine(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
   XmTextFieldWidget tf = (XmTextFieldWidget) w;
   XmTextPosition cursorPos, position;

   cursorPos = TextF_CursorPosition(tf);

   if (cursorPos > 0) {
      position = 0;
      SimpleMovement(tf, event, cursorPos, position);
   }
}

/*--------------------------------------------------------------------------+*/
static void SetSelection(tf, left, right, redisplay)
/*--------------------------------------------------------------------------+*/
XmTextFieldWidget tf;
XmTextPosition left, right;
Boolean redisplay;
{
   XmTextPosition display_left, display_right;
   XmTextPosition old_prim_left, old_prim_right;

   if (left < 0) left = 0;
   if (right < 0) right = 0;

    if (left > tf->text.string_length)
       left = tf->text.string_length;
    if (right > tf->text.string_length)
       right = tf->text.string_length;

   if (left == tf->text.prim_pos_left && right == tf->text.prim_pos_right)
      return;

   TextFieldSetHighlight(tf, tf->text.prim_pos_left,
		         tf->text.prim_pos_right, XmHIGHLIGHT_NORMAL);

   old_prim_left = tf->text.prim_pos_left;
   old_prim_right = tf->text.prim_pos_right;

   if (left > right) {
      tf->text.prim_pos_left = right;
      tf->text.prim_pos_right = left;
   } else {
      tf->text.prim_pos_left = left;
      tf->text.prim_pos_right = right;
   }

   TextFieldSetHighlight(tf, tf->text.prim_pos_left,
	                    tf->text.prim_pos_right, XmHIGHLIGHT_SELECTED);

   if (tf->text.prim_pos_left == tf->text.prim_pos_right)
      XmTextFieldSetAddMode((Widget)tf, False);

   if (redisplay) {
      if (old_prim_left > tf->text.prim_pos_left) {
         display_left = tf->text.prim_pos_left;
      } else if (old_prim_left < tf->text.prim_pos_left) {
         display_left = old_prim_left;
      } else
         display_left = (old_prim_right > tf->text.prim_pos_right) ?
		        tf->text.prim_pos_right : old_prim_right;

      if (old_prim_right < tf->text.prim_pos_right) {
         display_right = tf->text.prim_pos_right;
      } else if (old_prim_right > tf->text.prim_pos_right) {
         display_right = old_prim_right;
      } else
         display_right = (old_prim_left < tf->text.prim_pos_left) ?
		         tf->text.prim_pos_left : old_prim_left;

      RedisplayText(tf, display_left, display_right);
   }
}


/*
 * Begin the selection by gaining ownership of the selection
 * and setting the selection parameters.
 */
/*--------------------------------------------------------------------------+*/
static void StartSelection(tf, left, right, sel_time)
/*--------------------------------------------------------------------------+*/
XmTextFieldWidget tf;
XmTextPosition left, right;
Time sel_time;
{
  if (!XtIsRealized((Widget)tf)) return;

  /* if we don't already own the selection */
  if (!tf->text.has_primary) {
    /*
     * Try to gain ownership. This function identifies the
     * XtConvertSelectionProc and the XtLoseSelectionProc.
     */
     if (XtOwnSelection((Widget)tf, XA_PRIMARY, sel_time, Convert, 
			LoseSelection, (XtSelectionDoneProc) NULL)) {
       XmAnyCallbackStruct cb;

       tf->text.prim_time = sel_time;
       DrawInsertionPoint(tf, False);
       tf->text.has_primary = True; 
       tf->text.prim_pos_left = tf->text.prim_pos_right =
			   tf->text.prim_anchor = TextF_CursorPosition(tf);
      /*
       * Set the selection boundries for highlighting the text,
       * and marking the selection.
       */
       SetSelection(tf, left, right, True);

       DrawInsertionPoint(tf, True);

      /* Call the gain selection callback */
       cb.reason = XmCR_GAIN_PRIMARY;
       cb.event = NULL;
       XtCallCallbackList ((Widget) tf, tf->text.gain_primary_callback, (Opaque) &cb);

    } else 
     /*
      * Failed to gain ownership of the selection so make sure
      * the text does not think it owns the selection.
      * (this might be overkill)
      */
       DeselectSelection((Widget)tf, True, sel_time);
  } else {
       XmTextFieldSetHighlight((Widget)tf, tf->text.prim_pos_left,
		          tf->text.prim_pos_right, XmHIGHLIGHT_NORMAL);
       tf->text.prim_pos_left = tf->text.prim_pos_right =
			   tf->text.prim_anchor = TextF_CursorPosition(tf);
      /*
       * Set the new selection boundries for highlighting the text,
       * and marking the selection.
       */
       SetSelection(tf, left, right, True);

       if (left == right) XmTextFieldSetAddMode((Widget)tf, False);
  }
}

/* ARGSUSED */
static void ProcessHorizontalParams(w, event, params, cursorPos)
Widget w;
XEvent *event;
char **params;
XmTextPosition *cursorPos;
{
    XmTextFieldWidget tf = (XmTextFieldWidget) w;
    XmTextPosition left, right, position;
    float bal_point;

    tf->text.extending = True;
    tf->text.orig_left = tf->text.prim_pos_left;
    tf->text.orig_right = tf->text.prim_pos_right;

    position = TextF_CursorPosition(tf);

    if (!XmTextFieldGetSelectionPosition(w, &left, &right) || left == right) {
        tf->text.orig_left = tf->text.orig_right = tf->text.prim_anchor;
        left = right = position;
        bal_point = tf->text.prim_anchor;
    } else {
        bal_point = (float)(((float)(right - left) / 2.0) + (float)left);

       /* special case for single character selection case */
        if (right - left == 1) {
           if (!strcmp(*params,"right") && position < bal_point)
              bal_point = (position) + 1;
           else if (!strcmp(*params,"left") && position > bal_point)
              bal_point = (position) - 1;
        }
    }


    if (!strcmp(*params, "right")) {
       if (position >= tf->text.string_length) return;
       position++;
    } else if (!strcmp(*params, "left")) {
       if (position <= 0) return;
       position--;
    } else
       return;

   /* shift anchor and direction to opposite end of the selection */
    if (position > tf->text.prim_anchor) {
      tf->text.prim_anchor = tf->text.orig_left; 
      right = position;
      left = tf->text.prim_anchor;
    } else {
      if (position < tf->text.prim_anchor)
         tf->text.prim_anchor = tf->text.orig_right; 
      left = position;
      right = tf->text.prim_anchor;
    }

    if (tf->text.has_primary)
       SetSelection(tf, left, right, True);
    else
       StartSelection(tf, left, right, event->xbutton.time);

    *cursorPos = position;
}


/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void ProcessSelectParams(w, event, position)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent *event;
XmTextPosition *position;
{
   XmTextFieldWidget tf = (XmTextFieldWidget) w;
   XmTextPosition left, right;
   XmTextPosition new_anchor = tf->text.prim_anchor;
   float bal_point;

   tf->text.extending = True;
   tf->text.orig_left = tf->text.prim_pos_left;
   tf->text.orig_right = tf->text.prim_pos_right;

   *position = TextF_CursorPosition(tf);

   if (!XmTextFieldGetSelectionPosition(w, &left, &right) || left == right) {
      tf->text.prim_anchor = tf->text.dest_position;
      left = right = *position;
      tf->text.orig_left = tf->text.orig_right = tf->text.prim_anchor;
      bal_point = tf->text.prim_anchor;
   } else
      bal_point = (float)(((float)(right - left) / 2.0) + (float)left);

   /* shift anchor and direction to opposite end of the selection */
    if (*position > bal_point) {
      tf->text.prim_anchor = tf->text.orig_left; 
      right = *position;
      left = tf->text.prim_anchor;
    } else {
      if (*position < bal_point)
         tf->text.prim_anchor = tf->text.orig_right; 
      left = *position;
      right = tf->text.prim_anchor;
    }

   StartSelection(tf, left, right, event->xbutton.time);
   tf->text.prim_anchor = new_anchor;
}


/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void KeySelection(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
  XmTextFieldWidget tf = (XmTextFieldWidget) w;
  XmTextPosition cursorPos;

  _XmTextFieldDestinationVisible(w, False);

  if (*num_params == 0) {
      ProcessSelectParams(w, event, &cursorPos);
  } else if ((!strcmp(*params, "right")) || (!strcmp(*params, "left"))) {
      ProcessHorizontalParams(w, event, params, &cursorPos);
  }

  tf->text.pending_off = False;

  (void) SetDestination((Widget)tf, cursorPos, False, event->xkey.time);
  SetCursorPosition(tf, event, cursorPos, True);

  tf->text.orig_left = tf->text.prim_pos_left;
  tf->text.orig_right = tf->text.prim_pos_right;

  _XmTextFieldDestinationVisible(w, True);
}




/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void TextFocusIn(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
   XmTextFieldWidget tf = (XmTextFieldWidget) w;
   Boolean newhasfocus = tf->text.has_focus;


   if (event->xfocus.send_event && !(newhasfocus)) {
      if (!tf->text.has_rect) XmTextFieldSetClipRect(tf);
      newhasfocus = True;
   }

   if (newhasfocus != tf->text.has_focus) {
      tf->text.has_focus = newhasfocus;
      if (tf->text.has_destination &&
          tf->text.dest_position == TextF_CursorPosition(tf))
         ClearDestination(tf, False);

      if (tf->core.sensitive)
         ChangeBlinkBehavior(tf, newhasfocus);

      if (_XmGetFocusPolicy((Widget)tf) == XmEXPLICIT)
          _XmHighlightBorder((Widget)w);
   }
}


/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void TextFocusOut(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
   XmTextFieldWidget tf = (XmTextFieldWidget) w;
   Boolean newhasfocus = tf->text.has_focus;


   if (event->xfocus.send_event && newhasfocus) {
        newhasfocus = False;
        XmTextFieldSetAddMode(w, False);	
        _XmUnhighlightBorder((Widget)w);
   }

   if (newhasfocus != tf->text.has_focus) {
      tf->text.has_focus = newhasfocus;
      if (tf->core.sensitive)
         ChangeBlinkBehavior(tf, newhasfocus);
      if (tf->text.has_destination)
         DrawDestination(tf);
   }

   /* If traversal is on, then the leave verification callback is called in
      the traversal event handler */
   if (event->xfocus.send_event && !tf->text.traversed &&
       _XmGetFocusPolicy((Widget)tf) == XmEXPLICIT) {
        if (!VerifyLeave(tf, event)) {
           if (tf->text.verify_bell) XBell(XtDisplay((Widget)tf), 0);
           return;
        }
   } else
        if (tf->text.traversed) {
	   tf->text.traversed = False;
        }
}


/*--------------------------------------------------------------------------+*/
static int SetScanIndex(tf, event)
/*--------------------------------------------------------------------------+*/
XmTextFieldWidget tf;
XEvent *event;
{
   Time sel_time;

   if (event->type == ButtonPress) sel_time = event->xbutton.time;
   else sel_time = event->xkey.time;
	
	
   if (sel_time > tf->text.last_time &&
	sel_time - tf->text.last_time < XtGetMultiClickTime(XtDisplay(tf))) {
       if (tf->text.sarray_index < TextF_SelectionArrayCount(tf)) {
	  tf->text.sarray_index++;
       } else
	  tf->text.sarray_index = 0;
    } else
       tf->text.sarray_index = 0;

    tf->text.last_time = sel_time;
}
    
    
/*--------------------------------------------------------------------------+*/
static void ExtendScanSelection(tf, event)
/*--------------------------------------------------------------------------+*/
XmTextFieldWidget tf;
XEvent *event;
{
   XmTextPosition pivot_left, pivot_right;
   XmTextPosition left, right;
   XmTextPosition new_position = GetPosFromX(tf, (Position) event->xbutton.x);
   XmTextPosition cursorPos = TextF_CursorPosition(tf);
   Boolean pivot_modify = False;
   float bal_point;

   if (!XmTextFieldGetSelectionPosition((Widget)tf, &left, &right) ||
       left == right) {
       tf->text.prim_anchor = tf->text.dest_position;
       tf->text.orig_left = tf->text.orig_right = tf->text.prim_anchor;
       bal_point = tf->text.prim_anchor;
   } else
       bal_point = (float)(((float)(right - left) / 2.0) + (float)left);

   if (!tf->text.extending)
      if ((float)new_position < bal_point) {
         tf->text.prim_anchor = tf->text.orig_right;
      } else if ((float)new_position > bal_point) {
         tf->text.prim_anchor = tf->text.orig_left;
      }

   tf->text.extending = True;

   switch (TextF_SelectionArray(tf)[tf->text.sarray_index]) {
       case XmSELECT_POSITION:
      	   if (tf->text.has_primary)
	      SetSelection(tf, tf->text.prim_anchor, new_position, True);
           else if (new_position != tf->text.prim_anchor)
     	      StartSelection(tf, tf->text.prim_anchor,
			     new_position, event->xbutton.time);
           tf->text.pending_off = False;
           cursorPos = new_position;
           break;
       case XmSELECT_WHITESPACE:
       case XmSELECT_WORD:
	   FindWord(tf, new_position, &left, &right);
           FindWord(tf, tf->text.prim_anchor,
		    &pivot_left, &pivot_right);
           tf->text.pending_off = False;
           if (left != pivot_left || right != pivot_right) {
              if (left > pivot_left)
                 left = pivot_left;
              if (right < pivot_right)
                 right = pivot_right;
              pivot_modify = True;
           }
      	   if (tf->text.has_primary)
              SetSelection(tf, left, right, True);
      	   else
     	      StartSelection(tf, left, right, event->xbutton.time);

           if (pivot_modify) {
              if ((((right - left) / 2) + left) <= new_position) {
                 cursorPos = right;
              } else
                 cursorPos = left;
           } else {
	      if (left >= TextF_CursorPosition(tf))
                 cursorPos = left;
              else
                 cursorPos = right;
           }
           break;
       default:
	   break;
   }
   if (cursorPos != TextF_CursorPosition(tf)) {
      (void) SetDestination((Widget)tf, cursorPos, False, event->xkey.time);
      SetCursorPosition(tf, event, cursorPos, True);
   }
}

/*--------------------------------------------------------------------------+*/
static void SetScanSelection(tf, event)
/*--------------------------------------------------------------------------+*/
XmTextFieldWidget tf;
XEvent *event;
{
   XmTextPosition left, right;
   XmTextPosition new_position = 0;
   XmTextPosition cursorPos = TextF_CursorPosition(tf);
  
   SetScanIndex(tf, event);

   if (event->type == ButtonPress)
       new_position = GetPosFromX(tf, (Position) event->xbutton.x);
   else
       new_position = TextF_CursorPosition(tf);

   switch (TextF_SelectionArray(tf)[tf->text.sarray_index]) {
       case XmSELECT_POSITION:
           tf->text.prim_anchor = new_position;
      	   if (tf->text.has_primary) {
              SetSelection(tf, new_position, new_position, True);
              tf->text.pending_off = False;
           }
           cursorPos = new_position;
           break;
       case XmSELECT_WHITESPACE:
       case XmSELECT_WORD:
	   FindWord(tf, TextF_CursorPosition(tf), &left, &right);
      	   if (tf->text.has_primary)
              SetSelection(tf, left, right, True);
      	   else
     	      StartSelection(tf, left, right, event->xbutton.time);
           tf->text.pending_off = False;
           if ((((right - left) / 2) + left) <= new_position)
              cursorPos = right;
           else
              cursorPos = left;
           break;
       case XmSELECT_LINE:
       case XmSELECT_ALL:
      	   if (tf->text.has_primary)
              SetSelection(tf, 0, tf->text.string_length, True);
      	   else
              StartSelection(tf, 0, tf->text.string_length,
			     event->xbutton.time);
           tf->text.pending_off = False;
   	   if (event->type == ButtonPress)
              if ((tf->text.string_length) / 2 <= new_position)
                 cursorPos = tf->text.string_length;
              else
                 cursorPos = 0;
           break;
   }

   (void) SetDestination((Widget)tf, cursorPos, False, event->xkey.time);
   if (cursorPos != TextF_CursorPosition(tf)) {
      SetCursorPosition(tf, event, cursorPos, True);
   }
}


/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void StartPrimary(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
  XmTextFieldWidget tf = (XmTextFieldWidget) w;

  if (_XmGetFocusPolicy(w) == XmEXPLICIT && w != _XmGetTabGroup(w))
     (void)XmProcessTraversal(w, XmTRAVERSE_CURRENT);

  _XmTextFieldDestinationVisible(w, False);

  SetScanSelection(tf, event); /* use scan type to set the selection */
}


/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void MoveDestination(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
  XmTextFieldWidget tf = (XmTextFieldWidget) w;
  XmTextPosition left, right;
  XmTextPosition new_position;

  new_position = GetPosFromX(tf, (Position) event->xbutton.x);

  if (XmTextFieldGetSelectionPosition(w, &left, &right) && (right != left))
     (void) SetDestination(w, new_position, False, event->xbutton.time);

  tf->text.pending_off = False;
  if (_XmGetFocusPolicy(w) == XmPOINTER)
     SetCursorPosition(tf, event, new_position, True);
  else if (w == _XmGetTabGroup(w) || XmProcessTraversal(w, XmTRAVERSE_CURRENT))
     SetCursorPosition(tf, event, new_position, True);
  if (new_position < left && new_position > right)
     tf->text.pending_off = True;

}

/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void ExtendPrimary(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
  XmTextFieldWidget tf = (XmTextFieldWidget) w;
  XmTextPosition left, right;

  if (tf->text.extending) _XmTextFieldDestinationVisible(w, False);

  if (tf->text.cancel) return;

  ExtendScanSelection(tf, event); /* use scan type to set the selection */

  tf->text.do_drop = False;
}


/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void ExtendEnd(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
  XmTextFieldWidget tf = (XmTextFieldWidget) w;

  tf->text.orig_left = tf->text.prim_pos_left;
  tf->text.orig_right = tf->text.prim_pos_right;
  tf->text.extending = False;
  tf->text.cancel = False;

  _XmTextFieldDestinationVisible(w, True);
}


/*--------------------------------------------------------------------------+*/
static Boolean Convert2(w, seltype, target, type, value, length, format)
/*--------------------------------------------------------------------------+*/
Widget w;
Atom *seltype;
Atom *target;
Atom *type;
caddr_t *value;
int *length;
int *format;
{
    XmTextFieldWidget tf = (XmTextFieldWidget) w;
    Atom XA_TARGETS = XmInternAtom(XtDisplay(w), "TARGETS", False);
    Atom XA_TEXT = XmInternAtom(XtDisplay(w), "TEXT", False);
    Atom XA_TIMESTAMP = XmInternAtom(XtDisplay(w), "TIMESTAMP", False);

    if (*seltype != XA_SECONDARY) return FALSE;
    if (*target == XA_STRING || *target == XA_TEXT) {
      XmTextPosition left, right;

      *type = (Atom) FMT8BIT;
      *format = 8;
      if (tf == NULL || !tf->text.has_secondary ||
          tf->text.sec_pos_left == tf->text.sec_pos_right)
         return FALSE;
      left = tf->text.sec_pos_left;
      right = tf->text.sec_pos_right;
      *length = right - left;
      *value = XtMalloc((unsigned) *length + 1);
      (void) strncpy(*value, TextF_Value(tf) + left, *length);
      return TRUE;
    } else if (*target == XA_TIMESTAMP) {
       Time *timestamp;
       timestamp = (Time *) XtMalloc((unsigned) sizeof(Time));
       *timestamp = tf->text.sec_time;
       *value = (char *) timestamp;
       *type = XA_TIMESTAMP;
       *length = sizeof(Time);
       *format = 32;
    } else if (*target == XA_TARGETS) {
       Atom *a = (Atom *)XtMalloc((unsigned) (4*sizeof(Atom)));
       *value = (caddr_t)a;
       *a++ = XA_STRING;
       *a++ = XA_TEXT;
       *a++ = XA_TIMESTAMP;
       *a = XA_TARGETS;
       *length = (4*sizeof(Atom)) >> 2; /*convert to work count */
       *type = XA_TARGETS; /* or do we need a "list of atoms" type? */
       *format = 32;
       return TRUE;
    } else return FALSE;
}


static void RestorePrimaryHighlight(tf, prim_left, prim_right)
XmTextFieldWidget tf;
XmTextPosition prim_left;
XmTextPosition prim_right;
{
   if (tf->text.sec_pos_right >= prim_left &&
       tf->text.sec_pos_right <= prim_right) {
     /* secondary selection is totally inside primary selection */
      if (tf->text.sec_pos_left >= prim_left) {
         TextFieldSetHighlight(tf, prim_left, tf->text.sec_pos_left,
                            XmHIGHLIGHT_SELECTED);
         TextFieldSetHighlight(tf, tf->text.sec_pos_left,
                               tf->text.sec_pos_right,
                               XmHIGHLIGHT_NORMAL);
         TextFieldSetHighlight(tf, tf->text.sec_pos_right, prim_right,
                               XmHIGHLIGHT_SELECTED);
     /* right side of secondary selection is inside primary selection */
      } else {
         TextFieldSetHighlight(tf, tf->text.sec_pos_left, prim_left,
                               XmHIGHLIGHT_NORMAL);
         TextFieldSetHighlight(tf, prim_left, tf->text.sec_pos_right,
                               XmHIGHLIGHT_SELECTED);
      }
   } else {
     /* left side of secondary selection is inside primary selection */
      if (tf->text.sec_pos_left <= prim_right &&
	  tf->text.sec_pos_left >= prim_left) {
         TextFieldSetHighlight(tf, tf->text.sec_pos_left, prim_right,
                               XmHIGHLIGHT_SELECTED);
         TextFieldSetHighlight(tf, prim_right, tf->text.sec_pos_right,
                               XmHIGHLIGHT_NORMAL);
      } else  {
       /* secondary selection encompasses the primary selection */
        if (tf->text.sec_pos_left <= prim_left &&
            tf->text.sec_pos_right >= prim_right){
           TextFieldSetHighlight(tf, tf->text.sec_pos_left, prim_left,
                                 XmHIGHLIGHT_NORMAL);
           TextFieldSetHighlight(tf, prim_left, prim_right,
                                 XmHIGHLIGHT_SELECTED);
           TextFieldSetHighlight(tf, prim_right, tf->text.sec_pos_right,
                                 XmHIGHLIGHT_NORMAL);
     /* secondary selection is outside primary selection */
        } else {
           TextFieldSetHighlight(tf, prim_left, prim_right,
                                 XmHIGHLIGHT_SELECTED);
           TextFieldSetHighlight(tf, tf->text.sec_pos_left,
                                 tf->text.sec_pos_right,
                                 XmHIGHLIGHT_NORMAL);
        }
      }
   }
}



/*--------------------------------------------------------------------------+*/
static void SetSel2(w, left, right, disown, sel_time)
/*--------------------------------------------------------------------------+*/
Widget w;
XmTextPosition left, right;
Boolean disown;
Time sel_time;
{
    XmTextFieldWidget tf = (XmTextFieldWidget) w;
    Boolean result;

    if (tf->text.has_secondary) {
       XmTextPosition prim_left, prim_right;

       if (left == tf->text.sec_pos_left && right == tf->text.sec_pos_right)
          return;

      /* If the widget has the primary selection, make sure the selection
       * highlight is restored appropriately.
       */
       if (XmTextFieldGetSelectionPosition(w, &prim_left, &prim_right))
          RestorePrimaryHighlight(tf, prim_left, prim_right);
       else
          TextFieldSetHighlight(tf, tf->text.sec_pos_left,
		                  tf->text.sec_pos_right, XmHIGHLIGHT_NORMAL);
    }

    if (left < right) {
       if (!tf->text.has_secondary) {
          result = XtOwnSelection(w, XA_SECONDARY, sel_time, Convert2,
			          LoseSel2, (XtSelectionDoneProc) NULL);
          tf->text.sec_time = sel_time;
          tf->text.has_secondary = result;
          if (result) {
             tf->text.sec_pos_left = left;
             tf->text.sec_pos_right = right;
          } 
       } else {
          tf->text.sec_pos_left = left;
          tf->text.sec_pos_right = right;
       }
       tf->text.sec_drag = True;
   } else {
       tf->text.sec_pos_left = tf->text.sec_pos_right = left;
       if (disown) {
          XtDisownSelection(w, XA_SECONDARY, sel_time);
          tf->text.has_secondary = False;
       }
   }

   TextFieldSetHighlight((Widget)tf, tf->text.sec_pos_left,
		      tf->text.sec_pos_right, XmHIGHLIGHT_SECONDARY_SELECTED);

  /* This can be optimized for performance enhancement */
    RedisplayText(tf, 0, tf->text.string_length);
}

/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void LoseSel2(w, selection)
/*--------------------------------------------------------------------------+*/
Widget w;
Atom *selection;
{
    XmTextFieldWidget tf = (XmTextFieldWidget) w;

    if (tf->text.has_secondary)
       SetSel2(w, 0, 0, True, CurrentTime);
}

/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void StartSecondary(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
  XmTextFieldWidget tf = (XmTextFieldWidget) w;
  XmTextPosition position = GetPosFromX(tf, (Position) event->xbutton.x);

  tf->text.sec_anchor = position;
  tf->text.selection_move = FALSE;

  XtGrabKeyboard(w, False, GrabModeAsync, GrabModeAsync, CurrentTime);
}

/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void ExtendSecondary(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
  XmTextFieldWidget tf = (XmTextFieldWidget) w;
  XmTextPosition position = GetPosFromX(tf, (Position) event->xbutton.x);

  if (tf->text.cancel) return;

  if (position < tf->text.sec_anchor) {
     SetSel2(w, position, tf->text.sec_anchor, False, event->xbutton.time);
  } else if (position > tf->text.sec_anchor) {
     SetSel2(w, tf->text.sec_anchor, position, False, event->xbutton.time);
  } else {
     SetSel2(w, position, position, False, event->xbutton.time);
  }
}


/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void DoStuff(w, closure, seltype, type, value, length, format)
/*--------------------------------------------------------------------------+*/
Widget w;
Opaque closure;
Atom *seltype;
Atom *type;
char *value;
int *length;
int *format;
{
    XmTextFieldWidget tf = (XmTextFieldWidget) w;
    PrimSelect *prim_select = (PrimSelect *) closure;
    Atom XA_DELETE = XmInternAtom(XtDisplay(w), "DELETE", False);
    XmTextPosition right, left;

    if (!value) return;

    if (*type == XA_DELETE) {
       if (prim_select->length > 0 && tf->text.selection_move) {
          if (XmTextFieldGetSelectionPosition(w, &left, &right) &&
              left < prim_select->position) {
             XmTextFieldSetSelection(w, prim_select->position -
			             prim_select->length, prim_select->position,
				     prim_select->time);
             tf->text.prim_anchor = prim_select->position;
          } else {
             XmTextFieldSetSelection(w, prim_select->position,
				    prim_select->position + prim_select->length,
				    prim_select->time);
             tf->text.prim_anchor = prim_select->position;
          }
          (void) SetDestination(w, TextF_CursorPosition(tf),
				False, prim_select->time);
       }
    } else {
       XmTextPosition cursorPos;
       prim_select->length = *length;
       if (ReplaceText(tf, NULL, prim_select->position,
  	                  prim_select->position, value, *length)) {

          tf->text.pending_off = FALSE;
          cursorPos = prim_select->position + *length;
          (void) SetDestination(w, cursorPos, False, prim_select->time);
          SetCursorPosition(tf, NULL, cursorPos, True);
          tf->text.pending_off = TRUE;
          if (prim_select->length == 0 && !tf->text.selection_move)
             tf->text.prim_anchor = cursorPos;
       }
    }
    XtFree(value);
}


/* This function make the request to do a primary paste */

/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void Stuff(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmAnyCallbackStruct cb;
    XmTextFieldWidget tf = (XmTextFieldWidget) w;
    PrimSelect *prim_select = (PrimSelect *)
                                XtMalloc((unsigned)sizeof(PrimSelect));
    XmTextPosition left, right;
    char *closures[2];
    Atom targets[2];

   /*
    * Set stuff position to the x and y position of
    * the button pressed event for primary pastes.
    */
    if (event->type == ButtonRelease) {
       prim_select->time = event->xbutton.time;
       prim_select->position =  GetPosFromX(tf, (Position) event->xbutton.x);
    } else {
       prim_select->time = event->xkey.time;
       prim_select->position = TextF_CursorPosition(tf);
    }

    if (XmTextFieldGetSelectionPosition(w, &left, &right) && left != right &&
        prim_select->position >= left && prim_select->position <= right)
       return;

    prim_select->length = 0;

    targets[0] = XA_STRING;
    closures[0] = (char *)prim_select;

    if (tf->text.selection_move) {
       targets[1] = XmInternAtom(XtDisplay(w), "DELETE", False);
       closures[1] = (char *)prim_select;

      /* Make request to call DoStuff() with the primary selection. */
       XtGetSelectionValues(w, XA_PRIMARY, targets, 2, DoStuff,
                            closures, event->xbutton.time);
    } else {
      /* Make request to call DoStuff() with the primary selection. */
       XtGetSelectionValue(w, XA_PRIMARY, targets[0], DoStuff,
                           (Opaque)prim_select, event->xbutton.time);
    }

}

static void HandleSelectionReplies(w, closure, ev)
Widget w;
XtPointer closure;
XEvent *ev;
{
   XSelectionEvent *event = (XSelectionEvent *) ev;
   XmTextFieldWidget tf = (XmTextFieldWidget) w;
   Atom property = (Atom) closure;
   TextFDestData dest_data;
   XmTextPosition left, right;
   int adjustment = 0;
   Atom XM_TEXT_PROP = XmInternAtom(XtDisplay(w), "_XM_TEXT_I_S_PROP", False);

   if (event->type != SelectionNotify) return;

   XtRemoveEventHandler(w, (EventMask) NULL, TRUE,
                        HandleSelectionReplies, (XtPointer)XM_TEXT_PROP);

   dest_data = GetTextFDestData(w);

   if (event->property == None) {
      (void) SetSel2(w, 0, 0, False, CurrentTime);
      tf->text.selection_move = False;
   } else {
      if (dest_data->has_destination) {
         adjustment = tf->text.sec_pos_right - tf->text.sec_pos_left;

         XmTextFieldSetHighlight((Widget)tf, tf->text.sec_pos_left,
		                 tf->text.sec_pos_right, XmHIGHLIGHT_NORMAL);

         if (dest_data->position <= tf->text.sec_pos_left) {
            tf->text.sec_pos_left += adjustment - dest_data->replace_length;
            tf->text.sec_pos_right += adjustment - dest_data->replace_length;
         } else if (dest_data->position > tf->text.sec_pos_left &&
                    dest_data->position < tf->text.sec_pos_right) {
            tf->text.sec_pos_left -= dest_data->replace_length;
            tf->text.sec_pos_right += adjustment - dest_data->replace_length;
         }
      }

      left = tf->text.sec_pos_left;
      right = tf->text.sec_pos_right;

      (void) SetSel2(w, 0, 0, False, CurrentTime);
      tf->text.has_secondary = False;

      if (tf->text.selection_move) {
         if (ReplaceText(tf, NULL, left, right, NULL, 0)) {
            if (dest_data->has_destination && tf->text.dest_position > right) {
              XmTextPosition cursorPos;
              cursorPos = tf->text.dest_position - (right - left);
              if (!dest_data->quick_key)
                 SetCursorPosition(tf, event, cursorPos, True);
              (void) SetDestination(tf, cursorPos, False, event->time);
            }
            if (!dest_data->has_destination) {
               tf->text.prim_anchor = tf->text.dest_position;
               XmTextFieldSetAddMode(w, False);	
            }
         }
         tf->text.selection_move = False;
      }
   }

   XDeleteProperty(XtDisplay(w), event->requestor, property);
}


/*
 * Notify the primary selection that the secondary selection
 * wants to insert it's selection data into the primary selection.
 */
/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void SecondaryNotify(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextFieldWidget tf = (XmTextFieldWidget) w;
    TextInsertPair *pair = (TextInsertPair *)
			          XtMalloc( (unsigned)sizeof(TextInsertPair));
    Atom XM_TEXT_PROP = XmInternAtom(XtDisplay(w), "_XM_TEXT_I_S_PROP", False);
    Atom XA_ATOM_PAIR = XmInternAtom(XtDisplay(w), "ATOM_PAIR", False);
    Atom XA_INSERT_SELECTION = XmInternAtom(XtDisplay(w),
					   "INSERT_SELECTION", False);
    Atom MOTIF_DESTINATION = XmInternAtom(XtDisplay(w),
                                          "MOTIF_DESTINATION", False);
    TextFDestData dest_data;
    XmTextPosition left, right;

    if (tf->text.selection_move == TRUE && tf->text.has_destination &&
        tf->text.dest_position >= tf->text.sec_pos_left &&
        tf->text.dest_position <= tf->text.sec_pos_right) {
       (void) SetSel2(w, 0, 0, False, CurrentTime);
       return;
    }
        
   /* fill in atom pair */
    pair->selection = XA_SECONDARY;
    pair->target = XA_STRING;

   /* add the insert selection property on the text field widget's window */
    XChangeProperty(XtDisplay(w), XtWindow(w), XM_TEXT_PROP, XA_ATOM_PAIR,
		    32, PropModeReplace, (unsigned char *)pair, 2);

    XtFree((XtPointer)pair);

    dest_data = GetTextFDestData(w);

    dest_data->has_destination = tf->text.has_destination;
    dest_data->position = tf->text.dest_position;
    dest_data->replace_length = 0;

    if (*num_params == 1) dest_data->quick_key = True;
    else dest_data->quick_key = False;

    if (XmTextFieldGetSelectionPosition(w, &left, &right) && left != right) {
       if (dest_data->position >= left && dest_data->position <= right)
          dest_data->replace_length = right - left;
    }

   /* add an event handler to handle selection notify events */
    XtAddEventHandler(w, (EventMask) NULL, TRUE,
		      HandleSelectionReplies, (XtPointer)XM_TEXT_PROP);

   /*
    * Make a request for the primary selection to convert to 
    * type INSERT_SELECTION as per ICCCM.
    */ 
    XConvertSelection(XtDisplay(w), MOTIF_DESTINATION, XA_INSERT_SELECTION,
                      XM_TEXT_PROP, XtWindow(w), event->xbutton.time);
}


/*--------------------------------------------------------------------------+*/
static void ProcessCopy(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmAnyCallbackStruct cb;
    XmTextFieldWidget tf = (XmTextFieldWidget) w;

    if (tf->text.has_secondary &&
               tf->text.sec_pos_left != tf->text.sec_pos_right) {

       SecondaryNotify(w, event, params, num_params);

      /* Call ValueChanged Callback to indicate that text has been modified */
       cb.reason = XmCR_VALUE_CHANGED;
       cb.event = event;
       XtCallCallbackList ((Widget) tf, tf->text.value_changed_callback, (Opaque) &cb);

    } else if (!tf->text.sec_drag && !tf->text.cancel) {
       tf->text.stuff_pos =  GetPosFromX(tf, (Position) event->xbutton.x);
      /*
       * Copy contents of primary selection to the stuff position found above.
       */
       Stuff(w, event, params, num_params);
    }

    if (!tf->text.cancel) XtUngrabKeyboard(w, CurrentTime);
    tf->text.sec_drag = False;
    tf->text.cancel = False;
}


/*--------------------------------------------------------------------------+*/
static void ProcessMove(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextFieldWidget tf = (XmTextFieldWidget) w;

    tf->text.selection_move = TRUE;

    ProcessCopy(w, event, params, num_params);
}


/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void DeleteSelection(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    (void) XmTextFieldRemove(w);
}

/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void ClearSelection(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextFieldWidget tf = (XmTextFieldWidget) w;
    XmTextPosition left = tf->text.prim_pos_left;
    XmTextPosition right = tf->text.prim_pos_right;
    XmTextPosition cursorPos = TextF_CursorPosition(tf);
    XmAnyCallbackStruct cb;

    if (XmTextFieldRemove(w)) {
       int length = right - left;
       char *spaces = (char *)XtMalloc((unsigned) (length + 1));
       int i;

       for (i = 0; i < length; i++) spaces[i] = ' ';
       spaces[length] = 0;

       XmTextFieldInsert(w, left, spaces);
       XtFree(spaces);

       (void) SetDestination(w, cursorPos, False, event->xkey.time);
       SetCursorPosition(tf, event, cursorPos, True);

       cb.reason = XmCR_VALUE_CHANGED;
       cb.event = event;
       XtCallCallbackList ((Widget) tf, tf->text.value_changed_callback, (Opaque) &cb);
    }
}

/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void PageRight(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextFieldWidget tf = (XmTextFieldWidget) w;
    Dimension margin_width = TextF_MarginWidth(tf) +
	                     tf->primitive.shadow_thickness +
			     tf->primitive.highlight_thickness;
    int length = XTextWidth(TextF_Font(tf), TextF_Value(tf),
			    tf->text.string_length);

    _XmTextFieldDestinationVisible(w, False);
    DrawInsertionPoint(tf, False);

    if (length - ((int)(tf->core.width - (2 * margin_width)) -
	 tf->text.h_offset) > tf->core.width - (2 * margin_width))
       tf->text.h_offset -= tf->core.width - (2 * margin_width);
    else
       tf->text.h_offset = -(length - (tf->core.width - (2 * margin_width)));

    RedisplayText(tf, 0, tf->text.string_length);
    _XmTextFieldDestinationVisible(w, True);
}

/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void PageLeft(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextFieldWidget tf = (XmTextFieldWidget) w;
    int margin_width = (int)TextF_MarginWidth(tf) +
	                     tf->primitive.shadow_thickness +
			     tf->primitive.highlight_thickness;

    _XmTextFieldDestinationVisible(w, False);
    DrawInsertionPoint(tf, False);
    if (margin_width + 1 <= tf->text.h_offset +
			    ((int)tf->core.width - (2 * margin_width)))
       tf->text.h_offset = margin_width + 1;
    else
       tf->text.h_offset += tf->core.width - (2 * margin_width);

    RedisplayText(tf, 0, tf->text.string_length);
    _XmTextFieldDestinationVisible(w, True);
}


/*--------------------------------------------------------------------------+*/
static void CopyPrimary(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextFieldWidget tf = (XmTextFieldWidget) w;

    tf->text.selection_move = False;

   /* perform the primary paste action */
    Stuff(w, event, params, num_params);
}


/*--------------------------------------------------------------------------+*/
static void CutPrimary(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextFieldWidget tf = (XmTextFieldWidget) w;

    tf->text.selection_move = True;
    Stuff(w, event, params, num_params);
}

/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void SetAnchor(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextFieldWidget tf = (XmTextFieldWidget) w;
    XmTextPosition left, right;

    tf->text.prim_anchor = TextF_CursorPosition(tf);
    (void) SetDestination(w, tf->text.prim_anchor, False, event->xkey.time);
    if (XmTextFieldGetSelectionPosition(w, &left, &right)) {
       StartSelection(tf, tf->text.prim_anchor,
                          tf->text.prim_anchor, event->xkey.time);
    }
}

/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void ToggleAddMode(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextFieldWidget tf = (XmTextFieldWidget) w;
    XmTextPosition left, right;

    if (tf->text.add_mode)
       XmTextFieldSetAddMode(w, False);	
    else
       XmTextFieldSetAddMode(w, True);	

    if (tf->text.add_mode &&
        (!(XmTextFieldGetSelectionPosition(w, &left, &right)) || left == right))
       tf->text.prim_anchor = tf->text.dest_position;
}

/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void SelectAll(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextFieldWidget tf = (XmTextFieldWidget) w;

    if (tf->text.has_primary)
       SetSelection(tf, 0, tf->text.string_length, True);
    else
       StartSelection(tf, 0, tf->text.string_length,
           	      event->xbutton.time);
    tf->text.pending_off = False;
    if (TextF_CursorPosition(tf) < tf->text.string_length)
       SetCursorPosition(tf, event, 0, True);
    else
       SetCursorPosition(tf, event, tf->text.string_length, True);
}

/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void DeselectAll(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextFieldWidget tf = (XmTextFieldWidget) w;

    SetSelection(tf, 0, TextF_CursorPosition(tf) + 1, False);
    tf->text.pending_off = True;
    SetCursorPosition(tf, event, TextF_CursorPosition(tf), True);
}

/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void QuickCopySet(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextFieldWidget tf = (XmTextFieldWidget) w;
    Boolean result;

    result = XtOwnSelection(w, XA_SECONDARY, event->xkey.time,
                            Convert2, LoseSel2, (XtSelectionDoneProc) NULL);
    tf->text.sec_time = event->xkey.time;
    tf->text.has_secondary = result;

    if (result) {
       tf->text.sec_pos_left = tf->text.sec_pos_right =
             tf->text.sec_anchor =  TextF_CursorPosition(tf);
    }

    tf->text.selection_move = False;
}

/*--------------------------------------------------------------------------+*/
static void QuickCutSet(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextFieldWidget tf = (XmTextFieldWidget) w;

    QuickCopySet(w, event, params, num_params);
    tf->text.selection_move = False;
}

/*--------------------------------------------------------------------------+*/
static void DoQuickAction(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextFieldWidget tf = (XmTextFieldWidget) w;

    XmTextPosition cursorPos = XmTextFieldGetCursorPosition(w);

    TextFieldSetHighlight(tf, tf->text.sec_pos_left,
		         tf->text.sec_pos_right, XmHIGHLIGHT_NORMAL);

    if (tf->text.has_secondary) {
       if (tf->text.sec_anchor < cursorPos) {
          tf->text.sec_pos_left = tf->text.sec_anchor;
          tf->text.sec_pos_right = cursorPos;
       } else if (tf->text.sec_anchor > cursorPos) {
          tf->text.sec_pos_left = cursorPos;
          tf->text.sec_pos_right = tf->text.sec_anchor;
       }

       XmTextFieldSetHighlight((Widget)tf, tf->text.sec_pos_left,
		        tf->text.sec_pos_right, XmHIGHLIGHT_SECONDARY_SELECTED);
      /*
       * When num_params equals 1, the SecondaryNotify() action proc
       * knows that the action came from a key action
       */
       *num_params = 1;
       SecondaryNotify(w, event, params, num_params);
    }
}

/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void CutClipboard(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    (void) XmTextFieldCut(w, event->xkey.time);
}

/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void CopyClipboard(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextFieldWidget tf = (XmTextFieldWidget) w;

    (void) XmTextFieldCopy(w, event->xkey.time);
    (void) SetDestination(w, TextF_CursorPosition(tf), False, CurrentTime);
}

/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void PasteClipboard(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextFieldWidget tf = (XmTextFieldWidget) w;

    (void) XmTextFieldPaste(w);
}

/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void TraverseDown(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent           *event;
char **params;
Cardinal *num_params;
{
    XmTextFieldWidget tf = (XmTextFieldWidget) w;

    if (tf->primitive.navigation_type == XmNONE && VerifyLeave(tf, event)) {
       tf->text.traversed = True;
       _XmProcessTraversal((Widget)tf, XmTRAVERSE_DOWN, True);
    }
}


/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void TraverseUp(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent           *event;
char **params;
Cardinal *num_params;
{
    XmTextFieldWidget tf = (XmTextFieldWidget) w;

    if (tf->primitive.navigation_type == XmNONE && VerifyLeave(tf, event)) {
       tf->text.traversed = True;
       _XmProcessTraversal((Widget)tf, XmTRAVERSE_UP, True);
    }
}

/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void TraverseHome(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent           *event;
char **params;
Cardinal *num_params;
{
    XmTextFieldWidget tf = (XmTextFieldWidget) w;

   /* Allow the verification routine to control the traversal */
    if (VerifyLeave(tf, event)) {
       tf->text.traversed = True;
       _XmProcessTraversal((Widget)tf, XmTRAVERSE_HOME, True);
    }
}


/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void TraverseNextTabGroup(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent           *event;
char **params;
Cardinal *num_params;
{
    XmTextFieldWidget tf = (XmTextFieldWidget) w;

   /* Allow the verification routine to control the traversal */
    if (VerifyLeave(tf, event)) {
       tf->text.traversed = True;
       _XmProcessTraversal((Widget)tf, XmTRAVERSE_NEXT_TAB_GROUP, True);
    }
}


/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void TraversePrevTabGroup(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent           *event;
char **params;
Cardinal *num_params;
{
    XmTextFieldWidget tf = (XmTextFieldWidget) w;

   /* Allow the verification routine to control the traversal */
    if (VerifyLeave(tf, event)) {
       tf->text.traversed = True;
       _XmProcessTraversal((Widget)tf, XmTRAVERSE_PREV_TAB_GROUP, True);
    }
}


/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void TextEnter(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent           *event;
char **params;
Cardinal *num_params;
{
    XmTextFieldWidget tf = (XmTextFieldWidget) w;
    Boolean newhasfocus = tf->text.has_focus;
    if (_XmGetFocusPolicy((Widget)tf) != XmEXPLICIT && !(newhasfocus) &&
	event->xcrossing.focus) {
       if (!tf->text.has_rect) XmTextFieldSetClipRect(tf);
       newhasfocus = True;
    }

    if (newhasfocus != tf->text.has_focus) {
       tf->text.has_focus = newhasfocus;
       if (XtIsSensitive(w)) {
          if (tf->text.has_destination &&
              tf->text.dest_position == TextF_CursorPosition(tf))
             ClearDestination(tf, False);
          ChangeBlinkBehavior(tf, newhasfocus);
       }
    }
    _XmPrimitiveEnter((XmPrimitiveWidget)w, event);
}

/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void TextLeave(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent           *event;
char **params;
Cardinal *num_params;
{
   XmTextFieldWidget tf = (XmTextFieldWidget) w;
   Boolean newhasfocus = tf->text.has_focus;

   if (_XmGetFocusPolicy((Widget)tf) != XmEXPLICIT && newhasfocus) {
      newhasfocus = False;
      (void) VerifyLeave(tf, event);
   }

   if (newhasfocus != tf->text.has_focus) {
      tf->text.has_focus = newhasfocus;
      if (tf->core.sensitive)
          ChangeBlinkBehavior(tf, newhasfocus);
      if (tf->text.has_destination)
         DrawDestination(tf);
   }

   _XmPrimitiveLeave((XmPrimitiveWidget)w, event);
}

/****************************************************************
 *
 * Private definitions.
 *
 ****************************************************************/

/*
 * ClassPartInitialize sets up the fast subclassing for the widget.i
 * It also merges translation tables.
 */
/*--------------------------------------------------------------------------+*/
static void ClassPartInitialize(class)
/*--------------------------------------------------------------------------+*/
WidgetClass class;
{
    char *event_bindings;

    _XmFastSubclassInit (class, XmTEXT_FIELD_BIT);
    event_bindings = (char *)XtMalloc((unsigned) (strlen(EventBindings1) +
                                      strlen(EventBindings2) +
                                      strlen(EventBindings3) + 1));

    strcpy(event_bindings, EventBindings1);
    strcat(event_bindings, EventBindings2);
    strcat(event_bindings, EventBindings3);
    class->core_class.tm_table =(String)XtParseTranslationTable(event_bindings);
    XtFree(event_bindings);
}

/****************************************************************
 *
 * Private functions used in Initialize.
 *
 ****************************************************************/

/*
 * Verify that the resource settings are valid.  Print a warning
 * message and reset the s if the are invalid.
 */
/*--------------------------------------------------------------------------+*/
static void Validates(tf)
/*--------------------------------------------------------------------------+*/
XmTextFieldWidget tf;
{
    if (TextF_CursorPosition(tf) < 0) {
	  _XmWarning ((Widget)tf, "Invalid cursor position, must be >= 0.");
          TextF_CursorPosition(tf) = 0;
    }

    if (TextF_Columns(tf) <= 0) {
	  _XmWarning ((Widget)tf, "Invalid columns, must be > 0.");
	  TextF_Columns(tf) = 20;
    }
}

static void LoadFontMetrics(tf)
XmTextFieldWidget tf;
{
    XmFontContext context;
    XmStringCharSet charset;
    XFontStruct *font;
    unsigned long charwidth = 0;

    if (!XmFontListInitFontContext(&context, TextF_FontList(tf)))
       _XmWarning ((Widget)tf, "XmFontListInitFontContext Failed.");

    if (!XmFontListGetNextFont(context, &charset, &font))
       _XmWarning ((Widget)tf, "XmFontListGetNextFont Failed.");

    TextF_Font(tf) = font;
    XtFree(charset);
    XmFontListFreeFontContext(context);

    if ((!XGetFontProperty(font, XA_QUAD_WIDTH, &charwidth)) ||
         charwidth == 0) {
       if (font->per_char && font->min_char_or_byte2 <= '0' &&
                             font->max_char_or_byte2 >= '0')
           charwidth = font->per_char['0' - font->min_char_or_byte2].width;
       else
           charwidth = font->max_bounds.width;
    }
    tf->text.average_char_width = charwidth;
}


/*--------------------------------------------------------------------------+*/
static void ValidateString(tf, value)
/*--------------------------------------------------------------------------+*/
XmTextFieldWidget tf;
char *value;
{
   int str_len = strlen(value);
   char *temp_str = XtMalloc((unsigned)(str_len + 1));
   char *start_temp = temp_str;
   char *curr_str = value;
   int i;

   for (i = 0; i < str_len; i++, curr_str++) {
      if (XTextWidth(TextF_Font(tf), curr_str, 1)) {
         *temp_str = *curr_str;
          temp_str++;
      } else {
         char warn_str[52];
         sprintf(warn_str, "Character '%c', not supported in font.  Discarded.",
                 *curr_str);
         _XmWarning ((Widget)tf, warn_str);
      }
   }
   
   *temp_str = '\0';

   tf->text.string_length = strlen(start_temp);

  /* malloc the space for the text value */
   TextF_Value(tf) = strcpy(XtMalloc((unsigned)(tf->text.string_length + 30)), start_temp);
   tf->text.size_allocd = tf->text.string_length + 30;

   XtFree(start_temp);
}


/*
 * Initialize the s in the text fields instance record.
 */
/*--------------------------------------------------------------------------+*/
static void InitializeTextStruct(tf)
/*--------------------------------------------------------------------------+*/
XmTextFieldWidget tf;
{


   /* Flag used in losing focus verification to indicate that a traversal
    * key was pressed.  Must be initialized to False.
    */
    tf->text.traversed = False;
    
    tf->text.add_mode = False;
    tf->text.has_focus = False;
    tf->text.blink_on = False;
    tf->text.cursor_on = False;
    tf->text.has_rect = False;
    tf->text.has_primary = False;
    tf->text.has_secondary = False;
    tf->text.has_destination = False;
    tf->text.selection_move = False;
    tf->text.pending_off = True;
    tf->text.fontlist_created = False;
    tf->text.cancel = False;
    tf->text.extending = False;
    tf->text.prim_time = 0;
    tf->text.dest_time = 0;
    tf->text.sec_drag = False;

    /* copy over the font list */
    if (TextF_FontList(tf) == NULL) {
       TextF_FontList(tf) = _XmGetDefaultFontList((Widget)tf,
				                (unsigned char)XmTEXT_FONTLIST);
       tf->text.fontlist_created = True;
    }

    TextF_FontList(tf) = (XmFontList)XmFontListCopy(TextF_FontList(tf));

    LoadFontMetrics(tf);

    tf->text.gc = NULL;
    tf->text.image_gc = NULL;

    tf->text.h_offset = TextF_MarginWidth(tf) +
			      tf->primitive.shadow_thickness +
                              tf->primitive.highlight_thickness;                 /* PIR2492 */

    if (TextF_Value(tf) == NULL)
	TextF_Value(tf) = "";

    ValidateString(tf, TextF_Value(tf));

   /* truncate the string is it is greater than maxlength */
    if (TextF_MaxLength(tf) < MAXINT) {
       if (tf->text.string_length > TextF_MaxLength(tf)) {
	  tf->text.string_length = TextF_MaxLength(tf);
          TextF_Value(tf)[tf->text.string_length] = '\0';
       }
    }


    tf->text.orig_left = tf->text.orig_right = tf->text.prim_pos_left =
      tf->text.prim_pos_right = tf->text.prim_anchor = TextF_CursorPosition(tf);

    tf->text.sec_pos_left = tf->text.sec_pos_right =
     	tf->text.sec_anchor = TextF_CursorPosition(tf);

    tf->text.stuff_pos = TextF_CursorPosition(tf);
    tf->text.dest_position = TextF_CursorPosition(tf);

    tf->text.cursor_height = tf->text.cursor_width = 0;
    tf->text.putback = NULL;
    tf->text.stipple_tile = (Pixmap) XmGetPixmap(XtScreen(tf), "50_foreground",
                                                 tf->primitive.foreground ^
                                                 tf->core.background_pixel, 0);
    tf->text.dest_cursor = XmUNSPECIFIED_PIXMAP;
    tf->text.add_mode_cursor = XmUNSPECIFIED_PIXMAP;
    tf->text.cursor = XmUNSPECIFIED_PIXMAP;

    tf->text.dest_on = False;
    tf->text.dest_visible = True;

    tf->text.last_time = CurrentTime;

    tf->text.sarray_index = 0;

   /* Initialize highlight elements */
    tf->text.highlight.number = tf->text.highlight.maximum = 1;
    tf->text.highlight.list = (HighlightRec *)XtMalloc((unsigned)
						 sizeof(HighlightRec));
    tf->text.highlight.list[0].position = 0;
    tf->text.highlight.list[0].mode = XmHIGHLIGHT_NORMAL;

    tf->text.timer_id = NULL;

}


/*
 * Get the graphics context for filling the background, and for drawing
 * and inverting text.  Used a unique pixmap so all text field widgets
 * share common GCs.
 */
/*--------------------------------------------------------------------------+*/
static void LoadGCs(tf, background, foreground)
/*--------------------------------------------------------------------------+*/
XmTextFieldWidget tf;
Pixel background, foreground;
{
    Display *display = XtDisplay(tf);
    XGCValues values;
    static XContext context = NULL;
    Pixmap tf_cache_pixmap;
    unsigned long valuemask = (GCFont | GCGraphicsExposures | GCFunction |
			       GCForeground | GCBackground | GCClipMask);

    if (context == NULL) context = XUniqueContext();

    if (XFindContext(display, (Window)0, context, (caddr_t*) &tf_cache_pixmap)) {

      /* Get the Pixmap identifier that the X Toolkit uses to cache our */
      /* GC's.  We never actually use this Pixmap; just so long as it's */
      /* a unique identifier. */
      tf_cache_pixmap = XCreatePixmap(display,
				      (Drawable) DefaultRootWindow(display),
                                      (unsigned int) 1, (unsigned int) 1,
				      (unsigned int) 1);
      XSaveContext(display, (Window)0, context, (void*)tf_cache_pixmap);
    }

   /*
    * Get GC for saving area under the cursor.
    */
    values.font = TextF_Font(tf)->fid;
    values.graphics_exposures = (Bool) TRUE;
    values.function = GXcopy;
    values.foreground = foreground ^ background;
    values.background = 0;
    values.clip_mask = tf_cache_pixmap;
    if (tf->text.gc != NULL)
        XtReleaseGC((Widget)tf, tf->text.gc);
    tf->text.gc = XtGetGC((Widget) tf, valuemask, &values);

    valuemask |= GCTile | GCFillStyle;
    values.tile = tf->text.stipple_tile;
    values.fill_style = FillSolid;
    if (tf->text.image_gc != NULL)
        XtReleaseGC((Widget)tf, tf->text.image_gc);
    tf->text.image_gc = XtGetGC((Widget) tf, valuemask, &values);
}

static void MakeIBeamCursor(tf, pixmap_name, cursor, line_width)
XmTextFieldWidget tf;
char *pixmap_name;
Pixmap *cursor;
int line_width;
{
   Screen *screen = XtScreen(tf);
   Display *dpy = XtDisplay(tf);
   XSegment segments[3];
   Pixel fg = tf->primitive.foreground ^ tf->core.background_pixel;
   GC fillGC;
   XRectangle ClipRect;

  /* Create a stipple to store the I-Beam cursor */
   *cursor =  XCreatePixmap(dpy, XtWindow(tf), tf->text.cursor_width,
			    tf->text.cursor_height, screen->root_depth);

  /* Create a GC for drawing the the segments to the cursor pixmap */
   fillGC = XCreateGC(dpy, *cursor, 0, (XGCValues *)NULL);

  /* Fill the cursor with 0's */
   XFillRectangle(dpy, *cursor, fillGC, 0, 0,
                  tf->text.cursor_width, tf->text.cursor_height);

  /* Draw the segments of the I-Beam */
  /* 1st segment is the top horizontal line of the 'I' */
   segments[0].x1 = 0;
   segments[0].y1 = line_width - 1;
   segments[0].x2 = tf->text.cursor_width;
   segments[0].y2 = line_width - 1;

  /* 2nd segment is the vertical line of the 'I' */
   segments[1].x1 = 0;
   segments[1].y1 = tf->text.cursor_height - 1;
   segments[1].x2 = tf->text.cursor_width;
   segments[1].y2 = tf->text.cursor_height - 1;

  /* 3rd segment is the bottom horizontal line of the 'I' */
   segments[2].x1 = tf->text.cursor_width/2;
   segments[2].y1 = line_width;
   segments[2].x2 = tf->text.cursor_width/2;
   segments[2].y2 = tf->text.cursor_height - 1;

  /* Set the clipping rectangle of the image GC from drawing */
   ClipRect.width = tf->text.cursor_width;
   ClipRect.height = tf->text.cursor_height;
   ClipRect.x = 0;
   ClipRect.y = 0;

   XSetClipRectangles(XtDisplay(tf), tf->text.image_gc, 0, 0,
                      &ClipRect, 1, Unsorted);

  /* Draw the segments onto the cursor */
   XDrawSegments(dpy, *cursor, tf->text.image_gc, segments, 3);

  /* Install the cursor for pixmap caching */
   (void) _XmInstallPixmap(*cursor, screen, pixmap_name, fg, 0);

  /* Free the fill GC */
   XFreeGC(XtDisplay(tf), fillGC);
}

static void MakeAddModeCursor(tf, pixmap_name, cursor, line_width)
XmTextFieldWidget tf;
char *pixmap_name;
Pixmap *cursor;
{
   XtGCMask  valueMask;
   XGCValues values;

   valueMask = GCFillStyle | GCLineWidth;
   values.fill_style = FillTiled;
   values.line_width = line_width;

   XChangeGC(XtDisplay(tf), tf->text.image_gc, valueMask, &values);

   MakeIBeamCursor(tf, pixmap_name, cursor, line_width);

   valueMask = GCFillStyle;
   values.fill_style = FillSolid;

   XChangeGC(XtDisplay(tf), tf->text.image_gc, valueMask, &values);
}


static void MakeDestCursor(tf)
XmTextFieldWidget tf;
{
   Display *dpy = XtDisplay(tf);
   Screen *screen = XtScreen(tf);
   Pixel fg = tf->primitive.foreground ^ tf->core.background_pixel;

  /* Create a pixmap to store the caret cursor stipple */
   tf->text.dest_cursor = XCreatePixmapFromBitmapData(dpy,
						     RootWindowOfScreen(screen),
					             (char*)caretBits, CARET_WIDTH,
					             CARET_HEIGHT, fg, 0, 
					             screen->root_depth);

  /* Install the pixmap for pixmap caching */
   (void) _XmInstallPixmap(tf->text.dest_cursor, screen,
			   "_XmText_Caret", fg, 0);
}


static void MakeCursors(tf)
XmTextFieldWidget tf;
{
   Screen *screen = XtScreen(tf);
   Pixel fg = tf->primitive.foreground ^ tf->core.background_pixel;
   char pixmap_name[25];
   int line_width = 1;

   if (!XtIsRealized((Widget) tf)) return;

   tf->text.cursor_width = 5;
   tf->text.cursor_height = TextF_Font(tf)->ascent + TextF_Font(tf)->descent;

/* Remove old destination cursor */
   if (tf->text.dest_cursor != XmUNSPECIFIED_PIXMAP)
       (void) XmDestroyPixmap(screen, tf->text.dest_cursor);

  /* Create a new destination cursor */
   tf->text.dest_cursor = (Pixmap) XmGetPixmap(screen, "_XmText_Caret", fg, 0);
   if (tf->text.dest_cursor == XmUNSPECIFIED_PIXMAP)
      MakeDestCursor(tf);

   if (tf->text.cursor_height > 19) {
      tf->text.cursor_width++;
      line_width = 2;
   }

  /* Remove old add mode cursor */
   if (tf->text.add_mode_cursor != XmUNSPECIFIED_PIXMAP)
       (void) XmDestroyPixmap(screen, tf->text.add_mode_cursor);

  /* Create a new add_mode cursor */
   sprintf(pixmap_name, "_XmText_AddMode_%d_%d",
		 tf->text.cursor_height, line_width);
   tf->text.add_mode_cursor = (Pixmap) XmGetPixmap(screen, pixmap_name, fg, 0);
   if (tf->text.add_mode_cursor == XmUNSPECIFIED_PIXMAP)
      MakeAddModeCursor(tf, pixmap_name,
                        &(tf->text.add_mode_cursor), line_width);

  /* Remove old insert cursor */
   if (tf->text.cursor != XmUNSPECIFIED_PIXMAP)
       (void) XmDestroyPixmap(screen, tf->text.cursor);

  /* Create a new i-beam cursor */
   sprintf(pixmap_name, "_XmText_%d_%d", tf->text.cursor_height, line_width);
   tf->text.cursor = (Pixmap) XmGetPixmap(screen, pixmap_name, fg, 0);
   if (tf->text.cursor == XmUNSPECIFIED_PIXMAP)
      MakeIBeamCursor(tf, pixmap_name, &(tf->text.cursor), line_width);

   XmSetXorGC(tf, tf->text.image_gc);
}


/*
 *
 * Initialize
 *    Intializes the text data and ensures that the data in new
 * is valid.
 *
 */
/*--------------------------------------------------------------------------+*/
static void Initialize(request, new)
/*--------------------------------------------------------------------------+*/
Widget request, new;
{
    XmTextFieldWidget req_tf = (XmTextFieldWidget) request;
    XmTextFieldWidget new_tf = (XmTextFieldWidget) new;
    Dimension width, height;
  
    Validates(new_tf);

    InitializeTextStruct(new_tf);

    LoadGCs(new_tf, new_tf->core.background_pixel,
		    new_tf->primitive.foreground );

    ComputeSize(new_tf, &width, &height);
  
    if (req_tf->core.width == 0)
       new_tf->core.width = width;
    if (req_tf->core.height == 0)
       new_tf->core.height = height;

}


/*--------------------------------------------------------------------------+*/
static void Realize(w, valueMask, attributes)
/*--------------------------------------------------------------------------+*/
Widget w;
Mask *valueMask;
XSetWindowAttributes *attributes;
{
    XmTextFieldWidget tf = (XmTextFieldWidget) w;
    XtCreateWindow(w, (unsigned int) InputOutput,
                   (Visual *) CopyFromParent, *valueMask, attributes);
    MakeCursors(tf);
    XmTextFieldSetClipRect(tf);
}


/*--------------------------------------------------------------------------+*/
static void Destroy(tf)
/*--------------------------------------------------------------------------+*/
XmTextFieldWidget tf;
{
    if (tf->text.timer_id)
	XtRemoveTimeOut(tf->text.timer_id);

    if (tf->text.has_rect) {
       TextFGCData gc_data = GetTextFGCData((Widget)tf);
       gc_data->tf = NULL;
    }

    XtFree(TextF_Value(tf));


    XmDestroyPixmap (XtScreen(tf), tf->text.stipple_tile);

    XtReleaseGC((Widget)tf, tf->text.gc);
    XtReleaseGC((Widget)tf, tf->text.image_gc);

    XtFree((char *)tf->text.highlight.list);

    if (tf->text.fontlist_created)
       XmFontListFree((XmFontList)TextF_FontList(tf));

    if (tf->text.dest_cursor != XmUNSPECIFIED_PIXMAP)
       (void) XmDestroyPixmap(XtScreen(tf), tf->text.dest_cursor);

    if (tf->text.add_mode_cursor != XmUNSPECIFIED_PIXMAP)
       (void) XmDestroyPixmap(XtScreen(tf), tf->text.add_mode_cursor);

    if (tf->text.cursor != XmUNSPECIFIED_PIXMAP)
       (void) XmDestroyPixmap(XtScreen(tf), tf->text.cursor);

    XtRemoveAllCallbacks ((Widget)tf, XmNactivateCallback);
    XtRemoveAllCallbacks ((Widget)tf, XmNlosingFocusCallback);
    XtRemoveAllCallbacks ((Widget)tf, XmNmodifyVerifyCallback);
    XtRemoveAllCallbacks ((Widget)tf, XmNmotionVerifyCallback);
    XtRemoveAllCallbacks ((Widget)tf, XmNvalueChangedCallback);
}


/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void Resize(w)
/*--------------------------------------------------------------------------+*/
Widget w;
{
    XmTextFieldWidget tf = (XmTextFieldWidget) w;

    XmTextFieldSetClipRect(tf);

    (void) AdjustText(tf, TextF_CursorPosition(tf), True);
}
 

/*--------------------------------------------------------------------------+*/
static XtGeometryResult QueryGeometry(w, intended, reply)
/*--------------------------------------------------------------------------+*/
Widget w;
XtWidgetGeometry *intended, *reply;
{
    if (GMode (intended) & (~(CWWidth | CWHeight)))
       return(XtGeometryNo);

    reply->request_mode = (CWWidth | CWHeight);

    ComputeSize((XmTextFieldWidget) w, &reply->width, &reply->height);

    if ((reply->width != intended->width) ||
	(reply->height != intended->height) ||
	(GMode(intended) != GMode(reply)))
       return (XtGeometryAlmost);
    else {
   	reply->request_mode = 0; 
	return (XtGeometryYes);
    }
}


/*
 * Redisplay will redraw shadows, borders, and text.
 */
/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void TextFieldExpose(w, event, region)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent *event;
Region region;
{
  XmTextFieldWidget tf = (XmTextFieldWidget) w;
  XExposeEvent *xe = (XExposeEvent *) event;
  Boolean erased_cursor = False;

  if (event->xany.type != Expose) return;

  XmTextFieldSetClipRect(tf);

  _XmTextFieldDestinationVisible(w, False);
  if (tf->core.sensitive && tf->text.has_focus)
     ChangeBlinkBehavior(tf, False);

  if (tf->text.has_destination) {
     XClearArea(XtDisplay(w), XtWindow(w), xe->x - CARET_WIDTH,
                xe->y - CARET_HEIGHT, xe->width + (2 * CARET_WIDTH),
                xe->height + (2 * CARET_HEIGHT), FALSE);
     tf->text.dest_on = False;
     erased_cursor = True;
  }

  if (XtIsRealized(tf)) {
     if (tf->primitive.shadow_thickness > 0)
       _XmDrawShadow(XtDisplay(tf), XtWindow(tf),
             tf->primitive.bottom_shadow_GC, 
             tf->primitive.top_shadow_GC,
             (int) tf->primitive.shadow_thickness,
             (int) tf->primitive.highlight_thickness,
             (int) tf->primitive.highlight_thickness,
             (int) (tf->core.width - (2 * tf->primitive.highlight_thickness)),
             (int) (tf->core.height - (2 * tf->primitive.highlight_thickness)));

     if (tf->primitive.highlighted)
       _XmHighlightBorder((Widget)tf);
     else
       _XmUnhighlightBorder((Widget)tf);

  /*
   * Since redisplay clears window, force cursor values to
   * the off state
   */
    tf->text.putback = NULL;
    tf->text.cursor_on = False;

    RedisplayText(tf, 0, tf->text.string_length);

   if (tf->text.has_destination)
       if (erased_cursor && (!tf->text.has_focus ||
            tf->text.dest_position != TextF_CursorPosition(tf)))
          DrawDestination(tf);
 }

  _XmTextFieldDestinationVisible(w, True);
  if (tf->core.sensitive && tf->text.has_focus)
     ChangeBlinkBehavior(tf, True);
}

/*
 *
 * SetValues
 *    Checks the new text data and ensures that the data is valid.
 * Invalid values will be rejected and changed back to the old
 * values.
 *
 */
/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static Boolean SetValues(old, request, new)
/*--------------------------------------------------------------------------+*/
Widget old, request, new;
{
    XmTextFieldWidget new_tf = (XmTextFieldWidget) new;
    XmTextFieldWidget old_tf = (XmTextFieldWidget) old;
    Boolean cursor_pos_set = False;
    Boolean new_size = False;

    if (new->core.being_destroyed) return False;

    if (new->core.sensitive == False &&
        new_tf->text.has_destination) {
       (void) SetDestination(new, TextF_CursorPosition(new_tf),
			     True, CurrentTime);
    }

   /* Make sure the new_tf cursor position is a valid value.
    */
    if (TextF_CursorPosition(new_tf) < 0) {
       _XmWarning ((Widget)new_tf, "Invalid cursor position, must be >= 0.");
       TextF_CursorPosition(new_tf) = TextF_CursorPosition(old_tf);
       cursor_pos_set = False;
    }

    if (TextF_CursorPosition(new_tf) != TextF_CursorPosition(old_tf)) {
       XmTextPosition new_position = TextF_CursorPosition(new_tf);
       TextF_CursorPosition(new_tf) = TextF_CursorPosition(old_tf);
       SetCursorPosition(new_tf, NULL, new_position, True);
       cursor_pos_set = True;
    }

   /* The text must be traversable.
    */
    if (new_tf->primitive.traversal_on != True) {
       _XmWarning ((Widget)new_tf, "Traversal_on must always be true.");
       new_tf->primitive.traversal_on = old_tf->primitive.traversal_on;
    }

    if (TextF_FontList(new_tf)!= TextF_FontList(old_tf)) {
       XmFontListFree((XmFontList) TextF_FontList(old_tf));
       if (TextF_FontList(new_tf) == NULL)
          TextF_FontList(new_tf) = _XmGetDefaultFontList((Widget)new,
							 XmTEXT_FONTLIST);
       TextF_FontList(new_tf) =
			    (XmFontList)XmFontListCopy(TextF_FontList(new_tf));
       LoadFontMetrics(new_tf);

    }

    if (TextF_Value(new_tf) != TextF_Value(old_tf)) {
       Boolean do_it = True;

       ValidateString(new_tf, TextF_Value(new_tf));

      /* If there are modify verify callbacks, verify that we want to continue
       * the action.
       */
       if (TextF_ModifyVerifyCallback(new_tf)) {
         /* If the function ModifyVerify() returns false then don't
          * continue with the action.
          */
          if (!ModifyVerify(new_tf, NULL, 0, old_tf->text.string_length,
			    TextF_Value(new_tf), new_tf->text.string_length)) {
             if (new_tf->text.verify_bell) XBell(XtDisplay((Widget)new_tf), 0);
             TextF_Value(new_tf) = strcpy(XtMalloc((unsigned) old_tf->text.size_allocd), TextF_Value(old_tf));
             new_tf->text.string_length = old_tf->text.string_length;
             XtFree(TextF_Value(old_tf));
             do_it = False;
          }
       }


       if (do_it) {
          XmAnyCallbackStruct cb;

          _XmTextFieldDestinationVisible((Widget)old_tf, False);                     /* PIR2806 */

          XtFree(TextF_Value(old_tf));
          XmTextFieldSetHighlight((Widget)new_tf, new_tf->text.prim_pos_left,
			          new_tf->text.prim_pos_right,
				  XmHIGHLIGHT_NORMAL);

          new_tf->text.pending_off = True;    

          if (TextF_ResizeWidth(new_tf))
             AdjustSize(new_tf);
          else
             if (!AdjustText(new_tf, TextF_CursorPosition(new_tf), False))
                RedisplayText(new_tf, 0, new_tf->text.string_length);

          cb.reason = XmCR_VALUE_CHANGED;
          cb.event = NULL;
          XtCallCallbackList ((Widget) new_tf, TextF_ValueChangedCallback(new_tf),
			        (Opaque) &cb);

          XmTextFieldSetCursorPosition((Widget)new_tf, new_tf->text.string_length);  /* PIR2806 */
          _XmTextFieldDestinationVisible((Widget)new_tf, True);                      /* PIR2806 */
          if (!cursor_pos_set)
             TextF_CursorPosition(new_tf) = new_tf->text.string_length;
       }
    }

    if (new_tf->primitive.foreground != old_tf->primitive.foreground ||
        TextF_FontList(new_tf)!= TextF_FontList(old_tf) ||
        new_tf->core.background_pixel != old_tf->core.background_pixel) {
       LoadGCs(new_tf, new_tf->primitive.foreground,
			 new_tf->core.background_pixel);
       MakeCursors(new_tf);
       XmTextFieldSetClipRect(new_tf);
    }

    if (new_tf->text.has_focus && XtSensitive(new_tf) &&
        TextF_BlinkRate(new_tf) != TextF_BlinkRate(old_tf)) {

        if (TextF_BlinkRate(new_tf) == 0) {
            new_tf->text.blink_on = True;
            if (new_tf->text.timer_id) {
                XtRemoveTimeOut(new_tf->text.timer_id);
                new_tf->text.timer_id = NULL;
            }
        } else if (new_tf->text.timer_id == NULL) {
           new_tf->text.timer_id =
		 XtAppAddTimeOut(XtWidgetToApplicationContext((Widget)new_tf),
				 (unsigned long)TextF_BlinkRate(new_tf),
                                 (XtTimerCallbackProc) HandleTimer,
                                 (caddr_t) new_tf);
        }
        BlinkInsertionPoint(new_tf);
    }

    new_size = TextF_MarginWidth(new_tf) != TextF_MarginWidth(old_tf) ||
               TextF_MarginHeight(new_tf) != TextF_MarginHeight(old_tf) ||
               TextF_FontList(new_tf) != TextF_FontList(old_tf);


    if (TextF_Columns(new_tf) < 0) {
       _XmWarning ((Widget)new_tf, "Invalid columns, must be >= 0.");
       TextF_Columns(new_tf) = TextF_Columns(old_tf);
    }

    if (TextF_Columns(new_tf) != TextF_Columns(old_tf) || new_size) {
       Dimension width, height;

       ComputeSize(new_tf, &width, &height);
       AdjustText(new_tf, 0, False);

       new->core.width = width;
       new->core.height = height;
    }

    if (new_tf->text.has_focus)
       if (XtIsSensitive(new_tf) != XtIsSensitive(old_tf))
          ChangeBlinkBehavior(new_tf, XtIsSensitive(new_tf));

    return FALSE;
}

/***********************************<->***************************************

 *                              Public Functions                             *
 ***********************************<->***************************************/

/*--------------------------------------------------------------------------+*/
char *XmTextFieldGetString (w)
/*--------------------------------------------------------------------------+*/
Widget w;
{
   XmTextFieldWidget tf = (XmTextFieldWidget) w;

   if (tf->text.string_length > 0) {
      return(XtNewString(TextF_Value(tf)));
   } else
      return(XtNewString(""));
}

#ifdef _NO_PROTO
/*--------------------------------------------------------------------------+*/
XmTextPosition XmTextFieldGetLastPosition (w)
/*--------------------------------------------------------------------------+*/
Widget w;
#else /* _NO_PROTO */
XmTextPosition XmTextFieldGetLastPosition (Widget w)
#endif /* _NO_PROTO */
{
   XmTextFieldWidget tf = (XmTextFieldWidget) w;

   return((tf->text.string_length));
}

#ifdef _NO_PROTO
/*--------------------------------------------------------------------------+*/
void XmTextFieldSetString (w, value)
/*--------------------------------------------------------------------------+*/
Widget w;
char *value;
#else /* _NO_PROTO */
void XmTextFieldSetString (Widget w, char *value)
#endif /* _NO_PROTO */
{
    XmTextFieldWidget tf = (XmTextFieldWidget) w;
    XmAnyCallbackStruct cb;

    _XmTextFieldDestinationVisible(w, False);
    if (tf->core.sensitive && tf->text.has_focus)
        ChangeBlinkBehavior(tf, False);

    if (TextF_ModifyVerifyCallback(tf)) {
         /* If the function ModifyVerify() returns false then don't
          * continue with the action.
          */
          if (!ModifyVerify(tf, NULL, 0, tf->text.string_length,
			    value, strlen(value))) {
             if (tf->text.verify_bell) XBell(XtDisplay((Widget)tf), 0);
             return;
          }
    }

    XtFree(TextF_Value(tf));

    ValidateString(tf, value);


    XmTextFieldSetHighlight((Widget)tf, tf->text.prim_pos_left,
       			    tf->text.prim_pos_right, XmHIGHLIGHT_NORMAL);

    tf->text.pending_off = True;    

    if (TextF_ResizeWidth(tf))
       AdjustSize(tf);
    else {
       if (!AdjustText(tf, TextF_CursorPosition(tf), False))
          RedisplayText(tf, 0, tf->text.string_length);
    }

    cb.reason = XmCR_VALUE_CHANGED;
    cb.event = NULL;
    XtCallCallbackList ((Widget) tf, TextF_ValueChangedCallback(tf),
		       (Opaque) &cb);

    XmTextFieldSetCursorPosition(w, tf->text.string_length);
    _XmTextFieldDestinationVisible(w, True);
    if (tf->core.sensitive && tf->text.has_focus)
        ChangeBlinkBehavior(tf, True);
}


#ifdef _NO_PROTO
/*--------------------------------------------------------------------------+*/
void XmTextFieldReplace (w, from_pos, to_pos, value)
/*--------------------------------------------------------------------------+*/
Widget w;
XmTextPosition from_pos, to_pos;
char *value;
#else /* _NO_PROTO */
void XmTextFieldReplace (Widget w, XmTextPosition from_pos, XmTextPosition to_pos, char *value)
#endif /* _NO_PROTO */
{
    XmTextFieldWidget tf = (XmTextFieldWidget) w;
    int save_maxlength = TextF_MaxLength(tf);
    Boolean save_editable = TextF_Editable(tf);

    if (from_pos < 0) from_pos = 0;

    if (to_pos >= tf->text.string_length)
       to_pos = tf->text.string_length;

    TextF_Editable(tf) = True;
    TextF_MaxLength(tf) = MAXINT;
    (void) ReplaceText(tf, NULL, from_pos, to_pos, value, strlen(value));
    TextF_Editable(tf) = save_editable;
    TextF_MaxLength(tf) = save_maxlength;;

    (void) SetDestination(w, TextF_CursorPosition(tf), False, CurrentTime);
}

#ifdef _NO_PROTO
/*--------------------------------------------------------------------------+*/
void XmTextFieldInsert (w, position, value)
/*--------------------------------------------------------------------------+*/
Widget w;
XmTextPosition position;
char *value;
#else /* _NO_PROTO */
void XmTextFieldInsert (Widget w, XmTextPosition position, char *value)
#endif /* _NO_PROTO */
{
    XmTextFieldWidget tf = (XmTextFieldWidget) w;

    XmTextFieldReplace((Widget)tf, position, position, value);
}

#ifdef _NO_PROTO
/*--------------------------------------------------------------------------+*/
void XmTextFieldSetAddMode (w, state)
/*--------------------------------------------------------------------------+*/
Widget w;
Boolean state;
#else /* _NO_PROTO */
void XmTextFieldSetAddMode (Widget w,     
#if NeedWidePrototypes
int state
#else
Boolean state
#endif 
)
#endif /* _NO_PROTO */
{
   XmTextFieldWidget tf = (XmTextFieldWidget) w;

   if (tf->text.cursor_on) DrawInsertionPoint(tf, False);
   tf->text.add_mode = state;
   if (!tf->text.cursor_on) DrawInsertionPoint(tf, True);
}

#ifdef _NO_PROTO
/*--------------------------------------------------------------------------+*/
Boolean XmTextFieldGetAddMode (w)
/*--------------------------------------------------------------------------+*/
Widget w;
#else /* _NO_PROTO */
Boolean XmTextFieldGetAddMode (Widget w)
#endif /* _NO_PROTO */
{
   XmTextFieldWidget tf = (XmTextFieldWidget) w;

   return (tf->text.add_mode);
}

#ifdef _NO_PROTO
/*--------------------------------------------------------------------------+*/
Boolean XmTextFieldGetEditable (w)
/*--------------------------------------------------------------------------+*/
Widget w;
#else /* _NO_PROTO */
Boolean XmTextFieldGetEditable (Widget w)
#endif /* _NO_PROTO */
{
   XmTextFieldWidget tf = (XmTextFieldWidget) w;

   return(TextF_Editable(tf));
}

#ifdef _NO_PROTO
/*--------------------------------------------------------------------------+*/
void XmTextFieldSetEditable (w, editable)
/*--------------------------------------------------------------------------+*/
Widget w;
Boolean editable;
#else /* _NO_PROTO */
void XmTextFieldSetEditable (Widget w,     
#if NeedWidePrototypes
int editable
#else
Boolean editable
#endif 
)
#endif /* _NO_PROTO */
{
   XmTextFieldWidget tf = (XmTextFieldWidget) w;

   TextF_Editable(tf) = editable;
}

#ifdef _NO_PROTO
/*--------------------------------------------------------------------------+*/
int XmTextFieldGetMaxLength (w)
/*--------------------------------------------------------------------------+*/
Widget w;
#else /* _NO_PROTO */
int XmTextFieldGetMaxLength (Widget w)
#endif /* _NO_PROTO */
{
   XmTextFieldWidget tf = (XmTextFieldWidget) w;

   return(TextF_MaxLength(tf));
}

#ifdef _NO_PROTO
/*--------------------------------------------------------------------------+*/
void XmTextFieldSetMaxLength (w, max_length)
/*--------------------------------------------------------------------------+*/
Widget w;
int max_length;
#else /* _NO_PROTO */
void XmTextFieldSetMaxLength (Widget w, int max_length)
#endif /* _NO_PROTO */
{
   XmTextFieldWidget tf = (XmTextFieldWidget) w;

   TextF_MaxLength(tf) = max_length;
}

#ifdef _NO_PROTO
/*--------------------------------------------------------------------------+*/
XmTextPosition XmTextFieldGetCursorPosition(w)
/*--------------------------------------------------------------------------+*/
Widget w;
#else /* _NO_PROTO */
XmTextPosition XmTextFieldGetCursorPosition (Widget w)
#endif /* _NO_PROTO */
{
   XmTextFieldWidget tf = (XmTextFieldWidget) w;

   return TextF_CursorPosition(tf);
}

#ifdef _NO_PROTO
/*--------------------------------------------------------------------------+*/
XmTextPosition XmTextFieldGetInsertionPosition(w)
/*--------------------------------------------------------------------------+*/
Widget w;
#else /* _NO_PROTO */
XmTextPosition XmTextFieldGetInsertionPosition (Widget w)
#endif /* _NO_PROTO */
{
   return XmTextFieldGetCursorPosition(w);
}

#ifdef _NO_PROTO
/*--------------------------------------------------------------------------+*/
void XmTextFieldSetCursorPosition(w, position)
/*--------------------------------------------------------------------------+*/
Widget w;
XmTextPosition position;
#else /* _NO_PROTO */
void XmTextFieldSetCursorPosition (Widget w, XmTextPosition position)
#endif /* _NO_PROTO */
{
   XmTextFieldWidget tf = (XmTextFieldWidget) w;

   SetCursorPosition(tf, NULL, position, True);

   (void) SetDestination(w, TextF_CursorPosition(tf), False, CurrentTime);

   if (tf->text.has_destination &&
       (tf->text.dest_position != TextF_CursorPosition(tf) ||
        !tf->text.has_focus)) {
       DrawDestination(tf);
   }
}

#ifdef _NO_PROTO
/*--------------------------------------------------------------------------+*/
void XmTextFieldSetInsertionPosition(w, position)
/*--------------------------------------------------------------------------+*/
Widget w;
XmTextPosition position;
#else /* _NO_PROTO */
void XmTextFieldSetInsertionPosition (Widget w, XmTextPosition position)
#endif /* _NO_PROTO */
{
   XmTextFieldSetCursorPosition(w, position) ;
}

#ifdef _NO_PROTO
/*--------------------------------------------------------------------------+*/
Boolean XmTextFieldGetSelectionPosition (w, left, right)
/*--------------------------------------------------------------------------+*/
Widget w;
XmTextPosition *left, *right;
#else /* _NO_PROTO */
Boolean XmTextFieldGetSelectionPosition (Widget w, XmTextPosition *left, XmTextPosition *right)
#endif /* _NO_PROTO */
{
   XmTextFieldWidget tf = (XmTextFieldWidget) w;

   if (!tf->text.has_primary) return False;

   *left = tf->text.prim_pos_left;
   *right = tf->text.prim_pos_right;

   return True;
}

#ifdef _NO_PROTO
/*--------------------------------------------------------------------------+*/
char *XmTextFieldGetSelection (w)
/*--------------------------------------------------------------------------+*/
Widget w;
#else /* _NO_PROTO */
char *XmTextFieldGetSelection (Widget w)
#endif /* _NO_PROTO */
{
   XmTextFieldWidget tf = (XmTextFieldWidget) w;
   Cardinal length;
   char *value;

   if (tf->text.prim_pos_left == tf->text.prim_pos_right)
	 return NULL;
   length = tf->text.prim_pos_right - tf->text.prim_pos_left;
   value = XtMalloc((unsigned) length + 1);
   (void) strncpy(value, TextF_Value(tf) + tf->text.prim_pos_left, length);
   value[length] = NULL;
   return (value);
}


#ifdef _NO_PROTO
/*--------------------------------------------------------------------------+*/
Boolean XmTextFieldRemove(w)
/*--------------------------------------------------------------------------+*/
Widget w;
#else /* _NO_PROTO */
Boolean XmTextFieldRemove (Widget w)
#endif /* _NO_PROTO */
{
   XmTextFieldWidget tf = (XmTextFieldWidget) w;
   XmTextPosition left, right;

   if (!XmTextFieldGetSelectionPosition(w, &left, &right) || left == right) {
      tf->text.prim_anchor = TextF_CursorPosition(tf);
      return False;
   }

   if (ReplaceText(tf, NULL, left, right, NULL, 0))
      XmTextFieldSetSelection((Widget)tf, TextF_CursorPosition(tf),
                              TextF_CursorPosition(tf), CurrentTime);

   tf->text.prim_anchor = TextF_CursorPosition(tf);

   return True;
}

#ifdef _NO_PROTO
/*--------------------------------------------------------------------------+*/
Boolean XmTextFieldCopy(w, clip_time)
/*--------------------------------------------------------------------------+*/
Widget w;
Time clip_time;
#else /* _NO_PROTO */
Boolean XmTextFieldCopy (Widget w, Time clip_time)
#endif /* _NO_PROTO */
{
   char *selected_string = XmTextFieldGetSelection(w); /* text selection */
   long item_id = 0;                         /* clipboard item id */
   int data_id = 0;                /* clipboard data id */
   int status = 0;                          /* clipboard status  */
   XmString clip_label;

   /* using the clipboard facilities, copy the selected text to the clipboard */
   if (selected_string != NULL) {
      clip_label = XmStringCreateLtoR ("XM_TEXT_FIELD",
				       XmSTRING_DEFAULT_CHARSET);
     /* start copy to clipboard */
      status = XmClipboardStartCopy (XtDisplay(w), XtWindow(w),
                                     clip_label, clip_time, w,
				     NULL, &item_id);

      if (status != ClipboardSuccess) return False;

     /* move the data to the clipboard */
      status = XmClipboardCopy (XtDisplay(w), XtWindow(w),
                                item_id, "STRING", selected_string,
                                (long)strlen(selected_string)+1, 0, &data_id);

      if (status != ClipboardSuccess) return False;

     /* end the copy to the clipboard */
      status = XmClipboardEndCopy (XtDisplay(w), XtWindow(w),
                                   item_id);

      if (status != ClipboardSuccess) return False;
   } else
      return False;
   return True;
}

#ifdef _NO_PROTO
/*--------------------------------------------------------------------------+*/
Boolean XmTextFieldCut(w, clip_time)
/*--------------------------------------------------------------------------+*/
Widget w;
Time clip_time;
#else /* _NO_PROTO */
Boolean XmTextFieldCut (Widget w, Time clip_time)
#endif /* _NO_PROTO */
{

    XmTextFieldWidget tf = (XmTextFieldWidget) w;
    Boolean success = False;

    if (tf->text.has_destination &&
        tf->text.dest_position != TextF_CursorPosition(tf))
       ClearDestination(tf, False);
    if (XmTextFieldCopy(w, clip_time))
       if (XmTextFieldRemove(w))
          success = True;
    if (tf->text.has_destination &&
        tf->text.dest_position != TextF_CursorPosition(tf))
       DrawDestination(tf);

    return success;
}


/*
 * Retrieves the current data from the clipboard
 * and paste it at the current cursor position
 */

#ifdef _NO_PROTO
/*--------------------------------------------------------------------------+*/
Boolean XmTextFieldPaste(w)
/*--------------------------------------------------------------------------+*/
Widget w;
#else /* _NO_PROTO */
Boolean XmTextFieldPaste (Widget w)
#endif /* _NO_PROTO */
{
   XmTextFieldWidget tf = (XmTextFieldWidget) w;
   XmTextPosition sel_left = 0;
   XmTextPosition sel_right = 0;
   XmTextPosition paste_pos_left, paste_pos_right;
   int status = 0;                            /* clipboard status        */
   char *buffer;                              /* temporary text buffer   */
   unsigned long length;                       /* length of buffer        */
   unsigned long outlength = 0;                /* length of bytes copied  */
   int private_id = 0;                        /* id of item on clipboard */
   Boolean dest_disjoint = True;

   if (tf->text.has_destination)
      paste_pos_left = paste_pos_right = tf->text.dest_position;
   else
      paste_pos_left = paste_pos_right = TextF_CursorPosition(tf);

   status = XmClipboardInquireLength(XtDisplay(w), XtWindow(w),
                                     "STRING", &length);

   if (status == ClipboardNoData || length == 0) return False;

   /* malloc length of clipboard data */
   buffer = XtMalloc((unsigned) length);

   status = XmClipboardRetrieve (XtDisplay(w), XtWindow(w), "STRING",
                                 buffer, length, &outlength, &private_id);

   if (status != ClipboardSuccess) return False;

   if (XmTextFieldGetSelectionPosition(w, &sel_left, &sel_right)) {
      if (tf->text.pending_delete &&
          paste_pos_left >= sel_left && paste_pos_right <= sel_right) {
          paste_pos_left = sel_left;
          paste_pos_right = sel_right;
          dest_disjoint = False;
      }
   }

  /* add new text */
   if (ReplaceText(tf, NULL, paste_pos_left, paste_pos_right,
		   buffer, outlength - 1)) {

       (void) SetDestination(w, TextF_CursorPosition(tf), False, CurrentTime);
       if (sel_left != sel_right) {
           if (!dest_disjoint) {
              tf->text.prim_anchor = tf->text.dest_position;
              XmTextFieldSetSelection(w, tf->text.dest_position,
                                      tf->text.dest_position, CurrentTime);
           } else {
              if (!tf->text.add_mode) {
                 tf->text.prim_anchor = tf->text.dest_position;
                 XmTextFieldSetSelection(w, tf->text.dest_position,
                                         tf->text.dest_position, CurrentTime);
              }
           }
        } else {
           tf->text.prim_anchor = tf->text.dest_position;
        }
   }
   return True;
}

#ifdef _NO_PROTO
/*--------------------------------------------------------------------------+*/
void XmTextFieldClearSelection (w, sel_time)
/*--------------------------------------------------------------------------+*/
Widget w;
Time sel_time;
#else /* _NO_PROTO */
void XmTextFieldClearSelection (Widget w, Time sel_time)
#endif /* _NO_PROTO */
{
    DeselectSelection(w, False, sel_time);
}

#ifdef _NO_PROTO
/*--------------------------------------------------------------------------+*/
void XmTextFieldSetSelection (w, first, last, sel_time)
/*--------------------------------------------------------------------------+*/
Widget w;
XmTextPosition first, last;
Time sel_time;
#else /* _NO_PROTO */
void XmTextFieldSetSelection (Widget w, XmTextPosition first, XmTextPosition last, Time sel_time)
#endif /* _NO_PROTO */
{
    XmTextFieldWidget tf = (XmTextFieldWidget) w;

    StartSelection(tf, first, last, sel_time);
    tf->text.pending_off = False;
    XmTextFieldSetCursorPosition(w, last);
}

/* ARGSUSED */
#ifdef _NO_PROTO
/*--------------------------------------------------------------------------+*/
XmTextPosition XmTextFieldXYToPos(w, x, y)
/*--------------------------------------------------------------------------+*/
Widget w;
Position x, y;
#else /* _NO_PROTO */
XmTextPosition XmTextFieldXYToPos (Widget w,     
#if NeedWidePrototypes
int x, int y
#else
Position x, Position y
#endif 
)
#endif /* _NO_PROTO */
{
    XmTextFieldWidget tf = (XmTextFieldWidget) w;

    return(GetPosFromX(tf, x));
}


#ifdef _NO_PROTO
/*--------------------------------------------------------------------------+*/
Boolean XmTextFieldPosToXY(w, position, x, y)
/*--------------------------------------------------------------------------+*/
Widget w;
XmTextPosition position;
Position *x, *y;
#else /* _NO_PROTO */
Boolean XmTextFieldPosToXY (Widget w, XmTextPosition position, Position *x, Position *y)
#endif /* _NO_PROTO */
{
    XmTextFieldWidget tf = (XmTextFieldWidget) w;

    return(GetXYFromPos(tf, position, x, y));
}


/*
 * Force the given position to be displayed.  If position < 0, then don't force
 * any position to be displayed.
 */

#ifdef _NO_PROTO
/*--------------------------------------------------------------------------+*/
void XmTextFieldShowPosition(w, position)
/*--------------------------------------------------------------------------+*/
Widget w;
XmTextPosition position;
#else /* _NO_PROTO */
void XmTextFieldShowPosition (Widget w, XmTextPosition position)
#endif /* _NO_PROTO */
{
    XmTextFieldWidget tf = (XmTextFieldWidget) w;

    if (position < 0) return;

    AdjustText(tf, position, True);
}

/* ARGSUSED */
#ifdef _NO_PROTO
/*--------------------------------------------------------------------------+*/
void XmTextFieldSetHighlight(w, left, right, mode)
/*--------------------------------------------------------------------------+*/
Widget w;
XmTextPosition left, right;
XmHighlightMode mode;
#else /* _NO_PROTO */
void XmTextFieldSetHighlight (Widget w, XmTextPosition left, XmTextPosition right, XmHighlightMode mode)
#endif /* _NO_PROTO */
{
    XmTextFieldWidget tf = (XmTextFieldWidget) w;

    if (left >= right || right <= 0) return;

    if (left < 0) left = 0;

    if (right > tf->text.string_length)
       right = tf->text.string_length;

    TextFieldSetHighlight(tf, left, right, mode);

    RedisplayText(tf, left, right);
}


#ifdef _NO_PROTO
/*--------------------------------------------------------------------------+*/
int XmTextFieldGetBaseLine(w)
/*--------------------------------------------------------------------------+*/
Widget w;
#else /* _NO_PROTO */
int XmTextFieldGetBaseLine (Widget w)
#endif /* _NO_PROTO */
{
    XmTextFieldWidget tf = (XmTextFieldWidget) w;
    Dimension margin_height = TextF_MarginHeight(tf) +
	                      tf->primitive.shadow_thickness +
			      tf->primitive.highlight_thickness;

    return(margin_height + TextF_Font(tf)->ascent);
}


#ifdef _NO_PROTO
/*--------------------------------------------------------------------------+*/
int XmTextFieldGetBaseline(w)
/*--------------------------------------------------------------------------+*/
Widget w;
#else /* _NO_PROTO */
int XmTextFieldGetBaseline (Widget w)
#endif /* _NO_PROTO */
{
    return XmTextFieldGetBaseLine(w) ;
}

/*
 * Text Field w creation convienence routine.
 */

#ifdef _NO_PROTO
/*--------------------------------------------------------------------------+*/
Widget XmCreateTextField (parent, name, arglist, argcount)
/*--------------------------------------------------------------------------+*/
Widget parent;
char *name;
ArgList arglist;
Cardinal argcount;
#else /* _NO_PROTO */
Widget XmCreateTextField (Widget parent, char *name, ArgList arglist, Cardinal argcount)
#endif /* _NO_PROTO */
{
    return (XtCreateWidget(name, xmTextFieldWidgetClass,
                           parent, arglist, argcount));
}


