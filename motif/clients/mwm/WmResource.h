#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)WmResource.h	3.10 90/07/23";
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
extern void GetAppearanceGCs ();
extern GC   GetHighlightGC ();
extern void MakeAppearanceResources ();
extern void ProcessClientResources ();
extern void ProcessWmResources ();
extern void ProcessScreenListResource ();
extern void ProcessAppearanceResources ();
void ProcessGlobalScreenResources ();
extern void ProcessScreenResources ();
extern void ProcessWmColors ();
extern void ProcessWorkspaceList ();
extern void ProcessWorkspaceResources ();
extern void SetStdClientResourceValues ();
extern void SetStdGlobalResourceValues ();
extern void SetStdScreenResourceValues ();
extern char *WmRealloc ();
extern char *WmMalloc ();
extern void SetupDefaultResources ();
extern Boolean SimilarAppearanceData ();
extern Boolean Monochrome ();
#else /* _NO_PROTO */
extern void GetAppearanceGCs (WmScreenData *pSD, Pixel fg, Pixel bg, XFontStruct *font, Pixmap bg_pixmap, Pixel ts_color, Pixmap ts_pixmap, Pixel bs_color, Pixmap bs_pixmap, GC *pGC, GC *ptsGC, GC *pbsGC);
extern GC   GetHighlightGC (WmScreenData *pSD, Pixel fg, Pixel bg, Pixmap pixmap);
extern void MakeAppearanceResources (WmScreenData *pSD, AppearanceData *pAData, Boolean makeActiveResources);
extern Boolean Monochrome (Screen *screen);
extern void ProcessWmColors (WmScreenData *pSD);
extern void ProcessWmResources (void);
extern void SetStdGlobalResourceValues (void);
extern void ProcessScreenListResource (void);
extern void ProcessAppearanceResources (WmScreenData *pSD);
void ProcessGlobalScreenResources (void);
extern void ProcessScreenResources (WmScreenData *pSD, unsigned char *screenName);
extern void ProcessWorkspaceList (WmScreenData *pSD);
extern void ProcessWorkspaceResources (WmWorkspaceData *pWS);
extern void ProcessClientResources (ClientData *pCD);
extern void SetStdClientResourceValues (ClientData *pCD);
extern void SetStdScreenResourceValues (WmScreenData *pSD);
extern char *WmRealloc (char *ptr, unsigned size);
extern char *WmMalloc (char *ptr, unsigned size);
extern void SetupDefaultResources (WmScreenData *pSD);
extern Boolean SimilarAppearanceData (AppearanceData *pAD1, AppearanceData *pAD2);
#endif /* _NO_PROTO */
