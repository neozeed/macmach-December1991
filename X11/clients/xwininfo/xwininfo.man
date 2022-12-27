.TH XWININFO 1 "Release 4" "X Version 11"
.SH NAME
xwininfo - window information utility for X
.SH SYNOPSIS
.B "xwininfo"
[-help] [-id \fIid\fP] [-root] [-name \fIname\fP] [-int]
[-tree] [-stats] [-bits] [-events] [-size] [-wm ] [-frame] [-all]
[-english] [-metric]
[-display \fIdisplay\fP]
.SH DESCRIPTION
.PP
.I Xwininfo
is a utility for displaying information about windows.
Various information is displayed depending on which options are selected.
If no options are chosen, \fB-stats\fP is assumed.
.PP
The user has the option of selecting the target window with
the mouse (by clicking any mouse button in the desired window) or by
specifying its window id on the command line with the \fB-id\fP option.
Or instead of specifying
the window by its id number, the \fB-name\fP option may be used to specify
which window is desired by name.
There is also a special \fB-root\fP option to quickly obtain information
on X's root window.
.SH OPTIONS
.PP
.TP 8
.B "-help"
Print out the `Usage:' command syntax summary.
.PP
.TP 8
.B "-id \fIid\fP"
This option allows the user to specify a target window \fIid\fP on the
command line rather than using the mouse to select the target window.
This is very useful in debugging X applications where the target
window is not mapped to the screen or where the use of the mouse might
be impossible or interfere with the application.
.PP
.TP 8
.B "-name \fIname\fP"
This option allows the user to specify that the window named \fIname\fP
is the target window on the command line rather than using the mouse to
select the target window.
.PP
.TP 8
.B "-root"
This option specifies that X's root window is the target window.
This is useful in situations where the root window is completely obscured.
.PP
.TP 8
.B "-int"
This option specifies that all X window ids should be displayed as
integer values.  The default is to display them as hexadecimal values.
.PP
.TP 8
.B -tree
This option causes the root, parent, and children windows' ids and names of
the selected window to be displayed.
.PP
.TP 8
.B -stats
This option causes the display of various attributes pertaining to 
the location and appearance of the selected window.
Information displayed includes the location of the window,
its width and height, its depth, border width, class, colormap id if any,
map state, backing-store hint, and location of the corners.
.PP
.TP 8
.B -bits
This option causes the display of various attributes pertaining to
the selected window's raw bits and how the selected window is to be stored.
Displayed information includes the selected window's bit gravity,
window gravity, backing-store hint, backing-planes value, backing pixel,
and whether or not the window has save-under set.
.PP
.TP 8
.B -events
This option causes the selected window's event masks to be displayed.
Both the event mask of events wanted by some client and the event mask of
events not to propagate are displayed.
.PP
.TP 8
.B -size
This option causes the selected window's sizing hints to be displayed.
Displayed information includes: for both the normal size hints and the
zoom size hints, the user supplied location if any; the program supplied
location if any; the user supplied size if any; the program supplied size if
any; the minimum size if any; the maximum size if any; the resize increments
if any; and the minimum and maximum aspect ratios if any.
.PP
.TP 8
.B -wm
This option causes the selected window's window manager hints to be
displayed.  Information displayed may include whether or not the application
accepts input, what the window's icon window # and name is, where the window's
icon should go, and what the window's initial state should be.
.TP 8
.B -frame
This option causes window manager frames not be ignored when manually 
selecting windows.
.PP
.TP 8
.B -metric
This option causes all individual height, width, and x and y positions to be
displayed in millimeters as well as number of pixels, based on what the
server thinks the resolution is. Geometry specifications that are in
\fB+x+y\fP form are not changed.
.TP 8
.B -english
This option causes all individual height, width, and x and y positions to be
displayed in inches (and feet, yards, and miles if necessary) as well as
number of pixels. \fB-metric\fP and \fB-english\fP may both be enabled at the
same time. 
.PP
.TP 8
.B -all
This option is a quick way to ask for all information possible.
.PP
.TP 8
.B -display \fIdisplay\fP
This option allows you to specify the server to connect to; see \fIX(1)\fP.
.SH EXAMPLE
.PP
The following is a sample summary taken with no options specified:

xwininfo ==> Please select the window about which you
         ==> would like information by clicking the
         ==> mouse in that window.

xwininfo ==> Window id: 0x60000f (xterm)

         ==> Upper left X: 4
         ==> Upper left Y: 19
         ==> Width: 726
         ==> Height: 966
         ==> Depth: 4
         ==> Border width: 3
         ==> Window class: InputOutput
         ==> Colormap: 0x80065
         ==> Window Bit Gravity State: NorthWestGravity
         ==> Window Window Gravity State: NorthWestGravity
         ==> Window Backing Store State: NotUseful
         ==> Window Save Under State: no
         ==> Window Map State: IsViewable
         ==> Window Override Redirect State: no
         ==> Corners:  +4+19  -640+19  -640-33  +4-33

.SH ENVIRONMENT
.PP
.TP 8
.B DISPLAY
To get the default host and display number.
.SH SEE ALSO
X(1), xprop(1)
.SH BUGS
Using \fB-stats -bits\fP shows some redundant information.
.SH COPYRIGHT
Copyright 1988, Massachusetts Institute of Technology.
.br
See \fIX(1)\fP for a full statement of rights and permissions.
.SH AUTHOR
Mark Lillibridge, MIT Project Athena
