#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)callbacks.c	1.11 90/08/16";
#endif /* lint */
#endif /* REV_INFO */
/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990 OPEN SOFTWARE FOUNDATION, INC.
*  ALL RIGHTS RESERVED
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
* Open Software Foundation is a trademark of The Open Software Foundation, Inc.
* OSF is a trademark of Open Software Foundation, Inc.
* OSF/Motif is a trademark of Open Software Foundation, Inc.
* Motif is a trademark of Open Software Foundation, Inc.
* X Window System is a trademark of the Massachusetts Institute of Technology
*
*******************************************************************************
******************************************************************************/

/****************************************************************************
 ****************************************************************************
 **
 **   File:     @(#)callbacks.c	1.9 - 90/08/14
 **
 **   Project:     Motif - widget examination program
 **
 **   Description: Program which shows resources of widgets
 **
 **   Author:      Pete Levine
 **
 **
 **
 **
 ****************************************************************************
 ****************************************************************************/
#define CALLBACK 1

#include "widgetView.h"
#include "widgets.h"
#include "globals.h"
#include "callbacks.h"

/***********************************************************/
void  Quit()
{
     printf("exiting...\n\r");
     fflush(stdout);
     exit(0);
}
/**********************************************************/
XtCallbackProc MenuQuit (w, client_data, call_data)
Widget   w;
caddr_t  client_data,
         call_data;
{

  exit (0);

}

/**********************************************************/
XtCallbackProc GenericCallback (w, client_data, call_data)
Widget   w;
caddr_t  client_data,
         call_data;
{

  printf ("callback activated, client data = %s\n", client_data);

  ProcessSequence (w, client_data, call_data);

}
/***********************************************************/
/* callback for a scaled text region                       */
/* this callback is invoked when the text region associated*/
/* with a scale is activated                               */
/* the reason for this region is to allow precise updating */
/* the scale.                                              */
/***********************************************************/
XtCallbackProc TextScaleUpdate (w, client_data, call_data)
Widget   w;
caddr_t  client_data,
         call_data;
{

  int i, position = -1, n = 0, scale_min, scale_max, value = 0;
  Arg    args[MAX_ARGS];
  char *normstring ;

  normstring = XmTextGetString (w);

  for (i=0; i<MAX_ARGS; i++) {
    /* go through the additional list to find the active widget position */
    if (resources->additional [i] != (MORE_INFO *) NULL) {
      if (w == (Widget) resources->additional [i]->name) {
	position = i;
	i = MAX_ARGS;
      }
    }
  }
  
  if (position < 0) {
    PrintError ("cannot find associated text widget.");
    return ((XtCallbackProc) FUNC_ERROR);
  }
  
  resources->value [position] = resources->additional [position]->value = 
    atoi (normstring);

  /* here update the scale value */
  /* must check for max and min being exceeded */
  
  n = 0;
  XtSetArg (args [n], XmNmaximum, &scale_max); n++;
  XtSetArg (args [n], XmNminimum, &scale_min); n++;
  XtGetValues (resources->widget_addr [position], args, n);
  
  value = resources->value [position];
  if (resources->value [position] > scale_max)
    value = scale_max;
  else
    if (resources->value [position] < scale_min)
      value = scale_min;
  
  XmScaleSetValue (resources->widget_addr [position], value);
  
}
/**********************************************************/
/* enter here whenever a value is being changed           */
/**********************************************************/
XtCallbackProc ValueField (w, client_data, call_data)
Widget   w;
caddr_t  client_data,
         call_data;
{


  UpdateValueField (w);

}

/**********************************************************/
UpdateValueField (w)
Widget w;
{

  int i, n = 0, cnt;
  Arg        args[MAX_ARGS];
  XmString   tcs, *PackCompoundStrings (); 
  static XmString *tmpcpd;
  char  *normstring, buff [256], *extract_normal_string ();

  if ((i = GetValuePosition (w)) < 0)
    PrintError ("error in get position.");

  switch (resources->widget_type [i]) {
    
      case SCALE:
                    /* update the scale widget */
                    n = 0;
		    XtSetArg (args [n], XmNvalue, &resources->value [i]); n++;
		    XtGetValues (resources->widget_addr [i], args, n);
		    break;
   
      case TOGGLE_BUT:
		    n = 0;
		    if (resources->value [i] == True) {
		      resources->value [i] = False;
		      tcs = XmStringLtoRCreate("False", 
					       XmSTRING_DEFAULT_CHARSET);
		      XtSetArg (args [n], XmNlabelString, tcs); n++;
		    }
		    else {
		      resources->value [i] = True;
		      tcs = XmStringLtoRCreate("True", 
					       XmSTRING_DEFAULT_CHARSET);
		      XtSetArg (args [n], XmNlabelString, tcs); n++;
		    }
		    XtSetValues (w, args, n);
		    XmStringFree (tcs);
		    break;

      case CPD_STRING:
                    n = 0;

		      /* get the string from the text widget itself */

		      normstring = XmTextGetString (w);
		      printf ("normstring = %s, %d\n", normstring,
			      strlen (normstring));
/*		
		      XtRemoveAllCallbacks (w, XmNvalueChangedCallback);
		      XmTextSetString (w, normstring);
		      XtAddCallback (w, XmNvalueChangedCallback, 
		                     ValueField, NULL);
*/
		    /* free up the ASCII name of the string representation,
		       use an ASCII representation for creating the source 
		       code*/

		    if (resources->additional [i]->name != (char *) NULL)
		      free (resources->additional [i]->name);
		    
		    /* pack string loads a new name into additional->name */

		    PackString (normstring, i);

		    /* free the allocation of the previous tcs */
		    if (resources->value [i]) {
		      printf ("i = %d, free value %ld\n", i, resources->value [i]);
		      XmStringFree ((XmString)resources->value [i]);
		    }

		    /* point to the new list , will free it above or in the
		       new widget callback */

                    tmpcpd = PackCompoundStrings (normstring, &cnt);

		    if (!strcmp(resources->type [i], "XmStringTable")) {
			printf ("we have a list of strings\n");
			printf ("count = %d\n", cnt);
			resources->value [i] = (long) tmpcpd;
		    }
		    else {
		      if (tmpcpd)
			resources->value [i] = (long) tmpcpd [0];
		      else
			resources->value [i] = (long) tmpcpd;
		    }
		    printf ("i = %d, new string alloc = %ld\n", i, resources->value [i]);

		    /* put into additional field the number of items in the
		       list */
		    resources->additional [i]->value = cnt;
		    
		    if (normstring)
		      XtFree (normstring);
		    break;

      case STRING:
                    n = 0;
		      if (resources->value [i])
			XtFree (resources->value [i]);
		      /* get a new string */
		      resources->value [i] = (long) XmTextGetString (w);
		    free (resources->additional [i]->name);
		    PackString (resources->value [i], i);
		    break;

      case CALLBACKS:
		    n = 0;
		    if (resources->value [i] == (long) NULL) {
		      resources->value [i] = (long) GenericCallback;
		      printf ("setting callback value = %ld\n", 
			      resources->value [i]);
		      tcs = XmStringLtoRCreate(" ON  ", 
					       XmSTRING_DEFAULT_CHARSET);
		      XtSetArg (args [n], XmNlabelString, tcs); n++;
		    }
		    else {
		      resources->value [i] = (long) NULL;
		      printf ("unsetting callback value = %ld\n",
			      resources->value [i]);
		      tcs = XmStringLtoRCreate(" OFF ", 
					       XmSTRING_DEFAULT_CHARSET);
		      XtSetArg (args [n], XmNlabelString, tcs); n++;
		    }
		    XtSetValues (w, args, n);
		    XmStringFree (tcs);
		    break;
      case COLOR:

		    ActiveColorPosition = i;
		    XtManageChild (ColorShell);
		    XtManageChild (ColorBoard);
		    /* set the scale and color palette */
		    n = 0;
		    XtSetArg(args [n], XmNbackground, 
			     resources->value [i]); n++;
		    XtSetArg(args[n], XmNforeground, 
			     resources->value [i]); n++;
		    XtSetValues (ColorLabel, args, n);
		    
		    n = 0;
		    XtSetArg(args[n], XmNvalue, resources->value [i]); n++;
		    XtSetValues (ColorScale, args, n);
		    
		    /* now update the list of names, if applicable */
		    
		    for (cnt = 0; cnt < colortype.num_colors; cnt++) {
		      if (colortype.color_list [cnt].pixel == 
			  resources->value [i]) {
			XmListSelectPos (ColorList, cnt + 1, False);
			XmListSetPos (ColorList, cnt + 1);
		      }
		    }
		    break;
	  default:
		    break;
  }

}
/**********************************************************/
/* update all the value fields and set 
   resources->value [i] = resources->orgvalue [i].
*/
/**********************************************************/
AutoUpdateValueField (w, value, i)
Widget w;
long value;
int i;
{

  int  n = 0, cnt;
  Arg        args[MAX_ARGS];
  XmString   tcs, *PackCompoundStrings ();
  static XmString *tmpcpd, *orgcpd; 
  char  *normstring, buff [256], *extract_normal_string ();

  switch (resources->widget_type [i]) {
    
      case SCALE:
                    /* update the scale widget */
                    n = 0;
		    XtSetArg (args [n], XmNvalue, value); n++;
		    XtSetValues (w, args, n);
		    resources->value [i] = resources->orgvalue [i] = value;
		    break;
   
      case TOGGLE_BUT:
		    n = 0;
		    if (value == True)
		      tcs = XmStringLtoRCreate("True", 
					       XmSTRING_DEFAULT_CHARSET);
		    else
		      tcs = XmStringLtoRCreate("False", 
					       XmSTRING_DEFAULT_CHARSET);
		    XtSetArg (args [n], XmNlabelString, tcs); n++;
		    XtSetValues (w, args, n);
		    XmStringFree (tcs);
		    resources->value [i] = resources->orgvalue [i] = value;
		    break;

      case CPD_STRING:

                    n = 0;
		    /* value here is a compund string */

		    if (value == (long) NULL)
		      normstring = (char *) NULL;
		    else
		      normstring = 
			extract_normal_string (value); 

		    XtRemoveAllCallbacks (w, XmNvalueChangedCallback);
		    XmTextSetString (w, normstring);
		    XtAddCallback (w, XmNvalueChangedCallback, 
				   ValueField, NULL);

/*		    if (normstring != (char *) NULL) {
*/
		      if (resources->additional [i]->name != (char *) NULL)
			free (resources->additional [i]->name);
		      PackString (normstring, i);

		      /* free the old allocation of both value and orgvalue */
		      if (resources->orgvalue [i] &&
                         resources->orgvalue [i] != resources->value [i])
                         XmStringFree ((XmString)resources->orgvalue [i]);

		      if (resources->value [i])
			XmStringFree ((XmString)resources->value [i]);

		      /* allocate two buffers, one for value and one
			 for orgvalue
		      */

		      tmpcpd = PackCompoundStrings (normstring, &cnt);
		      orgcpd = PackCompoundStrings (normstring, &cnt);

		      if (!strcmp(resources->type [i], "XmStringTable")) {
			printf ("we have a list of strings in auto...\n");
			printf ("count = %d, i = %d\n", cnt, i);
			printf ("name = %s\n", resources->additional [i]->name);
			resources->orgvalue [i] = (long) orgcpd;
			resources->value [i] = (long) tmpcpd;
		      }
		      else {
			if (normstring) {
			  resources->orgvalue [i] = (long) orgcpd [0];
			  resources->value [i] = (long) tmpcpd [0];
			}
			else {
			  resources->orgvalue [i] = (long) orgcpd;
			  resources->value [i] = (long) tmpcpd;
			}
		      }

		      /* put into additional field the number of items in the
			 list */

		      resources->additional [i]->value = cnt;
		      
		    if (normstring)
		      XtFree (normstring);
/*
		    }
*/

		    break;
		    
      case STRING:
                    n = 0;

		    XtRemoveAllCallbacks (w, XmNvalueChangedCallback);
		    XmTextSetString (w, (XmString)value);
		    XtAddCallback (w, XmNvalueChangedCallback, 
				   ValueField, NULL);

		    if (resources->additional [i]->name != (char *) NULL)
		      free (resources->additional [i]->name);
		    PackString (value, i);
		    
		    if (resources->value [i])
		      free (resources->value [i]);
		    if (resources->orgvalue [i])
		      free (resources->orgvalue [i]);
		    
		    if (value) {
		      resources->value [i] = calloc (1, strlen (value));
		      resources->orgvalue [i] = calloc (1, strlen (value));
		      strcpy (resources->value [i], value);
		      strcpy (resources->orgvalue [i], value);
		    }
		    else
		      resources->value [i] = resources->orgvalue [i] = value;

		    break;

      case CALLBACKS:
		    n = 0;
		    XtRemoveAllCallbacks (ActiveWidget.address, 
					  resources->name [i]);
		    if (value == (long) NULL)
		      tcs = XmStringLtoRCreate(" OFF ", 
					       XmSTRING_DEFAULT_CHARSET);
		    else {
		      tcs = XmStringLtoRCreate(" ON  ", 
					       XmSTRING_DEFAULT_CHARSET);
		      XtAddCallback (ActiveWidget.address, resources->name [i], 
				     value, resources->name [i]);
		    }
		    XtSetArg (args [n], XmNlabelString, tcs); n++;
		    XtSetValues (w, args, n);
		    XmStringFree (tcs);
		    resources->value [i] = resources->orgvalue [i] = value;
		    break;
      case COLOR:
		    n = 0;
		    XtSetArg(args [n], XmNbackground, 
			     value); n++;
		    XtSetArg(args[n], XmNforeground, 
			     value); n++;
		    XtSetValues (w, args, n);
		    resources->value [i] = resources->orgvalue [i] = value;
	            break;
      default:
		    break;
  }

}

/**********************************************************/
/* do the changed resources when the APPLY push button is pressed */
/**********************************************************/
XtCallbackProc ApplyResourceCallback (w, client_data, call_data)
Widget   w;
caddr_t  client_data,
         call_data;
{
  
  if (!ApplyResources ())
    return (0);

}

/**********************************************************/
ApplyResources ()
{
  int i, n = 0, argcnt = 0;
  Arg           args[MAX_ARGS];
  XmString      *tcs;

  if (ActiveWidget.address == (Widget) NULL ) {
    PrintError ("no active widget selected.");
    return (0);
  }

  XtUnmanageChild (ActiveWidget.address);

  /* set the entire resource list */
  argcnt = 0;
  for (i=0; i<resources->num_items; i++) {
    if (resources->editable [i]) {
      if ((resources->widget_type [i] == CALLBACKS) &&
	  (resources->value [i] != (long) NULL)) {
	/* kind of scwewy wittle wascal...set the arg to NULL */
	XtSetArg(args [i], resources->name [i], NULL);
      }
      else {
	if (!strcmp(resources->type [i], "XmStringTable")) {
	  printf ("IN APPLY name [%d] = %s, value = %ld\n", i,
		  resources->additional [i]->name,
		  resources->value [i]);
	  tcs = (XmString *) resources->value [i];
	  for (n=0; n<resources->additional [i]->value; n++) {
	    printf ("string [%d] = %s\n", i, extract_normal_string (tcs [n]));
	  }
	}
      }
      XtSetArg(args [argcnt], resources->name [i], resources->value [i]); argcnt++;
    }
  }

  XtSetValues (ActiveWidget.address, args, argcnt-1);

  XtManageChild (ActiveWidget.address);

  /* add callbacks where needed */

  for (i=0; i<resources->num_items; i++) {
    if (resources->widget_type [i] == CALLBACKS) {
      if (resources->value [i] != (long) NULL) {
	XtRemoveAllCallbacks (ActiveWidget.address, resources->name [i]);
	XtAddCallback (ActiveWidget.address, resources->name [i], 
		       resources->value [i], resources->name [i]);
	printf ("in call, %s, %ld\n", resources->name [i], resources->value [i]);
      }
    }
  }

}

/**********************************************************/
/* do the cancel resource when the CANCEL push button is pressed */
/**********************************************************/
XtCallbackProc CancelResource (w, client_data, call_data)
Widget   w;
caddr_t  client_data,
         call_data;
{
  int i;
  Arg           args[MAX_ARGS];

  if (ActiveWidget.address != (Widget) NULL) {

    /* reset the resources to the value when the resource wuz created */

    for (i=0; i<resources->num_items; i++) {
      AutoUpdateValueField (resources->widget_addr [i], 
			    resources->orgvalue [i],
			    i);
 /*     XtSetArg(args [i], resources->name [i], resources->orgvalue [i]);
*/
    }
/*    
    if (ActiveWidget.address != (Widget) NULL)
      XtSetValues (ActiveWidget.address, args, i-1);
*/
    ApplyResources ();
  }
  else
    PrintError ("no active widget selected.");
}

/**********************************************************/
XtCallbackProc NewFileOK (w, client_data, callback_data)
Widget   w;
char *client_data;
XmSelectionBoxCallbackStruct *callback_data;
{
  Widget CreateLabel (), GetWidgetList ();
  char *extract_normal_string ();
  int i;

  if (callback_data->value == (XmString) NULL)
    return (0);

  ActiveWidget.widget_name = extract_normal_string (callback_data->value);

  /* textstr now contains the widget selection */

  XtUnmanageChild (WidgetShell);
  XtUnmanageChild (WidgetDialog);


  ActiveWidget.address = GetWidgetList (ActiveWidget.widget_name, 
					&ActiveWidget.widget_offset,
					NULL, 0);

  /* got a new one so open it */

  if (ActiveWidget.address != (Widget) NULL)  
   /* this is where the resources struct is set */
    SetNewWidget (ActiveWidget.address);

}

/**********************************************************/
XtCallbackProc NewFile (w, client_data, call_data)
Widget   w;
caddr_t  client_data,
         call_data;
{

  int i;

  /* destroy the bulletin board in the resource edit region */

  /* here's the widget selection region */
  XtManageChild (WidgetShell);
  XtManageChild (WidgetDialog);

  /* just in case */

  XtUnmanageChild (ColorShell);
  XtUnmanageChild (ColorBoard);

  XtUnmanageChild (ObjectShell);
  XtUnmanageChild (ObjectDialog);

  XtUnmanageChild (MethodShell);
  XtUnmanageChild (MethodDialog);


  if (TBoardW != (Widget) NULL)
    XtDestroyWidget (TBoardW);
  if (ActiveWidget.address != (Widget) NULL)
    XtDestroyWidget (ActiveWidget.address);
  if (WidgetHierarchy != (Widget) NULL)
    XtDestroyWidget (WidgetHierarchy);
  for (i=0; i < MAX_CLASSES; i++)
    if (HierarchyLabels [i] != (Widget) NULL) {
      XtDestroyWidget (HierarchyLabels [i]);
      HierarchyLabels [i] = (Widget) NULL;
    }
  
  /* NULL the things to be sure */

  TBoardW = ActiveWidget.address = WidgetHierarchy = (Widget) NULL;
  if (resources != (RESOURCEVALS *) NULL) {
    for (i=0; i<resources->num_items; i++) {

      /* free up any compound string allocations */

      if (resources->widget_type [i] == CPD_STRING &&
	  resources->value [i])
	XmStringFree ((XmString)resources->value [i]);

      /* free up any normal string allocations */

      if (resources->widget_type [i] == STRING &&
	  resources->value [i])
	XtFree (resources->value [i]);

      free (resources->name [i]);
      free (resources->type [i]);
    }
  }
}

/**********************************************************/
XtCallbackProc OpenResourceFileOK (w, client_data, callback_data)
Widget   w;
char *client_data;
XmSelectionBoxCallbackStruct *callback_data;
{
  char *textstr;

  if (callback_data->value == (XmString) NULL)
    return (0);

  if (((textstr = extract_normal_string (callback_data->value)) == 
      (char *) NULL) || (!strlen(textstr)))
    return (0);

  if (DisplayResourceFile (textstr) == FUNC_ERROR) {
    free (textstr);
    return (0);
  }

  free (textstr);

}

/**********************************************************/
DisplayResourceFile (filename)
char *filename;
{

  Widget CreateLabel (), GetWidgetList ();
  char *buffer, *GetSource (), *GetWidgetNameFromBuffer (),
    *UnconvertName ();
  int i, error_flag = 0;
  char buff [256];

  XtUnmanageChild (FileDialog);
  XtUnmanageChild (FileSelection);

  if ((buffer = GetSource (filename)) == (char *) NULL)
    return (FUNC_ERROR);

  ActiveWidget.widget_name = UnconvertName (GetWidgetNameFromBuffer (buffer));

  ActiveWidget.address = GetWidgetList (ActiveWidget.widget_name, 
					&ActiveWidget.widget_offset,
					NULL, 0);

 
  if (ActiveWidget.address != (Widget) NULL)  {
    /* this is where the resources struct is set */
    SetNewWidget (ActiveWidget.address);
    /* now set the reources of the widget */
    if (SetResources (buffer) == FUNC_ERROR)
      error_flag = 1;
  }
  else {
    sprintf (buff, "File %s recovered an unrecognizeable widget name [%s]\n",
	    filename, ActiveWidget.widget_name);
    PrintError (buff);
    error_flag = 1;
  }

  free (buffer);

  if (error_flag) {
    sprintf (buff, "Error in file format [%s]...Unable to load\n", filename);
    PrintError (buff);

    /* reset everything, just like we pressed new selection */

    if (TBoardW != (Widget) NULL)
      XtDestroyWidget (TBoardW);
    if (ActiveWidget.address != (Widget) NULL)
      XtDestroyWidget (ActiveWidget.address);
    if (WidgetHierarchy != (Widget) NULL)
      XtDestroyWidget (WidgetHierarchy);
    for (i=0; i < MAX_CLASSES; i++)
      if (HierarchyLabels [i] != (Widget) NULL) {
	XtDestroyWidget (HierarchyLabels [i]);
	HierarchyLabels [i] = (Widget) NULL;
      }

    /* NULL the things to be sure */
    
    TBoardW = ActiveWidget.address = WidgetHierarchy = (Widget) NULL;
    
/*   if (ActiveWidget.widget_name)
      free (ActiveWidget.widget_name);
    ActiveWidget.widget_name = (char *) NULL;
*/
    return (FUNC_ERROR);
  }
  else
    return (FUNC_OK);
}

/**********************************************************/
XtCallbackProc OpenResourceFile (w, client_data, call_data)
Widget   w;
caddr_t  client_data,
         call_data;
{

  int i;

  XtManageChild (FileDialog);
  XtManageChild (FileSelection);
  XtRemoveAllCallbacks (FileSelection, XmNokCallback);
  XtAddCallback (FileSelection, XmNokCallback, OpenResourceFileOK, NULL);

  if (TBoardW != (Widget) NULL)
    XtDestroyWidget (TBoardW);
  if (ActiveWidget.address != (Widget) NULL)
    XtDestroyWidget (ActiveWidget.address);
  if (WidgetHierarchy != (Widget) NULL)
    XtDestroyWidget (WidgetHierarchy);
  for (i=0; i < MAX_CLASSES; i++)
    if (HierarchyLabels [i] != (Widget) NULL) {
      XtDestroyWidget (HierarchyLabels [i]);
      HierarchyLabels [i] = (Widget) NULL;
    }

  /* NULL the things to be sure */

  TBoardW = ActiveWidget.address = WidgetHierarchy = (Widget) NULL;

  if (resources != (RESOURCEVALS *) NULL) {
    for (i=0; i<resources->num_items; i++) {
      free (resources->name [i]);
      free (resources->type [i]);
    }
  }
}
/**********************************************************/
XtCallbackProc SingleSelectColor (w, client_data, callback_data)
Widget   w;
char *client_data;
XmListCallbackStruct *callback_data;
{

  Arg           args[MAX_ARGS];
  int           value, n = 0, i;
  XmString tcs;

  /* want to update the color region with the new color value */

  value = callback_data->item_position - 1;
  
  n = 0;
  XtSetArg(args [n], XmNbackground, colortype.color_list [value].pixel); n++;
  XtSetArg(args[n], XmNforeground, colortype.color_list [value].pixel); n++;
  XtSetValues (ColorLabel, args, n);

  n = 0;
  XtSetArg(args[n], XmNvalue, colortype.color_list [value].pixel); n++;
  XtSetValues (ColorScale, args, n);

}

/**********************************************************/
XtCallbackProc NewColorOK (w, client_data, callback_data)
Widget   w;
char *client_data;
XmListCallbackStruct *callback_data;
{

  Arg           args[MAX_ARGS];
  char *active_color, *extract_normal_string ();
  int i, list_position, n;

  if (callback_data->item == (XmString) NULL)
    return (0);

  active_color = extract_normal_string (callback_data->item);

  /* textstr now contains the widget selection */

  list_position = callback_data->item_position - 1;

  i = ActiveColorPosition;

  resources->value [i] = colortype.color_list [list_position].pixel; 
/*  sprintf (buff, "%s, [Pixel = %d]",
	   colortype.color_list [list_position].name,
	   colortype.color_list [list_position].pixel);
  tcs = XmStringLtoRCreate (buff, XmSTRING_DEFAULT_CHARSET);
*/  
  n = 0;
  XtSetArg(args [n], XmNbackground, colortype.color_list [list_position].pixel);
  n++;
  XtSetArg(args[n], XmNforeground, colortype.color_list [list_position].pixel);
  n++;
  XtSetValues (resources->widget_addr [i], args, n);

  /* save the new name and value */

  resources->additional [i]->name = colortype.color_list [list_position]. name;
  resources->additional [i]->value = 
    (long) colortype.color_list [list_position].pixel;

  XtUnmanageChild (ColorShell);
  XtUnmanageChild (ColorBoard);

}

/**********************************************************/
XtCallbackProc ColorUpdate (w, client_data, callback_data)
Widget   w;
char *client_data;
XmSelectionBoxCallbackStruct *callback_data;
{

  Arg           args[MAX_ARGS];
  int           value, n = 0, i;
  XmString tcs;
  char buff [256];

  XmScaleGetValue (w, &value);

  n = 0;
  for (i = 0; i < colortype.num_colors; i++) {
    if (colortype.color_list [i].pixel == value) {
      XmListSelectPos (ColorList, i + 1, False);
      XmListSetPos (ColorList, i + 1);
/*
      XmListSetItem (ColorList, tcs);
      XmListSetBottomItem (ColorList, tcs);
*/
    }
  }
     
  n = 0;
  XtSetArg(args [n], XmNbackground, value); n++;
  XtSetArg(args[n], XmNforeground, value); n++;
  XtSetValues (ColorLabel, args, n);

}

/**********************************************************/
XtCallbackProc ColorLabelPressed (w, client_data, callback_data)
Widget   w;
char *client_data;
XmSelectionBoxCallbackStruct *callback_data;
{

  Arg           args[MAX_ARGS];
  int           value, n = 0, i, color_found = 0, list_position;
  XmString tcs;
  char buff [256];

  XmScaleGetValue (ColorScale, &value);

  n = 0;
  for (i = 0; i < colortype.num_colors; i++) {
    if (colortype.color_list [i].pixel == value) {
      /* here the color selected is in the list */
      color_found = 1;
      list_position = i;
      i = colortype.num_colors;
    }
  }
  i = ActiveColorPosition;

  resources->value [i] = value; 

  n = 0;
  XtSetArg(args [n], XmNbackground, value);
  n++;
  XtSetArg(args[n], XmNforeground, value);
  n++;
  XtSetValues (resources->widget_addr [i], args, n);

  /* save the new name and value */
  resources->additional [i]->value = value;

  if (color_found)
    resources->additional [i]->name = 
      colortype.color_list [list_position]. name;
  else
    resources->additional [i]->name = NO_COLOR_NAME;
/*
  XtUnmanageChild (ColorShell);
  XtUnmanageChild (ColorBoard);
*/
}

/*********************************************************/
XtCallbackProc SaveResourceFile (w, client_data, call_data)
Widget   w;
caddr_t  client_data,
         call_data;
{

  Arg           args[MAX_ARGS];
  XtCallbackProc SaveResourceFileOK ();

  if (ActiveWidget.address != (Widget) NULL ) {
    XtManageChild (FileDialog);
    XtManageChild (FileSelection);
    XtRemoveAllCallbacks (FileSelection, XmNokCallback);
    XtAddCallback (FileSelection, XmNokCallback, SaveResourceFileOK, NULL);
  }
  else
    PrintError ("no active widget selected.");

}

/**********************************************************/
XtCallbackProc SaveResourceFileOK (w, client_data, callback_data)
Widget   w;
char *client_data;
XmSelectionBoxCallbackStruct *callback_data;
{
  static char *textstr;
  char *extract_normal_string ();

  if (callback_data->value == (XmString) NULL)
    return (0);

  textstr = extract_normal_string (callback_data->value);
  
  if (!strlen (textstr))
    return (0);

  /* write to the file */

  SaveResources (textstr);


  XtUnmanageChild (FileDialog);
  XtUnmanageChild (FileSelection);

}
/**********************************************************/
SaveResources (textstr)
char *textstr;
{

  char  *ConvertName ();
  int i;
  FILE *fp;
  char buff [256];
  XtCallbackProc QuestionDialogSaveOK ();

  if ((fp = fopen (textstr, "r+")) == NULL) {
    if ((fp = fopen (textstr, "a+")) == NULL) {
      sprintf (buff, "error creating or opening %s\n", textstr);
      PrintError (buff);
      return (0);
    }
    fprintf (fp, "WIDGET NAME: %s\n", ConvertName (ActiveWidget.widget_name));
    for (i=0; i<resources->num_items; i++) {
      if (resources->editable [i] == True) {
	if (resources->widget_type [i] == COLOR
	    || resources->widget_type [i] == CPD_STRING
	    || resources->widget_type [i] == STRING) {
	  
	  if ((resources->widget_type [i] == COLOR) &&
	      !strcmp (resources->additional [i]->name, NO_COLOR_NAME))
	    fprintf (fp, "%s\t%s\t%ld\n", 
		     resources->name [i], 
		     resources->type [i], 
		     resources->value [i]);
	  else
	    fprintf (fp, "%s\t%s\t%s\n", 
		     resources->name [i], 
		     resources->type [i],
		     resources->additional [i]->name);
	}
	else
	  fprintf (fp, "%s\t%s\t%ld\n", 
		   resources->name [i],
		   resources->type [i],
		   resources->value [i]);
	
      }
    }
    fclose (fp);
  }   /* file exists */
  else {
    sprintf (buff, "File %s exists.\nOverwrite ?", textstr);
    QuestionMsg (buff);
    XtRemoveAllCallbacks (QuestionDialog, XmNokCallback);
    XtAddCallback(QuestionDialog, XmNokCallback, QuestionDialogSaveOK, textstr);
  }
}

/**********************************************************/
/* Callback invoked when the save C file button is pressed
   called from the cascade button of the file pulldown menu.
*/
/**********************************************************/
XtCallbackProc SaveCFile (w, client_data, callback_data)
Widget   w;
char *client_data;
XmSelectionBoxCallbackStruct *callback_data;
{
  char *textstr, *extract_normal_string ();

  if (callback_data->value == (XmString) NULL)
    return (0);

  textstr = extract_normal_string (callback_data->value);

  if (!strcmp (client_data, C_CODE_WITH_TEXT))
    SaveFileOK (C_CODE_WITH_TEXT, C_TEMPLATE_WITH_TEXT_FILE, textstr);
  else
    SaveFileOK (C_CODE, C_TEMPLATE_FILE, textstr);

}
/**********************************************************/
/* Callback invoked when the save uil button is pressed
   called from the cascade button of the file pulldown menu.
*/
/**********************************************************/
XtCallbackProc SaveUILFile (w, client_data, callback_data)
Widget   w;
char *client_data;
XmSelectionBoxCallbackStruct *callback_data;
{
  char *textstr, *extract_normal_string ();

  if (callback_data->value == (XmString) NULL)
    return (0);

  textstr = extract_normal_string (callback_data->value);

  if (!strcmp (client_data, UIL_CODE_WITH_TEXT))
    SaveFileOK (UIL_CODE_WITH_TEXT, UIL_TEMPLATE_WITH_TEXT_FILE, textstr);
  else
    SaveFileOK (UIL_CODE, UIL_TEMPLATE_FILE, textstr);

}

/**********************************************************/
/* callback when the ok is pressed on the message input shell
*/
/**********************************************************/
XtCallbackProc TextOKFunction (w, client_data, call_data)
Widget   w;
caddr_t  client_data,
         call_data;
{
  if (ActiveWidget.address != (Widget) NULL ) {

    /* if we've loaded earlier, get rid of it */
    
    if (ActiveWidget.message_buffer != (char *) NULL)
      XtFree (ActiveWidget.message_buffer);
    
    /* get the string from the text widget */

    ActiveWidget.message_buffer = XmTextGetString (TextDialog);

    /* now put away the text window and put up the file dialog */
    
    XtUnmanageChild (TextShell);
    XtUnmanageChild (TextBulletin); 

    XtManageChild (PrintFileDialog);
    XtManageChild (PrintFileSelection);
    XtRemoveAllCallbacks (PrintFileSelection, XmNokCallback);
    if (!strcmp (client_data, C_CODE))
      XtAddCallback (PrintFileSelection, XmNokCallback, 
		     SaveCFile, C_CODE_WITH_TEXT);
    else
      if (!strcmp (client_data, UIL_CODE))
	XtAddCallback (PrintFileSelection, XmNokCallback, 
		       SaveUILFile, UIL_CODE_WITH_TEXT);
  }
  else
    PrintError ("no active widget selected.");

}

/**********************************************************/
/* callback when the Cancel is pressed on the message input shell
*/
/**********************************************************/
XtCallbackProc TextCancelFunction (w, client_data, call_data)
Widget   w;
caddr_t  client_data,
         call_data;
{

  if (ActiveWidget.address != (Widget) NULL ) {

    /* if we've loaded earlier, get rid of it */
    
    if (ActiveWidget.message_buffer != (char *) NULL)
      XtFree (ActiveWidget.message_buffer);
    
    /* NULL out the string */

    ActiveWidget.message_buffer = (char *) NULL;

    /* now put away the text window and get out */
    
    XtUnmanageChild (TextShell);
    XtUnmanageChild (TextBulletin); 
  }
  else
    PrintError ("no active widget selected.");

}

/**********************************************************/
/* callback when the Clear is pressed on the message input shell
   clear the buffer and replace into the text widget
*/
/**********************************************************/
XtCallbackProc TextClearFunction (w, client_data, call_data)
Widget   w;
caddr_t  client_data,
         call_data;
{
  int i, n = 0;
  Arg           args[MAX_ARGS];

  if (ActiveWidget.address != (Widget) NULL ) {
    
    /* if we've loaded earlier, get rid of it */
    
    if (ActiveWidget.message_buffer != (char *) NULL)
      XtFree (ActiveWidget.message_buffer);
    
    /* NULL out the active string */
    
    ActiveWidget.message_buffer = (char *) NULL;

    /* now put in the NULL string */

    XmTextSetString (TextDialog, ActiveWidget.message_buffer);

  }
  else
    PrintError ("no active widget selected.");

}

/**********************************************************/
XtCallbackProc WriteSourceCode (w, client_data, call_data)
Widget   w;
caddr_t  client_data,
         call_data;
{
  if (ActiveWidget.address != (Widget) NULL ) {
    if ((!strcmp (client_data, C_CODE)) ||
	(!strcmp (client_data, UIL_CODE) && !generate_message)) {
      XtManageChild (PrintFileDialog);
      XtManageChild (PrintFileSelection);
      XtRemoveAllCallbacks (PrintFileSelection, XmNokCallback);
      if (!strcmp (client_data, C_CODE))
	XtAddCallback (PrintFileSelection, XmNokCallback, 
		       SaveCFile, C_CODE);
      else
	if (!strcmp (client_data, UIL_CODE))
	  XtAddCallback (PrintFileSelection, XmNokCallback, 
			 SaveUILFile, UIL_CODE);
    }
    else {
      if (!strcmp (client_data, UIL_CODE) && generate_message) {	
	XtManageChild (TextShell);
	XtManageChild (TextBulletin); 
	XtRemoveAllCallbacks (TextOKButton, XmNactivateCallback);
	if (!strcmp (client_data, C_CODE))
	  XtAddCallback (TextOKButton, XmNactivateCallback, 
			 TextOKFunction, C_CODE);
	else
	  if (!strcmp (client_data, UIL_CODE))
	    XtAddCallback (TextOKButton, XmNactivateCallback, 
			   TextOKFunction, UIL_CODE);
      }
    }
  }
  else
    PrintError ("no active widget selected.");

}

/**********************************************************/
XtCallbackProc PrintToFile (w, client_data, call_data)
Widget   w;
caddr_t  client_data,
         call_data;
{

  Arg           args[MAX_ARGS];
  XtCallbackProc PrintToFileOK ();

  if (ActiveWidget.address != (Widget) NULL ) {
    XtManageChild (PrintFileDialog);
    XtManageChild (PrintFileSelection);
    XtRemoveAllCallbacks (PrintFileSelection, XmNokCallback);
    XtAddCallback (PrintFileSelection, XmNokCallback, PrintToFileOK, NULL);
  }
  else
    PrintError ("no active widget selected.");
  
}

/**********************************************************/
XtCallbackProc PrintToFileOK (w, client_data, callback_data)
Widget   w;
char *client_data;
XmSelectionBoxCallbackStruct *callback_data;
{
  char *textstr, *extract_normal_string (), *ConvertName ();
  int i;
  FILE *fp;
  char buff [256];

  if (callback_data->value == (XmString) NULL)
    return (0);

  textstr = extract_normal_string (callback_data->value);

  /* write to the file */

  if ((fp = fopen (textstr, "r+")) == NULL) {
    if ((fp = fopen (textstr, "a+")) == NULL) {
      printf (buff, "error creating or opening %s\n", textstr);
      PrintError (buff);
      return (0);
    }
    fprintf (fp, "WIDGET NAME: %s\n", ConvertName (ActiveWidget.widget_name));
    for (i=0; i<resources->num_items; i++) {
      if (resources->widget_type [i] == COLOR
	  || resources->widget_type [i] == CPD_STRING
	  || resources->widget_type [i] == STRING)
	fprintf (fp, "%-20s%-20s%-20s%-5ld [%s]\n", resources->class [i], 
		 resources->name [i], 
		 resources->type[i], resources->value [i], 
		 resources->additional [i]->name);
      else
	fprintf (fp, "%-20s%-20s%-20s%-5ld\n", resources->class [i], 
		 resources->name [i], 
		 resources->type[i], resources->value [i]);
    }
    fclose (fp);
  }
  else
    PrintError ("File Exists.");
    
  XtUnmanageChild (PrintFileDialog);
  XtUnmanageChild (PrintFileSelection);

}

/**********************************************************/
XtCallbackProc PrintToFileApply (w, client_data, call_data)
Widget   w;
caddr_t  client_data,
         call_data;
{


}


/**********************************************************/
XtCallbackProc PrintToPrinter (w, client_data, call_data)
Widget   w;
caddr_t  client_data,
         call_data;
{


  PrintError ("printout selected [FUNCTION UNIPLEMENTED.]");

}

/**********************************************************/
XtCallbackProc Enabled (w, client_data, call_data)
Widget   w;
caddr_t  client_data,
         call_data;
{

  PrintError ("save file selected [FUNCTION UNIMPLEMENTED.]");

}

/**********************************************************/
XtCallbackProc GenerateMessageToggle (w, client_data, call_data)
Widget   w;
caddr_t  client_data,
         call_data;
{
    register int  n = 0;
    Arg           args[MAX_ARGS];
    XmString tcs;

    generate_message = !generate_message;

    if (generate_message) 
      tcs = XmStringLtoRCreate("Create Message [ON] ", 
			       XmSTRING_DEFAULT_CHARSET);
    else
      tcs = XmStringLtoRCreate("Create Messgae [OFF]", 
			       XmSTRING_DEFAULT_CHARSET);
    XtSetArg (args [n], XmNlabelString, tcs); n++;
    XtSetValues (w, args, n);
    XmStringFree (tcs);


}

/**********************************************************/
XtCallbackProc ManHelp (w, client_data, call_data)
Widget   w;
caddr_t  client_data,
         call_data;
{

/*
  system ("xterm -e man label &");
*/
  PrintError ("manual selected [FUNCTION UNIMPLEMENTED.]");

}

/**********************************************************/
XtCallbackProc SourceHelp (w, client_data, call_data)
Widget   w;
caddr_t  client_data,
         call_data;
{

  PrintError ("source help selected. [FUNCTION UNIMPLEMENTED.]");

}

/**********************************************************/
XtCallbackProc ViewHelp (w, client_data, call_data)
Widget   w;
caddr_t  client_data,
         call_data;
{

  PrintError ("view help selected. [FUNCTION UNIMPLEMENTED.]");

}

/**********************************************************/
XtCallbackProc SelectObjects (w, client_data, call_data)
Widget   w;
caddr_t  client_data,
         call_data;
{

  /* here's the object selection region */
  XtManageChild (ObjectShell);
  XtManageChild (ObjectDialog);


}

/**********************************************************/
XtCallbackProc SelectMethods (w, client_data, call_data)
Widget   w;
caddr_t  client_data,
         call_data;
{

  /* here's the method selection region */
  XtManageChild (MethodShell);
  XtManageChild (MethodDialog);


}

/**********************************************************/
XtCallbackProc ErrorDialogAway (w, client_data, call_data)
Widget   w;
caddr_t  client_data,
         call_data;
{

  /* put away the error dialog */
  XtUnmanageChild (ErrorDialog);

}

/**********************************************************/
XtCallbackProc QuestionDialogAway (w, client_data, call_data)
Widget   w;
caddr_t  client_data,
         call_data;
{

  /* put away the question dialog */
  printf ("in question cancel\n");
  XtUnmanageChild (QuestionDialog);

}

/**********************************************************/
XtCallbackProc QuestionDialogSaveOK (w, client_data, call_data)
Widget   w;
caddr_t  client_data,
         call_data;
{

  char buff [256];

  /* we are entering with the notion of overwriting the
     existing file (client_data)
  */

  /* put away the question dialog */
  printf ("in question ok, str = %s\n", client_data);
  XtUnmanageChild (QuestionDialog);


  /* remove the file */
  sprintf (buff, "rm %s", client_data);
  system (buff);


  /* now re-save it */
  SaveResources (client_data);

}

/**********************************************************/
XtCallbackProc QuestionDialogOK (w, client_data, call_data)
Widget   w;
caddr_t  client_data,
         call_data;
{

  static char *file_type, *template_name, *file_name;
  char buff [256];

  /* here we want to erase the old file
     and save a new one by the same name */

 /* the string coming in is arranged:
     file_type, template_name, new_file
     
     so...we need to parse the string and recall the create
     routine...

 */

  file_type = strtok (client_data, ",");
  template_name = strtok (NULL, ",");
  file_name = strtok (NULL, ",");


  /* put away the question dialog */
  printf ("in question ok, type = %s, name = %s, file = %s\n",
	  file_type, template_name, file_name);
  XtUnmanageChild (QuestionDialog);

  /* remove the file */
  sprintf (buff, "rm %s", file_name);
  system (buff);


  /* now re-save it */
  SaveFileOK (file_type, template_name, file_name);


}






