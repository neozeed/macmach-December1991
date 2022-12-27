#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)sequence.c	1.5 90/08/01";
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
 **   File:     @(#)sequence.c	1.5 - 90/08/01
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
#define SEQUENCE 1

#include "widgetView.h"
#include "widgets.h"
#include "globals.h"
#include "callbacks.h"

#define LIST_HAS_NO_ELEMENTS -2

typedef int (*FUNCTION) ();

typedef struct _callback {
  char callback_name [50];
  FUNCTION callback_ptr;
}FUNC_CALLBACK;

typedef struct _method {
  char method_name [50];
  int num_callbacks;
  FUNC_CALLBACK **callback_list;
  struct _method *last, *next;
}METHOD;

typedef struct _action {
  char action_name [50];
  char mapping_name [50];
  int num_methods;
  METHOD *action_methods;
  struct _action *last, *next;
}ACTION;

typedef struct _widgetaction {
  char widget_name [50];
  ACTION *action_list;
  struct _widgetaction *last, *next;
}WIDGET_ACTION;


WIDGET_ACTION *widget_action_depend = (WIDGET_ACTION *) NULL;
METHOD *method_list = (METHOD *) NULL;


/**********************************************************/
/* main is commented out...used for development as
   a stand alone program 
*/
/**********************************************************/
/* main ()
 
{
  Widget widget;
  int i = 0, num_widgets, num_actions, num_methods, num_functions, x, z, y;
  char **widget_objects, **GetAllWidgets ();
  char **action_objects, **GetAllActions ();
  char **method_objects, **GetImbededMethods ();
  char **function_objects, **GetImbededFunctions ();
  
  
  CreateActionElements ();

  PerformActionObject (widget, "activateCallback");
  PerformActionObject (widget, "armCallback");
  PerformActionObject (widget, "disarmCallback");

  widget_objects = GetAllWidgets (&num_widgets);
  for (i=0; i<num_widgets; i++) {
    action_objects = GetAllActions (widget_objects [i], &num_actions);
    printf ("WIDGET = %s\n", widget_objects [i]);
    for (z=0; z<num_actions; z++) {
      method_objects = GetImbededMethods (widget_objects [i], 
					  action_objects [z],
					  &num_methods);
      printf ("    ACTION = %s\n", action_objects [z]);
      for (y=0; y<num_methods; y++) {
	function_objects = GetImbededFunctions (widget_objects [i], 
						action_objects [z],
						method_objects [y],
						&num_functions);
	printf ("        METHOD = %s\n", method_objects [y]);
	for (x=0; x<num_functions; x++) {
	  printf ("          FUNCTION = %s\n", function_objects [x]);
	}
      }
    }
  }
}
*/
/**********************************************************/
/*  routine called from the widget itself (entry point)   */
/**********************************************************/
ProcessSequence (w, client_data, call_data)
Widget   w;
caddr_t  client_data,
         call_data;
{

  printf ("in process sequence = %s\n", client_data);

  PerformActionObject (w, client_data);

  
}
/**********************************************************/
/* traverses the action items previously specified for
   the callback name
*/
/**********************************************************/
PerformActionObject (widget, motif_callback_name)
Widget widget;
char *motif_callback_name;
{

  char *widget_handle, *GetWidgetHandle ();
  char *action_name, *GetActionMapping ();

/* want to traverse linked list of actions and
   then go down the method tree and do the callback
   list */

  if (!(widget_handle = GetWidgetHandle ("DEFAULT")))
    return (FUNC_ERROR);

  /* find the action mapping item to our stuff */

  if (!(action_name = GetActionMapping (widget_handle, motif_callback_name)))
    return (FUNC_ERROR);


  /* now we do the thing.  based on an action,
     invoke the method bound with each 
     action */

  printf ("handle = %s, action = %s\n", widget_handle, action_name);
  if (InvokeAction (widget, widget_handle, action_name) == LIST_HAS_NO_ELEMENTS)
    PrintError ("No items in method list.");

  /* and it works !!! */

}
/**********************************************************/
/*  this is the callback function we are using right now. */
/*  could be anything though...                           */
/**********************************************************/
print_something (widget, name)
Widget widget;
char *name;
{

  printf ("in callback print_something = %s\n", name);
  

}

/**********************************************************/
/* show the active widget cascade across the area         */
/**********************************************************/
change_position (widget, name)
Widget widget;
char *name;

{
  int n = 0;
  Arg    args[MAX_ARGS];
  int i;

  n = 0;
  XtSetArg (args [n], XmNx, 100); n++;
  XtSetArg (args [n], XmNy, 100); n++;
  XtSetValues (widget, args, n);

  printf ("in callback change_position = %s\n", name);

}

/**********************************************************/
/* called by widgetView (main) to set up the action elements
   and all the data lists associated with the objects
*/
/**********************************************************/
CreateActionElements ()
{
  
  char *action_handle, *act1;
  char *CreateActionList (), *AddActionElement (),
       *CreateMethod (), *AddMethod ();
  FUNC_CALLBACK **arry;
  int i;


  
  /* create an action list with a widget name "DEFAULT"
     this will be the default case.  all widgets
     will process action of the "DEFAULT" named widget reference */
  action_handle = CreateActionList ("DEFAULT");
  
  /* add elements to the "DEFAULT" named widget.
     add three actions, PUSH, RELEASE, ACTIVATE and
     bind them to real callback names */

  AddActionElement (action_handle, "PUSH", "activateCallback");
  AddActionElement (action_handle, "RELEASE", "disarmCallback");
  AddActionElement (action_handle, "ACTIVATE", "armCallback");


  /* now create the methods list */

  CreateMethod ("MOVE");
  CreateMethod ("RESIZE");
  CreateMethod ("COLOR");
  CreateMethod ("BORDER");

  arry = (FUNC_CALLBACK **) calloc (10, sizeof (FUNC_CALLBACK));

  /* point the array at the method to be performed 
     could be set to anything !
  */

  for (i=0; i<4; i++) {
    arry [i] = (FUNC_CALLBACK *) calloc (1, sizeof (FUNC_CALLBACK));
    arry [i]->callback_ptr = print_something;
    strcpy (arry [i]->callback_name, "print_something");
  }

  /* here's some real functionality */
  arry [1]->callback_ptr = change_position;
  strcpy (arry [1]->callback_name, "change_position");
  /* add "real" functionality to the method 
     in this case simply attach a print routine to the
     method names */

  AddMethodFunction ("MOVE", arry, 4);

  /* reset the function to check that move really "moves it" */
  arry [1]->callback_ptr = print_something;
  strcpy (arry [1]->callback_name, "print_something");
  AddMethodFunction ("RESIZE", arry, 2);
  AddMethodFunction ("COLOR", arry, 3);

  
  /* now give actions methods.  one action item may have
     multiple methods.  in this case the "PUSH" event
     will cause a move, resize and color function to
     happen */
  /* add move to the push */
  AddMethod (action_handle, "PUSH", "MOVE");

  /* add resize to the activate action */
  AddMethod (action_handle, "ACTIVATE", "RESIZE");
  AddMethod (action_handle, "ACTIVATE", "MOVE");

  /* add color to the release */
  AddMethod (action_handle, "RELEASE", "COLOR");
  AddMethod (action_handle, "RELEASE", "RESIZE");

  

  /* this just prints out the internals */
/*
  commented out 
  PrintImbededMethods (action_handle, "PUSH");
  PrintImbededMethods (action_handle, "ACTIVATE");
*/

}

/**********************************************************/
/* create a handle to an action list 
   its on a per widget basis, unless NULL is used, which will
   work for every widget */
/**********************************************************/
char *CreateActionList (widget_name)
char *widget_name;
{

  static WIDGET_ACTION *w;
  WIDGET_ACTION *CheckWidgetName ();

  w = (WIDGET_ACTION *) calloc (1, sizeof (WIDGET_ACTION));
  if (!widget_name)
    strcpy (w->widget_name, "DEFAULT");
  else
    strcpy (w->widget_name, widget_name);
  
  /* always put the next to NULL and sction list to NULL */

  w->next = (WIDGET_ACTION *) NULL;
  w->action_list = (ACTION *) NULL;

  if (widget_action_depend == (WIDGET_ACTION*) NULL) {  /* first time */
    w->last = (WIDGET_ACTION *) NULL;
  }
  else {    /* its a widget to be added */
    if (CheckWidgetName (widget_name)) {
      /* found the name in the list already */
      /* so error */
      free (w);
      return ((char *) NULL);
    }

    /* bring the list to the last known element, just in case */
    while (widget_action_depend->next != NULL)
      widget_action_depend = widget_action_depend->next;

    w->last = widget_action_depend;       /* register the last */
    widget_action_depend->next = w;       /* register previous' next element */
  }
  widget_action_depend = w;               /* update the active ptr */
  return ((char *) widget_action_depend->widget_name);
}
/**********************************************************/
/* add a new action element to the list
*/
/**********************************************************/
char *AddActionElement (widget_handle, action_name, callback_mapping_name)
char *widget_handle, *action_name, *callback_mapping_name;
{

  static ACTION *a;
  WIDGET_ACTION *w, *CheckWidgetHandle ();
  ACTION *CheckActionName ();
  if (!widget_handle || !action_name || !callback_mapping_name)
    return ((char *) NULL);

  if ((w = CheckWidgetName (widget_handle)) == (WIDGET_ACTION*) NULL)
    return ((char *) NULL);
  
  /* if we get here there is, at least, an widget handle
     which has previously been created */

  a = (ACTION *) calloc (1, sizeof (ACTION));

  strcpy (a->action_name, action_name);
  strcpy (a->mapping_name, callback_mapping_name);
  a->num_methods = 0;
  a->action_methods = (METHOD *) NULL;
  a->next = (ACTION *) NULL;

  if (w->action_list == (ACTION *) NULL) {   /* first allocation */
    a->last = (ACTION *) NULL;
  }
  else {     /* add this to the list */
    /* let's check to see if the action element exists already */
    if (CheckActionName (w, action_name)) {
      /* yup, it does...exit error */
      free (a);
      return ((char *) NULL);
    }
    /* bring the list to the last known element, just in case */
    while (w->action_list->next != NULL)
      w->action_list = w->action_list->next;
    a->last = w->action_list;         /* register the last */
    w->action_list->next = a;            /* register previous' next element */
  }
  w->action_list = a;                   /* update the real action list with
					   the current allocation */
  return ((char *) w->action_list->action_name);

}
/**********************************************************/
/* add a method to an action item                         */
/**********************************************************/
char *AddMethod (widget_handle, action_name, method_name)
char *widget_handle, *action_name, *method_name;
{
  WIDGET_ACTION *w, *CheckWidgetHandle ();
  ACTION *CheckActionName ();
  METHOD *m, *CheckMethodName ();
  static METHOD *new_method;
  int i;

  if (!widget_handle || !action_name || !method_name)
    return ((char *) NULL);

  if ((w = CheckWidgetName (widget_handle)) == (WIDGET_ACTION*) NULL)
    /* widget name not found */
    return ((char *) NULL);

  if ((w->action_list = CheckActionName (w, action_name)) == (ACTION*) NULL)
    /* action name not found */
    return ((char *) NULL);

  if ((m = CheckMethodName (method_list, method_name)) == (METHOD *) NULL)
    /* method not found */
    return ((char *) NULL);

  printf ("adding to name %s...\n", w->action_list->action_name);
  
  w->action_list->num_methods ++;
  
  /* ok here we are going to take the method from the method list 
     and make our own list with our own head and tail */

  new_method = (METHOD*) calloc (1, sizeof (METHOD));

  /* set up the new method by copying from the existing method */
  strcpy (new_method->method_name, m->method_name);
  new_method->num_callbacks =  m->num_callbacks;
  if (m->num_callbacks) {
    new_method->callback_list = 
      (FUNC_CALLBACK **) calloc (m->num_callbacks, sizeof (FUNC_CALLBACK));
    for (i=0; i<m->num_callbacks; i++) {
      new_method->callback_list [i] = (FUNC_CALLBACK *) 
	calloc (1, sizeof (FUNC_CALLBACK));
      new_method->callback_list [i]->callback_ptr = 
	m->callback_list [i]->callback_ptr;
      strcpy (new_method->callback_list [i]->callback_name,
	      m->callback_list [i]->callback_name);
    }
  }
  if (w->action_list->action_methods == (METHOD *) NULL) /* first one */ 
    new_method->last = (METHOD *) NULL;
  else {
    /* check to see if the method exists in the current list */
    if (CheckMethodName (w->action_list->action_methods, method_name)) {
      /* yup, it does...exit error */
      for (i=0; i<m->num_callbacks; i++)
	free (new_method->callback_list [i]);
      free (new_method->callback_list);
      free (new_method);
      return ((char *) NULL);
    }
    /* bring the list to the last known element, just in case */
    while (w->action_list->action_methods->next != NULL)
      w->action_list->action_methods = w->action_list->action_methods->next;
    new_method->last = w->action_list->action_methods;   /* register the last */
    w->action_list->action_methods->next = new_method;   /* register previous */
  }
  w->action_list->action_methods = new_method;       /* update the real 
						       action list with
						       the current allocation */

}
/**********************************************************/
/* create a method, add to method structure               */
/**********************************************************/
/**********************************************************/
/* create a methods list                                  */
/* will create the list if one does not exist             */
/* and will add to the list if the list is old            */
/**********************************************************/
char *CreateMethod (method_name)
char *method_name;
{

  static METHOD *m;
  METHOD *CheckMethodName ();

  m = (METHOD *) calloc (1, sizeof (METHOD));
  if (!method_name)
    return ((char *) NULL);
  else
    strcpy (m->method_name, method_name);
  
  /* always put the next to NULL and num_callbacks list to NULL */

  m->next = (METHOD *) NULL;
  m->num_callbacks = 0;
  m->callback_list = (FUNC_CALLBACK **) NULL;

  if (method_list == (METHOD*) NULL) {  /* first time */
    m->last = (METHOD *) NULL;
  }
  else {    /* its a widget to be added */
    if (CheckMethodName (method_list, method_name)) {
      /* found the name in the list already */
      /* so error */
      free (m);
      return ((char *) NULL);
    }

    /* bring the list to the last known element, just in case */
    while (method_list->next != NULL)
      method_list = method_list->next;

    m->last = method_list;                /* register the last */
    method_list->next = m;                /* register previous' next element */
  }
  method_list = m;                        /* update the active ptr */

  return ((char *) method_list->method_name);
}
/**********************************************************/
/* here add to a method a list of functions               */
/**********************************************************/
AddMethodFunction (method_name, func_list, num_callbacks)
char *method_name;
FUNC_CALLBACK **func_list;
int num_callbacks;
{
  METHOD *m, *CheckMethodName ();
  int i;

  if ((m = CheckMethodName (method_list, method_name)) == (METHOD *) NULL)
    /* method not found */
    return (FUNC_ERROR);

  /* ok we have the method */

  m->num_callbacks = num_callbacks;
  m->callback_list = (FUNC_CALLBACK **) 
    calloc (num_callbacks, sizeof (FUNC_CALLBACK));
  for (i=0; i<num_callbacks; i++) {
    m->callback_list [i] = (FUNC_CALLBACK *) calloc (1, sizeof (FUNC_CALLBACK));
    m->callback_list [i]->callback_ptr = func_list [i]->callback_ptr;
    strcpy (m->callback_list [i]->callback_name, func_list [i]->callback_name);
  }
}

/**********************************************************/
/* invoke an action element, call the method, and do the callbacks
   associated with it
   this is the whole ball of wax....
*/
/**********************************************************/
InvokeAction (widget, widget_handle, action_name)
Widget widget;
char *widget_handle, *action_name;
{
  WIDGET_ACTION *w, *CheckWidgetHandle ();
  ACTION *CheckActionName ();
  METHOD *m, *CheckMethodName ();
  static METHOD *new_method;
  int i;
  char buff [256];

  if (!widget_handle || !action_name)
    return (FUNC_ERROR);

  if ((w = CheckWidgetName (widget_handle)) == (WIDGET_ACTION*) NULL)
    /* widget name not found */
    return (FUNC_ERROR);

  if ((w->action_list = CheckActionName (w, action_name)) == (ACTION*) NULL)
    /* action name not found */
    return (FUNC_ERROR);

  /* ok, we have an active name, now let's start at the first
     method and invoke each series of functions */
  /* check for a NULL list */
  if (w->action_list->action_methods == NULL)
    /* the list is NULL */
    return (LIST_HAS_NO_ELEMENTS);
  /* back up the list */
  while (w->action_list->action_methods->last != NULL)
    w->action_list->action_methods = w->action_list->action_methods->last;
  /* ok at first element in list */

  /* now loop through list */

  while (1) {
    /* got the method now do the functions */
    for(i=0; i<w->action_list->action_methods->num_callbacks; i++) {
      if (!i)
	sprintf (buff, "[action = %s] [%s] [method = %s] [function [%d] = %s]",
		 w->action_list->action_name,
		 w->action_list->mapping_name,
		 w->action_list->action_methods->method_name, i,
	       w->action_list->action_methods->callback_list[i]->callback_name);
      else
	sprintf (buff, "                            [function [%d] = %s]", i,
	       w->action_list->action_methods->callback_list[i]->callback_name);

      w->action_list->action_methods->callback_list [i]->callback_ptr (widget, 
								   buff);
    }
    if (!w->action_list->action_methods->next)
      break;
    w->action_list->action_methods = w->action_list->action_methods->next;
  }
}

/**********************************************************/
/* given a widget name, check its existence and return a handle
   for now the handle is the name of the widget itself.
*/
/**********************************************************/
char *GetWidgetHandle (widget_name)
char *widget_name;
{

  WIDGET_ACTION *w, *CheckWidgetHandle ();

  if (!widget_name)
    return ((char *) NULL);

  if ((w = CheckWidgetName (widget_name)) == (WIDGET_ACTION*) NULL)
    return ((char *) NULL);
  
  return ((char *) widget_name);
}


/**********************************************************/
/* given a motif callback, map into an action item in our
   action list. return the item.
*/
/**********************************************************/
char *GetActionMapping (widget_handle, motif_callback_name)
char *widget_handle, *motif_callback_name;
{

  WIDGET_ACTION *w, *CheckWidgetHandle ();

  if (!widget_handle || !motif_callback_name)
    return ((char *) NULL);

  if ((w = CheckWidgetName (widget_handle)) == (WIDGET_ACTION*) NULL)
    return ((char *) NULL);
  
  /* we have the widget, so let's check the actions */

  while (w->action_list->last != NULL)
    w->action_list = w->action_list->last;

  /* ok, now we are at the first element */

   /* go forward until end of list */

  while (1) {
    if (!strcmp (w->action_list->mapping_name, motif_callback_name))
      /* we have a match */
      return ((char *) w->action_list->action_name);
    if (!w->action_list->next)
      break;
    w->action_list = w->action_list->next;
  }
  /* if we get here there is no widget in the list by the name requested */

  return ((char *) NULL);

}

/**********************************************************/
WIDGET_ACTION *CheckWidgetName (widget_name)
char *widget_name;

{

  if (!widget_name || !widget_action_depend)
    return ((WIDGET_ACTION *) NULL);

  /* back up the list */
  while (widget_action_depend->last != NULL)
    widget_action_depend = widget_action_depend->last;
  /* ok, now we are at the first element */

  /* go forward until end of list */
  while (1) {
    if (!strcmp (widget_action_depend->widget_name, widget_name))
      /* we found the widget */
      return ((WIDGET_ACTION *) widget_action_depend);
    if (!widget_action_depend->next)
      break;
    widget_action_depend = widget_action_depend->next;
  }
  /* if we get here there is no widget in the list by the name requested */

  return ((WIDGET_ACTION *) NULL);

}

/**********************************************************/
ACTION *CheckActionName (w, action_name)
WIDGET_ACTION *w;
char *action_name;
{

  if (!action_name || !w->action_list)
    return ((ACTION *) NULL);
 
  /* back up the list */

  while (w->action_list->last != NULL)
    w->action_list = w->action_list->last;

  /* ok, now we are at the first element */

   /* go forward until end of list */

  while (1) {
    if (!strcmp (w->action_list->action_name, action_name))
      /* we have a match */
      return ((ACTION *) w->action_list);
    if (!w->action_list->next)
      break;
    w->action_list = w->action_list->next;
  }
  /* if we get here there is no widget in the list by the name requested */

  return ((ACTION *) NULL);

}

/**********************************************************/
METHOD *CheckMethodName (method_list_ptr, method_name)
METHOD *method_list_ptr;
char *method_name;

{

  if (!method_name || !method_list_ptr)
    return ((METHOD *) NULL);

  /* back up the list */
  while (method_list_ptr->last != NULL)
    method_list_ptr = method_list_ptr->last;
  /* ok, now we are at the first element */

  /* go forward until end of list */
  while (1) {
    if (!strcmp (method_list_ptr->method_name, method_name))
      /* we found the widget */
      return ((METHOD *) method_list_ptr);
    if (!method_list_ptr->next)
      break;
    method_list_ptr = method_list_ptr->next;
  }
  /* if we get here there is no method in the list by the name requested */

  return ((METHOD *) NULL);

}


/**********************************************************/
char **GetAllActions (widget_name, num_actions)
char *widget_name;
int *num_actions;
{

  WIDGET_ACTION *w;
  static char *retstr [200];

  *num_actions = 0;

  if ((w = CheckWidgetName (widget_name)) == (WIDGET_ACTION *) NULL)
    return ((char **) NULL);

  while (w->action_list->last != NULL)
    w->action_list = w->action_list->last;

  /* ok, now we are at the first element */

  /* go forward until end of list */

  while (1) {
    (*num_actions)++;
/*    printf ("action name = %s\n", w->action_list->action_name);
    printf ("callback map = %s\n", w->action_list->mapping_name);
*/
    retstr [(*num_actions)-1] = w->action_list->action_name;
    if (!w->action_list->next)
      break;
    w->action_list = w->action_list->next;
  }
  return ((char **) retstr);
}

/**********************************************************/
char **GetAllWidgets (num_widgets)
int *num_widgets;
{
  static char *retstr [200];

  *num_widgets = 0;
  /* check a null list, just in case */
  if (widget_action_depend == (WIDGET_ACTION *) NULL)
    return ((char **) NULL);

  /* back up the list */
  while (widget_action_depend->last != NULL)
    widget_action_depend = widget_action_depend->last;
  /* ok, now we are at the first element */

  /* go forward until end of list */
  while (1) {
    (*num_widgets)++;
/*    printf ("widget = %s\n", widget_action_depend->widget_name);
*/    retstr [(*num_widgets)-1] = widget_action_depend->widget_name;
    if (!widget_action_depend->next)
      break;
    widget_action_depend = widget_action_depend->next;
  }
  return ((char **) retstr);
}


/**********************************************************/
/* gets all methods from a method list
*/
/**********************************************************/
char **GetAllMethods (method_list_ptr, num_methods)
METHOD *method_list_ptr;
int *num_methods;
{

  static char *retstr [200];

  *num_methods = 0;
  /* check for a null reference*/
  if (method_list_ptr == (METHOD *) NULL)
    return ((char **) NULL);
  /* back up the list */
  while (method_list_ptr->last != NULL)
    method_list_ptr = method_list_ptr->last;
  /* ok, now we are at the first element */

  /* go forward until end of list */
  while (1) {
    (*num_methods)++;
    printf ("method = %s\n", method_list_ptr->method_name);
    retstr [(*num_methods)-1] = method_list_ptr->method_name;
    if (!method_list_ptr->next)
      break;
    method_list_ptr = method_list_ptr->next;
  }
  return ((char **) retstr);
}

/**********************************************************/
/* gets all functions from a method list
*/
/**********************************************************/
char **GetAllMethodFunctions (method_name, num_functions)
char *method_name;
int *num_functions;
{

  METHOD *local_method_ptr;
  static char *retstr [200];
  int i;

  *num_functions = 0;
  /* check for a null reference*/
  if ((local_method_ptr = CheckMethodName (method_list, method_name)) ==
      (METHOD *) NULL)
    return ((char **) NULL);

  /* we are at the active method element in the list */

  /* go forward until end of list */
  for (i=0; i<local_method_ptr->num_callbacks; i++) {
    printf ("method function= %s\n", 
	    local_method_ptr->callback_list [i]->callback_name);
    retstr [i] = local_method_ptr->callback_list [i]->callback_name;
  }
  *num_functions = i;
  return ((char **) retstr);
}

/**********************************************************/
char **GetImbededMethods (widget_name, action_name, num_methods)
char *widget_name, *action_name;
int *num_methods;
{
  WIDGET_ACTION *w, *CheckWidgetHandle ();
  ACTION *CheckActionName ();
  METHOD m;
  static char *retstr [200];

  *num_methods = 0;
  if (!widget_name || !action_name)
    return ((char **) NULL);

  if ((w = CheckWidgetName (widget_name)) == (WIDGET_ACTION *) NULL)
    /* widget name not found */
    return ((char **) NULL);

  if ((w->action_list = CheckActionName (w, action_name)) == (ACTION*) NULL)
    /* action name not found */
    return ((char **) NULL);

  /* back up the list */
  while (w->action_list->action_methods->last != NULL)
    w->action_list->action_methods = w->action_list->action_methods->last;
  /* ok at first element in list */

  /* now loop through list */

  while (1) {
    (*num_methods)++;
/*    printf ("action = %s, imbeded method = %s\n", 
	    w->action_list->action_name,
	    w->action_list->action_methods->method_name);
*/
    retstr [(*num_methods)-1] = w->action_list->action_methods->method_name;
    if (!w->action_list->action_methods->next)
      break;
    w->action_list->action_methods = w->action_list->action_methods->next;
  }
  return ((char **) retstr);
}

/**********************************************************/
char **GetImbededFunctions(widget_name, action_name, method_name, num_functions)
char *widget_name, *action_name, *method_name;
int *num_functions;
{
  WIDGET_ACTION *w, *CheckWidgetHandle ();
  ACTION *CheckActionName ();
  METHOD *m, *CheckMethodName ();
  static char *retstr [200];
  int i;

  *num_functions = 0;
  if (!widget_name || !action_name || !method_name)
    return ((char **) NULL);

  if ((w = CheckWidgetName (widget_name)) == (WIDGET_ACTION *) NULL)
    /* widget name not found */
    return ((char **) NULL);

  if ((w->action_list = CheckActionName (w, action_name)) == (ACTION*) NULL)
    /* action name not found */
    return ((char **) NULL);

    /* check to see if the method exists in the current list */
  if ((m = CheckMethodName (w->action_list->action_methods, method_name)) ==
      (METHOD *) NULL)
    return ((char **) NULL);

  /* ok we now have the method we want */

  for (i=0; i<m->num_callbacks; i++) {
/*    printf ("function = %s\n", m->callback_list [i]->callback_name);
*/
    retstr [(*num_functions) ++] = m->callback_list [i]->callback_name;
  }
  return ((char **) retstr);
}

/***********************************************************/
/* get all the objects in the system and the hierarchy of 
   the system.
*/
/***********************************************************/
XmString *GetKnownObjects (num_objects)
int *num_objects;
{
  
  static XmString *addrstr;
  int i = 0, num_widgets, num_actions, num_methods, num_functions, x, z, y;
  char **widget_objects, **GetAllWidgets ();
  char **action_objects, **GetAllActions ();
  char **method_objects, **GetImbededMethods ();
  char **function_objects, **GetImbededFunctions ();
  char buff [256];

  *num_objects = 0;
  addrstr = (XmString *) calloc (500, sizeof (XmString));

  widget_objects = GetAllWidgets (&num_widgets);
  for (i=0; i<num_widgets; i++) {
    action_objects = GetAllActions (widget_objects [i], &num_actions);
    sprintf (buff, "WIDGET = %s", widget_objects [i]);
    printf ("WIDGET = %s\n", widget_objects [i]);
    addrstr[(*num_objects)++] = XmStringLtoRCreate (buff, 
						    XmSTRING_DEFAULT_CHARSET);
    for (z=0; z<num_actions; z++) {
      method_objects = GetImbededMethods (widget_objects [i], 
					  action_objects [z],
					  &num_methods);
      sprintf (buff, "    ACTION = %s", action_objects [z]);
      printf ("    ACTION = %s\n", action_objects [z]);
      addrstr[(*num_objects)++] = XmStringLtoRCreate (buff, 
						      XmSTRING_DEFAULT_CHARSET);
      for (y=0; y<num_methods; y++) {
	function_objects = GetImbededFunctions (widget_objects [i], 
						action_objects [z],
						method_objects [y],
						&num_functions);
	sprintf (buff, "        METHOD = %s", method_objects [y]);
	printf ("        METHOD = %s\n", method_objects [y]);
	addrstr[(*num_objects)++] = XmStringLtoRCreate (buff, 
						      XmSTRING_DEFAULT_CHARSET);
	for (x=0; x<num_functions; x++) {
	  sprintf (buff, "            FUNCTION = %s", function_objects [x]);
	  printf ("            FUNCTION = %s\n", function_objects [x]);
	  addrstr[(*num_objects)++] = XmStringLtoRCreate (buff, 
						      XmSTRING_DEFAULT_CHARSET);
	}
      }
    }
  }  
  return (addrstr);
}

/***********************************************************/
/* get the list of all the methods and their functions     */
/***********************************************************/
XmString *GetKnownMethods (num_objects)
int *num_objects;
{
  
  static XmString *addrstr;
  int i, z,  num_methods, num_functions;
  char **method_objects, **GetAllMethods ();
  char **method_functions, **GetAllMethodFunctions ();
  char buff [256];

  *num_objects = 0;

  addrstr = (XmString *) calloc (5000, sizeof (XmString));
  method_objects = GetAllMethods (method_list, &num_methods);
  for (i=0; i<num_methods; i++) {
    method_functions = GetAllMethodFunctions (method_objects [i], 
					      &num_functions);
    sprintf (buff, "METHOD = %s", method_objects [i]);
    printf ("METHOD = %s\n", method_objects [i]);
    addrstr[(*num_objects)++] = XmStringLtoRCreate (buff, 
						    XmSTRING_DEFAULT_CHARSET);
    for (z=0; z<num_functions; z++) {
      sprintf (buff, "    FUNCTION = %s", method_functions [z]);
      printf ("FUNCTION = %s\n", method_functions [z]);
      addrstr[(*num_objects)++] = XmStringLtoRCreate (buff, 
						      XmSTRING_DEFAULT_CHARSET);
    }
  }
  return (addrstr);
}
