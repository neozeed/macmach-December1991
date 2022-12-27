/* Version 0.8(35) - Jim Noble at Planning Research Corporation, June 1987. */
/* Ported to Megamax native Macintosh C compiler. */
/* Edit by Bill on Wed May 15, 15:53 */
/* A K is 1024 not 512 */

/*
 * file ckmscr.c
 *
 * Module of mackermit containing code for display of status dialog.
 *
 * Bill Schilit, May 1984
 *
 * Copyright (C) 1985, Trustees of Columbia University in the City of
 * New York.  Permission is granted to any individual or institution to
 * use, copy, or redistribute this software so long as it is not sold
 * for profit, provided this copyright notice is retained.
 *
 */

#include "ckcdeb.h"		/* C kermit debuggig */
#include "ckcker.h"		/* general kermit defs */

#include "ckmdef.h"		/* General Mac defs */
#include "ckmres.h"		/* Mac resource equates */

int scrpkt, scrnak;		/* NAK, PKT counts */
long scrck;			/* char (K) count */
int scrpacln, scrcksum, scrwslots, scrbufnum;	/* pkt len, checksum, win size */
long scrfsize, scrffc;
DialogPtr scrdlg = (DialogPtr) NULL;	/* screen's dialog */

extern CHAR filnam[];

void update_scr_therm();
static pascal void draw_screen_xfr_therm (WindowPtr win, short itemno);

/****************************************************************************/
/* scrcreate - create the status display.  Called when a protocol
 *    	       menu item is selected and a display is desired (I don't
 *    	       think you'd want to see this for REMOTE command).
 *
 */
/****************************************************************************/
scrcreate ()
{
    Rect r, itemr;
    short itype;
    Handle itemhdl;
    PenState oldpenstate;

    if (scrdlg != NULL)
	printerr ("scrcreate with active screen!", 0);

    scrdlg = GetNewDialog (SCRBOXID, NILPTR, (WindowPtr) -1);
    scrck = -1;
    scrnak = scrpkt = 0;
    scrpacln = scrcksum = 0;
    scrwslots = scrbufnum = -1;
    scrfsize = scrffc = 0;
    
    SetStrText (SRES_PTTXT, "\pEmergency exit: hold down \021 and type a period.",
	    	scrdlg);	/* yes, so set the text */

    SetStrText (SRES_DIR, (protocmd == SEND_FIL) ? "\p   Sending" : "\pReceiving",
		scrdlg);

    GetDItem (scrdlg, SRES_THERM, &itype, &itemhdl, &itemr);
    SetDItem (scrdlg, SRES_THERM, itype, (Handle) draw_screen_xfr_therm, &itemr);
    update_scr_therm();

    miniparser (TRUE);		/* keep things moving */
}				/* scrcreate */



/****************************************************************************/
/* scrdispose - called to finish up the status display, on a
 *    	      	transaction complete screen() call.
 */
/****************************************************************************/
scrdispose (wait)
Boolean wait;
{
    EventRecord dummyEvt;

    if (scrdlg == NULL)
	printerr ("scrdispose called with no screen active!", 0);

    SysBeep (3);

    if (wait) {			/* deactivate buttons */
	HiliteControl (getctlhdl (SRES_CANF, scrdlg), 255);
	HiliteControl (getctlhdl (SRES_CANG, scrdlg), 255);

	if (tlevel < 0)	{	/* if no takefile running */
	    SetStrText (SRES_PTTXT, "\pType a key or click the mouse to continue.",
	    	scrdlg);	/* yes, so set the text */

	    /*
	     * wait for mouse or key down and discard the event when it
	     * happens
	     */
	    while (!GetNextEvent (keyDownMask + mDownMask, &dummyEvt))
		 /* do nothing */ ;
	}
    } else {
	SetStrText (SRES_PTTXT, "\pNormal cleanupŠ",  scrdlg);
    }

    DisposDialog (scrdlg);
    scrdlg = NULL;
}				/* scrdispose */

void
update_scr_therm()
{
    GrafPtr savePort;
    
    GetPort (&savePort);	/* there just has to be a better way */
    SetPort (scrdlg);

    draw_screen_xfr_therm (scrdlg, SRES_THERM);

    SetPort (savePort);		/* there just has to be a better way */    
}

static pascal void
draw_screen_xfr_therm (WindowPtr win, short itemno)
{
    Rect r, itemr;
    short itype;
    Handle itemhdl;
    PenState oldpenstate;

    if (itemno != SRES_THERM)		/* Shouldn't ever happen */
    	return;

    GetPenState(&oldpenstate);		/* save so we can fuss with */
    PenNormal();
    
    GetDItem (scrdlg, itemno, &itype, &itemhdl, &itemr);
    
    /* EraseRect(&itemr); */

    if (fsize <= 0L) {

    	/* Frame the thermometer in gray */
	PenPat(qd.gray);
	PenSize (2, 2);
	FrameRect(&itemr);
	InsetRect(&itemr, 2, 2);
	
	EraseRect(&itemr);
	
    } else {

    	/* Frame the thermometer */
	PenSize (2, 2);
	FrameRect(&itemr);

	InsetRect(&itemr, 2, 2);
	PenPat (qd.white);
	FrameRect(&itemr);
	
	InsetRect(&itemr, 2, 2);
	    
	/*
	 * We want to fill first (ffc / fsize) of (itemr.right - itemr.left) with black,
	 * and the rest with gray.
	 */
	
	r.top = itemr.top;
	r.bottom = itemr.bottom;
	r.left = itemr.left;
	r.right = itemr.left
		  + (((long) (itemr.right - itemr.left) * (long) ffc) / (long) fsize);
	if (r.right > itemr.right)		/* reality check */
	    r.right = itemr.right;
	
	if (r.left < r.right)			/* has any been transfered? */
	    FillRect (&r, qd.dkGray);
	
	if (r.right < itemr.right) {	/* any left to transfer? */
	    r.left = r.right;
	    r.right = itemr.right;
	    FillRect (&r, qd.ltGray);
	}
    }
    
    scrfsize = fsize;
    scrffc = ffc;
    
    SetPenState(&oldpenstate);		/* restore old pen state */
}


/* ststrings - translation of SCR_ST subfunctions to descriptive text */

char *ststrings[] = {
    ": Transferred OK",		/* ST_OK */
    ": Discarded",		/* ST_DISC */
    ": Interrupted",		/* ST_INT */
    ": Skipped ",		/* ST_SKIP */
    ": Fatal error"		/* ST_ERR */
};

/* scrtosresnum - table to translate from SCR_XXX values into resource
 *    	       	  item numbers.  Entries we aren't interested in are
 *    	          set to SRES_UNDEF.
 */

int scrtoresnum[] = {
    SRES_UNDEF,			/* 0 - nothing */
    SRES_FILN,			/* SCR_FN - filename */
    SRES_AFILN,			/* SCR_AN - as filename */
    SRES_UNDEF,			/* SCR_FS - file size */
    SRES_UNDEF,			/* SCR_XD - x-packet data */
    SRES_PTEXT,			/* SCR_ST - status (goes in prev. text area) */
    SRES_UNDEF,			/* SCR_PN - packet number */
    SRES_UNDEF,			/* SCR_PT - packet type (special) */
    SRES_BTEXT,			/* SCR_TC - transaction complete */
    SRES_ITEXT,			/* SCR_EM - error msg (does alert) */
    SRES_ITEXT,			/* SCR_WM - warning message */
    SRES_BTEXT,			/* SCR_TU - arb text */
    SRES_BTEXT,			/* SCR_TN - arb text */
    SRES_BTEXT,			/* SCR_TZ - arb text */
    SRES_BTEXT,			/* SCR_QE - arb text */
    SRES_ITEXT			/* SCR_DT - date text */
};

/****************************************************************************/
/* screen - display status information on the screen during protocol.
 *    	    Here we just set the items in their StatText dialog boxes,
 *    	    updates occur through the miniparser, which we are nice
 *    	    enough to call here to handle things.
 *
 */
/****************************************************************************/
screen (f, c, n, s)
int f;
char c;
char *s;
long n;
{
    int rnum;
    long i;
    char buf[256];
    extern int spktl, rln, bctu, wslots;
    static char last_st = ST_OK;/* PWP: saves the most recent value of the
				 * status indication */
    static char got_err = 0; /* becomes true if we get an error message */

    miniparser (TRUE);		/* keep the mac going */

#ifdef COMMENT
    if (f == SCR_EM) {		/* error message? (warnings go into dialog) */
	printerr (s, 0);	/* display it */
	return;			/* and return */
    }
#endif
    
    if ((scrdlg == NULL) || quiet)	/* not using it or silent? */
	return;			/* but nothing for us to do */

    if (f == SCR_FN) {		/* seeing a file name? */
	SetStrText (SRES_AFILN, "\p", scrdlg);	/* and the name also */
	if (protocmd == RECV_FIL ||	/* seeing a file name from */
	    protocmd == GETS_FIL)	/* a receive command? */
	    dorecvdialog (s, &cmarg2);	/* yes, allow user to do dialog */
    }
    rnum = scrtoresnum[f];	/* load default DITL number */
    /* where result will be posted */

    switch (f) {		/* according to function... */
      case SCR_EM:		/* error message */
      case SCR_WM:		/* warning message */
	got_err = 1;
	SysBeep(3);		/* get the user's attention */
	Delay ((long) 10, &i);
	SysBeep(3);
	Delay ((long) 10, &i);
	SysBeep(3);
	if (n != 0L) {
	    strcpy (buf, s);
	    strcat (buf, " ");
	    s = &buf[strlen(buf)];
	    NumToString(n, s);
	    p2cstr(s);
	    s = buf;
	}
	break;
      
      case SCR_FN:
	got_err = 0;	/* reset for this file */
	SetStrText (SRES_BTEXT, "\p", scrdlg);	/* clear eg. remote size */
	SetStrText (SRES_ITEXT, "\p", scrdlg);	/* clear eg. date */
	break;

      case SCR_AN:		/* "AS" name is comming */
	if ((filargs.filflg & (FIL_RSRC | FIL_DATA)) == (FIL_RSRC | FIL_DATA)) {
	    /* in MacBinary mode */
	    SetStrText (SRES_FFORK, "\p", scrdlg);
	    SetStrText (SRES_FMODE, "\pMacBinary Mode", scrdlg);
	} else {
	    SetStrText (SRES_FFORK, (filargs.filflg & FIL_RSRC) ?
		    "\pRSRC Fork" : "\pData Fork", scrdlg);
	    SetStrText (SRES_FMODE, (filargs.filflg & FIL_BINA) ?
		    "\pBinary Mode" : "\pText Mode", scrdlg);
	}
	break;

      case SCR_PT:		/* packet type? */
	/* packet length */
	if (protocmd == SEND_FIL) {	/* sent a packet, see ckcfn2.c, spack() */
	    i = spktl-bctu;
	    if (i+2 <= MAXPACK) i += 2;	/* short packet */
	} else {		/* recieved a packet -- see ckcfn2.c, rpack() */
	    i = rln + bctu;
	    if (rln <= MAXPACK)	/* if it was a short packet */
		i += 2;		/* then add space for SEQ and TYPE */
	}
	if (i != scrpacln) {
	    scrpacln = i;
	    SetNumText (SRES_PACSZ, scrpacln, scrdlg);	/* PWP: do xmit length */
	}
	/* checksum type */
	if (bctu != scrcksum) {
	    scrcksum = bctu;
	    SetNumText (SRES_CKSUM, scrcksum, scrdlg);	/* PWP: and rec length */
	}
	
	/* window size */
	if ((wslots != scrwslots)
	    || ((protocmd == SEND_FIL) && (sbufnum != scrbufnum))
	    || ((protocmd != SEND_FIL) && (rbufnum != scrbufnum)))
	{
	    char *cp;

	    scrwslots = wslots;
	    if (protocmd == SEND_FIL)
		scrbufnum = sbufnum;
	    else
		scrbufnum = rbufnum;

	    NumToString ((wslots - scrbufnum), buf);	/* convert to number */
	    p2cstr(buf);
	    strcat (buf, "/");	/* make it be a fraction */
	    cp = (char *) buf + strlen (buf);
	    NumToString (scrwslots, cp);
	    p2cstr(cp);
	    c2pstr(buf);	/* convert whole buf back into string */
	    SetStrText (SRES_WINSZ, buf, scrdlg);	/* set new values */
	}
	
	if (c == 'Y')
	    return;		/* don't do anything for yaks */

	if (c == 'N' || c == 'Q' ||	/* check for all types of */
	    c == 'T' || c == '%')	/* NAK */
	    i = ++scrnak, rnum = SRES_NRTY; /* increment nak counter */
	else
	    i = ++scrpkt, rnum = SRES_NPKT; /* else increment pkt counter */

	NumToString (i, buf);	/* translate to number */
	p2cstr(buf);
	s = buf;		/* new buffer */
	break;			/* all done */

      case SCR_ST:		/* status */
	last_st = c;		/* PWP: save for later */
	if (f == ST_SKIP)
	    strcpy(buf, s);
	else
	    strcpy (buf, filnam);	/* file name; should be same as filargs.fillcl */
	strcat (buf, ststrings[c]);	/* add status */
	s = buf;
#ifdef COMMENT
	SetStrText (SRES_BTEXT, "\p", scrdlg);	/* clear eg. remote size */
	SetStrText (SRES_ITEXT, "\p", scrdlg);	/* clear eg. date */
#endif
	break;

      case SCR_TC:		/* transaction completed */
	if (!server) {		/* are we a server? */
	    scrdispose ((last_st != ST_OK) || got_err); /* if not, dispose the screen */
	    got_err = 0;	/* reset this */
	    return;		/* and we are done */
	}
	s = "Server transaction complete";
	break;

#ifdef COMMENT
      case SCR_DT:		/* file creation date */
        strcpy (buf, "Creation date: __/__/__ ");
	buf[15] = s[4];
	buf[16] = s[5];
	buf[18] = s[6];
	buf[19] = s[7];
	buf[21] = s[2];
	buf[22] = s[3];
	strcat (buf, &s[8]);
	s = buf;
	break;
#endif /* COMMENT */

      case SCR_QE:		/* quantity equals */
        strcpy (buf, s);
	strcat (buf, " = ");
	s = &buf[strlen(buf)];
	NumToString(n, s);
	p2cstr(s);
	s = buf;
	scrck = -1;	/* force update of # Ks transfered */
	break;
    }

    if (rnum != SRES_UNDEF)	/* have DITL number for this? */
	SetStrText (rnum, c2p_tmp(s), scrdlg);	/* yes, so set the text */

    if ((scrfsize != fsize) || (scrffc != ffc))
    	update_scr_therm();

    if ((i = (ffc + 512) / 1024) != scrck) {	/* more K's xmitted (or new val)? */
	scrck = i;		/* remember new value */
	NumToString (scrck, buf);	/* convert to number */
	p2cstr(buf);
	if (fsize != 0) {	/* know the size? (only local or w/attrs) */
	    char *cp;
	    strcat (buf, "/");	/* make it be a fraction */
	    cp = (char *) buf + strlen (buf);
	    NumToString ((fsize + 512) / 1024,	/* figure this one out (was filargs.filsiz) */
			 cp);
	    p2cstr(cp);

	}
	c2pstr(buf);
	SetStrText (SRES_KXFER, buf, scrdlg);	/* set new value */
    }
}				/* screen */



/****************************************************************************/
/* scrmydlg - handle dialog events occuring in the screen (status)
 *    	      dialog.  Called by the miniparser when a dialog event
 *    	      occurs and we are supposed to handle it.
 */
/****************************************************************************/
scrmydlg (item)
int item;
{
    switch (item) {
      case SRES_CANF:
	cxseen = TRUE;
	break;
      case SRES_CANG:
	czseen = TRUE;
	break;
    }
}				/* scrmydlg */
