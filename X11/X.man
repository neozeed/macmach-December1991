.TH X 1 "Release 4"  "X Version 11"
.SH NAME
X - a portable, network-transparent window system
.SH SYNOPSIS
.PP
The X Window System
is a network transparent window system developed at MIT which runs on a wide
range of computing and graphics machines.  The core distribution from MIT
has support for the following operating systems:
.sp
.ce 11
Ultrix 3.1 (Digital)
SunOS 4.0.3 (Sun)
HP-UX 6.5 (Hewlett-Packard)
Domain/OS 10.1 (HP/Apollo)
A/UX 1.1 (Apple)
AIX RT-2.2 and PS/2-1.1 (IBM)
AOS-4.3 (IBM)
UTEK 4.0 (Tektronix)
NEWS-OS 3.2 (Sony; client only)
UNICOS 5.0.1 (Cray; client only)
UNIX(tm) System V, Release 3.2 (AT&T 6386 WGS; client only)
.sp
It should be relatively easy to build the client-side software on a
variety of other system.
Commercial implementations are also available for a wide range
of platforms.
.PP
The X Consortium requests that the following names be used when
referring to this software:
.sp
.ce 5
X
.br
X Window System
.br
X Version 11
.br
X Window System, Version 11
.br
X11
.PP
.I "X Window System"
is a trademark of the Massachusetts Institute of Technology.
.SH DESCRIPTION
X Window System servers run on computers with bitmap displays.
The server distributes user input to and accepts output requests from various
client programs through a variety of different interprocess
communication channels.  Although the most common case is for the client
programs to be
running on the same machine as the server, clients can be run transparently
from other machines (including machines with different architectures and
operating systems) as well.
.PP
X supports overlapping hierarchical subwindows and text and
graphics operations, on both monochrome and color
displays.
For a full explanation of the functions that are available, see
the \fIXlib - C Language X Interface\fP manual, 
the \fIX Window System Protocol\fP specification,
the \fIX Toolkit Intrinsics - C Language Interface\fP manual,
and various toolkit documents.
.PP
The number of programs that use \fIX\fP is growing rapidly.  Of particular
interest are:
a terminal emulator (\fIxterm\fP),
a window manager (\fItwm\fP), 
a display manager (\fIxdm\fP),
mail managing utilities (\fIxmh\fP and \fIxbiff\fP),
a manual page browser (\fIxman\fP),
a bitmap editor (\fIbitmap\fP),
access control programs (\fIxauth\fP and \fIxhost\fP),
user preference setting programs (\fIxrdb\fP, \fIxset\fP, \fIxsetroot\fP,
and \fIxmodmap\fP),
a load monitor (\fIxload\fP), clocks (\fIxclock\fP and \fIoclock\fP),
a font displayer (\fIxfd\fP),
utilities for listing information about fonts, windows, and displays
(\fIxlsfonts\fP, \fIxfontsel\fP, \fIxlswins\fP, \fIxwininfo\fP,
\fIxlsclients\fP, \fIxdpyinfo\fP, and \fIxprop\fP),
a diagnostic for seeing what events are generated and when (\fIxev\fP),
screen image manipulation utilities (\fIxwd\fP, \fIxwud\fP, \fIxpr\fP, 
and \fIxmag\fP),
and various demos (\fIxeyes\fP, \fIico\fP, \fImuncher\fP, \fIpuzzle\fP,
\fIxgc\fP, etc.).
.PP
Many other utilities, window managers, games, toolkits, etc. are available
from the user-contributed software.  See your site administrator for
details.
.SH STARTING UP
.PP 
There are two main ways of getting the X server and an initial set of
client applications started.  The particular method used depends on what
operating system you are running and on whether or not you use other window
systems in addition to X.
.TP 8
.B "\fIxdm\fP (the X Display Manager)"
If you want to always have X running on your display, your site administrator
can set your machine up to use the X Display Manager \fIxdm\fP.  This program
is typically started by the system at boot time and takes care of keeping the
server running and getting users logged in.  If you are running
\fIxdm\fP, you will see a window on the screen welcoming you to the system and
asking for your username and password.  Simply type them in as you would at
a normal terminal, pressing the Return key after each.  If you make a mistake,
\fIxdm\fP will display an error message and ask you to try again.  After you
have successfully logged in, \fIxdm\fP will start up your X environment.  By
default, if you have an executable file named \fI.xsession\fP in your
home directory,
\fIxdm\fP will treat it as a program (or shell script) to run to start up 
your initial clients (such as terminal emulators, clocks, a window manager,
user settings for things like the background, the speed of the pointer, etc.).
Your site administrator can provide details.
.TP 8
.B "\fIxinit\fP (run manually from the shell)"
Sites that support more than one window system might choose to use the
\fIxinit\fP program for starting X manually.  If this is true for your 
machine, your site administrator will probably have provided a program 
named "x11", "startx", or "xstart" that will do site-specific initialization
(such as loading convenient default resources, running a window manager, 
displaying a clock, and starting several terminal emulators) in a nice
way.  If not, you can build such a script using the \fIxinit\fP program.
This utility simply runs one user-specified program to start the server,
runs another to start up any desired clients, and then waits for either to
finish.  Since either or both of the user-specified programs may be a shell 
script, this gives substantial flexibility at the expense of a
nice interface.  For this reason, \fIxinit\fP is not intended for end users.
.SH "DISPLAY NAMES"
.PP
From the user's prospective, every X server has a \fIdisplay name\fP of the
form:
.sp
.ce 1
\fIhostname:displaynumber.screennumber\fP
.sp
This information is used by the application to determine how it should
connect to the server and which screen it should use by default
(on displays with multiple monitors):
.TP 8
.I hostname
The \fIhostname\fP specifies the name of the machine to which the display is
physically connected.  If the hostname is not given, the most efficient way of 
communicating to a server on the same machine will be used.
.TP 8
.I displaynumber
The phrase "display" is usually used to refer to collection of monitors that
share a common keyboard and pointer (mouse, tablet, etc.).  Most workstations
tend to only have one keyboard, and therefore, only one display.  Larger, 
multi-user
systems, however, will frequently have several displays so that more than
one person can be doing graphics work at once.  To avoid confusion, each
display on a machine is assigned a \fIdisplay number\fP (beginning at 0)
when the X server for that display is started.  The display number must always
be given in a display name.
.TP 8
.I screennumber
Some displays share a single keyboard and pointer among two or more monitors.
Since each monitor has its own set of windows, each screen is assigned a
\fIscreen number\fP (beginning at 0) when the X server for that display is
started.  If the screen number is not given, then screen 0 will be used.
.PP
On POSIX systems, the default display name is stored 
in your DISPLAY environment variable.  This variable is set automatically
by the \fIxterm\fP terminal emulator.  However, when you log into another
machine on a network, you'll need to set DISPLAY by hand to point to your
display.  For example,
.sp
.in +8
.nf
% setenv DISPLAY myws:0
$ DISPLAY=myws:0; export DISPLAY
.fi
.in -8
.PP
Finally, most X programs accept a command line option of 
\fB-display \fIdisplayname\fR to temporarily override the contents of
DISPLAY.  This is most commonly used to pop windows on another person's
screen or as part of a "remote shell" command to start an xterm pointing back 
to your display.  For example,
.sp
.in +8
.nf
% xeyes -display joesws:0 -geometry 1000x1000+0+0
% rsh big xterm -display myws:0 -ls </dev/null &
.fi
.in -8
.PP
X servers listen for connections on a variety of different 
communications channels (network byte streams, shared memory, etc.).
Since there can be more than one way of contacting a given server,
The \fIhostname\fP part of the display name is used to determine the
type of channel 
(also called a transport layer) to be used.  The sample servers from MIT
support the following types of connections:
.TP 8
.I "local"
.br
The hostname part of the display name should be the empty string.
For example:  \fI:0\fP, \fI:1\fP, and \fI:0.1\fP.  The most efficient
local transport will be chosen.
.TP 8
.I TCP\/IP
.br
The hostname part of the display name should be the server machine's
IP address name.  Full Internet names, abbreviated names, and IP addresses
are all allowed.  For example:  \fIexpo.lcs.mit.edu:0\fP, \fIexpo:0\fP,
\fI18.30.0.212:0\fP, \fIbigmachine:1\fP, and \fIhydra:0.1\fP.
.TP 8
.I DECnet
.br
The hostname part of the display name should be the server machine's 
nodename followed by two colons instead of one.
For example:  \fImyws::0\fP, \fIbig::1\fP, and \fIhydra::0.1\fP.
.PP
.SH "ACCESS CONTROL"
The sample server provides two types of access control:  an authorization
protocol which provides a list of ``magic cookies'' clients can send to
request access, and a list of hosts from which connections are always
accepted.  \fIXdm\fP initializes magic cookies in the server, and also places
them in a file accessible to the user.  Normally, the list of hosts from
which connections are always accepted should be empty, so that only clients
with are explicitly authorized can connect to the display.  When you add
entries to the host list (with \fIxhost\fP), the server no longer performs any
authorization on connections from those machines.  Be careful with this.
.PP
The file for authorization which both \fIxdm\fP and \fIXlib\fP use can be
specified with the environment variable \fBXAUTHORITY\fP, and defaults to
the file \fB.Xauthority\fP in the home directory.  \fIXdm\fP uses
\fB$HOME/.Xauthority\fP and will create it or merge in authorization records
if it already exists when a user logs in.
.PP
To manage a collection of authorization files containing a collection of
authorization records use \fIxauth\fP.  This program allows you to extract
records and insert them into other files.  Using this, you can send
authorization to remote machines when you login.  As the files are
machine-independent, you can also simply copy the files or use NFS to share
them.  If you use several machines, and share a common home directory with
NFS, then you never really have to worry about authorization files, the
system should work correctly by default.  Note that magic cookies transmitted
``in the clear'' over NFS or using \fIftp\fP or \fIrcp\fP can be ``stolen''
by a network eavesdropper, and as such may enable unauthorized access.
In many environments this level of security is not a concern, but if it is,
you need to know the exact semantics of the particular magic cookie to know
if this is actually a problem.
.PP
.SH "GEOMETRY SPECIFICATIONS"
One of the advantages of using window systems instead of
hardwired terminals is that 
applications don't have to be restricted to a particular size or location
on the screen.
Although the layout of windows on a display is controlled
by the window manager that the user is running (described below), 
most X programs accept
a command line argument of the form \fB-geometry \fIWIDTHxHEIGHT+XOFF+YOFF\fR
(where \fIWIDTH\fP, \fIHEIGHT\fP, \fIXOFF\fP, and \fIYOFF\fP are numbers)
for specifying a preferred size and location for this application's main
window.
.PP
The \fIWIDTH\fP and \fIHEIGHT\fP parts of the geometry specification are
usually measured in either pixels or characters, depending on the application.
The \fIXOFF\fP and \fIYOFF\fP parts are measured in pixels and are used to
specify the distance of the window from the left or right and top and bottom
edges of the screen, respectively.  Both types of offsets are measured from the
indicated edge of the screen to the corresponding edge of the window.  The X
offset may be specified in the following ways:
.TP 8
.I +XOFF
The left edge of the window is to be placed \fIXOFF\fP pixels in from the
left edge of the screen (i.e. the X coordinate of the window's origin will be 
\fIXOFF\fP).  \fIXOFF\fP may be negative, in which case the window's left edge 
will be off the screen.
.TP 8
.I -XOFF
The right edge of the window is to be placed \fIXOFF\fP pixels in from the
right edge of the screen.  \fIXOFF\fP may be negative, in which case the 
window's right edge will be off the screen.
.PP
The Y offset has similar meanings:
.TP 8
.I +YOFF
The top edge of the window is to be \fIYOFF\fP pixels below the
top edge of the screen (i.e. the Y coordinate of the window's origin will be
\fIYOFF\fP).  \fIYOFF\fP may be negative, in which case the window's top edge 
will be off the screen.
.TP 8
.I -YOFF
The bottom edge of the window is to be \fIYOFF\fP pixels above the
bottom edge of the screen.  \fIYOFF\fP may be negative, in which case 
the window's bottom edge will be off the screen.
.PP
Offsets must be given as pairs; in other words, in order to specify either
\fIXOFF\fP or \fIYOFF\fP both must be present.  Windows can be placed in the
four corners of the screen using the following specifications:
.TP 8
.I +0+0
upper left hand corner.
.TP 8
.I -0+0
upper right hand corner.
.TP 8
.I -0-0
lower right hand corner.
.TP 8
.I +0-0
lower left hand corner.
.PP
In the following examples, a terminal emulator will be placed in roughly
the center of the screen and
a load average monitor, mailbox, and clock will be placed in the upper right 
hand corner:
.sp
.nf
        xterm -fn 6x10 -geometry 80x24+30+200 &
        xclock -geometry 48x48-0+0 &
        xload -geometry 48x48-96+0 &
        xbiff -geometry 48x48-48+0 &
.fi
.PP
.SH WINDOW MANAGERS
The layout of windows on the screen is controlled by special programs called
\fIwindow managers\fP.  Although many window managers will honor geometry
specifications as given, others may choose to ignore them (requiring the user
to explicitly draw the window's region on the screen with the pointer, for 
example).
.PP
Since window managers are regular (albeit complex) client programs,
a variety of different user interfaces can be built.  The core distribution
comes with a window manager named \fItwm\fP which supports overlapping windows,
popup menus, point-and-click or click-to-type input models, title bars, nice
icons (and an icon manager for those who don't like separate icon windows).
.PP
Several other window managers are available in the user-contributed
software: \fIgwm\fP, \fIm_swm\fP, \fIolwm\fP, and \fItekwm\fP.
.SH "FONT NAMES"
Collections of characters for displaying text and symbols in X are known as
\fIfonts\fP.  A font typically contains images that share a common appearance
and look nice together (for example, a single size, boldness, slant, and
character set).  Similarly, collections of fonts that are based on a common
type face (the variations are usually called roman, bold, italic, bold italic, 
oblique, and bold oblique) are called \fIfamilies\fP.  
.PP
Sets of 
font families of the same resolution (usually measured in dots per inch) 
are further grouped into \fIdirectories\fP
(so named because they were initially stored in file system directories).
Each directory contains a database which lists the name of the font and
information on how to find the font.  The server uses these
databases to translate \fIfont names\fP (which have nothing to do with
file names) into font data.
.PP
The list of font directories in which the server looks when trying to find
a font is controlled by the \fIfont path\fP.  Although most installations
will choose to have the server start up with all of the commonly used font
directories, the font path can be changed at any time with the \fIxset\fP
program.  However, it is important to remember that the directory names are
on the \fBserver\fP's machine, not on the application's.
.PP
The default font path for
the sample server contains three directories:
.TP 8
.I /usr/lib/X11/fonts/misc
This directory contains many miscellaneous fonts that are useful on all
systems.  It contains a small family of fixed-width fonts in pixel heights
5 through 10, a family of fixed-width fonts from Dale Schumacher in
similar pixel heights, several Kana fonts from Sony Corporation,
a Kanji font, the standard cursor font, two cursor fonts from
Digital Equipment Corporation, and OPEN LOOK(tm) cursor and glyph fonts
from Sun Microsystems.
It also has font name aliases for the font names
\fBfixed\fP and \fBvariable\fP.
.TP 8
.I /usr/lib/X11/fonts/75dpi
This directory contains fonts contributed by Adobe Systems, Inc.,
Digital Equipment Corporation, Bitstream, Inc.,
Bigelow and Holmes, and Sun Microsystems, Inc.
for 75 dots per inch displays.  An integrated selection of sizes, styles, 
and weights are provided for each family.
.TP 8
.I /usr/lib/X11/fonts/100dpi
This directory contains 100 dots per inch versions of some of the fonts in the 
\fI75dpi\fP directory.  
.PP
Font databases are created by running the \fImkfontdir\fP program in the
directory containing the source or compiled versions of the fonts (in both
compressed and uncompressed formats).
Whenever fonts are added to a directory, \fImkfontdir\fP should be rerun
so that the server can find the new fonts.  To make the server reread the
font database, reset the font path with the \fIxset\fP program.  For example,
to add a font to a private directory, the following commands could be used:
.sp
.nf
        %  cp newfont.snf ~/myfonts
        %  mkfontdir ~/myfonts
        %  xset fp rehash 
.fi
.PP
The \fIxlsfonts\fP program can be used to list all of the fonts that are
found in font databases in the current font path.  
Font names tend to be fairly long as they contain all of the information
needed to uniquely identify individual fonts.  However, the sample server
supports wildcarding of font names, so the full specification
.sp
.ce 1
\fI-adobe-courier-medium-r-normal--10-100-75-75-m-60-iso8859-1\fP
.sp
could be abbreviated as:
.sp
.ce 1
\fI-*-courier-medium-r-normal--*-100-*-*-*-*-*-*\fP
.sp
or, more tersely (but less accurately):
.sp
.ce 1
\fI*-courier-medium-r-normal--*-100-*\fP
.PP
Because the shell also has special meanings for \fI*\fP and \fI?\fP,
wildcarded font names should be quoted:
.sp
.nf
        %  xlsfonts -fn '*-courier-medium-r-normal--*-100-*'
.fi
.PP
If more than one font in a given directory in the font path matches a
wildcarded font name, the choice of which particular font to return is left
to the server.  However, if fonts from more than one directory match a name,
the returned font will always be from the first such directory in the font
path.  The example given above will match fonts in both the \fI75dpi\fP and
\fI100dpi\fP directories; if the \fI75dpi\fP directory is ahead of the
\fI100dpi\fP directory in the font path, the smaller version of the font will 
be used.  
.SH "COLOR NAMES"
Most applications provide ways of tailoring (usually through resources or
command line arguments) the colors of various elements
in the text and graphics they display.  Although black and white displays
don't provide much of a choice, color displays frequently allow anywhere
between 16 and 16 million different colors.  
.PP
Colors are usually specified by their commonly-used names
(for example, \fIred\fP, \fIwhite\fP, or \fImedium slate blue\fP).
The server translates these names into appropriate screen colors using
a color database that can usually be found in \fI/usr/lib/X11/rgb.txt\fP.
Color names are case-insensitive, meaning that \fIred\fP, \fIRed\fP, 
and \fIRED\fP all refer to the same color.  
.PP
Many applications also accept color specifications of the following form:
.sp
.ce 4
#rgb
#rrggbb
#rrrgggbbb
#rrrrggggbbbb
.sp
where \fIr\fP, \fIg\fP, and \fIb\fP are hexadecimal numbers indicating how
much \fIred\fP, \fIgreen\fP, and \fIblue\fP should be displayed (zero being
none and ffff being on full).  Each field
in the specification must have the same number of digits (e.g., #rrgb or
#gbb are not allowed).  Fields that have fewer than four digits (e.g. #rgb)
are padded out with zero's following each digit (e.g. #r000g000b000).  The
eight primary colors can be represented as:
.sp
.ta 1.25in
.in +8
.nf
black	#000000000000 (no color at all)
red	#ffff00000000
green	#0000ffff0000
blue	#00000000ffff
yellow	#ffffffff0000 (full red and green, no blue)
magenta	#ffff0000ffff
cyan	#0000ffffffff
white	#ffffffffffff (full red, green, and blue)
.fi
.in -8
.PP
Unfortunately, RGB color specifications are highly unportable since different
monitors produce different shades when given the same inputs.  Similarly,
color names aren't portable because there is no standard naming scheme and 
because the color database needs to be tuned for each monitor.
.PP
Application developers should take care to make their colors tailorable.
.SH "KEYS"
.PP
The X keyboard model is broken into two layers:  server-specific codes
(called \fIkeycodes\fP) which represent the physical keys, and 
server-independent symbols (called \fIkeysyms\fP) which
represent the letters or words that appear on the keys.  
Two tables are kept in the server for converting keycodes to keysyms:
.TP 8
.I "modifier list"
Some keys (such as Shift, Control, and Caps Lock) are known as \fImodifier\fP
and are used to select different symbols that are attached to a single key
(such as Shift-a generates a capital A, and Control-l generates a formfeed
character ^L).  The server keeps a list of keycodes corresponding to the
various modifier keys.  Whenever a key is pressed or released, the server 
generates an \fIevent\fP that contains the keycode of the indicated key as 
well as a mask that specifies which of the modifier keys are currently pressed.
Most servers set up this list to initially contain
the various shift, control, and shift lock keys on the keyboard.  
.TP 8
.I "keymap table"
Applications translate event keycodes and modifier masks into keysyms
using a \fIkeysym table\fP which contains one row for each keycode and one
column for various modifier states.  This table is initialized by the server
to correspond to normal typewriter conventions, but is only used by
client programs.  
.PP
Although most programs deal with keysyms directly (such as those written with
the X Toolkit Intrinsics), most programming libraries provide routines for
converting keysyms into the appropriate type of string (such as ISO Latin-1).
.SH "OPTIONS"
Most X programs attempt to use the same names for command line options and
arguments.  All applications written with the X Toolkit Intrinsics
automatically accept the following options:
.TP 8
.B \-display \fIdisplay\fP
This option specifies the name of the X server to use.
.TP 8
.B \-geometry \fIgeometry\fP
This option specifies the initial size and location of the window.
.TP 8
.B \-bg \fIcolor\fP, \fB\-background \fIcolor\fP
Either option specifies the color to use for the window background.
.TP 8
.B \-bd \fIcolor\fP, \fB\-bordercolor \fIcolor\fP
Either option specifies the color to use for the window border.
.TP 8
.B \-bw \fInumber\fP, \fB\-borderwidth \fInumber\fP
Either option specifies the width in pixels of the window border.
.TP 8
.B \-fg \fIcolor\fP, \fB\-foreground \fIcolor\fP
Either option specifies the color to use for text or graphics.
.TP 8
.B \-fn \fIfont\fP, \fB-font \fIfont\fP
Either option specifies the font to use for displaying text.
.TP 8
.B \-iconic
.br
This option indicates that the user would prefer that the application's
windows initially not be visible as if the windows had be immediately 
iconified by the user.  Window managers may choose not to honor the
application's request.  
.TP 8
.B \-name
.br
This option specifies the name under which resources for the
application should be found.  This option is useful in shell
aliases to distinguish between invocations of an application,
without resorting to creating links to alter the executable file name.
.TP 8
.B \-rv\fP, \fB\-reverse\fP
Either option indicates that the program should simulate reverse video if 
possible, often by swapping the foreground and background colors.  Not all
programs honor this or implement it correctly.  It is usually only used on
monochrome displays.
.TP 8
.B \+rv
.br
This option indicates that the program should not simulate reverse video.  
This is used to
override any defaults since reverse video doesn't always work properly.
.TP 8
.B \-selectionTimeout
This option specifies the timeout in milliseconds within which two
communicating applications must respond to one another for a selection
request.
.TP 8
.B \-synchronous
This option indicates that requests to the X server should be sent 
synchronously, instead of asynchronously.  Since 
.I Xlib
normally buffers requests to the server, errors do not necessarily get reported
immediately after they occur.  This option turns off the buffering so that
the application can be debugged.  It should never be used with a working 
program.
.TP 8
.B \-title \fIstring\fP
This option specifies the title to be used for this window.  This information 
is sometimes
used by a window manager to provide some sort of header identifying the window.
.TP 8
.B \-xnllanguage \fIlanguage[_territory][.codeset]\fP
This option specifies the language, territory, and codeset for use in
resolving resource and other filenames.
.TP 8
.B \-xrm \fIresourcestring\fP
This option specifies a resource name and value to override any defaults.  It 
is also very useful for setting resources that don't have explicit command 
line arguments.
.SH "RESOURCES"
To make the tailoring of applications to personal preferences easier, X 
supports several mechanisms for storing default values for program resources 
(e.g. background color, window title, etc.)
Resources are specified as strings of the form 
.sp
.ce 1
\fIappname*subname*subsubname...: value\fP
.sp
that are read in from various places when an application is run.  By
convention, the application name is the same as the program name, but with 
the first letter capitalized (e.g. \fIBitmap\fP or \fIEmacs\fP) although
some programs that begin with the letter ``x'' also capitalize the second
letter for historical reasons.  The precise syntax for resources is:
.PP
.nf
.ta 1.5i 1.75i
ResourceLine	=	Comment | ResourceSpec
Comment	=	"!" string | <empty line>
ResourceSpec	=	WhiteSpace ResourceName WhiteSpace ":" WhiteSpace value
ResourceName	=	[Binding] ComponentName {Binding ComponentName}
Binding	=	"\&." | "*"
WhiteSpace	=	{" " | "\\t"}
ComponentName	=	{"a"\-"z" | "A"\-"Z" | "0"\-"9" | "_" | "-"}
value	=	string
string	=	{<any character not including "\\n">}
.fi
.PP
Note that elements enclosed in curly braces ({\&.\&.\&.}) indicate
zero or more occurrences of the enclosed elements
.PP
To allow values to contain arbitrary octets,
the 4-character sequence \\\fInnn\fP\^,
where n is a digit in the range of "0"\-"7",
is recognized and replaced with a single byte that contains
this sequence interpreted as an octal number.
For example,
a value containing a NULL byte can be stored by specifying "\\000".
.PP
The \fIXlib\fP routine
.I XGetDefault(3X)
and the resource utilities within Xlib and the X Toolkit Intrinsics
obtain resources from the following sources:
.TP 8
.B "RESOURCE_MANAGER root window property"
Any global resources that should be available to clients on all machines 
should be stored in the RESOURCE_MANAGER property on the
root window using the \fIxrdb\fP program.  This is frequently taken care
of when the user starts up X through the display manager or \fIxinit\fP.
.TP 8
.B "application-specific files"
Programs that use the X Toolkit Intrinsics will also look in the directories
named by the environment variable XUSERFILESEARCHPATH or the environment
variable XAPPLRESDIR, plus directories in a standard place (usually under
/usr/lib/X11/, but this can be overridden with the XFILESEARCHPATH
environment variable) for application-specific resources.
See the \fIX Toolkit Intrinsics - C Language Interface\fP manual for
details.
.TP 8
.B XENVIRONMENT
Any user- and machine-specific resources may be specified by setting
the XENVIRONMENT environment variable to the name of a resource file
to be loaded by all applications.  If this variable is not defined,
a file named \fI$HOME\fP/.Xdefaults-\fIhostname\fP is looked for instead,
where \fIhostname\fP is the name of the host where the application
is executing.
.TP 8
.B \-xrm \fIresourcestring\fP
Applications that use the X Toolkit Intrinsics can have resources specified
from the 
command line.  The \fIresourcestring\fP is a single resource name and value as
shown above.  Note that if the string contains characters interpreted by
the shell (e.g., asterisk), they must be quoted.
Any number of \fB\-xrm\fP arguments may be given on the
command line.
.PP
Program resources are organized into groups called \fIclasses\fP, so that 
collections of individual resources (each of which are 
called \fIinstances\fP)
can be set all at once.  By convention, the instance name of a resource
begins with a lowercase letter and class name with an upper case letter.
Multiple word resources are concatenated with the first letter of the 
succeeding words capitalized.  Applications written with the X Toolkit
Intrinsics will have at least the following resources:
.PP
.TP 8
.B background (\fPclass\fB Background)
This resource specifies the color to use for the window background.
.PP
.TP 8
.B borderWidth (\fPclass\fB BorderWidth)
This resource specifies the width in pixels of the window border.
.PP
.TP 8
.B borderColor (\fPclass\fB BorderColor)
This resource specifies the color to use for the window border.
.PP
Most applications using the X Toolkit Intrinsics also have the resource
\fBforeground\fP
(class \fBForeground\fP), specifying the color to use for text
and graphics within the window.
.PP
By combining class and instance specifications, application preferences 
can be set quickly and easily.  Users of color displays will frequently
want to set Background and Foreground classes to particular defaults.
Specific color instances such as text cursors can then be overridden
without having to define all of the related resources.  For example,
.sp
.nf
        bitmap*Dashed:  off
        XTerm*cursorColor:  gold
        XTerm*multiScroll:  on
        XTerm*jumpScroll:  on
        XTerm*reverseWrap:  on
        XTerm*curses:  on
        XTerm*Font:  6x10
        XTerm*scrollBar: on
        XTerm*scrollbar*thickness: 5
        XTerm*multiClickTime: 500
        XTerm*charClass:  33:48,37:48,45-47:48,64:48
        XTerm*cutNewline: off
        XTerm*cutToBeginningOfLine: off
        XTerm*titeInhibit:  on
        XTerm*ttyModes:  intr ^c erase ^? kill ^u
        XLoad*Background: gold
        XLoad*Foreground: red
        XLoad*highlight: black
        XLoad*borderWidth: 0
        emacs*Geometry:  80x65-0-0
        emacs*Background:  #5b7686
        emacs*Foreground:  white
        emacs*Cursor:  white
        emacs*BorderColor:  white
        emacs*Font:  6x10
        xmag*geometry: -0-0
        xmag*borderColor:  white
.fi
.PP
If these resources were stored in a file called \fI.Xresources\fP in your home
directory, they could be added to any existing resources in the server with
the following command:
.sp
.nf
        %  xrdb -merge $HOME/.Xresources
.fi
.sp
This is frequently how user-friendly startup scripts merge user-specific 
defaults
into any site-wide defaults.  All sites are encouraged to set up convenient
ways of automatically loading resources. See the \fIXlib\fP 
manual section \fIUsing the Resource Manager\fP for more information.
.SH "EXAMPLES"
The following is a collection of sample command lines for some of the 
more frequently used commands.  For more information on a particular command,
please refer to that command's manual page.
.sp
.nf
        %  xrdb -load $HOME/.Xresources
        %  xmodmap -e "keysym BackSpace = Delete"
        %  mkfontdir /usr/local/lib/X11/otherfonts
        %  xset fp+ /usr/local/lib/X11/otherfonts
        %  xmodmap $HOME/.keymap.km
        %  xsetroot -solid '#888' 
        %  xset b 100 400 c 50 s 1800 r on
        %  xset q
        %  twm
        %  xmag
        %  xclock -geometry 48x48-0+0 -bg blue -fg white
        %  xeyes -geometry 48x48-48+0
        %  xbiff -update 20 
        %  xlsfonts '*helvetica*'
        %  xlswins -l
        %  xwininfo -root
        %  xdpyinfo -display joesworkstation:0
        %  xhost -joesworkstation
        %  xrefresh
        %  xwd | xwud
        %  bitmap companylogo.bm 32x32
        %  xcalc -bg blue -fg magenta
        %  xterm -geometry 80x66-0-0 -name myxterm $*
.fi
.SH DIAGNOSTICS
A wide variety of error messages are generated from various programs.  Various
toolkits are encouraged to provide a common mechanism for locating error 
text so that applications can be tailored easily.  Programs written
to interface directly
to the \fIXlib\fP C language library are expected to do their own error
checking.
.PP
The default error handler in \fIXlib\fP (also used by many toolkits) uses
standard resources to construct diagnostic messages when errors occur.  The
defaults for these messages are usually stored in \fI/usr/lib/X11/XErrorDB\fP.
If this file is not present, error messages will be rather terse and cryptic.
.PP
When the X Toolkit Intrinsics encounter errors converting resource strings to
the
appropriate internal format, no error messages are usually printed.  This is
convenient when it is desirable to have one set of resources across a variety
of displays (e.g. color vs. monochrome, lots of fonts vs. very few, etc.),
although it can pose problems for trying to determine why an application might
be failing.  This behavior can be overridden by the setting the
\fIStringConversionsWarning\fP resource.
.PP
To force the X Toolkit Intrinsics to always print string conversion error
messages,
the following resource should be placed at the top of the file that gets
loaded onto the RESOURCE_MANAGER property
using the \fIxrdb\fP program (frequently called \fI.Xresources\fP
or \fI.Xres\fP in the user's home directory):
.sp
.nf
        *StringConversionWarnings: on
.fi
.sp
To have conversion messages printed for just a particular application,
the appropriate instance name can be placed before the asterisk:
.sp
.nf
        xterm*StringConversionWarnings: on
.fi
.SH BUGS
If you encounter a \fBrepeatable\fP bug, please contact your site
administrator for instructions on how to submit an X Bug Report.
.SH "SEE ALSO"
.PP
XConsortium(1),
XStandards(1),
appres(1), bdftosnf(1), bitmap(1), imake(1), listres(1), maze(1),
mkfontdir(1),
muncher(1), oclock(1), puzzle(1), resize(1), showsnf(1), twm(1),
xauth(1), xbiff(1),
xcalc(1), xclipboard(1), xclock(1), xditview(1), xdm(1),
xdpyinfo(1), xedit(1), xev(1), xeyes(1), xfd(1), xfontsel(1), xhost(1),
xinit(1), xkill(1), xload(1),
xlogo(1), xlsatoms(1), xlsclients(1), xlsfonts(1), xlswins(1), xmag(1),
xman(1), xmh(1), xmodmap(1),
xpr(1), xprop(1), xrdb(1), xrefresh(1), xset(1), xsetroot(1), xstdcmap(1),
xterm(1), xwd(1), xwininfo(1), xwud(1),
Xserver(1), Xapollo(1), Xcfbpmax(1), Xhp(1), Xibm(1), XmacII(1), Xmfbpmax(1),
Xqdss(1), Xqvss(1), Xsun(1), Xtek(1),
kbd_mode(1), todm(1), tox(1), biff(1), init(8), ttys(5),
.I "Xlib \- C Language X Interface\fR,\fP"
.I "X Toolkit Intrinsics - C Language Interface\fR,\fP"
and
.I "Using and Specifying X Resources"
.SH COPYRIGHT
.PP
The following copyright and permission notice outlines the rights and
restrictions covering most parts of the core distribution of the X Window
System from MIT.  Other parts have additional or different copyrights
and permissions; see the individual source files.
.sp
Copyright 1984, 1985, 1986, 1987, 1988, and 1989, by the Massachusetts 
Institute of Technology.
.sp
Permission to use, copy, modify, distribute, and sell this
software and its documentation for any purpose is hereby granted without fee,
provided that the above copyright
notice appear in all copies and that both that copyright
notice and this permission notice appear in supporting
documentation, and that the name of MIT not be used in
advertising or publicity pertaining to distribution of the
software without specific, written prior permission.
MIT makes no representations about the suitability of
this software for any purpose.  It is provided "as is"
without express or implied warranty.
.sp
This software is not subject to any license of the American
Telephone and Telegraph Company or of the Regents of the
University of California.
.SH TRADEMARKS
.PP
UNIX and OPEN LOOK are trademarks of AT&T.
X Window System is a trademark of MIT.
.SH AUTHORS
.PP
A cast of thousands, literally.  The X distribution is brought to
you by the MIT X Consortium.  The staff members at MIT responsible
for this release are:
Donna Converse (MIT X Consortium),
Jim Fulton (MIT X Consortium),
Michelle Leger (MIT X Consortium),
Keith Packard (MIT X Consortium),
Chris Peterson (MIT X Consortium),
Bob Scheifler (MIT X Consortium), and
Ralph Swick (Digital/MIT Project Athena).
