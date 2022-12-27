.\"  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  ALL RIGHTS RESERVED
.hR CascadeButton
.hS "Description"
.iX "CascadeButton"
A CascadeButton should be used to post a PullDown Menu.
.mS
This component must be composed of a button, with either
a text or graphics label.
.mE
Following the label,
this component should also
include an arrow graphic, pointing in the direction that the
Menu will be posted to distinguish it from a PushButton.
The graphic is usually not shown in a MenuBar.
.hS "Illustration"
.br
.ne 2.2i
\0
.sp 2.0i
.in +1.0i
.P! graphics/cascade.ps 
.in -1.0i
.sp
.hS "Navigation"
.mS
A CascadeButton must have no internal navigation.
.mE
.hS "Other Operations"
CascadeButtons follow the Menu activation model described
in Section 2.3.3.
The following text describes the other operations
of this component.
.VL 10n
.LI "\fBKHelp\fP:"
.mS
Must provide any available help for the CascadeButton.
.mE
.LE
.hS "Common Bindings"
.TS
box, tab(@);
lb lb.
Virtual Keys@Default Bindings
_
KHelp@<Help>
@<F1>
.TE
.hS "Related Information"
For more information, see the reference pages for Menus.
