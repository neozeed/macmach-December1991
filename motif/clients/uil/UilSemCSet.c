#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)UilSemCSet.c	3.2 90/03/28";
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
**      This file contains and routines related to the semantics of
**	character sets.
**	semantic validation.
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
**      This function returns a pointer to a character set name given
**	its sym_k_..._charset code and possibly a userdefined charset
**	value entry.
**
**  FORMAL PARAMETERS:
**
**	l_charset	charset of the string (token value)
**	az_charset_entry   charset of the string (symbol table value entry)
**
**  IMPLICIT INPUTS:
**
**      charset data tables
**
**  IMPLICIT OUTPUTS:
**
**      none
**
**  FUNCTION VALUE:
**
**      pointer to name (must NOT be freed)
**
**  SIDE EFFECTS:
**
**      error messages may be issued for objects that are still undefined
**	or of the wrong type
**
**--
**/

char *sem_charset_name (l_charset, az_charset_entry)
    int				l_charset;
    sym_value_entry_type	*az_charset_entry;

{

int		charset;		/* mapped character set */

charset = sem_map_subclass_to_charset (l_charset);
switch ( charset )
    {
    case sym_k_userdefined_charset:
        /*
	 ** If the charset is user-defined, then fetch info from the symbol 
	 ** table entry for it.						   
	 */
	_assert (az_charset_entry!=NULL, "null userdefined charset entry");
	return az_charset_entry->value.c_value;
    default:
	return charset_xmstring_names_table[charset];
    }

}



/*
**++
**  FUNCTIONAL DESCRIPTION:
**
**      This function returns information about the character set specified
**	(writing direction and sixteen_bit properties).
**
**  FORMAL PARAMETERS:
**
**	l_charset	charset of the string (token value)
**	az_charset_entry   charset of the string (symbol table value entry)
**	direction	string writing direction
**	sixteen_bit	Boolean return value
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
**      none
**
**  SIDE EFFECTS:
**
**      none
**
**--
**/

void sem_charset_info
    (l_charset, az_charset_entry, write_direction, parse_direction,  sixteen_bit)

int			l_charset;
sym_value_entry_type	*az_charset_entry;
int			*write_direction;
int			*parse_direction;
int			*sixteen_bit;

{

int		charset;	/* mapped character set */
    
charset = sem_map_subclass_to_charset (l_charset);
switch (l_charset) 
    {
    /*
     ** If the charset is user-defined, then fetch info from the symbol 
     ** table entry for it.						   
     */
    case sym_k_userdefined_charset:
        {
	*write_direction = az_charset_entry->b_direction;
	*parse_direction = az_charset_entry->b_direction;
	*sixteen_bit = (az_charset_entry->b_aux_flags &
			sym_m_sixteen_bit) != 0;
	break;
	}
    default:
	{
	*write_direction = charset_writing_direction_table[charset];
	*parse_direction = charset_parsing_direction_table[charset];
	if ( charset_character_size_table[charset] != sym_k_onebyte_charsize )
	    *sixteen_bit = TRUE;
	else
	    *sixteen_bit = FALSE;
	break;
	}
	}
}



/*
**++
**  FUNCTIONAL DESCRIPTION:
**
**      This function returns the charset id corresponding to the
**	charset specified in a $LANG variable. User-defined character
**	sets are not recognized - it must be one available in the
**	the compiler tables.
**
**	The name match is case-insensitive.
**
**  FORMAL PARAMETERS:
**
**	lang_charset	string naming a character set
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
**      0	no match - character set not found
**	>0	character set code from sym_k_..._charset.
**		sym_k_userdefined_charset is never returned.
**		
**
**  SIDE EFFECTS:
**
**      none
**
**--
**/

int sem_charset_lang_name (lang_charset)
    char		*lang_charset;

{

char		uname[200];	/* upper-case character set name */
int		ndx;		/* loop index */


/*
 * Convert name to upper case, then search table (which is already in
 * upper case).
 */
strcpy (uname, lang_charset);
for ( ndx=0 ; ndx<strlen(uname) ; ndx++ )
    uname[ndx] = _upper (uname[ndx]);

for ( ndx=0 ; ndx<charset_lang_table_max ; ndx++ )
    if ( strcmp(uname,charset_lang_names_table[ndx]) == 0 )
	return (int)charset_lang_codes_table[ndx];
return 0;

}


