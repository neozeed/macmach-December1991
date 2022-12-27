/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
*  (c) Copyright 1989 Mitch Trachtenberg
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

/*
** MRE - Motif Resource Editor
**
** Revision history: 
**  created June 1989 Mitch Trachtenberg
**
*/

#define MAIN
#include "mre.h"
#include <Xm/CutPaste.h>
#define MRE_CLIP_STYLE "STRING"


int    type_flag; /* indicates currently displayable resource types */
static int last_selected_item;
static MrmCode		class ;
static Widget toplevel;              /* shell returned by XtInitialize */
static Widget mre_main;              /* main window of application */
static char  *global_current_file;   
static Boolean file_needs_saving;

static void set_generic_resource();
static void set_file_resource();
static void set_enum_resource();
static void select_generic_resource();
static void select_file_resource();
static void select_enum_resource();
static void enumeration_chosen();
static void range_chosen();
static void flip_boolean();
static void _edit_resource();

static void view_sort();
static void set_file_resource();
static void set_generic_resource();
static void prepare_main_selection_box();
static void main_restriction();
static void main_configure();
static void ok_button_pressed();
static void cancel_button_pressed();
static void file_save_proc();
static void file_save_as_proc();
static void file_new_proc();
static void file_new_confirmed();
static void file_open_proc();
static void file_open_confirmed();
static void open_the_file();
static void save_the_file();
static void write_listbox_to_process();
static void write_selection_to_process();
static void edit_paste_proc();
static void edit_copy_proc();
static void command_toggle_proc();
static void command_issue_proc();
static void edit_resource_cb();
static void delete_resource();
static void add_resource();
static void help_proc();
static void confirm_exit();
static void cancel_selection_box();
static void exit_app();
static void popup_popup_menu();
static void action_print_hierarchy();
static void enumeration_chosen();
static void destroy_self();
static void set_enum_resource();
static void dump_hierarchy();
static void system_cb();
extern void color_selection_made();

static char		*uid_files[]={"mre.uid","mre_menu.uid"};
static int num_uid_files = sizeof(uid_files) / sizeof(char *);


static MrmRegisterArg	callbackvec[] = {
  {"setbg_button_pressed",(caddr_t)setbg_button_pressed},
  {"setcolor_button_pressed",(caddr_t)setcolor_button_pressed},
  {"color_selection_made",(caddr_t)color_selection_made},
  {"create_colorform",(caddr_t)create_colorform},
  {"destroy_colorform",(caddr_t)destroy_colorform},
  {"destroy_parent",(caddr_t)destroy_parent},
  {"destroy_self",(caddr_t)destroy_self},
  {"set_file_resource",(caddr_t)set_file_resource},
  {"set_generic_resource",(caddr_t)set_generic_resource},
  {"view_sort",(caddr_t)view_sort},
  {"display_one_font",(caddr_t)display_one_font},
  {"prepare_main_selection_box",(caddr_t)prepare_main_selection_box},
  {"main_restriction",(caddr_t)main_restriction},
  {"prepare_font_selection_box",(caddr_t)prepare_font_selection_box},
  {"restrict_point_size",(caddr_t)restrict_point_size},
  {"restrict_family",(caddr_t)restrict_family},
  {"restrict_weight",(caddr_t)restrict_weight},
  {"restrict_slant",(caddr_t)restrict_slant},
  {"cancel_button_pressed",(caddr_t)cancel_button_pressed},
  {"display_fonts_from_list",(caddr_t)display_fonts_from_list},
  {"font_refilter",(caddr_t)font_refilter},
  {"font_select_button",(caddr_t)font_select_button},
  {"ok_button_pressed",(caddr_t)ok_button_pressed},
  {"command_toggle_proc",(caddr_t)command_toggle_proc},
  {"command_issue_proc",(caddr_t)command_issue_proc},
  {"enumeration_chosen",(caddr_t)enumeration_chosen},
  {"set_enum_resource",(caddr_t)set_enum_resource},
  {"edit_copy_proc",(caddr_t)edit_copy_proc},
  {"edit_paste_proc",(caddr_t)edit_paste_proc},
  {"save_the_file",(caddr_t)save_the_file},
  {"write_listbox_to_process",(caddr_t)write_listbox_to_process},
  {"write_selection_to_process",(caddr_t)write_selection_to_process},
  {"file_save_proc",(caddr_t)file_save_proc},
  {"file_save_as_proc",(caddr_t)file_save_as_proc},
  {"file_new_proc",(caddr_t)file_new_proc},
  {"file_new_confirmed",(caddr_t)file_new_confirmed},
  {"file_open_proc",(caddr_t)file_open_proc},
  {"file_open_confirmed",(caddr_t)file_open_confirmed},
  {"open_the_file",(caddr_t)open_the_file},
  {"edit_resource",(caddr_t)edit_resource_cb},
  {"add_resource",(caddr_t)add_resource},
  {"delete_resource",(caddr_t)delete_resource},
  {"scale_value_changed",(caddr_t)scale_value_changed},
  {"load_rgb_text",(caddr_t)load_rgb_text},
  {"help_proc",(caddr_t)help_proc},
  {"confirm_exit",(caddr_t)confirm_exit},
  {"cancel_selection_box",(caddr_t)cancel_selection_box},
  {"kill_global",(caddr_t)kill_global},
  {"dump_db",(caddr_t)dump_db},
  {"dump_hierarchy",(caddr_t)dump_hierarchy},
  {"system_cb",(caddr_t)system_cb},
  {"unmanage_parent",(caddr_t)unmanage_parent},
  {"exit",(caddr_t)exit_app}
};
static MrmCount callbacknum = sizeof(callbackvec) / sizeof(MrmRegisterArg);

static XtActionsRec actions[] = {
    {"popup_popup_menu", (XtActionProc) popup_popup_menu},
    {"print_hierarchy",(XtActionProc) action_print_hierarchy}
};
static int num_actions = sizeof(actions) / sizeof(XtActionsRec);


static XtResource resources[] = {
  { XtNfontText, XtCFontText, XtRString, sizeof(String),
    XtOffset(ApplicationDataPtr, font_text), XtRString, "This is a sample." },
  { XtNrgbTextFile, XtCRgbTextFile, XtRString, sizeof(String),
 XtOffset(ApplicationDataPtr,rgb_text_file),XtRString, "/usr/local/lib/X11/rgb.txt"},
  { XtNmreTypeFile, XtCMreTypeFile, XtRString, sizeof(String),
    XtOffset(ApplicationDataPtr,mre_type_file),XtRString, "mre_types"},
  { XtNprintHierarchy, XtCPrintHierarchy, XtRBoolean, sizeof(Boolean),
    XtOffset(ApplicationDataPtr,print_hierarchy),XtRString, "false"},
  { XtNmergeOnSave, XtCMergeOnSave, XtRBoolean, sizeof(Boolean),
    XtOffset(ApplicationDataPtr,merge_on_save),XtRString, "false"},
  { XtNinternalPixmaps, XtCInternalPixmaps, XtRBoolean, sizeof(Boolean),
    XtOffset(ApplicationDataPtr,internal_pixmaps),XtRString, "false"}
};

#define arrows_width 32
#define arrows_height 32
static char arrows_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0xfe, 0x7f, 0x00, 0x80, 0xfe, 0x3f, 0x00, 0xc0,
   0xfe, 0x1f, 0x00, 0xe0, 0xfe, 0x0f, 0x00, 0xf0, 0xfe, 0x07, 0x00, 0xf8,
   0xfe, 0x07, 0x00, 0xfc, 0xfe, 0x07, 0x00, 0xfe, 0xfe, 0x0f, 0x00, 0xff,
   0xfe, 0x1f, 0x80, 0xff, 0xfe, 0x3f, 0xc0, 0xff, 0xbe, 0x7f, 0xe0, 0xff,
   0x1e, 0xff, 0xf0, 0xff, 0x0e, 0xfe, 0xf9, 0xff, 0x06, 0xfc, 0xfb, 0xff,
   0x02, 0xf8, 0xf1, 0xff, 0x00, 0xf0, 0xe0, 0xff, 0x00, 0x60, 0xc0, 0xbf,
   0x00, 0xe0, 0x80, 0x9f, 0x00, 0xf0, 0x01, 0x8f, 0x00, 0xf8, 0x03, 0x86,
   0x00, 0xfc, 0x07, 0x80, 0x00, 0xfe, 0x0f, 0x80, 0x00, 0xff, 0x1f, 0x80,
   0x80, 0xff, 0x3f, 0x80, 0xc0, 0xff, 0x3f, 0x80, 0xe0, 0xff, 0x1f, 0x80,
   0xf0, 0xff, 0x0f, 0x80, 0xf8, 0xff, 0x07, 0x80, 0xfc, 0xff, 0x03, 0x80,
   0xfe, 0xff, 0x01, 0x80, 0xff, 0xff, 0xff, 0xff};
#define palette_width 32
#define palette_height 32
static char palette_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x3f, 0x00,
   0x00, 0x06, 0xc0, 0x00, 0x80, 0x81, 0x01, 0x01, 0x40, 0xe0, 0x01, 0x02,
   0x30, 0xe0, 0x01, 0x01, 0x10, 0xc2, 0xe1, 0x00, 0x88, 0x03, 0x18, 0x00,
   0x84, 0x07, 0x0c, 0x00, 0x04, 0x07, 0x04, 0x00, 0x02, 0xc0, 0x09, 0x3f,
   0x02, 0x20, 0xf2, 0x40, 0x62, 0x10, 0x02, 0x80, 0xe2, 0x10, 0x02, 0x87,
   0xe2, 0x10, 0x01, 0x8f, 0xe2, 0xe0, 0x00, 0x9f, 0x02, 0x00, 0x00, 0x9c,
   0x02, 0x00, 0x10, 0x98, 0x04, 0x08, 0xf0, 0x80, 0x04, 0x8f, 0xe1, 0x41,
   0x08, 0x8f, 0xe3, 0x41, 0x10, 0x8e, 0x03, 0x20, 0x20, 0x0c, 0x07, 0x10,
   0x40, 0x00, 0x00, 0x08, 0x80, 0x01, 0x00, 0x04, 0x00, 0x06, 0x80, 0x03,
   0x00, 0xf8, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
#define letter_A_width 32
#define letter_A_height 32
static char letter_A_bits[] = {
   0x00, 0xc0, 0xe0, 0xff, 0x00, 0xc0, 0xe0, 0xff, 0x00, 0xa0, 0xc1, 0xff,
   0x00, 0xa0, 0xc1, 0xff, 0x00, 0xa0, 0xc1, 0xff, 0x00, 0x90, 0x83, 0xff,
   0x00, 0x90, 0x83, 0xff, 0x00, 0x88, 0x83, 0xff, 0x00, 0x88, 0x07, 0xff,
   0x00, 0x88, 0x07, 0xff, 0x00, 0x84, 0x0f, 0xfe, 0x00, 0x84, 0x0f, 0xfe,
   0x00, 0x84, 0x0f, 0xfe, 0x00, 0x82, 0x1f, 0xfc, 0x00, 0x82, 0x1f, 0xfc,
   0x00, 0x81, 0x1f, 0xfc, 0x00, 0x81, 0x01, 0xf8, 0x00, 0x81, 0x01, 0xf8,
   0x80, 0x7f, 0x00, 0xf8, 0x80, 0x80, 0x01, 0xf0, 0x80, 0x80, 0x7f, 0xf0,
   0x40, 0x80, 0x7f, 0xf0, 0x40, 0x80, 0xff, 0xe0, 0x40, 0x80, 0xff, 0xe0,
   0x20, 0x80, 0xff, 0xc1, 0x20, 0x80, 0xff, 0xc1, 0x10, 0x80, 0xff, 0xc1,
   0x10, 0x80, 0xff, 0x83, 0x10, 0x80, 0xff, 0x83, 0x08, 0x80, 0xff, 0x83,
   0x18, 0x80, 0xff, 0x07, 0x7e, 0x80, 0xff, 0x07};


XtAppContext	app_context;
Display*	display;

/*
 *  Main program
 */
int main(argc, argv)
unsigned int argc;
char **argv;
{

    Arg arg[1] ;

    MrmInitialize ();

    /*
     *  Initialize the toolkit.  This call returns the id of a "shell,"
     *  whose only child should be the main window of the application.
     */

    XtToolkitInitialize();
    app_context = XtCreateApplicationContext();
    display = XtOpenDisplay(app_context, NULL, 
		"mre", "resourceeditor",
		NULL, 0,
		&argc, argv);
    if (!display) { printf("Unable to open display\n"); exit(0); }

    XtSetArg (arg[0], XtNallowShellResize, TRUE) ;
    toplevel = XtAppCreateShell("mre", NULL,
			applicationShellWidgetClass,
			display, arg, 1);

    if(argc>1 && argv[1]){global_current_file=strdup(argv[1]);}

    dpy = XtDisplay(toplevel);
    cmap = XDefaultColormap(dpy,0);

    XtGetApplicationResources(toplevel,
			      &data,
			      resources,
			      XtNumber(resources),
			      NULL,
			      0);

    XtAppAddActions(app_context, actions, num_actions);

    /*
     *  Define the Mrm.hierarchy 
     */

    if (MrmOpenHierarchy (num_uid_files,	            /* number of files	    */
			uid_files, 	     	    /* files     	    */
			NULL,			    /* os_ext_list (null)   */
			&s_MrmHierarchy)	    /* ptr to returned id   */
			!= MrmSUCCESS) {
	printf ("MRE: cannot open UID hierarchy.\n\
     Check UIDPATH environment variable\n\
     Goodbye.");
	return(0);
     }

    /*
     * 	Register callback routines 
     */

    if (MrmRegisterNames (callbackvec, callbacknum)
			!= MrmSUCCESS)
			    printf("can't register callback names\n");


    /*
     *  Load the MRE Type File, which provides information necessary
     *  to determine the type of a resource (color, font, etc...)
     */
    load_mre_type_file(data.mre_type_file);

    /*
     *  Fetch the resource editor's main window
     */

    if (MrmFetchWidget (s_MrmHierarchy,
			"mre_main",
			toplevel,
			&mre_main,
			&class)
			!= MrmSUCCESS)
			    printf("can't fetch main window\n");


    /*
     *  Make the toplevel widget "manage" the main window.
     *  It will not be "realized" until the toplevel widget is "realized"
     */

    XtManageChild(mre_main);
    
    /*
     *  Realize the toplevel widget.  This will cause the entire "managed"
     *  widget hierarchy to be displayed.
     */

    XtRealizeWidget(toplevel);
    if(data.print_hierarchy)
      print_hierarchy(toplevel);

  /* load non-UIL bitmaps, if internalPixmaps resource is true */
  if(data.internal_pixmaps){
    Pixmap pix;
    Widget button;
    button = XtNameToWidget(mre_main,
			  "main_form.main_button_box.main_restrict_to_colors");
    pix= XCreatePixmapFromBitmapData(dpy,XtWindow(toplevel),
      palette_bits,palette_width,palette_height,
      WhitePixel(dpy,0),BlackPixel(dpy,0),DefaultDepth(dpy,0));
    XtSetArg(arg[0],XmNlabelPixmap,pix);
    XtSetValues(button,arg,1);

    button = XtNameToWidget(mre_main,
			  "main_form.main_button_box.main_restrict_to_fonts");
    pix= XCreatePixmapFromBitmapData(dpy,XtWindow(toplevel),
      letter_A_bits,letter_A_width,letter_A_height,
      BlackPixel(dpy,0),WhitePixel(dpy,0),DefaultDepth(dpy,0));
    XtSetArg(arg[0],XmNlabelPixmap,pix);
    XtSetValues(button,arg,1);
    button = XtNameToWidget(mre_main,
		  "main_form.main_button_box.main_restrict_to_booleans");
    pix= XCreatePixmapFromBitmapData(dpy,XtWindow(toplevel),
      arrows_bits,arrows_width,arrows_height,
      BlackPixel(dpy,0),WhitePixel(dpy,0),DefaultDepth(dpy,0));
    XtSetArg(arg[0],XmNlabelPixmap,pix);
    XtSetValues(button,arg,1);
  }

    /*
     *  Loop and process events
     */

    XtAppMainLoop(app_context);

    /* UNREACHABLE */
    return (0);
}

/* keep the global variable "last_selected_item" in sync; */
/* this function is attached to the list child of the main selection box */
static void list_selection_made(widget, tag, callback_data)
Widget widget;
char *tag;
XmListCallbackStruct *callback_data;
{
  last_selected_item = callback_data->item_position;
#ifdef DEBUG_CALLBACKS
  printf("Position of selected item is %d.\n",last_selected_item);
#endif
}

/* this function is called from prepare_main_selection_box    */
/* and from file open; it reads the named file (if specified  */
/* or the default file, if no file is specified               */
static Boolean read_file(name)
char *name;
{
  FILE *defaults;
  char buf[160];
  if(!name){
      if(!get_filespec_of_defaults_file(buf))return MRE_FAIL;
      global_current_file = (char *)calloc(sizeof(char),strlen(buf)+1);
      strcpy(global_current_file,buf);
  }
  else strcpy(buf,name);
  if(!(defaults=fopen(buf,"r"))) {
    fetch_message_printf("fail_cant_open",buf);
    return MRE_FAIL;
  }
 read_into_li(defaults);
 fclose(defaults);
 return MRE_OK;
}

/* this function writes the specified file with information from the */
/* specified list widget; it also passes the information being written */
/* to "xrdb -merge", if merge is TRUE. */
static Boolean write_db_to_file(name,list,merge)
char *name;
Widget list;
Boolean merge;
{
  FILE *out;  
  FILE *xrdb;
  Arg arg[2];
  int item_count;
  int n;
  XmString *item_table;
  char *primitive_string;

  if(!(out = fopen(name,"w"))){
    fetch_message_printf("fail_cant_output",name);
    return MRE_FAIL;
  }
  if(merge)
    if(!(xrdb = popen("xrdb -merge","w"))){
      fetch_message_printf("fail_cant_xrdb");
      merge = 0;
    }
  write_db(out);
  fclose(out);  

  if(merge && xrdb){
    write_db(xrdb);
    fclose(xrdb);
  }
  return MRE_OK;
}

/* "file new" should clear out the main selection box's list; */
/* under current snapshots, a completely empty list crashes */
/* so we create a dummy list with one element. */
clear_list(list)
Widget list ;
{
  Arg arg[2];
  int n;
  XmString xms;
  xms = XmStringDefCreate("dummy_spec: value");

  n=0;
  XtSetArg(arg[n],XmNitemCount,1);n++;
  XtSetArg(arg[n],XmNitems,&xms);n++;
  XtSetValues(list,arg,n);
  XmListDeletePos(list,0);
  XmStringFree(xms);
}


/* this is currently tied to the create callback of the selection box, */
/* but will be moved to its map callback when this is feasible */
static void prepare_main_selection_box( widget, tag, callback_data )
Widget	widget;
char    *tag;
XmAnyCallbackStruct *callback_data;
{
  static char *mv[] = {
    "entry_to_file",
    "entry_to_edit",
    "entry_to_view",
    "entry_to_options",
    "entry_to_help",
    "popup_file.file",
    "popup_file.edit",
    "popup_file.view",
    "popup_file.options",
    "popup_file.help",
    "popup_file.view"
  };
  Widget list;
  int n;
#ifdef DEBUG_CALLBACKS
  puts("In prepare main selection box callback.");
#endif 
  main_selection_box_id = widget;
  list = XmSelectionBoxGetChild(widget,XmDIALOG_LIST);
  read_file(global_current_file);
  main_restriction(widget, "A", NULL);
  XtAddCallback(list,XmNbrowseSelectionCallback,list_selection_made,NULL);
  XtAddCallback(list,XmNdefaultActionCallback,list_selection_made,NULL);

  /* register an expose event handler to resize the list; not needed in 1.1 */
  /* XtAddEventHandler(widget,StructureNotifyMask,False,main_configure,NULL);*/
}

static void edit_resource_cb( widget, match, callback_data )
Widget	widget;
int     *match;
XmSelectionBoxCallbackStruct *callback_data;
{
  Arg arg[2];
  int count;
  XmString *xmsp;
  char *sel_text;
  char *text;
  char *temp;
  char *dup_text;

  text = extract_first_xms_segment(callback_data->value);

  if(!(*match)){
    /* check to see if the lhs of the spec matches; */
    /* if so, don't add resource */
    XtSetArg(arg[0],XmNselectedItemCount,&count);
    XtSetArg(arg[1],XmNselectedItems,&xmsp);
    XtGetValues(XmSelectionBoxGetChild(widget,XmDIALOG_LIST),arg,2);
    if(count){
      int lhs_length;
      sel_text = extract_first_xms_segment(*xmsp);
      lhs_length = strchr(sel_text,':') - sel_text;
      /* compare sel_text with text, up to the colon */
      /* if they are different, add */
      if(strncmp(sel_text,text,lhs_length)){
        add_resource(NULL,NULL,NULL);
      }
    }
  }
  /* stamp out any comment prior to passing text in for edit */
  dup_text = strdup(text);
  if(temp = strchr(dup_text,'!')){
    while(isspace(*(--temp)));
    *(temp+1) = '\0';
  }
  _edit_resource(dup_text);
  XtFree(dup_text);
  file_needs_saving = 1;
}

static void _edit_resource(text)
char *text;
{
  int linenum;
  char *valuestring;

  /* dont allow illegal entries */
  if(!has_colon(text) || is_comment(text) || is_hash(text) || is_blank(text)){
    fetch_message_printf("fail_not_resource",text);
    return;
  }

  /* store line_number with dialog, not listindex; listindex changes on sort */
  linenum = linenum_from_listindex(last_selected_item);
  valuestring = skip_to_value(text);
  switch(resource_type(text)){
     case MRE_TYPE_COLOR: 
	if(no_colors_available)
	  select_generic_resource(text,valuestring,linenum);
	else
	  create_colorform(text,valuestring,linenum);
	break;
      case MRE_TYPE_FONT: 
	select_font(text,valuestring,linenum);
	break;
      case MRE_TYPE_FILE:
	select_file_resource(text,valuestring,linenum);
	break;
      case MRE_TYPE_ENUM:
	select_enum_resource(text,valuestring,linenum);
	break;
      case MRE_TYPE_BOOLEAN:
	flip_boolean(
	     XmSelectionBoxGetChild(main_selection_box_id,XmDIALOG_TEXT)
		     );
	break;
      default: 
	select_generic_resource(text,valuestring,linenum);
	break;
      }
}


static void add_resource( widget, tag, callback_data )
Widget	widget;
char    *tag;
XmAnyCallbackStruct *callback_data;
{
  Widget list_widget;
  Widget text_widget;
  Arg arg[1];
  char *text;
  XmString tempstring;

  file_needs_saving = 1;
  list_widget = XmSelectionBoxGetChild(main_selection_box_id,XmDIALOG_LIST);
  text_widget = XmSelectionBoxGetChild(main_selection_box_id,XmDIALOG_TEXT);

  /* eventually, confirm that this item does not duplicate existing resource */
  /* for now, just add it to the list */
  text = (char *)XmTextGetString(text_widget);
  /* dont allow illegal entries */
  if(!has_colon(text)
		|| is_comment(text)
		|| is_hash(text)
		|| is_blank(text))
    return;
  if(resource_type(text)==MRE_TYPE_DONT_SHOW)
    return;
  tempstring = XmStringDefCreate(text);
  XmListAddItem(list_widget,
		tempstring,
		last_selected_item ? last_selected_item : 1);
  /* update listbox indices for everything above this */
  add_line_item(text,last_selected_item);
  XmListSelectPos(list_widget,last_selected_item ? last_selected_item : 1,1);
  /* don't need to increment; the notify takes care of that */
}

static void delete_resource( widget, tag, callback_data )
Widget	widget;
char    *tag;
XmAnyCallbackStruct *callback_data;
{
  Widget list_widget;
  Widget text_widget;
  int n;
  list_widget = XmSelectionBoxGetChild(main_selection_box_id,XmDIALOG_LIST);
  text_widget = XmSelectionBoxGetChild(main_selection_box_id,XmDIALOG_TEXT);

  file_needs_saving = 1;
  XmListDeletePos(list_widget,last_selected_item);
  XmListDeselectAllItems(list_widget);
  XmTextSetString(text_widget," ");
  /* update listbox indices for everything above this, */
  /* set resource_type of this item to indicate deletion */
  /* and set listbox index of this item to zero */
  delete_line_item(last_selected_item);
  last_selected_item = 0;
}

/* function to place popup windows properly */
initialize_coords()
{
  MrmRegisterArg arg[2];
  static Position x,y;
  /* init_red/green/blue are identifiers in the uil module */
  arg[0].name = "stagger_x";
  arg[1].name = "stagger_y";
  if(!x)
    XtTranslateCoords(toplevel,50,50,&x,&y);
  else {
    x+=50;
    y+=50;
  }
  
  arg[0].value = (caddr_t)x;
  arg[1].value = (caddr_t)y;
  if (MrmRegisterNames (arg, 2)
			!= MrmSUCCESS)
			    printf("can't register identifier names\n");

}

/* w update both the main window text and the main window list */
/* with the new text value for the rhs of the resource spec    */
set_values_in_main_window(s,item_to_adjust)
char *s;
int item_to_adjust;
{
  Widget text_widget;
  Widget list_widget;
  Arg arg[2];
  XmString tempstring;
  XmString *items;
  int  selected_item_count;
  char buf[MAX_ENTRY_SIZE];
  char *bufptr;
  char *comment;
  int colon_offset;
  int end_offset;
  char *primitive_string;

  /* the dialog reported the line number in the database; */
  /* we adjust the offset in the listbox or, if item is no longer in list, */
  /* we return without action */
  item_to_adjust = listindex_from_linenum(item_to_adjust);
  if(!item_to_adjust)return;

  text_widget = XmSelectionBoxGetChild(main_selection_box_id,XmDIALOG_TEXT);
  list_widget = XmSelectionBoxGetChild(main_selection_box_id,XmDIALOG_LIST);

 /* get the appropriate item */
  XtSetArg(arg[0],XmNitems,&items);
  XtGetValues(list_widget,arg,1);
  tempstring = XmStringCopy(items[item_to_adjust-1]);
  
  primitive_string = extract_first_xms_segment(tempstring);

  /* find a comment, if any, in the unedited version */
  if(comment = strchr(primitive_string,'!')){
    while(isspace(*(--comment)));
    comment++;
  }

  strcpy(buf,primitive_string);
  bufptr = buf;
  while(*bufptr && (*bufptr!=':'))bufptr++;
  if(*bufptr)bufptr++;
  while(*bufptr==' ')bufptr++;
  strcpy(bufptr,s);

  /* if there was a comment in the entry, append it to the edited version */
  if(comment)strcat(bufptr,comment);

  tempstring = XmStringDefCreate(buf);
  XmListDeletePos(list_widget,item_to_adjust);
  XmListAddItem(list_widget,tempstring,item_to_adjust);
  XmListSelectPos(list_widget,item_to_adjust,1);
  /* free tempstring */
  XmStringFree(tempstring);
}

static void ok_button_pressed( widget, tag, callback_data )
Widget	widget;
char    *tag;
XmAnyCallbackStruct *callback_data;
{
  typedef struct {
    int    item_to_adjust;
    char   colorname[40];
  } *DataPtr;
  DataPtr userdata;
  Arg arg[1];
  int item_to_adjust;
  char *specstring;
  /* we set the current font spec into the main window's selection text */
  /* and update the item in the list box as well */
  /* we then unmanage the color window, which pops its shell down */
  if(*tag==FONT_OK_BUTTON_PRESSED) {
    specstring = get_selected_font_string(widget);
    /* unmanage windows associated with the main font window, if present */
    if(global_one_font)XtUnmanageChild(global_one_font);
    if(font_paned_window)XtUnmanageChild(XtParent(font_paned_window));
    XtSetArg(arg[0],XmNuserData,&item_to_adjust);
    XtGetValues(XtParent(widget),arg,1); 
  }
  else if(*tag==COLOR_OK_BUTTON_PRESSED){
    XtSetArg(arg[0],XmNuserData,&userdata);
    XtGetValues(XtParent(widget),arg,1);
    specstring = userdata->colorname;
    item_to_adjust = userdata->item_to_adjust;
  }
  set_values_in_main_window(specstring,item_to_adjust);
  set_values_in_database(specstring,item_to_adjust);
  XtDestroyWidget(XtParent(widget));
}

static void cancel_button_pressed( widget, tag, callback_data )
Widget	widget;
char    *tag;
XmAnyCallbackStruct *callback_data;
{
  if(*tag==COLOR_OK_BUTTON_PRESSED){
    XtDestroyWidget(XtParent(widget));
  }
  else if(*tag==FONT_OK_BUTTON_PRESSED) {
    XtUnmanageChild(XtParent(widget));
    /* unmanage windows associated with the main font window, if present */
    if(global_one_font)XtUnmanageChild(global_one_font);
    if(font_paned_window)XtUnmanageChild(XtParent(font_paned_window));
  }
}

static void view_sort( widget, tag, callback_data )
Widget	widget;
char    *tag;
XmAnyCallbackStruct *callback_data;
{
  Arg arg[2];
  int n;
  int sort_method;
  XmString *items;
  XmString *new_items;
  int item_count;
  if(!main_selection_box_id){
    fetch_message_printf("fail_missing_main");
    return;
  }
  n = 0;
  XtSetArg(arg[n],XmNlistItems,&items);n++;
  XtSetArg(arg[n],XmNlistItemCount,&item_count);n++;
  XtGetValues(main_selection_box_id,arg,n);

  if(tag[0]=='R'){
    if(tag[1]=='0')
      sort_method = SORT_BY_RESOURCE_DESCENDING;
    else if(tag[1]=='1')
      sort_method = SORT_BY_RESOURCE;
  }
  if(tag[0]=='S'){
    if(tag[1]=='0')
      sort_method = SORT_BY_SPEC_DESCENDING;
    else if(tag[1]=='1')
      sort_method = SORT_BY_SPEC;
  }
  else if(tag[0]=='V'){
    if(tag[1]=='0')
      sort_method = SORT_BY_VALUE_DESCENDING;
    else if(tag[1]=='1')
      sort_method = SORT_BY_VALUE;
  }    
  /* adjust the listbox indices in the li array and build new list */
   sort_li_array(sort_method); 
   gen_new_list(type_flag);
}

void fetch(widget_index_string,widgetptr)
char *widget_index_string;
Widget *widgetptr;
{
  MrmType class;
  *widgetptr = NULL;
  if(MrmFetchWidget(s_MrmHierarchy,
		widget_index_string,
		toplevel,
		widgetptr,
		&class)!=MrmSUCCESS)
		  fetch_message_printf("fail_cant_fetch",widget_index_string);
}

static void file_save_proc(widget,tag,callback_data)
Widget widget;
char *tag;
XmAnyCallbackStruct *callback_data;
{
  if(global_current_file){
    fetch_message_printf("info_saving_file",global_current_file);
    if(MRE_OK==write_db_to_file(global_current_file,NULL,data.merge_on_save))
      file_needs_saving = 0;
  }
  else file_save_as_proc(widget,tag,callback_data);
}

static void file_new_proc(widget,tag,callback_data)
Widget widget;
char *tag;
XmAnyCallbackStruct *callback_data;
{
  Widget fetched;
  fetched = (Widget) 0;
  if(file_needs_saving){
    fetch("new_dialog",&fetched);
    XtManageChild(fetched);
  }
  else 
    file_new_confirmed(widget,tag,callback_data);
}
static void file_new_confirmed(widget,tag,callback_data)
Widget widget;
char *tag;
XmAnyCallbackStruct *callback_data;
{
  /* clear the displayed listbox */
  clear_list(XmSelectionBoxGetChild(main_selection_box_id,XmDIALOG_LIST));

  /* and clear the line information array */
  clear_li();

  /* and clear global_current_file */
  if(global_current_file)
    XtFree(global_current_file);
  global_current_file=NULL;
};

static void open_the_file(widget,tag,callback_data)
Widget widget;
char *tag;
XmFileSelectionBoxCallbackStruct *callback_data;
{
  Widget list;
  char *primitive_string;
  primitive_string = extract_first_xms_segment(callback_data->value);
  list = XmSelectionBoxGetChild(main_selection_box_id,XmDIALOG_LIST);
  read_file(primitive_string);
  main_restriction(widget,"A",NULL);
  if(global_current_file)free(global_current_file);
  global_current_file = (char *)calloc(sizeof(char),
				       strlen(primitive_string)+1);
  strcpy(global_current_file,primitive_string);
  XtDestroyWidget(widget);
}
static void cancel_selection_box(widget,tag,callback_data)
Widget widget;
char *tag;
XmAnyCallbackStruct *callback_data;
{
  XtDestroyWidget(widget);
} 

/* callback procedure attached to menu file save as button */
/* fetches fsb, specifies the current file as dirSpec */
static void file_save_as_proc(widget,tag,callback_data)
Widget widget;
char *tag;
XmAnyCallbackStruct *callback_data;
{
    Widget fetched=(Widget)0;
    fetch("file_save_as_shell",&fetched);
    if(global_current_file){
      Arg arg[1];
      XmString temp;
      temp = XmStringDefCreate(global_current_file);
      XtSetArg(arg[0],XmNdirMask,temp);
      XtSetValues(fetched,arg,1);
      XmStringFree(temp);
    }
    XtManageChild(fetched);
};

/* callback procedure attached to menu file open button */
static void file_open_proc(widget,tag,callback_data)
Widget widget;
char *tag;
XmAnyCallbackStruct *callback_data;
{
  Widget fetched;
  fetched = (Widget) 0;
  if(file_needs_saving){
    fetch("open_dialog",&fetched);
    XtManageChild(fetched);
  }
  else 
    file_open_confirmed(widget,tag,callback_data);
}
static void file_open_confirmed(widget,tag,callback_data)
Widget widget;
char *tag;
XmAnyCallbackStruct *callback_data;
{
    Widget fetched = (Widget) 0;
    fetch("file_open_shell",&fetched);
    XtManageChild(fetched);
}

/*static void file_open_proc(widget,tag,callback_data)
Widget widget;
char *tag;
XmAnyCallbackStruct *callback_data;
{
    Widget fetched=(Widget)0;
    if(file_needs_saving){
      fetch_message_printf("warn_needs_saving",global_current_file);
      return;
    }
    fetch("file_open_shell",&fetched);
    XtManageChild(fetched);
};
*/
static void write_listbox_to_process(widget,proc,callback_data)
Widget widget;
char *proc;
XmAnyCallbackStruct *callback_data;
{
  FILE *xrdb;
  Widget list;
  Arg arg[2];
  int item_count;
  int n;
  XmString *item_table;
  char *primitive_string;

  if(!(xrdb = popen(proc,"w"))){
    fetch_message_printf("fail_cant_open",proc);
    return;
  }
  list = XmSelectionBoxGetChild(main_selection_box_id,XmDIALOG_LIST);

  XtSetArg(arg[0],XmNitemCount,&item_count);
  XtSetArg(arg[1],XmNitems,&item_table);
  XtGetValues(list,arg,2);

  n = 0;
  while(item_count--){
    primitive_string = extract_first_xms_segment(item_table[n++]);
    fprintf(xrdb,"%s\n",primitive_string);
  }
  fclose(xrdb);
}

static void write_selection_to_process(widget,proc,callback_data)
Widget widget;
char *proc;
XmAnyCallbackStruct *callback_data;
{
  FILE *xrdb;
  Widget text_w;

  if(!(xrdb = popen(proc,"w"))){
    fetch_message_printf("fail_cant_open",proc);
    return;
  }
  text_w = XmSelectionBoxGetChild(main_selection_box_id,XmDIALOG_TEXT);
  fprintf(xrdb,"%s\n",XmTextGetString(text_w));
  fclose(xrdb);
}

/* callback procedure attached to the save file fsb's ok callback list */
static void save_the_file(widget, tag, callback_data )
Widget	widget;
char    *tag;
XmFileSelectionBoxCallbackStruct *callback_data;
{
  char buf[160];
  char *primitive_string;
  /* if this function is called by command issue proc, use tag as filename */
  if(!widget)
    primitive_string = tag;
  else 
    primitive_string = extract_first_xms_segment(callback_data->value);

  if(MRE_OK==write_db_to_file(primitive_string,NULL,data.merge_on_save)){
    file_needs_saving=0;
    fetch_message_printf("info_saving_file",primitive_string);
  }

  XtUnmanageChild(widget);
}

CopyToClipboard(time,w,string)
Time time;
Widget w;
char *string;
{
  XmString clip_label;
  long item_id = 0;
  int data_id = 0;
  int status = 0;
  clip_label = XmStringDefCreate("MRE");
  status = 0;
  while(status != ClipboardSuccess)
    status = XmClipboardStartCopy(dpy,XtWindow(w),
				  clip_label,time,w,NULL,&item_id);

  status = 0;
  while(status != ClipboardSuccess)
    status = XmClipboardCopy(dpy,XtWindow(w),item_id,MRE_CLIP_STYLE,
			     string,(long)strlen(string)+1,0,&data_id);

  status = 0;
  while(status != ClipboardSuccess)
    status = XmClipboardEndCopy(dpy,XtWindow(w),item_id);
}

char *GetFromClipboard(w)
Widget w;
{
  int status = 0;
  char *buffer;
  unsigned long length;
  unsigned long outlength = 0;
  int private_id = 0;

  status = 0;
  while(status != ClipboardSuccess)
    status = XmClipboardInquireLength(dpy,XtWindow(w),MRE_CLIP_STYLE,&length);
  if(!length)return(NULL);
  buffer = XtMalloc(length);
  status = XmClipboardRetrieve(dpy,XtWindow(w),"STRING",buffer,length,
			       &outlength,&private_id);
  if(status!=ClipboardSuccess)return(NULL);
  return(buffer);
}





static void edit_copy_proc(widget,cut,callback_data)
Widget widget;
int *cut;
XmAnyCallbackStruct *callback_data;
{
  Arg arg[2];
  XmString *items;
  Widget list_w;
  Widget text_w;
  int item_count;
  char *str;
  /* get the current selection from the listbox */
  list_w = XmSelectionBoxGetChild(main_selection_box_id,XmDIALOG_LIST);
  text_w = XmSelectionBoxGetChild(main_selection_box_id,XmDIALOG_TEXT);
  XtSetArg(arg[0],XmNselectedItemCount,&item_count);
  XtSetArg(arg[1],XmNselectedItems,&items);
  XtGetValues(list_w,arg,2);
  if(!item_count)return;
  str = extract_first_xms_segment(*items);
  if(!strlen(str))
    return;
  CopyToClipboard(callback_data->event->xbutton.time,toplevel,str);
  if(*cut){
    XmListDeleteItem(list_w,*items);
    delete_line_item(last_selected_item);
    XmListSelectPos(list_w,last_selected_item,1);
    file_needs_saving = 1;
  }
};

static void edit_paste_proc(widget,tag,callback_data)
Widget widget;
caddr_t tag;
XmAnyCallbackStruct *callback_data;
{
  Widget text_w;
  char *string;
  text_w = XmSelectionBoxGetChild(main_selection_box_id,XmDIALOG_TEXT);
  string = GetFromClipboard(toplevel);
  if(!string)return;
  XmTextSetString(text_w,string);
  add_resource(NULL,NULL,NULL);
  XtFree(string);
}

/* callback procedure to display exit confirmation dialog box */
static void confirm_exit()
{
    static Widget fetched;
    if(!file_needs_saving)
      exit_app();
    fetch("exit_dialog",&fetched);
    XtManageChild(fetched);
}


/* callback procedure to exit application */
static void exit_app()
{
  exit();
}

/* main_configure not needed in 1.1 */
#ifdef NOTDEF
/* this function is an event handler, not a callback */
static void main_configure( widget, client_data, event )
Widget	widget;
caddr_t client_data;
XEvent *event;
{
  Arg arg[1];
  Dimension height;
  Widget list;
  Widget hscroll;
  static int n;
  /* determine height of main_selection_box */
  XtSetArg(arg[0],XmNheight,&height); 
  XtGetValues(widget,arg,1);

  /* set height of scrolled list, allowing 150 pixels */
  /* for non-list part of selection box */
  height = (height>150) ? (height-150) : 2;
  list = XmSelectionBoxGetChild(widget, XmDIALOG_LIST);
  hscroll = XtNameToWidget(XtParent(list),"ListhScrollBar");
  if(hscroll && XtIsManaged(hscroll))
      height = (height>30) ? (height-30) : 2;
  XtSetArg(arg[0],XmNlistVisibleItemCount,height/15);
  XtSetValues(widget,arg,1);
}  
#endif
/* no longer needed in 1.1 */
/* note that because this callback may be called by non-toggles, */
/* we cannot rely on the callback data field for toggle info */
/* (this is to prevent multiple reanalyses of the db */
static void main_restriction( widget, tag, callback_data )
Widget	widget;
char    *tag;
XmAnyCallbackStruct *callback_data;
{
  static Boolean entered_already;
  Widget list;
  Widget button;
  Widget view;
  char str[150];
  if(!entered_already++){
    type_flag = 0x2F;
  }
  list = XmSelectionBoxGetChild(main_selection_box_id,XmDIALOG_LIST);
  /* toggle the appropriate restriction based on *tag, */
  if(*tag=='t'){
    if(*(tag+1)=='C')type_flag^=1;
    if(*(tag+1)=='F')type_flag^=2;
    if(*(tag+1)=='B')type_flag^=4;
    if(*(tag+1)=='I')type_flag^=8;
    if(*(tag+1)=='T')type_flag^=0x10;
    if(*(tag+1)=='O')type_flag^=0x20;
  }
  else if(*tag=='A'){
    type_flag=0x2F;
    view = XtNameToWidget(mre_main,"main_menu_bar.popup_file.view");
    if(view){
      button = XtNameToWidget(view,"view_colors");
      if(button)XmToggleButtonSetState(button,1,0);
      button = XtNameToWidget(view,"view_fonts");
      if(button)XmToggleButtonSetState(button,1,0);
      button = XtNameToWidget(view,"view_booleans");
      if(button)XmToggleButtonSetState(button,1,0);
      button = XtNameToWidget(view,"view_misc");
      if(button)XmToggleButtonSetState(button,1,0);
      button = XtNameToWidget(view,"view_files");
      if(button)XmToggleButtonSetState(button,1,0);
      button = XtNameToWidget(view,"view_translations");
      if(button)XmToggleButtonSetState(button,1,0);
    }
  }
  else {
    view = XtNameToWidget(mre_main,"main_menu_bar.popup_file.view");
    if(view){
      button = XtNameToWidget(view,"view_fonts");
      if(button)XmToggleButtonSetState(button,0,0);
      button = XtNameToWidget(view,"view_colors");
      if(button)XmToggleButtonSetState(button,0,0);
      button = XtNameToWidget(view,"view_booleans");
      if(button)XmToggleButtonSetState(button,0,0);
      button = XtNameToWidget(view,"view_misc");
      if(button)XmToggleButtonSetState(button,0,0);
      button = XtNameToWidget(view,"view_files");
      if(button)XmToggleButtonSetState(button,0,0);
      button = XtNameToWidget(view,"view_translations");
      if(button)XmToggleButtonSetState(button,0,0);
      button = (Widget)0;
    }
    if (*tag=='C'){
      type_flag=1;
      button = XtNameToWidget(view,"view_colors");
    }
    if (*tag=='F'){
      type_flag=2;
      button = XtNameToWidget(view,"view_fonts");
    }
    if (*tag=='B'){
      type_flag=4;
      button = XtNameToWidget(view,"view_booleans");
    }
    if (*tag=='I'){
      type_flag=8;
      button = XtNameToWidget(view,"view_files");
    }
    if (*tag=='T'){
      type_flag=0x10;
      button = XtNameToWidget(view,"view_translations");
    }
    if (*tag=='0'){
      type_flag=0x20;
      button = XtNameToWidget(view,"view_misc");
    }
    if(button)XmToggleButtonSetState(button,1,0);
  }
  /* then call routine to rebuild list box based on type */
  gen_new_list(type_flag);
}

static void command_toggle_proc( widget, tag, callback_data )
Widget	widget;
char    *tag;
XmToggleButtonCallbackStruct *callback_data;
{
  Widget command;
  command=XtNameToWidget(mre_main,"main_command");
  if(!command)return;
  if(!callback_data->set)
    XtUnmanageChild(command);
  else 
    XtManageChild(command);
}

static void command_issue_proc( widget, tag, callback_data )
Widget	widget;
char    *tag;
XmCommandCallbackStruct *callback_data;
{
  char *command;
  char *dup;
  char *tok;
  XmString t;
  command = extract_first_xms_segment(callback_data->value);
  dup = strdup(command);

  /* use upper case tokens */
  for(tok=dup;*tok;tok++)
    *tok=toupper(*tok);

  tok=strtok(dup," \t\n");

  if(!stricmp(tok,"SORT")){
    tok=strtok(NULL," \t\n");
    /* allow noise word */
    if(!stricmp(tok,"BY"))
      tok=strtok(NULL," \t\n");
    if(tok && !strncmp(tok,"SPEC",4)){
      tok = strtok(NULL," \t\n");
      if(tok && !strncmp(tok,"ASC",1))
        view_sort(NULLW,"S1",NULLCB);
      else if(!tok || !strncmp(tok,"DES",1))
	view_sort(NULLW,"S0",NULLCB);
    }
    else if(tok && !strncmp(tok,"VAL",3)){
      tok = strtok(NULL," \t\n");
      if(tok && !strncmp(tok,"ASC",1))
        view_sort(NULLW,"V1",NULLCB);
      else if(!tok || !strncmp(tok,"DES",1))
	view_sort(NULLW,"V0",NULLCB);
    }
    else if(tok && !strncmp(tok,"RES",3)){
      tok = strtok(NULL," \t\n");
      if(tok && !strncmp(tok,"ASC",1))
        view_sort(NULLW,"R1",NULLCB);
      else if(!tok || !strncmp(tok,"DES",1))
	view_sort(NULLW,"R0",NULLCB);
    }
    else {
      XmCommandError(widget,t=XmStringDefCreate("Error!"));
      XmStringFree(t);
    }
  }/* end sort */
  else if(!stricmp(tok,"EXIT")){
    confirm_exit();
  }
  else if(!stricmp(tok,"SAVE")){
    tok=strtok(NULL," \t\n");
    if(!tok)
      file_save_proc();
    else if(!strncmp(tok,"AS",2) && NULL!=(tok=strtok(NULL," \t\n"))){
      /* locate the uncapitalized version of the filename */
      if((tok - dup)>0)
        save_the_file(NULL,command+(tok-dup),NULL);
      else
	save_the_file(NULL,tok,NULL);
    }
  }
  else if(!stricmp(tok,"HELP")){
    tok=strtok(NULL," \t\n");
    if(!tok)
      help_proc(NULL,"Main",NULL);
    else 
      help_proc(NULL,tok,NULL);
  }
  else {
    XmCommandError(widget,t=XmStringDefCreate("Unrecognized command!"));
    XmStringFree(t);
  }
}

static XmString MRE_xms_resource_only(s)
char *s;
{
  char *dup;
  char *colon;
  XmString xms;
  dup = strdup(s);
  colon = strchr(dup,':');
  if(colon)*colon='\0';
  xms=XmStringDefCreate(dup);
  free(dup);
  return(xms);
}
/* the generic resource dialog sets the resource name into the dialog's */
/* label widget and the resource's value into the dialog's text widget */
static void select_generic_resource( text, value, item_to_adjust )
char    *text;
char    *value;
caddr_t item_to_adjust;
{
    Widget fetched;
    Arg arg[1];
    XmString t;
    
    fetched=(Widget)0;
    fetch("generic_dialog",&fetched);

    /* store as userData the index into the selection box item list */
    XtSetArg(arg[0],XmNuserData,item_to_adjust);
    XtSetValues(fetched,arg,1);

    XtSetArg(arg[0],XmNlabelString,t=MRE_xms_resource_only(text));
    XtSetValues(XmSelectionBoxGetChild(fetched,XmDIALOG_SELECTION_LABEL),
		arg,1);
    XmStringFree(t);

    XmTextSetString(XmSelectionBoxGetChild(fetched,XmDIALOG_TEXT),value);
    XtManageChild(fetched);
}

/* the enumerated resource dialog fills its fetched rowcolumn widget */
/* with toggle buttons representing valid enumerated choices */
static void select_enum_resource( text, value, item_to_adjust )
char    *text;
char    *value;
caddr_t item_to_adjust;
{
    Widget fetched;
    Widget rc_w;
    Widget label_w;
    Arg arg[1];
    
    fetched = (Widget)0;
    fetch("enumerated_dialog",&fetched);

    XtSetArg(arg[0],XmNuserData,item_to_adjust);
    XtSetValues(fetched,arg,1);

    label_w=XtNameToWidget(fetched,"ed_label");
    if(label_w){
      XmString t;
      XtSetArg(arg[0],XmNlabelString,t=MRE_xms_resource_only(text));
      XtSetValues(label_w,arg,1);
      XmStringFree(t);
    }
    rc_w=XtNameToWidget(fetched,"ed_rc");
    if(rc_w){
      char *tok;
      char *choices;
      /* add radio buttons */
      if(choices = enumeration_choices(item_to_adjust)){
	tok = strtok(strdup(choices),";");
	/* if all digits and dashes, build a range */
	if(strchr(choices,'-')){
	  Arg scale_arg[6];
	  Widget scale;
	  XtSetArg(scale_arg[0],XmNorientation , XmHORIZONTAL);
	  XtSetArg(scale_arg[1],XmNmaximum ,atoi(strchr(choices,'-')+1));
	  XtSetArg(scale_arg[2],XmNminimum ,atoi(choices));
	  XtSetArg(scale_arg[3],XmNvalue ,atoi(value));
	  XtSetArg(scale_arg[4],XmNshowValue ,1);
	  XtSetArg(scale_arg[5],XmNprocessingDirection ,XmMAX_ON_RIGHT);
	  XtManageChild(scale=XmCreateScale(rc_w,choices,scale_arg,6));
          XtAddCallback(scale,XmNvalueChangedCallback,range_chosen,NULL);
	}
	else{
          do{
	    Widget toggle;
	    XtManageChild(toggle=XmCreateToggleButtonGadget(rc_w,tok,NULL,0));
	    /* set the toggle button if its name matches the current value */
	    if(!strncmp(tok,value,strlen(tok)))
	      XmToggleButtonGadgetSetState(toggle,1,1);
          }while(tok = strtok(NULL,";"));
	}
      }
    }
    XtManageChild(fetched);
}

static void system_cb(widget, tag, callback_data)
Widget widget;
caddr_t tag;
XmAnyCallbackStruct *callback_data;
{
  system(tag);
}
static void select_file_resource (text, value, item_to_adjust )
char    *text;
char    *value;
caddr_t item_to_adjust;
{
    Widget fetched;
    Arg arg[2];
    char *lastslash;
    char *dup;
    XmString t;
    fetched=(Widget)0;
    fetch("file_resource_shell",&fetched);

    /* store as userData the index into the selection box item list */
    XtSetArg(arg[0],XmNuserData,item_to_adjust);
    XtSetArg(arg[1],XmNselectionLabelString,t=MRE_xms_resource_only(text));
    XtSetValues(fetched,arg,2);
    XmStringFree(t);

    dup=strdup(value);
    if(lastslash=strrchr(dup,'/')){
      *lastslash='\0';
      XtSetArg(arg[0],XmNdirMask,XmStringDefCreate(dup));
      XtSetValues(fetched,arg,1);
    }
    free(dup);

    /* a second update is necessary to get the spec; they cannot be combined */
    /* in the GA */
    XtSetArg(arg[0],XmNdirSpec,XmStringDefCreate(value));
    XtSetValues(fetched,arg,1);
    XtManageChild(fetched);
}

static void set_file_resource( widget, tag, callback_data)
Widget	widget;
char    *tag;
XmFileSelectionBoxCallbackStruct *callback_data;
{
   char *specstring;
   int item_to_adjust;
   Arg arg[1];
   specstring = extract_first_xms_segment(callback_data->value);
   XtSetArg(arg[0],XmNuserData,&item_to_adjust);
   XtGetValues(widget,arg,1);
   if(item_to_adjust>500){
     printf("Item to adjust, %d, is invalid.\n",item_to_adjust);
     XtDestroyWidget(widget);
     return;
   };
   set_values_in_main_window(specstring,item_to_adjust);
   set_values_in_database(specstring,item_to_adjust);
   XtDestroyWidget(widget);
}
static void set_generic_resource( widget, tag, callback_data)
Widget	widget;
char    *tag;
XmAnyCallbackStruct *callback_data;
{
   char *specstring;
   int item_to_adjust;
   Widget text_w;
   char *str;
   Arg arg[1];

   if(!(text_w = XmSelectionBoxGetChild(widget,XmDIALOG_TEXT)))return;
   specstring = XmTextGetString(text_w);
   XtSetArg(arg[0],XmNuserData,&item_to_adjust);
   XtGetValues(widget,arg,1);
   set_values_in_main_window(specstring,item_to_adjust);
   set_values_in_database(specstring,item_to_adjust);
   XtDestroyWidget(widget);
}

static void set_enum_resource( widget, tag, callback_data)
Widget	widget;
char    *tag;
XmAnyCallbackStruct *callback_data;
{
   char *specstring;
   int item_to_adjust;
   Widget form;
   Widget text_w;
   char *str;
   Arg arg[1];

   if(!(form=XtParent(widget)))return;
   if(!(text_w = XtNameToWidget(form,"ed_text")))return;
   specstring = XmTextGetString(text_w);
   XtSetArg(arg[0],XmNuserData,&item_to_adjust);
   XtGetValues(XtParent(widget),arg,1);
   set_values_in_main_window(specstring,item_to_adjust);
   set_values_in_database(specstring,item_to_adjust);
   XtDestroyWidget(XtParent(widget));
}

static void enumeration_chosen(widget,tag,callback_data)
Widget widget;
caddr_t tag;
XmRowColumnCallbackStruct *callback_data;
{
  Arg arg[1];
  XmString xms;
  char *s;
  XtSetArg(arg[0],XmNlabelString,&xms);
  XtGetValues(callback_data->widget,arg,1);
  
  s=extract_first_xms_segment(xms);
  XmTextSetString(XtNameToWidget(XtParent(widget),"ed_text"),s);
}

static void range_chosen(widget,tag,callback_data)
Widget widget;
caddr_t tag;
XmScaleCallbackStruct *callback_data;
{
  char buf[10];
  sprintf(buf,"%d",callback_data->value);
  XmTextSetString(XtNameToWidget(XtParent(XtParent(widget)),"ed_text"),buf);
}


static void flip_boolean(widget)
Widget widget;
{
  static char *bools[]={
    "TRUE","FALSE","YES","NO","ON","OFF",
    "True","False","Yes","No","On","Off",
    "true","false","yes","no","on","off"};
  int n;
  char buf[MAX_ENTRY_SIZE];
  char *text;
  char *valuestring;
  int  linenum;
  text = XmTextGetString(widget);
  linenum = linenum_from_listindex(last_selected_item);
  strcpy(buf,text);
  if(!(valuestring = strchr(buf,(int)':'))){
    fetch_message_printf("fail_not_resource",buf);
    return;
  }
  while(isspace(*++valuestring));
  /* find first match; if match offset is odd, replace with prev; else next */
  for(n=0;n<XtNumber(bools);n++){
    if(!strcmp(valuestring,bools[n])){
      strcpy(valuestring,bools[(n&1) ? n-1 : n+1]);
      break;
    }
  }
  XmTextSetString(widget,buf);
  set_values_in_main_window(valuestring,linenum);
  set_values_in_database(valuestring,linenum);
}


static void destroy_self(widget,tag,callback_data)
Widget	widget;
char    *tag;
XmAnyCallbackStruct *callback_data;
{
  XtDestroyWidget(widget);
}

void destroy_parent(widget,tag,callback_data)
Widget	widget;
char    *tag;
XmAnyCallbackStruct *callback_data;
{
  XtDestroyWidget(XtParent(widget));
}

static void help_proc( widget, tag, callback_data )
Widget	widget;
char    *tag;
XmAnyCallbackStruct *callback_data;
{
    Widget fetched;
    char buf[80];
    int n;

    if(!strcmp("Version",tag)){
      print_environment_info();
      return;
    }

    fetched = (Widget)0;
    fetch("help_shell",&fetched);

    /* eventually, open a file keyed to tag; now just display tag */
    strcpy(buf,"Help on...\n");
    strcat(buf,tag);
    XmTextSetString(XtNameToWidget(fetched,"help_stextSW.help_stext"),buf);
    XtManageChild(fetched);
}

/* action procedure to popup the color form's popup menu */
static void popup_popup_menu(widget, event, params, num_params)
Widget                      widget;
XButtonPressedEvent *       event;
char **                     params;
int                         num_params;
{
    static Widget fetched;
    /* store id of widget triggering popup */
    /* to assist in interpreting button functions */
    popped_by = widget;
    fetch("popup_menu_widget",&fetched);
    XmMenuPosition(fetched, event);  
    XtManageChild(fetched);
}

/* action procedure to call the debugging routine print_hierarchy */
static void action_print_hierarchy(widget, event, params, num_params)
Widget                      widget;
XButtonPressedEvent *       event;
char **                     params;
int                         num_params;
{
  if(widget)
    print_hierarchy(widget);
}

static void dump_hierarchy( widget, tag, callback_data )
Widget	widget;
char    *tag;
XmAnyCallbackStruct *callback_data;
{
  if(toplevel)
    print_hierarchy(toplevel);
}





















