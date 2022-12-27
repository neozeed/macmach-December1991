/* mcidas.c:
 *
 * McIDAS areafile support.  contributed by Glenn P. Davis
 * (davis@unidata.ucar.edu).
 */

#include "xloadimage.h"
#include "mcidas.h"

/*
 * convert from little endian to big endian four byte object
 */
static unsigned long
vhtonl(lend)
unsigned long lend ;
{
	unsigned long bend ;
	unsigned char *lp, *bp ;

	lp = ((unsigned char *)&lend) + 3 ;
	bp = (unsigned char *) &bend ;

	*bp++ = *lp-- ;
	*bp++ = *lp-- ;
	*bp++ = *lp-- ;
	*bp = *lp ;

	return(bend) ;
}


int mcidasIdent(fullname, name)
     char *fullname, *name;
{ ZFILE          *zf;
  struct area_dir dir ;
  int             r;

  if (! (zf= zopen(fullname))) {
    perror("mcidasIdent");
    return(0);
  }
  switch (zread(zf, &dir, sizeof(struct area_dir))) {
  case -1:
    perror("mcidasIdent");
    r= 0;
    break;

  case sizeof(struct area_dir):
    if (dir.type != 4 && dir.type != 67108864) {
      r= 0;
      break;
    }
    r= 1;
    break;

  default:
    r= 0;
    break;
  }
  zclose(zf);
  return(r);
}


Image *mcidasLoad(fullname, name, verbose)
     char         *fullname, *name;
     unsigned int  verbose;
{ ZFILE          *zf;
  struct area_dir  dir;
  struct navigation  nav;
  Image          *image;
  unsigned int    y;
  int doswap = 0 ;

  if (! (zf= zopen(fullname))) {
    perror("mcidasLoad");
    return(NULL);
  }
  switch (zread(zf, &dir, sizeof(struct area_dir))) {
  case -1:
    perror("mcidasLoad");
    zclose(zf);
    exit(1);

  case sizeof(struct area_dir):
    if (dir.type != 4) {
      if(dir.type != 67108864) {
        zclose(zf);
        return(NULL) ;
      } else {
	doswap = 1 ;
      }
    }
    break;

  default:
    zclose(zf);
    return(NULL);
  }

  if(doswap) {
    unsigned long *begin ; 
    unsigned long *ulp ;
    begin = (unsigned long *)&dir ;
    for(ulp = begin ; ulp < &begin[AREA_COMMENTS] ; ulp++)
       *ulp = vhtonl(*ulp) ;
     for(ulp = &begin[AREA_CALKEY] ; ulp < &begin[AREA_STYPE] ; ulp++)
        *ulp = vhtonl(*ulp) ;
   }

    if (verbose)
	(void)printf("%s %d %d (%d, %d) (%d, %d)\n",
		mc_sensor(dir.satid),
		dir.idate,
		dir.itime,
		dir.lcor,
		dir.ecor,
		dir.lres,
		dir.eres) ;
  /* skip the nav */
  if( zread(zf,&nav, sizeof(struct navigation)) != sizeof(struct navigation)) {
      zclose(zf);
      return(NULL) ;
  }

  /* get an image to put the data in
   */

   image= newRGBImage(dir.esiz,
		       dir.lsiz,
		       8 * dir.bands);

  /* set up the colormap, linear grey scale
   */

    for (y= 0; y < 255; y++) {
      *(image->rgb.red + y)= 
       *(image->rgb.green + y)=
        *(image->rgb.blue + y)= y * 257 ;
    }
    image->rgb.used= 255 ;

  zread(zf, image->data, dir.esiz * dir.lsiz) ;

  zclose(zf);
  image->title= dupString(name);
  return(image);
}
