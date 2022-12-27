.mc |
.\"  (c) Copyright 1989, 1990 OPEN SOFTWARE FOUNDATION, INC.
.mc
.\"  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
.\"  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
.\"  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
.\"  (c) Copyright 1988 MICROSOFT CORPORATION
.\"  ALL RIGHTS RESERVED
.nr H1 7
.H 1 "Additional Functionality"
The Motif widget library provides additional functionality that you can use
to perform certain tasks.  The following list provides an overview of this
functionality.  Subsequent sections of this chapter describe each topic in
detail.
.BL
.LI
.iX "Compound strings"
.mc |
Compound strings.  A compound string is designed to allow any message or
text to be displayed without having to resort to hard-coding certain
.mc
attributes that are language dependent.  The three main attributes
involved are direction, character set, and text.  The Motif Intrinsics
library provides a number of functions that allow you to create and manipulate
compound strings.
.LI
.iX "Cut and paste functions"
.iX "Functions" "cut and paste"
.mc |
Cut and paste functions.  The Motif Intrinsics has a clipboard that
.mc
is used to hold data that is being transferred between applications.  The
.mc |
Motif intrinsics also provide a set of cut and paste functions
.mc
that allow you
to modify the type and value of the data. 
.LI
.iX "Dynamic resource defaulting"
.iX "Defaulting, dynamic"
.mc |
Dynamic resource defaulting.  This feature incorporates a processing
.mc
function into a widget's resource definitions.  This function is 
used to calculate a default resource value when the widget is created, thus
overriding any static default value.
.mc |
.br
.ne 6
.mc
.LI
.iX "Grabbing"
.iX "Keyboard grabbing"
.mc |
Keyboard grabbing.  Grabbing refers to an action in which an application
.mc
or window isolates an input device (the keyboard in this case) from other
applications or windows, thus preventing the other applications or windows
from using the device.
.LI
.iX "Localization of defaults file"
.iX "Defaults files" "localization"
Localization.   You can use localized defaults files in the Motif system by
specifying the location of these files within certain environment
.mc |
variables.  \fBXtAppInitialize\fP determines the proper path to the localized
.mc
defaults variables.
.LI
.iX "Pixmap" "naming"
.iX "Pixmap" "caching"
.mc |
Pixmap naming and caching.  The Motif widgets provide functions that 
.mc
allow you to associate any image with a unique name.  With this
association, the functions can generate pixmaps through references to
.mc |
a \fB\&.Xdefaults\fP file and through an argument list for all widgets with
.mc
pixmap resources.
.LI
.iX "Resolution independence"
.mc |
Resolution independence.  Resolution independence is a feature that allows
.mc
your application to create and display images that are the same physical
size regardless of the resolution of the display.
.LI
.iX "Motif" "window manager"
.iX "Interacting with MWM"
.mc |
Interacting with MWM.  There are a number of functions
you can use when interacting with the MWM.  A sample
.mc
program is available that demonstrates how to use some of these functions.
.LI
.iX "Motif" "version"
.iX "Motif" "window manager"
.mc |
Motif version and MWW presence.  There are functions that 
.mc
provide information on the current version of Motif and whether or not
.mc |
MWM is running.
.mc
.LE
.H 2 "Compound Strings"
.iX "Compound strings" "definition"
A compound string is designed to allow any message or
.mc |
text to be displayed without having to resort to hard-coding certain
.mc
attributes that are language dependent.  The three main attributes
involved are character set, direction, and the text of the message. For  
example, suppose you have a message to display in English and Arabic.  This can easily be done
with a compound string because you specify not only the text of the
.mc |
message, but character set and direction as well.
.mc
.BL 
.LI
.iX "Character set"
.iX "Compound strings" "character set"
Character set.  The mapping between the string of bytes that
make up the text and the font in which the text will be displayed. 
.mc |
.br
.ne 6
.mc
.LI
.iX "Direction, compound string"
.iX "Compound strings" "direction"
Direction.  The relationship between the logical order (keystroke
entry) and the display order of the characters in a string.  In
English, the display order is left to right; as characters
are typed, they are displayed from left to right.  In Arabic,
.mc |
the direction is right to left; as characters are typed, they
.mc
are displayed from right to left.
.LI
.iX "Compound strings" "text"
Text.  This is simply the text of the message or string you wish to
display.
.LE
.P
In addition, you specify a font list from which to select the fonts
used to display the message.  The font list contains 
character set references that are
matched with an X font.  To display a compound string, the widget 
uses the character set specified in the compound string and searches the
font list for a matching character set.  Note that the font list is a
widget resource.
.P
Motif provides a set of compound string functions
that enable the creation and manipulation of compound strings and font
lists. This chapter discusses the compound 
string functions you can use to 
.BL
.LI
.mc |
Create compound strings
.mc
.LI
.mc |
Compare and manipulate compound strings
.mc
.LI
.mc |
Create a font list and font list entries
.mc
.LE
.H 3 "Components of a Compound String"
.iX "Compound strings" "components"
.iX "Tag-length-value"
.iX "Compound strings" "tag-length-value"
A compound string is a stream of data that is made up of 
.mc |
tag-length-value (TLV) segments.  Each TLV segment represents an attribute
.mc
of the compound string.  Within a TLV segment, the fields are
.BL
.LI
.mc |
Tag.  A 1-byte field that identifies the type of Value that follows.  
.mc
.LI
.mc |
Length.  A 2-byte field that specifies the length of the data in
.mc
the Value field.
.LI
Value.  The value of the segment.  The length of this field is the
number of bytes specified in the Length field.
.LE
.P
For example, for a TLV segment that identifies the character set to be
.mc |
used, the Tag field identifies the segment as a character set segment,
.mc
the Length field sets the
length of the segment, and the Value field contains the character-set
identifier.
.P
.mc |
A compound string always begins with a Tag field set to \fB0x7f\fP
.mc
and a Length field set to the length of the segments that follow.
Subsequent TLV segments then define the remainder of the compound string.
The Value field of each of the TLV segments contains the information about
the attributes of the compound string.  These attributes are described below.
.BL 
.LI
.iX "Character set" "identifier"
.iX "Compound strings" "character set identifier"
.mc |
A character-set identifier.  This is a sequence
.mc
of bytes that identifies the desired character set.
This information is used by Motif
to match a font with a compound string segment.
.mc |
All text between two character-set identifiers are
.mc
interpreted to be in the first set.  It is an error for
.mc |
a text component to precede the first character-set identifier.
.sp
The character-set identifier has \fBpersistence\fP, that is, any specified
.mc
character set is used for all subsequent text segments until a new
character set is encountered.
.mc |
.sp
.mc
There are times when an application must create a string without
knowing what character sets will be available at the time the string
.mc |
is to be displayed.  Motif provides a special character-set
.mc
identifier that will match any available font.
.iX "Universal character set"
.iX "Character set" "universal"
.iX "Compound strings" "universal character set"
This universal character
set is specified by the XmSTRING_DEFAULT_CHARSET identifier.  If this
identifier is used as the character set when a compound string is created,
.mc |
it will match the first font in the font list used to display the string,
.mc
regardless of the character set associated with that particular font.
By using the universal character set, an application can construct its 
strings so that they will be displayed in any font desired by the
user, no matter what character set is associated with that font.
.mc |
.sp
.mc
The universal character set can also be associated with a font in a font
list.  When used with a font, that font will match any string, no matter
what the character set of the string.  An application can thus construct
.mc |
a multiple-font font list and specify a default font to be used when no
.mc
other font is matched.
.LI
.iX "Direction, compound string"
.iX "Compound strings" "direction"
.mc |
A direction.  This can have three values: left-to-right,
right-to-left, and revert.  Like the character-set identifier,
.mc
it has persistence.
The default direction is left-to-right; that is, text components
preceding the first direction component will be assigned a direction
of left-to-right.
.mc |
.br
.ne 6
.mc
.LI
.iX "Compound strings" "text"
.iX "Text" "compound string"
Text.  This is the actual character data.  There
are no semantics for any bytes.  Specifically this means that
characters like \\n do not have any meaning.  As a convenience there
.mc |
is a function, \fBXmStringCreateLtoR\fP, which does impose this single
.mc
semantic.
.mc |
.br
.ne 5
.mc
.LI
.iX "Separator, compound string"
.iX "Compound strings" "separator"
A separator.  This is a tag with no value.  It is simply a marker
that allows an array of compound string segments to be presented
as a single entity.
.LE
.P
The following is a set of useful definitions for compound strings:
.P
.nf
.mc |
.in0
.S -2
.mc
\f(CWtypedef unsigned char   XmStringDirection  /* an enumerated type */\fP
.mc |
.S
.in
.mc
.fi
.P
The set of possible values for this type are
.nf
.P
.mc |
.in0
.S -2
.mc
\f(CWXmSTRING_DIRECTION_L_TO_R
XmSTRING_DIRECTION_R_TO_L

.mc |
typedef char * XmStringCharSet /*octet chars, null terminated*/
.mc

.mc |
typedef char * XmString  /*opaque to users*/
.mc

.mc |
typedef unsigned char XmStringComponentType /*component tag types*/\fP
.in
.S
.fi
.P
The set of currently possible values for this type are:
.nf
.in0
.S -2
\f(CW#define XmSTRING_COMPONENT_UNKNOWN 0
.mc
#define XmSTRING_COMPONENT_CHARSET 1
#define XmSTRING_COMPONENT_TEXT  2
#define XmSTRING_COMPONENT_DIRECTION 3
#define XmSTRING_COMPONENT_SEPARATOR 4
.mc |
#define XmSTRING_COMPONENT_END  126 /*no more components*/
#define XmSTRING_COMPONENT_RESERVED 127 /*0-127 reserved for Motif*/
#define XmSTRING_DEFAULT_CHARSET (-1) /*The universal character set*/\fP
.in
.S
.fi
.mc
.H 3 "Compound String Functions"
.iX "Functions" "compound string"
.iX "Compound strings" "functions"
.mc |
You can use a number of functions associated with compound strings.
The following table lists these functions
.mc
along with a brief
description of what each can do.  Subsequent sections describe the
.mc |
functions in more detail.  A short sample program at the end of
the list of functions shows how to use some of them.
.SK
.in0
.mc
.iX "XmFontListAdd"
.iX "XmFontListCreate"
.iX "XmFontListFree"
.mc |
.iX "XmFontListFreeFontContext"
.iX "XmFontListGetNextFont"
.iX "XmFontListInitFontContext"
.mc
.iX "XmStringBaseline" 
.iX "XmStringByteCompare" 
.iX "XmStringCompare" 
.iX "XmStringConcat"
.iX "XmStringCopy"
.iX "XmStringCreate"
.iX "XmStringCreateLtoR"
.mc |
.iX "XmStringCreateSimple"
.mc
.iX "XmStringDirectionCreate" 
.iX "XmStringDraw"
.iX "XmStringDrawImage" 
.iX "XmStringDrawUnderline" 
.iX "XmStringEmpty"
.iX "XmStringExtent" 
.iX "XmStringFree"
.iX "XmStringFreeContext"
.iX "XmStringGetLtoR" 
.iX "XmStringGetNextComponent"
.iX "XmStringGetNextSegment"
.mc |
.iX "XmStringHasSubstring"
.iX "XmStringHeight"
.iX "XmStringInitContext" 
.iX "XmStringLength"
.iX "XmStringLineCount" 
.iX "XmStringNConcat" 
.iX "XmStringNCopy" 
.iX "XmStringPeekNextComponent"
.iX "XmStringSegmentCreate"
.iX "XmStringSeparatorCreate"
.iX "XmStringWidth" 
.mc
.TB "Compound String Functions"
.mc |
.TS H
box, tab(;);
lb | lb
lB | l .
.mc
.sp .2
Function Name;Description
.sp .2
_
.mc |
.TH
.mc
.sp .2
.mc |
XmFontListAdd;Adds an entry to an existing font list.
XmFontListCopy;Copies a font list.
XmFontListCreate;Creates a new font list.
XmFontListFree;Recovers memory used by a font list.
XmFontListFreeFontContext;Recovers memory used by a font list context.
XmFontListGetNextFont;Accesses fonts and character sets in a font list.
XmFontListInitFontContext;T{
.na
Creates a context for accessing fonts
and character sets in a font list.
.ad
T}
.mc
XmStringBaseline;T{
.na
Returns the number of pixels between the top of the character box and the
baseline of the first line of text in the specified compound string.
.ad
T}
XmStringByteCompare;T{
.na
Returns True or False as the result of a byte-by-byte comparison of two
specified compound strings.
.na
T}
XmStringCompare;T{
.na
Returns True if two compound strings have the same text components, 
directions, and separators.
.ad
T}
XmStringConcat;Appends one compound string to another.
XmStringCopy;Returns a copy of the specified compound string.
XmStringCreate;Creates a compound string.
XmStringCreateLtoR;Creates a compound string in a left-to-right direction.
.mc |
XmStringCreateSimple;T{
.na
Creates a compound string in the
current language environment.
.ad
T}
.mc
XmStringDirectionCreate;T{
.na
Creates a compound string with just one component, the specified direction.
.ad
T}
XmStringDraw;Draws a compound string in an X window.
XmStringDrawImage;T{
.na
.mc |
This function is identical to \f3XmStringDraw\fP except that it also
.mc
paints both the foreground and background bits of each character.
.ad
T}
XmStringDrawUnderline;T{
.na
.mc |
Identical to \f3XmStringDraw\fP except that if the specified substring is
.mc
matched in the main string, then the substring is underlined.
.ad
T}
XmStringEmpty;Returns True if all text segments are empty.
XmStringExtent;T{
.na
Determines the height and width (in pixels) of the smallest rectangle
that will enclose the specified compound string.
.ad
T}
XmStringFree;Frees the memory used by a compound string.
XmStringFreeContext;Frees a previously established context.
XmStringGetLtoR;T{
.na
Returns True if a segment can be found in the input compound string
.mc |
that matches the specified character-set identifier.
.mc
.ad
T}
XmStringGetNextComponent;Returns the next component.
.mc *
.mc
XmStringGetNextSegment;Returns the bytes in the next segment.
.mc |
XmStringHasSubstring;T{
.na
Indicates whether one compound string
is contained within another.
.ad
T}
.mc
XmStringHeight;T{
.na
Returns the height (in pixels) of the sum of all the line heights of the
specified compound string.
.ad
T}
XmStringInitContext;T{
.na
.mc |
Specifies a context used to read the contents of a compound string segment-
by-segment.
.mc
.na
T}
XmStringLength;Returns the length of the specified compound string.
XmStringLineCount;T{
.na
Returns the number of lines of text in the specified
compound string.
.ad
T}
XmStringNConcat;T{
.na
Appends a specified number of bytes from one compound
string to another.
.ad
T}
XmStringNCopy;T{
.na
Returns a copy of a specified portion of a compound string.
.ad
T}
XmStringPeekNextComponent;Returns the type of the next component.
XmStringSegmentCreate;Creates a compound string segment.
XmStringSeparatorCreate;Creates a compound string with only a separator.
XmStringWidth;T{
.na
Returns the width (in pixels) of the longest sequence of text components
in the specified compound string.
.na
T}
.sp .2
.TE
.mc |
.in
.mc
.P
.H 4 "XmFontListAdd"
.iX "XmFontListAdd"
This function adds an entry to an existing font list.
.nf

.mc |
\fBXmFontList XmFontListAdd\fP(\fIoldlist, font, charset\fP)
    \fBXmFontList\fP \fIoldlist\fP;
    \fBXFontStruct\fP \fI*font\fP;
    \fBXmStringCharSet\fP \fIcharset;\fP
.mc
.fi
.mc |
.VL 1i
.LI "\fIoldlist\fP"
Specifies a pointer to the font list to which an entry will be added.
.LI "\fIfont\fP"
Specifies a pointer to the font structure to be added to the list.
.LI "\fIcharset\fP"
Specifies the character-set identifier for the font
being added to the list. This can be XmSTRING_DEFAULT_CHARSET.
.LE
.P
.mc
\fBXmFontListAdd\fP creates a new font list consisting of the
.mc |
contents of \fIoldlist\fP and the new font list element being added.
.mc
Note that this function de-allocates the \fIoldlist\fP after extracting
the required information; \fIoldlist\fP should not be referenced
.mc |
thereafter.  The following code segment shows you how this function is used.
.mc
.P
.ft CW
.nf
.mc |
.S -2
XmFontList font list1, font list2;
.mc

XmFontStruct *font1, *font2;

.iX "XmFontListCreate"
.mc |
font list1 = XmFontListCreate(font1, "chset1");
font list2 = XmFontListAdd(font list1, font2, "chset2");
.mc
.ft P
.mc |
.S
.mc
.fi
.P
.mc |
The variables \fIchset1\fP and \fIchset2\fP are set in an app-default file,
as shown in the sample program at the end of this section.
.mc
.P
.H 4 "XmFontListCreate"
.iX "XmFontListCreate"
.mc |
This function creates a new font list.  See the preceding segment for an
.mc
example of how to use this function.
.nf

.mc |
\fBXmFontList XmFontListCreate\fP(\fIfont, charset\fP)
   \fBXFontStruct\fP     *\fIfont\fP;
   \fBXmStringCharSet\fP \fIcharset;\fP
.mc

.fi
.VL 1i
.LI\fIfont\fP
Specifies a pointer to a font structure for which the new 
font list is generated.
.LI\fIcharset\fP
.mc |
Specifies the character-set identifier for the font.  This can be 
.mc
XmSTRING_DEFAULT_CHARSET.
.LE
.P
.mc |
The \fBXmFontListCreate\fP function 
.mc
creates a new font list with a single element specified by the
provided font and character set. It also allocates the space for the 
font list.
.H 4 " XmFontListFree"
.iX "XmFontListFree"
This function frees the memory used by a font list.
.nf

.mc |
\fBvoid XmFontListFree\fP \fI(list)\fP
    \fBXmFontList\fP \fIlist;\fP
.mc

.mc |
\fIlist\fP         Specifies the font list to be freed.
.mc
.fi
.H 4 "XmStringBaseline"
.iX "XmStringBaseline"
This function returns the number of pixels between the top of the
character box and the baseline of the first line of text in the
specified compound string.
.P
.nf
.mc |
\fBDimension XmStringBaseline (\fIfont list, string\fP)
    \fBXmFontList\fP \fIfont list;\fP
    \fBXmString\fP \fIstring;\fP
.mc
.fi
.H 4 "XmStringByteCompare"
.iX "XmStringByteCompare"
.mc |
This function determines whether two compound strings are identical. 
.mc
.nf

.mc |
\fBBoolean XmStringByteCompare\fP (\fIs1, s2\fP)
    \fBXmString\fP \fIs1, s2;\fP
.mc
.fi

This function returns True if the comparison shows the two specified
compound strings to be identical and False if they are not.
.P
It is important to note that when a compound
.mc |
string is placed in a widget, it is converted into
.mc
an internal form to allow faster processing.  Part
of the conversion process strips out
unnecessary or redundant information.  The result is that
if an application subsequently executes a call to \fBXtGetValues\fP
to retrieve a compound
string from a widget (specifically XmLabel and all of its
.mc |
subclasses), no guarantee can be given that the compound string
.mc
returned will be the same byte-for-byte as the original string 
in the widget. 
.H 4 "XmStringCompare"
.iX "XmStringCompare"
This function
.mc |
determines whether or not two compound strings are \fIsemantically\fP (but
.mc
not necessarily byte-for-byte) equivalent.

.nf
.mc |
\fBBoolean XmStringCompare\fP (\fIs1, s2\fP)
    \fBXmString\fP \fIs1, s2;\fP
.mc
.fi

This function returns True if the two compound strings are
.mc |
semantically equivalent and False otherwise.  Semantically
equivalent means that the strings have the same text components,
.mc
directions, and separators.  If character sets are specified, they
must be equal as well.  
.H 4 "XmStringConcat"
.iX "XmStringConcat"
This function appends a copy of one compound string to another
compound string. 

.nf
.mc |
\fBXmString XmStringConcat\fP (\fIs1, s2\fP)
    \fBXmString\fP \fIs1, s2;\fP
.mc
.fi

\fBXmStringConcat\fP appends \fIs2\fP to the end of \fIs1\fP
and returns the resulting compound string.  The original strings are preserved.
The space for the resulting compound string is allocated within the
function.  After using this function, you should free this space
.mc |
by calling \fBXtFree\fP.
.mc
.H 4 "XmStringCopy"
.iX "XmStringCopy"
This function creates a copy of a compound string.

.nf
.mc |
\fBXmString XmStringCopy\fP (\fIs1\fP)
    \fBXmString\fP \fIs1;\fP
.mc
.fi

This function returns a copy of \fIs1\fP.
The space for the resulting compound string is allocated within the
function.  The application is responsible for managing the 
the allocated space.  The memory can be recovered 
.mc |
by calling \fBXtFree\fP.
.mc
.H 4 "XmStringCreate"
.iX "XmStringCreate"
This function creates a compound string.
.P
.nf
.mc |
\fBXmString XmStringCreate\fP (\fItext, charset\fP)
    \fBchar\fP  *\fItext;\fP
    \fBXmStringCharSet\fP \fIcharset;\fP
.mc
.fi
.mc |
.VL 1i
.LI\fItext\fP
Specifies a pointer to a null terminated string.
.LI\fIcharset\fP
Specifies the character-set identifier to be associated with the 
given text. This can be XmSTRING_DEFAULT_CHARSET.
.LE
.mc
.P
This function creates a compound string with two components: a character
set and text.
.H 4 "XmStringCreateLtoR"
.iX "XmStringCreateLtoR
This function creates a compound string with a default direction of
left-to-right.

.nf
.mc |
\fBXmString XmStringCreateLtoR\fP (\fItext, charset\fP)
    \fBchar\fP  *\fItext;\fP
    \fBXmStringCharSet\fP \fIcharset;\fP
.mc

.mc |
.VL 1i
.LI\fItext\fP
Specifies a pointer to a null terminated string.
.LI\fIcharset\fP
Specifies the character-set identifier to be associated with the 
.mc
given text.  This can be XmSTRING_DEFAULT_CHARSET.
.mc |
.LE
.mc
.P
This function is similar to \fBXmStringCreate\fP
except that it scans the text for newline 
characters in the text.  When one is found, the text up to that point
is put into a segment followed by a separator component.  No final
separator component is appended to the end of the compound string.
The direction is defaulted to left-to-right.  Finally, note that
this function assumes that the encoding is single octet
rather than double or quadruple octet per character of text.  
.H 4 "XmStringDirectionCreate"
.iX "XmStringDirectionCreate"
This function creates a compound string with a single component,
a direction with the specified value.
.nf

.mc |
\fBXmString XmStringDirectionCreate\fP (\fIdirection\fP)
    \fBXmStringDirection\fP \fIdirection;\fP
.mc

.mc |
\fIdirection\fP\ \ \ \ \ Specifies the value of the directional component.
.mc
.fi
.P
.H 4 "XmStringDraw"
.iX "XmStringDraw"
This function is used to draw a compound string in an X window.

.nf
.mc |
\fBvoid XmStringDraw\fP\fI (d, w, font list, string, gc, x, y,
                        width, alignment, layout_direction, clip);\fP
    \fBDisplay\fP  \fI*d;\fP
    \fBWindow\fP \fIw;\fP
    \fBXmFontList\fP \fIfont list;\fP
    \fBXmString\fP \fIstring;\fP
    \fBGC\fP  \fIgc;\fP
    \fBPosition\fP \fIx,y;\fP
    \fBDimension\fP \fIwidth;\fP
    \fBByte\fP  \fIalignment;\fP
    \fBByte\fP  \fIlayout_direction;\fP
    \fBXRectangle\fP \fI*clip;\fP
.mc
.fi
.P
.mc |
The \fIx\fP and \fIy\fP parameters identify the top left coordinate of the
.mc
rectangle that contains the displayed compound string.  
.mc |
The \fIlayout_direction\fP parameter controls the direction in which the
segments of the compound string are laid out.  It is also
.mc
used to determine the meaning of the alignment parameter.
.mc |
The \fIclip\fP parameter allows the application to restrict the
.mc
area into which the compound string will be drawn.  If it is NULL,
no clipping is done.
.H 4 "XmStringDrawImage"
.iX "XmStringDrawImage"
This function is identical to \fBXmStringDraw\fP except that it paints both
the foreground and background bits of each character (equivalent to
\fBXDrawImageString\fP).
.P
.nf
.mc |
\fBvoid XmStringDrawImage\fP \fI(d, w, font list, string, gc, x, y,
                           width, alignment, layout_direction, clip);\fP
     \fBDisplay\fP \fI *d;\fP
     \fBWindow\fP  \fIw;\fP
     \fBXmFontList\fP \fIfont list;\fP
     \fBXmString\fP \fIstring;\fP
     \fBGC\fP  \fIgc;\fP
     \fBPosition\fP \fIx,y;\fP
     \fBDimension\fP \fIwidth;\fP
     \fBByte\fP  \fIalignment;\fP
     \fBByte\fP  \fIlayout_direction;\fP
     \fBXRectangle\fP *\fIclip;\fP
.mc
.fi
.H 4 "XmStringDrawUnderline"
.iX "XmStringDrawUnderline"
.mc |
This function is equivalent to \fBXmStringDraw\fP with the addition that
if the substring identified by \fIunderline\fP can be matched in \fIstring\fP
then the substring will be underlined.  Once a match has occurred, no
.mc
further matches or underlining will be done.
.nf

.mc |
\fBvoid XmStringDrawUnderline\fP \fI(disp, d, font list, string, gc, x, y, width,\fP
  \fBalignment, layout_direction, clip, underline);\fP
     \fBDisplay\fP  *\fIdisp;\fP
     \fBDrawable\fP \fId;\fP
     \fBXmFontList\fP \fIfont list;\fP
     \fBXmString\fP \fIstring;\fP
     \fBGC\fP  \fIgc;\fP
     \fBPosition\fP \fIx,y;\fP
     \fBDimension\fP \fIwidth;\fP
     \fBByte\fP  \fIalignment;\fP
     \fBByte\fP  \fIlayout_direction;\fP
     \fBXRectangle\fP \fI*clip;\fP
     \fBXmString\fP \fIunderline;\fP
.mc
.fi
.H 4 "XmStringEmpty"
.iX "XmStringEmpty"
.mc |
This function determines whether a compound string is empty.
.mc
.nf

.mc |
\fBBoolean XmStringEmpty\fP \fI(s1)\fP
    \fBXmString\fP \fIs1;\fP
.mc
.fi

.mc |
This function returns True or False depending on whether not any nonzero
.mc
text components exist in the provided compound string.  It returns
True if all text segments are empty or if the specified string
parameter is
NULL, and False otherwise.  
.H 4 "XmStringExtent"
.iX "XmStringExtent"
This function determines the width and height (in pixels) of the
smallest rectangle that will enclose the specified compound string.
.nf
.P
.mc |
\fBvoid XmStringExtent\fP \fI(font list, string, width, height)\fP
    \fBXmFontList\fP \fIfont list;\fP
    \fBXmString\fP \fIstring;\fP
    \fBDimension\fP \fI*width, *height;\fP
.mc
.P
.fi
.H 4 "XmStringFree"
.iX "XmStringFree"
This function frees the memory used by a compound string.
.P
.nf
.mc |
\fBvoid XmStringFree\fP \fI(string)\fP
.mc
.br
.mc |
   \fBXmString\fP \fIstring;\fP
.mc
.P
.fi
.mc |
\fIstring\fP       Specifies the compound string to be freed
.mc
.H 4 "XmStringFreeContext"
.iX "XmStringFreeContext"
.mc |
This function instructs the Intrinsics that the context is no longer needed
.mc
and will not be used without reinitialization.
.P
.nf
.mc |
\fBvoid XmStringFreeContext\fP \fI(context)\fP
    \fBXmStringContext\fP \fIcontext;\fP
.mc
.fi
.P
.H 4 "XmStringGetLtoR"
.iX "XmStringGetLtoR"
This function returns True if a segment can be found in the
.mc |
input compound string that matches the given character-set
.mc
identifier. 

.nf
.mc |
\fBBoolean XmStringGetLtoR\fP \fI(string, charset, text)\fP
    \fBXmString\fP \fIstring;\fP 
    \fBXmStringCharSet\fP \fIcharset;\fP
    \fBchar\fP  \fI**text;\fP
.mc
.fi
On return, \fItext\fP will have a null-terminated
octet sequence containing the matched segment.
.P
.H 4 "XmStringGetNextComponent"
.iX "XmStringGetNextComponent"
This function returns the
type and value of the next component in the compound string 
identified by the specified context.

.nf
.mc |
\fBXmStringComponentType XmStringGetNextComponent\fP \fI(context,
.mc
   text, charset, direction, unknown_tag, unknown_length, unknown_value)\fP
.mc |
    \fBXmStringContext\fP \fI*context;\fP
    \fBchar\fP  \fI**text;\fP
    \fBXmStringCharSet\fP \fI*charset;\fP
    \fBXmStringDirection\fP \fI*direction;\fP
    \fBXmStringComponentType\fP \fI*unknown_tag;\fP
    \fBshort\fP  \fI*unknown_length\fP
    \fBchar\fP  \fI**unknown_value;\fP
.mc
.fi
.P
This is a low-level component fetch function.  
Components are returned one
at a time.  Only some output parameters will be valid on return, and
this can be determined by examining the return status.  In the
.mc |
case of \fItext\fP, \fIcharset\fP, or \fIdirection\fP components, only one output
.mc
parameter is valid.  If the return status indicates that an
.mc |
unknown component was encountered, the \fItag\fP, \fIlength\fP and \fIvalue\fP are
.mc
returned.  This function will allocate the space necessary to
hold returned values; freeing this space is the caller's responsibility.
.H 4 "XmStringGetNextSegment"
.iX "XmStringGetNextSegment"
This function returns the bytes in the next segment of the specified
compound string.

.nf
.mc |
\fBBoolean XmStringGetNextSegment\fP \fI(context, text, charset, direction,
.mc
   separator)\fP
.mc |
    \fBXmStringContext\fP \fI*context;\fP
    \fBchar\fP  \fI**text;\fP
    \fBXmStringCharSet\fP \fI*charset;\fP
    \fBXmStringDirection\fP \fI*direction;\fP
    \fBBoolean\fP  \fI*separator;\fP
.mc
.fi
.P
.mc |
The \fItext\fP, \fIcharset\fP, and \fIdirection\fP of
.mc
the fetched segment are returned.  The separator parameter
.mc |
indicates whether the next component of the
compound string is a \fIseparator\fP.
True or False is returned to indicate whether a valid
.mc
segment was successfully parsed.
.H 4 "XmStringHeight"
.iX "XmStringHeight"
This function returns the height in pixels of the sum of all the line heights 
of the given compound string.  

.nf
.mc |
\fBDimension XmStringHeight\fP \fI(font list, string)\fP
    \fBXmFontList\fP \fIfont list;\fP
    \fBXmString\fP \fIstring;\fP
.mc
.fi
.P
Separator components delimit lines.
.P
.H 4 "XmStringInitContext"
.iX "XmStringInitContext"
This
function establishes the context for a subsequent segment-by-segment
read of the specified compound string.
.mc |

.mc
.nf
.mc |
\fBBoolean XmStringInitContext\fP \fI(context, string)\fP
    \fBXmStringContext\fP \fI*context;\fP
    \fBXmString\fP \fIstring;\fP
.mc
.fi
.P
In order to allow applications to read the contents of a compound
.mc |
string segment-by-segment some \fIcontext\fP needs to be maintained.  This
.mc
function establishes the context for such a  read.  A True or False value
.mc |
is returned to indicate whether the input string could be parsed.
.mc
.P
.H 4 "XmStringLength"
.iX "XmStringLength"
This function obtains the length of a compound string.
.mc |

.mc
.nf
.mc |
\fBint XmStringLength\fP \fI(s1)\fP
    \fBXmString\fP \fIs1;\fP
.mc
.fi
.P
.mc |
This function returns the number of bytes in \fIs1\fP, including the string
.mc
header (\fB0x7f\fP) and all
tags, direction indicators, and separators.  Zero is returned
if the compound string has an invalid structure.
.H 4 "XmStringLineCount"
.iX "XmStringLineCount"
This function returns the number of lines of text in the specified
compound string.
.mc |

.mc
.nf
.mc |
\fBint XmStringLineCount\fP \fI(string)\fP
    \fBXmString\fP \fIstring;\fP
.mc
.fi
.P
.H 4 "XmStringNConcat"
.iX "XmStringNConcat"
This function appends a specified number of bytes from one compound string
to another.

.nf
.mc |
\fBXmString XmStringNConcat\fP \fI(s1, s2, num_bytes)\fP
    \fBXmString\fP \fIs1, s2;\fP
    \fBint\fP \fInum_bytes;\fP
.mc
.fi

\fBXmStringNConcat\fP appends \fInum_bytes\fP bytes
from \fIs2\fP to \fIs1\fP, including tags, directional indicators, and
separators.  It then returns the resulting compound string.  If \fInum_bytes\fP
is less than the length of \fIs2\fP, the resulting string will not be a valid
compound string \(EM the original strings are preserved.  
The space for the resulting compound string is allocated within the
function.  The application is responsible for managing 
the allocated space.  The memory can be recovered 
by calling  \fBXtFree\fP.
.H 4 "XmStringNCopy"
.iX "XmStringNCopy"
This function copies a specified portion of a given compound string.
.nf
.P
.mc |
\fBXmStringNCopy\fP \fI(s1, num_bytes)\fP
  \fBXmString\fP \fIs1;\fP
  \fBint\fP \fInum_bytes;\fP
.mc
.fi
.P
This function creates a copy of \fIs1\fP which contains \fInum_bytes\fP bytes
from \fIs1\fP, including tags, directional indicators, and
separators.  It then returns the resulting copy.  If \fInum_bytes\fP
.mc |
is less than the length of \fIs1\fP, the resulting string will not be a valid
.mc
compound string and the original string is preserved.  For this reason,
you should normally use
\fBXmStringCopy\fP.  
The space for the resulting compound string is allocated within the
function.  The application is responsible for managing the 
allocated space.  The memory can be recovered 
by calling  \fBXtFree\fP.
.H 4 "XmStringPeekNextComponent" 
.iX "XmStringPeekNextComponent"
This function examines the next component that would be fetched 
by \fBXmStringGetNextComponent\fP and returns the component type.

.nf
.mc |
\fBXmStringComponentType XmStringPeekNextComponent\fP \fI(context)\fP
    \fBXmStringContext\fP \fI*context;\fP
.mc
.fi
.P
.H 4 "XmStringSegmentCreate"
.iX "XmStringSegmentCreate"
This is a high-level function that assembles a compound string
.mc |
consisting of a character-set identifier, a direction component,
.mc
a text component, and, optionally, a separator component.  
If \fIseparator\fP is False, then the compound string does not have a
separator component at the end.  If it is True, the
compound string has a separator component immediately
following the text component.
.mc |

.mc
.nf
.mc |
\fBXmString XmStringSegmentCreate\fP \fI(text, charset, direction, separator)\fP
     \fBchar\fP  \fI*text;\fP
     \fBXmStringCharSet\fP \fIcharset;\fP
     \fBXmStringDirection\fP \fIdirection;\fP
     \fBBoolean\fP  \fIseparator;\fP
.mc

.mc *
.mc
.fi
.mc |
.VL 1i
.LI\fItext\fP
Specifies a pointer to a null-terminated string.
.LI\fIcharset\fP
Specifies the character-set identifier to be associated with
the text. This can be XmSTRING_DEFAULT_CHARSET.
.LI\fIdirection\fP
Specifies the direction of the text.
.LI\fIseparator\fP
Specifies if a separator should be added to the compound 
string segment being constructed. 
.LE
.mc
.P
.H 4 "XmStringSeparatorCreate"
.iX "XmStringSeparatorCreate"
This function creates a compound string with a single component, 
.mc |
a \fIseparator\fP.
.mc
.nf

.mc |
\fBXmString XmStringSeparatorCreate\fP \fI(separator)\fP
     \fBBoolean\fP \fIseparator;\fP
.mc
.fi
.P
.H 4 "XmStringWidth"
.iX "XmStringWidth"
This function returns the width in pixels of the longest 
sequence of text components in the provided compound string.  Separator
components are used to delimit sequences of text components.
.nf
.P
.mc |
\fBDimension XmStringWidth\fP \fI(font list, string)\fP
    \fBXmFontList\fP \fIfont list;\fP
    \fBXmString\fP \fIstring;\fP
.mc
.fi
.H 3 "A Sample Program"
.mc |
This section contains a listing and defaults file
for a sample program that illustrates how you can use the compound
.mc
string functions in applications.  This program creates a PushButton,
.mc |
the Label for which is Hello World.  Each word in the
Label appears in a different font as specified in the applications-defaults file
.mc
for the program.
.H 4 "Program Listing"
.mc |
.P
The program listing follows.
.P
.mc
.ft CW
.nf
.mc |
.in0
.S -2
.mc
#include <X11/Xlib.h>
#include <Xt/Intrinsic.h>
#include <Xt/Shell.h>
#include <Xm/Xm.h>
#include <Xm/PushB.h>

Widget toplevel,pbutton;
Arg myArgs[10];

void main(argc, argv)
     unsigned int argc;
     char **argv;
{
     int i;
     XmString s1,s2,string,ButtonText;
     char *word1="Hello ",*word2="World";
.mc |
     XtAppContext app_context;
.mc
     char *appclass = "LAB0";
     
.mc |
.iX "XtAppInitialize"
     toplevel = XtAppInitialize(&app_context, appclass, NULL, 0,
                                  &argc, argv, NULL, NULL, 0);
.mc
.iX "XmStringSegmentCreate"
.mc |
     s1=XmStringSegmentCreate(word1,"chset1",XmSTRING_DIRECTION_L_TO_R,
         False);
     s2=XmStringSegmentCreate(word2,"chset2",XmSTRING_DIRECTION_L_TO_R,
         True);
.mc
     string=XmStringConcat(s1,s2);

     i=0;
.iX "XtSetArg"
     XtSetArg(myArgs[i], XmNlabelString, string); i++;
.iX "XmCreatePushButton"
     pbutton = XmCreatePushButton(toplevel,"x01",myArgs,i);
.iX "XtManageChild"
     XtManageChild(pbutton);
.iX "XtRealizeWidget"
     XtRealizeWidget(toplevel);
.mc |
.iX "XtAppMainLoop"
     XtAppMainLoop(app_context);
.mc
}
.mc |

.in
.S
.ft P
.mc
.P
.H 4 "Defaults File"
.mc |
.P
The applications-default file follows.
.P
.mc
.ft CW
.nf
.mc |
.in0
.S -2
.mc
!
!   Apps default file LAB0 for compound string function sample program
.mc *
.mc
*foreground: Yellow
*background: SlateBlue
*FontList: hp8.8x16b=chset1, hp8.8x16=chset2 
.ft P
.mc |
.in
.S
.mc
.fi
.P
.mc |
Note that font lists can be specified in a defaults file by setting the
font list resource to a string of the form \fIfontname\fP = \fIcharacter set\fP.
The widget will build a font list consisting of the specified font and
.mc
character set.  If the character set is omitted, the character set will
default to \fBXmSTRING_DEFAULT_CHARSET\fP. 
.P
Strings can be specified by setting the string resource to the desired
text.  For example, \fBoklabel.labelString:  OK\fP sets a label's text
.mc |
to OK.  Specifying a string in a defaults file is the same as 
.mc
creating the string using \fBXmStringCreateLtoR\fP with the default
character set \fBXmSTRING_DEFAULT_CHARSET\fP.
.H 2 " Cut and Paste Functions"
.iX "Functions" "cut and paste"
.iX "Cut and paste functions"
.iX "Clipboard"
The Motif clipboard is used to hold data that is to be transferred
between applications.  Motif provides the functions necessary to
modify the type and value of the data that is to be transferred via the
.mc |
clipboard.  These functions are known as cut and paste functions.
.mc
An application can interface to the Motif clipboard through
calls to the cut and paste functions.
.P
.mc |
The table below gives a brief description of each function.  Detailed
.mc
information
on each function is presented in the sections that follow.
.mc |
.P
.ne 5i
.mc
.iX "XmClipboardStartCopy" 
.iX "XmClipboardCancelCopy"
.iX "XmClipboardUndoCopy"
.iX "XmClipboardCopy"
.iX "XmClipboardCopyByName"
.iX "XmClipboardEndCopy"
.iX "XmClipboardInquireCount"
.iX "XmClipboardInquireFormat"
.iX "XmClipboardInquireLength"
.iX "XmClipboardInquirePendingItems"
.iX "XmClipboardStartRetrieve"
.iX "XmClipboardRetrieve"
.iX "XmClipboardEndRetrieve"
.iX "XmClipboardUnlock"
.iX "XmClipboardLock"
.iX "XmClipboardRegisterFormat"
.iX "XmClipboardWithdrawFormat" 
.mc *
.mc
.TB "Cut and Paste Functions"
.mc |
.in 0
.mc
.TS
box, tab(;);
lb | lb
lb | l.
.mc
.sp .2
Function Name;Description
.sp .2
_
.sp .2
XmClipboardStartCopy;T{
.na
Set up storage and data structures for clipboard copying.
.ad
T}
.mc |
XmClipboardCopy;Copies a data item to the clipboard.
XmClipboardCopyByName;Copies a data item passed by name.
XmClipboardCancelCopy;Cancels a copy to clipboard.
XmClipboardUndoCopy;Deletes the last item placed on the clipboard.
.mc
XmClipboardEndCopy;Ends copy to clipboard.
.sp .2
_
.sp .2
.mc |
XmClipboardInquireCount;Returns the number of data item formats.
XmClipboardInquireFormat;Returns a specified format name.
XmClipboardInquireLength;Returns the length of the stored data.
XmClipboardInquirePendingItems;Returns a list of data id/private id pairs.
.mc
.sp .2
_
.sp .2
.mc |
XmClipboardStartRetrieve;Starts copy from clipboard.
XmClipboardRetrieve;Retrieves a data item from the clipboard.
.mc
XmClipboardEndRetrieve;Ends copy from clipboard.
.sp .2
_
.sp .2
.mc |
XmClipboardLock;Locks the clipboard.
.mc
XmClipboardRegisterFormat;Registers a new format. 
.mc |
XmClipboardUnlock;Unlocks the clipboard.
.mc
XmClipboardWithdrawFormat;T{
.na
Indicates the application no longer wants to supply a data item.
.ad
T}
.sp .2
.TE
.mc |
.in
.mc
.H 3 "Clipboard Copy Functions"
.mc |
The following sections describe clipboard copy functions.
.mc
.H 4 "XmClipboardStartCopy"
.iX "XmClipboardStartCopy"
This function sets up storage and data structures to receive clipboard data.
.P
.mc |
\fBint XmClipboardStartCopy\fP\^(\^\fIdisplay\fP, \fIwindow\fP, \fIclip_label\fP, \fItimestamp\fP,
.mc
.br
.mc |
                                \fIwidget\fP, \fIcallback\fP, \fIitem_id\fP\^)
.mc
.br
.mc |
     \fBDisplay *\^\fIdisplay\fP\^;
.mc
.br
.mc |
     \fBWindow \fIwindow\fP\^;
.mc
.br
.mc |
     \fBXmString \fIclip_label\fP\^;
.mc
.br
.mc |
     \fBTime \fItimestamp\fP;
.mc
.br
.mc |
     \fBWidget \fIwidget\fP\^;
.mc
.br
.mc |
     \fBVoidProc \fIcallback\fP\^;
.br
     \fBlong *\^\fIitem_id\fP\^;
.mc
.VL 1i
.LI \fIdisplay\fR
Specifies a pointer to the
\fBDisplay\fP
structure that was returned in a previous call to
.mc |
\fBXtOpenDisplay\fP.
.mc
.LI \fIwindow\fP
Specifies the window ID that
relates the application window to the clipboard.
The same application instance should pass the same window ID to each of the
clipboard functions that it calls.
.mc |
Note that this window must be associated with a widget.
.mc
.LI \fIclip_label\fP
Specifies the label to be associated with the data item.
This argument
is used to identify the data item, for example, in a clipboard viewer.
An example of a label is the name of the application that places the
data in the clipboard.
.LI \fItimestamp\fP
The time of the event that triggered the copy.
.LI \fIwidget\fP
Specifies the ID of the widget that will receive messages requesting data
previously passed by name.
This argument must be present in order to pass data by name.
Any valid widget ID in your application can be used for this purpose and
all the message handling is taken care of by the cut and paste
functions.
.mc |
.br
.ne 7
.mc
.LI \fIcallback\fP
Specifies the address of the callback function that is 
called when the clipboard
needs data that was originally passed by name.
This is also the callback to receive the DELETE message for items that were
originally passed by name.
This argument must be present in order to pass data by name.
.LI \fIitem_id\fP
Specifies the number assigned to this data item.
The application uses this number in calls to
\fBXmClipboardCopy\fP,
\fBXmClipboardEndCopy\fP,
and
\fBXmClipboardCancelCopy\fP.
.LE
.P
The
\fBXmClipboardStartCopy\fP
function
sets up storage and data structures to receive clipboard data.
An application calls
\fBXmClipboardStartCopy\fP
during a cut or copy operation.
The data item that these structures receive then becomes 
.mc |
the next paste item
.mc
in the clipboard.
.P
Copying a large piece of data to the clipboard can take time.
It is possible that, once copied, no application will ever request that
data.
Motif provides a mechanism so that an 
application does not need to
actually pass data to the clipboard until the data has been requested by
some application.
Instead, the application passes format and length information in 
\fBXmClipboardCopy\fP to the
clipboard functions, along with a widget ID and a callback function
.mc |
address that is passed in \fBXmClipboardStartCopy\fP.
.mc
The widget ID is needed for communications between the clipboard
functions in the application that owns the data and the clipboard
functions in the application that requests the data.
Your callback functions are responsible for copying the actual data to
the clipboard (via
\fBXmClipboardCopyByName\fP).
The callback function is also called if the data item is removed from
the clipboard, and the actual data is therefore no longer needed.
.P
For more information on passing data by name, see
.mc |
\fBXmClipboardCopy\fP
.mc
and
\fBXmClipboardCopyByName\fP.
.P
The \fIwidget\fP and \fI callback\fP arguments must be present in order to
pass data by name. The callback format is as follows:
.P
.mc |
\fBfunction name\^(\^\fIwidget\fP, \fIdata_id\fP, \fIprivate\fP, \fIreason\fP\^) 
.mc
.br
.mc |
     \fBWidget *\^\fIwidget\fP\^;
.mc
.br
.mc |
     \fBint *\^\fIdata_id\fP\^;
.mc
.br
.mc |
     \fBint *\^\fIprivate\fP\^;
.mc
.br
.mc |
     \fBint *\^\fIreason\fP\^;
.mc
.VL 1i
.LI \fIwidget\fR
Specifies the ID of the widget passed to
.mc |
\fBXmClipboardStartCopy\fP
.br
.ne 8
.mc
.LI \fIdata_id\fP
Specifies the identifying number returned by
\fBXmClipboardCopy\fP,
which identifies the pass-by-name data.
.LI \fIprivate\fP
Specifies the private information passed to
\fBXmClipboardCopy\fP.
.LI \fIreason\fP
.mc |
Specifies the reason, either
\fBXmCR_CLIPBOARD_DATA_DELETE\fP
.mc
or
.mc |
\fBXmCR_CLIPBOARD_DATA_REQUEST\fP.
.mc
.LE
.P
This function can return one of the following status return constants: 
.mc |
.P
.VL 1.5i
.LI "\fBClipboardSuccess\fP"
.mc
The function is successful.
.mc |
.LI "\fBClipboardLocked\fP"
.mc
The function failed because the clipboard was locked by another application.
The application can continue to call the function again with the same
parameters until the lock goes away.
This gives the application the opportunity 
to ask if the user wants to keep trying
or to give up on the operation. 
.mc |
.LE
.mc
.P
.mc *
.mc
.H 4 "XmClipboardCopy"
.iX "XmClipboardCopy"
This function copies a data item to temporary clipboard storage.
.P
.mc |
\fBint XmClipboardCopy\^(\^\fIdisplay\fP, \fIwindow\fP, \fIitem_id\fP, \fIformat_name\fP,
.mc
.br
.mc |
                      \fIbuffer\fP, \fIlength\fP, \fIprivate_id\fP, \fIdata_id\fP\^)
.mc
.br
.mc |
     \fBDisplay *\^\fIdisplay\fP\^;
.mc
.br
.mc |
     \fBWindow \fIwindow\fP\^;
.mc
.br
.mc |
     \fBlong \fIitem_id\fP\^;
.mc
.br
.mc |
     \fBchar *\^\fIformat_name\fP\^;
.mc
.br
.mc |
     \fBchar *\^\fIbuffer\fP\^;
.mc
.br
.mc |
     \fBunsigned long \fIlength\fP\^;
.mc
.br
.mc |
     \fBint \^\fIprivate_id\fP\^;
.br
     \fBint *\^\fIdata_id\fP\^;
.VL 2i
.mc
.LI \fIdisplay\fR
Specifies a pointer to the
\fBDisplay\fP
structure that was returned in a previous call to
.mc |
\fBXtOpenDisplay\fP.
.mc
.LI \fIwindow\fP
Specifies the window ID that
relates the application window to the clipboard.
The same application instance should pass the same window ID to each of the
clipboard functions that it calls.
.mc |
Note that this window must be associated with a widget.
.mc
.LI \fIitem_id\fP
Specifies the number assigned to this data item.
This number was returned by a previous call to
\fBXmClipboardStartCopy\fP.
.LI \fIformat_name\fP
Specifies the name of the format in which the data item is stored on the
.mc |
clipboard.  Format is referred to as target in the \fIInter-Client
Communication Conventions Manual\fP.
.mc
.LI \fIbuffer\fP
Specifies the buffer from which the clipboard copies the data.
.LI \fIlength\fP
Specifies the length of the data being copied to the clipboard.
.LI \fIprivate_id\fP
Specifies the private data that the application wants 
to store with the data
item.
.LI \fIdata_id\fP
Specifies an identifying number assigned to the data item that uniquely
identifies the data item and the format.
This argument is required only for data that is passed by name.
.LE
.P
The
\fBXmClipboardCopy\fP
function
copies a data item to temporary clipboard storage.
The data item is moved from temporary storage to the clipboard data
structure when a 
call to
\fBXmClipboardEndCopy\fP is made.
Additional calls to
\fBXmClipboardCopy\fP
before a call to
\fBXmClipboardEndCopy\fP
.mc |
add more data item formats to the same data item or 
.mc
append data to an existing format.
.P
If the \fI buffer\fP argument is NULL, the data is considered passed by name.
If data passed by name is later needed by another application, the
application that owns the data receives a callback with a request for
the data.  The application that owns the data must then transfer the
data to the clipboard with the \fBXmClipboardCopyByName\fP function.
When a data item that was passed by name is deleted 
from the clipboard, the application that owns the data receives a
callback that states that the data is no longer needed.
.P
For information on the callback function, see the callback argument
description for
\fBXmClipboardStartCopy\fP.
.P
This function can return one of the following status return constants: 
.mc |
.P
.VL 1.5i
.LI "\fBClipboardSuccess\fP"
.mc
The function is successful.
.mc |
.LI "\fBClipboardLocked\fP
.mc
The function failed because the clipboard was locked by another application.
The application can continue to call the function again with the same
parameters until the lock goes away.
This gives the application the opportunity 
to ask if the user wants to keep trying
.mc |
or to give up on the operation.
.LE
.mc
.P
.H 4 "XmClipboardCopybyName"
.iX "XmClipboardCopyByName"
This function copies a data item to the clipboard.
.P
.mc |
\fBint XmClipboardCopyByName\^(\^\fIdisplay\fP, \fIwindow\fP, \fIdata_id\fP, \fIbuffer\fP,
.mc
.br
.mc |
                                       \fIlength\fP, \fIprivate_id\fP\^)
.mc
.br
.mc |
     \fBDisplay *\^\fIdisplay\fP\^;
.mc
.br
.mc |
     \fBWindow \fIwindow\fP\^;
.mc
.br
.mc |
     \fBint \fIdata_id\fP\^;
.mc
.br
.mc |
     \fBchar *\^\fIbuffer\fP\^;
.mc
.br
.mc |
     \fBunsigned long \fIlength\fP\^;
.br
     \fBint \fIprivate_id\fP\^;
.mc
.VL 1i
.mc |
.LI\fIdisplay\fR
.mc
Specifies a pointer to the
\fBDisplay\fP
structure that was returned in a previous call to
.mc |
\fBXtOpenDisplay\fP.
.LI\fIwindow\fR
.mc
Specifies the window ID that
relates the application window to the clipboard.
The same application instance should pass the same window ID to each of the
clipboard functions that it calls.
.mc |
Note that this window must be associated with a widget.
.LI\fIdata_id\fP
.mc
Specifies an identifying number assigned to the data item that uniquely
identifies the data item and the format.
This number was assigned by
\fBXmClipboardCopy\fP
to the data item.
.mc |
.LI\fIbuffer\fP
.mc
Specifies the buffer from which the clipboard copies the data.
.mc |
.LI\fIlength\fP
.mc
Specifies the number of bytes in the data item.
.mc |
.LI\fIprivate_id\fP
.mc
Specifies the private data that the application wants 
to store with the data
item.
.LE
.P
The
\fBXmClipboardCopyByName\fP
function
copies the actual data for a data item that was previously 
passed by name to the clipboard.  Data is considered to be passed by name
when a call to \fBXmClipboardCopy\fP is made with the \fIbuffer\fP
parameter  is set to NULL.
Additional calls to
\fBXmClipboardCopyByName\fP
append new data to the existing data.  The clipboard should be locked
.mc |
before making such calls by using \fPXmClipboardLock\fP to ensure the integrity of the clipboard
.mc
data.
.P
This function can return one of the following status return constants: 
.mc |
.P
.VL 1.5i
.LI "\fBClipboardSuccess\fP"
.mc
The function is successful.
.mc |
.LI "\fBClipboardLocked\fP"
.mc
The function failed because the clipboard was locked by another application.
The application can continue to call the function again with the same
parameters until the lock goes away.
This gives the application the opportunity 
to ask if the user wants to keep trying
.mc |
or to give up on the operation.
.LE
.mc
.P
.H 4 "XmClipboardCancelCopy"
.iX "XmClipboardCancelCopy"
This function cancels any copy to the clipboard that is in progress and
frees any temporary storage in use.
.P
.mc |
\fBvoid XmClipboardCancelCopy\^(\^\fIdisplay\fP, \fIwindow\fP, \fIitem_id\fP\^) 
.mc
.br
.mc |
     \fBDisplay *\^\fIdisplay\fP\^;
.mc
.br
.mc |
     \fBWindow \fIwindow\fP\^;
.mc
.br
.mc |
     \fBlong \fIitem_id\fP\^;
.mc
.VL 1i
.LI \fIdisplay\fR
Specifies a pointer to the
\fBDisplay\fP
structure that was returned in a previous call to
.mc |
\fBXtOpenDisplay\fP.
.mc
.LI \fIwindow\fP
Specifies the window ID that
relates the application window to the clipboard.
The same application instance should pass the same window ID to each of the
clipboard functions that it calls.
.mc |
Note that this window must be associated with a widget.
.mc
.LI \fIitem_id\fP
Specifies the number assigned to this data item.
This number was returned by a previous call to
\fBXmClipboardStartCopy\fP.
.LE
.P
The
\fBXmClipboardCancelCopy\fP
function
cancels any copy-to-clipboard
that is in progress and frees any temporary storage in use.  When a copy is
performed, \fBXmClipboardStartCopy\fP allocates temporary storage for the
clipboard data.  \fBXmClipboardCopy\fP places the data in the temporary
storage.  \fBXmClipboardEndCopy\fP copies the data to the clipboard data
structure and frees the temporary data storage. 

\fBXmClipboardCancelCopy\fP
also frees up temporary storage.
If
\fBXmClipboardCancelCopy\fP
is called, then
\fBXmClipboardEndCopy\fP
does not have to be called.
A call to
\fBXmClipboardCancelCopy\fP
is valid only after a call to
\fBXmClipboardStartCopy\fP
and before a call to
\fBXmClipboardEndCopy\fP.
.P
.H 4 "XmClipboardUndoCopy"
.iX "XmClipboardUndoCopy"
This function deletes the last item placed on the clipboard.
.P
.mc |
\fBint XmClipboardUndoCopy\^(\^\fIdisplay\fP, \fIwindow\fP\^) 
.mc
.br
.mc |
     \fBDisplay *\^\fIdisplay\fP\^;
.mc
.br
.mc |
     \fBWindow \fIwindow\fP\^;
.mc
.VL 1i
.LI \fIdisplay\fR
Specifies a pointer to the
\fBDisplay\fP
structure that was returned in a previous call to
.mc |
\fBXtOpenDisplay\fP.
.mc
.LI \fIwindow\fR
Specifies the window ID that
relates the application window to the clipboard.
The same application instance should pass the same window ID to each of the
clipboard functions that it calls.
.mc |
Note that this window must be associated with a widget.
.mc
.LE
.P
The
\fBXmClipboardUndoCopy\fP
function
deletes the last item placed on the clipboard
if the item was placed there by an
application with the passed 
\fIdisplay\fP
and
\fIwindow\fP
arguments.
Any data item deleted from the clipboard by the original call to
\fBXmClipboardCopy\fP
is restored.
If the
\fIdisplay\fP
or
\fIwindow\fP
IDs do not match the last copied item, no action is taken and this
function has no effect.
.P
This function can return one of the following status return constants: 
.mc |
.P
.VL 1.5i
.LI "\fBClipboard Success\fP"
.mc
The function is successful.
.mc |
.LI "\fBClipboardLocked\fP"
.mc
The function failed because the clipboard was locked by another application.
The application can continue to call the function again with the same
parameters until the lock goes away.
This gives the application the opportunity 
to ask if the user wants to keep trying
.mc |
or to give up on the operation.
.LE
.mc
.P
.H 4 "XmClipboardEndCopy"
.iX "XmClipboardEndCopy"
This function has several uses: to lock the clipboard, to place data in the
clipboard data structure, and to unlock the clipboard.
.P
.mc |
\fBint XmClipboardEndCopy\^(\^\fIdisplay\fP, \fIwindow\fP, \fIitem_id\fP\^) 
.mc
.br
.mc |
     \fBDisplay *\^\fIdisplay\fP\^;
.mc
.br
.mc |
     \fBWindow \fIwindow\fP\^;
.mc
.br
.mc |
     \fBlong \fIitem_id\fP\^;
.mc
.VL 1i
.LI \fIdisplay\fR
Specifies a pointer to the
\fBDisplay\fP
structure that was returned in a previous call to
.mc |
\fBXtOpenDisplay\fP.
.mc
.LI \fIwindow\fR
Specifies the window ID that
relates the application window to the clipboard.
The same application instance should pass the same window ID to each of the
clipboard functions that it calls.
.mc |
Note that this window must be associated with a widget.
.mc
.LI \fIitem_id\fP
Specifies the number assigned to this data item.
This number was returned by a previous call to
\fBXmClipboardStartCopy\fP.
.LE
.P
The
\fBXmClipboardEndCopy\fP
function
locks the clipboard from access by other applications, places data in the
clipboard data structure, and unlocks the clipboard.
Data items copied to the clipboard by
\fBXmClipboardCopy\fP
are not actually entered in the clipboard data structure until the call to
\fBXmClipboardEndCopy\fP.  It also frees the temporary storage that was
allocated by \fBXmClipboardStartCopy\fP.
.P
This function can return one of the following status return constants: 
.mc |
.P
.VL 1.5i
.LI "\fBClipboardSuccess\fP"
.mc
The function is successful.
.mc |
.LI "\fBClipboardLocked\fP"
.mc
The function failed because the clipboard was locked by another application.
The application can continue to call the function again with the same
parameters until the lock goes away.
This gives the application the opportunity 
to ask if the user wants to keep trying
.mc |
or to give up on the operation.
.LE
.mc
.H 3 "Clipboard Inquire Functions"
.mc |
.P
This section describes the Clipboard Inquire functions, which
obtain information about data in the Clipboard.
.mc
.H 4 "XmClipboardInquireCount"
.iX "XmClipboardInquireCount"
This function returns the maximum length for all data item formats.
.P
.mc |
\fBint XmClipboardInquireCount (\fIdisplay\fP, \fIwindow\fP, \fIcount\fP,
.mc
.br
.mc |
     \fImax_format_name_length\fP)
.mc
.br
.mc |
     \fBDisplay *\fIdisplay\fP;
.mc
.br
.mc |
     \fBWindow \fIwindow\fP;
.mc
.br
.mc |
     \fBint *\fIcount\fP;
.br
     \fBint *\fImax_format_name_length\fP;
.mc
.VL 2i
.LI \fIdisplay\fR
Specifies a pointer to the
\fBDisplay\fP
structure that was returned in a previous call to
.mc |
\fBXtOpenDisplay\fP.
.mc
.LI \fIwindow\fP
Specifies the window ID that
relates the application window to the clipboard.
The same application instance should pass the same window ID to each of the
clipboard functions that it calls.
.mc |
Note that this window must be associated with a widget.
.mc
.LI \fIcount\fP
Returns the number of data item formats available for 
.mc |
the next paste item in the clipboard.
.mc
If no formats are available, this argument equals zero.
The count includes the formats that were passed by name.
.LI \fImax_format_name_length\fP
.mc |
Specifies the maximum length of all format names for the next paste
.mc
item in the clipboard.
.LE
.P
The \fBXmClipboardInquireCount\fP function
returns the number of data item formats available for 
.mc |
the next paste item in the clipboard.
.mc
This function also returns the maximum name length for all formats 
in which the
.mc |
next paste item is stored.
.mc
.P
This function can return one of the following status return constants: 
.mc |
.P
.VL 1.5i
.LI "\fBClipboardSuccess\fP"
.mc
The function is successful.
.mc |
.LI "\fBClipboardLocked\fP"
.mc
The function failed because the clipboard was locked by another application.
The application can continue to call the function again with the same
parameters until the lock goes away.
This gives the application the opportunity 
to ask if the user wants to keep trying
.mc |
or to give up on the operation.
.LI "\fBClipBoardNoData\fP"
.mc
The function could not find data on the clipboard that corresponds to
the format requested.  This could occur because the clipboard is empty,
there is no data on the clipboard in the format requested, or the
data requested was passed by name and is no longer available.
.mc |
.LE
.mc
.P
.H 4 "XmClipboardInquireFormat"
.iX "XmClipboardInquireFormat"
This function obtains the format name for the next paste data
item in the clipboard.
.P
.mc |
\fBint XmClipboardInquireFormat(\fIdisplay\fP, \fIwindow\fP, \fIindex\fP, \fIformat_name_buf\fP,
.mc
.br
.mc |
                                       \fIbuffer_len\fP, \fIcopied_len\fP)
.mc
.br
.mc |
     \fBDisplay *\fIdisplay\fP;
.mc
.br
.mc |
     \fBWindow \fIwindow\fP;
.mc
.br
.mc |
     \fBint \fIindex\fP;
.mc
.br
.mc |
     \fBchar *\fIformat_name_buf\fP;
.mc
.br
.mc |
     \fBunsigned long \fIbuffer_len\fP;
.br
     \fBunsigned long *\fIcopied_len\fP;
.VL 2i
.mc
.LI \fIdisplay\fR
Specifies a pointer to the
\fBDisplay\fP
structure that was returned in a previous call to
.mc |
\fBXtOpenDisplay\fP.
.mc
.LI \fIwindow\fR
Specifies the window ID that
relates the application window to the clipboard.
The same application instance should pass the same window ID to each of the
clipboard functions that it calls.
.mc |
Note that this window must be associated with a widget.
.br
.ne 7
.mc
.LI \fIindex\fP
Specifies which of the ordered format names to be obtained.
If this index 
\fIi\fP
is greater than the number of formats for the data item,
\fBXmClipboardInquireFormat\fP
returns a zero in the
\fIcopied_len\fP
argument.
.LI \fIformat_name_buf\fP
Specifies the buffer that receives the format name.
.LI \fIbuffer_len\fP
Specifies the number of bytes in the format name buffer.
.LI \fIcopied_len\fP
Specifies the number of bytes in the string copied to the buffer.
If this argument equals zero, there is no 
\fInth\fP 
format 
.mc |
for the next paste item.
.mc
.LE
.P
The
\fBXmClipboardInquireFormat\fP
function
returns a specified format name for the 
.mc |
next paste item in the clipboard.
.mc
If the name must be truncated, the function returns a warning status.
This function can return one of the following status return constants: 
.mc |
.P
.VL 1.7i
.LI "\fBClipboardSuccess\fP"
.mc
The function is successful.
.mc |
.LI "\fBClipboardLocked\fP"
.mc
The function failed because the clipboard was locked by another application.
The application can continue to call the function again with the same
parameters until the lock goes away.
This gives the application the opportunity 
to ask if the user wants to keep trying
.mc |
or to give up on the operation.
.LI "\fBClipboardTruncate\fP"
.mc
The data returned is truncated because the user did not provide a buffer
that was large enough to hold the data.
.mc |
.LI "\fBClipBoardNoData\fP"
.mc
The function could not find data on the clipboard that corresponds to
the format requested.  This could occur because the clipboard is empty,
there is no data on the clipboard in the format requested, or the
data requested was passed by name and is no longer available.
.mc |
.LE
.mc
.P
.mc |
.ne 12
.mc
.H 4 "XmClipboardInquireLength"
.iX "XmClipboardInquireLength"
This function obtains the length of the data stored under a specified format.
.mc |

.nf
\fBint XmClipboardInquireLength\^(\^\fIdisplay\fP, \fIwindow\fP, 
.mc
.br
.mc |
     \fIformat_name\fP, \fIlength\fP\^)
.mc
.br
.mc |
     \fBDisplay *\^\fIdisplay\fP\^;
.mc
.br
.mc |
     \fBWindow \fIwindow\fP\^;
.mc
.br
.mc |
     \fBchar *\^\fIformat_name\fP\^;
.br
     \fBunsigned long *\^\fIlength\fP\^;
.fi
.mc
.VL 1i
.LI \fIdisplay\fR
Specifies a pointer to the
\fBDisplay\fP
structure that was returned in a previous call to
.mc |
\fBXtOpenDisplay\fP.
.mc
.LI \fIwindow\fP
Specifies the window ID that
relates the application window to the clipboard.
The same application instance should pass the same window ID to each of the
clipboard functions that it calls.
.mc |
Note that this window must be associated with a widget.
.mc
.LI \fIformat_name\fP
.mc |
Specifies the name of the format for the next paste item.
.mc
.LI \fIlength\fP
Specifies the length of the next data item in the specified format.
This argument equals zero if no data is found for the 
specified format, or if
there is no item on the clipboard.
.LE
.P
The
\fBXmClipboardInquireLength\fP
function
returns the length of the data stored under a specified format 
name for the 
clipboard data item.  This is accomplished by passing a pointer to
the length in the \fIlength\fP parameter in the function.
.P
If no data is found for the specified format, or if there is no item on the
clipboard,
\fBXmClipboardInquireLength\fP
returns a value of zero.
.P
This function can return one of the following status return constants: 
.mc |
.P
.VL 1.5i
.LI "\fBClipboardSuccess\fP"
.mc
The function is successful.
.mc |
.LI "\fBClipboardLocked\fP"
.mc
The function failed because the clipboard was locked by another application.
The application can continue to call the function again with the same
parameters until the lock goes away.
This gives the application the opportunity 
to ask if the user wants to keep trying
.mc |
or to give up on the operation.
.br
.ne 7
.LI "\fBClipBoardNoData\fP"
.mc
The function could not find data on the clipboard that corresponds to
the format requested.  This could occur because the clipboard is empty,
there is no data on the clipboard in the format requested, or the
data requested was passed by name and is no longer available.
.mc |
.LE
.mc
.P
.H 4 "XmClipboardInquirePendingItems"
.iX "XmClipboardInquirePendingItems"
.mc |
This function obtains a format name's list of data ID or private ID pairs.
.mc
.P
.mc |
\fBint XmClipboardInquirePendingItems\^(\^\fIdisplay\fP, \fIwindow\fP, \fIformat_name\fP,
.mc
.br
.mc |
                                             \fIitem_list\fP, \fIcount\fP\^)
.mc
.br
.mc |
     \fBDisplay *\^\fIdisplay\fP\^;
.mc
.br
.mc |
     \fBWindow \fIwindow\fP\^;
.mc
.br
.mc |
     \fBchar *\^\fIformat_name\fP\^;
.mc
.br
.mc |
     \fBXmClipboardPendingList *\^\fIitem_list\fP\^;
.br
     \fBunsigned long *\^\fIcount\fP\^;
.mc
.VL 1i
.LI \fIdisplay\fR
Specifies a pointer to the
\fBDisplay\fP
structure that was returned in a previous call to
.mc |
\fBXtOpenDisplay\fP.
.mc
.LI \fIwindow\fP
Specifies the window ID that
relates the application window to the clipboard.
The same application instance should pass the same window ID to each of the
clipboard functions that it calls.
.mc |
Note that this window must be associated with a widget.
.mc
.LI \fIformat_name\fP
Specifies a string that contains the name of the format for which the
.mc |
list of data ID/private ID pairs is to be obtained.
.mc
.LI \fIitem_list\fP
.mc |
Specifies the address of the array of data ID/private ID pairs 
.mc
for the specified
format name.
This argument is a type
\fBXmClipboardPendingList\fP.
The application is responsible for freeing the memory 
provided by this function
for storing the list.
.LI \fIitem_count\fP
Specifies the number of items returned in the list.
If there is no data for the specified format name, or if 
there is no item on the
clipboard, this argument equals zero. 
.LE
.P
The
\fBXmClipboardInquirePendingItems\fP
function
.mc |
returns a list of data ID/private ID pairs for a specified format name.
.mc
For the purposes of this function,
a data item is considered pending if the application originally passed 
it by name, the
.mc |
application has not yet copied the data, and the item
.ne 13
has not been deleted from
.mc
the clipboard.
.P
The application is responsible for freeing the memory 
provided by this function
to store the list.
.P
This function is used by an application when exiting
to determine if the
data that it passed by name should be sent to the clipboard.
.P
This function can return one of the following status return constants: 
.mc |
.P
.VL 1.5i
.LI "\fBClipboardSuccess\fP"
.mc
The function is successful.
.mc |
.LI "\fBClipboardLocked\fP"
.mc
The function failed because the clipboard was locked by another application.
The application can continue to call the function again with the same
parameters until the lock goes away.
This gives the application the opportunity 
to ask if the user wants to keep trying
or to give up on the operation. 
.mc |
.LE
.mc
.P
.H 3 "Clipboard Retrieve Functions"
.mc |
.P
This section describes the Clipboard Retrieve functions, which
you use to retrieve data from the Clipboard.
.mc
.H 4 "XmClipboardStartRetrieve"
.iX "XmClipboardStartRetrieve"
This function begins copying data incrementally from the clipboard. 
.P
.nf
.mc |
\fBint XmClipboardStartRetrieve \fI(display, window, timestamp);\fP
   \fBDisplay \fI*display;\fP
   \fBWindow   \fIwindow;\fP
   \fBTime     \fItimestamp;\fP
.mc
.fi
.VL 1i
.LI \fIdisplay\fR
Specifies a pointer to the
\fBDisplay\fP
structure that was returned in a previous call to
.mc |
\fBXtOpenDisplay\fP.
.mc
.LI \fIwindow\fP
Specifies the window ID that relates the application
window to the clipboard.  The same application instance
should pass the same window ID to each of the clipboard
functions that it calls.
.LI \fItimestamp\fP
The time of the event that triggered the copy.
.LE
.P
.mc |
.hy 14
.ne 12
.mc
This routine tells the cut and paste routines that the application is
ready to start copying an item from the clipboard.  The clipboard will be
locked by this routine and will stay locked until \fBXmClipboardEndRetrieve\fP
is called.  Between an \fBXmClipboardStartRetrieve\fP and an
\fBXmClipboardEndRetrieve\fP, 
multiple calls to \fBXmClipboardRetrieve\fP with the same
format name will result
in data being incrementally copied from the clipboard until the data in that
format has all been copied.  The return value \fBClipboardTruncate\fP
from calls to
\fBXmClipboardRetrieve\fP indicates that more data remains to be copied in the
given format.  It is recommended that any calls to the INQUIRE functions 
that the application needs to make to effect the copy from the clipboard 
be made between the call to \fBXmClipboardStartRetrieve\fP
and the first call to 
\fBXmClipboardRetrieve\fP.  That way, the application does not need to call 
\fBXmClipboardLock\fP and \fBXmClipboardUnlock\fP.  Applications do not need to use 
\fBXmClipboardStartRetrieve\fP and \fBXmClipboardEndRetrieve\fP, in which case 
\fBXmClipboardRetrieve\fP works as it did before.
.mc |
.hy 0
.mc
.P
.H 4 "XmClipboardRetrieve"
.iX "XmClipboardRetrieve"
This function obtains the current next paste data item from clipboard storage.
.P
.mc |
\fBint XmClipboardRetrieve\^(\^\fIdisplay\fP, \fIwindow\fP, \fIformat_name\fP, \fIbuffer\fP,
.mc
.br
.mc |
             \fIlength\fP, \fInum_bytes\fP, \fIprivate_id\fP\^) 
.mc
.br
.mc |
     \fBDisplay *\^\fIdisplay\fP\^;
.mc
.br
.mc |
     \fBWindow \fIwindow\fP\^;
.mc
.br
.mc |
     \fBchar *\^\fIformat_name\fP\^;
.mc
.br
.mc |
     \fBchar *\^\fIbuffer\fP\^;
.mc
.br
.mc |
     \fBunsigned long \fIlength\fP\^;
.mc
.br
.mc |
     \fBunsigned long *\^\fInum_bytes\fP\^;
.br
     \fBint *\^\fIprivate_id\fP\^;
.mc
.VL 1i
.LI \fIdisplay\fR
Specifies a pointer to the
\fBDisplay\fP
structure that was returned in a previous call to
.mc |
\fBXtOpenDisplay\fP.
.mc
.LI \fIwindow\fP
Specifies the window ID that
relates the application window to the clipboard.
The same application instance should pass the same window ID to each of the
clipboard functions that it calls.
.mc |
Note that this window must be associated with a widget.
.mc
.LI \fIformat_name\fP
Specifies the name of a format in which the data 
is stored on the clipboard.
.LI \fIbuffer\fP
Specifies the buffer to which the application wants the 
clipboard to copy the data.
.mc |
.br
.ne 8
.mc
.LI \fIlength\fP
Specifies the length of the application buffer.
.LI \fInum_bytes\fP
Specifies the number of bytes of data copied into the application
buffer.
.LI \fIprivate_id\fP
Specifies the private data stored with the data item by the application that
placed the data item on the clipboard.
If the application did not store private data with the data item, this argument
returns zero.
.LE
.P
The
\fBXmClipboardRetrieve\fP
function
retrieves the current data item from clipboard storage.
.mc *
.mc
\fBXmClipboardRetrieve\fP
returns a warning under the following circumstances:
.BL
.LI
The data needs to be truncated because the buffer length is too short.
.LI
The clipboard is locked.
.LI
There is no data on the clipboard.
.LE
.P
.mc *
.mc
This function can return one of the following status return constants: 
.mc |
.P
.VL 1.7i
.LI "\fBClipboardSuccess\fP"
.mc
The function is successful.
.mc |
.LI "\fBClipboardLocked\fP"
.mc
The function failed because the clipboard was locked by another application.
The application can continue to call the function again with the same
parameters until the lock goes away.
This gives the application the opportunity 
to ask if the user wants to keep trying
or to give up on the operation. 
.mc |
.LI "\fBClipboardTruncate\fP"
.mc
The data returned is truncated because the user did not provide a buffer
that was large enough to hold the data.
.mc |
.LI "\fBClipboardNoData\fP"
.mc
The function could not find data on the clipboard corresponding to the
format requested.
This could occur because (1) the clipboard is empty; (2) there is data
on the clipboard but not in the requested format; and (3) the data in
.mc |
the requested format was passed by name and is no longer available.
.LE
.mc
.P
.H 4 "XmClipboardEndRetrieve"
.iX "XmClipboardEndRetrieve"
This function suspends copying data incrementally from the clipboard.
.P
.nf
.mc |
\fBint XmClipboardEndRetrieve\fP \fI(display, window);\fP
   \fBDisplay\fP \fI*display;\fP
   \fBWindow\fP  \fI window;\fP
.mc
.fi
.P
.VL 1i
.mc |
.LI \fIdisplay\fR
.mc
Specifies a pointer to the
\fBDisplay\fP
structure that was returned in a previous call to
.mc |
\fBXtOpenDisplay\fP.
.br
.ne 5
.LI \fIwindow\fP
.mc
Specifies the window ID that relates the application
window to the clipboard.  The same application instance
should pass the same window ID to each of the clipboard
functions that it calls.
.LE
.P
\fBXmClipboardEndRetrieve\fP tells the cut and paste routines
the application is through copying an item to the clipboard.
Until \fBXmClipboardEndRetrieve\fP is called, data items can be retrieved
incrementally from the clipboard by calling \fBXmClipboardRetrieve\fP.
If the application calls \fBXmClipboardStartRetrieve\fP, it has to call
\fBXmClipboardEndRetrieve\fP.  If data is not being copied incrementally,
\fBXmClipboardStartRetrieve\fP and \fBXmClipboardEndRetrieve\fP do not need to
be called.
.P
.H 3 "Miscellaneous Clipboard Functions"
.mc |
.P
This section describes several additional clipboard functions.
.mc
.H 4 "XmClipboardLock"
.iX "XmClipboardLock"
This function locks the clipboard from access by other applications.
.P
.mc |
\fBint XmClipboardLock\^(\^\fIdisplay\fP, \fIwindow\fP\^) 
.mc
.br
.mc |
     \fBDisplay *\^\fIdisplay\fP\^;
.mc
.br
.mc |
     \fBWindow \fIwindow\fP\^;
.mc
.VL 1i
.mc |
.br
.ne 5
.mc
.LI \fIdisplay\fR
Specifies a pointer to the
\fBDisplay\fP
structure that was returned in a previous call to
.mc |
\fBXtOpenDisplay\fP.
.mc
.LI \fIwindow\fP
Specifies the window ID that
relates the application window to the clipboard.
The same application instance should pass the same window ID to each of the
clipboard functions that it calls.
.LE
.P
The
\fBXmClipboardLock\fP
function
locks the clipboard from access by another application until
you call
\fBXmClipboardUnlock\fP.
All clipboard functions lock and unlock the clipboard to prevent simultaneous
access.
The
\fBXmClipboardLock\fP
and
\fBXmClipboardUnlock\fP
functions
allow the application to keep the clipboard data from changing between calls to
the inquire functions and other clipboard functions.
The application does not
need to lock the clipboard between calls to
\fBXmClipboardStartCopy\fP
and
\fBXmClipboardEndCopy\fP, but it should do so before multiple calls to
\fBXmClipboardCopyByName\fP.
.P
If the clipboard is already locked by another application,
\fBXmClipboardLock\fP
returns an error status.
.P
Multiple calls to
\fBXmClipboardLock\fP
by the same application increase the lock level.
.P
This function can return one of the following status return constants: 
.mc |
.P
.VL 1.5i
.LI "\fBClipboardSuccess\fP"
.mc
The function is successful.
.mc |
.LI "\fBClipboardLocked\fP"
.mc
The function failed because the clipboard was locked by another application.
The application can continue to call the function again with the same
parameters until the lock goes away.
This gives the application the opportunity 
to ask if the user wants to keep trying
.mc |
or to give up on the operation.
.LE
.mc
.P
.H 4 "XmClipboardRegisterFormat"
.iX "XmClipboardRegisterFormat"
This function registers a new format.
.P
.nf
.mc |
\fBint ClipboardRegisterFormat\fP \fI(display, format_name, format_length)\fP
   \fBDisplay\fP      \fI*display;\fP
   \fBchar\fP         \fI*format_name;\fP
   \fBunsigned long\fP \fIformat_length;\fP
.mc
.fi
.P
.mc |
.VL 1.25i
.LI\fIdisplay\fR
.mc
Specifies a pointer to the
\fBDisplay\fP
structure that was returned in a previous call to
.mc |
\fBXtOpenDisplay\fP.
.mc
.LI\fIformat_name\fP
Specifies the string name for the new format.
.LI\fIformat_length\fP
Specifies the format length in bits (8, 16, or 32).
.LE
.P
Each format stored on the clipboard should have a length associated with
it and known to the cut and paste routines.  All of the formats specified
.mc |
by the \fIInter-Client Communication Conventions Manual\fP 
are pre-registered (formats are referred to as targets in the ICCCM).
.mc
Any other format that the
application wants to use must be registered via
this routine.  Failure to register the length of the data will result in
applications not being compatible across platforms having different byte
swapping orders.
.P
.H 4 "XmClipboardUnlock"
.iX "XmClipboardUnlock"
This function unlocks the clipboard.
.P
.mc |
\fBint XmClipboardUnlock\^(\^\fIdisplay\fP, \fIwindow\fP, \fIremove_all_locks\fP\^) 
.mc
.br
.mc |
     \fBDisplay *\^\fIdisplay\fP\^;
.mc
.br
.mc |
     \fBWindow \fIwindow\fP\^;
.mc
.br
.mc |
     \fBBoolean \fIremove_all_locks\fP\^;
.VL 1.5i
.LI\fIdisplay\fR
.mc
Specifies a pointer to the
\fBDisplay\fP
structure that was returned in a previous call to
.mc |
\fBXtOpenDisplay\fP.
.LI\fIwindow\fP
.mc
Specifies the window ID that
relates the application window to the clipboard.
The same application instance should pass the same window ID to each of the
clipboard functions that it calls.
.mc |
Note that this window must be associated with a widget.
.LI\fIremove_all_locks\fP
Specifies a Boolean value that, when
True,
.mc
indicates that
all nested locks should be removed.
If
.mc |
False,
.mc
indicates that only one level of lock should be removed.
.LE
.P
The \fBXmClipboardUnlock\fP function unlocks the clipboard, enabling it to
be accessed by other applications.
.P
.mc |
.ne 9
.mc
If multiple calls to \fBXmClipboardLock\fR have occurred, then the same
number of calls to \fBXmClipboardUnlock\fR is necessary to unlock the
.mc |
clipboard, unless the \fIremove_all_locks\fR argument is True.
.mc
.P
The application should lock the clipboard before making multiple calls
to \fBXmClipboardCopyByName\fP and should unlock the clipboard after
completion.
.P
This function can return one of the following status return constants: 
.mc |
.P
.VL 1.5i
.LI "\fBClipboardSuccess\fP"
.mc
The function is successful.
.mc |
.LI "\fBClipboardLocked\fP"
.mc
The function failed because the clipboard was locked by another application.
The application can continue to call the function again with the same
parameters until the lock goes away.
This gives the application the opportunity 
to ask if the user wants to keep trying
.mc |
or to give up on the operation.
.LE
.mc
.P
.H 4 "XmClipboardWithdrawFormat
.iX "XmClipboardWithdrawFormat"
This function indicates that the application is no longer willing
to supply a data
item to the clipboard.
.P
.mc |
\fBint XmClipboardWithdrawFormat\^(\^\fIdisplay\fP, \fIwindow\fP, \fIdata_id\fP\^) 
.mc
.br
.mc |
     \fBDisplay *\^\fIdisplay\fP\^;
.mc
.br
.mc |
     \fBWindow \fIwindow\fP\^;
.mc
.br
.mc |
     \fBint \fIdata_id\fP\^;
.mc
.VL 1i
.LI \fIdisplay\fR
Specifies a pointer to the
\fBDisplay\fP
structure that was returned in a previous call to
.mc |
\fBXtOpenDisplay\fP.
.mc
.LI \fIwindow\fP
Specifies the window ID that
relates the application window to the clipboard.
The same application instance should pass the same window ID to each of the
clipboard functions that it calls.
.mc |
Note that this window must be associated with a widget.
.mc
.LI \fIdata_id\fP
Specifies an identifying number assigned to the data item that uniquely
identifies the data item and the format.
This was assigned to the item when it was originally passed by
\fBXmClipboardCopy\fP.
.LE
.P
.mc |
.ne 9
.mc
The
\fBXmClipboardWithdrawFormat\fP
function
indicates that the application will no longer supply a data item to the
clipboard that the application had previously passed by name.
.P
This function can return one of the following status return constants: 
.mc |
.P
.VL 1.5i
.LI "\fBClipboardSuccess\fP"
.mc
The function is successful.
.mc |
.LI "\fBClipboardLocked\fP"
.mc
The function failed because the clipboard was locked by another application.
The application can continue to call the function again with the same
parameters until the lock goes away.
This gives the application the opportunity 
to ask if the user wants to keep trying
.mc |
or to give up on the operation.
.LE
.mc
.H 2 "Dynamic Resource Defaulting"
.iX "Dynamic resource defaulting"
.iX "Default, dynamic"
Dynamic resource defaulting is a mechanism that incorporates a processing
function into a widget's resource definitions.  The widget can use this
mechanism to calculate a default
resource value when it (the widget) is created, instead of having the
resource default be static.
The widget set uses this capability to determine much of its visual
resource defaults at run time.  This allows
the widget to make more sensible choices for color and pixmap
defaults.
.P
All of the color resources and pixmap resources that represent visual
data are dynamically defaulted.  This includes
the resources \fBXmNforeground, XmNbackground,
XmNbackgroundPixmap, XmNtopShadowColor, XmNtopShadowPixmap,
XmNbottomShadowColor, XmNbottomShadowPixmap, 
XmNhighlightColor\fP, and \fBXmNhighlightPixmap\fP.
.P
.mc |
Color and pixmap data are set as follows:
.mc
.BL
.LI
.mc |
Set to black and white if a monochrome system is used
.mc
.LI
If a color system is used:
.mc |
.DL
.mc
.LI
.mc |
A default
color scheme, or
.mc
.LI
A color scheme based on the background resource, \fBXmNbackground\fP.
.LE
.LE
.P
.mc |
.ne 7
.mc
Part of the design for the widget set and window manager
includes an algorithmic approach for generating color schemes.
This means that by specifying the background color, the foreground
and two shadowing 
colors are calculated.
.P
.mc |
.ig ++
.mc
.H 2 "Key/Keyboard Grabbing"
.iX "Grabbing"
.iX "Keyboard grabbing"
Motif provides several routines for redirecting keyboard
.mc |
events within a widget hierarchy.  These routines are front ends to the
.mc
Xlib routines that provide key/keyboard grabbing. 
.P
The client should use these routines rather than the raw Xlib routines
.mc |
so that the Intrinsics can be aware of grabs and process them in
.mc
correct priority relative to the intrinsics focus and modal cascade
.mc |
management.  Grabs will have priority over the focus redirection, but
.mc
will be overridden by the modal cascade in the case of contention. 
.H 3 "Passive Grabs"
.iX "Grabbing" "passive"
.iX "Passive grabbing"
.iX "XtGrabKey"
.iX "XtUngrabKey"
.P
\fBXtGrabKey\fP and \fBXtUngrabKey\fP allow the client to redirect the specified
keyevent (as described by keysym modifiers) to the root widget of a
hierarchy. The root widget is the widget parameter to the \fBXtGrabKey\fP
call and all keyevents that would have been dispatched to other
subwindows will get dispatched to it. Note that the use of
\fBowner_events = True\fP is not meaningful for \fBXtGrabKey\fP
since all widgets
in the hierarchy belong to the grabbing client. In addition, the
.mc |
\fIpointer_mode\fP and \fIkeyboard_mode\fP fields are currently forced to
.mc
\fBGrabModeAsync\fP in order to avoid deadlock conditions.
.P
.nf
.mc |
\fBvoid  XtGrabKey\fP \fI(widget, keysym, modifiers, owner_events,
                                           pointer_mode, keyboard_mode)\fP
   \fBWidget\fP \fIwidget;\fP
   \fBKeycode\fP \fIkeycode;\fP
   \fBunsigned int\fP \fImodifiers;\fP
   \fBBoolean\fP \fIowner_events;\fP
   \fBint\fP \fIpointer_mode;\fP
   \fBint\fP \fIkeyboard_mode;\fP
.mc
.fi
.mc |
.P
.ne 10
\fBXtGrabKey\fP establishes a passive grab on the specified keys, such that
.mc
when the specified key/modifier combination is pressed, the keyboard
.mc |
is grabbed. If owner events is False, then all key events will be
dispatched to the Grab widget.
.mc
.P
.nf
.mc |
\fBvoid XtUngrabKey\fP \fI(widget, keysym, modifiers)\fP
    \fBWidget\fP \fIwidget;\fP
    \fBKeycode\fP \fIkeycode;\fP
    \fBunsigned int\fP \fImodifiers;\fP
.mc
.P
.fi
\fBXtUngrabKey\fP cancels the passive grab on the key combination on the
specified widget. A modifier of \fBAnyModifier\fP is equivalent to issuing
the request for all possible modifier combinations. A keysym of
\fBAnyKey\fP is equivalent to issuing the request for all possible
.mc |
nonmodifier key codes. This call has no effect on an active grab.
.mc
.H 3 "Active Grabs"
.iX "Grabbing" "active"
.iX "Active grab"
.iX "XtGrabKeyboard"
.P
.mc |
.nf
\fBint XtGrabKeyboard\fP \fI(widget, owner_events, 
                                    pointer_mode, keyboard_mode,time)\fP
    \fBWidget\fP \fIwidget;\fP
    \fBBoolean\fP \fIowner_events;\fP
    \fBint\fP \fIpointer_mode;\fP
    \fBint\fP \fIkeyboard_mode;\fP
    \fBTime\fP \fItime;\fP
.mc
.P
.fi
\fBXtGrabKeyboard\fP actively grabs control of the main keyboard. If the
grab is successful, it returns the constant \fBGrabSuccess\fP. Further key
events are reported to the grab widget.
.P
.iX "XtUngrabKeyboard"
.nf
.mc |
\fBvoid  XtUngrabKeyboard\fP \fI(widget, time)\fP
    \fBWidget\fP \fIwidget;\fP
    \fBTime\fP \fItime;\fP
.mc
.P
.fi
\fBXtUngrabKeyboard\fP releases any active grab on the keyboard.
.mc |
.br
.ne 15
.++
.mc
.H 2 "Localization"
.iX "Localization of defaults file"
.mc |
.iX "Defaults files" "localization
The Motif toolkit supports the Xt Intrinsics mechanisms for localization
of resource files.
You can specify a language by using the \fBxnlLanguage\fP resource or
the \fBLANG\fP environment variable.
Elements of this variable are then used to establish a path to the
proper resource files.
The following substitutions are used in building the path:
.mc
.BL
.LI
.mc |
%N is replaced by class name of the application.
.mc
.LI
.mc |
%L is replaced by the display's language specification (from the
\fBxnlLanguage\fP resource or, if that resource is not defined, the
\fBLANG\fP environment variable).
.mc
.LI
.mc |
%l is replaced by the language part of the language specification.
.mc
.LI
.mc |
%t is replaced by the territory part of the language specification.
.mc
.LI
.mc |
%c is replaced by the codeset part of the language specification.
.mc
.LI
%% is replaced by %.
.LE
.P
.mc |
If the \fBxnlLanguage\fP resource or the \fBLANG\fP environment variable
is not defined, or if one of its parts is missing, then a % element that
references it is replaced by NULL.
.mc
.P
The paths contain a series of elements separated by colons.  Each
.mc |
element denotes a filename, and the filenames are looked up
left-to-right until one of them succeeds.  Before doing the
.mc
lookup, substitutions are performed.
.P
.mc |
\fBNOTE\fP: The Intrinsics use the X/Open convention of collapsing
multiple adjoining slashes in a filename into one slash.
.mc
.P
.mc |
The \fBXtDisplayInitalize\fP function loads the resource database by
merging in resources from these sources, in order:
.mc
.BL
.LI
.mc |
Application-specific class resource file on the local host  
.mc
.LI
.mc |
Application-specific user resource file on the local host
.mc
.LI
Resource property on the server or user preference resource file on the
.mc |
local host
.mc
.LI
.mc |
Per-host user environment resource file on the local host
.mc
.LI
.mc |
The application command line
.mc
.LE
.P
.mc |
To load the application-specific class resource file,
\fBXtDisplayInitialize\fP performs the appropriate substitutions on 
the path specified by the \fBXFILESEARCHPATH\fP environment variable.
If that fails, or if \fBXFILESEARCHPATH\fP is not defined,
\fBXtDisplayInitialize\fP uses the following as the path:
.mc
.P
.mc |
\fB/usr/lib/X11/%L/app-defaults/%N:/usr/lib/X11/%l/app-defaults/%N:/usr/lib/X11/app-defaults/%N\fP
.mc
.P
.mc |
If the language is not specified (or the first path lookup using the
language specification fails), the lookup defaults to the
current non-language-specific location
(\fB/usr/lib/X11/app_defaults/%N\fP).
.mc
.P
.mc |
If no application-specific class resource file is found,
\fBXtDisplayInitialize\fP looks for any fallback resources that may have
been defined by a call to \fBXtAppInitialize\fP or
\fBXtAppSetFallbackResources\fP.
.mc
.P
.mc |
To load the user's application resource file, \fBXtDisplayInitialize\fP
performs the following steps:
.mc
.AL
.LI
.mc |
Use \fBXUSERFILESEARCHPATH\fP to look up the file, performing
appropriate substitutions.
.mc
.LI
.mc |
If that fails, or if \fBXUSERFILESEARCHPATH\fP is not defined, and
.mc
if \fBXAPPLRESDIR\fP is defined, use the following as the path:
.mc |
.sp
\fB$XAPPLRESDIR/%L/%N:$XAPPLRESDIR/%l/%N:$XAPPLRESDIR/%N:$HOME/%N\fP
.sp
where \fB$XAPPLRESDIR\fP is the value of the \fBXAPPLRESDIR\fP
environment variable and \fB$HOME\fP is the user's home directory.
.LI
If \fBXAPPLRESDIR\fP is not defined, use the following as the path:
.sp
\fB$HOME/%L/%N:$HOME/%l/%N:$HOME/%N\fP
.mc
.LE
.P
.mc |
To load the server resource property or user preference file,
\fBXtDisplayInitialize\fP first looks for a RESOURCE_MANAGER property on
the root window of the display's screen 0.
If that property does not exist, \fBXtDisplayInitialize\fP looks for the
file \fB$HOME/.Xdefaults\fP.
.P
To load the per-host user environment resources,
\fBXtDisplayInitialize\fP uses the filename specified by the
\fBXENVIRONMENT\fP environment variable.
If \fBXENVIRONMENT\fP is not defined, \fBXtDisplayInitialize\fP looks
for the file \fB$HOME/.Xdefaults-\fP\fIhost\fP, where \fIhost\fP is the
name of the host on which the application is running.
.mc
.H 2 "Pixmap Caching Functions"
.iX "Pixmap" "naming"
.iX "Pixmap" "caching"
The pixmap caching functions provide the application and widget writer 
with a means of associating an image with a name.  Given this
association, these functions can generate pixmaps through references to
.mc |
a \fB\&.Xdefaults\fP file (by name) and through an argument list (by pixmap), for
.mc
all widgets that have pixmap resources.
A cache of all pixmaps is automatically maintained.  This improves
performance and decreases server data space when requesting identical
pixmaps.
.P
.iX "Functions" "pixmap caching"
.iX "Pixmap caching functions"
The pixmap caching provides four functions by which the application or
widget writer can install images, uninstall images, create pixmaps,
and destroy pixmaps.
.iX "XmInstallImage"
.P
.mc |
\fBBoolean XmInstallImage\fP \fI(image, image_name)\fP
.mc
.br
.mc |
    \fBXImage\fP    \fI* image\fP;
.mc
.br
.mc |
    \fBchar\fP      \fI* image_name\fP;
.mc
.P
.VL 1i
.mc |
.ne 12
.mc
.LI\fIimage\fP
Points to the image structure to be installed.  The installation
process does not make a local copy of the image, therefore the
application should not destroy the image until it is installed from
the caching functions.
.LI\fIimage_name\fP
.mc |
Specifies a string that the application uses to name the image.  After
installation, this name can be used in a \fB\&.Xdefaults\fP file for
.mc
referencing the image.  A local copy of the name is created by the image
caching functions.
.LE
.P
\fBXmInstallImage\fP is used to give to the caching routines an image
that can later be used to generate a pixmap.  Part of the installation
process is to extend the resource converter used to reference these
images.  The resource converter can access the image name so that the
.mc |
image can be referenced in a \fB\&.Xdefaults\fP file.
.mc
Since an image can be referenced by a widget through its pixmap 
resources, it is up to the
application to ensure that the image is installed before the widget
is created.  \fIimage\fP is a pointer to the image structure
to be installed.  The installation
process does not make a local copy of the image.  Therefore, the application
should not destroy the image until it is uninstalled from the caching
functions.  \fIimage_name\fP is a string the application uses
to name the image.  After 
.mc |
installation this name can be used in a \fB\&.Xdefaults\fP file for referencing the image.
.mc
A local copy of the name is created by the image caching functions.
.P
The image caching functions provide a set of eight preinstalled images.
.mc |
These names can be used within a \fB\&.Xdefaults\fP file for generating pixmaps
.mc
for the resource they are provided for.
.mc |
.br
.ne 3i
.mc
.TB "Preinstalled Images"
.TS
box, center, tab(;);
lb | lb
l | l .
Image Name;Description
_
.sp .2
background;A tile of solid background
25_foreground;A tile of 25% foreground, 75% background 
50_foreground;A tile of 50% foreground, 50% background 
75_foreground;A tile of 75% foreground, 25% background 
horizontal;A tile of horizontal lines of the two colors
vertical;A tile of vertical lines of the two colors
slant_right;A tile of slanting lines of the two colors
slant_left;A tile of slanting lines of the two colors
.sp .2
.TE
.P
.iX "XmUninstallImage"
.mc |
\fBBoolean XmUninstallImage\fP \fI(image)\fP
.mc
.br
.mc |
    \fBXmImage\fP    *\fIimage\fP;
.mc
.br
.VL 1i
.LI\fIimage\fP
Points to the image structure given to the \fBXmInstallImage\fP
.mc |
routine
.mc
.LE
.P
XmUninstallImage provides the mechanism by which an image can
be removed from the caching routines.  \fIimage\fP is a 
.mc |
pointer to the image given to the \fBXmInstallImage\fP routine.
.mc
.P
An application or widget makes a call to extract a pixmap when
the images have been installed or to access a set of the predefined images.
When an application or widget is finished with a pixmap, it can call a function
.mc |
to destroy the pixmap.  These functions are defined as follows:
.mc
.P
.iX "XmGetPixmap"
.mc |
\fBPixmap XmGetPixmap\fP \fI(screen, image_name, foreground, background)\fP
.mc
.br
.mc |
   \fBScreen\fP   \fI* screen\fP;
.mc
.br
.mc |
   \fBchar\fP     \fI* image_name\fP;
.mc
.br
.mc |
   \fBPixel\fP    \fIforeground\fP;
.mc
.br
.mc |
   \fBPixel\fP    \fIbackground\fP;
.mc
.fi
.P
.VL 1i
.LI\fIscreen\fP
Specifies the display screen on which the pixmap is to 
be drawn and is used to ensure that the pixmap matches the visual required 
for the screen.
.LI\fIimage_name\fP
Specifies the name of the image to be used to 
generate the pixmap.
.LI\fIforeground\fP
.mc |
Combines the image with the foreground color to create the pixmap
.mc
if the image referenced is a bit-per-pixel image.
.LI\fIbackground\fP
.mc |
Combines the image with the background color to create the pixmap
.mc
if the image referenced is a bit-per-pixel image.
.LE
.P
\fBXmGetPixmap\fP uses the parameter data to perform a lookup in the
pixmap cache to see if a pixmap has already been generated that
matches the data for the specified screen.  If one is found, a
reference count is incremented
and the pixmap is returned.  If one is not found, the image corresponding to
\fIimage_name\fP is used to generate a pixmap which is then cached
and returned.  \fIscreen\fP contains
the display screen on which the pixmap is to 
be drawn and is used to ensure the pixmap matches the visual required 
for the screen.  \fIimage_name\fP is the name of the image 
to be used to generate the pixmap.
.mc |
If a bit-per-pixel image is being accessed, \fIforeground\fP 
and \fIbackground\fP are combined with the image to create the pixmap.
.mc
.P
.iX "XmDestroyPixmap"
.mc |
\fBBoolean XmDestroyPixmap\fP \fI(screen, pixmap)\fP
.mc
.br
.mc |
   \fBScreen\fP    \fI* screen\fP;
.mc
.br
.mc |
   \fBPixmap\fP    \fIpixmap\fP;
.mc
.br 
.P
.VL 1i
.LI\fIscreen\fP
.mc |
Specifies the display screen for which the pixmap was requested
.mc
.LI\fIpixmap\fP
.mc |
Specifes the pixmap to be destroyed
.mc
.LE
.P
\fBXmDestroyPixmap\fP is used to remove pixmaps that are no longer
.mc |
needed.  A pixmap is completely freed only when there is no further
reference to it.
.mc
.H 2 "Resolution Independence"
.iX "Resolution independence"
.mc |
The OSF/Motif widget set has a built-in mechanism 
called resolution independence.  Resolution independence allows
.mc
applications to create and display images that are the same physical size
regardless of the resolution of the display.  This frees the application
developer from the task of ensuring that an application can be 
used on a wide range of systems.
.P
The resolution independence mechanism provides resource data 
to the widgets in various unit types, including millimeters, inches,
points, and font units.  All widget resources connected with size,
.mc |
position, thickness, padding, and spacing can be set using the preceding 
.mc
unit types.  The application or user of the application can provide 
resolution independent data 
.mc |
through \fB.Xdefaults\fP and applications-defaults files, command line arguments,
or argument lists.
.mc
.H 3 "The Resolution Independence Mechanism"
.mc |
The unit a widget uses is defined by the resource
.mc
XmNunitType, which can be found in the base classes of XmPrimitive,
XmGadget, and XmManager.  Since
all widgets are built from these base classes, it follows that all
.mc |
widgets support resolution independence.  
XmNunitType can have five values:
.mc
.BL
.LI
\fBXmPIXELS\fP - 
All values provided to the widget are treated as normal pixel values.  This
is the default value for the resource.
.LI
\fBXm100TH_MILLIMETERS\fP - 
All values provided to the widget are treated as 1/100 of a millimeter.
.LI
\fBXm1000TH_INCHES\fP - 
All values provided to the widget are treated as 1/1000 of an inch.
.LI
\fBXm100TH_POINTS\fP - 
All values provided to the widget are treated as 1/100 of a point.
A point is a unit typically used in text processing applications and
is defined as 1/72 of an inch.
.LI
\fBXm100TH_FONT_UNITS\fP - 
All values provided to the widget are treated as 1/100 of a font unit.
.mc |
The value to be used for the font unit is determined in one of three
ways:
.DL
.LI
The application can use \fBXmSetFontUnit\fP to specify the font unit
values.
This function is described later in this section.
.LI
If the application has not called \fBXmSetFontUnit\fP, the font units
can be derived from a font.
The user specifies this font by using the font resource in a
defaults file or on the command line, or by using the standard command
line options \fB\-fn\fP and \fB\-font\fP.
.LI
If the application has not called \fBXmSetFontUnit\fP and the user has
not specified a font resource, the horizontal and vertical font units
default to 10.
.mc
.LE
.mc |
.sp
A horizontal font unit is derived from a font as follows:
.BL
.LI
If the font has an \fBAVERAGE_WIDTH\fP property, the horizontal font
unit is the \fBAVERAGE_WIDTH\fP property divided by 10.
.LI
If the font has no \fBAVERAGE_WIDTH\fP property but has a
\fBQUAD_WIDTH\fP property, the horizontal font unit is the
\fBQUAD_WIDTH\fP property.
.LI
If the font has no \fBAVERAGE_WIDTH\fP or \fBQUAD_WIDTH\fP property, the
horizontal font unit is the sum of the font struct's min_bounds.width
and max_bounds.width divided by 2.3.
.LE
.sp
A vertical font unit is derived from a font as follows:
.BL
.LI
If the font has a \fBPIXEL_SIZE\fP property, the vertical font unit is
the \fBPIXEL_SIZE\fP property divided by 1.8.
.LI
If the font has no \fBPIXEL_SIZE\fP property but has \fBPOINT_SIZE\fP
and \fBRESOLUTION_Y\fP properties, the vertical font unit is the product
of the \fBPOINT_SIZE\fP and \fBRESOLUTION_Y\fP properties divided by
1400.
.LI
If the font has no \fBPIXEL_SIZE\fP, \fBPOINT_SIZE\fP, or
\fBRESOLUTION_Y\fP properties, the vertical font unit is the sum of the
font struct's max_bounds.ascent and max_bounds.descent divided by 2.2.
.LE
.LE
.mc
.P
.mc |
There are two reasons for the unit types to be fractional:
.mc
.BL
.LI
It allows all calculations to be done in integer representation.
This ensures maximum performance for type conversions.
.LI
.mc |
There is no way to supply a floating-point number to
.mc
a widget through an argument list.  This is because the value
field in the argument list is of type \fBchar *\fP.  When
.mc |
a floating-point value
.mc
is forced into a \fBchar *\fP variable, the fractional part is truncated.
.LE
.P
When a widget is created and its unit type is something other than pixels,
it converts the data specified by the application or the user
into pixel values,
taking into account the resolution of the screen.  These converted
pixel values are then placed into the internal data space of the widget,
and the widget operates as it would normally.  The same process occurs
.mc |
when the application issues an \fBXtSetValues\fP to a widget. The new
.mc
values are converted from unit type to pixels and placed back into
the widget.
.P
.mc |
When the application issues an \fBXtGetValues\fP to a widget, the pixel
.mc
values are taken out of the widget, converted back to the unit type,
and inserted into the argument list to be returned.
.P
The conversion and storing of unit type values to pixel values can cause some
rounding errors.  Therefore, when an application issues
.mc |
an \fBXtGetValues\fP, it 
.mc
should not expect exactly the same data to be returned as was originally 
specified.  This rounding error will only occur once and will not get
progressively worse.  For example, if a widget's width is set to
.mc |
1000/1000 inches (1 inch), \fBXtGetValues\fP may return 993/1000 inches.  If
.mc
this value is then used to set the width of a second widget and the
.mc |
application calls \fBXtGetValues\fP on the second widget, 993/1000 inches
.mc
will be returned.
.H 3 "Setting the Font Units"
.iX "Font units, setting"
.iX "Setting font units"
Applications may want to specify resolution independent data based on a 
global font size.  The widget set provides an external function to use to
initialize the font unit values.  This function needs to be called before
any widgets with resolution independent data are created.
.P
.nf
.mc |
    \fBXmSetFontUnit\fP \fI(display, font_unit_value)\fP
    \fBDisplay\fP \fI* display;\fP
    \fBint\fP \fIfont_unit_value;\fP
.mc
.fi
.P
.mc |
The parameters for this function are as follows:
.VL 1.5i
.LI \fIdisplay\fP
.mc
Defines the display for which this font unit
value is to be applied.
.mc |
.LI \fIfont_unit_value\fP
Specifies the value to be used for both horizontal and vertical font
units in the conversion calculations.
.mc
.LE
.H 3 "Converting Between Unit Types"
.iX "Converting between types"
The widgets use a general conversion function to convert between pixels 
and other unit types.  This function can convert values between any of the
defined unit types, and is available to the application for its use.
.P
.iX "XmConvertUnits"
.nf
.mc |
\fBint XmConvertUnits\fP \fI(widget, orientation, from_unit_type, from_value,
.mc
to_unit_type)\fP
.mc |
    \fBWidget\fP \fI* widget;\fP
    \fBint\fP \fIorientation;\fP
    \fBint\fP \fIfrom_unit_type;\fP
    \fBint\fP \fIfrom_value;\fP
    \fBint\fP \fIto_unit_type;\fP
.mc
.fi
.P
\fBXmConvertUnits\fP uses the parameter data to convert the value and return it
as the return value from the function.  The parameters for this function
are as follows:
.mc |
.VL 2.5i
.LI \fIwidget\fP
.mc
Specifies the widget for which the data is to be converted.
.mc |
.LI \fIorientation\fP
.mc
Specifies whether
the converter should use the horizontal screen resolution or vertical
screen resolution when performing the conversions.  \fIorientation\fP
can have values of \fBXmHORIZONTAL\fP
or \fBXmVERTICAL\fP.  
.mc |
.LI \fIfrom_unit_type\fP
.mc
Specifies the current unit type of the supplied value.
.mc |
.LI \fIfrom_value\fP
.mc
Specifies the value to be converted.
.mc |
.LI \fIto_unit_type\fP
.mc
Specifies the unit type into which the value should be converted.
.LE
.H 2 "Interacting With the Motif Window Manager"
.iX "Motif Window Manager"
.iX "Protocol"
This section explains the procedures an application can use to
interact with the WM_PROTOCOLS and system menu facilities provided by
.mc |
MWM.
Be sure that you have read at least the discussion of MWM properties in
the Motif Window Manager chapters before reading this section.
You should also be familiar with the concepts presented in the
\fIInter-Client Communications Conventions Manual\fR (ICCCM).
.ig zz
.mc
.P
.iX "Conventions" "ICCC"
.iX "ICCC conventions"
Many of the proposed Inter-Client Communications Conventions (ICCC)
are not currently supported by the X Consortium libraries (Xlib and
Xt), because the the conventions were not official when the R3 version
of the X Window System was introduced.  The X Consortium has
indicated its intent to add support
for the ICCC in the next X11 release (R4).  The Motif toolkit supports
a minimal set of inter-client (mainly application-to-window manager)
communication
services and provides them as a layer above existing intrinsics
facilities.
.mc |
.zz
.mc
.H 3 "Protocol Management"
.iX "Protocol" "management"
.mc |
The protocol management functions are a set of general purpose
.mc
routines for interacting with properties that contain atom arrays,
client messages, and associated callbacks.  They are used to support
the existing entries for the WM_PROTOCOLS and  _MOTIF_WM_MESSAGES properties.
.mc |
See the section about configuring the OSF/Motif window manager for more
.mc
information.
.P
.mc |
\fBNOTE:\fR In the following discussion, the names of atoms or properties 
are in upper case and are obtained by interning the strings with the
server.  Use \fBXmInternAtom\fP to convert these strings to a 32-bit 
tag.  The following code segment shows how to obtain the
.mc
_MOTIF_WM_MESSAGES atom.
.P
.nf
.mc |
.ta .5i 1i 1.5i
.mc
.ft CW
Atom motif_wm_messages;

.mc |
   motif_wm_messages = XmInternAtom(display, 
        "_MOTIF_WM_MESSAGES", true);
.ta .5i 3i
.mc
.P
.fi
.ft R
Alternatively, you could use _XA_MOTIF_WM_MESSAGES (defined in 
.mc |
\fBX11/MwmUtil.h\fP) as
.mc
the second argument to \fBXmInternAtom\fP. 
.P
.iX "Protocol"
.mc |
A protocol is a 32-bit tag used by clients to communicate with
.mc
the window manager.  This tag is either an
X Atom or an arbitrary long integer variable whose value is shared by
.mc |
the parties to the protocol communication.  The client
.mc
indicates interest in certain communications protocols by adding these
tags to a tag array that is the value of a special property on its
.mc |
top-level window.  For predefined ICCC protocols, this
property is WM_PROTOCOLS.  For MWM,
this property is _MOTIF_WM_MESSAGES.  The window
manager sends a protocol
message (when appropriate) in the form of a
client message event with the
.ne 5
\fImessage_type\fP
.mc
field of the \fBClientMessage\fP
structure set to the property and the \fIdata.l[0]\fP field set to the
protocol.  The client can associate a callback list
with the protocol that is invoked when the client message event is
received.
.P
Each shell can have one protocol manager per property  associated with
.mc |
it, and the protocol manager can have multiple
.mc
protocols registered.  Each protocol is identified by its tag
(WM_SAVE_YOURSELF, for example).  The protocols can have any number of client
.mc |
callbacks associated with them, in addition to prehook and posthook
.mc
callbacks (usually registered by the widget set) for each protocol.
.P 
.mc |
The protocol manager does the following:
.mc
.BL
.LI
Tracks the state of the protocols whether or not they 
.mc |
are active
.mc
.LI
Tracks the state of the Shell and creates
.mc |
and updates the protocol property accordingly
.mc
.LI
Processes the client messages received and invokes the
.mc |
appropriate callbacks
.mc
.LE
.H 4 "_MOTIF_WM_MESSAGES"
The client uses the _MOTIF_WM_MESSAGES property to indicate to MWM
.mc |
which messages (sent by MWM when an \fBsend_msg\fP function is
.mc
invoked from the MWM system menu) it is currently handling.  A client
.mc |
can add \fBf.send_msg\fP entries to the menu by using the
.mc
\fB\&.mwmrc\fP file or by using the \fBXmNmwmMenu\fP resource of
VendorShell.  This resource is a string that is parsed by MWM to
.mc |
determine what to display in the system menu and how to react to
.mc
an item's selection.  When the action associated with an item is
.mc |
\fBf.send_msg\fP, MWM sends the client a message if the specified
.mc
protocol is active.  The protocol is the integer argument to the
.mc |
\fBf.send_msg\fP action.  A menu protocol is active if the protocol
.mc
is in the _MOTIF_WM_MESSAGES property and the _MOTIF_WM_MESSAGES atom
is in the WM_PROTOCOLS property.  Otherwise, the protocol is inactive
.mc |
and the menu label will be grayed out.
.mc
.H 4 "WM_PROTOCOLS"
.mc |
A corresponding macro is provided for each of 
.mc
the general protocol manager routines to simplify their use.  The
only difference between
them is that the general routines are passed a protocol property in
.mc |
all calls while the macros always force this property to
.mc
WM_PROTOCOLS.  These macros are useful if you want to interact with 
ICCC protocols such as  WM_DELETE_WINDOW or WM_SAVE_YOURSELF.
.P
Note that if you are using the protocol manager for the system menu, the
property should be the atom corresponding to _MOTIF_WM_MESSAGES.
.H 3 "Protocol Manager Functions"
.mc |
The following sections list the protocol manager functions.  A
sample program, \fBxmprotocol\fP, in the \fB \./demos\fP directory 
.mc
adds or deactivates entries to the system menu.  You can use the
methods presented in that program to get an idea of how to use the
functions discussed in this section.
.P
Note that the
statement

\f(CW#include <X11/Protocols.h>\fP

.mc |
must be present in any program using these functions.
.mc
.P
.mc |
The functions that have the letters WM are the macros referred to
.mc
earlier.  Each function has a corresponding macro.  For example,
\fBXmAddProtocols\fP has a corresponding macro
\fBXmAddWMProtocols\fP.  The macro simply calls
\fBXmAddProtocols\fP with the \fIproperty\fP parameter set to
XA_WM_PROTOCOL.
.H 4 "Add and Remove Functions"
.mc *
.mc
.P
.mc |
The following routine adds the protocols to the protocol manager
.mc
corresponding to the specified property and allocate the internal
tables. The protocols are initialized to active.
.mc |
.P
.mc
.nf
.mc |
\fBvoid XmAddWMProtocols\fP \fI(shell, protocols, num_protocols)\fP
\fBWidget\fP   \fIshell\fP;
\fBAtom\fP   \fI*protocols\fP;
\fBCardinal\fP   \fInum_protocols;\fP
.P
.br
.ne 6
\fBvoid XmAddProtocols\fP \fI(shell, property, protocols, num_protocols)\fP
\fBWidget\fP      \fIshell;\fP
\fBAtom\fP      \fIproperty;\fP   
\fBAtom\fP      \fI*protocols;\fP
\fBCardinal\fP      \fInum_protocols;\fP
.P
.mc
.fi
.mc |
.P 
The following routine removes the protocols from the protocol manager and
.mc
deallocates the internal tables.  It also updates the handlers and the
property if any of the protocols are active
and the shell referenced in the \fIshell\fP parameter is realized.
.mc |
.P
.nf
\fBvoid XmRemoveWMProtocols\fP \fI(shell, protocols, num_protocols)\fP
\fBWidget\fP   \fIshell;\fP
\fBAtom\fP   \fI*protocol;\fP
\fBCardinal\fP \fInum_protocols;\fP

\fBvoid XmRemoveProtocols\fP \fI(shell, property, protocols, num_protocols)\fP
\fBWidget\fP   \fIshell;\fP
\fBAtom\fP   \fIproperty;\fP
\fBAtom\fP   \fI*protocols;\fP
\fBCardinal\fP \fInum_protocols;\fP
.fi
.mc
.H 4 "Protocol State"
It is sometimes useful to allow a protocol's state information
(callback lists for example) to persist, even though the client may choose
to temporarily resign from the interaction.  The main use of this
capability is to gray out \f(CWf.send_msg\fP labels in the system menu. This
is supported by allowing a protocol to be in one of two states, active
or inactive.  If the protocol is active and the shell is realized,
then the property contains the protocol atom.  If the protocol is
inactive, then the atom is not present in the property.
.mc *
.mc
.P 
.mc |
If the protocol is inactive, the following routine updates the handlers
.mc
and adds the protocol to the property if the shell is realized.
.mc |
.SP
.mc
.nf
.mc |
\fBvoid XmActivateWMProtocol\fP \fI(shell, protocol)\fP
\fBWidget\fP   \fIshell;\fP
\fBAtom\fP   \fIprotocol;\fP
.P
.br
.ne 5
\fBvoid XmActivateProtocol\fP \fI(shell, property, protocol)\fP
\fBWidget\fP   \fIshell;\fP
\fBAtom\fP   \fIproperty;\fP
\fBAtom\fP   \fIprotocol;\fP
.mc
.fi
.P
If the protocol is inactive, this routine updates the handlers
and the property if the shell is realized.
.mc |
.P   
.nf
\fBvoid XmDeactivateWMProtocol\fP \fI(shell, protocol)\fP
\fBWidget\fP   \fIshell;\fP
\fBAtom\fP   \fIprotocol;\fP
.P
\fBvoid XmDeactivateProtocol\fP \fI(shell, property, protocol)\fP
\fBAtom\fP   \fIproperty;\fP
\fBCardinal\fP \fInum_protocols;\fP
.fi
.mc
.H 4 "Protocol Callbacks"
When a client message associated with a protocol is received by the
protocol manager, it checks to see if the protocol is active.  If it is,
.mc |
then any callbacks associated with the protocol are called.  Three
callback lists can be associated with a protocol.  One is
for client use and is accessed by \fBXmAddProtocolCallback\fP
and \fBXmRemoveProtocolCallback\fP.  The
other two (the pre-hook and post-hook callbacks) are intended
.mc
for toolkit use and are accessed by the \fBXmSetProtocolHooks\fP routine.
The hook routines are called before and after the client callbacks (if
any) are called.  The protocol callbacks have a reason field of
XmCR_PROTOCOLS and a type of XmAnyCallbackStruct.
.P
.mc |
The following routine checks to see if the protocol is registered and if not, it calls
.mc
\fBXmAddProtocols\fP.  It then adds the callbacks to the internal list.
These callbacks are called when the corresponding client message
.mc |
is received.
.mc
.P
.mc |
.nf
.ta .5i 1i 1.5i
\fBvoid XmAddWMProtocolCallback\fP \fI(shell, protocol, callback, 
    closure)\fP
\fBWidget\fP   \fIshell;\fP
\fBAtom\fP   \fIprotocol;\fP
\fBXtCallbackProc\fP \fIcallback;\fP
\fBcaddr_t\fP \fIclosure;\fP
.mc

.mc |
.br
.ne 6
\fBvoid XmAddProtocolCallback\fP \fI(shell, property, protocol, 
    callback, closure)\fP
\fBWidget\fP   \fIshell;\fP
\fBAtom\fP   \fIproperty;\fP
\fBAtom\fP   \fIprotocol;\fP
\fBXtCallbackProc\fP \fIcallback;\fP
\fBcaddr_t\fP \fIclosure;\fP
.ta .5i 3i
.mc
.fi
.mc *
.mc
.nf
.P
.mc |
The following routine removes the callback from the internal list.
.P
.ta .5i 1i 1.5i
\fBvoid XmRemoveWMProtocolCallback\fP \fI(shell, protocol, callback, 
    closure)\fP
\fBWidget\fP   \fIshell;\fP
\fBAtom\fP   \fIprotocol;\fP
\fBXtCallbackProc\fP \fIcallback;\fP
\fBcaddr_t\fP \fIclosure;\fP
.mc

.mc |
\fBvoid XmRemoveProtocolCallback\fP \fI(shell, property, protocol, 
    callback, closure)\fP
\fBWidget\fP   \fIshell;\fP
\fBAtom\fP   \fIproperty;\fP
\fBAtom\fP   \fIprotocol;\fP
\fBXtCallbackProc\fP \fIcallback;\fP
\fBcaddr_t\fP \fIclosure;\fP
.mc
.fi
.mc |
.ta .5i 3i
.mc
.P
.mc |
The following routine is used by toolkit widgets that want to have before and
after actions executed when a protocol message is received from the window
.mc
manager.  Since there is no guaranteed ordering in execution of event
handlers or callback lists, this allows the shell to control the flow
.mc |
while leaving the protocol manager structures private.  
The callback procedure's \fICall_data\fP argument will
.mc
contain the same pointer as that passed to the client callbacks.
.mc |
.P
.nf
.ta .5i 1i 1.5i
\fBvoid XmSetWMProtocolHooks\fP \fI(shell, protocol, prehook, pre_closure,
    posthook, post_closure)\fP
\fBWidget\fP      \fIshell;\fP
\fBXtCallbackProc\fP    \fIprehook, posthook;\fP
\fBcaddr_t\fP \fIpre_closure, post_closure;\fP

\fBvoid XmSetProtocolHooks\fP \fI(shell, protocol, property, prehook, 
    pre_closure, posthook, post_closure\fP)
\fBWidget\fP      \fIshell;\fP
\fBAtom\fP      \fIproperty;\fP
\fBXtCallbackProc\fP    \fIprehook, posthook;\fP
\fBcaddr_t\fP    \fIpre_closure, post_closure;\fP
.ta .5i 3i
.fi
.P
.ne 7
.mc
.H 3 "Atom Management"
The atom management routines mirror the Xlib interfaces for atom
.mc |
management, but provide client-side caching.  When (and where) caching
.mc
is provided in Xlib, the routines will become pseudonyms for the Xlib
routines.
Note that the statement

.mc |
\f(CW#include <Xm/AtomMgr.h>\fP
.mc

.mc |
must be present in any program using these functions.
.mc
.P
.nf
.mc |
\fBAtom XmInternAtom\fP \fI(display, name, only_if_exists)\fP
   \fBDisplay\fP  \fI*display;\fP
   \fBString\fP   \fIname;\fP
   \fBBoolean\fP  \fIonly_if_exists;\fP
.mc

.mc |
\fBString XmGetAtomName\fP \fI(display, atom)\fP
   \fBDisplay\fP   \fI*display;\fP
   \fBAtom\fP      \fIatom;\fP
.mc

.fi
.H 2 "Motif Version Number"
.iX "Motif" "version number"
.iX "Version number, motif"
.iX "XmVersion"
Motif provides a macro, \fBXmVersion\fP, that returns the current 
Motif version.  Essentially, the macro multiplies the version number
of the library by 1000 and adds the revision number.  For example,
in the first release of Motif Version 1.0, the macro would return
1000.
.P
.mc |
.ne 8
.mc
Additionally, a global variable, \fBXmUseVersion\fP is provided.  The value
of this variable is set to reflect the value returned by \fBXmVersion\fP
as soon as the first widget is created (the setting takes place during
the class initialization procedure of the widget).  In the future, an
application may be able to set this variable to specify the kind of 
behavior the widget library should provide.
.H 2 "Motif Window Manager Presence"
.iX "Motif" "window manager"
.iX "Window manager presence"
.iX "XmIsMotifWMRunning"
.mc |
Users often need to determine whether MWM
is running on a given display.  The Motif 
\fBXmIsMotifWMRunning\fP function can provide this information.  
.mc
.P
.nf
.mc |
\fB#include <Xm/Xm.h>
.mc

Boolean XmIsMotifWMRunning\fP \fI(shell)\fP

.mc |
  \fRWidget\fP  \fIshell;\fP
.mc
.P
.fi
.mc |
\fIshell\fP       Specifies the shell whose screen should be tested for MWM's
.mc
presence.  The function returns True if MWM is running, False if it is
not.
