.\"  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  ALL RIGHTS RESERVED
.hR "Navigation"
.hS "Description"
.iX "navigation" "keyboard"
.iX "keyboard" "navigation"
Using a mouse, navigation is a simple matter of
moving the mouse pointer.
.mS
Using the keyboard,
the following navigation actions must be available.
.mE
.VL 15n
.LI "\fBKNextField\fP"
.iX "KNextField"
.mS
Must move the location cursor forward through fields.
If the next field is a field control,
.B KNextField
must move the location cursor to it.
If the next field is a group,
.B KNextField
must move the location cursor to the first traversable
component within the group, unless the field
contains a default component.
If the field contains a default component,
.B KNextField
must move the location cursor to the default component instead
of the first component.
If the next field contains no traversable components,
.B KNextField
must skip the field.
.mE
.LI "\fBKPrevField\fP"
.iX "KPrevField"
.mS
Must move the location cursor backward through fields
using the same rules as
.BR KNextField .
.mE
.LI "\fBKDown\fP"
.iX "KDown"
.mS
In a left-to-right direction language environment,
.B KDown
must move the location cursor through all traversable components
in the group, starting at the upper-left component
and ending at the lower-right component, then wrapping back
up to the upper-left component.
.mE
In a right-to-left direction language environment,
the location cursor can move, starting at the upper-right component
and moving to the lower-left component.
.LI "\fBKUp\fP"
.iX "KUp"
.mS
Must move the location cursor through all traversable components
in the opposite direction of
.BR KDown .
.mE
.LI "\fBKRight\fP"
.iX "KRight"
.mS
...\"In a left-to-right direction language environment,
...\".B KRight
...\"must
Must
move the location cursor through all traversable components
in the group, starting at the upper-left component
and ending at the lower-right component, then wrapping back
up to the upper-left component.
.mE
...\"In a right-to-left direction language environment,
...\"the location cursor can move, starting at the upper-right component
...\"and moving to the lower-left component.
.LI "\fBKLeft\fP"
.iX "KLeft"
.iX "navigation" "keyboard"
.mS
Must move the location cursor through all traversable components
in the opposite direction of
.BR KRight .
.mE
.LI "\fBKMenuBar\fP"
.iX "KMenuBAr"
.mS
If the MenuBar is inactive,
.B KMenuBar
must traverse to the MenuBar system.
The location cursor must be placed on the first CascadeButton
that accepts input
in the MenuBar.  If there are no CascadeButtons that accept
input
in the MenuBar,
.B KMenuBar
must do nothing.
If the MenuBar system is active,
.B KMenuBar
must unpost all Menus in the MenuBar system and return the
location cursor to the component that previously had it, if the
focus policy is explicit.
.mE
.LI "\fBKMenu\fP"
.iX "KMenu"
.iX "navigation" "keyboard"
.iX "keyboard" "navigation"
.mS
If the keyboard focus is in an area with an inactive Popup Menu,
.B KMenu
must pop up the Menu.
The location cursor must be placed on the first traversable
item in the Popup Menu.
.mE
If there are no traversable items
in the Popup Menu, this has no effect.
.mS
If there is an active Popup Menu,
.B KMenu
must unpost the Popup Menu system and return the location cursor
to the component that previously had it, if the focus policy is explicit.
.mE
.LI "\fBKNextFamilyWindow\fP"
.iX "KNextFamilyWindow"
.iX "windows" "navigation"
.br
.mS
Must move the focus to the next window in the window family.
.mE
.LI "\fBKPrevFamilyWindow\fP"
.iX "KPrevFamilyWindow"
.iX "windows" "navigation"
.iX "navigation" "keyboard"
.br
.mS
Must move the focus to the previous window in the window family.
.mE
.LI "\fBKNextWindow\fP"
.iX "KNextWindow"
.iX "windows" "navigation"
.mS
Must move the focus to the last window that had the input focus
in the next window family in the window hierarchy.
.mE
.LI "\fBKPrevWindow\fP"
.iX "KPrevWindow"
.iX "windows" "navigation of"
.iX "navigation" "keyboard"
.iX "keyboard" "navigation"
.mS
Must move the focus to previous last window that had the input focus
in the next window family in the window hierarchy.
.mE
.LE
.hS "Common Bindings"
.TS
box, tab(@);
lb lb.
Virtual Key@Default Bindings
_
KNextField@<Tab>
@MCtrl <Tab>
KPrevField@MShift <Tab>
@MCtrl MShift <Tab>
KDown@<\(da>
KLeft@<\(<->
KRight@<\(->>
KUp@<\(ua>
KMenuBar@<F10>
KMenu@<F4>
KNextFamilyWindow@MAlt <F6>
KPrevFamilyWindow@MShift MAlt <F6>
KNextWindow@MAlt <Tab>
KPrevWindow@MAlt MShift <Tab>
.TE
.hS "Related Information"
See Chapter 2 for more information about internal window navigation.
See Chapter 5 for more information about navigation among windows.
