/*
 * $XConsortium: ttextext.c,v 1.3 89/06/08 09:08:56 jim Exp $
 *
 * Copyright 1989 Massachusetts Institute of Technology
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of M.I.T. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission.  M.I.T. makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * M.I.T. DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL M.I.T.
 * BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN 
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Author:  Jim Fulton, MIT X Consortium
 */

#include <stdio.h>
#include <X11/Xos.h>
#include <X11/Xlib.h>
#include "mapfile.h"

char *ProgramName;
char *displayname = NULL;
char *filename = NULL;
Bool using_temp_file = False;
Bool twobytechars = False;
Display *dpy = NULL;
int error_total = 0, font_total = 0, badfont_total = 0;

#define V_OFF 0
#define V_PER_FONT 1
#define V_BAD 2
#define V_PER_STRING 3
int verbose = V_PER_FONT;


usage ()
{
    fprintf (stderr,
    "usage:  %s [-d dpy] [-2] [-v] [-q] [-file filename] [-fn pattern....]\n",
	     ProgramName);
    exit (1);
}

Exit (n)
    int n;
{
    if (dpy) XCloseDisplay (dpy);
    if (using_temp_file) unlink (filename);
    exit (n);
}

main (argc, argv)
    int argc;
    char **argv;
{
    int i;
    char *cp;

    ProgramName = argv[0];

    for (i = 1; i < argc; i++) {
	char *arg = argv[i];
	if (arg[0] == '-') {
	    switch (arg[1]) {
	      case 'd':
		if (++i >= argc) usage ();
		if (dpy) usage ();
		displayname = argv[i];
		continue;
	      case 'f':
		switch (arg[2]) {
		  case 'i':
		    if (++i >= argc) usage ();
		    if (filename) usage ();
		    filename = argv[i];
		    continue;
		  case 'n':
		    if (++i >= argc) usage ();
		    for (; i < argc; i++) {
			doit (argv[i]);
		    }
		    goto done;
		}
		usage ();
	      case 'v':
		for (cp = arg+1; *cp; cp++) {
		    if (*cp == 'v') verbose++;
		}
		continue;
	      case 'q':
		verbose = V_OFF;
		continue;
	      case '2':
		twobytechars = True;
		continue;
	    }
	}
	usage ();
    }

  done:
    if (dpy) {
	if (verbose >= V_PER_FONT) putchar ('\n');
	printf ("Total:  %d errors, %d of %d fonts gave bad extents\n", 
		error_total, badfont_total, font_total);
    }
    Exit (0);
}

doit (pattern)
    char *pattern;
{
    static char tmpfilename[256];
    static struct mapfile *linelist = NULL;
    static nlines = 0;
    char **fontnames;
    int nfonts;
    static Bool first_time = True;
    int i;

    if (!dpy) {
	dpy = XOpenDisplay (displayname);
	if (!dpy) {
	    fprintf (stderr, "%s:  unable to open display \"%s\"\n",
		     ProgramName, XDisplayName (displayname));
	    exit (1);
	}
    }

    if (!filename) {
	strcpy (tmpfilename, "/tmp/ttext.XXXXXX");
	(void) mktemp (tmpfilename);
	copy_to_file (stdin, tmpfilename);
	filename = tmpfilename;
	using_temp_file = True;
    }

    if (nlines == 0) {
	nlines = mapfilein (filename, &linelist);
	if (nlines < 0) {
	    fprintf (stderr,
		     "%s:  unable to read in test strings from file \"%s\"\n",
		     ProgramName, filename);
	    Exit (1);
	}

	if (nlines == 0) {
	    fprintf (stderr, "%s:  no input in file \"%s\"\n",
		     ProgramName, filename);
	    Exit (1);
	}
    }

    fontnames = XListFonts (dpy, pattern, 10000, &nfonts);
    if (!fontnames) {
	fprintf (stderr, "%s:  no fonts named \"%s\"\n",
		 ProgramName, pattern);
	return;
    }

    if (first_time) {
	if (verbose >= V_PER_FONT) {
	    printf ("Testing %d lines of input from file %s:\n\n",
		    nlines, filename);
	}
	if (verbose >= V_BAD) {
	    printf ("\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n",
		    "dir", "asc", "desc", "Owidth", "Oleft", "Oright", 
		    "Oasc", "Odesc");
	    printf ("\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n",
		    "---", "---", "----", "------", "-----", "------", 
		    "----", "-----");
	} else if (verbose == V_PER_FONT) {
	    printf ("\t%s\t%s\n", "Bad", "Font name");
	    printf ("\t%s\t%s\n", "---", "---------");
	    printf ("\n");
	}
	first_time = False;
    }
    for (i = 0; i < nfonts; i++) {
	int n = test_font (dpy, linelist, nlines, fontnames[i]);
	if (n) badfont_total++;
	error_total += n;
	font_total++;
    }
    XFreeFontNames (fontnames);
    return;
}

copy_to_file (infp, filename)
    FILE *infp;
    char *filename;
{
    FILE *outfp;
    char buf[BUFSIZ*8];
    int n;

    outfp = fopen (filename, "w");
    if (!outfp) {
	fprintf (stderr, "%s:  unable to open output file \"%s\"\n",
		 ProgramName, filename);
	Exit (1);
    }

    while ((n = read (fileno(infp), buf, sizeof buf)) > 0) {
	int nn = write (fileno(outfp), buf, n);
	if (nn != n) {
	    fprintf (stderr, "%s:  only wrote %d/%d bytes to file \"%s\"\n",
		     ProgramName, n, nn, filename);
	    Exit (1);
	}
    }

    fclose (outfp);
    return;
}


struct _finfo {
    int dir, ascent, descent;
    XCharStruct overall;
};

int test_font (dpy, linelist, nlines, fontname)
    Display *dpy;
    struct mapfile *linelist;
    int nlines;
    char *fontname;
{
    int i, j;
    struct _finfo load, query, load16, query16;
    XFontStruct *fs;
    Font fid;
    int bad;
    static XChar2b *twobs = NULL;
    static nbs = 0;

    fs = XLoadQueryFont (dpy, fontname);
    if (!fs) {
	fprintf (stderr, "%s:  unable to load and query font \"%s\"\n",
		 ProgramName, fontname);
	return 1;
    }

    if (verbose >= V_BAD) {
	printf ("font %s\n", fontname);
    }

    for (i = 0, bad = 0; i < nlines; i++, linelist++) {
	int error = 0;
	char *s = linelist->line;
	int len = linelist->len - 1;
	int width, width16;


	if (len < 1) continue;

	width = XTextWidth (fs, s, len);
	XTextExtents (fs, s, len, &load.dir,
		      &load.ascent, &load.descent,
		      &load.overall);
	XQueryTextExtents (dpy, fs->fid, s, len,
			   &query.dir, &query.ascent,
			   &query.descent, &query.overall);
	if ((load.dir != query.dir) ||
	    (load.ascent != query.ascent) ||
	    (load.descent != query.descent) ||
	    (load.overall.lbearing != query.overall.lbearing) ||
	    (load.overall.rbearing != query.overall.rbearing) ||
	    (load.overall.width != query.overall.width) ||
	    (load.overall.ascent != query.overall.ascent) ||
	    (load.overall.descent != query.overall.descent) ||
	    (width != load.overall.width)) {
	    error++;
	    bad++;
	}

	if (twobytechars) {
	    if (len > nbs) {
		if (twobs) free ((char *) twobs);
		nbs = len * 2;
		twobs = (XChar2b *) calloc (nbs, sizeof(XChar2b));
		if (!twobs) {
		    fprintf (stderr,
			     "%s:  unable to calloc %d XChar2b structs\n",
			     ProgramName, nbs);
		    nbs = 0;
		}
	    }
	    for (j = 0; j < len; j++) twobs[j].byte2 = (unsigned char) s[j];

	    width16 = XTextWidth16 (fs, twobs, len);
	    XTextExtents16 (fs, twobs, len, &load16.dir, &load16.ascent,
			    &load16.descent, &load16.overall);
	    XQueryTextExtents16 (dpy, fs->fid, twobs, len,
				 &query16.dir, &query16.ascent,
				 &query16.descent, &query16.overall);
	    if ((load16.dir != query16.dir) ||
		(load16.ascent != query16.ascent) ||
		(load16.descent != query16.descent) ||
		(load16.overall.lbearing != query16.overall.lbearing) ||
		(load16.overall.rbearing != query16.overall.rbearing) ||
		(load16.overall.width != query16.overall.width) ||
		(load16.overall.ascent != query16.overall.ascent) ||
		(load16.overall.descent != query16.overall.descent) ||
		(width16 != load16.overall.width)) {
		error++;
		bad++;
	    }
	    if ((load.dir != load16.dir) ||
		(load.ascent != load16.ascent) ||
		(load.descent != load16.descent) ||
		(load.overall.lbearing != load16.overall.lbearing) ||
		(load.overall.rbearing != load16.overall.rbearing) ||
		(load.overall.width != load16.overall.width) ||
		(load.overall.ascent != load16.overall.ascent) ||
		(load.overall.descent != load16.overall.descent) ||
		(width != width16)) {
		error++;
		bad++;
	    }
	}


	if (verbose >= V_PER_STRING || ((verbose == V_BAD) && error)) {
	    printf ("    (%s) \"", error ? "bad" : "good");
	    fwrite (s, sizeof (char), len, stdout);
	    printf ("\"\n");
	    printf ("\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t(%d)\n",
		    load.dir, load.ascent, load.descent,
		    load.overall.width, 
		    load.overall.lbearing, load.overall.rbearing,
		    load.overall.ascent, load.overall.descent, width);
	    printf ("\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
		    query.dir, query.ascent, query.descent, 
		    query.overall.width, 
		    query.overall.lbearing, query.overall.rbearing,
		    query.overall.ascent, query.overall.descent);
	    if (twobytechars) {
		printf ("\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t(%d)\n",
			load16.dir, load16.ascent, load16.descent,
			load16.overall.width, 
			load16.overall.lbearing, load16.overall.rbearing,
			load16.overall.ascent, load16.overall.descent,
			width16);
		printf ("\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
			query16.dir, query16.ascent, query16.descent, 
			query16.overall.width, 
			query16.overall.lbearing, query16.overall.rbearing,
			query16.overall.ascent, query16.overall.descent);
	    }
	}
    }

    if (verbose == V_PER_FONT) {
	printf ("\t%d\t%s\n", bad, fontname);
    }

    XFreeFont (dpy, fs);
    return bad;
}


