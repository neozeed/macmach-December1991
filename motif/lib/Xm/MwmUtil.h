#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)MwmUtil.h	3.4 90/03/28";
#endif /* lint */
#endif /* REV_INFO */
/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990, OPEN SOFTWARE FOUNDATION, INC.
*  (c) Copyright 1988, 1989, 1990, HEWLETT-PACKARD COMPANY
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
*  (c) Copyright 1989, 1990, Open Software Foundation, Inc.  Unpublished - all
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
*  (c) Copyright 1989, 1990, Open Software Foundation, Inc.
*  ALL RIGHTS RESERVED 
*  
*  
* Open Software Foundation is a trademark of The Open Software Foundation, Inc.
* OSF is a trademark of Open Software Foundation, Inc.
* OSF/Motif is a trademark of Open Software Foundation, Inc.
* Motif is a trademark of Open Software Foundation, Inc.
* X Window System is a trademark of the Massachusetts Institute of Technology
*
*******************************************************************************
******************************************************************************/
#ifndef _MwmUtil_h
#define _MwmUtil_h

#include <X11/Xmd.h>	/* for protocol typedefs */

/*
 * Contents of the _MWM_HINTS property.
 */

typedef struct
{
    long	flags;
    long	functions;
    long	decorations;
    int		input_mode;
} MotifWmHints;

typedef MotifWmHints	MwmHints;

/* bit definitions for MwmHints.flags */
#define MWM_HINTS_FUNCTIONS	(1L << 0)
#define MWM_HINTS_DECORATIONS	(1L << 1)
#define MWM_HINTS_INPUT_MODE	(1L << 2)

/* bit definitions for MwmHints.functions */
#define MWM_FUNC_ALL		(1L << 0)
#define MWM_FUNC_RESIZE		(1L << 1)
#define MWM_FUNC_MOVE		(1L << 2)
#define MWM_FUNC_MINIMIZE	(1L << 3)
#define MWM_FUNC_MAXIMIZE	(1L << 4)
#define MWM_FUNC_CLOSE		(1L << 5)

/* bit definitions for MwmHints.decorations */
#define MWM_DECOR_ALL		(1L << 0)
#define MWM_DECOR_BORDER	(1L << 1)
#define MWM_DECOR_RESIZEH	(1L << 2)
#define MWM_DECOR_TITLE		(1L << 3)
#define MWM_DECOR_MENU		(1L << 4)
#define MWM_DECOR_MINIMIZE	(1L << 5)
#define MWM_DECOR_MAXIMIZE	(1L << 6)

/* values for MwmHints.input_mode */
#define MWM_INPUT_MODELESS			0
#define MWM_INPUT_PRIMARY_APPLICATION_MODAL	1
#define MWM_INPUT_SYSTEM_MODAL			2
#define MWM_INPUT_FULL_APPLICATION_MODAL	3

/*
 * The following is for compatibility only. It use is deprecated.
 */
#define MWM_INPUT_APPLICATION_MODAL	MWM_INPUT_PRIMARY_APPLICATION_MODAL


/*
 * Contents of the _MWM_INFO property.
 */

typedef struct
{
    long	flags;
    Window	wm_window;
} MotifWmInfo;

typedef MotifWmInfo	MwmInfo;

/* bit definitions for MotifWmInfo .flags */
#define MWM_INFO_STARTUP_STANDARD	(1L << 0)
#define MWM_INFO_STARTUP_CUSTOM		(1L << 1)



/*
 * Definitions for the _MWM_HINTS property.
 */

typedef struct
{
    CARD32	flags;
    CARD32	functions;
    CARD32	decorations;
    INT32	inputMode;

} PropMotifWmHints;

typedef PropMotifWmHints	PropMwmHints;


/* number of elements of size 32 in _MWM_HINTS */
#define PROP_MOTIF_WM_HINTS_ELEMENTS	4
#define PROP_MWM_HINTS_ELEMENTS		PROP_MOTIF_WM_HINTS_ELEMENTS

/* atom name for _MWM_HINTS property */
#define _XA_MOTIF_WM_HINTS	"_MOTIF_WM_HINTS"
#define _XA_MWM_HINTS		_XA_MOTIF_WM_HINTS

/*
 * Definitions for the _MWM_MESSAGES property.
 */

#define _XA_MOTIF_WM_MESSAGES	"_MOTIF_WM_MESSAGES"
#define _XA_MWM_MESSAGES	_XA_MOTIF_WM_MESSAGES

/* atom that enables client frame offset messages */
#define _XA_MOTIF_WM_OFFSET	"_MOTIF_WM_OFFSET"

/*
 * Definitions for the _MWM_MENU property.
 */

/* atom name for _MWM_MENU property */
#define _XA_MOTIF_WM_MENU	"_MOTIF_WM_MENU"
#define _XA_MWM_MENU		_XA_MOTIF_WM_MENU


/*
 * Definitions for the _MWM_INFO property.
 */

typedef struct
{
    CARD32 flags;
    CARD32 wmWindow;
} PropMotifWmInfo;

typedef PropMotifWmInfo	PropMwmInfo;


/* number of elements of size 32 in _MWM_INFO */
#define PROP_MOTIF_WM_INFO_ELEMENTS	2
#define PROP_MWM_INFO_ELEMENTS		PROP_MOTIF_WM_INFO_ELEMENTS

/* atom name for _MWM_INFO property */
#define _XA_MOTIF_WM_INFO	"_MOTIF_WM_INFO"
#define _XA_MWM_INFO		_XA_MOTIF_WM_INFO


/*
 * Miscellaneous atom definitions
 */

/* atom for motif input bindings */
#define _XA_MOTIF_BINDINGS	"_MOTIF_BINDINGS"

#endif /* _MwmUtil_h */
