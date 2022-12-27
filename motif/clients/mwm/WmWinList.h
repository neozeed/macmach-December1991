#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)WmWinList.h	3.4 90/07/23";
#endif /* lint */
#endif /* REV_INFO */
 /*****************************************************************************
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

extern void			AddClientToList ();
extern void			AddEntryToList ();
extern void			AddTransient ();
extern Boolean			CheckIfClientObscuredByAny ();
extern Boolean			CheckIfClientObscuring ();
extern Boolean			CheckIfClientObscuringAny ();
extern Boolean			CheckIfObscuring ();
extern int			CountTransientChildren ();
extern void			DeleteClientFromList ();
extern void			DeleteEntryFromList ();
extern void			DeleteFullAppModalChildren();
extern void			DeleteTransient();
extern ClientListEntry *	FindClientNameMatch ();
extern ClientData *		FindTransientFocus ();
extern ClientData *		FindTransientOnTop ();
extern ClientData *		FindTransientTreeLeader ();
extern void			FixupFullAppModalCounts();
extern Window *		        MakeTransientWindowList ();
extern void			MarkModalSubtree ();
extern void			MarkModalTransient ();
extern void			MoveEntryInList ();
extern Boolean			PutTransientBelowSiblings ();
extern Boolean			PutTransientOnTop ();
extern void			RestackTransients ();
extern void			RestackTransientsAtWindow ();
extern void			SetupSystemModalState ();
extern void			StackTransientWindow ();
extern void			StackWindow ();
extern void			UnMarkModalTransient ();
extern void			UndoSystemModalState ();

#else /* _NO_PROTO */

extern void AddClientToList (WmWorkspaceData *pWS, ClientData *pCD, 
			     Boolean onTop);
extern void AddEntryToList (WmWorkspaceData *pWS, ClientListEntry *pEntry, 
			    Boolean onTop, ClientListEntry *pStackEntry);
extern void AddTransient (WmWorkspaceData *pWS, ClientData *pCD);
extern Boolean CheckIfClientObscuredByAny (ClientData *pcd);
extern Boolean CheckIfClientObscuring (ClientData *pcdTop, ClientData *pcd);
extern Boolean CheckIfClientObscuringAny (ClientData *pcd);
extern Boolean CheckIfObscuring (ClientData *pcdA, ClientData *pcdB);
extern int CountTransientChildren (ClientData *pcd);
extern void DeleteClientFromList (WmWorkspaceData *pWS, ClientData *pCD);
extern void DeleteEntryFromList (WmWorkspaceData *pWS, 
				 ClientListEntry *pListEntry);
extern void DeleteFullAppModalChildren (ClientData *pcdLeader, 
					ClientData *pCD);
extern void DeleteTransient (ClientData *pCD);
extern ClientListEntry *FindClientNameMatch (ClientListEntry *pEntry, 
					     Boolean toNext, 
					     String clientName,
					     unsigned long types);
extern ClientData *FindTransientFocus (ClientData *pcd);
extern ClientData *FindTransientOnTop (ClientData *pcd);
extern ClientData *FindTransientTreeLeader (ClientData *pcd);
extern void FixupFullAppModalCounts (ClientData *pcdLeader, 
				     ClientData *pcdDelete);
extern Window *MakeTransientWindowList (Window *windows, ClientData *pcd);
extern void MarkModalSubtree (ClientData *pcdTree, ClientData *pcdAvoid);
extern void MarkModalTransient (ClientData *pcdLeader, ClientData *pCD);
extern void MoveEntryInList (WmWorkspaceData *pWS, ClientListEntry *pEntry, 
			     Boolean onTop, ClientListEntry *pStackEntry);
extern Boolean PutTransientBelowSiblings (ClientData *pcd);
extern Boolean PutTransientOnTop (ClientData *pcd);
extern void RestackTransients (ClientData *pcd, Boolean doTop);
extern void RestackTransientsAtWindow (ClientData *pcd);
extern void SetupSystemModalState (ClientData *pCD);
extern void StackTransientWindow (ClientData *pcd);
extern void StackWindow (WmWorkspaceData *pWS, ClientListEntry *pEntry, 
			 Boolean onTop, ClientListEntry *pStackEntry);
extern void UnMarkModalTransient (ClientData *pcdModee, int modalCount, 
				  ClientData *pcdModal);
extern void UndoSystemModalState (void);

#endif /* _NO_PROTO */
