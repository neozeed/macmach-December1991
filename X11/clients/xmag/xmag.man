.TH XMAG 1 "Release 4" "X Version 11"
.SH NAME
xmag - magnify parts of the screen
.SH SYNOPSIS
.B "xmag"
[-option ...]
.SH DESCRIPTION
.PP
The \fIxmag\fP program allows you to magnify portions of the screen.  If no
explicit region is specified, a square centered around the pointer is displayed
indicating the area to be enlarged.  Once a region has been selected, a window
is popped up showing a blown up version of the region in which each pixel in 
the source image is represented by a small square of the same color.  Pressing
Button1 on the pointer in the enlargement window pops up a small window 
displaying the position, number, and RGB value of the pixel
under the pointer until the
button is released.  Pressing the space bar or any other pointer button removes
the enlarged image so that another region may be selected.
Pressing ``q'', ``Q'', or ``^C'' in the 
enlargement window exits the program.
.SH OPTIONS
.TP 8
.B \-display \fIdisplay\fP
This option specifies the X server to use for both reading the screen and
displaying the enlarged version of the image.
.TP 8
.B \-geometry \fIgeom\fP
This option specifies the size and/or location of the enlargement window.  By
default, the size is computed from the size of the source region and the 
desired magnification.  Therefore, only one of \fB\-source\fI size\fR and 
\fB\-mag \fImagfactor\fR options may be specified if a window size is given 
with this option.
.TP 8
.B \-source \fIgeom\fP
This option specifies the size and/or location of the source region on the
screen.  By default, a 64x64 square centered about the pointer is provided
for the user to select an area of the screen.  The size of the source is used
with the desired magnification to compute the default enlargement window
size.  Therefore, only one of \fB\-geometry \fIsize\fR and 
\fB\-mag \fImagfactor\fR options may be specified if a 
source size is given with this option.
.TP 8 
.B \-mag \fImagfactor\fP
This option specifies an integral factor by which the source region should be
enlarged.  The default magnification is 5.
This is used with the size of the source to compute the default
enlargement window size.  Therefore, only one of \fB-geometry \fIsize\fR and
\fB\-source \fIgeom\fR options may be specified if a magnification factor is 
given with this option.
.TP 8
.B \-bw \fIpixels\fP
This option specifies the width in pixels of the border surrounding the
enlargement window.
.TP 8
.B \-bd \fIcolor\fP
This option specifies the color to use for the border surrounding the 
enlargement window.
.TP 8
.B \-bg \fIcolororpixelvalue\fP
This option specifies the name of the color to be used as the background of 
the enlargement window.  If the name begins with a percent size (%), it is
interpreted to be an absolute pixel value.  This is useful when displaying
large areas since pixels that are the same color as the background do not
need to be painted in the enlargement.  The default is to use the BlackPixel
of the screen.
.TP 8
.B \-fn \fIfontname\fP
This option specifies the name of a font to use when displaying pixel values
(used when Button1 is pressed in the enlargement window).
.TP 8
.B \-z
This option indicates that the server should be grabbed during the dynamics
and the call to XGetImage.  This is useful for ensuring that clients don't
change their state as a result of entering or leaving them with the pointer.
.SH "X DEFAULTS"
The \fIxmag\fP program uses the following X resources:
.TP 8
.B geometry (\fPclass\fB Geometry)
Specifies the size and/or location of the enlargement window.
.TP 8
.B source (\fPclass\fB Source)
Specifies the size and/or location of the source region on the screen.
.TP 8
.B magnification (\fPclass\fB Magnification)
Specifies the enlargement factor.
.TP 8
.B borderWidth (\fPclass\fB BorderWidth)
Specifies the border width in pixels.
.TP 8
.B borderColor (\fPclass\fB BorderColor)
Specifies the color of the border.
.TP 8
.B background (\fPclass\fB Background)
Specifies the color or pixel value to be used for the background of the 
enlargement window.
.TP 8
.B font (\fPclass\fB Font)
Specifies the name of the font to use when displaying pixel values when the
user presses Button1 in the enlargement window.
.SH "SEE ALSO"
X(1), xwd(1)
.SH BUGS
.PP
This program will behave strangely on displays that support windows of 
different depths.
.PP
Because the window size equals the source size times the magnification, you
only need to specify two of the three parameters.  This can be confusing.
.PP
Being able to drag the pointer around and see a dynamic display
would be very nice.
.PP
Another possible interface would be for the user to drag out the desired
area to be enlarged.
.SH COPYRIGHT
Copyright 1988, Massachusetts Institute of Technology.
.SH AUTHOR
Jim Fulton, MIT X Consortium
