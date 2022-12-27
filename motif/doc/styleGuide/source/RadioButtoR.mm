.\"  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  ALL RIGHTS RESERVED
.hR RadioButton
.hS "Description"
.iX "RadioButton"
.iX "ToggleButton"
A RadioButton should be used to select one option
from a number of options.
A RadioButton is a special case of
a ToggleButton.  Only one RadioButton can be
set at a time.
.mS
This component must be composed of a text or graphic label, and
a graphic that indicates the state of the RadioButton.
.mE
The graphic should be a diamond preceding the label, and
should have two distinctive states that indicate the set 
and unset states of the RadioButton.
On color systems, the on state color can be
distinct from general application colors to visually distinguish
the on state.
.hS "Illustration"
.br
.ne 2.2i
\0
.sp 2.0i
.in +2.25i
.P! graphics/single.ps 
.in -2.25i
.sp
.hS "Navigation"
.mS
RadioButtons must have no internal navigation.
.mE
.hS "Other Operations"
The following text describes the operations
of this component.
.VL 10n
.LI "\fBBSelect Press\fP:"
.br
.mS
Must arm the RadioButton.  If the RadioButton was
previously unset, it must show the RadioButton in
the set state.
.mE
.LI "\fBBSelect Release\fP:"
.br
.mS
If the release happens in the same RadioButton that
the press occurred in, then if the RadioButton was
previously unset, it must be set, and any other RadioButton
in the same Panel that was previously set must
be unset.
The RadioButton must be disarmed,
and if the RadioButton was in a Menu, the
Menu must be unposted.
.mE
.LI "\fBKActivate\fP:"
.mS
If the RadioButton was
previously unset, it must be set, and any other RadioButton
in the same Panel that was previously set must be unset.
The RadioButton must be disarmed,
and if the RadioButton was in a Menu, the
Menu must be unposted.
.mE
.LI "\fBKSelect\fP:"
.mS
If the RadioButton was
previously unset, it must be set, and any other RadioButton
in the same Panel that was previously set must be unset.
The RadioButton must be disarmed,
and if the RadioButton was in a Menu, the
Menu must be unposted.
.mE
.LI "\fBKHelp\fP:"
.mS
Must provide any available help for the RadioButton.
.mE
.LE
.br
.ne 17
.hS "Common Bindings"
.TS
box, tab(@);
lb lb.
Virtual Keys@Default Bindings
_
BSelect@<Btn1>
KEnter@<Return>
KActivate@<Enter>
@MCtrl <Enter>
KSelect@<Select>
@<Space>
@MCtrl <Space>
KHelp@<Help>
@<F1>
.TE
.hS "Related Information"
For more information,
see the reference pages for CheckButton and ToggleButton.
