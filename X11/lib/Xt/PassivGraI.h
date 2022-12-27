/*
* $XConsortium: PassivGraI.h,v 1.4 89/12/16 19:23:09 rws Exp $
*/

/********************************************************

Copyright (c) 1988 by Hewlett-Packard Company
Copyright (c) 1987, 1988, 1989 by Digital Equipment Corporation, Maynard, 
              Massachusetts, and the Massachusetts Institute of Technology, 
              Cambridge, Massachusetts

Permission to use, copy, modify, and distribute this software 
and its documentation for any purpose and without fee is hereby 
granted, provided that the above copyright notice appear in all 
copies and that both that copyright notice and this permission 
notice appear in supporting documentation, and that the names of 
Hewlett-Packard, Digital or  M.I.T.  not be used in advertising or 
publicity pertaining to distribution of the software without specific, 
written prior permission.

DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

********************************************************/

#ifndef _PDI_h_
#define _PDI_h_


#define KEYBOARD TRUE
#define POINTER  FALSE

typedef enum {
    XtNoServerGrab, 
    XtPassiveServerGrab,
    XtActiveServerGrab,
    XtPseudoPassiveServerGrab,
    XtPseudoActiveServerGrab
}XtServerGrabType;

typedef struct _DetailRec {
    unsigned short 	exact;
    Mask  		*pMask;
} DetailRec;

typedef struct _XtServerGrabRec {
    struct _XtServerGrabRec 	*next;
    Widget			widget;
    Boolean	   		ownerEvents;
    int	     			pointerMode;
    int				keyboardMode;
    DetailRec 			modifiersDetail;
    Mask			eventMask;
    DetailRec			detail;		/* key or button */
    Window			confineTo;	/* always NULL for keyboards */
    Cursor			cursor;		/* always NULL for keyboards */
} XtServerGrabRec, *XtServerGrabPtr;

typedef struct _XtDeviceRec{
    XtServerGrabRec	grab; 	/* need copy in order to protect
				   during grab */
    XtServerGrabType	grabType;
}XtDeviceRec, *XtDevice;

typedef enum {
    XtMyAncestor,
    XtMyDescendant,
    XtMyCousin,
    XtMySelf,
    XtUnrelated
}XtGeneology;


typedef struct {
    Widget		focusKid;
    XtServerGrabPtr	keyList, ptrList;
    Widget		queryEventDescendant;
    Boolean		map_handler_added:1;
    Boolean		realize_handler_added:1;
    Boolean		active_handler_added:1;
    Boolean		haveFocus:1;
    XtGeneology		focalPoint;
}XtPerWidgetInputRec, *XtPerWidgetInput;

typedef struct XtPerDisplayInputRec{
    XtGrabList 	grabList;
    XtDeviceRec keyboard, pointer;
    KeyCode	activatingKey;
    Widget 	*trace;
    int		traceDepth, traceMax;
    Widget 	focusWidget;
}XtPerDisplayInputRec, *XtPerDisplayInput;

#define IsServerGrab(g) ((g == XtPassiveServerGrab) ||\
			 (g == XtActiveServerGrab))

#define IsAnyGrab(g) ((g == XtPassiveServerGrab) ||\
		      (g == XtActiveServerGrab)  ||\
		      (g == XtPseudoPassiveServerGrab))

#define IsEitherPassiveGrab(g) ((g == XtPassiveServerGrab) ||\
				(g == XtPseudoPassiveServerGrab))

#define IsPseudoGrab(g) ((g == XtPseudoPassiveServerGrab))

extern XtPerWidgetInput _XtGetPerWidgetInput();

extern XtPerDisplayInput _XtGetPerDisplayInput();

extern XtServerGrabPtr _XtCheckServerGrabsOnWidget();


#endif /* _PDI_h_ */
