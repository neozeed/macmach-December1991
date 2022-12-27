.\"  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  ALL RIGHTS RESERVED
.hR CommandBox
.hS "Description"
.iX "CommandBox"
A CommandBox
is a special purpose composite component for command entry
that provides a built-in command history mechanism.
.mS
The CommandBox must be composed of a Text component with a command line prompt
for command input,
and a List component above the Text component for a command history area.
When a command is entered in the Text component, it must be added to
the end of the List component and made visible.  This must not change
the state of the selection in the List if one exists.
.mE
The List component can be scrollable.
.P
.hS "Illustration"
.br
.ne 2.35i
\0
.sp 2.15i
.in +1.8i
.P! graphics/commandbox.ps
.in -1.8i
.hS "Related Information"
For more information,
see the reference page for Command DialogBox.
