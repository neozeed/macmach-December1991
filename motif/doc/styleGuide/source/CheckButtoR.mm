.\"  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  ALL RIGHTS RESERVED
.hR CheckButton
.hS "Description"
.iX "CheckButton"
.iX "ToggleButton"
A CheckButton should be used to set options
in the application.  A CheckButton is a special case of
a ToggleButton.  Any number of CheckButtons can be
set at the same time.
.P
.mS
This component must be composed of a text or graphic label, and
a graphic that indicates the state of the CheckButton.
.mE
The graphic should be a square preceding the label, and
should have two distinctive states indicating the set 
and unset states of the CheckButton.
On color systems, the on state color can be
distinct from general application colors to visually distinguish
the on state.
.hS "Illustration"
.br
.ne 1.1i
\0
.sp 0.9i
.in +2.4i
.P! graphics/checkbut.ps 
.in -2.4i
.sp
.hS "Navigation"
.mS
CheckButtons must have no internal navigation.
.mE
.hS "Other Operations"
The following text describes the CheckButton operations.
.VL 10n
.LI "\fBBSelect Press\fP:"
.br
.mS
Must arm the CheckButton.  If the CheckButton was
previously unset, it must show the CheckButton in
the set state.  If the CheckButton was previously
set, it must show the CheckButton in the unset state.
.mE
.LI "\fBBSelect Release\fP:"
.br
.mS
If the release happens in the same CheckButton that
the press occurred in, then if the CheckButton was
previously unset, it must be set, or if the CheckButton
was previously set, it must be unset.
In all cases the CheckButton must be disarmed,
and if the CheckButton was in a Menu, the
Menu must be unposted.
.mE
.LI "\fBKActivate\fP:"
.mS
If the CheckButton was
previously unset, it must be set.
If the CheckButton
was previously set, it must be unset.
In both cases the CheckButton must be disarmed,
and if the CheckButton was in a Menu, the
Menu must be unposted.
.mE
.LI "\fBKSelect\fP:"
.mS
If the CheckButton was
previously unset, it must be set.
If the CheckButton
was previously set, it must be unset.
In both cases the CheckButton must be disarmed,
and if the CheckButton was in a Menu, the
Menu must be unposted.
.mE
.LI "\fBKHelp\fP:"
.mS
Must provide any available help for the CheckButton.
.mE
.LE
.br
.ne 19
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
For more information, see the reference pages
for RadioButton and ToggleButton.
