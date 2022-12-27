/* Paul Placeway, Ohio State -- changed the format of saved Is and Cs to */
/*  {number, item, item, ...} */
/* Version 0.8(35) - Jim Noble at Planning Research Corporation, June 1987. */
/* Ported to Megamax native Macintosh C compiler. */
/* Edit by Frank on Jun 20 17:20 */
/* Don't set sio chip parity */
/* Edit by Bill on May 29 01:01 */
/* Add key configurator */
/* Edit by Bill on May 10 9:24 */
/* Saving settings file to a different disk doesn't work and may bomb */
/* Edit by Bill on May 8 7:17 */
/* Save default file settings, now incompatable with existing save files! */
/* Edit by Bill & Jeff on May 1 14:16 */
/* findfinderfiles was bombing because of fName[1] definition of myAppFile */
/* Edit by Bill on Apr 30 05:50 */
/* Call FlushVol after saving the settings */

/*
 * file ckmsav.c
 *
 * Module of MacKermit containing code for saving and restoring
 * various variables.
 *
 * Copyright (C) 1985, Trustees of Columbia University in the City of
 * New York.  Permission is granted to any individual or institution to
 * use, copy, or redistribute this software so long as it is not sold
 * for profit, provided this copyright notice is retained.
 *
 */

#include "ckcdeb.h"		/* Kermit definitions */
#include "ckcker.h"		/* Kermit definitions */

#include "ckmdef.h"		/* Common Mac module definitions */
#include "ckmres.h"		/* resource defs */

OSType kermtype = ApplCreator, settingstype = 'KERS';

int scrinvert;			/* intermediate container for screeninvert */
int scrsize;			/* ditto for size */
int scrpostop;			/* screen position of terminal window -- top edge */
int scrposlft;			/* ditto for left edge */
int scrfntsz;			/* screen font size */
int savinnum;			/* intermediate container for innum (I/O port) */
char savmcmdactive;		/* intermediate container for mcmdactive */

extern int drop_dtr;

extern int dfprty;                      /* Default parity */
extern int dfflow;                      /* Default flow control */
extern int current_size;
extern int current_font;

int *inames[] = {
    &speed, &parity, &duplex, &delay, &mypadn,
    &npad, &timint, &rtimo, &urpsiz, &spsiz,
    &turnch, &turn, &bctr, &filargs.fildflg,
    &newline, &autowrap, &scrinvert, &autorepeat,
    &smoothscroll, &dispcontchar, &keep, &blockcursor,
    &mouse_arrows, &visible_bell, &eightbit_disp,
    &blinkcursor, &scrsize, &savinnum, &sendusercvdef,
    &drop_dtr, &flow, &scrpostop, &scrposlft, &scrfntsz,
    &wslotr
};

#define NINAMES (sizeof(inames) / sizeof(int *))

char *cnames[] = {
    &mypadc, &padch, &eol, &seol, &stchr, &mystch,
    &fkeysactive, &savmcmdactive
};

#define NCNAMES (sizeof(cnames) / sizeof(char *))

typedef long **IHandle;		/* handle to int[] */
typedef char **CHandle;		/* handle to char[] */

extern WindowPtr terminalWindow;



/****************************************************************************/
/* Delete the specified resource if it exists in the current resource file */
/****************************************************************************/
KillResource (type, id)
ResType type;
int id;
{
    Handle theRsrc;

    theRsrc = GetResource (type, id);
    if ((theRsrc != NIL) && (HomeResFile (theRsrc) == CurResFile ())) {
	RmveResource (theRsrc);
	if (ResError () != noErr) {	/* check for error */
	    printerr ("Could not remove old resource: ", ResError ());
	    return;
	}
	DisposHandle (theRsrc);
    }
}				/* KillResource */



/****************************************************************************/
/* savevals - save variables for MacKermit */
/****************************************************************************/
savevals ()
{
    IHandle ihdl;
    CHandle chdl, shdl;

    SFReply savr;
    Point where;
    int err;
    int rfnum;
    int i;
    FInfo finf;
    Str255 name;

    GetWTitle (terminalWindow, name);

    SetPt (&where, 75, 115);
    SFPutFile (where, "\pSave variables in file:", name,
    	       (DlgHookProcPtr) NILPROC, &savr);
    if (!savr.good)		/* did they hit cancel? */
	return;			/* yes, so return now */

    SetVol (NILPTR, savr.vRefNum);	/* select volume for rsrc file */

    /* p2cstr (&savr.fName); */
    rfnum = OpenResFile (&savr.fName);
    err = ResError ();
    if (err == noErr) {		/* file exists, clear old resources */
	/* Be sure to delete the old resources, if they alraedy */
	/* exist. Otherwise the resources will be added to the */
	/* existing file (adding more and more resources with */
	/* the same type and number). Unfortunately, for setting */
	/* the parameters, Kermit always uses the first, i.e. */
	/* the !oldest! set of resources! */
	KillResource (SAVI_TYPE, SIVER);
	KillResource (SAVC_TYPE, SCVER);
	KillResource (KSET_TYPE, KSVER);
	KillResource (MSET_TYPE, KMVER);
	KillResource (SAVS_TYPE, SAVS_ID_FONT);
    } else if ((err == resFNotFound) || (err == fnfErr)) {
	/* file not existing ? */
	CreateResFile (&savr.fName);	/* try to create */
	if (ResError () != noErr) {	/* check for error */
	    printerr ("Unknown error from create: ", ResError ());
	    return;
	}
	/* set the file finder infos */
	err = GetFInfo (&savr.fName, savr.vRefNum, &finf);
	if (err != noErr)
	    printerr ("Can't get finder info for file: ", err);
	else {
	    finf.fdFldr = filargs.filfldr;	/* use same folder as
						 * application */
	    finf.fdType = settingstype;	/* set type */
	    finf.fdCreator = kermtype;	/* set creator */
	    err = SetFInfo (&savr.fName, savr.vRefNum, &finf);
	    if (err != noErr)
		printerr ("Can't set finder info: ", err);
	}
	/* try open again */
	rfnum = OpenResFile (&savr.fName);
	if (rfnum == -1) {	/* failed to open? */
	    printerr ("Couldn't Open resource file: ", ResError ());
	    return;
	}
    } else {
	printerr ("Couldn't Open resource file: ", err);
	return;
    }

    scrinvert = screeninvert;	/* save the current value */
    scrsize = screensize;
    savinnum = innum;		/* save current port too */
    savmcmdactive = mcmdactive;

    get_term_pos(&scrpostop, &scrposlft);	/* in ckmco2.c */
    scrfntsz = current_size;

    /*
     * PWP: changed the format so {count, item, item, ...} so that we can
     * load older versions without dying
     */

    ihdl = (IHandle) NewHandle ((long) (NINAMES + 1) * sizeof (int));
    (*ihdl)[0] = NINAMES;
    for (i = 0; i < NINAMES; i++)	/* copy from indirect table */
	(*ihdl)[i + 1] = *inames[i];
    AddResource ((Handle) ihdl, SAVI_TYPE, SIVER, "");

    chdl = (CHandle) NewHandle ((long) (NCNAMES + 1) * sizeof (char));
    (*chdl)[0] = NCNAMES;
    for (i = 0; i < NCNAMES; i++)	/* copy from indirect table */
	(*chdl)[i + 1] = *cnames[i];
    AddResource ((Handle) chdl, SAVC_TYPE, SCVER, "");

    savekset ();		/* save key bit table */
    savemset ();		/* save key macros table */
    
    /* save the screen font name */
    GetFontName (current_font, name);
    shdl = (CHandle) NewHandle ((long) (Length(name) + 1) * sizeof (char));
    SetString (shdl, name);
    AddResource ((Handle) shdl, SAVS_TYPE, SAVS_ID_FONT, "\pScreen font");
    
    CloseResFile (rfnum);	/* this does an UpdateResFile too */
    FlushVol (NILPTR, savr.vRefNum);	/* flush the bits out */

    DisposHandle((Handle) ihdl);
    DisposHandle((Handle) chdl);
    DisposHandle((Handle) shdl);
    
    SetWTitle (terminalWindow, &savr.fName);
}				/* savevals */



/****************************************************************************/
/* do a Load settings... dialog */
/****************************************************************************/
loadvals ()
{
    SFReply savr;
    Point where;

    SetPt (&where, 75, 115);
    SFGetFile (where, "\pLoad variables from:", (FileFilterProcPtr) NILPROC,
    	       1, &settingstype, (DlgHookProcPtr) NILPROC, &savr);

    if (!savr.good)		/* did they hit cancel? */
	return;			/* yes, so return now */

    p2cstr (&savr.fName);
    doloadvals (&savr.fName, savr.vRefNum);	/* do the load */
}				/* loadvals */



/****************************************************************************/
/****************************************************************************/
findfinderfiles ()
{
    short msg, cnt;
    int err;
    AppFile apf;
    FInfo ainfo;
    Str255 defvolname;
    short defvolrefnum;
    int i;

    CountAppFiles (&msg, &cnt);	/* anything clicked by user? */
    if (cnt == 0 || msg == appPrint) {	/* or they want us to print (?) */
	/* filargs.filfldr = fDesktop; */  /* forget about loading values */
	if (GetVol(&defvolname, &defvolrefnum) == noErr)
	    filargs.filfldr = defvolrefnum;
	else
	    filargs.filfldr = 0;
	loadkset ();		/* make new files appear on desk */
	return;			/* use our default KSET */
    }
    GetAppFiles (1, &apf);	/* get the first one */

    ClrAppFiles (1);		/* done with this */
    
    for (i = 2; i <= cnt; i++)
    	ClrAppFiles (i);	/* claim to be done with the rest too */

    p2cstr (&apf.fName);
    doloadvals (&apf.fName, apf.vRefNum);	/* load the file */
    c2pstr (&apf.fName);

    err = GetFInfo (&apf.fName,	/* get settings file info */
		    apf.vRefNum, &ainfo);
    if (err != noErr)
	printerr ("Couldn't GetFInfo for default folder: ", err);
    filargs.filfldr = ainfo.fdFldr;	/* use appl or text file's folder */
}				/* findfinderfiles */



extern MenuHandle menus[MAX_MENU + 1];	/* handle on our menus */

/****************************************************************************/
/****************************************************************************/
doloadvals (fn, refnum)
char *fn;
int refnum;
{
    int rfnum;
    int i, n;
    IHandle resinames;
    CHandle rescnames, resstrs;
    short fntnum;

    /*
     * Set the place-holder vars to their current values so an update
     * of an older kermit save doesn't do wierd things to them
     */
    scrinvert = screeninvert;	/* save the current value */
    scrsize = screensize;
    savinnum = innum;		/* save current port too */
    savmcmdactive = mcmdactive;
    get_term_pos(&scrpostop, &scrposlft);	/* in ckmco2.c */
    
    scrfntsz = current_size;

    SetVol (NILPTR, refnum);	/* select volume */
    rfnum = OpenResFile (c2p_tmp(fn));	/* open the resource file */
    if (rfnum == -1) {
	printerr ("Couldn't open file: ", ResError ());
	return;
    };

    /* load 'SAVI' resource, the saved integer values, */
    /* 'SAVC' saved characters */

    if ((resinames = (IHandle) GetResource (SAVI_TYPE, SIVER)) == NIL ||
	(rescnames = (CHandle) GetResource (SAVC_TYPE, SCVER)) == NIL) {
	CloseResFile (rfnum);
	printerr ("Can't load your settings, damaged file or wrong version.",
		  0);
	return;			/* and return */
    }
    cursor_erase ();		/* hide the current cursor */

    /*
     * PWP: changed the format to {count, item, item, ...} so that we can
     * load older versions without dieing
     */

    HLock((Handle) resinames);	
    n = (*resinames)[0];
    if (n > NINAMES)
	n = NINAMES;
    for (i = 0; i < n; i++)
	*inames[i] = (*resinames)[i + 1];
    HUnlock((Handle) resinames);	
    ReleaseResource((Handle) resinames);

    HLock(rescnames);	
    n = (*rescnames)[0];
    if (n > NCNAMES)
	n = NCNAMES;
    for (i = 0; i < n; i++)
	*cnames[i] = (*rescnames)[i + 1];
    HUnlock(rescnames);	
    ReleaseResource(rescnames);

    /* restore the screen font name */
    fntnum = 0;
    if ((resstrs = (CHandle) GetResource (SAVS_TYPE, SAVS_ID_FONT)) != NIL) {
	HLock(resstrs);	
	GetFNum(*resstrs, &fntnum);
	if (fntnum != 0)
	    current_font = fntnum;
	HUnlock(resstrs);
	ReleaseResource(resstrs);
    }

    loadkset ();		/* load new KSET */
    loadmset ();		/* release current MSET and load new one */
    
    CloseResFile (rfnum);	/* no longer needed */
    
    set_term_pos(scrpostop, scrposlft);	/* in ckmco2.c */

    current_size = scrfntsz;	/* change font size */
    /* we (may have) changed the font above */
    term_new_font();		/* update what font to use, font constants */
    setup_font_menu();		/* update font and sizes */

    grow_term_to (scrsize);	/* change screen size */

    if (scrinvert != screeninvert)
	invert_term ();

    cursor_draw ();		/* show the new cursor */

    if (savinnum != innum) {	/* if using the other port */
	port_close();
	port_open(savinnum);
    }

    /* tell serial driver about new vals */
    (void) setserial (innum, outnum, speed, KPARITY_NONE);
    
    /* Frank changed main() to call init and then set flow, parity, etc.
       so we make sure they will be set right (again) after we return. */
    dfprty = parity;                    /* Set initial parity, */
    dfflow = flow;                      /* and flow control. */

    /* set the two check menus */
    enable_fkeys (fkeysactive);
    CheckItem (menus[SETG_MENU], SCRD_SETG, (fkeysactive));
    if (savmcmdactive != mcmdactive) {
	mcmdactive = savmcmdactive;
	setup_menus();
    }
    CheckItem (menus[SETG_MENU], MCDM_SETG, (mcmdactive));

    SetWTitle (terminalWindow, c2p_tmp(fn));

    /* (PWP) bounds check the values we just got to be double extra safe */
    
    if (urpsiz > MAXRP-8) {
    	printerr("Recieve packet lengh is too big", urpsiz);
	urpsiz = MAXRP-8;
    }
    if (spsiz > MAXSP) {
    	printerr("Send packet length is too big", spsiz);
	spsiz = MAXSP;
    }
}				/* doloadvals */
