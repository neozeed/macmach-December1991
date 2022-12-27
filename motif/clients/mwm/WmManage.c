#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)WmManage.c	3.14.1.4 91/04/01";
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
#include "WmICCC.h"
/*
 * include extern functions
 */
#include "WmCDecor.h"
#include "WmCEvent.h"
#include "WmColormap.h"
#include "WmError.h"
#include "WmEvent.h"
#include "WmFunction.h"
#include "WmGraphics.h"
#include "WmIDecor.h"
#include "WmIconBox.h"
#include "WmImage.h"
#include "WmKeyFocus.h"
#include "WmMenu.h"
#include "WmProperty.h"
#include "WmProtocol.h"
#include "WmWinInfo.h"
#include "WmWinList.h"
#include "WmWinState.h"



/*
 * Function Declarations:
 */

#ifdef _NO_PROTO
void	AdoptIntialClients ();
void	FreeClientFrame ();
void	FreeCustomMenuSpec ();
void	FreeIcon ();
void	ManageWindow ();
void    ReManageDialog ();
void	ResetWithdrawnFocii ();
void	UnManageWindow ();
void    WithdrawDialog ();
void	WithdrawTransientChildren ();
void	WithdrawWindow ();
#else /* _NO_PROTO */
void AdoptInitialClients (WmScreenData *pSD);
void ManageWindow (WmScreenData *pSD, Window clientWindow, long manageFlags);
void UnManageWindow (ClientData *pCD);
void WithdrawTransientChildren (ClientData *pCD);
void WithdrawWindow (ClientData *pCD);
void ResetWithdrawnFocii (ClientData *pCD);
void FreeClientFrame (ClientData *pCD);
void FreeIcon (ClientData *pCD);
void WithdrawDialog (Widget dialogboxW);
void ReManageDialog (WmScreenData *pSD, Widget dialogboxW);
#endif /* _NO_PROTO */


/*
 * Global Variables:
 */



/*************************************<->*************************************
 *
 *  AdoptInitialClients (pSD)
 *
 *  Inputs:
 *  -------
 *  pSD = pointer to screen data
 *
 *
 *  Description:
 *  -----------
 *  This function is called to find client windows that were mapped prior to 
 *  starting (or restarting) the window manager.  These windows are included
 *  in the set of windows managed by the window manager.
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void AdoptInitialClients (pSD)

    WmScreenData *pSD;
#else /* _NO_PROTO */
void AdoptInitialClients (WmScreenData *pSD)
#endif /* _NO_PROTO */
{
    Window  root;
    Window  parent;
    Window *clients;
    unsigned int     nclients;
    ClientData *pcd;
    PropWMState *wmStateProp;
    Boolean manageOnRestart;


    /*
     * Look for mapped top-level windows and start managing them:
     */

    if (XQueryTree (DISPLAY, pSD->rootWindow, &root, &parent, &clients,
	    &nclients))
    {
	int i;
	long manageFlags;

	for (i = 0; i < nclients; i++)
	{
	    /* determine if the client window should be managed by wm */
            if ((clients[i] == XtWindow (pSD->screenTopLevelW)) ||
		(clients[i] == XtWindow (pSD->pActiveWS->workspaceTopLevelW)) ||
		(clients[i] == pSD->activeIconTextWin))
            {
		/* don't manage ancillary window manager windows */
                continue;
	    }
	    if (!XFindContext (DISPLAY, clients[i], wmGD.windowContextType,
	        (caddr_t *)&pcd)) 
	    {
		/* don't manage a window we've already established a 
		   context for (e.g. icon windows) */
		continue;
	    }
	    if (!WmGetWindowAttributes (clients[i]))
            {
		/* can't access the window; ignore it */
		continue;
            }
	    /* window attributes are put into the global cache */

	    /*
	     * Get the window WM_STATE property value to determine the
	     * initial window state if the wm is being restarted.
	     */

	    manageFlags = MANAGEW_WM_STARTUP;
	    manageOnRestart = True;

	    if (wmGD.wmRestarted)
	    {
		manageFlags |= MANAGEW_WM_RESTART;
		if (wmStateProp = GetWMState (clients[i]))
		{
		    if (wmStateProp->state == IconicState)
		    {
			manageFlags |= MANAGEW_WM_RESTART_ICON;
		    }
		    else if (wmStateProp->state != NormalState)
		    {
			manageOnRestart = False;
		    }
		    XFree ((char *)wmStateProp);
		}
		else 
		{
		    manageOnRestart = False;
		}
	    }

	    /*
	     * Don't manage any override_redirect windows (mapped or not).
	     * Manage an unmapped window if it has a WM_STATE property
	     *   and it is not Withdrawn.
	     * Manage any window that is mapped.
	     */

	    if ((wmGD.windowAttributes.override_redirect != True) &&
		((wmGD.wmRestarted && manageOnRestart) ||
		 (wmGD.windowAttributes.map_state != IsUnmapped)))
	    {
		ManageWindow (pSD, clients[i], manageFlags);
	    }
	}

	if (nclients)
	{
	    XFree ((char *)clients);
	}
    }


} /* END OF FUNCTION AdoptInitialClients */



/*************************************<->*************************************
 *
 *  ManageWindow (pSD, clientWindow, manageFlags)
 *
 *
 *  Description:
 *  -----------
 *  This is the highlevel function that is used to include a window in
 *  the set of windows that are managed by the window manager.  The window
 *  gets reparented and decorated, gets an icon, is setup for window
 *  management event handling, etc.  Client windows that are controlled
 *  by the window manager (e.g., the icon box) are also managed with
 *  this function.
 *
 *
 *  Inputs:
 *  ------
 *  clientWindow = window of the client that we should manage
 *
 *  manageFlags	= additional control information 
 *
 * 
 *  Outputs:
 *  -------
 *  pCD = initialized client data
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void 
ManageWindow (pSD, clientWindow, manageFlags)
    WmScreenData *pSD;
    Window clientWindow;
    long manageFlags;

#else /* _NO_PROTO */
void 
ManageWindow (WmScreenData *pSD, Window clientWindow, long manageFlags)
#endif /* _NO_PROTO */
{
    ClientData *pCD;
    int initialState;
    int i;
    Boolean sendConfigNotify;

    /*
     * Get client information including window attributes and window
     * property values.  Use this information to determine how the window
     * is to be managed.
     */

    if (!(pCD = GetClientInfo (pSD, clientWindow, manageFlags)))
    {
	/* error getting client info; do not manage the client window */
	return;
    }


    if (manageFlags & MANAGEW_WM_RESTART)
    {
	if (manageFlags & MANAGEW_WM_RESTART_ICON)
	{
	    pCD->clientState = MINIMIZED_STATE;
	}
	else
	{
	    pCD->clientState = NORMAL_STATE;
	}
    }


    /*
     * Setup the initial placement of the client window.  Do interactive
     * placement if configured.
     */

    sendConfigNotify = InitClientPlacement (pCD, manageFlags);


    /*
     * Make a window frame for the client window and reparent the client
     * window.
     */

    if (!FrameWindow (pCD))
    {
	/*
	 * Error in framing the window; clean up the wm resources made
	 * up to this point for the client window. Do not manage the
	 * client window.
	 */

	UnManageWindow (pCD);
	return;
    }

    /*
     * Send config notify if the client's been moved/resized
     */
    if (sendConfigNotify)
    {
	SendConfigureNotify (pCD);
    }

    /*
     * Send client offset message if:
     *
     *   1. The client is interested.
     *   2. The position we report to the user is not the client's real
     *      position.
     *   3. There is a client offset to report.
     */
    if ((pCD->protocolFlags & PROTOCOL_MWM_OFFSET) &&
	(wmGD.positionIsFrame) && 
	((pCD->clientOffset.x != 0) ||
	 (pCD->clientOffset.y != 0)))
    { 
	SendClientOffsetMessage (pCD);
    }

    /*
     * Make an icon for the client window if it is not a valid transient
     * window.
     */

    if ((pCD->clientFunctions & MWM_FUNC_MINIMIZE) &&
        (pCD->transientLeader == NULL) && 
	  !MakeIcon (pCD->pSD->pActiveWS, pCD))
    {
	/*
	 * Error in making an icon for the client window; clean up the wm
	 * resources; do not manage the client window.
	 */

	UnManageWindow (pCD);
	return;
    }


    /*
     * Register window contexts to facilitate event handling:
     */

    XSaveContext (DISPLAY, pCD->clientFrameWin, wmGD.windowContextType,
	(caddr_t)pCD);

    XSaveContext (DISPLAY, pCD->clientBaseWin, wmGD.windowContextType,
	(caddr_t)pCD);

    if (DECOUPLE_TITLE_APPEARANCE(pCD) && pCD->clientTitleWin)
    {
	/* 
	 * handle exposures on title bar if it has its own appearance
	 */
	XSaveContext (DISPLAY, pCD->clientTitleWin, wmGD.windowContextType,
	    (caddr_t)pCD);
    }
    if (pCD->iconFrameWin)
    {
	XSaveContext (DISPLAY, pCD->iconFrameWin, wmGD.windowContextType,
	    (caddr_t)pCD);
    }

    if (pCD->clientCmapCount > 0)
    {
	for (i = 0; i < pCD->clientCmapCount; i++)
	{
	    if (pCD->cmapWindows[i] != pCD->client)
	    {
	        XSaveContext (DISPLAY, pCD->cmapWindows[i],
		    wmGD.windowContextType, (caddr_t)pCD);
	    }
	}
    }

    pCD->clientFlags |= CLIENT_CONTEXT_SAVED;


    /*
     * Setup button binding handling for actions that apply to the client
     * window.
     */

    if (BUTTON_SPECS(pCD))
    {
	SetupCButtonBindings (pCD->clientBaseWin, BUTTON_SPECS(pCD));
    }

    if (pCD->iconWindow && pCD->iconFrameWin)
    {
	XGrabButton (DISPLAY, AnyButton, AnyModifier, pCD->iconFrameWin, True,
	    ButtonPressMask | ButtonReleaseMask | ButtonMotionMask,
	    GrabModeAsync, GrabModeAsync, None, wmGD.workspaceCursor);
    }


    /*
     * Setup key binding handling for system menu accelerators.
     */

    if (pCD->systemMenuSpec &&
        (pCD->systemMenuSpec->accelKeySpecs))
    {
	SetupKeyBindings (pCD->systemMenuSpec->accelKeySpecs,
			  pCD->clientFrameWin, GrabModeSync, F_CONTEXT_ALL);
	if (!pCD->pIconBox && pCD->iconFrameWin)
	{
	    SetupKeyBindings (pCD->systemMenuSpec->accelKeySpecs,
			      pCD->iconFrameWin, GrabModeSync, F_CONTEXT_ALL);
	}
    }

    if (!pCD->pIconBox && pCD->iconFrameWin)
    {
	static int iconKeySpec = 1;
	static int iconAccelSpec = 1;

        if ((iconKeySpec != 0) && KEY_SPECS(pCD))
        {
	    iconKeySpec = SetupKeyBindings (KEY_SPECS(pCD), pCD->iconFrameWin,
				GrabModeSync, F_CONTEXT_ICON);
        }

        if ((iconAccelSpec != 0) && ACCELERATOR_MENU_COUNT(pCD))
        {
	    int n;

	    iconAccelSpec = 0;
	    for (n= 0; n < pSD->acceleratorMenuCount; n++)
	    {
	        iconAccelSpec += SetupKeyBindings (
			    ACCELERATOR_MENU_SPECS(pCD)[n]->accelKeySpecs,
			    pCD->iconFrameWin, GrabModeSync,
			    F_CONTEXT_ICON);
	    }
	}
    }


    /*
     * Setup keyboard focus handling if policy is "explicit".
     */

    if (wmGD.keyboardFocusPolicy == KEYBOARD_FOCUS_EXPLICIT)
    {
	DoExplicitSelectGrab (pCD->clientBaseWin);
    }


    /*
     * Make sure the client window has been reparented ...
     */

    if (!(manageFlags & MANAGEW_WM_CLIENTS))
    {
        XSync (DISPLAY, False);

        if (pCD->clientFlags & CLIENT_DESTROYED)
        {
	    UnManageWindow (pCD);
	    return;
        }
    }


    /*
     * Setup the initial display state for the client window:
     */

    initialState = pCD->clientState;
    pCD->clientState = WITHDRAWN_STATE;
    pCD->clientFlags &= ~WM_INITIALIZATION;

    AddClientToList (pSD->pActiveWS, pCD, True /*on top*/);
    SetClientState (pCD, initialState, CurrentTime);

    /*
     * Set the keyboard input focus to the newly managed window if appropriate:
     * - focus is automatically set only if the focus policy is explicit
     * - if there is a system modal window active then set the focus only
     *   if the new window is in the system modal heirarchy
     * - focus is automatically set if startupKeyFocus is selected or
     *   the new window is a system modal window or the current focus window
     *   has the new window as an application modal subordinate
     * - don't automatically set the focus if the window is minimized or
     *   is a window that generally doesn't take input
     */

    if ((wmGD.keyboardFocusPolicy == KEYBOARD_FOCUS_EXPLICIT) &&
	((pCD->inputMode == MWM_INPUT_SYSTEM_MODAL) ||
	 ((!wmGD.systemModalActive ||
	   (wmGD.systemModalClient == FindTransientTreeLeader (pCD))) &&
	  (wmGD.startupKeyFocus ||
	   (wmGD.keyboardFocus && (IS_APP_MODALIZED(wmGD.keyboardFocus)))) &&
	  !(manageFlags &
	    (MANAGEW_WM_STARTUP | MANAGEW_WM_RESTART | MANAGEW_WM_CLIENTS)) &&
	  (pCD->clientState != MINIMIZED_STATE) &&
	  (pCD->inputFocusModel ||
	   (pCD->protocolFlags & PROTOCOL_WM_TAKE_FOCUS)))))
    {
	Do_Focus_Key (pCD, CurrentTime , ALWAYS_SET_FOCUS);
    }
    else if ((pCD->inputMode == MWM_INPUT_SYSTEM_MODAL) ||
	     (wmGD.keyboardFocus && IS_APP_MODALIZED(wmGD.keyboardFocus)))
    {
	Do_Focus_Key ((ClientData *)NULL, CurrentTime , ALWAYS_SET_FOCUS);
    }


} /* END OF FUNCTION ManageWindow */



/*************************************<->*************************************
 *
 *  UnManageWindow (pCD)
 *
 *
 *  Description:
 *  -----------
 *  This function removes a top-level client window and it's transients
 *  from the set of windows that is managed by the window manager.  
 *
 *
 *  Inputs:
 *  ------
 *  pCD 	- pointer to client data of window to unmanage
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void UnManageWindow (pCD)
    ClientData *pCD;

#else /* _NO_PROTO */
void UnManageWindow (ClientData *pCD)
#endif /* _NO_PROTO */
{
    /*
     * If this is a transient window, then delete it from the leader's
     * list of transients.
     */

    if (pCD->transientLeader)
    {
	DeleteTransient (pCD);
    }


    /*
     * Withdraw all the transient children of this window.
     */

    if (pCD->transientChildren != NULL) 
    {
	WithdrawTransientChildren (pCD);
    }


    /*
     * Withdraw this window
     */

    WithdrawWindow (pCD);

} /* END OF FUNCTION UnManageWindow */



/*************************************<->*************************************
 *
 *  WithdrawTransientChildren (pCD)
 *
 *
 *  Description:
 *  -----------
 *  This function withdraws all transient children of the specified window.
 *
 *
 *  Inputs:
 *  ------
 *  pCD = pointer to client data of the leader of the transient tree.
 * 
 *************************************<->***********************************/

#ifdef _NO_PROTO
void WithdrawTransientChildren (pCD)
    ClientData *pCD;

#else /* _NO_PROTO */
void WithdrawTransientChildren (ClientData *pCD)
#endif /* _NO_PROTO */
{
    ClientData *pcdNext;
    ClientData *pcdThis;


    pcdNext = pCD->transientChildren;
    while (pcdNext)
    {
	if (pcdNext->transientChildren)
	{
	    WithdrawTransientChildren (pcdNext);
	}
	pcdThis = pcdNext;
	pcdNext = pcdThis->transientSiblings;
	WithdrawWindow (pcdThis);
    }

} /* END OF FUNCTION WithdrawTransientChildren */



/*************************************<->*************************************
 *
 *  WithdrawWindow (pCD)
 *
 *
 *  Description:
 *  -----------
 *  This function removes a top-level client window from the set of windows
 *  that is managed by the window manager.  All window manager resources
 *  associtated with the client window are freed up (possibly cached for
 *  reuse).  Any custom system menu is destroyed.
 *
 *
 *  Inputs:
 *  ------
 *  pCD 	- pointer to client data of window to withdraw
 * 
 *************************************<->***********************************/

#ifdef _NO_PROTO
void WithdrawWindow (pCD)
    ClientData *pCD;

#else /* _NO_PROTO */
void WithdrawWindow (ClientData *pCD)
#endif /* _NO_PROTO */
{
    int x;
    int y;
    int i;
    XWindowChanges xwc;


    /*
     * Put the client window into a withdrawn state:
     *
     * - remove the icon/client window from the screen
     * - make sure the input focus no longer is associted with the window
     * - free the icon placement (if necessary)
     */

    if (!(pCD->clientFlags & WM_INITIALIZATION))
    {
	if (!pCD->transientLeader)
	{
	    DeleteClientFromList (pCD->pSD->pActiveWS, pCD);
	}
	ResetWithdrawnFocii (pCD);
	if (pCD->clientState & MINIMIZED_STATE)
	{
	    if (wmGD.iconAutoPlace && (!(P_ICON_BOX(pCD))))
	    {
		if (ICON_PLACE(pCD) != NO_ICON_PLACE)
		{
		pCD->pSD->pActiveWS->IPData.placeList[ICON_PLACE(pCD)].pCD 
		    = NULL;
		}
	    }
	    if (ICON_FRAME_WIN(pCD))
	    {
		XUnmapWindow (DISPLAY, ICON_FRAME_WIN(pCD));
	    }
	    XFlush (DISPLAY);
	}
	else if ((pCD->clientState == NORMAL_STATE) ||
		 (pCD->clientState == MAXIMIZED_STATE))
	{
	    XUnmapWindow (DISPLAY, pCD->clientFrameWin);
	    XFlush (DISPLAY);
	}
    }

    /*
     * Check to see if the window is being unmanaged because the window
     * was destroyed.
     */

    if (!(pCD->clientFlags & CLIENT_DESTROYED))
    {
	XEvent eventReturn;

	if (XCheckTypedWindowEvent (DISPLAY, pCD->clientBaseWin, DestroyNotify,
	        &eventReturn))
	{
	    pCD->clientFlags |= CLIENT_DESTROYED;
	}
    }


    /*
     * Reparent the client window back to root if the window has been
     * reparented by the window manager.  Remove the window from the
     * window managers save-set if necessary.
     */

    if ((pCD->clientFlags & CLIENT_REPARENTED) &&
        !(pCD->clientFlags & CLIENT_DESTROYED))
    {
	SetWMState (pCD->client, WithdrawnSTATE, ICON_FRAME_WIN(pCD));

	if (pCD->maxConfig)
	{
	    x = pCD->maxX;
	    y = pCD->maxY;
	}
	else
	{
	    x = pCD->clientX;
	    y = pCD->clientY;
	    if (wmGD.positionIsFrame)
	    {
		x -= pCD->clientOffset.x;
		y -= pCD->clientOffset.y;
	    }
	}

	XUnmapWindow (DISPLAY, pCD->client);
	XReparentWindow (DISPLAY, pCD->client, ROOT_FOR_CLIENT(pCD), x, y);

	/* give the window back it's X border */
	xwc.border_width = pCD->xBorderWidth;
	XConfigureWindow(DISPLAY, pCD->client, CWBorderWidth, &xwc);

	if (pCD->iconWindow && (pCD->clientFlags & ICON_REPARENTED))
	{
	    XUnmapWindow (DISPLAY, pCD->iconWindow);
	    XReparentWindow (DISPLAY, pCD->iconWindow, ROOT_FOR_CLIENT(pCD), 
			     ICON_X(pCD), ICON_Y(pCD));
	}
    }


    if ((pCD->clientFlags & CLIENT_IN_SAVE_SET) &&
        !(pCD->clientFlags & CLIENT_DESTROYED))
    {
	XRemoveFromSaveSet (DISPLAY, pCD->client);

	if (pCD->iconWindow && (pCD->clientFlags & ICON_IN_SAVE_SET))
	{
	    XRemoveFromSaveSet (DISPLAY, pCD->iconWindow);
	}
    }

    /*
     * Free a custom system menu if one was created.
     */

    FreeCustomMenuSpec (pCD->systemMenuSpec);

    /*
     * Free the client window frame:
     */

    if (pCD->clientFrameWin)
    {
	FreeClientFrame (pCD);
    }


    /*
     * Free the icon associated with the client window:
     */

    if ((pCD->iconFlags & ICON_HINTS_PIXMAP) && pCD->iconPixmap)
    {
	XFreePixmap (DISPLAY, pCD->iconPixmap);
    }

    if (ICON_FRAME_WIN(pCD))
    {
        FreeIcon (pCD);
    }


    /*
     * Free up the client protocol list:
     */

    if (pCD->clientProtocols)
    {
	free ((char *)pCD->clientProtocols);
    }


    /*
     * Free up the mwm messages list:
     */

    if (pCD->mwmMessages)
    {
	free ((char *)pCD->mwmMessages);
    }


    /*
     * Delete client window manager timers:
     */

    DeleteClientWmTimers (pCD);


    /*
     * Free up window context associations.  The context for the client
     * window is always set if there is a client data structure.
     */

    XDeleteContext (DISPLAY, pCD->client, wmGD.windowContextType);
    if (pCD->clientFlags & CLIENT_CONTEXT_SAVED)
    {
	XDeleteContext (DISPLAY, pCD->clientFrameWin, wmGD.windowContextType);
	XDeleteContext (DISPLAY, pCD->clientBaseWin, wmGD.windowContextType);
	if (DECOUPLE_TITLE_APPEARANCE(pCD))
	{
	    XDeleteContext (DISPLAY, pCD->clientTitleWin,
		wmGD.windowContextType);
	}
	if (ICON_FRAME_WIN(pCD)) 
	{
	    XDeleteContext (DISPLAY, pCD->iconFrameWin, 
	                     wmGD.windowContextType);
	}
    }


    /*
     * Free up window manager resources:
     */

    if (!(pCD->clientFlags & CLIENT_WM_CLIENTS))
    {
        if (pCD->clientName)
        {
	    XFree ((char *) (pCD->clientName));
        }
	if (pCD->clientClass)
	{
	    XFree ((char *) (pCD->clientClass));
	}
    }

    if ((pCD->clientFlags & CLIENT_HINTS_TITLE) && pCD->clientTitle)
    {
	XmStringFree (pCD->clientTitle);
    }

    if ((pCD->iconFlags & ICON_HINTS_TITLE) && pCD->iconTitle)
    {
	XmStringFree (pCD->iconTitle);
    }

    if (pCD->clientCmapCount > 0)
    {
	for (i = 0; i < pCD->clientCmapCount; i++)
	{
	    if (pCD->cmapWindows[i] != pCD->client)
	    {
		XDeleteContext (DISPLAY, pCD->cmapWindows[i],
		    wmGD.windowContextType);
	    }
	}
	free ((char *) (pCD->cmapWindows));
	free ((char *) (pCD->clientCmapList));
    }


    /*
     * Clean up references to this data before we free it.
     */
    if (wmGD.menuClient == pCD) {
	wmGD.menuClient = NULL;
    }

    if (wmGD.gadgetClient == pCD) {
	wmGD.gadgetClient = NULL;
	wmGD.gadgetDepressed = NULL;
    }

    free (pCD);


} /* END OF FUNCTION WithdrawWindow */



/*************************************<->*************************************
 *
 *  ResetWitdrawnFocii (pCD)
 *
 *
 *  Description:
 *  -----------
 *  This function resets the various types of focus if they are set to a
 *  window being withdrawn.
 *  
 *
 *
 *  Inputs:
 *  ------
 *  pCD 	- pointer to client data 
 * 
 *  Outputs:
 *  -------
 *
 *  Comments:
 *  --------
 * 
 *************************************<->***********************************/

#ifdef _NO_PROTO
void ResetWithdrawnFocii (pCD)
    ClientData *pCD;

#else /* _NO_PROTO */
void ResetWithdrawnFocii (ClientData *pCD)
#endif /* _NO_PROTO */
{
    if (wmGD.keyboardFocus == pCD)
    {
	if (wmGD.autoKeyFocus &&
	    (wmGD.keyboardFocusPolicy == KEYBOARD_FOCUS_EXPLICIT))
	{
	    AutoResetKeyFocus (pCD, CurrentTime);
	}
	else
	{
	    /*
	     * Set the focus to the default state if the focus is not in
	     * the process of being set (i.e. a FocusIn event will be 
	     * comming along shortly.
	     */

	    if (wmGD.nextKeyboardFocus == wmGD.keyboardFocus)
	    {
	        Do_Focus_Key ((ClientData *)NULL, CurrentTime,
		    ALWAYS_SET_FOCUS);
	    }
	}
	SetKeyboardFocus ((ClientData *)NULL, 0);
    }

    if ((pCD->inputMode == MWM_INPUT_PRIMARY_APPLICATION_MODAL) &&
	(wmGD.keyboardFocusPolicy == KEYBOARD_FOCUS_POINTER) &&
	(wmGD.keyboardFocus == NULL) &&
	(wmGD.nextKeyboardFocus == NULL))
    {
	RepairFocus ();
    }

    if (wmGD.nextKeyboardFocus == pCD)
    {
	wmGD.nextKeyboardFocus = NULL;
    }

    if (ACTIVE_PSD->colormapFocus == pCD)
    {
	SetColormapFocus (ACTIVE_PSD, (ClientData *)NULL);
    }

} /* END OF FUNCTION ResetWithdrawnFocii */



/*************************************<->*************************************
 *
 *  FreeClientFrame (pCD)
 *
 *
 *  Description:
 *  -----------
 *  This function frees up frame windows and associated resources.
 *
 *
 *  Inputs:
 *  ------
 *  pCD		- pointer to client data
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void FreeClientFrame (pCD)
    ClientData *pCD;

#else /* _NO_PROTO */
void FreeClientFrame (ClientData *pCD)
#endif /* _NO_PROTO */
{
    if (pCD->pclientTopShadows) {
	FreeRList (pCD->pclientTopShadows);
	pCD->pclientTopShadows = NULL;
    }
    if (pCD->pclientBottomShadows) {
	FreeRList (pCD->pclientBottomShadows);
	pCD->pclientBottomShadows = NULL;
    }
    if (pCD->pclientTitleTopShadows) {
	FreeRList (pCD->pclientTitleTopShadows);
	pCD->pclientTitleTopShadows = NULL;
    }
    if (pCD->pclientTitleBottomShadows) {
	FreeRList (pCD->pclientTitleBottomShadows);
	pCD->pclientTitleBottomShadows = NULL;
    }
    if (pCD->pclientMatteTopShadows) {
	FreeRList (pCD->pclientMatteTopShadows);
	pCD->pclientMatteTopShadows = NULL;
    }
    if (pCD->pclientMatteBottomShadows) {
	FreeRList (pCD->pclientMatteBottomShadows);
	pCD->pclientMatteBottomShadows = NULL;
    }
    if (pCD->pTitleGadgets) {
	free (pCD->pTitleGadgets);
	pCD->pTitleGadgets = NULL;
	pCD->cTitleGadgets = 0;
    }
    if (pCD->pResizeGadgets) {
	free (pCD->pResizeGadgets);
	pCD->pResizeGadgets = NULL;
    }

    /* destroy frame window & all children */
    XDestroyWindow (DISPLAY, pCD->clientFrameWin);

} /* END OF FUNCTION FreeClientFrame */



/*************************************<->*************************************
 *
 *  FreeIcon (pCD)
 *
 *
 *  Description:
 *  -----------
 *  This function frees up icon windows and associated resources.
 *
 *
 *  Inputs:
 *  ------
 *  pCD		- pointer to client data
 * 
 *************************************<->***********************************/

#ifdef _NO_PROTO
void FreeIcon (pCD)
    ClientData *pCD;

#else /* _NO_PROTO */
void FreeIcon (ClientData *pCD)
#endif /* _NO_PROTO */
{

    if (pCD->piconTopShadows) {
	FreeRList (pCD->piconTopShadows);
	pCD->piconTopShadows = NULL;
    }
    if (pCD->piconBottomShadows) {
	FreeRList (pCD->piconBottomShadows);
	pCD->piconBottomShadows = NULL;
    }

    /* 
     * destroy frame window & all children 
     */

    if (pCD->pSD->useIconBox && P_ICON_BOX(pCD))
    {
	DeleteIconFromBox (pCD->pSD->pActiveWS->pIconBox, pCD);
    }
    else
    {
	XDestroyWindow (DISPLAY, pCD->iconFrameWin);
    }

} /* END OF FUNCTION FreeIcon */




/*************************************<->*************************************
 *
 *  WithdrawDialog (dialogboxW)
 *
 *
 *  Description:
 *  -----------
 *  This function removes a DialogBox widget "client" from the set of windows 
 *  that are managed by the window manager.
 *
 *
 *  Inputs:
 *  ------
 *  dialogboxW = DialogBox widget to withdraw.
 * 
 *  Comments:
 *  --------
 *  Does not maintain the WM_STATE property on the dialog "client".
 * 
 *************************************<->***********************************/

#ifdef _NO_PROTO
void WithdrawDialog (dialogboxW)

    Widget   dialogboxW;

#else /* _NO_PROTO */
void WithdrawDialog (Widget dialogboxW)
#endif /* _NO_PROTO */
{
    ClientData *pCD;

    /*
     * Get the dialog shell window client data.
     */

    XFindContext (DISPLAY, XtWindow (XtParent (dialogboxW)),
		  wmGD.windowContextType, (caddr_t *)&pCD);

    XtUnmanageChild (dialogboxW);
    DeleteClientFromList (ACTIVE_WS, pCD);
    ResetWithdrawnFocii (pCD);
    XUnmapWindow (DISPLAY, pCD->clientFrameWin);

} /* END OF FUNCTION WithdrawDialog */



/*************************************<->*************************************
 *
 *  ReManageDialog (pSD, dialogboxW)
 *
 *
 *  Description:
 *  -----------
 *  This function remanages a DialogBox "client" that was unmanaged via 
 *  WithdrawDialog ().
 *
 *
 *  Inputs:
 *  ------
 *  pSD = pointer to screen data
 *  dialogboxW = DialogBox widget to remanage.
 *
 * 
 *  Outputs:
 *  -------
 *  Does not maintain the WM_STATE property on the dialog "client".
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void ReManageDialog (pSD, dialogboxW)

    WmScreenData *pSD;
    Widget   dialogboxW;

#else /* _NO_PROTO */
void ReManageDialog (WmScreenData *pSD, Widget dialogboxW)
#endif /* _NO_PROTO */
{
    ClientData *pCD;

    /*
     * Get the dialog shell window client data.
     */

    XFindContext (DISPLAY, XtWindow (XtParent (dialogboxW)),
		  wmGD.windowContextType, (caddr_t *)&pCD);

    /*
     * The order is important here:
     */


    if (pSD->clientList)
    {
      StackWindow (pSD->pActiveWS, &pCD->clientEntry,
                    TRUE, (ClientListEntry *) NULL);
    }
    AddClientToList (pSD->pActiveWS, pCD, True /*on top*/);
    XMapWindow (DISPLAY, pCD->clientFrameWin);
    XtManageChild (dialogboxW);

    if ((wmGD.keyboardFocusPolicy == KEYBOARD_FOCUS_EXPLICIT))
    {
	Do_Focus_Key (pCD, CurrentTime , ALWAYS_SET_FOCUS);
    }


} /* END OF FUNCTION ReManageDialog */

