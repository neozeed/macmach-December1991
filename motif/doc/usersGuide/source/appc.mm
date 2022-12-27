.\"  (c) Copyright 1989, 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
.\"  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
.\"  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
.\"  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
.\"  (c) Copyright 1988 MICROSOFT CORPORATION
.\"  ALL RIGHTS RESERVED
...\".SK
.nr H1 2
.af H1 A
.H 1 "Clients That Help You Manage Windows"
...\".nr !_ 0
.iX "managing" "windows"
.iX "window management clients"
.iX "clients" "for window management"
.P
In addition to \fBmwm\fP, the following clients are also
useful in managing windows:
.BL
.LI
\fBresize\fR
.LI
\fBxrefresh\fR
.LE
.iX "resize"
.iX "environment" "resetting variables"
.iX "TERM environment variable"
.iX "LINES environment variable"
.iX "COLUMNS environment variable"
.SK

.nr Hu 2
.HU "Resetting Environment Variables With resize"
The \fBresize\fR client resets three environment variables: TERM, LINES, and
COLUMNS.  This enables a shell to reflect the current size of its window.
.P
Don't confuse \fBresize\fR, the client, with the window manager function
\fBf.resize\fR.  The \fBf.resize\fR function changes the size of a window, but
does not reset any environment variables.  The \fBresize\fR client, on the
other hand, does not change the size of a window, but it does reset the
environment variables.  Resetting the environment variables enables
non-client programs to adjust their output to the window's new size.
.iX "resize" "when to use"
.nr Hu 3
.HU "When to Use resize"
Use \fBresize\fR whenever you resize a terminal emulator window and want a
non-client program running in that window to reflect the window's new size.
.nr Hu 3
.HU "Examples"
To see what the current COLUMN and LINES settings are, type the following
command:
.DS
\fBresize\ <Return>\fB\fR
.DE
After you have resized a window either by dragging the window frame or by
choosing Size from the window menu, you can reset the LINES and COLUMN
environment variables to reflect the new window size by issuing the
following command:
.DS
\fBeval\ 'resize'\ <Return>\fB\fR
.DE
.SK

.nr Hu 2
.HU "Repainting the Screen With xrefresh"
.iX "redrawing the screen"
.iX "screen" "repainting"
.iX "repainting the screen"
The \fBxrefresh\fR client repaints the screen or a specified portion of the
screen.  This client performs a similar task to the \fBf.refresh\fR window
manager function.  However, the \fBxrefresh\fR client, because of its
options, is more versatile.
.nr Hu 3
.HU "When to Use xrefresh"
.iX "xrefresh"
Occasionally, an X program may not clean up properly after itself,
leaving graphics litter on the screen.  Use \fBxrefresh\fR any time you
want X to redisplay everything on the screen (or on a portion of the screen.
.P
To use \fBxrefresh\fR, execute it from the command line of any terminal window.
If you want to repaint just a portion of the screen, use the \fB-geometry
\fIwidth\fBx\fIheight\fB\(+-\fIx\fB\(+-&y&\fR option.
.nr Hu 3
.HU "Examples"
The following command repaints the entire display:
.DS
\fBxrefresh\ <Return>\fB\fR
.DE
This command repaints a portion of the display in the upper-left corner:
.DS
\fBxrefresh\ -white\ -geometry\ 400x200+1+1\fR
.DE
