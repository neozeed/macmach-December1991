#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)WmCEvent.c	3.17.1.5 91/03/27";
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

#include <X11/Xatom.h>
#include <Xm/Traversal.h>  /* just for _XmGrabTheFocus declaration */

/*
 * include extern functions
 */
#include "WmCEvent.h"
#include "WmCDecor.h"
#include "WmColormap.h"
#include "WmEvent.h"
#include "WmFeedback.h"
#include "WmFunction.h"
#include "WmIDecor.h"
#include "WmKeyFocus.h"
#include "WmManage.h"
#include "WmMenu.h"
#include "WmProperty.h"
#include "WmProtocol.h"
#include "WmWinConf.h"
#include "WmWinInfo.h"
#include "WmWinList.h"
#include "WmWinState.h"



/*
 * Global Variables:
 */

extern unsigned int buttonModifierMasks[];



/*************************************<->*************************************
 *
 *  SetupCButtonBindings (window, buttonSpecs)
 *
 *
 *  Description:
 *  -----------
 *  This function sets up the event handling necessary to support user
 *  specified button bindings for window manager functions that apply to
 *  the client window.
 *
 *
 *  Inputs:
 *  ------
 *  window = grab window id
 *
 *  buttonSpecs = list of button bindings for window manager functions
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void SetupCButtonBindings (window, buttonSpecs)
    Window window;
    ButtonSpec *buttonSpecs;

#else /* _NO_PROTO */
void SetupCButtonBindings (Window window, ButtonSpec *buttonSpecs)
#endif /* _NO_PROTO */
{
    ButtonSpec *buttonSpec;
    unsigned int eventMask;
    unsigned int grabState;


    /*
     * If the context of the button binding includes "window" do button
     * grabs to get the button events that invoke window manger functions.
     * !!! don't do redundant grabs !!!
     */

    buttonSpec = buttonSpecs;
    while (buttonSpec)
    {
	if ((buttonSpec->context & F_CONTEXT_WINDOW) &&
	    (buttonSpec->subContext & F_SUBCONTEXT_W_CLIENT))
	{
	    eventMask = ButtonMotionMask | ButtonReleaseMask;

	    if (buttonSpec->eventType == ButtonRelease)
	    {
		/*
		 * Don't include the button down in the grab state.
		 */

		grabState = buttonSpec->state &
				~(buttonModifierMasks[buttonSpec->button]);
	    }
	    else
	    {
		grabState = buttonSpec->state;
	    }

	    XGrabButton (DISPLAY, buttonSpec->button, grabState,
	        window, False, eventMask, GrabModeSync,
	        GrabModeAsync, None, wmGD.workspaceCursor);

	    XGrabButton (DISPLAY, buttonSpec->button, (grabState | LockMask),
	        window, False, eventMask, GrabModeSync,
	        GrabModeAsync, None, wmGD.workspaceCursor);
	}
	/*
	 * If the window context is not "window" a general grab is not
	 * necessary.
	 */

	buttonSpec = buttonSpec->nextButtonSpec;
    }

} /* END OF FUNCTION SetupCButtonBindings */



/*************************************<->*************************************
 *
 *  WmDispatchClientEvent (event)
 *
 *
 *  Description:
 *  -----------
 *  This function detects and dispatches events that are reported to a client
 *  frame or icon window that are not widget-related (i.e. they would not be
 *  dispatched by the Xtk intrinsics).
 *
 *
 *  Inputs:
 *  ------
 *  event = This is an X event that has been retrieved by XtNextEvent.
 *
 *
 *  Outputs:
 *  -------
 *  RETURN = If True the event should be dispatched by the toolkit,
 *      otherwise the event should not be dispatched.
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
Boolean WmDispatchClientEvent (event)
    XEvent *event;
    
#else /* _NO_PROTO */
Boolean WmDispatchClientEvent (XEvent *event)
#endif /* _NO_PROTO */
{
    ClientData * pCD;
    Boolean dispatchEvent = False;

    /*
     * Detect and dispatch non-widget events that have been reported to
     * an icon or a client window frame.
     */

    if (XFindContext (DISPLAY, event->xany.window, wmGD.windowContextType,
	    (caddr_t *)&pCD))
    {
	/*
	 *  Set active screen if we're not sure. 
	 */
	if (wmGD.queryScreen)
	    DetermineActiveScreen (event);

	/*
	 * Handle events on windows that are made by mwm for
	 * non-client-specific functions.  Also handle "leftover"
	 * events on windows that used to be managed by mwm
	 * (e.g. ConfigureRequest events).
	 */

	return (HandleEventsOnSpecialWindows (event));
    }


    /*
     *  Set active screen if this is not a FocusOut event.
     *  We don't need to set it on focus out AND we use
     *  (SCREEN_FOR_CLIENT(pCD) != ACTIVE_SCREEN) in
     *  in HandleCFocusOut to determine if a new colormap needs
     *  to be installed.
     */

    if (!(event->type == FocusOut))
    {
	SetActiveScreen (PSD_FOR_CLIENT(pCD));
    }

    /*
     * Handle events on top-level client windows.
     */

    if (event->xany.window == pCD->client)
    {
	return (HandleEventsOnClientWindow (pCD, event));
    }

    /*
     * Handle events on windows created by mwm (for icons and client window
     * frames) and on non-top-level client windows (e.g., colormap
     * windows).
     */

    switch (event->type)
    {
	case ButtonPress:
	{
	    dispatchEvent = HandleCButtonPress (pCD, (XButtonEvent *)event);
	    break;
	}

	case ButtonRelease:
	{
	    if (wmGD.menuActive)
	    {
		dispatchEvent = True; /* have the toolkit dispatch the event */
	    }
	    else
	    {
		HandleCButtonRelease (pCD, (XButtonEvent *)event);
	    }
	    break;
	}

	case KeyPress:
	{
	    dispatchEvent = HandleCKeyPress (pCD, (XKeyEvent *)event);
	    break;
	}

	case MotionNotify:
	{
	    HandleCMotionNotify (pCD, (XMotionEvent *)event);
	    break;
	}

	case Expose:
	{
	    /* 
	     * If multiple expose events, wait for last one.
	     */

	    if (event->xexpose.count == 0) 
	    {
		if (event->xexpose.window == ICON_FRAME_WIN(pCD))
		{
		    IconExposureProc (pCD, False);
		    if (P_ICON_BOX(pCD))
		    {
			dispatchEvent = True;
		    }
		}
		else if (event->xexpose.window == 
			     pCD->pSD->activeIconTextWin)
		{
		    PaintActiveIconText (pCD, FALSE);
		}
		else if ((event->xexpose.window == pCD->clientFrameWin) ||
			 (event->xexpose.window == pCD->clientTitleWin) ||
			 (event->xexpose.window == pCD->clientBaseWin))
		{
		    FrameExposureProc (pCD);
		}
	    }
	    break;
	}

	case EnterNotify:
	{
	    HandleCEnterNotify (pCD, (XEnterWindowEvent *)event);
	    break;
	}

	case FocusIn:
	{
	    dispatchEvent = HandleCFocusIn (pCD, (XFocusChangeEvent *)event);
	    break;
	}

	case FocusOut:
	{
	    dispatchEvent = HandleCFocusOut (pCD, (XFocusChangeEvent *)event);
	    break;
	}

	case DestroyNotify:
	{
	    if (((XDestroyWindowEvent *)event)->window == pCD->client)
	    {
	        pCD->clientFlags |= CLIENT_DESTROYED;
	        UnManageWindow (pCD);
	    }
	    break;
	}

	case UnmapNotify:
	{
	    /*
	     * This event is generated when a managed  client window is 
	     * unmapped by the client or when the window manager unmaps the
	     * client window; check the wmMapCount to determine if this is
	     * the result of a window manager unmap. If this is a client
	     * unmap then the window is to be withdrawn from window manager
	     * control.
	     */

	    if (((XUnmapEvent *)event)->window == pCD->client)
	    {
	        if (pCD->wmUnmapCount)
	        {
		    pCD->wmUnmapCount--;
	        }
	        else
	        {
		    UnManageWindow (pCD);
	        }
            }
	    break;
	}

	case MapRequest:
	{
	    /*
	     * This is a request to change the state of the client window from
	     * iconic (minimized) to normal.
	     */
	    SetClientState (pCD, NORMAL_STATE, GetTimestamp ());
	    break;
	}

	case ConfigureRequest:
	{
	    HandleCConfigureRequest (pCD, (XConfigureRequestEvent *)event);
	    break;
	}

	case ColormapNotify:
	{
	    /*
	     * Process changes to client window colormaps:
	     */

	    HandleCColormapNotify (pCD, (XColormapEvent *)event);
	    break;
	}

	case ClientMessage:
	{
	    /*
	     * Handle client message events.
	     */

	    HandleClientMessage (pCD, (XClientMessageEvent *)event);
	    break;
	}
#ifndef OLD_REPARENTNOTIFY
	case ReparentNotify:
	{
	    if ((((XReparentEvent *)event)->window == pCD->client) &&
		(((XReparentEvent *)event)->parent != pCD->clientBaseWin))
	    {
		/*
		 * The window was reparented away from the frame.
		 * Unmanage to clean up the now empty frame.
		 *
		 * Note: We get here when the reparent is done while
		 * the client is unmapped (e.g. iconified). Otherwise
		 * the reparent will generate an UnmapNotify which
		 * will also cause us to unmanage the client.
		 */
		UnManageWindow (pCD);
	      }
	    break;
	}
#else
        case ReparentNotify:
 	{
 	    if ((((XReparentEvent *)event)->window == pCD->client) &&
 	        (((XReparentEvent *)event)->parent == ROOT_FOR_CLIENT(pCD)))
 	    {
 		/*
 		 * The window was reparented from the frame back to
 		 * the root! 
 		 * (This is a violation of ICCCM!)
 		 */
 		UnManageWindow (pCD);
             }
 	    break;
 	}
#endif
    } /* end of event.type switch */


    return (dispatchEvent);


} /* END OF FUNCTION WmDispatchClientEvent */



/*************************************<->*************************************
 *
 *  HandleEventsOnSpecialWindows (pEvent)
 *
 *
 *  Description:
 *  -----------
 *  Handles events on special window manager windows and "leftover" events
 *  from destroyed client window frames.
 *
 *
 *  Inputs:
 *  ------
 *  pEvent = pointer to an XEvent structure
 *
 * 
 *  Outputs:
 *  -------
 *  RETURN = If True the event should be dispatched by the toolkit,
 *      otherwise the event should not be dispatched.
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
Boolean HandleEventsOnSpecialWindows (pEvent)
    XEvent *pEvent;

#else /* _NO_PROTO */
Boolean HandleEventsOnSpecialWindows (XEvent *pEvent)
#endif /* _NO_PROTO */
{
    Boolean dispatchEvent = True;


    /*
     * The window is not a root window or a client frame window.  Check for
     * a special window manager window.  Have the toolkit dispatch the event
     * if the event is not on a special window.
     */

    if (pEvent->xany.window == ACTIVE_ROOT)
    {
	if (pEvent->type == FocusIn)
	{
	    SetKeyboardFocus ((ClientData *) NULL, REFRESH_LAST_FOCUS);
	}
    }
    else if (pEvent->xany.window == ACTIVE_PSD->feedbackWin)
    {
	if (pEvent->type == Expose)
	{
	    if (pEvent->xexpose.count == 0)
	    {
	        PaintFeedbackWindow(ACTIVE_PSD);
	    }
	}
	dispatchEvent = False; /* don't have the toolkit dispatch the event */
    }
    else if (pEvent->xany.window == ACTIVE_PSD->inputScreenWindow)
    {
	if (pEvent->type == ButtonPress)
	{
	    F_Beep (NULL, (ClientData *) NULL, (XEvent *) NULL);
	}
	else if ((wmGD.keyboardFocusPolicy == KEYBOARD_FOCUS_POINTER) &&
		 (pEvent->type == EnterNotify))
	{
	    HandleWsEnterNotify ((XEnterWindowEvent *)pEvent);
	}
	dispatchEvent = False; /* don't have the toolkit dispatch the event */
    }
    else
    {
	/*
	 * Events may come in for a client frame base window that no
	 * longer exists (the client window was just unmanaged but the
	 * the client did some action before the un-reparenting was
	 * actually done).  Confirm that this is the case and then
	 * handle the request as if it came in as a root window event.
	 */

	switch (pEvent->type)
	{
	    case ConfigureRequest:
	    {
		if (GetParentWindow (pEvent->xconfigurerequest.window) ==
		    ACTIVE_ROOT)
		{
		    /*
		     * This is an event for a client base window that
		     * no longer exists.  Handle the event as if it is a
		     * root window event.
		     */

		    dispatchEvent =  WmDispatchWsEvent (pEvent);
		}
		break;
	    }

	    case MapRequest:
	    {
		if (GetParentWindow (pEvent->xmaprequest.window) ==
		    ACTIVE_ROOT)
		{
		    /*
		     * This is an event for a client base window that
		     * no longer exists.  Handle the event as if it is a
		     * root window event.
		     */

		    dispatchEvent = WmDispatchWsEvent (pEvent);
		}
		break;
	    }
	}
    }

    return (dispatchEvent);

} /* END OF FUNCTION HandleEventsOnSpecialWindows */



/*************************************<->*************************************
 *
 *  HandleEventsOnClientWindow (pCD, pEvent)
 *
 *
 *  Description:
 *  -----------
 *  Handles events on a client top-level window.
 *
 *
 *  Inputs:
 *  ------
 *  pCD = pointer to client data
 *
 *  pEvent = pointer to an XEvent structure
 *
 * 
 *  Outputs:
 *  -------
 *  RETURN = If True the event should be dispatched by the toolkit,
 *      otherwise the event should not be dispatched.
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
Boolean HandleEventsOnClientWindow (pCD, pEvent)
    ClientData *pCD;
    XEvent *pEvent;

#else /* _NO_PROTO */
Boolean HandleEventsOnClientWindow (ClientData *pCD, XEvent *pEvent)
#endif /* _NO_PROTO */
{
    Boolean doXtDispatchEvent = True;

#ifdef SHAPE
    if (pEvent->type == (wmGD.shapeEventBase+ShapeNotify))
    {
        HandleCShapeNotify (pCD, (XShapeEvent *)pEvent);
    }
    else
#endif /* SHAPE */
    switch (pEvent->type)
    {
	case ColormapNotify:
	{
	    /*
	     * Process changes to top-level client window colormaps:
	     */

	    HandleCColormapNotify (pCD, (XColormapEvent *)pEvent);
	    doXtDispatchEvent = False;
	    break;
	}

	case PropertyNotify:
	{
	    /*
	     * Process property changes on managed client windows:
	     */
		
	    HandleCPropertyNotify (pCD, (XPropertyEvent *)pEvent);
	    doXtDispatchEvent = False;
	    break;
	}

	case ClientMessage:
	{
	    /*
	     * Handle client message events.
	     */

	    HandleClientMessage (pCD, (XClientMessageEvent *)pEvent);
	    break;
	}

    }

    return (doXtDispatchEvent);


} /* END OF FUNCTION HandleEventsOnClientWindow */



/*************************************<->*************************************
 *
 *  HandleCPropertyNotify (pCD, propertyEvent)
 *
 *
 *  Description:
 *  -----------
 *  This function handles propertyNotify events (indicating window property
 *  changes) that are reported to the client window.
 *
 *
 *  Inputs:
 *  ------
 *  pCD = pointer to the client data for the client window that got the event
 *
 *  propertyEvent = propertyNotify event that was received
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void HandleCPropertyNotify (pCD, propertyEvent)
    ClientData *pCD;
    XPropertyEvent *propertyEvent;

#else /* _NO_PROTO */
void HandleCPropertyNotify (ClientData *pCD, XPropertyEvent *propertyEvent)
#endif /* _NO_PROTO */
{

    switch (propertyEvent->atom)
    {
        case XA_WM_HINTS:
	{
	    ProcessWmHints (pCD, FALSE /*not first time*/);
	    break;
	}
	
        case XA_WM_NORMAL_HINTS:
	{
	    ProcessWmNormalHints (pCD, FALSE /*not first time*/, 0);
	    break;
	}
	
        case XA_WM_NAME:
	{
	    ProcessWmWindowTitle (pCD, FALSE /*not first time*/);
	    break;
	}
	
        case XA_WM_ICON_NAME:
	{
	    ProcessWmIconTitle (pCD, FALSE /*not first time*/);
	    break;
	}
	
        case XA_WM_CLASS:
	{

	    break;
	}
	
        case XA_WM_COMMAND:
	{
	    if (pCD->clientFlags & CLIENT_TERMINATING)
	    {
		DeleteClientWmTimers (pCD);
		XKillClient (DISPLAY, pCD->client);
	    }
	    break;
	}
	
	default:
	{
	    if (propertyEvent->atom == wmGD.xa_WM_PROTOCOLS)
	    {
		ProcessWmProtocols (pCD);
	    }
	    else if (propertyEvent->atom == wmGD.xa_MWM_MESSAGES)
	    {
		if (pCD->protocolFlags & PROTOCOL_MWM_MESSAGES)
		{
		    ProcessMwmMessages (pCD);
		}
	    }
	    else if (propertyEvent->atom == wmGD.xa_WM_COLORMAP_WINDOWS)
	    {
		if (propertyEvent->state == PropertyNewValue)
		{
		    ProcessWmColormapWindows (pCD);
		}
		else
		{
		    /* property was deleted */
		    ResetColormapData (pCD, NULL, 0);
		}

		if ((ACTIVE_PSD->colormapFocus == pCD) &&
		    ((pCD->clientState == NORMAL_STATE) ||
		     (pCD->clientState == MAXIMIZED_STATE)))
		{
		    /*
		     * The client window has the colormap focus, install the
		     * colormap.
		     */

		    WmInstallColormap (ACTIVE_PSD, pCD->clientColormap);
		}
	    }
	    break;
	}
    }

} /* END OF FUNCTION HandleCPropertyNotify */



/*************************************<->*************************************
 *
 *  HandleCButtonPress (pCD, buttonEvent)
 *
 *
 *  Description:
 *  -----------
 *  This function does window management actions associated with a button
 *  press event on the client window (including frame) or icon.
 *
 *
 *  Inputs:
 *  ------
 *  pCD = pointer to client data (identifies client window)
 *
 *  buttonEvent = ButtonPress event on client window
 *
 *
 *  Outputs:
 *  -------
 *  RETURN = True if the event should be dispatched by XtDispatchEvent
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
Boolean HandleCButtonPress (pCD, buttonEvent)
    ClientData *pCD;
    XButtonEvent *buttonEvent;

#else /* _NO_PROTO */
Boolean HandleCButtonPress (ClientData *pCD, XButtonEvent *buttonEvent)
#endif /* _NO_PROTO */
{
    Boolean dispatchEvent = False;
    Boolean replayEvent = True;
    Context context;
    int partContext;
    Context subContext;

    wmGD.passButtonsCheck = True;

    /*
     * Find out the event context and process the event accordingly.
     * If the event is due to a key focus selection grab or an application
     * modal grab then handle the grab (only these types of grabs are
     * done on the client window frame base window)..
     */

    if (wmGD.menuActive)
    {
	dispatchEvent = True;	/* have the toolkit dispatch the event */
    }
    else
    {
	IdentifyEventContext (buttonEvent, pCD, &context, &partContext);
	subContext = (1L << partContext);

	ProcessClickBPress (buttonEvent, pCD, context, subContext);

	if (CheckForButtonAction (buttonEvent, context, subContext, pCD) && pCD)
    	{
	    /*
	     * Button bindings have been processed, now check for bindings
	     * that associated with the built-in semantics of the window
	     * frame decorations.
	     */

            CheckButtonPressBuiltin (buttonEvent, context, subContext,
		partContext, pCD);
	}
	else
	{
	   /*
 	    * Else skip built-in processing due to execution of a function
	    * that does on-going event processing or that has changed the
	    * client state (e.g., f.move or f.minimize).
 	    */

	    replayEvent = False;
	}
    }

    if (buttonEvent->window == pCD->clientBaseWin)
    {
	ProcessButtonGrabOnClient (pCD, buttonEvent, replayEvent);
    }

    return (dispatchEvent);


} /* END OF FUNCTION HandleCButtonPress */



/*************************************<->*************************************
 *
 *  ProcessButtonGrabOnClient (pCD, buttonEvent, replayEvent)
 *
 *
 *  Description:
 *  -----------
 *  This function handles an activated button grab on the client window
 *  frame base window.
 *
 *
 *  Inputs:
 *  ------
 *  pCD = pointer to client data of window associated with the grab
 *
 *  buttonEvent = ButtonPress event on client window
 *
 *  replayEvent = True if event should be replayed
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void ProcessButtonGrabOnClient (pCD, buttonEvent, replayEvent)
    ClientData *pCD;
    XButtonEvent *buttonEvent;
    Boolean replayEvent;

#else /* _NO_PROTO */
void ProcessButtonGrabOnClient (ClientData *pCD, XButtonEvent *buttonEvent, Boolean replayEvent)
#endif /* _NO_PROTO */
{
    ButtonSpec *buttonSpec;
    Boolean passButton;



    if ((buttonEvent->button == SELECT_BUTTON) && (buttonEvent->state == 0))
    {
	passButton = wmGD.passSelectButton;
    }
    else
    {
	passButton = wmGD.passButtons;
    }

    if (IS_APP_MODALIZED(pCD) || !passButton)
    {
	replayEvent = False;
    }
    else if (replayEvent)
    {
	/*
	 * Replay the event as long as there is not another button binding
	 * for the button release.
	 */

	buttonSpec = ACTIVE_PSD->buttonSpecs;
	while (buttonSpec)
	{
	    if ((buttonSpec->eventType == ButtonRelease) &&
		(buttonEvent->state == buttonSpec->state) &&
		(buttonEvent->button == buttonSpec->button))
	    {
		replayEvent = False;
		break;
	    }

	    buttonSpec = buttonSpec->nextButtonSpec;
	}
    }

    if (replayEvent && wmGD.passButtonsCheck)
    {
	XAllowEvents (DISPLAY, ReplayPointer, CurrentTime);
    }
    else
    {
	if (IS_APP_MODALIZED(pCD))
	{
	    /*
	     * The grab is done on a window that has an application modal
	     * secondary window.  Beep to indicate no client processing of
	     * the event.
	     */

	    F_Beep (NULL, pCD, (XEvent *) NULL);
	}

	XAllowEvents (DISPLAY, AsyncPointer, CurrentTime);
    }

} /* END OF FUNCTION ProcessButtonGrabOnClient */



/*************************************<->*************************************
 *
 *  CheckButtonPressBuiltin (buttonEvent, context, subContext, partContext, pCD)
 *
 *
 *  Description:
 *  -----------
 *  This function checks to see if a built-in window manager function
 *  has been selected.  If yes, then the function is done.
 *
 *
 *  Inputs:
 *  ------
 *  buttonEvent = pointer to button event
 *
 *  context = button event context (root, icon, window)
 *
 *  subContext = button event subcontext (title, system button, ...)
 *
 *  partContext = part context within a window manager component
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void CheckButtonPressBuiltin (buttonEvent, context, subContext, partContext, pCD)
    XButtonEvent *buttonEvent;
    Context context;
    Context subContext;
    int partContext;
    ClientData *pCD;

#else /* _NO_PROTO */
void CheckButtonPressBuiltin (XButtonEvent *buttonEvent, Context context, Context subContext, int partContext, ClientData *pCD)
#endif /* _NO_PROTO */
{
    /*
     * All builtin button bindings are based on button 1 with no
     * modifiers.  The lock modifier has been filtered out in
     * doing builtin button binding processing.
     */

    if (((buttonEvent->button != SELECT_BUTTON)  && 
	 (buttonEvent->button != DMANIP_BUTTON)) || buttonEvent->state)
    {
	return;
    }


    /*
     * Process the builtin button bindings based on the window manager
     * component that was selected.
     */

    if (context & F_CONTEXT_ICON)
    {
	HandleIconButtonPress (pCD, buttonEvent);
    }
    else if (context & F_CONTEXT_ICONBOX)
    {
	HandleIconBoxButtonPress (pCD, buttonEvent, subContext);
    }
    else if (context & F_CONTEXT_WINDOW)
    {
	/*
	 * A client window frame component was selected.
	 */

	/*
	 * If the keyboard focus policy is explicit then all window frame
	 * components set the keyboard input focus when selected.
	 */

	if (wmGD.keyboardFocusPolicy == KEYBOARD_FOCUS_EXPLICIT)
	{
	    Do_Focus_Key (pCD, buttonEvent->time,
		(long)((partContext == FRAME_CLIENT) ? CLIENT_AREA_FOCUS : 0));
	}


	/*
         * Process the builtin button bindings based on the client window
	 * frame component that was selected.
	 */

	if ((buttonEvent->button == SELECT_BUTTON) && 
	    (subContext == F_SUBCONTEXT_W_SYSTEM))
	{
            /*
	     * System menu button component:
             * SELECT_BUTTON Press - post the system menu.
	     * SELECT_BUTTON double-click - close the window.
	     */

	    PushGadgetIn (pCD, partContext);

	    if ((wmGD.clickData.doubleClickContext == F_SUBCONTEXT_W_SYSTEM) &&
	        wmGD.systemButtonClick2 &&
	        (pCD->clientFunctions & MWM_FUNC_CLOSE))
	    {
	        /*
	         * Close the client window.  Don't do any of the other
	         * system menu button actions.
	         */

		wmGD.clickData.clickPending = False;
		wmGD.clickData.doubleClickPending = False;
		F_Kill (NULL, pCD, (XEvent *) buttonEvent);
		return;
	    }

	    if (pCD->clientState == NORMAL_STATE)
	    {
		context = F_CONTEXT_NORMAL;
	    }
	    else if (pCD->clientState == MAXIMIZED_STATE)
	    {
	        context = F_CONTEXT_MAXIMIZE;
	    }
	    else
	    {
	        context = F_CONTEXT_ICON;
	    }

	    /*
	     * Set up for "sticky" menu processing if specified.
	     */

	    if (wmGD.systemButtonClick)
	    {
		wmGD.checkHotspot = True;
	    }

            pCD->grabContext = context;

	    PostMenu (pCD->systemMenuSpec, pCD, 0, 0, SELECT_BUTTON, 
		      context, 0, (XEvent *)buttonEvent);

	}
	else if (subContext == F_SUBCONTEXT_W_TITLE)
	{
            /*
	     * Title component:
             * SELECT_BUTTON  or DMANIP_BUTTON Press - 
	     *               start looking for a move.
             */

	    PushGadgetIn (pCD, partContext);

	    wmGD.preMove = True;
	    wmGD.preMoveX = buttonEvent->x_root;
	    wmGD.preMoveY = buttonEvent->y_root;
	    wmGD.configButton = buttonEvent->button;
	    wmGD.configAction = MOVE_CLIENT;

	}
	else if (subContext & F_SUBCONTEXT_W_RBORDER)
	{
            /*
	     * Resize border handle components:
             * SELECT_BUTTON or DMANIP_BUTTON Press - 
	     *              start looking for a resize.
             */

	    wmGD.preMove = True;
	    wmGD.preMoveX = buttonEvent->x_root;
	    wmGD.preMoveY = buttonEvent->y_root;
	    wmGD.configButton = buttonEvent->button;
	    wmGD.configAction = RESIZE_CLIENT;
	    wmGD.configPart = partContext;
	    wmGD.configSet = True;
	}
	else if ((buttonEvent->button == SELECT_BUTTON) &&
            (subContext & (F_SUBCONTEXT_W_MINIMIZE|F_SUBCONTEXT_W_MAXIMIZE)))
	{
            /*
	     * Minimize and maximize button components:
             * SELECT_BUTTON Press - start of a click.
             */

	    PushGadgetIn (pCD, partContext);
	}
	   
	/*
	 * Other components: no action
	 */
    }

} /* END OF FUNCTION CheckButtonPressBuiltin */



/*************************************<->*************************************
 *
 *  HandleIconButtonPress (pCD, buttonEvent)
 *
 *
 *  Description:
 *  -----------
 *  This function handles builtin functions in the icon context.
 *
 *
 *  Inputs:
 *  ------
 *  pCD = pointer to client data of the icon that received the button event
 *
 *  buttonEvent = pointer to the button event that occurred
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void HandleIconButtonPress (pCD, buttonEvent)
    ClientData *pCD;
    XButtonEvent *buttonEvent;

#else /* _NO_PROTO */
void HandleIconButtonPress (ClientData *pCD, XButtonEvent *buttonEvent)
#endif /* _NO_PROTO */
{

    /*
     * Do icon component button press actions:
     * Button 1 press - set the keyboard input focus if policy is explicit
     * Button 1 double-click - normalize the icon
     */

    if (wmGD.clickData.doubleClickContext == F_SUBCONTEXT_I_ALL)
    {
        /*
         * A double-click was done, normalize the icon.
         */

        SetClientState (pCD, NORMAL_STATE, buttonEvent->time);
	wmGD.clickData.clickPending = False;
	wmGD.clickData.doubleClickPending = False;
    }
    else
    {
        /*
         * This is a regular button press (it may be the start of a 
         * double-click).  Set the focus and top the icon if appropriate.
         */

        if (wmGD.keyboardFocusPolicy == KEYBOARD_FOCUS_EXPLICIT)
        {
	    Do_Focus_Key (pCD, buttonEvent->time, ALWAYS_SET_FOCUS);
        }


        /*
         * Indicate that a move may be starting; wait for button motion
         * events before moving the icon.
         */

        wmGD.preMove = True;
        wmGD.preMoveX = buttonEvent->x_root;
        wmGD.preMoveY = buttonEvent->y_root;
        wmGD.configButton = buttonEvent->button;
        wmGD.configAction = MOVE_CLIENT;
    }


} /* END OF FUNCTION HandleIconButtonPress */



/*************************************<->*************************************
 *
 *  HandleIconBoxButtonPress (pCD, buttonEvent, subContext)
 *
 *
 *  Description:
 *  -----------
 *  This function handles builtin functions in the iconbox context.
 *
 *
 *  Inputs:
 *  ------
 *  pCD = pointer to client data of the icon that received the button event
 *
 *  buttonEvent = pointer to the button event that occurred
 *
 *  subContext = context id of event location inside icon box
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void HandleIconBoxButtonPress (pCD, buttonEvent, subContext)
    ClientData *pCD;
    XButtonEvent *buttonEvent;
    Context subContext;

#else /* _NO_PROTO */
void HandleIconBoxButtonPress (ClientData *pCD, XButtonEvent *buttonEvent, Context subContext)
#endif /* _NO_PROTO */
{

    /*
     * Do iconbox icon component button press actions:
     * Button 1 press - select the icon
     * Button 1 double-click - normalize the icon or raise the window
     */

    if ((wmGD.clickData.doubleClickContext == F_SUBCONTEXT_IB_IICON) ||
       (wmGD.clickData.doubleClickContext == F_SUBCONTEXT_IB_WICON))
    {
	F_Normalize_And_Raise ((String)NULL, pCD, (XEvent *)NULL);
    }
    else if ((subContext == F_SUBCONTEXT_IB_IICON) ||
	     (subContext == F_SUBCONTEXT_IB_WICON))
    {
	/*
	 * Indicate that a move may be starting; wait for button motion
	 * events before moving the icon.
	 */

	wmGD.preMove = True;
	wmGD.preMoveX = buttonEvent->x_root;
	wmGD.preMoveY = buttonEvent->y_root;
	wmGD.configButton = buttonEvent->button;
	wmGD.configAction = MOVE_CLIENT;
    }

    /*
     * Do icon box icon actions:
     * Button 1 press - select the icon in the icon box
     */

    /*
     * _XmGrabTheFocus will move the selection cursor to the
     * widget that was "boinked" with the mouse
     */

    if ((P_ICON_BOX(pCD)->pCD_iconBox == wmGD.keyboardFocus) ||
	(P_ICON_BOX(pCD)->pCD_iconBox == wmGD.nextKeyboardFocus))
    {
	_XmGrabTheFocus (XtWindowToWidget(DISPLAY, ICON_FRAME_WIN(pCD)), NULL);
    }


} /* END OF FUNCTION HandleIconBoxButtonPress */



/*************************************<->*************************************
 *
 *  HandleCButtonRelease (pCD, buttonEvent)
 *
 *
 *  Description:
 *  -----------
 *  This function does window management actions associated with a button
 *  release event on the client window (including frame) or icon.
 *
 *
 *  Inputs:
 *  ------
 *  pCD = pointer to client data for the window/icon that got the event
 *
 *  buttonEvent = pointer to the button event that occurred
 *
 *  Comments:
 *  ---------
 *  Skip builtin processing if move or resize button actions were started
 *  due to button-up bindings.
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void HandleCButtonRelease (pCD, buttonEvent)
    ClientData *pCD;
    XButtonEvent *buttonEvent;

#else /* _NO_PROTO */
void HandleCButtonRelease (ClientData *pCD, XButtonEvent *buttonEvent)
#endif /* _NO_PROTO */
{
    Context context;
    Context subContext;
    int partContext;


    /*
     * Find out whether the event was on the client window frame or the icon
     * and process the event accordingly.
     */

    IdentifyEventContext (buttonEvent, pCD, &context, &partContext);
    subContext = (1L << partContext);

    ProcessClickBRelease (buttonEvent, pCD, context, subContext);

    if (CheckForButtonAction (buttonEvent, context, subContext, pCD) && pCD)
    {
	/*
	 * Button bindings have been processed, now check for bindings
	 * that associated with the built-in semantics of the window
	 * frame decorations.
	 */

        CheckButtonReleaseBuiltin (buttonEvent, context, subContext, pCD);
    }
    /*
     * Else skip built-in processing due to execution of a function that
     * does on-going event processing or that has changed the client state
     * (e.g., f.move or f.minimize).
     */


    /* clear preMove state */
    wmGD.preMove = False;


} /* END OF FUNCTION HandleCButtonRelease */



/*************************************<->*************************************
 *
 *  HandleCKeyPress (pCD, keyEvent)
 *
 *
 *  Description:
 *  -----------
 *  This function does window management actions associated with a key
 *  press event on the client window (including frame) or icon.
 *
 *
 *  Inputs:
 *  ------
 *  pCD = pointer to client data for the window/icon that got the event
 *
 *  keyEvent = pointer to the key event that occurred
 *
 *
 *  Outputs:
 *  -------
 *  RETURN = True if the event should be dispatched by XtDispatchEvent
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
Boolean HandleCKeyPress (pCD, keyEvent)
    ClientData *pCD;
    XKeyEvent *keyEvent;

#else /* _NO_PROTO */
Boolean HandleCKeyPress (ClientData *pCD, XKeyEvent *keyEvent)
#endif /* _NO_PROTO */
{
    Boolean dispatchEvent = False;
    Boolean checkKeyEvent = True;


    if (wmGD.menuActive)
    {
	/*
	 * The active menu accelerators have been checked and keyEvent was
	 * not one of them.  We will check for an iconbox icon widget key and
	 * for pass keys mode and then have the toolkit dispatch the event, 
	 * without rechecking the client accelerator list.
	 */

	dispatchEvent = True;
	checkKeyEvent = False;
    }

    /*
     * If pass keys is active then only check for getting out of the pass
     * keys mode if the event is on the client frame or icon frame window.
     * Unfreeze the keyboard and replay the key if pass keys is active.
     */

    if (((keyEvent->window == ICON_FRAME_WIN(pCD)) ||
	 (keyEvent->window == pCD->pSD->activeIconTextWin)) &&
	P_ICON_BOX(pCD))
    {
	/*
	 * This is a non-grabbed key that is intended for the icon widget
	 * in the iconbox.
	 */

	dispatchEvent = True; /* have the toolkit dispatch the event */
	checkKeyEvent = False;
	if (keyEvent->window == pCD->pSD->activeIconTextWin)
	{
	    /*
	     * The event is really for the icon, not the active
	     * label, so ... correct the window id 
	     */

	    keyEvent->window = ICON_FRAME_WIN(pCD);
	}
    }
    else if (wmGD.passKeysActive)
    {
	if (wmGD.passKeysKeySpec &&
	    (wmGD.passKeysKeySpec->state == keyEvent->state) &&
	    (wmGD.passKeysKeySpec->keycode == keyEvent->keycode))
	{
	    /*
	     * Get out of the pass keys mode.
	     */

	    F_Pass_Key (NULL, (ClientData *) NULL, (XEvent *) NULL);
	    XAllowEvents (DISPLAY, AsyncKeyboard, CurrentTime);
	}
	else
	{
	    XAllowEvents (DISPLAY, ReplayKeyboard, CurrentTime);
	}
	checkKeyEvent = False;
    }
    else
    {
	XAllowEvents (DISPLAY, AsyncKeyboard, CurrentTime);
    }


    /*
     * Check for a "general" key binding that has been set only for the
     * icon context.  These key bindings are set with the keyBinding
     * resource or as accelerators in icon context menus.
     */

    if (checkKeyEvent && (keyEvent->window == ICON_FRAME_WIN(pCD)))
    {
	if ((checkKeyEvent = HandleKeyPress (keyEvent, 
					     ACTIVE_PSD->keySpecs, True,
					     F_CONTEXT_ICON, False,
					     (ClientData *)NULL))
	    && ACTIVE_PSD->acceleratorMenuCount)
	{
	    int n;

	    for (n = 0; ((keyEvent->keycode != 0) &&
			 (n < ACTIVE_PSD->acceleratorMenuCount)); n++)
	    {
		if (!HandleKeyPress (keyEvent,
		           ACTIVE_PSD->acceleratorMenuSpecs[n]->accelKeySpecs,
			   True, F_CONTEXT_ICON, True,(ClientData *)NULL))
		{
		    checkKeyEvent = False;
		    break;
		}
	    }
	}
    }

    /*
     * Check for a key binding that has been set as an accelerator in the
     * system menu.  We only do the first accelerator found.
     */

    if (checkKeyEvent && pCD->systemMenuSpec &&
        (pCD->systemMenuSpec->accelKeySpecs))
    {
	HandleKeyPress (keyEvent, pCD->systemMenuSpec->accelKeySpecs,
			FALSE, 0, TRUE,(ClientData *)NULL );
    }

    return (dispatchEvent);

} /* END OF FUNCTION HandleCKeyPress */



/*************************************<->*************************************
 *
 *  CheckButtonReleaseBuiltin (buttonEvent, context, subContext, pCD)
 *
 *
 *  Description:
 *  -----------
 *  This function checks to see if a built-in window manager function
 *  has been activated as a result of a button release. If yes, then the
 *  associated function is done.
 *
 *
 *  Inputs:
 *  ------
 *  buttonEvent = pointer to a button release event
 *
 *  context = button event context (root, icon, window)
 *
 *  subContext = button event subcontext (title, system button, ...)
 *
 *  pCD = pointer to client data for the window/icon that got the event
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void CheckButtonReleaseBuiltin (buttonEvent, context, subContext, pCD)
    XButtonEvent *buttonEvent;
    Context context;
    Context subContext;
    ClientData *pCD;

#else /* _NO_PROTO */
void CheckButtonReleaseBuiltin (XButtonEvent *buttonEvent, Context context, Context subContext, ClientData *pCD)
#endif /* _NO_PROTO */
{
    /*
     * All builtin botton buindings are based on button 1 with no modifiers.
     * In general, the lock modifier is filtered out in  doing builtin button
     * binding processing. 
     *
     * Test the event for a ``button up'' transition on buttons we are
     * interested in.
     */

    if (!((buttonEvent->button == SELECT_BUTTON) &&
	  (buttonEvent->state == SELECT_BUTTON_MASK)) &&
	!((buttonEvent->button == DMANIP_BUTTON) &&
	  (buttonEvent->state == DMANIP_BUTTON_MASK)))
    {
	return;
    }


    /*
     * Process the builtin button bindings based on the window manager
     * component that was selected.
     */

    if ((buttonEvent->button == SELECT_BUTTON) &&
	(context & F_CONTEXT_ICON))
    {
	/*
	 * Do the icon component button release actions:
	 * SELECT_BUTTON click - post the system menu if specified.
	 */

	if (wmGD.iconClick &&
	    (wmGD.clickData.clickContext == F_SUBCONTEXT_I_ALL))
	{
	    wmGD.checkHotspot = True;

	    /*
	     * Post the system menu with traversal on (Button 1 should be
	     * used to manipulate the menu).
	     */
	    pCD->grabContext = F_CONTEXT_ICON;
	    PostMenu (pCD->systemMenuSpec, pCD, 0, 0, NoButton, 
		      F_CONTEXT_ICON, 0, (XEvent *)buttonEvent);
	}
    }
/* post menu from icon in iconbox */
    else if ((buttonEvent->button == SELECT_BUTTON) &&
	     (context & F_CONTEXT_ICONBOX))
    {
        if ((wmGD.iconClick)  &&
            (((pCD->clientState == MINIMIZED_STATE)  &&
	      (wmGD.clickData.clickContext == F_SUBCONTEXT_IB_IICON)) ||
	     (wmGD.clickData.clickContext == F_SUBCONTEXT_IB_WICON))  )
        {
            wmGD.checkHotspot = True;
	    
            /*
             * Post the system menu with traversal on (Button 1 should be
             * used to manipulate the menu.
             */
            if ((wmGD.clickData.clickContext == F_SUBCONTEXT_IB_IICON) &&
                (pCD->clientState == MINIMIZED_STATE))
            {
		pCD->grabContext = F_SUBCONTEXT_IB_IICON;
                PostMenu (pCD->systemMenuSpec, pCD, 0, 0, NoButton,
                          F_SUBCONTEXT_IB_IICON, FALSE, (XEvent *)buttonEvent);
            }
            else
            {
		pCD->grabContext = F_SUBCONTEXT_IB_WICON;
                PostMenu (pCD->systemMenuSpec, pCD, 0, 0, NoButton,
                          F_SUBCONTEXT_IB_WICON, FALSE, (XEvent *)buttonEvent);
            }
        }
    }
/* end of post menu from icon in iconbox */
    else if (context & F_CONTEXT_WINDOW)
    {
	/*
	 * The button release is on a client window frame component.
	 */

	if ((buttonEvent->button == SELECT_BUTTON) &&
	    (subContext == F_SUBCONTEXT_W_MINIMIZE))
	{
	    /*
	     * Minimize button:
	     * Button 1 click - minimize the window.
	     */

	    if (wmGD.clickData.clickContext == F_SUBCONTEXT_W_MINIMIZE)
	    {
		SetClientState (pCD, MINIMIZED_STATE, buttonEvent->time);
	    }
	}
	else if ((buttonEvent->button == SELECT_BUTTON) &&
	         (subContext == F_SUBCONTEXT_W_MAXIMIZE))
	{
	    /*
	     * Maximize button:
	     * Button 1 click - maximize the window.
	     */

	    if (wmGD.clickData.clickContext == F_SUBCONTEXT_W_MAXIMIZE)
	    {
		if (pCD->clientState == NORMAL_STATE)
		{
	            SetClientState (pCD, MAXIMIZED_STATE, buttonEvent->time);
		}
		else
		{
		    SetClientState (pCD, NORMAL_STATE, buttonEvent->time);
		}
	    }
	}
    }


    /*
     * Clear the pre-configuration info that supports the move threshold.
     */

    wmGD.preMove = False;


} /* END OF FUNCTION CheckButtonReleaseBuiltin */



/*************************************<->*************************************
 *
 *  HandleCMotionNotify (pCD, motionEvent)
 *
 *
 *  Description:
 *  -----------
 *  This function does window management actions associated with a motion
 *  notify event on the client window (including frame) or icon.
 *
 *
 *  Inputs:
 *  ------
 *  pCD = pointer to the client data for the window/icon that got the motion
 *
 *  motionEvent = pointer to the motion event
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void HandleCMotionNotify (pCD, motionEvent)
    ClientData *pCD;
    XMotionEvent *motionEvent;

#else /* _NO_PROTO */
void HandleCMotionNotify (ClientData *pCD, XMotionEvent *motionEvent)
#endif /* _NO_PROTO */
{
    int diffX;
    int diffY;


    /*
     * Do pre-move processing (to support the move threshold) if appropriate:
     */

    if (wmGD.preMove)
    {
	diffX = motionEvent->x_root - wmGD.preMoveX;
	if (diffX < 0) diffX = -diffX;
	diffY = motionEvent->y_root - wmGD.preMoveY;
	if (diffY < 0) diffY = -diffY;


	if ((diffX >= wmGD.moveThreshold) || (diffY >= wmGD.moveThreshold)) 
	{
	    /*
	     * The move threshold has been exceded; start the config action.
	     */

	    wmGD.clickData.clickPending = False;
	    wmGD.clickData.doubleClickPending = False;
	    wmGD.preMove = False;

	    if (wmGD.configAction == MOVE_CLIENT)
	    {
		HandleClientFrameMove (pCD, (XEvent *) motionEvent);
	    }
	    else if (wmGD.configAction == RESIZE_CLIENT)
	    {
		HandleClientFrameResize (pCD, (XEvent *) motionEvent);
	    }
	}
    }

} /* END OF FUNCTION HandleCMotionNotify */



/*************************************<->*************************************
 *
 *  HandleCEnterNotify (pCD, enterEvent)
 *
 *
 *  Description:
 *  -----------
 *  This function does window management actions associated with an enter
 *  window event on the client window.
 *
 *
 *  Inputs:
 *  ------
 *  pCD = pointer to the client data for the window/icon that was entered
 *
 *  enterEvent = pointer to the enter event
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void HandleCEnterNotify (pCD, enterEvent)
    ClientData *pCD;
    XEnterWindowEvent *enterEvent;

#else /* _NO_PROTO */
void HandleCEnterNotify (ClientData *pCD, XEnterWindowEvent *enterEvent)
#endif /* _NO_PROTO */
{
    /*
     * If a client is being configured don't change the keyboard input
     * focus.  The input focus is "fixed" after the configuration has been
     * completed.
     */

    if (((enterEvent->mode == NotifyNormal) ||
	 (enterEvent->mode == NotifyUngrab)) &&
	!(enterEvent->detail == NotifyInferior) &&
	!wmGD.menuActive &&
	((wmGD.keyboardFocusPolicy == KEYBOARD_FOCUS_POINTER) ||
	 (wmGD.colormapFocusPolicy == CMAP_FOCUS_POINTER)))
    {
	/* 
	 * Make sure that EnterNotify is applicable; don't do anything if
	 * the window is minimized (not currently visible) or the event is
	 * associated with an icon in the icon box.
	 */

	if (!(((enterEvent->window == pCD->clientFrameWin) &&
	      (pCD->clientState == MINIMIZED_STATE)) ||
	     (((enterEvent->window == ICON_FRAME_WIN(pCD)) && 
	       P_ICON_BOX(pCD)) ||
	      (enterEvent->window == pCD->pSD->activeIconTextWin))))

	{
	    if (wmGD.keyboardFocusPolicy == KEYBOARD_FOCUS_POINTER)
	    {
		/*
		 * Set the focus only if the window does not currently have
		 * or if another window is in the process of getting the
		 * focus (this check avoids redundant focus setting).
		 */

		if ((pCD != wmGD.keyboardFocus) ||
		    (pCD != wmGD.nextKeyboardFocus))
		{
	            Do_Focus_Key (pCD, enterEvent->time, ALWAYS_SET_FOCUS);
		}
	    }
	    if (wmGD.colormapFocusPolicy == CMAP_FOCUS_POINTER)
	    {
	        SetColormapFocus (ACTIVE_PSD, pCD);
	    }
	}
    }

} /* END OF FUNCTION HandleCEnterNotify */




/*************************************<->*************************************
 *
 *  HandleCFocusIn (pCD, focusChangeEvent)
 *
 *
 *  Description:
 *  -----------
 *  This function does window management actions associated with a focus
 *  in event.
 *
 *
 *  Inputs:
 *  ------
 *  pCD = pointer to the client data for the window/icon that was entered
 *
 *  enterEvent = pointer to the focus in event
 *
 *
 *  Outputs:
 *  -------
 *  RETURN = True if event is to be dispatched by the toolkit
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
Boolean HandleCFocusIn (pCD, focusChangeEvent)
    ClientData *pCD;
    XFocusChangeEvent *focusChangeEvent;

#else /* _NO_PROTO */
Boolean HandleCFocusIn (ClientData *pCD, XFocusChangeEvent *focusChangeEvent)
#endif /* _NO_PROTO */
{
    Boolean setupNextFocus;
    Boolean doXtDispatchEvent = False;


    /*
     * Ignore the event if it is for a window that is no longer viewable.
     * This is the case for a client window FocusIn event that is being
     * processed for a window that has been minimized.
     */

    if ((focusChangeEvent->window == ICON_FRAME_WIN(pCD)) && 
	P_ICON_BOX(pCD))
    {
	doXtDispatchEvent = True;
    }
    else if (((focusChangeEvent->mode == NotifyNormal) ||
	     (focusChangeEvent->mode == NotifyWhileGrabbed)) &&
	    !((focusChangeEvent->window == pCD->clientBaseWin) &&
	      (pCD->clientState == MINIMIZED_STATE)) &&
	    !((focusChangeEvent->window == ICON_FRAME_WIN(pCD)) &&
	      (pCD->clientState != MINIMIZED_STATE)))
    {
	setupNextFocus = (wmGD.keyboardFocus == wmGD.nextKeyboardFocus);

	if (wmGD.keyboardFocus != pCD)
	{
	    if ((focusChangeEvent->detail == NotifyNonlinear) ||
	        (focusChangeEvent->detail == NotifyNonlinearVirtual))
	    {
	        SetKeyboardFocus (pCD, REFRESH_LAST_FOCUS);
		if (setupNextFocus)
		{
		    wmGD.nextKeyboardFocus = wmGD.keyboardFocus;
		}
	    }
	}
	else if ((focusChangeEvent->detail == NotifyInferior) &&
		 (wmGD.keyboardFocusPolicy == KEYBOARD_FOCUS_EXPLICIT))
	{
	    /*
	     * The client window was withdrawn (unmapped or destroyed).
	     * Reset the focus.
	     * !!! pointer focus !!!
	     */

	    if (wmGD.autoKeyFocus)
	    {
		/* !!! fix this up to handle transient windows !!! */
		AutoResetKeyFocus (wmGD.keyboardFocus, GetTimestamp ());
	    }
	    else
	    {
		Do_Focus_Key ((ClientData *) NULL, GetTimestamp (), 
			ALWAYS_SET_FOCUS);
	    }
	}
    }

    return (doXtDispatchEvent);

} /* END OF FUNCTION HandleCFocusIn */



/*************************************<->*************************************
 *
 *  HandleCFocusOut (pCD, focusChangeEvent)
 *
 *
 *  Description:
 *  -----------
 *  This function does window management actions associated with a focus
 *  out event that applies to a client window.
 *
 *
 *  Inputs:
 *  ------
 *  pCD = pointer to the client data for the window/icon that was entered
 *
 *  enterEvent = pointer to the focus out event
 *
 *
 *  Outputs:
 *  -------
 *  RETURN = True if event is to be dispatched by the toolkit
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
Boolean HandleCFocusOut (pCD, focusChangeEvent)
    ClientData *pCD;
    XFocusChangeEvent *focusChangeEvent;

#else /* _NO_PROTO */
Boolean HandleCFocusOut (ClientData *pCD, XFocusChangeEvent *focusChangeEvent)
#endif /* _NO_PROTO */
{
    Boolean doXtDispatchEvent = False;
    long focusFlags = REFRESH_LAST_FOCUS ;


    /*
     * Ignore the event if it is for a window that is no longer viewable.
     * This is the case for a client window FocusOut event that is being
     * processed for a window that has been minimized. Also, ignore focus
     * out events for clients that aren't on the current screen.
     */

    if (((focusChangeEvent->window == ICON_FRAME_WIN(pCD)) && 
	 P_ICON_BOX(pCD)) ||
	(SCREEN_FOR_CLIENT(pCD) != ACTIVE_SCREEN))
    {
	doXtDispatchEvent = True;
    }
    else if ((wmGD.keyboardFocus == pCD) &&
	     (focusChangeEvent->mode == NotifyNormal) &&
	     ((focusChangeEvent->detail == NotifyNonlinear) ||
	      (focusChangeEvent->detail == NotifyNonlinearVirtual)) &&
	     !((focusChangeEvent->window == pCD->clientBaseWin) &&
	       (pCD->clientState == MINIMIZED_STATE)) &&
	     !((focusChangeEvent->window == ICON_FRAME_WIN(pCD)) &&
	       (pCD->clientState != MINIMIZED_STATE)))
    {
	/*
	 * The keyboard focus was shifted to another window, maybe on
	 * another screen.  Clear the focus indication and reset focus
	 * handling for the client window.
	 */

	/*
	 * use SCREEN_SWITCH_FOCUS in SetKeyboardFocus to
	 * not call SetColormapFocus if we are moveing
	 * to another screen
	 */
	if (SCREEN_FOR_CLIENT(pCD) != ACTIVE_SCREEN)
	{
	    focusFlags |= SCREEN_SWITCH_FOCUS;
	}
	SetKeyboardFocus ((ClientData *) NULL, focusFlags);
	if (wmGD.nextKeyboardFocus == pCD)
	{
	    wmGD.nextKeyboardFocus = NULL;
	}
    }

    return (doXtDispatchEvent);

} /* END OF FUNCTION HandleCFocusOut */



/*************************************<->*************************************
 *
 *  HandleCConfigureRequest (pCD, configureRequest)
 *
 *
 *  Description:
 *  -----------
 *  This functions handles ConfigureRequest events that are for client windows.
 *
 *
 *  Inputs:
 *  ------
 *  pCD = pointer to client data
 *
 *  configureRequest = a pointer to a ConfigureRequest event
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void HandleCConfigureRequest (pCD, configureRequest)
    ClientData *pCD;
    XConfigureRequestEvent *configureRequest;

#else /* _NO_PROTO */
void HandleCConfigureRequest (ClientData *pCD, XConfigureRequestEvent *configureRequest)
#endif /* _NO_PROTO */
{
    unsigned int mask = configureRequest->value_mask;
    int stackMode = configureRequest->detail;
    unsigned int changeMask;
    ClientData *pcdLeader;
    ClientData *pcdSibling;
    ClientListEntry *pStackEntry;


    /*
     * Call ProcessNewConfiguration to handle window moving and resizing.
     * Send ConfigureNotify event (based on ICCCM conventions).
     * Then process the request for stacking.
     */

    if ((configureRequest->window == pCD->client) &&
	(mask & (CWX | CWY | CWWidth | CWHeight | CWBorderWidth)))
    {
	if (pCD->maxConfig) {
	    ProcessNewConfiguration (pCD,
		(mask & CWX) ? configureRequest->x : pCD->maxX,
		(mask & CWY) ? configureRequest->y : pCD->maxY,
		(unsigned int) ((mask & CWWidth) ? 
		    configureRequest->width : pCD->maxWidth),
		(unsigned int) ((mask & CWHeight) ? 
		    configureRequest->height : pCD->maxHeight),
		True /*client request*/);
	}
	else {
	    ProcessNewConfiguration (pCD,
		(mask & CWX) ? configureRequest->x : pCD->clientX,
		(mask & CWY) ? configureRequest->y : pCD->clientY,
		(unsigned int) ((mask & CWWidth) ? 
		    configureRequest->width : pCD->clientWidth),
		(unsigned int) ((mask & CWHeight) ? 
		    configureRequest->height : pCD->clientHeight),
		True /*client request*/);
	}
    }

    if (mask & CWStackMode)
    {
	changeMask = mask & (CWSibling | CWStackMode);
	if (changeMask & CWSibling)
	{
	    if (XFindContext (DISPLAY, configureRequest->above,
		    wmGD.windowContextType, (caddr_t *)&pcdSibling))
	    {
		changeMask &= ~CWSibling;
	    }
	    else
	    {
		/*
		 * For client requests only primary windows can be
		 * restacked relative to one another.
		 */

		pcdLeader = FindTransientTreeLeader (pCD);
		pcdSibling = FindTransientTreeLeader (pcdSibling);
		if (pcdLeader == pcdSibling)
		{
		    changeMask &= ~CWSibling;
		}
		else
		{
		    pStackEntry = &pcdSibling->clientEntry;
		    if ((stackMode == Above) || (stackMode == TopIf))
		    {
			/* lower the window to just above the sibling */
			Do_Lower (pcdLeader, pStackEntry);
	    	    }
	    	    else if ((stackMode == Below) || (stackMode == BottomIf))
	    	    {
			/* raise the window to just below the sibling */
			Do_Raise (pcdLeader, pStackEntry);
	    	    }
	    	    else if (stackMode == Opposite)
	    	    {
			F_Raise_Lower (NULL, pCD, (XEvent *)configureRequest);
	    	    }
		}
	    }
	}

	if (!(changeMask & CWSibling))
	{
	    if ((stackMode == Above) || (stackMode == TopIf))
	    {
		Do_Raise (pCD, (ClientListEntry *) NULL);
	    }
	    else if ((stackMode == Below) || (stackMode == BottomIf))
	    {
		Do_Lower (pCD, (ClientListEntry *) NULL);
	    }
	    else if (stackMode == Opposite)
	    {
		F_Raise_Lower (NULL, pCD, (XEvent *) configureRequest);
	    }
	}

	/* !!! should a synthetic ConfigureNotify be sent? !!! */
        if ((configureRequest->window == pCD->client) &&
	    !(mask & (CWX | CWY | CWWidth | CWHeight | CWBorderWidth)))
	{
	    SendConfigureNotify (pCD);
	}
    }


} /* END OF FUNCTION HandleCConfigureRequest */



/*************************************<->*************************************
 *
 *  HandleCColormapNotify (pCD, colorEvent)
 *
 *
 *  Description:
 *  -----------
 *  This function does window management actions associated with a colormap
 *  notify event on the client window.
 *
 *
 *  Inputs:
 *  ------
 *  pCD = pointer to client data
 *
 *  colorEvent = a ColormapNotify event
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void HandleCColormapNotify (pCD, colorEvent)
    ClientData *pCD;
    XColormapEvent *colorEvent;

#else /* _NO_PROTO */
void HandleCColormapNotify (ClientData *pCD, XColormapEvent *colorEvent)
#endif /* _NO_PROTO */
{
    int i;
    Boolean newClientColormap = False;


    /*
     * The colormap of the top-level client window or one of its subwindows
     * has been changed.
     */


    if (colorEvent->new)
    {
        /*
         * The colormap has been changed.
         */

        /*
         * !!! when the server ColormapNotify problem is fixed !!!
         * !!! use the colormap id from the event              !!!
         */
        if (WmGetWindowAttributes (colorEvent->window))
        {
	    colorEvent->colormap = wmGD.windowAttributes.colormap;
        }
	else
	{
	    return;
	}
        /*
         * !!! remove the above code when the problem is fixed  !!!
         */

	/*
	 * Identify the colormap that the window manager has associated
	 * with the window.
	 */

	if (pCD->clientCmapCount == 0)
	{
	    /* no subwindow colormaps; change top-level window colormap */
	    if (colorEvent->window == pCD->client)
	    {
	        if (colorEvent->colormap == None)
	        {
	            /* use the workspace colormap */
	            pCD->clientColormap = 
			ACTIVE_PSD->workspaceColormap;
		}
		else
		{
	            pCD->clientColormap = colorEvent->colormap;
		}
		newClientColormap = True;
	    }
	}
	else
	{
	    /* there are subwindow colormaps */
	    for (i = 0; i < pCD->clientCmapCount; i++)
	    {
		if (pCD->cmapWindows[i] == colorEvent->window)
		{
		    if (colorEvent->colormap == None)
		    {
			/* use the workspace colormap */
			pCD->clientCmapList[i] = 
			    ACTIVE_PSD->workspaceColormap;
		    }
		    else
		    {
			pCD->clientCmapList[i] = colorEvent->colormap;
		    }
		    if (i == pCD->clientCmapIndex)
		    {
			newClientColormap = True;
			pCD->clientColormap = pCD->clientCmapList[i];
		    }
		    break;
		}
	    }
	}

	if ((ACTIVE_PSD->colormapFocus == pCD) && newClientColormap &&
	    ((pCD->clientState == NORMAL_STATE) ||
	    (pCD->clientState == MAXIMIZED_STATE)))
	{
	    /*
	     * The client window has the colormap focus, install the
	     * colormap.
	     */

	    WmInstallColormap (ACTIVE_PSD, pCD->clientColormap);
	}
    }


} /* END OF FUNCTION HandleCColormapNotify */



/*************************************<->*************************************
 *
 *  HandleClientMessage (pCD, clientEvent)
 *
 *
 *  Description:
 *  -----------
 *  This function handles client message events that are sent to the root
 *  window.  The window manager action that is taken depends on the
 *  message_type of the event.
 *
 *
 *  Inputs:
 *  ------
 *  pCD = pointer to client data
 *
 *  clientEvent = pointer to a client message event on the root window
 * 
 *************************************<->***********************************/

#ifdef _NO_PROTO
void HandleClientMessage (pCD, clientEvent)
    ClientData *pCD;
    XClientMessageEvent *clientEvent;

#else /* _NO_PROTO */
void HandleClientMessage (ClientData *pCD, XClientMessageEvent *clientEvent)
#endif /* _NO_PROTO */
{
    unsigned int newState;

    /*
     * Process the client message event based on the message_type.
     */

    if (clientEvent->message_type == wmGD.xa_WM_CHANGE_STATE)
    {
	if ((clientEvent->data.l[0] == IconicState) &&
	    (pCD->clientFunctions & MWM_FUNC_MINIMIZE))
	{
	    newState = MINIMIZED_STATE;
	}
	else if (clientEvent->data.l[0] == NormalState)
	{
	    newState = NORMAL_STATE;
	}

	SetClientState (pCD, newState, GetTimestamp ());

    }

} /* END OF FUNCTION HandleClientMessage */


#ifdef SHAPE

/*************************************<->*************************************
 *
 *  HandleCShapeNotify (pCD, shapeEvent)
 *
 *
 *  Description:
 *  -----------
 *  Handle a shape notify event on a client window. Keeps track of
 *  the shaped state of the client window and calls
 *  SetFrameShape() to reshape the frame accordingly.
 *
 *  Inputs:
 *  ------
 *  shapeEvent = pointer to a shape notify in event on the client window.
 *
 *************************************<->***********************************/
#ifdef _NO_PROTO
void
HandleCShapeNotify (pCD, shapeEvent)

    ClientData *pCD;
    XShapeEvent *shapeEvent;
#else /* _NO_PROTO */
void
HandleCShapeNotify (ClientData *pCD,  XShapeEvent *shapeEvent)
#endif /* _NO_PROTO */
{
    if (pCD)
    {
	if (shapeEvent->kind != ShapeBounding)
	{
	    return;
	}
	
	pCD->wShaped = shapeEvent->shaped;
	SetFrameShape (pCD);
    }
} /* END OF FUNCTION HandleCShapeNotify */
#endif /* SHAPE */


/*************************************<->*************************************
 *
 *  GetParentWindow (window)
 *
 *
 *  Description:
 *  -----------
 *  This function identifies the parent window of the specified window.
 *
 *
 *  Inputs:
 *  ------
 *  window = find the parent of this window
 * 
 *  Outputs:
 *  -------
 *  Return = return the window id of the parent of the specified window
 * 
 *************************************<->***********************************/

#ifdef _NO_PROTO
Window GetParentWindow (window)
    Window window;

#else /* _NO_PROTO */
Window GetParentWindow (Window window)
#endif /* _NO_PROTO */
{
    Window root;
    Window parent;
    Window *children;
    unsigned int nchildren;


    if (XQueryTree (DISPLAY, window, &root, &parent, &children, &nchildren))
    {
	if (nchildren)
	{
	    XFree ((char *)children);
	}
    }
    else
    {
	parent = NULL;
    }

    return (parent);


} /* END OF FUNCTION GetParentWindow */


/*************************************<->*************************************
 *
 *  DetermineActiveScreen (pEvent)
 *
 *
 *  Description:
 *  -----------
 *  This function determines the currently active screen
 *
 *
 *  Inputs:
 *  ------
 *  pEvent = pointer to an event structure
 * 
 *  Outputs:
 *  -------
 *  ACTIVE_PSD =  set to point to the screen data for the currently
 *                active scree;
 *  wmGD.queryScreen =  set to False if we're sure about the ACTIVE_PSD
 *                      setting
 * 
 *************************************<->***********************************/

#ifdef _NO_PROTO
void DetermineActiveScreen (pEvent)
    XEvent *pEvent;

#else /* _NO_PROTO */
void DetermineActiveScreen (XEvent *pEvent)
#endif /* _NO_PROTO */
{
    WmScreenData *pSD;

    switch (pEvent->type)
    {
	case NoExpose:
	case GraphicsExpose:
		break;		/* ignore these events */

        default:
		/*
		 * Get the screen that the event occurred on.
		 */
		pSD = GetScreenForWindow (pEvent->xany.window);

		if (pSD) 
		{
		    /*
		     * Set the ACTIVE_PSD to the event's screen to
		     * make sure the event gets handled correctly.
		     */
		    SetActiveScreen (pSD);
		}
		break;
    }

} /* END OF FUNCTION DetermineActiveScreen */


/*************************************<->*************************************
 *
 *  GetScreenForWindow (win)
 *
 *
 *  Description:
 *  -----------
 *  This function determines the screen for a window
 *
 *
 *  Inputs:
 *  ------
 *  win = window id
 * 
 *  Outputs:
 *  -------
 *  value of function = pointer to screen data (pSD) or NULL on failure
 * 
 *************************************<->***********************************/

WmScreenData * GetScreenForWindow (win)
    Window win;

{
    XWindowAttributes attribs;
    WmScreenData *pSD = NULL;


    /*
     * Get the screen that the event occurred on.
     */
    if (XGetWindowAttributes (DISPLAY, win, &attribs))
    {
	if (!XFindContext (DISPLAY, attribs.root, wmGD.screenContextType, 
			    (caddr_t *)&pSD))
	{
	    if (pSD && !pSD->screenTopLevelW)
	    {
		pSD = NULL;
	    }
	}
    }

    return (pSD);

} /* END OF FUNCTION GetScreenForWindow */


/*************************************<->*************************************
 *
 *  SetActiveScreen (pSD)
 *
 *
 *  Description:
 *  -----------
 *  This function sets the currently active screen 
 *
 *
 *  Inputs:
 *  ------
 *  pSD = pointer to new active screen data
 * 
 *  Outputs:
 *  -------
 *  ACTIVE_PSD =  set to point to the screen data for the currently
 *                active scree;
 * 
 *************************************<->***********************************/

#ifdef _NO_PROTO
void SetActiveScreen (pSD)
    WmScreenData *pSD;

#else /* _NO_PROTO */
void SetActiveScreen (WmScreenData *pSD)
#endif /* _NO_PROTO */
{
    ACTIVE_PSD = pSD;
    wmGD.queryScreen  = False;

} /* END OF FUNCTION SetActiveScreen */

