.mc |
.\"  (c) Copyright 1989, 1990 OPEN SOFTWARE FOUNDATION, INC.
.\"  (c) Copyright 1989, DIGITAL EQUIPMENT COROBPORATION, MAYNARD, MASS.
.mc
.\"  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
.\"  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
.\"  (c) Copyright 1988 MICROSOFT CORPORATION
.\"  ALL RIGHTS RESERVED
.mc |
.nr H1 15
.mc
.H 1 "Quick Reference Tables"
.mc |
The reference tables in this chapter provide a quick way to look
.mc
up information on resources and functions.
.mc |
All of this information is covered in more detail in Chapters 13, 14, and 15.
.mc
The chapter begins with a review of the syntax patterns used with MWM
resources.
.mc |
.br
.ne 20
.mc
.H 2 "Syntax for Resources"
.iX "syntax" "for resources, quick reference"
.iX "resources" "syntax, quick reference"
.mc |
There are four syntax patterns used with OSF/Motif resources:
.mc
.AL
.LI
Resources controlling the general appearance of elements use this syntax:
.mc |
.sp
\fBMwm*\fR\fIresource: value\fR
.mc
.P
.mc *
.mc
.LI
Resources controlling elements of particular objects use this syntax:
.mc |
.sp
\fBMwm*[menu | icon | client | feedback]*\fR\fI resource: value\fR
.sp
.mc
To specifically configure the title area of a client window frame, use this
syntax:
.mc |
.sp
\fBMwm*client*title*\fR\fIresource: value\fR
.sp
.mc
To specifically configure the appearance of a menu, use this syntax:
.mc |
.sp
\fBMwm*menu*\fR\fImenuname\fR*\fIresource: value\fR
.sp
.mc
.LI
Resources to be used for specific clients use this syntax:
.mc |
.sp
\fBMwm*\fR[\fIclientclass | clientname\fR]*\fIresource: value\fR
.sp
.mc
.LI
Resources controlling appearance and behavior in default situations use
this syntax:
.mc |
.sp
\fBMwm*defaults*\fR\fIresource: value\fR
.mc
.LE
.iX "resource tables"
.H 2 "Resource Tables"
In the resource tables that follow, the number(s) in the syntax column
.mc |
refer to the preceding syntax patterns.
.mc
.P
The following tables are presented:
.BL
.LI
.mc |
Coloring (Table 16-1)
.mc
.LI
.mc |
Shading (Table 16-2)
.mc
.LI
.mc |
Configuring Window Decorations (Table 16-3)
.mc
.LI
.mc |
Configuring Focus Policies (Table 16-4)
.mc
.LI
.mc |
Controlling Windows (Table 16-5)
.mc
.LI
.mc |
Controlling Window Size and Position (Table 16-6)
.mc
.LI
.mc |
Configuring Icons (Table 16-7)
.LI
Configuring the Icon Box (Table 16-8)
.LI
Valid Window Manager Functions (Table 16-9)
.mc
.LE
.H 3 "Coloring Windows, Icons, Menus, and Mattes"
.iX "coloring windows" "resource table"
.iX "coloring icons" "resource table"
.iX "quick reference table" "coloring"
.iX "coloring mattes" "resource table"
Resources used to color elements of
.mc |
active and inactive frames, mattes, and icon images are listed in the following table.
Coloring is discussed in Chapter 13 (windows, icons,
menus, and mattes) and Chapter 15 (icon images).
.mc
.P
.mc |
.br
.ne 15
.mc
.iX "activeBackground" "resource"
.iX "activeBottomShadowColor" "resource"
.iX "activeForeground" "resource"
.iX "activeTopShadowColor" "resource"
.iX "background" "resource"
.iX "bottomShadowcolor" "resource"
.iX "foreground" "resource"
.iX "topShadowColor" "resource"
.iX "matte Background" "resource"
.iX "matteBottomShadowColor" "resource"
.iX "matteForeground" "resource"
.iX "matteTopShadowColor" "resource"
.iX "matteWidth" "resource"
.iX "iconImageBackground" "resource"
.iX "iconImageBottomShadowColor" "resource"
.iX "iconImageForeground" "resource"
.iX "iconImageTopShadowColor" "resource"
.mc |
.in0
.TB "Coloring"
.mc
.TS
box, tab(;);
lb | lb | lb | lb | lb
 ^ | ^ | lb | ^ | ^
l | l | l | l | l .
Name;Class;Value;Default;Syntax
;;Type;;
.mc
_
.mc |
\fBWindows, Icons, and Menus\fR
.mc
_
activeBackground;Background;color;varies*;1,2
_
activeBottomShadowColor;Foreground;color;varies*;1,2
_
activeForeground;Foreground;color;varies*;1,2
_
activeTopShadowColor;Background;color;varies*;1,2
_
background;Background;color;varies*;1,2
_
bottomShadowColor;Foreground;color;varies*;1,2
_
foreground;Foreground;color;varies*;1,2
_
topShadowColor;Background;color;varies*;1,2
_
.mc |
\fBMattes\fR
.mc
_
matteBackground;Background;color;background;1,3,4
_
.mc |
matteBottomShadowColor;Foreground;color;T{
bottom shadow color
T};1,3,4
.mc
_
matteForeground;Foreground;color;foreground;1,3,4
_
.mc |
matteTopShadowColor;Background;color;T{
top shadow color
T};1,3,4
.mc
_
matteWidth;MatteWidth;pixels;0;1,3,4
_
.mc |
\fBIcon Images\fR
.mc
_
.mc |
iconImageBackground;Background;color;T{
icon background
T};1,3,4
.mc
_
.mc |
iconImageBottomShadowColor;Foreground;color;T{
icon bottom shadow
T};1,3,4
.mc
_
.mc |
iconImageForeground;Foreground;color;T{
varies
T};1,3,4
.mc
_
.mc |
iconImageTopShadowColor;Background;color;T{
icon top shadow color
T};1,3,4
.mc
.TE
*The default is chosen based on the visual type of the screen.
.mc |
.in
.mc
.iX "pixmaps" "quick reference table"
.iX "resources" "for pixmaps, quick reference"
.iX "quick reference table" "shading (pixmaps)
.H 3 "Shading"
Resources used to shade elements of windows, icons, menus,
mattes, and
icon images are presented in the following table.  Shading resources are
most valuable when used with a monochrome display.  In addition, the
\f(CWcleanText\fR resource is available to reduce interference between
.mc |
shading used in the title bar and the title.  Shading is discussed in
Chapter 13 (windows, icons, menus, and mattes) and Chapter 15 (icon images).
.br
.ne 40
.mc
.iX "activeBackgroundPixmap resource"
.iX "activeBottomShadowPixmap resource"
.iX "activeTopShadowPixmap resource"
.iX "backgroundPixmap resource"
.iX "bottomShadowPixmap resource"
.iX "topShadowPixmap resource"
.iX "cleanText resource"
.iX "matteBottomShadowPixmap resource"
.iX "iconImageBottomShadowPixmap resource"
.iX "iconImageTopShadowPixmap resource"
.P
.mc |
.in0
.TB "Shading"
.mc
.TS
box, tab(;);
lb | lb | lb | lb | lb
 ^ | ^ | lb | ^ | ^
l | l | l | lw(.75i) | l .
Name;Class;Value;Default;Syntax
;;Type;;
.mc
_
.mc |
\fBWindows, Icons,
and Menus\fR
.mc
_
activeBackgroundPixmap;BackgroundPixmap;string;varies*;1,2
_
.mc |
activeBottom
    ShadowPixmap;BottomShadowPixmap;string;varies*;1,2
.mc
_
activeTopShadowPixmap;TopShadowPixmap;string;varies*;1,2
_
backgroundPixmap;BackgroundPixmap;string;varies*;1,2
_
bottomShadowPixmap;BottomShadowPixmap;string;varies*;1,2
_
topShadowPixmap;TopShadowPixmap;string;varies*;1,2
_
cleanText;CleanText;T/F;T;1
_
.mc |
\fBMattes\fR
.mc
_
.mc |
matteBottom
    ShadowPixmap;BottomShadowPixmap;color;T{
bottom shadow pixmap
T};1,3,4
.mc
_
.mc |
matteTop
    ShadowPixmap;TopShadowPixmap;color;T{
top
.br
shadow
.br
pixmap
T};1,3,4
.mc
_
.mc |
\fBIcon Images\fR
.mc
_
.mc |
iconImageBottom
    ShadowPixmap;BottomShadowPixmap;color;T{
icon
.br
bottom
.br
shadow
.br
pixmap
T};1,3,4
.mc
_
.mc |
iconImageTop
    ShadowPixmap;TopShadowPixmap;color;T{
icon
.br
top
.br
shadow
.br
pixmap
T};1,3,4
.mc
.TE
*The default is chosen based on the visual type of the screen.
.mc |
.in
.br
.ne 10
.mc
.iX "window decorations" "quick reference table"
.iX "quick reference table" "window decorations"
.H 3 "Window Decorations"
Resources used to declare applicable functions and decoration
elements for a client are presented in the following table.  (Window
.mc |
Decoration is discussed in Chapter 13.)
.br
.ne 20
.mc
.P
.mc |
.in0
.TB "Configuring Window Decorations"
.mc
.TS
allbox, tab(;);
lb  lb  lb  lb lb
l  l  l  l l .
.mc
Name;Class;Value Type;Default;Syntax
_
.iX "clientDecoration resource"
.iX "clientFunctions resource"
.iX "transientDecoration resource"
.iX "transientFunctions resource"
clientDecoration;ClientDecoration;string;all;1,3,4
clientFunctions;ClientFunctions;string;all;1,3,4
.mc |
transientDecoration;TransientDecoration;string;T{
menu
.br 
title
.br
resizeh
T};1
transientFunctions;TransientFunctions;string;T{
-minimize -maximize
T};1
.mc
.TE
.mc |
.in
.mc
.br
.ne 4i
.iX "focus policies" "quick reference table"
.iX "quick reference table" "focus policies"
.mc |
.br
.ne 35
.mc
.H 3 "Focus Policies"
Resources used to control colormap and keyboard input focus policies are
presented in the following table.  (Focus policies are discussed in Chapter
.mc |
13.)
.mc
.P
.iX "autoKeyFocus resource" 
.iX "colormapFocusPolicy resource" 
.iX "deiconifyKeyFocus resource" 
.iX "enforceKeyFocus resource" 
.iX "focusAutoRaise resource" 
.iX "keyboardFocusPolicy resource" 
.iX "passSelectButton resource" 
.iX "startupKeyFocus resource" 
.iX "passButtons resource"
.mc |
.in0
.TB "Configuring Focus Policies"
.mc
.TS
allbox, tab(;);
lb  lb  lb  lb lb
l  l  l  l l .
.mc
Name;Class;Value Type;Default;Syntax
_
autoKeyFocus;AutoKeyFocus;T/F;T;1
colormapFocusPolicy;ColormapFocusPolicy;string;keyboard;1
deiconifyKeyFocus;DeiconifyKeyFocus;T/F;T;1
enforceKeyFocus;EnforceKeyFocus;T/F;T;1
.mc |
focusAutoRaise;FocusAutoRaise;T/F;varies*;1,3,4
.mc
keyboardFocusPolicy;KeyboardFocusPolicy;string;explicit;1
passButtons;PassButtons;T/F;F;1
passSelectButton;PassSelectButton;T/F;T;1
startupKeyFocus;StartupKeyFocus;T/F;T;1
.TE
.mc |
*The default depends on the value of keyboardFocusPolicy.
.in
.mc
.iX "quick reference table" "controlling windows"
.iX "windows" "resources that control, quick reference table"
.mc |
.br
.ne 35
.mc
.H 3 "Controlling Windows"
Resources used to control miscellaneous aspects of window management and
.mc |
behavior are presented in the following table.  (These resources are discussed
.mc
in the chapters indicated after the subheadings in the table.)
.iX "buttonBindings resource"
.iX "keyBindings resource"
.iX "quitTimeout resource"
.iX "saveUnder resource"
.iX "fontList resource"
.iX "doubleClickTime resource"
.iX "bitmapDirectory resource"
.iX "configFile resource"
.iX "wMenuButtonClick resource"
.iX "wMenuButtonClick2 resource"
.iX "windowMenu resource"
.P
.mc |
.in0
.TB "Controlling Windows"
.mc
.TS
box, tab(;);
lb | lb | lb | lb | lb
 ^ |  ^ | lb | ^ | ^
l | l | l | l | l .
Name;Class;Value;Default;Syntax
;;Type;;
.mc
_
.mc |
\fBBindings\fR (Chapter 14)
.mc
_
.mc |
buttonBindings;ButtonBindings;string;"DefaultButtonBindings";1
.mc
_
.mc |
keyBindings;KeyBindings;string;"DefaultKeyBindings";1
.mc
_
.mc |
T{
\fBScreen Management\fR (Chapter 13)
T}
.mc
_
.mc |
multiScreen;MultiScreen;T/F;F
screens;Screens;string;varies*
_
T{
\fBClient Management\fR (Chapter 13)
T}
_
.mc
quitTimeout;QuitTimeout;millisec.;1000;1
_
saveUnder;SaveUnder;T/F;F;1,2
_
.mc |
\fBFont\fR (Chapter 13)
.mc
_
.mc |
fontList;FontList;string;T{
display based
T};1,2
.mc
_
.mc |
T{
\fBMouse Timing\fR 
.br
(Chapter 14)
T}
.mc
_
doubleClickTime;DoubleClickTime;millisec.;500;1
_
.mc |
T{
\fBResource 
.br
Directories\fR 
.br
(Chapters 14 and 15)
T}
.mc
_
.mc |
bitmapDirectory;BitmapDirectory;directory;T{
/usr/include/\e
.br
X11/bitmaps
T};1
.mc
_
configFile;ConfigFile;<file>;.mwmrc;1
_
.mc |
T{
\fBWindow Menus\fR 
.br
(Chapter 14)
T}
.mc
_
wMenuButtonClick;WMenuButtonClick;T/F;T;1
_
wMenuButtonClick2;WMenuButtonClick2;T/F;T;1
_
.mc |
windowMenu;WindowMenu;string;"DefaultWindowMenu";1,3,4
.mc
.TE
.mc |
*The default depends on the number of screens available.
.in
.mc
.ne 5i
.H 3 "Controlling Window Size and Position"
Resources used to control the size and position of windows are presented
.mc |
in the following table.  (Window Size and Position are discussed in Chapter 13.)
.mc
.iX "quick reference table" "controlling window size and position"
.iX "sizing" "resources, quick reference table"
.iX "positioning" "resources, quick reference table"
.iX "interactivePlacement resource"
.iX "limitResize resource"
.iX "maximumMaximumSize resource"
.iX "moveThreshold resource"
.iX "positionIsFrame resource"
.iX "positionOnScreen resource"
.iX "resizeBorderWidth resource"
.iX "resizeCursors resource"
.iX "maximumClientSize resource"
.iX "frameBorderWidth resource"
.iX "clientAutoPlace resource"
.iX "showFeedback resource"
.P
.mc |
.in0
.TB "Controlling Window Size and Position"
.mc
.TS
box, tab(;);
lb | lb | lb | lb | lb
 ^ | ^ | lb | ^ | ^ 
l | l | l | lw(.75i) | l .
Name;Class;Value;Default;Syntax
;;Type;;
.mc
_
.mc |
\fBSize Resources\fR
.mc
_
frameBorderWidth;FrameBorderWidth;pixels;5;1
_
limitResize;LimitResize;T/F;T;1
_
.mc |
maximumClientSize;MaximumClientSize;wxh ;T{
fill the
.br
screen
T};1,3,4
.mc
_
.mc |
maximumMaximumSize;MaximumMaximumSize;wxh;T{
2X 
.br
screen
.br
w&h
T};1
.mc
_
resizeBorderWidth;ResizeBorderWidth;pixels;10;1
_
resizeCursors;ResizeCursors;T/F;T;1
_
.mc |
\fBPosition Resources\fR
.mc
_
clientAutoPlace;ClientAutoPlace;T/F;T;1
_
interactivePlacement;InteractivePlacement;T/F;F;1
_
moveThreshold;MoveThreshold;pixels;4;1
_
positionIsFrame;PositionIsFrame;T/F;T;1
_
positionOnScreen;PositionOnScreen;T/F;T;1
_
showFeedback;ShowFeedback;string;all;1
.mc |
_
\fBOther resources\fR
_
enableWarp;EnableWarp;T/F;T;1
.mc
.TE
.mc |
.in
.br
.ne 4i 
.mc
.H 3 "Configuring Icons"
Resources used to configure icons are presented in the following table.
.mc |
(Icons are discussed in Chapter 15.)
.mc
.P
.iX "icons" "resources that configure, quick reference table"
.mc |
.iX "quick reference table" "configuring icons"
.mc
.iX "iconAutoPlace resource"
.iX "iconClick resource"
.iX "iconImage resource"
.iX "iconImageMaximum resource"
.iX "iconImageMinimum resource"
.iX "iconPlacement resource"
.iX "iconPlacementMargin resource"
.iX "lowerOnIconify resource"
.iX "useClientIcon resource"
.mc |
.in0
.TB "Configuring Icons"
.mc
.TS
allbox, tab(;);
lb  lb  lb  lb lb
l  l  l  l l .
.mc
Name;Class;Value Type;Default;Syntax
_
iconAutoPlace;IconAutoPlace;T/F;T;1
iconClick;IconClick;T/F;T;1
iconDecoration;IconDecoration;string;all*;1
iconImage;IconImage;pathname;varies\(dg;1,3,4
iconImageMaximum;IconImageMaximum;wxh;50x50;1
.mc |
iconImageMinimum;IconImageMinimum;wxh;16x16;1
.mc
iconPlacement;IconPlacement;string;left bottom;1
iconPlacementMargin;IconPlacementMargin;number;varies\(dd;1
lowerOnIconify;LowerOnIconify;T/F;T;1
useClientIcon;UseClientIcon;T/F;F;1,3,4
.TE
.mc |
*MWM defaults are "label image" when an icon box is used and 
"activelabel label 
.mc
.br
.mc |
image" when an icon box is not used.
.br
.mc
\(dgThe default value is determined by the client's WM_HINTS values and MWM
defaults.
.br
\(ddThe default value is equal to the space between icons as they are placed
on the screen.
.mc |
.br
.in
.ne 4.5i
.mc
.H 3 "Configuring the Icon Box"
.mc |
Resources used to configure the icon box are presented in the following table.
(The icon box is discussed in Chapter 15.)  
.mc
.iX "quick reference table" "configuring the icon box"
.iX "icon box" "resources that configure, quick reference table"
.iX "fadeNormalIcon resource"
.iX "iconBoxGeometry resource"
.iX "iconBoxName resource"
.iX "iconBoxTitle resource"
.iX "useIconBox resource"
.P
.mc |
.in0
.TB "Configuring the Icon Box"
.mc
.TS
center, allbox, tab(;);
lb  lb  lb  lb lb
l  l  l  l l .
.mc
Name;Class;Value Type;Default;Syntax
_
fadeNormalIcon;FadeNormalIcon;T/F;F;1
iconBoxGeometry;IconBoxGeometry;string;6x1+0-0;1
iconBoxName;IconBoxName;string;iconbox;1
iconBoxTitle;IconBoxTitle;string;Icons;1
useIconBox;UseIconBox;T/F;F;1
.TE
.mc |
.in
.br
.ne 6i
.mc
.iX "MWM functions" "quick reference table"
.iX "quick reference table" "MWM functions"
.H 2 "MWM Functions"
Window manager functions can be accessed with a mouse button binding, a
.mc |
keyboard binding, and/or a menu item.  (Functions, menus, and bindings,
including the contexts" in which they can be used, are
discussed in Chapter 14.)
.mc
.P
.iX "functions" "quick reference table"
The following table lists the valid functions for the OSF/Motif
.mc |
Window Manager.  In the Contexts column, R stands
for Root, I stands for Icon, and W stands for Window.  In the 
Devices column,
M stands for Mouse, K stands for Key, and ME stands for Menu. 
.mc
.P
.TB "Valid Window Manager Functions"
.mc |
.in 0
.mc
.TS H
box,tab(;);
cBs | cBss | cBss
l | l | c | c | c | c | c | c
l | lw(1.5i) | c | c | c | c | c | c .
Functions; Contexts; Devices
_
.mc |
\fBName\fP;\fBDescription\fP;\fBR\fP;\fBI\fP;\fBW\fP;\fBMO\fP;\fBK\fP;\fBME\fP
_
.mc
.TH
.mc |
_
\fBf.beep\fR;T{
.mc
Causes a beep to sound.
T};x;x;x;x;x;x
_
.mc |
\fBf.circle_down\fR;Puts window on;x;x;x;x;x;x
[icon | window];bottom
of stack.*;;;;;;
.mc
_
.mc |
\fBf.circle_up\fR;Puts window on top;x;x;x;x;x;x
[icon | window];of stack.*;;;;;;
.mc
_
.mc |
\fBf.exec\fR (or) !;Uses \fB/bin/sh\fR to exe-;x;x;x;x;x;x
command;cute a command.;;;;;;
.mc
_
.mc |
\fBf.focus_color\fR;T{
Sets colormap focus when colormap focus policy
is explicit.*
.mc
T};x;x;x;x;x;x
_
.mc |
\fBf.focus_key\fR;T{
Sets keyboard
input focus when keyboard focus policy is explicit.*
.mc
T};x;x;x;x;x;x
_
.mc |
\fBf.kill\fR;T{
Terminates a client.*
.mc
T};;x;x;x;x;x
_
.mc |
\fBf.lower\fR;Lowers a window to bot-;x;x;x;x;x;x
  [-\f2client\fP];tom of stack.;;;;;;
.mc
_
.mc |
\fBf.maximize\fR;T{
Enlarges a
window to its maximum size.
.mc
T};;x;x;x;x;x
_
.mc |
\fBf.menu\fR;Associates a menu with a;x;x;x;x;x;x;
\f2menu_name\fP;selection or binding.
.mc
_
.mc |
\fBf.minimize\fR;T{
.mc
Changes a window into an icon.
T};;;x;x;x;x
_
.mc |
\fBf.move\fR;T{
.mc
Enables the interactive moving of a window.
T};;x;x;x;x;x
_
.mc |
\fBf.next_cmap\fR;T{
Installs the next colormap in the
window with the colormap focus.
.mc
T};x;x;x;x;x;x
_
.mc |
\fBf.next_key\fR;Sets keyboard focus to;x;x;x;x;x;x
[icon | window;the next window/icon;;;;;;
  | transient];in the stack.;;;;;;
.mc
_
.mc |
\fBf.nop\fR;T{
No operation
performed.
.mc
T};x;x;x;x;x;x
_
.mc |
\fBf.normalize\fR;T{
Displays a window in
normal size.
.mc
T};;x;x;x;x;x
_
.mc |
\fBf.normalize_and_raise\fR;T{
Displays
a window in normal size and raises it to the top of the stack.
T};;x;x;x;x;x
_
\fBf.pack_icons\fR;Tidies up icon rows on;x;x;x;x;x;x
.mc
;the root window or in;;;;;;
.mc |
;the icon box;;;;;;
.mc
_
.mc |
\fBf.pass_keys\fR;Toggles between enabling;x;x;x;x;x;x
.mc
;and disabling the pro-;;;;;;
;cessing of key bindings.;;;;;;
_
.mc |
\fBf.post_wmenu\fR;T{
.mc
Posts the window menu.
T};x;x;x;x;x;
_
.mc |
\fBf.prev_cmap\fR;T{
Installs
the previous colormap in the window with the colormap focus.
.mc
T};x;x;x;x;x;x
_
.mc |
\fBf.prev_key\fR;Sets the keyboard input;x;x;x;x;x;x
.mc
[icon | window;focus to the previous;;;;;;
  | transient];window/icon in the stack.;;;;;;
_
.mc |
\fBf.quit_mwm\fR;T{
.mc
Terminates OSF/Motif Window Manager.
T};x;;;x;x;x
_
.mc |
\fBf.raise\fR [-\fIclient\fR];T{
Lifts a window to top of stack.
.mc
T};x;x;x;x;x;x
_
.mc |
\fBf.raise_lower\fR;T{
Raises a partially concealed window; lowers an
unconcealed window.
.mc
T};;x;x;x;x;x
_
.mc |
\fBf.refresh\fR;T{
Redraws all
windows.
.mc
T};x;x;x;x;x;x
_
.mc |
\fBf.refresh_win\fR;T{
Redraws a client
window.
.mc
T};;;x;x;x;x
_
.mc |
\fBf.resize\fR;T{
Enables interactive resizing
of windows.
.mc
T};;;x;x;x;x
_
.mc |
\fBf.restart\fR;T{
Restarts the OSF/Motif
Window Manager.
.mc
T};x;;;x;x;x
_
.mc |
\fBf.send_msg\fR;Sends a client message.*;;x;x;x;x;x
\fImessage_name\fP;;;;;;
.mc
_
.mc |
\fBf.separator\fR;T{
.mc
Draws a line between menu selections.
T};x;x;x;;;x
_
.mc |
\fBf.set_behavior\fR;Restarts MWM with;x;x;x;x;x;x
.mc
;default or custom;;;;;;
.mc |
;behavior.*;;;;;;
.mc
_
.mc |
\fBf.title\fR;T{
Inserts a title into
a menu at the position specified.
.mc
T};x;x;x;;;x
.TE
.mc |
.in

 
.mc
