.mc |
.\"  (c) Copyright 1989, 1990 OPEN SOFTWARE FOUNDATION, INC.
.mc
.\"  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
.\"  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
.\"  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
.\"  (c) Copyright 1988 MICROSOFT CORPORATION
.\"  ALL RIGHTS RESERVED
.mc |
.nr H1 11
.mc
.H 1 "Communicating Between MWM and Clients: MWM Properties"
This chapter provides technical information you need to establish
.mc |
communications between MWM and your client.  A typical user does not need
.mc
access to this information.  
.P
.mc |
Chapter 12 discusses the following topics:
.mc
.P
.BL
.LI
.mc |
MWM programmatic interface standards
.mc
.iX "Programmatic Interface Standards"
.LI
.mc |
the Inter-Client Communications Conventions 
.mc
.LI
.mc |
MWM specific information
.mc
.LE
.P
.H 2 "MWM Programmatic Interface Standards"
.iX "Inter-Client Communications Conventions Manual"
.iX "OSF/Motif toolkit"
.iX "toolkit" "OSF/Motif"
The MWM programmatic interface is based on the \fIInter-Client 
.mc |
Communications Conventions Manual\fR (ICCCM).
.mc
The ICCCM
establishes the standards for "good citizenship" among clients in
.mc |
a multi-client environment.  To avoid costly
.mc
compatibility problems, you should design and code your client
application to operate as a "good citizen."  The OSF/Motif toolkit supports
.mc |
the inter-client communications conventions and facilitates appropriate
communications with MWM.  
.mc
.P
.mc |
MWM fully supports Version 1.0 of the ICCCM.
.mc
Conventions documented in earlier editions of the ICCCM are supported
by MWM only to the extent
.mc |
necessary to support clients that use released
.mc
versions of the X11 Xlib and Xt Intrinsics libraries.
.P
.H 2 "Inter-Client Communication Conventions"
The ICCCM section "Client to Window Manager Communication"
specifically
discusses how clients communicate with a window manager.  Reading 
.mc |
the section is recommended as it gives you generally applicable
.mc
information about how your client application should communicate
with a
.mc |
window manager.  The remainder of this chapter provides additional
client information and MWM-specific information.
.mc
.P
.H 3 "Programming Client Actions"
As mentioned above you should design your client application
to be a good citizen whether or not a window manager is present
to police
.mc |
the environment.  The following information helps you
.mc
program your client application to be a good citizen in a
multi-client environment.
.iX "top-level window" "creating"
.iX "window" "top level"
.iX "window" "creating"
.iX "XtAppCreateShell"
.P
.H 4 "Creating a Top-Level Window"
The typical way to create a top-level window is to use an Xt Intrinsics or
.mc |
Xm Toolkit function (for example, \fBXtAppCreateShell\fP).  These functions set up
.mc
standard window properties that are used to provide the window manager with
client-specific information.
.P
.iX "window states" "normal, withdrawn, iconic"
.mc |
At any time, your client application's top-level windows
.mc
have one
of three states:
.P
.VL 1.25i
.LI \fRNormal\fR
A normal application window is displayed.
.LI \fRIconic\fR
An icon window is displayed instead of a normal window.
.LI \fRWithdrawn\fR
No normal or iconic window is displayed.
.LE
.P
.iX "client properties"
.iX "properties" "client"
.mc |
.iX "WmShell"
.mc
.H 4 "Working with Client Properties"
Each top-level window you create for your client should have several
standard
properties associated with it.  The values of these properties are generally
.mc |
accessible through \fBWmShell\fP resources.
.mc
These properties are what the
window
manager inspects to determine how it should manage the client's
behavior.
.P
This is especially important in the case where the proper
operation of
your client application depends on particular property values; any
properties you \fIdon't\fR specify are specified by the window
manager \fIusing
whatever values are most convenient\fR.
.P
Client applications can set up the following properties:
.iX "WM_ NAME"
.iX "property" "WM_ NAME"
.P
\fBWM_NAME.\fR  The WM_NAME property contains a string to be
displayed in the title area of
the client window frame.  MWM can dynamically change the window
title if
your client application changes the value of the string in the
WM_NAME
property.  If you don't set the WM_NAME property, MWM looks for a title in
the \f(CWres_name\fR part of the WM_CLASS property.  If MWM finds no
title, it uses the string "*****" as the window title.  The window manager
assumes that the string passed in the WM_NAME property
is compatible with the font used for the window title.
.iX "WM_CLASS"
.iX "property" "WM_CLASS"
.iX "WM_ICON_NAME"
.iX "property" "WM_ICON_NAME"
.P
\fBWM_ICON_NAME.\fR  The WM_ICON_NAME property contains a string
to be displayed in the label
part of the icon that is associated with the client window.  MWM
can
dynamically change the icon label if the WM_ICON_NAME property
value is changed by the client.  If you don't set the WM_ICON_NAME
property, MWM uses the window title as the icon label.  The window manager
assumes that the string passed in the WM_ICON_NAME property
is compatible with the font used for the icon label.
.P
.iX "WM_NORMAL_HINTS"
.iX "property" "WM_NORMAL_HINTS"
\fBWM_NORMAL_HINTS.\fR  The WM_NORMAL_HINTS property contains a
list of fields.  MWM tracks changes
to the WM_NORMAL_HINTS property.  A change of the property does not affect
.mc |
the window configuration at the time the change is made.  The change
affects only subsequent configurations.
.br
.ne 10
.mc
.P
.mc |
The type of the WM_NORMAL_HINTS property is WM_SIZE_HINTS.
.mc
The WM_NORMAL_HINTS property contains the following fields:
.P
.mc |
.VL 1i
.mc
.LI \fRflags\fR
.mc |
.iX "flags" "WM_NORMAL_HINTS"
.mc
.iX "WM_NORMAL_HINTS" "flags"
MWM places windows on the screen using various sources of
size and position information.  The order of precedence MWM
uses to look for this information is as follows:
.AL
.LI
User specified.  The client has been supplied configuration
information by the user (indicated to MWM by the USSize and USPosition values).
.LI
Interactive placement.  Interactive placement is established with
.mc |
the \f(CWinteractivePlacement\fR resource (see Chapter 13).
.mc
.LI
Default position, current window size.
.LE
.iX "WM_NORMAL_HINTS" "min_width, min_height"
.iX "min_width, min_height" "WM_NORMAL_HINTS"
.LI "\fRmin_width, min_height\fR"
.mc |
.br
.mc
The values set for min_width and min_height (minimum width and minimum
height) are used to configure a
minimum size for a client window.
If the values set for these fields 
are not greater than 0, or not set at all,
then a value of 1x1 or larger is used by MWM.  The actual minimum
size used by MWM is based on the window size that fits in the
minimum frame size for the frame type that is being used.
.iX "WM_NORMAL_HINTS" "max_width, max_height"
.iX "max_width, max_height" "WM_NORMAL_HINTS"
.iX "maximumClientSize resource"
.iX "maximumMaximumSize resource"
.LI "\fRmax_width, max_height\fR"
.mc |
.br
.mc
The values set for max_width and max_height (maximum width and maximum
height) are used only
if the \f(CWmaximumClientSize\fR resource is not set.
The values set with these fields are used to set a maximum size for a
client window.  If max_width and max_height are not configured, then MWM
.mc |
sizes the window and its frame to exactly fill the screen.
.mc
The maximum size of a window can be limited by the
.mc |
\f(CWmaximumMaximumSize\fR resource.  (See Chapter 13 for resource
.mc
descriptions.)
.iX "WM_NORMAL_HINTS" "width_inc, height_inc"
.iX "width_inc, height_inc" "WM_NORMAL_HINTS"
.mc |
.br
.ne 9
.mc
.LI "\fRwidth_inc, height_inc\fR" 
.mc |
.br
.mc
The values set for width_inc and height_inc (width increment and height
increment) determine the unit of
measure used to report window size.  When windows are being resized, a
feedback window reports the current size in the units specified.  If
.mc |
values are not set for these fields, then one pixel is used as the sizing
.mc
increment.
.mc |
.br
.ne 9
.mc
.iX "min_aspect.x, min_aspect.y" "WM_NORMAL_HINTS"
.iX "WM_NORMAL_HINTS" "min_aspect.x, min_aspect.y"
.LI "\fRmin_aspect.x, min_aspect.y\fR"
.mc |
.br
The values set for min_aspect.x and min_aspect.y [minimum aspect.x (width) and minimum aspect.y (length)]
.mc
determine constraints for the minimum ratio of width/length of a window.
.mc |
MWM applies a minimum aspect ratio sizing constraint when
.mc
the x and y values are set greater than or equal
.mc |
to 0.  The values must also be less than or equal to the max_aspect values.
.mc
.iX "WM_NORMAL_HINTS" "max aspect.x, max aspect.y"
.iX "max aspect.x, max aspect.y" "WM_NORMAL_HINTS"
.LI "\fRmax_aspect.x, max_aspect.y\fR"
.mc |
.br
.mc
The values set for max_aspect.x and max_aspect.y [maximum aspect.x (width)
and maximum aspect.y (length)]
determine constraints for the maximum ratio of width/length of a window.
.mc |
MWM applies a maximum aspect-ratio-sizing constraint when
.mc
the x and y values are set greater than or equal
to zero.  The values must also be greater than or equal to the min_aspect
values.
.iX "WM_NORMAL_HINTS" "base_width, base_height"
.iX "base_width, base_height" "WM_NORMAL_HINTS"
.LI "\fRbase_width, base_height\fR"
.mc |
.br
.mc
The values set for these fields determine the amount of "padding" (margin)
between
the window and the window frame.  The base width value sets the amount of
left and right padding.  The base height value sets the amount of top and
bottom padding.
If these fields have a value of less than 0, or if there is no
value set, then MWM uses a value of 0.
.mc |
.iX "WM_NORMAL_HINTS" "win_gravity"
.iX "win_gravity" "WM_NORMAL_HINTS"
.LI "\fRwin_gravity\fR"
.br
The value set for win_gravity determines where the window manager tries
to place windows.
If the window manager decides to place a window where the client asks,
the position on the parent window's border named by win_gravity will be
placed where the client window would have been placed in the absence of
a window manager.
The default value is NorthWest.
.mc
.LE
.iX "WM_HINTS"
.iX "property" "WM_HINTS"
.P
\fBWM_HINTS.\fR  The WM_HINTS property contains a list of
fields.  Except for changes to the
icon_pixmap, MWM tracks changes to the WM_HINTS property only
when the
client window changes state from the withdrawn state to the
normal or
iconic state.
.mc |
.br
.ne 25
.mc
.iX "icon_pixmap"
.iX "icon_window"
.iX "icon_x"
.iX "icon_y"
.iX "icon_mask"
.iX "input" "WM_HINTS"
.iX "window_group"
.iX "WM_HINTS" "icon_pixmap"
.iX "WM_HINTS" "icon_window"
.iX "WM_HINTS" "icon_x"
.iX "WM_HINTS" "icon_y"
.iX "WM_HINTS" "icon_mask"
.iX "WM_HINTS" "input"
.iX "WM_HINTS" "window_group"
.P
.mc |
The type of the WM_HINTS property is WM_HINTS.
.mc
The WM_HINTS property contains the following fields:
.P
.VL 1.25i
.LI \fRicon_pixmap\fR
An image for icon window.
.LI \fRicon_window\fR
A working window for the icon window.
.LI \fRicon_x\fR
X coordinate for icon window position.
.LI \fRicon_y\fR
Y coordinate for icon window position.
.LI \fRicon_mask\fR
.mc |
MWM does not currently use this.
.mc
.LI \fRinput\fR
.mc |
See the following NOTE. 
.mc
.LI \fRwindow_group\fR
.mc |
MWM does not currently use this.
.mc
.LE
.mc |
.br
.ne 9
.mc
.P
.mc |
\fBNOTE:\fR
MWM does not set keyboard focus to the client window if any of the following
.mc
conditions exist:
.AL
.LI
.mc |
The value for input is False.
.mc
.LI
.mc |
The value for the \f(CWenforceKeyFocus\fR resource is True.
.mc
.LI
The client has expressed an interest in the WM_TAKE_FOCUS protocol.
.LE
.mc *
.mc
.P
.iX "WM_TAKE_FOCUS" "and WM_HINTS"
If the user attempts to set the keyboard focus to the window, a
WM_TAKE_FOCUS client message is sent to the window.  The input field also
determines whether or not a window receives keyboard focus automatically.
.mc |
(See the following resources in Chapter 13: \f(CWautoKeyFocus,
.mc
startupKeyFocus\fR, and \f(CWdeiconifyKeyFocus\fR.)
.P
.iX "WM_CLASS"
.iX "property" "WM_CLASS"
\fBWM_CLASS.\fR  The WM_CLASS property contains two fields.
MWM tracks changes to the
WM_CLASS property only when the client window changes state from
the
withdrawn state to the normal or iconic state.
.P
.mc |
The res_class and res_name values are used by MWM to do client-specific 
configuration of window decorations and icons.  If the
.mc
WM_CLASS property is not set, then no special client
.mc |
customization is done.
.mc
.P
.mc |
The type of the WM_CLASS property is STRING.
.mc
The WM_CLASS property contains the following fields:
.P
.VL 1.25i
.iX "res_class"
.iX "WM_CLASS" "res_class"
.iX "res_name"
.iX "WM_CLASS" "res_name"
.LI \fRres_class\fR
When a client is initially managed by MWM,
the res_class value is used to determine the client's class.  All resources
.mc |
previously configured for that class are used for the new client.
.mc
.LI \fRres_name\fR
The res_name value is used by MWM to look up MWM resources
for a client of 
a particular name.  The value also determines the name used in the client's
window title when the WM_NAME property is not set.
.LE
.P
.iX "WM_TRANSIENT_FOR"
.iX "property" "WM_TRANSIENT_FOR"
\fBWM_TRANSIENT_FOR.\fR  MWM regards a transient window as
equivalent to a secondary window.  A
transient window is always on top (in terms of stacking order) of
its
primary window.  This primary window is identified by the
WM_TRANSIENT_FOR
property.
.mc |
.br
.ne 6
.mc
.P
The window manager places transient windows on the screen without
user
interaction.  MWM uses the window's existing size and position.  The amount
of decoration for a transient window
is controlled by the \f(CWtransientDecoration\fR resource.  (See
.mc |
Section 13.11 and the _MOTIF_WM_HINTS property in this chapter
for more information on window decoration.)
.mc
.P
A transient window is normally associated with a primary window.
.mc |
You can design your client windows so that transient windows
.mc
are arranged
in a tree structure where a transient window has another
transient window
.mc |
as its associated primary window.  However, the root of the
.mc
tree must be
a non-transient window.
.P
.mc |
The type of the WM_TRANSIENT_FOR property is WINDOW.
.mc
.iX "property" "WM_PROTOCOLS"
.iX "WM_PROTOCOLS"
.mc |
.iX "XmAddProtocols"
.iX "VendorShell"
.mc
\fBWM_PROTOCOLS.\fR  The WM_PROTOCOLS property contains a list
of atoms (32-bit values that represent unique names).  Each atom identifies a
protocol in which the client is willing to participate.  Atoms
can identify
standard protocols and private protocols specific to individual
.mc |
window managers.  The \fBXmAddProtocols\fP and \fBXmAddWMProtocols\fP functions
can be used to set up the WM_PROTOCOLS property for a \fBVendorShell\fP.
.mc
.P
.mc |
The type of the WM_PROTOCOLS property is ATOM.
.mc
MWM tracks changes to the WM_PROTOCOLS property and
supports the
following standard protocols:
.iX "protocol" "WM_DELETE_WINDOW"
.iX "WM_DELETE_WINDOW"
.P
.VL 2.5i
.LI \fRWM_DELETE_WINDOW\fR
Clients are notified when the MWM
\f(CWf.kill\fR function is invoked by the user.  MWM does not
terminate the client or destroy the window when a
WM_DELETE_WINDOW notification is done.
.mc |
.br
.ne 9
.mc
.LI \fRWM_SAVE_YOURSELF\fR
.iX "WM_SAVE_YOURSELF"
.iX "protocol" "WM_SAVE_YOURSELF"
.mc |
Clients with this atom are notified when a
.mc
session manager or a window manager wishes the window's state to
be changed.  The typical change is when the window is about to be
deleted or the session terminated.
.mc |
.ne 8
.mc
.P
.iX "quitTimeout resource"
.iX "resource" "quitTimeout"
.iX "WM_SAVE_YOURSELF" "and quitTimeout"
.mc |
The \f(CWquitTimeout\fR resource specifies
the amount of time (in milliseconds) that MWM waits for a
.mc
client to update the WM_COMMAND property after it has sent the
.mc |
WM_SAVE_YOURSELF message.  This protocol is used only for
clients that have a WM_SAVE_YOURSELF atom in the
.mc
WM_PROTOCOLS client window property.  The default time is 1000
(ms).  (Since \f(CWquitTimeout\fR is a resource, not a property, a user can
change its value.)
.iX "WM_TAKE_FOCUS"
.iX "protocol" "WM_TAKE_FOCUS"
.LI \fRWM_TAKE_FOCUS\fR
.mc |
Clients with this atom are notified when a
.mc
window manager believes that the client should explicitly set
the input focus to one of its windows.
.iX "protocol" "_MOTIF_WM_MESSAGES"
.iX "_MOTIF_WM_MESSAGES"
.iX "f.send_msg function" "and _MOTIF_WM_MESSAGES"
.LI \fR_MOTIF_WM_MESSAGES\fR
.mc |
Clients with this atom indicate to the
.mc
window manager which messages (sent by the window manager when
the \f(CWf.send_msg\fR function is invoked) are currently being
handled by the client.
.LE
.iX "property" "WM_COLORMAP_WINDOWS"
.iX "WM_COLORMAP_WINDOWS"
.P
\fBWM_COLORMAP_WINDOWS.\fR  The WM_COLORMAP_WINDOWS property
.mc |
contains a list of IDs of windows that may need colormaps installed that
differ from the colormap of the top-level window.
The type of the WM_COLORMAP_WINDOWS property is WINDOW.
.mc
.iX "properties" "working with"
.H 4 "Working with Window Manager Properties"
MWM uses properties to supply configuration and state information
to clients (usually session managers).
.iX "WM_STATE"
.iX "WM_STATE" "state field"
.iX "WM_STATE" "icon field"
.iX "state field" "WM_STATE"
.iX "icon field" "WM_STATE"
.iX "WM_HINTS" "and WM_STATE"
.iX "WM_STATE" "and WM_HINTS"
.P
.mc |
\fBWM_STATE.\fR  The type of the WM_STATE property is WM_STATE.
The WM_STATE property contains the following
.mc
fields:
.P
.VL 0.75i
.LI \fRstate\fR
NormalState, IconicState, and WithdrawnState are the values defined for MWM.
.mc |
.br
.ne 7
.mc
.LI \fRicon\fR
The icon window value is set to the window ID of the top-level icon window;
.mc |
this window is not the icon window supplied by the client.  (The icon
.mc
window, if it is set in WM_HINTS, is a child of the top-level, MWM-created
icon window.)
.LE
.P
The information in the WM_STATE property is generally used only by session
management clients.
.mc |
.br
.ne 9
.mc
.P
.iX "property" "WM_ICON_SIZE"
.iX "WM_ICON_SIZE"
\fBWM_ICON_SIZE.\fR  MWM sets the WM_ICON_SIZE property of the
.mc |
root window.
The type of the WM_ICON_SIZE property is WM_ICON_SIZE.
WM_ICON_SIZE contains the following fields:
.mc
.iX "WM_ICON_SIZE" "min_width, min_height"
.mc |
.iX "min_width, min_height" "WM_ICON_SIZE"
.mc
.P
.VL 1.5i
.LI "\fRmin_width, min_height\fR"
.mc |
.br
.mc
The values set for  min_width and min_height (minimum width and minimum
height) of an 
icon window are based on the value of (or default value for)
the \f(CWiconImageMinimum\fR resource.
.iX "WM_ICON_SIZE" "and iconImageMinimum"
.iX "iconImageMinimum" "and WM_ICON_SIZE"
.iX "WM_ICON_SIZE" "max_width, max_height"
.iX "max_width, max_height" "WM_ICON_SIZE"
.iX "WM_ICON_SIZE" "and iconImageMaximum"
.iX "iconImageMaximum" "and WM_ICON_SIZE"
.iX "width_inc, height_inc" "WM_ICON_SIZE"
.iX "WM_ICON_SIZE" "width_inc, height_inc"
.LI "\fRmax_width, max_height\fR"
.mc |
.br
.mc
The values set for max_width and max_height (maximum width and maximum
height) of an 
icon window are based on the value of (or default value for)
the \f(CWiconImageMaximum\fR resource.
.LI "\fRwidth_inc, height_inc\fR"
.mc |
.br
.mc
The values set for width_inc and height_inc (width increment and height
increment) determine the unit of measurement for changing the size of an
.mc |
icon window.  MWM sets this value to one pixel.
.mc
.LE
.P
.mc |
.H 4 "Changing Window States"
.mc
.iX "changing window state"
.iX "window" "changing state"
Windows are normal (full sized), iconic (small symbol), or
withdrawn (not visible).  You can control many attributes of
.mc |
normal and icon windows.  See Chapter 13 for information on the
.mc
appearance and behavior of windows in the NormalState.  See
.mc |
Chapter 15 for information on the appearance and behavior of
.mc
windows in the IconicState.
.P
.iX "configuring" "window"
.iX "window" "configuring"
.iX "ConfigureNotify"
.iX "XTranslateCoordinates"
.iX "attributes" "changing windows"
.iX "changing window attributes"
.iX "saveUnder resource" 
.iX "controlling input focus"
.iX "input focus" "controlling"
.H 4 "Configuring the Window"
Clients can request to be notified, with \f(CWConfigureNotify\fR
events, when windows change size or position.  The X,Y
coordinates in these events may be relative to either the root
window or the frame provided by MWM.  Use
\f(CWXTranslateCoordinates\fR to determine absolute coordinates.
.mc |
.br
.ne 10
.mc
.P
.H 4 "Changing Window Attributes"
If the client requests save unders with the \f(CWsaveUnder\fR
.mc |
resource, MWM sets this attribute for the MWM frame instead
.mc
of the client window.
.P
.H 4 "Controlling Input Focus"
Use the \f(CWkeyboardFocusPolicy\fR resource to control the input
focus.  Clients can request to be notified when given the input
focus.  See "WM_PROTOCOLS."
.mc |
.ne 4
.mc
.P
Windows that supply a WM_PROTOCOLS property containing the
.mc |
WM_TAKE_FOCUS atom receive a ClientMessage from the window
.mc
manager.
.iX "WM_TAKE_FOCUS"
.iX "WM_TAKE_FOCUS" "ClientMessage"
.P
.iX "establishing colormaps"
.iX "colormaps" "establishing"
.iX "WM_COLORMAP_WINDOWS"
.H 4 "Establishing Colormaps"
If more than one colormap is needed for client subwindows, then
set the WM_COLORMAP_WINDOWS property to the list of windows with
colormaps.
.P
.iX "client responses to MWM actions"
.iX "MapWindow"
.iX "ConfigureWindow"
.iX "CirculateWindow"
.H 3 "Client Responses to MWM Actions"
MWM redirects the following top-level window requests:
\f(CWMapWindow,\fR \f(CWConfigureWindow,\fR
\f(CWCirculateWindow\fR.  MWM may not immediately execute (or
execute at all) redirected requests.
.P
.H 4 "Window Size and Position"
Clients can request sizes and positions by doing a window configuration
operation, but MWM may not satisfy these requests.
.mc |
.br
.ne 11
.mc
.P
.H 4 "Window and Icon Mapping"
Client windows in the normalized state are mapped.  Client windows in the
iconified state are not mapped.
.P
.iX "ColormapNotify"
.iX "colormap" "changing"
.H 4 "Colormap Changes"
Clients can request to be notified when their colormap is in use (or no
longer in use), by using \f(CWColormapNotify\fR.
.P
.iX "XSetInputFocus"
.iX "Wm Widgets"
.H 4 "Input Focus"
Distribution of input within a client window can be handled using
Xt Intrinsics and the Xm Widgets.  Clients should generally avoid using
.mc |
\fBXSetInputFocus\fP.
.mc
.P
.iX "ClientMessage Events"
.H 4 "ClientMessage Events"
.mc |
Clients cannot prevent being sent \f(CWClientMessage\fR events,
but clients can ignore these if they are not useful.
.mc
.P
.iX "ICCCM" "MWM information not covered by"
.mc |
.iX "VendorShell"
.iX "XmBulletinBoard"
.mc
.H 2 "MWM Specific Information"
The following information details window manager conventions not
covered by the ICCCM, but which are required for supporting OSF/Motif
behavior.  Properties on the client window are used to communicate client
.mc |
preferences to MWM.  These properties are settable using \fBVendorShell\fP
and \fBXmBulletinBoard\fP resources.  Value definitions for MWM-specific
properties are set in the \fBXm/MwmUtil.h\fP file.
.br
.ne 12
.H 3 "The _MOTIF_WM_HINTS Property" 
.mc
.iX "_MOTIF_WM_HINTS"
.iX "property" "_MOTIF_WM_HINTS"
A client may communicate certain preferences directly to MWM via
.mc |
the _MOTIF_WM_HINTS property.
The type of the _MOTIF_WM_HINTS property is _MOTIF_WM_HINTS.
The following table shows the contents of this property.
.br
.ne 2i
.mc
.P
.mc |
.TB "Contents of _MOTIF_WM_HINTS Property"
.mc
.TS
center, box, tab(;);
lb | lb
l | l .
Field;Type
.mc |
_
.mc
flags;CARD32
_
decorations;CARD32
_
functions;CARD32
_
.mc |
input_mode;INT32
.mc
.TE
.P
.iX "_MOTIF_WM_HINTS" "flags field"
.iX "flags field" "_MOTIF_WM_HINTS"
.mc |
.H 4 "The flags Field"
The flags field indicates which fields in the _MOTIF_WM_HINTS
property contain data.  The following table shows the supported values.
.mc
.P
.mc |
.TB "Values of the flags Field"
.mc
.TS
center, box, tab(;);
lbw(2i) | lb | lbw(1.5i)
l | n | l .
Name;Value;Field
.mc *
.mc
_
.mc |
MWM_HINTS_FUNCTIONS;1;T{
MWM functions applicable to client
T}
.mc
_
.mc |
MWM_HINTS_DECORATIONS;2;T{
Client window frame decorations
T}
_
MWM_HINTS_INPUT_MODE;4;T{
Client input mode
T}
.mc
.TE
.mc |
.br
.ne 16
.mc
.P
.iX "WM_HINTS" "decorations field"
.iX "decorations field" "WM_HINTS"
.mc |
.H 4 "The decorations Field"
.mc
The decorations field indicates how the client window frame
should be
decorated (for example, whether the window should have a title
bar or
window menu button).  The information in this field is combined with the
value of the
.mc |
\f(CWclientDecoration\fR resource (see Section 13.11).
Only decorations that are indicated by
.mc
both \f(CWclientDecoration\fR and _MOTIF_WM_HINTS are displayed.
.mc |
Decorations for inapplicable functions are not displayed (for example, if a window
.mc
cannot be minimized, the minimize window button is not displayed).
.iX "clientDecoration resource" "and WM_HINTS"
.iX "WM_HINTS" "and clientDecoration resource"
.mc |
.br
.ne 3i
.mc
.P
.mc |
The following table shows the supported values.
.mc
.P
.mc |
.TB "Values of the decorations Field"
.mc
.TS
box, tab(;);
lbw(2i) | lb | lbw(2.5i)
l | n | l .
Name;Value;Comments
.mc *
.mc
_
.mc |
MWM_DECOR_ALL;1;T{
If set, remove decorations
from full set
T}
_
.mc
MWM_DECOR_BORDER;2;Client window border
_
MWM_DECOR_RESIZEH;4;Resize frame handles
_
MWM_DECOR_TITLE;8;Title bar
_
.mc |
MWM_DECOR_MENU;16;Window menu button
.mc
_
MWM_DECOR_MINIMIZE;32;Minimize window button
_
.mc |
MWM_DECOR_MAXIMIZE;64;Maximize window button
.mc
.TE
.P
.iX "_MOTIF_WM_HINTS" "functions field"
.iX "functions field" "_MOTIF_WM_HINTS"
.mc |
.H 4 "The functions Field"
.mc
The functions field indicates which MWM functions should apply to
the client window (for example, whether the window should be
resized).  The information in this field is combined with the value of the
.mc |
\f(CWclientFunctions\fR resource (see Section 14.3.4).  Only
.mc
functions that are indicated by 
both \f(CWclientFunctions\fR and _MOTIF_WM_HINTS are applied.
Also, decorations that
support
.mc |
a particular function (for example, the minimize button) are not
.mc
be shown if the associated function is not applicable.
.mc |
The following table shows the supported values for the functions field.
.br
.ne 20 
.mc
.P
.mc |
.TB "Values of the functions Field"
.mc
.iX "clientFunctions resource" "and WM_HINTS"
.iX "WM_HINTS" "and clientFunctions resource"
.TS
box, tab(;);
lbw(2i) | lb | lbw(2.5i)
l | n | l .
Name;Value;Comments
.mc *
.mc
_
.mc |
MWM_FUNC_ALL;1;T{
If set, remove functions 
from full set
T}
_
.mc
MWM_FUNC_RESIZE;2;f.resize
_
MWM_FUNC_MOVE;4;f.move
_
MWM_FUNC_MINIMIZE;8;f.minimize
_
MWM_FUNC_MAXIMIZE;16;f.maximize
_
MWM_FUNC_CLOSE;32;f.kill
.TE
.P
.iX "input_mode field" "WM_HINTS"
.iX "WM_HINTS" "input_mode field"
.mc |
.H 4 "The input_mode Field"
.mc
The input_mode field indicates the keyboard input focus
.mc |
constraints that are imposed by the client window.  The following table 
shows the values of the input_mode field.
.mc
.P
.mc |
.TB "Values of the input_mode Field"
.mc
.TS
box, tab(;);
lb | lb | lbw(2.5i)
l | n | l .
Name;Value;Comments
.mc |
_
MWM_INPUT_MODELESS;0;Input goes to any window
_
MWM_INPUT_PRIMARY_APPLICATION_MODAL;1;T{
Input does not go to
ancestors of this window
.mc
T}
_
.mc |
MWM_INPUT_SYSTEM_MODAL;2;T{
Input goes only to this 
window
.mc
T}
.mc |
_
MWM_INPUT_FULL_APPLICATION_MODAL;3;T{
Input does not go to other
windows in this application
T}
.mc
.TE
.mc |
.br
.ne 2.5i
.mc
.P
.iX "window menu" "adding to with MOTIF_WM_MENU"
.iX "_MOTIF_WM_MENU" "adding to window menu"
.iX "property" "_MOTIF_WM_MENU"
.iX "_MOTIF_WM_MENU"
.mc |
.H 4 "The _MOTIF_WM_MENU Property"
.mc
The client uses the _MOTIF_WM_MENU property to add menu items to the
end of the window menu for the client window.  The contents of
the property are a list of lines separated by new line
characters (\f(CW\\n\fR) , with the
following format:
.P
\fIlabel [mnemonic] [accelerator] function\fR \f(CW\\n\fR
.br
\fIlabel [mnemonic] [accelerator] function\fR
.P
Interpretation of the strings is the same as for menu items (see
.mc |
Section 14.3.2 for more information on menu items).
.br
.ne 18
.mc
.iX "f.send_msg function" "and _MOTIF WM MESSAGES"
.iX "_MOTIF_WM_MESSAGES" "and f.send_msg"
.iX "_MOTIF_WM_MESSAGES"
.iX "property" "_MOTIF_WM_MESSAGES"
.mc |
.H 4 "The _MOTIF_WM_MESSAGES Property"
.mc
The client uses the  _MOTIF_WM_MESSAGES property to indicate
to the window manager which messages (sent by the window manager
when the \f(CWf.send_msg\fR function is invoked) are currently
being handled by the client.  Menu items that have
\f(CWf.send_msg\fR specified as the function have grayed-out
labels when the associated message is not being handled by the
client.
.iX "XChangeProperty"
.iX "_MOTIF_WM_MESSAGES" "and WM_PROTOCOLS"
.iX "WM_PROTOCOLS" "and _MOTIF_WM_MESSAGES"
.P
This client property is tracked by the window manager if the
_MOTIF_WM_MESSAGES atom is included in the client's WM_PROTOCOLS
property.  The _MOTIF_WM_MESSAGES property contains a list of
integers (for XChangeProperty:  type atom is INTEGER, format is 32).  A
client places the property on a client window and it is processed by MWM when
the client window goes from withdrawn state to normalized or
iconified state.  Changes to the property are processed while the
client window is not in the withdrawn state.
.iX "_MOTIF_WM_INFO"
.iX "property" "_MOTIF_WM_INFO" 
.P
.mc |
.H 4 "The _MOTIF_WM_INFO Property"
.mc
The client receives MWM-specific information via the _MOTIF_WM_INFO
property.
This property is placed by MWM on the
root window and is used by clients.  The _MOTIF_WM_INFO property
.mc |
is set up as part of MWM initialization.
.mc
.P
.mc |
The type of the _MOTIF_WM_INFO property is _MOTIF_WM_INFO.
The following table shows the contents of the
_MOTIF_WM_INFO property.
.br
.ne 2i
.P
.TB "Contents of the _MOTIF_WM_INFO Property"
.mc
.TS
box, center, tab(;);
lb | lb
l  | l  .
Field;Type
.mc |
_
.mc
flags;CARD32
_
wmWindow;CARD32
.TE
.P
.mc |
The following table shows the values that may be set:
.br
.ne 3i
.sp 1
.mc
.iX "_MOTIF_WM_INFO" "flags field"
.iX "flags field" "_MOTIF_WM_INFO"
.mc |
.TB "Values for the flags Field"
.mc
.TS
box, tab(;);
lb | cb | cb
l  | l |  l  .
Name;Value;Field
.mc |
_
.mc
MWM_INFO_STARTUP_STANDARD;1;T{
Set for startup with standard behavior.
T}
_
MWM_INFO_STARTUP_CUSTOM;2;T{
Set for startup with customized behavior.
T}
.TE
.iX "wmWindow field" "_MOTIF_WM_INFO"
.iX "_MOTIF_WM_INFO" "wmWindow field"
.P
.mc |
The wmWindow field is always set to the window
ID of a window that is used by MWM.  When MWM is running, the
.mc
_MOTIF_WM_INFO property is present on the root window and
wmWindow is an ID for a window that exists.
.mc |
.br
.ne 3i
.mc
.iX "Window Management Calls"
.iX "synthetic events"
.iX "XSendEvent"
.iX "standard X events"
.mc |
.iX "Xlib routines"
.mc
.iX "Xt Intrinsics"
.iX "Xm OSF/Motif toolkit"
.H 3 "Window Management Calls"
Clients communicate with the window manager through properties
associated with top-level windows, synthetic events (generated
.mc |
using \fBXSendEvent\fP and standard X events.  Programmatically this
communication involves Xlib routines (directly or through libraries
.mc
such as the Xt Intrinsics or the Xm OSF/Motif toolkit).  Clients may
programmatically interact with MWM (or
any X11 window manager) in one of the following ways:
.BL
.LI
\fBNo explicit programmatic access.\fR
.iX "No explicit programmatic access"
.iX "programmatic access" "no explicit"
In this case, clients do not set up any window properties or do
any call that directly communicates to the window manager.
Communication occurs (indirectly) when the state of the client
window is changed (that is, the window is mapped, unmapped,
.mc |
configured, has a colormap change, and so on.).  To work with MWM,
.mc
clients are not required to do anything more than what is
required when a window manager is not being used.
.mc |
.br
.ne 5
.mc
.LI
\fBLow-level programmatic access.\fR
.mc |
Clients with special window management requirements can use such low-level Xlib
calls as \fBXStoreName\fP or \fBXSetWMHints\fP to communicate
.mc
with the window manager.
.LI
\fBHigh-level programmatic access.\fR
.iX "High-level programmatic access"
.iX "programmatic access" "high-level"
.iX "Xt Intrinsics"
.mc *
.mc
.iX "Low-level programmatic access"
.iX "programmatic access" "low-level"
.mc |
.iX "Xlib routines"
.iX "XStoreName"
.iX "XSetWMHints"
.mc
Clients can make calls to certain libraries built on Xlib (Xt Intrinsics,
Xm OSF/Motif toolkit) to establish and maintain standard communications
with MWM.  Client developers are encouraged to use these interfaces
unless the client has some specialized window management requirements.
.LE
.mc |
.iX "Xlib routines" "C Language X Interface"
.br
.ne 18
.H 4 "Xlib Routines"
The following Xlib routines are supported in revision 4 of the X Window
System, Version 11 (refer to \fIXlib - C Language X Interface\fR for more 
information).
.mc
.P
.TS
tab(;);
l l l  .
.mc |
\fBXGetIconSizes\fP;\fBXSetCommand\fP;\fBXSetWMIconName\fP
\fBXGetRGBColormap\fP;\fBXSetTransientForHint\fP;\fBXSetWMNormalHints\fP
\fBXSetClassHint\fP;\fBXSetWMHints\fP;\fBXStoreName\fP
.mc
.TE
.mc |
.iX "XGetIconSizes"
.iX "XGetRGBColormap"
.iX "XSetClassHint"
.iX "XSetCommand"
.iX "XSetTransientForHint"
.iX "XSetWMHints
.iX "XSetWMIconName"
.iX "XSetWMNormalHints"
.iX "XStoreName"
.mc
