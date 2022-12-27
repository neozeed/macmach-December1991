/*
** mre.h - Motif Resource Editor Header 
** Copyright (c) 1989, 1990, 1991 Open Software Foundation, Inc.
** ALL RIGHTS RESERVED.
**
**
** Revision history: 
**  created June 1989 Mitch Trachtenberg
**
*/

#include <stdio.h>
#include <ctype.h>
#include <X11/StringDefs.h>

/* include headers for each widget type used in MRE */
#include <Xm/Xm.h>
#include <Xm/BulletinB.h>
#include <Xm/Command.h>
#include <Xm/DialogS.h>
#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/LabelG.h>
#include <Xm/MainW.h>
#include <Xm/MenuShell.h>
#include <Xm/MessageB.h>
#include <Xm/PushB.h>
#include <Xm/PushBG.h>
#include <Xm/RowColumn.h>
#include <Xm/Scale.h>
#include <Xm/SelectioB.h>
#include <Xm/Text.h>
#include <Xm/ToggleB.h>
#include <Xm/ToggleBG.h>

#include <Mrm/MrmPublic.h>

#define NULLW ((Widget)0)
#define NULLCB ((XmAnyCallbackStruct *)0)

#define MRE_VERSION_INFO "MRE Demo (August 15, 1989)"
#define MAX_ENTRY_SIZE 500

/* maximum number of lines in a .Xdefaults file */
#define MAX_XDEF_LINES 5000

/* scales show between 0 and 100, color values are between 0 and 65535 */
#define COLOR_PCT_MULTIPLIER 655

/* initial letter of tag on ok_button_callback determines routine */
#define FONT_OK_BUTTON_PRESSED 'F'
#define COLOR_OK_BUTTON_PRESSED 'C'

#define SORT_BY_SPEC                  1
#define SORT_BY_VALUE                 2
#define SORT_BY_RESOURCE              3
#define SORT_BY_SPEC_DESCENDING       4
#define SORT_BY_VALUE_DESCENDING      5
#define SORT_BY_RESOURCE_DESCENDING   6

#define MRE_OK 1
#define MRE_FAIL 0

#define MRE_TYPE_DELETED     -2
#define MRE_TYPE_DONT_SHOW   -1
#define MRE_TYPE_MISC         0
#define MRE_TYPE_COLOR        1
#define MRE_TYPE_FONT         2
#define MRE_TYPE_BOOLEAN      3
#define MRE_TYPE_FILE         4
#define MRE_TYPE_TRANSLATION  5
#define MRE_TYPE_ENUM         6

#define XtNfontText "fontText"
#define XtNrgbTextFile "rgbTextFile"
#define XtNmreTypeFile "mreTypeFile"
#define XtNprintHierarchy "printHierarchy"
#define XtNmergeOnSave "mergeOnSave"
#define XtNinternalPixmaps "internalPixmaps"
#define XtCFontText "FontText"
#define XtCRgbTextFile "RgbTextFile"
#define XtCMreTypeFile "MreTypeFile"
#define XtCPrintHierarchy "PrintHierarchy"
#define XtCMergeOnSave "MergeOnSave"
#define XtCInternalPixmaps "InternalPixmaps"
#define XmStringDefCreate(s) XmStringLtoRCreate((s),XmSTRING_DEFAULT_CHARSET)

typedef struct {
  Pixel fg;
  char *font_text;
  char *rgb_text_file;
  char *mre_type_file;
  Boolean print_hierarchy;
  Boolean merge_on_save;
  Boolean internal_pixmaps;
} ApplicationData, *ApplicationDataPtr;

typedef struct {
	int line_number;       /* corresponds to source file */
	int continuation;      /* nonzero means a continuation line */
	int continues_line;    /* master line which this line continues */
	char *line_contents;   /* pointer to text */
	int resource_type;     /* resource type as determined at readin */
	int enum_type;
	int listbox_index;     /* corresponds to pos'n in sorted listbox */
	/* a zero listbox_index means NOT DISPLAYABLE */
} LineInfo, *LIP;

typedef struct {
        int type_flag;
	int enum_type;
	char *type_string;
}TypeInfo, *TIP;

typedef struct {
        char *enum_label;
	char *enum_choices;
}EnumInfo, *EIP;



#ifdef MAIN
Colormap cmap;
Display *dpy;
Widget main_selection_box_id;
Widget global_one_font; /* single font text form */
Widget font_paned_window;
Widget popped_by;       /* name of widget which has triggered popup */
MrmHierarchy	s_MrmHierarchy;		/* MRM database hierarch id */
ApplicationData data;
Boolean no_colors_available;
#else
extern Colormap cmap;
extern Display *dpy;
extern Widget main_selection_box_id;
extern Widget global_one_font;/* single font text window */
extern Widget font_paned_window;
extern Widget popped_by;
extern ApplicationData data;
extern MrmHierarchy    s_MrmHierarchy;		/* MRM database hierarch id */
extern int current_file_item;
extern int current_font_item;
extern int current_color_item;
extern int current_generic_item;
extern Boolean no_colors_available;
#endif

extern void fetch();
extern int  resource_type();
extern char *skip_to_value();
extern void setbg_button_pressed();
extern void setcolor_button_pressed();
extern void create_colorform();
extern void destroy_colorform();
extern void select_color();
extern int initialize_color();
extern void load_rgb_text();
extern void load_mre_type_file();
extern void scale_value_changed();
extern void select_font();
extern void display_one_font();
extern void prepare_font_selection_box();
extern char *get_selected_font_string();
extern void restrict_point_size();
extern void restrict_family();
extern void restrict_weight();
extern void restrict_slant();
extern void display_fonts_from_list();
extern void font_refilter();
extern void font_select_button();
extern void dump_db();
extern void unmanage_parent();
extern char *get_color_string();
extern char *get_filespec_of_defaults_file();
extern unsigned long get_background();
extern char *strdup();
extern char *strchr();
extern char *strrchr();
extern char *strtok();
extern char *extract_first_xms_segment();
extern char *enumeration_choices();
extern enumeration_type();
extern char *getenv();
extern void destroy_parent();
extern void kill_global();
extern void MRE_for_all_children();


















