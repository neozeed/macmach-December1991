\ 
.nr PS 10
.nr VS 12
.nr LL 6.25i
.nr PD 0.12i
.nr PO 1.0i
.fi
.LP
.DS I 4.5i
December 22, 1989
.DE
.LP
Dear X3D Member:
.LP
Version 4.0 of the PEX specification is now available to the public.
If your organization
has a representative to the PEX activity, contact them to obtain the
updated PEX documents.  If not, the documents
can be copied
via anonymous ftp from the machine expo.lcs.mit.edu (Internet number
18.30.0.212).  The following documents are available in PostScript form:
.DS
.ta 2.5i
~pub/PEX/doc/v4_cover.ps		This cover letter
~pub/PEX/doc/v4_proto.ps	PEX Protocol Specification
~pub/PEX/doc/v4_proto_toc.ps	PEX Protocol Specification table of contents
~pub/PEX/doc/v4_encoding.ps	PEX Protocol Encoding document
~pub/PEX/doc/v4_encoding_toc.ps	PEX Protocol Encoding table of contents
.DE
.LP
These documents are being released for public review and comment.
To report minor problems
with the documents send mail to Randi Rost, the PEX document editor, at
"rost@decwrl.dec.com".  More significant questions or comments may
be directed at the mailing list "pex-spec@expo.lcs.mit.edu".  I'd like
to thank Sally C. Barry and Andy Vesper of Digital Equipment Corp., 
Lisa Chabot and Cheryl Huntington of Sun Microsystems, and Jan Hardenbergh
of Hewlett-Packard Company's Apollo division for taking the time to
review the previous draft and report problems and errors, thereby
helping to create an even better set of specification documents.
.LP
We also would like to take this opportunity to advise people about those
areas of PEX that will be receiving attention in upcoming months.
The following updates are anticipated:
.IP \(bu
The Protocol Encoding document will be updated to allow for FLOATs that
are longer than four bytes.
.IP \(bu
More thought will be given to the best method of supporting international
text fonts in PEX.  We are asking that people
with expertise in this area contribute to our discussion so that we can
come up with a workable and worthwhile solution to this problem.
.IP \(bu
We will continue to examine PHIGS+ changes and decide their impact on
PEX on a case-by-case basis as they occur.  We have discussed differences
between PEX and PHIGS+ and will try to communicate the reasons for the
differences to the PHIGS+ committee so that they may consider changes
to PHIGS+ as necessary.
.IP \(bu
We will be adding support for the PHIGS+ "Set of Fill Area Set 3 With Data"
(formerly "Polyhedron") in the next version of PEX, unless it destabilizes
in the PHIGS+ committee.
.IP \(bu
We will be adding support for the PHIGS+ "Set Rendering Colour Model"
in the next version of PEX, unless it destabilizes
in the PHIGS+ committee.
.IP \(bu
The bitmask containing ASF flags and the bitmask for pipeline context
attributes are very nearly fully utilized.  If additional ASF flags or
pipeline context attributes have to be added, a non-upwards-compatible
protocol change will have to be made.
.sp 2
.ti 4.5i
.nf
Randi Rost

.ti 4.5i
.nf
PEX Document Editor
