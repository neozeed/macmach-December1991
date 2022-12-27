.\"  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  ALL RIGHTS RESERVED
.hR "Drag and Drop"
.hS "Description"
.iX "drag-and-drop model"
The drag-and-drop model allows the user to move or
copy the selected elements by dragging
them to a new location.
Even collections that do not support dragging of their own elements
can support drops of elements from other collections.
.P
If a component implements dragging, it should
use
.B BDrag
to perform the drag operations.  Some implementations
may choose to use
.B BSelect
instead of 
.BR BDrag .
.mS
When
.B BSelect
is used for dragging operations, its use must
supersede the use of
.B "BSelect Drag"
in the selection models described in Section 3.1.
Applications that implement the drag-and-drop model
using
.B BSelect
must also support the use of
.BR BDrag
instead.
.mE
.P
.mS
When using
.B BDrag
for dragging elements,
if
.B BDrag
is pressed on a selected element or within a linear range of
selected elements,
it must drag the entire selection until
it is released.
If
.B BDrag
is pressed outside of the selected range
in a text-like collection, it must do a quick transfer instead
of dragging the element.
.mE
Quick transfer is described in
Section 3.2.3.
If
.B BDrag
is pressed on an unselected element
in a list-like collection, it can either do a quick transfer
or a dragging operation.
The behavior of
.B "BDrag Press"
in a graphics-like collection is unspecified.  It can either
do a quick transfer, a dragging operation, or pan the visible
region.
.P
The type of the elements dragged, and the source and destination
of the drag-and-drop operations determine whether the operation
causes a move or a copy as follows:
.BL
.LI
.mS
Dragging to a transport device, such as a printer or mailbox,
must be treated as a copy.
.mE
.LI
.mS
Dragging to a removal device, such as a wastebasket or shredder,
must be treated as a move.
.mE
.LI
.mS
Dragging to a non-Text container must be treated as a move.
.mE
.LI
.mS
Dragging text to or from a Text component must be treated as a copy.
.mE
.LE
.P
.mS
Pressing
.B MCtrl
with the drag operation must make the operation a copy.
Pressing
.B MAlt
with the drag operation must make the operation a move.
.mE
.hS "Related Information"
See Chapter 3 for more information on the drag-and-drop
model, and for information on quick transfer and the 
selection models.
