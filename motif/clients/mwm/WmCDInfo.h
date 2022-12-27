#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)WmCDInfo.h	3.4 90/06/19";
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
extern int FrameX ();
extern int FrameY ();
extern unsigned int FrameWidth ();
extern unsigned int FrameHeight ();
extern unsigned int TitleBarHeight ();
extern unsigned int TitleTextHeight ();
extern unsigned int UpperBorderWidth ();
extern unsigned int LowerBorderWidth ();
extern unsigned int CornerWidth ();
extern unsigned int CornerHeight ();
extern int BaseWindowX ();
extern int BaseWindowY ();
extern unsigned int BaseWindowWidth ();
extern unsigned int BaseWindowHeight ();
extern Boolean GetFramePartInfo ();
extern int IdentifyFramePart ();
extern int GadgetID ();
extern void FrameToClient ();
extern void ClientToFrame ();
extern Boolean GetDepressInfo ();
extern void SetFrameInfo ();
extern void SetClientOffset ();
extern Boolean XBorderIsShowing ();
#else /* _NO_PROTO */
extern int FrameX (ClientData *pcd);
extern int FrameY (ClientData *pcd);
extern unsigned int FrameWidth (ClientData *pcd);
extern unsigned int FrameHeight (ClientData *pcd);
extern unsigned int TitleBarHeight (ClientData *pcd);
extern unsigned int TitleTextHeight (ClientData *pcd);
extern unsigned int UpperBorderWidth (ClientData *pcd);
extern unsigned int LowerBorderWidth (ClientData *pcd);
extern unsigned int CornerWidth (ClientData *pcd);
extern unsigned int CornerHeight (ClientData *pcd);
extern int BaseWindowX (ClientData *pcd);
extern int BaseWindowY (ClientData *pcd);
extern unsigned int BaseWindowWidth (ClientData *pcd);
extern unsigned int BaseWindowHeight (ClientData *pcd);
extern Boolean GetFramePartInfo (ClientData *pcd, int part, int *pX, int *pY, unsigned int *pWidth, unsigned int *pHeight);
extern int IdentifyFramePart (ClientData *pCD, int x, int y);
extern int GadgetID (int x, int y, GadgetRectangle *pgadget, unsigned int count);
extern void FrameToClient (ClientData *pcd, int *pX, int *pY, unsigned int *pWidth, unsigned int *pHeight);
extern void ClientToFrame (ClientData *pcd, int *pX, int *pY, unsigned int *pWidth, unsigned int *pHeight);
extern Boolean GetDepressInfo (ClientData *pcd, int part, int *pX, int *pY, unsigned int *pWidth, unsigned int *pHeight, unsigned int *pInvertWidth);
extern void SetFrameInfo (ClientData *pcd);
extern void SetClientOffset (ClientData *pcd);
extern Boolean XBorderIsShowing (ClientData *pcd);
#endif /* _NO_PROTO */
