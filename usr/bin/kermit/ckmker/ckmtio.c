/* edit 3/25/87 by Paul Placeway -- don't let the TextEdit buffer get too */
/*  big, because the Mac will crash if it does */
/* Version 0.8(35) - Jim Noble at Planning Research Corporation, June 1987. */
/* Ported to Megamax native Macintosh C compiler. */

/*  C K M T I O  --  interrupt, console, and port functions for Mac Kermit  */

/*
 Copyright (C) 1985, Trustees of Columbia University in the City of New York.
 Permission is granted to any individual or institution to use, copy, or
 redistribute this software so long as it is not sold for profit, provided this
 copyright notice is retained.
*/

/*
 Variables:

   dftty  -- Pointer to default tty name string, like "/dev/tty".
   dfloc  -- 0 if dftty is console, 1 if external line.
   dfprty -- Default parity
   dfflow -- Default flow control
   ckxech -- Flag for who echoes console typein:
     1 - The program (system echo is turned off)
     0 - The system (or front end, or terminal).
   functions that want to do their own echoing should check this flag
   before doing so.

 Functions for assigned communication line (either external or console tty):

   ttopen(ttname,local,mdmtyp,timo) -- Open the named tty for exclusive access.
   ttclos()                -- Close & reset the tty, releasing any access lock.
   ttpkt(speed,flow)       -- Put the tty in packet mode and set the speed.
   ttvt(speed,flow)        -- Put the tty in virtual terminal mode.
   ttinl(dest,max,timo)    -- Timed read line from the tty.
   ttinc(timo)             -- Timed read character from tty.
   ttchk()                 -- See how many characters in tty input buffer.
   ttxin(n,buf)            -- Read n characters from tty (untimed).
   ttol(string,length)     -- Write a string to the tty.
   ttoc(c)                 -- Write a character to the tty.
   ttflui()                -- Flush tty input buffer.

Functions for console terminal:

   congm()   -- Get console terminal modes.
   concb()   -- Put the console in single-character wakeup mode with no echo.
   conbin()  -- Put the console in binary (raw) mode.
   conres()  -- Restore the console to mode obtained by congm().
   conoc(c)  -- Unbuffered output, one character to console.
   conol(s)  -- Unbuffered output, null-terminated string to the console.
   conxo(n,s) -- Unbuffered output, n characters to the console.
   conchk()  -- Check if characters available at console.
   coninc()  -- Get a character from the console.
   conint()  -- Enable terminal interrupts on the console.
   connoi()  -- Disable terminal interrupts on the console.

Time functions

   msleep(m) -- Millisecond sleep
   ztime(&s) -- Return pointer to date/time string
*/

#include "ckcdeb.h"		/* Formats for debug() */

#include "ckcker.h"		/* kermit defs */
#include "ckmdef.h"		/* macintosh defs */

char *dftty = ".AIn";

/* dfloc is 0 if dftty is the user's console terminal, 1 if an external line */

int dfloc = 1;

/* Other defaults */

int dfprty = 0;			/* Default parity */
int ttprty = 0;			/* Parity in use. */
long ttspeed = -1;			/* For saving speed */
int dfflow = 1;			/* Xon/Xoff flow control */
int ttflow = -9;			/* For saving flow */

int backgrd = 0;		/* a Mac is allways in foreground */

/* Local variables */

int drop_dtr = 0;		/* drop DTR on Quit */

/* buffer and pointer for input processing */
static char *my_input_buf;	/* we give this to the serial driver queue */

/* Private buffer for myread() and its companions.  Not for use by anything
 * else.  ttflui() is allowed to reset them to initial values.  ttchk() is
 * allowed to read my_count.
 *
 * my_item is an index into mybuf[].  Increment it *before* reading mybuf[].
 *
 * A global parity mask variable could be useful too.  We could use it to
 * let myread() strip the parity on its own, instead of stripping sign
 * bits as it does now.
 */

#define MYBUFLEN 256
static unsigned char *mybufp;		/* Buffer, including push back */
static int my_count = 0;		/* Number of chars still in mybuf */
static int my_item = -1;		/* Last index read from mybuf[] */


#ifdef COMMENT
#define TTBUFL 200		/* good size (it's RBUFL guys!) */
static unsigned char *ttbuf;
#endif

static char *tto_buf;	/* output buffer for ttol() */
static ParamBlockRec iopb;	/* paramater block for ttol() */
static long tto_startticks;	/* when we started the output */
#define XOFFTIMEO 480L		/* a timeout of 8 seconds (in ticks) */

short dfltVol;			/* the volume to be used for Take- and
				 * Log-File */

extern Boolean usingRAMdriver,	/* true if using RAM serial driver (usually TRUE) */
	       have_128roms;	/* true if we are a Plus or better */

/****************************************************************************/
/*  S Y S I N I T  --  System-dependent program initialization.  */
/****************************************************************************/
sysinit ()
{
    ParamBlockRec pb;
    
    mac_init ();		/* Set up the Mac */

    /* get the default volume reference number for Take- and Log-Files */
    pb.volumeParam.ioNamePtr = NIL;
    PBGetVol (&pb, FALSE);
    dfltVol = pb.volumeParam.ioVRefNum;

    findfinderfiles ();		/* see if file was selected */

    mac_post_load_init ();	/* show the terminal window and stuff */

    return (0);
}				/* sysinit */

/* init terminal I/O buffers */
inittiobufs()
{
#ifdef COMMENT
    if ((ttbuf = (unsigned char *) NewPtr(TTBUFL + 1)) == NIL)
    	fatal("Can't allocate ttbuf", 0);
#endif
    if ((mybufp = (unsigned char *) NewPtr(MYBUFLEN + 4)) == NIL)
    	fatal("Can't allocate mybufp", 0);
    my_count = 0;		/* Number of chars still in mybuf */
    my_item = -1;		/* Last index read from mybuf[] */

    if ((tto_buf = (char *) NewPtr(MAXSP)) == NIL)
    	fatal("Can't allocate tto_buf", 0);
    if ((my_input_buf = (char *) NewPtr(MYBUFSIZE)) == NIL)
    	fatal("Can't allocate my_input_buf", 0);
}

/****************************************************************************/
/* P O R T _ O P E N -- Open and init a serial port.  port is either -6 (for */
/*  the modem port) or -8 (for the printer port)			    */
/****************************************************************************/

port_open(port)
int port;
{
    int err;
        
    if ((port != -6) && (port != -8))	/* sanity check */
	port = -6;
	
try_again:
    /* Set up IO drivers */
    innum = port;
    outnum = port - 1;
    if (innum == -6) {
	if (((err = OpenDriver ("\p.AIn", &innum)) != noErr) ||
	    ((err = OpenDriver ("\p.AOut", &outnum)) != noErr))
	    fatal ("Could not open the Modem port: ", err);
    } else {
	if (((err = OpenDriver ("\p.BIn", &innum)) != noErr) ||
	    ((err = OpenDriver ("\p.BOut", &outnum)) != noErr)) {
	    printerr("Could not open the Printer port.  Try\
 turning off Appletalk.", 0);
	    port = -6;
	    goto try_again;
	}
    }
    
    /* try for the RAM driver */
    if (innum == -6)
	err = RamSDOpen (sPortA);
    else
	err = RamSDOpen (sPortB);
	
    if (err == noErr) {
	usingRAMdriver = TRUE;
    } else {
	usingRAMdriver = FALSE;
	printerr("Can't open RAM serial driver; using the ROM driver\
 (without flow control).",0);
    }

    err = SerSetBuf (innum, my_input_buf, MYBUFSIZE);
			/* Make driver use larger buff */
    if (err)
	printerr ("Trouble making IO buffer:", err);
}

/*****************************************************************************/
/* P O R T _ C L O S E -- Close down the serial port.			     */
/*****************************************************************************/

port_close()
{
    int err;
    ParamBlockRec cpb;
    
    err = KillIO(innum);	/* Kill off IO drivers */
    if (err != noErr)
    	printerr("trouble KillIO-ing serial input driver:",err);
    err = KillIO(outnum);	/* Kill off IO drivers */
    if (err != noErr)
    	printerr("trouble KillIO-ing serial output driver:",err);

    err = SerSetBuf (innum, NULL, 0);	/* Make driver default buffer */
    if (err != noErr)
    	printerr("trouble resetting serial IO buffer:",err);

    if (usingRAMdriver) {
	if (!drop_dtr) {
	    cpb.cntrlParam.ioNamePtr = NULL;	/* see TN 174 */
	    /* tell the ram driver not to lower DTR on close */
	    cpb.cntrlParam.csCode = 13;		/* misc serial control */
	    *((unsigned char *) cpb.cntrlParam.csParam) = 0x80;	/* don't lower DTR */
	    cpb.cntrlParam.ioCRefNum = innum;
	    err = PBControl(&cpb, FALSE);
	    if ((err != noErr) && (err != -1))
    		printerr("trouble telling RAM serial driver (in) not to lower DTR:",err);

	    cpb.cntrlParam.ioNamePtr = NULL;	/* see TN 174 */
	    cpb.cntrlParam.csCode = 13;		/* misc serial control */
	    *((unsigned char *) cpb.cntrlParam.csParam) = 0x80;	/* don't lower DTR */
	    cpb.cntrlParam.ioCRefNum = outnum;
	    err = PBControl(&cpb, FALSE);
	    if ((err != noErr) && (err != -1))
    		printerr("trouble telling RAM serial driver (out) not to lower DTR:",err);
	}
	
	RamSDClose (sPortA);	/* this "returns" void */
    }
    
    err = CloseDriver (innum);
    if (err != noErr)
	printerr("trouble closing serial input driver:",err);
/*
 * For some reason or other, doing this close on a 64k ROM machine will cause
 * the mouse to freeze.  Since the input driver is the only one that really
 * matters, we just close it.
 */
    if (have_128roms) {
	err = CloseDriver (outnum);
	if (err != noErr)
	    printerr("trouble closing serial output driver:",err);
    }
}

/****************************************************************************/
/*  T T O P E N  --  Open a tty for exclusive access.  */
/*  Returns 0 on success, -1 on failure.  */
/****************************************************************************/
ttopen (ttname, lcl, modem, timeo)
char *ttname;
int *lcl;
int modem;
int timeo;
{
    my_count = 0;			/* Initialize myread() stuff */
    my_item = -1;

    if (*lcl < 0)
	*lcl = 1;		/* always in local mode */

#ifdef COMMENT
    if (ttbuf == NIL)
    	fatal("No ttbuf", 0);
#endif
    if (mybufp == NIL)
    	fatal("No mybufp", 0);
    if (tto_buf == NIL)
    	fatal("No tto_buf", 0);
    if (my_input_buf == NIL)
    	fatal("No my_input_buf", 0);

    iopb.ioParam.ioResult = 0;		/* no pending output */
    
    iopb.ioParam.ioActCount = 0;	/* for error checking in ttol() */
    iopb.ioParam.ioReqCount = 0;
    
    return (0);
}				/* ttopen */



/****************************************************************************/
/*  T T C L O S  --  Close the TTY, releasing any lock.  */
/****************************************************************************/
ttclos ()
{
    my_count = 0;			/* Initialize myread() stuff */
    my_item = -1;

    return;
}				/* ttclos */

/****************************************************************************/
/*  T T P K T  --  Condition the communication line for packets. */
/*  If called with speed > -1, also set the speed.  */
/*  Returns 0 on success, -1 on failure.  */
/****************************************************************************/
ttpkt (spd, flow, parity)	/* we only care about flow here */
int spd;
int flow;
int parity;
{
    int err1, err2;
    SerShk controlparam;	/* To change serial driver paramaters */

    ttprty = parity;                    /* Let other tt functions see these. */
    ttspeed = speed;
    ttflow = flow;			/* Now make this available too. */

    if (flow)
	controlparam.fXOn = TRUE;	/* obey flow control */
    else
	controlparam.fXOn = FALSE;	/* ignore flow control */
    controlparam.fCTS = FALSE;
    controlparam.xOn = 17;
    controlparam.xOff = 19;
    controlparam.errs = FALSE;
    controlparam.evts = FALSE;
    if (flow && usingRAMdriver)		/* old ROM driver can't do this */
	controlparam.fInX = TRUE;	/* send flow control when almost full */
    else
	controlparam.fInX = FALSE;

    err1 = SerHShake (outnum, &controlparam);
    if (err1)
	printerr ("ttpkt(): Trouble with output handshake: ", err1);
    err2 = SerHShake (innum, &controlparam);
    if (err2)
	printerr ("ttpkt(): Trouble with input handshake: ", err2);

    if (err1 || err2)
	return (-1);
    else
	return (0);
}				/* ttpkt */

/****************************************************************************/
/*  T T V T  --  Condition the communication line for a virtual terminal. */
/*  If called with spd > -1, also set the spd.  */
/*  Returns 0 on success, -1 on failure.  */
/****************************************************************************/
ttvt (spd, flow, parity)	/* all ignoreed */
int spd;
int flow;
int parity;
{
    (void) ttres();
    return (0);
}				/* ttvt */


/****************************************************************************/
/*  T T F L U I  --  Flush tty input buffer */
/****************************************************************************/
ttflui ()
{
    int err;

    my_count = 0;			/* Initialize myread() stuff */
    my_item = -1;

    err = KillIO (innum);
    if (err)
	printerr ("Bad input clear", err);

    return (0);
}				/* ttflui */

ttfluo() {				/* Flush output buffer */
    return(0);				/* (dummy for now) */
}

/****************************************************************************/
/*  T T S N D B  --  Send a break */
/****************************************************************************/
ttsndb() {
    long finalticks;

    /* delay wants 1/60th units */

    SerSetBrk (outnum);		/* start breaking */
    Delay ((long) 15, &finalticks);	/* delay about 250ms */
    SerClrBrk (outnum);		/* stop breaking */
}

/****************************************************************************/
/*
 *   Flushio:
 *      Initialize some communications constants, and clear screen and
 *      character buffers. */
/****************************************************************************/
flushio ()
{
    int err;

    err = KillIO (innum);
    if (err)
	printerr ("Bad input clear", err);
    err = KillIO (outnum);
    if (err)
	printerr ("Bad ouput clear", err);
}				/* flushio */



/****************************************************************************/
/* sendbreak - sends a break across the communictions line.
 *
 * The argument is in units of approximately 0.05 seconds (or 50
 * milliseconds).  To send a break of duration 250 milliseconds the
 * argument would be 5; a break of duration 3.5 seconds would be (umm,
 * lets see now) 70.
 *
 */
/****************************************************************************/
sendbreak (msunit)
int msunit;
{
    long finalticks;

/* delay wants 1/60th units.  We have 3/60 (50 ms.) units, convert */

    msunit = msunit * 3;

    SerSetBrk (outnum);		/* start breaking */
    Delay ((long) msunit, &finalticks);	/* delay */
    SerClrBrk (outnum);		/* stop breaking */
}				/* sendbreak */

/****************************************************************************/
/* toggledtr - Turn DTR off, wait a bit, turn it back on.
 *
 * the argument is in the same units as sendbreak (see above).
 */
/****************************************************************************/
toggle_dtr (msunit)
int msunit;
{
    long finalticks;
    ParamBlockRec pb;
    int err;

    if (usingRAMdriver) {
	/* delay wants 1/60th units.  We have 3/60 (50 ms.) units, convert */

	msunit = msunit * 3;

	pb.cntrlParam.csCode = 18;		/* lower DTR */
	pb.cntrlParam.ioCRefNum = outnum;
	err = PBControl (&pb, FALSE);
	if (err != noErr)
	    printerr ("toggle_dtr() trouble lowering DTR: ", err);

    	Delay ((long) msunit, &finalticks);	/* delay */

	pb.cntrlParam.csCode = 17;		/* raise DTR */
	pb.cntrlParam.ioCRefNum = outnum;
	err = PBControl (&pb, FALSE);
	if (err != noErr)
	    printerr ("toggle_dtr() trouble raising DTR: ", err);
   }
}				/* sendbreak */

/****************************************************************************/
/* do_xon - xon the output port and send an xon (control-Q) character        */
/****************************************************************************/
do_xon ()
{
    ParamBlockRec pb;
    int err;

    if (usingRAMdriver) {
	pb.cntrlParam.csCode = 22;		/* clear XOFF for my output */
	pb.cntrlParam.ioCRefNum = outnum;
	err = PBControl (&pb, FALSE);
	if (err != noErr)
	    printerr ("do_xon() trouble unblocking output port: ", err);

	pb.cntrlParam.csCode = 24;		/* unconditionally send XON */
	pb.cntrlParam.ioCRefNum = outnum;
	err = PBControl (&pb, FALSE);
	if (err != noErr)
	    printerr ("do_xon() trouble sending XON: ", err);
   } else {
   	OutputChar ('\021');	/* XON */
   }
}				/* sendbreak */

/****************************************************************************/
/*  T T S S P D  --  Set tty speed */
/****************************************************************************/

ttsspd (cps) int cps; {
    return (-1);		/* $$$ IMPLEMENT THIS!!! */
}


/****************************************************************************/
/*  T T G S P D  --  Set tty speed */
/****************************************************************************/

long
ttgspd () {
    if (speed <= 0L)
    	printerr("Speed got reset, now == ", (int) speed);
    return (speed);
}


/* Interrupt Functions */

/****************************************************************************/
/* Set up terminal interrupts on console terminal */
/* Set an interrupt trap. */
/****************************************************************************/
conint (f)
int (*f) ();
{
    return;
}				/* conint */



/****************************************************************************/
/* Reset console terminal interrupts */
/****************************************************************************/
connoi ()
{
    return;
}				/* connoi */

/****************************************************************************/
/* writeps - write a pascal string to the serial port.
 *
 */
/****************************************************************************/
writeps (s)
char *s;
{
    long wcnt, w2;
    char *s2;

    w2 = wcnt = *s++;		/* get count */

    for (s2 = s; w2 > 0; w2--, s2++)	/* add parity */
	*s2 = dopar (*s2);
    
    (void) ttol(s, wcnt);	/* ttol will printerr() if it has a problem */

    return;
}				/* writeps */

extern Boolean have_multifinder;/* true if running under MF */
extern Boolean in_background;	/* becomes true if running MF and in bg */
extern long mf_sleep_time;	/* number of 60ths between task time */

/****************************************************************************/
/*  T T O L  --  Similar to "ttinl", but for writing.  */
/****************************************************************************/
ttol (s, n)
char *s;
int n;
{
    long finalticks;
    int err;
    ParamBlockRec cpb;

#ifdef COMMENT
    debug(F101,"ttol n","",n);
    debug(F101," s","",s);

    /*
     * the straight-forward way to write out a buffer: synchronously
     */
    finalticks = n;
    err = FSWrite(outnum, &finalticks, s);
    if ((err != noErr) || (finalticks != n)) {
	printerr("ttol FSWrite error: ", err);
	return (-1);
    }
    return (n);
#endif

    /*
     * The fancy to write out strings: do async writes, waiting for the
     * previous call to finish first (and possibly unsticking the serial
     * driver)
     */

    /* wait for previous call to finish */
    while (iopb.ioParam.ioResult == 1) {	/* while the prev. request is still running */
	if (have_multifinder && protocmd != 0) {	/* if we're running protocol */
	    miniparser (TRUE);	/* keep mac running */
	    finalticks = TickCount ();
	} else {		/* else terminal emulation */
	    Delay ((long) 1, &finalticks);	/* wait for a bit */
	}
	/* (PWP) If we have waited too long, unblock the output (keep the
	   Mac from freezing up) */
	if ((usingRAMdriver) && (finalticks > (tto_startticks + XOFFTIMEO))) {
	    cpb.cntrlParam.csCode = 22;		/* clear XOFF for my output */
	    cpb.cntrlParam.ioCRefNum = outnum;
	    err = PBControl (&cpb, FALSE);
	    if (err != noErr)
		printerr ("ttol() trouble unblocking output port: ", err);
	    tto_startticks = TickCount ();	/* get starting time */
	}
    }
    
    /* check for errors in previous call */
    if (iopb.ioParam.ioResult) {
	printerr ("Error in previous PBWrite:", iopb.ioParam.ioResult);
	return (-1);
    }
    if (iopb.ioParam.ioActCount != iopb.ioParam.ioReqCount) {
	printerr ("PBWrite to serial didn't write enough: ", iopb.ioParam.ioActCount);
	printerr ("(asked for:)", iopb.ioParam.ioReqCount);
	return (-1);
    }

    /* the previous call is now done, so we can load in our new information */
    
    if (n > MAXSP) {		/* MAXSP == sizeof(tto_buf) */
    	printerr("ttol asked to write too many chars: ", n);
	return (-1);
    }
    bcopy(s, tto_buf, n);	/* in ckmfio.h if nowhere else */

    iopb.ioParam.ioCompletion = NULL;
    iopb.ioParam.ioNamePtr = NULL;
    iopb.ioParam.ioVRefNum = 0;

    iopb.ioParam.ioRefNum = outnum;
    iopb.ioParam.ioVersNum = 0;
    iopb.ioParam.ioPermssn = 0;
    iopb.ioParam.ioMisc = NULL;
    iopb.ioParam.ioBuffer = tto_buf;
    iopb.ioParam.ioReqCount = (long) n;
    iopb.ioParam.ioPosMode = 0;
    iopb.ioParam.ioPosOffset = 0;
    
    tto_startticks = TickCount ();	/* get starting time */

    PBWrite (&iopb, TRUE);	/* request an async. write */
    if (protocmd != 0)
	miniparser (TRUE);	/* allow other tasks to run */
    return (n);			/* fake a good run */
}				/* ttol */


/*  T T O C  --  Output a character to the communication line  */

/*
 This function should only used for interactive, character-mode operations,
 like terminal connection, script execution, dialer i/o, where the overhead
 of the signals and alarms does not create a bottleneck.
*/

ttoc(c) char c; {
    char foo[2];
    
    foo[0] = c;
    foo[1] = '\0';
    return (ttol(foo, 1));
}

/* ckumyr.c by Kristoffer Eriksson, ske@pkmab.se, 15 Mar 1990. */


/* myread() -- Efficient read of one character from communications line.
 *
 * Uses a private buffer to minimize the number of expensive read() system
 * calls.  Essentially performs the equivalent of read() of 1 character, which
 * is then returned.  By reading all available input from the system buffers
 * to the private buffer in one chunk, and then working from this buffer, the
 * number of system calls is reduced in any case where more than one character
 * arrives during the processing of the previous chunk, for instance high
 * baud rates or network type connections where input arrives in packets.
 * If the time needed for a read() system call approaches the time for more
 * than one character to arrive, then this mechanism automatically compensates
 * for that by performing bigger read()s less frequently.  If the system load
 * is high, the same mechanism compensates for that too.
 *
 * myread() is a macro that returns the next character from the buffer.  If the
 * buffer is empty, mygetbuf() is called.  See mygetbuf() for possible error
 * returns.
 *
 * This should be efficient enough for any one-character-at-a-time loops.
 * For even better efficiency you might use memcpy()/bcopy() or such between
 * buffers (since they are often better optimized for copying), but it may not
 * be worth it if you have to take an extra pass over the buffer to strip
 * parity and check for CTRL-C anyway.
 *
 * Note that if you have been using myread() from another program module, you
 * may have some trouble accessing this macro version and the private variables
 * it uses.  In that case, just add a function in this module, that invokes the
 * macro.
 */
 
#define mac_myread(timeo_ticks,intim)  (--my_count < 0 \
			? mac_mygetbuf(timeo_ticks,intim) \
			: (int) (mybufp[++my_item]))

/* Specification: Push back up to one character onto myread()'s queue.
 *
 * This implementation: Push back characters into mybuf. At least one character
 * must have been read through myread() before myunrd() may be used.  After
 * EOF or read error, again, myunrd() can not be used.  Sometimes more than
 * one character can be pushed back, but only one character is guaranteed.
 * Since a previous myread() must have read its character out of mybuf[],
 * that guarantees that there is space for at least one character.  If push
 * back was really needed after EOF, a small addition could provide that.
 *
 * myunrd() is currently not called from anywhere inside kermit...
 */
#ifdef NOTUSED
myunrd(ch) CHAR ch; {
    if (my_item >= 0) {
	mybufp[my_item--] = ch;
	++my_count;
    }
}
#endif

/* mygetbuf() -- Fill buffer for myread() and return first character.
 *
 * This function is what myread() uses when it can't get the next character
 * directly from its buffer.  First, it calls a system dependent myfillbuf()
 * to read at least one new character into the buffer, and then it returns
 * the first character just as myread() would have done.  This function also
 * is responsible for all error conditions that myread() can indicate.
 *
 * Returns: When OK	=> a positive character, 0 or greater.
 *	    When EOF	=> -2.
 *	    When error	=> -3, error code in errno.
 *
 * Older myread()s additionally returned -1 to indicate that there was nothing
 * to read, upon which the caller would call myread() again until it got
 * something.  The new myread()/mygetbuf() always gets something.  If it 
 * doesn't, then make it do so!  Any program that actually depends on the old
 * behaviour will break.
 *
 * The older version also used to return -2 both for EOF and other errors,
 * and used to set errno to 9999 on EOF.  The errno stuff is gone, EOF and
 * other errors now return different results, although Kermit currently never
 * checks to see which it was.  It just disconnects in both cases.
 *
 * Kermit lets the user use the quit key to perform some special commands
 * during file transfer.  This causes read(), and thus also mygetbuf(), to
 * finish without reading anything and return the EINTR error.  This should
 * be checked by the caller.  Mygetbuf() could retry the read() on EINTR,
 * but if there is nothing to read, this could delay Kermit's reaction to
 * the command, and make Kermit appear unresponsive.
 *
 * The debug() call should be removed for optimum performance.
 */
/* myfillbuf():
 * System-dependent read() into mybuf[], as many characters as possible.
 *
 * Returns: OK => number of characters read, always more than zero.
 *          EOF => 0
 *          Error => -1, error code in errno.
 *
 * If there is input available in the system's buffers, all of it should be
 * read into mybuf[] and the function return immediately.  If no input is
 * available, it should wait for a character to arrive, and return with that
 * one in mybuf[] as soon as possible.  It may wait somewhat past the first
 * character, but be aware that any such delay lengthens the packet turnaround
 * time during kermit file transfers.  Should never return with zero characters
 * unless EOF or irrecoverable read error.
 *
 * Correct functioning depends on the correct tty parameters being used.
 * Better control of current parameters is required than may have been the
 * case in older Kermit releases.  For instance, O_NDELAY (or equivalent) can 
 * no longer be sometimes off and sometimes on like it used to, unless a 
 * special myfillbuf() is written to handle that.  Otherwise the ordinary 
 * myfillbuf()s may think they have come to EOF.
 *
 * If your system has a facility to directly perform the functioning of
 * myfillbuf(), then use it.  If the system can tell you how many characters
 * are available in its buffers, then read that amount (but not less than 1).
 * If the system can return a special indication when you try to read without
 * anything to read, while allowing you to read all there is when there is
 * something, you may loop until there is something to read, but probably that
 * is not good for the system load.
 */


mac_mygetbuf(long timeo_tics, long intim) {
    long avail;		/* can't be register */
    long finaltics;	/* can't be register */
    int err;
    
    if (mybufp == NIL)
    	fatal("No mybufp (in mac_mygetbuf())", 0);

    for (;;) {
	SerGetBuf (innum, &avail);	/* Get available count */

	if (avail > 0)
	    break;			/* we can get these chars */
	
	/* no chars availiable yet */
	if (protocmd != 0) {	/* if we're running protocol */
	    miniparser (TRUE);	/* keep mac running */
	    /* Delay(6L, &finaltics); */
	    if (sstate == 'a')	/* abort occured? */
		return (-1);	/* ugh, look like timeout */
	}
	    
	finaltics = TickCount ();
	if (timeo_tics > 0) {		/* Want to do timeout? */
	    if (intim + timeo_tics < finaltics) {
		return (-1);	/* Too long, give up */
	    }
	}
	
	/* go back and try to get more chars */
    }

    if (avail > MYBUFLEN)
	avail = MYBUFLEN;

    /*
     * CAREFUL: the Mac FSRead() function gets how much to read,
     * AND PUTS HOW MUCH IT DID READ into avail
     */
    err = FSRead (innum, &avail, mybufp);		/* Into our buffer */

    if (err != noErr) {
	screen(SCR_EM,0,(long) err,"Serial input error:");
	return (-3);	/* return input error */
    }
    if (avail <= 0) {
	screen(SCR_EM,0,(long) avail,"No serial input error, but didn't read any:");
	return (-3);	/* return input error */
    }

    /* not at end of packet yet, so let other tasks have a chance */
    if (in_background)
	miniparser (TRUE);	/* allow other tasks to run */

    my_count = (int) avail;
    
    /* debug(F101, "myfillbuf read", "", my_count); */

    --my_count;
    return((int) (mybufp[my_item = 0]));
}




/****************************************************************************/
/*  T T I N L  --  Read a record (up to break character) from comm line.  */
/*
  If no break character encountered within "max", return "max" characters,
  with disposition of any remaining characters undefined.  Otherwise, return
  the characters that were read, including the break character, in "dest" and
  the number of characters read as the value of function, or 0 upon end of
  file, or -1 if an error occurred.  Times out & returns error if not completed
  within "timo" seconds.
*/
/*
  Reads up to "max" characters from the communication line, terminating on
  the packet-end character (eol), or timing out and returning -1 if the eol
  character not encountered within "timo" seconds.  The characters that were
  input are copied into "dest" with their parity bits stripped if parity was
  selected.  Returns the number of characters read.  Characters after the
  eol are available upon the next call to this function.

  The idea is to minimize the number of system calls per packet, and also to
  minimize timeouts.  This function is the inner loop of the program and must
  be as efficient as possible.  The current strategy is to use myread().

  WARNING: this function calls parchk(), which is defined in another module.
  Normally, ckutio.c does not depend on code from any other module, but there
  is an exception in this case because all the other ck?tio.c modules also
  need to call parchk(), so it's better to have it defined in a common place.
*/
/****************************************************************************/

#define CTRLC '\03'

#ifdef PARSENSE
ttinl(dest,max,timo,eol,start) int max,timo; CHAR *dest, eol, start; {
    int flag;
#else
ttinl(dest,max,timo,eol) int max,timo; CHAR *dest, eol; {
#endif
    register int i, j, m, n;		/* local variables */
    int x, ccn;
    long timeoticks;
    long intim;			/* when we started */
    int err;
    
    debug(F101,"ttinl max","",max);
    debug(F101,"ttinl timo","",timo);

#ifdef PARSENSE
    debug(F000,"ttinl start","",start);
    flag = 0;				/* Start of packet flag */
#endif

    ccn = 0;				/* Control-C counter */
    x = 0;				/* Return code */
    m = (ttprty) ? 0177 : 0377;         /* Parity stripping mask. */
    *dest = '\0';                       /* Clear destination buffer */

    if (timo <= 0) {		/* untimed */
	timo = 0;		/* Safety */
	intim = 0;		/* tell mac_myread not to timeout */
	timeoticks = 0;
    } else {
	timeoticks = timo * 60;
	intim = TickCount ();	/* now */
    }

    i = 0;
    while (i < max-1) {
	/* debug(F101,"ttinl i","",i); */
	if ((n = mac_myread(timeoticks,intim)) < 0) {
	    debug(F101,"ttinl myread failure, n","",n);
	    x = -1; break;
	}
	/* debug(F101,"ttinl char","",n&m); */

#ifdef PARSENSE
	if ((flag == 0) && ((n & 0x7f) == start)) flag = 1;
	if (flag) dest[i++] = n & m;
#else
	dest[i++] = n & m;
#endif
	if ((n & 0x7f) == CTRLC) {	/* Check for ^C^C */
	    if (++ccn > 1) {	/* If we got 2 in a row, bail out. */
	    	screen(SCR_EM,0,(long) 0,"Remote side ^C^C abort...");
		return(-2);
	    }
	} else ccn = 0;		/* Not ^C, so reset ^C counter, */

#ifdef PARSENSE
	if (flag == 0) {
	    debug(F101,"ttinl skipping","",n);
	    continue;
	}
#endif

    /* Check for end of packet */

	if ((n & 0x7f) == eol) {
	    debug(F101,"ttinl got eol","",eol);
	    dest[i] = '\0';		/* Yes, terminate the string, */
	    /* debug(F101,"ttinl i","",i); */
	    
#ifdef PARSENSE
/* Here's where we actually check and adjust the parity. */
/* The major flaw here is if parity is NONE (ttprty = 0) and the packets */
/* really do have no parity, then parchk() is called for every packet. */
/* In practice, this doesn't really harm efficiency noticably, but it would */
/* be better if ttinl() had a way of knowing to stop doing this once a */
/* particular file transfer had been started and checked. */
	    if (ttprty == 0) {
		if ((ttprty = parchk(dest,start,i)) > 0) {
		    int j;
		    debug(F101,"ttinl senses parity","",ttprty);
		    debug(F110,"ttinl packet before",dest,0);
		    for (j = 0; j < i; j++)
			dest[j] &= 0x7f; /* Strip parity from packet */
		    debug(F110,"ttinl packet after ",dest,0);
		} else debug(F101,"parchk","",ttprty);
	    }
#endif
	    debug(F111,"ttinl got", dest,i);
	    return(i);
	}
    }		/* end while (i < max-1) */
    debug(F100,"ttinl timout","",0);    /* Get here on timeout. */
    debug(F111," with",dest,i);
    return(x);                          /* and return error code. */

}				/* ttinl */


/****************************************************************************/
/* ttinc(timo) - read a character with timeout.  Return -1 on timeout. */
/****************************************************************************/
ttinc (timo)
int timo;
{
    register int m, n = 0;
    long timeoticks;
    long intim;			/* when we started */

    if (timo <= 0) {		/* untimed */
	intim = 0;		/* tell mac_myread not to timeout */
	timeoticks = 0;
    } else {
	timeoticks = timo * 60;
	intim = TickCount ();	/* now */
    }
    
    m = (ttprty) ? 0177 : 0377;         /* Parity stripping mask. */

        /* comm line failure returns -1 thru myread, so no &= 0377 */
    n = mac_myread(timeoticks,intim);		/* Wait for a character... */

    /* debug(F101,"ttinc n","",n); */
    return(n < 0 ? n : n & m);
}				/* ttinc */

/****************************************************************************/
/* PWP: input as many characters as we can read from the serial line right now */
/****************************************************************************/

ttinm(buf, max)
register char *buf;
register int max;
{
    long avil, num;
    int err, i;

    /*
     * DANGER WILL ROBINSON: this KNOWS about how mac_myread works
     * (in an insestious way), so BE CAREFUL!!!
     */

    if (my_count > 0) {		/* do we have chars buffered up? */
    	for (i = 0; (my_count > 0) && (i < max); i++)
	    *buf++ = mac_mygetbuf(0L,0L);
	return (i);		/* return contents of previous read buffer */
    }

    SerGetBuf (innum, &avil);	/* Get available count */

    if (avil > 0) {	/* Have something? */
	num = (avil > max) ? max : avil;	/* Set max */

	err = FSRead (innum, &num, buf);	/* Into our buffer */
	if (err != noErr)
	    printerr ("Serial input error: ", err);
	return (num);		/* return how many */
    } else {
    	return (0);
    }
}

/****************************************************************************/
/****************************************************************************/
ttchk ()
{
    long avcnt;			/* pascal long */

    SerGetBuf (innum, &avcnt);	/* get available */

    return (avcnt + my_count);	/* return avail plus our own */
}				/* ttchk */



/****************************************************************************/
/* T T R E S -- Reset the serial line after doing a protocol things	    */
/****************************************************************************/
ttres ()
{
    int err1, err2;
    SerShk controlparam;	/* To change serial driver paramaters */

    if (flow)
	controlparam.fXOn = TRUE;	/* obey flow control */
    else
	controlparam.fXOn = FALSE;	/* ignore flow control */
    controlparam.fCTS = FALSE;
    controlparam.xOn = 17;
    controlparam.xOff = 19;
    controlparam.errs = FALSE;
    controlparam.evts = FALSE;
    controlparam.fInX = FALSE;

    err1 = SerHShake (outnum, &controlparam);
    if (err1)
	printerr ("ttvt(): Trouble with output handshake: ", err1);
    err2 = SerHShake (innum, &controlparam);
    if (err2)
	printerr ("ttvt(): Trouble with input handshake: ", err2);

    if (err1 || err2)
	return (-1);
    else
	return (1);
}				/* ttres */



unsigned long starttime;
Boolean timerRunning = FALSE;

/****************************************************************************/
/*  R T I M E R --  Reset elapsed time counter  */
/****************************************************************************/
rtimer ()
{
    GetDateTime (&starttime);
    timerRunning = TRUE;
}				/* rtimer */



/****************************************************************************/
/*  G T I M E R --  Get current value of elapsed time counter in seconds  */
/****************************************************************************/
long
gtimer ()
{
    unsigned long secs;

    if (timerRunning) {
	GetDateTime (&secs);
	return (secs - starttime);
	timerRunning = FALSE;
    } else
	return (0);
}				/* gtimer */


/****************************************************************************/
/*  Z T I M E  --  Return date/time string  */
/****************************************************************************/
ztime (s)
char **s;
{
    unsigned long secs;
    char timestr[10];
    static char dtime[25];

    GetDateTime (&secs);
    IUDateString (secs, shortDate, dtime);
    p2cstr(dtime);
    IUTimeString (secs, FALSE, timestr);
    p2cstr(timestr);
    strcat (dtime, " ");
    strcat (dtime, timestr);
    *s = dtime;
}				/* ztime */



/* Console IO routines.  The console is implemented as a text edit structure.
 * These routines are supported:
 *
 * conoc(c)   -- output one character to TE record at insertion point
 * conol(s)   -- output null terminated string to TE record " "
 * conoll(s)  -- same but with CR on the end
 * conxo(n,s) -- n character to TE record " "
 *
 */

#define NILTE ((TEHandle ) NILPTR)
#define LF 012
#define CR 015

#define TE_TOOBIG	32000	/* should be less than 32k-1 */
#define TE_TRIM		512	/* should be some approiate size */
#define TE_MAX		32767	/* the maximum size of text in a TE buffer */

TEHandle consoleTE = NILTE;	/* storage for console TE ptr */

/****************************************************************************/
/****************************************************************************/
consette (t)
TEHandle t;
{
    if (consoleTE != NILTE)	/* already have TE record */
	printerr ("TE record present at consette! ", 0);
    consoleTE = t;
}				/* consette */

/****************************************************************************/
/* PWP: trimcon(length) should be called before TEInsert()ing <length>    */
/*      number of bytes.  This function checks to see if there is enough  */
/*	room for this many characters, and deletes a bit from the beginning  */
/*	if there isn't space.  */
/****************************************************************************/
static
trimcon(l)
register int l;
{
    if (consoleTE != NILTE) {
	if (((*consoleTE)->teLength + l) > TE_TOOBIG) {
	    TESetSelect(0, TE_TRIM, consoleTE);	/* select the beginning part */
	    TEDelete(consoleTE);		/* nuke it */
	    	/* and make insertion point at end again */
	    TESetSelect(TE_MAX, TE_MAX, consoleTE);
	}
    }
}

/****************************************************************************/
/*  C O N X O  --  Output a string of length len to the console text edit record */
/****************************************************************************/
conxo (s, len)
register char *s;
{
    register char *t;
    
    /* change NLs to CRs for the Mac */
    for (t = s; *t && (t - s < len); t++)
    	if (*t == LF)
	    *t = CR;

    /* debug (F101, "conxo here: ", s, len); */
    if (consoleTE != NILTE) {	/* is it present? */
	TEDeactivate(consoleTE);
	trimcon(len);
	TEInsert (s, (long) len, consoleTE);	/* insert the string */
	TESetSelect(TE_MAX, TE_MAX, consoleTE);
	TEActivate(consoleTE);
	rcdwscroll ();		/* possibly scroll it */
    }
    return (0);
}				/* conxo */


/****************************************************************************/
/*  C O N O C  --  Output a character to the console text edit record */
/****************************************************************************/
conoc (c)
char c;
{
    (void) conxo (&c, 1);

#ifdef COMMENT
    static long len = 1;

    if (c == LF)		/* we don't support this */
	return;

    if (consoleTE != NILTE) {	/* is it present? */
	TEDeactivate(consoleTE);
	trimcon(1);
	TEInsert (&c, len, consoleTE);	/* insert the char */
	TESetSelect(TE_MAX, TE_MAX, consoleTE);
	TEActivate(consoleTE);

	if (c == CR)		/* if CR */
	    rcdwscroll ();	/* then possibly scroll it */
    }
#endif
}				/* conoc */



/****************************************************************************/
/****************************************************************************/
conopen ()
{
    if (consoleTE == NILTE)
	printerr ("Tried to open console before TE set", 0);
    return (1);
}				/* conopen */



/****************************************************************************/
/*  C O N O L  --  Write a line to the console text edit record  */
/****************************************************************************/
conol (s)
register char *s;
{
    return (conxo (s, strlen (s)));

#ifdef COMMENT
    long len = strlen (s);	/* fetch length */

    /* debug (F101, "conol here: ", s, 0); */
    if (consoleTE != NILTE) {	/* is it present? */
	TEDeactivate(consoleTE);
	trimcon(len);
	TEInsert (s, len, consoleTE);	/* insert the string */
	TESetSelect(TE_MAX, TE_MAX, consoleTE);
	TEActivate(consoleTE);
	rcdwscroll ();		/* possibly scroll it */
    }
    return (0);
#endif
}				/* conol */




/****************************************************************************/
/*  C O N O L L  --  Output a string followed by CRLF  */
/****************************************************************************/
conoll (s)
char *s;
{
    (void) conol (s);		/* first the string */
    (void) conoc (CR);			/* now the return */
    return (0);
}				/* conoll */
