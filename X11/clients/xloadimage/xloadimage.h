/* xloadimage.h:
 *
 * jim frost 06.21.89
 *
 * Copyright 1989 Jim Frost.  See included file "copyright.h" for complete
 * copyright information.
 */

#include "copyright.h"
#include <stdio.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#ifdef SYSV
#include <string.h>
#ifndef index /* some SysV's do this for you */
#define index strchr
#endif
#ifndef rindex
#define rindex strrchr
#endif
#else
char *index();
char *rindex();
#endif

#include "image.h"
#include "options.h"

/* image name and option structure used when processing arguments
 */

typedef struct {
  char         *name;         /* name of image */
  int           atx, aty;     /* location to load image at */
  unsigned int  bright;       /* brightness multiplier */
  float gamma;                /* display gamma */
  unsigned int  center;       /* true if image is to be centered */
  unsigned int  clipx, clipy; /* area of image to be used */
  unsigned int  clipw, cliph;
  unsigned int  dither;       /* true if image is to be dithered */
  unsigned int  equalize;     /* true if image is to be equalized */
  unsigned int  gray;         /* true if image is to be grayed */
  unsigned int  normalize;    /* true if image is to be normalized */
  unsigned int  colors;       /* max # of colors to use for this image */
  int           rotate;	      /* # degrees to rotate image */
  unsigned int  smooth;       /* true if image is to be smoothed */
  char         *fg, *bg;      /* foreground/background colors if mono image */
  unsigned int  force;        /* true if we should force alloc of new cmap */
  unsigned int  xzoom, yzoom; /* zoom percentages */
} ImageOptions;

#ifndef MAXIMAGES
#define MAXIMAGES BUFSIZ /* max # of images we'll try to load at once */
#endif

/* function declarations
 */

void supportedImageTypes(); /* imagetypes.c */

unsigned int  fullscreen;
unsigned int  onroot;

char *tail(); /* misc.c */
void usage();
void goodImage();
Image *processImage();
int ioErrorHandler();

char *expandPath(); /* path.c */
int findImage();
void listImages();
void loadPathsAndExts();
void showPath();

void imageOnRoot(); /* root.c */

unsigned int sendImageToX(); /* send.c */

Visual *getBestVisual(); /* visual.c */

void cleanUpWindow(); /* window.c */
char imageInWindow();
