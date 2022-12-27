#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)Mrmwvalues.c	3.8 91/01/10";
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
 *	This module contains the routines which implement modifying
 *	a widget (XtSetValues) from UID literals.
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
 *	UrmFetchSetValues	Do XtSetValues from UID literals.
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



Cardinal UrmFetchSetValues (hierarchy_id, w, args, num_args)
    MrmHierarchy		hierarchy_id ;
    Widget			w ;
    ArgList			args ;
    Cardinal			num_args ;

/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	UrmFetchSetValues is the internal routine which will
 *	modify a widget via XtSetValues on an argument list whose values
 *	are evaluated literals from a URM hierarchy. 
 *
 *	The args list has ordinary Strings as each tag name. Each value
 *	is interpreted as a String whose value is the index of a literal
 *	to be fetched and evaluated as the actual value. This routine
 *	constructs its own arglist, copying each tag, and setting the
 *	value to the fetched, fixed up, and converted literal value. The
 *	argument is not set in this list (is dropped) if there are any
 *	errors.
 *
 *	This code must also handle pixmaps in the same way that URM does;
 *	it defers converting icons and inserting them in the list
 *	until all the rest of the arglist is done; this allows FG/BG
 *	values to be uncovered in the list for use in the pixmap.
 *
 *  FORMAL PARAMETERS:
 *
 *	hierarchy_id	URM hierarchy from which to read public resources
 *	w		widget to be modified
 *	args		arglist, with literal indexes as values
 *	num_args	# args in args
 *
 *  IMPLICIT INPUTS:
 *
 *  IMPLICIT OUTPUTS:
 *
 *  FUNCTION VALUE:
 *
 *	MrmSUCCESS		operation succeeded
 *	MrmPARTIAL_SUCCESS	at least one fetch suboperation succeeded
 *	other			any error fatal to the operation
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
 * Local structures
 */
typedef struct {
	RGMIconImagePtr	icon ;		/* icon to be converted */
	int		pixndx ;	/* argument in arglist (args) */
} _SavePixmapArg, *_SavePixmapArgPtr ;

/*
 *  Local variables
 */
Cardinal		result ;	/* function results */
int			num_succ = 0;	/* # of successful fetches */
int			fet_fail = 0;	/* # of failed fetches */
int			cvt_fail = 0;	/* # failed conversions */
Cardinal		badfet_res = MrmNOT_FOUND;
					/* to save NOT_FOUND */
ArgList			locargs ;	/* local arglist */
Cardinal		num_used = 0 ;	/* # args used in arglist */
RGMResourceDescPtr	resptr ;	/* descriptor for literals */
int			indexlen = 0 ;	/* longest literal length */
URMPointerListPtr	ptrlist ;	/* to hold scratch contexts */
int			ndx ;		/* loop index */
IDBFile			file_id ;	/* file from which literal read */
MrmCode			tag_code ;	/* compressed tag code */
MrmType			reptype ;	/* arg value representation type */
int			val ;		/* value as immediate or pointer */
int			vec_count ;	/* number of items in val if vector */
_SavePixmapArg		pixargs[10] ;	/* to save pixmap args */
Cardinal		pixargs_cnt = 0 ;
					/* # pixargs saved */
_SavePixmapArgPtr	savepix ;	/* current saved pixmap entry */
Screen			*screen ;	/* screen for pixmaps */
Display			*display ;	/* display for pixmaps */
int			fgint = -1 ;	/* foreground for pixmaps. -1 means
					   not set */
int			bgint = -1 ;	/* background for pixmaps */
MrmCode			cvt_code ;	/* ASCIZ conversion code */
RGMIconImagePtr		icon ;		/* value as icon image */
Pixel			pixmap ;	/* pixmap created from icon */
char			err_msg[300] ;


/*
 * Create local arglist and pointer list for contexts.
 */
locargs = (ArgList) XtMalloc (num_args*sizeof(Arg)) ;
UrmPlistInit (num_args, &ptrlist) ;

/*
 * Find the longest literal index, and allocate a resource descriptor
 * which can hold it. Set the fixed fields of the descriptor.
 */
for ( ndx=0 ; ndx<num_args ; ndx++ )
    indexlen = MAX (indexlen, strlen((char*)args[ndx].value)) ;

/*
 * Create a resource descriptor which can be reused to fetch each literal
 */
resptr = (RGMResourceDescPtr) XtMalloc (sizeof(RGMResourceDesc)+indexlen) ;
resptr->access = URMaPublic ;
resptr->type = URMrIndex ;
resptr->res_group = URMgLiteral ;
resptr->cvt_type = URMtNul ;

/*
 * Loop through all the entries in the given arglist. Copy each tag into
 * the local arglist. Create a new context to hold the literal, then
 * try to read it from the hierarchy.
 */
for ( ndx=0 ; ndx<num_args ; ndx++ )
    {
    locargs[num_used].name = args[ndx].name ;
    strcpy (resptr->key.index, (char*)args[ndx].value) ;
    result = Urm__CW_ReadLiteral (resptr, hierarchy_id, NULL,
        ptrlist, &reptype, &val, &vec_count, &file_id) ;
    if ( result == MrmSUCCESS )
	num_succ += 1;
    else
	{
	fet_fail += 1;
	if ( result != MrmNOT_FOUND )
	    badfet_res = result;
	continue;
	}

/*
 * Fix up and perform conversion on the value. If this succeeds, put it
 * in the local arglist. Special casing is required for some types,
 * particularly icons.
 *
 * Type conversion for some ASCIZ strings is also provided as a V1
 * compatibility feature.
 */
    switch ( reptype )
        {
        case MrmRtypeIconImage:
            savepix = &pixargs[pixargs_cnt] ;
            savepix->icon = (RGMIconImagePtr) val ;
            savepix->pixndx = ndx ;
            pixargs_cnt += 1 ;
            continue ;
            break ;
        default:
            result = Urm__CW_FixupValue (val, reptype, (caddr_t)val) ;
            if ( result != MrmSUCCESS )
		{
		num_succ -= 1;
		cvt_fail += 1;
		continue;
		}
	    if (XtIsWidget(w))
		display = XtDisplay(w);
	    else
		display = XtDisplay(XtParent(w));
	    
            result= Urm__CW_ConvertValue 
			(&val, reptype, NULL, display, hierarchy_id,  NULL) ;
            if ( result != MrmSUCCESS )
		{
		num_succ -= 1;
		cvt_fail += 1;
		continue;
		}
            locargs[num_used].value = (XtArgVal)val ;
            num_used += 1 ;
            break ;
        }

/*
 * Do any processing based on specific arguments (after conversion):
 *	- retain FG/BG info
 */
    switch ( reptype )
	{
	case MrmRtypeColor:

	    if ( strcmp(args[ndx].name,XmNforeground) == 0 )
		fgint = val;

	    else if ( strcmp (args[ndx].name,XmNbackground) == 0 )
		bgint = val;
	    break ;
        }

    }	/* end loop */

/*
 * Now set any pixmap arguments. This requires finding the display, screen,
 * foreground, and background values for this widget. These values are
 * available from the parent widget and the arglist.
 */
if ( pixargs_cnt > 0 )
    {
    Urm__CW_GetPixmapParms (w, &screen, &display, &fgint, &bgint) ;
    for ( ndx=0,savepix=pixargs ; ndx<pixargs_cnt ; ndx++,savepix++ )
        {
        result = UrmCreatePixmap (savepix->icon, screen, display,
            (Pixel)fgint, (Pixel)bgint, &pixmap) ;
        if ( result != MrmSUCCESS )
	    {
	    num_succ -= 1;
	    cvt_fail += 1;
	    continue;
	    }
	locargs[num_used].name = args[savepix->pixndx].name ;
        locargs[num_used].value = (XtArgVal) pixmap ;
        num_used += 1 ;
        }
    }

/*
 * arglist creation complete. Modify the widget if we have succeeded in
 * fetching any values. Deallocate resources.
 */
if ( num_used > 0 )
    XtSetValues (w, locargs, num_used) ;

XtFree ((char*)locargs) ;
XtFree ((char*)resptr) ;
for ( ndx=0 ; ndx<UrmPlistNum(ptrlist) ; ndx++ )
    UrmFreeResourceContext ((URMResourceContextPtr)UrmPlistPtrN(ptrlist,ndx)) ;
UrmPlistFree (ptrlist) ;

/*
 * Result depends on number of successes vs. number of failurs
 */
if ( num_succ > 0 )
    {
    if ( fet_fail==0 && cvt_fail == 0 )
	return MrmSUCCESS;
    else
	return MrmPARTIAL_SUCCESS;
    }
if ( fet_fail > 0 )
    return badfet_res;
else
    return MrmFAILURE;

}

