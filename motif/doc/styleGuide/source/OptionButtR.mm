.\"  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  ALL RIGHTS RESERVED
.hR OptionButton
.hS "Description"
.iX "OptionButton"
.iX "Options Menu"
.iX "menus" "option"
An OptionButton should be used to post an Option Menu.
.P
.mS
This component must be composed of a button, with either
a text or graphics label.
.mE
Following the label,
this component should also
include a bar graphic
to distinguish it from a PushButton.
.mS
The label must be the last selection made from the OptionButton.
.mE
.hS "Illustration"
.br
.ne .7i
\0
.sp .5i
.in +2.4i
.P! graphics/optionbtn.ps
.in -2.4i
.sp
.hS "Navigation"
.mS
An OptionButton must have no internal navigation.
.mE
.hS "Other Operations"
The following text describes the operations
of this component.
.VL 10n
.LI "\fBBSelect Press\fP:"
.br
.mS
Must activate the OptionButton and post the associated
Option Menu.
.mE
.LI "\fBBSelect Release\fP:"
.br
.mS
If the release is within the same OptionButton as the press, 
.B "BSelect Release"
must deactivate the OptionButton and leave the
associated Option Menu posted.
If the release is outside of the OptionButton,
.B "BSelect Release"
must deactivate the OptionButton and unpost the associated
Option Menu.
.mE
.LI "\fBKActivate\fP:"
.mS
Must post the associated Option Menu.
.mE
.LI "\fBKSelect\fP:"
.mS
Must post the associated Option Menu.
.mE
.LI "\fBMAny KCancel\fP:"
.br
.mS
Must unpost the last posted Menu in the Menu system
without changing the value of the OptionButton.
.mE
.LI "\fBKHelp\fP:"
.mS
Must provide any available help for the OptionButton.
.mE
.LE
.br
.ne 20
.hS "Common Bindings"
.TS
box, tab(@);
lb lb.
Virtual Keys@Default Bindings
_
BSelect@<Btn1>
KEnter@<Return>
@<Enter>
KActivate@KEnter
@MCtrl KEnter
KSelect@<Select>
@<Space>
@MCtrl <Space>
KHelp@<Help>
@<F1>
KCancel@<Escape>
@<Cancel>
.TE
.hS "Related Information"
For more information,
see the reference page for Menus.
