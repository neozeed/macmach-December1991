#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)UilSarExp.c	3.7.1.1 91/02/21";
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
**      This module supports value expressions in UIL.  
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
**      This function processes a binary arithmetic operator in an expression
**
**  FORMAL PARAMETERS:
**
**      operator_frame: [in-out]    frame holding the operator and the
**				    target frame
**	op1_frame: [in]		    frame holding operand 1
**	op2_frame: [in]		    frame holding operand 2
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
**      void
**
**  SIDE EFFECTS:
**
**      op1 or op2 symbol entry may be freed
**
**--
**/
void	sar_binary_op(operator_frame, op1_frame, op2_frame)

yystype	    *operator_frame;
yystype	    *op1_frame;
yystype	    *op2_frame;

{
    sym_value_entry_type    *res_entry;
    int	    		    operator;

    _assert( operator_frame->b_tag == sar_k_token_frame,
	     "operator missing" );

	res_entry = (sym_value_entry_type *)
			sem_allocate_node( sym_k_value_entry,
				       sym_k_value_entry_size );

	res_entry->obj_header.b_flags = sym_m_builtin | sym_m_private;
	res_entry->b_type = sym_k_any_value;

/*	res_entry->az_source_rec = op1_frame->az_source_record; */

	_sar_save_source_pos (&res_entry->header, op1_frame);

    /*
    **	Determine the operator from the operator frame
    */

    switch (operator_frame->b_type)
    {
    case AND:
	    operator = sym_k_and_op;
	break;

    case PLUS:
	operator = sym_k_add_op;
	break;

    case MINUS:
	operator = sym_k_subtract_op;
	break;

    case MULTIPLY:
	operator = sym_k_multiply_op;
	break;

    case DIVIDE:
	operator = sym_k_divide_op;
	break;

    case LEFT_SHIFT:
	operator = sym_k_left_shift_op;
	break;

    case RIGHT_SHIFT:
	operator = sym_k_right_shift_op;
	break;

    case OR:
	operator = sym_k_or_op;
	break;

    case XOR:
	operator = sym_k_or_op;
	break;

    default:
	_assert( FALSE, "unknown binary operator" );
    }

    res_entry->b_expr_opr = operator;

    /*
    ** If the value is a forward reference, we'll patch in the
    ** address of the the referenced value between passes.  Otherwise,
    ** just point to the referenced value node.
    */
    if ((op1_frame->b_flags & sym_m_forward_ref) != 0)
	sym_make_value_forward_ref (op1_frame, 
	(char*)&(res_entry->az_exp_op1), sym_k_patch_add);
    else    
	res_entry->az_exp_op1 =
	    (sym_value_entry_type *) op1_frame->value.az_symbol_entry;

    if ((op2_frame->b_flags & sym_m_forward_ref) != 0)
	sym_make_value_forward_ref (op2_frame, 
	(char*)&(res_entry->az_exp_op2), sym_k_patch_add);
    else    
	res_entry->az_exp_op2 =
	    (sym_value_entry_type *) op2_frame->value.az_symbol_entry;

    operator_frame->b_tag = sar_k_value_frame;
    operator_frame->b_type = res_entry->b_type;
    operator_frame->b_flags = res_entry->obj_header.b_flags;
    operator_frame->value.az_symbol_entry =
		(sym_entry_type *) res_entry;

    return;
}


/*
**++
**  FUNCTIONAL DESCRIPTION:
**
**      This function processes a unary arithmetic operator in an expression
**
**  FORMAL PARAMETERS:
**
**      operator_frame: [in-out]    frame holding the operator and the
**				    target frame
**	op1_frame: [in]		    frame holding operand 1
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
**      void
**
**  SIDE EFFECTS:
**
**      op1 symbol entry may be freed
**
**--
**/
void	sar_unary_op(operator_frame, op1_frame)

yystype	    *operator_frame;
yystype	    *op1_frame;

{
    sym_value_entry_type    *res_entry;
    int			    operator;
    int			    res_type;


    _assert( operator_frame->b_tag == sar_k_token_frame,
	     "operator missing" );

    /*
    **	Determine the operator from the operator frame
    */

    switch (operator_frame->b_type)
    {
    case PLUS:
	operator = sym_k_unary_plus_op;
	break;

    case MINUS:
	operator = sym_k_unary_minus_op;
	break;

    case NOT:
	operator = sym_k_not_op;
	break;

    case INTEGER:
	res_type = sym_k_integer_value;
	operator = sym_k_coerce_op;
        break;
    case FLOAT:
	res_type = sym_k_float_value;
	operator = sym_k_coerce_op;
        break;
    case SINGLE_FLOAT:
	res_type = sym_k_single_float_value;
	operator = sym_k_coerce_op;
        break;
    case KEYSYM:
	res_type = sym_k_keysym_value;
	operator = sym_k_coerce_op;
        break;


    default:
	_assert( FALSE, "unknown unary operator" );
    }

    /*
    **	Create the result
    */

    res_entry = (sym_value_entry_type *)
	sem_allocate_node (sym_k_value_entry, sym_k_value_entry_size);
    
    res_entry->b_expr_opr = operator;
    res_entry->az_exp_op1 =
	(sym_value_entry_type *) op1_frame->value.az_symbol_entry;
    res_entry->obj_header.b_flags = sym_m_builtin | sym_m_private;
    if (operator == sym_k_coerce_op)
	res_entry->b_type = res_type;
    
    _sar_save_source_pos (&res_entry->header, op1_frame );

    operator_frame->b_tag = sar_k_value_frame;
    operator_frame->b_type = res_entry->b_type;
    operator_frame->b_flags = res_entry->obj_header.b_flags;
    operator_frame->value.az_symbol_entry = (sym_entry_type *) res_entry;
    
    return;

}

