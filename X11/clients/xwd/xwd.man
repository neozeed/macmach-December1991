.TH XWD 1 "Release 4" "X Version 11"
.SH NAME
xwd - dump an image of an X window
.SH SYNOPSIS
.B "xwd"
[-debug] [-help] [-nobdrs] [-out \fIfile\fP] [-xy] [-frame]
[-display \fIdisplay\fP]
.SH DESCRIPTION
.PP
.I Xwd
is an X Window System window dumping utility.
.I Xwd
allows X users to store window images in a specially formatted dump
file.  This file can then be read by various other X utilities for
redisplay, printing, editing, formatting, archiving, image processing, etc.
The target window is selected by clicking the mouse in the desired window.
The keyboard bell is rung once at the beginning of the dump and twice when
the dump is completed.
.SH OPTIONS
.PP
.TP 8
.B "-display \fIdisplay\fP"
This argument allows you to specify the server to connect to; see \fIX(1)\fP.
.PP
.TP 8
.B "-help"
Print out the `Usage:' command syntax summary.
.PP
.TP 8
.B "-nobdrs"
This argument specifies that the window dump should not include the
pixels that compose the X window border.  This is useful in situations
where you may wish to include the window contents in a document 
as an illustration.
.PP
.TP 8
.B "-out \fIfile\fP"
This argument allows the user to explicitly specify the output
file on the command line.  The default is to output to standard out.
.PP
.TP 8
.B "-xy"
This option applies to color displays only. It selects `XY' format dumping
instead of the default `Z' format.
.PP
.TP 8
.B "-add \fIvalue\fP"
This option specifies an signed value to be added to every pixel.
.PP
.TP 8
.B "-frame"
This option indicates that the window manager frame should be included when
manually selecting a window.
.SH ENVIRONMENT
.PP
.TP 8
.B DISPLAY
To get default host and display number.
.SH FILES
.PP
.TP 8
.B XWDFile.h
X Window Dump File format definition file.
.SH SEE ALSO
xwud(1), xpr(1), X(1)
.SH COPYRIGHT
Copyright 1988, Massachusetts Institute of Technology.
.br
See \fIX(1)\fP for a full statement of rights and permissions.
.SH AUTHORS
Tony Della Fera, Digital Equipment Corp., MIT Project Athena
.br
William F. Wyatt, Smithsonian Astrophysical Observatory
