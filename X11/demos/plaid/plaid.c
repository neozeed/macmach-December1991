#ifndef lint
static char	*rcsid = "$XConsortium: plaid.c,v 1.12 89/12/14 09:54:16 rws Exp $";
#endif
/***********************************************************
Copyright 1987, 1988 by Digital Equipment Corporation, Maynard, Massachusetts,
and the Massachusetts Institute of Technology, Cambridge, Massachusetts.

             /gh10/X.V11R2/demos/plaid           All Rights Reserved

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
#include "X11/Xlib.h"
#include "X11/Xatom.h"
#include <stdio.h>
#include <errno.h>

extern int errno;

char *ProgramName;

int rand();
char *malloc();

Window XCreateWindow();

Window myWin, newWin, threeWin;

Display *dpy;

#define NUMRECTS 10
XRectangle rects[NUMRECTS];
GC gc;

usage ()
{
    fprintf (stderr, "usage:  %s [-options ...]\n\n", ProgramName);
    fprintf (stderr, "where options include:\n");
    fprintf (stderr, "    -display host:dpy        X server to use\n");
    fprintf (stderr, "    -geometry geom           geometry of window\n");
    fprintf (stderr, "    -b                       use backing store\n");
    fprintf (stderr, "    -fg color                set foreground color\n");
    fprintf (stderr, "    -bg color                set background color\n");
    fprintf (stderr, "    -bd color                set border color\n");
    fprintf (stderr, "    -bw width                set border width\n");
    fprintf (stderr, "\n");
    exit (1);
}


main(argc, argv)
int	argc;
char	*argv[];
{
    int i, j ;
    char *geom = NULL;
    int winx, winy, winw, winh;
    register int xdir, ydir;
    register int xoff, yoff;
    register int centerX, centerY;
    XGCValues xgcv;
    XSetWindowAttributes xswa;
    Colormap map;
    unsigned int bw = 1;
    char *display = NULL;
    Status status;
    char *fg = NULL;
    char *bg = NULL;
    char *bd = NULL;
    int fg_pix, bg_pix, bd_pix;
    XColor fg_def, fg_exact, bg_def, bg_exact, bd_def, bd_exact;
    int bs = NotUseful;
    Visual visual;

    ProgramName = argv[0];

    for (i=1; i < argc; i++)
    {
	char *arg = argv[i];

	if (arg[0] == '-') {
	    switch (arg[1]) {
		case 'd':			/* -display host:dpy */
		    if (++i >= argc) usage ();
		    display = argv[i];
		    continue;
		case 'g':			/* -geometry host:dpy */
		    if (++i >= argc) usage ();
		    geom = argv[i];
		    continue;
		case 'b':			/* -b or -bg or -bd */
		    if (!strcmp(argv[i], "-bg")) {
			if (++i >= argc) usage ();
			bg = argv[i];
		    } else if (!strcmp(argv[i], "-bd")) {
			if (++i >= argc) usage ();
			bd = argv[i];
		    } else if (!strcmp(argv[i], "-bw")) {
			if (++i >= argc) usage ();
			bw = atoi(argv[i]);
		    } else
			bs = Always;
		    continue;
		case 'f':			/* assume -fg */
		    if (++i >= argc) usage ();
		    fg = argv[i];
		    continue;
		default:
		    usage ();
	    }
	} else if (argv [i] [0] == '=') 	/* obsolete */
	        geom = argv[i];
	else
	    usage ();
    }

    if (!(dpy = XOpenDisplay(display)))
    {
	perror("Cannot open display\n");
	exit(-1);
    }

    map = XDefaultColormap(dpy,DefaultScreen(dpy));
    if (fg) {
	status = XAllocNamedColor(dpy, map, fg, &fg_def, &fg_exact);
	fg_pix = status ? fg_def.pixel : WhitePixel(dpy, DefaultScreen(dpy));
    } else
	fg_pix = WhitePixel(dpy, DefaultScreen(dpy));

    if (bg) {
	status = XAllocNamedColor(dpy, map, bg, &bg_def, &bg_exact);
	bg_pix = status ? bg_def.pixel : BlackPixel(dpy, DefaultScreen(dpy));
    } else
	bg_pix = BlackPixel(dpy, DefaultScreen(dpy));

    if (bd) {
	status = XAllocNamedColor(dpy, map, bd, &bd_def, &bd_exact);
	bd_pix = status ? bd_def.pixel : WhitePixel(dpy, DefaultScreen(dpy));
    } else
	bd_pix = WhitePixel(dpy, DefaultScreen(dpy));

    winx = 0;
    winy = 0;
    winw = 101;
    winh = 201;

    if (geom) 
    {
        (void) XParseGeometry(geom, &winx, &winy,
			      (unsigned int *)&winw,
			      (unsigned int *)&winh);
    }

    xswa.backing_store = bs;
    xswa.event_mask = ExposureMask | StructureNotifyMask;
    xswa.background_pixel = bg_pix;
    xswa.border_pixel = bd_pix;
    visual.visualid = CopyFromParent;
    myWin = XCreateWindow(dpy,
		RootWindow(dpy, DefaultScreen(dpy)),
		winx, winy, winw, winh, bw, 
		DefaultDepth(dpy, DefaultScreen(dpy)), InputOutput,
		&visual, 
	        CWEventMask | CWBackingStore | CWBorderPixel | CWBackPixel, 
		&xswa);

    XChangeProperty(dpy, myWin, XA_WM_NAME, XA_STRING, 8,
		PropModeReplace, (unsigned char *)"Plaid", 5);
    XMapWindow(dpy, myWin);

    xgcv.foreground = fg_pix;
    xgcv.function = GXinvert;
    xgcv.plane_mask = fg_pix ^ bg_pix;
    xgcv.fill_style = FillSolid;
    xgcv.graphics_exposures = False;
    gc = XCreateGC(dpy, myWin,
		GCForeground | GCFunction | GCPlaneMask | GCFillStyle |
		GCGraphicsExposures, &xgcv);
    j=0;
    while(1)
    {
	XEvent pe;
	XExposeEvent *ee;
	XConfigureEvent *ce;

        XNextEvent(dpy, &pe);	/* this should get first exposure event */    
	switch (pe.type) {
	  case Expose:
	    ee = (XExposeEvent *) &pe;
	    while (ee->count)
	    {
                XNextEvent(dpy, &pe);	    
	        ee = (XExposeEvent *) &pe;
	    }
	    break;
	  case ConfigureNotify:
	    ce = (XConfigureEvent *)&pe;
	    winx = ce->x;
	    winy = ce->y;
	    winw = ce->width;
	    winh = ce->height;
	    break;
	  case CirculateNotify:
	  case DestroyNotify:
	  case GravityNotify:
	  case MapNotify:
	  case ReparentNotify:
	  case UnmapNotify:
	    break;
	  default:
	    printf("Unknown event type: %d\n", pe.type);
	}

	printf("PLAID: Dealing with exposures\n");	
	XClearArea(dpy, myWin, 0, 0, winw, winh, 0);
        printf("PLAID: drawing rects\n");

	centerX = winw / 2;
	centerY = winh / 2;
	xdir = ydir = -2;
	xoff = yoff = 2;

	i = 0;
        while (! XPending(dpy))
	{
	    
	    rects[i].x = centerX - xoff;
	    rects[i].y = centerY - yoff;
	    rects[i].width = 2 * xoff;
	    rects[i].height = 2 * yoff;
	    xoff += xdir;
	    yoff += ydir;
	    if ((xoff <= 0) || (xoff >= centerX))
	    {
		xoff -= 2*xdir;
	        xdir = -xdir;
	    }
	    if ((yoff <= 0) || (yoff >= centerY))
	    {
	        yoff -= 2*ydir;
		ydir = -ydir;
	    }
	    if (i == (NUMRECTS - 1))
	    {
                XFillRectangles(dpy, myWin, gc, rects, NUMRECTS);
		i = 0;
	    }
	    else
		i++;
	}
    }
}
