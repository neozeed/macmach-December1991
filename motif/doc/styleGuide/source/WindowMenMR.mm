.\"  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  ALL RIGHTS RESERVED
.hR "Window Menu"
.hS "Description"
.iX "menus" "window"
.iX "menus" "system"
.iX "window menus"
.iX "system menus"
The window Menu, sometimes called the system Menu,
is used to display the list of window actions.  All
actions possible for a window should be displayed in the window
Menu, since keyboard-only users interact with the window
manager through this Menu.
.P
.mS
The window Menu button must be located in the upper-left corner
of the window.
Double-clicking the window Menu button must close the window.
Pressing
.BR KWindowMenu
must activate the window Menu button for the window
with the focus.
.mE
The illustration on this reference page
shows a posted window Menu.
.P
.mS
A primary window Menu must have the following selections
in the order listed:
.VL 2i
.LI "\fB\o'R_'estore\ \ \ \ Alt+F5\fP"
.iX "Restore"
Restores a minimized or maximized window to the previous
size and location of the window.
This selection must be deemphasized (grayed out) when
the window is in its normal state. 
This action must have the mnemonic
.BR R .
If this action has an accelerator, it must be
.BR "MAlt <F5>"
if
.B <F5>
is available.
.LI "\fB\o'M_'ove\ \ \ \ Alt+F7\fP"
.iX "Move"
Moves a window around the workspace.
This action must have the mnemonic
.BR M .
If this action has an accelerator, it must be
.BR "MAlt <F7>"
if
.B <F7>
is available.
.LI "\fB\o'S_'ize\ \ \ \ Alt+F8\fP"
.iX "Size"
Changes the height and width of the window in the direction
indicated by the pointer.
This action must have the mnemonic
.BR S .
If this action has an accelerator, it must be
.BR "MAlt <F8>"
if
.B <F8>
is available.
.LI "\fBMi\o'n_'imize\ \ \ \ Alt+F9\fP"
.iX "Minimize"
Changes a window into an icon.
This action must have the mnemonic
.BR N .
If this action has an accelerator, it must be
.BR "MAlt <F9>"
if
.B <F9>
is available.
.LI "\fBMa\o'x_'imize\ \ \ \ Alt+F10\fP"
.iX "Maximize"
Enlarges a window to its maximum size.
This action must have the mnemonic
.BR X .
If this action has an accelerator, it must be
.BR "MAlt <F10>"
if
.B <F10>
is available.
.LI "\fB\o'L_'ower\ \ \ \ Alt+F3\fP"
.iX "Lower"
.iX "menus" "window"
.iX "menus" "system"
.iX "window menus"
.iX "system menus"
Moves a window to the bottom of the window hierarchy.
This action can be omitted.
This action must have the mnemonic
.BR L .
If this action has an accelerator, it must be
.BR "MAlt <F3>"
if
.B <F3>
is available.
.LI "\fB\o'C_'lose\ \ \ \ Alt+F4\fP"
.iX "Close"
Closes a window and removes it from the workspace.
This action must have the mnemonic
.BR C .
If this action has an accelerator, it must be
.BR "MAlt <F4>"
if
.B <F4>
is available.
.LE
.mE
.P
.mS
A secondary window Menu must have the following selections
in the order listed:
.BR Restore ,
.BR Move ,
.BR Size ,
and
.BR Close .
.mE
.br
.ne 3i
.hS "Illustration"
.br
.ne 2.7i
\0
.sp 2.5i
.in +1.0i
.P! graphics/osfmenu.ps 
.in -1.0i
.hS "Common Bindings"
.TS
box, tab(@);
lb lb.
Virtual Key@Default Binding
_
KWindowMenu@MShift KEscape
@MAlt KSpace
.TE
.hS "Related Information"
See Chapter 5 for more information on the window Menu and
the window manager in general.  See Chapter 4 for
information about Menu design.
