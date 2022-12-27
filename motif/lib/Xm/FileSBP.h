#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)FileSBP.h	3.11 91/01/10";
#endif /* lint */
#endif /* REV_INFO */
/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
*  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
*  (c) Copyright 1987, 1988, 1989, 1990, HEWLETT-PACKARD COMPANY
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
#ifndef _XmFSelectP_h
#define _XmFSelectP_h

#include <Xm/SelectioBP.h>
#include <Xm/FileSB.h>


/* Defines for use in allocation geometry matrix. */

#define FSB_MAX_WIDGETS_VERT    9
#define FSB_MAX_NUM_WIDGETS     16

/* Bit locations for the state_flags bit field.
*/
#define FS_NO_MATCH		(1 << 0)
#define FS_IN_FILE_SEARCH	(1 << 1)

/*  New fields for the FileSelectionBox widget class record  */

typedef struct
{
    caddr_t             extension;      /* Pointer to extension record */
} XmFileSelectionBoxClassPart;


/* Full class record declaration */

typedef struct _XmFileSelectionBoxClassRec
{
   CoreClassPart       core_class;
   CompositeClassPart  composite_class;
   ConstraintClassPart constraint_class;
   XmManagerClassPart  manager_class;
   XmBulletinBoardClassPart    bulletin_board_class;
   XmSelectionBoxClassPart    selection_box_class;
   XmFileSelectionBoxClassPart    file_selection_box_class;
} XmFileSelectionBoxClassRec;

externalref XmFileSelectionBoxClassRec xmFileSelectionBoxClassRec;


typedef void (*XmQualProc)(
#ifndef _NO_PROTO
XmFileSelectionBoxWidget fs,
XmFileSelectionBoxCallbackStruct * searchData,
XmFileSelectionBoxCallbackStruct * qualifiedSearchData
#endif
);
typedef void (*XmDirProc)(
#ifndef _NO_PROTO
XmFileSelectionBoxWidget fs,
XmFileSelectionBoxCallbackStruct * searchData
#endif
);
typedef void (*XmFileProc)(
#ifndef _NO_PROTO
XmFileSelectionBoxWidget fs,
XmFileSelectionBoxCallbackStruct * searchData
#endif
);

/* New fields for the FileSelectionBox widget record */

typedef struct
{
    XmString        directory;        /* directory specification */
    XmString        pattern;          /* file search pattern */
    Widget          dir_list_label;   /* directory list Label */
    XmString        dir_list_label_string;/* directory list label text */
    Widget          dir_list;         /* directory List */
    XmString *      dir_list_items;   /* items in directory List */
    int             dir_list_item_count;/* number of items in directory List */
    int             dir_list_selected_item_position;
    Widget          filter_label;     /* file search filter label */
    XmString        filter_label_string;/* filter label text */
    Widget          filter_text;      /* filter text entry field */
    XmString        dir_mask;         /* string in filter text entry field */
    XmString        no_match_string;  /* string in list when no file match */
    XmQualProc    qualify_search_data_proc; /* directory and mask routine */
    XmDirProc       dir_search_proc;  /* change directory routine */
    XmFileProc      file_search_proc; /* file search routine */
    unsigned char   file_type_mask;   /* mask for type of files in file list */
    Boolean         list_updated;     /* flag to indicate file list update   */
    Boolean         directory_valid ; /* flag to indicate valid new directory*/
    unsigned char   state_flags ;     /* internal flags to indicate state.   */

} XmFileSelectionBoxPart;


/****************************************************************
 *
 * Full instance record declaration
 *
 ****************************************************************/

typedef struct _XmFileSelectionBoxRec
{
    CorePart	            core;
    CompositePart           composite;
    ConstraintPart          constraint;
    XmManagerPart           manager;
    XmBulletinBoardPart     bulletin_board;
    XmSelectionBoxPart      selection_box;
    XmFileSelectionBoxPart  file_selection_box;
} XmFileSelectionBoxRec;


/* Access macros */

#define FS_Directory( w) \
                (((XmFileSelectionBoxWidget)(w))->file_selection_box.directory)
#define FS_DirMask( w) \
                 (((XmFileSelectionBoxWidget)(w))->file_selection_box.dir_mask)
#define FS_DirListLabel( w) \
           (((XmFileSelectionBoxWidget)(w))->file_selection_box.dir_list_label)
#define FS_DirListLabelString( w) \
    (((XmFileSelectionBoxWidget)(w))->file_selection_box.dir_list_label_string)
#define FS_DirList( w) \
                 (((XmFileSelectionBoxWidget)(w))->file_selection_box.dir_list)
#define FS_DirListItems( w) \
           (((XmFileSelectionBoxWidget)(w))->file_selection_box.dir_list_items)
#define FS_DirListItemCount( w) \
      (((XmFileSelectionBoxWidget)(w))->file_selection_box.dir_list_item_count)
#define FS_FilterLabel( w) \
             (((XmFileSelectionBoxWidget)(w))->file_selection_box.filter_label)
#define FS_FilterLabelString( w) \
      (((XmFileSelectionBoxWidget)(w))->file_selection_box.filter_label_string)
#define FS_FilterText( w) \
              (((XmFileSelectionBoxWidget)(w))->file_selection_box.filter_text)
#define FS_Pattern( w) \
                  (((XmFileSelectionBoxWidget)(w))->file_selection_box.pattern)
#define FS_NoMatchString( w) \
          (((XmFileSelectionBoxWidget)(w))->file_selection_box.no_match_string)
#define FS_QualifySearchDataProc( w) (((XmFileSelectionBoxWidget) \
                             (w))->file_selection_box.qualify_search_data_proc)
#define FS_DirSearchProc( w) \
          (((XmFileSelectionBoxWidget)(w))->file_selection_box.dir_search_proc)
#define FS_FileSearchProc( w) \
         (((XmFileSelectionBoxWidget)(w))->file_selection_box.file_search_proc)
#define FS_RealDefaultButton( w) \
      (((XmFileSelectionBoxWidget)(w))->file_selection_box.real_default_button)
#define FS_FileTypeMask( w) \
           (((XmFileSelectionBoxWidget)(w))->file_selection_box.file_type_mask)
#define FS_ListUpdated( w) \
             (((XmFileSelectionBoxWidget)(w))->file_selection_box.list_updated)
#define FS_DirectoryValid( w) \
          (((XmFileSelectionBoxWidget)(w))->file_selection_box.directory_valid)
#define FS_StateFlags( w) \
              (((XmFileSelectionBoxWidget)(w))->file_selection_box.state_flags)
#define FS_DirListSelectedItemPosition( w) (((XmFileSelectionBoxWidget) w) \
                          ->file_selection_box.dir_list_selected_item_position)

#ifdef _NO_PROTO

extern void _XmFileSelectionBoxCreateFilterLabel() ;
extern void _XmFileSelectionBoxCreateDirListLabel() ;
extern void _XmFileSelectionBoxCreateDirList() ;
extern void _XmFileSelectionBoxCreateFilterText() ;
extern XmGeoMatrix _XmFileSBGeoMatrixCreate() ;
extern Boolean _XmFileSelectionBoxNoGeoRequest() ;
extern void _XmFileSelectionBoxGetDirectory() ;
extern void _XmFileSelectionBoxGetNoMatchString() ;
extern void _XmFileSelectionBoxGetPattern() ;
extern void _XmFileSelectionBoxGetFilterLabelString() ;
extern void _XmFileSelectionBoxGetDirListLabelString() ;
extern void _XmFileSelectionBoxGetDirListItemCount() ;
extern void _XmFileSelectionBoxGetDirListItems() ;
extern void _XmFileSelectionBoxGetListItemCount() ;
extern void _XmFileSelectionBoxGetListItems() ;
extern void _XmFileSelectionBoxGetDirMask() ;
extern void _XmFileSelectionBoxUpOrDown() ;
extern void _XmFileSelectionBoxRestore() ;
extern void _XmFileSelectionBoxFocusMoved() ;

#else /* _NO_PROTO */

extern void _XmFileSelectionBoxCreateFilterLabel(
                                                XmFileSelectionBoxWidget fsb) ;
extern void _XmFileSelectionBoxCreateDirListLabel( 
                                                XmFileSelectionBoxWidget fsb) ;
extern void _XmFileSelectionBoxCreateDirList( XmFileSelectionBoxWidget fsb) ;
extern void _XmFileSelectionBoxCreateFilterText( XmFileSelectionBoxWidget fs) ;
extern XmGeoMatrix _XmFileSBGeoMatrixCreate( XmFileSelectionBoxWidget fsb,
                                Widget instigator, XtWidgetGeometry *desired) ;
extern Boolean _XmFileSelectionBoxNoGeoRequest( XmGeoMatrix geoSpec) ;
extern void _XmFileSelectionBoxGetDirectory(
             XmFileSelectionBoxWidget fs, XrmQuark resource, XtArgVal *value) ;
extern void _XmFileSelectionBoxGetNoMatchString(
             XmFileSelectionBoxWidget fs, XrmQuark resource, XtArgVal *value) ;
extern void _XmFileSelectionBoxGetPattern(
             XmFileSelectionBoxWidget fs, XrmQuark resource, XtArgVal *value) ;
extern void _XmFileSelectionBoxGetFilterLabelString(
             XmFileSelectionBoxWidget fs, XrmQuark resource, XtArgVal *value) ;
extern void _XmFileSelectionBoxGetDirListLabelString( 
             XmFileSelectionBoxWidget fs, XrmQuark resource, XtArgVal *value) ;
extern void _XmFileSelectionBoxGetDirListItemCount(
             XmFileSelectionBoxWidget fs, XrmQuark resource, XtArgVal *value) ;
extern void _XmFileSelectionBoxGetDirListItems( XmFileSelectionBoxWidget fs,
                                          XrmQuark resource, XtArgVal *value) ;
extern void _XmFileSelectionBoxGetListItemCount(
             XmFileSelectionBoxWidget fs, XrmQuark resource, XtArgVal *value) ;
extern void _XmFileSelectionBoxGetListItems( XmFileSelectionBoxWidget fs,
                                          XrmQuark resource, XtArgVal *value) ;
extern void _XmFileSelectionBoxGetDirMask( XmFileSelectionBoxWidget fs,
				          XrmQuark resource, XtArgVal *value) ;
extern void _XmFileSelectionBoxUpOrDown( XmFileSelectionBoxWidget fsb,
                                        XEvent *event, char **argv, int argc) ;
extern void _XmFileSelectionBoxRestore( XmFileSelectionBoxWidget fsb,
                                        XEvent *event, char **argv, int argc) ;
extern void _XmFileSelectionBoxFocusMoved( Widget wid, XtPointer client_data,
                                       XmFocusMovedCallbackStruct *call_data) ;
#endif /* _NO_PROTO */

#endif /* _XmFSelectP_h */
/* DON'T ADD ANYTHING AFTER THIS #endif */
