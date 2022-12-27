#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)WmEvent.h	3.5 90/07/23";
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
extern Boolean		CheckForButtonAction ();
extern Time		GetTimestamp ();
extern Boolean		HandleKeyPress ();
extern void		HandleWsButtonPress ();
extern void		HandleWsButtonRelease ();
extern void		HandleWsConfigureRequest ();
extern void		HandleWsEnterNotify ();
extern void		HandleWsFocusIn ();
extern Boolean		HandleWsKeyPress ();
extern void		HandleWsLeaveNotify ();
extern void		IdentifyEventContext ();
extern void		InitEventHandling ();
extern void		ProcessClickBPress ();
extern void		ProcessClickBRelease ();
extern void		PullExposureEvents ();
extern int		SetupKeyBindings ();
extern Boolean		WmDispatchMenuEvent ();
extern Boolean		WmDispatchWsEvent ();

#else /* _NO_PROTO */

extern Boolean CheckForButtonAction (XButtonEvent *buttonEvent, 
				     Context context, Context subContext, 
				     ClientData *pCD);
extern Time GetTimestamp (void);
extern Boolean HandleKeyPress (XKeyEvent *keyEvent, KeySpec *keySpecs, 
			       Boolean checkContext, Context context, 
			       Boolean onlyFirst, ClientData *pCD);
extern void HandleWsButtonPress (XButtonEvent *buttonEvent);
extern void HandleWsButtonRelease (XButtonEvent *buttonEvent);
extern void HandleWsConfigureRequest (XConfigureRequestEvent *configureEvent);
extern void HandleWsEnterNotify (XEnterWindowEvent *enterEvent);
extern void HandleWsFocusIn (XFocusInEvent *focusEvent);
extern Boolean HandleWsKeyPress (XKeyEvent *keyEvent);
extern void HandleWsLeaveNotify (XLeaveWindowEvent *leaveEvent);
extern void IdentifyEventContext (XButtonEvent *event, ClientData *pCD, 
				  Context *pContext, int *pPartContext);
extern void InitEventHandling (void);
extern void ProcessClickBPress (XButtonEvent *buttonEvent, ClientData *pCD, 
				Context context, Context subContext);
extern void ProcessClickBRelease (XButtonEvent *buttonEvent, ClientData *pCD, 
				  Context context, Context subContext);
extern void PullExposureEvents (void);
extern int SetupKeyBindings (KeySpec *keySpecs, Window grabWindow, 
			     int keyboardMode, long context);
extern Boolean WmDispatchMenuEvent (XButtonEvent *event);
extern Boolean WmDispatchWsEvent (XEvent *event);
#endif /* _NO_PROTO */
