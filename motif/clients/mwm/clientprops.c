#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: 90/04/24 @(#)clientprops.c	3.5";
#endif /* lint */
#endif /* REV_INFO */
 /******************************************************************************
 *******************************************************************************
 *
 * (c) Copyright 1989, 1990, OPEN SOFTWARE FOUNDATION, INC.
 * (c) Copyright 1987, 1988, 1989, 1990 HEWLETT-PACKARD COMPANY
 * ALL RIGHTS RESERVED
 *
 *      THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED
 * AND COPIED ONLY IN ACCORDANCE WITH THE TERMS OF SUCH LICENSE AND
 * WITH THE INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR
 * ANY OTHER COPIES THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE
 * AVAILABLE TO ANY OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF THE
 * SOFTWARE IS HEREBY TRANSFERRED.
 *
 *      THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE WITHOUT
 * NOTICE AND SHOULD NOT BE CONSTRUED AS A COMMITMENT BY OPEN SOFTWARE
 * FOUNDATION, INC. OR ITS THIRD PARTY SUPPLIERS
 *
 *      OPEN SOFTWARE FOUNDATION, INC. AND ITS THIRD PARTY SUPPLIERS,
 * ASSUME NO RESPONSIBILITY FOR THE USE OR INABILITY TO USE ANY OF ITS
 * SOFTWARE .   OSF SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 * KIND, AND OSF EXPRESSLY DISCLAIMS ALL IMPLIED WARRANTIES, INCLUDING
 * BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Notice:  Notwithstanding any other lease or license that may pertain to,
 * or accompany the delivery of, this computer software, the rights of the
 * Government regarding its use, reproduction and disclosure are as set
 * forth in Section 52.227-19 of the FARS Computer Software-Restricted
 * Rights clause.
 *
 * (c) Copyright 1989,1990 Open Software Foundation, Inc.  Unpublished - all
 * rights reserved under the Copyright laws of the United States.
 *
 * RESTRICTED RIGHTS NOTICE:  Use, duplication, or disclosure by the
 * Government is subject to the restrictions as set forth in subparagraph
 * (c)(1)(ii) of the Rights in Technical Data and Computer Software clause
 * at DFARS 52.227-7013.
 *
 * Open Software Foundation, Inc.
 * 11 Cambridge Center
 * Cambridge, MA   02142
 * (617)621-8700
 *
 * RESTRICTED RIGHTS LEGEND:  This computer software is submitted with
 * "restricted rights."  Use, duplication or disclosure is subject to the
 * restrictions as set forth in NASA FAR SUP 18-52.227-79 (April 1985)
 * "Commercial Computer Software- Restricted Rights (April 1985)."  Open
 * Software Foundation, Inc., 11 Cambridge Center, Cambridge, MA  02142.  If
 * the contract contains the Clause at 18-52.227-74 "Rights in Data General"
 * then the "Alternate III" clause applies.
 *
 * (c) Copyright 1989,1990 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED
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

/***********************************************************************
 *
 *  FILE:     clientprops.c
 *
 *  This client tests setting most of the client properties and tests
 *  all combinations of changing the client window's state.
 *
 *  The client properties set are:
 *
 *    WM_COMMAND
 *    WM_NAME
 *    WM_ICON_NAME
 *    WM_NORMAL_HINTS
 *    WM_HINTS
 *    WM_CLASS
 *    WM_TRANSIENT_FOR
 *    WM_PROTOCOLS
 *
 *  The client state transitions tested are:
 *
 *    Withdrawn -> Normal
 *    Withdrawn -> Iconic
 *    Normal -> Iconic
 *    Normal -> Withdrawn
 *    Iconic -> Normal
 *    Iconic -> Withdrawn
 *
 **********************************************************************/

#include <stdio.h>
#include <X11/cursorfont.h>
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/IntrinsicP.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xmd.h>
#include <Xm/MwmUtil.h>
#include <Xm/Xm.h>

Atom xa_WM_PROTOCOLS;
Atom xa_WM_SAVE_YOURSELF;
Atom xa_WM_TAKE_FOCUS;
Atom xa_WM_DELETE_WINDOW;
Atom xa_WM_CHANGE_STATE;

main (argc, argv)
int   argc;
char *argv[];
{
   static char	        keybuf[10];
   char                *kbp;
   int		        n, x, y;
   int		        pcount = 0;
   unsigned             fg, bg, bd, bw;
   Display             *dpy;
   Screen              *screen;
   Window               win;
   XEvent               event;
   Mask		        emask;
   XClassHint           xch;
   XClientMessageEvent  clientEvent;
   XUnmapEvent          unmapEvent;
   XSetWindowAttributes xswa;
   XSizeHints           xsh;
   XWMHints             xwmh;
   Atom                 property;
   char                *atomName, *atomValue;

   /*
    * Open the display using the $DISPLAY environment variable to locate
    * the X server.
    */

   if ((dpy = XOpenDisplay(NULL)) == NULL)
   {
      printf ("%s: can't open %s.\n", argv[0], XDisplayName(NULL));
      exit(1);
   }
   screen = DefaultScreenOfDisplay (dpy);

   /*
    * Initialize atoms
    */

    xa_WM_PROTOCOLS =     XInternAtom (dpy, "WM_PROTOCOLS", False);
    xa_WM_SAVE_YOURSELF = XInternAtom (dpy, "WM_SAVE_YOURSELF", False);
    xa_WM_TAKE_FOCUS =    XInternAtom (dpy, "WM_TAKE_FOCUS", False);
    xa_WM_DELETE_WINDOW = XInternAtom (dpy, "WM_DELETE_WINDOW", False);
    xa_WM_CHANGE_STATE =  XInternAtom (dpy, "WM_CHANGE_STATE", False);

   /* 
    * Select colors for the border, the window background, and the
    * window foreground.  Select the border width.
    */

   bd = WhitePixel(dpy, DefaultScreen(dpy));
   bg = BlackPixel(dpy, DefaultScreen(dpy));
   fg = WhitePixel(dpy, DefaultScreen(dpy));
   bw = 1;

   /* 
    * Fill out the XSizeHints structure for initial window position
    * and size.
    */

   xsh.flags = PAllHints & ~PAspect;
   xsh.x = .5 * DisplayWidth(dpy, DefaultScreen(dpy));
   xsh.y = .5 * DisplayHeight(dpy, DefaultScreen(dpy));
   xsh.width = 200;
   xsh.height = 200;
   xsh.min_width = 150;
   xsh.min_height = 150;
   xsh.max_width = 300;
   xsh.max_height = 300;
   xsh.width_inc = 5;
   xsh.height_inc = 5;

   /*
    * Fill out the XWMHints structure for initial client state.
    */

   xwmh.flags = (InputHint|StateHint);
   xwmh.input = False;	/* ignored by mwm */
   xwmh.initial_state = NormalState;
   xwmh.icon_pixmap = NULL;
   xwmh.icon_window = NULL;
   xwmh.icon_x = 0;
   xwmh.icon_y = 0;
   xwmh.icon_mask = 0;
   xwmh.window_group = 0;  /* ignored by mwm */

   /* Create the unmapped parent window.  */

   win = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy),
         xsh.x, xsh.y, xsh.width, xsh.height, bw, bd, bg);

   /* 
    * Set the standard properties and window manager hints for the window.
    */

   XStoreName (dpy, win, "clientprops");
   XSetIconName (dpy, win, "clientprops");
   XSetCommand (dpy, win, argv, argc);

   xch.res_name =  "clientprops";
   xch.res_class = "Clientprops";
   XSetClassHint (dpy, win, &xch);
   XSetNormalHints (dpy, win, &xsh);
   XSetWMHints (dpy, win, &xwmh);

   /*
    * Ensure that the window's colormap field points to the default
    *   colormap.
    * Select a font cursor for the window.
    */

   xswa.colormap = DefaultColormap(dpy, DefaultScreen(dpy));
   xswa.cursor = XCreateFontCursor(dpy, XC_arrow);
   XChangeWindowAttributes(dpy, win, (CWColormap|CWCursor), &xswa);

   /* Specify the event types we are interested in. */

   emask =  KeyPressMask;

   XSelectInput(dpy, win, emask);

   /* Map the window. */

   XMapWindow(dpy, win);

   /* Loop forever, examining each received event. */

   while (1) {

      /* Get the next event and switch on its type. */

      XNextEvent(dpy, &event);
      switch (event.type)
      {

      /* KeyPressMask */

      case (KeyPress):

	 n = XLookupString(&event.xkey, keybuf, sizeof(keybuf), NULL, NULL);
	 keybuf[n] = NULL;
	 for (kbp = keybuf; n > 0; n--) 
	 {
	    if (*kbp == '\003')		/* ^C terminates this client. */
	    {
	        exit(0);
            }
	    else if (*kbp == 'c')	/* WM_COMMAND property */
	    {
		XSetCommand (dpy, win, argv, argc);
            }
	    else if (*kbp == 'n')	/* WM_NAME property */
	    {
		XStoreName (dpy, win, "Name");
            }
	    else if (*kbp == 'i')	/* WM_ICON_NAME property */
	    {
		XSetIconName (dpy, win, "IconName");
            }
	    else if (*kbp == 'h')	/* WM_NORMAL_HINTS property */
	    {
	        /*
	         * Changes the following fields of the WM_SIZE_HINTS structure:
	         * 
	         *   pad (x, y, width, height),
                 *   min_width, min_height,
	         *   max_width, max_height
                 *   width_inc, height_inc
	         *
	         * Does not change the following (aspects tested elsewhere):
	         *
	         *   min_aspect.x, min_aspect.y,
	         *   max_aspect.x, max_aspect.y,
	         *   win_gravity
		 *
		 *  Then change the window state:  Normal -> Withdrawn.
		 *                                 Withdrawn -> Normal.
	         */

                XGetNormalHints (dpy, win, &xsh);
                xsh.flags = PAllHints & ~PAspect;
                xsh.height = 300;
                xsh.width = 300;
                xsh.x = .75 * DisplayWidth(dpy, DefaultScreen(dpy));
                xsh.y = .25 * DisplayHeight(dpy, DefaultScreen(dpy));
                xsh.min_width = 200;
                xsh.min_height = 200;
                xsh.max_width = 400;
                xsh.max_height = 400;
                xsh.width_inc = 3;
                xsh.height_inc = 3;

                XSetNormalHints (dpy, win, &xsh);
                XUnmapWindow(dpy, win);
                unmapEvent.type = UnmapNotify;
                unmapEvent.event = DefaultRootWindow (dpy);
                unmapEvent.window = win;
                unmapEvent.from_configure = False;
	        XSendEvent (dpy, DefaultRootWindow (dpy), False,
		    (SubstructureRedirectMask|SubstructureNotifyMask),
		    &unmapEvent);
                XMapWindow(dpy, win);
		XFlush (dpy);
            }
	    else if (*kbp == 'm')	/* WM_HINTS property to Move Icon*/
	    {
		/*
		 */
		XGetWMHints (dpy, win, &xwmh);
		xwmh.icon_window =
		    XCreateSimpleWindow(dpy, DefaultRootWindow(dpy),
			0,		/* x */
			0, 		/* y */
			50,		/* width */
			50,		/* height */
			5,		/* border width */
			1,		/* border pixel */
			0		/* background pixel */
			);

		xwmh.flags = (IconWindowHint |IconPositionHint| StateHint );
		xwmh.input = False;	/* ignored by mwm */
		xwmh.initial_state = IconicState;
		xwmh.icon_pixmap = NULL;
		xwmh.icon_window = NULL;
		xwmh.icon_x = 420;
		xwmh.icon_y = 550;
		xwmh.icon_mask = 0;
		xwmh.window_group = 0;  /* ignored by mwm */
		XSetWMHints (dpy, win, &xwmh);


                XUnmapWindow(dpy, win);
                unmapEvent.type = UnmapNotify;
                unmapEvent.event = DefaultRootWindow (dpy);
                unmapEvent.window = win;
                unmapEvent.from_configure = False;
	        XSendEvent (dpy, DefaultRootWindow (dpy), False,
		    (SubstructureRedirectMask|SubstructureNotifyMask),
		    &unmapEvent);
                XMapWindow(dpy, win);
		XFlush (dpy);
		sleep (6);
            }
	    else if (*kbp == 'w')	/* WM_HINTS property */
	    {
		/*
		 * Set initial state to Iconic.
		 * Change the window state:  Normal -> Withdrawn.
		 *                           Withdrawn -> Iconic.
		 * Set initial state to Normal.
		 * Change the window state:  Iconic -> Withdrawn.
		 *                           Withdrawn -> Normal.
		 */

                XGetWMHints (dpy, win, &xwmh);

                xwmh.initial_state = IconicState;
                XSetWMHints (dpy, win, &xwmh);
                XUnmapWindow(dpy, win);
                unmapEvent.type = UnmapNotify;
                unmapEvent.event = DefaultRootWindow (dpy);
                unmapEvent.window = win;
                unmapEvent.from_configure = False;
	        XSendEvent (dpy, DefaultRootWindow (dpy), False,
		    (SubstructureRedirectMask|SubstructureNotifyMask),
		    &unmapEvent);
                XMapWindow(dpy, win);
		XFlush (dpy);
		sleep (30);

                xwmh.initial_state = NormalState;
                XSetWMHints (dpy, win, &xwmh);
                XUnmapWindow(dpy, win);
                unmapEvent.type = UnmapNotify;
                unmapEvent.event = DefaultRootWindow (dpy);
                unmapEvent.window = win;
                unmapEvent.from_configure = False;
	        XSendEvent (dpy, DefaultRootWindow (dpy), False,
		    (SubstructureRedirectMask|SubstructureNotifyMask),
		    &unmapEvent);
                XMapWindow(dpy, win);
		XFlush (dpy);
            }
	    else if (*kbp == 'l')	/* WM_CLASS property */
            {
		/*
		 * Change the Instance and Class names for this client.
		 * Change the window state:  Normal -> Withdrawn.
		 *                           Withdrawn -> Normal.
		 */

                XGetClassHint (dpy, win, &xch);
                xch.res_name =  "NameRes";
                xch.res_class = "ClassRes";
                XSetClassHint (dpy, win, &xch);
                XUnmapWindow(dpy, win);
                unmapEvent.type = UnmapNotify;
                unmapEvent.event = DefaultRootWindow (dpy);
                unmapEvent.window = win;
                unmapEvent.from_configure = False;
	        XSendEvent (dpy, DefaultRootWindow (dpy), False,
		    (SubstructureRedirectMask|SubstructureNotifyMask),
		    &unmapEvent);
                XMapWindow(dpy, win);
		XFlush (dpy);
	    }
	    else if (*kbp == 't')	/* set WM_TRANSIENT_FOR property */
	    {
		/*
		 * Make the client window transient for the root window.
		 * Change the window state:  Normal -> Withdrawn.
		 *                           Withdrawn -> Normal.
		 */
		XSetTransientForHint (dpy, win, DefaultRootWindow (dpy)); 
		XStoreName (dpy, win, "transient");
                XUnmapWindow(dpy, win);
                unmapEvent.type = UnmapNotify;
                unmapEvent.event = DefaultRootWindow (dpy);
                unmapEvent.window = win;
                unmapEvent.from_configure = False;
	        XSendEvent (dpy, DefaultRootWindow (dpy), False,
		    (SubstructureRedirectMask|SubstructureNotifyMask),
		    &unmapEvent);
                XMapWindow(dpy, win);
		XFlush (dpy);
            }
	    else if (*kbp == 'r')	/* delete WM_TRANSIENT_FOR property */
	    {
		/*
		 * Delete the TRANSIENT_FOR property for this window.
		 * Change the window state:  Normal -> Withdrawn.
		 *                           Withdrawn -> Normal.
		 */
		XDeleteProperty (dpy, win, XA_WM_TRANSIENT_FOR); 
                XStoreName (dpy, win, "Name");
                XUnmapWindow(dpy, win);
                unmapEvent.type = UnmapNotify;
                unmapEvent.event = DefaultRootWindow (dpy);
                unmapEvent.window = win;
                unmapEvent.from_configure = False;
	        XSendEvent (dpy, DefaultRootWindow (dpy), False,
		    (SubstructureRedirectMask|SubstructureNotifyMask),
		    &unmapEvent);
                XMapWindow(dpy, win);
		XFlush (dpy);
            }
	    else if (*kbp == 'p')	/* WM_PROTOCOLS property */
	    {
                /*
                 * Succesively set the WM_PROTOCOLS property to:
	         *
	         *   xa_WM_TAKE_FOCUS,
	         *   xa_WM_DELETE_WINDOW,
	         *   xa_WM_SAVE_YOURSELF.
	         *
		 * Change the window state:  Normal -> Withdrawn.
		 *                           Withdrawn -> Normal.
		 *
	         * The user should then attempt to close the client.
                 */
                if (pcount == 0)
		{
		    property = xa_WM_TAKE_FOCUS;
		}
                else if (pcount == 1)
		{
		    property = xa_WM_DELETE_WINDOW;
		}
                else
		{
		    property = xa_WM_SAVE_YOURSELF;
		}

                XChangeProperty (dpy, win, xa_WM_PROTOCOLS, xa_WM_PROTOCOLS,
		                 32, PropModeReplace, 
				 (unsigned char *) &property, 1);

		if (++pcount > 2)
		{
		   pcount = 0;
		}
                XUnmapWindow(dpy, win);
                unmapEvent.type = UnmapNotify;
                unmapEvent.event = DefaultRootWindow (dpy);
                unmapEvent.window = win;
                unmapEvent.from_configure = False;
	        XSendEvent (dpy, DefaultRootWindow (dpy), False,
		    (SubstructureRedirectMask|SubstructureNotifyMask),
		    &unmapEvent);
                XMapWindow(dpy, win);
		XFlush (dpy);
	    }
	    else if (*kbp == 'y')
            /*
             * Change the window state:  Normal -> Iconic.
             */
	    {
                clientEvent.type = ClientMessage;
                clientEvent.window = win;
                clientEvent.message_type = xa_WM_CHANGE_STATE;
                clientEvent.format = 32;
                clientEvent.data.l[0] = IconicState;
	        XSendEvent (dpy, DefaultRootWindow (dpy), False,
		    (SubstructureRedirectMask|SubstructureNotifyMask),
		    &clientEvent);
	        XFlush (dpy);
	    }
	    else if (*kbp == 'z')
            /*
             * Change the window state:  Normal -> Iconic.
             *                           Iconic -> Normal.
             */
	    {
                clientEvent.type = ClientMessage;
                clientEvent.window = win;
                clientEvent.message_type = xa_WM_CHANGE_STATE;
                clientEvent.format = 32;
                clientEvent.data.l[0] = IconicState;
	        XSendEvent (dpy, DefaultRootWindow (dpy), False,
		    (SubstructureRedirectMask|SubstructureNotifyMask),
		    &clientEvent);
	        XFlush (dpy);
		sleep (30);
                XMapWindow(dpy, win);
	    }

         }  /* for */
	 break;

      case (ClientMessage):

         if (event.xclient.message_type == xa_WM_PROTOCOLS)
	 {
             atomName = XGetAtomName (dpy, event.xclient.message_type);
             atomValue = XGetAtomName (dpy, event.xclient.data.l[0]);
             printf ("Received %s message:  %s\n", atomName, atomValue);
	     XFree (atomName);
	     XFree (atomValue);
	 }
	 break;

      }  /* switch */
   }  /* while */
}








