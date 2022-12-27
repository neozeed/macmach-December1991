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

/***************************************************************
** mre_util.c - Motif Resource Editor utility routines
**
** Revision history: 
**  created July 1989 Mitch Trachtenberg
**
** Note: functions MRE_for_all_children and print_hierarchy
**       access private widget data, and force inclusion of 
**       private header IntrinsicP.h in the middle of this file.
**
****************************************************************/

#include "mre.h"
#include <varargs.h>


/* get normal string from XmString */
char *extract_first_xms_segment(cs)
XmString cs;
{
  XmStringContext context;
  XmStringCharSet charset;
  XmStringDirection direction;
  Boolean separator;
  char *primitive_string;
  XmStringInitContext(&context,cs);
  XmStringGetNextSegment(context,&primitive_string,
	   &charset,&direction,&separator);
  XmStringFreeContext(context);
  return(primitive_string);
}

/* find first normal string in XmString with a non-zero length */
char *extract_nth_xms_segment(cs,seg_num)
XmString cs;
int seg_num;
{
  XmStringContext context;
  XmStringCharSet charset;
  XmStringDirection direction;
  Boolean separator;
  char *primitive_string;
  int n;
  XmStringInitContext(&context,cs);
  n=0;
  do{
    XmStringGetNextSegment(context,&primitive_string,
	   &charset,&direction,&separator);
  }
  while(n++<seg_num && primitive_string && !strlen(primitive_string));
  XmStringFreeContext(context);
  return(primitive_string);
}

/* return newly allocated duplicate copy of an asciiz string */
char *strdup(s)
char *s;
{
  char *dup;
  dup = (char *)calloc(strlen(s)+1,sizeof(char));
  strcpy(dup,s);
  return(dup);
}

/* return pointer to value component of resource spec */
char *skip_to_value(s)
char *s;
{
  if(!(s=strchr(s,(int)':')))
    return(NULL);
  /* hop off the colon */
  s++;
  /* skip past the spaces */
  while(isspace(*s))s++;
  return(s);
}

/* return pointer to last lhs component of resource spec */
char *skip_to_resource(s)
char *s;
{
  char *colon;
  char *dot;
  char *asterisk;

  if(!(colon=strchr(s,(int)':')))
    return(NULL);

  /* find last dot and last asterisk prior to colon */
  *colon = '\0';
  dot = strrchr(s,(int)'.');
  asterisk = strrchr(s,(int)'*');
  *colon = ':';

  /* return the location following the last dot or asterisk, or s */
  if(!dot && !asterisk)
    return(s);
  else 
    return(dot > asterisk ? ++dot : ++asterisk);
}

/* case insensitive strcmp */
stricmp(s1,s2)
char *s1, *s2;
{
  int c1, c2;
  while(c1= *s1++,c2= *s2++,toupper(c1)==toupper(c2))
    if(!*s1 || !*s2)break;
  return(toupper(c1)-toupper(c2));
}

/* fetch an appropriate message dialog and display the specified message; */
/* note that the first argument is NOT the message itself but is instead  */
/* an index used to fetch the actual message from UIL.                    */
fetch_message_printf(va_alist)
va_dcl
{
  static Widget w_fetched, e_fetched, i_fetched;
  Widget fetched;
  char *ptr;
  char *format;
  va_list va_args;
  char buf[1000];
  MrmCode return_type;
  XmString xms;
  Arg arg[1];

  va_start(va_args);
  format = va_arg(va_args, char *);
  if(MrmFetchLiteral(s_MrmHierarchy,
			format,
			dpy,
			&ptr,
			&return_type) != MrmSUCCESS){
    printf("Cannot fetch error message.");
    ptr = format;
  }
  if(!strncmp(format,"fail",4)){
    if(!e_fetched){
      fetch("error_print",&e_fetched);
      XtUnmanageChild(XmMessageBoxGetChild(e_fetched,XmDIALOG_CANCEL_BUTTON));
      XtUnmanageChild(XmMessageBoxGetChild(e_fetched,XmDIALOG_HELP_BUTTON));
    }
    fetched = e_fetched;
  }
  else   if(!strncmp(format,"info",4)){
    if(!i_fetched){
      fetch("info_print",&i_fetched);
      XtUnmanageChild(XmMessageBoxGetChild(i_fetched,XmDIALOG_CANCEL_BUTTON));
      XtUnmanageChild(XmMessageBoxGetChild(i_fetched,XmDIALOG_HELP_BUTTON));
    }
    fetched = i_fetched;
  }
  else{
    if(!w_fetched){
      fetch("warning_print",&w_fetched);
      XtUnmanageChild(XmMessageBoxGetChild(w_fetched,XmDIALOG_CANCEL_BUTTON));
      XtUnmanageChild(XmMessageBoxGetChild(w_fetched,XmDIALOG_HELP_BUTTON));
    }
    fetched = w_fetched;
  }
  vsprintf(buf,ptr,va_args);
  xms=XmStringCreateLtoR(buf,XmSTRING_DEFAULT_CHARSET);
  XtSetArg(arg[0],XmNmessageString,xms);
  XtSetValues(fetched,arg,1);
  XtManageChild(fetched);
  va_end(va_args);
}

/* callback routine to unmanage parent of widget which issues call */
void unmanage_parent(w,t,c)
Widget w;
caddr_t t;                /* ignored */
XmAnyCallbackStruct c;    /* ignored */
{
  if(w && XtParent(w))
    XtUnmanageChild(XtParent(w));
  else 
    fetch_message_printf("fail_no_parent");
}

/* display an info dialog with MRE, protocol, server and font information */
print_environment_info()
 {
  char **fontpath;
  char buf[1000];
  int numpaths_ret;
  sprintf(buf,"%s\nX Protocol Version %d rev %d.\n\
X Server Vendor %s release %d.\nCurrent font paths:\n ",
	   MRE_VERSION_INFO,
	   XProtocolVersion(dpy),
	   XProtocolRevision(dpy),
	   XServerVendor(dpy),
	   XVendorRelease(dpy));
  fontpath = XGetFontPath(dpy,&numpaths_ret);
  while(numpaths_ret--){
    strcat(buf,fontpath[numpaths_ret]);
    strcat(buf,"\n ");
  }
  fetch_message_printf("info_pass_string",buf);
}

/* return user's home directory and .Xdefaults as file spec
 * use .Xresources instead - CMU */
char *get_filespec_of_defaults_file(buf)
char *buf;
{
  char *homedir;
  if(NULL==( homedir = getenv("HOME"))){
    puts("Couldn't find home directory.");
    return(NULL);
  }
  strcpy(buf,homedir);
  strcat(buf,"/.Xresources");
  return(buf);
}

/* these widgets are used only by the functions below */
static Widget t1_w,t2_w,target;

/* Perform an XtSetValues using info entered in text fields */ 
/* of a widget's property sheet dialog; */
/* Called by the "OK" button of the property sheet. NO ERROR CHECKING! */
static void MreSetValues(widget,tag_wp,callback_data)
Widget widget;
Widget *tag_wp;
XmAnyCallbackStruct callback_data;
{
  char *argtext;
  char *val;
  Arg arg[1];
  argtext = XmTextGetString(t1_w);
  val = XmTextGetString(t2_w);
  XtSetArg(arg[0],argtext,atoi(val));
  XtSetValues(*tag_wp,arg,1);
}

/* Create and manage a property sheet dialog for a target widget, */
/* specified in the tag_w argument. This function is called when */
/* a button in the widget hierarchy dialog is pressed. */
static void MreGetResourceList(widget,tag_w,callback_data)
Widget widget;
Widget tag_w;
XmAnyCallbackStruct callback_data;
{
    XmString xms;
    XtResourceList xtrp;
    Cardinal num_res;
    int n;
    char buf[200];
    Arg arg[10];
    static Widget info_w,lab_w;

    /* get info on all public resources of target widget */
    XtGetResourceList(XtClass(tag_w),&xtrp,&num_res);

    /* and assemble an XmString with all information */
    xms=XmStringCreateLtoR("Resources\n",XmSTRING_DEFAULT_CHARSET);

    /* retrieve current value of each resource with XtGetValues; */
    /* interpret based on resource class of the resource */
    for(n=0;n<num_res && n<70;n++){
      if(!strncmp("StringDir",xtrp[n].resource_type,9)){
	int strdir;
	XtSetArg(arg[0],xtrp[n].resource_name,&strdir);
	XtGetValues(tag_w,arg,1);
	sprintf(buf,"%s %s %s %d\n",
		xtrp[n].resource_name,
		xtrp[n].resource_class, 
		xtrp[n].resource_type, 
		strdir);
      }
      else if(!strncmp("String",xtrp[n].resource_type,6)){
	char *str;
	XtSetArg(arg[0],xtrp[n].resource_name,&str);
	XtGetValues(tag_w,arg,1);
	sprintf(buf,"%s %s %s %.40s\n",
		xtrp[n].resource_name,
		xtrp[n].resource_class, 
		xtrp[n].resource_type, 
		str ? str : "NIL");
      }
      else if(!strcmp("Boolean",xtrp[n].resource_type)){
	Boolean b;
	XtSetArg(arg[0],xtrp[n].resource_name,&b);
	XtGetValues(tag_w,arg,1);
	sprintf(buf,"%s %s %s %s\n",
		xtrp[n].resource_name,
		xtrp[n].resource_class, 
		xtrp[n].resource_type, 
		b ? "ON" : "OFF");
      }
      else if(!strncmp("XmString",xtrp[n].resource_type,8)){
	XmString str;
	char *s;
	XtSetArg(arg[0],xtrp[n].resource_name,&str);
	XtGetValues(tag_w,arg,1);
	s = extract_first_xms_segment(str);
	if(s && !strlen(s))
	  s=extract_nth_xms_segment(str,2);
	sprintf(buf,"%s %s %s %.40s\n",
		xtrp[n].resource_name,
		xtrp[n].resource_class, 
		xtrp[n].resource_type, 
		s ? s : "NIL");
      }
      else if(!strcmp("Position",xtrp[n].resource_type)){
	Position pos;
	XtSetArg(arg[0],xtrp[n].resource_name,&pos);
	XtGetValues(tag_w,arg,1);
	sprintf(buf,"%s %s %s %d\n",
		xtrp[n].resource_name,
		xtrp[n].resource_class, 
		xtrp[n].resource_type, 
		pos);
      }
      else if(!strcmp("Dimension",xtrp[n].resource_type)){
	Dimension dim;
	XtSetArg(arg[0],xtrp[n].resource_name,&dim);
	XtGetValues(tag_w,arg,1);
	sprintf(buf,"%s %s %s %d\n",
		xtrp[n].resource_name,
		xtrp[n].resource_class, 
		xtrp[n].resource_type, 
		dim);
      }
      else if(!strcmp("Integer",xtrp[n].resource_type)){
	int i;
	XtSetArg(arg[0],xtrp[n].resource_name,&i);
	XtGetValues(tag_w,arg,1);
	sprintf(buf,"%s %s %s %d\n",
		xtrp[n].resource_name,
		xtrp[n].resource_class, 
		xtrp[n].resource_type, 
		i);
      }
      else {
	caddr_t i;
	XtSetArg(arg[0],xtrp[n].resource_name,&i);
	XtGetValues(tag_w,arg,1);
	sprintf(buf,"%s %s %s 0x%x\n",
		   xtrp[n].resource_name,
		   xtrp[n].resource_class,
		   xtrp[n].resource_type,i);
      }
      xms=XmStringConcat(xms,
		   XmStringCreateLtoR(buf,XmSTRING_DEFAULT_CHARSET));
    }
    /* create a form with scrolled label containing resource information, */
    /* and text widgets and pushbutton which can update resource values.  */
    XtSetArg(arg[0],XmNwidth,300);
    XtSetArg(arg[1],XmNheight,300);
    if(!info_w){
      Widget scrl_w,vsb_w,hsb_w,pb_w;
      info_w=(Widget)XmCreateFormDialog(XtParent(widget),"propsheet",arg,2);
      XtSetArg(arg[2],XmNscrollingPolicy,XmAUTOMATIC);
      XtSetArg(arg[3],XmNbottomAttachment,XmATTACH_FORM);
      XtSetArg(arg[4],XmNrightAttachment,XmATTACH_FORM);
      XtSetArg(arg[5],XmNtopAttachment,XmATTACH_FORM);
      XtSetArg(arg[6],XmNleftAttachment,XmATTACH_FORM);
      XtSetArg(arg[7],XmNtopOffset,20);
      XtManageChild(scrl_w=(Widget)XmCreateScrolledWindow(info_w,"sw",arg,8));
      XtManageChild(pb_w=(Widget)XmCreatePushButton(info_w,"ok",NULL,0));
      XtAddCallback(pb_w,XmNactivateCallback,MreSetValues,&target);
      XtSetArg(arg[0],XmNleftAttachment,XmATTACH_WIDGET);
      XtSetArg(arg[1],XmNleftWidget,pb_w);
      XtManageChild(t1_w=(Widget)XmCreateText(info_w,"arg",arg,2));
      XtSetArg(arg[1],XmNleftWidget,t1_w);
      XtManageChild(t2_w=(Widget)XmCreateText(info_w,"val",arg,2));
      XtManageChild(vsb_w =(Widget)XmCreateScrollBar(scrl_w,"vsb",NULL,0));
      XtSetArg(arg[0],XmNorientation,XmHORIZONTAL);
      XtManageChild(hsb_w =(Widget)XmCreateScrollBar(scrl_w,"hsb",arg,1));
      XtSetArg(arg[0],XmNlabelString,xms);
      XtSetArg(arg[1],XmNalignment,XmALIGNMENT_BEGINNING);
      XtManageChild(lab_w =(Widget)XmCreateLabel(scrl_w,"label",arg,2));
      XmScrolledWindowSetAreas(scrl_w,hsb_w,vsb_w,lab_w);
    }
    else {
      XtSetArg(arg[0],XmNlabelString,xms);
      XtSetValues(lab_w,arg,1);
    }
    target=tag_w;
    XtManageChild(info_w);
    XmStringFree(xms);
    XtFree((char*)xtrp);
}

/* FUNCTIONS BELOW THIS POINT USE PRIVATE WIDGET FIELDS! */

#include <X11/IntrinsicP.h>
#ifdef XtName
#undef XtName
#endif
#define XtName(widget) XrmQuarkToString (((Object)(widget))->object.xrm_name)
#include <math.h>


/* map a void function over all children of a composite widget */
void MRE_for_all_children(w,f)
CompositeWidget w;
void (*f)();
{
  int n;
  if(!(XtIsComposite(w)))return;
  n = w->composite.num_children;
  while(--n>=0)
    f(w->composite.children[n]);
}

/* Create a bulletin board with a button for each widget in the hierarchy */
/* topped by widget; indent buttons to show parent-child relationships;  */
/* set each button's callback to generate a property sheet for the widget*/
/* the button represents. */
print_hierarchy(widget)
Widget widget;
{
  XtResourceList xtrp;
  XmString xms;
  Arg arg[8];
  int num_res;
  int n,indent_count;
  Widget pbg;
  Widget scrl_w;
  static int indent_level;
  static Widget bb_w,f_w,old_f_w;
  static Position label_x;
  static Position label_y;
  /* if at top of this hierarchy, create a bulletin_board */
  /* within a scrolled_window, within a form_dialog */
  if(!indent_level){
    XtSetArg(arg[0],XmNautoUnmanage,0);
    XtSetArg(arg[1],XmNwidth,300);
    XtSetArg(arg[2],XmNheight,300);
    XtSetArg(arg[3],XmNallowShellResize,1);
    label_x = 5;
    label_y = 5;
    if(f_w)XtDestroyWidget(f_w);
    old_f_w = f_w;
    f_w=(Widget)XmCreateFormDialog(widget,"hierarchy",arg,4);
    XtSetArg(arg[1],XmNwidth,300);
    XtSetArg(arg[2],XmNheight,300);
    XtSetArg(arg[3],XmNscrollingPolicy,XmAUTOMATIC);
    XtSetArg(arg[4],XmNbottomAttachment,XmATTACH_FORM);
    XtSetArg(arg[5],XmNrightAttachment,XmATTACH_FORM);
    XtSetArg(arg[6],XmNtopAttachment,XmATTACH_FORM);
    XtSetArg(arg[7],XmNleftAttachment,XmATTACH_FORM);
    scrl_w=(Widget)XmCreateScrolledWindow(f_w,"sw",arg,8);
    bb_w=(Widget)XmCreateBulletinBoard(scrl_w,"nesting",arg,3);
  }
  /* don't print the diagnostic branch */
  if(widget==f_w) {
    return;
  }
  /* destruction of old_hierarchy hasn't completed yet; */
  /* don't print it, either */
  if(widget==old_f_w) {
    return;
  }
  XtSetArg(arg[0],XmNx,label_x+(indent_level*5));
  XtSetArg(arg[1],XmNy,label_y);
  XtSetArg(arg[2],XmNuserData,widget);
  XtManageChild(pbg=(Widget)XmCreatePushButtonGadget(bb_w,XtName(widget),arg,3));
  XtAddCallback(pbg,XmNactivateCallback,MreGetResourceList,widget);
  label_y+= pbg->core.height ? pbg->core.height : 20;
  if(label_y>600){
    label_y=5;
    label_x+=125;
  }
  if(!XtIsWidget(widget)){
    return;
  }
  if(widget->core.num_popups){
     n = widget->core.num_popups;
    while(--n>=0){
      indent_level++;
      print_hierarchy(widget->core.popup_list[n]);
      indent_level--;
    }
  }
  if(XtIsComposite(widget)){
    n = ((CompositeWidget)widget)->composite.num_children;
    while(--n>=0){
      indent_level++;
      print_hierarchy(((CompositeWidget)widget)->composite.children[n]);
      indent_level--;
    }
  }

  if(!indent_level && f_w){
    XtManageChild(bb_w);
    XtManageChild(scrl_w);
    XtManageChild(f_w);
  }
}
