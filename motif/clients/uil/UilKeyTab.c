#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)UilKeyTab.c	3.2 90/03/28";
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
**      This module contains the keyword table used by the lexical analyzer
**	to look up the keywords in the UIL.
**
**--
**/


/*
**
**  INCLUDE FILES
**
**/

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

/*    Keyword table pointer.    */

static key_keytable_entry_type * key_keytable_ptr;


/*
**++
**  FUNCTIONAL DESCRIPTION:
**
**	This routine searches for a symbol in the compiler's keyword table.
**	There are two arguments to the routine, the length of the symbol and
**      the address of the start of the symbol.  The routine returns the
**	address of the keyword entry found, or a NULL pointer if the
**	symbol is not found in the table.
**
**	The search for the symbol is case sensitive depending upon the
**	keytable binding that was established by the key_initialize routine.
**
**	The require file UilKeyTab.h defines and initializes the keyword
**	tables.  It is built automatically from other files, thus it should
**	not be hand editted.
**
**  FORMAL PARAMETERS:
**
**	symbol_length.rl.v : 	length of symbol to look up
**	symbol_ptr.ra.v : 	address of symbol to look up
**
**  IMPLICIT INPUTS:
**
**      key_keytable_ptr		: current keyword table
**
**  IMPLICIT OUTPUTS:
**
**      none
**
**  FUNCTION VALUE:
**
**	NULL		: if the symbol is not in the keyword table
**	otherwise	: the address of the keyword table entry for
**			  the specified symbol.
**
** SIDE EFFECTS:
**
**	none
**
**--
**/
key_keytable_entry_type *
	key_find_keyword (symbol_length, symbol_ptr)

unsigned int	symbol_length;
char		* symbol_ptr;

{
    
    int
	lower_limit,
	upper_limit;
    
/*    Check the arguments.    */

    if (symbol_length > key_k_keyword_max_length)
	return NULL;

/*    Initialize region to search.    */
    
    lower_limit = 0;
    upper_limit = key_k_keyword_count-1;
    
/*    Perform binary search on keyword index.    */
    
    do {
	int		mid_point, result;

	key_keytable_entry_type * keyword_entry_ptr;

	mid_point = (lower_limit + upper_limit) >> 1;	/* divide by 2 */

	keyword_entry_ptr = & key_keytable_ptr [mid_point];

	result = strcmp (symbol_ptr, keyword_entry_ptr -> at_name);

	if (result == 0) {
	    return keyword_entry_ptr;		/*    Found keyword.    */
	} else if (result < 0) {
	    upper_limit = mid_point - 1;	/*    Search lower half.    */
	} else {
	    lower_limit = mid_point + 1;	/*    Search upper half.    */
	}

    } while (lower_limit <= upper_limit);

/*    If we fall out of the bottom of the loop, symbol was not found.    */

    return NULL;

}

/*
**++
**  FUNCTIONAL DESCRIPTION:
**
**	This routine initializes the keyword lookup facility.  It can be
**	called multiple times during a single compilation.  It must be called
**	at least once before the keyword table is accessed.
**
**  FORMAL PARAMETERS:
**
**	none
**
**  IMPLICIT INPUTS:
**
**      uil_v_case_sensitive	: case sensitive switch, determines which
**				: keyword table to use.
**
**  IMPLICIT OUTPUTS:
**
**      key_keytable_ptr	: pointer to the keyword table to
**				  use for keyword lookups.
**
**  FUNCTION VALUE:
**
**	none
**
** SIDE EFFECTS:
**
**	none
**
**--
**/
void
	key_initialize ()

{

/*    Use the correct keyword table based on the global case
      sensitivity.   */

    if (uil_v_case_sensitive) {
	key_keytable_ptr = key_table;
    } else {
	key_keytable_ptr = key_table_case_ins;
    }

}    

