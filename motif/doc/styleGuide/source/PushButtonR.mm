.\"  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  ALL RIGHTS RESERVED
.hR PushButton
.hS "Description"
.iX "PushButton"
A PushButton should be used to start
an operation.
.mS
A PushButton must contain
either a text or graphic label indicating the
operation of the button.
.mE
.hS "Illustration"
.br
.ne .75i
\0
.sp .55i
.in +2.6i
.P! graphics/pushbtn.ps
.in -2.6i
.sp
.hS "Navigation"
.mS
A PushButton must have no internal navigation.
.mE
.hS "Other Operations"
The following text describes the operations
of this component.
.VL 10n
.LI "\fBBSelect Press\fP:"
.br
.mS
Must arm the PushButton.
.mE
.LI "\fBBSelect Release\fP:"
.br
.mS
If the release is within the same PushButton as the press, 
.B "BSelect Release"
must disarm the PushButton and activate it.
If the release is outside of the PushButton,
.B "BSelect Release"
must disarm the PushButton without activating it.
.mE
.LI "\fBKActivate\fP:"
.mS
Must activate the PushButton.
.mE
.LI "\fBKSelect\fP:"
.mS
Must activate the PushButton.
.mE
.LI "\fBKHelp\fP:"
.mS
Must provide any available help for the PushButton.
.mE
.LE
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
.TE
