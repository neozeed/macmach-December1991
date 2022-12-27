#ifndef lint
static char Xrcsid[] = "$XConsortium: NextEvent.c,v 1.83 90/04/05 11:43:46 swick Exp $";
/* $oHeader: NextEvent.c,v 1.4 88/09/01 11:43:27 asente Exp $ */
#endif /* lint */

/***********************************************************
Copyright 1987, 1988 by Digital Equipment Corporation, Maynard, Massachusetts,
and the Massachusetts Institute of Technology, Cambridge, Massachusetts.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the names of Digital or MIT not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  

DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/

#include <stdio.h>
#include <errno.h>
#include <X11/Xlib.h>
#include "IntrinsicI.h"

extern int errno;

static TimerEventRec* freeTimerRecs;
static WorkProcRec* freeWorkRecs;

#if defined(USG) && !defined(CRAY)
static int gettimeofday (tvp, tzp)
    struct timeval *tvp;
    struct timezone *tzp;
{
    time (&tvp->tv_sec);
    tvp->tv_usec = 0L;

    /* ignore tzp for now since this file doesn't use it */
}
#endif

/* Some systems running NTP daemons are known to return strange usec
 * values from gettimeofday.  At present (3/90) this has only been
 * reported on SunOS...
 */

#ifndef NEEDS_NTPD_FIXUP
# ifdef sun
#  define NEEDS_NTPD_FIXUP 1
# else
#  define NEEDS_NTPD_FIXUP 0
# endif
#endif

#if NEEDS_NTPD_FIXUP
#define FIXUP_TIMEVAL(t) { \
	while ((t).tv_usec >= 1000000) { \
	    (t).tv_usec -= 1000000; \
	    (t).tv_sec++; \
	} \
	while ((t).tv_usec < 0) { \
	    if ((t).tv_sec > 0) { \
		(t).tv_usec += 1000000; \
		(t).tv_sec--; \
	    } else { \
		(t).tv_usec = 0; \
		break; \
	    } \
	}}
#else
#define FIXUP_TIMEVAL(t)
#endif /*NEEDS_NTPD_FIXUP*/



/*
 * Private routines
 */
#define ADD_TIME(dest, src1, src2) { \
	if(((dest).tv_usec = (src1).tv_usec + (src2).tv_usec) >= 1000000) {\
	      (dest).tv_usec -= 1000000;\
	      (dest).tv_sec = (src1).tv_sec + (src2).tv_sec + 1 ; \
	} else { (dest).tv_sec = (src1).tv_sec + (src2).tv_sec ; \
	   if(((dest).tv_sec >= 1) && (((dest).tv_usec <0))) { \
	    (dest).tv_sec --;(dest).tv_usec += 1000000; } } }


#define TIMEDELTA(dest, src1, src2) { \
	if(((dest).tv_usec = (src1).tv_usec - (src2).tv_usec) < 0) {\
	      (dest).tv_usec += 1000000;\
	      (dest).tv_sec = (src1).tv_sec - (src2).tv_sec - 1;\
	} else 	(dest).tv_sec = (src1).tv_sec - (src2).tv_sec;  }

#define IS_AFTER(t1, t2) (((t2).tv_sec > (t1).tv_sec) \
	|| (((t2).tv_sec == (t1).tv_sec)&& ((t2).tv_usec > (t1).tv_usec)))

static void QueueTimerEvent(app, ptr)
    XtAppContext app;
    TimerEventRec *ptr;
{
        TimerEventRec *t,**tt;
        tt = &app->timerQueue;
        t  = *tt;
        while (t != NULL &&
                IS_AFTER(t->te_timer_value, ptr->te_timer_value)) {
          tt = &t->te_next;
          t  = *tt;
         }
         ptr->te_next = t;
         *tt = ptr;
}

/* 
 * Ultrix routine to block in the toolkit.  This should be the only call to 
 * select..
 *
 * This routine returns when there is something to be done
 *
 * Before calling this with ignoreInputs==False, app->outstandingQueue should
 * be checked; this routine will not verify that an alternate input source
 * has not already been enqueued.
 *
 *
 * _XtWaitForSomething( ignoreTimers, ignoreInputs, ignoreEvents,
 *			block, howlong, appContext)
 * Boolean ignoreTimers;     (Don't return if a timer would fire
 *				Also implies forget timers exist)
 *
 * Boolean ignoreInputs;     (Ditto for input callbacks )
 *
 * Boolean ignoreEvents;     (Ditto for X events)
 *
 * Boolean block;	     (Okay to block)
 * TimeVal howlong;	     (howlong to wait for if blocking and not
 *				doing Timers... Null mean forever.
 *				Maybe should mean shortest of both)
 * XtAppContext app;	     (Displays to check wait on)
 * Returns display for which input is available, if any
 * and if ignoreEvents==False, else returns -1
 *
 * if ignoring everything && block=True && howlong=NULL, you'll have
 * lots of time for coffee; better not try it!  In fact, it probably
 * makes little sense to do this regardless of the value of howlong
 * (bottom line is, we don't bother checking here).
 */
int _XtwaitForSomething(ignoreTimers, ignoreInputs, ignoreEvents,
			block, howlong, app)
	Boolean ignoreTimers;
	Boolean ignoreInputs;
	Boolean ignoreEvents;
	Boolean block;
	unsigned long *howlong;
	XtAppContext app;
{
	struct timezone cur_timezone;
	struct timeval  cur_time;
	struct timeval  start_time;
	struct timeval  wait_time;
	struct timeval  new_time;
	struct timeval  time_spent;
	struct timeval	max_wait_time;
	static struct timeval  zero_time = { 0 , 0};
	register struct timeval *wait_time_ptr;
	Fd_set rmaskfd, wmaskfd, emaskfd;
	static Fd_set zero = { 0 };
	int nfound, i, d, last_d_with_no_events = -1;
#ifdef DEBUG_SELECT
	int loop_count = -1;
#endif
	
 	if (block) {
		(void) gettimeofday (&cur_time, &cur_timezone);
		FIXUP_TIMEVAL(cur_time);
		start_time = cur_time;
		if(howlong == NULL) { /* special case for ever */
			wait_time_ptr = 0;
		} else { /* block until at most */
			max_wait_time.tv_sec = *howlong/1000;
			max_wait_time.tv_usec = (*howlong %1000)*1000;
			wait_time_ptr = &max_wait_time;
		}
	} else {  /* don't block */
		max_wait_time = zero_time;
		wait_time_ptr = &max_wait_time;
	}

      WaitLoop:
	while (1) {
#ifdef DEBUG_SELECT
	        loop_count++;
#endif
		if (app->timerQueue != NULL && !ignoreTimers && block) {
		    if(IS_AFTER(cur_time, app->timerQueue->te_timer_value)) {
			TIMEDELTA (wait_time, app->timerQueue->te_timer_value, 
				   cur_time);
			if(howlong==NULL || IS_AFTER(wait_time,max_wait_time)){
				wait_time_ptr = &wait_time;
			} else {
				wait_time_ptr = &max_wait_time;
			}
		    } else wait_time_ptr = &zero_time;
		} 
		if( !ignoreInputs ) {
			rmaskfd = app->fds.rmask;
			wmaskfd = app->fds.wmask;
			emaskfd = app->fds.emask;
		} else {
			rmaskfd = zero;
			wmaskfd = zero;
			emaskfd = zero;
		}
		if (!ignoreEvents) {
		    for (d = 0; d < app->count; d++) {
			FD_SET (ConnectionNumber(app->list[d]), &rmaskfd);
		    }
		}
		nfound = select (app->fds.nfds, (int *) &rmaskfd,
			(int *) &wmaskfd, (int *) &emaskfd, wait_time_ptr);
		if (nfound == -1) {
			/*
			 *  interrupt occured recalculate time value and select
			 *  again.
			 */
			if (errno == EINTR) {
			    errno = 0;  /* errno is not self reseting */
			    if(block && wait_time_ptr != NULL) {
				(void)gettimeofday (&new_time, &cur_timezone);
				FIXUP_TIMEVAL(new_time);
				TIMEDELTA(time_spent, new_time, cur_time);
				cur_time = new_time;
				if(IS_AFTER(time_spent, *wait_time_ptr)) {
					TIMEDELTA(wait_time, *wait_time_ptr,
						  time_spent);
					wait_time_ptr = &wait_time;
					continue;
				} else {
					/* time is up anyway */
					nfound = 0;
				}
			    }
			} else {
			    char Errno[100];
			    String param = Errno;
			    Cardinal param_count = 1;
			    sprintf( Errno, "%d", errno);
			    XtAppWarningMsg(app, "communicationError","select",
			       XtCXtToolkitError,"Select failed; error code %s",
			       &param, &param_count);
#ifdef DEBUG_SELECT
			    if (errno == EINVAL && wait_time_ptr != NULL) {
				char msg[1000];
				sprintf( msg, "howlong = %#x, wait_time = {%d, %d}, loop_count = %d",
					 howlong ? *howlong : 0,
					 wait_time_ptr->tv_sec,
					 wait_time_ptr->tv_usec,
					 loop_count );
				XtWarning( msg );
			    }
#endif /*DEBUG_SELECT*/
			    continue;
			}
		} /* timed out or input available */
		break;
	}
	
	if (nfound == 0) {
		if(howlong) *howlong = (unsigned long)0;  /* Timed out */
		return -1;
	}
	if(block && howlong != NULL) { /* adjust howlong */
	    (void) gettimeofday (&new_time, &cur_timezone);
	    FIXUP_TIMEVAL(new_time);
	    TIMEDELTA(time_spent, new_time, start_time);
	    if(*howlong <= (time_spent.tv_sec*1000+time_spent.tv_usec/1000))
		*howlong = (unsigned long)0;  /* Timed out */
	    else
		*howlong -= (time_spent.tv_sec*1000+time_spent.tv_usec/1000);
	}
	if(ignoreInputs) {
	    if (ignoreEvents) return -1; /* then only doing timers */
	    for (d = 0; d < app->count; d++) {
		if (FD_ISSET(ConnectionNumber(app->list[d]), &rmaskfd)) {
		    /*
		     * Have to be VERY careful here:  an error event
		     * could have arrived without any real events, or
		     * the stream may have been closed.  XEventsQueued
		     * will process any error events but there's no way
		     * to find out from Xlib that there aren't enough
		     * bytes on the wire.  We save enough state to
		     * notice that XEventsQueued has returned 0 more
		     * than once for the same fd and if so go ahead and
		     * let the Xlib IO processing handle things.
		     */
		    if (XEventsQueued( app->list[d], QueuedAfterReading )
			|| last_d_with_no_events == d)
			return d;
		    if (last_d_with_no_events == -1)
			last_d_with_no_events = d;
		}
	    }
	    goto WaitLoop;	/* must have been only error events */
        }
	{
	int ret = -1;
	Boolean found_input = False;

	for (i = 0; i < app->fds.nfds && nfound > 0; i++) {
	    if (FD_ISSET (i, &rmaskfd)) {
		nfound--;
		if (!ignoreEvents) {
		    for (d = 0; d < app->count; d++) {
			if (i == ConnectionNumber(app->list[d])) {
			    if (ret == -1) {
				/*
				 * An error event could have arrived
				 * without any real events, or the
				 * stream may have been closed.
				 */
				if (XEventsQueued( app->list[d],
						   QueuedAfterReading )
				    || last_d_with_no_events == d)
				    ret = d;
				else if (last_d_with_no_events == -1)
				    last_d_with_no_events = d;
			    }
			    goto ENDILOOP;
			}
		    }
		}

		app->selectRqueue[i]->ie_oq = app->outstandingQueue;
		app->outstandingQueue = app->selectRqueue[i];
		found_input = True;
	    }
	    if (FD_ISSET (i, &wmaskfd)) {
		app->selectWqueue[i]->ie_oq = app->outstandingQueue;
		app->outstandingQueue = app->selectWqueue[i];
		nfound--;
		found_input = True;
	    }
	    if (FD_ISSET (i, &emaskfd)) {
		app->selectEqueue[i]->ie_oq = app->outstandingQueue;
		app->outstandingQueue = app->selectEqueue[i];
		nfound--;
		found_input = True;
	    }
ENDILOOP:   ;
	} /* endfor */
	if (ret >= 0 || found_input)
	    return ret;
	goto WaitLoop;		/* must have been only error events */
	}
}

static void IeCallProc(ptr)
	InputEvent *ptr;
{
    while (ptr != NULL) {
	InputEvent *next = ptr->ie_next;
	(*ptr->ie_proc)(ptr->ie_closure, &ptr->ie_source, (XtInputId*)&ptr);
	ptr = next;
    }
}

static void TeCallProc(ptr)
	TimerEventRec *ptr;
{
	(* (ptr->te_proc))( ptr->te_closure, (XtIntervalId*)&ptr);
}

/*
 * Public Routines
 */

XtIntervalId XtAddTimeOut(interval, proc, closure)
	unsigned long interval;
	XtTimerCallbackProc proc;
	XtPointer closure;
{
	return XtAppAddTimeOut(_XtDefaultAppContext(), 
		interval, proc, closure); 
}


XtIntervalId XtAppAddTimeOut(app, interval, proc, closure)
	XtAppContext app;
	unsigned long interval;
	XtTimerCallbackProc proc;
	XtPointer closure;
{
	TimerEventRec *tptr;
        struct timeval current_time;
	struct timezone timezone;

	if (freeTimerRecs) {
	    tptr = freeTimerRecs;
	    freeTimerRecs = tptr->te_next;
	}
	else tptr = XtNew(TimerEventRec);

	tptr->te_next = NULL;
	tptr->te_closure = closure;
	tptr->te_proc = proc;
	tptr->app = app;
	tptr->te_timer_value.tv_sec = interval/1000;
	tptr->te_timer_value.tv_usec = (interval%1000)*1000;
        (void) gettimeofday(&current_time,&timezone);
	FIXUP_TIMEVAL(current_time);
        ADD_TIME(tptr->te_timer_value,tptr->te_timer_value,current_time);
	QueueTimerEvent(app, tptr);
	return( (XtIntervalId) tptr);
}

void  XtRemoveTimeOut(id)
    XtIntervalId id;
{
   TimerEventRec *t, *last, *tid = (TimerEventRec *) id;

   /* find it */

   for(t = tid->app->timerQueue, last = NULL;
	   t != NULL && t != tid;
	   t = t->te_next) last = t;

   if (t == NULL) return; /* couldn't find it */
   if(last == NULL) { /* first one on the list */
       t->app->timerQueue = t->te_next;
   } else last->te_next = t->te_next;

   t->te_next = freeTimerRecs;
   freeTimerRecs = t;
   return;
}

XtWorkProcId XtAddWorkProc(proc, closure)
	XtWorkProc proc;
	XtPointer closure;
{
	return XtAppAddWorkProc(_XtDefaultAppContext(), proc, closure);
}

XtWorkProcId XtAppAddWorkProc(app, proc, closure)
	XtAppContext app;
	XtWorkProc proc;
	XtPointer closure;
{
	WorkProcRec *wptr;

	if (freeWorkRecs) {
	    wptr = freeWorkRecs;
	    freeWorkRecs = wptr->next;
	} else wptr = XtNew(WorkProcRec);

	wptr->next = app->workQueue;
	wptr->closure = closure;
	wptr->proc = proc;
	wptr->app = app;
	app->workQueue = wptr;

	return (XtWorkProcId) wptr;
}

void  XtRemoveWorkProc(id)
	XtWorkProcId id;
{
	WorkProcRec *wid= (WorkProcRec *) id, *w, *last;

	/* find it */
	for(w = wid->app->workQueue, last = NULL; w != NULL && w != wid; w = w->next) last = w;

	if (w == NULL) return; /* couldn't find it */

	if(last == NULL) wid->app->workQueue = w->next;
	else last->next = w->next;

	w->next = freeWorkRecs;
	freeWorkRecs = w;
}

XtInputId XtAddInput( source, Condition, proc, closure)
	int source;
	XtPointer Condition;
	XtInputCallbackProc proc;
	XtPointer closure;
{
	return XtAppAddInput(_XtDefaultAppContext(),
		source, Condition, proc, closure);
}

XtInputId XtAppAddInput(app, source, Condition, proc, closure)
	XtAppContext app;
	int source;
	XtPointer Condition;
	XtInputCallbackProc proc;
	XtPointer closure;
{
	InputEvent *sptr;
	XtInputMask condition = (XtInputMask) Condition;
	
#define CondAllocateQueue(queue) \
	if (queue == NULL) { \
	    queue = (InputEvent**) \
		_XtHeapAlloc(&app->heap,(Cardinal)NOFILE*sizeof(InputEvent*));\
	    bzero( (char*)queue, (unsigned)NOFILE*sizeof(InputEvent*) ); \
	}

	sptr = XtNew(InputEvent);
	if(condition == XtInputReadMask){
	    CondAllocateQueue(app->selectRqueue);
	    sptr->ie_next = app->selectRqueue[source];
	    app->selectRqueue[source] = sptr;
	    FD_SET(source, &app->fds.rmask);
	} else if(condition == XtInputWriteMask) {
	    CondAllocateQueue(app->selectWqueue);
	    sptr->ie_next = app->selectWqueue[source];
	    app->selectWqueue[source] = sptr;
	    FD_SET(source, &app->fds.wmask);
	} else if(condition == XtInputExceptMask) {
	    CondAllocateQueue(app->selectEqueue);
	    sptr->ie_next = app->selectEqueue[source];
	    app->selectEqueue[source] = sptr;
	    FD_SET(source, &app->fds.emask);
	} else
	  XtAppErrorMsg(app, "invalidParameter","xtAddInput",XtCXtToolkitError,
                  "invalid condition passed to XtAddInput",
                   (String *)NULL, (Cardinal *)NULL);
	sptr->ie_proc = proc;
	sptr->ie_closure =closure;
	sptr->app = app;
	sptr->ie_oq = NULL;
	sptr->ie_source = source;
	
	if (app->fds.nfds < (source+1)) app->fds.nfds = source+1;
	app->fds.count++;
	return((XtInputId)sptr);

}

void XtRemoveInput( id )
	register XtInputId  id;
{
  	register InputEvent *sptr, *lptr;
	XtAppContext app = ((InputEvent *)id)->app;
	register int source = ((InputEvent *)id)->ie_source;
	app->fds.count--;

	sptr = app->outstandingQueue;
	lptr = NULL;
	for (; sptr != NULL; sptr = sptr->ie_oq) {
	    if (sptr == (InputEvent *)id) {
		if (lptr == NULL) app->outstandingQueue = sptr->ie_oq;
		else lptr->ie_oq = sptr->ie_oq;
	    }
	    lptr = sptr;
	}

	if(app->selectRqueue && (sptr = app->selectRqueue[source]) != NULL) {
		for( lptr = NULL ; sptr; sptr = sptr->ie_next ){
			if(sptr == (InputEvent *) id) {
				if(lptr == NULL) {
					app->selectRqueue[source] = sptr->ie_next;
					FD_CLR(source, &app->fds.rmask);
				} else {
					lptr->ie_next = sptr->ie_next;
				}
				XtFree((char *) sptr);
				return;
			}
			lptr = sptr;	      
		}
	}
	if(app->selectWqueue && (sptr = app->selectWqueue[source]) != NULL) {
		for(lptr = NULL;sptr; sptr = sptr->ie_next){
			if ( sptr ==  (InputEvent *) id) {
				if(lptr == NULL){
					app->selectWqueue[source] = sptr->ie_next;
					FD_CLR(source, &app->fds.wmask);
				}else {
					lptr->ie_next = sptr->ie_next;
				}
				XtFree((char *) sptr);
				return;
			}
			lptr = sptr;
		}
	    
	}
	if(app->selectEqueue && (sptr = app->selectEqueue[source]) != NULL) {
		for(lptr = NULL;sptr; sptr = sptr->ie_next){
			if ( sptr ==  (InputEvent *) id) {
				if(lptr == NULL){
					app->selectEqueue[source] = sptr->ie_next;
					FD_CLR(source, &app->fds.emask);
				}else {
					lptr->ie_next = sptr->ie_next;
				}
				XtFree((char *) sptr);
				return;
			}
			lptr = sptr;
		}
	    
	}
	XtAppWarningMsg(app, "invalidProcedure","inputHandler",XtCXtToolkitError,
                   "XtRemoveInput: Input handler not found",
		   (String *)NULL, (Cardinal *)NULL);
	app->fds.count++;	/* Didn't remove it after all */
}

/* Do alternate input and timer callbacks if there are any */

static void DoOtherSources(app)
	XtAppContext app;
{
	register TimerEventRec *te_ptr;
	register InputEvent *ie_ptr;
	struct timeval  cur_time;
	struct timezone cur_timezone;

#define DrainQueue() \
	for (ie_ptr = app->outstandingQueue; ie_ptr != NULL;) { \
	    app->outstandingQueue = ie_ptr->ie_oq;		\
	    ie_ptr ->ie_oq = NULL;				\
	    IeCallProc(ie_ptr);					\
	    ie_ptr = app->outstandingQueue;			\
	}
/*enddef*/
	DrainQueue();
	if (app->fds.count > 0) {
	    /* Call _XtwaitForSomething to get input queued up */
	    (void) _XtwaitForSomething(TRUE, FALSE, TRUE, FALSE,
		(unsigned long *)NULL, app);
	    DrainQueue();
	}
	if (app->timerQueue != NULL) {	/* check timeout queue */
	    (void) gettimeofday (&cur_time, &cur_timezone);
	    FIXUP_TIMEVAL(cur_time);
	    while(IS_AFTER (app->timerQueue->te_timer_value, cur_time)) {
		te_ptr = app->timerQueue;
		app->timerQueue = te_ptr->te_next;
		te_ptr->te_next = NULL;
		if (te_ptr->te_proc != NULL)
		    TeCallProc(te_ptr);
		te_ptr->te_next = freeTimerRecs;
		freeTimerRecs = te_ptr;
              if (app->timerQueue == NULL) break;
	    }
	}
#undef DrainQueue
}

/* If there are any work procs, call them.  Return whether we did so */

static Boolean CallWorkProc(app)
	XtAppContext app;
{
	register WorkProcRec *w = app->workQueue;
	Boolean delete;

	if (w == NULL) return FALSE;

	app->workQueue = w->next;

	delete = (*(w->proc)) (w->closure);

	if (delete) {
	    w->next = freeWorkRecs;
	    freeWorkRecs = w->next;
	}
	else {
	    w->next = app->workQueue;
	    app->workQueue = w;
	}
	return TRUE;
}

/*
 * XtNextEvent()
 * return next event;
 */

void XtNextEvent(event)
	XEvent *event;
{
	XtAppNextEvent(_XtDefaultAppContext(), event);
}

static void _RefreshMapping(event)
    XEvent *event;
{
    XtPerDisplay pd = _XtGetPerDisplay(event->xmapping.display);
    if (pd != NULL) {
	if (pd->keysyms != NULL)
	    _XtBuildKeysymTables( event->xmapping.display, pd );
	if (pd->mapping_callbacks != NULL)
	    _XtCallCallbacks( &pd->mapping_callbacks, (XtPointer)event );
    }
    XRefreshKeyboardMapping(&event->xmapping);
}

void XtAppNextEvent(app, event)
	XtAppContext app;
	XEvent *event;
{
    int i, d;

    for (;;) {
	if (app->count == 0)
	    DoOtherSources(app);
	else {
	    for (i = 1; i <= app->count; i++) {
		d = (i + app->last) % app->count;
		if (d == 0) DoOtherSources(app);
		if (XEventsQueued(app->list[d], QueuedAfterReading))
		    goto GotEvent;
	    }
	    for (i = 1; i <= app->count; i++) {
		d = (i + app->last) % app->count;
		if (XEventsQueued(app->list[d], QueuedAfterFlush))
		    goto GotEvent;
	    }
	}

	/* We're ready to wait...if there is a work proc, call it */
	if (CallWorkProc(app)) continue;

	d = _XtwaitForSomething(FALSE, FALSE, FALSE, TRUE,
				(unsigned long *) NULL, app);

	if (d != -1) {
	  GotEvent:
	    XNextEvent (app->list[d], event);
	    app->last = d;
	    if (event->xany.type == MappingNotify)
		_RefreshMapping(event);
	    return;
	} 

    } /* for */
}
    
void XtProcessEvent(mask)
	XtInputMask mask;
{
	void XtAppProcessEvent();
	XtAppProcessEvent(_XtDefaultAppContext(), mask);
}

void XtAppProcessEvent(app, mask)
	XtAppContext app;
	XtInputMask mask;
{
	InputEvent *ie_ptr;
	TimerEventRec *te_ptr;
	int i, d;
	XEvent event;
	struct timeval cur_time;
	struct timezone curzone;

	if (mask == 0) return;

	for (;;) {
	    if (mask & XtIMTimer && app->timerQueue != NULL) {
		(void) gettimeofday (&cur_time, &curzone);
		FIXUP_TIMEVAL(cur_time);
		if (IS_AFTER(app->timerQueue->te_timer_value, cur_time)) {
		    te_ptr = app->timerQueue;
		    app->timerQueue = app->timerQueue->te_next;
		    te_ptr->te_next = NULL;
                    if (te_ptr->te_proc != 0)
		        TeCallProc(te_ptr);
		    te_ptr->te_next = freeTimerRecs;
		    freeTimerRecs = te_ptr;
		    return;
		}
	    }
    
	    if (mask & XtIMAlternateInput) {
		if (app->fds.count > 0 && app->outstandingQueue == NULL) {
		    /* Call _XtwaitForSomething to get input queued up */
		    (void) _XtwaitForSomething(TRUE, FALSE, TRUE, FALSE,
			    (unsigned long *)NULL, app);
		}
		if (app->outstandingQueue != NULL) {
		    ie_ptr = app->outstandingQueue;
		    app->outstandingQueue = ie_ptr->ie_oq;
		    ie_ptr->ie_oq = NULL;
		    IeCallProc(ie_ptr);
		    return;
		}
	    }
    
	    if (mask & XtIMXEvent) {
		for (i = 1; i <= app->count; i++) {
		    d = (i + app->last) % app->count;
		    if (XEventsQueued(app->list[d], QueuedAfterReading))
			goto GotEvent;
		}
		for (i = 1; i <= app->count; i++) {
		    d = (i + app->last) % app->count;
		    if (XEventsQueued(app->list[d], QueuedAfterFlush))
			goto GotEvent;
		}
	    }

	    /* Nothing to do...wait for something */

	    if (CallWorkProc(app)) continue;

	    d = _XtwaitForSomething(
				    (mask & XtIMTimer ? FALSE : TRUE),
				    (mask & XtIMAlternateInput ? FALSE : TRUE),
				    (mask & XtIMXEvent ? FALSE : TRUE),
				    TRUE,
				    (unsigned long *) NULL, app);

	    if (mask & XtIMXEvent && d != -1) {
	      GotEvent:
		XNextEvent(app->list[d], &event);
		app->last = d;
		if (event.xany.type == MappingNotify) {
		    _RefreshMapping(&event);
		}
		XtDispatchEvent(&event);
		return;
	    } 
	
	}    
}

XtInputMask XtPending()
{
	return XtAppPending(_XtDefaultAppContext());
}

XtInputMask XtAppPending(app)
	XtAppContext app;
{
	struct timeval cur_time;
	struct timezone curzone;
	int d;
	XtInputMask ret = 0;

/*
 * Check for pending X events
 */
	for (d = 0; d < app->count; d++) {
	    if (XEventsQueued(app->list[d], QueuedAfterReading)) {
		ret = XtIMXEvent;
		break;
	    }
	}
	if (ret == 0) {
	    for (d = 0; d < app->count; d++) {
		if (XEventsQueued(app->list[d], QueuedAfterFlush)) {
		    ret = XtIMXEvent;
		    break;
		}
	    }
	}

/*
 * Check for pending alternate input
 */
	if (app->timerQueue != NULL) {	/* check timeout queue */ 
	    (void) gettimeofday (&cur_time, &curzone);
	    FIXUP_TIMEVAL(cur_time);
	    if ((IS_AFTER(app->timerQueue->te_timer_value, cur_time))  &&
                (app->timerQueue->te_proc != 0)) {
		ret |= XtIMTimer;
	    }
	}

	if (app->outstandingQueue != NULL) ret |= XtIMAlternateInput;
	else {
	    /* This won't cause a wait, but will enqueue any input */

	    if(_XtwaitForSomething(TRUE, FALSE, FALSE, FALSE, (unsigned long *) NULL,
		    app) != -1) ret |= XtIMXEvent;
	    if (app->outstandingQueue != NULL) ret |= XtIMAlternateInput;
	}
	return ret;
}

/* Peek at alternate input and timer callbacks if there are any */

Boolean PeekOtherSources(app)
	XtAppContext app;
{
	struct timeval  cur_time;
	struct timezone cur_timezone;

	if (app->outstandingQueue != NULL) return TRUE;

	if (app->fds.count > 0) {
	    /* Call _XtwaitForSomething to get input queued up */
	    (void) _XtwaitForSomething(TRUE, FALSE, TRUE, FALSE,
		    (unsigned long *)NULL, app);
	    if (app->outstandingQueue != NULL) return TRUE;
	}

	if (app->timerQueue != NULL) {	/* check timeout queue */
	    (void) gettimeofday (&cur_time, &cur_timezone);
	    FIXUP_TIMEVAL(cur_time);
	    if (IS_AFTER (app->timerQueue->te_timer_value, cur_time)) return TRUE;
	}

	return FALSE;
}

Boolean XtPeekEvent(event)
	XEvent *event;
{
	return XtAppPeekEvent(_XtDefaultAppContext(), event);
}

Boolean XtAppPeekEvent(app, event)
	XtAppContext app;
	XEvent *event;
{
	int i, d;
	Boolean foundCall = FALSE;

	for (;;) {
	    for (i = 1; i <= app->count; i++) {
		d = (i + app->last) % app->count;
		if (d == 0) foundCall = PeekOtherSources(app);
		if (XEventsQueued(app->list[d], QueuedAfterReading))
		    goto GotEvent;
	    }
	    for (i = 1; i <= app->count; i++) {
		d = (i + app->last) % app->count;
		if (XEventsQueued(app->list[d], QueuedAfterFlush))
		    goto GotEvent;
	    }

	    if (foundCall) {
		event->xany.type = 0;
		event->xany.display = NULL;
		event->xany.window = NULL;
		return FALSE;
	    }

	    d = _XtwaitForSomething(FALSE, FALSE, FALSE, TRUE,
		    (unsigned long *) NULL, app);

	    if (d != -1) {
	      GotEvent:
		XPeekEvent(app->list[d], event);
		app->last = (d == 0 ? app->count : d) - 1;
		return TRUE;
	    }
	    event->xany.type = 0;	/* Something else must be ready */
	    event->xany.display = NULL;
	    event->xany.window = NULL;
	    return FALSE;

	    /* Greater than 0 is number of dpys with input -- loop around */
	}
}	
