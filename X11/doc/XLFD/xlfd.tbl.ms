.\" Use tbl and -ms
.\" $XConsortium: xlfd.tbl.ms,v 1.1 89/11/16 18:12:10 rws Exp $
.\" The following is a copy of macros.t from the Protocol
.na
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
.de FD
.LP
.KS
.TA .5i 3i
.ta .5i 3i
.nf
..
.de FN
.fi
.KE
.LP
..
.de IN		\" send an index entry to the stderr
.tm \\n%:\\$1:\\$2:\\$3
..
.de C{
.KS
.nf
.D
.\"
.\"	choose appropriate monospace font
.\"	the imagen conditional, 480,
.\"	may be changed to L if LB is too
.\"	heavy for your eyes...
.\"
.ie "\\*(.T"480" .ft L
.el .ie "\\*(.T"300" .ft L
.el .ie "\\*(.T"202" .ft PO
.el .ie "\\*(.T"aps" .ft CW
.el .ft R
.ps \\n(PS
.ie \\n(VS>40 .vs \\n(VSu
.el .vs \\n(VSp
..
.de C}
.DE
.R
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
.de NT
.ne 7
.ds NO Note
.if \\n(.$>$1 .if !'\\$2'C' .ds NO \\$2
.if \\n(.$ .if !'\\$1'C' .ds NO \\$1
.ie n .sp
.el .sp 10p
.TB
.ce
\\*(NO
.ie n .sp
.el .sp 5p
.if '\\$1'C' .ce 99
.if '\\$2'C' .ce 99
.in +5n
.ll -5n
.R
..
.		\" Note End -- doug kraft 3/85
.de NE
.ce 0
.in -5n
.ll +5n
.ie n .sp
.el .sp 10p
..
.ny0
.EH ''''
.OH ''''
.EF ''''
.OF ''''
.ps 11
.nr PS 11
\&
.sp 8
.ce 5
\s+2\fBX Logical Font Description Conventions\fP\s-2

\s-1\fBVersion 1.3\fP\s-1

\s+1\fBMIT X Consortium Standard\fP\s-1
.sp 10
.ps 9
.nr PS 9
.LP
Copyright \(co 1988 
Massachusetts Institute of Technology, 
Cambridge, MA.
.LP
Copyright \(co 1988, 1989 
Digital Equipment Corporation, Maynard MA. All rights reserved.
.LP 
Permission to use, copy, modify, and distribute this documentation 
for any purpose and without fee is hereby granted, provided 
that the above copyright notice and this permission 
notice appear in all copies.
MIT and Digital Equipment Corporation make no representations about the 
suitability for any purpose of the information in this document. 
This documentation is provided as is without express or implied warranty. 
.sp 8
.LP
X Window System is a trademark of M.I.T.
.LP             
Helvetica and Times are registered trademarks of Linotype Company.
.LP
ITC Avant Garde Gothic is a registered trademark of International 
Typeface Corporation.
.LP
Times Roman is a registered trademark of Monotype Corporation.
.LP
Bitstream Amerigo is a registered trademark of Bitstream Inc.
.LP             
Stone is a registered trademark of Adobe Systems Inc.
.ps 11
.nr PS 11
.bp
.EH '\fBX Logical Font Description Conventions\fP''\fBX11, Release 4'
.OH '\fBX Logical Font Description Conventions\fP''\fBX11, Release 4'
.EF ''\fB % \fP''
.OF ''\fB % \fP''
.NH 1 
Introduction
.XS
\*(SN Introduction
.XE
.LP 
It is a requirement that X client applications must be portable across server
implementations, with very different file systems, naming conventions, and 
font libraries.
However, font access requests, 
as defined by the X Window System Protocol, Version 11, 
do not specify server-independent conventions for font names, 
nor provide adequate font properties for logically describing typographic fonts.
.LP
X clients must be able to dynamically determine fonts 
available on any given server, in what sizes, styles, etc., so that 
understandable information can be presented to the user, or that intelligent
font fallbacks can be chosen.
It is desirable that the most common queries could be accomplished 
without the overhead of opening each font and inspection of font properties, 
i.e., through simple 
.PN ListFonts 
requests.
(E.g., if a user selected a Helvetica typeface family, 
a client application should be able to query the server 
for all Helvetica fonts and present only those setwidths, weights, slants, 
point sizes, character sets available for that family).
.LP
This document gives a standard logical font description 
(XLFD) and conventions to be used in the X protocol 
so that clients can query and access screen type libraries 
in a consistent manner across all X servers.
In addition to completely specifying a given font via its 
.PN FontName ,
the XLFD also provides for a standard set of key 
.PN FontProperties
that describe the font in more detail. 
.LP
The XLFD provides an adequate set of typographic font properties, 
such as \s-1CAP_HEIGHT\s+1, \s-1X_HEIGHT\s+1, \s-1RELATIVE_SETWIDTH\s+1, 
for publishing and other applications to do intelligent font matching 
or substitution when handling documents created on some foreign server 
using potentially unknown fonts.
In addition, 
this information is required by certain clients to automatically place 
subscripts, determine small capital heights, recommended leading, 
wordspace values, etc.
.LP
Examples are for illustrative purposes only.
.NH 1
Requirements and Goals
.XS
\*(SN Requirements and Goals
.XE
.LP 
This specification meets the short and long-term goals to have a 
standard logical font description which:
.IP \(bu 5
provides unique, descriptive font names that support simple pattern matching;
.IP \(bu 5
supports multiple font vendors, arbitrary character sets and encodings;
.IP \(bu 5
is X server and operating/file system independent;
.IP \(bu 5
provides adequate descriptive font information for arbitrarily complex font 
matching/substitution;
.IP \(bu 5
is extensible.
.NH 2
Unique, Descriptive Font Names
.XS
\*(SN Unique, Descriptive Font Names
.XE
.LP
It should be possible to have font names that are long enough and 
descriptive enough to have a reasonable probability of being unique 
without inventing a new registration organization.
Resolution/size-dependent font masters, multi-vendor font libraries, 
etc., must be anticipated and handled by the font name alone.
.LP
The name itself should be structured to be amenable to simple pattern 
matching and parsing, allowing X clients to restrict font queries to 
some subset of all possible fonts in the server.
.NH 2
Support Multiple Font Vendors and Character Sets
.XS
\*(SN Support Multiple Font Vendors and Character Sets
.XE
.LP
The font name and properties should distinguish between fonts 
that were supplied by different font vendors 
but that possibly share the same name. 
We anticipate a highly competitive font market where users will be able to 
buy fonts from a number of sources according to their particular requirements.
.LP
A number of font vendors deliver each font with all glyphs designed for that
font, where charset mappings are defined by encoding vectors.
Some server implementations may force these mappings to proprietary 
or standard charsets statically in the font data, 
while others may desire to perform the mapping dynamically in the server.
Provisions must be made in the font name 
which allows a font request to specify/identify specific charset mappings 
in server environments where multiple charsets are supported.
.NH 2
Server, Operating and File System Independent
.XS
\*(SN Server, Operating and File System Independent
.XE
.LP
X client applications that require a particular font should be able to use 
the descriptive name, without knowledge of the file system or other 
repository in use by the server.
However, 
it should be possible for servers to translate a given font name 
into a file name syntax that it knows how to deal with, 
without compromising the uniqueness of the font name.
This algorithm should be reversible (exactly how this translation is done is 
implementation dependent).
.NH 2
Supports Arbitrarily Complex Font Matching/Substitution
.XS
\*(SN Supports Arbitrarily Complex Font Matching/Substitution
.XE
.LP
In addition to the font name, 
the XLFD should define a standard list of descriptive font properties 
with agreed upon fallbacks for all fonts, so that client applications 
can derive font-specific formatting/display data, 
perform font matching/substitution 
when asked to handle potentially unknown fonts, as required.
.NH 2
Extensible
.XS
\*(SN Extensible
.XE
.LP
The XLFD must be extensible so that new and/or private descriptive font 
properties can be added to conforming fonts without obsoleting existing 
X client or server implementations.
.NH 1
X Logical Font Description
.XS
\*(SN X Logical Font Description
.XE
.LP
XLFD is divided into two basic components: 
the 
.PN FontName , 
which gives all font information needed to uniquely identify a font 
in X protocol requests (e.g.,
.PN OpenFont , 
.PN ListFonts , 
etc.) and a variable list of optional 
.PN FontProperties 
that describe a font in more detail.
.LP
The 
.PN FontName 
is used in font queries and returned as data in certain X protocol requests.
The
.PN FontName 
is also specified as the data value for the 
.PN FONT
item in the X Consortium Character Bitmap Distribution Format specification 
(BDF V2.1).
.LP
The 
.PN FontProperties 
are supplied on a font-by-font basis, and are returned 
as data in certain X protocol requests as part of the 
.PN XFontStruct
data structure.
The
.PN FontProperties 
names and associated data values may also appear as items of the 
\s-1\fBSTARTPROPERTIES\fP\s+1...\s-1\fBENDPROPERTIES\fP\s+1 list 
in the BDF V2.1 specification.
.NH 2
FontName
.XS
\*(SN FontName
.XE
.LP
The
.PN FontName 
is logically composed of two strings: a 
.PN FontNameRegistry
prefix, followed by a 
.PN FontNameSuffix . 
The 
.PN FontNameRegistry
is an x-registered-name 
that identifies the registration authority that owns the specified 
.PN FontNameSuffix
syntax and semantics.
.LP
All font names that conform to this specification are to use a 
.PN FontNameRegistry
prefix defined to be the string ``\-'', 
i.e., ISO 8859-1 HYPHEN (Column/Row 02/13). 
All 
.PN FontNameRegistry 
prefixes of the form ``+\fIversion\fP\-'', 
where \fIversion\fP is the version of some future XLFD specification, 
are reserved by the X Consortium for future extensions to XLFD font names.
If required, extensions to the current XLFD font name shall be constructed 
by appending new fields to the current structure, 
each delimited by the existing field delimiter.
The availability of other 
.PN FontNameRegistry
prefixes or fonts that support other registries 
is server implementation dependent.
.LP
In the X protocol specification, 
the 
.PN FontName 
is required to be a string; 
hence, numeric field values are represented in the name as string equivalents.
All 
.PN FontNameSuffix 
fields are also defined as 
.PN FontProperties , 
in which case numeric property values are represented as signed 
or unsigned integers as appropriate.
.NH 3
FontName Syntax
.LP
The
.PN FontName 
is a structured, parsable string (X data type STRING8) 
whose Backus-Naur Form syntax description is as follows:
.IN "FontName Syntax"
.ps 9
.nr PS 9
.TS 
rw(1.5i) lw(3.75i).
.sp 6p
T{
FontName ::=
T}	T{
XFontNameRegistry XFontNameSuffix | 
PrivFontNameRegistry PrivFontNameSuffix
T}
T{
XFontNameRegistry ::=
T}	T{
XFNDelim | XFNExtPrefix Version XFNDelim
T}
T{
XFontNameSuffix ::=
T}	T{
FOUNDRY XFNDelim FAMILY_NAME XFNDelim WEIGHT_NAME
XFNDelim SLANT XFNDelim SETWIDTH_NAME XFNDelim ADD_
STYLE_NAME XFNDelim PIXEL_SIZE XFNDelim POINT_SIZE 
XFNDelim RESOLUTION_X XFNDelim RESOLUTION_Y XFNDelim 
SPACING XFNDelim AVERAGE_WIDTH XFNDelim CHARSET_
REGISTRY XFNDelim CHARSET_ENCODING
T}
T{
Version ::=
T}	T{
STRING8 \- the XLFD version that defines an extension 
to the font name syntax (e.g., ``2.0'')
T}
T{
XFNExtPrefix ::=
T}	T{
OCTET \- the value of ISO8859-1 PLUS (Column/Row 02/13)
T}
T{
XFNDelim ::=
T}	T{
OCTET \- the value of ISO8859-1 HYPHEN (Column/Row 02/13)
T}
T{
PrivFontNameRegistry ::=
T}	T{
STRING8 \- other than those strings reserved by XLFD
T}
PrivFontNameSuffix ::=	STRING8
.TE
.ps 11
.nr PS 11
.LP
Field values are constructed as strings of ISO8859-1 graphic characters, 
excluding the following:
.IP \(bu 5
HYPHEN (02/13), the XLFD font name delimiter character;
.IP \(bu 5
QUESTION MARK (03/15) and ASTERISK (02/10), the X protocol 
fontname wildcard characters.
.LP
Alphabetic case distinctions are allowed, but are for human readability 
concerns only.
Conforming X servers will perform matching on font name query/open requests 
independent of case.
The entire font name string must have no more than 255 characters.
It is recommended that clients construct font name query patterns 
by explicitly including all field delimiters to avoid unexpected results.
Note, that SPACE is a valid character of a 
.PN FontName 
field; e.g., a FAMILY_NAME might be ITC Avant Garde Gothic.
.NH 3
FontName Field Definitions
.XS
\*(SN FontName Field Definitions
.XE
.NH 4
FOUNDRY : x-registered-name
.XS
\*(SN FOUNDRY : x-registered-name
.XE
.LP
FOUNDRY is an x-registered-name,
the name or identifier of the digital type foundry 
that digitized and supplied the font data, 
or if different, the identifier of the organization that last modified 
the font shape or metric information.
.LP
The reason this distinction is necessary is 
that a given font design may be licensed from one source (e.g., ITC) 
but digitized and sold by any number of different type suppliers.
Each digital version of the original design will in general be somewhat 
different in metrics and shape from the idealized original font data, 
as each font foundry, for better or for worse, has its own standards 
and practices for tweaking a typeface for a particular generation 
of output technologies, or has its own perception of market needs. 
.LP
It is up to the type supplier to register with the X Consortium a 
suitable name for this 
.PN FontName 
field, according to the registration procedures defined by the Consortium.
.LP
The X Consortium shall define procedures for registering foundry names, 
and shall maintain and publish in a timely manner 
a registry of such registered names for use in XLFD font names and properties.
.LP
.NH 4
FAMILY_NAME : string
.XS
\*(SN FAMILY_NAME : string
.XE
.LP
FAMILY_NAME is a string that identifies the range or ``family'' of 
typeface designs that are all variations of one basic typographic style. 
This must be spelled out in full, with words separated by spaces as required. 
This name must be human-understandable and suitable for presentation to a 
font user to identify the typeface family.
.LP
It is up to the type supplier to supply and maintain a suitable string for 
this field and font property, to secure the proper legal title to a given 
name, and to guard against the infringement of other's copyrights or 
trademarks.
By convention, FAMILY_NAME is not translated.
FAMILY_NAME may include an indication of design ownership 
if considered a valid part of the 
typeface family name (see examples below).
.LP
Examples of FAMILY_NAMEs:
.LP
.Ds
Helvetica
ITC Avant Garde Gothic 
Times
Times Roman
Bitstream Amerigo
Stone
.De
.NH 4
WEIGHT_NAME : string
.XS
\*(SN WEIGHT_NAME : string
.XE
.LP
WEIGHT_NAME is a string that identifies the font's typographic weight, 
i.e., the nominal blackness of the font, 
according to the FOUNDRY's judgement.
This name must be human-understandable and suitable for presentation to a 
font user.
.LP 
The interpretation of this field is somewhat problematic, 
as the typographic judgement of weight has traditionally 
depended on the overall design of the typeface family in question 
(i.e., it is possible that the DemiBold weight of one font could be 
almost equivalent in typographic feel to a Bold font from another family).
.LP
WEIGHT_NAME is captured as an arbitrary string 
since it is an important part of a font's complete human-understandable name, 
but it should not be used for font matching/substitution.
X client applications should use the weight-related font properties 
(RELATIVE_WEIGHT and WEIGHT) that give the coded relative weight, 
and the calculated weight, respectively, for this purpose.
.NH 4
SLANT : code-string
.XS
\*(SN SLANT : code-string
.XE
.LP
SLANT is a code-string that indicates the overall posture of the 
typeface design used in the font.
The encoding is as follows:
.TS H
lw(.5i) lw(1.25i) lw(3.5i).
_
.sp 6p
.B
Code	English Translation	Description
.sp 6p
_
.sp 6p
.TH
.R
``R''	Roman	Upright design
``I''	Italic	T{
Italic design, slanted clockwise from vertical
T}
``O''	Oblique	T{
Obliqued upright design, slanted clockwise from vertical
T}
``RI''	Reverse Italic	T{
Italic design, slanted counter clockwise from vertical
T}
``RO''	Reverse Oblique	T{
Obliqued upright design, slanted counter clockwise from vertical
T}
``OT''	Other	Other
.sp 6p
_
.TE
.LP
The SLANT codes are for programming convenience only, and usually are 
converted into their equivalent human-understandable form before being 
presented to a user.
.NH 4
SETWIDTH_NAME : string
.XS
\*(SN SETWIDTH_NAME : string
.XE
.LP 
SETWIDTH_NAME is a string that gives the font's typographic 
proportionate width, i.e., the nominal width per horizontal unit of the 
font, according to the FOUNDRY's judgment.
.LP
As with WEIGHT_NAME, the interpretation of this field or font property is 
somewhat problematic, as the designer's judgment of setwidth has 
traditionally depended on the overall design of the typeface family in 
question.
X client applications should use the RELATIVE_SETWIDTH font property 
which gives the relative coded proportionate width, or calculate 
the proportionate width, 
for purposes of font matching or substitution.
.LP
Examples of SETWIDTH_NAMEs:
.LP
.Ds
Normal 
Condensed 
Narrow 
Double Wide
.De
.NH 4
ADD_STYLE_NAME : string
.XS
\*(SN ADD_STYLE_NAME : string
.XE
.LP
ADD_STYLE_NAME is a string that identifies additional typographic 
style information not captured by other fields, but needed 
to uniquely identify the font.
.LP
ADD_STYLE_NAME is not a typeface classification field, 
and is only used for uniqueness. 
Its usage, as such, is not limited to typographic style distinctions.
.LP
Examples of ADD_STYLE_NAMEs:
.LP
.Ds
Serif
Sans Serif
Informal
Decorated
.De
.NH 4
PIXEL_SIZE : integer-string
.XS
\*(SN PIXEL_SIZE : integer-string
.XE
.LP 
PIXEL_SIZE is an unsigned integer-string typographic metric in 
device pixels which gives the body size of the font at a particular 
POINT_SIZE and RESOLUTION_Y.
PIXEL_SIZE normally incorporates additional vertical spacing 
considered part of the font design.
(Note, however, that this value is not necessarily equivalent to the height 
of the font bounding box).
PIXEL_SIZE is in the range zero to a ``very-large-number''.
.LP
PIXEL_SIZE would normally be used by X client applications that need to 
query fonts according to device-dependent size, 
regardless of the point size or vertical resolution 
the font was designed for.
.NH 4
POINT_SIZE : integer-string
.XS
\*(SN POINT_SIZE integer-string
.XE
.LP 
POINT_SIZE is an unsigned integer-string typographic metric in 
device-independent units which gives the body size 
the font was designed for.
This field normally incorporates additional vertical spacing 
considered part of the font design.
(Note, however, that POINT_SIZE is not necessarily equivalent to the height 
of the font bounding box). 
POINT_SIZE is expressed in decipoints (where points are as defined 
in the X protocol or 72.27 pts = 1 inch) in the range zero to a 
``very-large-number''.
.LP
POINT_SIZE and RESOLUTION_Y would be used by X clients to query fonts 
according to device-independent size, to maintain constant text 
size on the display regardless of the PIXEL_SIZE used for the font.
.NH 4
RESOLUTION_X : integer-string
.XS
\*(SN RESOLUTION_X : integer-string
.XE
.NH 4
RESOLUTION_Y : integer-string
.XS
\*(SN RESOULTION_Y : integer-string
.XE
.LP 
RESOLUTION_X and RESOLUTION_Y are unsigned integer-strings, 
the horizontal and vertical resolution that the font was designed for, 
measured in pixels/dots per inch (dpi).
Horizontal and vertical values are required 
since a separate bitmap font must be designed 
for displays with very different aspect ratios
(e.g., 1:1, 4:3, 2:1, etc.).
.LP 
The separation of pixel/point size and resolution is necessary 
because X allows for servers with very different video characteristics 
(e.g., horizontal and vertical resolution, screen and pixel size, 
pixel shape, etc.) to potentially access the same font library.
The font name, then, must differentiate between a 14 point font designed 
for 75 dpi (body size of about 14 pixels) or a 14 point font designed 
for 150 dpi (about 28 pixels), etc.
Further, 
in servers that implement some or all fonts as continuously scaled outlines,
POINT_SIZE and RESOLUTION_Y will help the server to differentiate 
between potentially separate font masters for text, title,
and display sizes or for other typographic considerations.
.NH 4 
SPACING : code-string
.XS
\*(SN SPACING : code-string
.XE
.LP 
SPACING is a code-string that indicates the escapement class of the font, 
i.e., monospace (fixed pitch), proportional (variable pitch), 
or charcell (a special monospaced font that conforms to the traditional 
data processing character cell font model).
.ps 9
.nr PS 9
.TS H
lw(.5i) lw(1.25i) lw(3.5i).
_
.sp 6p
.B
Code	English Translation	Description
.sp 6p
_
.sp 6p
.TH
.R
``P''	Proportional	T{
A font whose logical character widths vary for each glyph.
Note that no other restrictions are placed on the metrics 
of a proportional font.
T}
``M''	Monospaced	T{
A font whose logical character widths are constant 
(i.e., all char widths of the font are = max_bounds.width).
No other restrictions are placed on the metrics of a monospaced font.
T}
``C''	CharCell	T{
A monospaced font which follows the standard typewriter character cell model
(i.e., the glyphs of the font can be modeled by X clients as ``boxes'' 
of the same width and height which are imaged side by side 
to form text strings, or top to bottom to form text lines.
By definition, 
all glyphs have the same logical character width, 
and no glyphs have ``ink'' outside of the character cell\-there is no kerning 
(i.e., on a per char basis with positive metrics: 
0 <= left-bearing <= right-bearing <= width; 
with negative metrics: width <= left-bearing <= right-bearing <= 0)\-and 
the vertical extents of the font do not exceed the vertical spacing 
(i.e., on a per char basis: ascent <= font-ascent 
and descent <= font-descent). 
The cell height = font-descent + font-ascent, and width = AVERAGE_WIDTH.
T}
.sp 6p
_
.TE
.ps 11
.nr PS 11
.NH 4
AVERAGE_WIDTH : integer-string
.XS
\*(SN AVERAGE_WIDTH : integer-string
.XE
.LP 
AVERAGE_WIDTH is an unsigned integer-string typographic metric value 
giving the unweighted arithmetic mean width of all glyphs in the font, 
measured in 1/10th pixels.
Note, for monospaced and character cell fonts, 
this is the width of all glyphs in the font.
.NH 4
CHARSET_REGISTRY : x-registered-name
.XS
\*(SN CHARSET_REGISTRY : x-registered-name
.XE
.NH 4
CHARSET_ENCODING : registered-name
.XS
\*(SN CHARSET_ENCODING : registered-name
.XE
.LP
The character set used to encode the glyphs of the font (and implicitly 
the font's glyph repertoire), as maintained by the X Consortium character 
set registry.
CHARSET_REGISTRY is an x-registered-name that identifies 
the registration authority that owns the specified encoding.
CHARSET_ENCODING is a registered-name that identifies the coded character set 
as defined by that registration authority.
.LP
Although the X protocol does not explicitly have any knowledge about 
character set encodings, 
it is expected that server implementers will prefer to embed knowledge 
of certain proprietary or industry standard charsets into their font library 
for reasons of performance and convenience. 
The CHARSET_REGISTRY and CHARSET_ENCODING fields/properties allow 
an X client font request to specify a specific charset mapping 
in server environments where multiple charsets are supported.
The availability of any particular 
character set is font and server implementation dependent.
.LP
To prevent collisions when defining character set names, 
it is recommended that CHARSET_REGISTRY/CHARSET_ENCODING name pairs 
be constructed according to the following conventions:
.IN "CHARSET Syntax"
.ps 9
.nr PS 9
.TS
rw(1.5i) lw(3.75i).
.sp 6p
CharsetRegistry ::=	T{
StdCharsetRegistryName | PrivCharsetRegistryName
T}
CharsetEncoding ::=	T{
StdCharsetEncodingName | PrivCharsetEncodingName
T}
StdCharsetRegistryName ::=	T{
StdOrganizationId StdNumber | StdOrganizationId StdNumber Dot Year
T}
PrivCharsetRegistryName ::=	OrganizationId STRING8
StdCharsetEncodingName ::=	T{
STRING8--numeric part # of referenced standard
T}
PrivCharsetEncodingName ::=	STRING8
StdOrganizationId ::=	T{
STRING8--the registered name or acronym of the referenced standard organization
T}
StdNumber ::=	STRING8--referenced standard number
OrganizationId ::=	T{
STRING8--the registered name or acronym of the organization
T}
Dot ::=	``\.''--ISO 8859-1 FULL STOP (Column/Row 2/14)
Year ::=	STRING8--numeric year (for example, 1989)
.TE
.ps 11
.nr PS 11
.LP
The X Consortium shall maintain and publish in a timely manner a 
registry of such character set names for use in X protocol font names 
and properties as specified in XLFD.
.LP
The ISO Latin 1 character set shall be registered by the X Consortium as the 
CHARSET_REGISTRY-CHARSET_ENCODING value pair: ``ISO8859-1''.
.NH 3
Examples
.LP
The following examples of font names are derived from the screen fonts 
shipped with the R3 server.
.ps 9
.nr PS 9
.TS H
lw(1.35i) lw(4.55i).
_
.sp 6p
.B
Font	X FontName
.sp 6p
_
.sp 6p
.TH
.R
\fB75dpi Fonts\fP
.sp 3p
T{
Charter 12pt
T}	T{
-Bitstream-Charter-Medium-R-Normal--12-120-75-75-P-68-ISO8859-1
T}
T{
Charter Bold 12pt
T}	T{
-Bitstream-Charter-Bold-R-Normal--12-120-75-75-P-76-ISO8859-1
T}
T{
Charter BoldItalic 12pt
T}	T{
-Bitstream-Charter-Bold-I-Normal--12-120-75-75-P-75-ISO8859-1C
T}
T{
Charter Italic 12pt
T}	T{
-Bitstream-Charter-Medium-I-Normal--12-120-75-75-P-66-ISO8859-1
T}
Courier 8pt	-Adobe-Courier-Medium-R-Normal--8-80-75-75-M-50-ISO8859-1
Courier 10pt	-Adobe-Courier-Medium-R-Normal--10-100-75-75-M-60-ISO8859-1
Courier 12pt	-Adobe-Courier-Medium-R-Normal--12-120-75-75-M-70-ISO8859-1
Courier 14pt	-Adobe-Courier-Medium-R-Normal--14-140-75-75-M-90-ISO8859-1
Courier 18pt	-Adobe-Courier-Medium-R-Normal--18-180-75-75-M-110-ISO8859-1
Courier 24pt	-Adobe-Courier-Medium-R-Normal--24-240-75-75-M-150-ISO8859-1
T{
Courier Bold 10pt
T}	T{
-Adobe-Courier-Bold-R-Normal--10-100-75-75-M-60-ISO8859-1
T}
T{
Courier BoldOblique 10pt
T}	T{
-Adobe-Courier-Bold-O-Normal--10-100-75-75-M-60-ISO8859-1
T}
T{
Courier Oblique 10pt
T}	T{
-Adobe-Courier-Medium-O-Normal--10-100-75-75-M-60-ISO8859-1
T}
.sp 3p
\fB100dpi Fonts\fP
.sp 3p
T{
Symbol 8pt
T}	T{
-Adobe-Symbol-Medium-R-Normal--11-80-100-100-P-61-Adobe-FONTSPECIFIC
T}
T{
Symbol 10pt
T}	T{
-Adobe-Symbol-Medium-R-Normal--14-100-100-100-P-85-Adobe-FONTSPECIFIC
T}
T{
Symbol 12pt
T}	T{
-Adobe-Symbol-Medium-R-Normal--17-120-100-100-P-95-Adobe-FONTSPECIFIC
T}
T{
Symbol 14pt
T}	T{
-Adobe-Symbol-Medium-R-Normal--20-140-100-100-P-107-Adobe-\%FONTSPECIFIC
T}
T{
Symbol 18pt
T}	T{
-Adobe-Symbol-Medium-R-Normal--25-180-100-100-P-142-Adobe-\%FONTSPECIFIC
T}
T{
Symbol 24pt
T}	T{
-Adobe-Symbol-Medium-R-Normal--34-240-100-100-P-191-Adobe-\%FONTSPECIFIC
T}
T{
Times Bold 10pt
T}	T{
-Adobe-Times-Bold-R-Normal--14-100-100-100-P-76-ISO8859-1
T}
T{
Times BoldItalic 10pt
T}	T{
-Adobe-Times-Bold-I-Normal--14-100-100-100-P-77-ISO8859-1
T}
T{
Times Italic 10pt
T}	T{
-Adobe-Times-Medium-I-Normal--14-100-100-100-P-73-ISO8859-1
T}
T{
Times Roman 10pt
T}	T{
-Adobe-Times-Medium-R-Normal--14-100-100-100-P-74-ISO8859-1
T}
_
.TE
.ps 11
.nr PS 11
.NH 2
FontProperties
.XS
\*(SN FontProperties
.XE
.LP
All font properties are optional, but will generally include the 
font name fields, and on a font-by-font basis any other useful font 
descriptive/usage information that may be required to use the font 
intelligently.
The XLFD specifies an extensive set of standard X font properties, 
their interpretation and fallback rules when the property is not defined 
for a given font.
The goal is to provide client applications with enough font information 
to be able to make automatic formatting/display decisions 
with good typographic results.
.LP
Additional standard X font property definitions may be defined in the 
future and private properties may exist in X fonts at any time.
Private font properties should be defined to conform to the general mechanism 
defined in the X protocol to prevent overlap of name space and ambiguous 
property names, i.e., private font property names are of the form: 
ISO8859-1 UNDERSCORE (Column/Row 05/15), 
followed by the organizational identifier, followed by UNDERSCORE, 
and terminated with the property name.
.LP
The Backus-Naur Form syntax description of X Font Properties is:
.IN "Font Properties" "BNF Syntax"
.ps 9
.nr PS 9
.TS
rw(1.5i) lw(3.75i).
.sp 6p
Properties ::=	OptFontPropList
OptFontPropList ::=	NULL | OptFontProp OptFontPropList
OptFontProp ::=	PrivateFontProp | XFontProp
PrivateFontProp ::=	T{
STRING8 | Underscore OrganizationId Underscore STRING8
T}
XFontProp ::=	T{
FOUNDRY | FAMILY_NAME | WEIGHT_NAME | SLANT | \%SETWIDTH_NAME | ADD_STYLE_NAME 
| PIXEL_SIZE | POINT_SIZE \%| RESOLUTION_X | RESOLUTION_Y | SPACING | 
AVERAGE_WIDTH | CHARSET_REGISTRY | CHARSET_ENCODING | QUAD_WIDTH | 
RESOLUTION | MIN_SPACE | NORM_SPACE | MAX_SPACE | END_SPACE | SUPERSCRIPT_X | 
SUPERSCRIPT_Y | SUBSCRIPT_X | SUBSCRIPT_Y | UNDERLINE_POSITION | 
UNDERLINE_THICKNESS | STRIKEOUT_ASCENT | STRIKEOUT_DESCENT | ITALIC_ANGLE 
| X_HEIGHT | WEIGHT | FACE_NAME | COPYRIGHT | AVG_CAPITAL_WIDTH | 
AVG_LOWERCASE_WIDTH | RELATIVE_SETWIDTH | RELATIVE_WEIGHT | CAP_HEIGHT | 
SUPERSCRIPT_SIZE | FIGURE_WIDTH | SUBSCRIPT_SIZE | SMALL_CAP_SIZE | 
NOTICE | DESTINATION
T}
Underscore ::=	T{
OCTET-the value of ISO8859-1 UNDERSCORE character (Column/Row \%05/15)
T}
OrganizationId ::=	T{
STRING8-the registered name of the organization
T}
.TE
.ps 11
.nr PS 11
.NH 3
FOUNDRY : ATOM
.XS
\*(SN FOUNDRY : ATOM
.XE
.LP
As defined in the
.PN FontName ,
except the property type is ATOM. 
.LP
FOUNDRY can not be calculated or defaulted if not supplied as a font property.
.NH 3
FAMILY_NAME : ATOM
.XS
\*(SN FAMILY_NAME : ATOM
.XE
.LP
As defined in the 
.PN FontName ,
except the property type is ATOM. 
.LP
FAMILY_NAME can not be calculated or defaulted if not supplied as a font 
property.
.NH 3
WEIGHT_NAME : ATOM
.XS
\*(SN WEIGHT_NAME : ATOM
.XE
.LP
As defined in the 
.PN FontName ,
except the property type is ATOM. 
.LP
WEIGHT_NAME can be defaulted if not supplied as a font property, as follows:
.LP
.DS
if (WEIGHT_NAME undefined) then 
   WEIGHT_NAME = ATOM(``Medium'')
.DE
.NH 3
SLANT : ATOM
.XS
\*(SN SLANT : ATOM
.XE
.LP
As defined in the 
.PN FontName ,
except the property type is ATOM. 
.LP
SLANT can be defaulted if not supplied as a font property, as follows:
.LP
.DS
if (SLANT undefined) then 
   SLANT = ATOM(``R'')
.DE
.NH 3
SETWIDTH_NAME : ATOM
.XS
\*(SN SETWIDTH_NAME : ATOM
.XE
.LP
As defined in the 
.PN FontName ,
except the property type is ATOM.
.LP
SETWIDTH_NAME can be defaulted if not supplied as a font property, as follows:
.LP
.DS
if (SETWIDTH_NAME undefined) then
   SETWIDTH_NAME = ATOM(``Normal'')
.DE
.NH 3
ADD_STYLE_NAME : ATOM
.XS
\*(SN ADD_STYLE_NAME : ATOM
.XE
.LP
As defined in the 
.PN FontName ,
except the property type is ATOM.
.LP
ADD_STYLE_NAME can be defaulted if not supplied as a font property, as follows:
.LP
.DS
if (ADD_STYLE_NAME undefined) then
   ADD_STYLE_NAME = ATOM(``'')
.DE
.NH 3
PIXEL_SIZE : CARD32
.XS
\*(SN PIXEL_SIZE : CARD32
.XE
.LP
As defined in the 
.PN FontName ,
except the property type is CARD32.
.LP
X clients requiring pixel values for the various typographic fixed 
spaces (EM space, EN space and THIN space), can use the following 
algorithm for computing these values from other properties specified 
for a font:
.LP
.DS
DeciPointsPerInch = 722.7
EMspace = ROUND ((RESOLUTION_X * POINT_SIZE) / DeciPointsPerInch)
ENspace = ROUND (EMspace / 2)
THINspace = ROUND (EMspace / 3)\fP
.DE
.LP
Note that a ``/'' denotes real division, ``*'' denotes real multiplication,
and ``ROUND'' denotes a function that rounds its real argument ``a'' up/down 
to the next integer, according to x = FLOOR(a + 0.5), where FLOOR is a 
function that rounds its argument down to an integer.
.LP
PIXEL_SIZE can be approximated if not supplied as a font property, 
according to the algorithm:
.LP
.DS
DeciPointsPerInch = 722.7
if (PIXEL_SIZE undefined) then
   PIXEL_SIZE = ROUND ((RESOLUTION_Y * POINT_SIZE) / DeciPointsPerInch)
.DE
.NH 3
POINT_SIZE : CARD32
.XS
\*(SN POINT_SIZE : CARD32
.XE
.LP
As defined in the 
.PN FontName ,
except the property type is CARD32.
.LP
X clients requiring device-independent values for EMspace, 
ENspace and THINspace, can use the following algorithm:
.LP
.DS I
EMspace = ROUND (POINT_SIZE / 10)
ENspace = ROUND (POINT_SIZE / 20)
THINspace = ROUND (POINT_SIZE / 30)
.DE
.LP
Design POINT_SIZE can not be calculated or approximated.
.NH 3
RESOLUTION_X : CARD32
.XS
\*(SN RESOLUTION_X : CARD32
.XE
.LP
As defined in the 
.PN FontName ,
except the property type is CARD32.
.LP
RESOLUTION_X cannot be calculated or approximated.
.NH 3
RESOLUTION_Y : CARD32
.XS
\*(SN RESOLUTION_Y : CARD32
.XE
.LP
As defined in the 
.PN FontName ,
except the property type is CARD32.
.LP
RESOLUTION_Y cannot be calculated or approximated.
.NH 3
SPACING : ATOM
.XS
\*(SN SPACING : ATOM
.XE
.LP
As defined in the 
.PN FontName ,
except the property type is ATOM.
.LP
SPACING can be calculated if not supplied as a font property, 
according to the definitions given above for the 
.PN FontName .
.NH 3
AVERAGE_WIDTH : CARD32
.XS
\*(SN AVERAGE_WIDTH : CARD32
.XE
.LP
As defined in the 
.PN FontName ,
except the property type is CARD32.
.LP
AVERAGE_WIDTH can be calculated if not provided as a font property, 
according to the following algorithm:
.LP
.DS
if (AVERAGE_WIDTH undefined) then
   AVERAGE_WIDTH = ROUND (MEAN (all glyph widths in font) * 10)
.DE
.LP
where MEAN is a function that returns the arithmetic mean of its arguments.
.LP
X clients requiring values for the number of characters per inch (pitch) 
of a monospaced font can use the following algorithm using the 
AVERAGE_WIDTH and RESOLUTION_X font properties:
.LP
.DS
if (SPACING not proportional) then
   CharPitch = (RESOLUTION_X * 10) / AVERAGE_WIDTH
.DE
.NH 3
CHARSET_REGISTRY : ATOM
.XS
\*(SN CHARSET_REGISTRY : ATOM
.XE
.LP
As defined in the 
.PN FontName ,
except the property type is ATOM.
.LP
CHARSET_REGISTRY can not be defaulted if not supplied as a font property.
.NH 3
CHARSET_ENCODING : ATOM
.XS
\*(SN CHARSET_ENCODING : ATOM
.XE
.LP
As defined in the 
.PN FontName ,
except the property type is ATOM.
.LP
CHARSET_ENCODING can not be defaulted if not supplied as a font property.
.NH 3
MIN_SPACE : CARD32
.XS
\*(SN MIN_SPACE : CARD32
.XE
.LP
MIN_SPACE is an unsigned integer value 
that gives the recommended minimum wordspace value to be used with this font.
.LP
MIN_SPACE can be approximated if not provided as a font property, 
according to the algorithm:
.LP
.DS I
if (MIN_SPACE undefined) then
   MIN_SPACE = ROUND(0.75 * NORM_SPACE)
.DE
.NH 3
NORM_SPACE : CARD32
.XS
\*(SN NORM_SPACE : CARD32
.XE
.LP
NORM_SPACE is an unsigned integer value 
that gives the recommended normal wordspace value to be used with this font.
.LP
NORM_SPACE can be approximated if not provided as a font property, 
according to the following algorithm:
.LP
.DS 0
DeciPointsPerInch = 722.7
if (NORM_SPACE undefined) then
   if (SPACE glyph exists) then
      NORM_SPACE = width of SPACE
   else NORM_SPACE = ROUND((0.33 * RESOLUTION_X * POINT_SIZE) /
					DeciPointsPerInch)
.DE
.NH 3
MAX_SPACE : CARD32
.XS
\*(SN MAX_SPACE : CARD32
.XE
.LP
MAX_SPACE is an unsigned integer value 
that gives the recommended maximum wordspace value to be used with this font.
.LP
MAX_SPACE can be approximated if not provided as a font property, 
according to the following algorithm:
.LP
.DS
if (MAX_SPACE undefined) then
   MAX_SPACE = ROUND(1.5 * NORM_SPACE)
.DE
.NH 3
END_SPACE : CARD32
.XS
\*(SN END_SPACE : CARD32
.XE
.LP
END_SPACE is an unsigned integer value 
that gives the recommended spacing at the end of sentences.
.LP
END_SPACE can be approximated if not provided as a font property, 
according to the following algorithm:
.LP
.DS 
if (END_SPACE undefined) then
   END_SPACE = NORM_SPACE
.DE
.NH 3
AVG_CAPITAL_WIDTH : INT32
.XS
\*(SN AVG_CAPITAL_WIDTH : INT32
.XE
.LP
AVG_CAPITAL_WIDTH is an integer value 
that gives the unweighted arithmetic mean width of all the capital glyphs 
in the font, in 1/10th pixels (applies to Latin and non-Latin fonts).
For Latin fonts, 
capitals are the glyphs A-Z.
Normally used for font matching/substitution.
.LP
AVG_CAPITAL_WIDTH can be calculated if not provided as a font property, 
according to the following algorithm:
.LP
.DS I
if (AVG_CAPITAL_WIDTH undefined) then
   AVG_CAPITAL_WIDTH = ROUND (MEAN (capital glyph widths) * 10)
.DE
.LP 
Note that MEAN is a function that returns the arithmetic mean of its arguments.
.NH 3
AVG_LOWERCASE_WIDTH : INT32
.XS
\*(SN AVG_LOWERCASE_WIDTH : INT32
.XE
.LP
AVG_LOWERCASE_WIDTH is an integer value 
that gives the unweighted arithmetic mean width of all the lower case glyphs 
in the font in 1/10th pixels.
For Latin fonts, 
lower case are the glyphs a-z. 
Normally used for font matching or substitution. 
.LP
Where appropriate, 
AVG_LOWERCASE_WIDTH can be approximated if not provided as a font property, 
according to the following algorithm:
.LP
.DS
if (AVG_LOWERCASE_WIDTH undefined) then
   if (lower case exists) then
      AVG_LOWERCASE_WIDTH = ROUND (MEAN (lower case glyph widths) * 10)
   else AVG_LOWERCASE_WIDTH undefined
.DE
.NH 3
QUAD_WIDTH : INT32 (DEPRECATED)
.XS
\*(SN QUAD_WIDTH : INT32 (DEPRECATED)
.XE
.LP
QUAD_WIDTH was incorrectly defined in the X protocol, 
and is redundant since all typographic fixed spaces (EM, EN and THIN) 
are constant for a given font size 
(i.e., they do not vary according to setwidth).
X clients requiring these properties are encouraged to discontinue usage of 
QUAD_WIDTH and compute these values from other font properties.
X clients requiring a font-dependent width value should use 
either the FIGURE_WIDTH or one of the average character width font properties 
(AVERAGE_WIDTH, AVG_CAPITAL_WIDTH or AVG_LOWERCASE_WIDTH) for this purpose.
.LP
See also PIXEL_SIZE, FIGURE_WIDTH, AVERAGE_WIDTH, AVG_CAPITAL_WIDTH and 
AVG_LOWERCASE_WIDTH font property definitions.
.NH 3
FIGURE_WIDTH : INT32
.XS
\*(SN FIGURE_WIDTH : INT32
.XE
.LP
FIGURE_WIDTH is an integer typographic metric 
that gives the width of the tabular figures and the dollar sign,
if suitable for tabular setting (all widths equal).
For Latin fonts, these tabular figures are the arabic numerals 0-9.
.LP
FIGURE_WIDTH can be approximated if not supplied as a font property, 
according to the following algorithm:
.LP
.DS I
if (numerals and DOLLAR sign are defined & widths are equal) then
   FIGURE_WIDTH = width of DOLLAR
else FIGURE_WIDTH property undefined
.DE
.NH 3
SUPERSCRIPT_X : INT32
.XS
\*(SN SUPERSCRIPT_X : INT32
.XE
.LP
SUPERSCRIPT_X is an integer value 
that gives the recommended horizontal offset in pixels 
from the position point to the X origin of synthetic superscript text.
If the current position point is at [X,Y], 
then superscripts should begin at [X + SUPERSCRIPT_X, Y - SUPERSCRIPT_Y].
.LP
SUPERSCRIPT_X can be approximated, if not provided as a font property, 
according to the following algorithm:
.LP
.DS
if (SUPERSCRIPT_X undefined) then
   if (TANGENT(ITALIC_ANGLE) defined) then
      SUPERSCRIPT_X = ROUND((0.40 * CAP_HEIGHT) / TANGENT(ITALIC_ANGLE))
   else SUPERSCRIPT_X = ROUND(0.40 * CAP_HEIGHT)
.DE
.LP
Note that TANGENT is a trigonometric function that returns the tangent of 
its argument (in degrees scaled by 64).
.NH 3
SUPERSCRIPT_Y : INT32
.XS
\*(SN SUPERSCRIPT_Y : INT32
.XE
.LP
SUPERSCRIPT_Y is an integer value 
that gives the recommended vertical offset in pixels 
from the position point to the Y origin of synthetic superscript text.
If the current position point is at [X,Y], 
then superscripts should begin at [X + SUPERSCRIPT_X, Y - SUPERSCRIPT_Y].
.LP
SUPERSCRIPT_Y can be approximated, if not provided as a font property, 
according to the following algorithm:
.LP
.DS
if (SUPERSCRIPT_Y undefined) then
   SUPERSCRIPT_Y = ROUND(0.40 * CAP_HEIGHT)
.DE
.NH 3
SUBSCRIPT_X : INT32
.XS
\*(SN SUBSCRIPT_X : INT32
.XE
.LP
SUBSCRIPT_X is an integer value 
that gives the recommended horizontal offset in pixels 
from the position point to the X origin of synthetic subscript text.
If the current position point is at [X,Y], 
then subscripts should begin at [X + SUBSCRIPT_X, Y + SUBSCRIPT_Y].
.LP
SUBSCRIPT_X can be approximated, if not provided as a font property, 
according to the following algorithm:
.LP
.DS
if (SUBSCRIPT_X undefined) then
   if (TANGENT(ITALIC_ANGLE) defined) then
      SUBSCRIPT_X = ROUND((0.40 * CAP_HEIGHT) / TANGENT(ITALIC_ANGLE))
   else SUBSCRIPT_X = ROUND(0.40 * CAP_HEIGHT)
.DE
.NH 3
SUBSCRIPT_Y : INT32
.XS
\*(SN SUBSCRIPT_Y : INT32
.XE
.LP
SUBSCRIPT_Y is an integer value 
that gives the recommended vertical offset in pixels 
from the position point to the Y origin of synthetic subscript text.
If the current position point is at [X,Y], 
then subscripts should begin at [X + SUBSCRIPT_X, Y + SUBSCRIPT_Y].
.LP
SUBSCRIPT_Y can be approximated, if not provided as a font property, 
according to the following algorithm:
.LP
.DS
if (SUBSCRIPT_Y undefined) then
   SUBSCRIPT_Y = ROUND(0.40 * CAP_HEIGHT)
.DE
.NH 3
SUPERSCRIPT_SIZE : CARD32
.XS
\*(SN SUPERSCRIPT_SIZE : CARD32
.XE
.LP
SUPERSCRIPT_SIZE is an unsigned integer value 
that gives the recommended body size of synthetic superscripts 
to be used with this font, in pixels.
Note that this will generally be smaller than the size of the 
current font;
i.e., superscripts are imaged from a smaller font, 
offset according to SUPERSCRIPT_X and SUPERSCRIPT_Y.
.LP
SUPERSCRIPT_SIZE can be approximated if not provided as a font property, 
according to the following algorithm:
.LP
.DS
if (SUPERSCRIPT_SIZE undefined) then
   SUPERSCRIPT_SIZE = ROUND(0.60 * PIXEL_SIZE)
.DE
.NH 3
SUBSCRIPT_SIZE : CARD32
.XS
\*(SN SUBSCRIPT_SIZE : CARD32
.XE
.LP
SUBSCRIPT_SIZE is an unsigned integer value 
that gives the recommended body size of synthetic subscripts 
to be used with this font, in pixels.
As with SUPERSCRIPT_SIZE, 
this will generally be smaller than the size of the current font; 
i.e., subscripts are imaged from a smaller font, 
offset according to SUBSCRIPT_X and SUBSCRIPT_Y.
.LP
SUBSCRIPT_SIZE can be approximated if not provided as a font property, 
according to the algorithm:
.LP
.DS
if (SUBSCRIPT_SIZE undefined) then
   SUBSCRIPT_SIZE = ROUND(0.60 * PIXEL_SIZE)
.DE
.NH 3
SMALL_CAP_SIZE : CARD32
.XS
\*(SN SMALL_CAP_SIZE : CARD32
.XE
.LP
SMALL_CAP_SIZE is an integer value 
that gives the recommended body size of synthetic small capitals 
to be used with this font, in pixels.
Small capitals are generally imaged from a smaller font, 
of slightly more weight.
No offset [X,Y] is necessary.
.LP
SMALL_CAP_SIZE can be approximated if not provided as a font property, 
according to the following algorithm:
.LP
.DS
if (SMALL_CAP_SIZE undefined) then
   SMALL_CAP_SIZE = ROUND(PIXEL_SIZE * ((X_HEIGHT 
                              + ((CAP_HEIGHT - X_HEIGHT) / 3)) / CAP_HEIGHT))
.DE
.NH 3
UNDERLINE_POSITION : INT32
.XS
\*(SN UNDERLINE_POSITION : INT32
.XE
.LP
UNDERLINE_POSITION is an integer value 
that gives the recommended vertical offset in pixels
from the baseline to the top of the underline. 
If the current position point is at [X,Y], 
the top of the baseline is given by [X, Y + UNDERLINE_POSITION].
.LP
UNDERLINE_POSITION can be approximated if not provided as a font 
property, according to the following algorithm:
.LP
.DS
if (UNDERLINE_POSITION undefined) then
   UNDERLINE_POSITION = ROUND(max_bounds.descent / 2)
.DE
.NH 3
UNDERLINE_THICKNESS : CARD32
.XS
\*(SN UNDERLINE_THICKNESS : CARD32
.XE
.LP
UNDERLINE_POSITION is an unsigned integer value 
that gives the recommended underline thickness, in pixels.
.LP
UNDERLINE_THICKNESS can be approximated if not provided as a font property, 
according to the following algorithm:
.LP
.DS
CapStemWidth = average width of the stems of capitals
if (UNDERLINE_THICKNESS undefined) then
   UNDERLINE_THICKNESS = CapStemWidth
.DE
.NH 3
STRIKEOUT_ASCENT : INT32
.XS
\*(SN STRIKEOUT_ASCENT : INT32
.XE
.LP
STRIKEOUT_ASCENT is an integer value 
that gives the vertical ascent for boxing or voiding glyphs in this font.
If the current position is at [X,Y] and the string extent is EXTENT, 
the upper-left corner of the strikeout box is at [X, Y - STRIKEOUT_ASCENT] 
and the lower-right corner of the box is at [X + EXTENT, Y + STRIKEOUT_DESCENT].
.LP
STRIKEOUT_ASCENT can be approximated if not provided as a font property, 
according to the following algorithm:
.LP
.DS
if (STRIKEOUT_ASCENT undefined)
   STRIKEOUT_ASCENT =  max_bounds.ascent
.DE
.NH 3
STRIKEOUT_DESCENT : INT32
.XS
\*(SN STRIKEOUT_DESCENT : INT32
.XE
.LP
STRIKEOUT_DESCENT is an integer value 
that gives the vertical descent for boxing or voiding glyphs in this font.
If the current position is at [X,Y] and the string extent is EXTENT,
the upper-left corner of the strikeout box is at [X, Y - STRIKEOUT_ASCENT] 
and the lower-right corner of the box is at [X + EXTENT, Y + STRIKEOUT_DESCENT].
.LP
STRIKEOUT_DESCENT can be approximated if not provided as a font property, 
according to the following algorithm:
.LP
.DS
if (STRIKEOUT_DESCENT undefined)
   STRIKEOUT_DESCENT =  max_bounds.descent
.DE
.NH 3
ITALIC_ANGLE : INT32
.XS
\*(SN ITALIC_ANGLE : INT32
.XE
.LP
ITALIC_ANGLE is an integer value 
that gives the nominal posture angle of the typeface design, in 1/64 degrees, 
measured from the glyph origin counterclockwise from the three o'clock positon.
.LP
ITALIC_ANGLE can be defaulted if not provided as a font property, 
according to the following algorithm:
.LP
.DS
if (ITALIC_ANGLE undefined) then
   ITALIC_ANGLE = (90 * 64)
.DE
.NH 3
CAP_HEIGHT : CARD32
.XS
\*(SN CAP_HEIGHT : CARD32
.XE
.LP
CAP_HEIGHT is an unsigned integer, 
the nominal height of the capital letters contained in the font, 
as specified by the FOUNDRY or typeface designer.
Where applicable, 
it is defined to be the height of the Latin upper case letter X.
.LP
CAP_HEIGHT is required by certain clients to compute scale factors and 
positioning offsets for algorithmically generated glyphs where this 
information or designed glyphs are not explicitly provided by the font 
(e.g., small capitals, superiors, inferiors, etc.).
Capital height is also a critical factor in font matching and substitution.
.LP
CAP_HEIGHT can be approximated if not provided as a font property, 
according to the following algorithm:
.LP
.DS
if (CAP_HEIGHT undefined) then
   if (Latin font) then
      CAP_HEIGHT = XCharStruct.ascent[glyph X]
   else if (capitals exist) then
       CAP_HEIGHT = XCharStruct.ascent[some capital glyph]
   else CAP_HEIGHT undefined
.DE
.NH 3
X_HEIGHT : CARD32
.XS
\*(SN X_HEIGHT : CARD32
.XE
.LP
X_HEIGHT is a unsigned integer, 
the nominal height above the baseline of the lower case glyphs contained 
in the font, 
as specified by the FOUNDRY or typeface designer.
Where applicable, 
it is defined to be the height of the Latin lower case letter x.
.LP
As with Capital height, 
X_HEIGHT is required by certain clients to compute scale factors 
for algorithmically generated small capitals, 
where not explicitly provided by the font resource and is a critical factor in 
font matching and substitution.
.LP
X_HEIGHT can be approximated if not provided as a font property, 
according to the following algorithm:
.LP
.DS I
if (X_HEIGHT undefined) then
   if (Latin font) then
      X_HEIGHT = XCharStruct.ascent[glyph x]
   else if (lower case exists) then
        X_HEIGHT = XCharStruct.ascent[some lower case glyph]
   else X_HEIGHT is undefined
.DE
.NH 3
RELATIVE_SETWIDTH : CARD32
.XS
\*(SN RELATIVE_SETWIDTH : CARD32
.XE
.LP
RELATIVE_SETWIDTH is an integer 
that gives the coded proportionate width of the font,
relative to all known fonts of the same typeface family, 
according to the type designer's or FOUNDRY's judgement.
.LP
The possible values are:
.TS H
lw(.5i) lw(1i) lw(2.75i).
_
.sp 6p
.B
Code	English String	Description
.sp 6p
_
.sp 6p
.TH
.R
0	undefined	Undefined or unknown
10	UltraCondensed	Lowest ratio of average width to height
20	ExtraCondensed
30	Condensed	Condensed, Narrow, Compressed, ...
40	SemiCondensed
50	Medium	Medium, Normal, Regular, ...
60	SemiExpanded	SemiExpanded, DemiExpanded, ...
70	Expanded
80	ExtraExpanded	ExtraExpanded, Wide, ...
90	UltraExpanded	Highest ratio of average width to height
.sp 6p
_
.TE
.LP
RELATIVE_SETWIDTH can be defaulted if not provided as a font property, 
according to the following algorithm:
.LP
.DS
if (RELATIVE_SETWIDTH undefined) then
   RELATIVE_SETWIDTH = 50
.DE
.LP
X clients that wish to obtain a calculated proportionate width of the 
font (i.e., a font-independent way of identifying the proportionate 
width across all fonts and all font vendors) can use the following algorithm: 
.LP
.DS
SETWIDTH = AVG_CAPITAL_WIDTH / (CAP_HEIGHT * 10)
.DE
.LP
Note that SETWIDTH is a real with 0 being the ``narrowest'' calculated setwidth.
.NH 3
RELATIVE_WEIGHT : CARD32
.XS
\*(SN RELATIVE_WEIGHT : CARD32
.XE
.LP
RELATIVE_WEIGHT is an integer 
that gives the coded weight of the font, 
relative to all known fonts of the same typeface family, 
according to the type designer's or FOUNDRY's judgement.
.LP
The possible values are:
.TS H
lw(.5i) lw(1i) lw(3.75i).
_
.sp 6p
.B
Code	English String	Description
.sp 6p
_
.sp 6p
.TH
.R
0	undefined	Undefined or unknown
10	UltraLight	Lowest ratio of stem width to height
20	ExtraLight
30	Light
40	SemiLight	SemiLight, Book, ...
50	Medium	Medium, Normal, Regular,...
60	SemiBold	SemiBold, DemiBold, ...
70	Bold
80	ExtraBold	ExtraBold, Heavy, ...
90	UltraBold	T{
UltraBold, Black, ..., the highest ratio of stem width to height
T}
.sp 6p
_
.TE
.LP
RELATIVE_WEIGHT can be defaulted if not provided as a font property, 
according to the following algorithm:
.LP
.DS
if (RELATIVE_WEIGHT undefined) then
   RELATIVE_WEIGHT = 50
.DE
.NH 3 
WEIGHT : CARD32
.XS
\*(SN WEIGHT : CARD32
.XE
.LP
Calculated WEIGHT is an unsigned integer, 
the calculated weight of the font, 
computed as the ratio of capital stem width to CAP_HEIGHT, 
in the range 0 to 1000, where zero is the ``lightest'' weight.
.LP
WEIGHT can be calculated if not supplied as a font property, 
according to the following algorithm:
.LP
.DS
CapStemWidth = average width of the stems of capitals
if (WEIGHT undefined) then
   WEIGHT = ROUND ((CapStemWidth * 1000) / CAP_HEIGHT)
.DE
.LP
A calculated value for weight is necessary when matching fonts from 
different families because both the RELATIVE_WEIGHT and the WEIGHT_NAME are 
assigned by the typeface supplier, according to its tradition and practice, 
and therefore somewhat subjective.
Calculated WEIGHT provides a font-independent way of identifying 
the weight across all fonts and all font vendors.
.NH 3
RESOLUTION : CARD32 (DEPRECATED)
.XS
\*(SN RESOLUTION : CARD32 (DEPRECATED)
.XE
.LP
Independent horizontal and vertical design resolution components 
are required to accomodate displays with nonsquare aspect ratios 
and are given by the RESOLUTION_X and RESOLUTION_Y font name 
fields/properties. 
The units of the original definition of RESOLUTION are also in conflict with 
these new properties.
X clients are encouraged to discontinue usage of RESOLUTION 
and to use the appropriate X,Y resolution properties as required.
.NH 3
FACE_NAME : ATOM
.XS
\*(SN FACE_NAME : ATOM
.XE
.LP
FACE_NAME is a human-understandable string 
that gives the full device-independent typeface name, 
including the owner, weight, slant, set, etc., 
but not the resolution, size, etc.
Normally used as feedback during font selection.
.LP
FACE_NAME can not be calculated or approximated if not provided as a font 
property.
.NH 3
COPYRIGHT : ATOM
.XS
\*(SN COPYRIGHT : ATOM
.XE
.LP
COPYRIGHT is a human-understandable string 
that gives the copyright information of the legal owner 
of the digital font data.
.LP
This information is a required component of a font
but is independent of the particular format used to represent it 
(i.e., it cannot be captured as a comment that could later 
be ``thrown away'' for efficiency reasons).
.LP
COPYRIGHT can not be calculated or approximated if not provided as a font 
property.
.NH 3
NOTICE : ATOM
.XS
\*(SN NOTICE : ATOM
.XE
.LP
NOTICE is a human-understandable string 
that gives the copyright information of the legal owner of the font design, 
or if not applicable, the trademark information for the typeface FAMILY_NAME.
.LP
Typeface design and trademark protection laws vary from country to country, 
the USA having no design copyright protection currently, 
while various countries in Europe offer both design and typeface family name 
trademark protection.
As with COPYRIGHT, 
this information is a required component of a font 
but is independent of the particular format used to represent it.
.LP
NOTICE can not be calculated or approximated if not provided as a font property.
.NH 3
DESTINATION : CARD32
.XS
\*(SN DESTINATION : CARD32
.XE
.LP
DESTINATION is an unsigned integer code 
that gives the font design destination, 
i.e., whether it was designed as a screen proofing font to match 
printer font glyph widths (WYSIWYG), as an optimal video font (possibly with 
corresponding printer font) for extended screen viewing (VideoText), etc.
.LP
The font design considerations are very different, 
and at current display resolutions, 
the readability and legibility of these two kinds of screen fonts 
are very different.
DESTINATION allows publishing clients that use X to model the printed page, 
and Video Text clients such as on-line documentation browsers, 
to query for X screen fonts that suit their particular requirements.
.LP
The encoding is as follows:
.TS H
lw(.5i) lw(1i) lw(3.75i).
_
.sp 6p
.B
Code	English String	Description
.sp 6p
_
.sp 6p
.TH
.R
0	WYSIWYG	T{
A font optimized to match the typographic design and metrics of an 
equivalent printer font
T}
1	Video Text	T{
A font optimized for screen legibility and readability
T}
.sp 6p
_
.TE
.NH 2
Built-in Font Property Atoms
.LP
The following font property atom definitions were predefined in the initial 
Version 11 of the X protocol:
.TS H
l l.
_
.sp 6p
.B
Font Property	Property Type
.sp 6p
_
.sp 6p
.TH
.R
MIN_SPACE	CARD32
NORM_SPACE	CARD32
MAX_SPACE	CARD32
END_SPACE	CARD32
SUPERSCRIPT_X	INT32
SUPERSCRIPT_Y	INT32
SUBSCRIPT_X	INT32
SUBSCRIPT_Y	INT32
UNDERLINE_POSITION	INT32
UNDERLINE_THICKNESS	CARD32
STRIKEOUT_ASCENT	INT32
STRIKEOUT_DESCENT	INT32
FONT_ASCENT	INT32
FONT_DESCENT	INT32
ITALIC_ANGLE	INT32
X_HEIGHT	INT32
QUAD_WIDTH	INT32 \- deprecated
WEIGHT	CARD32
POINT_SIZE	CARD32
RESOLUTION	CARD32 \- deprecated
COPYRIGHT	ATOM
FACE_NAME	ATOM
FAMILY_NAME	ATOM
DEFAULT_CHAR	CARD32
.sp 6p
_
.TE
.NH 1
Affected Elements of Xlib and the X Protocol
.XS
\*(SN Affected Elements of Xlib and the X Protocol
.XE
.LP
The following X protocol requests must use the font naming conventions:
.IP \(bu 5
.PN OpenFont
\- for the name parameter
.IP \(bu 5
.PN ListFonts
\- for the pattern parameter
.IP \(bu 5
.PN ListFontsWithInfo
\- for the pattern parameter
.LP
In addition, 
the following Xlib functions must use the font naming conventions:
.IP \(bu 5
.PN XLoadFont
\- for the name parameter
.IP \(bu 5
.PN XListFontsWithInfo
\- for the pattern parameter
.IP \(bu 5
.PN XLoadQueryFont
\- for the name parameter
.IP \(bu 5
.PN XListFonts
\- for the pattern parameter
.NH 1
BDF Conformance
.XS
\*(SN BDF Conformance
.XE
.LP
The bitmap font distribution and interchange format adopted by the 
X Consortium (BDF V2.1) provides a general mechanism for identifying the 
font name of an X font and a variable list of font properties, 
but does not mandate the syntax/semantics of the font name 
or the semantics of the font properties that might be provided in a BDF font.
This section identifies the requirements for BDF fonts that conform to XLFD.
.NH 2
XLFD Conformance Requirements
.XS
\*(SN XLFD Conformance Requirements
.XE
.LP
A BDF font conforms to the XLFD V1.3 specification if and only if the 
following conditions are satisfied:
.IP \(bu 5
the value for the BDF item \fBFONT\fP conforms to the syntax 
and semantic definition of a XLFD 
.PN FontName 
string;
.IP \(bu 5
the 
.PN FontName 
begins with the X 
.PN FontNameRegistry 
prefix: ``-'';
.IP \(bu 5
all XLFD 
.PN FontName 
fields are defined;
.IP \(bu 5
any
.PN FontProperties 
provided conform in name and semantics to the XLFD 
.PN FontProperties 
definitions.
.LP             
A simple method of testing for conformance would entail first 
verifying that the 
.PN FontNameRegistry 
prefix is the string ``-'', 
that the number of field delimiters in the string and coded field values 
are valid, 
and that each font property name either matches a standard XLFD property name 
or follows the definition of a private property.
.NH 2
FONT_ASCENT, FONT_DESCENT and DEFAULT_CHAR
.XS
\*(SN FONT_ASCENT, FONT_DESCENT and DEFAULT_CHAR
.XE
.LP
FONT_ASCENT, FONT_DESCENT and DEFAULT_CHAR are provided in the BDF 
specification as properties that are moved to the 
.PN XFontStruct 
by the BDF font compiler in generating the X server-specific 
binary font encoding. 
If present, 
these properties shall comply with the following semantic definitions.
.NH 3
FONT_ASCENT : INT32
.XS
\*(SN FONT_ASCENT : INT32
.XE
.LP
FONT_ASCENT is an unsigned integer 
that gives the recommended typographic ascent above the baseline, 
for determining interline spacing. 
Specific glyphs of the font may extent beyond this.
If the current position point for line \fIn\fP is at [X,Y], 
then the origin of the next line \fIn+1\fP 
(allowing for a possible font change) is [X, Y + FONT_DESCENT 
\d\s-1\fIn\s0\u\fP + FONT_ASCENT \d\s-1\fIn+1\s0\u\fP].
.LP
FONT_ASCENT can be approximated if not provided as a font property, 
according to the following algorithm:
.LP
.DS
if (FONT_ASCENT undefined) then
   FONT_ASCENT = max_bounds.ascent
.DE
.NH 3
FONT_DESCENT : INT32
.XS
\*(SN FONT_DESCENT : INT32
.XE
.LP
FONT_DESCENT is an unsigned integer 
that gives the recommended typographic descent below the baseline, 
for determining interline spacing. 
Specific glyphs of the font may extent beyond this.
If the current position point for line \fIn\fP is at [X,Y],
then the origin of the next line \fIn+1\fP 
(allowing for a possible font change) is [X, Y + FONT_DESCENT 
\d\s-1\fIn\s0\u\fP + FONT_ASCENT \d\s-1\fIn+1\s0\u\fP].
.LP
The logical extent of the font is inclusive between the Y-coordinate values: 
Y - FONT_ASCENT and Y + FONT_DESCENT + 1.
.LP
FONT_DESCENT can be approximated if not provided as a font property, 
according to the following algorithm:
.LP
.DS
if (FONT_DESCENT undefined) then
   FONT_DESCENT = max_bounds.descent
.DE
.NH 3
DEFAULT_CHAR : CARD32
.XS
\*(SN DEFAULT_CHAR : CARD32
.XE
.LP
DEFAULT_CHAR is an unsigned integer value 
that gives the default character to be used by the X server 
when an attempt is made to display an undefined or non-existent character 
in the font.
.LP
The DEFAULT_CHAR is a 16-bit character (not a two byte character).
For a font using two byte matrix format,
the DEFAULT_CHAR has byte1 in the most significant byte and byte2 
in the least significant byte.
If the DEFAULT_CHAR itself is undefined 
or specifies an undefined or non-existent character in the font, 
then no display is performed.
.LP
DEFAULT_CHAR can not be approximated if not provided as a font property.
.TC

