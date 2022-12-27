#ifdef REV_INFO
#ifndef lint
static char SCCSID[] ="OSF/Motif: @(#)dogs.c	1.12 91/01/10";
#endif /* lint */
#endif /* REV_INFO */
/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
*  (c) Copyright 1990 Alastair Gourlay
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

/*****************************************************************************
*
*  dogs.c - Square & Dog widget demo source file.
*  
******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Mrm/MrmPublic.h>
#include <Square.h>
#include <Dog.h>

XtAppContext  app_context;

#define k_dog1_id 1
#define k_dog2_id 2
#define k_dog3_id 3
#define k_help_id 4

static void create_cb();
static void bark_cb();
static void tb_cb();
static void scale_cb();
static void help_cb();
static void exit_cb();

static MrmHierarchy mrm_id;
static char *mrm_vec[]={"dogs.uid"};
static MrmCode mrm_class;
static MRMRegisterArg mrm_names[] = {
        {"create_cb", (caddr_t)create_cb },
        {"bark_cb", (caddr_t)bark_cb },
        {"tb_cb", (caddr_t)tb_cb },
        {"scale_cb", (caddr_t)scale_cb },
        {"help_cb", (caddr_t)help_cb },
        {"exit_cb", (caddr_t)exit_cb }
};

static Widget dog1_id;
static Widget dog2_id;
static Widget dog3_id;
static Widget help_id;

main(argc, argv)
    int argc;
    char **argv;
{
    Widget shell;
    Display *display;
    Widget app_main = NULL;
    Arg args[3];

    MrmInitialize ();
    SquareMrmInitialize();
    DogMrmInitialize();

    XtToolkitInitialize();
    app_context = XtCreateApplicationContext();
    display = XtOpenDisplay(app_context, NULL, argv[0], "Dogs",
			NULL, 0, &argc, argv);
    
    if (display == NULL) {
	    fprintf(stderr, "%s:  Can't open display\n", argv[0]);
	    exit(1);
    }

    XtSetArg (args[0], XtNallowShellResize, True);
    XtSetArg (args[1], XtNminWidth, 620);
    XtSetArg (args[2], XtNminHeight, 370);
    shell = XtAppCreateShell(argv[0], NULL, applicationShellWidgetClass,
			  display, args, 3);

    if (MrmOpenHierarchy (1, mrm_vec, NULL, &mrm_id) != MrmSUCCESS) exit(0);
    MrmRegisterNames(mrm_names, XtNumber(mrm_names));
    MrmFetchWidget (mrm_id, "app_main", shell, &app_main, &mrm_class);
    XtManageChild(app_main);
    XtRealizeWidget(shell);
    XtAppMainLoop(app_context);
}

static void create_cb(w, id, reason)
    Widget w;
    int *id;
    unsigned long *reason;
{
    switch (*id) {
        case k_dog1_id: dog1_id = w; break;
        case k_dog2_id: dog2_id = w; break;
        case k_dog3_id: dog3_id = w; break;
        case k_help_id:
	    help_id = w;
	    XtUnmanageChild(XmMessageBoxGetChild(help_id,
				XmDIALOG_CANCEL_BUTTON));
	    XtUnmanageChild(XmMessageBoxGetChild(help_id,
				XmDIALOG_HELP_BUTTON));
	    break;
    }
}

static void bark_cb (w, volume, cb)
    Widget w;
    int *volume;
    XtPointer cb;
{
    XBell(XtDisplay(w), *volume);
}

static void tb_cb (w, tag, cb)
    Widget w;
    int *tag;
    XmToggleButtonCallbackStruct *cb;
{
    Arg args[1];
    Widget dog;

    switch (*tag) {
	case(1) : dog = dog1_id; break;
	case(2) : dog = dog2_id; break;	
	case(3) : dog = dog3_id; break;	
    }
    XtSetArg(args[0], SquareNmakeSquare, cb->set);
    XtSetValues(dog, args, 1);
}

static void scale_cb(w, tag, cb)
    Widget w;
    int *tag;
    XmScaleCallbackStruct *cb;
{
    Arg args[1];
    Widget dog;

    switch (*tag) {
	case(1) : dog = dog1_id; break;
	case(2) : dog = dog2_id; break;	
	case(3) : dog = dog3_id; break;	
    }
    XtSetArg(args[0], DogNwagTime, cb->value);
    XtSetValues(dog, args, 1);
}

static void help_cb (w, name, cb)
    Widget w;
    XmString name;
    XtPointer cb;
{
    Arg args[1];

    if (name == NULL) return;
    XtSetArg (args[0], XmNmessageString, name);
    XtSetValues(help_id, args, 1);
    XtManageChild(help_id);
}

static void exit_cb (w, name, cb)
    Widget w;
    XmString name;
    XtPointer cb;
{
    exit(0);
}

