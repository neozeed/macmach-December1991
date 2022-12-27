.\"  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  ALL RIGHTS RESERVED
.hR MenuBar
.hS "Description"
.iX "MenuBar"
.iX "pull-down menu"
A MenuBar should be used to organize
the most common features of an application.
.mS
The MenuBar must be a horizontal bar at the
top edge of the application just below the title
area of the window
frame.
.mE
The MenuBar contains a list of Menu topics
in CascadeButtons with PullDown Menus connected to
them.
.mS
Commands must not be included as topics
in the MenuBar because they inhibit Menu browsing.
.mE
Each Menu topic should have a single letter
mnemonic indicated by an underline.
.P
The following Menus pulled down from the MenuBar
provide general functions common to most applications.
.VL 10n
.LI "\fBFile\fP"
.iX "File"
.iX "file menu"
.iX "menus" "file"
The
.B File
Menu should contain components for performing actions
on the files, such as opening, saving, closing, and printing.
It should also contain components for performing actions
on the application as a whole, such as quitting.
.mS
If your application includes a
.B File
Menu, it must be the first entry in the MenuBar, and placed
to the far left.
.mE
The
.B File
Menu should have a mnemonic of
.BR F .
.LI "\fBEdit\fP"
.iX "Edit"
.iX "edit menu"
.iX "menus" "edit"
The
.B Edit
Menu should contain components for performing actions
on the current data of the application, such as an
undo action or making global changes to text.
It should also include components for interacting
with the system clipboard, such as cut, copy, and paste.
.mS
If your application includes an
.B Edit
Menu, it must be placed to the right of the
.B File
Menu.
.mE
The
.B Edit
Menu should have a mnemonic of
.BR E .
.LI "\fBView\fP"
.iX "View"
.iX "view menu"
.iX "menus" "view"
The
.B View
Menu should contain components for changing the
users view on the data.  Components in the
.B View
Menu should not actually change the data.
The exact contents of the
.B View
Menu are application specific, but it
may include components that change the appearance
of the data, the amount of data that is displayed, or
the order in which the data is displayed.
.mS
If your application includes a
.B View
Menu, it must be placed to the right of the
.B Edit
Menu.
.mE
The
.B View
Menu should have a mnemonic of
.BR V .
.LI "\fBOptions\fP"
.iX "Options Menu"
.iX "view menu"
.iX "menus" "view"
The
.B Options
Menu should contain components for a user to customize
the application.  The exact contents of the
.B Options
Menu are application specific.
.mS
If your application includes an
.B Options
Menu, it must be placed to the right of the
.B View
Menu.
.mE
The
.B Options
Menu should have a mnemonic of
.BR O .
.LI "\fBHelp\fP"
.iX "Help"
.iX "help" "menu"
.iX "menus" "help"
The
.B Help
Menu should contain components that provide user help
facilities.
The components in the
.B Help
Menu usually bring up a DialogBox with help information.
Every application should have a
.B Help
Menu.
.mS
If your application includes a
.B Help
Menu, it must be the last Menu item, and placed to the
far right on the MenuBar.
.mE
The
.B Help
Menu should have a mnemonic of
.BR H .
.LE
.P
While we recommended that you include the common Menus
in the MenuBar of your application,
your choice of Menu titles and items depends on the
nature of your application.
If your application requires it, you should
design more relevant titles and selections,
but do not change the meanings of words
used in the common Menus.
.P
The recommended contents of the
.BR File ,
.BR Edit ,
and
.BR Help
Menus are described on their own reference pages.
The contents of the
.BR View
and
.BR Options
Menus are application specific and are not specified here.
.br
.ne 2i
.hS "Illustration"
.br
.ne 1.2i
\0
.sp 1.0i
.in +.5i
.P! graphics/menubar.ps 
.in -.5i
.sp
.hS "Navigation"
The MenuBar follows the navigation model described in
Chapter 2.
.hS "Other Operations"
The following text describes the operations
of this component.
.VL 10n
.LI "\fBKHelp\fP:"
.mS
Must provide any available help for the current CascadeButton.
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
For information about the suggested contents of these Menus,
see the reference pages for
File Menu,
Edit Menu,
and
Help Menu.
