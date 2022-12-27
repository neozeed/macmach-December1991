.\"  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  ALL RIGHTS RESERVED
.hR Menus
.hS "Description"
.iX "menus"
.iX "Menu"
.iX "PullDown"
.iX "menus" "pull down"
.iX "Popup Menus"
.iX "menus" "popup"
.iX "Options Menu"
.iX "menus" "option"
Menus should be used to organize a 
collection of basic controls in a horizontal,
vertical, or 2-dimensional layout within a
separate Menu window.  There are three
types of Menus: PullDown, Popup, and Option.
A Menu is only available while it is posted.
.hS "Illustration"
.br
.ne 3.1i
\0
.sp 2.9i
.in +.6i
.P! graphics/menuanat.ps 
.in -.6i
.sp
.hS "Navigation"
Menus follows the navigation model described in
Chapter 2.
.hS "Other Operations"
The following text describes the operations
of this component.
.VL 10n
.LI "\fBKHelp\fP:"
.mS
Must provide any available help for the Menu.
.mE
.LE
.hS "Common Bindings"
.TS
box, tab(@);
lb lb.
Virtual Key@Default Bindings
_
KHelp@<Help>
@<F1>
.TE
.hS "Related Information"
For more information,
see the reference pages for CascadeButton, OptionButton, and MenuBar.
