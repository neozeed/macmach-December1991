#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)Protocols.c	3.15 91/01/10";
#endif /* lint */
#endif /* REV_INFO */
/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
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
* X Window System is a trademark of the Massachusetts Institute of Technology
*
*******************************************************************************
******************************************************************************/
#include <Xm/XmP.h>
#include <Xm/ProtocolsP.h>
#include <X11/Vendor.h>
#include <X11/Xatom.h>

#define XmCR_PROTOCOLS	6666	/* needs to be somewhere else */
#define MAX_PROTOCOLS		32
#define PROTOCOL_BLOCK_SIZE	4

/* forward declarations for internal functions */

static XtEventHandler	RealizeHandler();
static XtEventHandler	ProtocolHandler();
static XmProtocolMgr 	GetProtocolMgr();
static XmProtocol 	GetProtocol();
static XmProtocolMgr	AddProtocolMgr();
static void		RemoveProtocolMgr(); 



/***************************************************************************
 *
 * ProtocolObject Resources
 *
 ***************************************************************************/

#define Offset(field) XtOffset(XmProtocol, protocol.field)

static XtResource protocolResources[] =
{
    {
	XmNextensionType,
	XmCExtensionType, XmRExtensionType, sizeof (unsigned char),
	XtOffset (XmExtObject, ext.extensionType),
	XmRImmediate, (XtPointer)XmPROTOCOL_EXTENSION,
    },
    {
	XmNprotocolCallback,
	XmCProtocolCallback, XmRCallback, sizeof (XtCallbackList),
	Offset (callbacks),
	XmRImmediate, (XtPointer)NULL,
    },
#ifdef notdef
    {
	XmNatom,
	XmCAtom, XmRAtom, sizeof (Atom),
	Offset (atom),
	XmRImmediate, (XtPointer)NULL,
    },
    {
	XmNactive,
	XmCActive, XmRBoolean, sizeof (Boolean),
	Offset (active),
	XmRImmediate, (XtPointer)FALSE,
    },
#endif /* notdef */
};
#undef Offset

/* Foward reference for class routines */

static void		ClassInitialize();
static void		ClassPartInitialize();
static void 		Initialize();
static void 		Realize();
static void		GetValuesHook();
static void		Destroy();
static void 		ChangeManaged();

/* forward reference for utility routines */

XmProtocolClassRec xmProtocolClassRec = {
    {	
	(WidgetClass) &xmExtClassRec,	/* superclass 		*/   
	"protocol",			/* class_name 		*/   
	sizeof(XmProtocolRec),	 	/* size 		*/   
	ClassInitialize, 		/* Class Initializer 	*/   
	ClassPartInitialize, 		/* class_part_init 	*/ 
	FALSE, 				/* Class init'ed ? 	*/   
	Initialize, 			/* initialize         	*/   
	NULL, 				/* initialize_notify    */ 
	NULL,	 			/* realize            	*/   
	NULL,	 			/* actions            	*/   
	0,				/* num_actions        	*/   
	protocolResources,		/* resources          	*/   
	XtNumber(protocolResources),	/* resource_count     	*/   
	NULLQUARK, 			/* xrm_class          	*/   
	FALSE, 				/* compress_motion    	*/   
	FALSE, 				/* compress_exposure  	*/   
	FALSE, 				/* compress_enterleave	*/   
	FALSE, 				/* visible_interest   	*/   
	Destroy,			/* destroy            	*/   
	NULL,		 		/* resize             	*/   
	NULL, 				/* expose             	*/   
	NULL,		 		/* set_values         	*/   
	NULL, 				/* set_values_hook      */ 
	NULL,			 	/* set_values_almost    */ 
	NULL,				/* get_values_hook      */ 
	NULL, 				/* accept_focus       	*/   
	XtVersion, 			/* intrinsics version 	*/   
	NULL, 				/* callback offsets   	*/   
	NULL,				/* tm_table           	*/   
	NULL, 				/* query_geometry       */ 
	NULL, 				/* display_accelerator  */ 
	NULL, 				/* extension            */ 
    },	
    {
	NULL,				/* synthetic resources	*/
	0,				/* num syn resources	*/
    },
    {
	NULL,				/* extension		*/
    },
};

externaldef(xmprotocolobjectclass) WidgetClass 
  xmProtocolObjectClass = (WidgetClass) (&xmProtocolClassRec);

/************************************************************************
 *
 *  ClassInitialize
 *    Initialize the vendorShell class structure.  This is called only
 *    the first time a vendorShell widget is created.  It registers the
 *    resource type converters unique to this class.
 *
 ************************************************************************/

static void ClassInitialize()
{

}


/************************************************************************
 *
 *  ClassPartInitialize
 *    Set up the inheritance mechanism for the routines exported by
 *    vendorShells class part.
 *
 ************************************************************************/

static void ClassPartInitialize (w)
    WidgetClass w;
    
{
    XmProtocolObjectClass wc = (XmProtocolObjectClass) w;
    XmProtocolObjectClass sc =
      (XmProtocolObjectClass) wc->object_class.superclass;
    
    if (wc == (XmProtocolObjectClass)xmProtocolObjectClass)
      return;
}

static void Initialize(req, new, args, num_args)
    Widget	req, new;
    ArgList	args;
    Cardinal	*num_args;
{
    XmProtocol				ne = (XmProtocol) new;
    XmWidgetExtData			extData;

    /*
     * we should free this in ExtObject's destroy proc, but since all
     * gadgets would need to change to not free it in thier code we'll
     * do it here. |||
     */
    extData = _XmGetWidgetExtData(ne->ext.logicalParent,
				  ne->ext.extensionType);
    XtFree(extData->reqWidget);
}

/************************************************************************
 *
 *  Destroy
 *
 ************************************************************************/
static void Destroy(protocolObj)
    XmProtocol	protocolObj;
{
}

static XContext	allProtocolsMgrContext = NULL;

/************************************<+>*************************************
 *
 *   RemoveAllPMgr
 *
 *************************************<+>************************************/
static XtCallbackProc RemoveAllPMgr(w, ap_mgr, call_data)
    Widget	w;
    XmAllProtocolsMgr ap_mgr;
    caddr_t	call_data;
{ 
    Cardinal	i;
    
    for (i = 0; i < ap_mgr->num_protocol_mgrs; i++)
      {
	  RemoveProtocolMgr(ap_mgr, ap_mgr->protocol_mgrs[i]);
      }
    /* free the context manager entry ||| */
    XDeleteContext(XtDisplay(w), 
		   (Window)w, 
		   allProtocolsMgrContext);
    XtFree((char *)ap_mgr);
}

/************************************<+>*************************************
 *
 *   GetAllProtocolsMgr
 *
 *************************************<+>************************************/
static XmAllProtocolsMgr GetAllProtocolsMgr(shell)
    Widget	shell;
{
    XmAllProtocolsMgr	ap_mgr;
    Display		*display;
    
    if (!XmIsVendorShell(shell))
      {
	  XtWarning("must be a vendor shell");
	  return ((XmAllProtocolsMgr)0);
      }
    else
      {
	  display = XtDisplay(shell);
	  
	  if (allProtocolsMgrContext == NULL)
	    allProtocolsMgrContext = XUniqueContext();
	  
	  if (XFindContext(display,
			   (Window) shell,
			   allProtocolsMgrContext,
			   (caddr_t *)&ap_mgr))
	    {
		ap_mgr = XtNew(XmAllProtocolsMgrRec);
		
		ap_mgr->shell = shell;
		ap_mgr->num_protocol_mgrs = 
		  ap_mgr->max_protocol_mgrs = 0;
		ap_mgr->protocol_mgrs = NULL;
		(void) XSaveContext(display, 
				    (Window) shell, 
				    allProtocolsMgrContext, 
				    (caddr_t) ap_mgr);
		
		/* !!! should this be in some init code for vendor shell ? */
		/* if shell isn't realized, add an event handler for everybody */
		
		if (!XtIsRealized(shell))
		  {
		      XtAddEventHandler(shell, StructureNotifyMask, FALSE,
					RealizeHandler, (caddr_t) ap_mgr);
		  }
		XtAddCallback(shell, XmNdestroyCallback, RemoveAllPMgr, 
			      (caddr_t)ap_mgr);
		
	    }
	  return ap_mgr;
      }
}
/************************************<+>*************************************
 *
 *   SetProtocolProperty
 *
 *************************************<+>************************************/
#define SetProtocolProperty(shell, property, prop_type, atoms, num_atoms) \
  XChangeProperty((shell)->core.screen->display, XtWindow(shell), \
		  property, prop_type, 32, PropModeReplace, \
		  (unsigned char*)atoms, (int)num_atoms)


/************************************<+>*************************************
 *
 *   UpdateProtocolMgrProperty
 *
 *************************************<+>************************************/
static void	UpdateProtocolMgrProperty(shell, p_mgr)
    Widget		shell;
    XmProtocolMgr	p_mgr;
{
    Cardinal	i, num_active = 0;
    static Atom	active_protocols[MAX_PROTOCOLS];
    XmProtocolList	protocols = p_mgr->protocols;
    
    for (i = 0; i < p_mgr->num_protocols; i++) {
	if (protocols[i]->protocol.active)
	  active_protocols[num_active++] = protocols[i]->protocol.atom;
    }
    SetProtocolProperty(shell, p_mgr->property, XA_ATOM, active_protocols, num_active);
}


/************************************<+>*************************************
 *
 *   InstallProtocols
 *
 *************************************<+>************************************/
static void InstallProtocols(w, ap_mgr)
    Widget	 	w;
    XmAllProtocolsMgr	ap_mgr;
{
    XmProtocolMgr	p_mgr;
    XmProtocol		protocol;
    Cardinal		i;
    
    XtAddRawEventHandler(w, (EventMask)0, TRUE, 
			 ProtocolHandler, ap_mgr);
    XtRemoveEventHandler(w,StructureNotifyMask , FALSE, 
			 RealizeHandler, ap_mgr);
    
    for (i=0; i < ap_mgr->num_protocol_mgrs; i++)
      UpdateProtocolMgrProperty(w, ap_mgr->protocol_mgrs[i]);
    
}

/************************************<+>*************************************
 *
 *   RealizeHandler
 *
 *************************************<+>************************************/
static XtEventHandler RealizeHandler(w, closure, event)
    Widget 	w;
    caddr_t	closure;
    XEvent	*event;
{
    XmAllProtocolsMgr	ap_mgr = (XmAllProtocolsMgr)closure;
    
    switch (event->type) 
      {
	case MapNotify:
	  InstallProtocols(w, ap_mgr);
	default:
	  break;
      }
}

/************************************<+>*************************************
 *
 *   ProtocolHandler
 *
 *************************************<+>************************************/
static XtEventHandler ProtocolHandler(w, closure, event)
    Widget 	w;
    caddr_t	closure;
    XEvent	*event;
{
    XmAllProtocolsMgr	ap_mgr = (XmAllProtocolsMgr)closure;
    XmProtocolMgr	p_mgr;
    XmProtocol		protocol;
    XmAnyCallbackStruct	call_data_rec;
    XtCallbackProc	func;
    
    call_data_rec.reason = XmCR_PROTOCOLS;
    call_data_rec.event = event;
    
    switch (event->type) 
      {
	case ClientMessage:
	  {
	      XClientMessageEvent	*p_event = (XClientMessageEvent *) event;
	      Atom			p_atom = (Atom) p_event->data.l[0];
	      
	      if (((p_mgr = GetProtocolMgr(ap_mgr, (Atom)p_event->message_type)) 
		  == (XmProtocolMgr)0) ||
		  ((protocol = GetProtocol(p_mgr, p_atom)) == (XmProtocol)0))
		return;
	      else {
		  if ((func = protocol->protocol.pre_hook.callback) != (XtCallbackProc)0)
		    (*func) (w, protocol->protocol.pre_hook.closure, (caddr_t) &call_data_rec);
		  
		  if (protocol->protocol.callbacks)
		    XtCallCallbackList(w,
				       protocol->protocol.callbacks, 
				       (XtPointer) &call_data_rec);
		  
		  if ((func = protocol->protocol.post_hook.callback) != (XtCallbackProc)0)
		    (*func) (w, protocol->protocol.post_hook.closure, (caddr_t) &call_data_rec);
	      }
	      break;
	    default:
	      break;
	  }
      }
}



/************************************<+>*************************************
 *
 *   GetProtocol
 *
 *************************************<+>************************************/
static XmProtocol GetProtocol(p_mgr, p_atom)
    XmProtocolMgr		p_mgr;
    Atom			p_atom;
{
    Cardinal	i;
    XmProtocol	protocol;
    
    for (i = 0; 
	 i < p_mgr->num_protocols && p_mgr->protocols[i]->protocol.atom != p_atom;
	 i++){}
    
    if (i < p_mgr->num_protocols)
      {
	  protocol = p_mgr->protocols[i];
      }
    else 
      {
	  protocol = (XmProtocol)0;
      }
    return(protocol);
}


/************************************<+>*************************************
 *
 *   AddProtocolMgr
 *
 *************************************<+>************************************/
static XmProtocolMgr AddProtocolMgr(ap_mgr, property)
    XmAllProtocolsMgr	ap_mgr;
    Atom		property;
{
    XmProtocolMgr		p_mgr = (XmProtocolMgr)0;
    Cardinal		i;
    
    for (i = 0; 
	 i < ap_mgr->num_protocol_mgrs &&
	 ap_mgr->protocol_mgrs[i]->property != property;
	 i++){}
    
    if (i < ap_mgr->num_protocol_mgrs)
      {
	  XtWarning("protocol mgr already exists");
      }
    
    if (ap_mgr->num_protocol_mgrs + 2 >= ap_mgr->max_protocol_mgrs) 
      {
	  ap_mgr->max_protocol_mgrs += 2;
	  ap_mgr->protocol_mgrs = (XmProtocolMgrList) 
	    XtRealloc((caddr_t) ap_mgr->protocol_mgrs ,
		      ((unsigned) (ap_mgr->max_protocol_mgrs) 
		       * sizeof(XmProtocolMgr)));
      }
    ap_mgr->protocol_mgrs[ap_mgr->num_protocol_mgrs++] 
      = p_mgr = XtNew(XmProtocolMgrRec);
    
    p_mgr->property = property;
    p_mgr->num_protocols =
      p_mgr->max_protocols = 0;
    
    p_mgr->protocols = NULL;
    
    return(p_mgr);
}
/************************************<+>*************************************
 *
 *   GetProtcolMgr
 *
 *************************************<+>************************************/
static XmProtocolMgr GetProtocolMgr(ap_mgr, property)
    XmAllProtocolsMgr ap_mgr;
    Atom	property;
{
    XmProtocolMgr	p_mgr = (XmProtocolMgr)0;
    Cardinal		i;
    
    if (!ap_mgr) return p_mgr;
    
    for (i = 0; 
	 i < ap_mgr->num_protocol_mgrs &&
	 ap_mgr->protocol_mgrs[i]->property != property;
	 i++){}
    
    if (i < ap_mgr->num_protocol_mgrs)
      {
	  p_mgr = ap_mgr->protocol_mgrs[i];
      }
    else
      p_mgr = (XmProtocolMgr)0;

    return p_mgr;
}


/************************************<+>*************************************
 *
 *   RemoveProtocolMgr
 *
 *************************************<+>************************************/

static void RemoveProtocolMgr(ap_mgr, p_mgr)
    XmAllProtocolsMgr	ap_mgr;
    XmProtocolMgr	p_mgr;
{
    Widget	shell = ap_mgr->shell;
    Cardinal 	i;
    
    for (i = 0; i < p_mgr->num_protocols; i++)
      {
	  XtDestroyWidget(p_mgr->protocols[i]);
      }
    if (XtIsRealized(shell))
	XDeleteProperty(XtDisplay(shell), 
			XtWindow(shell), 
			p_mgr->property);
    
    XtFree((char *) p_mgr);
    
    for (i = 0; 
	 i < ap_mgr->num_protocol_mgrs;
	 i++)
      if (ap_mgr->protocol_mgrs[i] = p_mgr)
	break;

    /* ripple mgrs down */
    for ( ; i < ap_mgr->num_protocol_mgrs-1; i++)
      ap_mgr->protocol_mgrs[i] = ap_mgr->protocol_mgrs[i+1];
}
/************************************<+>*************************************
 *
 *  AddProtocols
 *
 *************************************<+>************************************/
static void AddProtocols(shell, p_mgr, protocols, num_protocols)
    Widget		shell;
    XmProtocolMgr	p_mgr;
    Atom		*protocols;
    Cardinal		num_protocols;
{	
    Cardinal		new_num_protocols, i, j;
    XmProtocol		protocol;
    
    new_num_protocols = p_mgr->num_protocols + num_protocols;
    
    if (new_num_protocols >= p_mgr->max_protocols) 
      {
	  /* Allocate more space */
	  Cardinal	add_size;
	  
	  if (num_protocols >= PROTOCOL_BLOCK_SIZE)
	    add_size = num_protocols + PROTOCOL_BLOCK_SIZE;
	  else
	    add_size = PROTOCOL_BLOCK_SIZE;
	  
	  p_mgr->max_protocols +=  add_size;
	  p_mgr->protocols = (XmProtocolList) 
	    XtRealloc((caddr_t) p_mgr->protocols ,
		      (unsigned) (p_mgr->max_protocols) * sizeof(XmProtocol));
      }
    
    for (i = p_mgr->num_protocols, j = 0;
	 i < new_num_protocols; 
	 i++,j++)
      {
	  p_mgr->protocols[i] = protocol = (XmProtocol)
	    XtCreateWidget("protocol", 
			   xmProtocolObjectClass,
			   shell, 
			   NULL, 0);
	  protocol->protocol.atom = protocols[j];
	  protocol->protocol.active = TRUE; /*default */
	  protocol->protocol.callbacks = (XtCallbackList)0;
	  protocol->protocol.pre_hook.callback = 
	    protocol->protocol.post_hook.callback = (XtCallbackProc)0;
	  protocol->protocol.pre_hook.closure = 
	    protocol->protocol.post_hook.closure = (caddr_t)0;
      }
    p_mgr->num_protocols = new_num_protocols;
    
}

/************************************<+>*************************************
 *
 *   RemoveProtocols
 *
 *************************************<+>************************************/
static void RemoveProtocols(shell, p_mgr, protocols, num_protocols)
    Widget		shell;
    XmProtocolMgr	p_mgr;
    Atom		*protocols;
    Cardinal		num_protocols;
{
    static Boolean	match_list[MAX_PROTOCOLS];
    Cardinal		i, j;
    
    if (!p_mgr || !p_mgr->num_protocols || !num_protocols) return;
    
    if (p_mgr->num_protocols > MAX_PROTOCOLS)
      XtWarning("more protocols than I can handle");
    
    for (i = 0; i <= p_mgr->num_protocols; i++)
      match_list[i] = FALSE;
    
    /* setup the match list */
    for (i = 0; i < num_protocols; i++)
      {
	  for (j = 0 ; 
	       ((j < p_mgr->num_protocols) &&
		(p_mgr->protocols[j]->protocol.atom != protocols[i]));
	       j++) 
	    {};
	  if (j < p_mgr->num_protocols)
	    match_list[j] = TRUE;
      }
    
    /* 
     * keep only the protocols that arent in the match list. 
     */
    for (j = 0, i = 0; i < p_mgr->num_protocols; i++)
      {
	  if ( ! match_list[i] ) {
	      p_mgr->protocols[j] = p_mgr->protocols[i];
	      j++;
	  }
	  else 
	    XtDestroyWidget(p_mgr->protocols[i]);
      }

    p_mgr->num_protocols = j;
    
}




/*
 *  
 * PUBLIC INTERFACES
 *
 */


/************************************<+>*************************************
 *
 *   _XmInstallProtocols
 *
 *************************************<+>************************************/
#ifdef _NO_PROTO
void _XmInstallProtocols(w)
    Widget	w;
#else /* _NO_PROTO */
void _XmInstallProtocols (Widget w)
#endif /* _NO_PROTO */
{
    XmAllProtocolsMgr	ap_mgr;

    if (ap_mgr = GetAllProtocolsMgr(w))
      InstallProtocols(w, ap_mgr);
}



/************************************<+>*************************************
 *
 *   XmAddProtocols
 *
 *************************************<+>************************************/
#ifdef _NO_PROTO
void XmAddProtocols(shell, property, protocols, num_protocols)
    Widget	shell;
    Atom	property;
    Atom	*protocols;
    Cardinal	num_protocols;
#else /* _NO_PROTO */
void XmAddProtocols (Widget shell, Atom property, Atom *protocols, Cardinal num_protocols)
#endif /* _NO_PROTO */
{
    XmAllProtocolsMgr	ap_mgr; 
    XmProtocolMgr	p_mgr ;
    XmProtocol		protocol;
    
    if (((ap_mgr = GetAllProtocolsMgr(shell)) == 0) ||	!num_protocols)
      return;
    if ((p_mgr = GetProtocolMgr(ap_mgr, property)) == 0)
      p_mgr = AddProtocolMgr(ap_mgr, property);

    /* get rid of duplicates and then append to end */
    RemoveProtocols(shell, p_mgr, protocols, num_protocols);
    AddProtocols(shell, p_mgr, protocols, num_protocols);
    
    if (XtIsRealized(shell))
      UpdateProtocolMgrProperty(shell, p_mgr);
}



/************************************<+>*************************************
 *
 *   XmRemoveProtocols
 *
 *************************************<+>************************************/
#ifdef _NO_PROTO
void XmRemoveProtocols(shell, property, protocols, num_protocols)
    Widget	shell;
    Atom	property;
    Atom	*protocols;
    Cardinal	num_protocols;
#else /* _NO_PROTO */
void XmRemoveProtocols (Widget shell, Atom property, Atom *protocols, Cardinal num_protocols)
#endif /* _NO_PROTO */
{
    XmAllProtocolsMgr	ap_mgr; 
    XmProtocolMgr	p_mgr ;
    XmProtocol		protocol;
    
    if (((ap_mgr = GetAllProtocolsMgr(shell)) == 0) 		||
	((p_mgr = GetProtocolMgr(ap_mgr, property)) == 0) 	||
	!num_protocols)
      return;

    
    RemoveProtocols(shell, p_mgr, protocols, num_protocols);

    if (XtIsRealized(shell))
      UpdateProtocolMgrProperty(shell, p_mgr);
}

/************************************<+>*************************************
 *
 *   XmAddProtocolCallback
 *
 *************************************<+>************************************/
#ifdef _NO_PROTO
void XmAddProtocolCallback(shell, property, proto_atom, callback, closure)
    Widget	shell;
    Atom	property;
    Atom	proto_atom;
    XtCallbackProc callback;
    caddr_t	closure;
#else /* _NO_PROTO */
void XmAddProtocolCallback (Widget shell, Atom property, Atom proto_atom, XtCallbackProc callback, caddr_t closure)
#endif /* _NO_PROTO */
{
    XmAllProtocolsMgr	ap_mgr; 
    XmProtocolMgr	p_mgr ;
    XmProtocol		protocol;
    
    if ((ap_mgr = GetAllProtocolsMgr(shell)) == (XmAllProtocolsMgr)0)
      return;	
    if ((p_mgr = GetProtocolMgr(ap_mgr, property)) == (XmProtocolMgr)0)
      p_mgr = AddProtocolMgr(ap_mgr, property);
    if ((protocol = GetProtocol(p_mgr, proto_atom)) == (XmProtocol)0)
      {
	  XmAddProtocols(shell, property, &proto_atom, 1);
	  protocol = GetProtocol(p_mgr, proto_atom);
      }
    XtAddCallback(protocol, XmNprotocolCallback, callback, closure);
}

/************************************<+>*************************************
 *
 *   XmRemoveProtocolCallback
 *
 *************************************<+>************************************/
#ifdef _NO_PROTO
void XmRemoveProtocolCallback(shell, property, proto_atom, callback, closure)
    Widget	shell;
    Atom	property;
    Atom	proto_atom;
    XtCallbackProc callback;
    caddr_t	closure;
#else /* _NO_PROTO */
void XmRemoveProtocolCallback (Widget shell, Atom property, Atom proto_atom, XtCallbackProc callback, caddr_t closure)
#endif /* _NO_PROTO */
{
    XmAllProtocolsMgr	ap_mgr; 
    XmProtocolMgr	p_mgr ;
    XmProtocol		protocol;
    
    if (((ap_mgr = GetAllProtocolsMgr(shell)) == 0) 		||
	((p_mgr = GetProtocolMgr(ap_mgr, property)) == 0) 	||
	((protocol = GetProtocol(p_mgr, proto_atom)) == 0))
      return;

    XtRemoveCallback(protocol, XmNprotocolCallback, callback, closure);
}

/************************************<+>*************************************
 *
 *   XmActivateProtocol
 *
 *************************************<+>************************************/
#ifdef _NO_PROTO
void  XmActivateProtocol(shell, property, proto_atom)
    Widget	shell;
    Atom	property;
    Atom	proto_atom;
#else /* _NO_PROTO */
void XmActivateProtocol (Widget shell, Atom property, Atom proto_atom)
#endif /* _NO_PROTO */
{
    XmAllProtocolsMgr	ap_mgr; 
    XmProtocolMgr	p_mgr ;
    XmProtocol		protocol;
    
    if (((ap_mgr = GetAllProtocolsMgr(shell)) == 0) 		||
	((p_mgr = GetProtocolMgr(ap_mgr, property)) == 0) 	||
	((protocol = GetProtocol(p_mgr, proto_atom)) == 0) 	||
	protocol->protocol.active)
      return;
    else
      {
	  protocol->protocol.active = TRUE;
	  if (XtIsRealized(shell))
	    UpdateProtocolMgrProperty(shell, p_mgr);
      }
}

/************************************<+>*************************************
 *
 *   XmDeactivateProtocol
 *
 *************************************<+>************************************/
#ifdef _NO_PROTO
void  XmDeactivateProtocol(shell, property, proto_atom)
    Widget	shell;
    Atom	property;
    Atom	proto_atom;
#else /* _NO_PROTO */
void XmDeactivateProtocol (Widget shell, Atom property, Atom proto_atom)
#endif /* _NO_PROTO */
{
    XmAllProtocolsMgr	ap_mgr; 
    XmProtocolMgr	p_mgr ;
    XmProtocol		protocol;
    
    if (((ap_mgr = GetAllProtocolsMgr(shell)) == 0) 		||
	((p_mgr = GetProtocolMgr(ap_mgr, property)) == 0) 	||
	((protocol = GetProtocol(p_mgr, proto_atom)) == 0) 	||
	!protocol->protocol.active)
      return;
    else
      {
	  protocol->protocol.active = FALSE;
	  if (XtIsRealized(shell))
	    UpdateProtocolMgrProperty(shell, p_mgr);
      }
}

/************************************<+>*************************************
 *
 *   XmSetProtocolHooks
 *
 *************************************<+>************************************/
#ifdef _NO_PROTO
void  XmSetProtocolHooks(shell, property, proto_atom, 
			 pre_hook, pre_closure,
			 post_hook, post_closure)
    Widget	shell;
    Atom	property;
    Atom	proto_atom;
    XtCallbackProc pre_hook, post_hook;
    caddr_t	pre_closure, post_closure;
#else /* _NO_PROTO */
void XmSetProtocolHooks (Widget shell, Atom property, Atom proto_atom, XtCallbackProc pre_hook, caddr_t pre_closure, XtCallbackProc post_hook, caddr_t post_closure)
#endif /* _NO_PROTO */
{
    XmAllProtocolsMgr	ap_mgr; 
    XmProtocolMgr	p_mgr ;
    XmProtocol		protocol;
    
    if (((ap_mgr = GetAllProtocolsMgr(shell)) == 0) 		||
	((p_mgr = GetProtocolMgr(ap_mgr, property)) == 0) 	||
	((protocol = GetProtocol(p_mgr, proto_atom)) == 0))
      return;
    
    protocol->protocol.pre_hook.callback = pre_hook;
    protocol->protocol.pre_hook.closure = pre_closure;
    protocol->protocol.post_hook.callback = post_hook;
    protocol->protocol.post_hook.closure = post_closure;
}
