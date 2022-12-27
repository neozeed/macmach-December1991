/* -*-C-*-
********************************************************************************
*
* File:         xldebug.c
* RCS:          $Header: xldbug.c,v 1.4 90/08/07 16:32:28 mayer Exp $
* Description:  xlisp debugging support
* Author:       David Michael Betz; Niels Mayer
* Created:      
* Modified:     Tue Aug  7 16:32:16 1990 (Niels Mayer) mayer@hplnpm
* Language:     C
* Package:      N/A
* Status:       X11r4 contrib tape release
*
* WINTERP 1.0 Copyright 1989 Hewlett-Packard Company (by Niels Mayer).
* XLISP version 2.1, Copyright (c) 1989, by David Betz.
*
* Permission to use, copy, modify, distribute, and sell this software and its
* documentation for any purpose is hereby granted without fee, provided that
* the above copyright notice appear in all copies and that both that
* copyright notice and this permission notice appear in supporting
* documentation, and that the name of Hewlett-Packard and David Betz not be
* used in advertising or publicity pertaining to distribution of the software
* without specific, written prior permission.  Hewlett-Packard and David Betz
* make no representations about the suitability of this software for any
* purpose. It is provided "as is" without express or implied warranty.
*
* HEWLETT-PACKARD AND DAVID BETZ DISCLAIM ALL WARRANTIES WITH REGARD TO THIS
* SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS,
* IN NO EVENT SHALL HEWLETT-PACKARD NOR DAVID BETZ BE LIABLE FOR ANY SPECIAL,
* INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
* LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
* OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
* PERFORMANCE OF THIS SOFTWARE.
*
* See ./winterp/COPYRIGHT for information on contacting the authors.
* 
* Please send modifications, improvements and bugfixes to mayer@hplabs.hp.com
* Post XLISP-specific questions/information to the newsgroup comp.lang.lisp.x
*
********************************************************************************
*/
static char rcs_identity[] = "@(#)$Header: xldbug.c,v 1.4 90/08/07 16:32:28 mayer Exp $";

#ifdef WINTERP
#include <X11/Intrinsic.h>
#endif
#include "xlisp.h"

/* external variables */
extern int xldebug;
extern int xlsample;
extern LVAL s_debugio,s_unbound;
extern LVAL s_tracenable,s_tlimit,s_breakenable;
extern LVAL true;
extern char buf[];

/* external routines */
extern char *malloc();

/* forward declarations */
FORWARD LVAL stacktop();
LOCAL int breakloop();

/* xlabort - xlisp serious error handler */
xlabort(emsg)
  char *emsg;
{
    xlsignal(emsg,s_unbound);
    xlerrprint("error",NULL,emsg,s_unbound);
    xlbrklevel();
}

/* xlbreak - enter a break loop */
xlbreak(emsg,arg)
  char *emsg; LVAL arg;
{
    breakloop("break","return from BREAK",emsg,arg,TRUE);
}

/* xlfail - xlisp error handler */
xlfail(emsg)
  char *emsg;
{
    xlerror(emsg,s_unbound);
}

/* xlerror - handle a fatal error */
xlerror(emsg,arg)
  char *emsg; LVAL arg;
{
    if (getvalue(s_breakenable) != NIL)
	breakloop("error",NULL,emsg,arg,FALSE);
    else {
	xlsignal(emsg,arg);
	xlerrprint("error",NULL,emsg,arg);
	xlbrklevel();
    }
}

/* xlcerror - handle a recoverable error */
xlcerror(cmsg,emsg,arg)
  char *cmsg,*emsg; LVAL arg;
{
    if (getvalue(s_breakenable) != NIL)
	breakloop("error",cmsg,emsg,arg,TRUE);
    else {
	xlsignal(emsg,arg);
	xlerrprint("error",NULL,emsg,arg);
	xlbrklevel();
    }
}

/* xlerrprint - print an error message */
xlerrprint(hdr,cmsg,emsg,arg)
  char *hdr,*cmsg,*emsg; LVAL arg;
{
    /* print the error message */
    sprintf(buf,"%s: %s",hdr,emsg);
    errputstr(buf);

    /* print the argument */
    if (arg != s_unbound) {
	errputstr(" - ");
	errprint(arg);
    }

    /* no argument, just end the line */
    else
	errputstr("\n");

    /* print the continuation message */
    if (cmsg) {
	sprintf(buf,"if continued: %s\n",cmsg);
	errputstr(buf);
    }
}

#ifdef WINTERP
/*
 * This version of breakloop() works with server in winterp.c. It allows you
 * to be in the breakloop while X Events and Xlisp server requests to be
 * processed.
 */
LOCAL int breakloop(hdr,cmsg,emsg,arg,cflag)
     char *hdr,*cmsg,*emsg; LVAL arg; int cflag;
{
  extern int read_eval_print_just_called; /* from winterp.c */
  extern int lisp_reader_hit_eof; /* from winterp.c */
  extern XtAppContext app_context; /* from winterp.c */
  LVAL expr,val;
  CONTEXT cntxt;
  int type;
  XEvent event;

  xlerrprint(hdr,cmsg,emsg,arg); /* print the error message */

  /* flush the input buffer --  needed if using (read)/(read-line) from stdin */
  xlflush();

  if (getvalue(s_tracenable)) {	/* do the back trace */
    val = getvalue(s_tlimit);
    xlbaktrace(fixp(val) ? (int)getfixnum(val) : -1);
  }
  xlsave1(expr);		/* protect some pointers */
  ++xldebug;			/* increment the debug level */

  read_eval_print_just_called = TRUE; /* special initial cond */
  lisp_reader_hit_eof = FALSE;

  /* debug command processing loop -- note similarity to loop winterp.c:main() */
  xlbegin(&cntxt,CF_BRKLEVEL|CF_CLEANUP|CF_CONTINUE,true);
  for (type = 0; type == 0; ) {
    /* 
     * We need to setup a new error return only after each time that an XLISP 
     * evaluation occurs. Therefore, we check for read_eval_print_just_called 
     * (which is set by Read_Eval_Print()) and then clear it once the setjmp() 
     * has been done. This avoids setting up an error return for each X event
     * being processed in this loop. 
     */
    if (read_eval_print_just_called) { /* set in winterp.c:Read_Eval_Print callback */
      read_eval_print_just_called = FALSE;
      if (lisp_reader_hit_eof) {	/* set in winterp.c:Read_Eval_Print callback */
	type = CF_CLEANUP;
	break;
      }
      if (type = setjmp(cntxt.c_jmpbuf)) /* setup the continue trap */
	switch (type) {
	case CF_CLEANUP:
	  continue;
	case CF_BRKLEVEL:
	  type = 0;
	  break;
	case CF_CONTINUE:
	  if (cflag) {
	    dbgputstr("[ continue from break loop ]\n");
	    continue;
	  }
	  else 
	    xlabort("this error can't be continued");
	}
      sprintf(buf, "XLisp-Breakloop-Level-%d> ", xldebug); /* print a "prompt" */
      dbgputstr(buf);
      fflush(stdout); fflush(stderr); /* otherwise output won't happen while blocked in XtAppNextEvent() */
    }

    /*
     * XtAppNextEvent() waits for Xevents, and while it is waiting, it will
     * process inputs added via AtAppAddInput() or XtAppAddWorkProc(). Lisp 
     * server input will cause Read_Eval_Print() to get called, and that
     * procedure sets the globals lisp_reader_hit_eof and 
     * read_eval_print_just_called. Read_Eval_Print() sends a bogus 
     * XAnyEvent (event.type == 0) so as to force XtAppNextEvent() to return; 
     * otherwise it would only return if a lisp evaluation caused X events 
     * to be generated, which means that XLISP error returns for non-X 
     * evaluations wouldn't get set up properly.
     *
     * XtDispatchEvent() will dispatch the actions from the events gathered
     * by XtAppNextEvent(). Note that XtAppNextEvent() ignores the aforementioned
     * bogus events: "if (event->type == 0) return;"
     */
    XtAppNextEvent(app_context, &event);
    XtDispatchEvent(&event);
  }
  xlend(&cntxt);

  --xldebug;			/* decrement the debug level */
  xlpop();			/* restore the stack */
  if (type == CF_CLEANUP)	/* check for aborting to the previous level */
    xlbrklevel();
}

#else

/* breakloop - the debug read-eval-print loop */
LOCAL int breakloop(hdr,cmsg,emsg,arg,cflag)
  char *hdr,*cmsg,*emsg; LVAL arg; int cflag;
{
    LVAL expr,val;
    CONTEXT cntxt;
    int type;

    /* print the error message */
    xlerrprint(hdr,cmsg,emsg,arg);

    /* flush the input buffer */
    xlflush();

    /* do the back trace */
    if (getvalue(s_tracenable)) {
	val = getvalue(s_tlimit);
	xlbaktrace(fixp(val) ? (int)getfixnum(val) : -1);
    }

    /* protect some pointers */
    xlsave1(expr);

    /* increment the debug level */
    ++xldebug;

    /* debug command processing loop */
    xlbegin(&cntxt,CF_BRKLEVEL|CF_CLEANUP|CF_CONTINUE,true);
    for (type = 0; type == 0; ) {

	/* setup the continue trap */
	if (type = setjmp(cntxt.c_jmpbuf))
	    switch (type) {
	    case CF_CLEANUP:
		continue;
	    case CF_BRKLEVEL:
		type = 0;
		break;
	    case CF_CONTINUE:
		if (cflag) {
		    dbgputstr("[ continue from break loop ]\n");
		    continue;
		}
		else xlabort("this error can't be continued");
	    }

	/* print a prompt */
	sprintf(buf,"%d> ",xldebug);
	dbgputstr(buf);

	/* read an expression and check for eof */
	if (!xlread(getvalue(s_debugio),&expr,FALSE)) {
	    type = CF_CLEANUP;
	    break;
	}

	/* save the input expression */
	xlrdsave(expr);

	/* evaluate the expression */
	expr = xleval(expr);

	/* save the result */
	xlevsave(expr);

	/* print it */
	dbgprint(expr);
    }
    xlend(&cntxt);

    /* decrement the debug level */
    --xldebug;

    /* restore the stack */
    xlpop();

    /* check for aborting to the previous level */
    if (type == CF_CLEANUP)
	xlbrklevel();
}

#endif

/* baktrace - do a back trace */
xlbaktrace(n)
  int n;
{
    LVAL *fp,*p;
    int argc;
    for (fp = xlfp; (n < 0 || n--) && *fp; fp = fp - (int)getfixnum(*fp)) {
	p = fp + 1;
	errputstr("Function: ");
	errprint(*p++);
	if (argc = (int)getfixnum(*p++))
	    errputstr("Arguments:\n");
	while (--argc >= 0) {
	    errputstr("  ");
	    errprint(*p++);
	}
    }
}

/* xldinit - debug initialization routine */
xldinit()
{
    xlsample = 0;
    xldebug = 0;
}

