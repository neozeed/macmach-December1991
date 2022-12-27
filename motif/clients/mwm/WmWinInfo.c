#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)WmWinInfo.c	3.23.1.4 91/03/22";
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
#include "WmResNames.h"
#include "WmIBitmap.h"
#include <Xm/Xm.h>


#define makemult(a, b) ((b==1) ? (a) : (((int)((a) / (b))) * (b)) )

/*
 * include extern functions
 */
#include "WmWinInfo.h"
#include "WmCDInfo.h"
#include "WmCDecor.h"
#include "WmCPlace.h"
#include "WmError.h"
#include "WmIDecor.h"
#include "WmIPlace.h"
#include "WmIconBox.h"
#include "WmImage.h"
#include "WmManage.h"
#include "WmMenu.h"
#include "WmProperty.h" 
#include "WmResource.h"
#include "WmWinList.h"



/*
 * Global Variables:
 */



/*************************************<->*************************************
 *
 *  GetClientInfo (pSD, clientWindow, manageFlags)
 *
 *
 *  Description:
 *  -----------
 *  This function is used to initialize client window data based on the
 *  contents of client window properties and the client window configuration.
 *
 *
 *  Inputs:
 *  ------
 *  pSD = pointer to screen data for screen that client lives in
 *
 *  clientWindow = window id for the client window that is to be managed
 *
 *  manageFlags = flags that indicate wm state info
 *
 * 
 *  Outputs:
 *  -------
 *  Return = pointer to an initialized client data structure for the
 *           specified client window
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
ClientData * 
GetClientInfo (pSD, clientWindow, manageFlags)

	WmScreenData *pSD;
	Window clientWindow;
	long manageFlags;
#else /* _NO_PROTO */
ClientData * 
GetClientInfo (WmScreenData *pSD, Window clientWindow, long manageFlags)
#endif /*_NO_PROTO */

{
    ClientData *pCD;
    XSetWindowAttributes sAttributes;


    /*
     * Allocate and initialize a client data structure:
     */

    if (!(pCD = (ClientData *)malloc (sizeof (ClientData))))
    {
	/* unable to allocate space */
	Warning ("Insufficient memory for client data");
	return (NULL);
    }

    
    /*
     * Initialize the data structure:
     */

    pCD->client = clientWindow;
    pCD->clientID = ++(pSD->clientCounter);
    pCD->clientFlags = WM_INITIALIZATION;
    pCD->iconFlags = 0;
    pCD->pIconBox = NULL;
    pCD->thisIconBox = NULL;
    pCD->wmUnmapCount = 0;
    pCD->transientFor = NULL;
    pCD->transientLeader = NULL;
    pCD->transientChildren = NULL;
    pCD->transientSiblings = NULL;
    pCD->fullModalCount = 0;
    pCD->primaryModalCount = 0;
    pCD->focusPriority = 0;

    pCD->clientClass = NULL;
    pCD->clientName = NULL;
    pCD->clientFrameWin = NULL;
    pCD->iconFrameWin = NULL;
    pCD->iconWindow = NULL;
    pCD->iconPixmap = NULL;
    pCD->iconPlace = NO_ICON_PLACE;
    pCD->clientProtocols = NULL;
    pCD->clientProtocolCount = 0;
    pCD->mwmMessages = NULL;
    pCD->mwmMessagesCount = 0;
    pCD->clientCmapCount = 0;
    pCD->clientCmapIndex = 0;
    pCD->systemMenuSpec = NULL;

    pCD->decorFlags = 0L;
    pCD->pTitleGadgets = NULL;
    pCD->cTitleGadgets = 0;
    pCD->pResizeGadgets = NULL;
    pCD->clientTitleWin = NULL;
    pCD->pclientTopShadows = NULL;
    pCD->pclientBottomShadows = NULL;
    pCD->pclientTitleTopShadows = NULL;
    pCD->pclientTitleBottomShadows = NULL;
    pCD->pclientMatteTopShadows = NULL;
    pCD->pclientMatteBottomShadows = NULL;
    pCD->piconTopShadows = NULL;
    pCD->piconBottomShadows = NULL;

    pCD->maxConfig = FALSE;
    pCD->pSD = pSD;
    pCD->dataType = CLIENT_DATA_TYPE;


     /*
     * Do special processing for client windows that are controlled by
     * the window manager.
     */

    if (manageFlags & MANAGEW_WM_CLIENTS)
    {
	return (GetWmClientInfo (pSD->pActiveWS, pCD, manageFlags));
    }


    /*
     * Register the client window to facilitate event handling:
     */

    XSaveContext (DISPLAY, clientWindow, wmGD.windowContextType, (caddr_t)pCD);


    /*
     * Listen for property change events on the window so that we keep 
     * in sync with the hints.
     */
    sAttributes.event_mask = (PropertyChangeMask | ColormapChangeMask);    
    XChangeWindowAttributes (DISPLAY, pCD->client, CWEventMask,
        &sAttributes);

    /*
     * Get window configuration attributes.  WmGetWindowAttributes sets
     * up the global window attributes cache with the client window 
     * attributes.
     */

    if (!WmGetWindowAttributes (clientWindow))
    {
	/*
	 * Cannot get window attributes. Do not manage window.
	 * (error message within WmGetWindowAttributes)
	 */

	UnManageWindow (pCD);
	return (NULL);
    }
    pCD->xBorderWidth = wmGD.windowAttributes.border_width;


    /*
     * Retrieve and process WM_CLASS hints client window property info:
     */

    ProcessWmClass (pCD);


    /*
     * Retrieve and process WM_TRANSIENT_FOR client window property info:
     */

    ProcessWmTransientFor (pCD);


    /*
     * Get client window resource data (from resources, .mwmrc):
     */

    ProcessClientResources (pCD);


    /*
     * Retrieve and process M_CLIENT_DECOR client window property info:
     */

    ProcessMwmHints (pCD);


    /*
     * Retrieve and process WM_HINTS client window property info:
     */

    ProcessWmHints (pCD, True /*first time*/);


    /*
     * Set offset from frame of client window
     */

    SetClientOffset (pCD);


    /*
     * Retrieve and process WM_NORMAL_HINTS client window property info:
     * 
     */

    ProcessWmNormalHints (pCD, True /*first time*/, manageFlags);


    /*
     * Retrieve and process WM_NAME client window property info (this
     * property contains the window title NOT the window resource name):
     */

    ProcessWmWindowTitle (pCD, TRUE);


    /*
     * Retrieve and process WM_ICON_NAME client window property info:
     */

    ProcessWmIconTitle (pCD, TRUE);


    /*
     * Retrieve and process the WM_PROTOCOLS property.
     */

    ProcessWmProtocols (pCD);


    /*
     * If necessary retrieve and process the _MWM_MESSAGES property.
     */

    if (pCD->protocolFlags & PROTOCOL_MWM_MESSAGES)
    {
	ProcessMwmMessages (pCD);
    }


    /*
     * Make or find a system menu for the client.
     */

    if (pCD->systemMenu)
    {
	MakeSystemMenu (pCD);
    }
    else
    {
	pCD->systemMenuSpec = NULL;
    }


    /*
     * Setup the colormap data for the client window.  This includes
     * retrieving and processing client window properties that deal with
     * subwindow colormaps.
     */

    InitCColormapData (pCD);


    /* successful return */

    return (pCD);


} /* END OF FUNCTION GetClientinfo */



/*************************************<->*************************************
 *
 *  GetWmClientInfo (pWS, pCD, manageFlags)
 *
 *
 *  Description:
 *  -----------
 *  This function is used to initialize client window data for a window
 *  that is controlled by the window manager (e.g., the icon box).  The 
 *  client window may get made in the process.
 *
 *
 *  Inputs:
 *  ------
 *  pWS = pointer to workspace data
 *
 *  pCD = pointer to client window data structure
 *
 *  manageFlags = flags that indicate wm state info
 *
 * 
 *  Outputs:
 *  -------
 *  Return = pointer to an initialized client data structure or NULL
 *           if the client data could not be initialized
 *
 *************************************<->***********************************/
#ifdef _NO_PROTO
ClientData * 
GetWmClientInfo (pWS, pCD, manageFlags)
	WmWorkspaceData *pWS;
	ClientData * pCD;
	long manageFlags;
#else /* _NO_PROTO  */
ClientData * 
GetWmClientInfo (WmWorkspaceData *pWS,
			      ClientData * pCD,
			      long manageFlags)

#endif /* _NO_PROTO  */
{
    Pixmap 	iconBitmap;

    /*
     * Set up the client class and name for resource retrieval.
     * Get client specific resource data (from resources, .mwmrc).
     */

    if (manageFlags & MANAGEW_ICON_BOX)
    {
	SetIconBoxInfo (pWS, pCD);
    }
    else if (manageFlags & MANAGEW_CONFIRM_BOX)
    {
	pCD->clientClass = WmCConfirmbox;
	pCD->clientName = WmNconfirmbox;
	pCD->iconImage = NULL;
	pCD->useClientIcon = False;
	pCD->focusAutoRaise = True;
	pCD->internalBevel = FRAME_INTERNAL_SHADOW_WIDTH;
	pCD->matteWidth = 0;
	pCD->maximumClientSize.width = 0;
	pCD->maximumClientSize.height = 0;
	pCD->systemMenu = NULL;
    }


    /*
     * Set up transient for data.
     */

    if (manageFlags & MANAGEW_ICON_BOX)
    {
    }


    /*
     * Set up WM_HINTS type information.
     */

    pCD->inputFocusModel = True;
    pCD->clientState = NORMAL_STATE;
    
    if (ICON_DECORATION(pCD) & ICON_IMAGE_PART)
    {
	if (manageFlags & MANAGEW_ICON_BOX)
	{
	    pCD->clientFlags |= ICON_BOX;
	}
        
	if (!pCD->useClientIcon && pCD->iconImage)
        {
	    /*
	     * Make a client supplied icon image.
	     * Do not use the default icon image if iconImage is not found.
	     */

	    pCD->iconPixmap = MakeNamedIconPixmap (pCD, pCD->iconImage);
        }

	if (!pCD->iconPixmap)
        {
	    /*
	     * Use a built-in icon image for the window manager client.
	     * The image may differ from the default icon image, depending on
	     * the particular client (eg the iconbox).
	     */

	    if (manageFlags & MANAGEW_ICON_BOX)
	    {
		/*
		 * Make a default iconBox icon image.
		 */

		iconBitmap = XCreateBitmapFromData (DISPLAY, 
				ROOT_FOR_CLIENT(pCD), iconBox_bits, 
				iconBox_width, iconBox_height);

		pCD->iconPixmap = MakeIconPixmap (pCD, 
				iconBitmap, NULL,
				iconBox_width, iconBox_height);

	    }
        }
    }

    pCD->iconPlace = NO_ICON_PLACE;
    pCD->iconX = 0;
    pCD->iconY = 0;
    pCD->windowGroup = NULL;


    /*
     * Set up _MWM_HINTS data.
     */
    /*
     * Fix the client functions and decorations fields if they have
     * default resource values.
     */


    if (manageFlags & MANAGEW_CONFIRM_BOX)
    {
        pCD->clientFunctions = WM_FUNC_NONE;
        pCD->clientDecoration = WM_DECOR_BORDER;
    }
    else
    {
        if (pCD->clientFunctions & WM_FUNC_DEFAULT)
        {
	    pCD->clientFunctions = WM_FUNC_ALL;
        }

        if (pCD->clientDecoration & WM_DECOR_DEFAULT)
        {
	    pCD->clientDecoration = WM_DECOR_ALL;
        }

        if (manageFlags & MANAGEW_ICON_BOX)
        {
            pCD->clientFunctions &= ICON_BOX_FUNCTIONS;
        }


        if (!(pCD->clientFunctions & MWM_FUNC_RESIZE))
        {
	    pCD->clientDecoration &= ~MWM_DECOR_RESIZEH;
        }

        if (!(pCD->clientFunctions & MWM_FUNC_MINIMIZE))
        {
	    pCD->clientDecoration &= ~MWM_DECOR_MINIMIZE;
        }

        if (!(pCD->clientFunctions & MWM_FUNC_MAXIMIZE))
        {
	    pCD->clientDecoration &= ~MWM_DECOR_MAXIMIZE;
        }
    }

    pCD->decor = pCD->clientDecoration;

    if (manageFlags & MANAGEW_ICON_BOX)
    {
        pCD->inputMode = MWM_INPUT_MODELESS;
    }
    else if (manageFlags & MANAGEW_CONFIRM_BOX)
    {
        pCD->inputMode = MWM_INPUT_SYSTEM_MODAL;
    }

    /*
     * Set up WM_NORMAL_HINTS data.
     */

    pCD->icccVersion = ICCC_CURRENT;
    pCD->sizeFlags = US_POSITION | US_SIZE;

    /* 
     * Any calls to create Window Manager clients should
     * return with the values for the following fields set.
     *  If it fails, it should free any space allocated and
     *  set pCD = NULL
     * 
     *  pCD->clientX =
     *  pCD->clientY =
     *  pCD->clientWidth =
     *  pCD->clientHeight =
     *  pCD->minWidth =
     *  pCD->minHeight =
     *  pCD->widthInc =
     *  pCD->heightInc =
     *  pCD->baseWidth =
     *  pCD->baseHeight =
     *  pCD->maxWidth =
     *  pCD->maxHeight =
     *
     *        AND PROBABLY SHOULD SET
     *         pCD->client = THE_WINDOW_THE_FUNCTION_CREATES
     */

    pCD->windowGravity = NorthWestGravity;

    /* 
     * Create IconBox window 
     */
    
    if (manageFlags & MANAGEW_ICON_BOX)
    {
        if (!MakeIconBox (pWS, pCD))
        {
            /*
             *  May want a more verbose message here 
             */

            Warning ("Couldn`t make icon box");
	    return (NULL);
        }
    }
    else if (manageFlags & MANAGEW_CONFIRM_BOX)
    {
	Window       root;
	unsigned int cbWidth, cbHeight;
	unsigned int depth;

        XGetGeometry (DISPLAY, pCD->client, &root,
		      &(pCD->clientX), &(pCD->clientY),
		      &cbWidth, &cbHeight, 
		      (unsigned int*)&(pCD->xBorderWidth), &depth);

        pCD->clientWidth = cbWidth;
        pCD->clientHeight = cbHeight;
	pCD->minWidth = pCD->baseWidth = pCD->maxWidth = pCD->clientWidth;
	pCD->minHeight = pCD->baseHeight = pCD->maxHeight = pCD->clientHeight;
        pCD->widthInc = 1;
        pCD->heightInc = 1;
        pCD->clientFlags |= CONFIRM_BOX;
    }

    /*
     * Set offset from frame of client window (need client size information).
     */

    SetFrameInfo (pCD);


    /*
     * Register the client window to facilitate event handling.
     */

    XSaveContext (DISPLAY, pCD->client, wmGD.windowContextType, (caddr_t)pCD);


    /*
     * Set up WM_PROTOCOLS data.
     */

    pCD->clientProtocolCount = 0;
    pCD->protocolFlags = 0;


    /*
     * Make the system menu.
     */

    if (manageFlags & MANAGEW_ICON_BOX)
    {
	/* This MenuSpec is not added to pSD->acceleratorMenuSpecs */

	pCD->systemMenuSpec = 
	    MakeMenu (PSD_FOR_CLIENT(pCD), pCD->systemMenu, F_CONTEXT_WINDOW,
	              F_CONTEXT_WINDOW|F_CONTEXT_ICON, 
		      GetIconBoxMenuItems (PSD_FOR_CLIENT(pCD)), 
		      TRUE);
    }



    /*
     * Setup the colormap data.
     */

    pCD->clientColormap = PSD_FOR_CLIENT(pCD)->workspaceColormap;


    /*
     * Return the pointer to the client data.
     */

    return (pCD);


} /* END OF FUNCTION GetWmClientInfo */



/*************************************<->*************************************
 *
 *  ProcessWmClass (pCD)
 *
 *
 *  Description:
 *  -----------
 *  This function retrieves the contents of the WM_CLASS property on the
 *  cient window.  The resource class and the resource name are saved in
 *  the ClientData structure (note that the space for the strings is
 *  allocated using Xmalloc).
 *
 *
 *  Inputs:
 *  ------
 *  pCD		- pointer to client data
 *
 * 
 *  Outputs:
 *  -------
 *
 *  Comments:
 *  --------
 * 
 *************************************<->***********************************/

#ifdef _NO_PROTO
void 
ProcessWmClass (pCD)
	ClientData *pCD;
#else /* _NO_PROTO  */
void 
ProcessWmClass (ClientData *pCD)
#endif /* _NO_PROTO  */
{
    XClassHint classHint;


#ifdef PORT_OLDXLIB
    classHint.res_class = "";
    classHint.res_name = "";
    XGetClassHint (DISPLAY, pCD->client, &classHint);
#else
    if (XGetClassHint (DISPLAY, pCD->client, &classHint))
#endif
    {
	/* the WM_CLASS property exists for the client window */
	pCD->clientClass = classHint.res_class;
	pCD->clientName = classHint.res_name;
    }
    /* else no WM_CLASS property; assume clientClass, clientName are NULL */

} /* END OF FUNCTION ProcessWmClass */



/*************************************<->*************************************
 *
 *  ProcessWmHints (pCD, firstTime)
 *
 *
 *  Description:
 *  -----------
 *  This function retrieves the contents of the WM_HINTS property on the
 *  cient window.
 *
 *
 *  Inputs:
 *  ------
 *  pCD = pointer to client data for the window with the property
 *
 *  firstTime = if True this is the first time the property has been processed
 *
 * 
 *  Outputs:
 *  -------
 *  pCD = initialize various WM_HINTS related fields
 *
 *
 *  Comments:
 *  --------
 * 
 *************************************<->***********************************/

#ifdef _NO_PROTO
void 
ProcessWmHints (pCD, firstTime)
	ClientData *pCD;
	Boolean firstTime;

#else /* _NO_PROTO */
void 
ProcessWmHints (ClientData *pCD, Boolean firstTime)
#endif /* _NO_PROTO */
{
    register XWMHints *pXWMHints;
    register long flags;
    Pixmap iconPixmap;



    /*
     * If the WM_HINTS property does not exist the flags field will be
     * set to 0.  If flags is 0 don't reference the WMHints structure
     * since they may be none.
     */

    pXWMHints = XGetWMHints (DISPLAY, pCD->client);

    if (pXWMHints)
    {
	flags = pXWMHints->flags;
    }
    else
    {
	flags = 0;
    }


    /*
     * Parse the WM_HINTS information.  If this is the first time the hints
     * have been processed then parse all fields and set defaults where hint
     * fields are not set.  If this is not the first time do selective
     * parsing.
     */

    if (firstTime)
    {
        /*
         * The window manger does not do anything with the input hint.  Input
         * always goes to the selected window.
         */

        if (flags & InputHint)
	{
	    pCD->inputFocusModel = pXWMHints->input;
	}
	else
	{
	    pCD->inputFocusModel = True;
	}


        /*
         *  The default state is NORMAL_STATE.  States other than iconic
         *  (e.g., ZoomState from the R2 ICCC) indicate to the window manager
         *  that the NORMAL_STATE is to be used.
         */

        if ((flags & StateHint) && (pXWMHints->initial_state == IconicState) &&
	    (pCD->clientFunctions & MWM_FUNC_MINIMIZE))
        {
	    pCD->clientState = MINIMIZED_STATE;
	}
        else
        {
	    /*
	     * States other than iconic are treated as normal.
	     */
	    pCD->clientState = NORMAL_STATE;
        }




        /*
	 * If an icon is to be made for the client then ...
         * save the icon image if useClientIcon is True or there is no
	 * user specified icon image.  A client supplied image may be a
	 * pixmap or a window (a client icon window takes precedence over
	 * a pixmap).
         */

	if ((pCD->clientFunctions & MWM_FUNC_MINIMIZE) &&
	    (pCD->transientLeader == NULL))
	{
            if ((ICON_DECORATION(pCD) & ICON_IMAGE_PART) &&
	        (pCD->useClientIcon || !pCD->iconImage))
            {
	        if ((flags & IconWindowHint) &&
		    (pXWMHints->icon_window != pCD->client))
	        {
	            /* 
	             * An icon window has been supplied that is different from
		     * the client window.  Check out the window and get it 
		     * ready to be reparented to the window manager supplied
	             * icon frame.
	             */

	            if (!SetupClientIconWindow (pCD, pXWMHints->icon_window))
		    {
		        /*
		         * Cannot use the client supplied icon window.  Use
		         * an icon image if specified or a default image.
		         */
		    }
	        }
	        if (!pCD->iconWindow && (flags & IconPixmapHint))
	        {
	            /*
	             * A client supplied icon window is NOT 
		     * available so use the client supplied icon image.
	             */

	            if (pCD->iconPixmap = MakeClientIconPixmap (pCD,
					      pXWMHints->icon_pixmap))
		    {
		        /*
		         * Indicate that a client supplied icon image is being
		         * used.
		         */

		        pCD->iconFlags |= ICON_HINTS_PIXMAP;
		    }
		    else
		    {
		        /*
		         * Cannot make a client supplied image.  Use a user
		         * specified icon image if it is available or a default
		         * icon image.
		         */
		    }
	        }
	    }

            if ((ICON_DECORATION(pCD) & ICON_IMAGE_PART) && !pCD->iconPixmap)
	    {
	        /*
	         * Use a user supplied icon image if it is available or a
	         * default icon image.
	         */

	        if (pCD->iconImage)
	        {
		    /*
		     * Try to make a user specified icon image.
		     */

		    pCD->iconPixmap = 
			MakeNamedIconPixmap (pCD, pCD->iconImage);
	        }

	        if (!pCD->iconPixmap)
	        {
		    /*
		     * The icon image was not provided or not available.
		     * Use the default icon image.
		     */

		    pCD->iconPixmap = MakeNamedIconPixmap (pCD, NULL);
	        }
	    }
    

            /*
             * Save the client (user?) supplied icon position:
             */

            if (flags & IconPositionHint)
            {
	        pCD->iconFlags |= ICON_HINTS_POSITION;
	        if (wmGD.iconAutoPlace)
	        {
		    pCD->iconPlace = 
			FindIconPlace (pCD, &(pCD->pSD->pActiveWS->IPData),
			    pXWMHints->icon_x, pXWMHints->icon_y);
		    if (pCD->iconPlace != NO_ICON_PLACE)
		    {
		        CvtIconPlaceToPosition (&(pCD->pSD->pActiveWS->IPData),
			    pCD->iconPlace, &pCD->iconX, &pCD->iconY);
		    }
	        }
	        else
	        {
		    pCD->iconX = pXWMHints->icon_x;
		    pCD->iconY = pXWMHints->icon_y;
                }
	    }
	    else
	    {
	        if (wmGD.iconAutoPlace)
	        {
		    pCD->iconPlace = NO_ICON_PLACE;
		    pCD->iconX = 0;
		    pCD->iconY = 0;
	        }
	    }
	}


        /*
         * Save the window group.
         */

        if (flags & WindowGroupHint)
	{
	    pCD->windowGroup = pXWMHints->window_group;
	}
	else
	{
	    pCD->windowGroup = NULL;
	}

    }
    else /* not the first time the hints are processed */
    {
	if (flags & IconPixmapHint)
	{
	    /*
             * Process an icon image change if the icon image was initially
             * set up with a client supplied icon image OR, if the client
             * now wants to supply an image.
             */

	    if (iconPixmap = MakeClientIconPixmap (pCD,
						   pXWMHints->icon_pixmap))
	    {
		/*
		 * Made new icon image; free up the old image and display
		 * the new image.
		 */
		if (pCD->iconFlags & ICON_HINTS_PIXMAP)
                {
                    /*
                     * ICON_HINTS_PIXMAP was set either initally or
                     * below because a new pixmap was made for the client.
                     * It is now safe to free the previous pixmap since it
                     * is not the shared default iconPixmap
                     */
                    if (pCD->iconPixmap)
                    {
                        XFreePixmap (DISPLAY, pCD->iconPixmap);
                    }
                }
                else
                {
                    pCD->iconFlags |= ICON_HINTS_PIXMAP;
                }
		
                pCD->iconPixmap = iconPixmap;
		
                /*
                 * Display new icon image if the icon is showing:
                 */
		
                if (((pCD->clientState == MINIMIZED_STATE) ||
                     ((pCD->pSD->useIconBox) && (P_ICON_BOX(pCD)))) &&
                    ICON_FRAME_WIN(pCD))
                {
                    IconExposureProc (pCD, True);
                }
	    }
	}
    }

    if (pXWMHints)
    {
	XFree ((char*)pXWMHints);
    }


} /* END OF FUNCTION ProcessWmHints */



/*************************************<->*************************************
 *
 *  ProcessWmNormalHints (pCD, firstTime, manageFlags)
 *
 *
 *  Description:
 *  -----------
 *  This function retrieves the contents of the WM_NORMAL_HINTS property on
 *   the cient window.  There are several versions of the property that must be
 *  handled (currently R2 and CURRENT).
 *
 *
 *  Inputs:
 *  ------
 *  pCD = pointer to client data for the window with the property
 *
 *  firstTime = if True this is the first time the property has been processed
 *
 *  manageFlags = flags that indicate wm state information
 *
 * 
 *  Outputs:
 *  -------
 *  pCD = client location and size fields set
 *
 *
 *  Comments:
 *  --------
 *  If the hints are being reprocessed (!firstTime) the configuration values
 *  will be ignored.  The size constraint values will be processed but the
 *  client configuration will not be changed even if it is not in line with
 *  the new values.  Reconfigurations subsequent to the hints changes will
 *  be done with the new constraints.
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void 
ProcessWmNormalHints (pCD, firstTime, manageFlags)

    ClientData *pCD;
    Boolean     firstTime;
    long        manageFlags;

#else /* _NO_PROTO */
void 
ProcessWmNormalHints (ClientData *pCD, Boolean firstTime, long manageFlags)
#endif /* _NO_PROTO */
{
    register SizeHints *pNormalHints;
    register long       flags;
    int                 diff;
    unsigned long       decoration;
    unsigned int        boxdim, tmpMin;

    /*
     * Use a custom verion of the Xlib routine to get WM_NORMAL_HINTS.
     * A custom version is necessary to handle the different versions
     * of WM_NORMAL_HINTS that may be encountered.  If the WM_NORMAL_HINTS
     * property does not exist the flags field will be set to 0.
     */

    pNormalHints = GetNormalHints (pCD);

    pCD->icccVersion = pNormalHints->icccVersion;


    /*
     * Parse the WM_NORMAL_HINTS information:
     */

    if (((flags = pNormalHints->flags) == 0) && !firstTime)
    {
	return;
    }


    /*
     * Process the size only if this is the first time the hints are
     * being processed for the window.
     */

    if (firstTime)
    {
        /*
         * Process client window size flags and information:
         */

        pCD->sizeFlags = flags & (US_POSITION | US_SIZE | P_POSITION | P_SIZE);

        /*
         * The R2 conventions and Xlib manual indicate that the window size
         * and position should be taken out of the WM_NORMAL_HINTS property
         * if they are specified there.  The current conventions indicate that
         * the size and position information should be gotten from the window
         * configuration. Mwm 1.1 always uses the current conventions.
         */

	pCD->clientX = wmGD.windowAttributes.x;
	pCD->clientY = wmGD.windowAttributes.y;

	/*
	 * Use current conventions for initial window dimensions.
	 */

	pCD->clientWidth = wmGD.windowAttributes.width;
	pCD->clientHeight = wmGD.windowAttributes.height;
    }

    /*
     * Process the minimum size:
     */

    if (flags & P_MIN_SIZE)
    {
	pCD->minWidth =
		 (pNormalHints->min_width < 0) ? 0 : pNormalHints->min_width;
	pCD->minHeight =
		 (pNormalHints->min_height < 0) ? 0 : pNormalHints->min_height;
	if (pCD->minWidth > MAX_MAX_SIZE(pCD).width)
	{
	    pCD->minWidth = MAX_MAX_SIZE(pCD).width;
	}
	if (pCD->minHeight > MAX_MAX_SIZE(pCD).height)
	{
	    pCD->minHeight = MAX_MAX_SIZE(pCD).height;
	}
    }
    else if (firstTime)
    {
	pCD->minWidth = 0;
	pCD->minHeight = 0;
    }


    /*
     * Process the resizing increments:
     */

    if (flags & P_RESIZE_INC)
    {
	pCD->widthInc =
		 (pNormalHints->width_inc < 1) ? 1 : pNormalHints->width_inc;
	pCD->heightInc =
		 (pNormalHints->height_inc < 1) ? 1 : pNormalHints->height_inc;
    }
    else if (firstTime)
    {
	pCD->widthInc = 1;
	pCD->heightInc = 1;
    }


    /*
     * Process the base size:
     */

    if (flags & P_BASE_SIZE)
    {
	pCD->baseWidth =
		(pNormalHints->base_width < 0) ? 0 : pNormalHints->base_width;
	pCD->baseHeight =
		(pNormalHints->base_height < 0) ? 0 : pNormalHints->base_height;
    }
    else if ((pNormalHints->icccVersion == ICCC_R2) &&
	     ((firstTime) ||
	      (!firstTime && (flags & P_MIN_SIZE))))
    {
	/*
	 * In this version of the hints the minimum size was effectively
	 * the base size.
	 */
	pCD->baseWidth = pCD->minWidth;
	pCD->baseHeight = pCD->minHeight;
    }
    else if (firstTime)
    {
        if (flags & P_MIN_SIZE)
        {
            pCD->baseWidth = pCD->minWidth;
            pCD->baseHeight = pCD->minHeight;
        }
        else
        {
            pCD->baseWidth = 0;
            pCD->baseHeight = 0;
        }
    }


    /*
     * Process the maximum size.
     */

    if (pCD->maximumClientSize.width)
    {
	pCD->maxWidth = (pCD->maximumClientSize.width * 
			pCD->widthInc) + pCD->baseWidth;
    }
    else
    {
	if (flags & P_MAX_SIZE)
	{
	    if (pNormalHints->max_width < 0)
	    {
	        /* go to min (full screen width, max maximum width) */
		pCD->maxWidth = DisplayWidth (DISPLAY, SCREEN_FOR_CLIENT(pCD)) -
				          (2 * pCD->clientOffset.x);
	    }
	    else
	    {
	        pCD->maxWidth = pNormalHints->max_width;
	    }
	}
	else if (firstTime)
	{
	    /* go to min (full screen width, max maximum width */
	    pCD->maxWidth = DisplayWidth (DISPLAY, SCREEN_FOR_CLIENT(pCD)) -
			    (2 * pCD->clientOffset.x);
	}
	else
	{
	    /* reset the maxHeight before further processing */
	    pCD->maxWidth = pCD->maxWidthLimit;
	}
	if (pCD->maxWidth > MAX_MAX_SIZE(pCD).width)
	{
	    pCD->maxWidth = MAX_MAX_SIZE(pCD).width;
	}
    }

    if (pCD->maximumClientSize.height)
    {
	pCD->maxHeight = (pCD->maximumClientSize.height *
			 pCD->heightInc) + pCD->baseHeight;
    }
    else
    {
	if (flags & P_MAX_SIZE)
	{
	    if (pNormalHints->max_height < 0)
	    {
	        /* go to min (full screen height, max maximum height */
	        pCD->maxHeight = DisplayHeight (
				 DISPLAY, SCREEN_FOR_CLIENT(pCD)) -
				 (pCD->clientOffset.x +
				  pCD->clientOffset.y);
	    }
	    else
	    {
	        pCD->maxHeight = pNormalHints->max_height;
	    }
	}
	else if (firstTime)
	{
	    /* go to min (full screen height, max maximum height */
	    pCD->maxHeight = DisplayHeight (DISPLAY, SCREEN_FOR_CLIENT(pCD)) -
			     (pCD->clientOffset.x +
			      pCD->clientOffset.y);
	}
	else
	{
	    /* reset the maxHeight before further processing */
	    pCD->maxHeight = pCD->maxHeightLimit;
	}
	if (pCD->maxHeight > MAX_MAX_SIZE(pCD).height)
	{
	    pCD->maxHeight = MAX_MAX_SIZE(pCD).height;
	}
    }

    /*
     * Get the initial aspect ratios, if available.  Only use them if:
     *
     *   minAspect.y > 0
     *   maxAspect.y > 0
     *   0 <= minAspect.x / minAspect.y <= maxAspect.x / maxAspect.y 
     */

    if (flags & P_ASPECT)
    {
	pCD->minAspect.x = pNormalHints->min_aspect.x;
	pCD->minAspect.y = pNormalHints->min_aspect.y;
	pCD->maxAspect.x = pNormalHints->max_aspect.x;
	pCD->maxAspect.y = pNormalHints->max_aspect.y;

        if (pCD->minAspect.y > 0 &&
	    pCD->maxAspect.y > 0 &&
	    pCD->minAspect.x > 0 &&
	    pCD->maxAspect.x > 0 &&
	    (pCD->minAspect.x * pCD->maxAspect.y <=
	    pCD->maxAspect.x * pCD->minAspect.y))
        {
	    pCD->sizeFlags |= P_ASPECT;
	}
	else
	{
	    pCD->sizeFlags &= ~P_ASPECT;
	}
    }

    /* compute for minimum frame size */
    if ((decoration = pCD->decor) & MWM_DECOR_TITLE)
    {
	boxdim = TitleBarHeight(pCD);
	tmpMin = boxdim +
	         ((decoration & MWM_DECOR_MENU) ? boxdim : 0) +
	         ((decoration & MWM_DECOR_MINIMIZE) ? boxdim : 0) +
	         ((decoration & MWM_DECOR_MAXIMIZE) ? boxdim : 0) -
		 2*(pCD->matteWidth);
    }
    else {
	tmpMin = 0;
    }


    /*
     * Process the window gravity (for positioning):
     */

    if (flags & P_WIN_GRAVITY)
    {
	pCD->windowGravity = pNormalHints->win_gravity;
    }
    else
    {
        if (pNormalHints->icccVersion == ICCC_R2)
	{
	    pCD->windowGravity = wmGD.windowAttributes.win_gravity;
	}
	else
	{
	    pCD->windowGravity = NorthWestGravity;
	}
    }


    /*
     * Make sure that all the window sizing constraints are compatible:
     */

    /*
     * Make:
     *
     *   minWidth >= tmpMin
     *   minWidth >= max (baseWidth, widthInc) > 0
     *     & an integral number of widthInc from baseWidth.
     *   minHeight >= max (baseHeight, heightInc) > 0
     *     & an integral number of heightInc from baseHeight.
     */

    if (pCD->minWidth < tmpMin)
    {
        if (diff = ((tmpMin - pCD->baseWidth)%pCD->widthInc))
        {
	    pCD->minWidth = tmpMin + pCD->widthInc - diff;
        }
        else
        {
	    pCD->minWidth = tmpMin;
        }
    }

    if (pCD->minWidth < pCD->baseWidth)
    {
	pCD->minWidth = pCD->baseWidth;
    }

    if (pCD->minWidth == 0)
    {
	pCD->minWidth = pCD->widthInc;
    }
    else if ((diff = ((pCD->minWidth - pCD->baseWidth)%pCD->widthInc)) != 0)
    {
	pCD->minWidth += pCD->widthInc - diff;
    }

    if (pCD->minHeight < pCD->baseHeight)
    {
	pCD->minHeight = pCD->baseHeight;
    }

    if (pCD->minHeight == 0)
    {
	pCD->minHeight = pCD->heightInc;
    }
    else if (diff = ((pCD->minHeight - pCD->baseHeight) % pCD->heightInc))
    {
	pCD->minHeight += pCD->heightInc - diff;
    }

    /*
     * Make:
     *
     *   maxWidth >= minWidth 
     *     & an integral number of widthInc from baseWidth.
     *   maxHeight >= minHeight
     *     & an integral number of heightInc from baseHeight.
     */

    if (pCD->maxWidth < pCD->minWidth)
    {
	pCD->maxWidth = pCD->minWidth;
    }

    pCD->maxWidthLimit = pCD->maxWidth;

    if (diff =((pCD->maxWidth - pCD->baseWidth)% pCD->widthInc))
    {
	pCD->maxWidth -= diff;
    }

    if (pCD->maxHeight < pCD->minHeight)
    {
	pCD->maxHeight = pCD->minHeight;
    }

    pCD->maxHeightLimit = pCD->maxHeight;

    if (diff =((pCD->maxHeight - pCD->baseHeight)% pCD->heightInc))
    {
	pCD->maxHeight -= diff;
    }

    /*
     * If using aspect ratios, make:
     *
     *  minWidth / maxHeight <= minAspect.x / minAspect.y 
     *                       <= maxAspect.x / maxAspect.y
     *                       <= maxWidth / minHeight
     */

    if (pCD->sizeFlags & P_ASPECT)
    {
        if (pCD->minWidth * pCD->minAspect.y >
	    pCD->minAspect.x * pCD->maxHeight)
        {
            pCD->minAspect.x = pCD->minWidth;
            pCD->minAspect.y = pCD->maxHeight;
	}

        if (pCD->maxAspect.x * pCD->minHeight >
	    pCD->maxWidth * pCD->maxAspect.y)
        {
            pCD->maxAspect.x = pCD->maxWidth;
            pCD->maxAspect.y = pCD->minHeight;
	}

        FixWindowSize (pCD, (unsigned int *) &(pCD->maxWidth),
	                    (unsigned int *) &(pCD->maxHeight),
			    (unsigned int) (pCD->widthInc), 
			    (unsigned int) (pCD->heightInc));
    }

    /*
     * If this is the first time, make sure the client dimensions are within 
     * range and that they satisfy any aspect ratio constraints:
     *
     *  0 < minWidth  <= clientWidth  <= maxWidth
     *  0 < minHeight <= clientHeight <= maxHeight
     *
     *  minAspect.x / minAspect.y <= clientWidth / clientHeight
     *                            <= maxAspect.x / maxAspect.y
     *
     * Initial max width/height are set to max of max size or normal
     * client size unless a maximumClientSize was specified.
     */

    if (firstTime)
    {
	if (!pCD->maximumClientSize.width)
	{
	    if (pCD->clientWidth > pCD->pSD->maximumMaximumSize.width)
	    {
		pCD->clientWidth = pCD->pSD->maximumMaximumSize.width;
	    }
	}

	if (!pCD->maximumClientSize.height)
	{
	    if (pCD->clientHeight > pCD->pSD->maximumMaximumSize.height)
	    {
		pCD->clientHeight = pCD->pSD->maximumMaximumSize.height;
	    }
	}

        FixWindowSize (pCD, (unsigned int *) &(pCD->clientWidth), 
	                    (unsigned int *) &(pCD->clientHeight),
			    (unsigned int) (pCD->widthInc), 
			    (unsigned int) (pCD->heightInc));
    }

} /* END OF FUNCTION ProcessWmNormalHints */


/*************************************<->*************************************
 *
 *  ProcessWmWindowTitle (pCD, firstTime)
 *
 *
 *  Description:
 *  -----------
 *  This function retrieves the contents of the WM_NAME property on the
 *  cient window.  A default name is set if the property does not exist.
 *
 *
 *  Inputs:
 *  ------
 *  pCD		- pointer to client data structure
 *  firstTime	- false if the window is already managed and the title
 *                is being changed.
 *
 * 
 *  Outputs:
 *  -------
 *  pCD		- clientTitle, iconTitle
 * 
 *************************************<->***********************************/

#ifdef _NO_PROTO
void 
ProcessWmWindowTitle (pCD, firstTime)
	ClientData *pCD;
	Boolean firstTime;

#else /* _NO_PROTO */
void 
ProcessWmWindowTitle (ClientData *pCD, Boolean firstTime)
#endif /* _NO_PROTO */
{
    XTextProperty wmNameProp;


    if ((pCD->clientDecoration & MWM_DECOR_TITLE) &&
	XGetWMName(DISPLAY, pCD->client, &wmNameProp) && 
	wmNameProp.value && strlen(wmNameProp.value) > 0)
    {
	if (!firstTime && (pCD->iconTitle == pCD->clientTitle))
	{
	    /*
	     * The client window title is being used for the icon title so
	     * change the icon title with the window title.
	     */
	    pCD->iconTitle = XmCvtCTToXmString((char*)wmNameProp.value);
	    RedisplayIconTitle (pCD);
	}

	if (pCD->clientFlags & CLIENT_HINTS_TITLE)
	{
	    XmStringFree (pCD->clientTitle);
	}

	pCD->clientTitle = XmCvtCTToXmString((char*)wmNameProp.value);
	pCD->clientFlags |= CLIENT_HINTS_TITLE;

	if (!firstTime)
	{
	    DrawWindowTitle (pCD, True);
	}
    }
    /*
     * The client frame does not have a place to put the title or the WM_NAME
     * property does not exist or there was some error in getting
     * the property information, so use a default value.
     */
    else if (firstTime)
    {
	if (pCD->clientName)
        {
	    pCD->clientTitle = XmStringCreateLtoR(pCD->clientName,
                                                  XmSTRING_DEFAULT_CHARSET);
        }
        else
        {
	    pCD->clientTitle = wmGD.clientDefaultTitle;
        }
    }

} /* END OF FUNCTION ProcessWmWindowTitle */



/*************************************<->*************************************
 *
 *  ProcessWmIconTitle (pCD, firstTime)
 *
 *
 *  Description:
 *  -----------
 *  This function retrieves the contents of the WM_ICON_NAME property on the
 *  cient window.  The value of the property is a string that is used for the
 *  icon title.  A default title is set if the property does not exist.
 *
 *
 *  Inputs:
 *  ------
 *  pCD		- pointer to client data structure
 *
 *  firstTime	- false if the window is already managed and the title
 *                is being changed.
 *
 * 
 *  Outputs:
 *  -------
 *  pCD		- iconTitle
 * 
 *************************************<->***********************************/

#ifdef _NO_PROTO
void 
ProcessWmIconTitle (pCD, firstTime)
	ClientData *pCD;
	Boolean firstTime;

#else /* _NO_PROTO */
void 
ProcessWmIconTitle (ClientData *pCD, Boolean firstTime)
#endif /* _NO_PROTO */
{
    XTextProperty wmIconNameProp;


    if ((pCD->clientFunctions & MWM_FUNC_MINIMIZE) &&
	(pCD->transientLeader == NULL) &&
	XGetWMIconName (DISPLAY, pCD->client, &wmIconNameProp) &&
        wmIconNameProp.value  && strlen(wmIconNameProp.value) > 0)
    {
	if (pCD->iconFlags & ICON_HINTS_TITLE)
	{
	    XmStringFree (pCD->iconTitle);
	}

	pCD->iconTitle = XmCvtCTToXmString((char*)wmIconNameProp.value);
	pCD->iconFlags |= ICON_HINTS_TITLE;

	if (!firstTime)
	{
	    RedisplayIconTitle (pCD);
	}
    }
    /*
     * The WM_ICON_NAME property does not exist (or there was some error
     * in getting * the property information), so use a default value.
     */
    else if (firstTime)
    {
        if (pCD->clientTitle && 
	    (pCD->clientTitle != wmGD.clientDefaultTitle))
        {
	    pCD->iconTitle = pCD->clientTitle;
        }
        else
        {
	    pCD->iconTitle = wmGD.iconDefaultTitle;
        }
    }

} /* END OF FUNCTION ProcessWmIconTitle */



/*************************************<->*************************************
 *
 *  ProcessWmTransientFor (pCD)
 *
 *
 *  Description:
 *  -----------
 *  This function retrieves the contents of the WM_TRANSIENT_FOR property on
 *  the cient window.
 *
 *
 *  Inputs:
 *  ------
 *  pCD = pointer to the client data structure for the window with the property
 *
 * 
 *  Outputs:
 *  -------
 *  pCD.transientFor = if tranient then this is the associated main window
 *
 *  pCD.clientFlags = indicate that this is a transient window
 * 
 *************************************<->***********************************/

#ifdef _NO_PROTO
void 
ProcessWmTransientFor (pCD)
	ClientData *pCD;

#else /* _NO_PROTO */
void 
ProcessWmTransientFor (ClientData *pCD)
#endif /* _NO_PROTO */
{
    Window window;
    ClientData *leader;


    if (XGetTransientForHint (DISPLAY, pCD->client, &window))
    {
	pCD->clientFlags |= CLIENT_TRANSIENT;

	/*
	 * Only save the (leader) transientFor window if it is NOT the
	 * client window and it is already managed by the window manager.
	 */

	if ((pCD->client != window) &&
	    !XFindContext (DISPLAY, window, wmGD.windowContextType,
		(caddr_t *)&leader))
	{
	    pCD->transientFor = window;
	    pCD->transientLeader = leader;
	}
    }
    /* else this is not a transient window */


} /* END OF FUNCTION ProcessWmTransientFor */



/*************************************<->*************************************
 *
 *  MakeSystemMenu (pCD)
 *
 *
 *  Description:
 *  -----------
 *  This function finds or makes a system menu for the client.  A check
 *  is made for the _MWM_MENU property and, if present, client-specific
 *  items are added to the custom system menu.  Any custom system menu
 *  must be destroyed when the client is unmanaged (or killed).
 *
 *
 *  Inputs:
 *  ------
 *  pCD = pointer to the client data structure for the managed window
 *
 * 
 *  Outputs:
 *  -------
 *  pCD.systemMenuSpec = system menu specification for the client, not added
 *                       to wmGD.acceleratorMenuSpecs 
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void 
MakeSystemMenu (pCD)
	ClientData *pCD;

#else /* _NO_PROTO */
void 
MakeSystemMenu (ClientData *pCD)
#endif /* _NO_PROTO */
{

    pCD->mwmMenuItems = GetMwmMenuItems(pCD);
    pCD->systemMenuSpec = 
       MakeMenu (PSD_FOR_CLIENT(pCD), pCD->systemMenu, F_CONTEXT_WINDOW, 
	         F_CONTEXT_WINDOW|F_CONTEXT_ICON, pCD->mwmMenuItems, TRUE);

} /* END OF FUNCTION MakeSystemMenu */



/*************************************<->*************************************
 *
 *  InitCColormapData (pCD)
 *
 *
 *  Description:
 *  -----------
 *  This function initializes colormap data for the client window that is
 *  by the window manager in maintaining the colormap focus.  This may
 *  involve retrieving and processing properties that deal with subwindow
 *  colormaps.
 *
 *
 *  Inputs:
 *  ------
 *  pCD = pointer to the client data structure for the managed window
 *
 * 
 *  Outputs:
 *  -------
 *  pCD.clientColormap = client colormap to be installed when the client
 *                       window gets the colormap focus
 *
 *  pCD = (cmapWindows, clientCmapList, clientCmapCount, clientCmapIndex)
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void 
InitCColormapData (pCD)
	ClientData *pCD;

#else /* _NO_PROTO */
void 
InitCColormapData (ClientData *pCD)
#endif /* _NO_PROTO */
{

    if (wmGD.windowAttributes.colormap == None)
    {
	pCD->clientColormap = WORKSPACE_COLORMAP(pCD);
    }
    else
    {
	pCD->clientColormap = wmGD.windowAttributes.colormap;
    }

    /*
     * Process subwindow colormap windows if they are specified.
     */

    ProcessWmColormapWindows (pCD);
    

} /* END OF FUNCTION InitCColormapData */



/*************************************<->*************************************
 *
 *  CalculateGravityOffset (pCD, xoff, yoff)
 *
 *
 *  Description:
 *  -----------
 *  This function calculates the window offsets based on the window gravity
 *  and the window frame client offset.
 *
 *
 *  Inputs:
 *  ------
 *  pCD = pointer to client data (client window configuration fields)
 *  xoff = pointer to xoffset
 *  yoff = pointer to yoffset
 *
 * 
 *  Outputs:
 *  -------
 *  xoff = pointer to xoffset set
 *  yoff = pointer to yoffset set
 * 
 *************************************<->***********************************/

#ifdef _NO_PROTO
void
CalculateGravityOffset (pCD, xoff, yoff)
	ClientData *pCD;
	int *xoff;
	int *yoff;

#else /* _NO_PROTO */
void
CalculateGravityOffset (ClientData *pCD, int *xoff, int *yoff)
#endif /* _NO_PROTO */
{
    int borderWidth = pCD->xBorderWidth;

    if (pCD->windowGravity < ForgetGravity ||
	pCD->windowGravity > StaticGravity)
    {
	*xoff = 0;
	*yoff = 0;
    }
    else
    {
	switch (pCD->windowGravity)
	{
	    case NorthWestGravity:
	    default:
	    {
		*xoff = pCD->clientOffset.x;
		*yoff = pCD->clientOffset.y;
		break;
	    }

	    case NorthGravity:
	    {
		*xoff = borderWidth;
		*yoff = pCD->clientOffset.y;
		break;
	    }
                
	    case NorthEastGravity:
	    {
		*xoff = -(pCD->clientOffset.x - (2 * borderWidth));
		*yoff = pCD->clientOffset.y;
		break;
	    }

	    case EastGravity:
	    {
		*xoff = -(pCD->clientOffset.x - (2 * borderWidth));
		*yoff = borderWidth +
				(pCD->clientOffset.y - pCD->clientOffset.x)/2;
		break;
	    }

	    case SouthEastGravity:
	    {
		*xoff = -(pCD->clientOffset.x - (2 * borderWidth));
		*yoff = -(pCD->clientOffset.x - (2 * borderWidth));
		break;
	    }

	    case SouthGravity:
	    {
		*xoff = borderWidth;
		*yoff = -(pCD->clientOffset.x - (2 * borderWidth));
		break;
	    }

	    case SouthWestGravity:
	    {
		*xoff = pCD->clientOffset.x;
		*yoff = -(pCD->clientOffset.x - (2 * borderWidth));
		break;
	    }

	    case WestGravity:
	    {
		*xoff = pCD->clientOffset.x;
		*yoff = borderWidth +
				(pCD->clientOffset.y - pCD->clientOffset.x)/2;
		break;
	    }

	    case CenterGravity:
	    {
		*xoff = 0;
		*yoff = 0;
		break;
	    }
	}
    }
} /* END OF FUNCTION CalculateGravityOffset */



/*************************************<->*************************************
 *
 *  InitClientPlacement (pCD, manageFlags)
 *
 *
 *  Description:
 *  -----------
 *  This function sets up the initial client window placement (for both
 *  the normal and maximized state).
 *
 *
 *  Inputs:
 *  ------
 *  pCD = pointer to client data (client window configuration fields)
 *
 *  manageFlags = flags that indicate wm state information (e.g. whether
 *                the window manager is starting up or restarting)
 *
 * 
 *  Outputs:
 *  -------
 *  Return = True if position changed by this routine.
 *  pCD = changes to the client window configuration fields
 * 
 *************************************<->***********************************/

#ifdef _NO_PROTO
Boolean 
InitClientPlacement (pCD, manageFlags)
	ClientData *pCD;
	long manageFlags;

#else /* _NO_PROTO */
Boolean 
InitClientPlacement (ClientData *pCD, long manageFlags)
#endif /* _NO_PROTO */
{
    Boolean interactivelyPlaced = False;
    Boolean autoPlaced = False;
    Boolean rval = False;
    int xoff, yoff;
    int origX, origY, origWidth, origHeight;


    /*
     * Save initial client values
     */
    origX = pCD->clientX;
    origY = pCD->clientY;
    origWidth = pCD->clientWidth;
    origHeight = pCD->clientHeight;

    /*
     * Do interactive placement if...
     *     + the resource is turned on 
     *     + the window's coming up on the active screen
     *
     * Don't do it if...
     *     + the user has specified a position
     *     + the window is coming up iconic 
     *     + if it is a transient window
     *     + we're system modal
     */

    if (wmGD.interactivePlacement && 
	!(pCD->sizeFlags & US_POSITION) &&
	(pCD->clientState != MINIMIZED_STATE) &&
        (manageFlags == MANAGEW_NORMAL) &&
	!(pCD->clientFlags & CLIENT_TRANSIENT) &&
	(pCD->inputMode != MWM_INPUT_SYSTEM_MODAL) &&
	(PSD_FOR_CLIENT(pCD) == ACTIVE_PSD))
    {
	/*
	 * Interactively place the window on the screen.
	 */
	interactivelyPlaced = True;
	PlaceWindowInteractively (pCD);
    }


    /*
     * Check out the configuration values to insure that they are within
     * the constraints.
     */

    FixWindowConfiguration (pCD, (unsigned int *) &(pCD->clientWidth), 
	                         (unsigned int *) &(pCD->clientHeight),
				 (unsigned int) (pCD->widthInc), 
	                         (unsigned int) (pCD->heightInc));

    /*
     * Do autoplacement of the client window if appropriate.
     */

    if ((manageFlags == MANAGEW_NORMAL) && !interactivelyPlaced &&
	!(pCD->sizeFlags & US_POSITION) &&
	!(pCD->clientFlags & CLIENT_TRANSIENT) &&
	(pCD->inputMode != MWM_INPUT_SYSTEM_MODAL) && wmGD.clientAutoPlace)
    {
	FindClientPlacement (pCD);
	autoPlaced = True;
    }

    /*
     * Do PositionIsFrame processing:
     * Use window gravity to allow the user to specify the window 
     * position on the screen  without having to know the dimensions 
     * of the decoration that mwm is adding.
     */

    if ((wmGD.positionIsFrame) &&
	!interactivelyPlaced && !autoPlaced)
    {
	CalculateGravityOffset (pCD, &xoff, &yoff);
	pCD->clientX += xoff;
	pCD->clientY += yoff;
    }


    /*
     * Do PositionOnScreen processing:
     */


    if ((wmGD.positionOnScreen) && !interactivelyPlaced)
    {
	PlaceFrameOnScreen (pCD, &pCD->clientX, &pCD->clientY,
	    pCD->clientWidth, pCD->clientHeight);
    }


    /*
     * Position the maximized frame:
     */

    pCD->maxX = pCD->clientX;
    pCD->maxY = pCD->clientY;
    PlaceFrameOnScreen (pCD, &pCD->maxX, &pCD->maxY, pCD->maxWidth,
	pCD->maxHeight);


    if (!wmGD.iconAutoPlace)
    {
	if (!(pCD->iconFlags & ICON_HINTS_POSITION))
	{
	    pCD->iconX = pCD->clientX;
	    pCD->iconY = pCD->clientY;
	}
	PlaceIconOnScreen (pCD, &pCD->iconX, &pCD->iconY);
    }

    /* 
     * if client size or position has been changed by this routine,
     * then indicate in return value
     */
    if ((origX != pCD->clientX) || (origY != pCD->clientY) ||
	(origWidth != pCD->clientWidth) || (origHeight != pCD->clientHeight))
    {
	rval = True;
    }

    return (rval);

} /* END OF FUNCTION InitClientPlacement */



/*************************************<->*************************************
 *
 *  PlaceFrameOnScreen (pCD, pX, pY, w, h)
 *
 *
 *  Description:
 *  -----------
 *  This function is used to nudge a client window do that it is totally
 *  onscreen if possible.  At least the top left corner will be onscreen.
 *
 *
 *  Inputs:
 *  ------
 *  pCD		- pointer to client data
 *  pX		- pointer to x-coord
 *  pY		- pointer to y-coord
 *  w		- width of window
 *  h		- height of window
 *
 * 
 *  Outputs:
 *  -------
 *  *pX		- new x-coord
 *  *pY		- new y-coord
 *
 *
 *  Comments:
 *  --------
 * 
 *************************************<->***********************************/

#ifdef _NO_PROTO
void 
PlaceFrameOnScreen (pCD, pX, pY, w, h)
	ClientData *pCD;
	int *pX;
	int *pY;
	int w;
	int h;

#else /* _NO_PROTO */
void 
PlaceFrameOnScreen (ClientData *pCD, int *pX, int *pY, int w, int h)
#endif /* _NO_PROTO */
{
    int clientOffsetX;
    int clientOffsetY;
    int frameX;
    int frameY;
    int frameWidth;
    int frameHeight;
    int screenHeight;
    int screenWidth;


    clientOffsetX = pCD->clientOffset.x;
    clientOffsetY = pCD->clientOffset.y;
    frameX = *pX - clientOffsetX;
    frameY = *pY - clientOffsetY;
    frameWidth = w + (2 * clientOffsetX);
    frameHeight = h + clientOffsetX + clientOffsetY;
    screenWidth = DisplayWidth (DISPLAY, SCREEN_FOR_CLIENT(pCD));
    screenHeight = DisplayHeight (DISPLAY, SCREEN_FOR_CLIENT(pCD));

    if ((frameX + frameWidth) > screenWidth)
    {
        frameX -= (frameX + frameWidth) - screenWidth;
    }
    if ((frameY + frameHeight) > screenHeight)
    {
        frameY -= (frameY + frameHeight) - screenHeight;
    }
    if (frameX < 0)
    {
        frameX = 0;
    }
    if (frameY < 0)
    {
        frameY = 0;
    }

    *pX = frameX + clientOffsetX;
    *pY = frameY + clientOffsetY;

} /* END OF FUNCTION PlaceFrameOnScreen */



/*************************************<->*************************************
 *
 *  PlaceIconOnScreen (pCD, pX, pY)
 *
 *
 *  Description:
 *  -----------
 *  This function positions an icon on-screen.
 *
 *
 *  Inputs:
 *  ------
 *  pCD		- pointer to client data
 *  pX		- pointer to x-coord
 *  pY		- pointer to y-coord
 * 
 *  Outputs:
 *  -------
 *  *pX		- new x-coord
 *  *pY		- new y-coord
 *
 *  Comments:
 *  --------
 * 
 *************************************<->***********************************/

#ifdef _NO_PROTO
void 
PlaceIconOnScreen (pCD, pX, pY)
	ClientData *pCD;
	int *pX;
	int *pY;

#else /* _NO_PROTO */
void 
PlaceIconOnScreen (ClientData *pCD, int *pX, int *pY)
#endif /* _NO_PROTO */
{
    int screenWidth;
    int screenHeight;
    int iconX;
    int iconY;


    screenWidth = DisplayWidth (DISPLAY, SCREEN_FOR_CLIENT(pCD));
    screenHeight = DisplayHeight (DISPLAY, SCREEN_FOR_CLIENT(pCD));
    iconX = *pX;
    iconY = *pY;

    if ((iconX + ICON_WIDTH(pCD)) > screenWidth)
    {
        iconX = screenWidth - ICON_WIDTH(pCD);
    }
    else if (iconX < 0)
    {
        iconX = 0;
    }

    if ((iconY + ICON_HEIGHT(pCD)) > screenHeight)
    {
        iconY = screenHeight - ICON_HEIGHT(pCD);
    }
    else if (iconY < 0)
    {
        iconY = 0;
    }

    *pX = iconX;
    *pY = iconY;


} /* END OF FUNCTION PlaceIconOnScreen */



/*************************************<->*************************************
 *
 *  FixWindowConfiguration (pCD, pWidth, pHeight, widthInc, heightInc)
 *
 *
 *  Description:
 *  -----------
 *  This function adjusts the configuration for the client window so that
 *  it is in line with the client window's sizing constraints.
 *
 *
 *  Inputs:
 *  ------
 *  pCD = a pointer to the client window data
 *  pWidth, pHeight = pointers to the window configuration values
 *  widthInc, heightInc = window size increment values
 *
 * 
 *  Outputs:
 *  -------
 *  pWidth, pHeight = adjusted configuration values are returned here
 *
 * 
 *************************************<->***********************************/

#ifdef _NO_PROTO
void 
FixWindowConfiguration (pCD, pWidth, pHeight, widthInc, heightInc)

    ClientData   *pCD;
    unsigned int *pWidth;
    unsigned int *pHeight;
    unsigned int  widthInc;
    unsigned int  heightInc;

#else /* _NO_PROTO */
void 
FixWindowConfiguration (ClientData *pCD, unsigned int *pWidth, unsigned int *pHeight, unsigned int widthInc, unsigned int heightInc)
#endif /* _NO_PROTO */
{
    register int  delta;

    /*
     * Make sure we're on width/height increment boundaries.
     */

    if ((int) *pWidth < pCD->minWidth)
    {
	*pWidth = pCD->minWidth;
    }
    else if ((delta = (*pWidth - pCD->baseWidth) % pCD->widthInc))
    {
	*pWidth -= delta;
    }

    if ((int) *pHeight < pCD->minHeight)
    {
	*pHeight = pCD->minHeight;
    }
    else if ((delta = (*pHeight - pCD->baseHeight) % pCD->heightInc))
    {
	*pHeight -= delta;
    }

    /*
     * Constrain size within bounds.
     */

    FixWindowSize (pCD, pWidth, pHeight, widthInc, heightInc);

} /* END OF FUNCTION FixWindowConfiguration */



/*************************************<->*************************************
 *
 *  FixWindowSize (pCD, pWidth, pHeight, widthInc, heightInc)
 *
 *
 *  Description:
 *  -----------
 *  This function adjusts the client window width and height so that
 *  it is in line with its sizing constraints.
 *
 *
 *  Inputs:
 *  ------
 *  pCD = a pointer to the client window data
 *  pWidth, pHeight = pointers to the window size values
 *  widthInc, heightInc = window size increment values
 *  pWS->limitResize 
 *
 * 
 *  Outputs:
 *  -------
 *  pWidth, pHeight = adjusted size values.
 *
 * 
 *************************************<->***********************************/

#ifdef _NO_PROTO
void 
FixWindowSize (pCD, pWidth, pHeight, widthInc, heightInc)

    ClientData   *pCD;
    unsigned int *pWidth;
    unsigned int *pHeight;
    unsigned int  widthInc;
    unsigned int  heightInc;

#else /* _NO_PROTO */
void 
FixWindowSize (ClientData *pCD, unsigned int *pWidth, unsigned int *pHeight, unsigned int widthInc, unsigned int heightInc)
#endif /* _NO_PROTO */
{
    register int  deltaW;
    register int  deltaH;
    WmScreenData *pSD = pCD->pSD;

    if ((int) *pWidth < pCD->minWidth)
    {
	*pWidth = pCD->minWidth;
    }
    else if (*pWidth > pCD->maxWidth &&
             pSD->limitResize && 
	     !(pCD->clientFlags & CLIENT_WM_CLIENTS))
    {
	*pWidth = pCD->maxWidth;
    }

    if ((int) *pHeight < pCD->minHeight)
    {
	*pHeight = pCD->minHeight;
    }
    else if (*pHeight > pCD->maxHeight &&
             pSD->limitResize && 
	     !(pCD->clientFlags & CLIENT_WM_CLIENTS))
    {
	*pHeight = pCD->maxHeight;
    }

    if ((pCD->sizeFlags & P_ASPECT) &&
        *pWidth * pCD->maxAspect.y > *pHeight * pCD->maxAspect.x)
    /* 
     * Client aspect is too big.
     * Candidate height >= client height:
     *   Try to increase the client's height without violating bounds.
     *   If this fails, use maximum height and try to decrease its width.
     * Candidate height < client height:
     *   Try to decrease the client's width without violating bounds.
     *   If this fails, use minimum width and try to increase its height.
     */
    {
        if ((*pHeight >= pCD->clientHeight) ||
            (*pWidth > pCD->clientWidth))
        /* 
         * Candidate height >= client height:
         *   Try to increase the client's height without violating bounds.
         *   If this fails, use maximum height and try to decrease its width.
	 */
        {
            deltaH = makemult (*pWidth * pCD->maxAspect.y / pCD->maxAspect.x -
		               *pHeight, heightInc);
	    if (*pHeight + deltaH <= pCD->maxHeight ||
                !pSD->limitResize ||
		pCD->clientFlags & CLIENT_WM_CLIENTS)
	    {
	        *pHeight += deltaH;
	    }
	    else 
	    {
	        *pHeight = pCD->maxHeight;
	        deltaW = makemult (*pWidth - *pHeight * pCD->maxAspect.x / 
			           pCD->maxAspect.y, widthInc);
	        if (*pWidth - deltaW >= pCD->minWidth)
	        {
	            *pWidth -= deltaW;
                }  
		else
		{
	            *pWidth = pCD->minWidth;
		}
	    }
	}
	else
        /*
         * Candidate height < client height and candidate width <= client width.
         *   Try to decrease the client's width without violating bounds.
         *   If this fails, use minimum width and try to increase its height.
	 */
	{
	    deltaW = makemult (*pWidth - *pHeight * pCD->maxAspect.x / 
			       pCD->maxAspect.y, widthInc);

	    if (*pWidth - deltaW >= pCD->minWidth)
	    {
	        *pWidth -= deltaW;
            }  
	    else
	    {
	        *pWidth = pCD->minWidth;
                deltaH = makemult (*pWidth * pCD->maxAspect.y / 
				   pCD->maxAspect.x - *pHeight, heightInc);
	        if (*pHeight + deltaH <= pCD->maxHeight ||
                     !pSD->limitResize ||
	             pCD->clientFlags & CLIENT_WM_CLIENTS)
	        {
	            *pHeight += deltaH;
	        }
		else
		{
	            *pHeight = pCD->maxHeight;
		}
	    }
	}
    }

    else if ((pCD->sizeFlags & P_ASPECT) &&
             *pHeight * pCD->minAspect.x > *pWidth * pCD->minAspect.y)
    /* 
     * Client aspect is too small.
     * Candidate width >= client width:
     *   Try to increase the client's width without violating bounds.
     *   If this fails, use maximum width and try to decrease its height.
     * Candidate width < client width:
     *   Try to decrease the client's height without violating bounds.
     *   If this fails, use minimum height and try to increase its width.
     */
    {
        if ((*pWidth >= pCD->clientWidth) ||
            (*pHeight > pCD->clientHeight))
        /*
         * Candidate width >= client width:
         *   Try to increase the client's width without violating bounds.
         *   If this fails, use maximum width and try to decrease its height.
	 */
	{
            deltaW = makemult (*pHeight * pCD->minAspect.x / pCD->minAspect.y -
			       *pWidth, widthInc);
	    if (*pWidth + deltaW <= pCD->maxWidth ||
                !pSD->limitResize ||
	        pCD->clientFlags & CLIENT_WM_CLIENTS)
	    {
	        *pWidth += deltaW;
	    }
	    else
	    {
	        *pWidth = pCD->maxWidth;
	        deltaH = makemult (*pHeight - *pWidth * pCD->minAspect.y / 
			           pCD->minAspect.x, heightInc); 
	        if (*pHeight - deltaH >= pCD->minHeight)
	        {
	            *pHeight -= deltaH;
                }
		else
		{
	            *pHeight = pCD->minHeight;
		}
	    }
	}
	else
        /*
         * Candidate width < client width and Candidate height <= client height:
         *   Try to decrease the client's height without violating bounds.
         *   If this fails, use minimum height and try to increase its width.
	 */
	{
	    deltaH = makemult (*pHeight - *pWidth * pCD->minAspect.y / 
			       pCD->minAspect.x, heightInc); 
	    if (*pHeight - deltaH >= pCD->minHeight)
	    {
	        *pHeight -= deltaH;
            }
	    else
	    {
	        *pHeight = pCD->minHeight;
                deltaW = makemult (*pHeight * pCD->minAspect.x / 
				   pCD->minAspect.y - *pWidth, widthInc);
	        if (*pWidth + deltaW <= pCD->maxWidth ||
                     !pSD->limitResize ||
	             pCD->clientFlags & CLIENT_WM_CLIENTS)
	        {
	            *pWidth += deltaW;
	        }
		else
		{
	            *pWidth = pCD->maxWidth;
		}
	    }
	}
    }

} /* END OF FUNCTION FixWindowSize */



/*************************************<->*************************************
 *
 *  FindClientPlacement (pCD)
 *
 *
 *  Description:
 *  -----------
 *  This function finds a position for the client window on the screen.
 *  Windows positions are stepped down the screen.  An attempt is made
 *  to key windows from being clipped by the edge of the screen.
 *
 *
 *  Inputs:
 *  ------
 *  pCD = pointer to client data (client window configuration fields)
 *
 * 
 *  Outputs:
 *  -------
 *  pCD = changes to the client window configuration fields
 * 
 *************************************<->***********************************/

#ifdef _NO_PROTO
void 
FindClientPlacement (pCD)
	ClientData *pCD;

#else /* _NO_PROTO */
void 
FindClientPlacement (ClientData *pCD)
#endif /* _NO_PROTO */
{
    static clientPlacementInitialized = False;
    static int clientPlacementOffset;
    static int clientPlacementX;
    static int clientPlacementY;
    static int clientPlacementOrigin;
    static int clientPlacementXOrigin;

    Boolean placed = False;
    int frameWidth;
    int frameHeight;
    int screenWidth;
    int screenHeight;
    Boolean offScreenX;
    Boolean offScreenY;


    if (!clientPlacementInitialized)
    {
	clientPlacementOffset =
		TitleBarHeight(pCD) +
		((RESIZE_BORDER_WIDTH(pCD) > FRAME_BORDER_WIDTH(pCD)) ?
		    RESIZE_BORDER_WIDTH(pCD) : FRAME_BORDER_WIDTH(pCD));
	clientPlacementOrigin = 2 * clientPlacementOffset;
	clientPlacementX = clientPlacementOrigin;
	clientPlacementY = clientPlacementOrigin;
	clientPlacementXOrigin = clientPlacementX;
	clientPlacementInitialized = True;
    }

    frameWidth = pCD->clientWidth + (2 * pCD->clientOffset.x);
    frameHeight = pCD->clientHeight + pCD->clientOffset.y + pCD->clientOffset.x;
    screenWidth = DisplayWidth (DISPLAY, SCREEN_FOR_CLIENT(pCD));
    screenHeight = DisplayHeight (DISPLAY, SCREEN_FOR_CLIENT(pCD));

    while (!placed)
    {
	if ((clientPlacementX - pCD->clientOffset.x + frameWidth)
	    > screenWidth)
	{
	    offScreenX = True;
	}
	else
	{
	    offScreenX = False;
	}
	if ((clientPlacementY - pCD->clientOffset.y + frameHeight)
	    > screenHeight)
	{
	    offScreenY = True;
	}
	else
	{
	    offScreenY = False;
	}

	if (offScreenX || offScreenY)
	{
	    if (clientPlacementX == clientPlacementOrigin)
	    {
		/*
		 * Placement location is already as far to the NW as it is
		 * going to go.
		 */

		placed = True;
	    }
	    else if (clientPlacementY == clientPlacementOrigin)
	    {
		/*
		 * Placement location is as far to the N as it is going to go.
		 * Use the current placement if the window is not off the
		 * screen in the x coordinate otherwise reset the placement
		 * back to the NW origin.
		 */

		if (offScreenX)
		{
		    clientPlacementXOrigin = clientPlacementOrigin;
		    clientPlacementX = clientPlacementXOrigin;
		}
		placed = True;
	    }
	    else
	    {
		/*
		 * Reset the placement location back to the NW of the
		 * current location.  Go as far N as possible and step the
		 * x coordinate to the E.
		 */

		clientPlacementXOrigin += clientPlacementOffset;
		clientPlacementX = clientPlacementXOrigin;
		clientPlacementY = clientPlacementOrigin;
	    }
	}
	else
	{
	    placed = True;
	}
    }

    /*
     * The window has been placed, now update the placement information.
     */

    pCD->clientX = clientPlacementX;
    pCD->clientY = clientPlacementY;
    clientPlacementX += clientPlacementOffset;
    if (clientPlacementX >= screenWidth)
    {
	clientPlacementXOrigin = clientPlacementOrigin;
	clientPlacementX = clientPlacementXOrigin;
    }
    clientPlacementY += clientPlacementOffset;
    if (clientPlacementY >= screenHeight)
    {
	clientPlacementY = clientPlacementOrigin;
    }


} /* END OF FUNCTION FindClientPlacement */



/*************************************<->*************************************
 *
 *  WmGetWindowAttributes (window)
 *
 *
 *  Description:
 *  -----------
 *  This function gets window attributes if necessary and saves them in the
 *  global window attribute cache.  If the window attributes are already
 *  there then no X call is made.
 *
 *
 *  Inputs:
 *  ------
 *  window = get attributes for window with this id
 *
 * 
 *  Outputs:
 *  -------
 *  wmGD.attributesWindow = set to window that matches windowAttributes
 *
 *  wmGD.windowAttributes = XWindowAttributes of window
 *
 *
 *  Comments:
 *  --------
 *  The attributes in the global cache are (known) current only for a
 *  single pass through the wm event processing loop.  They (should be)
 *  regularly cleared.
 * 
 *************************************<->***********************************/

#ifdef _NO_PROTO
Boolean 
WmGetWindowAttributes (window)
	Window window;

#else /* _NO_PROTO */
Boolean 
WmGetWindowAttributes (Window window)
#endif /* _NO_PROTO */
{
    if (wmGD.attributesWindow != window)
    {
	if (!XGetWindowAttributes (DISPLAY, window, &wmGD.windowAttributes))
	{
	    /*
	     * Cannot get window attributes.
	     */

	    wmGD.attributesWindow = NULL;
	    return (False);
	}
	wmGD.attributesWindow = window;
    }

    return (True);

} /* END OF FUNCTION WmGetWindowAttributes */



/*************************************<->*************************************
 *
 *  SetupClientIconWindow (pCD, window)
 *
 *
 *  Description:
 *  -----------
 *  This function prepares a client supplied icon window for insertion into
 *  a window manager icon frame.
 *
 *
 *  Inputs:
 *  ------
 *  pCD = pointer to client data
 *
 *  window = client supplied icon window
 *
 * 
 *  Outputs:
 *  -------
 *  pCD = (iconWindow)
 *
 *  Return = True if the icon window can be used
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
Boolean 
SetupClientIconWindow (pCD, window)
	ClientData *pCD;
	Window window;

#else /* _NO_PROTO */
Boolean 
SetupClientIconWindow (ClientData *pCD, Window window)
#endif /* _NO_PROTO */
{
    ClientData *pcd;


    /*
     * Check to see if the icon window can be used (i.e there is no conflict
     * of interest.
     */

    if (!XFindContext (DISPLAY, window, wmGD.windowContextType,
	     (caddr_t *)&pcd))
    {
	if (window == pCD->client)
	{
	    /*
	     * The proposed icon window is the same as the client!
	     */

	    return (False);
	}

	/*
	 * The proposed icon window is already being managed. 
	 * Assume that we managed it by mistake. Unmanage the 
	 * window and use it as the icon window for this client.
	 */

	 UnManageWindow (pcd);
    }

    /* update client data */
    pCD->iconWindow = window;

    /* put in window manager's save set */
    XChangeSaveSet (DISPLAY, pCD->iconWindow, SetModeInsert);
    pCD->clientFlags  |=  ICON_IN_SAVE_SET;

    return (True);

} /* END OF FUNCTION SetupClientIconWindow */



/*************************************<->*************************************
 *
 *  ProcessMwmHints (pCD)
 *
 *
 *  Description:
 *  -----------
 *  Process the _MWM_HINTS property on the window (if any).  Setup the
 *  applicable function and decoration masks.
 *
 *
 *  Inputs:
 *  ------
 *  pCD	= pointer to client data
 *
 * 
 *  Outputs:
 *  -------
 *  pCD	= may be changed.
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void 
ProcessMwmHints (pCD)
	ClientData *pCD;

#else /* _NO_PROTO */
void 
ProcessMwmHints (ClientData *pCD)
#endif /* _NO_PROTO */
{
    PropMwmHints *pHints;


    /*
     * Fix the client functions and decorations fields if they have
     * default resource values.
     */

    if (pCD->clientFunctions & WM_FUNC_DEFAULT)
    {
	if (pCD->clientFlags & CLIENT_TRANSIENT)
	{
	    pCD->clientFunctions = TRANSIENT_FUNCTIONS(pCD);
	}
	else
	{
	    pCD->clientFunctions = WM_FUNC_ALL;
	}
    }

    if (pCD->clientDecoration & WM_DECOR_DEFAULT)
    {
	if (pCD->clientFlags & CLIENT_TRANSIENT)
	{
	    pCD->clientDecoration = TRANSIENT_DECORATION(pCD);
	}
	else
	{
	    pCD->clientDecoration = WM_DECOR_ALL;
	}
    }


    /*
     * Retrieve the _MWM_HINTS property if it exists.
     */

    pCD->inputMode = MWM_INPUT_MODELESS;

    if (pHints = GetMwmHints (pCD))
    {
	if (pHints->flags & MWM_HINTS_FUNCTIONS)
	{
	    if (pHints->functions & MWM_FUNC_ALL)
	    {
		/* client indicating inapplicable functions */
		pCD->clientFunctions &= ~(pHints->functions);
	    }
	    else
	    {
		/* client indicating applicable functions */
		pCD->clientFunctions &= pHints->functions;
	    }
	    /* !!! check for some minimal level of functionality? !!! */
	}

	if (pHints->flags & MWM_HINTS_DECORATIONS)
	{
	    if (pHints->decorations & MWM_DECOR_ALL)
	    {
		/* client indicating decorations to be removed */
		pCD->clientDecoration &= ~(pHints->decorations);
	    }
	    else
	    {
		/* client indicating decorations to be added */
		pCD->clientDecoration &= pHints->decorations;
	    }

	    /*
	     * Fix up decoration configuration.
	     */

	    if (pCD->clientDecoration &
		  (MWM_DECOR_MENU | MWM_DECOR_MINIMIZE | MWM_DECOR_MAXIMIZE))
	    {
		pCD->clientDecoration |= MWM_DECOR_TITLE;
	    }
	    if (pCD->clientDecoration & MWM_DECOR_RESIZEH)
	    {
		pCD->clientDecoration |= MWM_DECOR_BORDER;
	    }
	}

	if (pHints->flags & MWM_HINTS_INPUT_MODE)
	{
	    if ((pHints->inputMode == MWM_INPUT_PRIMARY_APPLICATION_MODAL) ||
		(pHints->inputMode == MWM_INPUT_FULL_APPLICATION_MODAL) ||
		((pHints->inputMode == MWM_INPUT_SYSTEM_MODAL) &&
		 !wmGD.systemModalActive))
		
	    {
	        pCD->inputMode = pHints->inputMode;

	    }

	    /*
	     * Don't allow a system modal window to be a secondary window
	     * (except with respect to applicable functions and frame
	     * decorations). Also, don't allow system modal window to
	     * be minimized.
	     */

	    if (pCD->inputMode == MWM_INPUT_SYSTEM_MODAL)
	    {
		pCD->transientLeader = NULL;
		if (pCD->clientFunctions & MWM_FUNC_MINIMIZE)
		{
		    pCD->clientFunctions &= ~(MWM_FUNC_MINIMIZE);
		}
	    }
	}

	XFree ((char*)pHints);
    }


    /*
     * Fix up functions based on system modal settings.  System modal
     * windows and their descendents cannot be minimized.
     */

    if (!((FindTransientTreeLeader (pCD))->clientFunctions&MWM_FUNC_MINIMIZE))
    {
	pCD->clientFunctions &= ~MWM_FUNC_MINIMIZE;
    }


    /*
     * Fix up decoration configuration based on applicable functions.
     */

    if (!(pCD->clientFunctions & MWM_FUNC_RESIZE))
    {
	pCD->clientDecoration &= ~MWM_DECOR_RESIZEH;
    }

    if (!(pCD->clientFunctions & MWM_FUNC_MINIMIZE))
    {
	pCD->clientDecoration &= ~MWM_DECOR_MINIMIZE;
    }

    if (!(pCD->clientFunctions & MWM_FUNC_MAXIMIZE))
    {
	pCD->clientDecoration &= ~MWM_DECOR_MAXIMIZE;
    }
    
    pCD->decor = pCD->clientDecoration;  /* !!! combine decor ... !!! */


} /* END OF ProcessMwmHints */





