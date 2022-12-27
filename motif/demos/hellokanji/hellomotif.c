#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "%W% %E%"
#endif /* lint */
#endif /* REV_INFO */
/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
*  (c) Copyright 1989 DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
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
* DEC is a registered trademark of Digital Equipment Corporation
* DIGITAL is a registered trademark of Digital Equipment Corporation
* X Window System is a trademark of the Massachusetts Institute of Technology
*
*******************************************************************************
******************************************************************************/
#include <stdio.h>

#include <Xm/Xm.h>          /* Motif Toolkit */
#include <Mrm/MrmPublic.h>   /* Mrm */

Display                 *display;
XtAppContext            app_context;
 
static MrmHierarchy	s_MrmHierarchy;		/* MRM database hierarch id */
static char		*vec[]={"hellomotif.uid"};
						/* MRM database file list   */
static MrmCode		class ;

static void helloworld_button_activate();

static MrmCount		regnum = 1 ;
static MrmRegisterArg	regvec[] = {
	{"helloworld_button_activate",(caddr_t)helloworld_button_activate}
	};

/*
 *  Main program
 */
int main(argc, argv)
int argc;
char **argv;
{
    int n;

     /*
     *  Declare the variables to contain the two widget ids
     */
    Widget toplevel, helloworldmain;
    Arg arglist[1] ;

    /*
     *  Initialize the MRM
     */


    MrmInitialize ();

    /*
     *  Initialize the toolkit.  This call returns the id of the "toplevel"
     *  widget.  The applications "main" widget must be the only child
     *  of this widget.
     */

    XtToolkitInitialize();

    app_context = XtCreateApplicationContext();
    display = XtOpenDisplay(app_context, NULL, argv[0], "helloworldclass",
                            NULL, 0, &argc, argv);
    if (display == NULL) {
        fprintf(stderr, "%s:  Can't open display\n", argv[0]);
        exit(1);
    }

    n = 0;
    XtSetArg(arglist[n], XmNallowShellResize, True);  n++;
    toplevel = XtAppCreateShell(argv[0], NULL, applicationShellWidgetClass,
                              display, arglist, n);

    /*
     *  Define the Mrm.hierarchy (only 1 file)
     */


    if (MrmOpenHierarchy (1,			    /* number of files	    */
			vec, 			    /* files     	    */
			NULL,			    /* os_ext_list (null)   */
			&s_MrmHierarchy)	    /* ptr to returned id   */
			!= MrmSUCCESS) {
	printf ("can't open hierarchy\n");
     }


    /*
     * 	Register our callback routines so that the resource manager can 
     * 	resolve them at widget-creation time.
     */


    if (MrmRegisterNames (regvec, regnum)
			!= MrmSUCCESS)
			    printf("can't register names\n");


    /*
     *  Call MRM to fetch and create the pushbutton and its container
     */


    if (MrmFetchWidget (s_MrmHierarchy,
			"helloworld_main",
			toplevel,
			&helloworldmain,
			&class)
			!= MrmSUCCESS)
			    printf("can't fetch interface\n");


    /*
     *  Make the toplevel widget "manage" the main window (or whatever the
     *  the uil defines as the topmost widget).  This will
     *  cause it to be "realized" when the toplevel widget is "realized"
     */


    XtManageChild(helloworldmain);
    
    /*
     *  Realize the toplevel widget.  This will cause the entire "managed"
     *  widget hierarchy to be displayed
     */

    XtRealizeWidget(toplevel);

    /*
     *  Loop and process events
     */


    XtAppMainLoop(app_context);

    /* UNREACHABLE */
    return (0);
}

static void helloworld_button_activate( widget, tag, callback_data )
	Widget	widget;
	char    *tag;
	XmAnyCallbackStruct *callback_data;
{
    Arg arglist[2];

    static int call_count = 0;

    XmString a, b, c;

    call_count += 1 ;
    switch ( call_count )
        {
        case 1:
	    a = XmStringSegmentCreate ("さようなら","JISX0208.1983-1",XmSTRING_DIRECTION_L_TO_R, TRUE);
	    b = XmStringSegmentCreate ("Good-bye!","JISX0208.1983-1",XmSTRING_DIRECTION_L_TO_R, FALSE);
            c = XmStringConcat (a,b);
            XmStringFree (a);
            XmStringFree (b);
            XtSetArg( arglist[0], XmNlabelString, c);
            XtSetArg( arglist[1], XmNx, 11 );  
            XtSetValues( widget, arglist, 2 );
	    XmStringFree (c);
            break ;
        case 2:
            exit(1);
            break ;
        }
}

