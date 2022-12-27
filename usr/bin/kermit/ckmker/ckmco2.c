/*
 * FILE ckmco2.c
 *
 * Module of mackermit: contains code for dealing with the Mac side
 * of terminal emulation.
 */

#include "ckcdeb.h"
#include "ckcasc.h"

#include "ckmdef.h"
#include "ckmasm.h"		/* Assembler code */
#include "ckmres.h"		/* kermit resources */

#include "ckmcon.h"		/* defines, etc. for terminal emulator */

extern WindowPtr terminalWindow;	/* the terminal window */
Rect ScreenRect;
ControlHandle t_vscroll;

 /*
  * (UoR) don't need scrollrect any more (use scroll_up and scroll_down), use
  * ScreenRect for mouse check
  */

/* Screen book keeping variables */
extern int screensize,		/* variable number of lines on screen */
    topmargin,			/* Edges of adjustable window */
    bottommargin,
    graphicsinset[4],		/* (UoR) current character sets */
    Gl_set, 			/* (UoR) current chosen set */
    Gr_set,			/* (PWP) current chosen upper set */
    screeninvert,		/* (UoR) inverted screen flag */
    insert,
    newline,			/* (UoR) linefeed mode by default */
    autowrap,			/* Autowrap on by default */
    relorigin,			/* (UoR) relative origin off */
    autorepeat,			/* (UoR) auto repeat flag */
    appl_mode,			/* (PWP) application mode */
    curskey_mode,		/* (PWP) cursor key application mode */
    smoothscroll,		/* do smooth scrolling (PWP: or not) */
    scroll_amount,		/* number of lines of scroll we have saved up */
    refresh_amount,		/* number of lines yet to refresh */
    dispcontchar,		/* do not show control characters */
    blockcursor,		/* show block or underline cursor */
    cursor_shown,		/* (PWP) show the cursor */
    mouse_arrows,		/* mouse down in screen does arrow keys */
    visible_bell,		/* true if we do blink instead of bell */
    eightbit_disp,		/* do 8 bit wide to the screen */
    blinkcursor;		/* true if we make the cursor blink */

extern ucharptr *scr, *scr_attrs;		/* virtual screen pointer */
ucharptr *real_scr;		/* The real screen, including scrollback */
ucharptr *real_attrs;		/* the attributes of each character */

int display_topline;		/* top line actually displayed */
int display_totlines;		/* number of real lines in screen + scrollback */
extern int curlin, curcol;	/* Cursor position */
extern int savcol, savlin;		/* Cursor save variables */
extern int savsty, savfnt, savgrf, savmod, savset[2];	/* (UoR) cursor save
						 * variables */
extern int savund;			/* PWP for saved underlining */
extern int scrtop, scrbot;		/* Absolute scrolling region bounds */

int saved_tlin;			/* used with scroll_amount for high-speed scrolling */
int saved_blin;

/* (PWP) font info */
int current_font = VT100FONT;
int current_size = 9;
int textstyle = 0;		/* (UoR) current style */
int current_style = 0;
int draw_sing_chars = FALSE;
int font_is_locked = FALSE;
int lineheight;
int charwidth;
int chardescent;

int oldlin = -1;
int oldcol = 0;			/* (UoR) for last mouse position */

/* (PWP) variables for controling the selected text */
int from_lin = -1;
int from_col = 0;
int to_lin;
int to_col;
Boolean have_selection = 0;

#define ABS(a)	((a) < 0 ? -(a) : (a))

int cursor_invert = FALSE,	/* (UoR) for flashing cursor */
    cur_drawn = FALSE;
long last_flash = 0;

char **myclip_h;		/* internal clipboard */
int myclip_size;		/* size of above */
int my_scrapcount;		/* the value of PScrapStuff->scrapCount when we cut */

static int mousecurs_drawn = FALSE;	/* (PWP) is the mouse cursor drawn? */

static int in_front = 0;	/* PWP: true when we are the front window */

extern Boolean usingRAMdriver,
	       have_128roms;	/* true if we are a Plus or better */

RgnHandle dummyRgn;		/* dummy region for ScrollRect */
				/* Initialized in mac_init */

long MyCaretTime;		/* (UoR) ticks between flashes */

extern Cursor *textcurs, *normcurs, *watchcurs;	/* mouse cursor shapes */

extern	int		to_printer;		/*JAO*/
extern	int		to_screen;		/*JAO*/
extern	int		printer_is_on_line_num;	/*JAO*/
extern	Handle	hPrintBuffer;			/*JAO*/
extern	long	lPrintBufferSize;		/*JAO*/
extern	long	lPrintBufferChars;		/*JAO*/
extern	long	lPrintBufferAt;			/*JAO*/

extern	DialogPtr	bufferingDialog;	/*JAO*/
extern	DialogPtr	overflowingDialog;	/*JAO*/

extern	MenuHandle menus[];	/* handle on our menus */  /*JAO*/

/* keyboard handling stuff */

extern char keytable[512];	/* the key redefintion flag table */
extern modrec modtable[NUMOFMODS];	/* modifier records */

#define myKeyCodeMask	0x7F00
#define keyModifierMask	0x1F00
#define ctrlCodeMask	0x1F
#define metaOrBits	0x80

#define UnmodMask	0x80		/* action bits */
#define CapsMask	0x40
#define CtrlMask	0x20
#define MetaMask	0x10


Boolean have_scriptmgr = FALSE;

long old_KCHR, old_SICN;	/* pointers to current system key script, icon */
long cur_KCHR;

/***** forward decls ********/

void cursor_draw(), cursor_erase();
static void draw_w_line_attrs(register int lin, register int v,
			      register int l_col, register int r_col,
			      int must_drawblanks);
static void mouse_cursor_move (EventRecord *evt);
static void do_arrow(unsigned char dir);
static void do_keypad (int n);
static void do_pfkey(int n);
static void do_keyenter();
static void invert_text(int from_lin, int from_col, int to_lin, int to_col);
static void mouse_region_select (EventRecord *evt);
void maybe_nuke_selection(int tlin, int blin);

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
/* keyboard event handling routines                                         */
/****************************************************************************/


InitKeyStuff()
{
    have_scriptmgr = NGetTrapAddress(num_UnknownTrap, 1) !=
		     NGetTrapAddress(num_ScriptTrap, 1);
		     
    if (have_scriptmgr) {
	old_KCHR = GetScript( smRoman, smScriptKeys);
	old_SICN = GetScript( smRoman, smScriptIcon);
    }
    cur_KCHR = old_KCHR;

    UpdateOptKey(1);	/* get things set right initially */
}

UpdateOptKey(enable)
int enable;
{
    int i;
    int futzit = 0;
    
    if (enable) {
	for (i = 0; i < NUMOFMODS; i++) {
	/* shift what to look for into high byte */
	    if ((modtable[i].modbits) & (optionKey >> 4))	/* if Option is selected */
	        futzit = 1;
	}
    } else {	/* allways turn off when disabling window */
        futzit = 0;
    }
    
    (void) FutzOptKey(futzit);
}

FutzOptKey(enable)
int enable;
{
    int err;
    
    if (have_scriptmgr) {		/* if we are system 4.1 or later... */
	if (enable) {	/* no deadkeys */
	    if (cur_KCHR != old_KCHR)
		return (1);	/* we are allready fine */
	    if (GetEnvirons(smKeyScript) == smRoman) {  /* only if in roman script */
		/* set the key map */
		err = SetScript (smRoman, smScriptKeys, NODEAD_KCHR);
		if (err != noErr) {
		    printerr ("Trouble setting custom keymap (KCHR):", err);
		    return (0);
		}
		/* set the icon */
		err = SetScript (smRoman, smScriptIcon, NODEAD_SICN);
		if (err != noErr) {
		    printerr ("Trouble setting custom keymap icon (SICN):", err);
		    return (0);
		}
		KeyScript (smRoman);
		cur_KCHR = NODEAD_KCHR;
		return (1);	/* success! */
	    } else {
	        printerr("Can't disable Option key -- you have a non-US keyboard",0);
	        return (0);
	    }
	} else {	/* back to normal */
	    if (cur_KCHR == old_KCHR)
		return (1);	/* we are allready fine */
	    /* set the key map */
	    err = SetScript (smRoman, smScriptKeys, old_KCHR);
	    if (err != noErr) {
		printerr ("Trouble resetting default keymap (KCHR):", err);
		return (0);
	    }
	    /* set the icon */
	    err = SetScript (smRoman, smScriptIcon, old_SICN);
	    if (err != noErr) {
		printerr ("Trouble resetting default keymap icon (SICN):", err);
		return (0);
	    }
	    KeyScript (smRoman);
	    cur_KCHR = old_KCHR;
	    return (1);		/* success! */
	}
    } else {
	/* do something or other to do the old way */
	/* printerr("Kermit can't disable Option on old systems",0); */
    }
    return (0);
}


/****************************************************************************/
/* return the ASCII character which is generated by the keyCode specified */
/* with no modifiers pressed */
/****************************************************************************/
unsigned char
DeModifyChar (keyCode, modifiers)
long keyCode, modifiers;
{
    long c;
    long mystate;
    short s_keycode;
    Handle kchr_h;
    THz curZone;

#ifdef COMMENT
    ProcHandle KeyTrans;

    if (keyCode > 64)
	KeyTrans = (ProcHandle) 0x2A2;	/* keypad decode */
    else
	KeyTrans = (ProcHandle) 0x29E;	/* keyboard decode */

    SaveRegs ();		/* save all registers */
    AllRegs ();

    /* setup regs for procedure call */
    /* loadD1 ((long) modifiers); */		/* no modifiers */
    loadD1 ((long) 0);		/* no modifiers */
    loadD2 ((long) keyCode);	/* set the keycode */
    loadA0 (*KeyTrans);		/* load the content of Key1Trans to A0 */

    /* run the translation routine */
    execute ();			/* call the Key1Trans procedure */

    /* move the result from reg D0 to c */
    loadA0 (&c);		/* set destination address */
    pushD0 ();			/* move register D0 to stack */
    poptoA0 ();			/* load the stacktop to (A0) */

    RestoreRegs ();		/* restore all registers */
    AllRegs ();

#endif /* COMMENT */

    if (have_scriptmgr) {		/* if we are system 4.1 or later... */
        mystate = 0;
	
	kchr_h = GetResource('KCHR', cur_KCHR);
	if (kchr_h == NIL) {
	    printerr("DeModifyChar: couldn't get KCHR address",0);
	    return(0);
	}
	LoadResource(kchr_h);
	HLock(kchr_h);
	
	s_keycode = (modifiers & 0xff00) | (keyCode & 0xff);

	c = KeyTrans(*kchr_h, s_keycode, &mystate);
	HUnlock(kchr_h);
	curZone = GetZone();		/* as per John Norstad's (Disinfectant) */
	SetZone(HandleZone(kchr_h));	/* "Toolbox Gotchas" */
	ReleaseResource(kchr_h);
	SetZone(curZone);
    }    
    return (c);
}				/* DeModifyChar */



unsigned char obuf[2] = {1, 0};	/* single char output buffer */

/****************************************************************************/
/* send a character to the line if it is in ASCII range. Do local echo if */
/* necessary */
/****************************************************************************/
OutputChar (c)
unsigned char c;
{

    /*
     * PWP: NO 7 bit masking!!!  If we do this, then I can't use Emacs, and
     * the European users will be VERY unhappy, 'cause they won't be able to
     * send all of their characters.
     */

    obuf[1] = c;		/* store character */
    writeps (obuf);		/* and write it out */

    if (duplex != 0) {
	cursor_erase ();	/* remove from screen */
	printem (&obuf[1], 1);	/* Echo the char to the screen */
	flushbuf ();		/* flush the character */
	cursor_draw ();		/* put it back */
    }
}				/* OutputChar */

#ifdef COMMENT
/****************************************************************************/
/* Bittest returns the setting of an element in a Pascal PACKED ARRAY [0..n]
   OF Boolean such as the KeyMap argument returned by GetKey
/****************************************************************************/
Boolean
bittest (bitmap, bitnum)
char bitmap[];
int bitnum;
{
    return (0x01 & (bitmap[bitnum / 8] >> (bitnum % 8)));
}				/* bittest */

/* PWP: or, as a macro, */
#define bittest(bitmap,bitnum)	(0x01 & (bitmap[bitnum / 8] >> (bitnum % 8)))

#endif /* COMMENT */


/****************************************************************************/
/* Process a character received from the keyboard */
/****************************************************************************/
handle_char (evt)
EventRecord *evt;
{
    short i;
    short len;
    short theCode;
    short modCode;
    short theModBits;
    char flags;
    char tmpstr[256];
    unsigned char c;

    /* (UoR) check for auto repeated keys */
    if ((autorepeat == FALSE) && (evt->what == autoKey))
	return;

    ObscureCursor ();		/* PWP: hide the cursor until next move */

    modCode = evt->modifiers & keyModifierMask;
    theCode = ((evt->message & myKeyCodeMask) >> 8) + (modCode >> 1);
    
    /* check for a special code for this key */
    if (BitTst (keytable, theCode)) {
	GetMacro (theCode, &flags, tmpstr);	/* get the macrostring */

	if (flags) {		/* check special flags */
	    switch (flags) {
	      case shortBreak:
		sendbreak (5);
		return;

	      case longBreak:
		sendbreak (70);
		return;

	      case leftArrowKey:
		do_arrow (leftARROW);
		return;

	      case rightArrowKey:
		do_arrow (rightARROW);
		return;

	      case upArrowKey:
		do_arrow (UPARROW);
		return;

	      case downArrowKey:
		do_arrow (DOWNARROW);
		return;
		
	      case keycomma:
	      case keyminus:
	      case keyperiod:
	      /* there is no keyslash */
	      case key0:
	      case key1:
	      case key2:
	      case key3:
	      case key4:
	      case key5:
	      case key6:
	      case key7:
	      case key8:
	      case key9:
		do_keypad(flags - keycomma);
		return;
		
	      case keypf1:
	      case keypf2:
	      case keypf3:
	      case keypf4:
		do_pfkey(flags - keypf1);
		return;
		
	      case keyenter:
		do_keyenter();
		return;		
	    }
	}
	/* send key macro string */

	/*
	 * PWP: note, we DON'T have to convert it to a Pascal string, 'cause
	 * the macros are now stored as Pascal strings
	 */
	writeps (tmpstr);	/* send it to the line */
	if (duplex != 0)
	    printps (tmpstr);	/* echo it locally */
	return;
    }
    for (i = 0; i < NUMOFMODS; i++) {
	/* shift what to look for into high byte */
	theModBits = modtable[i].modbits << 4;
	len = strlen (modtable[i].prefix);

	if ((theModBits || len) &&
	    ((theModBits & modCode) == (theModBits & keyModifierMask))) {
	    /* send prefix if there is one */
	    if (len) {
		/* PWP: these are saved as Pascal strings now */
		BlockMove (modtable[i].prefix, tmpstr, (modtable[i].prefix[0] + 1));
		writeps (tmpstr);	/* send it to the line */
		if (duplex != 0)
		    printps (tmpstr);	/* echo it locally */
	    }

	    /*
	     * get the unmodified ASCII code if the unmodify action bit is
	     * active
	     */
	    if (theModBits & UnmodMask)
		c = DeModifyChar ((long) ((evt->message & myKeyCodeMask) >> 8),
				  (long) (modCode & shiftKey));
			 /* PWP: we pass through the shiftedness of this key */
	    else
		c = evt->message & charCodeMask;	/* otherwise get the
							 * standard char */

	    /* make an uppercase character if the caps action bit is active */
	    if ((theModBits & CapsMask) && islower (c))
		c = toupper (c);

	    /* make a control character if the control action bit is active */
	    if (theModBits & CtrlMask)
		c &= ctrlCodeMask;

	    /* PWP: for Meta characters (yes, I use Emacs) */
	    if (theModBits & MetaMask)
		c |= metaOrBits;

	    OutputChar (c);
	    return;
	}			/* if */
    }				/* for */

    /* get the ASCII code and send it */
    OutputChar (evt->message & charCodeMask);
}				/* handle_char */

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
/* general rectangle routines                                               */
/****************************************************************************/



/****************************************************************************/
/*
 * Routine makerect
 *
 * Make a rectangle in r starting on line lin and column col extending
 * numlin lines and numcol characters.
 *
 */
/****************************************************************************/
makerect (r, lin, col, numlin, numcol)
Rect *r;
int lin;
int col;
int numlin;
int numcol;
{
    r->top = lin * lineheight + TOPMARGIN;
    r->left = col * charwidth + LEFTMARGIN;
    r->bottom = r->top + numlin * lineheight;
    r->right = r->left + numcol * charwidth;
}				/* makerect */

/* (PWP) do what makerect does, then invert the rect */
invertchars (lin, col, numlin, numcol)
int lin;
int col;
int numlin;
int numcol;
{
    Rect r;
    r.top = lin * lineheight + TOPMARGIN;
    r.left = col * charwidth + LEFTMARGIN;
    r.bottom = r.top + numlin * lineheight;
    r.right = r.left + numcol * charwidth;
    InvertRect (&r);
}

#ifdef UNUSED_CODE
/* Make rect r (made by makerect()) into the right shape
	 for underlining */
makertou (r)
Rect *r;
{
    r->top = r->bottom;
    r->bottom = r->top + 2;
}
#endif /* UNUSED_CODE */

/****************************************************************************/
/* Connect support routines */
/****************************************************************************/
void
term_new_font()
{
    FontInfo fi;
    GrafPtr savePort;
    
    GetPort (&savePort);	/* there just has to be a better way */
    SetPort (terminalWindow);

    SetFontLock(false);
    font_is_locked = false;
    
    TextFont(current_font);	/* make sure to set this font */
    TextSize(current_size);
    
    GetFontInfo (&fi);
    
    lineheight = fi.ascent + fi.descent + fi.leading;
    chardescent = fi.descent;
    /* charwidth = fi.widMax; */
    charwidth = CharWidth('W');		/* idea from NCSA telnet 2.3 */

#ifdef COMMENT
    printerr("current_size == ", current_size);
    printerr("current_font == ", current_font);
    printerr("lineheight == ", lineheight);
    printerr("charwidth == ", charwidth);
    printerr("chardescent == ", chardescent);
#endif

    SetPort (savePort);		/* there just has to be a better way */
}
    
/* consetup is called once at startup */
void
consetup ()
{
    GrafPtr savePort;
    
    GetPort (&savePort);	/* there just has to be a better way */
    SetPort (terminalWindow);

    PenMode (srcCopy);		/* (PWP) was patXor */
    flushio ();			/* Get rid of pending characters */

    current_font = VT100FONT;	/* (UoR) Set initial font to VT100 */
    current_size = 9;
#ifdef COMMENT
    current_font = monaco;	/* (UoR) Set initial font to VT100 */
    current_size = 12;
#endif
    term_new_font();
    
    if (screeninvert)
	TextMode (srcBic);
    else
	TextMode (srcOr);
    TextFace (0);		/* PWP: be safe.  We allways stay like this */

    init_term ();		/* Set up some terminal variables */

    /* normal char mode, home cursor, clear screen, and save position */
    norm_home_clear_save();

    cursor_draw ();		/* (UoR) be sure to draw it */

    SetPort (savePort);		/* there just has to be a better way */
}				/* consetup */

void
term_reset ()
{
    GrafPtr savePort;
    
    GetPort (&savePort);	/* there just has to be a better way */
    SetPort (terminalWindow);

    PenMode (srcCopy);		/* (PWP) was patXor */
    flushio ();			/* Get rid of pending characters */
    screen_to_bottom ();	/* slide the visible region to active area */

    graphicsinset[0] = ASCII_SET;
    graphicsinset[1] = GRAF_SET;
    Gl_set = 0;
    Gr_set = 1;
    textstyle = 0;
    current_style = 0;
    draw_sing_chars = 0;
    font_is_locked = FALSE;
    screeninvert = FALSE;	/* (UoR) inverted screen flag */
    insert = FALSE;
    newline = FALSE;		/* (UoR) linefeed mode by default */
    autowrap = TRUE;		/* Autowrap on by default */
    relorigin = FALSE;		/* (UoR) relative origin off */
    autorepeat = TRUE;		/* (UoR) auto repeat flag */
    appl_mode = FALSE;		/* (PWP) keypad application mode */
    curskey_mode = FALSE;	/* (PWP) cursor key application mode */
    smoothscroll = FALSE;	/* do smooth scrolling (PWP: or not) */
    scroll_amount = 0;		/* no pending scroll */
    refresh_amount = 0;		/* no pending refresh */
    dispcontchar = TRUE;	/* do not show control characters */
    blockcursor = TRUE;		/* show block or underline cursor */
    cursor_shown = TRUE;	/* (PWP) show the cursor */
    mouse_arrows = FALSE;	/* mouse down in screen does arrow keys */
    visible_bell = FALSE;	/* true if we do blink instead of bell */
    eightbit_disp = FALSE;	/* default to 7 bits */
    blinkcursor = TRUE;		/* true if we make the cursor blink */

    have_selection = FALSE;	/* (PWP) we have no selected text */
    
    saved_tlin = 0;
    saved_blin = 0;
    
    current_font = VT100FONT;	/* (UoR) Set initial font to VT100 */
    current_size = 9;
    term_new_font();
#ifdef COMMENT
    TextMode (srcXor);		/* (UoR) use XOR mode (for inverse) */
#endif
    if (screeninvert)
	TextMode (srcBic);
    else
	TextMode (srcOr);
    TextFace (0);		/* PWP: be safe.  We allways stay like this */
    
    /* normal char mode, home cursor, clear screen, and save position */
    norm_home_clear_save();
    
    cursor_draw ();		/* (UoR) be sure to draw it */

    SetPort (savePort);		/* there just has to be a better way */
}				/* consetup */

/****************************************************************************/
/****************************************************************************/


/*************************************************/
/* cursor drawing stuff                          */
/*************************************************/

Boolean
cursor_rect (line, col, r)
Rect *r;
{
    if (line - display_topline >= screensize)	/* if cursor not on screen */
    	return FALSE;
	
    if (col >= 80)	/* make it look like a VT100 */
	col = 79;
    
    makerect (r, line - display_topline, col, 1, 1);	/* Get character rectangle */
    if (!blockcursor) {
	r->top = r->bottom;
	r->bottom = r->top + 2;
    }

    return TRUE;
}				/* cursor_rect */

void
cursor_draw ()
{
    Rect r;
    GrafPtr savePort;

    if (!cursor_shown) return;		/* (PWP) not if we are hiding cursor */
    
    GetPort (&savePort);	/* there just has to be a better way */
    SetPort (terminalWindow);

    if (!cursor_invert) {
	if (cursor_rect (curlin, curcol, &r)) {
	    if (in_front) {
		InvertRect (&r);
	    } else {
		PenMode (patXor);
		FrameRect (&r);
		PenMode (patCopy);
	    }
	}
    }

    if ((oldlin >= 0) && (!mousecurs_drawn)) {	/* (UoR) replace mouse cursor */
	makerect (&r, oldlin, oldcol, 1, 1);
	PenMode (patXor);
	FrameRect (&r);
	PenMode (patCopy);
	mousecurs_drawn = TRUE;
    }
    
    cursor_invert = TRUE;
    cur_drawn = TRUE;

    SetPort (savePort);		/* there just has to be a better way */
}				/* cursor_draw */


void
cursor_erase ()
{
    Rect r;
    GrafPtr savePort;
    
    GetPort (&savePort);	/* there just has to be a better way */
    SetPort (terminalWindow);


    if (cursor_invert) {
	if (cursor_rect (curlin, curcol, &r)) {
	    if (in_front) {
		InvertRect (&r);
	    } else {
		PenMode (patXor);
		FrameRect (&r);
		PenMode (patCopy);
	    }
	}
    }
    
    if ((oldlin >= 0) && (mousecurs_drawn)) {	/* (UoR) remove mouse cursor */
	makerect (&r, oldlin, oldcol, 1, 1);
	PenMode (patXor);
	FrameRect (&r);
	PenMode (patCopy);
 	mousecurs_drawn = FALSE;
   }

    cursor_invert = FALSE;
    cur_drawn = FALSE;

    SetPort (savePort);		/* there just has to be a better way */
}				/* cursor_erase */

void
flash_cursor (theWindow)
WindowPtr theWindow;
{
    register long tc;
    Rect r;
    GrafPtr savePort;
    
    if (theWindow == (WindowPtr) NIL) {
	last_flash = TickCount ();
	return;
    }

    GetPort (&savePort);	/* there just has to be a better way */
    SetPort (terminalWindow);

    tc = TickCount ();
    if (((tc - last_flash) > MyCaretTime) ||
	(tc - last_flash) < 0L) {
	last_flash = tc;

	if (cur_drawn) {
	    if (cursor_rect (curlin, curcol, &r)) {
		if (blinkcursor && in_front) {	/* PWP: only blink if asked for */
		    InvertRect (&r);
		    cursor_invert = !cursor_invert;
		} else if (!cursor_invert) {	/* make sure that the cursor
						 * shows up */
		    if (in_front) {
			InvertRect (&r);
		    } else {
			PenMode (patXor);
			FrameRect (&r);
			PenMode (patCopy);
		    }
		    cursor_invert = TRUE;
		}
	    }
	}
    }

    SetPort (savePort);		/* there just has to be a better way */
}				/* flash_cursor */


/****************************************************************************/
/* PWP -- like waitasec(), but don't get any characters.  Used for 
   visable bell. */
/****************************************************************************/
waitnoinput ()
{
    long ticks = 2, end_time;

    Delay (ticks, &end_time);	/* pause for 1/30th second */
}				/* waitnoinput */


/****************************************************************************/
/* (UoR) get any characters, and pause for a while */
/****************************************************************************/
waitasec ()
{
    waitnoinput();
    inpchars ();
}				/* waitasec */


/****************************************************************************/
/* updateCursor -- taken from NCSA Telnet for the Macintosh, v 2.2   */
/****************************************************************************/
void
updateCursor(force, myfrontwindow)
int force;
WindowPeek myfrontwindow;
{
    static Point lastPoint;
    static int optwasdown = 0;
    static Cursor *lastCursor=0L;	/* what we set the cursor to last */
    Cursor *thisCursor;
    int optDown;
    KeyMap allthekeys;	/* Someplace to put the keymap */
    char *cp_allthekeys = (char *) &allthekeys;	/* $$$ HACK HACK */
    int newlin, newcol;
    Point MousePt;
    Rect r;
    GrafPtr savePort;
    
    GetPort (&savePort);	/* there just has to be a better way */
    SetPort (terminalWindow);

    GetMouse(&MousePt);

    GetKeys(allthekeys);
    optDown = cp_allthekeys[7] & 4;	/* should be symbolic */

    if ( (!force) && (!in_background) && (MousePt == lastPoint) && (optDown == optwasdown))
    {
	SetPort (savePort);		/* there just has to be a better way */
	return;
    }
    
    if (force)
	lastCursor=0L;
	
    if (in_background)
    {
	lastCursor = 0L;		/* allways force if in background */
	thisCursor = normcurs;		/* default cursor shape */
    }
    else if (protocmd != 0)
    {					/* if doing a transfer */
	thisCursor = watchcurs;	/* in forground and doing a transfer */
    }
    else if (((myfrontwindow == terminalWindow) ||
		((myfrontwindow == NIL) &&
		 (FrontWindow() == terminalWindow))) &&
	       PtInRect (MousePt, &ScreenRect))
    {
	if (mouse_arrows || optDown) {
	    newlin = (MousePt.v - TOPMARGIN) / lineheight;
	    newcol = (MousePt.h - LEFTMARGIN + charwidth/2) / charwidth;
	
	    if ((oldlin != newlin) || (oldcol != newcol)) {
		PenMode (patXor);	/* For FrameRect calls */
		if (oldlin >= 0) {	/* if old rectangle */
		    if (mousecurs_drawn) {
			makerect (&r, oldlin, oldcol, 1, 1);
			FrameRect (&r);
		    }
		} else {		/* else if drawing for the first time */
		    HideCursor ();
		}
		
		makerect (&r, newlin, newcol, 1, 1);
		FrameRect (&r);
		PenMode (patCopy);	/* reset to normal pen mode */
	
		oldlin = newlin;
		oldcol = newcol;
		mousecurs_drawn = TRUE;
	    }
	    lastPoint=MousePt;
	    optwasdown=optDown;
	    SetPort (savePort);		/* there just has to be a better way */
	    return;
	} else {			
	    thisCursor = textcurs;
	}
    }
    else
    {
	thisCursor  = normcurs;		/* default cursor shape */
    }
	
    if  (lastCursor!= thisCursor) {
	SetCursor(thisCursor);
	lastCursor = thisCursor;
    }

    lastPoint=MousePt;
    optwasdown=optDown;

    if (oldlin >= 0) {		/* if we hade drawn a movement outline */
	if (mousecurs_drawn) {
	    PenMode (patXor);	/* For FrameRect calls */
	    makerect (&r, oldlin, oldcol, 1, 1);
	    FrameRect (&r);
	}
	
	oldlin = -1;
 	mousecurs_drawn = FALSE;
	ShowCursor ();
	PenMode (patCopy);	/* reset to normal pen mode */
    }

    SetPort (savePort);		/* there just has to be a better way */
}


/****************************************************************************/
/* Put characters onto the actual screen                                    */
/****************************************************************************/

int out_maxcol = 0, out_mincol;

static int to_mac_style[] = {
	normal,	underline, italic, underline|italic,
	bold|condense, bold|condense|underline,
	bold|condense|italic, bold|condense|underline|italic
};

/****************************************************************************/
/* flushbuf() -- draw all the buffered characters on the screen */
/****************************************************************************/
void
flushbuf ()
{
    register int i, scrl_amt;
    Rect r;

    if (out_maxcol == 0)
	return;			/* Nothing to flush */

    if (to_printer) {					/*JAO*/
	for (i = out_mincol; i < out_maxcol; i++) {
	    (*hPrintBuffer)[lPrintBufferAt++] = scr[curlin+display_topline][i];
	    if (lPrintBufferAt == lPrintBufferSize)
		lPrintBufferAt = 0L;
	    lPrintBufferChars++;
	    if (lPrintBufferChars == lPrintBufferSize) {
		overflowingDialog = GetNewDialog(OVERFLOWINGBOXID, NILPTR, (WindowPtr) - 1);
		DrawDialog(overflowingDialog);
		break;					/* PWP */
	    }
	}
    }

    if (!to_screen) {
	out_maxcol = 0;			/* Say no more chars to output */
	return;													/*JAO*/
    }
    
    scrl_amt = scroll_amount;	/* save because flushscroll() resets scroll_amount */
    if (scrl_amt) {
	/*
	 * If we have pending scrolling, and we are about to draw outside of the
	 * region that will be refreshed when the scrolling happens, do the scrolling
	 * now.
	 */
    	if ((refresh_amount < 0) && ((curlin < saved_blin + refresh_amount)
				     || (curlin > saved_blin)))
	{
	    flushscroll();
	}
	else if ((refresh_amount > 0) && ((curlin < saved_tlin)
					  || (curlin > saved_tlin + refresh_amount)))
	{
	    flushscroll();
	}
	else
	{
	    out_maxcol = 0;		/* Say no more chars to output */
	    return;													/*JAO*/
	}
    }
    
    if (have_selection)
	maybe_nuke_selection (curlin, curlin);
    
    /*
     * PWP: Why have two routines to do the same thing?  I centralized
     * all the font crap into draw_w_line_attrs() so I didn't need
     * to keep track of code here too.  Side benifit -- we can nuke
     * the outbuf array.
     */
    draw_w_line_attrs(curlin+display_topline, curlin,
		      out_mincol, out_maxcol, (scrl_amt == 0));
    
    out_maxcol = 0;		/* Say no more chars to output */
}				/* flushbuf */

/****************************************************************************/
/* set_style(style) - set the correct stuff for displaying chars in style   */
/****************************************************************************/
static void
set_style(style)
int style;
{
    int m_sty, m_font;
    static int o_sty = 0, o_font = 0;
    
    if (style == current_style) return;
    current_style = style;
    
    m_sty = to_mac_style[style & STY_MSTY];
    m_font = ((style & STY_FONT) >> 3) + current_font;
    draw_sing_chars = ((style & VT_BLINK)
		       || (CharWidth('W')!=CharWidth('i'))
		       || (!RealFont(current_font, current_size)));
/* printerr("draw_sing_chars == ", draw_sing_chars); */

    if (!have_128roms && (m_sty & bold)) {	/* if on an old mac and bolding */
	if (m_font == VT100FONT) {
	    m_font = VT100BOLD;
	    m_sty &= ~(bold|condense);
	} else {
	    draw_sing_chars = 1;
	}
    }

    if (m_font != o_font) {
	TextFont (m_font);		/* new font */
	o_font = m_font;
    }
    if (m_sty != o_sty) {
	TextFace (m_sty);		/* new text face */
	o_sty = m_sty;
    }
}

/****************************************************************************/
/****************************************************************************/
static void
draw_w_line_attrs(register int lin, register int v,
		  register int l_col, register int r_col,
		  int must_drawblanks)
{
    register int o, i, sty, j;
    register char *cp, *ap;
    register int max;
    Rect r;
    
    /***** I shouldn't have to put this here! *****/
    if (screeninvert) {
	BackPat(qd.black);
	PenPat(qd.white);
	TextMode (srcBic);
    } else {
	BackPat(qd.white);
	PenPat(qd.black);
	TextMode (srcOr);
    }

    if ((v < 0) || (v > screensize))
	printerr("draw_w_line_attrs, v out of range:", v);

    cp = scr[lin];
    ap = scr_attrs[lin];
    
    /*
     * find the last character that is not a plain space character
     */
    sty = 0;	/* the style for normal blank space */
    for (max = r_col-1; max >= l_col; max--) {
			/* PWP: we should never see a NUL in the line here */
	if ((cp[max] != ' ') || (ap[max] != sty))
	    break;
    }
    max++;
    if (max > r_col) max = r_col;

    if (must_drawblanks && max < r_col) {
	makerect (&r, v, max, 1, r_col-max);
	EraseRect (&r);		/* set r to bkPat */
    }


    /*
     * loop through all the characters up to max, looking for the longest
     * string of characters all of the same style.  Draw them, switch styles,
     * and repeat until max.
     */
    sty = ap[l_col];
    o = l_col;
    i = l_col;
    do {
    	if ((ap[i] != sty) || (i == max)) {	/* if this style != current style */
	    if (must_drawblanks) {
		makerect (&r, v, o, 1, i-o);
		EraseRect (&r);
	    }
	    
	    if (sty != current_style)
		set_style(sty);
		
	    if (draw_sing_chars) {
		for (j = o; j < i; j++) {
		    MOVETOCHAR(j, v);
		    DrawChar((short) (cp[j] & 0377));
		}
	    } else {	/* non-blinking */
		MOVETOCHAR(o, v);
		DrawText (cp, (short) o, (short) i-o);	/* Output this part */
	    }
	    
	    if (sty & VT_INVERT) {
		makerect (&r, v, o, 1, i-o);
		InvertRect (&r);
	    }
	    
	    if (!font_is_locked) {
		SetFontLock(true);
		font_is_locked = true;
	    }
	    
	    o = i;	/* now left extent == current */
	    sty = ap[i];	/* new current style */
	}
	i++;
    } while (i <= max);
}

/****************************************************************************/
/****************************************************************************/
static void
scroll_term()
{
    register int new_topline, delta, lin, i;
    int fl, fc, tl, tc;
    Rect r;			/* cannot be register */

    new_topline = screensize - display_totlines + GetCtlValue (t_vscroll);
    if ((new_topline > 0) || (new_topline <  screensize - MAX_SCREENSIZE)) {
    	printerr("BUG: in scroll_term(), new_topline out of range:", new_topline);
	return;
    }
    if ((delta = (display_topline - new_topline)) == 0)
	return;		/* we didn't move */

    makerect (&r, 0, 0, screensize, MAXCOL);
    
    if ((delta >= screensize) || (-delta >= screensize)) {   /* if whole screen */
    	EraseRect(&r);
	
    	lin = new_topline;			/* new top line */
	for (i = 0; i < screensize; i++) {
	    draw_w_line_attrs(lin, i, 0, MAXCOL, 0);
	    lin++;
	}
	display_topline = new_topline;

	if (have_selection)
	    invert_text(from_lin, from_col, to_lin, to_col);
	    
	return;	/* we are done */
    }

    /* if we get here, we are not doing the whole screen */
    ScrollRect (&r, 0, delta * lineheight, dummyRgn);

    if (delta > 0) {	/* scrolling down (pushing top arrow) */
    	lin = new_topline;			/* new top line */
	for (i = 0; (i < delta) && (i < screensize); i++) {
	    draw_w_line_attrs(lin, i, 0, MAXCOL, 0);
	    lin++;
	}
	display_topline = new_topline;

	if (have_selection &&
	    (from_lin < display_topline + delta) &&
	    (to_lin >= display_topline)) {
	    if (from_lin < display_topline) {
		fl = display_topline;
		fc = 0;
	    } else {
		fl = from_lin;
		fc = from_col;
	    }
	    if (to_lin >= display_topline + delta) {
		tl = display_topline + delta - 1;
		tc = MAXCOL;
	    } else {
		tl = to_lin;
		tc = to_col;
	    }
	    invert_text(fl, fc, tl, tc);
	}
    } else {		/* scrolling up (pushing bottom arrow) */
    	lin = display_topline + screensize;	/* one past old bottom line */
	/*********** PWP: delta is negative here ****************/
	i = screensize + delta;
	if (i < 0) i = 0;	/* bounds */
	while (i < screensize)
	    draw_w_line_attrs(lin++, i++, 0, MAXCOL, 0);
	display_topline = new_topline;

	if (have_selection &&
	    (from_lin < (display_topline + screensize)) &&
	    (to_lin >= (display_topline + screensize + delta))) {
	    if (from_lin < display_topline + screensize + delta) {
		fl = display_topline + screensize + delta;
		fc = 0;
	    } else {
		fl = from_lin;
		fc = from_col;
	    }
	    if (to_lin >= display_topline + screensize) {
		tl = display_topline + screensize - 1;
		tc = MAXCOL;
	    } else {
		tl = to_lin;
		tc = to_col;
	    }
	    invert_text(fl, fc, tl, tc);
	}
    }
}

static pascal void
doscroll (WHICHCONTROL, THECODE)
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
    if ((val >= 0) && (val <= max)) {
	SetCtlValue (WHICHCONTROL, val);
	scroll_term ();
    }
}				/* doscroll */


/****************************************************************************/
/* we move the displayed region to the bottom when we recieve characters */
/****************************************************************************/

screen_to_bottom()
{
    if (display_topline != toplin) {
	SetCtlValue (t_vscroll, display_totlines - screensize);
	scroll_term ();
    }
}

/****************************************************************************/
/* update_vscroll - adjust the scaling of the vertical scroll bar, or  */
/*    	      	    disable it if we havn't saved anything back yet */
/****************************************************************************/
update_vscroll ()
{
    if (in_front && display_totlines > screensize) {
	SetCtlMax (t_vscroll, display_totlines - screensize);
	SetCtlValue (t_vscroll, display_totlines - screensize);
	HiliteControl (t_vscroll, 0);
    } else {
	HiliteControl (t_vscroll, 255);
    }
}				/* sizescrollbars */

/****************************************************************************/
/****************************************************************************/
static void
t_pagescroll (code, amount, ctrlh)
ControlHandle ctrlh;
{
    Point myPt;
    register int val, max;

    max = GetCtlMax (ctrlh);
    val = GetCtlValue (ctrlh);
    
    do {
	GetMouse (&myPt);
	if (TestControl (ctrlh, myPt) != code)
	    continue;
	
	val += amount;
	if (val < 0)
	    val = 0;
	if (val > max)
	    val = max;
	SetCtlValue (ctrlh,  val);
	scroll_term ();
    } while (StillDown ());
}				/* pagescroll */


termmouse(evt)
EventRecord *evt;
{
    int actrlcode;
    long ticks;
    ControlHandle acontrol;
    GrafPtr savePort;
    
    GetPort (&savePort);	/* save the current port */
    SetPort (terminalWindow);

    GlobalToLocal (&evt->where);/* convert to local */
    if (mouse_arrows || (evt->modifiers & optionKey)) {
	if (PtInRect (evt->where, &ScreenRect)) {	/* In terminal content? */
	    mouse_cursor_move(evt);	    
	    SetPort (savePort);		/* restore previous port */
	    return;			/* yes, do mouse stuff */
	}
    }
    cursor_erase();
    actrlcode = FindControl (evt->where, terminalWindow, &acontrol);
    switch (actrlcode) {	
      case inUpButton:
      case inDownButton:
	(void) TrackControl (acontrol, evt->where, (ProcPtr) doscroll);
	break;

      case inPageUp:
	t_pagescroll (actrlcode, -(screensize/2), acontrol);
	break;

      case inPageDown:
	t_pagescroll (actrlcode, (screensize/2), acontrol);
	break;

      case inThumb:
	(void) TrackControl (acontrol, evt->where, (ProcPtr) NIL);
	scroll_term ();
	break;
	
      case 0:		/* in the window content itself */
        /* $$$ SHOULD DO SOMETHING ABOUT DOUBLE CLICKS HERE!! */
	mouse_region_select(evt);
	break;
    }
    /* MOVETOCHAR(curcol, curlin - display_topline); */
    cursor_draw();
    SetPort (savePort);		/* restore previous port */
}


static void
do_arrow(dir)	/* dir is 'A' (up), 'B' (down), 'C' (right), or 'D' (left) */
unsigned char dir;
{
    OutputChar('\033');		/* ESC */
    if (curskey_mode)
    	OutputChar('O');	/* SS3 */
    else
    	OutputChar('[');	/* CSI */
    OutputChar(dir);
}

static void
do_keypad (n)	/* char to send is n + ',' */
int n;
{
    if (appl_mode) {
    	OutputChar('\033');		/* ESC */
    	OutputChar('O');	/* SS3 */
	OutputChar((unsigned char) n + 'l');
    } else {
    	OutputChar((unsigned char) n + ',');	/* normal digit or glyph */
    }
}

static void
do_pfkey(n)	/* pf1 == 0 ... pf4 == 3 */
int n;
{
    OutputChar('\033');		/* ESC */
    OutputChar('O');	/* SS3 */
    OutputChar((unsigned char) n + 'P');
}

static void
do_keyenter()
{
    if (appl_mode) {
    	OutputChar('\033');		/* ESC */
    	OutputChar('O');	/* SS3 */
	OutputChar('M');
    } else {
    	OutputChar('\015');
    }
}

static void
mouse_cursor_move (evt)
EventRecord *evt;
{
    int mouselin;
    int mousecol;
    int tempcol;
    int templin;
    int i;
    Point MousePt;

    MousePt = evt->where;
    mouselin = (MousePt.v - TOPMARGIN) / lineheight;
    mousecol = (MousePt.h - LEFTMARGIN + charwidth/2) / charwidth;
    tempcol = curcol;
    templin = curlin;

    if (mousecol < tempcol)
	for (i = tempcol; i > mousecol; i--) {
	    do_arrow (leftARROW);
	    waitasec ();
	    /* If tabs are used, we may go too far, so end loop */
	    if (curcol <= mousecol)
		i = mousecol;
	}

    if (mouselin < templin)
	for (i = templin; i > mouselin; i--) {
	    do_arrow (UPARROW);
	    waitasec ();
	}

    else if (mouselin > templin)
	for (i = templin; i < mouselin; i++) {
	    do_arrow (DOWNARROW);
	    waitasec ();
	}

    if (curlin == mouselin)
	tempcol = curcol;	/* for short lines */

    if (tempcol < mousecol)
	for (i = tempcol; i < mousecol; i++) {
	    do_arrow (rightARROW);
	    waitasec ();
	    /* If tabs are used, we may go too far, so end loop */
	    if (curcol >= mousecol)
		i = mousecol;
	}
}				/* mouse_cursor_move */

static void
invert_text(int from_lin, int from_col, int to_lin, int to_col)
{
    int t;
    
    if (from_lin > to_lin) {		/* make from < to */
    	t = to_lin;
	to_lin = from_lin;
	from_lin = t;
    	t = to_col;
	to_col = from_col;
	from_col = t;
    }
    
    from_lin -= display_topline;	/* convert to screen coords */
    if (from_lin < 0) {
	from_lin = 0;
	from_col = 0;
    }
    
    if (from_lin >= screensize)		/* if down out of sight, forget it */
	return;
	
    to_lin -= display_topline;		/* convert to screen coords */
    
    if (to_lin < 0)			/* if up out of sight, forget it */
	return;
	
    if (to_lin >= screensize) {
	to_lin = screensize-1;
	to_col = MAXCOL;
    }

    if (from_lin == to_lin) {	/* if only one line */
	if (from_col > to_col) {
    	    t = to_col;
	    to_col = from_col;
	    from_col = t;
	}
	if (from_col != to_col)	/* then invert the characters in between */
	    invertchars(from_lin, from_col, 1, to_col - from_col);
    } else {
	if (from_col < MAXCOL)
	    invertchars(from_lin, from_col, 1, MAXCOL - from_col);
	t = to_lin - from_lin - 1;
	if (t > 0)
	    invertchars(from_lin+1, 0, t, MAXCOL);
	if (to_col > 0)
	    invertchars(to_lin, 0, 1, to_col);
    }
}

static int
typeof_char(c)
unsigned char c;
{
    if ((c == ' ') || (c == '\240'))
    	return (0);			/* whitespace char */

    if (((c >= '0') && (c <= '9'))
    	|| ((c >= 'A') && (c <= 'Z'))
    	|| ((c >= 'a') && (c <= 'z'))
    	|| ((c >= '\300') && (c <= '\377') && (c != '\327') && (c != '\367'))
    	|| ((c >= '\271') && (c <= '\276') && (c != '\273'))
    	|| (c == '\262') || (c == '\263') || (c == '\252'))
	return (1);			/* alpha-numeric char */

    return (2);				/* printing, non-alphanum char */
}


static int
all_spaces(register int r, register int c)
{
    register int i;
    
    for (i = c; i < MAXCOL; i++)
	if ((scr[r][i] != ' ')
	    || (scr_attrs[r][i] != 0))
	    return (0);			/* found a non-space */

    return (1);				/* everything was spaces */
}


static void
point_to_mouse_low_high (Point *MousePt_p,
			 int n_clicks,
			 int *mouselin_p,
			 int *mousecol_p,
			 int *mousecol_lp,
			 int *mousecol_hp)
{
    int real_lin, real_col;
    int ch_type, ch_attr;
    int i;
    
    real_lin = (MousePt_p->v - TOPMARGIN) / lineheight + display_topline;
    if (real_lin < display_topline)
	real_lin = display_topline;
    if (real_lin >= display_topline + screensize)
	real_lin = display_topline + screensize-1;

    *mouselin_p = real_lin;
    
    real_col = (MousePt_p->h - LEFTMARGIN + charwidth/2) / charwidth;
    if (real_col < 0) real_col = 0;
    if (real_col > MAXCOL) real_col = MAXCOL;

    /*
     * We spoof things a bit here -- if the rest of the line is all blanks,
     * then we treat it as a single character (CRLF, really), and pretend
     * that the user clicked the mouse in the first blank character after all
     * text.
     */
    if (all_spaces (real_lin, real_col))
    {
    	for (i = real_col; i >= 0; i--)
	    if ((scr[real_lin][i] != ' ')
	        || (scr_attrs[real_lin][i] != 0))
		break;
	real_col = i+1;
	if (real_col < 0) real_col = 0;
	if (real_col > MAXCOL) real_col = MAXCOL;
    }
    
    *mousecol_p = real_col;

    if (n_clicks == 0)		/* if a SINGLE click */
    {
    	*mousecol_lp = real_col;
	*mousecol_hp = real_col;
    }
    else if (n_clicks == 1)	/* if a DOUBLE click */
    {
	ch_type = typeof_char (scr[real_lin][real_col]);
	ch_attr = scr_attrs[real_lin][real_col];
	for (i = real_col-1; i >= 0; i--)
	    if ((typeof_char (scr[real_lin][i]) != ch_type)	/* if a different type */
	        || (scr_attrs[real_lin][i] != ch_attr))		/* or colored different */
		break;				/* then it isn't the same kind of char */
	*mousecol_lp = i+1;
	for (i = real_col+1; i < MAXCOL; i++)
	    if ((typeof_char (scr[real_lin][i]) != ch_type)	/* if a different type */
	        || (scr_attrs[real_lin][i] != ch_attr))		/* or colored different */
		break;				/* then it isn't the same kind of char */
	*mousecol_hp = i;
    }
    else				/* a TRIPLE click */
    {
    	*mousecol_lp = 0;
	*mousecol_hp = MAXCOL;
    }
}


static void
mouse_region_select (evt)
EventRecord *evt;
{
    int mouselin;
    int mousecol_l, mousecol_h, real_mousecol;
    int i, shift, sval, smax;
    int old_from_lin, old_from_col, old_to_lin, old_to_col;
    Point MousePt;
    static Point prev_mouse_point = {0, 0};	/* used for double-click determination */
    static long prev_mouse_up = 0L;
    static int n_clicks_here = 0;

    MousePt = evt->where;

    /* if no selection, then a shift drag is just a drag */
    if (have_selection)
	shift = (evt->modifiers) & shiftKey;
    else
	shift = 0;

    if (((evt->when - prev_mouse_up) <= GetDblTime())
    	&& (ABS(MousePt.v - prev_mouse_point.v) < lineheight)
	&& (ABS(MousePt.h - prev_mouse_point.h) < charwidth))
	n_clicks_here = (n_clicks_here + 1) % 3;
    else
    	n_clicks_here = 0;		/* just one click */

    prev_mouse_point = MousePt;			/* save for next time */
    prev_mouse_up = TickCount ();		/* save when the mouse went up */

    /* if not adding to region, remove old one */
    if (!shift && have_selection)
    	invert_text(from_lin, from_col, to_lin, to_col);


    point_to_mouse_low_high (&MousePt, n_clicks_here, &mouselin,
    			     &real_mousecol, &mousecol_l, &mousecol_h);

    if (shift) {
	/*
	 * Swap from_* and to_* if closer to from.  This sets the further-away
	 * side as the anchor, and the closer one as the part we are changing.
	 */
	if (ABS((MAXCOL * from_lin + from_col) - (MAXCOL * mouselin + real_mousecol)) <
	    ABS((MAXCOL * to_lin + to_col) - (MAXCOL * mouselin + real_mousecol))) {
    	    i = to_lin;
	    to_lin = from_lin;
	    from_lin = i;
    	    i = to_col;
	    to_col = from_col;
	    from_col = i;
	}
    } else {
	from_lin = mouselin;
	from_col = mousecol_l;
	to_lin = mouselin;
	to_col = mousecol_h;
	
	/* Select the text if a double or triple click. */
	if (from_col != to_col)
    	    invert_text(from_lin, from_col, to_lin, to_col);
    }
    /* save in case we have to swap which point is the anchor */
    old_from_lin = from_lin;
    old_from_col = from_col;
    old_to_lin = to_lin;
    old_to_col = to_col;
    
    while (StillDown()) {
	GetMouse(&MousePt);
	point_to_mouse_low_high (&MousePt, n_clicks_here, &mouselin,
    				 &real_mousecol, &mousecol_l, &mousecol_h);

	/*
	 * If above of below screen, auto-scroll the slider and select more.
	 */
	
	if (mouselin < display_topline) {
	    sval = GetCtlValue (t_vscroll) - 1;
	    smax = GetCtlMax (t_vscroll);
	    if ((sval >= 0) && (sval <= smax)) {
		SetCtlValue (t_vscroll, sval);
		scroll_term ();
	    }
	    mouselin = display_topline;
	    real_mousecol = 0;

	} else if (mouselin >= display_topline + screensize) {
	    sval = GetCtlValue (t_vscroll) + 1;
	    smax = GetCtlMax (t_vscroll);
	    if ((sval >= 0) && (sval <= smax)) {
		SetCtlValue (t_vscroll, sval);
		scroll_term ();
	    }
	    mouselin = display_topline + screensize-1;
	    real_mousecol = MAXCOL;
	}
	
	/*
	 * If we are above the anchor, then the "interesting" side of the click extent
	 * is mousecol_l, else it is mousecol_h.
	 */
	if ((MAXCOL * from_lin + from_col) > (MAXCOL * mouselin + real_mousecol))
	{
	    i = mousecol_l;		/* "above" the anchor point */

	    /* but if we were below the anchor, restore and swap */
	    if ((MAXCOL * from_lin + from_col) < (MAXCOL * to_lin + to_col))
	    {
		/* Unselect current text. */
 		invert_text(from_lin, from_col, to_lin, to_col);

		from_lin = old_to_lin;
		from_col = old_to_col;
		to_lin = old_from_lin;
		to_col = old_from_col;
		
		/* Reselect new (old) text. */
 		invert_text(from_lin, from_col, to_lin, to_col);
	    }
	} else {
	    i = mousecol_h;		/* "below" the anchor point */

	    /* but if we were above the anchor, restore and swap */
	    if ((MAXCOL * from_lin + from_col) > (MAXCOL * to_lin + to_col))
	    {
		/* Unselect current text. */
 		invert_text(from_lin, from_col, to_lin, to_col);

		from_lin = old_from_lin;
		from_col = old_from_col;
		to_lin = old_to_lin;
		to_col = old_to_col;
		
		/* Reselect new (old) text. */
 		invert_text(from_lin, from_col, to_lin, to_col);
	    }
	}
	    
	
	/*
	 * If any new text was selected, invert it.
	 */
	if ((i != to_col) || (mouselin != to_lin)) {
	    invert_text(to_lin, to_col, mouselin, i);
	    to_lin = mouselin;
	    to_col = i;
	}
    }
    
    /* make from < to */	
    if ((MAXCOL * from_lin + from_col) > (MAXCOL * to_lin + to_col)) {
    	i = to_lin;
	to_lin = from_lin;
	from_lin = i;
    	i = to_col;
	to_col = from_col;
	from_col = i;
    }
    
    if ((from_lin != to_lin) || (from_col != to_col))
	have_selection = TRUE;
    else
	have_selection = FALSE;
	
    /*
     * If the mouse wasn't down long enough to be a drag, time double click from
     * the mouse UP.
     */
    if (((TickCount () - prev_mouse_up) <= GetDblTime())
    	&& (ABS(MousePt.v - prev_mouse_point.v) < lineheight)
	&& (ABS(MousePt.h - prev_mouse_point.h) < charwidth)) {
	prev_mouse_up = TickCount ();		/* save when the mouse went up */
    }
}

/* (PWP) if the selection is within [tlin,blin], then remove it */

void
maybe_nuke_selection(int tlin, int blin)
{
    int my_to_lin;

    if (!have_selection)
	return;

    my_to_lin = to_lin;
    if ((to_col == 0) && (from_lin != to_lin))
    	my_to_lin--;

    if (!(((tlin < from_lin) && (blin < from_lin))
	  || ((tlin > my_to_lin) && (blin > my_to_lin))) ) {
	have_selection = FALSE;
	invert_text(from_lin, from_col, to_lin, to_col);
    }
}

/*
 * Copy the current selction to the (internal) clipboard.
 *
 * This is an external, but we don't have to save the GrafPort
 * because we don't do anything to the screen.
 */
scr_copy()
{
    int lin, i, rcol;
    long sz;
    char *dp;
    ScrapStuff *pss;
    
    if (myclip_h == NIL) {
	printerr("scr_copy: clip handle not allocated", 0);
	return;
    }
    
    if (have_selection) {
	/****** find out how big the text to copy is ******/
    	if (from_lin == to_lin) {
	    /*
	     * If we are copying to the end of line, we should really only copy
	     * a CR instead of all those trailing blanks.  So we must find out
	     * where the last real character is.
	     */
	    if (to_col >= MAXCOL) {
		for (rcol = MAXCOL; rcol > 0; rcol--)		/* last */
		    if ((scr[to_lin][rcol-1] != ' ')
			|| (scr_attrs[to_lin][rcol-1] != 0))
			break;
	    } else {
	    	rcol = to_col;
	    }
	    sz = rcol - from_col + 1;
	} else {
	    for (rcol = MAXCOL; rcol > from_col; rcol--)	/* first */
		if ((scr[from_lin][rcol-1] != ' ')
		    || (scr_attrs[from_lin][rcol-1] != 0))
		    break;
	    sz = rcol - from_col + 1;	/* chars plus one for the newline */
	    for (lin = from_lin+1; lin < to_lin; lin++) {	/* in between */
		for (rcol = MAXCOL; rcol > 0; rcol--)
		    if ((scr[lin][rcol-1] != ' ')
			|| (scr_attrs[lin][rcol-1] != 0))
			break;
		sz += rcol + 1;	/* chars plus one for the newline */
	    }
	    if (to_col >= MAXCOL) {
	    	/***** find the last real character *****/
		for (rcol = MAXCOL; rcol > 0; rcol--)		/* last */
		    if ((scr[to_lin][rcol-1] != ' ')
			|| (scr_attrs[to_lin][rcol-1] != 0))
			break;
	    } else {
		rcol = to_col;
	    }
	    sz += rcol;		/* chars */
	    if (to_col >= MAXCOL)
	    	sz++;
	}
	
	/***** Reality Check *****/
	if (sz > 8192) {
	    printerr("Too big to copy: ", sz);
	    return;
	}
	    
	/****** allocate and lock a buffer for the text ******/
	if (sz > GetHandleSize ((Handle) myclip_h)) {
	    HUnlock((Handle) myclip_h);
	    /* $$$ this may fail, but we have no way of knowing. */
	    /* (in assembler, this will return a result, but the pascal */
	    /*  version is a PROCEDURE, so we get zilch.  If this fails */
	    /*  we will probably crash the Mac...) */
	    SetHandleSize((Handle) myclip_h, sz);
	}
	HLock((Handle) myclip_h);
	dp = *myclip_h;
	
	/****** copy the characters over to the clip ******/
    	if (from_lin == to_lin) {
	    if (to_col >= MAXCOL) {
		for (rcol = MAXCOL; rcol > 0; rcol--)		/* last */
		    if ((scr[to_lin][rcol-1] != ' ')
			|| (scr_attrs[to_lin][rcol-1] != 0))
			break;
	    } else {
	    	rcol = to_col;
	    }
	    for (i = from_col; i < rcol; i++)
	    	*dp++ = scr[from_lin][i];
	    if (to_col >= MAXCOL)
	    	*dp++ = CR;		/* add the return */
	} else {
	    /* trim off spaces */
	    for (rcol = MAXCOL; rcol > from_col; rcol--)	/* first */
		if ((scr[from_lin][rcol-1] != ' ')
		    || (scr_attrs[from_lin][rcol-1] != 0))
		    break;
	    for (i = from_col; i < rcol; i++)
		*dp++ = scr[from_lin][i];
	    *dp++ = CR;
	    for (lin = from_lin+1; lin < to_lin; lin++) {	/* in between */
		for (rcol = MAXCOL; rcol > 0; rcol--)
		    if ((scr[lin][rcol-1] != ' ')
			|| (scr_attrs[lin][rcol-1] != 0))
			break;
		for (i = 0; i < rcol; i++)
		    *dp++ = scr[lin][i];
		*dp++ = CR;
	    }
	    if (to_col == MAXCOL) {
		for (rcol = MAXCOL; rcol > 0; rcol--)		/* last */
		    if ((scr[to_lin][rcol-1] != ' ')
			|| (scr_attrs[to_lin][rcol-1] != 0))
			break;
	    } else {
		rcol = to_col;
	    }
	    for (i = 0; i < rcol; i++)
		*dp++ = scr[to_lin][i];
	    if (to_col >= MAXCOL)
		*dp++ = CR;
	}
	myclip_size = (dp - *myclip_h);

	/****** check to make sure we didn't overflow the clipboard ******/
	if (myclip_size > sz)
	    fatal ("Overflow! myclip_size - sz ==",
	    	myclip_size - sz);


	/****** Now copy our internal clipboard to the Macintosh one *****/
	/*
	 * $$$ at this point we really should allocate a second buffer,
	 * and copy the characters of our buffer into it, converting them
	 * from whatever ISO set we are displaying with to the Mac char set.
	 */
	ZeroScrap();
	if (PutScrap(myclip_size, 'TEXT', *myclip_h) != noErr)
		printerr("Couldn't PutScrap", 0);		
	
	pss = InfoScrap();
	my_scrapcount = pss->scrapCount;	/* save this to see if the user */
						/* cuts/copies outside of us */
	
	/****** We are done.  Unlock the handle ******/
	HUnlock((Handle) myclip_h);
    } else {
	SysBeep(3);
    }
}

/*
 * Paste the clipboard into the terminal, by "typing" it in.
 *
 * This also is an external, but we don't have to save the GrafPort
 * because the only time we do anything to the screen, it's through
 * inpchars(), which saves the GrafPort itself.
 */
scr_paste()
{
    char *cp, *endp;
    char **h;
    long l, o;
    ScrapStuff *pss;

    pss = InfoScrap();
    if (my_scrapcount == pss->scrapCount) {
    	/* if this is still the same scrap that we made */
	if (myclip_size > 0) {
	    HLock((Handle) myclip_h);
	    cp = *myclip_h;
	    endp = cp + myclip_size;
	    for (; cp < endp; cp++) {
		OutputChar(*cp);
		if (*cp == CR)
		    waitasec ();
	    }
	    HUnlock((Handle) myclip_h);
	} else {
	    SysBeep(3);
	}
    } else {	/* we have to get the TEXT scrap from the clipboard */
	h = NewHandle(0);
	l = GetScrap(h, 'TEXT', &o);
	if (l <= 0) {
	    SysBeep(3);
	} else {
	    HLock((Handle) h);
	    cp = *h;
	    endp = cp + l;
	    for (; cp < endp; cp++) {
	    	/* $$$ Should convert *cp to whatever ISO font we are typing */
		OutputChar(*cp);
		if (*cp == CR)
		    waitasec ();
	    }
	    HUnlock((Handle) h);
	}	/* end if (l > 0) */
	DisposHandle(h);
    }
}


/****************************************************************************/
/****************************************************************************/

#ifdef COMMENT
show_inval_rgn(w)
WindowPeek w;
{
    RgnHandle r = NewRgn();

    CopyRgn (w->updateRgn, r);
    OffsetRgn(r,			/* convert to local grafport coords */
	      (((w->port).portBits).bounds).left,
	      (((w->port).portBits).bounds).top);
    FillRgn(r, qd.black);
    DisposeRgn(r);
}
#endif /* COMMENT */

/****************************************************************************/
/*
 * PWP: actually do all the scrolling and refreshing we have promised to
 * do.
 *
 * Method (and many var and fcn names) stolen from X11 xterm.
 */
/****************************************************************************/
void
flushscroll()
{
    register int i, now;
    Rect r, opened_r;		/* cannot be register */
    GrafPtr currWindow;		/* cannot be register */
    RgnHandle newupdateRgn;
    
    if (scroll_amount == 0) {
    	printerr ("flushscroll() called with no scroll to flush", 0);
    	return;
    }
    
    /* printerr("flushscroll(), scroll_amount == ", scroll_amount); */
    
    /* sould hide the cursor here if not already hidden */
    
    /* (PWP) if our selected region overlaps, but is not enclosed by the region
       we want to scroll, then remove it, because the region no longer contains
       what the user thought it did. */
    if (have_selection && (saved_tlin != toplin) && (saved_blin != botlin) &&
	((from_lin < saved_tlin) || (to_lin > saved_blin)) &&
    	((to_lin > saved_tlin)   || (from_lin < saved_blin))) {
	have_selection = FALSE;
	invert_text(from_lin, from_col, to_lin, to_col);
    }
    
    if (!in_front) {
	/* if not in front, compensate update region for scrolling */
	GetPort (&currWindow);
	/* scroll the old updateRgn */
	OffsetRgn (((WindowPeek) currWindow)->updateRgn, 0, -lineheight);
    }
    

    /* 
     * Do the scrolling
     */
    makerect (&r, saved_tlin, 0, saved_blin - saved_tlin + 1, MAXCOL);
    newupdateRgn = NewRgn();
    
    if (smoothscroll && in_front) {
	for (i = 1; i <= scroll_amount * lineheight; i += 1) {
	    /* PWP: wait for a vertical reblank (in a sneaky way) */
	    now = TickCount ();
	    while (TickCount () == now)
		/* wait... */ ;
	    if (scroll_amount < 0)
		ScrollRect (&r, 0, -1, newupdateRgn);
	    else
		ScrollRect (&r, 0, 1, newupdateRgn);
	}
    } else {
	ScrollRect (&r, 0, scroll_amount * lineheight, newupdateRgn);
    }

    if (!in_front) {
	if (scroll_amount < 0)
	    makerect (&opened_r, saved_blin + scroll_amount,
		      0, -scroll_amount, MAXCOL);
	else
	    makerect (&opened_r, saved_tlin, 0, scroll_amount, MAXCOL);
	InvalRgn(newupdateRgn);
	ValidRect(&opened_r);
    }

    DisposeRgn(newupdateRgn);

    /*
     * Now refresh any lines that need to be drawn
     */
    if (refresh_amount < 0)		/* scrolling UP */
    {
	for (i = saved_blin + refresh_amount; i < saved_blin; i++)
	    draw_w_line_attrs(i+display_topline, i, 0, MAXCOL, 0);
    }
    else if (refresh_amount > 0)	/* scrolling DOWN */
    {
	for (i = saved_tlin; i < saved_tlin + refresh_amount; i++)
	    draw_w_line_attrs(i+display_topline, i, 0, MAXCOL, 0);
    }

    scroll_amount = 0;		/* we've done it now */
    refresh_amount = 0;
}


/****************************************************************************/
/*
 * (UoR)
 *
 * Scroll lines within the scroll region upwards from line tlin
 * to line blin (lines are assumed to be in the region)
 *
 * (PWP) scroll_screen is the combination of scroll_up and scroll_down.
 *       dir is the number of lines to scroll, <0 if up, >0 if down.
 *	 (actually, right now only -1 and 1 are handled.)
 */
/****************************************************************************/
void
scroll_screen (tlin, blin, delta)		/* these are in scr[][] cordinates */
register int tlin;
register int blin;
register int delta;
{
    register int i, now;
    char *savedline, *savedattr;  /* temporary to hold screen line pointer */
    Rect r, opened_r;		/* cannot be register */
    GrafPtr currWindow;		/* cannot be register */
    RgnHandle newupdateRgn;

    /*
     * flush out any pending characters.
     */
    if (out_maxcol) flushbuf();
    
    /*
     * See if we are scrolling something different and have to flush
     * our pending scroll.
     * We do if asked to scroll something different than the curr scrolling rgn,
     * or if we are changing direction, or if we have allready collected an
     * entire scrolling region worth of scroll to do.
     */
    if (scroll_amount) {
	i = scroll_amount + delta;
	if (i < 0) i = -i;	/* set i to ABS( old scroll plus new scroll ) */
	if (smoothscroll || (tlin != saved_tlin) || (blin != saved_blin)
	    || ((scroll_amount > 0) && (delta < 0))
	    || ((scroll_amount < 0) && (delta > 0))
	    || (i >= (saved_blin - saved_tlin)))
	{
	    flushscroll();
	}
    }

    /*
     * Save up how much to scroll and where for later...
     */
    saved_tlin = tlin;
    saved_blin = blin;
    scroll_amount += delta;
    /*
     * Should really set refresh_amount to 0, then add to it when we "draw" characters
     * onto lines, but this is safe.
     */
    refresh_amount += delta;
    
    /* printerr("scroll_amount now ", scroll_amount); */

    if (delta < 0)		/* if scrolling UP (forwards) */
    {
	/* adjust the internal character buffers */
	if ((tlin == toplin) && (blin == botlin)) {	/* if whole screen */
	    display_totlines -= delta;		/* remember that delta is negitive */
	    if (display_totlines > MAX_SCREENSIZE)
		display_totlines = MAX_SCREENSIZE;	/* bounds */
    	    tlin = screensize - display_totlines;	/* top of saved buffer */
	}
	for (now = 0; now < -delta; now++) {
 	    savedline = scr[tlin];
	    savedattr = scr_attrs[tlin];
	    for (i = tlin+1; i <= blin; i++) {
    		scr[i-1] = scr[i];
    		scr_attrs[i-1] = scr_attrs[i];
	    }
	    scr[blin] = savedline;
	    scr_attrs[blin] = savedattr;
    
	    zeroline (blin);		/* clear the line */
	}

	/* adjust selection */
	if (have_selection && (from_lin >= tlin) && (to_lin <= blin)) {
	    from_lin += delta;
	    if (from_lin < screensize - MAX_SCREENSIZE)
	    	from_lin = screensize - MAX_SCREENSIZE;
	    to_lin += delta;
	    if (to_lin < screensize - MAX_SCREENSIZE)
	    	to_lin = screensize - MAX_SCREENSIZE;
	}
    }
    else			/* else scrolling DOWN (reverse scroll) */
    {
        /* adjust the internal buffers */
	for (now = 0; now < delta; now++) {
	    savedline = scr[blin];
	    savedattr = scr_attrs[blin];
	    for (i = blin-1; i >= tlin; i--) {
    		scr[i+1] = scr[i];
    		scr_attrs[i+1] = scr_attrs[i];
	    }
	    scr[tlin] = savedline;
	    scr_attrs[tlin] = savedattr;

	    zeroline (tlin);
	}
	
	/* adjust selection */
	if (have_selection && (from_lin >= tlin) && (to_lin <= blin)) {
	    from_lin += delta;
	    if (from_lin > botlin) from_lin = botlin;
	    to_lin += delta;
	    if (to_lin > botlin) to_lin = botlin;
	}
    }

    /*
     * but if we are smooth (slow) scrolling and in front, do the scroll now.
     * must do this after adjusting internal buffers, so that the refresh
     * lines don't get confused.
     */
    if (smoothscroll && in_front)
    	flushscroll();

}				/* scroll_up */

/****************************************************************************/
/* redraw the terminal screen (we got a redraw event) */
/****************************************************************************/
term_redraw ()
{
    int i, lin;
    int vtoplin, vbotlin, vleftcol, vrightcol;
    Rect r, *rp;
    GrafPtr savePort;
    
    GetPort (&savePort);	/* there just has to be a better way */
    SetPort (terminalWindow);

    if (screeninvert) {
	BackPat(qd.black);
	PenPat(qd.white);
    } else {
	BackPat(qd.white);
	PenPat(qd.black);
    }

#ifdef COMMENT
    r = terminalWindow->portRect;	/* copy the window size */
    /* r.right -= 15;	*/		/* subtract control */
    /* makerect (&r, 0, 0, screensize, MAXCOL); */   /* PWP: clear the screen first */
    /* EraseRect (&r); */
#endif /* COMMENT */

    /* See if the scroll bar and grow box need to be updated */
    r = terminalWindow->portRect;	/* copy of the window size */
    /* r.left = r.right - 16; */
    r.left = rightMARGIN;
    if (RectInRgn (&r, terminalWindow->visRgn))
    {
	EraseRect (&r);
	draw_grow_and_erase_line(0);	/* don't add to invalid while redrawing invalid */
	DrawControls (terminalWindow);
    }

    /* Update area above first line */
    r = terminalWindow->portRect;	/* copy of the window size */
    r.bottom = r.top + TOPMARGIN;
    r.right -= 16;
    if (RectInRgn (&r, terminalWindow->visRgn))
	EraseRect (&r);

    /* Update area below last line */
    r = terminalWindow->portRect;	/* copy of the window size */
    r.top = bottommargin - 1;
    r.right -= 16;
    if (RectInRgn (&r, terminalWindow->visRgn))
	EraseRect (&r);

    /* Update area to the left of first column */
    r = terminalWindow->portRect;	/* copy of the window size */
    r.top = r.top + TOPMARGIN;
    r.bottom = bottommargin;
    r.right = LEFTMARGIN;
    if (RectInRgn (&r, terminalWindow->visRgn))
	EraseRect (&r);

    
    /* update_vscroll(); */
    /* SetCtlValue (t_vscroll, GetCtlValue (t_vscroll)); */

#ifdef COMMENT
    lin = display_topline;
    for (i = 0; i < screensize; i++) {
    	makerect (&r, i, 0, i+1, MAXCOL);
	if (RectInRgn (&r, terminalWindow->visRgn))
	    draw_w_line_attrs(lin, i, 0, MAXCOL, 1);
	lin++;
    }
#endif

    /* This is an inverse of makerect() -- find the bounding chars */
    rp = &(**(terminalWindow->visRgn)).rgnBBox;
    vtoplin = (rp->top - TOPMARGIN) / lineheight;
    vbotlin = (rp->bottom - TOPMARGIN + lineheight -1) / lineheight;
    vleftcol = (rp->left - LEFTMARGIN) / charwidth;
    vrightcol = (rp->right - LEFTMARGIN + charwidth -1) / charwidth;

    /* bounds limit it to the actual screen area */
    if (vtoplin < 0) vtoplin = 0;
    if (vbotlin > screensize) vbotlin = screensize;
    if (vleftcol < 0) vleftcol = 0;
    if (vrightcol > MAXCOL) vrightcol = MAXCOL;

#ifdef COMMENT
    debug(F101,"term_redraw bounds vtoplin","",vtoplin);
    debug(F101,"term_redraw bounds vbotlin","",vbotlin);
    debug(F101,"term_redraw bounds vleftcol","",vleftcol);
    debug(F101,"term_redraw bounds vrightcol","",vrightcol);
#endif

    lin = display_topline + vtoplin;
    for (i = vtoplin; i < vbotlin; i++) {
	draw_w_line_attrs(lin, i, vleftcol, vrightcol, 1);
	lin++;
    }
    
    
    if (have_selection)
    	invert_text(from_lin, from_col, to_lin, to_col);

    if (cur_drawn && cursor_invert) {	/* (UoR) only if cursor is showing */
	cursor_invert = FALSE;	/* (UoR) make sure we draw it */
	cursor_draw ();		/* redraw cursor */
	last_flash = TickCount ();	/* (UoR) reset timer */
    }

    SetPort (savePort);		/* there just has to be a better way */
}				/* term_redraw */

draw_grow_and_erase_line(invalidate_it)
int invalidate_it;
{
    Rect r;

    DrawGrowIcon (terminalWindow);
    /* erase the bottom scroll line (but only if inverted screen) */
    if (!screeninvert) {
	PenMode(patBic);
	MoveTo(0, (terminalWindow->portRect).bottom - 15);
	LineTo((terminalWindow->portRect).right - 15,
    		(terminalWindow->portRect).bottom - 15);
	PenMode(patCopy);
    }
    
    if (invalidate_it) {
	r.top = (terminalWindow->portRect).bottom - 16;
	r.bottom = (terminalWindow->portRect).bottom - 14;
	r.left = 0;
	r.right = (terminalWindow->portRect).right - 16;
	InvalRect(&r);
    }
}

term_activate(mod)
int mod;
{
    GrafPtr savePort;
    
    GetPort (&savePort);	/* there just has to be a better way */
    SetPort (terminalWindow);

    cursor_erase ();		/* remove cursor from screen */
    in_front = mod & activeFlag;
    if (in_front) {
	HiliteControl (t_vscroll, 0);
	UpdateOptKey(1);
	DisableItem(menus[EDIT_MENU], UNDO_EDIT);
	DisableItem(menus[EDIT_MENU], CLEAR_EDIT);
    } else {
	HiliteControl (t_vscroll, 255);
	UpdateOptKey(0);
	EnableItem(menus[EDIT_MENU], UNDO_EDIT);
	EnableItem(menus[EDIT_MENU], CLEAR_EDIT);
    }
    draw_grow_and_erase_line(1);	/* this does the right thing for background too */
    cursor_draw ();

    SetPort (savePort);		/* there just has to be a better way */
}

/*
 * This CAN be called external to inpchars(), so save and restore the
 * GrafPort just in case.
 */
set_term_invert(new_inv)
int new_inv;
{
    GrafPtr savePort;
    
    GetPort (&savePort);	/* there just has to be a better way */
    SetPort (terminalWindow);

    if (new_inv == screeninvert)
    	return;
	
    if (new_inv) {
	BackPat (qd.black);	/* (UoR) use black background */
	PenPat(qd.white);
	screeninvert = TRUE;
    } else {
	BackPat (qd.white);
	PenPat(qd.black);
	screeninvert = FALSE;
    }
    InvalRect (&terminalWindow->portRect);	/* invalidate whole window rectangle */
    SetPort (savePort);		/* there just has to be a better way */
}

/****************************************************************************/
/* sizevscroll - called when window is created and after a window grow */
/*    	      	    sequence to resize the scroll window's bars. */
/****************************************************************************/
static void
sizevscroll ()
{
    register Rect *r;

    r = &terminalWindow->portRect;/* window size */
    HideControl (t_vscroll);

    MoveControl (t_vscroll, r->right - 15, r->top - 1);
    SizeControl (t_vscroll, 16, r->bottom - r->top - 13);

    SetCtlMin (t_vscroll, 0);
    update_vscroll();
    ShowControl (t_vscroll);
}				/* sizescrollbars */

/****************************************************************************/
/* initalize the terminal emulator. */
/****************************************************************************/
init_term ()
{
    register int i, j;
    register char *scp, *acp;
    char *scr_cp, *attr_cp;
    GrafPtr savePort;
    
    GetPort (&savePort);	/* there just has to be a better way */
    SetPort (terminalWindow);
   
    topmargin = TOPMARGIN;	/* Edges of adjustable window */
    bottommargin = bottomMARGIN;
    
    if ((scr_cp = (char *)NewPtr(((long)(MAXCOL+1) * (long) MAX_SCREENSIZE))) == NIL)
	fatal("Could not allocate screen buffer", 0);
	
    if ((attr_cp = (char *)NewPtr(((long)(MAXCOL+1) * (long) MAX_SCREENSIZE))) == NIL)
	fatal("Could not allocate screen attribute buffer", 0);
	
    if ((real_scr = (ucharptr *) NewPtr ((long)(MAX_SCREENSIZE)
					 * (long) sizeof(ucharptr))) == NIL)
	fatal("Could not allocate screen buffer", 0);
	
    if ((real_attrs = (ucharptr *) NewPtr ((long)(MAX_SCREENSIZE)
					   * (long) sizeof(ucharptr))) == NIL)
	fatal("Could not allocate screen buffer", 0);
	
    for (i = 0; i < MAX_SCREENSIZE; i++) {
    	real_scr[i] = scr_cp + (i * (MAXCOL+1));	/* divvy up screen buffer */
    	real_attrs[i] = attr_cp + (i * (MAXCOL+1));	/* divvy up screen attribute buf */

	scp = real_scr[i];
	acp = real_attrs[i];
	j = MAXCOL;
	do {			/* put normal spaces in all columns */
	    *scp++ = ' ';
	    *acp++ = 0;
	} while (--j > 0);
	*scp = ' ';		/* Terminate the lines as strings */
	*acp = 0;		/* Terminate the attrs as strings */
    }
    
    scr = &real_scr[MAX_SCREENSIZE - screensize];
    if (scr[0] == NIL)
	fatal("init_term: scr assignment botched for [0]", 0);
    if (scr[screensize-1] == NIL)
	fatal("init_term: scr assignment botched for [screensize-1]", 0);

    scr_attrs = &real_attrs[MAX_SCREENSIZE - screensize];
    if (scr_attrs[0] == NIL)
	fatal("init_term: scr assignment botched for [0]", 0);
    if (scr_attrs[screensize-1] == NIL)
	fatal("init_term: scr assignment botched for [screensize-1]", 0);
    
    scrtop = toplin;		/* Scrolling region equals all */
    scrbot = botlin;
    
    scroll_amount = 0;		/* no pending scroll */
    refresh_amount = 0;		/* no pending refresh */
    saved_tlin = 0;
    saved_blin = 0;
    
    display_topline = toplin;	/* init display w/elevator at bottom */
    display_totlines = screensize;
    makerect (&ScreenRect, 0, 0, screensize, MAXCOL);
    /* (UoR) full screen rectangle */
    
    SizeWindow(terminalWindow,
    	rightMARGIN + 1 + 16,     /* add extra to side for asthetics */
	bottomMARGIN + TOPMARGIN,     /* add extra to bottom for asthetics */
	FALSE);
    /* PWP: make the window match it's real size */

    t_vscroll = GetNewControl (RCMDVSCROLL, terminalWindow);
    sizevscroll();

    InitKeyStuff();		/* find the original KCHR keymaps */

#ifdef COMMENT
    draw_grow_and_erase_line(0);	/* it's new so don't invalidate it */
#endif

    /* ClipRect(&ScreenRect); */

    SetPort (savePort);		/* there just has to be a better way */
}				/* init_term */

/****************************************************************************/
/* grow_term_to(size) -- change the size of the terminal window to size.
   this is called by growterm() (see below) and the terminal settings dialog
   handler (termsetdialog()).
/****************************************************************************/
grow_term_to (size)
int size;
{
    char *savedline;
    int i, j;
    GrafPtr savePort;

    GetPort (&savePort);
    SetPort (terminalWindow);

    if ((size < 1) || (size > MAX_SCREENSIZE))
       size = 24;	/* the default case */
    
    if (size > display_totlines) {	/* if getting bigger than we were */
	/*
	 * We would zero out lines from (screensize-size) to (screensize-display_totlines),
	 * but these were already zeroed when the original screen was inited.
	 */
	display_totlines = size;
    }
    
    /* $$$ Make sure to scroll screen to what will be the new bottom here */

    curlin += size - screensize;	/* adjust cursor row to match stretch */
    if (curlin < 0)
	curlin = 0;
    if (curlin > size-1)
	curlin = size-1;
    
    screensize = size;
    if (screensize > MAX_SCREENSIZE)
    	screensize = MAX_SCREENSIZE;		/* bounds check */

    scr = &real_scr[MAX_SCREENSIZE - screensize];
    scr_attrs = &real_attrs[MAX_SCREENSIZE - screensize];
    
    bottommargin = bottomMARGIN;	/* this changes */
    
    scrtop = toplin;		/* Scrolling region equals all */
    scrbot = botlin;
    display_topline = 0;	/* re-init display w/elevator at bottom */
    makerect (&ScreenRect, 0, 0, screensize, MAXCOL);
    /* (UoR) full screen rectangle */
    
    SizeWindow(terminalWindow,
    	rightMARGIN + 1 + 16,     /* add extra to side for asthetics */
	bottomMARGIN + TOPMARGIN,     /* add extra to bottom for asthetics */
	FALSE);
    /* PWP: make the window match it's real size */
    sizevscroll ();	/* size the scroll bars */

    /* ClipRect(&ScreenRect); */

    InvalRect (&terminalWindow->portRect);	/* invalidate whole window rectangle */

    SetPort (savePort);
}				/* grow_term_to */

/****************************************************************************/
/* 
 * growterm() -- called when we get a mouse-down in the lower right corner grow
 * box.
 * Probably all right not to save the grafport, but we do anyway just to be
 * double extra safe.
 */
/****************************************************************************/
growterm (p)
Point *p;
{
    long gr;
    int height;
    int width;
    int size;
    Rect growRect;
    GrafPtr savePort;
    
    GetPort (&savePort);	/* there just has to be a better way */
    SetPort (terminalWindow);

    growRect = qd.screenBits.bounds;
    growRect.top = 50;		/* minimal horizontal size */
    growRect.left = rightMARGIN + 18;	/* minimal vertical size */
    growRect.right = rightMARGIN + 18;	/* minimal vertical size */

    gr = GrowWindow (terminalWindow, *p, &growRect);

    if (gr == 0)
	return;
    height = HiWord (gr);
    width = LoWord (gr);

    size = (height - (2 * TOPMARGIN)) / lineheight;
    if (size > MAX_SCREENSIZE)
    	screensize = MAX_SCREENSIZE;		/* bounds check */
    if (size < 1)
    	size = 1;

    grow_term_to(size);

    SetPort (savePort);		/* there just has to be a better way */
}				/* growterm */

/****************************************************************************/
get_term_pos(top_p, left_p)
int *top_p, *left_p;
{
    Point mypoint;
    GrafPtr savePort;
    
    GetPort (&savePort);	/* there just has to be a better way */
    SetPort (terminalWindow);

    mypoint.v = terminalWindow->portRect.top;
    mypoint.h = terminalWindow->portRect.left;
    LocalToGlobal(&mypoint);
    
    if (top_p)
	*top_p = mypoint.v;
    if (top_p)
	*left_p = mypoint.h;
	
    SetPort (savePort);		/* there just has to be a better way */
}

set_term_pos(top, left)
int top, left;
{
    MoveWindow(terminalWindow, left, top, TRUE);
}
