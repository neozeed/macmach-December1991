#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)UilSrcDef.h	3.1 90/03/02";
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
**      This include file defines the interface to the UIL source
**	management procedures in the UIL compiler.
**
**--
**/

#ifndef UilSrcDef_h
#define UilSrcDef_h


/*
**  Interface to src_open_source
*/

#define src_k_open_normal   1	    /* open succeeded */
#define src_k_open_error    0	    /* open was unsuccessful */

/*
** Close return statuses
*/

#define src_k_close_normal	1	/* close succeeded */
#define src_k_close_error	0	/* close was unsuccessful */

/*
**  Interface to src_get_source_line
*/

#define src_k_read_truncated	3   /* record truncated */
#define src_k_read_error    	2   /* error during read */
#define src_k_read_normal   	1   /* record read normally */
#define src_k_end_source    	0   /* end of source */

/*
**  Source records describe the lines of the source program.  The are
**  used to retrieve the source file for the listing and diagnostics.
**  Diagnostics randomly access these data structures by maintaining
**  the address of the corresponding source record in tokens, the parse
**  stack and symbol table entries.  The listing file walks the source
**  records via the linked list provide by ar_next_source_record.
*/






#define _src_null_access_key( _key ) (_key.l_key == EOF)
#define src_k_key_length 4

typedef struct
{
    unsigned long	l_key;
} z_key;

typedef struct  _src_message_item_type
{
    struct _src_message_item_type   *az_next_message;
    status			    l_message_number;
    unsigned char		    b_source_pos;
    char			    c_text[ 1 ];
} src_message_item_type;

#define src_message_item_type_size \
    (sizeof(src_message_item_type) -\
     sizeof( struct _src_message_item_type.c_text))

typedef struct  _src_machine_code_type
{
    struct _src_machine_code_type   *az_next_machine_code;
    unsigned short int		    w_offset;
    unsigned short int		    w_code_len;
    char			    c_data[ 1 ];
} src_machine_code_type;

#define src_machine_code_type_size \
    (sizeof(src_machine_code_type) -\
     sizeof( struct _src_machine_code_type.c_text))

/*
**  Mask for bits in b_flags of the source record
*/

#define src_m_form_feed			(1<<0)
#define src_m_unprintable_chars		(1<<1)

typedef struct  _src_source_record_type
{
    struct _src_source_record_type  *az_next_source_record;
    src_message_item_type	    *az_message_list;    
    unsigned short		    w_line_number;
    unsigned char		    b_file_number;
    unsigned char		    b_flags;
    z_key			    z_access_key;
    src_machine_code_type	    *az_machine_code_list;    
    unsigned short		    w_machine_code_cnt;
} src_source_record_type;

/*
**  Source buffers describe the lines of the source program that are
**  currently being SCAN.  Include files and macros a can cause a program
**  to have more than 1 source buffer.  These buffers are managed as a
**  stack, implemented via a linked list.
*/

typedef struct	_src_source_buffer_type
{
    struct _src_source_buffer_type  *az_prior_source_buffer;
    unsigned short		    w_current_line_number;
    unsigned short		    w_current_position;
    char			    b_file_number;
    char			    c_text[ src_k_max_source_line_length+1];
} src_source_buffer_type;

#endif /* UilSrcDef_h */
/* DON'T ADD STUFF AFTER THIS #endif */
