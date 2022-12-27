#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)utils.c	1.6 90/08/01";
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
 **   File:     @(#)utils.c	1.6 - 90/08/01
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
#include "widgetView.h"
#include "widgets.h"
#include "globals.h"
#include "callbacks.h"

/**********************************************************/
/* pack a list of strings                                 */
/**********************************************************/
XmString *PackCompoundStrings (normstring, cnt)
char *normstring;
int *cnt;
{

  int i=0, n=0, list_found = 0;
  char *arry[100];
  static XmString *tcs;

  *cnt = 0;
  if (!normstring)
    return ((XmString *) NULL);

  while (normstring [n] != NULL) {
    /* check to see if a comma exists in the string */
    if (normstring [n++] == ',') {
      /* if a comma is found, we have a list, so process */
      arry [i++] = strtok (normstring, ",");
      while ((arry [i++] = strtok (NULL, ",")) != (char *) NULL);
      *cnt = i - 1;
      if ( i > 0) {
	  tcs = (XmString *) calloc (i, sizeof (XmString));
	  for (n = 0; n < i - 1; n++) {
	      /* allocate a new tcs */
	      tcs [n] = XmStringLtoRCreate (arry [n], 
				      XmSTRING_DEFAULT_CHARSET);
	      printf ("arry [%d] = %s\n", n, arry [n]);
	  }
	  list_found = 1;
	  printf ("list found....str = %s\n", normstring);
	  break;
      }
    }
  } /* while */

  if (!list_found) {
    printf ("no list....normal packing, str = %s\n", normstring);
    tcs = (XmString *) calloc (1, sizeof (XmString));
    tcs [0] = XmStringLtoRCreate (normstring, 
				  XmSTRING_DEFAULT_CHARSET);
  }

  return (tcs);
  

}
/**********************************************************/
/* prepare a list of strings which is UIL compatible
   used in the string_table comand of uil
   format: string_table ("item1", "item2", ...);
   this routine takes a string item1, item2,....
   and puts quotes around each word */
/**********************************************************/
char *PrepareUILStrings (normstring)
char *normstring;
{

  int i, n;
  char *arry[100];
  char buffer [50];
  static char *retstr;

  if (!normstring)
    return ((char *) NULL);

  i = 0;
  arry [i++] = strtok (normstring, ",");
  while ((arry [i++] = strtok (NULL, ",")) != (char *) NULL);
    
  /* allocate a return string which is the
     original string plus has quotes around each word */

  retstr = (char *) calloc (1, strlen (normstring) + i*2);

  /* normalize i */
  i--;
  for (n = 0; n < i; n++) {
    strcat (retstr, "\"");
    strcat (retstr, arry [n]);
    strcat (retstr, "\"");
    strcat (retstr, ",");
  }
  /* null out the last comma */
  retstr [strlen (retstr)-1] = 0;
  return ((char *) retstr);
}
/**********************************************************/
/* puts the physical name of a string into the additional field */
/* this avoids having to unpack a compound string at the  */
/* time of code generation.   this saves the ASCII representation */
/* of an associated string......                         */
/*********************************************************/
PackString (normstring, position)
char *normstring;
int position;
{

	
  if (normstring != (char *) NULL) {
    resources->additional [position]->name = 
      (char *) calloc (1, strlen (normstring));
    strcpy (resources->additional [position]->name, normstring);
  }
  else {
    resources->additional [position]->name = 
      (char *) calloc (1, strlen (STRING_IS_NULL));
    strcpy (resources->additional [position]->name, STRING_IS_NULL);
  }
}

/**********************************************************/
GetValuePosition (widget)
Widget widget;

{
  
  int i;

  for (i=0; i<MAX_ARGS; i++) {
    if (widget == resources->widget_addr [i])
      return (i);
  }
  return (-1);
}

/**********************************************************/
char *GetWidgetNameFromBuffer (buffer)
char *buffer;
{

  char *tmpbuff, *holdbuff;
  char *retbuff;
  static char *sendbuff;

  sendbuff = (char *) NULL;
  if ((tmpbuff = strpbrk (buffer, "WIDGET NAME:")) != NULL) {
    holdbuff = (char *) calloc (1, strlen (tmpbuff));
    strcpy (holdbuff, tmpbuff);
    retbuff = strtok (holdbuff, " \n");   /* gets WIDGET */
    retbuff = strtok (NULL, " \n");   /* gets NAME: */
    retbuff = strtok (NULL, " \n");   /* gets the widget */
    sendbuff = (char *) calloc (1, strlen (retbuff));
    strcpy (sendbuff, retbuff);
    free (holdbuff);
  }
  return ((char *) sendbuff);
  

}

/**********************************************************/
/* update the resources for a particular widget, given a
   buffer from a file.
   the buffer will have the format 
   WIDGET NAME: <widget>
   resourceName1    resourceType1        value
   resourceName2    resourceType2        value
   ...
   resourceNameN    resourceTypeN        value

   All of the resources->...structure at this point have been
   loaded, so simply compare the names with the buffer
   and update those which are the same.
   
   Then do a setvalue (args) and  AutoUpdate to set the sliders
*/
typedef struct _nv {
  char *name;
  int  widget_type;    /* TOGGLE_BUTTON, SCALE... */
  char *value;        
}NAME_VALUE;

/**********************************************************/
SetResources (buffer)
char *buffer;

{
  Arg  args[MAX_ARGS];
  int i, num_names, n = 0, offset;
  NAME_VALUE **nv, **GetNameAndValue ();
  long ConvertNameAndValue ();

  if (ActiveWidget.address != (Widget) NULL) {


    /* let's set the orgvalue field with any common
       names we find in the buffer */

    nv = GetNameAndValue (buffer, &num_names);

    /* ok we have the resources in the nv structure */

    for (i = 0; i<num_names; i++) {
      for (offset = 0; offset < resources->num_items; offset++) {
	if (!strcmp (resources->name [offset], nv [i]->name)) {
	  if (resources->editable [i] == True)

	    /* want to add a check to only update those
	       things which are indeed different from orgvalue */

	    if ((resources->orgvalue [offset] = 
		 ConvertNameAndValue (nv, i)) == (long) FUNC_ERROR) {
	      free_nv (nv, num_names);
	      return (FUNC_ERROR);
	    }



	  offset = resources->num_items;
	}
      }
    }


    for (i = 0; i<resources->num_items; i++) {
      AutoUpdateValueField (resources->widget_addr [i],
			    resources->orgvalue [i],
			    i);
    }

    /* now apply all the changes */

    ApplyResources ();
  }
  else
    PrintError ("active widget not selected.");
  
  free_nv (nv, num_names);

  return  (FUNC_OK);

}

/**********************************************************/
free_nv (nv, num_names)
NAME_VALUE **nv;
int num_names;
{
  int i;

  for (i=0; i<num_names; i++) {
    free (nv [i]->value);
    free (nv [i]->name);
    free (nv [i]);
  }
  free (nv);
}


/**********************************************************/
NAME_VALUE **GetNameAndValue (buffer, num_names)
char *buffer;
int *num_names;
{

  int i, error_flag = 0;
  char *tmpbuff, *holdbuff;
  static NAME_VALUE **nv;

  *num_names = 0;
  if ((tmpbuff = strpbrk (buffer, "WIDGET NAME:")) != NULL) {
    holdbuff = (char *) calloc (1, strlen (tmpbuff) + 1);
    strcpy (holdbuff, tmpbuff);
    for (i=0; i<strlen (holdbuff); i++) {
      if (holdbuff [i] == '\n')
	(*num_names)++;
    }
    (*num_names)--;

    if (*num_names <= 0) 
	nv = (NAME_VALUE **) calloc (1, sizeof (NAME_VALUE));
    else
	nv = (NAME_VALUE **) calloc (*num_names, sizeof (NAME_VALUE));


    strtok (holdbuff, "\n");   /* gets the first line  */
    for (i=0; i<*num_names; i++) {
      nv[i] = (NAME_VALUE *) calloc (1, sizeof (NAME_VALUE));
      tmpbuff = strtok (NULL, "\t\n");   /* gets the name */
      nv[i]->name = (char *) calloc (1, strlen (tmpbuff) + 1);
      strcpy (nv[i]->name, tmpbuff); /* copy the stuff right into some space */

      tmpbuff = strtok (NULL, "\t\n");   /* gets the resource type  */
      nv[i]->widget_type = GetFieldType (tmpbuff, -1, NULL, NULL);

      tmpbuff = strtok (NULL, "\t\n");  /* now the value */
      nv[i]->value = (char *) calloc (1, strlen (tmpbuff) + 1);
      strcpy (nv[i]->value, tmpbuff); /* copy the value */

      tmpbuff = (char *) NULL;

    }
    free (holdbuff);
  }
  
  /* and now we return with three pieces of data:
     1. the resource name;
     2. the widget type to be displayed (TOGGLE BUTTON, SCALE etc.)
     3. the value in string form
  */

  return ((NAME_VALUE **) nv);

}

/**********************************************************/
long ConvertNameAndValue (nv, position)
NAME_VALUE **nv;
int position;
{

  int cnt;

  switch (nv [position]->widget_type) {
    
      case SCALE:
                    /* just convert the string into an int and
		       get the hell out */

                    return (atol (nv [position]->value));
		    break;
     
     case TOGGLE_BUT:
		    
		    /* here we take a True or False entry and simply
		       convert the string...for now the file outputs
		       a zero for False and non-zero for true */

		    if (!strcmp (nv [position]->value, "False") ||
			!strcmp (nv [position]->value, "0"))
		      return ((long) False);
		    else
		      return ((long) True);
		    break;

      case CPD_STRING:
		    
		    printf ("got a compound string - %s\n", 
			    nv [position]->value);
		    if (!strcmp (nv [position]->value, STRING_IS_NULL) ||
			!strcmp(nv [position]->value, "0"))
		      return ((long) NULL);
		    else
		      return ((long) 
			      XmStringLtoRCreate (nv [position]->value, 
						  XmSTRING_DEFAULT_CHARSET));
		    break;

      case STRING:
		   
		    printf ("got a string - %s\n", nv [position]->value);
		    return ((long) nv [position]->value);
		    break;

      case CALLBACKS:
		    
		    if (!strcmp(nv [position]->value, "OFF") ||
			!strcmp(nv [position]->value, "0"))
		      return ((long) NULL);
		    else {
		      printf ("in ConvertNameAndValue callback = %ld\n", 
			      GenericCallback);
		      return ((long) GenericCallback);
		    }
		    break;

      case COLOR:

		    for (cnt = 0; cnt < colortype.num_colors; cnt++) {
		      if (!strcmp (colortype.color_list [cnt].name,
				   nv [position]->value))
			return ((long) colortype.color_list [cnt].pixel);
		    }
		    return (atol (nv [position]->value));
		    break;
      default:
		    
		    printf ("resource type in ConvertNameAndValue not found\n");
		    printf ("position = %d, name = %s, type = %d\n",
			    position,
			    nv [position]->name,
			    nv [position]->widget_type);
		    return ((long) FUNC_ERROR);
		    break;

      } /* switch */
}

/**************************************************************/
PrintError (msg)
char *msg;

{

  Arg  args[MAX_ARGS];
  int  n = 0;
  XmString      tcs;

  tcs = XmStringLtoRCreate(msg, XmSTRING_DEFAULT_CHARSET);
  XtManageChild (ErrorDialog);
  
  XtSetArg(args[n], XmNmessageString, tcs);  n++;
  XtSetValues (ErrorDialog, args, n);
  XmStringFree(tcs);
  
}

/**************************************************************/
QuestionMsg (msg)
char *msg;

{

  Arg  args[MAX_ARGS];
  int  n = 0;
  XmString      tcs;

  tcs = XmStringLtoRCreate(msg, XmSTRING_DEFAULT_CHARSET);
  XtManageChild (QuestionDialog);
  
  XtSetArg(args[n], XmNmessageString, tcs);  n++;
  XtSetValues (QuestionDialog, args, n);
  XmStringFree(tcs);
  
}

/***********************************************************/
XmString *DoTheFont (count)
int *count;
{

     char **fontlist;
     int i;
     static XmString *addrstr;

     fontlist = XListFonts (display, "*", 100, count);
     addrstr = (XmString *) calloc (*count, sizeof (XmString));
     printf ("num = %d\n\r", *count);
     for (i = 0; i < *count; i++) {
       addrstr[i] = XmStringLtoRCreate (fontlist[i], 
					XmSTRING_DEFAULT_CHARSET);
     }

     return (addrstr);
 }
 /***********************************************************/
 char *GetSource (fileptr)
 char *fileptr;
 {

   static char *retbuff;
   int fd, flen;
   char buff [256];

   if ((fd = open (fileptr, O_RDONLY)) < 0) {
    sprintf (buff, "Cannot open file %s\n", fileptr);
    PrintError (buff);
    return ((char *) NULL);
  }
  flen = GetFileLen(fd);
  retbuff = (char*) calloc (1, flen + 1);
  if (read (fd, retbuff, flen) <= 0) {
    sprintf (buff, "Error reading file %s\n", fileptr);
    PrintError (buff);
    return ((char *) NULL);
  }
  close (fd);
  return (retbuff);
}

/***********************************************************/
GetFileLen (fd)
int fd;
{
  static int retval;

  lseek (fd, 0, L_SET);  
  retval = lseek (fd, 0, L_XTND);
  lseek (fd, 0, L_SET);  

  return (retval);

}

/***********************************************************/
char *PackValueBuffer (message)
char *message;
{

  char *value_buffer;
  int i, offset = 0;

  value_buffer = (char *) calloc (1, 
		 strlen (message) + (strlen (message) % 80)*5);
  

  value_buffer [offset ++] = '\"';
  for (i=0; i<strlen (message); i++) {
    if (!(i%80) && (i>0)) {
      value_buffer [i + offset] = '\"';
      value_buffer [i + ++offset] = ' ';
      value_buffer [i + ++offset] = '&';
      value_buffer [i + ++offset] = '\n';
      value_buffer [i + ++offset] = '\"';

      /* make sure that the character we're replacing
	 is not a /n...it would be a rare case when the /n
	 happened at the 80th character...but check to make sure */

      if (message [i] != '\n')
	value_buffer [i + ++offset] = message [i];

    }
    else
      if (message [i] == '\n') {
	value_buffer [i + offset] = '\\';
	value_buffer [i + ++offset] = 'n';
      }
      else
	value_buffer [i + offset] = message [i];
  }
  value_buffer [i + offset++] = '\"';
  value_buffer [i + offset] = ';';
  return (value_buffer);
}


/***********************************************************/
char *ConvertName (name)
char *name;

{
  int i;
  static char buffer [256];

  if (!name)
    return ((char *) NULL);

  strcpy (buffer, name);
  for (i=0; i<strlen (name); i++)
    if (name [i] == ' ')
      buffer [i] = '_';

  buffer [i] = 0;
  return ((char *) buffer);

}

/***********************************************************/
char *UnconvertName (name)
char *name;

{
  int i;
  static char buffer [256];

  if (!name)
    return ((char *) NULL);

  strcpy (buffer, name);
  for (i=0; i<strlen (name); i++)
    if (name [i] == '_')
      buffer [i] = ' ';

  buffer [i] = 0;
  return ((char *) buffer);

}

/***********************************************************/
/* support routine to get normal string from XmString */
char *extract_normal_string(cs)
XmString cs;
{

  XmStringContext context;
  XmStringCharSet charset;
  XmStringDirection direction;
  Boolean separator;
  static char *primitive_string;


  if (cs == (XmString) NULL)
    return ((char *) NULL);

  XmStringInitContext (&context,cs);
  XmStringGetNextSegment (context,&primitive_string,
			  &charset,&direction,&separator);
  XmStringFreeContext (context);
  return ((char *) primitive_string);
}










