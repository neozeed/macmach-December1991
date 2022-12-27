.\"  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  ALL RIGHTS RESERVED
.hR ToggleButton
.hS "Description"
.iX "ToggleButton"
.iX "RadioButton"
.iX "CheckButton"
A ToggleButton should be used to
select options in an application.  When
the choice is one of many, the ToggleButton
is called a RadioButton.  When the choice
is any of many, the ToggleButton is called
a CheckButton.
.mS
The ToggleButton must be composed
of a text or graphic label,
.mE
and a can contain a graphic that indicates the state of the ToggleButton.
The graphic should precede the label, and
should have two distinctive states that indicate the set 
and unset states of the ToggleButton.
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
ToggleButtons must have no internal navigation.
.mE
.hS "Related Information"
For a description of the activation of the two types of
ToggleButtons,
see the reference pages for CheckButton and RadioButton.
