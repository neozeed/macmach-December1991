char *versio = "C-Kermit 5A(174) ALPHA, 5 Nov 91"; /* Version herald. */
long vernum =            501174L;
/*
 String and numeric version numbers, keep these two in sync!
 Second digit of vernum: 0 = A, 1 = B, etc.
*/

#ifndef VERWHO
/* Change verwho in following line, or with -DVERWHO=x in makefile CFLAGS. */
#define VERWHO 0
#endif
int verwho = VERWHO; /* Who produced this version, 0 = Columbia University */
/*
 IMPORTANT: If you are working on your own private version of C-Kermit, please
 include some special notation, like your site name or your initials, in the
 "versio" string, e.g. "5A(159)-XXX", and use a nonzero code for the "verwho"
 variable (e.g. in the USA use your zip code).  Unless we stick to this
 discipline, divergent copies of C-Kermit will begin to appear that are
 intistinguishable from each other, which is a big support issue.  Also, if
 you have edited C-Kermit and made copies available to others, please add
 appropriate text to the BUG command (ckuus6.c, function dobug()).
*/
#define CKCMAI

/*  C K C M A I  --  C-Kermit Main program  */

/*
 Author: Frank da Cruz (fdc@columbia.edu, FDCCU@CUVMA.BITNET),
 Columbia University Center for Computing Activities.
 First released January 1985.
 Copyright (C) 1985, 1991, Trustees of Columbia University in the City of New 
 York.  Permission is granted to any individual or institution to use, copy, or
 redistribute this software as long as it is not sold for profit, provided this
 copyright notice is retained. 
*/
/*
 The Kermit file transfer protocol was developed at the Columbia University
 Center for Computing Activities (CUCCA).  It is named after Kermit the Frog,
 star of the television series THE MUPPET SHOW; the name is used by permission
 of Henson Associates, Inc.  "Kermit" is also Celtic for "free".
*/
/*
 Thanks to at least the following people for their contributions to this 
 program over the years:

   Chris Adie, Edinburgh U, Scotland (OS/2 support)
   Larry Afrin, Clemson U
   Barry Archer, U of Missouri
   Robert Andersson, Oslo, Norway
   Chris Armstrong, Brookhaven National Lab (OS/2)
   Fuat Baran, CUCCA
   Stan Barber, Rice U
   Karl Berry, UMB
   Charles Brooks, EDN
   Mike Brown, Purdue U
   Mark Buda, DEC (VAX/VMS)
   Bjorn Carlsson, Stockholm University Computer Center QZ
   Bill Catchings, formerly of CUCCA
   Bob Cattani, Columbia U CS Dept
   John L. Chmielewski, AT&T, Lisle, IL
   Howard Chu, U of Michigan
   Bill Coalson, McDonnell Douglas
   Alan Crosswell, CUCCA
   Jeff Damens, formerly of CUCCA
   Mark Davies, Bath U, UK
   Joe R. Doupnik, Utah State U
   S. Dezawa, Fujifilm, Japan
   Kristoffer Eriksson, Peridot Konsult AB, Oerebro, Sweden
   John R. Evans, IRS, Kansas City
   Glenn Everhart, RCA Labs
   Herm Fischer, Encino, CA (extensive contributions to version 4.0)
   Carl Fongheiser, CWRU
   Marcello Frutig, Catholic University, Sao Paulo, Brazil
   Andy Fyfe, Caltech
   Christine M. Gianone, CUCCA
   John Gilmore, UC Berkeley
   Yekta Gursel, MIT
   Jim Guyton, Rand Corp
   Rob Healey, rhealey@kas.helios.mn.org
   Christian Hemsing, RWTH Aachen, Germany
   Marion Kananson, hakanson@cs.orst.edu
   Stan Hanks, Rice U.
   Ken Harrenstein, SRI
   Chuck Hedrick, Rutgers U
   Ron Heiby, Motorola Micromputer Division
   Steve Hemminger, Tektronix
   Christian Hemsing, University of Technology, Aachen, Germany
   Andrew Herbert, Monash Univ, Australia
   Mike Hickey, ITI
   Randy Huntziger, National Library of Medicine
   Larry Jacobs, Transarc
   Peter Jones, U of Quebec Montreal
   Phil Julian, SAS Institute
   Sergey Kartashoff, Institute of..., Moscow, USSR
   Howie Kaye, CUCCA
   Rob Kedoin, Linotype Co, Hauppauge, NY (OS/2)
   Douglas Kingston, morgan.com
   Tom Kloos, Sequent Computer Systems
   Jim Knutson, U of Texas at Austin
   Thomas Krueger, UWM
   Bo Kullmar, Central Bank of Sweden, Kista
   John Kunze, UC Berkeley
   Bob Larson, USC (OS-9)
   Bert Laverman, Groningen U, Netherlands
   David Lawyer, UC Irvine
   S.O. Lidie, Lehigh U
   Tor Lillqvist, Helsinki University, Finland
   Kevin Lowey, U of Saskatchewan (OS/2)
   David MacKenzie, Environmental Defense Fund, University of Maryland
   John Mackin, University of Sidney, Australia
   Martin Maclaren, Bath U, UK
   Chris Maio, Columbia U CS Dept
   Fulvio Marino, Olivetti, Ivrea, Italy
   Peter Mauzey, AT&T
   Tye McQueen, Utah State U
   Hellmuth Michaelis,
   Leslie Mikesell, American Farm Bureau
   Martin Minow, DEC (VAX/VMS)
   Ray Moody, Purdue U
   Tony Movshon, NYU
   Dan Murphy, ???
   Gary Mussar,
   Jim Noble, Planning Research Corporation (Macintosh)
   Ian O'Brien, Bath U, UK
   Paul Placeway, Ohio State U (Macintosh & more)
   Piet W. Plomp, ICCE, Groningen University, Netherlands
   Ken Poulton, HP Labs
   Frank Prindle, NADC
   Anton Rang, ???
   Scott Ribe, ???
   Larry Rosenman
   Jay Rouman, U of Michigan
   Jack Rouse, SAS Institute (Data General and/or Apollo)
   Stew Rubenstein, Harvard U (VAX/VMS)
   Eric Schnoebelen, Convex
   Benn Schreiber, DEC
   Dan Schullman, DEC (modems, DIAL command, etc)
   Steven Schultz
   Gordon Scott, Micro Focus, Newbury UK
   David Sizeland, U of London Medical School
   Bradley Smith, UCLA
   Andy Tanenbaum, Vrije U, Amsterdam, Netherlands
   Markku Toijala, Helsinki U of Technology
   Warren Tucker, Tridom Corp, Mountain Park, GA
   Dave Tweten, AMES-NAS
   Walter Underwood, Ford Aerospace
   Pieter Van Der Linden, Centre Mondial, Paris
   Ge van Geldorp, Netherlands
   Fred van Kempen, MINIX User Group, Voorhout, Netherlands
   Wayne Van Pelt, GE/CRD
   Mark Vasoll & Gregg Wonderly, Oklahoma State U (V7 UNIX)
   Paul Vixie (DEC)
   Roger Wallace, Raytheon
   Stephen Walton, Calif State U, Northridge
   Jamie Watson, Switzerland
   Lauren Weinstein
   Joachim Wiesel, U of Karlsruhe
   Michael Williams, UCLA
   Patrick Wolfe, Kuck & Associates, Inc.
   Farrell Woods, Concurrent (formerly Masscomp)
   Dave Woolley, CAP Communication Systems, London
   Ken Yap, U of Rochester (Telnet support)
   John Zeeff, Ann Arbor, MI
*/

#include "ckcsym.h"			/* Macintosh once needed this */
#include "ckcasc.h"			/* ASCII character symbols */
#include "ckcdeb.h"			/* Debug & other symbols */
#include "ckcker.h"			/* Kermit symbols */
#include "ckcnet.h"			/* Network symbols */

/* Text message definitions.. each should be 256 chars long, or less. */
#ifdef MAC
char *hlptxt = "\r\
Mac Kermit Server Commands:\r\
\r\
    BYE\r\
    FINISH\r\
    GET filespec\r\
    REMOTE CD directory\r\
    REMOTE HELP\r\
    SEND filespec\r\
\r\0";
#else
#ifdef AMIGA
char *hlptxt = "C-Kermit Server Commands:\n\
\n\
GET filespec, SEND filespec, FINISH, BYE, REMOTE HELP\n\
\n\0";
#else
#ifdef OS2
char *hlptxt = "C-Kermit Server REMOTE Commands:\n\
\n\
GET files  REMOTE CD [dir]     REMOTE DIRECTORY [files]\n\
SEND files REMOTE SPACE [dir]  REMOTE HOST command\n\
FINISH     REMOTE DELETE files REMOTE TYPE files\n\
BYE        REMOTE HELP         REMOTE SET parameter value\n\
\n\0";
#else
#ifdef MINIX
char *hlptxt = "C-Kermit Server REMOTE Commands:\n\
GET SEND BYE FINISH REMOTE: CD DEL DIR HELP HOST SET SPACE TYPE WHO\n\0";
#else
#ifdef VMS
char *hlptxt = "C-Kermit Server REMOTE Commands:\r\n\
\r\n\
GET files  REMOTE CD [dir]     REMOTE DIRECTORY [files]\r\n\
SEND files REMOTE SPACE [dir]  REMOTE HOST command\r\n\
MAIL files REMOTE DELETE files REMOTE WHO [user]\r\n\
BYE        REMOTE PRINT files  REMOTE TYPE files\r\n\
FINISH     REMOTE HELP         REMOTE SET parameter value\r\n\
\0";
#else
char *hlptxt = "C-Kermit Server REMOTE Commands:\n\
\n\
GET files  REMOTE CD [dir]     REMOTE DIRECTORY [files]\n\
SEND files REMOTE SPACE [dir]  REMOTE HOST command\n\
MAIL files REMOTE DELETE files REMOTE WHO [user]\n\
BYE        REMOTE PRINT files  REMOTE TYPE files\n\
FINISH     REMOTE HELP         REMOTE SET parameter value\n\
\n\0";
#endif
#endif
#endif
#endif
#endif

#ifdef MINIX
char *srvtxt = "\r\n\
Entering server mode.\r\n\
\n\0";
#else
#ifdef OSK
char *srvtxt = "\r\l\
C-Kermit server starting.  Return to your local machine by typing\r\l\
its escape sequence for closing the connection, and issue further\r\l\
commands from there.  To shut down the C-Kermit server, issue the\r\l\
BYE command to logout, or the FINISH command and then reconnect.\r\l\
\l\0";
#else
char *srvtxt = "\r\n\
C-Kermit server starting.  Return to your local machine by typing\r\n\
its escape sequence for closing the connection, and issue further\r\n\
commands from there.  To shut down the C-Kermit server, issue the\r\n\
BYE command to logout, or the FINISH command and then reconnect.\r\n\
\r\n\0";
#endif
#endif

/* Declarations for Send-Init Parameters */

int spsiz = DSPSIZ,                     /* Current packet size to send */
    spmax = DSPSIZ,		/* (PWP) Biggest packet size we can send */
                                /* (see rcalcpsz()) */
    spsizr = DSPSIZ,			/* Send-packet size requested */
    spsizf = 0,                         /* Flag to override what you ask for */
    rpsiz = DRPSIZ,                     /* Biggest we want to receive */
    urpsiz = DRPSIZ,			/* User-requested rpsiz */
    maxrps = MAXRP,			/* Maximum incoming long packet size */
    maxsps = MAXSP,			/* Maximum outbound l.p. size */
    maxtry = MAXTRY,			/* Maximum retries per packet */
    wslots = 1,				/* Window size in use */
    wslotr = 1,				/* Window size requested */
    wslotn = 1,				/* Window size negotiated */
    timeouts = 0,			/* For statistics reporting */
    spackets = 0,			/*  ... */
    rpackets = 0,			/*  ... */
    retrans = 0,			/*  ... */
    crunched = 0,			/*  ... */
    wmax = 0,				/*  ... */
    timint = DMYTIM,                    /* Timeout interval I use */
    srvtim = DSRVTIM,			/* Server command wait timeout */
    srvdis = 0,				/* Server file xfer display */
    pkttim = DMYTIM,			/* Normal packet wait timeout */
    rtimo = URTIME,                     /* Timeout I want you to use */
    timef = 0,                          /* Flag to override what you ask */
    npad = MYPADN,                      /* How much padding to send */
    mypadn = MYPADN,                    /* How much padding to ask for */
    bctr = 1,                           /* Block check type requested */
    bctu = 1,                           /* Block check type used */
    ebq =  MYEBQ,                       /* 8th bit prefix */
    ebqflg = 0,                         /* 8th-bit quoting flag */
    rqf = -1,				/* Flag used in 8bq negotiation */
    rq = 0,				/* Received 8bq bid */
    sq = 'Y',				/* Sent 8bq bid */
    rpt = 0,                            /* Repeat count */
    rptq = MYRPTQ,                      /* Repeat prefix */
    rptflg = 0;                         /* Repeat processing flag */

int capas = 9,				/* Position of Capabilities */
    atcapb = 8,				/* Attribute capability */
    atcapr = 1,				/*  requested */
    atcapu = 0,				/*  used */
    swcapb = 4,				/* Sliding Window capability */
    swcapr = 0,				/*  requested */
    swcapu = 0,				/*  used */
    lpcapb = 2,				/* Long Packet capability */
    lpcapr = 1,				/*  requested */
    lpcapu = 0,				/*  used */
    lscapb = 32,			/* Locking Shift capability */
    lscapr = 1,				/*  requested by default */
    lscapu = 0;				/*  used */

/* Flags for whether to use particular attributes */

int atenci = 1,				/* Encoding in */
    atenco = 1,				/* Encoding out */
    atdati = 1,				/* Date in */
    atdato = 1,				/* Date out */
    atdisi = 1,				/* Disposition in/out */
    atdiso = 1,
    atleni = 1,				/* Length in/out (both kinds) */
    atleno = 1,
    atblki = 1,				/* Blocksize in/out */
    atblko = 1,
    attypi = 1,				/* File type in/out */
    attypo = 1,
    atsidi = 1,				/* System ID in/out */
    atsido = 1,
    atsysi = 1,			       /* System-dependent parameters in/out */
    atsyso = 1;

CHAR padch = MYPADC,                    /* Padding character to send */
    mypadc = MYPADC,                    /* Padding character to ask for */
    seol = MYEOL,                       /* End-Of-Line character to send */
    eol = MYEOL,                        /* End-Of-Line character to look for */
    ctlq = CTLQ,                        /* Control prefix in incoming data */
    myctlq = CTLQ;                      /* Outbound control character prefix */

struct zattr iattr;			/* Incoming file attributes */

/* File related variables, mainly for the benefit of VAX/VMS */

int fblksiz = DBLKSIZ;		/* File blocksize */
int frecl = DLRECL;		/* File record length */
int frecfm = XYFF_S;		/* File record format (default = stream) */
int forg = XYFO_S;		/* File organization (sequential) */
int fcctrl = XYFP_N;		/* File carriage control (ctrl chars) */

#ifdef VMS
/* VMS labeled file options */
int lf_opts = LBL_NAM;
#else
int lf_opts = 0;
#endif /* VMS */

/* Packet-related variables */

int pktnum = 0,                         /* Current packet number */
    sndtyp,                             /* Type of packet just sent */
    rsn,				/* Received packet sequence number */
    rln,				/* Received packet length */
    size,                               /* Current size of output pkt data */
    osize,                              /* Previous output packet data size */
    maxsize,                            /* Max size for building data field */
    spktl = 0,				/* Length packet being sent */
    rprintf,				/* REMOTE PRINT flag */
    rmailf;				/* MAIL flag */

CHAR
#ifdef NO_MORE  /* Buffers used before sliding windows... */
    sndpkt[MAXSP+100],			/* Entire packet being sent */
    recpkt[MAXRP+200],			/* Packet most recently received */
    data[MAXSP+4],			/* Packet data buffer */
#endif
#ifdef DYNAMIC
    *srvcmd = (CHAR *)0,		/* Where to decode server command */
#else
    srvcmd[MAXRP+4],                    /* Where to decode server command */
#endif
    padbuf[95],				/* Buffer for send-padding */
    *recpkt,
    *rdatap,				/* Pointer to received packet data */
    *data = (CHAR *)0,			/* Pointer to send-packet data */
    *srvptr,                            /* Pointer to srvcmd */
    mystch = SOH,                       /* Outbound packet-start character */
    stchr = SOH;                        /* Incoming packet-start character */

/* File-related variables */

char filnam[256];                       /* Name of current file. */
char cmdfil[80];			/* Application file name. */

int nfils = 0;				/* Number of files in file group */
long fsize;                             /* Size of current file */
int wildxpand = 0;			/* Who expands wildcards */
int clfils = 0;				/* Flag for command-line files */

/* Communication line variables */

char ttname[50];                        /* Name of communication line. */

long speed = -1L;			/* Line speed */
int mdmspd = 1;				/* Modem changes speed flag */

#ifndef NODIAL
int dialhng = 1;			/* Whether to hangup before dialing */
int dialtmo = 0;			/* Secs to wait for call to complete */
int dialksp = 0;			/* Kermit spoof in modem */
int dialdpy = 0;			/* Dial display */
int dialmnp = 0;			/* Dial MNP-negotiation enabled */
char *dialini = (char *) 0;		/* User-spec'd dialer init string */
#endif /* NODIAL */

int parity,                             /* Parity specified, 0,'e','o',etc */
    autopar = 0,			/* Automatic parity change flag */
    sosi = 0,				/* Shift-In/Out flag */
    flow,                               /* Flow control */
    turn = 0,                           /* Line turnaround handshake flag */
    turnch = XON,                       /* Line turnaround character */
    duplex = 0,                         /* Duplex, full by default */
    escape = DFESC,			/* Escape character for connect */
    delay = DDELAY,                     /* Initial delay before sending */
    tnlm = 0,				/* Terminal newline mode */
    mdmtyp = 0;                         /* Modem type (initially none)  */

/* Networks for SET HOST */

    int network = 0;			/* Network vs tty connection */
    int nettype = NET_TCPB;		/* Assume TCP/IP (BSD sockets) */

#ifdef SUNX25
    extern initpad();
    int revcall = 0;            /* X.25 reverse call not selected */
    int closgr  = -1;		/* X.25 closed user group not selected */
    int cudata = 0;		/* X.25 call user data not specified */
    char udata[MAXCUDATA];	/* X.25 call user data */
#endif /* SUNX25 */

/* Other recent additions */

    int tlevel = -1;			/* Take-file command level */
    int carrier = CAR_AUT;		/* Pay attention to carrier signal */
    int cdtimo = 0;			/* Carrier wait timeout */
    int xitsta = GOOD_EXIT;		/* Program exit status */
#ifdef VMS				/* Default filename collision action */
    int fncact = XYFX_X;		/* REPLACE for VAX/VMS */
#else
    int fncact = XYFX_B;		/* BACKUP for everybody else */
#endif /* VMS */
    int bgset = -1;			/* BACKGROUND mode set explicitly */
#ifdef UNIX
    int suspend = DFSUSP;		/* Whether SUSPEND command, etc, */
#else					/* is to be allowed. */
    int suspend = 0;
#endif /* UNIX */

/* Statistics variables */

long filcnt,                    /* Number of files in transaction */
    flci,                       /* Characters from line, current file */
    flco,                       /* Chars to line, current file  */
    tlci,                       /* Chars from line in transaction */
    tlco,                       /* Chars to line in transaction */
    ffc,                        /* Chars to/from current file */
    tfc,                        /* Chars to/from files in transaction */
    rptn;			/* Repeated characters compressed */

int tsecs;                      /* Seconds for transaction */

/* Flags */

int deblog = 0,                         /* Flag for debug logging */
    debses = 0,				/* Flag for DEBUG SESSION */
    pktlog = 0,                         /* Flag for packet logging */
    seslog = 0,                         /* Session logging */
    tralog = 0,                         /* Transaction logging */
    displa = 0,                         /* File transfer display on/off */
    stdouf = 0,                         /* Flag for output to stdout */
    stdinf = 0,				/* Flag for input from stdin */
    xflg   = 0,                         /* Flag for X instead of F packet */
    hcflg  = 0,                         /* Doing Host command */
    fncnv  = 1,                         /* Flag for file name conversion */
    binary = 0,                         /* Flag for binary file */
    savmod = 0,                         /* Saved file mode (whole session) */
    bsave  = 0,				/* Saved file mode (per file) */
    bsavef = 0,				/* Flag if bsave was used. */
    cmask  = 0177,			/* Connect byte mask */
    fmask  = 0377,			/* File byte mask */
    warn   = 0,                         /* Flag for file warning */
    quiet  = 0,                         /* Be quiet during file transfer */
    local  = 0,                         /* Flag for external tty vs stdout */
    server = 0,                         /* Flag for being a server */
    cflg   = 0,				/* Connect before transaction */
    cnflg  = 0,                         /* Connect after transaction */
    cxseen = 0,                         /* Flag for cancelling a file */
    czseen = 0,                         /* Flag for cancelling file group */
    discard = 0,			/* Flag for file to be discarded */
    keep = 0,                           /* Keep incomplete files */
    unkcs = 1,				/* Keep file w/unknown character set */
    nakstate = 0,			/* In a state where we can send NAKs */
    dblchar = -1;			/* Character to double when sending */

/* Variables passed from command parser to protocol module */

#ifndef NOICP
int parser();                           /* The parser itself */
#endif

CHAR sstate  = (CHAR) 0;                /* Starting state for automaton */
char *cmarg  = "";                      /* Pointer to command data */
char *cmarg2 = "";                      /* Pointer to 2nd command data */
char **cmlist;                          /* Pointer to file list in argv */

/* Flags for the ENABLE and DISABLE commands */

int en_cwd = 1;				/* CD/CWD */
int en_del = 1;				/* DELETE */
int en_dir = 1;				/* DIRECTORY */
int en_fin = 1;				/* FINISH/BYE */
int en_get = 1;				/* GET */
int en_hos = 1;				/* HOST */
int en_sen = 1;				/* SEND */
int en_set = 1;				/* SET */
int en_spa = 1;				/* SPACE */
int en_typ = 1;				/* TYPE */
int en_who = 1;				/* WHO */
int en_bye = 1;				/* BYE */

/* Miscellaneous */

char **xargv;                           /* Global copies of argv */
int  xargc;                             /* and argc  */
int xargs;				/* an immutable copy of argc */
char *xarg0;				/* and of argv[0] */

extern char *dftty;                     /* Default tty name from ck?tio.c */
extern int dfloc;                       /* Default location: remote/local */
extern int dfprty;                      /* Default parity */
extern int dfflow;                      /* Default flow control */

/*
  Buffered file input and output buffers.  See getpkt() in ckcfns.c
  and zoutdump() in the system-dependent file i/o module (usually ck?fio.c).
*/
#ifndef DYNAMIC
/* Now we allocate them dynamically, see getiobs() below. */
CHAR zinbuffer[INBUFSIZE], zoutbuffer[OBUFSIZE];
#endif
CHAR *zinptr, *zoutptr;
int zincnt, zoutcnt;

_PROTOTYP( int getiobs, () );

/*  M A I N  --  C-Kermit main program  */

#ifndef NOCCTRAP
#include <setjmp.h>
extern jmp_buf cmjbuf;
#endif

#ifdef aegis
/* On the Apollo, intercept main to insert a cleanup handler */
int
ckcmai(argc,argv) int argc; char **argv;
#else
int
main(argc,argv) int argc; char **argv;
#endif /* aegis */
{

/* Do some initialization */

    if (sysinit() < 0)			/* System-dependent initialization. */
      fatal("Can't initialize!");
    connoi();				/* Console interrupts off */
    xargc = xargs = argc;		/* Make global copies of argc */
    xargv = argv;                       /* ...and argv. */
    xarg0 = argv[0];
    sstate = 0;                         /* No default start state. */
#ifdef DYNAMIC
    if (getiobs() < 0) 
      fatal("Can't allocate i/o buffers!");
#endif
    strcpy(ttname,dftty);               /* Set up default tty name. */
    local = dfloc;                      /* And whether it's local or remote. */
    parity = dfprty;                    /* Set initial parity, */
    flow = dfflow;                      /* and flow control. */
    if (local) if (ttopen(ttname,&local,0,0) < 0) { /* If default tty line */
	printf("?Can't open %s\n",ttname);          /* is external, open it */
	local = 0;			            /* now... */
	strcpy(ttname,CTTNAM);
    }
    speed = ttgspd();			/* Get transmission speed. */

#ifdef SUNX25
    initpad();                          /* Initialize X.25 PAD */
#endif /* SUNX25 */

    if (inibufs(SBSIZ,RBSIZ) < 0)	/* Allocate packet buffers */
      fatal("Can't allocate packet buffers!");

#ifndef NOICP

#ifdef COMMENT
#ifdef DCMDBUF
    if (cmsetup() < 0) fatal("Can't allocate command buffers!");
#endif /* DCMDBUF */
#ifndef NOSPL
    if (macini() < 0) fatal("Can't allocate macro buffers!");
#endif /* NOSPL */
#endif /* COMMENT */
/* Attempt to take ini file before doing command line */

    *cmdfil = '\0';			/* Assume no command file. */
    prescan();				/* But first check for -y option */

#ifndef NOCCTRAP
    setint();				/* Set up interrupts */
    if (setjmp(cmjbuf)) {		/* Control-C trap returns to here. */
	doexit(GOOD_EXIT,-1);		/* Exit with good status. */
    } else {
#endif /* NOCCTRAP */
	cmdini();			/* Sets tlevel */
	while (tlevel > -1) {		/* Execute init file. */
	    sstate = parser(0);		/* Loop getting commands. */
	    if (sstate) proto();	/* Enter protocol if requested. */
	}
#ifndef NOCCTRAP
    }
#endif

/*
  In UNIX there are two ways to invoke Kermit with a cmdfile:
  (1) From the kermit command line, e.g. "kermit cmdfile [ options... ]"
      argv[0] = "kermit"
      argv[1] = "cmdfile"
  (2) By executing a cmdfile whose first line is like "#!/path/kermit"
      argv[0] = "/path/kermit" (sometimes just "kermit")
      argv[1] = "/path/cmdfile"
*/
    if (argc > 1) {
	if (*argv[1] != '-') {
	    if (zchki(argv[1]) > 0) {
		strcpy(cmdfil,argv[1]);
	    }
	}
    }
    if (*cmdfil) {			/* If we got one, */
	dotake(cmdfil);			/* execute it */
	while (tlevel > -1) {		/* until it runs out. */
	    sstate = parser(0);		/* Loop getting commands. */
	    if (sstate) proto();	/* Enter protocol if requested. */
	}
    }
    *cmdfil = '\0';
#endif /* NOICP */

#ifndef NOCMDL
/* Look for a UNIX-style command line... */

    if (argc > 1) {                     /* Command line arguments? */
        sstate = cmdlin();              /* Yes, parse. */
        if (sstate) {
#ifndef NOCCTRAP
	    setint();			/* Set up interrupts */
	    if (setjmp(cmjbuf)) {	/* Control-C trap returns to here. */
		if (cnflg) conect();	/* connect if requested, */
		doexit(GOOD_EXIT,xitsta); /* and then exit with good status */
	    } else {
#endif
		proto();		/* Take any requested action, then */
		if (!quiet)		/* put cursor back at left margin, */
		  conoll("");
		if (cnflg) conect();	/* connect if requested, */
		doexit(GOOD_EXIT,xitsta); /* and then exit with status 0. */
#ifndef NOCCTRAP
	    }
#endif
        }
    }
#endif /* NOCMDL */

#ifdef NOICP				/* No interactive command parser */
    else {
#ifndef NOCMDL
	usage();			/* Command-line-only version */
	doexit(BAD_EXIT,-1);
#else					/* Neither one! */
#ifdef MAC				/* Mac has its own alternative */
	while(1) {			/* command parser */
	    sstate = parser(0);
	    if (sstate) proto();
	}
#else  /* not MAC */
	doexit(BAD_EXIT,-1);
#endif /* MAC */	
#endif /* NOCMDL */
    }
#else /* not NOICP */
/* If no action requested on command line, enter interactive parser */

    herald();				/* Display program herald. */

#ifndef NOCCTRAP			/* If not no Control-C trap */
ccagain:
    if (setjmp(cmjbuf)) {		/* Control-C trap returns to here. */
	fixcmd();			/* Pop command stacks, etc. */
	debug(F100,"ckcmai got interrupt","",0);
	goto ccagain;			/* set up trap again. */
    } else {
	debug(F100,"ckcmai setting interrupt trap","",0);
	setint();			/* Set up command interrupt traps */
    }
#else /* NOCCTRAP */
    setint();				/* Set up command interrupt traps */
#endif /* NOCCTRAP */
    if (*cmdfil) dotake(cmdfil);	/* Command file spec'd on cmd line */
    while(1) {				/* Loop getting commands. */
	sstate = parser(0);
        if (sstate) proto();            /* Enter protocol if requested. */
    }
#endif /* NOICP */
}

#ifdef DYNAMIC
/* Allocate file i/o buffers */

CHAR *zinbuffer, *zoutbuffer;

int
getiobs() {
    zinbuffer = (CHAR *)malloc(INBUFSIZE);
    if (!zinbuffer) return(-1);
    zoutbuffer = (CHAR *)malloc(OBUFSIZE);
    if (!zoutbuffer) return(-1);    
    debug(F100,"getiobs ok","",0);
    return(0);
}
#endif /* DYNAMIC */
