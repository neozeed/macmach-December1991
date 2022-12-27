.TH XSTANDARDS 1 "Release 4"  "X Version 11"
.SH NAME
X Standards
.SH SYNOPSIS
The major goal of the MIT X Consortium is to promote cooperation within the
computer industry in the creation of standard software interfaces at all layers
in the X Window System environment.  The status of various standards and
proposed standards, and of the software in the X11R4 distribution,
is explained below.
.SH STANDARDS
The following documents are MIT X Consortium standards:
.nf

X Window System Protocol
X Version 11, Release 4
Robert W. Scheifler

Xlib - C Language X Interface
X Version 11, Release 4
James Gettys, Robert W. Scheifler, Ron Newman

X Toolkit Intrinsics - C Language Interface
X Version 11, Release 4
Joel McCormack, Paul Asente, Ralph R. Swick

Bitmap Distribution Format
Version 2.1

Inter-Client Communication Conventions Manual
Version 1.0
David S. H. Rosenthal

Compound Text Encoding
Version 1.1
Robert W. Scheifler

X Logical Font Description Conventions
Version 1.3
Jim Flowers

X Display Manager Control Protocol
Version 1.0
Keith Packard

X11 Nonrectangular Window Shape Extension
Version 1.0
Keith Packard
.fi
.SH "DRAFT STANDARDS"
The following documents are draft standards of the MIT X Consortium.
To become standards, further ``proof of concept'' is required, in the form of
working implementations.  The specifications may be subject to incompatible
changes if implementation efforts uncover significant problems.
.nf

PEX Protocol Specification
Version 4.0P
Randi J. Rost (editor)

Extending X for Double-Buffering, Multi-Buffering, and Stereo
Version 3.2
Jeffrey Friedberg, Larry Seiler, Jeff Vroom
.fi

.SH "PUBLIC REVIEW DRAFTS"
The following documents are out for Public Review for adoption as
MIT X Consortium standards.
.nf

X11 Input Extension Protocol Specification
Public Review Draft
George Sachs, Mark Patrick

X11 Input Extension Library Specification
Public Review Draft
Mark Patrick, George Sachs
.fi

.SH "INCLUDE FILES"
The following include files are part of the Xlib standard.  The C++
support in these header files is experimental, and is not yet part
of the standard.
.nf

<X11/X.h>
<X11/Xatom.h>
<X11/Xproto.h>
<X11/Xprotostr.h>
<X11/keysym.h>
<X11/keysymdef.h>
<X11/Xlib.h>
<X11/Xresource.h>
<X11/Xutil.h>
<X11/cursorfont.h>
<X11/X10.h>
<X11/Xlibint.h>
.fi
.PP
The following include files are part of the X Toolkit Intrinsics standard.
The C++ support in these header files is experimental, and is not yet part
of the standard.
.nf

<X11/Composite.h>
<X11/CompositeP.h>
<X11/ConstrainP.h>
<X11/Constraint.h>
<X11/Core.h>
<X11/CoreP.h>
<X11/Intrinsic.h>
<X11/IntrinsicP.h>
<X11/Object.h>
<X11/ObjectP.h>
<X11/Quarks.h>
<X11/RectObj.h>
<X11/RectObjP.h>
<X11/Shell.h>
<X11/ShellP.h>
<X11/StringDefs.h>
<X11/Vendor.h>
<X11/VendorP.h>
.fi
.PP
The following include file is part of the
Nonrectangular Window Shape Extension standard.
.nf

<X11/extensions/shape.h>
.fi
.PP
The following include file is part of the Multi-Buffering draft standard.
.nf

<X11/extensions/multibuf.h>
.fi

.SH "NON STANDARDS"
The X11R4 distribution contains \fIsample\fP implementations, not
\fIreference\fP implementations.  Although much of the code is believed
to be correct, the code should be assumed to be in error wherever it
conflicts with the specification.
.PP
At the public release of X11R4, the only MIT X Consortium standards are
the ones listed above.
No other documents, include files, or software in X11R4 carry special
status within the X Consortium.  For example, none of the following
are standards:
internal interfaces of the sample server;
the MIT-SHM extension;
the Input Synthesis extension;
the Athena Widget Set;
the Xmu library;
the Xau library;
CLX, the Common Lisp interface to X (although a Consortium review is
finally expected to begin);
the RGB database;
the fonts distributed with X11R4;
the applications distributed with X11R4;
the include files <X11/XWDFile.h> and <X11/Xos.h>;
the bitmap files in <X11/bitmaps>.
