#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)FileSB.c	3.32 91/01/10";
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

#include <Xm/FileSBP.h>

#include <Xm/List.h>
#include <Xm/LabelG.h>
#include <Xm/Text.h>
#include <Xm/DialogS.h>

#include <stdio.h>
#include <sys/types.h>

#ifdef SYS_DIR
#include <sys/dir.h>
#else
#ifdef NDIR
#include <ndir.h>
#else
#include <dirent.h>
#endif
#endif

#include <sys/stat.h>
#ifndef MCCABE
#include <pwd.h>
#endif

#ifdef USE_GETWD
#include <sys/param.h>
#define MAX_DIR_PATH_LEN    MAXPATHLEN
#else
#define MAX_DIR_PATH_LEN    1024
#endif
#define MAX_USER_NAME_LEN   256

#ifndef S_ISDIR
#define S_ISDIR(m) ((m & S_IFMT)==S_IFDIR)
#endif

#ifndef S_ISREG
#define S_ISREG(m) ((m & S_IFMT)==S_IFREG)
#endif

#define	ARG_LIST_CNT	(25)
#define	MAXFILES	500
#define MAXFILELENGTH	256
#define FILE_LIST_BLOCK 64
 
typedef struct
    {   XmFileSelectionBoxWidget fsb ;
        XmFileSelectionBoxCallbackStruct change_data ;
        } FS_DoSearchCallbackRec ;

typedef struct
    {   XmKidGeometry dir_list_label ;
        XmKidGeometry file_list_label ;
        Dimension   prefer_width ;
        Dimension   delta_width ;
        } FS_GeoExtensionRec, *FS_GeoExtension ;

/*---------------------------------------------------*/
/* forward declarations                              */
/*                                                   */
/* this is a list of all private procedures in this  */
/* module                                            */
/*---------------------------------------------------*/
 
#ifdef _NO_PROTO

static void             ClassPartInitialize() ;
static void	        Initialize() ;
static void             Destroy() ;
static void             DeleteChild() ;
static void             ListLabelFix() ;
static void             ListFix() ;
static String           GetQualifiedDir() ;
static char *           FindPatternPart() ;
static Boolean          QualifyFileSpec() ;
static char *           GetFixedMatchPattern() ;
static void             GetDirEntries() ;
static int              fileCompare() ;
static void             QualifySearchDataProc();
static void             FileSelectionBoxUpdate();
static void             DirSearchProc();
static void             FileSearchProc();
static void             ListCallback() ;
static void             DoSearchFromSetValues() ;
static Boolean          SetValues() ;
static void             BuildFileList() ;
static Widget           GetActiveText() ;
static void             FileSelectionPB() ;

#else /* _NO_PROTO */

static void             ClassPartInitialize( 
                                           XmFileSelectionBoxWidgetClass fsc) ;
static void	        Initialize( XmFileSelectionBoxWidget request,
                                                XmFileSelectionBoxWidget new) ;
static void             Destroy( XmFileSelectionBoxWidget fsb) ;
static void             DeleteChild( Widget w) ;
static void             ListLabelFix( XmGeoMatrix geoSpec, int action,
                              XmGeoRowLayout layoutPtr, XmKidGeometry rowPtr) ;
static void             ListFix( XmGeoMatrix geoSpec, int action,
                              XmGeoRowLayout layoutPtr, XmKidGeometry rowPtr) ;
static String           GetQualifiedDir( String dirSpec) ;
static char *           FindPatternPart( char * fileSpec) ;
static Boolean          QualifyFileSpec( String dirSpec, String filterSpec, 
                          String * pQualifiedDir, String * pQualifiedPattern) ;
static char *           GetFixedMatchPattern( char * pattern) ;
static void             GetDirEntries( String qualifiedDir, 
                             String matchPattern, unsigned char fileType,
                               Boolean matchDotsLiterally,
                                 Boolean listWithFullPath, String * * pEntries,
                                   unsigned int *  pNumEntries, 
                                     unsigned int *  pNumAlloc) ;
static int              fileCompare( String * sp1, String * sp2) ;
static void             QualifySearchDataProc( XmFileSelectionBoxWidget fs,
                                 XmFileSelectionBoxCallbackStruct * searchData,
                                             XmFileSelectionBoxCallbackStruct *
                                                         qualifiedSearchData) ;
static void             FileSelectionBoxUpdate( XmFileSelectionBoxWidget fs,
                               XmFileSelectionBoxCallbackStruct * searchData) ;
static void             DirSearchProc( XmFileSelectionBoxWidget fs,
                               XmFileSelectionBoxCallbackStruct * searchData) ;
static void             FileSearchProc( XmFileSelectionBoxWidget fs,
                               XmFileSelectionBoxCallbackStruct * searchData) ;
static void             ListCallback( Widget wid, XtPointer client_data,
                                                         XtPointer call_data) ;
static void             DoSearchFromSetValues( FS_DoSearchCallbackRec * data,
                                                           XtIntervalId * id) ;
static Boolean          SetValues( XmFileSelectionBoxWidget current,
                                              XmFileSelectionBoxWidget request,
                                                XmFileSelectionBoxWidget new) ;
static void             BuildFileList( String dirPath, String pattern,
                                   unsigned char typeMask, String * * pEntries,
                                                   unsigned int *  pNumEntries,
                                                   unsigned int *  pNumAlloc) ;
static Widget           GetActiveText( XmFileSelectionBoxWidget fsb,
                                                    XKeyPressedEvent * event) ;
static void             FileSelectionPB( Widget wid, XtPointer which_button,
                                              XmAnyCallbackStruct * callback) ;

#endif /* _NO_PROTO */

/*
 * transfer vector from translation manager action names to
 * address of routines 
 */
 
static XtActionsRec ActionsTable[] =
{
    { "UpOrDown", (XtActionProc) _XmFileSelectionBoxUpOrDown }, /* Motif 1.0 */
    { "SelectionBoxUpOrDown", (XtActionProc)  _XmFileSelectionBoxUpOrDown },
    { "SelectionBoxRestore",  (XtActionProc)  _XmFileSelectionBoxRestore },
    };
 
/*---------------------------------------------------*/
/* widget resources                                  */
/*---------------------------------------------------*/
static XtResource resources[] = 
{
    /* fileselection specific resources */
 
	{	XmNdirectory,
		XmCDirectory,
		XmRXmString,
		sizeof( XmString),
		XtOffset( XmFileSelectionBoxWidget, 
                                                 file_selection_box.directory),
		XmRXmString,
		(XtPointer) NULL    /* This will initialize to the current   */
	},                          /*   directory, because of XmNdirMask.   */
	{	XmNpattern,
		XmCPattern,
		XmRXmString,
		sizeof( XmString), 
		XtOffset( XmFileSelectionBoxWidget,
                                                   file_selection_box.pattern),
                XmRImmediate,
                (XtPointer) NULL  /* This really initializes to "*", because */
	},                        /*   of interaction with "XmNdirMask".     */
	{	XmNdirListLabelString, 
		XmCDirListLabelString, 
		XmRXmString, 
		sizeof (XmString), 
		XtOffset(XmFileSelectionBoxWidget,
                                     file_selection_box.dir_list_label_string),
		XmRString,
		"Directories"
	},
        {       XmNdirListItems,
                XmCDirListItems,
                XmRXmStringTable,
                sizeof( XmStringTable),
		XtOffset(XmFileSelectionBoxWidget,
                                            file_selection_box.dir_list_items),
                XmRImmediate,
                (XtPointer) NULL
        },
        {       XmNdirListItemCount,
                XmCDirListItemCount,
                XmRInt,
                sizeof( int),
		XtOffset(XmFileSelectionBoxWidget,
                                       file_selection_box.dir_list_item_count),
                XmRImmediate,
                (XtPointer) UNSPECIFIED
        },
	{	XmNfilterLabelString, 
		XmCFilterLabelString, 
		XmRXmString, 
		sizeof (XmString), 
		XtOffset( XmFileSelectionBoxWidget,
                                       file_selection_box.filter_label_string),
		XmRString,
		"Filter"
	},
	{	XmNdirMask, 
		XmCDirMask, 
		XmRXmString, 
		sizeof (XmString), 
		XtOffset( XmFileSelectionBoxWidget,
                                                  file_selection_box.dir_mask),
		XmRString,
		"*"
	},
	{	XmNnoMatchString, 
		XmCNoMatchString, 
		XmRXmString, 
		sizeof (XmString), 
		XtOffset( XmFileSelectionBoxWidget,
                                           file_selection_box.no_match_string),
		XmRString,
		(XtPointer) " [    ] "
	},
	{	XmNqualifySearchDataProc,
		XmCQualifySearchDataProc,
		XmRProc, 
		sizeof(XtProc),
		XtOffset( XmFileSelectionBoxWidget,
                                  file_selection_box.qualify_search_data_proc),
		XmRImmediate,
		(XtPointer) QualifySearchDataProc
	},
	{	XmNdirSearchProc,
		XmCDirSearchProc,
		XmRProc, 
		sizeof(XtProc),
		XtOffset( XmFileSelectionBoxWidget,
                                           file_selection_box.dir_search_proc),
		XmRImmediate,
		(XtPointer) DirSearchProc
	},
	{	XmNfileSearchProc, 
		XmCFileSearchProc,
		XmRProc,
		sizeof(XtProc),
		XtOffset( XmFileSelectionBoxWidget,
                                          file_selection_box.file_search_proc),
		XmRImmediate,
		(XtPointer) FileSearchProc
	},
	{	XmNfileTypeMask,
		XmCFileTypeMask,
		XmRFileTypeMask,
		sizeof( unsigned char),
		XtOffset( XmFileSelectionBoxWidget, 
                                            file_selection_box.file_type_mask),
		XmRImmediate,
		(XtPointer) XmFILE_REGULAR
	}, 
	{	XmNlistUpdated,
		XmCListUpdated,
		XmRBoolean,
		sizeof(Boolean),
		XtOffset(XmFileSelectionBoxWidget,
                                              file_selection_box.list_updated),
		XmRImmediate,
		(XtPointer) TRUE
	},
	{	XmNdirectoryValid,
		XmCDirectoryValid,
		XmRBoolean,
		sizeof(Boolean),
		XtOffset(XmFileSelectionBoxWidget,
                                           file_selection_box.directory_valid),
		XmRImmediate,
		(XtPointer) TRUE
	},
	/* superclass resource default overrides */

	{	XmNdirSpec,
		XmCDirSpec,
		XmRXmString,
		sizeof( XmString),
		XtOffset( XmFileSelectionBoxWidget, selection_box.text_string),
		XmRImmediate,
		(XtPointer) UNSPECIFIED
	},                                        
	{	XmNautoUnmanage,
		XmCAutoUnmanage,
		XmRBoolean,
		sizeof(Boolean),
		XtOffset( XmFileSelectionBoxWidget,
                                                 bulletin_board.auto_unmanage),
		XmRImmediate,
		(XtPointer) FALSE
	},
	{	XmNfileListLabelString,
		XmCFileListLabelString,
		XmRXmString,
		sizeof(XmString),
		XtOffset( XmFileSelectionBoxWidget,
                                              selection_box.list_label_string),
		XmRString,
		"Files"
	},
	{	XmNapplyLabelString,
		XmCApplyLabelString,
		XmRXmString,
		sizeof(XmString),
		XtOffset( XmFileSelectionBoxWidget,
                                             selection_box.apply_label_string),
		XmRString,
		"Filter"
	},
	{	XmNdialogType,
		XmCDialogType,
		XmRDialogType,
		sizeof(unsigned char),
		XtOffset( XmFileSelectionBoxWidget, selection_box.dialog_type),
		XmRImmediate,
		(XtPointer) XmDIALOG_FILE_SELECTION
	},
	{	XmNfileListItems, 
		XmCItems, XmRXmStringTable, sizeof (XmString *), 
		XtOffset (XmSelectionBoxWidget, selection_box.list_items), 
		XmRImmediate, NULL
	},                                        
	{	XmNfileListItemCount, 
		XmCItemCount, XmRInt, sizeof(int), 
		XtOffset (XmSelectionBoxWidget, selection_box.list_item_count), 
		XmRImmediate, (caddr_t) UNSPECIFIED
	}, 

};
static XmSyntheticResource syn_resources[] =
{
  {	XmNdirectory,
	sizeof (XmString),
	XtOffset(XmFileSelectionBoxWidget, file_selection_box.directory),
	_XmFileSelectionBoxGetDirectory,
	NULL
  },
  {	XmNdirListLabelString,
	sizeof (XmString), 
	XtOffset(XmFileSelectionBoxWidget,
		 file_selection_box.dir_list_label_string),
	_XmFileSelectionBoxGetDirListLabelString,
	NULL
  },
  {     XmNdirListItems,
        sizeof( XmString *),
	XtOffset(XmFileSelectionBoxWidget, file_selection_box.dir_list_items),
        _XmFileSelectionBoxGetDirListItems,
        NULL
  },
  {    XmNdirListItemCount,
        sizeof( int),
	XtOffset(XmFileSelectionBoxWidget,
		 file_selection_box.dir_list_item_count),
        _XmFileSelectionBoxGetDirListItemCount,
        NULL
  },
  {	XmNfilterLabelString,
	sizeof (XmString), 
	XtOffset(XmFileSelectionBoxWidget,
		 file_selection_box.filter_label_string),
	_XmFileSelectionBoxGetFilterLabelString,
	NULL
  },
  {	XmNdirMask,
	sizeof( XmString), 
	XtOffset( XmFileSelectionBoxWidget, file_selection_box.dir_mask),
	_XmFileSelectionBoxGetDirMask,
	NULL
  },
  {	XmNdirSpec,
	sizeof (XmString), 
	XtOffset( XmFileSelectionBoxWidget, selection_box.text_string),
	_XmSelectionBoxGetTextString,
	NULL
  },
  {	XmNfileListLabelString,
	sizeof (XmString), 
	XtOffset( XmFileSelectionBoxWidget, selection_box.list_label_string),
	_XmSelectionBoxGetListLabelString,
	NULL
  },
  {	XmNfileListItems, 
	sizeof (XmString *), 
	XtOffset (XmSelectionBoxWidget, selection_box.list_items), 
	_XmFileSelectionBoxGetListItems,
	NULL
  },                                        
  {	XmNfileListItemCount, 
	sizeof(int), 
	XtOffset (XmSelectionBoxWidget, selection_box.list_item_count),
	_XmFileSelectionBoxGetListItemCount,
	NULL
  }, 
  {	XmNnoMatchString, 
	sizeof (XmString), 
	XtOffset( XmFileSelectionBoxWidget,
		 file_selection_box.no_match_string),
	_XmFileSelectionBoxGetNoMatchString,
	NULL
  },
  {	XmNpattern,
	sizeof( XmString), 
	XtOffset( XmFileSelectionBoxWidget,
		 file_selection_box.pattern),
	_XmFileSelectionBoxGetPattern,
	NULL
  },  
};

externaldef( xmfileselectionboxclassrec) XmFileSelectionBoxClassRec
                                                   xmFileSelectionBoxClassRec =
{
    {   /* core class record        */
	/* superclass	            */	(WidgetClass) &xmSelectionBoxClassRec,
	/* class_name		    */	"XmFileSelectionBox",
	/* widget_size		    */	sizeof(XmFileSelectionBoxRec),
	/* class_initialize	    */	NULL,
	/* class part init          */	ClassPartInitialize,
	/* class_inited		    */	FALSE,
	/* initialize		    */	Initialize,
	/* initialize hook	    */	NULL,
	/* realize		    */	_XtInherit,
	/* actions		    */	ActionsTable,
	/* num_actions		    */	XtNumber(ActionsTable),
	/* resources		    */	resources,
	/* num_resources	    */	XtNumber(resources),
	/* xrm_class		    */	NULLQUARK,
	/* compress_motion	    */	TRUE,
	/* compress_exposure        */	XtExposeCompressMaximal,
	/* compress crossing        */	FALSE,
	/* visible_interest	    */	FALSE,
	/* destroy		    */	Destroy,
	/* resize		    */	_XtInherit,
	/* expose		    */	_XtInherit,
	/* set_values		    */	SetValues,
	/* set_values_hook	    */	NULL,                    
	/* set_values_almost        */	_XtInherit,
	/* get_values_hook	    */	NULL,                    
	/* accept_focus		    */	NULL,
	/* version		    */	XtVersion,
	/* callback_offsets         */	NULL,
	/* tm_table                 */	XtInheritTranslations,
	/* query_geometry	    */	(XtGeometryHandler) _XtInherit,
	/* display_accelerator	    */	NULL,
	/* extension		    */	NULL,
	},
    {   /* composite class record   */    
	/* geometry manager         */	(XtGeometryHandler) _XtInherit,
	/* set changed proc	    */	_XtInherit,
	/* insert_child		    */	_XtInherit,
	/* delete_child 	    */	DeleteChild,
	/* extension		    */	NULL,
	},
    {   /* constraint class record  */
	/* no additional resources  */	NULL,
	/* num additional resources */	0,
	/* size of constraint rec   */	0,
	/* constraint_initialize    */	NULL,
	/* constraint_destroy	    */  NULL,
	/* constraint_setvalue      */	NULL,
	/* extension                */	NULL,
	},
    {   /* manager class record     */
	/* translations             */	XtInheritTranslations,
	/* get_resources            */	syn_resources,
	/* num_syn_resources        */	XtNumber(syn_resources),
	/* constraint_syn_resources */	NULL,
	/* num_constraint_syn_resources*/ 0,
        /* parent_process           */  XmInheritParentProcess,
	/* extension		    */	NULL,
	},
    {	/* bulletinBoard class record*/
	/* always_install_accelerators*/TRUE,
	/* geo_matrix_create        */	_XmFileSBGeoMatrixCreate,
	/* focus_moved_proc         */	_XmFileSelectionBoxFocusMoved,
	/* extension                */	NULL,
	},
    {	/*selectionbox class record */
        /* list_callback            */  ListCallback,
	/* extension		    */	NULL,
	},
    {	/* fileselection class record*/
	/* extension		    */	NULL,
	}
};

externaldef( xmfileselectionboxwidgetclass) WidgetClass
     xmFileSelectionBoxWidgetClass = (WidgetClass)&xmFileSelectionBoxClassRec ;

/****************************************************************/
static void
#ifdef _NO_PROTO
ClassPartInitialize( fsc)
            XmFileSelectionBoxWidgetClass fsc ;
#else
ClassPartInitialize(
            XmFileSelectionBoxWidgetClass fsc)
#endif
/****************
 * Class Initialization.  Sets up accelerators and fast subclassing.
 ****************/
{
/****************/

    _XmFastSubclassInit( fsc, XmFILE_SELECTION_BOX_BIT) ;

    return ;
    }
/****************************************************************/
static void
#ifdef _NO_PROTO
Initialize( request, new)
            XmFileSelectionBoxWidget request ;
            XmFileSelectionBoxWidget new ;
#else
Initialize(
            XmFileSelectionBoxWidget request,
            XmFileSelectionBoxWidget new)
#endif
/****************
 * This routine initializes an instance of the file selection widget.
 * Instance record fields which are shadow resources for child widgets and
 *   which are of an allocated type are set to NULL after they are used, since
 *   the memory identified by them is not owned by the File Selection Box.
 ****************/
{
            Arg             args[16] ;
            int             numArgs ;
            XmFileSelectionBoxCallbackStruct searchData ;
/****************/

    FS_StateFlags( new) = 0 ;

    /* must set adding_sel_widgets to avoid adding these widgets to 
     * selection work area
     */
    SB_AddingSelWidgets( new) = TRUE ;

    if(    !(SB_ListLabel( new))    )
    {   _XmSelectionBoxCreateListLabel(( XmSelectionBoxWidget) new) ;
        } 
    if(    !(SB_List( new))    )
    {   _XmSelectionBoxCreateList(( XmSelectionBoxWidget) new) ;
        } 
    if(    !(SB_SelectionLabel( new))    )
    {   _XmSelectionBoxCreateSelectionLabel(( XmSelectionBoxWidget) new) ;
        } 
    if(    !(SB_Text( new))    )
    {   _XmSelectionBoxCreateText(( XmSelectionBoxWidget) new) ;
        } 
    if(    !(SB_ApplyButton( new))    )
    {   _XmSelectionBoxCreateApplyButton(( XmSelectionBoxWidget) new) ;
        } 
    if(    !(SB_OkButton( new))    )
    {   _XmSelectionBoxCreateOkButton(( XmSelectionBoxWidget) new) ;
        } 
    if(    !(SB_CancelButton( new))    )
    {   _XmSelectionBoxCreateCancelButton(( XmSelectionBoxWidget) new) ;
        } 
    if(    !(SB_HelpButton( new))    )
    {   _XmSelectionBoxCreateHelpButton(( XmSelectionBoxWidget) new) ;
        } 

    numArgs = 0 ;
    XtSetArg( args[numArgs], XmNscrollBarDisplayPolicy, XmSTATIC) ; ++numArgs ;
    XtSetValues( SB_List( new), args, numArgs) ;

    _XmFileSelectionBoxCreateFilterLabel( new) ;
    FS_FilterLabelString(new) = NULL ;

    _XmFileSelectionBoxCreateDirListLabel(new) ;
    FS_DirListLabelString( new) = NULL ;

    _XmFileSelectionBoxCreateFilterText(new);

    _XmFileSelectionBoxCreateDirList(new) ;

    /* Since the DirSearchProc is going to be run during initialize,
    *   and since it has the responsibility to manage the directory list and
    *   the filter text, any initial values of the following resources can
    *   be ignored, since they will be immediately over-written.
    */
    FS_DirListItems( new) = NULL ;  /* Set/Get Values only.*/
    FS_DirListItemCount( new) = UNSPECIFIED ; /* Set/Get Values only.*/

    SB_AddingSelWidgets( new) = FALSE;

    BB_DefaultButton( new) = SB_OkButton( new) ;

    /* Remove the activate callbacks that our superclass
    *   may have attached to these buttons
    */
    XtRemoveAllCallbacks( SB_ApplyButton( new), XmNactivateCallback) ;
    XtRemoveAllCallbacks( SB_OkButton( new), XmNactivateCallback) ;
    XtRemoveAllCallbacks( SB_CancelButton( new), XmNactivateCallback) ;
    XtRemoveAllCallbacks( SB_HelpButton( new), XmNactivateCallback) ;

    XtAddCallback( SB_ApplyButton( new), XmNactivateCallback,
                                      FileSelectionPB, XmDIALOG_APPLY_BUTTON) ;
    XtAddCallback( SB_OkButton( new), XmNactivateCallback,
                                         FileSelectionPB, XmDIALOG_OK_BUTTON) ;
    XtAddCallback( SB_CancelButton( new), XmNactivateCallback,
                                     FileSelectionPB, XmDIALOG_CANCEL_BUTTON) ;
    XtAddCallback( SB_HelpButton( new), XmNactivateCallback,
                                       FileSelectionPB, XmDIALOG_HELP_BUTTON) ;
    _XmChangeNavigationType((Widget) new, ((XtIsShell( XtParent( request))
                                        ? XmSTICKY_TAB_GROUP : XmTAB_GROUP))) ;
    if(    FS_NoMatchString( new)    )
    {   FS_NoMatchString( new) = XmStringCopy( FS_NoMatchString( new)) ;
        } 
    searchData.reason = XmCR_NONE ;
    searchData.event = NULL ;
    searchData.value = NULL ;
    searchData.length = 0 ;
    searchData.mask = NULL ;
    searchData.mask_length = 0 ;
    searchData.dir = NULL ;
    searchData.dir_length = 0 ;
    searchData.pattern = NULL ;
    searchData.pattern_length = 0 ;

    /* The XmNdirSpec resource will be loaded into the Text widget by
    *   the Selection Box (superclass) Initialize routine.  It will be 
    *   picked-up there by the XmNqualifySearchDataProc routine to fill
    *   in the value field of the search data.
    */
    if(    FS_DirMask( new) != (XmString) UNSPECIFIED    )
    {   
        searchData.mask = XmStringCopy( FS_DirMask( new)) ;
        searchData.mask_length = XmStringLength( searchData.mask) ;

        /* The DirMask field will be set after subsequent call to
        *   the DirSearchProc.  Set field to NULL to prevent freeing of
        *   memory owned by request.
        */
        FS_DirMask( new) = (XmString) UNSPECIFIED ;
        } 
    if(    FS_Directory( new)    )
    {
        searchData.dir = XmStringCopy( FS_Directory( new)) ;
        searchData.dir_length = XmStringLength( searchData.dir) ;

        /* The Directory field will be set after subsequent call to
        *   the DirSearchProc.  Set field to NULL to prevent freeing of
        *   memory owned by request.
        */
        FS_Directory( new) = NULL ;
        }
    if(    FS_Pattern( new)    )
    {
        searchData.pattern = XmStringCopy( FS_Pattern( new)) ;
        searchData.pattern_length = XmStringLength( searchData.pattern) ;

        /* The Pattern field will be set after subsequent call to
        *   the DirSearchProc.  Set field to NULL to prevent freeing of
        *   memory owned by request.
        */
        FS_Pattern( new) = NULL ;
        }

    if(    !FS_QualifySearchDataProc( new)    )
    {   FS_QualifySearchDataProc( new) = QualifySearchDataProc ;
        } 
    if(    !FS_DirSearchProc( new)    )
    {   FS_DirSearchProc( new) = DirSearchProc ;
        } 
    if(    !FS_FileSearchProc( new)    )
    {   FS_FileSearchProc( new) = FileSearchProc ;
        } 

    FileSelectionBoxUpdate( new, &searchData) ;

    XmStringFree( searchData.mask) ;
    XmStringFree( searchData.pattern) ;
    XmStringFree( searchData.dir) ;

    /* Mark everybody as managed because no one else will.
    *   Only need to do this if we are the instantiated class.
    */
    if(    XtClass( new) == xmFileSelectionBoxWidgetClass    )
    {   XtManageChildren( new->composite.children, 
                                                 new->composite.num_children) ;
        } 
    return ;
    }
/****************************************************************/
static void
#ifdef _NO_PROTO
Destroy( fsb)
            XmFileSelectionBoxWidget fsb ;
#else
Destroy(
            XmFileSelectionBoxWidget fsb)
#endif
/****************
 * 
 ****************/
{
/****************/

    XmStringFree( FS_NoMatchString( fsb)) ;
    XmStringFree( FS_Pattern( fsb)) ;
    XmStringFree( FS_Directory( fsb)) ;

    return ;
    }
/****************************************************************/
static void
#ifdef _NO_PROTO
DeleteChild( w)
            Widget          w ;
#else
DeleteChild(
            Widget          w)
#endif
/****************
 * This procedure is called to remove the child from
 *   the child list, and to allow the parent to do any
 *   neccessary clean up.
 ****************/
{   
            XmFileSelectionBoxWidget fs ;
/****************/

    if(    XtIsRectObj( w)    )
    {   
        fs = (XmFileSelectionBoxWidget) XtParent( w) ;

        if(    w == FS_FilterLabel( fs)    )
        {   FS_FilterLabel( fs) = NULL ;
            } 
        else
        {   if(    w == FS_FilterText( fs)    )
            {   FS_FilterText( fs) = NULL ;
                } 
            else
            {   if(    w == FS_DirList( fs)    )
                {   FS_DirList( fs) = NULL ;
                    } 
                else
                {   if(    w == FS_DirListLabel( fs)    )
                    {   FS_DirListLabel( fs) = NULL ;
                        } 
                    } 
                } 
            }
        }
    (*((XmSelectionBoxWidgetClass) xmSelectionBoxWidgetClass)
                                          ->composite_class.delete_child)( w) ;
    return ;
    }
/****************************************************************/
void
#ifdef _NO_PROTO
_XmFileSelectionBoxCreateFilterLabel( fsb)
            XmFileSelectionBoxWidget fsb ;
#else
_XmFileSelectionBoxCreateFilterLabel(
            XmFileSelectionBoxWidget fsb)
#endif
/****************
 * 
 ****************/
{
/****************/

    FS_FilterLabel( fsb) = _XmBB_CreateLabelG( (Widget)fsb, FS_FilterLabelString( fsb),
                                                              "fsb_filter_label") ;
    return ;
    }
/****************************************************************/
void
#ifdef _NO_PROTO
_XmFileSelectionBoxCreateDirListLabel( fsb)
            XmFileSelectionBoxWidget fsb ;
#else
_XmFileSelectionBoxCreateDirListLabel(
            XmFileSelectionBoxWidget fsb)
#endif
/****************
 * 
 ****************/
{
/****************/

    FS_DirListLabel( fsb) = _XmBB_CreateLabelG((Widget) fsb,
                               FS_DirListLabelString( fsb),
                                                        "fsb_dir_list_label") ;
    return ;
    }
/****************************************************************/
void
#ifdef _NO_PROTO
_XmFileSelectionBoxCreateDirList( fsb)
            XmFileSelectionBoxWidget fsb ;
#else
_XmFileSelectionBoxCreateDirList(
            XmFileSelectionBoxWidget fsb)
#endif
/****************
 * Create the directory List widget.
 ****************/
{
	XmString	selected_items[2];
	Arg		al[20];
	register int	ac = 0;
            XtCallbackProc callbackProc ;
/****************/

    FS_DirListSelectedItemPosition( fsb) = 0 ;

    XtSetArg( al[ac], XmNvisibleItemCount,
                                        SB_ListVisibleItemCount( fsb)) ; ac++ ;
    XtSetArg( al[ac], XmNstringDirection, SB_StringDirection( fsb));  ac++;
    XtSetArg( al[ac], XmNselectionPolicy, XmBROWSE_SELECT);  ac++;
    XtSetArg( al[ac], XmNlistSizePolicy, XmCONSTANT);  ac++;
    XtSetArg( al[ac], XmNscrollBarDisplayPolicy, XmSTATIC);  ac++;
    XtSetArg( al[ac], XmNnavigationType, XmSTICKY_TAB_GROUP) ; ++ac ;

    FS_DirList( fsb) = XmCreateScrolledList((Widget) fsb, "fsb_dir_list", al, ac);

    callbackProc = ((XmSelectionBoxWidgetClass) fsb->core.widget_class)
                                          ->selection_box_class.list_callback ;
    if(    callbackProc    )
    {   
        XtAddCallback( FS_DirList( fsb), XmNsingleSelectionCallback,
                                               callbackProc, (XtPointer) fsb) ;
        XtAddCallback( FS_DirList( fsb), XmNbrowseSelectionCallback,
                                               callbackProc, (XtPointer) fsb) ;
        XtAddCallback( FS_DirList( fsb), XmNdefaultActionCallback,
                                               callbackProc, (XtPointer) fsb) ;
        } 
    XtManageChild( FS_DirList( fsb)) ;

    return ;
    }
/****************************************************************/
void
#ifdef _NO_PROTO
_XmFileSelectionBoxCreateFilterText( fs)
            XmFileSelectionBoxWidget fs ;
#else
_XmFileSelectionBoxCreateFilterText(
            XmFileSelectionBoxWidget fs)
#endif
/****************
 * Creates fs dir search filter text entry field.
 ****************/
{
            Arg             arglist[10] ;
            int             argCount ;
            char *          stext_value ;
            XtAccelerators  temp_accelerators ;
/****************/

    /* Get text portion from Compound String, and set
    *   fs_stext_charset and fs_stext_direction bits...
    */
    /* Should do this stuff entirely with XmStrings when the text
    *   widget supports it.
    */
    if(    !(XmStringGetLtoR( FS_Pattern( fs), XmSTRING_DEFAULT_CHARSET, 
                                                            &stext_value))    )
    {   stext_value = (char *) XtMalloc( 1) ;
        stext_value[0] = '\0' ;
        }
    argCount = 0 ;
    XtSetArg( arglist[argCount], XmNrows, 1) ; argCount++ ;
    XtSetArg( arglist[argCount], XmNcolumns, 
                                            SB_TextColumns( fs)) ; argCount++ ;
    XtSetArg( arglist[argCount], XmNresizeWidth, FALSE) ; argCount++ ;
    XtSetArg( arglist[argCount], XmNvalue, stext_value) ; argCount++ ;
    XtSetArg( arglist[argCount], XmNeditMode, XmSINGLE_LINE) ; argCount++ ;
    XtSetArg( arglist[argCount], XmNnavigationType, 
                                             XmSTICKY_TAB_GROUP) ; argCount++ ;
    FS_FilterText( fs) = XmCreateText( (Widget)fs, "fsb_filter_text",
                                                           arglist, argCount) ;
    /*	Install text accelerators.
    */
    temp_accelerators = fs->core.accelerators ;
    fs->core.accelerators = SB_TextAccelerators( fs) ;
    XtInstallAccelerators( FS_FilterText( fs), fs) ;
    fs->core.accelerators = temp_accelerators ;

    XtFree( stext_value) ;
    return ;
    }
/****************************************************************/
XmGeoMatrix    
#ifdef _NO_PROTO
_XmFileSBGeoMatrixCreate( fsb, instigator, desired)
            XmFileSelectionBoxWidget fsb ;
            Widget          instigator ;
            XtWidgetGeometry *desired ;
#else
_XmFileSBGeoMatrixCreate(
            XmFileSelectionBoxWidget fsb,
            Widget          instigator,
            XtWidgetGeometry *desired)
#endif
/****************
 * Get Geo matrix filled with kid widgets.
 ****************/
{
            XmGeoMatrix     geoSpec ;
    register XmGeoRowLayout  layoutPtr ;
    register XmKidGeometry   boxPtr ;
            XmKidGeometry   firstButtonBox ; 
            Boolean         dirListLabelBox ;
            Boolean         listLabelBox ;
            Boolean         dirListBox ;
            Boolean         listBox ;
            Boolean         selLabelBox ;
            Boolean         filterLabelBox ;
/****************/

    geoSpec = _XmGeoMatrixAlloc( FSB_MAX_WIDGETS_VERT, FSB_MAX_NUM_WIDGETS,
                                                 sizeof( FS_GeoExtensionRec)) ;
    geoSpec->composite = (Widget) fsb ;
    geoSpec->instigator = (Widget) instigator ;
    if(    desired    )
    {   geoSpec->instig_request = *desired ;
        } 
    geoSpec->margin_w = BB_MarginWidth( fsb) + fsb->manager.shadow_thickness ;
    geoSpec->margin_h = BB_MarginHeight( fsb) + fsb->manager.shadow_thickness ;
    geoSpec->no_geo_request = _XmFileSelectionBoxNoGeoRequest ;

    layoutPtr = geoSpec->layouts ;
    boxPtr = geoSpec->boxes ;

    filterLabelBox = FALSE ;
    if(    _XmGeoSetupKid( boxPtr, FS_FilterLabel( fsb))    )
    {   
        filterLabelBox = TRUE ;
        layoutPtr->space_above = BB_MarginHeight( fsb) ;
        boxPtr += 2 ;       /* For new row, add 2. */
        ++layoutPtr ;       /* For new row. */
        } 
    if(    _XmGeoSetupKid( boxPtr, FS_FilterText( fsb))    )
    {   
        if(    !filterLabelBox    )
        {   layoutPtr->space_above = BB_MarginHeight( fsb) ;
            } 
        boxPtr += 2 ;       /* For new row, add 2. */
        ++layoutPtr ;       /* For new row. */
        } 
    dirListLabelBox = FALSE ;
    if(    _XmGeoSetupKid( boxPtr, FS_DirListLabel( fsb))    )
    {   
        dirListLabelBox = TRUE ;
        ++boxPtr ;
        } 
    listLabelBox = FALSE ;
    if(    _XmGeoSetupKid( boxPtr, SB_ListLabel( fsb))    )
    {   
        listLabelBox = TRUE ;
        ++boxPtr ;
        } 
    if(    dirListLabelBox  ||  listLabelBox    )
    {   layoutPtr->fix_up = ListLabelFix ;
        layoutPtr->space_above = BB_MarginHeight( fsb) ;
        layoutPtr->space_between = BB_MarginWidth( fsb) ;
	if ( dirListLabelBox && listLabelBox    )
	{   layoutPtr->sticky_end = TRUE ;
	    }
        layoutPtr->fill_mode = XmGEO_PACK ;
        ++boxPtr ;
        ++layoutPtr ;       /* For new row. */
        } 
    dirListBox = FALSE ;
    if(     FS_DirList( fsb)
        &&  _XmGeoSetupKid( boxPtr, XtParent( FS_DirList( fsb)))    )
    {   
        dirListBox = TRUE ;
        ++boxPtr ;
        } 
    listBox = FALSE ;
    if(    SB_List( fsb)
        && _XmGeoSetupKid( boxPtr, XtParent( SB_List( fsb)))    )
    {   
        listBox = TRUE ;
        ++boxPtr ;
        } 
    if(    dirListBox  || listBox    )
    {   layoutPtr->fix_up = ListFix ;
        layoutPtr->fit_mode = XmGEO_AVERAGING ;
        layoutPtr->space_between = BB_MarginWidth( fsb) ;
        layoutPtr->stretch_height = TRUE ;
        layoutPtr->min_height = 70 ;
        if(    !listLabelBox  &&  !dirListLabelBox    )
        {   layoutPtr->space_above = BB_MarginHeight( fsb) ;
            } 
        ++boxPtr ;
        ++layoutPtr ;
        } 
    if(    _XmGeoSetupKid( boxPtr, SB_WorkArea( fsb))    )
    {   layoutPtr->space_above = BB_MarginHeight( fsb) ;
        boxPtr += 2 ;       /* For new row, add 2. */
        ++layoutPtr ;       /* For new row. */
        } 
    selLabelBox = FALSE ;
    if(    _XmGeoSetupKid( boxPtr, SB_SelectionLabel( fsb))    )
    {   selLabelBox = TRUE ;
        layoutPtr->space_above = BB_MarginHeight( fsb) ;
        boxPtr += 2 ;       /* For new row, add 2. */
        ++layoutPtr ;       /* For new row. */
        } 
    if(    _XmGeoSetupKid( boxPtr, SB_Text( fsb))    )
    {   
        if(    !selLabelBox    )
        {   layoutPtr->space_above = BB_MarginHeight( fsb) ;
            } 
        boxPtr += 2 ;       /* For new row, add 2. */
        ++layoutPtr ;       /* For new row. */
        } 
    if(    _XmGeoSetupKid( boxPtr, SB_Separator( fsb))    )
    {   layoutPtr->fix_up = _XmSeparatorFix ;
        layoutPtr->space_above = BB_MarginHeight( fsb) ;
        boxPtr += 2 ;       /* For new row, add 2. */
        ++layoutPtr ;       /* For new row. */
        } 
    firstButtonBox = boxPtr ;
    if(    _XmGeoSetupKid( boxPtr, SB_OkButton( fsb))    )
    {   ++boxPtr ;
        } 
    if(    _XmGeoSetupKid( boxPtr, SB_ApplyButton( fsb))    )
    {   ++boxPtr ;
        } 
    if(    _XmGeoSetupKid( boxPtr, SB_CancelButton( fsb))    )
    {   ++boxPtr ;
        } 
    if(    _XmGeoSetupKid( boxPtr, SB_HelpButton( fsb))    )
    {   ++boxPtr ;
        } 
    if(    boxPtr != firstButtonBox    )
    {   /* Had at least one button.
        */
        layoutPtr->fill_mode = XmGEO_CENTER ;
        layoutPtr->fit_mode = XmGEO_WRAP ;
        if(    !(SB_MinimizeButtons( fsb))    )
        {   layoutPtr->even_width = 1 ;
            } 
        layoutPtr->space_above = BB_MarginHeight( fsb) ;
        layoutPtr->even_height = 1 ;
        ++boxPtr ;
	++layoutPtr ;
        } 
    layoutPtr->space_above = BB_MarginHeight( fsb) ;
    layoutPtr->end = TRUE ;        /* Mark the last row. */
    return( geoSpec) ;
    }
/****************************************************************/
Boolean
#ifdef _NO_PROTO
_XmFileSelectionBoxNoGeoRequest( geoSpec)
            XmGeoMatrix     geoSpec ;
#else
_XmFileSelectionBoxNoGeoRequest(
            XmGeoMatrix     geoSpec)
#endif
/****************
 * 
 ****************/
{
/****************/

    if(    BB_InSetValues( geoSpec->composite)
        && (XtClass( geoSpec->composite) == xmFileSelectionBoxWidgetClass)    )
    {   
        return( TRUE) ;
        } 
    return( FALSE) ;
    }
/****************************************************************/
static void
#ifdef _NO_PROTO
ListLabelFix( geoSpec, action, layoutPtr, rowPtr)
            XmGeoMatrix     geoSpec ;
            int             action ;
            XmGeoRowLayout  layoutPtr ;
            XmKidGeometry   rowPtr ;
#else
ListLabelFix(
            XmGeoMatrix     geoSpec,
            int             action,
            XmGeoRowLayout  layoutPtr,
            XmKidGeometry   rowPtr)
#endif
/****************           ARGSUSED
 * This routine saves the geometry pointers of the list labels so that they
 *   can be altered as appropriate by the ListFix routine.
 ****************/
{
            FS_GeoExtension extension ;
/****************/

    extension = (FS_GeoExtension) geoSpec->extension ;
    extension->dir_list_label = rowPtr++ ;
    extension->file_list_label = rowPtr ;

    return ;
    }

/****************************************************************/
static void
#ifdef _NO_PROTO
ListFix( geoSpec, action, layoutPtr, rowPtr)
            XmGeoMatrix     geoSpec ;
            int             action ;
            XmGeoRowLayout  layoutPtr ;
            XmKidGeometry   rowPtr ;
#else
ListFix(
            XmGeoMatrix     geoSpec,
            int             action,
            XmGeoRowLayout  layoutPtr,
            XmKidGeometry   rowPtr)
#endif
/****************           ARGSUSED
 * Geometry layout fixup routine for the directory and file lists.  This
 *   routine reduces the preferred width of the file list widget according 
 *   to the length of the directory  path.
 * This algorithm assumes that each row has at least one box.
 ****************/
{
            Dimension       listPathWidth ;
            XmListWidget    dirList ;
            XmListWidget    fileList ;
            XmKidGeometry   fileListGeo ;
            XmKidGeometry   dirListGeo ;
            XmString        dirString ;
            Arg             argv[2] ;
            Cardinal        argc ;
            XmFontList      listFonts ;
            FS_GeoExtension extension ;
            int             listLabelsOffset ;
/****************/

    dirListGeo = rowPtr++ ;
    fileListGeo = rowPtr ;

    if(    !fileListGeo->kid    )
    {   /* Only one list widget in this row, so do nothing.
        */
        return ;
        }
    extension = (FS_GeoExtension) geoSpec->extension ;
    fileList = (XmListWidget) SB_List( geoSpec->composite) ;
    dirString = FS_Directory( geoSpec->composite) ;

    switch(    action    )
    {   
        case XmGET_PREFERRED_SIZE:
        {   
            argc = 0 ;
            XtSetArg( argv[argc], XmNfontList, &listFonts) ; ++argc ;
            XtGetValues( fileList, argv, argc) ;

            listPathWidth = XmStringWidth( listFonts, dirString) ;

            if(    !(FS_StateFlags( geoSpec->composite) & FS_NO_MATCH)    )
            {   
                if(    listPathWidth < fileListGeo->box.width    )
                {   fileListGeo->box.width -= listPathWidth ;
                    } 
                } 
            if(    listPathWidth < dirListGeo->box.width    )
            {   dirListGeo->box.width -= listPathWidth ;
                } 
            if(    extension->dir_list_label
                && (extension->dir_list_label->box.width
                                                  < dirListGeo->box.width)    )
            {   extension->dir_list_label->box.width = dirListGeo->box.width ;
                } 
            /* Drop through to pick up extension record field for either
            *   type of geometry request.
            */
            }
        case XmGET_ACTUAL_SIZE:
        {   extension->prefer_width = fileListGeo->box.width ;
            break ;
            } 
        case XmGEO_PRE_SET:
        {   
            if(    fileListGeo->box.width > extension->prefer_width    )
            {   
                /* Add extra space designated for file list to dir list
                *   instead, assuring that file list only shows the file name
                *   and not a segment of the path.
                */
                extension->delta_width = fileListGeo->box.width
                                                    - extension->prefer_width ;
                fileListGeo->box.width -= extension->delta_width ;
                fileListGeo->box.x += extension->delta_width ;
                dirListGeo->box.width += extension->delta_width ;
                } 
            else
            {   extension->delta_width = 0 ;
                } 
            /* Set label boxes to be the same width and x dimension as the 
            *   lists below them.
            */
            if(    extension->file_list_label    )
            {   
                if(    extension->file_list_label->box.width 
                                                  < fileListGeo->box.width    )
                {   extension->file_list_label->box.width
                                                     = fileListGeo->box.width ;
                    extension->file_list_label->box.x = fileListGeo->box.x ;
                    } 
                if(    extension->dir_list_label    )
                {   
                    listLabelsOffset = extension->file_list_label->box.x
                                           - extension->dir_list_label->box.x ;
                    if(    listLabelsOffset
                                          > (int) layoutPtr->space_between    )
                    {   extension->dir_list_label->box.width =
                                            (Dimension) listLabelsOffset
                                                   - layoutPtr->space_between ;
                        } 
                    }
                } 
            break ;
            } 
        case XmGEO_POST_SET:
        {   
            if(    extension->delta_width    )
            {   /* Undo the changes of PRE_SET, so subsequent re-layout
                *   attempts will yield correct results.
                */
                fileListGeo->box.width += extension->delta_width ;
                fileListGeo->box.x -= extension->delta_width ;
                dirListGeo->box.width -= extension->delta_width ;
                } 
            if(    !(FS_StateFlags( geoSpec->composite) & FS_NO_MATCH)    )
            {   
                /* Move horizontal position so path does not show in file list.
                */
                argc = 0 ;
                XtSetArg( argv[argc], XmNfontList, &listFonts) ; ++argc ;
                XtGetValues( fileList, argv, argc) ;
                listPathWidth = XmStringWidth( listFonts, dirString) ;
                XmListSetHorizPos((Widget) fileList, listPathWidth) ;
                } 
            /* Move horizontal scroll position of directory list as far to the
            *   right as it will go, so that the right end of the list is 
            *   never hidden.
            */
            dirList = (XmListWidget) FS_DirList( geoSpec->composite) ;
            argc = 0 ;
            XtSetArg( argv[argc], XmNfontList, &listFonts) ; ++argc ;
            XtGetValues( dirList, argv, argc) ;

            listPathWidth = XmStringWidth( listFonts, dirString) ;
            XmListSetHorizPos((Widget) dirList, listPathWidth) ;
            
            break ;
            } 
        } 
    return ;
    }
/****************************************************************/
static String
#ifdef _NO_PROTO
GetQualifiedDir( dirSpec)
            String          dirSpec ;
#else
GetQualifiedDir(
            String          dirSpec)
#endif
/****************
 * Builds directory name showing descriptive path components.  The result
 *   is a directory path beginning at the root directory and terminated
 *   with a '/'.  The path will not contain ".", "..", or "~" components.  
 * The routine allocates memory for the result, which is guaranteed to be
 *   of length >= 1.  This memory should eventually be freed using XtFree().
 ****************/
{
            int             dirSpecLen ;
            struct passwd * userDir ;
            int             userDirLen ;
            int             userNameLen ;
            char *          outputBuf ;
            char *          destPtr ;
            char *          srcPtr ;
            char *          scanPtr ;
            char            nameBuf[MAX_USER_NAME_LEN] ;
            char            dirbuf[MAX_DIR_PATH_LEN] ;

#ifdef USE_GETWD
    extern  char *          getwd() ;
#else
    extern  char *          getcwd() ;
#endif
/****************/

    dirSpecLen = strlen( dirSpec) ;
    outputBuf = NULL ;

    switch(    *dirSpec    )
    {   case '~':
        {   if(    !(dirSpec[1])  ||  (dirSpec[1] == '/')    )
	    {
		userDir = (struct passwd *) getpwuid( getuid()) ;
                if(    userDir    )
                {   
    		    userDirLen = strlen( userDir->pw_dir) ;
    		    outputBuf = XtMalloc( userDirLen + dirSpecLen + 2) ;
                    strcpy( outputBuf, userDir->pw_dir) ;
    		    strcpy( &outputBuf[userDirLen], (dirSpec + 1)) ;
                    }
    	        }
	    else
	    {
		destPtr = nameBuf ;
                userNameLen = 0 ;
		srcPtr = dirSpec + 1 ;
		while(    *srcPtr  &&  (*srcPtr != '/')
                       && (++userNameLen < MAX_USER_NAME_LEN)    )
		{   *destPtr++ = *srcPtr++ ;
                    } 
		*destPtr = NULL ;

                userDir = (struct passwd *)getpwnam( nameBuf) ;
		if(    userDir    )
		{   
		    userDirLen = strlen( userDir->pw_dir) ;
		    dirSpecLen = strlen( srcPtr) ;
		    outputBuf = XtMalloc( userDirLen + dirSpecLen + 2) ;
                    strcpy( outputBuf, userDir->pw_dir) ;
                    strcpy( &outputBuf[userDirLen], srcPtr) ;
                    } 
		}
            break ;
            } 
        case '/':
        {   outputBuf = XtMalloc( dirSpecLen + 2) ;
	    strcpy( outputBuf, dirSpec) ;
            break ;
            } 
        default:
        {  
#ifdef USE_GETWD
            destPtr = getwd( dirbuf) ;
#else
            destPtr = getcwd( dirbuf, MAX_DIR_PATH_LEN) ;
#endif
            if(    destPtr    )
            {   userDirLen = strlen( destPtr) ;
	        outputBuf = XtMalloc( userDirLen + dirSpecLen + 3) ;
                strcpy( outputBuf, destPtr) ;
	        outputBuf[userDirLen++] = '/';
                strcpy( &outputBuf[userDirLen], dirSpec) ;
                } 
            break ;
            } 
        } 
    if(    !outputBuf    )
    {   outputBuf = XtMalloc( 2) ;
        outputBuf[0] = '/' ;
        outputBuf[1] = '\0' ;
        } 
    else
    {   userDirLen = strlen( outputBuf) ;
        if(    outputBuf[userDirLen - 1]  !=  '/'    )
        {   outputBuf[userDirLen] = '/' ;
            outputBuf[++userDirLen] = NULL ;
            } 
        /* The string in outputBuf is assumed to begin and end with a '/'.
        */
        scanPtr = outputBuf ;
        while(    *++scanPtr    )               /* Skip past '/'. */
        {   /* scanPtr now points to non-NULL character following '/'.
            */
            if(    scanPtr[0] == '.'    )
            {   
                if(    scanPtr[1] == '/'    )
                {   /* Have "./", so just erase (overwrite with shift).
                    */
                    destPtr = scanPtr ;
                    srcPtr = &scanPtr[2] ;
                    while(    *destPtr++ = *srcPtr++    )
                    {   } 
                    --scanPtr ;     /* Leave scanPtr at preceding '/'. */
                    continue ;
                    } 
                else
                {   if(    (scanPtr[1] == '.')  &&  (scanPtr[2] == '/')    )
                    {   /* Have "../", so back up one directory.
                        */
                        srcPtr = &scanPtr[2] ;
                        --scanPtr ;         /* Move scanPtr to preceding '/'.*/
                        if(    scanPtr != outputBuf    )
                        {   while(    (*--scanPtr != '/')    )
                            {   }                /* Now move to previous '/'.*/
                            } 
                        destPtr = scanPtr ;
                        while(    *++destPtr = *++srcPtr    )
                        {   }               /* Overwrite "../" with shift.*/
                        continue ;
                        } 
                    } 
                } 
            else
            {   /* Check for embedded "//".  Posix allows a leading double
                *   slash (and Apollos require it).
                */
                if(    *scanPtr == '/'    )
                {   
		    if(    (scanPtr > (outputBuf + 1))
                        || (scanPtr[1] == '/')    )
                    {
                        /* Have embedded "//" (other than root specification),
			 *   so erase with shift and reset scanPtr.
			 */
			srcPtr = scanPtr ;
			--scanPtr ;
			destPtr = scanPtr ;
			while(    *++destPtr = *++srcPtr    )
			    {   } 
		    }
                    continue ;
		}
	    } 
            while(    *++scanPtr != '/'    )
		{   } 
	} 
    } 
	    return( outputBuf) ;
}
/****************************************************************/
static char *
#ifdef _NO_PROTO
FindPatternPart( fileSpec)
            char *          fileSpec ;
#else    
FindPatternPart(
            char *          fileSpec)
#endif
/****************
 * Returns the pointer to the character following the '/' of the name segment
 *   which contains a wildcard or which is not followed by a '/'.
 ****************/
{
            char *          lookAheadPtr = fileSpec ;
            char *          maskPtr ;
            Boolean         hasWildcards ;
            char            prevChar ;
            char            prev2Char ;
/****************/

    do
    {   /* Stop at final name segment or if wildcards were found.
        */
        maskPtr = lookAheadPtr ;
        hasWildcards = FALSE ;
        prevChar = NULL ;
        prev2Char = NULL ;
        while(  (*lookAheadPtr != '/')  &&  !hasWildcards  &&  *lookAheadPtr  )
        {   switch(    *lookAheadPtr    )
            {   case '*':
                case '?':
                case '[':
                {   if(    (prevChar != '\\')  ||  (prev2Char == '\\')    )
                    {   hasWildcards = TRUE ;
                        break ;
                        } 
                    } 
                } 
            prev2Char = prevChar ;
            prevChar = *lookAheadPtr ;
            ++lookAheadPtr ;
            } 
        }while(    !hasWildcards  &&  *lookAheadPtr++    ) ;
    if(    *maskPtr == '/'    )
    {   ++maskPtr ;
        } 
    return( maskPtr) ;
    }
/****************************************************************/
static Boolean
#ifdef _NO_PROTO
QualifyFileSpec( dirSpec, filterSpec, pQualifiedDir, pQualifiedPattern)
            String          dirSpec ;
            String          filterSpec ;
            String *        pQualifiedDir ;     /* Cannot be NULL.*/
            String *        pQualifiedPattern ; /* Cannot be NULL.*/
#else
QualifyFileSpec(
            String          dirSpec,
            String          filterSpec,
            String *        pQualifiedDir,      /* Cannot be NULL.*/
            String *        pQualifiedPattern)  /* Cannot be NULL.*/
#endif
/****************
 *
 ****************/
{
            Boolean         directoryOverride = FALSE ;
            int             filterLen ;
            int             dirLen ;
            char *          fSpec ;
            char *          remFSpec ;
            char *          maskPtr ;
            char *          dSpec ;
            char *          dPtr ;
/****************/

    if(    !dirSpec    )
    {   dirSpec = "" ;
        } 
    if(    !filterSpec    )
    {   filterSpec = "" ;
        } 
    filterLen = strlen( filterSpec) ;

    /* Allocate extra for NULL character and for the appended '*' (as needed).
    */
    fSpec = XtMalloc( filterLen + 2) ;
    strcpy( fSpec, filterSpec) ;

    /* If fSpec ends with a '/' or is a null string, add '*' since this is
    *   the interpretation.
    */
    if(    !filterLen  ||  (fSpec[filterLen - 1] == '/')    )
    {   fSpec[filterLen] = '*' ;
        fSpec[filterLen + 1] = '\0' ;
        } 
    /* Some parts of fSpec may be copied to dSpec, so allocate "filterLen" 
    *   extra, plus some for added literals.
    */
    dirLen = strlen( dirSpec) ;
    dSpec = XtMalloc( filterLen + dirLen + 4) ;
    strcpy( dSpec, dirSpec) ;
    dPtr = dSpec + dirLen ;

    /* Check for cases when the specified filter overrides anything
    *   in the dirSpec.
    */
    remFSpec = fSpec ;
    switch(    *fSpec    )
    {   
        case '/':
        {   
            dSpec[0] = '/' ;
            dSpec[1] = '\0' ;
            dPtr = dSpec + 1 ;
            ++remFSpec ;
            directoryOverride = TRUE ;
            break ;
            } 
        case '~':
        {   
            dPtr = dSpec ;
            while(    (*dPtr = *remFSpec)  &&  (*remFSpec++ != '/')    )
            {   ++dPtr ;
                } 
            *dPtr = NULL ;
            directoryOverride = TRUE ;
            break ;
            } 
        } 
    /* If directory spec. is not null, then make sure that it has a
    *   trailing '/', to be prepared for appending from filter spec.
    */
    if(    *dSpec  &&  (*(dPtr - 1) != '/')    )
    {   *dPtr++ = '/' ;
        *dPtr = NULL ;
        } 

    maskPtr = FindPatternPart( remFSpec) ;

    if(    maskPtr != remFSpec    )
    {   directoryOverride = TRUE ;
        do
        {   *dPtr++ = *remFSpec++ ;
            } while(    remFSpec != maskPtr    ) ;
        *dPtr = '\0' ;
        } 

    if(    remFSpec != fSpec    )
    {   /* Shift remaining filter spec. to the beginning of the buffer.
        */
        remFSpec = fSpec ;
        while(    *remFSpec++ = *maskPtr++    )
        {   }
        } 
    *pQualifiedDir = GetQualifiedDir( dSpec) ;
    *pQualifiedPattern = fSpec ;
    XtFree( dSpec) ;
    return( directoryOverride) ;
    }

/****************************************************************/
static char *
#ifdef _NO_PROTO
GetFixedMatchPattern( pattern)
            char *          pattern ;
#else
GetFixedMatchPattern(
            char *          pattern)
#endif
/****************
 * The pattern parameter is converted to the format required of the
 *   the regex and re_comp routines.
 * Memory is allocated and returned with the result.  This memory
 *   should eventually be freed by a call to XtFree().
 ****************/
{
    register char *         bufPtr ;
            char *          outputBuf ;
/****************/

    outputBuf = XtCalloc( 2, strlen( pattern) + 4) ;

    bufPtr = outputBuf ;
    *bufPtr++ = '^' ;

    while(    *pattern  &&  (*pattern != '/')   )
    {   
        switch(    *pattern    )
        {   
            case '.':
            {   *bufPtr++ = '\\' ;
                *bufPtr++ = '.' ;
                break ;
                } 
            case '?':
            {   *bufPtr++ = '.' ;
                break;
                } 
            case '*':
            {   *bufPtr++ = '.' ;
                *bufPtr++ = '*' ;
                break ;
                } 
            default:
            {   *bufPtr++ = *pattern ;
                break ;
                } 
            } 
        ++pattern ;
        } 
    *bufPtr++ = '$' ;
    *bufPtr = '\0' ;

    return( outputBuf) ;
    }
/****************************************************************/
static void
#ifdef _NO_PROTO
GetDirEntries( qualifiedDir, matchPattern, fileType, matchDotsLiterally,
                            listWithFullPath, pEntries, pNumEntries, pNumAlloc)
            String          qualifiedDir ;
            String          matchPattern ;
            unsigned char   fileType ;
            Boolean         matchDotsLiterally ;
            Boolean         listWithFullPath ;
            String * *      pEntries ;      /* Cannot be NULL. */
            unsigned int *  pNumEntries ;   /* Cannot be NULL. */
            unsigned int *  pNumAlloc ;     /* Cannot be NULL. */
#else
GetDirEntries(
            String          qualifiedDir,
            String          matchPattern,
            unsigned char   fileType,
            Boolean         matchDotsLiterally,
            Boolean         listWithFullPath,
            String * *      pEntries,       /* Cannot be NULL. */
            unsigned int *  pNumEntries,    /* Cannot be NULL. */
            unsigned int *  pNumAlloc)      /* Cannot be NULL. */
#endif
/****************
 * This routine opens the specified directory and builds a buffer containing
 *   a series of strings containing the full path of each file in the
 *   directory.  The memory allocated should eventually be freed using XtFree.
 * The 'qualifiedDir' parameter must be a fully qualified directory path 
 *   (begins with '/', does not have embedded "." or "..", but does not need
 *   trailing '/').
 * Note that the matchPattern parameter must be in the proper form 
 *   for regcmp or re_comp (whichever is being used).
 * If the location pointed to by pEntries is NULL, this routine allocates
 *   and returns a list to *pEntries, though the list may have no entries.
 *   pEntries, pEndIndex, pNumAlloc are updated as required for memory 
 *   management.
 ****************/
{
            char *          fixedMatchPattern ;
            String          entryPtr ;
            DIR *           dirStream ;
            struct stat     statBuf ;
            Boolean         entryTypeOK ;
            unsigned int    dirLen = strlen( qualifiedDir) ;
#ifndef NO_REGEX
            char *          compiledRE = NULL ;
    extern  char *          regcmp() ;
#endif
#ifdef NDIR 
            struct direct * dirEntry ;
#else 
#ifdef SYS_DIR
            struct direct * dirEntry ;
#else
            struct dirent * dirEntry ;
#endif
#endif
/****************/

    if(    !*pEntries    )
    {   *pNumEntries = 0 ;
        *pNumAlloc = FILE_LIST_BLOCK ;
        *pEntries = (String *) XtMalloc( FILE_LIST_BLOCK * sizeof( char *)) ;
        } 
    fixedMatchPattern = GetFixedMatchPattern( matchPattern) ;

    if(    fixedMatchPattern    )
    {   
        if(    !*fixedMatchPattern    )
        {   
            XtFree( fixedMatchPattern) ;
            fixedMatchPattern = NULL ;
            } 
        else
        {   
#ifndef NO_REGEX
            compiledRE = regcmp( fixedMatchPattern, (char *) NULL) ;
            if(    !compiledRE    )
#else
            /* On Sun's re_comp does not return the address of
            *   the regular expression.  Instead it returns 
            *   NULL if the expression was parsed successfully.
            * Since re_exec does'nt use the 're' argument, don't need
            *   to set it.
            */
            if(    re_comp( fixedMatchPattern, 0)    )
#endif
            {   XtFree( fixedMatchPattern) ;
                fixedMatchPattern = NULL ;
                } 
            }
        }
    dirStream = opendir( qualifiedDir) ;
    if(    dirStream    )
    {   
        while(    dirEntry = readdir( dirStream)    )
        {   
            if(    fixedMatchPattern    )
            {   
#ifndef NO_REGEX
                if(    !regex( compiledRE, dirEntry->d_name)    )
#else
                /* On Sun's they use re_exec instead of regex,
                *   and it takes different arguments.
                */
                if(    !re_exec( dirEntry->d_name)    )
#endif
                {   continue ;
                    } 
                } 
            if(    matchDotsLiterally
                && (dirEntry->d_name[0] == '.')
                && (*matchPattern != '.')    )
            {   continue ;
                } 
            if(    *pNumEntries == *pNumAlloc    )
            {   *pNumAlloc += FILE_LIST_BLOCK ;
                *pEntries = (String *) XtRealloc( *pEntries, (*pNumAlloc
                                                          * sizeof( char *))) ;
                } 
            if(    listWithFullPath    )
            {   entryPtr = XtMalloc( dirEntry->d_namlen + dirLen + 1) ;
                strcpy( entryPtr, qualifiedDir) ;
                strcpy( &entryPtr[dirLen], dirEntry->d_name) ;
                }
            else
            {   entryPtr = XtMalloc( dirEntry->d_namlen + 1) ;
                strcpy( entryPtr, dirEntry->d_name) ;
                } 
            /* Now screen entry according to type.
            */
            entryTypeOK = FALSE ;
	    if (fileType == XmFILE_ANY_TYPE) {
		entryTypeOK = TRUE ;
	    } else
            if(    !stat( entryPtr, &statBuf)    )
            {   
                switch(    fileType    )
                {   
                    case XmFILE_REGULAR:
                    {   
                        if(    S_ISREG( statBuf.st_mode)    )
                        {   
                            entryTypeOK = TRUE ;
                            } 
                        break ;
                        } 
                    case XmFILE_DIRECTORY:
                    {   
                        if(    S_ISDIR( statBuf.st_mode)    )
                        {   
                            entryTypeOK = TRUE ;
                            } 
                        break ;
                        } 
                    } 
	    }
            if(    entryTypeOK    )
            {   (*pEntries)[(*pNumEntries)++] = entryPtr ;
                } 
            else
            {   XtFree( entryPtr) ;
                } 
            }
        closedir( dirStream) ;
        }

#ifndef NO_REGEX
    if(    compiledRE    )
    {   /* Use free instead of XtFree since malloc is inside of regex().
        */
        free( compiledRE) ; 
        } 
#endif

    XtFree( fixedMatchPattern) ;
    return ;
    }
/****************************************************************/
static int
#ifdef _NO_PROTO
fileCompare(sp1, sp2)
            String *        sp1 ;
            String *        sp2 ;
#else
fileCompare(
            String *        sp1,
            String *        sp2)
#endif
/****************
 * 
 ****************/
{
/****************/

    return( strcmp( *sp1, *sp2)) ;
    }
/****************************************************************/
static void
#ifdef _NO_PROTO
FileSearchProc( fs, searchData)
            XmFileSelectionBoxWidget fs ;
            XmFileSelectionBoxCallbackStruct * searchData ;
#else
FileSearchProc(
            XmFileSelectionBoxWidget fs,
            XmFileSelectionBoxCallbackStruct * searchData)
#endif
/****************
 * 
 ****************/
{   
            String          dir ;
            String          pattern ;
            Arg             args[3] ;
            int             Index ;
            String *        fileList ;
            unsigned int             numFiles ;
            unsigned int             numAlloc ;
            XmString *      XmStringFileList ;
/****************/

    if(   !XmStringGetLtoR( searchData->dir, XmSTRING_DEFAULT_CHARSET, &dir)  )
    {   return ;
        } 
    if(    !XmStringGetLtoR( searchData->pattern, XmSTRING_DEFAULT_CHARSET,
                                                                 &pattern)    )
    {   XtFree( dir) ;
        return ;
        } 
    fileList = NULL ;
    BuildFileList( dir, pattern, FS_FileTypeMask( fs), 
                                             &fileList, &numFiles, &numAlloc) ;
    if(    fileList  &&  numFiles    )
    {   if(    numFiles > 1    )
        {   qsort( fileList, numFiles, sizeof( char *), fileCompare) ;
            } 
        XmStringFileList = (XmString *) XtMalloc( 
                                                numFiles * sizeof( XmString)) ;
        Index = 0 ;
        while(    Index < numFiles    )
        {   XmStringFileList[Index] = XmStringLtoRCreate( fileList[Index], 
                                                    XmSTRING_DEFAULT_CHARSET) ;
            ++Index ;
            } 
        /* Update the list.
        */
        Index = 0 ;
        XtSetArg( args[Index], XmNitems, XmStringFileList) ; Index++ ;
        XtSetArg( args[Index], XmNitemCount, numFiles) ; Index++ ;
        XtSetValues( SB_List( fs), args, Index) ;

        Index = numFiles ;
        while(    Index--    )
        {   XtFree( fileList[Index]) ;
            } 
        while(    numFiles--    )
        {   XmStringFree( XmStringFileList[numFiles]) ;
            }
        XtFree( XmStringFileList) ;
        }
    else
    {   XtSetArg( args[0], XmNitemCount, 0) ;
        XtSetValues( SB_List( fs), args, 1) ;
        } 
    FS_ListUpdated( fs) = TRUE ;

    XtFree( fileList) ;
    XtFree( pattern) ;
    XtFree( dir) ;
    return ;
    }
/****************************************************************/
static void
#ifdef _NO_PROTO
QualifySearchDataProc( fs, searchData, qualifiedSearchData)
            XmFileSelectionBoxWidget fs ;
            XmFileSelectionBoxCallbackStruct * searchData ;
            XmFileSelectionBoxCallbackStruct * qualifiedSearchData ;
#else
QualifySearchDataProc(
            XmFileSelectionBoxWidget fs,
            XmFileSelectionBoxCallbackStruct * searchData,
            XmFileSelectionBoxCallbackStruct * qualifiedSearchData)
#endif
/****************
 * This routine validates and allocates new copies of all searchData
 *   fields that are required by the DirSearchProc and the FileSearchProc
 *   routines.  The default routines require only the "dir" and "pattern" 
 *   fields to be filled with appropriate qualified non-null XmStrings.
 * Any of the fields of the searchData passed into this routine may be NULL.
 *   Generally, only those fields which signify changes due to a user action
 *   will be passed into this routine.  This data should always override
 *   data derived from other sources.
 * The caller is responsible to free the XmStrings of all (non-null) fields
 *   of the qualifiedSearchData record.
 ****************/
{
            String          valueString ;
            String          patternString ;
            String          dirString ;
            String          maskString ;
            String          qualifiedDir ;
            String          qualifiedPattern ;
            String          qualifiedMask ;
            char *          dirPartPtr ;
            char *          patternPartPtr ;
            unsigned int    qDirLen ;
/****************/

    if(    !searchData->mask
        || !XmStringGetLtoR( searchData->mask, XmSTRING_DEFAULT_CHARSET,
                                                              &maskString)    )
    {   maskString = NULL ;
        } 
        
    if(    !searchData->dir
        || !XmStringGetLtoR( searchData->dir, XmSTRING_DEFAULT_CHARSET,
                                                               &dirString)    )
    {   dirString = NULL ;
        } 

    if(    !searchData->pattern
        || !XmStringGetLtoR( searchData->pattern, XmSTRING_DEFAULT_CHARSET,
                                                           &patternString)    )
    {   patternString = NULL ;
        } 

    if(    !maskString  ||  (dirString  &&  patternString)    )
    {   
        if(    !dirString    )
        {   XmStringGetLtoR( FS_Directory( fs), XmSTRING_DEFAULT_CHARSET,
                                                                  &dirString) ;
            } 
        if(    !patternString    )
        {   XmStringGetLtoR( FS_Pattern( fs), XmSTRING_DEFAULT_CHARSET,
                                                              &patternString) ;
            } 
        QualifyFileSpec( dirString, patternString,
                                            &qualifiedDir, &qualifiedPattern) ;
        } 
    else
    {   patternPartPtr = FindPatternPart( maskString) ;

        if(    patternPartPtr != maskString    )
        {   
            /* To avoid allocating memory and copying part of the mask string,
            *   just stuff '\0' at the '/' which is between the directory part
            *   and the pattern part.  The QualifyFileSpec below does not
            *   require the trailing '/', and it will assure that the resulting
            *   qualifiedDir will have the required trailing '/'.
            */
            *(patternPartPtr - 1) = '\0' ;

            if(    *maskString    )
            {   dirPartPtr = maskString ;
                } 
            else 
            {   /* The '/' that was replaced with '\0' above was the only 
                *    character in the directory specification (it was a
                *    root directory specification), so simply restore it.
                */
                dirPartPtr = "/" ;
                } 
            } 
        else
        {   dirPartPtr = NULL ;
            } 
        if(    dirString    )
        {   dirPartPtr = dirString ;
            } 
        if(    patternString    )
        {   patternPartPtr = patternString ;
            } 
        QualifyFileSpec( dirPartPtr, patternPartPtr,
                                            &qualifiedDir, &qualifiedPattern) ;
        }
    qDirLen = strlen( qualifiedDir) ;
    qualifiedMask = XtMalloc( 1 + qDirLen + strlen( qualifiedPattern)) ;
    strcpy( qualifiedMask, qualifiedDir) ;
    strcpy( &qualifiedMask[qDirLen], qualifiedPattern) ;

    qualifiedSearchData->reason = searchData->reason ;
    qualifiedSearchData->event = searchData->event ;

    if(    searchData->value    )
    {   qualifiedSearchData->value = XmStringCopy( searchData->value) ;
        valueString = NULL ;
        } 
    else
    {   valueString = XmTextGetString( SB_Text( fs)) ;
        qualifiedSearchData->value = XmStringLtoRCreate( valueString,
                                                    XmSTRING_DEFAULT_CHARSET) ;
        } 
    qualifiedSearchData->length = XmStringLength( qualifiedSearchData->value) ;

    qualifiedSearchData->mask = XmStringLtoRCreate( qualifiedMask,
                                                    XmSTRING_DEFAULT_CHARSET) ;
    qualifiedSearchData->mask_length = XmStringLength(
                                                   qualifiedSearchData->mask) ;

    qualifiedSearchData->dir = XmStringLtoRCreate( qualifiedDir,
                                                    XmSTRING_DEFAULT_CHARSET) ;
    qualifiedSearchData->dir_length = XmStringLength(
                                                    qualifiedSearchData->dir) ;

    qualifiedSearchData->pattern = XmStringLtoRCreate( qualifiedPattern,
                                                    XmSTRING_DEFAULT_CHARSET) ;
    qualifiedSearchData->pattern_length = XmStringLength(
                                                qualifiedSearchData->pattern) ;
    XtFree( valueString) ;
    XtFree( qualifiedMask) ;
    XtFree( qualifiedPattern) ;
    XtFree( qualifiedDir) ;
    XtFree( patternString) ;
    XtFree( dirString) ;
    XtFree( maskString) ;
    return ;
    }
/****************************************************************/
static void
#ifdef _NO_PROTO
FileSelectionBoxUpdate( fs, searchData)
            XmFileSelectionBoxWidget fs ;
            XmFileSelectionBoxCallbackStruct * searchData ;
#else
FileSelectionBoxUpdate(
            XmFileSelectionBoxWidget fs,
            XmFileSelectionBoxCallbackStruct * searchData)
#endif

/****************
 * 
 ****************/
{
            Arg             ac[5] ;
            Cardinal        al ;
            int             itemCount ;
            XmString        item ;
            String          textValue ;
            String          dirString ;
            String          maskString ;
            String          patternString ;
            int             len ;
            XmFileSelectionBoxCallbackStruct qualifiedSearchData ;
/****************/

    /* Unmap file list, so if it takes a long time to generate the
    *   list items, the user doesn't wonder what is going on.
    */
    XtSetMappedWhenManaged( SB_List( fs), FALSE) ;
    XFlush( XtDisplay( fs)) ;

    if(    FS_StateFlags( fs) & FS_NO_MATCH    )
    {   XmListDeleteAllItems( SB_List( fs)) ;
        } 
    FS_StateFlags( fs) |= FS_IN_FILE_SEARCH ;

    (*FS_QualifySearchDataProc( fs))( fs, searchData, &qualifiedSearchData) ;

    FS_ListUpdated( fs) = FALSE ;
    FS_DirectoryValid( fs) = FALSE ;

    (*FS_DirSearchProc( fs))( fs, &qualifiedSearchData) ;

    if(    FS_DirectoryValid( fs)    )
    {   
        (*FS_FileSearchProc( fs))( fs, &qualifiedSearchData) ;

        /* Now update the Directory and Pattern resources.
        */
        if(    !XmStringCompare( qualifiedSearchData.dir, FS_Directory( fs))  )
        {   if(    FS_Directory( fs)    )
            {   XmStringFree( FS_Directory( fs)) ;
                } 
            FS_Directory( fs) = XmStringCopy( qualifiedSearchData.dir) ;
            } 

        if(   !XmStringCompare( qualifiedSearchData.pattern, FS_Pattern( fs)) )
        {   if(    FS_Pattern( fs)    )
            {   XmStringFree( FS_Pattern( fs)) ;
                } 
            FS_Pattern( fs) = XmStringCopy( qualifiedSearchData.pattern) ;
            } 
        /* Also update the filter text.
        */
        if(    XmStringGetLtoR( FS_Directory( fs), XmSTRING_DEFAULT_CHARSET,
                                                               &dirString)    )
        {   if(    XmStringGetLtoR( FS_Pattern( fs),
                                 XmSTRING_DEFAULT_CHARSET, &patternString)    )
            {   len = strlen( dirString) ;
                maskString = XtMalloc( len + strlen( patternString) + 1) ;
                strcpy( maskString, dirString) ;
                strcpy( &maskString[len], patternString) ;

                XmTextSetString( FS_FilterText( fs), maskString) ;
                XmTextSetCursorPosition( FS_FilterText( fs),
                                                         strlen( maskString)) ;
                XtFree( maskString) ;
                XtFree( patternString) ;
                } 
            XtFree( dirString) ;
            } 
        } 
    FS_StateFlags( fs) &= ~FS_IN_FILE_SEARCH ;

    al = 0 ;
    XtSetArg( ac[al], XmNitemCount, &itemCount) ; ++al ;
    XtGetValues( SB_List( fs), ac, al) ;

    if(    itemCount    )
    {   FS_StateFlags( fs) &= ~FS_NO_MATCH ;
        } 
    else
    {   FS_StateFlags( fs) |= FS_NO_MATCH ;

        if(    item = FS_NoMatchString( fs)    )
        {   al = 0 ;
            XtSetArg( ac[al], XmNitems, &item) ; ++al ;
            XtSetArg( ac[al], XmNitemCount, 1) ; ++al ;
            XtSetValues( SB_List( fs), ac, al) ;
            } 
        } 
    if(    FS_ListUpdated( fs)    )
    {   
        if(    FS_StateFlags( fs) & FS_NO_MATCH    )
        {   XmTextSetString( SB_Text( fs), NULL) ;
            } 
        else
        {   if(    XmStringGetLtoR( FS_Directory( fs), 
                                     XmSTRING_DEFAULT_CHARSET, &textValue)    )
            {   XmTextSetString( SB_Text( fs), textValue) ;
                XmTextSetCursorPosition( SB_Text( fs), strlen( textValue)) ;
                XtFree( textValue) ;
                } 
            } 
        _XmBulletinBoardSizeUpdate(( XmBulletinBoardWidget) fs) ;
        } 
    XtSetMappedWhenManaged( SB_List( fs), TRUE) ;

    XmStringFree( qualifiedSearchData.value) ;
    XmStringFree( qualifiedSearchData.mask) ;
    XmStringFree( qualifiedSearchData.dir) ;
    XmStringFree( qualifiedSearchData.pattern) ;
    return ;
    }

/****************************************************************/
static void
#ifdef _NO_PROTO
DirSearchProc( fs, searchData)
            XmFileSelectionBoxWidget fs ;
            XmFileSelectionBoxCallbackStruct * searchData ;
#else
DirSearchProc(
            XmFileSelectionBoxWidget fs,
            XmFileSelectionBoxCallbackStruct * searchData)
#endif
/****************
 * This loads the list widget with a directory list based
 *   on the directory specification.
 ****************/
{   
            String          qualifiedDir ;
            Arg             args[10] ;
            int             Index ;
            String *        dirList ;
            unsigned int             numDirs ;
            unsigned int             numAlloc ;
            XmString *      XmStringDirList ;
/****************/

    if(    !XmStringCompare( searchData->dir, FS_Directory( fs))    )
    {   
        /* Directory is different than current, so update dir list.
        */
        if(    !XmStringGetLtoR( searchData->dir, XmSTRING_DEFAULT_CHARSET,
                                                            &qualifiedDir)    )
        {   XBell( XtDisplay( fs), 0) ;
            return ;
            } 

        dirList = NULL ;
        GetDirEntries( qualifiedDir, "*", XmFILE_DIRECTORY, FALSE, TRUE,
                                               &dirList, &numDirs, &numAlloc) ;
        if(    !numDirs    )
        {   /* Directory list is empty, so have attempted to go 
            *   into a directory without permissions.  Don't do it!
            */
            XBell( XtDisplay( fs), 0) ;
            return ;
            } 
        if(    numDirs > 1    )
        {   qsort( dirList, numDirs, sizeof( char *), fileCompare) ;
            } 
        XmStringDirList = (XmString *) XtMalloc( numDirs * sizeof( XmString)) ;

        Index = 0 ;
        while(    Index < numDirs    )
        {   XmStringDirList[Index] = XmStringLtoRCreate( dirList[Index],
                                                    XmSTRING_DEFAULT_CHARSET) ;
            ++Index ;
            } 
        /* Update the list.  */
        Index = 0;
        XtSetArg( args[Index], XmNitems, XmStringDirList) ; Index++ ;
        XtSetArg( args[Index], XmNitemCount, numDirs) ; Index++ ;
        XtSetArg( args[Index], XmNtopItemPosition, 1) ; Index++ ;
        XtSetValues( FS_DirList( fs), args, Index);

        XmListSelectPos( FS_DirList( fs), 1, FALSE) ;
        FS_DirListSelectedItemPosition( fs) = 1 ;

        Index = numDirs ;
        while(    Index--    )
        {   XtFree( dirList[Index]) ;
            } 
        XtFree( dirList) ;
    
        while(    numDirs--    )
        {
            XmStringFree( XmStringDirList[numDirs]) ;
            }
        XtFree( XmStringDirList) ;
        XtFree( (char *) qualifiedDir) ;
        FS_ListUpdated( fs) = TRUE ;
        }
    FS_DirectoryValid( fs) = TRUE ;
    return ;
    }
/****************************************************************/
static void
#ifdef _NO_PROTO
ListCallback( wid, client_data, call_data)
            Widget          wid ;
            XtPointer       client_data ;
            XtPointer       call_data ;
#else
ListCallback(
            Widget          wid,
            XtPointer       client_data,
            XtPointer       call_data)
#endif
/****************           ARGSUSED
 * Process callback from either List of the File Selection Box.
 ****************/
{   
            XmListCallbackStruct * callback ;
            XmFileSelectionBoxWidget fsb ;
            XmGadgetClass   gadget_class ;
            XmGadget        dbutton ;
            XmFileSelectionBoxCallbackStruct change_data ;
            XmFileSelectionBoxCallbackStruct qualified_change_data ;
            String          textValue ;
            String          dirString ;
            String          maskString ;
            String          patternString ;
            int             len ;
/****************/

    callback = (XmListCallbackStruct *) call_data ;
    fsb = (XmFileSelectionBoxWidget) client_data ;

    switch(    callback->reason    )
    {   
        case XmCR_BROWSE_SELECT:
        case XmCR_SINGLE_SELECT:
        {   
            if(    wid == FS_DirList( fsb)    )
            {   
                FS_DirListSelectedItemPosition( fsb)
                                                    = callback->item_position ;
                change_data.event  = NULL ;
                change_data.reason = XmCR_NONE ;
                change_data.value = NULL ;
                change_data.length = 0 ;
                textValue = XmTextGetString( FS_FilterText( fsb)) ;
                change_data.mask = XmStringLtoRCreate( textValue,
                                                    XmSTRING_DEFAULT_CHARSET) ;
                change_data.mask_length = XmStringLength( change_data.mask) ;
                change_data.dir = XmStringCopy( callback->item) ;
                change_data.dir_length = XmStringLength( change_data.dir) ;
                change_data.pattern = NULL ;
                change_data.pattern_length = 0 ;

                /* Qualify and then update the filter text.
                */
                (*FS_QualifySearchDataProc( fsb))( fsb, &change_data, 
                                                      &qualified_change_data) ;

                if(    XmStringGetLtoR( qualified_change_data.dir,
                                     XmSTRING_DEFAULT_CHARSET, &dirString)    )
                {   if(    XmStringGetLtoR( qualified_change_data.pattern,
                                    XmSTRING_DEFAULT_CHARSET, &patternString)    )
                    {   len = strlen( dirString) ;
                        maskString = XtMalloc( len
                                                 + strlen( patternString) + 1) ;
                        strcpy( maskString, dirString) ;
                        strcpy( &maskString[len], patternString) ;
                        XmTextSetString( FS_FilterText( fsb), maskString) ;
                        XmTextSetCursorPosition( FS_FilterText( fsb),
                                                         strlen( maskString)) ;
                        XtFree( maskString) ;
                        XtFree( patternString) ;
                        } 
                    XtFree( dirString) ;
                    } 
                XmStringFree( qualified_change_data.pattern) ;
                XmStringFree( qualified_change_data.dir) ;
                XmStringFree( qualified_change_data.mask) ;
                XmStringFree( qualified_change_data.value) ;
                XmStringFree( change_data.mask) ;
                XmStringFree( change_data.dir) ;
                XtFree( textValue) ;
                } 
            else    /* wid is File List. */
            {   
                if(    FS_StateFlags( fsb) & FS_NO_MATCH    )
                {   
                    XmListDeselectPos( SB_List( fsb), 1) ;
                    break ;
                    } 
                SB_ListSelectedItemPosition( fsb) = callback->item_position ;
                if(    XmStringGetLtoR( callback->item, 
                                     XmSTRING_DEFAULT_CHARSET, &textValue)    )
                {   XmTextSetString( SB_Text( fsb), textValue) ;
                    XmTextSetCursorPosition( SB_Text( fsb),
                                                          strlen( textValue)) ;
                    } 
                } 
            break ;
            }
        case XmCR_DEFAULT_ACTION:
        {   
            dbutton = (XmGadget) BB_DynamicDefaultButton( fsb) ;
            /* Catch only double-click default action here.
            *  Key press events are handled through the ParentProcess routine.
            */
            if(    (callback->event->type != KeyPress)
                && dbutton  &&  XtIsManaged( dbutton)
                && XtIsSensitive( dbutton)  &&  XmIsGadget( dbutton)    )
            {   
                gadget_class = (XmGadgetClass) dbutton->object.widget_class ;
                if(    gadget_class->gadget_class.arm_and_activate    )
                {   (*(gadget_class->gadget_class.arm_and_activate))(
                                                      (Widget) dbutton, NULL) ;
                    } 
                }
            break ;
            } 
        default:
        {   break ;
            } 
        }
    return ;
    }
/****************************************************************/
static void
#ifdef _NO_PROTO
DoSearchFromSetValues( data, id)
            FS_DoSearchCallbackRec * data ;
            XtIntervalId *  id ;
#else
DoSearchFromSetValues(
            FS_DoSearchCallbackRec * data,
            XtIntervalId *  id)
#endif
/****************           ARGSUSED
 * This routine is the routine that causes the file
 *   search proc to be called right after SetValues.
 *   Calling the search proc during SetValues can cause
 *   an indirect recursion of SetValues --a nasty thing.
 * The pointers associated with FS_DoSearchCallbackRec are assumed to
 *   have been allocated during the SetValues call and are deallocated
 *   in this routine.
 ****************/
{   
            XmFileSelectionBoxWidget fsb ;
            String          real_string ;
            XmFileSelectionBoxCallbackStruct * searchData ;
/****************/

    fsb = data->fsb ;
    searchData = &data->change_data ;

    FileSelectionBoxUpdate( fsb, searchData) ;

    XmStringFree( searchData->value) ;
    XmStringFree( searchData->mask) ;
    XmStringFree( searchData->dir) ;
    XmStringFree( searchData->pattern) ;
    XtFree( data) ;
    return ;
    }
/****************************************************************/
static Boolean
#ifdef _NO_PROTO
SetValues( current, request, new)
            XmFileSelectionBoxWidget current ;
            XmFileSelectionBoxWidget request ;
            XmFileSelectionBoxWidget new ;
#else
SetValues(
            XmFileSelectionBoxWidget current,
            XmFileSelectionBoxWidget request,
            XmFileSelectionBoxWidget new)
#endif
/****************
 * This routine detects differences in two versions
 *   of a widget, when a difference is found the
 *   appropriate action is taken.
 ****************/
{
            Arg             args[10] ;
            int             n ;
            String          newString ;
            FS_DoSearchCallbackRec * doSearchData = NULL ;
/****************/

    BB_InSetValues( new) = TRUE ;

    if(    FS_DirListLabelString( current) != FS_DirListLabelString( new)    )
    {   
        n = 0 ;
        XtSetArg( args[n], XmNlabelString, FS_DirListLabelString( new)) ; n++ ;
        XtSetArg( args[n], XmNlabelType, XmSTRING) ; n++ ;
        XtSetValues( FS_DirListLabel( new), args, n) ;
        FS_DirListLabelString( new) = NULL ;
        }
    if(    FS_FilterLabelString( current) != FS_FilterLabelString( new)    )
    {   
        n = 0 ;
        XtSetArg( args[n], XmNlabelString, FS_FilterLabelString( new)) ; n++ ;
        XtSetArg( args[n], XmNlabelType, XmSTRING) ; n++ ;
        XtSetValues( FS_FilterLabel( new), args, n) ;
        FS_FilterLabelString( new) = NULL ;
        }
    n = 0 ;
    if(    SB_ListVisibleItemCount( current)
                                          != SB_ListVisibleItemCount( new)    )
    {   XtSetArg( args[n], XmNvisibleItemCount, 
                                         SB_ListVisibleItemCount( new)) ; ++n ;
        } 
    if(    FS_DirListItems( new)    )
    {   
        XtSetArg( args[n], XmNitems, FS_DirListItems( new)) ; ++n ;
        FS_DirListItems( new) = NULL ;
        } 
    if(    FS_DirListItemCount( new) != UNSPECIFIED    )
    {   
        XtSetArg( args[n], XmNitemCount, FS_DirListItemCount( new)) ; ++n ;
        FS_DirListItemCount( new) = UNSPECIFIED ;
        } 

    if(    n    )
    {   XtSetValues( FS_DirList( new), args, n) ;
        } 

    if(    (SB_TextColumns( new) != SB_TextColumns( current))
        && FS_FilterText( new)    )
    {   
        n = 0 ;
        XtSetArg( args[n], XmNcolumns, SB_TextColumns( new)) ; ++n ;
        XtSetValues( FS_FilterText( new), args, n) ;
        }
    if(    FS_NoMatchString( new) != FS_NoMatchString( current)    )
    {   XmStringFree( FS_NoMatchString( current)) ;
        FS_NoMatchString( new) = XmStringCopy( FS_NoMatchString( new)) ;
        } 
    if(    !FS_QualifySearchDataProc( new)    )
    {   FS_QualifySearchDataProc( new) = QualifySearchDataProc ;
        } 
    if(    !FS_DirSearchProc( new)    )
    {   FS_DirSearchProc( new) = DirSearchProc ;
        } 
    if(    !FS_FileSearchProc( new)    )
    {   FS_FileSearchProc( new) = FileSearchProc ;
        } 
    /* The XmNdirSpec resource will be loaded into the Text widget by
    *   the Selection Box (superclass) SetValues routine.  It will be 
    *   picked-up there by the XmNqualifySearchDataProc routine to fill
    *   in the value field of the search data.
    */
    if(    FS_DirMask( new) != FS_DirMask( current)    )
    {   
        if(    FS_StateFlags( new) & FS_IN_FILE_SEARCH    )
        {   
            if(    FS_FilterText( new)    )
            {   
                if(    !XmStringGetLtoR( FS_DirMask( new),
                                     XmSTRING_DEFAULT_CHARSET, &newString)    )
                {   newString = NULL ;
                    } 
                /* Should do this stuff entirely with XmStrings when the text
                *   widget supports it.
                */
                XmTextSetString( FS_FilterText( new), newString) ;
                if(    newString    )
                {   XmTextSetCursorPosition( FS_FilterText( new),
                                                          strlen( newString)) ;
                    } 
                XtFree( newString) ;
                }
            } 
        else
        {   if(    !doSearchData    )
            {   doSearchData = (FS_DoSearchCallbackRec *) XtCalloc( 1,
                                             sizeof( FS_DoSearchCallbackRec)) ;
                } 
            doSearchData->change_data.mask = XmStringCopy( 
                                                        FS_DirMask( request)) ;
            doSearchData->change_data.mask_length = XmStringLength( 
                                              doSearchData->change_data.mask) ;
            } 
        FS_DirMask( new) = (XmString) UNSPECIFIED ;
        } 
    if(    FS_Directory( current) != FS_Directory( new)    )
    {   
        if(    FS_StateFlags( new) & FS_IN_FILE_SEARCH    )
        {   
            FS_Directory( new) = XmStringCopy( FS_Directory( request)) ;
            XmStringFree( FS_Directory( current)) ;
            } 
        else
        {   if(    !doSearchData    )
            {   doSearchData = (FS_DoSearchCallbackRec *) XtCalloc( 1,
                                             sizeof( FS_DoSearchCallbackRec)) ;
                } 
            doSearchData->change_data.dir = XmStringCopy( 
                                                      FS_Directory( request)) ;
            doSearchData->change_data.dir_length = XmStringLength( 
                                               doSearchData->change_data.dir) ;
            /* The resource will be set to the new value after the Search
            *   routines have been called for validation.
            */
            FS_Directory( new) = FS_Directory( current) ;
            }
        }
    if(    FS_Pattern( current) != FS_Pattern( new)    )
    {   
        if(    FS_StateFlags( new) & FS_IN_FILE_SEARCH    )
        {   
            FS_Pattern( new) = XmStringCopy( FS_Pattern( request)) ;
            XmStringFree( FS_Pattern( current)) ;
            } 
        else
        {   if(    !doSearchData    )
            {   doSearchData = (FS_DoSearchCallbackRec *) XtCalloc( 1,
                                             sizeof( FS_DoSearchCallbackRec)) ;
                } 
            doSearchData->change_data.pattern = XmStringCopy( 
                                                     FS_Pattern( request)) ;
            doSearchData->change_data.pattern_length = XmStringLength( 
                                            doSearchData->change_data.pattern) ;
            /* The resource will be set to the new value after the Search
            *   routines have been called for validation.
            */
            FS_Pattern( new) = FS_Pattern( current) ;
            }
        }
    if(    FS_FileTypeMask( new) != FS_FileTypeMask( current)    )
    {   
        if(    !(FS_StateFlags( new) & FS_IN_FILE_SEARCH)
            && !doSearchData    )
        {   
            doSearchData = (FS_DoSearchCallbackRec *) XtCalloc( 1,
                                             sizeof( FS_DoSearchCallbackRec)) ;
            } 
        }
    if(    doSearchData    )
    {   /* remove the nasty timeout, since there is already a 
	   FS_IN_FILE_SEARCH flag, there shouldn't be any recursivity.
	   Morevover, the AES says what to do in case of a dirSearchProc
	   which query the user again, so ... */
        doSearchData->fsb = new ;
        doSearchData->change_data.reason = XmCR_NONE ;
        DoSearchFromSetValues(doSearchData, NULL) ;
        }

    BB_InSetValues( new) = FALSE ;

    if(    XtClass( new) == xmFileSelectionBoxWidgetClass    )
    {   
        _XmBulletinBoardSizeUpdate((XmBulletinBoardWidget) new) ;
        }
    return( FALSE) ;
    }
/****************************************************************/
static void
#ifdef _NO_PROTO
BuildFileList( dirPath, pattern, typeMask, pEntries, pNumEntries, pNumAlloc)
            String          dirPath ;
            String          pattern ;
            unsigned char   typeMask ;
            String * *      pEntries ;      /* Cannot be NULL. */
            unsigned int *  pNumEntries ;   /* Cannot be NULL. */
            unsigned int *  pNumAlloc ;     /* Cannot be NULL. */
#else
BuildFileList(
            String          dirPath,
            String          pattern,
            unsigned char   typeMask,
            String * *      pEntries,       /* Cannot be NULL. */
            unsigned int *  pNumEntries,    /* Cannot be NULL. */
            unsigned int *  pNumAlloc)      /* Cannot be NULL. */
#endif
/****************
 * The 'dirPath' parameter must be a qualified directory path (no match
 *   characters, with '/' at end).
 * The 'pattern' parameter must be valid as a suffix to dirPath.
 ****************/
{   
            String          qualifiedDir ;
            String          nextPatternPtr ;
            String *        localEntries ;
            unsigned int    localNumEntries ;
            unsigned int    localNumAlloc ;
            unsigned int    entryIndex ;
/****************/

    qualifiedDir = GetQualifiedDir( dirPath) ;
    nextPatternPtr = pattern ;
    while(    *nextPatternPtr  &&  (*nextPatternPtr != '/')    )
    {   ++nextPatternPtr ;
        } 
    if(    !*nextPatternPtr    )
    {   /* At lowest level directory, so simply return matching entries.
        */
        GetDirEntries( qualifiedDir, pattern, typeMask, FALSE, TRUE, 
                                            pEntries, pNumEntries, pNumAlloc) ;
        }
    else
    {   ++nextPatternPtr ;               /* Move past '/' character.*/
        localEntries = NULL ;
        GetDirEntries( qualifiedDir, pattern, XmFILE_DIRECTORY, TRUE, TRUE, 
                             &localEntries, &localNumEntries, &localNumAlloc) ;
        entryIndex = 0 ;
        while(    entryIndex < localNumEntries    )
        {   BuildFileList( localEntries[entryIndex], nextPatternPtr, 
                                  typeMask, pEntries, pNumEntries, pNumAlloc) ;
            XtFree( localEntries[entryIndex]) ;
            ++entryIndex ;
            } 
        XtFree( localEntries) ;
        }
    XtFree( qualifiedDir) ;
    return ;
    }
/****************************************************************/
void
#ifdef _NO_PROTO
_XmFileSelectionBoxGetDirectory( fs, resource, value)
            XmFileSelectionBoxWidget fs ;
            XrmQuark        resource ;
            XtArgVal *      value ;
#else
_XmFileSelectionBoxGetDirectory(
            XmFileSelectionBoxWidget fs,
            XrmQuark        resource,
            XtArgVal *      value)
#endif
/****************           ARGSUSED
 * This does get values hook magic to keep the
 * user happy.
 ****************/
{
  XmString        data ;
  Arg             al[1] ;
/****************/
  
  data = XmStringCopy(FS_Directory(fs));
  *value = (XtArgVal) data ;

    return ;
    }
/****************************************************************/
void
#ifdef _NO_PROTO
_XmFileSelectionBoxGetNoMatchString( fs, resource, value)
            XmFileSelectionBoxWidget fs ;
            XrmQuark        resource ;
            XtArgVal *      value ;
#else
_XmFileSelectionBoxGetNoMatchString(
            XmFileSelectionBoxWidget fs,
            XrmQuark        resource,
            XtArgVal *      value)
#endif
/****************           ARGSUSED
 * This does get values hook magic to keep the
 * user happy.
 ****************/
{
  XmString        data ;
  Arg             al[1] ;
/****************/
  
  data = XmStringCopy(FS_NoMatchString(fs));
  *value = (XtArgVal) data ;

    return ;
    }
/****************************************************************/
void
#ifdef _NO_PROTO
_XmFileSelectionBoxGetPattern( fs, resource, value)
            XmFileSelectionBoxWidget fs ;
            XrmQuark        resource ;
            XtArgVal *      value ;
#else
_XmFileSelectionBoxGetPattern(
            XmFileSelectionBoxWidget fs,
            XrmQuark        resource,
            XtArgVal *      value)
#endif
/****************           ARGSUSED
 * This does get values hook magic to keep the
 * user happy.
 ****************/
{
  XmString        data ;
  Arg             al[1] ;
/****************/
  
  data = XmStringCopy(FS_Pattern(fs));
  *value = (XtArgVal) data ;

    return ;
    }
/****************************************************************/
void
#ifdef _NO_PROTO
_XmFileSelectionBoxGetFilterLabelString( fs, resource, value)
            XmFileSelectionBoxWidget fs ;
            XrmQuark        resource ;
            XtArgVal *      value ;
#else
_XmFileSelectionBoxGetFilterLabelString(
            XmFileSelectionBoxWidget fs,
            XrmQuark        resource,
            XtArgVal *      value)
#endif
/****************           ARGSUSED
 * This does get values hook magic to keep the
 * user happy.
 ****************/
{
            XmString        data ;
            Arg             al[1] ;
/****************/

    XtSetArg( al[0], XmNlabelString, &data) ;
    XtGetValues( FS_FilterLabel( fs), al, 1) ;
    *value = (XtArgVal) data ;

    return ;
    }
/****************************************************************/
void
#ifdef _NO_PROTO
_XmFileSelectionBoxGetDirListLabelString( fs, resource, value)
            XmFileSelectionBoxWidget fs ;
            XrmQuark        resource ;
            XtArgVal *      value ;
#else
_XmFileSelectionBoxGetDirListLabelString(
            XmFileSelectionBoxWidget fs,
            XrmQuark        resource,
            XtArgVal *      value)
#endif
/****************           ARGSUSED
 * This does get values hook magic to keep the user happy.
 ****************/
{
            XmString        data ;
            Arg             al[1] ;
/****************/

    XtSetArg( al[0], XmNlabelString, &data) ;
    XtGetValues( FS_DirListLabel( fs), al, 1) ;
    *value = (XtArgVal) data ;

    return ;
    }
/****************************************************************/
void
#ifdef _NO_PROTO
_XmFileSelectionBoxGetDirListItems( fs, resource, value)
            XmFileSelectionBoxWidget fs ;
            XrmQuark        resource ;
            XtArgVal *      value ;
#else
_XmFileSelectionBoxGetDirListItems(
            XmFileSelectionBoxWidget fs,
            XrmQuark        resource,
            XtArgVal *      value)
#endif
/****************           ARGSUSED
 * This does get values hook magic to keep the user happy.
 ****************/
{
            XmString        data ;
            Arg             al[1] ;
/****************/

    XtSetArg( al[0], XmNitems, &data) ;
    XtGetValues( FS_DirList( fs), al, 1) ;
    *value = (XtArgVal) data ;

    return ;
    }
/****************************************************************/
void
#ifdef _NO_PROTO
_XmFileSelectionBoxGetDirListItemCount( fs, resource, value)
            XmFileSelectionBoxWidget fs ;
            XrmQuark        resource ;
            XtArgVal *      value ;
#else
_XmFileSelectionBoxGetDirListItemCount(
            XmFileSelectionBoxWidget fs,
            XrmQuark        resource,
            XtArgVal *      value)
#endif
/****************           ARGSUSED
 * This does get values hook magic to keep the user happy.
 ****************/
{
            XmString        data ;
            Arg             al[1] ;
/****************/

    XtSetArg( al[0], XmNitemCount, &data) ;
    XtGetValues( FS_DirList( fs), al, 1) ;
    *value = (XtArgVal) data ;

    return ;
    }
/****************************************************************/
void
#ifdef _NO_PROTO
_XmFileSelectionBoxGetListItems( fs, resource, value)
            XmFileSelectionBoxWidget fs ;
            XrmQuark        resource ;
            XtArgVal *      value ;
#else
_XmFileSelectionBoxGetListItems(
            XmFileSelectionBoxWidget fs,
            XrmQuark        resource,
            XtArgVal *      value)
#endif
/****************           ARGSUSED
 * This does get values hook magic to keep the user happy.
 ****************/
{
            XmString        data ;
            Arg             al[1] ;
/****************/

    if(    FS_StateFlags( fs) & FS_NO_MATCH    )
    {   
        *value = (XtArgVal) NULL ;
        } 
    else
    {   XtSetArg( al[0], XmNitems, &data) ;
        XtGetValues( SB_List( fs), al, 1) ;
        *value = (XtArgVal) data ;
        } 
    return ;
    }
/****************************************************************/
void
#ifdef _NO_PROTO
_XmFileSelectionBoxGetListItemCount( fs, resource, value)
            XmFileSelectionBoxWidget fs ;
            XrmQuark        resource ;
            XtArgVal *      value ;
#else
_XmFileSelectionBoxGetListItemCount(
            XmFileSelectionBoxWidget fs,
            XrmQuark        resource,
            XtArgVal *      value)
#endif
/****************           ARGSUSED
 * This does get values hook magic to keep the user happy.
 ****************/
{
            XmString        data ;
            Arg             al[1] ;
/****************/

    if(    FS_StateFlags( fs) & FS_NO_MATCH    )
    {   
        *value = (XtArgVal) 0 ;
        } 
    else
    {   XtSetArg( al[0], XmNitemCount, &data) ;
        XtGetValues( SB_List( fs), al, 1) ;
        *value = (XtArgVal) data ;
        } 

    return ;
    }
/****************************************************************/
void
#ifdef _NO_PROTO
_XmFileSelectionBoxGetDirMask( fs, resource, value)
            XmFileSelectionBoxWidget fs ;
            XrmQuark        resource ;
            XtArgVal *      value ;
#else
_XmFileSelectionBoxGetDirMask(
            XmFileSelectionBoxWidget fs,
            XrmQuark        resource,
            XtArgVal *      value)
#endif
/****************           ARGSUSED
 * This does get values hook magic to keep the
 * user happy.
 ****************/
{   
            String          filterText ;
            XmString        data ;
/****************/

    filterText = XmTextGetString( FS_FilterText(fs)) ;
    data = XmStringLtoRCreate( filterText, XmSTRING_DEFAULT_CHARSET) ;
    *value = (XtArgVal) data ;
    XtFree( filterText) ; 

    return ;
    }
/****************************************************************/
static Widget
#ifdef _NO_PROTO
GetActiveText( fsb, event)
            XmFileSelectionBoxWidget fsb ;
            XKeyPressedEvent * event ;
#else
GetActiveText(
            XmFileSelectionBoxWidget fsb,
            XKeyPressedEvent * event)
#endif
/****************
 * 
 ****************/
{
            Widget          activeChild = NULL ;
/****************/

    if(    _XmGetFocusPolicy( (Widget)fsb) == XmEXPLICIT    )
    {   
        if(    (fsb->manager.active_child == SB_Text( fsb))
            || (fsb->manager.active_child == FS_FilterText( fsb))    )
        {   
            activeChild = fsb->manager.active_child ;
            } 
        } 
    else
    {   
#ifdef TEXT_IS_GADGET
        activeChild = _XmInputInGadget( (CompositeWidget) fsb, 
                                                          event->x, event->y) ;
        if(    (activeChild != SB_Text( fsb))
            && (activeChild != FS_FilterText( fsb))    )
        {   
            activeChild = NULL ;
            } 
#else /* TEXT_IS_GADGET */
        if(    SB_Text( fsb)
            && (XtWindow( SB_Text( fsb)) == event->window)   )
        {   
            activeChild = SB_Text( fsb) ;
            } 
        else
        {   if(    FS_FilterText( fsb)
                && (XtWindow( FS_FilterText( fsb)) == event->window)   )
            {   
                activeChild = FS_FilterText( fsb) ;
                } 
            } 
#endif /* TEXT_IS_GADGET */
        } 
    return( activeChild) ;
    }

/****************************************************************/
void
#ifdef _NO_PROTO
_XmFileSelectionBoxUpOrDown( fsb, event, argv, argc)
            XmFileSelectionBoxWidget fsb ;
            XEvent *        event ;
            char * *        argv ;
            int             argc ;
#else
_XmFileSelectionBoxUpOrDown(
            XmFileSelectionBoxWidget fsb,
            XEvent *        event,
            char * *        argv,
            int             argc)
#endif
/****************           ARGSUSED
 * 
 ****************/
{   
            int	            visible ;
            int	            top ;
            int	            key_pressed ;
            Widget	    list ;
            int	*           position ;
            int	            count ;
            Widget          activeChild ;
            Arg             av[5] ;
            Cardinal        ac ;
/****************/

    if(    !(activeChild = GetActiveText((XmFileSelectionBoxWidget) fsb, 
					 (XKeyPressedEvent *)event))    )
    {   return ;
        } 
    if(    activeChild == SB_Text( fsb)    )
    {   
        if(    FS_StateFlags( fsb) & FS_NO_MATCH    )
        {   return ;
            } 
        list = SB_List( fsb) ;
        position = &SB_ListSelectedItemPosition( fsb) ;
        } 
    else /* activeChild == FS_FilterText( fsb) */
    {   list = fsb->file_selection_box.dir_list ;
        position = &FS_DirListSelectedItemPosition( fsb) ;
        } 
    if(    !list    )
    {   return ;
        } 
    ac = 0 ;
    XtSetArg( av[ac], XmNitemCount, &count) ; ++ac ;
    XtSetArg( av[ac], XmNtopItemPosition, &top) ; ++ac ;
    XtSetArg( av[ac], XmNvisibleItemCount, &visible) ; ++ac ;
    XtGetValues( (XmListWidget) list, av, ac) ;

    if(    !count    )
    {   return ;
        } 
    key_pressed = atoi( *argv) ;

    if(    *position == 0    )
    {   /*  No selection, so select first item.
        */
        XmListSelectPos( list, ++*position, True) ;
        } 
    else
    {   if(    !key_pressed && (*position > 1)    )
        {   /*  up  */
            XmListDeselectPos( list, *position) ;
            XmListSelectPos( list, --*position, True) ;
            }
        else
        {   if(    (key_pressed == 1) && (*position < count)    )
            {   /*  down  */
                XmListDeselectPos( list, *position) ;
                XmListSelectPos( list, ++*position, True) ;
                } 
            else
            {   if(    key_pressed == 2    )
                {   /*  home  */
                    XmListDeselectPos( list, *position) ;
                    *position = 1 ;
                    XmListSelectPos( list, *position, True) ;
                    } 
                else
                {   if(    key_pressed == 3    )
                    {   /*  end  */
                        XmListDeselectPos( list, *position) ;
                        *position = count ;
                        XmListSelectPos( list, *position, True) ;
                        } 
                    } 
                } 
            }
        } 
    if(    top > *position    )
    {   XmListSetPos( list, *position) ;
        } 
    else
    {   if(    (top + visible) <= *position    )
        {   XmListSetBottomPos( list, *position) ;
            } 
        } 
    return ;
    }
/****************************************************************/
void
#ifdef _NO_PROTO
_XmFileSelectionBoxRestore( fsb, event, argv, argc)
            XmFileSelectionBoxWidget fsb ;
            XEvent *        event ;
            char * *        argv ;
            int             argc ;
#else
_XmFileSelectionBoxRestore(
            XmFileSelectionBoxWidget fsb,
            XEvent *        event,
            char * *        argv,
            int             argc)
#endif
/****************
 * 
 ****************/
{   
            String          itemString ;
            String          dir ;
            String          mask ;
            int             dirLen ;
            int             maskLen ;
            Widget          activeChild ;
/****************/

    if(    !(activeChild = GetActiveText((XmFileSelectionBoxWidget) fsb, 
					 (XKeyPressedEvent *)event))    )
    {   return ;
        } 
    if(    activeChild == SB_Text( fsb)    )
    {   _XmSelectionBoxRestore((XmSelectionBoxWidget) fsb, event, argv, argc) ;
        } 
    else /* activeChild == FS_FilterText( fsb) */
    {   /* Should do this stuff entirely with XmStrings when the text
        *   widget supports it.
        */
        if(    XmStringGetLtoR( FS_Directory( fsb), XmSTRING_DEFAULT_CHARSET,
                                                                     &dir)    )
        {   dirLen = strlen( dir) ;
            if(    XmStringGetLtoR( FS_Pattern( fsb), XmSTRING_DEFAULT_CHARSET,
                                                                    &mask)    )
            {   maskLen = strlen( mask) ;
                itemString = XtMalloc( dirLen + maskLen + 1) ;
                strcpy( itemString, dir) ;
                strcpy( &itemString[dirLen], mask) ;

                XmTextSetString( FS_FilterText( fsb), itemString) ;
                XmTextSetCursorPosition( FS_FilterText( fsb),
                                                          (dirLen + maskLen)) ;

                XtFree( itemString) ;
                XtFree( mask) ;
                } 
            XtFree( dir) ;
            }
        } 
    return ;
    }
/****************************************************************/
void
#ifdef _NO_PROTO
_XmFileSelectionBoxFocusMoved( wid, client_data, call_data)
            Widget          wid ;
            XtPointer       client_data ;
            XmFocusMovedCallbackStruct * call_data ;
#else
_XmFileSelectionBoxFocusMoved(
            Widget          wid,
            XtPointer       client_data,
            XmFocusMovedCallbackStruct * call_data)
#endif
/****************           ARGSUSED
 * 
 ****************/
{            
            Widget          ancestor ;
/****************/

    if(    !call_data->cont    )
    {   /* Preceding callback routine wants focus-moved processing
        *   to be discontinued.
        */
        return ;
        } 

    if(    call_data->new
        && (   (call_data->new == FS_FilterText( client_data))
            || (call_data->new == FS_DirList( client_data)))
        && XtIsManaged( SB_ApplyButton( client_data))    )
    {   
        BB_DefaultButton( client_data) = SB_ApplyButton( client_data) ;
        }
    else
    {   BB_DefaultButton( client_data) = SB_OkButton( client_data) ;
        }

    _XmBulletinBoardFocusMoved( wid, client_data, call_data) ;

    /* Since the focus-moved callback of an ancestor bulletin board may
    *   have already been called, we must make sure that it knows that
    *   we have changed our default button.  So, walk the hierarchy and
    *   synchronize the dynamic default button of all ancestor bulletin 
    *   board widgets.
    */
    if(    call_data->cont    )
    {   
        ancestor = XtParent( (Widget) client_data) ;
        
        while(    ancestor  &&  !XtIsShell( ancestor)    )
        {   
            if(    XmIsBulletinBoard( ancestor)    )
            {   
                if(    BB_DynamicDefaultButton( ancestor)
                    && BB_DynamicDefaultButton( client_data)    )
                {   
                    _XmBulletinBoardSetDynDefaultButton( ancestor, 
                                       BB_DynamicDefaultButton( client_data)) ;
                    } 
                } 
            ancestor = XtParent( ancestor) ;
            } 
        } 
    return ;
    }
/****************************************************************/
static void
#ifdef _NO_PROTO
FileSelectionPB( wid, which_button, callback)
            Widget          wid ;
            XtPointer       which_button ;
            XmAnyCallbackStruct * callback ;
#else
FileSelectionPB(
            Widget          wid,
            XtPointer       which_button,
            XmAnyCallbackStruct * callback)
#endif
/****************
 * This is the procedure which does all of the button
 *   callback magic.
 ****************/
{   
            XmFileSelectionBoxWidget fs ;
            XmFileSelectionBoxCallbackStruct searchData ;
            XmFileSelectionBoxCallbackStruct qualifiedSearchData ;
            Boolean         match = True ;
            String          text_value ;
            Boolean         allowUnmanage = FALSE ;
/****************/

    fs = (XmFileSelectionBoxWidget) XtParent( wid) ;

    searchData.event = callback->event ;
    searchData.value = NULL ;
    searchData.length = 0 ;
    searchData.mask = NULL ;
    searchData.mask_length = 0 ;
    searchData.dir = NULL ;
    searchData.dir_length = 0 ;
    searchData.pattern = NULL ;
    searchData.pattern_length = 0 ;
                
    if(    ((int) which_button) == XmDIALOG_APPLY_BUTTON    )
    {   
        if(    FS_FilterText( fs)
            && (text_value = XmTextGetString( FS_FilterText( fs)))    )
        {   
            searchData.mask = XmStringLtoRCreate( text_value, 
                                                    XmSTRING_DEFAULT_CHARSET) ;
            searchData.mask_length = XmStringLength( searchData.mask) ;
            XtFree( text_value) ;
            } 
        searchData.reason = XmCR_NONE ;

        FileSelectionBoxUpdate( fs, &searchData) ;

        XmStringFree( searchData.mask) ;
        searchData.mask = NULL ;
        searchData.mask_length = 0 ;
        }

    /* Use the XmNqualifySearchDataProc routine to fill in all fields of the
    *   callback data record.
    */
    (*FS_QualifySearchDataProc( fs))( fs, &searchData, &qualifiedSearchData) ;

    switch(    (int) which_button    )
    {   
        case XmDIALOG_OK_BUTTON:
        {   
            if(    SB_MustMatch( fs)    )
            {   
                match = XmListItemExists( SB_List( fs),
                                                   qualifiedSearchData.value) ;
                }
            if(    !match    )
            {   
                qualifiedSearchData.reason = XmCR_NO_MATCH ;
                XtCallCallbackList ((Widget) fs,
                   fs->selection_box.no_match_callback, &qualifiedSearchData) ;

                }
            else
            {   qualifiedSearchData.reason = XmCR_OK ;
                XtCallCallbackList ((Widget) fs,
                         fs->selection_box.ok_callback, &qualifiedSearchData) ;

                }
            allowUnmanage = TRUE ;
            break ;
            }
        case XmDIALOG_APPLY_BUTTON:
        {   
            qualifiedSearchData.reason = XmCR_APPLY ;
            XtCallCallbackList ((Widget) fs,
                      fs->selection_box.apply_callback, &qualifiedSearchData) ;

            break ;
            }
        case XmDIALOG_CANCEL_BUTTON:
        {   
            qualifiedSearchData.reason = XmCR_CANCEL ;
            XtCallCallbackList ((Widget) fs,
                     fs->selection_box.cancel_callback, &qualifiedSearchData) ;

            allowUnmanage = TRUE ;
            break ;
            }
        case XmDIALOG_HELP_BUTTON:
        {   
	  if (fs->manager.help_callback)
	    {
	      qualifiedSearchData.reason = XmCR_HELP ;
	      XtCallCallbackList ((Widget) fs, fs->manager.help_callback,
				 &qualifiedSearchData) ;
	    }
	  else 	  /* Invoke the general help system. */
	    _XmManagerHelp((XmManagerWidget)fs, callback->event);

            break ;
            }
        }
    XmStringFree( qualifiedSearchData.pattern) ;
    XmStringFree( qualifiedSearchData.dir) ;
    XmStringFree( qualifiedSearchData.mask) ;
    XmStringFree( qualifiedSearchData.value) ;

    if(    allowUnmanage
        && fs->bulletin_board.shell
        && fs->bulletin_board.auto_unmanage   )
    {   
        XtUnmanageChild( fs) ;
        } 
    return ;
    }
/****************************************************************/
Widget
#ifdef _NO_PROTO
XmFileSelectionBoxGetChild( fs, which)
            Widget          fs ;        /* SelectionBox widget  */
            unsigned char   which ;     /* Which child          */
#else
XmFileSelectionBoxGetChild(
            Widget          fs,         /* SelectionBox widget  */
#if NeedWidePrototypes
    unsigned int   which
#else
    unsigned char   which
#endif 
)
#endif
/****************
 * This function returns the widget id of the
 *   specified SelectionBox child widget.
 ****************/
{   
            Widget          child = NULL ;
/****************/

    switch(    which    )
    {   
        case XmDIALOG_DIR_LIST:
        {   child = FS_DirList( fs) ;
            break ;
            } 
        case XmDIALOG_DIR_LIST_LABEL:
        {   child = FS_DirListLabel( fs) ;
            break ;
            } 
        case XmDIALOG_FILTER_LABEL:
        {   child = FS_FilterLabel( fs) ;
            break ;
            } 
        case XmDIALOG_FILTER_TEXT:
        {   child = FS_FilterText( fs) ;
            break ;
            }
        default:
        {   child = XmSelectionBoxGetChild( fs, which) ;
            break ;
            }
        }
    return( child) ;
    }
/****************************************************************/
void
#ifdef _NO_PROTO
XmFileSelectionDoSearch( fs, dirmask)
            Widget          fs ;
            XmString        dirmask ;
#else
XmFileSelectionDoSearch(
            Widget          fs,
            XmString        dirmask)
#endif
/****************
 * 
 ****************/
{   
            XmFileSelectionBoxCallbackStruct searchData ;
            String          textString ;
/****************/

    searchData.reason = XmCR_NONE ;
    searchData.event = 0 ;
    searchData.value = NULL ;
    searchData.length = 0 ;
    searchData.dir = NULL ;
    searchData.dir_length = 0 ;
    searchData.pattern = NULL ;
    searchData.pattern_length = 0 ;

    if(    dirmask    )
    {   
        searchData.mask = XmStringCopy( dirmask) ;
        searchData.mask_length = XmStringLength( searchData.mask) ;
        }
    else
    {   if(    FS_FilterText( fs)    )
        {   textString = XmTextGetString( FS_FilterText( fs)) ;
            } 
        else
        {   textString = NULL ;
            } 
        if(    textString    )
        {   searchData.mask = XmStringLtoRCreate( textString, 
                                                    XmSTRING_DEFAULT_CHARSET) ;
            searchData.mask_length = XmStringLength( searchData.mask) ;
            XtFree( textString) ;
            } 
        else
        {   searchData.mask = NULL ;
            searchData.mask_length = 0 ;
            } 
        } 
    FileSelectionBoxUpdate((XmFileSelectionBoxWidget) fs, &searchData) ;

    XmStringFree( searchData.mask) ;
    return ;
    }
/****************************************************************/
Widget
#ifdef _NO_PROTO
XmCreateFileSelectionBox( p, name, args, n)
            Widget          p ;         /*  parent widget   */
            String          name ;      /*  widget name     */
            ArgList         args ;      /*  arg list        */
            Cardinal        n ;         /*  arg count       */
#else
XmCreateFileSelectionBox(
            Widget          p,          /*  parent widget   */
            String          name,       /*  widget name     */
            ArgList         args,       /*  arg list        */
            Cardinal        n)          /*  arg count       */
#endif
/****************
 * 
 ****************/
{
/****************/

    return( XtCreateWidget( name, xmFileSelectionBoxWidgetClass, p, args, n));
    }
/****************************************************************/
Widget
#ifdef _NO_PROTO
XmCreateFileSelectionDialog (ds_p, name, fsb_args, fsb_n)
            Widget          ds_p ;      /*  parent for shell    */
            String          name ;      /*  widget name         */
            ArgList         fsb_args ;  /*  arglist for sb      */
            Cardinal        fsb_n ;     /*  argcount for sb     */
#else
XmCreateFileSelectionDialog(
            Widget          ds_p,       /*  parent for shell    */
            String          name,       /*  widget name         */
            ArgList         fsb_args,   /*  arglist for sb      */
            Cardinal        fsb_n)      /*  argcount for sb     */
#endif
/****************
 * This convenience function creates a DialogShell
 *   and a FileSelectionBox child of the shell;
 *   returns the FileSelectionBox widget.
 ****************/
{   
            Widget          fsb ;       /*  new fsb widget      */
            Widget          ds ;        /*  DialogShell         */
            Arg             ds_args[10] ;/*  arglist for shell  */
            char *          ds_name ;
/****************/

    /*  Create DialogShell parent.
    */
    ds_name = XtCalloc( strlen( name)+DIALOG_SUFFIX_SIZE+1, sizeof( char)) ;
    strcpy( ds_name, name) ;
    strcat( ds_name, DIALOG_SUFFIX) ;

    XtSetArg( ds_args[0], XmNallowShellResize, True) ; 
    ds = XmCreateDialogShell( ds_p, ds_name, ds_args, 1) ;

    XtFree( ds_name) ;

    /*  Create FileSelectionBox.
    */
    fsb = XtCreateWidget( name, xmFileSelectionBoxWidgetClass, ds, 
                                                             fsb_args, fsb_n) ;
    XtAddCallback( fsb, XmNdestroyCallback, _XmDestroyParentCallback, NULL) ;

    return( fsb) ;
    }
