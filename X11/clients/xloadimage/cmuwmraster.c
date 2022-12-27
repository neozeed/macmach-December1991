/*
 *
 * handle CMU Window Manager (ITC) raster image type
 *
 * dan lovinger (dl2n+@andrew.cmu.edu) 07.11.90
 *
 * the format is essentially a byte-reversed sun raster w/o encoding
 *
 * Copyright 1989 Jim Frost.  See included file "copyright.h" for complete
 * copyright information.
 */

#include "copyright.h"
#include "image.h"
#include "cmuwmraster.h"

int babble(name, headerp)
char *name;
struct cmuwm_header *headerp;
{
    printf("%s is a %dx%d %d plane CMU WM raster\n",
	   name,
	   memToVal(headerp->width, sizeof(long)),
	   memToVal(headerp->height, sizeof(long)),
	   memToVal(headerp->depth, sizeof(short)));
}

int cmuwmIdent(fullname, name)
char *fullname, *name;
{
    ZFILE *zf;
    struct cmuwm_header header;
    int r;

    if (!(zf = zopen(fullname)))
      {
	  perror("cmuwmIdent");
	  return(0);
      }

    switch (zread(zf, &header, sizeof(struct cmuwm_header)))
      {
      case -1:
	  perror("cmuwmIdent");
	  r =0;
	  break;

      case sizeof(struct cmuwm_header):
	if (memToVal(header.magic, sizeof(long)) != CMUWM_MAGIC)
	  {
	      r = 0;
	      break;
	  }
	  babble(name, &header);
	  r = 1;
	  break;

      default:
	  r = 0;
	  break;
      }

    zclose(zf);

    return r;
}

Image* cmuwmLoad(fullname, name, verbose)
char *fullname, *name;
unsigned int verbose;
{
    ZFILE *zf;
    struct cmuwm_header header;
    Image *image;
    int height, width, row, col, linelen, r;
    byte *lineptr;

    if (!(zf= zopen(fullname))) 
      {
	  perror("cmuwmLoad");
	  return(NULL);
      }

    switch (zread(zf, &header, sizeof(struct cmuwm_header))) 
      {
      case -1:
	  perror("cmuwmLoad");
	  zclose(zf);
	  exit(1);

      case sizeof(struct cmuwm_header):
	  if (memToVal(header.magic, sizeof(long)) != CMUWM_MAGIC)
	    {
		zclose(zf);
		return(NULL);
	    }
	  if (verbose) babble(name, &header);
	  break;

      default:
	  zclose(zf);
	  return(NULL);
      }

    if (memToVal(header.depth, sizeof(short)) != 1)
      {
	  fprintf(stderr,"CMU WM raster %s is of depth %d, must be 1",
		  name,
		  header.depth);
	  return(NULL);
      }

    image = newBitImage(width = memToVal(header.width, sizeof(long)),
			height = memToVal(header.height, sizeof(long)));

    linelen = (width / 8) + (width % 8 ? 1 : 0);
    lineptr = image->data;

    for (row = 0; row < height; row++)
      {
	  r = zread(zf, lineptr, linelen);

	  if (r == -1)
	    {
		perror("cmuwmLoad");
		exit(1);
	    }
	  
	  if (r != linelen)
	    {
		printf("cmuwmLoad: short raster\n");
		exit(1);
	    }

	  for (r = 0; r < linelen; r++)
	    {
		lineptr[r] ^= 0xff;
	    }

	  lineptr += linelen;
      }

    zclose(zf);

    image->title = dupString(name);

    return image;
}

	  



