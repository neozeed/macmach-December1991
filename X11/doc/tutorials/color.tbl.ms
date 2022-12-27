.\ "MIT header page and copyright notice
.\" MIT page header and footers
.ps 10
.nr PS 10
.nr LL 5.75i
.nr PO .75i
.EH ''''
.OH ''''
.EF ''''
.OF ''''
\.".EQ
\."delim !!
\.".EN
.ad b
.sp 8
.ce 4
\s+1\fBColor\fP\s-1
.sp
\s+1\fBA Chapter from O'Reilly and Associates\fP\s-1
.br
\s+1\fBXlib Programming Manual\fP\s-1
.sp 6
.ce 4
\s-1Adrian Nye
.br
Tim O'Reilly\s+1
.sp 6p
\s-1O'Reilly and Associates, Inc.\s+1
.sp 2
.bp 
.sp 8
.LP
The X Window System is a trademark of MIT.
.sp .5
Copyright \(co 1988  O'Reilly and Associates, Inc.
.sp .5
Permission to use, copy, modify and distribute this documentation for any
purpose and without fee is hereby granted, provided that the above copyright
notice appears in all copies and that both that copyright notice and this
permission notice appear in supporting documentation, and that the name of
O'Reilly and Associates, Inc. not be used in in advertising or publicity pertaining
to distribution of the software without specific, written prior permission.
O'Reilly and Associates, Inc makes no representations about the 
suitability of the software described herein for any purpose.
It is provided ``as is'' without express or implied warranty.
.bp 
.sp 1
.ce
\s+1\fBAcknowledgments\fP\s-1
.sp 2
We'd like to thank Robert Scheifler for reviewing this document.
His comments on an earlier draft are represented here, but he has
not seen this draft, and it is not certain that his view
is fully and correctly represented.
.sp .5
Al Tabayoyon, Paul Shearer, and others at Tektronix also provided
valuable comments on earlier drafts.
.pn +170
.bp
.nr HM .75i
.nr FM 3i
.ad b
.af PN 1
.EH ''''
.OH ''''
.EF '%''\f(HIXlib Programming Manual\fR'
.OF '\f(HIColor\fR''%'
.sp 3
.ti 5.65i
\s24\f(HB7\fR\s10
.sp 
.ti 5i
\s20\f(HBColor\fR\s10
.sp 4
.IP "" 6n
A typical X application allows the user to specify
colors for the background and border of each of its windows, colors
for the cursor, and foreground and background colors to be set in GCs
for drawing text and graphics.*   More complex applications (such as 
.FS
*The version of this chapter in our book includes a number of figures, 
which it would have been difficult to provide on the MIT tape.
The figures and their references have been omitted.
.FE
Computer Aided Design) might use color to distinguish physical or 
logical layers.  Still more complex 
applications, such as imaging, might use fine gradations of color to 
represent real-world data.  Yet in discussing window attributes, and 
how to set the foreground and background members of the GC, we have 
spoken only of pixel values.
.sp .5
How are these pixel values translated to colors?  And how must an X 
client manage color if it is to run successfully on the wide 
variety of display hardware available in the X environment?
.sp .5
Because X must support a wide variety of systems with differing
display hardware, the Xlib color-handling mechanisms are fairly complex.  
Even programmers who have previously written color graphics 
applications will find there are some new concepts to learn.
.sp .5
This chapter starts out by describing the different types of displays that 
an X application may run on, and the mechanisms Xlib provides for 
determining the display type.  
It then describes the simplest color-allocation 
mechanisms, which could be used by applications whose principal use of 
color is for decoration.  It 
proceeds to discuss more complex color applications, and concludes with a 
section on writing applications that will be portable across different types of 
color and monochrome displays.
.sp 2
.IP \s+4\f(HB7.1\fR\s-4  6n
\s+4\f(HBBasic Color Terms and Concepts\fR\s-4
.sp 
Most color displays on the market today are based on the RGB 
color model.  Each pixel on the screen is actually made up of 
three phosphors:  one red, one green, and one blue.  Each of 
these three phosphors is sensitive to a separate electron beam.
When all three phosphors are fully illuminated, the pixel appears white to 
the human eye.  When all three are dark, the pixel appears black.  When the 
illumination of each primary color varies, the three phosphors generate a 
subtractive color that might seem surprising.  For example, equal portions of 
red and green, with no admixture of blue, make yellow.
.sp .5
You no doubt know that a color display uses multiple bits per pixel 
(also referred to as multiple planes) to specify colors.   There is a 
type of display (which we'll discuss later) in which these bits are 
used directly to control the illumination of the red, green and blue 
phosphors that make up each pixel.  Far more commonly, though, a 
\fIcolormap\fP  is used to translate each 
pixel's bit value into the visible colors you see 
on the screen.
.sp .5
A colormap is no more than a lookup table.  Any given pixel 
value is used as an index into this table--for example, a pixel 
value of 16 will select the sixteenth element, or \fIcolorcell\fP.
.sp .5
On the most common type of color system, each colorcell contains 
separate intensity values for each of the three primary colors.  
Depending on the display hardware, these values can be 
anywhere from 1 to 16 bits each, and are most commonly 8 bits 
each.
.ne 5
A pixel value uniquely identifies a particular 
colorcell.  Each pixel value in the visible portions of a window
is continuously read out of display memory and looked up in the colormap. 
The RGB values in the specified colorcell control the intensity of the three 
primary colors and thus determine the color that is displayed on the screen.
.sp .5
The range of colors possible on the display is a function of the 
number of bits available in the colormap for RGB specification.  
If eight bits is available for each primary, then the range of 
possible colors is
(about 16 million colors).
.sp .5
However, the number of different colors that can be displayed 
on the screen at any one time is a function of the number of 
planes.  A four-plane system could index
colorcells (16 distinct colors); an eight-plane system could index
colorcells (256 distinct colors); and a 24-plane system could index
colorcells (over 16 million distinct colors).
.sp 2
.IP \s+4\f(HB7.2\fR\s-4  6n  
\s+4\f(HBDifferences in Display Hardware\fR\s-4\fR
.sp
The description of color mapping given in the previous section was
actually somewhat over-simplified.  There are significant differences
in how the colormap is used on mid-range color displays, monochrome
and gray-scale displays, and high performance color displays.
.sp 2
.IP \s+2\f(HB7.2.1\fR\s-2  6n  
\s+2\f(HBMid-range Color Displays\fR\s-2
.sp
The most common type of color display has between 4 and 8 planes,
and uses the colormap indexing technique described above.
This type of display is so widespread because it provides a
flexible color system while being moderately priced.
The mapping of pixel values to 
colorcells, with arbitrary RGB values stored in each colorcell, 
allows a very large gamut of possible colors, even though a more
limited number can be shown on the screen at any one time.
.sp .5
If the colormap is predefined, and cannot be modified, this type of 
display is referred to as a \fIstatic color\fR display.   
Realistically, a static color display is 
unlikely to exist, since the main advantage of a read-only, predefined 
colormap is the predictability of the mapping between
pixel values and colors, but displays with a 
relatively small number of simultaneous
colors don't lend themselves to applications that use this mapping
(such as 3-D rendering).
.\"The way pixel values are mapped to RGB values described above
.\"is valid for most color displays.  However, you also need to consider monochrome
.\"and high-performance color displays.
.sp 2
.IP \s+2\f(HB7.2.2\fR\s-2 6n
\s+2\f(HBMonochrome and Gray Scale\fR\s-2
.sp
Monochrome (black and white) displays have 
only a single plane of display memory.  Each 
pixel is made up of a single phosphor, which can be either on or off.
.sp .5
Gray scale displays are sometimes used for publishing applications, 
since pixels made up of a single phosphor are smaller than those made 
up of three phosphors, and the resolution is therefore better.
A gray scale display works by looking 
up the intensity of the pixel in the colormap, which for this display 
type contains only a single value.  This
controls the intensity of a single electron beam.  Gray scale can 
be simulated on a color display by making the red, green, and blue 
values equal in a given colorcell to determine the brightness of gray 
pixels on the screen.
.sp .5
A gray scale display might have a read-only colormap, so that the gray
levels in each cell could not be changed.
A monochrome display is an example of this type; it is a single-plane 
display with a two-element colormap.
.sp 2
.IP \s+2\f(HB7.2.3\fR\s-2 6n
\s+2\f(HBHigh Performance Color Displays\fR\s-2
.sp
As memory has become cheaper, and applications more advanced,  
workstations with up to 24 planes have become more common.    With 24 
bits per pixel, it is possible to display many more distinct colors 
at the same time.  This makes it possible to do smooth shading and 
other applications that use a large number of closely-spaced colors.
.sp .5
The problem with so many planes is that a colormap 
of the style used in mid-range color displays
would be impossibly large:  it would contain over sixteen million 
entries.  Instead, the available bits per pixel are broken down into three 
separate colormap indices, one for each primary color.
This approach still allows the full range of colors to be generated, but makes 
the job of loading the colormap much more manageable.
.sp .5
In high-performance displays, having a read-only colormap
makes just as much sense as having it read/write, because nearly 
every color imaginable can be simultaneously available.
With a read-only colormap there is a fixed relationship between the 
pixel values used to select a color and the actual RGB values 
generated.  This makes possible applications that want to calculate
pixel values directly instead of having to calculate colors and then
determine which pixel value represents that color as is necessary when
the colormap is read/write.
.sp .5
In reality, most displays in this class let you use the color resources
in either fashion.  How this is done is our next topic.
.sp 2
.IP \s+2\f(HB7.2.4\fR\s-2 6n  
\s+2\f(HBHow X Manages Different Display Types\fR\s-2
.sp
X describes the color capabilities of the display hardware with
a visual.  A \fIvisual\fR is actually a structure containing
information about a way of using a particular display.
A visual must be specified when creating a colormap or a 
window, and the same visual must be used in creating a window as was used
to create the colormap to be used in that window.   A window can 
inherit its parent's visual, and since the root window is 
created with a default visual, windows will often share this default 
visual.  If you need to get the default visual, you can do so with the 
\f(CWDefaultVisual\fP macro.
.sp .5
.ne 6
The \f(CWVisual\fP structure is intended
to be opaque; programs are not supposed to access its contents directly.  
The reason for this is so that Xlib's authors have the ability to change
the structure without breaking existing clients.
However, the procedure used to avoid accessing its members directly is
cumbersome, and most programmers compromise by flagging their code where
they break the rules so that if \f(CWVisual\fP ever does change, they will
be able to fix the code quickly.
.sp .5
The members of the \f(CWVisual\fP structure that are important to 
understand are \f(CWmap_entries\fP and \f(CWclass\fP.  The other members
are only used in advanced color handling.
The \f(CWmap_entries\fP member specifies how many colorcells there
are in a colormap created with this visual.  On high-performance
color screens with the colormap composed of separate maps for
each primary color, \f(CWmap_entries\fP is the largest of the three 
submaps.  The \f(CWclass\fP member contains a constant specifying one of
six different visual classes,* corresponding to the different types 
of screen: \f(CWGrayScale\fP, \f(CWStaticGray\fP, 
\f(CWPseudoColor\fP, \f(CWStaticColor\fP,
\f(CWDirectColor\fP, or \f(CWTrueColor\fP.
.FS 
*Do not confuse \fIvisual\fR class with \fIwindow\fR class.  
While both are represented in certain structures as the 
\f(CWclass\fR member, and both are set when a window 
is created and cannot be changed, they are 
quite different.  The window 
class is \f(CWInputOutput\fP or \f(CWInputOnly\fP.
The visual class is only part of the 
overall visual, which is the way color
is represented for a window.
.FE
.sp .5
As summarized in Table 7-1, the visual classes 
distinguish between color or monochrome, whether the colormap is 
read/write or read-only, and whether a pixel value provides a single 
index to the colormap or is decomposed into separate indices for red, 
green and blue values.
.sp 
\s-1\f(HITable 7-1.  Comparison of Visual Classes\fR\s+1
.sp 
.TS
linesize(2),tab(@);
l | l | l
l | lp9fCW| lp9fCW.
.sp 1p
Colormap Type@Read/Write@Read-only
.sp 1p
_
.sp 5p
Monochrome/Gray@GrayScale@StaticGray
Single Index for RG&B@PseudoColor@StaticColor
Decomposed Index for RG&B@DirectColor@TrueColor
.sp 8p
.TE
.IP "" 6n
It is important to realize that a visual is not just a simple record 
of the screen characteristics.  A visual is \fIone way\fR of 
interpreting the color resources on a screen.  There may be several 
valid visuals for a particular screen if the colormap is read/write.
Several visuals of the same class may be provided, but at different
depths.  On high-performance screens
that we described in Section 7.2.3, it is possible to use
the colormap as if it were read/write or as if it were read/only.
Both methods have certain advantages and would be used for different
applications.  There would be a separate visual for
each of these ways of using the display hardware.  One of
these visuals would be \f(CWTrueColor\fP and the other 
\f(CWDirectColor\fP.  Some 24-plane 
displays allow the screen to be treated as two separate 12-plane 
\f(CWPseudoColor\fP visuals.  (This allows for "double-buffering," a 
technique useful for animation, or for storing distance data to 
simplify hidden line and plane calculations in 3-D applications.)  
In fact, on some advanced workstations you can use a different visual 
in each window.
.sp .5
A display that supports
the \f(CWDirectColor\fP class can theoretically support any of the 
six visual classes.  A \f(CWPseudoColor\fP display can support 
\f(CWPseudoColor\fP, \f(CWStaticColor\fP, \f(CWGrayScale\fP, or \f(CWStaticGray\fP visuals.  A
\f(CWGrayScale\fP display can support \f(CWGrayScale\fP or 
\f(CWStaticGray\fP.  The three types of screen with read-only 
colormaps can only support visuals of their own class.
But remember that just because a 
certain visual class can theoretically be supported by a certain 
display hardware doesn't mean that the server implementors will
decide to support that class.  
.sp 2
.IP \s+2\f(HB7.2.5\fR\s-2 6n
\s+2\f(HBShareability vs. Changeability\fR\s-2
.sp
Notice that \f(CWPseudoColor\fP, \f(CWDirectColor\fP, and 
\f(CWGrayScale\fP visuals have changeable colormaps, but
\f(CWStaticColor\fP, \f(CWTrueColor\fP, and \f(CWStaticGray\fP have 
immutable colormaps.  Within the
changeable colormaps, it is possible to have both
read-only and read/write colorcells, while in the immutable 
colormaps, you are limited
to only read-only cells.  
.sp .5
One advantage of immutable colormaps is
that all the cells are read-only, and can be shared between clients,
so all the cells are available to every 
client.  Immutable colormaps make it
possible to calculate pixel values from the 
colors desired, since the
mapping between pixel values and colors is predictable.
This technique is necessary for smooth shading and 3-D rendering
algorithms.
The disadvantage of immutable colormaps are that there may not
be the exact color you desire (if there are a small number of planes), 
and you cannot change the colorcell to
change the color of existing pixels on the screen.  To change a color, you have
to redraw the graphics with a new pixel value.
.sp .5
In general, the
advantages of changeable colormaps are that you can have both private 
read/write
cells and shareable read-only cells.  That is why \f(CWPseudoColor\fP and 
\f(CWDirectColor\fP
are the most useful visuals, when a screen supports them.
\f(CWPseudoColor\fP and
\f(CWDirectColor\fP allow you to decide
whether your client really needs read/write cells, or
whether it can use read-only cells.
Read-only usage is preferred
.ne 2
since these cells can be shared by all clients, which means that the
colormap is less likely to run out of free cells.
.sp .5
Try not to
confuse the writeability of colormaps with the writeability 
of colorcells.  A colorcell in a read/write colormap can
be allocated read/write or read-only.  A colorcell in a 
read-only colormap can only be allocated read-only.
A changeable colormap could be made entirely read-only if the server 
or window manager fills all available colorcells.  
.sp .5
The advantages of read/write colorcells, available only in 
changeable colormaps, are that your program can select 
exactly the color you want (as long as
it is physically possible on the screen), and you can change the color
at will, which instantly changes the visible color of everything 
drawn with that pixel value if the colormap is currently installed.  
Although any other client can also change the values in a read/write
cell, it is a convention that only the client that allocated the
cell should change its contents.  You \fIown\fR that pixel value.
Since most clients can't be satisfied with
having no control over their displayed colors, this pixel value
is not shareable.
That means that if several clients that
use changeable colormap cells are running, all 
.ne 4
the cells might be used.  Then some
client will be forced to create its own colormap, with the consequences
described in Section 7.5.
.sp .5
Usually the best trade-off on a color system with a
changeable colormap is to use the default
colormap, which usually provides mostly read-only, shareable cells, 
and also leaves free a few
read/write cells for clients that require them.
Applications
needing read-only cells are encouraged to choose colors from the
RGB color database, by specifying a color name
instead of RGB values.  Using color names will maximize 
the chance of clients
requesting the same RGB values.  Otherwise, clients might
waste cells by allocating very close, but not identical, RGB values
for certain colors.
.sp 2
.IP \s+2\f(HB7.2.6\fR\s-2 6n
\s+2\f(HBThe Default Visual\fR\s-2
.sp
A window is created with a certain visual, which
cannot be changed.  The root window is created with a default visual
when the server initializes.
All windows which inherit the visual from the root window will
use the default visual;  this includes all top-level windows created
with \f(CWXCreateSimpleWindow\fP, and all top-level windows created
with \f(CWXCreateWindow\fP using \f(CWCopyFromParent\fP for
the \f(CIvisual\fP argument.
With \f(CWXCreateSimpleWindow\fP, the visual
is automatically inherited from the parent.
The default visual can also be specified for a window
using \f(CWXCreateWindow\fP and specifying
the \f(CWDefaultVisual\fP macro as the \f(CIvisual\fP argument.
.sp .5
The default visual usually specifies the visual that the server 
implementors expect most of the clients running on their machine 
to want to use.  Therefore, most programs
should use it unless they have color needs 
that can't be satisfied by it.  Programs that require only a small 
number of colors, and don't depend on the colors being exactly as 
specified can use the default visual.
.sp .5
On most color systems, the default visual is
a \f(CWPseudoColor\fP visual.  
On black and white systems, the
default visual class doesn't really matter, because the mere fact that
there is only one display plane determines that a program should
use only \f(CWBlackPixel\fP and \f(CWWhitePixel\fP for pixel values.
.sp 2
.IP \s+2\f(HB7.2.7\fR\s-2 6n 
\s+2\f(HBThe Default Colormap\fR\s-2
.sp
Some displays support multiple hardware colormaps.  Most workstations 
support only one.  In any case, X can manage multiple \fIvirtual 
colormaps\fP.  If the hardware colormap is read/write, then it is 
possible to swap virtual colormaps in and out of the hardware colormap. 
.sp .5
A virtual colormap is created with a call to \f(CWXCreateColormap\fP. 
Like the call to create a window, this routine takes a visual as an 
argument.  In order to be used with a window, a colormap must be 
created using the same visual as the window.
.sp .5
.ne 2
When the server initializes, it creates one virtual colormap called the
default colormap, sets the colormap attribute of the root window, and
installs this colormap.  
.sp .5
Each window can have its own colormap as a window attribute (see 
Chapter 4, \fIWindow Attributes\fR,).  Because the hardware colormap is a 
limited resource that must be shared by all clients, it should generally 
be left to the window manager to install colormaps (although we will tell you 
how to do this later in the chapter).  Unless they have special color needs, 
clients should plan to use the default colormap. 
.sp .5
Table 7-2 shows a typical visual type for 
the default colormap for each
type of display hardware (color, gray scale, and monochrome) and for
each type of hardware colormap (immutable or changeable).
The default visual type for color servers varies widely.  It is
likely to be \f(CWPseudoColor\fP on 4-to-8-plane systems, and
it could be \f(CWTrueColor\fP, \f(CWDirectColor\fP, or 
\f(CWPseudoColor\fP on high-performance systems.
.sp 
\s-1\f(HITable 7-2. Visual Class of Default Colormaps for Various Servers\fR\s+1
.sp 
.TS
linesize(2),tab(@);
l | c s s
^ | l s s
^ | l l l
l | l | l | l.
.sp 1p
Hardware Colormap Type@Display Type@@
.sp 1p
@_@@
.sp 1p
@Color@Gray Scale@Monochrome
.sp 1p
_
.sp 3p
Changeable@varies@\f(CWGrayScale\fP@\f(CWGrayScale\fP
Immutable@\f(CWStaticColor\fP@\f(CWStaticGray\fP@\f(CWStaticGray\fP
.sp 8p
.TE
.IP "" 6n
The contents of the default colormap are server-dependent.  
For servers whose default visual is a \f(CWPseudoColor\fP visual, 
some of the colormap cells will be initially unallocated
cells, and some will probably be read-only sharable cells.
Someday, the default colormap may be
allocated with the \f(CWXA_RGB_DEFAULT_MAP\fP standard colormap 
described in Section 7.6, but few if any servers
or window managers do this today.
.sp .5
On an eight-plane color display, the default
colormap would have read-only cells allocated with evenly distributed
colors produced from any combination of six levels of red, six of green, and 
six of blue.  This yields 216 colors that can be shared by all clients,
and still leaves 40 colorcells to be allocated by clients as
read/write, private cells.
.sp .5
Since the default colormap is of the same 
visual type as the root window,
it can be used only in windows which have the same visual as the
root window.
.sp .5
Monochrome clients should use the default colormap
and select the pixel values returned by the 
\f(CWBlackPixel\fP and \f(CWWhitePixel\fP
macros.  They should not normally create windows with 
a \f(CWStaticGray\fP visual unless it is the 
default, because having multiple virtual colormaps would cause an
installation problem on servers with a single hardware colormap. 
.bp
.sp 2
.IP \s+4\f(HB7.3\fR\s-4 6n  
\s+4\f(HBAllocating Shared Colors\fR\s-4
.sp
.ne 8
On \f(CWPseudoColor\fP, \f(CWDirectColor\fP and \f(CWGrayScale\fP displays, the colormap is 
read-write, and clients can store their own 
RGB values into colorcells, or even swap entire virtual colormaps in and out 
of the hardware colormap.  
Even when the colormap is read/write, though, the 
server or the window manager could load a 
default colormap with a reasonable distribution of predefined colors, 
leaving a percentage of the colorcells free for private color storage by 
clients that require exact colors.   The default colormap is 
returned by the \f(CWDefaultColormap\fP macro.
.sp .5
Since free colorcells can quickly become a scarce resource when 
clients store private color values, simple clients that 
mainly use color for decoration are encouraged always to allocate 
from the pool of predefined colors.
.sp .5
A client attempting to allocate a shared, read-only colorcell doesn't 
\fIspecify\fP a pixel value in order to draw in a given color.  
Instead, it requests a color, and is \fIreturned\fP a pixel value 
that will point to the closest available color.   Colors can be 
specified either as actual RGB values, 
or as names that will be looked up in a predefined color database.  The 
returned pixel value can be used to set the \f(CWbackground_pixel\fP or 
\f(CWborder_pixel\fP attribute of a window, or to set the \f(CWforeground\fP or 
\f(CWbackground\fP member of a GC, which are used by drawing requests.
(See Chapter 4, \fIWindow Attributes\fR, and Chapter 5, \fIThe
Graphics Context\fR, for more information.)
.sp .5
Read-only colorcells can be allocated with the following routines:
.RS
.IP "\f(CWXAllocColor\fP" 16n
Returns the index of the colorcell (a pixel value) that contains the RGB values 
closest to those requested.
.RE
.RS
.IP "\f(CWXAllocNamedColor\fP" 16n
.br
Returns the index of the colorcell that contains the RGB values closest to 
those associated with a specified color name from the ASCII color name 
database.
.RE
.RS
.IP "\f(CWXParseColor\fP" 16n
Parses either an ASCII color name or a hexadecimal color specification into 
RGB values that can be used with \f(CWXAllocColor\fP.
.RE
.sp .5
By convention, clients should allow users to specify colors from the command 
line or in the resource database, using either a color name or a 
hexadecimal 
specification.  Since \f(CWXAllocNamedColor\fP can interpret only color 
names, the combination of \f(CWXParseColor\fP and \f(CWXAllocColor\fP is 
preferred for clients that allocate read/write cells (where
sharing is not an issue).  \f(CWXParseColor\fP parses both forms the 
user may specify.
.sp .5
If you want to find out about the composition of the default colormap before 
allocating colors, you can tell how many colorcells it has
from the information in the \f(CWVisual\fP structure, or if you
wish to respect that this is intended to be an opaque structure,
using \f(CWXGetVisualInfo\fP or \f(CWXMatchVisualInfo\fP as
described in Section 7.7.3.
Using \f(CWXQueryColor\fP and \f(CWXQueryColors\fP you can find
out what RGB values are in each colorcell.  But there is no
way to determine whether a given cell is read-only or read/write,
or how many cells are currently unallocated.
.sp .5
.Nd 2
Applications must allocate colors by trial and error.
The routines that allocate colorcells all have \f(CWStatus\fP return values.
If the call to allocate colorcells returns \f(CWFalse\fP, 
the client may modify the arguments and try again.  
If repeated attempts fail, the client can settle 
with \f(CWBlackPixel\fP and \f(CWWhitePixel\fP, or if
these colors are inadequate, create a new virtual colormap
if the screen has changeable hardware colormap,
or if not, decide that its color needs can't be met
and exit.  For more information, see Section 7.7.
.sp 2
.IP \s+2\f(HB7.3.1\fR\s-2 6n
\s+2\f(HBThe XColor Structure\fR\s-2
.sp
Both \f(CWXAllocColor\fP and \f(CWXAllocNamedColor\fP (as well as other 
functions that manipulate colorcells) take as an argument an \f(CWXColor\fP 
structure.  This structure is used to specify the desired RGB values, 
as well as to return the pixel value.  
.sp .5
The \f(CWXColor\fP structure is shown in Example 7-1.  The 
information it contains closely matches the information
in each cell of the colormap.
.sp 
\s-1\f(HIExample 7-1.  The XColor structure\fR\s+1
.sp 
.nf
.ta 4.5n 2.25i
\s-1\f(CWtypedef struct {
	unsigned long pixel;	/* pixel value */
	unsigned short red, green, blue;        /* RGB values */
	char flags;	/* DoRed,  DoGreen, and/or DoBlue */
	char pad;	/* unused; pads structure to 
		 * even word boundary */
} XColor;\fR\s+1
.fi
.sp .5
The \f(CWpixel\fP member indicates which cell in the colormap
is being set or is having its RGB values queried.  This member is used to set 
the pixel value in the GC or window attributes.
.sp .5
The \f(CWred\fP, \f(CWgreen\fP, and \f(CWblue\fP members are 16-bit 
values, even though many colormaps only use eight bits to specify each 
primary color.  The server will automatically scale the values.  Full 
brightness in a color would be a value of 65535, half-brightness would be 
32767, and off would be 0.  
.sp .5
When allocating colors,
the exact values requested and the closest available values are
not the same in most cases, because there would have to be
different colormap entries to cover every possible triple 
of red, green, and blue.  Even in more reasonable cases,
many of the nonshared colormap entries will have been taken
by other programs.  This means that a client that requires precise
colors should check the returned RGB values to make sure the
values meet its specifications.
.sp .5
The \f(CWflags\fP member of the \f(CWXColor\fP structure is
a bitwise OR of the symbols \f(CWDoRed\fP, \f(CWDoGreen\fP, 
and \f(CWDoBlue\fP.  These flags are used to specify which of the red, green, 
and blue values should be read while changing the RGB values in a 
read/write colorcell.
.sp 2
.IP \s+2\f(HB7.3.2\fR\s-2 6n  
\s+2\f(HBThe Color Name Database\fR\s-2
.sp
In order to simplify color specification and to promote sharing
of colors, X provides an ASCII color name database.  
By using names from this database, you are reasonably sure of 
getting a color close to the one you request.    
Server implementors will change the RGB values corresponding to
each color name to make sure that the appropriate color
appears on their screen.  If they haven't done this, the colors
that appear may be off.
.sp .5
It is also important to note that the color names are not
specified by the X11 protocol or Xlib.  Therefore, server implementors
may change them, but more often they will simply add to the list.
(Note that some servers allow users to 
customize this file.   For more information, see Volume Three, \fIX 
Window System User's Guide\fP.)
.sp .5
Table 7-3 shows the color names and corresponding RGB values in 
the default color database.  The text version of this database in the 
standard distribution on a UNIX system is in 
the file \fI/usr/lib/X11/rgb.txt\fP.  The location of this file may
vary.  
.sp .5
The color names in the color database are strings in which each
character uses the ISO Latin-1 encoding.  The ISO (International
Standards Organization) encoding is used by virtually all workstations manufacturers.
What this means is that the first 127 character codes correspond to
7-bit ASCII, and are the normal English characters that appear on U.S.
keyboards.  But ISO characters are 8-bit, and the characters from 
128 to 255 are used for characters
with accents and other variations, necessary for other western 
languages.  
.sp .5
Server vendors should be able
to supply a color database file for each foreign language.
The RGB values would be the same, but the names would be different.
In the English file, the entry for green is encoded with the
ISO character codes 103 (g), 114 (r), 101 (e), 101 (e), 110 (n).
In German, the same entry would be for \fIgr\*(u:n\fR, encoded with
the ISO codes 103 (g), 114 (r), 252 (\*(u:), 110 (n).
In a workstation configured for German, this string will be just
as easy to type as \fIgreen\fR is in English, because there will be
a keyboard key for \fI\*(u:\fR.
.sp .5
Note that keysyms also use the ISO Latin standard (see Chapter 8,
\fIEvents\fR).  Therefore, should you need the code for
any character, you can get it from the 8 least significant bits of
the keysym value for that character, listed in <\fIX11/keysymdef.h\fR>.
.bp
\s-1\f(HITable 7-3. The Color Database\fR\s+1
.sp 
.TS H
linesize(2);
l | l | l | l
l | n | n | n.
.sp 1p
English Words	Green	Blue	Red
.sp 1p
_
.sp 5p
.TH
aquamarine	112	219	147
black	0	0	0
blue	0	0	255
blue violet	159	95	159
brown	165	42	42
cadet blue	95	159	159
coral	255	127	0
cornflower blue	66	66	111
cyan	0	255	255
dark green	47	79	47
dark olive green	79	79	47
dark orchid	153	50	204
dark slate blue	107	35	142
dark slate gray	47	79	79
dark slate grey	47	79	79
dark turquoise	112	147	219
dim gray	84	84	84
dim grey	84	84	84
firebrick	142	35	35
forest green	35	142	35
gold	204	127	50
goldenrod	219	219	112
gray	192	192	192
green	0	255	0
green yellow	147	219	112
grey	192	192	192
indian red	79	47	47
khaki	159	159	95
light blue	191	216	216
light gray	168	168	168
light grey	168	168	168
light steel blue	143	143	188
lime green	50	204	50
magenta	255	0	255
maroon	142	35	107
.TE
.in 3.25i
.rt
.sp 
.TS H
linesize(2);
l | l | l | l
l | n | n | n.
.sp 5p
English Words	Green	Blue	Red
.sp 1p
_
.TH
medium aquamarine	50	204	153
medium blue	50	50	204
medium forest green	107	142	35
medium goldenrod	234	234	173
medium orchid	147	112	219
medium sea green	66	111	66
medium slate blue	127	0	255
medium spring green	127	255	0
medium turquoise	112	219	219
medium violet red	219	112	147
midnight blue	47	47	79
navy	35	35	142
navy blue	35	35	142
orange	204	50	50
orange red	255	0	127
orchid	219	112	219
pale green	143	188	143
pink	188	143	143
plum	234	173	234
red	255	0	0
salmon	111	66	66
sea green	35	142	107
sienna	142	107	35
sky blue	50	153	204
slate blue	0	127	255
spring green	0	255	127
steel blue	35	107	142
tan	219	147	112
thistle	216	191	216
turquoise	173	234	234
violet	79	47	79
violet red	204	50	153
wheat	216	216	191
white	252	252	252
yellow	255	255	0
yellow green	153	204	50
.TE
.in 0
.bp
.sp 2
.IP \s+2\f(HB7.3.3\fR\s-2 6n 
\s+2\f(HBHexadecimal Color Specification\fR\s-2
.sp 
It is also possible to specify colors using a hexadecimal string.
The hexadecimal form of color specification is necessary since
you may want the user to be able to specify an exact color, not just the rough
approximation allowed by an ASCII name.
The hexadecimal specification must be in one of the following formats:
.nf
	\s-1\f(CW#RGB			(4 bits each of red, green, and blue)
	#RRGGBB		(8 bits each of red, green, and blue)
	#RRRGGGBBB	(12 bits each of red, green, and blue)
	#RRRRGGGGBBBB	(16 bits each of red, green, and blue)\fR\s+1
.sp .5
.fi
Each of the letters represents a hexadecimal digit.  For example,
\f(CW#3a7\fP and \f(CW#0003000a0007\fP are equivalent.
.sp .5
\f(CWXParseColor\fP returns a zero status if the color specification begins 
with \f(CW#\fP but does not match this form, or does not begin with \f(CW#\fP and cannot be
found in the color database.
.sp 2
.IP \s+2\f(HB7.3.4\fR\s-2 6n  
\s+2\f(HBCode to Allocate Standard Colors\fR\s-2
.sp
Example 7-2 shows code to allocate a color using a name from the 
ASCII color name database.  The same calls would be used to parse a 
hexadecimal color string.  (Pink could be specified in the call to 
\f(CWXParseColor\fP as \f(CW"#6c8f8f"\fP instead of \f(CW"pink"\fP.)  
Because 
simple allocation of colors should work on all types of color system, the 
program doesn't need to do any checking of the visual class.  Instead, it 
simply needs to take into account the possibility that it might be running on 
a black-and-white system.  It does this by testing the return values of the 
functions used to allocate colors, and if they fail, setting a flag that will cause 
the program to use the \f(CWBlackPixel\fP and \f(CWWhitePixel\fP macros 
instead.
.sp 
\s-1\f(HIExample 7-2.  Allocating a read-only colorcell\fR\s+1
.sp 
.nf
\f(CW#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <stdio.h>

extern Display *display;
extern int screen;
extern unsigned long foreground_pixel, background_pixel, border_pixel;

#define MAX_COLORS 3

get_colors()
{
	int depth;
	Visual *visual;
	static char *name[] = {"Red", "Yellow", "Green"};
	XColor exact_def;
	Colormap cmap;
	int ncolors = MAX_COLORS;
	int colors[MAX_COLORS];
	int i;

	printf("in get colors\n");

	depth  = DisplayPlanes(display, screen);
	visual = DefaultVisual(display, screen);
	cmap   = DefaultColormap(display, screen);
	if (depth == 1)  {  /* one-plane monochrome */
		/* Use BlackPixel and WhitePixel only. 
		 * We must do this first if depth is 1, since
		 * it may allocate colors successfully but 
		 * the colors might all be black */;
		border_pixel = BlackPixel(display, screen);
		background_pixel = WhitePixel(display, screen);
		foreground_pixel = BlackPixel(display, screen);
		}
	else {
		for (i = 0; i < MAX_COLORS; i++) {
		printf("allocating %s\n", name[i]);
			if (!XParseColor (display, cmap, name[i], &exact_def)) {
				fprintf(stderr, "basic: color name %s not in \
						database", name[i]);
				exit(0);
			}
			printf("The RGB values from the database are \
					%d, %d, %d\n", exact_def.red, 
					exact_def.green, exact_def.blue);
   			if (!XAllocColor(display, cmap, &exact_def)) {
				fprintf(stderr, "basic: all colorcells allocated \
						and read/write\n");
				exit(0);
			}
			printf("The RGB values actually allocated are \
					%d, %d, %d\n", exact_def.red, 
					exact_def.green, exact_def.blue);
			colors[i] = exact_def.pixel;
		}

		printf("basic: allocated %d read/write colorcells\n", 
				ncolors);

		border_pixel = colors[0];
		background_pixel = colors[1];
		foreground_pixel = colors[2];
.EL
	}
}\fR
.sp .5
.fi
.sp 2
.IP \s+2\f(HB7.3.5\fR\s-2 6n
\s+2\f(HBSpecifying Explicit RGB Values\fR\s-2
.sp
If you are using read-only colorcells, you should most likely use
\f(CWXParseColor\fP to convert a color name or hexadecimal string to an 
RGB value.  However, it is also possible to specify the desired RGB 
values explicitly.  Simply 
declare an \f(CWXColor\fP structure, and set its \f(CWred\fP, \f(CWgreen\fP 
and \f(CWblue\fP members to the desired RGB values.  Example 7-3 shows
a brief code fragment that demonstrates how to allocate a color 
using explicit RGB values:
.sp 
\s-1\f(HIExample 7-3.  Explicit RGB specification\fR\s+1
.sp 
.nf
	\s-1\f(CWDisplay *display;
	int screen;
	Colormap colormap;
	Window window;
	XColor pink;

	/* Open Display, etc. */

	colormap = \f(CWDefaultColormap\fP (display, screen);

	/* allocate using RGB values */
	pink.red = 45000; pink.green = 65536; pink.blue = 65536;
	if (XAllocColor (display, colormap, &pink) == 0)
		/* handle failure */

	/* Now you can use pink.pixel as the value for a window
	     background or border, or to set the foreground or 
	     background member of a GC  */\fR\s+1
.fi
.sp .5
There are several ways of specifying an explicit RGB value.  In
Example 7-2, we use an integral value.  
Table 7-4 shows four ways to specify pink as explicit individual RGB values.  
.sp  
.ne 10
\s-1\f(HITable 7-4. Four ways to specify pink\fR\s+1
.sp 
.TS
linesize(2);
l | lp9fCW lp9fCW lp9fCW
l | lp9fCW lp9fCW lp9fCW.
.sp 1p
	pink.red	pink.green	pink.blue
.sp 1p
_
.sp 4p
Integer	48128	36608	36608
Shifted Int	(188<<8)	(143<<8)	(143<<8)
Hexadecimal	0x6c00	0x8f00	0x8f00
Shifted Hex	(0x6c<<8)	(0x8f<<8)	(0x8f<<8)
.sp 8p
.TE
.ne 4
As described in Section 7.3.3,
there is also a combined hexadecimal format that can be used to
specify colors on the command line or in the user's resource database 
file.  In this format, pink would be \f(CW#6c8f8f\fP.  
.ne 2
And, as we have said, it is better to use color names
when allocating read-only colorcells than to use any of these 
explicit RGB values.
.sp 2
.IP \s+2\f(HB7.3.6\fR\s-2 6n
\s+2\f(HBChoosing Default Colors\fR\s-2
.sp
Usually a client should allow the user to
specify the preferred colors, either on the command line or in the
resource database or both.  The resource manager (described in Chapter
11, \fIManaging User Preferences\fR) can be used to merge these
preferences with the defaults of the program.
However, the client needs to have reasonable default colors in
case the user doesn't specify any preferences.
.sp .5
Follow these guidelines for your application's default colors:*
.FS 
*Courtesy Oliver Jones, Apollo Computer.
.FE
.RS
.IP \(bu 5
Use ASCII color names for read-only colorcells if possible,
since this maximizes the chance of sharing cells.
.IP \(bu 5
Use colors with large contributions from two or 
all three primary colors--they light the screen more brightly.
.IP \(bu 5
Avoid shades of pure blue--the human eye is relatively insensitive to and
unable to focus on images made of pure blue light.  Mix blue shades with
white (white contains equal parts of all three primary colors).
.IP \(bu 5
Remember that some users are color-blind.  
Don't use the same intensity of green and 
red for "safe" and "danger"--use colors with differing intensity.
.RE
.sp 2
.IP \s+4\f(HB7.4\fR\s-4 6n 
\s+4\f(HBStoring Private Color\fR\s-4
.sp
On a system with a read/write colormap, a client that 
requires exact colors can allocate read/write cells and then store 
colors into them.
.sp .5  
For read/write cells the steps of allocation and
setting of colors are separate, since the colors may be changed at will.
The colors in the allocated colorcells are not 
defined until they are stored.
.sp .5
\f(CWXAllocColorCells\fP at its simplest allows you to
allocate read/write cells so you can change the RGB values dynamically.
You specify as arguments \f(CIncolors\fP and \f(CInplanes\fP
and it allocates
pixel values.
It returns the pixel values in two arrays, \f(CIpixels\fP and
\f(CIplane_masks\fP, which you have to combine with OR in various
permutations in order to calculate all the pixel values.  
To be more precise, each plane mask has one bit for \f(CWGrayScale\fP 
and \f(CWPseudoColor\fP, or three bits for \f(CWDirectColor\fP or
\f(CWTrueColor\fP, and none of the masks have bits in common.
The real
reason for this style of arguments will become clear in Section 7.4.1.
But to simply allocate just a few cells, you set the \f(CIncolors\fP argument to the 
number of colorcells desired, and \f(CInplanes\fP to \f(CW0\fP, and
all the pixel values you need will be returned in the \f(CIpixels\fP array.
The RGB values of the allocated cells are
set with \f(CWXStoreColor\fP, \f(CWXStoreColors\fP or 
\f(CWXStoreNamedColor\fP.
.sp .5
\f(CWXAllocColorPlanes\fP, on the other hand,
is only used when you want to be able to vary a
primary color component of graphics already drawn without
redrawing them.  It allocates read/write cells,
so that a preset number of bits are reserved for each primary color.
Primarily for \f(CWDirectColor\fP, it also
allows you to simulate a small \f(CWDirectColor\fP colormap on a 
\f(CWPseudoColor\fP visual, but uses up colorcells quickly.
It treats the colormap as three separate lookup tables, allocating
\f(CIncolors\fR \f(CW*\fP 
.Su + \f(CW2 \f(CInreds
\fRentries in the red lookup table, 
\f(CIncolors\fR \f(CW*\fP 
.Su + \f(CW2 \f(CIngreens 
\fRentries
in the green lookup table, and
\f(CIncolors\fR \f(CW*\fP 
.Su + \f(CW2 \f(CInblues 
\fRentries
in the blue lookup table.
.sp .5
The following routines are used to actually store colors into 
read/write colorcells once they are allocated:
.RS
.IP "\f(CWXStoreColor\fP"
.br
Changes the read/write colormap cell corresponding to the specified 
pixel value to the hardware color that most closely matches
the RGB values specified.*  The flags \f(CWDoRed\fP, 
\f(CWDoGreen\fP, and \f(CWDoBlue\fP in the \f(CWXColor\fP structure 
indicate which primary colors in the cell are to be changed.
.RE
.FS
*Even when storing explicit RGB values, you may not get the precise color 
you specify.  For example, if the hardware colormap only supports four bits 
of intensity in each primary, and you specify eight-bit values, the server will 
scale the values you provide to the closest possible equivalent on the 
hardware.
.FE
.RS
.IP "\f(CWXStoreColors\fP"
.br
Like \f(CWXStoreColor\fP, except it does multiple cells per call.
Changes the read/write colormap cell corresponding to the specified 
pixel value to the hardware color that most closely matches
the RGB values specified.  The flags \f(CWDoRed\fP, 
\f(CWDoGreen\fP, and \f(CWDoBlue\fP in each \f(CWXColor\fP structure
indicate which primary colors in each cell are to be changed.
.RE
.RS
.IP "\f(CWXStoreNamedColor\fP"
.br
Performs the same function as \f(CWStoreColor\fP,
except that it stores the RGB values associated with
an ASCII color name in the RGB database.  This call would 
be useful for loading a private colormap with each of the default 
named colors.
.RE
.sp 2
.IP \s+2\f(HB7.4.1\fR\s-2 6n  
\s+2\f(HBAllocating Read/Write Colorcells for Overlays\fR\s-2
.sp
\f(CWXAllocColorCells\fP allows you to nondestructively overlay
one set of graphics over another.  The underlying graphics won't
be visible, but they can be refreshed by simply setting or clearing
one or more planes in the drawable.
This technique can improve the performance of a client by reducing
the amount of complicated graphics that have to be redrawn.
It can be useful for highlighting text or graphics for selection.
.sp .5
.ne 7
The trick that allows drawing without destroying what is already
drawn relies on the fact that we can draw in one plane of the
drawable, changing the pixel values and therefore the color, without
changing any another plane.  It is these other planes that contain
the information about the drawing that was already there.
The disadvantage of this approach is that we have to allocate more
colorcells than we would normally need in order to
be able to save the information already there.
.ne 2
Some of the colorcells will need to be loaded with duplicate RGB values.
.sp .5
To illustrate this trick, we're going to draw in one color, with the 
background in a second color, and then draw something temporary over 
the top with a third color.  To do this we need to allocate four 
colorcells with \f(CWXAllocColorCells\fP.  The pixel values allocated
will look something like this:
.sp .5
.DS 3n
\fRColor\v'1p'\h'-\w'Color'u'\l'\w'Color'u\(ul'\v'-1p'                        Important Bits\v'1p'\h'-\w'Important Bits'u'\l'\w'Important Bits'u\(ul'\v'-1p'                                               Other bits\v'1p'\h'-\w'Other bits'u'\l'\w'Other bits'u\(ul'\v'-1p'\f(CW

foreground:    ----0--0------------------------  \fIall other bits don't care\fP

background:    ----0--1------------------------  \fIall other bits don't care\fP

highlighting:  ----1--0------------------------  \fIall other bits don't care\fP

highlighting:  ----1--1------------------------  \fIall other bits don't care\fP
.DE
.sp .5
The bits indicated could have been any bits, but it is significant
that only two bits distinguish the four pixel values.
The first pixel value is used for the foreground, and the second
for the background.
We draw overlays in the third pixel value, that has a \f(CW0\fP
for the bit that distinguishes foreground and background.
Since we don't want to erase what was drawn in the foreground and
background pixel values, we use a plane mask to restrict the drawing
of the highlighting pixel value to a single plane, the one where
bits in the highlighting pixel value are set to 1.
When the entire plane indicated by the 1 in the third and fourth
pixel values is cleared, anything drawn in the third pixel value 
disappears and anything that was drawn in the foreground or background 
will reappear.  The color in the cell indicated by the fourth pixel 
value must be the same as the color of the third for the same color 
to appear regardless of the value of the bit set or not set in the 
foreground and background pixel values.
.sp .5
\f(CWXAllocColorCells\fP doesn't return these four pixel values 
directly.  Instead, it returns the arrays \f(CIpixels\fP and
\f(CIplane_masks\fP that are more convenient
for actually using the overlays than a single array of pixel values.
(Each of these arrays has the number of members that was specified
in the \f(CIncolors\fP and \f(CInplanes\fP arguments.)
Both arrays consist of unsigned long 
values like pixel values.  One array contains the plane masks of 
the overlay planes, and the other contains the pixel values that can 
be used for drawing independent of the overlay planes.
Here are the values in each array when we call \f(CWXAllocColorCells\fP
with \f(CIncolors \f(CW= 2\fR and \f(CInplanes \f(CW= 1\fR.
.sp .5
.DS 3n
\fRArray Members\v'1p'\h'-\w'Array Members'u'\l'\w'Array Members'u\(ul'\v'-1p'                Significant Bits\v'1p'\h'-\w'Significant Bits'u'\l'\w'Significant Bits'u\(ul'\v'-1p'                                             Remaining Bits\v'1p'\h'-\w'Remaining Bits'u'\l'\w'Remaining Bits'u\(ul'\v'-1p'\f(CW

pixels[0]      =  -----0-0------------------------  \fIall other bits don't care\fP
pixels[1]      =  -----0-1------------------------

plane_masks[0] =  -----1--------------------------  \fIall other bits 0\fR
.DE
.sp .5
The two members of the \f(CIpixels\fP array are used for the 
foreground and background. 
The pixel values used for drawing in the highlighting
color are composed by combining with a bitwise OR each 
item in the \f(CIpixels\fP array with each item in the 
\f(CIplane_masks\fP array.  In this case, the highlighting pixel 
value is \f(CW(pixels[0] | plane_mask[0])\fP and the overwriting 
pixel value is \f(CW(pixels[1] | plane_mask[0])\fP.
When highlighting, the plane mask in the GC should
be set to the OR of the members of \f(CIplane_masks\fP used to 
make the highlighting pixel value.  In this simplest case, highlighting
should be done with the plane mask set to \f(CWplane_mask[0]\fP.
.sp .5
We have been hinting at the fact that this overlay technique
can be used with more than two colors and more than one plane.  
\f(CIncolors\fP specifies the number of colors than can be drawn in 
and preserved while drawing in the overlays.  \f(CInplanes\fP 
specifies how many separate overlays you may have, or how many bits 
of color are available in a single overlay.
By ORing together each \f(CIpixels\fP with any combination of
\f(CIplane_masks\fP, a total of \f(CIncolors\fR \f(CW*\fR
.Su + \f(CW2 \f(CInplanes
\fRcolorcells are allocated.  
Each plane mask has one bit for \f(CWGrayScale\fP and \f(CWPseudoColor\fP, 
or three bits for \f(CWDirectColor\fP or
\f(CWTrueColor\fP, and none of the masks have bits in common.
But remember that the chances of success in allocating the colorcells
you request decreases rapidly as you request more colors and planes,
not only because of the number of cells, but because of the special
requirements for the significant bits in each cell.
.sp .5
\f(CWXAllocColorCells\fP takes a \f(CIcontig\fP argument that
specifies whether the planes returned in \f(CIplane_masks\fP
must be contiguous.  The \f(CIcontig\fP argument is normally set to
\f(CWFalse\fP, specifying that the allocated planes need not be contiguous,
because then the chances of success of the \f(CWXAllocColorCells\fP call are greater.
There are more likely to be a number of non-contiguous planes available than the same
number of contiguous planes.  The \f(CIcontig\fP argument may have to be
set to \f(CWTrue\fP for imaging applications that want to be able to perform
mathematical operations on the pixel values.  It is easier to perform operations
by shifting bits with contiguous planes than to acheive the same effect with
random planes. 
.sp .5
Your code should always have a backup plan in case you can't
get the overlay scheme you prefer.  In most cases, the underlying
graphics can be redrawn if the overlays that would preserve
them cannot be allocated.
.sp .5
Example 7-4 demonstrates allocating the read-write cells for a 
single overlay plane.  It allocates two \f(CIpixels\fP and 
one \f(CIplane_masks\fP, or in other words, four read/write colorcells.
The third and fourth are both set to the highlighting color.
If this overlay plan fails, it allocates three colors so that
a highlight can still be implemented even though the underlying
graphics will have to be redrawn.  If the color allocation fails 
completely, it uses black and white, which can be highlighted
using the \f(CWGXxor\fP logical function to invert the color.
.sp 
\s-1\f(HIExample 7-4.  XAllocColorCells allocating read/write colorcells for
overlay plane\fR\s+1
.sp 
.nf
\s-1\f(CW#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <stdio.h>

extern Display *display;
extern int screen;
extern unsigned long foreground, background_pixel;
extern unsigned long overlay_pixel_1, overlay_pixel_2;
extern int overlay_plane;

#define MAX_COLORS 3
#define MAX_PLANES 1
#define CANNOT_OVERLAY 0
#define CAN_OVERLAY 1

int
get_colors()
{
	int depth;
	static char *name[] = {"Red", "Yellow", "Green", "Green"};
	XColor exact_defs[MAX_COLORS];
	Colormap cmap;
	int ncolors = 4;
	int plane_masks[MAX_PLANES];
	int colors[MAX_COLORS];
	int i;

	depth = DisplayPlanes(display, screen);
	cmap   = DefaultColormap(display, screen);
	if (depth == 1)  {  /* one-plane monochrome */
		/* use BlackPixel and WhitePixel only */;
		background_pixel = WhitePixel(display, screen);
		foreground = BlackPixel(display, screen);
		printf("using black and white\n");
		return(CANNOT_OVERLAY);
	}
	else {
		/* allocate our colorcells */
   		if (XAllocColorCells (display, cmap, False, plane_masks, 1, 
				colors, 2) == 0) {
			/* Can't get enough read/write cells to overlay.
			 * Try at least to get three colors. */
   			if (XAllocColorCells (display, cmap, False, plane_masks, 
					0, colors, 3) == 0) {
				/* Can't even get that.  Give up and
				 * use black and white */
				background_pixel = WhitePixel(display, screen);
				foreground = BlackPixel(display, screen);
				printf("using black and white\n");
			}
			
			ncolors = 3;
			printf("got only three colors\n");
		}
      		
		/* allocated colorcells successfully,
		 * now set their colors - three and four
		 * are set to the same RGB values */
		for (i = 0; i < ncolors; i++)
		{
			if (!XParseColor (display, cmap, name[i], 
					&exact_defs[i])) {
				fprintf(stderr, "basic: color name %s not \
						in database", name[i]);
				exit(0);
			}

			/* set pixel value in struct to the allocated one */
			exact_defs[i].pixel = colors[i];
		}
		printf("set RGB values\n");

		/* this sets the color of read/write cell */
		XStoreColors (display, cmap, exact_defs, ncolors);
		printf("stored colors\n");
		background_pixel = colors[0];
		foreground = colors[1];
		if (ncolors == 4) {
			overlay_pixel_1 = colors[0] | plane_masks[0];
			overlay_pixel_2 = colors[1] | plane_masks[0];
			overlay_plane = plane_masks[0];
			return(CAN_OVERLAY);
		}




		else {
			overlay_pixel_1 = colors[2];
			return(CANNOT_OVERLAY);
		}
	}
}\fR\s+1
.fi
.sp 2
.IP \s+2\f(HB7.4.2\fR\s-2 6n  
\s+2\f(HBUsing XAllocColorPlanes\fR\s-2
.sp 
\f(CWXAllocColorPlanes\fP is used when you want to be able
to change the amount of a primary color in graphics 
without having to redraw them.  In other words, perhaps you
are looking at an image and would like to increase the redness of it.
The best way to do this is to increase the amount
of red in every pixel value.  \f(CWXAllocColorPlanes\fP would be the 
way to allocate colors to allow this.
It is rarely used except in imaging applications and 3-D graphics,
and will rarely work except on 24-plane workstations.
.sp .5
Note that for applications like a paint mixing program, in which you
have three bars for the three primary colors and a palette that shows the mixed
color, you would not use \f(CWXAllocColorPlanes\fP.  
The correct way to implement this is to allocate a single
read/write color for the palette and to change it dynamically.
(The primary colors should be allocated using read-only colors.)
.sp .5
The next piece of code, Example 7-5, is similar to 
Example 7-4, but it uses \f(CWXAllocColorPlanes\fP.  
It is somewhat sketchy because real applications that use 
\f(CWXAllocColorPlanes\fP are complicated.
.sp .5
After allocating colors with \f(CWXAllocColorPlanes\fP,
you can then use \f(CWXStoreColors\fP to set the 
colors (the other calls for setting colors set only one cell at a 
time).  When \f(CInred\fP, \f(CIngreen\fP, and \f(CInblue\fP
are each \f(CW8\fP, only one call to \f(CWXAllocColorPlanes\fP and 
one call to \f(CWXStoreColors\fP are necessary to allocate and set 
all 16 million colors of an entire 24-plane colormap.
.sp 
\s-1\f(HIExample 7-5.  Using XAllocColorPlanes to allocate colorcells for
DirectColor\fR\s+1
.sp 
.nf
	\s-1\f(CW#define PIXELS 256
	Display *display;
	int screen;
	int contig = False;		/* non-contiguous planes */
	unsigned long pixels[PIXELS];	/* return of pixel values */

	/* number of independent pixel values allocate */
	unsigned int ncolors = PIXELS;	

	XColor defs[2048];	/* PIXELS * 2^maxplanes where maxplanes
		 * is the largest of nred, ngreen, and nblue */

	/* number of planes to allocate for each primary */
	unsigned int nreds = 3, ngreens = 3, nblues = 2;	

	/* returned masks, which bits of pixel value for each primary */
	unsigned long \f(CWred_mask\fP, \f(CWgreen_mask\fP, \f(CWblue_mask\fP;	

	Colormap colormap;
	Status status;

	/* open display, etc. */
	/* get or create large \f(CWDirectColor\fP colormap */

	while (status = \f(CWXAllocColorPlanes\fP(display, colormap, 
			contig, pixels, ncolors, nreds, ngreens, nblues, 
			&red_mask, &green_mask, &blue_mask) == 0) {
		{
		/* Make contig False if it was True,
		 * reduce value of ncolors,
		 * reduce value of nreds, ngreens and/or nblues,
		 * or try allocating new map,
		 * break when you give up */
		break;
		}
	if (status == 0)
		{
		fprintf(stderr, "%s: couldn't allocate requested colorcells", 
				argv[0]);
		exit(-1);
		}

	/* define desired colors in defs */

	while (status = XStoreColors(display, colormap, defs, 
			ncolors) == 0)
		{
		fprintf(stderr, "%s: can't store colors", argv[0]);
		/* try to fix problem here, exit or break */
		exit(-1);
		}
		
	/* draw your shaded stuff! */\fR\s+1
.fi
.sp 2
.IP \s+2\f(HB7.4.3\fR\s-2 6n  
\s+2\f(HBHighlighting in Monochrome\fR\s-2
.sp
It is easy to highlight monochrome graphics.
The simplest way is to set the GC to the \f(CWGXxor\fP logical function,
and draw your graphics once to draw them, and again to undraw them.
You must grab the server between the drawing and undrawing so that
no other client change the same pixels in between.
This can be made slightly more efficient on some servers by
setting the foreground used in drawing to the exclusive OR of 
\f(CWBlackPixel\fP and \f(CWWhitePixel\fP, or by setting the logical 
function to \f(CWGXinvert\fP and using a plane mask which is the 
exclusive OR of \f(CWWhitePixel\fP and \f(CWBlackPixel\fP.
.sp .5
On a color screen, these techniques will draw in
pixel values you didn't allocate, and therefore the color may not
necessarily contrast against the background.
A more controlled approach is called for.
Probably it is best to allocate a read-only color to draw in,
or highlight using overlays as described in Section 7.4.1.
.sp 2
.IP \s+4\f(HB7.5\fR\s-4  6n 
\s+4\f(HBCreating and Installing Colormap\fR\s-4
.sp
In discussing colormaps earlier in this chapter, we have sidestepped the 
distinction between \fIhardware colormaps\fP and \fIvirtual colormaps\fP.
.sp .5
A hardware colormap is a physical register from which the display hardware 
reads the RGB intensity values that generate the colors on the screen.  
Most workstations have only one hardware colormap, in which case
all windows on the screen are interpreted using the same colormap.  Some 
high-performance workstations have multiple hardware colormaps, in which case 
separate windows may have their own independent colormaps.
.sp .5
If the hardware colormap cannot be changed, it is termed \fIimmutable\fR.  
The \f(CWStaticColor\fP, \f(CWTrueColor\fP, and \f(CWStaticGray\fP 
visuals are the only visuals that can possibly work on systems
that have immutable hardware colormaps.  In immutable colormaps,
no client can allocate private colorcells
and all RGB values are preset.
.sp .5
Monochrome systems normally have an immutable colormap, since it
does little good to swap the two entries or make them both black or
white.  Immutable colormaps are uncommon on color systems,
but they might be present on some low-cost workstations.
.sp .5
.ne 3
On most color workstations, you can write new
values into the hardware colormap or colormaps to change that mapping.
These hardware colormaps are termed \fIchangeable\fR. 
The \f(CWPseudoColor\fP, 
\f(CWDirectColor\fP, and \f(CWGrayScale\fP visuals have changeable 
colormaps.
.sp .5
If you can write into the hardware colormap, you can also copy
an entire separate colormap into the hardware colormap.
X manages multiple colormaps by keeping a copy of each colormap
called a virtual colormap.
X allows any virtual colormap to be modified 
by clients if the colormap has
a visual that indicates it is writable.  X copies
these colormaps into the hardware colormap as needed 
and as instructed by the window manager or applications.  
.sp .5
.Nd 6
On the most common 
color workstations, with 4 to 8 planes, it is quite easy for
clients that require precise colors to allocate all 
the available colorcells.
Virtual colormaps are one response to the realization that
the color needs of all the clients cannot always be 
satisfied.  When a client can't get the colorcells it
.ne 2
needs from the installed colormap, it can create a new one.
.sp .5
Note, however, that
hardware colormaps are a limited resource that must be shared by
all the clients using a server.  Particularly when there
is only one hardware colormap, this resource must be carefully
managed.  
.sp .5
When a virtual colormap is installed and there is only one
hardware colormap, any colormap that is currently 
installed in the hardware will be uninstalled (made virtual again),
and the new colormap will be installed into the hardware.
When the new colormap is installed,
all the clients that used the old colormap will be displayed in
false colors, since the pixel values in their 
windows will be interpreted according to the new colormap.
This is true assuming that only one hardware colormap is allowed (on all
but high-performance workstations).
.sp .5
.Nd 5
The window manager should ideally have
the responsibility to mediate the desires of 
all the different clients displaying
on a screen that want to modify the colormap. 
Unfortunately, the initial crop
of window managers (including \fIuwm\fP) don't do this, leading to an
unfortunate catch-22 for current applications.*
.FS
*According to the current conventions for interclient communication,
it is the window manager's responsibility to install and uninstall 
(at times of its choosing)
the colormaps specified in the window attributes of the top-level windows.
The colormaps specified in the window attributes of subwindows must be
installed by the application itself, as described in the section 
regarding colormaps in Appendix F.
Unfortunately, most current window managers don't install colormaps yet.  
While this is another incentive to use the default colormap that is
usually installed anyway, your application may need to install its
own colormap until better window managers become available.
.FE
.sp .5
By now you should be getting the idea that it is much better to 
arrange to share the default colormap with the other applications 
than to try to create one of your own.  
The only time when you should really need to create
a special colormap is when you are doing smooth shading, or similar 
applications that need many, strangely distributed colors.  But if 
you are doing that, you will probably be resigned to the fact that 
it will only run on a high-performance workstation.
Since those often allow
multiple hardware colormaps, you can create your own colormap and
you may be able to install it without affecting other applications.
.sp 2
.ne 7
.IP \s+2\f(HB7.5.1\fR\s-2 6n  
\s+2\f(HBFunctions for Manipulating Colormaps\fR\s-2
.sp
The following functions should be used primarily by window 
managers, and by applications
only if they need a special purpose colormap.
.sp .5
.RS
.IP "\f(CWXCreateColormap\fP" 10n
.br
Creates a virtual colormap resource, either with no 
allocated entries, or with all 
allocated writable, that matches the passed visual.
If no entries are allocated, they can be allocated either
as read/write or as read-only cells.  If all entries are allocated
writable, the colormap is completely private, does not need to be
allocated, but just needs its colors set with \f(CWXStoreColors\fP.
.br
.ne 4
.IP "\f(CWXInstallColormap\fP"
.br
A window manager function to install a colormap.
Any window using that colormap ID as an 
attribute receives a \f(CWColormapNotify\fP event.
Only the default colormap is initially installed.
.IP "\f(CWXUninstallColormap\fP
.br
Removes a virtual colormap from the set of installed hardware colormaps.
Sends \f(CWColormapNotify\fP event
to windows that are using the specified map.
.IP "\f(CWXFreeColormap\fP
.br
Uninstalls the specified virtual colormap and frees the resources
associated with the colormap.
Sends a \f(CWColormapNotify\fP event to 
any windows that were using the colormap.
.IP "\f(CWXListInstalledColormaps\fP
.br
Lists the installed colormaps.
.IP "\f(CWXCopyColormapAndFree\fP
.br
Moves all the client's existing colormap entries
to a new colormap and frees those entries of the old colormap.  This is
used when read/write colorcell allocation fails, either
because all the cells in the old colormap are already in use, or because
the number of planes requested were not available.
.IP "\f(CWXSetStandardColormap\fP
.br
Creates a standard
colormap, one of \f(CWXA_RGB_DEFAULT_MAP\fP, \f(CWXA_RGB_BEST_MAP\fP, 
\f(CWXA_RGB_RED_MAP\fP,
\f(CWXA_RGB_GREEN_MAP\fP,
\f(CWXA_\fP \f(CWRGB_BLUE_MAP\fR,
or \f(CWXA_RGB_GRAY_MAP\fP.   (See Section 7.6 for
details.)
.IP \f(CWXGetStandardColormap\fP
.br
Gets a structure containing information about the
standard colormap specified by a
atom.  Once this is done, you can find out
what pixel values are represented by various RGB values.
This call does not install the colormap.
.IP "\f(CWXSetWindowColormap\fP
.br
Sets the colormap attribute of a window.
.RE
.sp 2
.IP \s+2\f(HB7.5.2\fR\s-2 6n  
\s+2\f(HBThe ColormapNotify Event\fR\s-2\fR
.sp
\f(CWColormapNotify\fP events herald changes relating to the colormap 
specified in the colormap attribute for a particular 
window, or changes to the attribute itself.
.sp .5
If your application might be adversely affected by changes
to the colormap, you should watch for these events and act
accordingly.  To receive \f(CWColormapNotify\fP events, pass
\f(CWColormapChangeMask\fP to \f(CWXSelectInput\fP.
Example 7-6 shows the structure for the \f(CWXColormapEvent\fP.
.sp 
.Nd 12
\s-1\f(HIExample 7-6.  The ColormapEvent structure\fR\s+1
.sp 
.nf
.ta 4.5n 1.85i
\f(CWtypedef struct {
	int type;
	unsigned long serial;   /* # of last request processed by server */
	Bool send_event;	/* true if this came from SendEvent request */
	Display *display;	/* display the event was read from */
	Window window;
	Colormap colormap;	/* Colormap or None */
	Bool new;
	int state;	/* ColormapInstalled, ColormapUninstalled */
} XColormapEvent;\fR
.fi
.sp .5
Here is a brief explanation of each member of the \f(CWXColormapEvent\fP structure:
.RS
.IP "\f(CWwindow\fP" 12n
The window this event was selected for, whose colormap attribute
or colormap was changed.
.IP "\f(CWcolormap\fP"
The colormap associated with the window,
either a colormap ID or the constant \f(CWNone\fP.
It will be \f(CWNone\fP only if this event was in response 
to an \f(CWXFreeColormap\fP call.
.IP "\f(CWnew\fP"
\f(CWTrue\fP when the colormap attribute has been changed, or
\f(CWFalse\fP when the colormap is installed or uninstalled.
.IP "\f(CWstate\fP"
Either \f(CWColormapInstalled\fP or \f(CWColormapUninstalled\fP;
it indicates whether the colormap is installed or uninstalled.
.RE
.sp .5
\f(CWXFreeColormap\fR, \f(CWXInstallColormap\fR, and 
\f(CWXUninstallColormap\fR generate this event for windows 
that have their colormap attribute set to the colormap that
was affected.  From the information in the structure, you can tell 
which of these calls generated the event, and what the current status
of the colormap is.  The conventions for what the applications should do
in response to each of these contingencies has not yet been determined.
See Chapter 10, \fIInterclient Communication\fR and Appendix F, \fIProposed
Interclient Communications\fR for additional details.
.sp 2
.IP \s+2\f(HB7.5.3\fR\s-2 6n  
\s+2\f(HBThe Required Colormap List\fR\s-2
.sp
The X Protocol specifies that each server can specify a required list
of colormaps, which affects what happens when other colormaps are
installed or uninstalled.  Here is what the Protocol specification
says about the required list (translated into Xlib terms):
.sp 
.nr LL 5.5i
.in .25i
At any time, there is a subset of the installed maps, viewed as an
ordered list, called the required list.  The length of the required
list is at most \f(CWmin_maps\fP, where \f(CWmin_maps\fP is a member
of the \f(CWDisplay\fP structure.  The required list is maintained as
follows.  When a colormap is an explicit argument to \f(CWXInstallColormap\fP,
it is added to the head of the list, and the list is truncated at the
tail if necessary to keep the length of the list to at most \f(CWmin_maps\fP.  When a
colormap is an explicit argument to \f(CWXUninstallColormap\fP and it is in the
required list, it is removed from the list.  A colormap is not added to
the required list when it is installed implicitly by the server, and
the server cannot implicitly uninstall a colormap that is in the
required list.
.in -.25i
.nr LL 5.5i
.sp 
In less precise words, the \f(CWmin_maps\fP most recently installed maps
are guaranteed to be installed.  This number will often be one; clients
needing multiple colormaps should beware.
.sp
.IP \s+4\f(HB7.6\fR\s-4 6n
\s+4\f(HBThe Standard Colormap\fR\s-4
.sp
X defines a set of standard colormap properties that refer to a set of 
commonly-used colormaps.  Their purpose is to increase the sharing of
color resources between applications.
At this point, however, few if any servers or window 
managers create the standard colormaps, so it is unclear if or when
standard colormaps will begin to play a major part in the design of 
color handling in applications.
Applications that create their own colormaps should create standard
colormaps if one exists that fit their needs.  Then it is possible
that other applications creating the same standard colormap could
cooperate without the help of a window manager.
If your application does not create or use a custom colormap,
you can skip this section if pressed for time.
.sp .5
Even though they don't exist as predefined virtual colormaps, standard 
colormaps are useful, because applications that share these colormap 
conventions display true colors more often, especially on displays with
severely limited hardware colormaps. 
.sp .5 
Applications can also use the knowledge about
the structure of a standard colormap to optimize the process of figuring
out which existing pixel values correspond to required colors, and
which colors must be allocated and set from scratch.
One of the standard colormaps is the default colormap.
.sp 2
.IP \s+2\f(HB7.6.1\fR\s-2 6n  
\s+2\f(HBThe Standard Colormap Properties\fR\s-2
.sp
This section describes the standard colormap properties.
These colormaps may have
been created by the server, or they may have to be created by the
application.  Either way, the application can get information about them
by calling \f(CWXGetStandardColormap\fP.  You specify an
atom like \f(CWXA_RGB_BEST_MAP\fP and this function returns
an \f(CWXStandardColormap\fP structure.
.sp .5
The following list names the atoms and describes the colormap
associated with each one.
.sp .5
.RS
.IP \f(CWXA_RGB_DEFAULT_MAP\fP 1i
.br
This property defines part of the system default colormap,
as described in Section 7.2.7.  
These colorcells are normally read-only and shareable.
A typical allocation of the \f(CWXA_RGB_DEFAULT_MAP\fP on 8-plane displays
is all the colors produced from any combination 
of 6 reds, 6 greens, and 6 blues.  This gives 216 uniformly distributed
colors, and leaves 40 for other programs or for special-purpose colors 
for text, borders, and so on.
A typical allocation for the \f(CWXA_RGB_DEFAULT_MAP\fP on 24-plane 
displays
is 64 reds, 64 greens, and 64 blues.
This gives about one million uniformly distributed colors 
(64 intensities of 4096 different hues) and
leaves lots of colorcells available for other purposes.
.IP \f(CWXA_RGB_BEST_MAP\fP 1i
.br
This property defines the "best" RGB colormap available on
the display.  Of course, this is a subjective evaluation.
Many image-processing and 3-D programs need to
use all available colormap cells and to distribute as many
perceptually distinct colors as possible over those cells.
In this case there may be more green values available than
red, and more green or red than blue.
.sp .5
On an 8-plane \f(CWDirectColor\fP visual, 
\f(CWXA_RGB_BEST_MAP\fP is a 3/3/2 allocation.  
On a 24-plane \f(CWDirectColor\fP visual, 
\f(CWXA_RGB_BEST_MAP\fP is an 8/8/8 allocation.  
On other displays, 
\f(CWXA_RGB_BEST_MAP\fP is purely up to the implementor of the display.
.IP \f(CWXA_RGB_RED_MAP\fP 1i
.br
.ns
.IP \f(CWXA_RGB_GREEN_MAP\fP 1i
.br
.ns
.IP \f(CWXA_RGB_BLUE_MAP\fR 1i
.br
These properties define all-red, all-green, and all-blue
colormaps, respectively.  These maps are used by programs 
that make color-separated images.  
For example, a user might generate a full-color image 
on an 8-plane display by rendering an image
once with high color resolution in red, once with green, 
and once with blue, and exposing a single frame in a camera with
three images.
.IP \f(CWXA_RGB_GRAY_MAP\fP 1i
.br
This property describes the "best" gray-scale colormap
available on the display.  As previously mentioned, 
only the colormap, \f(CWred_max\fP, \f(CWred_mult\fP, and 
\f(CWbase_pixel\fP fields of the
\f(CWXStandardColormap\fP structure are used for gray-scale colormaps.
.RE
.sp 2
.IP \s+2\f(HB7.6.2\fR\s-2 6n  
\s+2\f(HBThe XStandardColormap Structure\fR\s-2
.sp
.ne 4
An application that wants to use a standard colormap must get the
structure that contains the specification for the colormap
using \f(CWXGetStandardColormap\fP.
Some servers, particularly on high-performance workstations, create
some or all of the standard colormaps when they initialize.
If the desired colormap has already been created, it is returned in
the \f(CWcolormap\fP member of the \f(CWXStandardColormap\fP 
structure 
shown in Example 7-7.  If the colormap does not yet exist,
the \f(CWcolormap\fP member will be \f(CW0\fP.
In that case, the application can create a colormap and allocate
entries to match the specification in the remaining
members of \f(CWXStandardColormap\fP.
.sp 
.Nd 8
\s-1\f(HIExample 7-7.  The XStandardColormap structure\fR\s+1
.sp 
.nf
\s-1\f(CWtypedef struct _XStandardColormap {
	Colormap colormap;
	unsigned long \f(CWred_max\fP, \f(CWgreen_max\fP, \f(CWblue_max\fP;
	unsigned long \f(CWred_mult\fP, \f(CWgreen_mult\fP, \f(CWblue_mult\fP;
	unsigned long \f(CWbase_pixel\fP;
} \f(CWXStandardColormap\fP;\fR\s+1
.fi
.sp .5
The members of the \f(CWXStandardColormap\fP structure are
.ne 4
as follows:
.RS
.IP \(bu 5 .4v
The \f(CWcolormap\fR member is the ID of a colormap created by the
\f(CWXCreateColormap\fP function or the default colormap.  This ID
can be used to install a virtual colormap into the hardware colormap.
.IP \(bu 5 .4v
The \f(CWred_max\fP, \f(CWgreen_max\fP, and \f(CWblue_max\fP fields 
give the maximum
red, green, and blue values, respectively.  
A typical allocation that provides 6 * 6 * 6 = 216 read-only, shareable colors 
in a \f(CWPseudoColor\fP colormap on
a standard 8-plane workstation is \f(CWred_max\fP = 5, 
\f(CWgreen_max\fP = 5, and \f(CWblue_max\fP = 5.  This 
leaves 40 cells available for special colors
and private, non-shareable purposes.
On a 24-plane workstation, there would be 8 bits 
available for each color in a \f(CWTrueColor\fP visual,
which would allow 256 shades of each primary color.
In this case, \f(CWred_max\fP = 255, \f(CWgreen_max\fP = 
255, and \f(CWblue_max\fP = 255.
This map would include 256 x 256 x 256 = 16.38 million total colors.
.IP \(bu 5 .4v
The \f(CWred_mult\fP, \f(CWgreen_mult\fP, and \f(CWblue_mult\fP fields 
scale each pixel subfield
into the proper range in
the 16-bit RGB value in the colorcell with the range 0 to 65535.
The red pixel subfield is moved \f(CWred_mult\fP bits
toward the most significant bit of the pixel value.
.sp .5
For a 3/3/2 \f(CWDirectColor\fP allocation (8 reds, 8 greens, 
4 blues), \f(CWred_mult\fP might be 32,
\f(CWgreen_mult\fP might be 4, and \f(CWblue_mult\fP might be 1.
These effectively move the red value into the 
most significant bits of the RGB value in the colorcell, the 
green into the middle, and the blue into the least significant bits.
.ne 4
This arrangement is arbitrary but useful.
For a 6-colors-each allocation, which must be \f(CWPseudoColor\fP since
the planes can't be evenly allocated to separate primaries,
\f(CWred_mult\fP might be 36, 
\f(CWgreen_mult\fP might be 6, and \f(CWblue_mult\fP might be 1.
.IP \(bu 5 .4v
The \f(CWbase_pixel\fP field gives the base value
that is added to the pixel value calculated 
from the RGB values and scale factors.
Usually, the \f(CWbase_pixel\fP is obtained from a call to the 
\f(CWXAllocColorPlanes\fP function.  
.RE
.sp 2
.IP \s+2\f(HB7.6.3\fR\s-2 6n  
\s+2\f(HBThe 3/3/2 Standard Colormap\fR\s-2
.sp
Now let's look at a typical standard colormap.
The following example describes the 3/3/2 \f(CWDirectColor\fP
standard colormap used on eight-plane displays.
Three planes are used for
red, three planes for green, and two planes for blue.
This 3/3/2 allocation allows values in the range of:
.sp .5
.nf
        \f(CWred    0-7\fR    thus   \f(CWred_max\fP      =  \f(CW7\fP  
        \f(CWgreen  0-7\fR             \f(CWgreen_max\fP =  \f(CW7\fP  
        \f(CWblue   0-3\fR             \f(CWblue_max\fP   =  \f(CW3\fP  
.fi
.sp .5
To obtain the pixel value, these RGB values must be shifted to their
corresponding planes.  If the red value is contained in the three most
significant planes or bits, the green values in the three next
most significant planes or bits, and the blue value in the two least
significant planes or bits, then the pixel can be constructed as
follows.
.sp .5
.nf
.ft CW

        7  6  5  4  3  2  1  0
        _  _  _  _  _  _  _  _

        _______  _______  ____
          red     green   blue


        _  _  _<----------------   \f(CWred_mult\fP   = 2!"" sup 5! = 32
        red

                 _  _  _<-------   \f(CWgreen_mult\fP = 2!"" sup 2! =  4 green

                          _  _<    \f(CWblue_mult\fP  = 2!"" sup 0! =  1
.ft R
.fi
.sp .5
In a \f(CWDirectColor\fP system like this, the multiples are
equal to 2 sup \fIn\fR#, where \fIn\fR is their lowest plane or bit position.
If the red, green, and blue were stored in a different
order, the multiples would not be 32, 4, 1, but would still be
calculated from the above description and formula.  The
3/3/2 standard colormap allocation is fairly standard.
.sp 2
.IP \s+2\f(HB7.6.4\fR\s-2 6n  
\s+2\f(HBInstalling a Standard Colormap\fR\s-2
.sp
Example 7-8 installs the \f(CWXA_RGB_BEST_MAP\fP standard colormap.  This
example assumes a \f(CWPseudoColor\fP visual, but you should check the 
visual class of the root window to be sure.
.sp 
\s-1\f(HIExample 7-8.  Code to install and use XA_RGB_BEST_MAP\fR\s+1
.sp 
.nf
	\s-1\f(CWDisplay *display;
	int screen;
	XStandardColormap \f(CWbest_map_info\fP;	   /* structure to fill */
	unsigned long whitepixel;	   /* computed pixel value for white */
	Colormap colormap;
	Window window;
	Status status;
	XSetWindowAttributes attrib;	   /* so we can set colormap */
	unsigned long attribmask;

	/* Open Display, etc. */

	if (status = XGetStandardColormap(display, RootWindow(display, 
			screen), &best_map_info, XA_RGB_BEST_MAP) == 0);
		printf("%s: can't get standard colormap", argv[0]);

	attrib.colormap = \f(CWbest_map_info\fP.colormap;

	whitepixel = \f(CWbest_map_info\fP.base_pixel  +
			(best_map_info.red_max * \f(CWbest_map_info\fP.red_mult) +
			(best_map_info.green_max * \f(CWbest_map_info\fP.green_mult) +
			(best_map_info.blue_max * \f(CWbest_map_info\fP.blue_mult);

	attrib.background_pixel = whitepixel;

	attribmask = CWBackPixel | CWColormap;

	XChangeWindowAttributes(display, window, attribmask, &attrib);\fR\s+1
.fi
.sp 2
.IP \s+2\f(HB7.6.5\fR\s-2 6n  
\s+2\f(HBExamples of RGB to Pixel Conversion\fR\s-2
.sp
The standard colormaps such as \f(CWXA_RGB_BEST_MAP\fP
are useful when you want to calculate pixel values from RGB values.  
.sp .5
Consider a 3-D display program that draws a smoothly shaded sphere.  
At each pixel in the image of the sphere, 
the program computes the intensity and color of light
reflected to the viewer.  
The result of each computation is a triple of red, green, and blue
coefficients in the range 0.0 to 1.0.  
To draw the sphere, the program needs a colormap that provides a
large range of uniformly distributed colors.  
The colormap must be arranged so that the program can
convert its RGB triples into pixel values very quickly,
because drawing the entire sphere will require many such
conversions.  An example of one such calculation is shown in Example 
7-9.
.sp 
\s-1\f(HIExample 7-9.  Calculating pixel values from floating point RGB 
values\fR\s+1
.sp
.nf
   \s-1\f(CWXStandardColormap \f(CWbest_map_info\fP;
   float red, green, blue;
   unsigned long pixelvalue;
   int status;

   status = XGetStandardColormap(display, RootWindow(display,
      screen), &best_map_info, XA_RGB_BEST_MAP);

   if (!status)
      {
      printf("%s: specified standard colormap not available", argv[0]);
      exit(-1);
      }

   pixelvalue = \f(CWbest_map_info\fP.base_pixel +
      ((unsigned long)(0.5 + (red * \f(CWbest_map_info\fP.red_max)) *
           \f(CWbest_map_info\fP.red_mult) + 
      ((unsigned long)(0.5 + (green * \f(CWbest_map_info\fP.green_max)) * 
           \f(CWbest_map_info\fP.green_mult) +
      ((unsigned long)(0.5 + (blue * \f(CWbest_map_info\fP.blue_max)) *
           \f(CWbest_map_info\fP.blue_mult);\fR\s+1
.fi
.sp .5
Example 7-9 demonstrated how to
translate colors represented by real numbers between
zero and one.  The next example, Example 7-10, demonstrates how 
to do it for integer primary colors.
.sp 
\s-1\f(HIExample 7-10.  Calculating pixel values from integer RGB values\fR\s+1
.sp 
.nf
	\s-1\f(CWXStandardColormap \f(CWbest_map_info\fP;
	unsigned long red, green, blue;
	unsigned long pixelvalue;
	int status;

	status = \f(CWXGetStandardColormap\fP(display, \f(CWRootWindow\fP(display, 
		screen), &best_map_info, XA_RGB_BEST_MAP);

	if (!status)
		{
		printf("%s: specified standard colormap not available", 
				argv[0]);
		exit(-1);
		}

	pixelvalue = \f(CWbest_map_info\fP.base_pixel +
			(red * \f(CWbest_map_info\fP.red_mult) +
			(green * \f(CWbest_map_info\fP.green_mult) +
			(blue * \f(CWbest_map_info\fP.blue_mult);\fR\s+1
.fi
.sp .5
For gray scale colormaps, only the \f(CWcolormap\fP, \f(CWred_max\fP, 
\f(CWred_mult\fP, 
and \f(CWbase_pixel\fP fields of the \f(CWXStandardColormap\fP structure 
are defined.  The other fields are ignored.  
Pixel values for a \f(CWStaticGray\fP or \f(CWGrayScale\fP visual 
must be in the range \f(CWbase_pixel < \f(CWpixel_value\fP < \f(CW(red_max * red_mult) + base_pixel\fP\fR.
To compute a gray pixel value, use the
following expression:
.nf
.in 5n
\f(CWpixel_value\fP = gray * \f(CWred_mult\fP + \f(CWbase_pixel\fP
.in
.sp .5
.fi
where:
.sp .5
.in 5n
\f(CWgray\fP = the gray value you desire (0 to \f(CWred_max\fP).
.sp .5
\f(CWred_mult\fP = value from \f(CWXStandardColormap\fP structure
.sp .5
\f(CWbase_pixel\fP = the offset in the colormap.
.sp 2
.IP \s+4\f(HB7.7\fR\s-4 6n
\s+4\f(HBWriting Portable Color Program\fR\s-4
.sp
As we've shown in previous sections, basic color allocation and storage is 
fairly simple.  Even virtual colormaps are fairly easy to create and install.
But because X supports so many different types of 
equipment, you must take special care to make sure that your program will 
run on both black-and-white and color displays, and that it makes intelligent 
decisions about whether or not it is possible for it to run on a particular type 
of color display.
.sp .5
Unless your application has special color needs, tailor 
it to run on any type of screen.  The way to do that is 
to determine the characteristics of the default visual, and 
modify your application's color allocation accordingly.
On all but high-performance
workstations, the macros \f(CWDisplayCells\fP, \f(CWDisplayPlanes\fP, 
\f(CWDefaultVisual\fP, \f(CWDefaultColormap\fP,
and \f(CWDefaultDepth\fP provide you with all the information you need.
(For more information on these macros, see Volume Two, \fIXlib Reference 
Manual\fP.)
.sp .5
.ne 4
An application with special color needs
can determine all the types of visuals that are available
on a screen, and then choose the best one.  
\f(CWXMatchVisualInfo\fP and \f(CWXGetVisualInfo\fP 
are used to get this information and select 
one visual from those available.  How to use these routines is described in 
Section 7.7.3.
Some high-performance displays support more than one visual, 
and in fact allow the applications to choose the visual and 
depth on a window-by-window basis.  
.sp .5
The default visual is returned by the \f(CWDefaultVisual\fP macro.
On color workstations with four to eight planes, this is 
usually a \f(CWPseudoColor\fP visual, associated with
the default colormap (whose ID is returned by 
\f(CWDefaultColormap\fP), containing
read-only cells corresponding to the colors in the 
RGB database, plus a few unallocated read/write cells.
The initial contents of the default colormap are server-dependent.
Most programs without special color requirements can use 
this colormap and visual without further ado.
.sp .5
A window is created with a depth, window 
class, and visual class that can't
be changed during the lifetime of the window.  If 
\f(CWXCreateSimpleWindow\fP
was used to create the window,  all of these are inherited from the parent.
.sp 2
.IP \s+2\f(HB7.7.1\fR\s-2 6n  
\s+2\f(HBMonochrome vs. Color\fR\s-2
.sp
A simple color client may work
on a monochrome workstation, simply by virtue of the 
fact that \f(CWXAllocColor\fP returns the nearest 
realizable color to the one you ask for,
which on single-plane workstations will always either be
black or white.  A problem arises if the closest colors to those 
specified are both black or both white when they are supposed to 
contrast.  You can avoid this by checking
the returned pixel values to make sure they are not the same--but it is 
probably better to code defensively, and test explicitly the number of
planes on the screen to distinguish black and white 
from color.
.sp .5
Example 7-2 takes this defensive approach to supporting color or
monochrome.  Example 7-11 below uses a more concise
approach.  It tests that the number of colorcells 
is greater than 2, and if not, it sets the appropriate black 
and white values on the spot.  It also defaults to black and white if
either of the routines to parse or allocate colors fail.
I had to hit a C reference manual to understand this one, not having
seen the ? or : operators before.  If the returned value of \f(CWiscolor\fP,
\f(CWXParseColor\fP, and \f(CWXAllocColor\fP are \f(CW1\fP, \f(CWgc.foreground\fP is set
to \f(CWblue.pixel\fP.  If any of them are \f(CW0\fP, 
\f(CWgc.foreground\fP is set to \f(CWBlackPixel\fP.
.nf
.sp 
\s-1\f(HIExample 7-11.  A code fragment to use color or monochrome as
appropriate\fR\s+1
\f(CWBool iscolor;
XColor blue;

iscolor = (XDisplayCells (display, screen) > 2);
gc.foreground = (iscolor &&
	XParseColor (display, cmap, "blue", &blue) &&
	XAllocColor (display, cmap, &blue))
		? blue.pixel : BlackPixel (display, screen));\fR
.fi
.sp .5
When you design an application to run on black and white workstations,
you can easily make it run on color units (in black and white) too.  
Use the \f(CWBlackPixel\fP and \f(CWWhitePixel\fP macros to find the 
appropriate pixel values; never assume hardcoded values of 0 and 1.
However, these pixel values will only result in predictably contrasting
colors when the default colormap is installed.
When the default colormap is not installed, you are at the mercy of the window manager,
and your application can do nothing constructive about it.
(It will not work to use \f(CWXAllocColor\fP to explicitly allocate cells 
for black and white from the currently installed colormap.  This might
temporarily get you the colors you want, but you have no control over 
which colormap is installed, and it is even more likely to be 
.Nd 4
uninstalled since it is not the default colormap.
Furthermore, you don't own that colormap, and you may break the application
that created it.)
.sp .5
.Nd 6
You should also be careful using logical functions
in the GC other than \f(CWGXcopy\fP on color screens, since these have
unpredictable results unless the colormap is preset or a plane mask
is set in the GC.
You can still use the functions like \f(CWGXinvert\fP that are
useful on monochrome screens, but you need to limit the
effect of the operation to a single plane with the GC's plane mask.
.sp .5
On a gray scale workstation, a color application should still work
correctly.  If a \f(CWPseudoColor\fP visual is available, use 
it.  You don't need to set the red, green and blue values to be equal, 
but instead you can expect the resultant gray scale intensities to be 
described by the following formula:
.sp .5
.nf
.ft CW
	intensity = (.30 * red) + (.59 * green) + (.11 * blue)
.fi
.ft R
.sp .5
If only a \f(CWGrayScale\fP visual is available, the red value only will
be used for gray scale intensity.  
.sp .5
Again, it is probably safer to explicitly test the visual class, and modify 
your color handling accordingly.
.sp 2
.IP \s+2\f(HB7.7.2\fR\s-2 6n  
\s+2\f(HBFinding the Visual Class\fR\s-2
.sp
Example 7-12 shows a sample of code that 
tests the default visual, and branches accordingly.
Within each branch the application would call routines 
to allocate colors appropriate for that visual class.
.nf
.sp 
\s-1\f(HIExample 7-12.  Finding the default visual\fR\s+1
.sp 
\s-1\f(CWunsigned long foreground;
unsigned long background;
Display *display;
int screen;
int depth;
Visual *visual;

/* open display, get default screen, etc. */
	
depth = DisplayPlanes(display, screen);
visual =DefaultVisual(display, screen);
if (depth == 1)  {  /* one-plane monochrome */
	/* use BlackPixel and WhitePixel only */;
	foreground = BlackPixel(display, screen);
	background = WhitePixel(display, screen);
}
else {
	switch (visual->class) {
		case PseudoColor:
			/* read/write, limited colors */
			break;
		case StaticColor:
			/* read-only, limited colors */
			break;
		case DirectColor:
			/* read/write, many colors */
			break;
		case TrueColor:
			/* read-only, many colors */
			break;
		case GrayScale:
			/* read/write, monochrome shades */
			break;
		case StaticGray:
			/* read-only, monochrome shades */
			break;
	}
}\fR\s+1
.fi
.sp .5
However, this routine takes a short-cut that is not strictly correct.
Members of the \f(CWVisual\fP structure are not supposed to be
referenced in programs, so that changes to the \f(CWVisual\fP
structure won't make programs nonportable.
.bp
.IP \s+2\f(HB7.7.3\fR\s-2 6n  
\s+2\f(HBGetting Information about Visuals\fR\s-2
.sp
The correct way to access the contents of a \f(CWVisual\fP structure
is to use \f(CWXGetVisualInfo\fP or \f(CWXMatchVisualInfo\fP.
Only programs that need to do something different depending on the
characteristics of the available visuals need to get visual information.
As demonstrated in Section 7.3.4, programs with simple color
needs can simply allocate colors or use black and white based
on the number of planes the screen has.
The \f(CWDisplayCells\fP, \f(CWDisplayPlanes\fP, and
\f(CWDefaultDepth\fP macros can give you a quick idea of what is 
available on a screen without getting visual information, except that 
you can't tell the difference between color and gray scale.
.sp .5
Applications that require a particular visual type or particular
qualities of a certain visual type can use \f(CWXGetVisualInfo\fP to
find the appropriate visual if it exists.
\f(CWXGetVisualInfo\fP returns a list of visual structures that match the
attributes specified by template and mask arguments.  The template
is an \f(CWXVisualInfo\fP structure with members set to the required 
values,
and the mask indicates which members are matched with the list of
available visuals.  \f(CWXMatchVisualInfo\fP returns a single visual that 
matches the required visual class and depth.
.sp .5
The \f(CWXVisualInfo\fP structure returns information about the
the available visuals.  It is used both to select a visual type
from those available and as a source of information while
using a particular visual.
.sp .5
The \f(CWXVisualInfo\fP structure is shown in Example 7-13.
.nf
.sp 
\s-1\f(HIExample 7-13.  The XVisualinfo structure\fR\s+1
.sp 
\s-1\f(CWtypedef struct {
	Visual *visual;
	VisualID visualid;
	int screen;
	unsigned int depth;
	int class;
	unsigned long \f(CWred_mask\fP;
	unsigned long \f(CWgreen_mask\fP;
	unsigned long \f(CWblue_mask\fP;
	int \f(CWcolormap_size\fP;	/* same as map_entries member of Visual */
	int \f(CWbits_per_rgb\fP;
} \f(CWXVisualInfo\fP;\fR\s+1
.fi
.sp .5
The \f(CWvisual\fP member is a pointer to the internal \f(CWVisual\fP
structure.  This pointer is used as the \f(CIvisual\fP argument
of \f(CWXCreateWindow\fP and \f(CWXCreateColormap\fP. 
.LP
The \f(CWvisualid\fP member is not normally needed by applications.
.sp .5
As discussed earlier, the \f(CWclass\fP member specifies whether the 
screen is to be considered color or monochrome and changeable or 
immutable.  The \f(CWclass\fP member can be one of the constants
\f(CWStaticGray\fP, \f(CWGrayScale\fP, \f(CWStaticColor\fP,
\f(CWPseudoColor\fP, \f(CWTrueColor\fP, or \f(CWDirectColor\fP.
.sp .5
The \f(CWred_mask\fP, \f(CWgreen_mask\fP, and \f(CWblue_mask\fP
members are used only for the \f(CWDirectColor\fP 
and \f(CWTrueColor\fP visual classes, 
where there is a separate map for each primary color.  
They define which bits of the pixel 
value index into the colormap for each primary color.
Each mask has one contiguous set of bits, with no bits in common
with the other masks.  These values are zero for monochrome and most 
four- to eight-plane color systems.  
.sp .5
The \f(CWcolormap_size\fP member of the structure tells you how
many different pixel values are valid with this visual.
For a monochrome screen, this value is two.
For the default visual of an eight-plane color system, this value
is typically 254 or 256 (two colors are often reserved for the 
cursor).  For \f(CWDirectColor\fP and 
\f(CWTrueColor\fP, \f(CWcolormap_size\fP will be the number of
cells for the biggest individual pixel subfield.
The \f(CWcolormap_size\fP member is the same as \f(CWmap_entries\fP
member of the visual structure.
.sp .5
The \f(CWbits_per_rgb\fP member specifies how many bits in each of
the Red, Green, and Blue values in a
colorcell are used to drive the RGB gun in the display.
For a monochrome screen, this value is one.
For the default visual of an eight-plane color system, this value
is typically eight.
The pixel subfields (the red, green, and blue values in each 
colorcell) are 16-bit unsigned short values, but only the
highest \f(CWbits_per_rgb\fP bits are used to drive the 
RGB gun in the screen.  This number corresponds the number of
bits of resolution in the Digital to Analog Converter (DAC) in the 
display hardware.
.sp 2
.IP \s+1\f(HB7.7.3.1\fR\s-1 6n
\s+1\f(HBExample of Choosing a Visual\fR\s-1
.sp
Example 7-14 shows a routine that uses \f(CWXGetVisualInfo\fR to get 
all the visuals of depth 8 on the current screen, as defined by the X server, 
and then creates a colormap and window.
.nf
.sp 
\s-1\f(HIExample 7-14.  Code to match visuals\fR\s+1
.sp 
\s-1\f(CW#include <X11/Xlib.h>
#include <X11/Xutil.h>

visual()
{
Display *display;
Colormap colormap;
Window window;
int screen;
	.
	.
	.
XVisualInfo vTemplate;    /* template of the visual we want */
XVisualInfo *visualList;  /* list of XVisualInfo structs that match */
int visualsMatched;       /* number of visuals that match */
	.
	.
	.
/* 
 * Set up the XVisualInfo template so that it returns 
 * a list of all the visuals of depth 8 defined on the 
 * current screen by the X server 
 */
vTemplate.screen = screen;
vTemplate.depth = 8; 
visualList = XGetVisualInfo (display, VisualScreenMask | 
		VisualDepthMask, &vTemplate, &visualsMatched);
if ( visualsMatched == 0 )
	fatalError ("No matching visuals\en");

/*
 * Create a colormap for a window using the first of the visuals
 * in the list ov XVisualInfo structs returned by XGetVisualInfo.
 */
colormap = XCreateColormap (display, RootWindow(display, screen), 
	visualList[0].visual, AllocNone);
	.
	.
	.
window = XCreateWindow (display, RootWindow(display, screen),
	x, y, width, height, border_width, vTemplate.depth,
	InputOutput, visualList[0].visual, mask, &attributes);
XSetWindowColormap(display, window, colormap);

/* All done with visual information.  Free it.  */

XFree(visualList);
	.
	.
	.
} /* end routine */\fR\s+1
.fi
.bp
.IP \s+4\f(HB7.8\fR\s-4
\s+4\f(HBMiscellaneous Color Handling Function\fR\s-4
.sp
The following miscellaneous functions provide additional ways to use
the color database, to find out the RGB values in a colormap cell, and
to free cells that are no longer needed.
.RS
.IP "\f(CWXLookupColor\fP" 16n
Looks up an ASCII color name in the color database and returns 
separate color structures containing 
the exact RGB values specified in the database for that name
and the closest RGB values available on the hardware.
This function does not look at any cells in the colormap, even
though it has a \f(CIcolormap\fP argument!  This argument 
specifies which screen the color should be looked up on.  
The difference between \f(CWXLookupColor\fP and \f(CWXParseColor\fP
is that \f(CWXParseColor\fP accepts the hexadecimal
color specification (which \f(CWXLookupColor\fP does not), while 
\f(CWXLookupColor\fP returns the closest
colors available on the hardware (which \f(CWXParseColor\fP does not).
.IP "\f(CWXQueryColor\fP"
Fills an \f(CWXColor\fP structure with the RGB values
corresponding to the colormap cell indicated by a pixel value.
Also sets the \f(CWflags\fP member of the structure to
\f(CW(DoRed | \f(CWDoGreen\fP | \f(CWDoBlue)\fR.
.IP "\f(CWXQueryColors\fR"
Fills multiple \f(CWXColor\fP structures with the RGB values and
flags corresponding to the colormap cells indicated by a pixel values.
Also sets each \f(CWflags\fP member to
\f(CW(DoRed | \f(CWDoGreen\fP | \f(CWDoBlue)\fR.
.IP "\f(CWXFreeColors\fR"
Frees the colormap cells associated with the given pixel
values, and/or frees the given planes.  Since all the colorcells
an application allocates are freed when the application exits, this
routine is needed only when an application is finished with cells
before it exits.  Freeing a read/write colorcell makes that cell available
to other applications.  Freeing a read-only cell may make the cell
unallocated, but only if no other application is sharing that cell.
.RE

