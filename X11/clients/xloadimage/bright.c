/* bright.c
 *
 * miscellaneous colormap altering functions
 *
 * jim frost 10.10.89
 *
 * Copyright 1989 Jim Frost.  See included file "copyright.h" for complete
 * copyright information.
 */

#include "copyright.h"
#include "image.h"

/* alter an image's brightness by a given percentage
 */

void brighten(image, percent, verbose)
     Image        *image;
     unsigned int  percent;
     unsigned int  verbose;
{ int          a;
  unsigned int newrgb;
  float        fperc;

  if (! RGBP(image)) /* we're AT&T */
    return;

  if (verbose) {
    printf("  Brightening colormap by %d%%...", percent);
    fflush(stdout);
  }

  fperc= (float)percent / 100.0;
  for (a= 0; a < image->rgb.used; a++) {
    newrgb= *(image->rgb.red + a) * fperc;
    if (newrgb > 65535)
      newrgb= 65535;
    *(image->rgb.red + a)= newrgb;
    newrgb= *(image->rgb.green + a) * fperc;
    if (newrgb > 65535)
      newrgb= 65535;
    *(image->rgb.green + a)= newrgb;
    newrgb= *(image->rgb.blue + a) * fperc;
    if (newrgb > 65535)
      newrgb= 65535;
    *(image->rgb.blue + a)= newrgb;
  }

  if (verbose)
    printf("done\n");
}

void gammacorrect(image, disp_gam, verbose)
     Image        *image;
     float  disp_gam;
     unsigned int  verbose;
{ int          a;
  int gammamap[256];

  if (! RGBP(image)) /* we're AT&T */
    return;

  if (verbose) {
    printf("  Adjusting colormap for display gamma of %4.2f...", disp_gam);
    fflush(stdout);
  }

  make_gamma(disp_gam,gammamap);

  for (a= 0; a < image->rgb.used; a++) {
    *(image->rgb.red + a)= gammamap[(*(image->rgb.red + a))>>8]<<8;
    *(image->rgb.green + a)= gammamap[(*(image->rgb.green + a))>>8]<<8;
    *(image->rgb.blue + a)= gammamap[(*(image->rgb.blue + a))>>8]<<8;
  }

  if (verbose)
    printf("done\n");
}

/* normalize an image.  this was originally written by Michael H. Douglas
 * to normalize into greyscale and modified by jim frost for color.
 */

void normalize(image, verbose)
     Image        *image;
     unsigned int  verbose;
{ int          a;
  Intensity minintensity, maxintensity;
  float factor;

  if (! RGBP(image)) /* we're AT&T */
    return;

  compress(image, verbose);

  if (verbose) {
    printf("  Normalizing image...");
    fflush(stdout);
  }

  /* find range of intensities
   */

  maxintensity = 0;
  minintensity = 65536;
  for (a= 0; a < image->rgb.used; a++) {
    if (image->rgb.red[a] > maxintensity)
      maxintensity = image->rgb.red[a];
    if (image->rgb.red[a] < minintensity)
      minintensity= image->rgb.red[a];
    if (image->rgb.green[a] > maxintensity)
      maxintensity = image->rgb.green[a];
    if (image->rgb.green[a] < minintensity)
      minintensity= image->rgb.green[a];
    if (image->rgb.blue[a] > maxintensity)
      maxintensity = image->rgb.blue[a];
    if (image->rgb.blue[a] < minintensity)
      minintensity= image->rgb.blue[a];
  }

  printf("remapping from %d:%d to 0:65535...", minintensity, maxintensity);
  fflush(stdout);
  factor= 65535.0 / (maxintensity - minintensity);
  for (a= 0; a < image->rgb.used; a++) {
    image->rgb.red[a]= (float)(image->rgb.red[a] - minintensity) * factor;
    image->rgb.green[a]= (float)(image->rgb.green[a] - minintensity) * factor;
    image->rgb.blue[a]= (float)(image->rgb.blue[a] - minintensity) * factor;
#if 0
    newrgb = *(image->rgb.red + a);
    newrgb = ( newrgb - minrgb ) * 65535 / range;
    *(image->rgb.red + a)= newrgb;
    *(image->rgb.green + a)= newrgb;
    *(image->rgb.blue + a)= newrgb;
#endif
  }

  if (verbose)
    printf("done\n");
}

/* convert to grayscale
 */

void gray(image, verbose)
     Image *image;
{ int a;
  Intensity intensity;

  if (!RGBP(image))
    return;

  if (verbose) {
    printf("  Converting image to grayscale...");
    fflush(stdout);
  }
  for (a= 0; a < image->rgb.used; a++) {
    intensity= colorIntensity(image->rgb.red[a],
			      image->rgb.green[a],
			      image->rgb.blue[a]);
    image->rgb.red[a]= intensity;
    image->rgb.green[a]= intensity;
    image->rgb.blue[a]= intensity;
  }
  if (verbose)
    printf("done\n");
}
