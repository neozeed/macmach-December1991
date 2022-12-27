#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)widgets.h	1.8 90/08/14";
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
 **   File:     @(#)widgets.h	1.8 - 90/08/14
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

#define ARROW_BUTTON 0
#define BULLETIN_BOARD 1
#define BULLETIN_BOARD_DIALOG 2
#define CASCADE_BUTTON 3
#define CASCADE_BUTTON_GADGET 4
#define COMMAND 5
#define DIALOG_SHELL 6
#define DRAWING_AREA 7
#define DRAWN_BUTTON 8
#define ERROR_DIALOG 9
#define FILE_SELECTION_BOX 10
#define FILE_SELECTION_DIALOG 11
#define FORM 12
#define FORM_DIALOG 13
#define FRAME 14
#define INFORMATION_DIALOG 15
#define LABEL 16
#define LABEL_GADGET 17
#define LIST 18
#define MENU_BAR 19
#define MENU_SHELL 20
#define MESSAGE_BOX 21
#define MESSAGE_DIALOG 22
#define OPTION_MENU 23
#define PANED_WINDOW 24
#define POPUP_MENU 25
#define PROMPT_DIALOG 26
#define PULLDOWN_MENU 27
#define PUSH_BUTTON 28
#define PUSH_BUTTON_GADGET 29
#define QUESTION_DIALOG 30
#define RADIO_BOX 31
#define ROW_COLUMN 32
#define SCALE_WIDGET 33
#define SCROLLED_LIST 34
#define SCROLL_BAR 35
#define SCROLLED_TEXT 36
#define SCROLLED_WINDOW 37
#define SELECTION_BOX 38
#define SELECTION_DIALOG 39
#define SEPARATOR 40
#define SEPARATOR_GADGET 41
#define TEXT 42
#define TOGGLE_BUTTON 43
#define TOGGLE_BUTTON_GADGET 44
#define WARNING_DIALOG 45
#define WORKING_DIALOG 46

typedef struct _widgetlist {
  char name [50];
  int  item;
  char create_string [50];
  char uil_string [50];   /* the uil create routine, if different than create */
}WIDGET_LIST;

#ifdef MAIN
static WIDGET_LIST 
  widgetlist [] = {
"Arrow Button", ARROW_BUTTON, "XmCreateArrowButton", "NULL",
"Bulletin Board", BULLETIN_BOARD, "XmCreateBulletinBoard", "NULL",
"Command", COMMAND, "XmCreateCommand", "NULL",
"Drawing Area", DRAWING_AREA, "XmCreateDrawingArea", "NULL",
"Drawn Button", DRAWN_BUTTON, "XmCreateDrawnButton", "NULL",
"File Selection Box", FILE_SELECTION_BOX, "XmCreateFileSelectionBox", "XmFileSelection",
"Form", FORM, "XmCreateForm", "NULL",
"Frame", FRAME, "XmCreateFrame", "NULL",
"Label", LABEL, "XmCreateLabel", "NULL",
"Label Gadget", LABEL_GADGET, "XmCreateLabelGadget", "NULL",
"List", LIST, "XmCreateList", "NULL",
"Menu Bar", MENU_BAR, "XmCreateMenuBar", "XmMenuBar",
"Message Box", MESSAGE_BOX, "XmCreateMessageBox", "XmMessageBox", 
"Push Button", PUSH_BUTTON, "XmCreatePushButton", "NULL",
"Push Button Gadget", PUSH_BUTTON_GADGET, "XmCreatePushButtonGadget", "NULL", 
"Radio Box", RADIO_BOX, "XmCreateRadioBox", "XmRadioBox",
"Row Column", ROW_COLUMN, "XmCreateRowColumn", "XmRowColumn",
"Scale", SCALE_WIDGET, "XmCreateScale", "NULL",
"Scroll Bar", SCROLL_BAR, "XmCreateScrollBar", "NULL",
"Scrolled List", SCROLLED_LIST, "XmCreateScrolledList", "XmScrolledList",
"Scrolled Text", SCROLLED_TEXT, "XmCreateScrolledText", "XmScrolledText",
"Scrolled Window", SCROLLED_WINDOW,"XmCreateScrolledWindow","XmScrolledWindow",
"Selection Box", SELECTION_BOX, "XmCreateSelectionBox", "NULL",
"Separator", SEPARATOR, "XmCreateSeparator", "NULL",
"Separator Gadget", SEPARATOR_GADGET, "XmCreateSeparatorGadget", "NULL",
"Text", TEXT, "XmCreateText", "NULL",
"Toggle Button", TOGGLE_BUTTON, "XmCreateToggleButton", "NULL",
"Toggle Button Gadget", TOGGLE_BUTTON_GADGET, "XmCreateToggleButtonGadget", "NULL",
"END", 0};
#endif

/* these defines are the resource widget type */
#define SCALE 0
#define TOGGLE_BUT 1
#define TEXT_INPUT 2
#define PUSH_BUT 3
#define CPD_STRING 4
#define STRING 5
#define CALLBACKS 6
#define FONTS 7
#define COLOR 8

typedef struct _scale_types {
  char type [25];
  int minimum;
  int maximum;
}SCALE_TYPES;

#ifdef MAIN
static SCALE_TYPES scale_vals [] = {"Position", 0, 250,
				    "HorizontalPosition", 0, 250,
				    "VerticalPosition", 0, 250,
				    "Dimension", 0, 250,
				    "HorizontalDimension", 0, 250,
				    "VerticalDimension", 0, 250,
				    "Int", -20, 100,
				    "Short", -20, 50,
				    "END", 0, 0};

static char *toggle_types [] = {"Boolean",
				"END"};

static char *cpd_strings [] = {"XmString",
			       "XmStringTable",
			       "END"};

static char *strings [] = {"String",
			   "XmFontList",
			   "END"};

static char *callback_types [] = {"XtCallbackList",
				  "Callback",
				  "END"};

static char *font_types [] = {"XmFontList",
                              "END"};

static char *color_types [] = {"Pixel",
                              "END"};
#endif


#ifndef WIDGETS
extern Widget CreateCascade ();
extern Widget CreateDialog ();
extern Widget CreateDialogShell ();
extern Widget CreateList ();
extern Widget CreateFileSelection ();
extern Widget CreatePushButton ();
extern Widget CreateRadioBox ();
extern Widget CreateSeparator ();
extern Widget CreateToggle ();
extern Widget CreateLabel ();
extern Widget CreateColorLabel ();
extern Widget CreateText ();
extern Widget CreateTextInputField ();
extern Widget CreateMultilineText ();
extern Widget CreateScale ();
#endif


