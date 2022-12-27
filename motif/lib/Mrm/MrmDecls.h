#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)MrmDecls.h	3.9 91/01/10";
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
#ifndef MrmDecls_H
#define MrmDecls_H

/*----------------------------------*/
/* URM external routines (Motif)    */
/*----------------------------------*/
#ifndef _ARGUMENTS
#ifdef _NO_PROTO
#define _ARGUMENTS(arglist) ()
#else
#define _ARGUMENTS(arglist) arglist
#endif
#endif

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

/* mrminit.c */
extern void MrmInitialize  _ARGUMENTS(( void ));

/* mrmlread.c */
extern Cardinal MrmFetchLiteral  _ARGUMENTS(( MrmHierarchy hierarchy_id , String index , Display *display , caddr_t *value_return , MrmCode *type_return ));
extern Cardinal MrmFetchIconLiteral  _ARGUMENTS(( MrmHierarchy hierarchy_id , String index , Screen *screen , Display *display , Pixel fgpix , Pixel bgpix , Pixmap *pixmap_return ));
extern Cardinal MrmFetchColorLiteral  _ARGUMENTS(( MrmHierarchy hierarchy_id , String index , Display *display , Colormap cmap , Pixel *pixel_return ));

/* Deal with Wide stuff now because there is an error in Saber 3.0 */

#if NeedWidePrototypes

extern Cardinal MrmOpenHierarchy  _ARGUMENTS(( int num_files , String *name_list , MrmOsOpenParamPtr *os_ext_list , MrmHierarchy *hierarchy_id_return ));
extern Cardinal MrmRegisterNames  _ARGUMENTS(( MrmRegisterArglist reglist ,int num_reg ));
extern Cardinal MrmRegisterNamesInHierarchy  _ARGUMENTS(( MrmHierarchy hierarchy_id , MrmRegisterArglist reglist , int num_reg ));
extern Cardinal MrmRegisterClass  _ARGUMENTS(( int class_code , String class_name , String create_name , Widget (*creator )(), WidgetClass class_record ));

#else

extern Cardinal MrmOpenHierarchy  _ARGUMENTS(( MrmCount num_files , String *name_list , MrmOsOpenParamPtr *os_ext_list , MrmHierarchy *hierarchy_id_return ));
extern Cardinal MrmRegisterNames  _ARGUMENTS(( MrmRegisterArglist reglist ,MrmCount num_reg ));
extern Cardinal MrmRegisterNamesInHierarchy  _ARGUMENTS(( MrmHierarchy hierarchy_id , MrmRegisterArglist reglist , MrmCount num_reg ));
extern Cardinal MrmRegisterClass  _ARGUMENTS(( MrmType class_code , String class_name , String create_name , Widget (*creator )(), WidgetClass class_record ));

#endif 

extern Cardinal MrmCloseHierarchy  _ARGUMENTS(( MrmHierarchy hierarchy_id ));
extern Cardinal MrmFetchInterfaceModule  _ARGUMENTS(( MrmHierarchy hierarchy_id , char *module_name , Widget parent , Widget *w_return ));
extern Cardinal MrmFetchWidget  _ARGUMENTS(( MrmHierarchy hierarchy_id , String index , Widget parent , Widget *w_return , MrmType *class_return ));
extern Cardinal MrmFetchWidgetOverride  _ARGUMENTS(( MrmHierarchy hierarchy_id , String index , Widget parent , String ov_name , ArgList ov_args , Cardinal ov_num_args , Widget *w_return , MrmType *class_return ));
extern Cardinal MrmFetchSetValues  _ARGUMENTS(( MrmHierarchy hierarchy_id , Widget w , ArgList args , Cardinal num_args ));

/* mrmwci.c */

/* extern Cardinal XmRegisterMrmCallbacks () ; */

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#undef _ARGUMENTS

#endif /* MrmDecls_H */
/* DON'T ADD STUFF AFTER THIS #endif */
