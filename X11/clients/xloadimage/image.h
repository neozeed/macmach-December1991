/* image.h:
 *
 * portable image type declarations
 *
 * jim frost 10.02.89
 *
 * Copyright 1989 Jim Frost.  See included file "copyright.h" for complete
 * copyright information.
 */

#include "copyright.h"
#include <stdio.h>

typedef unsigned long  Pixel;     /* what X thinks a pixel is */
typedef unsigned short Intensity; /* what X thinks an RGB intensity is */
typedef unsigned char  byte;      /* byte type */

struct cache {
  int           len;
  char          buf[BUFSIZ];
  struct cache *next;
};

typedef struct {
  unsigned int  type;     /* ZIO file type */
  FILE         *stream;   /* file input stream */
  char         *filename; /* filename */
  struct cache *data;     /* data cache */
  struct cache *dataptr;  /* ptr to current cache block */
  int           bufptr;   /* ptr within current cache block */
} ZFILE;

#define ZSTANDARD 0 /* standard file */
#define ZPIPE     1 /* file is a pipe (ie uncompress) */
#define ZSTDIN    2 /* file is stdin */

typedef struct rgbmap {
  unsigned int  size;       /* size of RGB map */
  unsigned int  used;       /* number of colors used in RGB map */
  unsigned int  compressed; /* image uses colormap fully */
  Intensity    *red;        /* color values in X style */
  Intensity    *green;
  Intensity    *blue;
} RGBMap;

/* image structure
 */

typedef struct {
  char         *title;  /* name of image */
  unsigned int  type;   /* type of image */
  RGBMap        rgb;    /* RGB map of image if IRGB type */
  unsigned int  width;  /* width of image in pixels */
  unsigned int  height; /* height of image in pixels */
  unsigned int  depth;  /* depth of image in bits if IRGB type */
  unsigned int  pixlen; /* length of pixel if IRGB type */
  byte         *data;   /* data rounded to full byte for each row */
} Image;

#define IBITMAP 0 /* image is a bitmap */
#define IRGB    1 /* image is RGB */

#define BITMAPP(IMAGE) ((IMAGE)->type == IBITMAP)
#define RGBP(IMAGE)    ((IMAGE)->type == IRGB)

/* special case 1-byte transfers so they're inline
 */

#define memToVal(PTR,LEN)    ((LEN) == 1 ? (unsigned long)(*(PTR)) : \
			      doMemToVal(PTR,LEN))
#define memToValLSB(PTR,LEN) ((LEN) == 1 ? (unsigned long)(*(PTR)) : \
			      doMemToValLSB(PTR,LEN))
#define valToMem(VAL,PTR,LEN)    ((LEN) == 1 ? \
				  (unsigned long)(*(PTR) = (byte)(VAL)) : \
				  doValToMem(VAL,PTR,LEN))
#define valToMemLSB(VAL,PTR,LEN) ((LEN) == 1 ? \
				  (unsigned long)(*(PTR) = (byte)(VAL)) : \
				  (int)doValToMemLSB(VAL,PTR,LEN))

/* function declarations
 */

Image *clip(); /* clip.c */

void brighten(); /* bright.c */
void equalize();
void gray();
void normalize();

void compress(); /* compress.c */

Image *dither(); /* dither.c */

void fill(); /* fill.c */

void fold(); /* fold.c */

Image *halftone(); /* halftone.c */

Image *loadImage(); /* imagetypes.c */
void   identifyImage();
void   goodImage();

ZFILE *zopen(); /* io.c */
int    zread();
int    zgetc();
char  *zgets();
void   zclose();

void merge(); /* merge.c */

char  *dupString(); /* new.c */
Image *newBitImage();
Image *newRGBImage();
void   freeImage();
void   freeImageData();
void   newRGBMapData();
void   freeRGBMapData();
byte  *lcalloc();
byte  *lmalloc();
void   lfree();

void reduceRGBMap(); /* reduce.c */
void reduce();

/* doMemToVal and doMemToValLSB used to be void type but some compilers
 * (particularly the 4.1.1 SunOS compiler) couldn't handle the
 * (void)(thing= value) conversion used in the macros.
 */

unsigned long doMemToVal(); /* value.c */
unsigned long doValToMem();
unsigned long doMemToValLSB();
unsigned long doValToMemLSB();

Image *zoom(); /* zoom.c */
Image *rotate(); /* rotate.c */
Image *smooth(); /* smooth.c */

/* this returns the (approximate) intensity of an RGB triple
 */

#define colorIntensity(R,G,B) \
  (RedIntensity[(R) >> 8] + GreenIntensity[(G) >> 8] + BlueIntensity[(B) >> 8])

extern unsigned short RedIntensity[];
extern unsigned short GreenIntensity[];
extern unsigned short BlueIntensity[];
