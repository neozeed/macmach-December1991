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
 *      UIL Manager (URM)
 *
 *  ABSTRACT:
 *
 *      This module contains the keyword tables and functions used by to
 *	support the compressing and uncompressing of strings in URM.
 *
 *--
 */


/*
 *
 *  INCLUDE FILES
 *
 */


#include <X11/Intrinsic.h>
#include <Mrm/MrmAppl.h>
#include <Mrm/Mrm.h>

/*
 *
 *  TABLE of CONTENTS
 *
 *	Urm__FixupCompressionTable	make table memory resident
 *
 *	Urm__FindClassDescriptor	find descriptor in file/memory
 *
 *	Urm__UncompressCode		uncompress a code
 *
 *	Urm__IsSubtreeResource		predicate for subtree resources
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



Cardinal Urm__FixupCompressionTable 
#ifndef _NO_PROTO
    (UidCompressionTablePtr	ctable,
    Boolean			qfindcldesc)
#else
(ctable, qfindcldesc)
    UidCompressionTablePtr	ctable;
    Boolean			qfindcldesc;
#endif

/*
 *++
 *
 *  FUNCTIONAL DESCRIPTION:
 *
 *	This routine fixes up a file-based compression table. It resolves
 *	offsets into memory pointers. If requested, it then looks up
 *	each resulting string and attempts to replace it with a
 *	function pointer (from the function hash table).
 *
 *  FORMAL PARAMETERS:
 *
 *	ctable		the compression table to fix up
 *	qfindcldesc	if TRUE, attempt to look up the strings as
 *			indexes of class descriptors
 *
 *  IMPLICIT INPUTS:
 *
 *  IMPLICIT OUTPUTS:
 *
 *      none
 *
 *  FUNCTION VALUE:
 *
 *	MrmSUCCESS	if all fixup operations succeed
 *	MrmFAILURE	if any operation fails (usually function lookup)    
 *
 * SIDE EFFECTS:
 *
 *	none
 *
 *--
 */

{

/*
 *  Local variables
 */
int			fixndx ;	/* table fixup loop index */
Cardinal		result = MrmSUCCESS;
WCIClassDescPtr		cldesc;		/* for class descriptor */
Cardinal		clres;		/* lookup result */
Widget			(*func) ();	/* function looked up */


/*
 * Fix up the table offsets to be pointers
 */
for ( fixndx=UilMrmMinValidCode ; fixndx<ctable->num_entries ; fixndx++ )
    ctable->entry[fixndx].cstring = (char *)
	((int)ctable+ctable->entry[fixndx].stoffset);
/*
 * Look up each string as a function if requested
 */
if ( qfindcldesc )
    for ( fixndx=UilMrmMinValidCode ; fixndx<ctable->num_entries ; fixndx++ )
	{
	clres = Urm__WCI_LookupClassDescriptor
	    (ctable->entry[fixndx].cstring, &cldesc);
	if ( result == MrmSUCCESS )
	    ctable->entry[fixndx].cldesc = cldesc;
	else
	    {
	    ctable->entry[fixndx].cldesc = NULL;
	    result = MrmFAILURE;
	    }
	}

return result;

}    



Cardinal Urm__FindClassDescriptor 
#ifndef _NO_PROTO
    (IDBFile			cfile,
    MrmCode			code,
    char			*name,
    WCIClassDescPtr		*class_return)
#else
(cfile, code, name, class_return)
    IDBFile			cfile;
    MrmCode			code ;
    char			*name;
    WCIClassDescPtr		*class_return ;
#endif

/*
 *++
 *
 *  FUNCTIONAL DESCRIPTION:
 *
 *	This routine finds a class descriptor corresponding to a class
 *	compression code or name. It looks up the class in the file's
 *	compression table if possible. Otherwise, it uses the older built-in
 *	tables.
 *
 *  FORMAL PARAMETERS:
 *
 *	cfile		IDB file in which to find compression table
 *	code		compression code to be uncompressed
 *	name		the class name; the convenience function name.
 *	class_return	to return the class descriptor
 *
 *  IMPLICIT INPUTS:
 *
 *  IMPLICIT OUTPUTS:
 *
 *      none
 *
 *  FUNCTION VALUE:
 *
 *	MrmSUCCESS	if class descriptor found
 *	MrmFAILURE	otherwise
 *
 * SIDE EFFECTS:
 *
 *	none
 *
 *--
 */

{

/*
 *  Local variables
 */
char			errmsg[300];


/*
 * Use the built-in tables if the file has none. Else a simple lookup.
 */
if ( code == UilMrmUnknownCode )
    return Urm__WCI_LookupClassDescriptor (name, class_return);
if ( cfile->class_ctable == NULL )
    return Urm__UT_Error
	("Urm__FindClassDescriptor",
	 "UID file is obsolete - has no compression table",
	 NULL, NULL, MrmFAILURE);
if ( code < UilMrmMinValidCode )
    return MrmFAILURE;
if ( code >= cfile->class_ctable->num_entries )
    return MrmFAILURE;
*class_return = cfile->class_ctable->entry[code].cldesc;
if ( *class_return == NULL )
    return MrmFAILURE;
return MrmSUCCESS;

}    



Cardinal Urm__UncompressCode 
#ifndef _NO_PROTO
    (IDBFile			cfile,
    MrmCode			code,
    String			*stg_return)
#else
(cfile, code, stg_return)
    IDBFile			cfile;
    MrmCode			code ;
    String			*stg_return ;
#endif

/*
 *++
 *
 *  FUNCTIONAL DESCRIPTION:
 *
 *	This routine returns the string corresponding to a compression code.
 *	It looks up the code in the file's compression table if it has
 *	one, else falls back on the old built-in tables. The code is looked
 *	up in the resource tables.
 *
 *  FORMAL PARAMETERS:
 *
 *	cfile		IDB file in which to find compression table
 *	code		compression code to be uncompressed
 *	stg_return	to return result of uncompression
 *
 *  IMPLICIT INPUTS:
 *
 *  IMPLICIT OUTPUTS:
 *
 *      none
 *
 *  FUNCTION VALUE:
 *
 *	MrmSUCCESS	if uncompression successful
 *	MrmFAILURE	otherwise
 *
 * SIDE EFFECTS:
 *
 *	none
 *
 *--
 */

{

/*
 *  Local variables
 */


/*
 * A simple lookup in the file's compression tables.
 */
if ( cfile->resource_ctable == NULL )
    return Urm__UT_Error
	("Urm__UncompressCode",
	 "UID file is obsolete - has no compression table",
	 NULL, NULL, MrmFAILURE);
if ( code < UilMrmMinValidCode )
    return MrmFAILURE;
if ( code >= cfile->resource_ctable->num_entries )
    return MrmFAILURE;
*stg_return = cfile->resource_ctable->entry[code].cstring;
return MrmSUCCESS;

}    



Boolean Urm__IsSubtreeResource 
#ifndef _NO_PROTO
    (IDBFile			cfile,
    MrmCode			code)
#else
(cfile, code)
    IDBFile			cfile;
    MrmCode			code ;
#endif

/*
 *++
 *
 *  FUNCTIONAL DESCRIPTION:
 *
 *	This routine checks to see if a resource is marked as rooting a
 *	widget subtree, that is, is a resource which requires that a
 *	widget subtree be instantiated as its value.
 *
 *	Initial version uses crude check. To be replaced by table lookup.
 *
 *  FORMAL PARAMETERS:
 *
 *	cfile		file containing table information
 *	code		code to be checked
 *
 *  IMPLICIT INPUTS:
 *
 *  IMPLICIT OUTPUTS:
 *
 *      none
 *
 *  FUNCTION VALUE:
 *
 *	TRUE		if subtree value is required
 *	FALSE		otherwise
 *
 * SIDE EFFECTS:
 *
 *	none
 *
 *--
 */

{

/*
 * Do a string comparison for for the subtree resources.
 */
if ( code < UilMrmMinValidCode )
    return FALSE;
if ( code >= cfile->resource_ctable->num_entries )
    return FALSE;

if ( strcmp(cfile->resource_ctable->entry[code].cstring,XmNsubMenuId) == 0 )
    return TRUE;
return FALSE;

}    


