/*
 * $XConsortium: appres.c,v 1.5 89/12/10 15:51:38 rws Exp $
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
#include <X11/Intrinsic.h>

#define NOCLASS "-NoSuchClass-"

char *ProgramName;

static void usage ()
{
    fprintf (stderr, "usage:  %s  [class [instance]]\n", ProgramName);
    exit (1);
}

main (argc, argv)
    int argc;
    char **argv;
{
    Widget toplevel;
    char *iname = NULL, *cname = NULL;
    char tmpbuf[1025];
    char buf[BUFSIZ];
    FILE *fp;
    Bool incont, printit;
    int clen, ilen;

    ProgramName = argv[0];
    if (argc > 1) {
	int newargc = 1;
	char **newargv = (char **) XtCalloc (argc + 1, sizeof(char *));

	if (newargv) {
	    int i;
	    newargv[0] = argv[0];
	    for (i = 1; i < argc; i++) {
		if (argv[i][0] != '-') {
		    if (!cname) {
			cname = argv[i];
			continue;
		    } else if (!iname) {
			iname = argv[i];
			continue;
		    } 
		}
		newargv[newargc++] = argv[i];
	    }
	    if (iname) newargv[0] = iname;
	    argc = newargc;
	    argv = newargv;
	}
    }
    toplevel = XtInitialize (NULL, cname ? cname : NOCLASS, NULL, 0,
			     &argc, argv);
    if (argc != 1) usage ();

    strcpy (tmpbuf, "/tmp/ar.XXXXXX~");
    mktemp (tmpbuf);

    unlink (tmpbuf);
    XrmPutFileDatabase (XtDatabase(XtDisplay(toplevel)), tmpbuf);
    
    fp = fopen (tmpbuf, "r");
    unlink (tmpbuf);
    printit = False;
    incont = False;
    if (!cname) cname = "";
    if (!iname) iname = "";
    clen = strlen (cname);
    ilen = strlen (iname);
    while (fgets (buf, sizeof buf, fp)) {
	int len = strlen (buf);

	if (len < 1) continue;
	if (buf[len - 1] == '\n') {
	    buf[--len] = '\0';
	    if (len == 0) continue;
	}
	if (!incont) {
	    /*
	     * check for 
	     *
	     *     [*.]
	     *     class[*.]
	     *     instance[*.]
	     *
	     * and set printit
	     */
#define match(l,n) (l > 0 && len > l && strncmp (buf, n, l) == 0 && \
		    (buf[l] == '*' || buf[l] == '.'))
	    printit = (buf[0] == '*' || buf[0] == '.' ||
		       match (clen, cname) || match (ilen, iname));
	}
	if (printit) puts (buf);
	incont = (buf[len - 1] == '\\');
	if (!incont) printit = False;
    }

    fclose (fp);
    exit (0);
}



