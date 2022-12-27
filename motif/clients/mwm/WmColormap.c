#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)WmColormap.c	3.8 91/01/10";
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

/*
 * include extern functions
 */

#include "WmKeyFocus.h"

/*
 * Function Declarations:
 */

#ifdef _NO_PROTO
void	InitColormapFocus ();
void	InitWorkspaceColormap ();
void	ResetColormapData ();
void	SetColormapFocus ();
void	WmInstallColormap ();
#else /* _NO_PROTO */
void InitWorkspaceColormap (WmScreenData *pSD);
void InitColormapFocus (WmScreenData *pSD);
void SetColormapFocus (WmScreenData *pSD, ClientData *pCD);
void WmInstallColormap (WmScreenData *pSD, Colormap colormap);
void ResetColormapData (ClientData *pCD, Window *pWindows, int count);
#endif /* _NO_PROTO */




/*************************************<->*************************************
 *
 *  InitWorkspaceColormap ()
 *
 *
 *  Description:
 *  -----------
 *  This function sets up the default workspace colormap and prepares for
 *  workspace colormap processing.
 *
 *
 *  Inputs:
 *  -------
 *  pSD = ptr to screen data
 * 
 *  Outputs:
 *  -------
 *  wmGD = (workspaceColormap)
 * 
 *************************************<->***********************************/

#ifdef _NO_PROTO
void InitWorkspaceColormap (pSD)
    WmScreenData *pSD;

#else /* _NO_PROTO */
void InitWorkspaceColormap (WmScreenData *pSD)
#endif /* _NO_PROTO */
{
    /*
     * Setup the default (workspace) colormap:
     * !!! this should be made more general to get the colormap for the !!!
     * !!! workspace (root) and then track colormap changes             !!!
     */

    pSD->workspaceColormap = DefaultColormap (DISPLAY, pSD->screen);

} /* END OF FUNCTION InitWorkspaceColormap */



/*************************************<->*************************************
 *
 *  InitColormapFocus (pSD)
 *
 *
 *  Description:
 *  -----------
 *  This function prepares for managing the colormap focus and sets the
 *  initial colormap focus (if the focus policy is "keyboard" - i.e. the
 *  colormap focus tracks the keyboard focus) the initial colormap
 *  installation is done in InitKeyboardFocus.
 *
 *  Inputs:
 *  -------
 *  pSD = pointer to screen data
 *
 *  Outputs:
 *  -------
 *  *pSD = (colormapFocus)
 * 
 *************************************<->***********************************/

#ifdef _NO_PROTO
void InitColormapFocus (pSD)
    WmScreenData *pSD;

#else /* _NO_PROTO */
void InitColormapFocus (WmScreenData *pSD)
#endif /* _NO_PROTO */
{
    ClientData *pCD;
    Boolean sameScreen;


    /*
     * Set up the initial colormap focus.  If the colormapFocusPolicy is
     * "keyboard" or it is "pointer" and the keyboard input focus policy
     * is "pointer" then set up the initial colormap focus when the
     * initial keyboard input focus is set up.
     */

    pSD->colormapFocus = NULL;

    if (wmGD.colormapFocusPolicy == CMAP_FOCUS_POINTER)
    {
	if (wmGD.keyboardFocusPolicy != KEYBOARD_FOCUS_POINTER)
	{
	    if (pCD = GetClientUnderPointer (&sameScreen))
	    {
	        SetColormapFocus (pSD, pCD);
	    }
	    else
	    {
	        WmInstallColormap (pSD, pSD->workspaceColormap);
	    }
	}
    }
    else
    {
	WmInstallColormap (pSD, pSD->workspaceColormap);
    }

} /* END OF FUNCTION InitColormapFocus */



/*************************************<->*************************************
 *
 *  SetColormapFocus (pSD, pCD)
 *
 *
 *  Description:
 *  -----------
 *  This function is used to set the colormap focus to a particular client
 *  window or to clear the colormap focus (set focus to the root window).
 *
 *
 *  Inputs:
 *  ------
 *  pSD = pointer to Screen Data
 *  pCD = pointer to client data (clientColormap ...)
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void SetColormapFocus (pSD, pCD)
    WmScreenData *pSD;
    ClientData *pCD;

#else /* _NO_PROTO */
void SetColormapFocus (WmScreenData *pSD, ClientData *pCD)
#endif /* _NO_PROTO */
{
    if (pCD == pSD->colormapFocus)
    {
	/*
	 * The focus is already set to the right place.
	 */

	return;
    }

    if (pCD && ((pCD->clientState == NORMAL_STATE) ||
		(pCD->clientState == MAXIMIZED_STATE)))
    {
	pSD->colormapFocus = pCD;
	WmInstallColormap (pSD, pCD->clientColormap);
    }
    else
    {
	/*
	 * The default colormap is installed for minimized windows that have
	 * the colormap focus.
	 * !!! should colormaps be installed for icons with client      !!!
	 * !!! icon windows?  should the client colormap be installed ? !!!
	 */

	pSD->colormapFocus = NULL;
	WmInstallColormap (pSD, pSD->workspaceColormap);
    }

} /* END OF FUNCTION SetColormapFocus */



/*************************************<->*************************************
 *
 *  WmInstallColormap (pSD, colormap)
 *
 *
 *  Description:
 *  -----------
 *  This function installs colormaps for the window manager.  It trys to be
 *  intelligent and avoid unnecessary installations.  It assumes that no
 *  other program is installing colormaps.
 *
 *
 *  Inputs:
 *  ------
 *  pSD = ptr to screen data
 *  colormap = the id for the colormap to be installed
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void WmInstallColormap (pSD, colormap)
    WmScreenData *pSD;
    Colormap colormap;

#else /* _NO_PROTO */
void WmInstallColormap (WmScreenData *pSD, Colormap colormap)
#endif /* _NO_PROTO */
{
    /*
     * !!! this could be generalized to work better for systems that !!!
     * !!! support multiple installed colormaps                      !!!
     */

    if (colormap != pSD->lastInstalledColormap)
    {
	XInstallColormap (DISPLAY, colormap);
	pSD->lastInstalledColormap = colormap;
    }

} /* END OF FUNCTION WmInstallColormap */



/*************************************<->*************************************
 *
 *  ResetColormapData (pCD, pWindows, count)
 *
 *
 *  Description:
 *  -----------
 *  This function is used to release old colormap data (contexts, malloc'ed
 *  space).
 *
 *
 *  Inputs:
 *  ------
 *  pCD = pointer to client data (cmapWindows ...)
 *
 *  pWindows = new list of colormap windows
 *
 *  count = number of windows in new colormap windows list
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void ResetColormapData (pCD, pWindows, count)
    ClientData *pCD;
    Window *pWindows;
    int count;

#else /* _NO_PROTO */
void ResetColormapData (ClientData *pCD, Window *pWindows, int count)
#endif /* _NO_PROTO */
{
    int i;


    if (pCD->clientCmapCount)
    {
	if (count == 0)
	{
	    /* reset the client colormap to the toplevel window colormap */
	    for (i = 0; i < pCD->clientCmapCount; i++)
	    {
		if (pCD->cmapWindows[i] == pCD->client)
		{
		    pCD->clientColormap = pCD->clientCmapList[i];
		    break;
		}
	    }
	}

	/*
	 * Free up old contexts.
	 */

	for (i = 0; i < pCD->clientCmapCount; i++)
	{
	    if (pCD->cmapWindows[i] != pCD->client)
	    {
		XDeleteContext (DISPLAY, pCD->cmapWindows[i],
		    wmGD.windowContextType);
	    }
	}

	/*
	 * Free up old colormap data.
	 */

	free ((char *)(pCD->cmapWindows));
	free ((char *)(pCD->clientCmapList));
	pCD->clientCmapCount = 0;
    }

    if (count)
    {
	/*
	 * Set new contexts.
	 */

	for (i = 0; i < count; i++)
	{
	    if (pWindows[i] != pCD->client)
	    {
		XSaveContext (DISPLAY, pWindows[i], wmGD.windowContextType,
		    (caddr_t)pCD);
	    }
	}
    }

} /* END OF FUNCTION ResetColormapData */
