.\"  (c) Copyright 1989, 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
.\"  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
.\"  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
.\"  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
.\"  (c) Copyright 1988 MICROSOFT CORPORATION
.\"  ALL RIGHTS RESERVED
.nr `P 9
.ds bT "OSF AES/UE
.nr Hu 1
.HU "Preface"
...\" book stuff:
...\".nr P 9
...\".nr !_ 0
.P
Part of the charter of the Open Software Foundation\^\s-4\uTM\d\s+4
(OSF\^\s-4\uTM\d\s+4) is to
foster the development of portable software that will
run on a variety of hardware platforms.  The 
Application Environment Specification (AES) specifies the interfaces 
that support such software.  
.P
Specifically, this document (the Application Environment 
Specification (AES) User Environment Volume, Revision B) 
specifies interfaces 
for the user environment portion of OSF's Applications Environment.
.P
.ne 11
Chapter 1 describes the purpose of the Application
Environment Specification, incorporating a document originally
published by itself as "The AES
Definition."  It provides a detailed description of the 
relationship of the AES to: 
.BL
.LI
Formal (de jure) standards and specifications.
.LI
Implementations; for example, operating systems like 
OSF's operating system component (OSC).
.LI
Portable applications software.
.LE
.br
.ne 3i
.nr Hu 2
.HU "Audience"
This document is written for:
.BL
.LI
Software engineers developing AES-compliant applications 
to run on AES-compliant implementations.
.LI
Software engineers developing AES-compliant 
implementations on which AES-compliant applications
can run.
.LI
Organizations (for example, standards-setting bodies) for 
whom the AES (or some part of it) is an appropriate part 
of the formal, de jure process.
.LE
.br
.ne 3i
.nr Hu 2
.HU "Contents"
This document is organized into two chapters.
.BL
.LI
\fIChapter 1\fP introduces the AES, providing the general AES definition,
and the general rationale for inclusion and specification of
interfaces in the AES.
This chapter includes the AES Service Outline.
.LI
\fIChapter 2\fP contains manual pages for 
all of the AES/UE interfaces.  They are ordered alphabetically within
commands and functions.
.LE
.br
.ne 3i
.nr Hu 2
.HU "Typographical Conventions"
.P
This volume uses the following typographical conventions: 
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
.ne 3i
.nr Hu 2
.HU "Interface Definition Manual Page Format"
.P
.iX "manual page" "format"
The manual pages for interface definitions 
in this volume use the following format:
.br
.VL 10 5
.LI \fBPurpose\fP
.br
This section gives
a short description of the interface.
.LI "\fBAES Support Level\fP"
.br
This section indicates whether the interfaces's AES support status is
full-, trial-, or temporary-use.
.LI \fBCompatibility\fP
.br
This section lists the standards and industry specifications
in which the interface exists.
.LI \fBSynopsis\fP
.br
This section describes the appropriate syntax for using
the interface.
.LI \fBDescription\fP
.ne 16
.br
This section describes the behavior of the interface.  On widget
man pages there are tables of resource values in the descriptions.
Those tables have the following headers:
.VL 10n "" 1
.LI \fBName\fP
Contains the name of the resource.  Each new resource is described
following the new resources table.
.LI \fBClass\fP
Contains the class of the resource.
.LI \fBType\fP
Contains the type of the resource.
.LI \fBDefault\fP
Contains the default value of the resource.
.LI \fBAccess\fP
Contains the access permissions for the resource.  A \fBC\fP in this
column means the resource can be set at widget creation time.  An
\fBS\fP means the resource can be set anytime.  A
\fBG\fP means the resource's value can retrieved.
.LE
.LI "\fBReturn Value\fP"
.br
This lists the values returned by function interfaces.
.LI \fBErrors\fP
.br
This section describes the error conditions associated 
with using this interface.
.LI "\fBRelated Information\fP"
.br
This section provides cross references to related 
interfaces and header files described within this document.
.LE
