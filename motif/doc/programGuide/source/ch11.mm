.mc |
.\"  (c) Copyright 1989, 1990 OPEN SOFTWARE FOUNDATION, INC.
.mc
.\"  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
.\"  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
.\"  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
.\"  (c) Copyright 1988 MICROSOFT CORPORATION
.\"  ALL RIGHTS RESERVED
.mc |
.nr H1 10
.mc
.H 1 "Understanding the Principles of Window Management"
.P
This chapter provides you with background information.  It
.mc |
gives you a conceptual foundation upon which
.mc
to build
your understanding of the more technical aspects of window
.mc |
management that you encounter in the following chapters.
.mc
.P
.mc |
This chapter discusses the following topics:
.mc
.P
.BL
.LI
.mc |
Getting user input
.mc
.LI
.mc |
Focus policies: keyboard input and colormaps
.mc
.LI
.mc |
Distinguishing windows by type
.mc
.LI
.mc |
Understanding the parts of the MWM window frame
.mc
.LI
.mc |
Placing and sizing windows
.mc
.LI
.mc |
Introduction to icons
.mc
.LE
.P
.iX "user interface"
.iX "user interface" "default behavior"
.iX "Style Guide"
.H 2 "Getting User Input"
The default user interface behavior of MWM is based on the
\fIOSF/Motif
.mc |
Style Guide\fR.  MWM provides all of the required elements of OSF/Motif
.mc
behavior,
.mc |
but it is also customizable so you can modify window
.mc
appearance
.mc |
and behavior to suit the specific needs of you and your application.
.mc
.iX "pointer"
.iX "pointer cursor"
.iX "mouse"
.P
.H 3 "Using a Pointer"
The pointer cursor (as distinguished from the text
cursor) is controlled with
a pointer device.  In most cases, the pointer device is a mouse.
In this
.mc |
guide, the term \fBpointer\fR refers to the pointer cursor and
.mc
the term
\fBmouse\fR refers to the pointer device.
.P
A mouse has one or more buttons and is capable of the following
actions:
.P
.mc |
.VL 1.5i
.mc
.LI \fRPointing\fR
Sliding the mouse on the desktop to position the pointer on
.mc |
an object on the screen
.mc
.LI \fRPressing\fR
.mc |
Holding down a mouse button
.mc
.LI \fRClicking\fR
.mc |
Pressing and releasing a mouse button without moving the pointer
.mc
.LI \fRDouble-Clicking\fR
.mc |
Clicking a mouse button twice in rapid succession
.mc
.LI \fRDragging\fR
.mc |
Pressing and holding down a mouse button while moving the
pointer
.mc
.LE
.P
MWM has a resource, \f(CWdoubleClickTime\fR, for controlling the maximum time
(in milliseconds) that can elapse between button clicks before a double-click
.mc |
becomes just two clicks in a row.  This resource is discussed in Section 14.5.
.mc
.P
.ne 12c
.mc |
In conformance with OSF/Motif behavior, a mouse under MWM has the
uses described in the following table:
.mc
.P
.TB "Mouse Actions and Their Effects"
.TS
box, tab(;);
lb | lb | lb
lw(1.5i)  | lw(1.5i)  | l .
.mc
This action...   ;Does this...  ;For example...
.mc |
_
.mc
T{
Pressing Select button. (Button 1)
T};T{
.mc |
Initiates a drag 
operation.
.mc
T};T{
Pressing the Select button on the window menu button of a client's window
frame brings up the window menu.
T}
_
.mc |
T{
Releasing Select button.
T};T{
.mc
Completes an action started with a press and drag of the Select
button.
T};T{
Releasing the Select button after dragging the button on a title
bar
repositions the window and ends the move operation.
T}
_
.mc |
T{
Clicking Select button.
T};T{
.mc
Selects an object or action.
T};T{
.mc |
Clicking the Select button on a client's Window-frame Minimize
button turns the client window into an icon.
.mc
T}
_
T{
Double-clicking Select button.
T};T{
.mc |
Provides a shortcut for doing a default action associated with an object.
.mc
T};T{
.mc |
Double-clicking the Select button on an icon turns the icon into the
.mc
window associated with the icon.
T}
_
.mc |
T{
Dragging Select button.
.mc
T};T{
.mc |
Performs a drag function such as resizing a window, or browsing a menu.
T};T{
Pointing to the title area of a client's window frame and dragging the
.mc
Select button performs a move operation.
T}
.TE
.iX "keyboard"
.iX "keyboard" "actions"
.P
.H 3 "Using a Keyboard"
In conformity with OSF/Motif behavior, MWM allows you to perform
window
management functions without using a mouse.
.P
.mc |
For example, pressing <\fBShift\fR> <\fBEsc\fR> pops up the window
.mc
menu for the
active window.  The arrow keys are used to browse the menu
items, then
.mc |
pressing <\fBSelect\fR> selects an item.  Pressing <\fBEsc\fR>
cancels the current window management operation.
.mc
.P
Window management functions performed from the keyboard generally
apply to
.mc |
the active window, the window that is getting keyboard input.  
.mc
However, you
.mc |
can also use the keyboard for nonspecific window management
.mc
functions such
.mc |
as changing the focus or stacking order of windows on the screen.
.mc
.iX "modifying" "default behavior"
.iX "default behavior" "modifying"
.iX "resource files"
.P
.H 3 "Modifying Default Behavior"
While the default window management behavior is recommended for
the sake of
.mc |
consistency, MWM
.mc
allows users to modify the default behavior to
suit their
needs.  Users can modify
the default
.mc |
behavior of MWM
.mc
by changing the entries in the resource files
that it
.mc |
uses to configure its appearance and behavior.
.P
Table 11-2 summarizes the resource files used by MWM.
Some of the filenames in the table represent customary locations for
the resource files described.
In some cases the actual location can depend on environment variables
and on the current language environment.
For more information see the ``Creating Defaults Files'' section in
Chapter 3, the ``Localization'' section in Chapter 8, and the
description of the MWM \fBconfigFile\fP resource in Chapter 14.
.mc
.iX "configuration files"
.mc *
.mc
.iX "configuration files" "$HOME/.Xdefaults"
.iX "configuration files" "/usr/lib/X11/system.mwmrc"
.iX "configuration files" "$HOME/.mwmrc"
.iX "configuration files" "/usr/lib/X11/app-defaults/Mwm"
.P
.mc |
.ne 10c
.TB "MWM Resource Configuration Files"
.mc
.TS
center, box, tab(;);
lbw(2.25i) | lb
l  | l .
File Name;Description
.mc |
_
.iX "/usr/lib/X11/app-defaults/Mwm"
/usr/lib/X11/app-defaults/Mwm;T{
.hy 14
Contains system-wide MWM resource configuration information.
The actual file location may depend on the XFILESEARCHPATH environment
variable and the current language environment.
.hy 0
.mc
T}
_
.mc |
.iX "$HOME/Mwm"
$HOME/Mwm;T{
.hy 14
Contains user-specific MWM resource configuration information.
The actual file location may depend on the XUSERFILESEARCHPATH and
XAPPLRESDIR environment variables and the current language environment.
.hy 0
T}
_
.mc
.iX "$HOME/.Xdefaults"
$HOME/.Xdefaults;T{
.mc |
.hy 14
.mc
Contains user-specific resource configuration
information about MWM and other clients used by this user.
.mc |
.hy 0
.mc
T}
_
.mc |
.iX "$HOME/.Xdefaults-\fIhost\fP"
$HOME/.Xdefaults-\fIhost\fP;T{
.hy 14
Contains user-specific and host-specific resource configuration
information about MWM and other clients used by this user.
The actual file location may depend on the XENVIRONMENT environment
variable.
.hy 0
T}
_
.iX "$HOME/.motifbind"
$HOME/.motifbind;T{
.hy 14
Contains user-specific virtual key bindings.
.hy 0
T}
_
.iX "/usr/lib/X11/$LANG/system.mwmrc"
.mc
.iX "/usr/lib/X11/system.mwmrc"
.mc |
T{
/usr/lib/X11/$LANG/system.mwmrc or
.br
/usr/lib/X11/system.mwmrc
T};T{
.hy 14
Contains system-wide configuration
information about MWM menus and keyboard and button bindings.
.hy 0
.mc
T}
_
.mc |
.iX "$HOME/$LANG/.mwmrc"
.mc
.iX "$HOME/.mwmrc"
.mc |
T{
$HOME/$LANG/.mwmrc or
.br
$HOME/.mwmrc
T};T{
.hy 14
Contains user-specific configuration information about
MWM menus
.mc
and keyboard and button bindings used by this user.
.mc |
.hy 0
.mc
T}
.mc *
.mc
.TE
.P
Menus, mouse button bindings, and keyboard bindings are discussed in
.mc |
Chapter 14.  
.mc
.iX "focus policies"
.iX "active window"
.iX "window" "active"
.P
.H 2 "Using Focus Policies:  Keyboard Input and Colormaps"
The input focus policy determines how and when a window becomes the active
window.  An active window has the following characteristics:
.BL
.LI
.mc |
What you type appears in the window
.mc
.LI
The color of the window frame changes to indicate the active
.mc |
focus
.mc
.LI
.mc |
Input from extended input devices goes to the window
.mc
.LE
.iX "keyboard input focus"
.iX "keyboard input focus" "default methods for changing"
.P
.mc |
The default methods of changing the keyboard input focus are shown in
Tables 11-3 and 11-4.
.mc
.P
.ne 4c
.mc |
.TB "MWM Default Keyboard Input Focus with a Mouse"
.mc
.TS
center, box, tab(;);
lb |  lb
l  |  l .
Doing this...;Does this...
.mc |
_
.mc
T{
Clicking the mouse Select button with the pointer on the window or window
frame.
T};T{
Selects that window as the focus of keyboard input.
T}
_
T{
Clicking the mouse Select button with the pointer on an icon.
T};T{
Selects that icon as the focus of keyboard input.
T}
.TE
.P
.ne 2i
.mc |
.TB "MWM Default Keyboard Input Focus with a Keyboard"
.mc
.TS
center, box, tab(;);
lb | lb
l  | l  .
Pressing this key sequence...;Does this...
.mc *
.mc
_
.mc |
<\f2Alt\fP> <\f2Tab\fP>;Moves the keyboard input focus
;to the next window in the
;window stack.
_
<\f2Alt\fP> <\f2Shift\fP> <\f2Tab\fP>;Moves the keyboard input focus
;to the previous window in the
;window stack.
.mc
.TE
.P
.iX "input focus policy"
.iX "input focus policy" "explicit"
.iX "input focus policy" "track pointer"
.iX "explicit"
.iX "track pointer"
The default input focus policy is an "explicit" selection policy; the user
must perform
a mouse button action or a key action to change the input focus.
A "track pointer" policy, which changes the input
focus to match the pointer's position (no other action is required), is
.mc |
also available.  These policies are explained further in Chapter 13.
.mc
.iX "colormap focus policy"
.H 3 "Setting the Colormap Focus Policy "
The colormap focus policy dictates which client window has its colormap
installed.  A \fBcolormap\fR is a display resource that controls the
set of colors appearing on the display.  Colormap focus policy is
.mc |
explained in Chapter 13.
.mc
.H 2 "Distinguishing Windows by Type"
.P
.mc |
As you design your client application and write the code, you 
.mc
use
particular types of windows to fulfill the specific needs of your
design
plan.
.iX "client windows"
.P
.H 3 "Choosing the Right Type of Client Window"
When you design your client application, remember that MWM
recognizes
the following types of client windows:
.iX "primary window"
.iX "top-level window"
.iX "window" "top-level"
.iX "root" "window"
.iX "window" "root"
.P
.VL 1.35i
.mc |
.LI \fRPrimary Window\fR
.mc
A primary window is a top-level window, a direct
descendent of the root window.  MWM provides this type of client
.mc |
window with a window frame.  By default, this frame is decorated with
the full set of functional frame components (resize frame handles, 
title bar, and window control buttons).  You can change the window 
decoration on primary windows either programmatically from your client 
or by using the \f(CWclientDecoration\fR resource, as explained in 
Section 13.11.
.mc
.iX "clientDecoration resource"
.iX "resource" "clientDecoration"
.iX "secondary window"
.iX "windows" "transient"
.mc |
.LI \fRSecondary Window\fR
.mc
A secondary window is a window that is transient in
nature.  A secondary window is associated with another window,
.mc |
usually a primary window, and is always over that window in the window
stack.  Secondary windows are iconified (minimized) together with their 
associated windows.  A secondary window may also receive keyboard or
.ne 6
pointer input that it does not pass on to its associated window.  This 
is known as being "modal" with respect to the associated window.  For 
example, you might design a secondary window as an application modal 
dialog box that requires an action by the user before the application
.mc
continues.
.iX "windows" "modal"
.P
The WM_TRANSIENT_FOR property indicates that a window is a
secondary
window and identifies the associated window.  The _MOTIF_WM_HINTS
property
indicates the input mode of a secondary window.  Window
properties are
discussed in more detail in the next chapter.
.iX "WM_TRANSIENT_FOR"
.iX "property" "WM_TRANSIENT_FOR"
.iX "_MOTIF_WM_HINTS"
.iX "property" "_MOTIF_WM_HINTS"
.mc |
A secondary window typically receives less window-frame
.mc
decoration than
a primary window, and, typically, fewer window management
functions are
available to control the window.   For example, a secondary window
may not
have a minimize button in the window frame or resize frame handles.
.P
.iX "transientDecoration resource"
.iX "resource" "transientDecoration"
.iX "windows" "client icon windows"
.iX "client icon windows"
.iX "client icon" "windows"
.iX "windows" "client icon"
.iX "minimized windows"
.iX "windows" "minimized"
.iX "override-redirect windows"
.iX "windows" "override-redirect"
You can configure a secondary window either programmatically,
with the
.mc |
_MOTIF_WM_HINTS property (Chapter 12), or with the \f(CWtransientDecoration\fR
resource, (Chapter 13).
.mc
.LI "\fRClient Icon\fR"
A client icon is supplied by a client for use as an image in an MWM icon.
.LI "\fRClient Icon Window\fR"
.mc |
.br
.mc
A client icon window is supplied by a client for
use as an alternative to a pixmap image in MWM icons (minimized
windows).  This window can be used while the window is in its iconic state.
.mc |
.LI \fROverride-redirect Window\fR
.br
.mc
An override-redirect window is typically visible for
only a short time and, while in use, the pointer should be grabbed by
.mc |
the client.  A common example of this type of window is a pop-up menu.
MWM does not place override-redirect windows in a window frame, nor 
does MWM support window management operations on override-redirect 
windows.
.mc
.LE
.P
.iX "input modes"
.H 3 "Input Modes"
.mc |
Certain windows constrain the user's input.  There are four
.mc
levels of window constraints, called "modes."
.BL
.iX "modeless windows"
.iX "windows" "modeless"
.LI
\fBModeless windows\fR do not constrain user input to other
windows.  Client primary windows are generally modeless.
.LI
.mc |
.iX "primary application modal windows"
.iX "windows" "primary application modal"
\fBPrimary Application Modal Windows\fR "prevent" input from going to any
of the window's ancestors.  Secondary client windows are
.mc
sometimes application modal with respect to their associated
.mc |
primary window.
.mc
.LI
.mc |
.iX "full application modal windows"
.iX "windows" "full application modal"
\fBFull Application Modal Windows\fR "prevent" input from going to any
of the window's ancestors or to any other windows in the same
application that are not in the window's hierarchy.
.LI
.mc
.iX "system modal windows"
.iX "windows" "system modal"
\fBSystem Modal Windows\fR are similar to application modal
.mc |
windows except that they prevent input from going to any other
.mc
window on the screen.  For example, restart confirmation messages are
displayed in system modal windows.
.LE
.H 3 "Understanding Window Manager Windows"
MWM uses the following window types to provide window
management
services to your client application:
.P
.iX "icon frame windows"
.iX "windows" "icon frame"
.iX "client frame windows"
.iX "windows" "client frame"
.mc |
.VL 2i
.mc
.LI "\fRClient Frames\fR"
A client frame is placed around the client area.
.LI "\fRIcon Frames\fR"
An icon is a small graphic representation of your client
.mc |
application window.  When the window manager minimizes (iconifies)
.mc
a full-sized client window, it uses an icon window frame to represent
the client.
Icons can be arranged on the screen by the window manager or
.mc |
placed in an icon box.
.br
.ne 10v
.mc
.LI "\fRIcon Box\fR"
.iX "icon box" "windows"
.iX "windows" "icon box"
An icon box is a window used by the window manager to
contain icons.  An icon box window is decorated with a window
frame that is
typically the same as a primary window's frame.
.iX "feedback window"
.iX "windows" "feedback"
.iX "windows" "size"
.iX "windows" "position"
.mc |
.ne 8
.mc
.LI "\fRFeedback Window\fR"
.mc |
A feedback window appears at the center of the
.mc
screen when a primary client window is being either resized or
repositioned
by the window manager.  If the client is being resized, the
feedback window
contains the size (width and height) of the client.  If the
client is being
relocated, the feedback window contains the new location (x-offset
and y-offset).
.P
A configuration feedback window is displayed whenever MWM is restarted or a
behavior switch is requested.  The confirmation window is system modal and
is displayed in the center of the screen.  Explicit user confirmation is
required for MWM to continue the restart or behavior switch.
.LE
.iX "window frame" "elements"
.mc |
.br
.ne 40
.mc
.P
.H 2 "Understanding the Parts of the MWM Window Frame"
.mc |
MWM surrounds client windows with a functional frame.
.mc
Positioning
the pointer on a part of the frame and performing the appropriate
mouse
button action or key action executes the function of that frame part.
.mc |
The following figure shows the components of the frame.
.mc
.P
.iX "window menu button" "location"
.iX "resize frame handles" "location"
.iX "resize border" "location"
.iX "window frame parts"
.iX "title bar" "location"
.iX "title area" "location"
.iX "menu button" "location"
.iX "buttons" "on window frame"
.iX "minimize button" "location"
.iX "maximize button" "location"
.iX "corner brackets" "location"
.iX "frame parts"
.iX "window frame parts" "window menu button" 
.iX "window frame parts" "resize frame handles" 
.iX "window frame parts" "resize border" 
.iX "resize border" "See also resize frame handles."
.iX "window frame parts" "title bar" 
.iX "window frame parts" "title area" 
.iX "menu button" "location"
.iX "buttons" "on window frame"
.iX "window frame parts" "minimize button" 
.iX "window frame parts" "maximize button" 
.iX "window frame parts" "corner brackets" 
.ig ++
.br
.ie \nZ \{\
.br
.ne 29P
.P
.sC graphics/nudefrm.tif tiff 300 29P 38P
\&
.P
.sp +1
.FG "MWM Surrounds a Client's Window with a Frame"
.sp +1.5
.\}
.el \{\
.br
.ne 29P
\&
.sp 27.5P
.ce
illustration graphics/nudefrm.eps goes here
.sp +1
.FG "MWM Surrounds a Client's Window with a Frame"
.sp +1.5
.\}
.P
.++
.br
.mc |
.ne 4.75i
.mc
\0
.mc *
.mc
.FG "MWM Surrounds a Client's Window with a Frame"
.mc |
.sp 4.5i
.P! graphics/nudefrm.ps 
.br
.ne 8
.mc
.P
The parts of the MWM window manager frame and their functions 
.mc |
are listed in the following table:
.mc
.P
.ne 2.3i
.iX "window frames" "behavior of components (summary table)"
.TB "Window Frame Parts and What They Do"
.TS
center, box, tab(;);
lb | lb
l  | l  .
This frame part...;Is used to...
.mc |
_
.mc
Title area.;Move a window.
_
Window menu button.;Display the window menu.
_
Minimize button.;Iconify a window.
_
Maximize button.;T{
Expand a window to maximum size.
T}
_
Resize frame handles.;T{
Stretch or shrink a window horizontally, vertically, or
diagonally
(in two directions).
T}
.TE
.P
.H 3 "Coloring Frame Components"
MWM gives you the option of selecting varied color schemes for
active and inactive client windows.  Additionally, you may
.mc |
specify the color of a 3D matte frame.  This topic
is covered in Section 13.8.
.mc
.P
.H 3 "Understanding Frame Components and Their Behavior"
The components of the client window frame provide
users with visual cues for direct manipulation of the window.  You can
configure the basic frame layout for your client application
.mc |
programmatically by using the _MOTIF_WM_HINTS property (Chapter 12).
Users can set the \f(CWclientDecoration\fR (Chapter 13) and
\f(CWclientFunction\fR (Chapter 14) resources to configure the
.mc
client window frame.
.iX "resize frame handles"
.H 4 "Resize Handles"
The function of the resize handles is to resize the window frame
and your
client application with it.
.P
.iX "mouse actions" "on frame components, defaults"
A user initiates a resize operation by positioning the
pointer on one of the "handles," pressing the mouse Select
button, and
.mc |
dragging the window to the desired size.  A wire outline shows changing
.mc
dimensions as the mouse is dragged.  Also, a
small configuration feedback window displays the current
dimensions of the
frame.  The resize operation stops when the user releases the Select
button.
.mc |
It is also canceled when the user presses \fB<Esc>\fP.
.mc
.P
.iX "key actions" "on frame components, defaults"
.iX "window resizing"
The resize handles form a frame composed of eight distinct pieces:
.P
.BL
.LI
.mc |
Top
.mc
.LI
.mc |
Bottom
.mc
.LI
.mc |
Left side
.mc
.LI
.mc |
Right side
.mc
.LI
.mc |
Lower right corner
.mc
.LI
.mc |
Lower left corner
.mc
.LI
.mc |
Upper right corner
.mc
.LI
.mc |
Upper left corner
.mc
.LE
.P
.mc |
Each handle has a distinct cursor shape.  The following figure 
illustrates these shapes.
.mc
.iX "resize frame handles" "cursor shapes"
.ig ++
.P
.br
.ie \nZ \{\
.br
.ne 11P
.P
.sC graphics/wmcalls.tif tiff 300 11P 18P
\&
.P
.sp +1
.FG "Resize Handles Have Distinctive Cursor Shapes"
.sp +1.5
.\}
.el \{\
.br
.ne 11P
\&
.sp 9.5P
.ce
illustration graphics/wmpoint.eps goes here
.sp +1
.FG "Resize Handles Have Distinctive Cursor Shapes"
.sp +1.5
.\}
.P
.++
.br
.mc |
.ne 2i
.mc
\0
.mc *
.mc
.FG "Resize Handles Have Distinctive Cursor Shapes"
.mc |
.sp 1.75i
.in +1.56i
.P! graphics/wmpoint.ps 
.in -1.56i
.br
.ne 5
.mc
.P
Which handle a user drags, and in which direction the
.mc |
user drags it, affects the outcome of the resize operation.
The following table lists where to grab a handle for the desired result.
.mc
.P
.ne 3i
.TB "Where to Grab an MWM Resize Handle"
.TS
center, box, tab(;);
lb | lb
lb | ^
l | s
c | l
c | l
l | s
c | l
c | l
l | s
c | l .
To stretch or;Position the pointer on the...
shrink the window...
.mc *
.mc
_
.mc |
\fBvertically from the...\fP
_
.mc
top;top of the frame, above the title bar
_
bottom;bottom of the frame
_
.mc |
\fBhorizontally from the...\fP
.mc
_
right;right side of the frame
_
left;left side of the frame
_
.mc |
\fBdiagonally from the...\fP
.mc
_
bottom left corner;frame's lower left corner
_
top left;frame's upper left corner
_
top right;frame's upper right corner
_
bottom right;frame's lower right corner
.TE
.P
As an application programmer, you have several ways to control a user's
ability
to resize your client window:
.P
.BL
.LI
A window cannot be resized smaller than the minimum size
specified by your
client.
.LI
A window cannot be resized smaller than the minimum size dictated
by the
components you select for the window frame.
.LI
A window cannot be resized larger than the maximum window size
specified by your client.
.LI
A client can indicate that the resize function should not apply (use the
.mc |
_MOTIF_WM_HINTS property, Chapter 12).
.mc
.LE
.P
.mc |
Window frames can be configured to omit resize handles.  The frame can then
.mc
be configured to have a border or to be borderless.
.H 4 "The Title Area"
The title area has two functions:
.iX "title area"
.iX "window frame" "title area"
.P
.BL
.LI
It usually lists the name of the application appearing within the client
area of
the window.
.LI
It provides the user with a "handle" to grab during a move
operation.
.LE
.iX "window" "positioning"
.iX "moving a window"
.P
A user initiates a move operation by positioning the pointer
anywhere in
the title area, pressing the mouse Select button, and then dragging the
pointer to
a new location.  A wire frame outline represents the location of
the window
during the move.  The user completes the move operation by
releasing the
Select button at the desired location.  When the button is
released, the
window moves to that location.
.mc |
A move in progress is canceled when the user presses \fB<Esc>\fP.
.mc
.iX "window sizing" "constraints on minimum"
.iX "title bar"
.H 4 "The Title Bar"
.mc |
The title bar, which may include a title area, window menu button, and
.mc
minimize
.mc |
and maximize buttons, is constrained so that all components in
.mc
the title
bar are always visible.  This necessitates a minimum frame size
and, by
extension, a minimum size for your application's window.  The
size of the
components in the title bar depends upon the size of the font
you choose
for the title area text.
.P
As a user shrinks your client window horizontally, space is
removed from
the title area first.  As the window is resized smaller, title text is
clipped from the right.  The absolute minimum size of
the title bar is equal to the size of the buttons in the
title bar, plus a title area equivalent to the width of one title bar button.
.iX "window menu button"
.mc |
.br
.ne 15
.mc
.H 4 "Window Menu Button"
The function of the window menu button is to display the window
menu.  There are two ways to display the window menu:
.P
.AL
.LI
\fBUsing the window menu button.\fR  Position the pointer on the
.mc |
window-menu button of the window frame and press the mouse Select
.mc
button.  (If you want the menu to remain posted, click the mouse Select
.mc |
button.)  Once the menu appears,
.mc
browse the menu by dragging the pointer down the menu.  As the
.mc |
pointer encounters an available selection, the selection is
highlighted.  To
.mc
choose a selection, highlight the desired selection and
release the mouse Select button.
.LI
\fBUsing the keyboard.\fR  Set the keyboard input focus to a window and
.mc |
press <\fBShift\fR> <\fBEsc\fR>, or <\fBAlt\fR> <\fBSpace\fR>.  Use arrow
.mc
keys to
.mc |
browse the menu and <\fBSelect\fR> to choose menu items.  Cancel the
posted menu using <\fBShift\fR> <\fBEsc\fR> again, or just
<\fBEsc\fR>.
.mc
.LE
.P
There are two resources used to configure the behavior of the window menu
button with respect to mouse clicks.
The \f(CWwMenuButtonClick\fR resource
indicates whether a click of the mouse when the pointer is over
.mc |
the window menu button posts and leaves posted the window
.mc
menu. The \f(CWwMenuButtonClick2\fR resource indicates whether
.mc |
a double-click action on the window menu button closes the window and
terminates the application.
Both resources are discussed in Chapter 14, "Changing the Menu Associated
with the Window Menu Button."
.mc
.iX "minimize button"
.iX "window frame" "minimize button"
.iX "buttons" "minimize"
.iX "buttons" "small square image"
.iX "iconify" "minimizing a window"
.iX "minimize" "iconifying a window"
.H 4 "Minimize Button"
The function of a window's minimize button is to iconify the window.
.P
A user minimizes your client window by positioning the pointer on
the
minimize button and then clicking the mouse Select button.
As a result of
the minimize operation, your client window changes into an icon
on the screen.
If you use an icon box, the icon in the box displays its
iconified
client appearance.
.P
.mc |
.br
.ne 15
.mc
.H 4 "Maximize Button"
The function of a window's maximize button is to make the window its
largest allowable size.  
.iX "maximize button"
.iX "window frame" "maximize button"
.iX "windows" "maximizing the size"
.iX "buttons" "maximize"
.iX "buttons" "large square image"
.P
A user maximizes your client window by positioning the pointer on
the
maximize button and then clicking the mouse Select button.
As a
result of the maximize operation, the client window expands
.mc |
to the maximum size set by either the client or the
configured resources.  By default, the client fills the screen.
.mc
.P
If a maximum size other than the default (full screen) size is
important to
your application, you can set the maximum size for your client by
specifying values in the appropriate fields in the
WM_NORMAL_HINTS
.mc |
property (see Chapter 12.)
.mc
Users can also set a maximum size for windows by using the
\f(CWmaximumClientSize\fR resource and the
.mc |
\f(CWmaximumMaximumSize\fR resource (see Chapter 13).
.mc
.P
MWM tries to maximize your client window without moving it if
your client
.mc |
fits on the screen at its current location.  If the maximized
.mc
window
.mc |
extends off the screen, MWM moves the maximized window
onscreen,
.mc
placing the window with the title bar showing so that the user
still has
access to the window control buttons.  When a maximized window extends
beyond the screen boundaries, MWM places the upper left-hand portion of the
maximized window in the upper left-hand corner of the screen.
.P
.mc |
Once your client is maximized, the large square image on the
.mc
maximize button
appears recessed.  In this state, the maximize button functions
as a
"restore" button.  Clicking the mouse Select button on the recessed
maximize
button restores your client to its previous size.
.mc |
.br
.ne 4i
.mc
.H 4 "Manipulating the Window Frame with a Mouse or a Keyboard"
.mc |
The following table summarizes the default frame manipulations available for
mouse operations.
.mc
.P
.iX "window frame parts" "manipulating with a mouse"
.iX "window frame parts" "manipulating with a keyboard"
.mc |
.TB "Default Mouse Operations"
.in0
.mc
.TS
center, box, tab(;);
lb | lb | lb
^ | cb | ^
l | l | l .
Doing this...;With the pointer;Does this...
;on this component...;
.mc |
_
Clicking the Select button;Frame or window;T{
Selects current keyboard input focus and brings window to top of the stack
.mc
T}
_
.mc |
Clicking the Select button;Frame or window;Brings window to top
.mc
_
.mc |
Dragging the Select button;Resize frame handles;Resizes the window
.mc
_
.mc |
Clicking the Select button;Window menu button;T{
Posts window menu
.mc
T}
_
.mc |
Dragging the Select button;Window menu button;T{
Pulls down window menu
.mc
T}
_
.mc |
Double-clicking the Select button;Window menu button;T{
Closes the window
.mc
T}
_
.mc |
Dragging the Select button;Title area;Moves the window
.mc
_
.mc |
Clicking the Select button;Minimize button;Iconifies the window
.mc
_
.mc |
Clicking the Select button;Maximize button;Maximizes the window
.mc
.TE
.mc |
.in
.br
.ne 4i
.mc
.P
The following table summarizes the default frame manipulations
.mc |
available for keyboard operations.
.mc
.P
.mc |
.TB "Default Keyboard Operations"
.mc
.TS
center, box, tab(;);
lb | lb
lb | ^
l | l .
.mc
Pressing this;Does this...
key sequence...
.mc *
.mc
_
.mc |
<\fBShift\fP> <\fBEsc\fP>;Posts or unposts
.br
the window menu
.mc
_
.mc |
<\fBAlt\fP> <\fBSpace\fP>;Posts or unposts
.br
the window menu
.mc
_
.mc |
\fBWhen window 
.br
menu is posted...\fP
.mc
_
.mc |
<\fBEsc\fP> menu;Unposts the window
.mc
_
.mc |
<\(da>;Moves to the next selection
.br
selection on a menu
.mc
_
.mc |
<\(ua>;Moves to the previous selection
.br
selection on a menu
.mc
_
.mc |
<\(->>;Displays a submenu
.br
cascade button
.mc
_
.mc |
<\(<->;Unposts a submenu
.br
cascade button
_
<\fBSelect\fP>;Selects the highlighted menu selection
.mc
.TE
.mc |
.br
.ne 3.5i
.mc
.P
.mc |
.H 3 "Selecting From the Window Menu"
.mc
.iX "window menu" "selecting from"
The window menu contains selections that provide consistent
.mc |
functionality
.mc
from one MWM application to another.  This consistency reduces
the time it
takes a user to learn to manage your application windows.
.P
The window menu provides an additional way to access window manager
functionality.  You can select items in the window menu with either the
.mc |
mouse or the keyboard.  The following figure shows the default menu window.
.mc
.ig ++
.P
.br
.ie \nZ \{\
.br
.ne 20P
.P
.sC graphics/baremenu.tif tiff 300 20P 28P
\&
.P
.sp +1
.FG "OSF/Motif Default Window Menu"
.sp +1.5
.\}
.el \{\
.br
.ne 20P
\&
.sp 18.5P
.ce
illustration graphics/baremenu.eps goes here
.sp +1
.FG "OSF/Motif Default Window Menu"
.sp +1.5
.\}
.P
.++
.br
.mc |
.ne 5i
.mc
\0
.mc *
.mc
.FG "OSF/Motif Default Window Menu"
.mc |
.sp 2.8i
.in +1.31i
.P! graphics/baremenu.ps 
.in -1.31i
.ne 5i
.mc
.P
.H 4 "Looking at the Contents of the Window Menu"
.mc |
The following table describes the window menu selections.
.mc
.P
.iX "window menu selections"
.iX "restore" "window menu selection"
.iX "move " "window menu selection"
.iX "size" "window menu selection"
.iX "minimize" "window menu selection"
.iX "maximize" "window menu selection"
.iX "bottom" "window menu selection"
.iX "close" "window menu selection"
.mc *
.mc
.TB "The Window Menu Selections"
.TS
center, box, tab(;);
lb | lb
l | l .
This selection...;Does this...
.mc |
_
.mc
Restore;T{
Restores a window to its normal size from an icon or after maximizing.
T}
_
Move;Changes the location of a window.
_
Size;Changes the width and height of a window.
_
Minimize;Shrinks a window to its icon (graphic representation).
_
Maximize;Enlarges a window to its maximum size.
_
Lower;T{
.mc |
Places a window at the bottom of the window stack.
.mc
T}
_
Close;Terminates the client.
.TE
.P
Menu selections for functions that are not immediately applicable
when the
menu is displayed are grayed out.  This is the case for the
.mc |
Restore
selection when your client is in its normal state (see Figure 11-3).
.mc
.P
.iX "window menu" "selecting items with mouse"
.mc |
.H 4 "Selecting with the Mouse"
The mouse can be used in 2 ways to select items from the window menu:  1)
press to open, drag to highlight, release to select, or, 2) click to open
and post, move to desired item, and click to select.
.mc
.iX "window menu" "selecting items with keyboard"
.mc |
.H 4 "Selecting with the Keyboard"
.mc
Key actions can be used to select items from the window menu and to
access functions of the window frame components.  Once the menu is
displayed, using
.mc |
the <\fBShift\fR> <\fBEsc\fR> sequence, the user presses the \fB<\(ua>\fP
and \fB<\(da>\fP keys to highlight the appropriate selection and the
<\fBSelect\fR> key to make
.mc
the choice.
.mc |
.br
.ne 10
.mc
.P
.iX "windows" "positioning"
.iX "windows" "resizing"
.mc |
While this completes the operation in the case of the Minimize,
Maximize, and Lower operations, the Resize and Move
operations
.mc
require further input from the user.  When the user chooses 
a move or resize operation, the
pointer
.mc |
changes to a 4-headed arrow centered on the client area of the
.mc
window.  The user moves or resizes the window by pressing the
.mc |
arrow keys.  Pressing <\fBCtrl\fR> simultaneously with the arrow keys
moves the pointer in multiple increments.  Once the window is in the
.mc
desired location or has
.mc |
the desired size, the user presses <\fBSelect\fR>
.mc
to complete
.mc |
the operation.  Pressing <\fBEsc\fR> cancels a move or resize
.mc
operation.
.H 4 "Accelerators and Mnemonics"
.iX "accelerators"
.iX "mnemonics"
Window menu items can also be selected using accelerators and mnemonics.
Accelerators are key bindings that cause a menu
item to be selected, and the associated function done, without
.mc |
posting the menu.  For example, <\fBAlt\fR> <\fBF9\fR> is used to
.mc
minimize (iconify) a window.  Accelerators appear in the
menu to the right of their associated menu items.  Mnemonics
are used to select a menu item once the window menu is posted.
Mnemonic keys are indicated in a menu item by an underlined
character in the menu item label.
.iX "windows" "closing"
.iX "closing a window"
.iX "killing a window"
.H 4 "Closing Your Client's Window"
.mc |
When a user chooses the Close selection from the window menu,
.mc
the window
manager executes an \f(CWf.kill\fR function on the client window.
You can
configure the \f(CWf.kill\fR function either to send a message to
the client
window or to terminate the client's X connection.  Refer to
WM_SAVE_YOURSELF, WM_PROTOCOLS, and WM_COMMAND in
.mc |
Chapter 12, and to the MWM function \f(CWf.kill\fR in Chapter 14.
.mc
.P
.iX "closing a window" "window menu button"
Another way to close a window is to double-click the mouse Select button
with the pointer on the window menu button.  This behavior is enabled with
.mc |
the \f(CWwMenuButtonClick2\fR resource.  (See Section 14.4 for more
information on window menus.)
.mc
.H 2 "Placing and Sizing Windows"
In the OSF/Motif environment you can think of client windows as existing in
a stack on the screen.  A new client window is initially placed on top of the
window stack with a size and position specified by the client or the user.
.P
A user can establish a different initial size and position for windows by
.mc |
using the procedures presented in Chapter 13, "Controlling Window
.mc
Size and Position."
.iX "window position" "interactivePlacement"
.iX "window size" "interactivePlacement"
.P
.mc |
Users can enable interactive placement by setting the
\f(CWinteractivePlacement\fR
resource.  During interactive placement, the following visual clues appear
.mc
on the screen:
.P
.BL
.LI
The pointer changes to an upper-left-corner bracket for
.mc |
the duration of the placement operation
.mc
.LI
A wire frame outline shows the current size and location of the
.mc |
window
.mc
.LI
.mc |
A configuration feedback window appears in the center of the
screen, relating location and size information
.mc
.LE
.P
.iX "interactive placement" "with mouse"
To interactively set the initial position of your client without changing
its size, the
user moves the
pointer (and with it the wire outline) to the desired location and
clicks the mouse
Select button.
.P
To interactively set the initial position \fIand\fR change the size of your
client, the
user performs the following steps:
.P
.AL
.LI
Establishes the upper left-hand corner of your client window by
moving the
pointer to the desired location and pressing the mouse Select button.
.LI
.mc |
With the mouse Select button pressed, resizes your client window to the
desired size by dragging the
.mc
Select
button to the desired location of the lower right-hand corner of
.mc |
the window.  The pointer changes to a lower-right-hand corner bracket
.mc
for the duration of the sizing operation.
.LE
.mc |
.br
.ne 7
.mc
.P
.mc |
Interactive placement can also be done from the keyboard. The following table
.mc
summarizes interactive placement behavior when using the
.mc |
keyboard.
.mc
.P
.ne 4i
.iX "interactive placement" "with keyboard"
.TB "Interactive Placement Using the Keyboard."
.TS
center, box, tab(;);
lb | lb
l  |  l  .
This key press...;Performs this function...
.mc *
.mc
_
.mc |
\fBwhile positioning the window...\fP;
.mc
_
.mc |
<\(ua>;Moves up one pixel
.mc
_
.mc |
<\(da>;Moves down one pixel
.mc
_
.mc |
<\(<->;Moves left one pixel
.mc
_
.mc |
<\(->>;Moves right one pixel
.mc
_
.mc |
<\fBCtrl\fP> <\fIarrow key\fP>;Move several pixels
.mc
_
.mc |
<\fBSpace\fP>;End positioning, start sizing operation
.mc
_
.mc |
\fBwhile sizing the window...\fP;
.mc
_
.mc |
<\(ua>;Decreases the height one pixel
.mc
_
.mc |
<\(da>;Increases the height one pixel
.mc
_
.mc |
<\(<->;Decreases the width one pixel
.mc
_
.mc |
<\(->>;Increases the width one pixel
_
<\fBCtrl\fP> <\fIarrow key\fP>;T{
Changes the height or width several pixels
.mc
T}
_
.mc |
<\fBSelect\fP>;Completes the interactive placementI
.mc
.TE
.P
The arrow keys move the pointer in the direction of the arrow.
Holding
.mc |
down <\fBCtrl\fR> moves the pointer in larger increments.  The
space bar is used to complete positioning and switch to sizing the window.
Pressing 
<\fBSelect\fR> completes the interactive placement operation.  
.mc
.P
The configuration feedback window provides position and size
information as
the pointer moves.  The size feedback is in units that depend
on the client.  You can specify the units with the \f(CWWM_NORMAL_HINTS\fR
.mc |
property (see Chapter 12).
.mc
.P
If window position information is specified by the user with the
.mc |
command-line
.mc
geometry option when the client is invoked, no interactive
placement takes
place.  The window is simply placed on the screen with the size and
position
.mc |
specified.  If the user specifies only the size of the window,
.mc
MWM uses
that size as the default during the interactive placement
operation.
.mc |
.br
.ne 3
.mc
.P
MWM does not support interactive placement on secondary windows or on the
initial placement of icons.
.P
.iX "stacking windows"
.iX "windows" "stacking"
.H 3 "Stacking Windows"
With the default MWM configuration, you can change the stacking order of
windows
on the screen using the following methods:
.P
.BL
.LI
By setting the keyboard input focus so that the active window
rises to the
top of the stack.  Set the keyboard input focus either by
clicking the mouse Select button on
.mc |
the window or by pressing the <\fBAlt\fR> <\fBTab\fR> accelerator
.mc
sequence.  For this feature to work, the \f(CWfocusAutoRaise\fR
.mc |
resource should be set to "True." (See Chapter 13, "Setting Focus Policies.")
.mc
.P
.LI
By iconifying a window, thus effectively taking it off the stack
until the
icon is normalized.
.LI
By performing a window manager stacking function such as
\f(CWf.circle_up\fR,
\f(CWf.circle_down\fR, \f(CWf.raise\fR, \f(CWf.raise_lower\fR, or
.mc |
\f(CWf.lower\fR.  (Functions are explained and defined in Chapter 14.)
.mc
.LE
.iX "icons"
.H 2 "Introduction to Icons"
Icons are small graphic representations of windows that are used
to reduce clutter on the screen.  A primary client window can be
.mc |
turned into an icon and restored to window form when desired.  Icons
.mc
can be arranged on the
screen in a pattern you select, or in the default pattern (bottom
left to right).
.P
.iX "icon box"
Icons can be placed into
an "icon box."  The icon box is an MWM window, similar to other
windows, that holds icons.  The icon box itself can be
minimized (turned into an icon).
.P
Information on using and configuring icons and the icon box is
.mc |
found in Chapter 15.
.mc
.P
The following chapters discuss communications between your client
and MWM, how a user can configure MWM resources and functions (which might
affect your client application), and how to work
with icons.
