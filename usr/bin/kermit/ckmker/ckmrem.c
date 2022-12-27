/* Version 0.8(35) - Jim Noble at Planning Research Corporation, June 1987. */
/* Ported to Megamax native Macintosh C compiler. */

/*
 * file ckmrem.c
 *
 * Module of MacKermit containing code for remote commands and the display
 * of remote commands.
 *
 */

/*
 Copyright (C) 1985, Trustees of Columbia University in the City of New York.
 Permission is granted to any individual or institution to use, copy, or
 redistribute this software so long as it is not sold for profit, provided this
 copyright notice is retained.
*/

#include "ckcdeb.h"
#include "ckcasc.h"
#include "ckcker.h"

#include "ckmdef.h"		/* General Mac defs */
#include "ckmres.h"		/* Resource file defs */

int isbehind;

typedef struct {
    int resid;
    char scode, *name, *hlp1, *hlp2;
}   RCMDTBL;

RCMDTBL remotecmds[] = {
    {CWD_REMO, 'C', "\pCWD", "\pDirectory", "\pPassword"},
    {DEL_REMO, 'E', "\pDelete", "\pFilespec", ""},
    {DIR_REMO, 'D', "\pDirectory", "\pFilespec", ""},
    {HELP_REMO, 'H', "\pHelp", "\pTopic", ""},
    {HOST_REMO, ' ', "\pHost", "\pCommand", ""},
    {SPAC_REMO, 'U', "\pSpace", "\pArea", ""},
    {TYPE_REMO, 'T', "\pType", "\pFilespec", ""},
    {WHO_REMO, 'W', "\pWho", "\pUser ID", "\pOptions"},
{0, 0, NILPTR, NILPTR, NILPTR}};



/****************************************************************************/
/* Remote command dialog */
/* remotedialog - returns FALSE if cancel hit, TRUE otherwise with */
/*    	      	  generic command setup in gstr. */
/****************************************************************************/
int
remotedialog (rid, gstr)
int rid;
char *gstr;
{
    short itemhit;
    short itemtype;
    int i;
    DialogPtr remoteDialog;
    Handle itemhdl;
    Rect itemrect;
    RCMDTBL *rcmdh = (RCMDTBL *) NULL;
    char arg1[256], arg2[256];

    for (i = 0; remotecmds[i].resid != 0; i++)	/* locate remote command */
	if (remotecmds[i].resid == rid)	/* record for this command */
	    rcmdh = &remotecmds[i];	/* set our handle */

    if (rcmdh == (RCMDTBL *) NULL) {	/* find anything? */
	printerr ("Can't find remote command info for ", rid);	/* ugh... */
	return (FALSE);
    }
    remoteDialog = GetNewDialog (REMOTEBOXID, NILPTR, (WindowPtr) - 1);
    circleOK(remoteDialog);
    
    /* setup variables */
    ParamText (rcmdh->name, rcmdh->hlp1, rcmdh->hlp2, "");

    /* second argument? no, remove from screen */
    if (Length (rcmdh->hlp2) == 0) {
	GetDItem (remoteDialog, RRES_ARG2, &itemtype, &itemhdl, &itemrect);
	if (itemtype != editText)
	    printerr ("RRES_ARG2 is not editText!", 0);	/* ugh now we die! */
	itemtype |= itemDisable;/* disable it */
	SetRect (&itemrect, 1044, 20, 1145, 116); /* off the end of our
						   * world */
	SetDItem (remoteDialog, RRES_ARG2, itemtype, itemhdl, &itemrect);
    }
    for (;;) {
	ModalDialog ((ModalFilterProcPtr) NILPROC, &itemhit);
	switch (itemhit) {
	  case OKBtn:
	    GetDItem (remoteDialog, RRES_ARG1, &itemtype, &itemhdl, &itemrect);
	    GetIText (itemhdl, arg1);
	    p2cstr(arg1);
	    GetDItem (remoteDialog, RRES_ARG2, &itemtype, &itemhdl, &itemrect);
	    GetIText (itemhdl, arg2);
	    p2cstr(arg2);
	    if (rid == HOST_REMO)
		strcpy (gstr, arg1);
	    else
				/* setup the command */
		ssetgen (gstr, rcmdh->scode, arg1, arg2, "");

	  case QuitBtn:	/* fall through */
	    DisposDialog (remoteDialog);	/* all done, answer on cterm
						 * screen */
	    if (itemhit == OKBtn) {	/* want to do the command?/ */
		conoc (CR);	/* output CR to delimit */
		conoll ("-----");	/* and dashes followed by CR */
		if (isbehind)	/* response window behind?/ */
		    rcmdwshow ();	/* yes, so show it */
	    }
	    return (itemhit == OKBtn);	/* and return ok or bad... */
	}
    }
}				/* remotedialog */



/* Remote command response window */

extern MenuHandle menus[];
WindowRecord remoteWRec;	/* store window stuff here */
WindowPtr remoteWindow;		/* the remote command window */
Rect teviewr;
TEHandle rem_teh;
ControlHandle vscroll;
ControlHandle hscroll;
Point theorigin;

/****************************************************************************/
/* initrcmdw - initialize the remote command window */
/****************************************************************************/
initrcmdw ()
{
    GrafPtr savePort;

    GetPort (&savePort);	/* save current port */

    isbehind = TRUE;
    remoteWindow = GetNewWindow (RCMDBOXID, (Ptr) &remoteWRec, (WindowPtr) 0);
    SetPort (remoteWindow);	/* set new stuff */
    TextFont (monaco);
    TextSize (9);

    /* min and max are defined in the resource declaration */
    vscroll = GetNewControl (RCMDVSCROLL, remoteWindow);
    hscroll = GetNewControl (RCMDHSCROLL, remoteWindow); 

    sizescrollbars ();		/* make controls adjust to wind size */
    sizeteviewr ();		/* resize text edit rect */

    rem_teh = TENew (&teviewr, &teviewr);	/* create text edit portion */
    HLock((Handle)rem_teh);
    (*rem_teh)->crOnly = -1;	/* only break lines at CR */

    consette (rem_teh);		/* setup for low lvl console rtns */

    theorigin.h = 0;
    theorigin.v = 0;


    SetPort (savePort);		/* restore previous port */
}				/* initrcmdw */



/****************************************************************************/
/****************************************************************************/
togglercmdw ()
{
    if (isbehind)
	rcmdwshow ();
    else
	rcmdwhide ();
}				/* togglercmdw */



/****************************************************************************/
/****************************************************************************/
rcmdwhide ()
{
    HideWindow (remoteWindow);	/* hide it */
    HideControl (vscroll);	/* these will be shown on select */
    HideControl (hscroll);
    SetItem (menus[REMO_MENU], RESP_REMO, "\pShow Response");
    isbehind = TRUE;
}				/* rcmdwhide */



/****************************************************************************/
/****************************************************************************/
rcmdwshow ()
{
    ShowWindow (remoteWindow);	/* show it */
    SelectWindow (remoteWindow);
    SetItem (menus[REMO_MENU], RESP_REMO, "\pHide Response");
    isbehind = FALSE;
}				/* rcmdwshow */



/****************************************************************************/
/* rcdactivate - activate event on rcd window */
/****************************************************************************/
rcdactivate (mod)
int mod;
{
    DrawGrowIcon (remoteWindow);
    if (mod & activeFlag) {
	ShowControl (vscroll);
	ShowControl (hscroll);
	DisableItem(menus[EDIT_MENU], UNDO_EDIT);
    } else {
	HideControl (vscroll);
	HideControl (hscroll);
	EnableItem(menus[EDIT_MENU], UNDO_EDIT);
    }
}				/* rcdactivate */



/****************************************************************************/
/****************************************************************************/
pascal void
rdoscroll (WHICHCONTROL, THECODE)
ControlHandle WHICHCONTROL;
short THECODE;
{
    register int amount = 0, val, max;

    if (THECODE == inUpButton)
	amount = -1;
    if (THECODE == inDownButton)
	amount = 1;
    if (amount == 0)
	return;
    val = GetCtlValue (WHICHCONTROL) + amount;
    max = GetCtlMax (WHICHCONTROL);
    if ((val < 0) || (val > max))
	return;
    SetCtlValue (WHICHCONTROL, GetCtlValue (WHICHCONTROL) + amount);
    scrollbits ();
}				/* rdoscroll */


/****************************************************************************/
/* rcdkey - key event on rcd window */
/****************************************************************************/
rcdkey (evt)
EventRecord *evt;
{
    unsigned char the_char;
    
    the_char = evt->message & charCodeMask;
    TEKey(the_char, rem_teh);
}

/****************************************************************************/
/* rcdmouse - mouse event on rcd window */
/****************************************************************************/
rcdmouse (evt)
EventRecord *evt;
{
    int actrlcode;
    int t;
    ControlHandle acontrol;
    GrafPtr savePort;
    
    GetPort (&savePort);	/* save the current port */
    SetPort (remoteWindow);

    GlobalToLocal (&evt->where);/* convert to local */
    if (PtInRect (evt->where, &teviewr)) {	/* in text edit? */
    	GlobalToLocal (&evt->where);	/* convert to local */
	TEClick(evt->where, (evt->modifiers) & shiftKey, rem_teh);
	SetPort (savePort);	/* restore previous port */
	return;			/* yes, do nothing */
    }
    actrlcode = FindControl (evt->where, remoteWindow, &acontrol);
    switch (actrlcode) {
      case inUpButton:
      case inDownButton:
	t = TrackControl (acontrol, evt->where, (ProcPtr) rdoscroll);
	break;

      case inPageUp:
	pagescroll (actrlcode, -10, acontrol);
	break;

      case inPageDown:
	pagescroll (actrlcode, 10, acontrol);
	break;

      case inThumb:
	t = TrackControl (acontrol, evt->where, (ProcPtr) NIL);
	scrollbits ();
	break;
    }
    SetPort (savePort);		/* restore previous port */
}				/* rcdmouse */

rcd_cut()
{
    TECut(rem_teh);
}

rcd_copy()
{
    TECopy(rem_teh);
}

rcd_paste()
{
    TEPaste(rem_teh);
}

rcd_clear()
{
    TEDelete(rem_teh);
}


/****************************************************************************/
/****************************************************************************/
void
growremwindow (window, p)
WindowPtr window;
Point p;
{
    long gr;
    int height;
    int width;
    Rect growRect;
    GrafPtr savePort;

    growRect = qd.screenBits.bounds;
    growRect.top = 50;		/* minimal horizontal size */
    growRect.left = 50;		/* minimal vertical size */

    gr = GrowWindow (window, p, &growRect);

    if (gr == 0)
	return;
    height = HiWord (gr);
    width = LoWord (gr);

    SizeWindow (window, width, height, FALSE);	/* resize the window */
    sizescrollbars ();		/* size the scroll bars */
    sizeteviewr ();		/* size for text edit */
    (*rem_teh)->viewRect = teviewr;	/* set it */

    GetPort (&savePort);
    SetPort (window);
    InvalRect (&window->portRect);	/* invalidate whole window rectangle */
    SetPort (savePort);
}				/* growremwindow */



/****************************************************************************/
/****************************************************************************/
rcdupdate (window)
WindowPtr window;
{
    EraseRect (&teviewr);
    TEUpdate (&teviewr, rem_teh);
    DrawGrowIcon (window);
    DrawControls (window);
}				/* rcdupdate */



/****************************************************************************/
/****************************************************************************/
scrollbits ()
{
    Point oldorigin;
    int dh, dv;

    oldorigin = theorigin;
    theorigin.h = GetCtlValue (hscroll);
    theorigin.v = GetCtlValue (vscroll);
    dh = (oldorigin.h - theorigin.h);
    dv = (oldorigin.v - theorigin.v) * (*rem_teh)->lineHeight;
    TEScroll (dh, dv, rem_teh);
}				/* scrollbits */



/****************************************************************************/
/****************************************************************************/
rcdwscroll ()
{
    GrafPtr savePort;

    GetPort (&savePort);
    SetPort (remoteWindow);	/* our window is the port */
    setscrollmax ();		/* set the max */
    SetCtlValue (vscroll, GetCtlMax (vscroll));	/* and set control */
    scrollbits ();		/* do scroll */
    SetPort (savePort);		/* back to old port */
}				/* rcdwscroll */



/****************************************************************************/
/****************************************************************************/
pagescroll (code, amount, ctrlh)
ControlHandle ctrlh;
{
    Point myPt;

    do {
	GetMouse (&myPt);
	if (TestControl (ctrlh, myPt) != code)
	    continue;
	SetCtlValue (ctrlh, GetCtlValue (ctrlh) + amount);
	scrollbits ();
    } while (StillDown ());
}				/* pagescroll */



/****************************************************************************/
/* setscrollmax - sets the vertical scroll control's maximum value.
 *    	      	  The max is the total number of lines in the te record
 *    	      	  minus the number of lines that can be displayed in the
 *    	      	  viewing rectangle (plus 1).  This makes the max setting
 *    	      	  of the control result in the display of the last chunk
 *    	      	  of text.
 */
/****************************************************************************/
setscrollmax ()
{
    int maxv;

    maxv = (*rem_teh)->nLines + 1 -
	(((*rem_teh)->viewRect.bottom - (*rem_teh)->viewRect.top) / (*rem_teh)->lineHeight);
    if (maxv < 0)		/* for less than one page */
	maxv = 0;		/* use this value as max */
    SetCtlMax (vscroll, maxv);	/* set it... */
}				/* setscrollmax */



/****************************************************************************/
/* sizescrollbars - called when window is created and after a window grow */
/*    	      	    sequence to resize the scroll window's bars. */
/****************************************************************************/
sizescrollbars ()
{
    register Rect *r;

    r = &remoteWindow->portRect;/* window size */
    HideControl (vscroll);
    HideControl (hscroll);

    MoveControl (vscroll, r->right - 15, r->top - 1);
    SizeControl (vscroll, 16, r->bottom - r->top - 13);

    MoveControl (hscroll, r->left - 1, r->bottom - 15);
    SizeControl (hscroll, r->right - r->left - 13, 16);

    ShowControl (vscroll);
    ShowControl (hscroll);
}				/* sizescrollbars */



/****************************************************************************/
/****************************************************************************/
sizeteviewr ()
{
    teviewr = remoteWindow->portRect;
    teviewr.left = teviewr.left + 4;
    teviewr.right = teviewr.right - 15;
    teviewr.bottom = teviewr.bottom - 15;
}				/* sizeteviewr */
