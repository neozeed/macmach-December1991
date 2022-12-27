#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)UilSarProc.c	3.6 91/01/10";
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
**      This module contain the routines for processing procedures.
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
**      This function create the procedure definition entry symbol 
**	node a procedure declaration.
**
**  FORMAL PARAMETERS:
**
**	id_frame	ptr to token frame for the procedure name
**	param_frame	ptr to token frame or null frame holding the
**			type of the argument
**	class_frame	ptr to frame whose b_flags holds private, etc. info
**
**  IMPLICIT INPUTS:
**
**	sym_az_current_section_entry	the "current" section list
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
**      errors may be issued for previously defined name
**
**--
**/

void	sar_create_procedure(id_frame, param_frame, class_frame, semi_frame)

const yystype	    *id_frame;
const yystype	    *param_frame;
const yystype	    *class_frame;
const yystype	    *semi_frame;

{
    sym_name_entry_type	    *name_entry;
    sym_proc_def_entry_type *proc_def_entry;
    sym_section_entry_type  *section_entry;

    /*
    **  Call standard routine to check name entry for id_frame.
    **	This routine handles font name, color names, etc used as ids
    */

    name_entry = (sym_name_entry_type *) sem_dcl_name( id_frame );

    if (name_entry == NULL)
	return;

    /*
    **	Allocate the procedure definition entry and fill it in
    */

    proc_def_entry = (sym_proc_def_entry_type *)
	sem_allocate_node (sym_k_proc_def_entry, sym_k_proc_def_entry_size);

    proc_def_entry->obj_header.az_name = (sym_name_entry_type *) name_entry;
    name_entry->az_object = (sym_entry_type *) proc_def_entry;

    /* 
    **	Parameter frame has 3 cases:
    **	    1) no argument checking desired
    **	       syntax: PROCEDURE id
    **	    2) argument checking desired - single argument
    **	       syntax: PROCEDURE id( type )
    **	    3) argument checking desired - no argument
    **	       syntax: PROCEDURE id( )
    **  These cases are distinguished as follows:
    **	    1) tag = null  type = 0
    **	    2) tag = null  type = sar_k_no_value
    **	    2) tag = token type = argument type
    */

    proc_def_entry->v_arg_checking = TRUE;

    switch (param_frame->b_tag)
    {
    case sar_k_null_frame:
	if (param_frame->b_type == sym_k_no_value )
	{
	    proc_def_entry->b_arg_count = 0;
	    proc_def_entry->b_arg_type = sym_k_no_value;
	}
	else
	    proc_def_entry->v_arg_checking = FALSE;
	    
	break;

    case sar_k_token_frame:
	proc_def_entry->b_arg_type = param_frame->b_type;
	proc_def_entry->b_arg_count = 1;
	break;

    default:
	_assert( FALSE, "param frame in error" );

    }

    /*
    **	Process the class clause
    */

    switch (class_frame->b_flags)
    {
    case sym_m_exported:
	sym_make_external_def( name_entry );

    case sym_m_private:
    case sym_m_imported:
	break;

    default:
	_assert( FALSE, "class frame in error" );

    }

    proc_def_entry->obj_header.b_flags = class_frame->b_flags;

    /*
    ** save the source file info for this procedure entry
    */
    _sar_save_source_info (&proc_def_entry->header, id_frame, semi_frame );
    sar_assoc_comment((sym_obj_entry_type *)proc_def_entry);       /* preserve comments */

    /*
    ** allocate a section entry to link the proc_def entry into the structure
    */
    section_entry = (sym_section_entry_type *) sem_allocate_node
			(sym_k_section_entry, sym_k_section_entry_size);

    /*
    ** Link this entry off of the current section list
    */
    section_entry->next = (sym_entry_type *) sym_az_current_section_entry->entries;
    sym_az_current_section_entry->entries = (sym_entry_type *) section_entry;

    section_entry->entries = (sym_entry_type *) proc_def_entry;

}

/*
**++
**  FUNCTIONAL DESCRIPTION:
**
**      This function processes a reference to a procedure.
**
**  FORMAL PARAMETERS:
**
**	id_frame	ptr to token frame for the procedure name
**	value_frame	ptr to token frame or null frame holding the
**			value of the argument to the procedure
**	context		value indicating how the procedure is being used
**
**  IMPLICIT INPUTS:
**
**      none
**
**  IMPLICIT OUTPUTS:
**
**      none
**
**  FUNCTION VALUE:
**
**      a procedure reference entry / NULL in case of an illegal reference
**
**  SIDE EFFECTS:
**
**      errors may be issued 
**
**--
**/

sym_proc_ref_entry_type
	* sem_reference_procedure( id_frame, value_frame, context )

const yystype	    *id_frame;
const yystype	    *value_frame;
const int	    context;

{
    sym_value_entry_type    *value_entry;
    sym_proc_def_entry_type *proc_def_entry;
    sym_proc_ref_entry_type *proc_ref_entry;
    int			    actual_arg_count;
    int			    expected_arg_type;
    int			    actual_arg_type;
    int			    arg_checking;

    /*
    **  Call standard routine to check name entry for id_frame.
    **	This routine handles font name, color names, etc used as ids
    */

    proc_def_entry =
	(sym_proc_def_entry_type *)
	    sem_ref_name( id_frame, sym_k_proc_def_entry );

    switch (value_frame->b_tag)
    {
    case sar_k_null_frame:
	value_entry = NULL;
	break;

    case sar_k_value_frame:
	if ((value_frame->b_flags & sym_m_forward_ref) != 0)
	    value_entry = NULL;
	else
	    value_entry = (sym_value_entry_type *) 
			  value_frame->value.az_symbol_entry;
	break;

    case sar_k_object_frame:
	value_entry =
		(sym_value_entry_type *) value_frame->value.az_symbol_entry;
	break;

    default:
	_assert( FALSE, "actual arg in error" );
    }

    /*
    **	Allocate the procedure reference entry and fill it in
    */

    proc_ref_entry = (sym_proc_ref_entry_type *)
	sem_allocate_node (sym_k_proc_ref_entry, sym_k_proc_ref_entry_size);

    if ((id_frame->b_flags & sym_m_forward_ref) != 0)
        sym_make_value_forward_ref (id_frame, 
	(char*)&(proc_ref_entry->az_proc_def), sym_k_patch_list_add);
    else 
	proc_ref_entry->az_proc_def = proc_def_entry;

    if ((value_frame->b_flags & sym_m_forward_ref) != 0)
        sym_make_value_forward_ref (value_frame, 
	(char*)&(proc_ref_entry->az_arg_value), sym_k_patch_add);
    else
	proc_ref_entry->az_arg_value = value_entry;

    /*
    **	Apply context constraints
    **
    **	If this is a procedure being used as a user object,
    **	it should not have any arguments.  The arguments to such
    **	a procedure are always a parent widget id and an argument list.
    **	This constraint is currently inforced by the grammar.
    **
    **	At this time the compiler permits all types of values for callback
    **  arguments.  This may be limited shortly when we see if it is
    **  reasonable to pass fonts, colors, reasons, etc.
    */

    return proc_ref_entry;

}


/*
**++
**  FUNCTIONAL DESCRIPTION:
**
**      This function checks to see if a object is defined with the name
**	corresponding to the id given in the first parameter.
**
**  FORMAL PARAMETERS:
**
**      id_frame	ptr to a token frame on the parse stack holding the name
**	tag		the type of construct needed
**
**  IMPLICIT INPUTS:
**
**      none
**
**  IMPLICIT OUTPUTS:
**
**      none
**
**  FUNCTION VALUE:
**
**      ptr to a symbol entry for construct or NULL
**
**  SIDE EFFECTS:
**
**	error message if the name is undefined or for a different construct
**
**--
**/

sym_entry_type
    	*sem_ref_name(id_frame, tag)

const yystype	    *id_frame;
const int	    tag;

{
    sym_name_entry_type	    *name_entry;
    sym_entry_type	    *symbol_entry;

    _assert( id_frame->b_tag == sar_k_token_frame, "arg1 not id frame" );

    /* 
    ** The id frame may hold a name or the keyword for a font name, color
    ** name, reason name etc.  If it is one of these special name, then
    ** we must see if the symbol table holds a name for the special type.
    */

    if (id_frame->b_type != NAME)
    {
	name_entry = 
	    sym_find_name
		( id_frame->value.az_keyword_entry->b_length,
		  id_frame->value.az_keyword_entry->at_name );

	if (name_entry == NULL)
	{
	    diag_issue_diagnostic
		( d_undefined,
		  _sar_source_position( id_frame ),
		  diag_tag_text( sym_k_proc_def_entry ),
		  id_frame->value.az_keyword_entry->at_name );

	    return NULL;
	}

    }
    else
	name_entry =
		(sym_name_entry_type *) id_frame->value.az_symbol_entry;

    /*
    ** If the name entry already has no object linked from it, we are
    ** referencing an undefined object.
    */

    symbol_entry = name_entry->az_object;

    if (symbol_entry == NULL )
	{
	id_frame->b_flags |= sym_m_forward_ref;
	return NULL;
	}
    /*
    ** If the name entry has the wrong type of object, this is also
    ** an error.
    */

    if (symbol_entry->header.b_tag != tag )
    {
	diag_issue_diagnostic
	    ( d_ctx_req,
	      _sar_source_position( id_frame ),
	      diag_tag_text( tag ),
	      diag_tag_text( symbol_entry->header.b_tag ) );

	return NULL;
    }

    return symbol_entry;

}
