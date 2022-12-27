#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)UilP2Reslv.c	3.5 91/01/10";
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
**++
**  FACILITY:
**
**      User Interface Language Compiler (UIL)
**
**  ABSTRACT:
**
**      This module contain the second pass routines for resolving forward
**	references.
**
**--
**/


/*
**
**  INCLUDE FILES
**
**/

#include "UilDef.h"


/*
**
**  DEFINE and MACRO DEFINITIONS
**
**/



/*
**
**  EXTERNAL VARIABLE DECLARATIONS
**
**/


/*
**
**  GLOBAL VARIABLE DECLARATIONS
**
**/



/*
**
**  OWN VARIABLE DECLARATIONS
**
**/


/*
**++
**  FUNCTIONAL DESCRIPTION:
**
**      This function processes forward references from the first pass.
**
**  FORMAL PARAMETERS:
**
**      none
**
**  IMPLICIT INPUTS:
**
**      sym_az_forward_ref_chain
**	sym_az_val_forward_ref_chain
**
**  IMPLICIT OUTPUTS:
**
**      none
**
**  FUNCTION VALUE:
**
**      void
**
**  SIDE EFFECTS:
**
**      error messages may be issued for objects that are still undefined
**	or of the wrong type
**
**--
**/

void	sem_resolve_forward_refs()

{
    sym_forward_ref_entry_type		* fwd_entry;
    sym_forward_ref_entry_type		* next_fwd_entry;
    sym_val_forward_ref_entry_type	* fwd_val_entry;
    sym_val_forward_ref_entry_type	* next_fwd_val_entry;
    sym_widget_entry_type		** target_obj_entry;
    sym_value_entry_type		** target_val_entry;
    sym_parent_list_type        	* parent_node;
    sym_parent_list_type        	* parent_ptr;
    int                         	found;

    /* 
    **	Forward references are placed on a chain by the first pass of
    **	the compiler.  This routine walks the chain checking that
    **	    1) name is now defined
    **	    2) name points to the correct type of object
    */

    for (fwd_entry = sym_az_forward_ref_chain;  
	 fwd_entry != NULL;  
	 fwd_entry = next_fwd_entry)
    {
	sym_name_entry_type	* name_entry;
	sym_widget_entry_type	* object_entry;
	unsigned short int	object_type;


	/*
	**  Save the pointer to the next forward entry so we can free the current 
	**  entry after it is processed.
	*/
	next_fwd_entry = fwd_entry->az_next_ref;


	/*
	**  Call the Status callback routine to report our progress.
	*/
	/* %COMPLETE */
	Uil_percent_complete = 60;
	if (Uil_cmd_z_command.status_cb != NULL)
	    diag_report_status();    


	object_type = fwd_entry->header.b_type;
	name_entry = fwd_entry->az_name;
	object_entry = (sym_widget_entry_type *) name_entry->az_object;

	if (object_entry == NULL)
	{
	    diag_issue_diagnostic
		( d_never_def,
		  _sar_source_pos2 (fwd_entry),
		  diag_object_text( object_type ),
		  name_entry->c_text );
	    continue;
	}

	/*
	** Make sure object references are to correct type of object.
	** A forward reference naming a widget class may be correctly
	** resolved by the corresponding gadget class, and vice versa.
	*/
	if ((object_entry->header.b_type!=object_type) &&
	    (uil_gadget_variants[object_entry->header.b_type]!=object_type) &&
	    (uil_gadget_variants[object_type]!=object_entry->header.b_type))
	    {
	    diag_issue_diagnostic
		(d_ctx_req,
		 _sar_source_pos2(fwd_entry),
		 diag_object_text(object_type),
		 diag_object_text(object_entry->header.b_type));
	    continue;
	    }

	target_obj_entry =
		(sym_widget_entry_type * *) fwd_entry->a_update_location;
	*target_obj_entry = object_entry;

	/*
	** Update objects on forward refernce chain so that their parent_lists point
	** to the objects which reference them
	*/
	if (fwd_entry -> parent != NULL)
	    {
	    found = FALSE;
	    for (parent_ptr = object_entry -> parent_list;
		((parent_ptr != NULL) && (found == FALSE));
		parent_ptr = parent_ptr -> next)
		{
		if (parent_ptr -> parent == fwd_entry -> parent)
		    found = TRUE;
		}
	    if (found == FALSE)
		{
		parent_node = (sym_parent_list_type *) 
		    sem_allocate_node (sym_k_parent_list_entry, 
		    sym_k_parent_list_size);
		parent_node -> next = object_entry -> parent_list;
		object_entry -> parent_list = parent_node;
		parent_node -> parent = fwd_entry -> parent;
		}
	    }


	/*
	**  Free the Forward reference entry now that it is no longer needed
	*/    
	sem_free_node(( sym_entry_type *)fwd_entry);

    }


    /*
    **  Now resolve the forward references to values
    **/

    /* 
    **	Forward references are placed on a chain by the first pass of
    **	the compiler.  This routine walks the chain checking that
    **	    1) name is now defined
    **	    2) name points to the correct type of value
    */

    for (fwd_val_entry = sym_az_val_forward_ref_chain;  
	 fwd_val_entry != NULL;  
	 fwd_val_entry = next_fwd_val_entry)
    {
	sym_name_entry_type	* name_entry;
	sym_value_entry_type	* value_entry;
	sym_obj_entry_type	* obj_entry;


	/*
	**  Save the pointer to the next forward entry so we can free the current 
	**  entry after it is processed.
	*/
	next_fwd_val_entry = fwd_val_entry->az_next_ref;


	/*
	**  Call the Status callback routine to report our progress.
	*/
	/* %COMPLETE */
	Uil_percent_complete = 60;
	if (Uil_cmd_z_command.status_cb != NULL)
	    diag_report_status();    

	name_entry = fwd_val_entry->az_name;
	value_entry = (sym_value_entry_type *) name_entry->az_object;
	obj_entry = (sym_obj_entry_type *) name_entry->az_object;

	if (value_entry == NULL)
	{
	    diag_issue_diagnostic
		( d_never_def,
		  _sar_source_pos2 (fwd_val_entry),
		  "value",
		  name_entry->c_text );
	    continue;
	}

	switch (fwd_val_entry->fwd_ref_flags)
	    {
	    case (sym_k_patch_add):
	    case (sym_k_patch_list_add):
		{
    	        target_val_entry =
		        (sym_value_entry_type * *) fwd_val_entry->a_update_location;
	        *target_val_entry = value_entry;
	        break;
	        }
	    default:
		_assert(FALSE, "Illegal forward reference");
            }


	/*
	**  Free the Forward reference entry now that it is no longer needed
	*/    
	sem_free_node(( sym_entry_type *)fwd_val_entry);

    }




}
