.\"  (c) Copyright 1989, 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
.\"  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
.\"  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
.\"  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
.\"  (c) Copyright 1988 MICROSOFT CORPORATION
.\"  ALL RIGHTS RESERVED
.nr `P 9
.ds bT "OSF/Motif Programmer's Reference
.nr Hu 1
.HU "Preface"
.P
This is the reference manual for OSF/Motif\^\s-4\uTM\d\s+4 commands and functions.
It contains toolkit, window manager, and user interface language commands
and functions.
.nr Hu 2
.HU "Audience"
This document is written for
programmers who want to write applications using
Motif\^\s-4\uTM\d\s+4 interfaces to use as a reference.
.br
.ne 20
.nr Hu 2
.HU "Typographic Conventions"
.P
This volume uses the following typographic conventions: 
.BL
.LI
\fBBoldfaced\fP strings represent literals; type them exactly
as they appear.
.LI
\fIItalicized\fP strings represent variables (for example,
function or macro arguments).
.LI
Ellipses (...) indicate that additional arguments are optional. 
.LE
.br
.ne 30
.nr Hu 2
.HU "Manual Page Format"
.P
The manual pages in this volume use the following format:
.iX "manual pages" "format"
.iX "manual pages" "access"
.VL 10 5
.LI Purpose
.br
This section gives
a short description of the interface.
.LI Synopsis
.br
This section describes the appropriate syntax for using
the interface.
.LI Description
.br
This section describes the behavior of the interface.  On widget
manual pages there are tables of resource values in the descriptions.
Those tables have the following headers:
.VL 10n
.LI Name
Contains the name of the resource.  Each new resource is described
following the new resources table.
.LI Class
Contains the class of the resource.
.LI Type
Contains the type of the resource.
.LI Default
Contains the default value of the resource.
.LI Access
Contains the access permissions for the resource.  A \fBC\fP in this
column means the resource can be set at widget creation time.  An
\fBS\fP means the resource can be set anytime.  A
\fBG\fP means the resource's value can retrieved.
.LE
.br
.ne 4i
.LI "Examples"
.br
This sections gives practical examples for using the interface.
.LI "Return Value"
.br
This lists the values returned by function interfaces.
.LI Errors
.br
This section describes the error conditions associated 
with using this interface.
.LI "Related Information"
.br
This section provides cross-references to related 
interfaces and header files described within this document.
.LE
