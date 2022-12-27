#ifndef lint
static char rcsid[] = "$XConsortium: StrToCurs.c,v 1.8 89/08/17 15:33:05 jim Exp $";
#endif /* lint */


/***********************************************************
Copyright 1987, 1988 by Digital Equipment Corporation, Maynard, Massachusetts,
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

#include	<X11/IntrinsicP.h>	/* 'cause CoreP.h needs it */
#include	<X11/CoreP.h>		/* just to do XtConvert() */
#include	<X11/StringDefs.h>
#include	<sys/param.h>		/* just to get MAXPATHLEN */
#ifndef MAXPATHLEN
#define MAXPATHLEN 256
#endif
#include	<X11/Xmu/Converters.h>
#include	<X11/Xmu/Drawing.h>


/*
 * XmuConvertStringToCursor:
 *
 * allows String to specify a standard cursor name (from cursorfont.h), a
 * font name and glyph index of the form "FONT fontname index [[font] index]", 
 * or a bitmap file name (absolute, or relative to the global resource
 * bitmapFilePath, class BitmapFilePath).  If the resource is not
 * defined, the default value is the build symbol BITMAPDIR.
 *
 * shares lots of code with XmuCvtStringToPixmap, but unfortunately
 * can't use it as the hotspot info is lost.
 *
 * To use, include the following in your ClassInitialize procedure:

static XtConvertArgRec screenConvertArg[] = {
    {XtBaseOffset, (caddr_t) XtOffset(Widget, core.screen), sizeof(Screen *)}
};

    XtAddConverter("String", "Cursor", XmuCvtStringToCursor,      
		   screenConvertArg, XtNumber(screenConvertArg));
 *
 */

#define	done(address, type) \
	{ (*toVal).size = sizeof(type); (*toVal).addr = (caddr_t) address; }

#ifndef BITMAPDIR
#define BITMAPDIR "/usr/include/X11/bitmaps"
#endif

#define FONTSPECIFIER		"FONT "

/*ARGSUSED*/
void XmuCvtStringToCursor(args, num_args, fromVal, toVal)
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr	fromVal;
    XrmValuePtr	toVal;
{
    static Cursor cursor;		/* static for cvt magic */
    char *name = (char *)fromVal->addr;
    Screen *screen;
    register int i;
    char maskname[MAXPATHLEN];
    Pixmap source, mask;
    static XColor bgColor = {0, ~0, ~0, ~0};  /* XXX - make a resource */
    static XColor fgColor = {0, 0, 0, 0};     /* XXX - ditto */
    int xhot, yhot;
    int len;


    if (*num_args != 1)
     XtErrorMsg("wrongParameters","cvtStringToCursor","XtToolkitError",
             "String to cursor conversion needs screen argument",
              (String *)NULL, (Cardinal *)NULL);

    screen = *((Screen **) args[0].addr);

    if (0 == strncmp(FONTSPECIFIER, name, strlen(FONTSPECIFIER))) {
	char source_name[MAXPATHLEN], mask_name[MAXPATHLEN];
	int source_char, mask_char, fields;
	WidgetRec widgetRec;
	Font source_font, mask_font;
	XrmValue fromString, toFont;

	fields = sscanf(name, "FONT %s %d %s %d",
			source_name, &source_char,
			mask_name, &mask_char);
	if (fields < 2) {
	    XtStringConversionWarning( name, "Cursor" );
	    return;
	}

	/* widgetRec is stupid; we should just use XtDirectConvert,
	 * but the names in Xt/Converters aren't public. */
	widgetRec.core.screen = screen;
	fromString.addr = source_name;
	fromString.size = strlen(source_name);
	XtConvert(&widgetRec, XtRString, &fromString, XtRFont, &toFont);
	if (toFont.addr == NULL) {
	    XtStringConversionWarning( name, "Cursor" );
	    return;
	}
	source_font = *(Font*)toFont.addr;

	switch (fields) {
	  case 2:		/* defaulted mask font & char */
	    mask_font = source_font;
	    mask_char = source_char;
	    break;

	  case 3:		/* defaulted mask font */
	    mask_font = source_font;
	    mask_char = atoi(mask_name);
	    break;

	  case 4:		/* specified mask font & char */
	    fromString.addr = mask_name;
	    fromString.size = strlen(mask_name);
	    XtConvert(&widgetRec, XtRString, &fromString, XtRFont, &toFont);
	    if (toFont.addr == NULL) {
		XtStringConversionWarning( name, "Cursor" );
		return;
	    }
	    mask_font = *(Font*)toFont.addr;
	}

	cursor = XCreateGlyphCursor( DisplayOfScreen(screen), source_font,
				     mask_font, source_char, mask_char,
				     &fgColor, &bgColor );
	done(&cursor, Cursor);
	return;
    }

    i = XmuCursorNameToIndex (name);
    if (i != -1) {
	cursor = XCreateFontCursor (DisplayOfScreen(screen), i);
	done(&cursor, Cursor);
	return;
    }

    if ((source = XmuLocateBitmapFile (screen, name, 
				       maskname, (sizeof maskname) - 4,
				       NULL, NULL, &xhot, &yhot)) == None) {
	XtStringConversionWarning (name, "Cursor");
    }
    len = strlen (maskname);
    for (i = 0; i < 2; i++) {
	strcpy (maskname + len, i == 0 ? "Mask" : "msk");
	if ((mask = XmuLocateBitmapFile (screen, maskname, NULL, 0, 
					 NULL, NULL, NULL, NULL)) != None)
	  break;
    }

    cursor = XCreatePixmapCursor( DisplayOfScreen(screen), source, mask,
				  &fgColor, &bgColor, xhot, yhot );
    XFreePixmap( DisplayOfScreen(screen), source );
    if (mask != None) XFreePixmap( DisplayOfScreen(screen), mask );

    done(&cursor, Cursor);
}
