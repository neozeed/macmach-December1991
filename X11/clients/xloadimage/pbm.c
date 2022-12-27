/* pbm.c:
 *
 * portable bit map (pbm) format images
 *
 * jim frost 09.27.89
 *
 * patched by W. David Higgins (wdh@mkt.csd.harris.com) to support
 * raw-format PBM files.
 *
 * patched by Ian MacPhedran (macphed@dvinci.usask.ca) to support
 * PGM and PPM files (03-July-1990)
 */

#include "image.h"
#include "hash.h"

static int          IntTable[256];
static unsigned int Initialized= 0;

#define NOTINT  -1
#define COMMENT -2
#define SPACE   -3
#define NEWLINE -4

#define BADREAD    0 /* read error */
#define NOTPBM     1 /* not a pbm file */
#define PBMNORMAL  2 /* pbm normal type file */
#define PBMCOMPACT 3 /* pbm compacty type file */
#define PBMRAWBITS 4 /* pbm raw bits type file */
#define PGMNORMAL  5 /* pgm normal type file */
#define PGMRAWBITS 6 /* pgm raw bytes type file */
#define PPMNORMAL  7 /* ppm normal type file */
#define PPMRAWBITS 8 /* ppm raw bytes type file */

static void initializeTable()
{ unsigned int a;

  for (a= 0; a < 256; a++)
    IntTable[a]= NOTINT;
  IntTable['#']= COMMENT;
  IntTable['\n']= NEWLINE;
  IntTable['\r']= IntTable['\t']= IntTable[' ']= SPACE;
  IntTable['0']= 0;
  IntTable['1']= 1;
  IntTable['2']= 2;
  IntTable['3']= 3;
  IntTable['4']= 4;
  IntTable['5']= 5;
  IntTable['6']= 6;
  IntTable['7']= 7;
  IntTable['8']= 8;
  IntTable['9']= 9;
  Initialized= 1;
}

static int pbmReadChar(zf)
     ZFILE *zf;
{ int c;

  if ((c= zgetc(zf)) == EOF) {
    zclose(zf);
    return(-1);
  }
  if (IntTable[c] == COMMENT)
    do {
      if ((c= zgetc(zf)) == EOF)
	return(-1);
    } while (IntTable[c] != NEWLINE);
  return(c);
}

static int pbmReadInt(zf)
     ZFILE *zf;
{ int c, value;

  for (;;) {
    c= pbmReadChar(zf);
    if (c < 0)
      return(-1);
    if (IntTable[c] >= 0)
      break;
  };

  value= IntTable[c];
  for (;;) {
    c= pbmReadChar(zf);
    if (c < 0)
      return(-1);
    if (IntTable[c] < 0)
      return(value);
    value= (value * 10) + IntTable[c];
  }
}

static int isPBM(zf, name, width, height, maxval, verbose)
     ZFILE        *zf;
     char         *name;
     unsigned int *width, *height, *maxval;
     unsigned int  verbose;
{ unsigned char buf[4];

  if (! Initialized)
    initializeTable();

  if (zread(zf, buf, 2) != 2)
    return(NOTPBM);
  if (memToVal(buf, 2) == memToVal("P1", 2)) {
    if (((*width= pbmReadInt(zf)) < 0) || ((*height= pbmReadInt(zf)) < 0))
      return(NOTPBM);
    *maxval = 1;
    if (verbose)
      printf("%s is a %dx%d PBM image\n", name, *width, *height);
    return(PBMNORMAL);
  }
  if (memToVal(buf, 2) == memToVal("P4", 2)) {
    if (((*width= pbmReadInt(zf)) < 0) || ((*height= pbmReadInt(zf)) < 0))
      return(NOTPBM);
    *maxval = 1;
    if (verbose)
      printf("%s is a %dx%d RawBits PBM image\n", name, *width, *height);
    return(PBMRAWBITS);
  }
  if (memToVal(buf, 2) == 0x2a17) {
    if (zread(zf, buf, 4) != 4)
      return(NOTPBM);
    *width= memToVal(buf, 2);
    *height= memToVal(buf + 2, 2);
    *maxval = 1;
    if (verbose)
      printf("%s is a %dx%d Compact PBM image\n", name, *width, *height);
    return(PBMCOMPACT);
  }
  if (memToVal(buf, 2) == memToVal("P2", 2)) {
    if (((*width= pbmReadInt(zf)) < 0) || ((*height= pbmReadInt(zf)) < 0))
      return(NOTPBM);
    *maxval = pbmReadInt(zf);
    if (verbose)
      printf("%s is a %dx%d PGM image with %d levels\n", name, *width,
	*height, (*maxval+1));
    return(PGMNORMAL);
  }
  if (memToVal(buf, 2) == memToVal("P5", 2)) {
    if (((*width= pbmReadInt(zf)) < 0) || ((*height= pbmReadInt(zf)) < 0))
      return(NOTPBM);
    *maxval = pbmReadInt(zf);
    if (verbose)
      printf("%s is a %dx%d Raw PGM image with %d levels\n", name, *width,
	*height, (*maxval+1));
    return(PGMRAWBITS);
  }
  if (memToVal(buf, 2) == memToVal("P3", 2)) {
    if (((*width= pbmReadInt(zf)) < 0) || ((*height= pbmReadInt(zf)) < 0))
      return(NOTPBM);
    *maxval = pbmReadInt(zf);
    if (verbose)
      printf("%s is a %dx%d PPM image with %d levels\n", name, *width,
	*height, (*maxval+1));
    return(PPMNORMAL);
  }
  if (memToVal(buf, 2) == memToVal("P6", 2)) {
    if (((*width= pbmReadInt(zf)) < 0) || ((*height= pbmReadInt(zf)) < 0))
      return(NOTPBM);
    *maxval = pbmReadInt(zf);
    if (verbose)
      printf("%s is a %dx%d Raw PPM image with %d levels\n", name, *width,
	*height, (*maxval+1));
    return(PPMRAWBITS);
  }
  return(NOTPBM);
}

int pbmIdent(fullname, name)
     char *fullname, *name;
{ ZFILE        *zf;
  unsigned int  width, height, maxval, ret;

  if (! (zf= zopen(fullname, name)))
    return(0);

  ret= isPBM(zf, name, &width, &height, &maxval, (unsigned int)1);
  zclose(zf);
  return(ret != NOTPBM);
}

Image *pbmLoad(fullname, name, verbose)
     char         *fullname, *name;
     unsigned int  verbose;
{ ZFILE        *zf;
  Image        *image;
  unsigned int  x, y;
  unsigned int  width, height, maxval, depth, curmax;
  unsigned int  linelen;
  byte          srcmask, destmask;
  byte         *destptr, *destline;
  int           src, size;
  unsigned int  numbytes, numread;
  hash_list chl;
  Intensity *red,*grn,*blu;

  if (! (zf= zopen(fullname)))
    return(NULL);

  switch (isPBM(zf, name, &width, &height, &maxval, verbose)) {
  case NOTPBM:
    zclose(zf);
    return(NULL);
    
  case PBMNORMAL:
    image= newBitImage(width, height);
    linelen= (width / 8) + (width % 8 ? 1 : 0);
    destline= image->data;
    for (y= 0; y < height; y++) {
      destptr= destline;
      destmask= 0x80;
      for (x= 0; x < width; x++) {
	do {
	  if ((src= pbmReadChar(zf)) < 0) {
	    printf("%s: Short image\n", fullname);
	    zclose(zf);
	    exit(1);
	  }
	  if (IntTable[src] == NOTINT) {
	    printf("%s: Bad image data\n", fullname);
	    zclose(zf);
	    exit(1);
	  }
	} while (IntTable[src] < 0);
	
	switch (IntTable[src]) {
	case 1:
	  *destptr |= destmask;
	case 0:
	  if (! (destmask >>= 1)) {
	    destmask= 0x80;
	    destptr++;
	  }
	  break;
	default:
	  printf("%s: Bad image data\n", fullname);
	  zclose(zf);
	  exit(1);
	}
      }
      destline += linelen;
    }
    break;

  case PBMRAWBITS:
    image= newBitImage(width, height);
    destline= image->data;
    linelen= (width + 7) / 8;
    numbytes= linelen * height;
    srcmask= 0;		/* force initial read */
    numread= 0;
    for (y= 0; y < height; y++) {
      destptr= destline;
      destmask= 0x80;
      if (srcmask != 0x80) {
        srcmask= 0x80;
	if ((numread < numbytes) && ((src= zgetc(zf)) == EOF)) {
	   printf("%s: Short image\n", fullname);
	   zclose(zf);
	   exit(1);
	}
	numread++;
      }
      for (x= 0; x < width; x++) {
	if (src & srcmask)
	  *destptr |= destmask;
	if (! (destmask >>= 1)) {
	  destmask= 0x80;
	  destptr++;
	}
	if (! (srcmask >>= 1)) {
	  srcmask= 0x80;
	  if ((numread < numbytes) && ((src= zgetc(zf)) == EOF)) {
	    printf("%s: Short image\n", fullname);
	    zclose(zf);
	    exit(1);
	  }
	  numread++;
	}
      }
      destline += linelen;
    }
    break;
 
  case PBMCOMPACT:
    image= newBitImage(width, height);
    destline= image->data;
    linelen= (width / 8) + (width % 8 ? 1 : 0);
    srcmask= 0x80;
    destmask= 0x80;
    if ((src= zgetc(zf)) == EOF) {
      printf("%s: Short image\n", fullname);
      zclose(zf);
      exit(1);
    }
    numread= 1;
    numbytes= width * height;
    numbytes= (numbytes / 8) + (numbytes % 8 ? 1 : 0);
    for (y= 0; y < height; y++) {
      destptr= destline;
      destmask= 0x80;
      for (x= 0; x < width; x++) {
	if (src & srcmask)
	  *destptr |= destmask;
	if (! (destmask >>= 1)) {
	  destmask= 0x80;
	  destptr++;
	}
	if (! (srcmask >>= 1)) {
	  srcmask= 0x80;
	  if ((numread < numbytes) && ((src= zgetc(zf)) == EOF)) {
	    printf("%s: Short image\n", fullname);
	    zclose(zf);
	    exit(1);
	  }
	  numread++;
	}
      }
      destline += linelen;
    }
    break;
  case PGMRAWBITS:
    for (depth=0,curmax=1;maxval>curmax;depth++) curmax=curmax*2;
    image = newRGBImage(width, height, depth);
    for (y=0; y<=maxval; y++)
    { /* As in sunraster.c, use simple ramp for grey scale */
	*(image->rgb.red + y) = PM_SCALE(y, maxval, 0xffff);
	*(image->rgb.green + y) = PM_SCALE(y, maxval, 0xffff);
	*(image->rgb.blue + y) = PM_SCALE(y, maxval, 0xffff);
    }
    image->rgb.used = maxval+1;
    destline = image->data;
    size = height*width;
    for (y=0; y<size; y++)
    {
	if ((src = zgetc(zf)) == EOF)
	{
	  printf("%s: Short image\n", fullname);
	  zclose(zf);
	  exit(1);
	}
	else
	{
	  valToMem(src,destline,image->pixlen); destline += image->pixlen;
	}
    }
    break;
  case PGMNORMAL:
    for (depth=0,curmax=1;maxval>curmax;depth++) curmax=curmax*2;
    image = newRGBImage(width, height, depth);
    for (y=0; y<=maxval; y++)
    { /* As in sunraster.c, use simple ramp for grey scale */
	*(image->rgb.red + y) = PM_SCALE(y, maxval, 0xffff);
	*(image->rgb.green + y) = PM_SCALE(y, maxval, 0xffff);
	*(image->rgb.blue + y) = PM_SCALE(y, maxval, 0xffff);
    }
    image->rgb.used = maxval+1;
    destline = image->data;
    size = height*width;
    for (y=0; y<size; y++)
    {
	if ((src = pbmReadInt(zf)) < 0)
	{
	  printf("%s: Short image\n", fullname);
	  zclose(zf);
	  exit(1);
	}
	else
	{
	  valToMem(src,destline,image->pixlen); destline += image->pixlen;
	}
    }
    break;
  case PPMRAWBITS:
    size = height*width;
    red = (Intensity *)lmalloc(size * sizeof(Intensity));
    grn = (Intensity *)lmalloc(size * sizeof(Intensity));
    blu = (Intensity *)lmalloc(size * sizeof(Intensity));
    chl = init_hash();
    for (y=0; y<size; y++)
    {
	if ((src = zgetc(zf)) == EOF)
	{
	  printf("%s: Short image\n", fullname);
	  zclose(zf);
	  exit(1);
	}
	else
	  red[y] = src;
	if ((src = zgetc(zf)) == EOF)
	{
	  printf("%s: Short image\n", fullname);
	  zclose(zf);
	  exit(1);
	}
	else
	  grn[y] = src;
	if ((src = zgetc(zf)) == EOF)
	{
	  printf("%s: Short image\n", fullname);
	  zclose(zf);
	  exit(1);
	}
	else
	  blu[y] = src;
	add_hash(chl, red[y], grn[y], blu[y]);
    }

    y = count_hash(chl); /* Find number of colours in table */
    printf("%d colors found in image",y);
    if (y > 256)
	printf(". You may wish to run ppmquant 256 on it.\n");
    else
	printf(".\n");

    for (depth=0,curmax=1;y>curmax;depth++) curmax=curmax*2;
    image = newRGBImage(width, height, depth);
    image->rgb.used = y;
    fill_map(chl,&(image->rgb),maxval);
    destline = image->data;
    for (x=0; x<size; x++)
    {
	y = find_hash( chl, red[x], grn[x], blu[x]);
	valToMem(y,destline,image->pixlen); destline += image->pixlen;
    }

    free_hash( chl );
    lfree( red ); lfree( grn ); lfree( blu );
    break;
  case PPMNORMAL:
    size = height*width;
    red = (Intensity *)lmalloc(size * sizeof(Intensity));
    grn = (Intensity *)lmalloc(size * sizeof(Intensity));
    blu = (Intensity *)lmalloc(size * sizeof(Intensity));
    chl = init_hash();
    for (y=0; y<size; y++)
    {
	if ((src = pbmReadInt(zf)) == EOF)
	{
	  printf("%s: Short image\n", fullname);
	  zclose(zf);
	  exit(1);
	}
	else
	  red[y] = src;
	if ((src = pbmReadInt(zf)) == EOF)
	{
	  printf("%s: Short image\n", fullname);
	  zclose(zf);
	  exit(1);
	}
	else
	  grn[y] = src;
	if ((src = pbmReadInt(zf)) == EOF)
	{
	  printf("%s: Short image\n", fullname);
	  zclose(zf);
	  exit(1);
	}
	else
	  blu[y] = src;
	add_hash(chl, red[y], grn[y], blu[y]);
    }

    y = count_hash(chl); /* Find number of colours in table */
    printf("%d colors found in image",y);
    if (y > 256)
	printf(". You may wish to run ppmquant 256 on it.\n");
    else
	printf(".\n");

    for (depth=0,curmax=1;y>curmax;depth++) curmax=curmax*2;
    image = newRGBImage(width, height, depth);
    image->rgb.used = y;
    fill_map(chl,&(image->rgb),maxval);
    destline = image->data;
    for (x=0; x<size; x++)
    {
	y = find_hash( chl, red[x], grn[x], blu[x]);
	valToMem(y,destline,image->pixlen); destline += image->pixlen;
    }

    free_hash( chl );
    lfree( red ); lfree( grn ); lfree( blu );
    break;
  }
  image->title= dupString(name);
  zclose(zf);
  return(image);
}
