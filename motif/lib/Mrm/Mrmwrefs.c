#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)Mrmwrefs.c	3.6 91/01/10";
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
 *	This module contains all routines which manage references to
 *	widgets which must be handled during or immediately after
 *	widget creation.
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



Cardinal Urm__CW_InitWRef (wref_id)
    URMResourceContextPtr		*wref_id;

/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	routine acquires a resource context, and initializes
 *	it for use a a widget reference structure.
 *
 *  FORMAL PARAMETERS:
 *
 *	wref_id		to return pointer to initialized context
 *
 *  IMPLICIT INPUTS:
 *
 *  IMPLICIT OUTPUTS:
 *
 *  FUNCTION VALUE:
 *
 *      See UrmGetResourceContext
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
URMWRefStructPtr	refdsc;		/* buffer as reference structure */


/*
 * Acquire and initialize resource context
 */
result = UrmGetResourceContext (NULL, NULL, 500, wref_id);
if ( result != MrmSUCCESS ) return result;

refdsc = (URMWRefStructPtr) UrmRCBuffer (*wref_id);
refdsc->num_refs = 0;
refdsc->heap_size = 0;

return MrmSUCCESS;

}



Cardinal Urm__CW_AddWRef (wref_id, w_name, w_id)
    URMResourceContextPtr	wref_id;
    String			w_name;
    Widget			w_id;

/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	This routine adds a widget definition to the widget reference structure
 *
 *  FORMAL PARAMETERS:
 *
 *	wref_id		widget reference structure
 *	w_name		name of the widget to enter
 *	w_id		widget id
 *
 *  IMPLICIT INPUTS:
 *
 *  IMPLICIT OUTPUTS:
 *
 *  FUNCTION VALUE:
 *
 *      See UrmResizeResourceContext
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
URMWRefStructPtr	refdsc;		/* buffer as reference structure */
MrmCount		name_bytes;	/* # bytes for name & NULL */
MrmCount		bytes_needed;	/* # bytes for whole ref */
Cardinal		ndx;		/* entry index in structure */
MrmOffset		new_offs;	/* new offset in heap */
Cardinal		old_size;	/* old buffer size */
Cardinal		new_size;	/* new buffer size */
Cardinal		delta;		/* difference in size */
char			*old_heap;	/* old heap address */
char			*new_heap;	/* new heap address */


/*
 * Set up pointers. Compute size needed and available, and resize structure
 * if required. Name need not be entered if it is compiler-generated
 * (contains non-legal character '-')
 */
refdsc = (URMWRefStructPtr) UrmRCBuffer (wref_id);
name_bytes = strlen(w_name);
for ( ndx=0 ; ndx<name_bytes ; ndx++ )
    if ( w_name[ndx] == '-' ) return MrmFAILURE;
name_bytes += 1;

bytes_needed = sizeof(URMWRef) + name_bytes;
if ( bytes_needed > (UrmRCBufSize(wref_id)-UrmWRefBytesUsed(refdsc)) )
    {
    old_size = UrmRCBufSize (wref_id);
    new_size = 2 * old_size;
    delta = new_size - old_size;
    result = UrmResizeResourceContext (wref_id, new_size);
    if ( result != MrmSUCCESS ) return result;
    refdsc = (URMWRefStructPtr) UrmRCBuffer (wref_id);
    old_heap = (char *) ((int)refdsc+old_size-refdsc->heap_size);
    new_heap = (char *) (old_heap+delta);
    UrmBCopy (old_heap, new_heap, refdsc->heap_size);
    for ( ndx=0 ; ndx<refdsc->num_refs ; ndx++ )
	refdsc->refs[ndx].w_name_offs += delta;
    }

/*
 * There is enough space. Copy in the name, and set up the descriptor.
 */
new_offs = UrmRCBufSize(wref_id) - refdsc->heap_size - name_bytes;
ndx = refdsc->num_refs;
refdsc->refs[ndx].w_id = w_id;
refdsc->refs[ndx].w_name_offs = new_offs;
refdsc->num_refs += 1;
refdsc->heap_size += name_bytes;
strcpy ((String)((int)refdsc+new_offs), w_name);

return MrmSUCCESS;

}



Cardinal Urm__CW_FindWRef (wref_id, w_name, w_id_return)
    URMResourceContextPtr	wref_id;
    String			w_name;
    Widget			*w_id_return;

/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	This routine searches the widget reference structure for a widget,
 *	and returns its id if found.
 *
 *  FORMAL PARAMETERS:
 *
 *	wref_id		widget reference structure
 *	w_name		name of widget to be found
 *	w_id_return	to return widget id
 *
 *  IMPLICIT INPUTS:
 *
 *  IMPLICIT OUTPUTS:
 *
 *  FUNCTION VALUE:
 *
 *      MrmSUCCESS	- found, id is returned
 *	MrmNOT_FOUND	- not found
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
URMWRefStructPtr	refdsc;		/* buffer as reference structure */
int			ndx;		/* loop index */


/*
 * Search the vector for the name, returning the id if found. Search
 * backwards, so the most recent definitions are searched first.
 */
refdsc = (URMWRefStructPtr) UrmRCBuffer (wref_id);
for ( ndx=refdsc->num_refs-1 ; ndx>=0 ; ndx-- )
    if ( strcmp(w_name,UrmWRefNameN(refdsc,ndx)) == 0 )
	{
	*w_id_return = UrmWRefIdN (refdsc, ndx);
	return MrmSUCCESS;
	}
return MrmNOT_FOUND;

}



void Urm__CW_AppendSVWidgetRef 
#ifndef _NO_PROTO
    (IDBFile			file_id,
    URMPointerListPtr		*svlist,
    String			w_name,
    MrmCode			argtag,
    String			argname)
#else
(file_id, svlist, w_name, argtag, argname)
    IDBFile			file_id ;
    URMPointerListPtr		*svlist ;
    String			w_name ;
    MrmCode			argtag ;
    String			argname ;
#endif

/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	This routine creates a SetValues descriptor, initializes it from
 *	the resource descriptor, and appends it to the list. The id of the
 *	widget to be modified is set NULL to identify this descriptor as
 *	one to be set.
 *
 *  FORMAL PARAMETERS:
 *
 *	file_id		file from which to uncompress argtag
 *	svlist		list of SetValues of descriptor
 *	w_name		widget name
 *	argtag		Argument tag code
 *	argname		Argument tag name if code is URMcUnknown
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
URMSetValuesDescPtr	svdesc ;	/* new descriptor */
Cardinal		uncmp_res;	/* function result */
char			errmsg[300];


/*
 * Initialize the pointer list if required
 */
if ( *svlist == NULL )
    UrmPlistInit (10, svlist) ;

/*
 * Create and set the new descriptor. The argument tag must be uncompressed
 * into its string now, when the compression code for it is available from the
 * file. If the tag code is unknown, then the string must be copied into
 * allocated memory.
 */
svdesc = (URMSetValuesDescPtr) XtMalloc (sizeof(URMSetValuesDesc)) ;
svdesc->setw = NULL ;
svdesc->type = URMsvWidgetRef ;
svdesc->tagcode = argtag ;
if ( argtag == UilMrmUnknownCode )
    {
    svdesc->tagname = (String) XtMalloc (strlen(argname)+1);
    strcpy (svdesc->tagname, argname);
    }
else
    {
    uncmp_res = Urm__UncompressCode (file_id, argtag, &svdesc->tagname);
    if ( uncmp_res != MrmSUCCESS )
	{
	sprintf (errmsg, "Couldn't uncompress string code %d", argtag);
	XtFree ((char *)svdesc);
	Urm__UT_Error
	    ("Urm__CW_AppendSVWidgetRef", errmsg, NULL, NULL, MrmNOT_FOUND);
	return;
	}
    }
svdesc->done = FALSE ;
svdesc->sv.wname = Urm__UT_AllocString (w_name) ;
UrmPlistAppendPointer (( URMPointerListPtr)*svlist, (caddr_t) svdesc) ;

}



void Urm__CW_UpdateSVWidgetRef (svlist, cur_name, cur_id)
    URMPointerListPtr		*svlist ;
    String			cur_name ;
    Widget			cur_id ;

/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	This routine updates a SetValues list from a just-created widget.
 *	Any entry whose modified widget field is NULL is has the modified
 *	widget set to the given widget. It also uses this widget to
 *	immediately update any previously saved widget which can be
 *	resolved with this new widget.
 *
 *  FORMAL PARAMETERS:
 *
 *	svlist		List of SetValues descriptors
 *	cur_name	current widget's name
 *	cur_id		current widget to be entered in descriptors
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
int			ndx ;		/* loop index */
URMSetValuesDescPtr	svdesc ;	/* current SetValues descriptor */
Arg			args[1] ;	/* SetValues argument list */


/*
 * Loop over all descriptors. Enter the current widget as appropriate.
 * Since each descriptor is used only once, immediately free any
 * allocated tag string.
 */
for ( ndx=0 ; ndx<UrmPlistNum(*svlist) ; ndx++ )
    {
    svdesc = (URMSetValuesDescPtr) UrmPlistPtrN(*svlist,ndx) ;
    if ( svdesc->type != URMsvWidgetRef ) continue ;
    if ( svdesc->done ) continue ;
    if ( svdesc->setw == NULL )
	{
        svdesc->setw = cur_id ;
	continue ;
	}
    if ( strcmp(cur_name,svdesc->sv.wname) == 0 )
	{
	args[0].name = svdesc->tagname ;
	args[0].value = (XtArgVal) cur_id ;
	XtSetValues (svdesc->setw, args, 1) ;
	svdesc->done = TRUE ;
	if ( svdesc->tagcode == UilMrmUnknownCode )
	    {
	    XtFree ((char *)svdesc->tagname);
	    svdesc->tagname = NULL;
	    }
	}
    }

}

