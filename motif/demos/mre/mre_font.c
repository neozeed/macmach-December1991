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
** mre_font.c - Motif Resource Editor Font Handling 
**
**
** Revision history: 
**  created July 1989 Mitch Trachtenberg
**
**
*/

#include "mre.h"

#define FONT_VENDOR    0
#define FONT_FOUNDRY   1
#define FONT_FAMILY    2
#define FONT_WEIGHT    3
#define FONT_SLANT     4
#define FONT_POINTSIZE 7

#define MAX_FONT_STRING_SIZE 150
#define F_C_SIZE 30
#define F_C_COUNT 9
static Widget global_font_selection_box_id;
static Widget global_font_spec_mask;
static char   font_string_fragments[F_C_COUNT][F_C_SIZE] = 
    {"*","*","Times","*","*","*","*","12","*"};
static char   font_string[MAX_FONT_STRING_SIZE];

static void initialize_font();
static void font_add_to_paned();
static char *get_font_spec_string();
static char *set_font_component();
static char *get_font_component();
static void set_radio_box_item();

char *get_selected_font_string();
void display_one_font();
void select_font();

#ifdef notdef
static void initialize_font();
#endif

extern void fetch();

/* retrieved from uil */
char **spec_components;

void select_font( text, value, item_to_adjust )
char    *text;
char    *value;
caddr_t item_to_adjust;
{
    static Widget fetched;
    Arg arg[1];
    MrmCode return_type;

    initialize_coords();

    fetched = 0;
    fetch("font_dialog",&fetched);

    XtSetArg(arg[0],XmNuserData,item_to_adjust);
    XtSetValues(fetched,arg,1);

    XtUnmanageChild(
      XmSelectionBoxGetChild(XtNameToWidget(fetched,"font_selection_box"),
			     XmDIALOG_APPLY_BUTTON)
    );
    global_font_spec_mask = XtNameToWidget(fetched,"font_spec_mask");

    if(MrmFetchLiteral(s_MrmHierarchy,
			"spec_components",
			dpy,
			(caddr_t *)&spec_components,
			&return_type) != MrmSUCCESS)
    fetch_message_printf("fail_cant_find_radiobox");

    initialize_font(fetched,value);
    XtManageChild(fetched);
}



static void initialize_font ( font_dialog_box,valuestring )
Widget font_dialog_box;
char *valuestring;
{
    char **names_ret;
    int num_names_ret;
    XmString *items;    
    int item_count;
    Arg arg[2];
    Widget list;
    Widget text;
    int max_ret = 50;
    Boolean	font_valid;

    /* set font spec to current resource value */
    if(global_font_spec_mask)
      XmTextSetString(global_font_spec_mask,get_font_spec_string());

    /* filter to match the extracted components only */
    names_ret = XListFonts(dpy,get_font_spec_string(),max_ret,&num_names_ret);
    list = XmSelectionBoxGetChild(global_font_selection_box_id,XmDIALOG_LIST);

    /* Make sure the font is valid before attempting to set radio boxes */
    if(num_names_ret > 0)
      font_valid = True;
    else
      font_valid = False;
    
    item_count = num_names_ret;
    items = (XmString *)calloc(num_names_ret,sizeof(XmString));

    while(num_names_ret--){
      items[num_names_ret]=XmStringDefCreate(names_ret[num_names_ret]);
    }
    XtSetArg(arg[0],XmNitems,items);
    XtSetArg(arg[1],XmNitemCount,item_count);
    XtSetValues(list,arg,2);
    while(item_count--)
      XmStringFree(items[item_count]);

    text = XmSelectionBoxGetChild(global_font_selection_box_id,XmDIALOG_TEXT);
    XmTextSetString(text,valuestring);
    
    /* set the radio boxes to match the current resource value */
    /* by extracting components from the spec, if possible */
    if(font_valid && (*valuestring=='-')){ /* presume to be in proper format */
      char buf[F_C_SIZE];
      char *component;
      int n;
      /* copy components into local, NULL terminated buffer */
      component = get_font_component(FONT_SLANT,valuestring);
      for(n=0;n<F_C_SIZE && component&& component[n]!='-';n++)
	buf[n]=component[n];
      buf[n]='\0';
      set_font_component(FONT_SLANT,buf);
      set_radio_box_item(font_dialog_box,FONT_SLANT,buf);

      component = get_font_component(FONT_WEIGHT,valuestring);
      for(n=0;n<F_C_SIZE && component&& component[n]!='-';n++)
	buf[n]=component[n];
      buf[n]='\0';
      set_font_component(FONT_WEIGHT,buf);
      set_radio_box_item(font_dialog_box,FONT_WEIGHT,buf);

      component = get_font_component(FONT_FAMILY,valuestring);
      for(n=0;n<F_C_SIZE && component&& component[n]!='-';n++)
	buf[n]=component[n];
      buf[n]='\0';
      set_font_component(FONT_FAMILY,buf);
      set_radio_box_item(font_dialog_box,FONT_FAMILY,buf);

      component =  get_font_component(FONT_POINTSIZE,valuestring);
      for(n=0;n<F_C_SIZE && component&& component[n]!='-';n++)
	buf[n]=component[n];
      buf[n]='\0';
      set_font_component(FONT_POINTSIZE,buf);
      set_radio_box_item(font_dialog_box,FONT_POINTSIZE,buf);
    }
    /* the fragment below bombs when listbox hasn't been managed */
    /* but may be more appropriate when the bug is fixed */

  /* free list */
  XFreeFontNames(names_ret);
}



void prepare_font_selection_box( widget, tag, callback_data )
Widget	widget;
char    *tag;
XmAnyCallbackStruct *callback_data;
{
    Widget child;
    int n;
    Widget list;
    char buf[80];
    char *ptr;
    MrmCode return_type;
#ifdef DEBUG_CALLBACKS
    printf("In font sel box create routine; called by %s.\n",XtName(widget));
#endif
    global_font_selection_box_id = widget;

    /* we need to add a callback to the selection box's listbox child */
    /* so that selections from the listbox update the current displayed font */
    list = XmSelectionBoxGetChild(widget,XmDIALOG_LIST);
    XtAddCallback(list,XmNbrowseSelectionCallback,
		  display_one_font,NULL);
    XtAddCallback(list,XmNdefaultActionCallback,
		  display_one_font,NULL);
}


static char *get_font_spec_string()
{
  return(font_string);
}

char *get_selected_font_string(widget)
Widget widget;
{
  Widget text_widget;
  text_widget = XmSelectionBoxGetChild(widget,XmDIALOG_TEXT);
  return(XmTextGetString(text_widget));
}

static char *set_font_component(num,str)
int num;
char *str;
{
  int n;
  strcpy(font_string_fragments[num],str);
  font_string[0]='\0';
  for(n=0;n<F_C_COUNT;n++){
    strcat(font_string,font_string_fragments[n]);
    strcat(font_string,"-");
  }
  strcat(font_string,"*");
  return(font_string);
}

static char *get_font_component(num,str)
int num;
char *str;
{
  int n;
  for(n=0;n<num;n++){
    while(*str && *str!='-')str++;
    if(!*str)
      return(NULL);
    str++;
  }
  return(str);
}

static void restrict_using_string ( widget, which_comp, tag, callback_data )
Widget	widget;
int which_comp;
char *tag;
XmToggleButtonCallbackStruct *callback_data;
{
    if(!(callback_data->reason == XmCR_VALUE_CHANGED))return;
    if(!(callback_data->set))return;
    if(global_font_spec_mask)
      XmTextSetString(global_font_spec_mask,set_font_component(which_comp,tag));
}

void restrict_family ( widget, tag, callback_data )
	Widget	widget;
	char *tag;
	XmRowColumnCallbackStruct *callback_data;
{
    restrict_using_string(widget,FONT_FAMILY,tag,callback_data);
}

void restrict_weight ( widget, tag, callback_data )
	Widget	widget;
	char *tag;
	XmRowColumnCallbackStruct *callback_data;
{
    restrict_using_string(widget,FONT_WEIGHT,tag,callback_data);
}

void restrict_slant ( widget, tag, callback_data )
	Widget	widget;
	char *tag;
	XmRowColumnCallbackStruct *callback_data;
{
    restrict_using_string(widget,FONT_SLANT,tag,callback_data);
}

void restrict_point_size ( widget, tag, callback_data )
Widget	widget;
char *tag;
XmRowColumnCallbackStruct *callback_data;
{
    restrict_using_string(widget,FONT_POINTSIZE,tag,callback_data);
}


/* this function creates labels for all fonts named in a list widget */
void display_fonts_from_list(widget,tag,callback_data)
Widget widget;
char *tag;
XmAnyCallbackStruct *callback_data;
{
  Widget fetched=NULL;
  Arg arg[2];
  Arg pos_args[4];
  Position x,y;
  Dimension width;
  int item_count;
  Widget list;
  XmString *item_table;
  char *primitive_string;


  fetch("multiple_fonts_shell",&fetched);
  font_paned_window = XtNameToWidget(fetched,"mf_paned_window");

  /* place to the right of the font selection dialog */
  XtSetArg(pos_args[0],XmNx,&x);
  XtSetArg(pos_args[1],XmNy,&y);
  XtSetArg(pos_args[2],XmNwidth,&width);
  if(global_font_selection_box_id)
    XtGetValues(XtParent(XtParent(global_font_selection_box_id)),pos_args,3);
  XtSetArg(pos_args[0],XmNx,x+width);
  XtSetArg(pos_args[1],XmNy,y+100);
  XtSetValues(fetched,pos_args,2);
      
  XtSetArg(arg[0],XmNitemCount,&item_count);
  XtSetArg(arg[1],XmNitems,&item_table);
  list = XmSelectionBoxGetChild(global_font_selection_box_id,XmDIALOG_LIST);
  XtGetValues(list,arg,2);

  while(item_count--){
    primitive_string = extract_first_xms_segment(item_table[item_count]);
    font_add_to_paned(primitive_string);
  }

  XtManageChild(fetched);
}

void font_refilter(widget,tag,callback_data)
Widget	widget;
char *tag;
XmAnyCallbackStruct *callback_data;
{
    Arg arg[2];
    char **names_ret;
    int num_names_ret;
    XmString *items;
    Widget list;
    Widget text;
    int max_ret = 1000;
    int item_count;
    char *fontspec;

    if(global_font_spec_mask)
      fontspec = XmTextGetString(global_font_spec_mask);
    names_ret = XListFonts(dpy,fontspec,max_ret,&num_names_ret);
#ifdef DEBUG_CALLBACKS
    printf("Selecting matches for: %s\n",fontspec);
#endif
    list = XmSelectionBoxGetChild(global_font_selection_box_id,XmDIALOG_LIST);
    text = XmSelectionBoxGetChild(global_font_selection_box_id,XmDIALOG_TEXT);
    XtSetArg(arg[0],XmNitemCount,&item_count);
    XtGetValues(list,arg,1);

    item_count = num_names_ret;
    items = (XmString *)calloc(num_names_ret,sizeof(XmString));
    while(num_names_ret--){
      items[num_names_ret]=XmStringDefCreate(names_ret[num_names_ret]);
    }
    XtSetArg(arg[0],XmNitems,items);
    XtSetArg(arg[1],XmNitemCount,item_count);
    XtSetValues(list,arg,2);
    XmListSelectPos(list,1,0);
    if(names_ret && names_ret[0])
      XmTextSetString(text,names_ret[0]);
    else XmTextSetString(text," ");
    while(item_count--)
      XmStringFree(items[item_count]);
    XFreeFontNames(names_ret);
}
static void font_add_to_paned(fontname)
char *fontname;
{
  Arg arg[5];
  Widget form,text,label,button;
  char *alphabet = data.font_text;
  MrmType class;

  if(!font_paned_window){
    fetch_message_printf("fail_paned_window");
    return;
  }

  /* for each font, create a form and fill it with label and editable text */
  button = label = text = form = NULL;
  if(MrmFetchWidget(s_MrmHierarchy,"form_template",font_paned_window,
			&form,&class)!=MrmSUCCESS)
    fetch_message_printf("fail_form");

  if(MrmFetchWidget(s_MrmHierarchy,"button_template",form,
			&button,&class)!=MrmSUCCESS)
    fetch_message_printf("fail_button");

  XtSetArg(arg[0],XmNlabelString,XmStringDefCreate(fontname));
  XtSetArg(arg[1],XmNleftAttachment,XmATTACH_WIDGET);
  XtSetArg(arg[2],XmNleftWidget,button);
  if(MrmFetchWidgetOverride(s_MrmHierarchy,"name_template",form,NULL,arg,3,
			&label,&class)!=MrmSUCCESS)
    fetch_message_printf("fail_label");

  XtSetArg(
   arg[0],
   XmNfontList,
   XmFontListCreate(XLoadQueryFont(dpy,fontname),XmSTRING_DEFAULT_CHARSET)
   /* should this font list be freed after assignment to widget ? */
  );
  XtSetArg(arg[1],XmNtopAttachment,XmATTACH_WIDGET);
  XtSetArg(arg[2],XmNtopWidget,button);
  XtSetArg(arg[3],XmNeditMode,XmMULTI_LINE_EDIT);
  if(MrmFetchWidgetOverride(s_MrmHierarchy,"text_template",form,NULL,arg,4,
			       &text,&class)!=MrmSUCCESS)
    fetch_message_printf("fail_text");
  XmTextSetString(text,data.font_text);


  XtManageChild(label);
  XtManageChild(button);
  XtManageChild(text);

  XtManageChild(form);
}


void font_select_button( widget, tag, callback_data )
Widget	widget;
char    *tag;
XmAnyCallbackStruct *callback_data;
{
  Widget label_widget,text_widget;
  Arg arg[1];
  XmString xms;
  char *ps;

  /* get the label string from the brother label, extract the char * */
  label_widget = XtNameToWidget(XtParent(widget),"name_template");
  XtSetArg(arg[0],XmNlabelString,&xms);
  XtGetValues(label_widget,arg,1);
  ps = extract_first_xms_segment(xms);

  /* set the selected font text string */
  text_widget = 
   XmSelectionBoxGetChild(global_font_selection_box_id,XmDIALOG_TEXT);
  XmTextSetString(text_widget,ps);

  /* and trigger the one font select button callback */
  display_one_font( widget, tag, callback_data);
}

void kill_global( widget, tag, callback_data )
Widget	widget;
char    *tag;
XmAnyCallbackStruct *callback_data;
{
  if(!strcmp(tag,"mf_paned_window"))font_paned_window = (Widget) NULL;
  else if(!strcmp(tag,"one_font_shell"))global_one_font = (Widget) NULL;
}

void display_one_font( widget, tag, callback_data )
Widget	widget;
char    *tag;
XmAnyCallbackStruct *callback_data;
{
    static Widget fetched,disp_text;
    Widget text;
    Arg arg[1];
    MrmType class;
    char *fontname; /* retrieved from the text of the selection box */
    char *ptr;
    XFontStruct *fs;
    int num_names_ret;
    char **names_ret;
    MrmCode return_type;

    text = XmSelectionBoxGetChild(global_font_selection_box_id,XmDIALOG_TEXT);
    /* cast may be removed when XmTextGetString header is corrected */
    fontname = (char *)XmTextGetString(text);

    /* empty font string cannot work */
    if(!strlen(fontname)){
      fetch_message_printf("fail_empty_font_name");
      return;
    }

    names_ret = XListFonts(dpy,fontname,1,&num_names_ret);
    if(!num_names_ret){
      fetch_message_printf("fail_font_no_match",fontname);
      return;
    }
    XFreeFontNames(names_ret);

    fs = XLoadQueryFont(dpy,fontname);
    if(fs==NULL){
      fetch_message_printf("fail_font_load",fontname);
      return;
    }

    if(!global_one_font){
      Arg pos_args[4];
      Position x,y;
      Dimension width;
      fetch("one_font_shell",&fetched);
      /* place to the right of the font selection dialog */
      XtSetArg(pos_args[0],XmNx,&x);
      XtSetArg(pos_args[1],XmNy,&y);
      XtSetArg(pos_args[2],XmNwidth,&width);
      if(global_font_selection_box_id)
	XtGetValues(XtParent(XtParent(global_font_selection_box_id)),pos_args,3);
      XtSetArg(pos_args[0],XmNx,x+width);
      XtSetArg(pos_args[1],XmNy,y);
      XtSetValues(fetched,pos_args,2);
      
      global_one_font = fetched;
      disp_text = XtNameToWidget(global_one_font,
				 "one_font_stextSW.one_font_stext");
      if(MrmFetchLiteral(s_MrmHierarchy,
			"one_font_string",
			dpy,
			&ptr,
			&return_type) != MrmSUCCESS)
	ptr = "This is the selected font.";
      XmTextSetString(disp_text,ptr);

    }
    arg[0].name = XmNfontList;
    arg[0].value = (XtArgVal)XmFontListCreate(fs,XmSTRING_DEFAULT_CHARSET);
    XtSetValues(disp_text,arg,1);
    XtManageChild(fetched);
}


#include <X11/IntrinsicP.h>
#define XtName(widget) XrmQuarkToString (((Object)(widget))->object.xrm_name)
static void set_radio_box_item(widget,num,buf)
Widget widget;
int num;
char *buf;
{
  int n;
  int user_data;
  char *user_charp;
  CompositeWidget tb,rc,rb;
  Widget w;
  Boolean found;
  Arg arg[1];
  char rbtext[9];
  char *rcname;
  tb = (CompositeWidget)XtNameToWidget(widget,"font_toggle_box");
  found=0;
  for(n=0;n<tb->composite.num_children;n++){
    XtSetArg(arg[0],XmNuserData,&user_data);
    XtGetValues((Widget)rc=tb->composite.children[n],arg,1);
    if(user_data == num){
      rcname = XtName((Widget)rc);
      found=1;
      break;
    }
  }
  if(!found)return;
  /* locate the radio box child of rc */
  strcpy(rbtext,"font_rb?");
  rbtext[8]='\0';
  rbtext[7]=rcname[2];
  rb = (CompositeWidget)XtNameToWidget((Widget)rc,rbtext);
  /* locate the correct child of rb */
  for(n=0;n<rb->composite.num_children;n++){
    XtSetArg(arg[0],XmNuserData,&user_data);
    XtGetValues(w=rb->composite.children[n],arg,1);
    if(!stricmp(buf,spec_components[user_data])){
      XmToggleButtonSetState(w,1,1);
      break;
    }
  }
}








