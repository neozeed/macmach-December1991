/* rotate.c
 *
 * rotate an image
 *
 * Contributed by Tom Tatlow (tatlow@dash.enet.dec.com)
 */

#include "copyright.h"
#include "image.h"

/* rotate_bitmap()
 * converts an old bitmap bit position into a new one
 */
void rotate_bitmap(num, pos, width, height, new_num, new_pos)
int      num;				/* Source byte number       */
int      pos;				/* Source bit position      */
int    width;				/* Width of source bitmap   */
int   height;				/* Height of source bitmap  */
int *new_num;				/* Destination byte number  */
int *new_pos;				/* Destination bit position */
{
  int   slen;				/* Length of source line      */
  int   dlen;				/* Length of destination line */
  int sx, sy;
  int dx, dy;

  slen = (width / 8) + (width % 8 ? 1 : 0);
  dlen = (height / 8) + (height % 8 ? 1 : 0);
  sy = num / slen;
  sx = ((num - (sy * slen)) * 8) + pos;
  dx = (height - sy) - 1;
  dy = sx;
  *new_num = (dx / 8) + (dy * dlen);
  *new_pos = dx % 8;
}

/* rotate()
 * rotates an image
 */
Image *rotate(simage, rotate, verbose)
Image *simage;				/* Image to rotate             */
int    rotate;				/* Number of degrees to rotate */
{ 
  char buf[BUFSIZ];			/* New title                   */
  Image    *image1;			/* Source image                */
  Image    *image2;			/* Destination image           */
  byte         *sp;			/* Pointer to source data      */
  byte         *dp;			/* Pointer to destination data */
  int     slinelen;			/* Length of source line       */
  int     dlinelen;			/* Length of destination line  */
  int       bit[8];			/* Array of hex values         */
  int         x, y;
  int         i, b;
  int   newx, newy;
  int   newi, newb;

  bit[0] = 128;
  bit[1] =  64;
  bit[2] =  32;
  bit[3] =  16;
  bit[4] =   8;
  bit[5] =   4;
  bit[6] =   2;
  bit[7] =   1;

  goodImage(simage, "rotate");

  if (verbose)
    { printf("  Rotating image by %d degrees...", rotate);
      fflush(stdout);
    }
  sprintf(buf, "%s (rotated by %d degrees)", simage->title, rotate);

  image1 = simage;
  do {
    rotate -= 90;
    switch (image1->type) {
    case IBITMAP:
      image2= newBitImage(image1->height, image1->width);
      for (x= 0; x < image1->rgb.used; x++) {
	*(image2->rgb.red + x)= *(image1->rgb.red + x);
	*(image2->rgb.green + x)= *(image1->rgb.green + x);
	*(image2->rgb.blue + x)= *(image1->rgb.blue + x);
      }
      slinelen= (image1->width / 8) + (image1->width % 8 ? 1 : 0);
      sp = image1->data;
      dp = image2->data;
      for (i = 0; i < (slinelen * image1->height); i++)
	for (b = 0; b < 8; b++)
	  if (sp[i] & bit[b])
	    { rotate_bitmap(i, b, image1->width, image1->height, &newi, &newb);
	      dp[newi] |= bit[newb];
	    }
      break;
      
    case IRGB:
      image2= newRGBImage(image1->height, image1->width, image1->depth);
      for (x= 0; x < image1->rgb.used; x++) {
	*(image2->rgb.red + x)= *(image1->rgb.red + x);
	*(image2->rgb.green + x)= *(image1->rgb.green + x);
	*(image2->rgb.blue + x)= *(image1->rgb.blue + x);
      }
      image2->rgb.used= image1->rgb.used;
      slinelen= image1->width * image1->pixlen;
      dlinelen= image1->height * image1->pixlen;
      sp = image1->data;
      dp = image2->data;
      for (y = 0; y < image1->height; y++)
	for (x = 0; x < image1->width; x++)
	  { newx = ((image1->height - y) - 1);
	    newy = x;
	    for (i = 0; i < image1->pixlen; i++)
	      dp[((newy * dlinelen) + newx + i)] = 
		sp[((y * slinelen) + x + i)];
	  }
      break;
    default:
      printf("rotate: Unsupported image type\n");
      exit(1);
    }
    image1 = image2;
  } while (rotate);
  image1->title= dupString(buf);
  if (verbose)
    printf("done\n");
  return(image1);
}

