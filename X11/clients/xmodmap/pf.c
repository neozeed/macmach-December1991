/*
 * xmodmap - program for loading keymap definitions into server
 *
 * $XConsortium: pf.c,v 1.2 88/09/06 17:33:39 jim Exp $
 *
 * Copyright 1988 Massachusetts Institute of Technology
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of M.I.T. not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  M.I.T. makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * Author:  Jim Fulton, MIT X Consortium
 */

#include <X11/Xos.h>
#include <X11/Xlib.h>
#include <stdio.h>
#include <ctype.h>
#include "xmodmap.h"

#define NOTINFILEFILENAME "commandline"
char *inputFilename = NOTINFILEFILENAME;
int lineno = 0;

int process_file (filename)
    char *filename;			/* NULL means use stdin */
{
    FILE *fp;
    char buffer[BUFSIZ];
    int status;

    /* open the file, eventually we'll want to pipe through cpp */

    if (!filename) {
	fp = stdin;
	inputFilename = "stdin"; 
    } else {
	fp = fopen (filename, "r");
	if (!fp) {
	    fprintf (stderr, "%s:  unable to open file '%s' for reading\n",
		     ProgramName, filename);
	    return (-1);
	}
	inputFilename = filename;
    }


    /* read the input and filter */

    if (verbose) {
	printf ("! %s:\n", inputFilename);
    }

    status = 0;
    for (lineno = 0; ; lineno++) {
	buffer[0] = '\0';
	if (fgets (buffer, BUFSIZ, fp) == NULL)
	  break;

	if (process_line (buffer) < 0) 
	  status = -1;
    }

    inputFilename = NOTINFILEFILENAME;
    lineno = 0;
    (void) fclose (fp);

    return (status);
}


int process_line (buffer)
    char *buffer;
{
    int len;
    int i;
    char *cp;

    len = strlen (buffer);

    for (i = 0; i < len; i++) {		/* look for blank lines */
	register char c = buffer[i];
	if (!(isspace(c) || c == '\n')) break;
    }
    if (i == len) return (0);

    cp = &buffer[i];

    if (*cp == '!') return (0);		/* look for comments */
    len -= (cp - buffer);		/* adjust len by how much we skipped */

					/* pipe through cpp */

					/* strip trailing space */
    for (i = len-1; i >= 0; i--) {
	register char c = cp[i];
	if (!(isspace(c) || c == '\n')) break;
    }
    if (i >= 0) cp[len = (i+1)] = '\0';  /* nul terminate */

    if (verbose) {
	printf ("! %d:  %s\n", lineno, cp);
    }

    /* handle input */
    return (handle_line (cp, len));
}

