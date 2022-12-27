.\"  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  ALL RIGHTS RESERVED
.hR "Accelerators"
.hS "Description"
.iX "accelerators"
An accelerator is a key or key combination that invokes
the action of some component without the location cursor
being on the component when the accelerator is pressed.
Accelerators are most commonly used to activate Menu items without
first posting the Menu.
You should provide accelerators primarily as a matter
of utility, not for design conformity.
.P
.mS
If the button with an accelerator is within a primary or
secondary window, or within a PullDown Menu system
from its MenuBar, it must be activatable whenever the
input focus is in the window.  If the button with an
accelerator is within a Popup Menu system, it must be
activatable whenever the focus is in the component with
the Popup Menu.
.mE
.P
Applications can provide accelerators for any button
component.
.mS
Implementations must support accelerators in
PushButtons and ToggleButtons
that are in Menus.
If a button has an accelerator, the accelerator must
be shown following the label of the button.
.mE
