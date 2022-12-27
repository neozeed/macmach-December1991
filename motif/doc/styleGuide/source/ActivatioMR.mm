.\"  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  ALL RIGHTS RESERVED
.hR "Activation"
.hS "Description"
.iX "activation"
This reference page only provides a short description
of the various types of activation.  Chapter 3 describes
each type in detail.
.VL 10n
.LI "\fBBasic Activation\fP"
.br
.iX "basic activation"
.mS
All activatable basic controls must be
activated using basic activation.
Clicking
.B BSelect
on the button must activate the button.
Pressing
.B KActivate
and
.B KSelect
on the button with the location cursor
must activate the button.
.mE
.B KActivate
has a different function when a default button is
available.
.LI "\fBAccelerators\fP"
.br
.iX "accelerators"
An accelerator is a key or key combination that invokes
the action of some component without the location cursor
on the component when the accelerator is pressed.
Accelerators are most commonly used to activate Menu items without
first posting the Menu.
.LI "\fBMnemonics\fP"
.br
.iX "mnemonics"
A mnemonic is a single character that can be associated with
any component that contains a text label.
.mS
When the location cursor is on a component within a Menu, a MenuBar or
the same field as a component with a mnemonic,
typing the mnemonic character must move the location
cursor to the component and activate it.
If a mnemonic is used for an OptionButton, for a CascadeButton in a
MenuBar, or a PushButton that is not in a basic group
(that is, not in a Panel, a Menu, or a MenuBar), typing
.B MAlt
and the mnemonic anywhere in the window or its Menus must
move the cursor to the component with that mnemonic
and must activate it.
.mE
.LI "\fBHelp Activation\fP"
.br
.iX "help" "activation"
.mS
Pressing
.B KHelp
on a component must invoke any context-sensitive help
for the component or its nearest ancestor with
context-sensitive help available.
.mE
Within DialogBoxes,
context-sensitive help should provide help information
or the DialogBox as a whole.
.LI "\fBPopup Menu Activation\fP"
.br
.iX "Popup Menus"
.mS
Clicking
.BR BMenu 
in a component with a Popup Menu
must post the Popup Menu.
Pressing
.B BMenu
in a component with a Popup Menu
must post the Popup Menu in a spring-loaded manner until
the button is released or a selection is made.
.mE
.LI "\fBDefault Activation\fP"
.br
.iX "default action" "selecting"
.mS
In a DialogBox,
pressing
.B KActivate
anywhere except in a PushButton component
or double-clicking
.B BSelect
in an activatable field component, such as
Text or List,
must activate the default PushButton in the DialogBox.
.mE
.LI "\fBExpert Activation\fP"
.br
.iX "expert action"
.iX "double-clicking"
Some elements, usually PushButtons and ToggleButtons,
can have expert actions associated with them.
.mS
.BR "BSelect Click 2"
must activate any expert action for the element.
.mE
Expert action should only be available in a Panel,
and the expert action of all the buttons should be similar.
The expert action should include the regular action
of the component in a more global manner.
.LE
.br
.ne 20
.hS "Common Bindings"
.TS
box, tab(@);
lb lb.
Virtual Key@Default Bindings
_
KEnter@<Return>
@<Enter>
KActivate@KEnter
@MCtrl KEnter
KSelect@<Select>
@<Space>
@MCtrl <Space>
KHelp@<F1>
@<Help>
.TE
.hS "Related Information"
See Chapter 3 for more information about the activation
model.


