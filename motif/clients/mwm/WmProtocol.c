#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)WmProtocol.c	3.8 91/01/10";
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

#include "WmError.h"
#include "WmFunction.h"
#include "WmKeyFocus.h"
#include "WmMenu.h"
#include "WmWinInfo.h"

/*
 * Function Declarations:
 */

#ifdef _NO_PROTO
Boolean	AddWmTimer ();
void	DeleteClientWmTimers ();
void	SetupWmICCC ();
void	SendClientMsg ();
void	SendConfigureNotify ();
void	TimeoutProc ();
#else /* _NO_PROTO */
void SetupWmICCC (void);
void SendConfigureNotify (ClientData *pCD);
void SendClientOffsetMessage (ClientData *pCD);
void SendClientMsg (Window window, long type, long data0, Time time, long *pData, int dataLen);
Boolean AddWmTimer (unsigned int timerType, unsigned long timerInterval, ClientData *pCD);
void DeleteClientWmTimers (ClientData *pCD);
void TimeoutProc (caddr_t client_data, XtIntervalId *id);
#endif /* _NO_PROTO */



/*
 * Global Variables:
 */



/*************************************<->*************************************
 *
 *  SetupWmICCC ()
 *
 *
 *  Description:
 *  -----------
 *  This function sets up the window manager handling of the inter-client
 *  communications conventions.
 *
 *
 *  Outputs:
 *  -------
 *  (wmGD) = Atoms id's are setup.
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void SetupWmICCC ()

#else /* _NO_PROTO */
void SetupWmICCC (void)
#endif /* _NO_PROTO */
{
    XIconSize sizeList;
    int scr;

    /*
     * Make atoms that are required by the ICCC and mwm.  The atom for
     * _MOTIF_WM_INFO is intern'ed in ProcessMotifWmInfo.
     */

    wmGD.xa_WM_STATE = XInternAtom (DISPLAY, _XA_WM_STATE, False);
    wmGD.xa_WM_PROTOCOLS = XInternAtom (DISPLAY, _XA_WM_PROTOCOLS, False);
    wmGD.xa_WM_CHANGE_STATE = XInternAtom (DISPLAY, _XA_WM_CHANGE_STATE, False);
    wmGD.xa_WM_SAVE_YOURSELF = XInternAtom (DISPLAY, _XA_WM_SAVE_YOURSELF,
				   False);
    wmGD.xa_WM_DELETE_WINDOW = XInternAtom (DISPLAY, _XA_WM_DELETE_WINDOW,
				   False);
    wmGD.xa_WM_COLORMAP_WINDOWS = XInternAtom (DISPLAY, _XA_WM_COLORMAP_WINDOWS,
				   False);
    wmGD.xa_WM_TAKE_FOCUS = XInternAtom (DISPLAY, _XA_WM_TAKE_FOCUS, False);
    wmGD.xa_MWM_HINTS = XInternAtom (DISPLAY, _XA_MWM_HINTS, False);
    wmGD.xa_MWM_MENU = XInternAtom (DISPLAY, _XA_MWM_MENU, False);
    wmGD.xa_MWM_MESSAGES = XInternAtom (DISPLAY, _XA_MWM_MESSAGES, False);
    wmGD.xa_MWM_OFFSET = XInternAtom (DISPLAY, _XA_MOTIF_WM_OFFSET, False);


    /*
     * Setup the icon size property on the root window.
     */

    sizeList.width_inc = 1;
    sizeList.height_inc = 1;

    for (scr = 0; scr < wmGD.numScreens; scr++)
    {
	if (wmGD.Screens[scr].managed)
	{
	    sizeList.min_width = wmGD.Screens[scr].iconImageMinimum.width;
	    sizeList.min_height = wmGD.Screens[scr].iconImageMinimum.height;
	    sizeList.max_width = wmGD.Screens[scr].iconImageMaximum.width;
	    sizeList.max_height = wmGD.Screens[scr].iconImageMaximum.height;

	    XSetIconSizes (DISPLAY, wmGD.Screens[scr].rootWindow, 
		&sizeList, 1);
	}
    }


} /* END OF FUNCTION SetupWmICCC */



/*************************************<->*************************************
 *
 *  SendConfigureNotify (pCD)
 *
 *
 *  Description:
 *  -----------
 *  This function is used to send a synthetic ConfigureNotify event when
 *  a client window is reconfigured in certain ways (e.g., the window is
 *  moved without being resized).
 *
 *
 *  Inputs:
 *  ------
 *  pCD = pointer to client data (window id and client size data)
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void SendConfigureNotify (pCD)
	ClientData *pCD;

#else /* _NO_PROTO */
void SendConfigureNotify (ClientData *pCD)
#endif /* _NO_PROTO */
{
    XConfigureEvent notifyEvent;


    /*
     * Send a synthetic ConfigureNotify message:
     */

    notifyEvent.type = ConfigureNotify;
    notifyEvent.display = DISPLAY;
    notifyEvent.event = pCD->client;
    notifyEvent.window = pCD->client;
    if (pCD->maxConfig)
    {
	notifyEvent.x = pCD->maxX;
	notifyEvent.y = pCD->maxY;
	notifyEvent.width = pCD->maxWidth;
	notifyEvent.height = pCD->maxHeight;
    }
    else
    {
	notifyEvent.x = pCD->clientX;
	notifyEvent.y = pCD->clientY;
	notifyEvent.width = pCD->clientWidth;
	notifyEvent.height = pCD->clientHeight;
    }
    notifyEvent.border_width = 0;
    notifyEvent.above = None;
    notifyEvent.override_redirect = False;

    XSendEvent (DISPLAY, pCD->client, False, StructureNotifyMask,
	(XEvent *)&notifyEvent);


} /* END OF FUNCTION SendConfigureNotify */



/*************************************<->*************************************
 *
 *  SendClientOffsetMessage (pCD)
 *
 *
 *  Description:
 *  -----------
 *  This function is used to send a client message containing the offset
 *  between the window position reported to the user and the actual
 *  window position of the client over the root.
 *
 *  This can be used by clients that map and unmap windows to help them
 *  work with the window manager to place the window in the same location
 *  when remapped. 
 *
 *  Inputs:
 *  ------
 *  pCD = pointer to client data (frame geometry info)
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void SendClientOffsetMessage (pCD)
	ClientData *pCD;

#else /* _NO_PROTO */
void SendClientOffsetMessage (ClientData *pCD)
#endif /* _NO_PROTO */
{
    XClientMessageEvent clientMsgEvent;

    clientMsgEvent.type = ClientMessage;
    clientMsgEvent.window = pCD->client;
    clientMsgEvent.message_type = wmGD.xa_MWM_MESSAGES;
    clientMsgEvent.format = 32;
    clientMsgEvent.data.l[0] = wmGD.xa_MWM_OFFSET;
    clientMsgEvent.data.l[1] = (long)pCD->clientOffset.x;
    clientMsgEvent.data.l[2] = (long)pCD->clientOffset.y;

    XSendEvent (DISPLAY, pCD->client, False, NoEventMask,
	(XEvent *)&clientMsgEvent);


} /* END OF FUNCTION SendClientOffsetMessage */


/*************************************<->*************************************
 *
 *  SendClientMsg (window, type, data0, time, pData, dataLen)
 *
 *
 *  Description:
 *  -----------
 *  This function is used to send a client message event that to a client
 *  window.  The message may be sent as part of a protocol arranged for by
 *  the client with the WM_PROTOCOLS property.
 *
 *
 *  Inputs:
 *  ------
 *  window = destination window for the client message event
 *
 *  type = client message type
 *
 *  data0 = data0 value in the client message
 *
 *  time = timestamp to be used in the event
 *
 *  pData = pointer to data to be used in the event
 *
 *  dataLen = len of data (in 32 bit units)
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void SendClientMsg (window, type, data0, time, pData, dataLen)
	Window window;
	long type;
	long data0;
	Time time;
	long *pData;
	int dataLen;

#else /* _NO_PROTO */
void SendClientMsg (Window window, long type, long data0, Time time, long *pData, int dataLen)
#endif /* _NO_PROTO */
{
    XClientMessageEvent clientMsgEvent;
    int i;


    clientMsgEvent.type = ClientMessage;
    clientMsgEvent.window = window;
    clientMsgEvent.message_type = type;
    clientMsgEvent.format = 32;
    clientMsgEvent.data.l[0] = data0;
    clientMsgEvent.data.l[1] = (long)time;
    if (pData)
    {
	/*
	 * Fill in the rest of the ClientMessage event (that holds up to
	 * 5 words of data).
	 */

        if (dataLen > 3)
        {
	    dataLen = 3;
        }
        for (i = 2; i < (2 + dataLen); i++)
        {
	    clientMsgEvent.data.l[i] = pData[i];
        }
    }
    
    
    XSendEvent (DISPLAY, window, False, NoEventMask,
	(XEvent *)&clientMsgEvent);


} /* END OF FUNCTION SendClientMsg */



/*************************************<->*************************************
 *
 *  AddWmTimer (timerType, timerInterval, pCD)
 *
 *
 *  Description:
 *  -----------
 *  This function sets a window manager timer of the specified type.
 *
 *
 *  Inputs:
 *  ------
 *  timerType = type of timer to be set
 *
 *  timerInterval = length of timeout in ms
 *
 *  pCD = pointer to client data associated with the timer
 *
 *  return = True if timer could be set
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
Boolean AddWmTimer (timerType, timerInterval, pCD)
unsigned int timerType;
unsigned long timerInterval;
ClientData *pCD;

#else /* _NO_PROTO */
Boolean AddWmTimer (unsigned int timerType, unsigned long timerInterval, ClientData *pCD)
#endif /* _NO_PROTO */
{
    WmTimer *pWmTimer;


    if (!(pWmTimer = (WmTimer *)malloc (sizeof (WmTimer))))
    {
	Warning ("Insufficient memory for window manager data");
	return (False);
    }

    /* !!! handle for XtAppAddTimeOut error !!! */
    pWmTimer->timerId = XtAppAddTimeOut (wmGD.mwmAppContext, 
			    timerInterval, (XtTimerCallbackProc)TimeoutProc, (caddr_t)pCD);
    pWmTimer->timerCD = pCD;
    pWmTimer->timerType = timerType;
    pWmTimer->nextWmTimer = wmGD.wmTimers;
    wmGD.wmTimers = pWmTimer;

    return(True);

} /* END OF FUNCTION AddWmTimer */



/*************************************<->*************************************
 *
 *  DeleteClientWmTimers (pCD)
 *
 *
 *  Description:
 *  -----------
 *  This function deletes all window manager timers that are associated with
 *  the specified client window.
 *
 *
 *  Inputs:
 *  ------
 *  pCD = pointer to client data for client whose timers are to be deleted
 *
 *  wmGD = (wmTimers)
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void DeleteClientWmTimers (pCD)
	ClientData *pCD;

#else /* _NO_PROTO */
void DeleteClientWmTimers (ClientData *pCD)
#endif /* _NO_PROTO */
{
    WmTimer *pPrevTimer;
    WmTimer *pWmTimer;
    WmTimer *pRemoveTimer;


    pPrevTimer = NULL;
    pWmTimer = wmGD.wmTimers;
    while (pWmTimer)
    {
	if (pWmTimer->timerCD == pCD)
	{
	    if (pPrevTimer)
	    {
		pPrevTimer->nextWmTimer = pWmTimer->nextWmTimer;
	    }
	    else
	    {
		wmGD.wmTimers = pWmTimer->nextWmTimer;
	    }
	    pRemoveTimer = pWmTimer;
	    pWmTimer = pWmTimer->nextWmTimer;
	    XtRemoveTimeOut (pRemoveTimer->timerId);
	    free ((char *)pRemoveTimer);
	}
	else
	{
	    pPrevTimer = pWmTimer;
	    pWmTimer = pWmTimer->nextWmTimer;
	}
    }


} /* END OF FUNCTION DeleteClientWmTimers */



/*************************************<->*************************************
 *
 *  TimeoutProc (client_data, id)
 *
 *
 *  Description:
 *  -----------
 *  This function is an Xtk timeout handler.  It is used to handle various
 *  window manager timers (i.e. WM_SAVE_YOURSELF quit timeout).
 *
 *
 *  Inputs:
 *  ------
 *  client_data = pointer to window manager client data
 *
 *  id = Xtk timer id
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void TimeoutProc (client_data, id)
	caddr_t client_data;
	XtIntervalId *id;

#else /* _NO_PROTO */
void TimeoutProc (caddr_t client_data, XtIntervalId *id)
#endif /* _NO_PROTO */
{
    WmTimer *pPrevTimer;
    WmTimer *pWmTimer;

    
    /*
     * Find out if the timer still needs to be serviced.
     */

    pPrevTimer = NULL;
    pWmTimer = wmGD.wmTimers;
    while (pWmTimer)
    {
	if (pWmTimer->timerId == *id)
	{
	    break;
	}
	pPrevTimer = pWmTimer;
	pWmTimer = pWmTimer->nextWmTimer;
    }

    if (pWmTimer)
    {
	/*
	 * Do the timer related action.
	 */

	switch (pWmTimer->timerType)
	{
	    case TIMER_QUIT:
	    {
		XKillClient (DISPLAY, pWmTimer->timerCD->client);
		break;
	    }

	    case TIMER_RAISE:
	    {
		Boolean sameScreen;

		if ((wmGD.keyboardFocus == pWmTimer->timerCD) &&
		    (pWmTimer->timerCD->focusPriority == 
			(PSD_FOR_CLIENT(pWmTimer->timerCD))->focusPriority) &&
		    (wmGD.keyboardFocusPolicy == KEYBOARD_FOCUS_POINTER) &&
		    (pWmTimer->timerCD == GetClientUnderPointer(&sameScreen)))
		{
		    Do_Raise (pWmTimer->timerCD, (ClientListEntry *)NULL);
		}
		break;
	    }
	}


	/*
	 * Remove the timer from the wm timer list.
	 */

	if (pPrevTimer)
	{
	    pPrevTimer->nextWmTimer = pWmTimer->nextWmTimer;
	}
	else
	{
	    wmGD.wmTimers = pWmTimer->nextWmTimer;
	}
	free ((char *)pWmTimer);
    }

    /*
     * Free up the timer.
     */

    XtRemoveTimeOut (*id);


} /* END OF FUNCTION TimeoutProc */
