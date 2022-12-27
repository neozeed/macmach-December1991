#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)ArrowBtn3.h	3.3  91/01/11";
#endif /* lint */
#endif /* REV_INFO */
/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
*  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
*  ALL RIGHTS RESERVED
*  
*  	THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED
*  AND COPIED ONLY IN ACCORDANCE WITH THE TERMS OF SUCH LICENSE AND
*  WITH THE INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR
*  ANY OTHER COPIES THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE
*  AVAILABLE TO ANY OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF THE
*  SOFTWARE IS HEREBY TRANSFERRED.
*  
*  	THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE WITHOUT
*  NOTICE AND SHOULD NOT BE CONSTRUED AS A COMMITMENT BY OPEN SOFTWARE
*  FOUNDATION, INC. OR ITS THIRD PARTY SUPPLIERS  
*  
*  	OPEN SOFTWARE FOUNDATION, INC. AND ITS THIRD PARTY SUPPLIERS,
*  ASSUME NO RESPONSIBILITY FOR THE USE OR INABILITY TO USE ANY OF ITS
*  SOFTWARE .   OSF SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
*  KIND, AND OSF EXPRESSLY DISCLAIMS ALL IMPLIED WARRANTIES, INCLUDING
*  BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
*  FITNESS FOR A PARTICULAR PURPOSE.
*  
*  Notice:  Notwithstanding any other lease or license that may pertain to,
*  or accompany the delivery of, this computer software, the rights of the
*  Government regarding its use, reproduction and disclosure are as set
*  forth in Section 52.227-19 of the FARS Computer Software-Restricted
*  Rights clause.
*  
*  (c) Copyright 1989, 1990, 1991 Open Software Foundation, Inc.  Unpublished - all
*  rights reserved under the Copyright laws of the United States.
*  
*  RESTRICTED RIGHTS NOTICE:  Use, duplication, or disclosure by the
*  Government is subject to the restrictions as set forth in subparagraph
*  (c)(1)(ii) of the Rights in Technical Data and Computer Software clause
*  at DFARS 52.227-7013.
*  
*  Open Software Foundation, Inc.
*  11 Cambridge Center
*  Cambridge, MA   02142
*  (617)621-8700
*  
*  RESTRICTED RIGHTS LEGEND:  This computer software is submitted with
*  "restricted rights."  Use, duplication or disclosure is subject to the
*  restrictions as set forth in NASA FAR SUP 18-52.227-79 (April 1985)
*  "Commercial Computer Software- Restricted Rights (April 1985)."  Open
*  Software Foundation, Inc., 11 Cambridge Center, Cambridge, MA  02142.  If
*  the contract contains the Clause at 18-52.227-74 "Rights in Data General"
*  then the "Alternate III" clause applies.
*  
*  (c) Copyright 1989, 1990, 1991 Open Software Foundation, Inc.
*  ALL RIGHTS RESERVED 
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
*******************************************************************************
******************************************************************************/
#define MAX_ARGS 	100

Boolean   wait;
Widget    ContinuePopup = NULL;

/*  Private Functions  */
static void            Pause();
static void            FlushEvents();
static Widget          CreateContinuePopup();
static XtCallbackProc  ContinueCB();
static XtCallbackProc  QuitCB();

static char *Pause_events[] = 
      {
	"1) A client with arrow button pointing upwards should appear.\n2) Two messages should be printed in term window. The first says that\n   the unitType is inches (1000TH_INCHES) and the size is set at 2000x1000.\n   The next line prints out the width and height. These\n   values are returned by XtGetValues. The width and height should be\n   approximately, but not exactly, the same as the values in the first line. This is because\n   there is a rounding difference when converting 1000TH_INCHES to pixels.\n2) Continue for more testing\n",
	"1) The arrow button should change size 10 times.\n2) Each time, the Expected and the Returned sizes should be printed \n in the term window. The Expected values are those used in XtSetValues \n and the Returned values are those retured by XtGetValues. The Returned \n values should be approximately equal to the Expected values.\n This is beacause there is a rounding difference when converting 1000TH_INCHES \n to pixels.\n3) Cointinue for more testing\n",
	"1) A message should be printed in the term window saying that unit type \n is being set to millimeters (100TH_MILLIMETERS).\n2) The arrow button should change size 10 times.\n3) Each time, the Expected and the Returned sizes should be printed \n in the term window. The Expected values are those used in XtSetValues \n and the Returned values are those retured by XtGetValues. The Returned \n values should be approximately equal to the Expected values.\n This is beacause there is a rounding difference when \n converting 100TH_MILLIMETERS to pixels.\n4) Cointinue for more testing\n",
	"1) A arrow button should change size to 2000 x 1000.\n2) Two messages should be printed in term window. The first says that\n   the size is set at 2000x1000. The next line is again the width and\n   height printed out. These values are returned by XtGetValues. The\n   width and height should be approximately 2000 by 1000.\n   This is because there is a rounding difference when\n   converting 100TH_MILLIMETERS to pixels.\n3) Test Finished -- Exit Please"
      };
	
int Pause_cnt = 0;
XtAppContext  app_context;
