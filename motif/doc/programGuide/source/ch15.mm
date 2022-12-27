.mc |
.\"  (c) Copyright 1989, 1990 OPEN SOFTWARE FOUNDATION, INC.
.mc
.\"  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
.\"  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
.\"  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
.\"  (c) Copyright 1988 MICROSOFT CORPORATION
.\"  ALL RIGHTS RESERVED
.mc |
.nr H1 14
.mc
.H 1 "Working with Icons"
.iX "icons"
This chapter presents information users need to customize the appearance
and behavior of icons.  Icons provide the user with a handy way to
straighten up a cluttered workspace, and
.mc |
are a great tool for efficient multiprocessing.  Information is also
.mc
presented on configuring the MWM icon box.  As a programmer, being aware of
this information can help you avoid user-caused problems in your
application.
.P
.mc |
This chapter discusses the following topics:
.mc
.BL
.LI
.mc |
Studying icon anatomy
.mc
.LI
.mc |
Manipulating icons
.mc
.LI
.mc |
Controlling icon appearance and behavior
.mc
.LI
.mc |
Coloring icons
.mc
.LI
.mc |
Using the icon box to hold icons
.mc
.LE
.iX "icons" "anatomy"
.H 2 "Studying Icon Anatomy"
.mc |
As shown in the following figure, icons consist of two parts: a 
text label and a graphic image.
.mc
Like the other objects appearing on the
workspace, the user can configure the appearance of all icons in
.mc |
\fB\&.Xdefaults\fR.
.mc
.ig ++
.P
.br
.ie \nZ \{\
.br
.ne 20.5P
.P
.sC graphics/iconpart.tif tiff 300 20.5P 24.5P
\&
.P
.sp +1
.FG "Icons Have Two Parts."
.sp +1.5
.\}
.el \{\
.br
.ne 22P
\&
.sp 20.5P
.ce
illustration graphics/iconpart.eps goes here
.sp +1
.FG "Icons Have Two Parts"
.sp +1.5
.\}
.P
.++
.br
.ne 3.5i
\0
.mc |
.FG "Icons Have Two Parts"
.mc
.sp 3.25i
.mc |
.in +1.56i
.mc
.P! graphics/iconpart.ps 
.mc |
.in -1.56i
.mc
.P
.iX "icons" "image"
.iX "images" "icon"
.iX "icon image" "default image"
.H 3 "The Image"
An icon image (a bitmap) is the actual graphic illustration in
the icon.
An image can come from any one of the following three sources:
.P
.iX "iconImage resource"
.iX "WM_HINTS" "and icon images"
.iX "icon images" "and WM_HINTS"
.mc |
.VL .8i
.LI 
\fBClient\fR
A client can use the WM_HINTS window
.mc
property to specify either an icon window or a bitmap for the
window manager to use as
the icon image.
.mc |
.br
.ne 3
.LI 
\fBUser\fR
.mc
The user can specify an icon image using the
.mc |
\f(CWMwm*iconImage\fP
or \f(CWMwm*default*iconImage\fP resources.
.LI 
\fBDefault\fR
The window manager uses its own built-in default
.mc
icon image if an image is not specified elsewhere.
.mc |
.LE
.mc
.P
.iX "resource" "bitmapDirectory"
.iX "bitmapDirectory resource"
.mc |
\fBbitmapDirectory.\fR  You can also, using the
\f(CWbitmapDirectory\fR resource, direct the window manager to
.mc
search a specified directory for bitmaps.  The
\f(CWbitmapDirectory\fR resource causes the window manager
to search the specified directory whenever a bitmap is named with
no complete path.  The default value for \f(CWbitmapDirectory\fR
.mc |
is \fB/usr/include/X11/bitmaps\fR.
.mc
.P
.iX "resource" "iconImage"
.iX "iconImage resource"
\fBiconImage.\fR  The \f(CWiconImage\fR resource tells the
window manager to use a particular bitmap for an icon
image.  The value that follows this resource is the path to the
bitmap file.  Note that, if specified,
this resource overrides any client-specified image.
.P
.iX "resource" "useClientIcon"
.iX "useClientIcon resource"
\fBuseClientIcon.\fR  The \f(CWuseClientIcon\fR resource determines whether
.mc |
a client-supplied icon image takes precedence over a user-supplied
.mc
image.  If the value
.mc |
is True, then a client-supplied icon image takes precedence
over a user-supplied icon image.  The default value is False, making the
.mc
user-supplied icon image have higher precedence than the client-supplied
icon image.
.H 3 "The Label"
.iX "icon" "label"
.iX "WM_ICON_NAME" "and icon label"
.iX "icon label" "and WM_ICON_NAME"
The icon \fBlabel\fR is a rectangle, located beneath the icon image,
containing the name of the icon.  A label
is
.mc |
usually supplied by the client (via the WM_ICON_NAME 
.mc
window property),
.mc |
but some clients, for example \fBxclock\fR,
.mc
provide a
.mc |
command-line option enabling you to write in your own label.
.mc
.P
Icon labels are truncated on the right to the width of the icon
image when the icon is not active.  When the icon becomes active, the full
.mc |
text of the label appears.
.mc
.iX "icons" "manipulating with a mouse"
.H 2 "Manipulating Icons"
.mc |
Icons can be manipulated like windows: by
.mc
positioning the pointer on the icon and clicking,
double-clicking, or
dragging a mouse button.  The keyboard can also be used to manipulate icons.
.mc |
The following table describes the operations that can be performed on
icons with a mouse.
.mc
.P
.mc |
.TB "Manipulating Icons with a Mouse"
.TS H
center, tab(;), box;
lB | lB | lB
^  | lB | ^
lw(1.5i) | l | l .
To do this...;Position the pointer;Result
.mc
;on the icon and...;
.mc |
_
.TH
T{
Turn an icon into a window.
T};T{
.mc
Double-click the mouse Select button.
T};T{
Restores the window to its former size and location.
T}
_
T{
Move an icon around on the root window.
.mc |
T};T{
Press the mouse Select button and drag.
T};T{
Moves a wire frame with the pointer, showing where the icon is be moved.
.mc
T}
_
T{
Move keyboard input focus.
T};T{
Press the mouse Select button.
T};T{
Makes the icon the focus of input.
T}
.mc |
.br
.ne 15
.mc
_
T{
Move an icon to the top of the window stack.
T};T{
Press the mouse Select button on an icon that has keyboard input focus.
T};T{
.mc |
Moves a partially concealed icon to the top of the window stack.
.mc
T}
_
T{
Display the icon's window menu.
T};T{
.mc |
Click the mouse Select button; or, press \fB<Shift>\fR
\fB<ESC>\fR or \fB<Alt><Space>\fR.
.mc
T};T{
The window menu for an icon is like the window menu of
its associated window.  No window is active while an icon has
the keyboard focus.
T}
.TE
.P
.mc |
The following table describes the operations that can be performed on icons with
the keyboard.  Keyboard actions apply only to active icons.
.br
.ne 8
.mc
.P
.mc |
.iX "icons" "manipulating with the keyboard"
.TB "Manipulating Icons with the Keyboard"
.mc
.TS
center, tab(;), box;
lb | lb
l | l .
To do this...;Use this key action...
.mc |
_
Display or remove icon menu.;\fB<Shift> <ESC>\fR
.mc
;or
.mc |
;\fB<Alt> <Space>\fR
.mc
_
.mc |
When the menu is posted...
.mc
_
Move the cursor to different items;Arrow keys
_
.mc |
Accept the highlighted menu item;\fB<Select>\fR
.mc
_
.mc |
Remove the icon menu;\fB<ESC>\fR
.mc
.TE
.P
The icon's window menu contains the same items as the window menu of the
associated client window.
.mc |
Menu items that do not apply to icons are grayed out
(for example, Minimize and Size).
.mc
.P
.mc |
The following figure shows the appearance of an icon with the input focus.
.sp .5
.FG "Appearance of an Icon with the Input Focus"
.if n .ig +n
.mc
.DS CB
.in +.3i
.PS
.mc |
IPART: box ht 1i wid 1.4i at 0,0
.mc
IMAGE: box ht .9i wid .9i at 0,0 "Image"
LPART: box ht .4i wid 1.5i at IPART - 0,.7i
LABEL: box ht .3i wid 1.4i at LPART "Label"
.\"MENU: box ht 1.4i wid 1.2i with .sw at IPART.nw
.\"======
.mc |
MENU: box ht 1.7i wid 1.7i with .sw at IPART.nw
.mc

.\" labels
.mc |
T1: "Restore"  ljust at MENU - .7,-.60
.mc
T2: "Move"     ljust at T1 - 0,.2
.mc |
T3: "Size"     ljust at T2 - 0,.2
T4: "Minimize" ljust at T3 - 0,.2
.mc
T5: "Maximize" ljust at T4 - 0,.2
T6: "Lower"    ljust at T5 - 0,.2
T7: "Close"    ljust at T6 - 0,.3

.\" keyboard accelerators
.mc |
T1a: "Alt+F5"  ljust at T1 + .80,0
T2a: "Alt+F7"  ljust at T2 + .80,0
T3a: "Alt+F8"  ljust at T3 + .80,0
T4a: "Alt+F9"  ljust at T4 + .80,0
T5a: "Alt+F10" ljust at T5 + .80,0
T6a: "Alt+F3" ljust at T6 + .80,0
T7a: "Alt+F4"  ljust at T7 + .80,0
.mc

.\" mnemonics  (under line of key letter)
line right .08i from T2.nw + 0,.119
line right .08i from T3.nw + 0,.119
line right .08i from T4.nw + 0,.119
line right .08i from T5.nw + .155,.119
line right .08i from T6.nw + .17,.119
line right .08i from T7.nw + 0,.2
line right .08i from T7.sw - 0,.07

.\" menu separator line
.mc |
.\"line right 1.4i from T6.w - .2,.15
line right 1.4i from T6.w - .01,.15
.mc
.\"
.\"=======
.\"T1: "Restore" ljust at MENU - .3,-.55
.\"T2: "Move" ljust at T1 - 0,.2
.\"T3: "Size" ljust at T2 - 0,.2
.\"T4: "Minimize" ljust at T3 - 0,.2
.\"T5: "Maximize" ljust at T4  -0,.2
.\"T6: "Close" ljust at T5 - 0,.3
.\"T7: "-------------" ljust at T5 - 0,.15
.\"=======
.PE
.mc *
.mc
.in -.3i
.mc *
.mc
.DE
.mc |
.+n
.mc
.P
.mc *
.mc
.iX "resource" "iconClick"
.iX "iconClick resource"
\fBiconClick.\fR  The \f(CWiconClick\fR resource controls the posting of
the window menu when the mouse Select button is clicked with the pointer
.mc |
on the icon.  The value of True causes the icon window menu to be
posted and remain posted.  The value of False results in the icon
window menu not remaining posted.
.mc
.P
.iX "resource" "lowerOnIconify"
.iX "lowerOnIconify resource"
\fBlowerOnIconify.\fR  The \f(CWlowerOnIconify\fR resource can be used to
.mc |
control the stacking order of icons.  The default value of True 
places a window's icon on the bottom of the window stack when the window
is minimized (iconified).  A value of False places the icon in the
.mc
stacking order at the same place as its associated window.
.iX "icons" "placement on the workspace"
.H 3 "Controlling Icon Placement"
By default, the window manager places icons in the lower left
corner of the workspace.  Successive icons are placed in a row proceeding
.mc |
toward the right.  Icons are prevented from overlapping.  An icon is
.mc
placed in the position it last occupied if no icon is already there.  If
.mc |
that place is taken, the icon is placed at the next free location.
.mc
.P
.ne 5c
The following three resources enable the user to control the placement
of
icons:
.P
.iX "resource" "iconAutoPlace"
.iX "iconAutoPlace resource"
\fBiconAutoPlace.\fR  The \f(CWiconAutoPlace\fR resource indicates whether
.mc |
the window manager arranges icons in a particular area of the screen or
places each icon where the window was when it was iconified.  The
value True indicates that icons are arranged in a particular area of the
screen, determined by the \f(CWiconPlacement\fP resource.
The value
False indicates that an icon is placed at the location of the window
when it is iconified.
The default is True.
.mc
.P
.iX "iconPlacement resource"
.iX "resource" "iconPlacement"
\fBiconPlacement.\fR  This resource is available only when the
.mc |
\f(CWiconAutoPlace\fR resource has the value True.  The
.mc
\f(CWiconPlacement\fR resource specifies the arrangement scheme the window
manager is to use when placing icons on the workspace.   The default value
is "left bottom."
.P
.mc *
.mc
.ne 10c
.mc |
The following table lists options for \f(CWiconPlacement\fR values.
.mc
.P
.iX "iconPlacement resource" "table of values"
.TB "Options for iconPlacement Values"
.TS
center, box, tab(;);
lB | lB
l | l .
If you want this icon placement...;Choose this scheme...
.mc |
_
.mc
T{
From left to right across the top of the screen, new rows below.
T};left top
_
T{
From right to left across the top of the screen, new rows below.
T};right top
_
T{
From left to right across the bottom of the screen, new rows above.
T};left bottom
_
T{
From right to left across the bottom of the screen, new rows above.
T};right bottom
_
T{
From bottom to top along the left of the screen, new columns to right.
T};bottom left
_
T{
From bottom to top along the right of the screen, new columns to left.
T};bottom right
_
T{
From top to bottom along the left of the screen, new columns to right.
T};top left
_
T{
From top to bottom along the right of the screen, new columns to left.
T};top right
.TE
.mc |
.br
.ne 16
.mc
.P
.iX "icon placement resources" "syntax"
.iX "syntax" "icon placement resources"
.iX "icon placement resources"
.mc |
.P
.iX "iconPlacementMargin resource"
.iX "resource" "iconPlacementMargin"
\fBiconPlacementMargin.\fR  The \f(CWiconPlacementMargin\fR resource
specifies the distance between the screen edge and icons.  The unit of
measurement is pixels.  The default value is equal to the default space
between icons.
.mc
.H 3 "The Syntax for Icon Placement Resources"
The resources that place icons share a common syntax:
.P
.TS
tab(;);
l l l .
.mc |
\fBMwm*\fP\fIresource\fR:;\fIvalue\fR
.mc
.TE
.P
For example, if the user wants automatic placement of icons starting at
the
top
of the screen and proceeding down the right side, the
.mc |
following lines amust be added in the user's \fB.Xdefaults\fR file:
.mc
.P
.TS
tab(;);
l l l l l .
.mc |
\fBMwm*iconPlacement:;top right\fR;;
.mc

.mc |
\fBMwm*iconAutoPlace:;True\fR;;
.mc
.TE
.P
.iX "icons" "configuring appearance and behavior"
.H 2 "Controlling Icon Appearance and Behavior"
.mc |
MWM offers a number of resources to control the
appearance and behavior of icons.  Among these are resources that
.mc
enable the user to select icon decoration, control icon size, and create new
icon pixmaps.
.iX "iconDecoration resource"
.iX "iconDecoration resource" "table of values"
.iX "resource" "iconDecoration"
.iX "decoration" "icons"
.H 3 "Selecting Icon Decoration"
\fBiconDecoration.\fR  The \f(CWiconDecoration\fR resource indicates
.mc |
which parts of an icon are to be displayed.  The following table shows the possible
values for \f(CWiconDecoration\fR and the results.
.mc
.P
.mc |
.ne 2.5i
.mc
.TB "The Values for Controlling Icon Appearance"
.TS
center, box, tab(;);
lB | lB
l | l .
For an icon that looks like this...;Use this value...
.mc *
.mc
_
.mc |
Just the label;label
.mc
_
.mc |
Just the image;image
_
Both label and image;T{
label image (default value when icon box is used.)
.mc
T}
_
T{
.mc |
The label of an active icon is not truncated
.mc
T};T{
.mc |
activelabel label image (default value when icon box is not used.)
.mc
T}
.TE
.P
.iX "icons" "sizing"
.iX "sizing" "icons"
.H 3 "Sizing Icons"
Each icon image has a maximum and minimum size.  MWM
.mc |
has maximum and minimum
default sizes as well as maximum and minimum allowable
.mc
sizes.
The following two resources control icon image size:
.P
.iX "resource" "iconImageMaximum"
.iX "iconImageMaximum resource"
.iX "iconImageMinimum resource"
.iX "resource" "iconImageMinimum"
\fBiconImageMaximum.\fR  The \f(CWiconImageMaximum\fR resource limits the
maximum size of an icon image.  The largest value allowed is 128x128
pixels.  The default value is 50x50 pixels.
.P
\fBiconImageMinimum.\fR  The \f(CWiconImageMinimum\fR resource limits the
minimum size of an icon image.  The smallest value allowed is 16x16 pixels.
.mc |
The default value is 16x16 pixels.
.mc
.P
When calculating limits for image size, remember that the width of
an
.mc |
icon is the image width plus the icon frame plus the
space between icons.  The height of an icon is the image height plus
the icon frame plus the space between icons.
.mc
The amount of icon decoration and the size of font used in the icon
.mc |
label also affects the height of the icon.
.mc
.P
How the window manager treats an
icon depends on the size of the image in relation to the maximum
and
.mc |
minimum sizes, as shown in the following table.
.mc
.P
.ne 1.75i
.TB "Icon Size Affects Treatment of Icon"
.TS
center, box, tab(;);
lB | lB
l | l .
.mc |
If an icon image is...;The window manager...
.mc
_
.mc |
Smaller than the minimum size;Act as if no image was specified
_
Within maximum and minimum limits;T{
Center the image within the maximum area
.mc
T}
_
.mc |
Larger than the maximum size;T{
.mc
Clip the right side and bottom of the image to fit the maximum
.mc |
size
.mc
T}
.TE
.iX "syntax" "icon appearance resources"
.iX "icon appearance resources" "syntax"
.H 3 "The Syntax for Resources that Control Icon Appearance"
The resources that control icon appearance have the following
syntax:
.mc |
.sp
.mc
.TS
tab(;);
l l l .
.mc |
;\fBMwm*\fR\fIresource\fR:;\fIvalue\fR
.mc
.TE
.mc |
.sp
.mc
For example, the label part of the icon can be eliminated using the
\f(CWiconDecoration\fR resource:
.P
.TS
tab(;);
l l l .
.mc |
;\fBMwm*iconDecoration:  image\fR
.mc
.TE
.P
The \f(CWiconImage\fR resource has three syntaxes available:
.P
.AL
.LI
.mc |
To use the image for all classes of clients, the syntax is
.mc
.P
.TS
tab(;);
l l .
.mc |
; \fBMwm*iconImage:\fR    \fIpath/bitmap\fR
.mc
.TE
.mc |
.br
.ne 8
.mc
.P
.LI
.mc |
To use the image for a specific class of clients, or a specific client, the syntax is
.mc
.P
.TS
tab(;);
l l .
.mc |
; \fBMwm*\fR\fIclientclass\fR\fB*iconImage:\fR   \fIpath/bitmap\fR
.mc

.mc |
; \fBMwm*\fR\fIclientname\fR\fB*iconImage:\fR   \fIpath/bitmap\fR
.mc
.TE
.P
.LI
.mc |
To use the image as the default image whenever the client class is unknown, the syntax is
.mc
.P
.TS
tab(;);
l l .
.mc |
; \fBMwm*defaults*iconImage:\fR     \fIpath/bitmap\fR
.mc
.TE
.mc *
.mc
.LE
.mc |
.br
.ne 15
.sp
For example, users who want to use a personal \fBkaleidoscope\fR bitmap
for a terminal emulator window (say, \fBmterm\fR) and see a complete label
whenever any icon is active, must add the following lines in
their \fB.Xdefaults\fR file:
.sp
.mc
.TS
tab(;);
l l l .
.mc |
\fBMwm*mterm*iconImage:;/users\fR/\fIusername\fR\fB/Bitmaps/kaleid.bits\fR
.sp
\fBMwm*iconDecoration:;label activelabel image\fR
.mc
.TE
.iX "icons" "coloring frame elements"
.iX "coloring" "icon frame elements"
.H 2 "Coloring Icons"
Icon window frame elements are colored with the same resources as normalized
.mc |
window frame elements.  (These resources are discussed in Chapter 13.)  The following
table lists the color resources.
.mc
.P
.ne 2.2i
.mc |
.in0
.mc
.TB "MWM Color Resources and What They Color"
.TS
center, tab(;), box;
lb | lb | lb
lb | lb | ^
l | l | l .
Inactive Window and;Active Window and;Area Colored
Icon Resources;Icon Resources;
.mc |
_
.mc
foreground;activeForeground;Foreground areas.
_
background;activeBackground;Background areas.
_
.mc |
topShadowColor;activeTopShadowColor;Top and left 3D bevels.
.mc
_
bottomShadowColor;activeBottomShadowColor;T{
.mc |
Bottom and right 3D bevels.
.mc
T}
.TE
.mc |
.in
.br
.ne 5
.P
.mc
The image part of an icon can be displayed with client specific colors.
The following table lists icon image elements and the resources
.mc |
that control their color.
.mc
.iX "iconImageBackground resource"
.iX "iconImageTopShadowColor resource"
.iX "iconImageBottomShadowColor resource"
.iX "iconImageForeground resource"
.iX "resource" "iconImageBackground"
.iX "resource" "iconImageTopShadowColor"
.iX "resource" "iconImageBottomShadowColor"
.iX "resource" "iconImageForeground"
.mc |
.P
.mc
.ne 5c
.mc |
.in0
.TB "Coloring Icon Images with MWM Resources"
.mc
.TS
center, tab(;), box;
lB | lB
l | l .
To color this...;Use this resource...
.mc *
.mc
_
.mc |
Icon image background;iconImageBackground
_
.mc
T{
.mc |
Left and upper 3D bevels
T};iconImageTopShadowColor
.mc
_
.mc |
Right and lower three-dimensional bevels;iconImageBottomShadowColor
.mc
_
.mc |
Icon image foreground;iconImageForeground
.mc
.TE
.mc |
.in
.P
Two pixmap resources are available for use in shading icon images:
.mc
.iX "iconImageTopShadowPixmap resource"
.iX "resource" "iconImageTopShadowPixmap"
.P
\fBiconImageTopShadowPixmap.\fR  The default
value for \f(CWiconImageTopShadowPixmap\fR is the icon top shadow pixmap
.mc |
(specified by \fBMwm*icon*topShadowPixmap\fR).
.mc
.P
.iX "iconImageBottomShadowPixmap resource"
.iX "resource" "iconImageBottomShadowPixmap"
\fBiconImageBottomShadowPixmap.\fR.  The default value for
\f(CWiconImageBottomShadowPixmap\fR is the icon bottom
.mc |
shadow pixmap (specified by \fBMwm*icon*bottomShadowPixmap\fR).
.mc
.P
.mc |
Procedures for making pixmaps for frame elements are described in Section 13.9.
.mc
.iX "icon coloring resources" "syntax"
.iX "syntax" "icon coloring resources"
.H 3 "The Syntax for Icon Coloring Resources"
The resources that color icons can have any of three different
syntaxes.
.AL
.LI
.mc |
To color all clients regardless of class, the syntax is
.mc
.TS
tab(;);
l l l .
.mc |
\fBMwm*icon*\fR\fIresource\fR:;\fIcolor\fR
.mc
.TE
.mc |
.sp
For example, a proper use of this syntax is to ensure
.mc
that all icon backgrounds were the same color, a good thing
.mc |
for consistency:
.mc
.TS
tab(;);
l l l .
.mc |
\fBMwm*icon*background:;Cadet Blue\fR
.mc
.TE
.LI
.mc |
.ne 10
To color specific classes of clients, the syntax is
.mc
.TS
tab(;);
l l l .
.mc |
\fBMwm*icon*\fR\fIclientclass or clientname\fR*\fIresource\fR:;\fIcolor\fR
.mc
.TE
.mc |
.sp
.mc
The colors specified with this resource take precedence over
any other specification for this resource for this class of clients.
.LI
.mc |
To color any client with an unknown class, the syntax is
.mc
.TS
tab(;);
l l l .
.mc |
\fBMwm*icon*default*\fR\fIresource:\fR;\fIcolor\fR
.mc
.TE
.P
.LE
.iX "icon box"
.H 2 "Using the Icon Box to Hold Icons"
The user may want to use an icon box to contain icons instead
.mc |
of having standalone icons on the workspace.  An icon box is an MWM
.mc
window and frame that can
reduce the amount of "real estate" taken up by client icons.  The icon box
is a scrollable window that displays icons in a grid (rows and columns).
.P
.iX "useIconBox resource"
.iX "resource" "useIconBox"
\fBuseIconBox.\fR  The \f(CWuseIconBox\fR resource enables the
icon box window manager facility.  The value of
.mc |
True places icons in an icon box.  The default value of False 
places icons on the root window.
.mc
.P
.iX "iconBoxName resource"
.iX "resource" "iconBoxName"
\fBiconBoxName.\fR  The \f(CWiconBoxName\fR resource specifies the name
.mc |
that is used to look up icon box resources.  The default name is iconbox.
.mc
.P
.iX "iconBoxTitle resource"
.iX "resource" "iconBoxTitle"
\fBiconBoxTitle.\fR  The \f(CWiconBoxTitle\fR resource specifies the name
that is used in the title area of the icon box frame.  The default name is
.mc |
Icons.
.mc
.iX "icon box" "configuring appearance and behavior"
.mc |
.br
.ne 15
.mc
.H 3 "Controlling Icon Box Appearance and Behavior"
The icon box is displayed in a standard window management client
frame.  Client-specific resources for the icon box can be
specified using "iconbox" as the client name.
.mc |
.sp
.mc
.TS
tab(;);
l l .
.mc |
;\fBMwm*iconbox*\fR\fIresource:   value\fR
.mc
.TE
.P
Resources that may be used with the icon box are:
\f(CWclientDecoration\fR; all window resources dealing with color,
shadow, and matte; and \f(CWwindowMenu\fR.
.P
.iX "iconBoxGeometry resource"
.iX "resource" "iconBoxGeometry"
\fBiconBoxGeometry.\fR  The \f(CWiconBoxGeometry\fR resource is
used to set the initial size and placement of the icon box.
If the \f(CWiconBoxGeometry\fR resource is set, the
largest dimension of the size determines if the icons are
placed in a row or a column.  The default
.mc |
policy is to place icons in rows going from left to
.mc
right, top to bottom.
.mc |
.br
.ne 6
.mc
.P
The value of the resource is a
standard window geometry string with the following syntax:
.mc |
.sp
.mc
.TS
tab(;);
l l .
.mc |
;[=][<width>x<height>][{+-}<xoffset>{+-}<yoffset>]\fP
.mc
.TE
.P
.mc |
If the width and height are not provided, the \fBiconPlacement\fR policy is
used to determine the initial size and orientation (horizontal and vertical).
.mc
.P
.mc |
The actual size of the icon box window depends on the
.mc
\f(CWiconImageMaximum\fR (size) and \f(CWiconDecoration\fR resources.  The
default value for size is (6 * iconWidth + padding) wide by (1 * iconHeight
+ padding) high.  The default location (value) is +0-0.
.iX "icon box" "window menu"
.iX "window menu" "icon box"
.H 4 "The Icon Box Window Menu"
.br
The window menu for the icon box differs from the standard window
.mc |
menu in that it does not contain the "Close" selection.  In its place is
the "PackIcons" selection, as shown in the following figure.
.mc
.P
.ne 2.25i
.mc |
.FG "The System Menu for the Icon Box"
.mc
.TS
center, box, tab(;);
l  l  .
.mc |
Restore;Alt+F5
.mc
Move;Alt+F7
Size;Alt+F8
Minimize;Alt+F9
Maximize;Alt+F10
.mc |
Lower;Alt+F3
PackIcons;Alt+Shift+F7
.mc
.TE
.P
.mc *
.mc
.P
.P
.iX "f.pack_icons function"
.mc |
\fBf.pack_icons\fR  The function \fBf.pack_icons\fR shifts icons to
.mc
fill empty spaces in the icon placement grid so that the icons appear in
neat, complete rows.  Icons in the icon box do not overlap.
.P
If there are icons that cannot be displayed in the visible part of
the icon box, the user can scroll to see the icons.  The sliders within
the scroll bars show the extent of the icon grid that is
visible.
.P
The icon box can be minimized (iconified) just like any other
window.  If the icon box is minimized, it is placed into the
icon grid on the workspace.
.iX "icons" "manipulating in icon box"
.H 3 "Controlling Icons in the Icon Box"
Every client window that can be iconified has an icon in the icon
box, even when the window is in the normal state.  The icon for a
client is put into the icon box when the client becomes managed
by the window manager and is removed from the icon box when the
client withdraws from being managed.
.P
.iX "fadeNormalIcon resource"
.iX "resource" "fadeNormalIcon"
\fBfadeNormalIcon.\fR   Icons for windows in the normal state are
visually distinct from icons for windows that are iconified.
.mc |
Icons for windows that are iconified look like standalone icons.
Icons for windows that are in the normal state appear
flat and are optionally grayedout.  The value of True for the
.mc
\f(CWfadeNormalIcon\fR resource grays out normalized windows.  The
.mc |
default value is False.
.mc
.iX "iconDecoration resource" "icon box"
.P
The text and image attributes of icons in icon boxes are
.mc |
determined in the same way as for standalone icons, using the
.mc
\f(CWiconDecoration\fR resource.
.P
.iX "cursor" "in icon box"
A standard "control" location cursor is used to indicate the
particular icon in the icon box to which keyboard actions apply.
.mc |
The location cursor is a rectangle outline that surrounds
.mc
the icon.
.P
Icons contained in the icon box can be manipulated with the mouse
and from the keyboard.  Mouse button actions apply whenever the
.mc |
pointer is on any part of the icon.  The following table describes the
mouse button actions for controlling icons.
.mc
.P
.ne 2i
.iX "icons" "manipulating in icon box with a mouse"
.mc |
.TB "Controlling Icons in the Icon Box with a Mouse"
.mc
.TS
box, center, tab(;);
lb | lb
l | l  .
Button Action;Function Description
.mc *
.mc
_
.mc |
Click the mouse Select button;Selects the icon
_
T{
Double-click mouse Select button
.br
with pointer on an icon of an
.br
iconified window
T};T{
Normalizes the iconified window
.mc
T}
_
.mc |
T{
Double-click mouse Select button
.br
with pointer on an icon of a
.br
normalized window
T};T{
Raises the normalized window to the top of the stack
.mc
T}
.TE
.P
.iX "icons" "manipulating in icon box with keyboard"
.mc |
An icon is manipulated from the keyboard by making the icon
.mc
box the active window and then using the arrow keys to traverse the icons
in the icon
.mc |
box.  Pressing <\fBEnter\fR> does the default action for the
.mc
selected icon:  for an icon of a normalized window, the window
.mc |
is raised; for an icon of an iconified window, the window
is normalized.
.mc
.P
.iX "iconbox" "<\fITab\fR> key"
.mc |
Pressing <\fBTab\fR> moves the keyboard input focus around the box in this
order:  icons, scroll bars, icons.
Pressing the <\fBShift\fR> <\fBTab\fR> sequence moves the focus in the opposite direction.

.mc
