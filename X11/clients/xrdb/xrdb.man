.TH XRDB 1 "Release 4" "X Version 11"
.SH NAME
xrdb - X server resource database utility
.SH SYNOPSIS
.B xrdb
[-option ...] [\fIfilename\fP]
.SH DESCRIPTION
.I Xrdb
is used to get or set the contents of the RESOURCE_MANAGER property
on the root window of screen 0.  You would normally run this program from 
your X startup file.
.LP
The resource manager (used by the Xlib routine \fIXGetDefault(3X)\fP and the
X Toolkit) uses the RESOURCE_MANAGER property to get user preferences about 
color, fonts, and so on for applications.  Having this information in
the server (where it is available to all clients) instead of on disk,
solves the problem in previous versions of X that required you to 
maintain \fIdefaults\fP files on every machine that you might use.  It also allows
for dynamic changing of defaults without editing files.
.LP
For compatibility, if there is no RESOURCE_MANAGER property defined (either 
because xrdb was not run or if the property was removed), the resource manager
will look for a file called 
.I \.Xdefaults
in your home directory.
.LP
The
.I filename
(or the standard input if - or no input file is given)
is optionally passed through the C preprocessor with the 
following symbols defined, based on the capabilities of the server
being used:
.TP 8
.B BITS_PER_RGB=num
the number of significant bits in an RGB color specification.  This is the
log base 2 of the number of distinct shades of each primary that the hardware
can generate.  Note that it usually is not related to PLANES.
.TP 8
.B CLASS=visualclass
one of StaticGray, GrayScale, StaticColor, PseudoColor, TrueColor,
DirectColor.  This is the visual class of the root window of the
default screen.
.TP 8
.B COLOR
defined only if CLASS is one of StaticColor, PseudoColor, TrueColor, or
DirectColor.
.TP 8
.B HEIGHT=num
the height of the default screen in pixels.
.TP 8
.B SERVERHOST=hostname
the hostname portion of the display to which you are connected.
.TP 8
.B HOST=hostname
the same as
.BR SERVERHOST .
.TP 8
.B CLIENTHOST=hostname
the name of the host on which
.I xrdb
is running.
.TP 8
.B PLANES=num
the number of bit planes (the depth) of the root window of the default screen.
.TP 8
.B RELEASE=num
the vendor release number for the server.  The interpretation of this
number will vary depending on VENDOR.
.TP 8
.B REVISION=num
the X protocol minor version supported by this server (currently 0).
.TP 8
.B VERSION=num
the X protocol major version supported by this server (should always be 11).
.TP 8
.B VENDOR=vendor
a string specifying the vendor of the server.
.TP 8
.B WIDTH=num
the width of the default screen in pixels.
.TP 8
.B X_RESOLUTION=num
the x resolution of the default screen in pixels per meter.
.TP 8
.B Y_RESOLUTION=num
the y resolution of the default screen in pixels per meter.
.LP
Lines that begin with an exclamation mark (!) are ignored and may
be used as comments.
.SH "OPTIONS"
.PP
.I xrdb
program accepts the following options:
.TP 8
.B \-help
This option (or any unsupported option) will cause a brief description of 
the allowable options and parameters to be printed.
.TP 8
.B \-display \fIdisplay\fP
This option specifies the X server to be used; see \fIX(1)\fP.
.TP 8
.B \-n
This option indicates that changes to the property (when used with \fI-load\fP)
or to the resource file (when used with \fI-edit\fP) should be shown on the
standard output, but should not be performed.
.TP 8
.B \-quiet
This option indicates that warning about duplicate entries should not be 
displayed.
.TP 8
.B -cpp \fIfilename\fP
This option specifies the pathname of the C preprocessor program to be used.
Although 
.I xrdb
was designed to use CPP, any program that acts as a filter
and accepts the -D, -I, and -U options may be used.
.TP 8
.B -nocpp
This option indicates that
.I xrdb
should not run the input file through a preprocessor before loading it
into the RESOURCE_MANAGER property.
.TP 8
.B \-symbols
This option indicates that the symbols that are defined for the preprocessor
should be printed onto the standard output.  It can be used in conjunction with 
.B \-query,
but not with the options that change the RESOURCE_MANAGER property.
.TP 8
.B \-query
This option indicates that the current contents of the RESOURCE_MANAGER 
property should be printed onto the standard output.  Note that since
preprocessor commands in the input resource file are part of the input
file, not part of the property, they won't appear in the output from this
option.  The
.B \-edit
option can be used to merge the contents of the property back into the input resource
file without damaging preprocessor commands.
.TP 8
.B \-load
This option indicates that the input should be loaded as the new value
of the RESOURCE_MANAGER property, replacing whatever was there (i.e.
the old contents are removed).  This is the default action.
.TP 8
.B \-merge
This option indicates that the input should be merged with, instead of
replacing, the current contents of the RESOURCE_MANAGER property.  Since
.I xrdb
can read the standard input, this option can be used to
the change the contents of the RESOURCE_MANAGER property directly from
a terminal or from a shell script.  Note that this option does a
lexicographic sorted merge of the two inputs, which is almost certainly
not what you want, but remains for backward compatibility.
.TP 8
.B \-remove
This option indicates that the RESOURCE_MANAGER property should be removed
from its window.
.TP 8
.B \-retain
This option indicates that the server should be instructed not to reset if
\fIxrdb\fP is the first client.
.TP 8
.B \-edit \fIfilename\fP
This option indicates that the contents of the RESOURCE_MANAGER property
should be edited into the given file, replacing any values already listed
there.  This allows you to put changes that you have made to your defaults
back into your resource file, preserving any comments or preprocessor lines.
.TP 8
.B \-backup \fIstring\fP
This option specifies a suffix to be appended to the filename used with
.B \-edit
to generate a backup file.
.TP 8
.B \-D\fIname\[=value\]\fP
This option is passed through to the preprocessor and is used to define 
symbols for use with conditionals such as
.I #ifdef.
.TP 8
.B \-U\fIname\fP
This option is passed through to the preprocessor and is used to remove
any definitions of this symbol.
.TP 8
.B \-I\fIdirectory\fP
This option is passed through to the preprocessor and is used to specify
a directory to search for files that are referenced with 
.I #include.
.SH FILES
Generalizes \fI~/.Xdefaults\fP files.
.SH "SEE ALSO"
X(1), XGetDefault(3X), Xlib Resource Manager documentation
.SH ENVIRONMENT
.TP 8
.B DISPLAY
to figure out which display to use.
.SH BUGS
.PP
The default for no arguments should be to query, not to overwrite, so that
it is consistent with other programs.
.SH COPYRIGHT
Copyright 1988, Digital Equipment Corporation.
.SH AUTHORS
Phil Karlton, rewritten from the original by Jim Gettys
