#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)XmString.c	3.32.1.2 91/03/07";
#endif /* lint */
#endif /* REV_INFO */
/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
*  (c) Copyright 1989, 1990, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
*  (c) Copyright 1988, 1989, 1990, HEWLETT-PACKARD COMPANY
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
/*
 * this enables compilation of debugging routines at the end
 */

/***********/
#define DEBUG
/************/

#include <ctype.h>
#include <X11/Xlib.h>
/* #include <X11/IntrinsicI.h> */
#include <X11/Intrinsic.h>
#include <X11/IntrinsicP.h>
#include <X11/Xutil.h>
#include <X11/StringDefs.h>
#include <Xm/Xm.h>
#include <Xm/XmP.h>

#define CVT_STRING_SIZE         1000  /*  max size of a string that the */
                                      /* type convertes will handle */


#define two_byte_font(f)        (( (f)->min_byte1 != 0 || (f)->max_byte1 != 0))

#define Half(x)		(x >> 1)

typedef unsigned short UShort;

/****************
 *
 * Because of possible byte alignment problems, we define a macro to
 * read and write the short quantities. If we are aligned, the shorts 
 * are stored low byte = low 8 bits
 *
 ****************/
#ifdef STRINGS_ALIGNED

#define WriteShort(addr, short)                         \
        *((unsigned char *)addr) = (unsigned char)(short & 0xff);       \
        *(((unsigned char *)addr) + 1) = (unsigned char)(short >> 8);  

#define ReadShort(addr, short)                                  \
        short = (((UShort) *(((unsigned char *)addr) + 1)) << 8)    \
                | ((UShort ) *((unsigned char *)(addr)));
#else

#define WriteShort(addr, short) *addr = short;        
#define ReadShort(addr, short) short = *addr;        

#endif  /* STRINGS_ALIGNED */



/*
 * These are the fontlist structures
 */


typedef struct _XmFontListRec
{
    XFontStruct      *font;
    XmStringCharSet  charset;
}
    XmFontListRec;

typedef struct _FontlistCache {
    XmFontList fontlist;
    struct _FontlistCache *next;
    int        refcount;
  } FontlistEntry;


typedef struct _XmFontListContextRec
{
    XmFontList          nextPtr;                /* next one in fontlist */
    Boolean             error;                  /* something bad */
}
    XmFontListContextRec;

/*
 * these are the structures which make up the optimized
 * internal version of the TCS.
 */

typedef struct
{
    short       line_count;             /* lines in this _XmString */
    XmStringCharSet charset;            /* name of charset */
    Dimension   pixel_width;            /* width of segment */
    short       font_index;             /* index of font to use */
    short       char_count;             /* octet count for this segment */
    XmStringDirection direction;        /* octet order of this segment */
    char        text[1];                /* the string text */
}
    _XmStringOptRec, *_XmStringOpt;




/*
 * these are the structures which make up the internal version of the TCS.
 */
typedef struct
{
    char        *charset;               /* name of charset */
    short       font_index;             /* index of font to use */
    short       char_count;             /* octet count for this segment */
    char        *text;                  /* ptr to octets. If RtoL then */
                                        /* is a local flipped copy */
    XmStringDirection direction;        /* octet order of this segment */
    Dimension   pixel_width;            /* width of segment */
}
    _XmStringSegmentRec, *_XmStringSegment;

typedef struct
{
    short               segment_count;  /* segments in this line */
    _XmStringSegment    segment;        /* array of segments */
}
    _XmStringLineRec, *_XmStringLine;



typedef struct __XmStringRec
{
    short       line_count;             /* lines in this _XmString */
    _XmStringLine line;                 /* array of lines */
}
    _XmStringRec;



/*
 * internal context data block, for read-out
 */

typedef struct __XmStringContextRec
{
    _XmString   string;			/* pointer to internal string */
    short       current_line;           /* index of current line */
    short       current_seg;            /* index of current segment */
    Boolean     optimized;              /* flags whether this is optimized */
    Boolean     error;                  /* something wrong */
}
    _XmStringContextRec;

						

/*
 * external context data block
 */

typedef struct _XmtStringContextRec
{
    XmString            string;
    unsigned short      offset;                 /* current place TCS */
    unsigned short      length;                 /* max length */
    XmStringCharSet     charset;                /* last charset seen */
    unsigned short      charset_length;         /* and it's length */
    XmStringDirection   direction;              /* last direction */
    Boolean             error;                  /* something bad */
}
    XmStringContextRec;



/*
 * Data structure macros for fontlist and fontcache access
 */

#define  FontListFont(f)		((XmFontList)(f))->font
#define  FontListCharset(f)		((XmFontList)(f))->charset
#define  FontCacheFontList(cache)	((FontlistEntry *)(cache))->fontlist
#define  FontCacheNext(cache)		(((struct _FontlistCache *)(cache)) \
					 ->next)
#define  FontCacheRefCount(cache)       ((FontlistEntry *)(cache))->refcount
#define  FontListContextNext(context)	(((XmFontListContextRec *)(context))-> \
					nextPtr)
#define  FontListContextError(context)	(((XmFontListContextRec *)(context))-> \
					error)

/*
 * Macros for the optimized segment data structure access
 */
#define _XmOptLineCount(opt)            ((_XmStringOpt)(opt))->line_count
#define _XmOptCharset(opt)              ((_XmStringOpt)(opt))->charset
#define _XmOptPixelWidth(opt)           ((_XmStringOpt)(opt))->pixel_width
#define _XmOptFontIndex(opt)            ((_XmStringOpt)(opt))->font_index
#define _XmOptCharCount(opt)            ((_XmStringOpt)(opt))->char_count
#define _XmOptDirection(opt)            ((_XmStringOpt)(opt))->direction
#define _XmOptText(opt)                 ((_XmStringOpt)(opt))->text

/*
 * Macros for non-optimized segment data structure access
 */

#define _XmSegCharset(seg)		((_XmStringSegment)(seg))->charset
#define _XmSegFontIndex(seg)		((_XmStringSegment)(seg))->font_index
#define _XmSegCharCount(seg)		((_XmStringSegment)(seg))->char_count
#define _XmSegText(seg)			((_XmStringSegment)(seg))->text
#define _XmSegDirection(seg)		((_XmStringSegment)(seg))->direction
#define _XmSegPixelWidth(seg)		((_XmStringSegment)(seg))->pixel_width


/*
 * Macros for line data structure access
 */

#define _XmStrLineSegCount(line)        ((_XmStringLine)(line))->segment_count
#define _XmStrLineSegment(line)	        ((_XmStringLine)(line))->segment

/*
 * Macros for internal string data structure access
 */

#define _XmStrLineCnt(str)		((_XmString)(str))->line_count
#define _XmStrLineLine(str)		((_XmString)(str))->line

/*
 * Macros for string internal context block data structure access
 */

#define _XmStrContString(cont)	  \
                 ((_XmStringContextRec *)(cont))->string
#define _XmStrContCurrLine(cont)  \
                 ((_XmStringContextRec *)(cont))->current_line
#define _XmStrContCurrSeg(cont)   \
                 ((_XmStringContextRec *)(cont))->current_seg
#define _XmStrContOpt(cont)	  \
                 ((_XmStringContextRec *)(cont))->optimized
#define _XmStrContError(cont)	((_XmStringContextRec *)(cont))->error


/*
 * Macros for string external context block data structure access
 */

#define XmStrContString(cont)	((XmStringContextRec *)(cont))->string
#define XmStrContOffset(cont)	((XmStringContextRec *)(cont))->offset
#define XmStrContLength(cont)	((XmStringContextRec *)(cont))->length
#define XmStrContCharSet(cont)	((XmStringContextRec *)(cont))->charset
#define XmStrContCharSetLen(cont)	\
                 ((XmStringContextRec *)(cont))->charset_length
#define XmStrContDirection(cont)	\
                 ((XmStringContextRec *)(cont))->direction
#define XmStrContError(cont)	((XmStringContextRec *)(cont))->error



/*
 * this set constructs external XmString format object.  The TLV version
 */

/*
 	The external version of XmString is:

	COMPOUND_STRING			5 bytes (see description below)

	    component tag		1 byte
	    length			2 bytes
	    value			n bytes

	    component tag		1 byte
	    length			2 bytes
	    value			n bytes

	eg. very simple... 
*/


/*
 * ASN.1 header for compound string - 3 byte header, followed by length
 * which is two bytes maximum, but almost always 1 byte.
 *
 * The first byte defines the ASN.1 space:  (0xdf)
 *              1 1      0      1 1 1 1 1
 *              class    form   ID code
 *
 *    class is private, form is primitive (not constructed from other
 *    forms), and the ID code value means the actual ID code value is
 *    extended into one or more octets.
 *
 * The second and third bytes define the actual ID code value.  The
 * value used is the original XUI value.
 *     second byte:  (0xff)
 *               1       1111111
 *              MSB      high seven bits of ID code
 *
 *     third byte:   (0x79)
 *               0       1111001
 *              LSB      low seven bits of ID code
 *
 * The length field of the ASN.1 conformant compound string header
 * is dynamically constructed.  There are two possible forms for depending
 * upon the length of the string.  Note that this length excludes the
 * header bytes.
 *
 *    Short Form: range 0 .. 127
 *    one byte
 *                  0         nnnnnnn
 *                 short       7 bit length
 *
 *    Long Form: range 128 .. 2**32-1
 *    three bytes
 *    first:        1         nnnnnnn
 *                 long       number of bytes to follow
 *
 *    second:
 *                  nnnnnnnn
 *                  MSB of length
 *
 *    third:
 *                  nnnnnnnn
 *                  LSB of length
 *
 */


#define CSHEADERLEN     3
#define CSHEADER1       0xdf
#define CSHEADER2       0xff
#define CSHEADER3       0x79
static unsigned char CSHeader[3] = {CSHEADER1, CSHEADER2, CSHEADER3};


#define MAXSHORTVALUE   127             /* maximum len to be used for short 
                                           length form */
#define CSLONGLEN       3
#define CSSHORTLEN      1
#define CSLONGLEN1      0x82
#define CSLONGBIT	0x80





#define HEADER 3	/* num bytes for tag & length */





/*
 * forward references
 */


#ifndef _ARGUMENTS
#ifdef _NO_PROTO
#define _ARGUMENTS(arglist) ()
#else
#define _ARGUMENTS(arglist) arglist
#endif
#endif

static Boolean _is_short_length  _ARGUMENTS(( unsigned char *p ));
static void _write_long_length  _ARGUMENTS(( unsigned char *p , UShort length ));
static unsigned char *_write_header  _ARGUMENTS(( unsigned char *p , UShort length ));
static unsigned char *_read_header  _ARGUMENTS(( unsigned char *p ));
static UShort _read_header_length  _ARGUMENTS(( unsigned char *p ));
static UShort _read_string_length  _ARGUMENTS(( unsigned char *p ));
static unsigned char *_write_component  _ARGUMENTS(( unsigned char *p , unsigned char tag , UShort length , unsigned char *value , Boolean move_by_length ));
static unsigned char *_read_component  _ARGUMENTS(( unsigned char *p , unsigned char *tag , UShort *length , unsigned char *value ));
static UShort _read_component_length  _ARGUMENTS(( unsigned char *p ));
static unsigned char *_copy_short_to_long  _ARGUMENTS(( unsigned char *p ));
static char *_cache_charset  _ARGUMENTS(( char *charset , int length ));
static void _cache_fontlist  _ARGUMENTS(( XmFontList f ));
static Boolean _is_compound  _ARGUMENTS(( XmString string ));
static Dimension _XmStringOptLineAscender  _ARGUMENTS(( XmFontList f , _XmStringOpt opt ));
static Dimension _XmStringOptLineDescender  _ARGUMENTS(( XmFontList f , _XmStringOpt opt ));
static Dimension _XmStringOptLineWidth  _ARGUMENTS(( XmFontList fontlist , _XmStringOpt optline ));
static Dimension _XmStringOptLineHeight  _ARGUMENTS(( XmFontList fontlist , _XmStringOpt optline ));
static Dimension _XmStringLineAscender  _ARGUMENTS(( XmFontList f , _XmStringLine line ));
static Dimension _XmStringLineDescender  _ARGUMENTS(( XmFontList f , _XmStringLine line ));
static Dimension _XmStringLineWidth  _ARGUMENTS(( XmFontList fontlist , _XmStringLine line ));
static Dimension _XmStringLineHeight  _ARGUMENTS(( XmFontList fontlist , _XmStringLine line ));
static void _XmStringSubStringPosition  _ARGUMENTS(( Boolean one_byte , XFontStruct *font , _XmStringSegment seg , _XmStringSegment under_seg , Position x , Dimension *under_begin , Dimension *under_end ));
static void _XmStringDrawSegment  _ARGUMENTS(( Display *d , Window w , int x , int y , _XmStringSegment seg , GC gc , XmFontList fontlist , Boolean image , _XmString underline , Dimension *under_begin , Dimension *under_end ));
static void _XmStringDrawLine  _ARGUMENTS(( Display *d , Window w , int x , int y , _XmStringLine line , GC gc , XmFontList fontlist , Boolean image , _XmString underline , Dimension *under_begin , Dimension *under_end , Boolean opt ));
static void _calc_align_and_clip  _ARGUMENTS(( Display *d , GC gc , Position *x , Position y , Dimension width , int line_width , unsigned char lay_dir , XRectangle *clip , unsigned char align , int descender , int *restore ));
static void _draw  _ARGUMENTS(( Display *d , Window w , XmFontList fontlist , _XmString string , GC gc , Position x , Position y , Dimension width , unsigned char align , unsigned char lay_dir , XRectangle *clip , Boolean image , _XmString underline ));
static void new_segment  _ARGUMENTS(( _XmString string , int line_index , _XmStringSegment value ));
static void new_line  _ARGUMENTS(( _XmString string ));
static _XmString _XmStringOptCreate  _ARGUMENTS(( unsigned char *c , unsigned char *end , UShort textlen , Boolean havecharset));
static _XmString _XmStringNonOptCreate  _ARGUMENTS(( unsigned char *c , unsigned char *end ,Boolean havecharset ));
static void _update_opt  _ARGUMENTS(( XmFontList fontlist , _XmStringOpt optline ));
static void _update_segment  _ARGUMENTS(( XmFontList fontlist , _XmStringSegment seg ));
static void _clear_segment  _ARGUMENTS(( XmFontList fontlist , _XmStringSegment seg ));
static void _clear_opt  _ARGUMENTS(( XmFontList fontlist , _XmStringOpt opt ));
static void _parse_locale _ARGUMENTS((char *str, int *index, int *chlen));
static Boolean _isISO _ARGUMENTS(( char *charset ));

/*
 * calculates the number of bytes in the header of an external compound
 * string, given the total length of the components.
 */

#define _calc_header_size(len) \
    ((((UShort)(len)) > MAXSHORTVALUE) ? (CSHEADERLEN + CSLONGLEN) : (CSHEADERLEN + CSSHORTLEN))


/*
 * Determines whether this string has a short or long length field
 */
static Boolean
_is_short_length(p)
unsigned char      *p;
{

    unsigned char *uchar_p = (unsigned char *) p;

    uchar_p += CSHEADERLEN;

    if (*uchar_p & (char)CSLONGBIT)
       return (FALSE);
    else return (TRUE);
}


/*
 * Routine that writes a long length field
 */

static void
#ifdef _NO_PROTO
_write_long_length (p, length)
unsigned char  *p;
UShort length;
#else /* _NO_PROTO */
_write_long_length (unsigned char *p, UShort length)
#endif /* _NO_PROTO */
{

    unsigned char   * uchar_p = (unsigned char *) p;

    /* 
     * flag the long version
     */
    *uchar_p = CSLONGLEN1;
    uchar_p++;

    /* 
     * need to pull off the high 8 bits 
     */

    *uchar_p = (unsigned char) (length >> 8);
    uchar_p++;
    *uchar_p = (unsigned char) (length & 0xff);

}
	

/*
 * Private routines for manipulating the ASN.1 header of external
 * compound strings.
 */

static unsigned char *
#ifdef _NO_PROTO
_write_header (p, length)
unsigned char   *p;
UShort  length;
#else /* _NO_PROTO */
_write_header (unsigned char *p, UShort length)
#endif /* _NO_PROTO */
{

    unsigned char * uchar_p = p;
    int     headlen;

    /* write the header in. */

    headlen = CSHEADERLEN;
    bcopy (CSHeader, uchar_p, CSHEADERLEN);
    uchar_p += CSHEADERLEN;

    /* short or long length value? */
 
    if (length > MAXSHORTVALUE)
    {
        _write_long_length(uchar_p, length);
        headlen += CSLONGLEN;
    }
    else {
        /* Short version */
        *uchar_p = (unsigned char) length;
        headlen += CSSHORTLEN;
    }
    return (p + headlen);
}




/*
 * extracts the ASN.1 header from the external compound string.
 */

static unsigned char *
_read_header (p)
unsigned char  *p;
{
    /*
     * Read past the ASN.1 header; get the first length byte and see if this
     * is a one or three byte length.
     */

    if (_is_short_length(p))
        return (p + CSHEADERLEN + CSSHORTLEN);
    else
       return (p + CSHEADERLEN + CSLONGLEN); 
}

/*
 * reads the length the ASN.1 header of an external
 * compound string.
 */

static UShort
_read_header_length (p)
unsigned char *p;
{
    /*
     * Read past the ASN.1 header; get the first length byte and see if this
     * is a one or three byte length.
     */

    if (_is_short_length(p))
       return (CSHEADERLEN + CSSHORTLEN);
    else
       return (CSHEADERLEN + CSLONGLEN);

}


/*
 * calculates the length of the external compound string, excluding the
 * ASN.1 header.
 */

static UShort
_read_string_length (p)
unsigned char *p;
{


    unsigned char * uchar_p = (unsigned char *) p;
    UShort totallen = 0;

    /*
     * Read past the ASN.1 header; get the first length byte and see if this
     * is a one or three byte length.
     */

    uchar_p += CSHEADERLEN;

    if (_is_short_length(p))
    {
       totallen += (UShort) *uchar_p;
    }
    else {
       UShort i;

       uchar_p++;
       i = ((UShort) *uchar_p) << 8;
       uchar_p++;
       i |= ((UShort) *uchar_p);    /* Mask on the low byte */
       totallen += i;
    }
    return (totallen);
}



/*
 * Private routines for reading/writing the individual compound string
 * components
 */



static unsigned char *
#ifdef _NO_PROTO
_write_component (p, tag, length, value, move_by_length)
    unsigned char *p;
    unsigned char tag;
    UShort length;
    unsigned char *value;
    Boolean move_by_length;
#else /* _NO_PROTO */
_write_component (unsigned char *p, unsigned char tag, UShort length, 
		  unsigned char *value, Boolean move_by_length)
#endif /* _NO_PROTO */
{
    unsigned char * uchar_p = p;
    UShort * short_p = (UShort *) (p + 1);
    int k;

    *uchar_p = tag;				/* stuff tag */
    uchar_p += HEADER;				/* move to value */

    WriteShort(short_p, length);
    
/*    *short_p = length;	*/

    if (value != (unsigned char *) NULL)
 	for (k=0; k<length; k++) *uchar_p++ = *value++;

    if (move_by_length)
	return (p + length + HEADER);
    else
	return (p + HEADER);
}



static unsigned char *
_read_component (p, tag, length, value)
    unsigned char *p;
    unsigned char *tag;
    UShort *length;
    unsigned char *value;
{
    unsigned char * uchar_p = (unsigned char *) p;
    UShort * short_p = (UShort *) (p + 1);
    UShort s;
    int k;

    *tag = *uchar_p;				/* read tag */
    uchar_p += HEADER;				/* move to value */

    ReadShort(short_p, s);
    WriteShort(length, s);

    for (k=0; k<s; k++) *value++ = *uchar_p++;

    return (p + s + HEADER);
}



static UShort 
_read_component_length (p)
    unsigned char *p;
{
    UShort * short_p = (UShort *) (p + 1);
    UShort s;

    ReadShort(short_p, s)
    return (s);				/* read length */
}

/*
 * Routine to copy a short header length string to a long header
 * length version.  Used by concatenates.  The original string
 * is freed.
 */

static unsigned char *
_copy_short_to_long(p)
    unsigned char *p;
{

    unsigned char    * uchar_p = (unsigned char *) p;
    unsigned char    * q;
    unsigned char    * newstring;
    UShort   len;
    

    len = _read_string_length(p) + CSHEADERLEN + CSLONGLEN;

    newstring = (unsigned char *) XtMalloc(len);
    uchar_p = p;
    q = newstring;

    q = _write_header(q, (UShort) MAXSHORTVALUE+1);
    uchar_p = _read_header(uchar_p);
    len = _read_string_length(p);
    bcopy(q, uchar_p, len);

    q = newstring + CSHEADERLEN;
    _write_long_length(q, len);
    return (newstring);
}




/*
 * build an external TCS text component
 */

XmString
XmStringCreate (text, charset)
    char	*text;
    XmStringCharSet charset;
{
    unsigned char    *p;
    XmString string;
    char     *curcharset;
    int      t_length;
    int      c_length;
    int      i;
    short    head_size;
    
    if (!text) return ((XmString) NULL);
    if (!charset) return ((XmString) NULL);    

    t_length = strlen (text);

    if ((strcmp(charset, XmSTRING_DEFAULT_CHARSET) == 0))
      curcharset = _XmStringGetCurrentCharset();
    else curcharset = _XmCharsetCanonicalize(charset);
    c_length = strlen (curcharset);
	
    /* 
     * Add up the number of bytes to allocate.  The header length field size
     * is calculated from the number of bytes in the string.
     */

    i = t_length + HEADER + c_length + HEADER;
    head_size = _calc_header_size((UShort)i);  
    i += head_size;

    string = (XmString) XtMalloc (i);
    p = (unsigned char *) string;

    p = _write_header (p, (UShort)(i - head_size));

    p = _write_component (p, (unsigned char) XmSTRING_COMPONENT_CHARSET, 
                             (UShort) c_length, (unsigned char *) curcharset, TRUE);

    p = _write_component (p, (unsigned char) XmSTRING_COMPONENT_TEXT, 
                             (UShort) t_length, (unsigned char *) text, TRUE);
    XtFree(curcharset);

    return (string);
}

/*
 * Convenience routine to create an XmString from a NULL terminated string.
 */

XmString
XmStringCreateSimple (text)
     char     *text;
{
  return (XmStringCreate(text, XmSTRING_DEFAULT_CHARSET));
 }


/*
 * build an external TCS direction component
 */

XmString
#ifdef _NO_PROTO
XmStringDirectionCreate (direction)
    XmStringDirection direction;
{
#else /* _NO_PROTO */
XmStringDirectionCreate (
#if NeedWidePrototypes
    int dir)
{
    XmStringDirection	direction = (XmStringDirection) dir;
#else
    XmStringDirection direction)
{
#endif 

#endif /* _NO_PROTO */
    XmString string;
    unsigned char    *p;
    int      i;
    short    head_size;

    i = HEADER + 1;                     /* direction header and value */
    head_size = _calc_header_size((UShort) i);
    i += head_size;

    string = (XmString) XtMalloc (i);
    p = (unsigned char *) string;

    p = _write_header (p, (UShort)(i - head_size));

    p = _write_component (p, (unsigned char) XmSTRING_COMPONENT_DIRECTION, 
                             (UShort) 1, &direction, TRUE);

    return (string);
}


/*
 * build an external TCS separator component
 */

XmString
XmStringSeparatorCreate ()
{
    XmString string;
    unsigned char    *p;
    int      i;
    short    head_size;

    i = HEADER;
    head_size = _calc_header_size((UShort) i);
    i += head_size;

    string = (XmString) XtMalloc (i);
    p = (unsigned char *) string;

    p = _write_header (p, (UShort) (i - head_size));

    p = _write_component (p, (unsigned char) XmSTRING_COMPONENT_SEPARATOR, 0, NULL, TRUE);

    return (string);
}




/*
 * build an external TCS 'segment', just a high level create
 */

XmString
#ifdef _NO_PROTO
XmStringSegmentCreate (text, charset, direction, separator)
    char 		*text;
    XmStringCharSet	charset;
    XmStringDirection	direction;
    Boolean		separator;
#else /* _NO_PROTO */
XmStringSegmentCreate (char *text, XmStringCharSet charset, 
#if NeedWidePrototypes
int direction, int separator
#else
XmStringDirection direction, Boolean separator
#endif 
)
#endif /* _NO_PROTO */
{
    XmString a, b, c;

    a = XmStringDirectionCreate (direction);
    b = XmStringCreate (text, charset);

    c = XmStringConcat (a, b);

    XtFree (a);
    XtFree (b);

    if (separator)
    {
	a = XmStringSeparatorCreate ();

	b = XmStringConcat (c, a);

	XtFree (a);
	XtFree (c);

	c = b;
    }

    return (c);
}


/*
 * as close as we can come to Latin1Create without knowing the charset of
 * Latin1.  This imposes the semantic of \n meaning separator.
 */

/* BEGIN OSF Fix pir 2652 */
XmString XmStringLtoRCreate (text, charset)
    char	*text;
    XmStringCharSet charset;
{
    char *c;
    char *start, *end;
    Boolean done;
    XmString string = (XmString) NULL, next, tmp;

    if (!text) return (string);

    c = (char *) strcpy ((char *) XtMalloc (strlen (text) + 1), text);
    start = c;
    done = FALSE;

    /* Set the direction once only at the beginning. */
    string = XmStringDirectionCreate(XmSTRING_DIRECTION_L_TO_R);
    
    while ( ! done)				/* loop thru local copy */
    {						/* looking for \n */
	end = start;

	while ((*end != '\0') && (*end != '\n'))  end++;

	if (*end == '\n')			/* replace \n with \0 */
	    *end = '\0';
	else
	    done = TRUE;			/* we are at the end */

	/* Don't convert an empty string. */
	if (start != end) {
	  next = XmStringCreate(start, charset);
	  tmp = string;
	  string = XmStringConcat(tmp, next);
	
	  XmStringFree(tmp);
	  XmStringFree(next);
	}
	
	/* Make a separator if this isn't the last segment. */
	if (!done) {
	  next = XmStringSeparatorCreate();
	  tmp = string;
	  string = XmStringConcat(tmp, next);
	
	  XmStringFree(tmp);
	  XmStringFree(next);
	}
	
	start = ++end;				/* start at next char */
    }

    XtFree (c);
    return (string);
}
/* END OSF Fix pir 2652 */


XmString
XmStringCreateLtoR (text, charset)
    char	*text;
    XmStringCharSet charset;
{
    return (XmStringLtoRCreate (text, charset));
}
/*
 * this set provides access to the external TCS
 */


/*
 * set up the read-out context
 */

Boolean 
XmStringInitContext (context, string)
    XmStringContext *context;
    XmString string;
{
    XmStringContext q;
    
    if (!string) return(FALSE);
    
    q = (XmStringContext) XtMalloc (sizeof (XmStringContextRec));

    /*
     * read past header
     */

    XmStrContOffset(q)     = _read_header_length((unsigned char *)string);
    XmStrContLength(q)     = _read_string_length((unsigned char *)string);
    XmStrContCharSet(q)    = NULL;
    XmStrContCharSetLen(q) = 0;
    XmStrContDirection(q)  = XmSTRING_DIRECTION_L_TO_R;
    XmStrContError(q)      = FALSE;

    XmStrContString(q)     = string;
    
    *context = q;

    XmStrContError(q)      = !_is_compound(string);

    return ( ! XmStrContError(q));
}


void
XmStringFreeContext (context)
    XmStringContext context;
{
    XtFree (context);
}



/*
 * fetch the next component of the external TCS
 */

XmStringComponentType
XmStringGetNextComponent (context, text, charset, direction, 
		unknown_tag, unknown_length, unknown_value)
    XmStringContext 	context;
    char		**text;			/* output */
    XmStringCharSet	*charset;		/* output */
    XmStringDirection	*direction;		/* output */
    XmStringComponentType *unknown_tag;		/* output */
    UShort		*unknown_length;	/* output */
    unsigned char		**unknown_value;	/* output */
{
    unsigned char *p = ((unsigned char *) XmStrContString(context)) + XmStrContOffset(context);
    XmStringComponentType answer;
    unsigned char tag;
    UShort length,foo;
    
    if ((XmStrContError(context)) || 
        (XmStrContOffset(context) >= 
            (XmStrContLength(context) + 
                 _read_header_length((unsigned char *)XmStrContString(context)))))
       return (XmSTRING_COMPONENT_END);
    
    length = _read_component_length (p);

    switch (*p)
    {
	case XmSTRING_COMPONENT_CHARSET:
	    XmStrContCharSetLen(context) = length;		/* update context */
	    XmStrContCharSet(context) = (char *) (p + HEADER);

            *charset  = (XmStringCharSet) XtMalloc (length + 1);
            length = _read_component_length (p);
    	    p = _read_component (p, &tag, &foo, (unsigned char *)*charset);
	    (*charset)[length] = '\0';
	    answer = (XmStringComponentType) XmSTRING_COMPONENT_CHARSET;
	    break;

	case XmSTRING_COMPONENT_TEXT:
	    *text  = (char *) XtMalloc (length + 1);
            length = _read_component_length (p);
    	    p = _read_component (p, &tag, &foo, (unsigned char *)*text);
	    (*text)[length] = '\0';
	    answer = (XmStringComponentType) XmSTRING_COMPONENT_TEXT;
	    break;

	case XmSTRING_COMPONENT_DIRECTION:
            length = _read_component_length (p);
    	    p = _read_component (p, &tag, &foo, direction);

	    XmStrContDirection(context) = *direction;
	    answer = (XmStringComponentType) XmSTRING_COMPONENT_DIRECTION;
	    break;

	case XmSTRING_COMPONENT_SEPARATOR:
            length = _read_component_length (p);
    	    p = _read_component (p, &tag, &foo, NULL);
	    answer = (XmStringComponentType) XmSTRING_COMPONENT_SEPARATOR;
	    break;

	default:
	    *unknown_tag = (XmStringComponentType) *p;
	    WriteShort(unknown_length,length);
	    *unknown_value = (unsigned char *) XtMalloc (length + 1);
            length = _read_component_length (p);
    	    p = _read_component (p, &tag, &foo, *unknown_value);
	    answer = (XmStringComponentType) XmSTRING_COMPONENT_UNKNOWN;
	    break;
    }

    XmStrContOffset(context) += length + HEADER;

    return (answer);
}




XmStringComponentType
XmStringPeekNextComponent (context)
    XmStringContext	context;
{
    unsigned char *p = ((unsigned char *) XmStrContString(context)) + XmStrContOffset(context);
    XmStringComponentType answer;

    if ((XmStrContError(context)) ||
        (XmStrContOffset(context) >= 
            (XmStrContLength(context) + 
                 _read_header_length((unsigned char *)XmStrContString(context)))))
       return (XmSTRING_COMPONENT_END);

    switch (*p)
    {
	case XmSTRING_COMPONENT_CHARSET:
	case XmSTRING_COMPONENT_TEXT:
	case XmSTRING_COMPONENT_DIRECTION:
	case XmSTRING_COMPONENT_SEPARATOR:
	    answer = (XmStringComponentType) *p;
	    break;

	default:
	    answer = (XmStringComponentType) XmSTRING_COMPONENT_UNKNOWN;
	    break;
    }
    return (answer);
}




/*
 * fetch the next 'segment' of the external TCS
 */

Boolean
XmStringGetNextSegment (context, text, charset, direction, separator)
    XmStringContext 	context;
    char		**text;			/* output */
    XmStringCharSet	*charset;		/* output */
    XmStringDirection	*direction;		/* output */
    Boolean		*separator;		/* output */
{
    XmStringComponentType j;
    unsigned char  *jv;
    UShort jl;
    char *t;
    XmStringCharSet	 c_set = NULL;
    XmStringDirection	 dir = XmSTRING_DIRECTION_L_TO_R;

    *text      = NULL;				/* pre-condition results */
    *charset   = NULL;
    *direction = XmStrContDirection(context);
    *separator = FALSE;

    if (XmStrContError(context)) return (FALSE);

    while (TRUE)				/* return directly */
    {
     	switch (XmStringGetNextComponent (context, &t, &c_set, &dir, 
		&j, &jl, &jv))
	{
	   case XmSTRING_COMPONENT_DIRECTION:	/* just record these */
                *direction = dir;
                break;

	   case XmSTRING_COMPONENT_SEPARATOR:
		break;

	   case XmSTRING_COMPONENT_UNKNOWN:	/* skip these */
		XtFree (jv);
		break;

	   case XmSTRING_COMPONENT_CHARSET:	/* careful of memory leak */
		if (c_set != NULL) XtFree (c_set);
		break;

	   case XmSTRING_COMPONENT_TEXT:	/* at last */
		*text = t;
		*separator = XmStringPeekNextComponent (context) == 
				XmSTRING_COMPONENT_SEPARATOR;

		*charset = (XmStringCharSet) XtMalloc (XmStrContCharSetLen(context) + 1);
                bcopy (	       XmStrContCharSet(context),
                               (char *)*charset,
                               XmStrContCharSetLen(context));
		(*charset)[XmStrContCharSetLen(context)] = '\0';

		return (TRUE && ( ! XmStrContError(context)));
		break;

	    case XmSTRING_COMPONENT_END:	/* no more */
		/* no break */

	    default:
		return (FALSE);
		break;
	}
    }
}


/*
 * fetch the first text 'segment' of the external TCS that matches the given
 * char set.
 */

Boolean
XmStringGetLtoR (string, charset, text)
    XmString 		string;
    XmStringCharSet	charset;
    char		**text;			/* output */
{
    XmStringContext context;
    char * t;
    char *curcharset;
    XmStringCharSet c;
    XmStringDirection d;
    Boolean s, done = FALSE;


    *text = NULL;				/* pre-condition result */
    if (!string) return(FALSE);
    if (!charset) return (FALSE);

    if ((strcmp(charset, XmSTRING_DEFAULT_CHARSET) == 0))
      curcharset = _XmStringGetCurrentCharset();
    else curcharset = _XmCharsetCanonicalize(charset);
    
    XmStringInitContext (&context, string);

    while ( ! done)
    {
	if (XmStringGetNextSegment (context, &t, &c, &d, &s))
	{
	    if ((c) && (curcharset) &&
                 (strcmp (c, curcharset) == 0) && 
                 (d == XmSTRING_DIRECTION_L_TO_R))
	    {
		*text = t;			/* OK, pass text to caller */
		done = TRUE;
	    }
	    else
		XtFree (t);			/* not this text */

   	    if (c)
	        XtFree (c);				/* always dump charset */
	}
	else
	    done = TRUE;
    }

    XmStringFreeContext (context);
    XtFree(curcharset);

    return (*text != NULL);
}








/*
 * this set is the TCS font list handling stuff
 */



static char  *_default_charset = "";
static char **_charset_cache = NULL;
static int    _cache_count = 0;
static FontlistEntry *_fontlist_cache = NULL;

static char *
_cache_charset (charset, length)
    char *charset;
    int length;
{
    char *a;
    int i;

    for (i=0; i<_cache_count; i++)
    {
	int l = strlen(_charset_cache[i]);
        if (l == length &&
	    strncmp (charset, _charset_cache[i], l) == 0) 
	{
	    return (_charset_cache[i]);
	}
    }

    _charset_cache = (char **) XtRealloc (_charset_cache, 
			sizeof (char **) * (_cache_count + 1));

    a = XtMalloc (length + 1);
    bcopy (charset, a, length);
    a[length] = '\0';

    _charset_cache[_cache_count++] = a;

    return (a);
}

static void
_cache_fontlist(f)
    XmFontList    f;
{

    FontlistEntry *node = (FontlistEntry *) XtMalloc(sizeof(FontlistEntry));

    FontCacheFontList(node) = f;
    FontCacheRefCount(node) = 1;
    FontCacheNext(node) = _fontlist_cache;

    _fontlist_cache = node;
}







XmFontList
XmFontListCreate (font, charset)
    XFontStruct 	*font;
    XmStringCharSet 	charset;
{

    FontlistEntry       *cachePtr;
    XmFontList          f;
    char 	        *curcharset;

    if ((font == NULL) || (charset == NULL))
         return (NULL);

    if ((strcmp(charset, XmSTRING_DEFAULT_CHARSET) == 0))
      curcharset = _XmStringGetCurrentCharset();
    else curcharset = _XmCharsetCanonicalize(charset);

    /*
     * See if this fontlist is cached.
     */

    for (cachePtr = _fontlist_cache; cachePtr; 
                                     cachePtr = FontCacheNext(cachePtr))

    {

        XmFontList    listPtr;

        listPtr = FontCacheFontList(cachePtr);
        if ((FontListFont(&listPtr[0]) == font) &&
            (FontListCharset(&listPtr[0]) != NULL) &&
            (strcmp(FontListCharset(&listPtr[0]),curcharset) == 0) &&
            (FontListFont(&listPtr[1]) == NULL) &&
            (FontListCharset(&listPtr[1]) == NULL))
        {
            FontCacheRefCount(cachePtr)++;
/* BEGIN OSF Fix pir 2473 */
	    XtFree(curcharset);
/* END OSF Fix pir 2473 */
            return (listPtr);
        }
    }

    /*
     * If we got here, the fontlist isn't in the cache.
     */

    f = (XmFontList) XtMalloc (sizeof (XmFontListRec) * 2);

    FontListFont(&f[0]) = font;
    FontListCharset(&f[0]) = _cache_charset (curcharset, strlen (curcharset));

    FontListFont(&f[1]) = NULL;
    FontListCharset(&f[1]) = NULL;

    _cache_fontlist(f);
    XtFree(curcharset);

    return (f);
}

XmFontList
XmStringCreateFontList (font, charset)
    XFontStruct 	*font;
    XmStringCharSet 	charset;
{
    return (XmFontListCreate(font,charset));
}






/*
 * dump a font list
 */

void 
XmFontListFree (fontlist)
    XmFontList      fontlist;
{

    FontlistEntry   *cachePtr;
    FontlistEntry   *lastPtr;

    if (fontlist == NULL) return;

    for (cachePtr = _fontlist_cache, lastPtr = NULL;
         cachePtr;
         lastPtr = cachePtr, cachePtr = FontCacheNext(cachePtr))
    {

        if (FontCacheFontList(cachePtr) == fontlist)
        {
            FontCacheRefCount(cachePtr)--;
            if (FontCacheRefCount(cachePtr) == 0)
            {
                if (lastPtr == NULL)
                     _fontlist_cache = FontCacheNext(cachePtr);
                else FontCacheNext(lastPtr) = FontCacheNext(cachePtr);
                XtFree(cachePtr);
                XtFree(fontlist);
            }
            return;
        }
    }

    /*
     *  If we got here, for some bad reason this font wasn't in the
     *  cache.  Free it.
     */
    if (fontlist != NULL) 
        XtFree (fontlist);
}



/*
 * extent a font list by one element, the old font list is gone
 */

XmFontList
XmFontListAdd (old, font, charset)
    XmFontList 	old;
    XFontStruct		*font;
    XmStringCharSet	charset;
{
    int i, k, oldcnt;
    XmFontList p = old, q;
    FontlistEntry    *cachePtr;
    char             *curcharset;
    Boolean          matched;

    if (!old) 
      return((XmFontList ) NULL);
    if ((font == NULL) || (charset == NULL))
      return ((XmFontList) old);

    if ((strcmp(charset, XmSTRING_DEFAULT_CHARSET) == 0))
      curcharset = _XmStringGetCurrentCharset();
    else curcharset = _XmCharsetCanonicalize(charset);

    for (i=0; FontListFont(p) != NULL; p++, i++)	/* count list */
	;
    oldcnt = i;

    /*
     * try to find a fontlist in the cache that the newly expanded fontlist
     * would match.
     */

    for (cachePtr = _fontlist_cache; cachePtr; 
                                     cachePtr = FontCacheNext(cachePtr))
    {
        XmFontList   listPtr, oldfontPtr;

        /*
         * count the fontlist entries in this cached fontlist.  We're looking
         * for a fontlist with one entry more than the old fontlist.
         */
        p = FontCacheFontList(cachePtr);
        for (k=0; FontListFont(p) != NULL; p++, k++)
            ;

        if (k != (oldcnt+1))
           break;                    /* wrong length */

        /* ok, this one is the right length.  loop thru the fontlist entries,
         * until they don't match or the old font ends. Skip the terminating
         * NULL entries, we know the length and it will screw up the matching.
         */

        matched = TRUE;
        for (oldfontPtr = old, listPtr = FontCacheFontList(cachePtr);
             FontListFont(oldfontPtr);
             oldfontPtr++, listPtr++)
        {
            if ((FontListFont(listPtr) != FontListFont(oldfontPtr)) ||
                (strcmp(FontListCharset(listPtr), 
                        FontListCharset(oldfontPtr)) != 0))
            {
               /* match failed; try next fontlist in cache */
               matched = FALSE;
               break;
            }

        }

        /*
         * That fontlist entry matched exactly what we're looking for.  So,
         * now, compare the next cached entry against the font and charset
         * we would add.
         */

        if (matched)
        {
            if ((FontListFont(listPtr) == font) && 
                (strcmp(FontListCharset(listPtr), curcharset) == 0))
            {

               /*
                * found it.
                */
               FontCacheRefCount(cachePtr)++;
	       XtFree(curcharset);
	       
               return (listPtr);
            }
        }
    }

    /*
       After all this, we didn't find it.  Make a new fontlist from the
       old one and cache it.
    */


    q = (XmFontList) XtMalloc (sizeof (XmFontListRec) * (i+2));

    bcopy (old, q, (sizeof (XmFontListRec) * i));	/* copy over */

    FontListFont(&q[i]) = font;
    FontListCharset(&q[i]) = _cache_charset (curcharset, strlen (curcharset));

    i++;

    FontListFont(&q[i]) = NULL;
    FontListCharset(&q[i]) = NULL;

    /* make a new cache node and insert it in the beginning. */

    _cache_fontlist(q);
    XmFontListFree (old);

    XtFree(curcharset);
    return (q);
}


/*
 * replicate a font list
 */

XmFontList 
XmFontListCopy (fontlist)
    XmFontList fontlist;
{
    XmFontList p, q;
    FontlistEntry       *cachePtr;
    int i, j;

    if (!fontlist) return((XmFontList ) NULL);

    /*
     *	Find the font list in the cache, and increment the number
     *	of references.
     */
    for (cachePtr = _fontlist_cache; cachePtr; 
                                     cachePtr = FontCacheNext(cachePtr))
    {
        if (FontCacheFontList(cachePtr) == fontlist)
        {
            FontCacheRefCount(cachePtr)++;
            return (fontlist);
        }
    }

    /*
     *  If we got here, for some bad reason this font wasn't in the
     *  cache.  Make a real copy.
     */
    for (i=0, p = fontlist; FontListFont(p) != NULL; p++, i++)  /* count  list */
	;

    q = (XmFontList) XtMalloc (sizeof (XmFontListRec) * (i+1));

    for (j=0; j<i; j++)	q[j] = fontlist[j]; 		/* copy over */

    q[i] = fontlist[i];					/* copy null item too */

    return (q);
}


/*
 * Find an entry in the fontlist which matches the current charset or
 * return the first font if none match.
 */
Boolean
_XmFontListGetDefaultFont (fontlist, font_struct)
     XmFontList		fontlist;
     XFontStruct	**font_struct;
{
  XmStringCharSet	charset = _XmStringGetCurrentCharset();
  short			index = -1;
  Boolean		retval;
  
  retval = _XmFontListSearch (fontlist, charset, &index, font_struct);
  XtFree(charset);
  return(retval);
}

/*
 * find an entry in the font list which matches, return index (or -1) and
 * font stuct ptr (or first in list).
 */
Boolean
_XmFontListSearch (fontlist, charset, index, font_struct)
    XmFontList 	fontlist;
    XmStringCharSet 	charset;
    short		*index;
    XFontStruct 	**font_struct;
{
    int i;

    *index = -1;

    if ((charset == NULL) && (fontlist != NULL))
    {
	*font_struct = FontListFont(&fontlist[0]); /* pickup first one */
    	*index = 0;
	return(TRUE);	
    }
    
    if (fontlist != NULL)
    {
 	for (i=0; FontListFont(&fontlist[i]) != NULL; i++)
	{
	    if (strcmp (FontListCharset(&fontlist[i]), charset) == 0)
	    {
	    	*index = i;
	    	*font_struct = FontListFont(&fontlist[i]);
	    	return (TRUE);
	    }
	}


	*font_struct = FontListFont(&fontlist[0]);	/* pickup first one */
    	*index = 0;
       	return (TRUE);
    }
    else
/****************
 *
 * We should create a default fontlist and return that.
 *
 ****************/
	*font_struct = NULL;				/* or NULL */

    return (FALSE);
}



/*
 * Fontlist access routines
 */

Boolean XmFontListInitFontContext( context, fontlist)
XmFontContext   *context;
XmFontList      fontlist;
{

    XmFontContext p;

    if ((!fontlist) || (!context))
        return(FALSE);

    p = (XmFontContext) XtMalloc (sizeof (XmFontListContextRec));
    if (p == NULL)
       return (FALSE);
    FontListContextNext(p) = fontlist;
    FontListContextError(p) = FALSE;
    *context = p;
    return (TRUE);

}

Boolean XmFontListGetNextFont (context, charset, font)
XmFontContext   context;
XmStringCharSet *charset;
XFontStruct     **font;
{

    int length;

    if (!(context && charset && font))
       return (FALSE);

    if (context->error)
       return (FALSE);

    *font = FontListFont(FontListContextNext(context));
    if (*font == NULL)
    {
       FontListContextError(context) = TRUE;
       return (FALSE);
    }
    else {
        length = strlen(FontListCharset(FontListContextNext(context)));
        *charset = (XmStringCharSet) XtMalloc(length + 1);
        strcpy (*charset, FontListCharset(FontListContextNext(context)));
        (*charset)[length] = '\0';
        FontListContextNext(context)++;
        return (TRUE);
    }
}

void XmFontListFreeFontContext(context)
XmFontContext   context;
{

    if (context)
       XtFree (context);
}








/*
 * general external TCS utilties
 */

XmString
XmStringConcat (a, b)
    XmString a;
    XmString b;
{
    UShort  a_length, b_length;
    unsigned char   *p, *q, *c;
    int     i, j;
    short   head_size;

    if (!a && !b) return ((XmString) NULL);
    if (a == (XmString) NULL) return (XmStringCopy (b));
    if (b == (XmString) NULL) return (XmStringCopy (a));

    a_length = _read_string_length ((unsigned char *) a);
    b_length = _read_string_length ((unsigned char *) b);
    head_size = _calc_header_size(a_length + b_length);

    i = a_length + b_length		 	/* sum + outer component */
	+ head_size;

    p = c = (unsigned char *) XtMalloc (i);
    p = _write_header(p, a_length + b_length);

    q = (unsigned char *) a;				/* start at beginning */
    q = _read_header(q);
    
    for (j=0; j<(a_length); j++) 		/* copy a into new one */
	*p++ = *q++;				/* excluding outer header */

    q = ((unsigned char *) b);				/* skip b's outer HEADER */
    q = _read_header(q);

    for (j=0; j<b_length; j++)	 		/* copy b into new one */
	*p++ = *q++;

    return ((XmString) c);
}




/*
 * concat two external strings.  Only concat a component at a time
 * so that we always wind up with a meaningful string
 */

XmString
XmStringNConcat (first, second, n)
    XmString first, second;
    int n;
{
    unsigned char    *a = (unsigned char *) first;
    unsigned char    *b = (unsigned char *) second;
    XmString string;
    UShort   used, delta;
    unsigned char    *p, *new, *b_end;
    unsigned char    *q ;
    unsigned char    *bp;
    UShort   a_length, b_length;
    short    head_size;
 
    if (!a || !b) return ((XmString) NULL);
    if (n < HEADER) return ((XmString) NULL);
    a_length = _read_string_length(a);
    b_length = _read_string_length(b);

    head_size = _calc_header_size(a_length + b_length);

    if (_is_short_length(a) && 
        (head_size > _read_header_length(a)))
    {    

    /* 
     * If the first string was short enough to use the smaller length
     * field, we have to check that we can still use it for the concatenated
     * version.  Otherwise, the whole string needs to be copied into a
     * large length version
     */

        string = (XmString) _copy_short_to_long(a);
    }
    else {
       /*
        * Whichever length type it is, we will keep it that way.
        */
        string = XmStringCopy((XmString)a);
    }

    used =  _read_string_length ((unsigned char *) string) + 
            _read_header_length ((unsigned char *) string);
    string = (XmString)XtRealloc(string, used + n);

    /*
     * Point to starting location to concat second string into
     */

    p = (unsigned char *) string + used ;

    /*
     * Get a pointer to the string length for update later
     */

    q = (unsigned char *) string;


    /*
     * Point to first component of string
     */

    bp = _read_header(b);

    /*
     * Calculate pointer to end of string
     */

    b_end = ((unsigned char *) b) + _read_string_length (b) + _read_header_length(b);

    while (((_read_component_length (bp) + HEADER) < (n - used)) && 
           (bp < b_end))
    {
	new = _read_component (bp, p, (UShort *) (p + 1), (p + HEADER));

	delta = new - bp;
	used += delta;
	p += delta;
	bp += delta;
    }

    /*
     * Update length field of entrie string
     */
    _write_header(q, used -  _read_header_length((unsigned char *)string));
    string = (XmString)XtRealloc(string, used);

    return (string);
}




XmString
XmStringCopy (string)
    XmString string;
{
    XmString c;
    UShort length;

    if (string == (XmString) NULL) return ((XmString) NULL);

    length = _read_string_length ((unsigned char *) string) + 
             _read_header_length ((unsigned char *) string);

    c = (XmString) XtMalloc (length);

    bcopy (string, c, length);

    return (c);
}





/*
 * copy an external string.  Only copy a component at a time
 * so that we always wind up with a meaningful string
 */

XmString
XmStringNCopy (str, n)
    XmString str;
    int n;
{
    unsigned char    *a = (unsigned char *) str;
    XmString string;
    UShort   used , delta;
    unsigned char    *p, *new, *a_end;
    UShort   *q;
    unsigned char    *ap;
    short    head_size;

    if (!a) return ((XmString) NULL);
    if (n < HEADER) return ((XmString) NULL);
    
    string = (XmString) XtMalloc (n);
    used = _read_header_length(a);
    p = (unsigned char *) string;
    q = (UShort *) string;
    ap = _read_header(a);
    head_size = used;

    p = _write_header (p, (UShort) head_size);

    a_end = ((unsigned char *) a) + _read_string_length (a) + head_size;

    while (((used + _read_component_length (ap) + HEADER) < (n - used)) && 
           (ap < a_end))
    {
	new = _read_component (ap, p, (UShort *) (p + 1), (p + HEADER));

	delta = new - ap;
	used += delta;
	p += delta;
	ap += delta;
    }

    _write_header((unsigned char *) q, used);

    return (string);
}


Boolean
XmStringByteCompare (a1, b1)
    XmString a1, b1;
{
    unsigned char  *a = (unsigned char *) a1;
    unsigned char  *b = (unsigned char *) b1;
    UShort a_length, b_length;

    if ((a == (unsigned char *) NULL) && (b == (unsigned char *) NULL)) return (TRUE);
    if (a == (unsigned char *) NULL) return (FALSE);
    if (b == (unsigned char *) NULL) return (FALSE);

    a_length = _read_string_length (a) + _read_header_length(a);
    b_length = _read_string_length (b) + _read_header_length(b);

    if (a_length < b_length) return (FALSE);
    if (a_length > b_length) return (FALSE);

    return (!memcmp (a, b, a_length));
}


/************************************************************************
 *									*
 * XmStringCompare - compare two strings.  				*
 *									* 
 * Returns TRUE if the strings are equal, FALSE o.w.			*
 *									*
 ************************************************************************/
Boolean
XmStringCompare (a, b)
    XmString a, b;
{
    _XmString _a, _b;
    Boolean ret;

    if (!a && !b) return(TRUE);
    if (!a || !b) return(FALSE);

    _a = _XmStringCreate (a);
    _b = _XmStringCreate (b);
    ret = _XmStringByteCompare(_a, _b);
    _XmStringFree(_a);
    _XmStringFree(_b);
    return(ret);

}





int
XmStringLength (string)
    XmString string;
{
    if (!string) return (0);
    if (!_is_compound(string)) return (0);
    return ((int) _read_string_length ((unsigned char *) string) + 
                  _read_header_length ((unsigned char *) string));
}



Boolean
XmStringEmpty (string)
    XmString string;
{
    
    unsigned char *c;
    unsigned char *end;
    
    if (!string) return (TRUE);
    if (!_is_compound(string)) return (TRUE);
    
    end = ((unsigned char *) string) + _read_string_length ((unsigned char *) string) + 
                               _read_header_length ((unsigned char *) string);
    c = (unsigned char *) _read_header((unsigned char *) string);	/* skip outer tag */

    while (c < end)
    {
	UShort length = _read_component_length (c);

	if ((*c == XmSTRING_COMPONENT_TEXT) && (length > 0)) return (FALSE);

	c += length + HEADER;
    }

    return (TRUE);
}

Boolean XmStringHasSubstring(string, substring)
    XmString  string;
    XmString  substring;
{
    _XmString str;
    _XmString substr;
    Boolean   retvalue;

    if ((string == NULL) || (substring == NULL) || (XmStringEmpty(substring)))
       return (FALSE);

    str = _XmStringCreate(string);
    substr = _XmStringCreate(substring);

    retvalue = _XmStringHasSubstring(str, substr);

    _XmStringFree(str);
    _XmStringFree(substr);
    return (retvalue);
}
/************************************************************************
 *                                                                      *
 * _XmStringIsXmString - returns TRUE if the parameter is an XmString.   *
 *                                                                      *
 ************************************************************************/
Boolean _XmStringIsXmString(string)
XmString    string;
{
    if (string == NULL) return(FALSE);
    return (_is_compound(string));
}

/*
 * determines from ASN.1 header whether this is an external compound string.
 * returns T or F.
 */

static Boolean
_is_compound(string)
XmString    string;
{

    unsigned char * uchar_p = (unsigned char *) string;


   /*
    *  Start with comparing the ASN.1 header.
    */

    return (strncmp (uchar_p, CSHeader, CSHEADERLEN) == 0);
            

}


/*
 * internal structure access routines
 */

Boolean _XmStringInitContext(context, string)
    _XmStringContext  *context;
    _XmString         string;
{
 
    _XmStringContext  ct;


    /* make sure there is something in the string.  we are
       going to assume a good string in the get next routine
     */
    if (!(string && context)) return (FALSE);

    ct = (_XmStringContext) XtMalloc(sizeof(_XmStringContextRec));

    _XmStrContError(ct)     = FALSE;
    _XmStrContCurrLine(ct)  = 0;
    _XmStrContCurrSeg(ct)   = 0;
    _XmStrContString(ct)    = string;
    _XmStrContOpt(ct)       = (_XmStrLineCnt(string) == 0);

    *context = ct;
    return (TRUE);

}

Boolean _XmStringGetNextSegment(context, charset, direction, text, char_count,
          separator)
    _XmStringContext  context;
    XmStringCharSet   *charset;
    XmStringDirection *direction;
    char              **text;
    short             *char_count;
    Boolean           *separator;
{

    if (! (context && charset && direction && text && char_count && separator))
       return (FALSE);

    if (_XmStrContError(context))
       return(FALSE);

    if (_XmStrContOpt(context))
    {
        _XmStringOpt  str = (_XmStringOpt) (_XmStrContString(context));
        int           charsetlen = strlen(_XmOptCharset(str));

        *charset = XtMalloc(charsetlen + 1);
        bcopy(_XmOptCharset(str), *charset, charsetlen);
        (*charset)[charsetlen] = '\0';

        *char_count = _XmOptCharCount(str);
        *text = XtMalloc(*char_count + 1);
        bcopy(_XmOptText(str), *text, *char_count);
        (*text)[*char_count] = '\0';

        *direction = _XmOptDirection(str);
        *separator = FALSE;

        /* 
         * Always set this to true.  Optimized strings only have one
         * segment, so if they try for another, it's an error.
         */        
        _XmStrContError(context) = TRUE;
        return (TRUE);
    }
       /* not optimized */
    else {

        _XmString        str = _XmStrContString(context);
        _XmStringSegment seg;
        _XmStringLine    line;

        if (_XmStrContCurrLine(context) >= _XmStrLineCnt(str))
        {
            _XmStrContError(context) = TRUE;
            return (FALSE);
        }
        else {
            line = &(_XmStrLineLine(str)[_XmStrContCurrLine(context)]);
            seg = &(_XmStrLineSegment(line)[_XmStrContCurrSeg(context)]);

            *charset = XtMalloc(strlen(_XmSegCharset(seg)) + 1);
            bcopy (_XmSegCharset(seg), *charset, strlen(_XmSegCharset(seg)));
            (*charset)[strlen(_XmSegCharset(seg))] = '\0';

            *char_count = _XmSegCharCount(seg);
            *text = XtMalloc(*char_count + 1);
            bcopy (_XmSegText(seg), *text, *char_count);
            (*text)[*char_count] = '\0';

            *direction = _XmSegDirection(seg);
            *separator = FALSE;

            _XmStrContCurrSeg(context)++;
            if (_XmStrContCurrSeg(context) >= _XmStrLineSegCount(line))
            {
               *separator = TRUE;
               _XmStrContCurrSeg(context) = 0;
               _XmStrContCurrLine(context)++;
            }
        }
   
    }
    return (TRUE);
}

void _XmStringFreeContext(context)
    _XmStringContext  context;
{
    XtFree(context);
}




/*
 * optimized internal TCS structure handling routines
 */

static Dimension
_XmStringOptLineAscender (f, opt)
XmFontList     f;
_XmStringOpt   opt;
{
    int max = 0;

    if (_XmOptFontIndex(opt) == -1) _update_opt (f, opt);

    if (_XmOptFontIndex(opt) >= 0)
    {
      if (FontListFont(&f[_XmOptFontIndex(opt)])->ascent > max)
             max = FontListFont(&f[_XmOptFontIndex(opt)])->ascent;
    }
    return (max);
}


static Dimension
_XmStringOptLineDescender (f, opt)
XmFontList     f;
_XmStringOpt   opt;
{
    int max = 0;

    if (_XmOptFontIndex(opt) == -1) _update_opt (f, opt);

    if (_XmOptFontIndex(opt) >= 0)
    {
	if (FontListFont(&f[_XmOptFontIndex(opt)])->descent > max)
	    max = FontListFont(&f[_XmOptFontIndex(opt)])->descent;
    }
    return (max);
}


static Dimension
_XmStringOptLineWidth (fontlist, optline)
XmFontList     fontlist;
_XmStringOpt   optline;
{
    int width = 0;

    if (_XmOptFontIndex(optline) == -1) _update_opt (fontlist, optline);

    width += _XmOptPixelWidth(optline);
    return (width);
}


static Dimension
_XmStringOptLineHeight (fontlist, optline)
XmFontList     fontlist;
_XmStringOpt   optline;
{
    int height = 0;

    if (_XmOptFontIndex(optline) == -1) _update_opt (fontlist, optline);

    if (_XmOptFontIndex(optline) != -1)
    {
        XFontStruct *font_struct = 
                 FontListFont(&fontlist[_XmOptFontIndex(optline)]);
        
        height =   font_struct->ascent
                 + font_struct->descent;
    }

    return (height);
}



/*
 * internal TCS structure handling routines
 */
/*
 * find biggest ascender in this line
 */

static Dimension
_XmStringLineAscender (f, line)
    XmFontList f;
    _XmStringLine line;
{
    int i, max = 0;

    for (i=0; i<_XmStrLineSegCount(line); i++)
    {
	_XmStringSegment seg = &(_XmStrLineSegment(line)[i]);

	if (_XmSegFontIndex(seg) == -1) _update_segment (f, seg);

	if (_XmSegFontIndex(seg) >= 0)
	{
	   if (FontListFont(&f[_XmSegFontIndex(seg)])->ascent > max)
              max = FontListFont(&f[_XmSegFontIndex(seg)])->ascent;
	}
    }
    return (max);
}




/*
 * find biggest descender in this line
 */

static Dimension
_XmStringLineDescender (f, line)
    XmFontList f;
    _XmStringLine line;
{
    int i, max = 0;

    for (i=0; i<_XmStrLineSegCount(line); i++)
    {
	_XmStringSegment seg = &(_XmStrLineSegment(line)[i]);

	if (_XmSegFontIndex(seg) == -1) _update_segment (f, seg);

	if (_XmSegFontIndex(seg) >= 0)
	{
	  if (FontListFont(&f[_XmSegFontIndex(seg)])->descent > max)
	     max = FontListFont(&f[_XmSegFontIndex(seg)])->descent;
	}
    }
    return (max);
}



/*
 * find the total length of this line
 */

static Dimension
_XmStringLineWidth (fontlist, line)
    XmFontList fontlist;
    _XmStringLine line;
{
    int i, width = 0;

    for (i=0; i<_XmStrLineSegCount(line); i++)
    {
	_XmStringSegment seg = &(_XmStrLineSegment(line)[i]);

	if (_XmSegFontIndex(seg) == -1) _update_segment (fontlist, seg);

	width += _XmSegPixelWidth(seg);
    }
    return (width);
}



/*
 * find the tallest segment in this line
 */

static Dimension
_XmStringLineHeight (fontlist, line)
    XmFontList fontlist;
    _XmStringLine line;
{

    XFontStruct *font_struct;
    int i, height, max_height = 0;

    for (i=0; i<_XmStrLineSegCount(line); i++)
    {
	_XmStringSegment seg = &(_XmStrLineSegment(line)[i]);

	if (_XmSegFontIndex(seg) == -1) _update_segment (fontlist, seg);

        font_struct = FontListFont(&fontlist[_XmSegFontIndex(seg)]);        
	height =   font_struct->ascent
	         + font_struct->descent;
	if (height > max_height) max_height = height;

    }

    return (max_height);
}



/*
 * find width of widest line in internal TCS
 */

Dimension
_XmStringWidth (fontlist, string)
    XmFontList fontlist;
    _XmString  string;
{
    int j;
    Dimension max_width = 0;

    if (_XmStrLineCnt(string) == 0)
       max_width = _XmStringOptLineWidth(fontlist, (_XmStringOpt) string);
    else

        for (j=0; j<_XmStrLineCnt(string); j++)
        {
	    Dimension width = _XmStringLineWidth 
                  (fontlist, &(_XmStrLineLine(string)[j]));

	    if (width > max_width) max_width = width;
        }
    return (max_width);
}

Dimension
_XmStringHeight (fontlist, string)
    XmFontList fontlist;
    _XmString string;
{
    Dimension height = 0;
    int j;

    if (_XmStrLineCnt(string) == 0)
	height = _XmStringOptLineHeight (fontlist, (_XmStringOpt) string);

    else
        for (j=0; j<_XmStrLineCnt(string); j++)
        {
	    height += _XmStringLineHeight (fontlist, 
                      &(_XmStrLineLine(string)[j]));
        }

    return (height);
}



/*
 * find the rectangle which will enclose the text 
 */

void
_XmStringExtent (fontlist, string, width, height)
    XmFontList fontlist;
    _XmString string;
    Dimension *width, *height;
{
    *width  = _XmStringWidth  (fontlist, string);
    *height = _XmStringHeight (fontlist, string);
}



Boolean
_XmStringEmpty (string)
    _XmString string;
{
    int i, j;

    if (!string) return (TRUE);

    if (_XmStrLineCnt(string) == 0)
    {
        if (_XmOptCharCount((_XmStringOpt)string) > 0) 
            return FALSE;
    }
    else {
        _XmStringLine  line = _XmStrLineLine(string);

        for (i=0; i<_XmStrLineCnt(string); i++)
        {
            int segcount = _XmStrLineSegCount(&line[i]);
   	    for (j = 0; j < segcount; j++)
       	    {
	        _XmStringSegment seg = 
                   &(_XmStrLineSegment(&line[i])[j]);

	        if (_XmSegCharCount(seg) > 0) return (FALSE);
            }
        }
    }

    return (TRUE);
}




/*
 * figure out if there is sub string match, and if so the begining
 * and end of the match section in pixels.  Don't touch anything if
 * there is no match
 */

static void
#ifdef _NO_PROTO
_XmStringSubStringPosition (one_byte, font, seg, under_seg, 
	x, under_begin, under_end)
    Boolean one_byte;
    XFontStruct *font;
    _XmStringSegment seg, under_seg;
    Position x;
    Dimension *under_begin, *under_end;		/* output */
#else /* _NO_PROTO */
_XmStringSubStringPosition (Boolean one_byte, XFontStruct *font,
			    _XmStringSegment seg, _XmStringSegment under_seg, 
			    Position x, Dimension *under_begin, 
			    Dimension *under_end)
#endif /* _NO_PROTO */
{

    char *a = _XmSegText(seg), *b = _XmSegText(under_seg);
    int i, j, begin, max;
    Boolean fail;

    if (_XmSegCharset(seg) != _XmSegCharset(under_seg)) return;

    if (_XmSegCharCount(seg) < _XmSegCharCount(under_seg)) return;

    max = (_XmSegCharCount(seg) - _XmSegCharCount(under_seg));
    if (one_byte)
    {
	for (i = 0; i <= max; i++)
	{
	    fail = FALSE;
   	    begin = i;

	    for (j = 0; j < _XmSegCharCount(under_seg); j++)
	    {
		if (a[i+j] != b[j]) 
		{
		    fail = TRUE;
		    break;
		}
	    }
	    if ( ! fail)				/* found it */
	    {
		if (begin == 0)
		    *under_begin = x;
		else
		    *under_begin = x + XTextWidth (font, a, begin);

		if (_XmSegPixelWidth(under_seg) == 0)
		    _XmSegPixelWidth(under_seg) = XTextWidth (
			font, b, _XmSegCharCount(under_seg));

		*under_end = *under_begin + _XmSegPixelWidth(under_seg);

		return;
	    }
 	}
    }
    else
    {

        /*
         * If either string isn't even byte length, it can't be
         * two bytes/char.
         */

        if (((_XmSegCharCount(seg) % 2) != 0) || 
            ((_XmSegCharCount(under_seg) % 2) != 0))
            return;

        /*
         * search for the substring
         */

	for (i = 0; i <= max; i+=2)
	{
	    fail = FALSE;
   	    begin = i;

	    for (j = 0; j < _XmSegCharCount(under_seg); j+=2)
	    {
		if ((a[i+j] != b[j]) || (a[i+j+1] != b[j+1]))
		{
		    fail = TRUE;
		    break;
		}
	    }
	    if ( ! fail)				/* found it */
	    {
		if (begin == 0)
		    *under_begin = x;
		else
		    *under_begin = x + XTextWidth16 (font, (XChar2b *)a, begin/2);

		if (_XmSegPixelWidth(under_seg) == 0)
		    _XmSegPixelWidth(under_seg) = XTextWidth16 (
			font, (XChar2b *)b, _XmSegCharCount(under_seg)/2);

		*under_end = *under_begin + _XmSegPixelWidth(under_seg);

		return;
	    }
 	}
    }
}





/*
 * draw a single internal TCS segment
 */

static void 
#ifdef _NO_PROTO
_XmStringDrawSegment (d, w, x, y, seg, gc, fontlist, image, 
	underline, under_begin, under_end)
    Display *d;
    Window w;
    int x, y;
    _XmStringSegment seg;
    GC  gc;
    XmFontList fontlist;
    Boolean image;
    _XmString underline;
    Dimension *under_begin, *under_end;
#else /* _NO_PROTO */
_XmStringDrawSegment (Display *d, Window w, int x, int y, _XmStringSegment seg,
		      GC gc, XmFontList fontlist, Boolean image, 
		      _XmString underline, Dimension *under_begin, 
		      Dimension *under_end)
#endif /* _NO_PROTO */
{
    XFontStruct *f;
    Boolean text16;
    Font    oldfont = NULL;
    XGCValues xgcv;
    char *save_text = seg->text;			/* this is slimy */
    char  flip_char[100];				/* but simple */
/* BEGIN OSF Fix pir 2699 */
    char *flip_char_extra = NULL;
/* END OSF Fix pir 2699 */

    if (_XmSegFontIndex(seg) == -1) _update_segment (fontlist, seg);

    if ((_XmSegFontIndex(seg) == -1) || (_XmSegCharCount(seg) == 0)) return;

    f = FontListFont(&fontlist[_XmSegFontIndex(seg)]);
    text16 = two_byte_font (f);

    xgcv.font = f->fid;				/* get segment font */

    if (gc->values.font != xgcv.font)		/* not right one */
    {						/* change it */
        oldfont = gc->values.font;
        XChangeGC (d, gc, GCFont, &xgcv);
    }

    if (_XmSegDirection(seg) == XmSTRING_DIRECTION_R_TO_L)
/* BEGIN OSF Fix pir 2699 */
      /* Flip the bytes. */
      {
	char *p = flip_char, *q;
	int i;
	if (_XmSegCharCount(seg) > 100) 
	  p = flip_char_extra = (char *) XtMalloc (_XmSegCharCount(seg));
      
	_XmSegText(seg) =  p;	/* change seg for a while */

	if ( ! text16)
	  {
	    for (i=0, q = (save_text + _XmSegCharCount(seg) - 1); 
		 i < _XmSegCharCount(seg); 
		 i++) 
	      *p++ = *q--;
	  }
	else	/* Have to flip two at a time, maintaining their order. */
	  {
	    char tmp;
	    
	    for (i=0, q = (save_text + _XmSegCharCount(seg) - 1); 
		 i < Half (_XmSegCharCount(seg)); 
		 i++) 
	      {
		tmp = *q--;
		*p++ = *q--;
		*p++ = tmp;
	      }
/* END OSF Fix pir 2699 */
	  }
      }

    if ((underline != (_XmString) NULL) && (*under_begin == *under_end))
    {

        if (_XmStrLineCnt(underline) == 0)
        {

            /*
             * This is an optimized string; build a segment and call the
	     * sub-string search routine.
             */

            _XmStringSegmentRec  under_seg;
            _XmStringOpt         opt;

            opt = (_XmStringOpt) underline;
	    _XmSegCharset(&under_seg) = _XmOptCharset(opt);
            _XmSegFontIndex(&under_seg) = _XmOptFontIndex(opt);
            _XmSegCharCount(&under_seg) = _XmOptCharCount(opt);
            _XmSegText(&under_seg) = _XmOptText(opt);
            _XmSegDirection(&under_seg) = _XmOptDirection(opt);
            _XmSegPixelWidth(&under_seg) = _XmOptPixelWidth(opt);
	    _XmStringSubStringPosition (
		    (! text16), f, seg, &under_seg, x, under_begin, under_end);
        }
        else {
            _XmStringLine line = _XmStrLineLine(underline);
	    if ((_XmStrLineCnt(underline) > 0) && 
	        (_XmStrLineSegCount(&line[0]) > 0))
	    {
	        _XmStringSegment under_seg =  _XmStrLineSegment(line);

	        _XmStringSubStringPosition (
		    (! text16), f, seg, under_seg, x, under_begin, under_end);
	    }
        }
    }

    if (image)
    {
	if (text16)
            XDrawImageString16 (d, w, gc, x, y, (XChar2b *)_XmSegText(seg), 
				Half (_XmSegCharCount(seg)));
        else
            XDrawImageString (d, w, gc, x, y, 
                         _XmSegText(seg), _XmSegCharCount(seg));
    }
    else
    {
	if (text16)
            XDrawString16 (d, w, gc, x, y, (XChar2b *) _XmSegText(seg), 
			   Half (_XmSegCharCount(seg)));
        else
            XDrawString (d, w, gc, x, y, _XmSegText(seg), _XmSegCharCount(seg));
    }

    if (oldfont)                                /* if font was changed */
    {                                           /* put it back */
        xgcv.font = oldfont;
        XChangeGC (d, gc, GCFont, &xgcv);
    }

    _XmSegText(seg) = save_text;			/* put seg back */

    if (flip_char_extra  != NULL) XtFree (flip_char_extra);
/* BEGIN OSF Fix pir 2699 */
/* END OSF Fix pir 2699 */
}



/* 
 * Draw a single internal TCS line
 */

static void
#ifdef _NO_PROTO
_XmStringDrawLine (d, w, x, y, line, gc, fontlist, image, 
	underline, under_begin, under_end, opt)
    Display       *d;
    Window        w;
    int           x, y;
    _XmStringLine line;
    GC            gc;
    XmFontList    fontlist;
    Boolean       image;
    _XmString     underline;
    Dimension     *under_begin, *under_end;
    Boolean       opt;
#else /* _NO_PROTO */
_XmStringDrawLine (Display *d, Window w, int x, int y, _XmStringLine line, 
		   GC gc, XmFontList fontlist, Boolean image, 
		   _XmString underline, Dimension *under_begin, 
		   Dimension *under_end, Boolean opt)
#endif /* _NO_PROTO */
{
    int i;

    if (opt)
    {
        /*
         * This is optimized; build a full segment and call the drawing
	 * routine.
         */

        _XmStringOpt         optline = (_XmStringOpt) line;
        _XmStringSegmentRec  segm;

        _XmSegCharset(&segm) = _XmOptCharset(optline);
        _XmSegFontIndex(&segm) = _XmOptFontIndex(optline);
        _XmSegCharCount(&segm) = _XmOptCharCount(optline);
        _XmSegText(&segm) = _XmOptText(optline);
        _XmSegDirection(&segm) = _XmOptDirection(optline);
        _XmSegPixelWidth(&segm) = _XmOptPixelWidth(optline);

	_XmStringDrawSegment (d, w, x, y, &segm, gc, fontlist, image,
		underline, under_begin, under_end);

    }
    else {
        for (i=0; i<_XmStrLineSegCount(line); i++)
        {
            _XmStringSegment seg;

	    seg = &(_XmStrLineSegment(line)[i]);

	    _XmStringDrawSegment (d, w, x, y, seg, gc, fontlist, image,
		underline, under_begin, under_end);

	    x += _XmSegPixelWidth(seg);
        }
    }
}

/*
 * calculate the alignment, position and clipping for the string
 */

static void
#ifdef _NO_PROTO
_calc_align_and_clip(d, gc, x, y, width, line_width, lay_dir, clip, align,
		     descender, restore)
    Display    *d;
    GC         gc;
    Position   *x;
    Position   y;
    Dimension  width;
    int        line_width;
    unsigned char      lay_dir;
    XRectangle *clip;
    unsigned char      align;
    int        descender;
    int        *restore;
#else /* _NO_PROTO */
_calc_align_and_clip(Display *d, GC gc, Position *x, Position y, 
		     Dimension width, int line_width, unsigned char lay_dir,
		     XRectangle *clip, unsigned char align, int descender, 
		     int *restore)
#endif /* _NO_PROTO */
{

    Boolean l_to_r = (lay_dir == XmSTRING_DIRECTION_L_TO_R);


    switch (align)
    {
    	case XmALIGNMENT_BEGINNING:
	    if ( ! l_to_r) *x += width - line_width;
	    break;

    	case XmALIGNMENT_CENTER:
	    *x += Half (width) - Half (line_width);
	    break;

    	case XmALIGNMENT_END :
	    if (l_to_r)
	    	*x += width - line_width;
	    break;
   }

    if ((clip != NULL) && ( ! *restore))

        if ((line_width > width) ||
	     (y + descender) > (clip->y + clip->height))
	{
	    *restore = TRUE;
            XSetClipRectangles (d, gc, 0, 0, clip, 1, YXBanded);
	}

}



/*
 * draw a complete internal format TCS
 */

static void
#ifdef _NO_PROTO
_draw (d, w, fontlist, string, gc, x, y, width, 
	align, lay_dir, clip, image, underline)
    Display	*d;
    Window 	w;
    XmFontList fontlist;
    _XmString	string;
    GC		gc;
    Position	x, y;
    Dimension	width;
    unsigned char	align;
    unsigned char	lay_dir;
    XRectangle	*clip;
    Boolean 	image;
    _XmString	underline;
#else /* _NO_PROTO */
_draw (Display *d, Window w, XmFontList fontlist, _XmString string, GC gc, 
       Position x, Position y, Dimension width, unsigned char align, 
       unsigned char lay_dir, XRectangle *clip, Boolean image, 
       _XmString underline)
#endif /* _NO_PROTO */
{
    int base_x = x, line_width, descender;
    Position draw_x;
    static _XmStringLine line;
    int i;
    int restore_clip = FALSE;
    Dimension under_begin = 0, under_end = 0;

/* BEGIN OSF Fix pir 2115 */
    if (string) 
/* END OSF Fix pir 2115 */
      {
	if (_XmStrLineCnt(string) == 0)
	  {
	    /* baseline of this line */
	    y += _XmStringOptLineAscender (fontlist, (_XmStringOpt) string);

	    descender = _XmStringOptLineDescender (fontlist, (_XmStringOpt) string);

	    line_width = _XmStringOptLineWidth (fontlist, (_XmStringOpt) string);
	    if (line_width != 0)
	      {
		draw_x = base_x; /* most left position */

		_calc_align_and_clip(d, gc, &draw_x, y, width, line_width, 
				     lay_dir, clip, align, descender, &restore_clip);

		_XmStringDrawLine (d, w, draw_x, y, (_XmStringLine) string, 
				   gc, fontlist, image,
				   underline, &under_begin, &under_end,TRUE);
	      }

	    y += descender;	/* go to bottom of this line */

	    if ((underline != NULL) && (under_begin != under_end))
	      XDrawLine (d, w, gc, under_begin, y, under_end, y);

	  }
	else {
	  for (i=0; i < _XmStrLineCnt(string); i++)
	    {
	      line = &(_XmStrLineLine(string)[i]);

	      /* baseline of this line */	
	      y += _XmStringLineAscender (fontlist, line);

	      descender = _XmStringLineDescender (fontlist, line);

	      line_width = _XmStringLineWidth (fontlist, line);

	      if (line_width != 0)
		{
		  draw_x = base_x; /* most left position */

		  _calc_align_and_clip(d, gc, &draw_x, y, width, line_width, 
				       lay_dir, clip, align, descender, &restore_clip);

		  _XmStringDrawLine (d, w, draw_x, y, line, gc, fontlist, image,
				     underline, &under_begin, &under_end,FALSE);

		}

	      y += descender;	/* go to bottom of this line */

	      if ((underline != NULL) && (under_begin != under_end))
		{
		  underline = (_XmString) NULL;	/* only once */

		  XDrawLine (d, w, gc, under_begin, y, under_end, y);
		}
	    }

	}
	if (restore_clip) XSetClipMask (d, gc, None); 
/* BEGIN OSF Fix pir 2115 */
      }
/* END OSF Fix pir 2115 */
}




void
#ifdef _NO_PROTO
_XmStringDraw (d, w, fontlist, string, gc, x, y, width, align, lay_dir, clip)
    Display	*d;
    Window 	w;
    XmFontList fontlist;
    _XmString	string;
    GC		gc;
    Position	x, y;
    Dimension	width;
    unsigned char	align;
    unsigned char	lay_dir;
    XRectangle	*clip;
#else /* _NO_PROTO */
_XmStringDraw (Display *d, Window w, XmFontList fontlist, _XmString string, 
	       GC gc, Position x, Position y, Dimension width, 
	       unsigned char align, unsigned char lay_dir, XRectangle *clip)
#endif /* _NO_PROTO */
{
    _draw (d, w, fontlist, string, gc, x, y, width, 
	align, lay_dir, clip, FALSE, NULL);
}

void
#ifdef _NO_PROTO
_XmStringDrawImage (d, w, fontlist, string, gc, x, y, width, 
	align, lay_dir, clip)
    Display	*d;
    Window 	w;
    XmFontList fontlist;
    _XmString	string;
    GC		gc;
    Position	x, y;
    Dimension	width;
    unsigned char	align;
    unsigned char	lay_dir;
    XRectangle	*clip;
#else /* _NO_PROTO */
_XmStringDrawImage (Display *d, Window w, XmFontList fontlist, 
		    _XmString string, GC gc, Position x, Position y, 
		    Dimension width, unsigned char align, 
		    unsigned char lay_dir, XRectangle *clip)
#endif /* _NO_PROTO */
{
    _draw (d, w, fontlist, string, gc, x, y, width, 
	align, lay_dir, clip, TRUE, NULL);
}



void
#ifdef _NO_PROTO
_XmStringDrawUnderline (d, w, f, s, gc, x, y, width, 
	align, lay_dir, clip, u)
    Display	*d;
    Window 	w;
    XmFontList f;
    _XmString	s;
    GC		gc;
    Position	x, y;
    Dimension	width;
    unsigned char	align;
    unsigned char	lay_dir;
    XRectangle	*clip;
    _XmString	u;
#else /* _NO_PROTO */
_XmStringDrawUnderline (Display *d, Window w, XmFontList f, _XmString s, 
			GC gc, Position x, Position y, Dimension width, 
			unsigned char align, unsigned char lay_dir, 
			XRectangle *clip, _XmString u)
#endif /* _NO_PROTO */
{
    _draw (d, w, f, s, gc, x, y, width, 
	align, lay_dir, clip, FALSE, u);
}


#ifdef _NO_PROTO
void _XmStringDrawMnemonic(d, w, fontlist, string, gc, x, y, width, 
	align, lay_dir, clip, mnemonic, charset)
    Display	*d;
    Window 	w;
    XmFontList fontlist;
    _XmString	string;
    GC		gc;
    Position	x, y;
    Dimension	width;
    unsigned char	align;
    unsigned char	lay_dir;
    XRectangle	*clip;
    String	mnemonic;
    XmStringCharSet charset;
#else /* _NO_PROTO */
void _XmStringDrawMnemonic(Display *d, Window w, XmFontList fontlist, 
			   _XmString string, GC gc, Position x, Position y, 
			   Dimension width, unsigned char align, 
			   unsigned char lay_dir, XRectangle *clip, 
			   String mnemonic, XmStringCharSet charset)
#endif /* _NO_PROTO */
{
    
    XmString  mne_string;
    _XmString underline;
 
    mne_string = XmStringCreate(mnemonic, charset);
    underline = _XmStringCreate(mne_string);
    XmStringFree(mne_string);
 
    _draw (d, w, fontlist, string, gc, x, y, width, 
	align, lay_dir, clip, FALSE, underline);
    _XmStringFree(underline);
}

/*
 * build the internal TCS given the external TCS
 */



static void
new_segment (string, line_index, value)
    _XmString string;
    int line_index;
    _XmStringSegment value;
{
    _XmStringLine line = &(_XmStrLineLine(string)[line_index]);
    _XmStringSegment seg;
    int sc = _XmStrLineSegCount(line);

    _XmStrLineSegment(line) = 
          (_XmStringSegment) XtRealloc (_XmStrLineSegment(line), 
				sizeof (_XmStringSegmentRec) * (sc+1));

    seg = &(_XmStrLineSegment(line)[sc]);

    *seg = *value;

    _XmStrLineSegCount(line)++;
}


static void
new_line (string)
    _XmString string;
{
    int lc = _XmStrLineCnt(string);

    _XmStrLineLine(string) = (_XmStringLine) 
		XtRealloc (_XmStrLineLine(string), 
                sizeof (_XmStringLineRec) * (lc + 1));

    _XmStrLineSegCount(&(_XmStrLineLine(string)[lc])) = 0;
    _XmStrLineSegment(&(_XmStrLineLine(string)[lc])) = NULL;

    _XmStrLineCnt(string)++;
}


#define _init_segment(seg)				\
{							\
    _XmSegFontIndex(seg)   = -1;			\
    _XmSegCharCount(seg)   = 0;				\
    _XmSegText(seg)         = NULL;			\
    _XmSegPixelWidth(seg)  = 0;				\
}


static _XmString
#ifdef _NO_PROTO
_XmStringOptCreate(c, end, textlen, havecharset)
    unsigned char  *c;
    unsigned char  *end;    
    UShort textlen;
    Boolean havecharset;
#else /* _NO_PROTO */
_XmStringOptCreate(unsigned char *c, unsigned char *end, UShort textlen, 
		   Boolean havecharset)
#endif /* _NO_PROTO */
{
    _XmStringOpt  string;
    char          *charset;
    UShort        length;

    string = (_XmStringOpt) XtMalloc (sizeof (_XmStringOptRec)+textlen);

    _XmOptLineCount(string) = 0;    /* always zero for optimized */
    _XmOptFontIndex(string) = -1;
    _XmOptCharCount(string) = textlen;
    _XmOptPixelWidth(string) = 0;
    if (!havecharset)
    {
         charset = _XmStringGetCurrentCharset();
         _XmOptCharset(string) = _cache_charset((char *) charset, 
                                                strlen(charset));
    }
    _XmOptDirection(string) = XmSTRING_DIRECTION_L_TO_R;

    while (c < end)
    {
	length = _read_component_length (c);

	switch (*c)
	{
	    case XmSTRING_COMPONENT_CHARSET:
		_XmOptCharset(string) = _cache_charset 
                                        ((char *)(c + HEADER), (int) length);
		break;

	    case XmSTRING_COMPONENT_TEXT:
	 	bcopy ((c + HEADER), _XmOptText(string), textlen);
		break;

	    case XmSTRING_COMPONENT_DIRECTION:		/* record dir */
		_XmOptDirection(string) = (XmStringDirection) *(c + HEADER);
		break;

	    case XmSTRING_COMPONENT_SEPARATOR:		/* start new line */
                XtFree(string);
                return (NULL);
		break;

	    default:
		break;
	}

	c += length + HEADER;
    }

    if (!havecharset)
       XtFree(charset);

    return((_XmString) string);
}

static _XmString
#ifdef _NO_PROTO
_XmStringNonOptCreate(c, end, havecharset)
    unsigned char  *c;
    unsigned char  *end;
    Boolean havecharset;
#else /* _NO_PROTO */
_XmStringNonOptCreate(unsigned char *c, unsigned char *end, 
		      Boolean havecharset)
#endif /* _NO_PROTO */
{

    int lc;
    _XmStringSegmentRec seg;
    UShort length;
    _XmString string ;
    char *charset;

    string = (_XmString) XtMalloc (sizeof (_XmStringRec));

    _XmStrLineCnt(string) = 0;			/* init root */
    _XmStrLineLine(string) = (_XmStringLine) NULL;

    new_line (string);

    _init_segment (&seg);

    if (!havecharset)
    {
       charset = _XmStringGetCurrentCharset();
       _XmSegCharset(&seg) = _cache_charset((char *) (charset), 
                                            (int) strlen(charset));
    }  
    _XmSegDirection(&seg) = XmSTRING_DIRECTION_L_TO_R;

    lc = 0;

    while (c < end)
    {
	length = _read_component_length (c);

	switch (*c)
	{
	    case XmSTRING_COMPONENT_CHARSET:
		_XmSegCharset(&seg) = _cache_charset 
                                      ((char *) (c + HEADER), (int) length);
		break;

	    case XmSTRING_COMPONENT_TEXT:
		_XmSegText(&seg) = XtMalloc (length);
	 	bcopy ((c + HEADER), _XmSegText(&seg), length);

		_XmSegCharCount(&seg) = length;

/****************
 *
 * Why are they doing this?? It forces the text to be last.
 *
 ****************/
		new_segment (string, lc, &seg);		/* alloc new one and */
							/* copy static to it */
		_init_segment (&seg);  
		break;

	    case XmSTRING_COMPONENT_DIRECTION:		/* record dir */
		_XmSegDirection(&seg) = (XmStringDirection) *(c + HEADER);
		break;

	    case XmSTRING_COMPONENT_SEPARATOR:		/* start new line */
		new_line (string);
		lc++;
		break;

	    default:
		break;
	}

	c += length + HEADER;
    }

    if (!havecharset)
       XtFree(charset);

    return(string);
}



_XmString
_XmStringCreate (cs)
    XmString 	cs;					/* external TCS */
{
    unsigned char       *c;
    unsigned char       *c_opt;
    unsigned char       *end;
    UShort      length;
    UShort      txtlength;
    _XmString   string ;
    Boolean     continue_flag;
    Boolean     optimized;
    Boolean     havecharset;
 
    if (!cs) return((_XmString) NULL);
    if (!_is_compound(cs)) return ((_XmString) NULL);

    c  = (unsigned char *) _read_header((unsigned char *) cs);
    end = c + _read_string_length ((unsigned char *) cs);
    if (c >= end) return ((_XmString) NULL);
   
    /*
     * In order to build an optimized string, we have to see if this one
     * qualifies.  Do some preprocessing to see.
     * We also need to know if this CS contains a character set component,
     * so look for that too.
     */

    c_opt = c;
    continue_flag = TRUE;
    optimized = TRUE;
    txtlength = 0;		/* For strings with no text component. */
    havecharset = FALSE;
    while (continue_flag)
    {
	length = _read_component_length (c_opt);

	switch (*c_opt)
	{

	    case XmSTRING_COMPONENT_TEXT:
                if ((c_opt + length + HEADER) < end)
                   optimized = FALSE;
                txtlength = length;
		break;

	    case XmSTRING_COMPONENT_SEPARATOR:		/* start new line */
                optimized = FALSE;
		break;

            case XmSTRING_COMPONENT_CHARSET:
                havecharset = TRUE;
                break;

	    default:
		break;
	}

	c_opt += length + HEADER;
        if ((c_opt >= end) || (!optimized))
           continue_flag = FALSE;
    }

  

    if (optimized)
        string = (_XmString) _XmStringOptCreate(c, end, txtlength, havecharset);
    else string = _XmStringNonOptCreate(c, end, havecharset);   

    return (string);
}



/*
 * free the TCS internal data structure
 */

void 
_XmStringFree (string)
    _XmString string;
{
    int i, j;

    if (!string) return;

    if (_XmStrLineCnt(string) != 0)
    {
        _XmStringLine line = _XmStrLineLine(string);

        for (i=0; i<_XmStrLineCnt(string); i++)
        {
            _XmStringSegment seg = _XmStrLineSegment(&line[i]);
            int              segcount = _XmStrLineSegCount(&line[i]);

	    for (j = 0; j < segcount; j++)
                XtFree (_XmSegText(&seg[j]));
            XtFree (seg);
        }
        XtFree (_XmStrLineLine(string));
    }
    XtFree (string);
}

void 
XmStringFree (string)
    XmString string;
{
    XtFree (string);
}


static void
#ifdef _NO_PROTO
_update_opt (fontlist, optline)
    XmFontList    fontlist;
    _XmStringOpt  optline;
#else /* _NO_PROTO */
_update_opt (XmFontList fontlist, _XmStringOpt optline)
#endif /* _NO_PROTO */
{

    XFontStruct *font_struct;

    if (_XmOptFontIndex(optline) == -1)
    {
	_XmFontListSearch (fontlist, _XmOptCharset(optline), 
		&(_XmOptFontIndex(optline)), &font_struct);
    }

    if (_XmOptFontIndex(optline) != -1)
    {
    	if (two_byte_font (font_struct))
	{
            if (_XmOptCharCount(optline) < 2)
                _XmOptPixelWidth(optline) = 1;
            else
                _XmOptPixelWidth(optline) = 
		  XTextWidth16 (font_struct, (XChar2b *)_XmOptText(optline), 
				Half (_XmOptCharCount(optline)));
	}
	else
	{
            if (_XmOptCharCount(optline) < 1)
                _XmOptPixelWidth(optline) = 1;
            else
     	        _XmOptPixelWidth(optline) = XTextWidth (font_struct, 
			_XmOptText(optline), _XmOptCharCount(optline));
	}
    }
}


static void
#ifdef _NO_PROTO
_update_segment (fontlist, seg)
    XmFontList fontlist;
    _XmStringSegment seg;
#else /* _NO_PROTO */
_update_segment (XmFontList fontlist, _XmStringSegment seg)
#endif /* _NO_PROTO */
{
    XFontStruct *font_struct;

    if (_XmSegFontIndex(seg) == -1)
    {
	_XmFontListSearch (fontlist, _XmSegCharset(seg), 
		&(_XmSegFontIndex(seg)), &font_struct);
    }

    if (_XmSegFontIndex(seg) != -1)
    {
    	if (two_byte_font (font_struct))
	{
            if (_XmSegCharCount(seg) < 2)
                _XmSegPixelWidth(seg) = 1;
            else
                _XmSegPixelWidth(seg) = 
		  XTextWidth16 (font_struct, (XChar2b *)_XmSegText(seg),
				Half (_XmSegCharCount(seg)));
	}
	else
	{
            if (_XmSegCharCount(seg) < 1)
                _XmSegPixelWidth(seg) = 1;
            else
     	        _XmSegPixelWidth(seg) = XTextWidth (font_struct, 
				_XmSegText(seg), _XmSegCharCount(seg));
	}
    }
}


static void
_clear_segment (fontlist, seg)
    XmFontList fontlist;
    _XmStringSegment seg;
{
    _XmSegFontIndex(seg)  = -1;
    _XmSegPixelWidth(seg)  = 0;
}

static void
_clear_opt (fontlist, opt)
    XmFontList fontlist;
    _XmStringOpt opt;
{
    _XmOptFontIndex(opt)   = -1;
    _XmOptPixelWidth(opt)  = 0;
}

/* These are the os-specific environment variables checked for a language
** specification.
*/
#define env_variable "LANG"


struct __Xmlocale {
    char   *charset;
    int    charsetlen;
    Boolean inited;
};

static struct __Xmlocale locale;

     
static void _parse_locale(str, index, len)
    char *str;
    int  *index;
    int  *len;
{


    char     *temp;
    int      start;
    int      end;


    /*
     *  Set the return variables to zero.  If we find what we're looking
     *  for, we reset them.
     */

    *index = 0;
    *len = 0;

    /*
     *  The format of the locale string is:
     *          language[_territory[.codeset]]
     */

    temp = str;
    start = 0;
    for (end = 0; (temp[end] != '.') && (temp[end] != 0); end++)
      ;

    if (temp[end] == '.')
    {
        start = end + 1;
        *index = start;
        for (end = start; temp[end] != 0; end++)
            ;
        *len = end - start;
    }
}


 /* This function returns current default charset being used.  This is */
 /* determined from teh value of the $LANG environment variable or */
 /* XmFALLBACK_CHARSET.  This version converts the charset to the */
 /* canonical form (set _XmCharsaetCanonicalize) before caching it. */
char *_XmStringGetCurrentCharset()
{
    char *str;
    char *ptr;
    char *charset;
    char *tmpstr = NULL;
    int  chlen;
    int  index;
    int  len;
   
    if (!locale.inited)
    {
        locale.charset = NULL;
        locale.charsetlen = 0;
 
        str = (char *)getenv(env_variable);

        if (str)
        {
           _parse_locale(str, &index, &chlen);
           if (chlen > 0)
           {
               ptr = &str[index];

	       if (!strcmp(ptr, "ASCII"))
		 {
		   len = strlen(XmSTRING_ISO8859_1);
		   ptr = XmSTRING_ISO8859_1;
		 }
	       else if ((chlen == 5) &&
			isdigit(ptr[0]) &&
			isdigit(ptr[1]) &&
			isdigit(ptr[2]) &&
			isdigit(ptr[3]) &&
			isdigit(ptr[4]))
		 {
		   /* "ISO####-#" */
		   tmpstr = XtMalloc(3 + 4 + 1 + 1 + 1);
		   sprintf(tmpstr, "ISO%s", ptr);
		   tmpstr[7] = '-';
		   tmpstr[8] = ptr[4];
		   tmpstr[9] = '\0';
		   
		   len = 9;
		   ptr = tmpstr;
		 }
	       else
		 {
		   len = chlen;
		 }
	       
           }
           else {
               len = strlen(XmFALLBACK_CHARSET);
               ptr = XmFALLBACK_CHARSET;
           }
        }
        else {
           len = strlen(XmFALLBACK_CHARSET);
           ptr = XmFALLBACK_CHARSET;
        }
        locale.charset = (char *) XtMalloc(len + 1);
        strncpy(locale.charset, ptr, len);
        locale.charset[len] = '\0';
        locale.charsetlen = len;
        locale.inited = TRUE;
    }

    charset = (char *) XtMalloc(locale.charsetlen + 1);
    strncpy(charset, locale.charset, locale.charsetlen);
    charset[locale.charsetlen] = '\0';

    if (tmpstr) XtFree(tmpstr);
    
    return (charset);
}

 /* This function takes a charset string as it might be found in $LANG */
 /* and converts it to the form being used within Xm. */
extern char *_XmCharsetCanonicalize(charset) 
     String charset;
{
  String	new;
  int		len;
  
  /* ASCII -> ISO8859-1 */
  if (!strcmp(charset, "ASCII"))
    {
      len = strlen(XmSTRING_ISO8859_1);
      
      new = XtMalloc(len + 1);
      strncpy(new, XmSTRING_ISO8859_1, len);
      new[len] = '\0';
    }
  else if (_isISO(charset))
    {
      /* "ISO####-#" */
      new = XtMalloc(3 + 4 + 1 + 1 + 1);
      sprintf(new, "ISO%s", charset);
      new[7] = '-';
      new[8] = charset[4];
      new[9] = '\0';
    }
  else
    /* Anything else is copied but not modified. */
    {
      len = strlen(charset);
      
      new = XtMalloc(len + 1);
      strncpy(new, charset, len);
      new[len] = '\0';
    }
  return (new);
}

/* Quick check for possible numerical only designation of ISO charset. */
static Boolean _isISO(charset)
     String charset;
{
  register int	i;
  
  if (strlen(charset) == 5) 
    {
      for (i = 0; i < 5; i++) 
	{
	  if (!isdigit(charset[i])) return (False);
	}
      return (True);
    }
  else return (False);
}


/*
 * update the font indexs and the dimensional information in the internal 
 * TCS, used when the font changes
 */

void 
_XmStringUpdate (fontlist, string)
    XmFontList fontlist;
    _XmString string;
{
    int i, j;

    /* optimized; clear and leave */

    if (_XmStrLineCnt(string) == 0)
    {
        _clear_opt(fontlist, (_XmStringOpt) string);
        _update_opt(fontlist, (_XmStringOpt) string);
        return;
    }

    /* non-optimized; grind through it */

    else {
        _XmStringLine line = _XmStrLineLine(string);
        for (i=0; i<_XmStrLineCnt(string); i++)
        {
            _XmStringSegment seg = _XmStrLineSegment(&line[i]);
            int        segcount = _XmStrLineSegCount(&line[i]);

	    for (j = 0; j < segcount; j++)
	    {
	        _clear_segment (fontlist, &seg[j]);
	        _update_segment (fontlist, &seg[j]);
	    }
        }
    }
}




/*
 * duplicate an internal string
 */

_XmString
_XmStringCopy (string)
    _XmString string;
{

    _XmString new_string;

    if (_XmStrLineCnt(string) == 0)
    {
       _XmStringOpt o_string = (_XmStringOpt) string;
       _XmStringOpt n_o_string = 
              (_XmStringOpt) XtMalloc(sizeof(_XmStringOptRec) + 
                            _XmOptCharCount(o_string));
       _XmOptLineCount(n_o_string) = _XmOptLineCount(o_string);
       _XmOptCharset(n_o_string) = _XmOptCharset(o_string);
       _XmOptFontIndex(n_o_string) = _XmOptFontIndex(o_string);
       _XmOptCharCount(n_o_string) = _XmOptCharCount(o_string);
       _XmOptPixelWidth(n_o_string) = _XmOptPixelWidth(o_string);
       _XmOptDirection(n_o_string) = _XmOptDirection(o_string);
       bcopy(_XmOptText(o_string), _XmOptText(n_o_string), 
                          _XmOptCharCount(o_string));
       new_string = (_XmString) n_o_string;        
                
    }
    else
    {
        int i, j;
        _XmString n_string = (_XmString) XtMalloc (sizeof (_XmStringRec));

        _XmStrLineCnt(n_string) = _XmStrLineCnt(string);
	_XmStrLineLine(n_string) = (_XmStringLine) XtMalloc (
			sizeof (_XmStringLineRec) * _XmStrLineCnt(string));

        for (i=0; i<_XmStrLineCnt(string); i++)
        {
	    _XmStringLine line   = &(_XmStrLineLine(string)[i]);
	    _XmStringLine n_line = &(_XmStrLineLine(n_string)[i]);

	    _XmStrLineSegCount(n_line) = _XmStrLineSegCount(line);

	    if (_XmStrLineSegCount(line) > 0)
	    {
	        _XmStrLineSegment(n_line) = (_XmStringSegment) XtMalloc (
			  sizeof (_XmStringSegmentRec) * 
                          _XmStrLineSegCount(line));

	        for (j=0; j<_XmStrLineSegCount(line); j++)
	        {
	    	    _XmStringSegment seg   = &(_XmStrLineSegment(line)[j]);
	    	    _XmStringSegment n_seg = &(_XmStrLineSegment(n_line)[j]);

	    	    _XmSegCharset(n_seg)      = _XmSegCharset(seg);
	    	    _XmSegFontIndex(n_seg)   = -1;
	    	    _XmSegCharCount(n_seg)   = _XmSegCharCount(seg);
	    	    _XmSegText(n_seg)         = (char *) XtMalloc (_XmSegCharCount(seg));
		    bcopy (       _XmSegText(seg), 
                                  _XmSegText(n_seg), 
                                  _XmSegCharCount(seg));
	    	    _XmSegDirection(n_seg)    = _XmSegDirection(seg);
	    	    _XmSegPixelWidth(n_seg)  = 0;
	        }
	    }
	    else
	        _XmStrLineSegment(n_line) = (_XmStringSegment) NULL;
        }
        new_string = (_XmString) n_string;
    }
    return(new_string);
}



/*
 * check these two internals
 */

Boolean
_XmStringByteCompare (a, b)
    _XmString a, b;
{

    if (_XmStrLineCnt(a) != _XmStrLineCnt(b)) 
        return (FALSE);


    if (_XmStrLineCnt(a) == 0)
    {
       _XmStringOpt a1 = (_XmStringOpt) a;
       _XmStringOpt b1 = (_XmStringOpt) b;

       if (_XmOptCharset(a1) != _XmOptCharset(b1))
           return (FALSE);
       if (_XmOptCharCount(a1) != _XmOptCharCount(b1))
           return (FALSE);
       if (_XmOptDirection(a1) != _XmOptDirection(b1))
           return (FALSE);
       if (strncmp(_XmOptText(a1), _XmOptText(b1), _XmOptCharCount(a1)) != 0)
           return (FALSE);

    }
    else {
        int i, j;
        _XmStringLine line_a = _XmStrLineLine(a);
        _XmStringLine line_b = _XmStrLineLine(b);

        for (i=0; i<_XmStrLineCnt(a); i++)
        {
	    if (_XmStrLineSegCount(&line_a[i]) != 
                _XmStrLineSegCount(&line_b[i])) 
	        return (FALSE);

	    for (j=0; j<_XmStrLineSegCount(&line_a[i]); j++)
	    {
	        _XmStringSegment 
                    a_seg = &(_XmStrLineSegment(&line_a[i])[j]),
                    b_seg = &(_XmStrLineSegment(&line_b[i])[j]);

	        if (_XmSegCharset(a_seg) != _XmSegCharset(b_seg))
                     return (FALSE);
	        if (_XmSegCharCount(a_seg)   != _XmSegCharCount(b_seg))
                     return (FALSE);
	        if (_XmSegDirection(a_seg)    != _XmSegDirection(b_seg))
                     return (FALSE);
	        if (strncmp (_XmSegText(a_seg), _XmSegText(b_seg), 
                             _XmSegCharCount(a_seg)) != 0)
		    return (FALSE);
	    }
        }
    }
    return (TRUE);
}



Boolean _XmStringHasSubstring(string, substring)
   _XmString     string;
   _XmString     substring;
{
    _XmStringContext  context;
    XmStringCharSet   charset;
    XmStringDirection direction;
    char              *text;
    char              *subtext;
    short             char_count;
    short             subchar_count;
    Boolean           separator;
    Boolean           found;
    int               i, j, max;

    if ((string == NULL) || (substring == NULL) || (_XmStringEmpty(substring)))
        return (FALSE);

    /*
     * The substring must be a one line/one segment string.
     */
  
    if (_XmStrLineCnt(substring) > 0)
        return (FALSE);


    /*
     * Get the text out of the substring.
     */

    if (found = _XmStringInitContext(&context, substring))
    {
        found = _XmStringGetNextSegment(context, &charset, &direction, 
                    &subtext, &subchar_count, &separator);
        _XmStringFreeContext(context);
    }

    if (!found)
       return (FALSE);

    if (_XmStringInitContext(&context, string))
    {
        while (_XmStringGetNextSegment(context, &charset, &direction, &text,
                    &char_count, &separator))
        {
            if (char_count >= subchar_count)
            {
                max = char_count - subchar_count;
                for (i = 0; i <= max; i++)
	        {
	            found = FALSE;

	            for (j = 0; j < subchar_count; j++)
	            {
		        if (text[i+j] != subtext[j]) 
		        {
		            found = TRUE;
		            break;
		        }
	            }
	            if ( ! found)
                    {
                        _XmStringFreeContext(context);
		        return(TRUE);
                    }
                }
            }
        }
        _XmStringFreeContext(context);
    }
    return (FALSE);
}



/*
 * build the external TCS given an internal.

 * this is pretty crude yet
 * could be made a lot faster by making a pass to figure out how big it 
 * needs to be and building the external in-place rather than all the
 * concatenations...
 */

XmString
_XmStringCreateExternal (fontlist, cs)
    XmFontList fontlist;
    _XmString 	cs;					/* internal TCS */
{
    int i, j;
    XmString a, b, c, d, ext;
    char *x;


    if (!cs) return((XmString) NULL);

    ext = NULL;

    if (_XmStrLineCnt(cs) == 0)
    {
        _XmStringOpt str = (_XmStringOpt) cs;

	a = XmStringDirectionCreate (_XmOptDirection(str));

	x = XtMalloc (_XmOptCharCount(str) + 1);
	bcopy (_XmOptText(str), x, _XmOptCharCount(str));
	x[_XmOptCharCount(str)] = '\0';

	b = XmStringCreate (x, _XmOptCharset(str));

	c = XmStringConcat (a, b);

	d = ext;
	ext = XmStringConcat (d, c);

	XtFree (a); 
	XtFree (b);
	XtFree (c);
	XtFree (d);
        XtFree (x);
    }
    else {

        _XmStringLine  line = _XmStrLineLine(cs);

        for (i=0; i<_XmStrLineCnt(cs); i++)
        {

            _XmStringSegment seg = _XmStrLineSegment(&line[i]);
            int        segcount = _XmStrLineSegCount(&line[i]);

	    for (j = 0; j < segcount; j++)
	    {
	        if (_XmSegCharCount(&seg[j]) > 0)
	        {
		    a = XmStringDirectionCreate (_XmSegDirection(&seg[j]));

		    x = XtMalloc (_XmSegCharCount(&seg[j]) + 1);
		    bcopy (_XmSegText(&seg[j]), x, _XmSegCharCount(&seg[j]));
		    x[_XmSegCharCount(&seg[j])] = '\0';

		    b = XmStringCreate (x, _XmSegCharset(&seg[j]));

		    c = XmStringConcat (a, b);

		    d = ext;
		    ext = XmStringConcat (d, c);

		    XtFree (a); 
		    XtFree (b);
		    XtFree (c);
		    XtFree (d);
                    XtFree (x);
	        }
	    }

            if (i < (_XmStrLineCnt(cs) - 1))
            {
                a = XmStringSeparatorCreate ();
	        b = ext;
	        ext = XmStringConcat (b, a);
                XtFree (a); 
	        XtFree (b);
            }
        }
    }
    return (ext);
}



Dimension
_XmStringBaseline (fontlist, string)
    XmFontList fontlist;
    _XmString string;
{
    if (_XmStrLineCnt(string) > 0)
	return (_XmStringLineAscender (fontlist, 
                                       &(_XmStrLineLine(string)[0])));
    else
	return (_XmStringOptLineAscender(fontlist, (_XmStringOpt)string));
}

Dimension
XmStringBaseline (fontlist, string)
    XmFontList fontlist;
    XmString string;
{
    _XmString s;
    Dimension bl;

    if (!string || !fontlist) return (0);
    s = _XmStringCreate(string);
    bl = _XmStringBaseline (fontlist, s);
    _XmStringFree(s);
    return(bl);
}


int
_XmStringLineCount (string)
    _XmString string;
{
    return (_XmStrLineCnt(string));
}


/*
 * external TCS routines which just do on-the-fly conversion to
 * internal and then call the internal ones
 */



/*
 * find width of widest line in external TCS
 */

Dimension
XmStringWidth (fontlist, string)
    XmFontList fontlist;
    XmString string;
{
    _XmString a;
    Dimension width;

    if (!string || !fontlist) return (0);

    a = _XmStringCreate (string);
    width = _XmStringWidth (fontlist, a);
    _XmStringFree (a);

    return (width);
}

/*
 * find total height of external TCS
 */

Dimension
XmStringHeight (fontlist, string)
    XmFontList fontlist;
    XmString string;
{
    _XmString a;
    Dimension height;

    if (!string || !fontlist) return (0);

    a = _XmStringCreate (string);
    height = _XmStringHeight (fontlist, a);

    _XmStringFree (a);

    return (height);
}

/*
 * find the rectangle which will enclose the text 
 */

void
XmStringExtent (fontlist, string, width, height)
    XmFontList fontlist;
    XmString string;
    Dimension *width, *height;
{
    _XmString a;
    if (!string || !fontlist)
    {
        *width = 0;
        *height = 0;
        return;
    }
    a = _XmStringCreate (string);    
    *width  = _XmStringWidth  (fontlist, a);
    *height = _XmStringHeight (fontlist, a);

    _XmStringFree (a);
}



/*
 * count the number of lines in an external TCS
 */

int
XmStringLineCount (string)
    XmString string;
{
    int i = 0;
    unsigned char *c = (unsigned char *) _read_header((unsigned char *) string);
    unsigned char *end = c + _read_string_length ((unsigned char *) string);

    while (c < end)
    {
	switch (*c)
	{
	    case XmSTRING_COMPONENT_SEPARATOR:
		i++;
		/* no break */

	    default:
		c += _read_component_length (c) + HEADER;
		break;
	}
    }

    return (i);
}



/*
 * drawing routine for external TCS
 */

void
#ifdef _NO_PROTO
XmStringDraw (d, w, fontlist, string, gc, x, y, width, align, lay_dir, clip)
    Display	*d;
    Window 	w;
    XmFontList fontlist;
    XmString	string;
    GC		gc;
    Position	x, y;
    Dimension	width;
    unsigned char	align;
    unsigned char	lay_dir;
    XRectangle	*clip;
#else /* _NO_PROTO */
XmStringDraw (Display *d, Window w, XmFontList fontlist, XmString string, 
	      GC gc, 
#if NeedWidePrototypes
int x , int y , int width , unsigned int align , unsigned int lay_dir , 
#else
Position x, Position y, Dimension width, unsigned char align, unsigned char lay_dir, 
#endif 
XRectangle *clip)
#endif /* _NO_PROTO */
{
  if (string) 
    {
      
      _XmString internal = _XmStringCreate (string);

      _draw (d, w, fontlist, internal, gc, x, y, width, 
	     align, lay_dir, clip, FALSE, NULL);

      _XmStringFree (internal);
    }
}


void
#ifdef _NO_PROTO
XmStringDrawImage (d, w, fontlist, string, gc, x, y, 
	width, align, lay_dir, clip)
    Display	*d;
    Window 	w;
    XmFontList fontlist;
    XmString	string;
    GC		gc;
    Position	x, y;
    Dimension	width;
    unsigned char	align;
    unsigned char	lay_dir;
    XRectangle	*clip;
#else /* _NO_PROTO */
XmStringDrawImage (Display *d, Window w, XmFontList fontlist, XmString string,GC gc, 
#if NeedWidePrototypes
int x , int y , int width , unsigned int align , unsigned int lay_dir , 
#else
Position x, Position y, Dimension width, unsigned char align, unsigned char lay_dir, 
#endif 
XRectangle *clip)
#endif /* _NO_PROTO */
{
  if (string)
    {
      _XmString internal = _XmStringCreate (string);

      _draw (d, w, fontlist, internal, gc, x, y, width, 
	     align, lay_dir, clip, TRUE, NULL);

      _XmStringFree (internal);
    }
}

void
#ifdef _NO_PROTO
XmStringDrawUnderline (d, w, fntlst, str, gc, x, y, 
	width, align, lay_dir, clip, under)
    Display	*d;
    Window 	w;
    XmFontList fntlst;
    XmString	str;
    GC		gc;
    Position	x, y;
    Dimension	width;
    unsigned char	align;
    unsigned char	lay_dir;
    XRectangle	*clip;
    XmString	under;
#else /* _NO_PROTO */
XmStringDrawUnderline (Display *d, Window w, XmFontList fntlst, XmString str,GC gc, 
#if NeedWidePrototypes
int x , int y , int width , unsigned int align , unsigned int lay_dir , 
#else
Position x, Position y, Dimension width, unsigned char align, unsigned char lay_dir, 
#endif 
XRectangle *clip, XmString under)
#endif /* _NO_PROTO */
{
  if (str)
    {
      _XmString internal = _XmStringCreate (str),
                int_under = _XmStringCreate (under);

      _draw (d, w, fntlst, internal, gc, x, y, width, 
	     align, lay_dir, clip, FALSE, int_under);

      _XmStringFree (internal);
      _XmStringFree (int_under);
    }
}


#ifdef DEBUG


void dump_fontlist (f)
    XmFontList f;
{
    int i = 0;

    for ( ; FontListFont(f) != NULL; f++, i++)
    {
   	printf ("fontlist[%3d]\n", i);
	printf ("\tfont = %d\n", FontListFont(f));
	printf ("\tcharset = <%s>\n", FontListCharset(f));
    }
}


void dump_fontlist_cache()
{
    FontlistEntry *cache;

    if (_fontlist_cache == NULL)
    {
        printf("fontlist cache is empty\n");
        return;
    }

    for (cache = _fontlist_cache; cache; cache = FontCacheNext(cache))
    {
        printf("cache pointer:   %x\n", cache);
        dump_fontlist(FontCacheFontList(cache));
        printf("refcount:      %d\n", FontCacheRefCount(cache));
        printf("next:          %x\n\n", FontCacheNext(cache));
    }
}


void dump_external (cs)
    XmString 	cs;					/* external TCS */
{
    unsigned char *c = (unsigned char *) cs;
    unsigned char *end = c + _read_string_length (c) + _read_header_length(c);
    int k;

    if (_is_compound(cs))
    {
	printf ("Compound string\n");
	printf ("overall length = %d\n", _read_string_length(c));
	c = _read_header(c);
    }
    else {
        printf ("Not a compound string\n");
        return;
    }


    while (c < end)
    {
	UShort length = _read_component_length (c);

	switch (*c)
	{
	    case XmSTRING_COMPONENT_CHARSET:
		printf ("\tCharacter set component\n");
		printf ("\tlength = %d\n", length);
	  	printf ("\tvalue  = <");
		for (k=0; k<length; k++) printf ("%c", *(c + HEADER + k));
		printf (">\n");
		c += length + HEADER;
		break;

	    case XmSTRING_COMPONENT_TEXT:
		printf ("\tText component\n");
		printf ("\tlength = %d\n", length);
	  	printf ("\tvalue  = <");
		for (k=0; k<length; k++) printf ("%c", *(c + HEADER + k));
		printf (">\n");
		c += length + HEADER;
		break;

	    case XmSTRING_COMPONENT_DIRECTION:		/* record dir */
		printf ("\tDirection component\n");
		printf ("\tlength = %d\n", length);
	  	printf ("\tvalue  = %d\n", *(c + HEADER));
		c += length + HEADER;
		break;

	    case XmSTRING_COMPONENT_SEPARATOR:		/* start new line */
		printf ("\tSeparator component\n");
		printf ("\tlength = %d\n", length);
		c += length + HEADER;
		break;

	    default:
		printf ("\tUnknown component\n");
		printf ("\tlength = %d\n", length);
	  	printf ("\tvalue  = <");
		for (k=0; k<length; k++)
			printf ("%3d ", (int) *(c + HEADER + k));
		printf ("\n");
		c += length + HEADER;
		break;
	}

	printf ("\n");

    }
}


void dump_internal (string)
    _XmString string;
{
    int i, j, k;

    printf ("string with %d lines\n", _XmStrLineCnt(string));

    if (_XmStrLineCnt(string) == 0)
    {
        _XmStringOpt str = (_XmStringOpt) string;
        printf ("\tOptimized string - single segment\n");
	printf ("\t\tchar count   = %4d\n", _XmOptCharCount(str));
	printf ("\t\tfont index   = %4d\n", _XmOptFontIndex(str));
	printf ("\t\ttext         = <");
	for (k=0; k<_XmOptCharCount(str); k++) printf ("%c", _XmOptText(str)[k]);
	  printf (">\n");
	printf ("\t\tdirection    = %4d\n", _XmOptDirection(str));
	printf ("\t\tpixel width  = %4d\n", _XmOptPixelWidth(str));
    }     

    else {

        _XmStringLine line = _XmStrLineLine(string);

        for (i = 0; i < _XmStrLineCnt(string); i++)
        {
	    _XmStringSegment seg = _XmStrLineSegment(&line[i]);

	    printf ("\tline [%2d] has %5d segments\n", i, 
                     _XmStrLineSegCount(&line[i]));

	    for (j = 0; j < _XmStrLineSegCount(&line[i]); j++)
	    {
	        printf ("\t\tsegment [%2d]\n", j); 
	        printf ("\t\t\tchar count   = %4d\n", _XmSegCharCount(&seg[j]));
	        printf ("\t\t\tfont index   = %4d\n", _XmSegFontIndex(&seg[j]));
	        printf ("\t\t\ttext         = <");
	        for (k=0; k<_XmSegCharCount(&seg[j]); k++) 
                    printf ("%c", _XmSegText(&seg[j])[k]);
	        printf (">\n");
	        printf ("\t\t\tdirection    = %4d\n", _XmSegDirection(&seg[j]));
	        printf ("\t\t\tpixel width  = %4d\n", _XmSegPixelWidth(&seg[j]));
	    }
        }
    }    
}
#endif
