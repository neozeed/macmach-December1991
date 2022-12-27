#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)Mrmlread.c	3.7 91/01/10";
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
 *	This module contains the literal read routines. All these routines
 *	read a literal from a hierarchy or IDB file into a resource context.
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
 *	UrmGetIndexedLiteral		Read indexed literal from IDB file
 *
 *	UrmGetRIDLiteral		Read RID literal from IDB file
 *
 *	UrmHGetIndexedLiteral		Read indexed literal from hierarchy
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



Cardinal Urm__FetchLiteral
	(hierarchy_id, index, context_id, ctxlist)
    MrmHierarchy		hierarchy_id;
    String			index;
    URMResourceContextPtr	context_id;
    URMPointerListPtr		*ctxlist;

/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	This routine is a service routine for the various XmFetch...Literal
 *	routine. It fetches a literal into a context. It also fetches any
 *	other literals which are referenced.
 *
 *  FORMAL PARAMETERS:
 *
 *	hierarchy_id	id of an open hierarchy containing the literal
 *	index		index of the desired literal
 *	context_id	context in which to return literal. Literal will
 *			be fixed up.
 *	ctxlist		list of other contexts created (if needed)
 *
 *  IMPLICIT INPUTS:
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
Cardinal		result;		/* function results */
IDBFile			file_id = NULL;	/* file containing literal */
int			val;		/* literal value */
MrmType			type;		/* literal type */
char			err_msg[300];	/* for error messages */

/*
 * read the literal into the context
 */
result = Urm__HGetIndexedLiteral (hierarchy_id, index, context_id, &file_id);
if ( result != MrmSUCCESS )
    {
    sprintf (err_msg, "Can't find literal '%s'", index);
    return Urm__UT_Error ("Urm__FetchLiteral", err_msg, NULL, NULL, result);
    }

/*
 * Acquire the literal type and value, and do any fixups which are required.
 */
val = (int) UrmRCBuffer (context_id);
type = UrmRCType (context_id);
switch ( type )
    {
    case MrmRtypeIconImage:
        UrmPlistInit (10, ctxlist);
        result = Urm__CW_LoadIconImage
	    ((RGMIconImagePtr)val, (caddr_t)val,
	     hierarchy_id, file_id, *ctxlist);
	break;
    default:
	Urm__CW_FixupValue (val, type, (caddr_t)val);
	break;
    }

return MrmSUCCESS;

}



Cardinal MrmFetchLiteral
#ifndef _NO_PROTO
    (MrmHierarchy		hierarchy_id,
    String			index,
    Display			*display,
    caddr_t			*value_return,
    MrmCode			*type_return)
#else
	(hierarchy_id, index, display, value_return, type_return)
    MrmHierarchy		hierarchy_id ;
    String			index ;
    Display			*display;
    caddr_t			*value_return ;
    MrmCode			*type_return ;
#endif

/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	This routine reads and returns the value of a literal (named value)
 *	stored as a public resource in a single UID file. It returns a
 *	pointer to value of the literal, fixed up and converted for
 *	use as a toolkit argument. The return is always a pointer -- an
 *	integer is returned as a pointer to an integer, and a string is
 *	returned as a pointer to a string. The caller is responsible for
 *	freeing the literal's storage, which was acquired with XtMalloc.
 *
 *	This routine should not be used for fetching icon or color literals,
 *	as more information is required for converting them. If fetched,
 *	an error is returned.
 *
 *  FORMAL PARAMETERS:
 *
 *	hierarchy_id	id of an open hierarchy containing the literal
 *	index		index of the desired literal
 *	value_return	to return pointer to literal value
 *	type_return	will be set to the literal data type, from RGMrType...
 *
 *  IMPLICIT INPUTS:
 *
 *  IMPLICIT OUTPUTS:
 *
 *  FUNCTION VALUE:
 *
 *	MrmSUCCESS	- literal found and returned
 *	MrmWRONG_TYPE	- unsupported literal type encountered
 *	MrmNOT_FOUND	- literal not found
 *	MrmFAILURE	- file system error
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
Cardinal		result;		/* function results */
URMResourceContextPtr	context_id;	/* for the literal */
URMPointerListPtr	ctxlist = NULL;	/* save added contexts */
Cardinal		ndx;		/* loop index */

/*
 * Read the literal, discard the context, and return.
 */
UrmGetResourceContext (NULL, NULL, 0, &context_id);
result = Urm__FetchLiteral (hierarchy_id, index, context_id, &ctxlist);
if ( result == MrmSUCCESS )
    {
    *value_return = (caddr_t) UrmRCBuffer (context_id);
    (*type_return) = (MrmCode) UrmRCType (context_id);
    (*(context_id->free_func)) (context_id) ;
    switch ( *type_return )
	{
	case MrmRtypeIconImage:
	case MrmRtypeColor:
	case MrmRtypeColorTable:
	case MrmRtypeXBitmapFile:
	    if ( ctxlist != NULL )
		{
		for ( ndx=0 ; ndx<UrmPlistNum(ctxlist) ; ndx++ )
		    UrmFreeResourceContext (
			  (URMResourceContextPtr)UrmPlistPtrN(ctxlist,ndx)) ;
		UrmPlistFree (ctxlist) ;
		}

	    UrmFreeResourceContext (context_id);
	    return MrmWRONG_TYPE;
	    break ;

	default :
	    /*
	    **  Do necessary conversions (Fixups were done by Urm__FetchLiteral)
	    */
	    result = Urm__CW_ConvertValue
		((int*)value_return, (MrmType)*type_return, NULL, display,
		 hierarchy_id, NULL) ;
	    if ( result != MrmSUCCESS ) return MrmFAILURE;
	    break;
	}
    return MrmSUCCESS;
    }
else
    {
    (*(context_id->free_func)) (context_id) ;
    return result;
    }

}



Cardinal MrmFetchIconLiteral

#ifndef _NO_PROTO
    (MrmHierarchy                hierarchy_id,
    String                      index,
    Screen                      *screen,
    Display                     *display,
    Pixel                       fgpix,
    Pixel                       bgpix,
    Pixmap                      *pixmap_return)
#else
(hierarchy_id, index, screen, display, fgpix, bgpix, pixmap_return)
    MrmHierarchy		hierarchy_id ;
    String			index ;
    Screen			*screen ;
    Display			*display ;
    Pixel			fgpix ;
    Pixel			bgpix ;
    Pixmap			*pixmap_return ;
#endif
/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	This routine fetches an icon literal from a hierarchy. It converts
 *	the icon to an X pixmap.
 *
 *  FORMAL PARAMETERS:
 *
 *	hierarchy_id	id of an open hierarchy containing the literal
 *	index		index of the desired literal
 *	screen		screen to use for pixmap
 *	display		display to use for pixmap
 *	fgpix		foreground color for pixmap
 *	bgpix		background color for pixmap
 *	pixmap_return	to return resulting X pixmap
 *
 *  IMPLICIT INPUTS:
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
Cardinal		result;		/* function results */
URMResourceContextPtr	context_id;	/* for the literal */
URMPointerListPtr	ctxlist = NULL;	/* save added contexts */
Cardinal		ndx;		/* loop index */
MrmType			type;

/*
 * Read the literal, and deal with errors
 */
UrmGetResourceContext (NULL, NULL, 0, &context_id);
result = Urm__FetchLiteral (hierarchy_id, index, context_id, &ctxlist);
if ( result == MrmSUCCESS ) {
    type = UrmRCType(context_id);
    }
else
    {
    if ( ctxlist != NULL )
	{
	for ( ndx=0 ; ndx<UrmPlistNum(ctxlist) ; ndx++ )
	    UrmFreeResourceContext (
              (URMResourceContextPtr)UrmPlistPtrN(ctxlist,ndx)) ;
	UrmPlistFree (ctxlist) ;
	}
    UrmFreeResourceContext (context_id);
    return result;
    }

/*
 * Convert to X pixmap, and free resources
 */
switch (type) {
    case MrmRtypeIconImage:
	result = UrmCreatePixmap
	    ((RGMIconImagePtr)UrmRCBuffer(context_id), screen, display,
	     fgpix, bgpix, pixmap_return);
	break;
    case MrmRtypeXBitmapFile:
	result = Urm__CW_ReadBitmapFile (UrmRCBuffer(context_id), screen, 
	    fgpix, bgpix, pixmap_return);
	break;
    default:
	result = MrmWRONG_TYPE;
    }

if ( ctxlist != NULL )
    {
    for ( ndx=0 ; ndx<UrmPlistNum(ctxlist) ; ndx++ )
	UrmFreeResourceContext (
           (URMResourceContextPtr)UrmPlistPtrN(ctxlist,ndx)) ;
    UrmPlistFree (ctxlist) ;
    }
UrmFreeResourceContext (context_id);

return result;

}



Cardinal MrmFetchColorLiteral 

#ifndef _NO_PROTO
    (MrmHierarchy                hierarchy_id,
    String                      index,
    Display                     *display,
    Colormap                    cmap,
    Pixel                       *pixel_return)
#else
(hierarchy_id, index, display, cmap, pixel_return)
    MrmHierarchy		hierarchy_id ;
    String			index ;
    Display			*display ;
    Colormap			cmap ;
    Pixel			*pixel_return ;
#endif

/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	This routine fetches a named color literal from a UID file, and
 *	converts it to a pixel value.
 *
 *  FORMAL PARAMETERS:
 *
 *	hierarchy_id	id of an open hierarchy containing the literal
 *	index		index of the desired literal
 *	display		display to use for pixmap
 *	cmap		color map ID. If NULL, the default color map is used
 *	pixel_return	to return the pixel value for the color
 *
 *  IMPLICIT INPUTS:
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
Cardinal		result;		/* function results */
URMResourceContextPtr	context_id;	/* for the literal */
URMPointerListPtr	ctxlist = NULL;	/* save added contexts */
Cardinal		ndx;		/* loop index */
RGMColorDescPtr		colorptr;
char                    err_msg[300] ;

/*
 * Read the literal, and deal with errors.
 */
UrmGetResourceContext (NULL, NULL, 0, &context_id);
result = Urm__FetchLiteral (hierarchy_id, index, context_id, &ctxlist);
if ( result == MrmSUCCESS )
    if (UrmRCType(context_id) != MrmRtypeColor)
	result = MrmWRONG_TYPE;
if ( result != MrmSUCCESS)
    {
    if ( ctxlist != NULL )
	{
	for ( ndx=0 ; ndx<UrmPlistNum(ctxlist) ; ndx++ )
	    UrmFreeResourceContext (
                   (URMResourceContextPtr)UrmPlistPtrN(ctxlist,ndx)) ;
	UrmPlistFree (ctxlist) ;
	}
    UrmFreeResourceContext (context_id);
    return result;
    }

/*
 * Convert to X named color, and free resources
 */
colorptr = (RGMColorDescPtr) UrmRCBuffer(context_id);

switch (colorptr->desc_type)
    {
     case URMColorDescTypeName:
	result = Urm__UT_GetNamedColorPixel
	    (display, cmap, colorptr, pixel_return);
	break;
    case URMColorDescTypeRGB:
	result = Urm__UT_GetColorPixel
	    (display, cmap, colorptr, pixel_return);
	break;
    default:
	sprintf
	    (err_msg,"Invalid color descriptor type");
	return Urm__UT_Error ("MrmFetchColorLiteral",
			      err_msg, NULL, NULL, MrmFAILURE) ;

    };
UrmFreeResourceContext (context_id);

return result;

}



Cardinal UrmGetIndexedLiteral (file_id, index, context_id)
    IDBFile			file_id ;
    String			index ;
    URMResourceContextPtr	context_id ;

/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	UrmGetIndexedLiteral searches a single database file for a literal
 *	given its index (i.e. it gets a public literal from a single file).
 *	It returns the literal as the contents of the context buffer. The
 *	group which is fetched is always URMgLiteral. The literal type
 *	filter is taken from the context; if unmodified in the context
 *	as obtained from UrmGetResourceContext, there is no filtering
 *	(type=RGMtNul).
 *
 *	The buffer contents will be as follows for some common literal
 *	types obtained from a .UID file. Note that in some cases that
 *	the caller must fix up offsets to be memory pointers.
 *
 *	UrmRCType(context_id) == MrmRtypeChar8:
 *		UrmRCBuffer(context_id) contains a nul-terminated
 *		ASCII string
 *
 *	UrmRCType(context_id) == MrmRtypeCString:
 *		UrmRCBuffer(context_id) contains a compound string
 *
 *	UrmRCType(context_id) == MrmRtypeChar8Vector:
 *	UrmRCType(context_id) == MrmRtypeCStringVector:
 *		UrmRCBuffer(context_id) contains an RGM text vector
 *		or stringtable (RGMTextVector). The items in the
 *		text vector contain offsets into the buffer which
 *		locate either nul-terminated ASCII strings or compound
 *		compound strings. These may be relocated to memory
 *		pointers by adding the buffer address to the offset, i.e.
 *		item[n].text_item.pointer = item[n].text_item.offset+bufadr
 *
 *  FORMAL PARAMETERS:
 *
 *	file_id		id of an open URM database file (IDB file)
 *	index		index of the desired literal
 *	context_id	resource context into which to read the literal
 *
 *  IMPLICIT INPUTS:
 *
 *  IMPLICIT OUTPUTS:
 *
 *  FUNCTION VALUE:
 *
 *	MrmSUCCESS	operation succeeded
 *	MrmBAD_CONTEXT	invalid resource context
 *	Other		See UrmIdbGetIndexedResource
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
MrmType		lit_type ;	/* the type of the literal */


/*
 * Validate context, then attempt the read.
 */
if ( ! UrmRCValid(context_id) )
    return Urm__UT_Error
        ("UrmGetIndexedLiteral", "Invalid resource context",
        file_id, context_id, MrmBAD_CONTEXT) ;

lit_type = UrmRCType (context_id) ;
return UrmIdbGetIndexedResource
    (file_id, index, URMgLiteral, lit_type, context_id) ;

}			/* end of routine UrmGetIndexedLiteral */



Cardinal UrmGetRIDLiteral (file_id, resource_id, context_id)
    IDBFile			file_id ;
    MrmResource_id		resource_id ;
    URMResourceContextPtr	context_id ;

/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	UrmGetRIDLiteral retrieves a literal from a single database file
 *	given its resource id as an accessor. It returns the literal record.
 *
 *  FORMAL PARAMETERS:
 *
 *	file_id		id of an open URM database file (IDB file)
 *	resource_id	resource id for literal
 *	context_id	literal context in which to return record read in
 *
 *  IMPLICIT INPUTS:
 *
 *  IMPLICIT OUTPUTS:
 *
 *  FUNCTION VALUE:
 *
 *	MrmSUCCESS	operation succeeded
 *	MrmNOT_FOUND	literal not found
 *	MrmFAILURE	operation failed, further reason not given.
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
MrmType		lit_type ;	/* the type of the literal */


/*
 * Validate context, then attempt the read.
 */
if ( ! UrmRCValid(context_id) )
    return Urm__UT_Error
        ("UrmGetRIDLiteral", "Invalid resource context",
        file_id, context_id, MrmBAD_CONTEXT) ;

lit_type = UrmRCType (context_id) ;
return UrmIdbGetRIDResource
    (file_id, resource_id, URMgLiteral, lit_type, context_id) ;

}			/* end of routine UrmGetRIDLiteral */



Cardinal Urm__HGetIndexedLiteral
        (hierarchy_id, index, context_id, file_id_return)
    MrmHierarchy		hierarchy_id ;
    String			index ;
    URMResourceContextPtr	context_id ;
    IDBFile			*file_id_return ;

/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	Urm__HGetIndexedLiteral is exactly like UrmGetLiteral except
 *	that it searches a hierarchy for the literal rather than reading
 *	from a single file.
 *
 *  FORMAL PARAMETERS:
 *
 *	hierarchy_id	hierarchy to be searched
 *	index		index of the desired literal
 *	context_id	resource context into which to read the literal
 *	file_id_return	to return IDB file in which literal was found
 *
 *  IMPLICIT INPUTS:
 *
 *  IMPLICIT OUTPUTS:
 *
 *  FUNCTION VALUE:
 *
 *	MrmSUCCESS	operation succeeded
 *	MrmBAD_CONTEXT	invalid resource context
 *	Other		See UrmIdbGetIndexedResource
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
MrmType			lit_type ;	/* the type of the literal */

/*
 * Validate hierarchy and context, then attempt the read.
 */
if ( hierarchy_id == NULL )
    return Urm__UT_Error
        ("Urm__HGetIndexedLiteral", "NULL hierarchy id",
        NULL, NULL, MrmBAD_HIERARCHY) ;
if ( ! MrmHierarchyValid(hierarchy_id) )
    return Urm__UT_Error
        ("Urm__HGetIndexedLiteral", "Invalid hierarchy",
        NULL, NULL, MrmBAD_HIERARCHY) ;
if ( ! UrmRCValid(context_id) )
    return Urm__UT_Error
        ("Urm__HGetIndexedLiteral", "Invalid resource context",
        NULL, context_id, MrmBAD_CONTEXT) ;

lit_type = UrmRCType (context_id) ;
return UrmHGetIndexedResource
    (hierarchy_id, index, URMgLiteral, lit_type, context_id, file_id_return) ;

}



Cardinal UrmHGetIndexedLiteral (hierarchy_id, index, context_id)
    MrmHierarchy		hierarchy_id ;
    String			index ;
    URMResourceContextPtr	context_id ;

/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	UrmHGetIndexedLiteral is exactly like UrmGetLiteral except
 *	that it searches a hierarchy for the literal rather than reading
 *	from a single file.
 *
 *  FORMAL PARAMETERS:
 *
 *	hierarchy_id	hierarchy to be searched
 *	index		index of the desired literal
 *	context_id	resource context into which to read the literal
 *
 *  IMPLICIT INPUTS:
 *
 *  IMPLICIT OUTPUTS:
 *
 *  FUNCTION VALUE:
 *
 *	MrmSUCCESS	operation succeeded
 *	MrmBAD_CONTEXT	invalid resource context
 *	Other		See UrmIdbGetIndexedResource
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
MrmType			lit_type ;	/* the type of the literal */
IDBFile			dummy ;		/* unused file return */


return Urm__HGetIndexedLiteral (hierarchy_id, index, context_id, &dummy) ;

}

