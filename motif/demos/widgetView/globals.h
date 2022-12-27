#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)globals.h	1.6 90/08/01";
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
 **   File:     @(#)globals.h	1.6 - 90/08/01
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
#include <ctype.h>

#define TEXT_INPUT_LENGTH 25
#define  SCALE_TEXT_LENGTH 5
#define MAX_COLORS 500
#define RGB "/usr/local/lib/X11/rgb.txt"
#define NO_COLOR_NAME "<No Color Name>"
#define STRING_IS_NULL "Null_String"
#define MAX_CLASSES 10
#define MAX_HIERARCHY_COLORS 4
#define STRING_OFFSET 14
#define TESTLABEL "cancel"
#define NOT_SET (char *) -1
#define MAX_TEXT_INPUT_LEN 50


#define C_TEMPLATE_FILE "template/template.c"
#define C_TEMPLATE_WITH_TEXT_FILE "template/mess_tmpl.c"
#define C_CODE "c_code"
#define C_CODE_WITH_TEXT "c_code_with_text"

#define UIL_TEMPLATE_FILE "template/template.uil"
#define UIL_TEMPLATE_WITH_TEXT_FILE "template/mess_tmpl.uil"
#define UIL_TEXT_TEMPLATE "template/text_templ.uil"
#define UIL_CODE "uil_code"
#define UIL_CODE_WITH_TEXT "uil_code_with_text"

/* for the FILE selection on menu bar */
#define LABEL1 "New"
#define LABEL2 "Open"
#define LABEL3 "Save"
#define LABEL4 "Quit"

/* for the Convienience function selection */
#define LABEL5 "Enabled"

/* the HELP selection */
#define LABEL6 "Manual"
#define LABEL7 "Source Code"
#define LABEL8 "widgetView"

typedef struct _active_widget {
  Widget address;             /* the address of the active widget */
  char *widget_name;          /* name returned by selection box */
  int widget_offset;          /* reference into the widgetlist arry */
  char hierarchy [50];       /* the hierarchy name, XmCore, XmPrimitive... */
  char *message_buffer;      /* buffer to hold any message information */
}ACTIVE_WIDGET;


typedef struct _more_info {
  char *name;
  long value;
}MORE_INFO;

typedef struct resource_vals {
  Cardinal num_items;             /* number of resources */
  char *name [MAX_ARGS];          /* resource names */
  char *type [MAX_ARGS];          /* resource types */
  char *class [MAX_ARGS];         /* class, of CORE, PRIMITIVE... */
  long value [MAX_ARGS];          /* resources value */
  long orgvalue [MAX_ARGS];       /* the original value */
  long color [MAX_ARGS];          /* color of the resource */
  Widget widget_addr [MAX_ARGS];  /* widget address of each resource button */
  short  widget_type [MAX_ARGS];  /* resource widget type(see above #defines) */
  Boolean editable [MAX_ARGS];    /* is the field user changeable ? */
  MORE_INFO *additional [MAX_ARGS]; /* used for any additional stuff */
}RESOURCEVALS;


typedef struct _colortype {
  
  int num_colors;
  struct _color_list {
    char *name;
    int pixel;
  }color_list [MAX_COLORS];

}COLORTYPE;


#ifdef MAIN
    RESOURCEVALS  *resources, *GetWidgetHierarchy ();
    ACTIVE_WIDGET ActiveWidget;
    COLORTYPE colortype;
    int ActiveColorPosition;
    int generate_message = 0;

    Widget HierarchyLabels [MAX_CLASSES];

    static char   buffy [256];

    /* the filename of this program */

    char filename [256];

    /* the colors allocated for use in the program,
       allocated in main () */
    unsigned long colors [MAX_HIERARCHY_COLORS];
    int black_and_white = 0;
    int max_hierarchy_colors = 0;
#else
    extern RESOURCEVALS  *resources, *GetWidgetHierarchy ();
    extern ACTIVE_WIDGET ActiveWidget;
    extern COLORTYPE colortype;
    extern int ActiveColorPosition;
    extern int generate_message;
    extern Widget HierarchyLabels [MAX_CLASSES];
    extern char   buffy [256];
    extern char filename [256];
    /* the colors allocated for use in the program,
       allocated in main () */
    extern unsigned long colors [MAX_HIERARCHY_COLORS];
    extern int black_and_white;
    extern int max_hierarchy_colors;
#endif
