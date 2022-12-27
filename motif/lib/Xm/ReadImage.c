#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)ReadImage.c	3.7 91/01/10";
#endif /* lint */
#endif /* REV_INFO */
/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
*  (c) Copyright 1987, 1988, 1989, 1990, HEWLETT-PACKARD COMPANY
*  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
*  ALL RIGHTS RESERVED
*  
*  	THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED
*  AND COPIED ONLY IN ACCORDANCE WITH THE TERMS OF SUCH LICENSE AND
*  WITH THE INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR
*  ANY OTHER COPIES THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE
*  AVAILABLE TO ANY OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF THE
*  SOFTWARE IS HEREBY TRANSFERRED.
*  
*  	THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE WITHOUT
*  NOTICE AND SHOULD NOT BE CONSTRUED AS A COMMITMENT BY OPEN SOFTWARE
*  FOUNDATION, INC. OR ITS THIRD PARTY SUPPLIERS  
*  
*  	OPEN SOFTWARE FOUNDATION, INC. AND ITS THIRD PARTY SUPPLIERS,
*  ASSUME NO RESPONSIBILITY FOR THE USE OR INABILITY TO USE ANY OF ITS
*  SOFTWARE .   OSF SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
*  KIND, AND OSF EXPRESSLY DISCLAIMS ALL IMPLIED WARRANTIES, INCLUDING
*  BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
*  FITNESS FOR A PARTICULAR PURPOSE.
*  
*  Notice:  Notwithstanding any other lease or license that may pertain to,
*  or accompany the delivery of, this computer software, the rights of the
*  Government regarding its use, reproduction and disclosure are as set
*  forth in Section 52.227-19 of the FARS Computer Software-Restricted
*  Rights clause.
*  
*  (c) Copyright 1989, 1990, 1991 Open Software Foundation, Inc.  Unpublished - all
*  rights reserved under the Copyright laws of the United States.
*  
*  RESTRICTED RIGHTS NOTICE:  Use, duplication, or disclosure by the
*  Government is subject to the restrictions as set forth in subparagraph
*  (c)(1)(ii) of the Rights in Technical Data and Computer Software clause
*  at DFARS 52.227-7013.
*  
*  Open Software Foundation, Inc.
*  11 Cambridge Center
*  Cambridge, MA   02142
*  (617)621-8700
*  
*  RESTRICTED RIGHTS LEGEND:  This computer software is submitted with
*  "restricted rights."  Use, duplication or disclosure is subject to the
*  restrictions as set forth in NASA FAR SUP 18-52.227-79 (April 1985)
*  "Commercial Computer Software- Restricted Rights (April 1985)."  Open
*  Software Foundation, Inc., 11 Cambridge Center, Cambridge, MA  02142.  If
*  the contract contains the Clause at 18-52.227-74 "Rights in Data General"
*  then the "Alternate III" clause applies.
*  
*  (c) Copyright 1989, 1990, 1991 Open Software Foundation, Inc.
*  ALL RIGHTS RESERVED 
*  
*  
* Open Software Foundation is a trademark of The Open Software Foundation, Inc.
* OSF is a trademark of Open Software Foundation, Inc.
* OSF/Motif is a trademark of Open Software Foundation, Inc.
* Motif is a trademark of Open Software Foundation, Inc.
* DEC is a registered trademark of Digital Equipment Corporation
* DIGITAL is a registered trademark of Digital Equipment Corporation
* X Window System is a trademark of the Massachusetts Institute of Technology
*
*******************************************************************************
******************************************************************************/
#include <stdio.h>
#include <ctype.h>
#include <Xm/XmP.h>


#define MAX_SIZE 255

/* shared data for the image read/parse logic */

static short hexTable[256];		/* conversion value */
static Boolean initialized = False;	/* easier to fill in at run time */

/*  Table index for the hex values. Initialized once, first time.  */
/*  Used for translation value or delimiter significance lookup.   */

static void initHexTable()
{
    /*
     * We build the table at run time for several reasons:
     *
     *     1.  portable to non-ASCII machines.
     *     2.  still reentrant since we set the init flag after setting table.
     *     3.  easier to extend.
     *     4.  less prone to bugs.
     */
    hexTable['0'] = 0;	hexTable['1'] = 1;
    hexTable['2'] = 2;	hexTable['3'] = 3;
    hexTable['4'] = 4;	hexTable['5'] = 5;
    hexTable['6'] = 6;	hexTable['7'] = 7;
    hexTable['8'] = 8;	hexTable['9'] = 9;
    hexTable['A'] = 10;	hexTable['B'] = 11;
    hexTable['C'] = 12;	hexTable['D'] = 13;
    hexTable['E'] = 14;	hexTable['F'] = 15;
    hexTable['a'] = 10;	hexTable['b'] = 11;
    hexTable['c'] = 12;	hexTable['d'] = 13;
    hexTable['e'] = 14;	hexTable['f'] = 15;

    /* delimiters of significance are flagged w/ negative value */
    hexTable[' '] = -1;	hexTable[','] = -1;
    hexTable['}'] = -1;	hexTable['\n'] = -1;
    hexTable['\t'] = -1;
	
    initialized = True;
}




/************************************************************************
 *
 *  NextInt
 *	Read next hex value in the input stream, return -1 if EOF
 *
 ************************************************************************/

static NextInt (fstream)
FILE *fstream;

{
    int	ch;
    int	value = 0;
    int gotone = 0;
    int done = 0;
    

    /*  Loop, accumulate hex value until find delimiter    */
    /*  skip any initial delimiters found in read stream.  */

    while (!done)
    {
	ch = getc(fstream);

	if (ch == EOF) 
        {
	    value = -1;
	    done++;
	}
        else
        {
	    /* trim high bits, check type and accumulate */

	    ch &= 0xff;

	    if (isascii(ch) && isxdigit(ch)) 
            {
		value = (value << 4) + hexTable[ch];
		gotone++;
	    }
            else if ((hexTable[ch]) < 0 && gotone)
	        done++;
	}
    }
    return value;
}




/************************************************************************
 *
 *  ReadBitmapDataFromFile
 *	The data returned by the following routine is always in left-most 
 *	byte first and left-most bit first.  If it doesn't return 
 *	BitmapSuccess then its arguments won't have been touched.
 *
 ************************************************************************/

static Boolean ReadBitmapDataFromFile (filename, width, height, data)
char *filename;
unsigned int *width, *height;	/* RETURNED */
unsigned char **data;		/* RETURNED */

{
    FILE *fstream;			/* handle on file  */
    unsigned char *new_data = NULL;	/* working variable */
    char line[MAX_SIZE];		/* input line from file */
    int size;				/* number of bytes of data */
    char name_and_type[MAX_SIZE];	/* an input line */
    char *type;				/* for parsing */
    int value;				/* from an input line */
    int version10p;			/* boolean, old format */
    int padding;			/* to handle alignment */
    int bytes_per_line;			/* per scanline of data */
    unsigned int ww = 0;		/* width */
    unsigned int hh = 0;		/* height */


    /* first time initialization */

    if (initialized == False) initHexTable();

    if ((fstream = fopen(filename, "r")) == NULL) 
	return (False);

    while (fgets(line, MAX_SIZE, fstream)) 
    {
	if (strlen(line) == MAX_SIZE - 1)
        {
           if (new_data) free (new_data);
           fclose (fstream);
	   return (False);
        }

	if (sscanf(line, "#define %s %d",name_and_type, &value) == 2) 
        {
	    if (!(type = rindex (name_and_type, '_')))
	      type = name_and_type;
	    else
	      type++;

	    if (!strcmp("width", type))
	      ww = (unsigned int) value;

	    if (!strcmp("height", type))
	      hh = (unsigned int) value;

	    if (!strcmp("hot", type)) 
            {
		if (type-- == name_and_type || type-- == name_and_type)
		  continue;
	    }
	    continue;
	}
    
	if (sscanf(line, "static short %s = {", name_and_type) == 1)
	  version10p = 1;
	else if (sscanf(line,"static unsigned char %s = {",name_and_type) == 1)
	  version10p = 0;
	else if (sscanf(line, "static char %s = {", name_and_type) == 1)
	  version10p = 0;
	else
	  continue;

	if (!(type = rindex(name_and_type, '_')))
	  type = name_and_type;
	else
	  type++;

	if (strcmp("bits[]", type))
	  continue;
    
	if (!ww || !hh)
        {
           if (new_data) free (new_data);
           fclose (fstream);
	   return (False);
        }

	if ((ww % 16) && ((ww % 16) < 9) && version10p)
	  padding = 1;
	else
	  padding = 0;

	bytes_per_line = (ww+7)/8 + padding;

	size = bytes_per_line * hh;
	new_data = (unsigned char *) XtMalloc ((unsigned int) size);

	if (version10p) 
        {
	    unsigned char *ptr;
	    int bytes;

	    for (bytes=0, ptr=new_data; bytes<size; (bytes += 2)) 
            {
		if ((value = NextInt(fstream)) < 0)
                {
                   if (new_data) XtFree (new_data);
                   fclose (fstream);
                   return (False);
                }

		*(ptr++) = value;
		if (!padding || ((bytes+2) % bytes_per_line))
		  *(ptr++) = value >> 8;
	    }
	} 
        else
        {
	    unsigned char *ptr;
	    int bytes;

	    for (bytes=0, ptr=new_data; bytes<size; bytes++, ptr++) 
            {
		if ((value = NextInt(fstream)) < 0) 
                {
                   if (new_data) XtFree (new_data);
                   fclose (fstream);
                   return (False);
                }
		*ptr=value;
	    }
	}
    }

    if (new_data == NULL)
    {
       fclose (fstream);
       return (False);
    }

    *data = new_data;
    new_data = NULL;
    *width = ww;
    *height = hh;

    fclose (fstream);
    return (True);
}



/************************************************************************
 *
 *  _XmGetImageFromFile
 *	Given a filename, extract and create an image from the file data.
 *
 ************************************************************************/

XImage * _XmGetImageFromFile (filename)
char * filename;

{
   int width; 
   int height;
   char * data;
   XImage * image;

   if (ReadBitmapDataFromFile (filename, &width, &height, &data))
   {
      image = (XImage *) XtMalloc (sizeof (XImage));
      image->width = width;
      image->height = height;
      image->data = data;
      image->depth = 1;
      image->xoffset = 0;
      image->format = XYBitmap;
      image->byte_order = LSBFirst;
      image->bitmap_unit = 8;
      image->bitmap_bit_order = LSBFirst;
      image->bitmap_pad = 8;
      image->bytes_per_line = (width+7)/8;
      return (image);
   }

   return (NULL);
}

