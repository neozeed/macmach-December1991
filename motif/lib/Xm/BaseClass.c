#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)BaseClass.c	3.16 91/01/10";
#endif /* lint */
#endif /* REV_INFO */
/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
*  (c) Copyright 1989, 1990 DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
*  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
*  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
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
#define HAVE_EXTENSIONS

#include <Xm/XmP.h>
#include <X11/ShellP.h>
#include <Xm/MenuShell.h>
#include "BaseClassI.h"


#define IsBaseClass(wc) \
  ((wc == xmGadgetClass) 		||\
   (wc == xmManagerWidgetClass)		||\
   (wc == xmPrimitiveWidgetClass)	||\
   (wc == vendorShellWidgetClass) 	||\
   (wc == xmExtObjectClass)		||\
   (wc == xmMenuShellWidgetClass))


XrmQuark	XmQmotif;
XmBaseClassExt	*_Xm_fastPtr;

typedef struct _XmObjectClassWrapper{
    XtInitProc		initialize;
    XtSetValuesFunc	setValues;
    XtArgsProc		getValues;
    XtWidgetClassProc	classPartInit;
}XmObjectClassWrapper;

static XmObjectClassWrapper objectClassWrapper;

/* forward references */
static void 	InitializeLeafWrapper();
static Boolean	SetValuesLeafWrapper();
static void	GetValuesLeafWrapper();
static void	ClassPartInitLeafWrapper();

externaldef(xminheritclass) int _XmInheritClass = NULL;

#ifdef _NO_PROTO
Boolean _XmIsSlowSubclass(wc, bit)
    WidgetClass		wc;
    unsigned int	bit;
#else /* _NO_PROTO */
Boolean _XmIsSlowSubclass (WidgetClass wc, unsigned int bit)
#endif /* _NO_PROTO */
{
    XmBaseClassExt	*wcePtr = _XmGetBaseClassExtPtr(wc, XmQmotif);
    unsigned int	curr_bit;

    if (!wcePtr || !(*wcePtr))
      return False;

    if (curr_bit = _XmGetFlagsBit((*wcePtr)->flags, bit))
      return True;
    else
      return False;
}

XmGenericClassExt * _XmGetClassExtensionPtr(listHeadPtr, owner)
    XmGenericClassExt	* listHeadPtr;
    XrmQuark		owner;
{
    XmGenericClassExt	*lclPtr = listHeadPtr;

#ifdef DEBUG    
    if (!lclPtr) 
      {
	  XtWarning("_XmGetClassExtensionPtr: invalid class ext pointer");
	  return NULL;
      }
#endif /* DEBUG */
    for (; 
	 lclPtr && *lclPtr && ((*lclPtr)->record_type != owner);
	 lclPtr = (XmGenericClassExt *) &((*lclPtr)->next_extension)
	 ) ;

    return lclPtr;
}

static XContext wrapperContext = NULL;

#ifdef _NO_PROTO
XmWrapperData _XmGetWrapperData(class)
    WidgetClass		class;
#else /* _NO_PROTO */
XmWrapperData _XmGetWrapperData (WidgetClass class)
#endif /* _NO_PROTO */
{
    XmWrapperData	wrapperData;

    if (wrapperContext == NULL)
      wrapperContext = XUniqueContext();

    if (XFindContext((Display *)NULL,
		      (Window) class,
		      wrapperContext,
		      (caddr_t*)&wrapperData))
      {
	  wrapperData = XtNew(XmWrapperDataRec);
	  wrapperData->initializeNestingLevel = 0;
	  wrapperData->setValuesNestingLevel = 0;
	  wrapperData->getValuesNestingLevel = 0;
	  wrapperData->initializeLeaf = 0;
	  wrapperData->setValuesLeaf = 0;
	  wrapperData->getValuesLeaf = 0;
	  wrapperData->realize = 0;

	  XSaveContext((Display *)NULL,
		      (Window) class,
		      wrapperContext,
		      wrapperData);
      }
    return 
      wrapperData;
}

#ifdef _NO_PROTO
void _XmFreeWrapperData(class)
    WidgetClass		class;
#else /* _NO_PROTO */
void _XmFreeWrapperData (WidgetClass class)
#endif /* _NO_PROTO */
{
    XmWrapperData	wrapperData;

    XFindContext((Display *)NULL,
		 (Window) class,
		 wrapperContext,
		 (caddr_t*)&wrapperData);
    XtFree(wrapperData);
    XDeleteContext((Display *)NULL,
		   (Window) class,
		   wrapperContext);
}

typedef struct _ExtToContextRec{
    unsigned char	extType;
    XContext		context;
}ExtToContextRec, *ExtToContext;

static XContext ExtTypeToContext(extType)
    unsigned char	extType;
{
    static ExtToContextRec 	extToContextMap[16];
    Cardinal			i;
    ExtToContext		curr;
    XContext			context = NULL;

    for (i = 0, curr = &extToContextMap[0];
	 i < XtNumber(extToContextMap) && !context;
	 i++, curr++)
      {
	  if (curr->extType == extType)
	    context = curr->context;
	  else if (!curr->extType)
	    {
		curr->extType = extType;
		context =  
		  curr->context = 
		    XUniqueContext();
	    }
      }
    if (!context)
      XtWarning("no context found for extension");
    return context;
}
	 
typedef struct _XmAssocDataRec{
    XtPointer			data;
    struct _XmAssocDataRec	*next;
}XmAssocDataRec, *XmAssocData;


#ifdef _NO_PROTO
void _XmPushWidgetExtData(widget, data, extType)
    Widget	widget;
    unsigned char extType;
    XmWidgetExtData data;
#else /* _NO_PROTO */
void _XmPushWidgetExtData (Widget widget, XmWidgetExtData data, unsigned char extType)
#endif /* _NO_PROTO */
{
    XmAssocData		newData = NULL;
    XmAssocData		assocData = NULL;
    XmAssocData		*assocDataPtr;
    Boolean		empty;
    XContext		widgetExtContext = ExtTypeToContext(extType);

    newData= (XmAssocData)
      XtCalloc(sizeof(XmAssocDataRec), 1);

    newData->data = (XtPointer)data;

    if (XFindContext(XtDisplay(widget),
		     (Window) widget,
		     widgetExtContext,
		     (caddr_t*)&assocData))
      empty = True;
    else
      empty = False;

    for (assocDataPtr= &assocData; 
	 *assocDataPtr; 
	 assocDataPtr = &((*assocDataPtr)->next))
      {};

    *assocDataPtr = newData;

    if (empty)
      XSaveContext(XtDisplay(widget),
		   (Window) widget,
		   widgetExtContext,
		   assocData);
}

#ifdef _NO_PROTO
void _XmPopWidgetExtData(widget, dataRtn, extType)
    Widget	widget;
    unsigned char extType;
    XmWidgetExtData *dataRtn;
#else /* _NO_PROTO */
void _XmPopWidgetExtData (Widget widget, XmWidgetExtData *dataRtn, unsigned char extType)
#endif /* _NO_PROTO */
{
    XmAssocData		assocData = NULL,  *assocDataPtr;
    XmWidgetExtData	data;
    XContext		widgetExtContext = ExtTypeToContext(extType);

    if (XFindContext(XtDisplay(widget),
		     (Window) widget,
		     widgetExtContext,
		     (caddr_t*)&assocData))
      {
	  XtWarning("_XmPopWidgetExtData: no extension found with XFindContext");
      }

    for (assocDataPtr= &assocData; 
	 (*assocDataPtr)->next; 
	 assocDataPtr = &((*assocDataPtr)->next)){};

    if (*assocDataPtr == assocData)
      {
	  XDeleteContext(XtDisplay(widget),
			 (Window)widget,
			 widgetExtContext);
      }
    *dataRtn = (XmWidgetExtData) (*assocDataPtr)->data;
    XtFree(*assocDataPtr);
    *assocDataPtr = NULL;
}

#ifdef _NO_PROTO
XmWidgetExtData _XmGetWidgetExtData(widget, extType)
    Widget	widget;
    unsigned char extType;
#else /* _NO_PROTO */
XmWidgetExtData _XmGetWidgetExtData (Widget widget, unsigned char extType)
#endif /* _NO_PROTO */
{
    XmAssocData		assocData = NULL,  *assocDataPtr;
    XmWidgetExtData	data;
    XContext		widgetExtContext = ExtTypeToContext(extType);

    
    if ((XFindContext(XtDisplay(widget),
		      (Window) widget,
		      widgetExtContext,
		      (caddr_t*)&assocData)))
      {
#ifdef DEBUG
	  XtWarning("no extension data on stack");
#endif /* DEBUG */
#ifndef NO_TEMP_BC
	  assocData = (XmAssocData)
	    XtCalloc(sizeof(XmAssocDataRec), 1);
	  assocData->data =  (XtPointer)
	    XtCalloc(sizeof(XmWidgetExtDataRec), 1);
	  XSaveContext(XtDisplay(widget),
		       (Window) widget,
		       widgetExtContext,
		       assocData);
	  return (XmWidgetExtData)assocData->data ;
#endif
      }
    else
      {
	  for (assocDataPtr= &assocData; 
	       (*assocDataPtr)->next; 
	       assocDataPtr = &((*assocDataPtr)->next)){};

	  return (XmWidgetExtData) (*assocDataPtr)->data;
      }
}

#ifdef _NO_PROTO
void _XmFreeWidgetExtData(widget)
    Widget	widget;
#else /* _NO_PROTO */
void _XmFreeWidgetExtData (Widget widget)
#endif /* _NO_PROTO */
{
    XtWarning(" XmFreeWidgetExtData is an unsupported routine");
}

#ifdef _NO_PROTO
void _XmBaseClassPartInitialize(wc)
    WidgetClass	wc;
#else /* _NO_PROTO */
void _XmBaseClassPartInitialize(WidgetClass wc)
#endif /* _NO_PROTO */
{
#ifdef notdef
    XtWarning("_XmBaseClassPartInitialize is an ubnsupported");
#endif /* notdef */
}



/************************************************************************
 *
 *  BaseClassPartInitialize
 *
 ************************************************************************/
static XmBaseClassExt * BaseClassPartInitialize (wc)
    WidgetClass	wc;
{
    XmBaseClassExt		*wcePtr, *scePtr;
    Cardinal			i;
    Boolean			inited = False;
    Boolean			isBaseClass = IsBaseClass(wc);

    /* 
     * this routine is called out of the ClassPartInitRootWrapper. It
     * needs to make sure that this is a motif class and if it is,
     * then to initialize it. We assume that the base classes always
     * have a static initializer !!!
     */

    wcePtr = _XmGetBaseClassExtPtr(wc, XmQmotif);
    scePtr = _XmGetBaseClassExtPtr(wc->core_class.superclass, XmQmotif);

    if (!isBaseClass && (!scePtr || !(*scePtr)))
      return NULL;

    if (!(*wcePtr))
      {
	  inited = False;
	  *wcePtr = (XmBaseClassExt)
	    XtCalloc(sizeof(XmBaseClassExtRec), 1);
#ifndef NO_CLASS_PART_INIT_HOOK
	  (*wcePtr)->classPartInitPrehook = XmInheritClassPartInitPrehook;
	  (*wcePtr)->classPartInitPosthook = XmInheritClassPartInitPosthook;
#endif /* NO_CLASS_PART_INIT_HOOK */
	  (*wcePtr)->initializePrehook 	= XmInheritInitializePrehook;
	  (*wcePtr)->setValuesPrehook 	= XmInheritSetValuesPrehook;
	  (*wcePtr)->getValuesPrehook 	= XmInheritGetValuesPrehook;
	  (*wcePtr)->initializePosthook = XmInheritInitializePosthook;
	  (*wcePtr)->setValuesPosthook 	= XmInheritSetValuesPosthook;
	  (*wcePtr)->getValuesPosthook 	= XmInheritGetValuesPosthook;
	  (*wcePtr)->secondaryObjectClass = XmInheritClass;
	  (*wcePtr)->secondaryObjectCreate = XmInheritCreate;
	  (*wcePtr)->getSecResData 	= XmInheritGetSecResData;
      }
    else
      inited = True;

    /* this should get done by the static initializers */
    for (i = 0; i < 32; i++)
      (*wcePtr)->flags[i] = 0;

    if (scePtr && *scePtr)
      {
	  if (!inited)
	    {
		(*wcePtr)->next_extension = NULL;
		(*wcePtr)->record_type 	= (*scePtr)->record_type;
		(*wcePtr)->version	= (*scePtr)->version;
		(*wcePtr)->record_size	= (*scePtr)->record_size;
	    }
#ifndef NO_CLASS_PART_INIT_HOOK
	  if ((*wcePtr)->classPartInitPrehook == XmInheritClassPartInitPrehook)
	    (*wcePtr)->classPartInitPrehook = (*scePtr)->classPartInitPrehook;
	  if ((*wcePtr)->classPartInitPosthook == XmInheritClassPartInitPosthook)
	    (*wcePtr)->classPartInitPosthook = (*scePtr)->classPartInitPosthook;
#endif /* NO_CLASS_PART_INIT_HOOK */
	  if ((*wcePtr)->initializePrehook == XmInheritInitializePrehook)
	    (*wcePtr)->initializePrehook = (*scePtr)->initializePrehook;
	  if ((*wcePtr)->setValuesPrehook == XmInheritSetValuesPrehook)
	    (*wcePtr)->setValuesPrehook = (*scePtr)->setValuesPrehook;
	  if ((*wcePtr)->getValuesPrehook == XmInheritGetValuesPrehook)
	    (*wcePtr)->getValuesPrehook = (*scePtr)->getValuesPrehook;
	  if ((*wcePtr)->initializePosthook == XmInheritInitializePosthook)
	    (*wcePtr)->initializePosthook = (*scePtr)->initializePosthook;
	  if ((*wcePtr)->setValuesPosthook == XmInheritSetValuesPosthook)
	    (*wcePtr)->setValuesPosthook = (*scePtr)->setValuesPosthook;
	  if ((*wcePtr)->getValuesPosthook == XmInheritGetValuesPosthook)
	    (*wcePtr)->getValuesPosthook = (*scePtr)->getValuesPosthook;
	  if ((*wcePtr)->secondaryObjectClass == XmInheritClass)
	    (*wcePtr)->secondaryObjectClass = (*scePtr)->secondaryObjectClass;
	  if ((*wcePtr)->secondaryObjectCreate == XmInheritCreate)
	    (*wcePtr)->secondaryObjectCreate = (*scePtr)->secondaryObjectCreate;
	  if ((*wcePtr)->getSecResData == XmInheritGetSecResData)
	    (*wcePtr)->getSecResData = (*scePtr)->getSecResData;
      }
#ifdef DEBUG
    else if (!IsBaseClass(wc))
      XtError("class must have non-null superclass extension");
#endif /* DEBUG */

    return wcePtr;
}


#ifndef NO_CLASS_PART_INIT_HOOK
/*
 * This function replaces the objectClass classPartInit slot and is
 * called at the start of the first XtCreate invocation.
 */
static void ClassPartInitRootWrapper(wc)
    WidgetClass	wc;
{
    XtWidgetClassProc			*leafFuncPtr;
    XmBaseClassExt			*wcePtr;


    wcePtr = BaseClassPartInitialize(wc);
    /*
     * check that it's a class that we know about
     */
    if (wcePtr && *wcePtr)
      {
	  if ((*wcePtr)->classPartInitPrehook)
	    (*((*wcePtr)->classPartInitPrehook)) (wc);

	  /*
	   * if we have a prehook, then envelop the leaf class function
	   * that whould be called last. 
	   */
	  if ((*wcePtr)->classPartInitPosthook)
	    {
		XmWrapperData 		wrapperData;

		wrapperData = _XmGetWrapperData(wc);
		leafFuncPtr = (XtWidgetClassProc *)
		  &(wc->core_class.class_part_initialize);
		wrapperData->classPartInitLeaf = *leafFuncPtr;
		*leafFuncPtr = ClassPartInitLeafWrapper;
	    }
      }
    if (objectClassWrapper.classPartInit)
      (* objectClassWrapper.classPartInit) (wc);
}



static void ClassPartInitLeafWrapper(wc)
    WidgetClass	wc;
{
    XtWidgetClassProc			*leafFuncPtr;
    XmBaseClassExt		*wcePtr;

    wcePtr = _XmGetBaseClassExtPtr(wc, XmQmotif); 
    
    if (*wcePtr && (*wcePtr)->classPartInitPosthook)
      {
	  XmWrapperData 		wrapperData;
	  wrapperData = _XmGetWrapperData(wc);
	  leafFuncPtr = (XtWidgetClassProc *)
	    &(wc->core_class.class_part_initialize);
	  
	  if (wrapperData->classPartInitLeaf)
	    (* wrapperData->classPartInitLeaf) (wc);
	  if ((*wcePtr)->classPartInitPosthook)
	    (*((*wcePtr)->classPartInitPosthook)) (wc);
#ifdef DEBUG
	  else
	    XtWarning("there should be a non-null hook for a leaf wrapper");
#endif /* DEBUG */
	  *leafFuncPtr = wrapperData->classPartInitLeaf;
	  wrapperData->classPartInitLeaf = NULL;
      }
}
#endif /* NO_CLASS_PART_INIT_HOOK */


/*
 * This function replaces the objectClass initialize slot and is
 * called at the start of every XtCreate invocation.
 */
static void InitializeRootWrapper(req, new, args, num_args)
    Widget	req, new;
    ArgList	args;
    Cardinal	*num_args;
{
    XtInitProc			*leafFuncPtr;
    XmBaseClassExt		*wcePtr;
    WidgetClass			wc = XtClass(new);
 
    wcePtr = _XmGetBaseClassExtPtr(wc, XmQmotif); 

    /*
     * check that it's a class that we know about
     */
    if (wcePtr && *wcePtr)
      {
	  if ((*wcePtr)->initializePrehook)
	    (*((*wcePtr)->initializePrehook)) (req, new, args,  num_args);

	  /*
	   * if we have a prehook, then envelop the leaf class function
	   * that whould be called last. This is different if the
	   * parent is a constraint subclass, since constraint
	   * initialize is called after core initialize.
	   */
	  if ((*wcePtr)->initializePosthook)
	    {
		XmWrapperData 		wrapperData;

		if (XtParent(new) && XtIsConstraint(XtParent(new)))
		  {
		      ConstraintWidgetClass cwc;
		      
		      cwc = (ConstraintWidgetClass)XtClass(XtParent(new));
		      wrapperData = _XmGetWrapperData((WidgetClass)cwc);
		      leafFuncPtr = (XtInitProc *)
			&(cwc->constraint_class.initialize);
		  }
		else
		  {
		      wrapperData = _XmGetWrapperData((WidgetClass)wc);
		      leafFuncPtr = (XtInitProc *)
			&(wc->core_class.initialize);
		  }
		/*
		 * worry about reentrancy by using nestingLevel. If at top, then
		 * replace the the function of the leaf class
		 */
		if (!wrapperData->initializeNestingLevel++)
		  {
		      wrapperData->initializeLeaf = *leafFuncPtr;
		      *leafFuncPtr = InitializeLeafWrapper;
		  }
	    }
      }
    if (objectClassWrapper.initialize)
      (* objectClassWrapper.initialize) (req, new, args, num_args);
}


static void InitializeLeafWrapper(req, new, args, num_args)
    Widget	req, new;
    ArgList	args;
    Cardinal	*num_args;
{
    XtInitProc			*leafFuncPtr;
    XmBaseClassExt		*wcePtr;
    WidgetClass			wc = XtClass(new);

    wcePtr = _XmGetBaseClassExtPtr(wc, XmQmotif); 

    if (*wcePtr && (*wcePtr)->initializePosthook)
      {
	  XmWrapperData 		wrapperData;
	  
	  if (XtParent(new) && XtIsConstraint(XtParent(new)))
	    {
		ConstraintWidgetClass cwc;
		
		cwc = (ConstraintWidgetClass)XtClass(XtParent(new));
		wrapperData = _XmGetWrapperData((WidgetClass)cwc);
		leafFuncPtr = (XtInitProc *)
		  &(cwc->constraint_class.initialize);
	    }
	  else
	    {
		wrapperData = _XmGetWrapperData(wc);
		leafFuncPtr = (XtInitProc *)
		  &(wc->core_class.initialize);
	    }
	  if (wrapperData->initializeLeaf)
	    (* wrapperData->initializeLeaf) (req, new, args, num_args);
	  if ((*wcePtr)->initializePosthook)
	    (*((*wcePtr)->initializePosthook)) (req, new, args,  num_args);
#ifdef DEBUG
	  else
	    XtWarning("there should be a non-null hook for a leaf wrapper");
#endif /* DEBUG */
	  
	  /*
	   * worry about reentrancy by using nestingLevel. If at top, then
	   * replace the the function of the leaf class
	   */
	  if (--(wrapperData->initializeNestingLevel) == 0)
	    {
		*leafFuncPtr = wrapperData->initializeLeaf;
		wrapperData->initializeLeaf = NULL;
	    }
      }
}


/*
 * This function replaces the objectClass set_values slot and is
 * called at the start of every XtSetValues invocation.
 */
static Boolean SetValuesRootWrapper(current, req, new, args, num_args)
    Widget	current, req, new;
    ArgList	args;
    Cardinal	*num_args;
{
    XtSetValuesFunc		*leafFuncPtr;
    XmBaseClassExt		*wcePtr;
    WidgetClass			wc = XtClass(new);
    Boolean			returnVal = False;

    wcePtr = _XmGetBaseClassExtPtr(wc, XmQmotif); 
    /*
     * check that it's a class that we know about
     */
    if (wcePtr && (*wcePtr))
      {
	  if ((*wcePtr)->setValuesPrehook)
	    returnVal |= 
	      (*((*wcePtr)->setValuesPrehook)) (current, req, new, args,  num_args);

	  /*
	   * if we have a prehook, then envelop the leaf class function
	   * that whould be called last. This is different if the
	   * parent is a constraint subclass, since constraint
	   * set_values is called after core set_values.
	   */
	  if ((*wcePtr)->setValuesPosthook)
	    {
		XmWrapperData 		wrapperData;

		if (XtParent(new) && XtIsConstraint(XtParent(new)))
		  {
		      ConstraintWidgetClass cwc;
		      
		      cwc = (ConstraintWidgetClass)XtClass(XtParent(new));
		      wrapperData = _XmGetWrapperData((WidgetClass)cwc);
		      leafFuncPtr = (XtSetValuesFunc *)
			&(cwc->constraint_class.set_values);
		  }
		else
		  {
		      wrapperData = _XmGetWrapperData(wc);
		      leafFuncPtr = (XtSetValuesFunc *)
			&(wc->core_class.set_values);
		  }
		/*
		 * worry about reentrancy by using nestingLevel. If at top, then
		 * replace the the function of the leaf class
		 */
		if (!wrapperData->setValuesNestingLevel++)
		  {
		      wrapperData->setValuesLeaf = *leafFuncPtr;
		      *leafFuncPtr = SetValuesLeafWrapper;
		  }
	    }
      }
    if (objectClassWrapper.setValues)
      returnVal |= 
	(* objectClassWrapper.setValues) (current, req, new, args,
					  num_args);
    return returnVal;
}

static Boolean SetValuesLeafWrapper(current, req, new, args, num_args)
    Widget	current, req, new;
    ArgList	args;
    Cardinal	*num_args;
{
    XtSetValuesFunc		*leafFuncPtr;
    XmBaseClassExt		*wcePtr;
    XmWrapperData 		wrapperData;
    Boolean			returnVal = False;
    WidgetClass			wc = XtClass(new);

    wcePtr = _XmGetBaseClassExtPtr(wc, XmQmotif); 

    if (*wcePtr && (*wcePtr)->setValuesPosthook)
      {
	  
	  if (XtParent(new) && XtIsConstraint(XtParent(new)))
	    {
		ConstraintWidgetClass cwc;
		
		cwc = (ConstraintWidgetClass)XtClass(XtParent(new));
		wrapperData = _XmGetWrapperData((WidgetClass)cwc);
		leafFuncPtr = (XtSetValuesFunc *)
		  &(cwc->constraint_class.set_values);
	    }
	  else
	    {
		wrapperData = _XmGetWrapperData(wc);
		leafFuncPtr = (XtSetValuesFunc *)
		  &(wc->core_class.set_values);
	    }
	  if (wrapperData->setValuesLeaf)
	    returnVal = (* wrapperData->setValuesLeaf) (current, req, new, args, num_args);
	  if ((*wcePtr)->setValuesPosthook)
	    returnVal |= 
	      (*((*wcePtr)->setValuesPosthook)) (current, req, new, args,  num_args);
#ifdef DEBUG
	  else
	    XtWarning("there should be a non-null hook for a leaf wrapper");
#endif /* DEBUG */
	  
	  /*
	   * worry about reentrancy by using nestingLevel. If at top, then
	   * replace the the function of the leaf class
	   */
	  if (--(wrapperData->setValuesNestingLevel) == 0)
	    {
		*leafFuncPtr = wrapperData->setValuesLeaf;
		wrapperData->setValuesLeaf = NULL;
	    }
      }
    return returnVal;
}

/*
 * This function replaces the objectClass get_values slot and is
 * called at the start of every XtGetValues invocation.
 */
static void GetValuesRootWrapper(new, args, num_args)
    Widget	new;
    ArgList	args;
    Cardinal	num_args;
{
    XtArgsProc			*leafFuncPtr;
    XmBaseClassExt		*wcePtr;
    WidgetClass			wc = XtClass(new);
 
    wcePtr = _XmGetBaseClassExtPtr(wc, XmQmotif); 
    /*
     * check that it's a class that we know about
     */
    if (wcePtr && (*wcePtr))
      {
	  if ((*wcePtr)->getValuesPrehook)
	    (*((*wcePtr)->getValuesPrehook)) (new, args,  num_args);

	  /*
	   * if we have a prehook, then envelop the leaf class function
	   * that whould be called last. This is different if the
	   * parent is a constraint subclass, since constraint
	   * get_values is called after core get_values.
	   */
	  if ((*wcePtr)->getValuesPosthook)
	    {
		XmWrapperData 		wrapperData;

#ifdef SUPPORT_GET_VALUES_EXT
		if (XtParent(new) && XtIsConstraint(XtParent(new)))
		  {
		      ConstraintWidgetClass cwc;
		      
		      cwc = (ConstraintWidgetClass)XtClass(XtParent(new));
		      wrapperData = _XmGetWrapperData(cwc);
		      leafFuncPtr = (XtGetValuesFunc *)
			&(cwc->constraint_class.get_values);
		  }
		else
#endif		  
		  {
		      wrapperData = _XmGetWrapperData(wc);
		      leafFuncPtr = (XtArgsProc *)
			&(wc->core_class.get_values_hook);
		  }
		/*
		 * worry about reentrancy by using nestingLevel. If at top, then
		 * replace the the function of the leaf class
		 */
		if (!wrapperData->getValuesNestingLevel++)
		  {
		      wrapperData->getValuesLeaf = *leafFuncPtr;
		      *leafFuncPtr = GetValuesLeafWrapper;
		  }
	    }
      }
    if (objectClassWrapper.getValues)
      (* objectClassWrapper.getValues) (new, args, num_args);
}


static void GetValuesLeafWrapper(new, args, num_args)
    Widget	new;
    ArgList	args;
    Cardinal	num_args;
{
    XtArgsProc			*leafFuncPtr;
    XmBaseClassExt		*wcePtr;
    XmWrapperData 		wrapperData;
    WidgetClass			wc = XtClass(new);

    wcePtr = _XmGetBaseClassExtPtr(wc, XmQmotif); 

    if (*wcePtr && (*wcePtr)->getValuesPosthook)
      {
#ifdef SUPPORT_GET_VALUES_EXT
	  if (XtParent(new) && XtIsConstraint(XtParent(new)))
	    {
		ConstraintWidgetClass cwc;
		
		cwc = (ConstraintWidgetClass)XtClass(XtParent(new));
		wrapperData = _XmGetWrapperData(cwc);
		leafFuncPtr = (XtSetValuesFunc *)
		  &(cwc->constraint_class.set_values);
	    }
	  else
#endif
	    {
		wrapperData = _XmGetWrapperData(wc);
		leafFuncPtr = (XtArgsProc *)
		  &(wc->core_class.get_values_hook);
	    }
	  if (wrapperData->getValuesLeaf)
	    (* wrapperData->getValuesLeaf) (new, args, num_args);
	  if ((*wcePtr)->getValuesPosthook)
	    (*((*wcePtr)->getValuesPosthook)) (new, args,  num_args);
#ifdef DEBUG
	  else
	    XtWarning("there should be a non-null hook for a leaf wrapper");
#endif /* DEBUG */
	  
	  /*
	   * worry about reentrancy by using nestingLevel. If at top, then
	   * replace the the function of the leaf class
	   */
	  if (--(wrapperData->getValuesNestingLevel) == 0)
	    {
		*leafFuncPtr = wrapperData->getValuesLeaf;
		wrapperData->getValuesLeaf = NULL;
	    }
      }
}

#ifdef _NO_PROTO
void _XmInitializeExtensions()
#else /* _NO_PROTO */
void _XmInitializeExtensions (void)
#endif /* _NO_PROTO */
{
    static Boolean		firstTime = True;

    if (firstTime)
      {
	  XmQmotif = XrmStringToQuark("OSF_MOTIF");

	  objectClassWrapper.initialize =
	    objectClass->core_class.initialize;
	  objectClassWrapper.setValues =
	    objectClass->core_class.set_values;
	  objectClassWrapper.getValues =
	    objectClass->core_class.get_values_hook;

#ifndef NO_CLASS_PART_INIT_HOOK
	  objectClassWrapper.classPartInit =
	    objectClass->core_class.class_part_initialize;
	  objectClass->core_class.class_part_initialize = 
	    ClassPartInitRootWrapper;
#endif /* NO_CLASS_PART_INIT_HOOK */

	  objectClass->core_class.initialize = 
	    InitializeRootWrapper;
	  objectClass->core_class.set_values = 
	    SetValuesRootWrapper;
	  objectClass->core_class.get_values_hook =
	    GetValuesRootWrapper;
	  firstTime = False;
      }
}


