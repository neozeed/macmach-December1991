/* smooth.c:
 *
 * this performs a smoothing convolution using a 3x3 area.
 *
 * jim frost 09.20.90
 *
 * Copyright 1990 Jim Frost.  See included file "copyright.h" for complete
 * copyright information.
 */

#include "copyright.h"
#include "image.h"

/* this stuff is all used to pick the best possible color out of the
 * image's colormap.  it creates a table sorted by the sum of the rgb
 * values.  we then do a binary search to find the colors which are
 * closest to our sum.  we then search those colors to find the best
 * possible color.  this will have a complexity between log n base 2 and
 * n depending on the distribution of colormap sums.  usually it's pretty
 * good.
 */

extern unsigned int squareInit;
extern unsigned long squareTable[];

struct colorarray {
  Pixel         pixel; /* pixel value */
  unsigned long sum;   /* sum of rgb values */
};

static int colorPredicate(c1, c2)
     struct colorarray *c1, *c2;
{
  return(c1->sum < c2->sum ? -1 : 1);
}

static struct colorarray *createColorArray(rgb)
     RGBMap *rgb;
{ struct colorarray *carray;
  Pixel a;

  carray= (struct colorarray *)lmalloc(rgb->used * sizeof(struct colorarray));
  for (a= 0; a < rgb->used; a++) {
    carray[a].pixel= a;
    carray[a].sum= rgb->red[a] + rgb->green[a] + rgb->blue[a];
  }
  qsort((char *)carray, rgb->used, sizeof(struct colorarray),
	colorPredicate);
  return(carray);
}

#define DIST(A, B) ((A) < (B) ? (B) - (A) : (A) - (B))

static Pixel bestColorByRGB(rgb, carray, red, green, blue)
     RGBMap *rgb;
     struct colorarray *carray;
     unsigned long red, green, blue;
{ unsigned long bdist, qdist, sum, sum2;
  Pixel pick, bpick, qpick;
  unsigned int areasize;

  sum= red + green + blue;

  /* binary search to find colors which are close to us
   */

  pick= areasize= rgb->used / 2;
  while ((pick > 0) && (pick < rgb->used - 1)) {
    if ((carray[pick].sum <= sum) && (sum < carray[pick + 1].sum))
      break;
    areasize= (areasize + 1) >> 1;
    if (carray[pick].sum > sum) {
      if (pick > areasize)
	pick -= areasize;
      else
	pick= 0;
    }
    else {
      pick += areasize;
      if (pick >= rgb->used)
	pick= rgb->used - 1;
    }
  }

  /* look at colors that are a little darker than we are
   */

  bdist= 
    squareTable[DIST(rgb->red[carray[pick].pixel], red) >> 1] +
    squareTable[DIST(rgb->green[carray[pick].pixel], green) >> 1] +
    squareTable[DIST(rgb->blue[carray[pick].pixel], blue) >> 1];
  bpick= pick;

  if (pick > 0) {
    qpick= pick - 1;
    sum2= carray[qpick].sum;
    while (carray[qpick].sum == sum2) {
      qdist= 
        squareTable[DIST(rgb->red[carray[qpick].pixel], red) >> 1] +
        squareTable[DIST(rgb->green[carray[qpick].pixel], green) >> 1] +
        squareTable[DIST(rgb->blue[carray[qpick].pixel], blue) >> 1];
      if (qdist < bdist) {
	bpick= qpick;
	bdist= qdist;
	if (!bdist)
	  return(carray[bpick].pixel);
      }
      if (qpick == 0)
	break;
      qpick--;
    }
  }

  if (pick == rgb->used - 1) /* nothing following */
    return(carray[bpick].pixel);

  /* look at colors which are a little lighter than we are
   */

  qpick= pick + 1;
  sum2= carray[qpick].sum;
  do {
    qdist= 
      squareTable[DIST(rgb->red[carray[qpick].pixel], red) >> 1] +
      squareTable[DIST(rgb->green[carray[qpick].pixel], green) >> 1] +
      squareTable[DIST(rgb->blue[carray[qpick].pixel], blue) >> 1];
    if (qdist < bdist) {
      bpick= qpick;
      bdist= qdist;
      if (!bdist)
	return(carray[bpick].pixel);
    }
    qpick++;
  } while ((qpick < rgb->used) && (carray[qpick].sum == sum2));

  return(carray[bpick].pixel);
}

static Image *doSmooth(image)
     Image *image;
{ Image *new;
  int    x, y, x1, y1, linelen, reductions;
  int    xindex[3];
  byte  *yindex[3];
  byte  *srcptr, *destptr;
  Pixel  pixval;
  unsigned long avgred, avggreen, avgblue;
  struct colorarray *carray;

  /* create new image and copy colormap
   */

  new= newRGBImage(image->width, image->height, image->depth);
  new->title= (char *)lmalloc(strlen(image->title) + 12);
  sprintf(new->title, "%s (smoothed)", image->title);
  bcopy(image->rgb.red, new->rgb.red, image->rgb.used * sizeof(Intensity));
  bcopy(image->rgb.green, new->rgb.green, image->rgb.used * sizeof(Intensity));
  bcopy(image->rgb.blue, new->rgb.blue, image->rgb.used * sizeof(Intensity));
  new->rgb.used= image->rgb.used;

  carray= createColorArray(&(new->rgb));

  /* run through image and take a guess as to what the color should
   * actually be.
   */

  for (y= 0; y < image->height; y++) {
    linelen= image->pixlen * image->width;
    destptr= new->data + (y * linelen);
    yindex[1]= image->data + (y * linelen);
    yindex[0]= yindex[1] - (y > 0 ? linelen : 0);
    yindex[2]= yindex[1] + (y < image->height - 1 ? linelen : 0);
    for (x= 0; x < image->width; x++) {
      avgred= avggreen= avgblue= 0;
      xindex[1]= x * image->pixlen;
      xindex[0]= xindex[1] - (x > 0 ? image->pixlen : 0);
      xindex[2]= xindex[1] + (x < image->width - 1 ? image->pixlen : 0);
      for (y1= 0; y1 < 3; y1++) {
	for (x1= 0; x1 < 3; x1++) {
	  pixval= memToVal(yindex[y1] + xindex[x1], image->pixlen);
	  avgred += *(image->rgb.red + pixval);
	  avggreen += *(image->rgb.green + pixval);
	  avgblue += *(image->rgb.blue + pixval);
	}
      }

      /* pick the pixel that's closest to this average and put in image
       */

      avgred= ((avgred + 8) / 9);
      avggreen= ((avggreen + 8) / 9);
      avgblue= ((avgblue + 8) / 9);
      pixval= bestColorByRGB(&(new->rgb), carray, avgred, avggreen, avgblue);
      valToMem(pixval, destptr, new->pixlen);
      destptr += new->pixlen;
    }
  }

  lfree(carray);
  return(new);
}

Image *smooth(image, iterations, verbose)
     Image *image;
     int    verbose;
{ int a;
  Image *new;

  if (!squareInit)
    initSquareTable();

  if (!RGBP(image)) /* no effect unless color image (yet) */
    return(image);

  if (verbose) {
    printf("  Smoothing...");
    fflush(stdout);
  }

  for (a= 0; a < iterations; a++) {
    new= doSmooth(image);
    freeImage(image);
    image= new;
  }

  if (verbose)
    printf("done\n");

  return(image);
}
