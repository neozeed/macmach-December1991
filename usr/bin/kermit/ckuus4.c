#ifndef NOICP

/*  C K U U S 4 --  "User Interface" for Unix Kermit, part 4  */
 
/*
 Author: Frank da Cruz (fdc@columbia.edu, FDCCU@CUVMA.BITNET),
 Columbia University Center for Computing Activities.
 First released January 1985.
 Copyright (C) 1985, 1991, Trustees of Columbia University in the City of New 
 York.  Permission is granted to any individual or institution to use, copy, or
 redistribute this software so long as it is not sold for profit, provided this
 copyright notice is retained.
*/

/*
  File ckuus4.c -- Functions moved from other ckuus*.c modules to even
  out their sizes.
*/
#include "ckcdeb.h"
#include "ckcasc.h"
#include "ckcker.h"
#include "ckuusr.h"
#include "ckcnet.h"			/* Network symbols */

#ifndef NOCSETS
#include "ckcxla.h"
#endif /* NOCSETS */

#ifndef AMIGA
#include <signal.h>
#include <setjmp.h>
#endif /* AMIGA */

#ifdef OS2
#define SIGALRM SIGUSR1
void alarm( unsigned );
#endif

#ifdef SUNX25
extern int revcall, closgr, cudata, npadx3;
int x25ver;
extern char udata[MAXCUDATA];
extern CHAR padparms[MAXPADPARMS+1];
extern struct keytab padx3tab[];
#endif /* SUNX25 */ 

#ifndef NOSPL
char *months[] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};
#endif /* NOSPL */

#ifdef UNIX
extern int ttyfd;
#endif

extern CHAR mystch, stchr, eol, seol, padch, mypadc, ctlq;
extern CHAR *data, *rdatap;
extern char *ckxsys, *ckzsys, *cmarg, *cmarg2, **xargv, **cmlist;
extern char ttname[];
#ifndef NOSPL
extern char fspec[];
#endif /* NOSPL */
#ifdef DCMDBUF
extern char *cmdbuf;
#else
extern char cmdbuf[];
#endif /* DCMDBUF */
extern char  line[],
  pktfil[],
#ifdef DEBUG
  debfil[],
#endif
#ifdef TLOG
  trafil[],
#endif
  sesfil[];

#ifndef NOXMIT
extern char xmitbuf[];
extern int xmitf, xmitl, xmitp, xmitx, xmits, xmitw;
#endif /* NOXMIT */

#ifndef NOSPL
extern char **a_ptr[];
extern int a_dim[];
extern char inpbuf[];			/* Buffer for INPUT and REINPUT */
extern char *inpbp;			/* And pointer to same */
char *inpbps = inpbuf;			/* And another */
extern struct mtab *mactab;
extern char *mrval[];
extern int macargc[], count[], cmdlvl;
#endif /* NOSPL */

extern char flfnam[];
extern char kermrc[];

extern struct keytab cmdtab[], colxtab[];
extern int ncmd, network, nettype, ttnproto, fncact, carrier, cdtimo, success;
extern int rcflag, haslock, swcapr, autopar, bgset, maxnam, maxpath, ncolx;
extern int escape, nmac, mdmspd, lscapr, lscapu;
extern int action, cflg, xargc, stdouf, stdinf, displa, cnflg, nfils, cnflg;
extern int nrmt, nprm, dfloc, seslog, local, parity, duplex, xargs;
extern int turn, turnch, pktlog, tralog, flow, cmask, timef, spsizf, sessft;
extern int rtimo, timint, srvtim, npad, mypadn, bctr, delay, pkttim, spsizr;
extern int maxtry, spsiz, urpsiz, maxsps, maxrps, ebqflg, ebq;
extern int rptflg, rptq, fncnv, binary, pktlog, warn, quiet, fmask, keep;
extern int tsecs, bctu, len, atcapu, lpcapu, swcapu, sq, rpsiz;
extern int wslots, wslotn, wslotr;
extern int capas, atcapr;
extern int spackets, rpackets, timeouts, retrans, crunched, wmax;
extern int indef, intime, incase, inecho;
extern int cmask, zincnt, fblksiz, frecl, frecfm, forg, fcctrl, sosi;
extern int bigsbsiz, bigrbsiz;

extern CHAR *zinptr;
extern long filcnt, tfc, tlci, tlco, ffc, flci, flco, rptn, atol();
extern long vernum, speed;
extern char *dftty, *versio, *ckxsys;
extern struct keytab prmtab[];
extern struct keytab remcmd[];

extern int mdmtyp;

#ifndef NODIAL
extern int nmdm, dialhng, dialtmo, dialksp, dialdpy, dialmnp;
extern char *dialini, *dialnum;
extern struct keytab mdmtab[];
#endif

#ifndef NOSPL
/* Macro stuff */
extern int maclvl;
extern char *m_arg[MACLEVEL][10]; /* You have to put in the dimensions */
extern char *g_var[GVARS];	  /* for external 2-dimensional arrays. */
#endif /* NOSPL */

#ifndef NOCSETS
/* Translation stuff */
extern int fcharset, tcharset, tslevel, language, nlng, tcsr, tcsl;
extern struct keytab lngtab[];
extern struct csinfo fcsinfo[], tcsinfo[];
extern struct langinfo langs[];
extern CHAR (*xls[MAXTCSETS+1][MAXFCSETS+1])();	/* Character set */
extern CHAR (*xlr[MAXTCSETS+1][MAXFCSETS+1])();	/* translation functions. */
#endif /* NOCSETS */

#ifndef NOSPL
/* Built-in variable names, maximum length VNAML (20 characters) */

struct keytab vartab[] = {
    "argc",      VN_ARGC,  0,
    "args",      VN_ARGS,  0,
    "count",     VN_COUN,  CM_INV,
    "date",      VN_DATE,  0,
    "directory", VN_DIRE,  0,
    "filespec",  VN_FILE,  0,
    "home",      VN_HOME,  0,
    "host",      VN_HOST,  0,
    "input",     VN_IBUF,  0,
    "line",      VN_LINE,  0,
    "ndate",     VN_NDAT,  0,
    "ntime",     VN_NTIM,  0,
    "platform",  VN_SYSV,  0,
    "program",   VN_PROG,  0,
    "return",    VN_RET,   0,
    "speed",     VN_SPEE,  0,
    "status",    VN_SUCC,  0,
    "system",    VN_SYST,  0,
    "time",      VN_TIME,  0,
#ifdef UNIX
    "ttyfd",     VN_TTYF,  0,
#endif
    "version",   VN_VERS,  0
};
int nvars = (sizeof(vartab) / sizeof(struct keytab));
#endif /* NOSPL */

#ifndef NOSPL
struct keytab fnctab[] = {		/* Function names */
    "character",  FN_CHR, 0,		/* Character */
    "contents",   FN_CON, 0,		/* Definition (contents) of variable */
    "definition", FN_DEF, 0,		/* Return definition of given macro */
    "evaluate",   FN_EVA, 0,		/* Evaluate given arith expression */
    "execute",    FN_EXE, 0,		/* Execute given macro */
    "files",      FN_FC,  0,		/* File count */
    "index",      FN_IND, 0,		/* Index (string search) */
    "length",     FN_LEN, 0,		/* Return length of argument */
    "literal",    FN_LIT, 0,		/* Return argument literally */
    "lower",      FN_LOW, 0,		/* Return lowercased argument */
    "lpad",       FN_LPA, 0,		/* Return left-padded argument */
    "maximum",    FN_MAX, 0,		/* Return maximum of two arguments */
    "minimim",    FN_MIN, 0,		/* Return minimum of two arguments */ 
#ifdef COMMENT
/* not needed because \feval() has it */
    "modulus",    FN_MOD, 0,		/* Return modulus of two arguments */ 
#endif
    "nextfile",   FN_FIL, 0,		/* Next file in list */
    "repeat",     FN_REP, 0,		/* Repeat argument given # of times */
    "reverse",    FN_REV, 0,		/* Reverse the argument string */
    "rpad",       FN_RPA, 0,		/* Right-pad the argument */
    "substring",  FN_SUB, 0,		/* Extract substring from argument */
    "upper",      FN_UPP, 0		/* Return uppercased argument */
};
int nfuncs = (sizeof(fnctab) / sizeof(struct keytab));
#endif /* NOSPL */

#ifndef NOSPL
#define VVBUFL 60
char vvbuf[VVBUFL];
#endif /* NOSPL */

struct keytab disptb[] = {
    "append",    1,  0,
    "new",       0,  0
};

/*** The following functions moved here from ckuus2.c because that module ***/
/*** got too big... ***/

/*  P R E S C A N -- Quick look thru command-line args for init file name */
/*  Also for -d (debug) and -z (force foreground) */

VOID
prescan() {
    int yargc; char **yargv;
    char x;

    yargc = xargc;
    yargv = xargv;
    strcpy(kermrc,KERMRC);		/* Default init file name */
#ifndef NOCMDL
    while (--yargc > 0) {		/* Look for -y on command line */
	yargv++;
	if (**yargv == '-') {		/* Option starting with dash */
	    x = *(*yargv+1);		/* Get option letter */
	    if (x == 'y') {		/* Is it y? */
		yargv++, yargc--;	/* Yes, count and check argument */
		if (yargc < 1) fatal("missing name in -y");
		strcpy(kermrc,*yargv);	/* Replace init file name */
		rcflag = 1;		/* Flag that this has been done */
		continue;
	    } else if (x == 'd') {	/* Do this early as possible! */
		debopn("debug.log",0);
		continue;
	    } else if (x == 'z') {	/* = SET BACKGROUND OFF */
		bgset = 0;
		continue;
	    }
	} 
    }
#endif /* NOCMDL */
}


int tr_int;				/* Flag if TRANSMIT interrupted */

SIGTYP
trtrap(foo) int foo; {			/* TRANSMIT interrupt trap */
    tr_int = 1;				/* (Need arg for ANSI C) */
    return;
}

#ifndef NOXMIT
/*  T R A N S M I T  --  Raw upload  */

/*  Obey current line, duplex, parity, flow, text/binary settings. */
/*  Returns 0 upon apparent success, 1 on obvious failure.  */

/***
 Things to add:
 . Make both text and binary mode obey set file bytesize.
 . Maybe allow user to specify terminators other than CR?
 . Maybe allow user to specify prompts other than single characters?
***/

/*  T R A N S M I T  --  Raw upload  */

/*  s is the filename, t is the turnaround (prompt) character  */

/*
  Maximum number of characters to buffer.
  Must be less than LINBUFSIZ
*/
#define XMBUFS 120

int
#ifdef CK_ANSIC
transmit(char * s, char t)
#else
transmit(s,t) char *s; char t;
#endif /* CK_ANSIC */
/* transmit */ {
#ifndef OS2
    SIGTYP (* oldsig)();		/* For saving old interrupt trap. */
#endif /* OS2 */
    long zz;
    int z = 1;				/* Return code. 0=fail, 1=succeed. */
    int x, c, i;			/* Workers... */
    char *p;

#ifndef NOCSETS
    int tcs;			/* Intermediate (xfer) char set */
    CHAR (*sxo)();		/* Local translation functions */
    CHAR (*rxo)();		/* for output (sending) terminal chars */
    int langsv;			/* Save current language */
#endif /* NOCSETS */

    if (zopeni(ZIFILE,s) == 0) {	/* Open the file to be transmitted */
	printf("?Can't open %s\n",s);
	return(0);
    }
    x = -1;				/* Open the communication line */
    if (ttopen(ttname,&x,mdmtyp,cdtimo) < 0) {	/* (no harm if already open) */
	printf("Can't open %s\n",ttname);
	return(0);
    }
    zz = x ? speed : -1L;
    if (binary) {			/* Binary file transmission */
	if (ttvt(zz,flow) < 0) {	/* So no Xon/Xoff! */
	    printf("Can't condition line\n");
	    return(0);
	}	
    } else {
	if (ttpkt(zz,flow,parity) < 0) { /* Put the line in "packet mode" */
	    printf("Can't condition line\n"); /* so Xon/Xoff will work, etc. */
	    return(0);
	}
    }

#ifndef NOCSETS
/* Set up character set translations */
    if (binary == 0) {
#ifdef KANJI
/* Kanji not supported yet */
	if (fcsinfo[tcsr].alphabet == AL_JAPAN ||
	    fcsinfo[tcsl].alphabet == AL_JAPAN )
	  tcs = TC_TRANSP;
	else
#endif /* KANJI */
#ifdef CYRILLIC
      if (fcsinfo[tcsl].alphabet == AL_CYRIL)
	tcs = TC_CYRILL;
      else
#else
	tcs = TC_1LATIN;
#endif /* CYRILLIC */

	if (tcsr == tcsl || binary) {	/* Remote and local sets the same? */
	    sxo = rxo = NULL;		/* Or file type is not text? */
	} else {			/* Otherwise, set up */
	    sxo = xls[tcs][tcsl];	/* translation function */
	    rxo = xlr[tcs][tcsr];	/* pointers for output functions */
	}
/*
  This is to prevent use of zmstuff() and zdstuff() by translation functions.
  They only work with disk i/o, not with communication i/o.  Luckily Russian
  translation functions don't do any stuffing...
*/
	langsv = language;
	language = L_USASCII;
    }
#endif /* NOCSETS */

    i = 0;				/* Beginning of buffer. */
#ifndef OS2
#ifndef AMIGA
    oldsig = signal(SIGINT, trtrap);	/* Save current interrupt trap. */
#endif
#endif
    tr_int = 0;				/* Have not been interrupted (yet). */
    z = 1;				/* Return code presumed good. */
#ifdef VMS
    conres();
#endif /* VMS */

    c = 0;				/* Initial condition */
    while (c > -1) {			/* Loop for all characters in file */
	if (tr_int) {			/* Interrupted? */
	    printf("^C...\n");		/* Print message */
	    z = 0;
	    break;
	}
	c = zminchar();			/* Get a file character */
	debug(F101,"transmit char","",c);

	if (binary) {			/* If binary file, */
	    if (c == -1)		/* if at eof, */
	      break;			/* quit, */
	    else if (ttoc(dopar(c)) < 0) { /* else just send the character. */
		printf("?Can't transmit character\n");
		z = 0;
		break;
	    }
	    if (xmitw) msleep(xmitw);	/* Pause if requested */
	    if (xmitx) {		/* Echoing? */
		if (duplex) {		/* for half duplex */
		    conoc(c);		/* echo locally. */
		} else {		/* For full duplex, */
		    x = ttinc(1);	/* try to read back echo */
		    if (x > -1) conoc(x); /* and display it. */
		}
	    } else ttflui();		/* Not echoing, just flush input. */

	} else {			/* Text mode, line at a time. */

	    if (c == '\n') {		/* Got a line */
		if (i == 0) {		/* Blank line? */
		    if (xmitf)		/* Yes, insert fill if asked. */
		      line[i++] = dopar(xmitf);
		}
		if (i == 0 || line[i-1] != dopar('\r'))
		  line[i++] = dopar('\r'); /* Terminate it with CR */
		if (xmitl) line[i++] = dopar('\n'); /* Include LF if asked */

	    } else if (c != -1) {	/* Not a newline, regular character */
#ifndef NOCSETS
		/* Translate character sets */
		if (sxo) c = (*sxo)(c); /* From local to intermediate. */
		if (rxo) c = (*rxo)(c); /* From intermediate to remote. */
#endif /* NOCSETS */

		if (xmits && parity && (c & 0200)) { /* If shifting */
		    line[i++] = dopar(SO);          /* needs to be done, */
		    line[i++] = dopar(c);           /* do it here, */
		    line[i++] = dopar(SI);          /* crudely. */
		} else {
		    line[i++] = dopar(c); /* Otherwise, just the char itself */
		}
	    }

/* Send characters if buffer full, or at end of line, or at end of file */

	    if (i >= XMBUFS || c == '\n' || c == -1) { 
		p = line;
		line[i] = '\0';
		debug(F111,"transmit buf",p,i);
		if (ttol((CHAR *)p,i) < 0) { /* try to send it. */
		    printf("Can't send buffer\n");
		    z = 0;
		    break;
		}
		i = 0;			/* Reset buffer pointer. */

/* Worry about echoing here. "xmitx" is SET TRANSMIT ECHO flag. */

		if (duplex && xmitx)	/* If local echo, echo it */
		  conoll(p);
		if (xmitw)		/* Give receiver time to digest. */
		  msleep(xmitw);
		if (t != 0 && c == '\n') { /* Want a turnaround character */
		    x = 0;		   /* Wait for it */
		    while (x != t) {
			if ((x = ttinc(1)) < 0) break;
			if (xmitx && !duplex) conoc(x); /* Echo any echoes */
		    }
		} else if (xmitx && !duplex) { /* Otherwise, */
		    while (ttchk() > 0) {      /* echo for as long as */
			if ((x = ttinc(0)) < 0) break; /* anything is there. */
			conoc(x);
		    }
		} else ttflui();	/* Otherwise just flush input buffer */
	    }				/* End of buffer-dumping block */
	}				/* End of text mode */
    }					/* End of character-reading loop */

    if (*xmitbuf) {			/* Anything to send at EOF? */
	p = xmitbuf;			/* Yes, point to string. */
	while (*p)			/* Send it. */
	  ttoc(dopar(*p++));		/* Don't worry about echo here. */
    }

#ifndef OS2
#ifndef AMIGA
    signal(SIGINT,oldsig);		/* put old signal action back. */
#endif
#endif
#ifdef VMS
    concb(escape);			/* Put terminal back, */
#endif /* VMS */
    zclose(ZIFILE);			/* close file, */
#ifndef NOCSETS
    language = langsv;			/* restore language, */
#endif /* NOCSETS */
    return(z);				/* and return successfully. */
}
#endif /* NOXMIT */

#ifdef COMMENT
/*
  This code is not used any more.  Instead we call system to do the
  typing.  Revive this code if your system can't be called to do this.
*/

/*  D O T Y P E  --  Type a file  */

int
dotype(s) char *s; {

#ifndef OS2
    SIGTYP (* oldsig)();		/* For saving old interrupt trap. */
#endif
    int z;				/* Return code. */
    int c;				/* Worker. */

    if (zopeni(ZIFILE,s) == 0) {	/* Open the file to be transmitted */
	printf("?Can't open %s\n",s);
	return(0);
    }
#ifndef OS2
#ifndef AMIGA
    oldsig = signal(SIGINT, trtrap);	/* Save current interrupt trap. */
#endif
#endif
    tr_int = 0;				/* Have not been interrupted (yet). */
    z = 1;				/* Return code presumed good. */

#ifdef VMS
    conoc(CR);				/* On VMS, display blank line first */
    conoc(LF);
    conres();				/* So Ctrl-C/Y will work */
#endif /* VMS */
    while ((c = zminchar()) != -1) {	/* Loop for all characters in file */
	if (tr_int) {			/* Interrupted? */
	    printf("^C...\n");		/* Print message */
	    z = 0;
	    break;
	}
	conoc(c);			/* Echo character on screen */
    }
#ifndef OS2
#ifndef AMIGA
    signal(SIGINT,oldsig);		/* put old signal action back. */
#endif
#endif
    tr_int = 0;
#ifdef VMS
    concb(escape);			/* Get back in command-parsing mode, */
#endif /* VMS */
    zclose(ZIFILE);			/* close file, */
    return(z);				/* and return successfully. */
}
#endif /* COMMENT */

#ifndef NOCSETS

CHAR (*sxx)();			/* Local translation function */
CHAR (*rxx)();			/* Local translation function */
extern CHAR zl1as(), xl1as();

/*  X L A T E  --  Translate a local file from one character set to another */

/*
  Translates input file (fin) from character set csin to character set csout
  and puts the result in the output file (fout).  The two character sets are
  file character sets from fcstab.
*/

int
xlate(fin, fout, csin, csout) char *fin, *fout; int csin, csout; {

#ifndef OS2
    SIGTYP (* oldsig)();		/* For saving old interrupt trap. */
#endif
    int z = 1;				/* Return code. */
    int c, tcs;				/* Workers */

    if (zopeni(ZIFILE,fin) == 0) {	/* Open the file to be transmitted */
	printf("?Can't open input file %s\n",fin);
	return(0);
    }
    if (zopeno(ZOFILE,fout,NULL,NULL) == 0) { /* And the output file */
	printf("?Can't open output file %s\n",fout);
	return(0);
    }
#ifndef OS2
#ifndef AMIGA
    oldsig = signal(SIGINT, trtrap);	/* Save current interrupt trap. */
#endif
#endif
    tr_int = 0;				/* Have not been interrupted (yet). */
    z = 1;				/* Return code presumed good. */

#ifdef KANJI
/* Kanji not supported yet */
    if (fcsinfo[csin].alphabet == AL_JAPAN)
      tcs = TC_TRANSP;
    else
#endif /* KANJI */
#ifdef CYRILLIC
    if (fcsinfo[csout].alphabet == AL_CYRIL)
	tcs = TC_CYRILL;
      else
	tcs = TC_1LATIN;
#else
	tcs = TC_1LATIN;
#endif /* CYRILLIC */
    printf("%s (%s) => %s (%s)\n",	/* Say what we're doing. */
	   fin, fcsinfo[csin].name,
	   fout,fcsinfo[csout].name
    );
    printf("via %s", tcsinfo[tcs].name);
    if (language)
      printf(", language: %s\n",langs[language].description);
    printf("\n\n");

    if (csin == csout) {		/* Input and output sets the same? */
	sxx = rxx = NULL;		/* If so, no translation. */
    } else {				/* Otherwise, set up */
	sxx = xls[tcs][csin];		/* translation function */
	rxx = xlr[tcs][csout];		/* pointers. */
	if (rxx == zl1as) rxx = xl1as;
    }
    while ((c = zminchar()) != -1) {	/* Loop for all characters in file */
	if (tr_int) {			/* Interrupted? */
	    printf("^C...\n");		/* Print message */
	    z = 0;
	    break;
	}
	if (sxx) c = (*sxx)(c);		/* From fcs1 to tcs */
	if (rxx) c = (*rxx)(c);		/* from tcs to fcs2 */

	if (zchout(ZOFILE,c) < 0) {	/* Output the translated character */
	    printf("File output error\n");
	    z = 0;
	    break;
	}
    }
#ifndef OS2
#ifndef AMIGA
    signal(SIGINT,oldsig);		/* put old signal action back. */
#endif
#endif
    tr_int = 0;
    zclose(ZIFILE);			/* close files, */
    zclose(ZOFILE);
    return(z);				/* and return successfully. */
}
#endif /* NOCSETS */

/*  D O L O G  --  Do the log command  */
 
int
dolog(x) int x; {
    int y, disp; char *s;
 
    switch (x) {
 
	case LOGD:
#ifdef DEBUG
	    y = cmofi("Name of debugging log file","debug.log",&s,xxstring);
#else
    	    y = -9; s = "";
	    printf("%s","?Sorry, debug log not available\n");
#endif /* DEBUG */
	    break;
 
	case LOGP:
	    y = cmofi("Name of packet log file","packet.log",&s,xxstring);
	    break;
 
	case LOGS:
	    y = cmofi("Name of session log file","session.log",&s,xxstring);
	    break;
 
	case LOGT:
#ifdef TLOG
	    y = cmofi("Name of transaction log file","transact.log",&s,
		      xxstring);
#else
    	    y = -2; s = "";
	    printf("%s","- Sorry, transaction log not available\n");
#endif /* TLOG */
	    break;
 
	default:
	    printf("\n?Unexpected log designator - %d\n",x);
	    return(-2);
    }
    if (y < 0) return(y);
 
    strcpy(line,s);
    s = line;
    if ((y = cmkey(disptb,2,"Disposition","new",xxstring)) < 0)
      return(y);
    disp = y;
    if ((y = cmcfm()) < 0) return(y);
 
    switch (x) {
 
#ifdef DEBUG
	case LOGD:
	    return(deblog = debopn(s,disp));
#endif /* DEBUG */
 
	case LOGP:
	    return(pktlog = pktopn(s,disp));

	case LOGS:
	    return(seslog = sesopn(s,disp));
 
	case LOGT:
	    return(tralog = traopn(s,disp));
 
	default:
	    return(-2);
	}
}
 
int
pktopn(s,disp) char *s; int disp; {
    extern char pktfil[];
    static struct filinfo xx;
    int y;

    zclose(ZPFILE);
    if(s[0] == '\0') return(0);
    if (disp) {
	xx.bs = 0; xx.cs = 0; xx.rl = 0; xx.org = 0; xx.cc = 0;
	xx.typ = 0; xx.dsp = XYFZ_A; xx.os_specific = '\0';
	xx.lblopts = 0;
	y = zopeno(ZPFILE,s,NULL,&xx);
    } else y = zopeno(ZPFILE,s,NULL,NULL);
    if (y > 0) 
      strcpy(pktfil,s); 
    else 
      *pktfil = '\0';
    return(y);
}

int
traopn(s,disp) char *s; int disp; {
#ifdef TLOG
    extern char trafil[];
    static struct filinfo xx;
    int y;

    zclose(ZTFILE);
    if(s[0] == '\0') return(0);
    if (disp) {
	xx.bs = 0; xx.cs = 0; xx.rl = 0; xx.org = 0; xx.cc = 0;
	xx.typ = 0; xx.dsp = XYFZ_A; xx.os_specific = '\0';
	xx.lblopts = 0;
	y = zopeno(ZTFILE,s,NULL,&xx);
    } else y = zopeno(ZTFILE,s,NULL,NULL);
    if (y > 0) {
	strcpy(trafil,s);
	tlog(F110,"Transaction Log:",versio,0L);
	tlog(F100,ckxsys,"",0L);
	ztime(&s);
	tlog(F100,s,"",0L);
    } else *trafil = '\0';
    return(y);
#else
    return(0);
#endif
}

int
sesopn(s,disp) char * s; int disp; {
    extern char sesfil[];
    static struct filinfo xx;
    int y;

    zclose(ZSFILE);
    if(s[0] == '\0') return(0);
    if (disp) {
	xx.bs = 0; xx.cs = 0; xx.rl = 0; xx.org = 0; xx.cc = 0;
	xx.typ = 0; xx.dsp = XYFZ_A; xx.os_specific = '\0';
	xx.lblopts = 0;
	y = zopeno(ZSFILE,s,NULL,&xx);
    } else y = zopeno(ZSFILE,s,NULL,NULL);
    if (y > 0)
      strcpy(sesfil,s);
    else
      *sesfil = '\0';
    return(y);
}
 
int
debopn(s,disp) char *s; int disp; {
#ifdef DEBUG
    char *tp;
    static struct filinfo xx;

    zclose(ZDFILE);

    if (disp) {
	xx.bs = 0; xx.cs = 0; xx.rl = 0; xx.org = 0; xx.cc = 0;
	xx.typ = 0; xx.dsp = XYFZ_A; xx.os_specific = '\0';
	xx.lblopts = 0;
	deblog = zopeno(ZDFILE,s,NULL,&xx);
    } else deblog = zopeno(ZDFILE,s,NULL,NULL);
    if (deblog > 0) {
	strcpy(debfil,s);
	debug(F110,"Debug Log ",versio,0);
	debug(F100,ckxsys,"",0);
	ztime(&tp);
	debug(F100,tp,"",0);
    } else *debfil = '\0';
    return(deblog);
#else
    return(0);
#endif
}

/*  S H O P A R  --  Show Parameters  */
 
char *
#ifdef CK_ANSIC
parnam(char c)
#else
parnam(c) char c; 
#endif /* CK_ANSIC */
/* parnam */ {
    switch (c) {
	case 'e': return("even");
	case 'o': return("odd");
	case 'm': return("mark");
	case 's': return("space");
	case 0:   return("none");
	default:  return("invalid");
    }
}

#ifdef SUNX25
VOID
shox25() {
    if (nettype == NET_SX25) {
	printf("SunLink X.25 V%d.%d",x25ver / 10,x25ver % 10);
	if (ttnproto == NP_X3) printf(", PAD X.3, X.28, X.29 protocol,");
	printf("\n Reverse charge call %s",
	       revcall ? "selected" : "not selected");
	printf (", Closed user group ");
	if (closgr > -1)
	  printf ("%d",closgr);
	else
	  printf ("not selected");
	printf (",");
	printf("\n Call user data %s.", cudata ? udata : "not selected");
    }
}
#endif /* SUNX25 */

VOID
shoparc() {
    int i; char *s;
    long zz;

    puts("Communications Parameters:");

    if (network) {
	printf(" Host: %s",ttname);
    } else {
	printf(" Line: %s, speed: ",ttname);
	if ((zz = ttgspd()) < 0) {
	    printf("unknown");
        } else {
	    if (speed == 8880) printf("75/1200"); else printf("%ld",zz);
	}
    }
    printf(", mode: ");
    if (local) printf("local"); else printf("remote");
    if (network == 0) {
#ifndef NODIAL
	for (i = 0; i < nmdm; i++) {
	    if (mdmtab[i].kwval == mdmtyp) {
		printf(", modem: %s",mdmtab[i].kwd);
		break;
	    }
	}
#endif
    } else {
	if (nettype == NET_TCPA) printf(", TCP/IP/streams");
	if (nettype == NET_TCPB) printf(", TCP/IP/sockets");
        if (nettype == NET_DEC) printf(", DECnet");
#ifdef SUNX25
	shox25();
#endif /* SUNX25 */
	if (ttnproto == NP_TELNET) printf(", telnet protocol");
    }
    if (local) {
	i = parity ? 7 : 8;
	if (i == 8) i = (cmask == 0177) ? 7 : 8;
	printf("\n Terminal bits: %d, p",i);
    } else printf("\n P");
    printf("arity: %s",parnam(parity));
    printf(", duplex: ");
    if (duplex) printf("half, "); else printf("full, ");
    printf("flow: ");
    if (flow == FLO_XONX) printf("xon/xoff");
	else if (flow == FLO_NONE) printf("none");
#ifdef UTEK
	else if (flow == FLO_HARD) printf("idtr/octs");
#else
	else if (flow == FLO_RTSC) printf("rts/cts");
#endif /* UTEK */
        else if (flow == FLO_DTRC) printf("dtr/cd");
	else printf("%d",flow);
    printf(", handshake: ");
    if (turn) printf("%d\n",turnch); else printf("none\n");
    if (local && !network) {		/* Lockfile & carrier stuff */
	if (carrier == CAR_OFF) s = "off";
	else if (carrier == CAR_ON) s = "on";
	else if (carrier == CAR_AUT) s = "auto";
	else s = "unknown";
	printf(" Carrier: %s", s);
	if (carrier == CAR_ON) {
	    if (cdtimo) printf(", timeout: %d sec", cdtimo);
	    else printf(", timeout: none");
	}
#ifdef UNIX
	if (haslock && *flfnam) {	/* Lockfiles only apply to UNIX... */
	    printf(", lockfile: %s\n",flfnam);
	}
#endif /* UNIX */
	printf(" Escape character: %d (^%c)\n",escape,ctl(escape));
    }
}

VOID
shonet() {
#ifdef NETCONN
    printf("Supported networks:\n");
#ifdef VMS
#ifdef MULTINET    
    printf(" TGV MultiNet TCP/IP\n");
#else
    printf(" None\n");
#endif /* MULTINET */

#else /* Not VMS */

#ifdef TCPSOCKET
    printf(" TCP/IP (sockets library)\n");
#endif /* TCPSOCKET */
#ifdef TCPSTREAM
    printf(" TCP/IP (streams)\n");
#endif /* TCPSTREAM */
#ifdef TNCODE
    printf(" Telnet protocol\n");
#endif /* TNCODE */
#ifdef SUNX25
    printf(" SunLink X.25\n");
#endif /* SUNX25 */
#endif /* VMS */

    printf("Active SET HOST connection: ");
    if (network) {
	printf("%s via ",ttname);
	if (nettype == NET_TCPA) printf("TCP/IP (streams)\n");
	else if (nettype == NET_TCPB) printf("TCP/IP (sockets)\n");
	else if (nettype == NET_SX25) printf("SunLink X.25\n");
	else if (nettype == NET_DEC) printf("DECnet\n");
#ifdef SUNX25
	if (nettype == NET_SX25) shox25();
#endif /* SUNX25 */
#ifdef TNCODE
	if (ttnproto == NP_TELNET) printf("TCP/IP telnet protocol\n");
#endif /* TNCODE */
    } else printf("None\n");
#else /* Not NETCONN */
    printf("No network support\n");
#endif /* NETCONN */
}

#ifndef NODIAL

VOID
shodial() {
    if (mdmtyp >= 0 || local != 0) doshodial();
}

int
doshodial() {
    char c, *s;
    printf(" Dial hangup: %s, dial timeout: ",
	   dialhng ? "on" : "off");
    if (dialtmo > 0)
      printf("%d sec", dialtmo);
    else
      printf("auto");
    printf("\n Dial kermit-spoof: %s",dialksp ? "on" : "off");
    printf(", dial display: %s\n",dialdpy ? "on" : "off");
    printf(" Dial speed-matching: %s",mdmspd ? "off" : "on");
    printf(", dial mnp-enable: %s\n",dialmnp ? "on" : "off");
    printf(" Dial init-string: ");
    s = getdws(mdmtyp);			/* Ask dial module for it */
    if (s == NULL || !(*s)) {		/* Empty? */
	printf("none\n");
    } else {				/* Not empty. */
	while (c = *s++)		     /* Can contain controls */
	  if (c > 31 && c < 127) putchar(c); /* so display them */
	  else printf("\\{%d}",c);	     /* in backslash notation */
	printf("\n");	
    }
    printf(" Redial number: %s\n",dialnum ? dialnum : "none");
    return(0);
}
#endif /* NODIAL */

#ifdef SUNX25
VOID
shopad() {
    int i;
    printf("\nX.3 PAD Parameters:\n");
    for (i = 0; i < npadx3; i++)
      printf(" [%d] %s %d\n",padx3tab[i].kwval,padx3tab[i].kwd,
	     padparms[padx3tab[i].kwval]);
}
#endif /* SUNX25 */

VOID
shomdm() {
    int y;
    y = ttgmdm();
    switch (y) {
      case -3: printf("Modem signals unavailable in this version of Kermit\n");
	       break;
      case -2: printf("No modem control for this device\n"); break;
      case -1: printf("Modem signals unavailable\n"); break;
      default: 
	printf(" Carrier Detect      (CD):  %s\n",(y & BM_DCD) ? "On": "Off");
	printf(" Dataset Ready       (DSR): %s\n",(y & BM_DSR) ? "On": "Off");
	printf(" Clear To Send       (CTS): %s\n",(y & BM_CTS) ? "On": "Off");
        printf(" Ring Indicator      (RI):  %s\n",(y & BM_RNG) ? "On": "Off");
        printf(" Data Terminal Ready (DTR): %s\n",(y & BM_DTR) ? "On": "Off");
        printf(" Request to Send     (RTS): %s\n",(y & BM_RTS) ? "On": "Off");
    }
}

/*  Show File Parameters */

VOID
shoparf() {
    char *s; int i;
    printf("\nFile parameters:       ");
#ifdef COMMENT
    printf("Blocksize:     %5d      ",fblksiz);
#endif /* COMMENT */
    printf(" Attributes:       ");
    if (atcapr) printf("on"); else printf("off");
#ifdef VMS
    printf("  Record-Length: %5d",frecl);
#endif /* VMS */
    printf("\n Names:   ");
    if (fncnv) printf("%-12s","converted"); else printf("%-10s","literal");
#ifdef DEBUG
    printf("  Debugging Log:    ");
    if (deblog) printf("%s",debfil); else printf("none");
#endif /* DEBUG */

    printf("\n Type:    ");
    switch (binary) {
      case XYFT_T: s = "text";    break;
#ifdef VMS
      case XYFT_B: s = "binary fixed"; break;
      case XYFT_I: s = "image";        break;
      case XYFT_L: s = "labeled";      break;
      case XYFT_U: s = "binary undef"; break;
#else
      case XYFT_B: s = "binary"; break;
#endif /* VMS */
      default: s = "?"; break;
    }
    printf("%-12s",s);
#ifdef COMMENT
    printf(" Organization:  ");
    switch (forg) {
      case XYFO_I: printf("%-10s","indexed"); break;
      case XYFO_R: printf("%-10s","relative"); break;
      case XYFO_S: printf("%-10s","sequential"); break;
    }
#endif /* COMMENT */
    printf("  Packet Log:       ");
    if (pktlog) printf(pktfil); else printf("none");
#ifdef UNIX
    printf("  Longest filename: %d",maxnam);
#endif /* UNIX */
    printf("\n Collide: ");
    for (i = 0; i < ncolx; i++)
      if (colxtab[i].kwval == fncact) break;
    printf("%-12s", (i == ncolx) ? "unknown" : colxtab[i].kwd);

#ifdef COMMENT
    if (warn) printf("%-10s","on"); else printf("%-10s","off");
#endif /* COMMENT */
#ifdef COMMENT
    printf(" Format:        ");
    switch (frecfm) {
      case XYFF_F:  printf("%-10s","fixed"); break;
      case XYFF_VB: printf("%-10s","rcw"); break;
      case XYFF_S:  printf("%-10s","stream"); break;
      case XYFF_U:  printf("%-10s","undefined"); break;
      case XYFF_V:  printf("%-10s","variable"); break;
    }
#endif /* COMMENT */
    printf("  Session Log:      ");
    if (seslog) printf(sesfil); else printf("none");
#ifdef UNIX
    printf("  Longest pathname: %d",maxpath);
#endif /* UNIX */
    printf("\n Display: ");
    if (quiet) printf("%-12s","off"); else printf("%-12s","on");
#ifdef COMMENT
    printf("Carriage-Control: ");
    switch (fcctrl) {
      case XYFP_F: printf("%-10s","fortran"); break;
      case XYFP_N: printf("%-10s","newline"); break;
      case XYFP_P: printf("%-10s","machine"); break;
      case XYFP_X: printf("%-10s","none"); break;
    }
#endif /* COMMENT */
#ifdef TLOG
    printf("  Transaction Log:  ");
    if (tralog) printf(trafil); else printf("none");
#endif /* TLOG */
#ifndef NOCSETS
    if (binary == XYFT_T) {
	shocharset();
    } else
#endif /* NOCSETS */
      printf("\n");
    printf("\nFile Byte Size: %d",(fmask == 0177) ? 7 : 8);
    printf(", Incomplete Files: ");
    if (keep) printf("keep"); else printf("discard");
#ifdef KERMRC    
    printf(", Init file: %s",kermrc);
#endif /* KERMRC */
    printf("\n");
}

VOID
shoparp() {
    printf("\nProtocol Parameters:   Send    Receive");
    if (timef || spsizf) printf("    (* = override)");
    printf("\n Timeout:      %11d%9d", rtimo,  pkttim);
    if (timef) printf("*"); else printf(" ");
    printf("       Server timeout:%4d\n",srvtim);
    printf(  " Padding:      %11d%9d", npad,   mypadn);
    printf("        Block Check: %6d\n",bctr);
    printf(  " Pad Character:%11d%9d", padch,  mypadc);
    printf("        Delay:       %6d\n",delay);
    printf(  " Packet Start: %11d%9d", mystch, stchr);
    printf("        Max Retries: %6d\n",maxtry);
    printf(  " Packet End:   %11d%9d", seol,   eol);
    if (ebqflg)
      printf("        8th-Bit Prefix: '%c'",ebq);
    printf(  "\n Packet Length:%11d", spsizf ? spsizr : spsiz);
    printf( spsizf ? "*" : " " ); printf("%8d",  urpsiz);
    printf( (urpsiz > 94) ? " (94)" : "     ");
    if (rptflg)
      printf("   Repeat Prefix:  '%c'",rptq);
    printf(  "\n Length Limit: %11d%9d", maxsps, maxrps);
    printf("        Window:%12d set, %d used\n",wslotr,wslotn);
    printf(    " Buffer Size:  %11d%9d", bigsbsiz, bigrbsiz);
    printf("        Locking-Shift:    ");
    if (lscapu == 2) {
	printf("forced\n");
    } else {
	printf("%s", (lscapr ? "enabled" : "disabled"));
	if (lscapr) printf(",%s%s", (lscapu ? " " : " not "), "used");
	printf("\n");
    }
}

#ifndef NOCSETS
VOID
shoparl() {
    int i;
    printf("\nAvailable Languages:\n");
    for (i = 0; i < MAXLANG; i++) {
	printf(" %s\n",langs[i].description);
    }	
    printf("\nCurrent Language: %s\n",langs[language].description);
    shocharset();
    printf("\n\n");
}

VOID
shocharset() {
    printf("\nFile Character-Set: %s (",fcsinfo[fcharset].name);
    if (fcsinfo[fcharset].size == 128) printf("7-bit)");
    else printf("8-bit)");
    printf("\nTransfer Character-Set");
    if (tslevel == TS_L2)
      printf(": (international)");
    else if (tcharset == TC_TRANSP)
      printf(": Transparent");
    else
      printf(": %s",tcsinfo[tcharset].name);
}
#endif /* NOCSETS */

VOID
shopar() {
    printf("\n%s,%s\n",versio,ckxsys); 
    shoparc();
    shoparp();
    shoparf();
}

/*  D O S T A T  --  Display file transfer statistics.  */

int
dostat() {
    printf("\nMost recent transaction --\n");
    printf(" parity: %s",parnam(parity));
    if (autopar) printf(" (detected automatically)");
    printf("\n files: %ld\n",filcnt);
    printf(" total file characters  : %ld\n",tfc);
    printf(" communication line in  : %ld\n",tlci);
    printf(" communication line out : %ld\n",tlco);
    printf(" packets sent           : %d\n", spackets);
    printf(" packets received       : %d\n", rpackets);
    printf(" damaged packets rec'd  : %d\n", crunched);
    printf(" timeouts               : %d\n", timeouts);
    printf(" retransmissions        : %d\n", retrans);
    printf(" window slots used      : %d\n", wmax);
    printf(" elapsed time           : %d sec\n",tsecs);
    if (speed <= 0L) speed = ttgspd();
    if (speed > 0L) {
	if (speed == 8880)
	  printf(" transmission rate      : 75/1200 bps\n");
	else
	  printf(" transmission rate      : %ld bps\n",speed);
    }
    if (filcnt > 0) {
	if (tsecs > 0) {
	    long lx;
	    lx = (tfc * 10L) / (long) tsecs;
	    printf(" effective data rate    : %ld cps\n",lx/10L);
	    if (speed > 0L && speed != 8880L && network == 0) {
		lx = (lx * 100L) / speed;
		printf(" efficiency (percent)   : %ld\n",lx);
	    }
	}
	printf(" packet length          : %d (send), %d (receive)\n",
	       spsizf ? spsizr : spsiz, urpsiz);
	printf(" block check type used  : %d\n",bctu);
	printf(" compression            : ");
	if (rptflg) printf("yes [%c] (%d)\n",rptq,rptn); else printf("no\n");
	printf(" 8th bit prefixing      : ");
	if (ebqflg) printf("yes [%c]\n",ebq); else printf("no\n");
	printf(" locking shifts         : %s\n\n", lscapu ? "yes" : "no");
    } else printf("\n");
    return(1);
}

/*  D O C O N E C T  --  Do the connect command  */
 
/*  Note, we don't call this directly from dial, because we need to give */
/*  the user a chance to change parameters (e.g. parity) after the */
/*  connection is made. */
 
int
doconect() {
    int x;
    conres();				/* Put console back to normal */
    x = conect();			/* Connect */
    concb(escape);			/* Put console into cbreak mode, */
    return(x);				/* for more command parsing. */
}

#ifndef NOSPL
/* The INPUT command */

#ifdef NETCONN
extern int tn_init;
#ifndef IAC
#define IAC 255
#endif /* IAC */
#endif /* NETCONN */

int
doinput(timo,s) int timo; char *s; {
    int x, y, i, icn;
    char *xp, *xq = (char *)0;
    CHAR c;

    if (local) {			/* Put line in "ttvt" mode */
	y = ttvt(speed,flow);		/* if not already. */
	if (y < 0) {
	    printf("?Can't condition line for INPUT\n");
	    return(0);			/* Watch out for failure. */
	}
    }
    y = strlen(s);
    debug(F111,"doinput",s,y);
    if (timo <= 0) timo = 1;		/* Give at least 1 second timeout */
    x = 0;				/* Return code, assume failure */
    i = 0;				/* String pattern match position */

#ifdef COMMENT
/* This is done automatically in ttopen() now... */
#ifdef NETCONN
    if (network && ttnproto == NP_TELNET) /* If telnet connection, */
      if (!tn_init++) tn_ini();           /* initialize it if necessary */
#endif /* NETCONN */
#endif /* COMMENT */
    if (!incase) {			/* INPUT CASE = IGNORE?  */
	xp = malloc(y+2);		/* Make a separate copy of the */
	if (!xp) {			/* input string for editing. */
	    printf("?malloc error 5\n");
	    return(x);
	} else xq = xp;			/* Save pointer to beginning */

	while (*s) {			/* Convert to lowercase */
	    *xp = *s;
	    if (isupper(*xp)) *xp = tolower(*xp);
	    xp++; s++;
	}
	*xp = NUL;			/* Terminate the search string. */
	s = xq;				/* Point back to beginning. */
    }
    inpbps = inpbp;			/* Save current pointer. */
    rtimer();				/* Get current time. */
    while (1) {				/* Character-getting loop */
	if (local) {			/* One case for local */
	    y = ttinc(1);		/* Get character from comm line. */
	    debug(F101,"input ttinc(1) returns","",y);
	    if (icn = conchk()) {	/* Interrupted from keyboard? */
		debug(F101,"input interrupted from keyboard","",icn);
		while (icn--) coninc(0); /* Yes, read what was typed. */
		break;			/* And fail. */
	    }
	} else {			/* Another for remote */
	    y = coninc(1);
	    debug(F101,"input coninc(1) returns","",y);
	}
	if (y > -1) {			/* A character arrived */
#ifdef NETCONN
/* Check for telnet protocol negotiation */
	    if (network && (ttnproto == NP_TELNET) && ((y & 0xff) == IAC)) {
		tn_doop((y & 0xff),duplex);
		continue;
	    }
#endif /* NETCONN */

	    /* Real input character to be checked */

	    c = cmask & (CHAR) y;	/* Mask off parity */
	    if (c) *inpbp++ = c;	/* Store result in circular buffer */
	    if (inpbp >= inpbuf + INPBUFSIZ) { /* Time to wrap around? */
		inpbp = inpbuf;		/* Yes. */
		*(inpbp+INPBUFSIZ) = NUL; /* Make sure it's null-terminated. */
	    }
	    if (inecho) conoc(c);	/* Echo and log the input character */
	    if (seslog) {
#ifdef UNIX
		if (sessft != 0 || c != '\r')
#endif /* UNIX */
		  if (zchout(ZSFILE,c) < 0) seslog = 0;
	    }
	    if (!incase) {		/* Ignore alphabetic case? */
		if (isupper(c)) c = tolower(c); /* Yes */
	    }
	    debug(F000,"doinput char","",c);
	    debug(F000,"compare char","",s[i]);
	    if (c == s[i]) i++; else i = 0; /* Check for match */
	    if (s[i] == '\0') {		/* Matched all the way to end? */
		x = 1;			/* Yes, */
		break;			/* done. */
	    }
	}
	if (gtimer() > timo)		/* Did not match, timer exceeded? */
	  break;
    }					/* Still have time left, continue. */
    if (!incase) if (xq) free(xq);	/* Done, free dynamic memory. */
    return(x);				/* Return the return code. */
}
#endif /* NOSPL */

#ifndef NOSPL
/* REINPUT Command */

/* Note, the timeout parameter is required, but ignored. */
/* Syntax is compatible with MS-DOS Kermit except timeout can't be omitted. */
/* This function only looks at the characters already received */
/* and does not read any new characters from the communication line. */

int
doreinp(timo,s) int timo; char *s; {
    int x, y, i;
    char *xx, *xp, *xq = (char *)0;
    CHAR c;

    y = strlen(s);
    debug(F111,"doreinput",s,y);

    x = 0;				/* Return code, assume failure */
    i = 0;				/* String pattern match position */

    if (!incase) {			/* INPUT CASE = IGNORE?  */
	xp = malloc(y+2);		/* Make a separate copy of the */
	if (!xp) {			/* search string. */
	    printf("?malloc error 6\n");
	    return(x);
	} else xq = xp;			/* Keep pointer to beginning. */
	while (*s) {			/* Yes, convert to lowercase */
	    *xp = *s;
	    if (isupper(*xp)) *xp = tolower(*xp);
	    xp++; s++;
	}
	*xp = NUL;			/* Terminate it! */
	s = xq;				/* Move search pointer to it. */
    }
    xx = inpbp;				/* Current INPUT buffer pointer */
    do {
	c = *xx++;			/* Get next character */
	if (xx >= inpbuf + INPBUFSIZ) xx = inpbuf; /* Wrap around */
	if (!incase) {			/* Ignore alphabetic case? */
	    if (isupper(c)) c = tolower(c); /* Yes */
	}
	debug(F000,"doreinp char","",c);
	debug(F000,"compare char","",s[i]);
	if (c == s[i]) i++; else i = 0; /* Check for match */
	if (s[i] == '\0') {		/* Matched all the way to end? */
	    x = 1;			/* Yes, */
	    break;			/* done. */
	}
    } while (xx != inpbp);		/* Until back where we started. */

    if (!incase) if (xq) free(xq);	/* Free this if it was malloc'd. */
    return(x);				/* Return search result. */
}
#ifndef NOSPL

#endif /* NOSPL */
/*  X X S T R I N G  --  Interpret strings containing backslash escapes  */

/*
 Copies result to new string.
  strips enclosing braces or doublequotes.
  interprets backslash escapes.
  returns 0 on success, nonzero on failure.
  tries to be compatible with MS-DOS Kermit.

 Syntax of input string:
  string = chars | "chars" | {chars}
  chars = (c* e*)*
  where c = any printable character, ascii 32-126
  and e = a backslash escape
  and * means 0 or more repetitions of preceding quantity
  backslash escape = \operand
  operand = {number} or number
  number = [r]n[n[n]]], i.e. an optional radix code followed by 1-3 digits
  radix code is oO (octal), hHxX (hex), dD or none (decimal).
*/

int
yystring(s,s2) char *s; char **s2; {	/* Reverse a string */
    int x;
    static char *new;
    new = *s2;
    if (!s || !new) return(-1);		/* Watch out for null pointers. */
    if ((x = strlen(s)) == 0) {		/* Recursion done. */
	*new = '\0';
	return(0);
    }
    x--;				/* Otherwise, call self */
    *new++ = s[x];			/* to reverse rest of string. */
    s[x] = 0;
    return(yystring(s,&new));
}
#endif /* NOSPL */

#ifndef NOSPL
#define FNVALL 1000
char fnval[FNVALL+2];			/* Return value */

char *					/* Evaluate builtin function */
fneval(fn,argp,argn) char *fn, *argp[]; int argn; {
    int i, j, k, len1, len2, n, x, y;
    char *bp[FNARGS];			/* Pointers to malloc'd strings */
    char *p, *s;

    if (!fn) fn = "";			/* Paranoia */
    debug(F111,"fneval",fn,argn);
    debug(F110,"fneval",argp[0],0);
    y = lookup(fnctab,fn,nfuncs,&x);
    if (y < 0)				/* bad function name */
      return("");			/* so value is null */

#ifdef DEBUG
    if (deblog) {
	int j;
	for (j = 0; j < argn; j++)
	  debug(F111,"fneval function arg",argp[j],j);
    }
#endif
    if (y == FN_LIT)			/* literal(arg1) */
      return(argp[0] ? argp[0] : "");	/* return a pointer to arg itself */

    if (y == FN_CON) {			/* Contents of variable, unexpanded. */
	char c;
	if (!(p = argp[0]) || !*p) return("");
	if (*p == CMDQ) p++;
	if ((c = *p) == '%') {		/* Scalar variable. */
	    c = *++p;			/* Get ID character. */
	    p = "";			/* Assume definition is empty */
	    if (!c) return(p);		/* Double paranoia */
	    if (c >= '0' && c <= '9') { /* Digit for macro arg */
		c -= '0';		/* convert character to integer */
		if (maclvl < 0)		/* Digit variables are global */
		  p = g_var[c];		/* if no macro is active */
		else			/* otherwise */
		  p = m_arg[maclvl][c]; /* they're on the stack */
	    } else {
		if (isupper(c)) c -= ('a'-'A');		
		p = g_var[c];		/* Letter for global variable */
	    }
	    return(p ? p : "");
	}
	if (c == '&') {			/* Array reference. */
	    int vbi, d;
	    if (arraynam(p,&vbi,&d) < 0) /* Get name and subscript */
	      return("");
	    if (chkarray(vbi,d) > 0) {	/* Array is declared? */
		vbi -= 'a';		/* Convert name to index */
		if (a_dim[vbi] >= d) {	/* If subscript in range */
		    char **ap;
		    ap = a_ptr[vbi];	/* get data pointer */
		    if (ap) {		/* and if there is one */
			return(ap[d]);	/* return what it points to */
		    }
		}
	    }
	    return(p ? p : "");		/* Otherwise its enexpanded value. */
	}
    }
    for (i = 0; i < argn; i++) {	/* Not literal, expand the args */
	n = 1024;			/* allow 1K per expanded arg, yow! */
	bp[i] = s = malloc(n);		/* get the new space */
	if (bp[i] == NULL) {		/* handle failure to get space */
	    for (k = 0; k < i; k++) if (bp[k]) free(bp[k]);
	    debug(F101,"fneval malloc failure, arg","",i);
	    return("");
	}
	p = argp[i] ? argp[i] : "";
	if (xxstring(p,&s,&n) < 0) {	/* Expand arg into new space */
	    debug(F101,"fneval xxstring fails, arg","",i);
	    for (k = 0; k <= i; k++)	/* Free up previous space on error */
	      if (bp[k]) free(bp[k]);
	    return("");			/* and return null string. */
	}
	debug(F101,"fneval xxstring n","",n);
    }
#ifdef DEBUG
    if (deblog) {
	int j;
	for (j = 0; j < argn; j++) {
	    debug(F111,"fneval arg post eval",argp[j],j);
	    debug(F111,"fneval evaluated arg",bp[j],j);
	}
    }
#endif

    switch (y) {			/* Do function on expanded args */

      case FN_DEF:
	k = mlook(mactab,bp[0],nmac);	/* def(arg1) - Return a macro def */
	p = (k > -1) ? mactab[k].mval : "";
	for (k = 0; k < argn; k++) if (bp[k]) free(bp[k]);
	return(p ? p : "");

      case FN_EVA:			/* eval(arg1) */
	p = evala(bp[0]);
	for (k = 0; k < argn; k++) if (bp[k]) free(bp[k]);
	return(p ? p : "");

      case FN_EXE:			/* execute(arg1) */
	j = strlen(s = bp[0]);		/* Length of macro invocation */
	p = "";				/* Initialize return value to null */
	if (j) {			/* If there is a macro to execute */
	    while (*s == SP) s++,j--;	/* strip leading spaces */
	    p = s;			/* remember beginning of macro name */
	    for (i = 0; i < j; i++) {	/* find end of macro name */
		if (*s == SP) break;
		s++;
	    }
	    if (*s == SP) {		/* if there was a space after */
		*s++ = NUL;		/* terminate the macro name */
		while (*s == SP) s++;	/* skip past any extra spaces */
	    } else s = "";		/* maybe there are no arguments */
	    if (p && *p)
	      k = mlook(mactab,p,nmac);	/* Look up the macro name */
	    else k = -1;

	    p = "";			/* Initialize return value */
	    if (k >= 0) {		/* If macro found in table */
		if ((j = dodo(k,s)) > 0) { /* Go set it up (like DO cmd) */
		    if (cmpush() > -1) { /* Push command parser state */
			extern int ifc;
			int ifcsav = ifc; /* Save IF condition (kludge) */
			k = parser(1);	/* Call parser to execute the macro */
			cmpop();	/* Pop command parser */
			ifc = ifcsav;	/* Restore IF condition */
			if (k == 0) {	/* No errors, ignore action cmds. */
			    p = mrval[maclvl+1]; /* If OK, set return value. */
			    if (p == NULL) p = "";
			}
		    } else {		/* Can't push any more */
			debug(F100,"fexec pushed too deep","",0);
                        printf("\n?\\fexec() too deeply nested\n");
			while (cmpop() > -1) ;
			p = "";
		    }
		} 
	    }
	}
	for (k = 0; k < argn; k++) if (bp[k]) free(bp[k]);
	return(p ? p : "");

      case FN_FC:			/* File count. */
	p = fnval;
	*p = NUL;
	if (argn > 0) {
	    k = zxpand(bp[0]);
	    sprintf(fnval,"%d",k);
	    for (k = 0; k < argn; k++) if (bp[k]) free(bp[k]);
	}
	return(p);

      case FN_FIL:			/* Next file in list. */
	p = fnval;			/* (no args) */
	*p = NUL;
	znext(p);
	for (k = 0; k < argn; k++) if (bp[k]) free(bp[k]);
	return(p ? p : "");

#ifdef COMMENT
/* Moved to \v(return) */
      case FN_VAL:			/* value() of last RETURN */
	p = mrval[maclvl+1];
	if (p == NULL) p = "";
	for (k = 0; k < argn; k++) if (bp[k]) free(bp[k]);
	return(p ? p : "");
#endif /* COMMENT */

      case FN_IND:			/* index(arg1,arg2) */
	if (argn > 1) {			/* Only works if we have 2 args */
	    int start;
	    len1 = strlen(bp[0]);	/* length of string to look for */
	    len2 = strlen(s = bp[1]);	/* length of string to look in */
	    if (len1 < 0) return("");	/* paranoia */
	    if (len2 < 0) return("");
	    j = len2 - len1;		/* length difference */
	    start = 0;			/* starting position */
	    if (argn > 2) {
		if (chknum(bp[2])) {
		    start = atoi(bp[2]) - 1;
		    if (start < 0) start = 0;
		}
	    }
	    if (j < 0 || start > j) {	/* search string is longer */
		p = "0";
	    } else {
		if (!incase) {		/* input case ignore? */
		    lower(bp[0]);
		    lower(bp[1]);
		}
		s = bp[1] + start;	/* Point to beginning of target */
		p = "0";
		for (i = 0; i <= (j - start); i++) { /* Now compare */
		    if (!strncmp(bp[0],s++,len1)) {
			sprintf(fnval,"%d",i+1+start);
			p = fnval;
			break;
		    }
		}
	    }
	} else p = "0";
	for (k = 0; k < argn; k++) if (bp[k]) free(bp[k]);
	return(p);

      case FN_CHR:			/* character(arg1) */
	if (chknum(bp[0])) {
	    i = atoi(bp[0]) & 255;
	    p = fnval;
	    *p++ = i;
	    *p = NUL;
	    p = fnval;
	} else p = "";
	for (k = 0; k < argn; k++) if (bp[k]) free(bp[k]);
	return(p);

      case FN_LEN:			/* length(arg1) */
	p = fnval;
	sprintf(p,"%d",strlen(bp[0]));
	for (k = 0; k < argn; k++) if (bp[k]) free(bp[k]);
	return(p);

      case FN_LOW:			/* lower(arg1) */
	s = bp[0];
	p = fnval;
    
	while (*s) {
	    if (isupper(*s))
	      *p = tolower(*s);
	    else
	      *p = *s;
	    p++; s++;
	}
	*p = NUL;
	for (k = 0; k < argn; k++) if (bp[k]) free(bp[k]);
	p = fnval;
	return(p);

      case FN_MAX:			/* max(arg1,arg2) */
      case FN_MIN:			/* min(arg1,arg2) */
      case FN_MOD:			/* mod(arg1,arg2) */
	if (chknum(bp[0]) && chknum(bp[1])) {
	    i = atoi(bp[0]);
	    j = atoi(bp[1]);
	    switch (y) {
	      case FN_MAX:
		if (j < i) j = i;
		break;
	      case FN_MIN:
		if (j > i) j = i;
		break;
	      case FN_MOD:
		j = i % j;
		break;
	    }
	    p = fnval;
	    sprintf(p,"%d",j);
	} else p = "";
	for (k = 0; k < argn; k++) if (bp[k]) free(bp[k]);
	return(p);

      case FN_SUB:			/* substr(arg1,arg2,arg3) */
	if (((argn > 1) && strlen(bp[1]) && !rdigits(bp[1])) ||
	    ((argn > 2) && strlen(bp[2]) && !rdigits(bp[2]))) {
	    p = "";			          /* if either, return null */
	} else {
	    p = fnval;			         /* pointer to result */
	    j = (argn > 1) ? atoi(bp[1]) : 1;    /* start position */
	    k = (argn > 2) ? atoi(bp[2]) : 1023; /* length */
	    if (k > 0 && j <= strlen(bp[0])) {	 /* if starting pos in range */
		s = bp[0]+j-1;    		 /* point to source string */
		for (i = 0; (i < k) && (*p++ = *s++); i++) ;  /* copy */
	    }
	    *p = NUL;			/* terminate the result */
	    p = fnval;			/* and point to it. */
	}
	for (k = 0; k < argn; k++) if (bp[k]) free(bp[k]); /* Free temp mem */
	return(p);

      case FN_UPP:			/* upper(arg1) */
	s = bp[0];
	p = fnval;
	while (*s) {
	    if (islower(*s))
	      *p = toupper(*s);
	    else
	      *p = *s;
	    p++; s++;
	}
	*p = NUL;
	for (k = 0; k < argn; k++) if (bp[k]) free(bp[k]);
	p = fnval;
	return(p);

      case FN_REP:			/* Repeat */
	p = "";				/* Return value */
	if (chknum(bp[1])) {		/* Repeat count */
	    n = atoi(bp[1]);
	    if (n > 0) {		/* Make n copies */
		p = fnval;
		*p = '\0';
		k = strlen(bp[0]);	/* Make sure string has some length */
		if (k > 0) {
		    for (i = 0; i < n; i++) {
			s = bp[0];
			for (j = 0; j < k; j++) {
			    if ((p - fnval) >= FNVALL) { /* Protect against */
				p = "";	             /* core dumps... */
				break;
			    } else *p++ = *s++;
			}
		    }
		}
	    }
	}
	for (k = 0; k < argn; k++) if (bp[k]) free(bp[k]);
	p = fnval;
	return(p);

      case FN_REV:
	p = fnval;
	yystring(bp[0],&p);
	for (k = 0; k < argn; k++) if (bp[k]) free(bp[k]);
	return(p);
	
      case FN_RPA:			/* RPAD and LPAD */
      case FN_LPA:
	*fnval = NUL;			/* Return value */
	if (argn == 1) {		/* If a number wasn't given */
	    p = fnval;			/* just return the original string */
	    strncpy(p,bp[0],FNVALL);
	} else if (chknum(bp[1])) {	/* Repeat count */
	    char pc;
	    n = atoi(bp[1]);
debug(F101,"pad n","",n);

	    if (n >= 0) {		/* Pad it out */
		p = fnval;
		k = strlen(bp[0]);	/* Length of string to be padded */
debug(F111,"pad bp[0]",bp[0],k);

		pc = (argn < 3) ? SP : *bp[2]; /* Padding character */

		if (n > FNVALL) n = FNVALL-1; /* protect against overruns */
		if (k > FNVALL) k = FNVALL-1; /* and silly args. */
                if (k > n) k = n;

debug(F101,"pad k","",k);
debug(F101,"pad n","",n);

		if (y == FN_RPA) {	/* RPAD */
		    strncpy(p,bp[0],k);
		    p += k;
		    for (i = k; i < n; i++)  
		      *p++ = pc;
		} else {		/* LPAD */
		    n -= k;
		    for (i = 0; i < n; i++)  
		      *p++ = pc;
		    strncpy(p,bp[0],k);
		    p += k;
		}
		*p = NUL;
	    }
	}
	for (k = 0; k < argn; k++) if (bp[k]) free(bp[k]);
	p = fnval;
debug(F110,"pad",p,0);
	return(p);

      default:
	return("");
    }
}
#endif /* NOSPL */


#ifndef NOSPL
#ifdef ATTSV
#include <sys/utsname.h>
#endif

char *					/* Evaluate builtin variable */
nvlook(s) char *s; {
    int x, y;
    long z;
    char *p, *g;
#ifdef ATTSV
    struct utsname hname;
#endif
    x = 30;
    p = vvbuf;
    if (xxstring(s,&p,&x) < 0) {
	y = -1;
    } else {
	s = vvbuf;
	if ((y = lookup(vartab,s,nvars,&x)) < 0) return(NULL);
    }
    switch (y) {
      case VN_ARGC:			/* ARGC */
	sprintf(vvbuf,"%d",macargc[maclvl]);
	return(vvbuf);

      case VN_ARGS:			/* ARGS */
	sprintf(vvbuf,"%d",xargs);
	return(vvbuf);

      case VN_COUN:			/* COUNT */
	sprintf(vvbuf,"%d",count[cmdlvl]);
	return(vvbuf);

      case VN_DATE:			/* DATE */
	ztime(&p);			/* Get "asctime" string */
	if (p == NULL || *p == NUL) return(NULL);
	vvbuf[0] = p[8];		/* dd */
	vvbuf[1] = p[9];
	vvbuf[2] = SP;
	vvbuf[3] = p[4];		/* mmm */
	vvbuf[4] = p[5];
	vvbuf[5] = p[6];
	vvbuf[6] = SP;
	for (x = 20; x < 24; x++)	/* yyyy */
	  vvbuf[x - 13] = p[x];
	vvbuf[11] = NUL;
	return(vvbuf);

      case VN_NDAT:			/* Numeric date */
	ztime(&p);			/* Get "asctime" string */
	if (p == NULL || *p == NUL) return(NULL);
	for (x = 20; x < 24; x++)	/* yyyy */
	  vvbuf[x - 20] = p[x];
        vvbuf[6] = (p[8] == ' ') ? '0' : p[8]; vvbuf[7] = p[9]; /* dd */
	for (x = 0; x < 12; x++)	  /* mm */
	  if (!strncmp(p+4,months[x],3)) break;
	if (x == 12) {
	    vvbuf[4] = vvbuf[5] = '?';
	} else {
	    x++;
	    vvbuf[4] = (x < 10) ? '0' : '1';
	    vvbuf[5] = (x % 10) + 48;
	}
	vvbuf[8] = NUL;
        return(vvbuf);

      case VN_DIRE:			/* DIRECTORY */
	return(zgtdir());

      case VN_FILE:			/* filespec */
	return(fspec);

      case VN_HOST:			/* host name */
	*vvbuf = NUL;
#ifdef ATTSV
	if (uname(&hname) > -1) strncpy(vvbuf,hname.nodename,VVBUFL);
#else
#ifdef BSD4
	if (gethostname(vvbuf,VVBUFL) < 0) *vvbuf = NUL;
#else
#ifdef VMS
	g = getenv("SYS$NODE");
	if (g) strncpy(vvbuf,g,VVBUFL);
	x = strlen(vvbuf);
	if (x > 1 && vvbuf[x-1] == ':' && vvbuf[x-2] == ':') vvbuf[x-2] = NUL;
#endif /* VMS */
#endif /* BSD4 */
#endif /* ATTSV */
	if (*vvbuf == NUL) {		/* If it's still empty */
	    g = getenv("HOST");		/* try this */
	    if (g) strncpy(vvbuf,g,VVBUFL);
	}
	if (*vvbuf == NUL)		/* And if it's still empty */
	  strcpy(vvbuf,"unknown");	/* just say "unknown" */
	vvbuf[VVBUFL-1] = NUL;		/* Make sure result is terminated. */
	return(vvbuf);

      case VN_SYST:			/* System type */
#ifdef UNIX	
	strcpy(vvbuf,"UNIX");
#else
#ifdef VMS
	strcpy(vvbuf,"VMS");
#else
	strcpy(vvbuf,"unknown");
#endif
#endif
	return(vvbuf);

      case VN_SYSV:			/* System version */
	for (x = y = 0; x < VVBUFL; x++) {
	    if (ckxsys[x] == SP && y == 0) continue;
	    vvbuf[y++] = (ckxsys[x] == SP) ? '_' : ckxsys[x];
	}
	vvbuf[y] = NUL;
	return(vvbuf);

      case VN_TIME:			/* TIME. Assumes that ztime returns */
	ztime(&p);			/* "Thu Feb  8 12:00:00 1990" */
	if (p == NULL || *p == NUL)	/* like asctime()! */
	  return(NULL);
	for (x = 11; x < 19; x++)	/* copy hh:mm:ss */
	  vvbuf[x - 11] = p[x];		/* to vvbuf */
	vvbuf[8] = NUL;			/* terminate */
	return(vvbuf);			/* and return it */

      case VN_NTIM:			/* Numeric time */
	ztime(&p);			/* "Thu Feb  8 12:00:00 1990" */
	if (p == NULL || *p == NUL)	/* like asctime()! */
	  return(NULL);
	z = atol(p+11) * 3600L + atol(p+14) * 60L + atol(p+17);
	sprintf(vvbuf,"%ld",z);
	return(vvbuf);

#ifdef UNIX
      case VN_TTYF:			/* TTY file descriptor */
	sprintf(vvbuf,"%d",ttyfd);
	return(vvbuf);
#endif

      case VN_VERS:			/* numeric version number */
	sprintf(vvbuf,"%ld",vernum);
	return(vvbuf);

      case VN_HOME:			/* Home directory */
#ifdef UNIX
        sprintf(vvbuf,"%s/",zhome());
	return(vvbuf);
#else
	return(zhome());
#endif

      case VN_IBUF:			/* INPUT buffer */
	return(inpbuf);

      case VN_SPEE: {			/* Transmission SPEED */
	  long t;
	  t = ttgspd();
	  if (t < 0L)
	    sprintf(vvbuf,"unknown");
	  else
	    sprintf(vvbuf,"%ld",t);
	  return(vvbuf);
      }
      case VN_SUCC:			/* SUCCESS flag */
	sprintf(vvbuf,"%d",(success == 0) ? 1 : 0);
	return(vvbuf);

      case VN_LINE:			/* LINE */
	p = (char *) ttname;
        return(p);

      case VN_PROG:			/* Program name */
	return("C-Kermit");

      case VN_RET:			/* Value of most recent RETURN */
	p = mrval[maclvl+1];
	if (p == NULL) p = "";
	return(p);

      default:
	return(NULL);
    }
}
#endif /* NOSPL */

/*
  X X S T R I N G  --  Expand variables and backslash codes.

    int xxtstring(s,&s2,&n);

  Expands \ escapes via recursive descent.
  Argument s is a pointer to string to expand (source).
  Argument s2 is the address of where to put result (destination).
  Argument n is the length of the destination string (to prevent overruns).
  Returns -1 on failure, 0 on success,
    with destination string null-terminated and s2 pointing to the
    terminating null, so that subsequent characters can be added.
*/

#define XXDEPLIM 100			/* Recursion depth limit */

int
xxstring(s,s2,n) char *s; char **s2; int *n; {
    int x,				/* Current character */
        y,				/* Worker */
        pp,				/* Paren level */
        argn,				/* Function argument counter */
        n2,				/* Local copy of n */
        d,				/* Array dimension */
        vbi,				/* Variable id (integer form) */
        argl;				/* String argument length */

    char vb,				/* Variable id (char form) */
        *vp,				/* Pointer to variable definition */
        *new,				/* Local pointer to target string */
        *p,				/* Worker */
        *q;				/* Worker */
    char *r  = (char *)0;		/* For holding function args */
    char *r2 = (char *)0;

#ifndef NOSPL
    char vnambuf[VNAML];		/* Buffer for variable/function name */
    char *argp[FNARGS];			/* Pointers to function args */
#endif /* NOSPL */

    static int depth = 0;		/* Call depth, avoid overflow */

    n2 = *n;				/* Make local copies of args */
    new = *s2;				/* for one less level of indirection */

    depth++;				/* Sink to a new depth */
    if (depth > XXDEPLIM) {		/* Too deep? */
	printf("?definition is circular or too deep\n");
	depth = 0;
	*new = NUL;
	return(-1);
    }
    if (!s || !new) {			/* Watch out for null pointers */
	depth = 0;
	*new = NUL;
	return(-1);
    }
    argl = strlen(s);			/* Get length of source string */
    debug(F111,"xxstring",s,argl);
    if (argl < 0) {			/* Watch out for garbage */
	depth = 0;
	*new = NUL;
	return(-1);
    }
    while ( x = *s ) {			/* Loop for all characters */
        if (x != CMDQ) {		/* Is it the command-quote char? */
	    *new++ = *s++;		/* No, normal char, just copy */
	    if (n2-- < 0) {		/* and count it, careful of overflow */
		return(-1); 
	    }
	    continue;
	}

/* We have the command-quote character. */

	x = *(s+1);			/* Get the following character. */
	switch (x) {			/* Act according to variable type */
#ifndef NOSPL
	  case '%':			/* Variable */
	    s += 2;			/* Get the letter or digit */
	    vb = *s++;			/* and move source pointer past it */
	    vp = NULL;			/* Assume definition is empty */
	    if (vb >= '0' && vb <= '9') { /* Digit for macro arg */
		if (maclvl < 0) 	/* Digit variables are global */
		  vp = g_var[vb];	/* if no macro is active */
		else			/* otherwise */
		  vp = m_arg[maclvl][vb - '0']; /* they're on the stack */
	    } else {
		if (isupper(vb)) vb -= ('a'-'A');
		vp = g_var[vb];		/* Letter for global variable */
	    }
	    if (vp) {			/* If definition not empty */
		if (xxstring(vp,&new,&n2) < 0) { /* call self to evaluate it */
		    return(-1);		/* Pass along failure */
		}
	    }
	    break;
	  case '&':			/* An array reference */
	    if (arraynam(s,&vbi,&d) < 0) { /* Get name and subscript */
		return(-1);
	    }
	    pp = 0;			/* Bracket counter */
	    while (*s) {		/* Advance source pointer */
		if (*s == '[') pp++;
		if (*s == ']' && --pp == 0) break;
		s++;
	    }
	    if (*s == ']') s++;		/* past the closing bracket. */
	    if (chkarray(vbi,d) > 0) {	/* Array is declared? */
		vbi -= 96;		/* Convert name to index */
		if (a_dim[vbi] >= d) {	/* If subscript in range */
		    char **ap;
		    ap = a_ptr[vbi];	/* get data pointer */
		    if (ap) {		/* and if there is one */
			if (ap[d]) {	/* If definition not empty */
			    if (xxstring(ap[d],&new,&n2) < 0) { /* evaluate */
				return(-1); /* Pass along failure */
			    }
			}
		    }
		}
	    }
	    break;

	  case 'F':			/* A builtin function */
	  case 'f':
	    q = vnambuf;		/* Copy the name */
	    y = 0;			/* into a separate buffer */
	    s+=2;			/* point past 'F' */
	    while (y++ < VNAML) {
		if (*s == '(') { s++; break; } /* Look for open paren */
		if ((*q = *s) == NUL) break;   /* or end of string */
		s++; q++;
	    }
	    *q = NUL;			/* Terminate function name */
	    if (y >= VNAML) {		/* Handle pathological case */
		while (*s && (*s != '(')) /* of very long string entered */
		  s++;			  /* as function name. */
		if (*s == ')') s++;	  /* Skip past it. */
	    }
	    r = r2 = malloc(argl+2);	/* And make a place to copy args */
	    debug(F101,"xxstring r2","",(int)r2);
	    if (!r2) {			/* Watch out for malloc failure */
		depth = 0;
		*new = NUL;
		return(-1);
	    }
	    argn = 0;			/* Argument counter */
	    argp[argn++] = r;		/* Point to first argument */
	    y = 0;			/* Completion flag */
	    pp = 1;			/* Paren level (already have one). */
	    while (*r = *s) {		/* Copy each argument, char by char. */
		if (*r == '(') pp++;	/* Count an opening paren. */
		if (*r == ')') {	/* Closing paren, count it. */
		    if (--pp == 0) {	/* Final one? */
			*r = NUL;	/* Make it a terminating null */
			s++;
			y = 1;		/* Flag we've got all the args */
			break;
		    }
		}
		if (*r == ',') {	/* Comma */
		    if (pp == 1) {	/* If not within ()'s, */
			*r = NUL;	    /* new arg, skip past it, */
			argp[argn++] = r+1; /* point to new arg. */
			if (argn == FNARGS) /* Too many args */
			  break;
		    }			/* Otherwise just skip past  */
		}
		s++; r++;		/* Advance pointers */
	    }
	    debug(F110,"xxstring function name",vnambuf,0);
	    if (!y) {			/* If we didn't find closing paren */
		debug(F101,"xxstring r2 before free","",(int)r2);
		if (r2) free(r2);	/* free the temporary storage */
		return(-1);		/* and return failure. */
	    }
#ifdef DEBUG
	    if (deblog)
	      for (y = 0; y < argn; y++)
		debug(F111,"xxstring function arg",argp[y],y);
#endif
	    vp = fneval(vnambuf,argp,argn); /* Evaluate the function. */
	    if (vp) {			/* If definition not empty */
		while (*new++ = *vp++)	/* copy it to output string */
		  if (n2-- < 0) return(-1); /* mindful of overflow */
		new--;			/* Back up over terminating null */
		n2++;			/* to allow for further deposits. */
	    }
	    if (r2) {
		debug(F101,"xxstring freeing r2","",(int)r2);
		free(r2);		/* Now free the temporary storage */
		r2 = NULL;
	    }
	    break;
	  case '$':			/* An environment variable */
	  case 'V':			/* Or a named builtin variable. */
	  case 'v':
	  case 'M':			/* Or a macro = long variable */
	  case 'm':
	    p = s+2;			/* $/V/M must be followed by (name) */
	    if (*p != '(') {		/* as in \$(HOME) or \V(count) */
		*new++ = *s++;		/* If not, just copy it */
		if (n2-- < 0) {
		    return(-1);
		}
		break;
	    }
	    p++;			/* Point to 1st char of name */
	    q = vnambuf;		/* Copy the name */
	    y = 0;			/* into a separate buffer */
	    while (y++ < VNAML) {	/* Watch out for name too long */
		if (*p == ')') {	/* Name properly terminated with ')' */
		    p++;		/* Move source pointer past ')' */
		    break;
		}
		if ((*q = *p) == NUL)	/* String ends before ')' */
		  break;
 		p++; q++;		/* Advance pointers */
	    }
	    *q = NUL;			/* Terminate the variable name */
	    if (y >= VNAML) {		/* Handle pathological case */
		while (*p && (*p != ')')) /* of very long string entered */
		  p++;			  /* as variable name. */
		if (*p == ')') p++;	  /* Skip ahead to the end of it. */
	    }
	    s = p;			/* Adjust global source pointer */
	    debug(F110,"xxstring vname",vnambuf,0);
	    if (x == '$') {		/* Look up its value */
		vp = getenv(vnambuf);	/* This way for environment variable */
	    } else if (x == 'm' || x == 'M') { /* or this way for macro */
		y = mlook(mactab,vnambuf,nmac);	/* contents (= long variable */
		vp = (y > -1) ? mactab[y].mval : ""; /* name)... */
	    } else { 			/*  or */
	        vp = nvlook(vnambuf);	/* this way for builtin variable */
	    }
	    if (vp) {			/* If definition not empty */
		while (*new++ = *vp++)	/* copy it to output string. */
		  if (n2-- < 0) {
		    return(-1);
		}
		new--;			/* Back up over terminating null */
		n2++;			/* to allow for further deposits. */
	    }
	    break;
#endif /* NOSPL	*/			/* Handle \nnn even if NOSPL. */
	  default:			/* Maybe it's a backslash code */
	    y = xxesc(&s);		/* Go interpret it */
	    if (y < 0) {		/* Upon failure */
		*new++ = x;		/* Just quote the next character */
		s += 2;			/* Move past the pair */
		n2 -= 2;
		if (n2 < 0) {
		    return(-1);
		}
		continue;		/* and go back for more */
	    } else {
		*new++ = y;		/* else deposit interpreted value */
		if (n2-- < 0) {
		    return(-1);
		}
	    }
	}
    }
    *new = NUL;				/* Terminate the new string */
    depth--;				/* Adjust stack depth gauge */
    *s2 = new;				/* Copy results back into */
    *n = n2;				/* the argument addresses */
    return(0);				/* and return. */
}

/*  X X S T R C M P  --  Caseless string comparison  */

int
xxstrcmp(s1,s2,n) char *s1, *s2; int n; { /* Caseless string comparison. */
    char t1, t2;			/*  1 if s1 > t1 */
    int n1, n2;				/*  0 if s1 = s2 */
					/* -1 if s1 < t1 */
    if (!s1) s1 = "";			/* Get lengths and watch out */
    n1 = strlen(s1);			/* for null pointers. */
    if (!s2) s2 = "";
    n2 = strlen(s2);

    if (n > n1) n = n1;
    if (n > n2) n = n2;

    while (n--) {
	t1 = *s1++;			/* Get next character from each. */
	if (isupper(t1)) t1 = tolower(t1);
	t2 = *s2++;
	if (isupper(t2)) t2 = tolower(t2);
	if (t1 < t2) return(-1);	/* s1 < s2 */
	if (t1 > t2) return(1);		/* s1 > s2 */
    }
    return(0);				/* They're equal */
}

#endif /* NOICP */
