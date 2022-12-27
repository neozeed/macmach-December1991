.\"  (C) Copyright 1989, 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
.\"  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
.\"  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
.\"  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
.\"  (c) Copyright 1988 MICROSOFT CORPORATION
.\"  ALL RIGHTS RESERVED
.H 1 "Managing Menus, Mouse Buttons, and Keyboard Bindings"
This chapter presents information users need if they want to customize or
create window menus, button bindings, and key bindings.  As an applications
programmer, you can also customize and create menus and bindings.  However, be
aware that users can modify your configurations.
.P
This chapter discusses the following topics:
.BL
.LI
The MWM resource description file
.LI
Modifying menus and default menus
.LI
Making new menus: item strings, mnemonics, accelerators, and MWM functions
.LI
Changing the menu associated with the window menu button
.LI
Mouse button bindings
.LI
Keyboard bindings
.LE
.iX "resource description file"
.iX "MWM resource description file"
.iX ".Xdefaults" "and resource description file"
.iX "/usr/lib/X11/app-defaults/Mwm" "and resource description file"
.H 2 "MWM Resource Description File"
The MWM resource description file is a supplementary resource file
that contains resource descriptions that are referred to by entries in
the defaults files (such as \fB\&.Xdefaults\fR and \fB/usr/lib/X11/app-defaults/Mwm\fR).
.P
The following types of resources can be described in the MWM
Resource Description File:
.VL .8i
.LI "Buttons"
Window manager functions can be bound (associated) with button
press events.
.LI "Keys"
Window manager functions can be bound (associated) with key
press events.
.LI "Menus"
Menus can be posted using key and button bindings.
.LE
.P
.iX "configFile resource"
.iX "resource" "configFile"
.iX ".mwmrc" "and configFile resource"
.iX "/usr/lib/X11/system.mwmrc" "and configFile resource"
.iX "$HOME" "and configFile resource"
\fBconfigFile.\fR  A specific MWM resource description
file can be selected using the \fBconfigFile\fR resource.
The value for
this resource is the pathname for an MWM resource description file.
.P
If the pathname begins with ~/ (tilde followed by slash), 
MWM considers it to be relative to the
user's home directory (as specified by the HOME environment variable).
If the LANG environment variable is set, MWM looks for
\fB$HOME/$LANG/\fIconfigFile\fR.
If that file does not exist or if LANG is not set, MWM looks for
\fB$HOME/\fIconfigFile\fR.
.P
If the \fIconfigFile\fR pathname does not begin with ~/ (tilde
followed by slash) MWM considers
it to be relative to the current working directory.
.P
If no value for the resource is specified or if the specified file does
not exist, MWM uses several default paths to find a configuration file.
If the LANG environment variable is set, MWM looks for the
configuration file first in \fB$HOME/$LANG/.mwmrc\fR.
If that file does not exist or if LANG is not set, MWM looks for
\fB$HOME/.mwmrc\fR.
If that file does not exist and if LANG is set, MWM next looks for
\fB/usr/lib/X11/$LANG/system.mwmrc\fR.
If that file does not exist or if LANG is not set, MWM looks for
\fB/usr/lib/X11/system.mwmrc\fR.
.br
.ne 6
.P
An MWM resource description file
is a standard text file containing items
of information separated by blanks, tabs, and newline characters.
Blank lines are ignored.
Items or characters can be quoted to avoid special interpretation
(for example, the comment character can be quoted to prevent it from being
interpreted as the comment character).
A quoted item can be contained in " (double quotation marks).
Single characters can be quoted by preceding them with \\\ (the backslash
character).
All text from an unquoted # (comment) character to the end of the line is regarded
as a comment and is not interpreted as part of a resource description.
If an ! (exclamation poin) is the first character of a line, the line is regarded
as a comment.
.P
.iX "resource" "bitmapDirectory"
.iX "bitmapDirectory resource"
.iX "/usr/include/X11/bitmaps" "and bitmapDirectory"
\fBbitmapDirectory.\fR  You can also, using the
\fBbitmapDirectory\fR resource, direct the window manager to
search a specified directory for bitmaps.  The
\fBbitmapDirectory\fR resource causes the window manager
to search the specified directory whenever a bitmap is named with
no complete path.  The default value for \fBbitmapDirectory\fR
is \fB/usr/include/X11/bitmaps\fR.
.iX "menus" "default"
.iX "menus" "modifying"
.iX "menus" "syntax for making or adding to"
.br
.ne 8
.H 2 "Modifying Menus and Default Menus"
All window manager menus, regardless of the mechanism that
calls them to
the screen, have the same syntax:
.TS
tab(;);
l l l l l .
\fBMenu;\fP\fImenu_name\fR
{
\fIitem1;[mnemonic];[accelerator];function;[argument]\fR
\fIitem2;[mnemonic];[accelerator];function;[argument]\fR
   .;;;
   .;;;
\fIitem#;[mnemonic];[accelerator];function;[argument]\fR
}
.TE
The menu is given a name, and then each \fBitem\fR to be listed in the
menu is given a name or graphic representation (bitmap).  The item is
followed by an optional mnemonic and/or accelerator, and then by a
\fBfunction\fR from Table 14-1.
The function is the action the window manager takes when that menu item is
selected.  Some functions require an argument.
.iX "window menu file configuration"
To illustrate menu syntax, the following default window menu is presented as
it appears in \fB/usr/lib/X11/system.mwmrc\fR and
in the home directory in \fB.mwmrc\fR.  The default window menu
illustrates the
default OSF/Motif behavior.
.TS
tab(~);
l s l s
l l l l .
.iX "default window menu" "configuration file"
.iX "menu" "default window menu, sample configuration file"
\f(CWMenu DefaultWindowMenu\fR
{~~~
\f(CWRestore~_R~Meta<Key>F5~f.normalize
Move~_M~Meta<Key>F7~f.move
Size~_S~Meta<Key>F8~f.resize
Minimize~_n~Meta<Key>F9~f.minimize
Maximize~_x~Meta<Key>F10~f.maximize
Lower~_L~Meta<Key>F3~f.lower
no-label~~~f.separator
Close~_C~Meta<Key>F4~f.kill\fR
}
.TE
.br
.ne 5
.iX "menu" "workspace or root, sample configuration"
.iX "root menu" "sample configuration"
.iX "workspace menu" "sample configuration"
.P
You might want to create a workspace menu that gives some control over
the entire workspace (screen area).  A sample workspace menu is illustrated
below:
.P
.TS
tab(;);
l s l
l lw(1.5i) .
\f(CWMenu RootMenu
{;
\f(CW"Root Menu";f.title
\f(CW"New Window";f.exec "mterm"
\f(CW"Shuffle Up";f.circle_up
\f(CW"Shuffle Down";f.circle_down
\f(CWRefresh;f.refresh
\f(CWno-label;f.separator
\f(CWRestart;f.restart\fR
};
.TE
.P
You can modify either menu to suit the specific needs of your
application.  However, for the sake of consistency in window
operation, it's usually better to modify the workspace menu and keep
the
window
menu the same.  (See _MOTIF_WM_MENU in Section 12.3.1.5 for instructions on
modifying the window menu.)
.iX "menus" "making a new"
.iX "menus" "modifying"
.H 2 "Making New Menus"
You may modify the window and workspace menus, but
you also have another option.  You can make a completely new menu,
calling
it to the screen by a mouse button press, by a key press,
or by
selecting it from an existing menu.
.P
.P
To create a completely new menu, use the general menu syntax as a
model to do
the following:
.P
.AL
.LI
Fill in a menu name
.LI
Make up item names
.LI
Choose a mnemonic and accelerator  (optional)
.LI
Give each item a function to perform from Table 14-1
.LE
.iX "menu" "titles"
.iX "titles" "menus"
.H 3 "Menu Titles"
A menu title is created with the \fBf.title\fR function.
The \fBf.title\fR function automatically places separators above and
below the title.
.H 3 "Menu Items"
.iX "menu" "items"
An item may be either a character string or a graphic representation
(bitmap).
.P
A character string for items must be compatible with the menu
font that is used.  Character strings must be typed precisely, using
one of the following styles:
.BL
.LI
Any character string containing a space must be enclosed in " 
(double quotation marks), for example, "Menu name."
.LI
Single-word strings do not have to be
enclosed, but it is probably a good idea for the sake of consistency,
for example, Menuname.
.LI
An alternate method of dealing with 2-word selection names is to use an
underbar in place of the space, for example, Menu_name.
.LE
.P
.iX "@" "in menu specification"
.iX "at symbol (@)"
.iX "menus" "making a new one, example"
A bitmap for an item can be created using the \fBbitmap\fR client.  Using
the @/ (at sign and slash) in the menu syntax tells the
window
manager that what follows is the full path to a bitmap file:
.P
     @/\fIbitmapfile   function\fR [\fIargument\fR]
.P
Here is an example of a user-made menu.  The menu is named
"Graphics Projects."
The menu
items are all bitmaps symbolizing different graphics
projects.
The
bitmaps are kept in the directory \fB/users/pat/bits\fR.
When the user, Pat, selects a symbol, the graphics program
starts and
opens the appropriate graphics file.
.br
.ne 11
.P
.in0
.TS
tab(;);
l s
lw(2.5i) l .
\f(CWMenu "Graphics Projects";
{;
"Graphics Projects";f.title
@/users/pat/bits/fusel.bits;f.exec "cad /spacestar/fusel.e12
@/users/pat/bits/lwing.bits;f.exec "cad /spacestar/lwing.s05
@/users/pat/bits/rwing.bits;f.exec "cad /spacestar/rwing.s04
@/users/pat/bits/nose.bits;f.exec "cad /spacestar/nose.e17\fR
};
.TE
.in
.P
Another method for specifying the path is to replace \fB/users/pat/\fR with the
~/ (tilde and slash) characters.  The ~/ specifies the user's home directory.
Yet another method is to use the \fBbitmapDirectory\fR resource,
described in Section 14.1, "MWM Resource Description File."  If the
\fBbitmapDirectory\fR is set to \fB/users/pat/bits\fR, then the
bitmap file
could be specified as:
.P
.TS
tab(;);
l l .
\f(CW@fusel.bits;f.exec "cad /spacestar/fusel.e12\fR
.TE
.iX "mnemonics"
.iX "accelerators"
.iX "syntax" "mnemonics"
.iX "syntax" "accelerators" 
.H 3 "Mnemonics and Accelerators"
You can use mnemonics and keyboard accelerators.
Mnemonics are functional only when the menu is
posted.  Accelerators are
functional whether or not the menu is posted.
A mnemonic specification has the following syntax:
.DS I
\fBmnemonic =\fP _\fIcharacter\fI
.DE
.br
.ne 5
.P
The _ (underbar) is placed under the first matching
\fIcharacter\fR in the label.  If there is no matching
\fIcharacter\fR in the label, no mnemonic is registered
with the window manager for that label.
The accelerator specification is a key action
with the same syntax as is used for binding keys to window
manager functions:
.DS I
\fIkey  context  function  [argument]\fR
.DE
.P
When choosing accelerators, be careful not to use
key actions that are already used in key bindings.
(Keyboard bindings are discussed in greater detail
in Section 14.6.)
.P
The following line from the Default Window Menu illustrates mnemonic and
accelerator syntax:
.DS I
\f(CWRestore  _R  Meta<Key>F5  f.normalize\fR
.DE
.H 3 "MWM Functions"
.iX "functions"
Each MWM function operates in one or more of the following contexts:
.P
.TS
tab (;);
l lw(3.75i) .
\fBroot\fP;T{
\fROperates the function when the workspace or "root window" is 
selected.\fP
T}
\fBwindow\fP;T{
\fROperates the function when a client window is selected.  All 
sub-parts of a
window are considered as "windows" for function contexts.  Note that some
functions operate only when the window is in its normalized state
(f.maximize), or its maximized or iconified state (f.normalize).\fP
T}
\fBicon\fP;T{
\fROperates the function when an icon is selected.\fP
T}
.TE
.P
Additionally, each function is operated by one or more of the
following devices:
.P
.BL
.LI
Mouse button
.LI
Keyboard key
.LI
Menu item
.LE
.br
.ne 6
.P
Any selection that uses an invalid context, an invalid function,
or a function that does not apply to the current context is 
grayed out.  This
is the case with the Restore selection of a terminal window's
window menu or the Minimize selection of an icon's menu.  Also,
menu items are grayed out if they do
the \fBf.nop\fR (no operation performed) function.
.br
.ne 12
.P
All window manager functions are available for use in a menu, except
\fBf.post_wmenu\fR.  MWM can be configured to disallow the use of five of
the functions: \fBf.resize\fR, \fBf.move\fR, \fBf.minimize\fR,
\fBf.maximize\fR, and \fBf.kill.\fR  You (the programmer) indicate disallowed functions with the
_MOTIF_WM_HINTS client window property (see Chapter 12).  The user indicates
disallowed functions with the \fBclientFunctions\fR resource, or the
\fBtransientFunctions\fR resource if the window is transient (see 
"Functions and Clients," which appears later).
.P
.iX "functions" "table of MWM valid"
The following table lists the valid functions for MWM.
Items followed by an asterisk are described in more detail following
the table. Under the heading Contexts, R stands for Root, I stands for
Icon, and W stands for Window.  Under the heading Devices, MO stands
for Mouse, K stands for Key, and ME stands for Menu.
.P
.TB "Valid Window Manager Functions"
.in 0
.TS H
box,tab(;);
cBs | cBss | cBss
l | l | c | c | c | c | c | c
l | lw(1.25i) | c | c | c | c | c | c .
Functions; Contexts; Devices
_
\f3Name\fP;\f3Description\fP;\f3R\fP;\f3I\fP;\f3W\fP;\f3MO\fP;\f3K\fP;\f3ME\fP
_
.TH
_
\f3f.beep\fP;T{
.na
Causes a beep to sound.
.ad
T};x;x;x;x;x;x
_
\f3f.circle_down\fP;Puts window on;x;x;x;x;x;x
[icon | window];bottom of stack.*;;;;;;
_
\f3f.circle_up\fP;Puts window on;x;x;x;x;x;x
[icon | window];top of stack.*;;;;;;
_
\f3f.exec\fP (or) !;Uses \f3/bin/sh\fP to;x;x;x;x;x;x
command;execute a;;;;;;
;command.;;;;;;
_
\f3f.focus_color\fP;T{
.na
Sets colormap focus when colormap focus policy
is explicit.*
.ad
T};x;x;x;x;x;x
_
\f3f.focus_key\fP;T{
.na
Sets keyboard
input focus when keyboard focus policy is explicit.*
.ad
T};x;x;x;x;x;x
_
\f3f.kill\fP;T{
.na
Terminates a client.*
.ad
T};;x;x;x;x;x
_
\f3f.lower\fP;Lowers a window;x;x;x;x;x;x
  [\-\f2client\fP];to bottom of;;;;;;
;stack.;;;;;;
_
\f3f.maximize\fP;T{
.na
Enlarges a
window to its maximum size.
.ad
T};;x;x;x;x;x
_
\f3f.menu\fP;Associates a;x;x;x;x;x;x;
\f2menu_name\fP;menu with a;;;;;;
;selection or;;;;;;
;binding.;;;;;;
_
\f3f.minimize\fP;T{
.na
Changes a window into an icon.
.ad
T};;;x;x;x;x
_
\f3f.move\fP;T{
.na
Enables the interactive moving of a window.
.ad
T};;x;x;x;x;x
_
\f3f.next_cmap\fP;T{
.na
Installs the next colormap in the
window with the colormap focus.
.ad
T};x;x;x;x;x;x
_
\f3f.next_key\fP;Sets keyboard;x;x;x;x;x;x
[icon | window;focus to the next;;;;;;
  | transient];window/icon in;;;;;;
;the stack.;;;;;;
_
\f3f.nop\fP;T{
.na
No operation
performed.
.ad
T};x;x;x;x;x;x
_
\f3f.normalize\fP;T{
.na
Displays a window in
normal size.
.ad
T};;x;x;x;x;x
_
\f3f.normalize_and_raise\fP;T{
.na
Displays
a window in normal size and raises it to the top of the stack.
.ad
T};;x;x;x;x;x
_
\f3f.pack_icons\fP;T{
.na
Tidies up icon rows on
the root window or in
the icon box.
.ad
T};x;x;x;x;x;x
_
\f3f.pass_keys\fP;T{
.na
Toggles between enabling
and disabling the processing
of key bindings.
.ad
T};x;x;x;x;x;x
_
\f3f.post_wmenu\fP;T{
.na
Posts the window menu.
.ad
T};x;x;x;x;x;
_
\f3f.prev_cmap\fP;T{
.na
Installs
the previous colormap in the window with the colormap focus.
.ad
T};x;x;x;x;x;x
_
\f3f.prev_key\fP;Sets keyboard;x;x;x;x;x;x
[icon | window;focus to the;;;;;;
  | transient];previous;;;;;;
;window/icon;;;;;;
;in the stack.;;;;;;
_
\f3f.quit_mwm\fP;T{
.na
Terminates OSF/Motif Window Manager.
.ad
T};x;;;x;x;x
_
\f3f.raise\fP [\-\f2client\fP];T{
.na
Lifts a window to top of stack.
.ad
T};x;x;x;x;x;x
_
\f3f.raise_lower\fP;T{
.na
Raises a partially concealed window; lowers an
unconcealed window.
.ad
T};;x;x;x;x;x
_
\f3f.refresh\fP;T{
.na
Redraws all
windows.
.ad
T};x;x;x;x;x;x
_
\f3f.refresh_win\fP;T{
.na
Redraws a client
window.
.ad
T};;;x;x;x;x
_
\f3f.resize\fP;T{
.na
Enables interactive resizing
of windows.
.ad
T};;;x;x;x;x
_
\f3f.restart\fP;T{
.na
Restarts the OSF/Motif
Window Manager.
.ad
T};x;;;x;x;x
_
\f3f.send_msg\fP;Sends a client;;x;x;x;x;x
\f2message_name\fP;message.*;;;;;
_
\f3f.separator\fP;T{
.na
Draws a line between menu selections.
.ad
T};x;x;x;;;x
_
\f3f.set_behavior\fP;T{
.na
Restarts MWM with
default or custom
behavior.*
.ad
T};x;x;x;x;x;x
_
\f3f.title\fP;T{
.na
Inserts a title into
a menu at the position specified.
.ad
T};x;x;x;;;x
.TE
.in
.br
.ne 10
.P
.iX "f.circle_down function"
\fBf.circle-down [icon | window]\fR 
This function causes the window or icon that is on the top of the window
stack to be put on the bottom of the window stack, so that it is no
longer obscuring any other window or icon.
This function affects only those windows and icons that are obscuring
other windows and icons or that are themselves obscured.
Secondary windows (transients) are restacked with their associated
primary window.
Secondary windows always stay on top of the associated primary window
and there can be no other primary windows between the secondary windows
and their primary window.
.br
.ne 5
.P
If an icon function argument is specified, the function applies only to
icons.
If a window function argument is specified, the function applies only to
windows.
.P
.iX "f.circle_up function"
\fBf.circle_up [icon | window]\fR
This function raises the window or icon on the bottom of the window
stack so that it is not obscured by any other windows.
It affects only those windows and icons that are obscuring other windows
and icons or that are themselves obscured.
Secondary windows (transients) are restacked with their associated
primary window.
.P
If an icon function argument is specified, the function applies only to
icons.
If a window function argument is specified, the function applies only to
windows.
.P
.iX "f.focus_color function"
\fBf.focus_color\fR  This function sets the colormap focus to a client
window or icon (for icons,\fR the default colormap for the screen is
installed).  If this function is done in a root context, the default
colormap (set by the X Window System for the screen where MWM is
running) is installed and there is no specific client window
colormap focus.  This function is ignored if
\fBcolormapFocusPolicy\fR is not "explicit."
.P
.iX "f.focus_key function"
\fBf.focus_key\fR  This function sets the keyboard input focus to a 
client
window or icon and is ignored if
\fBkeyboardFocusPolicy\fR is not "explicit."  See also the description of
the \fBfocusAutoRaise\fR resource in Section 13.4, "Setting Focus Policies
for Active Windows."  A button binding for this function
can be set up with a window context without the side-effect of having the
button made unavailable to the client window.
.br
.ne 5
.P
.iX "WM_SAVE_YOURSELF" "and f.kill function"
.iX "WM_DELETE_WINDOW" "and f.kill function"
.iX "f.kill function"
\fBf.kill\fR  This function sends a client message event to a client
indicating that the client window should be
deleted.  This occurs only if the WM_DELETE_WINDOW protocol is set up.
If the WM_SAVE_YOURSELF protocol is set up and
the WM_DELETE_WINDOW protocol is not set up, the client is sent a client
message event, indicating that the client needs to prepare to be terminated.
If the client has neither the WM_DELETE_WINDOW nor WM_SAVE_YOURSELF
protocol set up, \fBf.kill\fR causes a client's X connection to be
terminated (usually resulting in termination of the client and possibly
resulting in a loss of data).  Refer to the
description of the \fBquitTimeout\fR resource in Section 13.5, "Managing
Clients," and the WM_PROTOCOLS property in Section 12.2, "Inter-Client
Communications Conventions."
.br
.ne 8
.P
.iX "f.send_msg function"
.iX "WM_PROTOCOLS" "and f.send_msg function"
\fBf.send_msg \fImessage_name\fR\fR  This function sends a client message of
the type WM_PROTOCOLS with the protocols type indicated by the
\fImessage_name\fR function argument.  The client message is 
sent only if \fImessage_name\fR is included in the client's WM_PROTOCOLS
property.  A menu item label is grayed-out if the menu item wants to send
a message that is not included in the client's WM_PROTOCOLS property.
.P
.iX "f.set_behavior function"
.iX "OSF/Motif" "default behavior"
\fBf.set_behavior\fR  This function causes MWM
to restart with the default OSF/Motif behavior (if a custom behavior is
configured) or a custom behavior (if an OSF/Motif default behavior is
configured).
.P
A key binding for doing the \fBf.set_behavior\fR function is
included in the built-in default set of key bindings for MWM.
If the user specifies a custom set of key bindings to replace the
default set, then the \fBf.set_behavior\fR key binding is added
automatically to the custom set (and is always processed first).
The following table shows the key binding for \fBf.set_behavior\fR.
.P
.TB "Key Binding for f.set_behavior"
.TS
tab(~),center,allbox;
lb lb
l lw(2.5i).
Key Action~Function Description
_
\f3<Meta><Shift><Ctrl><!>\fP~T{
Switch (toggle) between the default and custom behaviors
T}
.TE
.br
.ne 6
.P
When the user switches to the default OSF/Motif behavior, a number of
MWM resources assume their default values and MWM restarts.
When the user switches back to the custom behavior, the resource values
that were changed to default values are reset with the custom values
and MWM restarts.
.br
.ne 4
.P
When an \fBf.set_behavior\fR function is done, the following user interaction
occurs:
.P
.AL
.LI
A system modal dialog box appears asking the user for confirmation
of the \fBf.set_behavior\fR action.
The user may cancel the action at this point.
.LI
The window manager restarts.
.br
.ne 6
.LI
The window manager applies the new (custom or default) configuration values.
.LI
Window manager components are mapped.
.LE
.br
.ne 7
.P
When the default OSF/Motif behavior is being set, default resource values
are applied, and client properties that control window manager
behavior are applied if specified.
This includes the _MOTIF_WM_HINTS and _MOTIF_WM_MENU properties.
These properties may alter default OSF/Motif behavior, but in a way that is
consistent for all users.
.H 4 "Functions and Clients"
.iX "resource" "clientFunctions"
.iX "clientFunctions resource"
.br
\fBclientFunctions.\fR  This resource is used to indicate which window
management functions are allowed or not allowed.
The resource value is a list of items, with each
item preceded by a + (plus) or - (minus) sign.
The sign of the first item in the list
determines the initial functionality.
If the sign of the first
item is minus, then MWM starts with all functions and
subtracts from that set.
If the sign of the first item is plus (or not
specified), then MWM starts with no functions and builds up a list from
the resource.  The following table lists the name and description for each client
function.
.br
.ne 10
.P
.TB "Client Functions"
.TS
center, tab(~), allbox;
lb | lb
l | l .
Name~Description
_
all~Include all functions (Default value)
none~Include no functions
resize~\f3f.resize\fP
move~\f3f.move\fP
minimize~\f3f.minimize\fP
maximize~\f3f.maximize\fP
close~\f3f.kill\fP
.TE
.P
Note that the five functions listed in the previous table 
are the only functions that can be individually specified.
.iX "transientFunctions"
.br
.ne 7
.P
.iX "resource" "transientFunctions" 
.iX "transientFunctions resource"
\fBtransientFunctions.\fR  This resource is used to indicate which window
management functions are allowed or not allowed with respect to transient
windows.  The function specification is exactly the same as for
the \fBclientFunctions\fR resource.  The default value for this resource is
"-minimize -maximize."
.H 3 "Size of Menu Window"
.iX "size" "of menu window"
.iX "menu window" "size"
The size of the menu window is affected not only by the number of items in
the menu, but also by the font you use.  The size and resolution of the
display determine the default value MWM uses.
.br
.ne 8
.iX "binding" "a menu to a mouse button or key"
.iX "menu" "binding to a mouse button or key"
.H 3 "Binding a Menu to a Mouse Button or Key"
If you want your new menu to appear whenever a certain
mouse button or keyboard key is pressed, follow these
steps:
.P
.AL
.LI
Choose the mouse button or keyboard key that you want to use.
.LI
Choose the action on the button or key that causes the menu
to appear.
.LI
Use the \fBf.menu\fR function with the new menu's
name as an argument to bind the menu to the button or key.
.LE
.P
For more information on mouse button and keyboard bindings,
including
examples, see
the Section 14.5, "Mouse Button Bindings," and Section 14.6, "Keyboard Bindings."
.iX "window menu" "changing"
.iX "menu" "window menu, changing"
.iX "windowMenu resource" "syntax"
.iX "windowMenu resource"
.iX "resource" "windowMenu"
.H 2 "Changing the Menu Associated with the Window Menu Button"
\fBwindowMenu.\fR  The \fBwindowMenu\fR resource enables you (and the
user) to change the menu that
appears when the user presses the mouse Select button on the window
frame's window menu
button.  This allows you to display a menu of your
choice from
the window menu button without having to
extensively remodel the
.ne 5
window menu to do it.  All you need do is make a new menu, then
use
the \fBwindowMenu\fR resource to associate this new menu
with the window menu button.
.P
The \fBwindowMenu\fR resource has three syntaxes.  Which
one you use
depends on the situation.
.AL
.LI
To specify the menu for all classes of clients, the syntax is
.TS
tab(;);
l l l .
; \fBMwm*windowMenu\fR:;\fIMenuName\fR
.TE
.sp
For example, if you want to associate a special \fBCADCAMMenu\fR menu
with the window menu button, you add the following line to
your \fB.Xdefaults\fR file:
.TS
tab(;);
l l l .
; \f(CWMwm*windowMenu:;CADCAMMenu\fR
.TE
.br
.ne 15
.LI
To specify the menu for a specific class of
clients, the syntax is
.TS
tab(;);
l l l .
; \fBMwm*\fR\fIclientclass\fR\fB.windowMenu\fR:;\fIMenuName\fR
.TE
.br
.ne 9
.sp
For example, you may want to associate a particular
\fBEditorMenu\fR of your own
creation with a terminal emulator window called \fBmterm\fR:
.P
.TS
tab(;);
l l l .
; \f(CWMwm*Mterm.windowMenu:;EditorMenu\fR
.TE
.LI
To specify a menu for any client whose
class is unknown, the syntax is
.P
.TS
tab(;);
l l l .
; \fBMwm*defaults*windowMenu\fR:;\fIMenuName\fR
.TE
.LE
.P
The default is DefaultWindowMenu.
.P
There are two resources involving the window menu button:
\fBwMenuButtonClick\fR and \fBwMenuButtonClick2\fR.
.iX "resource" "wMenuButtonClick"
.iX "wMenuButtonClick resource"
.P
The \fBwMenuButtonClick\fR resource
indicates whether a click of the mouse when the pointer is over
the window menu button posts and leaves posted the window
menu.  The default value of True posts and leaves posted the menu.
A value of False disables this action.
.br
.ne 6
.P
.iX "resource" "wMenuButtonClick2"
.iX "wMenuButtonClick2 resource"
The \fBwMenuButtonClick2\fR resource indicates
whether the window menu
button double-click action is to do an \fBf.kill\fR function.  The
default value of True causes an \fBf.kill\fR action.  A value of
False disables this action.
.br
.ne 20
.iX "mouse button bindings"
.H 2 "Mouse Button Bindings"
The mouse offers a quick way to make things happen in the user's
window
environment without the time-consuming
process of typing
commands on the keyboard (and retyping misspelled commands).
The window
manager recognizes the following button actions:
.VL 1.1i
.LI \fBPress\fR
Holding down a mouse button
.LI \fBRelease\fR
Releasing a pressed mouse button
.LI \fBClick\fR
Pressing and releasing a mouse button
.LI \fBDouble-click\fR
Pressing and releasing a mouse button twice in rapid succession
.LI \fBDrag\fR
Pressing a mouse button and moving the pointer/mouse device
.LE
.br
.ne 7
.P
.iX "mouse button bindings" "definition"
.iX "button bindings" "definition"
You can associate a mouse button action with a window management
function by using
a \fBbutton binding\fR.  A button binding is a command line you
put in the \fBsystem.mwmrc\fR or \fB.mwmrc\fR file that
associates
a button action
with a window manager function.
.P
User defined button bindings are added to built-in button bindings and
are always done first.
.iX "default" "mouse button bindings"
.iX "mouse button bindings" "default"
.H 3 "Default Button Bindings"
MWM has built-in default button bindings.
These button bindings define the functions of the window
frame components.
The user-specified button bindings that are
defined with the \fBbuttonBindings\fP resource
are added to the built-in button bindings.  The default value for this
resource is DefaultButtonBindings.
The built-in button bindings are described by the button
bindings specifications shown in the following table.
.P
.ne 3i
.TB "Default Button Bindings"
.TS
tab(~),center,allbox;
lb lb lb
l l l .
Button Action~Context*~Function
_
\f3<Btn1Down>\fP~menu~\f3f.post_wmenu\fP
\f3<Btn1Click2>\fP~menu~\f3f.kill\fP
\f3<Btn1Click>\fP~minimize~\f3f.minimize\fP
\f3<Btn1Click>\fP~maximize~\f3f.maximize\fP
\f3<Btn1Down>\fP~title~\f3f.move\fP
\f3<Btn1Down>\fP~window|icon~\f3f.focus_key\fP
\f3<Btn1Down>\fP~border~\f3f.resize\fP
\f3<Btn1Click>\fP~icon~\f3f.post_wmenu\fP
\f3<Btn1Click2>\fP~icon~\f3f.normalize\fP
\f3<Btn1Down>\fP~icon~\f3f.move\fP
.TE
*Context is explained in the next section.
.P
.br
.ne 5
.P
The following special handling applies to the built-in button bindings.
.P
.BL
.LI
The menu, minimize, maximize, and border contexts are internal-only
MWM contexts.
.LI
The window menu is posted just below the window menu button and
not at the pointer position.
When the window menu is posted with a Button 1 press
and the Button 1 release is done
with the pointer on the window menu button, then the window
menu remains posted and can be traversed from the keyboard.
.LI
.iX "f.resize function" "and mouse button bindings"
The \fBf.resize\fR function for the resize frame handles has a
behavior that is specific to the resize handle contexts.
.LE
.br
.ne 15
.P
The default button binding shown in the following table is built into MWM, but can be
replaced by user specified button bindings:
.P
.TB "Replaceable Default Button Binding"
.TS
tab(~),center,allbox;
lb lb lb
l l l .
Button Action~Context~Function
_
\f3<Btn1Down>\fP~icon|frame~\f3f.raise\fP
.TE
.iX "mouse button bindings" "modifying"
.iX "mouse button bindings" "syntax"
.H 3 "Button Binding Syntax and Modifying Button Bindings"
The syntax for button bindings is as follows:
.P
.TS
l l .
\fBButtons\fR     \fIButtonBindingSetName\fP
{
\fIbutton   context\fR [| \fIcontext]   function   [argument]\fP
\fIbutton   context\fR [| \fIcontext]   function   [argument]\fP
    .
    .
\fIbutton   context\fR [| \fIcontext]   function   [argument]\fP
}
.TE
.P
Each line identifies a certain mouse button action, followed by
the
context
in which the button action is valid, followed by the function
to be done.  Some functions require an argument.
.P
To modify the default button bindings, you need to
edit either \fBsystem.mwmrc\fR (to make system-wide changes) 
or \fB.mwmrc\fR (to make 
changes to the local environment).  The easiest way to modify
button
bindings is to change the default bindings or to insert extra
lines
in the
DefaultButtonBindings section.
.P
When modifying or creating a button binding, you need to first
decide which mouse button to use and which action is
performed on the button.  Make sure you do not use button-action
combinations already used by OSF/Motif.  You might want to require a
simultaneous key press with the mouse button action.  This is
called modifying the button action.  \fBModifiers\fR increase the
number of possible button bindings you can make.  The following table
shows the available modifier keys and their abbreviations (for
use in the button binding command line).
.P
.ne 3i
.iX "mouse button bindings" "modifier keys"
.TB "Button Binding Modifier Keys"
.TS
center, box, tab(;);
lb  | lb
l  | l  .
Modifier;Description
_
\f3Ctrl\fP;Control Key
\f3Shift\fP;Shift Key
\f3Meta\fP;Meta/Alt Key
\f3Lock\fP;Lock Key
\f3Mod1\fP;Modifier1
\f3Mod2\fP;Modifier2
\f3Mod3\fP;Modifier3
\f3Mod4\fP;Modifier4
\f3Mod5\fP;Modifier5
.TE
.iX "mouse buttons" "Button 4 and Button 5"
.iX "mouse button bindings" "button actions"
.P
You can bind up to five buttons if you have a 3-button mouse.
Button 4 is the simultaneous press of Buttons 1 and 2.  Button
5 is the simultaneous press of Buttons 2 and 3.  Each button can
be bound with one of four actions.  The following table describes the button actions and their
abbreviations.
.br
.ne 3.5i
.P
.TB "Button Actions for Button Bindings"
.TS
center, box, tab(;);
lb | lb
l  | l  .
Button;Description
_
\f3Btn1Down\fP;Button 1 Press
\f3Btn1Up\fP;Button 1 Release
\f3Btn1Click\fP;Button 1 Press and Release
\f3Btn1Click2\fP;Button 1 Double-Click
_
\f3Btn2Down\fP;Button 2 Press
\f3Btn2Up\fP;Button 2 Release
\f3Btn2Click\fP;Button 2 Press and Release
\f3Btn2Click2\fP;Button 2 Double-Click
_
\f3Btn3Down\fP;Button 3 Press
\f3Btn3Up\fP;Button 3 Release
\f3Btn3Click\fP;Button 3 Press and Release
\f3Btn3Click2\fP;Button 3 Double-Click
.TE
.P
After choosing the optional modifier and the mouse button action, you must
decide under which context(s) the binding works.  The following table lists the button-binding 
contexts that are recognized by the window manager.
.br
.ne 18
.P
.iX "contexts" "for mouse button bindings"
.iX "mouse button bindings" "contexts"
.ne 2.25i
.TB "Contexts for Mouse Button Bindings"
.TS
center, box, tab(;);
lb | lb
^ | lb
l | l .
This context...;For mouse action at this
;pointer position...
_
root;Workspace (root window)
_
window;Client window
_
frame;Window frame
_
icon;Icon
_
title;Title bar
_
border;Frame minus title bar
_
app;Application window (inside the frame)
.TE
.P
The context indicates where the pointer must be for the
button binding to be effective.  For example, a context of \fIwindow\fR
indicates that the pointer must be over a client window or window
frame for the button binding to be effective.  The \fIframe\fR
context is for the window frame around a client window
(including the border and title bar), the \fIborder\fR context is for the
border part of the window frame (not including the title bar),
the \fItitle\fR context is for the title bar of the window
frame, and the \fIapp\fR context is for the application window or client
area (not
including the window frame).
.br
.ne 25
.P
.iX "mouse button binding" "example"
Here is an example of a button binding.  Pat, the user who
created his own Graphics Projects menu, can
display the menu with a button action.  Pat chooses \fBMeta<Key>\fR 
as a modifier, and mouse custom-button press, \fBBtn3Down\fR, as the button
action.  He decides the pointer must be on the workspace.  The
function name for posting a special menu is
\fBf.menu\fR, and the argument is the menu name Graphics Projects.
\ He needs only to insert one line into
his \fB.mwmrc\fR file to make this happen.
The DefaultButtonBindings
section of his \fB.mwmrc\fR file then looks like the
following:
.DS
\f(CWButtons DefaultButtonBindings
{
<Btn3Down>     root  f.menu "DefaultRootMenu"
<Btn3Down>     frame f.post_wmenu
Meta<Btn3Down> root f.menu "Graphics Projects"\fR
}
.DE
.P
.iX "mouse button bindings" "making a new set"
.H 3 "Making a New Button Binding Set"
Perhaps inserting a new button binding into the
DefaultButtonBindings set
is not enough.  You might need to make a complete new set of
button
bindings.  To do this, use the DefaultButtonBindings section of
the \fB.mwmrc\fR file as a model.  After you have created the new
button binding set,
use the \fBbuttonBindings\fR resource to tell the
window manager
about it.
.P
.iX "buttonBindings resource"
.iX "resource" "buttonBindings"
\fBbuttonBindings.\fR  The \fBbuttonBindings\fR resource is used to
specify a button binding set.
The default is DefaultButtonBindings.
The syntax of the added \fB.Xdefaults\fR
line is as follows:
.P
.TS
tab(;);
l l l .
\fBMwm*buttonBindings:\fR;NewButtonBindingSetName
.TE
.P
.iX "mouse button bindings" "making a new set, example"
This line directs the window manager to use
NewButtonBindingSetName as the
source of its button binding information.  The button bindings
are
assumed
to exist in the file named by the \fBMwm*configFile:\fR
resource, the
default being \fB.mwmrc\fR.
.br
.ne 10
.P
For example, suppose Pat, our graphics user, wants to specify a
completely new button binding set instead of inserting a line in
the
existing DefaultButtonBindings set.  He needs to create a new
button
binding set, such as the following, modeled after the default
set:
.P
.TS
tab(;);
l l .
\f(CWButtons;GraphicsButtonBindings
{;
<Btn3Down>;root  f.menu  "Graphics Projects"\fR
};
.TE
.P
In his \fB.Xdefaults\fR file, Pat then inserts the
following line:
.P
.TS
tab(;);
l l l .
\f(CWMwm*buttonBindings:;GraphicsButtonBindings\fR
.TE
.P
To display his graphics menu, Pat needs only to press
button 3 on the mouse
when the pointer is on the workspace.
.P
.iX "doubleClickTime resource"
.iX "resource" "doubleClickTime"
.iX "mouse button" "double-click timing"
.H 3 "Modifying Button Click Timing"
\fBdoubleClickTime.\fR  This is another MWM resource for controlling
mouse button behavior.
The \fBdoubleClickTime\fR resource sets the maximum time
(in milliseconds)
that can elapse between button clicks before a double-click
becomes just
two clicks in a row.  The default value is the display's multiclick
time.
.ne 6c
.iX "keyboard bindings" "definition"
.H 2 "Keyboard Bindings"
In a manner similar to mouse button bindings, you (or the user) can bind
(associate)
window manager
functions to keys on the keyboard using keyboard
bindings.
.iX "bindings" "keyboard, see keyboard bindings"
.iX "keyboard bindings" "default"
.iX "default" "keyboard bindings"
.H 3 "Default Keyboard Bindings"
OSF/Motif has default key bindings.
These key bindings are replaced with user-specified key
bindings specified with the \fBkeyBindings\fP resource.
The following table lists the default key binding specifications.
.P
.ne 2.5i
.TB "Default Keyboard Bindings"
.in 0
.TS
tab(~),center,allbox;
lb lb lb
l l l .
Keys~Context~Function
_
\f3Shift<Key>Escape\fP~window|icon~\f3f.post_wmenu\fP
\f3Meta<Key>Space\fP~window|icon~\f3f.post_wmenu\fP
\f3Meta<Key>Tab\fP~root|icon|window~\f3f.next_key\fP
\f3Meta Shift<Key>Tab\fP~root|icon|window~\f3f.prev_key\fP
\f3Meta<Key>Escape\fP~root|icon|window~\f3f.circle_down\fP
\f3Meta Shift<Key>Escape\fP~root|icon|window~\f3f.circle_up\fP
\f3Meta<Key>F6\fP~window~\f3f.next_key transient\fP
\f3Meta Ctrl Shift<Key>Exclam\fP~root|icon|window~\f3f.set_behavior\fP
.TE
.in
.H 3 "Keyboard Binding Syntax and Modifying Keyboard Bindings"
.iX "keyboard bindings" "syntax"
.iX "modifying" "keyboard bindings"
The syntax for keyboard bindings is as follows:
.P
.TS
l l .
\fBKeys\fR \fIKeyBindingSetName\fP
{
\fIkey   context\fR [| \fIcontext]   function   [argument]\fP
\fIkey   context\fR [| \fIcontext]   function   [argument]\fP
       .
       .
\fIkey   context\fR [| \fIcontext]   function   [argument]\fP
}
.TE
.br
.ne 8
.P
Each line identifies a unique key press sequence, followed by the
context
in which that sequence is valid, followed by the  function to be
done.  Some functions require an argument.  Context refers to the
location of the keyboard input focus when the keys are pressed: window,
icon, or root (workspace).
.P
To modify the default keyboard bindings, you need to edit either
\fBsystem.mwmrc\fR (to make system-wide changes) or \fB.mwmrc\fR (to make
changes to the local environment).  The easiest way is to change
the
default bindings or to insert extra lines in the
DefaultKeyBindings section.
.P
Next, decide which key you want to bind.  Then choose the context
in which the key binding is to work.  The following table shows the contexts
for key bindings.
.br
.ne 15
.P
.iX "contexts" "for key bindings"
.iX "key bindings" "contexts"
.TB "Contexts for Key Bindings"
.TS
center, box, tab(;);
lb | lb
lw(2i) | l .
Use this context...;When the keyboard focus is here...
_
root;Workspace (root window)
_
T{
window, frame, border, title, or application
T};T{
Client window (includes frame, title, border, and application window)
T}
_
icon;Icon
.TE
.P
.iX "f.post_wmenu function" "and key bindings"
Note that if \fBf.post_wmenu\fR or \fBf.menu\fR is bound to a key, MWM
automatically uses the same key for removing the menu from the screen
after it has been popped up.
.br
.ne 18
.P
.iX "keyboard bindings" "modifying, example"
To illustrate key bindings, suppose Pat, the user who created his own
"Graphics Projects"
menu, kept pressing the \fB<Shift> <Esc>\fR sequence
and accidentally
displaying the window menu.  He might decide that he is better
off without that particular keyboard binding.  To disable it, he must
delete (or comment out) the proper line in his \fB.mwmrc\fR file.  The
"DefaultKeyBindings" section of his \fB.mwmrc\fR file might
then look like
the following:
.DS I
\f(CWKeys    DefaultKeyBindings
{
#Shift<Key>Escape icon|window  f.post_wmenu
Meta<Key>Tab      window       f.next_key
}\fR
.DE
.P
Pat has chosen simply to comment out the line by placing a # (comment)
character in the left margin of the line.
.iX "keyBindings resource"
.iX "resource" "keyBindings"
.iX "keyBindings resource" "syntax"
.iX "keyboard bindings" "making a new set"
.H 3 "Making a New Keyboard Binding Set"
With keyboard bindings, as with button bindings, you have the option of
creating a whole new binding set.  To do so, you can use the
DefaultKeyBindings section of your \fB.mwmrc\fR as a
model.  After you
have created the new keyboard binding set, use the \fBkeyBindings\fR
resource to explain your modification to the window manager.
.P
\fBkeyBindings.\fR  This resource is used to specify a key binding set.
The default is DefaultKeyBindings.
The syntax of the \fB.Xdefaults\fR line is as follows:
.P
.TS
tab(;);
l l l .
\fBMwm*keyBindings:\fR;\fINewKeyboardBindingSetName\fR
.TE
.P
This line directs the window manager to get its keyboard binding
information from the \fINewKeyboardBindingSetName\fR section of
the \fB.mwmrc\fR file.
You can have the window manager look in any file if you specify
the path and filename with the \fBMwm*configFile:\fR resource in
the \fB.Xdefaults\fR file.

