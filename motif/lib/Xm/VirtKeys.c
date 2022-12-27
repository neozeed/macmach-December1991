#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "@(#)VirtKeys.c	3.29 91/01/10";
#endif /* lint */
#endif /* REV_INFO */
/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
*  (c) Copyright 1990 HEWLETT-PACKARD COMPANY
*  (c) Copyright 1990 MOTOROLA, INC.
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
#include 	<Xm/XmP.h>
#include	<X11/keysym.h>
#include 	<X11/Xatom.h>
#include 	<Xm/VirtKeysP.h>
#include 	<X11/Shell.h>
#include <Xm/AtomMgr.h>


static void _XmTranslateKey();
static void GetDefaultBindingString();

#define	done(type, value) \
	{							\
	    if (toVal->addr != NULL) {				\
		if (toVal->size < sizeof(type)) {		\
		    toVal->size = sizeof(type);			\
		    return False;				\
		}						\
		*(type*)(toVal->addr) = (value);		\
	    }							\
	    else {						\
		static type static_val;				\
		static_val = (value);				\
		toVal->addr = (XtPointer)&static_val;		\
	    }							\
	    toVal->size = sizeof(type);				\
	    return True;					\
	}

#define GetVirtualBindingsData(display, v)			\
     XFindContext(display, (Window)NULL, virtKeysContext, (caddr_t*)&v)

static XmVirtualKeysymRec virtualKeysyms[] =
{
    {(String)XmVosfBackSpace	, (KeySym)0x1004FF08	},
    {(String)XmVosfInsert	, (KeySym)0x1004FF63	},
    {(String)XmVosfDelete	, (KeySym)0x1004FFFF	},
    {(String)XmVosfCopy		, (KeySym)0x1004FF02	},
    {(String)XmVosfCut		, (KeySym)0x1004FF03	},
    {(String)XmVosfPaste	, (KeySym)0x1004FF04	},
    {(String)XmVosfAddMode	, (KeySym)0x1004FF31	},
    {(String)XmVosfPrimaryPaste	, (KeySym)0x1004FF32	},
    {(String)XmVosfQuickPaste	, (KeySym)0x1004FF33	},
    {(String)XmVosfPageUp	, (KeySym)0x1004FF41	},
    {(String)XmVosfPageDown	, (KeySym)0x1004FF42	},
    {(String)XmVosfEndLine	, (KeySym)0x1004FF57	},
    {(String)XmVosfBeginLine	, (KeySym)0x1004FF58	},
    {(String)XmVosfActivate	, (KeySym)0x1004FF44	},
    {(String)XmVosfMenuBar	, (KeySym)0x1004FF45	},
    {(String)XmVosfClear	, (KeySym)0x1004FF0B	},
    {(String)XmVosfCancel	, (KeySym)0x1004FF69	},
    {(String)XmVosfHelp		, (KeySym)0x1004FF6A	},
    {(String)XmVosfMenu		, (KeySym)0x1004FF67	},
    {(String)XmVosfSelect	, (KeySym)0x1004FF60	},
    {(String)XmVosfUndo		, (KeySym)0x1004FF65	},
    {(String)XmVosfLeft		, (KeySym)0x1004FF51	},
    {(String)XmVosfUp		, (KeySym)0x1004FF52	},
    {(String)XmVosfRight	, (KeySym)0x1004FF53	},
    {(String)XmVosfDown		, (KeySym)0x1004FF54	},
};


static char fallbackBindingString[] = "\
osfCancel	:		<Key>Escape	\n\
osfLeft		:		<Key>Left	\n\
osfUp		:		<Key>Up		\n\
osfRight	:		<Key>Right	\n\
osfDown		:		<Key>Down	\n\
osfEndLine	:		<Key>End	\n\
osfBeginLine	:		<Key>Home	\n\
osfPageUp	:		<Key>Prior	\n\
osfPageDown	:		<Key>Next	\n\
osfBackSpace	:		<Key>BackSpace	\n\
osfDelete	:		<Key>Delete	\n\
osfInsert	:		<Key>Insert	\n\
osfAddMode	:Shift		<Key>F8		\n\
osfHelp		:		<Key>F1		\n\
osfMenu		:		<Key>F4		\n\
osfMenuBar	:		<Key>F10	\n\
osfSelect	:		<Key>Select	\n\
osfActivate	:		<Key>KP_Enter	\n\
osfClear	:		<Key>Clear	\n\
osfUndo		:		<Key>Undo	";

/*"Acorn Computers Ltd"
 * Acorn RISC iX versions 1.0->1.2 running on Acorn R140, R225, R260
 * (all national keyboard variants)*/

static char acornFallbackBindingString[] = "\
osfCancel	:		<Key>Escape	\n\
osfLeft		:		<Key>Left	\n\
osfUp		:		<Key>Up		\n\
osfRight	:		<Key>Right	\n\
osfDown		:		<Key>Down	\n\
osfEndLine	:Alt		<Key>Right	\n\
osfBeginLine	:Alt		<Key>Left	\n\
osfPageUp	:		<Key>Prior	\n\
osfPageDown	:		<Key>Next	\n\
osfBackSpace	:		<Key>BackSpace	\n\
osfDelete	:		<Key>Delete	\n\
osfInsert	:		<Key>Insert	\n\
osfAddMode	:Shift		<Key>F8		\n\
osfHelp		:		<Key>F1		\n\
osfMenu		:		<Key>F4		\n\
osfMenuBar	:		<Key>F10	\n\
osfActivate	:		<Key>KP_Enter	\n\
osfCopy		:		<Key>Select	";

/*"Apollo Computer Inc."*/

static char apolloFallbackBindingString[] = "\
osfCancel	:		<Key>Escape	\n\
osfLeft		:		<Key>Left	\n\
osfUp		:		<Key>Up		\n\
osfRight	:		<Key>Right	\n\
osfDown		:		<Key>Down	\n\
osfEndLine	:		<Key>apRightBar	\n\
osfBeginLine	:		<Key>apLeftBar	\n\
osfPageUp	:		<Key>apUpBox	\n\
osfPageDown	:		<Key>apDownBox	\n\
osfBackSpace	:		<Key>BackSpace	\n\
osfDelete	:		<Key>apCharDel	\n\
osfInsert	:		<Key>Select	\n\
osfAddMode	:Shift		<Key>F8		\n\
osfHelp		:		<Key>Help	\n\
osfMenu		:		<Key>F4		\n\
osfMenuBar	:		<Key>F10	\n\
osfCopy		:		<Key>apCopy	\n\
osfCut		:		<Key>apCut	\n\
osfPaste	:		<Key>apPaste	\n\
osfUndo		:		<Key>Undo	";


/*"DECWINDOWS DigitalEquipmentCorp."*/

static char decFallbackBindingString[] = "\
osfCancel	:		<Key>Escape	\n\
osfLeft		:		<Key>Left	\n\
osfUp		:		<Key>Up		\n\
osfRight	:		<Key>Right	\n\
osfDown		:		<Key>Down	\n\
osfEndLine	:Alt		<Key>Right	\n\
osfBeginLine	:Alt		<Key>Left	\n\
osfPageUp	:		<Key>Prior	\n\
osfPageDown	:		<Key>Next	\n\
osfBackSpace	:		<Key>Delete	\n\
osfDelete	:		<Key>DRemove	\n\
osfInsert	:		<Key>Insert	\n\
osfAddMode	:Shift		<Key>F8		\n\
osfHelp		:		<Key>Help	\n\
osfMenu		:		<Key>F4		\n\
osfMenuBar	:		<Key>F10	\n\
osfSelect	:		<Key>Select	\n\
osfActivate	:		<Key>KP_Enter	\n\
osfPrimaryPaste	:		<Key>F14	\n\
osfQuickPaste	:		<Key>F17	";

/*"Data General Corporation  Rev 04"
 * AViiON */

static char dgFallbackBindingString[] = "\
osfCancel	:		<Key>Escape	\n\
osfLeft		:		<Key>Left	\n\
osfUp		:		<Key>Up		\n\
osfRight	:		<Key>Right	\n\
osfDown		:		<Key>Down	\n\
osfEndLine	:		<Key>End	\n\
osfBeginLine	:		<Key>Home	\n\
osfPageUp	:		<Key>Prior	\n\
osfPageDown	:		<Key>Next	\n\
osfBackSpace	:		<Key>BackSpace	\n\
osfDelete	:		<Key>Delete	\n\
osfInsert	:		<Key>Insert	\n\
osfAddMode	:Shift		<Key>F8		\n\
osfHelp		:		<Key>F1		\n\
osfMenu		:		<Key>F4		\n\
osfMenuBar	:		<Key>F10	";

/*"Double Click Imaging, Inc. KeyX"
 * for the version of KeyX running on 386 AT bus compatibles. */

static char dblclkFallbackBindingString[] = "\
osfCancel	:		<Key>Escape	\n\
osfLeft		:		<Key>Left	\n\
osfUp		:		<Key>Up		\n\
osfRight	:		<Key>Right	\n\
osfDown		:		<Key>Down	\n\
osfEndLine	:		<Key>End	\n\
osfBeginLine	:		<Key>Home	\n\
osfPageUp	:		<Key>Prior	\n\
osfPageDown	:		<Key>Next	\n\
osfBackSpace	:		<Key>BackSpace	\n\
osfDelete	:		<Key>Delete	\n\
osfInsert	:		<Key>Insert	\n\
osfAddMode	:Shift		<Key>F8		\n\
osfHelp		:		<Key>F1		\n\
osfMenu		:		<Key>F4		\n\
osfMenuBar	:		<Key>F10	";

/*"Hewlett-Packard Company" */

static char hpFallbackBindingString[] = "\
osfCancel	:		<Key>Escape	\n\
osfLeft		:		<Key>Left	\n\
osfUp		:		<Key>Up		\n\
osfRight	:		<Key>Right	\n\
osfDown		:		<Key>Down	\n\
osfEndLine	:		<Key>F7		\n\
osfBeginLine	:		<Key>Home	\n\
osfPageUp	:		<Key>Prior	\n\
osfPageDown	:		<Key>Next	\n\
osfBackSpace	:		<Key>BackSpace	\n\
osfDelete	:		<Key>DeleteChar	\n\
osfInsert	:		<Key>InsertChar	\n\
osfAddMode	:Shift		<Key>F8		\n\
osfHelp		:		<Key>F1		\n\
osfMenu		:		<Key>F4		\n\
osfMenuBar	:		<Key>F10	\n\
osfSelect	:		<Key>Select	\n\
osfClear	:		<Key>Clear	\n\
osfUndo		:		<Key>Undo	\n\
osfPrimaryPaste	:		<Key>InsertLine	\n\
osfQuickPaste	:		<Key>DeleteLine	";

/*"International Business Machines"
 * for AIX/PS2 and RS/6000 systems */

static char ibmFallbackBindingString[] = "\
osfCancel	:		<Key>Escape	\n\
osfLeft		:		<Key>Left	\n\
osfUp		:		<Key>Up		\n\
osfRight	:		<Key>Right	\n\
osfDown		:		<Key>Down	\n\
osfEndLine	:		<Key>End	\n\
osfBeginLine	:		<Key>Home	\n\
osfPageUp	:		<Key>Prior	\n\
osfPageDown	:		<Key>Next	\n\
osfBackSpace	:		<Key>BackSpace	\n\
osfDelete	:		<Key>Delete	\n\
osfInsert	:		<Key>Insert	\n\
osfAddMode	:Shift		<Key>F8		\n\
osfHelp		:		<Key>F1		\n\
osfMenu		:		<Key>F4		\n\
osfMenuBar	:		<Key>F10	";

/*"Megatek Corporation"
 * Megatek X-Cellerator */

static char megatekFallbackBindingString[] = "\
osfCancel	:		<Key>Escape	\n\
osfLeft		:		<Key>Left	\n\
osfUp		:		<Key>Up		\n\
osfRight	:		<Key>Right	\n\
osfDown		:		<Key>Down	\n\
osfEndLine	:		<Key>R13	\n\
osfBeginLine	:		<Key>F27	\n\
osfPageUp	:		<Key>F29	\n\
osfPageDown	:		<Key>F35	\n\
osfBackSpace	:		<Key>BackSpace	\n\
osfDelete	:		<Key>Delete	\n\
osfInsert	:		<Key>Insert	\n\
osfAddMode	:Shift		<Key>F8		\n\
osfHelp		:		<Key>Help	\n\
osfMenu		:		<Key>F4		\n\
osfMenuBar	:		<Key>F10	\n\
osfCopy		:		<Key>F16	\n\
osfCut		:		<Key>F20	\n\
osfPaste	:		<Key>F18	\n\
osfUndo		:		<Key>F14	";

/*"Motorola Inc. (Microcomputer Division) " */
/* (c) Copyright 1990 Motorola Inc. */
/* Motorola provides these key bindings as is,
   with no guarantees or warranties implied.
   Motorola is under no obligation to support,
   update, or extend these key bindings for
   future releases. */

static char motorolaFallbackBindingString[] = "\
osfCancel	:		<Key>Escape	\n\
osfLeft		:		<Key>Left	\n\
osfUp		:		<Key>Up		\n\
osfRight	:		<Key>Right	\n\
osfDown		:		<Key>Down	\n\
osfEndLine	:		<Key>End	\n\
osfBeginLine	:		<Key>Home	\n\
osfPageUp	:		<Key>Prior	\n\
osfPageDown	:		<Key>Next	\n\
osfBackSpace	:		<Key>BackSpace	\n\
osfDelete	:		<Key>Delete	\n\
osfInsert	:		<Key>Insert	\n\
osfAddMode	:Shift		<Key>F8		\n\
osfHelp		:		<Key>F1		\n\
osfMenu		:		<Key>F4		\n\
osfMenuBar	:		<Key>F10	";

/*"Silicon Graphics Inc." */

static char sgiFallbackBindingString[] = "\
osfCancel	:		<Key>Escape	\n\
osfLeft		:		<Key>Left	\n\
osfUp		:		<Key>Up		\n\
osfRight	:		<Key>Right	\n\
osfDown		:		<Key>Down	\n\
osfEndLine	:		<Key>End	\n\
osfBeginLine	:		<Key>Home	\n\
osfPageUp	:		<Key>Prior	\n\
osfPageDown	:		<Key>Next	\n\
osfBackSpace	:		<Key>BackSpace	\n\
osfDelete	:		<Key>Delete	\n\
osfInsert	:		<Key>Insert	\n\
osfAddMode	:Shift		<Key>F8		\n\
osfHelp		:		<Key>F1		\n\
osfMenu		:		<Key>F4		\n\
osfMenuBar	:		<Key>F10	";

/*"Siemens Munich by SP-4's Hacker Crew"
 * Siemens WX200 system */

static char siemensWx200FallbackBindingString[] = "\
osfCancel	:		<Key>Escape	\n\
osfLeft		:		<Key>Left	\n\
osfUp		:		<Key>Up		\n\
osfRight	:		<Key>Right	\n\
osfDown		:		<Key>Down	\n\
osfEndLine	:		<Key>End	\n\
osfBeginLine	:		<Key>Home	\n\
osfPageUp	:		<Key>Prior	\n\
osfPageDown	:		<Key>Next	\n\
osfBackSpace	:		<Key>BackSpace	\n\
osfDelete	:		<Key>F29	\n\
osfInsert	:		<Key>Insert	\n\
osfAddMode	:Shift		<Key>F8		\n\
osfHelp		:		<Key>Help	\n\
osfMenu		:		<Key>Menu	\n\
osfMenuBar	:		<Key>F10	";

/*"Siemens Munich (SP-4's hacker-clan)"
 * Siemens 9733 system */

static char siemens9733FallbackBindingString[] = "\
osfCancel	:		<Key>Escape	\n\
osfLeft		:		<Key>Left	\n\
osfUp		:		<Key>Up		\n\
osfRight	:		<Key>Right	\n\
osfDown		:		<Key>Down	\n\
osfEndLine	:		<Key>End	\n\
osfBeginLine	:		<Key>Home	\n\
osfPageUp	:		<Key>Prior	\n\
osfPageDown	:		<Key>Next	\n\
osfBackSpace	:		<Key>BackSpace	\n\
osfDelete	:		<Key>Delete_char\n\
osfInsert	:		<Key>Insert_char\n\
osfAddMode	:Shift		<Key>F8		\n\
osfHelp		:		<Key>Help	\n\
osfMenu		:		<Key>Linefeed	\n\
osfMenuBar	:		<Key>F10	";

/*"X11/NeWS - Sun Microsystems Inc."
 * OpenWindows 1.0.1 Server for a Sun-4
 * with a type 4 keyboard */

static char sunFallbackBindingString[] = "\
osfCancel	:		<Key>Escape	\n\
osfLeft		:		<Key>F30	\n\
osfUp		:		<Key>F28	\n\
osfRight	:		<Key>F32	\n\
osfDown		:		<Key>F34	\n\
osfEndLine	:		<Key>R13	\n\
osfBeginLine	:		<Key>F27	\n\
osfPageUp	:		<Key>F29	\n\
osfPageDown	:		<Key>F35	\n\
osfBackSpace	:		<Key>BackSpace	\n\
osfDelete	:		<Key>Delete	\n\
osfInsert	:		<Key>Insert	\n\
osfAddMode	:Shift		<Key>F8		\n\
osfHelp		:		<Key>Help	\n\
osfMenu		:		<Key>F4		\n\
osfMenuBar	:		<Key>F10	\n\
osfCopy		:		<Key>F16	\n\
osfCut		:		<Key>F20	\n\
osfPaste	:		<Key>F18	\n\
osfUndo		:		<Key>F14	";

/*"Tektronix, Inc." */

static char tekFallbackBindingString[] = "\
osfCancel	:		<Key>Escape	\n\
osfLeft		:		<Key>Left	\n\
osfUp		:		<Key>Up		\n\
osfRight	:		<Key>Right	\n\
osfDown		:		<Key>Down	\n\
osfEndLine	:		<Key>End	\n\
osfBeginLine	:		<Key>Home	\n\
osfPageUp	:		<Key>Prior	\n\
osfPageDown	:		<Key>Next	\n\
osfBackSpace	:		<Key>BackSpace	\n\
osfDelete	:		<Key>Delete	\n\
osfInsert	:		<Key>Insert	\n\
osfAddMode	:Shift		<Key>F8		\n\
osfHelp		:		<Key>F1		\n\
osfMenu		:		<Key>F4		\n\
osfMenuBar	:		<Key>F10	";


static XmDefaultBindingStringRec defaultBindingStrings[] =
{
    {"Acorn Computers Ltd", acornFallbackBindingString},
    {"Apollo Computer Inc.", apolloFallbackBindingString},
    {"DECWINDOWS DigitalEquipmentCorp.", decFallbackBindingString},
    {"Data General Corporation  Rev 04", dgFallbackBindingString},
    {"Double Click Imaging, Inc. KeyX", dblclkFallbackBindingString},
    {"Hewlett-Packard Company", hpFallbackBindingString},
    {"International Business Machines", ibmFallbackBindingString},
    {"Megatek Corporation", megatekFallbackBindingString},
    {"Motorola Inc. (Microcomputer Division) ", motorolaFallbackBindingString},
    {"Silicon Graphics Inc.", sgiFallbackBindingString},
    {"Siemens Munich by SP-4's Hacker Crew", siemensWx200FallbackBindingString},
    {"Siemens Munich (SP-4's hacker-clan)", siemens9733FallbackBindingString},
    {"X11/NeWS - Sun Microsystems Inc.", sunFallbackBindingString},
    {"Tektronix, Inc.", tekFallbackBindingString},
};


static XtResource virtualBindingsResources[] =
{
    {
	"defaultVirtualBindings",
	"DefaultVirtualBindings",
	XmRString,
	sizeof(String),
	XtOffset(XmVirtualBindingsData, bindingsString),
	XmRCallProc,
	(XtPointer)GetDefaultBindingString,
    },
#ifdef DEBUG
    {
	"ignoreProperty",
	"IgnoreProperty",
	XmRBoolean,
	sizeof(Boolean),
	XtOffset(XmVirtualBindingsData, ignoreProperty),
	XmRImmediate,
	(XtPointer)True,
    },
#endif
};


static void GetDefaultBindingString(w, offset, value)
    Widget	w;
    int		offset;
    XrmValue	*value;
{
    Display			*dpy = XtDisplay(w);
    Cardinal			i;
    XmDefaultBindingString	currDefault;
    static String		defaultString;
    
    defaultString = NULL;
    for (i = 0, currDefault = defaultBindingStrings;
	 i < XtNumber(defaultBindingStrings);
	 i++, currDefault++)
      {
	  if (!strcmp(currDefault->vendorName, dpy->vendor))
	    {
		defaultString = currDefault->defaults;
		break;
	    }
      }
    if (!defaultString)
      defaultString = fallbackBindingString;
    
    value->size = sizeof(String);
    value->addr = defaultString;
}


/*ARGSUSED*/
static void FetchDisplayArg(widget, size, value)
    Widget widget;
    Cardinal *size;
    XrmValue* value;
{
    if (widget == NULL) {
	XtErrorMsg("missingWidget", "fetchDisplayArg", "XtToolkitError",
		   "FetchDisplayArg called without a widget to reference",
		   (String*)NULL, (Cardinal*)NULL);
    }
    value->size = sizeof(Display*);
    value->addr = (caddr_t)&DisplayOfScreen(XtScreenOfObject(widget));
}

static XtConvertArgRec displayConvertArg[] = {
    {XtProcedureArg, (XtPointer)FetchDisplayArg, 0},
};

/*ARGSUSED*/
static Boolean CvtStringToVirtualBinding(dpy, args, num_args,
					 fromVal, toVal, closure_ret)
    Display*	dpy;
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr	fromVal;
    XrmValuePtr	toVal;
    XtPointer	*closure_ret;
{
    char 		*str = (char *)fromVal->addr;
    XmKeyBindingRec	keyBindingRec;
    unsigned int	eventType;

    if (*num_args != 1)
      XtAppErrorMsg(XtDisplayToApplicationContext(dpy),
		    "wrongParameters","cvtStringToVirtualBinding","XtToolkitError",
		    "String to virtual Binding conversion needs display argument",
		    (String *)NULL, (Cardinal *)NULL);

    if (_XmMapKeyEvent(str, &eventType, 
			    (unsigned int*)&keyBindingRec.keysym, 
			    &keyBindingRec.modifiers))
      {
	  done(XmKeyBindingRec,  keyBindingRec);
      }
    XtDisplayStringConversionWarning(dpy, str, XmRVirtualBinding);
    return False;
}

static XmKeyBindingRec nullBinding = {NULL, NULL};

static XmKeyBindingRec * FillBindingsFromDB(dpy, rdb)
    Display	*dpy;
    XrmDatabase rdb;
{
    XmKeyBindingRec	*keyBindings, *virtBinding;
    XmVirtualKeysym	virtKey;
    XrmName 		xrm_name[2];
    XrmClass 		xrm_class[2];
    XrmRepresentation 	rep_type;
    XrmValue 		value;
    Cardinal		i;

    xrm_class[0] = XrmStringToQuark(XmRVirtualBinding);
    xrm_class[1] = NULL;

    keyBindings = (XmKeyBindingRec *)
      XtMalloc(sizeof(XmKeyBindingRec) * XtNumber(virtualKeysyms));

    for (virtKey = virtualKeysyms, virtBinding = keyBindings, i = 0; 
	 i < XtNumber(virtualKeysyms);
	 virtKey++, virtBinding++, i++)
      {
	  xrm_name[0] = XrmStringToQuark(virtKey->name);
	  xrm_name[1] = NULL;
	  if (XrmQGetResource(rdb, xrm_name, xrm_class,
			      &rep_type, &value ))
	    {
		if (rep_type == XrmStringToQuark(XmRVirtualBinding))

		  *virtBinding = *(XmKeyBindingRec *)value.addr;

		else if (rep_type == XrmStringToQuark(XmRString)) 
		  {
		      XrmValue toVal;
		      toVal.addr = (caddr_t)virtBinding;
		      toVal.size = sizeof(XmKeyBindingRec);
		      if (!XtCallConverter(dpy, 
					  CvtStringToVirtualBinding, 
					  displayConvertArg,
					  XtNumber(displayConvertArg),
					  &value, 
					  &toVal,
					  (XtCacheRef*)NULL))
			    *virtBinding = nullBinding;
		  }
		else 
		  *virtBinding = nullBinding;
	    }
	  else
	    *virtBinding = nullBinding;
      }
    return keyBindings;
}


#ifdef notdef
static void FillVirtualKeysyms()
{
    Cardinal		i;
    XmVirtualKeysym	virtKey;

    for (i = 0, virtKey = virtualKeysyms; 
	 i < XtNumber(virtualKeysyms);
	 i++, virtKey++)
      virtKey->keysym = XStringToKeysym(virtKey->name);
}
#endif /* notdef */
	  
static String	GetBindingsStringFromProperty(shell)
    Widget	shell;
{
    Atom		motif_bindings_atom;
    long		offset, length;
    Atom		actual_type;
    int			actual_format;
    unsigned long 	num_items, bytes_after;
    char		*prop = 0;
    Window		root = RootWindow(XtDisplay(shell), 0);
    String		data;
    Display		*dpy = XtDisplay(shell);
    /*
     * get the property off the root window and create a
     * resource data base
     */
    motif_bindings_atom = XmInternAtom(dpy,
				       "_MOTIF_BINDINGS",
				       FALSE);
    XGetWindowProperty (dpy, 
			root,
			motif_bindings_atom,
			0, (long)1000000,
			FALSE, XA_STRING,
			&actual_type, &actual_format,
			&num_items, &bytes_after,
			(unsigned char **) &prop);

    if ((actual_type != XA_STRING) ||
	(actual_format != 8) || 
	(num_items == 0))
      {
	  if (prop != 0) XFree((char *)prop);
	  data = NULL;
      }
    else
      data = prop;

    return data;
}

static XmVirtualBindingsData CreateVirtualBindingsData(shell)
    Widget	shell;
{
    XmVirtualBindingsData	bindingData;
    Boolean			ignoreProperty = False;
    XrmDatabase			keyDB;
    String			data;
    Screen			*saveScreen;
    Screen			*foolScreen = XtScreen(shell);

    bindingData = XtNew(XmVirtualBindingsDataRec);
    bindingData->lastKeyEvent = XtNew(XKeyEvent);

    for(; !XtIsApplicationShell(shell); shell = XtParent(shell)) {};
    /*
     * in order to make sure that we use the right display database
     * while still taking advantage of the convenience of the
     * XtGetApplicationResources we fool it by using the screen of
     * the original shell rather than that of the applicationShell
     */
    saveScreen = XtScreen(shell);
    shell->core.screen = foolScreen;
    XtGetApplicationResources(shell,
			      bindingData,
			      virtualBindingsResources,
			      XtNumber(virtualBindingsResources),
			      (ArgList)0,
			      (Cardinal)0);
    shell->core.screen = saveScreen;

#ifdef DEBUG    
      ignoreProperty = bindingData->ignoreProperty;
#endif

    /*
     * bindingData->bindingString will have been filled in by the
     * call proc. Check if we need to override it from the property.
     * This is the default when running in  non debug
     */
    if (!ignoreProperty && (data = GetBindingsStringFromProperty(shell)))
      bindingData->bindingsString = data;
    
    keyDB = XrmGetStringDatabase(bindingData->bindingsString);
    
    bindingData->bindings 
      = FillBindingsFromDB(foolScreen->display,keyDB);
    
    XrmDestroyDatabase(keyDB);
    
    return bindingData;
}
	   
static XContext	virtKeysContext = NULL;

/*
 * This routine is called by vendorShell to provide the global hooks
 * needed on a per-display basis for the virt routines that are called out of the
 * Key proc
 */

#ifdef _NO_PROTO
void _XmVirtKeysInitialize(shell)
    Widget	shell;
#else /* _NO_PROTO */
void _XmVirtKeysInitialize (Widget shell)
#endif /* _NO_PROTO */
{
    Display			*dpy = XtDisplay(shell);
    XmVirtualBindingsData	virtBindingsData;

    if (!virtKeysContext) /* first time */
      {
	  virtKeysContext = XUniqueContext();
	  XtSetTypeConverter(XmRString,
			     XmRVirtualBinding,
			     CvtStringToVirtualBinding,
			     displayConvertArg,
			     XtNumber(displayConvertArg),
			     XtCacheNone, 
			     NULL);
#ifdef notdef
	  FillVirtualKeysyms();
#endif /* notdef */
      }
    if (XFindContext(dpy,
		     (Window)NULL,
		     virtKeysContext,
		     (caddr_t*)&virtBindingsData))
      {
	  virtBindingsData = CreateVirtualBindingsData(shell);
	  XtSetKeyTranslator(dpy, _XmTranslateKey);
	  XSaveContext(dpy,
		       (Window)NULL,
		       virtKeysContext,
		       virtBindingsData);
      }
}

static void FindVirtKey(virtData, keycode, modifiers,
			modifiers_return, keysym_return)
    XmVirtualBindingsData	virtData;
    KeyCode 		keycode;
    Modifiers 		modifiers;
    Modifiers 		*modifiers_return;
    KeySym 		*keysym_return;
{
    XmKeyBinding	keyBindings = virtData->bindings;
    Cardinal		i;
    XmKeyBinding	currBinding;
    Modifiers		eventMods;

    /*
     * get the modifiers from the actual event
     */
    eventMods = (Modifiers)(virtData->lastKeyEvent->state);

    for (i = 0; i < XtNumber(virtualKeysyms); i++)
	 {
	     /*
	      * the null binding should not be interpreted as a match
	      * keysym is zero (e.g. pre-edit terminator)
	      */
	     currBinding = (XmKeyBinding) &keyBindings[i];
	     if ((currBinding->modifiers == (modifiers & eventMods)) &&
		 (currBinding->keysym &&
		  (currBinding->keysym == *keysym_return)))  
	       {
		   *keysym_return = virtualKeysyms[i].keysym;
		   break;
	       }
	 }
    *modifiers_return |= ControlMask | Mod1Mask;
}


/************************************************************************
 *
 *  _XmVirtKeysHandler
 *
 *  This handler provides all kind of magic. It is added to all widgets.
 *     
 ************************************************************************/
/* ARGSUSED */
void _XmVirtKeysHandler(widget, client_data, event, dontSwallow)
    Widget widget;
    XtPointer client_data;
    XEvent *event;
    Boolean *dontSwallow;
{
    XmVirtualBindingsData	virtData;

    GetVirtualBindingsData(XtDisplay(widget),virtData);

    if (widget->core.being_destroyed)
      {
	  *dontSwallow = False;
	  return;
      }
    switch( event->type ) {
      case KeyPress:
	*(virtData->lastKeyEvent) = *((XKeyEvent *)event);
	break;
    }
}

static void _XmTranslateKey(dpy, keycode, modifiers,
			    modifiers_return, keysym_return)
    register Display 	*dpy;
    KeyCode 		keycode;
    Modifiers 		modifiers;
    Modifiers 		*modifiers_return;
    KeySym 		*keysym_return;
{
    static XmTranslateCacheRec	translateCache[256];
    static Display		*displayCache = NULL;
    static unsigned long	serialCache = 0;
    static KeyCode              keycodeCache = 0;

    XmVirtualBindingsData virtBindingsData;

    GetVirtualBindingsData(dpy,virtBindingsData);

    if ((serialCache != virtBindingsData->lastKeyEvent->serial) ||
	(keycodeCache != keycode) ||
	(displayCache != virtBindingsData->lastKeyEvent->display)) {

	keycodeCache = keycode;
	displayCache = virtBindingsData->lastKeyEvent->display;
	serialCache = virtBindingsData->lastKeyEvent->serial;
	bzero((char *)translateCache, sizeof(XmTranslateCacheRec) * 256);
    }

    if (modifiers < 256) {
	if (translateCache[modifiers].keysym == (KeySym)0) {
	    XtTranslateKey(dpy, keycode, modifiers, modifiers_return,
			keysym_return);
	    FindVirtKey(virtBindingsData, keycode, modifiers, modifiers_return,
			keysym_return);
	    translateCache[modifiers].keysym = *keysym_return;
	    translateCache[modifiers].modifiers = *modifiers_return;
	}
	else {
	    *keysym_return = translateCache[modifiers].keysym;
	    *modifiers_return = translateCache[modifiers].modifiers;
	}
    }
    else {
	XtTranslateKey(dpy, keycode, modifiers, modifiers_return,
			keysym_return);
	FindVirtKey(virtBindingsData, keycode, modifiers, modifiers_return,
			keysym_return);
    }
/*
 * The code above fixes an Xt R4 performance problem in TMstate.c
 *
 * It can be removed and replaced by the code below as soon as Xt
 * is fixed to cache the return values to XtTranslateKeyCode(), and
 * stops calling it for every <Key> entry in the translation table.
 *
 *
    XmVirtualBindingsData virtBindingsData;

    GetVirtualBindingsData(dpy,virtBindingsData);

    XtTranslateKey(dpy, keycode, modifiers,
		modifiers_return, keysym_return);

    FindVirtKey(virtBindingsData, keycode, modifiers,
		modifiers_return, keysym_return);
 *
 *
 */
}

#ifdef _NO_PROTO
void _XmVirtualToActualKeysym(dpy, virtKeysym, actualKeysymRtn, modifiersRtn)
    Display     * dpy;
    KeySym	virtKeysym, *actualKeysymRtn;
    Modifiers	* modifiersRtn;
#else /* _NO_PROTO */
void _XmVirtualToActualKeysym(Display * dpy,
                              KeySym virtKeysym, 
                              KeySym * actualKeysymRtn, 
                              Modifiers * modifiersRtn)
#endif /* _NO_PROTO */
{
    Cardinal		i;
    XmKeyBinding	currBinding;
    XmKeyBinding        keyBindings;
    XmVirtualBindingsData v;

    GetVirtualBindingsData(dpy,v);
    keyBindings = v->bindings;

    for (i = 0; i < XtNumber(virtualKeysyms); i++)
	 {
	     if (virtualKeysyms[i].keysym == virtKeysym)
	       {
		   currBinding = (XmKeyBinding) &keyBindings[i];
		   
		   *actualKeysymRtn = currBinding->keysym;
		   *modifiersRtn = currBinding->modifiers;
		   return;
	       }
	 }
    *actualKeysymRtn = NoSymbol;
    *modifiersRtn = 0;
}
