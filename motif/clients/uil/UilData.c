#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)UilData.c	3.5 90/06/19";
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
**      Global data definitions
**
**--
**/


/*
**
**  INCLUDE FILES
**
**/

#include <Mrm/MrmAppl.h>
#include <Mrm/Mrm.h>



#include "UilDef.h"
#include "UilSymGen.h"

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

/*    Case sensitivity switch; TRUE if case sensitive.    */



externaldef(uil_comp_glbl) boolean	uil_v_case_sensitive = TRUE;

/*    Location to store error type.    */

externaldef(uil_comp_glbl) int		*uil_az_error_env_block = NULL;

/*    Debugging switches.    */

#if debug_version
externaldef(uil_comp_glbl) boolean	uil_v_dump_tokens = FALSE;
externaldef(uil_comp_glbl) boolean	uil_v_dump_symbols = FALSE;
#endif

externaldef(uil_comp_glbl) status	uil_l_compile_status = uil_k_success_status;
externaldef(uil_comp_glbl) Uil_compile_desc_type   *Uil_compile_desc_ptr = NULL;
externaldef(uil_comp_glbl) int		Uil_percent_complete = 0;
externaldef(uil_comp_glbl) int		Uil_lines_processed = 0;
externaldef(uil_comp_glbl) char	*Uil_current_file = "";


/*	Define the user-defined object and default character set */
externaldef(uil_sym_glbl) unsigned short int	uil_sym_user_defined_object =
		sym_k_user_defined_object;
externaldef(uil_sym_glbl) unsigned short int	uil_sym_default_charset =
		sym_k_iso_latin1_charset;
externaldef(uil_sym_glbl) unsigned short int	uil_sym_isolatin1_charset =
		sym_k_iso_latin1_charset;

/*	argument types, allowed tables, constraints, related arguments */
#include "UilSymArTy.h"
#include "UilSymRArg.h"
#include "UilSymArTa.h"
#include "UilSymReas.h"
#include "UilSymCtl.h"
#include "UilConst.h"

/*	sym_k... to name vectors. */
#include "UilSymNam.h"

/*	Define mapping of sym_k_..._value to data type names. */
externaldef(uil_comp_glbl) int uil_max_value = sym_k_max_value;
externaldef(uil_comp_glbl) char *uil_datatype_names [] = {
    "",	/* NOT USED */
    "any",
    "argument",
    "asciz_table",
    "boolean",
    "string",
    "class_rec_name",
    "color",
    "color_table",
    "compound_string",
    "float",
    "font",
    "font_table",
    "icon",
    "identifier",
    "integer",
    "integer_table",
    "keysym",
    "pixmap",
    "reason",
    "rgb",
    "single_float",
    "string_table",
    "translation_table",
    "widget_ref",
    "xbitmapfile",
};

/*	Enumeration Set/Value tables */
#include "UilSymEnum.h"

/*	Character set attribute tables */
#include "UilSymCSet.h"

/*	Mrm encoding information */
#include "UilUrmClas.h"

/*	Names of tokens in grammar */
#include "UilTokName.h"

/*	The keyword tables */
#include "UilKeyTab.h"


/*
**
**  OWN VARIABLE DECLARATIONS
**
**/

