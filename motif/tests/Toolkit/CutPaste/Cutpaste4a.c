#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)Cutpaste4a.c	3.4  91/01/11";
#endif /* lint */
#endif /* REV_INFO */
/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
*  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
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
#include <signal.h>

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>

#include <Xm/Xm.h>
#include <Xm/BulletinB.h>
#include <Xm/CutPaste.h>
#include <Xm/CutPasteP.h>
#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/List.h>
#include <Xm/MessageB.h>
#include <Xm/PushB.h>
#include <Xm/Text.h>
#include <Xm/PanedW.h>
#include <Xm/RowColumn.h>

#include "Cutpaste4.h"

#define NULL 0
#define TRUE 1

static int	big = 0;
static int	by_name = 0;
static int	private_id = 0;
static int	data_id = 0;

static char	*set_text = "initial text";
static char	*recopy_text = "recopy";

static char	*format_name = "STRING";
static int	length_text = 15;
static int	count_text = 0;
static int	max_length = 0;

static int	y_position = 5;
static int	t_position = 90;
static int	l_position = 5;

static Boolean	copy_started;
static Boolean	retr_started;

static XmString	cs;
static XmString	csv;

static void	cut_by_name_callback();
static void	drawbuttons();
static void	begin_copy();
static void	start_copy();
static void	copy_to();
static void	end_copy();
static void	start_copy_from();
static void	copy_from();
static void	end_copy_from();
static void	cancel_copy();
static void	undo_copy();
static void	copy_by_name();
static void	inq_next_format();
static void	inq_next_count();
static void	inq_next_length();
static void	list_pending();
static void	lock_unlock();
static void	display_copy_warning();
static void	display_retr_warning();
static void	ok_callback();

static void	begin_copy_test();
static void	start_copy_test();
static void	copy_by_name_test();
static void	undo_copy_test();
static void	cancel_copy_test();
static void	start_copy_from_test();
static void	mult_copy_from_test();
static void	inquire_test();

static XtCallbackRec	begin_copy_callback[3] = {begin_copy_test, NULL, NULL};
static XtCallbackRec	start_copy_callback[3] = {start_copy_test, NULL, NULL};
static XtCallbackRec	copy_by_name_callback[3] =
					{copy_by_name_test, NULL, NULL};
static XtCallbackRec	undo_copy_callback[3] = {undo_copy_test, NULL, NULL};
static XtCallbackRec	cancel_copy_callback[3] =
					{cancel_copy_test, NULL, NULL};
static XtCallbackRec	start_copy_from_callback[3] =
					{start_copy_from_test, NULL, NULL};
static XtCallbackRec	mult_copy_from_callback[3] =
					{mult_copy_from_test, NULL, NULL};
static XtCallbackRec	inquire_callback[3]  = {inquire_test, NULL, NULL};
static XtCallbackRec	copy_ok_callback[3]  = {ok_callback, NULL, NULL};
static XtCallbackRec	retrieve_ok_callback[3]  = {ok_callback, NULL, NULL};
static XtCallbackRec	dd_ok_callback[3]  = {ok_callback, NULL, NULL};
static XtCallbackRec	lock_ok_callback[3]  = {ok_callback, NULL, NULL};

static XrmOptionDescRec table[] =
{
    {"-v",	"value",		XrmoptionSepArg, NULL},
    {"-c",	"numwidgets",		XrmoptionSepArg, NULL},
    {"-sync",	"syncit",		XrmoptionNoArg,  "on"},
    {"-w",	"Text.wordWrap",	XrmoptionNoArg,  "on"},
    {"-nw",	"Text.wordWrap",	XrmoptionNoArg,  "off"},
};

static char		*value;
static Display		*display;
static unsigned long	itemid;
static int		locked;
static Window		window;
XmStringCharSet		charset = (XmStringCharSet) XmSTRING_DEFAULT_CHARSET;

static Widget		w,
			toplevel,
			panedwindow,
			workdialog, 
			setdialog,
			functdialog,
			worktext,
			settext,
			nametext,
			lengthtext,
			counttext,
			maxlength,
			pending,
			buttons[8];


void myQuit()
{
    fprintf(stdout,"Begin exiting ... please standby ... \n");
    fflush(stdout);
    exit(0);
}


int ErrorQuit(display, event)
     Display		*display;
     XErrorEvent	*event;
{
    myQuit();
}


main(argc, argv)
     unsigned int	argc;
     char		*argv[];
{
    Arg	args[20];
    int	nargs;
    int	i;

    signal(SIGINT, myQuit);

    copy_started = False;
    retr_started = False;

    XtToolkitInitialize();
    app_context = XtCreateApplicationContext();

    if((display = XtOpenDisplay(app_context, NULL, argv[0], "XMclient", NULL, 0,
				&argc, argv)) == NULL)
    {
        fprintf(stdout, "%s:  Can't open display\n", argv[0]);
        exit(1);
    }

/*
 * Top level shell
 */

    nargs = 0;
    XtSetArg(args[nargs], XmNallowShellResize, True); nargs++;
    toplevel = XtAppCreateShell(argv[0], NULL, applicationShellWidgetClass,
				display, args, nargs);

    XSetErrorHandler(ErrorQuit);

    panedwindow = XmCreatePanedWindow(toplevel, "panedwindow", NULL, 0);
    XtManageChild(panedwindow);

    cutpastetester(panedwindow);
    
    XtRealizeWidget(toplevel);

    window = XtWindow(panedwindow);

    XSynchronize(display, TRUE);

    Pause();
    Pause();
    Pause();
    Pause();

    XtAppMainLoop(app_context);
}

cutpastetester(parent)
     Widget	parent;
{
    Arg		args[20];
    Widget	widgetlist[50];
    WidgetList	widgetList = widgetlist;
    int		nargs;
    int		i,
    		widgetindex;

    locked = 0;
    big = 0;
    by_name = 0;
    widgetindex = 0;

/*
 * XmBulletinBoard: parent for pushbuttons, text and label widgets
 */

    nargs = 0;
    XtSetArg(args[nargs], XmNborderWidth, 1); nargs++;
    setdialog = XmCreateBulletinBoard(parent, "set", args, nargs);
    XtManageChild(setdialog);

/*
 * XmBulletinBoard: Used for various messages
 */

    nargs = 0;
    XtSetArg(args[nargs], XmNheight, 100); nargs++;
    XtSetArg(args[nargs], XmNborderWidth, 1); nargs++;
    XtSetArg(args[nargs], XmNresizePolicy, XmRESIZE_NONE); nargs++;
    workdialog = XmCreateBulletinBoard(parent, "work", args, nargs);
    XtManageChild(workdialog);

/*
 * XmText: Text to be cut
 */

    nargs = 0;
    XtSetArg(args[nargs], XmNx, t_position); nargs++;
    XtSetArg(args[nargs], XmNy, y_position); nargs++;
    XtSetArg(args[nargs], XmNcolumns, 15); nargs++;
    XtSetArg(args[nargs], XmNrows, 1); nargs++;
    settext = XmCreateText(setdialog, "name", args, nargs);

    widgetlist[widgetindex] = settext;
    widgetindex++;

/*
 * XmLabel: "to/from text"
 */

    cs = XmStringLtoRCreate("to/from text:", charset);

    nargs = 0;
    XtSetArg(args[nargs], XmNx, l_position); nargs++;
    XtSetArg(args[nargs], XmNy, y_position); nargs++;
    XtSetArg(args[nargs], XmNlabelString, cs); nargs++;
    w = XmCreateLabel(setdialog, "name", args, nargs);

    widgetlist[widgetindex] = w;
    widgetindex++;

    y_position += 40;

/*
 * XmText: Format name
 */

    nargs = 0;
    XtSetArg(args[nargs], XmNx, t_position); nargs++;
    XtSetArg(args[nargs], XmNy, y_position); nargs++;
    XtSetArg(args[nargs], XmNcolumns, 15); nargs++;
    XtSetArg(args[nargs], XmNrows, 1); nargs++;
    nametext = XmCreateText(setdialog, "name", args, nargs);

    widgetlist[widgetindex] = nametext;
    widgetindex++;

/*
 * XmLabel: "format name"
 */

    cs = XmStringLtoRCreate("format name:", charset);

    nargs = 0;
    XtSetArg(args[nargs], XmNx, l_position); nargs++;
    XtSetArg(args[nargs], XmNy, y_position); nargs++;
    XtSetArg(args[nargs], XmNlabelString, cs); nargs++;
    w = XmCreateLabel(setdialog, "name", args, nargs);

    widgetlist[widgetindex] = w;
    widgetindex++;

    y_position += 25;

/*
 * XmText: Length
 */

    nargs = 0;
    XtSetArg(args[nargs], XmNx, t_position); nargs++;
    XtSetArg(args[nargs], XmNy, y_position); nargs++;
    XtSetArg(args[nargs], XmNcolumns, 10); nargs++;
    XtSetArg(args[nargs], XmNrows, 1); nargs++;
    lengthtext = XmCreateText(setdialog, "name", args, nargs);

    widgetlist[widgetindex] = lengthtext;
    widgetindex++;

/*
 * XmLabel: "length"
 */

    cs = XmStringLtoRCreate("length:", charset);

    nargs = 0;
    XtSetArg(args[nargs], XmNx, l_position); nargs++;
    XtSetArg(args[nargs], XmNy, y_position); nargs++;
    XtSetArg(args[nargs], XmNlabelString, cs); nargs++;
    w = XmCreateLabel(setdialog, "name", args, nargs);

    widgetlist[widgetindex] = w;
    widgetindex++;

    y_position += 25;

/*
 * XmText: Count
 */

    nargs = 0;
    XtSetArg(args[nargs], XmNx, t_position); nargs++;
    XtSetArg(args[nargs], XmNy, y_position); nargs++;
    XtSetArg(args[nargs], XmNcolumns, 10); nargs++;
    XtSetArg(args[nargs], XmNrows, 1); nargs++;
    counttext = XmCreateText(setdialog, "name", args, nargs);

    widgetlist[widgetindex] = counttext;
    widgetindex++;

    cs = XmStringLtoRCreate("count:", charset);

/*
 * XmLabel: "count"
 */

    nargs = 0;
    XtSetArg(args[nargs], XmNx, l_position); nargs++;
    XtSetArg(args[nargs], XmNy, y_position); nargs++;
    XtSetArg(args[nargs], XmNlabelString, cs); nargs++;
    w = XmCreateLabel(setdialog, "name", args, nargs);

    widgetlist[widgetindex] = w;
    widgetindex++;

    y_position += 25;

/*
 * XmText: Max length
 */

    nargs = 0;
    XtSetArg(args[nargs], XmNx, t_position); nargs++;
    XtSetArg(args[nargs], XmNy, y_position); nargs++;
    XtSetArg(args[nargs], XmNcolumns, 10); nargs++;
    XtSetArg(args[nargs], XmNrows, 1); nargs++;
    maxlength = XmCreateText(setdialog, "name", args, nargs);

    widgetlist[widgetindex] = maxlength;
    widgetindex++;

/*
 * XmLabel: "max length"
 */

    cs = XmStringLtoRCreate("max length:", charset);

    nargs = 0;
    XtSetArg(args[nargs], XmNx, l_position); nargs++;
    XtSetArg(args[nargs], XmNy, y_position); nargs++;
    XtSetArg(args[nargs], XmNlabelString, cs); nargs++;
    w = XmCreateLabel(setdialog, "name", args, nargs);

    widgetlist[widgetindex] = w;
    widgetindex++;

    y_position += 25;

/*
 * XmLabel: "pending"
 */

    cs = XmStringLtoRCreate("pending:", charset);

    nargs = 0;
    XtSetArg(args[nargs], XmNx, l_position); nargs++;
    XtSetArg(args[nargs], XmNy, y_position); nargs++;
    XtSetArg(args[nargs], XmNlabelString, cs); nargs++;
    w = XmCreateLabel(setdialog, "name", args, nargs);

    widgetlist[widgetindex] = w;
    widgetindex++;

/*
 * Manage children
 */

    XtManageChildren(widgetList, widgetindex);

/*
 * Create pushbuttons
 */
 
    drawbuttons();

}


static void drawbuttons()
{
    Arg args[20];
    int nargs;
    int	button_number = 0;

    nargs = 0;
    XtSetArg(args[nargs], XmNwidth, 130); nargs++;
    XtSetArg(args[nargs], XmNx, 200); nargs++;
    XtSetArg(args[nargs], XmNy, 8); nargs++;
    XtSetArg(args[nargs], XmNborderWidth, 1); nargs++;
    XtSetArg(args[nargs], XmNdialogStyle, XmDIALOG_WORK_AREA); nargs++;
    functdialog = XmCreateForm(setdialog, "funct",  args, nargs);

/*
 * XmPushButton: Begin Copy Test
 */

    cs = XmStringLtoRCreate("Begin Copy", charset);

    nargs = 0;
    XtSetArg(args[nargs], XmNx, 0); nargs++;
    XtSetArg(args[nargs], XmNy, 0); nargs++;
    XtSetArg(args[nargs], XmNlabelString, cs); nargs++;
    XtSetArg(args[nargs], XmNactivateCallback, begin_copy_callback); nargs++;
    XtSetArg(args[nargs], XmNtopAttachment, XmATTACH_FORM); nargs++;
    XtSetArg(args[nargs], XmNtopOffset, 3); nargs++;
    XtSetArg(args[nargs], XmNleftAttachment, XmATTACH_FORM); nargs++;
    XtSetArg(args[nargs], XmNleftOffset, 5); nargs++;
    XtSetArg(args[nargs], XmNrightAttachment, XmATTACH_FORM); nargs++;
    XtSetArg(args[nargs], XmNrightOffset, 5); nargs++;
    buttons[button_number] = XmCreatePushButton(functdialog,"begin_copy_test",
						args, nargs);

    button_number++;

/*
 * XmPushButton: Start Copy Test
 */

    cs = XmStringLtoRCreate("Start Copy", charset);

    nargs = 0;
    XtSetArg(args[nargs], XmNx, 0); nargs++;
    XtSetArg(args[nargs], XmNy, 0); nargs++;
    XtSetArg(args[nargs], XmNlabelString, cs); nargs++;
    XtSetArg(args[nargs], XmNactivateCallback, start_copy_callback); nargs++;
    XtSetArg(args[nargs], XmNtopAttachment, XmATTACH_WIDGET); nargs++;
    XtSetArg(args[nargs], XmNtopWidget, buttons[button_number - 1]); nargs++;
    XtSetArg(args[nargs], XmNtopOffset, 3); nargs++;
    XtSetArg(args[nargs], XmNleftAttachment, XmATTACH_FORM); nargs++;
    XtSetArg(args[nargs], XmNleftOffset, 5); nargs++;
    XtSetArg(args[nargs], XmNrightAttachment, XmATTACH_FORM); nargs++;
    XtSetArg(args[nargs], XmNrightOffset, 5); nargs++;
    buttons[button_number] = XmCreatePushButton(functdialog,"start_copy_test",
						args, nargs);

    button_number++;

/*
 * XmPushButton: Copy By Name Test
 */

    cs = XmStringLtoRCreate("Copy By Name", charset);

    nargs = 0;
    XtSetArg(args[nargs], XmNx, 0); nargs++;
    XtSetArg(args[nargs], XmNy, 0); nargs++;
    XtSetArg(args[nargs], XmNlabelString, cs); nargs++;
    XtSetArg(args[nargs], XmNactivateCallback, copy_by_name_callback); nargs++;
    XtSetArg(args[nargs], XmNtopAttachment, XmATTACH_WIDGET); nargs++;
    XtSetArg(args[nargs], XmNtopWidget, buttons[button_number - 1]); nargs++;
    XtSetArg(args[nargs], XmNtopOffset, 3); nargs++;
    XtSetArg(args[nargs], XmNleftAttachment, XmATTACH_FORM); nargs++;
    XtSetArg(args[nargs], XmNleftOffset, 5); nargs++;
    XtSetArg(args[nargs], XmNrightAttachment, XmATTACH_FORM); nargs++;
    XtSetArg(args[nargs], XmNrightOffset, 5); nargs++;
    buttons[button_number] = XmCreatePushButton(functdialog,
						"copy_by_name_test", args,
						nargs);

    button_number++;

/*
 * XmPushButton: Undo Copy Test
 */

    cs = XmStringLtoRCreate("Undo Copy", charset);

    nargs = 0;
    XtSetArg(args[nargs], XmNx, 0); nargs++;
    XtSetArg(args[nargs], XmNy, 0); nargs++;
    XtSetArg(args[nargs], XmNlabelString, cs); nargs++;
    XtSetArg(args[nargs], XmNactivateCallback, undo_copy_callback); nargs++;
    XtSetArg(args[nargs], XmNtopAttachment, XmATTACH_WIDGET); nargs++;
    XtSetArg(args[nargs], XmNtopWidget, buttons[button_number - 1]); nargs++;
    XtSetArg(args[nargs], XmNtopOffset, 3); nargs++;
    XtSetArg(args[nargs], XmNleftAttachment, XmATTACH_FORM); nargs++;
    XtSetArg(args[nargs], XmNleftOffset, 5); nargs++;
    XtSetArg(args[nargs], XmNrightAttachment, XmATTACH_FORM); nargs++;
    XtSetArg(args[nargs], XmNrightOffset, 5); nargs++;
    buttons[button_number] = XmCreatePushButton(functdialog,"undo_copy_test",
						args, nargs);

    button_number++;

/*
 * XmPushButton: Cancel Copy Test
 */

    cs = XmStringLtoRCreate("Cancel Copy", charset);

    nargs = 0;
    XtSetArg(args[nargs], XmNx, 0); nargs++;
    XtSetArg(args[nargs], XmNy, 0); nargs++;
    XtSetArg(args[nargs], XmNlabelString, cs); nargs++;
    XtSetArg(args[nargs], XmNactivateCallback, cancel_copy_callback); nargs++;
    XtSetArg(args[nargs], XmNtopAttachment, XmATTACH_WIDGET); nargs++;
    XtSetArg(args[nargs], XmNtopWidget, buttons[button_number - 1]); nargs++;
    XtSetArg(args[nargs], XmNtopOffset, 3); nargs++;
    XtSetArg(args[nargs], XmNleftAttachment, XmATTACH_FORM); nargs++;
    XtSetArg(args[nargs], XmNleftOffset, 5); nargs++;
    XtSetArg(args[nargs], XmNrightAttachment, XmATTACH_FORM); nargs++;
    XtSetArg(args[nargs], XmNrightOffset, 5); nargs++;
    buttons[button_number] = XmCreatePushButton(functdialog,"cancel_copy_test",
						args, nargs);

    button_number++;

/*
 * XmPushButton: Retrieve Test
 */

    cs = XmStringLtoRCreate("Retrieve", charset);

    nargs = 0;
    XtSetArg(args[nargs], XmNx, 0); nargs++;
    XtSetArg(args[nargs], XmNy, 0); nargs++;
    XtSetArg(args[nargs], XmNlabelString, cs); nargs++;
    XtSetArg(args[nargs], XmNactivateCallback, start_copy_from_callback);
    nargs++;
    XtSetArg(args[nargs], XmNtopAttachment, XmATTACH_WIDGET); nargs++;
    XtSetArg(args[nargs], XmNtopWidget, buttons[button_number - 1]); nargs++;
    XtSetArg(args[nargs], XmNtopOffset, 15); nargs++;
    XtSetArg(args[nargs], XmNleftAttachment, XmATTACH_FORM); nargs++;
    XtSetArg(args[nargs], XmNleftOffset, 5); nargs++;
    XtSetArg(args[nargs], XmNrightAttachment, XmATTACH_FORM); nargs++;
    XtSetArg(args[nargs], XmNrightOffset, 5); nargs++;
    buttons[button_number] = XmCreatePushButton(functdialog,"copy_from_test",
						args, nargs);

    button_number++;

/*
 * XmPushButton: Multiple Retrieve Test
 */

    cs = XmStringLtoRCreate("Multiple Retrieve", charset);

    nargs = 0;
    XtSetArg(args[nargs], XmNx, 0); nargs++;
    XtSetArg(args[nargs], XmNy, 0); nargs++;
    XtSetArg(args[nargs], XmNlabelString, cs); nargs++;
    XtSetArg(args[nargs], XmNactivateCallback, mult_copy_from_callback);
    nargs++;
    XtSetArg(args[nargs], XmNtopAttachment, XmATTACH_WIDGET); nargs++;
    XtSetArg(args[nargs], XmNtopWidget, buttons[button_number - 1]); nargs++;
    XtSetArg(args[nargs], XmNtopOffset, 3); nargs++;
    XtSetArg(args[nargs], XmNleftAttachment, XmATTACH_FORM); nargs++;
    XtSetArg(args[nargs], XmNleftOffset, 5); nargs++;
    XtSetArg(args[nargs], XmNrightAttachment, XmATTACH_FORM); nargs++;
    XtSetArg(args[nargs], XmNrightOffset, 5); nargs++;
    buttons[button_number] = XmCreatePushButton(functdialog,
						"mult_copy_from_test", args,
						nargs);

    button_number++;

/*
 * XmPushButton: Inquire Test
 */

    cs = XmStringLtoRCreate("Inquire Status", charset);


    nargs = 0;
    XtSetArg(args[nargs], XmNx, 0); nargs++;
    XtSetArg(args[nargs], XmNy, 0); nargs++;
    XtSetArg(args[nargs], XmNlabelString, cs); nargs++;
    XtSetArg(args[nargs], XmNactivateCallback, inquire_callback); nargs++;
    XtSetArg(args[nargs], XmNtopAttachment, XmATTACH_WIDGET); nargs++;
    XtSetArg(args[nargs], XmNtopWidget, buttons[button_number - 1]); nargs++;
    XtSetArg(args[nargs], XmNtopOffset, 15); nargs++;
    XtSetArg(args[nargs], XmNleftAttachment, XmATTACH_FORM); nargs++;
    XtSetArg(args[nargs], XmNleftOffset, 5); nargs++;
    XtSetArg(args[nargs], XmNrightAttachment, XmATTACH_FORM); nargs++;
    XtSetArg(args[nargs], XmNrightOffset, 5); nargs++;
    buttons[button_number] = XmCreatePushButton(functdialog,"inquire_test",
						args, nargs);

    button_number++;

    XtManageChildren(buttons, button_number);

    XtManageChild(functdialog);
}

/*
 * Test copy to clipboard using XmClipboardBeginCopy
 */

static void begin_copy_test(widget, client_data, call_data)
     Widget	*widget;
     caddr_t	client_data;
     caddr_t	call_data;
{
    begin_copy(widget, client_data, call_data);
    copy_to(widget, client_data, call_data);
    end_copy(widget, client_data, call_data);
    
}

/*
 * Test copy to clipboard using XmClipboardStartCopy
 */

static void start_copy_test(widget, client_data, call_data)
     Widget	*widget;
     caddr_t	client_data;
     caddr_t	call_data;
{
    by_name = 1;
    
    start_copy(widget, client_data, call_data);
    copy_to(widget, client_data, call_data);
    end_copy(widget, client_data, call_data);
    
    by_name = 0;
    
}

/*
 * Test copy by name
 */

static void copy_by_name_test(widget, client_data, call_data)
     Widget	*widget;
     caddr_t	client_data;
     caddr_t	call_data;
{
    lock_unlock();

    copy_by_name(widget, client_data, call_data);

    lock_unlock();
}

/*
 * Test undo copy
 */

static void undo_copy_test(widget, client_data, call_data)
     Widget	*widget;
     caddr_t	client_data;
     caddr_t	call_data;
{
    big = 1;

    undo_copy(widget, client_data, call_data);

    big = 0;
}

/*
 * Test cancel copy
 */

static void cancel_copy_test(widget, client_data, call_data)
     Widget	*widget;
     caddr_t	client_data;
     caddr_t	call_data;
{
    set_text = XmTextGetString(settext);
    
    start_copy(widget, client_data, call_data);
    copy_to(widget, client_data, call_data);
    cancel_copy(widget, client_data, call_data);
}

/*
 * Test copy from clipboard
 */

static void start_copy_from_test(widget, client_data, call_data)
     Widget	*widget;
     caddr_t	client_data;
     caddr_t	call_data;
{
    start_copy_from(widget, client_data, call_data);
    copy_from(widget, client_data, call_data);
    end_copy_from(widget, client_data, call_data);
}

/*
 * Test multiple copy from clipboard
 */

static void mult_copy_from_test(widget, client_data, call_data)
     Widget	*widget;
     caddr_t	client_data;
     caddr_t	call_data;
{
    start_copy(widget, client_data, call_data);
    copy_to(widget, client_data, call_data);
    end_copy(widget, client_data, call_data);
    
    start_copy_from(widget, client_data, call_data);
    copy_from(widget, client_data, call_data);
    copy_from(widget, client_data, call_data);
    end_copy_from(widget, client_data, call_data);
}

/*
 * Test inquiry routines
 */

static void inquire_test(widget, client_data, call_data)
     Widget	*widget;
     caddr_t	client_data;
     caddr_t	call_data;
{
    list_pending(widget, client_data, call_data);
    inq_next_count(widget, client_data, call_data);
    inq_next_length(widget, client_data, call_data);
    inq_next_format(widget, client_data, call_data);
}


static void begin_copy(widget, client_data, call_data)
Widget *widget;
caddr_t client_data;
caddr_t call_data;
{
    XmString set_cs;
    int status;

    set_cs = XmStringLtoRCreate("Clipboard6", charset);

    status = XmClipboardBeginCopy(display, window, set_cs, workdialog,
				  cut_by_name_callback, &itemid);

    if(status == ClipboardLocked) 
    { 
    	display_lock_message();
    	return;
    }
    copy_started = True;
}


static void start_copy(widget, client_data, call_data)
Widget *widget;
caddr_t client_data;
caddr_t call_data;
{
    XmString set_cs;
    int status;
    XmAnyCallbackStruct *cb = (XmAnyCallbackStruct *)call_data;
    Time timestamp = cb->event->xbutton.time;

    set_cs = XmStringCreateLtoR(set_text, charset);

    status = XmClipboardStartCopy(display, window, set_cs, timestamp,
				  workdialog, cut_by_name_callback, &itemid);

    if(status == ClipboardLocked) 
    { 
    	display_lock_message();
    	return;
    }
    copy_started = True;
}


static void copy_to(widget, client_data, call_data)
Widget *widget;
caddr_t client_data;
caddr_t call_data;
{
    char *set_char, *priv_char, *length_char;
    int dummy;
    char *buffer, data_char[20];
    int status, set_length, buffer_size;
    int i;

    if(!copy_started) {
         display_copy_warning();
	 return;
    }

    if(big)
    {
	buffer_size = 263000;
    }else{
        buffer_size = 500;
    }

    if(!by_name)
    {
	buffer = XtMalloc(buffer_size);
	for(i=0; i<buffer_size; i++) buffer[i]='a';
	buffer[buffer_size-2] = 'b';
	buffer[buffer_size-1] = '\0';
    }
    if(big)
    {
        set_char = buffer;
        set_length = buffer_size;
    }else{
        if(by_name) 
        {
	    set_length = length_text;
        }else{
    	set_char = set_text;
    	set_length = strlen(set_char);
        }
    }

    if(by_name) set_char = 0;

    status = XmClipboardCopy(display, window, itemid, format_name, set_char, 
			     set_length, private_id, &data_id);

    if(status == ClipboardLocked) 
    { 
    	display_lock_message();
    	return;
    }
}


static void end_copy(widget, client_data, call_data)
Widget *widget;
caddr_t client_data;
caddr_t call_data;
{
    int status;

    if(!copy_started) {
         display_copy_warning();
	 return;
    }
    status = XmClipboardEndCopy(display, window, itemid);

    if(status == ClipboardLocked) 
    { 
    	display_lock_message();
    	return;
    }
    copy_started = False;
}


static void start_copy_from(widget, client_data, call_data)
Widget *widget;
caddr_t client_data;
caddr_t call_data;
{
    int status;
    XmAnyCallbackStruct *cb = (XmAnyCallbackStruct *)call_data;
    Time timestamp = cb->event->xbutton.time;

    status = XmClipboardStartRetrieve(display, window, timestamp);

    if(status == ClipboardLocked) 
    { 
    	display_lock_message();
    	return;
    }

    retr_started = True;
}


static void copy_from(widget, client_data, call_data)
Widget *widget;
caddr_t client_data;
caddr_t call_data;
{
    char *set_char, *name_char;
    char priv_char[20];
    int status, buffer_size;
    int copy_length;

    if(big) 
	buffer_size = 263000;
    else
	buffer_size = 12;

    set_char = XtMalloc(buffer_size + 1);

    status = XmClipboardRetrieve(display, window, format_name, set_char,
				 buffer_size, &copy_length, &private_id);

    if(status == ClipboardLocked) 
    { 
	XtFree(set_char);
	display_lock_message();
	return;
    }

    if(buffer_size > 20 && copy_length > 20) 
    {
	set_char[20] = '\0';
    }else{
	if(copy_length < buffer_size)
	{
	    set_char[copy_length] = '\0';
	}else{
	    set_char[buffer_size] = '\0';
	}
    }
    set_text = set_char;
}


static void end_copy_from(widget, client_data, call_data)
Widget *widget;
caddr_t client_data;
caddr_t call_data;
{
    int status;


    if(!retr_started) {
         display_retr_warning();
	 return;
    }

    status = XmClipboardEndRetrieve(display, window);

    if(status == ClipboardLocked) 
    { 
    	display_lock_message();
    	return;
    }
    retr_started = False;
}


static void cancel_copy(widget, client_data, call_data)
Widget *widget;
caddr_t client_data;
caddr_t call_data;
{

    if(!copy_started) {
         display_copy_warning();
	 return;
    }

    XmClipboardCancelCopy(display, window, itemid);

    copy_started = False;
}


static void copy_by_name(widget, client_data, call_data)
Widget *widget;
caddr_t client_data;
caddr_t call_data;
{
    char *set_char, *priv_char, *data_char;
    int dummy;
    int status;

    set_char = recopy_text;
    
    if(strlen(set_char) == 0) set_char = 0;
    status = XmClipboardCopyByName(display, window, data_id, set_char,
				   strlen(set_char), private_id);

    if(status == ClipboardLocked) 
    { 
    	display_lock_message();
    	return;
    }
}


static void undo_copy(widget, client_data, call_data)
Widget *widget;
caddr_t client_data;
caddr_t call_data;
{
    int status;

    status = XmClipboardUndoCopy(display, window);

    if(status == ClipboardLocked) 
    { 
    	display_lock_message();
    	return;
    }
}


static void inq_next_format(widget, client_data, call_data)
Widget *widget;
caddr_t client_data;
caddr_t call_data;
{
    char *n_char, buffer[100];
    int bufferlength, outlength;
    int n, dummy;
    int status;

    bufferlength = 100;

    status = XmClipboardInquireFormat(display, window, count_text, buffer,
				      bufferlength, &outlength);

    if(status == ClipboardLocked) 
    { 
    	display_lock_message();
    	return;
    }

    buffer[outlength] = '\0';

    XmTextSetString(nametext, buffer);
}


static void inq_next_length(widget, client_data, call_data)
Widget *widget;
caddr_t client_data;
caddr_t call_data;
{
    char length_char[20];
    int length;
    int status;

    status = XmClipboardInquireLength(display, window, format_name, &length);

    if(status == ClipboardLocked) 
    { 
    	display_lock_message();
    	return;
    }

    sprintf(length_char, "%d", length);
    XmTextSetString(lengthtext, length_char);

}


static void inq_next_count(widget, client_data, call_data)
Widget *widget;
caddr_t client_data;
caddr_t call_data;
{
    char count_char[20];
    int status;

    status = XmClipboardInquireCount(display, window, &count_text,
				     &max_length);

    if(status == ClipboardLocked) 
    { 
    	display_lock_message();
    	return;
    }

    sprintf(count_char, "%d", count_text);
    XmTextSetString(counttext, count_char);
    sprintf(count_char, "%d", max_length);
    XmTextSetString(maxlength, count_char);
}


static void list_pending(widget, client_data, call_data)
Widget *widget;
caddr_t client_data;
caddr_t call_data;
{
    Arg args[10];
    int nargs;
    int count, i;
    int status;
    XmClipboardPendingList list, listptr;
    char *charptrlist[20];
    char charstorage[500];
    char *ptr = charstorage, *tmp;

    status = XmClipboardInquirePendingItems(display, window, format_name,
					    &list, &count);

    if(status == ClipboardLocked) 
    { 
    	display_lock_message();
    	return;
    }

    listptr = list;

    for(i = 0; i < count; i++)
    {
    	charptrlist[i] = ptr;
        sprintf(ptr, "%d", listptr->DataId);
    	tmp = ptr;
    	ptr = tmp + strlen(ptr);
        sprintf(ptr, "   %d", listptr->PrivateId);
    	tmp = ptr;
    	ptr = tmp + strlen(ptr);

	charptrlist[i] = (char *)XmStringCreateLtoR(charptrlist[i], charset);

    	listptr = listptr + 1;

    }

    if(count > 0)
    {
        nargs = 0;
	XtSetArg(args[nargs], XmNx, t_position); nargs++;
	XtSetArg(args[nargs], XmNy, y_position); nargs++;
        XtSetArg(args[nargs], XmNvisibleItemCount, count); nargs++;
        XtSetArg(args[nargs], XmNitemCount, count); nargs++;
        XtSetArg(args[nargs], XmNitems, charptrlist); nargs++;
	pending = XmCreateList(setdialog, "name", args, nargs);
	XtManageChild(pending);
    }
}


static void lock_unlock(widget, client_data, call_data)
Widget *widget;
caddr_t client_data;
caddr_t call_data;
{
    int status;

    if(locked == 1)
    {
    	locked = 0;
    	status = XmClipboardUnlock(display, window, 0);

	if(status == ClipboardLocked) 
	{ 
	    display_lock_message();
    	    locked = 1;
	}
    }else{
    	locked = 1;
    	status = XmClipboardLock(display, window);

	if(status == ClipboardLocked) 
	{ 
	    display_lock_message();
            locked = 0;
	}
    }
}


static void ok_callback(widget, client_data, call_data)
Widget *widget;
caddr_t client_data;
caddr_t call_data;
{
	XtUnmanageChild(widget);
}


display_lock_message()
{
    XmString cs;
    Arg args[5];
    int nargs;
    Widget children[2];

    cs = XmStringLtoRCreate("Clipboard Locked, try again later", charset);

    nargs = 0;
    XtSetArg(args[nargs], XmNmessageString, cs); nargs++;
    XtSetArg(args[nargs], XmNokCallback, lock_ok_callback); nargs++;
    w = XmCreateMessageBox(workdialog, "locked", args, nargs);

    children[0] = XmMessageBoxGetChild(w, XmDIALOG_CANCEL_BUTTON);
    children[1] = XmMessageBoxGetChild(w, XmDIALOG_HELP_BUTTON);

    XtUnmanageChildren(children, 2);

    XtManageChild(w);

}


static void display_copy_warning()
{
    XmString cs;
    Arg args[5];
    int nargs;
    Widget children[2];

    cs = XmStringLtoRCreate("Warning: XmStartCopy or XmBeginCopy must be called first", charset);

    nargs = 0;
    XtSetArg(args[nargs], XmNmessageString, cs); nargs++;
    XtSetArg(args[nargs], XmNokCallback, copy_ok_callback); nargs++;
    w = XmCreateMessageBox(workdialog, "locked", args, nargs);

    children[0] = XmMessageBoxGetChild(w, XmDIALOG_CANCEL_BUTTON);
    children[1] = XmMessageBoxGetChild(w, XmDIALOG_HELP_BUTTON);

    XtUnmanageChildren(children, 2);

    XtManageChild(w);
}


static void display_retr_warning()
{
    XmString cs;
    Arg args[5];
    int nargs;
    Widget children[2];

    cs = XmStringLtoRCreate("Warning: XmStartRetrieve must be called first",
			    charset);
    nargs = 0;
    XtSetArg(args[nargs], XmNmessageString, cs); nargs++;
    XtSetArg(args[nargs], XmNautoUnmanage, cs); nargs++;
    XtSetArg(args[nargs], XmNokCallback, retrieve_ok_callback); nargs++;
    w = XmCreateMessageBox(workdialog, "locked", args, nargs);

    children[0] = XmMessageBoxGetChild(w, XmDIALOG_CANCEL_BUTTON);
    children[1] = XmMessageBoxGetChild(w, XmDIALOG_HELP_BUTTON);

    XtUnmanageChildren(children, 2);

    XtManageChild(w);
}


static void cut_by_name_callback(widget, dataid, privateid, reason)
Widget widget;
unsigned long *dataid;
int *privateid;
int *reason;
{
    XmString cs;
    char data_char[20];
    char private_char[20];
    char *set_char;
    int status, buffer_size;
    Arg args[10];
    int nargs;
    Widget children[2];
    int i;

    set_char = recopy_text;
    
    if(*reason == XmCR_CLIPBOARD_DATA_REQUEST)
    {
	if(big) 
	{
	    buffer_size = 263000;
	    set_char = XtMalloc(buffer_size);
	    for(i=0; i<buffer_size; i++) set_char[i]='a';
	    set_char[buffer_size-2] = 'b';
	    set_char[buffer_size-1] = '\0';
	}else{
	    buffer_size = strlen(set_char);
	    if(buffer_size == 0) set_char = 0;
	}

	status = XmClipboardCopyByName(display, window, *dataid,
				       set_char, buffer_size, *privateid);
    }

    if(*reason == XmCR_CLIPBOARD_DATA_DELETE)
    {

    	cs = XmStringLtoRCreate("Data Delete Message Recieved", charset);
   
	nargs = 0;
        XtSetArg(args[nargs], XmNmessageString, cs); nargs++;
        XtSetArg(args[nargs], XmNokCallback, dd_ok_callback); nargs++;
    	w = XmCreateMessageBox(workdialog, "message", args, nargs);
    	children[0] = XmMessageBoxGetChild(w, XmDIALOG_CANCEL_BUTTON);
    	children[1] = XmMessageBoxGetChild(w, XmDIALOG_HELP_BUTTON);

    	XtUnmanageChildren(children, 2);

        XtManageChild(w);
    }
}


static void  Pause()
{
    ContinuePopup = CreateContinuePopup();

    XtPopup(ContinuePopup, XtGrabNone);

    wait = True;
    while (wait)
        FlushEvents();
    Pause_cnt++;
}


static Widget CreateContinuePopup()
{
    Widget        popup;
    Widget        rowcol;
    Widget        message;
    Widget        exit_button, continue_button;
    Arg           args[MAX_ARGS];
    register int  n;
    XmString	  message_field, continue_field, exit_field;

    /*  create popup shell  */
    n = 0;
    XtSetArg(args[n], XtNgeometry, "+550+0");  n++;

    popup = XtCreatePopupShell("Next Event", topLevelShellWidgetClass,
                               toplevel, args, n);

    /*  create a frame for the pushbutton  */
    n = 0;
    XtSetArg(args[n], XmNmarginWidth,  12);  n++;
    XtSetArg(args[n], XmNmarginHeight, 12);  n++;
    rowcol = XmCreateRowColumn(popup, "frame", args, n);
    XtManageChild(rowcol);

    /*  create a Message Box and Control Continue and Exit */
    n = 0;
    message_field = XmStringCreateLtoR(Pause_events[Pause_cnt], XmSTRING_DEFAULT_CHARSET);
    continue_field = XmStringCreateLtoR("Continue", XmSTRING_DEFAULT_CHARSET);
    exit_field = XmStringCreateLtoR("Exit", XmSTRING_DEFAULT_CHARSET);
    XtSetArg(args[n], XmNmessageString, message_field); n++;
    XtSetArg(args[n], XmNokLabelString, continue_field); n++;
    XtSetArg(args[n], XmNcancelLabelString, exit_field); n++;
    message = XmCreateMessageBox(rowcol, "msgbox", args, n);
    XtManageChild(message);
    continue_button = XmMessageBoxGetChild(message, XmDIALOG_OK_BUTTON);
    exit_button = XmMessageBoxGetChild(message, XmDIALOG_CANCEL_BUTTON);
    XmStringFree(continue_field);
    XmStringFree(message_field);
    XmStringFree(exit_field);
    XtAddCallback(continue_button, XmNactivateCallback, ContinueCB, popup);
    XtAddCallback(exit_button, XmNactivateCallback, QuitCB, popup);

    return(popup);

}


static void  FlushEvents()
{
    XEvent event;

    while (XtAppPending(app_context)) {
        XtAppNextEvent(app_context, &event);
        XtDispatchEvent(&event);
    }
}


static XtCallbackProc  ContinueCB(w, client_data, call_data)
    Widget   w;
    caddr_t  client_data;
    caddr_t  call_data;
{
    wait = False;
    XtPopdown((Widget)client_data);
}


static XtCallbackProc  QuitCB(w, client_data, call_data)
    Widget   w;
    caddr_t  client_data;
    caddr_t  call_data;

{
	printf("Exiting...Please Standby...\n");
	exit(0);
}
