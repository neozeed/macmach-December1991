.\"  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  ALL RIGHTS RESERVED
.hR "Icon Menu"
.hS "Description"
.iX "icon Menu"
.iX "menus" "icon"
.mS
Clicking
.B BSelect
in an icon must give the icon keyboard focus and
post the icon Menu.  The icon Menu must be the same Menu as the window
Menu for the associated primary window.  
.B Size
and
.B Minimize
must not be available from the icon Menu.
.mE
.P
.mS
Double-clicking
.B BSelect
anywhere in the icon must restore the
window family, just as the icon Menu item
.BR Restore
does.
Pressing
.B BSelect
anywhere in the icon and dragging the mouse
pointer must move the icon to track the pointer, just as the icon
Menu item
.BR Move
does.
.mE
.hS "Related Information"
See Chapter 5 for more information on the icon Menu and
the window manager in general.  See Chapter 4 for
information about Menu design.
