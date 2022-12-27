/* xwd.c:
 *
 * XWD file reader.  unfortunately the bozo who thought up this format didn't
 * define anything at all that we can use as an identifier or even to tell
 * what kind of machine dumped the format.  what this does is read the
 * header and look at several fields to decide if this *might* be an XWD
 * file and if it is what byte order machine wrote it.
 *
 * jim frost 07.24.90
 *
 * Copyright 1990 Jim Frost.  See included file "copyright.h" for complete
 * copyright information.
 */

#include "copyright.h"
#include "xloadimage.h"
#include "xwd.h"

/* this reads the header and does the magic to determine if it is indeed
 * an XWD file.
 */

static int isXWD(name, zf, header, verbose)
     char      *name;
     ZFILE     *zf;
     XWDHeader *header;
     int        verbose;
{ GenericXWDHeader  gh;
  int               a;

  if (zread(zf, &gh, sizeof(GenericXWDHeader)) != sizeof(GenericXWDHeader))
    return(0);

  /* first try -- see if XWD version number matches in either MSB or LSB order
   */

  if (memToVal(gh.file_version, 4) != XWD_VERSION)
    return(0);

  /* convert fields to fill out header.  things we don't care about
   * are commented out.
   */

  header->header_size= memToVal(gh.header_size, 4);
  header->file_version= memToVal(gh.file_version, 4);
  header->pixmap_format= memToVal(gh.pixmap_format, 4);
  header->pixmap_depth= memToVal(gh.pixmap_depth, 4);
  header->pixmap_width= memToVal(gh.pixmap_width, 4);
  header->pixmap_height= memToVal(gh.pixmap_height, 4);
  header->xoffset= memToVal(gh.xoffset, 4);
  header->byte_order= memToVal(gh.byte_order, 4);
  header->bitmap_unit= memToVal(gh.bitmap_unit, 4);
  header->bitmap_bit_order= memToVal(gh.bitmap_bit_order, 4);
  header->bitmap_pad= memToVal(gh.bitmap_pad, 4);
  header->bits_per_pixel= memToVal(gh.bits_per_pixel, 4);
  header->bytes_per_line= memToVal(gh.bytes_per_line, 4);
  header->visual_class= memToVal(gh.visual_class, 4);
/*header->red_mask= memToVal(gh.red_mask, 4);*/
/*header->green_mask= memToVal(gh.green_mask, 4);*/
/*header->blue_mask= memToVal(gh.blue_mask, 4);*/
/*header->bits_per_rgb= memToVal(gh.bits_per_rgb, 4);*/
  header->colormap_entries= memToVal(gh.colormap_entries, 4);
  header->ncolors= memToVal(gh.ncolors, 4);
/*header->window_width= memToVal(gh.window_width, 4);*/
/*header->window_height= memToVal(gh.window_height, 4);*/
/*header->window_x= memToVal(gh.window_x, 4);*/
/*header->window_y= memToVal(gh.window_y, 4);*/
/*header->window_bdrwidth= memToVal(gh.window_bdrwidth, 4);*/

  /* if header size isn't either 100 or 104 bytes, this isn't an XWD file
   */

  if (header->header_size < sizeof(GenericXWDHeader))
    return(0);

  for (a= header->header_size - sizeof(GenericXWDHeader); a; a--)
    zgetc(zf);

  /* look at a variety of the XImage fields to see if they are sane.  if
   * they are, this passes our tests.
   */

  switch (header->pixmap_format) {
  case XYBitmap:
  case XYPixmap:
  case ZPixmap:
    break;
  default:
    return(0);
  }

  switch (header->visual_class) {
  case StaticGray:
  case GrayScale:
  case StaticColor:
  case PseudoColor:

    /* the following are unsupported but recognized
     */

  case TrueColor:
  case DirectColor:
    break;
  default:
    return(0);
  }

  if (verbose) {
    printf("%s is a %dx%d XWD image in ",
	   name, header->pixmap_width, header->pixmap_height);
    switch (header->pixmap_format) {
    case XYBitmap:
      printf("XYBitmap");
      break;
    case XYPixmap:
      printf("%d bit XYPixmap", header->pixmap_depth);
      break;
    case ZPixmap:
      printf("%d bit ZPixmap", header->pixmap_depth);
      break;
    }
    printf(" format\n");
  }

  /* if it got this far, we're pretty damned certain we've got the right
   * file type and know what order it's in.
   */

  return(1);
}

int xwdIdent(fullname, name)
     char *fullname, *name;
{ ZFILE     *zf;
  XWDHeader  header;
  int ret;

  if (! (zf= zopen(fullname)))
    return(0);
  ret= isXWD(name, zf, &header, 1);
  zclose(zf);
  return(ret);
}

static Image *loadXYBitmap(fullname, zf, header)
     char *fullname;
     ZFILE     *zf;
     XWDHeader  header;
{ Image *image;
  int    dlinelen;       /* length of scan line in data file */
  int    ilinelen;       /* length of line within image structure */
  int    unit;           /* # of bytes in a bitmap unit */
  int    xoffset;        /* xoffset within line */
  int    xunits;         /* # of units across the whole scan line */
  int    trailer;        /* # of bytes in last bitmap unit on a line */
  int    shift;          /* # of bits to shift last byte set */
  int    x, y;           /* horizontal and vertical counters */
  byte  *line;           /* input scan line */
  byte  *dptr, *iptr;    /* image data pointers */
  unsigned long (*loader)(); /* unit loading function */

  image= newBitImage(header.pixmap_width, header.pixmap_height);
  ilinelen= (header.pixmap_width / 8) + (header.pixmap_width % 8 ? 1 : 0);
  if (header.bitmap_unit > 7)     /* supposed to be 8, 16, or 32 but appears */
    unit= header.bitmap_unit / 8; /* to often be the byte count.  this will */
  else                            /* accept either. */
    unit= header.bitmap_unit;
  xoffset= (header.xoffset / (unit * 8)) * unit;
  if (header.bytes_per_line)
    dlinelen= header.bytes_per_line;
  else
    dlinelen= unit * header.pixmap_width;
  xunits= (header.pixmap_width / (unit * 8)) +
    (header.pixmap_width % (unit * 8) ? 1 : 0);
  trailer= unit - ((xunits * unit) - ilinelen);
  xunits--; /* we want to use one less than the actual # of units */
  shift= (unit - trailer) * 8;
  if (header.byte_order == MSBFirst)
    loader= doMemToVal;
  else
    loader= doMemToValLSB;
  line= (byte *)lmalloc(dlinelen);

  for (y= 0; y < header.pixmap_height; y++) {
    if (zread(zf, line, dlinelen) != dlinelen) {
      fprintf(stderr,
	      "%s: Short read while reading data! (returning partial image)\n",
	      fullname);
      lfree(line);
      return(image);
    }
    dptr= line + xoffset;
    iptr= image->data + (y * ilinelen);

    if (header.bitmap_bit_order == LSBFirst)
      flipBits(line, dlinelen);

    for (x= 0; x < xunits; x++) {
      valToMem(loader(dptr, unit), iptr, unit);
      dptr += unit;
      iptr += unit;
    }

    /* take care of last unit on this line
     */

    valToMem(loader(dptr, unit) >> shift, iptr, trailer);
  }

  lfree(line);
  return(image);
}

/* this is a lot like the above function but OR's planes together to
 * build the destination.  1-bit images are handled by XYBitmap.
 */

static Image *loadXYPixmap(fullname, zf, header)
     char *fullname;
     ZFILE *zf;
     XWDHeader header;
{ Image *image;
  int plane;
  int    dlinelen;       /* length of scan line in data file */
  int    ilinelen;       /* length of line within image structure */
  int    unit;           /* # of bytes in a bitmap unit */
  int    unitbits;       /* # of bits in a bitmap unit */
  int    unitmask;       /* mask for current bit within current unit */
  int    xoffset;        /* xoffset within data */
  int    xunits;         /* # of units across the whole scan line */
  int    x, x2, y;       /* horizontal and vertical counters */
  int    index;          /* index within image scan line */
  byte  *line;           /* input scan line */
  byte  *dptr, *iptr;    /* image data pointers */
  unsigned long pixvals; /* bits for pixels in this unit */
  unsigned long mask;
  unsigned long (*loader)(); /* unit loading function */

  image= newRGBImage(header.pixmap_width, header.pixmap_height,
		     header.pixmap_depth);
  ilinelen= image->width * image->pixlen;
  if (header.bitmap_unit > 7)     /* supposed to be 8, 16, or 32 but appears */
    unit= header.bitmap_unit / 8; /* to often be the byte count.  this will */
  else                            /* accept either. */
    unit= header.bitmap_unit;
  unitbits= unit * 8;
  unitmask= 1 << (unitbits - 1);
  xoffset= (header.xoffset / unitbits) * unit;
  if (header.bytes_per_line)
    dlinelen= header.bytes_per_line;
  else
    dlinelen= unit * header.pixmap_width;
  xunits= (header.pixmap_width / (unit * 8)) +
    (header.pixmap_width % (unit * 8) ? 1 : 0);
  if (header.byte_order == MSBFirst)
    loader= doMemToVal;
  else
    loader= doMemToValLSB;
  line= (byte *)lmalloc(dlinelen);

  /* for each plane, load in the bitmap and or it into the image
   */

  for (plane= header.pixmap_depth; plane > 0; plane--) {
    for (y= 0; y < header.pixmap_height; y++) {
      if (zread(zf, line, dlinelen) != dlinelen) {
	fprintf(stderr,
		"%s: Short read while reading data! (returning partial image)\n",
		fullname);
	lfree(line);
	return(image);
      }
      dptr= line + xoffset;
      iptr= image->data + (y * ilinelen);
      index= 0;

      if (header.bitmap_bit_order == LSBFirst)
	flipBits(line, dlinelen);
      
      for (x= 0; x < xunits; x++) {
	pixvals= loader(dptr, unit);
	mask= unitmask;
	for (x2= 0; x2 < unitbits; x2++) {
	  if (pixvals & mask)
	    valToMem(memToVal(iptr + index, image->pixlen) | (1 << plane),
		     iptr + index, image->pixlen);
	  index += image->pixlen;
	  if (index > ilinelen) {
	    x= xunits;
	    break;
	  }
	  if (! (mask >>= 1))
	    mask= unitmask;
	}
	dptr += unit;
      }
    }
  }

  lfree(line);
  return(image);
}

/* this loads a ZPixmap format image.  note that this only supports depths
 * of 4, 8, 16, 24, or 32 bits as does Xlib.  You gotta 6-bit image,
 * you gotta problem.  1-bit images are handled by XYBitmap.
 */

static Image *loadZPixmap(fullname, zf, header)
     char *fullname;
     ZFILE *zf;
     XWDHeader header;
{ Image *image;
  int    dlinelen;       /* length of scan line in data file */
  int    ilinelen;       /* length of scan line in image file */
  int    depth;          /* depth rounded up to 8-bit value */
  int    pixlen;         /* length of pixel in bytes */
  int    x, y;           /* horizontal and vertical counters */
  byte  *line;           /* input scan line */
  byte  *dptr, *iptr;    /* image data pointers */
  unsigned long pixmask; /* bit mask within pixel */
  unsigned long pixel;   /* pixel we're working on */
  unsigned long (*loader)(); /* unit loading function */

  image= newRGBImage(header.pixmap_width, header.pixmap_height,
		     header.pixmap_depth);

  /* for pixmaps that aren't simple depths, we round to a depth of 8.  this
   * is what Xlib does, be it right nor not.
   */

  if ((header.pixmap_depth != 4) && (header.pixmap_depth % 8))
    depth= header.pixmap_depth + 8 - (header.pixmap_depth % 8);
  else
    depth= header.pixmap_depth;

  pixmask= 0xffffffff >> (32 - header.pixmap_depth);
  pixlen= image->pixlen;
  if (header.bytes_per_line)
    dlinelen= header.bytes_per_line;
  else
    dlinelen= depth * header.pixmap_width;
  ilinelen= image->width * image->pixlen;
  if (header.byte_order == MSBFirst)
    loader= doMemToVal;
  else
    loader= doMemToValLSB;

  line= (byte *)lmalloc(dlinelen);

  for (y= 0; y < header.pixmap_height; y++) {
    if (zread(zf, line, dlinelen) != dlinelen) {
      fprintf(stderr,
	      "%s: Short read while reading data! (returning partial image)\n",
	      fullname);
      lfree(line);
      return(image);
    }
    dptr= line;
    iptr= image->data + (y * ilinelen);

    if (header.bitmap_bit_order == LSBFirst)
      flipBits(line, dlinelen);

    for (x= 0; x < header.pixmap_width; x++) {
      switch (depth) {
      case 4:
	pixel= memToVal(dptr, 1);
	if (header.bitmap_bit_order == LSBFirst) { /* nybbles are reversed */
	  valToMem(pixel & 0xf, iptr++, 1);        /* by flipBits */
	  if (++x < header.pixmap_width)
	    valToMem(pixel >> 4, iptr++, 1);
	}
	else {
	  valToMem(pixel >> 4, iptr++, 1);
	  if (++x < header.pixmap_width)
	    valToMem(pixel & 0xf, iptr++, 1);
	}
	break;
      case 8:
	pixel= ((unsigned long)*(dptr++)) & pixmask; /* loader isn't needed */
	valToMem(pixel, iptr++, 1);
	break;
      case 16:
      case 24:
      case 32:
	valToMem(loader(dptr, pixlen) & pixmask, iptr, pixlen);
	dptr += pixlen;
	iptr += pixlen;
	break;
      default:
	fprintf(stderr,
		"%s: ZPixmaps of depth %d are not supported (sorry).\n",
		fullname, header.pixmap_depth);
	exit(1);
      }
    }
  }

  lfree(line);
  return(image);
}

Image *xwdLoad(fullname, name, verbose)
     char *fullname, *name;
     int verbose;
{ ZFILE     *zf;
  XWDHeader  header;
  int        cmaplen;
  XWDColor  *cmap;
  Image     *image;
  int        a;

  if (! (zf= zopen(fullname)))
    return(NULL);
  if (! isXWD(name, zf, &header, verbose)) {
    zclose(zf);
    return(NULL);
  }

  /* complain if we don't understand the visual
   */

  switch (header.visual_class) {
  case StaticGray:
  case GrayScale:
  case StaticColor:
  case PseudoColor:
    break;
  case TrueColor:
  case DirectColor:
    fprintf(stderr, "Unsupported visual type, sorry\n");
    exit(1);
  }

  if ((header.pixmap_width == 0) || (header.pixmap_height == 0)) {
    fprintf(stderr, "Zero-size image -- header might be corrupted.\n");
    exit(1);
  }

  /* read in colormap
   */

  cmaplen= header.ncolors * sizeof(XWDColor);
  cmap= (XWDColor *)lmalloc(cmaplen);
  if (zread(zf, cmap, cmaplen) != cmaplen) {
    fprintf(stderr, "Short read in colormap!\n");
    exit(1);
  }

  /* any depth 1 image is basically a XYBitmap so we fake it here
   */

  if (header.pixmap_depth == 1)
    header.pixmap_format= XYBitmap;

  /* we can't realistically support images of more than depth 16 with the
   * RGB image format so this nukes them for the time being.
   */

  if (header.pixmap_depth > 16) {
    fprintf(stderr,
	    "%s: Sorry, cannot load images deeper than 16 bits (yet)\n",
	    fullname);
    exit(1);
  }

  switch (header.pixmap_format) {
  case XYBitmap:
    image= loadXYBitmap(fullname, zf, header);
    zclose(zf);
    image->title= dupString(name);
    return(image); /* we used to goof w/ the cmap but we gave up */
  case XYPixmap:
    image= loadXYPixmap(fullname, zf, header);
    break;
  case ZPixmap:
    image= loadZPixmap(fullname, zf, header);
    break;
  }
  zclose(zf);
  image->title= dupString(name);

  /* load the colormap.  we should probably use pixval instead of the color
   * number but the value seems pretty system-dependent and most colormaps
   * seem to be just dumped in order.
   */

  image->rgb.used= header.ncolors;
  for (a= 0; a < header.ncolors; a++) {
    image->rgb.red[memToVal(cmap[a].pixel, 4)]= memToVal(cmap[a].red, 2);
    image->rgb.green[memToVal(cmap[a].pixel, 4)]= memToVal(cmap[a].green, 2);
    image->rgb.blue[memToVal(cmap[a].pixel, 4)]= memToVal(cmap[a].blue, 2);
  }

  lfree(cmap);
  return(image);
}
