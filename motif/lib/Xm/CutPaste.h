#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)CutPaste.h	3.13 91/01/10";
#endif /* lint */
#endif /* REV_INFO */
/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
*  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
*  (c) Copyright 1987, 1988, 1989, 1990, HEWLETT-PACKARD COMPANY
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
#ifndef _XmCutPaste_h
#define _XmCutPaste_h

#include <Xm/Xm.h>

#define ClipboardFail     	0
#define ClipboardSuccess  	1 
#define ClipboardTruncate 	2
#define ClipboardLocked   	4
#define ClipboardBadFormat   	5
#define ClipboardNoData   	6

typedef struct {
    int DataId;
    int PrivateId;
} XmClipboardPendingRec, *XmClipboardPendingList;

typedef void (*VoidProc) ();

#ifdef _NO_PROTO

  extern int XmClipboardBeginCopy();
  extern int XmClipboardStartCopy();
  extern int XmClipboardCopy();
  extern int XmClipboardEndCopy();
  extern int XmClipboardCancelCopy();
  extern int XmClipboardCopyByName();
  extern int XmClipboardUndoCopy();
  extern int XmClipboardLock();
  extern int XmClipboardUnlock();
  extern int XmClipboardStartRetrieve();
  extern int XmClipboardRetrieve();
  extern int XmClipboardEndRetrieve();
  extern int XmClipboardInquireCount();
  extern int XmClipboardInquireFormat();
  extern int XmClipboardInquireLength();
  extern int XmClipboardInquirePendingItems();
  extern int XmClipboardRegisterFormat();
  extern int XmClipboardWithdrawFormat();

#else
#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

  extern int XmClipboardBeginCopy(Display *display, Window window, XmString label,
	Widget widget, VoidProc callback, long *itemid);
  extern int XmClipboardStartCopy(Display *display, Window window, XmString label,
	Time timestamp, Widget widget, VoidProc callback, long *itemid);
  extern int XmClipboardCopy(Display *display, Window window, long itemid,
	char *format, char *buffer, unsigned long length, int private_id,
	int *dataid);
  extern int XmClipboardEndCopy(Display *display, Window window, long itemid);
  extern int XmClipboardCancelCopy(Display *display, Window window, long itemid);
  extern int XmClipboardCopyByName(Display *display, Window window, int data,
	char *buffer, unsigned long length, int private_id);
  extern int XmClipboardUndoCopy(Display *display, Window window);
  extern int XmClipboardLock(Display *display, Window window);
  extern int XmClipboardUnlock(Display *display, Window window,  
#if NeedWidePrototypes
int all_levels
#else
Boolean all_levels
#endif 
);
  extern int XmClipboardStartRetrieve(Display *display, Window window, Time timestamp);
  extern int XmClipboardRetrieve(Display *display, Window window, char *format,
	char *buffer, unsigned long length, unsigned long *outlength,
	int *private_id);
  extern int XmClipboardEndRetrieve(Display *display, Window window);
  extern int XmClipboardInquireCount(Display *display, Window window, int *count,
	int *maxlength);
  extern int XmClipboardInquireFormat(Display *display, Window window, int n,
	char *buffer, unsigned long bufferlength, unsigned long *outlength);
  extern int XmClipboardInquireLength(Display *display, Window window, char *format,
	unsigned long *length);
  extern int XmClipboardInquirePendingItems(Display *display, Window window,
	char *format, XmClipboardPendingList *list, unsigned long *count);
  extern int XmClipboardRegisterFormat(Display *display, String format_name,
	unsigned long format_length);
  extern int XmClipboardWithdrawFormat(Display *display, Window window, int data);
#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif /* NO_PROTO */

#endif /* _XmCutPaste_h */
/* DON'T ADD ANYTHING AFTER THIS #endif */
