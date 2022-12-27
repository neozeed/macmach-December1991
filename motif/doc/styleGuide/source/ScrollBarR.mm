.\"  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  ALL RIGHTS RESERVED
.hR ScrollBar
.hS "Description"
.iX "ScrollBar"
A ScrollBar should be used to scroll the
visible area of another component.
A ScrollBar should be composed of a slider, moving within
an element that indicates the full size of the scrolled component,
and two buttons with arrow graphics for moving the slider.
The slider indicates the relative position and size of
the visible area of the scrolled component.
.hS "Illustration"
.br
.ne .45i
\0
.sp .25i
.in +1.2i
.P! graphics/scrollbar.ps
.in -1.2i
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
this action
must move the slider one page in the direction
of the side of the slider on which the trough was pressed,
and autorepeat until the button is released.
.mE
It can stop
when the slider reaches the position of the pointer.
.LI "\fBBSelect Drag\fP:"
.br
.mS
Must cause the slider to track the position of the pointer.
In a vertical ScrollBar, the slider must track the vertical
position of the pointer.
In a horizontal ScrollBar, the slider must track the horizontal
position of the pointer.
.mE
.LI "\fBMCtrl BSelect Press\fP:"
.br
.mS
In the slider trough or an arrow button,
this action
must move the slider to the end of the ScrollBar on which the button
was pressed.
.mE
.LI "\fBMShift BSelect Press\fP:"
.br
Can
scroll the view so that the cursor within that view is positioned at the
edge of the view region on the side of the ScrollBar on which the
button was pressed.
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
For vertical ScrollBars,
this action
must move the slider one increment down.
.mE
.LI "\fBKUp\fP:"
.mS
For vertical ScrollBars,
this action
must move the slider one increment up.
.mE
.LI "\fBKLeft\fP:"
.mS
For horizontal ScrollBars,
this action
must move the slider one increment left.
.mE
.LI "\fBKRight\fP:"
.mS
For horizontal ScrollBars,
this action
must move the slider one increment right.
.mE
.LI "\fBMCtrl KDown\fP:"
.br
.mS
For vertical ScrollBars,
this action
must move the slider one large increment down.
.mE
.LI "\fBMCtrl KUp\fP:"
.br
.mS
For vertical ScrollBars,
this action
must move the slider one large increment up.
.mE
.LI "\fBMCtrl KLeft\fP:"
.br
.mS
For horizontal ScrollBars,
this action
must move the slider one large increment left.
.mE
.LI "\fBMCtrl KRight\fP:"
.br
.mS
For horizontal ScrollBars,
this action
must move the slider one large increment right.
.mE
.LI "\fBKPageDown\fP:"
.br
.mS
For vertical ScrollBars,
this action
must move the slider one page down.
.mE
.LI "\fBKPageUp\fP:"
.mS
For vertical ScrollBars,
this action
must move the slider one page up.
.mE
.LI "\fBKPageLeft\fP:"
.mS
For horizontal ScrollBars,
this action
must move the slider one page left.
.mE
.LI "\fBKPageRight\fP:"
.br
.mS
For horizontal ScrollBars,
this action
must move the slider one page right.
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
Must provide any available help for the ScrollBar.
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
KPageUp@<PageUp>
KPageDown@<PageDown>
KPageLeft@MCtrl <PageUp>
KPageRight@MCtrl <PageDown>
KBeginLine@<Begin>
@<Home>
KBeginData@MCtrl <Begin>
@MCtrl <Home>
KEndLine@<End>
KEndData@MCtrl <End>
KHelp@<Help>
@<F1>
.TE
