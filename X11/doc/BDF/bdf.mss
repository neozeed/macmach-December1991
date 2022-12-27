@Make(Article)
@style(Font "TimesRoman", Size 12)
@Comment[
    @libraryfile(specialcharacters)
    this library is an adobe hack that doesn't work anywhere
    else. Replace with vile raw postscript, BKR, 3 Feb 87
]
@DefineFont(Symbolfontx,A=<TypeCase pssymbol1>)
@Define(ZZ,Font Symbolfontx,FaceCode A)
@String(CopyrightSerif="@ZZ(w)")
@String(RegisterSerif="@ZZ(v)")

@string(formatversion="2.1")
@Begin(TitlePage)
@Begin(TitleBox)
@MajorHeading(Bitmap Distribution Format @value(formatversion))
@End(TitleBox)
Copyright @value(CopyrightSerif) 1984, 1987, 1988 Adobe Systems, Inc.
@blankspace(1 line)
Permission to use, copy, modify, and distribute this
software and its documentation for any purpose and without
fee is hereby granted, provided that the above copyright
notice appear in all copies and that both that copyright
notice and this permission notice appear in supporting
documentation.
@pageHeading(left "@i[Bitmap Distribution Format]",
right "@value(page)")
@pagefooting(left "X11",
right "BDF Version @value(formatversion)")
@end(titlepage)
@Section(Introduction)
This document describes Bitmap Distribution Format
(BDF), version @value(formatversion).  BDF is an X Consortium
standard for font interchange,
intended to be easily understood by both humans and computers.

@Section(File Format)
Character bitmap information will be distributed in an @c[USASCII] encoded,
human readable form.
Each file is encoded in the printable characters (octal 40 through 176) of
 @c[USASCII] plus carriage return and linefeed. Each file consists of a
sequence of variable-length lines. Each line is terminated by a
carriage-return (octal 015) and line-feed (octal 012), or by just
a line-feed.

The information about a particular family and face at
one size and orientation will be contained in one file. The file begins
with information pertaining to the face as a whole, followed by the
information and bitmaps for the individual characters.

A font bitmap description file has the following general form, where each
item is contained on a separate line of text in the file. Items on a line
are separated by spaces. 

@begin(enumerate)
The word @c[STARTFONT] followed by a version number indicating the exact
file format used. The version described here is number @b[@value(formatversion)].

One or more lines beginning with the word @c[COMMENT]. These lines may be
ignored by any program reading the file.

The word @c[FONT] followed by either the XLFD font name (as specified in the
X Logical Font Description Conventions standard) or some private font name.
Creators of private font name syntaxes are encouraged to register unique font
name prefixes with the X Consortium to prevent naming conflicts.
Note that the name continues all the way to the end of the line,
and may contain spaces.

The word @c[SIZE] followed by the @b[point size]
of the characters, the @b[x resolution], and the @b[y resolution] 
of the device for which these characters were intended.

The word @c[FONTBOUNDINGBOX] followed by the @b[width in x], @b[height in
y], and the x and y displacement of the lower left corner from the @b[origin].
(See the examples in section @ref[figsect]).

Optionally the word @c[STARTPROPERTIES] followed by the number of properties
(@b[p]) that follow.

Then come @b[p] lines consisting of a word for the @b[property name] followed
by either an integer or string surrounded by double-quote (octal 042).
Internal double-quotes characters are indicated by using two in a row.

Properties named @c[FONT_ASCENT],  @c[FONT_DESCENT], and @c[DEFAULT_CHAR]
should be provided to define the logical font-ascent and font-descent
and the default-char for the font.  These properties will be removed from
the actual font properties in the binary form produced by a compiler.
If these properties are not provided, a compiler may reject the font,
or may compute (arbitrary) values for these properties.

The property section, if it exists, is terminated by ENDPROPERTIES.

The word @c[CHARS] followed by the number of character segments (@b[c]) that
follow.

Then come @b[c] character segments of the form:
@begin(enumerate)
The word @c[STARTCHAR] followed by up to 14 characters (no blanks) of
descriptive @b[name] of the glyph.

@begin(multiple)
The word @c[ENCODING] followed by one of the following forms:

@begin(enumerate)
<n> - the glyph index, i.e. a positive integer representing the character code
used to access the glyph in X requests, as defined by the encoded character
set given by the CHARSET_REGISTRY-CHARSET_ENCODING font properties for XLFD
conforming fonts.  If these XLFD font properties are not defined, then the
encoding scheme is font-dependent;

-1 <n> - equivalent to form above.  This syntax is provided for backwards
compatibility with previous versions of this specification, and is not
recommended for use with new fonts;

-1 - an unencoded glyph. Some font compilers may discard unencoded glyphs,
but in general the glyph names may be used by font compilers and X servers to
implement dynamic mapping of glyph repertoires to character encodings as seen
through the X protocol.
@end(enumerate)
@end(multiple)

The word @c[SWIDTH] followed by the @b[scalable width] in x and y of
character. Scalable widths are in units of 1/1000th of the size of the
character. If the size of the character is @i[p] points, the width
information must be scaled by @i[p]/1000 to get the width of the character
in printer's points. This width information should be considered as a vector
indication the position of the next character's origin relative to the
origin of this character.  To convert the scalable width to the width
in device pixels, multiply @c[SWIDTH] times @i[p]/1000 times @i[r]/72 where
@i[r] is the device resolution in pixels per inch.  The result is a real
number giving the ideal print width in device pixels.  The actual device width
must of course be an integral number of device pixels and is given in the
next entry.
The @c[SWIDTH] y value should always be zero for a standard X font.

The word @c[DWIDTH] followed by the width in x and y of the
character in device units.  Like the @c[SWIDTH], this width information is a
vector indicating the position of the next character's origin relative
to the origin of this character.
Note, that the @c[DWIDTH] of a given "hand-tuned" WYSIWYG glyph may deviate
slightly from its ideal device-independent width given by @c[SWIDTH] in order
to improve its typographic characteristics on a display.
The @c[DWIDTH] y value should always be zero for a standard X font.

The word @c[BBX] followed by the @b[width] in x (@i[BBw]), @b[height] in y
(@i[BBh]) and x and y displacement (@i[BBox, BBoy]) of the lower left corner
from the @b[origin] of the character.

The optional word @c[ATTRIBUTES] followed by the attributes as 4
@b[hex-encoded] characters. The interpretation of these attributes is
undefined in this document.

The word @c[BITMAP].

@i[h] lines of @b[hex-encoded bitmap], padded on the right with zero's to the 
nearest byte (i.e., multiple of 8).

The word @c[ENDCHAR].
@end(enumerate)

The file is terminated with the word @c[ENDFONT].
@end(enumerate)
@section(Metric Information)
@label(figsect)
Figures @ref(figj) and @ref(figq) best illustrate the bitmap format and
character metric information.
@begin[figure]
@blankspace(3.4in)
@Picture(size=3 in, ScaleableLaser="j.ps")
@center{BBw = 9, BBh = 22, BBox = -2, BBoy = -6
@c[DWIDTH] = 8 0
@c[SWIDTH] = 355 0
``+'' = character origin and width
}
@caption(An example of a descender)
@tag(figj)
@end[figure]
@begin[figure]
@blankspace(3.4in)
@Picture(size=3 in, ScaleableLaser="q.ps")
@center(BBh = 6, BBw = 4, BBox = +2, BBoy = +12
@c[DWIDTH] = 5 0
@c[SWIDTH] = 223 0)
@caption(An example with the origin outside the bounding box)
@tag(figq)
@end[figure]

@newpage
@section(An Example File)
The following is an abbreviated example of a bitmap file
containing the specification of two characters (the j and quoteright in
@ref[figsect]).@foot(Helvetica@value[registerserif] is a registered trademark of Allied Corporation.)

@begin(fileexample, Size 9, Free)
STARTFONT @value(formatversion)
COMMENT This is a sample font in 2.1 format.
FONT -Adobe-Helvetica-Bold-R-Normal--24-240-75-75-P-65-ISO8859-1
SIZE 24 75 75
FONTBOUNDINGBOX 9 24 -2 -6
STARTPROPERTIES 19
FOUNDRY "Adobe"
FAMILY "Helvetica"
WEIGHT_NAME "Bold"
SLANT "R"
SETWIDTH_NAME "Normal"
ADD_STYLE_NAME ""
PIXEL_SIZE 24
POINT_SIZE 240
RESOLUTION_X 75
RESOLUTION_Y 75
SPACING "P"
AVERAGE_WIDTH 65
CHARSET_REGISTRY "ISO8859"
CHARSET_ENCODING "1"
MIN_SPACE 4
FONT_ASCENT 21
FONT_DESCENT 7
COPYRIGHT "Copyright (c) 1987 Adobe Systems, Inc."
NOTICE "Helvetica is a registered trademark of Linotype Inc.
ENDPROPERTIES
CHARS 2
STARTCHAR j
ENCODING 106
SWIDTH 355 0
DWIDTH 8 0
BBX 9 22 -2 -6
BITMAP
0380
0380
0380
0380
0000
0700
0700
0700
0700
0E00
0E00
0E00
0E00
0E00
1C00
1C00
1C00
1C00
2C00
7800
F000
E000
ENDCHAR
STARTCHAR quoteright
ENCODING 39
SWIDTH 223 0
DWIDTH 5 0
BBX 4 5 2 12
ATTRIBUTES 01C0
BITMAP
70
70
60
E0
C0
ENDCHAR
ENDFONT
@end(fileexample)
