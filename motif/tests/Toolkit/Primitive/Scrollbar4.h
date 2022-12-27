#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)Scrollbar4.h	3.3  91/01/11";
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
static char *Pause_events[] = 
   {
        "1) A client window with a large horizontal scrollbar with two arrows\n   and a very thin slider will appear.\n2) Click MB1 on each end of the scrollbar.  The value should change,\n   and should be displayed in the xterm window.  Clicking MB1 on the\n   left arrow will increase the value, and clicking MB1 on the right\n   arrow will decrease the value.  The maximum value will be 90 and\n   the minimum value will be 0.\n3) Press the continue button to continue this test.\n",
        "1) The scrollbar will be much smaller, will now have a thicker slider, and\n   the processing direction will be reversed.  The initial value\n   should be 70.\n2) Press and hold down MB1 on the slider and drag the pointer to the\n   right and to the left.  Release MB1.  The current value should be\n   displayed in the xterm window.  The maximum value should be 90\n   and the minimum value should be 0.\n3) Press the continue button to continue this test.\n",
        "1) The scrollbar orientation will change to vertical, with two\n   arrows and a very thin slider.  The initial value should be 80.\n2) Click MB1 on each end of the scrollbar.  The value should change\n   and be displayed in the xterm window.  The maximum value will be\n   90 and the minimum value will be 0.  The bottom arrow increases\n   the value.\n3) Press the continue button to continue this test.\n",
        "1) The following resources will be changed by the stated amounts:\n     a) Value will increase by 5.\n     b) Slider size will increase by 5.\n     c) Increment will increase by 2.\n     d) Page increment will increase by 5.\n   These changes will be performed 10 times.  After each iteration,\n   the changes will be printed in the xterm window.\n2) Press the continue button to continue this test.\n",
        "1) The scrollbar will grow vertically, the arrows will grow larger,\n   and the slider will grow.\n2) An X Toolkit warning for setting the scrollbar value greater\n   than the maximum scrollbar value minus the scrollbar slider size\n   will be printed in the xterm window.\n3) The initial values for the scrollbar will be printed in the\n   xterm window.\n4) Press and hold down MB1 on the slider and drag the pointer up and\n   down.  No values will be printed to the xterm window.\n5) Click MB1 on the top and bottom arrows of the scrollbar.  The\n   slider will move in the direction of the arrow clicked on.  No\n   values will be printed.\n6) Press the continue button to continue this test.\n",
        "1) The scrollbar slider will be smaller.\n2) An X Toolkit warning for setting the scrollbar value greater\n   than the maximum scrollbar value minus the scrollbar slider size\n   will be printed in the xterm window.\n3) The initial values for the scrollbar will be printed in the xterm\n   window.\n4) Press and hold down MB1 on the slider and drag the pointer up and\n   down.  No values will be printed to the xterm window.\n5) Click MB1 on the top and bottom arrows of the scrollbar.  The\n   slider will move in the direction of the arrow clicked on.  No\n   values will be printed to the xterm window.\n6) Press the continue button to continue this test.\n",
        "1) The scrollbar slider size will not change.\n2) An X Toolkit warning for setting the scrollbar value greater\n   than the maximum scrollbar value minus the scrollbar slider size\n   will be printed in the xterm window.\n3) The initial values for the scrollbar will be printed in the xterm\n   window.\n4) Press and hold down MB1 on the slider and drag the pointer up and\n   down.  No values will be printed to the xterm window.\n5) Click MB1 on the top and bottom arrows of the scrollbar.  The\n   slider will move in the direction of the arrow clicked on.  No\n   values will be printed to the xterm window.\n6) Press the continue button to continue this test.\n",
        "1) The scrollbar slider will be at the top of the scrollbar.\n2) An X Toolkit warning for setting the scrollbar value greater\n   than the maximum scrollbar value minus the scrollbar\n   minimum value will be printed in the xterm window.\n3) The initial values for the scrollbar will be printed in the xterm\n   window.\n4) Press and hold down MB1 on the slider and drag the pointer up and\n   down.  No values will be printed to the xterm window.\n5) Click MB1 on the top and bottom arrows of the scrollbar.  The\n   slider will move in the direction of the arrow clicked on.  No\n   values will be printed to the xterm window.\n6) Press the continue button to continue this test.\n",
        "1) The scrollbar slider will be smaller.\n2) An X Toolkit warning for setting the scrollbar value less than\n   the minimum scrollbar value will be printed in the xterm window.\n3) The initial values for the scrollbar will be printed in the xterm\n   window.\n4) Press and hold down MB1 on the slider and drag the pointer up and\n   down.  No values will be printed to the xterm window.\n5) Click MB1 on the top and bottom arrows of the scrollbar.  The\n   slider will move in the direction of the arrow clicked on.  No\n   values will be printed to the xterm window.\n6) Press the continue button to continue this test.\n",
        "1) There will be no visible change in the scrollbar.\n2) An X Toolkit warning for setting the scrollbar value less than\n   he minimum scrollbar value will be printed in the xterm window.\n3) The initial values for the scrollbar will be printed in the xterm\n   window.\n4) Press and hold down MB1 on the slider and drag the pointer up and\n   down.  No values will be printed to the xterm window.\n5) Click MB1 on the top and bottom arrows of the scrollbar.  The\n   slider will move in the direction of the arrow clicked on.  No\n   values will be printed to the xterm window.\n6) Press the continue button to continue this test.\n",
        "1) There will be no visible change in the scrollbar.\n2) The initial values for the scrollbar will be printed in the xterm\n   window.\n3) Press and hold down MB1 on the slider and drag the pointer up and\n   down.  No values will be printed to the xterm window.\n4) Click MB1 on the top and bottom arrows of the scrollbar.  The\n   slider will move in the direction of the arrow clicked on.  No\n   values will be printed to the xterm window.\n5) Press the continue button to continue this test.\n",
        "1) The scrollbar slider will be at the bottom of the scrollbar.\n2) An X Toolkit warning for setting the scrollbar value greater\n   than the maximum scrollbar value minus the scrollbar slider size\n   will be printed in the xterm window.\n3) The initial values for the scrollbar will be printed in the xterm\n   window.\n4) Press and hold down MB1 on the slider and drag the pointer up and\n   down.  No values will be printed to the xterm window.\n5) Click MB1 on the top and bottom arrows of the scrollbar.  The\n   slider will move in the direction of the arrow clicked on.  No\n   values will be printed to the xterm window.\n6) Press the continue button to continue this test.\n",
        "1) The scrollbar slider will be larger.\n2) An X Toolkit warning for setting the scrollbar value greater\n   than the maximum scrollbar value minus the scrollbar slider size\n   will be printed in the xterm window.\n3) The initial values for the scrollbar will be printed in the xterm\n   window.\n4) Press and hold down MB1 on the slider and drag the pointer up and\n   down.  No values will be printed to the xterm window.\n5) Click MB1 on the top and bottom arrows of the scrollbar.  The\n   slider will move in the direction of the arrow clicked on.  No\n   values will be printed to the xterm window.\n6) Press the continue button to continue this test.\n",
        "1) There will be no visible change in the scrollbar.\n2) The initial values for the scrollbar will be printed in the xterm\n   window.\n3) Press and hold down MB1 on the slider and drag the pointer up and\n   down.  No values will be printed to the xterm window.\n4) Click MB1 on the top and bottom arrows of the scrollbar.  The\n   slider will move in the direction of the arrow clicked on.  No\n   values will be printed to the xterm window.\n5) Press the continue button to continue this test.\n",
        "1) The scrollbar slider will be larger.\n2) An X Toolkit warning for setting the scrollbar value greater\n   than the maximum scrollbar value minus the scrollbar slider size\n   will be printed in the xterm window.\n3) The initial values for the scrollbar will be printed in the xterm\n   window.\n4) Press and hold down MB1 on the slider and drag the pointer up and\n   down.  No values will be printed to the xterm window.\n5) Click MB1 on the top and bottom arrows of the scrollbar.  The\n   slider will move in the direction of the arrow clicked on.  No\n   values will be printed to the xterm window.\n6) Press the continue button to continue this test.\n",
        "1) The scrollbar slider will be at the top of the scrollbar.\n2) An X Toolkit warning for setting the scrollbar value less than\n   the minimum scrollbar value will be printed in the xterm window.\n3) The initial values for the scrollbar will be printed in the xterm\n   window.\n4) Press and hold down MB1 on the slider and drag the pointer up and\n   down.  No values will be printed to the xterm window.\n5) Click MB1 on the top and bottom arrows of the scrollbar.  The\n   slider will move in the direction of the arrow clicked on.  No\n   values will be printed to the xterm window.\n6) Press the continue button to continue this test.\n",
        "1) The scrollbar slider will be larger.\n2) An X Toolkit warning for setting the scrollbar value less than\n   the minimum scrollbar value will be printed in the xterm window.\n3) The initial values for the scrollbar will be printed in the xterm\n   window.\n4) Press and hold down MB1 on the slider and drag the pointer up and\n   down.  When you release MB1, the current scrollbar value will be\n   printed to the xterm window.\n5) Click MB1 on the top and bottom arrows of the scrollbar.  The\n   slider will move in the direction of the arrow clicked on.  The\n   current scrollbar value will be printed to the xterm window.\nTest Finished -- Exit Please",
   };

int Pause_cnt = 0;
XtAppContext  app_context;
