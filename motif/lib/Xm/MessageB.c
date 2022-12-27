#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)MessageB.c	3.19 91/01/10";
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

#include <Xm/MessageBP.h>

#include <Xm/LabelG.h>
#include <Xm/PushBG.h>
#include <Xm/DialogS.h>
#include <Xm/SeparatoG.h>


/* convenience macros */
#define Double(d)        ((d) << 1)
#define TotalWidth(w)    ((w)->core.width + Double ((w)->core.border_width))
#define TotalHeight(w)   ((w)->core.height + Double ((w)->core.border_width))
#define BottomEdge(w)    ((w)->core.y + TotalHeight (w))

#define MB_MAX_WIDGETS_VERT 3
#define MB_MAX_NUM_WIDGETS  7

#define ARG_LIST_CNT    25

#define PIXMAP_LIST_MOD 6

/* defines for warning message */
#define WARNING1 "Invalid Dialog Type."
#define WARNING2 "Invalid Default Button Type."
#define WARNING3 "Invalid Alignment Type."
#define WARNING4 "Invalid Child Type."
#define WARNING5 "PushButton Id cannot be changed directly."
#define WARNING6 "Use XmNdefaultButtonType to set MessageBox default button."


#ifdef _NO_PROTO

static XImage *         CreateDefaultImage() ;
static void             ClassInitialize() ;
static void             ClassPartInitialize() ;
static void             Initialize() ;
static void             Destroy() ;
static void             DeleteChild() ;
static void             MessageCallback() ;
static Boolean          SetValues() ;
static void             SetUpSymbox() ;
static void             SetUpMessage() ;
static Widget           CreateShell() ;
static void             CreateWidgets() ;
static Widget           _XmCreateDialog() ;
static void             GetMessageString() ;
static void             GetSymbolPixmap() ;
static void             GetOkLabelString() ;
static void             GetCancelLabelString() ;
static void             GetHelpLabelString() ;
static void             GetMsgBoxPixmap() ;

#else /* _NO_PROTO */

static XImage *         CreateDefaultImage( char * bits, unsigned int width,
                                                         unsigned int height) ;
static void             ClassInitialize() ;
static void             ClassPartInitialize( XmMessageBoxWidgetClass wc) ;
static void             Initialize( XmMessageBoxWidget request, 
                                                      XmMessageBoxWidget new) ;
static void             Destroy( XmMessageBoxWidget d) ;
static void             DeleteChild( Widget child) ;
static void             MessageCallback( Widget w, XmMessageBoxWidget tag,
                                              XmAnyCallbackStruct * callback) ;
static Boolean          SetValues( XmMessageBoxWidget r,
                            XmMessageBoxWidget request, XmMessageBoxWidget s) ;
static void             SetUpSymbox( XmMessageBoxWidget w) ;
static void             SetUpMessage( XmMessageBoxWidget w) ;
static Widget           CreateShell( Widget parent, char * name, ArgList args,
                                                                 Cardinal ac) ;
static void             CreateWidgets( XmMessageBoxWidget w) ;
static Widget           _XmCreateDialog( Widget parent, char * name, 
                                      ArgList al, int ac, unsigned char type) ;
static void             GetMessageString( XmMessageBoxWidget w,
                                         XrmQuark resource, XtArgVal * value) ;
static void             GetSymbolPixmap( XmMessageBoxWidget w,
                                         XrmQuark resource, XtArgVal * value) ;
static void             GetOkLabelString( XmMessageBoxWidget w, 
                                         XrmQuark resource, XtArgVal * value) ;
static void             GetCancelLabelString( XmMessageBoxWidget w,
                                         XrmQuark resource, XtArgVal * value) ;
static void             GetHelpLabelString( XmMessageBoxWidget w,
                                         XrmQuark resource, XtArgVal * value) ;
static void             GetMsgBoxPixmap( XmMessageBoxWidget mBox) ;

#endif /* _NO_PROTO */


/*  Resource definitions, "get resources" first */

static XmSyntheticResource syn_resources[] = {

    {   XmNmessageString,
        sizeof (XmString),
        XtOffset (XmMessageBoxWidget, message_box.message_string),
        GetMessageString,
		NULL},

    {   XmNsymbolPixmap,
        sizeof (Pixmap),
        XtOffset (XmMessageBoxWidget, message_box.symbol_pixmap),
        GetSymbolPixmap,
		NULL},

    {   XmNokLabelString,
        sizeof (XmString),
        XtOffset (XmMessageBoxWidget, message_box.ok_label_string),
        GetOkLabelString,
		NULL},

    {   XmNcancelLabelString,
        sizeof (XmString),
        XtOffset (XmMessageBoxWidget, message_box.cancel_label_string),
        GetCancelLabelString,
		NULL},

    {   XmNhelpLabelString,
        sizeof (XmString),
        XtOffset (XmMessageBoxWidget, message_box.help_label_string),
        GetHelpLabelString,
		NULL},
};


static XtResource resources[] = {

    {   XmNdialogType,
        XmCDialogType,
        XmRDialogType,
        sizeof(unsigned char),
        XtOffset (XmMessageBoxWidget, message_box.dialog_type),
        XmRImmediate,
        (caddr_t) XmDIALOG_MESSAGE},

    {   XmNminimizeButtons,
        XmCMinimizeButtons,
        XmRBoolean,
        sizeof(Boolean),
        XtOffset (XmMessageBoxWidget, message_box.minimize_buttons),
        XmRImmediate,
        (caddr_t) False},

    {   XmNdefaultButtonType,
        XmCDefaultButtonType,
        XmRDefaultButtonType,
        sizeof(unsigned char),
        XtOffset (XmMessageBoxWidget, message_box.default_type),
        XmRImmediate,
        (caddr_t) XmDIALOG_OK_BUTTON},

    {   XmNmessageString,
        XmCMessageString,
        XmRXmString,
        sizeof (XmString),
        XtOffset (XmMessageBoxWidget, message_box.message_string),
        XmRXmString,
        NULL},

    {   XmNmessageAlignment,
        XmCAlignment,
        XmRAlignment,
        sizeof(unsigned char),
        XtOffset(XmMessageBoxWidget, message_box.message_alignment),
        XmRImmediate,
        (caddr_t) XmALIGNMENT_BEGINNING},

    {   XmNsymbolPixmap,
        XmCPixmap,
        XmRManForegroundPixmap,
        sizeof (Pixmap),
        XtOffset (XmMessageBoxWidget, message_box.symbol_pixmap),
        XmRImmediate,
        (caddr_t) XmUNSPECIFIED_PIXMAP},

    {   XmNokLabelString,
        XmCOkLabelString,
        XmRXmString,
        sizeof (XmString),
        XtOffset (XmMessageBoxWidget, message_box.ok_label_string),
        XmRXmString,
        NULL},             /* "OK" default dynamically set from label name */

    {   XmNokCallback,
        XmCCallback,
        XmRCallback,
        sizeof (XtCallbackList),
        XtOffset (XmMessageBoxWidget, message_box.ok_callback),
        XmRCallback,
        NULL},

    {   XmNcancelLabelString,
        XmCCancelLabelString,
        XmRXmString,
        sizeof (XmString),
        XtOffset (XmMessageBoxWidget, message_box.cancel_label_string),
        XmRXmString,
        NULL},            /* "Cancel" default dynamically set from label name */

    {   XmNcancelCallback,
        XmCCallback,
        XmRCallback,
        sizeof (XtCallbackList),
        XtOffset (XmMessageBoxWidget, message_box.cancel_callback),
        XmRCallback,
        NULL},

    {   XmNhelpLabelString,
        XmCHelpLabelString,
        XmRXmString,
        sizeof (XmString),
        XtOffset (XmMessageBoxWidget, message_box.help_label_string),
        XmRXmString,
        NULL},             /* "Help" default dynamically set from label name */

};



/****************************************************************
 *
 * Full class record constant
 *
 ****************************************************************/

externaldef( xmmessageboxclassrec) XmMessageBoxClassRec xmMessageBoxClassRec =
{
   {                                            /* core_class fields  */
      (WidgetClass) &xmBulletinBoardClassRec,   /* superclass         */
      "XmMessageBox",                           /* class_name         */
      sizeof(XmMessageBoxRec),                  /* widget_size        */
      ClassInitialize,                          /* class_initialize   */
      ClassPartInitialize,                      /* class_part_init    */
      FALSE,                                    /* class_inited       */
      (XtInitProc) Initialize,                  /* initialize         */
      NULL,                                     /* initialize_hook    */
      _XtInherit,                               /* realize            */
      NULL,                                     /* actions            */
      0,                                        /* num_actions        */
      resources,                                /* resources          */
      XtNumber(resources),                      /* num_resources      */
      NULLQUARK,                                /* xrm_class          */
      TRUE,                                     /* compress_motion    */
      XtExposeCompressMaximal,                  /* compress_exposure  */
      FALSE,                                    /* compress_enterlv   */
      FALSE,                                    /* visible_interest   */
      Destroy,                                  /* destroy            */
      _XtInherit,                               /* resize             */
      _XtInherit,                               /* expose             */
      (XtSetValuesFunc) SetValues,              /* set_values         */
      NULL,                                     /* set_values_hook    */
      _XtInherit,                               /* set_values_almost  */
      NULL,                                     /* get_values_hook    */
      (XtAcceptFocusProc)_XtInherit,            /* enter_focus        */
      XtVersion,                                /* version            */
      NULL,                                     /* callback_private   */
      XtInheritTranslations,                    /* tm_table           */
      (XtGeometryHandler) _XtInherit,           /* query_geometry     */
      NULL,                                     /* display_accelerator*/
      NULL,                                     /* extension          */
   },

   {                                            /* composite_class fields */
      (XtGeometryHandler) _XtInherit,           /* geometry_manager   */
      _XtInherit,                               /* change_managed     */
      _XtInherit,                               /* insert_child       */
      DeleteChild,                              /* delete_child       */
      NULL,                                     /* extension          */
   },

   {                                            /* constraint_class fields */
      NULL,                                     /* resource list        */   
      0,                                        /* num resources        */   
      0,                                        /* constraint size      */   
      NULL,                                     /* init proc            */   
      NULL,                                     /* destroy proc         */   
      NULL,                                     /* set values proc      */   
      NULL,                                     /* extension            */
   },

   {                                            /* manager_class fields   */
      XtInheritTranslations,                    /* translations           */
      syn_resources,                            /* syn_resources          */
      XtNumber(syn_resources),                  /* num_syn_resources      */
      NULL,                                     /* syn_cont_resources     */
      0,                                        /* num_syn_cont_resources */
      XmInheritParentProcess,                   /* parent_process         */
      NULL,                                     /* extension              */
   },

   {                                            /* bulletinBoard class  */
      TRUE,                                     /*always_install_accelerators*/
      _XmMessageBoxGeoMatrixCreate,             /* geo__matrix_create */
      XtInheritFocusMovedProc,                  /* focus_moved_proc */
      NULL                                      /* extension */
   },   

   {                                            /* messageBox class - none */
      0                                         /* mumble */
   }    
};

externaldef( xmmessageboxwidgetclass) WidgetClass xmMessageBoxWidgetClass
                                        = (WidgetClass) &xmMessageBoxClassRec ;


/************************************************************************
 *  Bitmap Data for Default Symbol
 **********************************<->***********************************/

static char errorBits[] = {
   0x00, 0x00, 0x00, 0xc0, 0x0f, 0x00, 0xf0, 0x3a, 0x00, 0x58, 0x55, 0x00,
   0x2c, 0xa0, 0x00, 0x56, 0x40, 0x01, 0xaa, 0x80, 0x02, 0x46, 0x81, 0x01,
   0x8a, 0x82, 0x02, 0x06, 0x85, 0x01, 0x0a, 0x8a, 0x02, 0x06, 0x94, 0x01,
   0x0a, 0xe8, 0x02, 0x14, 0x50, 0x01, 0x28, 0xb0, 0x00, 0xd0, 0x5f, 0x00,
   0xa0, 0x2a, 0x00, 0x40, 0x15, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static char infoBits[] = {
   0x00, 0x00, 0x78, 0x00, 0x54, 0x00, 0x2c, 0x00, 0x54, 0x00, 0x28, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x2a, 0x00, 0x5c, 0x00, 0x28, 0x00,
   0x58, 0x00, 0x28, 0x00, 0x58, 0x00, 0x28, 0x00, 0x58, 0x00, 0x28, 0x00,
   0x58, 0x00, 0xae, 0x01, 0x56, 0x01, 0xaa, 0x00, 0x00, 0x00, 0x00, 0x00};

static char questionBits[] = {
   0xf0, 0x3f, 0x00, 0x58, 0x55, 0x00, 0xac, 0xaa, 0x00, 0xd6, 0x5f, 0x01,
   0xea, 0xbf, 0x02, 0xf6, 0x7f, 0x01, 0xea, 0xba, 0x02, 0xf6, 0x7d, 0x05,
   0xea, 0xba, 0x0a, 0x56, 0x7d, 0x15, 0xaa, 0xbe, 0x1e, 0x56, 0x5f, 0x01,
   0xac, 0xaf, 0x02, 0x58, 0x57, 0x01, 0xb0, 0xaf, 0x00, 0x60, 0x55, 0x01,
   0xa0, 0xaa, 0x00, 0x60, 0x17, 0x00, 0xa0, 0x2f, 0x00, 0x60, 0x17, 0x00,
   0xb0, 0x2a, 0x00, 0x50, 0x55, 0x00};

static char warningBits[] = {
   0x00, 0x00, 0x18, 0x00, 0x2c, 0x00, 0x56, 0x00, 0x2a, 0x00, 0x56, 0x00,
   0x2a, 0x00, 0x56, 0x00, 0x2c, 0x00, 0x14, 0x00, 0x2c, 0x00, 0x14, 0x00,
   0x2c, 0x00, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x14, 0x00,
   0x2c, 0x00, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00};

static char workingBits[] = {
   0x00, 0x00, 0x00, 0xfe, 0xff, 0x0f, 0xaa, 0xaa, 0x0a, 0x44, 0x55, 0x06,
   0xcc, 0x2a, 0x02, 0x44, 0x55, 0x06, 0xcc, 0x2a, 0x02, 0x84, 0x15, 0x06,
   0x8c, 0x2a, 0x02, 0x04, 0x15, 0x06, 0x0c, 0x0a, 0x02, 0x04, 0x06, 0x06,
   0x0c, 0x0b, 0x02, 0x84, 0x15, 0x06, 0xcc, 0x2a, 0x02, 0x44, 0x55, 0x06,
   0xcc, 0x2a, 0x02, 0x44, 0x55, 0x06, 0xcc, 0x2a, 0x02, 0x44, 0x55, 0x06,
   0xfe, 0xff, 0x0f, 0x56, 0x55, 0x05, 0x00, 0x00, 0x00};



/****************************************************************/
static XImage *
#ifdef _NO_PROTO
CreateDefaultImage( bits, width, height)
            char *          bits ;
            unsigned int    width ;
            unsigned int    height ;
#else
CreateDefaultImage(
            char *          bits,
            unsigned int    width,
            unsigned int    height)
#endif
/****************
 * Create a default images for symbol... used in ClassInitialize.
 ****************/
{
            XImage *        image ;
/****************/

    /* Since XCreateImage uses display-dependent information to fill
    *   client-specific fields of the XImage structure, simply allocate
    *   our own memory, zero it, initialize what we know about, and
    *   hope for the best as far as future releases of Xlib is concerned.
    */
    image = (XImage *) XtCalloc( 1, sizeof( XImage)) ;

    image->width = width ;
    image->height = height ;
    image->data = bits ;
    image->depth = 1 ;
    image->xoffset = 0 ;
    image->format = XYBitmap ;
    image->byte_order = LSBFirst ;
    image->bitmap_unit = 8 ;
    image->bitmap_bit_order = LSBFirst ;
    image->bitmap_pad = 8 ;
    image->bytes_per_line = (width + 7) >> 3 ;

    return( image) ;
    }
/****************************************************************/
static void
ClassInitialize()
/****************
 * 
 ****************/
{
    XImage *image;
/****************/

    /* create and install the default images for the symbol */

    image = CreateDefaultImage (errorBits, 20, 20);
    XmInstallImage (image, "default_xm_error");

    image = CreateDefaultImage (infoBits, 11, 24);
    XmInstallImage (image, "default_xm_information");

    image = CreateDefaultImage (questionBits, 22, 22);
    XmInstallImage (image, "default_xm_question");

    image = CreateDefaultImage (warningBits, 9, 22);
    XmInstallImage (image, "default_xm_warning");

    image = CreateDefaultImage (workingBits, 21, 23);
    XmInstallImage (image, "default_xm_working");

    return ;
}
/****************************************************************/
static void
#ifdef _NO_PROTO
ClassPartInitialize( wc)
            XmMessageBoxWidgetClass wc ;
#else
ClassPartInitialize(
            XmMessageBoxWidgetClass wc)
#endif
/****************
 * 
 ****************/
{
/****************/

    _XmFastSubclassInit (wc, XmMESSAGE_BOX_BIT);

    return ;
}
/****************************************************************/
static void
#ifdef _NO_PROTO
Initialize( request, new)
            XmMessageBoxWidget request ;
            XmMessageBoxWidget new ;
#else
Initialize(
            XmMessageBoxWidget request,
            XmMessageBoxWidget new)
#endif
/****************
 * MessageBox widget specific initialization
 ****************/
{
            Widget          defaultButton ;
/****************/

    /* initialize all widget id's to NULL */
    new->message_box.message_wid = NULL;
    new->message_box.symbol_wid = NULL;
    new->message_box.separator = NULL;
    new->message_box.ok_button = NULL;
    new->bulletin_board.cancel_button = NULL;
    new->message_box.help_button = NULL;
    new->bulletin_board.default_button = NULL;
    new->message_box.internal_pixmap = FALSE ;

    /* check for valid enumerated type values
    */

    switch (new->message_box.dialog_type) {
        case XmDIALOG_ERROR:
        case XmDIALOG_INFORMATION:
        case XmDIALOG_MESSAGE:
        case XmDIALOG_QUESTION:
        case XmDIALOG_WARNING:
        case XmDIALOG_WORKING:
            break;
        default: 
            new->message_box.dialog_type = XmDIALOG_MESSAGE;
            _XmWarning(new, WARNING1); 
    }
    switch (new->message_box.message_alignment) {
        case XmALIGNMENT_CENTER:
        case XmALIGNMENT_BEGINNING:
        case XmALIGNMENT_END:
            break;
        default: 
            new->message_box.message_alignment = XmALIGNMENT_BEGINNING;
            _XmWarning(new, WARNING3);
    }

    /* create all of the associated widgets */
    CreateWidgets( new) ;

    switch(    new->message_box.default_type    )
    {   
        case XmDIALOG_OK_BUTTON:
        {   defaultButton = new->message_box.ok_button ;
            break ;
            } 
        case XmDIALOG_CANCEL_BUTTON:
        {   defaultButton = new->bulletin_board.cancel_button ;
            break ;
            } 
        case XmDIALOG_HELP_BUTTON:
        {   defaultButton = new->message_box.help_button ;
            break ;
            } 
        case XmDIALOG_NONE:
        {   defaultButton = NULL ;
            break ;
            } 
        default: 
        {   new->message_box.default_type = XmDIALOG_OK_BUTTON ;
            defaultButton = new->message_box.ok_button ;
            _XmWarning( new, WARNING2) ;
            break ;
            } 
        }
    BB_DefaultButton( new) = defaultButton ;

    /* set up the buttons as a tab group */
    _XmChangeNavigationType((Widget) new, (XtIsShell( XtParent( request))
                                         ? XmSTICKY_TAB_GROUP : XmTAB_GROUP)) ;
    return ;
    }
/****************************************************************/
static void
#ifdef _NO_PROTO
Destroy (d)
            XmMessageBoxWidget d;
#else
Destroy(
            XmMessageBoxWidget d)
#endif
/****************
 * Destroy the widget specific data structs
 ****************/
{
/****************/

    if(    (d->message_box.symbol_pixmap != XmUNSPECIFIED_PIXMAP)
        && d->message_box.internal_pixmap    )
    {   
        XmDestroyPixmap( d->core.screen, d->message_box.symbol_pixmap) ;
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
 * Set the widget id to NULL for the child widget being destroyed.
 ****************/
{   
            XmMessageBoxWidget mbox ;
/****************/

    if(    XtIsRectObj( child)    )
    {   
        mbox = (XmMessageBoxWidget) XtParent( child) ;

        /* Check for which child is getting destroyed and set to NULL.
        */
        if(    child == mbox->message_box.message_wid    )
        {   mbox->message_box.message_wid = NULL ;
            } 
        else
        {   if(    child == mbox->message_box.symbol_wid    )
            {   mbox->message_box.symbol_wid = NULL ;
                } 
            else
            {   if(    child == mbox->message_box.ok_button    )
                {   mbox->message_box.ok_button = NULL ;
                    } 
                else
                {   if(    child == mbox->message_box.help_button    )
                    {   mbox->message_box.help_button = NULL ;
                        } 
                    else
                    {   if(    child == mbox->message_box.separator    )
                        {   mbox->message_box.separator = NULL ;
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
MessageCallback( w, tag, callback)
            Widget          w;                          /* widget that fired */
            XmMessageBoxWidget tag;
            XmAnyCallbackStruct * callback;
#else
MessageCallback(
            Widget          w,                          /* widget that fired */
            XmMessageBoxWidget tag,
            XmAnyCallbackStruct * callback)
#endif
/****************
 * 
 ****************/
{
    XmAnyCallbackStruct temp;
/****************/

    temp.reason  = callback->reason;
    temp.event   = callback->event;

    if (tag->message_box.ok_button == w) {
        temp.reason = XmCR_OK;
        XtCallCallbackList ((Widget) tag, tag->message_box.ok_callback, &temp);
    }
    else if (tag->bulletin_board.cancel_button == w) {
        temp.reason = XmCR_CANCEL;
        XtCallCallbackList ((Widget) tag, tag->message_box.cancel_callback, &temp);
    }
    else if (tag->message_box.help_button == w) {
      /* Invoke the help system. */
      _XmManagerHelp((XmManagerWidget)tag, callback->event);
    }
    return ;
}

/****************************************************************/
static Boolean
#ifdef _NO_PROTO
SetValues( current, request, new)
            XmMessageBoxWidget current ; /* r */
            XmMessageBoxWidget request ;
            XmMessageBoxWidget new ;
#else
SetValues(
            XmMessageBoxWidget current,
            XmMessageBoxWidget request,
            XmMessageBoxWidget new)
#endif
/****************           ARGSUSED
 * Set attributes of a message widget
 ****************/
{
            Arg             al[ARG_LIST_CNT] ;  /* arg list       */
            Cardinal        ac ;                /* arg list count */
            Boolean         need_layout = FALSE ;
            Boolean         newPixmap = FALSE ;
            Widget          defaultButton ;
/****************/

    /* "in_set_values" means the GeometryManager won't try to resize
    *    and/or re-layout subwidgets.
    */
    BB_InSetValues( new) = True;

    /* Over-ride BBoard to disallow a direct change of button id's
    */
    if (current->bulletin_board.cancel_button
                                        != new->bulletin_board.cancel_button) {
        new->bulletin_board.cancel_button 
                                       = current->bulletin_board.cancel_button;
        _XmWarning( new, WARNING5);
    }

    /* check for valid enumerated type values */

    switch (new->message_box.dialog_type) {
    case XmDIALOG_ERROR:
    case XmDIALOG_INFORMATION:
    case XmDIALOG_MESSAGE:
    case XmDIALOG_QUESTION:
    case XmDIALOG_WARNING:
    case XmDIALOG_WORKING:
        break;
    default: 
        new->message_box.dialog_type = current->message_box.dialog_type;
        _XmWarning( new, WARNING1);
    }
    switch (new->message_box.default_type) {
    case XmDIALOG_OK_BUTTON:
    case XmDIALOG_CANCEL_BUTTON:
    case XmDIALOG_HELP_BUTTON:
    case XmDIALOG_NONE:
        break;
    default: 
        new->message_box.default_type = current->message_box.default_type;
        _XmWarning( new, WARNING2);
    }
    switch (new->message_box.message_alignment) {
        case XmALIGNMENT_CENTER:
        case XmALIGNMENT_BEGINNING:
        case XmALIGNMENT_END:
            break;
        default: 
            new->message_box.message_alignment = current->message_box.message_alignment;
            _XmWarning( new, WARNING3);
    }

    if(    new->message_box.symbol_pixmap
                                     != current->message_box.symbol_pixmap    )
    {   newPixmap = TRUE ;
        new->message_box.internal_pixmap = FALSE ;

        if(    (current->message_box.symbol_pixmap != XmUNSPECIFIED_PIXMAP)
            && current->message_box.internal_pixmap    )
        {   
            XmDestroyPixmap( current->core.screen,
                                          current->message_box.symbol_pixmap) ;
            } 
        } 
    else
    {   /* If symbol pixmap is unchanged and a new dialog type is specified,
        *   then set to default pixmap.
        */
        if(    new->message_box.dialog_type
                                       != current->message_box.dialog_type    )
        {   newPixmap = TRUE ;

            GetMsgBoxPixmap( new) ;

            if(    (current->message_box.symbol_pixmap != XmUNSPECIFIED_PIXMAP)
                && current->message_box.internal_pixmap    )
            {   
                XmDestroyPixmap( current->core.screen, 
                                          current->message_box.symbol_pixmap) ;
                } 
            }
        } 
    if(    newPixmap    )
    {   
        need_layout = TRUE ;

        if(    new->message_box.symbol_wid    ) 
        {   
            ac = 0 ;
            XtSetArg( al[ac], XmNlabelPixmap, 
                                       new->message_box.symbol_pixmap) ; ++ac ;
            XtSetValues( new->message_box.symbol_wid, al, ac) ;
            }
        } 
    /* Check the buttons and labels
    */
    ac = 0 ;
    if(    new->message_box.message_string    )
    {   
        XtSetArg( al[ac], XmNlabelString, 
                                      new->message_box.message_string) ; ++ac ;
        XtSetArg( al[ac], XmNstringDirection,
                                             BB_StringDirection( new)) ; ++ac ;
        new->message_box.message_string = NULL ;
        need_layout = TRUE ;
        }
    if(    current->message_box.message_alignment
                                     != new->message_box.message_alignment    )
    {   XtSetArg( al[ac], XmNalignment, 
                                   new->message_box.message_alignment) ; ++ac ;
        need_layout = TRUE ;
        }
    if(    ac  &&  new->message_box.message_wid    )
    {   XtSetValues( new->message_box.message_wid, al, ac) ;
        } 

    if(    new->message_box.ok_label_string    )
    {   
        if(    new->message_box.ok_button    )
        {   ac = 0 ;
            XtSetArg( al[ac], XmNlabelString, 
                                     new->message_box.ok_label_string) ; ++ac ;
            XtSetArg( al[ac], XmNstringDirection,
                                             BB_StringDirection( new)) ; ++ac ;
            XtSetValues( new->message_box.ok_button, al, ac) ;
            } 
        new->message_box.ok_label_string = NULL ;
        need_layout = TRUE ;
        } 
    if(    new->message_box.cancel_label_string    )
    {   
        if(    new->bulletin_board.cancel_button    )
        {   ac = 0 ;
            XtSetArg( al[ac], XmNlabelString, 
                                 new->message_box.cancel_label_string) ; ++ac ;
            XtSetArg( al[ac], XmNstringDirection,
                                             BB_StringDirection( new)) ; ++ac ;
            XtSetValues( new->bulletin_board.cancel_button, al, ac) ;
            } 
        new->message_box.cancel_label_string = NULL ;
        need_layout = TRUE ;
        } 
    if(    new->message_box.help_label_string    )
    {   
        if(    new->message_box.help_button    )
        {   ac = 0 ;
            XtSetArg( al[ac], XmNlabelString, 
                                   new->message_box.help_label_string) ; ++ac ;
            XtSetArg( al[ac], XmNstringDirection,
                                             BB_StringDirection( new)) ; ++ac ;
            XtSetValues( new->message_box.help_button, al, ac) ;
            } 
        new->message_box.help_label_string = NULL ;
        need_layout = TRUE ;
        } 

    /* If Default Pushbutton changes, reset showAsDefault.
    */
    if (current->message_box.default_type != new->message_box.default_type)
    {
        switch(    new->message_box.default_type    )
        {
            case XmDIALOG_OK_BUTTON:
            {   defaultButton = new->message_box.ok_button ;
                break;
                }
            case XmDIALOG_CANCEL_BUTTON:
            {   defaultButton = new->bulletin_board.cancel_button ;
                break ;
                } 
            case XmDIALOG_HELP_BUTTON:
            {   defaultButton = new->message_box.help_button ;
                break ;
                } 
            default:
            {   defaultButton = NULL ;
                break;
                }
            }
        BB_DefaultButton( new) = defaultButton ;

        if(    BB_DescendantFocus( new)    )
        {   
            if(    defaultButton
                && (    XmIsPushButtonGadget( BB_DescendantFocus( new))
                     || XmIsPushButton( BB_DescendantFocus( new)))    )
            {   
                defaultButton = BB_DescendantFocus( new) ;
                }
            _XmBulletinBoardSetDynDefaultButton( (Widget) new, defaultButton) ;
            } 
        }
    BB_InSetValues( new) = False;

    /* Re-layout the sub-widgets if necessary
    */
    if(    need_layout
        && (XtClass( new) == xmMessageBoxWidgetClass)    )
    {
        _XmBulletinBoardSizeUpdate((XmBulletinBoardWidget) new) ;
        }
    return( FALSE) ;
    }

/****************************************************************/
static void
#ifdef _NO_PROTO
SetUpSymbol (w)
            XmMessageBoxWidget w ;
#else
SetUpSymbol(
            XmMessageBoxWidget w)
#endif
/****************
 * Set up the icon (pixmap label widget) and the label widget itself.
 ****************/
{
    Arg al[ARG_LIST_CNT];
    int ac;
/****************/

    /* If no pixmap specified, try to get from bitmap file or default
    */
    if(    w->message_box.symbol_pixmap == XmUNSPECIFIED_PIXMAP    )
    {   GetMsgBoxPixmap( w) ;
        }

    if(    !w->message_box.symbol_pixmap    )
    {   w->message_box.symbol_pixmap = XmUNSPECIFIED_PIXMAP ;
        } 

    /* Create symbol label even if no pixmap specified; allows SetValues
    *   on dialogType to make pixmap appear in what starts as message box.
    */
    ac = 0;
    XtSetArg (al[ac], XmNwidth, 0);                                    ac++;
    XtSetArg (al[ac], XmNheight, 0);                                   ac++;
    XtSetArg (al[ac], XmNlabelType, XmPIXMAP);                         ac++; 
    XtSetArg (al[ac], XmNlabelPixmap, w->message_box.symbol_pixmap);   ac++;
    XtSetArg (al[ac], XmNtraversalOn, False);                          ac++;

    w->message_box.symbol_wid = XmCreateLabelGadget ((Widget)w, "symbol", al, ac);
    return ;
    }

/****************************************************************/
static void
#ifdef _NO_PROTO
SetUpMessage( w)
            XmMessageBoxWidget w;
#else
SetUpMessage(
            XmMessageBoxWidget w)
#endif
/****************
 * Set up the message label (assumes none set yet).
 ****************/
{
    Arg al[ARG_LIST_CNT];
    int ac;
/****************/

    /* set up y value dependent on symbol_wid */

    ac = 0;
    XtSetArg (al[ac], XmNalignment, w->message_box.message_alignment);  ac++;
    XtSetArg (al[ac], XmNborderWidth, 0);                               ac++;
    XtSetArg (al[ac], XmNtraversalOn, False);                           ac++;
    if (w->message_box.message_string) {
       XtSetArg(al[ac], XmNlabelString, w->message_box.message_string); ac++;
       w->message_box.message_string = NULL;
    }
    XtSetArg (al[ac], XmNstringDirection, BB_StringDirection( w)) ;
    ac++;

    w->message_box.message_wid = XmCreateLabelGadget((Widget)w, "", al, ac);
    return ;
}

/****************************************************************/
XmGeoMatrix
#ifdef _NO_PROTO
_XmMessageBoxGeoMatrixCreate( mb, instigator, desired)
            XmMessageBoxWidget mb ;
            Widget          instigator ;
            XtWidgetGeometry *desired ;
#else
_XmMessageBoxGeoMatrixCreate(
            XmMessageBoxWidget mb,
            Widget          instigator,
            XtWidgetGeometry *desired)
#endif
/****************
 * 
 ****************/
{
            XmGeoMatrix     geoSpec ;
    register XmGeoRowLayout  layoutPtr ;
    register XmKidGeometry   boxPtr ;
            XmKidGeometry   firstBoxInRow ;
/****************/

    geoSpec = _XmGeoMatrixAlloc( MB_MAX_WIDGETS_VERT, MB_MAX_NUM_WIDGETS, 0) ;
    geoSpec->composite = (Widget) mb ;
    geoSpec->instigator = (Widget) instigator ;
    if(    desired    )
    {   geoSpec->instig_request = *desired ;
        }
    geoSpec->margin_w = BB_MarginWidth( mb) + mb->manager.shadow_thickness ;
    geoSpec->margin_h = BB_MarginHeight( mb) + mb->manager.shadow_thickness ;
    geoSpec->no_geo_request = _XmMessageBoxNoGeoRequest ;

    layoutPtr = geoSpec->layouts ;
    boxPtr = geoSpec->boxes ;

    firstBoxInRow = boxPtr ;
    if(    (mb->message_box.symbol_pixmap != XmUNSPECIFIED_PIXMAP)
        && _XmGeoSetupKid( boxPtr, mb->message_box.symbol_wid)    )
    {   ++boxPtr ;
        } 
    if(    _XmGeoSetupKid( boxPtr, mb->message_box.message_wid)    )
    {   ++boxPtr ;
        } 
    if(    boxPtr != firstBoxInRow    )
    {   layoutPtr->space_above = BB_MarginHeight( mb) ;
        layoutPtr->space_between = BB_MarginWidth( mb) ;
        ++boxPtr ;      /* Skip over NULL box which marks end of row.*/
        ++layoutPtr ;   /* Increment row layout pointer.*/
        } 
    if(    _XmGeoSetupKid( boxPtr, mb->message_box.separator)    )
    {   layoutPtr->fix_up = _XmSeparatorFix ;
        layoutPtr->space_above = BB_MarginHeight( mb) ;
        boxPtr += 2 ;       /* For new row, add 2. */
        ++layoutPtr ;       /* For new row. */
        } 
    firstBoxInRow = boxPtr ;
    if(    _XmGeoSetupKid( boxPtr, mb->message_box.ok_button)    )
    {   ++boxPtr ;
        } 
    if(    _XmGeoSetupKid( boxPtr, mb->bulletin_board.cancel_button)    )
    {   ++boxPtr ;
        } 
    if(    _XmGeoSetupKid( boxPtr, mb->message_box.help_button)    )
    {   ++boxPtr ;
        } 
    if(    boxPtr != firstBoxInRow    )
    {   /* Had at least one button.
        */
        layoutPtr->fill_mode = XmGEO_CENTER ;
        layoutPtr->fit_mode = XmGEO_WRAP ;
        if(    !mb->message_box.minimize_buttons    )
        {   layoutPtr->even_width = 1 ;
            } 
        layoutPtr->even_height = 1 ;
        layoutPtr->space_above = BB_MarginHeight( mb) ;
        ++boxPtr ;
	++layoutPtr ;
        } 
    layoutPtr->space_above = BB_MarginHeight( mb) ;
    layoutPtr->end = TRUE ;        /* Mark the last row. */
    return( geoSpec) ;
    }
/****************************************************************/
Boolean
#ifdef _NO_PROTO
_XmMessageBoxNoGeoRequest( geoSpec)
            XmGeoMatrix     geoSpec ;
#else
_XmMessageBoxNoGeoRequest(
            XmGeoMatrix     geoSpec)
#endif
/****************
 * 
 ****************/
{
/****************/

    if(    BB_InSetValues( geoSpec->composite)
        && (XtClass( geoSpec->composite) == xmMessageBoxWidgetClass)    )
    {   
        return( TRUE) ;
        } 
    return( FALSE) ;
    }

/****************************************************************/
static Widget
#ifdef _NO_PROTO
CreateShell (parent, name, args, ac)
            Widget parent;
            char *name;
            ArgList args ;
            Cardinal ac ;
#else
CreateShell(
            Widget parent,
            char *name,
            ArgList args,
            Cardinal ac)
#endif
/****************
 * 
 ****************/
{
    ArgList argsNew ;
    Widget retWid;
    char *ds_name;
/****************/

    ds_name = XtCalloc( 1, (strlen( name) + DIALOG_SUFFIX_SIZE + 1)) ;
    strcpy(ds_name,name);
    strcat(ds_name,DIALOG_SUFFIX);

    argsNew = (ArgList) XtMalloc( sizeof( Arg) * (ac + 1)) ;
    bcopy( args, argsNew, (sizeof( Arg) * ac)) ;
    XtSetArg (argsNew[ac], XmNallowShellResize, TRUE); ++ac ;
    retWid = XmCreateDialogShell (parent, ds_name, argsNew, ac);

    XtFree( argsNew) ;
    XtFree(ds_name);

    return (retWid);
}
/****************************************************************/
static void
#ifdef _NO_PROTO
CreateWidgets( w)
            XmMessageBoxWidget w ;
#else
CreateWidgets(
            XmMessageBoxWidget w)
#endif
/****************
 * Construct the required captive widgets for the box.  Don't worry about
 *   positioning since a layout will happen later on.
 ****************/
{
    Arg  al[2];
/****************/

    /* create the symbol label */
    SetUpSymbol (w);

    /* create the message label, after symbol is created */
    SetUpMessage (w);

    /* create the separator */
    XtSetArg (al[0], XmNhighlightThickness, 0);
    w->message_box.separator = XmCreateSeparatorGadget((Widget)w, "separator", al, 1);


    /* create all pushbuttons, user can unmanage if they don't want them */

    /* "Ok" button... if no label, use default "OK" name for label */
    w->message_box.ok_button = _XmBB_CreateButtonG( (Widget)w,
                                        w->message_box.ok_label_string, "OK") ;
    w->message_box.ok_label_string = NULL;
    XtAddCallback( w->message_box.ok_button, XmNactivateCallback, 
                                              MessageCallback, (XtPointer) w) ;
    /* "Cancel" button... if no label, use default "Cancel" name for label */
    w->bulletin_board.cancel_button = _XmBB_CreateButtonG((Widget) w,
                                w->message_box.cancel_label_string, "Cancel") ;
    w->message_box.cancel_label_string = NULL;
    XtAddCallback( w->bulletin_board.cancel_button, XmNactivateCallback, 
                                              MessageCallback, (XtPointer) w) ;
    /* "Help" button... if no label, use default "Help" name for label */
    w->message_box.help_button = _XmBB_CreateButtonG((Widget) w,
                                    w->message_box.help_label_string, "Help") ;
    w->message_box.help_label_string = NULL;

    /* Remove BulletinBoard Unmanage callback from help button.
    */
    XtRemoveAllCallbacks( w->message_box.help_button, XmNactivateCallback) ;
    XtAddCallback( w->message_box.help_button, XmNactivateCallback, 
                                              MessageCallback, (XtPointer) w) ;
    /* Now manage all my children.
    */
    XtManageChildren (w->composite.children, w->composite.num_children);        
    return ;
}

/****************************************************************/
static Widget
#ifdef _NO_PROTO
_XmCreateDialog (parent, name, al, ac, type)
            Widget        parent;         /* parent widget                */
            char         *name;           /* dialog widget name           */
            ArgList       al;             /* argument list                */
            int           ac;             /* argument count               */
            unsigned char type;           /* type of dialog being created */
#else
_XmCreateDialog(
            Widget        parent,         /* parent widget                */
            char         *name,           /* dialog widget name           */
            ArgList       al,             /* argument list                */
            int           ac,             /* argument count               */
            unsigned char type)           /* type of dialog being created */
#endif
/****************
 * Common create routine for message dialogs...
 *   it will create the shell and widgets, and set the dialog_type to
 *   whatever has been passed in...
 ****************/
{
    Widget shell;
    Widget w;
    ArgList  argsNew;
/****************/

    shell = CreateShell (parent, name, al, ac); 

    /* add dialogType to arglist and force to type passed in... */

    /*  allocate arglist, copy args, add dialog type arg */
    argsNew = (ArgList) XtMalloc (sizeof(Arg) * (ac + 1));
    bcopy (al, argsNew, sizeof(Arg) * ac);
    XtSetArg (argsNew[ac], XmNdialogType, type);  ac++;

    /*  create MessageBox, add shell destroy callback, free argsNew, return */
    w = XtCreateWidget (name, xmMessageBoxWidgetClass, shell, argsNew, ac);
    XtAddCallback (w, XmNdestroyCallback, _XmDestroyParentCallback, NULL);
    XtFree (argsNew);

    return (w);
}

/****************************************************************/
Widget
#ifdef _NO_PROTO
XmCreateMessageBox( parent, name, al, ac)
            Widget      parent;                 /* parent widget */
            char       *name;                   /* dialog widget name */
            ArgList     al;                     /* argument list */
            Cardinal    ac;                     /* argument count */
#else
XmCreateMessageBox(
            Widget      parent,                 /* parent widget */
            char       *name,                   /* dialog widget name */
            ArgList     al,                     /* argument list */
            Cardinal    ac)                     /* argument count */
#endif
/****************
 * 
 ****************/
{
    Widget w;
/****************/

    w = XtCreateWidget (name, xmMessageBoxWidgetClass, parent, al, ac);

    return w;
}
/****************************************************************/
Widget
#ifdef _NO_PROTO
XmCreateMessageDialog (parent, name, al, ac)
            Widget      parent;                 /* parent widget */
            char       *name;                   /* dialog widget name */
            ArgList     al;                     /* argument list */
            Cardinal    ac;                     /* argument count */
#else
XmCreateMessageDialog(
            Widget      parent,                 /* parent widget */
            char       *name,                   /* dialog widget name */
            ArgList     al,                     /* argument list */
            Cardinal    ac)                     /* argument count */
#endif
/****************
 * 
 ****************/
{
    Widget w;
/****************/

    w = _XmCreateDialog (parent, name, al, ac, XmDIALOG_MESSAGE);
    return w;
}
/****************************************************************/
Widget
#ifdef _NO_PROTO
XmCreateErrorDialog (parent, name, al, ac)
            Widget      parent;                 /* parent widget */
            char       *name;                   /* dialog widget name */
            ArgList     al;                     /* argument list */
            Cardinal    ac;                     /* argument count */
#else
XmCreateErrorDialog(
            Widget      parent,                 /* parent widget */
            char       *name,                   /* dialog widget name */
            ArgList     al,                     /* argument list */
            Cardinal    ac)                     /* argument count */
#endif
/****************
 * 
 ****************/
{
    Widget w;
/****************/

    w = _XmCreateDialog (parent, name, al, ac, XmDIALOG_ERROR);
    return w;
}
/****************************************************************/
Widget
#ifdef _NO_PROTO
XmCreateInformationDialog (parent, name, al, ac)
            Widget      parent;                 /* parent widget */
            char       *name;                   /* dialog widget name */
            ArgList     al;                     /* argument list */
            Cardinal    ac;                     /* argument count */
#else
XmCreateInformationDialog(
            Widget      parent,                 /* parent widget */
            char       *name,                   /* dialog widget name */
            ArgList     al,                     /* argument list */
            Cardinal    ac)                     /* argument count */
#endif
/****************
 * 
 ****************/
{
    Widget w;
/****************/

    w = _XmCreateDialog (parent, name, al, ac, XmDIALOG_INFORMATION);
    return w;
}
/****************************************************************/
Widget
#ifdef _NO_PROTO
XmCreateQuestionDialog (parent, name, al, ac)
            Widget      parent;                 /* parent widget */
            char       *name;                   /* dialog widget name */
            ArgList     al;                     /* argument list */
            Cardinal    ac;                     /* argument count */
#else
XmCreateQuestionDialog(
            Widget      parent,                 /* parent widget */
            char       *name,                   /* dialog widget name */
            ArgList     al,                     /* argument list */
            Cardinal    ac)                     /* argument count */
#endif
/****************
 * 
 ****************/
{
    Widget w;
/****************/

    w = _XmCreateDialog (parent, name, al, ac, XmDIALOG_QUESTION);
    return w;
}
/****************************************************************/
Widget
#ifdef _NO_PROTO
XmCreateWarningDialog (parent, name, al, ac)
            Widget      parent;                 /* parent widget */
            char       *name;                   /* dialog widget name */
            ArgList     al;                     /* argument list */
            Cardinal    ac;                     /* argument count */
#else
XmCreateWarningDialog(
            Widget      parent,                 /* parent widget */
            char       *name,                   /* dialog widget name */
            ArgList     al,                     /* argument list */
            Cardinal    ac)                     /* argument count */
#endif
/****************
 * 
 ****************/
{
    Widget w;
/****************/

    w = _XmCreateDialog (parent, name, al, ac, XmDIALOG_WARNING);
    return w;
}
/****************************************************************/
Widget
#ifdef _NO_PROTO
XmCreateWorkingDialog (parent, name, al, ac)
            Widget      parent;                 /* parent widget */
            char       *name;                   /* dialog widget name */
            ArgList     al;                     /* argument list */
            Cardinal    ac;                     /* argument count */
#else
XmCreateWorkingDialog(
            Widget      parent,                 /* parent widget */
            char       *name,                   /* dialog widget name */
            ArgList     al,                     /* argument list */
            Cardinal    ac)                     /* argument count */
#endif
/****************
 * 
 ****************/
{
    Widget w;
/****************/

    w = _XmCreateDialog (parent, name, al, ac, XmDIALOG_WORKING);
    return w;
}

/****************************************************************/
Widget
#ifdef _NO_PROTO
XmMessageBoxGetChild (widget, child)
            Widget          widget;
            unsigned char   child;
#else
XmMessageBoxGetChild(
            Widget          widget,
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
    XmMessageBoxWidget  w = (XmMessageBoxWidget)widget;
/****************/

    switch (child) {
        case XmDIALOG_DEFAULT_BUTTON: return (w->bulletin_board.default_button);
        case XmDIALOG_SYMBOL_LABEL:   return (w->message_box.symbol_wid);
        case XmDIALOG_MESSAGE_LABEL:  return (w->message_box.message_wid);
        case XmDIALOG_OK_BUTTON:      return (w->message_box.ok_button);
        case XmDIALOG_CANCEL_BUTTON:  return (w->bulletin_board.cancel_button);
        case XmDIALOG_HELP_BUTTON:    return (w->message_box.help_button);
        case XmDIALOG_SEPARATOR:      return (w->message_box.separator);
        default: _XmWarning (w, WARNING4); break ;
    }
    return NULL ;
}
/****************************************************************/
static void
#ifdef _NO_PROTO
GetMessageString (w, resource, value)
            XmMessageBoxWidget w ;
            XrmQuark        resource ;
            XtArgVal *      value ;
#else
GetMessageString(
            XmMessageBoxWidget w,
            XrmQuark        resource,
            XtArgVal *      value)
#endif
/****************           ARGSUSED
 * 
 ****************/
{
    XmString  data;
    Arg       al[1];
/****************/

    if (w->message_box.message_wid) {
        XtSetArg (al[0], XmNlabelString, &data);
        XtGetValues (w->message_box.message_wid, al, 1);
        *value = (XtArgVal) data;
    }
    else *value = NULL;
    return ;
}
/****************************************************************/
static void
#ifdef _NO_PROTO
GetSymbolPixmap(w, resource, value)
            XmMessageBoxWidget w ;
            XrmQuark        resource ;
            XtArgVal *      value ;
#else
GetSymbolPixmap(
            XmMessageBoxWidget w,
            XrmQuark        resource,
            XtArgVal *      value)
#endif
/****************           ARGSUSED
 * 
 ****************/
{
    Pixmap  data;
    Arg     al[1];
/****************/

    if (w->message_box.symbol_wid) {
        XtSetArg (al[0], XmNlabelPixmap, &data);
        XtGetValues (w->message_box.symbol_wid, al, 1);
        *value = (XtArgVal) data;
    }
    else *value = NULL;
    return ;
}
/****************************************************************/
static void
#ifdef _NO_PROTO
GetOkLabelString(w, resource, value)
            XmMessageBoxWidget w ;
            XrmQuark        resource ;
            XtArgVal *      value ;
#else
GetOkLabelString(
            XmMessageBoxWidget w,
            XrmQuark        resource,
            XtArgVal *      value)
#endif
/****************           ARGSUSED
 * 
 ****************/
{
    XmString  data;
    Arg       al[1];
/****************/

    if (w->message_box.ok_button) {
        XtSetArg (al[0], XmNlabelString, &data);
        XtGetValues (w->message_box.ok_button, al, 1);
        *value = (XtArgVal) data;
    }
    else *value = NULL;
    return ;
}
/****************************************************************/
static void
#ifdef _NO_PROTO
GetCancelLabelString(w, resource, value)
            XmMessageBoxWidget w ;
            XrmQuark        resource ;
            XtArgVal *      value ;
#else
GetCancelLabelString(
            XmMessageBoxWidget w,
            XrmQuark        resource,
            XtArgVal *      value)
#endif
/****************           ARGSUSED
 * 
 ****************/
{
    XmString  data;
    Arg       al[1];
/****************/

    if (w->bulletin_board.cancel_button) {
        XtSetArg (al[0], XmNlabelString, &data);
        XtGetValues (w->bulletin_board.cancel_button, al, 1);
        *value = (XtArgVal) data;
    }
    else *value = NULL;
    return ;
}
/****************************************************************/
static void
#ifdef _NO_PROTO
GetHelpLabelString(w, resource, value)
            XmMessageBoxWidget w ;
            XrmQuark        resource ;
            XtArgVal *      value ;
#else
GetHelpLabelString(
            XmMessageBoxWidget w,
            XrmQuark        resource,
            XtArgVal *      value)
#endif
/****************           ARGSUSED
 * 
 ****************/
{
    XmString  data;
    Arg       al[1];
/****************/

    if (w->message_box.help_button) {
        XtSetArg (al[0], XmNlabelString, &data);
        XtGetValues (w->message_box.help_button, al, 1);
        *value = (XtArgVal) data;
    }
    else *value = NULL;
    return ;
}
/****************************************************************/
static void
#ifdef _NO_PROTO
GetMsgBoxPixmap( mBox)
            XmMessageBoxWidget mBox ;
#else
GetMsgBoxPixmap( 
            XmMessageBoxWidget mBox)
#endif
/****************
 * 
 ****************/
{
            Pixmap          tmpPix = XmUNSPECIFIED_PIXMAP ;
            char *          fileName ;
            char *          defaultName ;
/****************/

    /* Try to get pixmap from bitmap file or default.
    */
    switch(    mBox->message_box.dialog_type    ) 
    {   
        case XmDIALOG_ERROR: 
        {   fileName = "xm_error" ; 
            defaultName = "default_xm_error" ;
            break ;
            } 
        case XmDIALOG_INFORMATION:
        {   fileName = "xm_information" ;
            defaultName = "default_xm_information" ;
            break ;
            }
        case XmDIALOG_QUESTION:
        {   fileName = "xm_question" ;
            defaultName = "default_xm_question" ;
            break ;
            }
        case XmDIALOG_WARNING:
        {   fileName = "xm_warning" ;
            defaultName = "default_xm_warning" ;
            break ;
            }
        case XmDIALOG_WORKING:
        {   fileName = "xm_working" ;
            defaultName = "default_xm_working" ;
            break ;
            }
        default: 
        {   fileName = NULL ;
            defaultName = NULL ;
            break ;
            }
        }
    if(    fileName    )
    {
        tmpPix = XmGetPixmap( mBox->core.screen, fileName, 
                       mBox->manager.foreground, mBox->core.background_pixel) ;
        if(    tmpPix == XmUNSPECIFIED_PIXMAP    )
        {   tmpPix = XmGetPixmap( mBox->core.screen, defaultName,
                       mBox->manager.foreground, mBox->core.background_pixel) ;
            } 
        }
    mBox->message_box.symbol_pixmap = tmpPix ;
    mBox->message_box.internal_pixmap = TRUE ;

    return ;
    }
/****************************************************************/
