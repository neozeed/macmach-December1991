#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)WmFunction.c	3.14.1.4 91/03/21";
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

#include <signal.h>
#include <stdio.h>
#include <string.h>
#include "WmGlobal.h"
#include "WmICCC.h"

/*
 * include extern functions
 */
#include "WmFunction.h"
#include "WmCDInfo.h"
#include "WmColormap.h"
#include "WmError.h"
#include "WmEvent.h"
#include "WmFeedback.h"
#include "WmIPlace.h"
#include "WmIconBox.h"
#include "WmKeyFocus.h"
#include "WmMenu.h"
#include "WmProperty.h"
#include "WmProtocol.h"
#include "WmWinConf.h"
#include "WmWinInfo.h"
#include "WmWinList.h"
#include "WmWinState.h"

extern char * getenv ();


/*
 * Global Variables:
 */




/******************************<->*************************************
 *
 *  F_Beep (args, pCD, event)
 *
 *
 *  Description:
 *  -----------
 *  This is the window manager function handler for beeping.
 *
 *
 *  Inputs:
 *  ------
 *  args = function arguments (specified in .mwmrc file)
 *
 *  pCD = pointer to the client data for the client window to which the
 *        function is to be applied
 *
 *  event = X event that invoked the function (key, button, or menu/NULL)
 *
 *
 *  Outputs:
 *  -------
 *  RETURN = if True then further button binding/function processing can
 *           be done for the event that caused this function to be called.
 *
 ******************************<->***********************************/

#ifdef _NO_PROTO
Boolean F_Beep (args, pCD, event)
    String args;
    ClientData *pCD;
    XEvent *event;

#else /* _NO_PROTO */
Boolean F_Beep (String args, ClientData *pCD, XEvent *event)
#endif /* _NO_PROTO */
{

    /* !!! what is a good value for percent (the second arg) !!! */
    XBell (DISPLAY, 0);

    return (True);

} /* END OF FUNCTION F_Beep */



/*************************************<->*************************************
 *
 *  F_Lower (args, pCD, event)
 *
 *
 *  Description:
 *  -----------
 *  This is the window manager function handler for bottoming a client window
 *  or icon.
 *
 *
 *  Inputs:
 *  ------
 *  args = function arguments (specified in .mwmrc file)
 *
 *  pCD = pointer to the client data for the client window to which the
 *        function is to be applied
 *
 *  event = X event that invoked the function (key, button, or menu/NULL)
 *
 *
 *  Outputs:
 *  -------
 *  RETURN = if True then further button binding/function processing can
 *           be done for the event that caused this function to be called.
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
Boolean F_Lower (args, pCD, event)
    String args;
    ClientData *pCD;
    XEvent *event;

#else /* _NO_PROTO */
Boolean F_Lower (String args, ClientData *pCD, XEvent *event)
#endif /* _NO_PROTO */
{
    ClientListEntry *pEntry;
    ClientListEntry *pNextEntry;
    ClientListEntry *pStackEntry;

    if (args)
    {
        pStackEntry = NULL;
	pNextEntry = ACTIVE_PSD->lastClient;
	while (pNextEntry &&
	       (pEntry = FindClientNameMatch (pNextEntry, False, args,
			         F_GROUP_ALL)))
	{
	    pNextEntry = pEntry->prevSibling;
	    Do_Lower (pEntry->pCD, pStackEntry);
	    pStackEntry = pEntry;
	}
    }
    else if (pCD)
    {
	Do_Lower (pCD, (ClientListEntry *) NULL);
    }

    wmGD.passButtonsCheck = False;
    return (True);

} /* END OF FUNCTION F_Lower */



/*************************************<->*************************************
 *
 *  Do_Lower (pCD, pStackEntry)
 *
 *
 *  Description:
 *  -----------
 *  This is the window manager function handler for lowering the client window
 *  so that it does not obscure any other window above the stack entry
 *  window.
 *
 *
 *  Inputs:
 *  ------
 *  pCD = pointer to the client data of the window (or icon) to be lowered.
 * 
 *  pStackEntry = pointer to client list entry for window that is to be 
 *	below the lowered window (if NULL, window is lowered to the bottom
 *	of the	stack).
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void Do_Lower (pCD, pStackEntry)
    ClientData *pCD;
    ClientListEntry *pStackEntry;

#else /* _NO_PROTO */
void Do_Lower (ClientData *pCD, ClientListEntry *pStackEntry)
#endif /* _NO_PROTO */
{
    Boolean restackTransients;
    ClientData *pcdLeader;
    WmWorkspaceData *pWS = ACTIVE_WS;


    pcdLeader = (pCD->transientLeader) ? FindTransientTreeLeader (pCD) : pCD;

    if ((pcdLeader->clientState == MINIMIZED_STATE) && !P_ICON_BOX(pcdLeader))
    {
	/*
	 * Only restack the icon if it is not currently lowered.
	 */

	if (pStackEntry)
	{
	    if (pStackEntry->prevSibling != &pcdLeader->iconEntry)
	    {
		StackWindow (pWS, &pcdLeader->iconEntry, True /*above*/,
		    pStackEntry);
		MoveEntryInList (pWS, &pcdLeader->iconEntry, True /*above*/,
		    pStackEntry);
	    }
	}
	else
	{
	    if (ACTIVE_PSD->lastClient != &pcdLeader->iconEntry)
	    {
		StackWindow (pWS, &pcdLeader->iconEntry, 
			     False /*on bottom*/, (ClientListEntry *) NULL);
		MoveEntryInList (pWS, &pcdLeader->iconEntry, 
			     False /*on bottom*/, (ClientListEntry *) NULL);
	    }
	}
    }
    else /* NORMAL_STATE, MAXIMIZED_STATE, adoption */
    {
	/*
	 * If this is a transient window then put it below its
	 * sibling transient windows.
	 */

	restackTransients = False;
	if (pCD->transientLeader)
	{
	    restackTransients = PutTransientBelowSiblings (pCD);
	}

	/*
	 * Only restack the window or transient window tree if it is
	 * not currently lowered and the window is not a system
	 * modal window.
	 */

	if (pStackEntry)
	{
	    if ((pStackEntry->prevSibling != &pcdLeader->clientEntry) &&
		!(wmGD.systemModalActive &&
		  (pcdLeader == wmGD.systemModalClient)))
	    {
	        StackWindow (pWS, &pcdLeader->clientEntry, True /*above*/,
		    pStackEntry);
		MoveEntryInList (pWS, &pcdLeader->clientEntry, True /*above*/,
		    pStackEntry);
	    }
	    else if (restackTransients)
	    {
		RestackTransients (pCD, False);
	    }
	}
	else
	{
	    if ((pWS->pSD->lastClient != &pcdLeader->clientEntry) &&
		!(wmGD.systemModalActive &&
		  (pcdLeader == wmGD.systemModalClient)))
	    {
	        StackWindow (pWS, &pcdLeader->clientEntry, False /*on bottom*/,
		    (ClientListEntry *) NULL);
		MoveEntryInList (pWS, &pcdLeader->clientEntry,
		    False /*on bottom*/, (ClientListEntry *) NULL);
	    }
	    else if (restackTransients)
	    {
		RestackTransients (pCD, False);
	    }
	}
    }

} /* END OF FUNCTION Do_Lower */



/*************************************<->*************************************
 *
 *  F_CircleDown (args, pCD, event)
 *
 *
 *  Description:
 *  -----------
 *  This is the window manager function handler for moving the client window
 *  on top of stack to the bottom.
 *
 *
 *  Inputs:
 *  ------
 *  args = function arguments (specified in .mwmrc file)
 *
 *  pCD = pointer to the client data for the client window to which the
 *        function is to be applied
 *
 *  event = X event that invoked the function (key, button, or menu/NULL)
 *
 *
 *  Outputs:
 *  -------
 *  RETURN = if True then further button binding/function processing can
 *           be done for the event that caused this function to be called.
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
Boolean F_Circle_Down (args, pCD, event)
    String args;
    ClientData *pCD;
    XEvent *event;

#else /* _NO_PROTO */
Boolean F_Circle_Down (String args, ClientData *pCD, XEvent *event)
#endif /* _NO_PROTO */
{
    unsigned long types;
    unsigned long windowType;
    ClientListEntry *pNextEntry;
    ClientData *pcdNext;


    /*
     * Go down through the client list looking for a window of an
     * appropriate type that is obscuring lower windows.
     */

    types = (unsigned long)args;
    pNextEntry = ACTIVE_PSD->clientList;
    while (pNextEntry)
    {
	/*
	 * Only check out the window if it is onscreen.
	 */

	pcdNext = pNextEntry->pCD;
	if (((pNextEntry->type == NORMAL_STATE) &&
	     (pcdNext->clientState != MINIMIZED_STATE)) ||
	    ((pNextEntry->type == MINIMIZED_STATE) &&
	     (pcdNext->clientState == MINIMIZED_STATE)))
	{
	    if (pcdNext->clientState == MINIMIZED_STATE)
	    {
		windowType = F_GROUP_ICON;
	    }
	    else
	    {
		windowType = F_GROUP_WINDOW;
		if (pcdNext->transientLeader || pcdNext->transientChildren)
		{
		    windowType |= F_GROUP_TRANSIENT;
		}
	    }
	    if (types & windowType)
	    {
		if (CheckIfClientObscuringAny (pcdNext))
		{
		    /*
		     * This window (or window tree) is obscuring another window
		     * on the screen.  Lower the window.
		     */

		    F_Lower (NULL, pcdNext, (XEvent *) NULL);
		    break;
		}
	    }
	}
	pNextEntry = pNextEntry->nextSibling;
    }

    return (True);

} /* END OF FUNCTION F_Circle_Down */



/*************************************<->*************************************
 *
 *  F_Circle_Up (args, pCD, event)
 *
 *
 *  Description:
 *  -----------
 *  This is the window manager function handler for moving the client window
 *  on the bottom of the stack to the top.
 *
 *
 *  Inputs:
 *  ------
 *  args = function arguments (specified in .mwmrc file)
 *
 *  pCD = pointer to the client data for the client window to which the
 *        function is to be applied
 *
 *  event = X event that invoked the function (key, button, or menu/NULL)
 *
 *
 *  Outputs:
 *  -------
 *  RETURN = if True then further button binding/function processing can
 *           be done for the event that caused this function to be called.
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
Boolean F_Circle_Up (args, pCD, event)
    String args;
    ClientData *pCD;
    XEvent *event;

#else /* _NO_PROTO */
Boolean F_Circle_Up (String args, ClientData *pCD, XEvent *event)
#endif /* _NO_PROTO */
{
    unsigned long types;
    unsigned long windowType;
    ClientListEntry *pNextEntry;
    ClientData *pcdNext;


    /*
     * Go up through the client list looking for a window of an
     * appropriate type that is obscured by higher windows.
     */

    types = (unsigned long)args;
    pNextEntry = ACTIVE_PSD->lastClient;
    while (pNextEntry)
    {
	/*
	 * Only check out the window if it is onscreen.
	 */

	pcdNext = pNextEntry->pCD;
	if (((pNextEntry->type == NORMAL_STATE) &&
	     (pcdNext->clientState != MINIMIZED_STATE)) ||
	    ((pNextEntry->type == MINIMIZED_STATE) &&
	     (pcdNext->clientState == MINIMIZED_STATE)))
	{
	    if (pcdNext->clientState == MINIMIZED_STATE)
	    {
		windowType = F_GROUP_ICON;
	    }
	    else
	    {
		windowType = F_GROUP_WINDOW;
		if (pcdNext->transientLeader || pcdNext->transientChildren)
		{
		    windowType |= F_GROUP_TRANSIENT;
		}
	    }
	    if (types & windowType)
	    {
		if (CheckIfClientObscuredByAny (pcdNext))
		{
		    /*
		     * This window (or window tree) is obscured by another
		     * window on the screen.  Raise the window.
		     */

		    F_Raise (NULL, pcdNext, (XEvent *) NULL);
		    break;
		}
	    }
	}
	pNextEntry = pNextEntry->prevSibling;
    }

    return (True);


} /* END OF FUNCTION F_Circle_Up */



/*************************************<->*************************************
 *
 *  F_Focus_Color (args, pCD, event)
 *
 *
 *  Description:
 *  -----------
 *  This is the window manager function handler for setting the colormap
 *  focus to a client window or reinstalling the default colormap.
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
Boolean F_Focus_Color (args, pCD, event)
    String args;
    ClientData *pCD;
    XEvent *event;

#else /* _NO_PROTO */
Boolean F_Focus_Color (String args, ClientData *pCD, XEvent *event)
#endif /* _NO_PROTO */
{

    if (wmGD.colormapFocusPolicy == CMAP_FOCUS_EXPLICIT)
    {
        if (pCD)
        {
	    /*
	     * The window selected for the colormap focus is a top-level client
	     * window.  If there are subwindow colormaps then determine if the
	     * selection was in one of the subwindows.
	     */

	    if (pCD->clientState == MINIMIZED_STATE)
	    {
		/* !!! colormap for client supplied icon window !!! */
		pCD = NULL;
	    }
        }

        SetColormapFocus (ACTIVE_PSD, pCD);
    }

    return (True);

} /* END OF FUNCTION F_Focus_Color */



/*************************************<->*************************************
 *
 *  F_Exec (args, pCD, event)
 *
 *
 *  Description:
 *  -----------
 *  This is the window manager function handler for executing a command
 *  (with /bin/sh).
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
Boolean F_Exec (args, pCD, event)
    String args;
    ClientData *pCD;
    XEvent *event;

#else /* _NO_PROTO */
Boolean F_Exec (String args, ClientData *pCD, XEvent *event)
#endif /* _NO_PROTO */
{
    int   status;
    int   pid;
    int   w;
    void (*intStat) ();
    void (*quitStat) ();
    char *shell;
    char *shellname;


    if(wmGD.pActiveSD->displayString)
    {
	putenv(wmGD.pActiveSD->displayString);
    }

    
    
    
    
    /*
     * Fork a process to exec a shell to run the specified command:
     */

#ifdef PORT_NOVFORK
    if ((pid = fork ()) == 0)
#else
    if ((pid = vfork ()) == 0)
#endif
    {
	/*
	 * Clean up window manager resources.
	 * The X file descriptor should be automatically closed.
	 */


	/*
	 * Fix up signal handling.
	 */

	if (signal (SIGINT, SIG_DFL) == SIG_IGN)
	{
	    signal (SIGINT, SIG_IGN);
	}
	if (signal (SIGHUP, SIG_DFL) == SIG_IGN)
	{
	    signal (SIGHUP, SIG_IGN);
	}
	signal (SIGQUIT, SIG_DFL);
	signal (SIGTERM, SIG_DFL);


	/*
	 * Exec the command using $MWMSHELL if set or 
	 * $SHELL if set and $MWMSHELL not set or sh.
	 */

        if (((shell = getenv ("MWMSHELL")) != NULL) ||
	    ((shell = getenv ("SHELL")) != NULL))

	{
	    shellname = strrchr (shell, '/');
	    if (shellname == NULL)
	    {
		shellname = shell;
	    }
	    else
	    {
		shellname++;
	    }
	    execl (shell, shellname, "-c", args, 0);
	}

	/*
	 * There is no SHELL environment variable or the first execl failed.
	 * Try /bin/sh .
	 */
	execl ("/bin/sh", "sh", "-c", args, 0);


	/*
	 * Error - command could not be exec'ed.
	 */

	_exit (127);
    }

    /*
     * Have the window manager wait for the shell to complete.
     */

    intStat = (void (*)())signal (SIGINT, SIG_IGN);
    quitStat = (void (*)())signal (SIGQUIT, SIG_IGN);

    while ((w = wait (&status)) != pid && (w != -1));

    if (w == -1)
    {
	status = -1;
    }

    signal (SIGINT, intStat);
    signal (SIGQUIT, quitStat);

    return (True);


} /* END OF FUNCTION F_Exec */



/*************************************<->*************************************
 *
 *  F_Quit_Mwm (args, pCD, event)
 *
 *
 *  Description:
 *  -----------
 *  This is the window manager function handler for terminating the window
 *  manager.
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
Boolean F_Quit_Mwm (args, pCD, event)
    String args;
    ClientData *pCD;
    XEvent *event;

#else /* _NO_PROTO */
Boolean F_Quit_Mwm (String args, ClientData *pCD, XEvent *event)
#endif /* _NO_PROTO */
{
    if (wmGD.showFeedback & WM_SHOW_FB_QUIT)
    {
	ConfirmAction (ACTIVE_PSD, QUIT_MWM_ACTION);
    }
    else
    {
	Do_Quit_Mwm(False);
    }
    
    return (False);

} /* END OF FUNCTION F_Quit_Mwm */



/*************************************<->*************************************
 *
 *  Do_Quit_Mwm (diedOnRestart)
 *
 *
 *  Description:
 *  -----------
 *  Callback to do the f.quit_mwm function.
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void Do_Quit_Mwm (diedOnRestart)
    Boolean diedOnRestart;
#else /* _NO_PROTO */
void Do_Quit_Mwm (Boolean diedOnRestart)
#endif /* _NO_PROTO */
{
    int scr;
    ClientListEntry *pNextEntry;


    /*
     * Close the X connection to get all the X resources cleaned up.
     * !!! maybe windows should be reparented / rebordered  before closing? !!!
     * !!! clean up the _MOTIF_WM_INFO property on the root window !!!
     */


    if (DISPLAY)
    {
        XSetInputFocus(DISPLAY, PointerRoot, RevertToPointerRoot, CurrentTime);
	for (scr = 0; scr < wmGD.numScreens; scr++)
	{
	    if (wmGD.Screens[scr].managed)
	    {
		pNextEntry = wmGD.Screens[scr].lastClient;
		while (pNextEntry)
		{
		    if (pNextEntry->type == NORMAL_STATE)
		    {
			if (!(pNextEntry->pCD->clientFlags & 
			      CLIENT_WM_CLIENTS))
			{
			    ReBorderClient (pNextEntry->pCD, diedOnRestart);
			}
		    }
		    pNextEntry = pNextEntry->prevSibling;
		}
	    }
	}
        XSync (DISPLAY, False);
        XCloseDisplay (DISPLAY);
    }
    
    if(diedOnRestart)
    {
	exit (WM_ERROR_EXIT_VALUE);
    }
    else
    {
	exit (0);
    }

} /* END OF FUNCTION Do_Quit_Mwm */


/*************************************<->*************************************
 *
 *  ReBorderClient (pCD, reMapClient)
 *
 *
 *  Description:
 *  -----------
 *  Restores X border for client window and reparents the
 *  window back to the root.
 *
 *
 *  Inputs:
 *  -------
 *  pCD = pointer to the client data for the window to be re-bordered.
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void ReBorderClient (pCD, reMapClient)
    ClientData *pCD;
    Boolean  reMapClient;
#else /* _NO_PROTO */
void ReBorderClient (ClientData *pCD, Boolean reMapClient)
#endif /* _NO_PROTO */
{
    int x, y;
    int xoff, yoff;
    XWindowChanges windowChanges;

    while (pCD)
    {
        if (pCD->iconWindow && (pCD->clientFlags & ICON_REPARENTED) &&
	    (!(reMapClient)))
        {
	    XUnmapWindow (DISPLAY, pCD->iconWindow);
	    XReparentWindow (DISPLAY, pCD->iconWindow, 
		ROOT_FOR_CLIENT(pCD), pCD->iconX, pCD->iconY);
        }

	if (!(reMapClient))
	{
	    if (pCD->maxConfig)
	    {
		x = pCD->maxX;
		y = pCD->maxY;
	    }
	    else
	    {
		if(wmGD.positionIsFrame)
		{
		    CalculateGravityOffset (pCD, &xoff, &yoff);
		    x = pCD->clientX - xoff;
		    y = pCD->clientY - yoff;
		}
	    }
	    XUnmapWindow(DISPLAY, pCD->clientFrameWin);
	    XReparentWindow (DISPLAY, pCD->client, 
			     ROOT_FOR_CLIENT(pCD), x, y);
	}
	else
	{
	    XMapWindow(wmGD.display, pCD->client);
	}

	if (pCD->transientChildren)
	{
	    ReBorderClient (pCD->transientChildren, reMapClient);
	}

	if (!(reMapClient))
	{
	    /*
	     * restore X border
	     */
	    windowChanges.x = x;
	    windowChanges.y = y;
	    windowChanges.border_width = pCD->xBorderWidth;
	    XConfigureWindow (DISPLAY, pCD->client, 
			      CWBorderWidth | CWX | CWY, &windowChanges);
	}

	if (pCD->transientLeader)
	{
	    pCD = pCD->transientSiblings;
	}
	else
	{
	    pCD = NULL;
	}
    }

} /* END OF FUNCTION ReBorderClient */



/*************************************<->*************************************
 *
 *  F_Focus_Key (args, pCD, event)
 *
 *
 *  Description:
 *  -----------
 *  This is the window manager function handler for setting the keyboard
 *  focus to a particular client window.
 *
 *
 *  Inputs:
 *  ------
 *  args = (immediate value) focus flags
 *
 *  pCD = pointer to the client data
 *
 *  event = X event that invoked the function (key, button, or menu/NULL)
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
Boolean F_Focus_Key (args, pCD, event)
    String args;
    ClientData *pCD;
    XEvent *event;

#else /* _NO_PROTO */
Boolean F_Focus_Key (String args, ClientData *pCD, XEvent *event)
#endif /* _NO_PROTO */
{
    long focusFlags = (long)args;


    if (pCD && (wmGD.keyboardFocusPolicy == KEYBOARD_FOCUS_EXPLICIT))
    {
	Do_Focus_Key (pCD, GetFunctionTimestamp ((XButtonEvent *)event),
	    (focusFlags | ALWAYS_SET_FOCUS));
    }

    return (True);

} /* END OF FUNCTION F_Focus_Key */



/*************************************<->*************************************
 *
 *  Do_Focus_Key (pCD, focusTime, flags)
 *
 *
 *  Description:
 *  -----------
 *  This function is used to set the focus to a window.  The focus indication
 *  is not changed until the FocusIn event is received.
 *
 *
 *  Inputs:
 *  ------
 *  pCD = pointer to the client data
 *
 *  focusTime = focus change time
 *
 *  flags = wm focus change flags
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void Do_Focus_Key (pCD, focusTime, flags)
    ClientData *pCD;
    Time focusTime;
    long flags;

#else /* _NO_PROTO */
void Do_Focus_Key (ClientData *pCD, Time focusTime, long flags)
#endif /* _NO_PROTO */
{
    ClientData *pcdFocus;
    Window focusWindow;


    pcdFocus = pCD;
    if (pCD)
    {
	if (pCD->clientState == MINIMIZED_STATE)
	{
	    focusWindow = ICON_FRAME_WIN(pCD);
	}
	else if (wmGD.keyboardFocusPolicy == KEYBOARD_FOCUS_EXPLICIT)
	{
	    /*
	     * Set the keyboard focus to the indicated client window.
	     * If the window has an application modal subordinate then
	     * set the input focus to that window if the focus isn't
	     * already owned by a subordinate.
	     */

	    if (IS_APP_MODALIZED(pCD))
	    {
		ClientData *pcdFocusLeader,*currFocusLeader;

		/*
		 * Handle case where a modal window exists when Mwm starts up.
		 * wmGD.keyboardFocus is NULL, give focus to the modal dialog.
		 */

	        if (wmGD.keyboardFocus)
		{
		    currFocusLeader = wmGD.keyboardFocus->transientLeader;
		}
		else
		{
		    currFocusLeader = (ClientData *) NULL;
		}

		/*
		 * Find focus leader for pCD
		 */

		pcdFocusLeader = pCD;
		while (pcdFocusLeader->transientLeader &&
		       (pcdFocusLeader != currFocusLeader))
		{
		    pcdFocusLeader = pcdFocusLeader->transientLeader;
		}

		if (pcdFocusLeader == currFocusLeader)
		{
		    pcdFocus = wmGD.keyboardFocus;
		    flags = 0;
		}
		else
		{
		    pcdFocus = FindTransientFocus (pcdFocusLeader);
		}
	    }

	    focusWindow = pcdFocus->client;
	}
	else
	{
	    /*
	     * If the focus policy is "pointer" don't set the focus to a
	     * window is it has an application modal subordinate.
	     */

	    if (IS_APP_MODALIZED(pCD))
	    {
		pcdFocus = NULL;
		focusWindow = ACTIVE_PSD->wmWorkspaceWin;
	    }
	    else
	    {
		focusWindow = pcdFocus->client;
	    }
	}
    }
    else
    {
	/*
	 * Set up the default (non client specific) keyboard input focus.
	 */

	if (wmGD.keyboardFocusPolicy == KEYBOARD_FOCUS_POINTER)
	{
	    focusWindow = PointerRoot;
	}
	else
	{
	    focusWindow = ACTIVE_PSD->wmWorkspaceWin;
	}
    }


    if ((pcdFocus != wmGD.keyboardFocus) || (flags & ALWAYS_SET_FOCUS))
    {
        if (pcdFocus)
	{
	    /*
	     * Set the focus and/or send a take focus client message.  This
	     * is not done if a client area button press was done to set
	     * set the focus and the window is a globally active input
	     * style window (See ICCCM).
	     */

	    if (!((flags & CLIENT_AREA_FOCUS) &&
		  (pcdFocus->protocolFlags & PROTOCOL_WM_TAKE_FOCUS) &&
		  !pcdFocus->inputFocusModel &&
		  (pcdFocus == pCD) &&
		  (wmGD.keyboardFocusPolicy == KEYBOARD_FOCUS_EXPLICIT)))
	    {
	        if (pcdFocus->protocolFlags & PROTOCOL_WM_TAKE_FOCUS)
	        {
	            SendClientMsg (pcdFocus->client, 
			(long) wmGD.xa_WM_PROTOCOLS,
		        (long) wmGD.xa_WM_TAKE_FOCUS, 
			focusTime, NULL, 0);
	        }

	        if (wmGD.enforceKeyFocus || pcdFocus->inputFocusModel ||
		    (pcdFocus->clientState == MINIMIZED_STATE) ||
		    !(pcdFocus->protocolFlags & PROTOCOL_WM_TAKE_FOCUS))
	        {
	            XSetInputFocus (DISPLAY, focusWindow, RevertToParent,
		        CurrentTime);
	        }
	    }
	}
	else
	{
	    /* !!! used timestamp based on SCREEN_SWITCH_FOCUS !!! */
	    XSetInputFocus (DISPLAY, focusWindow, RevertToPointerRoot,
		((flags & SCREEN_SWITCH_FOCUS) ? focusTime : CurrentTime));
	}

	wmGD.nextKeyboardFocus = pcdFocus;
    }


} /* END OF FUNCTION Do_Focus_Key */




/******************************<->*************************************
 *
 *  F_Next_Key (args, pCD, event)
 *
 *
 *  Description:
 *  -----------
 *  This is the window manager function handler for setting the keyboard
 *  input focus to the next window in the set of managed windows.
 *
 *
 *  Inputs:
 *  ------
 *  args = (immediate value) window type flags
 *
 *  pCD = pointer to the client data
 *
 *  event = X event that invoked the function (key, button, or menu/NULL)
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
Boolean F_Next_Key (args, pCD, event)
    String args;
    ClientData *pCD;
    XEvent *event;

#else /* _NO_PROTO */
Boolean F_Next_Key (String args, ClientData *pCD, XEvent *event)
#endif /* _NO_PROTO */
{
    Boolean focused = False;

    if (wmGD.keyboardFocusPolicy == KEYBOARD_FOCUS_EXPLICIT)
    {
	focused = FocusNextWindow ((unsigned long)args,
			 GetFunctionTimestamp ((XButtonEvent *)event));
    }

    return (True);

} /* END OF FUNCTION F_Next_Key */



/*************************************<->*************************************
 *
 *  F_Prev_Cmap (args, pCD, event)
 *
 *
 *  Description:
 *  -----------
 *  This is the window manager function handler installing the previous
 *  colormap in the list of client window colormaps.
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
Boolean F_Prev_Cmap (args, pCD, event)
    String args;
    ClientData *pCD;
    XEvent *event;

#else /* _NO_PROTO */
Boolean F_Prev_Cmap (String args, ClientData *pCD, XEvent *event)
#endif /* _NO_PROTO */
{
    if (pCD == NULL)
    {
	pCD = ACTIVE_PSD->colormapFocus;
    }

    if (pCD && (pCD->clientCmapCount > 0) &&
        ((pCD->clientState == NORMAL_STATE) ||
	 (pCD->clientState == MAXIMIZED_STATE)))
    {
	if (--(pCD->clientCmapIndex) < 0)
	{
	    pCD->clientCmapIndex = pCD->clientCmapCount - 1;
	}
	pCD->clientColormap = pCD->clientCmapList[pCD->clientCmapIndex];
	if (ACTIVE_PSD->colormapFocus == pCD)
	{
	    WmInstallColormap (ACTIVE_PSD, pCD->clientColormap);
	}
    }

    return (True);

} /* END OF FUNCTION F_Prev_Cmap */



/*************************************<->*************************************
 *
 *  F_Prev_Key (args, pCD, event)
 *
 *
 *  Description:
 *  -----------
 *  This is the window manager function handler for setting the keyboard
 *  input focus to the previous window in the set of managed windows.
 *
 *
 *  Inputs:
 *  ------
 *  args = (immediate value) window type flags
 *
 *  pCD = pointer to the client data
 *
 *  event = X event that invoked the function (key, button, or menu/NULL)
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
Boolean F_Prev_Key (args, pCD, event)
    String args;
    ClientData *pCD;
    XEvent *event;

#else /* _NO_PROTO */
Boolean F_Prev_Key (String args, ClientData *pCD, XEvent *event)
#endif /* _NO_PROTO */
{
    Boolean focused = False;

    if (wmGD.keyboardFocusPolicy == KEYBOARD_FOCUS_EXPLICIT)
    {
	focused = FocusPrevWindow ((unsigned long)args,
			    GetFunctionTimestamp ((XButtonEvent *)event));

    }

    return (True);

} /* END OF FUNCTION F_Prev_Key */



/*************************************<->*************************************
 *
 *  F_Pass_Key (args, pCD, event)
 *
 *
 *  Description:
 *  -----------
 *  This is a function stub for the f.pass_key window manager function.
 *
 *
 *  Inputs:
 *  ------
 *  args = (immediate value) window type flags
 *
 *  pCD = pointer to the client data
 *
 *  event = X event that invoked the function (key, button, or menu/NULL)
 *
 *************************************<->***********************************/

Boolean F_Pass_Key (args, pCD, event)
    String args;
    ClientData *pCD;
    XEvent *event;

{
    if (wmGD.passKeysActive)
    {
	/*
	 * Get out of pass keys mode.
	 */

	wmGD.passKeysActive = False;
	wmGD.passKeysKeySpec = NULL;
    }
    else
    {
	/*
	 * Get into pass keys mode.
	 */

	wmGD.passKeysActive = True;
    }

    return (False);

} /* END OF FUNCTION F_Pass_Key */



/*************************************<->*************************************
 *
 *  F_Maximize (args, pCD, event)
 *
 *
 *  Description:
 *  -----------
 *  This is the window manager function handler for maximizing a client
 *  window.
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
Boolean F_Maximize (args, pCD, event)
    String args;
    ClientData *pCD;
    XEvent *event;

#else /* _NO_PROTO */
Boolean F_Maximize (String args, ClientData *pCD, XEvent *event)
#endif /* _NO_PROTO */
{
    if (pCD && (pCD->clientFunctions & MWM_FUNC_MAXIMIZE))
    {
	SetClientState (pCD, MAXIMIZED_STATE,
	    GetFunctionTimestamp ((XButtonEvent *)event));
    }

    return (False);

} /* END OF FUNCTION F_Maximize */



/*************************************<->*************************************
 *
 *  F_Menu (args, pCD, event)
 *
 *
 *  Description:
 *  -----------
 *  This is the window manager function handler for posting a menu.
 *  This function can only be invoked by a key or button event.
 *   wmGD.menuUnpostKeySpec is assumed set appropriately; it will be set to
 *     NULL when the menu is unposted.
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
Boolean F_Menu (args, pCD, event)
    String args;
    ClientData *pCD;
    XEvent *event;

#else /* _NO_PROTO */
Boolean F_Menu (String args, ClientData *pCD, XEvent *event)
#endif /* _NO_PROTO */
{
    MenuSpec    *menuSpec;
    Context      menuContext;
    unsigned int button;
    int          x;
    int		 y;
    long         flags = POST_AT_XY;
    WmScreenData *pSD;


    if (event && 
	((event->type == ButtonPress) || (event->type == ButtonRelease)))
    {
        button = event->xbutton.button;
	x = event->xbutton.x_root;
	y = event->xbutton.y_root;
        if (event->type == ButtonRelease)
	{
	    flags |= POST_TRAVERSAL_ON;
	}
    }
    else if (event && 
	((event->type == KeyPress) || (event->type == KeyRelease)))
    {
        button = NoButton;
	x = event->xkey.x_root;
	y = event->xkey.y_root;
    }
    else
    {
	/*
	 * A button or key event must be used to post a menu using this 
	 * function.
	 */

	return (False);
    }

    if (pCD)
    {
	if (pCD->clientState == NORMAL_STATE)
	{
	    menuContext = F_CONTEXT_NORMAL;
	}
	else if (pCD->clientState == MAXIMIZED_STATE)
	{
	    menuContext = F_CONTEXT_MAXIMIZE;
	}
	else 
	{
	    menuContext = F_CONTEXT_ICON;
	}
	if (P_ICON_BOX(pCD) &&
            event->xany.window == ICON_FRAME_WIN(pCD))
	{
	    if (pCD->clientState == MINIMIZED_STATE)
	    {
		menuContext = F_SUBCONTEXT_IB_IICON;
	    }
	    else
	    {
		menuContext = F_SUBCONTEXT_IB_WICON;
	    }
	}
    }
    else
    {
	menuContext = F_CONTEXT_ROOT;
    }


    /* We do not add this MenuSpec to wmGD.acceleratorMenuSpecs.
     * This should have been done in MakeWmFunctionResources().
     */

    pSD = (pCD) ? PSD_FOR_CLIENT(pCD) : ACTIVE_PSD;
    if (menuSpec = MakeMenu (pSD, args, menuContext, 
			     menuContext, (MenuItem *) NULL, FALSE))
    {
        PostMenu (menuSpec, pCD, x, y, button, menuContext, flags, event);
    }

    return (False);

} /* END OF FUNCTION F_Menu */


/*************************************<->*************************************
 *
 *  F_Minimize (args, pCD, event)
 *
 *
 *  Description:
 *  -----------
 *  This is the window manager function handler for minimizing a client
 *  window.
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
Boolean F_Minimize (args, pCD, event)
    String args;
    ClientData *pCD;
    XEvent *event;

#else /* _NO_PROTO */
Boolean F_Minimize (String args, ClientData *pCD, XEvent *event)
#endif /* _NO_PROTO */
{
    ClientData *pcdLeader;


    if (pCD)
    {
	/*
	 * If the window is a transient then minimize the entire transient
	 * tree including the transient leader.
	 */
	
	pcdLeader = (pCD->transientLeader) ?
					FindTransientTreeLeader (pCD) : pCD;
	if (pcdLeader->clientFunctions & MWM_FUNC_MINIMIZE)
	{
	    SetClientState (pCD, MINIMIZED_STATE,
		GetFunctionTimestamp ((XButtonEvent *)event));
	}
    }

    return (False);

} /* END OF FUNCTION F_Minimize */



/*************************************<->*************************************
 *
 *  F_Move (args, pCD, event)
 *
 *
 *  Description:
 *  -----------
 *  This is the window manager function handler for moving a client window
 *  or icon.
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
Boolean F_Move (args, pCD, event)
    String args;
    ClientData *pCD;
    XEvent *event;

#else /* _NO_PROTO */
Boolean F_Move (String args, ClientData *pCD, XEvent *event)
#endif /* _NO_PROTO */
{
    if (pCD && (pCD->clientFunctions & MWM_FUNC_MOVE))
    {
	StartClientMove (pCD, event);
	HandleClientFrameMove (pCD, event);
    }

    return (False);

} /* END OF FUNCTION F_Move */



/*************************************<->*************************************
 *
 *  F_Next_Cmap (args, pCD, event)
 *
 *
 *  Description:
 *  -----------
 *  This is the window manager function handler installing the next
 *  colormap in the list of client window colormaps.
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
Boolean F_Next_Cmap (args, pCD, event)
    String args;
    ClientData *pCD;
    XEvent *event;

#else /* _NO_PROTO */
Boolean F_Next_Cmap (String args, ClientData *pCD, XEvent *event)
#endif /* _NO_PROTO */
{
    if (pCD == NULL)
    {
	pCD = ACTIVE_PSD->colormapFocus;
    }

    if (pCD && (pCD->clientCmapCount > 0) &&
        ((pCD->clientState == NORMAL_STATE) ||
	 (pCD->clientState == MAXIMIZED_STATE)))
    {
	if (++(pCD->clientCmapIndex) >= pCD->clientCmapCount)
	{
	    pCD->clientCmapIndex = 0;
	}
	pCD->clientColormap = pCD->clientCmapList[pCD->clientCmapIndex];
	if (ACTIVE_PSD->colormapFocus == pCD)
	{
	    WmInstallColormap (ACTIVE_PSD, pCD->clientColormap);
	}
    }

    return (True);

} /* END OF FUNCTION F_Next_Cmap */



/*************************************<->*************************************
 *
 *  F_Nop (args, pCD, event)
 *
 *
 *  Description:
 *  -----------
 *  This is the window manager function handler for doing nothing.
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
Boolean F_Nop (args, pCD, event)
    String args;
    ClientData *pCD;
    XEvent *event;

#else /* _NO_PROTO */
Boolean F_Nop (String args, ClientData *pCD, XEvent *event)
#endif /* _NO_PROTO */
{

    return (True);

} /* END OF FUNCTION F_Nop */



/*************************************<->*************************************
 *
 *  F_Normalize (args, pCD, event)
 *
 *
 *  Description:
 *  -----------
 *  This is the window manager function handler for putting a client window
 *  in the normal state.
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
Boolean F_Normalize (args, pCD, event)
    String args;
    ClientData *pCD;
    XEvent *event;

#else /* _NO_PROTO */
Boolean F_Normalize (String args, ClientData *pCD, XEvent *event)
#endif /* _NO_PROTO */
{

    if (pCD)
    {
	SetClientState (pCD, NORMAL_STATE,
	    GetFunctionTimestamp ((XButtonEvent *)event));
    }

    return (False);

} /* END OF FUNCTION F_Normalize */



/*************************************<->*************************************
 *
 *  F_Normalize_And_Raise (args, pCD, event)
 *
 *
 *  Description:
 *  -----------
 *  This is the window manager function handler for putting a client window
 *  in the normal state and raising it from and icon.
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
Boolean F_Normalize_And_Raise (args, pCD, event)
    String args;
    ClientData *pCD;
    XEvent *event;

#else /* _NO_PROTO */
Boolean F_Normalize_And_Raise (String args, ClientData *pCD, XEvent *event)
#endif /* _NO_PROTO */
{
    
    if (pCD)
    {
        if (pCD->clientState == MINIMIZED_STATE)
        {
            /* normalize window  */
            SetClientState (pCD, NORMAL_STATE,
                          (Time)
                          (event
                           ? GetFunctionTimestamp ((XButtonEvent *)event)
                           : GetTimestamp ()));
        }
        else
        {
            /* Raise the window and set the keyboard focus to the window */
            F_Raise (NULL, pCD, (XEvent *)NULL);
	    if (wmGD.raiseKeyFocus)
	    {
		F_Focus_Key (NULL, pCD,
			     (event 
			      ? ((XEvent *)event)
			      : ((XEvent *)NULL)));
	    }
        }
	wmGD.clickData.clickPending = False;
	wmGD.clickData.doubleClickPending = False;
    }

    return (False);

} /* END OF FUNCTION F_Normalize_And_Raise */



/*************************************<->*************************************
 *
 *  F_Pack_Icons (args, pCD, event)
 *
 *
 *  Description:
 *  -----------
 *  This is the window manager function handler for packing icons in the
 *  icon box or on the desktop.
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
Boolean F_Pack_Icons (args, pCD, event)
    String args;
    ClientData *pCD;
    XEvent *event;

#else /* _NO_PROTO */
Boolean F_Pack_Icons (String args, ClientData *pCD, XEvent *event)
#endif /* _NO_PROTO */
{
    
    IconBoxData *pIBD;

    if (ACTIVE_PSD->useIconBox && pCD)
    {
	pIBD = ACTIVE_WS->pIconBox;
	while (pCD != pIBD->pCD_iconBox)
	{
	   if (pIBD->pNextIconBox)
	   {
	       pIBD = pIBD->pNextIconBox;
	   }
	   else
	   {
	       pIBD = NULL;
	       break;
	   }
	}
	if (pIBD)
	{
	    PackIconBox (pIBD, False, False, 0, 0);
	}
	else
	{
	   PackRootIcons ();
	}
    }
    else
    {
	PackRootIcons ();
    }

    return (True);


} /* END OF FUNCTION F_Pack_Icons */



/*************************************<->*************************************
 *
 *  F_Post_SMenu (args, pCD, event)
 *
 *
 *  Description:
 *  -----------
 *  This is the window manager function handler for posting the system menu
 *  for the specified client.
 *  This function can only be invoked by a key or button event.
 *  wmGD.menuUnpostKeySpec is assumed set appropriately; it will be set to
 *    NULL when the menu is unposted.
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
Boolean F_Post_SMenu (args, pCD, event)
    String args;
    ClientData *pCD;
    XEvent *event;

#else /* _NO_PROTO */
Boolean F_Post_SMenu (String args, ClientData *pCD, XEvent *event)
#endif /* _NO_PROTO */
{
    Context menuContext;


    /*
     * An event must be used to post the system menu using this function.
     */

    if (event && pCD && pCD->systemMenuSpec)
    {
        /*
	 * Determine whether the keyboard is posting the menu and post
	 * the menu at an appropriate place.
         */

	if (pCD->clientState == NORMAL_STATE)
	{
	    menuContext = F_CONTEXT_NORMAL;
	}
	else if (pCD->clientState == MAXIMIZED_STATE)
	{
	    menuContext = F_CONTEXT_MAXIMIZE;
	}
	else 
	{
	    menuContext = F_CONTEXT_ICON;
	}
	if (P_ICON_BOX(pCD) &&
            event->xany.window == ICON_FRAME_WIN(pCD))
	{
	    if (pCD->clientState == MINIMIZED_STATE)
	    {
		menuContext = F_SUBCONTEXT_IB_IICON;
	    }
	    else
	    {
		menuContext = F_SUBCONTEXT_IB_WICON;
	    }
	}

	if ((event->type == KeyPress) || (event->type == KeyRelease))
	{
	    /*
	     * Set up for "sticky" menu processing if specified.
	     */

	    if (pCD->clientState == MINIMIZED_STATE ||
		menuContext == (F_SUBCONTEXT_IB_IICON | F_SUBCONTEXT_IB_WICON))
	    {
		if (wmGD.iconClick)
		{
		    wmGD.checkHotspot = True;
		}
	    }
	    else if (wmGD.systemButtonClick && (pCD->decor & MWM_DECOR_MENU))
	    {
		wmGD.checkHotspot = True;
	    }

	    PostMenu (pCD->systemMenuSpec, pCD, 0, 0, NoButton, menuContext,
		      0, event);
	}
	else if (event->type == ButtonPress)
	{
	    PostMenu (pCD->systemMenuSpec, pCD, 
		event->xbutton.x_root, event->xbutton.y_root,
	  	event->xbutton.button, menuContext, POST_AT_XY, event);
	}
	else if (event->type == ButtonRelease)
	{
	    PostMenu (pCD->systemMenuSpec, pCD, 
		event->xbutton.x_root, event->xbutton.y_root,
	  	event->xbutton.button, menuContext,
		POST_AT_XY | POST_TRAVERSAL_ON, event);
	}
    }

    return (False);

} /* END OF FUNCTION F_PostSMenu */



/*************************************<->*************************************
 *
 *  F_Kill (args, pCD, event)
 *
 *
 *  Description:
 *  -----------
 *  This is the window manager function handler for terminating a client.
 *  Essentially the client connection is shut down.
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
Boolean F_Kill (args, pCD, event)
    String args;
    ClientData *pCD;
    XEvent *event;

#else /* _NO_PROTO */
Boolean F_Kill (String args, ClientData *pCD, XEvent *event)
#endif /* _NO_PROTO */
{
    if (pCD && (pCD->clientFunctions & MWM_FUNC_CLOSE))
    {
	if (pCD->protocolFlags & PROTOCOL_WM_DELETE_WINDOW)
	{
	    /*
	     * The client wants to be notified, not killed.
	     */

	    SendClientMsg (pCD->client, (long) wmGD.xa_WM_PROTOCOLS,
		(long) wmGD.xa_WM_DELETE_WINDOW, CurrentTime, NULL, 0);
	}
	else if (pCD->protocolFlags & PROTOCOL_WM_SAVE_YOURSELF)
	{
	    /*
	     * Send a WM_SAVE_YOURSELF message and wait for a change to
	     * the WM_COMMAND property.
	     * !!! button and key input should be kept from the window !!!
	     */

	    if (AddWmTimer (TIMER_QUIT, (unsigned long) wmGD.quitTimeout, pCD))
	    {
	        SendClientMsg (pCD->client, (long) wmGD.xa_WM_PROTOCOLS,
		    (long) wmGD.xa_WM_SAVE_YOURSELF, CurrentTime, NULL, 0);

	        pCD->clientFlags |= CLIENT_TERMINATING;
	    }
	    else
	    {
		XKillClient (DISPLAY, pCD->client);
	    }
	}
	else
	{
	    XKillClient (DISPLAY, pCD->client);
	}
    }

    return (False);

} /* END OF FUNCTION F_Kill */



/*************************************<->*************************************
 *
 *  F_Refresh (args, pCD, event)
 *
 *
 *  Description:
 *  -----------
 *  This is the window manager function handler for causing all windows
 *  in the workspace to be redrawn.
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
Boolean F_Refresh (args, pCD, event)
    String args;
    ClientData *pCD;
    XEvent *event;

#else /* _NO_PROTO */
Boolean F_Refresh (String args, ClientData *pCD, XEvent *event)
#endif /* _NO_PROTO */
{
    Window win;

			 /* default background_pixmap is None */
    win = XCreateWindow (DISPLAY,
			 ACTIVE_ROOT, 0, 0,
	                 (unsigned int) DisplayWidth (DISPLAY, 
			     ACTIVE_SCREEN),
	                 (unsigned int) DisplayHeight (DISPLAY, 
			     ACTIVE_SCREEN),
	                 0, 
                         0,
	                 InputOutput,
                         CopyFromParent,
	                 0, 
			 (XSetWindowAttributes *)NULL);   

    XMapWindow (DISPLAY, win);
    XDestroyWindow (DISPLAY, win);
    XFlush (DISPLAY);

    return (True);

} /* END OF FUNCTION F_Refresh */



/*************************************<->*************************************
 *
 *  F_Resize (args, pCD, event)
 *
 *
 *  Description:
 *  -----------
 *  This is the window manager function handler for resizing a client window.
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
Boolean F_Resize (args, pCD, event)
    String args;
    ClientData *pCD;
    XEvent *event;

#else /* _NO_PROTO */
Boolean F_Resize (String args, ClientData *pCD, XEvent *event)
#endif /* _NO_PROTO */
{
    if (pCD && (pCD->clientFunctions & MWM_FUNC_RESIZE) &&
	((pCD->clientState == NORMAL_STATE) ||
					(pCD->clientState == MAXIMIZED_STATE)))
    {
	StartClientResize (pCD, event);
	HandleClientFrameResize (pCD, event);
    }

    return (False);

} /* END OF FUNCTION F_Resize */



/*************************************<->*************************************
 *
 *  F_Restart (args, pCD, event)
 *
 *
 *  Description:
 *  -----------
 *  This is the window manager function handler for restarting the window
 *  manager.
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
Boolean F_Restart (args, pCD, event)
    String args;
    ClientData *pCD;
    XEvent *event;

#else /* _NO_PROTO */
Boolean F_Restart (String args, ClientData *pCD, XEvent *event)
#endif /* _NO_PROTO */
{
    if (wmGD.showFeedback & WM_SHOW_FB_RESTART)
    {
	ConfirmAction (ACTIVE_PSD, RESTART_ACTION);
    }
    else
    {
	RestartWm (MWM_INFO_STARTUP_CUSTOM);
    }
    return (False);

} /* END OF FUNCTION F_Restart */



/*************************************<->*************************************
 *
 *  Do_Restart (dummy)
 *
 *
 *  Description:
 *  -----------
 *  Callback function for restarting the window manager.
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void Do_Restart (dummy)
Boolean dummy;
#else /* _NO_PROTO */
void Do_Restart (Boolean dummy)
#endif /* _NO_PROTO */
{
    RestartWm (MWM_INFO_STARTUP_CUSTOM);

} /* END OF FUNCTION Do_Restart */



/*************************************<->*************************************
 *
 *  RestartWm (startupFlags)
 *
 *
 *  Description:
 *  -----------
 *  Actually restarts the window manager.
 *
 *
 *  Inputs:
 *  ------
 *  startupFlags = flags to be put into the Wm_INFO property for restart.
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void RestartWm (startupFlags)
    long startupFlags;

#else /* _NO_PROTO */
void RestartWm (long startupFlags)
#endif /* _NO_PROTO */
{
    ClientListEntry *pNextEntry;
    int scr;


    for (scr=0; scr<wmGD.numScreens; scr++)
    {
	if(wmGD.Screens[scr].managed)
	{
	    
	    /*
	     * Set up the _MOTIF_WM_INFO property on the root window 
	     * to indicate a restart.
	     */
	    
	    SetMwmInfo (wmGD.Screens[scr].rootWindow, startupFlags, 0);
	    
	    
	    /*
	     * Unmap client windows and reparent them to the root window.
	     */
	    
	    pNextEntry = wmGD.Screens[scr].lastClient;
	    while (pNextEntry)
	    {
		if (pNextEntry->type == NORMAL_STATE)
		{
		    if (pNextEntry->pCD->clientFlags & CLIENT_WM_CLIENTS)
		    {
			if (pNextEntry->pCD->clientState != MINIMIZED_STATE)
			{
			    XUnmapWindow (DISPLAY, 
					  pNextEntry->pCD->clientFrameWin);
			}
		    }
		    else
		    {
			DeFrameClient (pNextEntry->pCD);
		    }
		}
		pNextEntry = pNextEntry->prevSibling;
	    }
	}
	
    }
    

    /*
     * This fixes restart problem when going from explicit focus to
     * pointer focus.  Window under pointer was not getting focus indication
     * until pointer was moved to new window, or out of and into the
     * window.
     */

    XSetInputFocus (DISPLAY, PointerRoot, RevertToPointerRoot, CurrentTime);
    XSync (DISPLAY, False);



    /*
     * Restart the window manager with the initial arguments plus
     * the restart settings.
     */

    execvp (*(wmGD.argv), wmGD.argv);

    Warning ("Cannot restart the window manager");
    Do_Quit_Mwm (True);



} /* END OF FUNCTION RestartWm */



/*************************************<->*************************************
 *
 *  DeFrameClient (pCD)
 *
 *
 *  Description:
 *  -----------
 *  Unmaps a client window (and client icon window) and reparents the
 *  window back to the root.
 *
 *
 *  Inputs:
 *  -------
 *  pCD = pointer to the client data for the window to be de-framed.
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void DeFrameClient (pCD)
    ClientData *pCD;

#else /* _NO_PROTO */
void DeFrameClient (ClientData *pCD)
#endif /* _NO_PROTO */
{
    int x, y;
    int xoff, yoff;
    XWindowChanges windowChanges;

    while (pCD)
    {
        if (pCD->clientState != MINIMIZED_STATE)
        {
	    XUnmapWindow (DISPLAY, pCD->clientFrameWin);
        }

        if (pCD->iconWindow && (pCD->clientFlags & ICON_REPARENTED))
        {
	    XUnmapWindow (DISPLAY, pCD->iconWindow);
	    XRemoveFromSaveSet (DISPLAY, pCD->iconWindow);
	    XReparentWindow (DISPLAY, pCD->iconWindow, 
		ROOT_FOR_CLIENT(pCD), pCD->iconX, pCD->iconY);
        }

        if (pCD->maxConfig)
        {
	    x = pCD->maxX;
	    y = pCD->maxY;
        }
        else
        {
	    if(wmGD.positionIsFrame)
	    {
		CalculateGravityOffset (pCD, &xoff, &yoff);
		x = pCD->clientX - xoff;
		y = pCD->clientY - yoff;
	    }

        }

#ifndef UNMAP_ON_RESTART
	if (pCD->clientState == MINIMIZED_STATE)
	{
	    XUnmapWindow (DISPLAY, pCD->client);
	}
#else
	XUnmapWindow (DISPLAY, pCD->client);
#endif
	XRemoveFromSaveSet (DISPLAY, pCD->client);
        XReparentWindow (DISPLAY, pCD->client, 
	    ROOT_FOR_CLIENT(pCD), x, y);

	if (pCD->transientChildren)
	{
	    DeFrameClient (pCD->transientChildren);
	}

	/*
	 * restore X border
	 */
	windowChanges.x = x;
	windowChanges.y = y;
	windowChanges.border_width = pCD->xBorderWidth;
	XConfigureWindow (DISPLAY, pCD->client, CWBorderWidth | CWX | CWY,
			  &windowChanges);

	if (pCD->transientLeader)
	{
	    pCD = pCD->transientSiblings;
	}
	else
	{
	    pCD = NULL;
	}
    }

} /* END OF FUNCTION DeFrameClient */



/******************************<->*************************************
 *
 *  F_Send_Msg (args, pCD, event)
 *
 *
 *  Description:
 *  -----------
 *  This is the window manager function handler for sending a client
 *  message event to a client window.
 *
 *
 *  Inputs:
 *  ------
 *  args = (immediate value) message id
 *
 *  pCD = pointer to the client data
 *
 *  event = X event that invoked the function (key, button, or menu/NULL)
 *
 *
 ******************************<->***********************************/

#ifdef _NO_PROTO
Boolean F_Send_Msg (args, pCD, event)
    String args;
    ClientData *pCD;
    XEvent *event;

#else /* _NO_PROTO */
Boolean F_Send_Msg (String args, ClientData *pCD, XEvent *event)
#endif /* _NO_PROTO */
{
    register int i;


    if (pCD && pCD->mwmMessagesCount)
    {
	/*
	 * A message id must be made "active" by being included in the
	 * _MWM_MESSAGES property before the associated message can be sent.
	 */

	for (i = 0; i < pCD->mwmMessagesCount; i++)
	{
	    if (pCD->mwmMessages[i] == (long)args)
	    {
		SendClientMsg (pCD->client, (long) wmGD.xa_MWM_MESSAGES, 
		    (long)args, CurrentTime, NULL, 0);
		return (True);
	    }
	}
    }

    return (True);

} /* END OF FUNCTION F_Send_Msg */



/*************************************<->*************************************
 *
 *  F_Separator (args, pCD, event)
 *
 *
 *  Description:
 *  -----------
 *  This is a placeholder function; it should never be called.
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
Boolean F_Separator (args, pCD, event)
    String args;
    ClientData *pCD;
    XEvent *event;

#else /* _NO_PROTO */
Boolean F_Separator (String args, ClientData *pCD, XEvent *event)
#endif /* _NO_PROTO */
{

    return (True);

} /* END OF FUNCTION F_Separator */



/*************************************<->*************************************
 *
 *  F_Raise (args, pCD, event)
 *
 *
 *  Description:
 *  -----------
 *  This is the window manager function handler for topping the client window
 *  so that it is unobscured.
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
Boolean F_Raise (args, pCD, event)
    String args;
    ClientData *pCD;
    XEvent *event;

#else /* _NO_PROTO */
Boolean F_Raise (String args, ClientData *pCD, XEvent *event)
#endif /* _NO_PROTO */
{
    ClientListEntry *pEntry;
    ClientListEntry *pNextEntry;
    ClientListEntry *pStackEntry;

    if (args)
    {
	pStackEntry = NULL;
	pNextEntry = ACTIVE_PSD->clientList;
	while (pNextEntry &&
	       (pEntry = FindClientNameMatch (pNextEntry, True, args,
			         F_GROUP_ALL)))
	{
	    pNextEntry = pEntry->nextSibling;
	    Do_Raise (pEntry->pCD, pStackEntry);
	    pStackEntry = pEntry;
	}
    }
    else if (pCD)
    {
	Do_Raise (pCD, (ClientListEntry *) NULL);
    }

    return (True);

} /* END OF FUNCTION F_Raise */



/*************************************<->*************************************
 *
 *  Do_Raise (pCD, pStackEntry)
 *
 *
 *  Description:
 *  -----------
 *  This is the window manager function handler for topping the client window
 *  so that it is unobscured.
 *
 *
 *  Inputs:
 *  ------
 *  pCD = pointer to the client data of the window (or icon) to be raised.
 * 
 *  pStackEntry = pointer to client list entry for window that is to be 
 *	above the raised window (if NULL window is raised to the top of the
 *	stack).
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void Do_Raise (pCD, pStackEntry)
    ClientData *pCD;
    ClientListEntry *pStackEntry;

#else /* _NO_PROTO */
void Do_Raise (ClientData *pCD, ClientListEntry *pStackEntry)
#endif /* _NO_PROTO */
{
    Boolean restackTransients;
    ClientData *pcdLeader;
    WmWorkspaceData *pWS = ACTIVE_WS;


    pcdLeader = (pCD->transientLeader) ? FindTransientTreeLeader (pCD) : pCD;

    if (wmGD.systemModalActive && (pcdLeader != wmGD.systemModalClient))
    {
	/*
	 * Don't raise the window above the system modal window.
	 */
    }
    else if ((pcdLeader->clientState == MINIMIZED_STATE) &&
	     !P_ICON_BOX(pcdLeader))
    {
	/*
	 * Only restack the icon if it is not currently raised.
	 */

	if (pStackEntry)
	{
	    if (pStackEntry->nextSibling != &pcdLeader->iconEntry)
	    {
	        StackWindow (pWS, &pcdLeader->iconEntry, False /*below*/,
			     pStackEntry);
	        MoveEntryInList (pWS, &pcdLeader->iconEntry, False /*below*/,
				 pStackEntry);
	    }
	}
	else
	{
	    if (ACTIVE_PSD->clientList != &pcdLeader->iconEntry)
	    {
	        StackWindow (pWS, &pcdLeader->iconEntry, 
		    True /*on top*/, (ClientListEntry *) NULL);
	        MoveEntryInList (pWS, &pcdLeader->iconEntry, 
		    True /*on top*/, (ClientListEntry *) NULL);
	    }
	}
    }
    else /* NORMAL_STATE, MAXIMIZED_STATE, adoption */
    {
	/*
	 * If this is a transient window then put it on top of its
	 * sibling transient windows.
	 */

	restackTransients = False;
	if (pCD->transientLeader)
	{
	    restackTransients = PutTransientOnTop (pCD);
	}

	/*
	 * Only restack the window or transient window tree if it is
	 * not currently on top.
	 */

	if (pStackEntry)
	{
	    if (pStackEntry->nextSibling != &pcdLeader->clientEntry)
	    {
		StackWindow (pWS, &pcdLeader->clientEntry, False /*below*/,
		    pStackEntry);
		MoveEntryInList (pWS, &pcdLeader->clientEntry, False /*below*/,
		    pStackEntry);
	    }
	    else if (restackTransients)
	    {
	        RestackTransients (pCD, False);
	    }
	}
	else
	{
	    if (ACTIVE_PSD->clientList != &pcdLeader->clientEntry)
	    {
		StackWindow (pWS, &pcdLeader->clientEntry, 
		    True /*on top*/, (ClientListEntry *) NULL);
	        MoveEntryInList (pWS, &pcdLeader->clientEntry, 
		    True /*on top*/, (ClientListEntry *) NULL);
	    }
	    else if (restackTransients)
	    {
	        RestackTransients (pCD, True);
	    }
	}
    }

} /* END OF FUNCTION Do_Raise */



/*************************************<->*************************************
 *
 *  F_Raise_Lower (args, pCD, event)
 *
 *
 *  Description:
 *  -----------
 *  This window manager function tops an obscured window or icon and bottoms 
 *  a window or icon that is on top of the window stack.
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
Boolean F_Raise_Lower (args, pCD, event)
    String args;
    ClientData *pCD;
    XEvent *event;

#else /* _NO_PROTO */
Boolean F_Raise_Lower (String args, ClientData *pCD, XEvent *event)
#endif /* _NO_PROTO */
{
    ClientData *pcdLeader;

    if (pCD)
    {
	pcdLeader = (pCD->transientLeader) ?
					FindTransientTreeLeader (pCD) : pCD;

	/*
	 * Treat a raise/lower on a window in a transient tree as if it is
	 * a raise/lower for the whole tree.
	 */

	if (CheckIfClientObscuredByAny (pcdLeader))
	{
	    /*
	     * The window is obscured by another window, raise the window.
	     */

	    F_Raise (NULL, pcdLeader, (XEvent *)NULL);
	}
	else if (CheckIfClientObscuringAny (pcdLeader))
	{
	    /*
	     * The window is obscuring another window, lower the window.
	     */

	    F_Lower (NULL, pcdLeader, (XEvent *)NULL);
	}
    }

    return (True);

} /* END OF FUNCTION F_Raise_Lower */



/*************************************<->*************************************
 *
 *  F_Refresh_Win (args, pCD, event)
 *
 *
 *  Description:
 *  -----------
 *  This is the window manager function handler for causing a client window
 *  to redisplay itself.
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
Boolean F_Refresh_Win (args, pCD, event)
    String args;
    ClientData *pCD;
    XEvent *event;

#else /* _NO_PROTO */
Boolean F_Refresh_Win (String args, ClientData *pCD, XEvent *event)
#endif /* _NO_PROTO */
{
    Window win;
    unsigned int w, h;

    if (pCD && ((pCD->clientState == NORMAL_STATE) ||
		(pCD->clientState == MAXIMIZED_STATE)))
    {
        if (pCD->clientState == NORMAL_STATE)
	{
	    w = (unsigned int) pCD->clientWidth;
	    h = (unsigned int) pCD->clientHeight;
	}
	else
	{
	    w = (unsigned int) pCD->maxWidth;
	    h = (unsigned int) pCD->maxHeight;
	}

			 /* default background_pixmap is None */
        win = XCreateWindow (DISPLAY,
		         pCD->clientBaseWin,
		         pCD->matteWidth,
		         pCD->matteWidth,
		         w, h,
	                 0, 
	                 0,
	                 InputOutput,
                         CopyFromParent,
	                 0, 
			 (XSetWindowAttributes *)NULL);  

        XMapWindow (DISPLAY, win);
        XDestroyWindow (DISPLAY, win);
        XFlush (DISPLAY);
    }

    return (True);

} /* END OF FUNCTION F_Refresh_Win */



/*************************************<->*************************************
 *
 *  F_Set_Behavior (args, pCD, event)
 *
 *
 *  Description:
 *  -----------
 *  This function is used to switch the window manager configuration between
 *  the built-in configuration (for CXI behavior) and the user's custom
 *  configuration.
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
Boolean F_Set_Behavior (args, pCD, event)
    String args;
    ClientData *pCD;
    XEvent *event;

#else /* _NO_PROTO */
Boolean F_Set_Behavior (String args, ClientData *pCD, XEvent *event)
#endif /* _NO_PROTO */
{
    /*
     * Go system modal in starting to do the set behavior.
     */

    /* !!! grab the server and the pointer !!! */


    /*
     * Confirm that a set_behavior should be done.
     * Execute restart if so.
     */

    if (wmGD.showFeedback & WM_SHOW_FB_BEHAVIOR)
    {
	ConfirmAction (ACTIVE_PSD, BEHAVIOR_ACTION);
    }
    else
    {
	RestartWm ((long) ((wmGD.useStandardBehavior) ?
			MWM_INFO_STARTUP_CUSTOM : MWM_INFO_STARTUP_STANDARD));
    }
    return (False);

} /* END OF FUNCTION F_Set_Behavior */



/*************************************<->*************************************
 *
 *  Do_Set_Behavior (dummy)
 *
 *
 *  Description:
 *  -----------
 *  Callback to do the f.set_behavior function.
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void Do_Set_Behavior (dummy)
Boolean dummy;
#else /* _NO_PROTO */
void Do_Set_Behavior (Boolean dummy)
#endif /* _NO_PROTO */
{
    RestartWm ((long) ((wmGD.useStandardBehavior) ?
			MWM_INFO_STARTUP_CUSTOM : MWM_INFO_STARTUP_STANDARD));

} /* END OF FUNCTION Do_Set_Behavior */



/*************************************<->*************************************
 *
 *  F_Title (args, pCD, event)
 *
 *
 *  Description:
 *  -----------
 *  This is a placeholder function; it should never be called.
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
Boolean F_Title (args, pCD, event)
    String args;
    ClientData *pCD;
    XEvent *event;

#else /* _NO_PROTO */
Boolean F_Title (String args, ClientData *pCD, XEvent *event)
#endif /* _NO_PROTO */
{

    return (True);

} /* END OF FUNCTION F_Title */



/*************************************<->*************************************
 *
 *  GetFunctionTimestamp (pEvent)
 *
 *
 *  Description:
 *  -----------
 *  This function is used to extract a timestamp from a key or button event.
 *  If the event passed in is not a key or button event then a timestamp
 *  is generated.
 *
 *
 *  Inputs:
 *  ------
 *  event = pointer to an X event
 *
 *
 *  Outputs:
 *  -------
 *  RETURN = a timestamp
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
Time GetFunctionTimestamp (pEvent)
    XButtonEvent *pEvent;

#else /* _NO_PROTO */
Time GetFunctionTimestamp (XButtonEvent *pEvent)
#endif /* _NO_PROTO */
{
    Time time;

    if (pEvent &&
	(((pEvent->type == ButtonPress) || (pEvent->type == ButtonRelease)) ||
	 ((pEvent->type == KeyPress) || (pEvent->type == KeyRelease))))
    {
	time = pEvent->time;
    }
    else
    {
	time = GetTimestamp ();
    }

    return (time);

} /* END OF FUNCTION GetFunctionTimestamp */


