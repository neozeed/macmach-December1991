.\"  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  ALL RIGHTS RESERVED
.hR Sash
.hS "Description"
.iX "Sash"
.iX "PanedWindow"
.iX "window panes"
A Sash should be used to adjust the border
between groups of components in a PanedWindow.
.mS
A Sash must be composed of a 
handle on the Separator between two Panes
for adjusting the position of the Separator, and therefore
the size of the Panes next to it.
.mE
.P
.mS
As a Sash is moved, the Pane in the direction of the Sash
movement must get smaller and the other Pane must get larger
by an equal amount.
.mE
If a Pane is adjusted to its minimum size, adjustment should continue
with the next Pane in the direction of the Sash movement.
The PanedWindow can optionally support movement of the Pane in
the initial direction of mouse movement with the modifier
.BR MShift
on button events,
rather than resizing the Pane.
.hS "Illustration"
.br
.ne 3.5i
\0
.sp 3.3i
.in +1.9i
.P! graphics/panedw2.ps 
.in -1.9i
.sp
.hS "Navigation"
.mS
A Sash must have no internal navigation.
.mE
.hS "Other Operations"
The following text describes the operations
of this component.
.VL 10n
.LI "\fBBSelect\fP or \fBBDrag Drag\fP:"
.br
.mS
Must cause the Sash to track the movement of the pointer.
In a vertically oriented PanedWindow, the Sash must
track the vertical position of the pointer.
In a horizontally oriented PanedWindow, the Sash must
track the horizontal position of the pointer.
.mE
.LI "\fBBSelect\fP or \fBBDrag Drag\fP:"
.br
Can cause the Pane in the initial direction of movement
to track the movement of the pointer.
.mS
In a vertically oriented PanedWindow, the Pane must
track the vertical position of the pointer.
In a horizontally oriented PanedWindow, the Pane must
track the horizontal position of the pointer.
.mE
.LI "\fBKUp\fP, \fBKDown\fP, \fBKRight\fP, or \fBKLeft\fP:"
.br
.mS
Must cause the Sash to move in the specified direction one
increment.
.mE
.LI "\fBMShift KUp\fP, \fBKDown\fP, \fBKRight\fP, or \fBKLeft\fP:"
.br
Can cause the Pane in the initial direction to move one
increment in the specified direction.
.LI "\fBMCtrl KUp\fP, \fBKDown\fP, \fBKRight\fP, or \fBKLeft\fP:"
.br
.mS
Must cause the Sash to move in the specified direction one
large increment.
.mE
.LI "\fBMCtrl MShift KUp\fP, \fBKDown\fP, \fBKRight\fP, or \fBKLeft\fP:"
.br
Can cause the Pane in the initial direction to move one
large increment in the specified direction.
.LI "\fBKHelp\fP:"
.mS
Must provide any available help for the Sash.
.mE
.LE
.hS "Common Bindings"
.TS
box, tab(@);
lb lb.
Virtual Keys@Default Bindings
_
BSelect@<Btn1>
BDrag@<Btn2>
KDown@<\(da>
KLeft@<\(<->
KRight@<\(->>
KUp@<\(ua>
KHelp@<Help>
@<F1>
.TE
.hS "Related Information"
For more information,
see the reference page for PanedWindow.
