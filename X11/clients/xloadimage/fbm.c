/*
 * fbm.c:
 *
 * adapted from code by Michael Mauldin, (mlm) at Carnegie-Mellon
 * University, (fbm tools) and Kirk L. Johnson, (tuna@athena.mit.edu),
 * (gif.c).
 *
 * fbmin.c
 * Mark Majhor
 * August 1990
 *
 * routines for reading FBM files
 *
 * Copyright 1990 Mark Majhor (see the included file
 * "mrmcpyrght.h" for complete copyright information)
 */
# include <stdio.h>
# include <math.h>
# include <ctype.h>
# include "image.h"
# include "fbm.h"

/****
 **
 ** local variables
 **
 ****/

static BYTE file_open = 0;	/* status flags */
static BYTE image_open = 0;

static ZFILE *ins;		/* input stream */
static FBMFILEHDR phdr;		/* header structure */

/****
 **
 ** global variables
 **
 ****/

static int  fbmin_img_width;           /* image width */
static int  fbmin_img_height;          /* image height */
static int  fbmin_img_depth;	       /* image depth */
static int  fbmin_img_bits;	       /* color bits */
static int  fbmin_img_rowlen;	       /* length of one row of data */
static int  fbmin_img_plnlen;	       /* length of one plane of data */
static int  fbmin_img_clrlen;	       /* length of the colormap */
static int  fbmin_img_aspect;	       /* image aspect ratio */
static int  fbmin_img_physbits;	       /* physical bits per pixel */
static char *fbmin_img_title;		/* name of image */
static char *fbmin_img_credit;		/* credit for image */

/*
 * open FBM image in the input stream; returns FBMIN_SUCCESS if
 * successful. (might also return various FBMIN_ERR codes.)
 */
static int fbmin_open_image(s)
ZFILE *s;
{
  char *hp;		/* header pointer */
  int c;

  /* make sure there isn't already a file open */
  if (file_open)
    return(FBMIN_ERR_FAO);

  /* remember that we've got this file open */
  file_open = 1;
  ins = s;

  /* read in the fbm file header */
  hp = (char *) &phdr;
  if (zread(ins, hp, sizeof(phdr)) != sizeof(phdr))
    return FBMIN_ERR_EOF;

  if (strncmp(FBM_MAGIC, phdr.magic, sizeof(FBM_MAGIC)) != 0)
    return FBMIN_ERR_BAD_SIG;

  /* Now extract relevant features of FBM file header */
  fbmin_img_width    = atoi(phdr.cols);
  fbmin_img_height   = atoi(phdr.rows);
  fbmin_img_depth    = atoi(phdr.planes);
  fbmin_img_bits     = atoi(phdr.bits);
  fbmin_img_rowlen   = atoi(phdr.rowlen);
  fbmin_img_plnlen   = atoi(phdr.plnlen);
  fbmin_img_clrlen   = atoi(phdr.clrlen);
  fbmin_img_aspect   = atoi(phdr.aspect);
  fbmin_img_physbits = atoi(phdr.physbits);
  fbmin_img_title    = phdr.title;
  fbmin_img_credit   = phdr.credits;

  if (fbmin_image_test() != FBMIN_SUCCESS)
    return FBMIN_ERR_BAD_SD;

  return FBMIN_SUCCESS;
}

/*
 * close an open FBM file
 */

static int fbmin_close_file()
{
  /* make sure there's a file open */
  if (!file_open)
    return FBMIN_ERR_NFO;

  /* mark file (and image) as closed */
  file_open  = 0;
  image_open = 0;

  /* done! */
  return FBMIN_SUCCESS;
}
    
static fbmin_image_test()
{
  if (fbmin_img_width < 1 || fbmin_img_width > 32767) {
    fprintf (stderr, "Invalid width (%d) on input\n", fbmin_img_width);
    return FBMIN_ERR_BAD_SD;
  }

  if (fbmin_img_height < 1 || fbmin_img_height > 32767) {
    fprintf (stderr, "Invalid height (%d) on input\n", fbmin_img_height);
    return (0);
  }

  if (fbmin_img_depth != 1 && fbmin_img_depth != 3) {
    fprintf (stderr, "Invalid number of planes (%d) on input %s\n",
	     fbmin_img_depth, "(must be 1 or 3)");
    return FBMIN_ERR_BAD_SD;
  }

  if (fbmin_img_bits < 1 || fbmin_img_bits > 8) {
    fprintf (stderr, "Invalid number of bits (%d) on input %s\n",
	     fbmin_img_bits, "(must be [1..8])");
    return FBMIN_ERR_BAD_SD;
  }

  if (fbmin_img_physbits != 1 && fbmin_img_physbits != 8) {
    fprintf (stderr, "Invalid number of physbits (%d) on input %s\n",
	     fbmin_img_physbits, "(must be 1 or 8)");
    return FBMIN_ERR_BAD_SD;
  }

  if (fbmin_img_rowlen < 1 || fbmin_img_rowlen > 32767) {
    fprintf (stderr, "Invalid row length (%d) on input\n",
	     fbmin_img_rowlen);
    return FBMIN_ERR_BAD_SD;
  }

  if (fbmin_img_depth > 1 && fbmin_img_plnlen < 1) {
    fprintf (stderr, "Invalid plane length (%d) on input\n",
	     fbmin_img_plnlen);
    return FBMIN_ERR_BAD_SD;
  }

  if (fbmin_img_aspect < 0.01 || fbmin_img_aspect > 100.0) {
    fprintf (stderr, "Invalid aspect ratio %lg on input\n",
	     fbmin_img_aspect);
    return FBMIN_ERR_BAD_SD;
  }
    return FBMIN_SUCCESS;
}

/*
 * semi-graceful fatal error mechanism
 */

static fbmin_fatal(msg)
     char *msg;
{
  printf("Error reading FBM file: %s\n", msg);
  exit(0);
}

/*
 * these are the routines added for interfacing to xloadimage
 */

/*
 * tell someone what the image we're loading is.  this could be a little more
 * descriptive but I don't care
 */

static void tellAboutImage(name)
     char *name;
{
  printf("%s is a %dx%d FBM image with %d colors\n", name,
    fbmin_img_width, fbmin_img_height, fbmin_img_clrlen / 3);
}

Image *fbmLoad(fullname, name, verbose)
     char         *fullname, *name;
     unsigned int  verbose;
{ 
  ZFILE *zf;
  Image *image;
  register int    x, y, c, j, k, mask, bit, ubyte, rowlen, plnlen;
  int	totalBytes, hdrbits;
  unsigned char *pixptr, *scan, *cm;
  extern int Scrn;
  unsigned char *r, *g, *b;

  if (! (zf= zopen(fullname)))
    return(NULL);
  if (fbmin_open_image(zf) != FBMIN_SUCCESS) {  /* read image header */
    fbmin_close_file();
    zclose(zf);
    return(NULL);
  }
  if (verbose)
    tellAboutImage(name);

  image = newRGBImage(fbmin_img_width, fbmin_img_height, fbmin_img_bits);

  /* if image has a local colormap, override global colormap
   */
  if (fbmin_img_clrlen > 0) {
    cm = (unsigned char *) lmalloc(fbmin_img_clrlen);

    if (zread(ins, cm, fbmin_img_clrlen) != fbmin_img_clrlen) {
      fprintf (stderr, "can't read colormap (%d bytes)\n", fbmin_img_clrlen);
      return(NULL);
    }
    /*
     * fbm color map is organized as
     * buf[3][16]
     */
    y = fbmin_img_clrlen / 3;
    r = &cm[0], g = &cm[y], b = &cm[2 * y];
    for (x = 0; x < y; x++, r++, g++, b++) {
      image->rgb.red[x]   = *r << 8;
      image->rgb.green[x] = *g << 8;
      image->rgb.blue[x]  = *b << 8;
    }
    image->rgb.used = y;

  } else
    cm = NULL;

  rowlen = fbmin_img_rowlen;
  plnlen = fbmin_img_plnlen;

  for (k = 0; k < fbmin_img_depth; k++) {
    pixptr = &(image->data[k * plnlen]);

    for (j = 0; j < fbmin_img_height; j++, pixptr += rowlen) {
      if (zread(ins, pixptr, rowlen) != rowlen) {
	printf("%s: Short read within image data\n", fullname);
        exit(1);
      }
    }
  }

  if (cm != NULL)
    lfree(cm);

  fbmin_close_file();
  zclose(zf);
  image->title= dupString(name);
  return(image);
}

unsigned int fbmIdent(fullname, name)
char *fullname, *name;
{
  ZFILE        *zf;
  unsigned int  ret;

  if (! (zf= zopen(fullname)))
    return(0);
  if (fbmin_open_image(zf) == FBMIN_SUCCESS) {
    tellAboutImage(name);
    ret = 1;
  } else
    ret = 0;
  fbmin_close_file();
  zclose(zf);
  return(ret);
}
