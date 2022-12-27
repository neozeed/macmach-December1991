/*
 * xkill - simple program for destroying unwanted clients
 *
 * $XConsortium: xkill.c,v 1.15 89/12/10 16:30:14 rws Exp $
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
 * Author:  Jim Fulton, MIT X Consortium; Dana Chee, Bellcore
 */

/*
 * Warning, this is a very dangerous client....
 */

#include <stdio.h>
#include <ctype.h>

#include <X11/Xos.h>
#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <X11/Xproto.h>

#include <X11/Xmu/WinUtil.h>

Display *dpy = NULL;
char *ProgramName;

#define SelectButtonAny (-1)
#define SelectButtonFirst (-2)

XID parse_id(), get_window_id();
int parse_button(), verify_okay_to_kill();

Exit (code)
    int code;
{
    if (dpy) {
	XCloseDisplay (dpy);
    }
    exit (code);
}

usage ()
{
    static char *options[] = {
"where options include:",
"    -display displayname    X server to contact",
"    -id resource            resource whose client is to be killed",
"    -frame                  don't ignore window manager frames",
"    -button number          specific button to be pressed to select window",
"    -all                    kill all clients with top level windows",
"",
NULL};
    char **cpp;

    fprintf (stderr, "usage:  %s [-option ...]\n",
	     ProgramName);
    for (cpp = options; *cpp; cpp++) {
	fprintf (stderr, "%s\n", *cpp);
    }
    Exit (1);
}

main (argc, argv)
    int argc;
    char *argv[];
{
    int i;				/* iterator, temp variable */
    char *displayname = NULL;		/* name of server to contact */
    int screenno;			/* screen number of dpy */
    XID id = None;			/* resource to kill */
    char *button_name = NULL;		/* name of button for window select */
    int button;				/* button number or negative for all */
    Bool kill_all = False;
    Bool top = False;

    ProgramName = argv[0];

    for (i = 1; i < argc; i++) {
	char *arg = argv[i];

	if (arg[0] == '-') {
	    switch (arg[1]) {
	      case 'd':			/* -display displayname */
		if (++i >= argc) usage ();
		displayname = argv[i];
		continue;
	      case 'i':			/* -id resourceid */
		if (++i >= argc) usage ();
		id = parse_id (argv[i]);
		continue;
	      case 'b':			/* -button number */
		if (++i >= argc) usage ();
		button_name = argv[i];
		continue;
	      case 'f':			/* -frame */
		top = True;
		continue;
	      case 'a':			/* -all */
		kill_all = True;
		continue;
	      default:
		usage ();
	    }
	} else {
	    usage ();
	}
    }					/* end for */

    dpy = XOpenDisplay (displayname);
    if (!dpy) {
	fprintf (stderr, "%s:  unable to open display \"%s\"\n",
		 ProgramName, XDisplayName (displayname));
	Exit (1);
    }
    screenno = DefaultScreen (dpy);

    if (kill_all) {
	if (verify_okay_to_kill (dpy, screenno)) 
	  kill_all_windows (dpy, screenno, top);
	Exit (0);
    }

    /*
     * if no id was given, we need to choose a window
     */

    if (id == None) {
	if (!button_name)
	    button_name = XGetDefault (dpy, ProgramName, "Button");

	if (!button_name)
	    button = SelectButtonFirst;
	else if (!parse_button (button_name, &button)) {
	    fprintf (stderr, "%s:  invalid button specification \"%s\"\n",
		     ProgramName, button_name);
	    Exit (1);
	}

	if (button >= 0 || button == SelectButtonFirst) {
	    unsigned char pointer_map[256];	 /* 8 bits of pointer num */
	    int count, j;
	    unsigned int ub = (unsigned int) button;


	    count = XGetPointerMapping (dpy, pointer_map, 256);
	    if (count <= 0) {
		fprintf (stderr, 
			 "%s:  no pointer mapping, can't select window\n",
			 ProgramName);
		Exit (1);
	    }

	    if (button >= 0) {			/* check button */
		for (j = 0; j < count; j++) {
		    if (ub == (unsigned int) pointer_map[j]) break;
		}
		if (j == count) {
		    fprintf (stderr,
	 "%s:  no button number %u in pointer map, can't select window\n",
			     ProgramName, ub);
		    Exit (1);
	        }
	    } else {				/* get first entry */
		button = (int) ((unsigned int) pointer_map[0]);
	    }
	}
	if (id = get_window_id (dpy, screenno, button,
				"the window whose client you wish to kill")) {
	    if (id == RootWindow(dpy,screenno)) id = None;
	    else if (!top) {
		Window root;
		int dummyi;
		unsigned int dummy;

		if (XGetGeometry (dpy, id, &root, &dummyi, &dummyi,
				  &dummy, &dummy, &dummy, &dummy) &&
		    id != root)
		  id = XmuClientWindow(dpy, id);
	    }
	}
    }

    if (id != None) {
	printf ("%s:  killing creator of resource 0x%lx\n", ProgramName, id);
	XSync (dpy, 0);			/* give xterm a chance */
	XKillClient (dpy, id);
	XSync (dpy, 0);
    }

    Exit (0);
}

int parse_button (s, buttonp)
    register char *s;
    int *buttonp;
{
    register char *cp;

    /* lower case name */
    for (cp = s; *cp; cp++) {
	if (isascii (*cp) && isupper (*cp)) {
#ifdef _tolower
	    *cp = _tolower (*cp);
#else
	    *cp = tolower (*cp);
#endif /* _tolower */
	}
    }

    if (strcmp (s, "any") == 0) {
	*buttonp = SelectButtonAny;
	return (1);
    }

    /* check for non-numeric input */
    for (cp = s; *cp; cp++) {
	if (!(isascii (*cp) && isdigit (*cp))) return (0);  /* bogus name */
    }

    *buttonp = atoi (s);
    return (1);
}


XID parse_id (s)
    char *s;
{
    XID retval = None;
    char *fmt = "%ld";			/* since XID is long */

    if (s) {
	if (*s == '0') s++, fmt = "%lo";
	if (*s == 'x' || *s == 'X') s++, fmt = "%lx";
	sscanf (s, fmt, &retval);
    }
    return (retval);
}

XID get_window_id (dpy, screen, button, msg)
    Display *dpy;
    int screen;
    int button;
    char *msg;
{
    Cursor cursor;		/* cursor to use when selecting */
    Window root;		/* the current root */
    Window retwin = None;	/* the window that got selected */
    int retbutton = -1;		/* button used to select window */
    int pressed = 0;		/* count of number of buttons pressed */

#define MASK (ButtonPressMask | ButtonReleaseMask)

    root = RootWindow (dpy, screen);
    cursor = XCreateFontCursor (dpy, XC_draped_box);
    if (cursor == None) {
	fprintf (stderr, "%s:  unable to create selection cursor\n",
		 ProgramName);
	Exit (1);
    }

    printf ("Select %s with ", msg);
    if (button == -1)
      printf ("any button");
    else
      printf ("button %d", button);
    printf ("....\n");
    XSync (dpy, 0);			/* give xterm a chance */

    if (XGrabPointer (dpy, root, False, MASK, GrabModeSync, GrabModeAsync, 
    		      None, cursor, CurrentTime) != GrabSuccess) {
	fprintf (stderr, "%s:  unable to grab cursor\n", ProgramName);
	Exit (1);
    }

    /* from dsimple.c in xwininfo */
    while (retwin == None || pressed != 0) {
	XEvent event;

	XAllowEvents (dpy, SyncPointer, CurrentTime);
	XWindowEvent (dpy, root, MASK, &event);
	switch (event.type) {
	  case ButtonPress:
	    if (retwin == None) {
		retbutton = event.xbutton.button;
		retwin = ((event.xbutton.subwindow != None) ?
			  event.xbutton.subwindow : root);
	    }
	    pressed++;
	    continue;
	  case ButtonRelease:
	    if (pressed > 0) pressed--;
	    continue;
	}					/* end switch */
    }						/* end for */

    XUngrabPointer (dpy, CurrentTime);
    XFreeCursor (dpy, cursor);
    XSync (dpy, 0);

    return ((button == -1 || retbutton == button) ? retwin : None);
}


int catch_window_errors (dpy, ev)
    Display *dpy;
    XErrorEvent *ev;
{
    return 0;
}

int kill_all_windows (dpy, screenno, top)
    Display *dpy;
    int screenno;
    Bool top;
{
    Window root = RootWindow (dpy, screenno);
    Window dummywindow;
    Window *children;
    unsigned int nchildren;
    unsigned int i;
    XWindowAttributes attr;

    XSync (dpy, 0);
    XSetErrorHandler (catch_window_errors);

    nchildren = 0;
    XQueryTree (dpy, root, &dummywindow, &dummywindow, &children, &nchildren);
    for (i = 0; i < nchildren; i++) {
	if (!XGetWindowAttributes(dpy, children[i], &attr) &&
	    (attr.map_state == IsViewable))
	    if (!top) children[i] = XmuClientWindow(children[i]);
	    XKillClient (dpy, children[i]);
    }
    XFree ((char *)children);

    XSync (dpy, 0);
    XSetErrorHandler (NULL);		/* pretty stupid way to do things... */

    return 0;
}

/*
 * ask the user to press in the root with each button in succession
 */
int verify_okay_to_kill (dpy, screenno)
    Display *dpy;
    int screenno;
{
    unsigned char pointer_map[256];
    int count = XGetPointerMapping (dpy, pointer_map, 256);
    int i;
    int button;
    static char *msg = "the root window";
    Window root = RootWindow (dpy, screenno);
    int okay = 0;

    for (i = 0; i < count; i++) {
	button = (int) pointer_map[i];
	if (button == 0) continue;	/* disabled */
	if (get_window_id (dpy, screenno, button, msg) != root) {
	    okay = 0;
	    break;
	}
	okay++;				/* must have at least one button */
    }
    if (okay) {
	return 1;
    } else {
	printf ("Aborting.\n");
	return 0;
    }
}