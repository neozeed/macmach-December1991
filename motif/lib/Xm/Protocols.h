#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)Protocols.h	3.7 90/06/20";
#endif /* lint */
#endif /* REV_INFO */
/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990, OPEN SOFTWARE FOUNDATION, INC.
*  (c) Copyright 1988, 1989, 1990, HEWLETT-PACKARD COMPANY
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
* X Window System is a trademark of the Massachusetts Institute of Technology
*
*******************************************************************************
******************************************************************************/
#ifndef _XmProtocols_h
#define _XmProtocols_h

#include <Xm/AtomMgr.h>

/* should be in XmP.h */

#ifndef XmCR_WM_PROTOCOLS
#define XmCR_WM_PROTOCOLS 666
#endif /* XmCR_WM_PROTOCOLS */

/* define the XM_PROTOCOLS atom for use in  routines */
#ifdef XA_WM_PROTOCOLS
#define XM_WM_PROTOCOL_ATOM(shell) XA_WM_PROTOCOLS
#else
#define XM_WM_PROTOCOL_ATOM(shell) \
    XmInternAtom(XtDisplay(shell),"WM_PROTOCOLS",FALSE)
#endif /* XA_WM_PROTOCOLS */


#define XmAddWMProtocols(shell, protocols, num_protocols) \
      XmAddProtocols(shell, XM_WM_PROTOCOL_ATOM(shell), \
			 protocols, num_protocols)

#define XmRemoveWMProtocols(shell, protocols, num_protocols) \
      XmRemoveProtocols(shell, XM_WM_PROTOCOL_ATOM(shell), \
			protocols, num_protocols)

#define XmAddWMProtocolCallback(shell, protocol, callback, closure) \
      XmAddProtocolCallback(shell, XM_WM_PROTOCOL_ATOM(shell), \
			    protocol, callback, closure)

#define XmRemoveWMProtocolCallback(shell, protocol, callback, closure) \
  XmRemoveProtocolCallback(shell, XM_WM_PROTOCOL_ATOM(shell), \
			    protocol, callback, closure)

#define XmActivateWMProtocol(shell, protocol) \
      XmActivateProtocol(shell, XM_WM_PROTOCOL_ATOM(shell), protocol)

#define XmDeactivateWMProtocol(shell, protocol) \
      XmDeactivateProtocol(shell, XM_WM_PROTOCOL_ATOM(shell), protocol)

#define XmSetWMProtocolHooks(shell, protocol, pre_h, pre_c, post_h, post_c) \
      XmSetProtocolHooks(shell, XM_WM_PROTOCOL_ATOM(shell), \
			 protocol, pre_h, pre_c, post_h, post_c)

#ifdef _NO_PROTO
extern void XmAddProtocols ();
extern void XmRemoveProtocols ();
extern void XmAddProtocolCallback ();
extern void XmRemoveProtocolCallback ();
extern void XmActivateProtocol ();
extern void XmDeactivateProtocol ();
extern void XmSetProtocolHooks ();
extern void _XmInstallProtocols();
#else /* _NO_PROTO */

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

extern void XmAddProtocols (Widget shell, Atom property, Atom *protocols, Cardinal num_protocols);
extern void XmRemoveProtocols (Widget shell, Atom property, Atom *protocols, Cardinal num_protocols);
extern void XmAddProtocolCallback (Widget shell, Atom property, Atom proto_atom, XtCallbackProc callback, caddr_t closure);
extern void XmRemoveProtocolCallback (Widget shell, Atom property, Atom proto_atom, XtCallbackProc callback, caddr_t closure);
extern void XmActivateProtocol (Widget shell, Atom property, Atom proto_atom);
extern void XmDeactivateProtocol (Widget shell, Atom property, Atom proto_atom);
extern void XmSetProtocolHooks (Widget shell, Atom property, Atom proto_atom, XtCallbackProc pre_hook, caddr_t pre_closure, XtCallbackProc post_hook, caddr_t post_closure);
extern void _XmInstallProtocols (Widget w);
#endif /* _NO_PROTO */

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif /* _XmProtocols_h */
