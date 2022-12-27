.\"  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  ALL RIGHTS RESERVED
.hR PanedWindow
.hS "Description"
.iX "PanedWindow"
.iX "window panes"
.iX "Panes"
.iX "Sash"
A PanedWindow should be used to group components
into Panes separated by Sashes and
Separators for
adjusting the relative size of each Pane.
.mS
This component must be composed of any number of groups of
components, called Panes, each
separated by a Sash and a Separator.
The Panes, Sashes and Separators must be grouped linearly, either
horizontally or vertically.  A Sash must be composed of a 
handle on the Separator between two Panes
for adjusting the position of the Separator, and therefore
the size of the Panes next to it.
.mE
.P
.mS
As a Sash is moved, the Pane in the direction of the Sash
movement must get smaller and the other Pane must get larger
by an equal amount.
If a Pane is adjusted to its minimum size, adjustment must continue
with the next Pane in the direction of the Sash movement.
.mE
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
This component must follow the navigation model described in
Chapter 2.
.mE
.hS "Related Information"
For information about
activating a Sash, 
see the reference page for Sash.
