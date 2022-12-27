.mc |
.\"  (c) Copyright 1989, 1990 OPEN SOFTWARE FOUNDATION, INC.
.mc
.\"  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
.\"  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
.\"  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
.\"  (c) Copyright 1988 MICROSOFT CORPORATION
.\"  ALL RIGHTS RESERVED
.mc |
.nr H1 17
.mc
.H 1 "UIL Language Syntax"
.P 0
.mc |
This chapter and the next chapter provide reference information on UIL.
.mc
This chapter describes syntax rules for low-level elements of the
language.
.mc |
Chapter 19 describes syntax rules for building a UIL module using these
.mc
low-level elements.
.P 0
This chapter describes the syntax rules for the following: 
.BL
.LI
Character set
.LI
Names
.LI
Keywords
.LI
Literals
.LI
Value-generating functions
.LI
.mc |
The \fBany\fR data type
.mc
.LI
.mc *
.mc
Compile-time value expressions
.LE
.P 0
.mc |
.ne 4
.mc
UIL is a free-form language.  This means that high-level constructs
such as object and value declarations do not need to begin in any
particular column and can span any number of lines.  Low-level constructs
.mc |
such as keywords and punctuation characters
.mc
can also begin in any column; however, except for string literals and comments,
they cannot span lines.
.P 0
The UIL compiler accepts input lines up to 132 characters in length.  
.H 2 "Character Set"
.P 0
.mc |
Use the character set described in the following table to construct elements of UIL.
.mc
.iX "UIL" "character set"
.iX "Character set"
.P 0
.TB "UIL Legal Character Set"
.TS 
box, tab(@);
lb | lb
l | l.
Element@Characters
.mc
_
.mc *
.mc
Letters:@T{
ABCDEFGHIJKLMNOPQRSTUVWXYZ 
.mc |
.br
.mc
abcdefghijklmnopqrstuvwxyz
T}
.mc |
_
.mc
Digits:@T{
0123456789
T}
.mc |
_
.mc
T{
.mc |
Formatting and punctuation 
.br
characters:
.mc
T}@T{
Space, tab, form-feed, _ ' ( ) * , + - . / ; : = ! $ \\ { }
T}
.mc |
_
.mc
T{
Other characters:
T}@T{
Remaining 8-bit character codes in the range 160 to 255 decimal,
& [ ] | < > % " # ? @ ~ ` ^ 
.mc *
.mc
T}
.mc *
.mc
.TE                                                                  
.mc |
.P
.mc
Alphabetic, numeric, and punctuation and formatting characters 
are used to build the elements of UIL.  Other characters are 
valid only in comments and string literals.
.P 0
Control characters, except for the form-feed character, are not permitted 
in a UIL module.  You must
use escape
.mc |
sequences (described in Section 18.4.1)
.mc
to construct a string literal containing a control character. 
.H 3 "Punctuation Characters"
.P 0
.mc |
You use the character sequences shown in the following table to punctuate a UIL 
.mc
module.  These characters cannot be used in UIL names.
.iX "UIL" "punctuation characters"
.iX "UIL module" "punctuation characters"
.iX "UIL specification file" "punctuation characters"
.iX "Punctuation characters"
.mc |
.P
.mc
.TB "Punctuation Characters"
.TS 
center, tab(@);
l l l l.
_
.sp 6p
\fB(\fP@Left parenthesis@\fB)\fP@Right parenthesis
\fB{\fP@Left brace@\fB}\fP@Right brace
\fB\\\\\fP@Backslash@\fB!\fP@Exclamation mark
\fB/*\fP@Slash asterisk@\fB*/\fP@Asterisk slash
\fB;\fP@Semicolon@\fB:\fP@Colon
\fB'\fP@Apostrophe@\fB,\fP@Comma
\fB=\fP@Equal sign
.sp 6p
_
.TE                              
.P 0
Punctuation in a UIL module resembles that used in C programs.
For example, statements end in a semicolon, braces are used
.mc |
to delimit definitions, and comments can be delimited by the /* (slash
and asterisk) and */ (asterisk and slash) character sequences.
.mc
.P 0
Spaces, tabs, and comments are special elements in the language.  They are
a means of delimiting other elements, such as two names.  One or more of
these elements can appear before or after any other element in the language.
However, spaces, tabs, and comments that appear in string literals are
treated as character sequences rather than delimiters.
.P 0
Comments can take one of two forms, as follows:  
.BL
.LI
The comment is introduced with the sequence 
.I /*
followed by the text of the comment and terminated with the sequence 
.I */.
This form of comment can span multiple source lines.
.LI
.mc |
The comment is introduced with an ! (exclamation point),
.mc
followed by
the text of the comment and terminated by the end of the source line.
.LE
.mc |
.P
.mc
Neither form of comment can be nested.
.mc |
.br
.ne 7
.mc
.P 0
The form-feed 
character is a control character and therefore cannot appear directly in a UIL
specification file. 
You must use the escape sequence \\12\\ instead (see
.mc |
Section 18.4.1 for information on specifying other escape sequences in
.mc
string literals). 
There is one exception to this rule: a
form-feed can appear in column 1 of a source line due to the common practice of
some editors separating parts of a program with a form-feed.  The form-feed 
causes a page break in the module listing. 
.H 2 "Names"
.iX "UIL" "names"
.iX "Names"
.iX "UIL module" "names"
.iX "UIL specification file" "names"
.P 0
Each entity in the UIL language, such as a value, procedure, or object, can be
identified by a name.  This name can also be used to reference the entity 
elsewhere in the UIL module.
.P 0
.mc |
Names can consist of any of the characters A to Z, a to z,
0 to 9, $ (dollar sign), and \^_\^ (underscore).  Names cannot begin
with a digit (0 to 9).  The maximum length of a name is 31 characters.
.mc
.P 0
UIL gives you a choice of
either case-sensitive or case-insensitive names through a clause 
.mc |
in the MODULE header (described in Section 19.1.2).  
For example, if names are case sensitive, the names "sample" and
.mc
"Sample" are distinct from each other.  If names are case insensitive,
.mc |
these names are treated as the same name and can be used
interchangeably.  By default, UIL assumes names are case sensitive. 
.mc
.P 0
In case-insensitive mode, the compiler outputs all names in the 
UID file in uppercase form.
In case-sensitive mode, 
names appear in the UIL file 
exactly as they appear in the source.  
.P 0
You must define any referenced name exactly once in a UIL module;
if you define the same name more than once or omit a definition,
the UIL compiler issues an error at compile time. 
.mc |
.br
.ne 7
.mc
.H 2 "Keywords"
.P 0
.iX "UIL" "keywords"
.iX "UIL module" "keywords"
.iX "UIL specification file" "keywords"
.iX "Keywords"
Keywords are names that have special meaning in UIL.  They are of two types,
reserved and nonreserved.  
You cannot use a reserved keyword 
to name an entity.  Nonreserved keywords can be used as names. 
.P 0
.mc |
If you specify case-insensitive mode, you can type UIL keywords in 
.mc
uppercase, lowercase, or mixed case.
If you specify case-sensitive mode, you must type UIL keywords in lowercase.
.P 0
You
cannot
abbreviate keywords by truncating characters from the end.
.P 0
.mc |
The following table lists the UIL reserved keywords.
.P
.mc
.TB "Reserved Keywords"
.TS H
tab(@);
lBw(2.5i) lBw(2.5i)
lw(2.5i) lw(2.5i).
_
.sp 6p
Keyword@Description
.sp 6p 
_
.sp 6p
.mc *
.mc
.TH
ARGUMENTS@T{
.mc |
Identifies an arguments list
.mc
T}
.br
CALLBACKS@T{
.mc |
Identifies a callbacks list
.mc
T}
.br
CONTROLS@T{
.mc |
Identifies a controls list
.mc
T}
.br
END@T{
.mc |
Signifies the end of the module
.mc
T}
.br
EXPORTED@T{
Specifies that this object or value can be referenced by other
.mc |
UIL modules
.mc
T}
.br
FALSE@T{
.mc |
Represents the Boolean value zero; synonym for Off
.mc
T}
.br
GADGET@T{
For objects having a widget and a gadget variant, specifies this
.mc |
object as the gadget variant
.mc
T}
.br
IDENTIFIER@T{
.mc |
Indicates an identifier declaration
.mc
T}
.br
INCLUDE@T{
.mc |
Used with the nonreserved keyword FILE to specify an include file
.mc
T}
.br
LIST@T{
.mc |
Identifies a list declaration
.mc
T}
.br
MODULE@T{
.mc |
Signifies the start of a UIL module
.mc
T}
.br
OFF@T{
.mc |
Represents the Boolean value zero; synonym for FALSE
.mc
T}
.br
ON@T{
.mc |
Represents the Boolean value 1; synonym for TRUE
.mc
T}
.br
OBJECT@T{
.mc |
Identifies an object declaration
.mc
T}
.br
PRIVATE@T{
Specifies that this object or value cannot be referenced by other
.mc |
UIL modules
.mc
T}
.br
PROCEDURE@T{
.mc |
Identifies a procedure declaration
.mc
T}
.br
PROCEDURES@T{
.mc |
Identifies a procedures list
.mc
T}
.br
TRUE@T{
.mc |
Represents the Boolean value 1; synonym for On
.mc
T}
.br
VALUE@T{
.mc |
Identifies a value (literal) declaration
.mc
T}
.br
WIDGET@T{
For objects having a widget and a gadget variant, specifies this object
.mc |
as the widget variant
.mc
T}
.sp 6p
_
.TE
.P 0
.mc |
Keywords listed in Table 18-4
.mc
are nonreserved keywords,
which you can use as names without generating an error.
However, if you use any keyword as a name, you cannot use the UIL-supplied
usage of that keyword.
For example, if you use the name of an argument (such as 
.mc |
\fBXmNx\fR)
.mc
as the name of a value, you cannot specify the 
.mc |
\fBXmNx\fR
.mc
argument in any 
object definitions.
.mc |
.P
.ne 3i
.in0
.mc
.TB "Nonreserved Keywords"
.TS H
tab(@);
lBw(2i) lBw(2.25i)
lw(2i) lw(2.25i).
.mc
_
.sp 6p
Keyword@Description
.sp 6p 
_
.sp 6p
.mc *
.mc
.TH
T{
Built-in argument names (for example: XmNx, XmNheight)
T}@T{
.mc |
Identifies an object argument (widget-specific attribute)
.mc
T}
.br
T{
.mc |
Built-in reason names (for example: XmNactivateCallback, XmNhelpCallback)
.mc
T}@T{
.mc |
Identifies a callback reason
.mc
T}
.br
T{
Character set names (for example: ISO_LATIN1, ISO_HEBREW_LR)
T}@T{
.mc |
Identifies a character set and its (implicit) writing direction
.mc
T}
.br
T{
.mc |
Constant value names (for example: XmMENU_OPTION, XmBROWSE_SELECT)
T}@T{
Identifies a constant value defined in the Motif toolkit
T}
.br
T{
.mc
Object types (for example: XmPushButton, XmBulletinBoard)
T}@T{
.mc |
Identifies a Motif Toolkit interface object
.mc
T}
.br
T{
ANY
T}@T{
.mc |
Suppresses data type checking
.mc
T}
.br
T{
ARGUMENT
T}@T{
.mc |
Identifies the built-in ARGUMENT function
.mc
T}
.br
T{
ASCIZ_STRING_TABLE
T}@T{
.mc |
Specifies a value as the UIL data type \f3asciz_table\fP
.mc
T}
.br
T{
ASCIZ_TABLE
T}@T{
.mc |
Specifies a value as the UIL data type \f3asciz_table\fP
.mc
T}
.br
T{
BACKGROUND
T}@T{
.mc |
In a color table, specifies monochrome mapping to the background color
.mc
T}
.br
BOOLEAN@T{
.mc |
Specifies a literal as the UIL data type \f3boolean\fP
.mc
T}
.br
T{
CASE_INSENSITIVE
T}@T{
Used with the nonreserved keyword NAMES to specify that names and keywords
.mc |
in the module are case insensitive
.mc
T}
.br
CASE_SENSITIVE@T{
Used with the nonreserved keyword NAMES to specify that names and keywords in
.mc |
the module are case sensitive
.mc
T}
.br
CHARACTER_SET@T{
.mc |
Identifies the default character-set clause; identifies the built-in
CHARACTER_SET function
.mc
T}
.br
COLOR@T{
.mc |
Specifies a value as the UIL data type \f3color\fP
.mc
T}
.br
COLOR_TABLE@T{
.mc |
Specifies a value as the UIL data type \f3color_table\fP
.mc
T}
.br
T{
COMPOUND_STRING
T}@T{
.mc |
Specifies a value as the UIL data type \f3compound_string\fP;
identifies the built-in COMPOUND_STRING data conversion function
.mc
T}
.br
T{
COMPOUND_STRING_TABLE
T}@T{
.mc |
Specifies a value as the UIL data type \f3string_table\fP
.mc
T}
.br
FILE@T{
.mc |
Used with the reserved keyword INCLUDE to specify an include file
.mc
T}
.br
FLOAT@T{
.mc |
Specifies a literal as the UIL data type \f3float\fP; identifies the
FLOAT data conversion function
.mc
T}
.br
FONT@T{
.mc |
Specifies a value as the UIL data type \f3font\fP
.mc
T}
.br
FONT_TABLE@T{
.mc |
Specifies a value as the UIL data type \f3font_table\fP
.mc
T}
.br
FOREGROUND@T{
.mc |
In a color table, specifies monochrome mapping to the foreground color
.mc
T}
.br
ICON@T{
.mc |
Specifies a value as the UIL data type \f3pixmap\fP; identifies the 
built-in ICON function
.mc
T}
.br
IMPORTED@T{
Specifies that this literal takes its value from a corresponding
.mc |
literal in another UIL module
.mc
T}
.br
INTEGER@T{
.mc |
Specifies a literal as the UIL data type \f3integer\fP; identifies
the INTEGER data conversion function
.mc
T}
.br
INTEGER_TABLE@T{
.mc |
Specifies a value as the UIL data type \f3integer_table\fP
.mc
T}
.br
.mc |
KEYSYM@T{
Specifies a literal as the UIL data type \fBkeysym\fP
T}
.br
.mc
MANAGED@T{
.mc |
Specifies that a child is managed by its parent at run time
.mc
T}
.br
NAMES@T{
.mc |
Identifies the case sensitivity clause
.mc
T}
.br
OBJECTS@T{
.mc |
Identifies the default object-variant clause
.mc
T}
.br
REASON@T{
.mc |
Identifies the built-in REASON function
.mc
T}
.br
.mc |
RGB@T{
Specifies a color using RGB values
T}
.br
.mc
T{
RIGHT_TO_LEFT
T}@T{
.mc |
Specifies the writing direction in the COMPOUND_STRING function
.mc
T}
.br
.mc |
SINGLE_FLOAT@T{
Specifies a literal as the UIL data type \fBsingle_float\fP
T}
.mc
.br
.mc |
STRING@Specifies a literal as the UIL data type \f3string\fP
.br
.mc
STRING_TABLE@T{
.mc |
Specifies a value as the UIL data type \f3string_table\fP
.mc
T}
.br
T{
TRANSLATION_TABLE
T}@T{
.mc |
Specifies a value as the UIL data type \f3translation_table\fP
.mc
T}
.br
UNMANAGED@T{
.mc |
Specifies that a child is unmanaged by its parent at run time
.mc
T}
.br
.mc |
USER_DEFINED@T{
Specifies that the object is a user-defined type
T}
.br
.mc
VERSION@T{
.mc |
Identifies the version clause
.mc
T}
.br
XBITMAPFILE@T{
.mc |
Specifies a value as the UIL data type \f3pixmap\fP; identifies the
XBITMAPFILE function
.mc
T}
.sp 6p 
_
.TE
.mc |
.in
.mc
.P 0
In this chapter, all examples assume case-insensitive mode.  
.mc |
Keywords are shown in uppercase to distinguish them from 
user-specified names, which are shown in lowercase;  
however, this use of uppercase is not required 
in case-insensitive mode.  In the following example, 
ARGUMENTS is a keyword and \fIcircle-radius\fP
is a user-specified value. In your UIL module, you could type the keyword 
ARGUMENTS in lowercase, uppercase, or mixed-case as long as you specified 
.mc
that names are case insensitive.
.mc |
If you specify that names are case sensitive, you must enter the keyword
in lowercase.
.P
.nf
.ft CW
\&.
\&.
\&.
.mc
{ ARGUMENTS
    { circle_radius = 1000 };
.mc |
\&.
\&.
\&.
.mc
};
.mc |
.fi
.ft R
.br
.ne 2.25i
.mc
.H 2 "Literals "
.iX "UIL" "literals"
.iX "UIL module" "literals"
.iX "UIL specification file" "literals"
.iX "Values" "literals"
.P 0
Literals are one means of specifying a value.  UIL provides literals for
several of the value types it supports.  Some of the value types are not
supported as literals (for example, pixmaps and string tables).  You can
specify values for these types 
.mc |
by using functions provided by UIL (discussed in Section 18.5).
.mc
Literal types directly supported by UIL are as follows:
.BL
.LI
String 
.LI
Integer
.LI
Boolean
.LI
Floating-point
.LE
.P 0
You can designate UIL values as exported, imported, or private.
An exported object or value can be referenced in another UIL module that uses 
the same name for the object or value and indicates that the object or 
value is to be
imported.  By default, top-level objects are exported, and all other
objects and values are private and are not accessible
by other UIL modules.  
Section
.mc |
19.2
.mc
explains the scope of UIL objects and values in
more detail.
.H 3 "String Literals"
.iX "UIL" "string literals"
.iX "UIL module" "string literals"
.iX "UIL specification file" "string literals"
.iX "Values" "string literals"
.P 0
A string literal is a sequence of zero or more 8-bit or 16-bit
characters or a combination 
.mc |
delimited by ' (single quotation marks) or " (double quotation marks).
String literals can be no more than 2000 characters long.
.mc
.P 0
A single-quoted string literal can span multiple source lines.  To continue
a single-quoted string literal, terminate the continued line with a 
.mc |
\\ (backslash).  The literal continues with the first character
.mc
on the next line.
.P 0
Double-quoted string literals cannot span multiple source lines.
(Because double-quoted strings can contain escape sequences and other special
characters, you cannot use the backslash character to designate
continuation of the string.)  To build a string value that must
span multiple source lines, use the concatenation operator.  See 
.mc |
Section 18.4.2 for a description of how to concatenate strings.
.mc
.P 0
.mc |
.ne 4
.mc
The syntax of a string literal is one of the following:
.mc |
.P
.nf
\&'[ \fIchar\fP... ]'

[ #\fIchar-set\fP ]"[ \fIchar\fP... ]"
.fi
.P
.mc
Both string forms associate a character set with a string value.
UIL uses the following rules to determine the character set and
storage format for string literals:
.BL
.LI
.mc |
A string declared as '\fIstring\fP' is equivalent to
#\fIcur_charset\fR"\fIstring\fR", where \fIcur_charset\fR will be the codeset
portion of the value of the LANG environment if it is set or the value
of \fBXmFALLBACK_CHARSET\fP if \fBLANG\fR is not set or has no codeset
component.
By default \fBXmFALLBACK_CHARSET\fP is ISO8859-1 (equivalent to
ISO_LATIN1), but vendors may define a different default.
See Section 20.3 for more information.
.mc
.LI
.mc |
A string declared as "\fIstring\fP" is equivalent to #\fIchar-set\fR"\fIstring\fP" if
you specified \fIchar-set\fR as the default character set for the module.
Otherwise, "\fIstring\fP" is equivalent to #\fIcur_charset\fR"\fIstring\fP", where
\fIcur_charset\fR is interpreted as described above.
.mc
.LI
.mc |
A string of the form "\fIstring\fP" or #\fIchar-set\fR"\fIstring\fP" is stored as a
.mc
null-terminated string.
.LE
.P 0
.mc |
Table 18-5 gives examples of valid and invalid string
.mc
literal syntax.  Note that the COMPOUND_STRING function (described
.mc |
in Section 18.5.9) forces the UIL compiler to generate a compound
.mc
string.
.mc |
.P
.mc
.TB "Examples of String Literal Syntax"
.TS H
tab(@);
lBw(2i) lBw(2.5i)
lw(2i) lw(2.5i).
.mc
_
.sp 6p
Form@Storage Format
.sp 6p
_
.sp 6p
.mc *
.mc
.TH
.mc |
'\fIstring\fP'@T{
.mc
Null-terminated string. 
.mc |
Character set is \fIcur_charset\fR (see Section 20.3).
.mc
T}
.mc |
.sp .7
#\fIchar-set\fR'\fIstring\fP'@T{
.mc
Invalid syntax. 
.mc |
Does not compile.
.mc
T}
.mc |
.sp .7
COMPOUND_STRING('\fIstring\fP')@T{
.mc
Compound string. 
.mc |
Character set is \fIcur_charset\fR (see Section 20.3).
.mc
T}
.mc |
.sp .7
"\fIstring\fP"@T{
.mc
Null-terminated string. 
If specified, the string has the default 
character set for the module. 
Otherwise, the character set is
.mc |
\fIcur_charset\fR (see Section 20.3).
.mc
T}
.mc |
.sp .5
.ne 2i
#ISO_GREEK"\fIstring\fP"@T{
.mc
Null-terminated string.
Character set is ISO_GREEK.
T}
.mc |
.sp .5
COMPOUND_STRING("\fIstring\fP")@T{
.mc
Compound string.
If specified, the string has the default
character set for the module.
Otherwise, the character set is
.mc |
\fIcur_charset\fR (see Section 20.3).
.mc
T}
.mc |
.sp .5
.mc
T{
.mc |
COMPOUND_STRING
.br
(#ISO_ARABIC"\fIstring\fP")
.mc
T}@T{
Compound string.
Character set is ISO_ARABIC.
T}
.mc |
.sp .5
'\fIstring\fP'&"\fIstring\fP"@T{
.mc
If the character sets and writing directions of the operands match, 
the resulting string is null-terminated; otherwise, the result
is a multiple-segment compound string.
String has the character set or sets specified for the individual segments.
T}
.mc |
.sp .5
"\fIstring\fP"&#ISO_HEBREW"\fIstring\fP"@T{
.mc
If the implicit character set and writing direction for the left
operand matches the explicit character set (ISO_HEBREW) and
writing direction (right to left) for the right operand, the resulting
string is a null-terminated string; otherwise, the result is a
multiple-segment compound string.
T}
.mc |
.sp 3p
.mc
_
.TE
.mc *
.mc
String literals can contain characters with the eighth (high-order)
bit set.  You cannot type control characters (00..1F, 7F, and 80..9F)
directly in a single-quoted string literal.  However, you can represent these 
characters with escape sequences.
The characters listed in Table 6 cannot be directly entered
in a UIL module. You must use the indicated escape sequence
to enter these characters in a string literal. 
.mc |
.br
.ne 2i
.P
.mc
.iX "UIL" "escape sequences"
.iX "Escape sequences"
.TB "Escape Sequences"
.mc *
.mc
.TS H
center, tab(@);
lBw(1.5i)0 lBw(2.5i)
lBw(1.5i)0 lBw(2.5i)
lw(1.5i)0 lw(2.5i).
.mc
_
.sp 6p
Escape@Meaning
Sequence@
.sp 6p
_
.sp 6p
.mc *
.mc
.TH
.mc |
\eb@Backspace
\ef@Form-feed
\en@Newline\(dg
\er@Carriage return
\et@Horizontal tab
\ev@Vertical tab
\e'@Single quotation mark
\e"@Double quotation mark
\e\e@Backslash
\e\f2integer\e\fP@T{
.mc
Character whose internal representation is given by
.mc |
\f2integer\fP (in the range 0 to 255 decimal)
.mc
T}
.mc *
.mc
.sp 6p
_
.mc |
.sp 6p
.T&
lp-1 s .
T{
.ll 4i
\(dg The UIL compiler does not process newline characters in compound
strings.
The effect of a newline character in a compound string depends only on
the character set of the string, and the result is not guaranteed to be
a multiline string.
T}
.sp 6p
_
.mc
.TE
.P 0
.mc |
.br
.ne 10
.mc
.H 3 "Concatenated String Literals"
.P 0
.iX "Concatentation operator"
.mc |
The & (ampersand) concatenation operator takes two strings as operands and 
.mc
creates a new string made up of the left operand 
followed immediately by the right 
operand.
.P 0
For example: 
.mc |
.P
.nf
.ft CW
.mc
\'abcd' & 'xyz' 
.mc |
.fi
.ft R
.P
becomes the following:
.P
.nf
.ft CW
.mc
\'abcdxyz'
.mc |
.fi
.ft R
.mc
.P 0
The operands of the concatenation operator can be
.mc |
null-terminated strings, compound strings, or a combination of both.
.mc
The operands can hold string values of
the same or different character sets.
.P 0
The string resulting from the concatenation is a null-terminated string
unless one or more of the following conditions exists:
.BL
.LI
One of the operands is a compound string
.LI
The operands have different character set properties
.LI
The operands have different writing directions
.LE
.mc |
.P
.mc
Then the resulting string is a compound string.   
You cannot use imported or exported values as operands of the concatenation 
operator.
.mc |
(See Section 19.2 for information on declaring values as private,
.mc
exported, or imported.)
.H 4 "Compound String Literals"
.P 0
A compound string consists of a string of 8-bit or 16-bit characters,
a named character set, and a writing direction.  Its UIL data type is 
\fBcompound_string\fP.
.P 0
The writing direction of a compound string is implied by the
character set specified for the string.  You can explicitly set the
writing direction for a compound string by using the COMPOUND_STRING
.mc |
function (discussed in Section 18.5.9).  
Section 18.4.2.2 describes the character
.mc
sets supported in UIL for compound strings.
.P 0
A compound string can consist of a sequence of concatenated compound strings,
.mc |
null-terminated strings, or a combination of both,
.mc
each of which can have a different character set property and writing
direction.  Use the
.mc |
concatenation operator & (ampersand) to create a 
.mc
sequence of compound strings.  The following is an example of
concatenated compound strings:
.mc |
.P
.nf
.ft CW
.mc
#ISO_HEBREW"txet werbeh"&#ISO_LATIN8"latin text"
.mc |
.fi
.ft R
.P
Each string in the sequence is stored, including
.mc
the character set and writing direction information.
You can manipulate a compound string with the Motif Toolkit
routines for compound strings.
.P 0
Generally, a string literal is stored in the UID file as a compound
string when the literal consists of concatenated strings having
different character sets or writing directions, or when you 
use the string to specify a value for an argument that requires
a compound string value.  If you want to
guarantee that a string literal is stored as a compound string,
.mc |
you must use the COMPOUND_STRING function (discussed in Section 18.5.9).
.P
Because the results of the newline character depend on the character set
of a compound string, there is no guarantee that an embedded '\en' will
generate a multiline string.
To ensure that you create a multiline string, use the SEPARATE clause as
follows:
.P
.nf
.ft CW
VALUE
  sample_string: COMPOUND_STRING( "Hello", SEPARATE = TRUE )
                                & "world!";
.fi
.ft R
.br
.ne 3i
.mc
.H 4 "Character Sets for String Literals"
.P 0
.mc |
Table 18-7 lists the character sets supported by the UIL compiler for
.mc
string literals.
.P 0
The first column shows the UIL name for the character set. 
The second column gives a brief description of the character
set.  Note that several UIL names map to the same character set.  In
some cases, the UIL name influences how string literals are read.  For example,
strings identified by a UIL character set name ending in _LR are read left-to-right.
Names that end in a different number reflect different fonts 
(for example, ISO_LATIN1 or ISO_LATIN6).
All character sets in this table are represented by 8 bits.
.mc |
.P
.ne 10
.mc
.TB "Supported Character Sets"
.P 0
.TS H
tab(@);
lBw(1.4i) lBw(3i)
lw(1.4i) lw(3i).
.mc
_
.sp 6p
UIL Name@Description
.sp 6p
_
.sp 6p
.mc *
.mc
.TH
ISO_LATIN1@GL: ASCII, GR: Latin-1 Supplement
.mc |
ISO_LATIN2@GL: ASCII, GR: Latin-2 Supplement
ISO_ARABIC@GL: ASCII, GR: Latin-Arabic Supplement
ISO_LATIN6@GL: ASCII, GR: Latin-Arabic Supplement
ISO_GREEK@GL: ASCII, GR: Latin-Greek Supplement
ISO_LATIN7@GL: ASCII, GR: Latin-Greek Supplement
ISO_HEBREW@GL: ASCII, GR: Latin-Hebrew Supplement
ISO_LATIN8@GL: ASCII, GR: Latin-Hebrew Supplement
ISO_HEBREW_LR@GL: ASCII, GR: Latin-Hebrew Supplement
ISO_LATIN8_LR@GL: ASCII, GR: Latin-Hebrew Supplement
JIS_KATAKANA@GL: JIS Roman, GR: JIS Katakana
.mc
.sp 6p
_
.TE
.P 0
.mc |
.ne 3i
.mc
The parsing rules for each
.mc |
of the character sets is described in Table 18-8.
.P
.nf
.ft CW
.S -2
.mc
#ISO_HEBREW "tfel ot thgir morf og sretcarahc"
.mc |
.fi
.ft R
.S
.P
.mc
In this example, the characters in the string value are presented
right to left.  Since the character set for the literal is 
ISO_HEBREW, the characters in quotation marks can be any legal
character as defined by the ISO_HEBREW
character set.  
.P 0
.mc |
.ne 10
.mc
.TB "Parsing Rules for Character Sets"
.P 0
.mc *
.mc
.TS H
tab(@);
lBw(1.5i) lBw(3.5i)
lw(1.5i) lw(3.5i).
.mc
_
.sp 6p
Character Set@Parsing Rules
.sp 6p
_
.sp 6p
.mc *
.mc
.TH
All character sets@T{
.mc |
Character codes in the range 00...1f, 7f, and 80...9f are control characters 
.mc
including both bytes of 16-bit characters.  
.mc |
The compiler flags these as illegal characters.
.mc
T}
.sp 3p
T{
.br
ISO_LATIN1
.br
ISO_LATIN2
.br
ISO_ARABIC
.br
ISO_LATIN3
.br
ISO_GREEK
.br
ISO_LATIN4
.br
T}@T{
These sets are parsed from left to right.  
The escape sequences for null-terminated strings
are also supported by these character sets.  See
.mc |
Table 18-6 for more information on escape sequences.
.mc
T}
.sp 3p
T{
.br
ISO_HEBREW
.br
ISO_LATIN8
T}@T{
These sets are parsed from right to left; for example, the string #ISO_HEBREW
"012345" generates a primitive string "543210" with character set ISO_HEBREW.  
A DDIS descriptor for such a string has this segment marked as 
being right_to_left.
The escape sequences for null-terminated
.mc |
strings in Table 18-6 are also supported by these 
character sets, and the characters that compose the escape sequences are 
in left-to-right order.  For example, you type \en, not n\e.
.mc
T}
.sp 3p
.mc |
.ne 2i
.mc
T{
ISO_HEBREW_LR
ISO_LATIN8_LR
T}@T{
These sets are parsed from left to right; for example, 
the string #ISO_HEBREW_LR "012345" generates a primitive string "012345" 
with character set ISO_HEBREW.  
A DDIS descriptor for such a string marks this segment as being left_to_right.
The escape sequences for null-terminated strings,
.mc |
given in Table 18-6, are also supported by these character sets.
.mc
T}
.sp 3p
.mc |
.ne 10
.mc
JIS_KATAKANA@T{
This set is parsed from left to right.  The escape sequences
.mc |
for null-terminated strings in Table 18-6 are also supported by this character
set.  Note that the \e (backslash) may be displayed as a yen symbol.
.mc
T}
.sp 6p
_
.TE
.P 0
In addition to designating parsing rules for strings, character set
information remains an attribute of a compound string.  
If the string is included in a string consisting of
several concatenated segments, the character set information
is included with that string segment.
This gives the Motif Toolkit
the information it needs to decipher the 
compound string and choose a font to display the string.
.P 0
For an application interface displayed only in English, 
UIL lets you ignore the distinctions between the two uses of strings.  
The compiler recognizes by context when a 
string must be passed as a null-terminated string or as a compound string.
.P 0
The UIL compiler recognizes enough about the various character sets to
correctly parse string literals.  
.mc *
.mc
The compiler also issues errors if you use a compound
string in a context that supports only null-terminated strings.
.P 0
.mc |
.ne 3i
.mc
Since the character set names are keywords, you must
put them in lowercase if case-sensitive names are in force.
If names are case insensitive, character set names can be 
.mc |
uppercase, lowercase, or mixed case.
.mc
.P 0
In addition to the built-in character sets recognized by UIL, you can define
your own character sets with the CHARACTER_SET function.  You can use the
CHARACTER_SET function anywhere a character set can be specified.
.mc |
See Section 18.5.1. for more information on the CHARACTER_SET
.mc
function.
.P 0
.H 4 "Data Storage Consumption for String Literals"
.P 0
The way a string literal is stored in the UID file depends on 
how you declare and use the string.  The UIL compiler 
automatically converts a null-terminated string to 
a compound string if you use the string to specify the 
value of an argument that requires a compound string.
However, this conversion is costly in terms of storage
consumption.
.P 0
Private, exported, and imported string literals require
storage for a single allocation when the literal is declared;
thereafter, storage is required for each reference to the 
.mc |
literal.  Literals declared inline require storage
.mc
for both an allocation and a reference.
.P 0
The following table summarizes data storage consumption for
string literals.  The storage requirement for an 
allocation consists of a fixed portion and a variable 
.mc |
portion.  The fixed portion of an allocation is roughly
.mc
the same as the storage requirement for a reference
(a few bytes).  The storage consumed by the variable portion
depends on the size of the literal value (that is, the 
length of the string).  To conserve storage space, avoid
making string literal declarations that result in an 
allocation per use.
.mc |
.br
.ne 3i
.in0
.TB "Data Storage for String Literals"
.P 0
.mc
.TS H
tab(@);
lBw(.75i) lBw(1i) lBw(1i) lBw(2.5i)
lw(.75i) lw(1i) lw(1i) lw(2.5i).
.mc
_
.sp 6p
Declaration@Data Type@Used As@Storage Requirements Per Use
.sp 6p
_
.sp 6p
.mc *
.mc
.TH
.mc |
Inline@Null-terminated@Null-terminated@T{
.mc
An allocation and a reference (within the module)
T}
Private@Null-terminated@Null-terminated@T{
A reference (within the module)
T}
Exported@Null-terminated@Null-terminated@T{
A reference (within the UID hierarchy)
T}
Imported@Null-terminated@Null-terminated@T{
A reference (within the UID hierarchy)
T}
.mc |
Inline@Null-terminated@Compound@T{
.mc
An allocation and a reference (within the module)
T}
Private@Null-terminated@Compound@T{
An allocation and a reference (within the module)
T}
Exported@Null-terminated@Compound@T{
A reference (within the UID hierarchy)
T}
Imported@Null-terminated@Compound@T{
.mc |
A reference (within the UID hierarchy)
.mc
T}
.mc |
Inline@Compound@Compound@T{
.mc
An allocation and a reference (within the module)
T}
Private@Compound@Compound@T{
A reference (within the module)
T}
Exported@Compound@Compound@T{
A reference (within the UID hierarchy)
T}
Imported@Compound@Compound@T{
A reference (within the UID hierarchy)
T}
.sp 6p
_
.TE
.mc |
.in
.mc
.H 3 "Integer Literals"
.iX "UIL" "integer literals"
.iX "UIL module" "integer literals"
.iX "UIL specification file" "integer literals"
.iX "Values" "integer literals"
.P 0
An integer literal represents the value of a whole number.
Integer literals have the form of 
an optional sign followed by one or more decimal digits.
An integer literal must not contain embedded spaces or commas.
.P 0
.mc |
Integer literals are stored in the UID file as long integers.
.mc
Exported and imported integer literals require a single allocation
when the literal is declared; thereafter, a few bytes of
storage are required for each reference to the literal.  Private
.mc |
integer literals and those declared inline require allocation
.mc
and reference storage per use.  To conserve storage
space, avoid making integer literal declarations that result
in an allocation per use.
.P 0
.mc |
.ne 2i
.mc
The following table shows data storage consumption for integer
literals.
.mc |
.P
.TB "Data Storage for Integer Literals"
.P 0
.mc
.TS H
tab(@);
lB lBw(3i)
l lw(3i).
_
.sp 6p
Declaration@Storage Requirements Per Use
.sp 6p
_
.sp 6p
.mc *
.mc
.TH
.mc |
Inline@An allocation and a reference (within the module)
.mc
Private@An allocation and a reference (within the module)
Exported@A reference (within the UID hierarchy)
Imported@A reference (within the UID hierarchy)
.sp 6p
_
.TE
.mc *
.mc
.H 3 "Boolean Literals"
.iX "UIL" "Boolean literals"
.iX "UIL module" "Boolean literals"
.iX "UIL specification file" "Boolean literals"
.iX "Values" "Boolean literals"
.iX "Boolean literals"
.P 0
.mc |
A Boolean literal represents the value True (reserved keyword TRUE
or On) or False reserved keyword FALSE or Off).  These keywords are subject to
case-sensitivity rules.
.mc
.P 0
In a UID file, TRUE is represented by the integer value 1 and
FALSE is represented by the integer value 0.
.P 0
Data storage consumption for Boolean literals is the same
as that for integer literals.
.mc *
.mc
.H 3 "Floating-Point Literals"
.iX "UIL" "floating-point literals"
.iX "UIL module" "floating-point literals"
.iX "UIL specification file" "floating-point literals"
.iX "Values" "floating-point literals"
.P 0
A floating-point literal represents the value of a real (or float) number.
Floating-point literals have one of the following forms:
.mc |
.P
.nf
[ \fB+\fR | \fB-\fR ] \fIdigit\fP... . [ \fIdigit\fP... ] [ { \fBE\fR | \fBe\fR } [ \fB+\fR | \fB-\fR ] \fIdigit\fP...
[ \fB+\fR | \fB-\fR ] . \fIdigit\fP... [ { \fBE\fR | \fBe\fR } [ \fB+\fR | \fB-\fR ] \fIdigit\fP... ]
.fi
.P
For maximum portability a floating-point literal can represent values in
the range 1.0E-37 to 1.0E+37
with at least 6 significant digits.
On many machines this range will be wider, with more significant digits.
.mc
A floating-point literal must not contain embedded spaces or commas.
.P 0
Floating-point literals are stored in the UID file as 
.mc |
double-precision, floating-point numbers.
.mc
The following table
gives examples of valid and invalid floating-point 
notation for the UIL compiler.
.mc |
.P
.ne 2i
.TB "Floating-Point Notation"
.P 0
.mc
.TS H
tab(@);
lB lB
l l.
_
.sp 6p
Valid Floating-Point Literals@Invalid Floating-Point Literals
.sp 6p
_
.sp 6p
.mc *
.mc
.TH
 1.0@1e1 (no decimal point)
 .1@E-1 (no decimal point or digits)
 3.1415E-2 (which equals .031415)@2.87 e6 (embedded blanks)
 -6.29e7 (which equals -62900000)@2.0e100 (out of range)
.sp 6p
_
.TE
.P 0
Data storage consumption for floating-point literals is
the same as that for integer literals.
.P 0
.H 2 "Value-Generating Functions"
.P 0
.mc |
.iX "UIL Functions"
.mc
.iX "UIL module" "functions"
.iX "UIL specification file" "functions"
.iX "Functions"
UIL provides functions to generate the following types of values:
.BL
.LI
Character sets
.LI
.mc |
Keysyms
.LI
.mc
Colors
.LI
Pixmaps
.LI
.mc |
Single-precision, floating-point numbers
.LI
Double-precision, floating-point numbers
.LI
.mc
Fonts
.LI
Font tables
.LI
Compound strings
.LI
Compound string tables
.LI
.mc |
ASCIZ (null-terminated) string tables
.mc
.LI
Integer tables
.LI
Arguments
.LI
Reasons
.LI
Translation tables
.LE
.P 0
Remember that all examples in the following sections assume case-insensitive 
mode.  Keywords are shown in uppercase letters to distinguish them
from user-specified names, which are shown in lowercase letters.
This use of uppercase letters is not required in case-insensitive
mode.  In case-sensitive mode, keywords 
.mc |
must be in lowercase letters.
.br
.ne 3i
.H 3 "The CHARACTER_SET Function"
.mc
.P 0
.mc *
.mc
.iX "UIL" "user-defined character set"
.iX "UIL module" "user-defined character set"
.iX "UIL specification file" "user-defined character set"
.mc |
.iX "UIL Functions" "CHARACTER_SET"
.mc
.iX "User-defined character set"
.iX "Character set" "user-defined"
.iX "CHARACTER_SET function"
.mc |
You can define your own character sets with the \fBCHARACTER_SET\fR function.  You
can use the \fBCHARACTER_SET\fR function anywhere a character set can be specified. 
The \fBCHARACTER_SET\fR function has the following syntax:
.P
.nf
\fBCHARACTER_SET\fR (\fIstring-expression\fP [,\fIproperty\fP]...);
.fi
.mc
.P 0
.mc |
The result of the \fBCHARACTER_SET\fR function is a character set with the name
.mc
\fIstring-expression\fP and the properties you specify. 
\fIString-expression\fP must be a
null-terminated string.  You can optionally include one or both of the following
clauses to specify properties for the resulting character set:
.mc |
.P
.nf
\fBRIGHT_TO_LEFT\fR = \fIboolean-expression\fP
\fBSIXTEEN_BIT\fR = \fIboolean-expression\fP
.fi
.mc
.P 0
.mc |
The \fBRIGHT_TO_LEFT\fR clause sets the default writing direction of the string from
right to left if \fIboolean-expression\fP is True, and right to left otherwise.
.mc
.P 0
.mc |
The \fBSIXTEEN_BIT\fR clause allows the strings associated with this character set to
be interpreted as 16-bit characters if \fIboolean-expression\fP is True, 
.mc
and 8-bit characters otherwise.
.mc |
.H 3 "The KEYSYM Function"
.mc
.P 0
.mc |
.iX "UIL" "keysyms"
.iX "UIL module" "keysyms"
.iX "UIL specification file" "keysyms"
.iX "UIL Functions" "KEYSYM"
.iX "Keysyms" "defining in UIL"
The \fBKEYSYM\fR function is used to specify a keysym for a mnemonic resource.
It has the following syntax:
.P
.nf
\fBKEYSYM\fR (\fIstring-literal\fP)
.fi
.P
The \fIstring-literal\fP must contain exactly one character.
.P
The following example shows how to use the \fBKEYSYM\fR function:
.P 0
.nf
.ft CW
OBJECT push_button_1:
    XmPushButton
      { ARGUMENTS
             { XmNmnemonic = KEYSYM( "Q" );
               XmNlabelString = COMPOUND_STRING( "Quit" ); };
      };
.ft R
.fi
.H 3 "Functions for Specifying Colors"
.P 0
Color values are designed to let you designate a value to specify a
color and then use that value for arguments requiring a color value.
.H 4 "The COLOR Function"
.P 0
.mc
.iX "UIL" "color values"
.iX "UIL module" "color values"
.iX "UIL specification file" "color values"
.mc |
.iX "UIL Functions" "COLOR"
.mc
.iX "Color values" "defining in UIL"
.mc |
The \fBCOLOR\fR function supports the definition of colors.  
Using the \fBCOLOR\fR function, you can designate a value to specify a color
.mc
and then use that value for arguments requiring a color value.
.P 0
.mc |
The \fBCOLOR\fR function has the following syntax:
.P
.nf
\fBCOLOR\fR (\fIstring-expression\fP [,\fBFOREGROUND\fR\^|,\fBBACKGROUND\fR])
.fi
.P
.mc
The string expression 
names the color you want to define;
.mc |
the optional keywords \fBFOREGROUND\fR and \fBBACKGROUND\fR identify how the color is 
.mc
to be displayed on a monochrome device when the color is used in the
definition of a color table.
.P 0
.mc |
.ne 2i
The following example shows how to use the \fBCOLOR\fR function:
.mc
.P 0
.mc |
.nf
.ft CW(wide
.mc
VALUE red: COLOR( 'Red' );
VALUE green: COLOR( 'Green' );
VALUE blue: COLOR( 'Blue' );
OBJECT primary_window: 
    XmMainWindow
      { ARGUMENTS
             { XmNforeground = green;
               XmNbackground = COLOR( 'Black' ); };
      };
.mc |
.fi
.ft R
.mc
.P 0
.mc |
In this example, the \fBCOLOR\fR function is used with the \fBVALUE\fR declaration
(described in Section 19.3)
.mc
to define three colors and give them each a name. One of these colors, 
.mc |
green is then used to specify the foreground color of the main window. 
.mc
.P 0
.mc |
A second use of the \fBCOLOR\fR function defines the background color for the main
.mc
window as the color associated with the string 'Black'.
.P 0
The UIL compiler does not 
have built-in color names.  Colors are a server-dependent attribute of
an
.mc |
object.  Colors are defined on each server and may have different red-green-blue (RGB)
.mc
values on each server.  The string you specify as the
color argument must be recognized by the server on which
your application runs.
.P 0
In a UID file, UIL represents a color as a character string.
MRM calls X translation routines
that convert a color string to the device-specific pixel value.  If you
are running on a monochrome server, all colors translate to black or
white.
If you are on a color server, the color names translate to their proper
colors if the following conditions are met:
.BL
.LI
The color is defined.
.LI
The color map is not yet full.
.LE
.mc |
.P
.mc
If the color map is full, even valid colors translate to black or white
(foreground or background).
.P 0
Interfaces do not, in general, specify colors for widgets,
so that the selection of colors can be controlled by the
.mc |
user through the \fB.Xdefaults\fR
file.
.mc
.P 0
.mc |
.ne 3i
.mc
To write an application that runs on both monochrome and color devices, you
.mc |
need to specify which colors in a color table (defined with the \fBCOLOR_TABLE\fR
.mc
function) 
map to the background and which colors map to the foreground. 
.mc |
UIL lets you use the \fBCOLOR\fR function to designate this mapping in the definition of the color.
.mc
The following example
.mc |
shows how to use the \fBCOLOR\fR function to map the color red to the background color on a monochrome device:
.P
.nf
.ft CW 0
.mc
VALUE c: COLOR ( 'red',BACKGROUND );
.mc |
.fi
.ft R
.mc
.P 0
The mapping comes into play only when the MRM is
given a color and the 
application is to be displayed on a monochrome device.  In this case,
each color is considered to be in one of the following three categories: 
.BL
.LI
The color is mapped to the background color on the monochrome device.
.LI
The color is mapped to the foreground color on the monochrome device.
.LI
Monochrome mapping is undefined for this color.
.LE
.P 0
If the color is mapped to the
foreground or background color, MRM substitutes the
foreground or background color, respectively.  If you do not specify the
monochrome mapping for a color, MRM passes the color string
to the Motif Toolkit 
for mapping to the foreground or background color. 
.mc |
.H 4 "The RGB Function"
.P 0
.iX "UIL" "color values"
.iX "UIL module" "color values"
.iX "UIL specification file" "color values"
.iX "UIL Functions" "RGB"
.iX "Color values" "defining in UIL"
.P 0
The \fBRGB\fR function has the following syntax:
.P
.nf
\fBRGB\fR (\fIinteger\fP, \fIinteger\fP, \fIinteger\fP);
.fi
.P
The three integers define the values for the red, green, and blue
components of the color, in that order.
The values of these components can range from 0 to 65,535, inclusive.
.P
The following example shows how to use the \fBRGB\fR function:
.P
.nf
.ft CW
VALUE green : RGB( 0, 65535, 0 );
OBJECT primary_window:
    XmMainWindow
      { ARGUMENTS
             { XmNforeground = green;
               XmNbackground = RGB(65025, 12996, 7396); };
      };
.fi
.ft R
.P
In a UID file, UIL represents an \fBRGB\fR value as three integers.
MRM calls X translation routines that
convert the integers to the device-specific pixel value.
If you are running on a monochrome server, all colors translate to
black or white.
If you are on a color server, \fBRGB\fR values translate to their proper
colors if the colormap is not yet full.
If the colormap is full, values translate to black or white (foreground
or background).
.mc
.H 3 "Functions for Specifying Pixmaps"
.iX "UIL" "pixmap value"
.iX "UIL module" "pixmap value"
.iX "UIL specification file" "pixmap value"
.mc |
.iX "UIL Functions" "COLOR_TABLE"
.iX "UIL Functions" "ICON"
.mc
.iX "Pixmaps" "defining in UIL"
.P 0
Pixmap values are designed to let you specify labels that are graphic images
rather than text.  
Pixmap values are not directly supported by UIL.  Instead, UIL supports
icons,
which are a simplified form of pixmap.  You use a character to describe
each pixel in the icon.
.P 0
Pixmap support in the UIL compiler is provided by the following
functions:
.mc |
\fBCOLOR_TABLE\fR, \fBICON\fR, and \fBXBITMAPFILE\fR.  
.mc
.P 0
In a UIL module, any argument of type 
\fBpixmap\fP 
should have an icon or an 
.mc |
\fBxbitmap\fR file specified as its value.
.br
.ne 4i
.H 4 "The COLOR_TABLE Function"
.mc
.P 0
.iX "COLOR_TABLE Function"
.mc |
The \fBCOLOR_TABLE\fR function has the following syntax:
.mc
.iX "COLOR_TABLE Function"
.mc |
.P
.nf
\fBCOLOR_TABLE\fR ( { \fIcolor-expression\fP = \fIcharacter\fP },... )
.fi
.mc
.P 0
The color expression
is a previously defined color, a color defined in line
.mc |
with the \fBCOLOR\fR function, or the phrase
\fBBACKGROUND COLOR\fR or \fBFOREGROUND COLOR\fR.  The character 
can be any valid UIL character (see Table 18-1).
.mc
.P 0
The following example shows how to specify a color table:
.P 0
.mc |
.nf
.S -2
.ft CW
.mc
VALUE 
  rgb : COLOR_TABLE ( red = 'r', green = 'g', blue = 'b' );
  bitmap_colors : COLOR_TABLE ( BACKGROUND COLOR = '0', 
                                FOREGROUND COLOR = '1' );
.mc |
.S
.fi
.ft R
.mc
.P 0
.mc |
The \fBCOLOR_TABLE\fR function provides a device-independent way to
.mc
specify a set of colors.  
.mc |
The \fBCOLOR_TABLE\fR function accepts either 
previously defined UIL color names or in line
color definitions (using the \fBCOLOR\fR function).
.mc
A color table must be private because its
contents must be known by the UIL compiler to construct an icon.  The
colors within a color table, however, can be imported, exported, or
.mc |
private.  See Section 19.2 for more information on scope of reference to
values.
.mc
.P 0
The single letter associated with each color is the character you 
use to represent that color when creating an icon.
Each letter used to represent a color must be unique within the
color table.
.mc |
.H 4 "The ICON Function"
.mc
.P 0
.mc |
The \fBICON\fR function has the following syntax:
.P
.nf
\fBICON\fR ( [ COLOR_TABLE=\fIcolor-table-name\fP , ] \fIrow\fP,... )
.fi
.mc
.P 0
.mc |
The color table name must refer to a previously defined color table
and the row is a character expression giving one row of the icon.
.mc
.P 0
.mc |
.ne 3i
.mc
The following example
.mc |
shows how to define a pixmap using the \fBCOLOR_TABLE\fR and \fBICON\fR functions:
.mc
.P 0
.mc |
.nf
.ft CW
.S -2
.in0
.mc
VALUE 
   rgb    : COLOR_TABLE ( red = '=', green = '.', blue = ' ' );
   x_icon : ICON( COLOR_TABLE=rgb, '=========',
                                   '==.   .==',
                                   '== . . ==',
                                   '==  .  ==',
                                   '== . . ==',
                                   '==.   .==',
                                   '=========' );
.mc |
.in
.fi
.S
.ft R
.mc
.P 0
.mc |
The \fBICON\fR function describes a rectangular icon that is x pixels wide
.mc
and y pixels high.  
The strings surrounded by single quotation marks describe 
the icon.  
Each string represents a row in the icon;
each character in the string represents a pixel.  
.P 0
The first row in an icon definition determines the width of the icon.
All rows
must have the same number of characters as the first row. 
The height of the icon is dictated by the number of rows.
.mc |
For example, the x_icon defined in the previous example is 9 pixels wide and 7
.mc
pixels high.
.P 0
.mc |
The first argument of the \fBICON\fR function 
.mc
(the color table specification) is optional and identifies the colors that are
available in this icon.  
By using the single letter associated with
each color, you can specify the color of each pixel in the icon.  
.mc |
In the example, an = (equal sign) represents the color red, a . 
(dot) is green, and a space is blue.
.mc
The icon must be constructed of characters
defined
in the specified color table.
.mc |
In the example, the color table named rgb specifies colors
for the = (equal sign), . (dot), and space.  The x_icon is constructed
.mc
with these three characters.
.P 0
.mc |
A default color table is used if you omit the argument specifying the color
.mc
table.  To make use of the default color table, the rows of your icon
must contain only spaces and asterisks.
.mc *
.mc
The default color table is defined as follows:
.mc |
.P
.nf
.ft CW
.S -2
.in 0
.mc
COLOR_TABLE( BACKGROUND COLOR = ' ', FOREGROUND COLOR = '*' ) 
.mc |
.S
.in
.fi
.ft R
.mc
.P 0
You can define other characters to represent the background color and
.mc |
foreground color by replacing the space and asterisk in the \fBBACKGROUND COLOR\fR
and \fBFOREGROUND COLOR\fR clauses shown in the previous statement.
.mc
You can specify icons as private, imported, or exported.
Use the MRM function 
.mc |
\fBMrmFetchIconLiteral\fR
.mc
to retrieve an exported icon at run
time.
.mc |
.br
.ne 3i
.H 4 "The XBITMAPFILE Function"
.mc
.P 0
.mc |
The \fBXBITMAPFILE\fR function is similar to the \fBICON\fR function in that both
.mc
describe a rectangular icon that is x pixels wide and y pixels high.
.mc |
However, \fBXBITMAPFILE\fR allows you to specify an external file containing the
definition of an X bitmap, whereas all \fBICON\fR function definitions must be
coded directly within UIL.  X bitmap files can be generated by 
.mc
many different X applications.
.mc |
UIL reads these files through the \fBXBITMAPFILE\fR function, but does
.mc
not support creation of these files.
The X bitmap file specified as the argument
.mc |
to the \fBXBITMAPFILE\fR function is read at application run time by MRM.
.mc
.P 0
.mc |
The \fBXBITMAPFILE\fR function returns a value of type 
.mc
\fBpixmap\fP 
and can be used
.mc |
anywhere a pixmap data type is expected. The \fBXBITMAPFILE\fR function has the
.mc
following syntax:
.mc |
.P
.nf
\fBXBITMAPFILE\fR( \fIstring-expression\fP );
.fi
.mc
.P 0
.mc |
The following example shows how to use the \fBXBITMAPFILE\fR function:
.P
.nf
.ft CW
.S -2
.mc
VALUE
.mc |
    background_pixmap=XBITMAPFILE('myfile_button.xbm');
.S
.fi
.ft R
.mc
.P 0
.mc |
In this example, the X bitmap specified in \fBmyfile_button.xbm\fP
is used to create a pixmap, which can be referenced by the value 
.mc
\fIbackground_pixmap\fP.
.mc |
.H 3 "The SINGLE_FLOAT Function"
.mc
.P 0
.mc |
.iX "UIL" "floating-point values"
.iX "UIL module" "floating-point values"
.iX "UIL specification file" "floating-point values"
.iX "UIL Functions" "SINGLE_FLOAT"
.iX "Floating-point values"
The \fBSINGLE_FLOAT\fR function lets you store floating-point literals in UIL
files as single-precision, floating-point numbers.
Single-precision floating-point numbers can often be stored using less
memory than double-precision, floating-point numbers.
.P
The function has the following syntax:
.P
.nf
\fBSINGLE_FLOAT\fR (\fIreal_number_literal\fP)
.fi
.P
The \fIreal_number_literal\fP can be either an integer literal or a
floating-point literal.
A value defined using this function cannot be used in an arithmetic
expression.
.H 3 "The FLOAT Function"
.P 0
.iX "UIL" "floating-point values"
.iX "UIL module" "floating-point values"
.iX "UIL specification file" "floating-point values"
.iX "UIL Functions" "FLOAT"
.iX "Floating-point values"
The \fBFLOAT\fR function lets you store floating-point literals in UIL files
as double-precision, floating-point numbers.
.P
The function has the following syntax:
.P
.nf
\fBFLOAT\fR (\fIreal_number_literal\fP)
.fi
.P
The \fIreal_number_literal\fP can be either an integer literal or a
floating-point literal.
.H 3 "The FONT Function"
.P 0
.mc
.iX "UIL" "font values"
.iX "UIL module" "font values"
.iX "UIL specification file" "font values"
.mc |
.iX "UIL Functions" "FONT"
.mc
.iX "Font values"
.mc |
You define fonts with the \fBFONT\fR function.
Using the \fBFONT\fR function,
.mc
you designate a value to specify a font
and then use that value for arguments that require a font value.
The UIL compiler has no built-in fonts.  You must define all fonts using
.mc |
the \fBFONT\fR function. 
.mc
.P 0
Each font makes sense only in the context of a character set.  
.mc |
The \fBFONT\fR function has an additional
.mc
parameter to let you specify the character set
for the font.  
This parameter is optional; if you omit it,
.mc |
the default character set depends on the value of the \fBLANG\fR environment
variable if it is set of the value of \fBXmFALLBACK_CHARSET\fP if \fBLANG\fR
is not set.
.mc
The font function has the following syntax:
.mc |
.P
.nf
\fBFONT\fR( \fIstring-expression\fP [, \fBCHARACTER_SET\fR = \fIchar-set\fP ] )
.fi
.mc
.P 0
.mc |
The string expression specifies the name of the font
and the clause \fBCHARACTER_SET\fR = \fIchar-set\fR
.mc
specifies the character set for the font.  
.mc |
The string expression used in the \fBFONT\fR function cannot be a compound string.
.mc
.P 0
.mc |
.ne 3i
The following example shows how to use the \fBFONT\fR function:
.mc
.P 0
.mc |
.P
.nf
.ft CW
.S -3
.in 0
.mc
VALUE big:  FONT('-ADOBE-Times-Medium-R-Normal--*-140-*-*-P-*-ISO8859-1');   
VALUE bold: FONT('-ADOBE-Helvetica-Bold-R-Normal--*-100-*-*-P-*-ISO8859-1');
OBJECT danger_window: 
    XmWarningDialog
    { ARGUMENTS
        { XmNdialogTitle = 'You are about to lose all changes';
          XmNlabelFontList = bold;
        };
    };
.mc |
.S
.in
.fi
.ft R
.mc
.P 0
.mc |
In this example, the FONT function is used with the \fBVALUE\fR 
declaration (described in Section 19.3)
.mc
to define two fonts and give them names. One of these fonts,
.mc |
\fIbold\fP,
.mc
is automatically converted to a font table by the compiler
(because the argument 
.mc |
\fBXmNlabelFontList\fR
.mc
requires a font table) and
is used to specify the text font of the warning dialog. 
.P 0
.mc |
Use the wildcard character * (an asterisk) to 
.mc
specify fonts in a device-independent manner.
.P 0
.mc *
.mc
If possible, you should not specify fonts for objects in your
.mc |
application interface.  This allows end users to control font selection
through the \fB.Xdefaults file\fR.
.H 3 "The FONT_TABLE Function"
.mc
.P 0
.iX "UIL" "font table value"
.iX "UIL module" "font table value"
.iX "UIL specification file" "font table value"
.iX "Functions" "font table value"
.mc |
.iX "UIL Functions" "FONT_TABLE"
.mc
.iX "Font table value" "defining"
.mc |
A font table is a sequence of pairs of fonts and character sets.  At run time
.mc
when an object needs to display a string, the object
scans the font table for the character set that
matches the character set of the string to be displayed.
.P 0
.mc |
UIL provides the \fBFONT_TABLE\fR function to let you supply such an argument.
The syntax of the \fBFONT_TABLE\fR function is as follows:
.P
.nf
\fBFONT_TABLE\fR( \fIfont expression\fP,... ) 
.fi
.mc
.P 0
.mc |
The font expression is created with the \fBFONT\fR function.
.mc
.P 0
If you specify a single font value to specify an argument that requires
a font table, the UIL compiler automatically converts a font value to a font
table.
.mc |
.br
.ne 3i
.H 3 "The COMPOUND_STRING Function"
.mc
.P 0
.mc |
Use the \fBCOMPOUND_STRING\fR function to set properties of a null-terminated string
.mc
and to convert it into a compound string.  
The properties you can set are the character set, writing direction, and 
separator.
.P 0
.mc |
The \fBCOMPOUND_STRING\fR function has the following syntax: 
.P
.nf
\fBCOMPOUND_STRING\fR( \fIstring-expression\fP [, \fIproperty\fP ]...);
.fi
.mc
.P 0
.mc |
The result of the \fBCOMPOUND_STRING\fR function is a compound string with the
.mc
string expression as its value.
You can optionally include one or more of the following clauses to specify
.mc |
properties for the resulting compound string: 
.P
.nf
\fBRIGHT_TO_LEFT\fR = \fIboolean-expression\fP
\fBSEPARATE\fR = \fIboolean-expression\fP
.fi
.mc
.P 0
.mc |
The \fBRIGHT_TO_LEFT\fR clause sets the writing direction of the
string from right to left if \fIboolean-expression\fP 
is True, and left to right otherwise.  
.mc
Specifying this argument does not cause the value 
of the string expression to change.
.mc |
If you omit the \fBRIGHT_TO_LEFT\fR argument, the resulting string has the same
.mc
writing direction as \fIstring-expression\fP.
.P 0
.mc |
The \fBSEPARATE\fR clause appends a separator to the end of the compound string if
\fIboolean-expression\fP is True.  If you omit the \fBSEPARATE\fR
.mc
clause, the resulting string does not have a separator.
.P 0
You cannot use imported or exported values as the operands of the 
.mc |
\fBCOMPOUND_STRING\fR function.  
.br
.ne 3i
.H 3 "The COMPOUND_STRING_TABLE Function"
.mc
.P 0
A compound string table is an array
of compound strings.  Objects requiring a list of string values, such
.mc |
as the \fBXmNitems\fR and \fBXmNselectedItems\fR
.mc
arguments for the list widget, use string table values.  The
.mc |
\fBCOMPOUND_STRING_TABLE\fR function builds the values
for these two arguments of the list widget.  The \fBCOMPOUND_STRING_TABLE\fR
function generates a value of type \fBstring_table\fP.
The name \fBSTRING_TABLE\fR is a synonym for \fBCOMPOUND_STRING_TABLE\fR.
.mc
.P 0
.mc |
The \fBCOMPOUND_STRING_TABLE\fR function has the following syntax:
.P
.nf
\fBCOMPOUND_STRING_TABLE\fR(\fIstring-expression\fP,...)
.fi
.mc
.P 0
The following example shows how to specify a string table:
.mc |
.P
.nf
.ft CW
.S -2
.mc
OBJECT file_privileges: XmList
.mc |
   { ARGUMENTS
        { XmNitems = COMPOUND_STRING_TABLE("owner read",
                                           "owner write",
                                           "owner delete",
                                           "system read",
                                           "system write",
                                           "system delete",
                                           "group read",
                                           "group write",
                                           "group delete" );
          XmNselectedItems = COMPOUND_STRING_TABLE
                                          ("owner read",
                                           "owner write",
                                           "system read",
                                           "system write" );
        };
   };				   
.S
.fi
.ft R
.mc
.P 0
This example creates a list box with nine menu choices. Four of the choices
are initially displayed as having been selected.
.P 0
The strings inside the string table can be simple strings, which the UIL
.mc |
compiler automatically converts to compound strings.
.br
.ne 2i
.H 3 "The ASCIZ_STRING_TABLE Function"
.mc
.P 0
An ASCIZ string table is an array of ASCIZ (null-terminated) string
values separated by commas. This function allows you to pass more than one
.mc |
ASCIZ string as a callback tag value.  The \fBASCIZ_STRING_TABLE\fR function
generates a value
.mc
of type 
\fBasciz_table\fP.
.mc |
The name \fBASCIZ_TABLE\fR is a synonym for \fBASCIZ_STRING_TABLE\fR.
.mc
.P 0
.mc |
The \fBASCIZ_STRING_TABLE\fR function has the following syntax:
.P
.nf
\fBASCIZ_STRING_TABLE\fR(\fIstring-expression\fP,...);
.fi
.mc
.P 0
.mc |
The following example shows how to specify an ASCIZ string table passed
as a callback tag:
.P
.nf
.ft CW
.S -2
.in 0
.mc
VALUE
.mc |
  v1 = "my_value_1";
  v2 = "my_value_2";
  v3 = "my_value_3";
.mc
OBJECT press_my: XmPushButton {
.mc |
 CALLBACKS {
  XmNactivateCallback = PROCEDURE my_callback(asciz_table(v1, v2, v3));
  };
 };     	        
.S
.in
.ps
.fi
.ft R
.mc
.P 0
.mc |
.H 3 "The INTEGER_TABLE Function"
.mc
.P 0
.mc *
.mc
An integer table is an array of integer values separated by commas.
This function allows you to pass more than one integer per callback
tag value.
.mc |
The \fBINTEGER_TABLE\fR function generates a value of type
\fBinteger_table\fP.
.mc
.P 0
.mc |
The \fBINTEGER_TABLE\fR function has the following syntax:
.P
.nf
\fBINTEGER_TABLE\fR( \fIinteger-expression\fP,...);
.fi
.mc
.P 0
.mc |
.ne 3.5i
The following example shows the \fBINTEGER_TABLE\fR function used to define an array
of integers to be passed as a callback tag to the procedure \fImy_callback\fP:
.P
.nf
.ft CW
.S -2
.in0
.mc
VALUE
.mc |
  v1 =  1;
  v2 =  2;
  v3 =  3;
.mc
OBJECT press_my: XmPushButton {
.mc |
 ARGUMENTS {
  XmNheight = 30;
  XmNwidth =  10;
 };	   
 CALLBACKS {
  XmNactivateCallback\fR \fP=\fR \fPPROCEDURE my_callback(integer_table(v1,\fR \fPv2,\fR \fPv3));
 };
.mc
};       
.mc |
.S
.in
.fi
.ft R
.H 3 "The ARGUMENT Function"
.mc
.P 0
.iX "UIL" "argument values"
.iX "UIL module" "argument values"
.iX "UIL specification file" "argument values"
.mc |
.iX "UIL Functions" "ARGUMENT"
.mc
.iX "Argument values" "defining in UIL"
.mc |
The \fBARGUMENT\fR function defines the arguments to a user-defined widget.
.mc
Each of the objects that can be described by UIL permits a set of arguments,
listed in Appendix B.  For example,
.mc |
\fBXmNheight\fR
.mc
is an argument to most objects and has integer data type.  To specify
height for a user-defined widget, you can use the built-in argument
name 
.mc |
\fBXmNheight\fR,
.mc
and specify an integer value when you declare the user-defined widget.
.mc |
You do not use the \fBARGUMENT\fR function to specify arguments that are 
.mc
built into the UIL compiler.
.P 0
.mc |
The \fBARGUMENT\fR function has the following syntax: 
.P
.nf
\fBARGUMENT\fR( \fIstring-expression\fP [, \fIargument_type\fP ])
.fi
.mc
.P 0
.mc |
.ne 18
The \fIstring-expression\fR name
.mc
is the
name the UIL compiler uses for the argument in the UID file.
.mc |
the \fIargument_type\fR
.mc
is the type of value that can be
associated with the argument.  If you omit the second argument, the default
type is 
.mc |
\fBany\fR
.mc
and no value type checking occurs.  Use one of the following keywords
to specify the argument type:
.BL
.LI
ANY
.LI
ASCIZ_TABLE
.LI
BOOLEAN
.LI
COLOR
.LI
COLOR_TABLE
.LI
COMPOUND_STRING
.LI
FLOAT
.LI
FONT
.LI
FONT_TABLE
.LI
ICON
.LI
INTEGER
.LI
INTEGER_TABLE
.LI
REASON
.LI
.mc |
SINGLE_FLOAT
.LI
.mc
STRING
.LI
STRING_TABLE
.LI
TRANSLATION_TABLE
.LE
.P 0
For example, suppose you built a user-defined widget that draws a circle
and takes four arguments: 
.mc |
\fImy_radius\fP, \fImy_color\fP, \fBXmNx\fR, and \fBXmNy\fR.
The following example shows how to use the \fBARGUMENT\fR function to define the
arguments to this user-defined widget.  Note that the \fBARGUMENT\fR function
is not used to specify arguments \fBXmNy\fR and \fBXmNy\fR
.mc
because these are built-in argument names.  The data type of 
.mc |
\fBXmNx\fR and \fBXmNy\fR is Position.
.mc
.P 0
.mc |
.ne 3i
.mc
When you declare the 
.I circle
.mc |
widget, you must use the \fBARGUMENT\fR function to define the name and 
.mc
data type of the arguments that are not built-ins (\fImy_radius\fP
and \fImy_color\fP).  Arguments are specified in an arguments list,
.mc |
identified by the keyword \fBARGUMENTS\fR in the following example:
.P
.nf
.ft CW
.S -2
.mc
VALUE circle_radius:  ARGUMENT( 'my_radius', INTEGER );
VALUE circle_color:  ARGUMENT( 'my_color', COLOR );
OBJECT circle:
    USER_DEFINED  PROCEDURE CircleCreate
      { ARGUMENTS
          { circle_radius = 1000;
            circle_color = color_blue
            XmNx = 1050;
            XmNy = 1050;
          };
      };
.mc |
.S
.fi
.ft R
.mc
.P 0
.mc |
In this example, the \fBARGUMENT\fR function is used in a value
declaration (described in Section 19.3)
.mc
to define the two arguments that are not UIL built-ins:  
\fIcircle-radius\fP,
which takes an integer value, and
\fIcircle-color\fP,
which takes a color
value.  
When referenced in the arguments list of the circle widget, the UIL
compiler verifies that the value you specify for each of these
arguments
.mc |
is of the type specified in the \fBARGUMENT\fR function. The following
table shows the argument list
.mc
placed in the UID file 
(and supplied to the creation
.mc |
function for the circle widget at run time).
.mc
.TS H
tab(@);
lB lB
l l.
_
.sp 6p
Argument@Argument Value
.sp 6p
_
.sp 6p
.mc *
.mc
.TH
.mc |
\f2my_radius\fP@1000
\f2my_color\fP@Value associated with the UIL name \f2color_blue\fP
.mc
.sp 6p
_
.TE
.P 0
.mc |
You can use the \fBARGUMENT\fR function to allow the UIL compiler to recognize
.mc
extensions to the Motif Toolkit.  For example, an existing widget
.mc |
may accept a new argument.  Using the \fBARGUMENT\fR function, you can make
.mc
this new argument available to the UIL compiler before the updated
.mc |
version of the compiler is released.  
.ne 3i
.H 3 "The REASON Function"
.mc
.P 0
.mc *
.mc
.iX "UIL" "reason value"
.mc |
.iX "UIL Functions" "reason value"
.mc
.iX "UIL module" "reason value"
.iX "UIL specification file" "reason value"
.iX "Functions" "reason value"
.iX "Reason value"
.mc |
The \fBREASON\fR function is useful for defining new reasons for user-defined
.mc
widgets.
.P 0
Each of the objects in the
Motif Toolkit defines a set of conditions under which it calls a
user-defined function.  These conditions are known as callback reasons.
The user-defined functions are termed callback procedures.  In a UIL
module, you use a callbacks list to specify which user-defined
functions are to be called for which reasons.
.P 0
Appendix B lists the callback reasons 
supported by the Motif Toolkit
objects.
.P 0
When you declare a user-defined widget, you can define callback reasons
.mc |
for that widget using the \fBREASON\fR function.
The \fBREASON\fR function has the following syntax:
.P
.nf
\fBREASON\fR(\fIstring-expression\fP)
.fi
.P
.mc
The string expression specifies the argument name stored in the UID
.mc |
file for the reason.  This reason name is supplied to the widget
.mc
creation routine at run time.
.P 0
Suppose you built a new widget that implements a password system
to prevent a set of windows from being displayed unless a user
enters the correct password.  The widget might define the 
following callbacks:
.mc |
.P
.nf
.ft CW
.mc
VALUE passed: REASON( 'AccessGrantedCallback' );
VALUE failed: REASON( 'AccessDeniedCallback' );
OBJECT guard_post:     
    USER_DEFINED  PROCEDURE guard_post_create
{ CALLBACKS
    { passed = PROCEDURE display_next_level();
      failed = PROCEDURE logout(); };
    };
.mc |
.fi
.ft R
.mc
.P 0
.mc |
In this example, the \fBREASON\fR function is used in a value 
declaration to define two new reasons, \fIpassed\fP and \fIfailed\fP.
The callback list of the widget named \fIguard_post\fP 
.mc
specifies the procedures to be called when these reasons occur.
.P 0
.mc |
.ne 3i
.mc
A widget specifies its callbacks by defining an argument for
.mc |
each reason that it supports.  The argument to the \fBREASON\fR function gives
.mc
the name of the argument that supports this reason.
Therefore, the argument list placed in the UID file for the 
\fIguard_post\fP
.mc |
widget includes the arguments listed in the following table.
.mc
.P 0
.ne 2.5i
.TS H
tab(@);
lB lB
l l.
_
.sp 6p
Argument@Argument Value
.sp 6p
_
.sp 6p
.mc *
.mc
.TH
.mc |
\f2AccessGrantedCallback\fP@Callback structure for procedure \f2display_next_level\fP
\f2AccessDeniedCallback\fP@Callback structure for procedure \f2logout\fP
.mc
.sp 6p
_
.TE  
.mc |
.H 3 "The TRANSLATION_TABLE Function"
.mc
.P 0
.mc *
.mc
.iX "UIL" "translation table value"
.iX "UIL module" "translation table value"
.iX "UIL specification file" "translation table value"
.iX "Functions" "translation table value"
.iX "Translation table value"
Each of the Motif Toolkit
widgets has a translation table that maps
X events (for example, mouse button 1 being pressed) to a sequence
of actions. Through widget arguments, such as the common translations argument,
you can specify an alternate set of events or actions for a particular widget. 
.mc |
The \fBTRANSLATION_TABLE\fR function creates a translation table that can be used as
the value of a argument that is of the data type \fBtranslation_table\fP.  
The \fBTRANSLATION_TABLE\fR function has the following syntax: 
.P
.nf
\fBTRANSLATION_TABLE\fR( \fIstring-expression\fP,... ) 
.fi
.mc
.P 0
.mc |
.ne 3i
.mc
Each of the string expressions specifies the
run-time binding of an X event
to a sequence of actions, as shown in the
following example 
.mc |
(arguments lists are discussed in Section 19.5.1):
.mc
.P 0
.mc |
.P
.nf
.ft CW 
.S -2
.in0
.mc
LIST new_translations:
.mc |
   ARGUMENTS
   { XmNtranslations = 
     TRANSLATION_TABLE
     ( 
      '#override',
      '<Btn1Down>:        XMPBFILLHIGHLIGHT() XMPBARM() XMPBUNGRAB()',
      '<Btn1Up>:          XMPBFILLUNHIGHLIGHT() XMPBACTIVATE() \e
                          XmSelectionDialog(self_destruct) XMPBDISARM()',
      '<Btn3Up>:          XMPBHELP()',
      'Any<LeaveWindow>:  XMPBFILLUNHIGHLIGHT() XMPBUNGRAB() \e
                          XMPBDISARM()'
     );
   };
.mc
OBJECT self_destruct: 
       XmPushButton
       { ARGUMENTS new_translations; };
.mc |
.in
.S
.fi
.ft R
.mc
.P 0
.mc |
.ne 2i
.mc
This example defines an argument list, called 
.mc |
\fInew_translations\fP.
.mc
The first translation
.mc |
specifies that pressing the left button <\fBBtn1Down\fR> should result in 
the sequence of procedures \fBXMPBFILLHIGHLIGHT\fR(), \fBXMPBARM\fR(), and 
\fBXMPBUNGRAB\fR() executing.  
.mc
The 
.mc |
\fIself_destruct\fP
.mc
push button widget defines 
.mc |
\fInew_translations\fP
.mc
as its translation table. 
.P 0
.mc |
In this example, the first line of the definition is a translation table
directive that indicates that the current translations are to be
overridden with those specified in the translation table.
The translations defined by \fInew_translations\fP will override the
current translations for the \fIself_destruct\fP PushButton.
.mc
.P 0
.mc |
You can use one of the following translation table directives with the
\fBTRANSLATION_TABLE\fR function: #override, #augment, or #replace.
The default is #replace.
If you specify one of these directives, it must be the first entry in
the translation table.
.P 0
The #override directive causes any duplicate translations to be ignored.
For example, if a translation for <\fBBtn1Down\fR> is already defined in the
current translations for a PushButton, the translation defined by
\fInew_translations\fP overrides the current definition.
If the #augment directive is specified, the current definition takes
precedence.
The #replace directive replaces all current translations with those
specified in the \fBXmNtranslations\fP resource.
.H 2 "The any Data Type"
.P 0
.iX "UIL" "any value"
.iX "UIL module" "any value"
.iX "UIL specification file" "any value"
.iX "Functions" "any value"
.iX "any value"
.mc
The purpose of the 
.mc |
\fBany\fR
.mc
data type is to shut off the data-type checking feature of the UIL compiler.
You can use the 
.mc |
\fBany\fR
.mc
data type for the following:
.BL
.LI
Specifying the type of a callback procedure tag
.LI
Specifying the type of a user-defined argument
.LE
.mc |
.P
.mc
You can use the 
.mc |
\fBany\fR
.mc
data type when you need to use a type not
.mc |
supported by the UIL compiler or when you want the data-type
.mc
restrictions imposed by the compiler to be relaxed.
For example, you might want to define a widget having an argument that
can accept different types of values, depending on run-time
circumstances.
.P 0
.mc |
.ne 3i
.mc
If you specify that an argument takes an 
.mc |
\fBany\fR
.mc
value, the compiler does not
check the type of the value specified for that argument; therefore, you
need to take care when specifying a value for an argument of type 
.mc |
\fBany\fR.
.mc
You could get unexpected results at run time if you pass a value having
a data type that the widget does not support for that argument.
.mc *
.mc
.H 2 "Compile-Time Value Expressions"
.iX "UIL" "compile-time expressions"
.iX "Compile-time expressions"
.iX "Expressions" 
.P 0
UIL provides literal values for a diverse set of types (integer,
string, real, Boolean) and a set of Motif Toolkit-specific types (for
example, colors and fonts).
These values are used to provide the value of Motif Toolkit
arguments.
.P 0
UIL includes compile-time value expressions.  These expressions
.mc |
can contain references to other UIL values, but cannot be 
.mc
forward referenced.
.P 0
Compile-time value expressions are useful for implementing relative
.mc |
positioning of children without using the Form widget.  
.mc
For example, suppose you wanted to create a message box
.mc |
inside a bulletin board, and you want
.mc
the message box to be half as
.mc |
wide and half as tall as the bulletin board, and centered within it.
.mc
Using compile-time expressions, you can specify the coordinates
of the message box by referring to the values you already defined
.mc |
for the bulletin board.  If you do not use compile-time value expressions
in this case, you must compute the x and y location and the
.mc
height and width of the message box, and recompute these values
.mc |
if the bulletin board changes size or location.  Furthermore, the
computed values are absolute numbers rather than descriptive
expressions like "bulletin_board_width / 2".
.mc
.P 0
.mc |
Note that the Motif Toolkit provides
.mc
direct support for resolution independence with the millimeter, inch,
.mc |
and point unit types; the preceding example could be implemented using this feature.
.mc
.P 0
The concatenation of strings is also a form of compile-time 
expression.  Use the concatenation operator to join strings and 
convert the result to a compound string.
.P 0
.mc |
.ne 3i
The following table lists the set of operators in UIL that
.mc
allow
you to create integer, real, and Boolean values based on other values
defined
with the UIL module.  In the table, a precedence of 1 is the highest.
.mc |
.P
.ne 3i
.in0
.mc
.TB "Operators"
.TS H
tab(@);
lBw(.75i) lBw(.75i) lBw(1i) lBw(.675i) cBw(1i)
lBw(.75i) lBw(.75i) lBw(1i) lBw(.675i) lBw(1i)
cw(.75i) lw(.75i) lw(1i) lw(.675i) cw(1i).
.mc
_
.sp 6p
Operator@Operator@Operand@Meaning@Precedence
@Type@Types@@
.sp 6p
_
.sp 6p
.mc *
.mc
.TH
~@Unary@Boolean@NOT@1
.mc |
@@integer@T{
One's complement
T}@
.sp 
.mc
-@Unary@float@Negate@
@@integer@Negate@1
.sp
.mc |
+@Unary@float@NOP@1
.mc
@@integer@NOP@1
.sp
*@Binary@float,float@Multiply@2
@@integer,integer@Multiply@2
.sp
/@Binary@float,float@Divide@2
@@integer,integer@Divide@2
.sp
+@Binary@float,float@Add@3
@@integer,integer@Add@3
.sp
\-@Binary@float,float@Subtract@3
@@integer,integer@Subtract@3
.sp
>>@Binary@integer,integer@Shift right@4
.sp
<<@Binary@integer,integer@Shift left@4
.sp
.mc |
.ne 4
.mc
&@Binary@Boolean,Boolean@AND@5
@@integer,integer@Bitwise AND@5
@@string,string@Concatentate@5
.sp
|@Binary@Boolean,Boolean@OR@6
@@integer,integer@Bitwise OR@6
.sp
^@Binary@Boolean,Boolean@XOR@6
@@integer,integer@Bitwise XOR@6
.sp 6p
_
.TE
.mc |
.in
.P
.mc
A string can be either a single compound string or a sequence of
compound strings.  If the two
concatenated strings have different properties (such as writing
direction or character set), the result of the concatenation is a 
.mc |
multisegment compound string.
.mc
.P 0
The result of each operator has the same type as its operands.
You cannot mix types in an expression without using conversion routines.
.P 0
You can use parentheses to override the normal precedence of operators.
.mc *
.mc
In a sequence of unary operators, the operations are performed in
right-to-left
order.  For example, \- + \-A is equivalent to \-(+(\-A)).
.mc *
.mc
In a sequence of binary operators of the same precedence, the operations
.mc |
are performed in left-to-right order.  For example, A*B/C*D is
equivalent to ((A*B)/C)*D.
.mc
.P 0
A value declaration gives a value a name.  You cannot redefine the value
.mc |
of that name in a subsequent value declaration.
.mc
You can use a value containing operators and functions anywhere you can
use a value in a UIL module.
.mc |
You cannot use imported values as operands in expressions.
.mc
.iX "UIL" "data type conversions"
.iX "Data type" "conversions"
.iX "Conversions" "data type"
.P 0
Several of the binary operators are defined for multiple data types.
For example, the operator for multiplication (*) is defined for both
floating-point and integer operands.
.P 0
For the UIL compiler to perform these binary operations, both
operands must be of the same type.
If you supply operands of different data types, the UIL compiler
automatically converts one of the operands to the type of the other
.mc |
according to the conversions rules listed in the following table.
.P
.mc
.br
.mc |
.ne 3i
.mc
.TB "Automatic Data Type Conversions"
.P 0
.mc *
.mc
.TS H
tab(@);
lB lB lB
lB lB lB
l l lw(3i).
_
.sp 6p
.mc |
Data Type@Data Type@@
of Operand 1@of Operand 2@Conversion Rule@
.mc
.sp 6p
_
.sp 6p
.TH
Boolean@Integer@T{
Operand 1 converted to integer
T}
Integer@Boolean@T{
Operand 2 converted to integer
T}
Integer@Floating-point@T{
Operand 1 converted to floating-point
T}
Floating-point@Integer@T{
Operand 2 converted to floating-point
T}
.sp 6p
_
.TE
.P 0
You can also explicitly convert the data type of a value by using
.mc |
one of the functions listed in the following table. 
.mc
.iX "UIL module" "data type conversion functions in"
.iX "Functions" "data type conversion"
.mc |
.iX "UIL Functions" "data type conversion"
.mc
.iX "UIL" "data type conversion"
.iX "Data type conversion"
.mc |
.sp .35
.mc
.TB "Conversion Functions"
.TS H
tab(@);
lB lB lBw(2.5i)
l l lw(2.5i).
.mc
_
.sp 6p
Result@Function@Comments
.sp 6p
_
.sp 6p
.mc *
.mc
.TH
Integer@INTEGER(Boolean)@TRUE\->1, FALSE\->0
Integer@INTEGER(integer)@
Integer@INTEGER(float)@T{
Integer part of the floating-point number (truncate toward zero); this can result in overflow.
T}
.mc |
Double-precision, floating-point@FLOAT(Boolean)@
Double-precision, floating-point@FLOAT(integer)@T{
Double-precision, floating-point representation of an integer; there
should not be any loss of precision.
.mc
T}
.mc |
Double-precision, floating-point@FLOAT(float)@
Single-precision, floating-point@SINGLE_FLOAT(Boolean)@
Single-precision, floating-point@SINGLE_FLOAT(integer)@T{
Single-precision, floating-point representation of an integer; there
should not be any loss of precision.
T}
Single-precision, floating-point@SINGLE_FLOAT(float)@
.sp 3p
.mc
_
.TE
.P 0
.mc |
.ne 4i
.mc
The following example shows a value section from a UIL module containing
compile-time
expressions and data conversion functions.
.iX "UIL module" "using compile-time expressions in"
.iX "UIL" "compile-time expressions"
.iX "Compile-time expressions"
.iX "UIL" "operators"
.iX "Functions" "data type conversion"
.mc |
.iX "UIL Functions" "data type conversion"
.mc
.iX "Data type conversion"
.mc |
.P
.nf
.ft CW
.S -2
.in0
.mc
VALUE
  value outer_box_width:  200;
  value outer_box_height: 250;
  value box_size_ratio:   0.5;
  value inner_box_width: integer(outer_box_width * box_size_ratio);
  value inner_box_height: integer(outer_box_height * box_size_ratio);
  value inner_box_x:     (outer_box_width - inner_box_width) >> 1;
  value inner_box_y:     (outer_box_height - inner_box_height) / 2;
  value type_field:       0;
  value class_field:      16;
  value type1:            1;
  value type2:            2;
  value type3:            3;
  value class1:           1;
  value class2:           2;
  value class3:           3;
  value combo1:  (type1 << type_field) | (class3 << class_field);
.mc |
.in
.S
.fi
.ft R
.mc
