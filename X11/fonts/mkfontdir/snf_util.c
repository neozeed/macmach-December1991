/***********************************************************
Copyright 1988 by Digital Equipment Corporation, Maynard, Massachusetts,
and the Massachusetts Institute of Technology, Cambridge, Massachusetts.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the names of Digital or MIT not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  

DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/

/* $XConsortium: snf_util.c,v 1.7 88/10/10 19:34:56 rws Exp $ */

#include <stdio.h>
#include <X11/Xos.h>
#include <sys/stat.h>

#include <X11/X.h>
#include <X11/Xproto.h>
#include <X11/Xmd.h>

#include "fontstruct.h"
#include "snfstruct.h"
#include "bdftosnf.h"

#include "fontdir.h"

void GetSNFFontName( fp, font_name)
    FILE *fp;
    char *font_name;
{
    FontInfoRec	fi;
    FontPropPtr	pffp;
    char	*propspace;
    int		bytestoread;
    int		i;
    char	*strings;
    struct stat	stats;
    int		c;
	
    bytestoread = sizeof(FontInfoRec);
    if ( fread((char *)&fi, 1, bytestoread, fp) != bytestoread) {
	font_name[0] = NUL;
        return;
    }

    if (fi.version1 != FONT_FILE_VERSION || fi.version2 != FONT_FILE_VERSION) {
	font_name[0] = NUL;
	return;
    }

    /* skip what we find unintersting */
    i = BYTESOFCHARINFO(&fi) + BYTESOFGLYPHINFO(&fi);
    fstat(fileno(fp), &stats);
    if ((stats.st_mode & S_IFMT) == S_IFREG)
	fseek(fp, i, 1);
    else
	while (i-- && ((c = getc(fp)) != EOF))
	    ;

    /* now read properties */
    bytestoread = BYTESOFPROPINFO(&fi) + BYTESOFSTRINGINFO(&fi);
    propspace = (char *) malloc(bytestoread);

    if ( fread(propspace, 1,	bytestoread, fp) != bytestoread) {
	free(propspace);
	font_name[0] = NUL;
        return;
    }

    pffp = (FontPropPtr)propspace;
    strings = propspace + BYTESOFPROPINFO(&fi);
    for (i = 0; i < fi.nProps; i++, pffp++) {
	if (strcmp(&strings[pffp->name], "FONT") == 0) {
	    if (pffp->indirect)	/* right type for name */
		strcpy(font_name, &strings[pffp->value]);
	    break;
	}
    }
    free(propspace);
}

