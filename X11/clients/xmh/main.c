/*
 * $XConsortium: main.c,v 2.16 89/10/06 15:02:44 converse Exp $
 *
 *
 *		       COPYRIGHT 1987, 1989
 *		   DIGITAL EQUIPMENT CORPORATION
 *		       MAYNARD, MASSACHUSETTS
 *			ALL RIGHTS RESERVED.
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE WITHOUT NOTICE AND
 * SHOULD NOT BE CONSTRUED AS A COMMITMENT BY DIGITAL EQUIPMENT CORPORATION.
 * DIGITAL MAKES NO REPRESENTATIONS ABOUT THE SUITABILITY OF THIS SOFTWARE FOR
 * ANY PURPOSE.  IT IS SUPPLIED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY.
 *
 * IF THE SOFTWARE IS MODIFIED IN A MANNER CREATING DERIVATIVE COPYRIGHT
 * RIGHTS, APPROPRIATE LEGENDS MAY BE PLACED ON THE DERIVATIVE WORK IN
 * ADDITION TO THAT SET FORTH ABOVE.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Digital Equipment Corporation not be
 * used in advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission.
 */

#define MAIN 1			/* Makes global.h actually declare vars */
#include "xmh.h"

static XtIntervalId timerid;
static unsigned long interval;

/* NeedToCheckScans() gets called every five minutes, by default.
 * The frequency of its invocations is can be changed by the resource
 * Xmh*CheckFrequency, whose default is one minute.  NeedToCheckScans()
 * will be called once for every five check frequency intervals.
 */

static void NeedToCheckScans()
{
    int i;
    DEBUG("[magic toc check ...")
    for (i = 0; i < numScrns; i++) {
	if (scrnList[i]->toc)
	    TocRecheckValidity(scrnList[i]->toc);
	if (scrnList[i]->msg)
	    TocRecheckValidity(MsgGetToc(scrnList[i]->msg));
    }
    DEBUG(" done]\n")
}


/*ARGSUSED*/
static void CheckMail(client_data, id)
    XtPointer client_data;	/* unused */
    XtIntervalId *id;		/* unused */
{
    static int count = 0;
    register int i;
    timerid = XtAppAddTimeOut(XtWidgetToApplicationContext( toplevel ),
			      interval, CheckMail, (XtPointer) NULL);
    if (app_resources.new_mail_check) {
	DEBUG("(Checking for new mail...")
	TocCheckForNewMail();
	DEBUG(" done)\n")
    }
    if (!subProcessRunning && (count++ % 5 == 0)) {
	NeedToCheckScans();
	if (app_resources.make_checkpoints) {
	    DEBUG("(Checkpointing...")
	    for (i=0 ; i<numScrns ; i++)
		if (scrnList[i]->msg) 
		    MsgCheckPoint(scrnList[i]->msg);
	    DEBUG(" done)\n")
	}
    }
}

/* Main loop. */

main(argc, argv)
unsigned int argc;
char **argv;
{
    InitializeWorld(argc, argv);
    if (app_resources.new_mail_check)
	TocCheckForNewMail();
    subProcessRunning = False;

    if (app_resources.check_frequency > 0) {
	interval = app_resources.check_frequency * 60000;
	timerid = XtAppAddTimeOut( XtWidgetToApplicationContext( toplevel ),
				  interval, CheckMail, (XtPointer) NULL);
    }

    lastInput.win = -1;		/* nothing mapped yet */
    for (;;) {
	XEvent ev;
	XtNextEvent( &ev );
	if (ev.type == KeyPress) {
	    lastInput.win = ev.xany.window;
	    lastInput.x = ev.xkey.x_root;
	    lastInput.y = ev.xkey.y_root;
	} else if (ev.type == ButtonPress) {
	    lastInput.win = ev.xany.window;
	    lastInput.x = ev.xbutton.x_root;
	    lastInput.y = ev.xbutton.y_root;
	}
	XtDispatchEvent( &ev );
    }
}
