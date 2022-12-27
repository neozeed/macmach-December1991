#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)MrmItopw.c	3.4 91/01/10";
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
 *      UIL Resource Manager (URM): IDB facility
 *
 *  ABSTRACT:
 *
 *      This module contains the publicly accessible top-level 
 *	IDB write routines.
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
#include <Mrm/IDB.h>


/*
 *
 *  TABLE OF CONTENTS
 *
 *	UrmIdbOpenFileWrite		Open a file for write access
 *
 *	UrmIdbPutIndexedResource		Put indexed resource to file
 *
 *	UrmIdbPutRIDResource		Put resource id'ed resource in file
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



Cardinal UrmIdbOpenFileWrite
        (name, os_ext, creator, creator_version, module, module_version,
            file_id_return, fname_return)
    String			name ;
    MrmOsOpenParamPtr		os_ext ;
    String			creator ;
    String			creator_version ;
    String			module ;
    String			module_version ;
    IDBFile			*file_id_return ;
    char			*fname_return ;

/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	IdbOpenFileWrite creates a new IDB database file, with null index
 *	structures. This call creates the file and initializes the file
 *	header.
 *
 *  FORMAL PARAMETERS:
 *
 *	name			A system-dependent string specifying the IDB
 *				file to be opened.
 *	os_ext			An operating-system specific structure which
 *				supports using specific file system features
 *	creator			Identifies the database creator (application)
 *	creator_version		Creator version
 *	module			Identifies the UIL module
 *	module_version		Module version
 *	file_id_return		returns the IDB file id
 *	fname_return		returns the name of the file actually opened
 *				(via strcpy). Should be at least 256 chars.
 *
 *  IMPLICIT INPUTS:
 *
 *  IMPLICIT OUTPUTS:
 *
 *  FUNCTION VALUE:
 *
 *	MrmSUCCESS	operation succeeded
 *	MrmFAILURE	operation failed, no further reason
 *
 *  SIDE EFFECTS:
 *
 *      1. Acquires memory for the file descriptor
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
unsigned		fileid ;	/* low-level id for file */
IDBFile			filedesc ;	/* new file descriptor */
int			ndx ;		/* loop index */

/*
 * An OS extension block must be present for Unix, and by default
 * specifies that existing files are over-written.
 */

MrmOsOpenParam		osext ;		/* to delete file on open */
osext.version = MrmOsOpenParamVersion ;
osext.nam_flg.clobber_flg = TRUE ;
if ( os_ext == NULL ) os_ext = &osext ;


/*
 * Attempt to open the file. For now, pay no attention to temporary naming
 * issues.
 */
result = Idb__FU_OpenFile
    (name, URMWriteAccess, os_ext, (IDBLowLevelFilePtr *)&fileid, fname_return) ;
if ( result != MrmCREATE_NEW ) return result ;

/*
 * File successfully opened. Acquire a file descriptor and initialize it.
 */
filedesc = (IDBFile) XtMalloc (sizeof(IDBOpenFile)) ;
filedesc->validation = IDBOpenFileValid ;
filedesc->access = URMWriteAccess ;
filedesc->lowlevel_id = fileid ;
filedesc->last_record = 0 ;
filedesc->last_data_record = 0 ;
filedesc->get_count = 0 ;
filedesc->put_count = 0 ;
for ( ndx=IDBrtMin ; ndx<=IDBrtMax ; ndx++ )
    filedesc->rt_counts[ndx] = 0 ;

/*
 * Write a new file header for this file
 */
result = Idb__HDR_InitHeader (filedesc, creator, creator_version,
				module, module_version) ;
if ( result != MrmSUCCESS )
    {
    UrmIdbCloseFile (filedesc, TRUE) ;
    return result ;
    }

/*
 * File successfully opened
 */
*file_id_return = filedesc ;
return MrmSUCCESS ;

}



Cardinal UrmIdbPutIndexedResource (file_id, index, context_id)
    IDBFile			file_id ;
    String			index ;
    URMResourceContextPtr	context_id ;

/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	IdbPutIndexedResource creates a resource record holding the
 *	group, type, and resource data in the resource context, and saves it
 *	under the given index. The group and type must not be null, and the
 *	index must not be currently entered in the database index. The
 *	resource record is marked public or private depending on the value
 *	of the access parameter.
 *
 *  FORMAL PARAMETERS:
 *
 *	file_id		The IDB file id returned by IdbOpenFile
 *	index		case-sensitive index for the new entry, must
 *			not match any existing entry
 *	context_id	URM resource context containing data block
 *			for entry
 *
 *  IMPLICIT INPUTS:
 *
 *  IMPLICIT OUTPUTS:
 *
 *  FUNCTION VALUE:
 *
 *	MrmSUCCESS	operation succeeded
 *	MrmEXISTS	operation failed, index already exists
 *	MrmNUL_GROUP	operation failed, null group parameter
 *	MrmNUL_TYPE	operation failed, null type parameter
 *	MrmFAILURE	operation failed, no further reason
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
IDBRecordBufferPtr	bufptr ;	/* not used */
MrmCount		recno ;		/* not used */
IDBDataHandle		data_entry ;	/* return new data entry */
MrmCode			group ;		/* the entry's group code */


if ( (result=Idb__FIL_Valid(file_id)) != MrmSUCCESS ) return result ;

/*
 * Validity check on group and type, and make sure entry doesn't
 * currently exist.
 */
if ( UrmRCGroup(context_id) == URMgNul ) return MrmNUL_GROUP ;
if ( UrmRCType(context_id) == URMtNul) return MrmNUL_TYPE ;
result = Idb__INX_FindIndex (file_id, index, &bufptr, &recno) ;
if ( result == MrmSUCCESS) return MrmEXISTS ;

/*
 * Create the data entry for this item
 */
result = Idb__DB_PutDataEntry (file_id, context_id, &data_entry) ;
if ( result != MrmSUCCESS ) return result ;

/*
 * Enter the data entry under the index
 */
result = Idb__INX_EnterItem (file_id, index, data_entry) ;
if ( result != MrmSUCCESS ) return result ;
file_id->num_indexed++ ;

group = UrmRCGroup (context_id) ;
if ( group>=URMgMin && group<=URMgMax )
    file_id->group_counts[group]++ ;

/*
 * item successfully entered
 */
return MrmSUCCESS ;

}



Cardinal UrmIdbPutRIDResource (file_id, resource_id, context_id)
    IDBFile			file_id ;
    MrmResource_id		resource_id ;
    URMResourceContextPtr	context_id ;

/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	IdbPutRIDResource creates a resource record holding the group, type,
 *	and resource data, and saves under the given resource id. The group
 *	and type must not be null, and the resource must currently have no
 *	resource record associated with it. Access is always URMaPrivate
 *	regardless of the data context access field.
 *
 *  FORMAL PARAMETERS:
 *
 *	file_id		The IDB file id returned by IdbOpenFile
 *	resource_id	resource id for the new entry, should
 *			currently have no existing resource record
 *	context_id	URM resource context holding data block.
 *
 *  IMPLICIT INPUTS:
 *
 *  IMPLICIT OUTPUTS:
 *
 *  FUNCTION VALUE:
 *
 *	MrmSUCCESS	operation succeeded
 *	MrmEXISTS	operation failed, index already exists
 *	MrmNUL_GROUP	operation failed, null group parameter
 *	MrmNUL_TYPE	operation failed, null type parameter
 *	MrmFAILURE	operation failed, no further reason
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
IDBDataHandle		data_entry ;	/* new data entry */
MrmCode			group ;		/* the entry's group code */


if ( (result=Idb__FIL_Valid(file_id)) != MrmSUCCESS ) return result ;

/*
 * Validity check on group and type, and make sure entry doesn't
 * currently exist.
 */
if ( UrmRCGroup(context_id) == URMgNul ) return MrmNUL_GROUP ;
if ( UrmRCType(context_id) == URMtNul) return MrmNUL_TYPE ;
result = Idb__RID_ReturnItem (file_id, resource_id, FALSE, &data_entry) ;
if ( result == MrmSUCCESS) return MrmEXISTS ;

/*
 * Create the data entry for this item
 */
result = Idb__DB_PutDataEntry (file_id, context_id, &data_entry) ;
if ( result != MrmSUCCESS ) return result ;

/*
 * Enter the data entry under the resource id
 */
result = Idb__RID_EnterItem (file_id, resource_id, data_entry) ;
if ( result != MrmSUCCESS ) return result ;
file_id->num_RID++ ;

group = UrmRCGroup (context_id) ;
if ( group>=URMgMin && group<=URMgMax )
    file_id->group_counts[group]++ ;

/*
 * item successfully entered
 */
return MrmSUCCESS ;

}


