.TH MKFONTDIR 1 "Release 4" "X Version 11"
.SH NAME
mkfontdir - create fonts.dir file from directory of font files.
.SH SYNOPSIS
.B "mkfontdir"
[directory-names]
.SH DESCRIPTION
.PP
.B Mkfontdir
For each directory argument, mkfontdir reads all of the font files in the
directory searching for properties named "FONT", or (failing that) the name
of the file stripped of its suffix.  These are used as font names, which are
written out to the file "fonts.dir" in the directory along with the name of
the font file.
.PP
The kinds of font files read by mkfontdir depends on configuration
parameters, but typically include SNF (suffix ".snf"), compressed SNF
(suffix ".snf.Z"), BDF (suffix ".bdf"), and compressed BDF (suffix ".bdf.Z").
If a font exists in multiple formats, the most efficient format will be used.
.SH "FONT NAME ALIASES"
.PP
The file "fonts.alias" which can be put in any directory of the font-path is
used to map new names to existing fonts, and should be edited by hand.  The
format is straight forward enough, two white-space separated columns, the
first containing aliases and the second containing font-name patterns.
.PP
When a font alias is used, the name it references is search for in the normal
manner, looking through each font directory in turn.  This means that the
aliases need not mention fonts in the same directory as the alias file.
.PP
To embed white-space in either name, simply enclose them in double-quote
marks, to embed double-quote marks (or any other character), precede them
with back-slash:
.PP
.nf
"magic-alias with spaces"	"\\"font\\name\\" with quotes"
regular-alias			fixed
.fi
.PP
If the string "FILE_NAMES_ALIASES" stands alone on a line, each file-name
in the directory (stripped of it's .snf suffix) will be used as an alias for
that font.
.SH "USAGE"
.PP
\fBXserver(1)\fP looks for both "fonts.dir" and "fonts.alias" in each
directory in the font path each time it is set (see xset(1)).
.SH "SEE ALSO"
X(1), Xserver(1), xset(1)
