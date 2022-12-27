#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)UilMessTab.h	3.7 90/06/06";
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



char const msg0[36] = "duplicate option \"%s\" was ignored";
char const msg1[34] = "unknown option \"%s\" was ignored";
char const msg2[43] = "additional UIL source file: %s was ignored";
char const msg3[30] = "error opening source file: %s";
char const msg4[43] = "error reading next line of source file: %s";
char const msg5[19] = "Internal error: %s";
char const msg6[32] = "line truncated at %d characters";
char const msg7[31] = "value of %s is out of range %s";
char const msg8[21] = "%s not terminated %s";
char const msg9[37] = "unprintable character \\%d\\ ignored";
char const msg10[32] = "unknown sequence \"%s\" ignored";
char const msg11[46] = "unknown escape sequence \"\\%c\" - \\ ignored";
char const msg12[46] = "name exceeds 31 characters - truncated to: %s";
char const msg13[35] = "compiler ran out of virtual memory";
char const msg14[56] = "unexpected %s token seen - parsing resumes after \"%c\"";
char const msg15[44] = "%s %s must be defined before this reference";
char const msg16[41] = "context requires a %s - %s was specified";
char const msg17[26] = "%s is not implemented yet";
char const msg18[39] = "found %s value when expecting %s value";
char const msg19[45] = "the %s %s is not supported for the %s object";
char const msg20[58] = "this %s %s supersedes a previous definition in this %s %s";
char const msg21[33] = "name %s previously defined as %s";
char const msg22[43] = "value used in this context must be private";
char const msg23[55] = "procedure %s was previously declared with %d arguments";
char const msg24[56] = "found %s value - procedure %s argument must be %s value";
char const msg25[33] = "found %s %s when expecting %s %s";
char const msg26[24] = "%s %s was never defined";
char const msg27[39] = "%s %s already specified for this %s %s";
char const msg28[29] = "%s item not allowed in %s %s";
char const msg29[45] = "compilation terminated - fix previous errors";
char const msg30[38] = "internal error - submit defect report";
char const msg31[2] = " ";
char const msg32[35] = "%s missing following \"%s\" option";
char const msg33[31] = "error opening listing file: %s";
char const msg34[34] = "error writing to listing file: %s";
char const msg35[51] = "invalid module structure - check UIL module syntax";
char const msg36[31] = "too many source files open: %s";
char const msg37[38] = "source line contains a null character";
char const msg38[45] = "errors: %d  warnings: %d  informationals: %d";
char const msg39[27] = "error opening UID file: %s";
char const msg40[25] = "no UID file was produced";
char const msg41[53] = "creation procedure is not supported by the %s widget";
char const msg42[59] = "creation procedure is not allowed in a %s widget reference";
char const msg43[58] = "creation procedure is required in a %s widget declaration";
char const msg44[46] = "a NULL character in a string is not supported";
char const msg45[43] = "widget %s is part of a circular definition";
char const msg46[25] = "no source file specified";
char const msg47[35] = "%s %s supports only a single %s %s";
char const msg48[41] = "%s widget supports only a single control";
char const msg49[22] = "unknown character set";
char const msg50[47] = "place names clause before other module clauses";
char const msg51[47] = "color letter string must be a single character";
char const msg52[48] = "color letter used for prior color in this table";
char const msg53[37] = "row %d must have same width as row 1";
char const msg54[52] = "row %d, column %d: letter \"%c\" not in color table";
char const msg55[32] = "too many %ss in %s, limit is %d";
char const msg56[48] = "Subqualifier not allowed with negated qualifier";
char const msg57[60] = "%s gadget is not supported - %s widget will be used instead";
char const msg58[28] = "%s type is not valid for %s";
char const msg59[66] = "support for this character set may be removed in a future release";
char const msg60[49] = "the %s constraint is not supported for the %s %s";
char const msg61[37] = "too many \"%s\" options, limit is %d";
char const msg62[30] = "error closing source file: %s";
char const msg63[35] = "the %s value is circularly defined";
char const msg64[28] = "overriding built-in name %s";
char const msg65[51] = "the %s argument does not support enumerated values";
char const msg66[57] = "the %s argument does not support the %s enumerated value";
char const msg67[40] = "$LANG contains an unknown character set";
char const msg68[66] = "the %s object's controls hierarchy contains a reference to itself";
char const msg69[41] = "value %s is too large for context buffer";
char const msg70[42] = "forward referencing is not allowed for %s";
char const msg71[34] = "cannot convert %s type to %s type";
char const msg72[14] = "%s is invalid";


struct
{
    int  l_severity;
    char *ac_text;
}       const diag_rz_msg_table[73] =
        {
	   { 2, msg0 },
	   { 2, msg1 },
	   { 3, msg2 },
	   { 4, msg3 },
	   { 4, msg4 },
	   { 4, msg5 },
	   { 3, msg6 },
	   { 3, msg7 },
	   { 3, msg8 },
	   { 3, msg9 },
	   { 3, msg10 },
	   { 3, msg11 },
	   { 3, msg12 },
	   { 4, msg13 },
	   { 3, msg14 },
	   { 3, msg15 },
	   { 3, msg16 },
	   { 3, msg17 },
	   { 3, msg18 },
	   { 2, msg19 },
	   { 1, msg20 },
	   { 3, msg21 },
	   { 3, msg22 },
	   { 3, msg23 },
	   { 3, msg24 },
	   { 3, msg25 },
	   { 3, msg26 },
	   { 3, msg27 },
	   { 3, msg28 },
	   { 4, msg29 },
	   { 4, msg30 },
	   { 1, msg31 },
	   { 3, msg32 },
	   { 4, msg33 },
	   { 4, msg34 },
	   { 3, msg35 },
	   { 4, msg36 },
	   { 3, msg37 },
	   { 1, msg38 },
	   { 4, msg39 },
	   { 1, msg40 },
	   { 3, msg41 },
	   { 3, msg42 },
	   { 3, msg43 },
	   { 2, msg44 },
	   { 3, msg45 },
	   { 4, msg46 },
	   { 2, msg47 },
	   { 2, msg48 },
	   { 3, msg49 },
	   { 3, msg50 },
	   { 3, msg51 },
	   { 3, msg52 },
	   { 3, msg53 },
	   { 3, msg54 },
	   { 3, msg55 },
	   { 4, msg56 },
	   { 2, msg57 },
	   { 3, msg58 },
	   { 2, msg59 },
	   { 2, msg60 },
	   { 2, msg61 },
	   { 2, msg62 },
	   { 3, msg63 },
	   { 2, msg64 },
	   { 2, msg65 },
	   { 2, msg66 },
	   { 3, msg67 },
	   { 3, msg68 },
	   { 4, msg69 },
	   { 4, msg70 },
	   { 3, msg71 },
	   { 3, msg72 },
        };
