#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)MrmPublic.h	3.4 90/08/07";
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

#ifndef MrmPublic
#define MrmPublic

#include <X11/Intrinsic.h>

/*
 * Success or other non-error return codes
 */
#define	MrmSUCCESS	1
#define MrmCREATE_NEW	3
#define	MrmINDEX_RETRY	5	/* Retry on entering index required */
#define	MrmINDEX_GT	7	/* Index orders greater-than entry */
#define	MrmINDEX_LT	9	/* Index orders less-than entry */
#define MrmPARTIAL_SUCCESS	11	/* operation partly succeeded */

/*
 * Failure return codes
 */
#define	MrmFAILURE	0
#define	MrmNOT_FOUND	2
#define	MrmEXISTS	4
#define	MrmNUL_GROUP	6
#define	MrmNUL_TYPE	8
#define	MrmWRONG_GROUP	10
#define	MrmWRONG_TYPE	12
#define	MrmOUT_OF_RANGE	14	/* Record number too big */
#define	MrmBAD_RECORD	16	/* Record number wrong type */
#define	MrmNULL_DATA	18	/* No data for entry */
#define	MrmBAD_DATA_INDEX	20	/* Data index in RID out of range */
#define	MrmBAD_ORDER	22	/* Bad ordering specifier */
#define	MrmBAD_CONTEXT	24	/* Invalid Mrm context */
#define	MrmNOT_VALID	26	/* Validation failure */
#define	MrmBAD_BTREE	28	/* GT/LT pointer error in BTree */
#define	MrmBAD_WIDGET_REC	30	/* Validation failure on widget record */
#define	MrmBAD_CLASS_TYPE	32	/* Class type not a valid Mrmwc... value */
#define	MrmNO_CLASS_NAME	34	/* User class name is null */
#define	MrmTOO_MANY	36	/* Too many entries requested in some list */
#define	MrmBAD_IF_MODULE	38	/* invalid interface module */
#define	MrmNULL_DESC	40	/* Arglist or children descriptor null */
#define	MrmOUT_OF_BOUNDS	42	/* Argument index out of arglist bounds */
#define	MrmBAD_COMPRESS	44	/* Invalid compression code */
#define	MrmBAD_ARG_TYPE	46	/* Invalid type, not in RGMrType... */
#define	MrmNOT_IMP	48	/* Not yet implemented */
#define	MrmNULL_INDEX	50	/* empty index string */
#define	MrmBAD_KEY_TYPE	52	/* key must be MrmrIndex or MrmrRID */
#define	MrmBAD_CALLBACK	54	/* Invalid callback descriptor */
#define	MrmNULL_ROUTINE	56	/* Empty callback routine name string */
#define	MrmVEC_TOO_BIG	58	/* too many elements in vector */
#define	MrmBAD_HIERARCHY	60	/* invalid Mrm file hierarchy */
#define	MrmBAD_CLASS_CODE	62	/* Class code not found in Mrmwc... */
#define MrmDISPLAY_NOT_OPENED	63	/* Display not yet created */
 

/*
 * Definitions associated with widget representations
 */

/*
 * Define the creation callback
 */
#define MrmNcreateCallback	"createCallback"


/*
 * Code for unknown (user-defined) classes.
 */
#define	MrmwcUnknown		1


/*
 * The data types of values stored in uid files.
 */
#define	MrmRtypeMin		1
#define	MrmRtypeInteger		1	/* int */
#define	MrmRtypeBoolean		2
#define	MrmRtypeChar8		3	/* a nul-terminated string */
#define	MrmRtypeChar8Vector	4	/* a vector of char_8 strings */
#define	MrmRtypeCString		5	/* a compound string (DDIS) */
#define	MrmRtypeCStringVector	6	/* a vector of compound strings */
#define	MrmRtypeFloat		7
					/* 8 = TypeCompressed now unused */
#define	MrmRtypeCallback	9	/* code for a callback descriptor */
#define	MrmRtypePixmapImage	10	/* Pixmap in image form */
#define	MrmRtypePixmapDDIF	11	/* Pixmap in DDIF form */
#define	MrmRtypeResource	12	/* Mrm resource descriptor */
#define	MrmRtypeNull		13	/* no value given */
#define	MrmRtypeAddrName	14	/* nul-terminated string to be
					   interpreted as runtime address */
#define	MrmRtypeIconImage	15	/* icon image */
#define	MrmRtypeFont		16	/* Mrm font structure */
#define	MrmRtypeFontList	17	/* Mrm font list */
#define	MrmRtypeColor		18	/* Mrm color descriptor */
#define	MrmRtypeColorTable	19	/* Mrm color table */
#define	MrmRtypeAny		20	/* Any is allowed in UID file */
#define MrmRtypeTransTable	21	/* Translation table (ASCIZ string) */
#define MrmRtypeClassRecName	22	/* class record name (ASCIZ string) */
#define MrmRtypeIntegerVector	23	/* a vector of integers */
#define MrmRtypeXBitmapFile	24	/* X bitmap file to make pixmap with */
#define MrmRtypeCountedVector	25	/* vector with associated count */
#define MrmRtypeKeysym		26	/* X keysym data type */
#define MrmRtypeSingleFloat	27	/* single float data type */
#define	MrmRtypeMax		27



/*
 * MRM typedefs
 */

/*
 * MRM primitive types
 */
typedef	short int	MrmCode ;	/* Used for codes, e.g. Mrmcr... */
typedef	char		MrmSCode ;	/* Short code for small ranges */
typedef	unsigned short int	MrmOffset ;  /* Used for offsets in records */
typedef	short int	MrmType ;	/* Used for types, e.g. MrmrType... */
typedef	unsigned short int	MrmSize ;	/* For size fields */
typedef	short int	MrmCount ;	/* For counter fields */
typedef	unsigned char	MrmFlag ;	/* flag fields */
typedef	long int	MrmResource_id ;	/* Resource id in IDB files */
typedef	short int	MrmGroup ;	/* For Mrm resource groups */

#define	MrmMaxResourceSize	65535	/* (2)16 - 1 */ 


/*
 * Operating System specific parameter struct.  Passed to low level
 * file OPEN.
 */
#define MrmOsOpenParamVersion 1
typedef struct	{
/*
 * The version of this structure. Set to MrmOsOpenParamVersion
 */
	Cardinal		version; 
/*
 * The default name, for systems which can use it
 */
	char			*default_fname;	
/*
 * The related name, for systems which can use it
 * Flag for file clobber (1 means OK to clobber)
 */
	union {
	    unsigned long	related_nam; 
	    Boolean		clobber_flg; 
	} nam_flg;

/*
 * The display, for which the user wants us to open
 * this hierarchy on. Used for the LANG variable.
 */
	Display			*display;

} MrmOsOpenParam, *MrmOsOpenParamPtr ;

/*
 * The opaque result of opening a Mrm hierarchy.
 */
typedef struct MrmHierarchyDescStruct *MrmHierarchy;



/*
 * Structure used to pass name/value pairs to MrmRegisterNames
 */
typedef struct {
	String		name ;		/* case-sensitive name */
	caddr_t		value ;		/* value/address associated with name */
} MRMRegisterArg, MrmRegisterArg, *MrmRegisterArglist ;

/*
 * Code for unknown (user-defined) classes. 
 */
#define	URMwcUnknown	1


/*
 * Declarations of Mrm public entry points
 */
#include <Mrm/MrmDecls.h>

#endif /* MrmPublic */
/* DON'T ADD STUFF AFTER THIS #endif */
