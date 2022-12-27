.mc |
.\"  (c) Copyright 1989, 1990 OPEN SOFTWARE FOUNDATION, INC.
.mc
.\"  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
.\"  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
.\"  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
.\"  (c) Copyright 1988 MICROSOFT CORPORATION
.\"  ALL RIGHTS RESERVED
.mc |
.nr H1 12
.mc
.H 1 "Managing Windows With MWM"
.P
.iX "Presentation Manager, Microsoft"
.iX "Microsoft's Presentation Manager"
.iX "X11 client"
This chapter presents information users need if they want to customize the
appearance and behavior of MWM windows.  Although you, as a programmer,
will not usually be setting values for MWM resources, knowing about the
resources can help you avoid user-caused problems in your application.  
.P
MWM is an X11 client that
manages the appearance and behavior of objects on the workspace (screen).
MWM and its management operations are controlled by using a mouse, a keyboard,
.mc |
and a functional window frame as they are used by the Microsoft\(rg
.mc
Presentation Manager.
.mc |
.br
.ne 17
.mc
.P
.mc |
Chapter 13 discusses the following topics:
.mc
.BL
.LI
.mc |
Starting up MWM
.mc
.LI
.mc |
Using resources to configure MWM
.mc
.LI
.mc |
Setting focus policies for active windows
.mc
.LI
.mc |
Managing clients
.mc
.LI
.mc |
Managing the general appearance of window frames and mattes
.mc
.LI
.mc |
Specifying a different font for the window manager
.mc
.LI
.mc |
Coloring window frames
.mc
.LI
.mc |
Making window frame pixmaps
.mc
.LI
.mc |
Specifying a matte for client windows
.mc
.LI
.mc |
Using frameless or reduced-element windows
.mc
.LI
.mc |
Controlling window size and position
.mc
.LE
.H 2 "Starting Up MWM"
.mc |
MWM clients receive configuration information from the configuration
resource files
described in preceding chapters.  
Users can copy to their home directory
as \fB.mwmrc\fR
the file \fB/usr/lib/X11/system.mwmrc\fR.
By editing \fB.mwmrc\fR
and their \fB.Xdefaults\fR file, they can customize the 
window manager to fit their needs. 
.mc
.iX "home directory"
.mc |
.iX ".Xdefaults"
.mc
.iX ".mwmrc"
.P
.mc *
.mc
.iX "/usr/lib/X11/system.mwmrc"
.iX "configuration files" "/usr/lib/X11/system.mwmrc"
.mc *
.mc
.P
.mc |
MWM is often started by a display or session manager.
The following syntax is used to specify display information and start-up resources:
.mc
.P
.mc *
.mc
.iX "display option"
.iX "xrm"
.mc |
\fBmwm\fP  -\fBdisplay\fP \fIhost:display.screen\fR -\fBxrm\fP \fIresourcestring\fR
-\fBmultiscreen\fP -\fBname\fP \fIname\fR -\fBscreens\fP \fIname\fP [\fIname\fP [...]]
.mc
.P
.VL 1i
.mc |
.LI \-\fBdisplay\fP
Specifies the display to use
.LI \-\fBxrm\fP
Enables the named resources when starting MWM
.LI \-\fBmultiscreen\fP
Causes MWM to manage all screens on the display; the default is to
manage one screen
.LI \-\fBname\fP
Causes MWM to retrieve its resources using the
specified name, as in \fIname*resource\fP
.LI \-\fBscreens\fP
Specifies the resource names to use for the screens managed
by MWM
.mc
.LE
.P
.mc |
The \fB-display\fR option has the following syntax:
.mc
.VL 1i
.LI \fIhost\fR
\fRSpecifies the hostname of a valid system on the network.
Depending on the
.mc |
situation, this could be the hostname of the
user's, or the hostname of a
.mc
remote system.
.LI \fIdisplay\fR
.mc |
\fRSpecifies the number (usually 0) of the display on the system on which 
.mc
the output is to appear.
.LI \fIscreen\fR
.mc |
\fRSpecifies the number of the screen where the output is to appear.
.mc
This
.mc |
number is 0 for single-screen systems.
.mc
.LE
.iX "resource, definition"
.iX "configuring" "MWM"
.H 2 "Using Resources to Configure MWM"
.mc |
The appearance and behavior of windows, window frames, menus, and icons are
.mc
controlled with \fBresources\fR.  Resources are always named for the
elements they affect, such as \f(CWfontList\fR for setting the font.  Users
do not need to specify values for any resources; MWM can run with its
default appearance and behavior.  However, to customize the
.mc |
appearance and/or behavior of MWM, users need to specify values for 
some resources.
.br
.ne 5i
.P
.mc
.H 3 "The Hierarchy of Resource Configuration Files"
.mc |
The following diagram illustrates the path MWM travels when looking for
.mc
resource values:
.iX "hierarchy of resource configuration files"
.iX "configuration files" "hierarchy of resource"
.P
.mc |
.in0
.mc
.TS
tab(;);
c  c  c  .
.mc |
\fBmwm\fP Command Line Option;;
.mc
.iX "Command Line Option"
.iX "XENVIRONMENT"
.iX "$HOME/.Xdefaults-host"
.iX "RESOURCE_MANAGER"
.iX "property" "RESOURCE_MANAGER"
.iX "$HOME/.Xdefaults"
.mc *
.mc
.iX "/usr/lib/X11/app-defaults/Mwm"
.iX "Internal Application Defaults"
\(da;;
.mc |
XENVIRONMENT;if set, else \(->;\f3$HOME/.Xdefaults-\fP\f2host\fP
.mc
\(da;;
.mc |
RESOURCE_MANAGER property;if set, else \(->;\f3$HOME/.Xdefaults\fP
;;if exists, else
.mc
;;\(da
.mc |
;;\f3$HOME/Mwm\fP
;;\(da
;;if exists, else
;;\(da
;;\f3/usr/lib/X11/app-defaults/Mwm\fP
;;\(da
;;Internal Application Defaults
.mc
.TE
.mc |
.in
.mc
.P
.mc |
The filenames \fB$HOME/Mwm\fP and \fB/usr/lib/X11/app-defaults/Mwm\fP
represent the customary locations for these files.
The actual locations may depend on several environment variables and the
current language environment.
For more information see the ``Localization'' section in Chapter 8.
.mc
.P
Entries in the resource database may refer to other resource
.mc |
files, such as \fB.mwmrc\fP, that
.mc
contain specific types of resources.  This includes files that contain
.mc |
bitmaps, fonts, and MWM-specific resources such as menus and behavior
specifications (for example, button and key
.mc
bindings).
.P
.iX "classification of resources"
.H 3 "Classification of Resources"
.mc |
MWM uses the three types of resources described in the following sections.
.mc
.H 4 "Component Appearance Resources"
.iX "component appearance resources"
.iX "resources" "component appearance"
These resources are used to specify appearance attributes of
window manager user interface components.  They can be applied to
the appearance of window manager menus, feedback windows, client
window frames, and icons.  Among the resources of this type are those that
.mc |
set frame colors (for example, background, foreground, shadows).
.mc
.H 4 "Specific Appearance and Behavior Resources"
.iX "specific appearance and behavior resources"
.iX "behavior resources" "specific appearance"
These resources are used to specify MWM appearance and behavior
.mc |
(for example, colormap and keyboard input focus policies).  They are not set
separately for different MWM user-interface components.
.mc
.H 4 "Client Specific Resources"
.iX "client specific resources"
.iX "resources" "client specific"
These MWM resources can be set for a particular client window or
class of client windows.  They specify client-specific icon and
.mc |
client window frame appearance and behavior (for example, client decoration and
.mc
client functions).
.P
.iX "mwm" "resource name"
.iX "resource name" "mwm"
.iX "Mwm" "resource class"
.iX "resource class" "Mwm"
.iX "~/"
.mc |
.iX "HOME environment variable"
.mc
.iX "order of precedence" "resource specifications"
Resource identifiers can be either a resource name beginning with
.mc |
a lowercase letter, \f(CWforeground\fR, or a resource
class beginning with an uppercase letter, \f(CWForeground\fR.  The
.mc
resource class for MWM is \fBMwm\fR and the resource name is \fBmwm\fR.
.P
If the value of a resource is a filename and
if the filename is prefixed by  ~/ , then it is relative to the path
.mc |
contained in the HOME environment variable (generally the user's
home directory).
.mc
.H 3 "Order of Precedence"
In general, a more detailed specification takes precedence over a less
.mc |
detailed specification.  For example, suppose the \fB.Xdefaults\fR file
includes the following lines:
.mc
.P
.TS
tab(;);
l l l l .
;\f(CW*Foreground:;;red
;XClock*Foreground:;;DarkSlateGray
;XClock*foreground:;;coral
;XClock*hands:;;green\fR
.TE
.mc |
The first line makes all resources of the class \f(CWForeground\fP red.  The second
.mc
line overrules the first line, but only in the case of clients of class
.mc |
\f(CWXclock\fP (of which there is only one \(em the xclock itself).  Line two
specifies that when the client is \f(CWXClock\fP, all resources of the class
\f(CWForeground\fP are DarkSlateGray.  Lines
three and four give \f(CWXclock\fP clients a coral foreground and green hands,
while the other \f(CWXClock\fP resources (if there were any) of class
\f(CWForeground\fP (pointerColor, cursorColor, foreground and
.mc
bottomShadowColor of the softkeys, and foreground and bottomShadowColor of
.mc |
the scroll bars) would remain
.mc
DarkSlateGray.
.iX "syntax for declaring resources"
.iX "resources" "syntax"
.H 3 "The Syntax for Declaring Resources"
The syntax used to declare a resource depends on the type of the
.mc |
resource.  The following shows one example for each type of resource:
.mc
.P
Component appearance resource:
.P
.mc |
\f(CWMwm*menu*foreground:   white\fR
.mc
.P
Specific appearance and behavior resource:
.P
.mc |
\f(CWMwm*useIconBox:   True\fR
.mc
.P
Client specific resource:
.P
.mc |
\f(CWMwm*XClock*clientDecoration:   None\fR
.mc
.P
More examples for each type of resource are presented at
.mc |
the end of the section in which the resources are discussed.
See Chapter 16 for quick reference tables of resources.
.br
.ne 3.5i
.H 2 "Managing Screens"
The following resources control management of single or multiple screens
on the display:
.iX "resource" "multiScreen"
.iX "multiScreen resource"
.P
\fBmultiScreen\fP.  This resource, if True, causes MWM to manage
all the screens on
the display.
If False, MWM manages only a single screen.
The default value is False.
.P
\fBscreens\fP.  This resource specifies the resource names to use for
the screens
mananged by MWM.
If MWM is managing a single screen, only the first name in the
list is used.
If MWM is managing multiple screens, the names are assigned to the
screens in order, starting with screen 0.
Screen 0 gets the first name, screen 1 the second name, and so on.
The default screen names are 0, 1, and so on.
.br
.ne 3.5i
.mc
.H 2 "Setting Focus Policies for Active Windows"
.iX "focus policy resources"
.P
.ne 5c
.mc |
Two focus policy resources allow the user to choose the
.mc
colormap and
.mc |
keyboard input focus policies \f(CWcolormapFocusPolicy\fP 
and \f(CWkeyboardFocusPolicy\fP:
.mc
.P
.iX "colormapFocusPolicy"
.iX "resource" "colormapFocusPolicy"
.H 3 "Colormap Focus Policy"
\fBcolormapFocusPolicy.\fR  The colormap focus policy can be set using the
\f(CWcolormapFocusPolicy\fR resource and one of the following three values:
.P
.VL 1.5i
.iX "keyboard value" "colormapFocusPolicy"
.iX "colormapFocusPolicy" "keyboard value"
.LI \f(CWkeyboard\fR
.mc |
\fR(Default value) The window manager tracks keyboard input and
.mc
installs a client's colormap when the client window gets the
keyboard input
focus.
.LI \f(CWexplicit\f(CW
.iX "explicit value" "colormapFocusPolicy"
.iX "colormapFocusPolicy" "explicit value"
\fRThe window manager tracks a specific focus-selection action
and installs
a client's colormap when the focus-selection action is done
in the client window.  The focus-selection action can be a key press, a
mouse button action, or a menu selection.  Key and mouse button actions
must be created with bindings.  (See "Mouse Button
.mc |
Bindings" and "Keyboard Bindings" in Chapter 14.)
.mc
.LI \f(CWpointer\fR
.iX "pointer value" "colormapFocusPolicy" 
.iX "colormapFocusPolicy" "pointer value"
\fRThe window manager tracks the pointer and installs a client's
colormap when
the pointer moves into the client window or the window frame
around the client.
.LE
.H 4 "Using Multiple Colormaps"
.iX "colormaps" "multiple"
MWM does all colormap installation based on the colormap focus
policy and
.mc |
any hints from the client about which colormap the client wants
installed.  Clients that require only one colormap at a time
can indicate the colormap they want installed by setting the
.mc
colormap
.mc |
attribute of the clients top-level window to that colormap.
.br
.ne 10
.mc
.P
.iX "WM_COLORMAP_WINDOWS" "and multiple colormaps"
.iX "f.next_cmap function" "and multiple colormaps"
Clients that have subwindows with colormaps that differ from that
of the
top-level window or that would like to have more than one
colormap installed
must use the \f(CWWM_COLORMAP_WINDOWS\fR property on the
top-level window.
Subwindow colormaps for the window with the colormap focus can be
installed
interactively by binding the \f(CWf.next_cmap\fR function to a
button, key, or
menu selection.  The user can then cycle through (shuffle) the
subwindow
colormaps.
.H 3 "Keyboard Focus Policy"
.iX "keyboardFocusPolicy"
.iX "resource" "keyboardFocusPolicy"
.iX "explicit value" "keyboardFocusPolicy"
.iX "keyboardFocusPolicy" "explicit"
.iX "click-to-type"
.iX "explicit-listener"
\fBkeyboardFocusPolicy\fR.  Use the \f(CWkeyboardFocusPolicy\fR resource to
specify the conditions under which you want a window to become active and 
the focus of keyboard input:
.P
.VL 1.5i
.LI \f(CWexplicit\fR
.mc |
\fR(Default value)  The window manager tracks a specific
.mc
focus-selection action and sets the
keyboard focus to a client window when the focus-selection
action is done in that client window.  Keyboard input goes to the selected
window regardless of the
location of the pointer until the user explicitly selects another
window
.mc |
for keyboard-input focus.  The default focus-selection action is a mouse
.mc
Button 1 press.  The focus-selection action can be a key press or a
different mouse button action, but these must be configured using bindings.
(See "Mouse Button
.mc |
Bindings" and "Keyboard Bindings" in Chapter 14.)  Explicit selection is also
.mc
known by the terms "click-to-type" and "explicit-listener."
.iX "pointer value" "keyboardFocusPolicy"
.iX "keyboardFocusPolicy" "pointer"
.iX "track pointer"
.iX "real-estate driven"
.iX "tracked-listener"
.LI \f(CWpointer\fR
\fRThe window manager tracks the pointer and sets the keyboard focus
to a
client window when the pointer moves into that window or the
window frame around the client.  The \f(CWpointer\fR policy is also known
by the terms "track pointer," "real-estate driven," and
"tracked-listener."
.LE
.iX "resource" "autoKeyFocus"
.iX "autoKeyFocus resource"
.P
.mc |
\fBautoKeyFocus.\fR  This resource is available only when the keyboard input
focus policy is explicit.  If the \f(CWautoKeyFocus\fR resource is given
.mc
a value of
.mc |
True, then when a window with the keyboard input focus is withdrawn from
.mc
window management or is iconified, the focus is set to the window
that previously had the focus.  If there was no previous window, or if
.mc |
the value given is False, then the user must select a window or
icon to receive the focus.  The default value is True.
.mc
.P
.iX "autoRaiseDelay resource"
.iX "resource" "autoRaiseDelay"
.mc |
\fBautoRaiseDelay.\fR  This resource is available only when the value for the
\f(CWfocusAutoRaise\fR resource is True and the keyboard focus policy is
pointer.  The \f(CWautoRaiseDelay\fR resource
.mc
specifies the amount of time (in milliseconds) that MWM
.mc |
waits before raising a window after it gets the keyboard focus.
The default value is 500 msec.
.mc
.P
.iX "resource" "deiconifyKeyFocus"
.iX "deiconifyKeyFocus resource"
.mc |
\fBdeiconifyKeyFocus.\fR  This resource is available only when the keyboard
input focus policy is explicit.  If the \f(CWdeiconifyKeyFocus\fR resource
.mc
is given a value of
.mc |
True, a window receives the keyboard input focus when it is
.mc
normalized (changed from an icon to a normal window).  The default value is
.mc |
True.
.mc
.P
.iX "resource" "enforceKeyFocus"
.iX "enforceKeyFocus resource"
.iX "globally active input windows"
\fBenforceKeyFocus.\fR  If the \f(CWenforceKeyFocus\fR resource value is
.mc |
True,
.mc
then the keyboard input focus is always explicitly set to selected windows
even if there is an indication that they are "globally active" input
.mc |
windows.  If the resource value is False, the keyboard input focus is not
explicitly set to globally active windows.  The default value is True.
.mc
(An example of a globally active client window would be a window with scroll
bars that allows users to scroll the window without disturbing the input
focus.)
.P
.iX "resource" "focusAutoRaise"
.iX "focusAutoRaise resource"
\fBfocusAutoRaise.\fR  When the value of the
.mc |
\f(CWfocusAutoRaise\fR resource is True, clients are
raised when they get the keyboard-input focus.  If
the value is False,  the stacking of windows on the display is
not changed when a window gets the keyboard input focus.
If \f(CWkeyboardFocusPolicy\fP is explicit, the default value
is True.
If \f(CWkeyboardFocusPolicy\fP is pointer, the default value
is False.
.mc
.P
.iX "passButtons resource"
.iX "resource" "passButtons"
\fBpassButtons.\fR  The \f(CWpassButtons\fR
.mc |
resource indicates whether button press events are passed to
.mc
clients after they are used to do a window manager function in the "client"
context.
.mc |
If the resource value is False, the button press is not passed
.mc
to the client.
.mc |
If the value is True, the button press is passed to the
client window.  The window manager function is done in either case.
The default value is False.
.mc
.iX "passSelectButton resource"
.iX "resource" "passSelectButton"
.P
.mc |
\fBpassSelectButton\fR.  This resource indicates whether or not to pass
the select button press events
to clients after they are used to do a window manager function in the
client context.  If the resource value is False, then the button press will
not be passed to the client.  If the value is True, the button press is
passed to the client window.  The window manager function is done in either
case.  The default value for this resource is True.
.mc
.P
.iX "resource" "startupKeyFocus"
.iX "startupKeyFocus resource"
.mc |
\fBstartupKeyFocus.\fR  This resource is available only when the keyboard input
.mc
focus policy is "explicit."  If the \f(CWstartupKeyFocus\fR resource is
given the
.mc |
value of True, a window gets the keyboard input focus when the window
is mapped (that is, initially managed by the window manager).  The default
value is True.
.mc
.H 3 "The Syntax of Focus Policy Resources"
Focus policy resources have the following syntax:
.iX "syntax" "focus policy resources"
.iX "focus policy resources" "syntax"
.P
.TS
tab(;);
l l l .
.mc |
\fRMwm*\fP\fIfocusPolicyResource\fR:;\fIpolicy\fR
.mc
.TE
.P
For example, by adding
.mc |
the following line to your \fB.Xdefaults\fR file, you could
.mc
change the \f(CWkeyboardFocusPolicy\fR so that
moving
the pointer into a window moves the keyboard input focus there (no other
action is needed):
.P
.TS
tab(;);
l l l .
.mc |
\f(CWMwm*keyboardFocusPolicy:;pointer\fR
.mc
.TE
.H 2 "Managing Clients"
.mc |
Two unique resources allow the user to refine management of clients:
\fBquitTimeout\fR sets the amount of time MWM waits for a client to
save information and close; \fBsaveUnder\fR controls the repainting of
.mc
windows that are uncovered after being obscured.
.P
.iX "quitTimeout resource"
.iX "resource" "quitTimeout"
\fBquitTimeout.\fR  The \f(CWquitTimeout\fR resource specifies
.mc |
the amount of time (in milliseconds) that MWM waits for a
.mc
client to update the WM_COMMAND property after it has sent the
.mc |
WM_SAVE_YOURSELF message.  This protocol is used only for
.mc
those clients that have a WM_SAVE_YOURSELF atom in the
WM_PROTOCOLS client window property.  (Refer to the sections on each
.mc |
property in Chapter 12.)  The default time is 1000 msec.
.mc
.P
.iX "saveUnder resource"
.iX "resource" "saveUnder"
\fBsaveUnder.\fR  The \f(CWsaveUnder\fR resource is used to indicate
whether "save unders" are used for MWM components.  For this to have any
effect, save unders must be implemented by the X server.  If save unders
.mc |
are implemented, the X server saves the contents of windows obscured by
.mc
windows that have the save under attribute set.  If the \f(CWsaveUnder\fR
.mc |
resource has a value of True, MWM sets the save under attribute on
.mc
the window manager frame for any client that has it set.  If the value
.mc |
is False, save unders are not used on any window manager frames.  The
default value is False.
.mc
.iX "window frames" "configuring the general appearance and behavior"
.iX "frames" "configuring the general appearance and behavior"
.mc *
.mc
.H 2 "Managing the Appearance and Behavior of Window Frames and Mattes"
The appearance and behavior of window frames and mattes in the user's
.mc |
environment are controlled by editing the user's 
\fB\&.Xdefaults\fR file.
.mc
.P
.mc |
.iX "configuring window frame appearance and behavior"
.mc
The following aspects of the appearance and behavior of window frames
and mattes are under the user's control:
.P
.VL 1.25i
.LI Font
The style (including size) of the text characters in the title
area, menus, and icon labels.
.LI Color
The color of foreground, background, top shadow, bottom shadow,
and side shadows.
.LI Pixmap
The mixture of foreground and background color that
composes the
pattern of the window frame or matte surface.  (This is particularly useful
for monochrome displays.)
.LI Elements
The number of frame pieces used to decorate a client window.
.LI Size
The maximum size a window can be in certain situations.
.LI Position
The initial position of a window on the screen and whether it can be moved 
interactively.
.LE
.P
The rest of this chapter describes the resources, their values, and any special
syntax that applies.
.iX "fontList resource"
.iX "resource" "fontList"
.mc |
.br
.ne 3i
.mc
.H 2 "Specifying a Different Font for the Window Manager"
.mc |
\fBfontList.\fR  The default font for the text of MWM
is the fixed font.
.mc
However, you can use the \f(CWfontList\fR resource to specify a different
.mc |
font if you desire.  The \f(CWfontList\fR resource can use any valid X11 R4
.mc
font description as its value.  Keep in mind that the font size affects the
size of
.mc |
window frames, icons and menus.  
.mc
.iX "coloring" "frame elements"
.iX "window frame" "coloring"
.iX "/usr/lib/X11/rgb.txt"
.H 2 "Coloring Window Frames"
Any of the standard X11 colors listed in
.mc |
\fB/usr/lib/X11/rgb.txt\fR can be used 
.mc
to color frame elements.  Additionally, custom colors can be created 
using hexadecimal values.  Frame elements and resources exist
for
inactive windows (any window not having the current keyboard
focus) and
for the active window (the window having the current keyboard
focus).  This
enables the user to distinguish the active window by giving it special
"active window" colors.
.iX "frame elements" "illustration"
.ig ++
.P
.br
.ie \nZ \{\
.br
.ne 27P
.P
.sC graphics/matfrm.tif tiff 300 27P 33.5P
\&
.P
.sp +1
.FG "MWM Frame Showing Frame Elements and Matte"
.sp +1.5
.\}
.el \{\
.br
.ne 27P
\&
.sp 25.5P
.ce
illustration graphics/matfrm.eps goes here
.sp +1
.FG "MWM Frame Showing Frame Elements and Matte"
.sp +1.5
.\}
.P
.++
.br
.mc |
.ne 4.5i
.mc
\0
.mc |
.FG "MWM Frame Showing Frame Elements and Matte"
.sp 4i
.in +.05i
.mc
.P! graphics/matfrm.ps 
.mc |
.in -.05i
.mc
.sp
.mc *
.mc
.P
.iX "coloring" "frame elements, default values for color display"
The default values for color resources are based on the visual type of the
.mc |
screen (for example, monochrome, 8-bit pseudocolor, and so on) and 
the values given to related resources.
.mc
The following table indicates default values
.mc |
for a color display.  On a monochrome display the default value for 
these resources is White.
.mc
.P
.mc |
.ne 3i
.mc
.TB "Default Values for Appearance on a Color Display"
.TS
tab(~),center,allbox;
lb lb
l l.
Resource Name~Resource Value
_
Mwm*activeBackground~CadetBlue
Mwm*activeBackgroundPixmap~NULL
Mwm*activeBottomShadowPixmap~NULL
Mwm*activeTopShadowPixmap~NULL
.mc |
Mwm*background~LightGrey
.mc
Mwm*backgroundPixmap~NULL
Mwm*bottomShadowPixmap~NULL
Mwm*topShadowPixmap~NULL
.TE
.iX "menu colors" "default values"
.iX "coloring menus" "default values"
.iX "widgets" "menu"
.P
The default menu colors (for various types of displays)
are set by the OSF/Motif menu widgets.
The value of the \f(CWbackground\fR resource is used to
.mc |
generate default values for the other color resources so that there
is a pleasing 3-dimensional effect.
.mc
The following rules are used by MWM in generating default values for color
resources:
.P
.BL
.LI
A top shadow color is generated by proportionally lightening the
associated background color.
.LI
A bottom shadow color is generated by proportionally darkening
the associated background color.
.LI
.mc |
The foreground color is black or white depending on the background
color.
.mc
.LE
.mc |
.br
.ne 4i
.mc
.iX "coloring" "frame elements, default values for monochrome display"
.iX "monochrome display" "coloring frames"
.P
.mc *
.mc
The following table indicates default values for appearance resources on
.mc |
a monochrome display.
.sp
.mc
.TB "Default Values for Appearance on a Monochrome Display"
.TS
tab(~),center,allbox;
lb lb
l l.
Resource Name~Resource Value
_
Mwm*activeBackground~White
Mwm*activeBackgroundPixamp~NULL
Mwm*activeBottomShadowPixmap~NULL
Mwm*activeTopShadowPixmap~50_foreground
Mwm*background~White
Mwm*backgroundPixmap~75_foreground
Mwm*bottomShadowColor~Black
Mwm*bottomShadowPixmap~NULL
Mwm*topShadowColor~White
Mwm*topShadowPixmap~50_foreground
.TE
.P
.iX "XmInstallImage(3X)"
.iX "OSF/Motif widget library (Xm)"
The standard pixmap names in the OSF/Motif widget library (Xm) can be
used in specifying pixmap resources such as \f(CWtopShadowPixmap\fR (refer to
.mc |
\fBXmInstallImage\fP).
.br
.ne 25
.mc
.H 3 "Coloring Individual Frame Elements"
The following table lists the individual elements of inactive and
active
window frames, and the resources that control their color:
.P
.iX "resource" "background"
.iX "resource" "topShadowColor"
.iX "resource" "bottomShadowColor"
.iX "resource" "foreground"
.iX "resource" "activeBackground"
.iX "resource" "activeTopShadowColor"
.iX "resource" "activeBottomshadowColor"
.iX "resource" "activeForeground"
.iX "background resource"
.iX "topShadowColor resource"
.iX "bottomShadowColor resource"
.iX "foreground resource"
.iX "activeBackground resource"
.iX "activeTopShadowColor resource"
.iX "activeBottomShadowColor resource"
.iX "activeForeground resource"
.P
.mc *
.mc
.TB "MWM Color Resources and What They Color"
.mc |
.in0
.mc
.TS
center, box, tab(;);
lb | lb | lb
lb | lb | ^
l | l | l .
Inactive Window and;Active Window and;Area Colored
Icon Resources;Icon Resources;
.mc |
_
.mc
foreground;activeForeground;Foreground areas. (text)
_
background;activeBackground;Background areas.
_
topShadowColor;activeTopShadowColor;Top and left 3-D bevels.
_
bottomShadowColor;activeBottomShadowColor;T{
Bottom and right 3-D bevels.
T}
.TE
.mc |
.in
.br
.ne 4
.mc
.P
Although the active and inactive color schemes can be made 
the same, it is usually more effective to pick contrasting color
schemes that make the active window readily distinguishable from
its inactive counterparts.
.iX "syntax" "for resources controlling the appearance of frame elements"
.mc |
.H 3 "Syntax for Resources Controlling Frame Appearance Element"
The following syntax in \fB.Xdefaults\fR is used
.mc
to specify the general appearance of frame elements:
.P
.TS
tab(;);
l l l .
.mc |
;\fBMwm*\fP\fIresource:     value\fR
.mc
.TE
.P
.mc |
For example, suppose the user wants the foreground and background of
.mc
inactive window
frames to be the reverse of the foreground and background of
the active window
frame, and chooses the colors SteelBlue for background and
VioletRed for
.mc |
.ne 10
foreground.  Also, the user wants 
to specify a different font.
To produce this appearance, 
the user puts the following lines in the \fB.Xdefaults\fR
file.  
.mc
.P
.ne 3c
.TS
tab(;);
l lw(2.5i) l .
\f(CW;Mwm*background:;SteelBlue
;Mwm*foreground:;VioletRed
;Mwm*activeBackground:;VioletRed
;Mwm*activeForeground:;SteelBlue
;Mwm*fontList:;6x10\fR
.TE
.mc |
.br
.ne .5i
.mc
.P
.mc |
The user can specify the appearance of window-frame
.mc
elements for
four particular objects.
.P
.BL
.LI
.mc |
Menus
.mc
.LI
.mc |
Icons (includes the frame elements of all icons)
.mc
.LI
.mc |
Clients (includes the frame elements of all clients)
.mc
.LI
.mc |
Feedback windows
.mc
.LE
.mc |
.br
.ne 10
.mc
.P
.mc |
This allows the user to select a different color or font
.mc
for a
particular object, perhaps menus, while the other objects (icons
and clients)
remain the same.  To do this, the following syntax is used:
.P
.TS
tab(;);
l l .
.mc |
\fBMwm*\fR{\fBmenu |icon |client |feedback\fP}*\fP\fIresource:  value\fR
.mc
.TE
.mc |
.br
.ne 10
.mc
.P
In addition, the user can specify resource values for the title area of a
client window and for menus with specific names:
.P
.TS
tab(;);
l l.
.mc |
\fBMwm*client*title*\fP\fIresource:   value\fR
.mc

.mc |
\fBMwm*menu*\fP\fImenuname*resource:   value\fR
.mc
.TE
.mc |
.br
.ne 20
.mc
.P
.mc |
For example, users who want the general appearance of the clients in
the environment to be as above, SteelBlue and VioletRed, but want the menus
to be different, can add the following lines to 
the \fB.Xdefaults\fR file:
.P
.mc
.TS
tab(;);
l lw(2.5i) l .
;\f(CWMwm*background:;SteelBlue
;Mwm*foreground:;VioletRed
;Mwm*activeBackground:;VioletRed
;Mwm*activeForeground:;SteelBlue
;Mwm*fontList:;6x10
;Mwm*menu*background:;SkyBlue
;Mwm*menu*foreground:;White\fR
.TE
.P
.P
Information on menus and icons appears in later chapters.
.iX "window frames" "changing the pixmap"
.iX "tiles" "definition"
.iX "monochrome display" "pixmap tiling"
.iX "pixmaps" "window frames"
.iX "pixmaps" "definition"
.H 2 "Making Window Frame Pixmaps"
.mc |
Making a pixmap is a way of creating shades of colors.  The pixmap is
composed of tiles that provide a surface pattern
.mc
or a visual texture. The concept is analogous to using ceramic tiles to make a
pattern or texture on a floor or
countertop.
.P
.mc |
Generally, the fewer colors a display
.mc
produces, the
more important the pixmap resource becomes.  This is because a pixmap
.mc |
provides a way to mix foreground and background colors into a
third color pattern.
.mc
.P
For example, with a monochrome display (two
.mc |
colors\(emblack and
white), one could use the pixmap resource to color window frame elements
.mc
with shades of
.mc |
gray to achieve a 3-dimensional look.
.br
.ne 5
.mc
.P
.mc |
The following table shows the MWM pixmap resources that let the user create a pattern for
.mc
the frame background and bevels of both inactive and active
windows:
.mc |
.br
.ne 7c
.mc
.iX "backgroundPixmap resource"
.iX "bottomShadowPixmap resource"
.iX "topShadowPixmap resource"
.iX "activeBackgroundPixmap resource"
.iX "activeBottomShadowPixmap resource"
.iX "activeTopShadowPixmap resource"
.iX "resource" "backgroundPixmap"
.iX "resource" "bottomShadowPixmap"
.iX "resource" "topShadowPixmap"
.iX "resource" "activeBackgroundPixmap"
.iX "resource" "activeBottomShadowPixmap"
.iX "resource" "activeTopShadowPixmap"
.P
.mc *
.mc
.TB "Creating a Pixmap for Window Frames"
.TS
box, tab(;) ;
lb | lbw(2.5i)
l | l .
Use this resource...; To pattern these elements...
.mc |
_
.mc
\f(CWbackgroundPixmap\fR;T{
Background and bevel for inactive frames.
T}
_
\f(CWbottomShadowPixmap\fR;T{
Right and bottom bevels of inactive frames.
T}
_
\f(CWtopShadowPixmap\fR;T{
Left and upper bevels of inactive frames.
T}
_
\f(CWactiveBackgroundPixmap\fR;T{
Background of the active frame.
T}
_
\f(CWactiveBottomShadowPixmap\fR;T{
Right and lower bevels of the active frame.
T}
_
\f(CWactiveTopShadowPixmap\fR;T{
Left and upper bevels of the active frame.
T}
.TE
.iX "Pixmaps" "illustration of values"
.P
.ne 11c
The following figure illustrates various pixmap values:
.ig ++
.P
.br
.ie \nZ \{\
.br
.ne 20.5P
.P
.sC graphics/tile6.pcl.tif tiff 300 20.5P 34P
\&
.P
.sp +1
.FG "Illustrations of Valid Pixmap Values"
.sp +1.5
.\}
.el \{\
.br
.ne 20.5P
\&
.sp 18.5P
.ce
illustration graphics/tile6.pcl.eps goes here
.sp +1
.FG "Illustrations of Valid Pixmap Values"
.sp +1.5
.\}
.P
.++
.br
.mc |
.ne 3.5i
.mc
\0
.mc *
.mc
.FG "Illustrations of Valid Pixmap Values"
.mc |
.sp 3.25i
.in +.31i
.P! graphics/tile6.ps 
.in -.31i
.br
.ne 6
.mc
.P
The following table lists the acceptable values for obtaining the
pixmap styles illustrated above:
.P
.ne 8.8c
.TB "Valid Pixmap Values"
.iX "pixmap" "valid values for"
.mc |
.TS H
center, box, tab(;) ;
lbw(3i) l lb
l | l .
To pattern an element in this manner...;Use this value...
.mc |
.TH
.mc
_
.mc |
The foreground color;foreground
.mc
_
.mc |
The background color;background
_
.mc
T{
.mc |
A mix of 25% foreground to 75% background
.mc
T};25_foreground
_
T{
.mc |
A mix of 50% foreground to 50% background
.mc
T};50_foreground
_
T{
.mc |
A mix of 75% foreground to 25% background
.mc
T};75_foreground
_
T{
In horizontal lines alternating between the foreground and
.mc |
background color
.mc
T};horizontal_tile
_
T{
In vertical lines alternating between the foreground and
.mc |
background color
.mc
T};vertical_tile
_
T{
In diagonal lines slanting to the right, alternating between the
foreground
.mc |
and background color
.mc
T};slant_right
_
T{
In diagonal lines slanting to the left, alternating between the
foreground
.mc |
and background color
.mc
T};slant_left
.TE
.P
.iX "resource" "cleanText"
.iX "cleanText resource"
\fBcleanText.\fR  The \f(CWcleanText\fR resource can be used to make text
easier to read on monochrome systems 
where a \f(CWbackgroundPixmap\fR is specified.  This resource controls the
display of window manager text in the title area and in feedback windows.
.mc |
If the default value of True is used, the text is drawn with a clear
.mc
background.  Only the background in the area
.mc |
immediately around the text is cleared.  If the value is False, the text is
.mc
drawn directly on top of the existing background.
.iX "mattes" "client windows"
.iX "windows" "mattes"
.iX "client windows" "mattes"
.iX "mattes" "definition"
.H 2 "Specifying a Matte for Client Windows"
OSF/Motif has "matte" resources that allow further decoration of windows.  
A \fBmatte\fR is
.mc |
a 3-dimensional border between the client's window area and the window frame.  
.mc
A matte can give an individual client, or class of clients, a
distinct appearance.  To configure a
matte, the \f(CWmatteWidth\fR resource must first be given a positive value.
.P
.mc |
.br
.ne 12
.mc
.iX "matteWidth resource"
.iX "resource" "matteWidth"
\fBmatteWidth\fR.  The \f(CWmatteWidth\fR resource defines the width of the
matte between
the client and the window frame.  The width is given in pixels.
For
example, to specify a matte of 10 pixels around all xload windows, 
.mc |
the following line must be in the \fB.Xdefaults\fR file:
.mc
.P
.TS
tab(;);
l l l .
;\f(CWMwm*XLoad.matteWidth:;10\fR
.TE
.P
.mc |
The default value of 0 disables the matte.
.mc
.iX "coloring" "matte elements"
.iX "matte elements" "coloring"
.H 3 "Coloring Individual Matte Elements"
Matte resources use the same wording as window frame resources,
but begin
.mc |
with the term "matte."  The following table 
lists the matte resources that control color.
.mc
.P
.iX "matteBackground resource"
.iX "matteTopShadowColor resource"
.iX "matteBottomShadowColor resource"
.iX "matteForeground resource"
.iX "resource" "matteBackground"
.iX "resource" "matteTopShadowColor"
.iX "resource" "matteBottomShadowColor"
.iX "resource" "matteForeground"
.TB "MWM Matte Resources and What They Color"
.TS
allbox, center, tab(;);
lb lb
l l .
Matte Resource;Area Colored
_
.mc |
matteBackground;Background areas
matteTopShadowColor;Top and left 3-dimensional bevels
matteBottomShadowColor;Bottom and right 3-dimensional bevels
matteForeground;Foreground areas
.mc
.TE
.iX "mattes" "changing the pixmap"
.iX "pixmap" "mattes"
.H 3 "Changing the Pixmap of Mattes"
.mc |
As with frame colors, the fewer colors a
.mc
display can
produce, the more value there is in creating a pixmap for mattes.
Again, a pixmap provides a way to "mix" foreground and
background
colors into a third color.  For a 2-color (monochrome)
display, one can 
create a pixmap for a window matte in shades of gray to achieve a
.mc |
pleasing 3-dimensional look.
.mc
.P
.ne 3.1c
.mc |
The following table lists the resources that are used to create a pixmap for mattes.
.mc
.P
.iX "matteBottomShadowPixmap resource"
.iX "matteTopShadowPixmap resource"
.iX "resource" "matteBottomShadowPixmap"
.iX "resource" "matteTopShadowPixmap"
.TB "MWM Resources for Creating a Pixmap"
.TS
center, box, tab(;) ;
lb | lb
l | l .
.mc
Use this resource...; To pattern these elements...
.mc |
_
matteBottomShadowPixmap;T{
Right and lower bevels of matte
.mc
T}
_
.mc |
matteTopShadowPixmap;T{
Left and upper bevels of matte
.mc
T}
.TE
.P
.P
.iX "mattes" "syntax"
.iX "syntax" "mattes"
.mc |
See Section 13.9 for a chart and an illustration of the values for pixmap resources.
.mc
.H 3 "The Syntax for Matte Resources"
Matte resources can have any of three syntaxes,
depending on
the situation.
.AL
.LI
.mc |
To matte all clients regardless of class, the syntax is
.mc
.TS
tab(;);
l l l .
.mc |
;   \fBMwm*\fP\fImatteResource\fR:;\fIvalue\fR
.mc
.TE
.mc *
.mc
For example, to create a 10-pixel-wide yellow matte for
.mc |
every client window, the user adds the following lines to 
the \fB.Xdefaults\fP file:
.mc
.TS
tab(;);
l l .
.mc |
;   \f(CWMwm*matteWidth:   10\fR
;   \f(CWMwm*matteBackground:   Yellow\fR
.mc
.TE
.mc |
.br
.ne 5
.mc
.LI
.mc |
To matte specific classes of clients, the syntax is
.mc
.TS
tab(;);
l l .
.mc |
;   \fBMwm*\fP\fIclientclass.matteResource\fR:   \fIvalue\fR
.mc
.TE
.LI
.mc |
To matte any client of an unknown class, the syntax is
.mc
.TS
tab(;);
l l l .
.mc |
;   \fBMwm*defaults*\fP\fImatteResource\fR:;\fIvalue\fR
.mc
.TE
.mc |
.br
.ne 5
.mc
.LI
.mc |
To matte a specific client, the syntax is
.mc
.TS
tab(;);
l l l .
.mc |
;   \fBMwm*\fP\fIclientname*matteResource\fR:;\fIvalue\fR
.mc
.TE
.LE
.iX "windows" "frameless and reduced-element"
.iX "frameless windows"
.iX "window elements" "adding or reducing"
.iX "reduced-element windows"
.iX "transientDecoration resource"
.iX "clientDecoration resource"
.iX "resource" "transientDecoration"
.iX "resource" "clientDecoration"
.H 2 "Using Frameless or Reduced-Element Windows"
.mc |
For some applications the full complement of window decorations may not be
.mc
desirable.  For example, a clock may not need resize frame handles.
.mc |
MWM has two resources for such situations:  \fBclientDecoration\fR
and \fBtransientDecoration\fR.
.mc
.P
.mc |
The \f(CWclientDecoration\fR resource allows the user to
choose how much decoration to put around each client.
.mc
The default value is "all."
.P
.mc |
The \f(CWtransientDecoration\fR resource
.mc
allows the user to choose how much or how little decoration to put
around each transient window.  (A \fBtransient window\fR is a relatively
short-lived window, for
example, a dialog box.)  The default value for this resource is "menu 
.mc |
title resizeh" (that is, transient windows have a title bar with a window
.mc
menu button and resize borders).
.mc |
Even if a decoration is specified by the \f(CWtransientDecoration\fR
resource, MWM does not put it around a transient window unless that
decoration is also specified by the \f(CWclientDecoration\fR resource.
.mc
.P
The user can still use any function associated with the window decoration 
removed either by binding the function to a button or to a key press, or by
.mc |
adding it to the window menu (as explained in Chapter 14).
.mc
.P
There are two sets of constraints when configuring window decoration:
.iX "_MOTIF_WM_HINTS" "and clientDecoration resource"
.iX "_MOTIF_WM_HINTS" "and transientDecoration resource"
.AL
.LI
.mc |
You, the applications programmer, can use the functions field of the
.mc
_MOTIF_WM_HINTS property to limit which MWM functions are to be
.mc |
used by the client window.  If a function is not to be applied to a 
window, the decoration that does that function is not shown, even if 
it is specified by the \f(CWclientDecoration\fP resource.  
.br
.sp
.ne .5i
The user can further limit the functions to be used by configuring the 
\f(CWclientFunctions\fP resource (or \f(CWtransientFunctions\fP 
resource if the window is a transient window).
.br
.ne 8
.mc
.iX "_MOTIF_WM_HINTS" "and clientFunctions resource"
.mc |
.sp
If the functions field of the _MOTIF_WM_HINTS client window property 
is set, it is combined with the \f(CWclientFunctions\fP resource value.
Functions can be removed but not added in doing this combination.
.sp
.mc
The default value for allowed functions (to be used if no specification
is done by the client or user) is that all functions are allowable.
.LI
The decoration specification for a window
is adjusted based on the limits set by the _MOTIF_WM_HINTS property and 
.mc |
the \f(CWclientDecoration\fR resource (as previously explained).  
.mc
Decorations that are associated with functions that are not allowed 
are not shown.
The \f(CWclientDecoration\fP resource
(or \f(CWtransientDecoration\fP resource if the window is a transient window)
is used to indicate the decoration specification for a window.
.mc |
.sp
.mc
If the decorations field of the _MOTIF_WM_HINTS client window property is set,
it is combined with the \f(CWclientDecorations\fR resource value.
.mc |
Decorations can be removed but not added in doing this
.mc
combination.
.mc |
.sp
.mc
The default decoration for the client window frame is
all title bar decorations and the resize frame handles.
.LE
.P
For more discussion of how the \f(CWclientFunctions\fR resource is used, see
.mc |
Section 14.3.4.  The _MOTIF_WM_HINTS property is discussed in Section 12.3.
.mc
.iX "syntax" "adding or removing frame elements"
.mc |
.br
.ne 15
.mc
.H 3 "Adding or Removing Elements"
The \f(CWclientDecoration\fR and \f(CWtransientDecoration\fR resources
are configured as a list of frame elements:
.BL
.LI
If the first element in the list is
.mc |
preceded by nothing or by a \f(CW+\fR (plus sign), the window manager starts
.mc
with no frame
and assumes that the list contains those elements to be added.
.LI
.mc |
If the list begins with a \f(CW-\fR (minus sign), the window manager
.mc
starts with a
complete frame and assumes that the list contains elements to be 
removed from the frame.
.LE
.P
.ne 3.5i
.mc |
The following table lists the valid window frame elements.
.mc
.P
.TB "Valid MWM Frame Elements"
.TS
center, box, tab(;) ;
lb | lbw(2.5i)
l | lw(2.5i) .
.mc
Frame Element;Description
.mc *
.mc
_
.mc |
all;Includes all decoration elements (default value).
_
.mc
maximize;Maximize button (includes title bar).
_
minimize;Minimize button (includes title bar).
_
none;No decorations.
_
resizeh;Resize frame handles (includes border).
_
border;Window border.
_
menu;Window menu (includes title bar).
_
title;Title bar (includes border).
.TE
.mc |
.br
.ne 3i
.mc
.iX "syntax" "clientDecoration"
.iX "syntax" "transientDecoration"
.iX "clientDecoration resource" "syntax"
.iX "transientDecoration resource" "syntax"
.P
.H 3 "The Syntax for clientDecoration and transientDecoration Resources"
The \f(CWclientDecoration\fR resource has three syntaxes:
.mc |
.BL
.mc
.LI
To add or remove
elements from
.mc |
all classes of clients, the syntax is
.sp
.mc
.TS
tab(;);
l l l .
.mc |
; \fBMwm*clientDecoration:\fP;\fIvalue\fR
.mc
.TE
.mc |
.sp
.mc
For example, remove the maximize button from all
windows by
.mc |
adding the following line to the \fB.Xdefaults\fR file:
.sp
.mc
.TS
tab(;);
l l l.
.mc |
; \f(CWMwm*clientDecoration:;\(mimaximize\fR
.mc
.TE
.mc |
.sp
.br
.ne 7
.mc
.LI
To add or remove elements
.mc |
from specific classes of clients, the syntax is
.sp
.mc
.TS
tab(;);
l l l .
.mc |
; \fBMwm*\fP\fIclientclass\fP\fB.clientDecoration:\fP;\fIvalue\fP
.mc
.TE
.mc |
.sp
.br
.ne 5
.mc
For example, to remove just the resize handles and the maximize
.mc |
button from all clocks displayed on the screen, add the following>:
.sp
.mc
.TS
tab(;);
l l l .
.mc |
; \f(CW Mwm*XClock.clientDecoration:;\(miresizeh \(mimaximize\fR
.mc
.TE
.mc |
.sp
.mc
.LI
To add or remove
elements from
.mc |
any client with an unknown class, the syntax is
.sp
.mc
.TS
tab(;);
l l l .
.mc |
; \fBMwm*defaults*clientDecoration:\fP;\fIvalue\fP
.mc
.TE
.mc |
.br
.ne 6
.sp
.mc
The \f(CWtransientDecoration\fR resource has the following
syntax:
.mc |
.sp
.mc
.TS
tab(;);
l l l .
.mc |
; \fRMwm*transientDecoration:\fP;\fIvalue\fP
.mc
.TE
.mc |
.sp
.mc
For example, remove the menu button from all transient
.mc |
windows by adding the following line to the \fB.Xdefaults\fR file:
.sp
.mc
.TS
tab(;);
l l l .
.mc |
; \f(CW Mwm*transientDecoration:;title resizeh\fR
.mc
.TE
.mc |
.br
.ne 10
.mc
.P
.iX "windows" "positioning"
.iX "sizing" "windows"
.iX "positioning windows"
.iX "windows" "sizing"
.H 2 "Controlling Window Size and Position"
MWM has a number of resources that allow the user 
to refine control of the size and position of windows.
.P
.mc |
.H 3 "Window Size Resources"
.mc
The following resources configure the size of windows:
.iX "resource" "frameBorderWidth"
.iX "frameBorderWidth resource"
.P
\fBframeBorderWidth.\fR  The \f(CWframeBorderWidth\fR resource specifies
the width (in pixels) of a client window frame border with shadow
.mc |
elements, but without resize
.mc
handles.  The default value is
.mc |
5 pixels.
.mc
.P
.iX "resource" "limitResize"
.iX "limitResize resource"
.iX "maximumClientSize resource" "and limitResize resource"
.iX "WM_NORMAL_HINTS" "and limitResize resource"
\fBlimitResize\fR.  The \f(CWlimitResize\fR resource
controls the ability to enlarge a window beyond the client's maximized
.mc |
size.  The default value of True limits a window's
.mc
size to no greater than the maximum size
specified by the \f(CWmaximumClientSize\fR resource, or the
\f(CWWM_NORMAL_HINTS\fR
window property, or the default maximum size assigned by MWM.  The
.mc |
value of "False" allows a window to be
.mc
resized to
any size.
.iX "resource" "maximumClientSize"
.iX "maximumClientSize resource"
.iX "WM_NORMAL_HINTS" "and maximumClientSize resource"
.iX "maximumClientSize resource" "and WM_NORMAL_HINTS"
.P
\fBmaximumClientSize\fR.  The \f(CWmaximumClientSize\fR resource
controls the maximum size of a maximized client.
Its value is a width \(mu height, interpreted in terms of the units that
the client uses.  If this resource
.mc |
is not specified, the maximum size is taken from the \f(CWWM_NORMAL_HINTS\fR
.mc
window property, or the default size (the size of the screen) is
used.
.mc |
.br
.ne 6
.mc
.iX "maximumMaximumSize resource"
.iX "resource" "maximumMaximumSize"
.P
\fBmaximumMaximumSize\fR.  The \f(CWmaximumMaximumSize\fR resource controls
the maximum size of a client window as set by the
client.  The dimensions are given in pixels.
The default value of this resource is twice the screen width and height.
.iX "resource" "resizeBorderWidth"
.iX "resizeBorderWidth resource"
.P
\fBresizeBorderWidth\fR.  The \f(CWresizeBorderWidth\fR resource
.mc |
specifies the width of a client window frame border with resize
.mc
handles and shadow elements.  The default value is 10 pixels.
.iX "resource" "resizeCursors"
.iX "resizeCursors resource"
.P
\fBresizeCursors\fR.  The \f(CWresizeCursors\fR resource
indicates whether the resize cursors are displayed when
the pointer is in the window resize border.  
.mc |
The default value True causes the appropriate resize cursor to appear
.mc
when the pointer
enters a resize handle in the window frame.
.mc |
The value of False prevents resize cursors from being displayed.
.mc
.P
.mc |
.H 3 "Window Position Resources"
.mc
The following resources configure the position of windows:
.iX "resource" "clientAutoPlace"
.iX "clientAutoPlace resource"
.P
\fBclientAutoPlace.\fR  The \f(CWclientAutoPlace\fR resource determines the
position of a window when the window has not been given a user specified
.mc |
position.  The default value of True positions a window with the top
.mc
left corner of the frame offset horizontally and vertically.  The value of
.mc |
False causes the currently configured position of the window to be
used.  In either case, MWM attempts to place the window totally on the screen.
.mc
.iX "interactivePlacement resource"
.iX "resource" "interactivePlacement"
.P
\fBinteractivePlacement\fR.  The \f(CWinteractivePlacement\fR resource
controls the initial placement of new windows on the screen.
.mc |
The value of True changes the shape of the pointer (to an
.mc
upper-left-corner bracket) 
before a new window displays, so a position can be chosen for
.mc |
the window.  When the default value of False is used, the window is
.mc
placed according to its initial configuration attributes and/or the values of
.mc |
other MWM resources (for example, \f(CWclientAutoPlace\fR).
.mc
.iX "resource" "moveThreshold"
.iX "moveThreshold resource"
.P
\fBmoveThreshold\fR.  The \f(CWmoveThreshold\fR resource
controls the sensitivity of dragging operations.
The value of the \f(CWmoveThreshold\fR resource is the number
of pixels that
the pointer must be moved with a button pressed before a drag
operation is
initiated.  This resource is used to prevent window or icon
movement
when the user unintentionally moves the pointer during a click or
double-click action.  The default value is 4 pixels.
.mc |
.br
.ne 8
.mc
.iX "resource" "positionIsFrame"
.iX "positionIsFrame resource"
.iX "positionIsFrame resource" "and WM_NORMAL_HINTS"
.iX "WM_NORMAL_HINTS" "and positionIsFrame resource"
.P
\fBpositionIsFrame\fR.  The \f(CWpositionIsFrame\fR resource
.mc |
determines how client window-position information is reported.  When the
default value True is used, the position information (from
.mc
\f(CWWM_NORMAL_HINTS\fR and
configuration
.mc |
files) refers to the position of the window frame.
When the value is False,
.mc
the position information refers to the position of the client
window itself.
.mc |
.br
.ne 8
.mc
.iX "resource" "positionOnScreen"
.iX "positionOnScreen resource"
.P
\fBpositionOnScreen\fR.  The \f(CWpositionOnScreen\fR resource
controls clipping of new windows by screen edges.
.mc |
The default value True causes a window to be placed, if possible, so
.mc
that it is
not clipped.  If clipping cannot be avoided, a window is placed so that at
least the upper
left
corner of the window is on the screen.
.mc |
The value of False causes a window to be placed at the requested
.mc
position even if it is totally off the screen.
.mc |
.br
.ne 8
.mc
.iX "resource" "showFeedback"
.iX "showFeedback resource"
.P
\fBshowFeedback.\fR  The \f(CWshowFeedback\fR resource controls when
feedback information is displayed.  It controls both window position and
size feedback during move or resize operations and initial client
.mc |
placement.  It also controls window manager message and dialog boxes.
.P
The
.mc
value for this resource is a list of names of the feedback options to be
.mc |
enabled or disabled; the names must be separated by a space.
If an option is preceded by a minus sign, that option is
excluded from the list.
The \fIsign\fR of the first item in the list
determines the initial set of options.
If the sign of the first
option is minus, \fBmwm\fP assumes all options are present and starts
subtracting from that set.
If the sign of the first decoration is plus (or not
specified), \fBmwm\fP starts with no options and builds up a list from
the resource.
.mc
.P
.mc |
The following table lists the names of the feedback options.
.br
.ne 24
.P
.mc
.TB "Feedback Options"
.TS
box, center, tab(;);
lB | lB
l  | l  .
Name;Description
.mc *
.mc
_
.mc |
all;Show all feedback (Default value)
.mc
_
.mc |
behavior;Confirm behavior switch
.mc
_
.mc |
kill;Confirm on receipt of KILL signal
.mc
_
.mc |
move;Show position during move
.mc
_
.mc |
none;Show no feedback
.mc
_
.mc |
placement;Show position and size during initial placement
_
quit;Confirm quitting MWM
_
resize;Show size during resize
_
restart;Confirm MWM restart
.mc
.TE
.mc |
.br
.ne 9
.mc
.P
The following resource specification illustrates the syntax for
\f(CWshowFeedback\fR: 
.mc |
.sp
.mc
.TS
tab(;);
l l .
.mc |
\f(CWMwm*showFeedback: placement resize behavior restart\fR
.mc
.TE
.P
.mc |
This command line provides feedback for initial client placement and
resize, and enables the dialog boxes to confirm the restart and set
.mc
behavior functions.  It disables feedback for the move function.
.mc |
.br
.H 3 "Other Resources"
The following resource applies to sizing and positioning of windows:
.iX "resource" "enableWarp"
.iX "enableWarp resource"
.P
\fBenableWarp\fP.  The default value of this resource, True, causes MWM
to "warp" the pointer to the center of the selected window during
keyboard-controlled resize and move operations.
Setting the value to False causes MWM to leave the pointer at its
original place on the screen, unless the user explicitly moves it with
the cursor keys or pointing device.
.br
.ne 3.25i
.mc
.H 3 "The Syntax for Size and Position Refinement Resources"
The resources that refine control over the size and
position
of
windows have the following syntax:
.iX "syntax" "for size and position refinement resources"
.mc |
.sp
.mc
.TS
tab(;);
l l l .
.mc |
\fBMwm*\fP\fIresource\fP:;\fIvalue\fP
.mc
.TE
.P
For example, to place each new window on the screen interactively, add
.mc |
the following line to the \fB.Xdefaults\fR file: 
.sp
.mc
.TS
tab(;);
l l l .
.mc |
\f(CWMwm*interactivePlacement:;True\fP
.mc
.TE
.P
In addition to this syntax, the \f(CWmaximumClientSize\fR
resource has two more
syntaxes:
.AL
.LI
To specify the maximum client size for specific classes of
.mc |
clients the syntax is
.mc
.TS
tab(;);
l l l .
.mc |
\fBMwm*\fP\fIclientclass\fP\fR.maximumClientSize:;\fP\fIwidth\(muheight\fP
.mc
.TE
.mc |
.br
.ne 10
.sp
.mc
For example, to specify that \f(CWxload\fR clients should
be maximized
to no more than one sixty-fourth of the size of a 1024\(mu768
.mc |
display, add the line:
.mc
.TS
tab(;);
l l l .
.mc |
\f(CWMwm*XLoad.maximumClientSize:;128\(mu96\fR
.mc
.TE
.P
.LI
To specify the maximum client size for any client with an unknown
.mc |
class, the syntax is:
.sp
.mc
.TS
tab(;);
l l l .
.mc |
\fBMwm*defaults*maximumClientSize:\fP;\fIwidth\(muheight\fR
.mc
.TE
.LE
.rs
