#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)WmICCC.h	3.5 90/06/19";
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


/*
 * Included Files:
 */

#include <X11/Xatom.h>
#include <X11/Xmd.h>


/*
 * Value definitions and macros:
 */



/*************************************<->*************************************
 *
 *  ICCC property data structures ...
 *
 *
 *  Description:
 *  -----------
 *  These data structures are mwm versions of the window manager data
 *  stuctures that are specified in the ICCCM and the Xlib specification
 *  for client/window manager communication.  In general these data
 *  structures correspond to client window property values.
 *
 *************************************<->***********************************/


/* mwm version of the xPropSizeHints structure: */

typedef struct _PropSizeHints
{
    CARD32	flags;
    INT32	x;				/* R2 conventions */
    INT32	y;				/* R2 conventions */
    INT32	width;				/* R2 conventions */
    INT32	height;				/* R2 conventions */
    INT32	minWidth;
    INT32	minHeight;
    INT32	maxWidth;
    INT32	maxHeight;
    INT32	widthInc;
    INT32	heightInc;
    INT32	minAspectX;
    INT32	minAspectY;
    INT32	maxAspectX;
    INT32	maxAspectY;
    INT32	baseWidth;			/* current conventions */
    INT32	baseHeight;			/* current conventions */
    INT32	winGravity;			/* current conventions */

} PropSizeHints;

#define PROP_SIZE_HINTS_ELEMENTS	18


/* mwm version of the XSizeHints structure: */

typedef struct _SizeHints
{
    int		icccVersion;
    long	flags;
    int		x;				/* R2 conventions */
    int		y;				/* R2 conventions */
    int		width;				/* R2 conventions */
    int		height;				/* R2 conventions */
    int		min_width;
    int		min_height;
    int		max_width;
    int		max_height;
    int		width_inc;
    int		height_inc;
    AspectRatio min_aspect;
    AspectRatio max_aspect;
    int		base_width;			/* current conventions */
    int		base_height;			/* current conventions */
    int		win_gravity;			/* current conventions */

} SizeHints;

/* mwm version of the xPropWMState structure: */

typedef struct _PropWMState
{
    CARD32	state;
    BITS32	icon;
} PropWMState;

#define PROP_WM_STATE_ELEMENTS		2


/* ICCC versions (icccVersion): */
#define ICCC_R2		0
#define ICCC_CURRENT	1
#define ICCC_UNKNOWN	ICCC_CURRENT


/* SizeHints flags field values: */
#define US_POSITION		(1L << 0)
#define US_SIZE			(1L << 1)
#define P_POSITION		(1L << 2)
#define P_SIZE			(1L << 3)
#define P_MIN_SIZE		(1L << 4)
#define P_MAX_SIZE		(1L << 5)
#define P_RESIZE_INC		(1L << 6)
#define P_ASPECT		(1L << 7)
#define P_BASE_SIZE		(1L << 8)
#define P_WIN_GRAVITY		(1L << 9)

/* PropWMState state field value: */
#define WithdrawnSTATE		0
