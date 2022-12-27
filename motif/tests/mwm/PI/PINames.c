#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)PINames.c	3.3 91/01/11";
#endif /* lint */
#endif /* REV_INFO */
/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
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
#include	<X11/Xatom.h>

#include	<PITests.h> 

/* 
 * Data for summary
 */

#define NUM_TESTS		5
#define TEST_DESCRIP_LENGTH	54

struct TestInfo PIInfo[NUM_TESTS] = 
{
	{"Change the window name of a window (WM_NAME)          ",
		True,	NULL},
	{"Check for a ***** default name                        ",
		True,	NULL},
	{"Change the icon name of a window (WM_ICON_NAME)       ",
		True,	NULL},
	{"Change the window name of ***** (WM_NAME)             ",
		True,	NULL},
	{"Change the class name structure of a window (WM_CLASS)",
		True,	NULL},
};
int		PIInfo_cnt = 0;

/*
 * External declarations
 */

extern MrmHierarchy	s_MrmHierarchy;
extern MrmType		*class;

extern Widget	toplevel,
				widgetmain;

extern String	summary();

/*
 * Local declarations
 */

#define VALID_TEST1	1
#define VALID_TEST2	2
#define VALID_TEST3	3
#define FINISH		4

Widget	test_top,
		test_top2,
		test_box;

int		iteration = 1;


Atom			act_type,			new_type;
int				act_format,			new_format;
unsigned long	act_nitems,			new_nitems,
				act_bytes_after,	new_bytes_after;
unsigned char	*act_data,			*new_data;
Atom			test_atom;

Display			*common_dsp;
Window			label_wdw, top_wdw, normal_wdw;

XClientMessageEvent		ClientMsg_ev;
XUnmapEvent				Unmap_ev;

void continue_proc(w, tag, reason)
     Widget		w;
     int		*tag;
     unsigned long	*reason;
{ 	
    Arg		args[2];
    int		nargs;
    XmString	message;
    String	summary_msg;

    String	msg_valid_tst1 =
		"There should now be a label displayed in a\nwindow that describes the window's new name.\nThe window decorations should be full\nexcept the border and the resize handles.\n\nThere should be a second window that is solid black with\nfull border decorations and a title of \"*****\".\n\nThis window's decorations should be\nONLY a border with a title bar in it.\n\nPress the continue button to continue the test.";

    String	msg_valid_tst2 =
		"The label window should now be iconified and\nthe name of the icon should be \"PI Icon\".\n\nThe solid black window should now have a\ntitle of \"Second New Name\".\n\nThis window's decorations should remain unchanged.\n\nPress the continue button to continue the test.";

    String	msg_valid_tst3 =
		"The label window should have been deiconified\nand the label should describe the window's new name.\nThis time, the label window's decorations should\nbe ONLY a title bar.\n\nThe solid black window should remain unchanged.\n\nAgain, this window's decorations should remain unchanged.\n\nPress the continue button to continue the test.";

	String	new_winname = "New Window Name";
	String	look_at_name = "Window name should be \"New Window Name\".";
	String	new_iconname = "PI Icon";
	String	look_at_icon = "Icon name should be \"PI Icon\".";
	String	look_at_classname =
			"Window name should be \"NEWER Window Class Name\".";

	char	class_buf[80];			/* string for WM_CLASS prop */
	int		l_name, l_class;		/* temp length vars */

	String	new_class_name = "NEWER Window Class Name";
	String	new_class_class = "PINewClass";
	String	new_winname2 = "Second New Name";
	String	old_winname2 = "*****";
	char	*new_name;
	XClassHint	class_hints;
	XWMHints	wm_hints;

    if (iteration == VALID_TEST1)
    {
		iteration++;

/*
 * Create top level shell widget as parent for test_box widget
 */
		nargs = 0;
		XtSetArg(args[nargs], XtNallowShellResize, TRUE); nargs++;

		test_top =
		XtAppCreateShell("test_top",	/* application name */
			 "Test_top",				/* application class */
			 topLevelShellWidgetClass,	/* widget class	     */
			 XtDisplay(widgetmain),		/* display	     */
			 args, nargs);				/* arguments	     */

/*
 * Fetch test_box widget and change the label
 */
		message = XmStringCreateLtoR(look_at_name,
			XmSTRING_DEFAULT_CHARSET);
		nargs = 0;
		XtSetArg(args[nargs], XmNlabelString, message); nargs++;
				       
		test_box = NULL;
		MrmFetchWidgetOverride(
			s_MrmHierarchy,
			"test_box",
			test_top,
			NULL,
			args,
			nargs,
			&test_box,
			&class);
		XmStringFree(message);

		XtManageChild(test_box);
		XtRealizeWidget(test_top);

		common_dsp = XtDisplay(test_top);
		label_wdw = XtWindow(test_top);

		top_wdw = XtWindow(toplevel);

		/*
		 * Move the test_top shell window and the toplevel window
		 */
		XMoveWindow(common_dsp, label_wdw, 100, 100);
		XMoveWindow(common_dsp, top_wdw, 400, 100);

/*
 * Change the window name of a window (WM_NAME)
 */
		test_atom = XmInternAtom(common_dsp, "WM_NAME", False);
		XChangeProperty(common_dsp, label_wdw,
			test_atom, 
			XA_STRING, 8, PropModeReplace,
			(unsigned char *)new_winname, strlen(new_winname));

		XGetWindowProperty(common_dsp, label_wdw,
			test_atom,
			0, 20, False, AnyPropertyType,
			&new_type, &new_format, &new_nitems, &new_bytes_after,
			&new_data);
		PIInfo[PIInfo_cnt].actual_return =
			!strcmp(new_winname, new_data);	
		PIInfo_cnt++;

/*
 * Check for a ***** default name
 */
		normal_wdw =
			XCreateSimpleWindow(common_dsp, DefaultRootWindow(common_dsp),
			100,	/* x */
			300,	/* y */
			200,	/* width */
			50,		/* height */
			10,		/* border width */
			1,		/* border pixel */
		        BlackPixel(common_dsp, DefaultScreen(common_dsp))
					/* background pixel */
			);
		XMapWindow(common_dsp, normal_wdw);
		XSync(common_dsp);
		if (!XFetchName(common_dsp, normal_wdw, &new_name))
		{
			if (!XGetClassHint(common_dsp, normal_wdw, &class_hints))
			{
				/*
				 * If the name couldn't be fetched, and res_name
				 * had no value, it means that
				 * there was no name, so it's supposed to use '*****'
				 */
				PIInfo[PIInfo_cnt].actual_return =
					True;	
				PIInfo_cnt++;
			}
			else
			{
				/*
				 * It must have found a class hint, using res_name
				 */
				printf("normal_wdw classhints, res_name=%s=\n",
					class_hints.res_name);
				PIInfo[PIInfo_cnt].actual_return =
					!strcmp(old_winname2, class_hints.res_name);	
				PIInfo_cnt++;
				XFree(class_hints.res_name);
				XFree(class_hints.res_class);
			}
		}
		else
		{
			/*
			 * It must have found a fetched name, using name returned
			 */
			printf("new_name=%s=\n",
				new_name);
			PIInfo[PIInfo_cnt].actual_return =
				!strcmp(old_winname2, new_name);	
			PIInfo_cnt++;
			XFree(new_name);
		}

/*
 * Replace instruction message with "valid test 1" message
 */
		message = XmStringCreateLtoR(msg_valid_tst1,
			XmSTRING_DEFAULT_CHARSET);
		nargs = 0;
		XtSetArg(args[nargs], XmNmessageString, message); nargs++;
		XtSetValues(widgetmain, args, nargs);
		XmStringFree(message);
				       
    }
    else if (iteration == VALID_TEST2)
    {
		iteration++;

/*
 * Change the icon name of a window (WM_ICON_NAME)
 */
		test_atom = XmInternAtom(common_dsp, "WM_ICON_NAME", False);
		XChangeProperty(common_dsp, label_wdw,
			test_atom, 
			XA_STRING, 8, PropModeReplace,
			(unsigned char *)new_iconname, strlen(new_iconname));

		XGetWindowProperty(common_dsp, label_wdw,
			test_atom,
			0, 20, False, AnyPropertyType,
			&new_type, &new_format, &new_nitems, &new_bytes_after,
			&new_data);
		PIInfo[PIInfo_cnt].actual_return =
			!strcmp(new_iconname, new_data);	
		PIInfo_cnt++;

		ClientMsg_ev.type = ClientMessage;
		ClientMsg_ev.window = label_wdw;
		ClientMsg_ev.message_type =
			XmInternAtom(common_dsp, "WM_CHANGE_STATE", False);
		ClientMsg_ev.format = 32;
		ClientMsg_ev.data.l[0] = IconicState;

		XSendEvent(common_dsp, DefaultRootWindow(common_dsp), False,
			(SubstructureNotifyMask|SubstructureRedirectMask), &ClientMsg_ev);

/*
 * Change the test_box label
 */
		message = XmStringCreateLtoR(look_at_icon,
			XmSTRING_DEFAULT_CHARSET);
		nargs = 0;
		XtSetArg(args[nargs], XmNlabelString, message); nargs++;
		XtSetValues(test_box, args, nargs);
		XmStringFree(message);
				       
/*
 * Change the window name of ***** (WM_NAME)
 */
		test_atom = XmInternAtom(common_dsp, "WM_NAME", False);
		XChangeProperty(common_dsp, normal_wdw,
			test_atom, 
			XA_STRING, 8, PropModeReplace,
			(unsigned char *)new_winname2, strlen(new_winname2));

		XGetWindowProperty(common_dsp, normal_wdw,
			test_atom,
			0, 20, False, AnyPropertyType,
			&new_type, &new_format, &new_nitems, &new_bytes_after,
			&new_data);
		PIInfo[PIInfo_cnt].actual_return =
			!strcmp(new_winname2, new_data);	
		PIInfo_cnt++;
		XSync(common_dsp);

/*
 * Replace the instruction message with "valid test 2" message
 */
		message = XmStringCreateLtoR(msg_valid_tst2,
			XmSTRING_DEFAULT_CHARSET);
		nargs = 0;
		XtSetArg(args[nargs], XmNmessageString, message); nargs++;
		XtSetValues(widgetmain, args, nargs);
		XmStringFree(message);

    }
    else if (iteration == VALID_TEST3)
    {
		iteration++;

/*
 * Change the class name structure of a window (WM_CLASS)
 */
		/* 
		 * put window from IconicState to WithdrawnState
		 */
		XUnmapWindow(common_dsp, label_wdw);

		Unmap_ev.type = UnmapNotify;
		Unmap_ev.event = DefaultRootWindow(common_dsp);
		Unmap_ev.window = label_wdw;
		Unmap_ev.from_configure = False;
		XSendEvent(common_dsp, DefaultRootWindow(common_dsp), False,
			(SubstructureNotifyMask|SubstructureRedirectMask), &Unmap_ev);

		/*
		 * delete the WM_NAME first so the class name kicks in
		 */
		test_atom = XmInternAtom(common_dsp, "WM_NAME", False);
		XDeleteProperty(common_dsp, label_wdw, test_atom);

		/*
		 * change class info
		 */
		l_name = strlen(new_class_name);
		l_class = strlen(new_class_class);
		strcpy(class_buf, new_class_name);
		strcpy(&class_buf[l_name+1], new_class_class);
		test_atom = XmInternAtom(common_dsp, "WM_CLASS", False);
		XChangeProperty(common_dsp, label_wdw,
			test_atom, 
			XA_STRING, 8, PropModeReplace,
			(unsigned char *)class_buf,
			l_name+l_class+2);
		
		/*
		 * revive from the WithdrawnState to NormalState
		 */
		wm_hints.flags = StateHint;
		wm_hints.initial_state = NormalState;
		XSetWMHints(common_dsp, label_wdw, &wm_hints);
		XMapWindow(common_dsp, label_wdw);
		XMoveWindow(common_dsp, label_wdw, 100, 100);

		XGetClassHint(common_dsp, label_wdw, &class_hints);
		PIInfo[PIInfo_cnt].actual_return =
			(!strcmp(new_class_name, class_hints.res_name) &&
			!strcmp(new_class_class, class_hints.res_class));	
		PIInfo_cnt++;

/*
 * Change the test_box label
 */
		message = XmStringCreateLtoR(look_at_classname,
			XmSTRING_DEFAULT_CHARSET);
		nargs = 0;
		XtSetArg(args[nargs], XmNlabelString, message); nargs++;
		XtSetValues(test_box, args, nargs);
		XmStringFree(message);

/*
 * Replace instruction message with "valid test 3" message
 */
		message = XmStringCreateLtoR(msg_valid_tst3,
			XmSTRING_DEFAULT_CHARSET);
		nargs = 0;
		XtSetArg(args[nargs], XmNmessageString, message); nargs++;
		XtSetValues(widgetmain, args, nargs);
		XmStringFree(message);

	}
    else if (iteration == FINISH)
    {
		iteration++;

/*
 * Replace the instruction message with test end message
 */

		summary_msg = summary(NUM_TESTS,
				      TEST_DESCRIP_LENGTH,
				      PIInfo);

		message = XmStringCreateLtoR(summary_msg,
			XmSTRING_DEFAULT_CHARSET);
		nargs = 0;
		XtSetArg(args[nargs], XmNmessageString, message); nargs++;
		XtSetValues(widgetmain, args, nargs);

		XtFree(summary_msg);
		XmStringFree(message);
    }
}
