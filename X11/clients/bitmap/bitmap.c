/*
 * Copyright 1985, 1986, 1987, 1988 Massachusetts Institute of Technology
 *
 * Written by Ron Newman, MIT Project Athena
 * Command line interface revised by Jim Fulton, MIT X Consortium
 */

#ifndef lint
static char *rcsid_bitmap_c = "$XConsortium: bitmap.c,v 1.67 89/12/12 12:03:19 jim Exp $";
#endif

#include <errno.h>
#include <stdio.h>
#include <ctype.h>

#include <X11/cursorfont.h>  
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <sys/stat.h>		/* Needs things in Intrinsic.h */

#include <X11/Xatom.h>
#include <X11/Xmu/Drawing.h>


#define TOP_MARGIN 10
#define LEFT_MARGIN 10
#define BOTTOM_MARGIN 10
#define AROUND_RASTER_MARGIN 20
#define GRID_TO_COMMAND_MARGIN 5
#define RIGHT_MARGIN 5

#define MIN_SQUARE_SIZE 3
#define DEFAULT_SQUARE_SIZE 13

#define bit int
#define boolean int
#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif
#define OUT_OF_RANGE -1

#define COPY		0
#define MOVE		1
#define OVERLAY		2

#define min(x,y) ((x < y) ? x : y)
#define max(x,y) ((x < y) ? y : x)

/* error handling stuff */
extern int errno;
extern char *sys_errlist[];
extern int _Xdebug;

/* global "constants" -- set once at startup time */
/* the first few variables are not static because they are shared
   with dialog.c */
Display *d;
int screen;
GC gc;
unsigned long foreground; /* pixel */
unsigned long background; /* pixel */
unsigned long border;     /* pixel */
int borderwidth = 2;
int invertplane;
static int highlightplane = 1;
boolean use_dashed_lines = TRUE;

static XImage image = {
   0, 0,			/* width, height */
   0, XYBitmap, NULL,		/* xoffset, format, data */
   LSBFirst, 8,    		/* byte-order, bitmap-unit */
   LSBFirst, 8, 1		/* bitmap-bit-order, bitmap-pad, depth */
   };

static char *raster, *new_raster;
static int raster_length; /* how many chars in the raster[] array */
static Window outer_window, grid_window;
static Window raster_window, raster_invert_window;
static XFontStruct *font;
static Cursor cross, upper_left, lower_right, dot;
static char *filename = NULL; /* name of input file */
static char *backup_filename;
static char *stripped_name; 
  /* file name without directory path or extension */
static char *progname; /* name this program was invoked by */


/* command-button data */
void ClearOrSetAll(), InvertAll(),
      ClearOrSetArea(), InvertArea(), CopyOrMoveArea(),
      Line(), Circle(), FloodFill(),
      DialogInputHandler(),
      SetHotSpot(), ClearHotSpot(), Quit();
boolean WriteOutput();

static int DefaultSquareSize();

static struct command_data {
  char *name;
  void (*proc)(); 
     /* function to invoke when command button is "pressed" */
  int data;  /* arbitrary instance data to call procedure back with */
  Window window;
  int name_length;
  int x_offset;  /* so text is centered within command box */
  boolean inverted;
  } commands [] = {
	{"Clear All",	ClearOrSetAll, 0},
	{"Set All",	ClearOrSetAll, 1},
	{"Invert All",	InvertAll},

	{"Clear Area",	ClearOrSetArea, 0},
	{"Set Area",	ClearOrSetArea, 1},
	{"Invert Area",	InvertArea},

	{"Copy Area",	CopyOrMoveArea, COPY},
	{"Move Area", 	CopyOrMoveArea, MOVE},
	{"Overlay Area",CopyOrMoveArea, OVERLAY},

	{"Line",	  Line},
	{"Circle",	  Circle, 0},
	{"Filled Circle", Circle, 1},

	{"Flood Fill", FloodFill},

	{"Set Hot Spot",	  SetHotSpot},
	{"Clear Hot Spot", ClearHotSpot},

	{"Write Output", (void (*)()) WriteOutput},
	{"Quit",	 Quit}
  };
#define N_COMMANDS (sizeof(commands)/sizeof(commands[0]))

/* global variables */
/* layout-related variables */
static int square_size;  /* length of square's side, in pixels */
static int outer_width = 1, outer_height = 1; /* real values set by ConfigureNotify event */
static int right_side_bottom, right_side_width;

  /* location of x'd-through squares, if any */
static int x1_square_exed_through = OUT_OF_RANGE;
static int y1_square_exed_through = OUT_OF_RANGE;
static int x2_square_exed_through = OUT_OF_RANGE;
static int y2_square_exed_through = OUT_OF_RANGE;

  /* location of "plus'd through" squares, if any */
static int x1_square_plus_through = OUT_OF_RANGE;
static int y1_square_plus_through = OUT_OF_RANGE;
static int x2_square_plus_through = OUT_OF_RANGE;
static int y2_square_plus_through = OUT_OF_RANGE;

  /* location of hot spot, if any */
static int x_hot_spot = OUT_OF_RANGE;
static int y_hot_spot = OUT_OF_RANGE;

static boolean changed = FALSE;
   /* has user changed bitmap since starting program or last write? */

enum RepaintGridType {e_AgainstBackground, e_AgainstForeground, e_Invert};

extern char *malloc();

static char *yes_answers[] = { "y", "yes", "on", "all", "true", NULL };
static char *no_answers[] = { "n", "no", "off", "none", "false", NULL };

static boolean is_in_table (s, tab)
    register char *s;
    register char **tab;
{
    register char *cp;

    for (cp = s; *cp; cp++) {
	if (isupper (*cp)) *cp = tolower (*cp);
    }

    for (; *tab; tab++) {
	if (strcmp (s, *tab) == 0) return (TRUE);
    }
    return (FALSE);
}



main (argc, argv)
  int argc;
  char **argv;
  {
  SetUp (argc, argv);
  while (TRUE) {
    XEvent event;
    XNextEvent(d, &event);
    ProcessEvent(&event);
    }
  }


int ReadBitmapFile (filename)
    char *filename;
{
    unsigned int width, height;
    int x_hot, y_hot;
    unsigned char *data;
    int status;

    status = XmuReadBitmapDataFromFile (filename, &width, &height, &data,
					&x_hot, &y_hot);
    if (status != BitmapSuccess) return status;

    image.width = width;
    image.height = height;
    image.data = (char *) data;
    image.bytes_per_line = (image.width + 7) / 8;
    raster_length = image.bytes_per_line * image.height;
    x_hot_spot = x_hot;
    y_hot_spot = y_hot;

    return BitmapSuccess;
}



struct attribs {
    char *argname;		/* command line argument name */
    char *resname;		/* resource name */
    char *value;
    char *desc;
} attributes[] = {
	{ "-bw", "BorderWidth", NULL, 
	  "-bw number                    border width in pixels" },
#define bm_attr_borderwidth 0
	{ "-fn", "BodyFont", NULL,
	  "-fn font                      font used for text" },
#define bm_attr_bodyfont 1
	{ "-fg", "Foreground", NULL,
	  "-fg color                     foreground color" },
#define bm_attr_foreground 2
	{ "-bg", "Background", NULL,
	  "-bg color                     background color" },
#define bm_attr_background 3
	{ "-hl", "Highlight", NULL,
	  "-hl color                     highlight color" },
#define bm_attr_highlight 4
	{ "-bd", "BorderColor", NULL,
	  "-bd color                     border color" },
#define bm_attr_border 5
	{ "-ms", "Mouse", NULL,
	  "-ms color                     mouse color" },
#define bm_attr_mouse 6
	{ NULL, NULL, NULL, NULL }};

char *backup_fonts[] = {"variable", "fixed", "vtsingle", NULL};

usage ()
{
    struct attribs *attr;

    fprintf (stderr, 
	     "usage:  %s [-options ...] filename [WIDTHxHEIGHT]\n\n",
	     progname);
    fprintf (stderr, 
	 "where options include:\n");
    fprintf (stderr, 
	 "    -display host:dpy             X server to use\n");
    fprintf (stderr, 
	 "    -geometry geom                size of window\n");
    fprintf (stderr, 
	 "    -nodashed                     don't use dashed lines\n");
    fprintf (stderr,
	 "    -name variablename            basename to use for variables\n");
    for (attr = attributes; attr->argname != NULL; attr++) {
	fprintf (stderr, "    %s\n", attr->desc);
    }
    fprintf (stderr, "\n");
    fprintf (stderr, "The default WIDTHxSIZE is 16x16.\n"); 
    exit (1);
}

SetUp (argc, argv)
  int argc;
  char **argv;
  {
  char *StripName(), *cify_name(), *BackupName();
  char *option;
  char *geometry = NULL, *host = NULL, *dimensions = NULL;
  int i;
  int status;
  struct attribs *attr;
  char *var_name = NULL;

  progname = argv[0];

  /* Parse command line */
  for (i = 1; i < argc; i++) {
    char *arg = argv[i];
    int len;

    if (arg[0] == '-') {

	for (attr = attributes; attr->argname != NULL; attr++) {
	    if (strcmp (arg, attr->argname) == 0) {
		if (++i >= argc) usage ();
		attr->value = argv[i];
		break;				/* out of deepest for loop */
	    }
	}
	if (attr->argname) continue;		/* got an arg */
	len = strlen (arg);
	switch (arg[1]) {
	    case 'd':				/* -display host:dpy */
		if ((arg[2] && strncmp ("-display", arg, len) != 0) ||
		    (++i >= argc))
		  usage ();
		host = argv[i];
		continue;
	    case 'g':				/* -geometry geom */
		if ((arg[2] && strncmp ("-geometry", arg, len) != 0) ||
		    (++i >= argc))
		  usage ();
		geometry = argv[i];
		continue;
	    case 'n':				/* -nodashed */
		if (!arg[2] || strncmp ("-nodashed", arg, len) == 0) {
		    use_dashed_lines = FALSE;
		} else if (strncmp ("-name", arg, len) == 0) {
		   if (++i >= argc) usage ();
		   var_name = argv[i];
		} else 
		  usage ();
		continue;
	    default:
		usage ();
	}
    } else if (filename == NULL)
    	filename = argv[i];
    else if (dimensions == NULL)
    	dimensions = argv[i];
    else 
      usage ();
  }

  if (filename == NULL) {
    fprintf (stderr, "%s:  no file name specified\n", progname);
    usage ();
  }
  
  if (!(d = XOpenDisplay(host))) {
	fprintf(stderr, "%s:  unable to open display \"%s\"\n",
		argv[0], XDisplayName(host));
	exit (1);
    }

  if (geometry == NULL) geometry = XGetDefault (d, progname, "Geometry");
  if (dimensions == NULL) 
    dimensions = XGetDefault (d, progname, "Dimensions");

  option = XGetDefault (d, progname, "Dashed");
  if (option != NULL && use_dashed_lines) {
	if (is_in_table (option, no_answers)) 
	    use_dashed_lines = FALSE;
  }

  screen = DefaultScreen(d);
  gc = DefaultGC (d, screen);
  XSetLineAttributes (d, gc, 0, LineSolid, CapNotLast, JoinMiter);

  for (attr = attributes; attr->argname != NULL; attr++) {
	if (attr->value != NULL) continue;	/* got from command line */
	attr->value = XGetDefault (d, progname, attr->resname);
  }


  /* get data */

  stripped_name = cify_name (var_name ? var_name : StripName (filename));
  backup_filename = BackupName (filename);

  status = ReadBitmapFile(filename);
  if (status == BitmapFileInvalid) {
      fprintf (stderr, "%s:  input file \"%s\" is not in bitmap format\n",
	       progname, filename);
      exit (1);
  }
  else if (status == BitmapOpenFailed) {
      struct stat statbuf;

      /*
       * if the file exists, then there was some problem reading it; otherwise,
       * create it
       */
      if (stat (filename, &statbuf) == 0) {  /* exists, but can't read */
	  fprintf (stderr, 
		   "%s:  unable to open bitmap file \"%s\" for reading.\n",
		   progname, filename);
	  exit (1);
      }

      if (dimensions) {
         if (sscanf (dimensions, "%dx%d", &image.width, &image.height) != 2) {
	    fprintf (stderr, "%s:  invalid bitmap dimensions \"%s\"\n",
	       progname, dimensions);
	    usage ();
	 }
         if ((image.width <=0) || (image.height <=0)) {
            fprintf (stderr, "%s:  dimensions \"%s\" must be positive\n", 
	    	     progname, dimensions);
            exit (1);
         }
      }
      
      else /* dimensions not supplied on command line */
         image.width = image.height = 16;

      image.bytes_per_line = (image.width+7)/8;
      raster_length = image.bytes_per_line * image.height;
      raster = image.data = malloc (raster_length);
      if (!raster) {
	  fprintf (stderr, 
		   "%s:  unable to allocate %d bytes for raster\n",
		   progname, raster_length);
	  exit (1);
      }

      /* set raster to all 0's (background color) */
      for (i=0; i < raster_length; i++) raster[i] = 0;
  }
  else  /* status == BitmapSuccess */
      raster = image.data;

  new_raster = malloc (raster_length);
  if (!new_raster) {
    fprintf (stderr, "%s:  unable to allocate %d bytes for new raster\n",
	     progname, raster_length);
    exit (1);
  }
  for (i=0; i < raster_length; i++) new_raster[i] = 0;

  /* set attributes */

  if ((option = attributes[bm_attr_borderwidth].value) != NULL) {
      borderwidth = atoi (option);
      if (borderwidth < 0) borderwidth = 0;
  }

  option = attributes[bm_attr_bodyfont].value;
  font = option ? XLoadQueryFont (d, option) : NULL;
  if (!font) {
    if (option) 
      fprintf (stderr, "%s:  unable to open font \"%s\"\n", progname, option);
    for (i = 0; (option = backup_fonts[i]) != NULL; i++) {
	font = XLoadQueryFont (d, option);
	if (font)
	    break;
	else 
	    fprintf (stderr, "%s:  unable to open font \"%s\"\n", 
	    	     progname, option);
    }
    if (!font) {
	XCloseDisplay (d);
	exit (1);
    }
  }
  XSetFont (d, gc, font->fid);

  upper_left = XCreateFontCursor (d, XC_ul_angle);
  lower_right = XCreateFontCursor (d, XC_lr_angle);
  cross = XCreateFontCursor (d, XC_crosshair);
  dot = XCreateFontCursor (d, XC_dot);

  foreground = border = BlackPixel (d, screen);
  background = WhitePixel (d, screen);
  invertplane = foreground ^ background;

  if (DisplayCells(d, screen) > 2) {
    Colormap cmap = DefaultColormap (d, screen);
    char *fore_color = attributes[bm_attr_foreground].value;
    char *back_color = attributes[bm_attr_background].value;
    char *high_color = attributes[bm_attr_highlight].value;
    char *brdr_color = attributes[bm_attr_border].value;
    char *mous_color = attributes[bm_attr_mouse].value;
    XColor fdef, bdef, hdef, mdef;
    unsigned long masks[2];
    int result = 0;
    boolean warn = FALSE;

    if (!(result = (fore_color && XParseColor(d, cmap, fore_color, &fdef)))) {
	if (fore_color)
	   fprintf (stderr, "%s:  invalid foreground color \"%s\"\n", 
	   	    progname, fore_color);
	fdef.pixel = foreground;
	XQueryColor(d, cmap, &fdef);
    }
    if (back_color && XParseColor(d, cmap, back_color, &bdef)) {
	result = 1;
    } else {
	if (back_color)
	   fprintf (stderr, "%s:  invalid background color \"%s\"\n", 
	   	    progname, back_color);
	bdef.pixel = background;
	XQueryColor(d, cmap, &bdef);
    }
    if (high_color && XParseColor(d, cmap, high_color, &hdef)) {
	result = 1;
    } else {
	if (high_color)
	   fprintf (stderr, "%s:  invalid highlight color \"%s\"\n", 
	   	    progname, high_color);
	highlightplane = invertplane;
    }
    /* if background or foreground or highlight is found */
    if (result) {
	if (XAllocColorCells(d, cmap, FALSE, masks, high_color ? 2 : 1,
	 	&background, 1)) {
            bdef.pixel = background;
            XStoreColor(d, cmap, &bdef);
            invertplane = masks[0];
            if (high_color) {
	      highlightplane = masks[1];
	      hdef.pixel = background | highlightplane;
	      XStoreColor(d, cmap, &hdef);
	      hdef.pixel |= invertplane;
	      XStoreColor(d, cmap, &hdef);
            } else
	      highlightplane = invertplane;
            fdef.pixel = foreground = background | invertplane;
            XStoreColor(d, cmap, &fdef);
	}
	else {
	    warn = TRUE;
	    fprintf (stderr, "%s:  unable to allocate color cells\n", 
	    	     progname);
	}
    }
    if (brdr_color) {
	if (XParseColor(d, cmap, brdr_color, &bdef)) {
	   if (XAllocColor(d, cmap, &bdef))
	      border = bdef.pixel;
	   else {
	      if (!warn)
		 fprintf (stderr, 
		 	  "%s:  unable to allocate border color \"%s\"\n",
			  progname, brdr_color);
	   }
	}
	else {
	   fprintf (stderr, "%s:  invalid border color \"%s\"\n", 
	   	    progname, brdr_color);
	}
    }
    /* recoloring cursors is not done well */
    if (mous_color) {
	if (XParseColor (d, cmap, mous_color, &mdef)) {
	   XColor whitecolor;

	   whitecolor.red = whitecolor.green = whitecolor.blue = ~0;
	   XRecolorCursor (d, cross, &mdef, &whitecolor);
	   XRecolorCursor (d, dot, &mdef, &whitecolor);
	}
	else {
	   fprintf (stderr, "%s:  invalid mouse color \"%s\"\n", 
	   	    progname, mous_color);
	}
    }
  }    

  {
  XSizeHints hints;
  XWMHints wmHints;
  XClassHint classHints;
  XTextProperty windowName, iconName;
  char defgeom[256];
  int geom_result;
  int display_width = DisplayWidth(d, screen);
  int display_height = DisplayHeight(d, screen);
  XSetWindowAttributes attrs;

  attrs.background_pixel = background;
  attrs.border_pixel = border;
  attrs.event_mask = StructureNotifyMask | KeyPressMask;
  attrs.cursor = cross;

  outer_window = XCreateWindow (d, RootWindow (d, screen),
     0, 0, 1, 1,  /* dummy x, y, width, height; see MoveResizeWindow below */
     borderwidth, CopyFromParent, CopyFromParent, CopyFromParent,
     CWBackPixel | CWBorderPixel | CWEventMask | CWCursor,
     &attrs);
  LayoutStage1(); /* sets global variables
                     right_side_bottom, right_side_width */

  OuterWindowDims (DefaultSquareSize (display_width, display_height,
				      right_side_width, right_side_bottom),
		   right_side_width, right_side_bottom,
		   &hints.width, &hints.height);
  OuterWindowDims (1, right_side_width, right_side_bottom,
		   &hints.min_width, &hints.min_height);
  hints.base_width = hints.base_height = 0;
  hints.flags = (PMinSize | PBaseSize);

  defgeom[0] = '\0';
  sprintf (defgeom, "%dx%d", hints.width, hints.height);

  geom_result = XWMGeometry (d, screen, geometry, defgeom, borderwidth,
			     &hints, &hints.x, &hints.y, &hints.width,
			     &hints.height, &hints.win_gravity);
  hints.flags |= PWinGravity;
      
  hints.flags |= (((geom_result & WidthValue) &&
		   (geom_result & HeightValue)) ? USSize : PSize);
  
  if ((geom_result & XValue) && (geom_result & YValue))
    hints.flags |= USPosition;
  else {
     hints.x = min (100, display_width - hints.width - 2*borderwidth);
     hints.y = min (100, display_height - hints.height - 2*borderwidth);
  }

  XMoveResizeWindow (d, outer_window, hints.x, hints.y,
		     hints.width, hints.height);

  windowName.encoding = XA_STRING;
  windowName.format = 8;
  windowName.value = (unsigned char *) "Bitmap Editor";
  windowName.nitems = strlen (windowName.value);
  iconName.encoding = XA_STRING;
  iconName.format = 8;
  iconName.value = (unsigned char *) "bitmap";
  iconName.nitems = strlen (iconName.value);
  wmHints.input = True;
  wmHints.flags = InputHint;
  classHints.res_name = NULL;
  classHints.res_class = "Bitmap";
  XSetWMProperties (d, outer_window, &windowName, &iconName, argv, argc, 
		    &hints, &wmHints, &classHints);
  }

  XMapWindow (d, outer_window);

  /* the above XMoveResizeWindow will generate a ConfigureNotify event
     telling us the actual size of the window when it is mapped.  We
     wait for this event before proceeding to LayoutStage2() and
     mapping the subwindows. */
  }


ProcessEvent (event)
  register XEvent *event;
  {
  register Window w = event->xany.window;
  register int i;

  if (w == grid_window)
    ProcessGridWindowEvent (event);
  else if (w == outer_window)
    ProcessOuterWindowEvent (event);
  else if (w == raster_window)
    RepaintRaster();
  else if (w == raster_invert_window)
    RepaintRasterInverted();
  else for (i=0;i<N_COMMANDS;i++)
    if (w == commands[i].window)
      ProcessCommandButtonEvent (&commands[i], event);
  }


ProcessGridWindowEvent (event)
  XEvent *event;
  {
  int x_square, y_square;
  static int x_square_prev, y_square_prev;
  static boolean raster_outdated;
  switch (event->type) {

    case Expose: {
#define this_event ((XExposeEvent *)event)
      int x1 = this_event->x;
      int y1 = this_event->y;
      int x2 = x1 + this_event->width - 1;
      int y2 = y1 + this_event->height - 1;
#undef this_event
      if ((x2 / square_size) >= image.width)
        x2 = image.width * square_size;  /* sanity check */
      if ((y2 / square_size) >= image.height)
        y2 = image.height * square_size;  /* sanity check */
      RepaintGridLinesPartially(x1,y1,x2,y2,e_AgainstBackground,TRUE);
      x1 /= square_size;
      x2 /= square_size;
      y1 /= square_size;
      y2 /= square_size;
      if (x2 >= image.width)
        x2 = image.width - 1;  /* sanity check */
      if (y2 >= image.height)
        y2 = image.height - 1;  /* sanity check */
      RefillGridPartially (x1,y1,x2,y2,FALSE);
      if (x1_square_exed_through != OUT_OF_RANGE)
        ExThroughRectangle (
	  max (x1, x1_square_exed_through),
	  max (y1, y1_square_exed_through),
	  min (x2, x2_square_exed_through),
	  min (y2, y2_square_exed_through));
      if (x1_square_plus_through != OUT_OF_RANGE)
        PlusThroughRectangle (
	  max (x1, x1_square_plus_through),
	  max (y1, y1_square_plus_through),
	  min (x2, x2_square_plus_through),
	  min (y2, y2_square_plus_through));
      if (x_hot_spot >= x1 && x_hot_spot <= x2
      	&& y_hot_spot >= y1 && y_hot_spot <= y2)
	    HighlightHotSpot();
      break;
      }

    case ButtonPress:
      if (WhatSquare (event, &x_square, &y_square))
        return;  /* mouse outside grid; really shouldn't happen, but... */
      switch (((XButtonPressedEvent *)event)->button) {
	case 1: /* Left button */
	  PaintSquare (x_square, y_square, foreground);
	  if (!use_dashed_lines)
	     check_surroundings(x_square, y_square, 1);
	  if (x_square == x_hot_spot && y_square == y_hot_spot)
	    HighlightHotSpot();
	  SetRasterBit (raster, x_square, y_square, 1);
	  break;
	case 2: /* Middle button */
	  InvertSquare (x_square, y_square);
	  InvertRasterBit (raster, x_square, y_square);
	   if (!use_dashed_lines)
	      check_surroundings(x_square, y_square,
				GetRasterBit(raster, x_square, y_square));
	  break;
	case 3: /* Right button */
	  PaintSquare (x_square, y_square, background);
	  if (!use_dashed_lines)
	     check_surroundings(x_square, y_square, 0);
	  if (x_square == x_hot_spot && y_square == y_hot_spot)
	    HighlightHotSpot();
	  SetRasterBit (raster, x_square, y_square, 0);
	  break;
	}
      RepaintRaster();
      RepaintRasterInverted();
      x_square_prev = x_square;
      y_square_prev = y_square;
      raster_outdated = FALSE;
      changed = TRUE;
      break;
    
    case MotionNotify:
      if (WhatSquare (event, &x_square, &y_square))
        return;  /* mouse outside grid; really shouldn't happen, but... */
      if ((x_square != x_square_prev) || (y_square != y_square_prev))
       	  switch (((XMotionEvent *)event)->state) {
	    case Button1Mask: /* left button down */
	      PaintSquare (x_square, y_square, foreground);
	      if (!use_dashed_lines)
	         check_surroundings(x_square, y_square, 1);
	      if (x_square == x_hot_spot && y_square == y_hot_spot)
	      	HighlightHotSpot();
	      SetRasterBit (raster, x_square, y_square, 1);
	      changed = raster_outdated = TRUE;
	      break;
	    case Button2Mask: /* middle button down */
	      InvertSquare (x_square, y_square);
	      InvertRasterBit (raster, x_square, y_square);
	      if (!use_dashed_lines)
	         check_surroundings(x_square, y_square,
				GetRasterBit(raster, x_square, y_square));
	      changed = raster_outdated = TRUE;
	      break;
	    case Button3Mask: /* right button down */
	      PaintSquare (x_square, y_square, background);
	      if (!use_dashed_lines)
	         check_surroundings(x_square, y_square, 0);
	      if (x_square == x_hot_spot && y_square == y_hot_spot)
	      	HighlightHotSpot();
	      SetRasterBit (raster, x_square, y_square, 0);
	      changed = raster_outdated = TRUE;
	      break;
	    default: /* ignore events with multiple buttons down */
	      break; 
	    }
      if (raster_outdated && !MouseMovedEventQueued()) {
	RepaintRaster();
	RepaintRasterInverted();
	raster_outdated = FALSE;
	}
      x_square_prev = x_square;
      y_square_prev = y_square;
      break;
  
    }
  }

boolean MouseMovedEventQueued () {
  XEvent event;
  if (XPending(d) == 0) return (FALSE);
  XPeekEvent (d, &event);
  return (event.type == MotionNotify);
  }


ProcessOuterWindowEvent (event)
  XEvent *event;
  {
  char keybuf[10];

  if (event->type == KeyPress &&
      XLookupString (&event->xkey, keybuf, sizeof keybuf, NULL, NULL) == 1 &&
      keybuf[0] == 'q' || keybuf[0] == 'Q' || keybuf[0] == '\003') {
    Quit ();
    return;
  }
  if (event->type != ConfigureNotify)
    return;
  if ((outer_height == ((XConfigureEvent *)event)->height)
     && (outer_width == ((XConfigureEvent *)event)->width))
     /* if this isn't a resize, there's nothing to do here. */
     return;

  /* the outer window's size has changed.  Must rearrange subwindows. */
  outer_height = ((XConfigureEvent *)event)->height;
  outer_width = ((XConfigureEvent *)event)->width;
  LayoutStage2 ();
  XMapSubwindows (d, outer_window);
  }
  
ProcessCommandButtonEvent (command, event)
  struct command_data *command;
  XEvent *event;
  {
  static struct command_data *button_down_command;
  
  switch (event->type) {

    case Expose:
      if (((XExposeEvent *)event)->count)
      	break;  /* repaint only when last exposure is received */
      if (command->inverted)
        XClearWindow (d, command->window);
      XSetState (d, gc, foreground, background, GXcopy, AllPlanes);
      XDrawString (d, command->window, gc, command->x_offset, font->ascent,
	 command->name, command->name_length);
      if (command->inverted)
      	InvertCommandWindow (command);
      break;

    case ButtonPress:
      if (button_down_command != NULL)
        break;  /* must be a second button push--ignore */
      button_down_command = command;
      InvertCommandWindow (command);
      command->inverted = TRUE;
      break;

    case LeaveNotify:
      if (command == button_down_command) {
	InvertCommandWindow (command);
	command->inverted = FALSE;
	button_down_command = NULL;
	}
      break;

    case ButtonRelease:
      if (command == button_down_command) {
	(*command->proc)(command->data);
	button_down_command = NULL;
	InvertCommandWindow (command);
	command->inverted = FALSE;
	}
      break;
      
    }
  }


InvertCommandWindow (command)
  struct command_data *command;
  {
  XSetState (d, gc, 1L, 0L, GXinvert, invertplane);
  XFillRectangle (d, command->window, gc, 0, 0, 1000, 1000);
  }

	  
/* WhatSquare returns TRUE if mouse is outside grid, FALSE if inside.
   If it returns FALSE, it assigns to *x_square and *y_square. */

boolean WhatSquare (event, x_square, y_square)
  register XEvent *event;
  register int *x_square, *y_square; /*RETURN*/
  {
  int x = ((XButtonEvent *)event)->x;
  int y = ((XButtonEvent *)event)->y;
  if ((x < 0) || (y < 0))
    return (TRUE);
  *x_square = x/square_size;
  *y_square = y/square_size;
  return ((*x_square >= image.width) || (*y_square >= image.height));
  }


RepaintGridLines(how)
  enum RepaintGridType how;
  {
  RepaintGridLinesPartially (0, 0, image.width * square_size,
		image.height * square_size, how, TRUE);
  }

RepaintGridLinesPartially (x1, y1, x2, y2, how, include_boundaries)
  int x1, y1, x2, y2;
  enum RepaintGridType how;
  boolean include_boundaries;
  {
  int px1, px2, py1, py2;

  switch (how) {
    XGCValues gcv;
    case e_AgainstBackground:
        gcv.foreground = foreground;
	gcv.function = GXcopy;
	gcv.plane_mask = AllPlanes;
        gcv.line_style = (use_dashed_lines ? LineOnOffDash : LineSolid);
	gcv.dashes = 1;
	gcv.cap_style = CapButt;
	gcv.dash_offset = 0;
        XChangeGC (d, gc, GCForeground | GCFunction | GCPlaneMask |
	   GCLineStyle | GCDashList | GCDashOffset | GCCapStyle, &gcv);
	break;
    case e_AgainstForeground:
        gcv.foreground = background;
	gcv.function = GXcopy;
	gcv.plane_mask = AllPlanes;
        gcv.line_style = (use_dashed_lines ? LineOnOffDash : LineSolid);
	gcv.dashes = 1;
	gcv.cap_style = CapButt;
	gcv.dash_offset = 1;
        XChangeGC (d, gc, GCForeground | GCFunction | GCPlaneMask |
	   GCLineStyle | GCDashList | GCDashOffset | GCCapStyle, &gcv);
	break;
    case e_Invert:
        gcv.function = GXinvert;
        gcv.plane_mask = invertplane;
        gcv.line_style = LineSolid;
	gcv.cap_style = CapButt;
        XChangeGC (d, gc, GCFunction | GCPlaneMask | GCLineStyle |
		   GCCapStyle, &gcv);
	break;
    }
    
  /* draw vertical grid lines */
  py1 = y1;
  if (use_dashed_lines)
     py1 += (py1 & 1);  /* make sure pattern is aligned on even bit boundary */
  py2 = y2;
  if (!include_boundaries) {x1 += square_size; x2 -= square_size;}
  for(px1 = (x1 / square_size + (x1 % square_size ? 1 : 0)) * square_size;
	px1 <= x2; px1 = px1 + square_size) {
     XDrawLine (d, grid_window, gc, px1, py1, px1, py2);
     }
  if (!include_boundaries) {x1 -= square_size; x2 += square_size;}

  /* draw horizontal grid lines */
  px1 = x1;
  if (use_dashed_lines)
     px1 += (px1 & 1);  /* make sure pattern is aligned on even bit boundary */
  px2 = x2;
  if (!include_boundaries) {y1 += square_size; y2 -= square_size;}
  for(py1 = (y1 / square_size + (y1 % square_size ? 1 : 0)) * square_size;
	py1 <= y2; py1 = py1 + square_size) {
     XDrawLine (d, grid_window, gc, px1, py1, px2, py1);
     }
  }


RefillGridPartially(x1, y1, x2, y2, paint_background)
  register int x1, y1, x2, y2;
  boolean paint_background;
  {
  register i, j;
  for (i=x1; i<=x2; i++) {
    for (j=y1; j<=y2; j++) {
      bit b = GetRasterBit (raster, i, j);
      if (b || paint_background) {
      	PaintSquare (i, j, (b ? foreground : background));
	if (!use_dashed_lines)
	   check_surroundings(i, j, b);
        }
      }
    }
  }


PaintSquare(x, y, pixel)
  int x, y;
  unsigned long pixel;
  {
  XSetState (d, gc, pixel, 0L /* ignored */, GXcopy, AllPlanes);
  XFillRectangle (d, grid_window, gc, x*square_size + 1, y*square_size + 1,
    square_size - 1, square_size - 1);
  }

InvertSquare(x, y)
  int x, y;
  {
  XSetState (d, gc, 1L, 0L, GXinvert, invertplane);
  XFillRectangle (d, grid_window, gc, x*square_size + 1, y*square_size + 1,
    square_size - 1, square_size - 1);
  }

bit GetRasterBit (raster, x, y)
  char *raster;
  register int x;
  int y;
  {
  register char *byte = raster + x/8 + y*((image.width+7)/8);
  return ( (*byte & (1 << (x % 8))) ? 1 : 0);
  }


SetRasterBit (raster, x, y, new)
  char *raster;
  register int x;
  int y;
  bit new;
  {
  register char *byte = raster + x/8 + y*((image.width+7)/8);
  x %= 8;
  *byte = (new << x) | (*byte & ~(1 << x));
  }


InvertRasterBit (raster, x, y)
  char *raster;
  register int x;
  int y;
  {
  register char *byte = raster + x/8 + y*((image.width+7)/8);
  *byte ^= (1 << (x % 8));
  }


RepaintRaster() {
  XSetState (d, gc, foreground, background, GXcopy, AllPlanes);
  XPutImage (d, raster_window, gc, &image,
     0, 0, 3, 3, image.width, image.height);
  }


RepaintRasterInverted () {
  XSetState (d, gc, background, foreground, GXcopy, AllPlanes);
  XPutImage (d, raster_invert_window, gc, &image,
     0, 0, 3, 3, image.width, image.height);
  }


WriteOutputToFile (file)
  FILE *file;
  {
  register int i;
  fprintf (file, "#define %s_width %d\n", stripped_name, image.width);
  fprintf (file, "#define %s_height %d\n", stripped_name, image.height);
  if (x_hot_spot != OUT_OF_RANGE)
    fprintf (file, "#define %s_x_hot %d\n", stripped_name, x_hot_spot);
  if (y_hot_spot != OUT_OF_RANGE)
    fprintf (file, "#define %s_y_hot %d\n", stripped_name, y_hot_spot);
  fprintf (file, "static char %s_bits[] = {\n   0x%02x",
    stripped_name, (unsigned char) raster[0]);

  for (i=1;i<raster_length;i++) {
    fprintf (file, ",");
    fprintf (file, (i % 12) ? " " : "\n   ");
    fprintf (file, "0x%02x", (unsigned char) raster[i]);
    }
  fprintf (file, "};\n");
  }


char *cify_name (name)
    char *name;
{
    int length = name ? strlen (name) : 0;
    int i;

    for (i = 0; i < length; i++) {	/* strncpy (result, begin, length); */
	char c = name[i];
	if (!((isascii(c) && isalnum(c)) || c == '_')) name[i] = '_';
    }
    return name;
}

char *StripName(name)
  char *name;
  {
  char *begin = rindex (name, '/');
  char *end, *result;
  int length;

  begin = (begin ? begin+1 : name);
  end = index (begin, '.');	/* change to rindex to allow longer names */
  length = (end ? (end - begin) : strlen (begin));
  result = (char *) malloc (length + 1);
  if (!result) {
    fprintf (stderr, 
	     "%s:  unable to allocate %d bytes for stripped filename\n",
	     progname, length + 1);
    exit (1);
  }
  strncpy (result, begin, length);
  result [length] = '\0';
  return (result);
  }


char *BackupName(name)
  char *name;
  {
  int name_length = strlen (name);
  char *result = (char *) malloc (name_length+2);
  if (!result) {
    fprintf (stderr, 
	     "%s:  unable to allocate %d bytes for backup filename\n",
	     progname, name_length + 2);
    exit (1);
  }
  strncpy (result, name, name_length);
  result [name_length] = '~';
  result [name_length+1] = '\0';
  return (result);
  }

char *TmpFileName(name)
  char *name;
  {
  {
  char *begin = rindex (name, '/');
  if (begin)
    name = begin+1;
  }
  {
  char *tmp = "/tmp/";
  int name_length = strlen (name);
  int tmp_length = strlen (tmp);
  int result_length = name_length + tmp_length;
  char *result = (char *) malloc (result_length + 1);
  if (!result) {
    fprintf (stderr, 
	     "%s:  unable to allocate %d bytes for temporary filename\n",
	     progname, result_length + 1);
    exit (1);
  }
  strncpy (result, tmp, tmp_length);
  strncpy (result+tmp_length, name, name_length);
  result [result_length] = '\0';
  return (result);
  }
  }


/* LayoutStage1 creates the grid window, all commmand windows and both
   raster windows.

   The grid window is created with its proper x and y positions; a later call
   on LayoutStage2 will set its height and width before it is mapped.

   The command and raster windows are created with their proper height
   width, and y positions; it's up to LayoutStage2 to give them proper
   x positions before mapping them.  LayoutStage1 fills in everything in
   the commands[] array.

   This routine is called only once, at startup time.
   Everything done at this stage stays the same even if the user later
   reshapes the window. */

LayoutStage1 ()
  {
  int widths [N_COMMANDS];
  int command_width = 0;
  int ypos = TOP_MARGIN;
  int fontHeight = font->ascent + font->descent;
  register int i;
  XSetWindowAttributes attrs;

  /* first determine how wide the commands should be */
  for (i=0;i<N_COMMANDS;i++) {
    register struct command_data *command = &commands[i];
    command->name_length = strlen (command->name);
    widths[i] = XTextWidth (font, command->name, command->name_length);
    if (command_width < widths[i])
      command_width = widths[i];
    }

  command_width += 4; /* so even widest command has a little space around it */

  /* now create the command windows.  Command strings will be centered in them */
  /* x position of commands will be determined later */

  attrs.win_gravity = UnmapGravity;
  attrs.event_mask =
      ButtonPressMask | ButtonReleaseMask | LeaveWindowMask | ExposureMask;
  attrs.background_pixel = background;

  for (i=0;i<N_COMMANDS;i++) {
    register struct command_data *command = &commands[i];
    command->x_offset = (command_width - widths[i])/2;
    command->window = XCreateWindow (d, outer_window, 0, ypos,
	command_width, fontHeight, 1, CopyFromParent, CopyFromParent,
        CopyFromParent, CWBackPixel | CWWinGravity | CWEventMask, &attrs);
    ypos += fontHeight + 5;
    if (i == 2 || i == 5 || i == 8 || i == 11 || i == 12 || i == 14)
      ypos += fontHeight + 5;
      /* for gaps between command groups;  pretty random! */
    }
  
  /* set up raster window; x position to be determined later */
  attrs.event_mask = ExposureMask;
  ypos += AROUND_RASTER_MARGIN;
  raster_window = XCreateWindow (d, outer_window, 0, ypos,
	image.width + 6, image.height + 6, 1, CopyFromParent, CopyFromParent,
	CopyFromParent, CWBackPixel | CWWinGravity | CWEventMask, &attrs);
  
  /* set up raster invert window; x position to be determined later */
  ypos += image.height + 8 + AROUND_RASTER_MARGIN;
  raster_invert_window = XCreateWindow (d, outer_window, 0, ypos,
	image.width + 6, image.height + 6, 1, CopyFromParent, CopyFromParent,
	CopyFromParent, CWBackPixel | CWWinGravity | CWEventMask, &attrs);

  /* set up the grid window; width and height to be determined later */
  attrs.event_mask =  Button1MotionMask | Button2MotionMask | Button3MotionMask
    | ExposureMask | ButtonPressMask | ButtonReleaseMask;
    /* ButtonRelease is selected for AskUserForArea's benefit */
  grid_window = XCreateWindow (d, outer_window, LEFT_MARGIN, TOP_MARGIN,
	1, 1, 0, CopyFromParent, CopyFromParent,
	CopyFromParent, CWBackPixel | CWWinGravity | CWEventMask, &attrs);

  /* assign global variables based on this layout */
  right_side_bottom = ypos + image.height
     + 2 /* borders */ + AROUND_RASTER_MARGIN;
  right_side_width = 2 /* borders */ + max (
     command_width + GRID_TO_COMMAND_MARGIN + RIGHT_MARGIN,
     AROUND_RASTER_MARGIN + image.width);
  }


/* LayoutStage2 is called whenever a ConfigureNotify event occurs,
   whether this is at startup time or when the user resizes the outer
   window.  It figures out what the new grid square size should be,
   determines the new size of the grid window and the positions of all
   command and raster windows, then reconfigures those windows
   appropriately. */

LayoutStage2 ()
  {
  int x_room = outer_width - 1 - LEFT_MARGIN - right_side_width;
  int y_room = outer_height - 1 - TOP_MARGIN - BOTTOM_MARGIN;
  register int i;
  int grid_width;
  XWindowChanges changes;
  
  x_room /= image.width;
  y_room /= image.height;
  square_size = min (x_room, y_room);
  if (square_size < 1 || outer_height < right_side_bottom) {
     int done = FALSE;
     while (!done) {
	int result;
	static char *strings[2] = { "Yes", "No"};

	result = dialog(outer_window, font,
		"The window is not big enough; abort?",
		"Yes will abort the program, no will continue",
		strings, 2, DialogInputHandler); 
	if (result == 0)
	   Quit();
	else
	   done = TRUE;
     }
  }
  if (square_size < 1) square_size = 1;

  /* set the grid window's dimensions */
  grid_width = (image.width * square_size) + 1;
  XResizeWindow (d, grid_window, grid_width, (image.height * square_size) + 1);

  /* set x positions of command windows */
  changes.x = LEFT_MARGIN + grid_width + GRID_TO_COMMAND_MARGIN;
  for (i=0;i<N_COMMANDS;i++)
    XConfigureWindow (d, commands[i].window, CWX, &changes);

  /* set x offsets for raster and raster-inverted windows */
  changes.x = LEFT_MARGIN + grid_width + AROUND_RASTER_MARGIN;
  XConfigureWindow (d, raster_window, CWX, &changes);
  XConfigureWindow (d, raster_invert_window, CWX, &changes);
  }

/* OuterWindowDims determines the minimum size for the outer window,
   based on three constraints:  the grid square size, the width of
   the command/raster area, and the minimum height of the
   command/raster area ("right side" of the window).  It is called
   at startup time. */

OuterWindowDims (square_size, right_side_width,
  right_side_bottom, width, height)
  int square_size, right_side_width, right_side_bottom;
  int *width, *height; /* RETURN */
  {
  *width = LEFT_MARGIN + image.width*square_size + 1 + right_side_width;
  *height = TOP_MARGIN + image.height*square_size + 1 + BOTTOM_MARGIN;
  if (*height < right_side_bottom)
    *height = right_side_bottom;
  }

static int DefaultSquareSize (dwidth, dheight, rwidth, rbottom)
    int dwidth, dheight;
    int rwidth, rbottom;
{
    int maxhorz, maxvert, val;

    maxhorz = (dwidth - LEFT_MARGIN - 1 - rwidth) / image.width;
    maxvert = (dheight - TOP_MARGIN - 1 - BOTTOM_MARGIN) / image.height;
    val = (maxhorz < maxvert) ? maxhorz : maxvert;
    if (val > DEFAULT_SQUARE_SIZE) val = DEFAULT_SQUARE_SIZE;
    return val;
}

void ClearOrSetAll(b)
  bit b;  /* 0 for clear, 1 for set */
  {
  register int i;
  register int new = (b ? ~0: 0);
  for (i=0;i<raster_length;i++)
    raster[i] = new;
  changed = TRUE;
  XSetState (d, gc, b ? foreground : background, 0L /* ignored */,
    GXcopy, AllPlanes);
  XFillRectangle (d, grid_window, gc, 0, 0, 
    image.width*square_size+1, image.height*square_size+1);
  RepaintGridLines (b ? e_AgainstForeground : e_AgainstBackground);
  RepaintRaster();
  RepaintRasterInverted();
  if (x_hot_spot != OUT_OF_RANGE)
    HighlightHotSpot();
  }


void ClearOrSetArea(b)
  bit b;  /* 0 for clear, 1 for set */
  {
  int x1, y1, x2, y2;
  register int x, y;
  if (AskUserForArea (&x1, &y1, &x2, &y2))
    return;
  for (x=x1;x<=x2;x++)
    for (y=y1;y<=y2;y++)
      SetRasterBit (raster, x, y, b);
  XSetState (d, gc, b ? foreground : background, 0L /* ignored */,
    GXcopy, AllPlanes);
  XFillRectangle (d, grid_window, gc, x1*square_size+1, y1*square_size+1, 
    (x2-x1+1)*square_size-1, (y2-y1+1)*square_size-1);
  RepaintGridLinesPartially (x1 * square_size, y1 * square_size,
     (x2+1) * square_size, (y2+1) * square_size,
     b ? e_AgainstForeground : e_AgainstBackground, FALSE);
  if (!use_dashed_lines)
     check_box_surroundings(x1, y1, x2, y2);
  if (x_hot_spot >= x1 && x_hot_spot <= x2
    && y_hot_spot >= y1 && y_hot_spot <= y2)
    	HighlightHotSpot();
  changed = TRUE;
  RepaintRaster();
  RepaintRasterInverted();
  x1_square_exed_through = y1_square_exed_through = OUT_OF_RANGE;
  x2_square_exed_through = y2_square_exed_through = OUT_OF_RANGE;
  }


void InvertAll() {
  register int i;
  for (i=0;i<raster_length;i++)
    raster[i] ^= ~0;  /* invert = exclusive or with all 1's */
  changed = TRUE;
  XSetState (d, gc, 1L, 0L, GXinvert, invertplane);
  XFillRectangle (d, grid_window, gc, 0, 0,
    image.width*square_size+1, image.height*square_size+1);
  if (use_dashed_lines)
     RepaintGridLines (e_Invert);
  RepaintRaster();
  RepaintRasterInverted();
  }


void InvertArea() {
  int x1, y1, x2, y2;
  register int x, y;
  if (AskUserForArea (&x1, &y1, &x2, &y2))
    return;
  for (x=x1;x<=x2;x++)
    for (y=y1;y<=y2;y++)
      InvertRasterBit (raster, x, y);
  ExThroughRectangle (x1, y1, x2, y2);  /* wipe out X-outs */
  XSetState (d, gc, 1L, 0L, GXinvert, invertplane);
  XFillRectangle (d, grid_window, gc, x1*square_size+1,  y1*square_size+1,
    (x2-x1+1)*square_size-1,  (y2-y1+1)*square_size-1);
  if (use_dashed_lines)
     RepaintGridLinesPartially (x1 * square_size, y1 * square_size,
	(x2+1) * square_size, (y2+1) * square_size, e_Invert, FALSE);
  else
     check_box_surroundings(x1, y1, x2, y2);
  changed = TRUE;
  RepaintRaster();
  RepaintRasterInverted();
  x1_square_exed_through = y1_square_exed_through = OUT_OF_RANGE;
  x2_square_exed_through = y2_square_exed_through = OUT_OF_RANGE;
  }


void CopyOrMoveArea (what)
  {
  int x1, y1, x2, y2;
  int x1dest, y1dest;
  if (AskUserForArea (&x1, &y1, &x2, &y2))
    return;
  if (AskUserForDest (&x1dest, &y1dest, x2-x1+1, y2-y1+1))
    /* button released outside grid */
    ExThroughRectangle (x1_square_exed_through, y1_square_exed_through,
      x2_square_exed_through, y2_square_exed_through);
  else {
    register int xsrc, ysrc, xdest, ydest;

    if (x_hot_spot != OUT_OF_RANGE)
    	HighlightHotSpot();  /* actually UNhighlight it */

    /* copy whole raster to new raster */
    bcopy (raster, new_raster, raster_length);
    
    if (what == MOVE)
      /* clear source bits in new raster.  this is VERY inefficient.
         sure wish we had BitBlt available in user memory! */
      for (xsrc = x1; xsrc <= x2; xsrc++)
        for (ysrc = y1; ysrc <= y2; ysrc++)
	  SetRasterBit (new_raster, xsrc, ysrc, 0);

    /* copy old source bits to new destination. this is VERY inefficient.
       sure wish we had BitBlt available in user memory! */

    for (xsrc = x1, xdest = x1dest;
      xsrc<=x2 && xdest < image.width; xsrc++, xdest++) 
        for (ysrc = y1, ydest = y1dest;
          ysrc<=y2 && ydest < image.height; ysrc++, ydest++)
	    if (what == OVERLAY) {
		if (GetRasterBit (raster, xsrc, ysrc))
		    SetRasterBit (new_raster, xdest, ydest, 1);
	    } else
		SetRasterBit (new_raster, xdest, ydest, 
		  GetRasterBit (raster, xsrc, ysrc));

    /*
     * switch raster pointers
     */
    image.data = new_raster;
    new_raster = raster;
    raster = image.data;

    if (what == MOVE)
    	RepaintRectangles (x1, y1, x2, y2, x1dest, y1dest);
    else {
	ExThroughRectangle (x1_square_exed_through, y1_square_exed_through,
	    x2_square_exed_through, y2_square_exed_through);
    	RefillGridPartially (x1dest, y1dest, xdest-1, ydest-1, TRUE);
	}

    if (x_hot_spot != OUT_OF_RANGE)
    	HighlightHotSpot();  /* put the hot spot back on the screen */

    RepaintRaster();
    RepaintRasterInverted();
    changed = TRUE;
    x1_square_plus_through = y1_square_plus_through = OUT_OF_RANGE;
    x2_square_plus_through = y2_square_plus_through = OUT_OF_RANGE;
    }
    
  x1_square_exed_through = y1_square_exed_through = OUT_OF_RANGE;
  x2_square_exed_through = y2_square_exed_through = OUT_OF_RANGE;
  }

#define MAX(a,b)	(((a) > (b)) ? (a) : (b))
#define MIN(a,b)	(((a) < (b)) ? (a) : (b))
#define ABS(a)		(((a) >= 0) ? (a) : -(a))
#define CheckSetRasterBit(r,x,y,c)	\
	if ((x) >= 0 && (x) < image.width && (y) >= 0 && (y) < image.height) \
		SetRasterBit(r, x, y, c)

void Line ()
{
	int	i, x1, y1, x2, y2;
	double	dx, dy, x, y, diff;
	boolean	result;

	if (AskUserForPoint(&x1, &y1, 0))
		return;
	result = AskUserForPoint(&x2, &y2, 1);
	ExThroughRectangle (x1_square_exed_through, y1_square_exed_through,
		x2_square_exed_through, y2_square_exed_through);
	PlusThroughRectangle (x1_square_plus_through, y1_square_plus_through,
		x2_square_plus_through, y2_square_plus_through);
	if (result)
	   return;

	if (x_hot_spot != OUT_OF_RANGE)
    	    HighlightHotSpot();  /* actually UNhighlight it */
	dx = x2 - x1;
	dy = y2 - y1;
	x = x1 + 0.5;
	y = y1 + 0.5;
	diff = MAX(ABS(dx), ABS(dy));
	if (diff == 0)
		diff = 0.9;
	dx /= diff;
	dy /= diff;
	for (i = 0; i <= (int)diff; i++) {
		SetRasterBit(raster, (int)x, (int)y, 1);
		x += dx;
		y += dy;
	}
	RefillGridPartially(MIN(x1, x2), MIN(y1, y2), MAX(x1, x2), MAX(y1, y2),
		FALSE);
	changed = TRUE;
	x1_square_exed_through = y1_square_exed_through = OUT_OF_RANGE;
	x2_square_exed_through = y2_square_exed_through = OUT_OF_RANGE;
	x1_square_plus_through = y1_square_plus_through = OUT_OF_RANGE;
	x2_square_plus_through = y2_square_plus_through = OUT_OF_RANGE;
	if (x_hot_spot != OUT_OF_RANGE)
    	    HighlightHotSpot();  /* put the hot spot back on the screen */
	RepaintRaster();
	RepaintRasterInverted();
}

#include <math.h>

void Circle(filled)
{
	int	i, j, x, x1, y1, x2, y2, dx, dy;
	double	rad, half;
	boolean	result;

	if (AskUserForPoint(&x1, &y1, 0))
		return;
	result = AskUserForPoint(&x2, &y2, 1);
	ExThroughRectangle (x1_square_exed_through, y1_square_exed_through,
		x2_square_exed_through, y2_square_exed_through);
	PlusThroughRectangle (x1_square_plus_through, y1_square_plus_through,
		x2_square_plus_through, y2_square_plus_through);
	if (result)
	   return;

	if (x_hot_spot != OUT_OF_RANGE)
    	    HighlightHotSpot();  /* actually UNhighlight it */
	dx = x2 - x1;
	dy = y2 - y1;
	rad = sqrt((double)(dx * dx + dy * dy)) + 0.5;
	if (filled)
		for (i = 0; i <= (int)rad; i++) {
			x = sqrt(rad * rad - i * i);
			for (j = x1 - x; j <= x1 + x; j++) {
				CheckSetRasterBit(raster, j, y1 - i, 1);
				CheckSetRasterBit(raster, j, y1 + i, 1);
			}
		}
	else {
		half = rad * sqrt(2.0)/2;
		for (i = 0; i <= (int)half; i++) {
			x = sqrt(rad * rad - i * i);
			CheckSetRasterBit(raster, x1 - x, y1 - i, 1);
			CheckSetRasterBit(raster, x1 - x, y1 + i, 1);
			CheckSetRasterBit(raster, x1 + x, y1 - i, 1);
			CheckSetRasterBit(raster, x1 + x, y1 + i, 1);
			CheckSetRasterBit(raster, x1 - i, y1 - x, 1);
			CheckSetRasterBit(raster, x1 - i, y1 + x, 1);
			CheckSetRasterBit(raster, x1 + i, y1 - x, 1);
			CheckSetRasterBit(raster, x1 + i, y1 + x, 1);
		}
	}
	RefillGridPartially(x1-(int)rad, y1-(int)rad, 
		x1+(int)rad, y1+(int)rad, FALSE);
	changed = TRUE;
	x1_square_exed_through = y1_square_exed_through = OUT_OF_RANGE;
	x2_square_exed_through = y2_square_exed_through = OUT_OF_RANGE;
	x1_square_plus_through = y1_square_plus_through = OUT_OF_RANGE;
	x2_square_plus_through = y2_square_plus_through = OUT_OF_RANGE;
	if (x_hot_spot != OUT_OF_RANGE)
    	    HighlightHotSpot();  /* put the hot spot back on the screen */
	RepaintRaster();
	RepaintRasterInverted();
}


int flood_minx, flood_miny, flood_maxx, flood_maxy;

void FloodOne (x, y)
    int x, y;
{
    if ( x >= 0 && x < image.width && y >= 0 && y < image.height &&
       GetRasterBit( raster, x, y ) == 0 )
    {
      flood_minx = MIN(flood_minx, x);
      flood_miny = MIN(flood_miny, y);
      flood_maxx = MAX(flood_maxx, x);
      flood_maxy = MAX(flood_maxy, y);
      SetRasterBit(raster, x, y, 1);
      changed = TRUE;
      FloodOne( x + 1, y );
      FloodOne( x, y + 1 );
      FloodOne( x - 1, y );
      FloodOne( x, y - 1 );
    }
}

void FloodFill ()
{
    int       x, y;

    if (AskUserForPoint(&x, &y, 0))
      return;

    if (x_hot_spot != OUT_OF_RANGE)
    	HighlightHotSpot();  /* actually UNhighlight it */
    flood_minx = flood_maxx = x;
    flood_miny = flood_maxy = y;
    FloodOne( x, y );
    RefillGridPartially(flood_minx, flood_miny, flood_maxx, flood_maxy,
      FALSE);
    if (x_hot_spot != OUT_OF_RANGE)
	HighlightHotSpot();  /* put the hot spot back on the screen */
    RepaintRaster();
    RepaintRasterInverted();
  }


void ClearHotSpot() {
    if (x_hot_spot == OUT_OF_RANGE)
    	return;
    HighlightHotSpot();  /* UNhighlight existing hot spot */
    x_hot_spot = y_hot_spot = OUT_OF_RANGE;
    changed = TRUE;
    }

void SetHotSpot() {
    XDefineCursor (d, outer_window, dot);
    XSelectInput (d, outer_window,
       ButtonPressMask | ButtonReleaseMask | StructureNotifyMask);
    	/* so that we can detect button pressed outside grid */

    while (TRUE) {
	XEvent event;
	int x1, y1;
	XNextEvent (d, &event);
	switch (event.type) {

	    case ButtonPress:
	    case MotionNotify:
	    	if ((event.xany.window == grid_window)
		&& !WhatSquare (&event, &x1, &y1)
		&& (x_hot_spot != x1 || y_hot_spot != y1)) {

		    /* UNhighlight old hot spot */
		    if (x_hot_spot != OUT_OF_RANGE)
		    	HighlightHotSpot();  

		    x_hot_spot = x1;
		    y_hot_spot = y1;

		    /* highlight new hot spot */
		    HighlightHotSpot();

		    changed = TRUE;
		    }
		break;  /* keep looping until button is released */

	    case ButtonRelease:
		XDefineCursor (d, outer_window, cross);
		XSelectInput (d, outer_window, StructureNotifyMask);
	    	return;

	    case Expose:
	    case ConfigureNotify:
	    	ProcessEvent (&event);
		break;
		
	    default:
	    	break;  /* just throw it away */
		
	    }
	}
    }

RepaintRectangles (x1, y1, x2, y2, x3, y3)
    int x1, y1; /* first rectangle's top & left */
    int x2, y2; /* first rectangle's bottom & right */
    int x3, y3; /* second rectangle's top & left */
    {
    int x4 = x3 + (x2 - x1);  /* second rectangle's right edge */
    int y4 = y3 + (y2 - y1);  /* second rectangle's bottom edge */

    if (x4 >= image.width) x4 = image.width-1;
    if (y4 >= image.width) y4 = image.height-1;

    /* if first rectangle is right of second, swap "first" and "second" variables */
    if (x1 > x3)
    	{int temp;
#define swap(a,b) {temp = a; a = b; b = temp;}
	swap (x1, x3); swap (y1, y3); swap (x2, x4); swap (y2, y4);
#undef swap
    	}
    
    RefillGridPartially (x1, y1, x2, y2, TRUE);

    if ((x3 > x2) || (max (y1, y3) > min (y2, y4)))
    	/* rectangles don't overlap */
	RefillGridPartially (x3, y3, x4, y4, TRUE);

    else if (y1 < y3) {
	/* second rectangle is below & right of first */
	RefillGridPartially (x2+1, y3, x4, y2, TRUE);
	RefillGridPartially (x3, y2+1, x4, y4, TRUE);
	}

    else {
	/* second rectangle is above & right of first */
	RefillGridPartially (x3, y3, x4, y1-1, TRUE);
	RefillGridPartially (x2+1, y1, x4, y4, TRUE);
	}
    }


/* AskUserForArea returns FALSE if the user has defined a valid area,
   TRUE if the user hasn't (e.g. by clicking outside grid) */

boolean AskUserForArea(px1, py1, px2, py2) 
  int *px1, *py1, *px2, *py2;
  {
  XEvent event;
  int x1, y1, x2, y2;
  boolean result;

  XSelectInput (d, outer_window, ButtonPressMask | StructureNotifyMask);
    /* so that we can detect button pressed outside grid */

  XDefineCursor (d, outer_window, upper_left);
  
  while (TRUE) {
    XNextEvent (d, &event);
    switch (event.type) {
      case ButtonPress:
        if ((event.xany.window != grid_window)
	|| WhatSquare (&event, &x1, &y1)) {
          XDefineCursor (d, outer_window, cross);
	  XSelectInput (d, outer_window, StructureNotifyMask);
          return (TRUE);
	  }
	goto out1;  /* get out of the loop */
      case Expose:
      case ConfigureNotify:
      	ProcessEvent (&event);
      	break;
      default:
      	break;	/* just throw it away */
      }
    }

  out1:
  ExThroughSquare (x1, y1);
  FlushLineBuffer();
  x1_square_exed_through = x2_square_exed_through = x2 = x1;
  y1_square_exed_through = y2_square_exed_through = y2 = y1;
  XDefineCursor (d, outer_window, lower_right);
  
  while (TRUE) {
    XNextEvent (d, &event);
    switch (event.type) {
      case ButtonPress:
      	result = TRUE;
	goto out2;
      case Expose:
      case ConfigureNotify:
      	ProcessEvent (&event);
      	break;
      case MotionNotify:
      case ButtonRelease: {
        int x, y;
      	result = (event.xany.window != grid_window)
            || WhatSquare (&event, &x, &y)  /* mouse outside grid? */
	    || (x < x1) || (y < y1);
      	if (result) {
	  ExThroughRectangle (x1+1, y1, x2, y2);
	  ExThroughRectangle (x1, y1+1, x1, y2);
	  x2 = x2_square_exed_through = x1;
	  y2 = y2_square_exed_through = y1;
	  }
	else if ((x == x2) && (y == y2))
	  ; /* both dimensions the same; do nothing */
	else if ((x > x2) == (y > y2)) {
	  /* both dimensions bigger or smaller */
	  ExThroughRectangle (min(x2,x)+1, y1, max(x2,x), max(y2,y));
	  ExThroughRectangle (x1, min(y2,y)+1, min(x2,x), max(y2,y));
	  x2 = x2_square_exed_through = x;
	  y2 = y2_square_exed_through = y;
	  }
        else {
	  /* one dimension bigger, the other smaller */
	  ExThroughRectangle (min(x2,x)+1, y1, max(x2,x), min(y2,y));
	  ExThroughRectangle (x1, min(y2,y)+1, min(x2,x), max(y2,y));
	  x2 = x2_square_exed_through = x;
	  y2 = y2_square_exed_through = y;
	  }
	if (event.type == ButtonRelease)
	  goto out2;
	break;
	}
      default:
      	break;	/* just throw it away */
      }
    }

  out2:
  XSelectInput (d, outer_window, StructureNotifyMask);
  XDefineCursor (d, outer_window, cross);
  if (result) {
    /* no area properly selected; remove X-outs from display */
    ExThroughRectangle (x1, y1, x2, y2);
    x1_square_exed_through = y1_square_exed_through = OUT_OF_RANGE;
    x2_square_exed_through = y2_square_exed_through = OUT_OF_RANGE;
    }
  else {
    *px1 = x1;
    *px2 = x2;
    *py1 = y1;
    *py2 = y2;
    }
  return (result);
  }

boolean AskUserForDest (px1, py1, width, height)
  int *px1, *py1;
  int width, height;
  {
  XEvent event;
  boolean result;
  int x1_new, y1_new;

  XSelectInput (d, outer_window,
    ButtonPressMask | ButtonReleaseMask | StructureNotifyMask);
    /* so we can detect button action outside grid */
  XDefineCursor (d, outer_window, upper_left);

  while (TRUE) {
    XNextEvent (d, &event);
    switch (event.type) {

      case Expose:
      case ConfigureNotify:
        ProcessEvent (&event);
	break;

      case ButtonPress:
      case MotionNotify: {
	boolean this_window = (event.xany.window == grid_window)
	  && !WhatSquare (&event, &x1_new, &y1_new);

	if (this_window && (x1_new == *px1) && (y1_new == *py1))
	  break;  /* mouse is still in same square as before; do nothing */

        if (x1_square_plus_through != OUT_OF_RANGE)
          PlusThroughRectangle (x1_square_plus_through, y1_square_plus_through,
              x2_square_plus_through, y2_square_plus_through);

	if (this_window) {
	  *px1 = x1_square_plus_through = x1_new;
	  *py1 = y1_square_plus_through = y1_new;
	  x2_square_plus_through = min (x1_new + width, image.width) - 1;
	  y2_square_plus_through = min (y1_new + height, image.height) - 1;
          PlusThroughRectangle (x1_square_plus_through, y1_square_plus_through,
              x2_square_plus_through, y2_square_plus_through);
	  }
	else {
          x1_square_plus_through = y1_square_plus_through = OUT_OF_RANGE;
          x2_square_plus_through = y2_square_plus_through = OUT_OF_RANGE;
	  *px1 = *py1 = OUT_OF_RANGE;
	  }
        break;
	}

      case ButtonRelease: {
        result = (event.xany.window != grid_window)
          || WhatSquare (&event, &x1_new, &y1_new);
	goto out;
	}

      default:
        break;  /* throw it away */
      }
    }

    out:
    if (result || x1_new != *px1 || y1_new != *py1) {
      /* button released outside grid */
      /* or movement by the cursor between MotionNotify and ButtonRelease */
      if (x1_square_plus_through != OUT_OF_RANGE)
        PlusThroughRectangle (x1_square_plus_through, y1_square_plus_through,
          x2_square_plus_through, y2_square_plus_through);
      *px1 = x1_new;
      *py1 = y1_new;
      }

    XSelectInput (d, outer_window, StructureNotifyMask);
    XDefineCursor (d, outer_window, cross);
    return (result);
    }

boolean AskUserForPoint (xp, yp, plus)
  int *xp, *yp;
  {
  XEvent event;
  boolean this_window;

  XSelectInput (d, outer_window,
    ButtonPressMask | ButtonReleaseMask | StructureNotifyMask);
    /* so we can detect button action outside grid */
  XDefineCursor (d, outer_window, dot);

  while (TRUE) {
    XNextEvent (d, &event);
    switch (event.type) {

      case Expose:
      case ConfigureNotify:
        ProcessEvent (&event);
	break;

      case ButtonRelease:
	this_window = (event.xany.window == grid_window)
			      && !WhatSquare (&event, xp, yp);
	if (this_window) {
		if (plus) {
			PlusThroughRectangle (*xp, *yp, *xp, *yp);
			x1_square_plus_through = x2_square_plus_through = *xp;
			y1_square_plus_through = y2_square_plus_through = *yp;
		} else {
			ExThroughRectangle (*xp, *yp, *xp, *yp);
			x1_square_exed_through = x2_square_exed_through = *xp;
			y1_square_exed_through = y2_square_exed_through = *yp;
		}
	}
	goto out;

      default:
        break;  /* throw it away */
      }
    }

    out:
    XSelectInput (d, outer_window, StructureNotifyMask);
    XDefineCursor (d, outer_window, cross);
    return (!this_window);
    }

void DialogInputHandler (event)
  XEvent *event;
  {
  if (event->type == Expose || event->type == ConfigureNotify)
  	ProcessEvent (event);
  }

enum output_error {e_rename, e_write};

#if defined(USG) && !defined(CRAY)
int rename (from, to)
    char *from, *to;
{
    (void) unlink (to);
    if (link (from, to) == 0) {
        unlink (from);
        return 0;
    } else {
        return -1;
    }
}
#endif

/* WriteOutput returns TRUE if output successfully written, FALSE if not */

boolean WriteOutput() {
  FILE *file;
  if (rename (filename, backup_filename) && errno != ENOENT)
    return (HandleOutputError(e_rename));
  file = fopen (filename, "w+");
  if (!file)
    return (HandleOutputError(e_write));
  WriteOutputToFile (file);
  fclose (file);
  changed = FALSE;
  return (TRUE);
  }


/* HandleOutputError returns TRUE if alternate file written, FALSE if not */

int HandleOutputError(e)
  enum output_error e;
  {
  int result;
  static char *strings[] = {"Yes", "No"};
  char msg1[120], msg2[120];
  char *tmp_filename;
  if (e == e_rename)
    sprintf (msg1, "Can't rename %s to %s -- %s",
      filename, backup_filename, sys_errlist[errno]);
  else
    sprintf (msg1, "Can't write on file %s -- %s",
      filename, sys_errlist[errno]);
  tmp_filename = TmpFileName (filename);
  sprintf (msg2, "Should I write output to file %s?", tmp_filename);
  result = dialog (outer_window, font,
     msg1, msg2, strings, 2, DialogInputHandler);

  if (result == 0)  /* "yes" */ {
    filename = tmp_filename;
    free (backup_filename);
    backup_filename = BackupName (filename);
    return (WriteOutput());
    }
  else {  /* "no" */
    free (tmp_filename);
    return (FALSE);
    }
  }

    
void Quit() {
  if (changed) {
    int result;
    static char *strings[3] = {"Yes", "No", "Cancel"};
    result = dialog (outer_window, font,
      "Save changes before quitting?", "", strings, 3, DialogInputHandler);
      
    switch (result) {
      case 0:     /* "yes" */
      	if (WriteOutput())
	  exit(0);
	else return;
      case 1:    /* "no" */
        exit(0);
      default:  /* "cancel" */
      	return;
      }
    }

  exit(0);
  }

HighlightHotSpot() {
  /* Draw a diamond in the hot spot square */
  /* x1 and y1 are the center of the hot spot square */
  register int x1 = x_hot_spot*square_size + square_size/2;
  register int y1 = y_hot_spot*square_size + square_size/2;
  register int radius = square_size/6;
  XPoint points[5];
  points[0].x = points[2].x = points[4].x = x1;
  points[1].x = x1 + radius;
  points[3].x = x1 - radius;
  points[0].y = points[4].y = y1 + radius;
  points[1].y = points[3].y = y1;
  points[2].y = y1 - radius;
  XSetLineAttributes (d, gc, 0, LineSolid, CapNotLast, JoinMiter);
  XSetState (d, gc, 1L, 0L, GXinvert, highlightplane);
  XDrawLines (d, grid_window, gc, points, 5, CoordModeOrigin);
  }

ExThroughRectangle (x1, y1, x2, y2)
  register int x1, y1, x2, y2;
  {
  register int x, y;
  for (x=x1;x<=x2;x++)
    for (y=y1;y<=y2;y++)
      ExThroughSquare (x, y);
  FlushLineBuffer();
  }


ExThroughSquare (x, y)
  register int x, y;
  {
  register int x1 = x*square_size;
  register int y1 = y*square_size;
  LineIntoBuffer (x1+1, y1+1,
    x1+square_size, y1+square_size);
  LineIntoBuffer (x1+square_size-1, y1+1,
    x1, y1+square_size);
  }


PlusThroughRectangle (x1, y1, x2, y2)
  register int x1, y1, x2, y2;
  {
  register int x, y;
  for (x=x1;x<=x2;x++)
    for (y=y1;y<=y2;y++)
      PlusThroughSquare (x, y);
  FlushLineBuffer();
  }

PlusThroughSquare (x, y)
  register int x, y;
  {
  register int x1 = x*square_size;
  register int y1 = y*square_size;
  LineIntoBuffer (x1+square_size/2, y1+1,
    x1+square_size/2, y1+square_size);
  LineIntoBuffer (x1+1, y1+square_size/2,
    x1+square_size, y1+square_size/2);
  }


#define BUFFER_MAXLENGTH 100
static XSegment buffer [BUFFER_MAXLENGTH];
static int buffer_length = 0;

LineIntoBuffer (x1, y1, x2, y2) {
  register XSegment *seg = &buffer[buffer_length];
  seg->x1 = x1;
  seg->y1 = y1;
  seg->x2 = x2;
  seg->y2 = y2;
  if (++buffer_length == BUFFER_MAXLENGTH)
    FlushLineBuffer();
  }
  
FlushLineBuffer () {
  XSetLineAttributes (d, gc, 0, LineSolid, CapNotLast, JoinMiter);
  XSetState (d, gc, 1L, 0L, GXinvert, highlightplane);
  XDrawSegments (d, grid_window, gc, buffer, buffer_length);
  buffer_length = 0;
  }

/*
 * This routine is called only if we are using solid lines.
 * If a bit is changed, the surrounding bits must be checked
 * to see if they are the same color. Whenever this occurs
 * the grid line separating them is redrawn. Otherwise we
 * can have two bits next to each other without a grid line
 * between them.
 */
check_surroundings(x, y, b)
int x, y;
bit b;
{
   bit bb;

   /*
    * is the one to my left the same color
    * and change the line to opposite if so
    */
   if (x > 0 && (bb = GetRasterBit(raster, x-1, y)) == b)
      RepaintGridLinesPartially((x - 1) * square_size, y * square_size,
			(x + 1) * square_size, (y + 1) * square_size,
			b ? e_AgainstForeground : e_AgainstBackground, FALSE);

   /* is the one to my right the same color? */
   if (x < image.width - 1 && (bb = GetRasterBit(raster, x+1, y)) == b)
      RepaintGridLinesPartially(x * square_size, y * square_size,
			(x + 2) * square_size, (y + 1) * square_size,
			b ? e_AgainstForeground : e_AgainstBackground, FALSE);

   /* is the one above me the same color? */
   if (y > 0 && (bb = GetRasterBit(raster, x, y -1)) == b)
      RepaintGridLinesPartially(x * square_size, (y - 1) * square_size,
			(x + 1) * square_size, (y + 1) * square_size,
			b ? e_AgainstForeground : e_AgainstBackground, FALSE);

   /* is the one below me the same color? */
   if (y < image.height - 1 && (bb = GetRasterBit(raster, x, y+1)) == b)
      RepaintGridLinesPartially(x * square_size, y * square_size,
			(x + 1) * square_size, (y + 2) * square_size,
			b ? e_AgainstForeground : e_AgainstBackground, FALSE);

}

check_box_surroundings(x1, y1, x2, y2)
int x1, y1, x2, y2;
{
   bit b;
   bit bb;
   int x, y;

   /*
    * is the one to my left the same color
    * and change the line to opposite if so
    */
   if (x1 > 0) {
      for (x = x1, y = y1; y <= y2; y++) {
         b = GetRasterBit(raster, x, y);
         bb = GetRasterBit(raster, x-1, y);
         if (bb == b)
            RepaintGridLinesPartially((x - 1) * square_size, y * square_size,
			(x + 1) * square_size, (y + 1) * square_size,
			b ? e_AgainstForeground : e_AgainstBackground, FALSE);
      }
   }

   /* is the one to my right the same color? */
   if (x2 < image.width) {
      for (x = x2, y = y1; y <= y2; y++) {
         b = GetRasterBit(raster, x, y);
         bb = GetRasterBit(raster, x+1, y);
         if (bb == b)
            RepaintGridLinesPartially(x * square_size, y * square_size,
			(x + 2) * square_size, (y + 1) * square_size,
			b ? e_AgainstForeground : e_AgainstBackground, FALSE);
      }
   }

   /* is the one above me the same color? */
   if (y1 > 0) {
      for (y = y1, x = x1; x <= x2; x++) {
         b = GetRasterBit(raster, x, y);
         bb = GetRasterBit(raster, x, y-1);
         if (bb == b)
            RepaintGridLinesPartially(x * square_size, (y - 1) * square_size,
			(x + 1) * square_size, (y + 1) * square_size,
			b ? e_AgainstForeground : e_AgainstBackground, FALSE);
      }
   }

   /* is the one below me the same color? */
   if (y2 < image.height - 1) {
      for (y = y2, x = x1; x <= x2; x++) {
         b = GetRasterBit(raster, x, y);
         bb = GetRasterBit(raster, x, y+1);
         if (bb == b)
            RepaintGridLinesPartially(x * square_size, y * square_size,
			(x + 1) * square_size, (y + 2) * square_size,
			b ? e_AgainstForeground : e_AgainstBackground, FALSE);
      }
   }
}
