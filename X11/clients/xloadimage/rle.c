/* 
 * rle - read in a Utah RLE Toolkit type image.
 * 
 * Author:	Graeme Gill
 * Date: 	30/5/90
 * 
 * Bugs - doesn't free up memory used by rle functions.
 *
 */

#undef  DEBUG

#ifdef DEBUG
# define debug(xx)	fprintf(stderr,xx)
#else
# define debug(xx)
#endif


#define MIN(a,b) ( (a)<(b) ? (a) : (b))

#include <math.h>
#include <stdio.h>
#include "image.h"
#include "rle.h"

/* input file stuff */
static int ptype;				/* picture type : */
#define BW_NM	0		/* black and white, no map */
#define BW_M	1		/* black and white, and a map */
#define SC_M	2		/* single colour channel and colour map */
#define C_NM	3		/* full colour, no maps */
#define C_M		4		/* full colour with colour maps */
static rle_pixel **fmaps;		/* file colour maps from buildmap() */
static unsigned char **scan;	/* buffer for input data */
static int x_min;		/* copy of picture x_min */
static int y_min;		/* copy of picture y_min */

/* option stuff (Not functional) */
static float disp_gam = 1.0;	/* default display gamma correction factor */
static int gflag = 0;			/* if user supplies display gamma */
static int iflag=0;			/* user suplied image gamma */
static int lflag=0;			/* user supplied levels */
static float img_gam = 1.0;			/* image gamma */
static int bwflag = 0;			/* black and white flag */
static int gammamap[256];
static int background[3] = {0,0,0};	/* our background colour */

/* stuff for dithering colour pictures */
int colmap[216][3];		/* for dither map */
int magic[16][16];
int modN[256];
int divN[256];

int rleIdent(fullname,name)
char *fullname, *name;
	{
	ZFILE *rlefile;
	int x_len,y_len;
	int rv;

	debug("rleIdent called\n");
	rlefile = zopen(fullname);
	if(!rlefile)
		{
		perror("rleIdent: Unable to open file");
		return(0);
		}
	debug("rleIdent: Opened file ok\n");
	sv_globals.svfb_fd = rlefile;
	rv = rle_get_setup(&sv_globals);
	debug("rleIdent: got setup ok\n");
	zclose(rlefile);
	debug("rleIdent: closed file ok\n");
	switch(rv)
		{
		case -1:
			return 0;		/* Not rle format */
		case 0:
			/* now figure out the picture type */
			switch(sv_globals.sv_ncolors)
				{
				case 0:
					perror("rleIdent: no colour channels to display");
					return(0);
				case 1:
					x_len = sv_globals.sv_xmax - sv_globals.sv_xmin + 1;
					y_len = sv_globals.sv_ymax - sv_globals.sv_ymin + 1;
					printf("%s is a %dx%d,",name,x_len,y_len);
					switch(sv_globals.sv_ncmap)
						{
						case 0:
							/* black and white, no map */
							printf(" 8 bit grey scale image with no map"); 
							break;
						case 1:
							/* black and white with a map */
							printf(" 8 bit grey scale image with map"); 
							break;
						case 3:
							/* single channel encoded colour with decoding map */
							printf(" 8 bit colour image with colour map");
							break;
						default:
							perror(" 8 bit image with an illegal colour map");
							return 0;
						}
					break;
				case 3:
					switch(sv_globals.sv_ncmap)
						{
						case 0:
							printf(" 24 bit colour image with no map");
							break;
						case 3:
							printf(" 24 bit colour image with colour map");
							break;
						default:
							perror(" 24 bit colour image with an illegal colour map");
							return 0;
						}
					break;
				default:
					perror(" image with an illegal number of colour planes");
					return 0;
				}
			return 1;
		default:			/* Some sort of error */
/*			perror("rleIdent");*/
			return 0;
		}
	}


Image *rleLoad(fullname,name,verbose)
char *fullname,*name;
unsigned int verbose;
	{
	int x_len, y_len;
	int i,j;
	ZFILE *rlefile;
	int ncol;			/* number of colors */
	int depth;
	unsigned char *bufp;
	char *v;
	Image *image;
	unsigned char *buf;

	dith_levels = 256;	/* aim for 128 levels of each colour */

	debug("rleLoad called\n");
	rlefile = zopen(fullname);
	if(!rlefile)
		{
		perror("rleLoad: Cannot open input file");
		return(NULL);	
		}
	sv_globals.svfb_fd = rlefile;
	debug("rleLoad: About to call get_setup\n");
	if(rle_get_setup( &sv_globals )) {
		zclose(rlefile);
		return(NULL);	
	}

	debug("rleLoad: get_setup called ok\n");
	if(iflag == 1)	/* -i flag */
		img_gam = 1.0/img_gam;		/* convert to display gamma */

    /* If no image gamma on command line, check comments in file */
    if (!iflag)
		{
		char * v;
		if ( (v = rle_getcom( "image_gamma", &sv_globals )) != NULL )
			{	
			img_gam = atof( v );
			/* Protect against bogus information */
			if ( img_gam == 0.0 )
				img_gam = 1.0;
			else
				img_gam = 1.0 / img_gam;	/* convert to display gamma */
			}
		else if ( (v = rle_getcom( "display_gamma", &sv_globals )) != NULL )
			{
	   		img_gam = atof( v );
		    /* Protect */
		    if ( img_gam == 0.0 )
			img_gam = 1.0;
			}
		}

	x_len = sv_globals.sv_xmax - sv_globals.sv_xmin + 1;
	y_len = sv_globals.sv_ymax - sv_globals.sv_ymin + 1;

	x_min = sv_globals.sv_xmin;
	y_min = sv_globals.sv_ymin;

	/* fix this so that we don't waste space */
	sv_globals.sv_xmax -= sv_globals.sv_xmin;
	sv_globals.sv_xmin = 0;

	/* turn off the alpha channel (don't waste time and space)*/
	sv_globals.sv_alpha = 0;
	SV_CLR_BIT(sv_globals,SV_ALPHA);

	/* for now, force background clear */
	if(sv_globals.sv_background ==1)	/* danger setting */
		{
		debug("Forcing clear of background\n");
		sv_globals.sv_background = 2;
		if(sv_globals.sv_bg_color==0)	/* if none allocated */
			sv_globals.sv_bg_color = background;	/* use this one */
		}

	/* now figure out the picture type */
	switch(sv_globals.sv_ncolors)
		{
		case 0:
			perror("rleLoad: no colour channels to display");
			zclose(rlefile);
			return(NULL);
		case 1:
			switch(sv_globals.sv_ncmap)
				{
				case 0:
					ptype = BW_NM;	/* black and white, no map */
					break;
				case 1:
					ptype = BW_M;	/* black and white with a map */
					break;
				case 3:
					ptype = SC_M;	/* single channel encoded colour with decoding map */
					break;
				default:
					zclose(rlefile);
					perror("rleLoad: Illegal number of maps for one colour channel");
					return(NULL);
				}
			break;
		case 3:
			switch(sv_globals.sv_ncmap)
				{
				case 0:
					ptype = C_NM;	/* colour, no map */
					break;
				case 3:
					ptype = C_M;	/* colour with maps */
					break;
				default:
					perror("rleLoad: Illegal number of maps for colour picture");
					zclose(rlefile);
					return(NULL);
				}
			break;
		default:
			perror("rleLoad: Illegal number of colour channels");
			zclose(rlefile);
			return(NULL);
			break;
		}

	if(verbose)
		{
		printf("rleLoad: %s is a %dx%d,",name,x_len,y_len);
		switch(ptype)
			{
			case BW_NM:
				printf(" 8 bit grey scale image with no map"); 
				break;
			case BW_M:
				printf(" 8 bit grey scale image with map"); 
				break;
			case SC_M:
				printf(" 8 bit colour image with colour map");
				break;
			case C_NM:
				printf(" 24 bit colour image with no map (will dither to 8 bits)");
				break;
			case C_M:
				printf(" 24 bit colour image with colour map (will dither to 8 bits)");
				break;
			}
		printf(", and has an image gama of %4.2f\n",img_gam);
		}

	if(ptype==SC_M)
		{	/* don't mess with the image, but change their colour map a bit if needed */
		disp_gam /= img_gam;	/* amount to change their map */
		img_gam = 1.0;		/*  not to the image coming in */
		}

	/* get hold of the colour maps, and set to undo their images gamma */
	fmaps = buildmap(&sv_globals,sv_globals.sv_ncolors,img_gam);

	/* now we had better sort out the picture data */
	debug("done colour map\n");

	/* rle stufff */
	/* Get space for a full colour scan line */
	scan = (unsigned char **) lmalloc( (sv_globals.sv_ncolors +
				       sv_globals.sv_alpha) *
				      sizeof( unsigned char * ) );
	for ( i = 0; i < sv_globals.sv_ncolors + sv_globals.sv_alpha; i++ )
	    scan[i] = (unsigned char *)lmalloc(x_len);
	if ( sv_globals.sv_alpha )
	    scan++;
	debug("got space for get_row\n");

	depth = 8;		/* We always supply 8bit images */
	image = newRGBImage(x_len,y_len,depth);
	image->title = dupString(name);
	debug("got image structure\n");

	buf = image->data;

	/* If we are going to dither - then create the dither matrix. */
	if(!bwflag && ptype!=SC_M && ptype != BW_NM && ptype != BW_M)
		{
		dith_np2 = 1;		/* allow non-power of 2 dither map size */
		dithermap( 6, disp_gam, colmap, divN, modN, magic );
		}

	debug("About to read image in\n");
	bufp = buf + (y_len-1) * x_len;
	for(j=y_len;j>0;j--,bufp -= x_len)
		{
		rle_getrow(&sv_globals,scan);
		switch(ptype)
			{
			case SC_M:
				memcpy(bufp,&scan[0][0],x_len);
				break;
			case BW_NM:
			case BW_M:
				bw_m_line(bufp,x_len);
				break;
			case C_NM:
			case C_M:
				c_m_line(bufp,x_len,j);
				break;
			}
		}
	debug("Image Read in\n");

	/* Deal with colour maps */
	/* set up our gamma correction */
	make_gamma(disp_gam,gammamap);	/* we'll need it */

	debug("Creating color map\n");
	/* now load an appropriate colour map */
	if(!bwflag && ptype==SC_M)
		{
		/* use their maps  & correct their gamma */
		ncol = 1<<sv_globals.sv_cmaplen;	/* number of entries */
		for(i=0;i<ncol;i++)
			{
			*(image->rgb.red + i) = gammamap[fmaps[0][i]]<<8;
			*(image->rgb.green + i) = gammamap[fmaps[1][i]]<<8;
			*(image->rgb.blue + i) = gammamap[fmaps[2][i]]<<8;
			}
		}
	else if(bwflag || ptype == BW_NM || ptype == BW_M)
		{
		/* load a black and white map (gamma corrected for this display)*/
		ncol = 256;			/* don't know whats been used */
		for(i=0;i<ncol;i++)
			{
			*(image->rgb.red + i) = 
			*(image->rgb.green + i) = 
			*(image->rgb.blue + i) = gammamap[i]<<8;
			}
		}
	else
		{
		/* must be colour, so use dither map (gamma corrected for this display)*/
		ncol = 6*6*6;
		/* Dither map has already been created above */
		for(i = 0; i < ncol; i++)
			{
			*(image->rgb.red + i) = colmap[i][0]<<8;
			*(image->rgb.green + i) = colmap[i][1]<<8;
			*(image->rgb.blue + i) = colmap[i][2]<<8;
			}
		}
	image->rgb.used = ncol;

	zclose(rlefile);
	debug("finished\n");
	return(image);
	}

#define DMAP(v,x,y)	(modN[v]>magic[x][y] ? divN[v] + 1 : divN[v])

		/* run the black and white through its map */
		bw_m_line(dp,number)
		int number;
		register unsigned char *dp;
			{
			register unsigned char *r;
			register int i;

			for(i=number,r= &scan[0][0];i>0;i--,r++,dp++)
				{
				*dp = fmaps[0][*r];
				}
			}

		/* convert a colour line with map to 8 bits per pixel */
		c_m_line(dp,number,line)
		int number,line;
		register unsigned char *dp;
			{
			register unsigned char *r, *g, *b;
			register int i, col, row;

			if(!bwflag)
				{		
				for ( row = line % dith_size, col = x_min % dith_size, i = number, r = &scan[0][0]
				,g= &scan[1][0], b= &scan[2][0];
				i > 0; i--, r++, g++, b++, dp++, col = ((col + 1) % dith_size) )
					{
					*dp = DMAP(fmaps[0][*r], col, row) +
						  DMAP(fmaps[1][*g], col, row) * 6 +
						  DMAP(fmaps[2][*b], col, row) * 36;
					}
				}
			else
				{
				int red,green,blue;
				for (i = number, r= &scan[0][0], g= &scan[1][0]
				,b= &scan[2][0]; i>0;i--,r++,g++,b++,dp++)
					{
					red = fmaps[0][*r];green=fmaps[1][*g];blue = fmaps[2][*b];
					*dp = 0.35* red + 0.55* green + 0.1* blue;
					}
				}
			}

