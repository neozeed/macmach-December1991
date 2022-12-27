/* 
 * $Locker:  $ 
 */ 
static char	*rcsid = "$XConsortium: muncher.c,v 1.9 89/12/14 09:53:35 rws Exp $";
/******************************************************************************
 * Description:
 *	The famous munching squares.
 *
 * Brought to you by Jef Poskanzer.
 *
 * Copyright (C) 1987 by UniSoft Systems.  Permission to use, copy,
 * modify, and distribute this software and its documentation for any
 * purpose and without fee is hereby granted, provided that this copyright
 * notice appear in all copies and in all supporting documentation.  No
 * representation is made about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 * Arguments:
 *	-r		display on root window instead of creating a new one
 *	-s seed		use this for the seed
 *	=wxh+x+y	X geometry for new window (default 256x256 centered)
 *	host:display	X display on which to run
 *****************************************************************************/


#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <ctype.h>

char *ProgramName;

extern GC XCreateGC();
extern long time();

#ifdef SYSV
extern int rand();
#else
extern long random();
#endif

Bool verbose = False;

/* Some good seeds - if the user does not specify one, one of these gets
   chosen randomly. */
int seeds[] =
	{
	0x0001, 0x0002, 0x0101, 0x0666, 0x1111, 0x1212, 0x1249, 0x2222,
	0x3333, 0x4001, 0x4444, 0x5252, 0x5555, 0x6666, 0x8001, 0x8010
	};


void Usage ()
{
	fprintf (stderr, "usage:  %s [-options ...]\n\n", ProgramName);
	fprintf (stderr, "where options include:\n");
	fprintf (stderr, "    -display host:dpy        X server to use\n");
	fprintf (stderr, "    -geometry geom           size of window\n");
	fprintf (stderr, "    -r                       use root window\n");
	fprintf (stderr, "    -s seed                  random seed value\n");
	fprintf (stderr, "    -v                       verbose mode\n");
	fprintf (stderr, "    -q                       quiet mode\n");
	fprintf (stderr, "\n");
	exit (1);
}

main(argc, argv)
int argc;
char **argv;
	{
	char **ap;
	char *display = NULL;
	char *geom = NULL;
	int useRoot = 0;
	int seed = 0;
	Window win;
	int winX, winY, winW, winH;
	XSetWindowAttributes xswa;
	Display *dpy;
	Screen *scr;
	GC gc;
	XGCValues gcv;
	XEvent xev;
#define BATCHSIZE 400
	XPoint points[BATCHSIZE];
	int size, n, nmask;
	register int acc, i, x, y;
	int xoffset, yoffset;

	ProgramName = argv[0];

	/* Process arguments: */
	ap = argv;
	while (*++ap) {
		if (!strncmp(*ap, "-d", 2)) {
			if (*++ap) {
				display = *ap;
			} else 
				Usage ();
		} else if (!strncmp(*ap, "-g", 2)) {
			if (*++ap) {
				geom = *ap;
			} else 
				Usage ();
		} else if (**ap == '=') 		/* obsolete */
			geom = *ap;
		else if (!strcmp(*ap, "-v"))
			verbose = True;
		else if (!strcmp(*ap, "-q"))
			verbose = False;
		else if (!strcmp(*ap, "-r"))
			useRoot = 1;
		else if (!strcmp(*ap, "-s")) {
			if ( *++ap ) {
				char *fmt = "%d";
				char *cp = *ap;

				if (*cp == '0') cp++;
				if (*cp == 'x' || *cp == 'X') fmt = "%x", cp++;
				if (sscanf (cp, fmt, &seed) != 1) 
					Usage ();
			}  else
				Usage ();
		} else
			Usage ();
	}

	if (!(dpy= XOpenDisplay(display)))
	        {
		perror("Cannot open display\n");
		exit(-1);
	        }

	scr = DefaultScreenOfDisplay(dpy);

	/* Set up window parameters, create and map window if necessary: */
	if (useRoot)
		{
		win = DefaultRootWindow(dpy);
		winX = 0;
		winY = 0;
		winW = DisplayWidth(dpy, DefaultScreen(dpy));
		winH = DisplayHeight(dpy, DefaultScreen(dpy));
		}
	else
		{
		winW = 256;
		winH = 256;
		winX = (WidthOfScreen(scr) - winW) >> 1;
		winY = (HeightOfScreen(scr) - winH) >> 1;
		if (geom) 
			XParseGeometry(geom, &winX, &winY,
				       (unsigned int *)&winW,
				       (unsigned int *)&winH);

		xswa.event_mask = 0;
		xswa.background_pixel = BlackPixelOfScreen(scr);
		win = XCreateWindow(dpy, RootWindowOfScreen(scr),
		    winX, winY, winW, winH, 0, 
		    DefaultDepthOfScreen(scr), InputOutput,
		    DefaultVisualOfScreen(scr),
		    CWEventMask | CWBackPixel, &xswa);
		XChangeProperty(dpy, win, XA_WM_NAME, XA_STRING, 8, 
				PropModeReplace,
				(unsigned char *)"Muncher", 7);
		XMapWindow(dpy, win);
		}

	/* Set up a graphics context: */
	gcv.foreground = (BlackPixelOfScreen(scr) ^ WhitePixelOfScreen(scr));
	gcv.function = GXxor;
	gc = XCreateGC(dpy, win, GCForeground | GCFunction, &gcv);

	/* Initialize munch algorithm. */
	size = ( winW < winH ? winW : winH );
	if ( size <= 0 ) size = 1;
	for ( n = 30, nmask = 0x40000000; n >= 0; n--, nmask >>= 1 )
		if ( size & nmask )
			break;
	size = 1 << n;
	nmask = size - 1;
	xoffset = ( winW - size ) / 2;
	yoffset = ( winH - size ) / 2;
	if ( seed == 0 )
		{
#ifdef SYSV
		srand((int) time(0) % 231);
		seed = seeds[rand() % (sizeof(seeds)/sizeof(seeds[0]) )];
#else
		srandom((int) time(0) % 231);
		seed = seeds[random() % (sizeof(seeds)/sizeof(seeds[0]) )];
#endif
		}
	if (verbose)
		printf( "size = %d, seed = 0x%x\n", size, seed );
	acc = 0;

	/* Loop forever computing and drawing batches of points. */
	for (;;)
		{
		if (XPending(dpy))
			XNextEvent(dpy, &xev);
		
		for ( i=0; i < BATCHSIZE; i++ )
			{
			x = acc & nmask;
			y = ( ( acc >> n ) & nmask ) ^ x;

			points[i].x = x + xoffset;
			points[i].y = y + yoffset;

			acc += seed;
			}

		XDrawPoints(dpy, win, gc, points, BATCHSIZE, CoordModeOrigin);
		XSync(dpy, 0);
		}
	}
