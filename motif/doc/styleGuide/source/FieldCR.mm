.\"  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  ALL RIGHTS RESERVED
.hR "Field Controls"
.hS "Description"
.iX "field controls"
Field controls are components that use internal navigation
controls.  Field controls should be navigation fields.
The following text describes the field controls.
.VL 17n
.LI "\fBSash\fP"
.iX "Sash"
.iX "field controls" "Sash"
Used to set the boundary between two components.  A Sash is usually
a small square on the boundary between two components.  The separated
components are called Panes, and a group of Panes, Separators,
and Sashes is called a PanedWindow.
.LI "\fBScale\fP"
.iX "Scale"
.iX "field controls" "Scale"
Used to set or display a value in a range.
A Scale is usually composed of a slider, moving within
an element that indicates the size of the range, and a label
that indicates the current value.
The position of the slider indicates the
value relative to the range.  The slider is moved 
directly by
using the mouse pointer, or by
using the arrow keys.
A Scale can also have buttons with arrow graphics
for moving the slider with the mouse.
.LI "\fBScrollBar\fP"
.iX "ScrollBar" 
.iX "field controls" "ScrollBar"
Used to scroll the visible area of a component.
A ScrollBar is usually composed of a slider, moving within
an element that indicates the full size of the component, and
buttons with arrow graphics for moving the slider with the mouse.
The slider indicates the relative position and size of the visible
area of the component.  The slider is moved 
directly by
using the mouse pointer, or by
using the arrow keys.
.LI "\fBList\fP"
.iX "List"
.iX "field controls" "List"
Used for selecting elements from a list of elements.  A List
can allow multiple items to be selected or can be constrained
to only allow one item to be selected
at a time.  A List is usually composed of a vertical
list of items.  A List can also have both horizontal
and vertical ScrollBars for scrolling the visible portion
of the list of items.
.LI "\fBText\fP"
.iX "Text"
.iX "field controls" "text"
Used for displaying, entering, and modifying text.  There are
single line and multiple line variants.  Multiline Text can
have both horizontal and vertical ScrollBars for
scrolling the visible portion of the text area.  
.LI "\fBCanvas\fP"
.iX "Canvas" 
.iX "field controls" "Canvas"
Used for displaying, entering, and modifying graphics.
A Canvas can have both horizontal and vertical ScrollBars
for scrolling the visible portion of the drawing area.
.LE
.hS "Navigation"
Field controls have navigation that is specific to
the component.  See the reference page for each field control
for information about its navigation.
.hS "Other Operations"
Field controls have operations that are specific to
the component.  See the reference page for each field control
for information about its operations.
.hS "Related Information"
For more information,
see the reference pages for each field control.
