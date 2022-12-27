#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)WmWinState.c	3.14 91/01/10";
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
#include "WmProtocol.h"

/*
 * include extern functions
 */

#include "WmCDecor.h"
#include "WmFunction.h"
#include "WmIDecor.h"
#include "WmIPlace.h"
#include "WmIconBox.h"
#include "WmKeyFocus.h"
#include "WmManage.h"
#include "WmProperty.h"
#include "WmWinInfo.h"
#include "WmWinList.h"


/*
 * Function Declarations:
 */

#ifdef _NO_PROTO
void	ConfigureNewState ();
void	MapClientWindows ();
void	SetClientState ();
void	SetClientWMState ();
void	SetupWindowState ();
void	ShowIconForMinimizedClient ();
void	UnmapClients ();
#else /* _NO_PROTO */
void SetClientState (ClientData *pCD, int newState, Time setTime);
void SetupWindowState (ClientData *pCD, int newState, Time setTime);
void ConfigureNewState (ClientData *pcd);
void UnmapClients (ClientData *pCD);
void SetClientWMState (ClientData *pCD, int wmState, int mwmState);
void MapClientWindows (ClientData *pCD);
void ShowIconForMinimizedClient (WmWorkspaceData *pWS, ClientData *pCD);
#endif /* _NO_PROTO */



/*
 * Global Variables:
 */
extern int firstTime;


/******************************<->*************************************
 *
 *  SetClientState (pCD, newState, setTime)
 *
 *
 *  Description:
 *  -----------
 *  This function is used to change the state of a client window (between
 *  withdrawn, normal, minimized, maximized).
 *
 *
 *  Inputs:
 *  ------
 *  pCD = This is a pointer to the window data for the window that
 *        is to have its state changed. The fields that are used
 *        are clientState, ...
 *
 *  newState = This is the state that the client window is to be changed to.
 *
 *  setTime = timestamp for state setting operations
 *
 * 
 *  Outputs:
 *  -------
 *  pCD.clientState = new client state
 *
 ******************************<->***********************************/

#ifdef _NO_PROTO
void SetClientState (pCD, newState, setTime)
	ClientData *pCD;
	int newState;
	Time setTime;

#else /* _NO_PROTO */
void SetClientState (ClientData *pCD, int newState, Time setTime)
#endif /* _NO_PROTO */
{
    ClientData *pcdLeader;
    int currentState;
    WmScreenData *pSD = PSD_FOR_CLIENT(pCD);

    currentState = pCD->clientState;
    if (currentState == newState)
    {
	/* no change in state */
	return;
    }


    /*
     * Undo the old state and setup the new state.  If this is a transient
     * window then insure that it is put in a state that is compatible
     * with its transient leader (e.g., it cannot be minimized separately).
     */

    pcdLeader = (pCD->transientLeader) ? FindTransientTreeLeader (pCD) : pCD;

    if (pCD->transientLeader)
    {
	if ((pcdLeader->clientState == MINIMIZED_STATE) &&
	    (newState != WITHDRAWN_STATE))
	{
	    newState = MINIMIZED_STATE;
	}
	else if ((newState == MINIMIZED_STATE) &&
		 (pcdLeader->clientState != MINIMIZED_STATE))
	{
	    if (currentState == WITHDRAWN_STATE)
	    {
		newState = NORMAL_STATE;
	    }
	    else
	    {
		newState = currentState;
	    }
	}
	if (newState == currentState)
	{
	    return;
	}
    }

    switch (newState)
    {

	case WITHDRAWN_STATE:
	{
	    /*
	     * Free window manager resources (frame and icon).  The
	     * WM_STATE property is set in WithdrawWindow.
	     */

	    UnManageWindow (pCD);
	    break;
	}

	case NORMAL_STATE:
	case MAXIMIZED_STATE:
	{
	    SetupWindowState (pCD, newState, setTime);
	    break;
	}

	case MINIMIZED_STATE:
	{
	    Boolean clientHasFocus;

	    /*
	     * Transient windows are minimized with the rest of the transient
	     * tree, including the transient leader.
	     */

	    if ((pCD->clientState == NORMAL_STATE) ||
		(pCD->clientState == MAXIMIZED_STATE))
	    {
		if ((wmGD.keyboardFocus == pCD) ||
		    (pCD->transientChildren && wmGD.keyboardFocus &&
		     (pCD == FindTransientTreeLeader (wmGD.keyboardFocus))))
		{
		    clientHasFocus = True;
		}
		else
		{
		    clientHasFocus = False;
		}

		if (clientHasFocus ||
		  ((wmGD.nextKeyboardFocus == pCD) ||
		   (pCD->transientChildren && wmGD.keyboardFocus &&
		    (pCD == FindTransientTreeLeader (wmGD.nextKeyboardFocus)))))
	    	{
		    /*
		     * Give up the keyboard focus when minimized (including
		     * the case in which an associated transient window has
		     * the focus).  Immediately remove the focus indication
		     * from the window being minimized.
		     */

		    if (wmGD.autoKeyFocus &&
			(wmGD.keyboardFocusPolicy == KEYBOARD_FOCUS_EXPLICIT))
		    {
			AutoResetKeyFocus (pcdLeader, setTime);
		    }
		    else
		    {
		        Do_Focus_Key (NULL, setTime, ALWAYS_SET_FOCUS);
		    }

		    if (clientHasFocus)
		    {
			SetKeyboardFocus (NULL, 0);
		    }
		}

		/* unmap main client and all transients */
		UnmapClients (pCD);
	    }

	    /*
	     * Display the icon for the minimized client.
	     */

	    if (ICON_FRAME_WIN(pCD)) 
	    {
		ShowIconForMinimizedClient (pSD->pActiveWS, pCD);
	    }

	    SetClientWMState (pCD, IconicState, MINIMIZED_STATE);

	    if ((pSD->useIconBox) && P_ICON_BOX(pCD))
	    {
		if ((pCD->clientFlags & ICON_BOX) && ACTIVE_ICON_TEXT_WIN)
		{
		    /*
		     * Hide active icon text window and reparent it to
		     * root
		     */
		    HideActiveIconText((WmScreenData *)NULL);
		    pSD->activeLabelParent = ACTIVE_ROOT;
		    XReparentWindow(DISPLAY, ACTIVE_ICON_TEXT_WIN , 
				ACTIVE_ROOT, 0, 0 );
		}
		if (ICON_FRAME_WIN(pCD))
		{
		    /* 
		     * force icon appearance in icon box to change 
		     */
		    IconExposureProc (pCD, True);
		}
	    }
	    break;
	}

    }

} /* END OF FUNCTION SetClientState */



/*************************************<->*************************************
 *
 *  SetupWindowState (pCD, newState, setTime)
 *
 *
 *  Description:
 *  -----------
 *  This function is used to setup a client window in the Normal or Maximized
 *  state.
 *
 *
 *  Inputs:
 *  ------
 *  pCD = This is a pointer to the window data for the window that
 *        is to have its state changed.
 *
 *  newState = This is the state that the client window is to be changed to.
 *
 *  setTime = timestamp for state setting operations
 *
 * 
 *  Outputs:
 *  -------
 *  pCD.clientState = new client state
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void SetupWindowState (pCD, newState, setTime)
	ClientData *pCD;
	int newState;
	Time setTime;

#else /* _NO_PROTO */
void SetupWindowState (ClientData *pCD, int newState, Time setTime)
#endif /* _NO_PROTO */
{
    int currentState;
    WmWorkspaceData *pWS = PSD_FOR_CLIENT(pCD)->pActiveWS;
    WmScreenData *pSD = PSD_FOR_CLIENT(pCD);

    currentState = pCD->clientState;

    /*
     * A transient window is not restored or maximized if the transient leader
     * is minimized.
     */

    if (newState == NORMAL_STATE)
    {
	if (pCD->maxConfig == True)
	{
	    /*
	     * The configuration function uses maxConfig to determine
	     * what the current configuration is (and then resets
	     * maxConfig) and uses the state paramenter to determine
	     * what the new configuration is.
	     */

	    ConfigureNewState (pCD); 
	}
    }
    else /* MAXIMIZED_STATE */
    {
	if (pCD->maxConfig == False)
	{
	    ConfigureNewState (pCD); 
        }
    }

    if (currentState == MINIMIZED_STATE)
    {
	Boolean clearIconFocus;

	/*
	 * give up keyboard focus 
	 */

	if ((wmGD.keyboardFocus == pCD) ||
	    (wmGD.nextKeyboardFocus == pCD))
	{
	    Do_Focus_Key (NULL, setTime, ALWAYS_SET_FOCUS);
	}

	if (wmGD.keyboardFocus == pCD)
	{
	    clearIconFocus = True;
	}
	else
	{
	    clearIconFocus = False;
	}

	/*
	 * The wm icon frame window and the client icon window
	 * (if it is being used) are mapped and the client window and
	 * client frame are unmapped.
	 */

	if (ICON_FRAME_WIN(pCD))
	{
	    if (pSD->useIconBox && P_ICON_BOX(pCD) && 
		!(pCD->clientFlags & ICON_BOX))
	    {
	        ShowClientIconState(pCD, newState);
	    }
	    else 
	    {
	        XUnmapWindow (DISPLAY, ICON_FRAME_WIN(pCD));
	        if (pCD->iconWindow)
	        {
		    XUnmapWindow (DISPLAY, pCD->iconWindow);
	        }
	        if ((wmGD.iconAutoPlace) && (ICON_PLACE(pCD) != NO_ICON_PLACE))
	        {
		    pWS->IPData.placeList[ICON_PLACE(pCD)].pCD = 
			NULL;
	        }
	    }

	    if (clearIconFocus)
	    {
		ClearFocusIndication (pCD, False /*no refresh*/);
		wmGD.keyboardFocus = NULL;
	    }
	}
    }
    if ((currentState != NORMAL_STATE) && (currentState != MAXIMIZED_STATE))
    {
	/*
	 * Note that maximized state is considered a NormalState in
	 * the ICCC.  SetClientWMState also sets the state in the
	 * client data.
	 */

	if (currentState == MINIMIZED_STATE)
	{
	    /*
	     * Raise the window(s) when they are deiconified.
	     */

	    pCD->clientState = newState;
	    F_Raise (NULL, pCD, NULL);
	}

	if ( (!(pCD->clientFlags & ICON_BOX)) || 
	     ((pCD->clientFlags & ICON_BOX) && (!(firstTime))) )
	{
	    MapClientWindows (pCD);
	}


	/*
	 * Set the WM_STATE property of the window and any associated
	 * transients, along with the clientState value.  The call
	 * is made with an indication of NORMAL_STATE to insure
	 * that transient window clientState values are setup
	 * correctly.  The top-level window clientState is set later.
	 */

	SetClientWMState (pCD, NormalState, NORMAL_STATE);
    }
    pCD->clientState = newState;

    if ((wmGD.keyboardFocusPolicy == KEYBOARD_FOCUS_EXPLICIT) &&
	(currentState == MINIMIZED_STATE) && wmGD.deiconifyKeyFocus)
    {
	ClientData *pcdFocus;

	pcdFocus = FindTransientFocus (pCD);
	if (pcdFocus)
	{
	    Do_Focus_Key (pcdFocus, setTime, ALWAYS_SET_FOCUS);
	}
    }

    if ( pSD->useIconBox &&  P_ICON_BOX(pCD) &&
	 (!(pCD->clientFlags & ICON_BOX)) && (ICON_FRAME_WIN(pCD)))
    {
	/* 
	 * force icon appearance in icon box to change 
	 */

	IconExposureProc (pCD, True);
    }

} /* END OF FUNCTION SetupWindowState */




/*************************************<->*************************************
 *
 *  ConfigureNewState (pcd)
 *
 *
 *  Description:
 *  -----------
 *  Configure the window to a new state
 *
 *
 *  Inputs:
 *  ------
 *  pcd		- pointer to client data
 * 
 *  Outputs:
 *  -------
 *
 *
 *  Comments:
 *  --------
 *  o This is only good for going between NORMAL and MAXIMIZED state.
 * 
 *************************************<->***********************************/

#ifdef _NO_PROTO
void ConfigureNewState (pcd)
	ClientData *pcd;

#else /* _NO_PROTO */
void ConfigureNewState (ClientData *pcd)
#endif /* _NO_PROTO */
{
    if (pcd->maxConfig)
    {
	pcd->maxConfig = FALSE;
	RegenerateClientFrame(pcd);
	XResizeWindow (DISPLAY, pcd->client,
			   (unsigned int) pcd->clientWidth, 
			   (unsigned int) pcd->clientHeight);
    }
    else
    {
	XResizeWindow (DISPLAY, pcd->client,
			   (unsigned int) pcd->maxWidth, 
			   (unsigned int) pcd->maxHeight);
	pcd->maxConfig = TRUE;
	RegenerateClientFrame(pcd);
    }
    SendConfigureNotify (pcd);

    /*
     * Force repaint if size doesn't change to update frame appearance.
     */

    if ((pcd->clientWidth == pcd->maxWidth) &&
        (pcd->clientHeight == pcd->maxHeight))
    {
	FrameExposureProc (pcd);
    }

} /* END OF FUNCTION ConfigureNewState */



/*************************************<->*************************************
 *
 *  UnmapClients (pCD)
 *
 *
 *  Description:
 *  -----------
 *  Unmap the window(s).  The indicated client may be the head of a transient
 *  tree - if it is unmap all windows in the transient tree.
 *
 *
 *  Inputs:
 *  ------
 *  pCD = pointer to client data of window(s) to be unmapped
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void UnmapClients (pCD)
	ClientData *pCD;

#else /* _NO_PROTO */
void UnmapClients (ClientData *pCD)
#endif /* _NO_PROTO */
{
    ClientData *pNext;

    pNext = pCD->transientChildren;
    while (pNext)
    {

	/* unmap all children first */
	if (pNext->transientChildren)
	{
	    UnmapClients (pNext);
	}

	/* then unmap all siblings at this level */
	XUnmapWindow (DISPLAY, pNext->clientFrameWin);
	XUnmapWindow (DISPLAY, pNext->client);
	pNext->wmUnmapCount++;
	pNext = pNext->transientSiblings;
    }

    /* unmap this primary window */
    XUnmapWindow (DISPLAY, pCD->clientFrameWin); 
    XUnmapWindow (DISPLAY, pCD->client);
    pCD->wmUnmapCount++;

} /* END OF FUNCTION UnmapClients */



/*************************************<->*************************************
 *
 *  SetClientWMState (pCD, wmState, mwmState)
 *
 *
 *  Description:
 *  -----------
 *  Set a new window manage state for a client window or a tree of transient
 *  client windows.
 *
 *  Inputs:
 *  ------
 *  pCD	= pointer to  client data
 *
 *  wmState = new state for WM_STATE property
 *
 *  mwmState = mwm client state
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void SetClientWMState (pCD, wmState, mwmState)
	ClientData *pCD;
	int wmState;
	int mwmState;

#else /* _NO_PROTO */
void SetClientWMState (ClientData *pCD, int wmState, int mwmState)
#endif /* _NO_PROTO */
{
    ClientData *pNext;

    pNext = pCD->transientChildren;
    while (pNext)
    {
	if (pNext->transientChildren)
	{
	    SetClientWMState (pNext, wmState, mwmState);
	}

	SetWMState (pNext->client, wmState, ICON_FRAME_WIN(pNext));
	if (pNext->maxConfig && mwmState == NORMAL_STATE)
	{
	    pNext->clientState = MAXIMIZED_STATE;
	}
	else
	{
	    pNext->clientState = mwmState;
	}
	pNext = pNext->transientSiblings;
    }

    SetWMState (pCD->client, wmState, ICON_FRAME_WIN(pCD));
    pCD->clientState = mwmState;

} /* END OF FUNCTION SetClientWMState */



/*************************************<->*************************************
 *
 *  MapClientWindows (pCD)
 *
 *
 *  Description:
 *  -----------
 *  Maps the window.  If this is a transient tree then all the windows in
 *  the transient tree are mapped.
 *
 *
 *  Inputs:
 *  ------
 *  pCD	= pointer to  client data
 * 
 *************************************<->***********************************/

#ifdef _NO_PROTO
void MapClientWindows (pCD)
	ClientData *pCD;

#else /* _NO_PROTO */
void MapClientWindows (ClientData *pCD)
#endif /* _NO_PROTO */
{
    ClientData *pNext;


    pNext = pCD->transientChildren;
    while (pNext)
    {
	/* map all transient children first */
	if (pNext->transientChildren)
	{
	    MapClientWindows (pNext);
	}

	/* then map all siblings at this level */
	XMapWindow (DISPLAY, pNext->client);
	XMapWindow (DISPLAY, pNext->clientFrameWin);

	pNext = pNext->transientSiblings;
    }

    /* map the primary window */
    XMapWindow (DISPLAY, pCD->client);
    XMapWindow (DISPLAY, pCD->clientFrameWin);

} /* END OF FUNCTION MapClientWindows */



/*************************************<->*************************************
 *
 *  ShowIconForMinimizedClient (pWS, pCD)
 *
 *
 *  Description:
 *  -----------
 *  This function shows the icon for the specified client.  If the icon
 *  is in an icon box then the "minimized" icon is displayed.  If the icon
 *  is on the root window it is mapped.
 * 
 *
 *  Inputs:
 *  ------
 *  pWS = pointer to workspace data
 *  pCD	= pointer to  client data
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void ShowIconForMinimizedClient (pWS, pCD)
	WmWorkspaceData *pWS;
	ClientData *pCD;
	
#else /* _NO_PROTO */
void ShowIconForMinimizedClient (WmWorkspaceData *pWS, ClientData *pCD)
#endif /* _NO_PROTO */
{
    WmScreenData *pSD = PSD_FOR_CLIENT(pCD);

    /*
     * Handle auto-placement for root icons (icons not in an icon
     * box).
     */
    if (wmGD.iconAutoPlace && !P_ICON_BOX(pCD))
    {
        if ((ICON_PLACE(pCD) == NO_ICON_PLACE) ||
	    ((pWS->IPData.placeList[ICON_PLACE(pCD)].pCD) &&
	     (pWS->IPData.placeList[ICON_PLACE(pCD)].pCD != pCD)))
        {
            /*
             * Icon place not defined or occupied by another client,
	     * find a free place to put the icon.
             */

	    if ((ICON_PLACE(pCD) = GetNextIconPlace (&pWS->IPData)) 
		== NO_ICON_PLACE)
	    {
	        ICON_PLACE(pCD) = 
		    CvtIconPositionToPlace (&pWS->IPData,
							 pCD->clientX,
			               	                 pCD->clientY);
	    }
	    CvtIconPlaceToPosition (&pWS->IPData, ICON_PLACE(pCD), 
				    &ICON_X(pCD), &ICON_Y(pCD));

	    XMoveWindow (DISPLAY, ICON_FRAME_WIN(pCD), 
		ICON_X(pCD), ICON_Y(pCD));

        }

        pWS->IPData.placeList[ICON_PLACE(pCD)].pCD = pCD;
    }

    if (pCD->iconWindow)
    {
        XMapWindow (DISPLAY, pCD->iconWindow);
    }

    if ((pSD->useIconBox) && P_ICON_BOX(pCD))
    {
        ShowClientIconState (pCD, MINIMIZED_STATE );
    }
    else
    {
	XWindowChanges windowChanges;

	/*
	 * Map the icon on the screen at the appropriate place in the 
	 * window stack.
	 */

	if (wmGD.lowerOnIconify)
	{
	    if ((&pCD->iconEntry != pSD->lastClient) &&
		(pSD->lastClient))
	    {
		if (pSD->lastClient->type == MINIMIZED_STATE)
		{
		    windowChanges.sibling = 
			ICON_FRAME_WIN(pSD->lastClient->pCD);
		}
		else
		{
		    windowChanges.sibling =
			pSD->lastClient->pCD->clientFrameWin;
		}
		windowChanges.stack_mode = Below;
		XConfigureWindow (DISPLAY, ICON_FRAME_WIN(pCD),
		    		  (CWSibling | CWStackMode), &windowChanges);
		MoveEntryInList (pWS, &pCD->iconEntry, 
		    False /*on bottom*/, NULL);
	    }
	}
	else
	{
	    windowChanges.sibling = pCD->clientFrameWin;
	    windowChanges.stack_mode = Below;
	    XConfigureWindow (DISPLAY, ICON_FRAME_WIN(pCD),
	    		      (CWSibling | CWStackMode), &windowChanges);
	    MoveEntryInList (pWS, &pCD->iconEntry, False /*below*/,
			     &pCD->clientEntry);
	}

	XMapWindow (DISPLAY, ICON_FRAME_WIN(pCD));
    }

} /* END OF FUNCTION ShowIconForMinimizedClient */


