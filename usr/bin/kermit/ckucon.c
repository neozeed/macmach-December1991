char *connv = "CONNECT Command for UNIX, 5A(031) 5 Nov 91";

/*  C K U C O N  --  Dumb terminal connection to remote system, for UNIX  */
/*
 Author: Frank da Cruz (fdc@columbia.edu, FDCCU@CUVMA.BITNET),
 Columbia University Center for Computing Activities.
 First released January 1985.
 Copyright (C) 1985, 1991, Trustees of Columbia University in the City of New 
 York.  Permission is granted to any individual or institution to use, copy, or
 redistribute this software so long as it is not sold for profit, provided this
 copyright notice is retained. 
*/

#include "ckcdeb.h"			/* Common things first */

#ifdef NEXT
#undef NSIG
#include <sys/wait.h>			/* For wait() */
#endif /* NEXT */

#include <signal.h>			/* Signals */

#ifndef ZILOG
#include <setjmp.h>			/* Longjumps */
#else
#include <setret.h>
#endif /* ZILOG */

/* Kermit-specific includes */

#include "ckcasc.h"			/* ASCII characters */
#include "ckcker.h"			/* Kermit things */
#include "ckucmd.h"			/* For xxesc() prototype */
#include "ckcnet.h"			/* Network symbols */
#ifndef NOCSETS
#include "ckcxla.h"			/* Character set translation */
#endif /* NOCSETS */

/* Internal function prototypes */

_PROTOTYP( VOID doesc, (char) );
_PROTOTYP( VOID logchar, (char) );
_PROTOTYP( int hconne, (void) );

#ifndef SIGUSR1				/* User-defined signals */
#define SIGUSR1 30
#endif /* SIGUSR1 */

#ifndef SIGUSR2
#define SIGUSR2 31
#endif /* SIGUSR2 */

extern int local, escape, duplex, parity, flow, seslog, mdmtyp, ttnproto,
 cmask, cmdmsk, network, nettype, debses, deblog, tvtflg, sessft, sosi, tnlm,
 xitsta, what, ttyfd, quiet;
#ifndef NOICP
extern CHAR keymap[];
#endif /* NOICP */
extern long speed;
extern char ttname[], sesfil[];
static int quitnow = 0;
static int dohangup = 0;

_PROTOTYP( char * cbchr, ( int ) );

int goterr = 0, active;			/* Variables global to this module */
static char *p;
static char temp[50];

#define LBUFL 200			/* Line buffer */
char lbuf[LBUFL];
char kbuf[10], *kbp;			/* Keyboard buffer */

#ifdef SUNX25				/* SunLink X.25 defined */
char x25ibuf[MAXIX25];
char x25obuf[MAXOX25];
int ibufl;
int obufl;
unsigned char tosend = 0;
int linkid, lcn;
static int dox25clr = 0;
CHAR padparms[MAXPADPARMS+1];
int padpipe[2];				/* Pipe descriptor to pass PAD parms */
#endif /* SUNX25 */

/* Connect state parent/child communication signal handlers */

static jmp_buf con_env;		 /* Environment pointer for connect errors */

static
SIGTYP
conn_int(foo) int foo; {		/* Modem read failure handler, */
    longjmp(con_env,1);			/* notifies parent process to stop */
}

static
SIGTYP
mode_chg(foo) int foo; {
#ifdef SUNX25				/* X.25 read new params from pipe */
    if (nettype == NET_SX25) {
        read(padpipe[0],padparms,MAXPADPARMS);
        debug(F100,"pad_chg","",0);
    } else {
#endif /* SUNX25 */
	duplex = 1 - duplex;		/* Toggle duplex mode. */
	debug(F101,"mode_chg duplex","",duplex);
#ifdef SUNX25
    }
#endif /* SUNX25 */
    longjmp(con_env,2);			/* Return 2 so we can tell. */
}

/*  C O N E C T  --  Perform terminal connection  */

PID_T parent_id = (PID_T)0;	/* process id of parent (keyboard reader) */

#ifdef TNCODE
extern int tn_init;			/* Telnet protocol initialized. */
#endif /* TNCODE */

#ifndef NOCSETS
extern CHAR (*xls[MAXTCSETS+1][MAXFCSETS+1])();	/* Character set xlate */
extern CHAR (*xlr[MAXTCSETS+1][MAXFCSETS+1])();	/* functions. */
extern int language;		/* Current language. */
extern struct langinfo langs[];	/* Language info. */
extern int tcsr, tcsl;		/* Terminal character sets, remote & local. */
static int langsv;		/* Remember language */
static int tcs;			/* Intermediate (xfer) char set */
static CHAR (*sxo)();		/* Local translation functions */
static CHAR (*rxo)();		/* for output (sending) terminal chars */
static CHAR (*sxi)();		/* and for input (receiving) terminal chars. */
static CHAR (*rxi)();
#endif /* NOCSETS */

int shift = 0;			/* SO/SI shift state */

int
conect() {
    PID_T pid; 			/* process id of child (modem reader) */
    int	n,			/* general purpose counter */
        i;			/* workers */

    int c;			/* c is a character, but must be signed 
				   integer to pass thru -1, which is the
				   modem disconnection signal, and is
				   different from the character 0377 */
    int c2;			/* A copy of c */
    int csave;			/* Another copy of c */
    int sjval;			/* Setjump return value */
    int tx;			/* tn_doop() return code */

    char errmsg[50], *erp;		/* Error message and pointer */

    if (!local) {
#ifdef NETCONN
	printf("Sorry, you must 'set line' or 'set host' first\n");
#else
	printf("Sorry, you must 'set line' first\n");
#endif /* NETCONN */
	return(0);
    }
    if (speed < 0L && network == 0) {
	printf("Sorry, you must 'set speed' first\n");
	return(0);
    }
#ifdef TCPSOCKET
    if (network && (nettype != NET_TCPB)
#ifdef SUNX25
        && (nettype != NET_SX25)
#endif /* SUNX25 */
    ) {
	printf("Sorry, network type not supported yet\n");
	return(0);
    }
#endif /* TCPSOCKET */

    if (ttyfd < 0) {			/* If communication device not open */
	debug(F111,"ckucon opening",ttname,0); /* Open it now */
	if (ttopen(ttname,&local,mdmtyp,0) < 0) {
	    erp = errmsg;
	    sprintf(erp,"Sorry, can't open %s",ttname);
	    perror(errmsg);
	    debug(F110,"ckucon open failure",errmsg,0);
	    return(0);
	}
    }
    dohangup = 0;
#ifdef NETCONN
    if (network) {
	if (!quiet) printf("Connecting to host %s",ttname);
#ifdef SUNX25
        if (nettype == NET_SX25) {
            dox25clr = 0;
            if (!quiet) printf(", Link ID %d, LCN %d",linkid,lcn);
        }
#endif /* SUNX25 */
    } else {
	if (!quiet) printf("Connecting to %s",ttname);
	if (speed > -1L && !quiet) printf(", speed %ld",speed);
    }
#else /* Not NETCONN */
    if (!quiet) printf("Connecting to %s",ttname);
    if (speed > -1L && !quiet) printf(", speed %ld",speed);
#endif /* NETCONN */
    if (!quiet) {
	printf(".\r\nThe escape character is %s (ASCII %d).\r\n",
	       dbchr(escape),escape);
	printf("Type the escape character followed by C to get back,\r\n");
	printf("or followed by ? to see other options.\r\n");
	if (seslog) {
	    printf("(Session logged to %s, ",sesfil);
	    printf("%s)\r\n", sessft ? "binary" : "text");
	}
	if (debses) printf("Debugging Display...)\r\n");
    }    
/* Condition console terminal and communication line */	    

    if (conbin(escape) < 0) {
	printf("Sorry, can't condition console terminal\n");
	return(0);
    }
    debug(F101,"connect cmask","",cmask);
    debug(F101,"connect cmdmsk","",cmdmsk);
    goterr = 0;
    if (ttvt(speed,flow) < 0) {		/* Enter "virtual terminal" mode */
	goterr = 1;			/* Error recovery... */
	tthang();			/* Hang up and close the device. */
	ttclos(0);
#ifdef COMMENT
#ifdef TNCODE
	tn_init = 0;
#endif /* TNCODE */
#endif /* COMMENT */
	if (ttopen(ttname,&local,mdmtyp,0) < 0) { /* Open it again... */
	    erp = errmsg;
	    sprintf(erp,"Sorry, can't reopen %s",ttname);
	    perror(errmsg);
	    return(0);
	}
	if (ttvt(speed,flow) < 0) {	/* Try virtual terminal mode again. */
	    conres();			/* Failure this time is fatal. */
	    printf("Sorry, Can't condition communication line\n");
	    return(0);
	}
    }
    debug(F101,"connect ttvt ok, escape","",escape);

#ifndef NOCSETS
/* Set up character set translations */

#ifdef KANJI
/* Kanji not supported yet */
    if (tcsr == FC_JIS7 || tcsr == FC_SHJIS ||
	tcsr == FC_JEUC || tcsr == FC_JDEC  ||
	tcsl == FC_JIS7 || tcsl == FC_SHJIS ||
	tcsl == FC_JEUC || tcsl == FC_JDEC)
      tcs = TC_TRANSP;
    else
#endif /* KANJI */
#ifdef CYRILLIC
      if (tcsl == FC_KOI7  || tcsl == FC_KOI8 ||
	  tcsl == FC_CP866 || tcsl == FC_CYRILL)
	tcs = TC_CYRILL;
      else
#else
	tcs = TC_1LATIN;
#endif /* CYRILLIC */

    if (tcsr == tcsl) {			/* Remote and local sets the same? */
	sxo = rxo = NULL;		/* If so, no translation. */
	sxi = rxi = NULL;
    } else {				/* Otherwise, set up */
	sxo = xls[tcs][tcsl];		/* translation function */
	rxo = xlr[tcs][tcsr];		/* pointers for output functions */
	sxi = xls[tcs][tcsr];		/* and for input functions. */
	rxi = xlr[tcs][tcsl];
    }
/*
  This is to prevent use of zmstuff() and zdstuff() by translation functions.
  They only work with disk i/o, not with communication i/o.  Luckily Russian
  translation functions don't do any stuffing...
*/
    langsv = language;
    language = L_USASCII;
#endif /* NOCSETS */

/* This is a label we jump back to in case lower fork sensed the need */
/* to change modes.  Ugly, but it works.  (Another way would be to */
/* communicate thru pipes.  The X.25 code does this.) */

newfork:
    parent_id = getpid();		/* Get parent id for signalling */
    signal(SIGUSR1,SIG_IGN);		/* Don't kill myself */
    signal(SIGUSR2,SIG_IGN);
#ifdef SUNX25
    pipe(padpipe);                      /* Create pipe to pass PAD parms */
#endif /* SUNX25 */
#ifdef OXOS
/*
  Because of the "extended security controls" in Olivetti X/OS,
  the killing and killed process must have the same REAL uid.  
  Otherwise kill() gets ESRCH.
*/
    priv_on();
#endif /* OXOS */
    pid = fork();			/* All ok, make a fork */
    if (pid == -1) {			/* Can't create it. */
	conres();			/* Reset the console. */
	perror("Can't create keyboard fork");
	if (!quiet) printf("[Back at Local System]");
	printf("\n");
	what = W_NOTHING;		/* So console modes set right. */
#ifndef NOCSETS
	language = langsv;		/* Restore language */
#endif /* NOCSETS */
	parent_id = (PID_T) 0;
	return(1);
    }
    if (pid) {
	what = W_CONNECT;		/* Keep track of what we're doing */
	active = 1;			/* This fork reads, sends keystrokes */

	/* Catch communication errors or mode changes in lower fork */

	if ((sjval = setjmp(con_env)) == 0) {
	    signal(SIGUSR1,conn_int);	/* Routine for child process exit. */
	    signal(SIGUSR2,mode_chg);	/* Routine for mode change. */
#ifdef COMMENT
#ifdef TNCODE
	    if (network && ttnproto == NP_TELNET) /* If telnet connection, */
	      if (!tn_init++) tn_ini();           /* initialize it. */
#endif /* TNCODE */
#endif /* COMMENT */
#ifdef SUNX25
	    if (network && nettype == NET_SX25) {
		obufl = 0;
		bzero (x25obuf,sizeof(x25obuf)) ;
	    }
#endif /* SUNX25 */

/*
  Here is the big loop that gets characters from the keyboard and sends them
  out the communication device.  There are two components to the communication
  path: the connection from the keyboard to C-Kermit, and from C-Kermit to
  the remote computer.  The treatment of the 8th bit of keyboard characters 
  is governed by SET COMMAND BYTESIZE (cmdmsk).  The treatment of the 8th bit
  of characters sent to the remote is governed by SET TERMINAL BYTESIZE
  (cmask).   This distinction was introduced in edit C-Kermit 5A(164).
*/
	    while (active) {
		c = coninc(0);		/* Get character from keyboard */
                if (c == -1) {		/* If read() got an error... */
#ifdef COMMENT
/*
 This seems to cause problems.  If read() returns -1, signal has already
 been delivered, and nothing will wake up the pause().
*/
		    pause();		/* Wait for transmitter to finish. */
#else
		    conoc(BEL);		/* Beep */
		    active = 0;		/* and terminate the read loop */
		    continue;
#endif /* COMMENT */
		}
		c &= cmdmsk;		/* Do any requested masking */
#ifndef NOICP
		c = keymap[c];		/* And keyboard mapping */
#endif /* NOICP */
		csave = c;		/* Save char before translation */
		if ((c & 0x7f) == escape) { /* Look for escape char */
		    debug(F000,"connect got escape","",c);
		    c = coninc(0) & 0177;   /* Got esc, get its arg */
#ifndef NOICP
		    c = keymap[c];	    /* Do keyboard mapping */
#endif /* NOICP */
		    doesc(c);		    /* And process it */
		} else {		/* Ordinary character */
#ifndef NOCSETS
		    /* Translate character sets */
		    if (sxo) c = (*sxo)(c); /* From local to intermediate. */
		    if (rxo) c = (*rxo)(c); /* From intermediate to remote. */
#endif /* NOCSETS */

/*
 If Shift-In/Shift-Out selected and we have a 7-bit connection,
 handle shifting here.
*/
		    if (sosi) {		     /* Shift-In/Out selected? */
			if (cmask == 0177) { /* In 7-bit environment? */
			    if (c & 0200) {          /* 8-bit character? */
				if (shift == 0) {    /* If not shifted, */
				    ttoc(dopar(SO)); /* shift. */
				    shift = 1;
				}
			    } else {
				if (shift == 1) {    /* 7-bit character */
				    ttoc(dopar(SI)); /* If shifted, */
				    shift = 0;       /* unshift. */
				}
			    }
			}
			if (c == SO) shift = 1;	/* User typed SO */
			if (c == SI) shift = 0;	/* User typed SI */
		    }
		    c &= cmask;		/* Apply Kermit-to-host mask now. */

#ifdef NETCONN
#ifdef SUNX25
                    if (network && nettype == NET_SX25) {
                        if (padparms[PAD_ECHO]) {
                            if (debses)
			      conol(dbchr(c)) ;
                            else
			      if ((c != padparms[PAD_CHAR_DELETE_CHAR])   &&
				  (c != padparms[PAD_BUFFER_DELETE_CHAR]) &&
				  (c != padparms[PAD_BUFFER_DISPLAY_CHAR]))
                                conoc(c) ;
                            if (seslog) logchar(c);
                        }
                        if (c == padparms[PAD_BREAK_CHARACTER])
			  breakact();
                        else if (padparms[PAD_DATA_FORWARD_TIMEOUT]) {
                            tosend = 1;
                            x25obuf [obufl++] = c;
                        } else if (((c == padparms[PAD_CHAR_DELETE_CHAR])  ||
				    (c == padparms[PAD_BUFFER_DELETE_CHAR]) ||
				    (c == padparms[PAD_BUFFER_DISPLAY_CHAR])) 
				   && (padparms[PAD_EDITING]))
			  if (c == padparms[PAD_CHAR_DELETE_CHAR])
			    if (obufl > 0) {
				conol("\b \b"); obufl--;
			    } else {}
			  else if (c == padparms[PAD_BUFFER_DELETE_CHAR]) {
			      conol ("\r\nPAD Buffer Deleted\r\n");
			      obufl = 0;
			  }
			  else if (c == padparms[PAD_BUFFER_DISPLAY_CHAR]) {
			      conol("\r\n");
			      conol(x25obuf);
			      conol("\r\n");
			  } else {} 
                        else {
                            x25obuf [obufl++] = c;
                            if (obufl == MAXOX25) tosend = 1;
                            else if (c == CR) tosend = 1;
                        }
                        if (tosend) 
			  if (ttol(x25obuf,obufl) < 0) {
			      perror ("\r\nCan't send characters");
			      active = 0;
			  } else {
			      bzero (x25obuf,sizeof(x25obuf));
			      obufl = 0;
			      tosend = 0;
			  } else {};
                    } else {
#endif /* SUNX25 */ 

/*
  This is for telnetting to IBM mainframes in linemode.
  Blank lines (when you hit two CRs in a row) are normally ignored.
  According to the telnet RFC, we must CR and CRLF.
  Also, if the user types the 0xff character, which happens to be the
  telnet IAC character, then it must be doubled.
*/
                    if (network && ttnproto == NP_TELNET) {
			if (c == '\015' && duplex != 0) {
			    ttoc(dopar('\015'));
			    conoc('\015');
			    csave = c = '\012';
			} else if (c == IAC && parity == 0)
			  ttoc(IAC);
		    } else
#endif /* NETCONN */
		    if (c == '\015' && tnlm) { /* TERMINAL NEWLINE ON ? */
			ttoc(dopar('\015'));   /* Send CR as CRLF */
			c = '\012';
		    }

		    /* Send the character that the user typed. */

		    if (ttoc(dopar(c)) > -1) {
		    	if (duplex) {	/* If half duplex, must echo */
			    if (debses)
			      conol(dbchr(csave));
			    else
			      conoc(csave);
			    if (seslog) { /* And maybe log it too */
				c2 = csave;
				if (sessft == 0 && csave == '\r')
				  c2 = '\n';
				logchar(c2);
			    }
			}
    	    	    } else {
			perror("\r\nCan't send character");
			active = 0;
		    }
#ifdef SUNX25
                  } 
#endif /* SUNX25 */
		}
	      }
    	    }				/* Come here on death of child */
	    kill((int)pid,9);		/* Done, kill inferior fork. */
#ifdef OXOS
/* See comments above about Olivetti X/OS. */
	    priv_off();
#endif /* OXOS */
	    wait((WAIT_T *)0);		/* Wait till gone. */
   	    if (sjval == 1) {		/* Read error on comm device */
		dohangup = 1;
#ifdef NETCONN
		if (network) ttclos(0);
#ifdef COMMENT
#ifdef TNCODE
		tn_init = 0;
#endif /* TNCODE */
#endif /* COMMENT */
#ifdef SUNX25
                if (network && nettype == NET_SX25) initpad();
#endif /* SUNX25 */
#endif /* NETCONN */
	    }
	    if (sjval == 2)		/* If it was a mode change, */
	      goto newfork;		/* go back & restart fork. */
	    conres();			/* Reset the console. */
	    if (quitnow) doexit(GOOD_EXIT,xitsta); /* Exit now if requested. */
	    if (dohangup) {		/* If hangup requested, do that. */
		tthang();		/* And make sure we don't hang up */
		dohangup = 0;		/* again unless requested again. */
	    }
#ifdef SUNX25
            if (dox25clr) {
                x25clear();
                initpad();
            }
#endif /* SUNX25 */
	    if (!quiet) printf("[Back at Local System]");
	    printf("\n");
	    what = W_NOTHING;		/* So console modes set right. */
#ifndef NOCSETS
	    language = langsv;		/* Restore language */
#endif /* NOCSETS */
	    parent_id = (PID_T) 0;
	    return(1);

	} else {			/* Inferior reads, prints port input */

	    if (priv_can()) {		/* Cancel all privs */
		printf("?setuid error - fatal\n");
		doexit(BAD_EXIT,-1);
	    }
	    signal(SIGINT, SIG_IGN);	/* In case these haven't been */
	    signal(SIGQUIT, SIG_IGN);	/* inherited from above... */

	    shift = 0;			/* Initial shift state. */
	    sleep(1);			/* Wait for parent's handler setup.  */
	    while (1) {			/* Fresh read, wait for a character. */
#ifdef SUNX25
                if (network && (nettype == NET_SX25)) {
                    bzero(x25ibuf,sizeof(x25ibuf)) ;
                    if ((ibufl = ttxin(MAXIX25,x25ibuf)) < 0) {
                        if (ibufl == -2) {  /* PAD parms changes */
                            write(padpipe[1],padparms,MAXPADPARMS);
                            kill(parent_id,SIGUSR2);
                        } else {
                            if (!quiet)
			      printf("\r\nCommunications disconnect ");
                            kill(parent_id,SIGUSR1);
                        }
                        pause();
                    }
                    if (debses) {	/* Debugging output */
                        p = x25ibuf ;
                        while (ibufl--) { c = *p++; conol(dbchr(c)); }
                    } else {
			if (sosi || tcsl != tcsr) { /* Character at a time */
			    for (i = 1; i < ibufl; i++) {
				c = x25ibuf[i] & cmask;
				if (sosi) { /* Handle SI/SO */
				    if (c == SO) {
					shift = 1;
					continue;
				    } else if (c == SI) {
					shift = 0;
					continue;
				    }
				    if (shift)
				      c |= 0200;
				}
#ifndef NOCSETS
				/* Translate character sets */
				if (sxi) c = (*sxi)(c);
				if (rxi) c = (*rxi)(c);
#endif /* NOCSETS */
				c &= cmdmsk; /* Apply command mask. */
				conoc(c);    /* Output to screen */
				logchar(c);  /* and session log */
			    }
			} else {	            /* All at once */
			    for (i = 1; i < ibufl; i++)
			      x25ibuf[i] &= (cmask & cmdmsk);
			    conxo(ibufl,x25ibuf);
			    if (seslog) zsoutx(ZSFILE,x25ibuf,ibufl);
			}
                    }
                    continue;

                } else {		/* Not X.25... */
#endif /* SUNX25 */

		if ((c = ttinc(0)) < 0) { /* Get a character from comm line  */
		    if (!quiet)
		      printf("\r\nCommunications disconnect "); /* if fail */
		    tthang();		/* hang up our side. */
#ifdef COMMENT
#ifdef TNCODE
		    tn_init = 0;
#endif /* TNCODE */
#endif /* COMMENT */
 		    if (c == -3) perror("\r\nCan't read character");
		    kill(parent_id,SIGUSR1); /* notify parent. */
		    for (;;) pause();	     /* Wait to be killed by parent. */
		}
#ifdef TNCODE
		/* Handle telnet options */	
		if (network && nettype == NP_TELNET && ((c & 0xff) == IAC)) {
		    if ((tx = tn_doop((c & 0xff),duplex)) == 0) {
			continue;
		    } else if (tx == -1) {
			if (!quiet)
			  printf("\r\nCommunications disconnect ");
			kill(parent_id,SIGUSR1); /* Notify parent. */
			for (;;) pause(); /* Wait to be killed. */
		    } else if ((tx == 1) && (!duplex)) {
			kill(parent_id,SIGUSR2);
			for (;;) pause();
		    } else if ((tx == 2) && (duplex)) {
			kill(parent_id,SIGUSR2);
			for (;;) pause();
		    } else continue;	/* Negotiation OK, go get next char. */
		}
#endif /* TNCODE */
		if (debses) {		/* Output character to screen */
		    conol(dbchr(c));	/* debugging */
		} else {		/* or regular... */
		    c &= cmask;		/* Do first masking */
		    if (sosi) {		/* Handle SI/SO */
			if (c == SO) {	/* Shift Out */
			    shift = 1;
			    continue;
			} else if (c == SI) { /* Shift In */
			    shift = 0;
			    continue;
			}
			if (shift) c |= 0200; 
		    }
#ifndef NOCSETS
		    /* Translate character sets */
		    if (sxi) c = (*sxi)(c);
		    if (rxi) c = (*rxi)(c);
#endif /* NOCSETS */
		    c &= cmdmsk;	/* Apply command mask. */
		    conoc(c);		/* Output to screen */
		    if (seslog) logchar(c); /* Take care of session log */
		}
#ifdef TNCODE
/* The following code makes CONNECT mode a lot more efficient, */
/* especially on slow workstations.  But we can't use it if we're */
/* doing telnet protocol because we won't notice any in-band telnet */
/* commands.  So this code will always be used on non-network systems */
/* and it will be used on network systems except during a telnet session. */
	    if (!network || (network && ttnproto != NP_TELNET)) {
#endif /* TNCODE */
		while ((n = ttchk()) > 0) {	/* Any more left in buffer? */
		    if (n > LBUFL) n = LBUFL;   /* Get them all at once. */
		    if ((n = ttxin(n,(CHAR *)lbuf)) > 0) {
			if (debses) {
			    p = lbuf;
			    while (n--) { c = *p++; conol(dbchr(c)); }
			    continue;
			} 

/* If doing SO/SI or Char Set Translation, must output character at a time */

			if (sosi
#ifndef NOCSETS
			    || tcsl != tcsr
#endif /* NOCSETS */
			    ) {
			    for (i = 0; i < n; i++) {
				c = lbuf[i] & cmask;
				if (sosi) { /* Handle SI/SO */
				    if (c == SO) {
					shift = 1;
					continue;
				    } else if (c == SI) {
					shift = 0;
					continue;
				    }
				    if (shift)
				      c = (c | 0200);
				}
#ifndef NOCSETS
				/* Translate character sets */
				if (sxi) c = (*sxi)(c);
				if (rxi) c = (*rxi)(c);
#endif /* NOCSETS */
				c &= cmdmsk; /* Apply command mask. */
				conoc(c);    /* Output to screen */
				if (seslog) logchar(c);
			    }
			} else {	/* All at once */
			    for (i = 0; i < n; i++) {
				c = lbuf[i] & cmask;
#ifndef NOCSETS
				/* Translate character sets */
				if (sxi) c = (*sxi)(c);
				if (rxi) c = (*rxi)(c);
#endif /* NOCSETS */
				lbuf[i] = c & cmdmsk; /* Replace in buffer. */
				if (seslog) logchar(c);	/* Log it. */
			    }
			    conxo(n,lbuf); /* Output whole buffer at once */
			}
		    }
		}
#ifdef TNCODE
	    }
#endif /* TNCODE */
#ifdef SUNX25
         }
#endif /* SUNX25 */	
      }
   }
}

/*  H C O N N E  --  Give help message for connect.  */

int
hconne() {
    int c;
    static char *hlpmsg[] = {"\
\r\n  C to return to the C-Kermit prompt, or:",
"\r\n  0 (zero) to send a null",
"\r\n  B to send a BREAK",
#ifdef SUNX25
"\r\n  I to send X.25 interrupt packet",
"\r\n  R to reset the virtual circuit",
"\r\n  H to hangup the connection (clear virtual circuit)",
#else
"\r\n  H to hangup and close the connection",
#endif /* SUNX25 */
"\r\n  Q to hangup and quit Kermit",
"\r\n  S for status",
"\r\n  ! to push to local shell",
"\r\n  Z to suspend",
"\r\n  \\ backslash escape:",
"\r\n    \\nnn decimal character code",
"\r\n    \\Onnn octal character code",
"\r\n    \\Xhh  hexadecimal character code",
"\r\n    terminate with carriage return.",
"\r\n  ? for help",
"\r\n escape character twice to send the escape character.\r\n\r\n",
"" };

    conola(hlpmsg);			/* Print the help message. */
    conol("Command>");			/* Prompt for command. */
    c = coninc(0) & 0177;		/* Get character, strip any parity. */
#ifndef NOICP
    c = keymap[c];			/* Do keyboard mapping. */
#endif /* NOICP */
    if (c != CMDQ)
      conoll("");
    return(c);				/* Return it. */
}

/*  D O E S C  --  Process an escape character argument  */

VOID
#ifdef CK_ANSIC
doesc(char c)
#else
doesc(c) char c;
#endif /* CK_ANSIC */
/* doesc */ {
    CHAR d;
  
    while (1) {
	if (c == escape) {		/* Send escape character */
	    d = dopar(c); ttoc(d); return;
    	} else				/* Or else look it up below. */
	    if (isupper(c)) c = tolower(c);

	switch(c) {

	case 'c':			/* Close connection */
	case '\03':
	    active = 0; conol("\r\n"); return;

	case 'b':			/* Send a BREAK signal */
	case '\02':
	    ttsndb(); return;

	case 'h':			/* Hangup */
	case '\010':
#ifdef SUNX25
            if (network && (nettype == NET_SX25)) dox25clr = 1;
            else
#endif /* SUNX25 */
	    dohangup = 1; active = 0; conol("\r\n"); return;

#ifdef SUNX25
        case 'i':                       /* Send an X.25 interrupt packet */
        case '\011':
            if (network && (nettype == NET_SX25)) (VOID) x25intr(0);
            conol("\r\n"); return;

        case 'r':                       /* Reset the X.25 virtual circuit */
        case '\022':
            if (network && (nettype == NET_SX25)) (VOID) x25reset();
            conol("\r\n"); return;
#endif /* SUNX25 */
 
	case 'q':
	    quitnow = 1; active = 0; conol("\r\n"); return;

	case 's':			/* Status */
	    conol("\r\nConnected thru ");
	    conol(ttname);
#ifdef SUNX25
            if (network && (nettype == NET_SX25))
                printf(", Link ID %d, LCN %d",linkid,lcn);
#endif /* SUNX25 */
	    if (speed >= 0L) {
		sprintf(temp,", speed %ld",speed); conol(temp);
	    }
	    sprintf(temp,", %d terminal bits",(cmask == 0177) ? 7 : 8);
	    conol(temp);
	    if (parity) {
		conol(", ");
		switch (parity) {
		    case 'e': conol("even");  break;
		    case 'o': conol("odd");   break;
		    case 's': conol("space"); break;
		    case 'm': conol("mark");  break;
		}
		conol(" parity");
	    }
	    if (seslog) {
		conol(", logging to "); conol(sesfil);
            }
	    conoll(""); return;

	case '?':			/* Help */
	    c = hconne(); continue;

	case '0':			/* Send a null */
	    c = '\0'; d = dopar(c); ttoc(d); return;

	case 'z': case '\032':		/* Suspend */
	    stptrap(0); return;

	case '@':			/* Start inferior command processor */
	case '!':
	    conres();			/* Put console back to normal */
	    zshcmd("");			/* Fork a shell. */
	    if (conbin(escape) < 0) {
		printf("Error returning to remote session\n");
		active = 0;
	    }
	    return;

	case SP:			/* Space, ignore */
	    return;

	default:			/* Other */
	    if (c == CMDQ) {		/* Backslash escape */
		int x;
		kbp = kbuf;
		*kbp++ = c;
		while (((c = (coninc(0) & cmdmsk)) != '\r') && (c != '\n'))
		  *kbp++ = c;
		*kbp = NUL; kbp = kbuf;
		x = xxesc(&kbp);
		if (x >= 0) {
		    c = dopar(x);
		    ttoc(c);
		    return;
		} else {		/* Invalid backslash code. */
		    conoc(BEL);
		    return;
		}
	    }
	    conoc(BEL); return; 	/* Invalid esc arg, beep */
    	}
    }
}

VOID
#ifdef CK_ANSIC
logchar(char c)
#else
logchar(c) char c;
#endif /* CK_ANSIC */
/* logchar */ {			/* Log character c to session log */
    if (seslog) 
      if ((sessft != 0) ||
	  (c != '\r' &&
	   c != '\0' &&
	   c != XON &&
	   c != XOFF))
	if (zchout(ZSFILE,c) < 0) {
	    conoll("");
	    conoll("ERROR WRITING SESSION LOG, LOG CLOSED!");
	    seslog = 0;
	}
}
