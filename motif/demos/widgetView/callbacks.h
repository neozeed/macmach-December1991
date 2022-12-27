#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)callbacks.h	1.4 90/08/01";
#endif /* lint */
#endif /* REV_INFO */
/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990 OPEN SOFTWARE FOUNDATION, INC.
*  ALL RIGHTS RESERVED
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
* Open Software Foundation is a trademark of The Open Software Foundation, Inc.
* OSF is a trademark of Open Software Foundation, Inc.
* OSF/Motif is a trademark of Open Software Foundation, Inc.
* Motif is a trademark of Open Software Foundation, Inc.
* X Window System is a trademark of the Massachusetts Institute of Technology
*
*******************************************************************************
******************************************************************************/

/****************************************************************************
 ****************************************************************************
 **
 **   File:     @(#)callbacks.h	1.4 - 90/08/01
 **
 **   Project:     Motif - widget examination program
 **
 **   Description: Program which shows resources of widgets
 **
 **   Author:      Pete Levine
 **
 **
 **
 **
 ****************************************************************************
 ****************************************************************************/
#ifndef CALLBACK
extern void Quit ();
extern XtCallbackProc MenuQuit ();
extern XtCallbackProc GenericCallback ();
extern XtCallbackProc TextScaleUpdate ();
extern XtCallbackProc ValueField ();
extern XtCallbackProc ApplyResourceCallback ();
extern XtCallbackProc CancelResource ();
extern XtCallbackProc NewFileOK ();
extern XtCallbackProc NewFile ();
extern XtCallbackProc OpenFileOK ();
extern XtCallbackProc OpenResourceFile ();
extern XtCallbackProc SingleSelectColor ();
extern XtCallbackProc NewColorOK ();
extern XtCallbackProc ColorUpdate ();
extern XtCallbackProc ColorLabelPressed ();
extern XtCallbackProc SaveResourceFile ();
extern XtCallbackProc SaveCFile ();
extern XtCallbackProc SaveUILFile ();
extern XtCallbackProc TextOKFunction ();
extern XtCallbackProc TextCancelFunction ();
extern XtCallbackProc TextClearFunction ();
extern XtCallbackProc WriteSourceCode ();
extern XtCallbackProc PrintToFile ();
extern XtCallbackProc PrintToFileOK ();
extern XtCallbackProc PrintToFileApply ();
extern XtCallbackProc PrintToPrinter ();
extern XtCallbackProc Enabled ();
extern XtCallbackProc GenerateMessageToggle ();
extern XtCallbackProc ManHelp ();
extern XtCallbackProc SourceHelp ();
extern XtCallbackProc ViewHelp ();
extern XtCallbackProc SelectObjects ();
extern XtCallbackProc SelectMethods ();
extern XtCallbackProc ErrorDialogAway ();
extern XtCallbackProc QuestionDialogAway ();
extern XtCallbackProc QuestionDialogOK ();
extern XtCallbackProc QuestionDialogSaveOK ();
#endif

