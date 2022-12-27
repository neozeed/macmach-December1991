#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)Mrmptrlist.c	3.9 91/01/10";
#endif /* lint */
#endif /* REV_INFO */

/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
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
 *++
 *  FACILITY:
 *
 *      UIL Resource Manager (URM):
 *
 *  ABSTRACT:
 *
 *	These routines manage a dynamic pointer list
 *
 *--
 */


/*
 *
 *  INCLUDE FILES
 *
 */


#include <Mrm/MrmAppl.h>
#include <Mrm/Mrm.h>


/*
 *
 *  TABLE OF CONTENTS
 *
 */


/*
 *
 *  DEFINE and MACRO DEFINITIONS
 *
 */



/*
 *
 *  EXTERNAL VARIABLE DECLARATIONS
 *
 */


/*
 *
 *  GLOBAL VARIABLE DECLARATIONS
 *
 */


/*
 *
 *  OWN VARIABLE DECLARATIONS
 *
 */



Cardinal UrmPlistInit 
#ifndef _NO_PROTO
    (int		size,
    URMPointerListPtr	*list_id_return)
#else
(size, list_id_return)
    int			size ;
    URMPointerListPtr	*list_id_return ;
#endif

/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	This routine acquires and initializes a new pointer list.
 *
 *  FORMAL PARAMETERS:
 *
 *	size		number of pointer slots to allocate in list
 *	list_id_return	to return pointer to new list structure
 *
 *  IMPLICIT INPUTS:
 *
 *  IMPLICIT OUTPUTS:
 *
 *  FUNCTION VALUE:
 *
 *	MrmSUCCESS	operation succeeded
 *	MrmFAILURE	allocation failure
 *
 *  SIDE EFFECTS:
 *
 *--
 */

{

/*
 *  External Functions
 */

/*
 *  Local variables
 */


/*
 * Allocate the list buffer and the slot vector buffer, and initialize
 */
*list_id_return = (URMPointerListPtr) XtMalloc (sizeof(URMPointerList)) ;
if ( *list_id_return == NULL )
    return Urm__UT_Error
        ("UrmPlistInit", "List allocation failed",
        NULL, NULL, MrmFAILURE) ;

(*list_id_return)->ptr_vec = (caddr_t *) XtMalloc (size*sizeof(caddr_t)) ;
if ( (*list_id_return)->ptr_vec == NULL )
    return Urm__UT_Error
        ("UrmPlistInit", "List vector allocation failed",
        NULL, NULL, MrmFAILURE) ;

(*list_id_return)->num_slots = size ;
(*list_id_return)->num_ptrs = 0 ;
return MrmSUCCESS ;

}



Cardinal UrmPlistResize 
#ifndef _NO_PROTO
    (URMPointerListPtr	list_id,
    int			size)
#else
(list_id, size)
    URMPointerListPtr	list_id ;
    int			size ;
#endif

/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	This routine reallocates the list vector in a pointer list in order
 *	to increase its size. The contents of the current list are copied
 *	into the new list. If the size parameter is smaller than the
 *	current buffer size, the request is ignored.
 *
 *  FORMAL PARAMETERS:
 *
 *	list_id		The pointer list to be resized
 *	size		The new number of pointer slots
 *
 *  IMPLICIT INPUTS:
 *
 *	MrmSUCCESS	operation succeeded
 *	MrmFAILURE	memory allocation failure
 *
 *  IMPLICIT OUTPUTS:
 *
 *  FUNCTION VALUE:
 *
 *  SIDE EFFECTS:
 *
 *--
 */

{

/*
 *  External Functions
 */

/*
 *  Local variables
 */
caddr_t			*newvec ;	/* new pointer slot vector */


/*
 * Allocate the new vector, and copy the current vector into it.
 */
newvec = (caddr_t *) XtMalloc (size*sizeof(caddr_t)) ;
if ( newvec == NULL )
    return Urm__UT_Error
        ("UrmPlistResize", "Vector re-allocation failed",
        NULL, NULL, MrmFAILURE) ;

if ( list_id->num_ptrs > 0 )
    UrmBCopy (list_id->ptr_vec, newvec, list_id->num_ptrs*sizeof(caddr_t)) ;

list_id->num_slots = size ;
list_id->ptr_vec = newvec ;
return MrmSUCCESS ;

}



Cardinal UrmPlistFree (list_id)
    URMPointerListPtr	list_id ;

/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	This routine frees the pointer vector and list structure.
 *
 *  FORMAL PARAMETERS:
 *
 *	list_id		The pointer list to be freed
 *
 *  IMPLICIT INPUTS:
 *
 *  IMPLICIT OUTPUTS:
 *
 *  FUNCTION VALUE:
 *
 *	MrmSUCCESS	operation succeeded
 *
 *  SIDE EFFECTS:
 *
 *--
 */

{

/*
 *  External Functions
 */

/*
 *  Local variables
 */


XtFree ((char*)list_id->ptr_vec) ;
XtFree ((char*)list_id) ;
return MrmSUCCESS ;

}



Cardinal UrmPlistFreeContents (list_id)
    URMPointerListPtr	list_id ;

/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	This routine frees each of the items pointed to by the active
 *	pointers in the pointer list. The items must have been allocated
 *	with XtMalloc.
 *
 *  FORMAL PARAMETERS:
 *
 *	list_id		The pointer list
 *
 *  IMPLICIT INPUTS:
 *
 *  IMPLICIT OUTPUTS:
 *
 *  FUNCTION VALUE:
 *
 *	MrmSUCCESS	operatoin succeeded
 *
 *  SIDE EFFECTS:
 *
 *--
 */

{

/*
 *  External Functions
 */

/*
 *  Local variables
 */
int			ndx ;		/* loop index */


for ( ndx=0 ; ndx<list_id->num_ptrs ; ndx++ )
    XtFree (list_id->ptr_vec[ndx]) ;
return MrmSUCCESS ;

}



Cardinal UrmPlistAppendPointer (list_id, ptr)
    URMPointerListPtr	list_id ;
    caddr_t		ptr ;

/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	This routine appends a pointer to the list. If no space remains,
 *	the list is resized to double its current size.
 *
 *  FORMAL PARAMETERS:
 *
 *	list_id		The pointer list
 *	ptr		The pointer to append
 *
 *  IMPLICIT INPUTS:
 *
 *  IMPLICIT OUTPUTS:
 *
 *	MrmSUCCESS	operation succeeded
 *	MrmFAILURE	allocation failure
 *
 *  FUNCTION VALUE:
 *
 *  SIDE EFFECTS:
 *
 *--
 */

{

/*
 *  External Functions
 */

/*
 *  Local variables
 */
Cardinal		result ;	/* function results */


if ( list_id->num_ptrs == list_id->num_slots )
    {
    result = UrmPlistResize (list_id, 2*list_id->num_slots) ;
    if ( result != MrmSUCCESS ) return result ;
    }

list_id->ptr_vec[list_id->num_ptrs] = ptr ;
list_id->num_ptrs++ ;
return MrmSUCCESS ;

}



Cardinal UrmPlistAppendString (list_id, stg)
    URMPointerListPtr		list_id ;
    String			stg ;

/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	This routine allocates space for a string, copies, and appends the
 *	pointer to the string in the pointer list. All the strings in the
 *	list may be freed with UrmPlistFreeContents.
 *
 *  FORMAL PARAMETERS:
 *
 *	list_id		The pointer list
 *	stg		The string to alocate, copy, and append
 *
 *  IMPLICIT INPUTS:
 *
 *  IMPLICIT OUTPUTS:
 *
 *  FUNCTION VALUE:
 *
 *	MrmSUCCESS	operation succeeded
 *	MrmFAILURE	allocation failure
 *
 *  SIDE EFFECTS:
 *
 *--
 */

{

/*
 *  External Functions
 */

/*
 *  Local variables
 */
Cardinal		result ;	/* function results */
String			newstg ;	/* allocated string */


newstg = XtMalloc (strlen(stg)+1) ;
if ( newstg == NULL )
    return Urm__UT_Error
        ("UrmPlistAppendString", "String allocation failed",
        NULL, NULL, MrmFAILURE) ;

strcpy (newstg, stg) ;
result = UrmPlistAppendPointer (list_id, newstg) ;
return result ;

}



MrmCount UrmPlistFindString (list_id, stg)
    URMPointerListPtr		list_id ;
    String			stg ;

/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	This routine searches a list (assumed to be a list of string
 *	pointers) for a case-sensitive match to a string. If found,
 *	its index in the list is found; else -1 is returned.
 *
 *  FORMAL PARAMETERS:
 *
 *	list_id		The pointer list
 *	stg		the (case-sensitive) string to be found
 *
 *  IMPLICIT INPUTS:
 *
 *  IMPLICIT OUTPUTS:
 *
 *  FUNCTION VALUE:
 *
 *	0-based index in list if found
 *	-1 if not found
 *
 *  SIDE EFFECTS:
 *
 *--
 */

{

/*
 *  External Functions
 */

/*
 *  Local variables
 */
MrmCount		ndx ;		/* search index */


for ( ndx=0 ; ndx<UrmPlistNum(list_id) ; ndx++ )
    if ( strcmp(stg,(String)UrmPlistPtrN(list_id,ndx)) == 0 )
        return ndx ;
return -1 ;

}

