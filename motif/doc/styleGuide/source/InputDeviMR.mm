.\"  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  ALL RIGHTS RESERVED
.hR "Input Devices"
.hS "Description"
.iX "input devices"
.iX "mouse"
.iX "graphics tablet"
.iX "joystick"
.iX "track ball"
.iX "operation" "keyboard only"
.iX "operation" "mouse only"
.iX "keyboard" "input"
.iX "BSelect"
.iX "BDrag"
.iX "BCustom"
.iX "virtual key mechanism"
.iX "pointing devices"
The most typical pointing device is a mouse, although a
graphics tablet, track ball, joystick, and other tools
also work as pointing devices.
You can use any pointing device in place of a mouse.
This guide assumes
that a mouse, or any pointing device, has the following three buttons.
Chapter 2 describes the button bindings for
pointing devices that do not have three buttons.
.VL 10n
.LI "\fBBSelect\fP"
Used for selection, activation, and setting the location cursor.
.mS
This button must be the leftmost mouse button, except for left-handed
users where it can be the rightmost button.
.mE
.LI "\fBBDrag\fP"
Used for moving and copying elements.
.mS
This button must be the middle mouse button.
.mE
.LI "\fBBCustom\fP"
Used for additional user interactions, including popping up
Menus and special dialogs, and for application-specific
interactions.
.mS
This button must be the rightmost mouse button, except for left-handed
users where it can be the leftmost button.
This guide also mentions the button
.BR BMenu ,
which must be
bound to
.BR BCustom .
.mE
.B BMenu
is used for popping up Menus.
.LE
.P
.iX "keys" "virtual"
.iX "virtual keys
Because not all keyboards are the same, it is difficult
to give style guidelines that are correct for every
manufacturer's keyboard.  To solve this problem, this
guide describes keys using a virtual mechanism; that is,
by the action they perform.  Everywhere that keyboard
input is specified, the keys are indicated by their virtual
key name.
.P
The virtual key mechanism includes keys, modifiers, and mouse buttons.
The mouse button virtual keys are described earlier in this reference
page as
.BR BSelect ,
.BR BDrag ,
.BR BCustom ,
and
.BR BMenu .
Note that all button virtual keys start with the letter
.BR B .
All normal key virtual keys start with the letter
.BR K .
All modifier key virtual keys start with the letter
.BR M .
.P
This guide assumes that a keyboard has all the standard
alphabetic, numeric, and symbol keys,
as well as the special keys
.BR <Return> ,
.BR <Tab> ,
.BR <Escape> ,
.BR <Delete> ,
.BR <Spacebar> ,
and
.BR "<Backspace>" .
This
.I "Style Guide"
describes the common bindings of some actions in terms
of the function keys
.B <F1>
through
.BR <F10> .
These are nominal bindings and can be changed if
the function keys are not available, or if another
binding is preferred.
It also assumes that the arrow keys are available.
Additional keys, including navigation keys and special keys like
.BR <Menu>
and
.BR <Help> ,
are suggested, but not necessary for full keyboard equivalence.
.P
This guide also assumes that all keyboards have the three modifier
keys
.BR MShift ,
.BR MCtrl ,
and
.BR MAlt ,
which are usually bound to
.BR <Shift> ,
.BR <Ctrl> ,
and
.BR <Alt>
or
.BR <Meta> .
Your keyboard may have the virtual buttons, keys, or modifiers
bound as appropriate.  
Appendix A
lists all of the virtual keys used in this guide, as well
as suggested and alternative bindings.
.hS "Related Information"
See Chapter 2 for a more information about input devices.
See Appendix A for a listing of virtual keys and their common
bindings.
