/* misc.c:
 *
 * miscellaneous funcs
 *
 * jim frost 10.05.89
 *
 * Copyright 1989, 1990 Jim Frost.  See included file "copyright.h" for
 * complete copyright information.
 */

#include "copyright.h"
#include "xloadimage.h"

void usage(name)
     char *name;
{
  printf("Usage: %s [global options] {[image options] image_name ...}\n",
	 tail(name));
  printf("Global options:\n");
  printf("  -onroot               - load image onto root window\n");
  printf("  -border colorname     - border image with this color\n");
  printf("  -default              - set root window to default pattern\n");
  printf("  -display dispname     - destination display\n");
  printf("  -fullscreen           - use entire screen for display\n");
  printf("  -geometry WxH+X+Y     - destination size and location\n");
  printf("  -help                 - print this help message\n");
  printf("  -identify             - identify given images\n");
  printf("  -list                 - list images in path\n");
  printf("  -install              - explicitly install colormap\n");
  printf("  -path                 - show image path for loading\n");
  printf("  -private              - force allocation of private colormap\n");
  printf("  -quiet                - silence is golden\n");
  printf("  -slideshow            - show images in slideshow style\n");
  printf("  -supported            - show supported image types\n");
  printf("  -verbose              - whistle while you work\n");
  printf("  -version              - show version and patchlevel\n");
  printf("  -view                 - view image in a window\n");
  printf("Image_options:\n");
  printf("  -at X,Y               - load image at location\n");
  printf("  -background colorname - background color for bitmap images\n");
  printf("                        - typical display needs 2.0 - 2.5\n");
  printf("  -brighten percentage  - specify brightness multiplier\n");
  printf("  -center               - center image\n");
  printf("  -clip X,Y,W,H         - use clipped portion of image\n");
  printf("  -colors number        - specify maximum number of RGB colors\n");
  printf("  -dither               - dither color image to bitmap image\n");
  printf("  -foreground colorname - foreground color for bitmap images\n");
  printf("  -gamma display_gamma  - specify display gamma - default = 1.0\n");
  printf("  -gray                 - convert to grayscale (also -grey)\n");
  printf("  -halftone             - halftone a color image to bitmap image\n");
  printf("  -name name            - force next argument to be image name\n");
  printf("  -normalize            - convert image to normalized b & w\n");
  printf("  -rotate degrees       - rotate an image by a 90 degree angle\n");
  printf("  -smooth               - smooth a color image\n");
  printf("  -xzoom percentage     - zoom the X axis by a percentage\n");
  printf("  -yzoom percentage     - zoom the Y axis by a percentage\n");
  printf("  -zoom percentage      - zoom the image by a percentage\n");
  exit(1);
}

char *tail(path)
     char *path;
{ int   s;
  char *t;

  t= path;
  for (s= 0; *(path + s) != '\0'; s++)
    if (*(path + s) == '/')
      t= path + s + 1;
  return(t);
}

Image *processImage(disp, scrn, image, options, verbose)
     Display      *disp;
     int           scrn;
     Image        *image;
     ImageOptions *options;
     unsigned int  verbose;
{ Image        *tmpimage;
  XColor        xcolor;
  unsigned int  compressed= 0;
  XWindowAttributes windowAttrs;

  goodImage(image, "processImage");

  /* clip the image if requested
   */

  if ((options->clipx != 0) || (options->clipy != 0) ||
      (options->clipw != 0) || (options->cliph != 0)) {
    if (!options->clipw)
      options->clipw= image->width;
    if (!options->cliph)
      options->cliph= image->height;
    tmpimage= clip(image, options->clipx, options->clipy,
		   (options->clipw ? options->clipw : image->width),
		   (options->cliph ? options->cliph : image->height),
		   verbose);
    freeImage(image);
    image= tmpimage;
  }

  if (options->rotate)
    { tmpimage = rotate(image, options->rotate, verbose);
      freeImage(image);
      image = tmpimage;
    }

  if (onroot && fullscreen)
    { XGetWindowAttributes(disp, XRootWindow(disp, scrn), &windowAttrs);
      options->xzoom = (((float) windowAttrs.width) / 
			((float) image->width)) * 100;
      options->yzoom = (((float) windowAttrs.height) / 
			((float) image->height)) * 100;
    }

  if (options->xzoom || options->yzoom) { /* zoom image */
    if (!options->colors && RGBP(image) &&             /* if the image is to */
	(!options->xzoom && (options->yzoom > 100)) || /* be blown up, */
	(!options->yzoom && (options->xzoom > 100)) || /* compress before */
	(options->xzoom + options->yzoom > 200)) {     /* doing it */
      compress(image, verbose);
      compressed= 1;
    }
    tmpimage= zoom(image, options->xzoom, options->yzoom, verbose);
    freeImage(image);
    image= tmpimage;
  }

  if (options->gray) /* convert image to grayscale */
    gray(image, verbose);

  if (options->normalize) /* normalize image */
    normalize(image, verbose);

  if (options->bright) /* alter image brightness */
    brighten(image, options->bright, verbose);

  if (options->gamma != 1.0) /* do display gamma compensation */
    gammacorrect(image, options->gamma, verbose);

  /* forcibly reduce colormap
   */

  if (options->colors && RGBP(image) && (options->colors < image->rgb.used)) {
    reduce(image, options->colors, verbose);
    image->rgb.size= options->colors; /* lie */
    compressed= 1;
  }

  if (options->dither && (image->depth > 1)) { /* image is to be dithered */
    if (options->dither == 1)
      tmpimage= dither(image, verbose);
    else
      tmpimage= halftone(image, verbose);
    freeImage(image);
    image= tmpimage;
  }
  else if (!compressed)       /* make sure colormap is minimized */
    compress(image, verbose);

  if (options->smooth > 0) { /* image is to be smoothed */
    tmpimage= smooth(image, options->smooth, verbose);
    if (tmpimage != image) {
      freeImage(image);
      image= tmpimage;
    }
  }

  /* set foreground and background colors of mono image
   */

  xcolor.flags= DoRed | DoGreen | DoBlue;
  if ((image->depth == 1) && options->fg) {
    XParseColor(disp, DefaultColormap(disp, scrn), options->fg, &xcolor);
    *(image->rgb.red + 1)= xcolor.red;
    *(image->rgb.green + 1)= xcolor.green;
    *(image->rgb.blue + 1)= xcolor.blue;
  }
  if ((image->depth == 1) && options->bg) {
    XParseColor(disp, DefaultColormap(disp, scrn), options->bg, &xcolor);
    *image->rgb.red= xcolor.red;
    *image->rgb.green= xcolor.green;
    *image->rgb.blue= xcolor.blue;
  }
  return(image);
}

/* this gets called on an I/O error; it really assumes that a KillClient
 * was issued.
 */

/* ARGSUSED */
int ioErrorHandler(disp)
     Display *disp;
{
  exit(0);
}
