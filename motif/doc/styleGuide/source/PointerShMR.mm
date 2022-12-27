.\"  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  ALL RIGHTS RESERVED
.hR "Pointer Shapes"
.hS "Description"
.iX "pointer shapes"
.iX "shapes" "pointer"
.iX "mouse pointer shapes"
.iX "design" "pointer shapes"
You should not create new mouse pointer shapes for functions
that already have mouse pointer shapes associated with
them, except for localization; however, you can create new mouse pointer shapes for
functions not already associated with a pointer shape.
.mS
You must not use a predefined shape to symbolize a function
it was not designed to represent.
.mE
.P
.iX "hotspot"
A list of the defined pointer shapes follows, along with
their usage and hotspots.  The hotspot of a pointer is the
actual position on the pointer that tracks the movements of
the mouse.
.mS
As the pointer changes from one shape to another,
the location of the hotspot must not move on the screen.
.mE
The hotspot is the precise location on the pointer where
mouse actions occur.
.VL 15n
.if n \{
.LI "Arrow"
'br\}
.if t \{
.ne .5i
.LI ""
.br
\0
.in -15n
.P! graphics/arrowP.ps
.in +15n
.sp -.3i
.fl
'br\}
.iX "arrow pointer"
.iX "pointers" "arrow"
The upper left pointing arrow pointer is a general-purpose pointer.
It is used
in most window areas for single object selection and activation.
The hotspot for the arrow pointer should be in the point of 
the arrow.
.if n \{
.LI "I-beam"
'br\}
.if t \{
.ne .5i
.LI ""
.br
\0
.in -15n
.P! graphics/ibeamP.ps
.in +15n
.sp -.3i
.fl
'br\}
.iX "I-beam pointer"
.iX "pointers" "I-beam"
You can optionally use an I-beam pointer in any Text component.
It is used to change the location of the text insertion
cursor and to perform actions on text.
If the I-beam is used, it can be hidden during the time
between any keyboard action and a mouse movement.
This helps the user distinguish the I
beam pointer from the text insertion cursor, which can also be
an I-beam.
The hotspot for the I-beam pointer should be on the vertical
bar of the I-beam about one-third up from the bottom.
.if n \{
.LI "X"
'br\}
.if t \{
.ne .5i
.LI ""
.br
\0
.in -15n
.P! graphics/xP.ps
.in +15n
.sp -.3i
.fl
'br\}
.iX "X pointer"
.iX "pointers" "X"
The X pointer can indicate when the pointer is outside of any
application area.
The hotspot for the X pointer should be where the lines
intersect.
.if n \{
.LI "Resize"
'br\}
.if t \{
.ne .5i
.LI ""
.br
.sp +.35i
\0
.in -15n
.P! graphics/8sizeP.ps
.in +15n
.sp -.65i
.fl
'br\}
.iX "resize pointer"
.iX "pointers" "resize"
The resize pointers indicate positions for area resize, and
remains during a resize operation.  The direction of the
arrow in the pointer indicates the direction of increasing
size.  The horizontal and vertical pointers indicate resize
in either the horizontal or vertical direction.
The diagonal pointers indicate resize
in both the horizontal and vertical directions simultaneously.
The hotspot for the resizing pointers should be on the elbow or the
line at the position pointed to by the arrow.
.if n \{
.LI "Hourglass"
'br\}
.if t \{
.ne .5i
.LI ""
.br
.sp +.1i
\0
.in -15n
.P! graphics/hourglassP.ps
.in +15n
.sp -.4i
.fl
'br\}
.iX "hourglass pointer"
.iX "pointers" "hourglass"
The hourglass pointer, a working pointer, indicates that 
an action is in progress in the area, and that the pointer
has no effect in the area.  While the hourglass pointer
is active, all mouse button and keyboard events are ignored
in the area.
.mS
Even though there is no activation with the hourglass pointer,
is must still have a hotspot.
.mE
The hotspot for the
hourglass pointer should be located at the center of the hourglass.
The hourglass pointer can be used interchangeably with the watch pointer.
.if n \{
.LI "Watch"
'br\}
.if t \{
.ne .5i
.LI ""
.br
\0
.in -15n
.P! graphics/watchP.ps
.in +15n
.sp -.3i
.fl
'br\}
.iX "watch pointer"
.iX "pointers" "watch"
The watch pointer, a working pointer, indicates that 
an action is in progress in the area, and that the pointer
has no effect in the area.  While the watch pointer
is active, all mouse button and keyboard events are ignored
in the area.
.mS
Even though there is no activation with the watch pointer,
is must still have a hotspot.
.mE
The hotspot for the
watch pointer should be located at the top of the watch.
The watch pointer can be used interchangeably with the hourglass pointer.
.if n \{
.LI "Moving"
'br\}
.if t \{
.ne .5i
.LI ""
.br
\0
.in -15n
.P! graphics/movingP.ps
.in +15n
.sp -.3i
.fl
'br\}
.iX "pointers" "four-directional arrow"
.iX "arrow pointer" "four-directional"
.iX "four-directional arrow pointer"
The 4-directional arrow pointer indicates a move 
operation in progress, or a resize operation before the resize direction
has been determined.
During a move operation, the object, or an outline of the object
should move to track the location of the pointer.
During a resize operation, the pointer is used to indicate a
direction for resizing.  The 4-directional arrow pointer
should change to the appropriate resize arrow when the resize
direction is determined, either by crossing an object boundary
with the pointer or by pressing a keyboard direction key.
The hotspot for the 4-directional arrow pointer should be
at the spot where the arrows intersect.
.if n \{
.LI "Sighting"
'br\}
.if t \{
.ne .5i
.LI ""
.br
\0
.in -15n
.P! graphics/sightP.ps
.in +15n
.sp -.3i
.fl
'br\}
.iX "pointers" "sighting"
.iX "sighting pointer"
The sighting pointer is used to make fine position selections.
For example, in a drawing program it may be used to
indicate a pixel to fill or the connecting points of lines.
The hotspot for the sighting pointer should be
at the spot where the arrows intersect.
.if n \{
.LI "Caution"
'br\}
.if t \{
.ne .5i
.LI ""
.br
.sp +.1i
\0
.in -15n
.P! graphics/cautionP.ps
.in +15n
.sp -.4i
.fl
'br\}
.iX "caution pointer"
.iX "pointers" "caution"
The caution pointer is used to indicate that action is
expected in another area before input can be given to
the current area, and that the pointer has no effect in the
area.  While the caution pointer is active, all mouse button
and keyboard events are ignored in the area.
.mS
Even though there is no activation with the caution pointer,
is must still have a hotspot.
.mE
The hotspot for the
caution pointer should be located at the center of the caution symbol.
.if n \{
.LI "Question"
'br\}
.if t \{
.ne .5i
.LI ""
.br
\0
.in -15n
.P! graphics/questP.ps
.in +15n
.sp -.3i
.fl
'br\}
.iX "pointers" "question"
.iX "question pointer"
The question pointer is used to request an input position or component
from the user.
This is often used to input an object for interactive
help.
The user requests interactive help, then the question pointer
is displayed to allow the user to indicate what position or
component to provide help for.
The hotspot for the question pointer should be at the bottom of the
question mark.
.if n \{
.LI "Other\ Arrow"
'br\}
.if t \{
.ne .5i
.LI ""
.br
\0
.in -15n
.P! graphics/menuarrowP.ps
.in +15n
.sp -.3i
.fl
'br\}
.iX "arrow pointer"
.iX "pointers" "arrow"
You can optionally use an arrow pointing to the upper right corner
to indicate a pending Menu action.  This shape indicates that
a Menu is popped up or pulled down and waiting for a
Menu item to be activated or the Menu to be removed.
The hotspot for this arrow pointer should be in the point of 
the arrow.
.LE
.hS "Related Information"
See Chapter 2 for information on the input device model
and designing new pointer shapes.
