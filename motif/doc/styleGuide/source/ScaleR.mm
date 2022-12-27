.\"  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  ALL RIGHTS RESERVED
.hR Scale
.hS "Description"
.iX "Scale"
A Scale should be used to select a value
from a range.  This component should be composed of
a slider, moving within an element that indicates the
size of the range, called the trough, and a label that indicates
the current value.  The position of the slider
that indicates the value relative to the range.
A Scale can also have buttons with arrow graphics
for moving the slider.
.hS "Illustration"
.br
.ne 1.8i
\0
.sp 1.6i
.in +2.3i
.P! graphics/scale.ps 
.in -2.3i
.sp
.hS "Other Operations"
The following text describes the operations
of this component.
.VL 10n
.LI "\fBBSelect Press\fP:"
.br
.mS
In an arrow button,
this action
must move the slider one increment in the direction
of the side of the slider on which the button was pressed,
and autorepeat until the button is released.
.mE
.P
.mS
In the trough,
if the Scale has tick marks,
.B "BSelect Press"
must move the slider one tick mark in the direction
of the side of the slider on which the trough was pressed,
and autorepeat until the button is released.
.mE
It can stop
when the slider reaches the position of the pointer.
.mS
If the Scale does not have tick marks,
.B "BSelect Press"
in the trough
must move the slider one large increment in the direction
of the side of the slider on which the trough was pressed,
and autorepeat until the button is released.
.mE
It can stop
when the slider reaches the position of the pointer.
.LI "\fBBSelect Drag\fP:"
.br
.mS
Must cause the slider to track the position of the pointer.
In a vertical Scale, the slider must track the vertical
position of the pointer.
In a horizontal Scale, the slider must track the horizontal
position of the pointer.
.mE
.LI "\fBMCtrl BSelect Press\fP:"
.br
.mS
In the slider trough or an arrow button,
this action
must move the slider to the end of the Scale on which the button
was pressed.
.mE
.LI "\fBBDrag Press\fP:"
.br
In the trough,
this action
can directly position the slider to the position of the
button press.
.LI "\fBBDrag Drag\fP:"
.br
.mS
Must cause the slider to track the position of the pointer.
In a vertical ScrollBar, the slider must track the vertical
position of the pointer.
In a horizontal ScrollBar, the slider must track the horizontal
position of the pointer.
.mE
.P
In the trough,
this action
can directly position the slider to the position of the
button press, and then cause the slider to track the position
of the pointer.
.LI "\fBKDown\fP:"
.mS
For vertical Scales,
this action
must move the slider one increment down.
.mE
.LI "\fBKUp\fP:"
.mS
For vertical Scales,
this action
must move the slider one increment up.
.mE
.LI "\fBKLeft\fP:"
.mS
For horizontal Scales,
this action
must move the slider one increment left.
.mE
.LI "\fBKRight\fP:"
.mS
For horizontal Scales,
this action
must move the slider one increment right.
.mE
.LI "\fBMCtrl KDown\fP:"
.br
.mS
For vertical Scales,
this action
must move the slider one large increment down.
.mE
.LI "\fBMCtrl KUp\fP:"
.br
.mS
For vertical Scales,
this action
must move the slider one large increment up.
.mE
.LI "\fBMCtrl KLeft\fP:"
.br
.mS
For horizontal Scales,
this action
must move the slider one large increment left.
.mE
.LI "\fBMCtrl KRight\fP:"
.br
.mS
For horizontal Scales,
this action
must move the slider one large increment right.
.mE
.LI "\fBKBeginLine\fP or \fBKBeginData\fP:"
.br
.mS
Must move the slider to the minimum value.
.mE
.LI "\fBKEndLine\fP or \fBKEndData\fP:"
.br
.mS
Must move the slider to the maximum value.
.mE
.LI "\fBKHelp\fP:"
.mS
Must provide any available help for the Scale.
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
KBeginLine@<Begin>
@<Home>
KBeginData@MCtrl <Begin>
@MCtrl <Home>
KEndLine@<End>
KEndData@MCtrl <End>
KHelp@<Help>
@<F1>
.TE
