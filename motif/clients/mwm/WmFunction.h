#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)WmFunction.h	3.6 90/07/23";
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
extern Boolean F_Beep ();
extern Boolean F_Lower ();
extern void Do_Lower ();
extern Boolean F_Circle_Down ();
extern Boolean F_Circle_Up ();
extern Boolean F_Focus_Color ();
extern Boolean F_Exec ();
extern Boolean F_Quit_Mwm ();
extern void Do_Quit_Mwm ();
extern Boolean F_Focus_Key ();
extern void Do_Focus_Key ();
extern Boolean F_Next_Key ();
extern Boolean F_Prev_Cmap ();
extern Boolean F_Prev_Key ();
extern Boolean F_Pass_Key ();
extern Boolean F_Maximize ();
extern Boolean F_Menu ();
extern Boolean F_Minimize ();
extern Boolean F_Move ();
extern Boolean F_Next_Cmap ();
extern Boolean F_Nop ();
extern Boolean F_Normalize ();
extern Boolean F_Normalize_And_Raise ();
extern Boolean F_Pack_Icons ();
extern Boolean F_Post_SMenu ();
extern Boolean F_Kill ();
extern Boolean F_Refresh ();
extern Boolean F_Resize ();
extern Boolean F_Restart ();
extern void Do_Restart ();
extern void RestartWm ();
extern void DeFrameClient ();
extern Boolean F_Send_Msg ();
extern Boolean F_Separator ();
extern Boolean F_Raise ();
extern void Do_Raise ();
extern Boolean F_Raise_Lower ();
extern Boolean F_Refresh_Win ();
extern Boolean F_Set_Behavior ();
extern void Do_Set_Behavior ();
extern Boolean F_Title ();
extern Time GetFunctionTimestamp ();
extern void ReBorderClient ();

#else /* _NO_PROTO */

extern Boolean F_Beep (String args, ClientData *pCD, XEvent *event);
extern Boolean F_Lower (String args, ClientData *pCD, XEvent *event);
extern void Do_Lower (ClientData *pCD, ClientListEntry *pStackEntry);
extern Boolean F_Circle_Down (String args, ClientData *pCD, XEvent *event);
extern Boolean F_Circle_Up (String args, ClientData *pCD, XEvent *event);
extern Boolean F_Focus_Color (String args, ClientData *pCD, XEvent *event);
extern Boolean F_Exec (String args, ClientData *pCD, XEvent *event);
extern Boolean F_Quit_Mwm (String args, ClientData *pCD, XEvent *event);
extern void Do_Quit_Mwm (Boolean diedOnRestart);
extern Boolean F_Focus_Key (String args, ClientData *pCD, XEvent *event);
extern void Do_Focus_Key (ClientData *pCD, Time focusTime, long flags);
extern Boolean F_Next_Key (String args, ClientData *pCD, XEvent *event);
extern Boolean F_Prev_Cmap (String args, ClientData *pCD, XEvent *event);
extern Boolean F_Prev_Key (String args, ClientData *pCD, XEvent *event);
extern Boolean F_Pass_Key (String args, ClientData *pCD, XEvent *event);
extern Boolean F_Maximize (String args, ClientData *pCD, XEvent *event);
extern Boolean F_Menu (String args, ClientData *pCD, XEvent *event);
extern Boolean F_Minimize (String args, ClientData *pCD, XEvent *event);
extern Boolean F_Move (String args, ClientData *pCD, XEvent *event);
extern Boolean F_Next_Cmap (String args, ClientData *pCD, XEvent *event);
extern Boolean F_Nop (String args, ClientData *pCD, XEvent *event);
extern Boolean F_Normalize (String args, ClientData *pCD, XEvent *event);
extern Boolean F_Normalize_And_Raise (String args, ClientData *pCD, 
				      XEvent *event);
extern Boolean F_Pack_Icons (String args, ClientData *pCD, XEvent *event);
extern Boolean F_Post_SMenu (String args, ClientData *pCD, XEvent *event);
extern Boolean F_Kill (String args, ClientData *pCD, XEvent *event);
extern Boolean F_Refresh (String args, ClientData *pCD, XEvent *event);
extern Boolean F_Resize (String args, ClientData *pCD, XEvent *event);
extern Boolean F_Restart (String args, ClientData *pCD, XEvent *event);
extern void Do_Restart (Boolean dummy);
extern void RestartWm (long startupFlags);
extern void DeFrameClient (ClientData *pCD);
extern Boolean F_Send_Msg (String args, ClientData *pCD, XEvent *event);
extern Boolean F_Separator (String args, ClientData *pCD, XEvent *event);
extern Boolean F_Raise (String args, ClientData *pCD, XEvent *event);
extern void Do_Raise (ClientData *pCD, ClientListEntry *pStackEntry);
extern Boolean F_Raise_Lower (String args, ClientData *pCD, XEvent *event);
extern Boolean F_Refresh_Win (String args, ClientData *pCD, XEvent *event);
extern Boolean F_Set_Behavior (String args, ClientData *pCD, XEvent *event);
extern void Do_Set_Behavior (Boolean dummy);
extern Boolean F_Title (String args, ClientData *pCD, XEvent *event);
extern Time GetFunctionTimestamp (XButtonEvent *pEvent);
extern void ReBorderClient (ClientData *pCD, Boolean reMapClient);
#endif /* _NO_PROTO */

