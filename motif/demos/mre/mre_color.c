/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990 OPEN SOFTWARE FOUNDATION, INC.
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
** mre_color.c - Motif Resource Editor Color Handling 
**
** Revision history: 
**  created July 1989 Mitch Trachtenberg
**
**  substantially rewritten July 24, 1989 to eliminate pre-GA workarounds
**  and permit multiple simultaneous color forms; the major change is that
**  information specific to a given color form is stored in a structure
**  pointed to by the color form's userData resource
*/

#include "mre.h"

typedef struct {
  int    item_to_adjust;
  char   colorname[40];
  char   redname[10];
  char   greenname[10];
  char   bluename[10];
  XColor color; 
  unsigned long fg_pixel;/* foreground   */
  unsigned long bg_pixel;/* background   */
  unsigned long ts_pixel;/* topshadow    */
  unsigned long bs_pixel;/* bottomshadow */
} ColorData, *ColorDataPtr;

extern void fetch();
void color_selection_made ();
void scale_value_changed ();
void load_rgb_text ();
void setbg_button_pressed ();
void setcolor_button_pressed ();
void select_color ();

void create_colorform(resourcestring,valuestring,item_to_adjust)
char *resourcestring;
char *valuestring;
int   item_to_adjust;
{
    ColorDataPtr colordata;
    Widget fetched;
    Arg arg[4];
    unsigned long plane_masks_return[1];
    unsigned long pixels_return[4];
    Status status;
    Boolean false = 0;
    XColor color;
    Position x,y;

    fetched = (Widget) 0;
    colordata = (ColorDataPtr)calloc(1,sizeof(ColorData));
    colordata->item_to_adjust = item_to_adjust;
    status = XAllocColorCells(
			  dpy,
			  cmap,
			  false,
			  plane_masks_return,
			  1,
			  pixels_return,
			  4);
    if(!status){
      fetch_message_printf("fail_alloc_colors");
      no_colors_available = 1;
      return;
    }
    colordata->color.pixel = pixels_return[0];
    colordata->color.flags = DoRed | DoGreen | DoBlue;
    colordata->fg_pixel = pixels_return[0];
    colordata->bg_pixel = pixels_return[1];
    colordata->ts_pixel = pixels_return[2];
    colordata->bs_pixel = pixels_return[3];

    initialize_coords();

    fetch("color_form",&fetched);

    /* store userData */
    XtSetArg(arg[0],XmNuserData,colordata);
    XtSetValues(fetched,arg,1);
    setcolor_colorform(fetched,valuestring);

    /* store resourcestring information */
    XtSetArg(arg[0],XmNlabelString,XmStringDefCreate(resourcestring));
    XtSetValues(XtNameToWidget(fetched,"color_resource_label"),arg,1);

    /* set the background (and other fields) of the color patch */
    /* to the allocated color and associated colors */
    color.pixel = pixels_return[0];
    XQueryColor(dpy,cmap,&color);
    color.pixel = pixels_return[2];
    XStoreColor(dpy,cmap,&color);
    color.pixel = pixels_return[3];
    XStoreColor(dpy,cmap,&color);

    /* note that foreground is initialized to contrast with background */
    color.red = 65535 - color.red;
    color.green = 65535 - color.green;
    color.blue = 65535 - color.blue;
    color.pixel = pixels_return[1];
    XStoreColor(dpy,cmap,&color);

    XtSetArg(arg[0],XmNbackground,pixels_return[0]);
    XtSetArg(arg[1],XmNforeground,pixels_return[1]);
    XtSetArg(arg[2],XmNtopShadowColor,pixels_return[2]);
    XtSetArg(arg[3],XmNbottomShadowColor,pixels_return[3]);
    XtSetValues(XtNameToWidget(fetched,"color_patch"),arg,4);

    XtManageChild(fetched);
}

void destroy_colorform(widget,tag,callback_data)
Widget widget;
char *tag;
XmAnyCallbackStruct *callback_data;
{
  Arg arg[1];
  ColorDataPtr userdata;
  /* get the userdata and free it*/
  XtSetArg(arg[0],XmNuserData,&userdata);
  XtGetValues(widget,arg,1);
  free(userdata);
}

/* callback notified when selection is made */
void color_selection_made(widget,tag,callback_data)
Widget widget;
char *tag;
XmSelectionBoxCallbackStruct *callback_data;
{
  Arg arg[1];
  char *color_string;

  color_string = extract_first_xms_segment(callback_data->value);
  setcolor_colorform(XtParent(widget),color_string);
}

/* callback notified when selection is made */
void colorlist_selection_made(widget,tag,callback_data)
Widget widget;
char *tag;
XmListCallbackStruct *callback_data;
{
  Arg arg[1];
  char *color_string;

  color_string = extract_first_xms_segment(callback_data->item);
  /* the following code will fail if the list is not a descendant of a form */
  while(!XtIsSubclass(widget,xmFormWidgetClass))
    widget = XtParent(widget);
  setcolor_colorform(widget,color_string);
}

setcolor_colorform(form,valuestring)
Widget form;
char *valuestring;
{
  Arg arg[1];
  ColorDataPtr colordata;
  Widget scale;
  Widget text;
  Widget label;
  XmString tempstring;

  XtSetArg(arg[0],XmNuserData,&colordata);
  XtGetValues(form,arg,1);
  /* set the color cell to valuestring */
  if(*valuestring=='#'){
    XParseColor(dpy,cmap,valuestring,&(colordata->color));
    XStoreColor(dpy,cmap,&(colordata->color));
  }
  else 
    XStoreNamedColor(dpy,cmap,valuestring,
		     colordata->color.pixel,
		     colordata->color.flags);
  /* and fill the XColor struct with RGB values for scales */
  XQueryColor(dpy,cmap,&(colordata->color));

  /* based on the values in the color fields, set the slider values */
  scale = XtNameToWidget(form,"slider_box.scale1");
  XtSetArg(arg[0],XmNvalue,(int)(colordata->color.red/COLOR_PCT_MULTIPLIER));
  XtSetValues(scale,arg,1);
  sprintf(colordata->redname,"%4.4x",colordata->color.red);

  scale = XtNameToWidget(form,"slider_box.scale2");
  XtSetArg(arg[0],XmNvalue,(int)(colordata->color.green/COLOR_PCT_MULTIPLIER));
  XtSetValues(scale,arg,1);
  sprintf(colordata->greenname,"%4.4x",colordata->color.green);

  scale = XtNameToWidget(form,"slider_box.scale3");
  XtSetArg(arg[0],XmNvalue,(int)(colordata->color.blue/COLOR_PCT_MULTIPLIER));
  XtSetValues(scale,arg,1);
  sprintf(colordata->bluename,"%4.4x",colordata->color.blue);
    
  sprintf(colordata->colorname,valuestring);
  text = XmSelectionBoxGetChild(
	    XtNameToWidget(form,"color_sb"),
	    XmDIALOG_TEXT);
  XmTextSetString(text,valuestring);
  /* set the top label's text, also */
  tempstring=XmStringDefCreate(colordata->colorname);
  XtSetArg(arg[0],XmNlabelString,tempstring);
  label = XtNameToWidget(form,"color_patch_label");
  XtSetValues(label,arg,1);
  XmStringFree(tempstring);

}

void scale_value_changed ( widget, tag, callback_data )
Widget	widget;
char *tag;
XmScaleCallbackStruct *callback_data;
{
  Widget form;
  Widget label_widget;
  Widget patch_widget;
  Widget text_widget;
  ColorDataPtr colordata;

  static char red_label[5],green_label[5],blue_label[5];
  char rgb_label[15];
  unsigned long plane_masks_return[1];
  unsigned long pixels_return[1];
  Arg    arg[2];
  Status status;
  Boolean false = 0;
  int n;
  char label_string[15];
  XmString tempstring;

  form = XtParent(XtParent(widget));
  patch_widget = XtNameToWidget(form,"color_patch");
  label_widget = XtNameToWidget(form,"color_patch_label");
  text_widget = XmSelectionBoxGetChild(
	    XtNameToWidget(form,"color_sb"),
	    XmDIALOG_TEXT);

  XtSetArg(arg[0],XmNuserData,&colordata);
  XtGetValues(form,arg,1);

  /* based on the tag, set either the red,green, or blue static variables, */
  /* then set color of label0 */

  if(*tag == 'r'){
    colordata->color.red = callback_data->value * COLOR_PCT_MULTIPLIER;
    sprintf(colordata->redname,"%4.4x",colordata->color.red);
  }
  else if(*tag == 'g'){
    colordata->color.green = callback_data->value * COLOR_PCT_MULTIPLIER;
    sprintf(colordata->greenname,"%4.4x",colordata->color.green);
  }
  else if(*tag == 'b'){
    colordata->color.blue = callback_data->value * COLOR_PCT_MULTIPLIER;
    sprintf(colordata->bluename,"%4.4x",colordata->color.blue);
  }

  /* update the color cell */
  XStoreColor(dpy,cmap,&(colordata->color));

  /* assemble complete hex-string and set editable text to it */
  strcpy(colordata->colorname,"#");
  strcat(colordata->colorname,colordata->redname);
  strcat(colordata->colorname,colordata->greenname);
  strcat(colordata->colorname,colordata->bluename);

  XmTextSetString(text_widget,colordata->colorname);

  /* set the top label's text, also */
  tempstring=XmStringDefCreate(colordata->colorname);
  XtSetArg(arg[0],XmNlabelString,tempstring);
  XtSetValues(label_widget,arg,1);
  XmStringFree(tempstring);
}


char *get_color_string(widget)
Widget widget;
{
  Widget text_widget;
  text_widget = XtNameToWidget(widget,"color_sb.sb_text");
  /* if no color_text child of widget, must be the popup button pressed */
  /* so locate using global widget "popped_by," which might be color_form */
  /* or color_patch */
  if(!text_widget && popped_by){
    text_widget = XtNameToWidget(XtParent(popped_by),"color_sb.sb_text");
    if(!text_widget)
      text_widget = XtNameToWidget(popped_by,"color_sb.sb_text");
    if(!text_widget)
      return("black");
  }
  return(XmTextGetString(text_widget));
}

void setbg_button_pressed( widget, tag, callback_data )
Widget	widget;
char    *tag;
XmAnyCallbackStruct *callback_data;
{
  char buf[80];
  char *specstring;
  strcpy(buf,"xsetroot -solid ");
  specstring = get_color_string(XtParent(widget));
  /* shell needs hash to be escaped */
  if(*specstring == '#')strcat(buf,"\\");
  /* dd: enclose with " ", so it could handle multi space color name */
  strcat(buf, "\"");
  strcat(buf,specstring);
  strcat(buf, "\"");
  
  system(buf);
}

/* This function is called back by the buttons of the color popup menu */
/* Depending upon the tag, either the foreground, topshadow, bottomshadow */
/* or background of the color patch widget is set to the current color. */
void setcolor_button_pressed( widget, tag, callback_data )
Widget	widget;
char    *tag;
XmAnyCallbackStruct *callback_data;
{
  XColor color;
  Widget form;
  Arg arg[1];
  ColorDataPtr colordata;

  /* determine which color patch you have been popped up regarding */
  /* and use userData from its parent; get the current rgb and set */
  /* them into the appropriate pixel */
  form = XtParent(popped_by);
  XtSetArg(arg[0],XmNuserData,&colordata);
  XtGetValues(form,arg,1);

  /* store rgb values from foreground into appropriate field */
  color.red = colordata->color.red;
  color.green = colordata->color.green;
  color.blue = colordata->color.blue;
  color.flags = colordata->color.flags;

  if(*tag=='F'){
    XtSetArg(arg[0],XmNforeground,&(color.pixel));
    XtGetValues(popped_by,arg,1);
  }
  else if(*tag=='S'){
    XtSetArg(arg[0],XmNbottomShadowColor,&(color.pixel));
    XtGetValues(popped_by,arg,1);
  }
  else if(*tag=='T'){
    XtSetArg(arg[0],XmNtopShadowColor,&(color.pixel));
    XtGetValues(popped_by,arg,1);
  }
  else return;

  XStoreColor(dpy,cmap,&color);

}


/* this routine is called when the color list is created */
#define MAX_DEFINED_COLORS 300
void load_rgb_text( widget, tag, callback_data )
Widget	widget;
char    *tag;
XmAnyCallbackStruct *callback_data;
{
  Arg arg[2];
  FILE *rgb_text;
  int item_count;
  XmString xms[MAX_DEFINED_COLORS]; 
  char buf[160];

  if(!(rgb_text=fopen(data.rgb_text_file,"r"))) {
    fetch_message_printf("fail_cant_open",buf);
    return;
  }

  for(item_count=0; (item_count<MAX_DEFINED_COLORS) &&
                    fgets(buf,sizeof(buf),rgb_text); item_count++) {
      char *name;
      /* get correct (4th) field of rgb.txt lines */
      strtok(buf," \t\n");
      strtok(NULL," \t\n");
      strtok(NULL," \t\n");
      name = strtok(NULL,"\t\n");
      /* if you don't set the array item, counteract the autoincrement */
      if(name)
	xms[item_count] = XmStringDefCreate(name);
      else item_count--;
  }

  fclose(rgb_text);

  XtSetArg(arg[0],XmNlistItems,xms);
  XtSetArg(arg[1],XmNlistItemCount,item_count);
  XtSetValues(widget,arg,2);

  while(--item_count > 0)
      XmStringFree(xms[item_count]);

  widget = XmSelectionBoxGetChild(widget, XmDIALOG_LIST);
  XtAddCallback(widget,
		XmNsingleSelectionCallback,colorlist_selection_made,NULL);
  XtAddCallback(widget,
		XmNbrowseSelectionCallback,colorlist_selection_made,NULL);
  XtAddCallback(widget,
		XmNdefaultActionCallback,colorlist_selection_made,NULL);

}












