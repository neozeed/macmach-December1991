#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)MenuUtil.c	3.8 91/01/10";
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
#include <stdio.h>
#include <ctype.h>
#include <Xm/XmP.h>
#include <X11/ShellP.h>

XContext _XmMenuCursorContext = NULL;

/*
 * The following set of functions support the menu cursor functionality.
 * A client has the ability to specify which cursor it would like displayed
 * anytime a menu is displayed.  A cursor can be specified for each display
 * opened by the application.
 */

#ifdef _NO_PROTO
void _XmInitializeMenuCursor()
#else /* _NO_PROTO */
void _XmInitializeMenuCursor (void)
#endif /* _NO_PROTO */
{
   /* Create the context used to store the menu cursors */

   if (_XmMenuCursorContext == NULL)
       _XmMenuCursorContext = XUniqueContext();
}

static void GetApplNameAndClass(m, nameList, classList)
Widget m;
XrmNameList nameList;
XrmClassList classList;
{
   while (m->core.parent != NULL)
      m = m->core.parent;

   if (XtIsApplicationShell(m))
      classList[0] = ((ApplicationShellWidget)m)->application.xrm_class;
   else
      classList[0] = XtClass(m)->core_class.xrm_class;

   nameList[0] = m->core.xrm_name;
}


#ifdef _NO_PROTO
void _XmCreateMenuCursor (m)
Widget m;
#else /* _NO_PROTO */
void _XmCreateMenuCursor (Widget m)
#endif /* _NO_PROTO */
{
   static String dftCursor = "arrow";
   Cursor cursor;
   XrmValue from, to;

   if (XFindContext(XtDisplay(m), NULL, _XmMenuCursorContext, 
		    (caddr_t*)&cursor) ==
       XCNOENT)
   {
      /* Create a new cursor for this display */

      /* Start by seeing if a cursor was specified in the database */
      XrmName xrm_name[3];
      XrmClass xrm_class[3];
      XrmRepresentation rep_type;
      XrmValue value;

      GetApplNameAndClass(m, xrm_name, xrm_class);
      xrm_name[1] = XrmStringToQuark(XmNmenuCursor);
      xrm_name[2] = NULL;
      xrm_class[1] = XrmStringToQuark(XmCCursor);
      xrm_class[2] = NULL;

      if (XrmQGetResource(XtDatabase(XtDisplay(m)), xrm_name, xrm_class,
                  &rep_type, &value ))
      {
          if (rep_type == XrmStringToQuark(XmRString)) 
          {
             from.size = strlen(value.addr) + 1;
             from.addr = value.addr;
             XtConvert(m, XmRString, &from, XmRCursor, &to);
             if (to.size != 0)
             {
                cursor = *((Cursor *)to.addr);
                XSaveContext(XtDisplay(m), NULL, _XmMenuCursorContext, 
			     (void*)	cursor);
                return;
             }
          }
      }

      /* Use the default cursor */
      from.size = strlen(dftCursor) + 1;
      from.addr = dftCursor;
      XtConvert(m, XmRString, &from, XmRCursor, &to);
      cursor = *((Cursor *)to.addr);
      XSaveContext(XtDisplay(m), NULL, _XmMenuCursorContext, (void*)cursor);
   }
}


/* Global menu cursor manipulation functions */

#ifdef _NO_PROTO
void XmSetMenuCursor (display, cursorId)
Display * display;
Cursor cursorId;
#else /* _NO_PROTO */
void XmSetMenuCursor (Display *display, Cursor cursorId)
#endif /* _NO_PROTO */
{
      if (_XmMenuCursorContext == NULL)
         _XmMenuCursorContext = XUniqueContext();

      XSaveContext(display, NULL, _XmMenuCursorContext, (void*)cursorId);
}


#ifdef _NO_PROTO
Cursor XmGetMenuCursor (display)
Display * display;
#else /* _NO_PROTO */
Cursor XmGetMenuCursor (Display *display)
#endif /* _NO_PROTO */
{
   Cursor cursor;

   if (_XmMenuCursorContext == NULL)
      return(None);

   if (XFindContext(display, NULL, _XmMenuCursorContext, 
		    (caddr_t*)&cursor) == XCNOENT)
      return(None);
   else
      return(cursor);
}

/*
 * The following functions are used to separate the private class function
 * in RowColumn from the buttons that may be children of the RowColumn.
 * This is simply an interface supplied so that the buttons will not have
 * to have intimate knowledge of the RowColumn class functions.
 */


static XContext menuProcContext = NULL;

/*
 * this routine is called at RowColumn class init to register with the
 * context manager the address of the menuProcedureEntry routine.
 */
#ifdef _NO_PROTO
void _XmSaveMenuProcContext (address)
caddr_t address;
#else /* _NO_PROTO */
void _XmSaveMenuProcContext (caddr_t address)
#endif /* _NO_PROTO */
{
   if (menuProcContext == NULL)
   {
      /* has not been called before */
      menuProcContext = XUniqueContext();

      XSaveContext (NULL, NULL, menuProcContext, address);
   }
}


/*
 * This routine is used by the button children of the RowColumn (currently
 * all label and labelgadget subclasses) to get the address of the
 * menuProcedureEntry routine.  It is called by the buttons class init
 * routines.
 */
#ifdef _NO_PROTO
caddr_t _XmGetMenuProcContext()
#else /* _NO_PROTO */
caddr_t _XmGetMenuProcContext (void)
#endif /* _NO_PROTO */
{
   caddr_t data;
   
   if (menuProcContext)
   {
      if (! XFindContext (NULL, NULL, menuProcContext, &data))
      {
	 /* context found, return the data */
	 return (data);
      }
   }

   /* no context registered yet */
   return (NULL);
}

   
