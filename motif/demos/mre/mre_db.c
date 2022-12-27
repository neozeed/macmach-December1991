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
** mre_db.c - Motif Resource Editor Line Information Handling
**
** Functions in this file maintain mre's internal database connecting
** lines in the users' resource file with mre's listbox display
**
** Revision history: 
**  created July 1989 Mitch Trachtenberg
**
*/

#include "mre.h"

/* Use re_comp instead of regcmp if defined at the top level of the build */

#ifndef USE_RE_COMP
#ifdef NO_REGEX
#define USE_RE_COMP
#endif
#endif

extern stricmp();
extern char *skip_to_value();
extern char *skip_to_resource();

static LineInfo li[MAX_XDEF_LINES];
static int lc;

static TypeInfo typed_resources[100];
static int num_typed_resources;

static EnumInfo enumerations[100];
static int num_enumerations;

static TypeInfo mre_type_descriptions[]={
{MRE_TYPE_DELETED,     0,"DELETED"},
{MRE_TYPE_DONT_SHOW,   0,"DONT_SHOW"},
{MRE_TYPE_MISC,        0,"MISC"},
{MRE_TYPE_COLOR,       0,"COLOR"}, 
{MRE_TYPE_FONT,        0,"FONT"},
{MRE_TYPE_BOOLEAN,     0,"BOOLEAN"},
{MRE_TYPE_FILE,        0,"FILE"},
{MRE_TYPE_TRANSLATION, 0,"TRANSLATION"},
{MRE_TYPE_ENUM,        0,"ENUMERATED"}
};

static TypeInfo hardcoded_typed_resources[]={
 {MRE_TYPE_COLOR,0,"ground$"},
 {MRE_TYPE_COLOR,0,"fg"},
 {MRE_TYPE_COLOR,0,"bg"},
 {MRE_TYPE_FONT,0,"font$"},
 {MRE_TYPE_FONT,0,"fontList$"},
 {MRE_TYPE_BOOLEAN,0,"traversalOn$"},
 {MRE_TYPE_FILE,0,"windowManagerName$"},
 {MRE_TYPE_FILE,0,"terminalEmulatorName$"}
};

static TypeInfo value_determiners[]={
 {MRE_TYPE_BOOLEAN,0,"on"},
 {MRE_TYPE_BOOLEAN,0,"off"},
 {MRE_TYPE_BOOLEAN,0,"true"},
 {MRE_TYPE_BOOLEAN,0,"false"},
 {MRE_TYPE_COLOR,0,"red"},
 {MRE_TYPE_COLOR,0,"green"},
 {MRE_TYPE_COLOR,0,"blue"}
};

/* if an external file containing resource type information exists, load it */
void load_mre_type_file(name)
char *name;
{
  FILE *f;
  char buf[MAX_ENTRY_SIZE];
  char *type_string;
  int n;

  if(!(f = fopen(name,"r"))){
    fetch_message_printf("fail_cant_read",name);
    return;
  }
  while(fgets(buf,MAX_ENTRY_SIZE,f)){
    type_string=strtok(buf," \t\n");
    if(type_string==NULL)break;

    /* check to see if you are defining an enumeration */
    if(!stricmp(type_string,"DEFINE")){
      enumerations[num_enumerations].enum_label = strdup(strtok(NULL," \t\n"));
      enumerations[num_enumerations].enum_choices = strdup(strtok(NULL,"\n"));
      ++num_enumerations;
      continue;
    }
    /* find the type description string matching the line's first component */
    /* and set the type of the line's remainder to the matching MRE_TYPE */
    for(n=0;n<XtNumber(mre_type_descriptions);n++){
      if(!stricmp(type_string,mre_type_descriptions[n].type_string)){
	char *resource_string;
	resource_string = strtok(NULL,"\n");
	if(resource_string==NULL){
	  fetch_message_printf("fail_resource_string");
	  continue;
	}
	typed_resources[num_typed_resources].type_string =
	  strdup(resource_string);
        typed_resources[num_typed_resources].type_flag =
	  mre_type_descriptions[n].type_flag;
	num_typed_resources++;
      }
    }
    /* if the search for a defined type didn't work, check for an enum */
    for(n=0;n<num_enumerations;n++){
      if(!stricmp(type_string,enumerations[n].enum_label)){
	char *resource_string;
	resource_string = strtok(NULL,"\n");
	if(resource_string==NULL){
	  fetch_message_printf("fail_resource_string");
	  continue;
	}
	typed_resources[num_typed_resources].type_string =
	  strdup(resource_string);
        typed_resources[num_typed_resources].type_flag = MRE_TYPE_ENUM;
        typed_resources[num_typed_resources].enum_type = n;
	num_typed_resources++;
      }
    }
  }
  return;
}

/* reset the list; called by file_new_proc */
clear_li()
{
 int n;
 for(n=0;n<lc;n++)
   if(li[lc].line_contents)free(li[lc].line_contents);
 lc = 0;
}

/* this function reads a resource file into the li line information array */
read_into_li(f)
FILE *f;
{
 int n;
 int listbox_index;
 int saved_lc;
 char buf[MAX_ENTRY_SIZE];
 for(listbox_index=1,lc=0;fgets(buf,MAX_ENTRY_SIZE,f);lc++){
	int length;
	length = strlen(buf);
	/* clip terminal newlines, spaces */
	while(length>1 && 
	      (buf[length-1]=='\n' || 
	       buf[length-1]=='\t' || 
	       buf[length-1]==' ')){
	  buf[length-1]='\0';
	  length--;
	}
	/* make ss8 listbox XmString routines happy */
	if(*buf=='\0')strcpy(buf,"!");

	/* allow for multiple calls to this function; free and zero resources*/
	if(li[lc].line_contents)
	  free(li[lc].line_contents);
	li[lc].continuation = li[lc].listbox_index = 0;
        li[lc].resource_type = MRE_TYPE_DONT_SHOW;
	li[lc].line_number = lc;
	li[lc].line_contents = strdup(buf);
	saved_lc = lc;
	while(ends_in_slash(buf)){
		fgets(buf,MAX_ENTRY_SIZE,f);
		length = strlen(buf);
		/* clip terminal newline */
		if(buf[length-1]=='\n')
		  buf[length-1]='\0';
		else
		  length++;
		lc++;
		li[lc].line_number = lc;
		li[lc].continues_line = saved_lc;
		li[lc].line_contents = (char *)calloc(length,sizeof(char));
		strcpy(li[lc].line_contents,buf);
	}
	if(has_colon(li[saved_lc].line_contents) 
		&& !is_comment(li[saved_lc].line_contents)
		&& !is_hash(li[saved_lc].line_contents) 
		&& !is_blank(li[saved_lc].line_contents)) {
	                int cont;
			li[saved_lc].resource_type = 
			  resource_type(li[saved_lc].line_contents);
			if(li[saved_lc].resource_type == MRE_TYPE_ENUM)
			  li[saved_lc].enum_type = get_enumerated_type();
			for(cont=0;saved_lc<=lc;cont++,saved_lc++){
			  li[saved_lc].listbox_index = listbox_index;
			  li[saved_lc].continuation = cont;
			}
			listbox_index++;
		}
	else li[saved_lc].resource_type = MRE_TYPE_DONT_SHOW;
 }
}

static int global_enumerated_type;
set_enumerated_type(val)
{
  global_enumerated_type = val;
}

get_enumerated_type()
{
  return global_enumerated_type;
}

/* the type of a given resource is determined by comparing the resource */
/* against hard-coded and externally loaded regular expressions;        */
/* then checking the resource's value against hard-coded strings;       */
/* then searching for special characters in the resource value.         */
resource_type(s)
char *s;
{
  char buf[MAX_ENTRY_SIZE];
  char *res;
  char *val;
  int hyphens;
  int n;
  int good;

  res = skip_to_resource(s);
  for(n=0;(buf[n]=(*res))!=':';n++,res++);
  buf[n]='\0';
#ifdef USE_RE_COMP
  for(n=0;n<XtNumber(hardcoded_typed_resources);n++){
    re_comp(hardcoded_typed_resources[n].type_string);
    if(1==re_exec(buf)){
      if(hardcoded_typed_resources[n].type_flag == MRE_TYPE_ENUM)
	set_enumerated_type(hardcoded_typed_resources[n].enum_type);
      return(hardcoded_typed_resources[n].type_flag);
    }
  }
#else
  for(n=0;n<XtNumber(hardcoded_typed_resources);n++){
    char *ptr;
    ptr = NULL;
    ptr=(char *)regcmp(hardcoded_typed_resources[n].type_string,NULL);
    if(regex(ptr,buf)){
      if(ptr)free(ptr);
      if(hardcoded_typed_resources[n].type_flag == MRE_TYPE_ENUM)
	set_enumerated_type(hardcoded_typed_resources[n].enum_type);
      return(hardcoded_typed_resources[n].type_flag);
    }
    if(ptr)free(ptr);
  }
#endif /* USE_RE_COMP */
  /* if not, check to see if it matches one from the mre_type_file */
#ifdef USE_RE_COMP
  for(n=0;n<num_typed_resources;n++){
    re_comp(typed_resources[n].type_string);
    if(1==re_exec(buf)){
      if(typed_resources[n].type_flag == MRE_TYPE_ENUM)
	set_enumerated_type(typed_resources[n].enum_type);
      return(typed_resources[n].type_flag);
    }
  }
#else
  for(n=0;n<num_typed_resources;n++){
    char *ptr;
    ptr = NULL;
    ptr=(char *)regcmp(typed_resources[n].type_string,NULL);
    if(regex(ptr,buf)){
      if(ptr)free(ptr);
      if(typed_resources[n].type_flag == MRE_TYPE_ENUM)
	set_enumerated_type(typed_resources[n].enum_type);
      return(typed_resources[n].type_flag);
    }
    if(ptr)free(ptr);
  }
#endif /* USE_RE_COMP */
  /* if not, check to see if value matches a hard-coded string */
  val = skip_to_value(s);
  strcpy(buf,val);
  for(n = strlen(buf) - 1;n>0 && isspace(buf[n]);n--)
    buf[n]='\0';
  for(n=0;n<XtNumber(value_determiners);n++)
    if(!stricmp(buf,value_determiners[n].type_string))
      return(value_determiners[n].type_flag);

  /* check to see if value "looks like" a font specification */
  /* (does it have >5 hyphens?) */
  for(n=0,hyphens=0;buf[n];n++)
    if(buf[n]=='-')
      hyphens++;
  if(hyphens>5)
    return(MRE_TYPE_FONT);

  /* check to see if value "looks like" a file specification */
  /* (does it have a forwardslash?) */
  for(n=0;buf[n];n++)
    if(buf[n]=='/' || buf[n]=='.')
      return(MRE_TYPE_FILE);

  /* check to see if value "looks like" a xlat specification */
  /* (does it have a "<> */
  /* NOTE THIS MUST PRECEDE CHECK FOR COLOR SHARP SIGN */
  for(n=0;buf[n];n++)
    if(buf[n]=='<' || buf[n]=='>')
      return(MRE_TYPE_TRANSLATION);

  /* check to see if value "looks like" a color specification */
  /* (does it begin with a hash and have hex digits?) */
  /* (does it match an entry in the rgb.txt file?) */
  good = 1;
  if(*buf!='#')good = 0;
  else {
    for(n=1;buf[n];n++)
      if(!isxdigit(buf[n]))
	good=0;
  }
  if(good)return(MRE_TYPE_COLOR);

  return(MRE_TYPE_MISC);
}


/* this function determines whether to allow a given line in the li array */
/* to be entered and displayed in the main listbox */
allow_entry(t,f)
int t, f;
{
  int n;
  /* never allow entry of deleted or dontshow types */
  if(t<0)return(0);

  /* allow entry for a type if that type's bit is set in the type_flag */
  for(n=1;n<7;n++){
    if(t==n && f&(1<<(n-1)))return(1);
  }

  /* allow entry for miscellaneous if allow_misc bit is set */
  if(t==MRE_TYPE_MISC && f&0x20)
    return(1);

  /* dont allow anything else */
  return(0);
}

/* routine to perform string compares on the value part of line infos */
int valstrcmp(lip1,lip2)
LIP lip1,lip2;
{
  char *s1, *s2;
  s1 = lip1->line_contents;
  s2 = lip2->line_contents;
  if(!(s1 = skip_to_value(s1)))
    return(1);
  if(!(s2 = skip_to_value(s2)))
    return(-1);
  return(stricmp(s1,s2));
}

/* opposite of above */
int revvalstrcmp(lip1,lip2)
LIP lip1,lip2;
{
  return(-valstrcmp(lip1,lip2));
}

/* routine to perform string compares on the resource part of line infos */
int resstrcmp(lip1,lip2)
LIP lip1, lip2;
{
  char *s1, *s2;
  s1 = lip1->line_contents;
  s2 = lip2->line_contents;
  if(!(s1 = skip_to_resource(s1)))
    return(1);
  if(!(s2 = skip_to_resource(s2)))
    return(-1);
  return(stricmp(s1,s2));
}

/* opposite of above */
int revresstrcmp(lip1,lip2)
LIP lip1,lip2;
{
  return(-resstrcmp(lip1,lip2));
}

/* routine to perform string compares on the line contents of line infos */
int lipstrcmp(lip1,lip2)
LIP lip1, lip2;
{
  char *s1, *s2;
  s1 = lip1->line_contents;
  s2 = lip2->line_contents;
  return(stricmp(s1,s2));
}

/* opposite of above */
int revstrcmp(lip1,lip2)
LIP lip1,lip2;
{
  return(-lipstrcmp(lip1,lip2));
}

/* function to give to qsort to sort a line info array back to file order */
cmplinenums(lip1,lip2)
LIP lip1, lip2;
{
  return(lip1->line_number - lip2->line_number);
}

sort_li_array(method)
int method;
{
  static int (*compar[])()={
    lipstrcmp,
    lipstrcmp,
    valstrcmp,
    resstrcmp,
    revstrcmp,
    revvalstrcmp,
    revresstrcmp
    };
  int n;
  int indx;

  qsort(li,lc,sizeof(LineInfo),compar[method]);
  /* now walk sorted array, setting non-zero listbox_indices in sequence */
  indx = 1;
  for(n=0;n<lc;n++){
    if(li[n].listbox_index)li[n].listbox_index=indx++;
  }
}

/* this function builds a new item array for the main selection listbox */
/* based on the current state of the line_info array */
gen_new_list(type_flag)
int type_flag;
{
  int n;
  int listbox_index = 1;
  int item_count;
  Widget list;
  Arg arg[2];
  XmString *items;
  for(n=0;n<lc;n++){
    if(allow_entry(li[n].resource_type,type_flag)){
      if(!li[n].continuation){
        li[n].listbox_index = listbox_index++;
      }
    }
    else if(li[n].resource_type>=0){
      li[n].listbox_index = 0;
    }
  }
  /* calloc an item array of listbox_index (less one) items; */
  /* fill it with compound strings generated from list,*/
  /* and set the main listbox to that array and item count */
  item_count = listbox_index - 1;
  items = (XmString *)calloc(item_count,sizeof(XmString));

  for(listbox_index=n=0;listbox_index<item_count;n++){
    char buf[MAX_ENTRY_SIZE];
    if(allow_entry(li[n].resource_type,type_flag)){
      if(!li[n].continuation){
	if(ends_in_slash(li[n].line_contents)){
	  strcpy(buf,li[n].line_contents);
	  replace_slash_with_ellipsis(buf);
	  items[listbox_index++] = XmStringDefCreate(buf);
	}
	else{
	  int m;
	  strcpy(buf,li[n].line_contents);
	  for(m=0;buf[m];m++)if(buf[m]=='\t')buf[m]=' ';
	  items[listbox_index++] = XmStringDefCreate(buf);
	}
      }
      /* should these compound strings be freed? */
    }
  }
  n = 0;
  XtSetArg(arg[n],XmNitemCount,item_count);n++;
  XtSetArg(arg[n],XmNitems,items);n++;
  XtSetValues(list=XmSelectionBoxGetChild(main_selection_box_id,XmDIALOG_LIST),
	      arg,n);
  /* now, if item_count>0, get selectedItemCount and, if 0, select item 1 */
  if(item_count>0){
    XtSetArg(arg[0],XmNselectedItemCount,&item_count);
    XtSetArg(arg[1],XmNselectedItems,&items);
    XtGetValues(list,arg,2);
    if(item_count==0  || !XmListItemExists(list,*items))
      XmListSelectPos(list,1,1);
  }
}

delete_line_item(indx)
int indx;
{
  int n;
  for(n=0;n<lc;n++){
    if(li[n].listbox_index==indx){
      li[n].listbox_index=0;
      li[n].resource_type= MRE_TYPE_DELETED; /*deleted*/
    }
    if(li[n].listbox_index &&
       li[n].listbox_index>indx)li[n].listbox_index--;
  }
  /* DONT lower lc; the deleted item is still in the array */
}

char *enumeration_choices(line)
int line;
{
  int n;
  for(n=0;n<lc;n++){
    if(li[n].line_number==line){
      return(enumerations[li[n].enum_type].enum_choices);
    }
  }
  return(NULL);
}

linenum_from_listindex(listbox_index)
int listbox_index;
{
  int n;
  for(n=0;n<lc;n++)
    if(li[n].listbox_index == listbox_index)
      return(li[n].line_number);
  /* -1 is impossible line number */
  return(-1);
}

listindex_from_linenum(line_number)
int line_number;
{
  int n;
  for(n=0;n<lc;n++)
    if(li[n].line_number == line_number)
      return(li[n].listbox_index);
  /* 0 indicates line not in listbox */
  return(0);
}

add_line_item(s,indx)
char *s;
int indx;
{
  int n;
  char *dup;
  for(n=0;n<lc;n++){
    if(li[n].listbox_index &&
       li[n].listbox_index>=indx)li[n].listbox_index++;
  }
  /* and create an entry in the li array for the new item */
  li[lc].line_number=lc;
  li[lc].resource_type=resource_type(s);
  if(li[lc].resource_type == MRE_TYPE_ENUM)
    li[lc].enum_type = get_enumerated_type();
  li[lc].listbox_index=indx;
  li[lc].continuation=0;
  /* ss8 listboxes die when fed zero length XmStrings */
  if(!strlen(s)){
    dup = (char *)calloc(2,sizeof(char));
    strcpy(dup,"!");
  }
  else{
    dup=strdup(s);
  }
  li[lc].line_contents=dup;

  /*increment top of sort flag */
  lc++;  
}

set_values_in_database(s,item_to_adjust)
char *s;
int item_to_adjust;
{
  char buf[MAX_ENTRY_SIZE];
  char *colon;
  char *comment;
  int n;
  int found_match=0;

  /* search for non-continuation line with line_number==item_to_adjust */
  for(n=0;n<lc;n++){
    if (!(li[n].continuation) && li[n].line_number==item_to_adjust){
      found_match=1;
      strcpy(buf,li[n].line_contents);
      break;/* to prevent n increment */
    }
  }
  if(!found_match)
    return;
  /* s contains rhs of spec, except for comment component */
  if(colon=strchr(buf,':')){
    colon++;
    while(*colon==' ')colon++;
    if(comment = strchr(colon,'!')){
      while(isspace(*(--comment)));
      ++comment;
      comment = strdup(comment);
    }
    *colon='\0';
    strcat(buf,s);
    if(comment){
      strcat(buf,comment);
      XtFree(comment);
    }
    free(li[n].line_contents);
    li[n].line_contents = strdup(buf);
  }
}

write_db(f)
FILE *f;
{
  int n;
  qsort(li,lc,sizeof(LineInfo),cmplinenums);
  for(n=0;n<lc;n++)
   if(li[n].resource_type != MRE_TYPE_DELETED)
     fprintf(f,"%s\n",li[n].line_contents);
}

void dump_db()
{
  int n;
  qsort(li,lc,sizeof(LineInfo),cmplinenums);
  for(n=0;n<lc;n++)
   printf("LINE %d TYPE %d ENUM %d LISTBOX %d CONT %d TEXT %s\n",
	  li[n].line_number,
	  li[n].resource_type,
	  li[n].enum_type,
	  li[n].listbox_index,
	  li[n].continuation,
	  li[n].line_contents);
}

has_colon(s)
char *s;
{
	if(strchr(s,':'))return(1);
	return(0);
}
is_comment(s)
char *s;
{
  while(isspace(*s))s++;
  return(*s == '!' ? 1 : 0);
}
is_hash(s)
char *s;
{
  while(isspace(*s))s++;
  return(*s == '#' ? 1 : 0);
}
is_blank(s)
char *s;
{
  while(isspace(*s))s++;
  return(*s  ? 0 : 1);
}

ends_in_slash(s)
char *s;
{
  int length;
  length = strlen(s);
  if(s[length-1]=='\\')
	return(1);
  else return(0);
}

/* WARNING -- buffer passed to this function must accomodate ellipsis */
replace_slash_with_ellipsis(s)
char *s;
{
  int length;
  length = strlen(s);
  s[length-1]='\0';
  strcat(s,"...");
}

