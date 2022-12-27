.TH BITMAP 1 "Release 4" "X Version 11"
.SH NAME
bitmap, bmtoa, atobm \- bitmap editor and converter utilities for X

.SH SYNOPSIS
.B bitmap
[-options ...] \fIfilename\fP \fIWIDTH\fPx\fIHEIGHT\fP
.sp
.B bmtoa
[-chars ...] [\fIfilename\fP]
.sp
.B atobm
[-chars \fIcc\fP] [-name \fIvariable\fP]
[-xhot \fInumber\fP] [-yhot\fP \fInumber\fP] [\fIfilename\fP]
.SH DESCRIPTION
The \fIbitmap\fP program is a rudimentary tool for creating or editing
rectangular images made up of 1's and 0's.  Bitmaps are used in X for
defining clipping regions, cursor shapes, icon shapes, and tile and stipple
patterns.
.PP
The \fIbmtoa\fP and \fIatobm\fP filters convert \fIbitmap\fP files (FILE
FORMAT) to and from ASCII strings.  They are most commonly used to 
quickly print out bitmaps and to generate versions for including in text.
.SH USAGE
\fIBitmap\fP displays grid in which each square represents a single bit in
the picture being edited.  Squares can be set, cleared, or inverted directly
with the buttons on the pointer and a menu of higher level operations such
as draw line and fill circle is provided to the side of the grid.  Actual size
versions of the bitmap as it would appear normally and inverted appear below 
the menu.
.PP
If the bitmap is to be used for defining a cursor, one of the squares in the
images may be designated as the \fIhotspot\fP.  This determines where the
cursor is actually pointing.  For cursors with sharp tips (such as arrows or
fingers), this is usually at the end of the tip; for symmetric cursors (such
as crosses or bullseyes), this is usually at the center.
.PP
Bitmaps are stored as small C code fragments suitable for including in 
applications.  They provide an array of bits as well as symbolic constants
giving the width, height, and hotspot (if specified) that may be used in
creating cursors, icons, and tiles.
.PP
The \fIWIDTHxHEIGHT\fP argument gives the size to use when creating a new
bitmap (the default is 16x16).  Existing bitmaps are always edited at
their current size.  
.PP
If the \fIbitmap\fP window is resized by the window manager, the size of the
squares in the grid will shrink or enlarge to fit.
.SH "OPTIONS"
.PP
\fIBitmap\fP accepts the following options:
.TP 4
.B \-help
This option will cause a brief description of the allowable options and 
parameters to be printed.
.TP 4
.B \-display \fIdisplay\fP
This option specifies the name of the X server to used.
.TP 4
.B \-geometry \fIgeometry\fP
This option specifies the placement and size of the bitmap window on the
screen.  See \fIX\fP for details.
.TP 4
.B \-nodashed
This option indicates that the grid lines in the work area should not be
drawn using dashed lines.  Although dashed lines are prettier than solid
lines, on some servers they are significantly slower.
.TP 4
.B \-name \fIvariablename\fP
This option specifies the variable name to be used when writing out the
bitmap file.  The default is to use the basename of the \fIfilename\fP command
line argument.
.TP 4
.B \-bw \fInumber\fP
This option specifies the border width in pixels of the main window.
.TP 4
.B \-fn \fIfont\fP
This option specifies the font to be used in the buttons.
.TP 4
.B \-fg \fIcolor\fP
This option specifies the color to be used for the foreground.
.TP 4
.B \-bg \fIcolor\fP
This option specifies the color to be used for the background.
.TP 4
.B \-hl \fIcolor\fP
This option specifies the color to be used for highlighting.
.TP 4
.B \-bd \fIcolor\fP
This option specifies the color to be used for the window border.
.TP 4
.B \-ms \fIcolor\fP
This option specifies the color to be used for the pointer (mouse).
.PP
\fIBmtoa\fP accepts the following option:
.TP 4
.B \-chars \fIcc\fP
This option specifies the pair of characters to use in the string version
of the bitmap.  The first character is used for 0 bits and the second character
is used for 1 bits.  The default is to use dashes (-) for 0's and sharp signs
(#) for 1's.
.PP
\fIAtobm\fP accepts the following options:
.TP 4
.B \-chars \fIcc\fP
This option specifies the pair of characters to use when converting string
bitmaps into arrays of numbers.  The first character represents a 0 bit and
the second character represents a 1 bit.  The default is to use dashes (-)
for 0's and sharp signs (#) for 1's.
.TP 4
.B \-name \fIvariable\fP
This option specifies the variable name to be used when writing out the
bitmap file.  The default is to use the basename of the \fIfilename\fP command
line argument or leave it blank if the standard input is read.
.TP 4
.B \-xhot \fInumber\fP
This option specifies the X coordinate of the hotspot.  Only positive values
are allowed.  By default, no hotspot information is included.
.TP 4
.B \-yhot \fInumber\fP
This option specifies the Y coordinate of the hotspot.  Only positive values
are allowed.  By default, no hotspot information is included.
.SH CHANGING GRID SQUARES
Grid squares may be set, cleared, or inverted by pointing to them and 
clicking one of the buttons indicated below.  Multiple squares can be changed
at once by holding the button down and dragging the cursor across them.  Set
squares are filled and represent 1's in the bitmap; clear squares are empty
and represent 0's.
.RS .5in
.TP 4
.I "Button 1"
.br
This button (usually leftmost on the pointer) is used to set one or 
more squares.  The
corresponding bit or bits in the bitmap are turned on (set to 1) and the square
or squares are filled.
.TP 4
.I "Button 2"
.br
This button (usually in the middle) is used to invert one or more squares.  The
corresponding bit or bits in the bitmap are flipped (1's become 0's and 0's 
become 1's).
.TP 4
.I "Button 3"
.br
This button (usually on the right) is used to clear one or more squares.  The
corresponding bit or bits in the bitmap are turned off (set to 0) and the 
square or squares are emptied.
.RE
.SH MENU COMMANDS
To make defining shapes easier, \fIbitmap\fP provides 13 commands for
drawing whole sections of the grid at once, 2 commands for manipulating the
hotspot, and 2 commands for updating the bitmap file and exiting.  A command
buttons for each of these operations is located to the right of the grid.
.PP
Several of the commands operate on rectangular portions of the grid.  These
areas are selected after the command button is pressed by 
moving the cursor to the upper left square of the desired area, pressing
a pointer button, dragging the cursor to the lower right hand corner (with the
button still pressed) , and then releasing the button.  The command may be
aborted by pressing any other button while dragging or by releasing outside
the grid.
.PP
To invoke a command, move the pointer over that command and click any button.
.RS .5in
.TP 4
.I Clear All
This command is used to clear all of the bits in the bitmap as if Button 3 
had been dragged through every square in the grid.  It cannot be undone.
.TP 4
.I Set All
This command is used to set all of the bits in the bitmap as if Button 1 
had been dragged through every square in the grid.  It cannot be undone.
.TP 4
.I Invert All
This command is used to invert all of the bits in the bitmap as if Button 2 
had been dragged through every square in the grid.
.TP 4
.I Clear Area
This command is used to clear a region of the grid as if Button 3 had been
dragged through each of the squares in the region.  When this command is
invoked, the cursor will change shape to indicate that the area to be 
cleared should be selected as outlined above.
.TP 4
.I Set Area
This command is used to set a region of the grid as if Button 1 had been
dragged through each of the squares in the region.  When this command is
invoked, the cursor will change shape to indicate that the area to be 
set should be selected as outlined above.
.TP 4
.I Invert Area
This command is used to inverted a region of the grid as if Button 2 had been
dragged through each of the squares in the region.  When this command is
invoked, the cursor will change shape to indicate that the area to be 
inverted should be selected as outlined above.
.TP 4
.I Copy Area
This command is used to copy a region of the grid from one location to another.
When this command is invoked, the cursor will change shape to indicate that
the area to be copied should be selected as outlined above.  The cursor should
then be clicked on the square to which
the upper left hand corner of the region should be copied.
.TP 4
.I Move Area
This command is used to move a region of the grid from one location to another.
When this command is invoked, the cursor will change shape to indicate that
the area to be moved should be selected as outlined above.  The cursor should
then be clicked on the square to which
the upper left hand corner of the region should be moved.  Any squares in the
region's old position that aren't also in the new position are cleared.
.TP 4
.I Overlay Area
This command is used to copy all of the set squares in a region of the grid 
from one location to another.
When this command is invoked, the cursor will change shape to indicate that
the area to be copied should be selected as outlined above.  The cursor 
should then be clicked on the square to which
the upper left hand corner of the region should be overlaid.  Only the squares
that are set in the region will be touched in the new location.
.TP 4
.I Line
This command will set the squares in a line between two points.  When this
command is invoked, the cursor will change shape to indicate that the pointer
should be clicked on the two end points of the line.
.TP 4
.I Circle
This command will set the squares on a circle specified by a center and a 
point on the curve.  When this command is invoked, the cursor will change
shape to indicate that the pointer should be clicked on the center of the
circle and then over a point on the curve.  Small circles may not look very
round because of the size of the grid and the limits of having to work with
discrete pixels.
.TP 4
.I Filled Circle
This command will set all of the squares in a circle specified by a center
and a point on the curve.  When this command is invoked, the cursor will change
shape to indicate that the pointer should be clicked on the center of the
circle and then over a point on the curve.  All squares side and including
the circle are set.
.TP 4
.I Flood Fill
This command will set all clear squares in an enclosed shape.  When this 
command is invoked, the cursor will change shape to indicate that the pointer
should be clicked on any empty square inside the shape to be filled.  All
empty squares that border horizontally or vertically with the indicated square
are set out to the enclosing shape.  If the shape is not closed, the entire
grid will be filled.
.TP 4
.I Set Hot Spot
This command designates one square in the grid as the hot spot if this
bitmap to be used for defining a cursor.  When the command is invoked, the
cursor will change indicating that the pointer should be clicked on the
square to contain the hot spot.
.TP 4
.I Clear Hot Spot
This command removes any designated hot spot from the bitmap.
.PP
.TP 4
.I Write Output
This command writes a small fragment of C code representing the bitmap
to the filename specified on the
command line.  If the file already exists, the original file will be renamed
to \fIfilename~\fP before the new file is created.  If an error occurs in
either the renaming or the writing of the bitmap file, a dialog box will 
appear asking whether or not \fIbitmap\fP should use \fI/tmp/filename\fP 
instead.  
.PP
.TP 4
.I Quit
This command causes \fIbitmap\fP to display a dialog box asking whether or not
it should save the bitmap (if it has changed) and then exit.  Answering 
\fIyes\fP is the same as invoking \fIWrite Output\fP; \fIno\fP causes
\fIbitmap\fP to simply exit; and \fIcancel\fP will abort the \fIQuit\fP
command so that more changes may be made.
.RE
.SH "FILE FORMAT"
The \fIWrite Output\fP command stores bitmaps as simple C program fragments 
that can be compiled into programs, referred to by X Toolkit pixmap resources, 
manipulated by other programs (see \fIxsetroot\fP), or read in using utility 
routines in the various programming libraries.  The width and height of the
bitmap as well as the hotspot, if specified, are written as preprocessor 
symbols at the start of the file.  The bitmap image is then written out as an
array of characters:
.sp
.nf
        #define \fBname\fP_width 11
        #define \fBname\fP_height 5
        #define \fBname\fP_x_hot 5
        #define \fBname\fP_y_hot 2

        static char \fBname\fP_bits[] = {
            0x91, 0x04, 0xca, 0x06, 0x84, 
            0x04, 0x8a, 0x04, 0x91, 0x04
        };
.fi
.sp
The \fBname\fP prefix to the preprocessor symbols and to the bits array
is constructed from the \fIfilename\fP argument given on the command line.
Any directories are stripped off the front of the name 
and any suffix beginning with a 
period is stripped off the end.  Any remaining non-alphabetic characters are
replaced with underscores.
The \fIname_x_hot\fP and \fIname_y_hot\fP symbols will only be present if 
a hotspot has been designated using the \fISet Hot Spot\fP command.
.PP
Each character in the the array contains 8 bits from one row of the image 
(rows are padded out at the end to a multiple of 8 to make this is possible).
Rows are written out from left to right and top to bottom.  The first
character of the array holds the leftmost 8 bits of top line, and the last
characters holds the right most 8 bits (including padding) of the bottom line.
Within each character, the leftmost bit in the bitmap is the least significant
bit in the character.  
.PP
This process can be demonstrated visually by splitting a row into words
containing 8 bits each, reversing the bits each word (since Arabic numbers
have the significant digit on the right and images have the least significant
bit on the left), and translating each word from binary to hexadecimal.  
.PP
In the following 
example, the array of 1's and 0's
on the left represents a bitmap containing 5 rows and 11 columns that spells
\fIX11\fP.  To its 
right is is the same array split into 8 bit words with each row padded
with 0's so that it is a multiple of 8 in length (16):
.sp
.ce 5
10001001001            10001001 00100000
01010011011            01010011 01100000
00100001001            00100001 00100000
01010001001            01010001 00100000
10001001001            10001001 00100000
.sp
Reversing the bits in each word of the padded, split version of the bitmap
yields the left hand figure below.  Interpreting each word as hexadecimal
number yields the array of numbers on the right:
.sp
.ce 5
10010001 00000100            0x91 0x04
11001010 00000110            0xca 0x06
10000100 00000100            0x84 0x04
10001010 00000100            0x8a 0x04
10010001 00000100            0x91 0x04
.sp
The character array can then be generated by reading each row from left to
right, top to bottom:
.sp
.nf
        static char \fBname\fP_bits[] = {
           0x91, 0x04, 0xca, 0x06, 0x84, 
           0x04, 0x8a, 0x04, 0x91, 0x04
        };
.fi
.sp
The \fIbmtoa\fP program may be used to convert \fIbitmap\fP files into
arrays of characters for printing or including in text files.  The \fIatobm\fP
program can be used to convert strings back to \fIbitmap\fP format.
.SH USING BITMAPS IN PROGRAMS
The format of \fIbitmap\fP files is designed to make bitmaps and cursors
easy to use within X programs.  The following code could be used to create
a cursor from bitmaps defined in \fIthis.cursor\fP and \fIthis_mask.cursor\fP:
.sp
.nf
        #include "this.cursor"
        #include "this_mask.cursor"

        XColor foreground, background;
        /* fill in foreground and background color structures */
        Pixmap source = XCreateBitmapFromData (display, drawable, 
                this_bits, this_width, this_height);
        Pixmap mask = XCreateBitmapFromData (display, drawable, 
                this_mask_bits, this_mask_width, this_mask_height);
        Cursor cursor = XCreatePixmapCursor (display, source, mask, 
                foreground, background, this_x_hot, this_y_hot);
.sp
.fi
.PP
Additional routines are available for reading in \fIbitmap\fP files and 
returning the data in the file, in Bitmap (single-plane Pixmap for use with 
routines that require stipples), or full depth Pixmaps (often used for window 
backgrounds and borders).  Applications writers should be careful to understand
the difference between Bitmaps and Pixmaps so that their programs function
correctly on color and monochrome displays.
.PP
For backward compatibility, \fIbitmap\fP will also accept X10 format 
\fIbitmap\fP files.  However, when the file is written out again it will
be in X11 format
.SH X DEFAULTS
.PP
.I Bitmap
uses the following resources:
.TP 4
.B Background
The window's background color.  Bits which are 0 in the bitmap are
displayed in this color.  This option is useful only on color
displays.  The default value is \fIwhite\fP.
.PP
.TP 4
.B BorderColor
The border color.  This option is useful only on color displays. 
The default value is \fIblack\fP.
.PP
.TP 4
.B BorderWidth
The border width.  The default value is 2.
.PP
.TP 4
.B BodyFont
The text font.  The default value is \fIvariable\fP.
.PP
.TP 4
.B Dashed
If ``off'', then \fIbitmap\fP will draw the grid lines with solid lines.
The default is ``on''.
.PP
.TP 4
.B Foreground
The foreground color.  Bits which are 1 in the bitmap are
displayed in this color.  This option is useful only on color
displays. The default value is \fIblack\fP.
.PP
.TP 4
.B Highlight
The highlight color.
.I bitmap
uses this color to show the hot spot and to indicate rectangular areas
that will be affected by the
.I Move Area, Copy Area, Set Area, Clear Area,
and
.I Invert Area
commands.   If a highlight color is not given, then
.I bitmap
will highlight by inverting.  This option is useful only on color displays.
.PP
.TP 4
.B Mouse
The pointer (mouse) cursor's color.  This option is useful only on color displays.
The default value is \fIblack\fP.
.PP
.TP 4 
.B Geometry
The size and location of the bitmap window.
.PP
.TP 4
.B Dimensions
The 
.I WIDTHxHEIGHT
to use when creating a new bitmap.
.SH "SEE ALSO"
X(1), \fIXlib - C Language X Interface\fP (particularly the section 
on \fIManipulating Bitmaps\fP), \fIXmuReadBitmapDataFromFile\fP
.SH BUGS
.PP
The old command line arguments aren't consistent with other X programs.
.PP  
If you move the pointer too fast while holding a pointer button down,
some squares may be missed.  This is caused by limitations in how
frequently the X server can sample the pointer location.
.PP
There is no way to write to a file other than the one specified on the
command line.
.PP
There is no way to change the size of the bitmap once the program
has started.
.PP
There is no \fIundo\fP command.
.SH COPYRIGHT
Copyright 1988, Massachusetts Institute of Technology.
.br
See \fIX(1)\fP for a full statement of rights and permissions.
.SH AUTHOR
\fIbitmap\fP by Ron Newman, MIT Project Athena; 
documentation, \fIbmtoa\fP, and \fIatobm\fP by Jim Fulton, MIT X Consortium.
