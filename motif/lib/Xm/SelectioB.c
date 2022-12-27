#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)SelectioB.c	3.28.1.3 91/04/08";
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
/*-------------------------------------------------------------------------
**
**	include files
**
**-------------------------------------------------------------------------
*/

#include <Xm/SelectioBP.h>

#include <Xm/Text.h>
#include <Xm/LabelG.h>
#include <Xm/DialogS.h>
#include <Xm/List.h>
#include <Xm/PushBG.h>
#include <Xm/SeparatoG.h>


#define WARN_DIALOG_TYPE	"Incorrect dialog type."
#define WARN_DIALOG_TYPE_CHANGE "Dialog type cannot be modified."
#define WARN_WORK_AREA		"Only one work area child allowed."
#define WARN_CHILD_TYPE		"Invalid child type."


/* Internal functions:
*/
#ifdef _NO_PROTO

static void             ClassInitialize() ;
static void 		ClassPartInitialize() ;
static void 		Initialize() ;
static void		InsertChild() ;
static void		DeleteChild() ;
static void             _XmDialogTypeDefault() ;
static XmImportOperator _XmSetSyntheticResForChild();
static void 		SelectionBoxCallback() ;
static void 		ListCallback() ;
static void             UpdateString() ;
static Boolean		SetValues() ;

#else /* _NO_PROTO */

static void             ClassInitialize() ;
static void 		ClassPartInitialize( XmSelectionBoxWidgetClass class) ;
static void 		Initialize( XmSelectionBoxWidget request,
                                                    XmSelectionBoxWidget new) ;
static void		InsertChild( Widget child) ;
static void		DeleteChild( Widget child) ;
static void             _XmDialogTypeDefault( Widget widget, int offset, 
                                                            XrmValue * value) ;
static XmImportOperator _XmSetSyntheticResForChild(Widget widget, int offset, XtArgVal *value);
static void 		SelectionBoxCallback( Widget w, caddr_t client_data,
                                                           caddr_t call_data) ;
static void 		ListCallback( Widget w, caddr_t client_data,
                                                           caddr_t call_data) ;
static void             UpdateString( Widget w, XmString string, 
                                                 XmStringDirection direction) ;
static Boolean		SetValues( XmSelectionBoxWidget current,
                                                  XmSelectionBoxWidget request,
                                                    XmSelectionBoxWidget new) ;
#endif /* _NO_PROTO */



static XtAccelerators defaultTextAcceleratorsParsed;

#ifndef MCCABE
static char defaultTextAccelerators[] =
   "\043override\n\
	Ctrl <Key>osfBeginLine: SelectionBoxUpOrDown(2)\n\
	Ctrl <Key>osfEndLine: SelectionBoxUpOrDown(3)\n\
	Shift Ctrl <Key>osfInsert:	SelectionBoxRestore()\n\
	<Key>osfUp:	SelectionBoxUpOrDown(0)\n\
	<Key>osfDown:	SelectionBoxUpOrDown(1)";
#else
static char defaultTextAccelerators[];
#endif

/*  Action list  */

static XtActionsRec actionsList[] =
{
    { "UpOrDown",    (XtActionProc)  _XmSelectionBoxUpOrDown }, /* Motif 1.0 */
    { "SelectionBoxUpOrDown",   (XtActionProc)  _XmSelectionBoxUpOrDown },
    { "SelectionBoxRestore",    (XtActionProc)  _XmSelectionBoxRestore },
    };


/*  Resource definitions for SelectionBox
*/

static XmSyntheticResource syn_resources[] = 
{
	{	XmNselectionLabelString, 
		sizeof (XmString), 
		XtOffset (XmSelectionBoxWidget, selection_box.selection_label_string), 
		_XmSelectionBoxGetSelectionLabelString,
		_XmSetSyntheticResForChild
	}, 

	{	XmNlistLabelString, 
		sizeof (XmString), 
		XtOffset (XmSelectionBoxWidget, selection_box.list_label_string), 
		_XmSelectionBoxGetListLabelString,
		_XmSetSyntheticResForChild
	}, 

	{	XmNtextColumns, 
		sizeof(short), 
		XtOffset (XmSelectionBoxWidget, selection_box.text_columns), 
		_XmSelectionBoxGetTextColumns,
		NULL
	}, 

	{	XmNtextString, 
		sizeof (XmString), 
		XtOffset (XmSelectionBoxWidget, selection_box.text_string), 
		_XmSelectionBoxGetTextString,
		_XmSetSyntheticResForChild
	}, 

	{	XmNlistItems, 
		sizeof (XmString *), 
		XtOffset (XmSelectionBoxWidget, selection_box.list_items), 
		_XmSelectionBoxGetListItems,
		_XmSetSyntheticResForChild
	},                                        

	{	XmNlistItemCount, 
		sizeof(int), 
		XtOffset (XmSelectionBoxWidget, selection_box.list_item_count), 
		_XmSelectionBoxGetListItemCount,
		_XmSetSyntheticResForChild
	}, 
 
	{	XmNlistVisibleItemCount, 
		sizeof(int), 
		XtOffset (XmSelectionBoxWidget, selection_box.list_visible_item_count), 
		_XmSelectionBoxGetListVisibleItemCount,
		_XmSetSyntheticResForChild
	}, 

	{	XmNokLabelString, 
		sizeof (XmString), 
		XtOffset (XmSelectionBoxWidget, selection_box.ok_label_string), 
		_XmSelectionBoxGetOkLabelString,
		NULL
	}, 

	{	XmNapplyLabelString, 
		sizeof (XmString), 
		XtOffset (XmSelectionBoxWidget, selection_box.apply_label_string), 
		_XmSelectionBoxGetApplyLabelString,
		NULL
	}, 

	{	XmNcancelLabelString, 
		sizeof (XmString), 
		XtOffset (XmSelectionBoxWidget, selection_box.cancel_label_string), 
		_XmSelectionBoxGetCancelLabelString,
		NULL
	}, 

	{	XmNhelpLabelString, 
		sizeof (XmString), 
		XtOffset (XmSelectionBoxWidget, selection_box.help_label_string), 
		_XmSelectionBoxGetHelpLabelString,
		NULL
	}, 
};



static XtResource resources[] = 
{
	{	XmNtextAccelerators, 
		XmCAccelerators, XmRAcceleratorTable, sizeof (XtAccelerators), 
		XtOffset (XmSelectionBoxWidget, selection_box.text_accelerators), 
		XmRImmediate, NULL		
	}, 

	{	XmNselectionLabelString, 
		XmCSelectionLabelString, XmRXmString, sizeof (XmString), 
		XtOffset (XmSelectionBoxWidget, selection_box.selection_label_string), 
		XmRString, NULL
	}, 

	{	XmNlistLabelString, 
		XmCListLabelString, XmRXmString, sizeof (XmString), 
		XtOffset (XmSelectionBoxWidget, selection_box.list_label_string), 
		XmRString, NULL
	}, 

	{	XmNtextColumns, 
		XmCColumns, XmRShort, sizeof(short), 
		XtOffset (XmSelectionBoxWidget, selection_box.text_columns), 
		XmRImmediate, (caddr_t) 20
	}, 

	{	XmNtextString, 
		XmCTextString, XmRXmString, sizeof (XmString), 
		XtOffset (XmSelectionBoxWidget, selection_box.text_string), 
		XmRImmediate, (caddr_t) UNSPECIFIED
	}, 

	{	XmNlistItems, 
		XmCItems, XmRXmStringTable, sizeof (XmStringTable), 
		XtOffset (XmSelectionBoxWidget, selection_box.list_items), 
		XmRImmediate, NULL
	},                                        

	{	XmNlistItemCount, 
		XmCItemCount, XmRInt, sizeof(int), 
		XtOffset (XmSelectionBoxWidget, selection_box.list_item_count), 
		XmRImmediate, (caddr_t) UNSPECIFIED
	}, 
 
	{	XmNlistVisibleItemCount, 
		XmCVisibleItemCount, XmRInt, sizeof(int), 
		XtOffset (XmSelectionBoxWidget, selection_box.list_visible_item_count), 
		XmRImmediate, (caddr_t) 8
	}, 

	{	XmNokLabelString, 
		XmCOkLabelString, XmRXmString, sizeof (XmString), 
		XtOffset (XmSelectionBoxWidget, selection_box.ok_label_string), 
		XmRString, NULL
	}, 

	{	XmNapplyLabelString, 
		XmCApplyLabelString, XmRXmString, sizeof (XmString), 
		XtOffset (XmSelectionBoxWidget, selection_box.apply_label_string), 
		XmRString, NULL
	}, 

	{	XmNcancelLabelString, 
		XmCCancelLabelString, XmRXmString, sizeof (XmString), 
		XtOffset (XmSelectionBoxWidget, selection_box.cancel_label_string), 
		XmRString, NULL
	}, 

	{	XmNhelpLabelString, 
		XmCHelpLabelString, XmRXmString, sizeof (XmString), 
		XtOffset (XmSelectionBoxWidget, selection_box.help_label_string), 
		XmRString, NULL
	}, 

	{	XmNnoMatchCallback, 
		XmCCallback, XmRCallback, sizeof (XtCallbackList), 
		XtOffset (XmSelectionBoxWidget, selection_box.no_match_callback), 
		XmRImmediate, (caddr_t) NULL
	}, 

	{	XmNmustMatch, 
		XmCMustMatch, XmRBoolean, sizeof(Boolean), 
		XtOffset (XmSelectionBoxWidget, selection_box.must_match), 
		XmRImmediate, (caddr_t) False
	}, 
                     
	{	XmNminimizeButtons, 
		XmCMinimizeButtons, XmRBoolean, sizeof(Boolean), 
		XtOffset (XmSelectionBoxWidget, selection_box.minimize_buttons), 
		XmRImmediate, (caddr_t) False
	}, 
                     
	{	XmNokCallback, 
		XmCCallback, XmRCallback, sizeof (XtCallbackList), 
		XtOffset (XmSelectionBoxWidget, selection_box.ok_callback), 
		XmRImmediate, (caddr_t) NULL
	}, 

	{	XmNapplyCallback, 
		XmCCallback, XmRCallback, sizeof (XtCallbackList), 
		XtOffset (XmSelectionBoxWidget, selection_box.apply_callback), 
		XmRImmediate, (caddr_t) NULL
	}, 

	{	XmNcancelCallback, 
		XmCCallback, XmRCallback, sizeof (XtCallbackList), 
		XtOffset (XmSelectionBoxWidget, selection_box.cancel_callback), 
		XmRImmediate, (caddr_t) NULL
	}, 

	{	XmNdialogType, 
		XmCDialogType, XmRDialogType, sizeof (unsigned char), 
		XtOffset (XmSelectionBoxWidget, selection_box.dialog_type), 
		XmRCallProc, (caddr_t) _XmDialogTypeDefault
	}, 
};



externaldef( xmselectionboxclassrec) XmSelectionBoxClassRec
                                                       xmSelectionBoxClassRec =
{
    {
    	/* superclass	      */	(WidgetClass) &xmBulletinBoardClassRec, 
    	/* class_name	      */	"XmSelectionBox", 
    	/* widget_size	      */	sizeof(XmSelectionBoxRec), 
    	/* class_initialize   */    	ClassInitialize, 
    	/* chained class init */	ClassPartInitialize, 
    	/* class_inited       */	FALSE, 
    	/* initialize	      */	Initialize, 
    	/* initialize hook    */	NULL, 
    	/* realize	      */	_XtInherit, 
    	/* actions	      */	actionsList, 
    	/* num_actions	      */	XtNumber(actionsList), 
    	/* resources	      */	resources, 
    	/* num_resources      */	XtNumber(resources), 
    	/* xrm_class	      */	NULLQUARK, 
    	/* compress_motion    */	TRUE, 
    	/* compress_exposure  */	XtExposeCompressMaximal,
    	/* compress enter/exit*/	TRUE, 
    	/* visible_interest   */	FALSE, 
    	/* destroy	      */	NULL, 
    	/* resize	      */	_XtInherit,
    	/* expose	      */	_XtInherit, 
    	/* set_values	      */	SetValues, 
	/* set_values_hook    */	NULL,                    
	/* set_values_almost  */	_XtInherit, 
	/* get_values_hook    */	NULL, 
    	/* accept_focus	      */	NULL, 
	/* version	      */	XtVersion, 
        /* callback_offsets   */        NULL, 
        /* tm_table           */        XtInheritTranslations, 
    	/* query_geometry     */	(XtGeometryHandler) _XtInherit,
    	/* display_accelerator*/	NULL, 
	/* extension	      */	NULL, 
    }, 

    {	/* composite class record */    

	/* childrens geo mgr proc   */  (XtGeometryHandler) _XtInherit,
	/* set changed proc	    */  _XtInherit,
	/* insert_child		    */	InsertChild, 
	/* delete_child 	    */	DeleteChild, 
	/* extension		    */	NULL, 
    }, 

    {	/* constraint class record */

	/* no additional resources  */	NULL, 
	/* num additional resources */	0, 
	/* size of constraint rec   */	0, 
	/* constraint_initialize    */	NULL, 
	/* constraint_destroy	    */	NULL, 
	/* constraint_setvalue	    */	NULL, 
	/* extension		    */	NULL, 
    }, 

    {	/* manager class record */
      XtInheritTranslations, 		        /* default translations   */
      syn_resources, 				/* syn_resources      	  */
      XtNumber (syn_resources), 		/* num_syn_resources 	  */
      NULL, 					/* syn_cont_resources     */
      0, 					/* num_syn_cont_resources */
      XmInheritParentProcess,                   /* parent_process         */
      NULL, 					/* extension		  */
    }, 

    {	/* bulletin board class record */     
        TRUE,                                   /*always_install_accelerators*/
        _XmSelectionBoxGeoMatrixCreate,         /* geo_matrix_create */
        XtInheritFocusMovedProc,                /* focus_moved_proc */
	NULL, 					/* extension */
    }, 	

    {	/* selection box class record */
        ListCallback,                           /* list_callback */
	NULL, 					/* extension  */
    }, 
};

externaldef( xmselectionboxwidgetclass) WidgetClass xmSelectionBoxWidgetClass
                                      = (WidgetClass) &xmSelectionBoxClassRec ;


/****************************************************************/
static void
ClassInitialize()
/****************
 * 
 ****************/
{   
/****************/

    /* parse the default translation and accelerator tables
    */
    defaultTextAcceleratorsParsed =
                            XtParseAcceleratorTable( defaultTextAccelerators) ;
    return ;
    }
/****************************************************************/
static void
#ifdef _NO_PROTO
ClassPartInitialize (class)
            XmSelectionBoxWidgetClass	class;
#else
ClassPartInitialize(
            XmSelectionBoxWidgetClass	class)
#endif
/****************
 * 
 ****************/
{   
/****************/

    _XmFastSubclassInit( class, XmSELECTION_BOX_BIT) ;

    return ;
    }



/****************************************************************/
static void
#ifdef _NO_PROTO
Initialize( request, new)
            XmSelectionBoxWidget request;
            XmSelectionBoxWidget new;
#else
Initialize(
            XmSelectionBoxWidget request,
            XmSelectionBoxWidget new)
#endif
/****************
 * Create a SelectionBox instance.
 ****************/
{
            Arg	            al[5];
/****************/

	new->selection_box.work_area = NULL;

        if ( new->selection_box.text_accelerators == NULL )
	{
		new->selection_box.text_accelerators = 
			defaultTextAcceleratorsParsed;
	}


	/*	Validate dialog type.
	*/
	if ( request->selection_box.dialog_type != XmDIALOG_WORK_AREA &&
	     request->selection_box.dialog_type != XmDIALOG_SELECTION &&
	     request->selection_box.dialog_type != XmDIALOG_FILE_SELECTION &&
	     request->selection_box.dialog_type != XmDIALOG_COMMAND &&
	     request->selection_box.dialog_type != XmDIALOG_PROMPT ) 
	{	
		_XmWarning (new, WARN_DIALOG_TYPE);
		if ( request->bulletin_board.shell )
			new->selection_box.dialog_type = XmDIALOG_SELECTION;
		else
			new->selection_box.dialog_type = XmDIALOG_WORK_AREA;
	}

                      
	/*	Create child widgets.
	*/
	new->selection_box.adding_sel_widgets = True;

	if ( (new->selection_box.dialog_type != XmDIALOG_PROMPT)
		&& (new->selection_box.dialog_type != XmDIALOG_COMMAND) )
	{   
            _XmSelectionBoxCreateListLabel( new) ;
            }
	else
        {   SB_ListLabel (new) = NULL;
            } 
        new->selection_box.list_label_string = NULL ;

	if (new->selection_box.dialog_type != XmDIALOG_PROMPT)
	{   
            _XmSelectionBoxCreateList( new) ;
            }
	else
        {   SB_List (new) = NULL;
            } 
        new->selection_box.list_items = NULL ;
        new->selection_box.list_item_count = UNSPECIFIED ;

	_XmSelectionBoxCreateSelectionLabel(new);
        new->selection_box.selection_label_string = NULL ;

	_XmSelectionBoxCreateText(new);

        /* Do not reset text_string to UNSPECIFIED until after calls 
        *   to CreateList and CreateText.
        */
        new->selection_box.text_string = (XmString) UNSPECIFIED ;

	if (new->selection_box.dialog_type != XmDIALOG_COMMAND)
	{
		_XmSelectionBoxCreateSeparator (new);
		_XmSelectionBoxCreateOkButton (new);
		_XmSelectionBoxCreateApplyButton (new);
		_XmSelectionBoxCreateCancelButton (new);
		_XmSelectionBoxCreateHelpButton (new);

                BB_DefaultButton( new) = SB_OkButton( new) ;

                _XmChangeNavigationType((Widget) new, (XtIsShell( XtParent( request))
                                         ? XmSTICKY_TAB_GROUP : XmTAB_GROUP)) ;
	}
	else
	{
		SB_Separator (new) = NULL;
		SB_OkButton (new) = NULL;
		SB_ApplyButton (new) = NULL;
		SB_CancelButton (new) = NULL;
		SB_HelpButton (new) = NULL;
	}
        new->selection_box.ok_label_string = NULL ;
        new->selection_box.apply_label_string = NULL ;
        new->selection_box.cancel_label_string = NULL ;
        new->selection_box.help_label_string = NULL ;

	new->selection_box.adding_sel_widgets = False;

	XtManageChildren (new->composite.children, 
                                                 new->composite.num_children) ;
	if (new->selection_box.dialog_type == XmDIALOG_PROMPT ||
	    new->selection_box.dialog_type == XmDIALOG_WORK_AREA)
	{
		XtUnmanageChild (SB_ApplyButton (new));
	}
        return ;
}

/****************************************************************/
static void
#ifdef _NO_PROTO
InsertChild( child)
            Widget          child ;
#else
InsertChild(
            Widget          child)
#endif
/****************
 * Selection widget supports ONE child.  This routine
 *   handles adding a child to selection widget
 ****************/
{   
            XmSelectionBoxWidget sb ;
/****************/

    /* Use the dialog class insert proc to do all the dirty work
    */
    (*((XmBulletinBoardWidgetClass) xmBulletinBoardWidgetClass)
                                      ->composite_class.insert_child) (child) ;
    if(    !XtIsRectObj( child)    )
    {   return ;
        } 
    sb = (XmSelectionBoxWidget) XtParent( child) ;

    /* check if this child is to be the selection widget's work area widget
    */
    if(    !sb->selection_box.adding_sel_widgets    )
    {   
        if(    !sb->selection_box.work_area    )
        {   sb->selection_box.work_area = child ;
            } 
        else
        {   _XmWarning( sb, WARN_WORK_AREA) ;
            } 
        } 
    return ;
    }

/****************************************************************/
static void
#ifdef _NO_PROTO
DeleteChild( child)
            Widget          child ;
#else
DeleteChild(
            Widget          child)
#endif
/****************
 * Remove child from selection widget
 ****************/
{   
            XmSelectionBoxWidget sel ;
/****************/

    if(    XtIsRectObj( child)    )
    {   
        sel = (XmSelectionBoxWidget) XtParent( child) ;
        /*	Clear widget fields (BulletinBoard does default and cancel).
        */
        if(    child == SB_ListLabel (sel)    )
        {   SB_ListLabel( sel) = NULL ;
            } 
        else
        {   if(    child == SB_List (sel)    )
            {   SB_List( sel) = NULL ;
                } 
            else
            {   if(    child == SB_SelectionLabel (sel)    )
                {   SB_SelectionLabel( sel) = NULL ;
                    } 
                else
                {   if(    child == SB_Text (sel)    )
                    {   SB_Text( sel) = NULL ;
                        } 
                    else
                    {   if(    child == SB_WorkArea (sel)    )
                        {   SB_WorkArea( sel) = NULL ;
                            } 
                        else
                        {   if(    child == SB_Separator (sel)    )
                            {   SB_Separator( sel) = NULL ;
                                } 
                            else
                            {   if(    child == SB_OkButton (sel)    )
                                {   SB_OkButton( sel) = NULL ;
                                    } 
                                else
                                {   if(    child == SB_ApplyButton (sel)    )
                                    {   SB_ApplyButton( sel) = NULL ;
                                        } 
                                    else
                                    {   if(    child == SB_HelpButton (sel)   )
                                        {   SB_HelpButton( sel) = NULL ;
                                            } 
                                        }
                                    } 
                                } 
                            } 
                        } 
                    } 
                } 
            }
        } 
    (*((XmBulletinBoardWidgetClass) xmBulletinBoardWidgetClass)
                                      ->composite_class.delete_child)( child) ;
    return ;
    }

/****************************************************************/
static void
#ifdef _NO_PROTO
_XmDialogTypeDefault( widget, offset, value)
            Widget          widget ;
            int             offset ;
            XrmValue *      value ;
#else
_XmDialogTypeDefault(
            Widget          widget,
            int             offset,
            XrmValue *      value)
#endif
/****************           ARGSUSED
 * Set the default type (selection or workarea) based on parent.
 ****************/
{
    static unsigned char	type;
/****************/

    /*
     * Set the default type.  To do this, we check the dialog
     * box's parent.  If it is a DialogShell widget, then this
     * is a "pop-up" dialog box, and the default type is selection.
     * Else the default type is workarea.
     */
    type = XmDIALOG_WORK_AREA;
    if (XmIsDialogShell (XtParent (widget)))
	type = XmDIALOG_SELECTION;
    value->addr = (caddr_t)(&type);
    return ;
}

/****************************************************************/
static 
#ifdef _NO_PROTO
XmImportOperator _XmSetSyntheticResForChild  (widget, offset, value)
Widget widget;
int offset;
XtArgVal * value;
#else /* _NO_PROTO */
XmImportOperator  _XmSetSyntheticResForChild  (Widget widget, int offset, XtArgVal *value)
#endif /* _NO_PROTO */
{ 
  return XmSYNTHETIC_LOAD;
}
/****************************************************************/
void
#ifdef _NO_PROTO
_XmSelectionBoxCreateListLabel( sel)
            XmSelectionBoxWidget sel ;
#else
_XmSelectionBoxCreateListLabel(
            XmSelectionBoxWidget sel)
#endif
/****************
 * Create the Label displayed above the List widget.
 ****************/
{
/****************/

    SB_ListLabel( sel) = _XmBB_CreateLabelG((Widget) sel,
                               sel->selection_box.list_label_string, "Items") ;
    return ;
    }
/****************************************************************/
void
#ifdef _NO_PROTO
_XmSelectionBoxCreateSelectionLabel( sel)
            XmSelectionBoxWidget sel ;
#else
_XmSelectionBoxCreateSelectionLabel(
            XmSelectionBoxWidget sel)
#endif
/****************
 * Create the Label displayed above the Text widget.
 ****************/
{
/****************/

    SB_SelectionLabel( sel) = _XmBB_CreateLabelG((Widget) sel,
                      sel->selection_box.selection_label_string, "Selection") ;
    return ;
    }


/****************************************************************/
void
#ifdef _NO_PROTO
_XmSelectionBoxCreateList (sel)
            XmSelectionBoxWidget sel;
#else
_XmSelectionBoxCreateList(
            XmSelectionBoxWidget sel)
#endif
/****************
 * Create the List widget.
 ****************/
{   
            Arg		al[20] ;
    register int	ac = 0 ;
            int *       position ;
            int         pos_count ;
            XtCallbackProc callbackProc ;
/****************/

    if(    sel->selection_box.list_items    )
    {   
        XtSetArg( al[ac], XmNitems, sel->selection_box.list_items) ; ac++ ;
        }
    if(    sel->selection_box.list_item_count != UNSPECIFIED    )
    {   
        XtSetArg( al[ac], XmNitemCount, 
                                  sel->selection_box.list_item_count) ;  ac++ ;
        }
    XtSetArg( al[ac], XmNvisibleItemCount, 
		sel->selection_box.list_visible_item_count) ;  ac++ ;

    sel->selection_box.list_selected_item_position = 0 ;

    XtSetArg( al[ac], XmNstringDirection, SB_StringDirection (sel)) ;  ac++ ;
    XtSetArg( al[ac], XmNselectionPolicy, XmBROWSE_SELECT) ;  ac++ ;
    XtSetArg( al[ac], XmNlistSizePolicy, XmCONSTANT) ;  ac++ ;
    XtSetArg( al[ac], XmNscrollBarDisplayPolicy, XmAS_NEEDED) ;  ac++ ;
    XtSetArg( al[ac], XmNnavigationType, XmSTICKY_TAB_GROUP) ; ++ac ;

    SB_List( sel) = XmCreateScrolledList( (Widget)sel, "sb_list", al, ac) ;

    if(    sel->selection_box.text_string != (XmString) UNSPECIFIED    )
    {   
        if(    sel->selection_box.text_string
            && XmListGetMatchPos( SB_List( sel), 
                    sel->selection_box.text_string, &position, &pos_count)    )
        {   if(    pos_count    )
            {   
                sel->selection_box.list_selected_item_position = position[0] ;
                XmListSelectPos( SB_List( sel), position[0], FALSE) ;
                } 
            XtFree( position) ;
            } 
        }
    callbackProc = ((XmSelectionBoxWidgetClass) sel->core.widget_class)
                                          ->selection_box_class.list_callback ;
    if(    callbackProc    )
    {   
        XtAddCallback( SB_List( sel), XmNsingleSelectionCallback,
                                               callbackProc, (XtPointer) sel) ;
        XtAddCallback( SB_List( sel), XmNbrowseSelectionCallback,
                                               callbackProc, (XtPointer) sel) ;
        XtAddCallback( SB_List( sel), XmNdefaultActionCallback,
                                               callbackProc, (XtPointer) sel) ;
        } 
    XtManageChild( SB_List( sel)) ;

    return ;
    }

/****************************************************************/
void
#ifdef _NO_PROTO
_XmSelectionBoxCreateText (sel)
            XmSelectionBoxWidget sel;
#else
_XmSelectionBoxCreateText(
            XmSelectionBoxWidget sel)
#endif
/****************
 * Create the Text widget.
 ****************/
{
	Arg		al[10];
	register int	ac = 0;
	String		text_value = NULL ;
	XtAccelerators	temp_accelerators ;
/****************/

	XtSetArg (al[ac], XmNcolumns, sel->selection_box.text_columns);  ac++;
	XtSetArg (al[ac], XmNresizeWidth, False);  ac++;
	XtSetArg (al[ac], XmNeditMode, XmSINGLE_LINE_EDIT);  ac++;
        XtSetArg( al[ac], XmNnavigationType, XmSTICKY_TAB_GROUP) ; ++ac ;

	SB_Text (sel) = 
		XmCreateText ((Widget)sel, "sb_text", al, ac);

	if(    (sel->selection_box.text_string != (XmString) UNSPECIFIED)    )
        {   
            if(    !XmStringGetLtoR (sel->selection_box.text_string, 
                                    XmSTRING_DEFAULT_CHARSET, &text_value)    )
            {   text_value = XtMalloc(1 * sizeof(String)) ;
		strcpy(text_value, "");
		/* text_value = ""; */
                } 
            XmTextSetString( SB_Text (sel), text_value) ;
            if(    text_value    )
            {   XmTextSetCursorPosition( SB_Text (sel), strlen (text_value)) ;
            	XtFree( text_value) ;
                } 
            } 

	/*	Install text accelerators.
	*/
        temp_accelerators = sel->core.accelerators;
	sel->core.accelerators = sel->selection_box.text_accelerators;
	XtInstallAccelerators (SB_Text (sel), sel);
	sel->core.accelerators = temp_accelerators;
        return ;
}

/****************************************************************/
void
#ifdef _NO_PROTO
_XmSelectionBoxCreateSeparator (sel)
            XmSelectionBoxWidget sel;
#else
_XmSelectionBoxCreateSeparator(
            XmSelectionBoxWidget sel)
#endif
/****************
 * Create the Separator displayed above the buttons.
 ****************/
{
	Arg		al[10];
	register int	ac = 0;
/****************/

	XtSetArg (al[ac], XmNhighlightThickness, 0);  ac++;
	SB_Separator (sel) =
		XmCreateSeparatorGadget ((Widget)sel, "Separator", al, ac);
        return ;
}
/****************************************************************/
void
#ifdef _NO_PROTO
_XmSelectionBoxCreateOkButton( sel)
            XmSelectionBoxWidget sel ;
#else
_XmSelectionBoxCreateOkButton(
            XmSelectionBoxWidget sel)
#endif
/****************
 * Create the "OK" PushButton.
 ****************/
{
/****************/

    SB_OkButton( sel) = _XmBB_CreateButtonG((Widget) sel, 
                                    sel->selection_box.ok_label_string, "OK") ;
    XtAddCallback (SB_OkButton (sel), XmNactivateCallback, 
                                    SelectionBoxCallback, XmDIALOG_OK_BUTTON) ;
    return ;
    }

/****************************************************************/
void
#ifdef _NO_PROTO
_XmSelectionBoxCreateApplyButton( sel)
            XmSelectionBoxWidget sel ;
#else
_XmSelectionBoxCreateApplyButton(
            XmSelectionBoxWidget sel)
#endif
/****************
 * Create the "Apply" PushButton.
 ****************/
{
/****************/

    SB_ApplyButton( sel) = _XmBB_CreateButtonG((Widget) sel, 
                              sel->selection_box.apply_label_string, "Apply") ;
    /* Remove BulletinBoard Unmanage callback from apply and help buttons.
    */
    XtRemoveAllCallbacks( SB_ApplyButton( sel), XmNactivateCallback) ;
    XtAddCallback (SB_ApplyButton (sel), XmNactivateCallback, 
                                 SelectionBoxCallback, XmDIALOG_APPLY_BUTTON) ;
    return ;
    }
/****************************************************************/
void
#ifdef _NO_PROTO
_XmSelectionBoxCreateCancelButton( sel)
            XmSelectionBoxWidget sel ;
#else
_XmSelectionBoxCreateCancelButton(
            XmSelectionBoxWidget sel)
#endif
/****************
 * Create the "Cancel" PushButton.
 ****************/
{
/****************/

    SB_CancelButton( sel) = _XmBB_CreateButtonG((Widget) sel, 
                            sel->selection_box.cancel_label_string, "Cancel") ;
    XtAddCallback( SB_CancelButton( sel), XmNactivateCallback, 
                                SelectionBoxCallback, XmDIALOG_CANCEL_BUTTON) ;
    return ;
    }
/****************************************************************/
void
#ifdef _NO_PROTO
_XmSelectionBoxCreateHelpButton( sel)
            XmSelectionBoxWidget sel ;
#else
_XmSelectionBoxCreateHelpButton(
            XmSelectionBoxWidget sel)
#endif
/****************
 * Create the "Help" PushButton.
 ****************/
{
/****************/

    SB_HelpButton( sel) = _XmBB_CreateButtonG((Widget) sel, 
                                sel->selection_box.help_label_string, "Help") ;
    /* Remove BulletinBoard Unmanage callback from apply and help buttons.
    */
    XtRemoveAllCallbacks( SB_HelpButton( sel), XmNactivateCallback) ;
    XtAddCallback (SB_HelpButton (sel), XmNactivateCallback, 
                                  SelectionBoxCallback, XmDIALOG_HELP_BUTTON) ;
    return ;
    }
   
/****************************************************************/
XmGeoMatrix    
#ifdef _NO_PROTO
_XmSelectionBoxGeoMatrixCreate( sb, instigator, desired)
            XmSelectionBoxWidget sb ;
            Widget          instigator ;
            XtWidgetGeometry *desired ;
#else
_XmSelectionBoxGeoMatrixCreate(
            XmSelectionBoxWidget sb,
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
            XmListWidget    list ;
            Boolean         listLabelBox ;
            Boolean         selLabelBox ;
/****************/

    geoSpec = _XmGeoMatrixAlloc( SB_MAX_WIDGETS_VERT, SB_MAX_NUM_WIDGETS, 0) ;
    geoSpec->composite = (Widget) sb ;
    geoSpec->instigator = (Widget) instigator ;
    if(    desired    )
    {   geoSpec->instig_request = *desired ;
        } 
    geoSpec->margin_w = BB_MarginWidth( sb) + sb->manager.shadow_thickness ;
    geoSpec->margin_h = BB_MarginHeight( sb) + sb->manager.shadow_thickness ;
    geoSpec->no_geo_request = _XmSelectionBoxNoGeoRequest ;

    layoutPtr = geoSpec->layouts ;
    boxPtr = geoSpec->boxes ;

    listLabelBox = FALSE ;
    if(    _XmGeoSetupKid( boxPtr, SB_ListLabel( sb))    )
    {   
        listLabelBox = TRUE ;
        layoutPtr->space_above = BB_MarginHeight( sb) ;
        boxPtr += 2 ;       /* For new row, add 2. */
        ++layoutPtr ;       /* For new row. */
        } 
    list = (XmListWidget) SB_List( sb) ;
    if(    list  &&  _XmGeoSetupKid( boxPtr, XtParent( list))    )
    {   
        if(    !listLabelBox    )
        {   layoutPtr->space_above = BB_MarginHeight( sb) ;
            } 
        layoutPtr->stretch_height = TRUE ;
        layoutPtr->min_height = 70 ;
        boxPtr += 2 ;       /* For new row, add 2. */
        ++layoutPtr ;       /* For new row. */
        } 
    if(    _XmGeoSetupKid( boxPtr, SB_WorkArea( sb))    )
    {   layoutPtr->space_above = BB_MarginHeight( sb) ;
        boxPtr += 2 ;       /* For new row, add 2. */
        ++layoutPtr ;       /* For new row. */
        } 
    selLabelBox = FALSE ;
    if(    _XmGeoSetupKid( boxPtr, SB_SelectionLabel( sb))    )
    {   selLabelBox = TRUE ;
        layoutPtr->space_above = BB_MarginHeight( sb) ;
        boxPtr += 2 ;       /* For new row, add 2. */
        ++layoutPtr ;       /* For new row. */
        } 
    if(    _XmGeoSetupKid( boxPtr, SB_Text( sb))    )
    {   
        if(    !selLabelBox    )
        {   layoutPtr->space_above = BB_MarginHeight( sb) ;
            } 
        boxPtr += 2 ;       /* For new row, add 2. */
        ++layoutPtr ;       /* For new row. */
        } 
    if(    _XmGeoSetupKid( boxPtr, SB_Separator( sb))    )
    {   layoutPtr->fix_up = _XmSeparatorFix ;
        layoutPtr->space_above = BB_MarginHeight( sb) ;
        boxPtr += 2 ;       /* For new row, add 2. */
        ++layoutPtr ;       /* For new row. */
        } 
    firstButtonBox = boxPtr ;
    if(    _XmGeoSetupKid( boxPtr, SB_OkButton( sb))    )
    {   ++boxPtr ;
        } 
    if(    _XmGeoSetupKid( boxPtr, SB_ApplyButton( sb))    )
    {   ++boxPtr ;
        } 
    if(    _XmGeoSetupKid( boxPtr, SB_CancelButton( sb))    )
    {   ++boxPtr ;
        } 
    if(    _XmGeoSetupKid( boxPtr, SB_HelpButton( sb))    )
    {   ++boxPtr ;
        } 
    if(    boxPtr != firstButtonBox    )
    {   /* Had at least one button.
        */
        layoutPtr->fill_mode = XmGEO_CENTER ;
        layoutPtr->fit_mode = XmGEO_WRAP ;
        layoutPtr->space_above = BB_MarginHeight( sb) ;
        if(    !(sb->selection_box.minimize_buttons)    )
        {   layoutPtr->even_width = 1 ;
            } 
        layoutPtr->even_height = 1 ;
        ++boxPtr ;
	++layoutPtr ;
        } 
    layoutPtr->space_above = BB_MarginHeight( sb) ;
    layoutPtr->end = TRUE ;        /* Mark the last row. */
    return( geoSpec) ;
    }
/****************************************************************/
Boolean
#ifdef _NO_PROTO
_XmSelectionBoxNoGeoRequest( geoSpec)
            XmGeoMatrix     geoSpec ;
#else
_XmSelectionBoxNoGeoRequest(
            XmGeoMatrix     geoSpec)
#endif
/****************
 * 
 ****************/
{
/****************/

    if(    BB_InSetValues( geoSpec->composite)
        && (XtClass( geoSpec->composite) == xmSelectionBoxWidgetClass)    )
    {   
        return( TRUE) ;
        } 
    return( FALSE) ;
    }
   
/****************************************************************/
static void
#ifdef _NO_PROTO
SelectionBoxCallback (w, client_data, call_data)
            Widget		w;
            caddr_t		client_data;
            caddr_t		call_data;
#else
SelectionBoxCallback(
            Widget		w,
            caddr_t		client_data,
            caddr_t		call_data)
#endif
/****************
 * Call the callbacks for a SelectionBox button.
 ****************/
{
	unsigned char		which_button = (unsigned char) client_data;
	XmSelectionBoxWidget	sel = (XmSelectionBoxWidget) XtParent (w);
	XmAnyCallbackStruct	*callback = (XmAnyCallbackStruct *) call_data;
	XmSelectionBoxCallbackStruct	temp;
	Boolean			match = True;
	String			text_value;
/****************/

	text_value = XmTextGetString (SB_Text (sel));
	temp.event = callback->event;
	temp.value = XmStringLtoRCreate (text_value, XmSTRING_DEFAULT_CHARSET);
	temp.length = XmStringLength (temp.value);
	XtFree (text_value);
	
	switch (which_button)
	{
		case XmDIALOG_OK_BUTTON:
	                if (sel->selection_box.list != NULL)
		  	  if (sel->selection_box.must_match)
			   {
				match = XmListItemExists (SB_List (sel), temp.value);
			   }
			if (!match)
			{
				temp.reason = XmCR_NO_MATCH;
				XtCallCallbackList ((Widget) sel,
 sel->					selection_box.no_match_callback, &temp);

			}
			else
			{
				temp.reason = XmCR_OK;
				XtCallCallbackList ((Widget) sel,
 sel->					selection_box.ok_callback, &temp);

			}
			break;

		case XmDIALOG_APPLY_BUTTON:
			temp.reason = XmCR_APPLY;
			XtCallCallbackList ((Widget) sel, sel->selection_box.apply_callback, &temp);
			break;

		case XmDIALOG_CANCEL_BUTTON:
			temp.reason = XmCR_CANCEL;
			XtCallCallbackList 
			  ((Widget) sel, 
			   sel->selection_box.cancel_callback, &temp);
			break;

		case XmDIALOG_HELP_BUTTON:
			/* Invoke the help system. */
			_XmManagerHelp((XmManagerWidget)sel, callback->event);
			break;
	}
        XmStringFree( temp.value) ;
        return ;
}
   
/****************************************************************/
static void
#ifdef _NO_PROTO
ListCallback (w, client_data, call_data)
            Widget          w ;
            caddr_t         client_data ;
            caddr_t         call_data ;
#else
ListCallback(
            Widget          w,
            caddr_t         client_data,
            caddr_t         call_data)
#endif
/****************           ARGSUSED
 * Process callback from the List in a SelectionBox.
 ****************/
{
	XmListCallbackStruct	*callback = (XmListCallbackStruct *) call_data;
	XmSelectionBoxWidget	sel = (XmSelectionBoxWidget) client_data ;
	XmGadgetClass		gadget_class;
	XmGadget		dbutton = (XmGadget)
                                                BB_DynamicDefaultButton( sel) ;
	char			*text_value;
/****************/

	/*	Update the text widget to relect the latest list selection.
	*	If list default action (double click), activate default button.
	*/
	sel->selection_box.list_selected_item_position = 
                                                       callback->item_position;
	if(    XmStringGetLtoR( callback->item,
                                    XmSTRING_DEFAULT_CHARSET, &text_value)    )
	{   XmTextSetString (SB_Text (sel), text_value);
            XmTextSetCursorPosition (SB_Text (sel), strlen (text_value));
            XtFree( text_value) ;
            } 
        /* Catch only double-click default action here.
        *  Key press events are handled through the ParentProcess routine.
        */
	if(    (callback->reason == XmCR_DEFAULT_ACTION)
            && (callback->event->type != KeyPress)
            && dbutton  &&  XtIsManaged( dbutton)
            && XtIsSensitive( dbutton)  &&  XmIsGadget( dbutton)    )
	{
		gadget_class = (XmGadgetClass) dbutton->object.widget_class ;
		if(    gadget_class->gadget_class.arm_and_activate    )
		{   (*(gadget_class->gadget_class.arm_and_activate))( 
                                                      (Widget) dbutton, NULL) ;
                    } 
	}
        return ;
}


/****************************************************************/
static void
#ifdef _NO_PROTO
UpdateString (w, string, direction)
            Widget			w;
            XmString		string;
            XmStringDirection	direction;
#else
UpdateString(
            Widget          w,
            XmString        string,
            XmStringDirection direction)
#endif
/****************
 * Set the label string of a label or button
 ****************/
{
	Arg		al[3];
    	register int	ac = 0;
/****************/

	if (w)
	{
		XtSetArg (al[ac], XmNstringDirection, direction);  ac++;
		XtSetArg (al[ac], XmNlabelString, string);  ac++;
		XtSetValues (w, al, ac);
	}
        return ;
}

/****************************************************************/
static Boolean
#ifdef _NO_PROTO
SetValues (current, request, new)
            XmSelectionBoxWidget current;
            XmSelectionBoxWidget request;
            XmSelectionBoxWidget new;
#else
SetValues(
            XmSelectionBoxWidget current,
            XmSelectionBoxWidget request,
            XmSelectionBoxWidget new)
#endif
/****************
 * Update widget when values change.
 ****************/
{
	Arg		al[10];
	register int	ac = 0;
	Dimension       w = 0, h = 0;

	XmGeoMatrix    	geoSpec = NULL ;
	String		text_value ;
/****************/

	BB_InSetValues (new) = True;

	/*	Update label strings.
	*/
	if(    new->selection_box.selection_label_string
                          != current->selection_box.selection_label_string    )
	{   UpdateString( SB_SelectionLabel (new), 
                                     new->selection_box.selection_label_string,
                                                    SB_StringDirection (new)) ;
            new->selection_box.selection_label_string = NULL ;
            }

	if(    new->selection_box.list_label_string
                               != current->selection_box.list_label_string    )
	{   UpdateString( SB_ListLabel (new), 
                                          new->selection_box.list_label_string,
                                                    SB_StringDirection (new)) ;
            new->selection_box.list_label_string = NULL ;
            }

	if(    new->selection_box.ok_label_string
                                 != current->selection_box.ok_label_string    )
	{   UpdateString( SB_OkButton (new), 
                                            new->selection_box.ok_label_string,
                                                    SB_StringDirection (new)) ;
            new->selection_box.ok_label_string = NULL ;
            }

	if(    new->selection_box.apply_label_string 
                              != current->selection_box.apply_label_string    )
	{   UpdateString( SB_ApplyButton (new), 
                                         new->selection_box.apply_label_string,
                                                    SB_StringDirection (new)) ;
            new->selection_box.apply_label_string = NULL ;
            }

	if(    new->selection_box.cancel_label_string
                             != current->selection_box.cancel_label_string    )
	{   UpdateString( SB_CancelButton (new), 
                                        new->selection_box.cancel_label_string,
                                                    SB_StringDirection (new)) ;
            new->selection_box.cancel_label_string = NULL ;
            }

	if(    new->selection_box.help_label_string
                               != current->selection_box.help_label_string    )
	{   UpdateString( SB_HelpButton (new), 
                                          new->selection_box.help_label_string,
                                                    SB_StringDirection (new)) ;
            new->selection_box.help_label_string = NULL ;
            }

	/*	Update List widget.
	*/
	ac = 0;
	if(    new->selection_box.list_items    )
	{   
            XtSetArg( al[ac], XmNitems, 
                                       new->selection_box.list_items) ;  ac++ ;
            new->selection_box.list_items = NULL ;
            }
	if(    new->selection_box.list_item_count != UNSPECIFIED    )
	{   XtSetArg( al[ac], XmNitemCount, 
                                  new->selection_box.list_item_count) ;  ac++ ;
            new->selection_box.list_item_count = UNSPECIFIED ;
            }
	if (new->selection_box.list_visible_item_count !=
		current->selection_box.list_visible_item_count)
	{
		XtSetArg (al[ac], XmNvisibleItemCount, 
			new->selection_box.list_visible_item_count);  ac++;
	}
	if (ac)
	{
		if (SB_List (new))
			XtSetValues (SB_List (new), al, ac);
	}


	/*	Update Text widget.
	*/
        text_value = NULL ;
	ac = 0;
	if(    new->selection_box.text_string
                                     != current->selection_box.text_string    )
	{   
            if(    !XmStringGetLtoR( new->selection_box.text_string, 
                                    XmSTRING_DEFAULT_CHARSET, &text_value)    )
            {   text_value = XtMalloc(1 * sizeof(String)) ;
		strcpy(text_value, "");
		/* text_value = ""; */
                } 
            XtSetArg( al[ac], XmNvalue, text_value) ;  ac++ ;
            new->selection_box.text_string = (XmString) UNSPECIFIED ;
            }
	if (new->selection_box.text_columns !=
		current->selection_box.text_columns)
	{
		XtSetArg (al[ac], XmNcolumns, 
			new->selection_box.text_columns);  ac++;
	}
	if (ac)
	{
		if (SB_Text (new))
			XtSetValues (SB_Text (new), al, ac);
	}
	if (text_value)
	{
		if (SB_Text (new))
			XmTextSetCursorPosition (SB_Text (new), 
					strlen (text_value));
		XtFree (text_value);
	}

	/*	Validate dialog type.
	*/
	if (request->selection_box.dialog_type !=
			 current->selection_box.dialog_type)
	{
		_XmWarning (new, WARN_DIALOG_TYPE_CHANGE);
		new->selection_box.dialog_type = 
			current->selection_box.dialog_type;
	}
	BB_InSetValues (new) = False;

	/*	If this is the instantiated class then do layout.
	*/
	if(    XtClass( new) == xmSelectionBoxWidgetClass    )
	{
            _XmBulletinBoardSizeUpdate(( XmBulletinBoardWidget) new) ;
	    }
	return (Boolean) (FALSE);
        }

/****************************************************************/
void
#ifdef _NO_PROTO
_XmSelectionBoxGetSelectionLabelString( sel, resource, value)
            XmSelectionBoxWidget sel ;
            XrmQuark        resource ;
            XtArgVal *      value ;
#else
_XmSelectionBoxGetSelectionLabelString(
            XmSelectionBoxWidget sel,
            XrmQuark        resource,
            XtArgVal *      value)
#endif
/****************           ARGSUSED
 * 
 ****************/
{   
            XmString        data ;
            Arg             al[1] ;
/****************/

    if(    SB_SelectionLabel (sel)    )
    {   
        XtSetArg( al[0], XmNlabelString, &data) ;
        XtGetValues( SB_SelectionLabel( sel), al, 1) ;
        *value = (XtArgVal) data ;
        }
    else
    {   *value = NULL ;
        } 
    return ;
    }
/****************************************************************/
void
#ifdef _NO_PROTO
_XmSelectionBoxGetListLabelString( sel, resource, value)
            XmSelectionBoxWidget sel ;
            XrmQuark        resource ;
            XtArgVal *      value ;
#else
_XmSelectionBoxGetListLabelString(
            XmSelectionBoxWidget sel,
            XrmQuark        resource,
            XtArgVal *      value)
#endif
/****************           ARGSUSED
 * 
 ****************/
{
            XmString        data ;
            Arg             al[1] ;
/****************/

    if(    SB_ListLabel( sel)    )
    {   
        XtSetArg( al[0], XmNlabelString, &data) ;
        XtGetValues( SB_ListLabel( sel), al, 1) ;
        *value = (XtArgVal) data ;
        }
    else
    {   *value = NULL ;
        } 
    return ;
    }
/****************************************************************/
void
#ifdef _NO_PROTO
_XmSelectionBoxGetTextColumns( sel, resource, value)
            XmSelectionBoxWidget sel ;
            XrmQuark        resource ;
            XtArgVal *      value ;
#else
_XmSelectionBoxGetTextColumns(
            XmSelectionBoxWidget sel,
            XrmQuark        resource,
            XtArgVal *      value)
#endif
/****************           ARGSUSED
 * 
 ****************/
{
            short           data ;
            Arg             al[1] ;
/****************/

    if(    SB_Text( sel)    )
    {   
        XtSetArg( al[0], XmNcolumns, &data) ;
        XtGetValues( SB_Text( sel), al, 1) ;
        *value = (XtArgVal) data ;
        }
    else
    {   *value = 0 ;
        } 
    return ;
    }
/****************************************************************/
void
#ifdef _NO_PROTO
_XmSelectionBoxGetTextString( sel, resource, value)
            XmSelectionBoxWidget sel ;
            XrmQuark        resource ;
            XtArgVal *      value ;
#else
_XmSelectionBoxGetTextString(
            XmSelectionBoxWidget sel,
            XrmQuark        resource,
            XtArgVal *      value)
#endif
/****************           ARGSUSED
 * 
 ****************/
{   
            String          data = NULL ;
            XmString        text_string ;
            Arg             al[1] ;
/****************/

    if(    SB_Text( sel)    )
    {   
        XtSetArg( al[0], XmNvalue, &data) ;
        XtGetValues( SB_Text( sel), al, 1) ;
        text_string = XmStringLtoRCreate( data, XmSTRING_DEFAULT_CHARSET) ;
        *value = (XtArgVal) text_string ;
        }
    else
    {   *value = NULL ;
        } 
    return ;
    }
/****************************************************************/
void
#ifdef _NO_PROTO
_XmSelectionBoxGetListItems( sel, resource, value)
            XmSelectionBoxWidget sel ;
            XrmQuark        resource ;
            XtArgVal *      value ;
#else
_XmSelectionBoxGetListItems(
            XmSelectionBoxWidget sel,
            XrmQuark        resource,
            XtArgVal *      value)
#endif
/****************           ARGSUSED
 * 
 ****************/
{   
            Arg             al[1] ;
            XmString        data ;
/****************/

    if(    SB_List( sel)    )
    {   
        XtSetArg( al[0], XmNitems, &data) ;
        XtGetValues( SB_List( sel), al, 1) ;
        *value = (XtArgVal) data ;
        }
    else
    {   *value = NULL ;
        } 
    return ;
    }
/****************************************************************/
void
#ifdef _NO_PROTO
_XmSelectionBoxGetListItemCount( sel, resource, value)
            XmSelectionBoxWidget sel ;
            XrmQuark        resource ;
            XtArgVal *      value ;
#else
_XmSelectionBoxGetListItemCount(
            XmSelectionBoxWidget sel,
            XrmQuark        resource,
            XtArgVal *      value)
#endif
/****************           ARGSUSED
 * 
 ****************/
{   
            int             data ;
            Arg             al[1] ;
/****************/

    if(    SB_List( sel)    )
    {   
        XtSetArg( al[0], XmNitemCount, &data) ;
        XtGetValues( SB_List( sel), al, 1) ;
        *value = (XtArgVal) data ;
        }
    else
    {   *value = 0 ;
        } 
    return ;
    }
/****************************************************************/
void
#ifdef _NO_PROTO
_XmSelectionBoxGetListVisibleItemCount( sel, resource, value)
            XmSelectionBoxWidget sel ;
            XrmQuark        resource ;
            XtArgVal *      value ;
#else
_XmSelectionBoxGetListVisibleItemCount(
            XmSelectionBoxWidget sel,
            XrmQuark        resource,
            XtArgVal *      value)
#endif
/****************           ARGSUSED
 * 
 ****************/
{
            int             data ;
            Arg             al[1] ;
/****************/

    if(    SB_List( sel)    )
    {   
        XtSetArg( al[0], XmNvisibleItemCount, &data) ;
        XtGetValues( SB_List( sel), al, 1) ;
        *value = (XtArgVal) data ;
        }
    else
    {   *value = 0 ;
        } 
    return ;
    }
/****************************************************************/
void
#ifdef _NO_PROTO
_XmSelectionBoxGetOkLabelString( sel, resource, value)
            XmSelectionBoxWidget sel ;
            XrmQuark        resource ;
            XtArgVal *      value ;
#else
_XmSelectionBoxGetOkLabelString(
            XmSelectionBoxWidget sel,
            XrmQuark        resource,
            XtArgVal *      value)
#endif
/****************           ARGSUSED
 * 
 ****************/
{
            XmString        data ;
            Arg             al[1] ;
/****************/

    if(    SB_OkButton( sel)    )
    {   
        XtSetArg( al[0], XmNlabelString, &data) ;
        XtGetValues( SB_OkButton( sel), al, 1) ;
        *value = (XtArgVal) data ;
        }
    else
    {   *value = NULL ;
        } 
    return ;
    }
/****************************************************************/
void
#ifdef _NO_PROTO
_XmSelectionBoxGetApplyLabelString( sel, resource, value)
            XmSelectionBoxWidget sel ;
            XrmQuark        resource ;
            XtArgVal *      value ;
#else
_XmSelectionBoxGetApplyLabelString(
            XmSelectionBoxWidget sel,
            XrmQuark        resource,
            XtArgVal *      value)
#endif
/****************           ARGSUSED
 * 
 ****************/
{
            XmString        data ;
            Arg             al[1] ;
/****************/

    if(    SB_ApplyButton( sel)    )
    {   
        XtSetArg( al[0], XmNlabelString, &data) ;
        XtGetValues( SB_ApplyButton( sel), al, 1) ;
        *value = (XtArgVal) data ;
        }
    else
    {   *value = NULL ;
        } 
    return ;
    }
/****************************************************************/
void
#ifdef _NO_PROTO
_XmSelectionBoxGetCancelLabelString( sel, resource, value)
            XmSelectionBoxWidget sel ;
            XrmQuark        resource ;
            XtArgVal *      value ;
#else
_XmSelectionBoxGetCancelLabelString(
            XmSelectionBoxWidget sel,
            XrmQuark        resource,
            XtArgVal *      value)
#endif
/****************           ARGSUSED
 * 
 ****************/
{
            XmString        data ;
            Arg             al[1] ;
/****************/

    if(    SB_CancelButton( sel)    )
    {   
        XtSetArg( al[0], XmNlabelString, &data) ;
        XtGetValues( SB_CancelButton( sel), al, 1) ;
        *value = (XtArgVal) data ;
        }
    else
    {   *value = NULL ;
        } 
    return ;
    }
/****************************************************************/
void
#ifdef _NO_PROTO
_XmSelectionBoxGetHelpLabelString( sel, resource, value)
            XmSelectionBoxWidget sel ;
            XrmQuark        resource ;
            XtArgVal *      value ;
#else
_XmSelectionBoxGetHelpLabelString(
            XmSelectionBoxWidget sel,
            XrmQuark        resource,
            XtArgVal *      value)
#endif
/****************           ARGSUSED
 * 
 ****************/
{
            XmString        data ;
            Arg             al[1] ;
/****************/

    if(    SB_HelpButton( sel)    )
    {   
        XtSetArg( al[0], XmNlabelString, &data) ;
        XtGetValues( SB_HelpButton( sel), al, 1) ;
        *value = (XtArgVal) data ;
        }
    else
    {   *value = NULL ;
        } 
    return ;
    }

/****************************************************************/
void
#ifdef _NO_PROTO
_XmSelectionBoxUpOrDown( sel, event, argv, argc)
            XmSelectionBoxWidget sel ;
            XEvent *        event ;
            char * *        argv ;
            int             argc ;
#else
_XmSelectionBoxUpOrDown(
            XmSelectionBoxWidget sel,
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
            Arg             av[5] ;
            Cardinal        ac ;
/****************/

    if(    !(list = sel->selection_box.list)    )
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
    position = &(sel->selection_box.list_selected_item_position) ;

    if(    *position == 0    )
    {   /*  No selection, so select first item or last if key_pressed == end.
        */
        if(    key_pressed == 3    )
        {   *position = count ;
            XmListSelectPos( list, *position, True) ;
            } 
        else
        {   XmListSelectPos( list, ++*position, True) ;
            } 
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
_XmSelectionBoxRestore( sel, event, argv, argc)
            XmSelectionBoxWidget sel ;
            XEvent *        event ;
            char * *        argv ;
            int             argc ;
#else
_XmSelectionBoxRestore(
            XmSelectionBoxWidget sel,
            XEvent *        event,
            char * *        argv,
            int             argc)
#endif
/****************           ARGSUSED
 * 
 ****************/
{
            Widget          list ;
            int	            count ;
            XmString *      items ;
            Arg             al[5] ;
            int             ac ;
            String          textItem ;
/****************/

    list = SB_List( sel) ;

    if(    list
        && SB_Text( sel)    )
    {   
        ac = 0 ;
        XtSetArg( al[ac], XmNselectedItems, &items) ; ++ac ;
        XtSetArg( al[ac], XmNselectedItemCount, &count) ; ++ac ;
        XtGetValues( list, al, ac) ;
        if(    count    )
        {   XmStringGetLtoR( *items, XmSTRING_DEFAULT_CHARSET, &textItem) ;
            XmTextSetString( SB_Text( sel), textItem) ;
            XmTextSetCursorPosition( SB_Text( sel), strlen( textItem)) ;
            XtFree( textItem) ;
            } 
        else
        {   XmTextSetString( SB_Text( sel), NULL) ;
            } 
        } 
    return ;
    }

/****************************************************************/
Widget
#ifdef _NO_PROTO
XmSelectionBoxGetChild (sb, which)
            Widget	    sb;	/*  SelectionBox widget	*/
            unsigned char   which;	/*  which child		*/
#else
XmSelectionBoxGetChild(
            Widget          sb,         /*  SelectionBox widget	*/
#if NeedWidePrototypes
    unsigned int   which
#else
    unsigned char   which
#endif 
)      
#endif
/****************
 * This function returns the widget id of a SelectionBox child widget.
 ****************/
{
/****************/
	Widget	child = NULL;

	switch (which)
	{
		case XmDIALOG_LIST:
			child = SB_List (sb);
			break;

		case XmDIALOG_LIST_LABEL:
			child = SB_ListLabel (sb);
			break;

		case XmDIALOG_SELECTION_LABEL:
			child = SB_SelectionLabel (sb);
			break;

		case XmDIALOG_WORK_AREA:
			child = SB_WorkArea (sb);
			break;

		case XmDIALOG_TEXT:
			child = SB_Text (sb);
			break;

		case XmDIALOG_SEPARATOR:
			child = SB_Separator (sb);
			break;

		case XmDIALOG_OK_BUTTON:
			child = SB_OkButton (sb);
			break;

		case XmDIALOG_APPLY_BUTTON:
			child = SB_ApplyButton (sb);
			break;

		case XmDIALOG_CANCEL_BUTTON:
			child = SB_CancelButton (sb);
			break;

		case XmDIALOG_HELP_BUTTON:
			child = SB_HelpButton (sb);
			break;

		case XmDIALOG_DEFAULT_BUTTON:
			child = SB_DefaultButton (sb);
			break;

		default:
			_XmWarning (sb, WARN_CHILD_TYPE);
			break;
	}
	return (child);
}

/****************************************************************/
Widget
#ifdef _NO_PROTO
XmCreateSelectionBox (p, name, args, n)
            Widget		p;	/*  parent widget	*/
            String		name;	/*  widget name		*/
            ArgList		args;	/*  arg list	*/
            Cardinal	n;	/*  arg count	*/
#else
XmCreateSelectionBox(
            Widget		p,	/*  parent widget	*/
            String		name,	/*  widget name		*/
            ArgList		args,	/*  arg list	*/
            Cardinal	        n)	/*  arg count	*/
#endif
/****************
 * This function creates and returns a SelectionBox widget.
 ****************/
{
/****************/

    return (XtCreateWidget (name, xmSelectionBoxWidgetClass, p, args, n));
    }
/****************************************************************/
Widget
#ifdef _NO_PROTO
XmCreateSelectionDialog (ds_p, name, sb_args, sb_n)
            Widget		ds_p;		/*  parent for shell	*/
            String		name;		/*  widget name		*/
            ArgList		sb_args;	/*  arglist for sb	*/
            Cardinal	sb_n;		/*  argcount for sb	*/
#else
XmCreateSelectionDialog(
            Widget		ds_p,		/*  parent for shell	*/
            String		name,		/*  widget name		*/
            ArgList		sb_args,	/*  arglist for sb	*/
            Cardinal	        sb_n)		/*  argcount for sb	*/
#endif
/****************
 * This convenience function creates a DialogShell and a SelectionBox
 *   child of the shell; returns the SelectionBox widget.
 ****************/
{
	Widget		ds;		/*  DialogShell		*/
	Arg		ds_args[10];	/*  arglist for shell	*/
	ArgList		_sb_args;	/*  arglist for sb	*/
	Widget		sb;		/*  new sb widget	*/
	char           *ds_name;
/****************/

	/*  create DialogShell parent
	*/
	ds_name = XtCalloc(strlen(name)+DIALOG_SUFFIX_SIZE+1,sizeof(char));
	strcpy(ds_name,name);
	strcat(ds_name,DIALOG_SUFFIX);

	XtSetArg (ds_args[0], XmNallowShellResize, True);
	ds = XmCreateDialogShell (ds_p, ds_name, ds_args, 1);

	XtFree(ds_name);

	/*  allocate arglist, copy args, add dialog type arg
	*/
	_sb_args = (ArgList) XtMalloc (sizeof (Arg) * (sb_n + 1));

	bcopy (sb_args, _sb_args, sizeof (Arg) * sb_n);
	XtSetArg (_sb_args[sb_n], XmNdialogType, XmDIALOG_SELECTION);  sb_n++;


	/*  create SelectionBox, free args, return 
	*/
	sb = XtCreateWidget (name, xmSelectionBoxWidgetClass, 
		ds, _sb_args, sb_n);
	XtAddCallback (sb, XmNdestroyCallback, _XmDestroyParentCallback, NULL);

	XtFree (_sb_args);
	return (sb);
}

/****************************************************************/
Widget
#ifdef _NO_PROTO
XmCreatePromptDialog (ds_p, name, sb_args, sb_n)
            Widget		ds_p;		/*  parent for shell	*/
            String		name;		/*  widget name		*/
            ArgList		sb_args;	/*  arglist for sb	*/
            Cardinal	sb_n;		/*  arg count for sb	*/
#else
XmCreatePromptDialog(
            Widget		ds_p,		/*  parent for shell	*/
            String		name,		/*  widget name		*/
            ArgList		sb_args,	/*  arglist for sb	*/
            Cardinal	        sb_n)		/*  arg count for sb	*/
#endif
/****************
 * This convenience function creates a DialogShell and a SelectionBox
 *   child of the shell; returns the SelectionBox widget.
 ****************/
{
	Widget		ds;		/*  DialogShell		*/
	Arg		ds_args[10];	/*  arglist for shell	*/
	ArgList		_sb_args;	/*  arglist for sb	*/
	Widget		sb;		/*  new sb widget	*/
	char           *ds_name;
/****************/

	/*  create DialogShell parent
	*/
	ds_name = XtCalloc(strlen(name)+DIALOG_SUFFIX_SIZE+1,sizeof(char));
	strcpy(ds_name,name);
	strcat(ds_name,DIALOG_SUFFIX);

	XtSetArg (ds_args[0], XmNallowShellResize, True);
	ds = XmCreateDialogShell (ds_p, ds_name, ds_args, 1);

	XtFree(ds_name);

	/*  allocate arglist, copy args, add dialog type arg
	*/
	_sb_args = (ArgList) XtMalloc (sizeof (Arg) * (sb_n + 1));

	bcopy (sb_args, _sb_args, sizeof (Arg) * sb_n);
	XtSetArg (_sb_args[sb_n], XmNdialogType, XmDIALOG_PROMPT);  sb_n++;

	/*  create SelectionBox, free args, return 
	*/
	sb = XtCreateWidget (name, xmSelectionBoxWidgetClass, 
		ds, _sb_args, sb_n);
	XtAddCallback (sb, XmNdestroyCallback, _XmDestroyParentCallback, NULL);

	XtFree (_sb_args);
	return (sb);
}
