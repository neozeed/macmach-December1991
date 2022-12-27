.mc |
.\"  (c) Copyright 1989, 1990 OPEN SOFTWARE FOUNDATION, INC.
.mc
.\"  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
.\"  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
.\"  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
.\"  (c) Copyright 1988 MICROSOFT CORPORATION
.\"  ALL RIGHTS RESERVED
.nr H1 3
.H 1 "Shell Widgets"
.iX "Shell" "widgets"
.iX "Widgets" "shell"
.iX "Widget" "hierarchy"
Shell widgets are used to provide communication between the widgets in an
application and the window manager.  An application's widgets are arranged
.mc |
in a hierarchy, with upper-level widgets acting as the parents of lower-level 
widgets.  Widgets at the top of the hierarchy do not have normal
.mc
parent widgets, but have a Shell as the parent.  Different Shell widgets
.mc |
are provided for the various categories of toplevel widgets, including
dialogs and menu panes.  Figure 4-1 shows the hierarchy of the Shell
widgets.  Keep in mind that Shell is a subclass of Composite (see Figure
1-4).
.br
.ne 15 
.sp
.FG "Shell Widget Hierarchy"
.if n .ig +n
.mc
.DS
.ps 8
.in +1i
.PS 4.0 3.5
.mc |
.vs 10
.mc
boxwid = 0.75
down
box wid 1.25 "Shell" "(Private)"
.mc |
line down .5
.mc
move up .25
line left 2
line down .25
box same "OverrideShell" "(Public)"
line down 3.5
box same "MenuShell" "(Public)"
.mc |
move up 4.5
.mc
move right 2
.mc |
move down .25
.mc
box same "WMShell" "(Private)"
.mc |
line down .5
.mc
box same "VendorShell" "(Private)"
line down .25
line right 2
line down .25
box same "TransientShell" "(Public)"
line down 1.5
box same "DialogShell" "(Semi-Public)"
.mc |
move up 2.75
.mc
move left 2
line down .5
box same "TopLevelShell" "(Public)"
.mc |
line down .5
.mc
box same "ApplicationShell" "(Public)"
.mc |
.vs
.mc
.PE
\s10
.in -1i
.mc *
.mc
.DE
.mc |
.+n
.mc
.iX "Shell" "class"
.iX "WMShell"
.iX "VendorShell"
The classes Shell, WmShell, and VendorShell are private and should not
be instantiated.  The other Shell classes are for public use,
although a DialogShell is normally created by a convenience function
as part of a set rather than by itself.  Each of
.mc |
the Shell classes has a manpage (found in the
\fIOSF/Motif Programmer's Reference\fP) that has information on
.mc
the resources belonging to the specific Shell widget.
.H 2 "Descriptions of Shell Widgets"
.iX "Shell" "private"
.iX "Shell" "public"
.iX "Public shell"
.iX "Private Shell"
.mc |
The Shell widgets shown in Figure 4-1 are of two types: Private and Public.
.mc
This means that the Public widgets are those that you should instantiate, 
either individually or, as in the case of DialogShell, as part of a set.
.mc |
The Private widgets are those that you should not instantiate.
.mc
These widgets typically just supply resources to Shell widgets that are lower
.mc |
in the hierarchy.  Keep in mind the hierarchy diagram of Figure 4-1 as you
.mc
study the definitions of the Shell widgets.
.P
.iX "Shell"
.iX "OverrideShell"
.iX "WMShell"
.iX "VendorShell"
.iX "TopLevelShell"
.iX "ApplicationShell"
.mc |
.iX "XtAppInitialize"
.mc
.iX "XtAppCreateShell"
.iX "MenuShell"
.iX "DialogShell"
.iX "XmCreateErrorDialog"
.BL
.LI
.mc |
\fBShell\fP - Shell is the base class for all shell widgets.  It
.mc
provides the fields needed by all the Shell widgets.  Shell is a 
.mc |
subclass of Composite (see Figure 1-2).
.mc
.LI
\fBOverrideShell\fP - OverrideShell is used for shell windows that bypass
the window manager.  Popup menus are one example of where an OverrideShell
might be used.
.LI
\fBWMShell\fP - WMShell contains fields that are needed by the common
window manager protocol.
.LI
\fBVendorShell\fP - VendorShell contains fields that are used by
vendor-specific window managers.
.LI
.mc |
\fBTopLevelShell\fP - TopLevelShell is used for normal toplevel windows.
It is not the root shell used by an application, rather it is
normally used to create peer toplevel windows in situations
.mc
where an application needs more than one set of windows.  The root
shell is normally the ApplicationShell.
.LI
.mc |
\fBApplicationShell\fP - ApplicationShell is an application's toplevel or
root window.  This is the shell that is created by \fBXtAppInitialize\fP.
.mc
An application should not have more than one ApplicationShell.  Subsequent
.mc |
toplevel shells should be of class TopLevelShell and are created by 
.mc
\fBXtAppCreateShell\fP.  These top-level shells can be considered the root
of a second widget tree for the application.
.LI
.mc |
\fBMenuShell\fP - MenuShell is used as the parent of pop-up and pull-down
menu panes.  It is a subclass of OverrideShell.
.mc
.LI
\fBDialogShell\fP - DialogShell is the parent of Dialogs.  Although it can
be instantiated by itself, it is normally instantiated as part of a set by
one of the convenience dialogs.  For example, \fBXmCreateErrorDialog\fP
.mc |
creates a DialogShell and a message box as its child.  See Chapter 5,
.mc
``Dialog Widgets and Functions,'' for more information.
.LE
.H 2 "Shell Widget Appearance"
.iX "Shell" "appearance"
.mc |
Most Shell widgets are invisible.  However, the type of Shell class can
.mc
have an impact on how its children are displayed.  For example, children
of a TransientShell (typically Dialogs) by default have no buttons on
the window manager frame that surrounds the window.  Also, as long as the
transient window is visible, it will remain above the window from which
it is transient.
