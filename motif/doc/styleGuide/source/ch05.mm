.\" (c) Copyright 1989, 1990, OPEN SOFTWARE FOUNDATION, INC.
.\" (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
.\" (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
.\" (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
.\" (c) Copyright 1988 MICROSOFT CORPORATION
.\"  ALL RIGHTS RESERVED
.nr H1 4
.H 1 "Window Manager Design Principles"
.iX "design" "window manager"
.iX "window manager" "design"
A window manager is a specialized application.
.mS
In designing
a window manager you must follow the same principles as
for any other application.
A window manager must also follow the style guidelines for
input, navigation, and selection models as set forth in Chapters 2 and 3.
.mE
.P
There are a few elements of user interface
design that are specific to window managers.
This chapter discusses those elements.
.BL
.LI
Configurability
.LI
Window support
.LI
Window decorations
.LI
Window placement
.LI
Window navigation
.LI
Icons
.LE
.H 2 "Configurability"
.iX "configurability"
.iX "hot key"
This chapter describes only those elements of window manager design
that help create a consistent user interface.
A window manager can supply a mechanism that allows the user to configure
the window manager.  The window manager can make any element
user configurable,
including key bindings, Menu contents,
default window decorations, or any other rules and element
defined in this guide.
.mS
Any window manager that supplies a configuration mechanism must
also include support for toggling between the current user
configuration and the default configuration as mandated by this guide.
The configuration toggle key press,
.BR "MAlt MShift MCtrl !" ,
must initiate a configuration toggle.
.mE
Any request for toggling the configuration should request
user verification before proceeding.
.H 2 "Window Support"
.iX "windows" "types"
.iX "types of windows"
Users communicate with applications using windows.
A window is an area of the screen (usually rectangular)
that provides the user with the functional means to communicate with an
application and through which an application can communicate with them.
.P
.iX "primary windows"
.iX "main windows"
.iX "windows" "primary"
.iX "windows" "main"
.iX "windows" "secondary"
.iX "secondary windows"
.iX "transient windows"
.iX "windows" "transient"
.iX "DialogBox"
A typical environment has several applications in operation simultaneously.
Each application typically has a main or primary window that displays
data and in which the user carries on
primary interaction with the application.
Applications can have additional windows
to communicate context-specific interactions with the user of the application.
These additional windows are called secondary windows, or transient windows.
DialogBoxes are often used to create secondary windows.
Figure 5-1 illustrates a typical OSF/Motif environment.
.br
.ne 4.6i
\0
.FG "A Typical OSF/Motif User Environment"
.sp 4.4i
.in +.9i
.P! graphics/rootpix.ps 
.in -.9i
.P
While each application can be made up of many windows,
each window is one of only three basic types:
.P
.BL
.LI
.iX "primary windows"
.iX "windows" "primary"
A primary window, the main application windows.
.LI
.iX "secondary windows"
.iX "windows" "secondary"
.iX "transient windows"
.iX "windows" "transient"
A secondary window, a window that provides secondary and
transient interaction with the user.
.LI
.iX "menus" "window"
.iX "windows" "menu"
A menu window, a temporary window for displaying a menu of choices
for the user.
.LE
.P
.mS
A window manager must support multiple applications, each with
one or more primary windows.  The window manager must also recognize
and support secondary windows.  Also, it must associate each secondary
window with a primary window or another secondary window.
.mE
.H 3 "Primary Window"
.iX "primary windows"
.iX "windows" "primary"
.iX "main windows"
.iX "windows" "main"
A primary window is the window from which all the other windows used by
an application are generated.
.mS
The window manager must support one or more primary windows for each
windowing application.
.mE
When an application has multiple primary windows, the window manager
should treat each primary window as if it were an independent application.
.P
.iX "iconized window"
.iX "minmized window"
.iX "windows" "iconized"
.iX "windows" "minmized"
.iX "windows" "secondary"
.iX "secondary windows"
.mS
When a primary window is iconized (minimized), the window and
all of its associated secondary windows must be
removed from the display and replaced with a single icon representing
the primary window.  Iconizing a window must not automatically
suspend any processes of the window.
.mE
However, a client can notice that a primary window has been unmaped
and adjust its processing accordingly.
.mS
When the icon of the window is opened, the primary
window and all the current associated secondary windows must be
restored.
.mE
The window manager should try to place the windows
in the same position from which they were iconified.
.P
.iX "icons"
.mS
When a primary window is closed, the window and its icon
must be removed from the display.
All secondary windows associated with the primary window
must also be closed.  If the last primary window of an application is
closed, the application must also be closed.
.mE
.H 3 "Secondary Windows (Dialog)"
.iX "secondary windows"
.iX "windows" "secondary"
.iX "parent windows"
.iX "windows" "parent"
Applications use secondary windows to conduct context-specific dialog
with the user.  Such context-specific dialogs are usually transitory, and
the secondary windows can be removed when they are no longer needed.
.mS
When a secondary window is closed, its parent must not be affected, and
any secondary windows that are children of it must also be closed.
.mE
.P
Secondary windows are always related to a parent window.  Sometimes the
parent is a primary window, sometimes another secondary window.  Any
window can have any number of secondary window children.
.P
.mS
Secondary windows are not constrained to be clipped within
their parent window, but they must always appear on top of
that parent window in the window hierarchy.
.mE
In a layered
window manager, you can think of a primary window and its
associated secondary windows as occupying one layer in the
window hierarchy.
.mS
Whenever one window is moved in the hierarchy,
all of the associated windows must move accordingly.  Thus, lowering
a window must move that window and all associated windows to the
bottom of the window hierarchy:
Giving a window the focus
must raise that window and all of the associated windows to the top of
the hierarchy.
.mE
.P
Secondary windows may limit how a user can interact with windows
in order to force the order of interaction.
.mS
A window manager
must support the following four types, known as modes,
of interaction with secondary windows.
.VL 22n
.LI "\fBModeless\fP"
.iX "windows" "modes"
.iX "secondary windows" "modes"
.iX "secondary windows" "interaction"
.iX "modeless"
.iX "primary modal"
.iX "application modal"
.iX "system modal"
.iX "secondary windows" "modeless"
.iX "secondary windows" "primary modal"
.iX "secondary windows" "application modal"
.iX "secondary windows" "system modal"
Allows interaction with the secondary window and all other
windows.
.LI "\fBPrimary modal\fP"
Does not allow interaction with any ancestor of the window.
.LI "\fBApplication modal\fP"
Does not allow interaction with any window created by
the same application even if the application has multiple
primary windows.
.LI "\fBSystem modal\fP"
Does not allow interaction with any window on the screen.
This includes windows from all other applications and any
icon box.  To indicate a system modal secondary window,
the pointer should change shape to a caution pointer
whenever it leaves the system modal secondary window. 
.LE
.mE
.P
.mS
A window manager must not allow
focus to be given to any window that is not allowed to
accept input because of the modality of a DialogBox.
.mE
.H 3 "Menu Windows"
.iX "menus" "window"
.iX "Tear-Off Menus"
Menu windows are used to present Menus.  They are not
specifically created by applications, but by the components
used to create the application.  Menu windows are always
related to a parent window.  The parent can be either a primary
window, secondary window, or another menu window.  A window
can only have one menu window child at a time.
.P
A menu window is very short lived.  It is only available to
a client while no mouse or button actions are being performed
elsewhere on the screen.
.mS
Once interaction starts in another
window on the screen, except a child menu window, the
menu window must be removed.
.mE
Some systems have menu
windows, called Tear-off Menus, that can be made to remain on
the screen during other interactions.
.mS
Once these menus are
made permanent, they must become secondary windows.
.mE
.P
Menu windows are not constrained to be clipped within
their parent window, but
.mS
they must always appear on top of
that parent window in the window hierarchy.
he window manager
must not supply any window decoration to menu windows.
.mE
.H 2 "Window Decorations"
.iX "decoration" "windows"
.iX "windows" "decoration"
.iX "windows" "components"
.iX "parts" "window"
.iX "components" "window"
.iX "window manager" "functions"
.P
A window manager can provide windows with a window frame that
contains components, often called decorations.  The window decorations
allow user interaction with the window manager.  Along with the
frame components, a window manager contains a client area.  The
client area is the display area for an application.
Figure 5-2 shows a typical window and its decorations.
.br
.ne 4.45i
\0
.FG "A Typical OSF/Motif Window Layout"
.sp 4.25i
.in +.25i
.P! graphics/wmcalls2.ps 
.in -.25i
.P
A window manager can support any number of window decorations,
.mS
but must support the client area and the window frame.
A window manager must support the following window frame components:
.BL
.LI
.iX "title area"
Title area
.LI
.iX "maximize button"
Maximize button
.LI
Minimize button
.iX "minimize button"
.LI
Resize border
.iX "resize border"
.LI
Window Menu button
.iX "window Menu button"
.LE
.mE
.P
.mS
Each window must have a client area.  A window manager must allow
each window to choose which combination of decorations to include,
except that a window must not have any buttons without a title area.
A primary window must by default have all the decorations: window
Menu button,
maximize button, minimize button, title area and
resize border.  A secondary window must by default have a window
Menu button, and title area.  In a secondary window, resize borders
and a maximize button can be provided if there is a reason to
resize the window.
.mE
.P
Clicking
.B BSelect
on the window frame should
give the window focus and raise the window to the top of the window
hierarchy, except when dialog modality disallows focus to be set in
the window.
.H 3 "Client Area"
.iX "client areas"
The client area is the portion of the window in which the user performs
most application-level tasks.  For example, if the user is working with a
graphics editor or a text editor, the client area contains the figure or
document being edited. The client area is inside the window frame and can
be composed of multiple work areas.
Figure 5-2 shows the client area in a typical window.
.H 3 "Title Area"
.iX "title area"
.iX "title bar"
.iX "application title"
.iX "multiple windows"
.iX "windows" "multiple"
The title area, also called the title bar,
supplies a place to identify the window.
.mS
If a window includes a title area,
it must be a horizontal bar at the top of the window.
It must be just above the client area and lie horizontally
between the window Menu (or other buttons) on the left and the window control
buttons on the right.
.mE
Figure 5-2 shows the title area in a typical window.
.P
.iX "multiple windows"
.iX "windows" "multiple"

The title area should contain a short string called the title that
labels the contents of the window.
.mS
The title must be settable at window startup
both by the user and by the application.
.mE
For applications with a single primary window, the title
should be the name of the application.
For applications with multiple primary windows, the title
should indicate the purpose of the window.
The title in secondary windows should indicate the purpose of
the window.  The title can also contain other useful information,
such as the machine on which the application is running, the
current directory, or similar relatively static information.
.P
Besides supplying a location for a label, the title area also acts as
a position, or handle, for moving a window.
.mS
Pressing
.B BSelect
or
.B BDrag
in the title area and dragging the pointer 
must move the window relative to the moving pointer.
.mE
.H 3 "Maximize Button"
.iX "maximize button"
.iX "windows" "frame components"
.iX "buttons"
.iX "restore"
The maximize button provides mouse users a shortcut
to the
.B maximize
entry in the window Menu.
.mS
If a window
includes a maximize button,
it must be just above the client area and its right border
must align with the right border of the client area.
.mE
Figure 5-2 shows the maximize button in a typical window.
Primary windows should have a maximize button.  Secondary windows
generally do not have a maximize button.
.P
The graphic in the maximize button should be a large square or an
up arrow.
If the maximize button uses an up arrow as its graphic,
a maximized window should use a double-headed arrow to indicate
that it is maximized.
If the maximized button uses a large square as its graphic,
a maximized window can show the graphic in a different state.
.mS
Activating the maximize button must increase the size of
the window to the maximum allowable size.
As a shortcut for mouse users to the
.B Restore
entry in the
window Menu,
activating the
maximize
button of a maximized window must
restore the window to its size and location before being maximized.
.mE
.H 3 "Minimize Button"
.iX "minimize button"
.iX "windows" "frame components"
.iX "buttons"
The minimize button provides mouse users a shortcut
to the
.B Minimize
entry in the window Menu.
.mS
If a window
includes a minimize button,
it must be just above the client area and directly to the left
of the maximize button.
.mE
Figure 5-2 shows the minimize button in a typical window.
Primary windows should have a minimize button.
.mS
Secondary windows
must not have a minimize button.
.mE
.P
The graphic in the minimize button should be a small square or a
down arrow.
.mS
Activating the minimize button must iconify
the window unless the window family contains a system modal
DialogBox.
.mE
.H 3 "Other Buttons"
.iX "window control buttons"
.iX "window manager" "functions"
You can bind additional window manager functions to buttons
on the window frame.
.mS
Any additional buttons must be placed
directly to the left of the minimize button or directly to
the right of the window Menu button and above the
client area.  Each button action must correspond to a
entry in the window Menu.
.mE
.H 3 "Resize Borders"
.iX "resize border"
.iX "windows" "resize border"
Applications may suggest the initial size of their windows to the window
manager.  Window sizes can vary according to the work performed in them.
At any time, a user should be able to alter the size of most windows.
The
.B Size
entry in the window Menu provides a method for the user
to alter the size of windows.
The resize borders provide a shortcut for mouse users for the 
.B Size
entry in the window Menu.
Resize borders are not generally provided on secondary windows.
.P
.iX "corner handles"
.iX "edge handles"
.iX "handles"
The resize borders are the outermost components of the window manager frame.  
They are made up of two components: the corner handles and the edge handles.
.mS
If a window includes resize borders, there must be one corner handle in each
corner of the window at its extremes, and one edge handle between each pair
of corner handles.  There must be no window components outside the boundary
formed by the resize borders.
.mE
Figure 5-2 shows resize borders in a typical window.
.P
.mS
Pressing
.B BSelect
or
.B BDrag
in a corner handle and dragging the pointer 
must change the height and width of the window relative to the
moving pointer without changing the position of the opposite corner.
Pressing
.B BSelect
or
.B BDrag
in a top or bottom edge handle and dragging the pointer 
must change the height of the window relative to the
moving pointer without changing the width or the position of the opposite edge.
Pressing
.B BSelect
or
.B BDrag
in a side edge handle and dragging the pointer 
must change the width of the window relative to the
moving pointer without changing the height or the position of the opposite edge.
.mE
.H 3 "Window Menu"
.iX "window Menu button"
.iX "system Menu button"
.iX "menus" "window"
.iX "menus"
.iX "buttons" "window Menu"
.iX "buttons" "system Menu"
The window Menu, sometimes called the system Menu or control Menu,
is used to display the list of window actions.  All
actions possible for a window should be displayed in the window
Menu because keyboard-only users interact with the window
manager through this Menu.  Because of this, it is rare that
a window does not need a window Menu.
The window Menu can be configured out by the
application or by the user, but should not be removed by the window
manager.
.P
.mS
If a window includes a window Menu,
the window Menu button must be located just above the client area,
the left edge of the button must align with the left edge of the client area,
and the button must be just to the left of the title area, unless
other buttons are included between the window Menu button and the
title area. 
.mE
.P
Double-clicking the window Menu button can be used to close the window,
unless focus is disallowed in the window.
.mS
A user must be able to activate the window Menu button for the
window with the focus by using
.BR KWindowMenu .
.mE
.B KWindowMenu
is usually bound to
.BR "MShift KEscape"
or
.BR "MAlt KSpace"
as shown in the following table.
.P
.TB "KWindowMenu Bindings"
.TS
center, box, tab(@);
lb lb.
Virtual Key@Default Binding
_
KWindowMenu@MShift <Escape>
@MAlt KSelect
KSelect@<Select>
@KSpace
@MCtrl KSpace
KSpace@<Space>
.TE
.P
Figure 5-3 shows a typical window Menu.
.br
.ne 3.0i
\0
.FG "The Window Menu Button with Menu Pulled Down"
.sp 2.8i
.in +1.0i
.P! graphics/osfmenu.ps 
.in -1.0i
.P
.mS
A primary window Menu must have the following entries
in the order listed:
.VL 2i
.LI "\fB\o'R_'estore        Alt+F5\fP"
.iX "Restore"
Restores a minimized or maximized window to the previous
size and location of the window.
This entry must be deemphasized (grayed out) when
the window is in its normal state. 
This action must have the mnemonic
.BR R .
If this action has an accelerator, it must be
.BR "MAlt <F5>"
if
.B <F5>
is available.
.LI "\fB\o'M_'ove            Alt+F7\fP"
.iX "Move"
Moves a window around the workspace.
This action must have the mnemonic
.BR M .
If this action has an accelerator, it must be
.BR "MAlt <F7>"
if
.B <F7>
is available.
.LI "\fB\o'S_'ize             Alt+F8\fP"
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
.LI "\fBMi\o'n_'imize     Alt+F9\fP"
.iX "Minimize"
Changes a window into an icon.
This action must have the mnemonic
.BR N .
If this action has an accelerator, it must be
.BR "MAlt <F9>"
if
.B <F9>
is available.
.LI "\fBMa\o'x_'imize   Alt+F10\fP"
.iX "Maximize"
Enlarges a window to its maximum size.
This action must have the mnemonic
.BR X .
If this action has an accelerator, it must be
.BR "MAlt <F10>"
if
.B <F10>
is available.
.LI "\fB\o'L_'ower         Alt+F3\fP"
.iX "Lower"
Moves a window to the bottom of the window hierarchy.
This action can be omitted.
This action must have the mnemonic
.BR L .
If this action has an accelerator, it must be
.BR "MAlt <F3>"
if
.B <F3>
is available.
.LI "\fB\o'C_'lose         Alt+F4\fP"
.iX "Close"
Closes a window and removes it from the workspace.
This action must have the mnemonic
.BR C .
If this action has an accelerator, it must be
.BR "MAlt <F4>"
if
.B <F4>
is available.
Applications should prompt the user to save any unsaved changes
when a window is closed.
.LE
.mE
.P
.mS
A secondary window Menu must have the following entries
in the order listed:
.BR Restore ,
.BR Move ,
.BR Size ,
and
.BR Close .
.mE
A secondary window Menu can also include
.B Lower
between
.B Size
and
.BR Close ,
but the lower option on a secondary window lowers all
the windows secondary to that window's primary window.
.mS
A secondary window must not include an entry for
.BR Minimize .
.mE
.P
Additional Menu items can be added to the window Menus of
both primary and secondary windows according to the guidelines
for Menus described in Chapter 4.
.H 2 "Window Navigation"
.iX "windows" "navigation"
.iX "navigation" "window"
Moving the focus among windows using the keyboard is
called window navigation.  Because applications should not warp
the mouse pointer, a window manager need not support
window navigation when the focus policy is in pointer mode.
.mS
Window managers must support window navigation when the
focus policy is in explicit mode.
.mE
.P
The window navigation model can be divided into two levels:
.AL
.LI
Moving among window families (among primary windows)
.LI
Moving within a window family (among secondary windows)
.LE
.P
.iX "KNextFamilyWindow"
.iX "KPrevFammilyWindow"
A window family consists of a single primary window and
all of its associated secondary windows.
.mS
A window manager
must support moving the keyboard focus among windows
in a window family using the
.BR KNextFamilyWindow
key.
.mE
.B KNextFamilyWindow
is usually bound to
.B "MAlt <F6>"
if
.B <F6>
is available.
A window manager may support moving the keyboard focus
in the opposite direction from
.B KNextFamilyWindow
using the
.BR KPrevFamilyWindow
key.
.B KPrevFamilyWindow
is usually bound to
.BR "MShift MAlt <F6>"
if
.B <F6>
is available.
.P
.iX "KPrevWindow"
.iX "KNextWindow"
.mS
Moving the focus between window families must be done
using
.B KNextWindow
and
.BR KPrevWindow .
.mE
.B KPrevWindow
should move the focus among the windows
in the opposite direction from
.BR KNextWindow .
.B KNextWindow
is usually bound to
.BR "MAlt KTab" .
.B KPrevWindow
is usually bound to
.BR "MAlt KBackTab" .
If there is no icon box,
.mS
.BR KNextWindow
and
.BR KPrevWindow
must move among the icons as well as the windows.
.mE
When focus moves into a
window family, the focus should go to the window in the window
family that last had the focus.  If no window in the family
has ever had the focus, it should go to the most recently opened
secondary window. 
The following table shows the common window navigation bindings.
.P
.TB "Window Navigation Bindings"
.TS
center, box, tab(@);
lb lb.
Virtual Key@Default Binding
_
KNextPane@<F6>
KNextFamilyWindow@MAlt KNextPane
KPrevFamilyWindow@MAlt MShift KNextPane
KNextWindow@MAlt KTab
KPrevWindow@MAlt KBackTab
.TE
.H 2 "Icons"
.iX "icons"
An icon is a stylized representation of an object.  A window
icon is a minimized representation of a window or window
family that can help organize windows and tasks in the display.
Iconifying a window is also known as minimizing a window.
.mS
The window manager must iconify all windows of window family
together.  It must not iconify any single window, primary or
secondary, from a window family without also iconifying all the
other windows in the window family.
.mE
The iconic representation of a window family
should not change any state in the windows except the visual
representation of the window.
.mS
The application running inside of a window must
continue running even when the application is iconified. 
.mE
The application can adjust its own state when it is iconified.
.H 3 "Icon Decoration"
An icon is made up of an image and a label.  Figure 5-4
shows a typical Motif icon.
.br
.ne 1.5i
\0
.FG "A Typical OSF/Motif Icon"
.sp 1.3i
.in +2.6i
.P! graphics/TermIcon.ps 
.in -2.6i
.P
.mS
The image must be
surrounded by a border that indicates when the icon has the
keyboard focus.
.mE
The image area can contain text or a bitmap.
.mS
The label must be located just below the image and its border.
.mE
The label can also indicate when the icon has the keyboard focus
by highlighting along with the image area.
.P
The label should contain the same text as the title area of the
corresponding primary window, or an abbreviated form of it.
.mS
When the icon does not have the keyboard focus, the width of the
label must be the same as the width of the image area and its
border, truncating text if necessary.
.mE
When the icon has the keyboard focus, the width of the label can
expand to display the entire text.  
.H 3 "Icon Menu"
.iX "icon Menu"
.iX "menus" "icon"
.mS
Clicking
.B BSelect
in an icon must give the icon the keyboard focus and
post the icon Menu.  The icon Menu must be the same Menu as the window
Menu for the associated primary window.  
.B Size
and
.B Minimize
must not be available from the icon Menu.
Navigating to the icon must also give the icon the keyboard focus and
post the icon Menu.
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
or
.B BDrag
anywhere in the icon and dragging the mouse
pointer must move the icon to track the pointer, just as the icon
Menu item
.BR Move
does.
.mE
.P
.mS
If the Window Manager has been customized by the user so that
the icon Menu does not popup when the icon gets the focus,
.BR KWindowMenu ,
.BR KMenu ,
and
.BR BMenu
must popup the icon Menu.
.mE
.H 3 "Icon Box"
.iX "icon box"
.iX "box" "icon"
An icon box is a specialized window that acts as a storage location
for icons.
An icon box acts like a typical window in the sense that it has a window
frame and frame components.
.mS
The client area of an icon box must have an area for holding
icons and horizontal and vertical scroll bars for moving around the
icon area.
.mE
Figure 5-5 shows a typical icon box.
.br
.ne 2.5i
\0
.FG "A Typical OSF/Motif Icon Box"
.sp 2.3i
.in +1.3i
.P! graphics/iconbox.ps
.in -1.3i
.P
.mS
The icon box must have all the same
components as any other primary window.
.mE
Like other windows it can be sized, moved,
minimized, maximized, restored, and lowered.
.mS
However, the window manager must not allow the icon box to be closed.
The system Menu action
.B Close
must be replaced with the system Menu action
.B "Pack Icons"
in an icon box.  Double-clicking
.B BSelect
in the icon box system
Menu must only open the icon box system Menu.  It must not close
the icon box.
.mE
.B "Pack Icons"
arranges the icons as close as possible together in the visible
icon area if possible.
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
When the input focus is in the icon box,
.B KWindowMenu
must pop up the icon box system Menu if the Menu is unposted.
If the Menu is already posted,
.B KWindowMenu
must unpost the Menu.
.mE
.P
.iX "Pack Icons"
The icon box can contain an icon for each window family, even
if it is active.
.mS
The icon for minimized window families
must be the same as the icon would be outside of the icon box.
.mE
The icon for an active
window family should be similar to the minimized icon, deemphasized
somehow.
.P
.mS
Clicking
.B BSelect
in an icon must give the icon keyboard focus and
post the icon Menu.  The icon Menu must be the same Menu as the window
Menu for the associated primary window.
If the icon represents an active window family, 
.BR Restore
and
.BR Size
must not be available from the icon Menu.
If the icon represents a minimized window family,
.BR Size
and
.BR Minimize
must not be available from the icon Menu.
If the window manager is customized so that the icon with the focus
does not have its window Menu automatically posted,
.B BMenu
or
.B KMenu
must post its Menu.
.mE
.P
.mS
Double-clicking
.B BSelect
anywhere in an icon that represents
a minimized window family must restore the
window family, just as the icon Menu item
.BR Restore
does.
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
.P
.mS
.BR KDown ,
.BR KUp ,
.BR KRight
and
.BR KLeft ,
must navigate among the icons in the icon box.
.mE
.BR KDown ,
.BR KUp ,
.BR KRight
and
.BR KLeft ,
are usually bound to the arrow keys, as shown in the table
at the end of this section.
.mS
Icon navigation must behave as described in the following text.
.mE
Note that
.B KDown
and
.B KRight
do not need to traverse icons in the same order.
.VL 8n
.LI "\fBKDown\fP"
.iX "KDown"
.mS
in a left-to-right direction language environment,
must move the location cursor through the icons 
in the icon box, starting at the upper-left icon
and ending at the lower-right icon then wrapping back
up to the upper-left.
.mE
In a right-to-left direction language environment,
the location cursor can move, starting at the upper-right
and moving to the lower-left.
.LI "\fBKLeft\fP"
.mS
must move the location cursor through the icons
in the opposite direction of
.BR KRight .
.mE
.LI "\fBKRight\fP"
.iX "KRight"
.mS
in a left-to-right direction language environment,
must move the location cursor through the icons
in the icon box, starting at the upper-left icon
and ending at the lower-right icon then wrapping back
up to the upper-left.
.mE
In a right-to-left direction language environment,
the location cursor can move, starting at the upper-right
and moving to the lower-left.
.LI "\fBKUp\fP"
.iX "KUp"
.mS
must move the location cursor through the icons
in the opposite direction of
.BR KDown .
.mE
.LE
.P
.iX "KNextField"
.iX "KPrevField"
Traversal can be either through all the icons in the icon
box, regardless of whether they are in the visible portion
of the icon box, or only through the icons in the visible
portion of the icon box.
.mS
If traversal only goes through
the visible portion of the icon box, the ScrollBars in the
icon box must be traversable using
.B KNextField
and
.BR KPrevField ,
which move the location cursor among the ScrollBars and
the icon area, if the icon area contains any visible icons.
.mE
If traversal goes through all icons in the icon box, the
ScrollBars should not be traversable, and
.mS
the visible portion
of the icon area must be scaled to keep the location cursor
visible.
.mE
.P
.TB "Icon Box Traversal Bindings"
.TS
center, box, tab(@);
lb lb.
Virtual Key@Default Bindings
_
KTab@<Tab>
KBackTab@MShift <Tab>
KNextField@KTab
@MCtrl KTab
KPrevField@KBackTab
@MCtrl KBackTab
KWindowMenu@MShift KEscape
@MAlt KSpace
KMenu@<Menu> or <F4>
KDown@<\(da>
KLeft@<\(<->
KRight@<\(->>
KUp@<\(ua>
.TE
