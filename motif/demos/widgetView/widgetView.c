#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)widgetView.c	1.16 90/08/16";
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
 **   File:     @(#)widgetView.c	1.15 - 90/08/15
 **
 **   Project:     Motif - widget examination program
 **
 **   Description: Program which shows resources of widgets
 **
 **   Author:	   Pete Levine
 **
 **
 **
 ****************************************************************************
 ****************************************************************************/

#define MAIN 1

#include "widgetView.h"
#include "widgets.h"
#include "globals.h"
#include "callbacks.h"
#define UNEDITABLE 99

/**********************************************************/
SaveFileOK (file_type, template_name, new_file)
char *file_type;
char *template_name, *new_file;
{
  int i;
  FILE *fp;
  char *buffer, *GetSource ();
  char buff [256];

  if ((buffer = GetSource (template_name)) == (char *) NULL) {
    PrintError ("Cannot find template.");
    return (0);
  }
  
  /* write to the file */
  
  if ((fp = fopen (new_file, "r+")) == NULL) {
    /* DNE, so create */
    if ((fp = fopen (new_file, "a+")) == NULL) {
      sprintf (buff, "error creating or opening %s\n", new_file);
      PrintError (buff);
      return (0);
    }
    else {
      if (!strcmp (file_type, C_CODE) ||
	  !strcmp (file_type, C_CODE_WITH_TEXT))
	CreateCSourceFile (fp, buffer, file_type);
      else
	if (!strcmp (file_type, UIL_CODE) ||
	    !strcmp (file_type, UIL_CODE_WITH_TEXT))
	  CreateUILSourceFile (fp, buffer, file_type);
    }
  }
  else {
    sprintf (buff, "File %s exists.\nOverwrite ?", new_file);
    QuestionMsg (buff);
    XtRemoveAllCallbacks (QuestionDialog, XmNokCallback);
    sprintf (buff, "%s,%s,%s", file_type, template_name, new_file);
    XtAddCallback (QuestionDialog, XmNokCallback, QuestionDialogOK, buff);

  }
  
  
  fclose (fp);
  free (buffer);

  XtUnmanageChild (PrintFileDialog);
  XtUnmanageChild (PrintFileSelection);

}

/***********************************************************/
void  main(argc, argv)
int     argc;
char    **argv;
{
  XtAppContext    app_context;
  register int           n;
  Arg           args[MAX_ARGS];
  char flag [50];
  
  /* NULL out some widgets */
  
  MenuBar = (Widget) NULL;
  ActiveWidget.address = (Widget) NULL;
  ActiveWidget.message_buffer = (char *) NULL;
  
  signal(SIGHUP,  Quit);
  signal(SIGINT,  Quit);
  signal(SIGQUIT, Quit);
  
  if (access (C_TEMPLATE_FILE, R_OK) < 0) {
    printf ("Cannot read or find %s. Please Install.\n", C_TEMPLATE_FILE);
    exit (0);
  }
  if (access (C_TEMPLATE_WITH_TEXT_FILE, R_OK) < 0) {
    printf ("Cannot read or find %s. Please Install.\n", 
	    C_TEMPLATE_WITH_TEXT_FILE);
    exit (0);
  }
  if (access (UIL_TEMPLATE_FILE, R_OK) < 0) {
    printf ("Cannot read or find %s. Please Install.\n", UIL_TEMPLATE_FILE);
    exit (0);
  }
  if (access (UIL_TEMPLATE_WITH_TEXT_FILE, R_OK) < 0) {
    printf ("Cannot read or find %s. Please Install.\n", 
	    UIL_TEMPLATE_WITH_TEXT_FILE);
    exit (0);
  }
  
  XtToolkitInitialize();
  app_context = XtCreateApplicationContext();
  if ((display = XtOpenDisplay (app_context, NULL, "widgetView", "XMclient",
				NULL, 0, &argc, argv)) == NULL)
    {
      fprintf (stderr,"\n%s:  Can't open display\n", argv[0]);
      exit(1);
    }
  
  screen = DefaultScreen (display);
  if (DisplayPlanes (display, screen) == 1) {
    black_and_white = 1;
  }
  cmap = DefaultColormap (display, screen);
  
  if (argv [1]) {
    strcpy (flag, argv[1]);
    if (!strcmp(flag, "-f")) {
      strcpy (filename, argv [2]);
      printf ("filename = %s\n", filename);
    }
    else {
      printf ("Only -f option is specified.");
      exit (0);
    }
  }
  
  
  /* create the callback action list */
  
  /* not in this version */
  /*     CreateActionElements ();  */
  
  
  n = 0;
  XtSetArg(args[n], XmNallowShellResize, True);  n++;
  Shell = XtAppCreateShell(NULL, NULL, applicationShellWidgetClass,
			   display, args, n);
  
  n = 0;
  MainWindow = XmCreateMainWindow(Shell, "MainWindow", args, n);
  XtManageChild(MainWindow);
  
  n = 0;
  XtSetArg(args[n], XmNresizePolicy,  XmRESIZE_GROW);  n++;
  WorkRegion = XmCreateBulletinBoard (MainWindow, "s_text", args, n);
  XtManageChild (WorkRegion);
  
  SetColor (colors);
  
  CreateMenuBar ();
  
  CreateTextWin (); 
  
  CreateDialogShells ();
  

  CreateTextShell ();
  
  GetColor ();
  
  if (argv [1])
    DisplayResourceFile (filename);
  
  
  XtRealizeWidget(Shell);
  
  XtAppMainLoop(app_context);
  
}

/***********************************************************/
SetNewWidget (widget)
Widget widget;
{

    int           n, i;
    Arg           args[MAX_ARGS];
    char          buff [256];

    long   hscroll, vscroll;
    
    /* create a main bulletin for the scrolled window */

    n=0;
    XtSetArg(args[n], XmNx,  0);  n++;
    XtSetArg(args[n], XmNy,  0);  n++;
    XtSetArg(args[n], XmNheight,  1500);  n++;
    XtSetArg(args[n], XmNwidth,  WIDTH);  n++;
    XtSetArg(args[n], XmNshadowThickness, 0);  n++; 
    XtSetArg(args[n], XmNresizePolicy,  XmRESIZE_ANY);  n++;
    TBoardW = XmCreateBulletinBoard (TextWin, "Form", args, n);


    XtSetArg(args[0], XmNverticalScrollBar,  &vscroll);
    XtGetValues (TextWin, args, 1);

    XmScrolledWindowSetAreas (TextWin, NULL, NULL, TBoardW);

    XtManageChild (TBoardW);
    XtUnmanageChild (vscroll);

    CreateLabel (FormWin, "RESOURCE NAME", 12, 5);
    CreateLabel (FormWin, "RESOURCE TYPE", 136, 5);
    CreateLabel (FormWin, "VALUE CHANGE FIELD", 302, 5);

    resources = GetWidgetHierarchy (widget);
    for (i = 0; i<resources->num_items; i++) {
      resources->value [i] = 0;
      sprintf (buff, "%-20s%-20s", resources->name [i], resources->type[i]);
      CreateColorLabel (TBoardW, buff, 0, (i) * 50, resources->color [i]);
      XtSetArg(args [i], resources->name [i], &resources->value [i]);
    }
    XtGetValues (widget, args, i);

    for (i = 0; i<resources->num_items; i++) {
      /* we have the value so get the correct display widget up */
      CreateValueField (TBoardW, i);
      /* hold the original values around */
      resources->orgvalue [i] = resources->value [i];
/*
      AutoUpdateValueField (resources->widget_addr [i],
			    resources->value [i],
			    i);
*/
    }

    XtManageChild (vscroll);

    return (FUNC_OK);

}

/***********************************************************/
CreateValueField (parent, offset)
Widget parent;
int offset;
{

  Widget widget;
  int minimum, maximum, pixel_has_a_name = 0;
  char *textstr = NOT_SET, *normstring, buffer [128];
  char *extract_normal_string ();
  int           n, i;
  Arg           args[MAX_ARGS];

  minimum = maximum = 0;
  resources->widget_type [offset] = 0;

  /* set all edit fields to editable */

  resources->editable [offset] = True;
  
  switch (GetFieldType (resources->type [offset], offset, &minimum, &maximum)) {

      case TOGGLE_BUT:
          /* create toggle button widget */
          if (resources->value [offset]) {
	    widget = CreatePushButton (parent, "True", 300, 
				   (offset)* 50, 0, 0);
	    /* set explicitly True */
	    resources->value [offset] = True;
	  }
	  else {
	    widget = CreatePushButton (parent, "False", 300, 
				   (offset)* 50, 0, 0);
	    /* set explicitly False */
	    resources->value [offset] = False;
	  }
	  XtAddCallback (widget, XmNactivateCallback, ValueField, NULL);
	  resources->widget_type [offset] = TOGGLE_BUT;
	  break;

      case SCALE:
	  /* do a little sanity check on the values */
	  if (resources->value [offset] < minimum)
	    resources->value [offset] = minimum;
	  if (resources->value [offset] > maximum)
	    resources->value [offset] = maximum;
	  widget = CreateScale (parent, resources->value [offset], 
				minimum, maximum, 300, (offset) * 50);
	  XtAddCallback (widget, XmNdragCallback, ValueField, NULL);
	  XtAddCallback (widget, XmNvalueChangedCallback, ValueField, NULL);
	  resources->widget_type [offset] = SCALE;
	  break;

      case CPD_STRING:
      
	  if (resources->value [offset] == (long) NULL)
	    textstr = (char *) NULL;
	  else
	    textstr = extract_normal_string (resources->value [offset]);

	  resources->widget_type [offset] = CPD_STRING;

	  /* fall through and proceed */

      case STRING:
	  
	  if (textstr == NOT_SET)
	    textstr = (char *) resources->value [offset];

	  resources->additional[offset] = 
	    (MORE_INFO *) calloc (1, sizeof (MORE_INFO));

	  PackString (textstr, offset);

	  resources->additional [offset]->value = resources->value [offset];

	  widget = CreateTextInputField (parent, textstr, 300, (offset) * 50,
                                         TEXT_INPUT_LENGTH);
	  XtAddCallback (widget, XmNvalueChangedCallback, ValueField, NULL);
	  if (!resources->widget_type [offset])
	    resources->widget_type [offset] = STRING;
	  break;

      case CALLBACKS:

	  /* create a toogle button which turns on or off the callback */
	  /* turn off all callbacks to begin with */
	  resources->value [offset] = (long) NULL;
	  widget = CreatePushButton (parent, " OFF ", 300, 
				   (offset)* 50, 0, 0);
	  XtAddCallback (widget, XmNactivateCallback, ValueField, NULL);
	  resources->widget_type [offset] = CALLBACKS;
	  break;

      case COLOR:
	  /* here we have a color entry */
	  resources->editable [offset] = True;
	  widget = CreatePushButton (parent, "          ", 300, 
					 (offset) * 50, 0, 0);
	  n = 0;
	  XtSetArg(args [n], XmNbackground, resources->value [offset]);
	  n++;
	  XtSetArg(args[n], XmNforeground, resources->value [offset]);
	  n++;
	  XtSetValues (widget, args, n);
	  resources->additional[offset] = 
	    (MORE_INFO *) calloc (1, sizeof (MORE_INFO));
	  for (i=0; i<colortype.num_colors; i++) {
	    if (colortype.color_list [i].pixel == resources->value [offset]) {
	      resources->additional[offset]->name = 
		colortype.color_list [i]. name;
	      resources->additional[offset]->value = 
		(long) colortype.color_list [i].pixel;
	      pixel_has_a_name = 1;
	      i = colortype.num_colors;
	    }
	  }
	  if (!pixel_has_a_name) {
	      resources->additional[offset]->name = NO_COLOR_NAME;
	      resources->additional[offset]->value = resources->value [offset];
	  }
	  resources->widget_type [offset] = COLOR;
	  XtAddCallback (widget, XmNactivateCallback, ValueField, NULL);
	  break;

      default:
	  sprintf (buffy, "%ld", resources->value [offset]);
	  widget = CreateLabel (parent, buffy, 300, (offset) * 50);
	  resources->editable [offset] = False;
	  resources->widget_type [offset] = UNEDITABLE;
  } /* switch */

  resources->widget_addr [offset] = widget;
 

}

/***********************************************************/
GetFieldType (resource_type, offset, minimum, maximum)
char *resource_type;
int offset,
    *minimum,
    *maximum;

{

  int i = 0;

  /* global structures give the values of the world 
   scale_types [], toggle_types []*/

  if (maximum && minimum)
    *minimum = *maximum = 0;

  while (strcmp (scale_vals [i].type , "END")) {
    if (!strcmp (scale_vals [i].type, resource_type)) {
      if (maximum && minimum) {
	*minimum = scale_vals [i].minimum;
	*maximum = scale_vals [i].maximum;
      }
      if ((offset > -1) || (maximum && minimum)) {
	if (resources->value [offset] > *maximum)
	  *maximum = (int) resources->value [offset];
      }
      return (SCALE);
    }
    i++;
  }
  i = 0;
  while (strcmp (toggle_types [i], "END")) {
    if (!strcmp (toggle_types [i++], resource_type))
      return (TOGGLE_BUT);
  }
  i = 0;
  while (strcmp (cpd_strings [i], "END")) {
    if (!strcmp (cpd_strings [i++], resource_type))
      return (CPD_STRING);
  }
  i = 0;
  while (strcmp (strings [i], "END")) {
    if (!strcmp (strings [i++], resource_type))
      return (STRING);
  }
  i = 0;
  while (strcmp (callback_types [i], "END")) {
    if (!strcmp (callback_types [i++], resource_type))
      return (CALLBACKS);
  }
  i = 0;
  while (strcmp (font_types [i], "END")) {
    if (!strcmp (font_types [i++], resource_type))
      return (FONTS);
  }
  i = 0;
  while (strcmp (color_types [i], "END")) {
    if (!strcmp (color_types [i++], resource_type))
      return (COLOR);
  }
  return (NOT_FOUND);
}
					    
/***********************************************************/
RESOURCEVALS *GetWidgetHierarchy (GenWidget)
Widget GenWidget;
{
  
  static XtResourceList resource;
  static RESOURCEVALS  *local_resources;
  WidgetClass s_class;
  int i, n, num_items, color_position = 0, num_classes = 0;
  char local_str [128];
  char local_classes [10][25];


  local_resources = (RESOURCEVALS *) calloc (1, sizeof (RESOURCEVALS));

  local_resources->num_items = num_items = 0;

  /* ok...it's monday morning after a weekend on the vineyahd.
     i think i ought to explain what the code will be doing here.
     the goal is to return a list of resources associated with the
     newly selected widget.
     however, we want to color code each resource with its corresponding
     class reference.  (i.e., x, y, - CORE - RED)
     the method which i use first gets the widget_class for the
     current widget.  by looping through the structure and obtaining
     a superclass each time, i am able to get the complete hierarchy.
     i will call get GetResourceList () each time a new superclass is
     derived and build the list of resources->..
   */


  /* get the starting class */

  s_class = XtClass (GenWidget);

  ActiveWidget.hierarchy [0] = 0;
  strcat (ActiveWidget.hierarchy, s_class->core_class.class_name);
  ActiveWidget.hierarchy [strlen (s_class->core_class.class_name)] = 0;

  /* get the first set of resources */
  XtGetResourceList (s_class, &resource, &local_resources->num_items);
  /* now put the resources into the list */
  for (i = 0; i < local_resources->num_items; i++) {
    local_resources->name [i] = 
      (char *) calloc (strlen (resource [i].resource_name) + 1, 1);
    strcpy (local_resources->name [i], resource [i].resource_name);

    local_resources->type [i] = 
      (char *) calloc (strlen (resource [i].resource_type) + 1, 1);
    strcpy (local_resources->type [i], resource [i].resource_type);
    local_resources->class [i] = (char *) calloc (25, 1);
    strcpy (local_resources->class [i], s_class->core_class.class_name);
    local_resources->color [i] = colors [num_classes%max_hierarchy_colors];
  }

  strcpy (local_classes [num_classes], s_class->core_class.class_name);

  /* so now we have a complete list, all with the same color */

  s_class = s_class->core_class.superclass;
  /* set the starting number of total resources (for packing and coloring) */
  color_position = local_resources->num_items;


  while(1) {

    /* loop through superclasses until we reach core */

    if (!s_class)
      break;
    if (!strcmp (s_class->core_class.class_name, "Core")) {
      /* get the core class */
      XtGetResourceList (s_class, &resource, &num_items);
      num_classes ++;
      UpdateResourceList (local_resources, 
			  colors [(num_classes%max_hierarchy_colors)], 
			  s_class->core_class.class_name, num_items);
      strcpy (local_classes [num_classes], s_class->core_class.class_name);
      break;
    }
    /* get more resources */
    XtGetResourceList (s_class, &resource, &num_items);
    num_classes++;
    UpdateResourceList (local_resources, 
			colors [(num_classes%max_hierarchy_colors)], 
			s_class->core_class.class_name, num_items);
    strcpy (local_classes [num_classes], s_class->core_class.class_name);

    color_position = num_items;
    s_class = s_class->core_class.superclass;
  } /* while */


  /* create the hierarchy string */

  i = 0;
  while (num_classes >= 0) { 
    HierarchyLabels [i] = 
      CreateColorLabel (WorkRegion, local_classes [num_classes], 
			i * 100, 550, 
			colors [(num_classes%max_hierarchy_colors)]);     
    num_classes --; i ++;
  }

  /* this is the core.manager... hierarchy */
  sprintf (local_str, "ACTIVE WIDGET: %s", ActiveWidget.widget_name);
  WidgetHierarchy = CreateColorLabel (WorkRegion, local_str, 
				      350, 35, colors [0]);

  return (local_resources);

}

/***********************************************************/
UpdateResourceList (local_resources, local_color, class, num_items)
RESOURCEVALS  *local_resources;
long local_color;
char *class;
int num_items;
{
  int i, flg = 0;

  for (i = 0; i < num_items; i++) {
      local_resources->color [i] = local_color;
      strcpy (local_resources->class [i], class);
  }
}

/***********************************************************/
CreateMenuBar ()
{

    int           n;
    Arg           args[MAX_ARGS];


    /*
    **  the menubar
    */

    n = 0;
    MenuBar = (Widget) XmCreateMenuBar ((Widget) MainWindow, "MenuBar", 
					args, n); 
    if (MenuBar == NULL) {
      PrintError ("null menu bar");
      exit (0);
    }
    XtManageChild(MenuBar);

    /*  create the first level pulldowns */

    n = 0;
    PullDown1 = XmCreatePulldownMenu (MenuBar, "File", args, n);
    PullDown2 = XmCreatePulldownMenu (MenuBar, "Objects", args, n);
    PullDown3 = XmCreatePulldownMenu (MenuBar, "Help", args, n);

    /*  create cascade buttons for menubar  */

    MenuBtn1 = CreateCascade ("File", 'F', PullDown1, MenuBar);
    MenuBtn2 = CreateCascade ("Objects", 'O',PullDown2, MenuBar);
    MenuBtn3 = CreateCascade ("Help", 'H', PullDown3, MenuBar);


    /*
    **  Menu1 (FILE menu)
    */

    Label1A = CreatePushButton (PullDown1, LABEL1, 0, 0, 0, 0);
    XtAddCallback (Label1A, XmNactivateCallback, NewFile, NULL);

    Label1B = CreatePushButton (PullDown1, LABEL2, 0, 0, 0, 0);
    XtAddCallback (Label1B, XmNactivateCallback, OpenResourceFile, NULL);

    Label1C = CreatePushButton (PullDown1, LABEL3, 0, 0, 0, 0);
    XtAddCallback (Label1C, XmNactivateCallback, SaveResourceFile, NULL);


    CreateSeparator (PullDown1);

    /* create the write source code cascade menu */

    n = 0;
    SourcePullDown = XmCreatePulldownMenu (PullDown1, "Generate Source Code", 
					   args, n);

    SourceCascade = CreateCascade ("Generate Source Code", '4', 
				   SourcePullDown, PullDown1);

    Ccode = CreatePushButton (SourcePullDown, "C Source Code", 0, 0, 0, 0);
    XtAddCallback (Ccode, XmNactivateCallback, WriteSourceCode, C_CODE);

    UILcode = CreatePushButton (SourcePullDown, "UIL Code", 0, 0, 0, 0);
    XtAddCallback (UILcode, XmNactivateCallback, WriteSourceCode, UIL_CODE);

    CreateSeparator (PullDown1);

    SourceToggle = CreatePushButton (PullDown1, "Create Message [OFF]", 
				 '1', 0, 0, False);

    XtAddCallback (SourceToggle, XmNactivateCallback, 
		   GenerateMessageToggle, NULL);


    CreateSeparator (PullDown1);

    /* create the print resource cascade menu */

    n = 0;
    PullDown1A = XmCreatePulldownMenu(PullDown1, "Print Resources",args, n);
    Cascade1 = CreateCascade ("Print Resources", '4', PullDown1A, PullDown1);

    ToFileLabel = CreatePushButton (PullDown1A, "To File", 0, 0, 0, 0);
    XtAddCallback (ToFileLabel, XmNactivateCallback, PrintToFile, NULL);

    ToPrinterLabel = CreatePushButton (PullDown1A, "To Printer", 0, 0, 0, 0);
    XtAddCallback (ToPrinterLabel, XmNactivateCallback, PrintToPrinter, NULL);


    CreateSeparator (PullDown1);

    Label1D = CreatePushButton (PullDown1, LABEL4, 0, 0, 0, 0);
    XtAddCallback (Label1D, XmNactivateCallback, MenuQuit, NULL);


    /*
    **  Menu2   (Object menu)
    */

    Label2A = CreatePushButton (PullDown2, "Widget", 0, 0, 0, 0);
/*    XtAddCallback (Label2A, XmNactivateCallback, SelectObjects, NULL); */

    Label2B = CreatePushButton (PullDown2, "Method", 0, 0, 0, 0);
/*    XtAddCallback (Label2B, XmNactivateCallback, SelectMethods, NULL); */

    Label2C = CreatePushButton (PullDown2, "Action", 0, 0, 0, 0);
/*    XtAddCallback (Label2C, XmNactivateCallback, SelectObjects, NULL); */

    /* menu 3  (HELP menu) */


    Label3A = CreatePushButton (PullDown3, LABEL6, 0, 0, 0, 0);
    XtAddCallback (Label3A, XmNactivateCallback, ManHelp, NULL);

    Label3B = CreatePushButton (PullDown3, LABEL7, 0, 0, 0, 0);
    XtAddCallback (Label3B, XmNactivateCallback, SourceHelp, NULL);

    Label3C = CreatePushButton (PullDown3, LABEL8, 0, 0, 0, 0);
    XtAddCallback (Label3C, XmNactivateCallback, ViewHelp, NULL);

}

/***********************************************************/
CreateDialogShells ()
{
  register int           n;
  Arg           args[MAX_ARGS];
  int           num_items, i, num_widgets, num_objects;
  XmString *addrstr, *GetKnownWidgets (), *GetKnownObjects (),
  *GetKnownMethods ();
  
  WidgetShell = CreateDialogShell (Shell, "List O' Widgets", 220, 220);
  addrstr = GetKnownWidgets (&num_widgets);
  WidgetDialog = CreateDialog (WidgetShell, addrstr, num_widgets); 
  XtAddCallback (WidgetDialog, XmNokCallback, NewFileOK, NULL);
  
  ObjectShell = CreateDialogShell (Shell, "List O' Actions", 220, 220);
  addrstr = GetKnownObjects (&num_objects);
  if (!num_objects) addrstr = NULL ;
  ObjectDialog = CreateDialog (ObjectShell, addrstr, num_objects); 
  
  MethodShell = CreateDialogShell (Shell, "List O' Methods", 220, 220);
  addrstr = GetKnownMethods (&num_objects);
  if (!num_objects) addrstr = NULL ;
  MethodDialog = CreateDialog (MethodShell, addrstr, num_objects); 
  
  FileDialog = CreateDialogShell (Shell, "Resource File Selection", 320, 320);
  FileSelection = CreateFileSelection (FileDialog, "Select File", "*.res"); 
  
  PrintFileDialog = CreateDialogShell (Shell, "Print Resources to File", 
				       320, 320);
  PrintFileSelection = CreateFileSelection (PrintFileDialog, 
					    "Select File", (char *) NULL); 
  
  ErrorDialog = XmCreateErrorDialog (Shell, "Error !", args, 0);
  XtAddCallback (ErrorDialog, XmNokCallback, ErrorDialogAway, NULL);
  XtAddCallback (ErrorDialog, XmNcancelCallback, ErrorDialogAway, NULL);
  
  QuestionDialog = XmCreateQuestionDialog (Shell, " ", args, 0);
  XtAddCallback (QuestionDialog, XmNokCallback, QuestionDialogOK, NULL);
  XtAddCallback (QuestionDialog, XmNcancelCallback, QuestionDialogAway, NULL);
  
}

/***********************************************************/
CreateTextShell ()
{
    int           n, i;
    Arg           args[MAX_ARGS];
    char          *buffer, *GetSource ();

    TextShell = CreateDialogShell (Shell, "Enter Text For Test", 200, 200);
    n = 0;
    XtSetArg(args[n], XmNwidth, 300);  n++;
    XtSetArg(args[n], XmNheight, 300);  n++;
    XtSetArg(args[n], XmNautoUnmanage, False);  n++;
    TextBulletin = XmCreateBulletinBoard (TextShell, "bulletin b", args, n);

    TextDialog = CreateMultilineText (TextBulletin, 0, 30, 200, 200);
    n = 0;
    XtSetArg(args[n], XmNeditMode,  XmMULTI_LINE_EDIT);  n++;
    XtSetArg(args[n], XmNeditable,  True);  n++;
    XtSetValues (TextDialog, args, n);

    TextLabelButton = CreateLabel (TextBulletin, 
				   "Enter Text To Appear With Test:",
				   10, 10);

    TextOKButton = CreatePushButton (TextBulletin, "OK", 10, 250, 50, 30);
    TextCancelButton = CreatePushButton (TextBulletin, "Cancel", 
					 100, 250, 50, 30);
    TextClearButton = CreatePushButton (TextBulletin, "Clear", 
					190, 250, 50, 30);

    /* ok button defined later */

    XtAddCallback (TextCancelButton, XmNactivateCallback, 
		   TextCancelFunction, NULL);
    XtAddCallback (TextClearButton, XmNactivateCallback, 
		   TextClearFunction, NULL);
  }

/***********************************************************/
CreateTextWin ()
{
    int           n, i;
    Arg           args[MAX_ARGS];
    char          *buffer, *GetSource ();
    
    /* widget bulletin board */

    n=0;
    XtSetArg(args[n], XmNx,  0);  n++;
    XtSetArg(args[n], XmNy,  60);  n++;
    XtSetArg(args[n], XmNheight,  400);  n++;
    XtSetArg(args[n], XmNwidth,  275);  n++;
    XtSetArg(args[n], XmNshadowThickness, 2);  n++; 
    XtSetArg(args[n], XmNresizePolicy,  XmRESIZE_GROW);  n++;

    WidgetBoard = XmCreateBulletinBoard (WorkRegion, "Form", args, n);
    XtManageChild (WidgetBoard);

    /* form to contain scrolled window with labels above it */

    n=0;
    XtSetArg(args[n], XmNx,  350);  n++;
    XtSetArg(args[n], XmNy,  60);  n++;
    FormWin = XmCreateForm (WorkRegion, "formwin", args, n);
    XtManageChild(FormWin);

    /* text widget over on the right */

    n=0;
    XtSetArg(args[n], XmNx,  0);  n++;
    XtSetArg(args[n], XmNy,  30);  n++;
    XtSetArg(args[n], XmNheight,  370);  n++;
    XtSetArg(args[n], XmNwidth,  WIDTH - 350);  n++;
    XtSetArg(args[n], XmNscrollingPolicy, XmAUTOMATIC);  n++; 
    TextWin = XmCreateScrolledWindow (FormWin, "s_text", args, n);
    XtManageChild (TextWin);

    n=0;
    XtSetArg(args[n], XmNx,  0);  n++;
    XtSetArg(args[n], XmNy,  0);  n++;
    XtSetArg(args[n], XmNheight,  400);  n++;
    XtSetArg(args[n], XmNwidth,  WIDTH - 300);  n++;
    XtSetArg(args[n], XmNshadowThickness, 0);  n++; 
    XtSetArg(args[n], XmNresizePolicy,  XmRESIZE_ANY);  n++;
    TBoardW = XmCreateBulletinBoard (TextWin, "Form", args, n);
    XtManageChild (TBoardW);

    LabelW = CreateLabel (WorkRegion, "WIDGET DISPLAY REGION", 0, 0);
    LabelHierarchy = CreateLabel (WorkRegion, "WIDGET HIERARCHY", 0, 520);
    CreateLabel (WorkRegion, "RESOURCE EDIT WINDOW", 350, 0);
    n = 0;
    ResourceButton = CreatePushButton (WorkRegion, "Apply Resource Changes",
				       500, 490, 0, 0);
    XtAddCallback (ResourceButton, XmNactivateCallback, 
		   ApplyResourceCallback, NULL);

    ResourceButton1 = CreatePushButton (WorkRegion, "Cancel Resource Changes",
					500, 520, 0, 0);
    XtAddCallback (ResourceButton1, XmNactivateCallback, CancelResource, NULL);

}

/***********************************************************/
Widget GetWidgetList (widget_name, widget_offset, args, n)
char *widget_name;
int *widget_offset;
Arg  args [MAX_ARGS];
int n;
{
  int i = 0;
  static Widget widget;
  Arg  local_args [MAX_ARGS];
  int  local_n = 0;

  /* let's find the position of the widget list */



  if (!widget_name)
    return ((Widget) NULL);

  while (strcmp (widgetlist [i].name, "END")) {
    if (!strcmp (widgetlist [i].name, widget_name))
      break;
    i++;
  }

  /* check if we found something useful, basically if the last item is null
     we haven't found what we're looking for */

  if (!strcmp (widgetlist [i].name, "END"))
	 return ((Widget) NULL);

  /* got position in widgetlist [i].item */
 
  *widget_offset = i;

  switch (widgetlist [i].item) {
      /* create the widget with WidgetBoard as parent */
      case ARROW_BUTTON:
          widget = XmCreateArrowButton (WidgetBoard, "Arrow Button", args, n);
          break;
      case BULLETIN_BOARD:
          widget = XmCreateBulletinBoard (WidgetBoard, "Bulletin Board", args, n);
          break;
      case BULLETIN_BOARD_DIALOG:
          widget = XmCreateBulletinBoardDialog (WidgetBoard, "Bulletin Board Dialog", args, n);
          break;
      case CASCADE_BUTTON:
          widget = XmCreateCascadeButton (WidgetBoard, "Cascade Button", args, n);
          break;
      case CASCADE_BUTTON_GADGET:
	  widget = XmCreateCascadeButtonGadget (WidgetBoard, "Cascade Button", args, n);
          break;
      case COMMAND:
          widget = XmCreateCommand (WidgetBoard, "Command", args, n);
          break;
      case DIALOG_SHELL:
          widget = XmCreateDialogShell (WidgetBoard, "Dialog Shell", args, n);
          break;
      case DRAWING_AREA:
          widget = XmCreateDrawingArea (WidgetBoard, "Drawing Area", args, n);
          break;
      case DRAWN_BUTTON:
          widget = XmCreateDrawnButton (WidgetBoard, "Drawn Button", args, n);
          break;
      case ERROR_DIALOG:
          widget = XmCreateErrorDialog (WidgetBoard, "Error Dialog", args, n);
          break;
      case FILE_SELECTION_BOX:
          widget = XmCreateFileSelectionBox (WidgetBoard, "File Selection Box", args, n);
          break;
      case FILE_SELECTION_DIALOG:
          widget = XmCreateFileSelectionDialog (WidgetBoard, "File Selection Dialog", args, n);
          break;
      case FORM:
          widget = XmCreateForm (WidgetBoard, "Form", args, n);
          break;
      case FORM_DIALOG:
          widget = XmCreateFormDialog (WidgetBoard, "Form Dialog", args, n);
          break;
      case FRAME:
          widget = XmCreateFrame (WidgetBoard, "Frame", args, n);
          break;
      case INFORMATION_DIALOG:
          widget = XmCreateInformationDialog (WidgetBoard, "Information Dialog", args, n);
          break;
      case LABEL:
          widget = XmCreateLabel (WidgetBoard, "Label", args, n);
          break;
      case LABEL_GADGET:
	  widget = XmCreateLabelGadget (WidgetBoard, "Label", args, n);
          break;
      case LIST:
          widget = XmCreateList (WidgetBoard, "List", args, n);
          break;
      case MENU_BAR:
          widget = XmCreateMenuBar (WidgetBoard, "Menu Bar", args, n);
          break;
      case MENU_SHELL:
	  widget = XmCreateMenuShell (WidgetBoard, "Menu Shell", args, n);
          break;
      case MESSAGE_BOX:
          widget = XmCreateMessageBox (WidgetBoard, "Message Box", args, n);
          break;
      case MESSAGE_DIALOG:
          widget = XmCreateMessageDialog (WidgetBoard, "Message Dialog", args, n);
          break;
      case OPTION_MENU:
          widget = XmCreateOptionMenu (WidgetBoard, "Option Menu", args, n);
          break;
      case PANED_WINDOW:
          widget = XmCreatePanedWindow (WidgetBoard, "Paned Window", args, n);
          break;
      case POPUP_MENU:
          widget = XmCreatePopupMenu (WidgetBoard, "Popup Menu", args, n);
          break;
      case PROMPT_DIALOG:
          widget = XmCreatePromptDialog (WidgetBoard, "Prompt Dialog", args, n);
          break;
      case PULLDOWN_MENU:
          widget = XmCreatePulldownMenu (WidgetBoard, "Popup Menu", args, n);
          break;
      case PUSH_BUTTON:
          widget = XmCreatePushButton (WidgetBoard, "Push Button", args, n);
          break;
      case PUSH_BUTTON_GADGET:
          widget = XmCreatePushButtonGadget (WidgetBoard,"Push Button",args, n);
          break;
      case QUESTION_DIALOG:
          widget = XmCreateQuestionDialog (WidgetBoard, "Question Dialog", args, n);
          break;
      case RADIO_BOX:
          widget = XmCreateRadioBox (WidgetBoard, "Radio Box", args, n);
          break;
      case ROW_COLUMN:
          widget = XmCreateRowColumn (WidgetBoard, "Row Column", args, n);
          break;
      case SCALE_WIDGET:
          widget = XmCreateScale (WidgetBoard, "Scale", args, n);
          break;
      case SCROLLED_LIST:
          widget = XmCreateScrolledList (WidgetBoard, "Scrolled List", args, n);
          break;
      case SCROLL_BAR:
          widget = XmCreateScrollBar (WidgetBoard, "Scroll Bar", args, n);
          break;
      case SCROLLED_TEXT:
	  widget = XmCreateScrolledText (WidgetBoard, "Scrolled Text", args, n);
	  break;
      case SCROLLED_WINDOW:
	  widget = XmCreateScrolledWindow (WidgetBoard, "Scrolled Text", args, n);
	  break;
      case SELECTION_BOX:
          widget = XmCreateSelectionBox (WidgetBoard, "Selection Box", args, n);
          break;
      case SELECTION_DIALOG:
          widget = XmCreateSelectionDialog (WidgetBoard, "Selection Dialog", args, n);
          break;
      case SEPARATOR:
          widget = XmCreateSeparator (WidgetBoard, "Separator", args, n);
          break;
      case SEPARATOR_GADGET:
          widget = XmCreateSeparatorGadget (WidgetBoard, "Separator", args, n);
          break;
      case TEXT:
          widget = XmCreateText (WidgetBoard, "Text", args, n);
          break;
      case TOGGLE_BUTTON:
          widget = XmCreateToggleButton (WidgetBoard, "Toggle Button", args, n);
          break;
      case TOGGLE_BUTTON_GADGET:
          widget = XmCreateToggleButtonGadget (WidgetBoard, "Toggle Button Gadget", args, n);
          break;
      case WARNING_DIALOG:
          widget = XmCreateWarningDialog (WidgetBoard, "Warning Dialog", args, n);
          break;
      case WORKING_DIALOG:
          widget = XmCreateWorkingDialog (WidgetBoard, "Working Dialog", args, n);
	  break;
	default:
	  widget = (Widget) NULL;
     
	}
  if (widget != (Widget) NULL) {
    XtManageChild (widget);
  }
  return (widget);


}

/***********************************************************/
CreateCSourceFile (fp, buffer, file_type)
FILE *fp;
char *buffer;
char *file_type;
{
  
  int i;
  Arg  args [MAX_ARGS];
  int n, cpd_str = 0;

  if (!strcmp (file_type, C_CODE_WITH_TEXT))
    if (ActiveWidget.message_buffer != (char *) NULL)
      printf ("C WITH, buffer = %s\n", ActiveWidget.message_buffer);

  fprintf (fp, "%s", buffer);

  fprintf (fp, "/*****************************************************/\n");
  fprintf (fp, "Widget GenericRoutine (parent)\n");
  fprintf (fp, "Widget parent;\n{\n");
  fprintf (fp, "    Cardinal n, i, cpd_str = 0;\n    Arg  args[MAX_ARGS];\n");
  fprintf (fp, "    Widget widget;\n\n");
  fprintf (fp, "    XmString tcs [20];\n\n");
  fprintf (fp, "    \n\n\n/**************** create the widget ************/\n");
  fprintf (fp, "    widget = %s(parent, \"%s\", args, n);\n", 
           widgetlist [ActiveWidget.widget_offset].create_string,
	   ActiveWidget.widget_name);
  fprintf (fp, "\n\n\n");

  fprintf (fp,"    /*set up the resources for the widget*/\n");
  for (i=0; i<resources->num_items; i++) {
    if (resources->value [i] != resources->orgvalue [i]) {
      if (resources->widget_type [i] == CALLBACKS) {
	/* set all callbacks to NULL, for now */
	fprintf (fp, "    XtSetArg (args [n], XmN%s, NULL); n++;\n", 
		 resources->name [i]);
      }
      else {
	if (resources->editable [i] == True) {
	  /* only change the editable fields */
	  switch (resources->widget_type [i]) {

	  case CPD_STRING :
	    if (resources->value [i] == NULL)
	      fprintf (fp, "    XtSetArg (args [n], XmN%s, (XmString) NULL); n++;\n", 
		       resources->name [i]);
	    else {
	      if (!strcmp(resources->type [i], "XmStringTable")) {
		/* do a for loop and pack each member of the string */
		fprintf (fp, "/* list = %s */\n", 
			 resources->additional [i]->name);
	      }
	      else {
		fprintf (fp, "    tcs [%d] = XmStringLtoRCreate (\"%s\", XmSTRING_DEFAULT_CHARSET);\n", 
			 cpd_str, resources->additional [i]->name);
		fprintf (fp, "    XtSetArg (args [n], XmN%s, tcs [%d]); n++;\n", 
			 resources->name [i], cpd_str);
		fprintf (fp, "    cpd_str++;\n");
		cpd_str++;
	      }
	    }
	    break;

	  case STRING:
	    if (resources->value [i] == NULL) 
	      fprintf (fp, "    XtSetArg (args [n], XmN%s, (char *) NULL); n++;\n", resources->name [i]);
	    else
	      fprintf (fp, "    XtSetArg (args [n], XmN%s, \"%s\"); n++;\n", 
		       resources->name [i], resources->additional [i]->name);

	    break;

	  default:
	    fprintf (fp, "    XtSetArg (args [n], XmN%s, %ld); n++;\n", 
		     resources->name [i], resources->value [i]);
	    if (resources->widget_type [i] == COLOR)
	      fprintf (fp, "    /* color = %s, pixel = %ld */\n", 
		       resources->additional [i]->name,  
		       resources->additional [i]->value);
	    break;

	  } /* switch */
	}   /* if resources editable */
      }   /* else */
    }    /* if org != value */
  }    /* for */
    fprintf (fp, "    /* End the resource loading */\n");
  fprintf (fp, "\n    /* Now set the values */\n");
  fprintf (fp, "    XtSetValues (widget, args, n);\n\n");
  if (cpd_str) {
    fprintf (fp, "    for (i = 0; i < cpd_str-1; i++)\n");
    fprintf (fp, "        XmStringFree (tcs [i]);\n");
  }
  /* register the callback */ 

  for (i=0; i<resources->num_items; i++) {
    if (resources->widget_type [i] == CALLBACKS) {
      if (resources->value [i] != (long) NULL) {
	fprintf (fp, 
		 "    /********here is a callback being resgistered*******/\n");
	fprintf (fp, 
		 "    XtAddCallback(widget, XmN%s, \n                  GenericCallback, \"%s\");\n", 
		 resources->name [i], resources->name [i]);
      }
    }
  }
  fprintf (fp, "    return (widget);\n");
  fprintf (fp, "/*pete wuz here*/\n");
  fprintf (fp, "}\n");

}

/***********************************************************/
CreateUILSourceFile (fp, buffer, file_type)
FILE *fp;
char *buffer;
char *file_type;
{
  
  int i, numcolors = 0;
  Arg  args [MAX_ARGS];
  int n;
  char *colorbuff [256], *ConvertName ();
  char *value_buffer, *PackValueBuffer ();
  char *template_buffer, *GetSource ();
  char *normstring, *PrepareUILStrings ();
 
  if (!strcmp (file_type, UIL_CODE_WITH_TEXT))
    if (ActiveWidget.message_buffer != (char *) NULL) {
      /* load two colors to begin with */

      colorbuff [numcolors] = (char *) calloc (1, 5);
      sprintf (colorbuff [numcolors++], "navy");
      colorbuff [numcolors] = (char *) calloc (1, 6);
      sprintf (colorbuff [numcolors++], "wheat");

      value_buffer = PackValueBuffer (ActiveWidget.message_buffer);
      fprintf (fp, "module Test\n");
      fprintf (fp, "        version = 'v1.0'\n");
      fprintf (fp, "        names = case_sensitive\n\n");
      fprintf (fp, " value\n");
      fprintf (fp, "        message : ");
      fprintf (fp, "%s\n\n", value_buffer);
  
      free (value_buffer);

    }

  fprintf (fp, "%s", buffer);
  if (strcmp (file_type, UIL_CODE_WITH_TEXT))
    fprintf (fp,"    controls {\n");
  if (strcmp (widgetlist [ActiveWidget.widget_offset].uil_string, "NULL"))
    fprintf (fp,"                  %s    %s;\n", 
	     widgetlist [ActiveWidget.widget_offset].uil_string,
	     ConvertName (ActiveWidget.widget_name));
  else
    fprintf (fp,"                  %s    %s;\n", ActiveWidget.hierarchy,
	   ConvertName (ActiveWidget.widget_name));
  fprintf (fp,"    };\n");
  fprintf (fp,"   callbacks {\n");
  fprintf (fp,"    };\n");
  fprintf (fp,"  };\n");
  fprintf (fp,"\n\n\n");

  fprintf (fp,"  value\n");
  for (i=0; i<resources->num_items; i++) {
    if (resources->widget_type [i] == COLOR)
      if (strcmp (resources->additional [i]->name, NO_COLOR_NAME))
	if (NewColor (colorbuff, i, &numcolors))
	  fprintf (fp, "        %s: color ('%s');\n", 
		   ConvertName(resources->additional [i]->name),
		   resources->additional [i]->name);
  }
  fprintf (fp, "\n");
  for (i=0; i < numcolors; i++)
    free (colorbuff [i]);

  fprintf (fp,"  object\n");
  if (strcmp (widgetlist [ActiveWidget.widget_offset].uil_string, "NULL"))
    fprintf (fp,"        %s :  %s {\n", ConvertName (ActiveWidget.widget_name),
	     widgetlist [ActiveWidget.widget_offset].uil_string);
  else
    fprintf (fp,"        %s :  %s {\n", ConvertName (ActiveWidget.widget_name),
	     ActiveWidget.hierarchy);
  fprintf (fp,"            arguments {\n");

  for (i=0; i<resources->num_items; i++) {
    if (resources->value [i] != resources->orgvalue [i]) {
	if (resources->editable [i] == True &&
	    resources->widget_type [i] != CALLBACKS) {
	  switch (resources->widget_type [i]) {
	    case COLOR:
	               fprintf (fp, "                XmN%s =  %s;\n", 
		       resources->name [i], 	
		       ConvertName (resources->additional [i]->name));
		       break;

            case CPD_STRING:
		       if (resources->value [i] != NULL) {
			 if (!strcmp(resources->type [i], "XmStringTable")) {
			   normstring = PrepareUILStrings (resources->additional [i]->name);
			   fprintf (fp, 
				    "                XmN%s =  string_table (%s);\n", 
				    resources->name [i], 
				    normstring);
			   free (normstring);
			   
			 }
			 else {
			   fprintf (fp, 
				    "                XmN%s =  compound_string (\"%s\");\n", 
				    resources->name [i], 
				    resources->additional [i]->name);
			 }
		       }
		       break;
		       
            case STRING:
		       if (resources->value [i] != NULL)
			 fprintf (fp, 
				  "                XmN%s =  (\"%s\");\n", 
				  resources->name [i], 
				  resources->additional [i]->name);
		       break;

           case TOGGLE_BUT:
		       if (resources->value [i])
			 fprintf (fp, "                XmN%s =  true;\n", 
				  resources->name [i]);
		       else
			 fprintf (fp, "                XmN%s =  false;\n", 
				  resources->name [i]);
		       break;
	   default:
		       fprintf (fp, "                XmN%s =  %ld;\n", 
				resources->name [i], resources->value [i]);
		       break;
		     } /* switch */
	} /* != Callbacks */
      } /* value != orgvalue */
  } /* for */

  fprintf (fp,"            };\n");
  fprintf (fp,"        callbacks {\n");
  fprintf (fp,"            };\n");
  fprintf (fp,"        };\n");
  fprintf (fp,"\n\n");
  
  if (!strcmp (file_type, UIL_CODE_WITH_TEXT)) {
    template_buffer = GetSource (UIL_TEXT_TEMPLATE);
    fprintf (fp, "%s", template_buffer);
    free (template_buffer);
  }
  fprintf (fp,"end module;\n");
  

}

/***********************************************************/
NewColor (colorbuff, position, numcolors)
char *colorbuff [];
int position;
int *numcolors;

{
    
  int i;
  
  for (i = 0; i < *numcolors; i++)
    if (!strcmp (colorbuff [i], resources->additional [position]->name))
      /* existing color don't allocate a new variable for uil */
      return (0);
  colorbuff [*numcolors] = 
    (char *) calloc (1, strlen (resources->additional [position]->name) + 2); 
  sprintf (colorbuff [*numcolors], "%s", 
	   resources->additional [position]->name);
  *numcolors += 1;
  /* new color, ok to allocate */
  return (1);
}
  




/***********************************************************/
SetColor (colors)
long *colors;
{
  
  int i;
  XColor exact_def;
  char name [4][25];

  if (black_and_white) {
    max_hierarchy_colors = 1;
    strcpy (name [0], "black");
    strcpy (name [1], "white");
  }
  else {
    max_hierarchy_colors = 4;
    strcpy (name [0], "cyan");
    strcpy (name [1], "yellow");
    strcpy (name [2], "red");
    strcpy (name [3], "green");
  }

  for (i = 0; i < max_hierarchy_colors; i++) {
    if (!XParseColor (display, cmap, name [i], &exact_def)) {
      printf ("color name not in database\n");
      return (FUNC_ERROR);
    }
    if (!XAllocColor (display, cmap, &exact_def)) {
      printf ("cannot allocate color\n");
      return (FUNC_ERROR);
    }
    colors [i] = exact_def.pixel;
  } /* for */
}

/***********************************************************/
GetColor ()
{

  int num_colors, i, n;
  Arg           args[MAX_ARGS];
  char          buff [256];
  XmString *addrstr, *GetKnownColors (), tcs;

  ColorShell = CreateDialogShell (Shell, "List O' Colors", 250, 250);

  /* create a bulletin for the shell */

  n=0;
  XtSetArg(args[n], XmNx,  0);  n++;
  XtSetArg(args[n], XmNy,  0);  n++;
  XtSetArg(args[n], XmNheight,  400);  n++;
  XtSetArg(args[n], XmNwidth,  300);  n++;
  XtSetArg(args[n], XmNshadowThickness, 0);  n++; 
  XtSetArg(args[n], XmNresizePolicy,  XmRESIZE_ANY);  n++;
  ColorBoard = XmCreateBulletinBoard (ColorShell, "Form", args, n);

  addrstr = GetKnownColors (&colortype);
  ColorList = CreateList (ColorBoard, addrstr, colortype.num_colors); 
  XtAddCallback (ColorList, XmNdefaultActionCallback, NewColorOK, NULL);
  XtManageChild (ColorList);
  XmListSelectPos (ColorList, 1,False);

  XtSetArg(args[n], XmNx,  0);  n++;
  XtSetArg(args[n], XmNy,  225);  n++;
  XtSetArg(args[n], XmNheight,  200);  n++;
  XtSetArg(args[n], XmNwidth,  300);  n++;
  XtSetArg(args[n], XmNshadowType, XmSHADOW_OUT);n++;
  ColorFrame = XmCreateFrame (ColorBoard, "Frame", args, n);
  XtManageChild (ColorFrame);


  tcs = XmStringLtoRCreate ("Color Adjustment Scale", XmSTRING_DEFAULT_CHARSET);

  n= 0;
  XtSetArg(args[n], XmNx,  0);  n++;
  XtSetArg(args[n], XmNy,  0);  n++;
  XtSetArg(args[n], XmNwidth,  140);  n++;
  XtSetArg(args[n], XmNborderWidth,  4);  n++;

  XtSetArg(args[n], XmNvalue,  0);  n++;
  XtSetArg(args[n], XmNminimum, 0);  n++;
  if (black_and_white) {
    XtSetArg(args[n], XmNmaximum,  1);  n++;
  }
  else {
    XtSetArg(args[n], XmNmaximum,  100);  n++;
  }
  XtSetArg(args[n], XmNshowValue,  True);  n++;
  XtSetArg(args[n], XmNscaleHeight,  50);  n++; 
  XtSetArg(args[n], XmNtitleString,  tcs);  n++;
  XtSetArg(args[n], XmNorientation,  XmHORIZONTAL);  n++;
  XtSetArg(args[n], XmNprocessingDirection,  XmMAX_ON_RIGHT);  n++;
  ColorScale = XmCreateScale (ColorFrame, "Scale", args, n);
  XtManageChild (ColorScale);
    
  XmStringFree (tcs);

  XtAddCallback (ColorScale, XmNdragCallback, ColorUpdate, NULL);
  XtAddCallback (ColorList, XmNsingleSelectionCallback, 
		 SingleSelectColor, NULL);

  ColorLabel = CreatePushButton (ColorBoard, "the original", 
				      175, 225, 100, 100);
  XtAddCallback (ColorLabel, XmNactivateCallback, ColorLabelPressed, NULL);

  n=0;
  XtSetArg(args [n], XmNbackground, 0); n++;
  XtSetArg(args[n], XmNforeground, 0); n++;
  XtSetValues (ColorLabel, args, n);

  ColorConfirm = CreatePushButton (ColorBoard, "Press Here to Confirm Color", 
				      0, 350, 250, 50);
  XtAddCallback (ColorConfirm, XmNactivateCallback, ColorLabelPressed, NULL);

  

}

/***********************************************************/
XmString *GetKnownWidgets (num_widgets)
int *num_widgets;
{
  
  static XmString *addrstr;
  int i = 0;

  addrstr = (XmString *) calloc (100, sizeof (XmString));
  while (1) { 
    addrstr[i] = XmStringLtoRCreate (widgetlist [i].name, 
				     XmSTRING_DEFAULT_CHARSET);
    if (!strcmp (widgetlist [i].name, "END")) 
      break;
    i++;
  }
  *num_widgets = i;
  return (addrstr);
}   

/***********************************************************/
XmString *GetKnownColors (colortype)
COLORTYPE *colortype;
{
  
  static XmString *addrstr;
  XColor exact_def;
  int i = 0, alpha_found, copy_pos;
  char *color_list [MAX_COLORS], buff [50];
  FILE *fp;
  int dum, j ;
  char sdum[50] ;
  int diff ;

  colortype->num_colors = 0;

  if (!black_and_white) {
      if ((fp = fopen (RGB, "r")) == NULL) {
	  printf ("can not open %s\n", RGB);
	  return (NULL);
      }
    
      while (fgets (buff, 80, fp) != NULL)  {
	  colortype->color_list [colortype->num_colors].name = 
	      (char *) calloc (1, 50);
	  if (sscanf(buff,"%d %d %d %s %s",&dum,&dum,&dum,
		     colortype->color_list [colortype->num_colors].name,
		     sdum) == 4) {
	      if (colortype->num_colors) {
		  diff = 0 ;
	          for (i=0; 
		      ((i < strlen (
			colortype->color_list [colortype->num_colors-1].name))
		    && (i < strlen(
			colortype->color_list [colortype->num_colors].name)));
		      i++) {
		     if ((tolower(colortype->color_list 
				 [colortype->num_colors-1].name[i])) !=
			 (tolower(colortype->color_list 
				 [colortype->num_colors].name[i]))) {
			 diff = 1 ;
			 break ;
		     }
		 }
		 if (diff) colortype->num_colors += 1;
		  else free(colortype->color_list 
			    [colortype->num_colors].name);
	      } else colortype->num_colors += 1;
	      if (colortype->num_colors >= MAX_COLORS) {
		  printf ("number of available colors exceeded\n");
		  break;
	      }
	  }
      }
      fclose (fp);
  }
  else {   /* black and white screen */
      colortype->color_list [colortype->num_colors].name = 
	  (char *) calloc (1, 10);
      strcpy (colortype->color_list [colortype->num_colors].name, "black");

      colortype->num_colors += 1;
      colortype->color_list [colortype->num_colors].name = 
	  (char *) calloc (1, 10);
      strcpy (colortype->color_list [colortype->num_colors].name, "white");
      /* normalize the colors */
      colortype->num_colors += 1;
  }
  

  /* now we are going to get the pixel value for each entry in the list */

  /* dd: should not be made at this stage, but I don't want to debug 
     this demo during one week */

  for (i = 0; i < colortype->num_colors; i++) {
      if (!XParseColor (display, cmap, 
			colortype->color_list [i].name , &exact_def)) {
	  printf ("color %s not in database\n", 
		  colortype->color_list [i].name);
	  for (j = i; j < colortype->num_colors-1; j++) 
	      strcpy(colortype->color_list [j].name,
		     colortype->color_list [j+1].name);
	  colortype->num_colors -- ;
	  i -- ;
      } else {
	  if (!XAllocColor (display, cmap, &exact_def)) {
	      for (j = i; j < colortype->num_colors-1; j++) 
		  strcpy(colortype->color_list [j].name,
			 colortype->color_list [j+1].name);
	      colortype->num_colors -- ;
	      i -- ;
	  } else {
	      colortype->color_list [i].pixel = exact_def.pixel;
	  }
      }
  }

  /* now turn the list into a compound string */

  addrstr = (XmString *) calloc (colortype->num_colors, sizeof (XmString));
  for (i = 0; i < colortype->num_colors; i++) {
      sprintf (buff, "%s, [Pixel = %d]", colortype->color_list [i].name, 
	       colortype->color_list [i].pixel);
      addrstr[i] = XmStringLtoRCreate (buff, XmSTRING_DEFAULT_CHARSET);
  }
  return (addrstr);
}   

/***********************************************************/
SwapTypes (colortype, num_colors)
COLORTYPE *colortype;
int num_colors;
{
  
  int swappos = 0, actpos = 0, hold;
  char buff [50];

  /* want to go down the list of colors and order
     the list by pixel value */

  for (swappos = 0; swappos < num_colors; swappos++) {
    for (actpos = swappos+1; actpos < num_colors; actpos ++) {
      if (colortype->color_list [actpos].pixel < 
	  colortype->color_list [swappos].pixel) {
	hold = colortype->color_list [swappos].pixel;
	strcpy (buff, colortype->color_list [swappos].name);

	colortype->color_list [swappos].pixel =
	  colortype->color_list [actpos].pixel;
	strcpy (colortype->color_list [swappos].name, 
		colortype->color_list [actpos].name);

	colortype->color_list [actpos].pixel = hold;
	strcpy (colortype->color_list [actpos].name, buff);
      }
    }
  }
}

