#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)TextInP.h	3.10 91/01/10";
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
#ifndef _XmTextInP_h
#define _XmTextInP_h

/****************************************************************
 *
 * Definitions for modules implementing text input modules.
 *
 ****************************************************************/

typedef int (*ActionProc)();

typedef struct {
    int x;
    int y;
} SelectionHint;

typedef struct _InputDataRec {
    XmTextWidget widget;		/* Back-pointer to widget record. */
    XComposeStatus compstatus;	/* Compose key status. */
    XmTextScanType *sarray;	/* Description of what to cycle through on */
				/* selections. */
    int sarraycount;		/* Number of elements in above. */
    int new_sel_length;		/* New selection length for selection moves. */
    Time lasttime;		/* Time of last event. */
    Boolean pendingdelete;	/* TRUE if we're implementing pending delete */
    Boolean syncing;		/* If TRUE, then we've multiple keystrokes */
    Boolean extending;      /* true if we are extending */
    Boolean Sel2Extending;	/* true if we are extending */
    Boolean hasSel2;   		/* has secondary selection */
    Boolean has_destination;  	/* has destination selection */
    Boolean selectionMove;	/* delete selection after stuff */
    Boolean cancel;		/* indicates that cancel was pressed */
    SelectionHint selectionHint; /* saved coords of button down */
    SelectionHint Sel2Hint;	/* saved the coords of button down */
    int threshold;		/* number of pixels crossed -> drag */
    XmTextScanType stype;	/* Current selection type. */
    XmTextScanDirection extendDir;
    XmTextScanDirection Sel2ExtendDir;
    XmTextPosition origLeft, origRight;
    XmTextPosition Sel2OrigLeft, Sel2OrigRight;
    XmTextPosition stuffpos;
    XmTextPosition sel2Left, sel2Right; /* secondary selection */
    XmTextPosition anchor;	/* anchor point of the primary selection */
    Time dest_time;		/* time of destination selection ownership */
    Time sec_time;		/* time of secondary selection ownership */
    Boolean changed_dest_visible;   /* destination visibility changed */
} InputDataRec, *InputData;


/* 
 * Create a new instance of an input object.  By the time this is called,
 * the widget context has been saved.
 */

typedef void (*InputCreateProc)(); /* widget, args, num_args */
    /* XmTextWidget widget; */
    /* ArgList args; */
    /* int num_args; */

/*
 * Get values out of the input object.
 */

typedef void (*InputGetValuesProc)(); /* widget, args, num_args */
    /* XmTextWidget widget; */
    /* ArgList args; */
    /* int num_args; */

/*
 * Set values in the input object.
 */

typedef void (*InputSetValuesProc)(); /* widget, args, num_args */
    /* XmTextWidget widget; */
    /* ArgList args; */
    /* int num_args; */

/*
 * Inform input of invalidated positions.
 */

#ifdef _NO_PROTO
typedef void (*InputInvalidateProc)(); /* ctx, position, topos, delta */
    /* XmTextWidget ctx; */
    /* XmTextPosition position, topos; */
    /* int delta; */
#else
#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif
  
typedef void (*InputInvalidateProc)(
    XmTextWidget ctx,
    XmTextPosition position,
    XmTextPosition topos,
    long delta);

#if defined(__cplusplus) || defined(c_plusplus)
};
#endif
#endif


typedef struct _InputRec {
    struct _InputDataRec *data;	/* Input-specific data; opaque type. */
    InputInvalidateProc Invalidate;
    InputGetValuesProc  GetValues;
    InputSetValuesProc	SetValues;
    XtWidgetProc	destroy;
} InputRec;

#endif /* _XmTextInP_h */
/*DON'T ADD ANYTHING AFTER THIS #endif */
