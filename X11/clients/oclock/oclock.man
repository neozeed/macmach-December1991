.TH OCLOCK 1 "Release 4" "X Version 11"
.SH NAME
oclock \- display time of day
.SH SYNOPSIS
.B oclock
[-option ...]
.SH DESCRIPTION
.I Clock
simply displays the current time on an analog display
.SH OPTIONS
.TP 8
.B \-fg \fIforeground color\fB
choose a different color for the both hands and the jewel of the clock
.TP 8
.B \-bg \fIbackground color\fB
choose a different color for the background.
.TP 8
.B \-jewel \fIjewel color\fB
choose a different color for the jewel on the clock.
.TP 8
.B \-minute \fIminute color\fB
choose a different color for the minute hand of the clock.
.TP 8
.B \-hour \fIhour color\fB
choose a different color for the hour hand of the clock.
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
choose a different width for the window border.  As the Clock widget changes
its border around quite a bit, this is most usefully set to zero.
.TP 8
.B \-noshape
causes the clock to not reshape itself and ancestors to exactly
fit the outline of the clock.
.SH COLORS
Although the default colors for the Clock widget are black and white, the
widget was designed in color; unfortunately, the toolkit makes specifying
these colors in a device-independent manner difficult.  If you want to
see the correct colors, add the following lines to your resource file:
.sp 1
Clock*Background: grey
.br
Clock*BorderColor: light blue
.br
Clock*hour: yellow
.br
Clock*jewel: yellow
.br
Clock*minute: yellow
.SH "SEE ALSO"
X(1), X Toolkit documentation
.SH COPYRIGHT
Copyright 1989, Massachusetts Institute of Technology.
.br
See \fIX(1)\fP for a full statement of rights and permissions.
.SH AUTHOR
Keith Packard, MIT X Consortium
