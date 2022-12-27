/*
 * mac.c:
 *
 * adapted from code by Patrick Naughton (naughton@sun.soe.clarkson.edu)
 *
 * macin.c
 * Mark Majhor
 * August 1990
 *
 * routines for reading MAC files
 *
 * Copyright 1990 Mark Majhor (see the included file
 * "mrmcpyrght.h" for complete copyright information)
 */
# include <stdio.h>
# include <math.h>
# include <ctype.h>
# include "image.h"
# include "mac.h"

/****
 **
 ** local variables
 **
 ****/

static BYTE file_open = 0;	/* status flags */
static BYTE image_open = 0;

static ZFILE *ins;		/* input stream */

/****
 **
 ** global variables
 **
 ****/

static int  macin_img_width;           /* image width */
static int  macin_img_height;          /* image height */
static int  macin_img_depth;	       /* image depth */
static int  macin_img_planes;	       /* image planes */
static int  macin_img_BPL;	       /* image bytes per line */

/*
 * open MAC image in the input stream; returns MACIN_SUCCESS if
 * successful. (might also return various MACIN_ERR codes.)
 */
static int macin_open_image(s)
ZFILE *s;
{
  BYTE mhdr[MAC_HDR_LEN];
  char *hp;		/* header pointer */
  int c, i, mhsum = 0;

  /* make sure there isn't already a file open */
  if (file_open)
    return(MACIN_ERR_FAO);

  /* remember that we've got this file open */
  file_open = 1;
  ins = s;

  /*
   * the mac paint files that came with xmac had an extra
   * 128 byte header on the front, with a image name in it.
   * true mac paint images don't seem to have this extra
   * header.  The following code tries to figure out what
   * type the image is and read the right amount of file
   * header (512 or 640 bytes).
   */
  /* read in the mac file header */
  hp = (char *) mhdr;
  if (zread(ins, hp, ADD_HDR_LEN) != ADD_HDR_LEN)
    return MACIN_ERR_EOF;

  if (mhdr[0] != MAC_MAGIC)
    return MACIN_ERR_BAD_SD;

  /* Get image name  (if available) */
  if (mhdr[1] != 0) {				/* if name header */
    if (zread(ins, hp, MAC_HDR_LEN) != MAC_HDR_LEN)
      return MACIN_ERR_EOF;
  } else
    /* else read rest of header */
    if (zread(ins, hp, MAC_HDR_LEN - ADD_HDR_LEN) != MAC_HDR_LEN - ADD_HDR_LEN)
      return MACIN_ERR_EOF;

  /* Now set relevant values */
  macin_img_width  = BYTES_LINE * 8;
  macin_img_height = MAX_LINES;
  macin_img_depth  = 1;		/* always monochrome */
  macin_img_planes = 1;		/* always 1 */
  macin_img_BPL    = BYTES_LINE;

  return MACIN_SUCCESS;
}

/*
 * close an open MAC file
 */

static int macin_close_file()
{
  /* make sure there's a file open */
  if (!file_open)
    return MACIN_ERR_NFO;

  /* mark file (and image) as closed */
  file_open  = 0;
  image_open = 0;

  /* done! */
  return MACIN_SUCCESS;
}

/*
 * semi-graceful fatal error mechanism
 */

static macin_fatal(msg)
     char *msg;
{
  printf("Error reading MacPaint file: %s\n", msg);
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
  printf("%s is a %dx%d MacPaint image\n",
    name, macin_img_width, macin_img_height);
}

Image *macLoad(fullname, name, verbose)
     char         *fullname, *name;
     unsigned int  verbose;
{ 
  ZFILE *zf;
  Image *image;
  BYTE *pixptr, ch;
  register int x, y, c, scanLine;
  register unsigned int i, j, k;

  if (! (zf = zopen(fullname)))
    return(NULL);
  if (macin_open_image(zf) != MACIN_SUCCESS) {  /* read image header */
    macin_close_file();
    zclose(zf);
    return(NULL);
  }

  if (verbose)
    tellAboutImage(name);

  image = newBitImage(macin_img_width, macin_img_height);

  pixptr = &(image->data[0]);
  scanLine = 0; k = 0;

  while (scanLine < macin_img_height) {

    ch = (BYTE) zgetc(zf);	/* Count byte */
    i = (unsigned int) ch;
    if (ch < 0x80) {	/* Unpack next (I+1) chars as is */
      for (j = 0; j <= i; j++)
	if (scanLine < macin_img_height) {
	  *pixptr++ = (BYTE) zgetc(zf); k++;
	  if (!(k %= BYTES_LINE)) {
	    scanLine++;
	  }
	}
    } else {	/* Repeat next char (2's comp I) times */
      ch = zgetc(zf);
      for (j = 0; j <= 256 - i; j++)
	if (scanLine < macin_img_height) {
	  *pixptr++ = (BYTE) ch; k++;
	  if (!(k %= BYTES_LINE)) {
	    scanLine++;
	  }
        }
    }
  }
  macin_close_file();
  zclose(zf);
  image->title = dupString(name);
  return(image);
}

unsigned int macIdent(fullname, name)
char *fullname, *name;
{
  ZFILE        *zf;
  unsigned int  ret;

  if (! (zf = zopen(fullname)))
    return(0);
  if (macin_open_image(zf) == MACIN_SUCCESS) {
    tellAboutImage(name);
    ret = 1;
  } else
    ret = 0;
  macin_close_file();
  zclose(zf);
  return(ret);
}
