#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)Mrmcontext.c	3.4 90/07/20";
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
 *++
 *  FACILITY:
 *
 *      UIL Resource Manager (URM):
 *	
 *	URM context routines
 *
 *  ABSTRACT:
 *
 *	These routines create, modify, and delete a URM context structure.
 *	Note that accessors for a URM context are macros.
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
 *	UrmGetResourceContext		- Allocate a new resource context
 *
 *	UrmResizeResourceContext	- Resize a resource context
 *
 *	UrmFreeResourceContext	- Free a resource context
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




Cardinal UrmGetResourceContext
#ifndef _NO_PROTO
    (char			*((*alloc_func) ()),
    void			(*free_func) (),
    MrmSize			size,
    URMResourceContextPtr	*context_id_return)
#else
        (alloc_func, free_func, size, context_id_return)
    char			*((*alloc_func) ()) ;
    void			(*free_func) () ;
    MrmSize			size ;
    URMResourceContextPtr	*context_id_return ;
#endif

/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	UrmGetResourceContext allocates a new resource context, then
 *	allocates a memory buffer of the requested size and associates it
 *	with the context.
 *
 *  FORMAL PARAMETERS:
 *
 *	alloc_func		Function to use in allocating memory for this
 *				context. A null pointer means use default
 *				(XtMalloc).
 *	free_func		Function to use in freeing memory for this
 * 				context. A null pointer means use default
 *				(XtFree).
 *	size			Size of memory buffer to allocate.
 *	context_id_return	To return new context.
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
 * Set function defaults if NULL
 */
if ( alloc_func == NULL ) alloc_func = XtMalloc ;
if ( free_func == NULL ) free_func = XtFree ;

/*
 * Allocate the context buffer and memory buffer, and set the
 * context up.
 */
*context_id_return =
    (URMResourceContextPtr) (*alloc_func) (sizeof(URMResourceContext)) ;
if ( *context_id_return == NULL )
    return Urm__UT_Error
        ("UrmGetResourceContext", "Context allocation failed",
        NULL, *context_id_return, MrmFAILURE) ;

(*context_id_return)->validation = URMResourceContextValid ;
(*context_id_return)->data_buffer = NULL ;
if ( size < 0 ) size = 0 ;
if ( size > 0 )
    {
    (*context_id_return)->data_buffer = (char *) (*alloc_func) (size) ;
    if ( (*context_id_return)->data_buffer == NULL )
        {
        (*free_func) (*context_id_return) ;
        return Urm__UT_Error
            ("UrmGetResourceContext", "Buffer allocation failed",
            NULL, *context_id_return, MrmFAILURE) ;
        }
    }

(*context_id_return)->buffer_size = size ;
(*context_id_return)->resource_size = 0 ;
(*context_id_return)->group = URMgNul ;
(*context_id_return)->type = URMtNul ;
(*context_id_return)->access = 0 ;
(*context_id_return)->lock = 0 ;
(*context_id_return)->alloc_func = alloc_func ;
(*context_id_return)->free_func = free_func ;

/*
 * Context successfully created
 */
return MrmSUCCESS ;


}



Cardinal UrmResizeResourceContext (context_id, size)
    URMResourceContextPtr	context_id ;
    int				size ;

/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	UrmResizeResourceContext reallocates the memory buffer in a
 *	resource context to increase its size. The contents of the current
 *	memory buffer are copied into the new memory buffer. If the size
 *	parameter is smaller than the current memory buffer size, the request
 *	is ignored. If the size parameter is greater than MrmMaxResourceSize, a
 *	MrmTooMany error is returned
 *
 *  FORMAL PARAMETERS:
 *
 *	context_id	Resource context to receive resized memory buffer
 *	size		New size for memory buffer in resource context
 *
 *  IMPLICIT INPUTS:
 *
 *  IMPLICIT OUTPUTS:
 *
 *  FUNCTION VALUE:
 *
 *	MrmSUCCESS	operation succeeded
 *	MrmBAD_CONTEXT	Illegal resource context
 *	MrmFAILURE	memory allocation failure
 *	MrmTOO_MANY	size was larger than MrmMaxResourceSize
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
char			*newbuf ;	/* new buffer */


if ( ! UrmRCValid(context_id) )

    return Urm__UT_Error
        ("UrmResizeResourceContext", "Validation failed",
        NULL, context_id, MrmBAD_CONTEXT) ;

if ( size > MrmMaxResourceSize)
    return Urm__UT_Error
        ("MrmResizeResourceContext", "Resource size too large",
        NULL, context_id, MrmTOO_MANY) ;

/*
 * Resize unless buffer is bigger than requested size.
 */
if ( context_id->buffer_size > size ) return MrmSUCCESS ;

/*
 * Allocate the new buffer, copy the old buffer contents, and
 * update the context.
 */
if ( context_id->alloc_func == XtMalloc )
    {
    context_id->data_buffer = XtRealloc (context_id->data_buffer, size) ;
    context_id->buffer_size = size ;
    }
else
    {
    newbuf = (char *) (*(context_id->alloc_func)) (size) ;
    if ( newbuf == NULL )
        return Urm__UT_Error
            ("UrmResizeResourceContext", "Buffer allocation failed",
            NULL, context_id, MrmFAILURE) ;
    if ( context_id->data_buffer != NULL )
        {
	UrmBCopy (context_id->data_buffer, newbuf, context_id->buffer_size) ;
        (*(context_id->free_func)) (context_id->data_buffer) ;
        }
    context_id->data_buffer = newbuf ;
    context_id->buffer_size = size ;
    }

/*
 * Resize succeeded
 */
return MrmSUCCESS ;

}



Cardinal UrmFreeResourceContext (context_id)
    URMResourceContextPtr	context_id ;

/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	UrmFreeResourceContext frees the memory buffer and the
 *	resource context.
 *
 *  FORMAL PARAMETERS:
 *
 *	context_id	Resource context to be freed
 *
 *  IMPLICIT INPUTS:
 *
 *  IMPLICIT OUTPUTS:
 *
 *  FUNCTION VALUE:
 *
 *	MrmSUCCESS	operation succeeded
 *	MrmBAD_CONTEXT	Illegal resource context
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


if ( ! UrmRCValid(context_id) )
    return Urm__UT_Error
        ("UrmFreeResourceContext", "Validation failed",
        NULL, context_id, MrmBAD_CONTEXT) ;

context_id->validation = NULL ;
if ( context_id->data_buffer != NULL )
    (*(context_id->free_func)) (context_id->data_buffer) ;
(*(context_id->free_func)) (context_id) ;
return MrmSUCCESS ;

}
