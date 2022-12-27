.TH XEYES 1 "Release 4" "X Version 11"
.SH NAME
xeyes \- watch over your shoulder
.SH SYNOPSIS
.B xeyes
[-option ...]
.SH DESCRIPTION
.I Xeyes
watches what you do and reports to the Boss.
.SH OPTIONS
.TP 8
.B \-fg \fIforeground color\fB
choose a different color for the pupil of the eyes.
.TP 8
.B \-bg \fIbackground color\fB
choose a different color for the background.
.TP 8
.B \-outline \fIoutline color\fB
choose a different color for the outline of the eyes.
.TP 8
.B \-center \fIcenter color\fB
choose a different color for the center of the eyes.
.TP 8
.B \-backing \fI{ WhenMapped Always NotUseful }\fB
selects an appropriate level of backing store.
.TP 8
.B \-geometry \fIgeometry\fB
define the initial window geometry; see \fIX(1)\fP.
.TP 8
.B \-display \fIdisplay\fB
specify the display to use; see \fIX(1)\fP.
.TP 8
.B \-bd \fIborder color\fB
choose a different color for the window border.
.TP 8
.B \-bw \fIborder width\fB
choose a different width for the window border.
.TP 8
.B \-shape
uses the SHAPE extension to shape the window.
.SH "SEE ALSO"
X(1), X Toolkit documentation
.SH COPYRIGHT
Copyright 1988, Massachusetts Institute of Technology.
.br
See \fIX(1)\fP for a full statement of rights and permissions.
.SH AUTHOR
Keith Packard, MIT X Consortium
