#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)UilSarDef.h	3.1 90/03/02";
#endif /* lint */
#endif /* REV_INFO */

/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990, OPEN SOFTWARE FOUNDATION, INC.
*  (c) Copyright 1989, 1990, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
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
*  (c) Copyright 1989, 1990, Open Software Foundation, Inc.  Unpublished - all
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
*  (c) Copyright 1989, 1990, Open Software Foundation, Inc.
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
**++
**  FACILITY:
**
**      User Interface Language Compiler (UIL)
**
**  ABSTRACT:
**
**      This include file defines the interface to the UIL parser.
**	UIL uses YACC as its parsing tool.
**
**--
**/

#ifndef UilSarDef_h
#define UilSarDef_h



/*
**  Format of a value on YACC value stack.  This is also the form of a
**  token created by the lexical analyzer.
*/

#define	    sar_k_null_frame	0	/* tag for an epsilon production */
#define	    sar_k_token_frame	1	/* tag for a token frame */
#define	    sar_k_value_frame	2	/* tag for a value frame */
#define	    sar_k_module_frame	3	/* tag for module frame */
#define     sar_k_object_frame	4	/* tag for object frame */
#define     sar_k_root_frame    5	/* tag for root frame */

typedef struct
{
    src_source_record_type  *az_source_record;	/* actual record where token exists */
    unsigned char	    b_source_pos;	/* the character in az_source_record
						   where this token begins */
    unsigned char	    b_source_end;	/* the character in az_source_record
						   where this token ends */
    unsigned char	    b_tag;		/* tag of stack frame */
    unsigned char	    b_type;		/* for tokens - token number
						   for value - the data type */
    unsigned short	    b_flags;		/* used by value */
    unsigned char	    b_direction;	/* used by value */
    unsigned char	    b_charset;		/* used by value */
    union
    {
	int		    l_integer;		/* integer value*/
	sym_entry_type	    *az_symbol_entry;	/* symbol entry */
	key_keytable_entry_type
			    *az_keyword_entry;	/* keyword entry */
    }	value;
} yystype;


/*
**  Macros for moving source information to and from parse stack frames
*/

#define    _sar_move_source_info( _target, _source )			\
	   {								\
		yystype	    *__target;					\
		yystype	    *__source;					\
									\
		__target = (_target);  __source = (_source);		\
		__target->az_source_record = __source->az_source_record;\
		__target->b_source_pos = __source->b_source_pos;	\
		__target->b_source_end = __source->b_source_end;	\
	   }

#define    _sar_move_source_info_2( _target, _source )		\
   {								\
	sym_entry_header_type	*__target;			\
	sym_entry_header_type	*__source;			\
								\
	__target = (_target);  __source = (_source);		\
								\
	__target->az_src_rec = __source->az_src_rec;		\
	__target->b_src_pos = __source->b_src_pos;		\
	__target->b_end_pos = __source->b_end_pos;		\
   }


#define _sar_save_source_info( _target, _src_beg, _src_end )	\
   {								\
	sym_entry_header_type	*__target;			\
	yystype	    		*__src_beg;			\
	yystype	    		*__src_end;			\
								\
	__target = (_target);  __src_beg = (_src_beg);		\
	__src_end = (_src_end);					\
								\
	__target->az_src_rec	= __src_end->az_source_record;	\
	__target->b_src_pos	= __src_end->b_source_pos;	\
	__target->b_end_pos	= __src_end->b_source_end;	\
   }

#define _sar_save_source_pos( _target, _src )			\
   {								\
	sym_entry_header_type	*__target;			\
	yystype	    		*__src;				\
								\
	__target = (_target);	__src = (_src);			\
								\
	__target->az_src_rec	= __src->az_source_record;	\
	__target->b_src_pos	= __src->b_source_pos;		\
	__target->b_end_pos	= __src->b_source_end;		\
   }

#define    _sar_source_position( _source )			\
		_source->az_source_record, 			\
		_source->b_source_pos

#define    _sar_source_pos2( _source )				\
		_source->header.az_src_rec, 			\
		_source->header.b_src_pos



#endif /* UilSarDef_h */
/* DON'T ADD STUFF AFTER THIS #endif */
