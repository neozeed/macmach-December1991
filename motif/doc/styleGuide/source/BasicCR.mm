.\"  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  ALL RIGHTS RESERVED
.hR "Basic Controls"
.hS "Description"
Basic controls are components that only take simple input.
They are distinct from field controls in that they
are usually elements of navigation fields rather than
fields themselves.
.mS
Basic controls must have no internal 
navigation.
.mE
The following text describes the basic controls.
.VL 17n
.LI "\fBSeparator\fP"
.iX "Separator"
Draws a separating line within windows, between Menu items,
and between Panes of a PanedWindow.
A Separator allows no application interaction.
.LI "\fBLabel\fP"
.iX "Label"
Displays static text and images.
A Label presents application information to users.
.LI "\fBPushButton\fP"
.iX "PushButton"
A button used to activate an operation.
A PushButton contains a label that indicates the operation of
the button.  The label can contain text or an image.
.LI "\fBCascadeButton\fP"
.iX "CascadeButton"
A button used to display a PullDown Menu.
A CascadeButton contains a label that indicates the Menu displayed.
CascadeButtons can also contain an arrow graphic
after the label to distinguish it from PushButtons and to
indicate the direction of the cascading Menu.
.LI "\fBOptionButton\fP"
.iX "OptionButton"
A button used to display an Option Menu.  An Option Menu
allows for a one of many selection.  An OptionButton contains
a label that indicates the current state of the Option Menu,
and a bar graphic to distinguish it from a PushButton.
.LI "\fBToggleButton\fP"
.iX "ToggleButton"
A button with two states: on and off.
A ToggleButton contains a label that indicates the state of
the ToggleButton when it is set.
Normally, preceding the label is a graphic indicator of the state
of the ToggleButton.
.LI "\fBCheckButton\fP"
.iX "CheckButton"
A ToggleButton in a group of ToggleButtons where any
number of the ToggleButtons can be on at a time.
The graphic indicator for a CheckButton is usually a raised
square to indicate the on state or a pressed square to indicate
the off state.
.LI "\fBRadioButton\fP"
.iX "RadioButton"
A ToggleButton in a group of ToggleButtons where only
one of the ToggleButtons can be on at a time.
The graphic indicator for a RadioButton is usually a filled
diamond or circle to indicate the on state or an empty diamond or
circle to indicate the off state.
.LE
.P
A single line Text control can be configured to act like a basic
control.
.hS "Navigation"
.mS
Basic controls must have no internal navigation.
.mE
.hS "Other Operations"
Activatable basic controls follow the basic activation model
described in Section 3.3.1.
.hS "Related Information"
For more information, see the reference pages for each basic control.
