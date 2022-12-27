#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)VirtKeys.h	3.5 90/04/24";
#endif /* lint */
#endif /* REV_INFO */
/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
*  (c) Copyright 1990, HEWLETT-PACKARD COMPANY
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
*  (c) Copyright 1989, 1990, Open Software Foundation, Inc.  Unpublished - all
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
*  (c) Copyright 1989, 1990, Open Software Foundation, Inc.
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
#ifndef _VirtKeys_h
#define _VirtKeys_h

#include <Xm/Xm.h>

#define osfXK_BackSpace	0x1004FF08
#define osfXK_Insert	0x1004FF63
#define osfXK_Delete	0x1004FFFF
#define osfXK_Copy	0x1004FF02
#define osfXK_Cut	0x1004FF03
#define osfXK_Paste	0x1004FF04

#define osfXK_AddMode		0x1004FF31
#define osfXK_PrimaryPaste	0x1004FF32
#define osfXK_QuickPaste	0x1004FF33

#define osfXK_PageUp	0x1004FF41
#define osfXK_PageDown	0x1004FF42

#define osfXK_EndLine	0x1004FF57
#define osfXK_BeginLine	0x1004FF58

#define osfXK_Activate	0x1004FF44

#define osfXK_MenuBar	0x1004FF45

#define osfXK_Clear	0x1004FF0B
#define osfXK_Cancel	0x1004FF69
#define osfXK_Help	0x1004FF6A
#define osfXK_Menu	0x1004FF67
#define osfXK_Select	0x1004FF60
#define osfXK_Undo	0x1004FF65

#define osfXK_Left	0x1004FF51
#define osfXK_Up	0x1004FF52
#define osfXK_Right	0x1004FF53
#define osfXK_Down	0x1004FF54


#define XmVosfBackSpace		"osfBackSpace"
#define XmVosfInsert	"osfInsert"
#define XmVosfDelete	"osfDelete"
#define XmVosfCopy	"osfCopy"
#define XmVosfCut	"osfCut"
#define XmVosfPaste	"osfPaste"

#define XmVosfAddMode	"osfAddMode"
#define XmVosfPrimaryPaste	"osfPrimaryPaste"
#define XmVosfQuickPaste	"osfQuickPaste"

#define XmVosfPageUp	"osfPageUp"
#define XmVosfPageDown	"osfPageDown"

#define XmVosfEndLine	"osfEndLine"
#define XmVosfBeginLine	"osfBeginLine"

#define XmVosfActivate	"osfActivate"

#define XmVosfMenuBar	"osfMenuBar"

#define XmVosfClear	"osfClear"
#define XmVosfCancel	"osfCancel"
#define XmVosfHelp	"osfHelp"
#define XmVosfMenu	"osfMenu"
#define XmVosfSelect	"osfSelect"
#define XmVosfUndo	"osfUndo"

#define XmVosfLeft	"osfLeft"
#define XmVosfUp	"osfUp"
#define XmVosfRight	"osfRight"
#define XmVosfDown	"osfDown"

#ifdef _NO_PROTO
extern void _XmVirtKeysInitialize ();
#else /* _NO_PROTO */
extern void _XmVirtKeysInitialize (Widget shell);
#endif /* _NO_PROTO */

#endif /* _VirtKeys_h */
