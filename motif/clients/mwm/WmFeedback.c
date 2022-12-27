#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)WmFeedback.c	3.15 91/01/10";
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
#include "WmResNames.h"
#include <Xm/Xm.h>
#include <X11/Shell.h>
#include <Xm/Label.h>
#include <Xm/DialogS.h>
#include <Xm/BulletinB.h>
#include <Xm/MessageB.h>

#define MOVE_OUTLINE_WIDTH	2
#define FEEDBACK_BEVEL		2

#define DEFAULT_POSITION_STRING	"(0000x0000)"

#define  CB_HIGHLIGHT_THICKNESS  3

/*
 * include extern functions
 */
#include "WmFeedback.h"
#include "WmFunction.h"
#include "WmGraphics.h"
#include "WmManage.h"



/*
 * Global Variables:
 */
static Cursor  waitCursor = NULL;

/* see WmGlobal.h for index defines: */

static char *confirm_mesg[3] = {"Toggle Behavior?",
                                "Restart Mwm?",
                                "QUIT Mwm?"};


#ifdef _NO_PROTO
typedef void (*ConfirmFunc)();
#else
typedef void (*ConfirmFunc)(Boolean);
#endif
static ConfirmFunc confirm_func[3] = {Do_Set_Behavior,
				      Do_Restart,
				      Do_Quit_Mwm};


/*************************************<->*************************************
 *
 *  ShowFeedbackWindow(pSD, x, y, width, height, style)
 *
 *
 *  Description:
 *  -----------
 *  Pop up the window for moving and sizing feedback
 *
 *
 *  Inputs:
 *  ------
 *  pSD		- pointer to screen data
 *  x		- initial x-value
 *  y		- initial y-value
 *  width 	- initial width value
 *  height	- initial height value
 *  style	- show size, position, or both
 *  
 * 
 *  Outputs:
 *  -------
 *
 *
 *  Comments:
 *  --------
 *************************************<->***********************************/
#ifdef _NO_PROTO
void ShowFeedbackWindow (pSD, x, y, width, height, style)
    
    WmScreenData *pSD;
    int           x, y; 
    unsigned int  width, height;
    unsigned long style;
#else /* _NO_PROTO */
void ShowFeedbackWindow (WmScreenData *pSD, int x, int y, unsigned int width, unsigned int height, unsigned long style)
#endif /* _NO_PROTO */
{
    unsigned long        mask;
    XSetWindowAttributes win_attribs;
    XWindowChanges       win_changes;
    int                  direction, ascent, descent;
    XCharStruct          xcsLocation;
    int                  winX, winY;

    if ( (pSD->fbStyle = style) == FB_OFF)
	return;

    pSD->fbLastX = x;
    pSD->fbLastY = y;
    pSD->fbLastWidth = width;
    pSD->fbLastHeight = height;

    /*
     * Derive the size and position of the window from the text extents
     * Set starting position of each string 
     */
    XTextExtents(pSD->feedbackAppearance.font, DEFAULT_POSITION_STRING, 
		 strlen(DEFAULT_POSITION_STRING), &direction, &ascent, 
		 &descent, &xcsLocation);
    
    pSD->fbWinWidth = xcsLocation.width + 4*FEEDBACK_BEVEL;

    switch (pSD->fbStyle) 
    {
	case FB_SIZE:
	    pSD->fbSizeY = 2*FEEDBACK_BEVEL + ascent;
	    pSD->fbWinHeight = (ascent + descent) + 4*FEEDBACK_BEVEL;
	    break;

	case FB_POSITION:
	    pSD->fbLocY = 2*FEEDBACK_BEVEL + ascent;
	    pSD->fbWinHeight = (ascent + descent) + 4*FEEDBACK_BEVEL;
	    break;

	default:
	case (FB_SIZE | FB_POSITION):
	    pSD->fbLocY = 2*FEEDBACK_BEVEL + ascent;
	    pSD->fbSizeY = pSD->fbLocY + ascent + descent;
	    pSD->fbWinHeight = 2*(ascent + descent) + 4*FEEDBACK_BEVEL;
	    break;
    }

    winX = (DisplayWidth(DISPLAY, pSD->screen) - pSD->fbWinWidth)/2;
    winY = (DisplayHeight(DISPLAY, pSD->screen) -pSD->fbWinHeight)/2;

    /* 
     * Put new text into the feedback strings
     */
    UpdateFeedbackText (pSD, x, y, width, height);

    /*
     * bevel the window border for a 3-D look
     */
    if ( (pSD->fbTop && pSD->fbBottom) ||
	 ((pSD->fbTop = AllocateRList((unsigned)2*FEEDBACK_BEVEL)) &&
	  (pSD->fbBottom = AllocateRList((unsigned)2*FEEDBACK_BEVEL))) )
    {
	pSD->fbTop->used = 0;
	pSD->fbBottom->used = 0;
	BevelRectangle (pSD->fbTop,
			pSD->fbBottom,
			0, 0, 
			pSD->fbWinWidth, pSD->fbWinHeight,
			FEEDBACK_BEVEL, FEEDBACK_BEVEL,
			FEEDBACK_BEVEL, FEEDBACK_BEVEL);
    }

    /*
     * Create window if not yet created, otherwise fix size and position
     */

    if (!pSD->feedbackWin)
    {

	/*
	 * Create the window
	 */

	mask = CWEventMask | CWOverrideRedirect | CWSaveUnder;
	win_attribs.event_mask = ExposureMask;
	win_attribs.override_redirect = TRUE;
	win_attribs.save_under = TRUE;

	/* 
	 * Use background pixmap if one is specified, otherwise set the
	 * appropriate background color. 
	 */

	if (pSD->feedbackAppearance.backgroundPixmap)
	{
	    mask |= CWBackPixmap;
	    win_attribs.background_pixmap =
				pSD->feedbackAppearance.backgroundPixmap;
	}
	else
	{
	    mask |= CWBackPixel;
	    win_attribs.background_pixel =
				pSD->feedbackAppearance.background;
	}

	pSD->feedbackWin = XCreateWindow (DISPLAY, pSD->rootWindow, 
					  winX, winY,
					  pSD->fbWinWidth, 
					  pSD->fbWinHeight,
					  0, CopyFromParent, 
					  InputOutput, CopyFromParent, 
					  mask, &win_attribs);
    }
    else
    {
	win_changes.x = winX;
	win_changes.y = winY;
	win_changes.width = pSD->fbWinWidth;
	win_changes.height = pSD->fbWinHeight;
	win_changes.stack_mode = TopIf;

	mask = CWX | CWY | CWWidth | CWHeight | CWStackMode;

	XConfigureWindow(DISPLAY, pSD->feedbackWin, (unsigned int) mask, 
	    &win_changes);
    }


    /*
     * Make the feedback window visible (map it)
     */

    if (pSD->feedbackWin)
    {
#ifdef OPAQUE
	if(pSD->moveOpaque)
	{
	    XRaiseWindow(DISPLAY, pSD->feedbackWin);
	}
#endif /* OPAQUE */
	XMapWindow (DISPLAY, pSD->feedbackWin);
	PaintFeedbackWindow(pSD);
    }

} /* END OF FUNCTION ShowFeedbackWindow */



/*************************************<->*************************************
 *
 *  PaintFeedbackWindow(pSD)
 *
 *
 *  Description:
 *  -----------
 *  Repaints the feedback window in response to exposure events
 *
 *
 *  Inputs:
 *  ------
 *  pSD		- pointer to screen data
 * 
 *  Outputs:
 *  -------
 *
 *
 *  Comments:
 *  --------
 *************************************<->***********************************/
#ifdef _NO_PROTO
void PaintFeedbackWindow (pSD)

    WmScreenData *pSD;
#else /* _NO_PROTO */
void PaintFeedbackWindow (WmScreenData *pSD)
#endif /* _NO_PROTO */
{
    if (pSD->feedbackWin)
    {
	/* 
	 * draw beveling 
	 */
	if (pSD->fbTop->used > 0) 
	{
	    XFillRectangles (DISPLAY, pSD->feedbackWin, 
			     pSD->feedbackAppearance.inactiveTopShadowGC,
			     pSD->fbTop->prect, pSD->fbTop->used);
	}
	if (pSD->fbBottom->used > 0) 
	{
	    XFillRectangles (DISPLAY, pSD->feedbackWin, 
			     pSD->feedbackAppearance.inactiveBottomShadowGC,
			     pSD->fbBottom->prect, 
			     pSD->fbBottom->used);
	}

	/*
	 * clear old text 
	 */
	XClearArea (DISPLAY, pSD->feedbackWin, 
		    FEEDBACK_BEVEL, FEEDBACK_BEVEL,
		    pSD->fbWinWidth-2*FEEDBACK_BEVEL, 
		    pSD->fbWinHeight-2*FEEDBACK_BEVEL,
		    FALSE);

	/*
	 * put up new text
	 */
	if (pSD->fbStyle & FB_POSITION) 
	{
	    WmDrawString (DISPLAY, pSD->feedbackWin, 
			 pSD->feedbackAppearance.inactiveGC,
			 pSD->fbLocX, pSD->fbLocY, 
			 pSD->fbLocation, strlen(pSD->fbLocation));
	}
	if (pSD->fbStyle & FB_SIZE) 
	{
	    WmDrawString (DISPLAY, pSD->feedbackWin, 
			 pSD->feedbackAppearance.inactiveGC,
			 pSD->fbSizeX, pSD->fbSizeY, 
			 pSD->fbSize, strlen(pSD->fbSize));
	}
    }
}



/*************************************<->*************************************
 *
 *  HideFeedbackWindow (pSD)
 *
 *
 *  Description:
 *  -----------
 *  Hide the feedback window
 *
 *
 *  Inputs:
 *  ------
 *  pDS		- pointer to screen data
 * 
 *  Outputs:
 *  -------
 *
 *
 *  Comments:
 *  --------
 * 
 *************************************<->***********************************/
#ifdef _NO_PROTO
void HideFeedbackWindow (pSD)

    WmScreenData *pSD;
#else /* _NO_PROTO */
void HideFeedbackWindow (WmScreenData *pSD)
#endif /* _NO_PROTO */
{
    if (pSD->feedbackWin)
    {
	XUnmapWindow (DISPLAY, pSD->feedbackWin);
    }
    pSD->fbStyle = FB_OFF;
}




/*************************************<->*************************************
 *
 *  UpdateFeedbackInfo (pSD, x, y, width, height)
 *
 *
 *  Description:
 *  -----------
 *  Update the information in the feedback window
 *
 *
 *  Inputs:
 *  ------
 *  pSD		- pointer to screen info
 *  x		- x-value
 *  y		- y-value
 *  width 	- width value
 *  height	- height value
 *
 * 
 *  Outputs:
 *  -------
 *
 *
 *  Comments:
 *  --------
 * 
 *************************************<->***********************************/
#ifdef _NO_PROTO
void UpdateFeedbackInfo (pSD, x, y, width, height)

    WmScreenData *pSD;
    int          x, y; 
    unsigned int width, height;
#else /* _NO_PROTO */
void UpdateFeedbackInfo (WmScreenData *pSD, int x, int y, unsigned int width, unsigned int height)
#endif /* _NO_PROTO */
{
    /*
     * Currently the feedback window must always be redrawn to (potentially)
     * repair damage done by moving the configuration outline.  The feedback
     * repainting generally only needs to be done when the information
     * changes or the feedback window is actually overwritten by the
     * configuration outline.
     */

    {
	pSD->fbLastX = x;
	pSD->fbLastY = y;
	pSD->fbLastWidth = width;
	pSD->fbLastHeight = height;

	UpdateFeedbackText (pSD, x, y, width, height);

	PaintFeedbackWindow(pSD);
    }
}




/*************************************<->*************************************
 *
 *  UpdateFeedbackText (pSD, x, y, width, height)
 *
 *
 *  Description:
 *  -----------
 *  Update the information in the feedback strings
 *
 *
 *  Inputs:
 *  ------
 *  pSD		- pointer to screen data
 *  x		- x-value
 *  y		- y-value
 *  width 	- width value
 *  height	- height value
 *
 * 
 *  Outputs:
 *  -------
 *
 *
 *  Comments:
 *  --------
 * 
 *************************************<->***********************************/
#ifdef _NO_PROTO
void UpdateFeedbackText (pSD, x, y, width, height)

    WmScreenData *pSD;
    int          x, y; 
    unsigned int width, height;
#else /* _NO_PROTO */
void UpdateFeedbackText (WmScreenData *pSD, int x, int y, unsigned int width, unsigned int height)
#endif /* _NO_PROTO */
{
    int         direction, ascent, descent;
    XCharStruct xcs;

    if (pSD->fbStyle & FB_POSITION) 
    {
	sprintf (pSD->fbLocation, "(%4d,%-4d)", x, y);
	XTextExtents(pSD->feedbackAppearance.font, pSD->fbLocation,
		 strlen(pSD->fbLocation), &direction, &ascent, 
		 &descent, &xcs);
	pSD->fbLocX = (pSD->fbWinWidth - xcs.width)/2;
    }

    if (pSD->fbStyle & FB_SIZE) 
    {
	sprintf (pSD->fbSize,     "%4dx%-4d", width, height);
	XTextExtents(pSD->feedbackAppearance.font, pSD->fbSize,
		 strlen(pSD->fbSize), &direction, &ascent, 
		 &descent, &xcs);
	pSD->fbSizeX = (pSD->fbWinWidth - xcs.width)/2;
    }
}



/*************************************<->*************************************
 *
 *  static void
 *  OkCB (w, client_data, call_data)
 *
 *
 *  Description:
 *  -----------
 *  QuestionBox Ok callback.
 *
 *
 *  Inputs:
 *  ------
 *  None.
 *
 * 
 *  Outputs:
 *  -------
 *  None.
 *
 *
 *  Comments:
 *  --------
 *  None.
 * 
 *************************************<->***********************************/

static void OkCB (w, client_data, call_data)

   Widget w;
   caddr_t client_data;
   caddr_t call_data;
{
    WithdrawDialog (w);

    confirm_func[((WmScreenData *)client_data)->actionNbr] (False);

} /* END OF FUNCTION OkCB */


/*************************************<->*************************************
 *
 *  static void
 *  CancelCB (w, client_data, call_data)
 *
 *
 *  Description:
 *  -----------
 *  QuestionBox Cancel callback.
 *
 *
 *  Inputs:
 *  ------
 *  None.
 *
 * 
 *  Outputs:
 *  -------
 *  None.
 *
 *
 *  Comments:
 *  --------
 *  None.
 * 
 *************************************<->***********************************/

static void CancelCB (w, client_data, call_data)

   Widget w;
   caddr_t client_data;
   caddr_t call_data;
{
    WithdrawDialog (w);

} /* END OF FUNCTION CancelCB */



/*************************************<->*************************************
 *
 *  void
 *  ConfirmAction (pSD,nbr)
 *
 *
 *  Description:
 *  -----------
 *  Post a QuestionBox and ask for confirmation.  If so, executes the
 *  appropriate action.
 *
 *
 *  Inputs:
 *  ------
 *  nbr = action number
 *  pSD->screen
 *  pSD->screenTopLevel
 *
 * 
 *  Outputs:
 *  -------
 *  actionNbr = current QuestionBox widget index.
 *  confirmW[actionNbr]  = QuestionBox widget.
 *
 *
 *  Comments:
 *  --------
 * 
 *************************************<->***********************************/

#ifdef _NO_PROTO
void ConfirmAction (pSD,nbr)

    WmScreenData *pSD;
    int	     nbr;
#else /* _NO_PROTO */
void ConfirmAction (WmScreenData *pSD, int nbr)
#endif /* _NO_PROTO */
{
    Arg           args[8];
    register int  n;
    int           x, y;
    Dimension     width, height;
    Widget        dialogShellW = NULL;

    if (pSD->confirmboxW[nbr] == NULL)
    /* First time for this one */
    {

        /* 
         * Create a dialog popup shell with explicit keyboard policy.
         */

        n = 0;
        XtSetArg(args[n], XmNx, (XtArgVal)
	         DisplayWidth (DISPLAY, pSD->screen)/2); n++;
        XtSetArg(args[n], XmNy, (XtArgVal)
	         DisplayHeight (DISPLAY, pSD->screen)/2); n++;
        XtSetArg(args[n], XtNallowShellResize, (XtArgVal) TRUE);  n++;
        XtSetArg(args[n], XtNkeyboardFocusPolicy, (XtArgVal) XmEXPLICIT);  n++;
        XtSetArg(args[n], XtNdepth, 
		(XtArgVal) DefaultDepth(DISPLAY, pSD->screen));  n++;
        XtSetArg(args[n], XtNscreen, 
		(XtArgVal) ScreenOfDisplay(DISPLAY, pSD->screen));  n++;

        dialogShellW =
    	        XtCreatePopupShell ((String) WmNfeedback, 
				    transientShellWidgetClass,
		                    pSD->screenTopLevelW, args, n);

        /* 
         * Create a QuestionBox as a child of the popup shell.
	 * Set traversalOn and add callbacks for the OK and CANCEL buttons.
	 * Unmanage the HELP button.
         */

        n = 0;
        XtSetArg(args[n], XmNdialogType, (XtArgVal) XmDIALOG_QUESTION); n++;
        XtSetArg(args[n], XmNmessageString, (XtArgVal)
           XmStringCreate(confirm_mesg[nbr],
			  XmSTRING_DEFAULT_CHARSET)); n++;
        XtSetArg(args[n], XmNmessageAlignment, (XtArgVal) XmALIGNMENT_CENTER);
	   n++;
        XtSetArg(args[n], XmNtraversalOn, (XtArgVal) TRUE); n++;
        pSD->confirmboxW[nbr] = 
	    XtCreateManagedWidget (WmNconfirmbox, xmMessageBoxWidgetClass,
                                   dialogShellW, args, n);

        n = 0;
        XtSetArg (args[n], XmNtraversalOn, (XtArgVal) TRUE); n++;
        XtSetArg (args[n], XmNhighlightThickness, 
		  (XtArgVal) CB_HIGHLIGHT_THICKNESS); n++;
        XtSetValues ( XmMessageBoxGetChild (pSD->confirmboxW[nbr], 
			    XmDIALOG_OK_BUTTON), args, n);
        XtSetValues ( XmMessageBoxGetChild (pSD->confirmboxW[nbr], 
			    XmDIALOG_CANCEL_BUTTON), args, n);
        XtAddCallback (pSD->confirmboxW[nbr], XmNokCallback, 
		       (XtCallbackProc) OkCB, 
			    (caddr_t)pSD); 
        XtAddCallback (pSD->confirmboxW[nbr], XmNcancelCallback, 
	    (XtCallbackProc)CancelCB, (caddr_t)NULL); 

        XtUnmanageChild
	    (XmMessageBoxGetChild (pSD->confirmboxW[nbr], 
		XmDIALOG_HELP_BUTTON));

        XtRealizeWidget (dialogShellW);

        /* 
         * Center the DialogShell in the display.
         */

        n = 0;
        XtSetArg(args[n], XmNheight, &height); n++;
        XtSetArg(args[n], XmNwidth, &width); n++;
        XtGetValues (dialogShellW, (ArgList) args, n);

        x = (DisplayWidth (DISPLAY, pSD->screen) - ((int) width))/2;
        y = (DisplayHeight (DISPLAY, pSD->screen) - ((int) height))/2;
        n = 0;
        XtSetArg(args[n], XmNx, (XtArgVal) x); n++;
        XtSetArg(args[n], XmNy, (XtArgVal) y); n++;
        XtSetValues (dialogShellW, (ArgList) args, n);

        ManageWindow (pSD, XtWindow(dialogShellW), MANAGEW_CONFIRM_BOX);
    }
    else
    {
        ReManageDialog (pSD, pSD->confirmboxW[nbr]);
    }

    pSD->actionNbr = nbr;

    XFlush(DISPLAY);

} /* END OF FUNCTION ConfirmAction */



/*************************************<->*************************************
 *
 *  ShowWaitState (flag)
 *
 *
 *  Description:
 *  -----------
 *  Enter/Leave the wait state.
 *
 *
 *  Inputs:
 *  ------
 *  flag = TRUE for Enter, FALSE for Leave.
 *
 * 
 *  Outputs:
 *  -------
 *  None.
 *
 *
 *  Comments:
 *  --------
 *  None.
 * 
 *************************************<->***********************************/

#ifdef LARGECURSORS

#define time32_width 32
#define time32_height 32
#define time32_x_hot 15
#define time32_y_hot 15
static char time32_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0xfe, 0xff, 0xff, 0x7f, 0xfe, 0xff, 0xff, 0x7f,
   0x8c, 0x00, 0x00, 0x31, 0x4c, 0x00, 0x00, 0x32, 0x4c, 0x00, 0x00, 0x32,
   0x4c, 0x00, 0x00, 0x32, 0x4c, 0x00, 0x00, 0x32, 0x4c, 0x00, 0x00, 0x32,
   0x8c, 0x00, 0x00, 0x31, 0x0c, 0x7f, 0xfe, 0x30, 0x0c, 0xfe, 0x7f, 0x30,
   0x0c, 0xfc, 0x3f, 0x30, 0x0c, 0xf8, 0x1f, 0x30, 0x0c, 0xe0, 0x07, 0x30,
   0x0c, 0x80, 0x01, 0x30, 0x0c, 0x80, 0x01, 0x30, 0x0c, 0x60, 0x06, 0x30,
   0x0c, 0x18, 0x18, 0x30, 0x0c, 0x04, 0x20, 0x30, 0x0c, 0x02, 0x40, 0x30,
   0x0c, 0x01, 0x80, 0x30, 0x8c, 0x00, 0x00, 0x31, 0x4c, 0x80, 0x01, 0x32,
   0x4c, 0xc0, 0x03, 0x32, 0x4c, 0xf0, 0x1f, 0x32, 0x4c, 0xff, 0xff, 0x32,
   0xcc, 0xff, 0xff, 0x33, 0x8c, 0xff, 0xff, 0x31, 0xfe, 0xff, 0xff, 0x7f,
   0xfe, 0xff, 0xff, 0x7f, 0x00, 0x00, 0x00, 0x00};

#define time32m_width 32
#define time32m_height 32
static char time32m_bits[] = {
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xcf, 0x00, 0x00, 0xf3, 0x6e, 0x00, 0x00, 0x76, 0x6e, 0x00, 0x00, 0x76,
   0x6e, 0x00, 0x00, 0x76, 0x6e, 0x00, 0x00, 0x76, 0x6e, 0x00, 0x00, 0x76,
   0xce, 0x00, 0x00, 0x73, 0x8e, 0x7f, 0xfe, 0x71, 0x0e, 0xff, 0xff, 0x70,
   0x0e, 0xfe, 0x7f, 0x70, 0x0e, 0xfc, 0x3f, 0x70, 0x0e, 0xf8, 0x1f, 0x70,
   0x0e, 0xe0, 0x07, 0x70, 0x0e, 0xe0, 0x07, 0x70, 0x0e, 0x78, 0x1e, 0x70,
   0x0e, 0x1c, 0x38, 0x70, 0x0e, 0x06, 0x60, 0x70, 0x0e, 0x03, 0xc0, 0x70,
   0x8e, 0x01, 0x80, 0x71, 0xce, 0x00, 0x00, 0x73, 0x6e, 0x80, 0x01, 0x76,
   0x6e, 0xc0, 0x03, 0x76, 0x6e, 0xf0, 0x1f, 0x76, 0x6e, 0xff, 0xff, 0x76,
   0xee, 0xff, 0xff, 0x77, 0xcf, 0xff, 0xff, 0xf3, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

#endif /* LARGECURSORS */


#define time16_x_hot 7
#define time16_y_hot 7
#define time16_width 16
#define time16_height 16
static char time16_bits[] = {
   0x00, 0x00, 0xfe, 0x7f, 0x14, 0x28, 0x14, 0x28, 0x14, 0x28, 0x24, 0x24,
   0x44, 0x22, 0x84, 0x21, 0x84, 0x21, 0x44, 0x22, 0x24, 0x24, 0x14, 0x28,
   0x94, 0x29, 0xd4, 0x2b, 0xfe, 0x7f, 0x00, 0x00};

#define time16m_width 16
#define time16m_height 16
static char time16m_bits[] = {
   0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xfe, 0x7f, 0xfe, 0x7f, 0xfe, 0x7f,
   0xfe, 0x7f, 0xfe, 0x7f, 0xfe, 0x7f, 0xfe, 0x7f, 0xfe, 0x7f, 0xfe, 0x7f,
   0xfe, 0x7f, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff};


#ifdef _NO_PROTO
void ShowWaitState (flag)

    Boolean  flag;
#else /* _NO_PROTO */
void ShowWaitState (Boolean flag)
#endif /* _NO_PROTO */
{
    char        *bits;
    char        *maskBits;
    unsigned int width;
    unsigned int height;
    unsigned int xHotspot;
    unsigned int yHotspot;
    Pixmap       pixmap;
    Pixmap       maskPixmap;
    XColor       xcolors[2];
    int          scr;


    if (!waitCursor)
    {
#ifdef LARGECURSORS
	if (wmGD.useLargeCursors)
	{
	    width = time32_width;
	    height = time32_height;
	    bits = time32_bits;
	    maskBits = time32m_bits;
	    xHotspot = time32_x_hot;
	    yHotspot = time32_y_hot;
	}
	else
#endif /* LARGECURSORS */

	{
	    width = time16_width;
	    height = time16_height;
	    bits = time16_bits;
	    maskBits = time16m_bits;
	    xHotspot = time16_x_hot;
	    yHotspot = time16_y_hot;
	}

        pixmap = XCreateBitmapFromData (DISPLAY, 
		         DefaultRootWindow(DISPLAY), bits, 
			 width, height);

        maskPixmap = XCreateBitmapFromData (DISPLAY, 
		         DefaultRootWindow(DISPLAY), maskBits, 
			 width, height);

        xcolors[0].pixel = BlackPixelOfScreen(DefaultScreenOfDisplay(DISPLAY));
        xcolors[1].pixel = WhitePixelOfScreen(DefaultScreenOfDisplay(DISPLAY));

        XQueryColors (DISPLAY, 
		      DefaultColormapOfScreen(DefaultScreenOfDisplay
					      (DISPLAY)), 
		      xcolors, 2);
	waitCursor = XCreatePixmapCursor (DISPLAY, pixmap, maskPixmap,
	                                  &(xcolors[0]), &(xcolors[1]),
                                          xHotspot, yHotspot);
        XFreePixmap (DISPLAY, pixmap);
        XFreePixmap (DISPLAY, maskPixmap);
    }

    if (flag)
    {
	XGrabPointer (DISPLAY, DefaultRootWindow(DISPLAY), FALSE, 
			0, GrabModeAsync, GrabModeAsync, None, 
			waitCursor, CurrentTime);
	XGrabKeyboard (DISPLAY, DefaultRootWindow(DISPLAY), FALSE, 
			GrabModeAsync, GrabModeAsync, CurrentTime);
    }
    else
    {
	XUngrabPointer (DISPLAY, CurrentTime);
	XUngrabKeyboard (DISPLAY, CurrentTime);
    }

} /* END OF FUNCTION ShowWaitState */



/*************************************<->*************************************
 *
 *  InitCursorInfo ()
 *
 *
 *  Description:
 *  -----------
 *  This function determines whether a server supports large cursors.  It it
 *  does large feedback cursors are used in some cases (wait state and
 *  system modal state); otherwise smaller (16x16) standard cursors are used.
 *
 *  Outputs:
 *  -------
 *  wmGD.useLargeCusors = set to True if larger cursors are supported.
 * 
 *************************************<->***********************************/

#ifdef _NO_PROTO
void InitCursorInfo ()

#else /* _NO_PROTO */
void InitCursorInfo (void)
#endif /* _NO_PROTO */
{
    unsigned int cWidth;
    unsigned int cHeight;

    wmGD.useLargeCursors = False;

    if (XQueryBestCursor (DISPLAY, DefaultRootWindow(DISPLAY), 
	32, 32, &cWidth, &cHeight))
    {
	if ((cWidth >= 32) && (cHeight >= 32))
	{
	    wmGD.useLargeCursors = True;
	}
    }

} /* END OF FUNCTION InitCursorInfo */






