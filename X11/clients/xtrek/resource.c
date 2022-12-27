/*
 * Copyright 1989 Jon Bennett, Mike Bolotski, David Gagne, Dan Lovinger
 * Copyright 1986 Chris Gutherie
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of the copyright holders not be used in
 * advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission.  The copyright holders make no
 * representations about the suitability of this software for any purpose.
 * It is provided "as is" without express or implied warranty.
 *
 * THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
 * USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#include <X11/Xos.h>
#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <X11/Xutil.h>

#include <stdio.h>
#include <math.h>
#include <signal.h>

#include "defs.h"
#include "data.h"

#define TILESIDE 32

extern XFontStruct *XLoadQueryFont();

/*
 * Note that the open_display function syncronizes the X server when the
 * Debug flag is set.  Code to add the FONTLIBDIR as the fontdir to the users
 * fontpath was taken from /afs/andrew/itc/src/andrew/atk/basics/xim.c at
 * xim__AddAndrewFontPath()
 */

LOCAL void      cond_addto_fontpath(display, element)
  Display        *display;
  char           *element;
{
  char          **fontPathList;		/* Return list of directories */
  long            numPaths;		/* Number of returned directories */
  int             i;			/* Index into font path list */

  fontPathList = XGetFontPath(display, &numPaths);
  for (i = 0; i < numPaths && strcmp(fontPathList[i], element); i++);
  if (i >= numPaths) {
    char          **newPathList;

    if (newPathList = (char **) malloc((numPaths + 1) * sizeof (char *))) {
      for (i = 0; i < numPaths; i++)
	newPathList[i] = fontPathList[i];
      newPathList[i] = element;
      XSetFontPath(display, newPathList, numPaths + 1);
      free(newPathList);
    }
  }
  /* Give back storage for returned font paths */
  if (fontPathList)
    XFreeFontPath(fontPathList);
}


#define GCOPT (GCForeground|GCBackground|GCFont)

LOCAL char      normalfont[] = "6x10";
LOCAL char      boldfont[] = "6x10b";
LOCAL char      italicfont[] = "6x10i";
LOCAL char      bigfont[] = "-bitstream-charter-medium-r-normal-*-240-*";
LOCAL char      xtrekfont[] = FONTNAME;

GLOBAL int      getFonts(p, prog)
  register struct player *p;
  char           *prog;
{
  char           *font_name;
  Window          window;
  XGCValues       gcv;
  Display        *display = p->display;

  cond_addto_fontpath(display, FONTDIR);

  if (!(font_name = XGetDefault(display, prog, "font")))
    font_name = normalfont;
  if (!(p->dfont = XLoadQueryFont(display, font_name))) {
    perror(font_name);
    return 1;
  }
  window = p->w;
  gcv.font = p->dfont->fid;
  gcv.foreground = p->textColor;
  gcv.background = p->backColor;
  p->dfgc = XCreateGC(display, window, GCOPT, &gcv);

  if (!(font_name = XGetDefault(display, PROGRAM_NAME, "boldfont")))
    font_name = boldfont;

  if (!(p->bfont = XLoadQueryFont(display, font_name)))
    p->bfont = p->dfont;

  gcv.font = p->bfont->fid;
  p->bfgc = XCreateGC(display, window, GCOPT, &gcv);

  if (!(font_name = XGetDefault(display, PROGRAM_NAME, "italicFont")))
    font_name = italicfont;

  if (!(p->ifont = XLoadQueryFont(display, font_name)))
    p->ifont = p->dfont;
  gcv.font = p->ifont->fid;
  p->ifgc = XCreateGC(display, window, GCOPT, &gcv);

  if (!(font_name = XGetDefault(display, PROGRAM_NAME, "bigFont")))
    font_name = bigfont;
  if (!(p->bigFont = XLoadQueryFont(display, font_name)))
    p->bigFont = p->dfont;

  gcv.font = p->bigFont->fid;
  p->bFgc = XCreateGC(display, window, GCOPT, &gcv);

  if (!(font_name = XGetDefault(display, PROGRAM_NAME, "xtrekFont")))
    font_name = xtrekfont;
  if (!(p->xfont = XLoadQueryFont(display, font_name)))
    return 1;

  gcv.font = p->xfont->fid;
  p->xfgc = XCreateGC(display, window, GCOPT, &gcv);
  return 0;
}


LOCAL char      solid[TILESIDE] =
{
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

LOCAL char      gray[TILESIDE] =
{
  0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55,
  0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55,
  0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55,
  0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55,
};

LOCAL char      striped[TILESIDE] =
{
  0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff,
  0xf0, 0x0f, 0xf0, 0x0f, 0xf0, 0x0f, 0xf0, 0x0f,
  0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00,
  0x0f, 0xf0, 0x0f, 0xf0, 0x0f, 0xf0, 0x0f, 0xf0,
};

#define stipple_width 16
#define stipple_height 16
LOCAL char      stipple_bits[] =
{
  0xfe, 0xfe, 0xfd, 0xfd, 0xfb, 0xfb, 0xf7, 0xf7,
  0xef, 0xef, 0xdf, 0xdf, 0xbf, 0xbf, 0x7f, 0x7f,
  0xfe, 0xfe, 0xfd, 0xfd, 0xfb, 0xfb, 0xf7, 0xf7,
  0xef, 0xef, 0xdf, 0xdf, 0xbf, 0xbf, 0x7f, 0x7f
};

LOCAL void      getTiles(p, prog)
  register struct player *p;
  char           *prog;
{
  register int    i;
  char            rPatt[TILESIDE], yPatt[TILESIDE], gPatt[TILESIDE];
  int             rSize = sizeof (rPatt) / 2;
  int             ySize = sizeof (yPatt) / 2;
  int             gSize = sizeof (gPatt) / 2;
  XImage         *image;
  GC              gc;
  XGCValues       gcv;
  Display        *display = p->display;

  if (p->mono) {
    Window          window;

    if (p->backColor == XWhitePixel(display, p->screen)) {
      /* Reverse video on, so reverse the bitmaps. */
      for (i = 0; i < TILESIDE; i++) {
	solid[i] = ~solid[i];
	gray[i] = ~gray[i];
	striped[i] = ~striped[i];
      }
    }
    if (arrayDefault(p, "RalertPattern", &rSize, rPatt) < 0) {
      rSize = TILESIDE / 2;
      bcopy(striped, rPatt, sizeof (rPatt));
    }
    if (arrayDefault(p, "YalertPattern", &ySize, yPatt) < 0) {
      ySize = TILESIDE / 2;
      bcopy(gray, yPatt, sizeof (yPatt));
    }
    if (arrayDefault(p, "GalertPattern", &gSize, gPatt) < 0) {
      gSize = TILESIDE / 2;
      bcopy(solid, gPatt, sizeof (gPatt));
    }
    window = p->w;
    p->aTile[(int) Red] =
      XCreateBitmapFromData(display, window, rPatt, rSize, rSize);
    p->aTile[(int) Yellow] =
      XCreateBitmapFromData(display, window, yPatt, ySize, ySize);
    p->aTile[(int) Green] =
      XCreateBitmapFromData(display, window, gPatt, gSize, gSize);
  }
  p->stippleTile = XCreatePixmap(display, p->baseWin,
				 stipple_width, stipple_height, p->depth);
  gcv.foreground = p->backColor;
  gcv.background = p->textColor;
  gc = XCreateGC(display, p->stippleTile, GCForeground | GCBackground, &gcv);
  image = XCreateImage(display, DefaultVisual(display, p->screen),
	  1, XYBitmap, 0, stipple_bits, stipple_width, stipple_height, 8, 0);
  XPutImage(display, p->stippleTile, gc, image, 0, 0, 0, 0,
	    stipple_width, stipple_height);
  XFree(image);
  XFreeGC(display, gc);
}


GLOBAL void     getResources(p, prog)
  register struct player *p;
  char           *prog;
{
  getTiles(p, prog);
  initStats(p, prog);

  p->showShields = booleanDefault(p, prog, "showshields");
  if (booleanDefault(p, prog, "showstats"))
    SETSHOWSTATS(p);
}
