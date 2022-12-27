#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)CutPaste.c	3.25.1.2 91/03/21";
#endif /* lint */
#endif /* REV_INFO */
/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
*  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
*  (c) Copyright 1987, 1988, 1989, 1990 HEWLETT-PACKARD COMPANY
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
#define CUTPASTE

#include <X11/Xlib.h>
#include <Xm/CutPasteP.h>
#include <Xm/AtomMgr.h>

#define XA_CLIPBOARD ( XmInternAtom( display, "CLIPBOARD", False ))
#define XA_CLIP_TEMP ( XmInternAtom( display, "CLIP_TEMPORARY", False ))
#define XA_CLIP_INCR ( XmInternAtom( display, "INCR", False ))
#define XA_TARGETS   ( XmInternAtom( display, "TARGETS", False ))
#define XA_LENGTH    ( XmInternAtom( display, "LENGTH", False ))
#define XA_TIMESTAMP ( XmInternAtom( display, "TIMESTAMP", False ))
#define XA_MULTIPLE ( XmInternAtom( display, "MULTIPLE", False ))

#define XMRETRY 3
#define XM_APPEND 0
#define XM_REPLACE 1
#define XM_PREPEND 2

#define XM_HEADER_ID 0
#define XM_NEXT_ID   1
#define XM_LOCK_ID   2

#define XM_FIRST_FREE_ID 1000	/* First Item Id allocated */
#define XM_ITEM_ID_INC 1000	/* Increase in Item Id between each copy */
#define XM_ITEM_ID_MAX 5000	/* 'Safe' threshold for resetting Item Id */

#define XM_FORMAT_HEADER_TYPE 1
#define XM_DATA_ITEM_RECORD_TYPE 2
#define XM_HEADER_RECORD_TYPE 3

#define XM_UNDELETE 0
#define XM_DELETE 1

#define XM_DATA_REQUEST_MESSAGE 0
#define XM_DATA_DELETE_MESSAGE  1

#define XM_CLIPBOARD_MESSAGE1 "Must call XmClipboardStartCopy() before XmClipboardCopy()"
#define XM_CLIPBOARD_MESSAGE2 "Must call XmClipboardStartCopy() before XmClipboardEndCopy()"
#define XM_CLIPBOARD_MESSAGE3 "Too many formats in XmClipboardCopy()"

#define XM_STRING	 8
#define XM_ATOM	32
#define XM_ATOM_PAIR	32 
#define XM_BITMAP	32
#define XM_PIXMAP	32
#define XM_DRAWABLE	32
#define XM_SPAN	32
#define XM_INTEGER	32
#define XM_WINDOW	32
#define XM_PIXEL	32
#define XM_COLORMAP	32
#define XM_TEXT	 8

#define MAX_SELECTION_INCR(dpy) (((65536 < dpy->max_request_size) ? \
      (65536 << 2)  : (dpy->max_request_size << 2))-100)

#define BYTELENGTH( length, format ) (( length )*(( format ) >> 3 ))

typedef long itemId;

typedef struct {
   Atom target;
   Atom property;
 } IndirectPair;

/*-----------------------------------------------------*/
/*   Define the clipboard selection information record */
/*-----------------------------------------------------*/

typedef struct _ClipboardSelectionInfo {

    Display *display;
    Window window;
    Window selection_window;
    Time time;
    char *format;
    unsigned long count;
    char *data;
    Atom type;
    Boolean incremental;
    Boolean selection_notify_received;

} ClipboardSelectionInfoRec, *ClipboardSelectionInfo;

/*------------------------------------------------------------*/
/*   Define the clipboard property destroy information record */
/*------------------------------------------------------------*/

typedef struct _ClipboardDestroyInfo {

    Display *display;
    Window  window;
    Atom property;

} ClipboardDestroyInfoRec, *ClipboardDestroyInfo;

/*-------------------------------------------------------*/
/*   Define the clipboard cut by name information record */
/*-------------------------------------------------------*/

typedef struct _ClipboardCutByNameInfo {

    Window window;
    itemId formatitemid;

} ClipboardCutByNameInfoRec, *ClipboardCutByNameInfo;

/*---------------------------------------------------*/
/*   Define the clipboard format registration record */
/*---------------------------------------------------*/

typedef struct _ClipboardFormatRegRec {

    int formatLength;

} ClipboardFormatRegRec, *ClipboardFormatRegPtr;

/*-------------------------------------------*/
/*   Define the clipboard lock record        */
/*-------------------------------------------*/

typedef struct _ClipboardLockRec {

    Window windowId;
    int lockLevel;

} ClipboardLockRec, *ClipboardLockPtr;

/*---------------------------------------------------*/
/*   Define the clipboard format item record         */
/*---------------------------------------------------*/

typedef struct _ClipboardFormatItemRec {

    int recordType;
    itemId parentItemId;  /* this is the data item that owns the format */
    Display *displayId;	/* display id of application owning data */
    int windowId;	/* window id for cut by name */
    Widget cutByNameWidget;    /* window id for cut by name */
    Window cutByNameWindow;    /* window id for cut by name */
    VoidProc cutByNameCallback;   /* address of callback routine for */
    					/* cut by name data */

    int itemLength;	/* length of this format item data */
    itemId formatDataId;	/* id for format item data, */
    			        /* 0 if passed by name */
    Atom formatNameAtom; /* format name atom */
    unsigned long formatNameLength;

    unsigned long cancelledFlag;  /* format was cancelled by poster */
    unsigned long cutByNameFlag;  /* data has not yet been provided */

    itemId thisFormatId;  /* id given application for identifying format item */
    itemId itemPrivateId; /* id provide by application for identifying item */

    unsigned long copiedLength;  /* amount already copied incrementally */

} ClipboardFormatItemRec, *ClipboardFormatItem;

/*-------------------------------------------------*/
/*   Define the clipboard data item record         */
/*-------------------------------------------------*/

typedef struct _ClipboardDataItemRec {

    int recordType;
    itemId adjunctData; /* for future compatibility */
    Display *displayId;	/* display id of application owning data */
    int windowId;	/* window id for cut by name */

    itemId thisItemId;  /* item id of this data item */

    unsigned long dataItemLabelId; /* id of label (comp) string */
    unsigned long formatIdList;	 /* offset of beginning of format id list */
    int formatCount;		/* number of formats stored for this item */
    int cancelledFormatCount;	/* number of cut by name formats cancelled */

    unsigned long cutByNameFlag;  /* data has not yet been provided */
    unsigned long deletePendingFlag;	/* is item marked for deletion? */
    unsigned long permanentItemFlag;	/* is item permanent or temporary? */

    VoidProc cutByNameCallback;   /* address of callback routine for */
    					/* cut by name data */
    Widget cutByNameWidget;   /* widget receiving messages concerning */
    				      /* cut by name */
    Window cutByNameWindow;

} ClipboardDataItemRec, *ClipboardDataItem;

/*----------------------------------------------*/
/*   Define the clipboard header record         */
/*----------------------------------------------*/

typedef struct _ClipboardHeaderRec {

    int recordType;
    itemId adjunctHeader;	/* for future compatibility */
    unsigned long maxItems;	/* maximum number of clipboard items */
    				/* including those marked for delete */
    unsigned long dataItemList;	    /* offset of data item id list */

    itemId nextPasteItemId;    	/* data id of next item id to paste */
    itemId oldNextPasteItemId; 	/* data id of old next paste item id */
    itemId deletedByCopyId;	/* item marked deleted by last copy, if any */
    itemId lastCopyItemId;  /* item id of last item put on clipboard */
    itemId recopyId;        /* item id of item requested for recopy */
      
    unsigned long currItems;   /* current number of clipboard items */
    			       /* including those marked for delete */
    Time   selectionTimestamp; /* for ICCCM clipboard selection compatability */
    Time   copyFromTimestamp; /* time of event causing inquire or copy from */
    int  foreignCopiedLength; /* amount copied so far in incr copy from */ 
			      /* selection */
    Window ownSelection;
    unsigned long incrementalCopyFrom;  /* requested in increments */

    unsigned long startCopyCalled;  /* to ensure that start copy is called
					before copy or endcopy */

} ClipboardHeaderRec, *ClipboardHeader;

/*---------------------------------------------*/

typedef union {
    ClipboardFormatItemRec format;
    ClipboardDataItemRec   item;
    ClipboardHeaderRec     header;
} ClipboardUnionRec, *ClipboardPointer;

/*---------------------------------------------*/
     
/*
 * forward declarations
 */

/* internal routines */

static Boolean _XmWeOwnSelection();
static void _XmAssertClipboardSelection();
static Boolean _XmGetConversion();
static int _XmGetWindowProperty();
static void _XmHandleSelectionEvents();
static Boolean _XmSelectionRequestHandler();
static int _XmClipboardSelectionIsReady();
static int _XmClipboardRequestorIsReady();
static Boolean _XmWaitForPropertyDelete();
static Window _XmInitializeSelection();
static int _XmRegIfMatch();
static int _XmRegisterFormat();
static int _XmClipboardGetLenFromFormat();
static Atom _XmClipboardGetAtomFromFormat();
static void _XmClipboardError();
static int  _XmClipboardDataIsReady();
static int  _XmClipboardEventHandler();
static ClipboardFormatItem _XmClipboardFindFormat();
static void _XmClipboardDeleteFormat();
static void _XmClipboardDeleteFormats();
static void _XmClipboardDeleteItemLabel();
static char *_XmClipboardAlloc();
static void _XmClipboardFreeAlloc();
static ClipboardHeader _XmClipboardOpen();
static void _XmClipboardClose();
static void _XmClipboardMarkItem();
static void _XmClipboardDeleteMarked();
static unsigned long _XmClipboardIsMarkedForDelete();
static void _XmClipboardDeleteItem();
static int  _XmClipboardFindItem();
static int  _XmClipboardRetrieveItem();
static void _XmClipboardReplaceItem();
static Atom _XmClipboardGetAtomFromId();
static itemId _XmClipboardGetNewItemId();
static int  _XmClipboardLock();
static int  _XmClipboardUnlock();
static int  _XmClipboardRequestDataAndWait();
static int  _XmClipboardSendMessage();
static int  _XmClipboardWindowExists();
static int  _XmClipboardSearchForWindow();
static char* AddAddresses();
static Time _XmClipboardGetCurrentTime();
static void _XmClipboardSetNextItemId();

/*---------------------------------------------*/
/* internal routines			       */
/*---------------------------------------------*/
static char* AddAddresses( base, offset )
char* base;
int   offset;
{
    char* ptr;

    ptr = (char*)(base);

    ptr = ptr + offset;

    return ptr;
}

static Time _XmClipboardGetCurrentTime(dpy)
    Display *dpy;
{
    XEvent event;

    XSelectInput(dpy,DefaultRootWindow(dpy),PropertyChangeMask);
    XChangeProperty(dpy,DefaultRootWindow(dpy),
                  XmInternAtom(dpy,"_MOTIF_CLIP_TIME",False),
                  XmInternAtom(dpy,"_MOTIF_CLIP_TIME",False),
                  8,PropModeAppend,NULL,0);
    XWindowEvent(dpy,DefaultRootWindow(dpy),PropertyChangeMask,&event);
    return(event.xproperty.time);
}
static void _XmClipboardSetNextItemId(display, itemid)
    Display *display;
    long itemid;
{
    int base;
    int next;
    int *int_ptr;
    int length;

    base = itemid - (itemid % XM_ITEM_ID_INC);
    if (base >= XM_ITEM_ID_MAX) {
	next = XM_FIRST_FREE_ID;
    } else {
	next = base + XM_ITEM_ID_INC;
    }

    _XmClipboardFindItem( display,
			XM_NEXT_ID,
			&int_ptr,
			&length,
			0,
			0 );
    *int_ptr = next;
    length = sizeof( int );
    _XmClipboardReplaceItem( display,
			XM_NEXT_ID,
			int_ptr,
			length,
			PropModeReplace,
			32,
			True );
}


static Boolean _XmWeOwnSelection( display, header )
Display *display;
ClipboardHeader header;
{
    Window selectionwindow;

    selectionwindow = XGetSelectionOwner (display, XA_CLIPBOARD);

    return (selectionwindow == header->ownSelection);
}

static void _XmAssertClipboardSelection( display, window, header, time )
Display *display;
Window window;
ClipboardHeader header;
Time time;
{
    Widget widget;

    header->ownSelection = None;
    header->selectionTimestamp = 0;

    widget = XtWindowToWidget (display, window);

    /* Need a valid Widget to add an event handler. */

    if (widget == NULL)
    {
	return;
    }

    /* Assert ownership of CLIPBOARD selection only if there is valid data. */

    if (header->nextPasteItemId == 0)
    {
	return;
    }

    header->ownSelection = window;
    header->selectionTimestamp = time;

    XSetSelectionOwner (display, XA_CLIPBOARD, window, time);
    XtAddEventHandler (widget, (EventMask)NULL, TRUE, _XmHandleSelectionEvents,
			NULL);
    return;
}



static Boolean _XmWaitForPropertyDelete( display, window, property )
Display *display;
Window window;
Atom property;
{
    XEvent event_return;
    ClipboardDestroyInfoRec info;

    info.window = window;
    info.property = property;


    /*
     * look for match event and discard all prior configures
     */
    if (XCheckIfEvent( display, &event_return,
                       _XmClipboardRequestorIsReady, (char *)&info )) {
	if ( info.window == 0 )
	    return FALSE;
	else
	    return TRUE;
    }

    XIfEvent( display, &event_return, _XmClipboardRequestorIsReady,
					(char *)&info );
    if ( info.window == 0 )
	return FALSE;
    else
	return TRUE;
}


static Boolean _XmGetConversion( selection, target, property, widget,
				 window, incremental, multiple, abort, ev )
Atom selection;
Atom target;
Atom property;
Widget widget;
Window window;
Boolean *incremental;
Boolean multiple;
Boolean *abort;
XEvent *ev;
{
    XWindowAttributes window_attributes;
    unsigned char *value;
    Atom loc_target;
    int length, length_sent, format, next_length;
    Display *display;
    XSelectionEvent *event;

    event = (XSelectionEvent*)ev;

    display = XtDisplay( widget );

    *abort = FALSE;

    /* get the data from the clipboard, regardless of how long it is
    */
    if ( _XmSelectionRequestHandler( widget,
                                      target,
                                      &value,
                                      &length,
                                      &format ) == FALSE )
    {
        return FALSE;
    }

    if ( length > MAX_SELECTION_INCR( display ) )
    {
        /* have to do an incremental
        */
        *incremental = TRUE;

        /* if this is a multiple request then don't actually send data now
        */
        if ( multiple ) return TRUE;

        /* get the current window event mask
        */
        XGetWindowAttributes( display, window, &window_attributes );

        /* select for property notify as well as current mask
        */
        XSelectInput( display,
                      window,
                      PropertyChangeMask  |
                      StructureNotifyMask | window_attributes.your_event_mask );

        loc_target = XA_CLIP_INCR;

        /* inform the requestor
        */
        XChangeProperty( display,
                         window,
                         property,
                         loc_target,
                         32,
                         PropModeReplace,
                         (unsigned char*)&length,
                         1 );

        (void) XSendEvent( display,
                           event->requestor,
                           False,
                           (unsigned long)NULL,
                           ev );

        /* now wait for the property delete
        */
        if (!_XmWaitForPropertyDelete( display, window, property))
        {
            /* this means the window got destroyed before the property was */
            /* deleted
            */
            _XmClipboardFreeAlloc((char *)value);
	    *abort = TRUE;
            return FALSE;
        }

        next_length = MAX_SELECTION_INCR( display );

        /* send the data in increments
        */
        for( length_sent = 0;
             next_length > 0;
             length_sent = length_sent + next_length )
        {
            next_length = MIN( length - length_sent,
                               next_length );

            /* last property changed will be length 0
            */
            XChangeProperty( display,
                             window,
                             property,
                             target,
                             format,
                             PropModeReplace,
                             value + length_sent,
                             (next_length * 8) / format );

            /* wait for the property delete, unless it has length zero
            */
            if ( next_length > 0 )
            {
                if ( !_XmWaitForPropertyDelete( display, window, property ) )
                {
                   /* this means the window got destroyed before the property */
                   /* was deleted
                   */
                   _XmClipboardFreeAlloc((char *)value);
                   *abort = TRUE;
                   return FALSE;
                }
            }
        }

        /* put mask back the way it was
        */
        XSelectInput( display,
                      window,
                      window_attributes.your_event_mask );

    }else{
        /* not incremental
        */
        *incremental = FALSE;

        /* write the data to the property
        */
        XChangeProperty( display,
                         window,
                         property,
                         target,
                         format,
                         PropModeReplace,
                         value,
                         (length * 8) / format );

    }

    _XmClipboardFreeAlloc((char *)value);

    return TRUE;
}


static void _XmHandleSelectionEvents( widget, closure, event )
Widget widget;
Opaque closure;
XEvent *event;
{
    XSelectionEvent ev;
    Boolean incremental;
    unsigned long bytesafter, length;
    IndirectPair *p;
    char *value;
    int format;
    Atom target;
    int count;
    Boolean writeback;

    switch( event->type )
    {
        case SelectionClear:
	    if (event->xselectionclear.selection != XmInternAtom(
		event->xselectionclear.display, "CLIPBOARD", False))
		    	return;
            break;

        case SelectionRequest:
        {
            Display *display;
            Window window;
            ClipboardHeader header;
            Time data_time;
            Boolean notify_sent = FALSE;
	    Boolean abort = FALSE;

	    if (event->xselectionrequest.selection != XmInternAtom(        
	    	event->xselectionrequest.display, "CLIPBOARD", False))
			return;


	    writeback = FALSE;

            display = XtDisplay( widget );
            window  = XtWindow(  widget );

            data_time = CurrentTime;

            if ( _XmClipboardLock( display, window ) == ClipboardSuccess )
            {
                /* get the clipboard header
                */
                header = _XmClipboardOpen( display, 0 );

                data_time = header->selectionTimestamp;

                _XmClipboardClose( display, header );
                _XmClipboardUnlock( display, window, False );
            }

            ev.type = SelectionNotify;
            ev.display = event->xselectionrequest.display;
            ev.requestor = event->xselectionrequest.requestor;
            ev.selection = event->xselectionrequest.selection;
            ev.time = event->xselectionrequest.time;
            ev.target = event->xselectionrequest.target;
            /* if obsolete requestor, then use the target as property */
            if( event->xselectionrequest.property == None )
                event->xselectionrequest.property =
					event->xselectionrequest.target;
            ev.property = event->xselectionrequest.property;

            if (( event->xselectionrequest.time != CurrentTime ) &&
                ( event->xselectionrequest.time < data_time ))
            {
                ev.property = None;
            }else{
                if ( ev.target == XA_MULTIPLE )
                {
                    (void) XGetWindowProperty(
                                event->xselectionrequest.display,
                                event->xselectionrequest.requestor,
                                event->xselectionrequest.property,
                                0L,
                                1000000,
                                False,
                                AnyPropertyType,
                                &target,
                                &format,
                                &length,
                                &bytesafter,
                                (unsigned char **)&value );

                    count = BYTELENGTH( length, format )
                                / sizeof( IndirectPair );

                    for ( p = (IndirectPair *)value; count; p++, count-- )
                    {
                        if ( _XmGetConversion( ev.selection,
                                            p->target,
                                            p->property,
                                            widget,
                                            event->xselectionrequest.requestor,
                                            &incremental,
                                            TRUE,
					    &abort,
                                            (XEvent *) &ev ))
                        {
                            if ( incremental )
                            {
                                p->target = XA_CLIP_INCR;
                                writeback = TRUE;
                            }
                        }else{
                            p->property = None;
                            writeback = TRUE;
                        }
                    }

                    if ( writeback )
                    {
                        XChangeProperty( ev.display,
                                         ev.requestor,
                                         event->xselectionrequest.property,
                                         target,
                                         format,
                                         PropModeReplace,
                                         (unsigned char *) value,
                                         (int) length);
                    }
		    if (value != NULL)
                    	XFree(value);
                 }else{
                    /* not multiple
                    */
                    if ( _XmGetConversion( ev.selection,
                                        event->xselectionrequest.target,
                                        event->xselectionrequest.property,
                                        widget,
                                        event->xselectionrequest.requestor,
                                        &incremental,
                                        FALSE,
					&abort,
                                        (XEvent *) &ev ))
                    {
                        if ( incremental )
                            notify_sent = TRUE;
                    }else{
                        ev.property = None;
                    }
                }
            } /* end time okay */

            if ( !notify_sent  && !abort )
            {
               (void) XSendEvent( display,
                                  ev.requestor,
                                  False,
                                  (unsigned long)NULL,
                                  (XEvent *) &ev );
            }
            break;

        } /* end selection request */

    } /* end switch on event type */
}

static Boolean _XmSelectionRequestHandler(  widget,
					    target,
					    value,
					    length,
					    format )
Widget widget;
Atom   target;
char **value;
unsigned long *length; /* returns length in bytes   */
int    *format;        /* returns format, 8, 16, 32 */
{
    Display *display;
    Window window;
    ClipboardHeader header;
    Boolean ret_value;
    int i;

    display = XtDisplay( widget );
    window  = XtWindow(  widget );

    if ( _XmClipboardLock( display, window ) != ClipboardSuccess )
	    return False;

    ret_value = True;

    /* get the clipboard header
    */
    header = _XmClipboardOpen( display, 0 );

    /* fake loop
    */
    for( i = 0; i < 1; i++ )
    {
	
    if ( !_XmWeOwnSelection( display, header ) ) 
    {
	/* we don't own the selection, something's wrong
	*/
	ret_value = False;
	break;
    }

    if ( target == XA_TARGETS )
    {
	Atom *ptr, *save_ptr;
	ClipboardFormatItem nextitem;
	int n, count;
	int dummy;
	int bytes_per_target;

	*length = 0;

	*format = XM_ATOM;  /* i.e. 32 */

	bytes_per_target = XM_ATOM / 8;

	n = 1;

	/* find the first format for the next paste item, if any remain
	*/
	nextitem = _XmClipboardFindFormat( display, 
					    header, 
					    0, 
					    NULL, 
					    n,
					    &dummy, 
					    &count,
					    &dummy );

	/* allocate storage for list of target atoms 
	*/
	ptr = (Atom *)_XmClipboardAlloc( count * bytes_per_target );
	save_ptr = ptr;

        while ( nextitem != NULL )
	{
	    /* add format to list 
	    */
	    *ptr = nextitem->formatNameAtom;

	    n = n + 1;

	    _XmClipboardFreeAlloc( nextitem );

	    /* find the nth format for the next paste item
	    */
	    nextitem = _XmClipboardFindFormat( display, 
						header, 
						0, 
						NULL, 
						n,
						&dummy, 
						&count,
						&dummy );
	    if (nextitem != NULL) ptr++;
		
	}
	*value = (char *) save_ptr;

	/* n - 1 is number of targets, length is 4 times that
	*/
	*length = (n - 1) * bytes_per_target;

    }else{
    if ( target == XA_TIMESTAMP )
    {
	Time *timestamp;

	timestamp = (Time *)_XmClipboardAlloc(sizeof(Time));
        *timestamp  = header->selectionTimestamp;

	*value = (char *) timestamp;

	*length = sizeof(Time);

	*format = XM_INTEGER;

    }else{
	/* some named format */

	char *format_name;
	int private_id;
	unsigned long outlength;

	/* convert atom to format name
	*/
	format_name = XmGetAtomName( display, target );

        _XmClipboardGetLenFromFormat( display, format_name, format );

	if (XmClipboardInquireLength( display, window, format_name, length )
	     != ClipboardSuccess)
	{
	    ret_value = False;
	    break;
	}

	if ( *length == 0 )
	{
	    ret_value = False;
	    break;
	}

	*value = _XmClipboardAlloc( *length );

        /* do incremental copy if it's too long
        */

	if ( XmClipboardRetrieve( display, 
				   window, 
				   format_name, 
				   *value, 
				   *length, 
				   &outlength, 
				   &private_id ) != ClipboardSuccess )
	{
	    ret_value = False;
	    break;
	}
    }
    } 
    } /* end fake loop */

    _XmClipboardClose( display, header );
    _XmClipboardUnlock( display, window, False );

    return ret_value;
}


static Window _XmInitializeSelection( display, header, window, time )
Display *display;
ClipboardHeader header;
Window window;
Time time;
{
    Window selectionwindow;    

    /* If there is no CLIPBOARD owner, and we have clipboard
     * data, then assert ownership, and use that data.
     */

    selectionwindow = XGetSelectionOwner (display, XA_CLIPBOARD);


    /* if the header is corrupted, give up. */

    if (selectionwindow == window && header->ownSelection == None)
    {
	selectionwindow = None;
	XSetSelectionOwner (display, XA_CLIPBOARD, None, time);
    }

    if (selectionwindow != None) /* someone owns CLIPBOARD already */
    {
	return selectionwindow;
    }

    /* assert ownership of the clipboard selection */

    _XmAssertClipboardSelection (display, window, header, time);

    selectionwindow = XGetSelectionOwner (display, XA_CLIPBOARD);

    return (selectionwindow);
}


static int _XmRegIfMatch( display, format_name, match_name, format_length )
Display		*display; /* Display id of application passing data */
char		*format_name;  /* Name string for data format */
char		*match_name;  /* Name string for predefined data format */
unsigned long	format_length; /* Format length  8-16-32 */
{
	if ( strcmp( format_name, match_name ) == 0 )
	{
	    _XmRegisterFormat( display, format_name, format_length );
	    return 1;
	}
    return 0;
}

static int _XmRegisterFormat( display, format_name, format_length )
Display		*display; /* Display id of application passing data */
char		*format_name;  /* Name string for data format */
unsigned long	format_length; /* Format length  8-16-32 */
{
    Window rootwindow;
    Atom formatatom;
    int stored_len;

    /* get the atom for the format_name
    */
    formatatom = _XmClipboardGetAtomFromFormat( display, 
    					         format_name );

    rootwindow = XDefaultRootWindow( display );

    if ( _XmClipboardGetLenFromFormat( display, 
				        format_name,
					&stored_len ) == ClipboardSuccess )
    {
	if ( stored_len == format_length )
	    return ClipboardSuccess;

	/* it is already registered, don't allow override
	*/
	return ClipboardFail;
    }

    XChangeProperty( display, 
		     rootwindow, 
		     formatatom,
		     formatatom,
		     32,
		     PropModeReplace,
		     (unsigned char*)&format_length,
		     1 );

    return ClipboardSuccess;
}

/*---------------------------------------------*/
static void _XmClipboardError( key, message )
char *key;
char *message;
{
    XMERROR(key, message );
}

/*---------------------------------------------*/
static int _XmClipboardEventHandler( widget, 
    				      closure,
    				      event )
Widget widget;
caddr_t closure;
XEvent *event;
{
    XClientMessageEvent *event_rcvd;
    Display *display;
    itemId formatitemid;
    ClipboardFormatItem formatitem; 
    int formatlength;
    int privateitemid;
    VoidProc callbackroutine;
    int reason, ret_value;

    event_rcvd = (XClientMessageEvent*)event;

    if ( (event_rcvd->type & 127) != ClientMessage )
    	return 0;

    display = event_rcvd->display;

    if ( event_rcvd->message_type != XmInternAtom( display, 
    					    "_MOTIF_CLIP_MESSAGE", False ) )
    	return 0;

    formatitemid  = event_rcvd->data.l[1];
    privateitemid = event_rcvd->data.l[2];

    /* get the callback routine
    */
    ret_value = _XmClipboardFindItem( display, 
    				       formatitemid,
    			   	       &formatitem,
    			   	       &formatlength,
				       0,
    			   	       XM_FORMAT_HEADER_TYPE );

    if ( ret_value != ClipboardSuccess ) 
    	return 0;

    callbackroutine = formatitem->cutByNameCallback;

    _XmClipboardFreeAlloc( formatitem );

    if ( callbackroutine == 0 )
    	return 0;

    reason = 0;

    if ( event_rcvd->data.l[0] == XmInternAtom( display, 
    	    			       	       "_MOTIF_CLIP_DATA_REQUEST", 
   	  					False ) )
    	reason = XmCR_CLIPBOARD_DATA_REQUEST;

    if ( event_rcvd->data.l[0] == XmInternAtom( display, 
    	    			    	        "_MOTIF_CLIP_DATA_DELETE", 
   	  					False ) )
    	reason = XmCR_CLIPBOARD_DATA_DELETE;

    if ( reason == 0 )
    	return 0;

    /* call the callback routine
    */
    (*callbackroutine)( widget, 
    		        &formatitemid,
    		        &privateitemid,
    			&reason );

    return 1;
}

/*---------------------------------------------*/
static int _XmClipboardFindItem( display, 
    				  itemid,
    				  outpointer,
    				  outlength,
				  format,
    				  rec_type )
Display *display;
itemId itemid;
char **outpointer;
int *outlength;
int *format;
int rec_type;
{

    Window rootwindow;
    int ret_value;
    Atom itematom;
    ClipboardPointer ptr;

    rootwindow = XDefaultRootWindow( display );

    /* convert the id into an atom
    */
    itematom = _XmClipboardGetAtomFromId( display, itemid );

    ret_value = _XmGetWindowProperty( display,
                                       rootwindow,
                                       itematom,
                                       outpointer,
                                       outlength,
                                       0,
                                       format,
                                       FALSE );

    if ( ret_value != ClipboardSuccess ) return ret_value;

    ptr = (ClipboardPointer)(*outpointer);

    if ( rec_type != 0 && ptr->header.recordType != rec_type )
    {
    	_XmClipboardFreeAlloc( *outpointer );
    	_XmClipboardError( "ClipboardBadDataType", "bad data type" );
    	return ClipboardFail;
    }

    return ClipboardSuccess;
}


/*---------------------------------------------*/
static int _XmGetWindowProperty( display,
                                  window,
                                  property_atom,
                                  outpointer,
                                  outlength,
                                  type,
                                  format,
                                  delete )
Display *display;
Window window;
Atom property_atom;
int *outlength;
char **outpointer;
int *type;
int *format;
Boolean delete;
{

    int ret_value;
    Atom loc_type;
    unsigned long bytes_left;
    int cur_length;
    unsigned char *loc_pointer;
    unsigned long this_length;
    char *cur_pointer;
    int loc_format;
    unsigned long request_size;


    bytes_left = 1;
    cur_length = 0;

    *outpointer = 0;
    *outlength = 0;

    request_size = MAX_SELECTION_INCR( display );

    while ( bytes_left > 0 )
    {
	/* retrieve the item from the root
	*/
	ret_value = XGetWindowProperty( display, 
					window, 
					property_atom,
					(long)cur_length/4, /*offset*/
					(long)request_size, /*length*/
					FALSE,
					AnyPropertyType,
					&loc_type,
					&loc_format,			    
					&this_length,
					&bytes_left,
					&loc_pointer );

        if ( ret_value != 0 )
            return ClipboardFail;

        if ( loc_pointer == 0 || this_length == 0 )
        {
            if ( delete )
            {
                XDeleteProperty( display, window, property_atom );
	    }
            return ClipboardFail;
	}

        /* convert length according to format
        */
        this_length = BYTELENGTH( this_length, loc_format );

	if ( cur_length == 0 )
	{
	    cur_pointer = _XmClipboardAlloc( (int)this_length + bytes_left );

	    *outpointer = cur_pointer;
	}

	bcopy( loc_pointer, cur_pointer, this_length );

	cur_pointer = cur_pointer + this_length;
	cur_length  = cur_length  + this_length;

	if (loc_pointer != NULL)
		XFree((char*)loc_pointer);
    }

    if ( delete )
    {
        XDeleteProperty( display, window, property_atom );
    }

    if ( format != 0 ) 
    {
	*format = loc_format;
    }

    if ( type != 0 )
    {
        *type = loc_type;
    }

    *outlength = cur_length;

    return ClipboardSuccess;
}

/*---------------------------------------------*/
static int _XmClipboardRetrieveItem( display, 
    				      itemid,
    				      add_length,  /* allocate this add'l */
    				      def_length,  /* if item non-exist */
    				      outpointer,
    				      outlength,
				      format,
    				      rec_type,
				      discard )   /* ignore old data */
Display *display;
itemId itemid;
int add_length;
int def_length;
int *outlength;
char **outpointer;
int *format;
int rec_type;
unsigned long discard;
{

    int ret_value;
    int loc_format;
    int loclength;
    ClipboardPointer clipboard_pointer;
    char *pointer;

     /* retrieve the item from the root
    */
    ret_value = _XmClipboardFindItem( display, 
    				       itemid,
    				       &pointer,
    				       &loclength,
				       &loc_format,
    				       rec_type );

    if (loclength == 0 || ret_value != ClipboardSuccess)
    {
    	*outlength = def_length;
    }else{    
	if ( discard == 1 ) loclength = 0;

	*outlength = loclength + add_length;
    }

    /* get local memory for the item
    */
    clipboard_pointer = (ClipboardPointer)_XmClipboardAlloc( *outlength );

    if (ret_value == ClipboardSuccess)
    {
	/* copy the item into the local memory
	*/
	bcopy( pointer, clipboard_pointer, loclength );
    }

    *outpointer = (char*)clipboard_pointer;

    /* free memory pointed to by pointer 
    */
    _XmClipboardFreeAlloc( pointer );

    if ( format != 0 )
    {
	*format = loc_format;
    }

    /* return a pointer to the item
    */
    return ret_value;

}

/*---------------------------------------------*/
static void _XmClipboardReplaceItem( display, 
    				      itemid,
    				      pointer,
    				      length,
    				      mode,
				      format,
    				      free_flag )
Display *display;
itemId itemid;
char *pointer;
int length;
int mode;
int format;  /* number of bits: 8, 16, 32 */
unsigned long free_flag;
{
    Window rootwindow;
    Atom itematom;
    char *loc_pointer;
    int loc_length;
    int loc_mode;
    unsigned int max_req_size;

    loc_pointer = pointer;
    loc_length  = length;
    loc_mode = mode;

    rootwindow = XDefaultRootWindow( display );

    /* convert the id into an atom
    */
    itematom = _XmClipboardGetAtomFromId( display, itemid );

    /* lengths are passed in bytes, but need to specify in format units */
    /* for ChangeProperty
    */
    loc_length = ( length * 8 ) / format;

    max_req_size = ( MAX_SELECTION_INCR( display ) ) * 8 / format;

    do
    {
	int next_length;

	if ( loc_length > max_req_size )
	{
	    next_length = max_req_size;
	}else{
	    next_length = loc_length;
	}
	
	/* put the new values in the root 
	*/
	XChangeProperty( display, 
			 rootwindow, 
			 itematom,
			 itematom,
			 format,
			 loc_mode,
			 (unsigned char*)loc_pointer,
			 next_length );

	loc_mode = PropModeAppend;
	loc_length = loc_length - next_length;
	loc_pointer = loc_pointer + next_length;
    }
    while( loc_length > 0 );

    if ( free_flag == True )
    {
	/* note:  you have to depend on the free flag, even if the length */
	/* is zero, the pointer may point to non-zero-length allocated data 
	*/
        _XmClipboardFreeAlloc( pointer );
    }
}

/*---------------------------------------------*/
static Atom _XmClipboardGetAtomFromId( display, 
    					itemid )
Display *display;
itemId itemid;
{
    char *item;
    char atomname[ 100 ];

    switch ( (int)itemid )
    {	
	case 0:	item = "_MOTIF_CLIP_HEADER";
	    break;
	case 1: item = "_MOTIF_CLIP_NEXT_ID";
	    break;
	default:
    	    sprintf( atomname, "_MOTIF_CLIP_ITEM_%d", itemid );
    	    item = atomname;
	    break;	
    }

    return XmInternAtom( display, item, False );
}
/*---------------------------------------------*/
static Atom _XmClipboardGetAtomFromFormat( display, 
    					    format_name )
Display *display;
char *format_name;
{
    char *item;
    Atom ret_value;

    item = _XmClipboardAlloc( strlen( format_name ) + 20 );

    sprintf( item, "_MOTIF_CLIP_FORMAT_%s", format_name );

    ret_value = XmInternAtom( display, item, False );

    _XmClipboardFreeAlloc( item );

    return ret_value;
}
/*---------------------------------------------*/
static int _XmClipboardGetLenFromFormat( display, 
    					  format_name,
					  format_length )
Display *display;
char *format_name;
int  *format_length;
{
    Atom format_atom;
    int ret_value;
    Window rootwindow;
    unsigned long outlength;
    unsigned char* outpointer;
    Atom type;
    int format;
    unsigned long bytes_left;

    format_atom = _XmClipboardGetAtomFromFormat( display, format_name );

    rootwindow = XDefaultRootWindow( display );

    /* get the format record 
    */
    ret_value = XGetWindowProperty( display, 
				    rootwindow, 
				    format_atom,
				    0, /*offset*/
				    10000000, /*length*/
				    False,
				    AnyPropertyType,
				    &type,
				    &format,			    
				    &outlength,
				    &bytes_left,
				    &outpointer );

    if ( outpointer == 0 || outlength == 0 || ret_value !=0 )
    {
	/* if not successful, return warning that format is not registered
	*/
	ret_value = ClipboardFail;

	*format_length = 8;

    }else{
	ret_value = ClipboardSuccess;

	/* return the length of the format
	*/
	*format_length = *((int *)outpointer);
    }

    if (outpointer != NULL)
    	XFree((char*)outpointer);
    return ret_value;
}

/*---------------------------------------------*/
static char *_XmClipboardAlloc( size )
int size;
{
        return XtMalloc( size );
}
/*---------------------------------------------*/
static void _XmClipboardFreeAlloc( addr )
char *addr;
{
     if ( addr != 0 )*addr = (char)255;
        XtFree( addr );
}

/*---------------------------------------------*/
static ClipboardHeader _XmClipboardOpen( display, add_length )
Display *display;
int add_length;
{
    int ret_value;
    int headerlength;
    ClipboardHeader root_clipboard_header;
    int number, length;
    int *int_ptr;

    ret_value = ClipboardSuccess;

    if ( add_length == 0 )
    {
	/* get the clipboard header
	*/
	ret_value = _XmClipboardFindItem( display, 
					   XM_HEADER_ID,
					   &root_clipboard_header,
					   &headerlength,
					   0,
    					   0 );
    }

    if ( add_length != 0 || ret_value != ClipboardSuccess )
    {
	/* get the clipboard header (this will allocate memory if doesn't exist)
	*/
	ret_value = _XmClipboardRetrieveItem( display, 
					       XM_HEADER_ID,
					       add_length,
					       sizeof( ClipboardHeaderRec ),
					       &root_clipboard_header,
					       &headerlength,
					       0,
    					       0,
					       0 ); /* don't discard old data */
    }

    /* means clipboard header had not been initialized
    */
    if ( ret_value != ClipboardSuccess ) 
    {
    	root_clipboard_header->recordType = XM_HEADER_RECORD_TYPE;
    	root_clipboard_header->adjunctHeader = 0;
    	root_clipboard_header->maxItems = 1;
    	root_clipboard_header->currItems = 0;
    	root_clipboard_header->dataItemList = sizeof( ClipboardHeaderRec );
    	root_clipboard_header->nextPasteItemId = 0;
    	root_clipboard_header->lastCopyItemId = 0;
    	root_clipboard_header->recopyId = 0;
    	root_clipboard_header->oldNextPasteItemId = 0;
    	root_clipboard_header->deletedByCopyId = 0;
	root_clipboard_header->ownSelection = 0;
	root_clipboard_header->selectionTimestamp = CurrentTime;
	root_clipboard_header->copyFromTimestamp  = CurrentTime;
    	root_clipboard_header->foreignCopiedLength = 0;
    	root_clipboard_header->incrementalCopyFrom = 0;
	root_clipboard_header->startCopyCalled = (unsigned long) False;
    }

    /* make sure "next free id" property has been initialized
    */
    ret_value = _XmClipboardFindItem( display, 
				       XM_NEXT_ID,
				       &int_ptr,
				       &length,
    				       0,
    				       0 );

    if ( ret_value != ClipboardSuccess ) 
    {
	number = XM_FIRST_FREE_ID;
    	int_ptr = &number;
    	length = sizeof( int );
    	
    	/* initialize the next id property
    	*/
	_XmClipboardReplaceItem( display,
				  XM_NEXT_ID,
				  int_ptr,
				  length,
    				  PropModeReplace,
				  32,
    				  False );
    }
    else
    {
    	_XmClipboardFreeAlloc( (char*)int_ptr );
    }

    return root_clipboard_header;
}

/*---------------------------------------------*/
static void _XmClipboardClose( display, root_clipboard_header )
Display *display;
ClipboardHeader root_clipboard_header;
{
    int headerlength;

    headerlength = sizeof( ClipboardHeaderRec ) + 
    		(root_clipboard_header -> currItems) * sizeof( itemId );

    /* replace the clipboard header
    */
    _XmClipboardReplaceItem( display, 
    			      XM_HEADER_ID,
    			      root_clipboard_header,
    			      headerlength,
    			      PropModeReplace,
			      32,
    			      True );
}

/*---------------------------------------------*/
static void _XmClipboardDeleteId( display, 
    				   itemid )
Display *display;
itemId itemid;
{
    Window rootwindow;
    Atom itematom;

    rootwindow = XDefaultRootWindow( display );

    itematom = _XmClipboardGetAtomFromId( display, itemid ); 

    XDeleteProperty( display, rootwindow, itematom );

}


/*---------------------------------------------*/
static ClipboardFormatItem _XmClipboardFindFormat( display, header, format, 
    						    itemid, n,
    						    maxnamelength, count,
    						    matchlength )
Display		    *display;
ClipboardHeader	    header;    /* Display id of application wanting data */
char 		    *format;
itemId   	    itemid;
int		    n;			/* if looking for nth format */
int		    *maxnamelength;	/* receives max format name length */
int		    *count;		/* receives next paste format count */
int		    *matchlength;	/* receives length of matching format */
{
    ClipboardDataItem queryitem;
    ClipboardFormatItem currformat, matchformat;
    int reclength, i, free_flag, index;
    itemId currformatid, queryitemid, *idptr;
    Atom formatatom;

    *count = 0;
    *maxnamelength = 0;

    if ( itemid < 0 ) return 0;

    /* if passed an item id then use that, otherwise use next paste item
    */
    if ( itemid != 0 )
    {
    	queryitemid = itemid;

    }else{

    	if ( header->currItems == 0 ) return 0;

        queryitemid = header->nextPasteItemId;
    }

    if ( queryitemid == 0 ) return 0;

    /* get the query item
    */
    if ( _XmClipboardFindItem( display, 
    				queryitemid,
    				&queryitem,
    				&reclength,
				0,
    				XM_DATA_ITEM_RECORD_TYPE ) == ClipboardFail ) 
    	return 0;

    if ( queryitem == 0 )
    {
	_XmClipboardError( "ClipboardCorrupt",
		"internal error - corrupt data structure" );
	return 0;
    }

    *count = queryitem->formatCount - queryitem->cancelledFormatCount;

    if ( *count < 0 ) *count = 0;

    /* point to the first format id in the list
    */
    idptr = (itemId*)AddAddresses( queryitem, queryitem->formatIdList );
    currformatid = *idptr;

    matchformat = 0;
    *matchlength = 0;
    index = 1;
    formatatom = XmInternAtom( display, format, False );

    /* run through all the formats for the query item looking */
    /* for a name match with the input format name
    */
    for ( i = 0; i < queryitem->formatCount; i++ )
    {
    	/* free the allocation unless it is the matching format
    	*/
    	free_flag = 1;

	/* get the next format
	*/
	_XmClipboardFindItem( display, 
			       currformatid,
			       &currformat,
			       &reclength,
			       0,
    			       XM_FORMAT_HEADER_TYPE );

    	if ( currformat == 0 )
    	{
	    _XmClipboardError( "ClipboardCorrupt",
		"internal error - corrupt data structure" );
    	    return 0;
    	}

    	if ( currformat->cancelledFlag == 0 )
    	{
	    /* format has not been cancelled
	    */
	    *maxnamelength = MAX( *maxnamelength, 
				  currformat->formatNameLength );

	    if (format != NULL)
    	    {
		if ( currformat->formatNameAtom == formatatom )
		{
		    matchformat = currformat;
    		    free_flag = 0;
    		    *matchlength = reclength;
    		}

    	    }else{
		/* we're looking for the n'th format 
		*/
    		if ( index == n )
    		{
		    matchformat = currformat;
    		    free_flag = 0;
    		    *matchlength = reclength;
    		}

		index = index + 1;
    	    }
    	}

    	if (free_flag == 1 )
    	{
    	    _XmClipboardFreeAlloc( currformat );
    	}

    	idptr = idptr + 1;
	
	currformatid = *idptr; 
    }

    _XmClipboardFreeAlloc( queryitem );

    return matchformat;
}

/*---------------------------------------------*/
static void _XmClipboardDeleteFormat( display, formatitemid )
Display *display;
itemId formatitemid;
{
    itemId  dataitemid;
    ClipboardDataItem dataitem;
    ClipboardFormatItem formatitem;
    int length, formatlength;

    /* first get the format item out of the root 
    */
    _XmClipboardFindItem( display, 
    			   formatitemid,
    			   &formatitem,
    			   &formatlength,
			   0,
    			   XM_FORMAT_HEADER_TYPE );

    if ( formatitem == 0 )
    {
	_XmClipboardError( "ClipboardCorrupt",
		"internal error - corrupt data structure" );
	return;
    }

    if ( ( formatitem->cutByNameFlag == 0 ) ||
    	 ( formatitem->cancelledFlag != 0 ) )
    {
    	/* nothing to do, data not passed by name or already cancelled
    	*/
    	_XmClipboardFreeAlloc( formatitem );
    	return;
    }

    dataitemid = formatitem->parentItemId;

    /* now get the data item out of the root 
    */
    _XmClipboardFindItem( display, 
    			   dataitemid,
    			   &dataitem,
    			   &length,
			   0,
    			   XM_DATA_ITEM_RECORD_TYPE );

    if ( dataitem == 0 )
    {
	_XmClipboardError( "ClipboardCorrupt",
		"internal error - corrupt data structure" );
	return;
    }

    dataitem->cancelledFormatCount = dataitem->cancelledFormatCount + 1; 

    if ( dataitem->cancelledFormatCount == dataitem->formatCount ) 
    {
    	/* no formats left, mark the item for delete
    	*/
        dataitem->deletePendingFlag = 1;
    }

    /* set the cancel flag on
    */
    formatitem->cancelledFlag = 1;

    /* return the property on the root window for the item
    */
    _XmClipboardReplaceItem( display, 
    			      formatitemid,
    			      formatitem,
    			      formatlength,
    			      PropModeReplace,
			      32,
			      True );

    _XmClipboardReplaceItem( display, 
    			      dataitemid,
    			      dataitem,
    			      length,
    			      PropModeReplace,
			      32,
    			      True );

}

/*---------------------------------------------*/
static void _XmClipboardDeleteFormats( display, window, dataitemid )
Display *display;
Window window;
itemId dataitemid;
{
    itemId *deleteptr;
    ClipboardDataItem datalist;
    ClipboardFormatItem formatdata;
    int length, i;

    /* first get the data item out of the root 
    */
    _XmClipboardFindItem( display, 
    			   dataitemid,
    			   &datalist,
    			   &length,
			   0,
    			   XM_DATA_ITEM_RECORD_TYPE );

    if ( datalist == 0 )
    {
	_XmClipboardError( "ClipboardCorrupt",
		"internal error - corrupt data structure" );
	return;
    }

    deleteptr = (itemId*)AddAddresses( datalist, datalist->formatIdList ); 

    for ( i = 0; i < datalist->formatCount; i++ )
    {
    	/* first delete the format data 
    	*/
	_XmClipboardFindItem( display, 
			       *deleteptr,
			       &formatdata,
			       &length,
			       0,
    			       XM_FORMAT_HEADER_TYPE );

    	if ( formatdata == 0 )
    	{
	    _XmClipboardError( "ClipboardCorrupt",
		"internal error - corrupt data structure" );
    	    return;
    	}

    	if ( formatdata->cutByNameFlag == 1 )
    	{
    	    /* format was cut by name
    	    */
	    _XmClipboardSendMessage( display, 
    				      window,
    				      formatdata,
				      XM_DATA_DELETE_MESSAGE );
    	}

    	_XmClipboardDeleteId( display, formatdata->formatDataId );

        _XmClipboardFreeAlloc( formatdata );

    	/* then delete the format header
    	*/
    	_XmClipboardDeleteId( display, *deleteptr );

    	*deleteptr = 0;

    	deleteptr = deleteptr + 1;
    }

    _XmClipboardFreeAlloc( datalist );
}

/*---------------------------------------------*/
 static void _XmClipboardDeleteItemLabel( display, window, dataitemid)
 Display *display;
 Window window;
 itemId dataitemid;
 {
     ClipboardDataItem datalist;
     int length;

     /* first get the data item out of the root
     */
     _XmClipboardFindItem( display,
                          dataitemid,
                          &datalist,
                          &length,
                          0,
                          XM_DATA_ITEM_RECORD_TYPE );

     if ( datalist == 0 )
     {
       _XmClipboardError( "ClipboardCorrupt",
               "internal error - corrupt data structure" );
       return;
     }
     /* delete item label
     */
     _XmClipboardDeleteId (display, datalist->dataItemLabelId);

     _XmClipboardFreeAlloc( datalist );
 }

/*---------------------------------------------*/
static unsigned long _XmClipboardIsMarkedForDelete( display, header, itemid )
Display *display;
ClipboardHeader header;
itemId itemid;
{
    ClipboardDataItem curritem;
    int reclength;
    unsigned long return_value;

    if ( itemid == 0 ) 
    {
	_XmClipboardError( "ClipboardCorrupt",
		"internal error - corrupt data structure" );
    	return 0;
    }

    /* get the next format
    */
    _XmClipboardFindItem( display, 
			   itemid,
			   &curritem,
			   &reclength,
			   0,
    			   XM_DATA_ITEM_RECORD_TYPE );

    return_value = curritem->deletePendingFlag;

    _XmClipboardFreeAlloc( curritem );

    return return_value;
}

/*---------------------------------------------*/
static void _XmClipboardDeleteItem( display, window, header, deleteid )
Display *display;
Window window;
ClipboardHeader header;
itemId deleteid;
{
    int i;
    itemId *listptr,*thisid, *nextid, nextpasteid;
    int nextpasteindex;
    int lastflag;

    /* find the delete id in the header item list 
    */
    listptr = (itemId*)AddAddresses( header, header->dataItemList ); 

    i = 0;

    nextpasteindex = 0;
    nextpasteid    = 0;

    nextid = listptr;
    thisid = nextid;

    /* redo the item list
    */
    while ( i < header->currItems )
    {
    	i = i + 1;

	if (*nextid == deleteid ) 
    	{
    	    nextid = nextid + 1;

    	    nextpasteindex = i - 2;

    	    /* if this flag doesn't get reset, then delete item was last item
    	    */
    	    lastflag = 1;

    	    continue;
    	}

    	lastflag = 0;

    	*thisid = *nextid;

    	thisid = thisid + 1;
	nextid = nextid + 1;
    }

    *thisid = 0;

    header->currItems = header->currItems - 1;

    /* if we are deleting the next paste item, then we need to find */
    /* a new one
    */
    if ( header->nextPasteItemId == deleteid )
    {

	if ( lastflag == 1 )
	{
	    nextpasteindex = nextpasteindex - 1; 
	}

	/* store this value temporarily
	*/
	i = nextpasteindex;

	/* now find the next paste candidate */
	/* first try to find next older item to make next paste
	*/
	while ( nextpasteindex >= 0 )
	{
	    thisid = listptr + nextpasteindex;

	    if ( !_XmClipboardIsMarkedForDelete( display, header, *thisid ) )
	    { 
		nextpasteid = *thisid;
		break;
	    }

	    nextpasteindex = nextpasteindex - 1;
	}

	/* if didn't find a next older item, find next newer item
	*/
	if ( nextpasteid == 0 )
	{
	    /* restore this value
	    */
	    nextpasteindex = i;

	    while ( nextpasteindex < header->currItems )
	    {
		thisid = listptr + nextpasteindex;

		if ( !_XmClipboardIsMarkedForDelete( display, header, *thisid ) )
		{ 
		    nextpasteid = *thisid;
		    break;
		}

		nextpasteindex = nextpasteindex + 1;
	    }
	}

        header->nextPasteItemId = nextpasteid;
        header->oldNextPasteItemId = 0;
    }
     /* delete the item label
     */
     _XmClipboardDeleteItemLabel( display, window, deleteid);

    /* delete all the formats belonging to the data item
    */
    _XmClipboardDeleteFormats( display, window, deleteid );

    /* now delete the item itself
    */
    _XmClipboardDeleteId( display, deleteid );
    	
}


/*---------------------------------------------*/
static void _XmClipboardDeleteMarked( display, window, header )
Display *display;
Window window;
ClipboardHeader header;
{
    itemId *nextIdPtr;
    int i, endi;

    /* find the header item list 
    */
    nextIdPtr = (itemId*)AddAddresses( header, header->dataItemList ); 

    i = 0;
    endi = header->currItems;

    /* run through the item list looking for things to delete
    */
    while( 1 )
    { 
    	if ( i >= endi ) break;

    	i = i + 1;

	if ( _XmClipboardIsMarkedForDelete( display, header, *nextIdPtr ) )
    	{
    	    _XmClipboardDeleteItem( display, window, header, *nextIdPtr );

    	}else{
    	    nextIdPtr = nextIdPtr + 1;
    	}    
    }
}
/*---------------------------------------------*/
static void _XmClipboardMarkItem( display, header, dataitemid, state )
Display *display;
ClipboardHeader header;
itemId dataitemid;
unsigned long state;
{
    ClipboardDataItem itemheader;
    int itemlength;

    if ( dataitemid == 0 ) return;

    /* get a pointer to the item
    */
    _XmClipboardFindItem( display, 
    			   dataitemid,
    			   &itemheader,
    			   &itemlength,
			   0,
    			   XM_DATA_ITEM_RECORD_TYPE );

    if ( itemheader == 0 ) 
    {
	_XmClipboardError( "ClipboardCorrupt",
		"internal error - corrupt data structure" );
    	return;
    }

    /* mark the delete pending flag
    */
    itemheader->deletePendingFlag = state;

    /* return the item to the root window 
    */
    _XmClipboardReplaceItem( display, 
    			      dataitemid,
    			      itemheader,
    			      itemlength,
    			      PropModeReplace,
			      32,
    			      True );

}

/*---------------------------------------------*/
static int _XmClipboardSendMessage( display, 
    				     window,
    				     formatptr,
    				     messagetype )
Display *display;
Window window;
ClipboardFormatItem formatptr;
int messagetype;
{
    Window widgetwindow;
    XClientMessageEvent event_sent;
    long event_mask = 0;
    Display *widgetdisplay;
    int headerlength;
    ClipboardHeader root_clipboard_header;

    widgetdisplay = formatptr->displayId;
    widgetwindow = formatptr->cutByNameWindow;

    if ( widgetwindow == 0 ) return 0;

    event_sent.type         = ClientMessage;
    event_sent.display      = widgetdisplay;
    event_sent.window       = widgetwindow;
    event_sent.message_type = XmInternAtom( display, 
    					    "_MOTIF_CLIP_MESSAGE", False );
    event_sent.format = 32;

    switch ( messagetype )
    {	
	case XM_DATA_REQUEST_MESSAGE:	

	    /* get the clipboard header
	    */
	    _XmClipboardFindItem( display, 
				   XM_HEADER_ID,
				   &root_clipboard_header,
				   &headerlength,
				   0,
				   0 );

	    /* set the recopy item id in the header (so locking can be circumvented)
	    */
	    root_clipboard_header->recopyId = formatptr->thisFormatId;

	    /* replace the clipboard header
	    */
	    _XmClipboardReplaceItem( display, 
				      XM_HEADER_ID,
				      root_clipboard_header,
				      headerlength,
				      PropModeReplace,
				      32,
				      True );

            event_sent.data.l[0] = XmInternAtom( display, 
    	    			    	        "_MOTIF_CLIP_DATA_REQUEST",
   	  					False );
	    break;

	case XM_DATA_DELETE_MESSAGE:	
            event_sent.data.l[0] = XmInternAtom( display, 
    	    			    		"_MOTIF_CLIP_DATA_DELETE", 
   	  					False );
	    break;
    }

    event_sent.data.l[1] = formatptr->thisFormatId;
    event_sent.data.l[2] = formatptr->itemPrivateId;

    /* is this the same application that stored the data?
    */
    if ( formatptr->windowId == window && formatptr->displayId == display )
    {
    	/* call the event handler directly to avoid blocking 
    	*/
    	_XmClipboardEventHandler( formatptr->cutByNameWidget,
    				   0,
    				   &event_sent );
    }else{

	/* if we aren't in same application that stored the data, then */
	/* make sure the window still exists
	*/
        if ( !_XmClipboardWindowExists( display, widgetwindow )) return 0;

	/* send a client message to the window supplied by the user
	*/
	XSendEvent( display, widgetwindow, True, event_mask, 
		   (XEvent*)&event_sent ); 
    }

    return 1;
}

/*---------------------------------------------*/
static int _XmClipboardDataIsReady( display, event, private_info )
Display *display;
XEvent *event;
char *private_info;
{
     XDestroyWindowEvent *destroy_event;
     ClipboardCutByNameInfo cutbynameinfo;
     ClipboardFormatItem formatitem;
     int formatlength;
     int okay;
 
     cutbynameinfo = ( ClipboardCutByNameInfo )private_info;

     if ( (event->type & 127) == DestroyNotify )
     {
        destroy_event = (XDestroyWindowEvent*)event;

        if ( destroy_event->window == cutbynameinfo->window )
        {
            cutbynameinfo->window = 0;
            return 1;
        }
     }

     if ( (event->type & 127) != PropertyNotify )
    	return 0;


     /* get the format item
     */
     _XmClipboardFindItem( display,
 			   cutbynameinfo->formatitemid,
 			   &formatitem,
 			   &formatlength,
			   0,
    			   XM_FORMAT_HEADER_TYPE );
 
 
     okay = (int)( formatitem->cutByNameFlag == 0 );
     	    
     /* release the allocation
     */
     _XmClipboardFreeAlloc( formatitem );
     
     return okay;
     
}


/*---------------------------------------------*/
static int _XmClipboardSelectionIsReady( display, event, private_info )
Display *display;
XEvent *event;
char *private_info;
{
    XPropertyEvent *property_event;
    XSelectionEvent *selection_event;
    XDestroyWindowEvent *destroy_event;
    char *outpointer, *temp_ptr;
    int outlength, loc_format, ret_value;
    Atom loc_type;
    ClipboardSelectionInfo info;
    Boolean get_the_property;

    info = ( ClipboardSelectionInfo )private_info;

    get_the_property = FALSE;

    if ( (event->type & 127) == DestroyNotify )
    {
        destroy_event = (XDestroyWindowEvent*)event;

        if ( destroy_event->window == info->selection_window )
        {
            info->selection_window = 0;
            return 1;
        }
    }

    if ( (event->type & 127) == SelectionNotify )
    {
        selection_event = (XSelectionEvent*)event;

	if (selection_event->property == None)
        {
            return 1;
        }

	if (selection_event->property == XA_CLIP_TEMP)
	{
	    info->selection_notify_received = TRUE;
	    get_the_property = TRUE;
	}
    }

    if ( (event->type & 127) == PropertyNotify )
    {
        property_event = (XPropertyEvent*)event;

        /* make sure we have right property and are ready
        */
        if ( property_event->atom == XA_CLIP_TEMP
                            &&
             property_event->state == PropertyNewValue
                            &&
             info->selection_notify_received == TRUE )
        {
            get_the_property = TRUE;
        }
    }

    if ( get_the_property )
    {
        /* get the property from the window, deleting it as we do
        */
        ret_value = _XmGetWindowProperty( info->display,
                                           info->window,
                                           XA_CLIP_TEMP,
                                           &outpointer,
                                           &outlength,
                                           &loc_type,
                                           &loc_format,
                                           TRUE );

        /* if zero length then done
        */
        if ( outpointer == 0 || outlength == 0
                             || ret_value != ClipboardSuccess )
        {
            return 1;
        }

        /* is the target INCR?
        */
        if ( loc_type == XA_CLIP_INCR )
        {
            info->incremental = TRUE;

            return 0;
        }

        info->type = loc_type;

        /* length returned is in bytes */
        /* allocate storage for the data
        */
        temp_ptr = (char *)_XmClipboardAlloc( info->count + outlength );

        /* copy over what we have so far
        */
        bcopy( info->data, temp_ptr, info->count );

        /* free up old allocation
        */
        _XmClipboardFreeAlloc( info->data );

        info->data = (char *)temp_ptr;

        /* append the new stuff from the property
        */
        bcopy( outpointer, temp_ptr + info->count, outlength );

        info->count = info->count + outlength;

        /* if we aren't in incremental receive mode, then we're done
        */
        if ( !info->incremental )
        {
            return 1;
        }
    }

    return 0;

 }


/*---------------------------------------------*/
static int _XmClipboardRequestorIsReady( display, event, private_info )
Display *display;
XEvent *event;
char *private_info;
{
    XPropertyEvent *property_event;
    XDestroyWindowEvent *destroy_event;
    ClipboardDestroyInfo info;

    info = ( ClipboardDestroyInfo )private_info;

    if ( (event->type & 127) == DestroyNotify )
    {
        destroy_event = (XDestroyWindowEvent*)event;

        if ( destroy_event->window == info->window )
        {
            info->window = 0;
            return 1;
        }
    }

    if ( (event->type & 127) == PropertyNotify )
    {
        property_event = (XPropertyEvent*)event;

        /* make sure we have right property and are ready
        */
        if ( property_event->atom == info->property
                            &&
             property_event->state == PropertyDelete )
        {
            return 1;
        }
    }

    return 0;
 }


/*---------------------------------------------*/
static int _XmClipboardGetSelection( display,
                                      window,
                                      format,
                                      header,
                                      format_data,
                                      format_length )
Display *display;
Window window;
char *format;
ClipboardHeader header;
char **format_data;
unsigned long *format_length;
{
    XEvent event_return;
    ClipboardSelectionInfoRec info;
    int dataisready;
    Window selectionwindow;
    XWindowAttributes window_attributes, sel_window_attributes;
    Atom format_atom;

    format_atom = XmInternAtom( display, format, FALSE );

    /* will return null if window no longer exists that last had selection
    */
    selectionwindow = XGetSelectionOwner( display, XA_CLIPBOARD );

    if ( selectionwindow == None )
    {
        return FALSE;
    }

    /* get the selection owner window event mask
    */
    XGetWindowAttributes( display, selectionwindow, &sel_window_attributes );

    /* select for structure notify as well as current mask
    */
    XSelectInput( display,
                  selectionwindow,
                  StructureNotifyMask | sel_window_attributes.your_event_mask );

    /* get the current window event mask
    */
    XGetWindowAttributes( display, window, &window_attributes );

    /* select for property notify as well as current mask
    */
    XSelectInput( display,
                  window,
                  PropertyChangeMask | window_attributes.your_event_mask );

    /* ask for the data in the specified format
    */
    XConvertSelection( display,
                       XA_CLIPBOARD,
                       format_atom,
                       XA_CLIP_TEMP,
                       window,
                       header->copyFromTimestamp );

    /* initialize the fields in the info record passed to the */
    /* predicate function
    */
    info.display = display;
    info.format  = format;
    info.window  = window;
    info.selection_window  = selectionwindow;
    info.time    = header->copyFromTimestamp;
    info.incremental          = FALSE;
    info.selection_notify_received = FALSE;
    info.data = 0;
    info.count = 0;

    /* check to see if have property change or selection notify events */
    /* this doesn't block
    */
    dataisready = XCheckIfEvent( display,
                                 &event_return,
                                 _XmClipboardSelectionIsReady,
                                 (char*)&info );

    if ( info.selection_window == 0 )
    {
        /* this means the window was destroyed before selection ready
        */
        /* put mask back the way it was
        */
        XSelectInput( display,
                      window,
                      window_attributes.your_event_mask );

        return FALSE;
    }

    if ( !dataisready )
    {
        /* check to see if have property change or selection notify events */
        /* this blocks waiting for event
        */
        XIfEvent( display,
                  &event_return,
                  _XmClipboardSelectionIsReady,
                  (char*)&info );
    }

    /* put mask back the way it was
    */
    XSelectInput( display,
                  window,
                  window_attributes.your_event_mask );


    if ( info.selection_window == 0 )
    {
        /* this means the window was destroyed before selection ready
        */
        return FALSE;
    }


    /* put mask back the way it was
    */
    XSelectInput( display,
                  selectionwindow,
                  sel_window_attributes.your_event_mask );

    *format_data = info.data;
    *format_length = info.count;

    if (*format_data==NULL || *format_length==0)
	return FALSE;

    return TRUE;
}


/*---------------------------------------------*/
static int _XmClipboardRequestDataAndWait( display, 
    					    window,
    					    formatptr )
Display *display;
Window window;
ClipboardFormatItem formatptr;
{
    XEvent event_return;
    int dataisready;
    XWindowAttributes rootattributes;
    Window rootwindow;
    ClipboardCutByNameInfoRec cutbynameinfo;

    rootwindow = XDefaultRootWindow( display );

    /* get the current root window event mask
    */
    XGetWindowAttributes( display, rootwindow, &rootattributes );

    /* select for property notify as well as current mask
    */
    XSelectInput( display, 
		  rootwindow, 
		  PropertyChangeMask  |
		  StructureNotifyMask | rootattributes.your_event_mask );

    if ( _XmClipboardSendMessage( display, 
    			           window,
    			           formatptr,
    			           XM_DATA_REQUEST_MESSAGE ) == 0 )
    {
	/* put mask back the way it was
	*/
	XSelectInput( display, 
		      rootwindow, 
		      rootattributes.your_event_mask );
    	return 0;
    }

    cutbynameinfo.formatitemid = formatptr->thisFormatId;
    cutbynameinfo.window = window;

    dataisready = XCheckIfEvent( display, 
    				 &event_return, 
    				 _XmClipboardDataIsReady, 
                                 (char*)&cutbynameinfo );

    if ( cutbynameinfo.window == 0 )
    {
        /* this means the cut by name window had been destroyed
        */
        return 0;
    }

    if ( !dataisready )
    {

        XIfEvent( display, 
    		  &event_return, 
    		  _XmClipboardDataIsReady, 
                  (char*)&cutbynameinfo );
    }

    if ( cutbynameinfo.window == 0 )
    {
        /* this means the cut by name window had been destroyed
        */
        return 0;
    }

    /* put mask back the way it was
    */
    XSelectInput( display, 
		  rootwindow, 
		  rootattributes.your_event_mask );

    return 1;
}

/*---------------------------------------------*/
static itemId _XmClipboardGetNewItemId( display )
{
    char *propertynumber;
    int *integer_ptr;
    int length;
    itemId loc_id;

    _XmClipboardFindItem( display,
			   XM_NEXT_ID,
			   &propertynumber,
			   &length,
			   0,
    			   0 );

    integer_ptr = (int *)propertynumber;

    *integer_ptr = *integer_ptr + 1;

    loc_id = (itemId)*integer_ptr;

    _XmClipboardReplaceItem( display,
			      XM_NEXT_ID,
			      propertynumber,
			      length,
    			      PropModeReplace,
			      32,
    			      True );

    return loc_id;
}


/*---------------------------------------------*/
static int _XmClipboardSetAccess( display, window )
Display *display;
Window window;
{
    Atom itematom;

    itematom = XmInternAtom( display, 
			    "_MOTIF_CLIP_LOCK_ACCESS_VALID", False );

    /* put the clipboard lock access valid property on window
    */
    XChangeProperty( display, 
		     window, 
		     itematom,
		     itematom,
		     8,
		     PropModeReplace,
		     (unsigned char*)"yes",
		     3 );
}

/*---------------------------------------------*/
static int _XmClipboardLock (display, window)
Display *display;
Window window;
{
    ClipboardLockPtr lockptr;
    int length;

    _XmClipboardFindItem (display, XM_LOCK_ID, &lockptr, &length, 0, 0);

    if (length == 0)	/* create new lock property */
    {
	lockptr = (ClipboardLockPtr)_XmClipboardAlloc(sizeof(ClipboardLockRec));
	lockptr->lockLevel = 0;
    }

    if (lockptr->lockLevel == 0) /* new or invalid V1.0 lock. Take the lock */
    {
	lockptr->windowId = window;
	lockptr->lockLevel = 1;
    }
    else if (lockptr->windowId == window) /* already have the lock */
    {
	lockptr->lockLevel += 1;
    }
    else	/* another client has the lock */
    {
	if (_XmClipboardWindowExists (display, lockptr->windowId))
	{
	    _XmClipboardFreeAlloc ((char *)lockptr);
	    return (ClipboardLocked);
	}
	else /* locking client has gone away, clipboard may be corrupted */
	{
	    ClipboardHeader header;
	    Window owner = XGetSelectionOwner (display, XA_CLIPBOARD);
	    Time timestamp = _XmClipboardGetCurrentTime(display);

	    /* Drop the selection if a Motif client owns it */

            header = _XmClipboardOpen (display, 0);
	    if (header->ownSelection == owner)
	    {
		XSetSelectionOwner (display, XA_CLIPBOARD, None, timestamp);
	    }
            _XmClipboardClose (display, header);

	    /* Reset the header property */

	    XDeleteProperty (display, XDefaultRootWindow (display),
			XmInternAtom (display, "_MOTIF_CLIP_HEADER", False));
	    header = _XmClipboardOpen (display, 0);
	    _XmClipboardClose (display, header);

	    /* Take the lock */

	    lockptr->windowId = window;
	    lockptr->lockLevel = 1;
	}	    
    }

    _XmClipboardReplaceItem (display, XM_LOCK_ID, lockptr,
		sizeof(ClipboardLockRec), PropModeReplace, 32, False );

    _XmClipboardSetAccess (display, window);

    _XmClipboardFreeAlloc ((char *)lockptr);

    return (ClipboardSuccess);
}

/*---------------------------------------------*/
static int _XmClipboardUnlock( display, window, all_levels )
Display *display;
Window window;
Boolean all_levels;
{
    int length;
    ClipboardLockPtr lockptr;

    _XmClipboardFindItem (display, XM_LOCK_ID, &lockptr, &length, 0, 0);

    if (length == 0) /* There is no lock property */
    {
    	return (ClipboardFail);
    }

    if ( lockptr->windowId != window ) /* Someone else has the lock */
    {
    	_XmClipboardFreeAlloc ((char *)lockptr);
    	return (ClipboardFail);
    }

    /* do the unlock */

    if (all_levels == 0)
    {
        lockptr->lockLevel -= 1;
    }else{
        lockptr->lockLevel = 0;
    }

    if (lockptr->lockLevel <= 0)
    {
	length = 0;
    }else{
	length = sizeof(ClipboardLockRec);
    }

    _XmClipboardReplaceItem (display, XM_LOCK_ID, lockptr, length,
			PropModeReplace, 32, False);
    _XmClipboardFreeAlloc ((char *)lockptr);

    return (ClipboardSuccess);
}

/*---------------------------------------------*/
static int _XmClipboardSearchForWindow( display, parentwindow, window )
Display *display;
Window parentwindow;
Window window;
{
    /* recursively search the roots descendant tree for the given window */

    Window rootwindow, p_window, *children;
    unsigned int numchildren;
    int found, i;
    Window *windowptr;

    XQueryTree( display, parentwindow, &rootwindow, &p_window,
		     &children, &numchildren );

    found = 0;
    windowptr = children;

    /* now search through the list for the window */
    for ( i = 0; i < numchildren; i++ )
    {
	if ( *windowptr == window )
    	{
    	    found = 1;
    	}else{
    	    found = _XmClipboardSearchForWindow( display, *windowptr, window); 
   	}
    	if ( found == 1 ) break;
	windowptr = windowptr + 1;
    }

    _XmClipboardFreeAlloc( (char*)children );

    return found;
}

/*---------------------------------------------*/
static int _XmClipboardWindowExists( display, window )
Display *display;
Window window;
{
    Window rootwindow;
    Atom itematom;
    int exists;
    unsigned long outlength;
    unsigned char *outpointer;
    Atom type;
    int ret_value;
    int format;
    unsigned long bytes_left;

    rootwindow = XDefaultRootWindow( display );

    exists = _XmClipboardSearchForWindow( display, rootwindow, window );

    if ( exists == 1 )
    {
	/* see if the window has the lock activity property, for if */
	/* it doesn't then this is a new assignment of the window id */
	/* and the lock is bogus due to a crash of the application */
	/* with the original locking window   
	*/
       itematom = XmInternAtom(display, "_MOTIF_CLIP_LOCK_ACCESS_VALID", False);

       ret_value = XGetWindowProperty( display, 
					window, 
					itematom,
					0, /*offset*/
					10000000, /*length*/
					False,
					AnyPropertyType,
					&type,
					&format,			    
					&outlength,
					&bytes_left,
					&outpointer );

	if ( outpointer == 0 || outlength == 0 )
    	{
    	    /* not the same window that locked the clipboard 
    	    */
	    exists = 0;
    	}
       
	if (outpointer != NULL)
		XFree((char*)outpointer);
    }    

    return exists;
}
/*---------------------------------------------*/
/* external routines			       */
/*---------------------------------------------*/


/*---------------------------------------------*/
#ifdef _NO_PROTO
int XmClipboardBeginCopy( display, window, label, widget, callback, itemid )
Display 	*display; /* display id for application passing data */
Window		window;   /* window to receive request for cut by name data */
XmString	label;    /* label to be associated with data item   */
Widget		widget;   /* only for cut by name */
VoidProc  	callback; /* address of callback routine for cut by name */
long            *itemid;  /* received id to identify this data item */
#else /* _NO_PROTO */
int XmClipboardBeginCopy(Display *display, Window window, XmString label,
			Widget widget, VoidProc callback, long *itemid)
#endif /* _NO_PROTO */
{
    int status;

    status = XmClipboardStartCopy( display, window, label, CurrentTime, 
			     widget, callback, itemid );

    return status;
}

/*---------------------------------------------*/
#ifdef _NO_PROTO
int XmClipboardStartCopy( display, window, label, timestamp, 
			     widget, callback, itemid )
Display 	*display; /* display id for application passing data */
Window		window;   /* window to receive request for cut by name data */
XmString	label;    /* label to be associated with data item   */
Time		timestamp; /* timestamp of event triggering copy to clipboard */
Widget		widget;   /* only for cut by name */
VoidProc  	callback; /* address of callback routine for cut by name */
long            *itemid;  /* received id to identify this data item */
#else /* _NO_PROTO */
int XmClipboardStartCopy(Display *display, Window window, XmString label,
		Time timestamp, Widget widget, VoidProc callback, long *itemid)
#endif /* _NO_PROTO */
{
    ClipboardHeader header;
    ClipboardDataItem itemheader;
    int itemlength;
    itemId loc_itemid;
    int status;

    status = _XmClipboardLock( display, window );
    if ( status == ClipboardLocked ) return ClipboardLocked;

    /* get the clipboard header, make sure clipboard is initialized
    */
    header = _XmClipboardOpen( display, 0 );

    header->selectionTimestamp = timestamp;
    header->startCopyCalled = (unsigned long) True;

    /* allocate storage for the data item  
    */ 
    itemlength = sizeof( ClipboardDataItemRec );

    itemheader = (ClipboardDataItem)_XmClipboardAlloc( itemlength );

    loc_itemid = _XmClipboardGetNewItemId( display ); 

    /* initialize fields in the data item
    */
    itemheader->thisItemId = loc_itemid;
    itemheader->adjunctData = 0;
    itemheader->recordType = XM_DATA_ITEM_RECORD_TYPE;
    itemheader->displayId = display;
    itemheader->windowId  = window;
    itemheader->dataItemLabelId = _XmClipboardGetNewItemId( display ); 
    itemheader->formatIdList = itemlength; /* offset */
    itemheader->formatCount = 0;
    itemheader->cancelledFormatCount = 0;
    itemheader->deletePendingFlag = 0;
    itemheader->permanentItemFlag = 0;
    itemheader->cutByNameFlag = 0;
    itemheader->cutByNameCallback = 0;
    itemheader->cutByNameWidget = 0;
    itemheader->cutByNameWindow = 0;

    if ( callback != 0 && widget != 0 )
    {
    	/* set up client message handling if widget passed
    	*/
	itemheader->cutByNameCallback = callback;
	itemheader->cutByNameWidget = widget;
	itemheader->cutByNameWindow = XtWindow( widget );
	_XmClipboardSetAccess( display, itemheader->cutByNameWindow );
    }
    
    /* store the label 
    */
    _XmClipboardReplaceItem( display, 
    			      itemheader->dataItemLabelId,
    			      label,
    			      XmStringLength( label ) + 1,
    			      PropModeReplace,
			      8,
    			      False );

    /* return the item to the root window 
    */
    _XmClipboardReplaceItem( display, 
    			      loc_itemid,
    			      itemheader,
    			      itemlength,
    			      PropModeReplace,
			      32,
    			      True );

    if ( itemid != 0 )
    {
        *itemid = (long)loc_itemid;
    }

    _XmClipboardClose( display, header );

    _XmClipboardUnlock( display, window, 0 );

    return ClipboardSuccess;
}

/*---------------------------------------------*/
#ifdef _NO_PROTO
int XmClipboardCopy( display, window, itemid, format, buffer, length,
    			private_id, dataid )
Display		*display; /* Display id of application passing data */
Window		window;
long		itemid;   /* id returned from begin copy */
char		*format; /* Name string for data format */
char		*buffer; /* Address of buffer holding data in this format */
unsigned long	length;  /* Length of the data */
int		private_id; /* Private id provide by application */
int		*dataid;   /* Data id returned by clipboard */
#else /* _NO_PROTO */
int XmClipboardCopy(Display *display, Window window, long itemid, char *format,
		char *buffer, unsigned long length, int private_id, int *dataid)
#endif /* _NO_PROTO */
{
    ClipboardDataItem itemheader;
    ClipboardHeader header;
    ClipboardFormatItem formatptr;
    char *formatdataptr;
    itemId formatid, formatdataid, *idptr;
    int itemlength, formatlength, formatdatalength;
    char *to_ptr;
    int status, maxname, count;
    int format_len;

    status = _XmClipboardLock( display, window );
    if ( status == ClipboardLocked ) return ClipboardLocked;

    /* get the clipboard header
    */
    header = _XmClipboardOpen( display, 0 );
 
    if (!header->startCopyCalled) {
	XtWarning(XM_CLIPBOARD_MESSAGE1);
        _XmClipboardUnlock( display, window, 0 );
        return ClipboardFail;
    } 

    /* first check to see if the format already exists for this item
    */
    formatptr = _XmClipboardFindFormat( display, header, format, 
    					   (itemId)itemid, NULL,
    					   &maxname, &count, &formatlength );

    /* if format doesn't exist, then have to access the data item */
    /* record
    */
    if ( formatptr == 0 )
    {
	/* get a pointer to the item
	*/
	_XmClipboardRetrieveItem( display, 
				   (itemId)itemid,
				   sizeof( itemId ),
				   0,
				   &itemheader,
				   &itemlength,
				   0,
				   XM_DATA_ITEM_RECORD_TYPE,
				   0 );

	itemheader->formatCount = itemheader->formatCount + 1;

	if ((itemheader->formatCount * 2 + 2) >= XM_ITEM_ID_INC) {
	    XtWarning(XM_CLIPBOARD_MESSAGE3);
	    _XmClipboardFreeAlloc(itemheader);
	    _XmClipboardUnlock( display, window, 0 );
	    return ClipboardFail;
	}

	formatlength = sizeof( ClipboardFormatItemRec );

	/* allocate local storage for the data format record 
	*/ 
	formatptr     = (ClipboardFormatItem)_XmClipboardAlloc( formatlength );

	formatid     = _XmClipboardGetNewItemId( display ); 
	formatdataid = _XmClipboardGetNewItemId( display ); 

	/* put format record id in data item's format id list
	*/
	idptr = (itemId*)AddAddresses( itemheader, 
				       itemlength - sizeof( itemId ) ); 

	*idptr = formatid; 

	/* initialize the fields in the format record
	*/
	formatptr->recordType = XM_FORMAT_HEADER_TYPE;
	formatptr->formatNameAtom = XmInternAtom( display, format, False );
	formatptr->itemLength = 0;
	formatptr->formatNameLength = strlen( format );
	formatptr->formatDataId = formatdataid;
	formatptr->thisFormatId = formatid;
	formatptr->itemPrivateId = private_id;
	formatptr->cancelledFlag = 0;
	formatptr->copiedLength = 0;
	formatptr->parentItemId = itemid;
	formatptr->cutByNameWidget = itemheader->cutByNameWidget;
	formatptr->cutByNameWindow = itemheader->cutByNameWindow;
	formatptr->cutByNameCallback = itemheader->cutByNameCallback;
	formatptr->windowId = itemheader->windowId;
	formatptr->displayId = itemheader->displayId;

	/* if buffer is null then it is a pass by name
	*/
	if ( buffer != 0 )
	{
	    formatptr->cutByNameFlag = 0;
	    formatdatalength = length;
	}else{
	    itemheader->cutByNameFlag = 1;
	    formatptr->cutByNameFlag = 1;
	    formatdatalength = 4;  /* we want a property stored regardless */
	}    	

	/* return the property on the root window for the item
	*/
	_XmClipboardReplaceItem( display, 
				  itemid,
				  itemheader,
				  itemlength,
				  PropModeReplace,
				  32,
				  True );


	if( _XmClipboardGetLenFromFormat( display, format, &format_len ) 
			== ClipboardFail)
	{
	    /* if it's one of the predefined formats, register it for second try
	    */
	    XmClipboardRegisterFormat( display, 
					format, 
					0 );

	    _XmClipboardGetLenFromFormat( display, format, &format_len );
	}

 
	formatdataptr = _XmClipboardAlloc( formatdatalength );

        to_ptr = formatdataptr; 

    }else{
    	formatid = formatptr->thisFormatId;
    	formatdataid = formatptr->formatDataId;
    	
    	/* the format already existed so get the data and append
    	*/
    	_XmClipboardRetrieveItem( display, 
    			           formatdataid,
    				   length,
    				   0,
    			           &formatdataptr,
    			           &formatdatalength,
				   &format_len,
    				   0,
				   0 );

        to_ptr = AddAddresses(formatdataptr, formatdatalength-length ); 
    }

    if ( buffer != 0 )
    {
	/* copy the format data over to acquired storage
	*/
	bcopy( buffer, to_ptr, length );
    }

    formatptr->itemLength = formatptr->itemLength + length;

    /* replace the property on the root window for the format data
    */
    _XmClipboardReplaceItem( display, 
			      formatdataid,
			      formatdataptr,
			      formatdatalength,
    			      PropModeReplace,
			      format_len,  /* 8, 16, or 32 */
    			      True );
    
    /* replace the property on the root window for the format 
    */
    _XmClipboardReplaceItem( display, 
    			      formatid,
    			      formatptr,
    			      formatlength,
    			      PropModeReplace,
			      32,
    			      True );

    if ( dataid != 0 )
    {
        *dataid = formatid;
    }

    _XmClipboardClose( display, header );

    _XmClipboardUnlock( display, window, 0 );

    return ClipboardSuccess;
}

/*---------------------------------------------*/
#ifdef _NO_PROTO
int XmClipboardEndCopy( display, window, itemid )
Display *display;
Window  window;
long    itemid;
#else /* _NO_PROTO */
int XmClipboardEndCopy(Display *display, Window window, long itemid)
#endif /* _NO_PROTO */
{
    ClipboardDataItem itemheader;
    ClipboardHeader header;
    itemId *itemlist;
    int itemlength, newitemoffset;
    itemId *newitemaddr;
    int status;

    status = _XmClipboardLock( display, window );
    if ( status == ClipboardLocked ) return ClipboardLocked;

    /* get the clipboard header
    */
    header = _XmClipboardOpen( display, sizeof( itemId ) );

    if (!header->startCopyCalled) {
	XtWarning(XM_CLIPBOARD_MESSAGE2);
        _XmClipboardUnlock( display, window, 0 );
        return ClipboardFail;
    } 

    _XmClipboardDeleteMarked( display, window, header );

    if (header->currItems >= header->maxItems) 
    {
    	itemlist = (itemId*)AddAddresses( header, header->dataItemList );

    	/* mark least recent item for deletion and delete previously marked
    	*/
        _XmClipboardMarkItem( display, header, *itemlist, XM_DELETE );

    	header->deletedByCopyId = *itemlist;
    } else {
	header->deletedByCopyId = 0;
    }

    newitemoffset = header->dataItemList + 
    		    (header->currItems * sizeof( itemId ));

    /* stick new item at the bottom of the list 
    */
    newitemaddr = (itemId*)AddAddresses( header, newitemoffset );

    *newitemaddr = (itemId)itemid;

    /* new items always become next paste item 
    */
    header->oldNextPasteItemId  = header->nextPasteItemId;
    header->nextPasteItemId = (itemId)itemid;
    header->lastCopyItemId = (itemId)itemid;

    header->currItems = header->currItems + 1;
    header->startCopyCalled = False;

    /* if there was a cut by name format, then set up event handling
    */
    _XmClipboardFindItem( display,
			   itemid,
			   &itemheader,
			   &itemlength,
			   0,
    			   XM_DATA_ITEM_RECORD_TYPE );

    if ( itemheader->cutByNameWidget != 0 )
    {
        unsigned long event_mask = 0;

    	XtAddEventHandler( itemheader->cutByNameWidget, event_mask, TRUE, 
    			   _XmClipboardEventHandler, 0 );
    }

    _XmClipboardFreeAlloc( itemheader ); 

    _XmAssertClipboardSelection( display, window, header, 
				  header->selectionTimestamp );

    _XmClipboardSetNextItemId(display, itemid);

    _XmClipboardClose( display, header );

    _XmClipboardUnlock( display, window, 0 );

    return ClipboardSuccess;
}

/*---------------------------------------------*/
#ifdef _NO_PROTO
int XmClipboardCancelCopy( display, window, itemid )
Display *display;
Window  window;
long    itemid;   /* id returned by begin copy */
#else /* _NO_PROTO */
int XmClipboardCancelCopy(Display *display, Window window, long itemid)
#endif /* _NO_PROTO */
{
    itemId deleteitemid;
    int previous;
    int *int_ptr;
    int length;
    ClipboardHeader header;

    if ( _XmClipboardLock( display, window) == ClipboardLocked )
	return (ClipboardLocked);

    deleteitemid = (itemId)itemid;

/*
 * first, free up the properties set by the StartCopy and Copy calls.
 */
     /* delete the item label
     */
     _XmClipboardDeleteItemLabel( display, window, deleteitemid);

    /* delete all the formats belonging to the data item
    */
    _XmClipboardDeleteFormats( display, window, deleteitemid );

    /* now delete the item itself
    */
    _XmClipboardDeleteId( display, deleteitemid );

/*
 * reset the startCopyCalled flag and reset the XM_NEXT_ID property to
 * it's value prior to StartCopy.
 */

    _XmClipboardFindItem( display,
			XM_NEXT_ID,
			&int_ptr,
			&length,
			0,
			0 );
    previous = itemid - 1;
    *int_ptr = previous;
    length = sizeof( int );
    _XmClipboardReplaceItem( display,
			XM_NEXT_ID,
			int_ptr,
			length,
			PropModeReplace,
			32,
			True );

    header = _XmClipboardOpen( display, 0);
    header->startCopyCalled = False;
    _XmClipboardClose( display, header );

    _XmClipboardUnlock( display, window, 0 );

    return (ClipboardSuccess);
}

/*---------------------------------------------*/
#ifdef _NO_PROTO
int XmClipboardWithdrawFormat( display, window, data )
Display *display;
Window  window;
int	data;     /* data id of format no longer provided by application */
#else /* _NO_PROTO */
int XmClipboardWithdrawFormat (Display *display, Window window, int data)
#endif /* _NO_PROTO */
{
    int status;

    status = _XmClipboardLock( display, window  );
    if ( status == ClipboardLocked ) return ClipboardLocked;

    _XmClipboardDeleteFormat( display, data );

    _XmClipboardUnlock( display, window, 0 );

    return ClipboardSuccess;
}

/*---------------------------------------------*/
#ifdef _NO_PROTO
int XmClipboardCopyByName( display, window, data, buffer, length, private_id )
Display		*display; /* Display id of application passing data */
Window		window;
int		data;    /* Data id returned previously by clipboard */
char		*buffer; /* Address of buffer holding data in this format */
unsigned long	length;  /* Length of the data */
int		private_id; /* Private id provide by application */
#else /* _NO_PROTO */
int XmClipboardCopyByName(Display *display, Window window, int data,
	char *buffer, unsigned long length, int private_id)
#endif /* _NO_PROTO */
{
    ClipboardFormatItem formatheader;
    int format;
    char *formatdataptr;
    int formatlength, formatdatalength;
    char *to_ptr;
    int status, locked;
    int headerlength;
    ClipboardHeader root_clipboard_header;

    /* get the clipboard header
    */
    _XmClipboardFindItem( display, 
			   XM_HEADER_ID,
			   &root_clipboard_header,
			   &headerlength,
			   0,
    			   0 );

    locked = 0;

    /* if this is a recopy as the result of a callback, then circumvent */
    /* any existing lock
    */
    if ( root_clipboard_header->recopyId != data )
    {        
	status = _XmClipboardLock( display, window );
	if ( status == ClipboardLocked ) return ClipboardLocked;
    	locked = 1;

    }else{
	root_clipboard_header->recopyId = 0;

	/* replace the clipboard header 
	*/
	_XmClipboardReplaceItem( display, 
				  XM_HEADER_ID,
				  root_clipboard_header,
				  headerlength,
				  PropModeReplace,
				  32,
				  False );
    }

    /* get a pointer to the format
    */
    if ( _XmClipboardFindItem( display, 
    				data,
    			 	&formatheader,
    				&formatlength,
				0,
    				XM_FORMAT_HEADER_TYPE ) == ClipboardSuccess )
    {
	formatheader->itemPrivateId = private_id;

	if (formatheader->cutByNameFlag)
	    formatheader->itemLength = length;
	else
	    formatheader->itemLength = formatheader->itemLength + length;

    	_XmClipboardRetrieveItem( display, 
    			           formatheader->formatDataId,
    				   length,
    				   0,
    			           &formatdataptr,
    			           &formatdatalength,
				   &format,
				   0,
    				   formatheader->cutByNameFlag ); 
					    /* if cut by  */
					    /* name, discard any old data */

    	formatheader->cutByNameFlag = 0;
    	
        to_ptr = AddAddresses(formatdataptr, formatdatalength-length ); 

	/* copy the format data over to acquired storage
	*/
	bcopy( buffer, to_ptr, length );

	/* create the property on the root window for the format data
	*/
	_XmClipboardReplaceItem( display, 
				  formatheader->formatDataId,
				  formatdataptr,
				  formatdatalength,
    				  PropModeReplace,
				  format,
    				  True );

	/* change the property on the root window for the format item
	*/
	_XmClipboardReplaceItem( display, 
				  data,
				  formatheader,
				  formatlength,
    				  PropModeReplace,
				  32,
    				  True );
    }

    if ( locked )
    {
        _XmClipboardUnlock( display, window, 0 );
    }

    _XmClipboardFreeAlloc( root_clipboard_header );

    return ClipboardSuccess;
}


/*---------------------------------------------*/
#ifdef _NO_PROTO
int XmClipboardUndoCopy( display, window )
Display *display;
Window  window;
#else /* _NO_PROTO */
int XmClipboardUndoCopy(Display *display, Window window)
#endif /* _NO_PROTO */
{
    ClipboardHeader header;
    ClipboardDataItem itemheader;
    int itemlength;
    itemId itemid;
    int status, undo_okay;

    status = _XmClipboardLock( display, window );
    if ( status == ClipboardLocked ) return ClipboardLocked;

    /* note: second call to undo, undoes the first call */

    /* get the clipboard header
    */
    header = _XmClipboardOpen( display, 0 );

    itemid = header->lastCopyItemId;

    undo_okay = 0;

    if ( itemid == 0 )
    {
    	undo_okay = 1;

    }else{
	/* get the item 
	*/
	_XmClipboardFindItem( display,
			       itemid,
			       &itemheader,
			       &itemlength,
			       0,
    			       XM_DATA_ITEM_RECORD_TYPE );

	/* if no last copy item */
	/* or if the item's window or display don't match, then can't undo
	*/
	if ( itemheader->windowId == window  &&
	     itemheader->displayId == display ) 
	{
    	    undo_okay = 1;

	    /* mark last item for delete
	    */
    	    _XmClipboardMarkItem( display, header, itemid, XM_DELETE );
    	}

    	_XmClipboardFreeAlloc( (char*)itemheader );
    }

    if ( undo_okay )
    {
	/* fetch the item marked deleted by the last copy, if any
	*/
	itemid = header->deletedByCopyId;

	/* mark it undeleted
	*/
	_XmClipboardMarkItem( display, header, itemid, XM_UNDELETE );

	/* switch item marked deleted 
	*/
	header->deletedByCopyId = header->lastCopyItemId;
	header->lastCopyItemId = itemid;

	/* switch next paste and old next paste
	*/
	itemid = header->oldNextPasteItemId;
	header->oldNextPasteItemId = header->nextPasteItemId;
	header->nextPasteItemId = itemid;
    }

    _XmClipboardClose( display, header );

    _XmClipboardUnlock( display, window, 0 );

    return ClipboardSuccess;
}

/*---------------------------------------------*/
#ifdef _NO_PROTO
int XmClipboardLock( display, window )
Display *display;
Window window;     /* identifies application owning lock */
#else /* _NO_PROTO */
int XmClipboardLock(Display *display, Window window)
#endif /* _NO_PROTO */
{
    return _XmClipboardLock( display, window );
}


/*---------------------------------------------*/
#ifdef _NO_PROTO
int XmClipboardUnlock( display, window, all_levels )
Display *display;
Window window;		/* specifies window owning lock, must match window */
    			/* passed to clipboardlock */
Boolean all_levels;
#else /* _NO_PROTO */
int XmClipboardUnlock(Display *display, Window window, 
#if NeedWidePrototypes
int all_levels
#else
Boolean all_levels
#endif 
)
#endif /* _NO_PROTO */
{
    return _XmClipboardUnlock( display, window, all_levels );
}


/*---------------------------------------------*/
#ifdef _NO_PROTO
int XmClipboardStartRetrieve( display, window, timestamp )

Display		*display;    /* Display id of application wanting data */
Window		window;
Time		timestamp;
#else /* _NO_PROTO */
int XmClipboardStartRetrieve(Display *display, Window window, Time timestamp)
#endif /* _NO_PROTO */
{
    ClipboardHeader header;
    int status;

    status = _XmClipboardLock( display, window );
    if ( status == ClipboardLocked ) return ClipboardLocked;

    header = _XmClipboardOpen( display, 0 );

    header->incrementalCopyFrom = True;
    header->copyFromTimestamp = timestamp;
    header->foreignCopiedLength = 0;

    _XmClipboardClose( display, header );

    return ClipboardSuccess;
}

/*---------------------------------------------*/
#ifdef _NO_PROTO
int XmClipboardEndRetrieve( display, window )

Display		*display;    /* Display id of application wanting data */
Window		window;
#else /* _NO_PROTO */
int XmClipboardEndRetrieve (Display *display, Window window)
#endif /* _NO_PROTO */
{
    ClipboardHeader header;

    header = _XmClipboardOpen( display, 0 );

    header->incrementalCopyFrom = False;
    header->copyFromTimestamp   = CurrentTime;

    _XmClipboardClose( display, header );

    _XmClipboardUnlock( display, window, 0 );

    return ClipboardSuccess;
}

/*---------------------------------------------*/
#ifdef _NO_PROTO
int XmClipboardRetrieve( display, window, 
    			  format, buffer, length, outlength, private_id )

Display		*display;    /* Display id of application wanting data */
Window		window;
char		*format;    /* Name string for data format */
char		*buffer;    /* Address of buffer to receive data in this format */
unsigned long	length;     /* Length of the data buffer */
unsigned long	*outlength;  /* Length of the data transferred to buffer */
int		*private_id;    /* Private id provide by application */
#else /* _NO_PROTO */
int XmClipboardRetrieve(Display *display, Window window, char *format,
	char *buffer, unsigned long length, unsigned long *outlength,
	int *private_id) 
#endif /* _NO_PROTO */
{
    ClipboardHeader header;
    ClipboardFormatItem matchformat;
    char *formatdata;
    int formatdatalength, matchformatlength;
    int truncate, maxname, count;
    itemId matchid;
    char *ptr;
    int status, dataok;
    int loc_outlength;
    int loc_private;
    int copiedlength, remaininglength;
    Time timestamp;
    
    status = _XmClipboardLock( display, window );
    if ( status == ClipboardLocked ) return ClipboardLocked;

    /* get the clipboard header
    */
    header = _XmClipboardOpen( display, 0 );
    timestamp = header->copyFromTimestamp;

    loc_outlength = 0;
    loc_private = 0;
    truncate = 0;
    dataok = 0;

    /* check to see if we need to reclaim the selection
    */
    _XmInitializeSelection( display, header, window, timestamp );

    /* get the data from clipboard or selection owner
    */
    if ( _XmWeOwnSelection( display, header ) )
    {
	/* we own the selection
	*/
	/* find the matching format for the next paste item
	*/
	matchformat = _XmClipboardFindFormat( display, header, format, 0, NULL,
					       &maxname, &count, 
					       &matchformatlength );
	if (matchformat != 0)
	{
	    dataok = 1;

	    matchid = matchformat->thisFormatId;

	    if ( matchformat->cutByNameFlag == 1 )
	    {
		/* passed by name 
		*/
		dataok = _XmClipboardRequestDataAndWait( display, 
							  window,
							  matchformat );
		if ( dataok )
		{
		    /* re-check out matchformat since it may have changed
		    */
		    _XmClipboardFreeAlloc( matchformat );

		    _XmClipboardFindItem( display,
					   matchid,
					   &matchformat,
					   &matchformatlength,
					   0,
					   XM_FORMAT_HEADER_TYPE );
		}
	    }

	    if ( dataok )
	    {
		_XmClipboardFindItem( display,
				       matchformat->formatDataId,
				       &formatdata,
				       &formatdatalength,
				       0,
				       0 );

		copiedlength = matchformat->copiedLength;
		ptr = formatdata + copiedlength;

		remaininglength = formatdatalength - copiedlength;

		if ( length < remaininglength )
		{
		    loc_outlength = length;
		    truncate = 1;
		}else{
		    loc_outlength = remaininglength;
		}

		if ( header->incrementalCopyFrom )
		{
		    /* update the copied length
		    */
		    if ( loc_outlength == remaininglength )
		    {
			/* we've copied everything, so reset
			*/
			matchformat->copiedLength = 0;
		    }else{
			matchformat->copiedLength = matchformat->copiedLength
						  + loc_outlength;
		    }
		}

		loc_private = matchformat->itemPrivateId;
	    }

	    _XmClipboardReplaceItem( display, 
				      matchid,
				      matchformat,
				      matchformatlength,
				      PropModeReplace,
				      32,
				      True );
	}

    }else{

	/* we don't own the selection, get the data from selection owner
	*/
        if ( _XmClipboardGetSelection( display, window, format, header,
                                        &formatdata, &loc_outlength ) )
	{
	    /* we're okay
	    */
		dataok = 1;

		copiedlength = header->foreignCopiedLength;

		ptr = formatdata + copiedlength;

		remaininglength = loc_outlength - copiedlength;

		if ( length < remaininglength )
		{
		    loc_outlength = length;
		    truncate = 1;
		}else{
		    loc_outlength = remaininglength;
		}

		if ( header->incrementalCopyFrom )
		{
		    /* update the copied length
		    */
		    if ( loc_outlength == remaininglength )
		    {
			/* we've copied everything, so reset
			*/
			header->foreignCopiedLength = 0;

		    }else{

			header->foreignCopiedLength = 
						header->foreignCopiedLength
						  + loc_outlength;
		    }
		}
	    }
    }

    if ( dataok )
    {
	/* copy the data to the user buffer
	*/
	bcopy( ptr, buffer, loc_outlength );

	_XmClipboardFreeAlloc( formatdata );
    }

    /* try to prevent access violation even if outlength is mandatory 
    */
    if ( outlength != 0 )
    {
    	*outlength = loc_outlength;
    }

    if ( private_id != 0 )
    {    	
	*private_id = loc_private;
    }

    _XmClipboardClose( display, header );

    _XmClipboardUnlock( display, window, 0 );

    if (truncate == 1) return ClipboardTruncate;
    if (dataok == 0)   return ClipboardNoData;    

    return ClipboardSuccess;
}

/*---------------------------------------------*/
#ifdef _NO_PROTO
int XmClipboardInquireCount( display, window, count, maxlength )
Display *display;
Window  window;
int     *count;     /* receives number of formats in next paste item */
int     *maxlength; /* receives max length of format names for next paste item */
#else /* _NO_PROTO */
int XmClipboardInquireCount (Display *display, Window window, int *count,
	int *maxlength)
#endif /* _NO_PROTO */
{
    ClipboardHeader header;
    char *alloc_to_free;
    int status, loc_maxlength, loc_count, loc_matchlength;
    Time timestamp;

    status = _XmClipboardLock( display, window );
    if ( status == ClipboardLocked ) return ClipboardLocked;

    /* get the clipboard header
    */
    header = _XmClipboardOpen( display, 0 );

    /* If StartRetrieve wasn't called use latest Timestamp from server */

    if ( header->copyFromTimestamp == CurrentTime )
    {
        timestamp = _XmClipboardGetCurrentTime(display);
    }else{
        timestamp = header->copyFromTimestamp;
    }

    /* check to see if we need to reclaim the selection
    */
    _XmInitializeSelection( display, header, window, timestamp );
    loc_maxlength = 0;
    loc_count = 0;

    /* do we own the selection?
    */
    if ( _XmWeOwnSelection( display, header ) )
    {
	/* yes, find the next paste item, only looking for maxlength and count
	*/
	alloc_to_free = (char*)_XmClipboardFindFormat( display, header, 0, 
							NULL, NULL,
							&loc_maxlength, 
							&loc_count,
							&loc_matchlength );
    }else{
	/* we don't own the selection, get the data from selection owner
	*/
        if ( !_XmClipboardGetSelection( display, window, "TARGETS", header,
                                         &alloc_to_free, &loc_count ) )
        {
	    _XmClipboardClose( display, header );
	    _XmClipboardUnlock( display, window, 0 );
            return ClipboardNoData;

        }else{

            /* we obtained a TARGETS type selection conversion
            */
            Atom *atomptr;
            int i;

            atomptr   = (Atom*)alloc_to_free;

            /* returned count is in bytes, targets are atoms of length 4 bytes
            */
            loc_count = loc_count / 4;

	    /* max the lengths of all the atom names
	    */
	    for( i = 0; i < loc_count; i++ )
	    {
		    int temp;

		    temp = strlen( XmGetAtomName( display, *atomptr ));

		    if ( temp > loc_maxlength) 
		    {
			loc_maxlength = temp;
		    }

		    atomptr = atomptr + 1;
	     }
	}
    }	

    if ( maxlength != 0 )
    {
	/* user asked for max length of available format names
	*/
	*maxlength = loc_maxlength;
    }

    if ( count != 0 )
    {
	*count = loc_count;
    }

    if ( alloc_to_free != 0 )
    {
	_XmClipboardFreeAlloc( alloc_to_free ); 
    }

    _XmClipboardClose( display, header );

    _XmClipboardUnlock( display, window, 0 );

    return ClipboardSuccess;
}


/*---------------------------------------------*/
#ifdef _NO_PROTO
int XmClipboardInquireFormat( display, window, n, buffer,
    				       bufferlength, outlength)
Display		*display;      /* Display id of application inquiring */
Window		window;
int		n;	       /* Which format for this data item? */
char		*buffer;       /* Address of buffer to receive format name */
unsigned long	bufferlength;  /* Length of the buffer */
unsigned long	*outlength;    /* Receives length copied to name buffer */
#else /* _NO_PROTO */
int XmClipboardInquireFormat (Display *display, Window window, int n,
	char *buffer, unsigned long bufferlength, unsigned long *outlength)
#endif /* _NO_PROTO */
{
    ClipboardHeader header;
    ClipboardFormatItem matchformat;
    char *alloc_to_free;
    char *ptr;
    int count, maxname, loc_outlength, loc_matchlength;
    int status;

    status = _XmClipboardLock( display, window );
    if ( status == ClipboardLocked ) return ClipboardLocked;

    status = ClipboardSuccess;

    /* get the clipboard header
    */
    header = _XmClipboardOpen( display, 0 );

    /* check to see if we need to reclaim the selection
    */
    _XmInitializeSelection( display, header, window, 
			     header->copyFromTimestamp );

    ptr = 0;
    loc_outlength = 0;

    /* do we own the selection?
    */
    if ( _XmWeOwnSelection( display, header ) )
    {
	/* retrieve the matching format 
	*/
	matchformat = _XmClipboardFindFormat( display, header, 0, NULL, n,
					       &maxname, &count, 
					       &loc_matchlength );

	if ( matchformat != 0 )
	{
	    ptr = XmGetAtomName( display, matchformat->formatNameAtom ); 

            _XmClipboardFreeAlloc( matchformat );
	}

    }else{
        /* we don't own the selection, get the data from selection owner
        */
        if ( !_XmClipboardGetSelection( display, window, "TARGETS", header,
                                         &alloc_to_free, &loc_matchlength ) )
        {
            *outlength = 0;
	    _XmClipboardClose( display, header );
	    _XmClipboardUnlock( display, window, 0 );
            return ClipboardNoData;

        }else{
            /* we obtained a TARGETS type selection conversion
            */
            Atom *nth_atom;

            nth_atom = (Atom*)alloc_to_free;

            /* returned count is in bytes, targets are atoms of length 4 bytes
            */
            loc_matchlength = loc_matchlength / 4;

            if ( loc_matchlength >= n )
            {
                nth_atom = nth_atom + n - 1;

                ptr = XmGetAtomName( display, *nth_atom );

                _XmClipboardFreeAlloc( alloc_to_free );
            }
	}
    }

    if ( ptr != 0 )
    {
	loc_outlength = strlen( ptr );

	if ( loc_outlength > bufferlength )
	{
	    status = ClipboardTruncate;

	    loc_outlength = bufferlength;
	}

	strncpy( buffer, ptr, loc_outlength );

	_XmClipboardFreeAlloc( ptr );
    }

    if ( outlength != 0 )
    {
    	*outlength = loc_outlength;
    }

    _XmClipboardClose( display, header );

    _XmClipboardUnlock( display, window, 0 );

    return status;
}


/*---------------------------------------------*/
#ifdef _NO_PROTO
int XmClipboardInquireLength( display, window, format, length )
Display		*display; /* Display id of application inquiring */
Window		window;
char		*format; /* Name string for data format */
unsigned long	*length;  /* Receives length of the data in that format */
#else /* _NO_PROTO */
int XmClipboardInquireLength (Display *display, Window window, char *format,
	unsigned long *length)
#endif /* _NO_PROTO */
{

    ClipboardHeader header;
    ClipboardFormatItem matchformat;
    char *alloc_to_free;
    int maxname, count, loc_length, loc_matchlength;
    int status;

    status = _XmClipboardLock( display, window );
    if ( status == ClipboardLocked ) return ClipboardLocked;

    /* get the clipboard header
    */
    header = _XmClipboardOpen( display, 0 );

    /* check to see if we need to reclaim the selection
    */
    _XmInitializeSelection( display, header, window, 
			     header->copyFromTimestamp );

    loc_length = 0;

    /* do we own the selection?
    */
    if ( _XmWeOwnSelection( display, header ) )
    {
	/* retrieve the next paste item
	*/
	matchformat = _XmClipboardFindFormat( display, header, format, 0, NULL,
					       &maxname, &count, &loc_matchlength );

	/* return the length 
	*/
	if ( matchformat != 0 )
	{
	    loc_length = matchformat->itemLength;
	    _XmClipboardFreeAlloc( matchformat );
	}
    }else{
        /* we don't own the selection, get the data from selection owner
        */
        if ( !_XmClipboardGetSelection( display, window, format, header,
                                         &alloc_to_free, &loc_length ) )
        {
	    _XmClipboardClose( display, header );
	    _XmClipboardUnlock( display, window, 0 );
            return ClipboardNoData;

        }else{

            _XmClipboardFreeAlloc( alloc_to_free );
	}
    }

    if ( length != 0 )
    {
    	*length = loc_length;
    }

    _XmClipboardClose( display, header );

    _XmClipboardUnlock( display, window, 0 );

    return ClipboardSuccess;
}


/*---------------------------------------------*/
#ifdef _NO_PROTO
int XmClipboardInquirePendingItems( display, window, format, list, count)
Display		*display; /* Display id of application passing data */
Window		window;
char		*format;  /* Name string for data format */
XmClipboardPendingList *list;
unsigned long	*count;   /* Number of items in returned list */
#else /* _NO_PROTO */
int XmClipboardInquirePendingItems (Display *display, Window window,
	char *format, XmClipboardPendingList *list, unsigned long *count)
#endif /* _NO_PROTO */
{
    ClipboardHeader header;
    ClipboardFormatItem matchformat;
    XmClipboardPendingList itemlist, nextlistptr;
    itemId *id_ptr;
    int loc_count, i, loc_matchlength;
    int maxname;
    int status;

    status = _XmClipboardLock( display, window );
    if ( status == ClipboardLocked ) return ClipboardLocked;

    if ( list == 0 )
    {
    	/* just get out to avoid access violation
    	*/
	_XmClipboardUnlock( display, window, 0 );
        return ClipboardSuccess;
    }

    *list = 0;
    loc_count = 0;

    /* get the clipboard header
    */
    header = _XmClipboardOpen( display, 0 );

    id_ptr = (itemId*)AddAddresses( header, header->dataItemList );

    itemlist = (XmClipboardPendingList)_XmClipboardAlloc( 
    			(header->currItems) * sizeof( XmClipboardPendingRec ));

    nextlistptr = itemlist;

    /* run through all the items in the clipboard looking for matching formats 
    */
    for ( i = 0; i < header->currItems; i++ )
    {
    	/* if it is marked for delete, skip it
    	*/
    	if ( _XmClipboardIsMarkedForDelete( display, header, *id_ptr ) )
    	{
    	    matchformat = 0;
    	}else{
    	    int dummy;

	    /* see if there is a matching format
	    */
	    matchformat = _XmClipboardFindFormat( display, header, 
						   format, *id_ptr, 
						   NULL, &maxname, &dummy,
    						   &loc_matchlength );
    	}

    	if ( matchformat != 0 )
    	{
    	    /* found matching format
    	    */
    	    if ( matchformat->cutByNameFlag == 1 )
    	    {
    		/* it was passed by name so is pending
    		*/
    	    	nextlistptr->DataId = matchformat->thisFormatId;    	    
    	    	nextlistptr->PrivateId = matchformat->itemPrivateId;

    		nextlistptr = nextlistptr + 1;
    		loc_count = loc_count + 1;
    	    }

    	    _XmClipboardFreeAlloc( matchformat );
    	}

    	id_ptr = id_ptr + 1;
    }

    _XmClipboardClose( display, header );

    _XmClipboardUnlock( display, window, 0 );

    if ( count != 0 )
    {
        *count = loc_count;
    }

    *list  = itemlist;

    return ClipboardSuccess;
}


/*---------------------------------------------*/
#ifdef _NO_PROTO
int XmClipboardRegisterFormat( display, format_name, format_length )
Display		*display; /* Display id of application passing data */
char		*format_name;  /* Name string for data format */
unsigned long	format_length; /* Format length  8-16-32 */
#else /* _NO_PROTO */
int XmClipboardRegisterFormat (Display *display, char *format_name,
	unsigned long format_length)
#endif /* _NO_PROTO */
{
    if ( format_length != 0 && 
	 format_length != 8 && format_length != 16 && format_length != 32 )
    {
    	_XmClipboardError( "ClipboardBadFormat",
		"Error - registered format length must be 8, 16, or 32" );
	return ClipboardBadFormat;
    }

    if ( format_name == 0  ||  strlen( format_name ) == 0 )
    {
    	_XmClipboardError( "ClipboardBadFormat",
		"Error - registered format name must be non-null" );
	return ClipboardBadFormat;
    }

    /* make sure predefined formats are registered */
    /* use dummy format as a test, if not found then register the rest
    */
    if ( format_length != 0 )
    {
	return _XmRegisterFormat( display, format_name, format_length );

    }else{
      /* caller asking to look through predefines for format name 
      */
      if (
	_XmRegIfMatch( display, format_name, "TARGETS",	XM_ATOM )
	 )  return ClipboardSuccess;
      if (
	_XmRegIfMatch( display, format_name, "MULTIPLE",	XM_ATOM_PAIR )
	 )  return ClipboardSuccess;
      if (
	_XmRegIfMatch( display, format_name, "TIMESTAMP",	XM_INTEGER )
	 )  return ClipboardSuccess;
      if (
	_XmRegIfMatch( display, format_name, "STRING",		XM_STRING )
	 )  return ClipboardSuccess;
      if (
	_XmRegIfMatch( display, format_name, "LIST_LENGTH",	XM_INTEGER )
	 )  return ClipboardSuccess;
      if (
	_XmRegIfMatch( display, format_name, "PIXMAP",		XM_DRAWABLE )
	 )  return ClipboardSuccess;
      if (
	_XmRegIfMatch( display, format_name, "DRAWABLE",	XM_DRAWABLE )
	 )  return ClipboardSuccess;
      if (
	_XmRegIfMatch( display, format_name, "BITMAP",		XM_BITMAP )
	 )  return ClipboardSuccess;
      if (
	_XmRegIfMatch( display, format_name, "FOREGROUND",	XM_PIXEL )
	 )  return ClipboardSuccess;
      if (
	_XmRegIfMatch( display, format_name, "BACKGROUND",	XM_PIXEL )
	 )  return ClipboardSuccess;
      if (
	_XmRegIfMatch( display, format_name, "COLORMAP",	XM_COLORMAP )
	 )  return ClipboardSuccess;
      if (
	_XmRegIfMatch( display, format_name, "ODIF",		XM_TEXT )
	 )  return ClipboardSuccess;
      if (
	_XmRegIfMatch( display, format_name, "OWNER_OS",	XM_TEXT )
	 )  return ClipboardSuccess;
      if (
	_XmRegIfMatch( display, format_name, "FILE_NAME",	XM_TEXT )
	 )  return ClipboardSuccess;
      if (
	_XmRegIfMatch( display, format_name, "HOST_NAME",	XM_TEXT )
	 )  return ClipboardSuccess;
      if (
	_XmRegIfMatch( display, format_name, "CHARACTER_POSITION", XM_SPAN )
	 )  return ClipboardSuccess;
      if (
	_XmRegIfMatch( display, format_name, "LINE_NUMBER",	XM_SPAN )
	 )  return ClipboardSuccess;
      if (
	_XmRegIfMatch( display, format_name, "COLUMN_NUMBER",	XM_SPAN )
	 )  return ClipboardSuccess;
      if (
	_XmRegIfMatch( display, format_name, "LENGTH",		XM_INTEGER )
	 )  return ClipboardSuccess;
      if (
	_XmRegIfMatch( display, format_name, "USER",		XM_TEXT )
	 )  return ClipboardSuccess;
      if (
	_XmRegIfMatch( display, format_name, "PROCEDURE",	XM_TEXT )
	 )  return ClipboardSuccess;
      if (
	_XmRegIfMatch( display, format_name, "MODULE",		XM_TEXT )
	 )  return ClipboardSuccess;
      if (
	_XmRegIfMatch( display, format_name, "PROCESS",	XM_INTEGER )
	 )  return ClipboardSuccess;
      if (
	_XmRegIfMatch( display, format_name, "TASK",		XM_INTEGER )
	 )  return ClipboardSuccess;
      if (
	_XmRegIfMatch( display, format_name, "CLASS",		XM_TEXT )
	 )  return ClipboardSuccess;
      if (
	_XmRegIfMatch( display, format_name, "NAME",		XM_TEXT )
	 )  return ClipboardSuccess;
      if (
	_XmRegIfMatch( display, format_name, "CLIENT_WINDOW",	XM_WINDOW )
	 )  return ClipboardSuccess;
    }

    return ClipboardFail;
}

