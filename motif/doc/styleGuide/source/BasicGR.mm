.\"  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  ALL RIGHTS RESERVED
.hR "Basic Groups"
.hS "Description"
Basic groups of controls are used to organize groups
of basic controls.  The following text describes 
the basic groups.
.VL 17n
.LI "\fBPanel\fP"
.iX "Panel"
Organizes a collection of basic controls in a horizontal,
vertical, or 2-dimensional layout.
A Panel is usually composed of just one type of control.
.LI "\fBMenu\fP"
.iX "Menu"
Organizes a collection of buttons, labels, and separators
in a horizontal, vertical, or 2-dimensional layout within
a separate Menu window.  There are three types of Menus:
PullDown, Popup, and Option.  A Menu is only available
while it is popped up or pulled down.
.LI "\fBMenuBar\fP"
.iX "MenuBar"
Organizes a collection of CascadeButtons in a horizontal
layout at the top of a MainWindow.
.LE
.hS "Navigation"
.mS
.BR KDown ,
.BR KLeft ,
.BR KRight ,
and
.BR KUp
must navigate within a basic group according to the navigation
model described in Chapter 2.
.mE
.hS "Common Bindings"
.TS
box, tab(@);
lb lb.
Virtual Keys@Default Bindings
_
KDown@<\(da>
KLeft@<\(<->
KRight@<\(->>
KUp@<\(ua>
.TE
.hS "Related Information"
For more information, see the reference pages for each basic group.
