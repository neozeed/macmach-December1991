/* window.c:
 *
 * display an image in a window
 *
 * jim frost 10.03.89
 *
 * Copyright 1989, 1990 Jim Frost.  See included file "copyright.h" for
 * complete copyright information.
 */

#include "copyright.h"
#include "xloadimage.h"
#include <ctype.h>
#include <X11/cursorfont.h>

static Window ImageWindow= 0;

static void setCursor(disp, window, iw, ih, ww, wh, cursor)
     Display      *disp;
     Window        window;
     unsigned int  iw, ih;
     unsigned int  ww, wh;
     Cursor       *cursor;
{ XSetWindowAttributes swa;

  if ((ww >= iw) && (wh >= ih))
    swa.cursor= XCreateFontCursor(disp, XC_icon);
  else if ((ww < iw) && (wh >= ih))
    swa.cursor= XCreateFontCursor(disp, XC_sb_h_double_arrow);
  else if ((ww >= iw) && (wh < ih))
    swa.cursor= XCreateFontCursor(disp, XC_sb_v_double_arrow);
  else
    swa.cursor= XCreateFontCursor(disp, XC_fleur);
  XChangeWindowAttributes(disp, window, CWCursor, &swa);
  XFreeCursor(disp, *cursor);
  *cursor= swa.cursor;
}

/* place an image
 */

static void placeImage(width, height, winwidth, winheight, rx, ry)
     int width, height, winwidth, winheight;
     int *rx, *ry; /* supplied and returned */
{ int pixx, pixy;

  pixx= *rx;
  pixy= *ry;

  if (winwidth > width)
    pixx= (winwidth - width) / 2;
  else {
    if ((pixx < 0) && (pixx + width < winwidth))
      pixx= winwidth - width;
    if (pixx > 0)
      pixx= 0;
  }
  if (winheight > height)
    pixy= (winheight - height) / 2;
  else {
    if ((pixy < 0) && (pixy + height < winheight))
      pixy= winheight - height;
    if (pixy > 0)
      pixy= 0;
  }
  *rx= pixx;
  *ry= pixy;
}

/* blit an image
 */

static void blitImage(disp, pixmap, window, gc, pixx, pixy, width, height,
		      winwidth, winheight, x, y, w, h)
     Display *disp;
     Pixmap   pixmap;
     Window   window;
     GC       gc;
     int      pixx, pixy, width, height, winwidth, winheight, x, y, w, h;
{
  if (x + w > winwidth)
    w= winwidth - x;
  if (y + h > winheight)
    h= winheight - y;
  if (x < pixx) {
    XClearArea(disp, window, x, y, pixx - x, y + h, False);
    w -= (pixx - x);
    x= pixx;
  }
  if (y < pixy) {
    XClearArea(disp, window, x, y, w, pixy - y, False);
    h -= (pixy - y);
    y= pixy;
  }
  if (x + w > pixx + width) {
    XClearArea(disp, window, pixx + width, y, w - width, h, False);
    w= width;
  }
  if (y + h > pixy + height) {
    XClearArea(disp, window, x, pixy + height, w, h - height, False);
    h= height;
  }
  XCopyArea(disp, pixmap, ImageWindow, gc, x - pixx, y - pixy, w, h,
	    x, y);
}

/* clean up static window if we're through with it
 */

void cleanUpWindow(disp)
     Display *disp;
{
  if (ImageWindow)
    XDestroyWindow(disp, ImageWindow);
  ImageWindow= 0;
}

/* this attempts to convert an image title into a reasonable icon name
 */

static char *iconName(s)
     char *s;
{ static char buf[BUFSIZ];
  char *t;

  buf[BUFSIZ - 1]= '\0';
  strncpy(buf, s, BUFSIZ - 1);
  t= index(buf, ' '); /* strip off stuff following 1st word.  this strips */
  if (t)              /* info added by processing functions too. */
    *t= '\0';

  /* strip off leading path.  if you don't use unix-style paths, you might
   * want to change this.
   */

  if (t= rindex(buf, '/')) {
    for (s= buf, t++; *t; s++, t++)
      *s= *t;
    *s= '\0';
  }
  t= index(buf, '.'); /* look for an extension and strip it off */
  if (t)
    *t= '\0';
  return(buf);
}

char imageInWindow(disp, scrn, image, user_geometry,
		   fullscreen, install, private_cmap, slideshow,
		   argc, argv, verbose)
     Display      *disp;
     int           scrn;
     Image        *image;
     char         *user_geometry;
     unsigned int  fullscreen;
     unsigned int  install;
     unsigned int  private_cmap;
     unsigned int  slideshow;
     int           argc;
     char         *argv[];
     unsigned int  verbose;
{ Pixmap               pixmap;
  Colormap             xcmap;
  XSetWindowAttributes swa;
  XSizeHints           sh;
  XWMHints             wmh;
  XGCValues            gcv;
  GC                   gc;
  int                  pixx, pixy;
  int                  lastx, lasty, mousex, mousey;
  int                  paint;
  union {
    XEvent              event;
    XAnyEvent           any;
    XButtonEvent        button;
    XKeyEvent           key;
    XConfigureEvent     configure;
    XExposeEvent        expose;
    XMotionEvent        motion;
    XResizeRequestEvent resize;
  } event;
  unsigned int          winx, winy, winwidth, winheight;

  /* figure out the window size.  unless specifically requested to do so,
   * we will not exceed 90% of display real estate.
   */

  if (user_geometry == NULL) {
    winx= winy= winwidth= winheight= 0;
  }
  else {
    char                def_geom[30];

    sprintf(def_geom, "%ux%u+0+0", image->width, image->height);
    XGeometry(disp, scrn, user_geometry, def_geom, 0, 1, 1, 0, 0,
	      &winx, &winy, &winwidth, &winheight);
  }

  if (fullscreen) {
    winwidth= DisplayWidth(disp, scrn);
    winheight= DisplayHeight(disp, scrn);
  }
  else {
    lastx= (winwidth || winheight); /* user set size flag */
    if (!winwidth) {
      winwidth= image->width;
      if (winwidth > DisplayWidth(disp, scrn) * 0.9)
	winwidth= DisplayWidth(disp, scrn) * 0.9;
    }
    if (!winheight) {
      winheight= image->height;
      if (winheight > DisplayHeight(disp, scrn) * 0.9)
	winheight= DisplayHeight(disp, scrn) * 0.9;
    }
  }

  if (! sendImageToX(disp, scrn, DefaultVisual(disp, scrn),
		     image, &pixmap, &xcmap, private_cmap, verbose))
    exit(1);

  swa.background_pixel= BlackPixel(disp,scrn);
  swa.backing_store= NotUseful;
  swa.bit_gravity= NorthWestGravity;
  swa.cursor= XCreateFontCursor(disp, XC_watch);
  swa.colormap= xcmap;
  swa.event_mask= ButtonPressMask | Button1MotionMask | KeyPressMask |
    ExposureMask | StructureNotifyMask | EnterWindowMask | LeaveWindowMask;
  swa.save_under= False;
  swa.override_redirect= (fullscreen ? True : False);
  if (!ImageWindow) {
    static XClassHint classHint;
    ImageWindow= XCreateWindow(disp, RootWindow(disp, scrn), winx, winy,
			       winwidth, winheight, 0,
			       DefaultDepth(disp, scrn),
			       InputOutput, CopyFromParent,
			       CWBackPixel | CWBackingStore |
			       CWBitGravity | CWCursor | CWColormap |
			       CWEventMask | CWSaveUnder |
			       CWOverrideRedirect, &swa);
    XSetCommand(disp, ImageWindow, argv, argc);
    classHint.res_class = "Xloadimage";
    classHint.res_name=NULL;
    (void) XSetClassHint(disp,ImageWindow,&classHint);
    paint= 0;
  }
  else
    paint= 1;

  XStoreName(disp, ImageWindow, image->title);
  XSetIconName(disp, ImageWindow, iconName(image->title));

  sh.width= winwidth;
  sh.height= winheight;
  if (fullscreen) {
    sh.min_width= sh.max_width= winwidth;
    sh.min_height= sh.max_height= winheight;
  }
  else {
    sh.min_width= 1;
    sh.min_height= 1;
    sh.max_width= image->width;
    sh.max_height= image->height;
  }
  sh.width_inc= 1;
  sh.height_inc= 1;
  sh.flags= PMinSize | PMaxSize | PResizeInc;
  if (lastx || fullscreen)
    sh.flags |= USSize;
  else
    sh.flags |= PSize;
  if (fullscreen) {
    sh.x= sh.y= 0;
    sh.flags |= USPosition;
  }
  else if (winx || winy) {
    sh.x= winx;
    sh.y= winy;
    sh.flags |= USPosition;
  }
  XSetNormalHints(disp, ImageWindow, &sh);

  wmh.input= True;
  wmh.flags= InputHint;
  XSetWMHints(disp, ImageWindow, &wmh);

  gcv.function= GXcopy;
  gcv.foreground= 0;
  gc= XCreateGC(disp, ImageWindow, GCFunction | GCForeground, &gcv);
  XMapWindow(disp, ImageWindow);
  placeImage(image->width, image->height, winwidth, winheight, &pixx, &pixy);
  if (paint) {
#if 0
    /* this will happen on ConfigureNotify; we avoid it here to keep from
     * doing a double paint.
     */

    blitImage(disp, pixmap, ImageWindow, gc,
	      pixx, pixy, image->width, image->height, winwidth, winheight,
	      0, 0, winwidth, winheight);
#endif
    XResizeWindow(disp, ImageWindow, winwidth, winheight);
    XChangeWindowAttributes(disp, ImageWindow, CWColormap, &swa);
  }

  setCursor(disp, ImageWindow, image->width, image->height,
	    winwidth, winheight, &(swa.cursor));

  lastx= lasty= -1;
  for (;;) {
    XNextEvent(disp, &event);
    switch (event.any.type) {
    case ButtonPress:
      if (event.button.button == 1) {
	lastx= event.button.x;
	lasty= event.button.y;
	break;
      }
      break;

    case KeyPress: {
      char buf[128];
      KeySym ks;
      XComposeStatus status;
      char ret;
      Cursor cursor;

      if (XLookupString(&event.key,buf,128,&ks,&status) != 1)
	break;
      ret= buf[0];
      if (isupper(ret))
	ret= tolower(ret);
      switch (ret) {
      case ' ':
      case 'n':
      case 'p':
	cursor= swa.cursor;
	swa.cursor= XCreateFontCursor(disp, XC_watch);
	XChangeWindowAttributes(disp, ImageWindow, CWCursor, &swa);
	XFreeCursor(disp, cursor);
	XFlush(disp);
	/* FALLTHRU */
      case '\003': /* ^C */
      case 'q':
	XFreeCursor(disp, swa.cursor);
	XFreePixmap(disp, pixmap);

	/* XCopyColormapAndFree accomplishes two things.  First, it frees up
	 * all our colors in the default colormap.  second, on some displays
	 * it will help cut down on technicolor.  i tried to duplicate the
	 * current colormap exactly but some servers return BadValue when
	 * trying XQueryColor on an unallocated colormap entry so I gave up.
	 */

	if (xcmap != DefaultColormap(disp, scrn))
	    XFreeColormap(disp, xcmap);
	else if (slideshow)
	  XFreeColormap(disp, XCopyColormapAndFree(disp, xcmap));

	return(ret);
      }
      break;
    }

    case MotionNotify:
      if ((image->width <= winwidth) && (image->height <= winheight))
	break; /* we're AT&T */
      mousex= event.button.x;
      mousey= event.button.y;
      while (XCheckTypedEvent(disp, MotionNotify, &event) == True) {
	mousex= event.button.x;
	mousey= event.button.y;
      }
      pixx -= (lastx - mousex);
      pixy -= (lasty - mousey);
      lastx= mousex;
      lasty= mousey;
      placeImage(image->width, image->height, winwidth, winheight,
		 &pixx, &pixy);
      blitImage(disp, pixmap, ImageWindow, gc,
		pixx, pixy, image->width, image->height, winwidth, winheight,
		0, 0, winwidth, winheight);
      break;

    case ConfigureNotify:
      winwidth= event.configure.width;
      winheight= event.configure.height;

      placeImage(image->width, image->height, winwidth, winheight,
		 &pixx, &pixy);

      /* configure the cursor to indicate which directions we can drag
       */

      setCursor(disp, ImageWindow, image->width, image->height,
		winwidth, winheight, &(swa.cursor));

      /* repaint if we just changed images
       */

      if (paint)
	blitImage(disp, pixmap, ImageWindow, gc,
		  pixx, pixy, image->width, image->height, winwidth, winheight,
		  0, 0, winwidth, winheight);
      break;

    case DestroyNotify:
      XFreeCursor(disp, swa.cursor);
      XFreePixmap(disp, pixmap);
      if (xcmap != DefaultColormap(disp, scrn))
	XFreeColormap(disp, xcmap);
      return('\0');

    case Expose:
      blitImage(disp, pixmap, ImageWindow, gc,
		pixx, pixy, image->width, image->height, winwidth, winheight,
		event.expose.x, event.expose.y,
		event.expose.width, event.expose.height);
      break;

    case EnterNotify:
      if (install)
	XInstallColormap(disp, xcmap);
      break;

    case LeaveNotify:
      if (install)
	XUninstallColormap(disp, xcmap);
    }
  }
}
