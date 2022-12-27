#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)Mrmwwrite.c	3.1 90/03/01";
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
 *	This module contains all the routines which write a widget
 *	from a resource context into an IDB file.
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
 *	UrmPutIndexedWidget		Write indexed widget to IDB file
 *
 *	UrmPutRIDWidget			Write RID widget to IDB file
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




Cardinal UrmPutIndexedWidget (file_id, index, context_id)
    IDBFile			file_id ;
    String			index ;
    URMResourceContextPtr	context_id ;

/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	UrmPutIndexedWidget puts a URMgWidget resource record in the
 *	database. Its contents are the widget record. The resource type is
 *	the value of the widget record's type field. The access is the
 *	value of the record's access field; the locking attribute is the
 *	value of the record's lock field.
 *
 *  FORMAL PARAMETERS:
 *
 *	file_id		file into which to write record
 *	index		case-sensitive index for the widget
 *	context_id	resource context containing widget record
 *
 *  IMPLICIT INPUTS:
 *
 *  IMPLICIT OUTPUTS:
 *
 *  FUNCTION VALUE:
 *
 *	MrmSUCCESS	operation succeeded
 *	MrmBAD_CONTEXT	invalid resource context
 *	MrmBAD_WIDGET_REC	invalid widget record in context
 *	Others:		See UrmIdbPutIndexedResource
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
RGMWidgetRecordPtr	widgetrec ;	/* Widget record in context */


/*
 * Validate record, then set resource group and type, and enter in
 * IDB file.
 */
if ( ! UrmRCValid(context_id) )
    return Urm__UT_Error
        ("UrmPutIndexedWidget", "Invalid resource context",
        file_id, NULL, MrmBAD_CONTEXT) ;
widgetrec = (RGMWidgetRecordPtr) UrmRCBuffer (context_id) ;
if ( ! UrmWRValid(widgetrec) )
    return Urm__UT_Error
        ("UrmPutIndexedWidget", "Invalid widget record",
        file_id, context_id, MrmBAD_WIDGET_REC) ;

UrmRCSetSize (context_id, widgetrec->size) ;
UrmRCSetGroup (context_id, URMgWidget) ;
UrmRCSetType (context_id, widgetrec->type) ;
UrmRCSetAccess (context_id, widgetrec->access) ;
UrmRCSetLock (context_id, widgetrec->lock) ;

result = UrmIdbPutIndexedResource (file_id, index, context_id) ;
return result ;

}



Cardinal UrmPutRIDWidget (file_id, resource_id, context_id)
    IDBFile			file_id ;
    MrmResource_id		resource_id ;
    URMResourceContextPtr	context_id ;

/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	UrmPutRIDWidget puts a widget accessed by a resource id into the
 *	given database file. The resource type is the value of the widget
 *	record's type field. As usual for resources accessed by resource id,
 *	the access is always private; this routine resets the record's access
 *	field if necessary to be consistent with private access. The value
 *	of the locking attribute is the value of the record's lock field.
 *
 *  FORMAL PARAMETERS:
 *
 *	file_id		file into which to write record
 *	resource_id	resource id for the record
 *	context_id	resource context containing widget record
 *
 *  IMPLICIT INPUTS:
 *
 *  IMPLICIT OUTPUTS:
 *
 *  FUNCTION VALUE:
 *
 *	MrmSUCCESS	operation succeeded
 *	MrmBAD_CONTEXT	invalid resource context
 *	MrmBAD_WIDGET_REC	invalid widget record in context
 *	Others:		See UrmIdbPutRIDResource
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
RGMWidgetRecordPtr	widgetrec ;	/* Widget record in context */


/*
 * Validate record, then set resource group and type, and enter in
 * IDB file.
 */
if ( ! UrmRCValid(context_id) )
    return Urm__UT_Error
        ("UrmPutRIDWidget", "Invalid resource context",
        file_id, NULL, MrmBAD_CONTEXT) ;
widgetrec = (RGMWidgetRecordPtr) UrmRCBuffer (context_id) ;
if ( ! UrmWRValid(widgetrec) )
    return Urm__UT_Error
        ("UrmPutRIDWidget", "Invalid widget record",
        file_id, context_id, MrmBAD_WIDGET_REC) ;

UrmRCSetSize (context_id, widgetrec->size) ;
UrmRCSetGroup (context_id, URMgWidget) ;
UrmRCSetType (context_id, widgetrec->type) ;
UrmRCSetAccess (context_id, URMaPrivate) ;
UrmRCSetLock (context_id, widgetrec->lock) ;

result = UrmIdbPutRIDResource (file_id, resource_id, context_id) ;
return result ;

}

