.TH XEV 1 "Release 4" "X Version 11"
.SH NAME
xev - print contents of X events
.SH SYNOPSIS
.B "xev"
[\-display \fIdisplayname\fP] [\-geometry \fIgeom\fP]
.SH DESCRIPTION
.PP
\fIXev\fP creates a window and then asks the X server to send it notices called
\fIevents\fP whenever anything happens to the window (such as being moved,
resized, typed in, clicked in, etc.).  It is useful for seeing what causes
events to occur and to display the information that they contain.
.SH OPTIONS
.TP 8
.B \-display \fIdisplay\fP
This option specifies the X server to contact.
.TP 8
.B \-geometry \fIgeom\fP
This option specifies the size and/or location of the window.
.SH "SEE ALSO"
X(1), xwininfo(1), xdpyinfo(1), Xlib Programmers Manual, X Protocol
Specification
.SH COPYRIGHT
Copyright 1988, Massachusetts Institute of Technology.
.br
See \fIX(1)\fP for a full statement of rights and permissions.
.SH AUTHOR
Jim Fulton, MIT X Consortium
