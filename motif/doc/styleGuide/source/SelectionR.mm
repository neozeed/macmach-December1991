.\"  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  ALL RIGHTS RESERVED
.hR SelectionBox
.hS "Description"
.iX "SelectionBox"
A SelectionBox
is a special purpose composite component for making selections
from a list of choices.
.mS
The SelectionBox must be composed of a Text component
for the selected alternative,
and a List component above the Text component for presenting the
alternatives.
.mE
Both the List and Text components should have a Label above them.
The List component can be scrollable.
The SelectionBox should also include one of the following button
arrangements.
.DS
\fBOK Cancel Help\fP
\fBOK Cancel Apply Help\fP
.DE
.P
.mS
The List must use either the single or browse selection model.
Selecting an element from the list must place the selected element
in the Text component.
Entering an item name into the Text component must make that
element visible in the List component.
.mE
.hS "Illustration"
.br
.ne 4.2i
\0
.sp 4.0i
.in +1.4i
.P! graphics/selbox.ps 
.in -1.4i
.hS "Related Information"
For more information,
see the reference page for Selection DialogBox.
