#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)VendorI.c	3.4 90/03/28";
#endif /* lint */
#endif /* REV_INFO */
/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990, OPEN SOFTWARE FOUNDATION, INC.
*  (c) Copyright 1987, 1988, 1989, 1990, HEWLETT-PACKARD COMPANY
*  (c) Copyright 1987, 1988, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
*  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY
*  ALL RIGHTS RESERVED
*  
*  	THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED
*  AND COPIED ONLY IN ACCORDANCE WITH THE TERMS OF SUCH LICENSE AND
*  WITH THE INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR
*  ANY OTHER COPIES THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE
*  AVAILABLE TO ANY OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF THE
*  SOFTWARE IS HEREBY TRANSFERRED.
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
*  Notice:  Notwithstanding any other lease or license that may pertain to,
*  or accompany the delivery of, this computer software, the rights of the
*  Government regarding its use, reproduction and disclosure are as set
*  forth in Section 52.227-19 of the FARS Computer Software-Restricted
*  Rights clause.
*  
*  (c) Copyright 1989, 1990, Open Software Foundation, Inc.  Unpublished - all
*  rights reserved under the Copyright laws of the United States.
*  
*  RESTRICTED RIGHTS NOTICE:  Use, duplication, or disclosure by the
*  Government is subject to the restrictions as set forth in subparagraph
*  (c)(1)(ii) of the Rights in Technical Data and Computer Software clause
*  at DFARS 52.227-7013.
*  
*  Open Software Foundation, Inc.
*  11 Cambridge Center
*  Cambridge, MA   02142
*  (617)621-8700
*  
*  RESTRICTED RIGHTS LEGEND:  This computer software is submitted with
*  "restricted rights."  Use, duplication or disclosure is subject to the
*  restrictions as set forth in NASA FAR SUP 18-52.227-79 (April 1985)
*  "Commercial Computer Software- Restricted Rights (April 1985)."  Open
*  Software Foundation, Inc., 11 Cambridge Center, Cambridge, MA  02142.  If
*  the contract contains the Clause at 18-52.227-74 "Rights in Data General"
*  then the "Alternate III" clause applies.
*  
*  (c) Copyright 1989, 1990, Open Software Foundation, Inc.
*  ALL RIGHTS RESERVED 
*  
*  
* Open Software Foundation is a trademark of The Open Software Foundation, Inc.
* OSF is a trademark of Open Software Foundation, Inc.
* OSF/Motif is a trademark of Open Software Foundation, Inc.
* Motif is a trademark of Open Software Foundation, Inc.
* X Window System is a trademark of the Massachusetts Institute of Technology
*
*******************************************************************************
******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <X11/Xatom.h>
#include <Xm/XmP.h>
#include <X11/StringDefs.h>

#include <X11/ShellP.h>

/************************************************************************
 *
 *  Font unit processing structures and data.
 *
 ************************************************************************/

#define DEFAULT_QUAD_WIDTH 10
#define HORIZONTAL	0
#define VERTICAL	1


typedef struct
{
   Display * display;
   int quad_width;
} FontUnitData;

static FontUnitData * font_unit_list = NULL;
static int list_size = 0;
static int list_count = 0;
static Display * font_unit_display = NULL;
static int quad_width = 0;

static void GetFontUnit();
extern void _XtSetFontUnit();



/************************************************************************
 *
 *  BuildResources
 *	Build up a new get values resource list based on a combination
 *	the the widget's class and super class resource list.
 *
 ************************************************************************/

void ShellBuildResources (wc_resources_ptr, wc_num_resources_ptr,
			sc_resources, sc_num_resources)
    ShellGetHookResource ** wc_resources_ptr;
    int                *  wc_num_resources_ptr;
    ShellGetHookResource *  sc_resources;
    int                   sc_num_resources;
{
   ShellGetHookResource * wc_resources;
   int                  wc_num_resources;
   ShellGetHookResource * new_resources;
   int                  new_num_resources;
   register int i, j;
   Boolean override;


   wc_resources = (ShellGetHookResource *) *wc_resources_ptr;
   wc_num_resources = (int) *wc_num_resources_ptr;


   /*  If there are no new resources, just use the super class data  */

   if (wc_num_resources == 0)
   {
      *wc_resources_ptr = sc_resources;
      *wc_num_resources_ptr = sc_num_resources;
      return;
   }


   /*  Allocate a new resource list to contain the combined set of  */
   /*  resources from the class and super class.  This allocation   */
   /*  may create to much space if there are overrides in the new   */
   /*  resource list.  Copy sc's resources into the space.          */

   new_resources = (ShellGetHookResource *) 
                   XtMalloc(sizeof (ShellGetHookResource) *
                   (wc_num_resources + sc_num_resources));
   bcopy (sc_resources, new_resources, 
            sc_num_resources * sizeof (ShellGetHookResource));


   /*  Loop through the wc resources and copy them into the new resources  */

   new_num_resources = sc_num_resources;

   for (i = 0; i < wc_num_resources; i++)
   {

      /*  First check to see if this is an override  */

      override = False;
      for (j = 0; j < sc_num_resources; j++)
      {
         if (new_resources[j].resource_name == wc_resources[i].resource_name)
         {
            override = True;
            new_resources[j].convert_proc = wc_resources[i].convert_proc;
            break;
         }
      }


      /*  If it wasn't an override stuff it into the list  */

      if (override == False)
         new_resources[new_num_resources++] = wc_resources[i];
   }


   /*  Replace the resource list and count will the newly generated one.  */

   *wc_resources_ptr = new_resources;
   *wc_num_resources_ptr = new_num_resources;
}

/************************************************************************
 *
 *  ShellInitializeGetValuesResources
 *	Initialize a get values resource list.  This is called out of
 *	Vendor, Manager and Gadgets class initialize to convert the
 *	resource names to quarks in the classes get values processing
 *	lists.
 *
 ************************************************************************/

void ShellInitializeGetValuesResources (resources, num_resources)
ShellGetHookResource * resources;
int num_resources;

{
   register int i;

   for (i = 0; i < num_resources; i++)
      resources[i].resource_name = 
         (String) XrmStringToQuark (resources[i].resource_name);
}




/************************************************************************
 *	
 * 
 * ShellProcessGetValues 
 *
 *      This procedure is used as the get values hook in Vendor, .  It uses
 *	the get values resource list attached to the class, comparing
 *	it to the input resource list, and for each match, calling the
 *	function to process the get value data.  *
 *	
 ************************************************************************/

void ShellProcessGetValues(w, resources, num_resources, args, num_args)
    Widget w;
    ShellGetHookResource * resources;
    int num_resources;
    ArgList args;
    Cardinal num_args;
    
{
   register int i, j;
   register XrmQuark quark;
   XtArgVal value;
   XtArgVal orig_value;
   int value_size;
   caddr_t value_ptr;


   /*  Loop through each argument, quarkifing the name.  Then loop  */
   /*  through each get value resource to see if there is a match.  */

   for (i = 0; i < num_args; i++)
   {
      quark = XrmStringToQuark (args[i].name);

      for (j = 0; j < num_resources; j++)
      {
         if ((XrmQuark)(resources[j].resource_name) == quark)
         {
            value_size = resources[j].resource_size;
            value_ptr = ((caddr_t) w) + resources[j].resource_offset;

            if (value_size == 1) value = (XtArgVal)(*(char *)value_ptr);
            else if (value_size == 2) value = (XtArgVal)(*(short *)value_ptr);
            else if (value_size == 4) value = (XtArgVal)(*(long *)value_ptr);
            orig_value = value;

            (*(resources[j].convert_proc))(w, quark, &value);

            if (orig_value == args[i].value)
            {
               args[i].value = value;
            }
            else
            {
               value_ptr = (caddr_t) args[i].value;

               if (value_size == 1) *(char *) value_ptr = (char) value;
               else if (value_size == 2) *(short *) value_ptr = (short) value;
               else if (value_size == 4) *(long *) value_ptr = (long) value;
            }
            break;
         }
      }
   }
}


/************************************************************************
 *
 *  ShellToHorizontalPixels
 *	Convert from a non-pixel unit type to pixels using the 
 *	horizontal resolution of the screen.  This function is
 *	accessed from a widget.
 *
 ************************************************************************/

int ShellToHorizontalPixels (widget, unit_type, value)
register Widget widget;
register unsigned char unit_type;
int value;

{
   static Screen * screen = NULL;
   static int mm_per_pixel;
   register int value_in_mm;


   /*  Check for type to same type conversions  */

   if (unit_type == XmPIXELS) return (value);


   /*  Pre-process FontUnit types  */

   if (unit_type == Xm100TH_FONT_UNITS)
   {
      GetFontUnit (widget);
      return (value * quad_width / 100);
   }


   /*  Get the screen dimensional data  */

   if (screen != XtScreen (widget))
   {
      screen = XtScreen (widget);
      mm_per_pixel = (WidthMMOfScreen(screen) * 100) / WidthOfScreen(screen);
   }


   /*  Convert the data  */

   if (unit_type == Xm100TH_POINTS)
      value_in_mm = (value * 353) / 1000;

   else if (unit_type == Xm1000TH_INCHES)
      value_in_mm = (value * 254) / 100;

   else if (unit_type == Xm100TH_MILLIMETERS)
      value_in_mm = value;


   return (value_in_mm / mm_per_pixel);
}




/************************************************************************
 *
 *  ShellToVerticalPixels
 *	Convert from non-pixel unit type to pixels using the 
 *	vertical resolution of the screen.  This function is
 *	accessed from a widget.
 *
 ************************************************************************/

int ShellToVerticalPixels (widget, unit_type, value)
register Widget widget;
register unsigned char unit_type;
int value;

{
   static Screen * screen = NULL;
   static int mm_per_pixel;
   register int value_in_mm;


   /*  Check for type to same type conversions  */

   if (unit_type == XmPIXELS) return (value);


   /*  Pre-process FontUnit types  */

   if (unit_type == Xm100TH_FONT_UNITS)
   {
      GetFontUnit (widget);
      return (value * quad_width / 100);
   }


   /*  Get the screen dimensional data  */

   if (screen != XtScreen (widget))
   {
      screen = XtScreen (widget);
      mm_per_pixel = (HeightMMOfScreen(screen) * 100) / HeightOfScreen(screen);
   }

   /*  Convert the data  */

   if (unit_type == Xm100TH_POINTS)
      value_in_mm = (value * 353) / 1000;

   else if (unit_type == Xm1000TH_INCHES)
      value_in_mm = (value * 254) / 100;

   else if (unit_type == Xm100TH_MILLIMETERS)
      value_in_mm = value;


   return (value_in_mm / mm_per_pixel);
}




/************************************************************************
 *
 *  ShellFromHorizontalPixels
 *	Convert from a pixel unit type to a non-pixels using the 
 *	horizontal resolution of the screen.  This function is
 *	accessed from a getvalues hook table.
 *
 ************************************************************************/

void ShellFromHorizontalPixels (widget, resource, value)
register Widget widget;
XrmQuark resource;
XtArgVal * value;

{
   static Screen * screen = NULL;
   static int mm_per_pixel;
   register unsigned char unit_type;
   register int value_in_mm;


   /*  Get the unit type of the widget  */

   unit_type = VPE(widget)->unit_type;

   /*  Check for type to same type conversions  */

   if (unit_type == XmPIXELS) return;


   /*  Pre-process FontUnit types  */

   if (unit_type == Xm100TH_FONT_UNITS)
   {
      GetFontUnit (widget);
      *value = (*value) * 100 / quad_width;
   }


   /*  Get the screen dimensional data  */

   if (screen != XtScreen (widget))
   {
      screen = XtScreen (widget);
      mm_per_pixel = (WidthMMOfScreen(screen) * 100) / WidthOfScreen(screen);
   }


   /*  Convert the data  */

   value_in_mm = (*value) * mm_per_pixel;

   if (unit_type == Xm100TH_POINTS)
      *value = (XtArgVal) ((value_in_mm * 1000) / 353);

   else if (unit_type == Xm1000TH_INCHES)
      *value = (XtArgVal) ((value_in_mm * 100) / 254);

   else if (unit_type == Xm100TH_MILLIMETERS)
      *value = (XtArgVal) (value_in_mm);
}




/************************************************************************
 *
 *  ShellFromVerticalPixels
 *	Convert from pixel unit type to non-pixels using the 
 *	vertical resolution of the screen.  This function is
 *	accessed from a getvalues hook table.
 *
 ************************************************************************/

void ShellFromVerticalPixels (widget, resource, value)
register Widget widget;
XrmQuark resource;
XtArgVal * value;

{
   static Screen * screen = NULL;
   static int mm_per_pixel;
   register unsigned char unit_type;
   register int value_in_mm;


   /*  Get the unit type of the widget  */

   unit_type = VPE(widget)->unit_type;



   /*  Check for type to same type conversions  */

   if (unit_type == XmPIXELS) return;


   /*  Pre-process FontUnit types  */

   if (unit_type == Xm100TH_FONT_UNITS)
   {
      GetFontUnit (widget);
      *value = (*value) * 100 / quad_width;
   }


   /*  Get the screen dimensional data  */

   if (screen != XtScreen (widget))
   {
      screen = XtScreen (widget);
      mm_per_pixel = (HeightMMOfScreen(screen) * 100) / HeightOfScreen(screen);
   }


   /*  Convert the data  */

   value_in_mm = (*value) * mm_per_pixel;

   if (unit_type == Xm100TH_POINTS)
      *value = (XtArgVal) ((value_in_mm * 1000) / 353);

   else if (unit_type == Xm1000TH_INCHES)
      *value = (XtArgVal) ((value_in_mm * 100) / 254);

   else if (unit_type == Xm100TH_MILLIMETERS)
      *value = (XtArgVal) (value_in_mm);
}



/************************************************************************
 ************************************************************************

	Font unit handling functions

 ************************************************************************
 ************************************************************************/


/************************************************************************
 *
 *  SetFontUnit
 *	Set the quad_width value for the display.  These values can
 *	then be used later to process the font unit conversions.
 *
 ************************************************************************/

void _XtSetFontUnit (display, value)
Display * display;
int value;

{
   register int i;


   /*  See if the display is already in the list  */

   for (i = 0; i < list_count; i++)
      if ((font_unit_list + i)->display == display)
      {
         (font_unit_list + i)->quad_width = value;
         return;
      }


   /*  See if more space is needed  */

   if (list_size == list_count)
   {
      list_size += 10;
      font_unit_list =
         (FontUnitData *) XtRealloc (font_unit_list, 
                                       sizeof(FontUnitData) * list_size);
   }
          

   /*  Add the new display and value to the font list  */

   (font_unit_list + list_count)->display = display;
   (font_unit_list + list_count)->quad_width = value;
   list_count++;
}




/************************************************************************
 *
 *  GetFontUnit
 *	Get the font unit for the provided display.  If one is not
 *	found get the default for the display by accessing the default
 *	font in the resource data base or by using the default font name.
 *
 ************************************************************************/

static void GetFontUnit (widget)
Widget widget;

{
   Display * display = XtDisplay (widget);
   register int i;
   XFontStruct * font_struct;
   unsigned long quad_width_return;

   static XtResource resources[] =
   {
      { 
         XtNfont, XtCFont, XtRFontStruct, 
         sizeof (XFontStruct *), 0, XtRString, "Fixed" 
      }
   };


   /*  If the display has not changed, return  */

   if (display == font_unit_display)
      return;


   /*  See if the display is already in the list  */

   for (i = 0; i < list_count; i++)
      if ((font_unit_list + i)->display == display)
      {
         font_unit_display = display;
         quad_width = (font_unit_list + i)->quad_width;
         return;
      }


   /*  Get the global font or default font by making a resource      */
   /*  data base query.  If we get a font struct, get the value for  */
   /*  the QUAD_WIDTH otherwise set the quad_width to a default.     */

   XtGetApplicationResources (widget, &font_struct, resources, 1, NULL, 0);

   if (font_struct != NULL)
   {
      if (XGetFontProperty (font_struct, XA_QUAD_WIDTH, &quad_width_return))
         quad_width = quad_width_return;
      else
         quad_width = DEFAULT_QUAD_WIDTH;

      XFreeFont (display, font_struct);
   }
   else
      quad_width = DEFAULT_QUAD_WIDTH;


   font_unit_display = display;
   _XtSetFontUnit (display, quad_width);
}
