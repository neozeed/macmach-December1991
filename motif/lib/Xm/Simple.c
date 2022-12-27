#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)Simple.c	3.16 91/01/10";
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
* DEC is a registered trademark of Digital Equipment Corporation
* DIGITAL is a registered trademark of Digital Equipment Corporation
* X Window System is a trademark of the Massachusetts Institute of Technology
*
*******************************************************************************
******************************************************************************/

#include <X11/StringDefs.h>
#include <Xm/RowColumnP.h>
#include <Xm/PushBG.h>
#include <Xm/ToggleBG.h>
#include <Xm/CascadeBG.h>
#include <Xm/LabelG.h>
#include <Xm/SeparatoG.h>


static XtResource SimpleMenuResources[] =
{
	{ XmNbuttonCount, XmCButtonCount, XmRInt, sizeof(int),
	  XtOffset(XmSimpleMenu, count),
	  XmRImmediate, (caddr_t) 0
	},
	{ XmNpostFromButton, XmCPostFromButton, XmRInt, sizeof(int),
	  XtOffset(XmSimpleMenu, post_from_button),
	  XmRImmediate, (caddr_t) -1
	},
	{ XmNsimpleCallback, XmCCallback, XmRCallbackProc, 
	  sizeof(XtCallbackProc), XtOffset(XmSimpleMenu, callback),
	  XmRImmediate, (caddr_t) NULL
	},
	{ XmNbuttons, XmCButtons, XmRXmStringTable,
	  sizeof(XmStringTable), XtOffset(XmSimpleMenu, label_string),
	  XmRImmediate, (caddr_t) NULL
	},
	{ XmNbuttonAccelerators, XmCButtonAccelerators, XmRStringTable, 
	  sizeof(String *), XtOffset(XmSimpleMenu,  accelerator),
	  XmRImmediate, (caddr_t) NULL
	},
	{ XmNbuttonAcceleratorText, XmCButtonAcceleratorText, 
	  XmRXmStringTable, sizeof(XmStringTable),
	  XtOffset(XmSimpleMenu, accelerator_text),
	  XmRImmediate, (caddr_t) NULL
	},
	{ XmNbuttonMnemonics, XmCButtonMnemonics, XmRKeySymTable,
	  sizeof(XmKeySymTable), XtOffset(XmSimpleMenu, mnemonic),
	  XmRImmediate, (caddr_t) NULL
	},
	{ XmNbuttonMnemonicCharSets, XmCButtonMnemonicCharSets, 
	  XmRCharSetTable, sizeof(XmStringCharSetTable),
	  XtOffset(XmSimpleMenu, mnemonic_charset),
	  XmRImmediate, (caddr_t) NULL
	},
	{ XmNbuttonType, XmCButtonType, XmRButtonType,
	  sizeof(XmButtonTypeTable), XtOffset(XmSimpleMenu, button_type),
	  XmRImmediate, (caddr_t) NULL
	},
	{ XmNbuttonSet, XmCButtonSet, XmRInt,
	  sizeof(int), XtOffset(XmSimpleMenu, button_set),
	  XmRImmediate, (caddr_t) -1
	},
	{ XmNoptionLabel, XmCOptionLabel, XmRXmString,
	  sizeof(XmString), XtOffset(XmSimpleMenu, option_label),
	  XmRImmediate, (caddr_t) NULL
	},
	{ XmNoptionMnemonic, XmCOptionMnemonic, XmRKeySym,
	  sizeof (KeySym), XtOffset(XmSimpleMenu, option_mnemonic),
          XmRImmediate, (caddr_t) NULL
	},
};

static void EvaluateConvenienceStructure(rc, sm)
XmRowColumnWidget rc;
XmSimpleMenu sm;
{
	int i, n;
	char name_buf[20];
	int button_count = 0;
	int separator_count = 0;
	int label_count = 0;
	Arg args[6];
	Widget child;
	XmButtonType btype;

	for (i = 0; i < sm->count; i++)
	{
		n = 0;
		if (sm->label_string && sm->label_string[i])
		{
			XtSetArg(args[n], 
				XmNlabelString, sm->label_string[i]);         n++;
		}
		if (sm->accelerator && sm->accelerator[i])
		{
			XtSetArg(args[n], 
				XmNaccelerator, sm->accelerator[i]);            n++;
		}
		if (sm->accelerator_text && sm->accelerator_text[i])
		{
			XtSetArg(args[n],
				XmNacceleratorText, sm->accelerator_text[i]); n++;
		}
		if (sm->mnemonic && sm->mnemonic[i])
		{
			XtSetArg(args[n],
				XmNmnemonic, sm->mnemonic[i]);                n++;
		}
		if (sm->mnemonic_charset && sm->mnemonic_charset[i])
		{
			XtSetArg(args[n],
				XmNmnemonicCharSet, sm->mnemonic_charset[i]); n++;
		}
		
		/* Dynamic Defaulting of button type */

		btype = (sm->button_type ? sm->button_type[i] : XmNONE);

		if (btype == XmNONE)
		{
			if (rc->row_column.type == XmMENU_BAR)
				btype = XmCASCADEBUTTON;
			else
				btype = XmPUSHBUTTON;
		}
		
		switch (btype)
		{
			case XmTITLE:
				sprintf(name_buf,"label_%d", label_count++);
				child = XtCreateManagedWidget(name_buf,
					xmLabelGadgetClass, rc, args, n);
			break;
			case XmDOUBLE_SEPARATOR:
				XtSetArg(args[n], XmNseparatorType, XmDOUBLE_LINE); n++;
			case XmSEPARATOR:
				sprintf(name_buf,"separator_%d", separator_count++);
				child = XtCreateManagedWidget(name_buf, 
					xmSeparatorGadgetClass, rc, args, n);
			break;
			case XmPUSHBUTTON:
				sprintf(name_buf,"button_%d", button_count++);
				child = XtCreateManagedWidget(name_buf, 
					xmPushButtonGadgetClass, rc, args, n);
				if (sm->callback)
					XtAddCallback(child, XmNactivateCallback,
						sm->callback, (button_count - 1));
			break;
			case XmRADIOBUTTON:
				XtSetArg(args[n], XmNindicatorType, XmONE_OF_MANY); n++;
			case XmCHECKBUTTON:
				sprintf(name_buf,"button_%d", button_count++);
				XtSetArg(args[n], XmNindicatorOn, TRUE); n++;
				child = XtCreateManagedWidget(name_buf,
					xmToggleButtonGadgetClass, rc, args, n);
				if (sm->callback)
					XtAddCallback(child, XmNvalueChangedCallback,
						sm->callback, (button_count - 1));
			break;
			case XmCASCADEBUTTON:
				sprintf(name_buf,"button_%d", button_count++);
				child = XtCreateManagedWidget(name_buf,
					xmCascadeButtonGadgetClass, rc, args, n);
				if (sm->callback)
					XtAddCallback(child, XmNactivateCallback,
						sm->callback, (button_count - 1));
			break;
			default:
				/* this is an error condition */
				;
			break;
		}
	}
}

#ifdef _NO_PROTO
Widget XmCreateSimpleMenuBar(parent, name, args, arg_count)
Widget parent;
String name;
ArgList args;
Cardinal arg_count;
#else /* _NO_PROTO */
Widget XmCreateSimpleMenuBar (Widget parent, String name, ArgList args, Cardinal arg_count)
#endif /* _NO_PROTO */
{
	Widget rc;
	XmSimpleMenuRec mr;

	XtGetSubresources(parent, &mr, name, XmCSimpleMenuBar,
		SimpleMenuResources, XtNumber(SimpleMenuResources), 
		args, arg_count);

	rc = XmCreateMenuBar(parent, name, args, arg_count);

	EvaluateConvenienceStructure(rc, &mr);

	return(rc);
}

#ifdef _NO_PROTO
Widget XmCreateSimplePopupMenu(parent, name, args, arg_count)
Widget parent;
String name;
ArgList args;
Cardinal arg_count;
#else /* _NO_PROTO */
Widget XmCreateSimplePopupMenu (Widget parent, String name, ArgList args, Cardinal arg_count)
#endif /* _NO_PROTO */
{
	Widget rc;
	XmSimpleMenuRec mr;

	XtGetSubresources(parent, &mr, name, XmCSimplePopupMenu,
		SimpleMenuResources, XtNumber(SimpleMenuResources), 
		args, arg_count);

	rc = XmCreatePopupMenu(parent, name, args, arg_count);

	EvaluateConvenienceStructure(rc, &mr);

	return(rc);
}

#ifdef _NO_PROTO
Widget XmCreateSimplePulldownMenu(parent, name, args, arg_count)
Widget parent;
String name;
ArgList args;
Cardinal arg_count;
#else /* _NO_PROTO */
Widget XmCreateSimplePulldownMenu (Widget parent, String name, ArgList args, Cardinal arg_count)
#endif /* _NO_PROTO */
{
	Widget rc;
	XmSimpleMenuRec mr;
	int n, i;
	Arg local_args[3];
	WidgetList buttons;
	Cardinal num_buttons;

	XtGetSubresources(parent, &mr, name, XmCSimplePulldownMenu,
		SimpleMenuResources, XtNumber(SimpleMenuResources), 
		args, arg_count);
	
	rc = XmCreatePulldownMenu(parent, name, args, arg_count);

	EvaluateConvenienceStructure(rc, &mr);

	if (mr.post_from_button >= 0)
	{
		n = 0;
		XtSetArg(local_args[n], XtNchildren, &buttons); n++;
		XtSetArg(local_args[n], XtNnumChildren, &num_buttons); n++;
		XtGetValues(parent, local_args, n);

		if (!num_buttons)
		{
			/* error condition */
			return(rc);
		}
		else
		{
			for (i = 0; i < num_buttons; i++)
			{
				if (((XmIsCascadeButtonGadget(buttons[i])) ||
					(XmIsCascadeButton(buttons[i])))
					&&
					(i == mr.post_from_button))
					break;
			}

			if ( i < num_buttons)
			{
				n = 0;
				XtSetArg(local_args[n], XmNsubMenuId, rc); n++;
				XtSetValues(buttons[i], local_args, n);
			}
		}
	}
	return(rc);
}

#ifdef _NO_PROTO
Widget XmCreateSimpleOptionMenu(parent, name, args, arg_count)
Widget parent;
String name;
ArgList args;
Cardinal arg_count;
#else /* _NO_PROTO */
Widget XmCreateSimpleOptionMenu (Widget parent, String name, ArgList args, Cardinal arg_count)
#endif /* _NO_PROTO */
{
	Widget rc, sub_rc;
	XmSimpleMenuRec mr;
	int n, i, button_count;
	Arg local_args[5];
	WidgetList buttons;
	Cardinal num_buttons;

	XtGetSubresources(parent, &mr, name, XmCSimpleOptionMenu,
		SimpleMenuResources, XtNumber(SimpleMenuResources), 
		args, arg_count);
	
	rc = XmCreateOptionMenu(parent, name, args, arg_count);

	sub_rc = XmCreatePulldownMenu(parent, name, args, arg_count);

	EvaluateConvenienceStructure(sub_rc, &mr);

	n = 0;
	if (mr.option_label)
	{
		XtSetArg(local_args[n], XmNlabelString, mr.option_label); n++;
	}
	if (mr.option_mnemonic)
	{
		XtSetArg(local_args[n], XmNmnemonic, mr.option_mnemonic); n++;
	}
	
	XtSetArg(local_args[n], XmNsubMenuId, sub_rc); n++;
	XtSetValues(rc, local_args, n);

	if (mr.button_set >= 0)
	{
		n = 0;
		XtSetArg(local_args[n], XtNchildren, &buttons); n++;
		XtSetArg(local_args[n], XtNnumChildren, &num_buttons); n++;
		XtGetValues(sub_rc, local_args, n);

		if (!num_buttons)
		{
			/* error condition */
			return(rc);
		}
		else
		{
			button_count = 0;
			for (i = 0; i < num_buttons; i++)
			{				/* count only PushB */
				if ((XmIsPushButtonGadget(buttons[i])) ||
					(XmIsPushButton(buttons[i])))
				{
					if (button_count == mr.button_set)
						break;
					button_count++;
				}
			}

			if ( i < num_buttons)
			{
				n = 0;
				XtSetArg(local_args[n], XmNmenuHistory, buttons[i]); n++;
				XtSetValues(rc, local_args, n);
			}
		}
	}

	return(rc);
}

#ifdef _NO_PROTO
Widget XmCreateSimpleRadioBox(parent, name, args, arg_count)
Widget parent;
String name;
ArgList args;
Cardinal arg_count;
#else /* _NO_PROTO */
Widget XmCreateSimpleRadioBox (Widget parent, String name, ArgList args, Cardinal arg_count)
#endif /* _NO_PROTO */
{
	Arg local_args[5];
	Widget rc, child;
	int i, n;
	XmSimpleMenuRec mr;
	char name_buf[20];

	rc = XmCreateRadioBox(parent, name, args, arg_count);

	XtGetSubresources(parent, &mr, name, XmCSimpleRadioBox,
		SimpleMenuResources, XtNumber(SimpleMenuResources), 
		args, arg_count);

	for(i=0; i < mr.count; i++)
	{
		sprintf(name_buf,"button_%d", i);

		n = 0;
		if (mr.label_string && mr.label_string[i])
		{
			XtSetArg(local_args[n], 
				XmNlabelString, mr.label_string[i]); n++;
		}
		if (mr.button_set == i)
		{
			XtSetArg(local_args[n], XmNset, TRUE); n++;
		}
		child = XtCreateManagedWidget(name_buf, 
			xmToggleButtonGadgetClass, rc, local_args, n);
		if (mr.callback)
			XtAddCallback(child, XmNvalueChangedCallback,
				mr.callback, i);
	}
	
	return(rc);
}

#ifdef _NO_PROTO
Widget XmCreateSimpleCheckBox(parent, name, args, arg_count)
Widget parent;
String name;
ArgList args;
Cardinal arg_count;
#else /* _NO_PROTO */
Widget XmCreateSimpleCheckBox (Widget parent, String name, ArgList args, Cardinal arg_count)
#endif /* _NO_PROTO */
{
	Arg local_args[5];
	Widget rc, child;
	int i, n;
	XmSimpleMenuRec mr;
	char name_buf[20];


	rc = XmCreateRadioBox(parent, name, args, arg_count);

	n = 0;
        XtSetArg(local_args[n], XmNradioBehavior, FALSE); n++;

	XtSetValues(rc, local_args, n);
	

	XtGetSubresources(parent, &mr, name, XmCSimpleCheckBox,
		SimpleMenuResources, XtNumber(SimpleMenuResources), 
		args, arg_count);

	for(i=0; i < mr.count; i++)
	{
		sprintf(name_buf,"button_%d", i);

		n = 0;
		if (mr.label_string && mr.label_string[i])
		{
			XtSetArg(local_args[n], 
				XmNlabelString, mr.label_string[i]); n++;
		}
		child = XtCreateManagedWidget(name_buf,
			xmToggleButtonGadgetClass, rc, local_args, n);
		if (mr.callback)
			XtAddCallback(child, XmNvalueChangedCallback,
				mr.callback, i);
	}

	return(rc);
}
