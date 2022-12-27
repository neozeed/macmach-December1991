/****************************************************************
 * mac.h:
 *
 * adapted from code by Patrick Naughton (naughton@sun.soe.clarkson.edu)
 *
 * macin.h
 * Mark Majhor
 * August 1990
 *
 * routines for reading MAC files
 *
 * Copyright 1990 Mark Majhor (see the included file
 * "mrmcpyrght.h" for complete copyright information)
 *
 ****************************************************************/

# define MAC_MAGIC	0x0

typedef unsigned char BYTE;	/* 8 bits unsigned		*/

/*
 * macin return codes
 */
#define MACIN_SUCCESS       0   /* success */

#define MACIN_ERR_BAD_SD   -1   /* bad screen descriptor */
#define MACIN_ERR_BAD_SIG  -2   /* bad signature */
#define MACIN_ERR_EOD      -3   /* unexpected end of raster data */
#define MACIN_ERR_EOF      -4   /* unexpected end of input stream */
#define MACIN_ERR_FAO      -5   /* file already open */
#define MACIN_ERR_IAO      -6   /* image already open */
#define MACIN_ERR_NFO      -7   /* no file open */
#define MACIN_ERR_NIO      -8   /* no image open */

static int macin_open_image();
static int macin_close_file();
static int macin_fatal();

#define	MAC_HDR_LEN	512
#define ADD_HDR_LEN	128
#define	MAX_LINES	720
#define	BYTES_LINE	72
