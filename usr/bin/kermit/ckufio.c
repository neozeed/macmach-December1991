char *ckzv = "UNIX file support, 5A(054) 4 Nov 91";

/* C K U F I O  --  Kermit file system support for Unix systems */

/*
 Author: Frank da Cruz (fdc@columbia.edu, FDCCU@CUVMA.BITNET),
 Columbia University Center for Computing Activities.  Many other contributors.
 First released January 1985.
 Copyright (C) 1985, 1991, Trustees of Columbia University in the City of New 
 York.  Permission is granted to any individual or institution to use, copy, or
 redistribute this software so long as it is not sold for profit, provided this
 copyright notice is retained. 
*/

/* Include Files */

#include "ckcdeb.h"

#include <signal.h>
#ifdef MINIX
#include <limits.h>
#endif /* MINIX */
#ifdef POSIX
#include <limits.h>
#endif /* POSIX */

/* Directory structure header file */

#ifdef SDIRENT
#define DIRENT
#endif /* SDIRENT */

#ifdef XNDIR
#include <sys/ndir.h>
#else /* !XNDIR */
#ifdef NDIR
#include <ndir.h>
#else /* !NDIR, !XNDIR */
#ifdef RTU
#include "/usr/lib/ndir.h"
#else /* !RTU, !NDIR, !XNDIR */
#ifdef DIRENT
#ifdef SDIRENT
#include <sys/dirent.h>
#else
#include <dirent.h>
#endif /* SDIRENT */
#else /* !RTU, !NDIR, !XNDIR, !DIRENT, i.e. all others */
#include <sys/dir.h>
#endif /* DIRENT */
#endif /* RTU */
#endif /* NDIR */
#endif /* XNDIR */

#include <pwd.h>			/* Password file for shell name */

#ifdef BSD4				/* BSD 4.x */
#define TIMESTAMP			/* Can do file dates */
#include <time.h>			/* Need this */
#include <sys/timeb.h>			/* Need this if really BSD */
#endif /* BSD4 */

#ifdef ATTSV
#define TIMESTAMP
#include <time.h>
/* void tzset(); (the "void" type upsets some compilers) */
extern long timezone;
#endif /* ATTSV */
 
/* Is `y' a leap year? */
#define leap(y) (((y) % 4 == 0 && (y) % 100 != 0) || (y) % 400 == 0)

/* Number of leap years from 1970 to `y' (not including `y' itself). */
#define nleap(y) (((y) - 1969) / 4 - ((y) - 1901) / 100 + ((y) - 1601) / 400)

#ifdef COMMENT /* not used */
/* Number of days in each month of the year. */
static char monlens[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
#endif /* COMMENT */

#ifdef CIE
#include <stat.h>			/* File status */
#else
#include <sys/stat.h>
#endif /* CIE */

/*
  Functions (n is one of the predefined file numbers from ckcker.h):

   zopeni(n,name)   -- Opens an existing file for input.
   zopeno(n,name,attr,fcb) -- Opens a new file for output.
   zclose(n)        -- Closes a file.
   zchin(n,&c)      -- Gets the next character from an input file.
   zsinl(n,&s,x)    -- Read a line from file n, max len x, into address s.
   zsout(n,s)       -- Write a null-terminated string to output file, buffered.
   zsoutl(n,s)      -- Like zsout, but appends a line terminator.
   zsoutx(n,s,x)    -- Write x characters to output file, unbuffered.
   zchout(n,c)      -- Add a character to an output file, unbuffered.
   zchki(name)      -- Check if named file exists and is readable, return size.
   zchko(name)      -- Check if named file can be created.
   zchkspa(name,n)  -- Check if n bytes available to create new file, name.
   znewn(name,s)    -- Make a new unique file name based on the given name.
   zdelet(name)     -- Delete the named file.
   zxpand(string)   -- Expands the given wildcard string into a list of files.
   znext(string)    -- Returns the next file from the list in "string".
   zxcmd(n,cmd)     -- Execute the command in a lower fork on file number n.
   zclosf()         -- Close input file associated with zxcmd()'s lower fork.
   zrtol(n1,n2)     -- Convert remote filename into local form.
   zltor(n1,n2)     -- Convert local filename into remote form.
   zchdir(dirnam)   -- Change working directory.
   zhome()          -- Return pointer to home directory name string.
   zkself()         -- Kill self, log out own job.
   zsattr(struct zattr *) -- Return attributes for file which is being sent.
   zstime(f, struct zattr *, x) - Set file creation date from attribute packet.
   zrename(old, new) -- Rename a file.
 */

/* Kermit-specific includes */
/*
  Definitions here supersede those from system include files.
  ckcdeb.h is included above.
*/
#include "ckcker.h"			/* Kermit definitions */
#include "ckucmd.h"			/* For sys-dependent keyword tables */
#include "ckuver.h"			/* Version herald */

char *ckzsys = HERALD;

/* Support for tilde-expansion in file and directory names */

#ifdef POSIX
#define NAMEENV "LOGNAME"
#endif /* POSIX */

#ifdef BSD4
#define NAMEENV "USER"
#endif /* BSD4 */

#ifdef ATTSV
#define NAMEENV "LOGNAME"
#endif /* ATTSV */

/* Berkeley Unix Version 4.x */
/* 4.1bsd support from Charles E Brooks, EDN-VAX */

#ifdef BSD4
#ifdef MAXNAMLEN
#define BSD42
#endif /* MAXNAMLEN */
#endif /* BSD4 */

/* Definitions of some Unix system commands */

char *DELCMD = "rm -f ";		/* For file deletion */
char *PWDCMD = "pwd ";			/* For saying where I am */
#ifdef COMMENT
char *DIRCMD = "/bin/ls -ld ";		/* For directory listing */
char *DIRCM2 = "/bin/ls -ld *";		/* For directory listing, no args */
#else
char *DIRCMD = "/bin/ls -l ";		/* For directory listing */
char *DIRCM2 = "/bin/ls -l ";		/* For directory listing, no args */
#endif
char *TYPCMD = "cat ";			/* For typing a file */

#ifdef FT18
#undef BSD4
#endif

#ifdef BSD4
char *SPACMD = "pwd ; df .";		/* Space in current directory */
#else
#ifdef FT18
char *SPACMD = "pwd ; du ; df .";
#else
char *SPACMD = "df ";
#endif
#endif

char *SPACM2 = "df ";			/* For space in specified directory */

#ifdef FT18
#define BSD4
#endif

#ifdef BSD4
char *WHOCMD = "finger ";		/* For seeing who's logged in */
#else
char *WHOCMD = "who ";			/* For seeing who's logged in */
#endif

#ifdef DTILDE				/* For tilde expansion */
_PROTOTYP( char * tilde_expand, (char *) );
#endif /* DTILDE */

/* More system-dependent includes, which depend on symbols defined */
/* in the Kermit-specific includes.  Oh what a tangled web we weave... */

#ifdef COHERENT
#define NOFILEH
#endif /* COHERENT */

#ifdef MINIX
#define NOFILEH
#endif /* MINIX */

#ifdef aegis				/* <sys/file.h> */
#define NOFILEH
#endif /* aegis */

#ifdef unos
#define NOFILEH
#endif /* unos */

#ifndef NOFILEH
#include <sys/file.h>
#endif /* NOFILEH */


#ifndef is68k				/* Whether to include <fcntl.h> */
#ifndef BSD41				/* All but a couple UNIXes have it. */
#ifndef FT18
#ifndef COHERENT
#include <fcntl.h>
#endif /* COHERENT */
#endif /* FT18  */
#endif /* BSD41 */
#endif /* not is68k */

#ifdef COHERENT
#include <sys/fcntl.h>
#endif /* COHERENT */

#ifndef _POSIX_SOURCE
#ifndef NEXT
#ifndef SVR4
/* POSIX <pwd.h> already gave prototypes for these. */
_PROTOTYP( struct passwd * getpwnam, (char *) );
_PROTOTYP( struct passwd * getpwuid, (PWID_T) );
_PROTOTYP( struct passwd * getpwent, (void) );
#endif /* SVR4 */
#endif /* NEXT */
#endif /* _POSIX_SOURCE */

/* Define macros for getting file type */

#ifdef OXOS
/*
  Olivetti X/OS 2.3 has S_ISREG and S_ISDIR defined
  incorrectly, so we force their redefinition.
*/
#undef S_ISREG
#undef S_ISDIR
#endif /* OXOS */

#ifdef UTSV				/* Same deal for Amdah UTSV */
#undef S_ISREG
#undef S_ISDIR
#endif /* UTSV */

#ifndef S_ISREG
#define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)
#endif /* S_ISREG */
#ifndef S_ISDIR
#define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)
#endif /* S_ISDIR */

/* Define maximum length for a file name if not already defined */

#ifndef MAXNAMLEN
#ifdef sun
#define MAXNAMLEN 255
#else
#ifdef FILENAME_MAX
#define MAXNAMLEN FILENAME_MAX
#else
#ifdef NAME_MAX
#define MAXNAMLEN NAME_MAX
#else
#ifdef _POSIX_NAME_MAX
#define MAXNAMLEN _POSIX_NAME_MAX
#else
#ifdef _D_NAME_MAX
#define MAXNAMLEN _D_NAME_MAX
#else
#ifdef DIRSIZ
#define MAXNAMLEN DIRSIZ
#else
#define MAXNAMLEN 14
#endif /* DIRSIZ */
#endif /* _D_NAME_MAX */
#endif /* _POSIX_NAME_MAX */
#endif /* NAME_MAX */
#endif /* FILENAME_MAX */
#endif /* sun */
#endif /* MAXNAMLEN */

/* Longest pathname */

#ifdef MAXPATHLEN
#ifdef MAXPATH
#undef MAXPATH
#endif /* MAXPATH */
#define MAXPATH MAXPATHLEN
#else
#ifdef PATH_MAX
#define MAXPATH PATH_MAX
#else
#ifdef _POSIX_PATH_MAX
#define MAXPATH _POSIX_PATH_MAX
#else
#ifdef BSD42
#define MAXPATH 1024
#else
#define MAXPATH 255
#endif /* BSD42 */
#endif /* _POSIX_PATH_MAX */
#endif /* PATH_MAX */
#endif /* MAXPATHLEN */

/* Maximum number of filenames for wildcard expansion */

#ifdef PROVX1
#define MAXWLD 50
#else
#ifdef BSD29
#define MAXWLD 50
#else
#ifdef pdp11
#define MAXWLD 50
#else
#define MAXWLD 1000
#endif /* pdp11 */
#endif /* BSD29 */
#endif /* PROVX1 */

/* More internal function prototypes */
/*
 * The path structure is used to represent the name to match.
 * Each slash-separated segment of the name is kept in one
 * such structure, and they are linked together, to make
 * traversing the name easier.
 */
struct path {
    char npart[MAXNAMLEN+4];		/* name part of path segment */
    struct path *fwd;			/* forward ptr */
};
_PROTOTYP( int shxpand, (char *, char **, int ) );
_PROTOTYP( static int fgen, (char *, char **, int ) );
_PROTOTYP( static VOID traverse, (struct path *, char *, char *) );
_PROTOTYP( static VOID addresult, (char *) );
_PROTOTYP( static int match, (char *, char *) );
_PROTOTYP( static char * whoami, (void) );
_PROTOTYP( char * xindex, (char *, char) );
_PROTOTYP( UID_T real_uid, (void) );

/* Some systems define these symbols in include files, others don't... */

#ifndef R_OK
#define R_OK 4				/* For access */
#endif

#ifndef W_OK
#define W_OK 2
#endif

#ifndef O_RDONLY
#define O_RDONLY 000
#endif

/* Declarations */

int maxnam = MAXNAMLEN;			/* Available to the outside */
int maxpath = MAXPATH;

FILE *fp[ZNFILS] = { 			/* File pointers */
    NULL, NULL, NULL, NULL, NULL, NULL, NULL };

/* (PWP) external def. of things used in buffered file input and output */
#ifdef DYNAMIC
extern CHAR *zinbuffer, *zoutbuffer;
#else
extern CHAR zinbuffer[], zoutbuffer[];
#endif /* DYNAMIC */
extern CHAR *zinptr, *zoutptr;
extern int zincnt, zoutcnt;
extern int wildxpand;

extern UID_T real_uid();

static long iflen = -1;			/* Input file length */

static PID_T pid = 0;			/* pid of child fork */
static int fcount;			/* Number of files in wild group */
static char nambuf[MAXNAMLEN+4];	/* Buffer for a filename */
static char zmbuf[200];			/* For mail, remote print strings */

/* static */				/* Not static, must be global now. */
char *mtchs[MAXWLD],			/* Matches found for filename */
     **mtchptr;				/* Pointer to current match */

/*  Z K S E L F  --  Kill Self: log out own job, if possible.  */

/* Note, should get current pid, but if your system doesn't have */
/* getppid(), then just kill(0,9)...  */

#ifndef SVR3
#ifndef POSIX
/* Already declared in unistd.h for SVR3 and POSIX */
#ifdef CK_ANSIC
extern PID_T getppid(void);
#else
extern PID_T getppid();
#endif /* CK_ANSIC */
#endif /* POSIX */
#endif /* SVR3 */
int
zkself() {				/* For "bye", but no guarantee! */
#ifdef PROVX1
    return(kill(0,9));
#else
#ifdef V7
    return(kill(0,9));
#else
#ifdef TOWER1
    return(kill(0,9));
#else
#ifdef FT18
    return(kill(0,9));
#else
#ifdef aegis
    return(kill(0,9));
#else
#ifdef COHERENT
    return(kill(getpid(),1));    
#else
    exit(kill(getppid(),1));
#endif
#endif
#endif
#endif
#endif
#endif
}

/*  Z O P E N I  --  Open an existing file for input. */

int
zopeni(n,name) int n; char *name; {
    debug(F111," zopeni",name,n);
    debug(F101,"  fp","",(int) fp[n]);
    if (chkfn(n) != 0) return(0);
    zincnt = 0;				/* Reset input buffer */
    if (n == ZSYSFN) {			/* Input from a system function? */
/*** Note, this function should not be called with ZSYSFN ***/
/*** Always call zxcmd() directly, and give it the real file number ***/
/*** you want to use.  ***/
        debug(F110,"zopeni called with ZSYSFN, failing!",name,0);
	*nambuf = '\0';			/* No filename. */
	return(0);			/* fail. */
#ifdef COMMENT
	return(zxcmd(n,name));		/* Try to fork the command */
#endif
    }
    if (n == ZSTDIO) {			/* Standard input? */
	if (isatty(0)) {
	    fprintf(stderr,"Terminal input not allowed");
	    debug(F110,"zopeni: attempts input from unredirected stdin","",0);
	    return(0);
	}
	fp[ZIFILE] = stdin;
	return(1);
    }
    fp[n] = fopen(name,"r");		/* Real file, open it. */
    debug(F111," zopeni", name, (int) fp[n]);
    if (fp[n] == NULL) perror("zopeni");
    return((fp[n] != NULL) ? 1 : 0);
}

/*  Z O P E N O  --  Open a new file for output.  */

int
zopeno(n,name,zz,fcb)
/* zopeno */  int n; char *name; struct zattr *zz; struct filinfo *fcb; {
    
    char *p;				/* Local-use pointer */

/* As of Version 5A, the attribute structure and the file information */
/* structure are included in the arglist. */

    debug(F111,"zopeno",name,n);
    if (fcb) {	
	debug(F101,"zopeno fcb disp","",fcb->dsp);
	debug(F101,"zopeno fcb type","",fcb->typ);
	debug(F101,"zopeno fcb char","",fcb->cs);
    } else {
	debug(F100,"zopeno fcb is NULL","",0);
    }
    if (n != ZDFILE)
      debug(F111," zopeno",name,n);
    if (chkfn(n) != 0) return(0);
    if ((n == ZCTERM) || (n == ZSTDIO)) {   /* Terminal or standard output */
	fp[ZOFILE] = stdout;
	if (n != ZDFILE)
	  debug(F101," fp[]=stdout", "", (int) fp[n]);
	zoutcnt = 0;
	zoutptr = zoutbuffer;
	return(1);
    }

/* A real file.  Open it in desired mode (create or append). */

    p = "w";				/* Assume write/create mode */
    if (fcb) {				/* If called with an FCB... */
	if (fcb->dsp == XYFZ_A)		/* Does it say Append? */
	  p = "a";			/* Yes. */
    }
    fp[n] = fopen(name,p);		/* Open the file */

    if (fp[n] == NULL) {
        perror("zopeno can't open");
    } else {
        if (n == ZDFILE) setbuf(fp[n],NULL); /* Debugging file unbuffered */
    }
    zoutcnt = 0;		/* (PWP) reset output buffer */
    zoutptr = zoutbuffer;
    if (n != ZDFILE)
      debug(F101, " fp[n]", "", (int) fp[n]);
    return((fp[n] != NULL) ? 1 : 0);
}

/*  Z C L O S E  --  Close the given file.  */

/*  Returns 0 if arg out of range, 1 if successful, -1 if close failed.  */

int
zclose(n) int n; {
    int x, x2;
    if (chkfn(n) < 1) return(0);	/* Check range of n */

    if ((n == ZOFILE) && (zoutcnt > 0))	/* (PWP) output leftovers */
      x2 = zoutdump();
    else
      x2 = 0;

    x = 0;				/* Initialize return code */
    if (fp[ZSYSFN]) {			/* If file is really pipe */
    	x = zclosf(n);			/* do it specially */
    } else {
    	if ((fp[n] != stdout) && (fp[n] != stdin)) x = fclose(fp[n]);
	fp[n] = NULL;
    }
    iflen = -1;				/* Invalidate file length */
    if (x == EOF)			/* if we got a close error */
	return (-1);
    else if (x2 < 0)		/* or an error flushing the last buffer */
	return (-1);		/* then return an error */
    else
	return (1);
}

/*  Z C H I N  --  Get a character from the input file.  */

/*  Returns -1 if EOF, 0 otherwise with character returned in argument  */

int
zchin(n,c) int n; int *c; {
    int a, x;

    /* (PWP) Just in case this gets called when it shouldn't. */
    if (n == ZIFILE) {
	x = zminchar();
	*c = x;
	return(x);
    }
    /* if (chkfn(n) < 1) return(-1); */
    a = getc(fp[n]);
    if (a == EOF) return(-1);
    *c = (CHAR) a & 0377;
    return(0);
}

/*  Z S I N L  --  Read a line from a file  */

/*
  Writes the line into the address provided by the caller.
  n is the Kermit "channel number".
  Writing terminates when newline is encountered, newline is not copied.
  Writing also terminates upon EOF or if length x is exhausted.
  Returns 0 on success, -1 on EOF or error.
*/
int
zsinl(n,s,x) int n, x; char *s; {
    int a, z = 0;

    if (chkfn(n) < 1) {			/* Make sure file is open */
	return(-1);
    }
    a = -1;
    while (x--) {
#ifndef NLCHAR
	int old;
	old = a;			/* Previous character */
#endif
	if (zchin(n,&a) < 0) {		/* Read a character from the file */
	    z = -1;
	    break;
	}
#ifdef NLCHAR
	if (a == (char) NLCHAR) break;	/* Single-character line terminator */
#else
	if (a == '\r') continue;	/* CRLF line terminator */
	if (old == '\r') {
	    if (a == '\n') break;
	    else *s++ = '\r';
	}
#endif /* NLCHAR */
	*s = a;
	s++;
    }
    *s = '\0';
    return(z);
}

/*
 * (PWP) (re)fill the buffered input buffer with data.  All file input
 * should go through this routine, usually by calling the zminchar()
 * macro (in ckcker.h).
 */

/*
 * Suggestion: if fread() returns 0, call ferror to find out what the
 * problem was.  If it was not EOF, then return -2 instead of -1.
 * Upper layers (getpkt function in ckcfns.c) should set cxseen flag
 * if it gets -2 return from zminchar macro.
 */
int
zinfill() {
    int x;

    errno = 0;
    zincnt = fread(zinbuffer, sizeof (char), INBUFSIZE, fp[ZIFILE]);
#ifdef COMMENT
    debug(F101,"zinfill fp","",fp[ZIFILE]);
    debug(F101,"zinfill zincnt","",zincnt);
#endif
    if (zincnt == 0) {
#ifndef UTEK
#ifdef ferror
	x = ferror(fp[ZIFILE]);
	debug(F101,"zinfill errno","",errno);
	debug(F101,"zinfill ferror","",x);
	if (x) return(-2);
#endif /* ferror */
#else
 	x = feof(fp[ZIFILE]);
 	debug(F101,"zinfill errno","",errno);
 	debug(F101,"zinfill feof","",x);
 	if (!x && ferror(fp[ZIFILE])) return(-2);
#endif /* UTEK */
	return(-1);
    }
    zinptr = zinbuffer;	   /* set pointer to beginning, (== &zinbuffer[0]) */
    zincnt--;			/* one less char in buffer */
    return((int)(*zinptr++) & 0377); /* because we return the first */
}

/*  Z S O U T  --  Write a string out to the given file, buffered.  */

int
zsout(n,s) int n; char *s; {
    if (chkfn(n) < 1) return(-1); /* Keep this here, prevents memory faults */
#ifdef COMMENT
    while (*s) {			/* (unbuffered for debugging) */
	write(fileno(fp[n]),s,1); ++s;
    }
#endif
#ifdef COMMENT
    return(fputs(s,fp[n]) == EOF ? -1 : 0);
#else
    if (n == ZSFILE)
	return(write(fileno(fp[n]),s,strlen(s)));
    else	       
        return(fputs(s,fp[n]) == EOF ? -1 : 0);
#endif
}

/*  Z S O U T L  --  Write string to file, with line terminator, buffered  */

int
zsoutl(n,s) int n; char *s; {
    /* if (chkfn(n) < 1) return(-1); */
    if (fputs(s,fp[n]) == EOF) return(-1);
    if (fputs("\n",fp[n]) == EOF) return(-1);
    return(0);
}

/*  Z S O U T X  --  Write x characters to file, unbuffered.  */

int
zsoutx(n,s,x) int n, x; char *s; {
#ifdef COMMENT
    if (chkfn(n) < 1) return(-1);
    return(write(fp[n]->_file,s,x));
#endif
    return(write(fileno(fp[n]),s,x) == x ? x : -1);
}


/*  Z C H O U T  --  Add a character to the given file.  */

/*  Should return 0 or greater on success, -1 on failure (e.g. disk full)  */

int
#ifdef CK_ANSIC
zchout(register int n, char c)
#else
zchout(n,c) register int n; char c;
#endif /* CK_ANSIC */
/* zchout() */ {
    /* if (chkfn(n) < 1) return(-1); */
    if (n == ZSFILE) {			/* Use unbuffered for session log */
    	return(write(fileno(fp[n]),&c,1) == 1 ? 0 : -1);
    } else {				/* Buffered for everything else */
	if (putc(c,fp[n]) == EOF)	/* If true, maybe there was an error */
	  return(ferror(fp[n])?-1:0);	/* Check to make sure */
	else				/* Otherwise... */
	  return(0);			/* There was no error. */
    }
}

/* (PWP) buffered character output routine to speed up file IO */

int
zoutdump() {
    int x;
    zoutptr = zoutbuffer;		/* Reset buffer pointer in all cases */
    debug(F101,"zoutdump chars","",zoutcnt);
    if (zoutcnt == 0) {			/* Nothing to output */
	return(0);
    } else if (zoutcnt < 0) {		/* Unexpected negative argument */
	zoutcnt = 0;			/* Reset output buffer count */
	return(-1);			/* and fail. */
    }
    
/* Frank Prindle suggested that replacing this fwrite() by an fflush() */
/* followed by a write() would improve the efficiency, especially when */
/* writing to stdout.  Subsequent tests showed a 5-fold improvement!   */
/* if (x = fwrite(zoutbuffer, 1, zoutcnt, fp[ZOFILE])) {              */

    fflush(fp[ZOFILE]);
    if ((x = write(fileno(fp[ZOFILE]),zoutbuffer,zoutcnt)) == zoutcnt) {
	debug(F101,"zoutdump write ok","",zoutcnt);
	zoutcnt = 0;			/* Reset output buffer count */
	return(0);			/* write() worked OK */
    } else {
	debug(F101,"zoutdump write error","",errno);
	debug(F101,"zoutdump write returns","",x);
	zoutcnt = 0;			/* Reset output buffer count */
	return(-1);			/* write() failed */
    }
}

/*  C H K F N  --  Internal function to verify file number is ok  */

/*
 Returns:
  -1: File number n is out of range
   0: n is in range, but file is not open
   1: n in range and file is open
*/
int
chkfn(n) int n; {
    switch (n) {
	case ZCTERM:
	case ZSTDIO:
	case ZIFILE:
	case ZOFILE:
	case ZDFILE:
	case ZTFILE:
	case ZPFILE:
	case ZSFILE:
	case ZSYSFN:
        case ZRFILE:
        case ZWFILE: break;
	default:
	    debug(F101,"chkfn: file number out of range","",n);
	    fprintf(stderr,"?File number out of range - %d\n",n);
	    return(-1);
    }
    return( (fp[n] == NULL) ? 0 : 1 );
}

/*  Z C H K I  --  Check if input file exists and is readable  */

/*
  Returns:
   >= 0 if the file can be read (returns the size).
     -1 if file doesn't exist or can't be accessed,
     -2 if file exists but is not readable (e.g. a directory file).
     -3 if file exists but protected against read access.
*/
/*
 For Berkeley Unix, a file must be of type "regular" to be readable.
 Directory files, special files, and symbolic links are not readable.
*/
long
zchki(name) char *name; {
    struct stat buf;
    int x;

    x = stat(name,&buf);
    if (x < 0) {
	debug(F111,"zchki stat fails",name,errno);
	return(-1);
    }
    if (!S_ISREG (buf.st_mode)) {	/* Must be regular file */
	debug(F111,"zchki skipping:",name,x);
	return(-2);
    }
    debug(F111,"zchki stat ok:",name,x);

    if ((x = access(name,R_OK)) < 0) { 	/* Is the file accessible? */
	debug(F111," access failed:",name,x); /* No */
    	return(-3);			
    } else {
	iflen = buf.st_size;		      /* Yes, remember size */
	strncpy(nambuf,name,MAXNAMLEN);	      /* and name globally. */
	debug(F111," access ok:",name,(int) iflen);
	return( (iflen > -1) ? iflen : 0 );
    }
}

/*  Z C H K O  --  Check if output file can be created  */

/*
 Returns -1 if write permission for the file would be denied, 0 otherwise.
*/
int
zchko(name) char *name; {
    int i, x;
    char *s;

    if (!name) return(-1);		/* Watch out for null pointer. */
    x = strlen(name);			/* Get length of filename */
    debug(F101," length","",x);
    s = malloc(x+3);			/* Must copy because we can't */
    if (!s) {				/* write into our argument. */
	fprintf(stderr,"Malloc error 46\n");
	return(-1);
    }
    strcpy(s,name);

    for (i = x; i > 0; i--)		/* Strip filename from right. */
      if (s[i-1] == '/') break;
    debug(F101," i","",i);

#ifdef COMMENT
/* X/OPEN XPG3-compliant systems fail if argument ends with "/"...  */
    if (i == 0)				/* If no path, use current directory */
      strcpy(s,"./");			
    else				/* Otherwise, use given one. */
      s[i] = '\0';
#else
/* So now we use "path/." if path given, or "." if no path given. */
    s[i++] = '.';			/* Append "." to path. */
    s[i] = '\0';
#endif /* COMMENT */

    x = access(s,W_OK);			/* Check access of path. */
    if (x < 0)
      debug(F111,"zchko access failed:",s,errno);
    else
      debug(F111,"zchko access ok:",s,x);
    free(s);				/* Free temporary storage */
    return((x < 0) ? -1 : 0);		/* and return. */
}

/*  Z D E L E T  --  Delete the named file.  */

int
zdelet(name) char *name; {
    return(unlink(name));
}


/*  Z R T O L  --  Convert remote filename into local form  */

/*  For UNIX, this means changing uppercase letters to lowercase.  */

VOID
zrtol(name,name2) char *name, *name2; {
    for ( ; *name != '\0'; name++) {
    	*name2++ = isupper(*name) ? tolower(*name) : *name;
    }
    *name2 = '\0';
    debug(F110,"zrtol:",name2,0);
}


/*  Z S T R I P  --  Strip device & directory name from file specification */

/*  Strip pathname from filename "name", return pointer to result in name2 */

char work[100];	/* buffer for use by zstrip and zltor */

VOID
zstrip(name,name2) char *name, **name2; {
    char *cp, *pp;
    debug(F110,"zstrip before",name,0);
    pp = work;
#ifdef DTILDE
    if (*name == '~') name++;
#endif
    for (cp = name; *cp != '\0'; cp++) {
    	if (*cp == '/')
	  pp = work;
	else
	  *pp++ = *cp;
    }
    *pp = '\0';				/* Terminate the string */
    *name2 = work;
    debug(F110,"zstrip after",*name2,0);
}

/*  Z L T O R  --  Local TO Remote */

VOID
zltor(name,name2) char *name, *name2; {
    char *cp, *pp;
    int dc = 0;
#ifdef aegis
    char *namechars;
    int tilde = 0, bslash = 0;

    if ((namechars = getenv("NAMECHARS")) != NULL) {
    	if (xindex(namechars, '~' ) != NULL) tilde  = '~';
    	if (xindex(namechars, '\\') != NULL) bslash = '\\';
    } else {
        tilde = '~';
        bslash = '\\';
    }
#endif

    debug(F110,"zltor",name,0);
    pp = work;
#ifdef aegis
    cp = name;
    if (tilde && *cp == tilde)
    	++cp;
    for (; *cp != '\0'; cp++) {
    	if (*cp == '/' || *cp == bslash) {	/* strip path name */
#else
    for (cp = name; *cp != '\0'; cp++) {	/* strip path name */
    	if (*cp == '/') {
#endif
	    dc = 0;
	    pp = work;
	}
	else if (islower(*cp)) *pp++ = toupper(*cp); /* Uppercase letters */
	else if (*cp == '~') *pp++ = 'X';	/* Change tilde to 'X' */
	else if (*cp == '#') *pp++ = 'X';	/* Change number sign to 'X' */
	else if ((*cp == '.') && (++dc > 1)) *pp++ = 'X'; /* & extra dots */
	else *pp++ = *cp;
    }
    *pp = '\0';				/* Tie it off. */
    cp = name2;				/* If nothing before dot, */
    if (*work == '.') *cp++ = 'X';	/* insert 'X' */
    strcpy(cp,work);
    debug(F110," name2",name2,0);
}    


/*  Z C H D I R  --  Change directory  */
/*
  Call with:
    dirnam = pointer to name of directory to change to,
      which may be "" or NULL to indicate user's home directory.
  Returns:
    0 on failure
    1 on success
*/
int
zchdir(dirnam) char *dirnam; {
    char *hd, *sp, *p;

    debug(F110,"zchdir",dirnam,0);
    if (dirnam == NULL || dirnam == "" || *dirnam == '\0') /* If arg is null */
      dirnam = getenv("HOME");		/* use user's home directory. */
    sp = dirnam;
    debug(F110,"zchdir 2",dirnam,0);

#ifdef DTILDE
    hd = tilde_expand(dirnam);		/* Attempt to expand tilde */
    if (*hd == '\0') hd = dirnam;	/* in directory name. */
#else
    hd = dirnam;
#endif /* DTILDE */
    debug(F110,"zchdir 3",hd,0);
    if (chdir(hd) > -1) return(1);	/* Try to cd */
    p = sp;				/* Failed, lowercase it. */
    while (*p) {
	if (isupper(*p)) *p = tolower(*p);
	p++;
    }
    debug(F110,"zchdir 4",hd,0);
#ifdef DTILDE
    hd = tilde_expand(sp);		/* Try again to expand tilde */
    if (*hd == '\0') hd = sp;
#else
    hd = sp;				/* Point to result */
#endif
    debug(F110,"zchdir 5",hd,0);
    return((chdir(hd) > -1) ? 1 : 0);
}


/*  Z H O M E  --  Return pointer to user's home directory  */

char *
zhome() {
    return(getenv("HOME"));
}

/*  Z G T D I R  --  Return pointer to user's current directory  */

#ifdef MAXPATHLEN
#define CWDBL MAXPATHLEN
#else
#define CWDBL 100
#endif
static char cwdbuf[CWDBL+1];

char *
zgtdir() {
    char *buf;

#ifdef SVORPOSIX
    extern char *getcwd();
    buf = cwdbuf;
    return(getcwd(buf,CWDBL));
#else
#ifdef BSD4
    extern char *getwd();
    buf = cwdbuf;
    return(getwd(buf));
#else
    return("directory unknown");
#endif
#endif
}

/*  Z X C M D -- Run a system command so its output can be read like a file */

int
zxcmd(filnum,comand) int filnum; char *comand; {
    int pipes[2];
    int out;

    if (chkfn(filnum) < 0) return(-1);	/* Need a valid Kermit file number. */
    if (filnum == ZSTDIO || filnum == ZCTERM) /* But not one of these. */
      return(0);

    out = (filnum == ZIFILE || filnum == ZRFILE) ? 0 : 1 ;

/* Output to a command */

    if (out) {				/* Need popen() to do this. */
#ifdef NOPOPEN
	return(0);			/* no popen(), fail. */
#else
/* Use popen() to run the command. */

#ifdef _POSIX_SOURCE
/* Strictly speaking, popen() is not available in POSIX.1 */
#define DCLPOPEN
#endif /* _POSIX_SOURCE */

#ifdef DCLPOPEN
/* popen() needs declaring because it's not declared in <stdio.h> */
	FILE *popen();
#endif

	if (priv_chk() || ((fp[filnum] = popen(comand,"w")) == NULL))
	  return(0);
	else return(1);
#endif /* NOPOPEN */	
    }

/* Input from a command */

    if (pipe(pipes) != 0) {
	debug(F100,"zxcmd pipe failure","",0);
	return(0);			/* can't make pipe, fail */
    }

/* Create a fork in which to run the named process */

#ifdef aegis
    if ((pid = vfork()) == 0) {		/* child */
#else
    if ((pid = fork()) == 0) {		/* child */
#endif

/* We're in the fork. */

	char *shpath, *shname, *shptr;	/* Find user's preferred shell */
#ifndef aegis
	struct passwd *p;
	char *defshell = "/bin/sh";	/* default shell */
#endif
	if (priv_can()) exit(1);	/* Turn off any privileges! */
	debug(F101,"zxcmd pid","",pid);
	close(pipes[0]);		/* close input side of pipe */
	close(0);			/* close stdin */
	if (open("/dev/null",0) < 0) return(0); /* replace input by null */
#ifndef SVORPOSIX
	dup2(pipes[1],1);		/* BSD: replace stdout & stderr */
	dup2(pipes[1],2);		/* by the pipe */
#else
	close(1);			/* AT&T: close stdout */
	if ( dup(pipes[1]) != 1 )	/* Send stdout to the pipe */
	  return(0);
	close(2);			/* Send stderr to the pipe */
	if ( dup(pipes[1]) != 2 )
	  return(0);
#endif
	close(pipes[1]);		/* Don't need this any more. */

#ifdef aegis
	if ((shpath = getenv("SERVERSHELL")) == NULL) shpath = "/bin/sh";
#else
	shpath = getenv("SHELL");	/* What shell? */
	if (shpath == NULL) {
	    p = getpwuid( real_uid() );	/* Get login data */
	    if (p == (struct passwd *)NULL || !*(p->pw_shell))
	      shpath = defshell;
	    else shpath = p->pw_shell;
        }
#endif
	shptr = shname = shpath;
	while (*shptr != '\0')
	  if (*shptr++ == '/')
	    shname = shptr;
	debug(F100,"zxcmd...","",0);
	debug(F110,shpath,shname,0);

	execl(shpath,shname,"-c",comand,(char *)NULL); /* Execute the cmd */
	exit(0);			/* just punt if it failed. */
    } else if (pid == -1) {
	debug(F100,"zxcmd fork failure","",0);
	return(0);
    }
    debug(F101,"zxcmd pid","",pid);
    if (out) {
	close(pipes[0]);		/* Don't need the input side */
	fp[filnum] = fdopen(pipes[1],"w"); /* Open a stream for output. */
	fp[ZSYSFN] = fp[filnum];	/* Remember. */
	zoutcnt = 0;			/* (PWP) reset input buffer */
	zoutptr = zoutbuffer;
    } else {
	close(pipes[1]);		/* Don't need the output side */
	fp[filnum] = fdopen(pipes[0],"r"); /* Open a stream for input. */
	fp[ZSYSFN] = fp[filnum];	/* Remember. */
	zincnt = 0;			/* (PWP) reset input buffer */
	zinptr = zinbuffer;
    }
    return(1);
}

/*  Z C L O S F  - wait for the child fork to terminate and close the pipe. */

int
zclosf(filnum) int filnum; {
    int wstat;
    debug(F101,"zclosf filnum","",filnum);
#ifndef NOPOPEN
    if (filnum == ZWFILE) {
	int x;
	x = pclose(fp[filnum]);
	fp[filnum] = fp[ZSYSFN] = NULL;
	return((x < 0) ? 0 : 1);
    }
#endif
    debug(F101,"zclosf fp[filnum]","",(int) fp[filnum]);
    debug(F101,"zclosf fp[ZSYSFN]","",(int) fp[ZSYSFN]);
    if (pid != 0) {
	debug(F101,"zclosf killing pid","",pid);
	kill(pid,9);
        while ((wstat = wait((WAIT_T *)0)) != pid && wstat != -1) ;
        pid = 0;
    }
    fclose(fp[filnum]);
    fp[filnum] = fp[ZSYSFN] = NULL;
    return(1);
}

/*  Z X P A N D  --  Expand a wildcard string into an array of strings  */
/*
  Returns the number of files that match fn1, with data structures set up
  so that first file (if any) will be returned by the next znext() call.
  Depends on external variable wildxpand: 0 means we expand wildcards 
  internally, nonzero means we call the shell to do it.
*/

int
zxpand(fn) char *fn; {
    char *p;

#ifdef DTILDE				/* Built with tilde-expansion? */
    char *tnam;
#endif /* DTILDE */
    debug(F111,"zxpand entry",fn,wildxpand);
#ifdef DTILDE				/* Built with tilde-expansion? */
    if (*fn == '~') {			/* Starts with tilde? */
	tnam = tilde_expand(fn);	/* Try to expand it. */
	if (tnam) fn = tnam;
    }
    debug(F110,"zxpand after tilde_x",fn,0);
#endif /* DTILDE */
    if (wildxpand)			/* Who is expanding wildcards? */
      fcount = shxpand(fn,mtchs,MAXWLD); /* Shell */
    else
      fcount = fgen(fn,mtchs,MAXWLD);	/* Kermit */
    if (fcount > 0) {
	mtchptr = mtchs;		/* Save pointer for next. */
    }
    if (fcount > 0) {
	debug(F111,"zxpand ok",mtchs[0],fcount);
	return(fcount);
    }
    debug(F111,"zxpand fgen1",fn,fcount); /* Didn't get one, or got too many */
    p = malloc(strlen(fn) + 10);	/* Make space */
    if (!p) return(0);
    zrtol(fn,p);			/* Try again, maybe lowercase */
    if (wildxpand)
      fcount = shxpand(p,mtchs,MAXWLD); /* Shell */
    else
      fcount = fgen(p,mtchs,MAXWLD);	/* Kermit */
    if (fcount > 0) {			/* Got one? */
	mtchptr = mtchs;		/* Save pointer for next. */
	debug(F111,"zxpand fgen2 ok",mtchs[0],fcount);
    } else debug(F111,"zxpand 2 not ok",p,fcount);
    free(p);
    return(fcount);
}


/*  Z N E X T  --  Get name of next file from list created by zxpand(). */
/*
 Returns >0 if there's another file, with its name copied into the arg string,
 or 0 if no more files in list.
*/
int
znext(fn) char *fn; {
    if (fcount-- > 0) strcpy(fn,*mtchptr++);
    else *fn = '\0';
    debug(F111,"znext",fn,fcount+1);
    return(fcount+1);
}


/*  Z C H K S P A  --  Check if there is enough space to store the file  */

/*
 Call with file specification f, size n in bytes.
 Returns -1 on error, 0 if not enough space, 1 if enough space.
*/
int
zchkspa(f,n) char *f; long n; {		/* Just dummy for now. */
    return(1);				/* Always say OK. */
}


/*  Z N E W N  --  Make a new name for the given file  */

/*
  Given the name, fn, of a file that already exists, this function builds a
  new name of the form "<oldname>.~<n>~", where <oldname> is argument name
  (fn), and <n> is a version number, one higher than any existing version
  number for that file, up to 9999.  This format is consistent with that used
  by GNU EMACS.  If the constructed name is too long for the system's maximum,
  enough characters are truncated from the end of <fn> to allow the version
  number to fit.  If no free version numbers exist between 1 and 9999, a
  version number of "xxxx" is used.  Returns a pointer to the new name in 
  argument s. 
*/

VOID
znewn(fn,s) char *fn, **s; {
#define ZNEWNBL 255			/* Name buffer length */
#define ZNEWNMD 4			/* Max digits for version number */
    static char buf[ZNEWNBL+1];

    char *bp, *xp;
    int len = 0, d = 0, n, t, i, j, k, power = 1;

    int max = MAXNAMLEN;		/* Maximum name length */

    if (max < 14) max = 14;		/* Make it reasonable */
    if (max > ZNEWNBL) max = ZNEWNBL;
    bp = buf;				/* Buffer for building new name */
    while (*fn) {			/* Copy old name into buffer */
	*bp++ = *fn++;
	if (len++ > ZNEWNBL) break;	/* ...up to buffer length */
    }
    for (i = 1; i < ZNEWNMD + 1; i++) {	/* Version numbers up to 10**i - 1 */
	power *= 10;			/* Next power of 10 */
	j = max - len;			/* Space left for version number */
	k = 3 + i;			/* Space needed for it */
	if (j < k) {			/* Make room if necessary */
	    len -= (k - j);		/* Adjust length of filename */
	    bp = buf + len;		/* Point to new end */
	}
	*bp++ = '*';			/* Put a star on the end (UNIX) */
	*bp-- = '\0';			/* Terminate with null */
	
	n = zxpand(buf);		/* Expand the resulting wild name */
					/* n is the number of matches */
	while (n-- > 0) {		/* Find any existing name.~n~ files */
	    xp = *mtchptr++;		/* Point at matching name */
	    xp += len;			/* Look for .~<n>~ at the end of it */
	    if (*xp == '.' && *(xp+1) == '~') {	/* Has a version number */
		t = atoi(xp+2);		        /* Get it */
		if (t > d) d = t;	/* Save d = highest version number */
	    }
	}
	if (d < power-1) {		/* Less than maximum possible? */
	    sprintf(bp,".~%d~",d+1);	/* Yes, make "name.~<d+1>~" */
	    *s = buf;			/* Point to new name */
	    return;			/* Done, return it */
	}
    }
    sprintf(bp,".~xxxx~");		/* Too many, use xxxx. */
    *s = buf;
    return;
}

/*  Z R E N A M E  --  Rename a file  */

/*  Note, link() and unlink() are used because rename() is not available  */
/*  in some versions of UNIX.   */
/*  Call with old and new names */
/*  Returns 0 on success, -1 on failure. */

int
zrename(old,new) char *old, *new; {
    if (link(old,new) < 0) {		/* Make a link with the new name. */
	debug(F110," link fails",old,0);
	return(-1);
    }
    if (unlink(old) < 0) {		/* Unlink the old name. */
	debug(F110," link fails",old,0);
	return(-1);
    }
    return(0);
}

/*  Z S A T T R */
/*
 Fills in a Kermit file attribute structure for the file which is to be sent.
 Returns 0 on success with the structure filled in, or -1 on failure.
 If any string member is null, then it should be ignored.
 If any numeric member is -1, then it should be ignored.
*/
int
zsattr(xx) struct zattr *xx; {
    long k;

    k = iflen % 1024L;			/* File length in K */
    if (k != 0L) k = 1L;
    xx->lengthk = (iflen / 1024L) + k;
    xx->type.len = 0;			/* File type can't be filled in here */
    xx->type.val = "";
    if (*nambuf) {
	xx->date.val = zfcdat(nambuf);	/* File creation date */
	xx->date.len = strlen(xx->date.val);
    } else {
	xx->date.len = 0;
	xx->date.val = "";
    }
    xx->creator.len = 0;		/* File creator */
    xx->creator.val = "";
    xx->account.len = 0;		/* File account */
    xx->account.val = "";
    xx->area.len = 0;			/* File area */
    xx->area.val = "";
    xx->passwd.len = 0;			/* Area password */
    xx->passwd.val = "";
    xx->blksize = -1L;			/* File blocksize */
    xx->access.len = 0;			/* File access */
    xx->access.val = "";
    xx->encoding.len = 0;		/* Transfer syntax */
    xx->encoding.val = 0;
    xx->disp.len = 0;			/* Disposition upon arrival */
    xx->disp.val = "";
    xx->lprotect.len = 0;		/* Local protection */
    xx->lprotect.val = "";
    xx->gprotect.len = 0;		/* Generic protection */
    xx->gprotect.val = "";
    xx->systemid.len = 2;		/* System ID */
    xx->systemid.val = "U1";
    xx->recfm.len = 0;			/* Record format */
    xx->recfm.val = "";
    xx->sysparam.len = 0;		/* System-dependent parameters */
    xx->sysparam.val = "";
    xx->length = iflen;			/* Length */
    return(0);
}

#ifdef COMMENT
int
zfree(f) char *f; {			/* Check free space */
    /* How??? */
    return(0);
}
#endif /* COMMENT */

/* Z F C D A T -- Return a string containing the time stamp for a file */

char *
zfcdat(name) char *name; {

#ifdef TIMESTAMP
    struct stat buffer;
    struct tm *time_stamp, *localtime();
    static char datbuf[20];

    datbuf[0] = '\0';
    if(stat(name,&buffer) != 0) {
	debug(F110,"zfcdat stat failed",name,0);
	return("");
    }
    time_stamp = localtime(&(buffer.st_mtime));
    if (time_stamp->tm_year < 1900) time_stamp->tm_year += 1900;
    sprintf(datbuf,"%-4.4d%02.2d%02.2d %002.2d:%002.2d:%002.2d",
	    time_stamp->tm_year,
	    time_stamp->tm_mon + 1,
	    time_stamp->tm_mday,
	    time_stamp->tm_hour,
	    time_stamp->tm_min,
	    time_stamp->tm_sec);
    debug(F111,"zfcdat",datbuf,strlen(datbuf));
    return(datbuf);
#else
    return("");
#endif /* timestamp */
}

/* Z S T I M E  --  Set creation date for incoming file */
/*
 Call with:
 f  = pointer to name of existing file.
 yy = pointer to a Kermit file attribute structure in which yy->date.val
      is a date of the form yyyymmdd hh:mm:ss, e.g. 19900208 13:00:00.
 x  = is a function code: 0 means to set the file's creation date as given.
      1 means compare the given date with the file creation date.      
 Returns:
 -1 on any kind of error.
  0 if x is 0 and the file date was set successfully.
  0 if x is 1 and date from attribute structure <= file creation date.
  1 if x is 1 and date from attribute structure > file creation date.
*/
int
zstime(f,yy,x) char *f; struct zattr *yy; int x; {
    int r = -1;				/* return code */
/*
  It is ifdef'd TIMESTAMP because it might not work on V7. bk@kullmar.se.
*/
#ifdef TIMESTAMP
/*
  To do: adapt code from OS-9 Kermit's ck9fio.c zstime function, which
  is more flexible, allowing [yy]yymmdd[ hh:mm[:ss]].
*/
    extern int ftime();
    long tm;
    int i, n, isleapyear, days, stat(), utime();
                   /*       J  F  M  A   M   J   J   A   S   O   N   D   */
                   /*      31 28 31 30  31  30  31  31  30  31  30  31   */
    static
    int monthdays [13] = {  0,0,31,59,90,120,151,181,212,243,273,304,334 };
    char s[5], *getenv(); 
    extern struct tm *localtime();
    struct stat sb;
#ifdef V7
    struct utimbuf {
      time_t timep[2];		/* New access and modificaton time */
    } tp;
    char *tz;
    long timezone;		/* In case timezone not defined in .h file */
#else
   struct utimbuf {
      time_t atime;		/* New access time */
      time_t mtime;		/* New modification time */
    } tp;
#endif /* V7 */

#ifdef ANYBSD
    long timezone;
    static struct timeb tbp;
#endif

    debug(F110,"zstime",f,0);

    if ((yy->date.len == 0)
        || (yy->date.len != 17)
        || (yy->date.val[8] != ' ')
        || (yy->date.val[11] != ':')
        || (yy->date.val[14] != ':') ) {
        debug(F111,"Bad creation date ",yy->date.val,yy->date.len);
        return(-1);
    }
    debug(F111,"zstime date check 1",yy->date.val,yy->date.len);
    for(i = 0; i < 8; i++) {
	if (!isdigit(yy->date.val[i])) {
	    debug(F111,"Bad creation date ",yy->date.val,yy->date.len);
	    return(-1);
	}
    }
    debug(F111,"zstime date check 2",yy->date.val,yy->date.len);
    i++;

    for (; i < 16; i += 3) {
	if ((!isdigit(yy->date.val[i])) || (!isdigit(yy->date.val[i + 1]))) {
	    debug(F111,"Bad creation date ",yy->date.val,yy->date.len);
	    return(-1);
	}
    }
    debug(F111,"zstime date check 3",yy->date.val,yy->date.len);

#ifdef ANYBSD
    debug(F100,"ztime BSD calling ftime","",0);
    ftime(&tbp);
    debug(F100,"ztime BSD back from ftime","",0);
    timezone = tbp.timezone * 60L;
    debug(F101,"ztime BSD timezone","",timezone);
#endif

#ifdef ATTSV
    tzset();				/* Set `timezone'. */
#endif /* ATTSV */

#ifdef V7
    if ((tz = getenv("TZ")) == NULL)
      timezone = 0;		/* UTC/GMT */
    else
      timezone = atoi(&tz[3]);	/* Set 'timezone'. */
    timezone *= 60L;
#endif

    debug(F100,"zstime so far so good","",0);

    s[4] = '\0';
    for (i = 0; i < 4; i++)	/* Fix the year */
      s[i] = yy->date.val[i];

    debug(F110,"zstime year",s,0);

    n = atoi(s);

    debug(F111,"zstime year",s,n);

/* Previous year's leap days. This won't work after year 2100, */
/* I don't care about that! */
    isleapyear = (( n % 4 == 0 && n % 100 !=0) || n % 400 == 0);
    days = ( n - 1970) * 365;
    days += (n - 1968 - 1) / 4 - (n - 1900 - 1) / 100 + (n - 1600 - 1) / 400;

    s[2] = '\0';
				
    for (i = 4 ; i < 16; i += 2) {
	s[0] = yy->date.val[i];
	s[1] = yy->date.val[i + 1];
	n = atoi(s);
    debug(F110,"zstime entering switch",s,0);
	switch (i) {
	  case 4:			/* MM: month */
	    if ((n < 1 ) || ( n > 12)) {
		debug(F111,"Bad creation date ",yy->date.val,yy->date.len);
		return(-1);
	    }
	    days += monthdays [n];
	    if (isleapyear && n > 2)
	      ++days;
	    continue;

	  case 6:			/* DD: day */
	    if ((n < 1 ) || ( n > 31)) {
		debug(F111,"Bad creation date ",yy->date.val,yy->date.len);
		return(-1);
	    }
	    tm = (days + n - 1) * 24L * 60L * 60L;
	    i++;			/* Skip the space */
	    continue;

	  case 9:			/* hh: hour */
	    if ((n < 0 ) || ( n > 23)) {
		debug(F111,"Bad creation date ",yy->date.val,yy->date.len);
		return(-1);
	    }
	    tm += n * 60L * 60L;
	    i++;			/* Skip the colon */
	    continue;

	  case 12:			/* mm: minute */
	    if ((n < 0 ) || ( n > 59)) {
		debug(F111,"Bad creation date ",yy->date.val,yy->date.len);
		return(-1);
	    }
#ifdef ANYBSD
	    tm += timezone;		/* Correct for time zone */
#else
	    tm += timezone;		/* Correct for time zone */
#endif
	    tm += n * 60L;
	    i++;			/* Skip the colon */
	    continue;

	  case 15:			/* ss: second */
	    if ((n < 0 ) || ( n > 59)) {
		debug(F111,"Bad creation date ",yy->date.val,yy->date.len);
		return(-1);
	    }
	    tm += n;
	}

	if (localtime(&tm)->tm_isdst)
	  tm -= 60L * 60L;		/* Adjust for daylight savings time */
    }

    debug(F111,"Attribute creation date ok ",yy->date.val,yy->date.len);

    if (stat(f,&sb)) {			/* Get the time for the file */
	debug(F110,"Can't do the stat system call on file:",f,0);
	return(-1);
    }
#ifdef V7
    tp.timep[0] = tm;			/* Set modif. time to creation date */
    tp.timep[1] = sb.st_atime;		/* Don't change the access time */
#else
    tp.mtime = tm;			/* Set modif. time to creation date */
    tp.atime = sb.st_atime;		/* Don't change the access time */
#endif  

    switch (x) {			/* Execute desired function */
      case 0:				/* Set the creation date of the file */
	if (utime(f,&tp)) {		/* Fix modification time */
	    debug(F110,"Can't set modification time for file: ",f,0);
	    r = -1;
	} else  {
	    debug(F110,"Modification time is set for file: ",f,0);
	    r = 0;	
	}
	break;
      case 1:				/* Compare the dates */
	debug(F111,"zstime compare",f,sb.st_atime);
	debug(F111,"zstime compare","packet",tm);
	if (sb.st_atime < tm) r = 0; else r = 1;
	break;
      default:				/* Error */
	r = -1;
    }
#endif /* TIMESTAMP */
    return(r);
}

/* Find initialization file. */

#ifdef NOTUSED
int
zkermini() {
/*  nothing here for Unix.  This function added for benefit of VMS Kermit.  */
    return(0);
}
#endif /* NOTUSED */

#ifndef NOFRILLS
int
zmail(p,f) char *p; char *f; {		/* Send file f as mail to address p */

#ifdef BSD4
/* The idea is to use /usr/ucb/mail, rather than regular mail, so that   */
/* a subject line can be included with -s.  Since we can't depend on the */
/* user's path, we use the convention that /usr/ucb/Mail = /usr/ucb/mail */
/* and even if Mail has been moved to somewhere else, this should still  */
/* find it...  The search could be made more reliable by actually using  */
/* access() to see if /usr/ucb/Mail exists. */

/* Should also make some check on zmbuf overflow... */

    sprintf(zmbuf,"Mail -s %c%s%c %s < %s", '"', f, '"', p, f);
    zsyscmd(zmbuf);
#else
#ifdef SVORPOSIX
    sprintf(zmbuf,"mail %s < %s", p, f);
    zsyscmd(zmbuf);
#else
    *zmbuf = '\0';
#endif
#endif    
    return(0);
}
#endif /* NOFRILLS */

#ifndef NOFRILLS
int
zprint(p,f) char *p; char *f; {		/* Print file f with options p */

#ifdef UNIX
#ifdef ANYBSD				/* BSD uses lpr to spool */
#define SPOOLER "lpr"
#else					/* Sys V uses lp */
#ifdef TRS16				/* except for Tandy-16/6000... */
#define SPOOLER "lpr"
#else
#define SPOOLER "lp"
#endif
#endif
/*
  Note use of standard input redirection.  In some systems, lp[r] runs
  setuid to lp (or ...?), so if user has sent a file into a directory
  that lp does not have read access to, it can't be printed unless it is
  feed to lp[r] as standard input.
*/
    sprintf(zmbuf,"%s %s < %s", SPOOLER, p, f); /* Construct print command */
    zsyscmd(zmbuf);
#else /* Not UNIX */
    *zmbuf = '\0';
#endif /* UNIX */
    return(0);
}
#endif /* NOFRILLS */

/*
  Wildcard expansion functions.  C-Kermit used to do this itself (see #else
  part...).  New code (version 5A, 1990-91) just asks UNIX to do it.
  This lets users use the wildcard expansion features of their favorite shell.
  Operation is slower because of the forking & piping, but flexibility is
  greater and program is smaller.
*/

static char scratch[MAXPATH+4];		/* Used by both methods */

static int oldmtchs = 0;		/* Let shell (ls) expand them. */
#ifdef COMMENT
static char *lscmd = "/bin/ls -d"; 	/* Command to use. */
#else
static char *lscmd = "echo";		/* Command to use. */
#endif /* COMMENT */

int
shxpand(pat,namlst,len) char *pat, *namlst[]; int len; {
    char *fgbuf = NULL;			/* Buffer for forming ls command */
    char *p, *q;			/* Workers */
    int i, x, retcode; char c;		/* ... */

    x = strlen(pat) + strlen(lscmd) + 3; /* Length of ls command */
    for (i = 0; i < oldmtchs; i++)	/* Free previous file list */
      free(namlst[i]);
    fgbuf = malloc(x);			/* Get buffer for command */
    if (!fgbuf) return(-1);		/* Fail if cannot */
    sprintf(fgbuf,"%s %s",lscmd,pat);	/* Form the command */
    zxcmd(ZIFILE,fgbuf);		/* Start the command */
    i = 0;				/* File counter */
    p = scratch;			/* Point to scratch area */
    retcode = -1;			/* Assume failure */
    while ((c = zminchar()) != -1) {	/* Read characters from command */
	if (c == ' ' || c == '\n') {	/* Got newline or space? */
	    *p = '\0';			/* Yes, terminate string */
	    p = scratch;		/* Point back to beginning */
	    if (zchki(p) == -1)		/* Does file exist? */
	      continue;			/* No, continue */
	    x = strlen(p);		/* Yes, get length of name */
	    q = malloc(x+1);		/* Allocate space for it */
	    if (!q) goto shxfin;	/* Fail if space can't be obtained */
	    strcpy(q,scratch);		/* Copy name to space */
	    namlst[i++] = q;		/* Copy pointer to name into array */
	    if (i > len) goto shxfin;	/* Fail if too many */
	} else {			/* Regular character */
	    *p++ = c;			/* Copy it into scratch area */
	}
    }
    retcode = i;			/* Return number of matching files */
shxfin:					/* Common exit point */
    free(fgbuf);			/* Free command buffer */
    zclosf(ZIFILE);			/* Delete the command fork. */
    oldmtchs = i;			/* Remember how many files */
    return(retcode);
}

/* Directory Functions for Unix, written by Jeff Damens, CUCCA, 1984. */

/* Define the size of the string space for filename expansion. */

#ifndef DYNAMIC
#ifdef PROVX1
#define SSPACE 500
#else
#ifdef BSD29
#define SSPACE 500
#else
#ifdef pdp11
#define SSPACE 500
#else
#ifdef aegis
#define SSPACE 10000			/* size of string-generating buffer */
static char bslash;			/* backslash character if active */
#else					/* Default static buffer size */
#define SSPACE 2000			/* size of string-generating buffer */
#endif /* aegis */
#endif /* pdp11 */
#endif /* BSD29 */
#endif /* PROVX1 */
static char sspace[SSPACE];             /* buffer for generating filenames */
#else /* DYNAMIC */
#define SSPACE 10000
static char *sspace = (char *)0;
#endif /* DYNAMIC */
static int ssplen = SSPACE;		/* length of string space buffer */

static char *freeptr,**resptr;         	/* copies of caller's arguments */
static int remlen;                      /* remaining length in caller's array*/
static int numfnd;                      /* number of matches found */

/*
 * splitpath:
 *  takes a string and splits the slash-separated portions into
 *  a list of path structures.  Returns the head of the list.  The
 *  structures are allocated by malloc, so they must be freed.
 *  Splitpath is used internally by the filename generator.
 *
 * Input: A string.
 * Returns: A linked list of the slash-separated segments of the input.
 */

struct path *
splitpath(p) char *p; {
    struct path *head,*cur,*prv;
    int i;

    debug(F110,"splitpath",p,0);

    head = prv = NULL;
    if (*p == '/') p++;			/* skip leading slash */
    while (*p != '\0') {
	cur = (struct path *) malloc(sizeof (struct path));
	debug(F101,"splitpath malloc","",(int) cur);
	if (cur == NULL)
	  fatal("malloc fails in splitpath()");
	cur -> fwd = NULL;
	if (head == NULL)
	  head = cur;
	else
	  prv -> fwd = cur;		/* link into chain */
	prv = cur;
#ifdef aegis
	/* treat backslash as "../" */
	if (bslash && *p == bslash) {
	    strcpy(cur->npart, "..");
	    ++p;
	} else {
	    for (i=0; i < MAXNAMLEN && *p && *p != '/' && *p != bslash; i++)
	      cur -> npart[i] = *p++;
	    cur -> npart[i] = '\0';	/* end this segment */
	    if (i >= MAXNAMLEN)
	      while (*p && *p != '/' && *p != bslash)
		p++;
	}
	if (*p == '/') p++;
#else
	for (i=0; i < MAXNAMLEN && *p != '/' && *p != '\0'; i++) {
	    cur -> npart[i] = *p++;
	}
	cur -> npart[i] = '\0';		/* end this segment */
	if (i >= MAXNAMLEN)
	  while (*p != '/' && *p != '\0') p++;
	if (*p == '/')
	  p++;

#endif
    }
    return(head);
}

/*
 * fgen:
 *  This is the actual name generator.  It is passed a string,
 *  possibly containing wildcards, and an array of character pointers.
 *  It finds all the matching filenames and stores them into the array.
 *  The returned strings are allocated from a static buffer local to
 *  this module (so the caller doesn't have to worry about deallocating
 *  them); this means that successive calls to fgen will wipe out
 *  the results of previous calls.  This isn't a problem here
 *  because we process one wildcard string at a time.
 *
 * Input: a wildcard string, an array to write names to, the
 *        length of the array.
 * Returns: the number of matches.  The array is filled with filenames
 *          that matched the pattern.  If there wasn't enough room in the
 *	    array, -1 is returned.
 * By: Jeff Damens, CUCCA, 1984.
 */
static int
fgen(pat,resarry,len) char *pat,*resarry[]; int len; {
    struct path *head;
    char *sptr;
#ifdef aegis
    char *namechars;
    int tilde = 0, bquote = 0;

    if ((namechars = getenv("NAMECHARS")) != NULL) {
	if (xindex(namechars, '~' ) != NULL) tilde  = '~';
	if (xindex(namechars, '\\') != NULL) bslash = '\\';
	if (xindex(namechars, '`' ) != NULL) bquote = '`';
    } else {
	tilde = '~'; bslash = '\\'; bquote = '`';
    }

    sptr = scratch;

    /* copy "`node_data", etc. anchors */
    if (bquote && *pat == bquote)
      while (*pat && *pat != '/' && *pat != bslash)
	*sptr++ = *pat++;
    else if (tilde && *pat == tilde)
      *sptr++ = *pat++;
    while (*pat == '/')
      *sptr++ = *pat++;
    if (sptr == scratch) {
	strcpy(scratch,"./");
	sptr = scratch+2;
    }					/* init buffer correctly */
    head = splitpath(pat);
#else
    debug(F110,"fgen pat",pat,0);
    head = splitpath(pat);
    if (*pat == '/') {
	scratch[0] = '/';
	sptr = scratch+1;
    } else {
	strcpy(scratch,"./");
	sptr = scratch+2;
    }					/* init buffer correctly */
#endif
    numfnd = 0;				/* none found yet */
#ifdef DYNAMIC
    if (!sspace) {			/* Need to allocate string space? */
	while (ssplen > 50) {
	    if ((sspace = malloc(ssplen+2))) { /* Got it. */
		debug(F101,"fgen string space","",ssplen);
		break;
	    }
	    ssplen = (ssplen / 2) + (ssplen / 4); /* Didn't, reduce by 3/4 */
	}
	if (ssplen < 50) {		/* Did we get it? */
	    fprintf(stderr,"fgen can't malloc string space\n");
	    return(-1);
	}
    }
#endif /* DYNAMIC */
    freeptr = sspace;			/* this is where matches are copied */
    resptr = resarry;			/* static copies of these so */
    remlen = len;			/* recursive calls can alter them */
    traverse(head,scratch,sptr);	/* go walk the directory tree */
#ifdef COMMENT
/*
  This code, circa 1984, has never worked right - it references the head
  pointer after it has already been freed.  Lord knows what might have been
  happening because of this.  Thanks to Steve Walton for finding & fixing this
  bug.
*/
    for (; head != NULL; head = head -> fwd)
      free(head);			/* return the path segments */
#else
    while (head != NULL) {
	struct path *next = head -> fwd;
	free(head);
	head = next;
    }
#endif /* COMMENT */
    return(numfnd);			/* and return the number of matches */
}

/* traverse:
 *  Walks the directory tree looking for matches to its arguments.
 *  The algorithm is, briefly:
 *   If the current pattern segment contains no wildcards, that
 *   segment is added to what we already have.  If the name so far
 *   exists, we call ourselves recursively with the next segment
 *   in the pattern string; otherwise, we just return.
 *
 *   If the current pattern segment contains wildcards, we open the name
 *   we've accumulated so far (assuming it is really a directory), then read
 *   each filename in it, and, if it matches the wildcard pattern segment, add
 *   that filename to what we have so far and call ourselves recursively on the
 *   next segment.
 *
 *   Finally, when no more pattern segments remain, we add what's accumulated
 *   so far to the result array and increment the number of matches.
 *
 * Input: a pattern path list (as generated by splitpath), a string
 *	  pointer that points to what we've traversed so far (this
 *	  can be initialized to "/" to start the search at the root
 *	  directory, or to "./" to start the search at the current
 *	  directory), and a string pointer to the end of the string
 *	  in the previous argument.
 * Returns: nothing.
 */
static VOID
traverse(pl,sofar,endcur) struct path *pl; char *sofar, *endcur; {

/* Define LONGFN (long file names) automatically for BSD 2.9 and 4.2 */
/* LONGFN can also be defined on the cc command line. */

#ifdef BSD29
#ifndef LONGFN
#define LONGFN
#endif
#endif

#ifdef BSD42
#ifndef LONGFN
#define LONGFN
#endif
#endif

/* Appropriate declarations for directory routines and structures */
/* #define OPENDIR means to use opendir(), readdir(), closedir()  */
/* If OPENDIR not defined, we use open(), read(), close() */

#ifdef DIRENT				/* New way, <dirent.h> */
#define OPENDIR
    DIR *fd, *opendir();
    struct dirent *dirbuf;
    struct dirent *readdir();
#else /* !DIRENT */
#ifdef LONGFN				/* Old way, <dir.h> with opendir() */
#define OPENDIR
    DIR *fd, *opendir();
    struct direct *dirbuf;
#else /* !LONGFN */
    int fd;				/* Old way, <dir.h> with open() */
    struct direct dir_entry;
    struct direct *dirbuf = &dir_entry;
#endif /* LONGFN */
#endif /* DIRENT */

    struct stat statbuf;		/* for file info */

    if (pl == NULL) {
	*--endcur = '\0';		/* end string, overwrite trailing / */
	addresult(sofar);
	return;
    }
    if (!iswild(pl -> npart)) {
	strcpy(endcur,pl -> npart);
	endcur += strlen(pl -> npart);
	*endcur = '\0';			/* end current string */
	if (stat(sofar,&statbuf) == 0) { /* if current piece exists */
	    *endcur++ = '/';		/* add slash to end */
	    *endcur = '\0';		/* and end the string */
	    traverse(pl -> fwd,sofar,endcur);
	}
	return;
    }

    /* Segment contains wildcards, have to search directory */

    *endcur = '\0';                        	/* end current string */
    if (stat(sofar,&statbuf) == -1) return;   	/* doesn't exist, forget it */
    if (!S_ISDIR (statbuf.st_mode)) return; /* not a directory, skip */

#ifdef OPENDIR
      if ((fd = opendir(sofar)) == NULL) return; /* Can't open, fail. */
    while (dirbuf = readdir(fd))
#else /* !OPENDIR */
      if ((fd = open(sofar,O_RDONLY)) < 0) return; /* Can't open, fail. */
    while (read(fd, (char *)dirbuf, sizeof dir_entry))
#endif /* OPENDIR */
      {
	  /* Get null-terminated copy!!! */
	  strncpy(nambuf,dirbuf->d_name,MAXNAMLEN); 
	  nambuf[MAXNAMLEN] = '\0';
#ifdef unos  
	  if (dirbuf->d_ino != -1 && match(pl -> npart,nambuf))
#else
/* #ifdef _POSIX_SOURCE */
/*
  This is not specified in POSIX.1.  In POSIX.2, there will be
  some kind of filename matching facility, but it will not be done this way.
  Therefore, we should not even be looking in the directory at all.
  Maybe POSIX implementations should force "set wildcard shell"?
*/

#ifdef sun
	  if (dirbuf->d_fileno != 0 && match(pl -> npart,nambuf))
#else 
#ifdef ultrix
	  if (dirbuf->gd_ino != 0 && match(pl -> npart,nambuf))
#else
	  if (dirbuf->d_ino != 0 && match(pl -> npart,nambuf))
#endif /* ultrix */
#endif /* sun */

/* #else */ /* not _POSIX_SOURCE */
/*	  if (dirbuf->d_ino != 0 && match(pl -> npart,nambuf)) */
/* #endif */ /* _POSIX_SOURCE */

#endif /* unos */
	    {
	      char *eos;
	      strcpy(endcur,nambuf);
	      eos = endcur + strlen(nambuf);
	      *eos = '/';		/* end this segment */
	      traverse(pl -> fwd,sofar,eos+1);
	  }
      }
#ifdef OPENDIR
    closedir(fd);
#else /* !OPENDIR */
    close(fd);
#endif /* OPENDIR */
}

/*
 * addresult:
 *  Adds a result string to the result array.  Increments the number
 *  of matches found, copies the found string into our string
 *  buffer, and puts a pointer to the buffer into the caller's result
 *  array.  Our free buffer pointer is updated.  If there is no
 *  more room in the caller's array, the number of matches is set to -1.
 * Input: a result string.
 * Returns: nothing.
 */
static VOID
addresult(str) char *str; {
    int l;
    debug(F111,"addresult",str,remlen);
    if (strncmp(str,"./",2) == 0) str += 2;
    if (--remlen < 0) {
	numfnd = -1;
	return;
    }
    l = strlen(str) + 1;		/* size this will take up */
    if ((freeptr + l) > (sspace + ssplen)) {
	numfnd = -1;			/* do not record if not enough space */
	return;
    }
    strcpy(freeptr,str);
    *resptr++ = freeptr;
    freeptr += l;
    numfnd++;
}

/*
 * match:
 *  pattern matcher.  Takes a string and a pattern possibly containing
 *  the wildcard characters '*' and '?'.  Returns true if the pattern
 *  matches the string, false otherwise.
 * by: Jeff Damens, CUCCA, 1984
 * skipping over dot files and backslash quoting added by fdc, 1990.
 *
 * Input: a string and a wildcard pattern.
 * Returns: 1 if match, 0 if no match.
 */
static int
match(pattern,string) char *pattern,*string; {
    char *psave,*ssave;			/* back up pointers for failure */
    int q = 0;				/* quote flag */

    debug(F110,"match str",string,0);
    psave = ssave = NULL;
#ifndef MATCHDOT
    if (*string == '.' && *pattern != '.') {
	debug(F110,"match skip",string,0);
	return(0);
    }
#endif
    while (1) {
	for (; *pattern == *string; pattern++,string++)  /* skip first */
	    if (*string == '\0') return(1);	/* end of strings, succeed */

	if (*pattern == '\\' && q == 0) { /* Watch out for quoted */
	    q = 1;			/* metacharacters */
	    pattern++;			/* advance past quote */
	    if (*pattern != *string) return(0);
	    continue;
	} else q = 0;

	if (q) {
	    return(0);
	} else {
	    if (*string != '\0' && *pattern == '?') {
		pattern++;		/* '?', let it match */
		string++;
	    } else if (*pattern == '*') { /* '*' ... */
		psave = ++pattern;	/* remember where we saw it */
		ssave = string;		/* let it match 0 chars */
	    } else if (ssave != NULL && *ssave != '\0') { /* if not at end  */
					/* ...have seen a star */
		string = ++ssave;	/* skip 1 char from string */
		pattern = psave;	/* and back up pattern */
	    } else return(0);		/* otherwise just fail */
	}
    }
}

/*
  The following two functions are for expanding tilde in filenames
  Contributed by Howie Kaye, CUCCA, developed for CCMD package.
*/
 
/*  W H O A M I  --  Get user's username.  */

/*
  1) Get real uid
  2) See if the $USER environment variable is set ($LOGNAME on AT&T)
  3) If $USER's uid is the same as ruid, realname is $USER
  4) Otherwise get logged in user's name
  5) If that name has the same uid as the real uid realname is loginname
  6) Otherwise, get a name for ruid from /etc/passwd
*/
static char *
whoami () {
#ifdef DTILDE
    static char realname[256];		/* user's name */
    static int ruid = -1;		/* user's real uid */
    char loginname[256], envname[256];	/* temp storage */
    char *c;
    struct passwd *p;
#ifndef _POSIX_SOURCE
    char *getlogin(), *getenv();
#endif /* _POSIX_SOURCE */

    if (ruid != -1)
      return(realname);

    ruid = real_uid();			/* get our uid */

  /* how about $USER or $LOGNAME? */
    if ((c = getenv(NAMEENV)) != NULL) { /* check the env variable */
	strcpy (envname, c);		
	p = getpwnam(envname);
	if (p->pw_uid == ruid) {	/* get passwd entry for envname */
	    strcpy (realname, envname);	/* if the uid's are the same */
	    return(realname);
	}
    }

  /* can we use loginname() ? */

    if ((c =  getlogin()) != NULL) {	/* name from utmp file */
	strcpy (loginname, c);	
	if ((p = getpwnam(loginname)) != NULL) /* get passwd entry */
	  if (p->pw_uid == ruid) {	/* for loginname */ 
	      strcpy (realname, loginname); /* if the uid's are the same */
	      return(realname);
	  }
    }

  /* Use first name we get for ruid */

    if ((p = getpwuid(ruid)) == NULL) { /* name for uid */
	realname[0] = '\0';		/* no user name */
	ruid = -1;
	return(NULL);
    }
    strcpy (realname, p->pw_name);	
    return(realname);
#else
    return(NULL);
#endif /* DTILDE */
}

/*  T I L D E _ E X P A N D  --  expand ~user to the user's home directory. */

#define DIRSEP '/'

char *
tilde_expand(dirname) char *dirname; {
#define BUFLEN 256
#ifdef DTILDE
    struct passwd *user;
    static char olddir[BUFLEN];
    static char oldrealdir[BUFLEN];
    static char temp[BUFLEN];
    int i, j;

    debug(F111,"tilde_expand",dirname,dirname[0]);
    
    if (dirname[0] != '~')		/* Not a tilde...return param */
      return(dirname);
    if (!strcmp(olddir,dirname)) {	/* Same as last time */
      return(oldrealdir);		/* so return old answer. */
    } else {
	j = strlen(dirname);
	for (i = 0; i < j; i++)		/* find username part of string */
	  if (dirname[i] != DIRSEP)
	    temp[i] = dirname[i];
	  else break;
	temp[i] = '\0';			/* tie off with a NULL */
	if (i == 1) {			/* if just a "~" */
	    user = getpwnam(whoami());	/*  get info on current user */
	} else {
	    user = getpwnam(&temp[1]);	/* otherwise on the specified user */
	}
    }
    if (user != NULL) {			/* valid user? */
	strcpy(olddir, dirname);	/* remember the directory */
	strcpy(oldrealdir,user->pw_dir); /* and their home directory */
	strcat(oldrealdir,&dirname[i]);
	return(oldrealdir);
    } else {				/* invalid? */
	strcpy(olddir, dirname);	/* remember for next time */
	strcpy(oldrealdir, dirname);
	return(oldrealdir);
    }
#else
    return(NULL);
#endif /* DTILDE */
}

/*
  Functions for executing system commands.
  zsyscmd() executes the system command in the normal, default way for
  the system.  In UNIX, it does what system() does.  Thus, its results
  are always predictable.
  zshcmd() executes the command using the user's preferred shell.
*/
int
zsyscmd(s) char *s; {
    PID_T shpid;
#ifdef COMMENT
/* This doesn't work... */
    WAIT_T status;
#else
    int status;
#endif /* COMMENT */

    if (shpid = fork()) {
	if (shpid < (PID_T)0) return(-1); /* Parent */
	while (shpid != (PID_T) wait(&status))
	  ;
	return(status);
    }
    if (priv_can()) {			/* Child: cancel any priv's */
	printf("?Privilege cancellation failure\n");
	_exit(255);
    }
    execl("/bin/sh","sh","-c",s,NULL);
    perror("/bin/sh");
    _exit(255);
    return(0);				/* Shut up ANSI compilers. */
}

/*
  UNIX code by H. Fischer; copyright rights assigned to Columbia Univ.
  Adapted to use getpwuid to find login shell because many systems do not
  have SHELL in environment, and to use direct calling of shell rather
  than intermediate system() call. -- H. Fischer
  Call with s pointing to command to execute.
*/

int
zshcmd(s) char *s; {
    PID_T pid;

#ifdef OS2
    if (*s == '\0') sprintf(s,"%s","CMD"); /* Command processor */
    concooked();
    if (!priv_chk()) system(s);
    conraw();
#else
#ifdef AMIGA
    if (!priv_chk()) system(s);
#else
#ifdef datageneral
    if (priv_chk) return(1);
    if (*s == '\0')			/* Interactive shell requested? */
#ifdef mvux
	system("/bin/sh ");
#else
        system("x :cli prefix Kermit_Baby:");
#endif
    else				/* Otherwise, */
        system(s);			/* Best for aos/vs?? */
 
#else
#ifdef aegis
    if ((pid = vfork()) == 0) {		/* Make child quickly */
	char *shpath, *shname, *shptr;	/* For finding desired shell */

	if (priv_can()) exit(1);	/* Turn off privs. */
        if ((shpath = getenv("SHELL")) == NULL) shpath = "/com/sh";

#else					/* All Unix systems */
    if ((pid = fork()) == 0) {		/* Make child */
	char *shpath, *shname, *shptr;	/* For finding desired shell */
	struct passwd *p;
	char *defshell = "/bin/sh";	/* Default */
 
	if (priv_can()) exit(1);	/* Turn off privs. */
	p = getpwuid(real_uid());	/* Get login data */
	if (p == (struct passwd *) NULL || !*(p->pw_shell))
	  shpath = defshell;
	else
	  shpath = p->pw_shell;
#endif
	shptr = shname = shpath;
	while (*shptr != '\0')
	  if (*shptr++ == '/')
	    shname = shptr;
	if (s == NULL || *s == '\0') {	/* Interactive shell requested? */
	    execl(shpath,shname,"-i",NULL); /* Yes, do that */
	} else {			/* Otherwise, */
	    execl(shpath,shname,"-c",s,NULL); /* exec the given command */
	}				/* If execl() failed, */
	exit(BAD_EXIT);			/* return bad return code. */

    } else {				/* Parent */

    	int wstat;			/* ... must wait for child */
	SIGTYP (*istat)(), (*qstat)();
 
	if (pid == -1) return(0);	/* fork() failed? */

	istat = signal(SIGINT,SIG_IGN);	/* Let the fork handle keyboard */
	qstat = signal(SIGQUIT,SIG_IGN); /* interrupts itself... */
 
    	while (((wstat = wait((WAIT_T *)0)) != pid) && (wstat != -1))
	  ;				/* Wait for fork */
	signal(SIGINT,istat);		/* Restore interrupts */
	signal(SIGQUIT,qstat);
    }
#endif
#endif
#endif
    return(1);
}

#ifdef aegis
/*
 Replacement for strchr() and index(), neither of which seem to be universal.
*/

static char *
#ifdef CK_ANSIC
xindex(char * s, char c)
#else
xindex(s,c) char *s, c;
#endif /* CK_ANSIC */
/* xindex */ {
    while (*s != '\0' && *s != c) s++;
    if (*s == c) return(s); else return(NULL);
}
#endif /* aegis */

/*  I S W I L D  --  Check if filespec is "wild"  */

/*
  Returns 0 if it is a single file, 1 if it contains wildcard characters.
  Note: must match the algorithm used by match(), hence no [a-z], etc.
*/
int
iswild(filespec) char *filespec; {
    char c; int x; char *p;
    if (wildxpand) {
	if ((x = zxpand(filespec)) > 1) return(1);
	p = malloc(MAXNAMLEN + 20);
	znext(p);
	x = (strcmp(filespec,p) != 0);
	free(p);
	return(x);
    } else {
	while ((c = *filespec++) != '\0')
	  if (c == '*' || c == '?') return(1);
	return(0);
    }
}

