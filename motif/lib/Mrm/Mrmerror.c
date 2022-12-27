#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)Mrmerror.c	3.3 90/07/20";
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
 *	All error signalling and handling routines are found here.
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
externaldef(urm__err_out)		MrmCode	urm__err_out = URMErrOutStdout ;
externaldef(urm__latest_error_code)	MrmCode	urm__latest_error_code = 0 ;
externaldef(urm__latest_error_msg)	String	urm__latest_error_msg = NULL ;

/*
 *
 *  OWN VARIABLE DECLARATIONS
 *
 */
#ifdef _NO_PROTO
String Urm__UT_UrmCodeString ();
#endif

static	String	urm_codes_codstg[] = {
    "MrmFAILURE"
    ,"MrmSUCCESS"
    ,"MrmNOT_FOUND"
    ,"MrmCREATE_NEW"
    ,"MrmEXISTS"
    ,"URMIndex retry"
    ,"MrmNUL_GROUP"
    ,"MrmINDEX_GT"
    ,"MrmNUL_TYPE"
    ,"MrmINDEX_LT"
    ,"MrmWRONG_GROUP"
    ,"MrmPARTIAL_SUCCESS"
    ,"MrmWRONG_TYPE"
    ,"URM unused code 13"
    ,"MrmOUT_OF_RANGE"
    ,"URM unused code 15"
    ,"MrmBAD_RECORD"
    ,"URM unused code 17"
    ,"MrmNULL_DATA"
    ,"URM unused code 19"
    ,"MrmBAD_DATA_INDEX"
    ,"URM unused code 21"
    ,"MrmBAD_ORDER"
    ,"URM unused code 23"
    ,"MrmBAD_CONTEXT"
    ,"URM unused code 25"
    ,"MrmNOT_VALID"
    ,"URM unused code 27"
    ,"MrmBAD_BTREE"
    ,"URM unused code 29"
    ,"MrmBAD_WIDGET_REC"
    ,"URM unused code 31"
    ,"MrmBAD_CLASS_TYPE"
    ,"URM unused code 33"
    ,"MrmNO_CLASS_NAME"
    ,"URM unused code 35"
    ,"MrmTOO_MANY"
    ,"URM unused code 37"
    ,"MrmBAD_IF_MODULE"
    ,"URM unused code 39"
    ,"MrmNULL_DESC"
    ,"URM unused code 41"
    ,"MrmOUT_OF_BOUNDS"
    ,"URM unused code 43"
    ,"MrmBAD_COMPRESS"
    ,"URM unused code 45"
    ,"MrmBAD_ARG_TYPE"
    ,"URM unused code 47"
    ,"MrmNOT_IMP"
    ,"URM unused code 49"
    ,"MrmNULL_INDEX"
    ,"URM unused code 51"
    ,"MrmBAD_KEY_TYPE"
    ,"URM unused code 53"
    ,"MrmBAD_CALLBACK"
    ,"URM unused code 55"
    ,"MrmNULL_ROUTINE"
    ,"URM unused code 57"
    ,"MrmVEC_TOO_BIG"
    ,"URM unused code 59"
    ,"MrmBAD_HIERARCHY"
    ,"URM unused code 61"
    ,"MrmBAD_CLASS_CODE"
    } ;

static String urm_codes_invalidcode = "Invalid URM code" ;




Cardinal Urm__UT_Error (module, error, file_id, context_id, status)
    char			*module ;
    char			*error ;
    IDBFile			file_id ;
    URMResourceContextPtr	context_id ;
    Cardinal			status ;

/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	This routine is an error signalling routine for use within URM.
 *	It currently just reports the error on the terminal.
 *
 *  FORMAL PARAMETERS:
 *
 *	module		Identifies the module (routine) detecting the error
 *	error		Brief description of the error
 *	file_id		if not NULL, the IDB file implicated in the error
 *	context_id	if not NULL, the URM resource implicated in the error
 *	status		the return code associated with the error.
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

char		msg[300] ;	/* error message */
String		perm_msg ;	/* allocated message */

/*
 * construct error message
 */

/* Old form
sprintf (msg, "%s detected error %s - %s", module, error,
    Urm__UT_UrmCodeString(status)) ;
 */
sprintf (msg, "%s: %s - %s", module, error, Urm__UT_UrmCodeString(status)) ;

/*
 * Print or save the message depending on the reporting style
 */
urm__latest_error_code = status ;

switch ( urm__err_out )
    {
    case URMErrOutMemory:
        if ( urm__latest_error_msg != NULL )
            XtFree (urm__latest_error_msg) ;
        urm__latest_error_msg = (String) XtMalloc (strlen(msg)+1) ;
        strcpy (urm__latest_error_msg, msg) ;
        return status ;
    case URMErrOutStdout:
    default:
        XtWarning (msg) ;
        return status ;
    }

}



Cardinal Urm__UT_SetErrorReport 

#ifndef _NO_PROTO
	(MrmCode report_type)
#else
	(report_type)
    MrmCode			report_type ;
#endif

/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	This routine sets the URM error report type to a standard state
 *
 *  FORMAL PARAMETERS:
 *
 *	report_type	URMErrOutMemory	- save message in memory, don't print
 *			URMErrOutStdout	- report to stdout
 *
 *  IMPLICIT INPUTS:
 *
 *	urm__err_out
 *
 *  IMPLICIT OUTPUTS:
 *
 *	urm__err_out
 *
 *  FUNCTION VALUE:
 *
 *      MrmSUCCESS	operation succeeded
 *	MrmFAILURE	illegal state requested
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
switch ( report_type )
    {
    case URMErrOutMemory:
    case URMErrOutStdout:
        urm__err_out = report_type ;
        return MrmSUCCESS ;
    default:
        return MrmFAILURE ;
    }

}



MrmCode Urm__UT_LatestErrorCode ()

/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	 Returns the current error code
 *
 *  FORMAL PARAMETERS:
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

return urm__latest_error_code ;

}



String Urm__UT_LatestErrorMessage ()

/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	 Returns the current error message
 *
 *  FORMAL PARAMETERS:
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

return urm__latest_error_msg ;

}



String Urm__UT_UrmCodeString 

#ifndef _NO_PROTO
	(MrmCode cod)
#else
	(cod)
    MrmCode		cod ;
#endif
/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	This routine returns a static string naming a URM return code.
 *
 *  FORMAL PARAMETERS:
 *
 *	cod		A URM return code
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
if ( cod>=MrmFAILURE && cod<=MrmBAD_CLASS_CODE )
    return urm_codes_codstg[cod] ;
return urm_codes_invalidcode ;

}

