#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)UilCompGl.h	3.2 90/06/19";
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
**++
**  FACILITY:
**
**      User Interface Language Compiler (UIL)
**
**  ABSTRACT:
**
**      This include file contains external declarations of all Uil
**	compiler globals excluding those which define the recognized
**	language which are declared in UilSymGl.h
**
**--
**/

#ifndef UilCompGl_h
#define UilCompGl_h

/*
 * Defined in UilCmd.c
 */
externalref cmd_command_line_type		Uil_cmd_z_command;

/*
 * Defined in UilData
 */
externalref boolean				uil_v_case_sensitive;
externalref int					*uil_az_error_env_block;
#if debug_version
externalref boolean				uil_v_dump_tokens;
externalref boolean				uil_v_dump_symbols;
#endif
externalref status				uil_l_compile_status;
externalref Uil_compile_desc_type		*Uil_compile_desc_ptr;
externalref int					Uil_percent_complete;
externalref int					Uil_lines_processed;
externalref char				*Uil_current_file;

/*
 * Defined in UilDiags
 */
externalref int					Uil_message_count[];

/*
 * Defined in UilLexAna
 */
externalref int					Uil_lex_l_user_default_charset;
externalref sym_value_entry_type		*Uil_lex_az_charset_entry;
externalref int					Uil_lex_l_charset_specified;
externalref int					Uil_lex_l_literal_charset;
externalref sym_value_entry_type		*Uil_lex_az_literal_charset;
externalref char				*comment_text;
externalref int					comment_size;
/* % Complete */
externalref int                                Uil_characters_read;

/*
 * Defined in UilLstLst
 */
externalref char				Uil_lst_c_title2[];

/*
 * Defined in UilP2Out
 */
externalref IDBFile				out_az_idbfile_id;

/*
 * Defined in UilSarMod
 */
externalref src_source_record_type		*src_az_module_source_record;
externalref unsigned short int			*uil_urm_variant;
externalref unsigned short int			*uil_arg_compr;
externalref unsigned short int			*uil_reas_compr;
externalref unsigned short int			*uil_widget_compr;

/*
 * Defined in UilSrcSrc
 */
externalref src_source_buffer_type		*src_az_current_source_buffer;
externalref src_source_buffer_type		*src_az_avail_source_buffer;
externalref src_message_item_type		*src_az_orphan_messages;
externalref src_source_record_type		*src_az_current_source_record;
externalref src_source_record_type		*src_az_first_source_record;
externalref uil_fcb_type			*src_az_source_file_table[];
externalref int	    				src_l_last_source_file_number;
/* %COMPLETE */
externalref long                                Uil_file_size;

/*
 * Defined in UilSymStor
 */
externalref sym_name_entry_type			*sym_az_hash_table[];
externalref sym_value_entry_type		*sym_az_error_value_entry;
externalref sym_external_def_entry_type		*sym_az_external_def_chain;
externalref sym_forward_ref_entry_type		*sym_az_forward_ref_chain;
externalref sym_val_forward_ref_entry_type	*sym_az_val_forward_ref_chain;
externalref sym_module_entry_type		*sym_az_module_entry;
externalref sym_root_entry_type			*sym_az_root_entry;
externalref sym_section_entry_type		*sym_az_current_section_entry;
externalref sym_entry_type			*sym_az_entry_list_header;
externalref URMPointerListPtr			sym_az_allocated_nodes;
externalref URMPointerListPtr			sym_az_freed_nodes;

#endif /* UilCompGl_h */
/* DON'T ADD STUFF AFTER THIS #endif */
