#ifndef lint
static char Xrcsid[] = "$XConsortium: PassivGrab.c,v 1.9 90/01/25 09:44:18 swick Exp $";
#endif

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

#include "IntrinsicI.h"
#include <X11/Xatom.h>
#include "StringDefs.h"
#include "PassivGraI.h"


/* typedef unsigned long Mask; */
#define BITMASK(i) (((Mask)1) << ((i) & 31))
#define MASKIDX(i) ((i) >> 5)
#define MASKWORD(buf, i) buf[MASKIDX(i)]
#define BITSET(buf, i) MASKWORD(buf, i) |= BITMASK(i)
#define BITCLEAR(buf, i) MASKWORD(buf, i) &= ~BITMASK(i)
#define GETBIT(buf, i) (MASKWORD(buf, i) & BITMASK(i))
#define MasksPerDetailMask 8

#define pDisplay(grabPtr) (((grabPtr)->widget)->core.screen->display)
#define pWindow(grabPtr) (((grabPtr)->widget)->core.window)


/***************************************************************************/
/*********************** Internal Support Routines *************************/
/***************************************************************************/

/*
 * Create a detail mask (32 bits * 8 bytes = 256 bits), which initially
 * has all of the mask bits enabled.
 */

static Mask *
  CreateDetailMask()
{
    Mask *pTempMask;
    int i;
    
    pTempMask = (Mask *)XtMalloc(sizeof(Mask) * MasksPerDetailMask);
    
    for ( i = 0; i < MasksPerDetailMask; i++)
      pTempMask[i]= ~0;
    
    return pTempMask;
    
}


/*
 * Turn off (clear) the bit in the specified detail mask which is associated
 * with the detail.
 */

static void
  DeleteDetailFromMask(ppDetailMask, detail)

Mask **ppDetailMask;
unsigned short detail;

{
    if (*ppDetailMask == NULL)
      *ppDetailMask = CreateDetailMask();
    
    BITCLEAR((*ppDetailMask), detail);
}


/*
 * Make an exact copy of the specified detail mask.
 */

static Mask *
  CopyDetailMask(pOriginalDetailMask)

Mask *pOriginalDetailMask;

{
    Mask *pTempMask;
    int i;
    
    if (pOriginalDetailMask == NULL)
      return NULL;
    
    pTempMask = (Mask *)XtMalloc(sizeof(Mask) * MasksPerDetailMask);
    
    for ( i = 0; i < MasksPerDetailMask; i++)
      pTempMask[i]= pOriginalDetailMask[i];
    
    return pTempMask;
    
}


/*
 * Add a new passive grab entry to the current list of passive grabs.
 */

static void
  AddServerGrabToList(passiveListPtr, pGrab)

XtServerGrabPtr	pGrab;
XtServerGrabPtr	*passiveListPtr;
{
    pGrab->next = *passiveListPtr;
    *passiveListPtr = pGrab;
}


/*
 * Allocate a new grab entry, and fill in all of the fields using the
 * specified parameters.
 */

static XtServerGrabPtr CreateGrab(widget, ownerEvents, modifiers, 
				key, pointer_mode, keyboard_mode,
				event_mask, confine_to, cursor)
    
    Widget	widget;
    Boolean	ownerEvents;
    Modifiers	modifiers;
    KeyCode 	key;
    int		pointer_mode, keyboard_mode;
    Mask	event_mask;
    Window 	confine_to;
    Cursor 	cursor;
{
    XtServerGrabPtr grab;
    
    grab = (XtServerGrabPtr)XtMalloc(sizeof(XtServerGrabRec));
    grab->widget = widget;
    grab->ownerEvents = ownerEvents;
    grab->modifiersDetail.exact = modifiers;
    grab->modifiersDetail.pMask = NULL;
    grab->detail.exact = key;
    grab->detail.pMask = NULL;
    grab->pointerMode = pointer_mode;
    grab->keyboardMode = keyboard_mode;
    grab->eventMask = event_mask;
    grab->confineTo = confine_to;
    grab->cursor = cursor;
    grab->next = NULL;
    return grab;
}


/*
 * Free up the space occupied by a grab entry.
 */

static void FreeGrab(pGrab)
    XtServerGrabPtr pGrab;
{
    if (pGrab->modifiersDetail.pMask != NULL)
      XtFree((XtPointer)pGrab->modifiersDetail.pMask);
    
    if (pGrab->detail.pMask != NULL)
      XtFree((XtPointer)pGrab->detail.pMask);
    
    XtFree((XtPointer)pGrab);
}


/*
 * If the first detail is set to 'exception' and the second detail
 * is contained in the mask of the first, then TRUE is returned.
 */

static Bool
  IsInGrabMask(firstDetail, secondDetail, exception)

DetailRec firstDetail, secondDetail;
unsigned short exception;

{
    if (firstDetail.exact == exception)
      {
	  if (firstDetail.pMask == NULL)
	    return TRUE;
	  
	  /* (at present) never called with two non-null pMasks */
	  if (secondDetail.exact == exception)
	    return FALSE;
	  
	  if (GETBIT(firstDetail.pMask, secondDetail.exact))
	    return TRUE;
      }
    
    return FALSE;
}


/*
 * If neither of the details is set to 'exception', and they match
 * exactly, then TRUE is returned.
 */

static Bool 
  IdenticalExactDetails(firstExact, secondExact, exception)

unsigned short firstExact, secondExact, exception;

{
    if ((firstExact == exception) || (secondExact == exception))
      return FALSE;
    
    if (firstExact == secondExact)
      return TRUE;
    
    return FALSE;
}


/*
 * If the first detail is set to 'exception', and its mask has the bit
 * enabled which corresponds to the second detail, OR if neither of the
 * details is set to 'exception' and the details match exactly, then
 * TRUE is returned.
 */

static Bool 
  DetailSupersedesSecond(firstDetail, secondDetail, exception)

DetailRec firstDetail, secondDetail;
unsigned short exception;

{
    if (IsInGrabMask(firstDetail, secondDetail, exception))
      return TRUE;
    
    if (IdenticalExactDetails(firstDetail.exact, secondDetail.exact, exception))
      return TRUE;
    
    return FALSE;
}


/*
 * If the two grab events match exactly, or if the first grab entry
 * 'encompasses' the second grab entry, then TRUE is returned.
 */

static Bool
  GrabSupersedesSecond(pFirstGrab, pSecondGrab)

XtServerGrabPtr pFirstGrab, pSecondGrab;

{
    if (!DetailSupersedesSecond(pFirstGrab->modifiersDetail,
				pSecondGrab->modifiersDetail, 
				(unsigned short)AnyModifier))
      return FALSE;
    
    if (DetailSupersedesSecond(pFirstGrab->detail,
			       pSecondGrab->detail, 
			       (unsigned short)AnyKey))
      return TRUE;
    
    return FALSE;
}


/*
 * Two grabs are considered to be matching if either of the following are true:
 *
 * 1) The two grab entries match exactly, or the first grab entry
 *    encompasses the second grab entry.
 * 2) The second grab entry encompasses the first grab entry.
 * 3) The keycodes match exactly, and one entry's modifiers encompasses
 *    the others.
 * 4) The keycode for one entry encompasses the other, and the detail
 *    for the other entry encompasses the first.
 */

static Bool
  GrabMatchesSecond(pFirstGrab, pSecondGrab)

XtServerGrabPtr pFirstGrab, pSecondGrab;

{
    if (pDisplay(pFirstGrab) != pDisplay(pSecondGrab))
      return FALSE;
    
    if (GrabSupersedesSecond(pFirstGrab, pSecondGrab))
      return TRUE;
    
    if (GrabSupersedesSecond(pSecondGrab, pFirstGrab))
      return TRUE;
    
    if (DetailSupersedesSecond(pSecondGrab->detail, 
			       pFirstGrab->detail, 
			       (unsigned short)AnyKey) 	&& 
	DetailSupersedesSecond(pFirstGrab->modifiersDetail, 
			       pSecondGrab->modifiersDetail, 
			       (unsigned short)AnyModifier))
      return TRUE;
    
    if (DetailSupersedesSecond(pFirstGrab->detail, 
			       pSecondGrab->detail, 
			       (unsigned short)AnyKey) && 
	DetailSupersedesSecond(pSecondGrab->modifiersDetail, 
			       pFirstGrab->modifiersDetail, 
			       (unsigned short)AnyModifier))
      return TRUE;
    
    return FALSE;
}


/*
 * Delete a grab combination from the passive grab list.  Each entry will
 * be checked to see if it is affected by the grab being deleted.  This
 * may result in multiple entries being modified/deleted.
 */

static void  DeleteServerGrabFromList(passiveListPtr, 
				       pdi, 
				       pMinuendGrab)
    XtServerGrabPtr 	*passiveListPtr;	
    XtPerDisplayInput	pdi;
    XtServerGrabPtr 	pMinuendGrab;
{
    register XtServerGrabPtr *next;
    register XtServerGrabPtr grab;
    
    for (next = passiveListPtr; *next; )
      {
	  grab = *next;
	  
	  if (GrabMatchesSecond(grab, pMinuendGrab) && 
	      (pDisplay(grab) == pDisplay(pMinuendGrab)))
	    {
		if (GrabSupersedesSecond(pMinuendGrab, grab))
		  {
		      /*
		       * The entry being deleted encompasses the list entry,
		       * so delete the list entry.
		       */
		      *next = grab->next;
		      FreeGrab(grab);
		      continue;
		  }
		
		if ((grab->detail.exact == AnyKey)
		    && (grab->modifiersDetail.exact != AnyModifier))
		  {
		      /*
		       * If the list entry has the key detail of AnyKey, and
		       * a modifier detail not set to AnyModifier, then we
		       * simply need to turn off the key detail bit in the
		       * list entry's key detail mask.
		       */
		      DeleteDetailFromMask(&(grab->detail.pMask),
					   pMinuendGrab->detail.exact);
		  }
		else
		  {
		      if ((grab->modifiersDetail.exact == AnyModifier) 
			  && (grab->detail.exact != AnyKey))
			{
			    /*
			     * The list entry has a specific key detail, but its
			     * modifier detail is set to AnyModifier; so, we only
			     * need to turn off the specified modifier combination
			     * in the list entry's modifier mask.
			     */
			    DeleteDetailFromMask(&(grab->modifiersDetail.pMask),
						 pMinuendGrab->modifiersDetail.exact); 
			}
		      else
			{
			    if ((pMinuendGrab->detail.exact != AnyKey)
				&& (pMinuendGrab->modifiersDetail.exact != AnyModifier))
			      {
				  /*
				   * The list entry has a key detail of AnyKey and a
				   * modifier detail of AnyModifier; the entry being
				   * deleted has a specific key and a specific modifier
				   * combination.  Therefore, we need to mask off the
				   * keycode from the list entry, and also create a
				   * new entry for this keycode, which has a modifier
				   * mask set to AnyModifier & ~(deleted modifiers).
				   */
				  XtServerGrabPtr pNewGrab;
				  
				  DeleteDetailFromMask(&(grab->detail.pMask),
						       pMinuendGrab->detail.exact);
				  
				  pNewGrab = CreateGrab(grab->widget,
							grab->ownerEvents,
							(Modifiers)AnyModifier,
							pMinuendGrab->detail.exact,
							grab->pointerMode,
							grab->keyboardMode, 0, 0, 0);
				  
				  pNewGrab->modifiersDetail.pMask = 
				    CopyDetailMask(grab->modifiersDetail.pMask);
				  
				  DeleteDetailFromMask
				    (&(pNewGrab->modifiersDetail.pMask),
				     pMinuendGrab->modifiersDetail.exact); 
				  
				  AddServerGrabToList(passiveListPtr, pNewGrab);
			      }   
			    else
			      {
				  if (pMinuendGrab->detail.exact == AnyKey)
				    {
					/*
					 * The list entry has keycode AnyKey and modifier
					 * AnyModifier; the entry being deleted has
					 * keycode AnyKey and specific modifiers.  So we
					 * simply need to mask off the specified modifier
					 * combination.
					 */
					DeleteDetailFromMask
					  (&(grab->modifiersDetail.pMask),
					   pMinuendGrab->modifiersDetail.exact);   	
				    }
				  else
				    {
					/*
					 * The list entry has keycode AnyKey and modifier
					 * AnyModifier; the entry being deleted has a
					 * specific keycode and modifier AnyModifier.  So 
					 * we simply need to mask off the specified 
					 * keycode.
					 */
					DeleteDetailFromMask
					  ( &(grab->detail.pMask),
					   pMinuendGrab->detail.exact); 	
				    }
			      }
			}
		      
		  }
	    }
	  next = &((*next)->next);
      }
}

static void DestroyPassiveList(passiveListPtr)
    XtServerGrabPtr	*passiveListPtr;
{
    XtServerGrabPtr	next, grab;

    for (next = *passiveListPtr; next;)
      {
	  grab = next;
	  next = grab->next;
	  
	  /* not necessary to explicitly ungrab key or button;
	   * window is being destroyed so server will take care of it.
	   */ 

	  FreeGrab(grab);
      }
}


/*
 * This function is called at widget destroy time to clean up
 */
/*ARGSUSED*/
void _XtDestroyServerGrabs(w, pwi, call_data)
    Widget		w;
    XtPerWidgetInput	pwi;
    XtPointer		call_data; /* unused */
{
    XtPerDisplayInput		pdi;
    
    pdi = _XtGetPerDisplayInput(XtDisplay(w));
    
    /* Remove the active grab, if necessary */
    if ((pdi->keyboard.grabType != XtNoServerGrab) && 
	(pdi->keyboard.grab.widget == w))
      XtUngrabKeyboard(w, CurrentTime);
    if ((pdi->pointer.grabType != XtNoServerGrab) && 
	(pdi->pointer.grab.widget == w))
      XtUngrabPointer(w, CurrentTime);
    
    DestroyPassiveList(&pwi->keyList);
    DestroyPassiveList(&pwi->ptrList);

    _XtFreePerWidgetInput(w, pwi);
}

/*
 * If the incoming event is on the passive grab list, then activate
 * the grab.  The grab will remain in effect until the key is released.
 */

XtServerGrabPtr _XtCheckServerGrabsOnWidget (event, widget, isKeyboard, pdi)
    XKeyEvent 		* event;
    Widget		widget;
    Boolean		isKeyboard;
    XtPerDisplayInput	pdi;
{
    register XtServerGrabPtr grab;
    XtServerGrabRec 	tempGrab;
    XtServerGrabPtr	*passiveListPtr;
    XtPerWidgetInput	pwi;

    if ((pwi = _XtGetPerWidgetInput(widget, FALSE)) == NULL)
      return (XtServerGrabPtr)0;

    if (isKeyboard)
	  passiveListPtr = &pwi->keyList;
    else
	  passiveListPtr = &pwi->ptrList;

    /*
     * if either there is no entry in the context manager or the entry
     * is empty, or the keyboard is grabed, then no work to be done
     */
    if (*passiveListPtr == 0)
      return (XtServerGrabPtr)0;
    
    
    tempGrab.widget = widget;
    tempGrab.detail.exact = event->keycode; /* also button */
    tempGrab.modifiersDetail.exact = event->state;
    tempGrab.detail.pMask = NULL;
    tempGrab.modifiersDetail.pMask = NULL;

    for (grab = *passiveListPtr; grab; grab = grab->next)
      {
	  if (GrabMatchesSecond(&tempGrab, grab))
	    {
		return (grab);
	    }
      }
    return (XtServerGrabPtr)0;
}






/*
 * This handler is needed to guarantee that we see releases on passive
 * grabs for widgets that haven't selected for keyrelease. Also
 * potentially for tracking focusOuts in order to know that the grab
 * widget has been unmapped.
 */

/*ARGSUSED*/
static void  ActiveHandler (widget, pdi, event)
    Widget 		widget;
    XtPerDisplayInput	pdi;
    XEvent 		*event;

{
    
}



/*
 *	MakeGrab
 */
static void  MakeGrab(grab, passiveListPtr, isKeyboard, pdi, pwi)
    XtServerGrabPtr	grab;
    XtServerGrabPtr	*passiveListPtr;
    Boolean		isKeyboard;
    XtPerDisplayInput	pdi;
    XtPerWidgetInput	pwi;
{
    Mask	mask = (isKeyboard ? 
			(KeyPressMask | KeyReleaseMask) :
			(ButtonPressMask | ButtonReleaseMask));
    
    
    if (!pwi->active_handler_added)
      {
	  XtAddEventHandler(grab->widget, mask, FALSE,
			    ActiveHandler,
			    (XtPointer)pdi);
	  pwi->active_handler_added = TRUE;
      }
	
    if (isKeyboard)
      XGrabKey(pDisplay(grab),
	       grab->detail.exact, grab->modifiersDetail.exact, 
	       pWindow(grab), grab->ownerEvents,
	       grab->pointerMode, grab->keyboardMode);
    
    else
      {
      XGrabButton(pDisplay(grab),
		  grab->detail.exact, grab->modifiersDetail.exact, 
		  pWindow(grab), grab->ownerEvents, grab->eventMask,
		  grab->pointerMode, grab->keyboardMode,
		  grab->confineTo, grab->cursor);
      }

#ifdef notdef
    /* Delete any existing grabs which will be superseded by the new grab */
    DeleteServerGrabFromList(passiveListPtr, pdi, grab);
#endif /* notdef */

    /* Add the new grab entry to the passive key grab list */
    AddServerGrabToList(passiveListPtr, grab);
}

static void MakeGrabs(passiveListPtr, isKeyboard, pdi)
    XtServerGrabPtr	*passiveListPtr;
    Boolean		isKeyboard;
    XtPerDisplayInput	pdi;
{
    XtServerGrabPtr	next = *passiveListPtr;
    XtServerGrabPtr	grab;
    XtPerWidgetInput	pwi;
    /*
     * make MakeGrab build a new list that has had the merge
     * processing done on it. Start with an empty list
     * (passiveListPtr).
     */
    *passiveListPtr = NULL;
    while (next)
      {
	  grab = next;
	  next = grab->next;
	  pwi = _XtGetPerWidgetInput(grab->widget, FALSE);
	  MakeGrab(grab, passiveListPtr, isKeyboard, pdi, pwi);
      }
} 
   
/*
 * This function is the event handler attached to the associated widget
 * when grabs need to be added, but the widget is not yet realized.  When
 * it is first mapped, this handler will be invoked, and it will add all
 * needed grabs.
 */

/*ARGSUSED*/
static void  RealizeHandler (widget, pwi, event)
    Widget 		widget;
    XtPerWidgetInput	pwi;
    XEvent 		*event;	/* unused */

{
    XtPerDisplayInput	pdi;
    
    pdi = _XtGetPerDisplayInput(XtDisplay(widget));
    
    MakeGrabs(&pwi->keyList, KEYBOARD, pdi);
    MakeGrabs(&pwi->ptrList, POINTER, pdi);
 
    XtRemoveEventHandler(widget, XtAllEvents, True,
			 RealizeHandler, (XtPointer)pwi);
    pwi->realize_handler_added = FALSE;
}

/***************************************************************************/
/**************************** Global Routines ******************************/
/***************************************************************************/


/*
 * Routine used by an application to set up a passive grab for a key/modifier
 * combination.
 */

static
void GrabKeyOrButton (widget, keyOrButton, modifiers, owner_events,
		       pointer_mode, keyboard_mode, event_mask,
		       confine_to, cursor, isKeyboard)
    Widget	widget;
    KeyCode	keyOrButton;
    Modifiers	modifiers;
    Boolean	owner_events;
    int 	pointer_mode;
    int 	keyboard_mode;
    Mask	event_mask;
    Window 	confine_to;
    Cursor 	cursor;
    Boolean	isKeyboard;
{
    XtServerGrabPtr	*passiveListPtr;
    XtServerGrabPtr 	newGrab, *nextPtr;
    XtPerWidgetInput	pwi;
    XtPerDisplayInput	pdi;
    
    
    if (!XtIsWidget(widget)){
	XtAppWarningMsg(XtWidgetToApplicationContext(widget),
		     "invalidWidget", "grabKeyOrButton", "XtToolkitError",
		     "Widget specified in grab is not a widget",
		     (String *)NULL, (Cardinal *)NULL);
	return;
    }
    
    pwi = _XtGetPerWidgetInput(widget, TRUE);
    if (isKeyboard)
      passiveListPtr = &pwi->keyList;
    else
      passiveListPtr = &pwi->ptrList;
    pdi = _XtGetPerDisplayInput(XtDisplay(widget));
    
    newGrab = CreateGrab(widget, owner_events, modifiers, 
			 keyOrButton, pointer_mode, keyboard_mode, 
			 event_mask, confine_to, cursor);
    /*
     *  if the widget is realized then process the entry into the grab
     * list. else if the list is empty (i.e. first time) then add the
     * event handler. then add the raw entry to the list for processing
     * in the handler at realize time.
     */
    if (XtIsRealized(widget))
      MakeGrab(newGrab, passiveListPtr, isKeyboard, pdi, pwi);
    else {
	if (!pwi->realize_handler_added)
	    {
		XtAddEventHandler(widget, StructureNotifyMask, FALSE,
				  RealizeHandler,
				  (XtPointer)pwi);
		pwi->realize_handler_added = TRUE;
	    }
	
	for (nextPtr = passiveListPtr;
	     *nextPtr; 
	     nextPtr = (XtServerGrabPtr *)&((*nextPtr)->next)) {};
	*nextPtr = newGrab;
    }
}


static
void   UngrabKeyOrButton (widget, keyOrButton, modifiers, isKeyboard)
    Widget	widget;
    int		keyOrButton;
    Modifiers	modifiers;
    Boolean	isKeyboard;
{
    XtServerGrabRec 	tempGrab;
    XtPerWidgetInput	pwi;
    
    if (!XtIsWidget(widget)){
	XtAppWarningMsg(XtWidgetToApplicationContext(widget),
		     "invalidWidget", "ungrabKeyOrButton", "XtToolkitError",
		     "Widget specified in ungrab is not a widget",
		     (String *)NULL, (Cardinal *)NULL);
	return;
    }
    
    /* Build a temporary grab list entry */
    tempGrab.widget = widget;
    tempGrab.modifiersDetail.exact = modifiers;
    tempGrab.modifiersDetail.pMask = NULL;
    tempGrab.detail.exact = keyOrButton;
    tempGrab.detail.pMask = NULL;
    
    
    pwi = _XtGetPerWidgetInput(widget, FALSE);
    
    /*
     * if there is no entry in the context manager then somethings wrong
     */
    if (!pwi)
      {
	  XtAppWarningMsg(XtWidgetToApplicationContext(widget),
		       "invalidGrab", "ungrabKeyOrButton", "XtToolkitError",
		       "Attempt to remove non-existant passive grab",
		       (String *)NULL, (Cardinal *)NULL);
	  return;
      }

    if (XtIsRealized(widget))
      {
	  if (isKeyboard)
	    XUngrabKey(widget->core.screen->display,
		       keyOrButton, (unsigned int)modifiers,
		       widget->core.window);
	  else
	    XUngrabButton(widget->core.screen->display,
			  keyOrButton, (unsigned int)modifiers, 
			  widget->core.window);
      }

   
    /* Delete all entries which are encompassed by the specified grab. */
    DeleteServerGrabFromList(isKeyboard ? &pwi->keyList : &pwi->ptrList,
			     _XtGetPerDisplayInput(XtDisplay(widget)), 
			     &tempGrab);
}


void  XtGrabKey (widget, keycode, modifiers, owner_events,
		 pointer_mode, keyboard_mode)
    Widget	widget;
    KeyCode	keycode;
    Modifiers	modifiers;
    Boolean	owner_events;
    int 	pointer_mode;
    int 	keyboard_mode;
    
{
    GrabKeyOrButton(widget, keycode, modifiers, owner_events,
		    pointer_mode, keyboard_mode, 
		    (Mask)0, (Window)None, (Cursor)None, KEYBOARD);
}

void  XtGrabButton(widget, button, modifiers, owner_events,
		   pointer_mode, keyboard_mode,
		   event_mask, confine_to, cursor)
    Widget	widget;
    int		button;
    Modifiers	modifiers;
    Boolean	owner_events;
    unsigned int event_mask;
    int 	pointer_mode;
    int 	keyboard_mode;
    Window 	confine_to;
    Cursor 	cursor;
{
    GrabKeyOrButton(widget, (KeyCode)button, modifiers, owner_events,
		    pointer_mode, keyboard_mode, 
		    event_mask, confine_to, cursor, POINTER);
}


/*
 * Routine used by an application to clear a passive grab for a key/modifier
 * combination.
 */

void   XtUngrabKey (widget, keycode, modifiers)
    Widget	widget;
    KeyCode	keycode;
    Modifiers	modifiers;
{

    UngrabKeyOrButton(widget, (int)keycode, modifiers, KEYBOARD);
}

void   XtUngrabButton (widget, button, modifiers)
    Widget	widget;
    int		button;
    Modifiers	modifiers;
{

    UngrabKeyOrButton(widget, button, modifiers, POINTER);
}

/*
 * Active grab of Device. clear any client side grabs so we dont lock
 */
static int GrabDevice (widget, owner_events,
		       pointer_mode, keyboard_mode, 
		       event_mask, confine_to, cursor, time, isKeyboard)
    Widget	widget;
    Boolean	owner_events;
    int 	pointer_mode;
    int 	keyboard_mode;
    Mask	event_mask;
    Window 	confine_to;
    Cursor 	cursor;
    Time	time;
    Boolean	isKeyboard;
{
    XtPerDisplayInput	pdi;
    int			returnVal;
    
    if (!XtIsWidget(widget) || !XtIsRealized(widget))
      XtAppErrorMsg(XtWidgetToApplicationContext(widget),
		    "invalidWidget", "grabDevice", "XtToolkitError",
		    "Grab widget must be a realized widget",
		    (String*)NULL, (Cardinal*)NULL);
    
    pdi = _XtGetPerDisplayInput(XtDisplay(widget));
    
    if (!isKeyboard)
      returnVal = XGrabPointer(XtDisplay(widget), XtWindow(widget), 
			       owner_events, event_mask,
			       pointer_mode, keyboard_mode,
			       confine_to, cursor, time);
    else
      returnVal = XGrabKeyboard(XtDisplay(widget), XtWindow(widget), 
				owner_events, pointer_mode, 
				keyboard_mode, time);

    if (returnVal == GrabSuccess)
      {	
	  XtServerGrabPtr	grab;
	  XtDevice		device;
	  
	  device = isKeyboard ? &pdi->keyboard : &pdi->pointer;
	  grab =  &device->grab;

	  /* fill in the server grab rec */
	  grab->widget = widget;
	  grab->modifiersDetail.exact = 0;
	  grab->modifiersDetail.pMask = NULL;
	  grab->detail.exact = 0;
	  grab->detail.pMask = NULL;
	  grab->ownerEvents = owner_events;
	  grab->pointerMode = pointer_mode;
	  grab->keyboardMode = keyboard_mode;

	  device->grabType = XtActiveServerGrab;
	  pdi->activatingKey = (KeyCode)0;
      }
    return returnVal;
}

static void   UngrabDevice(widget, time, isKeyboard)
    Widget	widget;
    Time	time;
    Boolean	isKeyboard;
{
    XtPerDisplayInput	pdi = _XtGetPerDisplayInput(XtDisplay(widget));
    XtDevice		device = isKeyboard ? &pdi->keyboard : &pdi->pointer;

    if (!XtIsWidget(widget) || !XtIsRealized(widget))
      XtAppErrorMsg(XtWidgetToApplicationContext(widget),
		    "invalidWidget", "ungrabDevice", "XtToolkitError",
		    "Grab widget must be a realized widget",
		    (String*)NULL, (Cardinal*)NULL);
     
    if (device->grabType != XtNoServerGrab)
      {
	  if (device->grabType != XtPseudoPassiveServerGrab)
	    {
		if (isKeyboard)
		  XUngrabKeyboard(XtDisplay(widget), time);
		else
		  XUngrabPointer(XtDisplay(widget), time);
	    }
	  device->grabType = XtNoServerGrab;
	  pdi->activatingKey = (KeyCode)0;
      }
}


/*
 * Active grab of keyboard. clear any client side grabs so we dont lock
 */
int XtGrabKeyboard (widget, owner_events,
		    pointer_mode, keyboard_mode, time)
    Widget	widget;
    Boolean	owner_events;
    int 	pointer_mode;
    int 	keyboard_mode;
    Time	time;
{
    return (GrabDevice (widget, owner_events,
			pointer_mode, keyboard_mode, 
			(Mask)0, (Window)None, (Cursor)None, time, KEYBOARD));
}


/*
 * Ungrab the keyboard
 */

void   XtUngrabKeyboard(widget, time)
    Widget	widget;
    Time	time;
{
    UngrabDevice(widget, time, KEYBOARD);
}




/*
 * grab the pointer
 */
int XtGrabPointer (widget, owner_events, event_mask,
		   pointer_mode, keyboard_mode, 
		   confine_to, cursor, time)
    Widget	widget;
    Boolean	owner_events;
    unsigned int event_mask;
    int 	pointer_mode;
    int 	keyboard_mode;
    Window 	confine_to;
    Cursor 	cursor;
    Time	time;
{
    return (GrabDevice (widget, owner_events,
			pointer_mode, keyboard_mode, 
			event_mask, confine_to, 
			cursor, time, POINTER));
}


/*
 * Ungrab the pointer
 */

void   XtUngrabPointer(widget, time)
    Widget	widget;
    Time	time;
{
    UngrabDevice(widget, time, POINTER);
}


