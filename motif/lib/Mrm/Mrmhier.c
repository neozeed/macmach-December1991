#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)Mrmhier.c	3.17 91/01/10";
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
 *--
 */

/*
 * This file contains routines which assist in managing URM hierarchies
 */


/*
 *
 *  INCLUDE FILES
 *
 */



#include <Mrm/MrmAppl.h>
#include <Mrm/Mrm.h>


#include <pwd.h>

/*
 *
 *  TABLE OF CONTENTS
 *
 *	Urm__OpenHierarchy		Open hierarchy (internal version)
 *
 *	Urm__CloseHierarchy		Close hierarchy (internal version)
 *
 *	UrmHGetIndexedResource		Get resource from hierarchy
 *
 */


/*
 *
 *  DEFINE and MACRO DEFINITIONS
 *
 */

extern char *getenv();
extern int getuid();
static String InitUidPath _ARGUMENTS((String name));
static String GetRootDirName();
static Cardinal I18NOpenFile  _ARGUMENTS(( Display *display, String name , MrmOsOpenParamPtr os_ext , IDBFile *file_id_return ));

/*
 *
 *  EXTERNAL VARIABLE DECLARATIONS
 *
 */

externalref Display	*_XHeadOfDisplayList;

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
static char		*uidPath;



Cardinal Urm__OpenHierarchy
#ifndef _NO_PROTO
    (MrmCount			num_files,
    String			*name_list,
    MrmOsOpenParamPtr		*os_ext_list,
    MrmHierarchy		*hierarchy_id_return)
#else
        (num_files, name_list, os_ext_list, hierarchy_id_return)
    MrmCount			num_files ;
    String			*name_list ;
    MrmOsOpenParamPtr		*os_ext_list ;
    MrmHierarchy		*hierarchy_id_return ;
#endif

/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	This routine allocates a hierarchy descriptor, and opens
 *	all the IDB files in the hierarchy. It initializes the
 *	optimized search lists in the hierarchy from the open files.
 *	All files are closed if there are any errors.
 *
 *  FORMAL PARAMETERS:
 *
 *	num_files		The number of files in the name list
 *	name_list		A list of the file names
 *	os_ext_list		A list of system-dependent ancillary
 *				structures corresponding to the files.
 *				This parameter may be NULL.
 *	hierarchy_id_return	To return the hierarchy id
 *
 *  IMPLICIT INPUTS:
 *
 *  IMPLICIT OUTPUTS:
 *
 *  FUNCTION VALUE:
 *
 *	MrmSUCCESS		operation succeeded
 *	MrmFAILURE		allocation or other failure
 *	MrmDISPLAY_NOT_OPENED	Display hasn't been opened yet
 *	Others			see UrmIdbOpenFileRead
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
Cardinal		result ;	/* function result */
MrmHierarchy		hiptr ;		/* hierarchy descriptor */
MrmCount		list_size ;	/* # bytes for file lists */
IDBFile			*cur_list ;	/* current file list */
int			ndx ;		/* loop index */
IDBFile			*idvec ;	/* current file id vector */
int			file_ndx ;	/* file loop index */
IDBFile			cur_file ;	/* current IDB file */
URMResourceContextPtr	class_ctx;	/* for class compression table */
URMResourceContextPtr	resource_ctx;	/* for resource compression table */
Display			*display;	/* display for XtResolvePathNames */

/*
 * Get the Display from os_ext_list. If that doesn't exist then get the 
 * display from _XHeadOfDisplayList. This is for the LANG variable in 
 * XtResolvePathName. If a valid display is not available then give back an
 * error message.
 */
if ( os_ext_list == NULL )
    {

    display = _XHeadOfDisplayList;
    }
else 
    {
    MrmOsOpenParamPtr os_data;
    os_data = *os_ext_list;
    if (os_data->display == NULL)
	{

        display = _XHeadOfDisplayList;
	}
    else
	{
	display = os_data->display;
	}
    }


if (display == NULL)
    {
    return Urm__UT_Error ("Urm__OpenHierarchy", "Display not yet opened",
			    NULL, NULL, MrmDISPLAY_NOT_OPENED);
    };

/*
 * If the uidPath was previously set, XtFree it so we can try any new paths that
 * may have been setup.
 */
if (uidPath != 0) 
    {
    XtFree (uidPath);
    uidPath = 0;
    }

/*
 * Allocate a hierarchy, and allocate all file lists.
 */
hiptr = (MrmHierarchy) XtMalloc (sizeof(MrmHierarchyDesc)) ;
if ( hiptr == NULL ) return MrmFAILURE ;

hiptr->validation = MrmHIERARCHY_VALID;
hiptr->num_file = 0 ;

list_size = num_files * sizeof(IDBFile) ;
hiptr->file_list = (IDBFile *) XtMalloc (list_size) ;
if ( hiptr->file_list == NULL ) return MrmFAILURE ;

for ( ndx=URMgMin ; ndx<=URMgMax ; ndx++ )
    {
    hiptr->grp_num[ndx] = 0 ;
    idvec = (IDBFile *) XtMalloc (list_size) ;
    if ( idvec == NULL ) return MrmFAILURE ;
    hiptr->grp_ids[ndx] = idvec ;
    }

hiptr->name_registry = NULL;

/*
 * Now open each file. Any failure causes an error return, with any
 * open files closed and the descriptor and lists freed.
 */
for ( file_ndx=0 ; file_ndx<num_files ; file_ndx++ )
    {
    if ( os_ext_list == NULL )
	result = I18NOpenFile
	    (display, name_list[file_ndx], NULL, &cur_file) ;
    else
        result = I18NOpenFile
	    (display, name_list[file_ndx], os_ext_list[file_ndx], &cur_file) ;
    if ( result != MrmSUCCESS )
        {
	XtFree (uidPath);
	uidPath = 0;
        Urm__CloseHierarchy (hiptr) ;
        return result;
        }

    hiptr->file_list[hiptr->num_file] = cur_file ;
    hiptr->num_file++ ;
    for ( ndx=URMgMin ; ndx<=URMgMax ; ndx++ )
        if ( cur_file->group_counts[ndx] > 0 )
            {
            idvec = hiptr->grp_ids[ndx] ;
            idvec[hiptr->grp_num[ndx]] = cur_file ;
            hiptr->grp_num[ndx]++ ;
            }

    /*
     * Attempt to read in compression tables for this UID file.
     * Retain and fixup the tables if they are found.
     */
    cur_file->class_ctable = NULL;
    cur_file->resource_ctable = NULL;
    result = UrmGetResourceContext (NULL, NULL, 0, &class_ctx);
    if ( result != MrmSUCCESS ) return result;
    result = UrmGetResourceContext (NULL, NULL, 0, &resource_ctx);
    if ( result != MrmSUCCESS ) return result;
    result = UrmGetIndexedLiteral
	(cur_file, UilMrmClassTableIndex, class_ctx);
    if ( result != MrmSUCCESS ) continue;
    result = UrmGetIndexedLiteral
	(cur_file, UilMrmResourceTableIndex, resource_ctx);
    if ( result != MrmSUCCESS ) continue;

    /*
     * Retain the buffers from the contexts, but free the contexts
     * themselves. Fixup the tables.
     */
    cur_file->class_ctable =  (UidCompressionTablePtr)
	UrmRCBuffer (class_ctx);
    UrmRCSetBuffer (class_ctx, NULL);
    UrmFreeResourceContext (class_ctx);
    Urm__FixupCompressionTable (cur_file->class_ctable, TRUE);
    cur_file->resource_ctable = (UidCompressionTablePtr)
	UrmRCBuffer (resource_ctx);
    UrmRCSetBuffer (resource_ctx, NULL);
    UrmFreeResourceContext (resource_ctx);
    Urm__FixupCompressionTable (cur_file->resource_ctable, FALSE);
    }

/*
 * successfully opened. Free the uidPath at this point
 */
XtFree (uidPath);
uidPath = 0;
*hierarchy_id_return = hiptr ;
return MrmSUCCESS ;

}



Cardinal Urm__CloseHierarchy (hierarchy_id)
    MrmHierarchy		hierarchy_id ;

/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	This is the internal routine which closes a URM search hierarchy
 *
 *  FORMAL PARAMETERS:
 *
 *	hierarchy_id	ID of an open URM database hierarchy
 *
 *  IMPLICIT INPUTS:
 *
 *  IMPLICIT OUTPUTS:
 *
 *  FUNCTION VALUE:
 *
 *	MrmSUCCESS	operation succeeded
 *	MrmBAD_HIERARCHY	invalid URM hierarchy
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
int			ndx ;		/* loop index */


/*
 * validity check the hierarchy, then close all files, deallocate all
 * lists, and deallocate the descriptor
 */
if ( hierarchy_id == NULL )
    return Urm__UT_Error
        ("Urm__CloseHierarchy", "NULL hierarchy id",
        NULL, NULL, MrmBAD_HIERARCHY) ;
if ( ! MrmHierarchyValid(hierarchy_id) )
    return Urm__UT_Error
        ("Urm__CloseHierarchy", "Invalid hierarchy",
        NULL, NULL, MrmBAD_HIERARCHY) ;

for ( ndx=0 ; ndx<hierarchy_id->num_file ; ndx++ )
    UrmIdbCloseFile (hierarchy_id->file_list[ndx], FALSE) ;

for ( ndx=URMgMin ; ndx<=URMgMax ; ndx++ )
    XtFree ((char*)hierarchy_id->grp_ids[ndx]) ;

hierarchy_id->validation = NULL;
XtFree ((char*)hierarchy_id) ;
return MrmSUCCESS ;

}



Cardinal UrmHGetIndexedResource
#ifndef _NO_PROTO
    (MrmHierarchy		hierarchy_id,
    String			index ,
    MrmGroup			group_filter, 
    MrmType			type_filter,
     URMResourceContextPtr	context_id,
    IDBFile			*file_id_return)
#else
        (hierarchy_id, index, group_filter, type_filter,
            context_id, file_id_return)
    MrmHierarchy		hierarchy_id; 
    String			index; 
    MrmGroup			group_filter; 
    MrmType			type_filter ;
     URMResourceContextPtr	context_id ;
    IDBFile			*file_id_return ;
#endif

/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	UrmHGetIndexedResource attempts to retrieve a resource
 *	from an open URM hierarchy. It functions exactly like
 *	UrmIdbGetIndexedResource except that it queries each file
 *	in the hierarchy in turn. It uses the optimized search lists
 *	where possible.
 *
 *  FORMAL PARAMETERS:
 *
 *	hierarchy_id		open URM hierarchy to search
 *	index			case-sensitive index for the entry to match
 *	group_filter		if not null, entry found must match this group
 *	type_filter		if not null, entry found must match this type
 *	context_id		URM resource context to receieve data block
 *	file_id_return		to return file in which resource was found
 *
 *  IMPLICIT INPUTS:
 *
 *  IMPLICIT OUTPUTS:
 *
 *  FUNCTION VALUE:
 *
 *	MrmSUCCESS	operation succeeded
 *	MrmBAD_HIERARCHY	invalid URM hierarchy
 *	MrmNOT_FOUND	entry not found
 *	MrmWRONG_GROUP	entry didn't match group filter
 *	MrmWRONG_TYPE	entry didn't match type filter
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
IDBFile			*file_ids ;	/* list of files to search */
int			num_ids ;	/* number of entries in file_ids */
int			ndx ;		/* loop index */


/*
 * Validate hierarchy. Then loop through the files which define the resource,
 * searching for the desired resource.
 */
if ( hierarchy_id == NULL )
    return Urm__UT_Error
        ("UrmHGetIndexedResource", "NULL hierarchy id",
        NULL, NULL, MrmBAD_HIERARCHY) ;
if ( ! MrmHierarchyValid(hierarchy_id) )
    return Urm__UT_Error
        ("UrmHGetIndexedResource", "invalid hierarchy",
        NULL, context_id, MrmBAD_HIERARCHY) ;

if ( group_filter>=URMgMin && group_filter<=URMgMax)
    {
    file_ids = hierarchy_id->grp_ids[group_filter] ;
    num_ids = hierarchy_id->grp_num[group_filter] ;
    }
else
    {
    file_ids = hierarchy_id->file_list ;
    num_ids = hierarchy_id->num_file ;
    }
for ( ndx=0 ; ndx<num_ids ; ndx++ )
    {
    *file_id_return = file_ids[ndx] ;
    result = UrmIdbGetIndexedResource
        (*file_id_return, index, group_filter, type_filter, context_id) ;
    if ( result == MrmSUCCESS ) return result ;
    }

/*
 * Not found
 */
return MrmNOT_FOUND ;

}



Cardinal Urm__RegisterNamesInHierarchy 
#ifndef _NO_PROTO
    (MrmHierarchy		hierarchy_id, 
    String			*names,
    caddr_t			*values,
    MrmCount			num_cb)
#else
(hierarchy_id, names, values, num_cb)
    MrmHierarchy		hierarchy_id; 
    String			*names;
    caddr_t			*values;
    MrmCount			num_cb;
#endif

/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	This routine inserts the names and their associated values into
 *	the hierarchy's name registration hash table (which is constructed
 *	if needed). In all respects it functions like Urm__WCI_RegisterNames.
 *
 *  FORMAL PARAMETERS:
 *
 *	hierarchy_id	open URM hierarchy in which to register names.
 *	names		A vector of case-sensitive callback routine names.
 *	values		A vector of the corresponding routine addresses
 *	num_cb		The number of entries in names and values.
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

URMHashTableEntryPtr	*name_table;
Boolean			inited = FALSE;
int			ndx;
URMHashTableEntryPtr	hash_entry;
char			*current_name;
char			*current_value;


/*
 * Make sure the hash table is initialized
 */
name_table = hierarchy_id->name_registry;
if ( name_table == NULL )
    {
    name_table = (URMHashTableEntryPtr *)
	XtMalloc(sizeof(URMHashTableEntryPtr)*k_hash_table_size);
    hierarchy_id->name_registry = name_table;
    hash_initialize (name_table, &inited);
    }

/*
 * Store each name-value pair in the hash table.
 */
for (ndx = 0 ; ndx < num_cb ; ndx++)
    {
    current_name = names [ndx];
    current_value = values [ndx];
    hash_entry = (URMHashTableEntryPtr)
	hash_insert_name (name_table, current_name);
    hash_entry->az_value = current_value;
    }

return MrmSUCCESS;

}



Cardinal Urm__LookupNameInHierarchy (hierarchy_id, name, value_return)
    MrmHierarchy		hierarchy_id; 
    String			name;
    caddr_t			*value_return;

/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	This routine returns the value registered for a name. It first
 *	attempts to look up the name in the hierarchy's name registry.
 *	If that fails, or their is no registry, then a global lookup is
 *	attempted.
 *
 *  FORMAL PARAMETERS:
 *
 *	hierarchy_id	open URM hierarchy to search.
 *	name		case-sensitive name to be matched
 *	value_return	to return value.
 *
 *  IMPLICIT INPUTS:
 *
 *  IMPLICIT OUTPUTS:
 *
 *  FUNCTION VALUE:
 *
 *	MrmSUCCESS	operation succeeded
 *	MrmNOT_FOUND	no match found
 *
 *  SIDE EFFECTS:
 *
 *--
 */

{

URMHashTableEntryPtr	*name_table;
URMHashTableEntryPtr	hash_entry;


/*
 * Look up in hierarchy first (if there is a registry)
 */
name_table = hierarchy_id->name_registry;
if ( name_table != NULL )
    {
    hash_entry = (URMHashTableEntryPtr)	hash_find_name (name_table, name);
    if (hash_entry != NULL)
	{
	*value_return = hash_entry->az_value;
	return MrmSUCCESS;
	}
    }

/*
 * Fall back on global table
 */
return Urm__WCI_LookupRegisteredName (name, value_return);

}



static Cardinal I18NOpenFile (display, name, os_ext, file_id_return)
    Display			*display ;
    String			name ;
    MrmOsOpenParamPtr		os_ext ;
    IDBFile			*file_id_return ;

/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	This routine opens a single UID file in a platform-dependent way,
 *	performing i18n language switching in order to do so.
 *
 *	Per the latest agreement on semantics, this routine does:
 *		- first, try to open in the local directory (that is, with
 *		  no switching).
 *		- second, try language switching and open
 *
 *  FORMAL PARAMETERS:
 *
 *	name		A system-dependent string specifying the IDB file
 *			to be opened.
 *	os_ext		An operating-system specific structure which
 *			supports using specific file system features
 *	file_id_return	returns the IDB file id used in all other IDB routines
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
char			dummy[300];	/* file name (unused) */
char			err_stg[300];






/*
 * Use XtResolvePathName. If the last 4 characters of the file name are not .uid
 * then pass in the suffix of .uid. If a file isn't found with the suffix passed
 * in then try without the suffix.
 */
char			*resolvedname;		/* current resolved name */
SubstitutionRec		subs[1];

subs[0].match = 'U';
subs[0].substitution = name;

if (uidPath == 0)
    {
    uidPath = InitUidPath(name);
    }

resolvedname = 0;

/*
 * Check and see if the .uid suffix is already on the file. If not then try to
 * resolve the pathname with .uid suffix first. If that fails or the suffix is
 * already on the file then just try to resolve the pathname.
 */
if ( strcmp (&name[strlen(name)-4],".uid") != 0 ) 
	resolvedname = XtResolvePathname (display,
				      "uid",
				      NULL,
				      ".uid",
				      uidPath,
				      subs,
				      XtNumber(subs),
				      NULL);

/*
 * No .uid suffix or a failure to resolve the pathname with the .uid suffix
 */
if (resolvedname == 0) 
	resolvedname = XtResolvePathname (display,
					  "uid",
					  NULL,
					  NULL,
					  uidPath,
					  subs,
					  XtNumber(subs),
					  NULL);

if (resolvedname == 0)
    {
    sprintf (err_stg,"Couldn't open file %s", name) ;
    return Urm__UT_Error ("I18NOpenFile", err_stg, NULL, NULL, MrmNOT_FOUND);
    }

result = UrmIdbOpenFileRead (resolvedname, os_ext, file_id_return, dummy) ;
switch ( result )
    {
    case MrmSUCCESS:
	return result;
    case MrmNOT_FOUND:
	sprintf (err_stg, "Couldn't open file %s", resolvedname) ;
	return Urm__UT_Error ("I18NOpenFile", err_stg, NULL, NULL, result);
    case MrmNOT_VALID:
	sprintf (err_stg,"Couldn't open file %s - UID file version mismatch",
		 resolvedname) ;
	return Urm__UT_Error ("I18NOpenFile", err_stg, NULL, NULL, result);
    default:
	sprintf (err_stg,"Couldn't open file %s", resolvedname) ;
	return Urm__UT_Error ("I18NOpenFile", err_stg, NULL, NULL, result);
    }


}



static String GetRootDirName()
{
     int uid;
     struct passwd *pw;
     static char *ptr = NULL;
     char *outptr;

     if (ptr == NULL)
	{
	if((ptr = getenv("HOME")) == NULL) 
	    {
	    if((ptr = getenv("USER")) != NULL) 
		{
		pw = getpwnam(ptr);
		}
	    else 
		{
		uid = getuid();
		pw = getpwuid(uid);
		}
	    if (pw) 
		{
		ptr = pw->pw_dir;
		}
	    else 
		{
		 ptr = NULL;
		}
	    }
	}

     outptr = XtMalloc (strlen(ptr) + 2);
     strcpy (outptr, ptr);
     strcat (outptr, "/");
     return outptr; 
}


/*
 * XAPPLRES_DEFAULT and UIDPATH_DEFAULT are intentionally split to support
 * SCCS. DONOT reformat the lines else %09:37:01%S could be converted by SCCS into
 * something totally bizarre causing MrmOpenHierarchy failures.
 */
static char XAPPLRES_DEFAULT[] = "\
%%U\
%%S:\
%s/%%L/%%T/%%N/%%U\
%%S:\
%s/%%l/%%T/%%N/%%U\
%%S:\
%s/%%T/%%N/%%U\
%%S:\
%s/%%L/%%T/%%U\
%%S:\
%s/%%l/%%T/%%U\
%%S:\
%s/%%T/%%U\
%%S:\
%s%%T/%%U\
%%S:\
%s%%U\
%%S:\
/usr/local/lib/X11/%%L/%%T/%%N/%%U\
%%S:\
/usr/local/lib/X11/%%l/%%T/%%N/%%U\
%%S:\
/usr/local/lib/X11/%%T/%%N/%%U\
%%S:\
/usr/local/lib/X11/%%L/%%T/%%U\
%%S:\
/usr/local/lib/X11/%%l/%%T/%%U\
%%S:\
/usr/local/lib/X11/%%T/%%U\
%%S:\
/usr/local/include/X11/%%T/%%U\
%%S";

static char UIDPATH_DEFAULT[] = "\
%%U\
%%S:\
%s%%L/%%T/%%N/%%U\
%%S:\
%s%%l/%%T/%%N/%%U\
%%S:\
%s%%T/%%N/%%U\
%%S:\
%s%%L/%%T/%%U\
%%S:\
%s%%l/%%T/%%U\
%%S:\
%s%%T/%%U\
%%S:\
%s%%U\
%%S:\
/usr/local/lib/X11/%%L/%%T/%%N/%%U\
%%S:\
/usr/local/lib/X11/%%l/%%T/%%N/%%U\
%%S:\
/usr/local/lib/X11/%%T/%%N/%%U\
%%S:\
/usr/local/lib/X11/%%L/%%T/%%U\
%%S:\
/usr/local/lib/X11/%%l/%%T/%%U\
%%S:\
/usr/local/lib/X11/%%T/%%U\
%%S:\
/usr/local/include/X11/%%T/%%U\
%%S";

static char ABSOLUTE_PATH[] = "\
%U\
%S";

static String InitUidPath(filename)
    String	filename ;
{
    String path;
    String old_path;
    String homedir;
    String uid_path;

    if (filename[0] == '/')
	{
	uid_path = XtMalloc(strlen(ABSOLUTE_PATH) + 1);
	strcpy (uid_path, ABSOLUTE_PATH);
	}
    else
	{
	path = (char *)getenv ("UIDPATH");
	if (path  == NULL) 
	    {
	    homedir = GetRootDirName();
	    old_path = (char *)getenv ("XAPPLRESDIR");
	    if (old_path == NULL) 
		{
		uid_path = XtCalloc(1, 7*strlen(homedir) + strlen(UIDPATH_DEFAULT));
		sprintf( uid_path, UIDPATH_DEFAULT, homedir, homedir, homedir,
			 homedir, homedir, homedir, homedir);
		} 
	    else 
		{
		uid_path = XtCalloc(1, 6*strlen(old_path) + 2*strlen(homedir) +
				 strlen(XAPPLRES_DEFAULT));
		sprintf(uid_path, XAPPLRES_DEFAULT, 
			old_path, old_path, old_path, old_path, old_path, old_path,
			homedir, homedir);
		}
	    XtFree (homedir);
	    }
	else
	    {
	    uid_path = XtMalloc(strlen(path) + 1);
	    strcpy (uid_path, path);
	    }
	}
    return (uid_path);
}
