#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)UilSarInc.c	3.3 90/06/12";
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
**      This module contain the routines for processing the INCLUDE construct.
**
**--
**/


/*
**
**  INCLUDE FILES
**
**/

#include <Xm/Xm.h>
#include "UilDef.h"


/*
**
**  DEFINE and MACRO DEFINITIONS
**
**/


/*
**
**  EXTERNAL VARIABLE DECLARATIONS
**
**/



/*
**
**  GLOBAL VARIABLE DECLARATIONS
**
**/



/*
**
**  OWN VARIABLE DECLARATIONS
**
**/


/*
**++
**  FUNCTIONAL DESCRIPTION:
**
**      This routine handles the include directive.
**
**  FORMAL PARAMETERS:
**
**      file_frame	parser stack frame that contains the name of the
**			file to be included.
**
**  IMPLICIT INPUTS:
**
**      none
**
**  IMPLICIT OUTPUTS:
**
**      none
**
**  FUNCTION VALUE:
**
**      void
**
**  SIDE EFFECTS:
**
**      none
**
**--
**/

void	sar_include_file ( file_frame, include_frame, semi_frame )

yystype	    * file_frame;
yystype	    * include_frame;
yystype	    * semi_frame;

{
    sym_value_entry_type	* value_entry;
    sym_include_file_entry_type	* include_entry;
    sym_section_entry_type	* section_entry;
    sym_section_entry_type	* section_tail_entry;
    char			* buffer;
    unsigned char		  tmp1;
    int				  i,j;

    value_entry =
	(sym_value_entry_type *) file_frame -> value . az_symbol_entry;

/*
 * If the direction is RtoL then reverse the include file.
 */
    if (value_entry->b_direction == XmSTRING_DIRECTION_R_TO_L) 
	{
	/*
	**  Just reverse the bytes from the first to last
	*/
	for (i=0, j=value_entry->w_length-1;  
	     i < (value_entry->w_length>>1);  
	     i++,j--)
	    {
	    tmp1 = value_entry->value.c_value[ i ];
	    value_entry->value.c_value[ i ] = 
		value_entry->value.c_value[ j ];
	    value_entry->value.c_value[ j ] = tmp1;
	    }
	}

    _assert (value_entry -> header . b_tag == sym_k_value_entry,
	     "missing value entry for include");

    /*
    ** Allocate an INCLUDE entry
    */

    include_entry = (sym_include_file_entry_type *) sem_allocate_node
			( sym_k_include_file_entry, sym_k_include_file_entry_size );

    /*
    ** Open the include file.  Set up a null-terminated name string.
    */

    buffer = (char *) _get_memory (value_entry -> w_length + 1);
    _move (buffer, value_entry -> value . c_value,
		   value_entry -> w_length);
    buffer [value_entry -> w_length] = NULL;

    src_open_file (buffer, include_entry->full_file_name);

    /*
    **  Allocate a section entry for this include "section".
    **  Store the source info in it
    */

    section_entry = (sym_section_entry_type *) sem_allocate_node
			( sym_k_section_entry, sym_k_section_entry_size );

    section_entry->header.b_type = sym_k_include_section;

    _sar_save_source_info ( &section_entry->header , include_frame , semi_frame );

    /*
    ** Link this section into the current section list.
    */

    section_entry->next = (sym_entry_type *) sym_az_current_section_entry;
    section_entry->prev_section = sym_az_current_section_entry->prev_section;
    sym_az_current_section_entry = section_entry;

    /*
    ** Link the include intry we created earlier onto the section list
    */

    section_entry->entries = (sym_entry_type *)include_entry;

    /*
    **  Save the file name
    */

    _move (include_entry->file_name, buffer, value_entry->w_length);
    include_entry->file_name [value_entry->w_length] = NULL;

    /*
    ** Since this is an INCLUDE we want included sections to hang off
    ** of it so we create a new place holder entry (tail) and change the
    ** "current" section list to it.
    */

    section_tail_entry = (sym_section_entry_type *) sem_allocate_node
			    ( sym_k_section_entry, sym_k_section_entry_size );

    section_tail_entry->header.b_type = sym_k_section_tail;

    include_entry->sections = section_tail_entry;

    section_tail_entry->prev_section = sym_az_current_section_entry;
    sym_az_current_section_entry = section_tail_entry;

    _free_memory (buffer);

}
