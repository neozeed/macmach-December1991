#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)MrmIfile.c	3.8 91/01/10";
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
 *      UIL Resource Manager (URM)
 *
 *  ABSTRACT:
 *
 *	This module contains the low-level file utilities
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





#include <stdio.h>		/* Standard IO definitions		*/
#if defined(ibm032) || defined(mac2)
extern int errno;
#endif
#include <errno.h>


/*
 *
 *  DEFINE and MACRO DEFINITIONS
 *
 */

#define	PMODE	0644	/* protection mode: RW for owner, all others R	*/
#define FAILURE	-1	/* creat/stat returns this			*/


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

Cardinal Idb__FU_OpenFile 

#ifndef _NO_PROTO
	(char *name,
	MrmCode access,
	MrmOsOpenParamPtr os_ext,
	IDBLowLevelFilePtr *file_id,
	char *returned_fname)
#else
	(name, access, os_ext, file_id, returned_fname)
    char		*name ;
    MrmCode		access ;
    MrmOsOpenParamPtr	os_ext ;
    IDBLowLevelFilePtr	*file_id ;
    char		*returned_fname ;
#endif

/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	This routine will take the file name specified and
 *	open or create it depending on the access parameter.
 *	An attempt is made to save any existing file of the
 *	same name.
 *
 *
 *  FORMAL PARAMETERS:
 *
 * 	name		the system-dependent file spec of the IDB file
 *			to be opened.
 *	accss		access type desired, read or write access.
 *	os_ext		an operating specific structure to take advantage
 *			of file system features (if any).
 *	file_id		IDB file id used in all calls to low level routines.
 *	returned_fname	The resultant file name.
 *
 *  IMPLICIT INPUTS:
 *
 *      NONE
 *
 *  IMPLICIT OUTPUTS:
 *
 *      NONE
 *
 *  FUNCTION VALUE:
 *
 *	Returns an integer:
 *
 *	MrmSUCCESS	- When access is read and open works
 *	MrmCREATE_NEW	- When access is write and open works
 *	MrmNOT_FOUND	- When access is read and the file isn't present
 *	MrmFAILURE	- When the open fails for any other reason
 *
 *  SIDE EFFECTS:
 *
 *      Opens or creates the named file and assigns a channel to it.
 *
 *--
 */





{
/*
 * External routines
 */

/*
 * Local variables
 */
int		file_desc;		/* 'unix' file descriptor		*/
int		length;			/* the length of the above string	*/
IDBLowLevelFile *a_file;		/* pointer to the file_id		*/

/* Fill in the result name with the name specified so far		    */
length = strlen (name);
strcpy (returned_fname, name);
returned_fname[length] = 0;

/* Check if this file is to be opened for read or write access		    */
if (access == URMWriteAccess)
    {
    file_desc = open (name, O_RDWR, PMODE);
    if (file_desc != FAILURE)		/* there exists a file by this
					   name already			*/
	if (os_ext == 0)
	    return MrmFAILURE;		/* we need to know if we can
					   clobber the existing file	*/
	else if (!os_ext->nam_flg.clobber_flg)
	    return MrmEXISTS;		/* no clobber. return Exists	*/
	else if (os_ext->version != MrmOsOpenParamVersion)
	    return MrmFAILURE;
    close (file_desc);		/* we care not what close returns	*/

    file_desc = creat (name,PMODE);
    if (file_desc == FAILURE)		/* verify that worked		*/
	return MrmFAILURE;

    close (file_desc);		/* we care not what close returns	*/
    file_desc = open (name, O_RDWR, PMODE);

    if (file_desc == FAILURE)		/* verify that worked		*/
	return MrmFAILURE;
    }


/* Else this file is to opened for read access				    */
else if (access == URMReadAccess)
    {
    file_desc = open (name, O_RDONLY, PMODE);

    /* verify that worked						    */
    if (file_desc == FAILURE)
	{
	if ( errno == EACCES )
	    return MrmFAILURE;
	else
	    return MrmNOT_FOUND;
	}
    }

/* Not URMReadAccess or URMWriteAccess, so return invalid access type	    */
else
    return MrmFAILURE;


/*
 * now all we have to do is set up the IDBFile and return the
 * proper success code.
 */

*file_id = (IDBLowLevelFilePtr)
    XtMalloc(sizeof (IDBLowLevelFile));
if (*file_id==0)
    return MrmFAILURE;

a_file = (IDBLowLevelFile *) *file_id;

a_file->name = XtMalloc (length+1);
if (a_file->name==0)
    {
    XtFree ((char*)*file_id);
    return (MrmFAILURE);
    }

a_file->file_desc = file_desc;
strcpy (a_file->name, name);
a_file->name[length] = 0;

if (access == URMWriteAccess)
    return (MrmCREATE_NEW);
else
    return (MrmSUCCESS);

}



Cardinal Idb__FU_CloseFile 
#ifndef _NO_PROTO
    (IDBLowLevelFile	*file_id ,
     int		delete)
#else

(file_id, delete)
    IDBLowLevelFile	*file_id ;
    int			delete ;
#endif
/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	This routine will close the file and free any allocated storage
 *
 *
 *  FORMAL PARAMETERS:
 *
 *	file_id		IDB file id
 *	delete		delete the file if == true
 *
 *  IMPLICIT INPUTS:
 *
 *      the file name and channel from the IDBFile record
 *
 *  IMPLICIT OUTPUTS:
 *
 *      NONE
 *
 *  FUNCTION VALUE:
 *
 *	MrmSUCCESS	- When the file is closed [and deleted] successfully
 *	MrmFAILURE	- When the close fails
 *
 *  SIDE EFFECTS:
 *
 *      Closes the file, deassigns the channel and possible deletes the file.
 *
 *--
 */






{
/*
 * Local variables
 */
int	status;				/* ret status for sys services	*/


status = close (file_id->file_desc);
if (status != 0)
    return MrmFAILURE;

if (delete) {
    status = unlink (file_id->name);
    }

XtFree (file_id->name);
XtFree ((char*)file_id);
return MrmSUCCESS;

}				/* end of routine Idb__FU_CloseFile	*/


Cardinal Idb__FU_GetBlock 

#ifndef _NO_PROTO
    (IDBLowLevelFile	*file_id,
    IDBRecordNumber	block_num,
    char		*buffer)
#else
	(file_id, block_num, buffer)
    IDBLowLevelFile	*file_id ;
    IDBRecordNumber	block_num ;
    char		*buffer ;
#endif

/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	This function reads in the desired record into the given
 *	buffer.
 *
 *  FORMAL PARAMETERS:
 *
 *	file_id		the IDB file identifier
 *	block_num	the record number to retrieve
 *	buffer		pointer to the buffer to fill in
 *
 *  IMPLICIT INPUTS:
 *
 *      NONE
 *
 *  IMPLICIT OUTPUTS:
 *
 *      NONE
 *
 *  FUNCTION VALUE:
 *
 *	MrmSUCCESS	operation succeeded
 *	MrmNOT_FOUND	entry not found
 *	MrmFAILURE	operation failed, no further reason
 *
 *  SIDE EFFECTS:
 *
 *      The buffer is filled in.  Should the $READ fail the buffer's
 *	content is not predictable.
 *
 *--
 */






{
/*
 * Local variables
 */
int	number_read;		/* the number of bytes actually read	*/
int	fdesc ;			/* file descriptor from lowlevel desc */


fdesc = file_id->file_desc ;
lseek (fdesc, (block_num-1)*IDBRecordSize, 0);
number_read = read (file_id->file_desc, buffer, IDBRecordSize);

if (number_read != IDBRecordSize) 
    return MrmFAILURE;
else
    return MrmSUCCESS;
}				/* end of routine Idb__FU_GetBlock	*/



Cardinal Idb__FU_PutBlock 

#ifndef _NO_PROTO
    (IDBLowLevelFile	*file_id,
    IDBRecordNumber	block_num,
    char		*buffer)
#else
        (file_id, block_num, buffer)
    IDBLowLevelFile	*file_id ;
    IDBRecordNumber	block_num ;
    char		*buffer ;
#endif

/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	This function writes the data in the givin buffer into
 *	the desired record in the file.
 *
 *  FORMAL PARAMETERS:
 *
 *	file_id		the IDB file identifier
 *	block_num	the record number to write
 *	buffer		pointer to the buffer to read from
 *
 *  IMPLICIT INPUTS:
 *
 *      NONE
 *
 *  IMPLICIT OUTPUTS:
 *
 *      NONE
 *
 *  FUNCTION VALUE:
 *
 *	Returns an integer by value:
 *
 *	MrmSUCCESS	operation succeeded
 *	MrmFAILURE	operation failed, no further reason
 *
 *  SIDE EFFECTS:
 *
 *	the file is modified.
 *
 *--
 */






{
/*
 * Local variables
 */
int	number_written;		/* the # of bytes acctually written	*/
int	fdesc ;			/* file descriptor from lowlevel desc */


fdesc = file_id->file_desc ;
lseek (fdesc, (block_num-1)*IDBRecordSize, 0);
number_written = write (file_id->file_desc, buffer, IDBRecordSize);

if (number_written != IDBRecordSize)
    return MrmFAILURE;
else
    return MrmSUCCESS;
}				/* end of routine Idb__FU_PutBlock	*/

