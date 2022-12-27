.\"  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  ALL RIGHTS RESERVED
.hR Panel
.hS "Description"
.iX "Panel"
A Panel group should be used to
organize a collection of basic controls in a
horizontal, vertical, or 2-dimensional layout.
This component is usually composed of just one
type of basic control.  This component is composed
of an area for organizing basic controls.
.hS "Illustration"
.br
.ne 1.7i
\0
.sp 1.5i
.in +1.5i
.P! graphics/radiobts.ps 
.in -1.5i
.sp
.hS "Navigation"
.mS
.BR KDown ,
.BR KLeft ,
.BR KRight ,
and
.BR KUp
must navigate among components in the Panel according to the navigation
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
