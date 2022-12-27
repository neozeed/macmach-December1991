#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)WmWinConf.h	3.5 90/07/23";
#endif /* lint */
#endif /* REV_INFO */
 /*****************************************************************************
 ******************************************************************************
 *
 * (c) Copyright 1989, 1990, OPEN SOFTWARE FOUNDATION, INC.
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
 ******************************************************************************
 *****************************************************************************/


#ifdef _NO_PROTO

extern void    AdjustPos ();
extern unsigned int ButtonStateBit ();
extern void    CancelFrameConfig();
extern void	CheckEatButtonRelease ();
extern Boolean CheckVisualPlace ();
extern void    CompleteFrameConfig ();
extern Cursor  ConfigCursor();
extern void    DoFeedback();
extern Boolean DoGrabs();
extern void    DrawOutline();
extern void	EatButtonRelease ();
extern void    FixFrameValues();
extern void    FlashOutline();
extern void    ForceOnScreen();
extern void	GetClipDimensions ();
extern void    GetConfigEvent ();
extern Window  GrabWin();
extern void    HandleClientFrameMove ();
extern void    HandleClientFrameResize ();
extern Boolean HandleResizeKeyPress();
#ifdef OPAQUE
extern void    MoveOpaque ();
#endif /* OPAQUE  */
extern void    MoveOutline ();
extern void    ProcessNewConfiguration ();
extern void    ReGrabPointer();
extern void	SetFrameInfo ();
extern Boolean SetKeyboardResizePart ();
extern void	SetOutline();
extern void    SetPointerPosition ();
extern Boolean SetPointerResizePart ();
extern Boolean StartClientMove ();
extern void    StartClientResize();
extern Boolean StartResizeConfig();
extern int     ResizeType();
extern void    UndoGrabs();

#else /* _NO_PROTO */

extern void AdjustPos (int *pX, int *pY, unsigned int oWidth, 
		       unsigned int oHeight, unsigned int nWidth, 
		       unsigned int nHeight);
extern unsigned int ButtonStateBit (unsigned int button);
extern void CancelFrameConfig (ClientData *pcd);
extern void	CheckEatButtonRelease (ClientData *pcd, XEvent *pev);
extern Boolean CheckVisualPlace (ClientData *pCD, int tmpX, int tmpY);
extern void CompleteFrameConfig (ClientData *pcd, XEvent *pev);
extern Cursor ConfigCursor (int frame_part);
extern void DoFeedback (ClientData *pcd, int x, int y, unsigned int width, 
			unsigned int height, unsigned long newStyle, 
			Boolean resizing);
extern Boolean DoGrabs (Window grab_win, Cursor cursor, unsigned int pmask, 
			Time grabTime, ClientData *pCD, Boolean alwaysGrab);
extern void DrawOutline (int x, int y, unsigned int width, 
			 unsigned int height);
extern void	EatButtonRelease (unsigned int releaseButtons);
extern void FixFrameValues (ClientData *pcd, int *pfX, int *pfY, 
			    unsigned int *pfWidth, unsigned int *pfHeight, 
			    Boolean resizing);
extern void FlashOutline (int x, int y, unsigned int width, 
			  unsigned int height);
extern void ForceOnScreen (int screen, int *pX, int *pY);
extern void GetClipDimensions (ClientData *pCD, Boolean fromRoot);
extern void GetConfigEvent (Display *display, Window window, 
			    unsigned long mask, int curX, int curY, 
			    int oX, int oY, unsigned oWidth, 
			    unsigned oHeight, XEvent *pev);
extern Window GrabWin (ClientData *pcd, XEvent *pev);
extern void HandleClientFrameMove (ClientData *pcd, XEvent *pev);
extern void HandleClientFrameResize (ClientData *pcd, XEvent *pev);
extern Boolean HandleResizeKeyPress (ClientData *pcd, XEvent *pev);
#ifdef OPAQUE
extern void    MoveOpaque (ClientData *pcd, int x, int y,
		 unsigned int width, unsigned int height);
#endif /* OPAQUE  */
extern void MoveOutline (int x, int y, unsigned int width, 
			 unsigned int height);
extern void ProcessNewConfiguration (ClientData *pCD, int x, int y, 
				     unsigned int width, unsigned int height, 
				     Boolean clientRequest);
extern void ReGrabPointer (Window grab_win, Time grabTime);
extern void SetOutline (XSegment *pOutline, int x, int y, unsigned int width, 
			unsigned int height, int fatness);
extern void SetPointerPosition (int newX, int newY, int *actualX, 
				int *actualY);
extern Boolean SetPointerResizePart (ClientData *pcd, XEvent *pev);
extern Boolean StartClientMove (ClientData *pcd, XEvent *pev);
extern void StartClientResize (ClientData *pcd, XEvent *pev);
extern Boolean StartResizeConfig (ClientData *pcd, XEvent *pev);
extern int ResizeType (ClientData *pcd, XEvent *pev);
extern void UndoGrabs (void);

#endif /* _NO_PROTO */
