/* xloadimage.c:
 *
 * generic image loader for X11
 *
 * jim frost 09.27.89
 *
 * Copyright 1989, 1990 Jim Frost.  See included file "copyright.h" for
 * complete copyright information.
 */

#include "copyright.h"
#include "xloadimage.h"
#include "patchlevel"

extern double atof();

/* options array and definitions.  note that the enum values must be in the
 * same order as the options strings.
 */

char *Options[] = {
  "onroot", /* global options */
  "border",
  "default",
  "display",
  "fullscreen",
  "geometry",
  "help",
  "identify",
  "list",
  "install",
  "path",
  "private",
  "quiet",
  "slideshow",
  "supported",
  "verbose",
  "version",
  "view",

  "at", /* image options */
  "background",
  "brighten",
  "center",
  "clip",
  "colors",
  "dither",
  "foreground",
  "gamma",
  "gray",
  "grey",
  "halftone",
  "name",
  "normalize",
  "rotate",
  "smooth",
  "xzoom",
  "yzoom",
  "zoom",
  NULL
};

enum {

  /* global options
   */

  ONROOT= 0, BORDER, DEFAULT, DISPLAY, FULLSCREEN, GEOMETRY, HELP,
  IDENTIFY, LIST, INSTALL, PATH, PRIVATE, QUIET, SLIDESHOW, SUPPORTED,
  VERBOSE, VER_NUM, VIEW,

  /* local options
   */

  AT, BACKGROUND, BRIGHT, CENTER, CLIP, COLORS, DITHER, FOREGROUND,
  GAMMA, GRAY, GREY, HALFTONE, NAME, NORMALIZE, ROTATE, SMOOTH,
  XZOOM, YZOOM, ZOOM
};

/* if an image loader needs to have our display and screen, it will get
 * them from here.  this is done to keep most of the image routines
 * clean
 */

Display *Disp= NULL;
int      Scrn= 0;

/* used for the -default option.  this is the root weave bitmap with
 * the bits in the order that xloadimage likes.
 */

#define root_weave_width 4
#define root_weave_height 4
static byte root_weave_bits[] = {
  0xe0, 0xb0, 0xd0, 0x70
};

/* the real thing
 */

main(argc, argv)
     int argc;
     char *argv[];
{ char         *border;
  char         *dname;
  unsigned int  identify;
  unsigned int  install;
  unsigned int  private_cmap;
  unsigned int  slideshow;
  unsigned int  verbose;
  unsigned int  set_default;
  Image        *dispimage;      /* image that will be sent to the display */
  Image        *newimage;       /* new image we're loading */
  Display      *disp;           /* display we're sending to */
  int           scrn;           /* screen we're sending to */
  XColor        xcolor;         /* color for border option */
  ImageOptions  images[MAXIMAGES]; /* list of image w/ options to load */
  int           a;
  unsigned int  imagecount;     /* number of images in ImageName array */
  char         *user_geometry;	/* -geometry passed by user */
  unsigned int  winwidth, winheight; /* geometry of image */
  unsigned int  slide_bright= 0; /* options which are propagated to all */
  unsigned int  slide_colors= 0; /* images when in -slideshow mode */
  unsigned int  slide_dither= 0;
  float         slide_gamma= 1.0;
  unsigned int  slide_normalize= 0;
  unsigned int  slide_smooth= 0;
  unsigned int  slide_xzoom= 0;
  unsigned int  slide_yzoom= 0;

  if (argc < 2)
    usage(argv[0]);

  /* defaults and other initial settings.  some of these depend on what
   * our name was when invoked.
   */

  loadPathsAndExts();
  onroot= 0;
  verbose= 1;
  if (!strcmp(tail(argv[0]), "xview")) {
    onroot= 0;
    verbose= 1;
  }
  else if (!strcmp(tail(argv[0]), "xsetbg")) {
    onroot= 1;
    verbose= 0;
  }
  border= NULL;
  dname= NULL;
  fullscreen= 0;
  identify= 0;
  install= 0;
  private_cmap= 0;
  set_default= 0;
  slideshow= 0;
  user_geometry = NULL;
  winwidth= winheight= 0;

  imagecount= 0;
  for (a= 0; a < MAXIMAGES; a++) {
    images[a].name= NULL;
    images[a].atx= images[a].aty= 0;
    images[a].bright= 0;
    images[a].normalize= 0;
    images[a].gamma= 1.0;
    images[a].center= 0;
    images[a].clipx= images[a].clipy= 0;
    images[a].clipw= images[a].cliph= 0;
    images[a].dither= 0;
    images[a].colors= 0;
    images[a].rotate= 0;
    images[a].fg= images[a].bg= NULL;
    images[a].xzoom= images[a].yzoom= 0;
    images[a].smooth= 0;
  }
  for (a= 1; a < argc; a++) {
    switch (optionNumber(argv[a], Options)) {
    case OPT_BADOPT:
      printf("%s: Bad option\n", argv[a]);
      usage(argv[0]);
      /* NOTREACHED */

    case OPT_NOTOPT:
      if (imagecount == MAXIMAGES)
	printf("%s: Too many images (ignoring)\n", argv[++a]);
      else {
	images[imagecount++].name= argv[a];
	if (slideshow && (imagecount < MAXIMAGES)) {
	  images[imagecount].bright= slide_bright;
	  images[imagecount].normalize= slide_normalize;
	  images[imagecount].gamma= slide_gamma;
	  images[imagecount].dither= slide_dither;
	  images[imagecount].colors= slide_colors;
	  images[imagecount].smooth= slide_smooth;
	  images[imagecount].xzoom= slide_xzoom;
	  images[imagecount].yzoom= slide_yzoom;
	}
      }
      break;

    case OPT_SHORTOPT:
      printf("%s: Not enough characters to identify option\n", argv[a]);
      usage(argv[0]);
      /* NOTREACHED */

    /* process options global to everything
     */

    case ONROOT:
      onroot= 1;
      break;

    case BORDER:
      if (argv[++a])
	border= argv[a];
      break;

    case DEFAULT:
      set_default= 1;
      break;

    case DISPLAY:
      if (argv[++a])
	dname= argv[a];
      break;

    case FULLSCREEN:
      fullscreen= 1;
      break;

    case GEOMETRY:
      if (argv[++a])
	user_geometry = argv[a];
      break;

    case HELP:
      usage(argv[0]);
      exit(0);

    case IDENTIFY:
      identify= 1;
      break;

    case LIST:
      listImages();
      exit(0);

    case INSTALL:
      install= 1;
      break;

    case PATH:
      showPath();
      break;

    case PRIVATE:
      private_cmap= 1;
      break;

    case QUIET:
      verbose= 0;
      break;

    case SLIDESHOW:
      slideshow= 1;
      break;

    case SUPPORTED:
      supportedImageTypes();
      break;

    case VERBOSE:
      verbose= 1;
      break;

    case VER_NUM:
      printf("Xloadimage version %s patchlevel %s by Jim Frost\n",
	     VERSION, PATCHLEVEL);
      break;

    case VIEW:
      onroot= 0;
      break;

    /* process options local to an image
     */

    case AT:
      if (!argv[++a])
	break;
      if (sscanf(argv[a], "%d,%d",
		 &images[imagecount].atx, &images[imagecount].aty) != 2) {
	printf("Bad argument to -at\n");
	usage(argv[0]);
	/* NOTREACHED */
      }
      break;

    case BACKGROUND:
      if (argv[++a])
	images[imagecount].bg= argv[a];
      break;

    case NORMALIZE:
      images[imagecount].normalize= 1;
      if (slideshow)
	slide_normalize= images[imagecount].normalize;
      break;

    case BRIGHT:
      if (argv[++a]) {
	images[imagecount].bright= atoi(argv[a]);
	if (slideshow)
	  slide_bright= images[imagecount].bright;
      }
      break;

    case GAMMA:
      if (argv[++a]) {
	images[imagecount].gamma= atof(argv[a]);
	if (slideshow)
	  slide_gamma= images[imagecount].gamma;
      }
      break;

    case GRAY:
    case GREY:
      images[imagecount].gray= 1;
      break;

    case CENTER:
      images[imagecount].center= 1;
      break;

    case CLIP:
      if (!argv[++a])
	break;
      if (sscanf(argv[a], "%d,%d,%d,%d",
		 &images[imagecount].clipx, &images[imagecount].clipy,
		 &images[imagecount].clipw, &images[imagecount].cliph) != 4) {
	printf("Bad argument to -clip\n");
	usage(argv[0]);
	/* NOTREACHED */
      }
      break;

    case COLORS:
      if (!argv[++a])
	break;
      images[imagecount].colors= atoi(argv[a]);
      if (images[imagecount].colors < 2) {
	printf("Argument to -colors is too low (ignored)\n");
	images[imagecount].colors= 0;
      }
      else if (images[imagecount].colors > 65536) {
	printf("Argument to -colors is too high (ignored)\n");
	images[imagecount].colors= 0;
      }
      if (slideshow)
	slide_colors= images[imagecount].colors;
      break;

    case DITHER:
      images[imagecount].dither= 1;
      if (slideshow)
	slide_dither= 1;
      break;

    case FOREGROUND:
      if (argv[++a])
	images[imagecount].fg= argv[a];
      break;

    case HALFTONE:
      images[imagecount].dither= 2;
      if (slideshow)
	slide_dither= 2;
      break;

    case NAME:
      if (imagecount == MAXIMAGES)
	printf("%s: Too many images (ignoring)\n", argv[++a]);
      else
	images[imagecount++].name= argv[++a];
      break;

    case ROTATE:
      if (!argv[++a])
	break;
      images[imagecount].rotate = atoi(argv[a]);
      if ((images[imagecount].rotate % 90) != 0)
	{ printf("Argument to -rotate must be a multiple of 90 (ignored)\n");
	  images[imagecount].rotate = 0;
	}
      else 
	while (images[imagecount].rotate < 0)
	  images[imagecount].rotate += 360;
      break;

    case SMOOTH:
      if (slideshow) {
	slide_smooth++;
	images[imagecount].smooth= slide_smooth;
      }
      else
	images[imagecount].smooth++; /* add a smoothing iteration */
      break;

    case XZOOM:
      if (argv[++a]) {
	if (atoi(argv[a]) < 0) {
	  printf("Zoom argument must be positive (ignored).\n");
	  continue;
	}
	images[imagecount].xzoom= atoi(argv[a]);
	if (slideshow)
	  slide_xzoom= images[imagecount].xzoom;
      }
      break;

    case YZOOM:
      if (argv[++a]) {
	if (atoi(argv[a]) < 0) {
	  printf("Zoom argument must be positive (ignored).\n");
	  continue;
	}
	images[imagecount].yzoom= atoi(argv[a]);
	if (slideshow)
	  slide_yzoom= images[imagecount].yzoom;
      }
      break;

    case ZOOM:
      if (argv[++a]) {
	if (atoi(argv[a]) < 0) {
	  printf("Zoom argument must be positive (ignored).\n");
	  continue;
	}
	images[imagecount].xzoom= images[imagecount].yzoom= atoi(argv[a]);
	if (slideshow)
	  slide_xzoom= slide_yzoom= images[imagecount].xzoom;
      }
      break;

    default:

      /* this should not happen!
       */

      printf("%s: Internal error parsing arguments\n", argv[0]);
      exit(1);
    }
  }

  if (!imagecount && !set_default) /* NO-OP from here on */
    exit(0);

  if (identify) {                    /* identify the named image(s) */
    for (a= 0; a < imagecount; a++)
      identifyImage(images[a].name);
    exit(0);
  }

  /* filter out mutually exclusive flags
   */

  if (onroot && slideshow) {
    printf("\
%s: -onroot and -slideshow are mutually exclusive (-onroot ignored)\n",
	   argv[0]);
    onroot= 0;
  }

  /* start talking to the display
   */

  if (! (Disp= disp= XOpenDisplay(dname))) {
    printf("%s: Cannot open display\n", XDisplayName(dname));
    exit(1);
  }
  Scrn= scrn= DefaultScreen(disp);
  XSetIOErrorHandler(ioErrorHandler);

  /* handle -default option.  this resets the colormap and loads the
   * default root weave.
   */

  if (set_default) {
    byte *old_data;

    dispimage= newBitImage(root_weave_width, root_weave_height);
    old_data= dispimage->data;
    dispimage->data= root_weave_bits;
    imageOnRoot(disp, scrn, dispimage, 0);
    dispimage->data= old_data;
    freeImage(dispimage);
    if (!imagecount) /* all done */
      exit(0);
  }

  dispimage= NULL;

  if (onroot && (winwidth || winheight || images[0].center ||
      images[0].atx || images[0].aty)) {
    if (!winwidth)
	winwidth= DisplayWidth(disp, scrn);
    if (!winheight)
      winheight= DisplayHeight(disp, scrn);
    if (DefaultDepth(disp, scrn) == 1)
      dispimage= newBitImage(winwidth, winheight);
    else {
      dispimage= newRGBImage(winwidth, winheight, DefaultDepth(disp, scrn));
      dispimage->rgb.used= 1;
    }
    *(dispimage->rgb.red)= 65535;   /* default border value is white */
    *(dispimage->rgb.green)= 65535;
    *(dispimage->rgb.blue)= 65535;
    if (border) {
      XParseColor(disp, DefaultColormap(disp, scrn), border, &xcolor);
      *dispimage->rgb.red= xcolor.red;
      *dispimage->rgb.green= xcolor.green;
      *dispimage->rgb.blue= xcolor.blue;
    }

    /* bitmap needs both black and white
     */

    if (DefaultDepth(disp, scrn) == 1) {
	if (*(dispimage->rgb.red)) {
	    *(dispimage->rgb.red + 1)= 0;
	    *(dispimage->rgb.green + 1)= 0;
	    *(dispimage->rgb.blue + 1)= 0;
	}
	else {
	    *(dispimage->rgb.red + 1)= 65535;
	    *(dispimage->rgb.green + 1)= 65535;
	    *(dispimage->rgb.blue + 1)= 65535;
	}
    }
    fill(dispimage, 0, 0, winwidth, winheight, 0);
  }

  /* load in each named image
   */

  for (a= 0; a < imagecount; a++) {
    if (! (newimage= loadImage(images[a].name, verbose)))
      continue;
    if (!images[a].dither &&
	((dispimage && BITMAPP(dispimage)) || (DefaultDepth(disp, scrn) == 1)))
      images[a].dither= 1;
    newimage= processImage(disp, scrn, newimage, &images[a], verbose);
    if (images[a].center && dispimage) {
      images[a].atx= (int)(dispimage->width - newimage->width) / 2;
      images[a].aty= (int)(dispimage->height - newimage->height) / 2;
    }
    if (dispimage) {
      if (! dispimage->title)
	dispimage->title= dupString(newimage->title);
      merge(dispimage, newimage, images[a].atx, images[a].aty, verbose);
      freeImage(newimage);
    }
    else
      dispimage= newimage;
    if (slideshow) {
      switch(imageInWindow(disp, scrn, dispimage, user_geometry,
			   fullscreen, install, private_cmap, slideshow,
			   argc, argv, verbose)) {
      case '\0': /* window got nuked by someone */
	XCloseDisplay(disp);
	exit(1);
      case '\003':
      case 'q':  /* user quit */
	XCloseDisplay(disp);
	exit(0);
      case ' ':
      case 'n':  /* next image */
	break;
      case 'p':  /* previous image */
	if (a > 0)
	  a -= 2;
	else
	  a--;
	break;
      }
      freeImage(dispimage);
      dispimage= NULL;
    }
  }

  if (!slideshow && !dispimage) {
    printf("No images to display\n");
    exit(1);
  }

  if (onroot)
    imageOnRoot(disp, scrn, dispimage, verbose);
  else {
    if (!slideshow)
      imageInWindow(disp, scrn, dispimage, user_geometry,
		    fullscreen, install, private_cmap, slideshow,
		    argc, argv, verbose);
    cleanUpWindow(disp);
  }
  XCloseDisplay(disp);
  exit(0);
}
