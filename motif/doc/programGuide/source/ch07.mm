.mc |
.\"  (c) Copyright 1989, 1990 OPEN SOFTWARE FOUNDATION, INC.
.mc
.\"  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
.\"  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
.\"  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
.\"  (c) Copyright 1988 MICROSOFT CORPORATION
.\"  ALL RIGHTS RESERVED
.nr H1 6
.mc |
.nr !_ 0
.mc
.H 1 "Specialized Widgets"
.iX "Specialized widgets"
.iX "Widget" "specialized"
.mc |
This chapter explains the use of the following three
specialized widgets:
.mc
.iX "List"
.iX "RowColumn"
.iX "Text"
.BL
.LI
.mc *
.mc
.LI
List
.LI
RowColumn
.LI
Text
.LE
.P
.mc |
The Form widget is explained in Chapter 5, ``Dialog Widgets.''  The others
.mc
are explained in this chapter.
.mc |
.br
.ne 15
.mc
.H 2 "List Widget"
The List widget allows you to make a selection from a list of items.  The
application defines an array of compound strings, each of which becomes
an item in the list.  You can set the number of items in the list that
are to be visible.  You can also choose to have the List appear with a
ScrollBar so that you can scroll through the list of items.  Items are
selected by moving the pointer to the desired item and pressing the mouse
.mc |
button or key defined as select.  The selected item is displayed in 
.mc
inverse color.
.H 3 "List Functions"
.iX "List" "functions"
.iX "Functions" "List widget"
.mc |
The List widget has a number of functions that can
perform a variety of tasks.  These functions are listed in the following table
and each has its own manpage in the \fIOSF/Motif Programmer's Reference\fP.  
.SK
.in0
.mc
.iX "XtCreateWidget"
.iX "XmCreateList"
.iX "XmCreateScrolledList"
.iX "XmListAddItem"
.mc |
.iX "XmListAddItems"
.mc
.iX "XmListAddItemUnselected"
.mc |
.iX "XmListDeleteAllItems"
.mc
.iX "XmListDeleteItem"
.mc |
.iX "XmListDeleteItems"
.iX "XmListDeleteItemsPos"
.mc
.iX "XmListDeletePos"
.mc *
.mc
.iX "XmListDeselectAllItems"
.mc |
.iX "XmListDeselectItem" 
.iX "XmListDeselectPos"
.iX "XmListGetMatchPos"
.iX "XmListGetSelectedPos"
.iX "XmListItemExists"
.iX "XmListItemPos"
.iX "XmListReplaceItems"
.iX "XmListReplaceItemsPos"
.mc
.iX "XmListSelectItem"
.mc |
.iX "XmListSelectPos"
.iX "XmListSetAddMode"
.iX "XmListSetBottomItem"
.iX "XmListSetBottomPos"
.mc
.iX "XmListSetHorizPos" 
.iX "XmListSetItem"
.iX "XmListSetPos"
.mc *
.mc
.TB "List Widget Functions"
.TS
center,box, tab(;);
lb | lb
lb | l .
.mc
.sp .2
Function;Description
.sp .2
_
.sp .2
.mc |
XtCreateWidget;Basic widget creation function. 
XmCreateList;Specific creation function for XmList.
XmCreateScrolledList;Creation function for ScrolledList.
XmListAddItem;Adds an item (possibly selected) to the list.
XmListAddItems;Adds items (possibly selected) to the list.
XmListAddItemUnselected;Adds an item (unselected) to the list.
XmListDeleteAllItems;Deletes all items from the list.
XmListDeleteItem;Deletes an item from the list.
XmListDeleteItems;Deletes items from the list.
XmListDeleteItemsPos;T{
Deletes items from the list starting at
the specified position.
T}
XmListDeletePos;Deletes an item at the specified position.
XmListDeselectAllItems;T{
.na
Unhighlights all items in the list
and removes them from the selected list.
.ad
T}
.mc
XmListDeselectItem;T{
.na
.mc |
Unhighlights the specified item and
removes it from the selected list.
.mc
.ad
T}
.mc |
XmListDeselectPos;T{
.mc
.na
.mc |
Unhighlights the item at the specified position
and removes it from the selected list.
.mc
.ad
T}
.mc |
XmListGetMatchPos;T{
.mc
.na
.mc |
Returns a list of all positions where the
specified item is found in the list.
.mc
.ad
T}
.mc |
XmListGetSelectedPos;T{
.mc
.na
.mc |
Returns a list of positions of all currently
selected items in the list.
.mc
.ad
T}
.mc |
XmListItemExists;Returns True if the specified item is present in the list.
XmListItemPos;Returns the position of the specified item in the list.
XmListReplaceItems;Replaces some existing list items with new items.
XmListReplaceItemsPos;T{
.mc
.na
.mc |
Replaces the specified number of existing items
with new items, starting at the specified position.
.mc
.ad
T}
.mc |
XmListSelectItem;T{
.na
Highlights the specified item and add it
to the selected list.
.ad
T}
XmListSelectPos;T{
.na
Highlights the item at the specified position
and adds it to the selected list.
.ad
T}
XmListSetAddMode;Turns Add Mode on or off in the list.
.mc
XmListSetBottomItem;T{
.na
.mc |
Makes the specified item the last
visible item in the list.
.mc
.ad
T}
XmListSetBottomPos;T{
.na
.mc |
Makes the item at the specified position
the last visible item in the list.
.mc
.ad
T}
.mc |
XmListSetHorizPos;T{
.mc
.na
.mc |
If the horizontal ScrollBar is visible, sets the
XmNvalue resource of the ScrollBar to the specified
position and updates the visible portion of the list.
.mc
.ad
T}
.mc |
XmListSetItem;T{
.mc
.na
.mc |
Makes the specified item the first visible
item in the list.
.mc
.ad
T}
.mc |
XmListSetPos;T{
.na
Makes the item at the specified position
the first visible item in the list.
.ad
T}
.mc
.sp .2
.TE
.mc |
.in
.br
.ne 5
.mc
.P
The use of many of these functions will be described in the sections
that follow.  Actual code segments accompanied by illustrations show
the results of certain programming actions.
.H 3 "Using the List Widget"
.iX "List" "using"
.iX "Widget" "using List"
.iX "Using the List widget"
.mc |
The figure that follows shows an example of a List widget.  You have seen other examples
in earlier chapters of this guide.  For example, the lists displayed in the
FileSelectionBox and SelectionBox widgets are List widgets.
.mc
.ig ++
.br
.ie \nZ \{\
.br
.ne 24.5P
.C
.sC graphics/list1.tif tiff 300 10P 22.5P
\&
.P
.sp +1
.FG "Example of a List Widget"
.sp +1.5
.\}
.el \{\
.br
.ne 24.5P
\&
.sp 22.5P
.ce
illustration list1.eps goes here
.sp +1
.FG "Example of a List Widget"
.sp +1.5
.\}
.++
.br
.mc |
.ne 4i
.mc
\0
.mc *
.mc
.FG "Example of a List Widget"
.mc |
.sp 3.75i
.in +1.94i
.P! graphics/list1.ps 
.in -1.94i
.mc
.P
.mc |
The program that produces the List widget just shown can be found
in the \fB\&./demos/xmsamplers/xmlist.c\fP file.
You can compile and link this program
using the procedure described in Chapter 1.  Segments of this program
.mc
are used in this section to describe how to accomplish certain functions
.mc |
associated with the List widget.
.br
.ne 6
.mc
.P
.mc |
In the figure, the two lines that appear in inverse video indicate
.mc
items that have
been selected by the application.  Any number of items can be selected.
This is accomplished as shown in the following code segment.
.mc |
.br
.ne 8
.mc
.P
.mc |
.in0
.S -2
.mc
.ft CW
.nf
   Arg Args[20];

   static char *CharSelectedItems[2] = {
   "New Item List",
   "New Policy"};

   #define NUM_SELECTED_ITEMS 2
   int i;
   XmStringCharSet  cs = "ISOLatin1";
   XmString SelectedItems[NUM_SELECTED_ITEMS];

/* Create compound strings for selected items */
   for (i = 0; i < NUM_SELECTED_ITEMS; i++)
.mc |
   SelectedItems[i] =
               (XmString) XmStringCreateLtoR(CharSelectedItems[i],cs);
.mc

/* Set the resource values */
   i = 0;
.mc |
   XtSetArg(args[n], XmNselectedItems, (XtArgVal) SelectedItems);
   i++;
   XtSetArg(args[n], XmNselectedCount, (XtArgVal) NUM_SELECTED_ITEMS);
   i++;
.mc
.fi
.mc |
.S
.in
.mc
.ft P
.P
As you can see, you can include as many items as you want in the selected
list.
.P
.mc |
In the next figure, you can see that one of the items in the list is Five Visible.
.mc
To select this item, move the pointer into the window and position
.mc |
it anywhere on the Five Visible line.  Clicking \fBBSelect\fP selects
.mc
the item and it is highlighted, as shown in 
.mc |
Figure 7-2.
.mc
.ig ++
.br
.ie \nZ \{\
.br
.ne 24.5P
.P
.sC graphics/list2.tif tiff 300 10P 22.5P
\&
.P
.sp +1
.FG "List Widget Before Selection Action"
.sp +1.5
.\}
.el \{\
.br
.ne 24.5P
\&
.sp 22.5P
.ce
illustration list2.eps goes here
.sp +1
.FG "List Widget Before Selection Action"
.sp +1.5
.\}
.++
.br
.mc |
.ne 4i
.mc
\0
.mc *
.mc
.FG "List Widget Before Selection Action"
.mc |
.sp 3.75i
.in +1.94i
.P! graphics/list2.ps 
.in -1.94i
.mc
.P
.iX "Double click"
.mc |
When you double-click \fBBSelect\fP, the action is performed.
.mc
In this case, now only five items are displayed in the list, as shown in 
.mc |
the following figure:
.mc
.ig ++
.br
.ie \nZ \{\
.br
.ne 8P
.P
.sC graphics/list2a.tif tiff 300 11P 6P
\&
.P
.sp +1
.mc |
.FG "List Widget After Selecting Five Visible
.mc
.sp +1.5
.\}
.el \{\
.br
.ne 8P
\&
.sp 8P
.ce
illustration list2a.eps goes here
.sp +1
.FG "List Widget After Selecting Five Visible"
.sp +1.5
.\}
.++
.br
.mc |
.ne 1.25i
.mc
\0
.mc *
.mc
.FG "List Widget After Selecting Five Visible"
.mc |
.sp 1i
.in +1.94i
.P! graphics/list2a.ps 
.in -1.94i
.mc
.P
.mc |
Note the ScrollBar in the preceding figure.  There are the
.mc
same number of items in the list as before, but now only five of them are
visible at any given time.
.H 4 "Callback"
.mc |
The callback procedure that is executed when a double-click occurs performs
.mc
the necessary steps to accomplish the task given by the selected item in
.mc |
the list.  In the preceding 
situation, the task was to make only five items visible at a time.  The code segments to
accomplish this are as follows:  
.sp 2
.mc
.P
.iX "XtAddCallback"
.ft CW
.nf
.mc |
.S -2
.in0
.mc
/* Add the callback */
.mc |
XtAddCallback(outer_box, XmNdefaultActionCallback,
                                            DoubleClickProc, NULL);
.mc

.mc |
/**********************************************************************
.mc
 *               *
.mc |
 * DoubleClickProc is the XmDEFAULT_ACTION callback.  It functions as a
 * big case statement, comparing the item that was double-clicked to  *
.mc
 * the items in the list.  When it finds a match, it takes the    *
 * appropriate action.           *
 *               *
.mc |
 *********************************************************************/
.mc
void  DoubleClickProc(w,closure,call_data)
  Widget w;
  caddr_t  closure, call_data;
{
  int j;
  XmListCallbackStruct *cb =  (XmListCallbackStruct *)call_data;
  int   i = 0;
  unsigned char k;

  DumpListCBStruct(call_data);
  
.mc |
  if (XmStringCompare(cb->item,ListItems[0])) /* Set a new item list */
.mc
  {
     XtSetArg(Args[i], XmNitems, (XtArgVal) NewListItems); i++;
.mc |
     XtSetArg(Args[i], XmNitemCount, (XtArgVal) NUM_NEW_LIST_ITEMS);
     i++;   
.mc
  }
  
.mc |
  if (XmStringCompare(cb->item,NewListItems[0]))
  {                                    /* Set the original Item List */
.mc
     XtSetArg(Args[i], XmNitems, (XtArgVal) ListItems); i++;
     XtSetArg(Args[i], XmNitemCount, (XtArgVal) NUM_LIST_ITEMS); i++;   
  }

.mc |
  if (XmStringCompare(cb->item,ListItems[1]))
  {                                  /* Set a new Selected Item List */
.mc
   XtSetArg(Args[i], XmNselectedItems, (XtArgVal) SelectedItems); i++;
.mc |
   XtSetArg(Args[i], XmNselectedItemCount,
                                  (XtArgVal) NUM_SELECTED_ITEMS); i++;
.mc
  }

.mc |
  if (XmStringCompare(cb->item,ListItems[2]))
  {                             /* Make all items visible by */
                                /* getting the current item  */
   XtSetArg(Args[0], XmNitemCount, &j); /* count and making that the */
   XtGetValues(w, Args, 1);             /* visible item count. */
.mc
   XtSetArg(Args[i], XmNvisibleItemCount, (XtArgVal) j); i++;
  }

.mc |
  if (XmStringCompare(cb->item,ListItems[3]))
  {                                       /* Make five items visible */
.mc
   XtSetArg(Args[i], XmNvisibleItemCount, (XtArgVal) 5); i++;
  }

.mc |
  if (XmStringCompare(cb->item,ListItems[4]))
  {                                    /* Set a new selection policy */
.mc
   XtSetArg(Args[0], XmNselectionPolicy, &k);
   XtGetValues(w, Args, 1);
   if (k == XmSINGLE_SELECT)
    k = XmMULTIPLE_SELECT;
    else
    if (k == XmMULTIPLE_SELECT)
       k = XmBROWSE_SELECT;
    else
      if (k == XmBROWSE_SELECT)
       k = XmEXTENDED_SELECT;
      else
      if (k == XmEXTENDED_SELECT)
         k = XmSINGLE_SELECT;
   XtSetArg(Args[i], XmNselectionPolicy, (XtArgVal)k ); i++;
  }
  
.mc |
  if (XmStringCompare(cb->item,ListItems[5]))
  {                           /* Increase the spacing between items */
.mc
   Spacing += 2;
   XtSetArg(Args[i], XmNlistSpacing, (XtArgVal)Spacing ); i++;
  }
  
.mc |
  if (XmStringCompare(cb->item,ListItems[6]))
  {                   /* Change the font the items are displayed in */
.mc
   if (curfont == font1) 
      curfont = font2;
   else 
      if (curfont == font2) 
        curfont = font3;
     else 
        curfont = font1;
   XtSetArg(Args[i], XmNfontList, (XtArgVal) curfont ); i++;
  }

.mc |
  if (XmStringCompare(cb->item,ListItems[21]))
  {                                   /* Set automatic selection ON */
.mc
   XtSetArg(Args[i], XmNautomaticSelection, (XtArgVal) TRUE); i++;
  }

.mc |
  if (XmStringCompare(cb->item,ListItems[22]))
  {                                  /* Set automatic selection OFF */
.mc
   XtSetArg(Args[i], XmNautomaticSelection, (XtArgVal) FALSE); i++;
  }
/****************
 *
 * If we have set any arguments, do the SetValues and return.
 *
 ****************/
  if (i > 0)  
  {                
    XtSetValues(w,Args,i);
    return;
  }
  
.mc |
  if (XmStringCompare(cb->item,ListItems[7]))
  {                                /* Add an item at the first */
   XmListAddItem(w,FirstItem,1);   /* position in the list   */
.mc
  }

.mc |
  if (XmStringCompare(cb->item,ListItems[8]))
  {                                /* Add an item at the last  */
   XmListAddItem(w,LastItem,0);    /* position in the list   */
.mc
  }

.mc |
  if (XmStringCompare(cb->item,ListItems[9]))
  {                                /* Add an item at the fifth */
   XmListAddItem(w,MiddleItem,5);  /* position in the list   */
.mc
  }

.mc |
  if (XmStringCompare(cb->item,ListItems[10]))
  {                                 /* Delete the 'Middle Item' */
   XmListDeleteItem(w, MiddleItem); /* list element       */
.mc
  }

.mc |
  if (XmStringCompare(cb->item,ListItems[11]))
  {                                 /* Delete the last item   */
.mc
   XmListDeletePos(w,0);
  }

.mc |
  if (XmStringCompare(cb->item,ListItems[12]))
  {                                 /* Select the first item */
.mc
    XmListSelectPos(w,1,TRUE);
  }

.mc |
  if (XmStringCompare(cb->item,ListItems[13]))
  {                                 /* Select the last item */
.mc
    XmListSelectPos(w,0,TRUE);
  }

.mc |
  if (XmStringCompare(cb->item,ListItems[14]))
  {                                 /* Deselect the first item */
.mc
    XmListDeselectPos(w,1);
  }

.mc |
  if (XmStringCompare(cb->item,ListItems[15]))
  {                                 /* Deselect the last item */
.mc
    XmListDeselectPos(w,0);
  }

.mc |
  if (XmStringCompare(cb->item,ListItems[16]))
  {                                 /* Deselect all selected items */
.mc
    XmListDeselectAllItems(w);
  }
  
.mc |
  if (XmStringCompare(cb->item,ListItems[17]))
  {                                 /* Make the fifth item the top */
.mc
    XmListSetPos(w,5);
  }

.mc |
  if (XmStringCompare(cb->item,ListItems[18]))
  {                                 /* Make the first item the top */
.mc
    XmListSetPos(w,1);
  }

.mc |
  if (XmStringCompare(cb->item,ListItems[19]))
  {                                 /* Make the fifth item bottom */
.mc
    XmListSetBottomPos(w,5);
  }

.mc |
  if (XmStringCompare(cb->item,ListItems[20]))
  {                                 /* Make the last item the bottom */
.mc
    XmListSetBottomPos(w,0);
  }

.mc |
  if (XmStringCompare(cb->item,ListItems[23]))
  {                                 /* End the program. */
.mc
    exit(0);
  }

}

.fi
.mc |
.S
.in
.mc
.ft P
.P
.iX "List" "callback structure"
.iX "Callback structure, list"
This callback is the heart of the program.  Basically, it compares the
.mc |
current selected item in the callback structure (see the List manpage
.mc
for a description of this structure) to the the original list of items.
.mc |
When a match is found, the necessary action is taken.  
.P
For example, if
Five Visible is the item selected from the List, 
the resource XmNvisibleItemCount is reset
.mc
to 5 and XtSetValues is called.  Note the use of the functions presented
in the table at the beginning of this chapter.
.iX "XmListAddItem"
.mc |
Continuing the example, if Add Item
at Top is the item selected from the List,
the function \fBXmListAddItem\fP is called with the
position value set to 1.  If Add Item at End is selected,
.mc
\fBXmListAddItem\fP is called with the position value set to 0.  You can
get an idea of how other list functions are used by examining the rest
of the callback code segment.
.H 4 "Selection Policies"
.iX "Selection policies, List"
.iX "List" "selection policies"
.P
The List widget has four selection policies that can be set
.mc |
programmatically.  The policies are defined as follows:
.mc
.BL
.LI
.mc |
Single Selection.  This policy allows you to move the pointer to the List
item you want and when you click \fBBSelect\fP, that item is highlighted.
.mc
No highlighting or selection occurs while you are moving the pointer, and
only one item can be selected at a time.
.mc |
.sp
With the keyboard, you can use the navigation keys to move the location
cursor to a List item and then press \fBKSelect\fP to select that item.
This deselects any previous selection.
.mc
.LI
Multiple Selection.  This policy allows you to select more than one item
.mc |
on the List.  Selection and highlighting occur only after you click mouse
button 1, but you can select as many items as you want.  You can deselect
any selected item by clicking \fBBSelect\fP when the pointer is on that item.
.sp
With the keyboard, you can use the navigation keys to move the location
cursor to a List item.
If that item is not selected, pressing \fBKSelect\fP adds that item to
the current selection.
If the item is already selected, pressing \fBKSelect\fP deselects it.
.mc
.LI
.mc |
Extended Selection.  This policy allows you to select more than one item on
the List without clicking \fBBSelect\fP for each item.  When you
press and hold \fBBSelect\fP on an item and then drag the pointer up or down
.mc
from that point, all items between the initial item and the pointer are
.mc |
highlighted.  Releasing \fBBSelect\fP stops the selection process and those
.mc
items selected remain highlighted.
.mc |
.sp
With the keyboard, the extended selection mechanism depends on whether
the List is in normal mode or add mode.
Normal mode allows selection of a single range of items.
In this mode, pressing a navigation key moves the location cursor and
an \fIanchor\fP to a List item.
It also selects that item and deselects any previous selection.
Pressing a navigation key with the \fBMShift\fP modifier moves the
location cursor to a List item and extends the current selection to
include all items between the anchor and the location cursor.
.sp
Add mode allows selection of discontinuous ranges of items.
In this mode, pressing a navigation key moves the location cursor, but
not the anchor or the selection, to a new List item.
Pressing \fBKSelect\fP moves the anchor to the item at the location
cursor.
If this item was previously selected, it is unselected; if this item was
previously unselected, it is selected.
Pressing \fBKExtend\fP adds all items between the anchor and the
location cursor to the current selection.
.mc
.LI
.mc |
Browse selection.  When you press and hold \fBBSelect\fP on an item, that
.mc
item is selected and highlighted.  Dragging the pointer up or down from
that point causes each succeeding item to be selected and highlighted while
the preceding item is unselected and unhighlighted.  When you release the
mouse button, the item on which the pointer rests is selected and highlighted.
.mc |
.sp
With the keyboard, pressing a navigation key moves the location cursor
to a List item, selects that item, and deselects any previous selection.
.mc
.LE
.P
.mc |
The policy selections can be changed in the sample program by double-clicking
on the New Policy item.
.P
Figures 7-2 and 7-3 show single selection,
Figure 7-4 shows multiple selection, and Figure 7-5 shows extended selection.

.mc
.ig ++
.br
.ie \nZ \{\
.br
.ne 24.5P
.C
.sC graphics/list3.tif tiff 300 10P 22.5P
\&
.P
.sp +1
.FG "List Widget Multiple Selection"
.sp +1.5
.\}
.el \{\
.br
.ne 24.5P
\&
.sp 22.5P
.ce
illustration list3.eps goes here
.sp +1
.FG "List Widget Multiple Selection"
.sp +1.5
.\}
.++
.br
.mc |
.ne 4.25i
.mc
\0
.mc *
.mc
.FG "List Widget Multiple Selection"
.mc |
.sp 4i
.in +1.94i
.P! graphics/list3.ps 
.in -1.94i
.mc
.ig ++
.br
.ie \nZ \{\
.br
.ne 24.5P
.C
.sC graphics/list4.tif tiff 300 10P 22.5P
\&
.P
.sp +1
.FG "List Widget Extended Selection"
.sp +1.5
.\}
.el \{\
.br
.ne 24.5P
\&
.sp 22.5P
.ce
illustration list4.eps goes here
.sp +1
.FG "List Widget Extended Selection"
.sp +1.5
.\}
.++
.br
.mc |
.ne 4i
.mc
\0
.mc *
.mc
.FG "List Widget Extended Selection"
.mc |
.sp 3.75i
.in +1.94i
.P! graphics/list4.ps 
.in -1.94i
.mc
.P
The code segments shown in the Callback section provide an example of how
selection policy can be changed programmatically.  The appropriate
.mc |
segment from that section is as follows:
.mc
.P
.ft CW
.nf
.mc |
.in0
.S -2
  if (XmStringCompare(cb->item,ListItems[4])) /*New Selection policy*/
.mc
  {
  XtSetArg(myArgs[0], XmNselectionPolicy, &k);
  XtGetValues(w, Args, 1);
  if (k == XmSINGLE_SELECT)
   k = XmMULTIPLE_SELECT;
   else
    if (k == XmMULTIPLE_SELECT)
     k = XmBROWSE_SELECT;
    else
     if (k == XmBROWSE_SELECT)
      k = XmEXTENDED_SELECT;
     else
    if (k == XmEXTENDED_SELECT)
       k = XmSINGLE_SELECT;
 /* Set the new values into the widget */
  XtSetArg(Args[i], XmNselectionPolicy, (XtArgVal)k); 
.ft P
.mc |
.S
.in
.mc
.fi
.P
.mc |
The variable \fIk\fP is defined as an unsigned char to match the type of the
resource \fBXmNselectionPolicy\fP.  The argument array is set up with a 
.mc
call to \fBXtSetArg\fP, and the current selection policy is obtained
by the call to \fBXtGetValues\fP.  The selection policy is then 
changed and the new selection policy is reset into the
widget with a call to \fBXtSetArg\fP.
.H 2 "RowColumn Widget"
.iX "RowColumn"
.iX "Widget" "RowColumn"
.mc |
The RowColumn widget is a general-purpose RowColumn manager capable
.mc
of containing any widget type as a child.  It requires no special
knowledge about how its children function, and provides nothing above
and beyond support for several different layout styles.  
.P
.mc |
The RowColumn widget has no 3-dimensional visuals associated with it. If an application
wishes to have a 3-dimensional shadow placed around the RowColumn widget, it
.mc
should create the RowColumn as a child of a Frame widget.
.H 3 "RowColumn Types"
.mc |
.iX "RowColumn" "types"
.mc
Motif provides several types of RowColumn widgets. The widget type is
.mc |
specified using the \fBXmNrowColumnType\fP resource. The possible settings for
.mc
this resource are as follows:
.BL
.LI
XmWORK_AREA. The XmWORK_AREA type provides the generalized RowColumn
manager. It is the default type when the widget is created using the
.mc |
\fBXmCreateRowColumn\fP function or the \fBXtCreateWidget\fP X Toolkit
.mc
function. 
.LI
.mc |
Any of the following four settings, used to create Motif menus:
.DL
.mc
.LI
XmMENU_POPUP
.LI
XmMENU_BAR
.LI
XmMENU_PULLDOWN
.LI
XmMENU_OPTION
.LE
.mc |
.br
.ne 5
.mc
.P
The specific create functions for Popup MenuPanes, MenuBars, Pulldown
MenuPanes, and Option menus create RowColumn widgets set to these types.
.LE
.P
.mc |
The various types of menus are covered in Chapter 6.  The rest of this
.mc
section deals only with the XmWORK_AREA type.
.H 3 "RowColumn Functions"
.iX "RowColumn" "functions"
.iX "Functions" "RowColumn"
The following functions create RowColumn widgets of default type XmWORK_AREA:
.P
.mc |
.ta .5i 1i 1.5i
.nf
.mc
.iX "XtCreateWidget"
.mc |
\f(CWWidget XtCreateWidget (name, 
	xmRowColumnWidgetClass, parent,
	arglist, argcount)
.ta .5i 3i
.fi
.mc
.P
.mc |
.nf
.ta .5i 1i 1.5i
.mc
.iX "XmCreateRowColumn"
.mc |
Widget XmCreateRowColumn (parent, 
	name, arglist, argcount)\fP
.fi
.ta .5i 3i
.mc
.P
Both create functions create an instance of a RowColumn widget and
return the associated widget ID.
.mc |
\fBXtCreateWidget\fP is the standard X Toolkit Create function.
\fBXmCreateRowColumn\fP is the RowColumn specific Create function.
.mc
.H 3 "Layout"
.iX "RowColumn" "layout"
RowColumn provides a variety of resources that determine the type of layout
performed. For example, resources control these attributes:
.BL
.LI
.iX "RowColumn" "sizing"
Sizing. The size of the widget can be set explicitly (by resource
.mc |
settings), or automatically according to requirements
.mc
of the children and their specified layout.
.LI
.iX "RowColumn" "orientation"
.mc |
Orientation. RowColumn can be configured to lay out its children in
columns (vertical) or rows (horizontal).
.br
.ne 8
.mc
.LI
.iX "RowColumn" "packing"
Packing. The children can be packed together tightly (not in an organized
.mc |
grid of rows and columns); or all children can be placed in identically
.mc
sized boxes, thus producing a symmetrical-looking arrangement of the
children.  Another alternative allows the
.mc |
application to specify the exact X and Y positions of the children.
.mc
.LI
.iX "RowColumn" "spacing"
.mc |
Spacing Between Children. The application can control the spacing
.mc
between the rows and columns.
.LI
.mc |
Margin Spacing. Resources set the spacing 
.mc
between the edges of the RowColumn widget and the children
placed along the edge.
.LE
.H 4 "Sizing"
.mc |
When \fBXmNresizeHeight\fP and/or \fBXmNresizeWidth\fP are set to True, RowColumn will
.mc
request new dimensions from its parent, if necessary. The resources should
be set to False if the application wants to control the dimensions.
.P
.mc |
When the \fBXmNpacking\fP resource is set to XmPACK_NONE, the RowColumn widget
.mc
expands, if necessary, to enclose its children.
.H 4 "Orientation"
.iX "RowColumn" "orientation"
.mc |
The orientation is set using the \fBXmNorientation\fP resource. There are two
.mc
possible settings:
.BL
.LI
.mc |
XmVERTICAL (default). This specifies a column-major orientation. In a
column-major orientation, children are laid out in columns from top to bottom.
.mc
.LI
.mc |
XmHORIZONTAL. This specifies a row-major orientation. In a row-major
.mc
orientation, children are laid out in rows, from left to right.
.LE
.mc |
.P
The two figures that follow show column-major and row-major orientations.
.sp
.ne 15
.FG "Column-Major Orientation (XmVERTICAL)"
.if n .ig +n
.sp -2
.mc
.DS CB
.ps 8
.PS
.mc |
.in +1i
.mc
boxwid = 1.00
boxht = .3
down
Box0: box invis
move down .1; move left .75; move down .1
box "Child #1"
arrow down .5
box "Child #2"
arrow down .5
Box3: box "Child #3"
move to Box0.s; move down .1; move right .75; move down .1
Box4: box "Child #4"
arrow down .5
box "Child #5"
arrow down .5
box "Child #6"
arrow from Box3.e to Box4.w
.mc |
.in -1i
.mc
.PE
.ps
.DE
.mc |
.+n
.sp 2
.FG "Row-Major Orientation (XmHORIZONTAL)"
.if n .ig +n
.sp -2
.mc
.DS CB
.ps 8
.PS
.mc |
.in +1i
.mc
boxwid = 1.00
boxht = .3
down
Box0: box invis
move down .1; move left .75; move down .1
Box7: box "Child #1"
move down .5
Box9: box "Child #3"
move down .5
Box11: box "Child #5"
move to Box0.s; move down .1; move right .75; move down .1
Box8: box "Child #2"
move down .5
Box10: box "Child #4"
move down .5
Box12: box "Child #6"
arrow from Box7.e to Box8.w
arrow from Box8.s to Box9.n
arrow from Box9.e to Box10.w
arrow from Box10.s to Box11.n
arrow from Box11.e to Box12.w
.mc |
.in -1i
.mc
.PE
.DE
.mc |
.+n
.br
.ne 3i
.mc
.H 4 "Packing"
.iX "RowColumn" "packing"
.mc |
The \fBXmNpacking\fP resource determines how the items in a RowColumn widget are
.mc
packed together. There are three possible settings:
.BL
.LI
XmPACK_TIGHT (default). The layout depends on the orientation:
.mc |
.DL
.mc
.LI
For Xm_VERTICAL orientation, items are placed one after another in a given
column until there is no room left for another item.  Wrapping then occurs
to the next column and continues until all the children
have been placed.  The boxes in a given column are set to the same width,
based on the widest box in that column.
Thus, the items are stacked vertically but may
.mc |
be staggered horizontally.  The following figure shows an \fBXmRowColumn\fP widget.
.LE
.LE
.mc
.ig ++
.br
.ie \nZ \{\
.br
.ne 8P
.C
.sC graphics/ptightv.tif tiff 300 18P 6P
\&
.P
.sp +1
.FG "XmRowColumn Widget"
.\}
.el \{\
.br
.ne 8P
\&
.sp 6P
.ce
illustration ptightv.eps goes here
.sp +1
.FG "XmRowColumn Widget"
.sp +1.5
.\}
.++
.br
.mc |
.ne 1.5i
.mc
\0
.mc *
.mc
.FG "XmRowColumn Widget"
.mc |
.sp 1.25i
.in +1.31i
.P! graphics/ptightv.ps 
.in -1.31i
.BL
.DL
.mc
.LI
For Xm_HORIZONTAL orientation, items
are placed one after
another in a given row
until there in no room left for another
item. Wrapping then occurs to the next row and continues until all the
children have been placed.  Boxes in a given row are set to the same
height, based on the highest box in that row.
Thus, the items are layered horizontally but may be staggered vertically. 
.mc |
The following figure shows a RowColumn widget with the XmPACK_TIGHT resource and
XmHORIZONTAL orientation.
.mc
.LE
.mc |
.LE
.mc
.ig ++
.br
.ie \nZ \{\
.br
.ne 8P
.C
.sC graphics/ptighth.tif tiff 300 18P 6P
\&
.P
.sp +1
.mc |
.FG "XmPACK_TIGHT with XmHORIZONTAL Orientation"
.mc
.\}
.el \{\
.br
.ne 8P
\&
.sp 6P
.ce
illustration ptighth.eps goes here
.sp +1
.mc |
.FG "XmPACK_TIGHT with XmHORIZONTAL Orientation"
.mc
.sp +1.5
.\}
.++
.br
.mc |
.ne 1.5i
.mc
\0
.mc |
.FG "XmPACK_TIGHT with XmHORIZONTAL Orientation"
.sp 1.25i
.in +1.31i
.mc
.P! graphics/ptighth.ps 
.mc |
.in -1.31i
.BL
.mc
.LI
XmPACK_COLUMN. In PACK_COLUMN packing, children are placed in identically
sized boxes so that the layout becomes a grid. The height of the boxes is
the height of the highest child; similarly, the width of the boxes is the
width of the widest child.
.P
.mc |
The \fBXmNnumColumns\fP resource
.mc
specifies how many columns (for XmVERTICAL orientation) or rows (for
XmHORIZONTAL orientation) are built. There is no automatic wrapping when a
column or row is too long to fit. 
.mc |
.P
The following figures show packed, 3-column RowColumn widgets with vertical
and horizontal orientations.
.mc
.ig ++
.br
.ie \nZ \{\
.br
.ne 11P
.P
.sC graphics/pcolv.tif tiff 300 18P 9P
\&
.P
.sp +1
.FG "XmPACK_COLUMN With XmVERTICAL Orientation and XmNnumColumns = 3"
.sp +1.5
.\}
.el \{\
.br
.ne 11P
\&
.sp 9P
.ce
illustration prompt.eps goes here -- no such illustration!
.sp +1
.FG "       "
.sp +1.5
.\}
.++
.br
.mc |
.ne 2i
.mc
\0
.mc *
.mc
.FG "XmPACK_COLUMN With XmVERTICAL Orientation and XmNnumColumns = 3"
.mc |
.sp 1.75i
.in +1.31i
.P! graphics/pcolv.ps 
.in -1.31i
.mc
.ig ++
.br
.ie \nZ \{\
.br
.ne 11P
.P
.sC graphics/pcolh.tif tiff 300 18P 9P
\&
.P
.sp +1
.FG "XmPACK_COLUMN With XmHORIZONTAL Orientation and XmNnumColumns = 3"
.sp +1.5
.\}
.el \{\
.br
.ne 11P
\&
.sp 9P
.ce
illustration pcolh.eps goes here
.sp +1
.FG "       "
.sp +1.5
.\}
.++
.br
.mc |
.ne 2i
.mc
\0
.mc *
.mc
.FG "XmPACK_COLUMN With XmHORIZONTAL Orientation and XmNnumColumns = 3"
.mc |
.sp 1.75i
.in +1.31i
.P! graphics/pcolh.ps 
.in -1.31i
.mc
.LI
XmPACK_NONE. When there is no packing, children are positioned according to
.mc |
the X and Y positions specified by their resources. If necessary, the
.mc
RowColumn widget will attempt to become large enough to enclose all its
children. 
.LE
.H 4 "Spacing Between Children"
.iX "RowColumn" "spacing"
.mc |
The \fBXmNspacing\fP resource specifies the horizontal and vertical spacing, in
.mc
pixels, between items within the RowColumn widget. The default is one pixel.
.H 4 "Margin Spacings"
.iX "RowColumn" "margin spacing"
.mc |
The \fBXmNmarginHeight\fP and \fBXmNmarginWidth\fP resources specify the size of the
.mc
margins between the edge of the RowColumn widget and the children along the
.mc |
edge.  The following figure shows a RowColumn widget using margins.
.mc
.ig ++
.br
.ie \nZ \{\
.br
.ne 15P
.C
.sC graphics/margin.tif tiff 300 29P 13P
\&
.P
.sp +1
.FG "XmNmarginHeight and XmNmarginWidth"
.sp +1.5
.\}
.el \{\
.br
.ne 15P
\&
.sp 13P
.ce
illustration margin.eps goes here
.sp +1
.FG "XmNmarginHeight and XmNmarginWidth"
.sp +1.5
.\}
.++
.br
.ne 3i
\0
.mc |
.FG "XmNmarginHeight and XmNmarginWidth"
.mc
.sp 2.75i
.mc |
.in +.43i
.mc
.P! graphics/margin.ps 
.mc |
.in -.43i
.mc
.H 2 "Text Widget"
.iX "Text" "widget"
.iX "Widget" "Text"
.mc |
The Text widget  can be used as a single-line or a  multiline text editor.
.mc
You can interact with the Text widget programmatically or by user action.
You can use the Text widget for single-line entry, form entry, or
full-window editing.
.H 3 "Text Functions"
.iX "Text" "functions"
.iX "Functions" "Text widget"
.mc |
The following table lists the functions associated with the Text widget.
Each function has its own manpage in the \fIOSF/Motif Programmer's
Reference\fP.
.br
.ne 10i
.mc
.iX "XtCreateWidget"
.iX "XmCreateText"
.iX "XmCreateScrolledText"
.iX "XmTextClearSelection"
.mc |
.iX "XmTextCopy"
.iX "XmTextCut"
.iX "XmTextGetBaseline"
.iX "XmTextGetEditable"
.iX "XmTextGetInsertionPosition"
.iX "XmTextGetLastPosition"
.iX "XmTextGetMaxLength"
.mc
.iX "XmTextGetSelection"
.mc |
.iX "XmTextGetSelectionPosition"
.iX "XmTextGetSource"
.mc
.iX "XmTextGetString"
.mc |
.iX "XmTextGetTopCharacter"
.iX "XmTextInsert"
.iX "XmTextPaste"
.iX "XmTextPosToXY"
.iX "XmTextRemove"
.mc
.iX "XmTextReplace"
.mc |
.iX "XmTextScroll"
.iX "XmTextSetAddMode"
.iX "XmTextSetEditable"
.iX "XmTextSetHighlight"
.iX "XmTextSetInsertionPosition"
.iX "XmTextSetMaxLength"
.iX "XmTextSetSelection"
.iX "XmTextSetSource"
.iX "XmTextSetString"
.iX "XmTextSetTopCharacter"
.iX "XmTextShowPosition"
.iX "XmTextXYToPos"
.mc
.TB "Text Widget Functions"
.TS
center, box, tab(;);
lb | lb
lB | lf .
.mc
.sp .2
Function;Description
.sp .2
_
.sp .2
.mc |
XtCreateWidget;Basic widget creation function 
XmCreateText;Specific creation function for XmText
XmCreateScrolledText;Creation function for ScrolledText
XmTextClearSelection;Clears the primary selection
XmTextCopy;Copies primary selection to the clipboard
XmTextCut;T{
.na
Copies primary selection to the
clipboard and deletes selected text
.ad
T}
XmTextGetBaseline;Finds the X position of the first baseline
XmTextGetEditable;Finds the value of the edit permission state
XmTextGetInsertionPosition;Finds the insert cursor position
XmTextGetLastPosition;Finds the position of the last character
XmTextGetMaxLength;Finds the maximum allowable length of the text string
XmTextGetSelection;Retrieves the value of the primary selection
XmTextGetSelectionPosition;Finds the position of the primary selection
XmTextGetSource;Accesses the source of the Text widget
XmTextGetString;Accesses the string value of the Text widget
XmTextGetTopCharacter;Finds the position of the first character
XmTextInsert;Inserts a character string into a text string
XmTextPaste;Inserts the clipboard selection
XmTextPosToXY;Finds the X and Y position of a character position
XmTextRemove;Deletes the primary selection
XmTextReplace;Replaces part of a string
XmTextScroll;Scrolls text in the Text widget
XmTextSetAddMode;Turns Add Mode on or off in the widget
XmTextSetEditable;Sets the value of the edit permission state
XmTextSetHighlight;Highlights text in the Text widget
XmTextSetInsertionPosition;Sets the insert cursor position
XmTextSetMaxLength;Sets the maximum allowable length of the text string
XmTextSetSelection;Sets the primary selection of text in the widget
XmTextSetSource;Sets the source of the Text widget
XmTextSetString;Sets the string value of the Text widget
XmTextSetTopCharacter;Sets the position of the first character
XmTextShowPosition;Forces text at a given position to be displayed
XmTextXYToPos;T{
.na
Finds the character position nearest
an X and Y position
.ad
T}
.mc
.sp .2
.TE
.mc |
.in0
.mc
.P
The use of some of these functions will be described in the sections that
follow.  Actual code segments accompanied by illustrations show the results
of programming actions.
.mc |
.H 3 "Using the Text Widget in a Program"
.mc
.iX "Using Text widget"
.mc |
.iX "Widget" "using Text"
The figure that follows shows an example of a Text widget.  You have seen other
.mc
examples in earlier chapters of this manual.  The FileSelectionBox
.mc |
Selection window is a Text widget, as is the same window in the 
.mc
SelectionBox widget.  
.ig ++
.br
.ie \nZ \{\
.br
.ne 15.5P
.P
.sC graphics/text1.tif tiff 300 18.5P 13.5P
\&
.P
.FG "Text Widget"
.P
.\}
.el \{\
.br
.ne 15.5P
\&
.sp 13.5P
.ce
illustration text1.eps goes here
.sp +1
.FG "Text Widget"
.sp +1.5
.\}
.++
.br
.mc |
.ne 2.75i
.mc
\0
.mc *
.mc
.FG "Text Widget"
.mc |
.sp 2.25i
.in +1.31i
.P! graphics/text1.ps 
.in -1.31i
.mc
.P
.mc |
The program that generated the window shown in the preceding Figure is called
\fBxmeditor\fP.  The source code is located on your system as
\fB./demos/xmsamplers/xmeditor.c\fP.
.mc
This program demonstrates how you can use the Text widget in concert with 
other widgets.  You can create a new file, edit an old file, cut and paste
text, and so on.
.H 4 "The File Menu"
.mc |
If you move the pointer so that it covers the File button in the 
MenuBar and then click mouse button 1, a menu appears like the following: 
.mc
.ig ++
.br
.ie \nZ \{\
.br
.ne 15.5P
.P
.sC graphics/text2.tif tiff 300 18.5P 13.5P
\&
.P
.sp +1
.FG "Text Demonstration File Menu"
.sp +1.5
.\}
.el \{\
.br
.ne 15.5P
\&
.sp 13.5P
.ce
illustration text2.eps goes here
.sp +1
.FG "Text Demonstration File Menu"
.sp +1.5
.\}
.P
.++
.br
.mc |
.ne 2.5i
.mc
\0
.mc *
.mc
.FG "Text Demonstration File Menu"
.mc |
.sp 2.25i
.in +1.31i
.P! graphics/text2.ps 
.in -1.31i
.mc
.P
.mc |
If you select Open, a FileSelectionBox will appear.  You can select
a file from this as described in ***Chapter 5***.  The other menu choices
.mc
are self-explanatory.
.H 4 "The Edit Menu"
.mc |
The Edit menu allows you to use some of the cut and paste
.mc
features of Motif within the Text widget.  Move the pointer to
.mc |
Edit and click
mouse button 1 to see the Edit menu.  The figure that follows shows
the Edit menu.  
.mc
.ig ++
.br
.ie \nZ \{\
.br
.ne 15.5P
.P
.sC graphics/text3.tif tiff 300 18.5P 13.5P
\&
.P
.sp +1
.FG "Text Demonstration Edit Menu"
.sp +1.5
.\}
.el \{\
.br
.ne 15.5P
\&
.sp 13.5P
.ce
illustration text3.eps goes here
.sp +1
.FG "Text Demonstration Edit Menu"
.sp +1.5
.\}
.++
.br
.mc |
.ne 2.5i
.mc
\0
.mc *
.mc
.FG "Text Demonstration Edit Menu"
.mc |
.sp 2.25i
.in +1.31i
.P! graphics/text3.ps 
.in -1.31i
.mc
.P
.H 4 "Accelerators and Mnemonics"
.iX "Mnemonic"
.mc |
Both the Edit and File menus contain \fImnemonics\fP,
.mc
indicated by underlined characters in the menus.  You can use them instead
of the mouse button to make the selection you want.  For example, to choose
.mc |
File using its mnemonic, move the pointer into the \fBxmeditor\fP 
window and press <\fBMeta\fP> <\fBf\fP>.  This will cause the
.mc
Pulldown menu to appear.  You can traverse the Pulldown menu by using the
.mc |
\fB<\(ua>\fP and \fB<\(da>\fP keys, or use the mouse to move the pointer.  You can
select from the Pulldown menu by using its mnemonics as well.  
Just press the key that corresponds to the underlined letter in the 
.ne 7
menu.  Don't worry about uppercase or lowercase as these are not
case-sensitive.  For example, if you want to open a file,
press <\fBo\fP>.  You need to use the <\fBMeta\fP>
key only when accessing a Pulldown menu from the MenuBar.
.mc
.P
.iX "Accelerator"
.mc |
The Edit menu also has accelerators in addition to the mnemonics.
.mc
Accelerators are keys or combinations of keys that perform a specific
.mc |
function within an application.  For example,  Cut has a mnemonic of
``t'' and an accelerator of <\fBShift\fP><\fBDel\fP>.  Pressing
either the <\fBt\fP> key or the <\fBShift\fP><\fBDel\fP> keys will
execute the Cut function of the menu.  Note that accelerators are not
.mc
part of the menu and will execute regardless of whether or not the menu is
visible.
.H 4 "Primary Selection"
.\".H 4 "Primary and Secondary Selections"
.iX "Primary selection"
.iX "Selection" "primary"
.iX "Text" "primary selection"
.\".iX "Secondary selection"
.\".iX "Selection" "secondary"
.\".iX "Text" "secondary selection"
.\"The Text widget has two types of selections, primary and secondary.  The
The Text widget has primary selection.
Primary selection is identified by highlighting the text in inverse video.
.\"and the secondary selection is identified by underlining the text. 
You
can cut out text that is the primary selection and paste it in at some other
point in the Text window.
.\"You can either copy a secondary selection or copy and delete it.
Selection is described in
more detail in later sections of this chapter.
.H 4 "Cutting Text"
.iX "Cutting text"
.mc |
To remove (or cut) text, move the insert cursor to the starting point
.mc
of the text you wish to remove.  You can then perform any of the following
actions to select the text (primary selection) to be removed:
.P
.mc |
\fBNOTE:\fP
.mc
To position the insert cursor, move the pointer into the text window and to
.mc |
the desired point on a text line, then click \fBBSelect\fP.
.mc
.BL
.LI
If you want just a single word removed, position the insert cursor before
.mc |
the first letter of the word and then double-click \fBBSelect\fP.  The
.mc
word is highlighted.
.LI
.mc |
If you want the entire line of text to be removed, triple-click mouse
.mc
button 1.  The entire line of text is highlighted.  Note that the entire
line is highlighted regardless of the initial position of the insert
cursor on the line.
.mc |
.br
.ne 8
.mc
.LI
.mc |
If you want all of the text to be removed, quadruple-click mouse
.mc
button 1.  All of the text is highlighted.  Note that the all of
the text is highlighted regardless of the initial position of the insert
cursor on the line.
.LI
If you want more than one line (or more than one word) of text to be
removed, you can drag the insert cursor to select the desired text.  Begin
by moving the pointer to the starting point in the text window and pressing
.mc |
and holding down \fBBSelect\fP.  This positions the insert cursor at that
.mc
point.  Now drag the insert cursor along the line (or lines) of text
that you want to remove.  When you reach the end of the text that
you want removed, release
.mc |
\fBBSelect\fP and that text is highlighted.  (See Figure 7-16.)
.br
.ne 6
.mc
.LI
If you want to add text to the current primary selection, move the pointer
to the position in the text that corresponds to the last character you want
.mc |
to add, and press \fBBExtend\fP.  You can add text in front
.mc
of or after the current primary selection.
.LE
.mc |
.P
You can also use the keyboard to select text.
The selection mechanism depends on whether the text widget is in normal
mode or add mode.
.P
In normal mode, pressing a navigation key moves the location cursor to a
new text element.
This also sets an \fIanchor\fP at the new element and deselects any
previous selection.
Pressing a navigation key along with the \fBMShift\fP modifier moves the
location cursor to a new text element and extends the current selection
to include all items between the anchor and the location cursor.
Pressing \fBKSelect\fP moves the anchor to the location cursor and
deselects any previous selection.
Pressing \fBKExtend\fP extends the current selection to include all
items between the anchor and the location cursor.
.P
In add mode selection works the same way as in normal mode, except that
pressing a navigation key does not change the anchor or the current
selection.
.mc
.ig ++
.P
.br
.ie \nZ \{\
.br
.ne 15.5P
.P
.sC graphics/text4.tif tiff 300 18.5P 13.5P
\&
.P
.sp +1
.FG "Selecting Text for Removal"
.sp +1.5
.\}
.el \{\
.br
.ne 15.5P
\&
.sp 13.5P
.ce
illustration text4.eps goes here
.sp +1
.FG "Selecting Text for Removal"
.sp +1.5
.\}
.++
.br
.mc |
.ne 2.5i
.mc
\0
.mc *
.mc
.FG "Selecting Text for Removal"
.mc |
.sp 2.25i
.in +1.31i
.P! graphics/text4.ps 
.in -1.31i
.mc
.P
Now that you have selected the text to be removed, move the pointer to the
.mc |
Edit button in the MenuBar and press mouse button 1.  The
menu shown in Figure 7-15 appears.  Move the pointer to Cut and
.mc
release mouse button 1.  The text you selected disappears.
.P
The source code to accomplish this is taken from \fBxmeditor.c\fP.  First, in
.mc |
the \fBMenuCB\fP procedure, which is the callback from PushButtons in the
Pulldown menu, the case called MENU_CUT is executed.
.br
.ne 6
.mc
.P
.ft CW
.nf
.mc |
.S -2
.in -.5i
.mc
case MENU_CUT:
   {
  /* needed to get the event time */
  XmAnyCallbackStruct * cb =  (XmAnyCallbackStruct *) call_data;

  /* call routine to copy selection to clipboard */
  CopyFileToClipboard(cb->event->xbutton.time);

  /* call routine to delete primary selection */
  DeletePrimarySelection();
  }
  break;

.fi
.mc |
.S
.in +.5i
.mc
.ft P
.mc |
The \fBCopyFileToClipboard\fP procedure copies the text you selected
.mc
to the clipboard for safekeeping, then that text is deleted by the
.mc |
\fBDeletePrimarySelection\fP procedure.  You can paste the text
that was cut by using the method in the \fBPasteFromClipboard\fP procedure, 
.mc
which is discussed later in this chapter.  Note that if you select
.mc |
Clear from the menu instead of Cut, the text is
.mc
not copied to the clipboard before being deleted.  This means
.mc |
that using Clear does not allow a subsequent paste of the deleted
.mc
text.
.P
The procedure \fBCopyFileToClipboard\fP uses several of the Cut and Paste
.mc |
functions described in Chapter 8.  Keep in mind that copying to the
.mc
clipboard is just a temporary measure.  You can then subsequently use
other Cut and Paste functions to retrieve the data.
.P
.ft CW
.nf
.mc |
.in0
.S -2
.mc
/*-------------------------------------------------------------
**  CopyFileToClipboard
**  Copy the present file to the clipboard.
*/
void CopyFileToClipboard(time)
Time time;
{
.mc |
   char *selected_string =
                   XmTextGetSelection (text); /* text selection  */
.mc
   unsigned long item_id = 0;      /* clipboard item id */
   int data_id = 0;        /* clipboard data id */
   int status = 0;        /* clipboard status  */
   XmString clip_label;

.mc |
   /* using the clipboard facilities, copy
    * the selected text to the clipboard */
.mc
   if (selected_string != NULL) {
  clip_label = XmStringCreateLtoR ("XM_EDITOR", charset);
  /* start copy to clipboard, and continue till
.mc |
   * a successful start copy is made */
.mc
  status = 0;
  while (status != ClipboardSuccess)
    status = XmClipboardStartCopy (XtDisplay(text), XtWindow(text),
         clip_label, time,
         text, NULL, &item_id);

   /* move the data to the clipboard, and
.mc |
    * continue till a successful copy is made */
.mc
  status = 0;
  while (status != ClipboardSuccess)
    status = XmClipboardCopy (XtDisplay(text), XtWindow(text), item_id, 
     "STRING", selected_string, (long)strlen(selected_string)+1, 0,
      &data_id);

  /* end the copy to the clipboard and continue till
.mc |
   * a successful end copy is made */
.mc
  status = 0;
  while (status != ClipboardSuccess)
    status = XmClipboardEndCopy (XtDisplay(text), XtWindow(text),
       item_id);

.mc |
  /* allow pasting when an item is successfully copied
   * to the clipboard */
.mc
  XtSetSensitive(paste_button, True);

   }
}
.mc |

.mc
.ft P
.mc |
.S
.in
.mc
.fi
.P
.iX "Primary selection"
.iX "Selection" "primary"
.mc |
The \fBXmTextGetSelection\fP function is used to set the 
\fIselected_string\fP variable to point to the primary selection in the Text
.mc
widget.  The primary selection in this case is the three words that
.mc |
are highlighted in inverse video shown in Figure 7-16.  Note that the
.mc
primary selection always appears in inverse video.
.P
.mc |
In the preceding code segment the actual copy-to-clipboard process involves
.mc
three steps:
.AL
.LI
Prepare the clipboard for copying by executing \fBXmClipboardStartCopy\fP.
.mc |
This procedure must return a success before copying can occur.  The 
\fIclip_label\fP variable is set to the string \fBXM_EDITOR\fP to identify the data
.mc
item for possible use
.mc |
in a clipboard viewer.  The variable \fIitem_id\fP specifies is set to
.mc
an arbitrary identification number assigned to this data item.  This
will be used in the calls to \fBXmClipboardCopy\fP and \fBXmClipboardEndCopy\fP.
.mc |
.br
.ne 7
.mc
.LI
Copy the data by using \fBXmClipboardCopy\fP.  The copying done
.mc |
by this function is not actually to the clipboard but to a buffer.
.mc
Copying to the clipboard occurs when the \fBXmClipboardEndCopy\fP function
is executed.  
.LI
The \fBXmClipboardEndCopy\fP function locks the clipboard to prevent
access by other applications while performing the copy, copies the
data to the clipboard, and
then unlocks the clipboard.
.LE
.H 4 "Pasting Text"
To paste the text you just removed, move the insert cursor to the position
.mc |
where you want the text to be inserted.  Move the pointer to the \fBEdit\fP
button in the MenuBar and press and hold down mouse button 1.  Move the pointer
to \fBPaste\fP and release mouse button 1.  The text you deleted is restored
(pasted) into the new position, as shown in the following figure:
.mc
.ig ++
.P
.br
.ie \nZ \{\
.br
.ne 15.5P
.P
.sC graphics/text5.tif tiff 300 18.5P 13.5P
\&
.P
.sp +1
.FG "Pasting Text"
.sp +1.5
.\}
.el \{\
.br
.ne 15.5P
\&
.sp 13.5P
.ce
illustration text5.eps goes here
.sp +1
.FG "Pasting Text"
.sp +1.5
.\}
.++
.br
.mc |
.ne 2.75i
.mc
\0
.mc *
.mc
.FG "Pasting Text"
.mc |
.sp 2.25i
.in +1.31i
.P! graphics/text5.ps 
.in -1.31i
.mc
.P
The code to accomplish this begins with that part
.mc |
of the \fBMenuCB\fP procedure dealing with a paste operation.
.mc
.P
.ft CW
.nf
   case MENU_PASTE:
   /* call the routine that pastes the
   text at the cursor position */
   PasteItemFromClipboard();
   break;
.P
.ft P
.fi
.mc |
When you select Paste from the menu, \fBPasteItemFromClipboard\fP is
.mc
called.
.mc |
.br
.ne 8
.mc
.P
.ft CW
.nf
.mc |
.in0
.S -2
.mc
/*-------------------------------------------------------------
**   PasteItemFromClipboard
**    paste item from the clipboard to the current cursor location
*/
void PasteItemFromClipboard()
{
   /* retrieve the current data from the clipboard
   and paste it at the current cursor position */

.mc |

.mc
   int status = 0;       /* clipboard status  */
   char *buffer;         /* temporary text buffer   */
   int length;         /* length of buffer     */
   int outlength = 0;      /* length of bytes copied  */
   int private_id = 0;     /* id of item on clipboard */
   XmTextPosition cursorPos;   /* text cursor position   */
   register int ac;      /* arg count         */
   Arg al[10];         /* arg list       */

.mc |

   /* find the length of the paste item,
    * continue till the length is found */
.mc
   while (status != ClipboardSuccess) {
   status = XmClipboardInquireLength(XtDisplay(text), XtWindow(text),
          "STRING", &length);
   if (status == ClipboardNoData) {
   length = 0;
   break;
   }
   }
     
.mc |

.mc
   if (length == 0) {
   fprintf(stderr, "Warning: paste failed, no items to paste.\\n");   
   return;
   }

.mc |

.mc
   /* malloc to necessary space */
   buffer = XtMalloc(length);

.mc |
   status = XmClipboardRetrieve (XtDisplay(text), XtWindow(text),
       "STRING", buffer, length, &outlength, &private_id);
.mc
     
.mc |

.mc
   /* Dialogs need to be added to indicate errors in pasting */
   if (status != ClipboardSuccess) {
   fprintf(stderr, "Warning: paste failed, status = %d\\n", status);   
   return;
   }

.mc |

.mc
   /* get cursor position for pasting */
   XtSetArg(al[0], XmNcursorPosition, &cursorPos);
   XtGetValues(text, al, 1);
     
   /* add new text */
   XmTextReplace(text, cursorPos, cursorPos, buffer);
}
.mc |
.S
.in
.mc
.ft P
.fi
.mc |
.P
The length of the data stored under the format name STRING is found
by using the \fBXmClipboardInquireLength\fP procedure.  If this
.mc
procedure is successful, then space is allocated for that length
.mc |
by \fBXtMalloc\fP, and \fBXmClipboardRetrieve\fP is called to
.mc
retrieve the text from the clipboard.  Then the current cursor position
in the Text widget is found through \fBXtGetValues\fP and the text is 
actually pasted by the function \fBXmTextReplace\fP.
.\".H 4 "Cutting and Pasting Using Secondary Selection"
.\".iX "Secondary selection" "Cut and Paste"
.\".iX "Cut and paste"
.\"The cut and paste procedure discussed above using the primary selection
.\"requires two steps.
.\"You can accomplish the same thing in one step by
.\"using the secondary selection feature of the Text widget.  Position the
.\"insert cursor at the starting point that you wish the text be copied to.
.\"Move the Pointer to the first character of the text you want to delete
.\"and copy, press and hold <\fICTRL\fP> mouse button 3, and drag the pointer
.\"until you reach the end of the text you want to delete and copy.  Release
.\"the <\fICTRL\fP> key and mouse button 3 and the text is deleted from its
.\"original position and copied to its new position.
.H 4 "Copying Text"
To copy text, you move the insert cursor to the starting point of the text
you want to copy and select the text in the same manner as for cutting
.mc |
text.  When you have the text selected, move the pointer to the Edit
button in the MenuBar and press and hold down mouse button 1.  When the menu
appears, drag the pointer to Copy and release mouse button 1.  You
.mc
won't see any change in the \fBxmeditor\fP window, but the text has been
copied to the clipboard.  In this case, the case called \fBMENU_COPY\fP is
executed.
.P
.nf
.mc |
.in0
.S -2
.mc
.ft CW
case MENU_COPY:
   {
   /* needed to get the event time */
   XmAnyCallbackStruct * cb = (XmAnyCallbackStruct *) call_data;

   /* call routine to copy selection to clipboard */
   CopyFileToClipboard(cb->event->xbutton.time);

   }
   break;
.mc |
.in
.S
.mc
.fi
.ft P
.mc |
.br
.ne 6
.P
.mc
The only difference between this case and \fBMENU_CUT\fP is that the
delete procedure \fBDeletePrimarySelection\fP is not called.  The data
is copied to the clipboard as before and can be pasted anywhere in the
text.
.\".H 4 "Copying Text Using Secondary Selection"
.\".iX "Secondary selection" "copying text"
.\".iX "Copying text"
.\"You can also copy text using the secondary selection method.  This is a 
.\"feature of the Text widget rather than the \fBxmeditor\fP program.  For
.\"example, perform the following steps to copy the words
.\"``Here is some more text"
.\"from the last line of the visible text to a new line.
.\".AL
.\".LI
.\"Move the insert cursor at the point you want the copied text to appear.  In
.\"this case, move the pointer to the start of the second line after the last
.\"visible line in the Text window and click mouse button 1.  The insert
.\"cursor should then appear.
.\".LI
.\"Move the pointer (not the insert cursor) to the first position in the last
.\"visible line of text.  Press and hold mouse button 3 
.\"and drag the pointer along the line of text.  The text is underlined as you
.\"drag the pointer.  Be careful not to move the pointer up or down
.\"as that will cause other areas of the text to be selected and underlined.  You
.\"can continue to select as long as you hold mouse button 3 down.  When you are
.\"satisfied with the text you have selected, release mouse button 3 and the copy
.\"is done automatically.
.\".LE
.\".P
.\"Figures 69 and 70 show the secondary selection and the results of the
.\"copy action.  Note that the insert cursor is positioned at the end of the
.\"newly copied text.
.\".ig ++
.\".P
.\".br
.\".ie \nZ \{\
.\".br
.\".ne 15.5P
.\".P
.\".sC graphics/text6.tif tiff 300 18.5P 13.5P
.\"\&
.\".P
.\".sp +1
.\".FG "Secondary Selection"
.\".sp +1.5
.\".\}
.\".el \{\
.\".br
.\".ne 15.5P
.\"\&
.\".sp 13.5P
.\".ce
.\"illustration text6.eps goes here
.\".sp +1
.\".FG "Pasting Text"
.\".sp +1.5
.\".\}
.\".++
.\".br
.\".ne 3i
.\"\0
.\".sp 2.75i
.\".in +1i
.\".\".P! graphics/text6.ps 
.\".\"Not currently in graphics directory; 7/19/89
.\".in -1i
.\".FG "Secondary Selection"
.\".sp
.\".ig ++
.\".br
.\".ie \nZ \{\
.\".br
.\".ne 15.5P
.\".P
.\".sC graphics/text7.tif tiff 300 18.5P 13.5P
.\"\&
.\".P
.\".sp +1
.\".FG "Secondary Selection Copy Result"
.\".sp +1.5
.\".\}
.\".el \{\
.\".br
.\".ne 15.5P
.\"\&
.\".sp 13.5P
.\".ce
.\"illustration text7.eps goes here
.\".sp +1
.\".FG "Pasting Text"
.\".sp +1.5
.\".\}
.\".++
.\".br
.\".ne 3i
.\"\0
.\".sp 2.75i
.\".in +1i
.\".\".P! graphics/text7.ps 
.\".\"Not currently in graphics directory; 7/19/89
.\".in -1i
.\".FG "Secondary Selection Copy Result"
