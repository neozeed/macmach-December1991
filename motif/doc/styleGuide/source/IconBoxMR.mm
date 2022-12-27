.\"  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  ALL RIGHTS RESERVED
.hR "IconBox"
.hS "Description"
.iX "IconBox"
.iX "box" "icon"
An IconBox is a specialized window that acts as a storage location
for icons.
The illustration on this reference page shows a typical IconBox.
.P
.mS
The window manager must allow the IconBox to have all the same
components as any other primary window.
.mE
Like other windows, it can be sized, moved,
minimized, maximized, restored, and lowered.
.mS
However, the window manager must not allow the IconBox to be closed.
The system Menu action
.B Close
must be replaced with the system Menu action
.B "Pack Icons"
in an IconBox.
.mE
.B "Pack Icons"
arranges the icons as close as possible together in the visible
icon area.
.mS
.B "Pack Icons"
must have the mnemonic
.BR P .
If
.B "Pack Icons"
has an accelerator, it must be
.BR "MAlt <F12>"
if
.B <F12>
is available.
.mE
.P
The IconBox can contain an icon for each window family, even
if it is active.
.mS
The icon for minimized window families must be the same
as the icon would be outside of the IconBox.
.mE
The icon for an active
window family should be similar to the minimized icon, deemphasized
somehow, and without the icon image border.
.P
.mS
Clicking
.B BSelect
in an icon must give the icon keyboard focus and
can post the icon Menu.  The icon Menu must be the same Menu as the window
Menu for the associated primary window.
If the icon represents an active window family, 
.BR Restore ,
.BR Size ,
.BR Minimize ,
and
.BR Maximize
must not be available from the icon Menu.
If the icon represents a minimized window family,
.BR Size
and
.BR Minimize
must not be available from the icon Menu.
.P
Double-clicking
.B BSelect
anywhere in an icon that represents
a minimized window family must restore the
window family, just as the icon Menu item
.BR Restore .
Double-clicking
.B BSelect
anywhere in an icon that represents
an active window family must raise the
window family to the top of the window hierarchy.
Pressing
.B BSelect
or
.B BDrag
anywhere in the icon and dragging the mouse
pointer must move the icon within the icon area
to track the pointer, just as the icon Menu item
.BR Move
does.
.mE
.hS "Illustration"
.br
.ne 2.3i
\0
.sp 2.1i
.in +1.3i
.P! graphics/iconbox.ps
.in -1.3i
.hS "Related Information"
See Chapter 5 for more information about icons and the
window manager.
