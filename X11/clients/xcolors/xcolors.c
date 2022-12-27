/* 
 * xcolors - display all X11 color names and colors
 * Time stamp <89/12/01 19:14:25 gildea>
 *
 * Copyright (c) 1989 by Stephen Gildea
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appears in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation.  The authors make no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * Authors:  Stephen Gildea, Paul Vixie
 */

#include <ctype.h>
#include <stdio.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Paned.h>

#ifndef RGB_TXT
#define RGB_TXT "/usr/lib/X11/rgb.txt"
#endif

static XrmOptionDescRec options[] = {
    {"-label", XtNlabel, XrmoptionSepArg, NULL}
};

char *program_name;
int n_colors_displayed = 0;	/* number of colors we are showing */

/* ARGSUSED */
void
Destroyed(widget, closure, callData)
     Widget widget;		/* unused */
     caddr_t closure;		/* unused */
     caddr_t callData;		/* unused */
{
    exit(0);
}



/* ARGSUSED */
void
quitFunc(widget, closure, callData)
     Widget widget;		/* unused */
     caddr_t closure;		/* widget to destroy */
     caddr_t callData;		/* unused */
{
    XtDestroyWidget((Widget)closure);
}


main(argc, argv)
     unsigned int argc;
     char **argv;
{
    static XtCallbackRec	callback[2];    /* K&R: initialized to NULL */
    Widget	toplevel, outer, colors;
    Arg		arg[10];
    int	 	n;
    char *startcolor = (char *)NULL;
    
    toplevel = XtInitialize(	"main", "XColors",
			    options, XtNumber(options),
			    &argc, argv	);

    program_name = argv[0];
    argc--; argv++;

    if (argc >= 2  && !strcmp(argv[0], "-start")) {
	startcolor = argv[1];
	argc -= 2;
	argv += 2;
    }

    if (argc != 0) {
	fprintf(stderr, "Usage: %s [-start startcolor] [XtOption] ...\n",
		program_name);
	exit(3);
    }

    n = 0;
    XtSetArg(arg[n], XtNwidth, 700);
    n++;
    outer = XtCreateManagedWidget(	(char *)NULL, panedWidgetClass,
				  toplevel, arg, n	);
    XtAddCallback(outer, XtNdestroyCallback, Destroyed, NULL);
    
    n = 0;
    callback[0].callback = quitFunc;
    callback[0].closure = (caddr_t) outer;
    XtSetArg(arg[n], XtNcallback, callback);
    n++;
    XtCreateManagedWidget(		"quit", commandWidgetClass,
			  outer, arg, n		);
    
    n = 0;
    colors = XtCreateManagedWidget(	"colors", boxWidgetClass,
				   outer, arg, n		);
    
    colordemo(colors, startcolor);

    if (n_colors_displayed == 0) {
	fprintf(stderr, "%s: no colors to display\n", program_name);
	exit(5);
    }
    
    XtRealizeWidget(toplevel);
    
    XtMainLoop();
}

colordemo(parent, startcolor)
     Widget parent;
     char *startcolor;
{
    int r, g, b, prev_r, prev_g, prev_b;
    char colorname[50], save_colorname[50];
    FILE *rgb;
    Bool do_color();
    
    if (!(rgb = fopen(RGB_TXT, "r"))) {
	perror(RGB_TXT);
	exit(2);		/* vms will be with me forever */
    }
    
    prev_r = prev_g = prev_b = -1;
    save_colorname[0] = '\0';
    while (4 == fscanf(rgb, "%d %d %d %[^\n]\n", &r, &g, &b, colorname)) {
	if (startcolor)
	  if (strcasecmp(colorname, startcolor))
	    continue;		/* haven't reached starting point yet */
	  else
	    startcolor = (char *)NULL;
	if (r != prev_r  ||  g != prev_g  ||  b != prev_b) {
	    if (save_colorname[0] != '\0') /* skip first time through */
	      if (!do_color(parent, save_colorname))
		return;
	    prev_r = r;
	    prev_g = g;
	    prev_b = b;
	}
	strcpy(save_colorname, colorname);
    }
    if (save_colorname[0] != '\0')
      (void)do_color(parent, colorname);

    if (startcolor) {		/* never found starting color in the file */
	fprintf(stderr, "%s: \"%s\" not found in %s\n",
		program_name, startcolor, RGB_TXT);
    }
}


/*
 * does one color.
 * Returns a success code.
 */
Bool
do_color(parent, colorname)
     Widget parent;
     char *colorname;
{
    Arg		arg[10];
    int 	n;
    XrmValue	namein, pixelout;
    
    /* convert colorname to a Pixel (a colormap index) */
    namein.addr = colorname;
    namein.size = strlen(colorname) + 1;
    pixelout.size = 0;	/* so we can check for success */
    XtConvert(parent, XtRString, &namein, XtRPixel, &pixelout);
    if (pixelout.size == 0) {
	fprintf(stderr, "%s: Not enough room in color map.\n", program_name);
	fprintf(stderr, "To see colors after this, ");
	fprintf(stderr, "use the -start option to name a starting color.\n");
	return FALSE;
    }
    
    /* create a widget to display the color */
    n = 0;
    XtSetArg(arg[n], XtNborderWidth, 10);
    n++;
    XtSetArg(arg[n], XtNborderColor, *(Pixel*)(pixelout.addr));
    n++;
    XtCreateManagedWidget(colorname, labelWidgetClass, parent, arg, n);
    n_colors_displayed++;
    return TRUE;
}


/*
 * Similar to strcmp, but ignores case.
 * Always returns 1 if different.
 */
int
strcasecmp(s1, s2)
     char *s1, *s2;
{
    for ( ; *s1 && *s2 ; s1++, s2++)
      if ((isupper(*s1) ? tolower(*s1) : *s1) !=
	  (isupper(*s2) ? tolower(*s2) : *s2))
	return 1;
    if (*s1 || *s2)
      return 1;
    return 0;
}
