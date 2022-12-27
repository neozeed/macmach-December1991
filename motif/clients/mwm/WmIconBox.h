#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)WmIconBox.h	3.8 90/07/23";
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

extern void	  AddNewBox ();
extern void	  ChangeActiveIconboxIconText ();
extern void	  CheckIconBoxResize ();
extern Boolean	  CheckIconBoxSize ();
extern void	  DeleteIconFromBox ();
extern void	  DeleteIconInfo ();
extern Boolean	  ExpandVirtualSpace ();
extern Boolean	  ExtendIconList();
extern void	  FindNewPosition ();
extern MenuItem  *GetIconBoxMenuItems ();
extern void       GetIconBoxIconRootXY ();
extern void	  HandleIconBoxButtonMotion ();
extern void       HandleIconBoxIconKeyPress ();
extern void	  IconActivateCallback ();
extern Boolean    IconVisible ();
extern IconInfo	 *InsertIconInfo ();
extern Boolean	  InsertIconIntoBox ();
extern void	  InitIconBox ();
extern void	  InitializeClientData ();
extern void	  InitializeIconBoxData ();
extern Cardinal	  InsertPosition ();
extern void	  MakeBulletinBoard ();
extern void	  MakeFadeIconGC ();
extern Boolean	  MakeIconBox ();
extern void	  MakeScrolledWindow ();
extern void	  MakeShell ();
extern void	  MakeShrinkWrapIconsGC ();
extern void	  MapIconBoxes ();
extern void	  PackIconBox ();
extern void	  RealignIconList ();
extern void	  RealizeIconBox ();
extern void       ReorderIconBoxIcons ();
extern void	  ResetArrowButtonIncrements();
extern void	  ResetIconBoxMaxSize ();
extern void	  SetGeometry ();
extern void	  SetGranularity ();
extern void	  SetIconBoxInfo ();
extern void	  SetNewBounds ();
extern void	  ShowClientIconState ();
extern void	  UpdateIncrements ();
extern String     WmXmStringToString ();

#else /* _NO_PROTO */

extern void AddNewBox (WmWorkspaceData *pWS, IconBoxData *pIBD);
extern void ChangeActiveIconboxIconText (Widget icon, caddr_t dummy, 
					 XFocusChangeEvent *event);
extern void CheckIconBoxResize (ClientData *pCD, unsigned int changedValues, 
				int newWidth, int newHeight);
extern Boolean CheckIconBoxSize (IconBoxData *pIBD);
extern void DeleteIconFromBox (IconBoxData *pIBD, ClientData *pCD);
extern void DeleteIconInfo (IconBoxData *pIBD, ClientData *pCD);
extern Boolean ExpandVirtualSpace (IconBoxData *pIBD, int newWidth, 
				   int newHeight);
extern Boolean ExtendIconList (IconBoxData *pIBD, int incr);
extern void FindNewPosition (Cardinal *newPosition, IconPlacementData *pIPD, 
			     int newPlace);
extern MenuItem *GetIconBoxMenuItems (WmScreenData *pSD);
extern void GetIconBoxIconRootXY (ClientData *pCD, int *pX, int *pY);
extern void HandleIconBoxButtonMotion (Widget icon, caddr_t client_data, 
				       XEvent *pev);
extern void HandleIconBoxIconKeyPress (Widget icon, caddr_t dummy, 
				       XKeyEvent *keyEvent);
extern void IconActivateCallback (Widget w, caddr_t client_data, 
				  XmAnyCallbackStruct *call_data);
extern Boolean IconVisible (ClientData *pCD);
extern IconInfo *InsertIconInfo (IconBoxData *pIBD, ClientData *pCD, 
				 Widget theWidget);
extern Boolean InsertIconIntoBox (IconBoxData *pIBD, ClientData *pCD);
extern void InitIconBox (WmScreenData *pSD);
extern void InitializeClientData (ClientData *pCD, IconBoxData *pIBD);
extern void InitializeIconBoxData (WmWorkspaceData *pWS, IconBoxData *pIBD);
extern Cardinal InsertPosition (Widget w);
extern void MakeBulletinBoard (WmWorkspaceData *pWS, IconBoxData *pIBD);
extern void MakeFadeIconGC (WmScreenData *pSD);
extern Boolean MakeIconBox (WmWorkspaceData *pWS, ClientData *pCD);
extern void MakeScrolledWindow (WmWorkspaceData *pWS, IconBoxData *pIBD);
extern void MakeShell (WmWorkspaceData *pWS, IconBoxData *pIBD);
extern void MakeShrinkWrapIconsGC (WmScreenData *pSD, Pixmap bgPixmap);
extern void MapIconBoxes (WmWorkspaceData *pWS);
extern void PackIconBox (IconBoxData *pIBD, Boolean packVert, 
			 Boolean packHorz, int passedInWidth, 
			 int passedInHeight);
extern void RealignIconList (IconBoxData *pIBD, int newCols, int newRows);
extern void RealizeIconBox (WmWorkspaceData *pWS, IconBoxData *pIBD, 
			    ClientData *pCD);
extern void ReorderIconBoxIcons (ClientData *pCD, IconBoxData *pIBD, 
				 Widget theIcon, int newX, int newY);
extern void ResetArrowButtonIncrements (ClientData *pCD);
extern void ResetIconBoxMaxSize (ClientData *pCD, Widget bBoardWidget);
extern void SetGeometry (WmWorkspaceData *pWS, ClientData *pCD, 
			 IconBoxData *pIBD);
extern void SetGranularity (WmWorkspaceData *pWS, ClientData *pCD, 
			    IconBoxData *pIBD);
extern void SetIconBoxInfo (WmWorkspaceData *pWS, ClientData *pCD);
extern void SetNewBounds (IconBoxData *pIBD);
extern void ShowClientIconState (ClientData *pCD, int newState);
extern void UpdateIncrements (Widget sWidget, IconBoxData *pIBD, 
			      XConfigureEvent *event);
extern String WmXmStringToString (XmString xmString);
#endif /* _NO_PROTO */

