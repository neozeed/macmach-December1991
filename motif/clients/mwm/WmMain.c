#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)WmMain.c	3.11 91/01/10";
#endif /* lint */
#endif /* REV_INFO */
 /*****************************************************************************
 ******************************************************************************
 *
 * (c) Copyright 1989, 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
 * (c) Copyright 1987, 1988, 1989, 1990 HEWLETT-PACKARD COMPANY
 * ALL RIGHTS RESERVED
 *
 * 	THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED
 * AND COPIED ONLY IN ACCORDANCE WITH THE TERMS OF SUCH LICENSE AND
 * WITH THE INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR
 * ANY OTHER COPIES THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE
 * AVAILABLE TO ANY OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF THE
 * SOFTWARE IS HEREBY TRANSFERRED.
 *
 * 	THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE WITHOUT
 * NOTICE AND SHOULD NOT BE CONSTRUED AS A COMMITMENT BY OPEN SOFTWARE
 * FOUNDATION, INC. OR ITS THIRD PARTY SUPPLIERS
 *
 * 	OPEN SOFTWARE FOUNDATION, INC. AND ITS THIRD PARTY SUPPLIERS,
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
 * (c) Copyright 1989,1990, 1991 Open Software Foundation, Inc.  Unpublished - all
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
 * (c) Copyright 1989,1990, 1991 Open Software Foundation, Inc.
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
 ******************************************************************************
 *****************************************************************************/



/*
 * Included Files:
 */

#include "WmGlobal.h"

#ifdef MULTIBYTE
#include <locale.h>
#endif

/*
 * include extern functions
 */

#include "WmCEvent.h"
#include "WmEvent.h"
#include "WmInitWs.h"


/*
 * Function Declarations:
 */

#ifdef _NO_PROTO
void main ();
Boolean ManagedRoot();
#else /* _NO_PROTO */
void main (int argc, char *argv [], char *environ []);
Boolean ManagedRoot (Window w);
#endif /* _NO_PROTO */

/*
 * Global Variables:
 */

WmGlobalData wmGD;



/*************************************<->*************************************
 *
 *  main (argc, argv, environ)
 *
 *
 *  Description:
 *  -----------
 *  This is the main window manager function.  It calls window manager
 *  initializtion functions and has the main event processing loop.
 *
 *
 *  Inputs:
 *  ------
 *  argc = number of command line arguments (+1)
 *
 *  argv = window manager command line arguments
 *
 *  environ = window manager environment
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void 
main (argc, argv, environ)
    int argc;
    char *argv[];
    char *environ[];

#else /* _NO_PROTO */
void
main (int argc, char *argv [], char *environ [])
#endif /* _NO_PROTO */
{
    XEvent	event;
    Boolean	dispatchEvent;

#ifdef MULTIBYTE
    setlocale (LC_ALL, "");
#endif /* MULTIBYTE */

    /*
     * Initialize the workspace:
     */

    InitWmGlobal (argc, argv, environ);
    
    /*
     * MAIN EVENT HANDLING LOOP:
     */

    for (;;)
    {
        XtAppNextEvent (wmGD.mwmAppContext, &event);


        /*
	 * Check for, and process non-widget events.  The events may be
	 * reported to the root window, to some client frame window,
	 * to an icon window, or to a "special" window management window.
	 * The lock modifier is "filtered" out for window manager processing.
	 */

	wmGD.attributesWindow = NULL;

	if ((event.type == ButtonPress) || (event.type == ButtonRelease) ||
	     (event.type == KeyPress) || (event.type == KeyRelease))
	{
	    wmGD.currentEventState = event.xbutton.state;
	    if (wmGD.ignoreLockMod)
	    {
	        event.xbutton.state &= ~(LockMask);
	    }
	}

	dispatchEvent = True;
	if (wmGD.menuActive)
	{
	    /*
	     * Do special menu event preprocessing.
	     */

	    if (wmGD.checkHotspot || wmGD.menuUnpostKeySpec ||
		wmGD.menuActive->accelKeySpecs)
	    {
	        dispatchEvent = WmDispatchMenuEvent ((XButtonEvent *) &event);
	    }
	}

	if (dispatchEvent)
	{
	    if (ManagedRoot(event.xany.window))
	    {
	        dispatchEvent = WmDispatchWsEvent (&event);
	    }
	    else
	    {
	        dispatchEvent = WmDispatchClientEvent (&event);
	    }

	    if (dispatchEvent)
	    {
                /*
                 * Dispatch widget related event:
                 */

                XtDispatchEvent (&event);
	    }
	}
    }

} /* END OF FUNCTION main */



/*************************************<->*************************************
 *
 *  ManagedRoot (w)
 *
 *
 *  Description:
 *  -----------
 *  Tests the window passed in to see if it is a root window we manage.
 *
 *
 *  Inputs:
 *  ------
 *  w = window id
 *
 *  Returns:
 *  --------
 *  True if window passed in is a root window we manage
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
Boolean
ManagedRoot (w)
    Window w;

#else /* _NO_PROTO */
Boolean
ManagedRoot (Window w)
#endif /* _NO_PROTO */
{
    WmScreenData *pSD;
    Boolean rval = False;
    
    if (!XFindContext (DISPLAY, w, wmGD.screenContextType, (caddr_t *)&pSD))
    {
	SetActiveScreen (pSD);
	rval = True;
    }

    return (rval);

} /* END OF FUNCTION ManagedRoot */
