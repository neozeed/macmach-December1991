.\"  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  ALL RIGHTS RESERVED
.hR "Default Activation"
.hS "Description"
.iX "default action" "selecting"
.iX "selection" "default"
The default action of a DialogBox is activated according
to the following rules:
.BL
.LI
.mS
When the input focus is on a PushButton other than
the default, and if the component accepts input, 
pressing
.B KActivate
must activate the PushButton.
The highlighting must be removed from the default PushButton.
.mE
The highlighting can be moved to the new PushButton.
When the focus policy is implicit, the changes in highlighting
should have a delay to prevent flicker when moving the pointer
through the components.
.LI
.mS
When the focus is on an activatable field control, such as List
or Text, and if the control accepts input,
pressing
.BR "BSelect Click 2"
or
.BR KActivate
must activate both the field control's corresponding action
and the default action of the DialogBox.
.mE
.LI
.mS
When the input focus is on any other component,
and if the component accepts input, 
pressing
.B KActivate
must activate
the default
action of the DialogBox, but not the
action of the component.
.mE
.LI
.mS
When the focus is not on any component,
pressing
.B KActivate
must activate the default action of the DialogBox.
.mE
.LE
.hS "Related Information"
See Chapter 3 for general information about the activation model
and default activation.


































