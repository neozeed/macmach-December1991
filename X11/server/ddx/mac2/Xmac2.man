.\" $XConsortium: Xmac2.man,v 1.4 88/09/06 14:40:29 jim Exp $
.TH XMAC2 1 "8 August 1988" "X Version 11 Release 4"
.SH NAME
Xmac2 \- MacMach Macintosh II server for X Version 11 Release 4
.SH SYNOPSIS
.B Xmac2
[ option ] ...
.SH DESCRIPTION
.I Xmac2
is the server for Version 11 Release 4 of the X window system on
Macintosh II hardware running MacMach.
It will normally be started by the shell script
.IR startx,
or perhaps directly by
.IR xinit (1).
.fi
.SH CONFIGURATIONS
.I Xmac2
operates under MacMach, Mach 2.5 and Mach 3.0 versions.
It supports a single monochrome display.
.SH OPTIONS
Options are described under
.I Xserver(1).
Note that the 
.I c
and
.I -c
keyclick control options are inoperative on the Macintosh II.
.SH "BUTTON MAPPINGS"
Many X clients assume the mouse has three buttons. The A/UX X server simulates
the middle and right mouse buttons with keystrokes -- the left-arrow key
generates middle button events, and the right-arrow key generates right button
events -- the real mouse button generates left button events. The 
open-apple or cloverleaf key is the "Meta" modifier, Meta can also be obtained
by pressing the up-arrow key. The down-arrow key duplicates the Control key.
Meta, Control, and Shift are often used in combination with other keystrokes
or mouse clicks. For example, the terminal emulator xterm pops up menus in
response to control-left and control-middle. The original function of the
arrow keys may be obtained by holding down the Option key while pressing
one of the arrow keys.
.SH "FILES"
/usr/bin/X11/Xmac2 (the server binary) 
.sp
/usr/bin/X11/* (client binaries)
.sp
/usr/lib/X11/fonts/*
.sp
/usr/lib/X11/rgb.{dir,pag,txt} (color names to RGB mapping)
.sp
$HOME/.twmrc (customization for the
.I twm
window manager. Reference copy in /usr/lib/X11/Sample.twmrc)
.sp
$HOME/.uwmrc (customization for the
.I uwm
window manager. Reference copy in /usr/lib/X11/default.uwmrc)
.sp
/usr/lib/X11/XErrorDB (client error message database)
.sp
/usr/lib/X11/app-defaults (client specific resource specifications)
.sp
/usr/lib/X11/examples/Xaw/* (source code examples of applications built on the
.I X
Toolkit)
.sp
/usr/lib/terminfo/x/xterm* (
.I terminfo
database entries descibing the terminal emulation capabilities of the
.I xterm
client. Reference copies in /usr/lib/X11/xterm*.tic)
.sp
/usr/include/X11/bitmaps/*icon (bitmap representations of icons used by the
.I twm
window manager)
.SH "SEE ALSO"
Xserver(1) xinit(1), X(1),
