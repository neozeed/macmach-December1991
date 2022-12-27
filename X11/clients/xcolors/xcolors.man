.TH XCOLORS 1 "29 November 1989" "X Version 11"
.SH NAME
xcolors \- display all X11 color names and colors
.SH SYNOPSIS
.B xcolors
[
.B \-start
startcolor
]
.RI [ " XtOption " "] ..."
.SH DESCRIPTION
.I xcolors
reads the rgb.txt file that defines symbolic names for RGB colors and
displays all colors found.  Accepts all standard X toolkit options.

The
.I -start
option can be given to specify a color name to start with; colors
before this one in the rgb.txt file are skipped.  This option is
useful if there are more colors than will fit on your screen or in
your colormap.
.SH FILES
.TP
/usr/lib/X11/rgb.txt
X11 color names and values.
.SH BUGS
Assumes that all names for the same color are consecutive in the
rgb.txt file.
.SH AUTHORS
Stephen Gildea, Paul Vixie
