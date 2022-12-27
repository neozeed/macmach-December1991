#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)TextIn.c	3.41.1.4 91/03/12";
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
*       THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED
*  AND COPIED ONLY IN ACCORDANCE WITH THE TERMS OF SUCH LICENSE AND
*  WITH THE INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR
*  ANY OTHER COPIES THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE
*  AVAILABLE TO ANY OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF THE
*  SOFTWARE IS HEREBY TRANSFERRED.
*
*       THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE WITHOUT
*  NOTICE AND SHOULD NOT BE CONSTRUED AS A COMMITMENT BY OPEN SOFTWARE
*  FOUNDATION, INC. OR ITS THIRD PARTY SUPPLIERS
*
*       OPEN SOFTWARE FOUNDATION, INC. AND ITS THIRD PARTY SUPPLIERS,
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
/* Private definitions. */

#include <Xm/TextP.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/Shell.h>
#include <X11/ShellP.h>
#include <X11/Vendor.h>
#include <X11/VendorP.h>
#include <Xm/AtomMgr.h>


typedef struct {
    XmTextPosition position;
    int length;
    Time time;
} PrimSelect;

typedef struct {
    Boolean has_destination;
    XmTextPosition position;
    int replace_length;
    Boolean quick_key;
} TextDestDataRec, *TextDestData;

static XContext textDestContext = NULL;

static XmTextScanType sarray[] = {
    XmSELECT_POSITION, XmSELECT_WORD, XmSELECT_LINE, XmSELECT_ALL
};

static int sarraysize = XtNumber(sarray);

static void _MovePreviousLine();
static void _MoveNextLine();
static void LoseSel2();
extern Boolean _XmTextSetDestinationSelection();

static XtResource input_resources[] = {
    {
      XmNselectionArray, XmCSelectionArray, XmRPointer,
      sizeof(caddr_t),
      XtOffset(InputData, sarray),
      XmRPointer, (caddr_t) sarray
    },

    {
      XmNselectionArrayCount, XmCSelectionArrayCount, XmRInt, sizeof(int),
      XtOffset(InputData, sarraycount),
      XmRInt, (caddr_t) &sarraysize
   },

    {
      XmNpendingDelete, XmCPendingDelete, XmRBoolean, sizeof(Boolean),
      XtOffset(InputData, pendingdelete),
      XmRImmediate, (caddr_t) True
    },

    {
      XmNselectThreshold, XmCSelectThreshold, XmRInt, sizeof(int),
      XtOffset(InputData, threshold),
      XmRImmediate, (caddr_t) 5
    },

};


static TextDestData GetTextDestData(tw)
Widget tw;
{
   TextDestData dest_data;

   if (textDestContext == NULL)
      textDestContext = XUniqueContext();

   if (XFindContext(XtDisplay(tw), (Window) XA_SECONDARY,
		    textDestContext, (caddr_t*)&dest_data)) {
       dest_data = (TextDestData)XtCalloc(sizeof(TextDestDataRec), 1);

       XSaveContext(XtDisplay(tw), (Window) XA_SECONDARY,
		    textDestContext, dest_data);
   }
		
   return dest_data;
}

static Boolean NeedsPendingDelete(tw)
XmTextWidget tw;
{
  InputData data = tw->text.input->data;
  XmTextPosition left, right;

  return (data->pendingdelete && 
     ((*tw->text.source->GetSelection)(tw->text.source, &left, &right)
      && left != right && left <= tw->text.cursor_position &&
      right >= tw->text.cursor_position));
}


/* ARGSUSED */
static void CheckSync(w, junk, event)
Widget w;
Opaque junk;
XEvent *event;
{
    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;
    XEvent ev2;
    Boolean onewaiting;

    if (XPending(XtDisplay(tw))) {
	XPeekEvent(XtDisplay(tw), &ev2);
	onewaiting = (ev2.xany.type == KeyPress &&
		      ev2.xany.window == XtWindow(tw));
    } else onewaiting = FALSE;
    if (data->syncing) {
	if (!onewaiting) {
	    data->syncing = FALSE;
	    _XmTextEnableRedisplay(tw);
	}
    } else {
	if (onewaiting) {
	    data->syncing = TRUE;
	    _XmTextDisableRedisplay(tw, FALSE);
	}
    }
}

/* ARGSUSED */
static void RingBell(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;
    if (tw->text.verify_bell)
       XBell(XtDisplay(tw), 0);
}

/* ARGSUSED */
static void InsertSelection(w, closure, seltype, type, value, length, format)
Widget w;
Opaque closure;
Atom *seltype;
Atom *type;
char *value;
int *length;
int *format;
{
    XmAnyCallbackStruct cb;
    Boolean *select_done = (Boolean *)closure;
    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;
    XmTextPosition left = 0;
    XmTextPosition right = 0;
    XmTextPosition replace_left, replace_right;
    XmTextPosition old_cursorPos;
    Boolean dest_disjoint = True;
    XmTextBlockRec block;
    XSelectionRequestEvent *req_event;

    if (!value) {
       select_done[0] = True;
       return;
    }

  /* Don't do replace if there is not text to add */
    if (*value == NULL || *length == 0){
       XtFree(value);
       select_done[0] = True;
       return;
    }

    block.ptr = value;
    block.length = *length;
    block.format = (int) *type;

    replace_left = replace_right = tw->text.dest_position;

    if ((*tw->text.source->GetSelection)
                (tw->text.source, &left, &right) && left != right) {
       if (replace_left >= left && replace_right <= right
           && data->pendingdelete) {
          replace_left = left;
          replace_right = right;
          dest_disjoint = False;
       }
    }

    req_event = XtGetSelectionRequest(w, *seltype, NULL);
    if ((*tw->text.source->Replace)(tw->text.source, (XEvent *) req_event,
			 replace_left, replace_right, &block) != EditDone) {
        if (tw->text.verify_bell) XBell(XtDisplay(tw), 0);
        select_done[1] = False;
    } else {
       /*
        * Call ValueChanged Callback to indicate that
        * text has been modified.
        */
        cb.reason = XmCR_VALUE_CHANGED;
        cb.event = (XEvent *) req_event;
        XtCallCallbackList ((Widget) tw, tw->text.value_changed_callback,
                           (Opaque) &cb);
        select_done[1] = True;

        _XmTextSetDestinationSelection((Widget)tw,
				       replace_left + *length,
			               False, CurrentTime);

        if (left != right) {
           if (!dest_disjoint) {
              data->anchor = tw->text.dest_position;
              (*tw->text.source->SetSelection)(tw->text.source,
					           data->anchor, data->anchor,
                                                   CurrentTime);
           } else {
              if (!tw->text.add_mode) {
                 data->anchor = tw->text.dest_position;
                 (*tw->text.source->SetSelection)(tw->text.source,
					              data->anchor,
						      data->anchor,
                                                      CurrentTime);
              }
           }
        } else {
           data->anchor = tw->text.dest_position;
        }
    }

    XtFree(value);
    select_done[0] = True;
}


/*
 * Converts requested target of insert selection.
 */
/*--------------------------------------------------------------------------+*/
static Boolean ConvertDestInsertSelection(w, selection, type,
                                          value, length, format)
/*--------------------------------------------------------------------------+*/
Widget w;
Atom *selection, *type;
caddr_t *value;
int *length, *format;
{
   XtAppContext app = XtWidgetToApplicationContext(w);
   XSelectionRequestEvent * req_event;
   static unsigned long old_serial = 0;
   Atom actual_type;
   int actual_format;
   unsigned long nitems;
   unsigned long bytes;
   unsigned char *prop;
   Boolean select_done[2];
   TextInsertPair *pair;

   select_done[0] = False;
   select_done[1] = False;

   req_event = XtGetSelectionRequest(w, *selection, NULL);

  /* Work around for intrinsics selection bug */
   if (old_serial != req_event->serial)
      old_serial = req_event->serial;
   else
      return False;

   XGetWindowProperty(req_event->display, req_event->requestor,
                      req_event->property, 0L, 10000000, False,
                      AnyPropertyType, &actual_type, &actual_format,
                      &nitems, &bytes, &prop);

   if (prop == NULL)
	return FALSE;

   pair = (TextInsertPair *)prop;

  /*
   * Make selection request to replace the primary selection
   * with the insert selection.
   */
   XtGetSelectionValue(w, pair->selection, pair->target, InsertSelection,
                       (Opaque)select_done, req_event->time);
  /*
   * Make sure the above selection request is completed
   * before returning from the convert proc.
   */
   for (;;) {
       XEvent event;

       if (select_done[0])
          break;
       XtAppNextEvent(app, &event);
       XtDispatchEvent(&event);
   }

   *type = XmInternAtom(XtDisplay(w), "INSERT_SELECTION", False);
   *format = 8;
   *value = NULL;
   *length = 0;

   XtFree((char *)prop);
   return (select_done[1]);
}


static Boolean ConvertDest(w, selection, target, type, value, length, format)
Widget w;
Atom *selection;
Atom *target;
Atom *type;
caddr_t *value;
int *length;
int *format;
{
    XmTextWidget tw = (XmTextWidget) w;
    Atom XA_TARGETS = XmInternAtom(XtDisplay(w), "TARGETS", False);
    Atom MOTIF_DESTINATION = XmInternAtom(XtDisplay(w),
			                "MOTIF_DESTINATION", False);
    Atom XA_INSERT_SELECTION = XmInternAtom(XtDisplay(w),
                                           "INSERT_SELECTION", False);
    Atom XA_TIMESTAMP = XmInternAtom(XtDisplay(w), "TIMESTAMP", False);

    if (*selection != MOTIF_DESTINATION) return FALSE;

   /*
    * XA_TARGETS identifies what targets the text widget can
    * provide data for.
    */
    if (*target == XA_TARGETS) {
      Atom *targs = (Atom *)XtMalloc((unsigned) (3 * sizeof(Atom)));

      *value = (caddr_t)targs;
      *targs++ = XA_TARGETS;
      *targs++ = XA_INSERT_SELECTION;
      *targs++ = XA_TIMESTAMP;
      *type = XA_TARGETS;
      *length = (3*sizeof(Atom)) >> 2; /*convert to work count */
      
    } else if (*target ==  XA_TIMESTAMP) {
      InputData data = tw->text.input->data;
      Time *timestamp;
      timestamp = (Time *) XtMalloc(sizeof(Time));
      *timestamp = data->dest_time;
      *value = (char *) timestamp;
      *type = XA_TIMESTAMP;
      *length = sizeof(Time);
      *format = 32;

   /*
    * Provide data for XA_INSERT_SELECTION requests
    */
    } else if (*target == XA_INSERT_SELECTION)
        return (ConvertDestInsertSelection(w, selection, type,
                                           value, length, format));

    return TRUE;
}


/* ARGSUSED */
static void LoseDestination(w, selection)
Widget w;
Atom *selection;
{
    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;

     _XmTextClearDestination(tw, True);
    data->has_destination = False;
}

static Boolean Convert2(w, seltype, target, type, value, length, format)
Widget w;
Atom *seltype;
Atom *target;
Atom *type;
caddr_t *value;
int *length;
int *format;
{
    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;
    Atom XA_TARGETS = XmInternAtom(XtDisplay(tw), "TARGETS", False);
    Atom XA_TIMESTAMP = XmInternAtom(XtDisplay(w), "TIMESTAMP", False);
    Atom XA_TEXT = XmInternAtom(XtDisplay(tw), "TEXT", False);
    XmTextBlockRec block;
    XmTextPosition pos;
    char *buf;
    int i;
    if (*seltype != XA_SECONDARY) return FALSE;
    if (*target == XA_STRING || *target == XA_TEXT) {
      *type = (Atom) FMT8BIT;
      *format = 8;
      if (!data->hasSel2) 
	return FALSE;
      *length = data->sel2Right - data->sel2Left;
      buf = XtMalloc((unsigned) *length + 1);
      pos = data->sel2Left;    
      i = 0;
      while(pos < data->sel2Right){
	  pos=(*tw->text.source->ReadSource)(tw->text.source, 	
		pos, data->sel2Right, &block);
         (void) strncpy(&buf[i], block.ptr, block.length);
	  i += block.length;
      }
      *value = buf;
      return TRUE;
    } else if (*target ==  XA_TIMESTAMP) {
      Time *timestamp;
      timestamp = (Time *) XtMalloc(sizeof(Time));
      *timestamp = data->sec_time;
      *value = (char *) timestamp;
      *type = XA_TIMESTAMP;
      *length = sizeof(Time);
      *format = 32;
    } else if (*target ==  XA_TARGETS) {
       /* I do hope that TARGETS gets built it... */
       Atom *a = (Atom *)XtMalloc((unsigned) (4*sizeof(Atom)));
       *value = (caddr_t)a;
       *a++ = XA_STRING;
       *a++ = XA_TEXT;
       *a++ = XA_TIMESTAMP;
       *a = XA_TARGETS;
       *length = (4*sizeof(Atom)) >> 2; /*convert to work count */
       *type = XA_TARGETS; /* or do we need a "list of atoms" type? */
       *format = 32;
       return TRUE;
    } else return FALSE;
}	

Boolean _XmTextSetDestinationSelection(w, position, disown, set_time)
Widget w;
XmTextPosition position;
Boolean disown;
Time set_time;
{
    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;
    Boolean result = TRUE;
    Atom MOTIF_DESTINATION = XmInternAtom(XtDisplay(w),
			                "MOTIF_DESTINATION", False);
    if (!XtIsRealized (w))
       return (False);
    if (!disown) {
        if (data->has_destination && 
	    (tw->text.dest_position != tw->text.cursor_position ||
            !tw->text.output->data->hasfocus))
            _XmTextClearDestination(tw, False);
        if (!data->has_destination) {
            result = XtOwnSelection(w, MOTIF_DESTINATION, set_time,
				    ConvertDest, LoseDestination,
				    (XtSelectionDoneProc) NULL);
	    data->dest_time = set_time;
	    data->has_destination = result;

	    if (result) _XmSetDestination(XtDisplay(w), w); 
	    /* Call XmGetDestination(dpy) to get widget that last had 
	       destination cursor. */
        }
        tw->text.dest_position = position;
        if (data->has_destination && 
	    (tw->text.dest_position != tw->text.cursor_position ||
            !tw->text.output->data->hasfocus))
            _XmTextDrawDestination(tw);
    } else {
	XtDisownSelection(w, MOTIF_DESTINATION, set_time);

        (*tw->text.output->DrawInsertionPoint)(tw, tw->text.cursor_position,
					       off);
	if (w == XmGetDestination(XtDisplay(w)))
	  _XmSetDestination(XtDisplay(w), (Widget)NULL);

        data->has_destination = False;
    }

    return result;
}

static Boolean DeleteOrKill(tw, event, from, to, kill)
XmTextWidget tw;
XEvent *event;
XmTextPosition from, to;
Boolean kill;
{
    XmTextBlockRec text;
    char *ptr;
    _XmTextDisableRedisplay(tw,False);
    if (kill && from < to) {

	ptr = _XmStringSourceGetString(tw, from, to);
        XRotateBuffers(XtDisplay(tw), 1);
	XStoreBuffer(XtDisplay(tw), ptr, strlen(ptr), 0);
	XtFree(ptr);
    }
    text.length = 0;
    if (EditDone != (*tw->text.source->Replace)(tw->text.source, event,
						 from, to, &text)) {
	_XmTextEnableRedisplay(tw);
	RingBell((Widget)tw, (XEvent *) NULL, (char **) NULL, (Cardinal) 0);
	return FALSE;
    } else {
        XmTextPosition top = XmTextGetTopCharacter((Widget)tw);

      tw->text.needs_redisplay = tw->text.needs_refigure_lines = True;
	_XmTextEnableRedisplay(tw);
        _XmTextSetDestinationSelection((Widget)tw, tw->text.cursor_position,
			               False, event->xkey.time);
	return TRUE;
    }
}


static void StuffFromBuffer(tw, event, buffer)
XmTextWidget tw;
XEvent *event;
int buffer;
{
    XmTextPosition cursorPos;
    XmTextBlockRec text;
    XmAnyCallbackStruct cb;

    cursorPos = XmTextGetCursorPosition((Widget)tw);
    text.ptr = XFetchBuffer(XtDisplay(tw), &(text.length), buffer);
    if ((*tw->text.source->Replace)(tw->text.source, event, cursorPos,
                                     cursorPos, &text) != EditDone) {
	RingBell((Widget)tw, (XEvent *) NULL, (char **) NULL, (Cardinal) 0);
	return;
    } else
        _XmTextSetDestinationSelection((Widget)tw, cursorPos,
				       False, event->xkey.time);

    cursorPos = (*tw->text.source->Scan)(tw->text.source, 
    	cursorPos, XmSELECT_POSITION, XmsdRight, text.length, TRUE);
    _XmTextSetCursorPosition((Widget)tw, cursorPos);

    /* Call ValueChanged Callback to indicate that text has been modified */
    cb.reason = XmCR_VALUE_CHANGED;
    cb.event = event;
    XtCallCallbackList ((Widget) tw, tw->text.value_changed_callback,
		       (Opaque) &cb);

    XtFree(text.ptr);
}

/* ARGSUSED */
static void UnKill(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;

    if (tw->text.output->data->dest_visible) {
       _XmTextDestinationVisible(w, False);
       data->changed_dest_visible = True;
    }
    StuffFromBuffer(tw, event, 0);
    
    if (data->changed_dest_visible) {
       _XmTextDestinationVisible(w, True);
       data->changed_dest_visible = False;
    }

}

/* ARGSUSED */
static void RemoveCurrentSelection(w, event, params, num_params, kill)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
Boolean kill;
{
    XmTextWidget tw = (XmTextWidget) w;
    XmAnyCallbackStruct cb;
    XmTextPosition cursorPos, left, right;
    if (!(*tw->text.source->GetSelection)
			(tw->text.source, &left, &right)) {
        XBell(XtDisplay(tw), 0);
    } else {
        if (left < right) {
	   cursorPos = XmTextGetCursorPosition((Widget)tw);
           (*tw->text.source->SetSelection)(tw->text.source, cursorPos,
					         cursorPos, event->xkey.time);
	   if (DeleteOrKill(tw, event, left, right, kill)) {
	      if (cursorPos > left && cursorPos < right) {
		  _XmTextSetCursorPosition((Widget)tw, left);
                  _XmTextSetDestinationSelection(w, left, False,
						 event->xkey.time);
              }

             /* Call ValueChanged Callback to indicate
              * that text has been modified.
              */
              cb.reason = XmCR_VALUE_CHANGED;
              cb.event = event;
    	      XtCallCallbackList ((Widget) tw,
	 		     tw->text.value_changed_callback, (Opaque) &cb);

	   } else 
              (*tw->text.source->SetSelection)(tw->text.source, left,
					           right, event->xkey.time);
        }
    }
}

static void DeleteCurrentSelection(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    RemoveCurrentSelection(w, event, params, num_params, FALSE);
}


static void KillCurrentSelection(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    RemoveCurrentSelection(w, event, params, num_params, TRUE);
}


static void CheckDisjointSelection(w, position, sel_time)
Widget w;
XmTextPosition position;
Time sel_time;
{
    XmTextWidget tw = (XmTextWidget) w;
    XmTextPosition left, right;
    InputData data = tw->text.input->data;

    if (!tw->text.add_mode && (*tw->text.source->GetSelection)
	(tw->text.source, &left, &right) &&
        position >= left && position <= right)
       tw->text.pendingoff = FALSE;
    else
       tw->text.pendingoff = TRUE;

    if (left == right) {
        _XmTextSetDestinationSelection(w, position, False, sel_time);
       data->anchor = position;
    } else {
       _XmTextSetDestinationSelection(w, position, False, sel_time);
       if (!tw->text.add_mode) data->anchor = position;
    }
}


static void SelfInsert(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;
    char str[32];
    int n;
    XmTextPosition cursorPos;
    XmTextBlockRec block;
    XmAnyCallbackStruct cb;
    OutputData o_data = tw->text.output->data;
    XFontStruct *font = o_data->font;

    n = XLookupString((XKeyEvent *)event, str, sizeof(str)-1,
		      (KeySym *)NULL, &(data->compstatus));

    if (n > 0) {
	str[n] = '\0';
	if (tw->text.output->data->dest_visible) {
	   _XmTextDestinationVisible(w, False);
	   data->changed_dest_visible = True;
	}
        if (XTextWidth(font, str, n)==0 && strchr(str, '\t')==0)
                return;
	if (NeedsPendingDelete(tw))
	    DeleteCurrentSelection(w, event, params, num_params);
        cursorPos = XmTextGetCursorPosition((Widget)tw);
	block.ptr = str;
	block.length = n;
	block.format = FMT8BIT;
	if ((*tw->text.source->Replace)(tw->text.source, event, cursorPos,
				       cursorPos, &block) != EditDone) {
	    RingBell(w, event, params, num_params);
	} else {
	    cursorPos = (*(tw->text.source->Scan))(tw->text.source,
                           cursorPos, XmSELECT_POSITION, XmsdRight, n, TRUE);
            CheckDisjointSelection(tw, cursorPos, event->xkey.time);
	    _XmTextSetCursorPosition((Widget)tw, cursorPos);

        /* Call ValueChanged Callback to indicate that text has been modified */
    	    cb.reason = XmCR_VALUE_CHANGED;
    	    cb.event = event;
    	    XtCallCallbackList ((Widget) tw,
 			     tw->text.value_changed_callback, (Opaque) &cb);

	}
	
	if (data->changed_dest_visible) {
	   _XmTextDestinationVisible((Widget)w, True);
	   data->changed_dest_visible = False;
	}

    }
}


static void InsertString(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;
    XmTextPosition cursorPos;
    XmAnyCallbackStruct cb;
    char *str;
    int i;
    XmTextBlockRec block;


    if (tw->text.output->data->dest_visible) {
       _XmTextDestinationVisible(w, False);
       data->changed_dest_visible = True;
    }
    _XmTextDisableRedisplay(tw, TRUE);
    if (NeedsPendingDelete(tw))
	DeleteCurrentSelection(w, event, params, num_params);
    cursorPos = XmTextGetCursorPosition((Widget)tw);
    for (i=0 ; i<*num_params ; i++) {
	str = params[i];
	block.ptr = str;
	block.length = strlen(str);
	block.format = FMT8BIT;
	if ((*tw->text.source->Replace)(tw->text.source, event, cursorPos,
				       cursorPos, &block) != EditDone) {
	    RingBell(w, event, params, num_params);
	    break;
	} else {
	    cursorPos = (*(tw->text.source->Scan))(tw->text.source,
					 cursorPos, XmSELECT_POSITION,
					 XmsdRight, block.length, TRUE);
            _XmTextSetDestinationSelection(w, cursorPos,
					   False, event->xkey.time);
	}
    }
    CheckDisjointSelection(tw, cursorPos, event->xkey.time);
    _XmTextSetCursorPosition((Widget)tw, cursorPos);

    /* Call ValueChanged Callback to indicate that text has been modified */
    cb.reason = XmCR_VALUE_CHANGED;
    cb.event = event;
    XtCallCallbackList ((Widget) tw, tw->text.value_changed_callback,
		       (Opaque) &cb);

    _XmTextEnableRedisplay(tw);
    
    if (data->changed_dest_visible) {
       _XmTextDestinationVisible(w, True);
       data->changed_dest_visible = False;
    }

}

static void ProcessVerticalParams(w, event, params, num_params,
			          left, right, position, bal_point)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
XmTextPosition *left;
XmTextPosition *right;
XmTextPosition *position;
float *bal_point;
{
    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;

    if (!strcmp(*params, "up"))
        _MovePreviousLine(w, event, params, num_params, False);
    else if (!strcmp(*params, "down"))
        _MoveNextLine(w, event, params, num_params, False);

    data->selectionHint.x = data->selectionHint.y = 0;
    data->extending = TRUE;

    _XmTextDisableRedisplay(tw, FALSE);

    *position = (*tw->text.source->Scan)(tw->text.source,
				        XmTextGetCursorPosition((Widget)tw),
			 	        XmSELECT_POSITION, XmsdRight, 1, False);

    if (!(*tw->text.source->GetSelection)
		(tw->text.source, left, right) || *left == *right) {
          if (data->anchor > *position) {
             data->origRight = *right = data->anchor;
             data->origLeft = *left = *position;
          } else {
             data->origLeft = *left = data->anchor;
             data->origRight = *right = *position;
          }
        *bal_point = data->anchor;
    } else
        *bal_point = (float)(((float)(*right - *left) / 2.0) + (float)*left);


}


/* ARGSUSED */
static void ProcessHorizontalParams(w, event, params, num_params, left,
                                  right, position, cursorPos, bal_point)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
XmTextPosition *left;
XmTextPosition *right;
XmTextPosition *position;
XmTextPosition *cursorPos;
float *bal_point;
{
    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;
    XmTextPosition old_cursorPos = XmTextGetCursorPosition((Widget)tw);

    data->selectionHint.x = data->selectionHint.y = 0;
    data->extending = TRUE;

    _XmTextDisableRedisplay(tw, FALSE);

    *position = *cursorPos = (*tw->text.source->Scan)(tw->text.source,
                                      XmTextGetCursorPosition((Widget)tw),
                                      XmSELECT_POSITION, XmsdRight, 1, False);

    if (!(*tw->text.source->GetSelection)
		(tw->text.source, left, right) || *left == *right) {
        data->origLeft = data->origRight = data->anchor;
        *left = *right = old_cursorPos;
        *bal_point = data->anchor;
    } else {
        *bal_point = (float)(((float)(*right - *left) / 2.0) + (float)*left);

       /* special case for single character selection case */
        if (*right - *left == 1) {
           if (!strcmp(*params,"right") && old_cursorPos < *bal_point)
              *bal_point = (*position) + 1;
           else if (!strcmp(*params,"left") && old_cursorPos > *bal_point)
              *bal_point = (*position) - 1;
        }
    }

    /* move text cursor in direction of cursor key */
    if (!strcmp(*params,"right")) {
       (*position)++;
    } else if (!strcmp(*params,"left"))
       (*position)--;
}


/* ARGSUSED */
static void ProcessSelectParams(w, event, left, right, position, bal_point)
Widget w;
XEvent *event;
XmTextPosition *left;
XmTextPosition *right;
XmTextPosition *position;
float *bal_point;
{
  XmTextWidget tw = (XmTextWidget) w;
  InputData data = tw->text.input->data;

  data->selectionHint.x = data->selectionHint.y = 0;
  data->extending = TRUE;

  _XmTextDisableRedisplay(tw, FALSE);

  *position = XmTextGetCursorPosition(w);

  if (!((*tw->text.source->GetSelection)(tw->text.source, left, right))
      || *left == *right) {
        data->anchor = tw->text.dest_position;
        *left = *right = *position;
        data->origLeft = data->origRight = data->anchor;
        *bal_point = data->anchor;
  } else
        *bal_point = (float)(((float)(*right - *left) / 2.0) + (float)*left);


}


static void KeySelection(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextPosition position, left, right, cursorPos;
    XmTextPosition old_position;
    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;
    float bal_point;

    XmTextScanDirection  cursorDir;                              /* PIR1858 */
    XmTextPosition       tempIndex;                              /* PIR1858 */


    if (tw->text.output->data->dest_visible) {
       _XmTextDestinationVisible(w, False);
       data->changed_dest_visible = True;
    }
    /* reset origLeft and origRight */
    (*tw->text.source->GetSelection)(tw->text.source,
                                    &(data->origLeft), &(data->origRight));

    if (*num_params == 0) {
       ProcessSelectParams(w, event, &left, &right, &position, &bal_point);
       cursorPos = position;
     
    } else if (!strcmp(*params, "up") || !strcmp(*params, "down")) {
       ProcessVerticalParams(w, event, params, num_params,
			     &left, &right, &position, &bal_point);
       cursorPos = position;

    } else if (!strcmp(*params, "right") || !strcmp(*params, "left")) {
       ProcessHorizontalParams(w, event, params, num_params, &left,
                             &right, &position, &cursorPos, &bal_point);
       cursorPos = position;
    }

 /* shift anchor and direction to opposite end of the selection */
    if ((float)position < bal_point) {
       data->extendDir = XmsdLeft;
       data->anchor = data->origRight;
    } else if ((float)position > bal_point) {
       data->extendDir = XmsdRight;
       data->anchor = data->origLeft;
    }

    if (position < 0 || position > tw->text.last_position) {
       _XmTextEnableRedisplay(tw);
       return;
    }

#ifdef NOT_DEF
    if (cursorPos < data->anchor)
       data->extendDir = XmsdLeft;
    else if (cursorPos > data->anchor)
       data->extendDir = XmsdRight;

    /* check for change in extend direction */
    if ((data->extendDir == XmsdRight && position < data->anchor) ||
          (data->extendDir == XmsdLeft && position > data->anchor)) {
        data->extendDir =
            (data->extendDir == XmsdRight) ? XmsdLeft : XmsdRight;
        left = data->origLeft;
        right = data->origRight;
    }
#endif /* NOT_DEF */

    cursorDir = data->extendDir;                                            /* PIR1858 */
    if (data->extendDir == XmsdRight) {
          if ( cursorPos < right )  /* We are backtracking  */              /* PIR1858 */
              cursorDir = XmsdLeft;                                         /* PIR1858 */
                                                                            /* PIR1858 */
          right = cursorPos = (*tw->text.source->Scan)(tw->text.source,
				        position, data->stype, cursorDir,   /* PIR1858 */
					1, FALSE);
          left = data->anchor;
    } else {
          if ( cursorPos > left )  /* We are backtracking */                /* PIR1858 */
              cursorDir = XmsdRight;                                        /* PIR1858 */
                                                                            /* PIR1858 */
          left = cursorPos = (*tw->text.source->Scan)(tw->text.source,
				        position, data->stype, cursorDir,   /* PIR1858 */
					1, FALSE);
          right = data->anchor;
    }
                                                                            /* PIR1858 */
    if ( left > right )     /* we are on other side of anchor */            /* PIR1858 */
    {                       /* (probably due to backtrack)    */            /* PIR1858 */
        tempIndex = left;                                                   /* PIR1858 */
        left      = right;                                                  /* PIR1858 */
        right     = tempIndex;                                              /* PIR1858 */
    }                                                                       /* PIR1858 */

    (*tw->text.source->SetSelection)(tw->text.source, left, right,
				    event->xkey.time);
    tw->text.pendingoff = FALSE;
    _XmTextSetDestinationSelection(w, cursorPos, False, event->xkey.time);
    _XmTextSetCursorPosition((Widget)tw, cursorPos);
    XmTextShowPosition((Widget)tw, (XmTextPosition) -1);

    if (tw->text.auto_show_cursor_position &&
        cursorPos == tw->text.bottom_position)
        (*tw->text.output->MakePositionVisible)((Widget)tw, cursorPos);

    _XmTextEnableRedisplay(tw);

    /* reset origLeft and origRight */
    (*tw->text.source->GetSelection)(tw->text.source,
                                    &(data->origLeft), &(data->origRight));
    data->extending = FALSE;
    
    if (data->changed_dest_visible) {
       _XmTextDestinationVisible(w, True);
       data->changed_dest_visible = False;
    }

}

static void SimpleMovement(tw, event, dir, type, include)
XmTextWidget tw;
XEvent *event;
XmTextScanDirection dir;
XmTextScanType type;
Boolean include;
{
    XmTextPosition position, cursorPos;
    XKeyEvent *k_event = (XKeyEvent *) event;
    XmTextPosition left, right;
    InputData data = tw->text.input->data;

    _XmTextDestinationVisible((Widget)tw, False);
    cursorPos = XmTextGetCursorPosition((Widget)tw);
    position = (*tw->text.source->Scan)(tw->text.source, cursorPos,
                                            type, dir, 1, include);

    if ((tw->text.add_mode && (*tw->text.source->GetSelection)
         (tw->text.source, &left, &right) &&
         position >= left && position <= right) ||
        k_event->state & ShiftMask)

       tw->text.pendingoff = FALSE;
    else
       tw->text.pendingoff = TRUE;

    if (!tw->text.add_mode) {
        if (!(k_event->state & ShiftMask))
           data->anchor = position;
       /*
        * if doing selection, reset the destination position
        * prior to calling KeySelection() to avoid having the
        * anchor getting reset to the wrong position.
        */
        if (k_event->state & ShiftMask)
           _XmTextSetDestinationSelection((Widget)tw, cursorPos, False,
				          event->xkey.time);
    }

    _XmTextSetCursorPosition((Widget)tw, position);

    if (k_event->state & ShiftMask) {
       Cardinal num = 0;
       KeySelection((Widget)tw, event, NULL, &num);
    } else
       _XmTextDestinationVisible((Widget)tw, True);
}    


/* ARGSUSED */
static void MoveForwardChar(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    SimpleMovement((XmTextWidget) w, event, XmsdRight, XmSELECT_POSITION, TRUE);
}

/* ARGSUSED */
static void MoveBackwardChar(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    SimpleMovement((XmTextWidget) w, event, XmsdLeft, XmSELECT_POSITION, TRUE);
}

/* ARGSUSED */
static void MoveForwardWord(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;
    XmTextPosition position, cursorPos;
    XKeyEvent *k_event = (XKeyEvent *) event;
    XmTextPosition left, right;
    InputData data = tw->text.input->data;

    if (tw->text.output->data->dest_visible) {
       _XmTextDestinationVisible(w, False);
       data->changed_dest_visible = True;
    }
    cursorPos = XmTextGetCursorPosition((Widget)tw);
    position = (*tw->text.source->Scan)(tw->text.source, cursorPos,
                                        XmSELECT_WORD, XmsdRight, 1, TRUE);

    if ((tw->text.add_mode && (*tw->text.source->GetSelection)
         (tw->text.source, &left, &right) &&
         position >= left && position <= right) ||
         k_event->state & ShiftMask)
       tw->text.pendingoff = FALSE;
    else
       tw->text.pendingoff = TRUE;

    if (!tw->text.add_mode) {
        if (!(k_event->state & ShiftMask))
           data->anchor = position;
       /*
        * if doing selection, reset the destination position
        * prior to calling KeySelection() to avoid having the
        * anchor getting reset to the wrong position.
        */
        if (k_event->state & ShiftMask)
           _XmTextSetDestinationSelection((Widget)tw, cursorPos, False,
				          event->xkey.time);
    }

    _XmTextSetCursorPosition((Widget)tw, position);
    if (k_event->state & ShiftMask)
	   { /* This is a kludge to handle the extend param */
       Cardinal num = 0;
       KeySelection(w, event, params, &num);
       /*
       KeySelection(w, event, params, num_params);
       */
	   }
    else
       
       if (data->changed_dest_visible) {
          _XmTextDestinationVisible(w, True);
          data->changed_dest_visible = False;
       }

}

/* ARGSUSED */
static void MoveBackwardWord(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;
    XmTextPosition position, cursorPos;
    XKeyEvent *k_event = (XKeyEvent *) event;
    XmTextPosition left, right;
    InputData data = tw->text.input->data;

    if (tw->text.output->data->dest_visible) {
       _XmTextDestinationVisible(w, False);
       data->changed_dest_visible = True;
    }
    cursorPos = XmTextGetCursorPosition((Widget)tw);
    position = (*tw->text.source->Scan)(tw->text.source, cursorPos,
                                        XmSELECT_WORD, XmsdLeft, 1, FALSE);
    if(position == cursorPos){
        position = (*tw->text.source->Scan)(tw->text.source, position,
                                        XmSELECT_WORD, XmsdLeft, 1, TRUE);
        position = (*tw->text.source->Scan)(tw->text.source, position,
                                        XmSELECT_WORD, XmsdLeft, 1, FALSE);
    }
    if ((tw->text.add_mode && (*tw->text.source->GetSelection)
         (tw->text.source, &left, &right) &&
         position >= left && position <= right) ||
        k_event->state & ShiftMask)
       tw->text.pendingoff = FALSE;
    else
       tw->text.pendingoff = TRUE;

    if (!tw->text.add_mode) {
        if (!(k_event->state & ShiftMask))
           data->anchor = position;
       /*
        * if doing selection, reset the destination position
        * prior to calling KeySelection() to avoid having the
        * anchor getting reset to the wrong position.
        */
        if (k_event->state & ShiftMask)
           _XmTextSetDestinationSelection((Widget)tw, cursorPos, False,
				          event->xkey.time);
    }

    _XmTextSetCursorPosition((Widget)tw, position);

    if (k_event->state & ShiftMask)
	   { /* This is a kludge to handle the extend param */
       Cardinal num = 0;
       KeySelection(w, event, params, &num);
       /*
       KeySelection(w, event, params, num_params);
       */
	   }
    else
       
       if (data->changed_dest_visible) {
          _XmTextDestinationVisible(w, True);
          data->changed_dest_visible = False;
       }

}

/* ARGSUSED */
static void MoveForwardParagraph(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
   XmTextWidget tw = (XmTextWidget) w;

   if (tw->text.edit_mode == XmSINGLE_LINE_EDIT) return;
   SimpleMovement((XmTextWidget) w, event, XmsdRight, XmSELECT_PARAGRAPH, FALSE);
}

/* ARGSUSED */
static void MoveBackwardParagraph(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
   XmTextWidget tw = (XmTextWidget) w;

   if (tw->text.edit_mode == XmSINGLE_LINE_EDIT) return;
   SimpleMovement((XmTextWidget) w, event, XmsdLeft, XmSELECT_PARAGRAPH, FALSE);
}

static void MoveToLineStart(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;
    LineNum line;
    XmTextPosition position, cursorPos;
    XKeyEvent *k_event = (XKeyEvent *) event;
    XmTextPosition left, right;
    InputData data = tw->text.input->data;

    if (tw->text.output->data->dest_visible) {
       _XmTextDestinationVisible(w, False);
       data->changed_dest_visible = True;
    }
    cursorPos = XmTextGetCursorPosition((Widget)tw);
    XmTextShowPosition((Widget)tw, cursorPos);
    line = _XmTextPosToLine(tw, cursorPos);
    if (line == NOLINE) {
       XBell(XtDisplay(tw), 0);
       
       if (data->changed_dest_visible) {
          _XmTextDestinationVisible(w, True);
          data->changed_dest_visible = False;
       }

    } else {
	_XmTextLineInfo(tw, line, &position, (LineTableExtra *) NULL);
        if (tw->text.add_mode &&
            (*tw->text.source->GetSelection)
			    (tw->text.source, &left, &right) &&
            position >= left && position <= right)
           tw->text.pendingoff = FALSE;
        else
           tw->text.pendingoff = TRUE;

        if (!tw->text.add_mode) {
            data->anchor = position;
           /*
            * if doing selection, reset the destination position
            * prior to calling KeySelection() to avoid having the
            * anchor getting reset to the wrong position.
            */
            if (k_event->state & ShiftMask)
               _XmTextSetDestinationSelection((Widget)tw, cursorPos, False,
				              event->xkey.time);
        }

	_XmTextSetCursorPosition((Widget)tw, position);

       if (k_event->state & ShiftMask)
           { /* This is a kludge to handle the extend param */
	   Cardinal num = 0;
	   KeySelection(w, event, params, &num);
	   /*
	   KeySelection(w, event, params, num_params);
	   */
	   }
       else if (data->changed_dest_visible) {
	  _XmTextDestinationVisible(w, True);
	  data->changed_dest_visible = False;
       }
    }
}

static void MoveToLineEnd(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;
    LineNum line;
    XmTextPosition position, cursorPos;
    XKeyEvent *k_event = (XKeyEvent *) event;
    XmTextPosition left, right;
    InputData data = tw->text.input->data;

    if (tw->text.output->data->dest_visible) {
       _XmTextDestinationVisible(w, False);
       data->changed_dest_visible = True;
    }
    cursorPos = XmTextGetCursorPosition((Widget)tw);
    XmTextShowPosition((Widget)tw, cursorPos);
    line = _XmTextPosToLine(tw, cursorPos);
    if (line == NOLINE) {
       XBell(XtDisplay(tw), 0);
       
       if (data->changed_dest_visible) {
          _XmTextDestinationVisible(w, True);
          data->changed_dest_visible = False;
       }

    } else {
	_XmTextLineInfo(tw, line+1, &position, (LineTableExtra *) NULL);
	if (position == PASTENDPOS)
	  position = (*tw->text.source->Scan)(tw->text.source, position,
					      XmSELECT_ALL, XmsdRight, 1, TRUE);
	else
	  position = (*tw->text.source->Scan)(tw->text.source, position,
					       XmSELECT_POSITION, XmsdLeft,
					       1, TRUE);
        if (tw->text.add_mode &&
            (*tw->text.source->GetSelection)
			    (tw->text.source, &left, &right) &&
            position >= left && position <= right)
           tw->text.pendingoff = FALSE;
        else
           tw->text.pendingoff = TRUE;

        if (!tw->text.add_mode) {
            data->anchor = position;
           /*
            * if doing selection, reset the destination position
            * prior to calling KeySelection() to avoid having the
            * anchor getting reset to the wrong position.
            */
            if (k_event->state & ShiftMask)
               _XmTextSetDestinationSelection((Widget)tw, cursorPos, False,
				              event->xkey.time);
        }

	_XmTextSetCursorPosition((Widget)tw, position);

       if (k_event->state & ShiftMask)
	   { /* This is a kludge to handle the extend param */
	   Cardinal num = 0;
	   KeySelection(w, event, params, &num);
	   /*
	   KeySelection(w, event, params, num_params);
	   */
	   }
       else if (data->changed_dest_visible) {
	  _XmTextDestinationVisible(w, True);
	  data->changed_dest_visible = False;
       }
    }
}


static void _MoveNextLine(w, event, params, num_params, pendingoff)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
Boolean pendingoff;
{
    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;
    LineNum line;
    XmTextPosition currentPos, newPos, start, start2;
    XmTextPosition left, right;
    int col;

    if (tw->text.edit_mode == XmSINGLE_LINE_EDIT) return;
    if (tw->text.output->data->dest_visible) {
       _XmTextDestinationVisible(w, False);
       data->changed_dest_visible = True;
    }
    currentPos = XmTextGetCursorPosition((Widget)tw);
    XmTextShowPosition((Widget)tw, currentPos);
    line = _XmTextPosToLine(tw, currentPos);
    if (line == NOLINE) XBell(XtDisplay(tw), 0);
    else {
	_XmTextLineInfo(tw, line, &start, (LineTableExtra *) NULL);
	col = currentPos - start;	/* %%% needs reviewing */
	_XmTextLineInfo(tw, line+1, &start, (LineTableExtra *) NULL);
	if (start == PASTENDPOS)
	  newPos = (*tw->text.source->Scan)(tw->text.source, currentPos,
					     XmSELECT_ALL, XmsdRight, 1, TRUE);
	else
	  newPos = (*tw->text.source->Scan)(tw->text.source, start,
					     XmSELECT_POSITION, XmsdRight,
					     col, TRUE);

        if (tw->text.add_mode &&
            (*tw->text.source->GetSelection)
                (tw->text.source, &left, &right) &&
            newPos >= left && newPos <= right)
           tw->text.pendingoff = FALSE;
        else
           tw->text.pendingoff = pendingoff;

	_XmTextSetCursorPosition((Widget)tw, newPos);

	XmTextShowPosition((Widget)tw, newPos);
	line = _XmTextPosToLine(tw, newPos);
	if (line != NOLINE) {
	    _XmTextLineInfo(tw, line, &start2, (LineTableExtra *) NULL);
	    if (start2 != start && start != PASTENDPOS) {
                if (tw->text.add_mode &&
                    (*tw->text.source->GetSelection)
                        (tw->text.source, &left, &right) &&
                    start >= left && start <= right)
                   tw->text.pendingoff = FALSE;
                else
                   tw->text.pendingoff = pendingoff;

		_XmTextSetCursorPosition((Widget)tw, start);
		MoveToLineEnd(w, event, params, num_params);
	    }
	}
    }
    
    if (data->changed_dest_visible) {
       _XmTextDestinationVisible(w, True);
       data->changed_dest_visible = False;
    }

}

static void MoveNextLine(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;

    _MoveNextLine(w, event, params, num_params, True);
    if (!tw->text.add_mode) data->anchor = tw->text.cursor_position;
}

/* ARGSUSED */
static void _MovePreviousLine(w, event, params, num_params, pendingoff)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;
    LineNum line;
    XmTextPosition top, currentPos, newPos, origstart, start, start2;
    XmTextPosition left, right;
    int col;

    if (tw->text.edit_mode == XmSINGLE_LINE_EDIT) return;
    if (tw->text.output->data->dest_visible) {
       _XmTextDestinationVisible(w, False);
       data->changed_dest_visible = True;
    }
    _XmTextDisableRedisplay(tw, FALSE);
    top = XmTextGetTopCharacter((Widget)tw);
    currentPos = XmTextGetCursorPosition((Widget)tw);
    XmTextShowPosition((Widget)tw, currentPos);
    line = _XmTextPosToLine(tw, currentPos);
    if (line == NOLINE) {
	  _XmWarning(tw, "Can't find position in MovePreviousLine().");
	newPos = currentPos;
    } else {
	_XmTextLineInfo(tw, line, &origstart, (LineTableExtra *) NULL);
	col = currentPos - origstart;	/* %%% needs reviewing */
	if (line == 0) {
	    XmTextScroll((Widget)tw, -1);
	    line = _XmTextPosToLine(tw, currentPos);
	    if (line == 0) {
		newPos = (*tw->text.source->Scan)(tw->text.source,
				 currentPos, XmSELECT_ALL, XmsdLeft, 1, TRUE);
		goto done;
	    }
	    if (line == NOLINE) line = 1;
	}
	_XmTextLineInfo(tw, line-1, &start, (LineTableExtra *) NULL);
	newPos = (*tw->text.source->Scan)(tw->text.source, start,
					    XmSELECT_POSITION, XmsdRight,
					    col, TRUE);
	XmTextShowPosition((Widget)tw, newPos);
	line = _XmTextPosToLine(tw, newPos);
	if (line != NOLINE) {
	    _XmTextLineInfo(tw, line, &start2, (LineTableExtra *) NULL);
	    if (start2 != start) {
		newPos = (*tw->text.source->Scan)(tw->text.source,
			 origstart, XmSELECT_POSITION, XmsdLeft, 1, TRUE);
	    }
	}
    }
  done:
    XmTextSetTopCharacter((Widget)tw, top);
    if (tw->text.add_mode &&
        (*tw->text.source->GetSelection)
            (tw->text.source, &left, &right) &&
        newPos >= left && newPos <= right)
       tw->text.pendingoff = FALSE;
    else
       tw->text.pendingoff = pendingoff;

    _XmTextSetCursorPosition((Widget)tw, newPos);
    _XmTextEnableRedisplay(tw);
    
    if (data->changed_dest_visible) {
       _XmTextDestinationVisible(w, True);
       data->changed_dest_visible = False;
    }

}


static void MovePreviousLine(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;

    _MovePreviousLine(w, event, params, num_params, True);
    if (!tw->text.add_mode) data->anchor = tw->text.cursor_position;
}


/* ARGSUSED */
static void MoveNextPage(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;
    XmTextPosition cursorPos;
    XmTextPosition left, right;
    int n;
    if (tw->text.edit_mode == XmSINGLE_LINE_EDIT) return;
    if (tw->text.output->data->dest_visible) {
       _XmTextDestinationVisible(w, False);
       data->changed_dest_visible = True;
    }
    _XmTextDisableRedisplay(tw, FALSE);
    n = _XmTextNumLines(tw);
    if (n > 1) n--;

    XmTextScroll((Widget)tw, n);
    cursorPos = XmTextGetCursorPosition((Widget)tw);

    if (tw->text.add_mode &&
        (*tw->text.source->GetSelection)
            (tw->text.source, &left, &right) &&
        cursorPos >= left && cursorPos <= right)
       tw->text.pendingoff = FALSE;
    else
       tw->text.pendingoff = TRUE;

    _XmTextEnableRedisplay(tw);
    
    if (data->changed_dest_visible) {
       _XmTextDestinationVisible(w, True);
       data->changed_dest_visible = False;
    }

}



/* ARGSUSED */
static void MovePreviousPage(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;
    XmTextPosition cursorPos;
    XmTextPosition left, right;
    int n;
    if (tw->text.edit_mode == XmSINGLE_LINE_EDIT) return;
    if (tw->text.output->data->dest_visible) {
       _XmTextDestinationVisible(w, False);
       data->changed_dest_visible = True;
    }
    _XmTextDisableRedisplay(tw, FALSE);
    n = _XmTextNumLines(tw);
    if (n > 1) n--;

    XmTextScroll((Widget)tw, -n);
    cursorPos = XmTextGetCursorPosition((Widget)tw);

    if (tw->text.add_mode &&
        (*tw->text.source->GetSelection)
            (tw->text.source, &left, &right) &&
        cursorPos >= left && cursorPos <= right)
       tw->text.pendingoff = FALSE;
    else
       tw->text.pendingoff = TRUE;

    _XmTextEnableRedisplay(tw);
    
    if (data->changed_dest_visible) {
       _XmTextDestinationVisible(w, True);
       data->changed_dest_visible = False;
    }

}


/* ARGSUSED */
static void MovePageLeft(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;

    if (tw->text.output->data->dest_visible) {
       _XmTextDestinationVisible(w, False);
       data->changed_dest_visible = True;
    }
    _XmTextDisableRedisplay(tw, FALSE);
    _XmTextChangeHOffset(tw, -tw->text.inner_widget->core.width);
    _XmTextEnableRedisplay(tw);
    
    if (data->changed_dest_visible) {
       _XmTextDestinationVisible(w, True);
       data->changed_dest_visible = False;
    }

}


/* ARGSUSED */
static void MovePageRight(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;

    if (tw->text.output->data->dest_visible) {
       _XmTextDestinationVisible(w, False);
       data->changed_dest_visible = True;
    }
    _XmTextDisableRedisplay(tw, FALSE);
    _XmTextChangeHOffset(tw, tw->text.inner_widget->core.width);
    _XmTextEnableRedisplay(tw);
    
    if (data->changed_dest_visible) {
       _XmTextDestinationVisible(w, True);
       data->changed_dest_visible = False;
    }

}


/* ARGSUSED */
static void MoveBeginningOfFile(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;
    SimpleMovement(tw, event, XmsdLeft, XmSELECT_ALL, TRUE);
}



/* ARGSUSED */
static void MoveEndOfFile(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;
    SimpleMovement(tw, event, XmsdRight, XmSELECT_ALL, TRUE);
}
    



/* ARGSUSED */
static void ScrollOneLineUp(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;

    if (tw->text.output->data->dest_visible) {
       _XmTextDestinationVisible(w, False);
       data->changed_dest_visible = True;
    }
    XmTextScroll((Widget)tw, 1);
    
    if (data->changed_dest_visible) {
       _XmTextDestinationVisible(w, True);
       data->changed_dest_visible = False;
    }

}

/* ARGSUSED */
static void ScrollOneLineDown(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;

    if (tw->text.output->data->dest_visible) {
       _XmTextDestinationVisible(w, False);
       data->changed_dest_visible = True;
    }
    XmTextScroll((Widget)tw, -1);
    
    if (data->changed_dest_visible) {
       _XmTextDestinationVisible(w, True);
       data->changed_dest_visible = False;
    }

}

static void InsertNewLine(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;
    XmTextPosition cursorPos;
    XmTextBlockRec block;
    XmAnyCallbackStruct cb;
    char str[32];

    str[0] = '\n';
    str[1] = 0;
    block.length = 1;
    block.ptr = str;

    if (tw->text.output->data->dest_visible) {
       _XmTextDestinationVisible(w, False);
       data->changed_dest_visible = True;
    }
    if (NeedsPendingDelete(tw))
	DeleteCurrentSelection(w, event, params, num_params);
    cursorPos = XmTextGetCursorPosition((Widget)tw);
    if ((*tw->text.source->Replace)(tw->text.source, event, cursorPos,
				   cursorPos, &block) != EditDone) {
	RingBell(w, event, params, num_params);
    } else {
	cursorPos = (*(tw->text.source->Scan))(tw->text.source,
			 cursorPos, XmSELECT_LINE, XmsdRight, 1, TRUE);
        CheckDisjointSelection(tw, cursorPos, event->xkey.time);
	_XmTextSetCursorPosition((Widget)tw, cursorPos);

        /* Call ValueChanged Callback to indicate that text has been modified */
        cb.reason = XmCR_VALUE_CHANGED;
        cb.event = event;
        XtCallCallbackList ((Widget) tw, tw->text.value_changed_callback,
			   (Opaque)&cb);
    }
    
    if (data->changed_dest_visible) {
       _XmTextDestinationVisible(w, True);
       data->changed_dest_visible = False;
    }

}    

static void InsertNewLineAndBackup(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    InsertNewLine(w, event, params, num_params);
    MovePreviousLine(w, event, params, num_params);
    MoveToLineEnd(w, event, params, num_params);
}

static void InsertNewLineAndIndent(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;
    XmTextBlockRec block;
    XmTextPosition  pos, left, right;
    XmAnyCallbackStruct cb;

    _XmTextDisableRedisplay(tw, TRUE);
    pos = XmTextGetCursorPosition((Widget)tw);
    left = (*tw->text.source->Scan)(tw->text.source, pos,
 	    XmSELECT_LINE, XmsdLeft,  1, FALSE);
    if(left != (*tw->text.source->Scan)(tw->text.source, left,
                                    XmSELECT_WHITESPACE, XmsdRight, 1, FALSE)){
	InsertNewLine(w, event, params, num_params);
    } else {
        right = (*tw->text.source->Scan)(tw->text.source, left,
                                       XmSELECT_WHITESPACE, XmsdRight, 1, TRUE);
        if(right > pos) 
	    right = pos;
        InsertNewLine(w, event, params, num_params);
        block.format = FMT8BIT;
        pos = XmTextGetCursorPosition((Widget)tw);
        while(left < right){
            left=(*tw->text.source->ReadSource)(tw->text.source,
						 left, right, &block);
	    if ((*tw->text.source->Replace)(tw->text.source, event, pos,
				       pos, &block) != EditDone) {
	        RingBell(w, event, params, num_params);
	        break;
	    } else {
	        pos = (*(tw->text.source->Scan))(tw->text.source, pos,
						  XmSELECT_POSITION, XmsdRight,
						  block.length, TRUE);
	    }
        }
        CheckDisjointSelection(tw, pos, event->xkey.time);
        _XmTextSetCursorPosition((Widget)tw, pos);

        /* Call ValueChanged Callback to indicate that text has been modified */
        cb.reason = XmCR_VALUE_CHANGED;
        cb.event = event;
        XtCallCallbackList ((Widget) tw, tw->text.value_changed_callback,
			   (Opaque)&cb);
    }
    _XmTextEnableRedisplay(tw);
}

/* ARGSUSED */
static void RedrawDisplay(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;
    XmTextPosition top = XmTextGetTopCharacter((Widget)tw);
    _XmTextInvalidate(tw, top, top, NODELTA);
}

/* ARGSUSED */
static void Activate(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmAnyCallbackStruct cb;
    XmTextWidget tw = (XmTextWidget) w;
    XmParentProcessDataRec  p_event ;


    p_event.process_type = XmINPUT_ACTION ;
    p_event.data.input_action.action = XmRETURN ;
    p_event.data.input_action.event = event ;/* Pointer to XEvent. */
    p_event.data.input_action.params = NULL ; /* Or use what you have if   */
    p_event.data.input_action.num_params = 0 ;/* input is from translation.*/

    cb.reason = XmCR_ACTIVATE;
    cb.event  = event;
    XtCallCallbackList ((Widget) tw, tw->text.activate_callback,
		          (Opaque) &cb);

    (void) _XmParentProcess(XtParent(tw), &p_event);
}

/* ARGSUSED */
static void ToggleAddMode(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;
    XmTextPosition left, right;

    (*tw->text.output->DrawInsertionPoint)(tw, tw->text.cursor_position, off);
    tw->text.add_mode = !tw->text.add_mode;
    (*tw->text.output->DrawInsertionPoint)(tw, tw->text.cursor_position, on);


    if (tw->text.add_mode &&
        (!(*tw->text.source->GetSelection)(data->widget->text.source,
					    &left, &right) || left == right)) {
       data->anchor = tw->text.dest_position;
    }
}


/* ARGSUSED */
static void SetCursorPosition(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;

    if (tw->text.output->data->dest_visible) {
       _XmTextDestinationVisible(w, False);
       data->changed_dest_visible = True;
    }
    _XmTextSetCursorPosition((Widget)tw, (*tw->text.output->XYToPos)(tw,
					   event->xbutton.x, event->xbutton.y));
    
    if (data->changed_dest_visible) {
       _XmTextDestinationVisible(w, True);
       data->changed_dest_visible = False;
    }

}

static void RemoveBackwardChar(w, event, params, num_params, kill)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
Boolean kill;
{
    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;
    XmTextPosition cursorPos, pos2;
    XmAnyCallbackStruct cb;

    if (tw->text.output->data->dest_visible) {
       _XmTextDestinationVisible(w, False);
       data->changed_dest_visible = True;
    }
    if (NeedsPendingDelete(tw)) {
	DeleteCurrentSelection(w, event, params, num_params);
    } else {
        cursorPos = XmTextGetCursorPosition((Widget)tw);
	pos2 = (*tw->text.source->Scan)(tw->text.source, cursorPos,
				       XmSELECT_POSITION, XmsdLeft, 1, TRUE);
	if (DeleteOrKill(tw, event, pos2, cursorPos, kill)) {
           CheckDisjointSelection(tw, pos2, event->xkey.time);
	   _XmTextSetCursorPosition((Widget)tw, pos2);

          /* Call ValueChanged Callback to indicate
	   * that text has been modified.
           */
    	   cb.reason = XmCR_VALUE_CHANGED;
    	   cb.event = event;
           XtCallCallbackList ((Widget) tw,
			     tw->text.value_changed_callback, (Opaque) &cb);

        }
    }
    
    if (data->changed_dest_visible) {
       _XmTextDestinationVisible(w, True);
       data->changed_dest_visible = False;
    }

}

static void DeleteBackwardChar(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    RemoveBackwardChar(w, event, params, num_params, FALSE);
}

static void KillBackwardChar(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    RemoveBackwardChar(w, event, params, num_params, TRUE);
}


static void RemoveForwardWord(w, event, params, num_params, kill)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
Boolean kill;
{
    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;
    XmTextPosition left, right;
    XmAnyCallbackStruct cb;

    if (tw->text.output->data->dest_visible) {
       _XmTextDestinationVisible(w, False);
       data->changed_dest_visible = True;
    }
    if (NeedsPendingDelete(tw)) {
	DeleteCurrentSelection(w, event, params, num_params);
    } else {
        _XmTextDisableRedisplay(tw, TRUE);
        left = XmTextGetCursorPosition((Widget)tw);
        right = (*tw->text.source->Scan)(tw->text.source, left,
                                             XmSELECT_WORD, XmsdRight, 1, TRUE);

        if (left < right) {
           if (DeleteOrKill(tw, event, left, right, kill)) {
              CheckDisjointSelection(tw, left, event->xkey.time);
	      _XmTextSetCursorPosition((Widget)tw, left);
    
             /* Call ValueChanged Callback to indicate 
              * that text has been modified.
              */
              cb.reason = XmCR_VALUE_CHANGED;
              cb.event = event;
              XtCallCallbackList ((Widget) tw,
		       tw->text.value_changed_callback, (Opaque)&cb);

           }
        }
        _XmTextEnableRedisplay(tw);
   }
   
   if (data->changed_dest_visible) {
      _XmTextDestinationVisible(w, True);
      data->changed_dest_visible = False;
   }

}

static void DeleteForwardWord(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    RemoveForwardWord(w, event, params, num_params, FALSE);
}

static void KillForwardWord(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    RemoveForwardWord(w, event, params, num_params, TRUE);
}



static void RemoveBackwardWord(w, event, params, num_params, kill)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
Boolean kill;
{
    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;
    XmTextPosition left, right;
    XmAnyCallbackStruct cb;

    if (tw->text.output->data->dest_visible) {
       _XmTextDestinationVisible(w, False);
       data->changed_dest_visible = True;
    }
    if (NeedsPendingDelete(tw)) {
       DeleteCurrentSelection(w, event, params, num_params);
    } else {
       _XmTextDisableRedisplay(tw, TRUE);
       right = XmTextGetCursorPosition((Widget)tw);
       left = (*tw->text.source->Scan)(tw->text.source, right,
                                        XmSELECT_WORD, XmsdLeft, 1, FALSE);
       if (left == right){
          left = (*tw->text.source->Scan)(tw->text.source, left,
                                          XmSELECT_WORD, XmsdLeft, 1, TRUE);
          left = (*tw->text.source->Scan)(tw->text.source, left,
                                          XmSELECT_WORD, XmsdLeft, 1, FALSE);
       }
       if (left < right) {
          if (DeleteOrKill(tw, event, left, right, kill)) {
             CheckDisjointSelection(tw, left, event->xkey.time);
	     _XmTextSetCursorPosition((Widget)tw, left);
   
            /* Call ValueChanged Callback to indicate
	     * that text has been modified.
             */
             cb.reason = XmCR_VALUE_CHANGED;
             cb.event = event;
             XtCallCallbackList ((Widget) tw,
			      tw->text.value_changed_callback, (Opaque)&cb);

          }
       }
       _XmTextEnableRedisplay(tw);
   }
   
   if (data->changed_dest_visible) {
      _XmTextDestinationVisible(w, True);
      data->changed_dest_visible = False;
   }

}

static void DeleteBackwardWord(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    RemoveBackwardWord(w, event, params, num_params, FALSE);
}

static void KillBackwardWord(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    RemoveBackwardWord(w, event, params, num_params, TRUE);
}

static void RemoveForwardChar(w, event, params, num_params, kill)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
Boolean kill;
{
    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;
    XmTextPosition cursorPos, pos2;
    XmTextBlockRec block;
    XmAnyCallbackStruct cb;

    block.length = 0;
    if (tw->text.output->data->dest_visible) {
       _XmTextDestinationVisible(w, False);
       data->changed_dest_visible = True;
    }
    if (NeedsPendingDelete(tw)) {
	DeleteCurrentSelection(w, event, params, num_params);
    } else {
        cursorPos = XmTextGetCursorPosition((Widget)tw);
	pos2 = (*tw->text.source->Scan)(tw->text.source, cursorPos,
				       XmSELECT_POSITION, XmsdRight, 1, TRUE);
	if (DeleteOrKill(tw, event, cursorPos, pos2, kill)) {
           CheckDisjointSelection(tw, cursorPos, event->xkey.time);
	   _XmTextSetCursorPosition((Widget)tw, cursorPos);

          /* Call ValueChanged Callback to indicate
	   * that text has been modified.
           */
           cb.reason = XmCR_VALUE_CHANGED;
           cb.event = event;
           XtCallCallbackList ((Widget) tw,
			   tw->text.value_changed_callback, (Opaque)&cb);

       }
    }
    
    if (data->changed_dest_visible) {
       _XmTextDestinationVisible((Widget)w, True);
       data->changed_dest_visible = False;
    }

}

static void KillForwardChar(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    RemoveForwardChar(w, event, params, num_params, TRUE);
}

static void DeleteForwardChar(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    RemoveForwardChar(w, event, params, num_params, FALSE);
}

static void RemoveToEndOfLine(w, event, params, num_params, kill)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
Boolean kill;
{
    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;
    XmTextPosition left, right;
    XmAnyCallbackStruct cb;

    if (tw->text.output->data->dest_visible) {
       _XmTextDestinationVisible(w, False);
       data->changed_dest_visible = True;
    }
    _XmTextDisableRedisplay(tw, TRUE);
    left = XmTextGetCursorPosition((Widget)tw);
    MoveToLineEnd(w, event, params, num_params);
    right = XmTextGetCursorPosition((Widget)tw);
    if (left < right) {
	if (DeleteOrKill(tw, event, left, right, kill)) {
           CheckDisjointSelection(tw, left, event->xkey.time);
	   _XmTextSetCursorPosition((Widget)tw, left);
          /* Call ValueChanged Callback to indicate
	   * that text has been modified.
           */
           cb.reason = XmCR_VALUE_CHANGED;
           cb.event = event;
           XtCallCallbackList ((Widget) tw,
			 tw->text.value_changed_callback, (Opaque) &cb);

        }
    } else if (left == right)
	DeleteForwardChar(w, event, params, num_params);	

    _XmTextEnableRedisplay(tw);
    
    if (data->changed_dest_visible) {
       _XmTextDestinationVisible(w, True);
       data->changed_dest_visible = False;
    }

}

static void RemoveToStartOfLine(w, event, params, num_params, kill)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
Boolean kill;
{
    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;
    XmTextPosition left, right;
    XmAnyCallbackStruct cb;

    if (tw->text.output->data->dest_visible) {
       _XmTextDestinationVisible(w, False);
       data->changed_dest_visible = True;
    }
    _XmTextDisableRedisplay(tw, TRUE);
    right = XmTextGetCursorPosition((Widget)tw);
    MoveToLineStart(w, event, params, num_params);
    left = XmTextGetCursorPosition((Widget)tw);
    if (left < right) {
	if (DeleteOrKill(tw, event, left, right, kill)) {
           CheckDisjointSelection(tw, left, event->xkey.time);
	   _XmTextSetCursorPosition((Widget)tw, left);
          /* Call ValueChanged Callback to indicate
	   * that text has been modified.
           */
           cb.reason = XmCR_VALUE_CHANGED;
           cb.event = event;
           XtCallCallbackList ((Widget) tw,
			 tw->text.value_changed_callback, (Opaque) &cb);

        }
    } else if (left == right)
	DeleteBackwardChar(w, event, params, num_params);	

    _XmTextEnableRedisplay(tw);
    
    if (data->changed_dest_visible) {
       _XmTextDestinationVisible(w, True);
       data->changed_dest_visible = False;
    }

}

static void DeleteToStartOfLine(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    RemoveToStartOfLine(w, event, params, num_params, FALSE);
}

static void KillToStartOfLine(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    RemoveToStartOfLine(w, event, params, num_params, TRUE);
}

static void DeleteToEndOfLine(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    RemoveToEndOfLine(w, event, params, num_params, FALSE);
}

static void KillToEndOfLine(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    RemoveToEndOfLine(w, event, params, num_params, TRUE);
}

static void RestorePrimaryHighlight(data, prim_left, prim_right)
InputData data;
XmTextPosition prim_left;
XmTextPosition prim_right;
{
   if (data->sel2Right >= prim_left && data->sel2Right <= prim_right) {
     /* secondary selection is totally inside primary selection */
      if (data->sel2Left >= prim_left) {
         XmTextSetHighlight((Widget)data->widget, prim_left, data->sel2Left,
                            XmHIGHLIGHT_SELECTED);
         XmTextSetHighlight((Widget)data->widget, data->sel2Left, data->sel2Right,
                            XmHIGHLIGHT_NORMAL);
         XmTextSetHighlight((Widget)data->widget, data->sel2Right, prim_right,
                            XmHIGHLIGHT_SELECTED);
     /* right side of secondary selection is inside primary selection */
      } else {
         XmTextSetHighlight((Widget)data->widget, prim_left, data->sel2Right,
                            XmHIGHLIGHT_SELECTED);
         XmTextSetHighlight((Widget)data->widget, data->sel2Left, prim_left,
                            XmHIGHLIGHT_NORMAL);
      }
   } else {
     /* left side of secondary selection is inside primary selection */
      if (data->sel2Left <= prim_right && data->sel2Left >= prim_left) {
         XmTextSetHighlight((Widget)data->widget, data->sel2Left, prim_right,
                            XmHIGHLIGHT_SELECTED);
         XmTextSetHighlight((Widget)data->widget, prim_right, data->sel2Right,
                            XmHIGHLIGHT_NORMAL);
      } else  {
       /* secondary selection encompasses the primary selection */
        if (data->sel2Left <= prim_left && data->sel2Right >= prim_right){
           XmTextSetHighlight((Widget)data->widget, data->sel2Left, prim_left,
                              XmHIGHLIGHT_NORMAL);
           XmTextSetHighlight((Widget)data->widget, prim_left, prim_right,
                              XmHIGHLIGHT_SELECTED);
           XmTextSetHighlight((Widget)data->widget, prim_right, data->sel2Right,
                              XmHIGHLIGHT_NORMAL);
     /* secondary selection is outside primary selection */
        } else {
           XmTextSetHighlight((Widget)data->widget, prim_left, prim_right,
                              XmHIGHLIGHT_SELECTED);
           XmTextSetHighlight((Widget)data->widget, data->sel2Left, data->sel2Right,
                              XmHIGHLIGHT_NORMAL);
        }
      }
   }
}

static Boolean SetSel2(data, left, right, set_time)
InputData data;
XmTextPosition left;
XmTextPosition right;	/* if right == -999, then we're in LoseSelection,
			   so don't call XtDisownSelection. */
Time set_time;
{
    Boolean result = TRUE;

    _XmTextDisableRedisplay(data->widget, FALSE);
    if (data->hasSel2) {
       XmTextPosition prim_left, prim_right;

      /* If the widget has the primary selection, make sure the selection
       * highlight is restored appropriately.
       */
       if ((*data->widget->text.source->GetSelection)(data->widget->text.source,
					    &prim_left, &prim_right))
          RestorePrimaryHighlight(data, prim_left, prim_right);
       else
          XmTextSetHighlight((Widget)data->widget, data->sel2Left, data->sel2Right,
			   XmHIGHLIGHT_NORMAL);
    }

    if (left <= right) {
        result = XtOwnSelection(data->widget, XA_SECONDARY, set_time,
				Convert2, LoseSel2, (XtSelectionDoneProc) NULL);
	data->sec_time = set_time;
	data->hasSel2 = result;
	if (result) {
	    XmTextSetHighlight((Widget)data->widget, left, right,
			       XmHIGHLIGHT_SECONDARY_SELECTED);
	    data->sel2Left = left;
	    data->sel2Right = right;
	}
    } else {
	data->hasSel2 = FALSE;
	if (right != -999)
	    XtDisownSelection(data->widget, XA_SECONDARY, set_time);
    }
    _XmTextEnableRedisplay(data->widget);
    return result;
}

/* ARGSUSED */
static void LoseSel2(w, selection)
Widget w;
Atom *selection;
{

    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;
    if (data->hasSel2)
       (void) SetSel2(data, 1, -999, CurrentTime);
}
 

static Boolean GetSel2(data, left, right)
InputData data;
XmTextPosition *left, *right; 
{
    if (data->hasSel2 && data->sel2Left <= data->sel2Right) {
	*left = data->sel2Left;
	*right = data->sel2Right;
	return TRUE;
    } else {
        data->hasSel2 = FALSE;
        return FALSE;
    }
}

/* ARGSUSED */
static void SetSelectionHint(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;
    data->selectionHint.x = event->xbutton.x;
    data->selectionHint.y = event->xbutton.y;
}

/*
 * This routine implements multi-click selection in a hardwired manner.
 * It supports multi-click entity cycling (char, word, line, file) and mouse
 * motion adjustment of the selected entities (i.e. select a word then, with
 * button still down, adjust which word you really meant by moving the mouse).
 * [Note: This routine is to be replaced by a set of procedures that
 * will allows clients to implements a wide class of draw through and
 * multi-click selection user interfaces.]
 */
static void a_Selection(tw, x, y, sel_time)
XmTextWidget tw;
int x, y;
Time sel_time;
{
    InputData data = tw->text.input->data;
    XmTextPosition position, newLeft, newRight;

    _XmTextDisableRedisplay(tw, FALSE);
    position = (*tw->text.output->XYToPos)(tw, x, y);

    newLeft = (*tw->text.source->Scan)(tw->text.source, position,
			 data->stype, XmsdLeft, 1, FALSE);
    newRight = (*tw->text.source->Scan)(tw->text.source, position,
		 data->stype, XmsdRight, 1, data->stype == XmSELECT_LINE);
    (*tw->text.source->SetSelection)(tw->text.source, newLeft,
					 newRight, sel_time);
    tw->text.pendingoff = FALSE;
    if (position - newLeft < newRight - position) {
	_XmTextSetCursorPosition((Widget)tw, newLeft); 
	data->extendDir = XmsdLeft;
    } else {
	_XmTextSetCursorPosition((Widget)tw, newRight); 
	data->extendDir = XmsdRight;
    }
    XmTextShowPosition((Widget)tw, (XmTextPosition) -1);
    _XmTextEnableRedisplay(tw);
    data->origLeft = newLeft;
    data->origRight = newRight;
}

/* ARGSUSED */
static void SetAnchor(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;
    XmTextPosition left, right;

    data->anchor = XmTextGetCursorPosition(w);
    _XmTextSetDestinationSelection(w, data->anchor, False, event->xkey.time);
    if ((*tw->text.source->GetSelection)
                     (tw->text.source, &left, &right)) {
       (*tw->text.source->SetSelection)(tw->text.source, data->anchor,
					    data->anchor, event->xkey.time);
    }
}

/* ARGSUSED */
static void DoSelection(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;
    a_Selection(tw, event->xbutton.x, event->xbutton.y,
		event->xbutton.time);
}

static void SetScanType(w, data, event)
Widget w;
InputData data;
XEvent *event;
{
    int i;
    int multi_click_time;

    multi_click_time = XtGetMultiClickTime(XtDisplay(w));

    if (event->xbutton.time > data->lasttime &&
        event->xbutton.time - data->lasttime <
			 (multi_click_time == 200 ? 500 : multi_click_time)) {

	for (i = 0;
	     i < data->sarraycount && data->sarray[i] != data->stype ;
	     i++) ;

	if (++i >= data->sarraycount) i = 0;
	data->stype = data->sarray[i];
    } else {			/* single-click event */
	data->stype = data->sarray[0];
    }
    data->lasttime = event->xbutton.time;
}

static void StartPrimary(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{                                              
    XmTextPosition Left, Right, position;
    XButtonPressedEvent *b_event = (XButtonPressedEvent *) event;
    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;
    if (tw->text.output->data->dest_visible) {
       _XmTextDestinationVisible(w, False);
       data->changed_dest_visible = True;
    }
    data->anchor = (*tw->text.output->XYToPos)((XmTextWidget)w, event->xbutton.x,
					           event->xbutton.y);
    SetSelectionHint(w, event, params, num_params);
    SetScanType(w, data, event);
    if (data->stype != XmSELECT_POSITION || 
	_XmStringSourceHasSelection(tw->text.source))
       DoSelection(w, event, params, num_params);
    _XmTextSetDestinationSelection(w, data->anchor, False, event->xbutton.time);
}


/* ARGSUSED */
static void StartSecondary(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{                                              
    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;
    if (tw->text.output->data->dest_visible) {
       _XmTextDestinationVisible(w, False);
       data->changed_dest_visible = True;
    }
    data->Sel2Hint.x = event->xbutton.x;
    data->Sel2Hint.y = event->xbutton.y;
    data->selectionMove = FALSE;

    XtGrabKeyboard(w, False, GrabModeAsync, GrabModeAsync, CurrentTime);
}

/*
 * This routine implements extension of the currently selected text in
 * the "current" mode (i.e. char word, line, etc.). It worries about
 * extending from either end of the selection and handles the case when you
 * cross through the "center" of the current selection (e.g. switch which
 * end you are extending!).
 * [NOTE: This routine will be replaced by a set of procedures that
 * will allows clients to implements a wide class of draw through and
 * multi-click selection user interfaces.]
*/
static Boolean dragged(selectionHint, event, threshold)
SelectionHint selectionHint;
XEvent *event;
int threshold;
{
  int xdiff, ydiff;
    xdiff = abs(selectionHint.x - event->xbutton.x);
    ydiff = abs(selectionHint.y - event->xbutton.y);
    if((xdiff > threshold) || (ydiff > threshold))
        return TRUE;
    else
	return FALSE;
}


/* ARGSUSED */
static void ExtendSelection(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;
    XmTextPosition position, left, right, cursorPos;
    float bal_point;

    if (data->cancel) return;

    if(data->selectionHint.x || data->selectionHint.y){
        if(!dragged(data->selectionHint, event, data->threshold))
	    return;
	a_Selection(tw, data->selectionHint.x, data->selectionHint.y,
		    event->xbutton.time);
	data->selectionHint.x = data->selectionHint.y = 0;
    }

    _XmTextDisableRedisplay(tw, FALSE);
    if (!((*tw->text.source->GetSelection)
	 (tw->text.source, &left, &right)) || left == right) {
        data->anchor = tw->text.dest_position;
        left = right = XmTextGetCursorPosition((Widget)tw);
	data->origLeft = data->origRight = data->anchor;
        bal_point = data->anchor;
    } else
        bal_point = (float)(((float)(right - left) / 2.0) + (float)left);


    position = (*tw->text.output->XYToPos)(tw, event->xbutton.x,
					  event->xbutton.y);

 /* shift anchor and direction to opposite end of the selection */
    if (!data->extending)
       if ((float)position < bal_point) {
          data->extendDir = XmsdLeft;
          data->anchor = data->origRight;
       } else if ((float)position > bal_point) {
          data->extendDir = XmsdRight;
          data->anchor = data->origLeft;
    }

    data->extending = TRUE;

    /* check for change in extend direction */
    if ((data->extendDir == XmsdRight && position < data->anchor) ||
	  (data->extendDir == XmsdLeft && position > data->anchor)) {
	data->extendDir =
	    (data->extendDir == XmsdRight) ? XmsdLeft : XmsdRight;
	left = data->origLeft;
	right = data->origRight;
    }
	
    if (data->extendDir == XmsdRight) {
	right = cursorPos =
		 (*tw->text.source->Scan)(tw->text.source, position,
						    data->stype, XmsdRight, 1,
                                                  data->stype == XmSELECT_LINE);
        left = data->anchor;
    } else {
	left = cursorPos =
		 (*tw->text.source->Scan)(tw->text.source, position,
						   data->stype, XmsdLeft,
						   1, FALSE);
        right = data->anchor;
    }

    (*tw->text.source->SetSelection)(tw->text.source, left, right,
				    event->xbutton.time);
    tw->text.pendingoff = FALSE;
    _XmTextSetDestinationSelection(w, cursorPos, False, event->xbutton.time);
    _XmTextEnableRedisplay(tw);
    _XmTextSetCursorPosition((Widget)tw, cursorPos);
}

/* ARGSUSED */
static void ExtendSecondary(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;
    XmTextPosition position, hintposition,  left, right;

    if (data->cancel) return;

    position = (*tw->text.output->XYToPos)(tw, event->xbutton.x,
                                          event->xbutton.y);
    if(data->Sel2Hint.x || data->Sel2Hint.y){
        if(!dragged(data->Sel2Hint, event, data->threshold))
	    return;
	hintposition = (*tw->text.output->XYToPos)(tw, data->Sel2Hint.x,
                                          data->Sel2Hint.y);
	if(position < hintposition) {
            data->Sel2Extending = SetSel2(data,  position, hintposition,
					  event->xbutton.time);
	    data->Sel2OrigLeft = hintposition; /**/
	    data->Sel2OrigRight = hintposition;
	    data->Sel2ExtendDir = XmsdLeft;
	} else {
	    data->Sel2Extending = SetSel2(data, hintposition, position,
					  event->xbutton.time);
	    data->Sel2OrigLeft = hintposition;
	    data->Sel2OrigRight = hintposition; /**/
	    data->Sel2ExtendDir = XmsdRight;
	}
	data->Sel2Hint.x = data->Sel2Hint.y = 0;
    } 
    if(!data->Sel2Extending)
	return;
    _XmTextDisableRedisplay(tw, FALSE);
    GetSel2(data, &left, &right);
    /* check for change in extend direction */
    if ((data->Sel2ExtendDir == XmsdRight && position < data->Sel2OrigLeft) ||
	  (data->Sel2ExtendDir == XmsdLeft &&
				 position > data->Sel2OrigRight)) {
	data->Sel2ExtendDir =
	    (data->Sel2ExtendDir == XmsdRight) ? XmsdLeft : XmsdRight;
	left = data->Sel2OrigLeft;
	right = data->Sel2OrigRight;
    }
	
    if (data->Sel2ExtendDir == XmsdRight)
	right = (*tw->text.source->Scan)(tw->text.source, position,
				XmSELECT_POSITION, XmsdRight,1, FALSE);
    else
	left = (*tw->text.source->Scan)(tw->text.source, position,
				XmSELECT_POSITION, XmsdLeft,  1, FALSE);
    (void) SetSel2(data, left, right, event->xbutton.time);
    _XmTextEnableRedisplay(tw);

}

static void ExtendEnd(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;
    if(data->extending || dragged(data->Sel2Hint, event, data->threshold)){
        ExtendSelection(w, event, params, num_params);
        (*tw->text.source->GetSelection)(tw->text.source,
				    &(data->origLeft), &(data->origRight));
    }
    data->extending = FALSE;
    data->selectionHint.x = data->selectionHint.y = 0;
    
    if (data->changed_dest_visible) {
       _XmTextDestinationVisible(w, True);
       data->changed_dest_visible = False;
    }

    data->cancel = False;
}

/* ARGSUSED */
static void DoGrabFocus(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{

   if (_XmGetFocusPolicy((Widget)w) == XmPOINTER) {
       SetCursorPosition(w, event, params, num_params);
       StartPrimary(w, event, params, num_params);
   } else {
       if (w == _XmGetTabGroup(w) || XmProcessTraversal(w, XmTRAVERSE_CURRENT)){
          StartPrimary(w, event, params, num_params);
          SetCursorPosition(w, event, params, num_params);
       } else
          StartPrimary(w, event, params, num_params);
   }
}
 
/* ARGSUSED */
static void MoveDestination(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
   XmTextWidget tw = (XmTextWidget) w;
   XmTextPosition new_pos, left, right;
   InputData data = tw->text.input->data;

   new_pos = (*tw->text.output->XYToPos)((XmTextWidget)w, event->xbutton.x,
					           event->xbutton.y);

   if ((*tw->text.source->GetSelection)(tw->text.source, &left, &right)
        && (right != left))
      _XmTextSetDestinationSelection(w, new_pos, False, event->xkey.time);

   tw->text.pendingoff = False;
   if (_XmGetFocusPolicy((Widget)w) == XmPOINTER)
       _XmTextSetCursorPosition((Widget)tw, new_pos);
   else if (w == _XmGetTabGroup(w) ||
	     XmProcessTraversal(w, XmTRAVERSE_CURRENT))
       _XmTextSetCursorPosition((Widget)tw, new_pos);
   if (new_pos < left && new_pos > right)
      tw->text.pendingoff = True;
}

/* Pastes the primary selection to the stuff position. */

/* ARGSUSED */
static void DoStuff(w, closure, seltype, type, value, length, format)
Widget w;
Opaque closure;
Atom *seltype;
Atom *type;
char *value;
int *length;
int *format;
{
    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;
    XmTextBlockRec block;
    XmTextPosition cursorPos;
    XmTextPosition right, left;
    XmAnyCallbackStruct cb;
    PrimSelect *prim_select = (PrimSelect *) closure;
    Atom XA_DELETE = XmInternAtom(XtDisplay(tw), "DELETE", False);
    XSelectionRequestEvent *req_event;

    if (!value) {
       return;
    }

    if (*type == XA_DELETE) {
       if (prim_select->length > 0 && data->selectionMove) {
          if (XmTextGetSelectionPosition((Widget)tw, &left, &right) &&
              left < prim_select->position) {
             (*tw->text.source->SetSelection)(tw->text.source,
			          prim_select->position - prim_select->length,
				  prim_select->position, prim_select->time);
             data->anchor = prim_select->position - prim_select->length;
          } else {
             (*tw->text.source->SetSelection)(tw->text.source,
			          prim_select->position, prim_select->position +
				  prim_select->length, prim_select->time);
             data->anchor = prim_select->position;
          }
           _XmTextSetDestinationSelection(w, tw->text.cursor_position,
					  False, prim_select->time);
      }
    } else {
       block.ptr = value;
       block.length = *length;
       prim_select->length = *length;
       block.format = (int) *type;
       if ((*tw->text.source->Replace)(tw->text.source, (XEvent *)NULL,
	     prim_select->position, prim_select->position, &block) != EditDone)
	   RingBell(w, (XEvent *) NULL, (char **) NULL, (Cardinal) 0);
       else {
           tw->text.pendingoff = FALSE;
           cursorPos = prim_select->position + *length;
           _XmTextSetDestinationSelection(w, cursorPos, False,
					  prim_select->time);
           _XmTextSetCursorPosition((Widget)tw, cursorPos);
           tw->text.pendingoff = TRUE;
           if (prim_select->length == 0 && !data->selectionMove)
              data->anchor = cursorPos;

	   /* Call ValueChanged Callback to indicate that text has been modified */
	   cb.reason = XmCR_VALUE_CHANGED;
	   cb.event = (XEvent *)NULL;
	   XtCallCallbackList ((Widget) tw, tw->text.value_changed_callback,
			      (Opaque) &cb);
       }
    }
    XtFree(value);
}


/* This function make the request to do a primary paste */

/* ARGSUSED */
static void Stuff(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;
    PrimSelect *prim_select = (PrimSelect *)
				XtMalloc((unsigned)sizeof(PrimSelect));
    XmTextPosition left, right;
    Opaque *closures[2];
    Atom targets[2];


   /*
    * Set stuff position to the x and y position of
    * the button pressed event for primary pastes.
    */
    if (event->type == ButtonRelease) {
       prim_select->time = event->xbutton.time;
       prim_select->position = (*tw->text.output->XYToPos)(tw,
					 event->xbutton.x, event->xbutton.y);
    } else {
       prim_select->time = event->xkey.time;
       prim_select->position = XmTextGetCursorPosition((Widget)tw);
    }

    if ((*tw->text.source->GetSelection)
                (tw->text.source, &left, &right) && left != right &&
        prim_select->position >= left && prim_select->position <= right)
       return;
       
    prim_select->length = 0;

    targets[0] = XA_STRING;
    closures[0] = (Opaque *)prim_select;

    if (data->selectionMove) {
       targets[1] = XmInternAtom(XtDisplay(tw), "DELETE", False);
       closures[1] = (Opaque *)prim_select;

      /* Make request to call DoStuff() with the primary selection. */
       XtGetSelectionValues(w, XA_PRIMARY, targets, 2, DoStuff,
			    closures, event->xbutton.time);
    } else {
      /* Make request to call DoStuff() with the primary selection. */
       XtGetSelectionValue(w, XA_PRIMARY, targets[0], DoStuff,
			   (Opaque)prim_select, event->xbutton.time);
    }

}

static void HandleSelectionReplies(w, closure, ev)
Widget w;
XtPointer closure;
XEvent *ev;
{
   XSelectionEvent *event = (XSelectionEvent *) ev;
   XmTextWidget tw = (XmTextWidget) w;
   InputData data = tw->text.input->data;
   Atom property = (Atom) closure;
   TextDestData dest_data;
   int adjustment = 0;
   XmTextBlockRec block;
   XmTextPosition left, right;
   Atom XM_TEXT_PROP = XmInternAtom(XtDisplay(w), "_XM_TEXT_I_S_PROP", False);

   if (event->type != SelectionNotify) return;

   XtRemoveEventHandler(w, (EventMask) NULL, TRUE,
                        HandleSelectionReplies, (XtPointer)XM_TEXT_PROP);

   dest_data = GetTextDestData(w);

   if (event->property == None) {
      (void) SetSel2(data, 1, 0, CurrentTime);
      data->selectionMove = False;
   } else {
      if (dest_data->has_destination) {
         adjustment = data->sel2Right - data->sel2Left;

         if (dest_data->position <= data->sel2Left) {
            data->sel2Left -= dest_data->replace_length;
            data->sel2Right += adjustment - dest_data->replace_length;
         } else if (dest_data->position > data->sel2Left &&
                    dest_data->position < data->sel2Right) {
            data->sel2Left -= dest_data->replace_length;
            data->sel2Right += adjustment - dest_data->replace_length;
         }
     }

     left = data->sel2Left;
     right = data->sel2Right;

     (void) SetSel2(data, 1, 0, CurrentTime);

     if (data->selectionMove) {
        block.length = 0;
        if (dest_data->position <= data->sel2Left) left += adjustment;
        if ((*tw->text.source->Replace)(tw->text.source, ev,
                                        left, right, &block) != EditDone) {
           RingBell(w, (XEvent *) NULL, (char **) NULL, (Cardinal) 0);
        } else {
           if (dest_data->has_destination && tw->text.dest_position > right) {
              XmTextPosition cursorPos;
              cursorPos = tw->text.dest_position - (right - left);
              if (!dest_data->quick_key)
                 _XmTextSetCursorPosition((Widget)tw, cursorPos);
              _XmTextSetDestinationSelection(w, cursorPos, False, event->time);
           }
           if (!dest_data->has_destination) {
	      data->anchor = tw->text.cursor_position;
              XmTextSetAddMode(w, False);
           }
        }
        data->selectionMove = False;
     }
   }

   XDeleteProperty(XtDisplay(w), event->requestor, property);
}


/* Send a client message to perform the quick cut/copy and paste */
/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void SecondaryNotify(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;
    TextInsertPair *pair = (TextInsertPair *)
                                  XtMalloc( (unsigned)sizeof(TextInsertPair));
    Atom XM_TEXT_PROP = XmInternAtom(XtDisplay(w), "_XM_TEXT_I_S_PROP", False);
    Atom XA_ATOM_PAIR = XmInternAtom(XtDisplay(w), "ATOM_PAIR", False);
    Atom XA_INSERT_SELECTION = XmInternAtom(XtDisplay(w),
					   "INSERT_SELECTION", False);
    Atom MOTIF_DESTINATION = XmInternAtom(XtDisplay(tw),
                                          "MOTIF_DESTINATION", False);
    TextDestData dest_data;
    XmTextPosition left, right;

    if (data->selectionMove == TRUE &&
        tw->text.input->data->has_destination &&
        tw->text.dest_position >= data->sel2Left &&
        tw->text.dest_position <= data->sel2Right) {
       (void) SetSel2(data, 1, 0, CurrentTime);
       return;
    }

   /* fill in atom pair */
    pair->selection = XA_SECONDARY;
    pair->target = XA_STRING;

   /* add the insert selection property on the text field tw's window */
    XChangeProperty(XtDisplay(w), XtWindow(w), XM_TEXT_PROP, XA_ATOM_PAIR,
                    32, PropModeReplace, (unsigned char *)pair, 2);

    XtFree((XtPointer)pair);

    dest_data = GetTextDestData(w);

    dest_data->has_destination = tw->text.input->data->has_destination;
    dest_data->position = tw->text.dest_position;
    dest_data->replace_length = 0;

    if (*num_params == 1) dest_data->quick_key = True;
    else dest_data->quick_key = False;

    if ((*tw->text.source->GetSelection)
                (tw->text.source, &left, &right) && left != right) {
       if (dest_data->position >= left && dest_data->position <= right)
          dest_data->replace_length = right - left;
    }

   /* add an event handler to handle selection notify events */
    XtAddEventHandler(w, (EventMask) NULL, TRUE,
                      HandleSelectionReplies, (XtPointer)XM_TEXT_PROP);

   /*
    * Make a request for the primary selection to convert to
    * type INSERT_SELECTION as per ICCCM.
    */
    XConvertSelection(XtDisplay(w), MOTIF_DESTINATION, XA_INSERT_SELECTION,
                      XM_TEXT_PROP, XtWindow(w), event->xbutton.time);
}


/*
 * This action proc gains ownership of the secondary selection
 * and initializes the secondary selection data for performing 
 * a quick cut.
 */
/* ARGSUSED */
static void SetQuickCut(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;
    Boolean result;

    result = XtOwnSelection(data->widget, XA_SECONDARY, event->xkey.time,
                            Convert2, LoseSel2, (XtSelectionDoneProc) NULL);
    data->sec_time = event->xkey.time;
    data->hasSel2 = result;

    if (result) {
       data->sel2Left = data->sel2Right =
             data->Sel2OrigLeft =  XmTextGetCursorPosition(w);
    }

    data->selectionMove = TRUE;
}


/*
 * This action proc gains ownership of the secondary selection
 * and initializes the secondary selection data for performing
 * a quick copy.
 */
/* ARGSUSED */
static void SetQuickCopy(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;
    Boolean result;

    result = XtOwnSelection(data->widget, XA_SECONDARY, event->xkey.time,
                            Convert2, LoseSel2, (XtSelectionDoneProc) NULL);
    data->sec_time = event->xkey.time;
    data->hasSel2 = result;

    if (result) {
       data->sel2Left = data->sel2Right =
             data->Sel2OrigLeft =  XmTextGetCursorPosition(w);
    }

    data->selectionMove = FALSE;
}


/*
 * This action proc. performs a quick copy/cut from the quick anchor
 * point (denoted by data->Sel2OrigLeft) to the current insert cursor
 * position.
 */
static void DoQuickAction(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;
    XmTextPosition cursorPos = XmTextGetCursorPosition(w);
    
    if (data->hasSel2) {
       if (data->Sel2OrigLeft < cursorPos) {
          data->sel2Left = data->Sel2OrigLeft;
          data->sel2Right = cursorPos;
       } else if (data->Sel2OrigLeft > cursorPos) {
          data->sel2Left = cursorPos;
          data->sel2Right = data->Sel2OrigLeft;
       }
       XmTextSetHighlight((Widget)data->widget, data->sel2Left,
			  data->sel2Right, XmHIGHLIGHT_SECONDARY_SELECTED);
      /*
       * When num_params equals 1, the SecondaryNotify() action proc
       * knows that the action came from a key action
       */
       *num_params = 1;
       SecondaryNotify(w, event, params, num_params);
    }
}



/*
 * This function set the final position of the secondary selection and
 * calls SecondaryNotify().
 */
static void ExtendSecondaryEnd(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;
    Boolean primary_paste = True;
    if ((data->Sel2Extending || dragged(data->Sel2Hint, event,data->threshold)) 
        && !data->cancel) {
       /* Set the final position of the secondary selection */
        ExtendSecondary(w, event, params, num_params);
        GetSel2(data, &(data->Sel2OrigLeft), &(data->Sel2OrigRight));

       /* doing quick paste instead of primary paste */
        primary_paste = False;
    }
   /* Re-initialize the secondary selection data */
    data->Sel2Extending = FALSE;
    data->Sel2Hint.x = data->Sel2Hint.y = 0;

   /* If not doing primary paste, do secondary selection notification.*/
    if (!primary_paste) SecondaryNotify(w, event, params, num_params);

    if (!data->cancel) XtUngrabKeyboard(w, CurrentTime);

    
    if (data->changed_dest_visible) {
       _XmTextDestinationVisible(w, True);
       data->changed_dest_visible = False;
    }

    data->cancel = False;
}


/*
 * This Action Proc selects all of the text.
 */
/* ARGSUSED */
static void SelectAll(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;
    Position x, y;

    _XmTextSetDestinationSelection(w, tw->text.last_position,
			           False, event->xkey.time);
    (*tw->text.output->PosToXY)(tw, tw->text.cursor_position,
				    &x, &y);

    data->stype = XmSELECT_ALL;
    a_Selection(w, x, y, event->xkey.time);
}


/*
 * This Action Proc deselects all of the text.
 */
/* ARGSUSED */
static void DeselectAll(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;
    XmTextPosition cursorPos = XmTextGetCursorPosition((Widget)tw);

    _XmTextSetDestinationSelection(w, cursorPos, False, event->xkey.time);
    data->stype = XmSELECT_POSITION;
    (*tw->text.source->SetSelection)(tw->text.source, cursorPos,
					         cursorPos, event->xkey.time);
    data->anchor = cursorPos;
}


/*
 * This Action Proc replaces the primary selection with spaces
 */
/* ARGSUSED */
static void ClearSelection(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;
    XmAnyCallbackStruct cb;
    XmTextPosition left, right;

    if (!(*tw->text.source->GetSelection)
			(tw->text.source, &left, &right)) {
        XBell(XtDisplay(tw), 0);
    } else if (left != right) {
           char *select_string = _XmStringSourceGetString(tw, left, right);
           XmTextBlockRec block;
           int length = strlen(select_string);
           int i;

           for(i = 0; i < length; i++) {
              if (select_string[i] != '\012') select_string[i] = ' ';
           }

	   block.ptr = select_string;
	   block.length = length;
	   block.format = FMT8BIT;
	   if ((*tw->text.source->Replace)(tw->text.source, event, left,
				              right, &block) != EditDone) {
	      RingBell(w, event, params, num_params);
	   } else {
              XmTextClearSelection(w, event->xkey.time);

             /* Call ValueChanged Callback to indicate
              * that text has been modified.
              */
              cb.reason = XmCR_VALUE_CHANGED;
              cb.event = event;
              XtCallCallbackList ((Widget) tw,
			     tw->text.value_changed_callback, (Opaque) &cb);

           }
           XtFree(select_string);
    }
}


/*
 * This function does a primary copy and paste on mouse button actions.
 * It copies the contents of the primary selection to the x and y
 * position of the button pressed event.
 */
static void ProcessCopy(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmAnyCallbackStruct cb;
    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;

    data->selectionMove = False;
    if (dragged(data->Sel2Hint, event, data->threshold)) {
       if (data->Sel2Extending) {
         /*
          * Causes the converter to perform a delete action of the
          * secondary selection when the Convert routine is called.
          */
	  ExtendSecondaryEnd(w, event, params, num_params);
       }
    } else {
      /*
       * Copy contents of primary selection to the stuff position found above.
       */
       Stuff(w, event, params, num_params);
       if (!data->cancel) XtUngrabKeyboard(w, CurrentTime);
       
       if (data->changed_dest_visible) {
          _XmTextDestinationVisible(w, True);
          data->changed_dest_visible = False;
       }

    }
}

/* This function does a primary cut and paste on mouse button actions. */
static void ProcessMove(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmAnyCallbackStruct cb;
    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;

    data->selectionMove = True;

    if (dragged(data->Sel2Hint, event, data->threshold)) {
       if (data->Sel2Extending) {
         /*
          * Causes the converter to perform a delete action of the
          * secondary selection when the Convert routine is called.
          */
	  ExtendSecondaryEnd(w, event, params, num_params);
       }
    } else {
      /*
       * Copy contents of primary selection to the stuff position found above.
       */
       Stuff(w, event, params, num_params);
       if (!data->cancel) XtUngrabKeyboard(w, CurrentTime);
       
       if (data->changed_dest_visible) {
          _XmTextDestinationVisible(w, True);
          data->changed_dest_visible = False;
       }

    }
}


/* This function does a primary copy and paste on keyboard actions. */

static void CopyPrimary(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;

    data->selectionMove = False;

    if (tw->text.output->data->dest_visible) {
       _XmTextDestinationVisible(w, False);
       data->changed_dest_visible = True;
    }
   /* perform the primary paste action */
    Stuff(w, event, params, num_params);
    
    if (data->changed_dest_visible) {
       _XmTextDestinationVisible(w, True);
       data->changed_dest_visible = False;
    }

}

/* This function does a primary cut and paste on keyboard actions. */

static void CutPrimary(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;

    data->selectionMove = True;

    if (tw->text.output->data->dest_visible) {
       _XmTextDestinationVisible(w, False);
       data->changed_dest_visible = True;
    }
   /* perform the primary paste action */
    Stuff(w, event, params, num_params);
    
    if (data->changed_dest_visible) {
       _XmTextDestinationVisible(w, True);
       data->changed_dest_visible = False;
    }

}

/* ARGSUSED */
static void CutClipboard(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
   XmTextWidget tw = (XmTextWidget) w;
   InputData data = tw->text.input->data;

   if ( XmTextCut(w, event->xkey.time) == False )             /* False returned if cannot cut    PIR1870 */
       RingBell(w, event, params, num_params);                /* Notify user cut wasn't done     PIR1870 */
}


/* ARGSUSED */
static void CopyClipboard(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
   XmTextWidget tw = (XmTextWidget) w;

   XmTextCopy(w, event->xkey.time);
   _XmTextSetDestinationSelection(w, tw->text.cursor_position,
			          False, event->xkey.time);
}


/* ARGSUSED */
static void PasteClipboard(w, event, params, num_params)
Widget w;
XEvent *event;
char **params;
Cardinal *num_params;
{
   XmTextWidget tw = (XmTextWidget) w;
   InputData data = tw->text.input->data;

   XmTextPaste(w);
}


/*--------------------------------------------------------------------------+*/
static Boolean VerifyLeave(w, event)
/*--------------------------------------------------------------------------+*/
Widget  w;
XEvent        *event;
{
    XmTextWidget tw = (XmTextWidget) w;

    XmTextVerifyCallbackStruct  cbdata;

    cbdata.reason = XmCR_LOSING_FOCUS;
    cbdata.event = event;
    cbdata.doit = True;
    cbdata.currInsert = tw->text.cursor_position;
    cbdata.newInsert = tw->text.cursor_position;
    cbdata.startPos = tw->text.cursor_position;
    cbdata.endPos = tw->text.cursor_position;
    cbdata.text = NULL;
    XtCallCallbackList ((Widget) tw, tw->text.losing_focus_callback,
		       (Opaque) &cbdata);
    return(cbdata.doit);
}

/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void TextLeave(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent           *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;

    if (_XmGetFocusPolicy((Widget)tw) == XmPOINTER) 
       VerifyLeave(tw, event);

    _XmPrimitiveLeave((XmPrimitiveWidget)tw, event);
}

/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void TextFocusOut(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent           *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;

   /* If traversal is on, then the leave verification callback is called in
      the traversal event handler */
    if (event->xfocus.send_event && _XmGetFocusPolicy((Widget)tw) == XmEXPLICIT &&
        !tw->text.traversed) {
       if (!VerifyLeave(tw, event)) return;
    } else
       if (tw->text.traversed) tw->text.traversed = False;

    _XmPrimitiveFocusOut((XmPrimitiveWidget)tw, event);
}

/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void TraverseDown(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent           *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;

  /* Find out if there is anything else to traverse to */
   /* Allow the verification routine to control the traversal */
    if (VerifyLeave(tw, event)) {
       tw->text.traversed = True;
      _XmProcessTraversal((Widget)tw, XmTRAVERSE_DOWN, True);
    }
}

/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void TraverseUp(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent           *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;

   /* Allow the verification routine to control the traversal */
    if (VerifyLeave(tw, event)) {
       tw->text.traversed = True;
       _XmProcessTraversal((Widget)tw, XmTRAVERSE_UP, True);
    }
}

/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void TraverseHome(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent           *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;

   /* Allow the verification routine to control the traversal */
    if (VerifyLeave(tw, event)) {
       tw->text.traversed = True;
       _XmProcessTraversal((Widget)tw, XmTRAVERSE_HOME, True);
    }
}

/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void TraverseNextTabGroup(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent           *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;

   /* Allow the verification routine to control the traversal */
    if (VerifyLeave(tw, event)) {
       tw->text.traversed = True;
       _XmProcessTraversal((Widget)tw, XmTRAVERSE_NEXT_TAB_GROUP, True);
    }
}

/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void TraversePrevTabGroup(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent           *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;

   /* Allow the verification routine to control the traversal */
    if (VerifyLeave(tw, event)) {
       tw->text.traversed = True;
       _XmProcessTraversal((Widget)tw, XmTRAVERSE_PREV_TAB_GROUP, True);
    }
}

/***************************************************************************
 * Functions to process text tw in multi-line edit mode versus single  *
 * line edit mode.                                                         *
 ***************************************************************************/

/* ARGSUSED */
/*--------------------------------------------------------------------------+*/
static void ProcessCancel(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent           *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;
    InputData data = tw->text.input->data;

    XmParentProcessDataRec  p_event ;

    p_event.process_type = XmINPUT_ACTION ;
    p_event.data.input_action.action = XmCANCEL ;
    p_event.data.input_action.event = event ;/* Pointer to XEvent. */
    p_event.data.input_action.params = NULL ; /* Or use what you have if   */
    p_event.data.input_action.num_params = 0 ;/* input is from translation.*/

    if (data->hasSel2 && data->Sel2Extending) {
       data->cancel = True;
       SetSel2(data, 1, 0, event->xkey.time);
       XtUngrabKeyboard(w, CurrentTime);
    }

    if (_XmStringSourceHasSelection(tw->text.source) && data->extending) {
       data->cancel = True;
      /* reset origLeft and origRight */
       (*tw->text.source->SetSelection)(tw->text.source, data->origLeft,
					data->origRight, event->xkey.time);
    }

    if (!data->cancel)
       (void) _XmParentProcess(XtParent(tw), &p_event);

}

/*--------------------------------------------------------------------------+*/
static void ProcessReturn(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent           *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;

    if (tw->text.edit_mode == XmSINGLE_LINE_EDIT)
       Activate(w, event, params, num_params);
    else
       InsertNewLine(w, event, params, num_params);
}

/*--------------------------------------------------------------------------+*/
static void ProcessTab(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent           *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;

    if (tw->text.edit_mode == XmSINGLE_LINE_EDIT)
       TraverseNextTabGroup(w, event, params, num_params);
    else
       SelfInsert(w, event, params, num_params);
}

/*--------------------------------------------------------------------------+*/
static void ProcessUp(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent           *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;

    if (tw->text.edit_mode == XmMULTI_LINE_EDIT)
       MovePreviousLine(w, event, params, num_params);
    else if (w != _XmFindTabGroup(w))
       TraverseUp(w, event, params, num_params);
}

/*--------------------------------------------------------------------------+*/
static void ProcessDown(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent           *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;

    if (tw->text.edit_mode == XmMULTI_LINE_EDIT)
       MoveNextLine(w, event, params, num_params);
    else if (w != _XmFindTabGroup(w))
       TraverseDown(w, event, params, num_params);
}

/*--------------------------------------------------------------------------+*/
static void ProcessShiftUp(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent           *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;

    if (tw->text.edit_mode == XmSINGLE_LINE_EDIT) {
       TraverseUp(w, event, params, num_params);
    } else {
       char *dir = "up";
       Cardinal num = 1;
       KeySelection(w, event, &dir, &num);
    }
}

/*--------------------------------------------------------------------------+*/
static void ProcessShiftDown(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent           *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;

    if (tw->text.edit_mode == XmSINGLE_LINE_EDIT) {
       TraverseDown(w, event, params, num_params);
    } else {
       char *dir = "down";
       Cardinal num = 1;
       KeySelection(w, event, &dir, &num);
    }
}

/*--------------------------------------------------------------------------+*/
static void ProcessHome(w, event, params, num_params)
/*--------------------------------------------------------------------------+*/
Widget w;
XEvent           *event;
char **params;
Cardinal *num_params;
{
    XmTextWidget tw = (XmTextWidget) w;

    MoveToLineStart(w, event, params, num_params);
}


static XtActionsRec ZdefaultTextActionsTable[] = {
/* Insert bindings */
  {"self-insert",		(XtActionProc)SelfInsert},
  {"insert-string",		(XtActionProc)InsertString},
/* Motion bindings */
  {"grab-focus",		(XtActionProc)DoGrabFocus},
  {"set-insertion-point",	(XtActionProc)SetCursorPosition},
  {"forward-character", 	(XtActionProc)MoveForwardChar},
  {"backward-character", 	(XtActionProc)MoveBackwardChar},
  {"forward-word", 		(XtActionProc)MoveForwardWord},
  {"backward-word", 		(XtActionProc)MoveBackwardWord},
  {"forward-paragraph", 	(XtActionProc)MoveForwardParagraph},
  {"backward-paragraph", 	(XtActionProc)MoveBackwardParagraph},
  {"beginning-of-line", 	(XtActionProc)MoveToLineStart},
  {"end-of-line", 		(XtActionProc)MoveToLineEnd},
  {"next-line", 		(XtActionProc)MoveNextLine},
  {"previous-line", 		(XtActionProc)MovePreviousLine},
  {"next-page", 		(XtActionProc)MoveNextPage},
  {"previous-page", 		(XtActionProc)MovePreviousPage},
  {"page-left", 		(XtActionProc)MovePageLeft},
  {"page-right", 		(XtActionProc)MovePageRight},
  {"beginning-of-file", 	(XtActionProc)MoveBeginningOfFile},
  {"end-of-file", 		(XtActionProc)MoveEndOfFile},
  {"move-destination",		(XtActionProc)MoveDestination},
  {"scroll-one-line-up", 	(XtActionProc)ScrollOneLineUp},
  {"scroll-one-line-down", 	(XtActionProc)ScrollOneLineDown},
/* Delete bindings */
  {"delete-selection", 		(XtActionProc)DeleteCurrentSelection},
  {"delete-next-character", 	(XtActionProc)DeleteForwardChar},
  {"delete-previous-character",	(XtActionProc)DeleteBackwardChar},
  {"delete-next-word", 		(XtActionProc)DeleteForwardWord},
  {"delete-previous-word", 	(XtActionProc)DeleteBackwardWord},
  {"delete-to-end-of-line", 	(XtActionProc)DeleteToEndOfLine},
  {"delete-to-start-of-line",	(XtActionProc)DeleteToStartOfLine},
/* Kill bindings */
  {"kill-selection", 		(XtActionProc)KillCurrentSelection},
  {"kill-next-character", 	(XtActionProc)KillForwardChar},
  {"kill-previous-character",	(XtActionProc)KillBackwardChar},
  {"kill-next-word", 		(XtActionProc)KillForwardWord},
  {"kill-previous-word", 	(XtActionProc)KillBackwardWord},
  {"kill-to-end-of-line", 	(XtActionProc)KillToEndOfLine},
  {"kill-to-start-of-line",	(XtActionProc)KillToStartOfLine},
/* Unkill bindings */
  {"unkill", 			(XtActionProc)UnKill},
  {"stuff", 			(XtActionProc)Stuff},
/* New line bindings */
  {"newline-and-indent", 	(XtActionProc)InsertNewLineAndIndent},
  {"newline-and-backup", 	(XtActionProc)InsertNewLineAndBackup},
  {"newline",			(XtActionProc)InsertNewLine},
/* Selection bindings */
  {"select-all", 		(XtActionProc)SelectAll},
  {"deselect-all", 		(XtActionProc)DeselectAll},
  {"select-start", 		(XtActionProc)StartPrimary},
  {"quick-cut-set", 		(XtActionProc)SetQuickCut},
  {"quick-copy-set", 		(XtActionProc)SetQuickCopy},
  {"do-quick-action", 		(XtActionProc)DoQuickAction},
  {"key-select", 		(XtActionProc)KeySelection},
  {"set-anchor", 		(XtActionProc)SetAnchor},
  {"select-adjust", 		(XtActionProc)DoSelection},
  {"select-end", 		(XtActionProc)DoSelection},
  {"extend-start", 		(XtActionProc)ExtendSelection},
  {"extend-adjust", 		(XtActionProc)ExtendSelection},
  {"extend-end", 		(XtActionProc)ExtendEnd},
  {"set-selection-hint",	(XtActionProc)SetSelectionHint},
  {"secondary-start",		(XtActionProc)StartSecondary},
  {"secondary-adjust",		(XtActionProc)ExtendSecondary},
  {"secondary-notify",		(XtActionProc)SecondaryNotify},
  {"clear-selection",		(XtActionProc)ClearSelection},
  {"copy-to",			(XtActionProc)ProcessCopy},
  {"move-to",			(XtActionProc)ProcessMove},
  {"copy-primary",		(XtActionProc)CopyPrimary},
  {"cut-primary",		(XtActionProc)CutPrimary},
/* Clipboard bindings */
  {"copy-clipboard",		(XtActionProc)CopyClipboard},
  {"cut-clipboard",		(XtActionProc)CutClipboard},
  {"paste-clipboard",		(XtActionProc)PasteClipboard},
/* Miscellaneous bindings */
  {"beep", 			(XtActionProc)RingBell},
  {"redraw-display", 		(XtActionProc)RedrawDisplay},
  {"activate",			(XtActionProc)Activate},
  {"toggle-add-mode",		(XtActionProc)ToggleAddMode},
  {"Help",			(XtActionProc)_XmPrimitiveHelp},
  {"enter",                     (XtActionProc)_XmPrimitiveEnter},
  {"leave",			(XtActionProc)TextLeave},
  {"focusIn",			(XtActionProc)_XmPrimitiveFocusIn},
  {"focusOut",			(XtActionProc)TextFocusOut},
  {"unmap",			(XtActionProc)_XmPrimitiveUnmap},
/* Process multi-line and single line bindings */
  {"process-cancel",		(XtActionProc)ProcessCancel},
  {"process-return",		(XtActionProc)ProcessReturn},
  {"process-tab",		(XtActionProc)ProcessTab},
  {"process-up",		(XtActionProc)ProcessUp},
  {"process-down",		(XtActionProc)ProcessDown},
  {"process-shift-up",		(XtActionProc)ProcessShiftUp},
  {"process-shift-down",	(XtActionProc)ProcessShiftDown},
  {"process-home",		(XtActionProc)ProcessHome},
/* Traversal bindings*/
  {"traverse-next",		(XtActionProc)TraverseDown},
  {"traverse-prev",		(XtActionProc)TraverseUp},
  {"traverse-home",		(XtActionProc)TraverseHome},
  {"next-tab-group",		(XtActionProc)TraverseNextTabGroup},
  {"prev-tab-group",		(XtActionProc)TraversePrevTabGroup},
};

externaldef(nonvisible) caddr_t _XmdefaultTextActionsTable =
					 (caddr_t) ZdefaultTextActionsTable;

Cardinal _XmdefaultTextActionsTableSize = XtNumber(ZdefaultTextActionsTable);

#ifndef MCCABE
char _XmTextEventBindings1[] = {
"Shift<Key>osfQuickPaste:	do-quick-action()\n\
 Meta <Key>osfQuickPaste:	quick-cut-set()\n\
 Alt <Key>osfQuickPaste:	quick-cut-set()\n\
 <Key>osfQuickPaste:		quick-copy-set()\n\
 Meta <Key>osfPrimaryPaste:	cut-primary()\n\
 Alt <Key>osfPrimaryPaste:	cut-primary()\n\
 <Key>osfPrimaryPaste:		copy-primary()\n\
 Meta <Key>osfCut:		cut-primary()\n\
 Alt <Key>osfCut:		cut-primary()\n\
 <Key>osfCut:			cut-clipboard()\n\
 <Key>osfPaste:			paste-clipboard()\n\
 Meta <Key>osfCopy:		copy-primary()\n\
 Alt <Key>osfCopy:		copy-primary()\n\
 <Key>osfCopy:			copy-clipboard()\n\
 Shift Ctrl <Key>osfBeginLine:	beginning-of-file(extend)\n\
 Ctrl <Key>osfBeginLine:	beginning-of-file()\n\
 Shift <Key>osfBeginLine:	beginning-of-line(extend)\n\
 <Key>osfBeginLine:		beginning-of-line()\n\
 Shift Ctrl <Key>osfEndLine:	end-of-file(extend)\n\
 Ctrl <Key>osfEndLine:		end-of-file()\n\
 Shift <Key>osfEndLine:		end-of-line(extend)\n\
 <Key>osfEndLine:		end-of-line()\n\
 Ctrl <Key>osfPageUp:		page-left()\n\
 Shift <Key>osfPageUp:		previous-page(extend)\n\
 <Key>osfPageUp:		previous-page()\n\
 Ctrl <Key>osfPageDown:		page-right()\n\
 Shift <Key>osfPageDown:	next-page(extend)\n\
 <Key>osfPageDown:		next-page()\n\
 <Key>osfClear:			clear-selection()\n\
 ~Meta ~Alt <Key>osfBackSpace:	delete-previous-character()\n\
 Shift Meta<Key>osfDelete:	cut-primary()\n\
 Shift Alt<Key>osfDelete:	cut-primary()\n\
 Shift ~Ctrl ~Meta ~Alt <Key>osfDelete:	cut-clipboard()\n\
 ~Shift Ctrl ~Meta ~Alt <Key>osfDelete:	delete-to-end-of-line()\n\
 ~Shift ~Ctrl ~Meta ~Alt <Key>osfDelete: delete-next-character()\n"
};

char _XmTextEventBindings2[] = {
"Ctrl Meta<Key>osfInsert:	copy-primary()\n\
 Ctrl Alt<Key>osfInsert:	copy-primary()\n\
 Shift ~Ctrl ~Meta ~Alt <Key>osfInsert:	paste-clipboard()\n\
 ~Shift Ctrl ~Meta ~Alt <Key>osfInsert:	copy-clipboard()\n\
 ~Shift <Key>osfSelect:		set-anchor()\n\
 Shift ~Ctrl <Key>osfSelect:	key-select()\n\
 <Key>osfActivate:		activate()\n\
 <Key>osfAddMode:		toggle-add-mode()\n\
 <Key>osfHelp:			Help()\n\
 <Key>osfCancel:		process-cancel()\n\
 Shift Ctrl <Key>osfLeft:	backward-word(extend)\n\
 Ctrl <Key>osfLeft:		backward-word()\n\
 Shift <Key>osfLeft:		key-select(left)\n\
 <Key>osfLeft:			backward-character()\n\
 Shift Ctrl <Key>osfRight:	forward-word(extend)\n\
 Ctrl <Key>osfRight:		forward-word()\n\
 Shift <Key>osfRight:		key-select(right)\n\
 <Key>osfRight:			forward-character()\n\
 Shift Ctrl <Key>osfUp:		backward-paragraph(extend)\n\
 Ctrl <Key>osfUp:		backward-paragraph()\n\
 Shift <Key>osfUp:		process-shift-up()\n\
 <Key>osfUp:			process-up()\n\
 Shift Ctrl <Key>osfDown:	forward-paragraph(extend)\n\
 Ctrl <Key>osfDown:		forward-paragraph()\n\
 Shift <Key>osfDown:		process-shift-down()\n\
 <Key>osfDown:			process-down()\n\
 Ctrl ~Meta ~Alt <Key>slash:	select-all()\n\
 Ctrl ~Meta ~Alt <Key>backslash: deselect-all()\n\
 Shift ~Meta ~Alt <Key>Tab:	prev-tab-group()\n\
 Ctrl ~Meta ~Alt <Key>Tab:	next-tab-group()\n\
 ~Meta ~Alt <Key>Tab:		process-tab()\n\
 Ctrl ~Shift ~Meta ~Alt <Key>Return:	activate()\n\
 ~Shift ~Meta ~Alt <Key>Return:	process-return()\n\
 Ctrl ~Shift ~Meta ~Alt <Key>space:	set-anchor()\n\
 Ctrl Shift ~Meta ~Alt <Key>space:	key-select()\n\
 ~Ctrl ~Meta ~Alt <Key>:	self-insert()\n"
};

char _XmTextEventBindings3[] = {
"~Ctrl  Shift ~Meta ~Alt<Btn1Down>:		extend-start()\n\
  Ctrl ~Shift ~Meta ~Alt<Btn1Down>:		move-destination()\n\
 ~Ctrl ~Shift ~Meta ~Alt<Btn1Down>:		grab-focus()\n\
 ~Ctrl ~Meta ~Alt<Btn1Motion>:			extend-adjust()\n\
 ~Ctrl ~Meta ~Alt<Btn1Up>:			extend-end()\n\
 ~Ctrl ~Shift  Meta ~Alt<Btn2Down>:		secondary-start()\n\
 ~Ctrl ~Shift ~Meta  Alt<Btn2Down>:		secondary-start()\n\
 ~Shift ~Meta ~Alt<Btn2Down>:			secondary-start()\n\
 ~Ctrl ~Shift  Meta ~Alt<Btn2Motion>:		secondary-adjust()\n\
 ~Ctrl ~Shift ~Meta  Alt<Btn2Motion>:		secondary-adjust()\n\
 ~Shift ~Meta ~Alt<Btn2Motion>:			secondary-adjust()\n\
 ~Ctrl ~Shift  Meta ~Alt<Btn2Up>:		move-to()\n\
 ~Ctrl ~Shift ~Meta  Alt<Btn2Up>:		move-to()\n\
 ~Shift ~Meta ~Alt<Btn2Up>:			copy-to()\n\
 <EnterWindow>:					enter()\n\
 <LeaveWindow>:					leave()\n\
 <FocusIn>:					focusIn()\n\
 <FocusOut>:					focusOut()\n\
 <Unmap>:					unmap()"
};

#else /* MCCABE */
char _XmTextEventBindings1[];
char _XmTextEventBindings2[];
char _XmTextEventBindings3[];
#endif /* MCCABE */


/* ARGSUSED */
static void Invalidate(tw, position, topos, delta)
XmTextWidget tw;
XmTextPosition position, topos;
int delta;
{
    InputData data = tw->text.input->data;
    if (delta == NODELTA) return; /* Just use what we have as best guess. */
    if (data->origLeft > position) data->origLeft += delta;
    if (data->origRight >= position) data->origRight += delta;
}

static void InputGetValues(tw, args, num_args)
XmTextWidget tw;
ArgList args;
Cardinal num_args;
{
    XtGetSubvalues((caddr_t) tw->text.input->data,
		   input_resources, XtNumber(input_resources), args, num_args);
}


static void InputSetValues(tw, args, num_args)
XmTextWidget tw;
ArgList args;
Cardinal num_args;
{
    XtSetSubvalues((caddr_t) tw->text.input->data,
		   input_resources, XtNumber(input_resources), args, num_args);
}


static void Destroy(tw)
XmTextWidget tw;
{
    Atom MOTIF_DESTINATION = XmInternAtom(XtDisplay(tw),
			                "MOTIF_DESTINATION", False);

    if (tw->core.window == XGetSelectionOwner(XtDisplay(tw),
						  MOTIF_DESTINATION))
       XtDisownSelection((Widget) tw, MOTIF_DESTINATION, CurrentTime);

    if (tw->core.window == XGetSelectionOwner(XtDisplay(tw),
						  XA_PRIMARY))
       XtDisownSelection((Widget) tw, XA_PRIMARY, CurrentTime);

    if (tw->core.window == XGetSelectionOwner(XtDisplay(tw),
                                                  XA_SECONDARY))
       XtDisownSelection((Widget) tw, XA_SECONDARY, CurrentTime);

    XtFree((char *)tw->text.input->data);
    XtFree((char *)tw->text.input);
}

#ifdef _NO_PROTO
XmTextPosition _XmTextGetAnchor(tw)
XmTextWidget tw;
#else /* _NO_PROTO */
XmTextPosition _XmTextGetAnchor (XmTextWidget tw)
#endif /* _NO_PROTO */
{
    InputData data = tw->text.input->data;

    return(data->anchor);
}

#ifdef _NO_PROTO
void _XmTextInputCreate(tw, args, num_args)
XmTextWidget tw;
ArgList args;
Cardinal num_args;
#else /* _NO_PROTO */
void _XmTextInputCreate (XmTextWidget tw, ArgList args, Cardinal num_args)
#endif /* _NO_PROTO */
{
    Input input;
    InputData data;

    tw->text.input = input = (Input) XtMalloc((unsigned) sizeof(InputRec));
    input->data = data = (InputData) XtMalloc((unsigned) sizeof(InputDataRec));
    XtGetSubresources((Widget) tw->core.parent, (caddr_t)data,
		      tw->core.name, "XmText", input_resources,
		      XtNumber(input_resources), args, num_args);
    data->widget = tw;
    bzero((char *) &(data->compstatus), sizeof(XComposeStatus));

    if (data->sarray == NULL) data->sarray = (XmTextScanType *) sarray;

    if (data->sarraycount <= 0) data->sarraycount = XtNumber(sarray);

    data->lasttime = 0;
    data->cancel = False;
    data->stype = data->sarray[0];
    data->extendDir = XmsdRight;
    data->origLeft = tw->text.cursor_position;
    data->origRight = tw->text.cursor_position;
    data->anchor = tw->text.cursor_position;
    data->syncing = FALSE;
    data->has_destination = FALSE;
    data->changed_dest_visible = FALSE;
    data->dest_time = 0;
    data->sec_time = 0;
    XtAddEventHandler((Widget) tw, KeyPressMask, FALSE, CheckSync, NULL);

    input->Invalidate = Invalidate;
    input->GetValues = InputGetValues;
    input->SetValues = InputSetValues;
    input->destroy = Destroy;

}

