.\"
.\"
.\"  This macro puts the section numbers, labels, and page numbers out
.\"   to the standard output.   aps, crw, rjr.
.\"   WARNING: This macro assumes certain knowledge about the the ms/mu
.\"   packages work (what number and string registers are used, to be exact).
.\"
.de AC
.NH \\$2
\\$1
.\"
.\"  This indents section heading of level two or greater.
.\"
.tm .Bg \\n(NS
.tm \\*(SN \\$1
.tm .Ed \\n%
..

.de Ao
.PN \\$1
.\"
.\"  This indents section heading of level two or greater.
.\"
.tm .Bg 3
.tm \\$1
.tm .Ed \\n%
..

.de RU
.br
\l'6.5i'
.sp
..

.de AR
.IP \\$1 1.0i
..

.de Sh		\" start a section (chapter type)
.bp
.NH 1
\fB\\$1\fP
.tm .Nh "\\$1" "\\n%" "0" "\\n(H1" "\\n(H2" "\\n(H3"	 \" zero is for chapter,group
..

.de Nh		\" Start a new section
.ds RH \\$1
.nr In 0 1
.ds Ic \\$1
.nr Ac 0 1
.LP
.NH 2
\\$1
.tm .Nh "\\$1" "\\n%" "1" "\\n(H1" "\\n(H2" "\\n(H3"   	\" one is for sub-chapter
..

.de Fs		\" Function Start
..
.de Nn		\" Start name of function
.NH 3
\\$1
..

.de Na		\" Start name of function
.sp
.LP
.in 0.5i
.ti -0.5i
\fBName:\fP 
.ti 0.5i
\fB\\$1\fP
..

.de Or		\" name of output request
.sp -1
.LP
\fB\\$1\fP
.tm .Bg 3
.tm \\$1
.tm .Ed \\n%
..

.de Op		\" add an output request parameter
.br
.RS
\fI\\$1 : \fP  \\$2
.RE
..

.de Pa		\" add a parameter
.ti 0.5i
\fI\\$1 : \fP  \\$2
..

.de Rq		\" Start Request list
.LP
.ti -0.5i
\fBRequest:\fP
..

.de Re		\" Start Reply list
.LP
.ti -0.5i
\fBReply:\fP
..

.de Se		\" Start ERRORS list
.LP
.ti -0.5i
\fBErrors:\fP
.ti 0.5i
..

.de Fe		\" End of Function
.in 0i
..

.de Bl		\" Start of bullet item
.sp -1
.IP "\fI\\$1\fP"
..
                                      
.de 2d		\" Description of 2d primtives
When processed, this command will cause \\$1 primitives to be drawn.
This primitive
functions exactly as the 3D \\$1 primitive
except that modeling
coordinate positions are specified using only \fIx-\fP and \fIy-\fP
coordinates, and the \fIz\fP-coordinate is always assumed to be zero.
..

.de Es		\" Start of enumerated type description table
.LD
.ta 0.2i 1.7i
..

.de Ee		\" End of enumerated type description table
.ta
.DE
..

.de Ds
.nf
.\\$1D \\$2 \\$1
.ft 1
.ps \\n(PS
.if \\n(VS>=40 .vs \\n(VSu
.if \\n(VS<=39 .vs \\n(VSp
..

.de De
.ce 0
.if \\n(BD .DF
.nr BD 0
.in \\n(OIu
.if \\n(TM .ls 2
.sp \\n(DDu
.fi
..

.de Pn
.IN \\$2
.ie t \\$1\fB\^\\$2\^\fR\\$3
.el \\$1\fI\^\\$2\^\fP\\$3
..

.de PN
.IN \\$1
.ie t \fB\^\\$1\^\fR\\$2
.el \fI\^\\$1\^\fP\\$2
..
.nr LL 6.5i
.nr PD 0.1i
.nr HM 1.2i
.nr FM 1.0i
.nr PO 1.0i
.nh
.LP
\ 
.sp 12
.PP
.TL
\fB\s+9PEX Protocol Encoding \s-9\fP
.sp 1
.AU
.mc |
Version 4.0P - \fBPUBLIC REVIEW DRAFT\fP
22-December-1989
.mc
.sp 1.0i
.nf
.ce
\fB\s+2Author\s-2\fP
.sp 0.05i
.ce 5
.R
Todd Newman
Digital Equipment Corporation
Workstation Systems Engineering
100 Hamilton Avenue
Palo Alto, CA 94301
.sp 0.5i
.ce
\fB\s+2Document Editor\s-2\fP
.sp 0.05i
.ce 5
.R
Randi J. Rost
Digital Equipment Corporation
Workstation Systems Engineering
100 Hamilton Avenue
Palo Alto, CA 94301
.ta
.bp 1
\ 
.LP
.sp 10
This document is a companion to the \fIPEX Protocol Specification\fP document,
.mc |
Version 4.0P PUBLIC REVIEW DRAFT,
.mc
and the sections in this document are numbered to match.
.ps 9
.nr PS 9
.vs 10
.nr VS 10
.sp 24
.LP
.nf
.ce 2
Copyright \(co 1988, 1989, Massachusetts Institute of Technology
All rights reserved.
.LP
.LP
Permission to use, copy, modify, and distribute this document for any purpose
and without fee is hereby granted, provided that the above copyright notice
appear in all copies and that both that copyright notice and this permission
notice are retained, and that the name of M.I.T. not be used in advertising or
publicity pertaining to this document without specific, written prior
permission.  M.I.T. makes no representations about the suitability of this
document or the protocol defined in this document for any purpose.  It is
provided "as is" without express or implied warranty.
.bp
.ps 10
.nr PS 10
.vs 12
.nr VS 12
.ad l
.mc |
.EH '\s+2\fBPEX V4.0P 22-December-1989\fP''\fBPEX Protocol Encoding\fP\s-2'
.OH '\s+2\fBPEX Protocol Encoding\fP''\fBPEX V4.0P 22-December-1989\fP\s-2'
.mc
.EF ''\s+2\fB % \fP\s-2''
.OF ''\s+2\fB % \fP\s-2''

.AC Acknowledgements 1
.LP
Randi Rost and Jeff Friedberg of Digital Equipment Corp. helped in
the production of the first draft of this document by explaining
the original intent of the architecture team.
Sally C. Barry of Digital Equipment Corp. has been an extremely
patient and thorough reviewer of this document throughout its many
revisions.  Sally also was responsible for converting
this document to its present typeset form.
.bp

.AC "PEX Protocol Specification" 1
.LP
.FS
.br
.sp
X Window System is a trademark of the
Massachusetts Institute of Technology

X3D-PEX and PEX are trademarks of the 
Massachusetts Institute of Technology
.FE

.AC "Protocol Format" 2
.LP

.AC "Request Format" 3
.LP
Requests are listed in section 2.7 and in section 4 through the end of the
document.  Output Commands are described in section 3.

.AC "Reply Format" 3
.LP
Replies are listed after the request.

.AC "Error Format" 3
.LP
See section 2.4.7 for the encoding of particular errors.

.AC "Event Format" 3
.LP
PEX defines no new events.

.AC "Syntax" 2
.LP
As far as possible, this document uses the layout and syntactic conventions used
in the X encoding document.  In particular, note that all numbers are decimal,
unless prefixed by #x, in which case they are hexadecimal.  Also note that, for
entities of a variable size, E, the notation pad(E) indicates the number of
bytes required to pad the entity to a multiple of 4 bytes.
.LP
There are some unavoidable differences from the X encoding document
conventions, however, because of the many variable sized entities and entities
with optional data in PEX.

.AC "Naming Conventions" 2
.LP
The names of the fields in requests, replies and lists of values match the
names used in the \fIPEX Protocol Specification\fP.

.AC "Common Types" 2
.LP
The types listed in this section are some of the types mentioned in
the PEX protocol.  Generally, derivative types (e.g. ENUM_TYPE_INDEX)
are replaced by the primitive or primitives of which they are composed
(e.g. INT16).  Only when this is impractical is the derivative type
used; generally this is when the type is of variable length or a
list of compound types is used.

.AC "LISTofFOO" 3
.LP
In this document, a LISTofFOO is a counted list of elements of type FOO,
the count field preceding it in the request or reply.
.LP
In cases where the number of items in the list is easily computed from the
length of the list, the number of items may not be supplied. (See, for
example, the \fBPolyline3D\fP output command.)  The goal is to reduce the
burden on the client. Such cases are indicated in the encoding document.
.LP
Unless otherwise marked, a LISTofLISTofFOO is implemented as follows:
somewhere in the request or reply there is a number of lists field (perhaps
several if there are multiple lists of lists). Each LIST is preceded by
a CARD32 count field.
.LP
So the overall format is:
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
	4	CARD32		number of lists
	. . .			(potentially other things here)
	4	CARD32		length of first list
	n	FOO		first item of first list
	n	FOO		second item of first list
	. . .
	n	FOO		last item of first list
	4	CARD32		length of second list
	n	FOO		first item of second list
	. . .
.De

.AC "BITMASK and LISTofVALUE" 3
.LP
Because the values in a value list are of variable size, a list of values
can only be interpreted serially; there is no way to find the Nth element
without examining the N-1 previous elements.
.LP
Elements have all been padded to a multiple of 4 bytes.  If values appear
which could be encoded in less space, it is assumed that they will be
placed in the least significant bits of the word.  (Byte-swapping may occur
between the client and the server.)

.AC "Floating Point Format - FLOAT" 3
.LP
A FLOAT is a 4 byte quantity in one of the server supported formats.  If
the client wishes to communicate with the server in other than the server's
native floating point format, the server must perform the necessary
conversions.  The format word of a request tells the server what format the
client wishes to use.

.AC "Colors" 3
.LP
.mc |
Colors passed to the server may be of any one of the registered
color types:
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.nf
		0	Indexed
		1	RGBFloat
		2	CIEFloat
		3	HSVFloat
		4	HLSFloat
		5	RGBInt8
		6	RGBInt16
.fi
Equations are provided for computing the length of the requests that
contain colors, however these equations assume that one of the seven
registered color types is being used.
.mc

.AC "Element Types" 3
.LP

.AC "Types" 3
.LP
Some PEX requests and replies contain matrices.  These are denoted in this
document as FLOAT[4][4] or FLOAT[3][3].  The elements of
the matrices are sent over the wire in a row-wise manner.  That is, the element
of a 4\(mu4 transformation matrix that indicates the
translation in the X direction is the fourth element sent.
.LP
The following compound types appear in the rest of the encoding document.
If a variable is given which computes the length of a compound type (e.g.,
the equation for f in FACET), that variable may be used later to express
the length of a request or reply (e.g. the \fBFillArea3DWithData\fP output
command).
.sp 1
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
COLOR
	n = ((color type == Indexed)||(color type == RGBInt8) ? 1 : \
((color type == RGBInt16) ? 2 : 3)
	(color type is obtained from elsewhere in the request or output command)
	4n			color value
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
COLOR_SPECIFIER
	n = ((color type == Indexed)||(color type == RGBInt8) ? 1 : \
((color type == RGBInt16) ? 2 : 3)
	2			color type
.mc *
.mc
	2			unused
	4n			color value
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
COORD_2D
	4	FLOAT		x
	4	FLOAT		y
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
COORD_3D
	4	FLOAT		x
	4	FLOAT		y
	4	FLOAT		z
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
COORD_4D
	4	FLOAT		x
	4	FLOAT		y
	4	FLOAT		z
	4	FLOAT		w
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
DEVICE_COORD
	2	INT16		x
	2	INT16		y
	4	FLOAT		z
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
DEVICE_RECT
	2	INT16		xmin
	2	INT16		ymin
	2	INT16		xmax
	2	INT16		ymax
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
DIRECT_COLOR
	n = ((color_type == RGBInt8) ? 1 : ((color_type == RGBInt16) ? 2 : 3)
	4n	DIRECT_COLOR		color
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
ELEMENT_REF
	4	CARD32		structure ID
	4	CARD32		offset
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
EXTENT_INFO
	4	FLOAT		lower left x
	4	FLOAT		lower left y
	4	FLOAT		upper right x
	4	FLOAT		upper right y
	4	FLOAT		concatpoint x
	4	FLOAT		concatpoint y
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
FACET
	(maximum size, some fields may be missing, see below)
	df	OPT_DATA		facet data (c and n may be set, but s is never set)
	4	nv		number of vertices in list
	nv(12+dv) LISTofVERTEX		vertices
.sp 1
	(let f be the size of a FACET in bytes.
	let df be the size of the per facet OPT_DATA.
	let dv be the size of the per vertex OPT_DATA.
	fc is true if facet attributes specifies a color is present.
	fn is true if facet attributes specifies a normal is present.
	let sc be the size of a direct color in bytes.
	sc = (color type == RGBInt8) ? 4 : ((color type == (RGBInt16) ? 8 : 12)
	fi is true if facet color is indexed, false if color is direct. Then
	df = (fc ? (fi ? 4 : sc) : 0 ) + (fn ? 12 : 0)
	vc is true if vertex attributes specifies a color is present.
	vn is true if vertex attributes specifies a normal is present.
	ve is true if vertex attributes specifies an edge flag is present.
	vi is true if vertex color is indexed, false if color is direct. Then
	dv = (vc ? (vi ? 4 : sc) : 0 ) + (vn ? 12 : 0) + (ve ? 4 : 0)
	so f = 4 + df + nv * (12 + dv)
	)
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
FONTINFO
	2	CARD16		first glyph
	2	CARD16		last glyph
	1			all glyphs exist
		0	False
		1	True
	1			unused
	2	CARD16		default glyph
	4	n		number of properties
	8n	LISTofFONTPROP		properties
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
FONTPROP
	4	CARD32		name
	4	FLOAT | CARD32		value
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
HALF_SPACE
	4	FLOAT		point x
	4	FLOAT		point y
	4	FLOAT		point z
	4	FLOAT		vector x
	4	FLOAT		vector y
	4	FLOAT		vector z
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
HALF_SPACE_2D
	4	FLOAT		point x
	4	FLOAT		point y
	4	FLOAT		vector x
	4	FLOAT		vector y
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
NAME_SET_PAIR
	4	CARD32		inclusion nameset ID
	4	CARD32		exclusion nameset ID
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
OPT_DATA
	(maximum size, some fields may be missing, see below)
	sc = ((color type == Indexed)||(color type == RGBInt8)) ? \
1 : ((color type == RGBInt16) ? 2 : 3)
	4sc			color
	4	FLOAT		normal x
	4	FLOAT		normal y
	4	FLOAT		normal z
	4	CARD32		edges
.sp 1
	(let d be the size of an OPT_DATA in bytes.
	c is true if control word specifies a color is present.
	n is true if control word specifies a normal is present.
	s is true if control word specifies an edge switch is present.
	i is true if color is indexed, false if color is direct.
	f is 1 if color type is RGBInt8
		2 if color type is RGBInt16
		3 otherwise.
	Then d = (c ? ((i || (f == 1)) ? 4 : ((f == 2) ? 8 : 12)) : 0) \
	+ (n ? 12 : 0) + (s ? 4 : 0)
	)
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
PICK_ELEMENT_REF
	4	CARD32		structure ID
	4	CARD32		offset
	4	CARD32		pick identifier
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
STRING
	2	n		length
	n	CARD8		string
	p			unused, p=pad(n)
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
STRUCTURE_INFO
	4	CARD32		structure ID
	4	FLOAT		priority
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
TRIM_CURVE
	1			visibility
		0	Off
		1	On
	3			unused
	2	CARD16		order
	2			type
		0	Rational
.mc |
		1	NonRational	(t = (type == Rational) ? 3 : 2)
.mc
	4	FLOAT		tmin
	4	FLOAT		tmax
	4	k		number of knots
	4	p		number of points
	4k	LISTofFLOAT		knots
.mc |
	4p*t	LISTofCOORD		points (COORD_3D or COORD_2D depending on t)
.mc

	(let c be the size of a TRIM_CURVE in bytes.)
	(c = 24 + 4 * (k + p*t))
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
VERTEX
	4	FLOAT		point x
	4	FLOAT		point y
	4	FLOAT		point z
	d	OPT_DATA		data (see OPT_DATA for d)

	(let v be the size of a VERTEX in bytes.)
	(v = 12 + d)
.De
.LP
.Ds 0
.TA .2i .45i .6i 1.6i 2.6i
.ta .2i .45i .6i 1.6i 2.6i
.R
VERTEX_INDEX
	2		CARD16		vertex
	e				edge (e == 0 if edge flag isn't set, e == 2 otherwise)
		0	Off
		1	On
.De

.AC "Errors" 3
.LP
\fIBEC\fR is the base error code for the extension, as returned by
\fBQueryExtension\fP.
.LP
.Ds 0
.TA .2i .5i 1.6i
.ta .2i .5i 1.6i
.R
.PN ColorType
	1	0	Error
	1	\fIBEC\fR+0	code
	2	CARD16	sequence number
	4	CARD32	bad color type
	2	CARD16	minor opcode
	1	CARD8	major opcode
	21		unused
.De
.LP
.Ds 0
.TA .2i .5i 1.6i
.ta .2i .5i 1.6i
.R
.PN RendererState
	1	0	Error
	1	\fIBEC\fR+1	code
	2	CARD16	sequence number
	4	CARD32	renderer ID
	2	CARD16	minor opcode
	1	CARD8	major opcode
	21		unused
.De
.LP
.Ds 0
.TA .2i .5i 1.6i
.ta .2i .5i 1.6i
.R
.PN FloatingPointFormat
	1	0	Error
	1	\fIBEC\fR+2	code
	2	CARD16	sequence number
	4	CARD32	bad format
	2	CARD16	minor opcode
	1	CARD8	major opcode
	21		unused
.De
.LP
.Ds 0
.TA .2i .5i 1.6i
.ta .2i .5i 1.6i
.R
.PN Label
	1	0	Error
	1	\fIBEC\fR+3	code
	2	CARD16	sequence number
	4	CARD32	bad label
	2	CARD16	minor opcode
	1	CARD8	major opcode
	21		unused
.De
.LP
.Ds 0
.TA .2i .5i 1.6i
.ta .2i .5i 1.6i
.R
.PN LookupTable
	1	0	Error
	1	\fIBEC\fR+4	code
	2	CARD16	sequence number
	4	CARD32	bad table ID
	2	CARD16	minor opcode
	1	CARD8	major opcode
	21		unused
.De
.LP
.Ds 0
.TA .2i .5i 1.6i
.ta .2i .5i 1.6i
.R
.PN Nameset
	1	0	Error
	1	\fIBEC\fR+5	code
	2	CARD16	sequence number
	4	CARD32	bad nameset ID
	2	CARD16	minor opcode
	1	CARD8	major opcode
	21		unused
.De
.LP
.Ds 0
.TA .2i .5i 1.6i
.ta .2i .5i 1.6i
.R
.PN Path
	1	0	Error
	1	\fIBEC\fR+6	code
	2	CARD16	sequence number
	4	CARD32	bad path ID
	2	CARD16	minor opcode
	1	CARD8	major opcode
	21		unused
.De
.LP
.Ds 0
.TA .2i .5i 1.6i
.ta .2i .5i 1.6i
.R
.PN PEXFont
	1	0	Error
	1	\fIBEC\fR+7	code
	2	CARD16	sequence number
	4	CARD32	bad font ID
	2	CARD16	minor opcode
	1	CARD8	major opcode
	21		unused
.De
.LP
.Ds 0
.TA .2i .5i 1.6i
.ta .2i .5i 1.6i
.R
.PN PhigsWKS
	1	0	Error
	1	\fIBEC\fR+8	code
	2	CARD16	sequence number
	4	CARD32	bad PHIGS workstation ID
	2	CARD16	minor opcode
	1	CARD8	major opcode
	21		unused
.De
.LP
.Ds 0
.TA .2i .5i 1.6i
.ta .2i .5i 1.6i
.R
.PN PickMeasure
	1	0	Error
	1	\fIBEC\fR+9	code
	2	CARD16	sequence number
	4	CARD32	bad device ID
	2	CARD16	minor opcode
	1	CARD8	major opcode
	21		unused
.De
.LP
.Ds 0
.TA .2i .5i 1.6i
.ta .2i .5i 1.6i
.R
.PN PipelineContext
	1	0	Error
	1	\fIBEC\fR+10	code
	2	CARD16	sequence number
	4	CARD32	bad pipeline context ID
	2	CARD16	minor opcode
	1	CARD8	major opcode
	21		unused
.De
.LP
.Ds 0
.TA .2i .5i 1.6i
.ta .2i .5i 1.6i
.R
.PN Renderer
	1	0	Error
	1	\fIBEC\fR+11	code
	2	CARD16	sequence number
	4	CARD32	bad renderer ID
	2	CARD16	minor opcode
	1	CARD8	major opcode
	21		unused
.De
.LP
.Ds 0
.TA .2i .5i 1.6i
.ta .2i .5i 1.6i
.R
.PN SearchContext
	1	0	Error
	1	\fIBEC\fR+12	code
	2	CARD16	sequence number
	4	CARD32	bad search context ID
	2	CARD16	minor opcode
	1	CARD8	major opcode
	21		unused
.De
.LP
.Ds 0
.TA .2i .5i 1.6i
.ta .2i .5i 1.6i
.R
.PN Structure
	1	0	Error
	1	\fIBEC\fR+13	code
	2	CARD16	sequence number
	4	CARD32	bad structure ID
	2	CARD16	minor opcode
	1	CARD8	major opcode
	21		unused
.De
.LP
.Ds 0
.TA .2i .5i 1.6i
.ta .2i .5i 1.6i
.R
.PN OutputCommand
	1	0	Error
	1	\fIBEC\fR+14	code
	2	CARD16	sequence number
	4	CARD32	renderer ID or structure ID
	2	CARD16	minor opcode
	1	CARD8	major opcode
	1	CARD8	unused
	2	CARD16	opcode of output command
	2	CARD16	number of output commands in the request that were processed successfully
	16		unused
.De

.mc |
.AC "Resource IDs" 3
.LP
Some resources have attributes that contain IDs of other resources.  It
is possible for the referenced resource to be freed while the referencing
resource remains in use.  Attempts to query attributes containing the resource
ID of a resource that has already been freed will return the value 
\fIAlreadyFreed\fP (whose value equals one).

.mc
.AC "Events" 2
.LP

.AC "Padding" 2
.LP
The contents of pad bytes is explicitly left undefined.
In some cases, the length of a field is followed by its type
which is shorter than the stated length of the field (e.g., "distinguish 4
CARD8").  This implies that a single CARD8 value is followed by three
unused bytes.  It is simply a shorthand to avoid having lots of "unused"
fields in certain structure descriptions.

.AC "Extension Information" 2
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXGetExtensionInfo
	1	CARD8		opcode (X11 assigned)
	1	1		PEX opcode
	2	2		request length
	2	CARD16		client protocol major version
	2	CARD16		client protocol minor version
.De
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
=>
	1	1		Reply
	1 			unused
	2	CARD16		sequence number
	4	(n+p)/4		reply length
	2	CARD16		protocol major version
	2	CARD16		protocol minor version
	4	CARD32		release number
	4	n		length of vendor name
	4	CARD32		subset info
	8			unused
	n	LISTofCARD8		vendor
	p			unused, p=pad(n)
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXGetEnumeratedTypeInfo
	1	CARD8		opcode (X11 assigned)
	1	2		PEX opcode
	2	4+(2n+p)/4		request length
	4	CARD32		drawable ID
	4			item mask
		1	index
		2	mnemonic
	4	n		number of enumerated types
	2n	LISTofCARD16		enum types
		1	MarkerType
		2	ATextStyle
		3	InteriorStyle
		4	HatchStyle
		5	LineType
		6	SurfaceEdgeType
		7	PickDeviceType
		8	PolylineInterpMethod
		9	CurveApproxMethod
.mc |
		10	ReflectionModel
		11	SurfaceInterpMethod
		12	SurfaceApproxMethod
		13	ModelClipOperator
		14	LightType
		15	ColorType
		16	FloatFormat
		17	HLHSRMode
		18	PromptEchoType
		19	DisplayUpdateMode
		20	ColorApproxType
		21	ColorApproxModel
		22	GDP
		23	GDP3
		24	GSE
		25	TrimCurveApproxMethod
.mc
	p			unused, p=pad(2n)
.De
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
=>
	1	1		Reply
	1			unused
	2	CARD16		sequence number
	4	(q+p)/4		reply length
	4	CARD32		number of lists
	20			unused
	q	LISTofLISTofVALUE		types
	p			unused, p=pad(q)
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXGetImpDepConstants
	1	CARD8		opcode (X11 assigned)
	1	3		PEX opcode
	2	4+(2n+p)/4		request length
	2	INT16		floating point format
	2			unused
	4	CARD32		example drawable ID
	4	n		number of names
	2n	LISTofNAMES		names
	p			unused, p=pad(2n)
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
=>
	1	1		Reply
	1			unused
	2	CARD16		sequence number
	4	n		reply length
	24			unused
	4n	LISTofVALUE		constants (length implicit)
.De
Names for \fBPEXGetImpDepConstants\fP:
All names are 16 bit constants, returns CARD32, except for the
names \fIChromaticityRedU, ChromaticityRedV, LuminanceRed,
ChromaticityGreenU, ChromaticityGreenV, LuminanceGreen,
ChromaticityBlueU, ChromaticityBlueV, LuminanceBlue,
ChromaticityWhiteU, ChromaticityWhiteV,\fP and
\fILuminanceWhite\fP, each of which returns a FLOAT.
.sp 1
.Ds 0
.TA .2i .6i 1.6i 2.1i
.ta .2i .6i 1.6i 2.1i
.R
	Name			Value
.sp 1
	DitheringSupported			4
	MaxEdgeWidth			5
	MaxLineWidth			6
	MaxMarkerSize			7
	MaxModelClipPlanes			8
	MaxNameSetNames			9
	MaxNonAmbientLights			10
	MaxNURBOrder			11
	MaxTrimCurveOrder			12
.mc |
	MinEdgeWidth			13
	MinLineWidth			14
	MinMarkerSize			15
	NominalEdgeWidth			16
	NominalLineWidth			17
	NominalMarkerSize			18
	NumSupportedEdgeWidths			19
	NumSupportedLineWidths			20
	NumSupportedMarkerSizes			21
	BestColorApproxValues			22
.mc
		0	ColorApproxAnyValues
		1	ColorApproxPowersOf2
.mc |
	TransparencySupported			23
	ChromaticityRedU			24
	ChromaticityRedV			25
	LuminanceRed			26
	ChromaticityGreenU			27
	ChromaticityGreenV			28
	LuminanceGreen			29
	ChromaticityBlueU			30
	ChromaticityBlueV			31
	LuminanceBlue			32
	ChromaticityWhiteU			33
	ChromaticityWhiteV			34
	LuminanceWhite			35
.mc
.De
.bp

.AC "Output Commands" 1
.LP

.AC "Data Formats" 2
.LP

.AC "Output Command Descriptions" 2
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao MarkerType 3
	2	1		output command number
	2	2		output command length
	2	INT16		marker type
	2			unused
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao MarkerScale 3
	2	2		output command number
	2	2		output command length
	4	FLOAT		scale
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao MarkerColorIndex 3
	2	3		output command number
	2	2		output command length
	2	CARD16		indexed color
	2			unused
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao MarkerColor 3
	2	4		output command number
	2	2+n		output command length
	4+4n	COLOR_SPECIFIER	color
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao MarkerBundleIndex 3
	2	5		output command number
	2	2		output command length
	2	CARD16		marker bundle index
	2			unused
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao TextFontIndex 3
	2	6		output command number
	2	2		output command length
	2	CARD16		font index
	2			unused
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao TextPrecision 3
	2	7		output command number
	2	2		output command length
	2			precision
		0	StringPrecision
		1	CharPrecision
		2	StrokePrecision
	2			unused
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao CharacterExpansion 3
	2	8		output command number
	2	2		output command length
	4	FLOAT		expansion
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao CharacterSpacing 3
	2	9		output command number
	2	2		output command length
	4	FLOAT		spacing
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao TextColorIndex 3
	2	10		output command number
	2	2		output command length
	2	CARD16		color index
	2			unused
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao TextColor 3
	2	11		output command number
	2	2+n		output command length
	4+4n	COLOR_SPECIFIER	color
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao CharacterHeight 3
	2	12		output command number
	2	2		output command length
	4	FLOAT		height
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao CharacterUpVector 3
	2	13		output command number
	2	3		output command length
	4	FLOAT		up x
	4	FLOAT		up y
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao TextPath 3
	2	14		output command number
	2	2		output command length
	2			path
		0	PathRight
		1	PathLeft
		2	PathUp
		3	PathDown
	2			unused
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao TextAlignment 3
	2	15		output command number
	2	2		output command length
	2			alignment vertical
		0	ValignNormal
		1	ValignTop
		2	ValignCap
		3	ValignHalf
		4	ValignBase
		5	ValignBottom
	2			alignment horizontal
		0	HalignNormal
		1	HalignLeft
		2	HalignCenter
		3	HalignRight
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao AnnotationTextHeight 3
	2	16		output command number
	2	2		output command length
	4	FLOAT		height
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao AnnotationTextUpVector 3
	2	17		output command number
	2	3		output command length
	4	FLOAT		up x
	4	FLOAT		up y
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao AnnotationTextPath 3
	2	18		output command number
	2	2		output command length
	2			path
		0	PathRight
		1	PathLeft
		2	PathUp
		3	PathDown
	2			unused
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao AnnotationTextAlignment 3
	2	19		output command number
	2	2		output command length
	2			alignment vertical
		0	ValignNormal
		1	ValignTop
		2	ValignCap
		3	ValignHalf
		4	ValignBase
		5	ValignBottom
	2			alignment horizontal
		0	HalignNormal
		1	HalignLeft
		2	HalignCenter
		3	HalignRight
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao AnnotationTextStyle 3
	2	20		output command number
	2	2		output command length
	2	INT16		annotation text style
	2			unused
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao TextBundleIndex 3
	2	21		output command number
	2	2		output command length
	2	CARD16		text bundle index
	2			unused
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao LineType 3
	2	22		output command number
	2	2		output command length
	2	INT16		line type
	2			unused
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao LineWidth 3
	2	23		output command number
	2	2		output command length
	4	FLOAT		width
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao LineColorIndex 3
	2	24		output command number
	2	2		output command length
	2	CARD16		color index
	2			unused
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao LineColor 3
	2	25		output command number
	2	2+n		output command length
	4+4n	COLOR_SPECIFIER	color
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao CurveApproximationMethod 3
	2	26		output command number
	2	3		output command length
	2	INT16		approximation method
	2			unused
	4	FLOAT		approximation tolerance
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao PolylineInterpolationMethod 3
	2	27		output command number
	2	2		output command length
	2	INT16		polyline interpolation method
	2			unused
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao LineBundleIndex 3
	2	28		output command number
	2	2		output command length
	2	CARD16		line bundle index
	2			unused
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao SurfaceInteriorStyle 3
	2	29		output command number
	2	2		output command length
	2	INT16		interior style
	2			unused
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao SurfaceInteriorStyleIndex 3
	2	30		output command number
	2	2		output command length
	2	CARD16 | INT16		table index or interior style
	2			unused
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao SurfaceColorIndex 3
	2	31		output command number
	2	2		output command length
	2	CARD16		color index
	2			unused
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao SurfaceColor 3
	2	32		output command number
	2	2+n		output command length
	4+4n	COLOR_SPECIFIER	color
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao SurfaceReflectionAttributes 3
	2	33		output command number
	2	7+n		output command length
	4	FLOAT		ambient  coefficient
	4	FLOAT		diffuse coefficient
	4	FLOAT		specular coefficient
	4	FLOAT		specular concentration
	4	FLOAT		transmission coefficient
	4+4n	COLOR_SPECIFIER	specular color
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao SurfaceReflectionModel 3
	2	34		output command number
	2	2		output command length
	2	INT16		reflection model
	2			unused
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao SurfaceInterpolationMethod 3
	2	35		output command number
	2	2		output command length
	2	INT16		surface interpolation method
	2			unused
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao BackfaceSurfaceInteriorStyle 3
	2	36		output command number
	2	2		output command length
	2	INT16		interior style
	2			unused
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao BackfaceSurfaceInteriorStyleIndex 3
	2	37		output command number
	2	2		output command length
	2	CARD16 | INT16		table index or interior style
	2			unused
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao BackfaceSurfaceColorIndex 3
	2	38		output command number
	2	2		output command length
	2	CARD16		color index
	2			unused
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao BackfaceSurfaceColor 3
	2	39		output command number
	2	2+n		output command length
	4+4n	COLOR_SPECIFIER	color
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao BackfaceSurfaceReflectionAttributes 3
	2	40		output command number
	2	7+n		output command length
	4	FLOAT		ambient coefficient
	4	FLOAT		diffuse coefficient
	4	FLOAT		specular coefficient
	4	FLOAT		specular concentration
	4	FLOAT		transmission coefficient
	4+4n	COLOR_SPECIFIER		specular color
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao BackfaceSurfaceReflectionModel 3
	2	41		output command number
	2	2		output command length
	2	INT16		reflection model
	2			unused
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao BackfaceSurfaceInterpolationMethod 3
	2	42		output command number
	2	2		output command length
	2	INT16		surface interpolation method
	2			unused
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao SurfaceApproximation 3
	2	43		output command number
	2	4		output command length
	2	INT16		surface approximation method
	2			unused
	4	FLOAT		approximation u tolerance
	4	FLOAT		approximation v tolerance
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao TrimCurveApproximation 3
	2	44		output command number
	2	3		output command length
	2	INT16		trim curve approximation method
	2			unused
	4	FLOAT		approximation tolerance
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao FacetCullingMode 3
	2	45		output command number
	2	2		output command length
	2			culling mode
		0	None
		1	BackFaces
		2	FrontFaces
	2			unused
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao FacetDistinguishFlag 3
	2	46		output command number
	2	2		output command length
	1			distinguish
		0	False
		1	True
	3			unused
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao NormalReorientationMode 3
	2	47		output command number
	2	2		output command length
	1			normalflip
		0	False
		1	True
	3			unused
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao PatternSize 3
	2	48		output command number
	2	3		output command length
	4	FLOAT		size x
	4	FLOAT		size y
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao PatternReferencePoint 3
	2	49		output command number
	2	3		output command length
	4	FLOAT		point x
	4	FLOAT		point y
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao PatternReferencePointAndVectors 3
	2	50		output command number
	2	10		output command length
	4	FLOAT		reference point x
	4	FLOAT		reference point y
	4	FLOAT		reference point z
	4	FLOAT		vector1 x
	4	FLOAT		vector1 y
	4	FLOAT		vector1 z
	4	FLOAT		vector2 x
	4	FLOAT		vector2 y
	4	FLOAT		vector2 z
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao InteriorBundleIndex 3
	2	51		output command number
	2	2		output command length
	2	CARD16		interior bundle index
	2			unused
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao SurfaceEdgeFlag 3
	2	52		output command number
	2	2		output command length
	1			onoff
		0	Off
		1	On
	3			unused
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao SurfaceEdgeType 3
	2	53		output command number
	2	2		output command length
	2	INT16		edge type
	2			unused
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao SurfaceEdgeWidth 3
	2	54		output command number
	2	2		output command length
	4	FLOAT		width
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao SurfaceEdgeColorIndex 3
	2	55		output command number
	2	2		output command length
	2	CARD16		color index
	2			unused
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao SurfaceEdgeColor 3
	2	56		output command number
	2	2+n		output command length
	4+4n	COLOR_SPECIFIER	color
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao EdgeBundleIndex 3
	2	57		output command number
	2	2		output command length
	2	CARD16		edge bundle index
	2			unused
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao SetIndividualASF 3
	2	58		output command number
	2	3		output command length
	4			ASF attribute to be modified
		#x00000001	MarkerTypeASF
		#x00000002	MarkerScaleASF
		#x00000004	MarkerColorASF
		#x00000008	TextFontIndexASF
		#x00000010	TextPrecASF
		#x00000020	CharExpansionASF
		#x00000040	CharSpacingASF
		#x00000080	TextColorASF
		#x00000100	LineTypeASF
		#x00000200	LineWidthASF
		#x00000400	LineColorASF
		#x00000800	CurveApproxASF
		#x00001000	PolylineInterpASF
		#x00002000	InteriorStyleASF
		#x00004000	InteriorStyleIndexASF
		#x00008000	SurfaceColorASF
		#x00010000	SurfaceInterpASF
		#x00020000	ReflectionModelASF
		#x00040000	ReflectionAttrASF
		#x00080000	BfInteriorStyleASF
		#x00100000	BfInteriorStyleIndexASF
		#x00200000	BfSurfaceColorASF
		#x00400000	BfSurfaceInterpASF
		#x00800000	BfReflectionModelASF
		#x01000000	BfReflectionAttrASF
		#x02000000	SurfaceApproxASF
		#x04000000	TrimCurveApproxASF
		#x08000000	SurfaceEdgesASF
		#x10000000	SurfaceEdgeTypeASF
		#x20000000	SurfaceEdgeWidthASF
		#x40000000	SurfaceEdgeColorASF
	1			new ASF value
		0	Bundled
		1	Individual
	3			unused
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao LocalTransform3D 3
	2	59		output command number
	2	18		output command length
	2			composition type
		0	PreConcatenate
		1	PostConcatenate
		2	Replace
	2			unused
	64	FLOAT[4][4]		matrix
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao LocalTransform2D 3
	2	60		output command number
	2	11		output command length
	2			composition type
		0	PreConcatenate
		1	PostConcatenate
		2	Replace
	2			unused
	36	FLOAT[3][3]		matrix
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao GlobalTransform3D 3
	2	61		output command number
	2	17		output command length
	64	FLOAT[4][4]		matrix
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao GlobalTransform2D 3
	2	62		output command number
	2	10		output command length
	36	FLOAT[3][3]		matrix
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao ModelClip 3
	2	63		output command number
	2	2		output command length
	1			clip switch
		0	Clip
		1	NoClip
	3			unused
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao SetModelClipVolume3D 3
	2	64		output command number
	2	2+6n		output command length
	2	INT16		operator
	2	n		number of halfspaces
	24n	LISTofHALF_SPACE		halfspaces
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao SetModelClipVolume2D 3
	2	65		output command number
	2	2+4n		output command length
	2	INT16		operator
	2	n		number of halfspaces
	16n	LISTofHALF_SPACE_2D		halfspaces
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao RestoreModelClipVolume 3
	2	66		output command number
	2	1		output command length
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao ViewIndex 3
	2	67		output command number
	2	2		output command length
	2	CARD16		view index
	2			unused
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao LightSourceState 3
	2	68		output command number
	2	2+(2e+2d+p1+p2)/4	output command length
	2	CARD16		number of lights to enable
	2	CARD16		number of lights to disable
	2e	LISTofCARD16		lights to be enabled
	p1			unused, p1=pad(2e)
	2d	LISTofCARD16		lights to be disabled
	p2			unused, p2=pad(2d)
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao DepthCueIndex 3
	2	69		output command number
	2	2		output command length
	2	CARD16		depth cue index
	2			unused
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao PickID 3
	2	70		output command number
	2	2		output command length
	4	CARD32		pick identifier
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao HLHSRIdentifier 3
	2	71		output command number
	2	2		output command length
	4	CARD32		HLHSR identifier
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao ColorApproximationIndex 3
.mc |
	2	72		output command number
.mc
	2	2		output command length
	2	CARD16		color approximation index
	2			unused
.De
.mc *
.mc
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao AddNamesToNameSet 3
.mc |
	2	128		output command number
.mc
	2	1+n		output command length
	4n	CARD32		list of names (length implicit)
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao RemoveNamesFromNameSet 3
.mc |
	2	129		output command number
.mc
	2	1+n		output command length
	4n	CARD32		list of names (length implicit)
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao ExecuteStructure 3
.mc |
	2	130		output command number
.mc
	2	2		output command length
	4	CARD32		structure ID
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao Label 3
.mc |
	2	131		output command number
.mc
	2	2		output command length
	4	INT32		label
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao ApplicationData 3
.mc |
	2	132		output command number
.mc
	2	2+(n+p)/4		output command length
	2	n		length of data
	2			unused
	n	CARD8		data
	p			unused, p=pad(n)
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao GSE 3
.mc |
	2	133		output command number
.mc
	2	3+(n+p)/4		output command length
	4	INT32		GSE identifier
	2	n		length of data
	2			unused
	n	CARD8		data
	p			unused, p=pad(n)
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao Marker3D 3
.mc |
	2	134		output command number
.mc
	2	1+3n		output command length
	12n	LISTofCOORD_3D		points (length implicit)
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao Marker2D 3
.mc |
	2	135		output command number
.mc
	2	1+2n		output command length
	8n	LISTofCOORD_2D		points (length implicit)
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao Text3D 3
.mc |
	2	136		output command number
.mc
	2	11+(n+p)/4		output command length
	4	FLOAT		origin x
	4	FLOAT		origin y
	4	FLOAT		origin z
	4	FLOAT		vector1 x
	4	FLOAT		vector1 y
	4	FLOAT		vector1 z
	4	FLOAT		vector2 x
	4	FLOAT		vector2 y
	4	FLOAT		vector2 z
	2	n		length of string in bytes
	2			unused
	n	LISTofCARD8		string
	p			unused, p=pad(n)
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao Text2D 3
.mc |
	2	137		output command number
.mc
	2	4+(n+p)/4		output command length
	4	FLOAT		origin x
	4	FLOAT		origin y
	2	n		length of string in bytes
	2			unused
	n	LISTofCARD8		string
	p			unused, p=pad(n)
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao AnnotationText3D 3
.mc |
	2	138		output command number
.mc
	2	8+(n+p)/4		output command length
	4	FLOAT		origin x
	4	FLOAT		origin y
	4	FLOAT		origin z
	4	FLOAT		offset x
	4	FLOAT		offset y
	4	FLOAT		offset z
	2	n		length of string in bytes
	2			unused
	n	LISTofCARD8		string
	p			unused, p=pad(n)
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao AnnotationText2D 3
.mc |
	2	139		output command number
.mc
	2	6+(n+p)/4		output command length
	4	FLOAT		origin x
	4	FLOAT		origin y
	4	FLOAT		offset x
	4	FLOAT		offset y
	2	n		length of string in bytes
	2			unused
	n	LISTofCARD8		string
	p			unused, p=pad(n)
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao Polyline3D 3
.mc |
	2	140		output command number
.mc
	2	1+3n		output command length
	12n	LISTofCOORD_3D		vertices (length implicit)
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao Polyline2D 3
.mc |
	2	141		output command number
.mc
	2	1+2n		output command length
	8n	LISTofCOORD_2D		vertices (length implicit)
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao PolylineSet3DWithData 3
.mc |
	2	142		output command number
.mc
	2	3+n+m*v/4		output command length
	2	INT16		color type
	2			vertex attributes
		#x0001	color
	4	n		number of lists
				(let m be the number of vertices in the list of lists)
	4n+m*v	LISTofLISTofVERTEX		vertices (see VERTEX for v)
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao NonUniformBSplineCurve 3
.mc |
	2	143		output command number
.mc
	2	6+k+t*p		output command length
	2	CARD16		order
	2			type
		0	Rational
		1	NonRational	(t = (type == Rational) ? 4 : 3)
	4	FLOAT		tmin
	4	FLOAT		tmax
	4	k		number of knots
	4	p		number of points
	4k	LISTofFLOAT		knots
	4t*p	LISTofCOORD		points (COORD_3D or COORD_4D depending on t)
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao FillArea3D 3
.mc |
	2	144		output command number
.mc
	2	2+3p		output command length
	2			shape
		0	Complex
		1	Nonconvex
		2	Convex
		3	Unknown
	1			ignore edges
		0	False
		1	True
	1			unused
	12p	LISTofCOORD_3D		vertices (length implicit)
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao FillArea2D 3
.mc |
	2	145		output command number
.mc
	2	2+2p		output command length
	2			shape
		0	Complex
		1	Nonconvex
		2	Convex
		3	Unknown
	1			ignore edges
		0	False
		1	True
	1			unused
	8p	LISTofCOORD_2D		vertices (length implicit)
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao FillArea3DWithData 3
.mc |
	2	146		output command number
.mc
	2	4+f/4		output command length
	2			shape
		0	Complex
		1	Nonconvex
		2	Convex
		3	Unknown
	1			ignore edges
		0	False
		1	True
	1			unused
	2	INT16		color type
	2			facet attributes
		#x0001	color
		#x0002	normal
	2			vertex attributes
		#x0001	color
		#x0002	normal
	2			unused
	f	FACET		facet (see FACET for f)
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao FillAreaSet3D 3
	(let m be the number of vertices in the list of lists)
.mc |
	2	147		output command number
.mc
	2	3+n+3m		output command length
	2			shape
		0	Complex
		1	Nonconvex
		2	Convex
		3	Unknown
	1			ignore edges
		0	False
		1	True
	1			unused
	4	n		number of lists
	4n+12m	LISTofLISTofCOORD_3D		vertices
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao FillAreaSet2D 3
	(let m be the number of vertices in the list of lists)
.mc |
	2	148		output command number
.mc
	2	3+n+2m		output command length
	2			shape
		0	Complex
		1	Nonconvex
		2	Convex
		3	Unknown
	1			ignore edges
		0	False
		1	True
	1			unused
	4	n		number of lists
	4n+8m	LISTofLISTofCOORD_2D		vertices
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao FillAreaSet3DWithData 3
	(let m be the number of vertices in the list of lists)
.mc |
	2	149		output command number
.mc
	2	5+n+(d+m*v)/4		output command length
	2			shape
		0	Complex
		1	Nonconvex
		2	Convex
		3	Unknown
	1			ignore edges
		0	False
		1	True
	1			unused
	2	INT16		color type
	2			facet attributes
		#x0001	color
		#x0002	normal
	2			vertex attributes
		#x0001	color
		#x0002	normal
		#x0004	edges
	2			unused
	4	n		number of lists
	d	OPT_DATA		facet data (see OPT_DATA for d)
	4n+m*v	LISTofLISTofVERTEX		vertices (see VERTEX for v)
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao TriangleStrip 3
.mc |
	2	150		output command number
.mc
	2	4+((p-2)d+p*v)/4		output command length
	2	INT16		color type
	2			facet attributes
		#x0001	color
		#x0002	normal
	2			vertex attributes
		#x0001	color
		#x0002	normal
	2			unused
	4	p		number of points
	(p-2)d	LISTofOPT_DATA		facet data (see OPT_DATA for d)
	p*v	LISTofVERTEX		vertices (see VERTEX for v)
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao QuadrilateralMesh 3
.mc |
	2	151		output command number
.mc
	2	4+(d(m*n-n-m+1)+m*n*v)/4		output command length
	2	INT16		color type
	2	m		number of points in m direction
	2	n		number of points in n direction
	2			facet attributes
		#x0001	color
		#x0002	normal
	2			vertex attributes
		#x0001	color
		#x0002	normal
	2			shape
		0	Complex
		1	Nonconvex
		2	Convex
		3	Unknown
	(m-1)(n-1)d	LISTofOPT_DATA		facet data (see OPT_DATA for d)
	m*n*v	LISTofVERTEX		vertices (see VERTEX for v)
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao IndexedPolygons 3
	(let s be the size of the data)
	(s = 2fc + p1 + fc*d + ec* (e ? 4 : 2) + p2 + vc*v)
.mc |
	2	152		output command number
.mc
	2	5+s/4		output command length
	2			shape
		0	Complex
		1	Nonconvex
		2	Convex
		3	Unknown
	2	INT16		color type
	2			facet attributes
		#x0001	color
		#x0002	normal
	2			vertex attributes
		#x0001	color
		#x0002	normal
	2			edge attributes (call this value e)
		0	Off
		1	On
	2	fc		number of facet counts
	2	ec		number of edges
	2	vc		number of vertices
	2fc	LISTofCARD16		facet counts
	p1			unused, p1=pad(2fc)
	fc*d	LISTofOPT_DATA		facet data (see OPT_DATA for d)
	ec*(e?4:2)	LISTofVERTEX_INDEX		edge data
	p2			unused, p2=pad(ec*(e?4:2))
	vc*v	LISTofVERTEX		vertices (see VERTEX for v)
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao NonUniformBSplineSurface 3
.mc |
	2	153		output command number
.mc
	2	11+uk+vk+m*n*s+t+sc/4		output command length
	2			type
		0	Rational
		1	NonRational	(s = (type == Rational) ? 4 : 3)
	2	CARD16		u order
	2	CARD16		v order
	2			unused
	4	FLOAT		umin
	4	FLOAT		umax
	4	FLOAT		vmin
	4	FLOAT		vmax
	4	uk		number of u knots
	4	vk		number of v knots
	2	m		number of points in m direction
	2	n		number of points in n direction
	4	t		number of trim curves
	4uk	LISTofFLOAT		u knots
	4vk	LISTofFLOAT		v knots
	m*n*4s	LISTofCOORD		points (COORD_3D or COORD_4D depending on s)
				(let sc be sum of sizes of TRIM_CURVEs in bytes)
	4t+sc	LISTofLISTofTRIM_CURVE		trim curves
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao CellArray3D 3
.mc |
	2	154		output command number
.mc
	2	12+(2q+p)/4		output command length
	4	FLOAT		point1 x
	4	FLOAT		point1 y
	4	FLOAT		point1 z
	4	FLOAT		point2 x
	4	FLOAT		point2 y
	4	FLOAT		point2 z
	4	FLOAT		point3 x
	4	FLOAT		point3 y
	4	FLOAT		point3 z
	4	CARD32		dx
	4	CARD32		dy (let q = dx * dy)
	2q	CARD16		color indices
	p			unused, p=pad(2q)
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao CellArray2D 3
.mc |
	2	155		output command number
.mc
	2	7+(2s+p)/4		output command length
	4	FLOAT		point1 x
	4	FLOAT		point1 y
	4	FLOAT		point2 x
	4	FLOAT		point2 y
	4	CARD32		dx
	4	CARD32		dy (let s = dx * dy)
	2s	CARD16		color indices
	p			unused, p=pad(2s)
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao ExtendedCellArray3D 3
.mc |
	(n = ((color type == Indexed) || (color type == RGBInt8)) ? 1 : \
((color type == RGBInt16) ? 2 : 3))
	2	156		output command number
	2	13+p*n		output command length
.mc
	2	INT16		color type
	2			unused
	4	FLOAT		point1 x
	4	FLOAT		point1 y
	4	FLOAT		point1 z
	4	FLOAT		point2 x
	4	FLOAT		point2 y
	4	FLOAT		point2 z
	4	FLOAT		point3 x
	4	FLOAT		point3 y
	4	FLOAT		point3 z
	4	CARD32		dx
	4	CARD32		dy (let p = dx * dy)
.mc |
	p*4n	LISTofCOLOR		colors
.mc
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao GDP3D 3
.mc |
	2	157		output command number
.mc
	2	4+3p+(d+q)/4		output command length
	4	INT32		GDP identifier
	4	p		number of points
	4	d		number of bytes of data
	12p	LISTofCOORD_3D		points
	d	LISTofCARD8		data
	q			unused, q=pad(d)
.De
.LP
.Ds 0
.TA .2i .9i 1.9i 2.9i
.ta .2i .9i 1.9i 2.9i
.R
.Ao GDP2D 3
.mc |
	2	158		output command number
.mc
	2	4+2p+(d+q)/4		output command length
	4	INT32		GDP identifier
	4	p		number of points
	4	d		number of bytes of data
	8p	LISTofCOORD_2D		points
	d	LISTofCARD8		data
	q			unused, q=pad(d)
.De
.mc *
.mc
.bp

.AC "Lookup Tables" 1
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
LineBundle  (20+4n)
	2	INT16		line type
	2	INT16		polyline interpolation method
	2	INT16		curve approximation method
	2			unused
	4	FLOAT		curve approximation tolerance
	4	FLOAT		line width
	4+4n	COLOR_SPECIFIER	line color
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
MarkerBundle (12+4n)
	2	INT16		marker type
	2			unused
	4	FLOAT		marker scale
	4+4n	COLOR_SPECIFIER	marker color
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
TextBundle (16+4n)
	2	CARD16		font table index
	2			text precision
		0	StringPrecision
		1	CharPrecision
		2	StrokePrecision
	4	FLOAT		character expansion factor
	4	FLOAT		character spacing
	4+4n	COLOR_SPECIFIER	text color
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
InteriorBundle (92+16n)
	2	INT16		interior style
	2	CARD16 | INT16		interior style index
	2	INT16		reflection model
	2	INT16		surface interpolation method
	2	INT16		bf interior style
	2	CARD16 | INT16		bf interior style index
	2	INT16		bf reflection model
	2	INT16		bf surface interpolation method
	2	INT16		surface approximation method
	2			unused
	4	FLOAT		surface approximation u tolerance
	4	FLOAT		surface approximation v tolerance
	2	INT16		trim curve approximation method
	2			unused
	4	FLOAT		trim curve approximation tolerance
	4+4n	COLOR_SPECIFIER	surface color
	4	FLOAT		reflection attribute ambient coefficient
	4	FLOAT		reflection attribute diffuse coefficient
	4	FLOAT		reflection attribute specular coefficient
	4	FLOAT		reflection attribute specular concentration
	4	FLOAT		reflection attribute transmission coefficient
	4+4n	COLOR_SPECIFIER	reflection attribute specular color
	4+4n	COLOR_SPECIFIER	bf surface color
	4	FLOAT		bf reflection attribute ambient coefficient
	4	FLOAT		bf reflection attribute diffuse coefficient
	4	FLOAT		bf reflection attribute specular coefficient
	4	FLOAT		bf reflection attribute specular concentration
	4	FLOAT		bf reflection attribute transmission coefficient
	4+4n	COLOR_SPECIFIER	bf reflection attribute specular color
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
EdgeBundle (12+4n)
	1			surface edges
		0	Off
		1	On
	1			unused
	2	INT16		surface edge type
	4	FLOAT		surface edge width
	4+4n	COLOR_SPECIFIER	surface edge color
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
Pattern (8+c*4n)
	(n = ((color type == Indexed) || (color type == RGBInt8)) ? 1 : \
((color type == RGBInt16) ? 2 : 3))
	2	INT16		color type
	2	CARD16		numx
	2	CARD16		numy
	2			unused
	c*4n	LISTofCOLOR		colors
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
Color (4+4n)
	n = ((color type == RGBInt8) ? 1 : ((color type == RGBInt16) ? 2 : 3)
	2	INT16		color type
	2			unused
	4n	DIRECT_COLOR		color
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
TextFont (4)
	4	CARD32		font ID
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
View (156)
	2 			clip flags
		#x0001	clip_xy
		#x0002	clip_back
		#x0004	clip_front
	2			unused
	4	FLOAT		clip limits min x
	4	FLOAT		clip limits min y
	4	FLOAT		clip limits min z
	4	FLOAT		clip limits max x
	4	FLOAT		clip limits max y
	4	FLOAT		clip limits max z
	64	FLOAT[4][4]		orientation
	64	FLOAT[4][4]		mapping
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
Light (48+4n)
	2	INT16		light type
	2			unused
	4	FLOAT		direction x
	4	FLOAT		direction y
	4	FLOAT		direction z
	4	FLOAT		point x
	4	FLOAT		point y
	4	FLOAT		point z
	4	FLOAT		concentration
	4	FLOAT		spread angle
	4	FLOAT		attenuation factor1
	4	FLOAT		attenuation factor2
	4+4n	COLOR_SPECIFIER	color
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
DepthCue (24+4n)
	1			mode
		0	Off
		1	On
	3			unused
	4	FLOAT		front plane
	4	FLOAT		back plane
	4	FLOAT		front scaling
	4	FLOAT		back scaling
	4+4n	COLOR_SPECIFIER	color
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
ColorApprox (40)
	2	INT16		color approximation type
	2	INT16		color approximation model
	2	CARD16		max1
	2	CARD16		max2
	2	CARD16		max3
	1			dither
		0	Off
		1	On
	1			unused
	4	CARD32		multiplier1
	4	CARD32		multiplier2
	4	CARD32		multiplier3
	4	FLOAT		weight1
	4	FLOAT		weight2
	4	FLOAT		weight3
	4	CARD32		base pixel
.De

.AC "Lookup Table Resource Management" 2
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXCreateLookupTable
	1	CARD8		opcode (X11 assigned)
	1	4		PEX opcode
	2	4		request length
	4	CARD32		example drawable ID
	4	CARD32		lookup table ID
	2			table type
		1	LineBundle
		2	MarkerBundle
		3	TextBundle
		4	InteriorBundle
		5	EdgeBundle
		6 	Pattern
		7	TextFont
		8	Color
		9	View
		10	Light
		11	DepthCue
		12	ColorApprox
	2			unused
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXCopyLookupTable
	1	CARD8		opcode (X11 assigned)
	1	5		PEX opcode
	2	3		request length
	4	CARD32		source lookup table ID
	4	CARD32		destination lookup table ID
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXFreeLookupTable
	1	CARD8		opcode (X11 assigned)
	1	6		PEX opcode
	2	2		request length
	4	CARD32		lookup table ID
.De

.AC "Lookup Table Inquiry" 2
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXGetTableInfo
	1	CARD8		opcode (X11 assigned)
	1	7		PEX opcode
	2	3		request length
	4	CARD32		example drawable ID
	2			table type
		1	LineBundle
		2	MarkerBundle
		3	TextBundle
		4	InteriorBundle
		5	EdgeBundle
		6 	Pattern
		7	TextFont
		8	Color
		9	View
		10	Light
		11	DepthCue
		12	ColorApprox
	2			unused
.De
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
=>
	1	1		Reply
	1			unused
	2	CARD16		sequence number
	4	0		reply length
	2			unused
	2	CARD16		definable entries
	2	CARD16		num predefined
	2	CARD16		predefined min
	2	CARD16		predefined max
	14			unused
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXGetPredefinedEntries
	1	CARD8		opcode (X11 assigned)
	1	8		PEX opcode
	2	5		request length
	2	INT16		floating point format
	2			unused
	4	CARD32		example drawable ID
	2			table type
		1	LineBundle
		2	MarkerBundle
		3	TextBundle
		4	InteriorBundle
		5	EdgeBundle
		6 	Pattern
		7	TextFont
		8	Color
		9	View
		10	Light
		11	DepthCue
		12	ColorApprox
	2	CARD16		start
	2	n		count
	2			unused
.De
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
=>
	1	1		Reply
	1			unused
	2	CARD16		sequence number
	4	s/4		reply length
	4			unused
	4	n		number of table entries
	16			unused
				(let s be the sum of the sizes of the n table \
entries in bytes)
				(each entry may be a different size, depending on colors)
	s	LISTofTABLE_ENTRY		entries
				(each entry will be aligned on a 4-byte boundary)
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXGetDefinedIndices
	1	CARD8		opcode (X11 assigned)
	1	9		PEX opcode
	2	2		request length
	4	CARD32		lookup table ID
.De
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
=>
	1	1		Reply
	1			unused
	2	CARD16		sequence number
	4	(2n+p)/4		reply length
	4	n		number of indices returned
	20			unused
	2n	LISTofCARD16		defined indices
	p			unused, p=pad(2n)
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXGetTableEntry
	1	CARD8		opcode (X11 assigned)
	1	10		PEX opcode
	2	4		request length
	2	INT16		floating point format
	2			unused
	4	CARD32		lookup table ID
	2	CARD16		index
	2			unused
.De
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
=>
	1	1		Reply
	1			unused
	2	CARD16		sequence number
	4	s/4		reply length
	2			status
		0	Default
		1	Defined
	2	t		table type
		1	LineBundle
		2	MarkerBundle
		3	TextBundle
		4	InteriorBundle
		5	EdgeBundle
		6 	Pattern
		7	TextFont
		8	Color
		9	View
		10	Light
		11	DepthCue
		12	ColorApprox
	20			unused
				(let s be size of entry of type t)
				(entry size differs, depending on colors)
	s	TABLE_ENTRY		entry
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXGetTableEntries
	1	CARD8		opcode (X11 assigned)
	1	11		PEX opcode
	2	4		request length
	2	INT16		floating point format
	2			unused
	4	CARD32		lookup table ID
	2	CARD16		start
	2	n		count
.De
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
=>
	1	1		Reply
	1			unused
	2	CARD16		sequence number
	4	s/4		reply length
	2	t		table type
		1	LineBundle
		2	MarkerBundle
		3	TextBundle
		4	InteriorBundle
		5	EdgeBundle
		6 	Pattern
		7	TextFont
		8	Color
		9	View
		10	Light
		11	DepthCue
		12	ColorApprox
				(let s be the sum of the sizes of the n table \
entries in bytes)
				(each entry may be a different size, depending on colors)
	2			unused
	4	n		number of table entries
	16			unused
	s	LISTofTABLE_ENTRY		entries
.De

.AC "Lookup Table Modification" 2
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXSetTableEntries
	1	CARD8		opcode (X11 assigned)
	1	12		PEX opcode
	2	4+s/4		request length
	2	INT16		floating point format
	2			unused
	4	CARD32		lookup table ID
	2	CARD16		start
	2	n		count
				(let s be the sum of the sizes of the n table \
entries in bytes)
				(each entry may be a different size, depending on colors)
	s	LISTofTABLE_ENTRY		entries
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXDeleteTableEntries
	1	CARD8		opcode (X11 assigned)
	1	13		PEX opcode
	2	3		request length
	4	CARD32		lookup table ID
	2	CARD16		start
	2	CARD16		count
.De
.bp

.AC "Pipeline Contexts" 1
.LP
The following components may appear in the requests \fBPEXCreatePipelineContext\fP
and \fBPEXChangePipelineContext\fP; they may also appear in replies to
\fBPEXGetPipelineContext\fP.  Because the components are not of a fixed size
(as they are in X11), the list must be parsed serially to know where one
component ends and the next begins.  A bitmap mask of two CARD32s specifies
which components are to be found in a list.  Proceding from the least
significant bit of the first CARD32 through to the last used bit of the
second CARD32, if the bit is set, the next item in the list is of the
specified type.
.sp 1
.LP
.Ds 0
.TA .2i 1i
.ta .2i 1i
PC_BITMASK
.sp 1
First CARD32:
	#x00000001	marker_type
	#x00000002	marker_scale
	#x00000004	marker_color
	#x00000008	marker_bundle_index
	#x00000010	text_font_index
	#x00000020	text_precision
	#x00000040	char_expansion
	#x00000080	char_spacing
	#x00000100	text_color
	#x00000200	char_height
	#x00000400	char_up_vector
	#x00000800	text_path
	#x00001000	text_alignment
	#x00002000	atext_height
	#x00004000	atext_up_vector
	#x00008000	atext_path
	#x00010000	atext_alignment
	#x00020000	atext_style
	#x00040000	text_bundle_index
	#x00080000	line_type
	#x00100000	line_width
	#x00200000	line_color
	#x00400000	curve_approx
	#x00800000	polyline_interp
	#x01000000	line_bundle_index
	#x02000000	interior_style
	#x04000000	interior_style_index
	#x08000000	surface_color
	#x10000000	reflection_attr
	#x20000000	reflection_model
	#x40000000	surface_interp
	#x80000000	bf_interior_style
Second CARD32:
	#x00000001	bf_interior_style_index
	#x00000002	bf_surface_color
	#x00000004	bf_reflection_attr
	#x00000008	bf_reflection_model
	#x00000010	bf_surface_interp
	#x00000020	surface_approx
	#x00000040	trim_curve_approx
	#x00000080	culling_mode
	#x00000100	distinguish
	#x00000200	normalflip
	#x00000400	pattern_size
	#x00000800	pattern_ref_pt
	#x00001000	pattern_ref_vec1
	#x00002000	pattern_ref_vec2
	#x00004000	interior_bundle_index
	#x00008000	surface_edges
	#x00010000	surface_edge_type
	#x00020000	surface_edge_width
	#x00040000	surface_edge_color
	#x00080000	edge_bundle_index
	#x00100000	local_transform
	#x00200000	global_transform
	#x00400000	model_clip
	#x00800000	model_clip_volume
	#x01000000	view_index
	#x02000000	light_state
	#x04000000	depth_cue_index
	#x08000000	asfs
	#x10000000	pick_id
	#x20000000	HLHSR_identifier
	#x40000000	name_set
	#x80000000	color_approx_index
.sp 1
ASF flags bits
	#x00000001	marker_type_asf
	#x00000002	marker_scale_asf
	#x00000004	marker_color_asf
	#x00000008	text_font_index_asf
	#x00000010	text_precison_asf
	#x00000020	char_expansion_asf
	#x00000040	char_spacing_asf
	#x00000080	text_color_asf
	#x00000100	line_type_asf
	#x00000200	line_width_asf
	#x00000400	line_color_asf
	#x00000800	curve_approx_asf
	#x00001000	polyline_interp_asf
	#x00002000	interior_style_asf
	#x00004000	interior_style_index_asf
	#x00008000	surface_color_asf
	#x00010000	surface_interp_asf
	#x00020000	reflection_model_asf
	#x00040000	reflection_attr_asf
	#x00080000	bf_interior_style_asf
	#x00100000	bf_interior_style_index_asf
	#x00200000	bf_surface_color_asf
	#x00400000	bf_surface_interp_asf
	#x00800000	bf_reflection_model_asf
	#x01000000	bf_reflection_attr_asf
	#x02000000	surface_approx_asf
	#x04000000	trim_curve_approx_asf
	#x08000000	surface_edges_asf
	#x10000000	surface_edge_type_asf
	#x20000000	surface_edge_width_asf
	#x40000000	surface_edge_color_asf
.De

.LP
Note that lengths are the (perhaps padded) lengths sent over the wire, and
are chosen to make entries align. Types may be smaller than the length field
into which they are assigned. If the natural type is smaller than the padded
field, this is equivalent to sending the value in its native type and then
a pad of (length - native size) bytes.
If a component has sub-fields, the sizes, types and interpretations of the
sub-fields are on subsequent lines.
.LP
For example, marker type has a length of 4, but a type of INT16, which is
only 2 bytes long.  When sending a marker type to a Pipeline Context, we
therefore send a 2 byte INT16 and 2 bytes of padding.
.LP
When sending a color specifier, we send 2 bytes indicating the color type,
2 bytes of pad, and either 4, 8, or 16 bytes of color information, depending
on the color type.  We will always have already sent the direct color
format so that we know what length a direct color will be.
.LP
.Ds 0
.TA 2i 2.3i 2.8i 3.5i 4i
.ta 2i 2.3i 2.8i 3.5i 4i
.R
\fBAttribute Name		Length	Type\fP
.sp 1
marker_type		4	INT16
marker_scale		4	FLOAT
marker_color		4+4n	COLOR_SPECIFIER
marker_bundle_index		4	CARD16
text_font_index		4	CARD16
text_precision		4	CARD16
			0	StringPrecision
			1	CharPrecision
			2	StrokePrecision
char_expansion		4	FLOAT
char_spacing		4	FLOAT
text_color		4+4n	COLOR_SPECIFIER
char_height		4	FLOAT
char_up_vector		8
		4	FLOAT		x
		4	FLOAT		y
text_path		4	CARD16
			0	PathRight
			1	PathLeft
			2	PathUp
			3	PathDown
text_alignment		4
		2	CARD16	alignment vertical
			0	ValignNormal
			1	ValignTop
			2	ValignCap
			3	ValignHalf
			4	ValignBase
			5	ValignBottom
		2	CARD16	alignment horizontal
			0	HalignNormal
			1	HalignLeft
			2	HalignCenter
			3	HalignRight

atext_height		4	FLOAT
atext_up_vector		8
		4	FLOAT		x
		4	FLOAT		y
atext_path		4	CARD16
			0	PathRight
			1	PathLeft
			2	PathUp
			3	PathDown
atext_alignment		4
		2	CARD16	alignment vertical
			0	ValignNormal
			1	ValignTop
			2	ValignCap
			3	ValignHalf
			4	ValignBase
			5	ValignBottom
		2	CARD16	alignment horizontal
			0	HalignNormal
			1	HalignLeft
			2	HalignCenter
			3	HalignRight
atext_style		4	INT16
text_bundle_index		4	CARD16
line_type		4	INT16
line_width		4	FLOAT
line_color		4+4n	COLOR_SPECIFIER
curve_approx		8
		4	INT16		curve approximation method
		4	FLOAT		curve approximation tolerance
polyline_interp		4	INT16
line_bundle_index		4	CARD16
interior_style		4	INT16
interior_style_index		4	INT16 | CARD16
surface_color		4+4n	COLOR_SPECIFIER
reflection_attr		24+4n
		4	FLOAT		ambient coefficient
		4	FLOAT		diffuse coefficient
		4	FLOAT		specular coefficient
		4	FLOAT		specular concentration
		4	FLOAT		transmission coefficient
		4+4n	COLOR_SPECIFIER	specular color
reflection_model		4	INT16
surface_interp		4	INT16
bf_interior_style		4	INT16
bf_interior_style_index		4	INT16 | CARD16
bf_surface_color		4+4n	COLOR_SPECIFIER
bf_reflection_attr		24+4n
		4	FLOAT		ambient coefficient
		4	FLOAT		diffuse coefficient
		4	FLOAT		specular coefficient
		4	FLOAT		specular concentration
		4	FLOAT		transmission coefficient
		4+4n	COLOR_SPECIFIER	specular color
bf_reflection_model		4	INT16
bf_surface_interp		4	INT16
surface_approx		12
		4	INT16	surface approximation method
		4	FLOAT	surface approximation u tolerance
		4	FLOAT	surface approximation v tolerance
trim_curve_approx		8
		4	INT16	trim curve approximation method
		4	FLOAT	trim curve approximation tolerance
culling_mode		4	CARD16
			0	None
			1	BackFaces
			2	FrontFaces
distinguish		4	CARD8
			0	False
			1	True
normalflip		4	CARD8
			0	False
			1	True
pattern_size		8
		4	FLOAT	x
		4	FLOAT	y
pattern_ref_pt		12
		4	FLOAT	x
		4	FLOAT	y
		4	FLOAT	z
pattern_ref_vec1		12
		4	FLOAT	x
		4	FLOAT	y
		4	FLOAT	z
pattern_ref_vec2		12
		4	FLOAT	x
		4	FLOAT	y
		4	FLOAT	z
interior_bundle_index		4	CARD16
surface_edges		4	CARD16
			0	Off
			1	On
surface_edge_type		4	INT16
surface_edge_width		4	FLOAT
surface_edge_color		4+4n	COLOR_SPECIFIER
edge_bundle_index		4	CARD16
local_transform		64	FLOAT[4][4]
global_transform		64	FLOAT[4][4]
model_clip		4	CARD16
			0	Clip
			1	NoClip
model_clip_volume		4+24n
		4	n	number of half spaces
		24n	LISTofHALF_SPACE
view_index		4	CARD16
light_state		4+2n+p
		4	n	number of lights in active list
		2n	CARD16	indices of active lights
		p		unused, p=pad(2n)
depth_cue_index		4	CARD16
asf_enables		4	CARD32	(see note below)
asfs		4 	CARD32
			0	Bundled
			1	Individual
pick_id		4	CARD32
HLHSR_identifier		4	CARD32
name_set		4	CARD32
color_approx_index		4	CARD16
.sp 1
.De
.LP
\fINote:\fR The asf_enables field is only used for the
\fBPEXCreatePipelineContext\fP and \fBPEXChangePipelineContext\fP requests.
For those requests, if the asf_enables bit for a particular asf is set, the
asf value is taken from the corresponding bit in the asfs field.  For the
\fBPEXGetPipelineContext\fP reply, asf_enables is present, but has no meaning
and must be zero.

.AC "Pipeline Context Resource Management" 2
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXCreatePipelineContext
	1	CARD8		opcode (X11 assigned)
	1	14		PEX opcode
	2	5+n/4		request length
	2	INT16		floating point format
	2			unused
	4	CARD32		pipeline context ID
	8	PC_BITMASK		item mask
	n	LISTofVALUE		item list
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXCopyPipelineContext
	1	CARD8		opcode (X11 assigned)
	1	15		PEX opcode
	2	5		request length
	4	CARD32		source pipeline context ID
	4	CARD32		destination pipeline context ID
	8	PC_BITMASK		item mask
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXFreePipelineContext
	1	CARD8		opcode (X11 assigned)
	1	16		PEX opcode
	2	2		request length
	4	CARD32		pipeline context ID
.De

.AC "Pipeline Context Inquiry" 2
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXGetPipelineContext
	1	CARD8		opcode (X11 assigned)
	1	17		PEX opcode
	2	5		request length
	2	INT16		floating point format
	2			unused
	4	CARD32		pipeline context ID
	8 	PC_BITMASK		item mask
.De
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
=>
	1	1		Reply
	1			unused
	2	CARD16		sequence number
	4	n/4		reply length
	4	CARD32		number of values
	20			unused
	n	LISTofVALUE		item list
.De

.AC "Pipeline Context Modification" 2
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXChangePipelineContext
	1	CARD8		opcode (X11 assigned)
	1	18		PEX opcode
	2	5+n/4		request length
	2	INT16		floating point format
	2			unused
	4	CARD32		pipeline context ID
	8	PC_BITMASK		item mask
	n	LISTofVALUE		item list
.De
.bp

.AC "Renderers" 1
.LP
.Ds 0
.TA .2i 1i
.ta .2i 1i
R_BITMASK
.sp 1
	#x00000001	pipeline_context_id
	#x00000002	current_path
	#x00000004	marker_bundle
	#x00000008	text_bundle
	#x00000010	line_bundle
	#x00000020	interior_bundle
	#x00000040	edge_bundle
	#x00000080	view_table
	#x00000100	color_table
	#x00000200	depth_cue_table
	#x00000400	light_table
	#x00000800	color_approx_table
	#x00001000	pattern_table
	#x00002000	text_font_table
	#x00004000	highlight_inclusion
	#x00008000	highlight_exclusion
	#x00010000	invisibility_inclusion
	#x00020000	invisibility_exclusion
	#x00040000	renderer_state
	#x00080000	HLHSR_mode
	#x00100000	NPC_subvolume
	#x00200000	viewport
	#x00400000	clip_list
.De
.LP
Note that lengths are the (perhaps padded) lengths sent over the wire, and
are chosen to make entries align. Types may be smaller than the length field
into which they are assigned. If the natural type is smaller than the padded
field, this is equivalent to sending the value in its native type and then
a pad of (length - native size) bytes.
If a component has sub-fields, the sizes, types and interpretations of the
sub-fields are on subsequent lines.
.LP
.Ds 0
.TA 2i 2.3i 2.8i 3.5i 4i
.ta 2i 2.3i 2.8i 3.5i 4i
.R
\fBAttribute Name		Length	Type\fP
.sp 1
pipeline_context		4	CARD32
current_path		4+8n
		4	n	number of element references
		8n	LISTofELEMENT_REF
marker_bundle		4	CARD32
text_bundle		4	CARD32
line_bundle		4	CARD32
interior_bundle		4	CARD32
edge_bundle		4	CARD32
view_table		4	CARD32
color_table		4	CARD32
depth_cue_table		4	CARD32
light_table		4	CARD32
color_approx_table		4	CARD32
pattern_table		4	CARD32
text_font_table		4	CARD32
highlight_inclusion		4	CARD32
highlight_exclusion		4	CARD32
invisibility_inclusion		4	CARD32
invisibility_exclusion		4	CARD32
renderer_state		4	CARD16
			0	Idle
			1	Rendering
HLHSR_mode		4	INT16
NPC_subvolume		24
		4	FLOAT	min x
		4	FLOAT	min y
		4	FLOAT	min z
		4	FLOAT	max x
		4	FLOAT	max y
		4	FLOAT	max z
viewport		20
		2	INT16	min x
		2	INT16	min y
		4	FLOAT	min z
		2	INT16	max x
		2	INT16	max y
		4	FLOAT	max z
		1	CARD8	use drawable
			0	False
			1	True
		3		unused
clip_list		4+8n
		4	n	number of device rectangles
		8n	LISTofDEVICE_RECT

.AC "Renderer Resource Management" 2
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXCreateRenderer
	1	CARD8		opcode (X11 assigned)
	1	19		PEX opcode
	2	5+n/4		request length
	2	INT16		floating point format
	2			unused
	4	CARD32		renderer ID
	4	CARD32		example drawable ID
	4	R_BITMASK		item mask
	n	LISTofVALUE		item list
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXFreeRenderer
	1	CARD8		opcode (X11 assigned)
	1	20		PEX opcode
	2	2		request length
	4	CARD32		renderer ID
.De

.AC "Renderer Modification" 2
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXChangeRenderer
	1	CARD8		opcode (X11 assigned)
	1	21		PEX opcode
	2	4+n/4		request length
	2	INT16		floating point format
	2			unused
	4	CARD32		renderer ID
	4	R_BITMASK		item mask
	n	LISTofVALUE		item list
.De

.AC "Renderer Inquiry" 2
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXGetRendererAttributes
	1	CARD8		opcode (X11 assigned)
	1	22		PEX opcode
	2	4		request length
	2	INT16		floating point format
	2			unused
	4	CARD32		renderer ID
	4	R_BITMASK		item mask
.De
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
=>
	1	1		Reply
	1			unused
	2	CARD16		sequence number
	4	n/4		reply length
	24			unused
	n	LISTofVALUE		item list
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXGetRendererDynamics
	1	CARD8		opcode (X11 assigned)
	1	31		PEX opcode
	2	2		request length
	4	CARD32		renderer ID
.De
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
=>
	1	1		Reply
	1			unused
	2	CARD16		sequence number
	4	0		reply length
	4			tables
		#x00000001	DynMarkerBundle
		#x00000002	DynTextBundle
		#x00000004	DynLineBundle
		#x00000008	DynInteriorBundle
		#x00000010	DynEdgeBundle
		#x00000020	DynViewTable
		#x00000040	DynColorTable
		#x00000080	DynDepthCueTable
		#x00000100	DynLightTable
		#x00000200	DynColorApproxTable
		#x00000400	DynPatternTable
		#x00000800	DynTextFontTable
		#x00010000	DynMarkerBundleContents
		#x00020000	DynTextBundleContents
		#x00040000	DynLineBundleContents
		#x00080000	DynInteriorBundleContents
		#x00100000	DynEdgeBundleContents
		#x00200000	DynViewTableContents
		#x00400000	DynColorTableContents
		#x00800000	DynDepthCueTableContents
		#x01000000	DynLightTableContents
		#x02000000	DynColorApproxTableContents
		#x04000000	DynPatternTableContents
		#x08000000	DynTextFontTableContents
	4			namesets
		#x00000001	DynHighlightNameset
		#x00000002	DynInvisibilityNameset
		#x00010000	DynHighlightNamesetContents
		#x00020000	DynInvisibilityNamesetContents
	4			attributes
		#x00000001	DynHLHSRMode
		#x00000002	DynNPCSubvolume
		#x00000004	DynViewport
		#x00000008	DynClipList
	12			unused
.De

.AC "Client-Side Traversal Support" 2
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXBeginRendering
	1	CARD8		opcode (X11 assigned)
	1	23		PEX opcode
	2	3		request length
	4	CARD32		renderer ID
	4	CARD32		drawable ID
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXEndRendering
	1	CARD8		opcode (X11 assigned)
	1	24		PEX opcode
	2	3		request length
	4	CARD32		renderer ID
	1			flush
		0	False
		1	True
	3			unused
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXBeginStructure
	1	CARD8		opcode (X11 assigned)
	1	25		PEX opcode
	2	3		request length
	4	CARD32		renderer ID
	4	CARD32		structure ID
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXEndStructure
	1	CARD8		opcode (X11 assigned)
	1	26		PEX opcode
	2	2		request length
	4	CARD32		renderer ID
.De

.AC "Rendering Commands" 2
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXRenderOutputCommands
	1	CARD8		opcode (X11 assigned)
	1	27		PEX opcode
	2	4+s/4		request length
	2	INT16		floating point format
	2			unused
	4	CARD32		renderer ID
	4	n		number of commands
				(let s be the total size of the output commands in bytes)
	s	LISTofOUTPUT_COMMAND		cmds
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXRenderNetwork
	1	CARD8		opcode (X11 assigned)
	1	28		PEX opcode
	2	4		request length
	4	CARD32		renderer ID
	4	CARD32		drawable ID
	4	CARD32		structure ID
.De
.bp

.AC "Structures" 1
.LP

.AC "Structure Resource Management" 2
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXCreateStructure
	1	CARD8		opcode (X11 assigned)
	1	29		PEX opcode
	2	2		request length
	4	CARD32		structure ID
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXCopyStructure
	1	CARD8		opcode (X11 assigned)
	1	30		PEX opcode
	2	3		request length
	4	CARD32		source structure ID
	4	CARD32		destination structure ID
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXDestroyStructures
	1	CARD8		opcode (X11 assigned)
	1	32		PEX opcode
	2	2+n		request length
	4	CARD32		number of structure IDs
	4n	LISTofCARD32		list of structure IDs
.De

.AC "Structure Inquiry" 2
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXGetStructureInfo
	1	CARD8		opcode (X11 assigned)
	1	33		PEX opcode
	2	3		request length
	2	INT16		floating point format
	2			unused
	4	CARD32		structure ID
.De
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
=>
	1	1		Reply
	1			unused
	2	CARD16		sequence number
	4	0		reply length
	2			editing mode
		0	StructureInsert
		1	StructureReplace
	2			unused
	4	CARD32		element pointer
	4	CARD32		number of elements
	4	CARD32		total length in bytes
	4	CARD32		number of references
	4			unused
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXGetElementInfo
	1	CARD8		opcode (X11 assigned)
	1	34		PEX opcode
	2	7		request length
	2	INT16		floating point format
	2			unused
	4	CARD32		structure ID
	2			position1 whence
		0	Beginning
		1	Current
		2	End
	2			unused
	4	INT32		position1 offset
	2			position2 whence
		0	Beginning
		1	Current
		2	End
	2			unused
	4	INT32		position2 offset
.De
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
=>
	1	1		Reply
	1			unused
	2	CARD16		sequence number
	4	n		reply length
	4	n		number of element info
	20			unused
	4n	LISTofELEMENT_INFO		info
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXGetStructuresInNetwork
	1	CARD8		opcode (X11 assigned)
	1	35		PEX opcode
	2	3		request length
	4	CARD32		structure ID
	2			which
		0	All
.mc |
		1	NoCrossRefs
.mc
	2			unused
.De
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
=>
	1	1		Reply
	1			unused
	2	CARD16		sequence number
	4	n		reply length
	8			unused
	4 	n		number of structures
	12			unused
	4n	LISTofSTRUCTURE_ID		structure IDs
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXGetAncestors
	1	CARD8		opcode (X11 assigned)
	1	36		PEX opcode
	2	4		request length
	4	CARD32		structure ID
	2			path part
		0	TopPart
		1	BottomPart
	2			unused
	4	CARD32		path depth
.De
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
=>
	1	1		Reply
	1			unused
	2	CARD16		sequence number
	4	n+2s		reply length
	12			unused
	4	n		number of element reference lists
	8	unused
				(let s be the sum of these n numbers)
	4n+8s	LISTofLISTofELEMENT_REF		paths
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXGetDescendants
	1	CARD8		opcode (X11 assigned)
	1	37		PEX opcode
	2	4		request length
	4	CARD32		structure ID
	2			path part
		0	TopPart
		1	BottomPart
	2			unused
	4	CARD32		path depth
.De
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
=>
	1	1		Reply
	1			unused
	2	CARD16		sequence number
	4	n+2s		reply length
	12			unused
	4	n		number of element reference lists
	8	unused
				(let s = sum of these n numbers)
	4n+8s	LISTofLISTofELEMENT_REF		paths
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXFetchElements
	1	CARD8		opcode (X11 assigned)
	1	38		PEX opcode
	2	7		request length
	2	INT16		floating point format
	2			unused
	4	CARD32		structure ID
	2			position1 whence
		0	Beginning
		1	Current
		2	End
	2			unused
	4	INT32		position1 offset
	2			position2 whence
		0	Beginning
		1	Current
		2	End
	2			unused
	4	INT32		position2 offset
.De
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
=>
	1	1		Reply
	1			unused
	2	CARD16		sequence number
	4	s/4		reply length
	4	CARD32		number of output commands
	20			unused
				(let s be the total size of the output commands in bytes)
	s	LISTofOUTPUT_COMMAND		elements
.De

.AC "Structure Resource Attribute Modification" 2
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXSetEditingMode
	1	CARD8		opcode (X11 assigned)
	1	39		PEX opcode
	2	3		request length
	4	CARD32		structure ID
	2			mode
		0	StructureInsert
		1	StructureReplace
	2			unused
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXSetElementPointer
	1	CARD8		opcode (X11 assigned)
	1	40		PEX opcode
	2	4		request length
	4	CARD32		structure ID
	2			position whence
		0	Beginning
		1	Current
		2	End
	2			unused
	4	INT32		position offset
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXSetElementPointerAtLabel
	1	CARD8		opcode (X11 assigned)
	1	41		PEX opcode
	2	4		request length
	4	CARD32		structure ID
	4	INT32		label
	4	INT32		offset
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXElementSearch
	1	CARD8		opcode (X11 assigned)
	1	42		PEX opcode
	2	7+(2i+2e+p+p2)/4		request length
	4	CARD32		structure ID
	2			position whence
		0	Beginning
		1	Current
		2	End
	2			unused
	4	INT32		position offset
	4			direction
		0	Forward
		1	Backward
	4	i		number of elements in inclusion list
	4	e		number of elements in exclusion list
	2i	LISTofCARD16		inclusion list
	p			unused, p=pad(2i)
	2e	LISTofCARD16		exclusion list
	p2			unused, p2=pad(2e)
.De
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
=>
	1	1		Reply
	1			unused
	2	CARD16		sequence number
	4	0		reply length
	2			status
		1	NotFound
		2	Found
	2			unused
	4	CARD32		found offset
	16			unused
.De

.AC "Structure Editing" 2
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXStoreElements
	1	CARD8		opcode (X11 assigned)
	1	43		PEX opcode
	2	4+s/4		request length
	2	INT16		floating point format
	2			unused
	4	CARD32		structure ID
	4	n		number of output commands
				(let s be the total size of the output commands in bytes)
	s	LISTofOUTPUT_COMMAND		elements
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXDeleteElements
	1	CARD8		opcode (X11 assigned)
	1	44		PEX opcode
	2	6		request length
	4	CARD32		structure ID
	2			position1 whence
		0	Beginning
		1	Current
		2	End
	2			unused
	4	INT32		position1 offset
	2			position2 whence
		0	Beginning
		1	Current
		2	End
	2			unused
	4	INT32		position2 offset
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXDeleteElementsToLabel
	1	CARD8		opcode (X11 assigned)
	1	45		PEX opcode
	2	5		request length
	4	CARD32		structure ID
	2			position whence
		0	Beginning
		1	Current
		2	End
	2			unused
	4	INT32		position offset
	4	INT32		label
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXDeleteElementsBetweenLabels
	1	CARD8		opcode (X11 assigned)
	1	46		PEX opcode
	2	4		request length
	4	CARD32		structure ID
	4	INT32		label1
	4	INT32		label2
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXCopyElements
	1	CARD8		opcode (X11 assigned)
	1	47		PEX opcode
	2	9		request length
	4	CARD32		source structure ID
	2			source position1 whence
		0	Beginning
		1	Current
		2	End
	2			unused
	4	INT32		source position1 offset
	2			source position2 whence
		0	Beginning
		1	Current
		2	End
	2			unused
	4	INT32		source position2 offset
	4	CARD32		destination structure ID
	2			destination position whence
		0	Beginning
		1	Current
		2	End
	2 			unused
	4	INT32		destination position offset
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXChangeStructureReferences
	1	CARD8		opcode (X11 assigned)
	1	48		PEX opcode
	2	3		request length
	4	CARD32		old structure ID
	4	CARD32		new structure ID
.De
.bp

.AC "Name Sets" 1
.LP

.AC "Name Set Resource Management" 2
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXCreateNameSet
	1	CARD8		opcode (X11 assigned)
	1	49		PEX opcode
	2	2		request length
	4	CARD32		nameset ID
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXCopyNameSet
	1	CARD8		opcode (X11 assigned)
	1	50		PEX opcode
	2	3		request length
	4	CARD32		source nameset ID
	4	CARD32		destination nameset ID
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXFreeNameSet
	1	CARD8		opcode (X11 assigned)
	1	51		PEX opcode
	2	2		request length
	4	CARD32		nameset ID
.De

.AC "Name Set Inquiry" 2
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXGetNameSet
	1	CARD8		opcode (X11 assigned)
	1	52		PEX opcode
	2	2		request length
	4	CARD32		nameset ID
.De
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
=>
	1	1		Reply
	1			unused
	2	CARD16		sequence number
	4	n		reply length
	4	n		number of names
	20			unused
	4n	LISTofNAME		names
.De

.AC "Name Set Modification" 2
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXChangeNameSet
	1	CARD8		opcode (X11 assigned)
	1	53		PEX opcode
	2	3+n		request length
	4	CARD32		nameset ID
	2			action
		0	Add
		1	Remove
		2	Replace
	2			unused
	4n	LISTofNAME		names (length implicit)
.bp

.AC "Search Contexts" 1
.LP
.Ds 0
.TA .2i 1i
.ta .2i 1i
SC_BITMASK
.sp 1
	#x0001	search_pos
	#x0002	search_dist
	#x0004	search_ceiling
	#x0008	start_path
	#x0010	normal_list
	#x0020	inverted_list
.De
.LP
Note that lengths are the (perhaps padded) lengths sent over the wire, and
are chosen to make entries align. Types may be smaller than the length field
into which they are assigned. If the natural type is smaller than the padded
field, this is equivalent to sending the value in its native type and then
a pad of (length - native size) bytes.
If a component has sub-fields, the sizes, types and interpretations of the
sub-fields are on subsequent lines.
.LP
.Ds 0
.TA 2i 2.3i 2.8i 3.5i 4i
.ta 2i 2.3i 2.8i 3.5i 4i
.R
\fBAttribute Name		Length	Type\fP
.sp 1
search_pos		12
		4	FLOAT		x
		4	FLOAT		y
		4	FLOAT		z
search_dist		4	FLOAT
search_ceiling		4	CARD16
start_path		4+8n
		4	n		length of LIST
		8n	LISTofELEMENT_REF
normal_list		4+8n
		4	n		length of LIST
		8n	LISTofNAME_SET_PAIR
inverted_list		4+8n
		4	n		length of LIST
		8n	LISTofNAME_SET_PAIR

.AC "Search Context Resource Management" 2
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXCreateSearchContext
	1	CARD8		opcode (X11 assigned)
	1	54		PEX opcode
	2	4+n/4		request length
	2	INT16		floating point format
	2			unused
	4	CARD32		search context ID
	4	SC_BITMASK		item mask
	n	LISTofVALUE		item list
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXCopySearchContext
	1	CARD8		opcode (X11 assigned)
	1	55		PEX opcode
	2	4		request length
	4	CARD32		source search context ID
	4	CARD32		destination search context ID
	4	SC_BITMASK		item mask
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXFreeSearchContext
	1	CARD8		opcode (X11 assigned)
	1	56		PEX opcode
	2	2		request length
	4	CARD32		search context ID
.De

.AC "Search Context Inquiry" 2
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXGetSearchContext
	1	CARD8		opcode (X11 assigned)
	1	57		PEX opcode
	2	4		request length
	2	INT16		floating point format
	2			unused
	4	CARD32		search context ID
	4	SC_BITMASK		item mask
.De
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
=>
	1	1		Reply
	1			unused
	2	CARD16		sequence number
	4	n/4		reply length
	4			unused
.mc |
	4	CARD32		number of items in reply
.mc
	16			unused
	n	LISTofVALUE		item list
.De

.AC "Search Context Modification" 2
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXChangeSearchContext
	1	CARD8		opcode (X11 assigned)
	1	58		PEX opcode
	2	4+n/4		request length
	2	INT16		floating point format
	2			unused
	4	CARD32		search context ID
	4	SC_BITMASK		item mask
	n	LISTofVALUE		item list
.De

.AC "Structure Network Searching" 2
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXSearchNetwork
	1	CARD8		opcode (X11 assigned)
	1	59		PEX opcode
	2	2		request length
	4	CARD32		search context ID
.De
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
=>
	1	1		Reply
	1			unused
	2	CARD16		sequence number
	4	2n		reply length
	4			unused
	4	n		number of element references
	16			unused
	8n	LISTofELEMENT_REF		found path
.De
.bp

.AC "PHIGS Workstations" 1
.LP

.AC "PHIGS Workstation Resource Management" 2
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXCreatePhigsWKS
	1	CARD8		opcode (X11 assigned)
	1	60		PEX opcode
	2	18		request length
	4	CARD32		PHIGS workstation ID
	4	CARD32		drawable ID
	4	CARD32		marker bundle ID
	4	CARD32		text bundle ID
	4	CARD32		line bundle ID
	4	CARD32		interior bundle ID
	4	CARD32		edge bundle ID
	4	CARD32		color table ID
	4	CARD32		depth cue table ID
	4	CARD32		light table ID
	4	CARD32		color approximation table ID
	4	CARD32		pattern table ID
	4	CARD32		text font table ID
	4	CARD32		highlight inclusion ID
	4	CARD32		highlight exclusion ID
	4	CARD32		invisibility inclusion ID
	4	CARD32		invisibility exclusion ID
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXFreePhigsWKS
	1	CARD8		opcode (X11 assigned)
	1	61		PEX opcode
	2	2		request length
	4	CARD32		PHIGS workstation ID
.De

.AC "PHIGS Workstation Inquiry" 2
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXGetWKSInfo
	1	CARD8		opcode (X11 assigned)
	1	62		PEX opcode
	2	4		request length
	2	INT16		floating point format
	2			unused
	4	CARD32		PHIGS workstation ID
	4			item mask
		#x00000001	display_update
		#x00000002	visual_state
		#x00000004	display_surface
		#x00000008	view_update
		#x00000010	defined_views
		#x00000020	wks_update
		#x00000040	req_NPC_subvolume
		#x00000080	cur_NPC_subvolume
		#x00000100	req_wks_viewpt
		#x00000200	cur_wks_viewpt
		#x00000400	HLHSR_update
		#x00000800	req_HLHSR_mode
		#x00001000	cur_HLHSR_mode
		#x00002000	drawable_id
		#x00004000	marker_bundle
		#x00008000	text_bundle
		#x00010000	line_bundle
		#x00020000	interior_bundle
		#x00040000	edge_bundle
		#x00080000	color_table
		#x00100000	depth_cue_table
		#x00200000	light_table
		#x00400000	color_approx_table
		#x00800000	pattern_table
		#x01000000	text_font_table
		#x02000000	highlight_inclusion
		#x04000000	highlight_exclusion
		#x08000000	invisibility_inclusion
		#x10000000	invisibility_exclusion
		#x20000000	posted_structs
		#x40000000	num_priorities
.De
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
=>
	1	1		Reply
	1			unused
	2	CARD16		sequence number
	4	n/4		reply length
	8			unused
.mc |
	4	CARD32		number of values in list
.mc
	12			unused
	n	LISTofVALUE		item list (see below)
.De
.LP
Note that lengths are the (perhaps padded) lengths sent over the wire, and
are chosen to make entries align. Types may be smaller than the length field
into which they are assigned. If the natural type is smaller than the padded
field, this is equivalent to sending the value in its native type and then
a pad of (length - native size) bytes.
If a component has sub-fields, the sizes, types and interpretations of the
sub-fields are on subsequent lines.
.LP
.Ds 0
.TA 2i 2.3i 2.8i 3.5i 4i
.ta 2i 2.3i 2.8i 3.5i 4i
.R
\fBAttribute Name		Length	Type\fP
.sp 1
display_update		4	INT16
visual_state		4	CARD8
			0	Correct
			1	Deferred
			2	Simulated
display_surface		4	CARD8
			0	Empty
			1	NotEmpty
view_update		4	CARD8
			0	NotPending
			1	Pending
defined_views		4+4n
		4	n	length
		4n	LISTofCARD16
wks_update		4	CARD8
			0	NotPending
			1	Pending
req_NPC_subvolume		24
		4	FLOAT		min x
		4	FLOAT		min y
		4	FLOAT		min z
		4	FLOAT		max x
		4	FLOAT		max y
		4	FLOAT		max z
cur_NPC_subvolume		24
		4	FLOAT		min x
		4	FLOAT		min y
		4	FLOAT		min z
		4	FLOAT		max x
		4	FLOAT		max y
		4	FLOAT		max z
req_wks_viewpt		20
		2	INT16		min x
		2	INT16		min y
		4	FLOAT		min z
		2	INT16		max x
		2	INT16		max y
		4	FLOAT		max z
		1	CARD8		use drawable
			0	False
			1	True
		3	unused
cur_wks_viewpt		20
		2	INT16		min x
		2	INT16		min y
		4	FLOAT		min z
		2	INT16		max x
		2	INT16		max y
		4	FLOAT		max z
		1	CARD8		use drawable
			0	False
			1	True
		3	unused
HLHSR_update		4	CARD8
			0	NotPending
			1	Pending
req_HLHSR_mode		4	INT16
cur_HLHSR_mode		4	INT16
drawable_id		4	CARD32
marker_bundle		4	CARD32
text_bundle		4	CARD32
line_bundle		4	CARD32
interior_bundle		4	CARD32
edge_bundle		4	CARD32
color_table		4	CARD32
depth_cue_table		4	CARD32
light_table		4	CARD32
color_approx_table		4	CARD32
pattern_table		4	CARD32
text_font_table		4	CARD32
highlight_inclusion		4	CARD32
highlight_exclusion		4	CARD32
invisibility_inclusion		4	CARD32
invisibility_exclusion		4	CARD32
posted_structs		4+8n
		4	n		length of list
		8n	LISTofSTRUCTURE_INFO
num_priorities		4	CARD32
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXGetDynamics
	1	CARD8		opcode (X11 assigned)
	1	63		PEX opcode
	2	2		request length
	4	CARD32		drawable ID
.De
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
=>
	1	1		Reply
	1			unused
	2	CARD16		sequence number
	4	0		reply length
	1			view rep
	1			marker bundle
	1			text bundle
	1			line bundle
	1			interior bundle
	1			edge bundle
	1			color table
	1			pattern table
	1			workstation transform
	1			highlight filter
	1			invisibility filter
	1			HLHSR mode
	1			structure modify
	1			post structure
	1			unpost structure
	1			delete structure
	1			reference modify
	7			unused
.De
.LP
The following constants are used for the reply fields above:
.LP
.Ds 0
.TA .5i 1i
.ta .5i 1i
.R
	0	IMM
	1	IRG
	2	CBS
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXGetViewRep
	1	CARD8		opcode (X11 assigned)
	1	64		PEX opcode
	2	3		request length
	2	INT16		floating point format
	2	CARD16		view index
	4	CARD32		PHIGS workstation ID
.De
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
=>
	1	1		Reply
	1			unused
	2	CARD16		sequence number
	4	80		reply length
	2			view update
		0	NotPending
		1	Pending
	22			unused
	2	CARD16		requested view index
	2			unused
	2			requested clip flags
		#x0001	clip_xy
		#x0002	clip_back
		#x0004	clip_front
	2			unused
	4	FLOAT		requested clip limits min x
	4	FLOAT		requested clip limits min y
	4	FLOAT		requested clip limits min z
	4	FLOAT		requested clip limits max x
	4	FLOAT		requested clip limits max y
	4	FLOAT		requested clip limits max z
	64	FLOAT[4][4]		requested orientation
	64	FLOAT[4][4]		requested mapping
	2	CARD16		current view index
	2			unused
	2			current clip flags
		#x0001	clip_xy
		#x0002	clip_back
		#x0004	clip_front
	2			unused
	4	FLOAT		current clip limits min x
	4	FLOAT		current clip limits min y
	4	FLOAT		current clip limits min z
	4	FLOAT		current clip limits max x
	4	FLOAT		current clip limits max y
	4	FLOAT		current clip limits max z
	64	FLOAT[4][4]		current orientation
	64	FLOAT[4][4]		current mapping
.De

.AC "PHIGS Workstation Manipulation" 2
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXRedrawAllStructures
	1	CARD8		opcode (X11 assigned)
	1	65		PEX opcode
	2	2		request length
	4	CARD32		PHIGS workstation ID
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXUpdateWorkstation
	1	CARD8		opcode (X11 assigned)
	1	66		PEX opcode
	2	2		request length
	4	CARD32		PHIGS workstation ID
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXRedrawClipRegion
	1	CARD8		opcode (X11 assigned)
	1	92		PEX opcode
	2	3+2n		request length
	4	CARD32		PHIGS workstation ID
	4	n		number of device rectangles
	8n	LISTofDEVICE_RECT		clip list
.De
.IP
\fINote:\fR \fBPEXRedrawClipRegion\fP \fIis the highest-valued request
opcode that has been defined.\fR
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXExecuteDeferredActions
	1	CARD8		opcode (X11 assigned)
	1	67		PEX opcode
	2	2		request length
	4	CARD32		PHIGS workstation ID
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXSetViewPriority
	1	CARD8		opcode (X11 assigned)
	1	68		PEX opcode
	2	4		request length
	4	CARD32		PHIGS workstation ID
	2	CARD16		index1
	2	CARD16		index2
	2			priority
		0	Higher
		1	Lower
	2			unused
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXSetDisplayUpdateMode
	1	CARD8		opcode (X11 assigned)
	1	69		PEX opcode
	2	3		request length
	4	CARD32		PHIGS workstation ID
	2	INT16		display update
	2			unused
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXMapDCtoWC
	1	CARD8		opcode (X11 assigned)
	1	70		PEX opcode
	2	4+2n		request length
	2	INT16		floating point format
	2			unused
	4	CARD32		PHIGS workstation ID
	4	n		number of coordinates
	8n	LISTofDEVICE_COORD		DC points
.De
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
=>
	1	1		Reply
	1			unused
	2	CARD16		sequence number
	4	3n		reply length
	2	CARD16		view index
	2			unused
	4	n		number of coordinates being returned
	16			unused
	12n	LISTofCOORD_3D		WC points
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXMapWCtoDC
	1	CARD8		opcode (X11 assigned)
	1	71		PEX opcode
	2	4+3n		request length
	2	INT16		floating point format
	2	CARD16		view index
	4	CARD32		PHIGS workstation ID
	4	n		number of coordinates
	12n	LISTofCOORD_3D		WC points
.De
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
=>
	1	1		Reply
	1			unused
	2	CARD16		sequence number
	4	2n		reply length
	4			unused
	4	n		number of coordinates being returned
	16			unused
	8n	LISTofDEVICE_COORD		DC points
.De

.AC "PHIGS Workstation Update" 2
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXSetViewRep
	1	CARD8		opcode (X11 assigned)
	1	72		PEX opcode
	2	43		request length
	2	INT16		floating point format
	2			unused
	4	CARD32		PHIGS workstation ID
	2	CARD16		view rep index
	2			unused
	2			view rep clip flags
		#x0001	clip_xy
		#x0002	clip_back
		#x0004	clip_front
	2			unused
	4	FLOAT		view rep clip limits min x
	4	FLOAT		view rep clip limits min y
	4	FLOAT		view rep clip limits min z
	4	FLOAT		view rep clip limits max x
	4	FLOAT		view rep clip limits max y
	4	FLOAT		view rep clip limits max z
	64	FLOAT[4][4]		view rep orientation
	64	FLOAT[4][4]		view rep mapping
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXSetWKSWindow
	1	CARD8		opcode (X11 assigned)
	1	73		PEX opcode
	2	9		request length
	2	INT16		floating point format
	2			unused
	4	CARD32		PHIGS workstation ID
	4	FLOAT		NPC subvolume min x
	4	FLOAT		NPC subvolume min y
	4	FLOAT		NPC subvolume min z
	4	FLOAT		NPC subvolume max x
	4	FLOAT		NPC subvolume max y
	4	FLOAT		NPC subvolume max z
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXSetWKSViewport
	1	CARD8		opcode (X11 assigned)
	1	74		PEX opcode
	2	8		request length
	2	INT16		floating point format
	2			unused
	4	CARD32		PHIGS workstation ID
	2	INT16		viewport min x
	2	INT16		viewport min y
	4	FLOAT		viewport min z
	2	INT16		viewport max x
	2	INT16		viewport max y
	4	FLOAT		viewport max z
	1			viewport use drawable
		0	False
		1	True
	3			unused
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXSetHLHSRMode
	1	CARD8		opcode (X11 assigned)
	1	75		PEX opcode
	2	3		request length
	4	CARD32		PHIGS workstation ID
	2	INT16		mode
	2			unused
.De

.AC "Posting/Unposting Structures" 2
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXPostStructure
	1	CARD8		opcode (X11 assigned)
	1	76		PEX opcode
	2	5		request length
	2	INT16		floating point format
	2			unused
	4	CARD32		PHIGS workstation ID
	4	CARD32		structure ID
	4	FLOAT		priority
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXUnpostStructure
	1	CARD8		opcode (X11 assigned)
	1	77		PEX opcode
	2	3		request length
	4	CARD32		PHIGS workstation ID
	4	CARD32		structure ID
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXUnpostAllStructures
	1	CARD8		opcode (X11 assigned)
	1	78		PEX opcode
	2	2		request length
	4	CARD32		PHIGS workstation ID
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXGetWKSPostings
	1	CARD8		opcode (X11 assigned)
	1	79		PEX opcode
	2	2		request length
	4	CARD32		structure ID
.De
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
=>
	1	1		Reply
	1			unused
	2	CARD16		sequence number
	4	n		reply length
	24			unused
	4n	LISTofPHIGS_WKS_ID		PHIGS workstation IDs (length implicit)
.De
.bp

.AC "Picking" 1
.LP
.Ds 0
.TA .2i 1i
.ta .2i 1i
P_BITMASK
.sp 1
	#x0001	pick_status
	#x0002	pick_path
	#x0004	pick_path_order
	#x0008	pick_inclusion
	#x0010	pick_exclusion
	#x0020	pick_data_rec
	#x0040	prompt_echo_type
	#x0080	echo_volume
	#x0100	echo_switch
.De
.LP
Note that lengths are the (perhaps padded) lengths sent over the wire, and
are chosen to make entries align. Types may be smaller than the length field
into which they are assigned. If the natural type is smaller than the padded
field, this is equivalent to sending the value in its native type and then
a pad of (length - native size) bytes.
If a component has sub-fields, the sizes, types and interpretations of the
sub-fields are on subsequent lines.
.LP
.Ds 0
.TA 2i 2.3i 2.8i 3.5i 4i
.ta 2i 2.3i 2.8i 3.5i 4i
.R
\fBAttribute Name		Length	Type\fP
pick_status		4	CARD16
			0	NoPick
			1	Ok
pick_path		4+12n
		4	n		number of pick element references
		12n	LISTofPICK_ELEMENT_REF
pick_path_order		4	CARD16
			0	TopFirst
			1	BottomFirst
pick_inclusion		4	CARD32
pick_exclusion		4	CARD32
pick_data_rec		4+n+p
		4	n		length
		n	LISTofCARD8
		p			unused, p=pad(n)
prompt_echo_type		4	INT16
echo_volume		20
		2	INT16		min x
		2	INT16		min y
		4	FLOAT		min z
		2	INT16		max x
		2	INT16		max y
		4	FLOAT		max z
		1	CARD8		use drawable
			0	False
			1	True
		3			unused
echo_switch		4	CARD16
			0	NoEcho
			1	Echo
.De

.AC "Pick Device Descriptors" 2
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXGetPickDevice
	1	CARD8		opcode (X11 assigned)
	1	80		PEX opcode
	2	4		request length
	2	INT16		floating point format
	2	INT16		device type
	4	CARD32		PHIGS workstation ID
	4	P_BITMASK		item mask
.De
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
=>
	1	1		Reply
	1			unused
	2	CARD16		sequence number
	4	n/4		reply length
	4			unused
.mc |
	4	CARD32		number of items in list
.mc
	16			unused
	n	LISTofVALUE		item list
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXChangePickDevice
	1	CARD8		opcode (X11 assigned)
	1	81		PEX opcode
	2	5+n/4		request length
	2	CARD16		floating point format
	2			unused
	4	CARD32		PHIGS workstation ID
	2	INT16		device type
	2			unused
	4	P_BITMASK		item mask
	n	LISTofVALUE		item list
.De

.AC "Pick Measure Resource Management" 2
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXCreatePickMeasure
	1	CARD8		opcode (X11 assigned)
	1	82		PEX opcode
	2	4		request length
	4	CARD32		PHIGS workstation ID
	4	CARD32		pick measure ID
	2	INT16		device type
	2			unused
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXFreePickMeasure
	1	CARD8		opcode (X11 assigned)
	1	83		PEX opcode
	2	2		request length
	4	CARD32		pick measure ID
.De

.AC "Pick Measure Inquiry" 2
.LP
.Ds 0
.TA .2i 1i
.ta .2i 1i
PM_BITMASK
.sp 1
	#x0001	pick_status
	#x0002	picked_prim
.De
.LP
Note that lengths are the (perhaps padded) lengths sent over the wire, and
are chosen to make entries align. Types may be smaller than the length field
into which they are assigned. If the natural type is smaller than the padded
field, this is equivalent to sending the value in its native type and then
a pad of (length - native size) bytes.
If a component has sub-fields, the sizes, types and interpretations of the
sub-fields are on subsequent lines.
.LP
.Ds 0
.TA 2i 2.3i 2.8i 3.5i 4i
.ta 2i 2.3i 2.8i 3.5i 4i
.R
\fBAttribute Name		Length	Type\fP
.sp 1
pick_status		4	CARD16
			0	NoPick
			1	Ok
picked_prim		4+12n
		4	n	number of pick element references
		12n	LISTofPICK_ELEMENT_REF
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXGetPickMeasure
	1	CARD8		opcode (X11 assigned)
	1	84		PEX opcode
	2	3		request length
	4	CARD32		pick measure ID
	4	PM_BITMASK		item mask
.De
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
=>
	1	1		Reply
	1			unused
	2	CARD16		sequence number
	4	n/4		reply length
	4			unused
	4 	CARD32		number of items
	16			unused
	n	LISTofVALUE		item list
.De

.AC "Pick Operations" 2
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXUpdatePickMeasure
	1	CARD8		opcode (X11 assigned)
	1	85		PEX opcode
	2	3+(n+p)/4		request length
	4	CARD32		pick measure ID
	4	n		length of list
	n	LISTofCARD8		input record
	p			unused, p=pad(n)
.De
.bp

.AC "PEX Fonts" 1
.LP

.AC "PEX Font Resource Management" 2
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXOpenFont
	1	CARD8		opcode (X11 assigned)
	1	86		PEX opcode
	2	3+(n+p)/4		request length
	4	CARD32		font ID
	4	n		length of name
	n	LISTofCARD8		name
	p			unused, p=pad(n)
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXCloseFont
	1	CARD8		opcode (X11 assigned)
	1	87		PEX opcode
	2	2		length
	4	CARD32		font ID
.De

.AC "PEX Font Inquiry" 2
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXQueryFont
	1	CARD8		opcode (X11 assigned)
	1	88		PEX opcode
	2	3		request length
	2	INT16		floating point format
	2			unused
	4	CARD32		font ID
.De
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
=>
	1	1		Reply
	1			unused
	2	CARD16		sequence number
	4	f/4		reply length
	4	f		length of FONTINFO
	20			unused
	f	FONTINFO		font info
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXListFonts
	1	CARD8		opcode (X11 assigned)
	1	89		PEX opcode
	2	2+(n+p)/4		request length
	2	CARD16		max names
	2	n		number of bytes in pattern
	n	LISTofCARD8		pattern
	p			unused, p=pad(n)
.De
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
=>
	1	1		Reply
	1			unused
	2	CARD16		sequence number
	4	r/4		reply length
	4	CARD32		number of STRINGs in list
	20			unused
	r	LISTofSTRING		font names
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXListFontsWithInfo
	1	CARD8		opcode (X11 assigned)
	1	90		PEX opcode
	2	3+(n+p)/4		request length
	2	INT16		floating point format
	2	CARD16		max names
	2	n		number of characters in pattern
	2			unused
	n	LISTofCARD8		pattern
	p			unused, p=pad(n)
.De
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
=>
	1	1		Reply
	1			unused
	2	CARD16		sequence number
	4	1+(r+f)/4		reply length
	4	CARD32		number of STRINGs in list
	20			unused
	r	LISTofSTRING		font names
	4	CARD32		number of FONTINFO in list
	f	LISTofFONTINFO		fonts
.De
.LP
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
.PN PEXQueryTextExtents
	1	CARD8		opcode (X11 assigned)
	1	91		PEX opcode
	2	8+(q+p)/4		request length
	2	INT16		floating point format
	2			path
		0	PathRight
		1	PathLeft
		2	PathUp
		3	PathDown
	4	CARD32		font ID
	4	FLOAT		expansion
	4	FLOAT		spacing
	4	FLOAT		height
	2			alignment vertical
		0	ValignNormal
		1	ValignTop
		2	ValignCap
		3	ValignHalf
		4	ValignBase
		5	ValignBottom
	2			alignment horizontal
		0	HalignNormal
		1	HalignLeft
		2	HalignCenter
		3	HalignRight
	4	n		number of strings
	q	LISTofLISTofCARD8		strings
	p			unused, p=pad(n)
.De
.Ds 0
.TA .2i .6i 1.6i 2.6i
.ta .2i .6i 1.6i 2.6i
.R
=>
	1	1		Reply
	1			unused
	2	CARD16		sequence number
	4	6n		reply length
	24			unused
	24n	LISTofEXTENT_INFO		extents (length implicit)
.De
\fBNote: PEXRedrawClipRegion\fP \fIis the last request for which an opcode has
been assigned (92).\fR
