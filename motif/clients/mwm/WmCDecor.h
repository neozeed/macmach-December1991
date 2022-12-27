#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)WmCDecor.h	3.6 91/01/10";
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


#ifdef _NO_PROTO
extern Boolean FrameWindow ();
extern void FrameExposureProc ();
extern void SetFrameShape ();
extern Boolean ConstructFrame ();
extern void GenerateFrameDisplayLists ();
extern void AdoptClient ();
extern void GetTextBox ();
extern void DrawWindowTitle ();
extern void CreateStretcherWindows ();
extern void CountFrameRectangles ();
extern Boolean AllocateFrameDisplayLists ();
extern void InitClientDecoration ();
extern Boolean AllocateGadgetRectangles ();
extern void ComputeGadgetRectangles ();
extern void GetSystemMenuPosition ();
extern void ShowActiveClientFrame ();
extern void ShowInactiveClientFrame ();
extern void RegenerateClientFrame ();
extern void BevelSystemButton ();
extern void BevelMinimizeButton ();
extern void BevelMaximizeButton ();
extern Boolean DepressGadget ();
extern void PushGadgetIn ();
extern void PopGadgetOut ();
#else /* _NO_PROTO */
extern Boolean FrameWindow (ClientData *pcd);
extern void FrameExposureProc (ClientData *pcd);
extern void SetFrameShape (ClientData *pcd);
extern Boolean ConstructFrame (ClientData *pcd);
extern void GenerateFrameDisplayLists (ClientData *pcd);
extern void AdoptClient (ClientData *pcd);
extern void GetTextBox (ClientData *pcd, XRectangle *pBox);
extern void DrawWindowTitle (ClientData *pcd, Boolean eraseFirst);
extern void CreateStretcherWindows (ClientData *pcd);
extern void CountFrameRectangles (WmScreenData *pSD);
extern Boolean AllocateFrameDisplayLists (ClientData *pcd);
extern void InitClientDecoration (WmScreenData *pSD);
extern Boolean AllocateGadgetRectangles (ClientData *pcd);
extern void ComputeGadgetRectangles (ClientData *pcd);
extern void GetSystemMenuPosition (ClientData *pcd, int *px, int *py, unsigned int height, Context context);
extern void ShowActiveClientFrame (ClientData *pcd);
extern void ShowInactiveClientFrame (ClientData *pcd);
extern void RegenerateClientFrame (ClientData *pcd);
extern void BevelSystemButton (RList *prTop, RList *prBot, int x, int y, unsigned int width, unsigned int height);
extern void BevelMinimizeButton (RList *prTop, RList *prBot, int x, int y, unsigned int height);
extern void BevelMaximizeButton (RList *prTop, RList *prBot, int x, int y, unsigned int height);
extern Boolean DepressGadget (ClientData *pcd, int gadget, Boolean depressed);
extern void PushGadgetIn (ClientData *pcd, int gadget);
extern void PopGadgetOut (ClientData *pcd, int gadget);
#endif /* _NO_PROTO */
