#ifndef NOCMDL
/*  C K U U S Y --  "User Interface" for Unix Kermit, part Y  */

/*  Command-Line Argument Parser */
 
/*
 Author: Frank da Cruz (fdc@columbia.edu, FDCCU@CUVMA.BITNET),
 Columbia University Center for Computing Activities.
 First released January 1985.
 Copyright (C) 1985, 1991, Trustees of Columbia University in the City of New 
 York.  Permission is granted to any individual or institution to use, copy, or
 redistribute this software so long as it is not sold for profit, provided this
 copyright notice is retained.
*/

#include "ckcdeb.h"
#include "ckcasc.h"
#include "ckcker.h"
#include "ckucmd.h"
#include "ckcnet.h"

#ifdef SUNX25
#include "ckcnet.h"
extern int revcall, closgr, cudata;
extern char udata[MAXCUDATA];
#endif /* SUNX25 */

extern char *ckxsys, *ckzsys, *cmarg, *cmarg2, **xargv, **cmlist;
extern int action, cflg, xargc, stdouf, stdinf, displa, cnflg, nfils,
  local, quiet, escape, network, mdmtyp, maxrps, rpsiz, bgset, xargs,
  urpsiz, wslotr, swcapr, binary, warn, parity, turn, turnch, duplex, flow,
  fncact, clfils, xitsta;
extern long speed, ttgspd(), zchki(), atol();
extern char ttname[];

#ifndef NODIAL
extern int nmdm;
extern struct keytab mdmtab[];
#endif



/*  C M D L I N  --  Get arguments from command line  */
/*
 Simple Unix-style command line parser, conforming with 'A Proposed Command
 Syntax Standard for Unix Systems', Hemenway & Armitage, Unix/World, Vol.1,
 No.3, 1984.
*/
int
cmdlin() {
    char x;				/* Local general-purpose int */
    cmarg = "";				/* Initialize globals */
    cmarg2 = "";
    action = cflg = 0;
 
/* If we were started directly from a Kermit application file, its name is */
/* in argv[1], so skip past it. */

    if (xargc > 1) {
	if (*xargv[0] != '-' && *xargv[1] != '-') {
	    if (
		/* some shells don't put full pathname... */
		/* zchki(xargv[0]) > 0 && */ /* ...so skip this test */
		zchki(xargv[1]) > 0) {	/* if it's an existing file */
		xargc -= 1;		/* skip past it */
		xargv += 1;		/* ... */
	    }
	}
    }

    while (--xargc > 0) {		/* Go through command line words */
	xargv++;
	debug(F111,"xargv",*xargv,xargc);
	if (**xargv == '=') return(0);
#ifdef VMS
	else if (**xargv == '/') continue;
#endif /* VMS */
    	else if (**xargv == '-') {	/* Got an option (begins with dash) */
	    x = *(*xargv+1);		/* Get the option letter */
	    if (doarg(x) < 0) doexit(BAD_EXIT,xitsta); /* Go handle option */
    	} else {			/* No dash where expected */
	    usage();
	    doexit(BAD_EXIT,xitsta);
	}
    }
    debug(F101,"action","",action);
    if (!local) {
	if ((action == 'c') || (cflg != 0))
	    fatal("-l and -b required");
    }
    if (*cmarg2 != 0) {
	if ((action != 's') && (action != 'r') &&
	    (action != 'v'))
	    fatal("-a without -s, -r, or -g");
    }
    if ((action == 'v') && (stdouf) && (!local)) {
    	if (isatty(1))
	    fatal("unredirected -k can only be used in local mode");
    }
    if ((action == 's') || (action == 'v') ||
    	(action == 'r') || (action == 'x')) {
	if (local) displa = 1;
	if (stdouf) { displa = 0; quiet = 1; }
    }
 
    if (quiet) displa = 0;		/* No display if quiet requested */
 
    if (cflg) {
	conect();			/* Connect if requested */
	if (action == 0) {
	    if (cnflg) conect();	/* And again if requested */
	    doexit(GOOD_EXIT,xitsta);	/* Then exit with return code */
	}
    }
    if (displa) concb(escape);		/* (for console "interrupts") */
    return(action);			/* Then do any requested protocol */
}

/*  D O A R G  --  Do a command-line argument.  */
 
int
#ifdef CK_ANSIC
doarg(char x)
#else
doarg(x) char x; 
#endif /* CK_ANSIC */
/* doarg */ {
    int i, y, z; long zz; char *xp;
 
    xp = *xargv+1;			/* Pointer for bundled args */
    while (x) {
	switch (x) {

case 'x':				/* server */
    if (action) fatal("conflicting actions");
    action = 'x';
    break;
 
case 'f':				/* finish */
    if (action) fatal("conflicting actions");
    action = setgen('F',"","","");
    break;
 
case 'r':				/* receive */
    if (action) fatal("conflicting actions");
    action = 'v';
    break;
 
case 'k':				/* receive to stdout */
    if (action) fatal("conflicting actions");
    stdouf = 1;
    action = 'v';
    break;
 
case 's': 				/* send */
    if (action) fatal("conflicting actions");
    if (*(xp+1)) fatal("invalid argument bundling after -s");
    nfils = z = 0;			/* Initialize file counter, flag */
    cmlist = xargv+1;			/* Remember this pointer */
    while (--xargc > 0) {		/* Traverse the list */	
	xargv++;
	if (**xargv == '-') {		/* Check for sending stdin */
	    if (strcmp(*xargv,"-") != 0) /* Watch out for next option. */
	      break;
	    z++;			/* "-" alone means send from stdin. */
        } else
#ifdef UNIX				/* Perhaps the ifdef can be removed? */
	if (zchki(*xargv) > -1)		/* Check if file exists */
#endif /* UNIX */
	  nfils++;			/* Bump file counter */
    }
    xargc++, xargv--;			/* Adjust argv/argc */
    if (nfils < 1 && z == 0) fatal("No files for -s");
    if (z > 1) fatal("-s: too many -'s");
    if (z == 1 && nfils > 0)
      fatal("invalid mixture of filenames and '-' in -s");
    if (nfils == 0) {
	if (isatty(0)) fatal("sending from terminal not allowed");
	else stdinf = 1;
    }

#ifdef COMMENT
    /* If only one filespec was given, indicate "internal list" rather than */
    /* "expanded list", so in case it includes wildcards, C-Kermit can */
    /* expand them itself. */
    if (nfils == 1) {
	cmarg = *cmlist;
	nfils = -1;
    }
#endif /* COMMENT */

    debug(F101,*xargv,"",nfils);
    action = 's';
#ifdef UNIX
/* When set, this flag tells Kermit not to expand wildcard characters. */
/* In UNIX, the shell has already expanded them.  In VMS, OS/2, etc, */
/* Kermit must expand them.  Kermit must not expand them in UNIX because */
/* a filename might itself contain metacharacters.  Imagine, for example, */
/* what would happen if a directory contained a file named "*". */
    clfils = 1;				/* Flag for command-line files */
#endif /* UNIX */
    break;
 
case 'g':				/* get */
    if (action) fatal("conflicting actions");
    if (*(xp+1)) fatal("invalid argument bundling after -g");
    xargv++, xargc--;
    if ((xargc == 0) || (**xargv == '-'))
    	fatal("missing filename for -g");
    cmarg = *xargv;
    action = 'r';
    break;
 
case 'c':				/* connect before */
    cflg = 1;
    break;
 
case 'n':				/* connect after */
    cnflg = 1;
    break;
 
case 'h':				/* help */
    usage();
    doexit(GOOD_EXIT,-1);
 
case 'a':				/* "as" */
    if (*(xp+1)) fatal("invalid argument bundling after -a");
    xargv++, xargc--;
    if ((xargc < 1) || (**xargv == '-'))
    	fatal("missing name in -a");
    cmarg2 = *xargv;
    break;
 
#ifndef NOICP
case 'y':				/* alternate init-file name */
    if (*(xp+1)) fatal("invalid argument bundling after -y");
    xargv++, xargc--;
    if (xargc < 1) fatal("missing name in -y");
    /* strcpy(kermrc,*xargv); ...this was already done in prescan()... */
    break;
#endif /* NOICP */

case 'l':				/* set line */
#ifdef NETCONN
case 'j':				/* set host (TCP/IP socket) */
#endif /* NETCONN */
    network = 0;
    if (*(xp+1)) fatal("invalid argument bundling after -l or -j");
    xargv++, xargc--;
    if ((xargc < 1) || (**xargv == '-'))
    	fatal("communication line device name missing");
    strcpy(ttname,*xargv);
    local = (strcmp(ttname,CTTNAM) != 0);
    if (x == 'l') {
	if (ttopen(ttname,&local,mdmtyp,0) < 0)
	  fatal("can't open device");
	if (speed < 0L)			/* If speed hasn't been set yet, */
	  speed = ttgspd();		/* get it. */
#ifdef NETCONN
    } else if (x == 'j') {
	mdmtyp = -1;
	if (ttopen(ttname,&local,mdmtyp,0) < 0)
	  fatal("can't open host connection");
	network = 1;
#endif /* NETCONN */
    }
    /* add more here later - decnet, etc... */
    break;
 
#ifdef SUNX25
case 'U':                               /* X.25 call user data */
    if (*(xp+1)) fatal("invalid argument bundling");
    xargv++, xargc--;
    if ((xargc < 1) || (**xargv == '-'))
        fatal("missing call user data string");
    strcpy(udata,*xargv);
    if (strlen(udata) <= MAXCUDATA) cudata = 1;
    else fatal("Invalid call user data");
    break;

case 'o':                               /* X.25 closed user group */
    if (*(xp+1)) fatal("invalid argument bundling");
    xargv++, xargc--;
    if ((xargc < 1) || (**xargv == '-'))
    	fatal("missing closed user group index");
    z = atoi(*xargv);			/* Convert to number */
    if (z >= 0 && z <= 99) closgr = z;
    else fatal("Invalid closed user group index");
    break;

case 'u':                               /* X.25 reverse charge call */
    revcall = 1;
    break;
#endif /* SUNX25 */

case 'b':   	    			/* set baud */
    if (*(xp+1)) fatal("invalid argument bundling");
    xargv++, xargc--;
    if ((xargc < 1) || (**xargv == '-'))
    	fatal("missing baud");
    zz = atol(*xargv);			/* Convert to long int */
    i = zz / 10L;
    if (ttsspd(i) > -1)			/* Check and set it */
      speed = zz;
    else
      fatal("unsupported transmission rate");
    break;
 
#ifndef NODIAL
case 'm':				/* Modem type */
    if (*(xp+1)) fatal("invalid argument bundling after -m");    
    xargv++, xargc--;
    if ((xargc < 1) || (**xargv == '-'))
    	fatal("modem type missing");
    y = lookup(mdmtab,*xargv,nmdm,&z);
    if (y < 0)
      fatal("unknown modem type");
    mdmtyp = y;
    break;
#endif

case 'e':				/* Extended packet length */
    if (*(xp+1)) fatal("invalid argument bundling after -e");
    xargv++, xargc--;
    if ((xargc < 1) || (**xargv == '-'))
    	fatal("missing length");
    z = atoi(*xargv);			/* Convert to number */
    if (z > 10 && z <= maxrps) {
        rpsiz = urpsiz = z;
	if (z > 94) rpsiz = 94;		/* Fallback if other Kermit can't */
    } else fatal("Unsupported packet length");
    break;

case 'v':				/* Vindow size */
    if (*(xp+1)) fatal("invalid argument bundling");
    xargv++, xargc--;
    if ((xargc < 1) || (**xargv == '-'))
    	fatal("missing or bad window size");
    z = atoi(*xargv);			/* Convert to number */
    if (z < 32) {			/* If in range */
	wslotr = z;			/* set it */
	if (z > 1) swcapr = 1;		/* Set capas bit if windowing */
    } else fatal("Unsupported packet length");
    break;

case 'i':				/* Treat files as binary */
    binary = 1;
    break;
 
case 'w':				/* Writeover */
    warn = 0;
    fncact = XYFX_X;
    break;
 
case 'q':				/* Quiet */
    quiet = 1;
    break;
 
#ifdef DEBUG
case 'd':				/* debug */
/** debopn("debug.log"); *** already did this in prescan() **/
    break;
#endif /* DEBUG */ 

case 'p':				/* set parity */
    if (*(xp+1)) fatal("invalid argument bundling");
    xargv++, xargc--;
    if ((xargc < 1) || (**xargv == '-'))
    	fatal("missing parity");
    switch(x = **xargv) {
	case 'e':
	case 'o':
	case 'm':
	case 's': parity = x; break;
	case 'n': parity = 0; break;
	default:  fatal("invalid parity");
        }
    break;
 
case 't':
    turn = 1;				/* Line turnaround handshake */
    turnch = XON;			/* XON is turnaround character */
    duplex = 1;				/* Half duplex */
    flow = 0;				/* No flow control */
    break;
 
#ifdef OS2
case 'u':
    /* get numeric argument */
    if (*(xp+1)) fatal("invalid argument bundling");
    *xargv++, xargc--;
    if ((xargc < 1) || (**xargv == '-'))
    	fatal("missing handle");
    z = atoi(*xargv);			/* Convert to number */
    ttclos(0);
    if (!ttiscom(z)) fatal("invalid handle");
    speed = ttspeed();
    break;
#endif /* OS2 */

case 'z':				/* Not background */
    bgset = 0;
    break;

default:
    fatal("invalid argument, type 'kermit -h' for help");
        }
 
    x = *++xp;				/* See if options are bundled */
    }
    return(0);
}
#else /* No command-line interface... */

extern int xargc;
int
cmdlin() {
    if (xargc > 1) printf("Sorry, command-line options disabled.\n");
    return(0);
}
#endif /* NOCMDL */
