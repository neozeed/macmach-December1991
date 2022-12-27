/*
 *	Utah RLE Toolkit library routines.
 * 
 * 	Read image support only.
 * 
 * 	Cobbled from Utah RLE include and library source files.
 * 
 * 	By Graeme Gill
 * 	30/5/90
 *
 */

#include <stdio.h>
#include <math.h>
#include <varargs.h>
#include <ctype.h>

#include "image.h"		/* need ZFILE definition */
#include "rle.h"

#define zeof(zfp) feof((zfp)->stream)
#define zclearerr(zfp) clearerr((zfp)->stream)

/*
 * This software is copyrighted as noted below.  It may be freely copied,
 * modified, and redistributed, provided that the copyright notice is 
 * preserved on all copies.
 * 
 * There is no warranty or other guarantee of fitness for this software,
 * it is provided solely "as is".  Bug reports or fixes may be sent
 * to the author, who may or may not act on them as he desires.
 *
 * You may not include this software in a program or other software product
 * without supplying the source, or without informing the end-user that the 
 * source is available for no extra charge.
 *
 * If you modify this software, you should include a notice giving the
 * name of the person performing the modification, the date of modification,
 * and the reason for such modification.
 */
/* 
 * Runsv.h - Definitions for Run Length Encoding.
 * 
 * Author:	Spencer W. Thomas
 * 		Computer Science Dept.
 * 		University of Utah
 * Date:	Mon Aug  9 1982
 * Copyright (c) 1982 Spencer W. Thomas
 */

#ifndef XTNDRUNSV
#define XTNDRUNSV

/* 
 * Opcode definitions
 */

#define     LONG                0x40
#define	    RSkipLinesOp	1
#define	    RSetColorOp		2
#define	    RSkipPixelsOp	3
#define	    RByteDataOp		5
#define	    RRunDataOp		6
#define	    REOFOp		7

#define     H_CLEARFIRST        0x1	/* clear framebuffer flag */
#define	    H_NO_BACKGROUND	0x2	/* if set, no bg color supplied */
#define	    H_ALPHA		0x4   /* if set, alpha channel (-1) present */
#define	    H_COMMENT		0x8	/* if set, comments present */

struct XtndRsetup
{
    short   h_xpos,
            h_ypos,
            h_xlen,
            h_ylen;
    char    h_flags,
            h_ncolors,
	    h_pixelbits,
	    h_ncmap,
	    h_cmaplen;
};
#define	    SETUPSIZE	((4*2)+5)

/* "Old" RLE format magic numbers */
#define	    RMAGIC	('R' << 8)	/* top half of magic number */
#define	    WMAGIC	('W' << 8)	/* black&white rle image */

#define	    XtndRMAGIC	((short)0xcc52)	/* RLE file magic number */

#endif /* XTNDRUNSV */

/*  "svfb.h" */
/*
 * This software is copyrighted as noted below.  It may be freely copied,
 * modified, and redistributed, provided that the copyright notice is 
 * preserved on all copies.
 * 
 * There is no warranty or other guarantee of fitness for this software,
 * it is provided solely "as is".  Bug reports or fixes may be sent
 * to the author, who may or may not act on them as he desires.
 *
 * You may not include this software in a program or other software product
 * without supplying the source, or without informing the end-user that the 
 * source is available for no extra charge.
 *
 * If you modify this software, you should include a notice giving the
 * name of the person performing the modification, the date of modification,
 * and the reason for such modification.
 */
/* 
 * svfb.h - Definitions and a few global variables for svfb.
 * 
 * Author:	Spencer W. Thomas
 * 		Computer Science Dept.
 * 		University of Utah
 * Date:	Mon Aug  9 1982
 * Copyright (c) 1982 Spencer W. Thomas
 */

/* ****************************************************************
 * Dispatch table for different output types.
 */
typedef void sv_fn();
struct sv_dispatch_tab {
    char   *magic;			/* magic type flags */
    sv_fn  *setup,			/* startup function */
	   *skipBlankLines,
	   *setColor,
	   *skipPixels,
	   *newScanLine,
	   *putdat,			/* put a set of differing pixels */
	   *putrn,			/* put a run all the same */
	   *blockHook,			/* hook called at start of new */
					/* output block */
	   *putEof;		/* write EOF marker (if possible) */
};

struct sv_dispatch_tab sv_DTable[];

/* 
 * These definitions presume the existence of a variable called
 * "fileptr", declared "long * fileptr".  *fileptr should be
 * initialized to 0 before calling Setup().
 * A pointer "globals" declared "struct sv_globals * globals" is also
 * presumed to exist.
 */
#define	    sv_magic		(sv_DTable[(int)globals->sv_dispatch].magic)
#define	    Setup()		(*sv_DTable[(int)globals->sv_dispatch].setup)(globals)
#define	    SkipBlankLines(n)	(*sv_DTable[(int)globals->sv_dispatch].skipBlankLines)(n, globals)
#define	    SetColor(c)		(*sv_DTable[(int)globals->sv_dispatch].setColor)(c, globals)
#define	    SkipPixels(n, l, r)	(*sv_DTable[(int)globals->sv_dispatch].skipPixels)(n,l,r, globals)
#define	    NewScanLine(flag)	(*sv_DTable[(int)globals->sv_dispatch].newScanLine)(flag, globals)
#define	    putdata(buf, len)	(*sv_DTable[(int)globals->sv_dispatch].putdat)(buf, len, globals)
#define	    putrun(val, len, f)	(*sv_DTable[(int)globals->sv_dispatch].putrn)(val,len,f, globals)
#define	    BlockHook()		(*sv_DTable[(int)globals->sv_dispatch].blockHook)(globals)
#define	    PutEof()		(*sv_DTable[(int)globals->sv_dispatch].putEof)(globals)

/* 
 * States for run detection
 */
#define	DATA	0
#define	RUN2	1
#define RUN3	2
#define	RUN4	3
#define	INRUN	-1

/*
 * This software is copyrighted as noted below.  It may be freely copied,
 * modified, and redistributed, provided that the copyright notice is 
 * preserved on all copies.
 * 
 * There is no warranty or other guarantee of fitness for this software,
 * it is provided solely "as is".  Bug reports or fixes may be sent
 * to the author, who may or may not act on them as he desires.
 *
 * You may not include this software in a program or other software product
 * without supplying the source, or without informing the end-user that the 
 * source is available for no extra charge.
 *
 * If you modify this software, you should include a notice giving the
 * name of the person performing the modification, the date of modification,
 * and the reason for such modification.
 *
 *  Modified at BRL 16-May-88 by Mike Muuss to avoid Alliant STDC desire
 *  to have all "void" functions so declared.
 */
/* 
 * svfb_global.c - Global variable initialization for svfb routines.
 * 
 * Author:	Spencer W. Thomas
 * 		Computer Science Dept.
 * 		University of Utah
 * Date:	Thu Apr 25 1985
 * Copyright (c) 1985,1986 Spencer W. Thomas
 */


void	RunSetup(),
		RunSkipBlankLines(),
		RunSetColor(),
		RunSkipPixels(),
		RunNewScanLine(),
		Runputdata(),
		Runputrun(),
		RunputEof();

void	DefaultBlockHook();
void	NullputEof();

struct sv_dispatch_tab sv_DTable[] = {
    {
	" OB",
	RunSetup,
	RunSkipBlankLines,
	RunSetColor,
	RunSkipPixels,
	RunNewScanLine,
	Runputdata,
	Runputrun,
	DefaultBlockHook,
	RunputEof
    },
};

static int sv_bg_color[3] = { 0, 0, 0 };

struct sv_globals sv_globals = {
    RUN_DISPATCH,		/* dispatch value */
    3,				/* 3 colors */
    sv_bg_color,		/* background color */
    0,				/* (alpha) if 1, save alpha channel */
    2,				/* (background) 0->just save pixels, */
				/* 1->overlay, 2->clear to bg first */
    0, 511,			/* (xmin, xmax) X bounds to save */
    0, 479,			/* (ymin, ymax) Y bounds to save */
    0,				/* ncmap (if != 0, save color map) */
    8,				/* cmaplen (log2 of length of color map) */
    NULL,			/* pointer to color map */
    NULL,			/* pointer to comment strings */
    NULL,			/* output file */
    { 7 }			/* RGB channels only */
    /* Can't initialize the union */
};

/* ARGSUSED */
void
NullputEof(globals)
struct sv_globals * globals;
{
				/* do nothing */
}

/*
 * This software is copyrighted as noted below.  It may be freely copied,
 * modified, and redistributed, provided that the copyright notice is 
 * preserved on all copies.
 * 
 * There is no warranty or other guarantee of fitness for this software,
 * it is provided solely "as is".  Bug reports or fixes may be sent
 * to the author, who may or may not act on them as he desires.
 *
 * You may not include this software in a program or other software product
 * without supplying the source, or without informing the end-user that the 
 * source is available for no extra charge.
 *
 * If you modify this software, you should include a notice giving the
 * name of the person performing the modification, the date of modification,
 * and the reason for such modification.
 *
 *  Modified at BRL 16-May-88 by Mike Muuss to avoid Alliant STDC desire
 *  to have all "void" functions so declared.
 */
/* 
 * Runsv.c - General purpose Run Length Encoding for svfb.
 * 
 * Author:	Spencer W. Thomas
 * 		Computer Science Dept.
 * 		University of Utah
 * Date:	Mon Aug  9 1982
 * Copyright (c) 1982,1986 Spencer W. Thomas
 */

/* THIS IS WAY OUT OF DATE.  See rle.5.
 * The output file format is:
 * 
 * Word 0:	A "magic" number.  The top byte of the word contains
 *		the letter 'R' or the letter 'W'.  'W' indicates that
 *		only black and white information was saved.  The bottom
 *		byte is one of the following:
 *	' ':	Means a straight "box" save, -S flag was given.
 *	'B':	Image saved with background color, clear screen to
 *		background before restoring image.
 *	'O':	Image saved in overlay mode.
 * 
 * Words 1-6:	The structure
 * { *     short   xpos,			/* Lower left corner
 *             ypos,
 *             xsize,			/* Size of saved box
 *             ysize;
 *     char    rgb[3];			/* Background color
 *     char    map;			/* flag for map presence
 * }
 * 
 * If the map flag is non-zero, then the color map will follow as 
 * 3*256 16 bit words, first the red map, then the green map, and
 * finally the blue map.
 * 
 * Following the setup information is the Run Length Encoded image.
 * Each instruction consists of a 4-bit opcode, a 12-bit datum and
 * possibly one or more following words (all words are 16 bits).  The
 * instruction opcodes are:
 * 
 * SkipLines (1):   The bottom 10 bits are an unsigned number to be added to
 *		    current Y position.
 * 
 * SetColor (2):    The datum indicates which color is to be loaded with
 * 		    the data described by the following ByteData and
 * 		    RunData instructions.  0->red, 1->green, 2->blue.  The
 * 		    operation also resets the X position to the initial
 * 		    X (i.e. a carriage return operation is performed).
 * 
 * SkipPixels (3):  The bottom 10 bits are an unsigned number to be
 * 		    added to the current X position.
 * 
 * ByteData (5):    The datum is one less than the number of bytes of
 * 		    color data following.  If the number of bytes is
 * 		    odd, a filler byte will be appended to the end of
 * 		    the byte string to make an integral number of 16-bit
 * 		    words.  The bytes are in PDP-11 order.  The X
 * 		    position is incremented to follow the last byte of
 * 		    data.
 * 
 * RunData (6):	    The datum is one less than the run length.  The
 * 		    following word contains (in its lower 8 bits) the
 * 		    color of the run.  The X position is incremented to
 * 		    follow the last byte in the run.
 */

#define UPPER 255			/* anything bigger ain't a byte */

/* Predefine LITTLE_ENDIAN for vax and pdp11 machines */
#if defined(vax) || defined(pdp11)
#define LITTLE_ENDIAN
#endif

#ifndef LITTLE_ENDIAN
static rle_pixel t;
static rle_pixel *a1,*a2;
static short tm;
#endif

#ifdef LITTLE_ENDIAN
#define	ASN(a,b)	a = b
#else
#define	ASN(a,b)	(tm=(int)(b)),(a1=(rle_pixel*)(&(a))),\
			(a2=(rle_pixel*)(&(tm))),(*a1=(*(a2+1))),(*(a1+1)=(*a2))
#endif

/*
 * Macros to make writing instructions with correct byte order easier.
 */
union { short s; char c[2]; } arg;
#ifdef LITTLE_ENDIAN
#define	put16(a)    arg.s=a,putc(arg.c[0],sv_fd), putc(arg.c[1],sv_fd)
#else
#define	put16(a)    arg.s=a,putc(arg.c[1],sv_fd), putc(arg.c[0],sv_fd)
#endif

/* short instructions */
#define mk_short_1(oper,a1)		/* one argument short */ \
    putc(oper,sv_fd), putc((char)a1,sv_fd)

#define mk_short_2(oper,a1,a2)		/* two argument short */ \
    putc(oper,sv_fd), putc((char)a1,sv_fd), put16(a2)

/* long instructions */
#define mk_long_1(oper,a1)		/* one argument long */ \
    putc((char)(LONG|oper),sv_fd), putc('\0', sv_fd), put16(a1)

#define mk_long_2(oper,a1,a2)		/* two argument long */ \
    putc((char)(LONG|oper),sv_fd), putc('\0', sv_fd), \
    put16(a1), put16(a2)

/* choose between long and short format instructions */
/* NOTE: these macros can only be used where a STATEMENT is legal */

#define mk_inst_1(oper,a1)		/* one argument inst */ \
    if (a1>UPPER) (mk_long_1(oper,a1)); else (mk_short_1(oper,a1))

#define mk_inst_2(oper,a1,a2)		/* two argument inst */ \
    if (a1>UPPER) (mk_long_2(oper,a1,a2)); else (mk_short_2(oper,a1,a2))

/* 
 * Opcode definitions
 */
#define	    RSkipLines(n)   	    mk_inst_1(RSkipLinesOp,(n))

#define	    RSetColor(c)	    mk_short_1(RSetColorOp,(c))
				    /* has side effect of performing */
				    /* "carriage return" action */

#define	    RSkipPixels(n)	    mk_inst_1(RSkipPixelsOp,(n))

#define	    RNewLine		    RSkipLines(1)

#define	    RByteData(n)	    mk_inst_1(RByteDataOp,n)
					/* followed by ((n+1)/2)*2 bytes */
					/* of data.  If n is odd, last */
					/* byte will be ignored */
					/* "cursor" is left at pixel */
					/* following last pixel written */

#define	    RRunData(n,c)	    mk_inst_2(RRunDataOp,(n),(c))
					/* next word contains color data */
					/* "cursor" is left at pixel after */
					/* end of run */

#define     REOF		    mk_inst_1(REOFOp,0)
					/* Really opcode only */

/*****************************************************************
 * TAG( RunSetup )
 * Put out initial setup data for RLE svfb files.
 */
void
RunSetup(globals)
register struct sv_globals * globals;
{
}

/*****************************************************************
 * TAG( RunSkipBlankLines )
 * Skip one or more blank lines in the RLE file.
 */
void
RunSkipBlankLines(nblank, globals)
register struct sv_globals * globals;
{
}

/*****************************************************************
 * TAG( RunSetColor )
 * Select a color and do carriage return.
 * color: 0 = Red, 1 = Green, 2 = Blue.
 */
void
RunSetColor(c, globals)
register struct sv_globals * globals;
{
}

/*****************************************************************
 * TAG( RunSkipPixels )
 * Skip a run of background.
 */

/* ARGSUSED */
void
RunSkipPixels(nskip, last, wasrun, globals)
register struct sv_globals * globals;
{
}

/*****************************************************************
 * TAG( RunNewScanLine )
 * Perform a newline action.  Since CR is implied by the Set Color
 * operation, only generate code if the newline flag is true.
 */
void
RunNewScanLine(flag, globals)
register struct sv_globals * globals;
{
}

/*****************************************************************
 * TAG( Runputdata )
 * Put one or more pixels of byte data into the output file.
 */
void
Runputdata(buf, n, globals)
rle_pixel * buf;
register struct sv_globals * globals;
{
}

/*****************************************************************
 * TAG( Runputrun )
 * Output a single color run.
 */

/* ARGSUSED */
void
Runputrun(color, n, last, globals)
register struct sv_globals * globals;
{
}


/*****************************************************************
 * TAG( RunputEof )
 * Output an EOF opcode
 */
void
RunputEof( globals )
register struct sv_globals * globals;
{
}

/*ARGSUSED*/
void
DefaultBlockHook(globals)
struct sv_globals * globals;
{
}


/*
 * This software is copyrighted as noted below.  It may be freely copied,
 * modified, and redistributed, provided that the copyright notice is 
 * preserved on all copies.
 * 
 * There is no warranty or other guarantee of fitness for this software,
 * it is provided solely "as is".  Bug reports or fixes may be sent
 * to the author, who may or may not act on them as he desires.
 *
 * You may not include this software in a program or other software product
 * without supplying the source, or without informing the end-user that the 
 * source is available for no extra charge.
 *
 * If you modify this software, you should include a notice giving the
 * name of the person performing the modification, the date of modification,
 * and the reason for such modification.
 */
/* 
 * buildmap.c - Build a color map from the RLE file color map.
 * 
 * Author:	Spencer W. Thomas
 * 		Computer Science Dept.
 * 		University of Utah
 * Date:	Sat Jan 24 1987
 * Copyright (c) 1987, University of Utah
 */

/*****************************************************************
 * TAG( buildmap )
 * 
 * Returns a color map that can easily be used to map the pixel values in
 * an RLE file.  Map is built from the color map in the input file.
 * Inputs:
 * 	globals:	sv_globals structure containing color map.
 *	minmap:		Minimum number of channels in output map.
 *	gamma:		Adjust color map for this image gamma value
 *			(1.0 means no adjustment).
 * Outputs:
 * 	Returns an array of pointers to arrays of rle_pixels.  The array
 *	of pointers contains max(sv_ncolors, sv_ncmap) elements, each 
 *	array of pixels contains 2^sv_cmaplen elements.  The pixel arrays
 *	should be considered read-only.
 * Assumptions:
 * 	[None]
 * Algorithm:
 *	Ensure that there are at least sv_ncolors rows in the map, and
 *	that each has at least 256 elements in it (largest map that can
 *	be addressed by an rle_pixel).
 */
rle_pixel **
buildmap( globals, minmap, gamma )
struct sv_globals *globals;
int minmap;
double gamma;
{
    rle_pixel ** cmap, * gammap;
    register int i, j;
    int maplen, cmaplen, ncmap, nmap;

    if ( globals->sv_ncmap == 0 )	/* make identity map */
    {
	nmap = (minmap < globals->sv_ncolors) ? globals->sv_ncolors : minmap;
	cmap = (rle_pixel **)lmalloc( nmap * sizeof(rle_pixel *) );
	cmap[0] = (rle_pixel *)lmalloc( 256 * sizeof(rle_pixel) );
	for ( i = 0; i < 256; i++ )
	    cmap[0][i] = i;
	for ( i = 1; i < nmap; i++ )
	    cmap[i] = cmap[0];
	maplen = 256;
	ncmap = 1;		/* number of unique rows */
    }
    else			/* make map from globals */
    {
	/* Map is at least 256 long */
	cmaplen = (1 << globals->sv_cmaplen);
	if ( cmaplen < 256 )
	    maplen = 256;
	else
	    maplen = cmaplen;

	if ( globals->sv_ncmap == 1 )	/* make "b&w" map */
	{
	    nmap = (minmap < globals->sv_ncolors) ?
		globals->sv_ncolors : minmap;
	    cmap = (rle_pixel **)lmalloc( nmap * sizeof(rle_pixel *) );
	    cmap[0] = (rle_pixel *)lmalloc( maplen * sizeof(rle_pixel) );
	    for ( i = 0; i < maplen; i++ )
		if ( i < cmaplen )
		    cmap[0][i] = globals->sv_cmap[i] >> 8;
		else
		    cmap[0][i] = i;
	    for ( i = 1; i < nmap; i++ )
		cmap[i] = cmap[0];
	    ncmap = 1;
	}
	else if ( globals->sv_ncolors <= globals->sv_ncmap )
	{
	    nmap = (minmap < globals->sv_ncmap) ? globals->sv_ncmap : minmap;
	    cmap = (rle_pixel **)lmalloc( nmap * sizeof(rle_pixel *) );
	    for ( j = 0; j < globals->sv_ncmap; j++ )
	    {
		cmap[j] = (rle_pixel *)lmalloc( maplen * sizeof(rle_pixel) );
		for ( i = 0; i < maplen; i++ )
		    if ( i < cmaplen )
			cmap[j][i] = globals->sv_cmap[j*cmaplen + i] >> 8;
		    else
			cmap[j][i] = i;
	    }
	    for ( i = j, j--; i < nmap; i++ )
		cmap[i] = cmap[j];
	    ncmap = globals->sv_ncmap;
	}
	else			/* ncolors > ncmap */
	{
	    nmap = (minmap < globals->sv_ncolors) ?
		globals->sv_ncolors : minmap;
	    cmap = (rle_pixel **)lmalloc( nmap * sizeof(rle_pixel *) );
	    for ( j = 0; j < globals->sv_ncmap; j++ )
	    {
		cmap[j] = (rle_pixel *)lmalloc( maplen * sizeof(rle_pixel) );
		for ( i = 0; i < maplen; i++ )
		    if ( i < cmaplen )
			cmap[j][i] = globals->sv_cmap[j*cmaplen + i] >> 8;
		    else
			cmap[j][i] = i;
	    }
	    for( i = j, j--; i < nmap; i++ )
		cmap[i] = cmap[j];
	    ncmap = globals->sv_ncmap;
	}
    }
	    
    /* Gamma compensate if requested */
    if ( gamma != 1.0 )
    {
	gammap = (rle_pixel *)lmalloc( 256 * sizeof(rle_pixel) );
	for ( i = 0; i < 256; i++ )
		{
#ifdef BYTEBUG
		int byteb1;
		byteb1 = (int)(0.5 + 255.0 * pow( i / 255.0, gamma ));
		gammap[i] = byteb1;
#else
	    gammap[i] = (int)(0.5 + 255.0 * pow( i / 255.0, gamma ));
#endif
		}
	for ( i = 0; i < ncmap; i++ )
	    for ( j = 0; j < maplen; j++ )
		cmap[i][j] = gammap[cmap[i][j]];
    }

    return cmap;
}

/*
 * This software is copyrighted as noted below.  It may be freely copied,
 * modified, and redistributed, provided that the copyright notice is 
 * preserved on all copies.
 * 
 * There is no warranty or other guarantee of fitness for this software,
 * it is provided solely "as is".  Bug reports or fixes may be sent
 * to the author, who may or may not act on them as he desires.
 *
 * You may not include this software in a program or other software product
 * without supplying the source, or without informing the end-user that the 
 * source is available for no extra charge.
 *
 * If you modify this software, you should include a notice giving the
 * name of the person performing the modification, the date of modification,
 * and the reason for such modification.
 */
/* 
 * rle_getcom.c - Get specific comments from globals structure.
 * 
 * Author:	Spencer W. Thomas
 * 		Computer Science Dept.
 * 		University of Utah
 * Date:	Sun Jan 25 1987
 * Copyright (c) 1987, University of Utah
 */

/*****************************************************************
 * TAG( match )
 * 
 * Match a name against a test string for "name=value" or "name".
 * If it matches name=value, return pointer to value part, if just
 * name, return pointer to NUL at end of string.  If no match, return NULL.
 *
 * Inputs:
 * 	n:	Name to match.  May also be "name=value" to make it easier
 *		to replace comments.
 *	v:	Test string.
 * Outputs:
 * 	Returns pointer as above.
 * Assumptions:
 *	[None]
 * Algorithm:
 *	[None]
 */
static char *
match( n, v )
register char *n;
register char *v;
{
    for ( ; *n != '\0' && *n != '=' && *n == *v; n++, v++ )
	;
    if (*n == '\0' || *n == '=')
	if ( *v == '\0' )
	    return v;
	else if ( *v == '=' )
	    return ++v;

    return NULL;
}

/*****************************************************************
 * TAG( rle_getcom )
 * 
 * Return a pointer to the value part of a name=value pair in the comments.
 * Inputs:
 * 	name:		Name part of the comment to search for.
 *	globals:	sv_globals structure.
 * Outputs:
 * 	Returns pointer to value part of comment or NULL if no match.
 * Assumptions:
 *	[None]
 * Algorithm:
 *	[None]
 */
char *
rle_getcom( name, globals )
char *name;
struct sv_globals *globals;
{
    char ** cp;
    char * v;

    if ( globals->sv_comments == NULL )
	return NULL;

    for ( cp = globals->sv_comments; *cp; cp++ )
	if ( (v = match( name, *cp )) != NULL )
	    return v;

    return NULL;
}

/*
 * This software is copyrighted as noted below.  It may be freely copied,
 * modified, and redistributed, provided that the copyright notice is 
 * preserved on all copies.
 * 
 * There is no warranty or other guarantee of fitness for this software,
 * it is provided solely "as is".  Bug reports or fixes may be sent
 * to the author, who may or may not act on them as he desires.
 *
 * You may not include this software in a program or other software product
 * without supplying the source, or without informing the end-user that the 
 * source is available for no extra charge.
 *
 * If you modify this software, you should include a notice giving the
 * name of the person performing the modification, the date of modification,
 * and the reason for such modification.
 *
 *  Modified at BRL 16-May-88 by Mike Muuss to avoid Alliant STDC desire
 *  to have all "void" functions so declared.
 */
/* 
 * rle_getrow.c - Read an RLE file in.
 * 
 * Author:	Spencer W. Thomas
 * 		Computer Science Dept.
 * 		University of Utah
 * Date:	Wed Apr 10 1985
 * Copyright (c) 1985 Spencer W. Thomas
 * 
 */
/*
 * Automatically define LITTLE_ENDIAN on vax and pdp11 machines
 */
#if defined(vax) || defined(pdp11)
#define	LITTLE_ENDIAN
#endif

struct inst {
  unsigned opcode:8, datum:8;
};

#define BREAD(type, var, len)\
	    zread( infile, (char *)&var,len )
#define OPCODE(inst) (inst.opcode & ~LONG)
#define LONGP(inst) (inst.opcode & LONG)
#define DATUM(inst) (0x00ff & inst.datum)

static int	   debug_f;		/* if non-zero, print debug info */
static void	bfill();

/*****************************************************************
 * TAG( rle_get_setup )
 * 
 * Read the initialization information from an RLE file.
 * Inputs:
 * 	globals:    Contains pointer to the input file.
 * Outputs:
 * 	globals:    Initialized with information from the
 *		    input file.
 *	Returns 0 on success, -1 if the file is not an RLE file,
 *	-2 if malloc of the color map failed, -3 if an immediate EOF
 *	is hit (empty input file), and -4 if an EOF is encountered reading
 *	the setup information.
 * Assumptions:
 * 	infile points to the "magic" number in an RLE file (usually
 * byte 0 in the file).
 * Algorithm:
 * 	Read in the setup info and fill in sv_globals.
 */
rle_get_setup( globals )
struct sv_globals * globals;
{
    struct XtndRsetup setup;
    short magic;			/* assume 16 bits */
    register ZFILE *infile = globals->svfb_fd;
    rle_pixel * bg_color;
    register int i;
    char * comment_buf;

    zclearerr(infile);
    BREAD( short, magic, sizeof magic );
    SWAB(magic);
    if ( zeof( infile ) )
	return -3;
    if ( magic != XtndRMAGIC )
	return -1;
    BREAD( struct XtndRsetup, setup, SETUPSIZE );  /* assume VAX packing */
    if ( zeof( infile ) )
	return -4;
    SWAB( setup.h_xpos );
    SWAB( setup.h_ypos );
    SWAB( setup.h_xlen );
    SWAB( setup.h_ylen );

    /* Extract information from setup */
    globals->sv_ncolors = setup.h_ncolors;
    for ( i = 0; i < globals->sv_ncolors; i++ )
	SV_SET_BIT( *globals, i );

    if ( !(setup.h_flags & H_NO_BACKGROUND) )
    {
	globals->sv_bg_color = (int *)lmalloc(
	    (unsigned)(sizeof(int) * setup.h_ncolors) );
	bg_color = (rle_pixel *)lmalloc(
	    (unsigned)(1 + (setup.h_ncolors / 2) * 2) );
	zread( infile, (char *)bg_color, 1 + (setup.h_ncolors / 2) * 2 );
	for ( i = 0; i < setup.h_ncolors; i++ )
	    globals->sv_bg_color[i] = bg_color[i];
	lfree( bg_color );
    }
    else
	zgetc( infile );			/* skip filler byte */

    if ( setup.h_flags & H_NO_BACKGROUND )
	globals->sv_background = 0;
    else if ( setup.h_flags & H_CLEARFIRST )
	globals->sv_background = 2;
    else
	globals->sv_background = 1;
    if ( setup.h_flags & H_ALPHA )
    {
	globals->sv_alpha = 1;
	SV_SET_BIT( *globals, SV_ALPHA );
    }
    else
	globals->sv_alpha = 0;

    globals->sv_xmin = setup.h_xpos;
    globals->sv_ymin = setup.h_ypos;
    globals->sv_xmax = globals->sv_xmin + setup.h_xlen - 1;
    globals->sv_ymax = globals->sv_ymin + setup.h_ylen - 1;

    globals->sv_ncmap = setup.h_ncmap;
    globals->sv_cmaplen = setup.h_cmaplen;
    if ( globals->sv_ncmap > 0 )
    {
	register int maplen =
		     globals->sv_ncmap * (1 << globals->sv_cmaplen);
	globals->sv_cmap = (rle_map *)lmalloc(
	    (unsigned)(sizeof(rle_map) * maplen) );
	if ( globals->sv_cmap == NULL )
	{
	    fprintf( stderr,
		"Malloc failed for color map of size %d*%d in rle_get_setup\n",
		globals->sv_ncmap, (1 << globals->sv_cmaplen) );
	    return -2;
	}
	zread( infile, (char *)globals->sv_cmap, sizeof(short) * maplen );
#ifndef LITTLE_ENDIAN
    	/* Swap bytes on bigendian machines
	 */
    	for ( i = 0; i < maplen; i++ )
    	    SWAB( globals->sv_cmap[i] );
#endif
    }

    /* Check for comments */
    if ( setup.h_flags & H_COMMENT )
    {
	short comlen, evenlen;
	register char * cp;

	BREAD( short, comlen, sizeof comlen );	/* get comment length */
	SWAB( comlen );
	evenlen = (comlen + 1) & ~1;	/* make it even */
	comment_buf = (char *)lmalloc( (unsigned) evenlen );
	if ( comment_buf == NULL )
	{
	    fprintf( stderr,
		     "Malloc failed for comment buffer of size %d in rle_get_setup\n",
		     comlen );
	    return -2;
	}
	zread( infile, comment_buf, evenlen );
	/* Count the comments */
	for ( i = 0, cp = comment_buf; cp < comment_buf + comlen; cp++ )
	    if ( *cp == 0 )
		i++;
	i++;			/* extra for NULL pointer at end */
	/* Get space to put pointers to comments */
	globals->sv_comments =
	    (char **)lmalloc( (unsigned)(i * sizeof(char *)) );
	if ( globals->sv_comments == NULL )
	{
	    fprintf( stderr,
		    "Malloc failed for %d comment pointers in rle_get_setup\n",
		     i );
	    return -2;
	}
	/* Get pointers to the comments */
	*globals->sv_comments = comment_buf;
	for ( i = 1, cp = comment_buf + 1; cp < comment_buf + comlen; cp++ )
	    if ( *(cp - 1) == 0 )
		globals->sv_comments[i++] = cp;
	globals->sv_comments[i] = NULL;
    }
    else
	globals->sv_comments = NULL;

    /* Initialize state for rle_getrow */
    globals->sv_private.get.scan_y = globals->sv_ymin;
    globals->sv_private.get.vert_skip = 0;
    globals->sv_private.get.is_eof = 0;
    globals->sv_private.get.is_seek = 0;	/* Can't do seek on zfile */
    debug_f = 0;

    if ( !zeof( infile ) )
	return 0;			/* success! */
    else
    {
	globals->sv_private.get.is_eof = 1;
	return -4;
    }
}


/*****************************************************************
 * TAG( rle_get_error )
 * 
 * Print an error message for the return code from rle_get_setup
 * Inputs:
 * 	code:		The return code from rle_get_setup.
 *	pgmname:	Name of this program (argv[0]).
 *	fname:		Name of the input file.
 * Outputs:
 * 	Prints an error message on standard output.
 *	Returns code.
 */

rle_get_error( code, pgmname, fname )
char *pgmname;
char *fname;
{
    switch( code )
    {
    case 0:			/* success */
	break;

    case -1:			/* Not an RLE file */
	fprintf( stderr, "%s: %s is not an RLE file\n",
		 pgmname, fname );
	break;

    case -2:			/* malloc failed */
	fprintf( stderr,
		 "%s: Malloc failed reading header of file %s\n",
		 pgmname, fname );
	break;

    case -3:
	fprintf( stderr, "%s: %s is an empty file\n", pgmname, fname );
	break;

    case -4:
	fprintf( stderr,
		 "%s: RLE header of %s is incomplete (premature EOF)\n",
		 pgmname, fname );
	break;

    default:
	fprintf( stderr, "%s: Error encountered reading header of %s\n",
		 pgmname, fname );
	break;
    }
    return code;
}


/*****************************************************************
 * TAG( rle_get_setup_ok )
 * 
 * Read the initialization information from an RLE file.
 * Inputs:
 * 	globals:    Contains pointer to the input file.
 *	prog_name:  Program name to be printed in the error message.
 *      file_name:  File name to be printed in the error message.
 *                  If NULL, the string "stdin" is generated.
 * Outputs:
 * 	globals:    Initialized with information from the
 *		    input file.
 *      If reading the globals fails, it prints an error message
 *	and exits with the appropriate status code.
 * Algorithm:
 * 	sv_get_setup does all the work.
 */
void
rle_get_setup_ok( globals, prog_name, file_name )
struct sv_globals * globals;
char *prog_name;
char *file_name;
{
    int code;

    if (! file_name)
	file_name = "stdin";

    code = rle_get_error( rle_get_setup( globals ), prog_name, file_name );
    if (code)
	exit( code );
}


/*****************************************************************
 * TAG( rle_debug )
 * 
 * Turn RLE debugging on or off.
 * Inputs:
 * 	on_off:		if 0, stop debugging, else start.
 * Outputs:
 * 	Sets internal debug flag.
 * Assumptions:
 *	[None]
 * Algorithm:
 *	[None]
 */
void
rle_debug( on_off )
int on_off;
{
    debug_f = on_off;
}


/*****************************************************************
 * TAG( rle_getrow )
 * 
 * Get a scanline from the input file.
 * Inputs:
 *	globals:    sv_globals structure containing information about 
 *		    the input file.
 * Outputs:
 * 	scanline:   an array of pointers to the individual color
 *		    scanlines.  Scanline is assumed to have
 *		    globals->sv_ncolors pointers to arrays of rle_pixel,
 *		    each of which is at least globals->sv_xmax+1 long.
 *	Returns the current scanline number.
 * Assumptions:
 * 	rle_get_setup has already been called.
 * Algorithm:
 * 	If a vertical skip is being executed, and clear-to-background is
 *	specified (globals->sv_background is true), just set the
 *	scanlines to the background color.  If clear-to-background is
 *	not set, just increment the scanline number and return.
 * 
 *	Otherwise, read input until a vertical skip is encountered,
 *	decoding the instructions into scanline data.
 */

rle_getrow( globals, scanline )
struct sv_globals * globals;
rle_pixel *scanline[];
{
    register rle_pixel * scanc;
    register int nc;
    register ZFILE *infile = globals->svfb_fd;
    int scan_x = globals->sv_xmin,	/* current X position */
	   channel = 0;			/* current color channel */
    short word, long_data;
    struct inst inst;

    /* Clear to background if specified */
    if ( globals->sv_background == 2 )
    {
	if ( globals->sv_alpha && SV_BIT( *globals, -1 ) )
	    bfill( (char *)scanline[-1], globals->sv_xmax + 1, 0 );
	for ( nc = 0; nc < globals->sv_ncolors; nc++ )
	    if ( SV_BIT( *globals, nc ) )
		bfill( (char *)scanline[nc], globals->sv_xmax+1,
			globals->sv_bg_color[nc] );
    }

    /* If skipping, then just return */
    if ( globals->sv_private.get.vert_skip > 0 )
    {
	globals->sv_private.get.vert_skip--;
	globals->sv_private.get.scan_y++;
	if ( globals->sv_private.get.vert_skip > 0 )
	    return globals->sv_private.get.scan_y;
    }

    /* If EOF has been encountered, return also */
    if ( globals->sv_private.get.is_eof )
	return ++globals->sv_private.get.scan_y;

    /* Otherwise, read and interpret instructions until a skipLines
     * instruction is encountered.
     */
    if ( SV_BIT( *globals, channel ) )
	scanc = scanline[channel] + scan_x;
    else
	scanc = NULL;
    for (;;)
    {
        BREAD(struct inst, inst, 2 );
	if ( zeof(infile) )
	{
	    globals->sv_private.get.is_eof = 1;
	    break;		/* <--- one of the exits */
	}

	switch( OPCODE(inst) )
	{
	case RSkipLinesOp:
	    if ( LONGP(inst) )
	    {
	        BREAD( short, long_data, sizeof long_data );
		SWAB( long_data );
		globals->sv_private.get.vert_skip = long_data;
	    }
	    else
		globals->sv_private.get.vert_skip = DATUM(inst);
	    if (debug_f)
		fprintf(stderr, "Skip %d Lines (to %d)\n",
			globals->sv_private.get.vert_skip,
			globals->sv_private.get.scan_y +
			    globals->sv_private.get.vert_skip );

	    break;			/* need to break for() here, too */

	case RSetColorOp:
	    channel = DATUM(inst);	/* select color channel */
	    if ( channel == 255 )
		channel = -1;
	    scan_x = globals->sv_xmin;
	    if ( SV_BIT( *globals, channel ) )
		scanc = scanline[channel]+scan_x;
	    if ( debug_f )
		fprintf( stderr, "Set color to %d (reset x to %d)\n",
			 channel, scan_x );
	    break;

	case RSkipPixelsOp:
	    if ( LONGP(inst) )
	    {
	        BREAD( short, long_data, sizeof long_data );
		SWAB( long_data );
		scan_x += long_data;
		scanc += long_data;
		if ( debug_f )
		    fprintf( stderr, "Skip %d pixels (to %d)\n",
			    long_data, scan_x );
			 
	    }
	    else
	    {
		scan_x += DATUM(inst);
		scanc += DATUM(inst);
		if ( debug_f )
		    fprintf( stderr, "Skip %d pixels (to %d)\n",
			    DATUM(inst), scan_x );
	    }
	    break;

	case RByteDataOp:
	    if ( LONGP(inst) )
	    {
	        BREAD( short, long_data, sizeof long_data );
		SWAB( long_data );
		nc = (int)long_data;
	    }
	    else
		nc = DATUM(inst);
	    nc++;
	    if ( SV_BIT( *globals, channel ) )
	    {
		zread( infile, (char *)scanc, nc );
		if ( nc & 1 )
		    (void)zgetc( infile );	/* throw away odd byte */
	    }
	    else
		{		/* Emulate a forward fseek */
		    register int ii;
		    for ( ii = ((nc + 1) / 2) * 2; ii > 0; ii-- )
			(void) zgetc( infile );	/* discard it */
		}

	    scanc += nc;
	    scan_x += nc;
	    if ( debug_f )
		if ( SV_BIT( *globals, channel ) )
		{
		    rle_pixel * cp = scanc - nc;
		    fprintf( stderr, "Pixel data %d (to %d):", nc, scan_x );
		    for ( ; nc > 0; nc-- )
			fprintf( stderr, "%02x", *cp++ );
		    putc( '\n', stderr );
		}
	    else
		fprintf( stderr, "Pixel data %d (to %d)\n", nc, scan_x );
	    break;

	case RRunDataOp:
	    if ( LONGP(inst) )
	    {
	        BREAD( short, long_data, sizeof long_data );
		SWAB( long_data );
		nc = long_data;
	    }
	    else
		nc = DATUM(inst);
	    scan_x += nc + 1;

	    BREAD( short, word, sizeof(short) );
	    SWAB( word );
	    if ( debug_f )
		fprintf( stderr, "Run length %d (to %d), data %02x\n",
			    nc + 1, scan_x, word );
	    if ( SV_BIT( *globals, channel ) )
	    {
		if ( nc >= 10 )		/* break point for 785, anyway */
		{
		    bfill( (char *)scanc, nc + 1, word );
		    scanc += nc + 1;
		}
		else
		    for ( ; nc >= 0; nc--, scanc++ )
			*scanc = word;
	    }
	    break;

	case REOFOp:
	    globals->sv_private.get.is_eof = 1;
	    break;

	default:
	    fprintf( stderr,
		     "rle_getrow: Unrecognized opcode: %d\n", inst.opcode );
	    exit(1);
	}
	if ( OPCODE(inst) == RSkipLinesOp || OPCODE(inst) == REOFOp )
	    break;			/* <--- the other loop exit */
    }

    return globals->sv_private.get.scan_y;
}


/* Fill buffer at s with n copies of character c.  N must be <= 65535*/
/* ARGSUSED */
static void bfill( s, n, c )
char *s;
int n, c;
{
#ifdef vax
    asm("   movc5   $0,*4(ap),12(ap),8(ap),*4(ap)");
#else
    while ( n-- > 0 )
	*s++ = c;
#endif
}

/*
 * This software is copyrighted as noted below.  It may be freely copied,
 * modified, and redistributed, provided that the copyright notice is 
 * preserved on all copies.
 * 
 * There is no warranty or other guarantee of fitness for this software,
 * it is provided solely "as is".  Bug reports or fixes may be sent
 * to the author, who may or may not act on them as he desires.
 *
 * You may not include this software in a program or other software product
 * without supplying the source, or without informing the end-user that the 
 * source is available for no extra charge.
 *
 * If you modify this software, you should include a notice giving the
 * name of the person performing the modification, the date of modification,
 * and the reason for such modification.
 *
 *  Modified at BRL 16-May-88 by Mike Muuss to avoid Alliant STDC desire
 *  to have all "void" functions so declared.
 *
 * Modified to generate an apropriate size dither map (ie 2x2, 4x4, 8x8
 * or 16x16) rather than use fixed 16x16 map. Use a large enough map 
 * to give a minimum of 128 effective levels rather than aiming for 256.
 * This should give less grainy pictures.
 * Two global variables can modify this:
 * dith_levels = 128 (default)
 * dith_np2 = 0	(default). Nonzero to enable non power of 2 dither mapping.
 * dith_size = actual dither matrix size chosen.
 *
 * Graeme Gill	3 June 88
 */

/* 
 * dither.c - Functions for RGB color dithering.
 * 
 * Author:	Spencer W. Thomas
 * 		Computer Science Dept.
 * 		University of Utah
 * Date:	Mon Feb  2 1987
 * Copyright (c) 1987, University of Utah
 */

void	make_square();

/* dither globals */
int dith_levels = 128;
int dith_np2 = 0;
int dith_size = 16;

/* basic dithering macro */
#define DMAP(v,x,y)	(modN[v]>magic[x][y] ? divN[v] + 1 : divN[v])

/*****************************************************************
 * TAG( dithermap )
 * 
 * Create a color dithering map with a specified number of intensity levels.
 * Inputs:
 * 	levels:		Intensity levels per primary.
 *	gamma:		Display gamma value.
 * Outputs:
 * 	rgbmap:		Generated color map.
 *	divN:		"div" function for dithering.
 *	modN:		"mod" function for dithering.
 * Assumptions:
 * 	rgbmap will hold levels^3 entries.
 * Algorithm:
 *	Compute gamma compensation map.
 *	N = 255.0 / (levels - 1) is number of pixel values per level.
 *	Compute rgbmap with red ramping fastest, green slower, and blue
 *	slowest (treat it as if it were rgbmap[levels][levels][levels][3]).
 *	Call make_square to get divN, modN, and magic
 *
 * Note:
 *	Call dithergb( x, y, r, g, b, levels, divN, modN, magic ) to get index
 *	into rgbmap for a given color/location pair, or use
 *	    row = y % 16; col = x % 16;
 *	    DMAP(v,col,row) =def (divN[v] + (modN[v]>magic[col][row] ? 1 : 0))
 *	    DMAP(r,col,row) + DMAP(g,col,row)*levels + DMAP(b,col,row)*levels^2
 *	if you don't want function call overhead.
 */
void
dithermap( levels, gamma, rgbmap, divN, modN, magic )
double gamma;
int rgbmap[][3];
int divN[256];
int modN[256];
int magic[16][16];
{
    double N, magicfact;
    register int i, j, k, l;
    int levelsq, levelsc;
    int gammamap[256];
    
	make_gamma(gamma,gammamap);

    levelsq = levels*levels;	/* squared */
    levelsc = levels*levelsq;	/* and cubed */

    N = 255.0 / (levels - 1);    /* Get size of each step */

    /* 
     * Set up the color map entries.
     */
    for(i = 0; i < levelsc; i++) {
	rgbmap[i][0] = gammamap[(int)(0.5 + (i%levels) * N)];
	rgbmap[i][1] = gammamap[(int)(0.5 + ((i/levels)%levels) * N)];
	rgbmap[i][2] = gammamap[(int)(0.5 + ((i/levelsq)%levels) * N)];
    }

    make_square( N, divN, modN, magic );
}


/*****************************************************************
 * TAG( bwdithermap )
 * 
 * Create a color dithering map with a specified number of intensity levels.
 * Inputs:
 * 	levels:		Intensity levels.
 *	gamma:		Display gamma value.
 * Outputs:
 * 	bwmap:		Generated black & white map.
 *	divN:		"div" function for dithering.
 *	modN:		"mod" function for dithering.
 * Assumptions:
 * 	bwmap will hold levels entries.
 * Algorithm:
 *	Compute gamma compensation map.
 *	N = 255.0 / (levels - 1) is number of pixel values per level.
 *	Compute bwmap for levels entries.
 *	Call make_square to get divN, modN, and magic.
 * Note:
 *	Call ditherbw( x, y, val, divN, modN, magic ) to get index into 
 *	bwmap for a given color/location pair, or use
 *	    row = y % 16; col = x % 16;
 *	    divN[val] + (modN[val]>magic[col][row] ? 1 : 0)
 *	if you don't want function call overhead.
 *	On a 1-bit display, use
 *	    divN[val] > magic[col][row] ? 1 : 0
 */
void
bwdithermap( levels, gamma, bwmap, divN, modN, magic )
double gamma;
int bwmap[];
int divN[256];
int modN[256];
int magic[16][16];
{
    double N, magicfact;
    register int i, j, k, l;
    int gammamap[256];
    
	make_gamma(gamma,gammamap);

    N = 255.0 / (levels - 1);    /* Get size of each step */

    /* 
     * Set up the color map entries.
     */
    for(i = 0; i < levels; i++)
	bwmap[i] = gammamap[(int)(0.5 + i * N)];

    make_square( N, divN, modN, magic );
}


/*****************************************************************
 * TAG( make_square )
 * 
 * Build the magic square for a given number of levels.
 * Inputs:
 * 	N:		Pixel values per level (255.0 / (levels-1)).
 * (global) dith_levels = 128 (default) - number of effective levels to aim for 
 * (global) dith_np2 = 0 (default) - non-zero if non power of two size is permissable.
 * Outputs:
 * 	divN:		Integer value of pixval / N
 *	modN:		Integer remainder between pixval and divN[pixval]*N
 *	magic:		Magic square for dithering to N sublevels.
 * (global) dith_size = magic square size chosen.
 * Assumptions:
 * 	
 * Algorithm:
 *	divN[pixval] = (int)(pixval / N) maps pixval to its appropriate level.
 *	modN[pixval] = pixval - (int)(N * divN[pixval]) maps pixval to
 *	its sublevel, and is used in the dithering computation.
 */
void
make_square( N, divN, modN, magic )
double N;
int divN[256];
int modN[256];
int magic[16][16] ;
{
    register int i, j, k, l;
    double magicfact;

    for ( i = 0; i < 256; i++ )
    {
	divN[i] = (int)(i / N);
	modN[i] = i - (int)(N * divN[i]);
    }
    modN[255] = 0;		/* always */

	/* figure out how big a square will give */
	/* the desired number of levels */
	if(dith_np2)
		for(dith_size= 2;((dith_size * dith_size)+1)<((N*dith_levels)/256);dith_size++ );
	else
		for(dith_size= 2;((dith_size * dith_size)+1)<((N*dith_levels)/256);dith_size *=2);

	/* make the basic square up */
	/* ( will have numbers 0 - size * size ) */
	make_magic(dith_size,magic);
	
	/* divN gives 0 - levels-1 */
	/* modN gives 0 - N-1 */
	/* dither is if(modN(pix) > magic[][] so */
	/* scale magic it to have levels 0 to N-2 */
	/* (ie takes account of magic square size allows size * size +1 levels */

	magicfact = (N-2)/((double)((dith_size * dith_size)-1));
	for(i=0;i<dith_size;i++)
	{
		for(j=0;j<dith_size;j++)
		{
			magic[i][j] = (int)(0.5 + magic[i][j] * magicfact);
		}
	}

	if(!dith_np2)	/* if we have power of 2 */
	{
		/* now replicate the size square we've chosen */  
		/* (and use a brick pattern) */
		for(k=0;k<16;k += dith_size)
		{
			for(l=k>0?0:dith_size;l<16;l += dith_size)
			{
				for(i=0;i<dith_size;i++)
				{
					for(j=0;j<dith_size;j++)
					{
						magic[k+i][((l+k/2)+j)%16] = magic[i][j];
					}
				}
			}
		}
	}
}

int magic16x16[16][16] = 
	{
		{0,128,32,160,8,136,40,168,2,130,34,162,10,138,42,170},
		{192,64,224,96,200,72,232,104,194,66,226,98,202,74,234,106},
		{48,176,16,144,56,184,24,152,50,178,18,146,58,186,26,154},
		{240,112,208,80,248,120,216,88,242,114,210,82,250,122,218,90},
		{12,140,44,172,4,132,36,164,14,142,46,174,6,134,38,166},
		{204,76,236,108,196,68,228,100,206,78,238,110,198,70,230,102},
		{60,188,28,156,52,180,20,148,62,190,30,158,54,182,22,150},
		{252,124,220,92,244,116,212,84,254,126,222,94,246,118,214,86},
		{3,131,35,163,11,139,43,171,1,129,33,161,9,137,41,169},
		{195,67,227,99,203,75,235,107,193,65,225,97,201,73,233,105},
		{51,179,19,147,59,187,27,155,49,177,17,145,57,185,25,153},
		{243,115,211,83,251,123,219,91,241,113,209,81,249,121,217,89},
		{15,143,47,175,7,135,39,167,13,141,45,173,5,133,37,165},
		{207,79,239,111,199,71,231,103,205,77,237,109,197,69,229,101},
		{63,191,31,159,55,183,23,151,61,189,29,157,53,181,21,149},
		{255,127,223,95,247,119,215,87,253,125,221,93,245,117,213,85}
	};

/*****************************************************************
 * TAG( make_magic )
     * 
 * Create the magic square.
 * Inputs:
 * 	size:		Order of the square
 *  magic:		Address of 16 x 16 magic square.
 * Outputs:
 * 	Fills in the 16 x 16 magic square.
 * Assumptions:
 * 	size is between 2 and 16
 * Algorithm:
 * 	Chose sub cell of 16 by 16 magic square
     */
make_magic( size, magic )
int size;
int magic[16][16];
{
	int j,i,li,bi,bx,by;
	int xx,yy;
	int total;

	total = size * size;

	i = 0;
	li = -1;
	for(j=0;j<total;j++)	
	{
		bi = 256;

		for(xx=0;xx<size;xx++)
		{
			for(yy=0;yy<size;yy++)
			{
				if(magic16x16[xx][yy] >li && magic16x16[xx][yy] < bi)
				{
					bx = xx;
					by = yy;
					bi = magic16x16[xx][yy];
				}
			}
		}
		magic[bx][by] = i;
		i++;
		li = bi;
	}
}

/*****************************************************************
 * TAG( make_gamma )
 * 
 * Makes a gamma compenstation map.
 * Inputs:
 *  gamma:			desired gamma
 * 	gammamap:		gamma mapping array
 * Outputs:
 *  Changes gamma array entries.
 */
make_gamma( gamma, gammamap )
double gamma;
int gammamap[256];
{
	register int i;

    for ( i = 0; i < 256; i++ )
		{
#ifdef BYTEBUG
		int byteb1;
		
		byteb1 = (int)(0.5 + 255 * pow( i / 255.0, 1.0/gamma ));
		gammamap[i] = byteb1;
#else
		gammamap[i] = (int)(0.5 + 255 * pow( i / 255.0, 1.0/gamma ));
#endif
		}
}

/*****************************************************************
 * TAG( dithergb )
 * 
 * Return dithered RGB value.
 * Inputs:
 * 	x:		X location on screen of this pixel.
 *	y:		Y location on screen of this pixel.
 *	r, g, b:	Color at this pixel (0 - 255 range).
 *	levels:		Number of levels in this map.
 *	divN, modN:	From dithermap.
 *	magic:		Magic square from dithermap.
 * Outputs:
 * 	Returns color map index for dithered pixelv value.
 * Assumptions:
 * 	divN, modN, magic were set up properly.
 * Algorithm:
 * 	see "Note:" in dithermap comment.
 */
dithergb( x, y, r, g, b, levels, divN, modN, magic )
int divN[256];
int modN[256];
int magic[16][16];
{
    int col = x % 16, row = y % 16;

    return DMAP(r, col, row) +
	DMAP(g, col, row) * levels +
	    DMAP(b, col, row) * levels*levels;
}


/*****************************************************************
 * TAG( ditherbw )
 * 
 * Return dithered black & white value.
 * Inputs:
 * 	x:		X location on screen of this pixel.
 *	y:		Y location on screen of this pixel.
 *	val:		Intensity at this pixel (0 - 255 range).
 *	divN, modN:	From dithermap.
 *	magic:		Magic square from dithermap.
 * Outputs:
 * 	Returns color map index for dithered pixel value.
 * Assumptions:
 * 	divN, modN, magic were set up properly.
 * Algorithm:
 * 	see "Note:" in bwdithermap comment.
 */
ditherbw( x, y, val, divN, modN, magic )
int divN[256];
int modN[256];
int magic[16][16];
{
    int col = x % 16, row = y % 16;

    return DMAP(val, col, row);
}
