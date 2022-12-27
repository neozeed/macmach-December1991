/*
 * xmag - simple magnifying glass application
 *
 * $XConsortium: xmag.c,v 1.29 89/12/10 17:23:58 rws Exp $
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
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <X11/Xatom.h>
#include <stdio.h>

#define DEFAULT_BORDER_SIZE 2
#define DEFAULT_CURSOR_SIZE 64
#define DEFAULT_MAGNIFICATION 5
#define DEFAULT_PIX_FONT "fixed"

int Argc;				/* copy of argc */
char **Argv;				/* copy of argv */

char *ProgramName;			/* name of program being executed */
Display *dpy = NULL;			/* X server being used */
int screen;				/* screen being used */
int pixmap_format;			/* XYPixmap or ZPixmap */
Colormap cmap;				/* colormap to use for drawing */
int ncolormapentries;			/* size of colormap being used */
Window w;				/* enlargement window */
Window root;				/* root window of screen */
GC fillGC = (GC) NULL;			/* for drawing enlargement */
Bool do_grab = False;			/* grab server */
char *pix_font = NULL;			/* for displaying pixel value */

void magnify(), get_source(), repaint_image();
Bool do_magnify();


void Exit (status)
    int status;
{
    if (dpy) {
	XCloseDisplay (dpy);
	dpy = NULL;
    }
    (void) exit (status);
}

Usage ()
{
    static char *help_message[] = {
"where options include:",
"    -display displaystring        X server to contact",
"    -geometry geomstring          size and location of enlargement",
"    -source geomstring            size and location of area to be enlarged",
"    -mag magfactor                integral amount by which area is enlarged",
"    -bw pixels                    border width in pixels",
"    -bd color                     border color",
"    -bg colororpixelvalue         background color or %pixel number",
"    -fn fontname                  name of font for examining enlarged pixels",
"    -z                            grab server during dynamics and XGetImage",
"",
"Options may be abbreviated to the shortest unambiguous prefix.",
"",
NULL
    }, **cpp;

    fprintf (stderr, "usage:\n        %s [-options ...]\n\n", ProgramName);
    for (cpp = help_message; *cpp; cpp++) {
	fprintf (stderr, "%s\n", *cpp);
    }
    Exit (1);
}


main (argc, argv)
    int argc;
    char **argv;
{
    int i;				/* iterator variable */
    char *displayname = NULL;		/* name of X server to contact */
    char *source_geometry = NULL;	/* size of square tracking pointer */
    char *geometry = NULL;		/* size and location of enlarged */
    int magnification = 0;		/* enlarged image will be m by m */
    char *border_color = NULL;		/* border color name */
    char *back_color = NULL;		/* background of enlargement */
    int border_width = -1;		/* border width in pixels */

    ProgramName = argv[0];
    Argc = argc;
    Argv = argv;

    for (i = 1; i < argc; i++) {
	char *arg = argv[i];

	if (arg[0] == '-') {		/* process command line switches */
	    switch (arg[1]) {
	      case 'd':			/* -display displaystring */
		if (++i >= argc) Usage ();
		displayname = argv[i];
		continue;
	      case 'g':			/* -geometry geomstring */
		if (++i >= argc) Usage ();
		geometry = argv[i];
		continue;
	      case 's':			/* -source geomstring */
		if (++i >= argc) Usage ();
		source_geometry = argv[i];
		continue;
	      case 'm':			/* -mag magnification */
		if (++i >= argc) Usage ();
		magnification = atoi (argv[i]);
		continue;
	      case 'b':
		switch (arg[2]) {
		  case 'w':		/* -bw pixels */
		    if (++i >= argc) Usage ();
		    border_width = atoi (argv[i]);
		    continue;
		  case 'd':		/* -bd color */
		    if (++i >= argc) Usage ();
		    border_color = argv[i];
		    continue;
		  case 'g':		/* -bg colororpixel*/
		    if (++i >= argc) Usage ();
		    back_color = argv[i];
		    continue;
		}			/* end switch */
		Usage ();
		/* NOTREACHED */
	      case 'z':			/* -z */
		do_grab = True;
		continue;
	      case 'f':			/* -font fontname */
		if (++i >= argc) Usage ();
		pix_font = argv[i];
		continue;
	      default:
		Usage ();
		/* NOTREACHED */
	    }				/* end switch */
	} else {			/* any command line parameters */
	    Usage ();
	    /* NOTREACHED */
	}				/* end if else */
    }					/* end for */

    dpy = XOpenDisplay (displayname);
    if (!dpy) {
	fprintf (stderr, "%s:  unable to open display \"%s\".\n",
		 ProgramName, XDisplayName (displayname));
	Exit (1);
    }

    /*
     * get defaults (would be nice to use the resource manager instead)
     */

    if (!pix_font) pix_font = XGetDefault (dpy, ProgramName, "font");
    if (!pix_font) pix_font = XGetDefault (dpy, ProgramName, "Font");

    if (!geometry) geometry = XGetDefault (dpy, ProgramName, "geometry");
    if (!geometry) geometry = XGetDefault (dpy, ProgramName, "Geometry");

    if (!source_geometry)
      source_geometry = XGetDefault (dpy, ProgramName, "source");
    if (!source_geometry)
      source_geometry = XGetDefault (dpy, ProgramName, "Source");

    if (magnification < 1) {
	char *tmp = XGetDefault (dpy, ProgramName, "magnification");
	if (!tmp) tmp = XGetDefault (dpy, ProgramName, "Magnification");

	if (tmp) magnification = atoi (tmp);
    }

    if (border_width < 0) {
	char *tmp = XGetDefault (dpy, ProgramName, "borderWidth");
	if (!tmp) tmp = XGetDefault (dpy, ProgramName, "BorderWidth");

	border_width = tmp ? atoi (tmp) : DEFAULT_BORDER_SIZE;
    }

    if (!border_color) 
      border_color = XGetDefault (dpy, ProgramName, "borderColor");
    if (!border_color) 
      border_color = XGetDefault (dpy, ProgramName, "BorderColor");

    if (!back_color)
      back_color = XGetDefault (dpy, ProgramName, "background");
    if (!back_color) 
      back_color = XGetDefault (dpy, ProgramName, "Background");

    /*
     * set up globals
     */

    screen = DefaultScreen (dpy);
    cmap = DefaultColormap (dpy, screen);
    ncolormapentries = DisplayCells (dpy, screen);
    root = RootWindow (dpy, screen);
    pixmap_format = (DisplayPlanes (dpy, screen) > 1 ? ZPixmap : XYPixmap);

    /*
     * do the work
     */

    magnify (geometry, source_geometry, magnification,
	     border_color, border_width, back_color);

    Exit (0);
}


/*
 * Do the setup for the magnification.  Use the geometry, source_geometry,
 * and magnification to figure out how big of an area to magnify, and by
 * how much it should be enlarged according to the following relations:
 *
 *         magnification = window.geometry.width / source_geometry.width 
 *         magnification = window.geometry.height / source_geometry.height
 *
 * Thus, only two of the three values may be specified by the user.  The
 * following values are used to indicate that the default should be taken:
 *
 *        Variable         blank value    default value
 *
 *        magnification        0                5
 *        window             NULL       "320x320-0-0"
 *        source               NULL          "64x64"
 *
 * If a location is not specified for the source, a square centered on the 
 * pointer will be displayed that can be dragged around the screen.
 */

void magnify (window_geometry, source_geometry, magnification, 
	 border_color, border_width, back_color)
    char *window_geometry;		/* size and location of enlargement */
    char *source_geometry;		/* size and location of area */
    int magnification;			/* enlargement ratio */
    char *border_color;			/* name of color to use for border */
    int border_width;			/* width of border */
    char *back_color;			/* color name or pixel value */
{
    XTextProperty windowName, iconName;
    XWMHints wmHints;
    XClassHint classHints;
    XSizeHints source_hints, window_hints;  /* for sizes and locations */
    int source_geom_mask, window_geom_mask;  /* for XParseGeometry results */
    int params;				/* for which arguments present */
    unsigned long border_pixel;		/* pixel value for border */
    int tmp;				/* work variable */
    XColor cdef;			/* for parsing border color name */
    unsigned long valuemask;		/* for window attributes */
    XSetWindowAttributes attr;		/* for creating window */
    XGCValues gcv;			/* for creating GC */
    unsigned long back_pixel;		/* background of enlargement */

    bzero ((char *) &source_hints, sizeof source_hints);
    bzero ((char *) &window_hints, sizeof window_hints);
    source_geom_mask = window_geom_mask = 0;

#define SizeValues (WidthValue | HeightValue)
#define valNothing 0
#define valMagnification 1
#define valWindow 2
#define valSource 4
#define valAll 7

    params = valNothing;

    if (magnification > 0) params |= valMagnification;

    if (window_geometry) {
	window_geom_mask = XParseGeometry (window_geometry,
					   &window_hints.x, &window_hints.y, 
					   (unsigned int *)&window_hints.width,
					   (unsigned int *)&window_hints.height);
	tmp = (window_geom_mask & SizeValues);
	if (tmp) {
	    if (tmp == SizeValues)
	      params |= valWindow;
	    else
	      window_geom_mask &= ~SizeValues;
	}
    }

    if (source_geometry) {
	source_geom_mask = XParseGeometry (source_geometry,
					   &source_hints.x, &source_hints.y, 
					   (unsigned int *)&source_hints.width,
					   (unsigned int *)&source_hints.height);
	tmp = (source_geom_mask & SizeValues);
	if (tmp) {
	    if (tmp == SizeValues)
	      params |= valSource;
	    else
	      source_geom_mask &= ~SizeValues;
	}
    }

    /*
     * Check to make sure that user didn't give -geometry, -source, and -mag
     */

    if (params == valAll) {
	fprintf (stderr, "%s:  magnifier size over specified, ignoring -mag\n",
		 ProgramName);
	magnification = 0;
	params &= ~valMagnification;
    }

    /*
     * Verify the input and compute the appropriate sizes
     */

    if ((params & valMagnification) && magnification <= 0) {
	fprintf (stderr, "%s:  bad magnification %d.\n",
		 ProgramName, magnification);
	Exit (1);
    }
    if ((params & valWindow) && (window_hints.width <= 0 ||
				 window_hints.height <= 0)) {
	fprintf (stderr, "%s:  bad window size %dx%d.\n",
		 ProgramName, window_hints.width, window_hints.height);
	Exit (1);
    }
    if ((params & valSource) && (source_hints.width <= 0 ||
				 source_hints.height <= 0)) {
	fprintf (stderr, "%s:  bad source size %dx%d.\n",
		 ProgramName, source_hints.width, source_hints.height);
	Exit (1);
    }

    switch (params) {
      case valMagnification:
	source_hints.width = source_hints.height = DEFAULT_CURSOR_SIZE;
	goto mag_and_cur;

      case valWindow:
	magnification = DEFAULT_MAGNIFICATION;
	/* fall through */
      case valMagnification|valWindow:
	source_hints.width = window_hints.width / magnification;
	source_hints.height = window_hints.height / magnification;
	break;

      case valNothing:
	source_hints.width = source_hints.height = DEFAULT_CURSOR_SIZE;
	/* fall through */
      case valSource:
	magnification = DEFAULT_MAGNIFICATION;
	/* fall through */
      case valMagnification|valSource:
      mag_and_cur:
	window_hints.width = source_hints.width * magnification;
	window_hints.height = source_hints.height * magnification;
	break;

      case valWindow|valSource:
	magnification = window_hints.width / source_hints.width;
	tmp = window_hints.height / source_hints.height;
	if (tmp < magnification) magnification = tmp;
	break;
    }

    /*
     * At this point, both hints structures contain the appropriate width
     * and height fields.  Now we need to do the placement calculations.
     */

    if ((window_geom_mask & XValue) && (window_geom_mask & XNegative))
      window_hints.x = DisplayWidth (dpy, screen) + window_hints.x - 
	window_hints.width - border_width * 2;

    if ((window_geom_mask & YValue) && (window_geom_mask & YNegative))
      window_hints.y = DisplayHeight (dpy, screen) + window_hints.y -
	window_hints.height - border_width * 2;

    if ((source_geom_mask & XValue) && (source_geom_mask & XNegative))
      source_hints.x = DisplayWidth (dpy, screen) + source_hints.x - 
	source_hints.width;

    if ((source_geom_mask & YValue) && (source_geom_mask & YNegative))
      source_hints.y = DisplayHeight (dpy, screen) + source_hints.y -
	source_hints.height;

    /*
     * Set the flags entries so that we know what we have 
     */

    source_hints.flags = 0;
    source_hints.flags |= USSize;
    if ((source_geom_mask & XValue) && (source_geom_mask & YValue)) 
      source_hints.flags |= USPosition;

    window_hints.flags = 0;
    window_hints.flags |= USSize;
    if ((window_geom_mask & XValue) && (window_geom_mask & YValue)) 
      window_hints.flags |= USPosition;

    if (border_color && XParseColor (dpy, cmap, border_color, &cdef) &&
	XAllocColor (dpy, cmap, &cdef)) {
	border_pixel = cdef.pixel;
    } else {
	border_pixel = BlackPixel (dpy, screen);
    }

    window_hints.flags |= (PResizeInc | PBaseSize | PWinGravity);
    window_hints.width_inc = window_hints.height_inc = magnification;
    window_hints.base_width = window_hints.base_height = 0;
    switch (window_geom_mask & (XNegative | YNegative)) {
      case 0:
        window_hints.win_gravity = NorthWestGravity;
	break;
      case XNegative:
        window_hints.win_gravity = NorthEastGravity;
	break;
      case YNegative:
        window_hints.win_gravity = SouthWestGravity;
	break;
      default:
        window_hints.win_gravity = SouthEastGravity;
	break;
    }

    back_pixel = BlackPixel (dpy, screen);
    if (back_color) {
	XColor cdef;

	if (back_color[0] == '%') {	/* pixel specifier */
	    char *fmt = "%lu";
	    register char *s = back_color + 1;

	    if (!*s) Usage ();
	    if (*s == '0') {
		s++;
		fmt = "%lo";
	    }
	    if (*s == 'x' || *s == 'X') {
		s++;
		fmt = "%lx";
	    }
	    if (sscanf (s, fmt, &back_pixel) != 1) Usage ();
	} else if (XParseColor (dpy, cmap, back_color, &cdef) &&
		   XAllocColor (dpy, cmap, &cdef)) {
	    back_pixel = cdef.pixel;
	}
    }

    /*
     * Make the enlargment window, but don't map it
     */

    attr.background_pixel = back_pixel;
    attr.border_pixel = border_pixel;
    attr.event_mask = (ExposureMask|ButtonPressMask|ButtonReleaseMask|KeyPressMask);
    attr.cursor = XCreateFontCursor (dpy, XC_top_left_arrow);
    valuemask = (CWBackPixel | CWBorderPixel | CWEventMask | CWCursor);

    w = XCreateWindow (dpy, root, 
		       window_hints.x, window_hints.y, 
		       window_hints.width, window_hints.height,
		       border_width, DefaultDepth (dpy, screen),
		       CopyFromParent, CopyFromParent, valuemask, &attr);

    windowName.encoding = XA_STRING;
    windowName.format = 8;
    windowName.value = (unsigned char *) "Magnifying Glass";
    windowName.nitems = strlen (windowName.value);
    iconName.encoding = XA_STRING;
    iconName.format = 8;
    iconName.value = (unsigned char *) "xmag";
    iconName.nitems = strlen (iconName.value);
    wmHints.input = True;
    wmHints.flags = InputHint;
    classHints.res_name = NULL;
    classHints.res_class = "Magnifier";
    XSetWMProperties (dpy, w, &windowName, &iconName, Argv, Argc,
		      &window_hints, &wmHints, &classHints);

    /*
     * Make up a dummy GC
     */
    gcv.function = GXcopy;
    gcv.plane_mask = AllPlanes;
    gcv.fill_style = FillSolid;
    /* repaint_image fills in foreground */

    fillGC = XCreateGC (dpy, w, (GCFunction | GCPlaneMask | GCFillStyle),
			&gcv);

    /*
     * Do the work.  If the user specified the location of the source,
     * then just that one spot, otherwise do some dynamics.
     */

    if (source_hints.flags & USPosition) {
	if (do_grab) {
	    XGrabServer (dpy);		/* ungrab is in do_magnify */
	    XSync (dpy, 0);
	}
	(void) do_magnify (&source_hints, &window_hints, magnification,
			   back_pixel);
    } else {
	do {
	    if (do_grab) {
		XGrabServer (dpy);	/* ungrab is in do_magnify */
	    }
	    XSync (dpy, 1);
	    get_source (&source_hints);
	} while (do_magnify (&source_hints, &window_hints, magnification,
			     back_pixel));
    }

    return;
}

#define NITERATIONS 6
#define NPOINTS (1 + (NITERATIONS * 2 * 4))  /* 1 move, 2 inverts, 4 sides */

void get_source (shp)
    XSizeHints *shp;
{
    register int x, y;
    int width = shp->width, height = shp->height;
    int dw = DisplayWidth (dpy, screen), dh = DisplayHeight (dpy, screen);
    Window root_window, child_window;
    int root_x, root_y, win_x, win_y;
    int xoff = width / 2, yoff = height / 2;
    XPoint box[NPOINTS];
    int lastx, lasty;
    unsigned int mask;
    Bool done;
    static GC invGC = (GC) NULL;	/* for drawing source region outline */
    static Cursor invCursor;

    if (!invGC) {			/* make it the first time */
	XGCValues gcv;

	gcv.function = GXxor;
	gcv.subwindow_mode = IncludeInferiors;
	gcv.foreground = (unsigned long) 1L;

	invGC = XCreateGC (dpy, root, (GCFunction | GCSubwindowMode |
				       GCForeground), &gcv);
	if (!invGC) {
	    fprintf (stderr, "%s:  unable to create invert GC.\n",
		     ProgramName);
	    Exit (1);
	}
	invCursor = XCreateFontCursor (dpy, XC_crosshair);
    }


    if (XGrabPointer (dpy, root, False, ButtonReleaseMask,
		      GrabModeAsync, GrabModeAsync, root,
		      invCursor, CurrentTime) != GrabSuccess) {
	fprintf (stderr, "%s:  unable to grab pointer\n", ProgramName);
	Exit (1);
    }

    /*
     * Do the loop looking for a button press, drawing the image each time;
     * since other clients may be running, make sure that the xor segments all
     * get drawn at the same time.  
     */

#define FAROFFSCREEN -9999

    for (done = False, lastx = lasty = FAROFFSCREEN; !done; ) {
	if (XPending (dpy) != 0) {
	    XEvent event;

	    XNextEvent (dpy, &event);

	    switch (event.type) {
	      case ButtonRelease:
		root_x = event.xbutton.x_root;
		root_y = event.xbutton.y_root;
		done = True;
		break;
	      default:
		fprintf (stderr, "%s:  warning unhandled event %lu = 0x%lx\n",
			 ProgramName, event.type, event.type);
		continue;
	    }				/* end switch */
	} else if (!XQueryPointer (dpy, root, &root_window, &child_window,
				   &root_x, &root_y, &win_x, &win_y, &mask)) {
	    fprintf (stderr, "%s:  unable to QueryPointer on root window.\n",
		     ProgramName);
	    Exit (1);
	}
	if (root_x != lastx || root_y != lasty) {
	    register int i;

	    /* clip to screen */

	    x = root_x - xoff;
	    y = root_y - yoff;
	    if (x < 0) x = 0;
	    if (y < 0) y = 0;
	    if (x + width > dw) x = dw - width;
	    if (y + height > dh) y = dh - height;

	    /* fill in the outline box */

	    for (i = 0; i < NPOINTS; i += 4) {
		box[i].x = x; box[i].y = y;	 /* u.l. */
		if (i == (NPOINTS - 1)) break;
		box[i+1].x = x + width; box[i+1].y = y;  /* u.r. */
		box[i+2].x = box[i+1].x; box[i+2].y = y + height;  /* l.r. */
		box[i+3].x = x; box[i+3].y = box[i+2].y;  /* l.l. */
	    }
	    lastx = root_x;
	    lasty = root_y;
	}

	XDrawLines (dpy, root, invGC, box, NPOINTS, CoordModeOrigin);
    }
    XUngrabPointer (dpy, CurrentTime);
    XFlush (dpy);

    shp->x = x;
    shp->y = y;

    return;
}

Bool do_magnify (shp, whp, magnification, back_pixel)
    XSizeHints *shp, *whp;
    int magnification;
    unsigned long back_pixel;
{
    int x = shp->x, y = shp->y, width = shp->width, height = shp->height;
    int dw = DisplayWidth (dpy, screen), dh = DisplayHeight (dpy, screen);
    XImage *image;
    Bool done, domore;

    /* 
     * Don't get bits that are off the edges of the screen
     */

    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x + width > dw) x = dw - width;
    if (y + height > dh) y = dh - height;

    image = XGetImage (dpy, root, x, y, width, height, 
    		       AllPlanes, pixmap_format);

    if (do_grab) {
	XUngrabServer (dpy);
	XSync (dpy, 0);
    }

    if (!image) return (False);

    /*
     * Map the window and do the work.  Space means do another;
     * Button1 press displays pixel value.
     * q, Q, or ^C mean quit.
     */

    domore = False;			/* ButtonRelease will override */

    XMapWindow (dpy, w);
    for (done = False; !done; ) {
	XEvent event;
        XButtonEvent *buttonevent;
        XColor def;
	int len,first;
	char keybuffer[10];

	XNextEvent (dpy, &event);
	switch (event.type) {
	  case Expose:
	    repaint_image (image, &event, magnification, back_pixel);
	    break;

	  case ButtonRelease:
            buttonevent = (XButtonEvent *) &event;
            if (buttonevent->button != Button1)
		domore = done = True;
	    break;

	  case ButtonPress:
            buttonevent = (XButtonEvent *) &event;
            if (buttonevent->button != Button1)
		continue;
            first = 1;
	    MapPixWindow (w, (buttonevent->y/magnification < height/2));
            while (1) {     /* loop until button released */
                Window rootW,childW;
                int rx,ry,x,y,lx,ly;
                unsigned int mask;
                char str[64];

                if (XQueryPointer(dpy,w,&rootW,&childW,&rx,&ry,&x,&y,&mask)) {
                    if (!(mask & Button1Mask)) break;    /* button released */
                    
                    x /= magnification;  y /= magnification;

		    /* look for new pixel */
                    if ((first || x != lx || y != ly) && 
			(x >= 0 && x < width && y >= 0 && y < height)) {
                        def.pixel = XGetPixel (image, x, y);
			if (def.pixel >= 0 && def.pixel < ncolormapentries) {
			    XQueryColor (dpy, cmap, &def);
			    sprintf (str, 
			        "Pixel at (%d,%d):  %3ld   (%04x, %04x, %04x)",
			        x, y, def.pixel, def.red, def.green, def.blue);
			} else {
			    def.red = def.green = def.blue = 0;
			    sprintf (str, "Pixel at (%d,%d):  %3ld",
				     x, y, def.pixel);
			}
            
                        DrawPixWindow (str);
                        first = 0;  lx = x;  ly = y;
		    }
		} else
		  break;
	    }
            UnmapPixWindow ();
	    break;

	  case KeyPress:
	    len = XLookupString (&event.xkey, keybuffer, sizeof keybuffer,
				 NULL, NULL);
	    if (len == 1 && (keybuffer[0] == 'q' || keybuffer[0] == 'Q' ||
			     keybuffer[0] == '\003')) {
		domore = False;		/* q, Q, or ^C to quit */
		done = True;
	    } else if (len == 1 && keybuffer[0] == ' ') {
		domore = True;		/* space to continue */
		done = True;
	    }				/* else ignore */
	    break;
	}
    }
    XUnmapWindow (dpy, w);
    whp->flags |= (USPosition | USSize);
    XSetWMNormalHints (dpy, w, whp);

    /*
     * free the image and return
     */

    XDestroyImage (image);
    return (domore);
}

void repaint_image (image, eventp, magnification, back_pixel)
    XImage *image;
    XEvent *eventp;
    int magnification;
    unsigned long back_pixel;
{
    XExposeEvent *ev = &eventp->xexpose;  /* to get repaint section */
    int e_row, e_column;		/* expose dimensions */
    int r_x, r_y, r_width, r_height;	/* filled rectangle dimensions */
    XGCValues gcv;			/* for doing filled rectangles */
    int row, column;			/* iterators */
    int initialx, maxrow, maxcolumn;	/* avoid recomputation */
    unsigned long pixel;		/* image pixel values */
    unsigned long prev_pixel;		/* last pixel set with XChangeGC */

    gcv.foreground = prev_pixel = back_pixel;
    XChangeGC (dpy, fillGC, GCForeground, &gcv);

    /*
     * figure out where in the image we have to start and end
     */

    e_row = ev->y / magnification;
    e_column = ev->x / magnification;
    initialx = e_column * magnification;
    maxrow = e_row +
      ((ev->y + ev->height + magnification - 1) / magnification) - 1;
    maxcolumn = e_column +
      ((ev->x + ev->width + magnification - 1) / magnification) - 1;

    /*
     * clip to image size
     */

    if (maxcolumn >= image->width) maxcolumn = image->width - 1;
    if (maxrow >= image->height) maxrow = image->height - 1;

    /*
     * Repaint the image; be somewhat clever by only drawing when the pixel
     * changes or when we run out of lines.  
     */

    r_y = e_row * magnification;	/* to start */
    r_height = magnification;		/* always doing just 1 line */

    /*
     * iterate over scanlines (rows)
     */

    for (row = e_row; row <= maxrow; row++) {
	r_x = initialx;			/* start at beginning */
	r_width = 0;			/* have done nothing so far */

	/*
	 * iterate over pixels in scanline (columns)
	 */

	for (column = e_column; column <= maxcolumn; column++) {
	    pixel = XGetPixel (image, column, row);

	    /*
	     * If the current pixel is different from the previous pixel,
	     * then we need to set the drawing color and do the previous
	     * rectangle.  We can also avoid extra calls to XChangeGC by
	     * keeping track of the last value we set (note that this may
	     * be different from gcv.foreground if the latter is the same
	     * as back_pixel).  This is most useful when magnifying the
	     * background stipple pattern on monochrome displays.
	     */
	    if (pixel != gcv.foreground) {
		/*
		 * We only need to draw the rectangle if it isn't background.
		 */
		if (gcv.foreground != back_pixel && r_width > 0) {
		    if (gcv.foreground != prev_pixel) {
			XChangeGC (dpy, fillGC, GCForeground, &gcv);
			prev_pixel = gcv.foreground;
		    }
		    XFillRectangle (dpy, w, fillGC,
				    r_x, r_y, r_width, r_height);
		}			/* end if */
		r_x += r_width;
		gcv.foreground = pixel;
		r_width = 0;
	    }				/* end if */
	    r_width += magnification;
	}				/* end for */
	/*
	 * draw final rectangle on line
	 */
	if (gcv.foreground != back_pixel && r_width > 0) {
	    if (gcv.foreground != prev_pixel) {
		XChangeGC (dpy, fillGC, GCForeground, &gcv);
		prev_pixel = gcv.foreground;
	    }
	    XFillRectangle (dpy, w, fillGC, r_x, r_y, r_width, r_height);
	}				/* end if */

	r_y += magnification;		/* advance to next scanline */
    }					/* end for */

    XFlush (dpy);
    return;
}


static Window pixwind = None;
static XFontStruct *pixfinfo = NULL;

MapPixWindow (wind, bot)
    Window wind;
    int bot;
{

    /* creates a small window inside of 'wind', at the top or bottom,
       suitable for displaying one line of text.  Also loads in a font 
       if necessary; John Bradley, University of Pennsylvania */

    Window rootW;
    int x,y;
    unsigned int w,h,bwide,depth,thigh;

    XGetGeometry (dpy, wind, &rootW, &x, &y, &w, &h, &bwide, &depth);

    if (!pixfinfo) {
	if (!pix_font) pix_font = DEFAULT_PIX_FONT;
	pixfinfo = XLoadQueryFont (dpy, pix_font);
	if (!pixfinfo) {
	    fprintf (stderr, "%s:  unable to load font '%s'\n",
		     ProgramName, pix_font);
	    Exit (1);
	}
    }

    thigh = pixfinfo->ascent + pixfinfo->descent + 4;
    y = (bot ? (h - thigh) : 0);

    if (pixwind == None) {
	XSetFont (dpy, fillGC, pixfinfo->fid);
	pixwind = XCreateSimpleWindow (dpy, wind, 0, y, w, thigh, 
				       0, None, BlackPixel (dpy, screen));
    } else {
	XMoveWindow (dpy, pixwind, 0, y);
    }

    XMapWindow (dpy, pixwind);
}


DrawPixWindow (str)
char *str;
{
    XClearWindow (dpy, pixwind);
    XSetForeground (dpy, fillGC, WhitePixel (dpy, screen));
    XDrawString (dpy, pixwind, fillGC, 4, 2 + pixfinfo->ascent,
		 str, strlen (str));
}


UnmapPixWindow()
{
    XUnmapWindow (dpy, pixwind);
}

