.\" $XConsortium: xdpr.man,v 1.9 88/09/06 17:20:30 jim Exp $
.TH XDPR 1 "10 May 1988" "X Version 11"
.SH NAME
xdpr \- dump an X window directly to a printer
.SH SYNOPSIS
.B xdpr
[
.I filename
]
[
.B \-display
.I host:display
]
[
.B \-P\fIprinter\fP
]
[
.B \-device
.I printer_device
]
[
option ...
]

.SH DESCRIPTION
.IR Xdpr
uses the commands 
.IR xwd (1),
.IR xpr (1), 
and
.IR lpr (1)
to dump an X window, process it for a particular printer type, and
print it out on the printer of your choice.  This is the easiest way
to get a printout of a window.  \fIXdpr\fP by default will print the
largest possible representation of the window on the output page.
.PP
The options for \fIxdpr\fP are the same as those for \fIxpr\fP,
\fIxwd\fP, and \fIlpr\fP.  The most commonly-used options are described
below; see the manual pages for these commands for
more detailed descriptions of the many options available.
.PP
.TP
.IB filename
Specifies a file containing a window dump (created by \fIxwd\fP) to be
printed instead of selecting an X window.
.TP
.B -P\fIprinter\fP
Specifies a printer to send the output to.  If a printer name is not
specified here, \fIxdpr\fP (really, \fIlpr\fP) will send your output to the
printer specified by the \fIPRINTER\fP environment variable. 
Be sure that type of the printer matches the type specified
with the \fI\-device\fP option.
.TP
.B -display \fIhost:display\fP[\fI.screen\fP]
.cm .IB host : display
Normally,
.I xdpr
gets the host and display number to use from the environment
variable ``DISPLAY''.
One can, however, specify them explicitly; see \fIX\fP(1).
.TP
.B -device \fIprinter-device\fP
Specifies the device type of the printer.  Available printer devices
are "ln03" for the DEC LN03, "pp" for the IBM 3812 PagePrinter, and
"ps" for any postscript printer (e.g. DEC LN03R or LPS40).  The
default is "ln03".
.TP
.B -help
This option displays the list of options known to xdpr.
.PP
Any other arguments 
will be passed to the 
.IR xwd (1),
.IR xpr (1),
and
.IR lpr (1)
commands as appropriate for each.
.SH SEE ALSO
xwd(1), xpr(1), lpr(1), xwud(1), X(1)
.SH ENVIRONMENT
DISPLAY - for which display to use by default.
.br
PRINTER - for which printer to use by default.
.SH COPYRIGHT
Copyright 1985, 1988, Massachusetts Institute of Technology.
.br
See \fIX(1)\fP for a full statement of rights and permissions.
.SH AUTHOR
Paul Boutin, MIT Project Athena
.br
Michael R. Gretzinger, MIT Project Athena
.br
Jim Gettys, MIT Project Athena
