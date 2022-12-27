#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)Command.c	3.8 90/09/06";
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

#include <Xm/CommandP.h>

#include <Xm/List.h>
#include <Xm/Text.h>


#define ARG_LIST_CNT 25

/* defines for warning messages */
#define WARNING1 "Dialog type must be XmDIALOG_COMMAND."
#define WARNING2 "Invalid child type, Command widget does not have this child."
#define WARNING3 "Invalid XmString, check for invalid charset."
#define WARNING4 "NULL or empty string passed in to CommandAppendValue."
#define WARNING5 "mustMatch is always False for a Command widget."
#define WARNING6 "historyMaxItems must be a positive integer greater than zero."


#ifdef _NO_PROTO

static void             ClassPartInitialize() ;
static void             Initialize() ;
static void             ListCallback() ;
static void             CommandCallback() ;
static Boolean          CommandParentProcess() ;
static Boolean          SetValues() ;

#else /* _NO_PROTO */

static void             ClassPartInitialize( XmCommandWidgetClass wc) ;
static void             Initialize( XmCommandWidget request, 
                                                         XmCommandWidget new) ;
static void             ListCallback( Widget w, caddr_t client_data,
                                                           caddr_t call_data) ;
static void             CommandCallback( Widget w, XmCommandWidget client_data,
                                              XmAnyCallbackStruct *call_data) ;
static Boolean          CommandParentProcess( XmCommandWidget cmd,
                                                   XmParentProcessData event) ;
static Boolean          SetValues( XmCommandWidget old, 
                                XmCommandWidget request, XmCommandWidget new) ;
#endif /* _NO_PROTO */


static XtActionsRec actionsList[] =
{
        { "Return",     (XtActionProc)  _XmCommandReturn },     /* Motif 1.0 */
        { "UpOrDown",   (XtActionProc)  _XmCommandUpOrDown },   /* Motif 1.0 */
        { "BulletinBoardReturn", (XtActionProc)  _XmCommandReturn },
        { "SelectionBoxUpOrDown", (XtActionProc)  _XmCommandUpOrDown },
};


/* define the resource stuff for a command widget */
static XmSyntheticResource syn_resources[] = {

    {   XmNpromptString,
        sizeof (XmString),
        XtOffset (XmSelectionBoxWidget, selection_box.selection_label_string),
        _XmSelectionBoxGetSelectionLabelString,
		NULL },

    {   XmNcommand,
        sizeof (XmString),
        XtOffset (XmSelectionBoxWidget, selection_box.text_string),
        _XmSelectionBoxGetTextString,
		NULL },

    {   XmNhistoryItems,
        sizeof (XmString *),
        XtOffset (XmSelectionBoxWidget, selection_box.list_items),
        _XmSelectionBoxGetListItems,
		NULL },

    {   XmNhistoryItemCount,
        sizeof(int),
        XtOffset (XmSelectionBoxWidget, selection_box.list_item_count),
        _XmSelectionBoxGetListItemCount,
		NULL },

    {   XmNhistoryVisibleItemCount,
        sizeof(int),
        XtOffset (XmSelectionBoxWidget, selection_box.list_visible_item_count),
        _XmSelectionBoxGetListVisibleItemCount,
		NULL },
};


static XtResource resources[] = 
{
    {   XmNcommandEnteredCallback, 
        XmCCallback, 
        XmRCallback, 
        sizeof (XtCallbackList),
        XtOffset (XmCommandWidget, command.callback), 
        XmRCallback, 
        (caddr_t)NULL},

    {   XmNcommandChangedCallback, 
        XmCCallback, 
        XmRCallback, 
        sizeof (XtCallbackList),
        XtOffset (XmCommandWidget, command.value_changed_callback), 
        XmRCallback, 
        (caddr_t) NULL},

    {   XmNpromptString,
        XmCPromptString, 
        XmRXmString, 
        sizeof (XmString),
        XtOffset (XmSelectionBoxWidget, selection_box.selection_label_string),
        XmRString, 
        ">"},

    {   XmNcommand,
        XmCTextString, 
        XmRXmString, 
        sizeof (XmString),
        XtOffset (XmSelectionBoxWidget, selection_box.text_string),
        XmRImmediate, 
        (caddr_t) UNSPECIFIED},

    {   XmNhistoryItems,
        XmCItems, 
        XmRXmStringTable, 
        sizeof (XmString *),
        XtOffset (XmSelectionBoxWidget, selection_box.list_items),
        XmRImmediate, 
        NULL},

    {   XmNhistoryItemCount,
        XmCItemCount, 
        XmRInt, 
        sizeof(int),
        XtOffset (XmSelectionBoxWidget, selection_box.list_item_count),
        XmRImmediate, 
        (caddr_t) UNSPECIFIED},

    {   XmNhistoryMaxItems,
        XmCMaxItems,
        XmRInt, 
        sizeof(int),
        XtOffset (XmCommandWidget, command.history_max_items),
        XmRImmediate, 
        (caddr_t) 100},

    {   XmNhistoryVisibleItemCount,
        XmCVisibleItemCount, 
        XmRInt, 
        sizeof(int),
        XtOffset (XmSelectionBoxWidget, selection_box.list_visible_item_count),
        XmRImmediate, 
        (caddr_t) 8},

    {   XmNdialogType,
        XmCDialogType, 
        XmRDialogType, 
        sizeof (unsigned char),
        XtOffset (XmSelectionBoxWidget, selection_box.dialog_type),
        XmRImmediate, 
        (caddr_t) XmDIALOG_COMMAND},

    {   XmNdefaultPosition,
        XmCDefaultPosition, 
        XmRBoolean, 
        sizeof (Boolean),
        XtOffset (XmBulletinBoardWidget, bulletin_board.default_position),
        XmRImmediate, 
        (caddr_t) False},

    {   XmNautoUnmanage,
        XmCAutoUnmanage,
        XmRBoolean, 
        sizeof (Boolean),
        XtOffset (XmBulletinBoardWidget, bulletin_board.auto_unmanage),
        XmRImmediate,
        (caddr_t) False},

    {   XmNresizePolicy,
        XmCResizePolicy,
        XmRResizePolicy,
        sizeof (unsigned char),
        XtOffset (XmBulletinBoardWidget, bulletin_board.resize_policy),
        XmRImmediate,
        (caddr_t) XmRESIZE_NONE},


};


/****************************************************************
 *
 * Full class record constant
 *
 ****************************************************************/

externaldef( xmcommandclassrec) XmCommandClassRec xmCommandClassRec =
{
    {                                            /*core class record         */
        (WidgetClass) &xmSelectionBoxClassRec,   /* superclass ptr           */
        "XmCommand",                             /* class_name               */
        sizeof(XmCommandRec),                    /* size of widget instance  */
        NULL,                         /* class init proc          */
        ClassPartInitialize,                     /* class part init proc     */
        FALSE,                                   /* class is not init'ed     */
        Initialize,                              /* widget init proc         */
        NULL,                                    /* widget init hook         */
        _XtInherit,                              /* widget realize proc      */
        actionsList,                             /* class action table       */
        XtNumber (actionsList),                  /* class action count       */
        resources,                               /* class resource list      */
        XtNumber (resources),                    /* class  resource_count    */
        NULLQUARK,                               /* xrm_class                */
        TRUE,                                    /* compressed motion        */
        TRUE,                                    /* compressed exposure      */
        TRUE,                                    /* compressed enter/exit    */
        FALSE,                                   /* VisibilityNotify         */
        NULL,                                    /* class destroy proc       */
        _XtInherit,                              /* class resize proc        */
        _XtInherit,                              /* class expose proc        */
        SetValues,                               /* class set_value proc     */
        NULL,                                    /* widget setvalues hook    */
        _XtInherit,                              /* set values almost        */
        NULL,                                    /* widget getvalues hook    */
        NULL,                                    /* class accept focus proc  */
        XtVersion,                               /* version                  */
        NULL,                                    /* callback offset list     */
        XtInheritTranslations,                   /* default translations     */
        (XtGeometryHandler)_XtInherit,           /* query geometry           */
        NULL,                                    /* Display Accelerator      */
        NULL,                                    /* Extension pointer        */
    },
    {                                            /*composite class record    */
        (XtGeometryHandler)_XtInherit,           /* childrens geo mgr proc   */
        _XtInherit,                              /* set changed proc         */
        _XtInherit,                              /* add a child              */
        _XtInherit,                              /* remove a child           */
        NULL,                                    /* Extension pointer        */
    },
    {                                            /*constraint class record   */
        NULL,                                    /* no additional resources  */
        0,                                       /* num additional resources */
        0,                                       /* size of constraint rec   */
        NULL,                                    /* constraint_initialize    */
        NULL,                                    /* constraint_destroy       */
        NULL,                                    /* constraint_setvalue      */
        NULL,                                    /* Extension pointer        */
    },
    {                                            /*manager class record      */
        XtInheritTranslations,                   /* default translations     */
        syn_resources,                           /* syn_resources            */
        XtNumber(syn_resources),                 /* num_syn_resources        */
        NULL,                                    /* syn_cont_resources       */
        0,                                       /* num_syn_cont_resources   */
        CommandParentProcess,                    /* parent_process           */
        NULL,                                    /* extension                */
    },
    {                                            /*bulletin board class rec  */
        TRUE,                                    /*always_install_accelerators*/
        XtInheritGeoMatrixCreate,                /* geo_matrix_create */
        XtInheritFocusMovedProc,                 /* focus_moved_proc */
        NULL                                     /* Extension pointer - none */
    },
    {                                            /*selection box class rec  */
        ListCallback,                            /* list_callback */
        NULL,                                    /* Extension pointer - none */
    },
    {                                            /*command class record      */
        NULL,                                    /* Extension pointer - none */
    }
};


/*
 * now make a public symbol that points to this class record
 */

externaldef( xmcommandwidgetclass) WidgetClass xmCommandWidgetClass
                                           = (WidgetClass) &xmCommandClassRec ;


/****************************************************************/
static void
#ifdef _NO_PROTO
ClassPartInitialize(wc)
            XmCommandWidgetClass wc;
#else
ClassPartInitialize(
            XmCommandWidgetClass wc)
#endif
/****************
 * 
 ****************/
{
/****************/

    _XmFastSubclassInit (wc, XmCOMMAND_BOX_BIT);

    return ;
    }
/****************************************************************/
static void
#ifdef _NO_PROTO
Initialize (request, new)
            XmCommandWidget request ;
            XmCommandWidget new ;
#else
Initialize(
            XmCommandWidget request,
            XmCommandWidget new)
#endif
/****************           ARGSUSED
 * Set attributes of a command widget
 ****************/
{
            int max;
            Arg             argv[5] ;
            Cardinal        argc ;
/****************/

    /* mustMatch does not apply to command... (it is always false) */
    if (new->selection_box.must_match != False) { 
        new->selection_box.must_match = False; 
        _XmWarning(new, WARNING5);
    }

    /* check for and change max history items (remove items if needed) */
    if (new->command.history_max_items < 1) {
        new->command.history_max_items = 100;
        _XmWarning (new, WARNING6);
    }
    argc = 0 ;
    XtSetArg( argv[argc], XmNitemCount, &max) ; ++argc ;
    XtGetValues( SB_List( new), argv, argc) ;

    if (new->command.history_max_items < max)
    {
        while (max > new->command.history_max_items) {
            XmListDeletePos (new->selection_box.list, 1);
            if (new->selection_box.list_selected_item_position > 0)
                new->selection_box.list_selected_item_position--;
            max--;
        }
    }

    /* if dialogType is not XmDIALOG_COMMAND, issue _XmWarning */
    if (new->selection_box.dialog_type != XmDIALOG_COMMAND)
        _XmWarning (new, WARNING1);

    /* add callback to text for value changed callback */
    XtAddCallback (new->selection_box.text, XmNvalueChangedCallback, 
                                                         CommandCallback, new);
    /* there is no error currently in list */
    new->command.error = FALSE;

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
            XmListCallbackStruct * listCB = (XmListCallbackStruct *)
                                                                    call_data ;
            XmCommandCallbackStruct cmdCB ;
            XmCommandWidget cmdWid = (XmCommandWidget) client_data ;
            XmGadgetClass   gadget_class ;
            char *          text_value ;
            Arg             argv[5] ;
            Cardinal        argc ;
            int             count ;
            XmString        tmpXmString ;
            Boolean         reset = FALSE ;
/****************/

    argc = 0 ;
    XtSetArg( argv[argc], XmNitemCount, &count) ; ++argc ;
    XtGetValues( SB_List( cmdWid), argv, argc) ;

    if(    !count    )
    {   return ;
        } 
    if(    cmdWid->command.error    )
    {   
        if(    (listCB->item_position == (count - 1))
            || (    (listCB->item_position == count)
                 && (listCB->reason != XmCR_DEFAULT_ACTION))    )
        {   
            /* Selection or default action on the blank line, or selection on
            *   error message.  Restore previous selection and return.
            */
            XmListDeselectPos( SB_List( cmdWid), listCB->item_position) ;

            if(    cmdWid->selection_box.list_selected_item_position    )
            {   XmListSelectPos( SB_List( cmdWid), 
                    cmdWid->selection_box.list_selected_item_position, FALSE) ;
                } 
            return ;
            } 
        XmListDeletePos( SB_List( cmdWid), 0) ;   /* Delete error message.*/
        XmListDeletePos( SB_List( cmdWid), 0) ;   /* Delete blank line.*/
        cmdWid->command.error = FALSE ;

        if(    count <= 2    )
        {   /* List only contained error message and blank line.
            */
            cmdWid->selection_box.list_selected_item_position = 0 ;
            return ;
            } 
        count -= 2 ;

        if(    (listCB->item_position > count)
            && (listCB->reason == XmCR_DEFAULT_ACTION)    )
        {   
            /* Default action on the error message line.  Restore previous
            *   selection, clear text area, and return.
            */
            if(    cmdWid->selection_box.list_selected_item_position    )
            {   XmListSelectPos( SB_List( cmdWid), 
                    cmdWid->selection_box.list_selected_item_position, FALSE) ;
                } 
            XmTextSetString( cmdWid->selection_box.text, "") ;
            return ;
            }
        }
    if(    listCB->reason == XmCR_DEFAULT_ACTION    )
    {   
        /* If we are already at max items, remove first item.
        */
        if(    count >= cmdWid->command.history_max_items    )
        {
            XmListDeletePos( cmdWid->selection_box.list, 1) ;

            if(    cmdWid->selection_box.list_selected_item_position > 0    )
            {   cmdWid->selection_box.list_selected_item_position-- ;
                } 
            }
        tmpXmString = XmStringCopy( listCB->item) ;
        XmListAddItemUnselected( cmdWid->selection_box.list, tmpXmString, 0) ;

        XmListSetBottomPos( cmdWid->selection_box.list, 0) ;
        XmTextSetString( cmdWid->selection_box.text, "") ;

        /* Call the users command entered callback.
        */
        cmdCB.reason = XmCR_COMMAND_ENTERED ;
        cmdCB.event  = NULL ;
        cmdCB.value  = tmpXmString ;
        cmdCB.length = XmStringLength( tmpXmString) ;
        XtCallCallbackList ((Widget) cmdWid, cmdWid->command.callback, &cmdCB) ;

        XmStringFree( tmpXmString) ;
        } 
    else /* listCB->reason is BROWSE_SELECT or SINGLE_SELECT */
    {   /* Update the text widget to relect the latest list selection.
        */
        cmdWid->selection_box.list_selected_item_position = 
                                                        listCB->item_position ;
        if(    XmStringGetLtoR( listCB->item, 
                                    XmSTRING_DEFAULT_CHARSET, &text_value)    )
        {   XmTextSetString( SB_Text( cmdWid), text_value) ;
            XmTextSetCursorPosition( SB_Text( cmdWid), strlen( text_value)) ;
            XtFree( text_value) ;
            } 
        } 
    return ;
    }
/****************************************************************/
static void
#ifdef _NO_PROTO
CommandCallback (w, client_data, call_data)
            Widget          w ;
            XmCommandWidget client_data ;
            XmAnyCallbackStruct * call_data ;
#else
CommandCallback(
            Widget          w,
            XmCommandWidget client_data,
            XmAnyCallbackStruct * call_data)
#endif
/****************           ARGSUSED
 * Callback for Text ValueChanged callback
 ****************/
{
    XmCommandCallbackStruct  cb;
    char *str;
/****************/

    cb.reason = XmCR_COMMAND_CHANGED;
    cb.event  = call_data->event;

    /* get char* string from text and convert to XmString type */
    str = XmTextGetString (client_data->selection_box.text);
    cb.value = XmStringLtoRCreate (str, XmSTRING_DEFAULT_CHARSET);
    XtFree (str);

    cb.length = XmStringLength (cb.value);

    XtCallCallbackList ((Widget) client_data, client_data->command.value_changed_callback, &cb);
    XmStringFree (cb.value);
    return ;
}

/****************************************************************/
static Boolean
#ifdef _NO_PROTO
CommandParentProcess( cmd, event)
            XmCommandWidget cmd ;
            XmParentProcessData event ;
#else /* _NO_PROTO */
CommandParentProcess(
            XmCommandWidget cmd,
            XmParentProcessData event)
#endif /* _NO_PROTO */

/****************
 * 
 ****************/
{
/****************/

    if(    (event->process_type == XmINPUT_ACTION)
        && (   (event->data.input_action.action == XmRETURN)
            || (    (event->data.input_action.action == XmCANCEL)
                 && BB_CancelButton( cmd)))    )
    {   
        if(    event->data.input_action.action == XmRETURN    )
        {   
            _XmCommandReturn( cmd, event->data.input_action.event,
                                      event->data.input_action.params,
                                         event->data.input_action.num_params) ;
            } 
        else
        {   _XmBulletinBoardCancel( ( XmBulletinBoardWidget)cmd, 
				     event->data.input_action.event,
                                      event->data.input_action.params,
                                         event->data.input_action.num_params) ;
            } 
        return( TRUE) ;
        } 

    return( _XmParentProcess( XtParent( cmd), event)) ;
    }

/****************************************************************/
void
#ifdef _NO_PROTO
_XmCommandReturn( w, event, params, numParams)
            XmCommandWidget w ;
            XEvent *        event ;
            String *        params ;
            Cardinal        numParams ;
#else
_XmCommandReturn(
            XmCommandWidget w,
            XEvent *        event,
            String *        params,
            Cardinal        numParams)
#endif
/****************           ARGSUSED
 * Return function to "complete" text entry
 ****************/
{
    XmCommandCallbackStruct cb;
    XmString                   tmpXmString;
    String                     tmpString;
            Arg             argv[5] ;
            Cardinal        argc ;
            int             count ;
/****************/

    /* if an error has been sent to the history list, remove it now */
    if (w->command.error) {
        XmListDeletePos (w->selection_box.list, 0);     /* delete error msg  */
        XmListDeletePos (w->selection_box.list, 0);     /* delete blank line */
        w->command.error = FALSE;
        XmListSetBottomPos (w->selection_box.list, 0);
    }

    /* update the history:  - get text, check null/empty  */
    /*                      - create XmString from text   */
    /*                      - increment local list count  */
    /*                      - add to history list         */
    /*                      - delete first element if too */
    /*                        many items (maxItemCount)   */
    /*                      - position list to end        */
    /*                      - reset selection list ptr    */
    /*                      - reset command to ""         */

    tmpString = XmTextGetString (w->selection_box.text);
    if ((tmpString == NULL) || (strcmp(tmpString, "") == 0)) { 
        if (tmpString) XtFree(tmpString);
        return;
    }
    argc = 0 ;
    XtSetArg( argv[argc], XmNitemCount, &count) ; ++argc ;
    XtGetValues( SB_List( w), argv, argc) ;
    /* if already at max items, remove first item in list */

    if (count >= w->command.history_max_items) 
    {
        XmListDeletePos (w->selection_box.list, 1);
        if (w->selection_box.list_selected_item_position > 0)
            w->selection_box.list_selected_item_position--;
    }
    tmpXmString = XmStringLtoRCreate (tmpString, XmSTRING_DEFAULT_CHARSET);
    XmListAddItemUnselected (w->selection_box.list, tmpXmString, 0);

    XmListSetBottomPos (w->selection_box.list, 0);
    XmTextSetString (w->selection_box.text, "");


    /* call the users command entered callback */

    cb.reason = XmCR_COMMAND_ENTERED;
    cb.event  = event;
    cb.value  = tmpXmString;
    cb.length = XmStringLength (tmpXmString);
    XtCallCallbackList ((Widget) w, w->command.callback, &cb);
    XmStringFree (tmpXmString);
    XtFree (tmpString);
    return ;
}
/****************************************************************/
void
#ifdef _NO_PROTO
_XmCommandUpOrDown( cmd, event, argv, argc)
            XmCommandWidget cmd ;
            XEvent *        event ;
            char * *        argv ;
            int             argc ;
#else
_XmCommandUpOrDown(
            XmCommandWidget cmd,
            XEvent *        event,
            char * *        argv,
            int             argc)
#endif
/****************           ARGSUSED
 * 
 ****************/
{
            int             visible ;
            int	            top ;
            int	            key_pressed ;
            Widget	    list ;
            int	*           position ;
            int	            count ;
            Arg             av[5] ;
            Cardinal        ac ;
/****************/

    if(    !(list = cmd->selection_box.list)    )
    {   return ;
        } 
    ac = 0 ;
    XtSetArg( av[ac], XmNitemCount, &count) ; ++ac ;
    XtSetArg( av[ac], XmNtopItemPosition, &top) ; ++ac ;
    XtSetArg( av[ac], XmNvisibleItemCount, &visible) ; ++ac ;
    XtGetValues( (XmListWidget) list, av, ac) ;

    if(    !count
        || (cmd->command.error  &&  (count <= 2))    )
    {   return ;
        } 
    key_pressed = atoi( *argv) ;
    position = &(cmd->selection_box.list_selected_item_position) ;

    if(    *position == 0    )
    {   
        /* If error is visible, select last item - 2.  Otherwise, select
        *   last item in list.
        */
        if(    cmd->command.error    )
        {   *position = count - 2 ;
            } 
        else
        {   *position = count ;
            } 
        XmListSelectPos( list, *position, True) ;
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
static Boolean
#ifdef _NO_PROTO
SetValues( old, request, new)
            XmCommandWidget old ;
            XmCommandWidget request ;
            XmCommandWidget new ;
#else
SetValues(
            XmCommandWidget old,
            XmCommandWidget request,
            XmCommandWidget new)
#endif
/****************           ARGSUSED
 * Set attributes of a command widget
 ****************/
{
    int max;
            Arg             argv[5] ;
            Cardinal        argc ;
/****************/

    /* mustMatch does not apply to command... (it is always false) */
    if (new->selection_box.must_match != False) { 
        new->selection_box.must_match = False; 
        _XmWarning(new, WARNING5);
    }

    /* if dialogType is not XmDIALOG_COMMAND, issue _XmWarning */
    if (new->selection_box.dialog_type != XmDIALOG_COMMAND) {
        new->selection_box.dialog_type = XmDIALOG_COMMAND;
        _XmWarning (new, WARNING1);
    }

    /* check for and change max history items (remove items if needed) */
    if (new->command.history_max_items < 1) {
        new->command.history_max_items = old->command.history_max_items;
        _XmWarning (new, WARNING6);
    }
    if (new->command.history_max_items < 
        old->command.history_max_items)
    {
        argc = 0 ;
        XtSetArg( argv[argc], XmNitemCount, &max) ; ++argc ;
        XtGetValues( SB_List( new), argv, argc) ;

        while (max > new->command.history_max_items) {
            XmListDeletePos (new->selection_box.list, 1);
            if (new->selection_box.list_selected_item_position > 0)
                new->selection_box.list_selected_item_position--;
            max--;
        }
    }
    /* require redisplay */
    return(True);
}
/****************************************************************/
Widget
#ifdef _NO_PROTO
XmCreateCommand( parent, name, al, ac)
            Widget      parent;
            char       *name;
            ArgList     al;
            Cardinal    ac;
#else
XmCreateCommand(
            Widget      parent,
            char       *name,
            ArgList     al,
            Cardinal    ac)
#endif
/****************
 * Create an instance of the widget
 ****************/
{
    Widget   w;
    ArgList  argsNew;
/****************/

    /* add dialogType to arglist and force to XmDIALOG_COMMAND... */
    /* big time bad stuff will happen if they use prompt type...  */
    /* (like, no list gets created, but used all through command) */

    /*  allocate arglist, copy args, add dialog type arg */
    argsNew = (ArgList) XtMalloc (sizeof(Arg) * (ac + 1));
    bcopy (al, argsNew, sizeof(Arg) * ac);
    XtSetArg (argsNew[ac], XmNdialogType, XmDIALOG_COMMAND);  ac++;

    /*  create Command, free argsNew, return */
    w = XtCreateWidget (name, xmCommandWidgetClass, parent, argsNew, ac);
    XtFree (argsNew);

    return (w);
}
/****************************************************************/
Widget
#ifdef _NO_PROTO
XmCommandGetChild (widget, child)
            Widget   widget;
            unsigned char   child;
#else
XmCommandGetChild(
            Widget   widget,
#if NeedWidePrototypes
    unsigned int   child
#else
    unsigned char   child
#endif 
)
#endif
/****************
 * 
 ****************/
{
    XmCommandWidget   w = (XmCommandWidget)widget;
/****************/

    switch (child) {
        case XmDIALOG_COMMAND_TEXT:    return w->selection_box.text;
        case XmDIALOG_HISTORY_LIST:    return w->selection_box.list;
        case XmDIALOG_PROMPT_LABEL:    return w->selection_box.selection_label;

        default: _XmWarning (w, WARNING2);
    }
    return NULL ;
}
/****************************************************************/
void
#ifdef _NO_PROTO
XmCommandSetValue (widget, value)
            Widget   widget;
            XmString value;
#else
XmCommandSetValue(
            Widget   widget,
            XmString value)
#endif
/****************
 * Replace the text value with "value" 
 *     note: selection_box.text_string isn't updated, as we are not
 *           going to guarantee that text_string is up to date with
 *           the text widget string at all times...
 ****************/
{
    XmCommandWidget   w = (XmCommandWidget)widget;
    char *str;
/****************/

    if ( !XmStringGetLtoR (value, XmSTRING_DEFAULT_CHARSET, &str) ) {
        _XmWarning (w, WARNING3);
        return;
    }

    XmTextSetString(w->selection_box.text, str);
    return ;
}
/****************************************************************/
void
#ifdef _NO_PROTO
XmCommandAppendValue (widget, value)
            Widget widget;
            XmString value;
#else
XmCommandAppendValue(
            Widget widget,
            XmString value)
#endif
/****************
 * Append "value" to the end of the current text value
 ****************/
{
    XmCommandWidget   w = (XmCommandWidget)widget;
    char            *strNew;
    char            *tmpStr;
    XmTextPosition   endPosition;
/****************/

    if (value == NULL) return;

    /* return if invalid string from "value" passed in */
    if ( !XmStringGetLtoR (value, XmSTRING_DEFAULT_CHARSET, &strNew) ) {
        _XmWarning (w, WARNING3);
        return;
    }
    if ((strNew == NULL) || (strcmp(strNew, "") == 0)) {
        _XmWarning (w, WARNING4);
        return;
    }

    /* get string length of current command string */
    tmpStr = XmTextGetString (w->selection_box.text);
    endPosition = strlen (tmpStr);
    XtFree (tmpStr);

    /* append new string to existing string */
    XmTextReplace (w->selection_box.text, endPosition, endPosition, strNew);

    /* reset insertion position to end of text, and free new string */
    endPosition += strlen(strNew);
    XmTextSetCursorPosition (w->selection_box.text, endPosition);
    XtFree (strNew);
    return ;
}
/****************************************************************/
void
#ifdef _NO_PROTO
XmCommandError( widget, error)
            Widget widget;
            XmString error;
#else
XmCommandError(
            Widget widget,
            XmString error)
#endif
/****************
 * Print a blank string and error string, which will get removed the next
 *   time a string is added to the history.
 ****************/
{
    XmCommandWidget   w = (XmCommandWidget)widget;
    XmString  blankXmString;
/****************/

    if (error == NULL) return;

    /* If error is currently in list remove it, but leave (or add) blank item.
    */
    if(    w->command.error    )
    {
        XmListDeletePos (w->selection_box.list, 0);
        }
    else
    {   blankXmString = XmStringLtoRCreate (" ", XmSTRING_DEFAULT_CHARSET);
        XmListAddItemUnselected (w->selection_box.list, blankXmString, 0);
        XmStringFree (blankXmString);
        w->command.error = TRUE;
        }

    /* create and add the error string as a list item */
    XmListAddItemUnselected (w->selection_box.list, error, 0);
    XmListSetBottomPos (w->selection_box.list, 0);

    return ;
    }
