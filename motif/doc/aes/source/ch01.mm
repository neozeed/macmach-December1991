.\"  (c) Copyright 1989, 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
.\"  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
.\"  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
.\"  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
.\"  (c) Copyright 1988 MICROSOFT CORPORATION
.\"  ALL RIGHTS RESERVED
...\".pn 1
...\".OP
...\".af P 1
...\".nr P 1
...\"	book stuff:
...\".OF "'''\fH\\\\n(H1\(mi\\\\nP\fR'"
...\".EF "'\fH\\\\n(H1\(mi\\\\nP\fR'''"
.nr H1 0
.H 1 "Introduction"
.P
This chapter introduces the user environment volume of the AES. 
Section 1.1 defines the AES.
Section 1.2 is the AES/UE service outline.
Section 1.3 is a table of AES support levels ordered by
functional area.
.H 2 "The Application Environment Specification Definition"
.P
This section provides a detailed description of the Application 
Environment Specification (AES).
.H 3 "Introduction to the AES Definition"
.P
Part of the charter of the Open Software Foundation
(OSF) is to 
foster the development of portable software applications 
that will run on a wide variety of hardware platforms.  The software
required to support such applications is an ``application environment''
provided by systems or software vendors.  
.ne 3
.iX "application environment" "" 1
An application environment
is a set of programming and user interfaces
and their associated semantics, available to applications and users. 
.P
The AES
is a specification for a consistent application environment 
across different hardware platforms.
This definition uses the term ``implementation''
to describe the application environment that vendors supply to application
developers, because such software is an implementation
.iX "implementation" "" 1
of the AES.
.P
The AES specifies the following:
.BL
.LI
Application-level interfaces that an implementation
must
provide in order to
support portable
applications
.LI
Semantics or protocols associated with each of these  
interfaces
.LE
.P
This definition describes the purpose, contents, and
organization of the AES. It also discusses
the meaning of AES compliance for applications
and implementations, the relationship of the
AES to other industry documents (standards
and specifications) and to implementations.
Finally, the document describes the AES development
process and the support levels used
to characterize interfaces
within
the AES.
.H 3 "Purpose of the AES"
.P
The primary purpose of the AES is to provide a common definition of
application interfaces that both 
systems providers and systems users can rely on in the
development of portable applications.
The composite AES defines the stable, reliable, application-level 
interfaces in several functional areas.
Systems sellers who comply
with the 
AES know exactly
what interfaces they must provide, and how each 
.ne 4
element of those interfaces
must behave.  Systems buyers who look for AES conformance know unequivocally
the interfaces they can rely on.
.P
A secondary purpose of the AES is to    
take advantage of OSF's organizational charter 
to expedite the specification of application environments.
Unlike specifications and standards bodies,
OSF also provides vendor-neutral, hardware-independent
implementations.  The task of providing 
these implementations gives practical experience and 
feedback that enables the AES to expand 
faster than standards documents can.  
.ne 3
Also, OSF's technical staff is chartered to provide
an expeditious resolution of the conflicts
that can delay the decision process in other types of
organizations.  
.P
By integrating functionality that is
already standardized with newer functionality that
is suitable for eventual standardization, the AES provides material 
that contributes to future standards.  Because of OSF's
timely, vendor-neutral decision process, this newer 
functionality can be added relatively quickly. 
.H 3 "Contents of the AES"
.P
As previously stated, the AES is a set
of specifications for programming and
user interfaces and their associated semantics.
The
library routines, user commands, and data objects 
specified in the AES have the following characteristics:
.BL
.LI
They provide valuable services to portable applications.
.LI
They can be implemented on a wide variety of hardware 
platforms
and support hardware-independent applications.
.LI
They are stable (they are not likely to need to change).
.LI
They are reliable (because they are tightly specified,
applications writers can rely on consistent behavior across
applications platforms).
.ig +
.LI
They do not duplicate other elements specified in the AES.
.+
.LE
.P
We call AES-specified program and user interfaces ``portability 
interfaces'' because they provide support for portable (hardware-independent) 
applications.  
.iX "portability interface" "" 1
.P
In order to be useful
for applications development within these
criteria, the AES must provide
the richest possible function set.  Development of the AES requires 
a balance between expansion to provide richer functionality,
and conservatism to guarantee the stability of included interfaces.
.P
Internal interfaces are interfaces that 
.iX "internal interfaces" "" 1
are not
visible to applications.  Internal interfaces are
not part of the AES.  The AES specifies only application-level
portability interfaces (as previously defined).
.H 3 "Organization of the AES"
.P
The AES is an evolving set of \fIarea specifications\fR.  Each 
area specification describes portability interfaces for one \fIfunctional 
area\fR of the application environment.  OSF has identified the 
following functional areas:
.BL
.LI
Operating System Programming Interfaces
.LI
Operating System Commands and Utilities
.LI
Network Services
.LI
User Environment Services
.LI
Graphics Services
.LI
Database Management Services
.LI
Programming Languages (BASIC, Fortran, Pascal, C, 
COBOL, Ada, and Lisp)
.LE
.P
In certain functional areas,
OSF publishes its own area specifications, which add to or extend
existing standards and specifications.  This
definition focuses on the development process for the specifications that
OSF publishes.
In other areas, the AES just points to widely known existing standards
and specifications.
.P
.ne 8
The initial AES area specifications for OSF publication
are in three functional areas.  
.P
.BL
.LI
The \fIAES/Operating System\fP
area specification describes the programming interfaces that an 
AES-conforming operating 
system must provide. 
.ig +
and consists of two volumes (describing programming 
interfaces and commands and utilities, respectively).  
.ne 2
This area specification also includes protocol-independent
interfaces to communications services.
.+
.LI
The \fIAES/Network Services\fR 
area specification describes protocol-independent and
protocol-specific interfaces to
network services.  
.br
.ne 3
.LI
The \fIAES/User Environment\fR area specification describes the 
programming interfaces that an AES-compliant user interface must provide. 
.ig +
and consists of one volume (describing user interface and window 
management services).
.+
.LE
.P
An area specification can consist
of one or more volumes, and the term "AES Volume" describes
the document that contains all or part of an area specification.
.ig ++
Figure 1 shows the organization of the initial AES volumes, including 
the \fIAES/Operating System, AES/Network Services\fR and 
\fIAES/User Environment\fR volumes.
.cS book_contents/progint_organization.ps 6i 3.5i +0 +7.5i
...\" Postscript macro .c[LRCS] file width height xoff yoff
...\" yoff must be 11i - height for graphics created at top of page
...\".fl
...\".P! progint_organization.ps 3.5i\"	name height
.ne 3.5i
.rs
.SP 3.5i
.FG "The Current Organization of AES Documents"
.++
.P
.ne 4
OSF's goal is to promote usability and coherence of
the AES documents themselves; therefore, document titles are
as descriptive as possible.
Also, OSF uses revisions, rather than supplements, to add functions
to existing AES volumes. 
.P
Each revision of an AES Volume includes, for reference, the complete list of
standards 
and specifications that make up the Application Environment.  (This list
first appeared as "AES Level 0" in May, 1988.)  Because, unless otherwise
stated, the standards
and specifications that comprise the Application Environment are independent
of one another, this list is provided only for user reference.
Any direct dependencies
that an area AES has on specifications in
another area are listed within the AES Volume
for the first area. 
.H 3 "The Meaning of AES Conformance"
.P
.iX "conformance" 
This section provides a brief, general discussion of AES conformance for 
implementations and applications. 
This document does not
discuss the specific means of measuring or proving conformance 
or such issues as
validation, compliance, branding, and waivers.
.P 
.ne 3
Implementations and applications that conform to the AES do so on the basis of 
.I
area functionality.  
.R
An implementation or application that conforms to one AES area 
is considered to be AES-conforming for that area. 
.P
Implementations and applications conform to the AES differently,
since implementations 
.I
provide
.R
services and applications 
.I
use
.R
interfaces
specified in the AES to access these services.
A conforming implementation provides
.I
at least
.R
the interfaces defined in the AES.  A conforming application uses
.I
only
.R 
the interfaces defined in the AES and those defined in any standards or
specifications that the AES depends on.  Such dependencies are
called out explicitly in AES area specifications. 
.H 4 "AES Conformance for Implementations"
.P
For an implementation to be AES conforming in a functional 
area, it must
.ig +
Provide every interface specified by the AES volume for that 
functional area
.+
implement each interface element as specified by the AES volume for 
that functional area.
.P
AES area-conforming implementations 
may offer additional interfaces not specified
in the AES, or extensions to AES interfaces,
provided they do not affect the conformance of any
element of the AES-specified interfaces.
.P
Unless specifically called out within the area specification,
an implementation that conforms to one functional area need not 
provide services from other functional areas, or to conform to
other functional areas.
.sp
.ne 14
.H 4 "Conformance Document"
.P
.iX "conformance" 
Vendors of AES-conforming implementations should make available to their customers 
a conformance document that describes:
.BL
.LI
The AES area and revision the implementation conforms to
.LI
The values of all implementation-defined variables and limits
that the implementation supports
.LI
The actual behavior of all features described in the AES as
implementation defined 
.LE
.P
If the implementation supports any nonstandard extensions that
change the behavior of interfaces specified in the AES, the conformance
document must specify how to create an environment that provides 
the AES-specified behavior.  
If conformance information exists in other documents provided with
the implementation, the AES conformance
.iX "conformance" 
document can provide a reference to that information rather than repeat it.
.ig yy
if the implementation
does provide services for 
any AES functional area, these services must be AES-compliant.  
For example, an implementation that complies to the AES Operating System
functional area is AES-compliant even if the implementation does not
provide COBOL, because COBOL is in a separate functional area.  However, 
if the implementation does provide COBOL, the COBOL provided 
must conform to ANSI X3.32-1985, FIPS 021-2.  If language bindings for
COBOL are included in a functional area of the AES, these must be
provided,
as well.
.yy
.H 4 "AES Conformance for Applications"
.P
.iX "conformance" 
For an application to be AES conforming in a functional area, it 
must use only the interfaces in the relevant AES area
specification
or those in other standards and specifications the area specification
depends on.
Standards or specifications included by
reference are considered to be part of the AES for conforming applications.
.P
The application must depend only on AES-specified behavior for AES 
interfaces.  An application should not depend on
any behavior that the AES describes as unspecified, undefined,
or implementation defined.
.ig ++
Because the intention of the AES is to promote portability over
time for new and rewritten applications, it is known that in the
first AES revisions, it may be difficult or impossible to write
a useful application that is completely AES-compliant.  
.++
.H 4 "AES Support Levels and Conformance"
.P
.iX "conformance" 
.iX "support levels" "" 1
Each AES interface element has a support level, which
specifies the commitment OSF makes to its definition.
The higher the support level, the longer the
warning period required before OSF
can delete the element, or make an incompatible
change in the element's definition.  
.ne 2
(An incompatible
change is one that might require conforming applications to
be rewritten.)
.iX "conformance" 
.iX "support levels"
.P
Conforming implementations will
provide all AES interfaces for the relevant area at all support levels.
Developers of conforming applications can choose 
to use or not use elements at any support level. 
Support levels serve as advisories for application developers
because they indicate the length of time that an interface
specification is guaranteed to remain stable.
.P
During the AES development process, OSF staff members
propose support levels for interface elements, based on criteria
defined later in this document.  OSF members review
and comment on these support levels along with the rest
of the document.  
.ig yy
the relative degree of stability of an element, and
.yy
.P
Section 1.1.9 provides detailed
information about AES support levels.    
.H 3 "Relationship of the AES to Standards and Other Specifications"
.P
The AES incorporates relevant industry standards and selected
industry specifications.  When an AES area specification incorporates
an industry standard or specification, the area specification either
points to that standard or specification, or includes the text.  (Document
revision schedules, source availability, and document usability 
influence the decision to use pointers or text.)  
.P
The
AES may extend or further specify interface elements derived from
an included standard or specification, and when such extensions occur,
they are clearly marked as such
within the appropriate AES Volume.  
.P
An AES-conforming implementation of an interface should also 
conform to all included standards and specifications that contain the
interface.  If this is not possible, because of conflicts between
definitions in included standards, the AES resolves conflicts
based on a defined precedence order of standards.
AES Volumes define, in an introductory chapter, the order of precedence 
of any included standards and specifications.
.iX "conformance" 
.P
Inclusion of whole standards
and specifications in the AES sometimes results
in inclusion of interfaces or interface elements
that might not have been selected for inclusion on
their own merit.  In such cases, OSF still includes the
element because it is in the standard and the standard
takes precedence.  However, the AES interface definition notes
any problems and discourages applications'
use of the problematic interface.  OSF also works within the
standards or specification bodies to remove or modify
such interfaces.
.H 3 "Relationship of the AES to Implementations"
.P
The AES is a specification to
which vendors can build implementations.
OSF itself provides implementations for some of the AES functional areas.
These OSF implementations include the portability interfaces 
defined in the AES, and may also include interfaces not specified in 
the AES, or extensions to AES-specified interfaces.  
These extensions may be obsolete interfaces provided for support of
existing applications, or they may be experimental, new interfaces.
Only those interfaces that are recommended for use by new portable
applications will be included in the AES.
Some of the interfaces that are not in the AES 
may be candidates for inclusion in a subsequent AES revision. 
.ig ++
Figure 1 shows AES-specified interfaces as a subset of the interfaces 
provided in an implementation. 
.cS book_contents/progint_services.ps 4i 3.5i +0 +7.5i
...\" Postscript macro .c[LRCS] file width height xoff yoff
...\".fl
...\".P! progint_services.ps 3.5i\"	name height
.br
.ne 3.5i
.rs
.SP 3.5i
.FG "Interfaces Within an Implementation"
.++
.ig yy
.P
OSF's implementations provide practical experience with vendor-neutral
hardware-independent interfaces.  This experience provides a feedback 
mechanism that enables the AES to include new
interfaces faster than standards 
documents can.
.yy
.H 3 "The AES Development Process"
.P
This section describes the
processes for development of the
AES, for document revision control, and for membership
review cycles.
.P
\fRFor each functional area, OSF produces one or more AES area 
specification 
drafts for its membership to review (see Section 1.1.8.3).  
After incorporating review 
comments, OSF produces a final AES area specification.  A final AES 
area specification for a particular revision includes the interface definitions for a stable 
set of portability interfaces in one functional area of the OSF-supported 
Application Environment.
.H 4 "Document Control and the AES"
.P
OSF labels the AES for each functional area with a \fIrevision 
letter\fR.  As the collection of revisions grows, OSF may group the existing 
revisions into an \fIAES Level\fR.  For example, an AES Level 
might include AES/UE Revision B, AES/OS Revision A, and so on.  
.H 4 "The Service Outline"
.P
.iX "service outline" "" 1
A Service Outline is a document that lists all
the elements in each AES interface for a functional
area and
indicates the
support level for each element without giving detailed descriptions.
OSF makes two uses of Service Outlines. 
.BL
.LI
In some functional areas, a Service Outline
may be a draft document that precedes a full AES volume.  (A full AES volume
provides complete
interface definitions or pointers to other information or conceptual descriptions.)
A draft Service Outline proposes the interfaces to be included 
in a full AES volume.
In this case, the full AES volume draft, when complete, supersedes the 
Service Outline draft.  
.LI
In other functional areas, a Service Outline may become a permanent document.  If 
all the interfaces for a functional area are completely specified, with
no conflicts,  
in industry standards or other specification documents, the  
.ne 2
AES area specification for that functional area may remain as a Service Outline and
never evolve into a full AES volume. 
.LE
.iX "service outline" 
.P
When appropriate, a Service Outline also contains a table 
that shows each element and lists relevant industry standards,
specifications, and implementations that include the element.
.ig yy
For example:

.TS H
expand,box,tab(@);
lb lb cb cb cb cb cb cb.
@AES@@POSIX@ANSI C@@@BSD
Name@Level@XPG3@1003.1@X3J11@SVID89@SVID2@4\.3
=
.TH
.T&
l l c c c c c c.
call_name@full-use@x@x@-@-@x@x
call_name@trial-use@x@-@-@x@x@-
.TE
.yy
.P
Complete AES documents (not Service Outlines)
provide complete interface definitions and
pointers to other information or conceptual descriptions.
.H 4 "The Membership Review"
.P
.iX "membership review" "" 1
\fRIn general, membership review proceeds as follows:
.AL 1
.LI
OSF prepares a draft Service Outline and/or AES Volume for a 
functional area, and circulates it to OSF members.  This review period
may last from one to several months. 
.LI
Members submit comments using a prescribed 
comment template.
.LI
OSF responds to members' comments in the next version of the document,
or in a discussion 
that takes place in an electronic news group or at a meeting.
.LE
.P
OSF considers all review comments during the development of AES 
documents and brings important or controversial issues up for further discussion.
However,
the review process is not a voting process, 
and OSF does not wait for consensus among the membership before 
adding new interfaces to the AES or making other technical decisions.
.H 3 "AES Support Levels"
.P
.iX "support levels"
This section defines the support levels assigned to each
AES interface element.  As previously mentioned,
support levels define OSF's commitment
to interface definitions by indicating the
warning period required to make an incompatible modification
or deletion of the definition.
.ig yy
.P
It is important to note that 
AES support levels do 
.I
not
.R
describe OSF's level of support for elements in an OSF implementation.
.yy
.P
New AES revisions may introduce upwardly compatible changes at any
time, regardless of the support level.  Any such changes will be noted
in the associated \fBSpecification Context\fP section.
.iX "support levels"
.P
The support levels are:
.BL
.LI
Full use
.LI
Trial use
.LI
Temporary use
.LE
.P
.iX "support levels"
.iX "full use" 
.iX "trial use"
.iX "temporary use"
Typically,
elements in the AES have full-use or trial-use support levels.  Conforming implementations
must provide them, and application developers can use them freely (with
the knowledge that trial-use elements are subject to more rapid
change than full-use elements).  The temporary-use support level appears only
rarely in area specifications; it is for special cases, as described below.
Implementations must provide temporary-use elements too, when they appear.
.P
The following sections explain each support level.  
Section 1.1.9.4 describes how elements move from
proposed status (in draft specifications) to final status
(in published specifications). 
.H 4 "Full Use"
.P
.iX "support levels"
.iX "full use" "" 1
A full-use element has the highest support level,
so it is the most protected from incompatible
modification or deletion from the AES.
.P
.ne 2.5i
OSF assigns a support level of full use
to elements for reasons such as the following: 
.BL
.LI
The element 
already exists
in an approved de-jure standard.  (A de-jure
standard is one that is set by an official
standards body.) 
.LI
The element as specified in the AES 
is considered stable and already
in widespread use in applications. 
.LI
The element has been upgraded to 
full-use status after a period of trial-use status in an
earlier AES
revision.  
.LE
.P
There should rarely be a need to remove a full-use element,
or make incompatible modifications to it. 
However, if this ever becomes necessary, 
a full-use element keeps its full-use status,
but a warning describing the proposed future change must be published
in at least two successive revisions of the AES area volume before the
change can be made.
This provides
time for applications to be altered to deal with a different behavior,
and for implementations to prepare for the change.
.P
For example, suppose
it becomes necessary 
to modify a full-use element that
appeared in Revision A of an AES Volume. The draft for
Revision B shows the element as "proposed-for-modification/removal."
Assuming the review concludes that this change is appropriate, 
the element in Revision B still
has full-use status, but is accompanied by a warning.  The warning
states that the element is scheduled for modification 
after Revision C, and describes the modified behavior.  
Application developers can now allow for either the original or
the modified behavior.  Revision
C contains the same warning.  Revision D provides the modified
definition only.  The \fBSpecification Context\fP section of the interface element
definition documents the history of such changes.
.iX "support levels"
.iX "full use" 
.H 4 "Trial Use"
.P
.iX "support levels"
.iX "trial use" "" 1
A trial-use element is 
easier to modify or delete than
a full-use element.  There are several reasons
that OSF classifies elements as trial use instead
of full use.
An element may be under consideration
for inclusion in a de-jure standard and so may possibly
change as a result of the standards process.  Or, OSF may
perceive that the element is new 
.ne 1i
compared
to other included elements and, therefore,
the implementation and use of the element may suggest revisions in its
definition.
.P
If it becomes necessary to modify or delete 
a trial-use element, it keeps its trial-use status,
with warnings about its removal or incompatible change,
for one full revision of the AES.  In the example
above, if the element to be modified were a trial-use
element, Revision B would include the unmodified definition with
a warning and description of the change, and Revision C would 
include the modified definition only.
.iX "support levels"
.iX "trial use" 
.H 4 "Temporary Use"
.P
.iX "support levels"
.iX "temporary use" "" 1
A temporary-use element is a special case.  
Because it is limited in use, not sufficiently general, or
faulty in some other way, it is likely to be changed.  As
such, it does not meet the criteria for inclusion in the AES with a 
full-use or trial-use support level, but it  
provides necessary functionality not available through other
full-use or trial-use interface elements.  Conforming applications
may use these elements as necessary; an application containing
a temporary-use element should be labeled
as such.  The AES replaces temporary-use interface
elements when appropriate full- or trial-use elements
become available.
.P
Giving an element temporary-use status makes
it clear from the outset that OSF intends to replace it.
In the meantime, the
element provides necessary and clearly specified functionality.
Compliant implementations must provide services
classified as temporary use, and applications can use them, although
it is clear that the element will eventually be replaced. 
Once a replacement exists, the AES includes a warning in the
.ne 3
temporary-use element's definition
and lists the removal date.  
As in the case of a trial-use element, the warning exists
for one revision only. 
.P
For example, certain network interfaces 
provide a general service using a protocol-specific algorithm.  These
might be candidates for temporary-use status,  
because as time passes, interfaces based on
protocol-independent algorithms will become available to replace them.
.iX "support levels"
.iX "temporary use" 
.ig yy
.H 4 "Intended-for-Removal"
.P
An intended-for-removal element is one that OSF intends to
remove from the AES.  Implementations must provide the
element, but new applications should not use it. 
Definitions of intended-for-removal elements
include the removal schedule. 
.P
OSF may mark as intended-for-removal
.BL
.LI
a temporary-use element that has been superseded by
a full- or trial-use
element.  (The element interface definition specifies
the exact removal schedule.)
.LI
an element that exists in a standard or specification 
included in the AES, but that is not
portable, or redundant with other AES elements.  Because
of its existence in an included standard or specification,
OSF is constrained not to remove the element, but will
work within the standard or specification bodies to have
them do so.  Removal dates for such elements are unspecified,
and set only when the constraints on removal are lifted.
.LI
a full-use element from a previous revision of the standard,
which OSF members have approved removing
from the standard.  Its actual removal date is in the second
final revision after the one in which it first appeared as
intended for removal.  (The element interface definition specifies
the exact removal schedule.) 
.LE
.yy
.H 4 "Proposed Usage Levels"
.P
.iX "support levels"
Draft versions of specifications give newly added
or changed elements a ``proposed-for-\fIlevel\fR'' status, where \fIlevel\fR is
one of those defined previously.  In final versions, these
elements move from 
``proposed-for-\fIlevel\fR''
status to
.I
level 
.R
status.   
.P
In the review draft of the first revision of an
AES area specification, all elements are
at ``proposed-for-\fIlevel\fP''
.R
status.
In review drafts of subsequent revisions, the
elements may have
one of several statuses.  Most existing 
elements retain their support level from the
existing revision.  A few may carry a 
proposed-for-\fIchange\fR
status (described below).  New elements
carry a proposed-for-\fIlevel\fP status.
.R
.P
The following list defines more exactly 
the AES proposed-for-inclusion and proposed
for-change levels. 
.P
.VL 5 0
.LI \fBProposed-for-\fIlevel\fP-use\fR
.br
A review level leading to \fIlevel\fR-use
inclusion on acceptance, and no change 
in status otherwise.  This 
status may be used to propose a new
element for \fIlevel\fR-use, or to move 
an existing element to
a higher status.
.LI \fBProposed-for-modification/removal\fP  
.br
A review level for existing elements that OSF
proposes to make 
an incompatible modification in or remove from the AES. 
If this proposal is accepted during the review process,
a full-use element remains as is, with a warning,
for two revisions; a trial-use or temporary-use element remains
as is with a warning, for one revision.  If the proposal is rejected,
the element remains as is.
.LI \fBProposed-for-correction\fP    
.iX "support levels"
.ne 3i
.br
A review level for elements of any support level in which OSF wishes to 
correct a specification error.  
.ig yy
Correction of
such errors must not introduce any incompatibilities.
.yy
OSF will propose correcting an element if
a definition was obviously wrong (and
implementations and applications could never follow
the specification as it is written),
if clarification of an unclear
section is required, or if an error makes a definition 
clearly internally inconsistent or inappropriate.  
Elements proposed for correction
return to their original status, in corrected form, 
on acceptance
of the correction. They return to their original
status in uncorrected form on rejection of the
.ne 3
correction.  (Proposal for correction is not required for OSF
to fix a typographical error.)
.LI \fBProposed-for-enhancement\fP
.iX "support levels"
.br
A review level for elements in which OSF
wants to make an upwardly compatible change in definition.
If accepted, the definition change is effective in
the published revision after the draft in which
the proposal for enhancement occurred.
.LE
.ig ++
.if t .SK
.cS book_contents/progint_levels.ps 6i 6i +0 +5i
...\" Postscript macro .c[LRCS] file width height xoff yoff
...\".fl
...\".P! progint_levels.ps 6i\"	name height
.ne 5i
.rs
.SP 5i
.FG "AES Levels of Support"
.++
.br
.H 2 "Service Outline"
.P
This service outline lists all the services
included in the AES/UE.
.BL
.LI
The first column
is the name of the service.  The table is organized
alphabetically by this column.  All services that are new in
the implementation for this revision are marked by an asterisk (*).
.LI
The second column is the
type of the service.  The type is widget,
function, command, data type, or file format.
.LI
The third column is
the AES support level.
.LE
.P
.ne 3i
.TB "Service Outline"
.sp
.in 0
.TS H
allbox,tab(@);
lb lb lb.
Service Name@Service Type@AES Level
_
.TH
.T&
l l l.
ApplicationShell@widget@full-use
Composite@widget@full-use
Constraint@widget@full-use
Core@widget@full-use
MrmCloseHierarchy@function@trial-use
MrmFetchColorLiteral@function@trial-use
MrmFetchIconLiteral@function@trial-use
MrmFetchLiteral@function@trial-use
MrmFetchSetValues@function@trial-use
MrmFetchWidget@function@trial-use
MrmFetchWidgetOverride@function@trial-use
MrmInitialize@function@trial-use
MrmOpenHierarchy@function@trial-use
MrmRegisterClass@function@trial-use
MrmRegisterNames@function@trial-use
MrmRegisterNamesInHierarchy*@function@trial-use
Object@widget@full-use
OverrideShell@widget@full-use
RectObj@widget@full-use
Shell@widget@full-use
TopLevelShell@widget@full-use
TransientShell@widget@full-use
Uil@function@trial-use
VendorShell@widget@full-use
WMShell@widget@full-use
XmActivateProtocol@function@trial-use
XmActivateWMProtocol@function@trial-use
XmAddProtocolCallbacks@function@trial-use
XmAddProtocols@function@trial-use
XmAddTabGroup@function@full-use
XmAddWMProtocolCallbacks@function@trial-use
XmAddWMProtocols@function@trial-use
XmArrowButton@widget@full-use
XmArrowButtonGadget@widget@full-use
XmBulletinBoard@widget@full-use
XmCascadeButton@widget@full-use
XmCascadeButtonGadget@widget@full-use
XmCascadeButtonGadgetHighlight*@function@trial-use
XmCascadeButtonHighlight@function@full-use
XmClipboardCancelCopy@function@full-use
XmClipboardCopy@function@full-use
XmClipboardCopyByName@function@full-use
XmClipboardEndCopy@function@full-use
XmClipboardEndRetrieve@function@full-use
XmClipboardInquireCount@function@full-use
XmClipboardInquireFormat@function@full-use
XmClipboardInquireLength@function@full-use
XmClipboardInquirePendingItems@function@full-use
XmClipboardLock@function@full-use
XmClipboardRegisterFormat@function@full-use
XmClipboardRetrieve@function@full-use
XmClipboardStartCopy@function@full-use
XmClipboardStartRetrieve@function@full-use
XmClipboardUndoCopy@function@full-use
XmClipboardUnlock@function@full-use
XmClipboardWithdrawFormat@function@full-use
XmCommand@widget@full-use
XmCommandAppendValue@function@full-use
XmCommandError@function@full-use
XmCommandGetChild@function@full-use
XmCommandSetValue@function@full-use
XmConvertUnits@function@trial-use
XmCreateArrowButton@function@full-use
XmCreateArrowButtonGadget@function@full-use
XmCreateBulletinBoard@function@full-use
XmCreateBulletinBoardDialog@function@full-use
XmCreateCascadeButton@function@full-use
XmCreateCascadeButtonGadget@function@full-use
XmCreateCommand@function@full-use
XmCreateDialogShell@function@full-use
XmCreateDrawingArea@function@full-use
XmCreateDrawnButton@function@full-use
XmCreateErrorDialog@function@full-use
XmCreateFileSelectionBox@function@full-use
XmCreateFileSelectionDialog@function@full-use
XmCreateForm@function@full-use
XmCreateFormDialog@function@full-use
XmCreateFrame@function@full-use
XmCreateInformationDialog@function@full-use
XmCreateLabel@function@full-use
XmCreateLabelGadget@function@full-use
XmCreateList@function@full-use
XmCreateMainWindow@function@full-use
XmCreateMenuBar@function@full-use
XmCreateMenuShell@function@full-use
XmCreateMessageBox@function@full-use
XmCreateMessageDialog@function@full-use
XmCreateOptionMenu@function@full-use
XmCreatePanedWindow@function@full-use
XmCreatePopupMenu@function@full-use
XmCreatePromptDialog@function@full-use
XmCreatePulldownMenu@function@full-use
XmCreatePushButton@function@full-use
XmCreatePushButtonGadget@function@full-use
XmCreateQuestionDialog@function@full-use
XmCreateRadioBox@function@full-use
XmCreateRowColumn@function@full-use
XmCreateScale@function@full-use
XmCreateScrollBar@function@full-use
XmCreateScrolledList@function@full-use
XmCreateScrolledText@function@full-use
XmCreateScrolledWindow@function@full-use
XmCreateSelectionBox@function@full-use
XmCreateSelectionDialog@function@full-use
XmCreateSeparator@function@full-use
XmCreateSeparatorGadget@function@full-use
XmCreateWorkArea*@function@trial-use
XmCreateText@function@full-use
XmCreateToggleButton@function@full-use
XmCreateToggleButtonGadget@function@full-use
XmCreateWarningDialog@function@full-use
XmCreateWorkingDialog@function@full-use
XmCvtCTToXmString*@function@trial-use
XmCvtXmStringToCT*@function@trial-use
XmDeactivateProtocol@function@trial-use
XmDeactivateWMProtocol@function@trial-use
XmDestroyPixmap@function@full-use
XmDialogShell@widget@full-use
XmDrawingArea@widget@full-use
XmDrawnButton@widget@full-use
XmFileSelectionBox@widget@full-use
XmFileSelectionBoxGetChild@function@full-use
XmFileSelectionDoSearch@function@full-use
XmFontList*@data type@trial-use
XmFontListAdd@function@trial-use
XmFontListCreate@function@trial-use
XmFontListFree@function@trial-use
XmForm@widget@full-use
XmFrame@widget@full-use
XmGadget@widget@full-use
XmGetAtomName@function@full-use
XmGetColors*@function@trial-use
XmGetMenuCursor@function@full-use
XmGetPixmap@function@full-use
XmGetPostedFromWidget*@function@trial-use
XmInstallImage@function@full-use
XmInternAtom@function@full-use
XmIsMotifWMRunning@function@full-use
XmLabel@widget@full-use
XmLabelGadget@widget@full-use
XmList@widget@full-use
XmListAddItem@function@full-use
XmListAddItems*@function@trial-use
XmListAddItemUnselected@function@full-use
XmListDeleteAllItems*@function@trial-use
XmListDeleteItem@function@full-use
XmListDeleteItems*@function@trial-use
XmListDeleteItemsPos*@function@trial-use
XmListDeletePos@function@full-use
XmListDeselectAllItems@function@full-use
XmListDeselectItem@function@full-use
XmListDeselectPos@function@full-use
XmListGetMatchPos*@function@trial-use
XmListGetSelectedPos*@function@trial-use
XmListItemExists@function@full-use
XmListItemPos*@function@trial-use
XmListReplaceItems*@function@trial-use
XmListReplaceItemsPos*@function@trial-use
XmListSelectItem@function@full-use
XmListSelectPos@function@full-use
XmListSetAddMode*@function@trial-use
XmListSetBottomItem@function@full-use
XmListSetBottomPos@function@full-use
XmListSetHorizPos@function@full-use
XmListSetItem@function@full-use
XmListSetPos@function@full-use
XmMainWindow@widget@full-use
XmMainWindowSep1@function@full-use
XmMainWindowSep2@function@full-use
XmMainWindowSep3*@function@trial-use
XmMainWindowSetAreas@function@full-use
XmManager@widget@full-use
XmMenuPosition@function@full-use
XmMenuShell@widget@full-use
XmMessageBox@widget@full-use
XmMessageBoxGetChild@function@full-use
XmOptionButtonWidget@function@full-use
XmOptionLabelWidget@function@full-use
XmPanedWindow@widget@full-use
XmPrimitive@widget@full-use
XmProcessTraversal*@function@trial-use
XmPushButton@widget@full-use
XmPushButtonGadget@widget@full-use
XmRemoveProtocolCallbacks@function@trial-use
XmRemoveProtocols@function@trial-use
XmRemoveTabGroup@function@full-use
XmRemoveWMProtocolCallbacks@function@trial-use
XmRemoveWMProtocols@function@trial-use
XmResolvePartOffsets@function@full-use
XmRowColumn@widget@full-use
XmScale@widget@full-use
XmScaleGetValue@function@full-use
XmScaleSetValue@function@full-use
XmScrollBar@widget@full-use
XmScrollBarGetValues@function@full-use
XmScrollBarSetValues@function@full-use
XmScrolledWindow@widget@full-use
XmScrolledWindowSetAreas@function@full-use
XmSelectionBox@widget@full-use
XmSelectionBoxGetChild@function@full-use
XmSeparator@widget@full-use
XmSeparatorGadget@widget@full-use
XmSetMenuCursor@function@full-use
XmSetProtocolHooks@function@trial-use
XmSetWMProtocolHooks@function@trial-use
XmString*@data type@trial-use
XmStringBaseline@function@trial-use
XmStringByteCompare@function@trial-use
XmStringCompare@function@trial-use
XmStringConcat@function@trial-use
XmStringCopy@function@trial-use
XmStringCreate@function@temporary-use
XmStringCreateSimple*@function@temporary-use
XmStringDirection*@data type@trial-use
XmStringDraw@function@trial-use
XmStringDrawImage@function@trial-use
XmStringDrawUnderline@function@trial-use
XmStringEmpty@function@trial-use
XmStringExtent@function@trial-use
XmStringFree@function@trial-use
XmStringHasSubstring*@function@trial-use
XmStringHeight@function@trial-use
XmStringLength@function@trial-use
XmStringLineCount@function@trial-use
XmStringNConcat@function@trial-use
XmStringNCopy@function@trial-use
XmStringSeparatorCreate@function@trial-use
XmStringTable*@data type@trial-use
XmStringWidth@function@trial-use
XmText@widget@full-use
XmTextClearSelection@function@full-use
XmTextCopy*@function@trial-use
XmTextCut*@function@trial-use
XmTextGetBaseline*@function@trial-use
XmTextGetEditable@function@full-use
XmTextGetInsertionPosition*@function@trial-use
XmTextGetLastPosition*@function@trial-use
XmTextGetMaxLength@function@full-use
XmTextGetSelection@function@full-use
XmTextGetSelectionPosition*@function@trial-use
XmTextGetString@function@full-use
XmTextGetSource*@function@trial-use
XmTextGetTopCharacter*@function@trial-use
XmTextInsert*@function@trial-use
XmTextPaste*@function@trial-use
XmTextPosToXY*@function@trial-use
XmTextPosition*@data type@trial-use
XmTextRemove*@function@trial-use
XmTextReplace@function@full-use
XmTextScroll*@function@trial-use
XmTextSetAddMode*@function@trial-use
XmTextSetEditable@function@full-use
XmTextSetHighlight*@function@trial-use
XmTextSetInsertionPosition*@function@trial-use
XmTextSetMaxLength@function@full-use
XmTextSetSelection@function@full-use
XmTextSetSource*@function@trial-use
XmTextSetString@function@full-use
XmTextSetTopCharacter*@function@trial-use
XmTextShowPosition*@function@trial-use
XmTextXYToPos*@function@trial-use
XmToggleButton@widget@full-use
XmToggleButtonGadget@widget@full-use
XmToggleButtonGadgetGetState@function@full-use
XmToggleButtonGadgetSetState@function@full-use
XmToggleButtonGetState@function@full-use
XmToggleButtonSetState@function@full-use
XmTrackingLocate*@function@trial-use
XmUninstallImage@function@full-use
XmUpdateDisplay@function@full-use
mwm@command@full-use
.TE
.in
.br
.ne 6i
.H 2 "Overview of Services by Type and Function"
.P
All the services are broken into four types in
the tables below.
.BL
.LI
Window manager
.LI
Widgets and widget functions
.LI
Toolkit functions
.LI
User interface language
.LE
.P
Within each table components are organized by function.
.br
.ne 2i
.H 3 "Window manager"
.P
.TB "Window Manager Services"
.sp
.TS H
allbox, center, tab(@);
lb lb lb.
Service Name@Service Type@AES Level
_
.TH
.T&
l l l.
mwm@command@full-use
.TE
.br
.ne 4i
.H 3 "Widgets and Widget Functions"
.P
This table organizes widgets by hierarchy.  
Position in the hierarchy is shown by the indentation of
the service name.
The functions for each widget immediately follow the widget.
.P
.in 0
.TB "Widget Services"
.sp
.TS H
allbox tab(@);
lb lb lb.
Service Name@Service Type@AES Level
_
.TH
.T&
l l l.
\f3Core\fP@widget@full-use
\ \ \f3Object\fP@widget@full-use
\ \ \ \ \f3RectObj\fP@widget@full-use
\ \ \ \ \ \ \ \ \f3XmPrimitive\fP@widget@full-use
\ \ \ \ \ \ \ \ \ \ \f3XmArrowButton\fP@widget@full-use
\ \ \ \ \ \ \ \ \ \ \ \ XmCreateArrowButton@function@full-use
\ \ \ \ \ \ \ \ \ \ \f3XmLabel\fP@widget@full-use
\ \ \ \ \ \ \ \ \ \ \ \ XmCreateLabel@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \f3XmCascadeButton\fP@widget@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ XmCreateCascadeButton@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ XmCascadeButtonHighlight@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \f3XmDrawnButton\fP@widget@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ XmCreateDrawnButton@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \f3XmPushButton\fP@widget@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ XmCreatePushButton@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \f3XmToggleButton\fP@widget@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ XmCreateToggleButton@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ XmToggleButtonGetState@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ XmToggleButtonSetState@function@full-use
\ \ \ \ \ \ \ \ \ \ \f3XmList\fP@widget@full-use
\ \ \ \ \ \ \ \ \ \ \ \ XmCreateList@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ XmListAddItem@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ XmListAddItems*@function@trial-use
\ \ \ \ \ \ \ \ \ \ \ \ XmListAddItemUnselected@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ XmListDeleteAllItems*@function@trial-use
\ \ \ \ \ \ \ \ \ \ \ \ XmListDeleteItem@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ XmListDeleteItems*@function@trial-use
\ \ \ \ \ \ \ \ \ \ \ \ XmListDeleteItemsPos*@function@trial-use
\ \ \ \ \ \ \ \ \ \ \ \ XmListDeletePos@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ XmListDeselectAllItems@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ XmListDeselectItem@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ XmListDeselectPos@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ XmListGetMatchPos*@function@trial-use
\ \ \ \ \ \ \ \ \ \ \ \ XmListGetSelectedPos*@function@trial-use
\ \ \ \ \ \ \ \ \ \ \ \ XmListItemExists@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ XmListItemPos*@function@trial-use
\ \ \ \ \ \ \ \ \ \ \ \ XmListReplaceItems*@function@trial-use
\ \ \ \ \ \ \ \ \ \ \ \ XmListReplaceItemsPos*@function@trial-use
\ \ \ \ \ \ \ \ \ \ \ \ XmListSelectItem@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ XmListSelectPos@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ XmListSetAddMode*@function@trial-use
\ \ \ \ \ \ \ \ \ \ \ \ XmListSetBottomItem@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ XmListSetBottomPos@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ XmListSetHorizPos@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ XmListSetItem@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ XmListSetPos@function@full-use
\ \ \ \ \ \ \ \ \ \ \f3XmScrollBar\fP@widget@full-use
\ \ \ \ \ \ \ \ \ \ \ \ XmCreateScrollBar@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ XmScrollBarGetValues@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ XmScrollBarSetValues@function@full-use
\ \ \ \ \ \ \ \ \ \ \f3XmSeparator\fP@widget@full-use
\ \ \ \ \ \ \ \ \ \ \ \ XmCreateSeparator@function@full-use
\ \ \ \ \ \ \ \ \ \ \f3XmText\fP@widget@full-use
\ \ \ \ \ \ \ \ \ \ \ \ XmCreateText@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ XmTextClearSelection@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ XmTextCopy*@function@trial-use
\ \ \ \ \ \ \ \ \ \ \ \ XmTextCut*@function@trial-use
\ \ \ \ \ \ \ \ \ \ \ \ XmTextGetBaseline*@function@trial-use
\ \ \ \ \ \ \ \ \ \ \ \ XmTextGetEditable@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ XmTextGetInsertionPosition*@function@trial-use
\ \ \ \ \ \ \ \ \ \ \ \ XmTextGetLastPosition*@function@trial-use
\ \ \ \ \ \ \ \ \ \ \ \ XmTextGetMaxLength@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ XmTextGetSelection@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ XmTextGetSelectionPosition*@function@trial-use
\ \ \ \ \ \ \ \ \ \ \ \ XmTextGetSource*@function@trial-use
\ \ \ \ \ \ \ \ \ \ \ \ XmTextGetString@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ XmTextGetTopCharacter*@function@trial-use
\ \ \ \ \ \ \ \ \ \ \ \ XmTextInsert*@function@trial-use
\ \ \ \ \ \ \ \ \ \ \ \ XmTextPaste*@function@trial-use
\ \ \ \ \ \ \ \ \ \ \ \ XmTextPosToXY*@function@trial-use
\ \ \ \ \ \ \ \ \ \ \ \ XmTextRemove*@function@trial-use
\ \ \ \ \ \ \ \ \ \ \ \ XmTextReplace@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ XmTextScroll*@function@trial-use
\ \ \ \ \ \ \ \ \ \ \ \ XmTextSetAddMode*@function@trial-use
\ \ \ \ \ \ \ \ \ \ \ \ XmTextSetEditable@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ XmTextSetHighlight*@function@trial-use
\ \ \ \ \ \ \ \ \ \ \ \ XmTextSetInsertionPosition*@function@trial-use
\ \ \ \ \ \ \ \ \ \ \ \ XmTextSetMaxLength@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ XmTextSetSelection@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ XmTextSetSource*@function@trial-use
\ \ \ \ \ \ \ \ \ \ \ \ XmTextSetString@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ XmTextSetTopCharacter*@function@trial-use
\ \ \ \ \ \ \ \ \ \ \ \ XmTextShowPosition*@function@trial-use
\ \ \ \ \ \ \ \ \ \ \ \ XmTextXYToPos*@function@trial-use
\ \ \ \ \ \ \ \ \f3Composite\fP@widget@full-use
\ \ \ \ \ \ \ \ \ \ \f3Shell\fP@widget@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \f3OverrideShell\fP@widget@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \f3XmMenuShell\fP@widget@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmCreateMenuShell@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \f3WMShell\fP@widget@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \f3VendorShell\fP@widget@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmGetAtomName@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmInternAtom@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \f3TopLevelShell\fP@widget@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \f3ApplicationShell\fP@widget@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \f3TransientShell\fP@widget@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \f3XmDialogShell\fP@widget@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmCreateDialogShell@function@full-use
\ \ \ \ \ \ \ \ \ \ \f3Constraint\fP@widget@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \f3XmManager\fP@widget@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \f3XmBulletinBoard\fP@widget@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmCreateBulletinBoard@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmCreateBulletinBoardDialog@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \f3XmForm\fP@widget@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmCreateForm@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmCreateFormDialog@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \f3XmMessageBox\fP@widget@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmCreateMessageBox@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmCreateErrorDialog@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmCreateInformationDialog@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmCreateMessageDialog@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmCreateQuestionDialog@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmCreateWarningDialog@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmCreateWorkingDialog@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmMessageBoxGetChild@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \f3XmSelectionBox\fP@widget@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmCreateSelectionBox@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmCreatePromptDialog@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmCreateSelectionDialog@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmSelectionBoxGetChild@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \f3XmCommand\fP@widget@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmCreateCommand@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmCommandAppendValue@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmCommandError@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmCommandGetChild@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmCommandSetValue@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \f3XmFileSelectionBox\fP@widget@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmCreateFileSelectionBox@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmCreateFileSelectionDialog@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmFileSelectionBoxGetChild@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmFileSelectionDoSearch@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \f3XmDrawingArea\fP@widget@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmCreateDrawingArea@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \f3XmFrame\fP@widget@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmCreateFrame@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \f3XmPanedWindow\fP@widget@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmCreatePanedWindow@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \f3XmRowColumn\fP@widget@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmCreateRowColumn@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmCreateMenuBar@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmCreateOptionMenu@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmCreatePopupMenu@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmCreatePulldownMenu@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmCreateRadioBox@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmCreateWorkArea*@function@trial-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmGetMenuCursor@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmGetPostedFromWidget*@function@trial-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmMenuPosition@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmOptionButtonWidget@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmOptionLabelWidget@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmSetMenuCursor@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \f3XmScale\fP@widget@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmCreateScale@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmScaleGetValue@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmScaleSetValue@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \f3XmScrolledWindow\fP@widget@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmCreateScrolledWindow@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmCreateScrolledList@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmCreateScrolledText@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmScrolledWindowSetAreas@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \f3XmMainWindow\fP@widget@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmCreateMainWindow@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmMainWindowSep1@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmMainWindowSep2@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmMainWindowSep3*@function@trial-use
\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ XmMainWindowSetAreas@function@full-use
\ \ \ \ \ \ \f3XmGadget\fP@widget@full-use
\ \ \ \ \ \ \ \ \f3XmArrowButtonGadget\fP@widget@full-use
\ \ \ \ \ \ \ \ \ \ XmCreateArrowButtonGadget@function@full-use
\ \ \ \ \ \ \ \ \f3XmLabelGadget\fP@widget@full-use
\ \ \ \ \ \ \ \ \ \ XmCreateLabelGadget@function@full-use
\ \ \ \ \ \ \ \ \ \ \f3XmCascadeButtonGadget\fP@widget@full-use
\ \ \ \ \ \ \ \ \ \ \ \ XmCascadeButtonGadgetHighlight*@function@trial-use
\ \ \ \ \ \ \ \ \ \ \ \ XmCreateCascadeButtonGadget@function@full-use
\ \ \ \ \ \ \ \ \ \ \f3XmPushButtonGadget\fP@widget@full-use
\ \ \ \ \ \ \ \ \ \ \ \ XmCreatePushButtonGadget@function@full-use
\ \ \ \ \ \ \ \ \ \ \f3XmToggleButtonGadget\fP@widget@full-use
\ \ \ \ \ \ \ \ \ \ \ \ XmCreateToggleButtonGadget@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ XmToggleButtonGadgetGetState@function@full-use
\ \ \ \ \ \ \ \ \ \ \ \ XmToggleButtonGadgetSetState@function@full-use
\ \ \ \ \ \ \ \ \f3XmSeparatorGadget\fP@widget@full-use
\ \ \ \ \ \ \ \ \ \ XmCreateSeparatorGadget@function@full-use
.TE
.in
.br
.ne 4i
.H 3 "Toolkit Functions"
.P
.in 0
.TB "Toolkit Services"
.sp
.TS H
allbox, center, tab(@);
lb lb lb.
Service Name@Service Type@AES Level
_
.TH
.T&
l l l.
XmActivateProtocol@function@trial-use
XmActivateWMProtocol@function@trial-use
XmAddProtocolCallbacks@function@trial-use
XmAddProtocols@function@trial-use
XmAddTabGroup@function@full-use
XmAddWMProtocolCallbacks@function@trial-use
XmAddWMProtocols@function@trial-use
XmClipboardCancelCopy@function@full-use
XmClipboardCopy@function@full-use
XmClipboardCopyByName@function@full-use
XmClipboardEndCopy@function@full-use
XmClipboardEndRetrieve@function@full-use
XmClipboardInquireCount@function@full-use
XmClipboardInquireFormat@function@full-use
XmClipboardInquireLength@function@full-use
XmClipboardInquirePendingItems@function@full-use
XmClipboardLock@function@full-use
XmClipboardRegisterFormat@function@full-use
XmClipboardRetrieve@function@full-use
XmClipboardStartCopy@function@full-use
XmClipboardStartRetrieve@function@full-use
XmClipboardUndoCopy@function@full-use
XmClipboardUnlock@function@full-use
XmClipboardWithdrawFormat@function@full-use
XmConvertUnits@function@trial-use
XmCvtCTToXmString*@function@trial-use
XmCvtXmStringToCT*@function@trial-use
XmDeactivateProtocol@function@trial-use
XmDeactivateWMProtocol@function@trial-use
XmDestroyPixmap@function@full-use
XmFontList*@data type@trial-use
XmFontListAdd@function@trial-use
XmFontListCreate@function@trial-use
XmFontListFree@function@trial-use
XmGetColors*@function@trial-use
XmGetPixmap@function@full-use
XmInstallImage@function@full-use
XmIsMotifWMRunning@function@full-use
XmProcessTraversal@function@trial-use
XmRemoveProtocolCallbacks@function@trial-use
XmRemoveProtocols@function@trial-use
XmRemoveTabGroup@function@full-use
XmRemoveWMProtocolCallbacks@function@trial-use
XmRemoveWMProtocols@function@trial-use
XmResolvePartOffsets@function@full-use
XmSetProtocolHooks@function@trial-use
XmSetWMProtocolHooks@function@trial-use
XmString*@data type@trial-use
XmStringBaseline@function@trial-use
XmStringByteCompare@function@trial-use
XmStringCompare@function@trial-use
XmStringConcat@function@trial-use
XmStringCopy@function@trial-use
XmStringCreate@function@temporary-use
XmStringCreateSimple*@function@temporary-use
XmStringDirection*@data type@trial-use
XmStringDraw@function@trial-use
XmStringDrawImage@function@trial-use
XmStringDrawUnderline@function@trial-use
XmStringEmpty@function@trial-use
XmStringExtent@function@trial-use
XmStringFree@function@trial-use
XmStringHasSubstring*@function@trial-use
XmStringHeight@function@trial-use
XmStringLength@function@trial-use
XmStringLineCount@function@trial-use
XmStringNConcat@function@trial-use
XmStringNCopy@function@trial-use
XmStringSeparatorCreate@function@trial-use
XmStringTable*@data type@trial-use
XmStringWidth@function@trial-use
XmTextPosition*@data type@trial-use
XmTrackingLocate@function@trial-use
XmUninstallImage@function@full-use
XmUpdateDisplay@function@full-use
.TE
.in
.br
.ne 5i
.H 3 "User Interface Language"
.P
.in 0
.TB "User Interface Language Services"
.sp
.TS H
allbox, center, tab(@);
lb lb lb.
Service Name@Service Type@AES Level
_
.TH
.T&
l l l.
MrmCloseHierarchy@function@trial-use
MrmFetchColorLiteral@function@trial-use
MrmFetchIconLiteral@function@trial-use
MrmFetchLiteral@function@trial-use
MrmFetchSetValues@function@trial-use
MrmFetchWidget@function@trial-use
MrmFetchWidgetOverride@function@trial-use
MrmInitialize@function@trial-use
MrmOpenHierarchy@function@trial-use
MrmRegisterClass@function@trial-use
MrmRegisterNames@function@trial-use
MrmRegisterNamesInHierarchy*@function@trial-use
Uil@function@trial-use
.TE
.in
