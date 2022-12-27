#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)Mrmwread.c	3.1 90/03/01";
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
 *  ABSTRACT:
 *
 *	This module contains the widget read routines. All these routines
 *	read a widget from a hierarchy or IDB file into a resource context.
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
 *	UrmHGetWidget		Read indexed widget from hierarchy
 *
 *	UrmGetIndexedWidget	Read indexed widget from IDB file
 *
 *	UrmGetRIDWidget		Read RID widget from IDB file
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




Cardinal UrmHGetWidget (hierarchy_id, index, context_id, file_id_return)
    MrmHierarchy		hierarchy_id ;
    String			index ;
    URMResourceContextPtr	context_id ;
    IDBFile			*file_id_return ;

/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	UrmHGetWidget searches the database hierarchy for a public
 *	(EXPORTed) widget given its index. It returns the RGM widget record
 *	in a resource context.
 *
 *  FORMAL PARAMETERS:
 *
 *	hierarchy_id	id of an open URM database hierarchy
 *	index		index of the desired widget
 *	context_id	widget context in which to return record read in
 *	file_id_return	to return IDB file in which widget was found
 *
 *  IMPLICIT INPUTS:
 *
 *  IMPLICIT OUTPUTS:
 *
 *  FUNCTION VALUE:
 *
 *	MrmSUCCESS	operation succeeded
 *	MrmNOT_FOUND	widget not found
 *	MrmBAD_HIERARCHY	invalid URM file hierarchy
 *	MrmBAD_WIDGET_REC	invalid widget record in context
 *	Others		see UrmGetIndexedWidget
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
RGMWidgetRecordPtr	widgetrec ;	/* widget record in context */


/*
 * Get the widget
 */
result = UrmHGetIndexedResource
    (hierarchy_id, index, URMgWidget, URMtNul, context_id, file_id_return) ;
if ( result != MrmSUCCESS ) return result ;

/*
 * Validate the widget record in the context
 */
widgetrec = (RGMWidgetRecordPtr) UrmRCBuffer (context_id) ;
if ( ! UrmWRValid(widgetrec) )
    return Urm__UT_Error
        ("UrmHGetIndexedWidget", "Invalid widget record",
        NULL, context_id, MrmBAD_WIDGET_REC) ;

/*
 * successfully retrieved
 */
return MrmSUCCESS ;

}



Cardinal UrmGetIndexedWidget (file_id, index, context_id)
    IDBFile			file_id ;
    String			index ;
    URMResourceContextPtr	context_id ;

/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	UrmGetIndexedWidget searches a single database file for a widget
 *	given its index (i.e. it gets a public widget from a single file).
 *	It returns the RGM widget record.
 *
 *  FORMAL PARAMETERS:
 *
 *	file_id		id of an open URM database file (IDB file)
 *	index		index of the desired widget
 *	context_id	widget context in which to return record read in
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
Cardinal		result ;	/* function results */
RGMWidgetRecordPtr	widgetrec ;	/* widget record in context */


/*
 * Validate context, then attempt the read.
 */
if ( ! UrmRCValid(context_id) )
    return Urm__UT_Error
        ("UrmGetIndexedWidget", "Invalid resource context",
        file_id, NULL, MrmBAD_CONTEXT) ;

result =
    UrmIdbGetIndexedResource (file_id, index, URMgWidget, URMtNul, context_id) ;
if ( result != MrmSUCCESS ) return result ;

/*
 * Validate the widget record in the context
 */
widgetrec = (RGMWidgetRecordPtr) UrmRCBuffer (context_id) ;
if ( ! UrmWRValid(widgetrec) )
    return Urm__UT_Error
        ("UrmGetIndexedWidget", "Invalid widget record",
        NULL, context_id, MrmBAD_WIDGET_REC) ;

/*
 * successfully retrieved
 */
return MrmSUCCESS ;

}



Cardinal UrmGetRIDWidget (file_id, resource_id, context_id)
    IDBFile			file_id ;
    MrmResource_id		resource_id ;
    URMResourceContextPtr	context_id ;

/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	UrmGetRIDWidget retrieves a widget from a single database file
 *	given its resource id as an accessor. It returns the widget record.
 *
 *  FORMAL PARAMETERS:
 *
 *	file_id		id of an open URM database file (IDB file)
 *	resource_id	resource id for widget
 *	context_id	widget context in which to return record read in
 *
 *  IMPLICIT INPUTS:
 *
 *  IMPLICIT OUTPUTS:
 *
 *  FUNCTION VALUE:
 *
 *	MrmSUCCESS	operation succeeded
 *	MrmNOT_FOUND	widget not found
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
Cardinal		result ;	/* function results */
RGMWidgetRecordPtr	widgetrec ;	/* widget record in context */


/*
 * Validate context, then attempt the read.
 */
if ( ! UrmRCValid(context_id) )
    return Urm__UT_Error
        ("UrmGetRIDWidget", "Invalid resource context",
        file_id, NULL, MrmBAD_CONTEXT) ;

result = UrmIdbGetRIDResource (file_id, resource_id,
    URMgWidget, URMgNul, context_id) ;
if ( result != MrmSUCCESS ) return result ;

/*
 * Validate the widget record in the context
 */
widgetrec = (RGMWidgetRecordPtr) UrmRCBuffer (context_id) ;
if ( ! UrmWRValid(widgetrec) )
    return Urm__UT_Error
        ("UrmGetIndexedWidget", "Invalid widget record",
        NULL, context_id, MrmBAD_WIDGET_REC) ;

/*
 * successfully retrieved
 */
return MrmSUCCESS ;

}

