.mc |
.\"  (c) Copyright 1989, 1990 OPEN SOFTWARE FOUNDATION, INC.
.mc
.\"  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
.\"  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
.\"  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
.\"  (c) Copyright 1988 MICROSOFT CORPORATION
.\"  ALL RIGHTS RESERVED
.mc |
.nr H1 21
.H 1 "Creating User Interfaces with UIL and MRM"
.mc
.P 0
This chapter shows how to build a UIL specification file for the
Motifburger demo, compile the file, and access it with MRM functions.
.mc |
It also provides programming tips for developing
.mc
international applications, working on large projects, and using
user-defined widgets.
.P 0
.H 2 "Specifying a User Interface Using UIL"
.P 0
The examples in this section are based on the demo application called
.mc |
Motifburger, shown in Figure 22-1. In this section, only relevant portions of
.mc
the UIL module for the Motifburger application are shown.  The complete UIL and
C source code for the Motifburger application is shipped with the Motif
.mc |
software kit.  You can copy these source files from \fB./demos/motifburger\fR.
.mc
.P 0
.mc *
.mc
Note that although the Motifburger application is designed to show as many
different widgets and UIL coding techniques as possible, this application does
not use every feature of UIL.
.mc |
Figure 22-5 shows the steps involved at run time to set up
.mc
an interface that was specified with UIL.
.P 0
To specify an interface using UIL, you 
create one or more UIL specification files with names that end with
.mc |
the characters .uil. Each file contains one UIL module block, or a
.mc
legal portion of a module block (such as an object definition).
The number of files you use to completely specify the interface depends
on the complexity of the application; the need for variations
(for example, English and French versions); and the size of the development
project team (on large projects, the UIL module can be distributed over
several files to avoid access competition).
.P 0
For each UIL module, do the following:
.AL 1
.LI 
Declare the UIL module (begin a module block).
.LI
Include the supplied UIL constants file.
.LI
Declare the callback procedures referenced in the object declarations.
.LI
.mc |
Declare the values (integers, strings, colors, and so on) to be used in 
.mc
the object declarations.
.LI
Declare the interface objects (widgets and gadgets).
.LI
End the module block.
.LE
.P 0
The following sections describe how to write each component of a UIL module.
.P 0
.H 3 "Creating a UIL Specification File"
.P 0
.iX "UIL" "structure of module"
.iX "UIL specification file" "structure of"
.iX "UIL module" "structure of"
A UIL specification file contains a module block that consists of a series of
value, identifier, procedure, list, and object sections. There can be any
.mc |
number of these sections in a UIL module.  The UIL include directive
allows you to include the contents of another file in your UIL
module.   To specify one or more complete sections, place the 
include directive wherever a section is valid.  You
cannot use an include directive to specify a part of a section.
.mc
.P 0
You can also use the include directive to have access to the supplied UIL
constants that are useful for specifying values for some arguments such as 
.mc |
\fBXmNdialogStyle\fR and \fBXmNalignment\fR.  Section 22.1.3 describes this constants file.
.mc
.P 0
.mc |
.ne 3i
.mc
The following example shows the overall structure of a UIL module. 
.mc |
.P
.nf
.ft CW
.S -2
.mc
!+
.mc |
!   Sample UIL module
.mc
!-
.mc |

.mc
module example                  ! Module name
!+
.mc |
!   Place module header clauses here.
.mc
!-
.mc |

.mc
!+
.mc |
!   Declare the VALUES, IDENTIFIERS, PROCEDURES, LISTS, and
!   OBJECTS here.
.mc
!-
end module;
.mc |
.S
.fi
.ft R
.mc
.P 0
.H 3 "Declaring the UIL Module"
.P 0
.iX "UIL" "module declaration"
.iX "UIL module" "module declaration"
.iX "UIL specification file" "module declaration"
.iX "Module declaration" "in UIL"
.iX "UIL" "clauses on module declaration"
.iX "UIL module" "module declaration clauses"
.iX "UIL specification file" "module declaration clauses"
.iX "Module declaration clauses" "in UIL"
In the module declaration, you name the module and make module-wide
specifications using module header clauses.
.mc |
The following table explains optional UIL module header
.mc
clauses used in the module declaration.
.mc |
.P
.in 0
.mc
.TB "Optional UIL Module Header Clauses"
.TS H
tab(@);
lB lB lB lB
lw(.6i) lw(1.6i) lw(.8i) lw(2i).
.mc
_
.sp 6p
Clause@Purpose@Default@Example
.sp 6p
_
.sp 6p
.mc *
.mc
.TH
Version@T{
Allows you to ensure the correct version of
the UIL module is being used.
T}@None@T{
.mc |
version = 'v1.1'
.mc
T}
.mc |
.sp .7
T{
Case 
.br
sensitivity
T}@T{
.mc
Specifies whether names in the UIL module are 
case sensitive or not.
.mc |
T}@T{
Case
.br
insensitive
T}@T{
.mc
names = case_sensitive
T}
.mc |
.sp .7
T{
Default
.br
character
.br
set
T}@T{
.mc
Specifies the 
default character set for string literals in the compiled UIL
module.
.mc |
T}@T{
\fIcur_charset\fP
.br
(see Section 20.3)
T}@T{
.mc
character_set = iso_latin6
T}
.mc |
.sp .7
T{
Object 
.br
variant
T}@T{
.mc
Specifies the default variant of objects defined in
the module on a type-by-type basis.
T}@Widget@T{
.mc |
objects = (XmSeparator = gadget;
.br
XmPushButton = widget;)
.mc
T}
.sp 6p
_
.TE
.mc |
.in
.mc
.P 0
The following example shows the module declaration for the Motifburger UIL
module.  The name you specify in the UIL module declaration is stored in the
UID file when you compile the module.  The module declaration for Motifburger
specifies the following:
.BL
.LI
.mc |
MRM identifies the Motifburger interface by the name \fImotifburger_demo\fP.
.mc
.LI
This is the first version of this module.
.LI
Names are case sensitive.
.LI
.mc |
All separator, label, push-button, and
toggle-button objects are gadgets unless overridden in specific object
.mc
declarations.  All other types of objects are widgets.
.mc *
.mc
.LE
.mc |
.P
.ne 3i
.nf
.ft CW
.mc
module motifburger_demo
.mc |

.mc
version = 'v1.0'
names = case_sensitive
objects = {
	XmSeparator = gadget ;
	XmLabel = gadget ;
	XmPushButton = gadget ;
	XmToggleButton = gadget ;
	  }
.mc |
.fi
.ft R
.mc
.P 0
If you specify that names are case sensitive in your UIL module, 
you must put UIL keywords in lowercase letters.
.mc |
For more information on keywords, see Chapter 18.
.mc
Do not use reserved keywords as names in a UIL module. 
.mc |
.H 3 "Using Constants Defined in the Motif Toolkit"
.mc
.P 0
.mc *
.mc
The UIL module for the Motifburger application makes use of some of the 
.mc |
constants defined in the Motif Toolkit.  For example, the constants 
XmDIALOG_MODELESS and XmVERTICAL shown in
the following example come from the toolkit.
.P
.nf
.ft CW
.S -2
object        ! The control panel.  All order entry
              ! is done through this bulletin board dialog.
 control_box : XmBulletinBoardDialog {
     arguments {
         XmNdialogTitle = k_motifburger_title;
         XmNdialogStyle = XmDIALOG_MODELESS;
         XmNx = 600;
         XmNy = 200;
         XmNmarginWidth = 20;
         XmNbackground = lightblue;
     };
     controls {
              ! Some labels and decoration.
         XmLabel           burger_label;
         XmLabel           fries_label;
         XmLabel           drink_label;
         XmSeparator       {arguments {
                             XmNx = 220;
                             XmNy = 20;
                             XmNunitType = XmPIXELS;
                             XmNorientation = XmVERTICAL;
                             XmNheight = 180; };};
.S
.fi
.ft R
.mc
.H 3 "Declaring Procedures"
.P 0
.iX "UIL" "procedure declaration"
.iX "UIL module" "procedure declaration"
.iX "UIL specification file" "procedure declaration"
.iX "Procedure declaration"
Use a procedure declaration to declare a function that can be used as a
callback procedure for an object. You can reference the procedure name in object
.mc |
declarations that occur anywhere in the UIL module.
.mc
.P 0
Callback procedures must be defined
to accept three parameters: the widget identifier of the widget triggering the
callback, a tag for user-defined information, and the callback data structure
(which is unique to each widget). The widget identifier and callback structure
parameters are under the control of the Motif Toolkit;  the tag
is under the control of the application program. 
.mc |
.br
.ne 3i
.mc
.P 0
In a UIL module, you can specify the data type of the tag to be passed to the
corresponding callback procedure at run time by putting the data type in
parentheses following the procedure name.  When you compile the module, the UIL
compiler checks that the argument you specify in references to the procedure is
of this type.  The data type of the tag must be one of the valid
.mc |
UIL types (see Section 22.1.5).
.mc
.P 0
For example, in the following procedure declaration, the callback procedure
named
\fItoggle_proc\fP
.mc |
is passed an integer tag at run time.
.mc
The UIL compiler checks that the parameter specified in
any reference to procedure
\fItoggle_proc\fP
is an integer.
.mc |
.P
.nf
.ft CW
.mc
PROCEDURE
.mc |
    toggle_proc (INTEGER);
.ft R
.fi
.P
While you can use any UIL data type to specify the type of a tag
.mc
in a procedure declaration, you must be able to represent that data type
.mc |
in the high-level language you are using to write your application program.
.mc
Some data types (such as integer, Boolean, and string) are common data types
recognized by most programming languages.  Other UIL data types (such as
string tables) are more complex and may require you to set up an appropriate
corresponding data structure in the application to pass a tag of
that type to a callback procedure.
.P 0
.mc |
The following table summarizes the rules the UIL compiler
.mc
follows for checking the argument type and count. The way you declare the
procedure determines which rule the UIL compiler uses to perform this checking. 
.mc |
.P
.br
.ne 4i
.mc
.TB "UIL Compiler Rules for Checking Argument Type and Count"
.TS H
tab(@);
lB lB
lw(2.0i) lw(2.75i).
_
.sp 6p
Declaration Type@Description of Rule
.sp 6p
_
.sp 6p
.mc *
.mc
.TH
No parameters@T{
.mc |
No argument type or argument-count checking. 
.mc
You can supply no arguments or one argument in the procedure reference.
T}
.mc |
.sp .7
.mc
( )@T{
.mc |
Checks that the argument count is 0.
.mc
T}
.mc |
.sp .7
(\f3any\fP)@T{
.mc
Checks that the argument count is 1.  Does
not check the argument type.  Use 
.mc |
\f3any\fP
.mc
to prevent type 
checking on procedure
tags.
T}
.mc |
.sp .7
(\f2value_type\fP)@T{
.mc
Checks for one argument of 
the specified value type.
T}
.sp 6p
_
.TE
.mc *
.mc
The following example shows that all procedures in
the Motifburger UIL module specify that argument type and argument count are
to be checked when the module is compiled.
.mc |
.P
.nf
.ft CW
.mc
procedure
    toggle_proc     (integer);
    activate_proc   (integer);
    create_proc     (integer);
    scale_proc      (integer);
    list_proc       (integer);
    quit_proc        (string);
    show_hide_proc  (integer);
    pull_proc       (integer);
.mc |
.fi
.ft R
.mc
.P 0
.mc |
.br
.ne 3i
.mc
You can also use a procedure declaration to specify the 
creation function for a
user-defined widget.  In this case, you must not specify any parameters.  The
procedure is invoked with the standard three arguments passed to all widget
creation functions (widget identifier, tag, and callback structure unique to the
calling object). 
.P 0
.H 3 "Declaring Values"
.P 0
.iX "UIL" "value declaration"
.iX "UIL module" "value declaration"
.iX "UIL specification file" "value declaration"
.iX "Value declaration"
A value declaration is a way of giving a name to a value expression.
The value name can be referenced by declarations that occur
.mc |
anywhere in the UIL module in any context where a value can be used.
.mc
.P 0
You should use meaningful names for values to help you recall their purpose
easily.  See Section 
.mc |
22.1.8 for recommended coding techniques.
.mc
.P 0
.mc |
The supported data types for UIL values are
.mc
.iX "UIL" "value types"
.mc |
.iX "Value types list"
.iX "UIL specification file" "value types list"
.iX "UIL module" "value types list"
.P
.BL
.LI
any
.LI
argument
.LI
asciz_table
.LI
Boolean
.LI
color
.LI
color_table
.LI
compound_string
.LI
float
.LI
font
.LI
font_table
.LI
integer
.LI
integer_table
.LI
keysym
.LI
pixmap
.LI
reason
.LI
single_float
.LI
string
.LI
string_table
.LI
translation_table
.LE
.P
.mc
.iX "UIL" "scope"
.iX "MRM" "scope"
.iX "Value declaration" "scope of"
.iX "Imported resource"
.iX "Exported resource"
.iX "Private resource"
.mc *
.mc
You can control whether values are local to the UIL module or globally
accessible by MRM by specifying one of the 
keywords EXPORTED, IMPORTED, or PRIVATE in the value
.mc |
declaration.  (Section 19.2 provides details on how these keywords determine the 
.mc
scope of references to named values and objects.)
.P 0
.mc |
.ne 3i
.mc
The Motifburger application makes use of several kinds of values, as shown
in the following examples.  There is a separate value section for each
type of value to make it easier to find the value declaration
during debugging.
.P 0
.H 4 "Defining Integer Values"
.P 0
Integer values are defined together in a single value section of the
Motifburger UIL module.   These integers are used as tags in the callback
procedures.  A tag provides information to the callback procedure concerning
the circumstances under which the procedure is being called.
The following example shows a segment of this value
.mc |
section:
.P
.nf
.ft CW
.mc
value
    k_create_order          : 1;
    k_order_pdme            : 2;
    k_file_pdme             : 3;
    k_edit_pdme             : 4;    
    k_nyi                   : 5;
    k_apply                 : 6;
    k_dismiss               : 7;
    k_noapply               : 8;
    k_cancel_order          : 9;
    k_submit_order          : 10;
    k_order_box             : 11;
    k_burger_rare           : 12;
    k_burger_medium         : 13;
.mc |
.fi
.ft R
.mc
.P 0
.H 4 "Defining String Values"
.P 0
The following example shows the Motifburger value section
containing string value
declarations.  These strings are the labels
for the various widgets used in the interface.  Using values for widget labels
rather than hardcoding the labels in the specification makes it easier to
modify the interface (for example, from English to German). Putting all 
label definitions together at the beginning of the module makes it easier to
find a label if you want to change it later.  Also, a string resource declared
as a value can be shared by many objects, thereby reducing the size of the UID
file.
.mc |
.P
.nf
.in0
.ft CW
.S -2
.mc
value
.mc |
  k_motifburger_title           : "Motifburger Order-Entry Box";
  k_nyi_label_text              : "Feature is not yet implemented";
  k_file_label_text             : "File";
    k_quit_label_text           : "Quit";
  k_edit_label_text             : "Edit";
    k_cut_dot_label_text        : "Cut";
    k_copy_dot_label_text       : "Copy";
.in
.S
.fi
.ft R
.mc
.P 0
All the string values in this value section, except one, are used as labels. 
.mc |
Because the XmNlabelString argument requires a compound string value,
.mc
the UIL compiler automatically converts these strings to compound strings
(although the strings are declared as null-terminated strings).
.P 0
The exception,
.mc |
k_0_label_text,
.mc
is used to define an argument for
the 
text widget;
since this widget does not accept compound strings, the
value for
.mc |
k_0_label_text
.mc
must be a null-terminated
string.
.P 0
Because there is no default character set specified in
the module header and the individual string values do
not specify a character set, the default character set
associated with all these compound strings is
.mc |
the codeset portion of the value of the \fBLANG\fR environment variable if it
is set, or the value of \fBXmFALLBACK_CHARSET\fP if \fBLANG\fR is not set or
has no codeset component (see Section 20.3).
.mc
.P 0
The indentation shown in the example is not required but improves the 
readability of the UIL module by giving an indication of the widget tree. For
example, the widgets labeled
Cut,
Copy,
Paste,
Clear,
and
Select All
are children
of the widget labeled
Edit.
.mc |
Section 22.1.6.2
explains how to define the widget tree.  Section 22.1.8.2 describes recommended
.mc
coding techniques to improve the readability of your UIL modules.
.P 0
.mc |
By convention, a label followed by ellipses (...) indicates that a
DialogBox appears when the object bearing this label is selected.  
.mc
.P 0
.H 4 "Defining String Table Values"
.P 0
A string table is a convenient way to express a table of strings.  Some
.mc |
widgets require a \fIstring_table\fR argument (such as the list widget, which
.mc
is used for drink selection in the Motifburger application).
.P 0
.mc |
The following example shows the definition of \fIstring_table\fR values in Motifburger. 
.mc
The labels for the types of drinks are elements of the string table named
.mc |
k_drink_list_text.
.mc
Notice that
Apple Juice
is a single
element in the string table named
.mc |
k_drink_list_select.
.mc
This value
is passed to the
.mc |
drink_list_box
.mc
widget to show apple juice as the
default drink selection. 
.P 0
The UIL compiler automatically converts the strings in a string table to
compound strings, regardless of whether the strings are delimited by double or
single quotation marks.
.mc |
.P
.nf
.ft CW
.S -2
.mc
value
.mc |

.mc
\0\0\0.
\0\0\0.
\0\0\0.
.mc |

  k_drinks_label_text       : "Drinks";
    k_0_label_text          : '0';
    k_drink_list_text       :
       string_table ('Apple Juice', 'Orange Juice', 
       'Grape Juice', 'Cola', 'Punch','Root beer', 
       'Water', 'Ginger Ale', 'Milk', 'Coffee', 'Tea');
    k_drink_list_select     : string_table('Apple Juice');
.S
.fi
.ft R
.mc
.P 0
.mc |
.ne 2i
.mc
.H 4 "Defining Font Values"
.P 0
.mc |
Use the \fBFONT\fR function to declare a UIL value as a font.
(See Chapter 18 for more information on defining font values.)
.mc
.P 0
The following example shows the declaration of a font value in the Motifburger
UIL module.  This value is used later as the value for the
.mc |
\fBXmNfontList\fP
.mc
attribute of the
.mc |
apply_button,
can_button,
.mc
and
.mc |
dismiss_button
.mc
push button
widgets.  
.mc |
.P
.nf
.ft CW
.in 0
.S -2
.mc
value
.mc |
 k_button_font  : 
   font('-ADOBE-Courier-Bold-R-Normal--14-140-75-75-M-90-ISO8859-1');
.S
.in
.fi
.ft R
.mc
.P 0
The UIL compiler converts a font to a font table when the font value
.mc |
is used to specify an argument that requires a font-table value.
.mc
.P 0
.mc |
Font names are server dependent.  If you specify a font name that is not
.mc
defined on your server, the system issues a warning and uses the default font.
.mc |
(See Chapter 18 for more
.mc
information on defining font values.)
.P 0
.H 4 "Defining Color Values"
.P 0
The following example shows the value section in the Motifburger module
.mc |
containing color declarations.   By using the \fBCOLOR\fR function, you can 
.mc
designate
a string as specifying a color and then use that string for arguments requiring
.mc |
a color value. The optional keywords \fBFOREGROUND\fR and \fBBACKGROUND\fR identify how the
.mc
color is to be displayed on a monochrome device.  
.mc |
(See Chapter 18 for more
.mc
information on defining color values.)
.mc |
.P
.nf
.ft CW
.S -2
.mc
value
.mc |
   yellow        : color('yellow', foreground);
   red           : color('red', background);
   green         : color('green', foreground);
   magenta       : color('magenta', background);
   gold          : color('gold', foreground);
   lightblue     : color('lightblue', background);
.S
.fi
.ft R
.mc
.P 0
.H 4 "Defining Pixmap Values"
.P 0
.iX "UIL" "pixmap value"
.iX "UIL module" "pixmap value"
.iX "UIL specification file" "pixmap value"
.iX "UIL functions" "COLOR_TABLE"
.iX "UIL functions" "ICON"
.iX "Pixmaps" "defining in UIL"
Pixmap values let you specify labels that are graphic images
rather than text strings.  Pixmap values are not directly supported by UIL.
Instead, UIL supports icons, a simplified form of pixmap (which you
.mc |
define directly in UIL), or \fBxbitmap\fR
.mc
files (which you create outside UIL).
.P 0
You can generate pixmaps in UIL in two ways:
.BL
.LI
.mc |
Define an icon inline using the \fBICON\fR function (and optionally use the 
\fBCOLOR_TABLE\fR function to specify colors for the icon).   You
.mc
use a character to describe each pixel in the icon.
.LI
.mc |
Use the \fBXBITMAPFILE\fR function, specifying the name of an X bitmap file that 
.mc
you created outside UIL to be used as the pixmap value.
.LE
.P 0
The  following example shows the value
section in the Motifburger module containing a color table declaration.
.P 0
The colors you specify when defining a color table must have been previously
.mc |
defined with the \fBCOLOR\fR function.  For example,
.mc
the colors yellow and red were previously defined in the example in Section
.mc |
22.1.5.5.
.mc
Color tables must be private because the UIL compiler must be able to interpret
their contents at compilation time to construct an icon. The colors within a
color table, however, can be imported, exported, or private.
.mc |
.P
.ne 2i
.nf
.ft CW
.mc
value
.mc |
    button_ct   : color_table(
                  yellow='o'
                  ,red='.'
                  ,background color=' ');
.fi
.ft R
.mc
.P 0
The following example
shows how the
.mc |
button_ct
.mc
color table is
used to specify an icon pixmap.  Referring to the color table shown in
the previous example,
.mc |
each lowercase "o"
in the icon definition is replaced with the color yellow, and each . (dot) is
.mc
replaced with the color red.  Whatever color is defined as the background color
when the application is run replaces the spaces.
.P 0
.mc |
In UIL, if you define an argument of type pixmap, you should specify
an icon or an \fBxbitmap\fR file as its value.   For example, the icon defined in the
.mc
following example is given as the value of the label on the drink quantity 
push button. (Refer to the definition of the 
.mc |
drink_quantity
.mc
form
widget in
.mc |
Section 22.1.7.)
.P
.nf
.ft CW
.S -2
.mc
value
.mc |

 drink_up_icon: icon(color_table=button_ct,
.mc
                '                        ',
                ' ..........oo.......... ',
                ' .........oooo......... ',
                ' ........oooooo........ ',
                ' .......oo....oo....... ',
                ' ......oo......oo...... ',
                ' .....oo........oo..... ',
                ' ....oo..........oo.... ',
                ' ...oo............oo... ',
                ' ..oo..............oo.. ',
                ' .oo................oo. ',
                ' oooooooooooooooooooooo ',
                ' oooooooooooooooooooooo ',
                ' .........oooo......... ',
                ' .........oooo......... ',
                ' .........oooo......... ',
                ' .........oooo......... ',
                ' .........oooo......... ',
                ' .........oooo......... ',
                '                        ');
.mc |
.S
.fi
.ft R
.mc
.P 0
Each row in the icon must contain the same number of pixels and therefore must
contain the same number of characters.  The height of the icon is dictated by
.mc |
the number of rows. For example, the preceding arrow icon is 24 pixels wide
.mc
and 20 pixels tall. (The rows of spaces at the top and bottom of the pixmap and
the spaces at the start and end of each row are included in this count and are
defined as the background color in the
.mc |
button_ct
.mc
color table.
.P 0
A default color table is used if you omit the color table argument from
.mc |
the \fBICON\fR function.  
.mc
The
definition of the default color table is as follows:
.mc |
.P
.nf
.ft CW
.S -2
.mc
color_table( background color = ' ', foreground color = '*' 
.mc |
.S
.fi
.ft R
.mc
.P 0
You can specify icons as private, imported, or exported.
.P 0
.H 3 "Declaring Interface Objects in a UIL Module"
.P 0
.iX "UIL" "widget declaration"
.iX "UIL module" "widget declaration"
.iX "UIL specification file" "widget declaration"
.iX "Widget declaration"
Use an object declaration to define an instance of a widget or 
gadget that is to be stored
in the UID file. You can reference the object name in declarations that occur
elsewhere in the UIL module, usually to specify one object as a child of
another object. Some widgets accept a widget name as an argument.  This use of
a widget name is called a symbolic reference to a widget identifier and is explained
.mc |
in Section 19.10.
.mc
.mc *
.mc
.P 0
The object declaration contains a sequence of lists that define the arguments
(attributes), children, and callback functions for the object.  You can
specify only one list of each type for an object.
.P 0
Objects can be forward referenced; that is, you can declare an object name
after you refer to it.  This is useful for declaring a parent first, followed
by the declarations for all its children.  (The declaration of the parent
includes a list of the names of its children.)  In this way, the structure of
your UIL module resembles the widget tree of your interface. 
.P 0
All references to an object name must be consistent with the type you specified
when you declared the object.
As with values, you can specify an object as exported, imported, or
private.
.P 0
The following example
shows how the
.mc |
file_menu
.mc
widget is 
declared in the Motifburger UIL module.
.mc |
.P
.nf
.ft CW
.S -2
.in 0
.mc
object
.mc |
     file_menu : XmPulldownMenu {

         arguments {
             XmNlabelString = k_file_label_text;
         };
         controls {
             XmPushButton m_print_button;
             XmPushButton m_quit_button;
         };
         callbacks {
             MrmNcreateCallback = procedure create_proc (k_file_menu);
         };
     };
.in
.S
.fi
.ft R
.mc
.P 0
Note that the objects and values in this example have meaningful names 
(for example, 
\fIfile_menu\fP 
and 
\fIk_file_label_text\fP).  Using meaningful names helps
you recall the purpose of the object or value in the user interface.  (See
.mc |
Section 22.1.8 for a summary of recommended coding techniques.)
.mc
As shown in this example, a widget declaration generally
consists of three parts: an arguments list, a controls list, and a callbacks
list. These parts are explained in the following sections. 
.P 0
.H 4 "Specifying Arguments in an Object Declaration"
.P 0
Use an arguments list to specify the arguments (attributes) for an object. An
arguments list defines the arguments to be specified in the
\fIoverride_arglist\fP
argument when the creation function for a
particular object is called at run time.  An arguments list also
specifies the values that these arguments are to have.  You identify an
.mc |
arguments list to the UIL compiler by using the keyword \fBARGUMENTS\fR. 
.mc
.P 0
Each entry in the list consists of the argument name and the argument value. 
.mc |
In the previous example, the XmNlabelString argument for the \fIfile_menu\fP
pull-down menu is defined as \fIk_file_label_text\fP.  The value
\fIk_file_label_text\fP is a
.mc
compound string defined in a value section at the beginning of the module.
.P 0
If you use the same argument name more than once in an arguments list, the
last entry supersedes all previous entries, and the compiler issues a message. 
.P 0
.H 4 "Specifying Children in an Object Declaration"
.P 0
You use a controls list to define which widgets are children of, or controlled
by, a particular widget. The controls lists for all the widgets in a UIL module
define the widget tree for an interface. If you specify that a child is to
.mc |
be managed (the default), at run time the widget is
.mc
created and managed; if you specify that the child is to be unmanaged at
.mc |
creation (by including the keyword \fBUNMANAGED\fR in the controls list entry), the
.mc
widget is only created.  You identify a controls list to the UIL compiler by
.mc |
using the keyword \fBCONTROLS\fR.
.mc
.P 0
.mc |
In the previous example, the objects \fIm_print_button\fP
and \fIm_quit_button\fP are children of the \fIfile_menu\fP
widget, which is a pull-down menu.  The objects \fIm_print_button\fP
and \fIm_quit_button\fP are defined as push
buttons, which are valid children of the object type XmPulldownMenu. 
.mc
.P 0
.mc |
In the following example, the bulletin board dialog called \fIcontrol_box\fP
is a top-level composite widget, having a variety of
.mc
widgets as children.  Some of these children are also composite widgets, having
.mc |
children of their own.  For example, the \fIbutton_box\fP and
\fIburger_doneness_box\fP widgets are declared later on in the module,
.mc
and each of these has its own controls list. 
.mc |
.P
.nf
.ft CW
.S -2
.in 0
object           ! The control panel.  All order entry
                 ! is done through this dialog box.
  control_box : XmBulletinBoardDialog {
    arguments {
        XmNdialogTitle = k_motifburger_title;
        XmNdialogStyle = XmDIALOG_MODELESS;
        XmNx = 600;
        XmNy = 200;
        XmNmarginWidth = 20;
        XmNbackground = lightblue;
    };
    controls {
               ! Some labels and decoration.
        XmLabel           burger_label;
        XmLabel           fries_label;
        XmLabel           drink_label;
        XmSeparator       {arguments {
                            XmNx = 220;
                            XmNy = 20;
                            XmNunitType = XmPIXELS;
                            XmNorientation = XmVERTICAL;
                            XmNheight = 180; };};
        XmSeparator       {arguments {
                            XmNx = 410;
                            XmNy = 20;
                            XmNunitType = XmPIXELS;
                            XmNorientation = XmVERTICAL;
                            XmNheight = 180; };};
        XmRowColumn 	button_box; 
                ! Command push buttons inside a menu
                ! across the bottom.
                ! For the hamburger, fries, and drink
                ! entry we use a different mechanism
                ! to demonstrate various widgets and
                ! techniques.
                ! Hamburger 'doneness' uses a radio box
                ! because although it is a '1 of N' type
                ! of entry, one (and only one) entry
                ! is allowed.
        XmRadioBox       burger_doneness_box;
.in
.S
.fi
.ft R
.mc
.P 0
.mc |
.ne 2i
.mc
Notice that the separators are defined locally in the controls list for
\fIcontrol_box\fP,
rather than in object sections of their own.  As a
result, the separators do not have names and cannot be referenced by other
objects in this UIL module.  However, the local definitions make it easier for
someone reading the UIL specification file to tell that the separators are used
only by the
\fIcontrol_box\fP
widget.  When you define an object locally,
you do not need to create an artificial name for that object. 
.P 0
Unlike the arguments list (and the callbacks list, described in
the next section),
when you specify the same widget in a
controls list more than once, MRM creates 
multiple instances of the widget at
run time when it creates the parent widget.
.P 0
.H 4 "Specifying Callbacks in an Object Declaration"
.P 0
Use a callbacks list to define which callback reasons are to
be processed by a particular widget at application run time.
.mc |
As shown in the example in Section 22.1.7, each entry in a callbacks list
.mc
has a reason name (in this example, 
.mc |
XmNactivateCallback) 
.mc
and the name of a
callback function
(\fIactivate_proc\fP).
.P 0
For Motif Toolkit widgets, the reason names are already built into UIL.
For a user-defined widget, you can refer to a user-defined reason name
.mc |
that you previously specified by using the \fBREASON\fR function 
(see Section 18.5.14). 
.mc
If you use a built-in reason name in a widget
definition, the UIL compiler ensures that the reason name is supported by the
type of widget you are defining.
.P 0
If you use the same reason name more than once in a callbacks list, the last
entry that uses that reason name supersedes all others, and the UIL
compiler issues a message. 
.P 0
.mc |
The callback procedure names you use in a callbacks list must be
declared in a procedure section.
.mc
In this example, the procedure
\fIactivate_proc\fP
was declared in
the beginning of the UIL module.
.P 0
Because the UIL compiler produces a UID file rather than an object module, the
binding of the UIL name to the address of the function entry point is
not done by the linker.
Instead, the
binding is established at run time with 
the MRM function
.iX "MrmRegisterNames"
.mc |
\fBMrmRegisterNames\fR.
.mc
You call this function before fetching any widgets, giving it both the UIL
names and the function addresses of each callback.   The name you register
with MRM in the application program must match the 
name you specified in the
UIL module.  
.mc |
Section 22.2 
.mc
explains how the Motifburger callback
function names are registered with MRM.
.P 0
Each callback function receives three arguments.  The first two arguments
have the same form for each callback.  The form of the third argument varies
from widget to widget.
.P 0
The first argument is the address of the data structure
.mc |
maintained by the Motif Toolkit for this widget instance.  This address
.mc
is called the widget ID for this widget.
.P 0
The second argument is the address of the value you specified in the callbacks
list for this function.  If you do not specify an argument, the address
.mc |
is NULL.  This is called the 
.mc
\fBtag_value\fP 
argument.
If you specify a value type for the 
\fBtag_value\fP 
argument, this type
must match the value type of the parameter in the corresponding
.mc |
procedure declaration (see Table 22-2). 
.mc
.P 0
The third argument is a data structure specific to the widget.
The reason name you specified in the UIL
module is the first field of this data structure. 
.P 0
.H 3 "Using an Icon as a Widget Label"
.P 0
.iX "UIL module" "using an icon as a label"
.iX "UIL specification file" "using an icon as a label"
.iX "Icon" "using as a label"
.iX "Label" "specifying as an icon"
.mc |
Figure 22-1 highlights the drink quantity selector. This
.mc
widget in the user interface for the Motifburger application uses icons for the
.mc |
labels on its push buttons.  When the user clicks on the up-arrow icon, the
drink quantity increases.  When the user clicks on the down-arrow icon, the
.mc
drink quantity decreases.
.br
.ne 4.5i
\0
.mc *
.mc
.FG "Using an Icon in the Motifburger Application Interface"
.mc |
.sp 4.25i
.in +.19i
...\".P! graphics/icon.ps -4.25i 
.P! graphics/icon.ps
.in -.19i
.mc
.P 0
The icon named
\fIdrink_up_icon\fP
.mc |
was defined in the example in Section 22.1.5.6.
.mc
The following example
shows how to specify this icon
.mc |
as a push-button label in a widget declaration.  
.mc
In the Motifburger UIL module,
the icon named
.mc |
\fIdrink_up_icon\fR
.mc
is a pixmap label argument to the
.mc |
\fIup_value\fR
push-button widget. In turn, the
\fIup_value\fR
.mc
widget is controlled by the
.mc |
\fIdrink_quantity\fR
.mc
form
widget. 
.mc |
.P
.nf
.ft CW
.S -2
.in0
.mc
object
.mc |
  drink_quantity : XmForm {
    arguments {
        XmNx = 460;
        XmNy = 170;
        XmNunitType = XmPIXELS;
.mc
        };
.mc |
    controls {
        XmLabel           quantity_label;
        XmLabel           value_label;
        XmPushButton     up_value;
        XmPushButton     down_value;
        };
    };

.mc
\0\0\0.
\0\0\0.
\0\0\0.
.mc |

.mc
object
.mc |
  up_value : XmPushButton widget {
    arguments {
        XmNy = 00 ;
        XmNleftAttachment = XmATTACH_WIDGET;
        XmNleftOffset = 20 ;
        XmNleftWidget = XmLabel value_label ;
         XmNlabelType = XmPIXMAP;
        XmNlabelPixmap = drink_up_icon;
.mc
        };
.mc |
    callbacks {
        XmNactivateCallback = procedure activate_proc (k_drink_add);
        };
    };
.S
.in
.fi
.ft R
.mc
.P 0
.H 3 "Recommended Coding Techniques"
.P 0
.mc |
.iX "UIL module" "coding techniques"
.mc
.iX "UIL" "coding techniques"
.iX "UIL specification file" "coding techniques"
The Motifburger UIL module shows recommended coding 
practices that should
improve your productivity and increase the flexibility of your programs.  This
section explains how these practices can help you write better UIL modules.
The language elements and semantics of UIL are similar to those in other
high-level programming languages.
.P 0
.H 4 "Naming Values and Objects Meaningfully"
.P 0
.iX "UIL" "names"
.iX "Names" "in UIL"
.iX "UIL module" "names"
.iX "UIL names"
.iX "UIL specification file" "names"
The names of constants, labels, colors, icons, and widgets 
in the Motifburger
UIL module indicate their purpose in the application.  For example, the
name for the constant having integer value 12 is
\fIk_burger_rare\fP.
From its name, you can tell that this constant represents the choice
Rare on the Hamburgers menu.
.mc *
.mc
Similarly, the names for objects (widgets and gadgets)
indicate their purpose
.mc |
in the application. Object names should, in addition,
reflect the object type.
.mc
For example, you can tell by its name that 
.mc |
.ne 4
.mc
\fIm_copy_button\fP
is a
button of some kind
on a menu and is associated with the Copy option. 
.P 0
.H 4 "Grouping Value, Identifier, and Procedure Declarations"
.P 0
.iX "UIL module" "value declaration"
.iX "UIL specification file" "value declaration"
.iX "Value declaration" "UIL coding techniques"
.iX "UIL module" "procedure declaration"
.iX "UIL specification file" "procedure declaration"
.iX "Procedure declaration" "UIL coding techniques"
.iX "UIL module" "identifier declaration"
.iX "UIL specification file" "identifier declaration"
.iX "Identifier declaration" "UIL coding techniques"
You should
group value declarations according to purpose and list them near
.mc |
the beginning of the module.  Although you could have a value
.mc
section to declare a value immediately preceding an object section in which the
.mc |
value is used, you can look up the definition of a particular
.mc
value more easily if all declarations are in one place in the module. 
.mc *
.mc
In the Motifburger UIL module, separate value sections are used to group values 
as follows:
.BL
.LI
Constants for positioning within forms 
.LI
Constants for callback functions
.LI
Labels
.LI
Fonts
.LI
Colors
.LI
Color tables
.LI
Icons
.LE
.P 0
Constants for callback procedures must be defined in the
program as well as in the UIL module.  Therefore, if these 
constants are in a single value section, it is
easier to cut the section from the module and paste it into the
application program. 
.P 0
By setting up all labels
as compound string values, rather than hardcoding them in the
object declarations, you can more easily change the labels from one language to
another. Specify a string as a compound string by using the UIL built-in
.mc |
function \fBCOMPOUND_STRING\fR.
.mc
(Some arguments for the simple text widget
and the command window widget accept only null-terminated strings. 
Labels for these widgets must be declared as null-terminated (ASCIZ) strings, delimited with
single quotation marks.)
.P 0
The same technique applies to procedure declarations.  In the Motifburger UIL
module, all procedure declarations are listed in a single procedure
section at the beginning of the module, immediately following the module
declaration and include directive.
.mc *
.mc
The Motifburger application does not use identifiers (which function like global
.mc |
variables). Treat identifier sections as you treat
value sections.  Identifiers are described in Section 19.7.
.mc
.P 0
.H 4 "Ordering Object Declarations to Reflect the Widget Tree"
.P 0
.iX "UIL module" "widget declaration"
.iX "UIL module" "object declaration"
.iX "UIL specification file" "widget declaration"
.iX "UIL specification file" "object declaration"
.iX "Widget declaration" "UIL coding techniques"
.iX "Object declaration" "UIL coding techniques"
Once all your values, identifiers, and procedures are declared, the rest of
the UIL module consists of object declarations.  You should
structure your module to reflect the widget tree of
the application interface.  For example, in the Motifburger UIL module, the
choices for how the hamburger should be cooked are presented in a radio box
having three children, which are toggle buttons.
.mc |
The following figure shows how this radio box looks in the
.mc
Motifburger application interface. 
.br
.mc |
.ne 6i
.mc
\0
.mc *
.mc
.FG "Radio Box with Toggle Buttons in the Motifburger Application"
.mc |
.sp 4.25i
.in +.19i
...\".P! graphics/radio.ps -4.25i
.P! graphics/radio.ps
.in -.19i
.mc
.P 0
.mc |
The following figure shows how these widgets are arranged
.mc
in a hierarchy, which is defined by the controls list for the radio box
.mc |
named \fIburger_doneness_box\fP.
.mc
.br
.ne 3i
\0
.mc *
.mc
.FG "Widget Tree for the Motifburger Radio Box"
.mc |
.sp 2.75i
.in +.8i
...\".P! graphics/tree.ps -2.75i
.P! graphics/tree.ps
.in -.8i
.mc
.P 0
The following example shows the object declaration in the UIL module
.mc |
for the \fIburger_doneness_box\fP
.mc
widget.  Notice that the children of
the radio box (the three toggle button widgets named in the controls list)
are declared
immediately following the radio box declaration.  By ordering your object
declarations in this way, you can get an idea of the overall widget tree
for your interface by scanning the UIL module. 
.mc |
.P
.nf
.ft CW
.mc
object
.mc |
  burger_doneness_box : XmRadioBox {
      arguments {

.mc
\0\0\0.
\0\0\0.
\0\0\0.
.mc |

          };
      controls {
          XmToggleButton   burger_rare;
          XmToggleButton   burger_medium;
          XmToggleButton   burger_well;
          };
      };

.mc
object
.mc *
.mc
.br
.mc |
.ne 6
  burger_rare : XmToggleButton {

.mc
\0\0\0.
\0\0\0.
\0\0\0.
.mc |

      };

.mc
object
.mc |
  burger_medium : XmToggleButton {

.mc
\0\0\0.
\0\0\0.
\0\0\0.
.mc |

      };

.mc
object
.mc |
  burger_well : XmToggleButton {
.mc
\0\0\0.
\0\0\0.
\0\0\0.
.mc |
      };
.fi
.ft R
.mc
.P 0
.H 4 "Using Local Definitions for Certain Objects"
.P 0
If you need to define an object that is used as a child of a single parent and
.mc |
is not be referred to by any other object in the UIL module, define the
.mc
object in the controls list for its parent rather than in an object section of
its own. This simplifies the UIL module and saves you from having to create an
.mc |
artificial name for that object.  The example in Section 22.1.6.2 shows a
.mc
separator defined locally.
.P 0
.mc |
.H 2 "Creating a User Interface at Run Time with \s-1MRM\s0"
.mc
.P 0
.iX "MRM" "creating interface"
.mc |
MRM creates interface objects based on
.mc
definitions in UID files, which are the compiled form of UIL
specification files.  You call MRM functions 
in your application to
initialize MRM, to provide information 
required by MRM to interpret information
in UID files, and to create objects using UID file definitions.  
.P 0
MRM allows you to defer fetching top-level objects until the application needs
to display them.  By deferring fetching, you can improve the start-up 
.mc |
performance of your application.  Section 22.2.2 explains how to defer fetching.
.mc
.P 0
MRM also has functions that allow an 
application to read literal definitions
.mc |
from UID files.  You create these literal definitions when you declare a
literal value
.mc
to be exported in UIL.  You can use these literals in your
.mc |
application program for any purpose.  Section 22.2.3 explains how
.mc
to read literals from UID files.
.P 0
You can use an MRM function to override 
values you specified for widget
attributes.  In effect, a single object definition can be used like a template
to create multiple widget instances from a single UIL definition.
.mc |
Section 22.2.4 describes this function.
All definitions required to use MRM are contained in the 
\fBMrmAppl.h\fR file.
.mc
.P 0
.mc |
MRM does not replace the X 
.mc
Resource Manager, but complements it. The 
X Resource
.mc |
Database (an in-memory database, stored in the \fB.Xdefaults\fR
.mc
file) supplies
default values. When you use UIL to specify a user interface, you do not need
to specify all argument values (resources); you need only specify an argument
when you want to override the default value stored in the X Resource Database. 
.mc |
The MRM generates the \fIoverride_arglist\fP argument for the appropriate
.mc
widget creation functions at run time.
.P 0
.mc |
Figure 22-4 shows how widget argument values are applied
inside the MRM fetch operation.
.mc
.br
.ne 8i
\0
.mc *
.mc
.FG "Widget Creation in an MRM Fetch Operation"
.mc |
.sp 7.5i
.in +.19i
...\".P! graphics/mrmfetch.ps -7.5i
.P! graphics/mrmfetch.ps
.in -.19i
.mc
.sp
.P 0
The examples in this section are based on the C program for the Motifburger
application.  The Motifburger application is part of the Motif software
.mc |
kit.  Section 22.1 explains how you can access the source files for
.mc
this application.  The Motifburger application demonstrates the most commonly
.mc |
used MRM functions.  The following table briefly 
.mc
describes the available MRM
functions.
.mc |
See Chapter 21 for a complete description of these functions.
.mc
.iX "MRM" "functions table"
.mc |
.P
.mc
.TB "MRM Functions " 
.TS H 
tab(@);
lB lB
lw(2i) lw(2.75i).
.mc
_
.sp 6p
Function Name@Description
.sp 6p
_
.sp 6p
.TH
.mc |
\fBMrmCloseHierarchy\fR@Closes a UID hierarchy
.mc
.sp
.mc |
\fBMrmFetchColorLiteral\fR@T{
.mc
Fetches a named color literal from a UID hierarchy
T}
.sp
.mc |
\fBMrmFetchIconLiteral\fR@Fetches a named icon from a UID hierarchy
.mc
.sp
.mc |
\fBMrmFetchInterfaceModule\fR@T{
.mc
Fetches all the objects defined in some
interface module in the UID hierarchy
T}
.sp
.mc |
\fBMrmFetchLiteral\fR@T{
.mc
Fetches a named string literal from a UID hierarchy
T}
.sp
.mc |
\fBMrmFetchSetValues\fR@T{
.mc
Fetches the values to be set from literals stored
in a UID hierarchy
T}
.sp
.mc |
\fBMrmFetchWidget\fR@T{
.mc
Fetches any named widget in a UID hierarchy
T}
.sp
.mc |
\fBMrmFetchWidgetOverride\fR@T{
.mc
Fetches any named widget and
overrides values stored in the UID hierarchy with those supplied 
in the function call
T}
.sp
.mc |
\fBMrmInitialize\fR@T{
.mc
Prepares an application to use MRM widget fetching
facilities
T}
.sp
.mc |
.ne 3
\fBMrmOpenHierarchy\fR@T{
.mc
Allocates a hierarchy descriptor and opens all the
files in the UID hierarchy
T}
.sp
.mc |
\fBMrmRegisterClass\fR@T{
.mc
Saves the information needed to access the widget
creation function using the information in a UID hierarchy and to perform type
conversion of arguments lists
T}
.sp
.mc |
\fBMrmRegisterNames\fR@T{
.mc
Registers a vector of names and associated
.mc |
values for access by MRM in the global namespace
.mc
T}
.mc |
.sp
\fBMrmRegisterNamesInHierarchy\fR@T{
Registers a vector of names and associated
values for access by MRM in a namespace
attached to a particular UIL hierarchy
T}
.mc
.sp 6p
_
.TE
.P 0
.H 3 "Accessing the UID File at Run Time"
.mc |
As Figure 22-5 shows, setting up an interface specified
.mc
with UIL requires the following steps: 
.AL 1
.LI 
.mc |
\fBInitialization\fR
.mc
.iX "Initialization" "user interface"
.iX "Initialization" "Motif Toolkit"
.iX "Initialization" "MRM"
.iX "User interface" "initialization"
.iX "Motif" "initialization"
.iX "MRM" "initialization"
.mc |
.sp .5
.mc
In the initialization step, the application program must 
make calls to the MRM
and Motif Toolkit intrinsics functions in the following sequence:
.BL
.LI
Initialize MRM.
.mc |
.LE
.sp .5
.mc
The MRM function 
.iX "MrmInitialize"
.mc |
\fBMrmInitialize\fR
.mc
prepares your application to use
MRM widget-fetching facilities.  This call must come before the
call to initialize the Motif Toolkit.
.mc |
.BL
.mc
.LI
Initialize the Motif Toolkit.
.mc |
.LE
.sp .5
.mc
The intrinsics function
.mc |
.iX "XtAppInitialize"
\fBXtAppInitialize\fR parses the command line used to 
invoke the application, opens the display, and initializes the Motif Toolkit.
.BL
.mc
.LI
Open the UID hierarchy.
.mc |
.LE
.sp .5
.mc
The UID hierarchy is the set of UID
files containing the widget definitions for the user interface.
The MRM function
.iX "MrmOpenHierarchy"
.mc |
\fBMrmOpenHierarchy\fR
.mc
opens these UID files.
.mc |
.BL
.mc
.LI
Register names for MRM.
.mc |
.LE
.sp .5
The MRM functions
.mc
.iX "MrmRegisterNames"
.mc |
.iX "MrmRegisterNamesInHierarchy"
\fBMrmRegisterNames\fR and \fBMrmRegisterNamesInHierarchy\fR register names and associated
values for access by the MRM.  The values may be callback functions,
pointers to user-defined data, or any other values.  MRM uses this 
.mc
information to resolve symbolic references in UID files to their 
run-time values.
.mc *
.mc
.LI
.mc |
\fBCreation\fR
.mc
.iX "Creation" "user interface"
.iX "User interface" "creation"
.iX "MRM" "fetch"
.mc |
.sp .5
.mc
In the creation step, you call the MRM function
.iX "MrmFetchWidget"
.mc |
\fBMrmFetchWidget\fR
.mc
to fetch
the user interface.  Fetching is a combination of widget creation and child
management.  The MRM function
.iX "MrmFetchWidget"
.mc |
\fBMrmFetchWidget\fR
.mc
performs the following tasks:
.BL
.LI
Locates a widget description in the UID hierarchy
.LI
Creates the widget and recursively creates the widget's children
.LI
Manages all children as specified in the UID hierarchy
.LI
Returns the widget identifier
.LE
.mc |
.sp .5
.mc
You specify the top-level widget of the application (usually the main window)
and its parent (the widget identifier returned by the call to 
.mc |
.iX "XtAppInitialize"
\fBXtAppInitialize\fR) in the call to 
.mc
.iX "MrmFetchWidget"
.mc |
\fBMrmFetchWidget\fR.  As a result of this single call, MRM fetches all widgets in
.mc
the widget tree below the top-level widget.
.mc |
You can defer fetching portions of an application interface until
.mc
they are requested by the end user.  For example, you can 
defer fetching a pull-down menu
until the user activates the corresponding cascade button.  Consider
deferring fetching of some portions of your interface if you need to improve
.mc |
the start-up performance of your application.  Deferred fetching is explained in
Section 22.2.2.
.br
.ne 2i
.mc
.LI
.mc |
\fBRealization\fR
.mc
.iX "Realization" "user interface"
.iX "User interface" "realization"
.mc |
.sp .5
.mc
The steps to manage and realize a user interface created using 
UIL and MRM are
the same as those for an interface created with Motif Toolkit
functions:
.BL
.LI
Manage the top-level widget.
.mc |
.LE
.sp .5
.mc
The intrinsics function
.iX "XtManageChild"
.mc |
\fBXtManageChild\fR
adds a child to the top-level widget returned by the call to 
.iX "XtAppInitialize"
\fBXtAppInitialize\fR.  The entire widget tree
.mc
below the top-level widget in the interface (usually the main window) is
automatically managed as a result of this call to
.iX "XtManageChild"
.mc |
\fBXtManageChild\fR.
.BL
.mc
.LI
Realize the top-level widget.
.mc |
.LE
.sp .5
.mc
The intrinsics function
.iX "XtRealizeWidget"
.mc |
\fBXtRealizeWidget\fR displays the entire interface (the
.mc
widget tree below the top-level widget) on the screen. 
.mc |
.sp .5     
.mc
.br
.ne 4.25i
\0
.mc *
.mc
.FG "Setting Up a User Interface Specified with UIL"
.mc |
.sp 4i
.in -.5i
...\".P! graphics/setup.ps -4i
.P! graphics/setup.ps
.in +.5i
.mc
.LE
.mc |
.br
.ne 3i
.mc
The role of MRM in a Motif application 
is limited primarily to widget creation.
MRM makes run-time calls that 
create widgets from essentially invariant
information (information that does not change from one invocation of
the application to the next).  After MRM 
fetches a widget (creates it and
manages its children), it provides no further services.  All subsequent
operations on the widget, such as realization, managing and 
unmanaging children after
initialization, and getting and setting resource values, must 
be done by run-time
calls. After widget creation, you modify widgets during 
application execution by 
using widget manipulation functions. 
.iX "User interface" "specifying using UIL"
.iX "Motifburger"
.P 0
.iX "MRM" "MrmInitialize function"
The call to the 
.iX "MrmInitialize"
.mc |
\fBMrmInitialize\fR
.mc
function must come before the call to the
.mc |
.iX "XtAppInitialize"
\fBXtAppInitialize\fR
.mc
function.  The following example shows the 
initialization of MRM
and the Motif Toolkit in the Motifburger application.
.mc |
.P
.nf
.ft CW
.S -2
.mc
unsigned int main(argc, argv)
.mc |
  unsigned int argc;                  
  char *argv[];                       
.mc
{
.mc |
  Widget toplevel_widget;
  XtAppContext app_context;
  MrmInitialize();                 
.sp .5
  toplevel_widget = XtAppInitialize(&app_context, "example",
                      NULL, 0, &argc, argv, NULL, NULL, 0);
.S
.fi
.ft R
.mc
.P 0
The compiled interface, described in one or more UIL modules, is connected to
the application when the UID hierarchy is set up at run 
time.  The names of the
UID files containing the compiled interface definitions are stored in an array. 
Because compiled UIL files are not object files, this run-time
connection is necessary to bind an interface with an application program. The
Motifburger application has a single UIL module,
so the MRM
hierarchy consists of one file.
The following example shows the declaration of the UID hierarchy
for Motifburger.
.mc |
.P
.nf
.ft CW
.mc
static MrmHierarchy s_MrmHierarchy;
static MrmType *dummy_class;       
static char *db_filename_vec[] =   
  {"motifburger.uid"                 
  };                               
.mc |
.fi
.ft R
.mc
.P 0
.iX "MRM" "MrmOpenHierarchy function"
.mc |
.ne 5
The name of the UID hierarchy is \fIs_MrmHierarchy\fP.  The array
.mc
containing the names of the UID files in the UID hierarchy is
.mc |
\fIdb_filename_vec\fP.  In the following example, the
.mc
application opens this UID hierarchy.  At this point 
in the application's execution, MRM
has access to the Motifburger interface definition and can fetch widgets.
.mc |
.P
.nf
.ft CW
if (MrmOpenHierarchy(db_filename_num,
  db_filename_vec,                   
  NULL,                              
  &s_MrmHierarchy)                   
  !=MrmSUCCESS)
  s_error("can't open hierarchy");
.fi
.ft R
.mc
.P 0
.iX "MRM" "registering names"
The final step in preparing to use MRM to fetch 
widgets is to register a vector
of names and associated values.  These values can be the names of callback
functions, pointers to user-defined data, or any other 
values.  MRM uses the
information provided in this vector to resolve symbolic references that occur
in UID files to their run-time values.   For callback procedures, the vector
provides procedure addresses required by the Motif Toolkit.  For names
used as variables in UIL (identifiers), this information provides whatever
mapping the application requires.  The use of identifiers is explained in
.mc |
Section 19.7.
.mc
The following example shows the declaration of the names vector in the
Motifburger C program.  In the Motifburger application, the names vector 
contains only names of callback procedures and their addresses.
.mc |
.P
.nf
.ft CW
.S -2
.mc
static MRMRegisterArg reglist[] = {
    {"activate_proc", (caddr_t) activate_proc}, 
    {"create_proc", (caddr_t) create_proc}, 
    {"list_proc", (caddr_t) list_proc}, 
    {"pull_proc", (caddr_t) pull_proc}, 
    {"quit_proc", (caddr_t) quit_proc}, 
    {"scale_proc", (caddr_t) scale_proc}, 
    {"show_hide_proc", (caddr_t) show_hide_proc}, 
    {"show_label_proc", (caddr_t) show_label_proc}, 
    {"toggle_proc", (caddr_t) toggle_proc}
};
.mc |
.sp .5
.mc
static int reglist_num = (sizeof reglist / sizeof reglist [0]);
.mc |
.S
.fi
.ft R
.mc
.P 0
.iX "MRM" "MrmRegisterNames function"
The names are registered in a call to the 
.iX "MrmRegisterNames"
.mc |
\fBMrmRegisterNames\fR function, as shown in the following example:
.P
.nf
.ft CW
MrmRegisterNames(reglist, reglist_num);
.fi
.ft R
.br
.ne 3i
.mc
.H 3 "Deferring Fetching"
.P 0
.iX "UIL" "setting up for deferred fetching"
.iX "MRM" "deferred fetching"
.iX "Fetch" "deferring"
MRM allows you to defer fetching off-screen 
widgets until the application needs
to display these widgets. There are two types of off-screen widgets:  
pull-down menus and dialogs.
Whenever MRM fetches an off-screen widget, 
it also fetches the
entire widget tree below that widget.  By deferring the fetching of
.mc |
off-screen widgets, you can reduce the time taken to start up your
.mc
application.
.P 0
The Motifburger application makes use of deferred fetching.  The pull-down menus
for the File, Edit, and Order options are not fetched when the main window is
fetched.  Instead, these menus are fetched and created by individual calls to
the 
.iX "MrmFetchWidget"
.mc |
\fBMrmFetchWidget\fR
.mc
function when the corresponding cascade button is
activated (selected by the end user).  You can use the 
.iX "MrmFetchWidget"
.mc |
\fBMrmFetchWidget\fR
.mc
function
at
.mc |
any time to fetch a widget that was not fetched at application startup.
.mc
.P 0
The UIL module for the Motifburger application is set up to allow either deferred
fetching or a single fetch to create the entire widget tree.  To fetch the
entire interface at once, remove the comment character (!) from the controls
.mc |
list for the \fIfile_menu_entry\fP, \fIedit_menu_entry\fP, and
\fIorder_menu_entry\fP widgets.  As long as the comment characters remain
on the controls list for the pull-down menu entries, their associated pull-down
.mc
menus are no longer children; they are top-level widgets and can be fetched
individually.  The following example shows the object declaration for
.mc |
the XmCascadeButton named \fIfile_menu_entry\fP.
.P
.nf
.ft CW
.S -2
.in 0
.mc
object
.mc |
  file_menu_entry : XmCascadeButton {

      arguments {
          XmNlabelString = k_file_label_text;
      };
       controls {
           XmPulldownMenu file_menu;
       };
      callbacks {
          XmNcascadingCallback = procedure pull_proc (k_file_pdme);
          MrmNcreateCallback = procedure create_proc (k_file_pdme);
      };
  };
.S
.in
.fi
.ft R
.mc
.P 0
.mc |
.br
.ne 4
.mc
When you remove the comment characters, the controls list on each
XmCascadeButton specifies the pull-down menu as a child. The pull-down menus are
no longer top-level widgets; instead, they are loaded when the XmCascadeButton
is created.
.P 0
.H 3 "Getting Literal Values from UID Files"
.P 0
.iX "MRM" "Getting literal values from UID files"
.iX "Literals" "getting from UID files"
.iX "UIL value" "getting from UID files"
.mc |
Using the literal fetching functions
(\fBMrmFetchColorLiteral\fR,
\fBMrmFetchIconLiteral\fR,
.mc
and 
.mc |
\fBMrmFetchLiteral\fR),
.mc
you can retrieve any named, exported UIL value
.mc |
from a UID file at run time.  This is useful when you want to use a
literal value in a
context other than fetching an object. These functions allow you to 
.mc
treat the UID
file as a repository for all the programming variables you need to specify your
application interface. 
.P 0
The MRM literal fetching functions have
a wide variety of uses.  For 
example,
you can store the following as named, exported literals in a UIL module for
run-time retrieval:
.BL
.LI
All the error messages to be displayed in a message box (stored in a 
string table)
.LI
All string tables used to query the operating system
.LI
Language-dependent strings
.LE
.P 0
.mc |
In the C program for the Motifburger application, the text string
.mc
displayed in the title bar of the main window is supplied directly to the
.mc |
.iX "XtAppInitialize"
\fBXtAppInitialize\fR
function, as shown in the following example:
.P
.nf
.ft CW
.S -2
  toplevel_widget = XtAppInitialize(&app_context, "example",
                      NULL, 0, &argc, argv, NULL, NULL, 0);
.S
.fi
.ft R
.mc
.P 0
Alternatively, this string could be specified in a UIL module as a named,
exported compound string, and retrieved from the UID file at run time with the 
.mc |
\fBMrmFetchLiteral\fR
.mc
function.  Since
this string appears in the interface, you should declare it as a compound
string.  Compound strings can be displayed in a variety of character sets, as
required by the language of the interface.
.H 3 "Setting Values at Run Time Using UID Resources"
.iX "MRM (Motif Resource Manager)" "MrmFetchSetValues function"
.iX "MrmFetchSetValues function"
.iX "Functions" "MrmFetchSetValues"
.iX "UIL (User Interface Language)" "modifying objects at run time"
.iX "Object" "modifying at run time"
.P 0
The MRM function 
.mc |
\fBMrmFetchSetValues\fR
.mc
allows you to modify at run time an object
that has already been created. The 
.mc |
\fBMrmFetchSetValues\fR
.mc
function works like the 
.mc |
\fBXtSetValues\fR
.mc
function except that MRM fetches the values to be set from named,
.mc |
exported values (literals) in the UID file. The fetched values are converted
.mc
to the correct data type, if necessary, and placed in the 
.mc |
\fIargs\fP argument for a call to the function \fBXtSetValues\fR.
Since the \fBMrmFetchSetValues\fR
.mc
function looks for the literal values in a UID file, the argument names
.mc |
you provide to the \fBMrmFetchSetValues\fR function must be UIL argument names (not
.mc
Motif Toolkit attribute names). 
.P 0
.mc |
You can think of \fBMrmFetchSetValues\fR as a convenience function that
packages the functions provided by \fBMrmFetchLiteral\fR and \fBXtSetValues\fR.
.mc
.P 0
The value member of the name and value pairs passed to
.mc |
\fBMrmFetchSetValues\fR
.mc
is the UIL name of the value, not an explicit value.  
When the
application calls 
.mc |
\fBMrmFetchSetValues\fR,
.mc
MRM looks up the names in the UID file,
then uses the values corresponding to those names to override the original
.mc |
values in the object declaration.  Therefore, the 
\fBMrmFetchSetValues\fR function
.mc
allows you to keep all values used in an application in the UIL module and not
in the application program.  
(The values you pass to the 
.mc |
\fBMrmFetchSetValues\fR
.mc
function must be named, exported literals in the UIL module.)
.P 0
The 
.mc |
\fBMrmFetchSetValues\fR
.mc
function offers the following advantages:
.BL
.LI
It performs all the necessary UIL resource manipulation to make the fetched
UIL values usable by the Motif Toolkit.  (For example, the 
.mc |
\fBMrmFetchSetValues\fR
.mc
function performs address recomputation for tables of strings and enables a 
UIL icon to act as a pixmap.)
.LI
It lets you isolate a greater amount of interface information from the 
.mc |
application program, to achieve further separation of form and function.
.mc
.LE
.P 0
There are some limitations to the 
.mc |
\fBMrmFetchSetValues\fR
.mc
function:
.BL
.LI
.mc |
All values in the \fIargs\fP argument must be names
.mc
of exported resources listed in a UIL module (UID hierarchy); therefore,
.mc |
the application cannot provide computed values from
.mc
within the program itself as part of the argument list.
.LI
.mc |
It uses the \fBXtSetValues\fR
.mc
function, ignoring the possibility of the less 
costly high-level function that the widget itself may provide.
.LE
.P 0
The examples in this section are based on a simple application that displays
text in two list widgets.  The text displayed in the second list widget
depends on what the user selected in the first. 
.mc |
Figure 22-6 shows the interface for this application.
.mc
.br
.ne 3.75i
\0
.mc *
.mc
.FG "Sample Application Using the MrmFetchSetValues Function"
.mc |
.sp 3.5i
.in +.31i
...\".P! graphics/application.ps -3.5i
.P! graphics/application.ps
.in -.31i
.mc
.P 0
This application is well-suited to using the MRM function 
.mc |
\fBMrmFetchSetValues\fR
.mc
for
the following reasons:
.BL
.LI
The data (list widget contents) are all known in advance; the
values themselves do not need to be computed at run time.
.LI
.mc |
The data consists of tables of compound strings that appear in
.mc
the user interface and, therefore, must be translated for
international markets.  (Strings that must be translated should be stored in a
UID file.)
.LI
The structure of compound string tables, if retrieved from the UID file
using the 
.mc |
\fBMrmFetchLiteral\fR
.mc
function, must be modified by the application
program due to the nature of MRM storage methods.  (Since string tables are
.mc |
stored in contiguous blocks, indexes to string-table values are offsets, not
.mc
true addresses, and must be added together to compute the actual address at run
time.)  The 
.mc |
\fBMrmFetchSetValues\fR
.mc
function performs this address computation
.mc |
automatically and deallocates memory used to
store the fetched tables.  Since
the program does not use the fetched
.ne 3
string table directly, but intends
.mc
only to modify the visual appearance of a widget based on items in the table,
the 
.mc |
\fBMrmFetchLiteral\fR
.mc
function is less convenient to use.
.LE
.P 0
The following examples show the UIL module for this
application and excerpts from the C program. The
segment of the UIL module shown 
assumes that the module header, procedure declarations, include files, and
value declarations for each of the names used in the example are in
place.
.mc |
.P
.nf
.ft CW
.S -2
.in0
.mc
value
(1) cs_wood     : compound_string("Wood");
    cst_materials_selected : string_table(cs_wood);
(2) cst_materials : exported string_table(
.mc |
              cs_wood,     ! material type 1
              "Metal",     ! material type 2
              "Waste");    ! material type 3
(3)  cst_type_1  : exported string_table( ! Materials for type 1 (wood)
              "Redwood","Dogwood","Birch","Pine","Cherry");
    l_count_type_1 : exported 5;
    cst_type_2 : exported string_table(   ! Materials for type 2 (metal)
        "Aluminum","Steel","Titanium","Iron","Linoleum");
    l_count_type_2 : exported 5;
    cst_type_3 : exported string_table(   ! Materials for type 3 (waste)
               "Toxic","Solid","Biodegradable","Party Platforms");
    l_count_type_3 : exported 4;
    k_zero : exported 0;
.mc
 object
.mc |
    materials_ListBox  : XmList
    {
        arguments
        {
            XmNx = k_tst_materials_ListBox_x;
            XmNy = k_tst_materials_ListBox_y;
            XmNwidth = k_tst_materials_ListBox_wid;
            XmNvisibleItemCount = 4;
            XmNitems = cst_materials;
            XmNselectedItems = cst_materials_selected;
        };
        callbacks
        {
            MrmNcreateCallback = 
              procedure tst_create_proc(k_tst_materials_ListBox);
            XmNsingleSelectionCallback = 
              procedure tst_single_proc(k_tst_materials_ListBox);
        };
    }; 
    types_ListBox      : XmList
    {
        arguments
        {
            XmNx = k_tst_types_ListBox_x;
            XmNy = k_tst_types_ListBox_y;
            XmNwidth = k_tst_types_ListBox_wid;
            XmNvisibleItemCount = 4;
            XmNitems = cst_type_1;
        };
        callbacks
        {
            MrmNcreateCallback = 
              procedure tst_create_proc(k_tst_types_ListBox);
            XmNsingleSelectionCallback = 
              procedure tst_single_proc(k_tst_types_ListBox);
        };
    };
.in
.S
.fi
.ft R
.br
.ne 8
.P
\fBNOTES:\fR
.mc
.AL 1
.LI 
Prefixes on value names indicate the type of value.  For example,
.mc |
\fIcs_\fP means compound string, \fIcst_\fP means compound string table, and 
\fIl_\fP means long integer.
.mc
.LI
This string table provides the contents for the Materials list box widget
.mc |
(on the left in Figure 22-6).  This string table does
.mc
not need to follow the naming scheme for the string table in the Material Types
.mc |
list widget (that is, \fIcst_type_n\fP) because the contents of the
.mc
Materials list does not change once the application is realized.  (The
numbering of the string tables (3)) is vital to the proper
functioning of the Material Types list widget.  The string table for the
Materials list box widget could have been named anything.)
.LI
These string tables provide the contents for the various versions of the
.mc |
Materials Types list box widget (on the right in Figure 22-6).
.mc
Each one of these lists of strings
corresponds (in order) to the string names in the first list widget
(Materials).  These tables are numbered to facilitate programming.  When the
user selects an item in the Materials list widget, the index of the
.mc |
selected item is concatenated with the string \fIcst_type_\fR to form the name
of one of these tables. This named table is retrieved with the
\fBMrmFetchSetValues\fR
.mc
function and placed in the Materials Type list widget.  
.P 0
.mc |
.br
.ne 6
.mc
Note that in addition to the string table, a count of the number of items in
the table is declared as an exported value.  This is done because using the 
.mc |
\fBXtSetValues\fR
.mc
function on a list widget requires that three arguments 
be set: 
.mc |
\fBXmNitems\fR,
\fBXmNitemCount\fR,
.mc
and
.mc |
\fBXmNselectedItemsCount\fR
(which must be set to 0).
.mc
.LE
.P 0
In the following C program segment, note the activation
.mc |
function named \fItst_single_proc\fP, where the user's selection causes
.mc
the program to act.
.mc |
.P
.nf
.ft CW
.in0
.S -3
.mc
 #define k_zero_name  "k_zero"
 #define k_table_name_prefix  "cst_type_"
 #define k_table_count_name_prefix  "l_count_type_"
(1) void tst_single_proc(w,object_index,callbackdata)
    Widget      w;
    int         *object_index;
    XmListCallbackStruct    *callbackdata;
 {
(2) char *t_number;
(3) char t_table_name[32] = k_table_name_prefix;
    char t_table_count_name[32] = k_table_count_name_prefix;
(4) Arg r_override_arguments[3] =
    {{XmNitems,NULL},{XmNitemsCount,NULL},{XmNselectedItemsCount,k_zero_name}};
   switch (*object_index)
   {
(5)     case    k_tst_materials_ListBox:
     {
(6)         sprintf(&t_number,"%d",callbackdata->item_number);
(7)         strcpy(&t_table_name[sizeof(k_table_name_prefix)-1],&t_number);
         XtSetArg(r_override_arguments[0],XmNitems,&t_table_name);
.mc |
(8)         strcpy(&t_table_count_name[sizeof(k_table_count_name_prefix)-1],
                                                                   &t_number);
.mc
         XtSetArg(r_override_arguments[1],XmNitemsCount,&t_table_count_name);
(9)         MrmFetchSetValues(ar_MRMHierarchy,
             object_ids[k_tst_types_ListBox],        
             r_override_arguments,3);              
         break;
       };
(10)      case    k_tst_types_ListBox:
       {
.mc *
.mc
\0\0\0.
\0\0\0.
\0\0\0.
.mc *
.mc
             break;
       };
   };
 };
.mc |
.in
.S
.fi
.ft R
.P
.ne 3i
\fBNOTES:\fR
.mc
.AL 1
.LI 
Function that handles the 
.mc |
XmNsingleSelectionCallback
.mc
callback functions for any
.mc |
object. When the user selects an item in a list widget, the contents of the
neighboring list widget are replaced.  This function uses the list 
.mc
widget callback structure named 
.mc |
\fBXmListCallbackStruct\fR.  
.mc
This structure
contains the following fields: reason, event, item, item_length, and
item_number.
.LI
Used to form the string version of the item number.
.LI
Local character storage.
.LI
Override argument list for the 
.mc |
\fBMrmFetchSetValues\fR
.mc
function.
.LI
.mc |
User has selected an item from the Materials list widget. The 
.mc
application needs to place a new items list in the Types list widget. 
The string tables stored in the UID file are 
.mc |
named \fIcst_type_"index number"\fP and their count
names are \fIl_count_type_"index number"\fP  (where \fIindex number\fR
.mc
corresponds to the item's position in the list widget). Using the index of the
selected item from this list, the application forms the name of the appropriate
compound string table.
.mc |
.sp
.mc
Using the item number instead of the text value of the selection separates the
function of the application from the form (in this case, the contents of the
list widgets) and reduces complexity.  If the program used the text value
.mc |
of the selected item as the means to determine what to display, it must
deal with possible invalid characters for a UIL name in the text and 
convert the text value (a compound string) to a null-terminated string
.mc
so that the string could be passed to the function 
.mc |
\fBXtSetValues\fR.
.mc
.LI
.mc |
Used to form the string version of the item number.
.mc
.LI
.mc |
Used to form the name of the string table.
.mc
.LI
.mc |
Used to form the name of the string table count.
.mc
.LI
.mc |
Used to fill the types list widget with a new list of items.
.mc
.LI
Similar selection recording code goes here.
.LE
.mc |
.br
.ne 3i
.mc
.H 3 "Using an Object Definition as a Template"
.P 0
.iX "MRM" "MrmFetchWidgetOverride function"
.iX "MrmFetchWidgetOverride function"
.iX "UIL" "using object declaration as a template"
.iX "Object declaration" "using as a template"
.mc |
.iX "Declaring objects in UIL" "using as template"
.mc
The
.iX "MrmFetchWidgetOverride"
.mc |
\fBMrmFetchWidgetOverride\fR
.mc
function is useful if you have to 
.mc |
create several similar widgets.
Consider an application interface that has a lot
of push buttons contained in a bulletin board.  The push buttons are the
.mc
same except for their 
.mc |
\fBXmNy\fR
.mc
position and label.  
Instead of declaring
each push button individually, you can declare one
push button and use the MRM
function
.iX "MrmFetchWidgetOverride"
.mc |
\fBMrmFetchWidgetOverride\fR
.mc
to use that declaration as a 
template, modifying
the 
.mc |
\fBXmNy\fR
.mc
position and label at run time.
.P 0
By calling the 
.iX "MrmFetchWidgetOverride"
.mc |
\fBMrmFetchWidgetOverride\fR
.mc
function instead of the
.iX "MrmFetchWidget"
.mc |
\fBMrmFetchWidget\fR
.mc
function, the application program creates the widget and 
overrides the original
values in the declaration with the values 
specified in the 
.iX "MrmFetchWidgetOverride"
.mc |
\fBMrmFetchWidgetOverride\fR
.mc
call.  The argument list you supply to the 
.iX "MrmFetchWidgetOverride"
.mc |
\fBMrmFetchWidgetOverride\fR
.mc
function must contain Motif Toolkit attribute names; therefore, it is
also possible to override the callbacks for an 
object using the 
.iX "MrmFetchWidgetOverride"
.mc |
\fBMrmFetchWidgetOverride\fR
function, since callbacks are Motif Toolkit attributes.
.mc
.P 0
To use the 
.iX "MrmFetchWidgetOverride"
.mc |
\fBMrmFetchWidgetOverride\fR
.mc
function in an application, you 
should use UIL
identifiers to specify the tag value for each callback procedure.  The tag is
specified in the callback structure and cannot be changed unless the callback is
deleted and replaced.  The callback structure is not stored in the widget data,
but by the X Toolkit intrinsics. 
.P 0
If you do not use identifiers for tag values, your callback procedures must
contain a check for the parent of the calling widget or some other field of the
widget (as opposed to checking only the tag value) because it is not possible
to override the tag value with the 
.iX "MrmFetchWidgetOverride"
.mc |
\fBMrmFetchWidgetOverride\fR
.mc
function. If 
you do not
use an identifier for the tag value, all instances of the fetched object return
identical tag values for all callbacks.  If the callback function checks only
the tag value,  the callback function could not distinguish 
which instance made
.mc |
the call.  Section 19.7 explains how to use UIL identifiers.
.mc
.P 0
.iX "UIL" "using Help widget"
.iX "Help widget"
Another practical use of the 
.iX "MrmFetchWidgetOverride"
.mc |
\fBMrmFetchWidgetOverride\fR
.mc
function is to 
create objects
with arguments whose values can only be determined at run time (that is, are
not known at UIL compilation time).
.P 0
.iX "MRM" "MrmFetchSetValues function"
.iX "MrmFetchSetValues function"
.iX "UIL" "using object declaration as a template"
The MRM function 
.iX "MrmFetchSetValues"
.mc |
\fBMrmFetchSetValues\fR
.mc
works like the
.iX "MrmFetchWidgetOverride"
.mc |
\fBMrmFetchWidgetOverride\fR function.  The
.mc
.iX "MrmFetchSetValues"
.mc |
\fBMrmFetchSetValues\fR function, like the 
.mc
.iX "MrmFetchWidgetOverride"
.mc |
\fBMrmFetchWidgetOverride\fR
.mc
function, accepts a
vector of name and value pairs.  This vector is passed as the
\fIoverride_args\fP
argument for the 
.iX "MrmFetchWidgetOverride"
.mc |
\fBMrmFetchWidgetOverride\fR function and as the \fIargs\fP argument for the 
.mc
.iX "MrmFetchSetValues"
.mc |
\fBMrmFetchSetValues\fR function. For the 
.mc
.iX "MrmFetchWidgetOverride"
.mc |
\fBMrmFetchWidgetOverride\fR function, the name and value pairs consist of the
.mc
Motif Toolkit attributes name and an explicit value for that attribute.
.P 0
The value member of the name and value pairs passed to
.iX "MrmFetchSetValues"
.mc |
\fBMrmFetchSetValues\fR is the UIL name of the value, not an explicit value.  
When the application calls 
.mc
.iX "MrmFetchSetValues"
.mc |
\fBMrmFetchSetValues\fR, MRM looks up the names in the UID file,
.mc
then uses the values corresponding to those names to override the original
values in the object declaration.  The 
.iX "MrmFetchSetValues"
.mc |
\fBMrmFetchSetValues\fR function, therefore,
.mc
allows you to keep all values used in an application in the UIL module, and not
.mc |
in the application program.  (The values you pass to the 
.mc
.iX "MrmFetchSetValues"
.mc |
\fBMrmFetchSetValues\fR function must be named, exported literals in the UIL module.)
.mc
.P 0
The 
.iX "MrmFetchSetValues"
.mc |
\fBMrmFetchSetValues\fR function is a convenience function that packages the
.mc
functions provided by the
.iX "MrmFetchLiteral"
.mc |
\fBMrmFetchLiteral\fR and
.mc
.iX "MrmFetchWidgetOverride"
.mc |
\fBMrmFetchWidgetOverride\fR functions.
.mc
.P 0
.H 2 "Customizing a Motif Interface Using UIL and MRM"
.P 0
.iX "Internationalization" "using UIL"
.iX "UIL" "specifying an interface for international markets"
.P 0
UIL offers the advantage of separating the form an interface takes from the
functions of the application.  The form of the interface can change, while the
functions the application performs remain the same.  By specifying these
varying forms of the interface in separate UIL modules, you can change the
interface by changing the definition of the UID 
hierarchy (the set of UID
files) in the application program, and recompiling and relinking the
application.
.P 0
Consider the UIL module in the following example, which shows
the compound string literals for the Motifburger interface translated into
French.  This is a separate UIL module, not an edited version of the original
Motifburger UIL module. 
.mc |
.P
.nf
.ft CW
.in0
.S -2
.mc
module french_literals
       version = 'v1.0'
       names = case_sensitive
value
   k_motifburger_title    : exported "Motifburger - Commandes";
   k_nyi_label_text     : exported "Fonction non disponible";
   k_file_label_text    : exported "Fichier";
     k_quit_label_text  : exported "Quitter";
   k_edit_label_text    : exported "Edition";
     k_cut_dot_label_text      : exported "Couper";
     k_copy_dot_label_text     : exported "Copier";
     k_paste_dot_label_text    : exported "Coller";
     k_clear_dot_label_text    : exported "Effacer tout";
     k_select_all_label_text   : exported "S\o'e\(aa'lectionner tout";
   k_order_label_text            : exported "Commande";
     k_show_controls_label_text  : exported "Voir codes...";
     k_cancel_order_label_text   : exported "Annuler commande";
     k_submit_order_label_text   : exported "Transmettre commande";
   k_hamburgers_label_text         : exported "Hamburgers";
     k_rare_label_text           : exported "Saignant";
     k_medium_label_text         : exported "A point";
     k_well_done_label_text      : exported "Tr\o'e\(ga's cuit";
     k_ketchup_label_text        : exported "Ketchup";
     k_mustard_label_text        : exported "Moutarde";
     k_onion_label_text          : exported "Oignons";
     k_mayonnaise_label_text     : exported "Mayonnaise";
     k_pickle_label_text         : exported "Cornichons";
     k_quantity_label_text       : exported "Quantit\o'e\(aa'";
   k_fries_label_text              : exported "Frites";
     k_size_label_text           : exported "Taille";
     k_tiny_label_text           : exported "Minuscule";
     k_small_label_text          : exported "Petit";
     k_large_label_text          : exported "Gros";
     k_huge_label_text           : exported "Enorme";
   k_drinks_label_text             : exported "Boissons";
     k_0_label_text              : exported '0';
     k_drink_list_text           : exported 
            string_table ('Jus de pomme', 'Jus d\'orange', 
            'Jus de raisin', 'Cola', 'Punch', 
            'Root beer', 'Eau', 'Ginger Ale',
            'Lait', 'Caf\o'e\(aa'', 'Th\o'e\(aa'');
     k_drink_list_select    : exported string_table("Jus de pomme");
     k_u_label_text         : exported "U";
     k_d_label_text         : exported "D";
   k_apply_label_text       : exported "Appliquer";
   k_reset_label_text       : exported "Remise \o'a\(ga' 0";
   k_cancel_label_text      : exported "Annulation";
   k_dismiss_label_text     : exported "Termin\o'e\(aa'";
.mc |

.mc
end module;
.mc |
.in
.S
.fi
.ft R
.mc
.P 0
.mc |
.ne 3i
.mc
In order to generate a French version of the Motifburger application, perform
the following steps:
.AL 1
.LI 
Change all string literal declarations in the original Motifburger UIL 
module to be imported compound strings. 
.mc |
For example, change the value declaration for the Fries label as follows:
.sp .5
.nf
.ft CW
.S -2
.mc
k_fries_label_text     : imported compound_string;
.mc |
.S
.fi
.ft R
.sp .5
.mc
As shown in the previous example,
the French UIL module 
specifies the corresponding values as exported and gives their definitions.  
.LI
In the original C program for the Motifburger application, specify the name
of the UID file containing the compiled French UIL module as the first element
of the UID hierarchy array.
Assume the name of the UIL specification file containing the French strings
.mc |
is french_literals.uil. Change the UID hierarchy array definition as follows:
.sp .5
.nf
.ft CW
.S -2
.mc
static char *db_filename_vec[] =        
  {"french_literals.uid",
   "motifburger.uid"                      
  };                                    
.mc |
.S
.fi
.ft R
.sp .5
.mc
.LI
Add a line to the script that compiles the French UIL module, and
execute the script.  
.LE
.mc *
.mc
.H 2 "Using UIL on Large Projects"
.P 0
When several programmers are working together to specify the interface for a
Motif application, contention for access to the UIL module can develop.
The UIL module can be broken up into
several small files, each containing a segment of the total interface
specification, to allow several people to work on it at one time.  
.P 0
One approach is to construct a main UIL file containing the following
information; once you create a main UIL file, you should rarely need to change
its contents: 
.BL
.LI
Comments describing copyright information, module history, project 
information, and other relevant information. 
.LI
Global declarations, such as case sensitivity, objects clause, and 
procedure declarations.
.LI
A series of include directives. Each include
directive points to a UIL specification file containing some portion
of the interface specification.
.LE
.P 0
The UIL specification for an 
application interface might be
divided into four files, as follows:
.BL
.LI
Shared literals
.mc |
.sp
This file defines all literals shared between the UIL
.mc
module and the application source code.  These are the constants used as tags
.mc |
to the callback procedures.
.mc
.LI
Main window
.mc |
.sp
This file defines the main window for the application.  This
.mc
might include a menu bar with associated cascade buttons, the work
region, and other relevant pieces.
.LI
Bulletin board dialogs
.mc |
.sp
This file defines all the bulletin board dialogs used in 
.mc
the application.
.LI
Other interface objects
.mc |
.sp
This file defines all the other objects 
.mc
that do not fit into the
first three categories.  This file might include display windows with their
menu bars and work regions, pop-up menus, and the command dialog box. 
.LE
.P 0
The purpose of using multiple UIL files 
is simply to make it easier for large programming
project teams to work concurrently on the same application interface. 
It is a matter of style whether the included files themselves contain include
directives.
.P 0
Some programmers prefer to work with a single main UIL file, and
know that this file names all of the remaining files needed to complete the
interface specification.  Having a list of all needed files visible in the main
UIL file can be helpful, for example, to someone translating the user interface
into another language.  All files can be accounted for easily and included in
the translation. 
.P 0
.mc |
.ne 3i
.mc
.H 2 "Working with User-Defined Widgets in UIL"
.P 0
You can extend the Motif Toolkit by building your own widgets.  In UIL, such a
user-defined widget is
identified by the UIL object type 
\fBuser_defined\fP.  
A user-defined widget can
accept any UIL built-in argument or callback reason.  If needed, you can use UIL
to define your own arguments and callback reasons for a user-defined widget. 
You can specify any object as a child of a user-defined widget.
.P 0
To use a user-defined widget in an application interface, follow these steps in
the UIL module:
.AL 1
.LI 
Define the arguments and callback reasons for the user-defined widget that
.mc |
are not UIL built-ins. This can be done inline when declaring an instance of 
.mc
the user-defined widget or in one or more value sections.
.LI
Declare the creation function for the user-defined widget.
.LI
Declare an instance of the user-defined widget. Use 
\fBuser_defined\fP 
as the 
object type and include the name of the widget creation function in the 
declaration.
.LE
.P 0
In the application program, you must register the class of the user-defined
widget using the MRM function 
.mc |
\fBMrmRegisterClass\fR.
.mc
Part of the information you
provide to the 
.mc |
\fBMrmRegisterClass\fR
.mc
function is the name of the widget creation
function.  
By registering the class (and creation function), you allow MRM to
create a user-defined widget using the same mechanisms used to
create Motif Toolkit objects.  You can specify the widget using UIL and
fetch the widget with MRM.
.P 0
The examples in this section are based on a previously built user-defined
widget called the XYZ widget.  The following sections explain how to
include the XYZ widget in an application interface using UIL and how to create
the widget at run time using MRM.
.H 3 "Defining Arguments and Reasons for a User-Defined Widget"
.P 0
The UIL compiler has built-in arguments and callback reasons that are supported
by objects in the Motif Toolkit.  A user-defined widget can be built having only
standard Motif Toolkit arguments and reasons as its resources.  If your
application interface uses a user-defined widget of this type, you can use the
UIL built-in argument names and callback reasons directly when you declare an
instance of the user-defined widget.  If the user-defined widget supports
arguments and reasons that are not built into the UIL compiler, you need to
.mc |
define these arguments and reasons using the \fBARGUMENT\fR and \fBREASON\fR functions,
.mc
respectively, before specifying them.
.P 0 
The following example shows a UIL specification file that defines
.mc |
arguments and callback reasons and declares the creation function for the
.mc
XYZ widget.  This UIL specification file should be included in any UIL module
in which you declare an instance of the XYZ widget.
.mc |
.P
.nf
.S -2
.in0
.ft CW
.mc
(1)  value
     xyz_font_level_0 :       argument ('fontLevel0' , font);
     xyz_font_level_1 :       argument ('fontLevel1' , font);
     xyz_font_level_2 :       argument ('fontLevel2' , font);
     xyz_font_level_3 :       argument ('fontLevel3' , font);
     xyz_font_level_4 :       argument ('fontLevel4' , font);
     xyz_indent_margin :      argument ('indentMargin' , integer);
     xyz_unit_level :         argument ('unitLevel' , integer);
     xyz_page_level :         argument ('pageLevel' , integer);
     xyz_root_widget:         argument ('rootWidget' , integer );
     xyz_root_entry:          argument ('rootEntry' , integer);
     xyz_display_mode:        argument ('displayMode' , integer);
     xyz_fixed_width_entries: argument ('fixedWidthEntries' , Boolean);
(2)  value
     xyz_select_and_confirm : reason ('selectAndConfirmCallback');
     xyz_extend_confirm :     reason ('extendConfirmCallback');
     xyz_entry_selected :     reason ('entrySelectedCallback');
     xyz_entry_unselected :   reason ('entryUnselectedCallback');
     xyz_help_requested:      reason ('helpCallback');
     xyz_attach_to_source :   reason ('attachToSourceCallback');
     xyz_detach_from_source : reason ('detachFromSourceCallback');
     xyz_alter_root :         reason ('alterRootCallback');
     xyz_selections_dragged : reason ('selectionsDraggedCallback');
     xyz_get_entry :          reason ('getEntryCallback');
     xyz_dragging :           reason ('draggingCallback');
     xyz_dragging_end :       reason ('draggingEndCallback');
     xyz_dragging_cancel :    reason ('draggingCancelCallback');
(3)  value
     XyzPositionTop :       1;
     XyzPositionMiddle :    2;
     XyzPositionBottom :    3;
     XyzDisplayOutline :    1;
     XyzDisplayTopTree :    2;
(4)  procedure XyzCreate();
.mc |
.S
.in
.fi
.ft R
.P
\fBNOTES:\fR
.mc
.AL 1
.LI 
.mc |
Defines UIL argument names for the XYZ widget that are not
.mc
built-in Toolkit arguments.  The strings you pass to the
.mc |
\fBARGUMENT\fR function must match the names listed in the resource list
.mc
structure in the widget class record for the XYZ widget. 
.mc *
.mc
In addition to the string, specify the data type of the argument.   Just as for
built-in arguments, when you declare an instance of the XYZ widget in a UIL
module, the UIL compiler checks the data type of the values you specify for
these arguments.  For example, the UIL compiler checks that the value you
.mc |
specify for the \fIxyz_indent_margin\fP argument is an integer.
.mc
.LI
Defines the XYZ widget's callback reason names that are not
.mc |
UIL built-in reasons.  The strings you pass to the \fBREASON\fR function
.mc
must match the names listed in the resource list structure in the
widget class record for the XYZ widget.
(Callback reasons, like UIL arguments, are considered to be widget-specific
attributes in the Motif Toolkit and are defined as resources.)
.LI
Defines some integer literals for specifying
arguments of the XYZ widget.
.LI
Declares the widget creation function for the XYZ
widget.  This creation function is registered with MRM through the
.mc |
\fBMrmRegisterClass\fR
function (see the example in Section 22.5.3).
.mc
.LE
.H 3 "Using a User-Defined Widget in an Interface Specification"
.P 0
The following example shows how to specify the XYZ widget in a UIL
module.  This UIL module includes the UIL specification file shown in 
the previous example as 
.mc |
\fBxyz_widget.uil\fR.
.mc
.P 0
.mc |
.nf
.S -2
.in0
.ft CW
.mc
     module xyz_example
         names = case_sensitive
.mc |
.br
.ne 3i
.mc
     include file 'XmAppl.uil';
(1)  include file 'xyz_widget.uil';
(2)      procedure
          XyzAttach       ();
          XyzDetach       ();
          XyzExtended     ();
          XyzConfirmed    ();
          XyzGetEntry     ();
          XyzSelected     ();
          XyzUnselected   ();
          XyzDragged      ();
          XyzDragging     ();
          XyzDraggingEnd  ();
          create_proc     ();
          MenuQuit        ();
          MenuExpandAll   ();
          MenuCollapseAll ();
(3)      object
          main : XmMainWindow 
              { arguments
                  {  
                     XmNx = 0; 
                     XmNy = 0;
                     XmNheight = 0;
                     XmNwidth = 0;
                  };
                controls
                  {  XmMenuBar main_menu;
                     user_defined xyz_widget;
                  };
              };
(4)        xyz_widget : user_defined procedure XyzCreate
              { arguments
                  {  
                     XmNx = 0; 
                     XmNy = 0;
                     XmNheight = 600;
                     XmNwidth = 400;
(5)                  xyz_display_mode = XyzDisplayOutline;
                  };
                callbacks
                  {  xyz_attach_to_source =   procedure XyzAttach();
                     xyz_detach_from_source = procedure XyzDetach();
                     xyz_get_entry =          procedure XyzGetEntry();
                     xyz_select_and_confirm = procedure XyzConfirmed();
                     xyz_extend_confirm =     procedure XyzExtended();
                     xyz_entry_selected =     procedure XyzSelected();
                     xyz_entry_unselected =   procedure XyzUnselected();
                     xyz_selections_dragged = procedure XyzDragged();
                     xyz_dragging =           procedure XyzDragging();
                     xyz_dragging_end =       procedure XyzDraggingEnd();
.mc |
(6)                  MrmNcreateCallback =      procedure create_proc();
.mc
                  };
              };
(7)          main_menu: XmMenuBar
              { arguments
                  {  XmNorientation = XmHORIZONTAL;
                  };
                controls
                  {  XmCascadeButton file_menu;
                  };
              };
          file_menu: XmCascadeButton
              { arguments
                  {  XmNlabelString = 'File';
                  };
                controls
                  {  XmPulldownMenu
                      { controls
                          { XmPushButton expand_all_button;
                            XmPushButton collapse_all_button;
                            XmPushButton quit_button;
                          };
                      };
                  };
              };
          expand_all_button: XmPushButton
              { arguments
                  {  XmNlabelString = "Expand All";
                  };
                callbacks
                  {  XmNactivateCallback = procedure MenuExpandAll();
                  };
              };
          collapse_all_button: XmPushButton
              { arguments
                  {  XmNlabelString = "Collapse All";
                  };
                callbacks
                  {  XmNactivateCallback = procedure MenuCollapseAll();
                  };
              };
          quit_button: XmPushButton
              { arguments
                  {  XmNlabelString = "Quit";
                  };
                callbacks
                  {  XmNactivateCallback = procedure MenuQuit();
                  };
              };
  end module;
.mc |
.in
.S
.fi
.ft R
.P
\fBNOTES:\fR
.mc
.AL 1
.LI 
.mc |
Includes a directive to include the definition of the XYZ widget shown in 
the example in Section 22.5.1.
.mc
.LI
Declarations for the callback functions defined in the application
program.
.LI
Declaration for the main window widget.  The main window widget has two 
children: a menu bar widget and the XYZ widget.
.LI
Declaration for the XYZ widget.  Note that the object type is 
.mc |
\fBuser_defined\fP and that the creation function, \fIXyzCreate\fP, 
is included in the declaration.
.mc
.LI
.mc |
The \fIxyz_display_mode\fP argument, defined with the \fBARGUMENT\fR
function in the example in Section 22.5.1, is specified using one of the
.mc
integer literals also defined in that example.
.LI
.mc |
All widgets support the \fBMrmNcreateCallback\fR reason.
.mc
.LI
The remaining objects declarations comprise the menu bar widget and its 
pull-down menu widgets.
.LE
.H 3 "Accessing a User-Defined Widget at Run Time"
.P 0
The following example shows a C application program that displays the XYZ 
.mc |
widget (defined in the example in Section 22.5.1 and declared in
the example in Section 22.5.2).
.P
.nf
.ft CW
.S -2
.in 0
.mc
     #include <Mrm/MrmAppl.h>
(1)  #include <XmWsXyz.h>
(2)  globalref int xyzwidgetclassrec;
(3)   extern void XyzAttach       ();
      extern void XyzDetach       ();
      extern void XyzGetEntry     ();
      extern void XyzConfirmed    ();
      extern void XyzExtended     ();
      extern void XyzSelected     ();
      extern void XyzUnselected   ();
      extern void XyzHelpRoutine  ();
      extern void XyzDragged      ();
      extern void XyzDragging     ();
      extern void XyzDraggingEnd  ();
      extern void create_proc     ();
      extern void MenuQuit        ();
      extern void MenuExpandAll   ();
      extern void MenuCollapseAll ();
(4)  static MrmRegisterArglist   register_vector[] =
      {
          { "XyzAttach",                  (caddr_t) XyzAttach },
          { "XyzDetach",                  (caddr_t) XyzDetach },
          { "XyzGetEntry",                (caddr_t) XyzGetEntry },
          { "XyzConfirmed",               (caddr_t) XyzConfirmed },
          { "XyzExtended",                (caddr_t) XyzExtended },
          { "XyzSelected",                (caddr_t) XyzSelected },
          { "XyzUnselected",              (caddr_t) XyzUnselected },
          { "XyzHelpRoutine",             (caddr_t) XyzHelpRoutine },
          { "XyzDragged",                 (caddr_t) XyzDragged },
          { "XyzDragging",                (caddr_t) XyzDragging },
          { "XyzDraggingEnd",             (caddr_t) XyzDraggingEnd },
          { "create_proc,                 (caddr_t) create_proc },
          { "MenuQuit",                   (caddr_t) MenuQuit },
          { "MenuExpandAll",              (caddr_t) MenuExpandAll },
          { "MenuCollapseAll",            (caddr_t) MenuCollapseAll }
      };
.mc |
  #define register_vector_length ( (sizeof register_vector) /  \e
.mc
                                   (sizeof register_vector[0]))
(5)  static MrmHierarchy     hierarchy_id ;
     static char             *vec[]={"xyz_example.uid"};
     static MrmCode          class ;
       Widget toplevel;
.mc |
       XtAppContext app_context;
.mc
       Widget mainwindow;
(6)    int main (argc, argv)
       unsigned int argc;
       char **argv;
  {
(7)       Arg arguments[1];
(8)       MrmInitialize();
.mc |
(9)       toplevel = XtAppInitialize (&app_context, "xyz", NULL, 0,
                                      &argc, argv, NULL, NULL, 0);
(10)      if( MrmRegisterClass
.mc
               ( MRMwcUnknown,
                 XyzClassName,
                 "XyzCreate",
                 XyzCreate,
                 &xyzwidgetclassrec )
           != MrmSUCCESS )
       {
           printf ("Can't register XYZ widget");
       }
.mc *
.mc
(11)      if( MrmOpenHierarchy
               ( 1, 
                 vec,
                 NULL,
                 &hierarchy_id ) 
            != MrmSUCCESS )
       {
           printf ("Can't open hierarchy");
       }
(12)     MrmRegisterNames( register_vector, register_vector_length );
         XtSetArg (arguments[0], XmNallowShellResize, TRUE);
         XtSetValues (toplevel, arguments, 1);
(13)     if( MrmFetchWidget
               ( hierarchy_id,
                 "main",      
                 toplevel,    
                 &mainwindow, 
                 &class )     
            != MrmSUCCESS )
       {
           printf ("Can't fetch interface ");
       }
       XtManageChild (mainwindow);
       XtRealizeWidget (toplevel);
.mc |
       XtAppMainLoop(app_context);
.mc
       return (0);
   }
.mc |

.mc
\0\0\0.
\0\0\0.
\0\0\0.
.mc |
.fi
.ft R
.S
.in
.P
\fBNOTES:\fR
.mc
.AL 1
.LI 
Includes XYZ declarations.
.LI
Provides a reference to the widget class record for the XYZ widget 
.mc |
(named \fIxyzwidgetclassrec\fP).
.mc
.LI
Declares callback routines defined (but not shown) later in the program.
.LI
Defines the mapping between UIL procedure names and their addresses.
.LI
Specifies the UID hierarchy list.  The UID hierarchy for this application 
consists of a single UID file, the compiled version of 
.mc |
\fBxyz_example.uil\fR.
.mc
(Assume the UIL specification file has the same name as the UIL module; see the
module header.  The file named 
.mc |
\fBxyz_example.uil\fR
.mc
includes the file 
.mc |
\fBxyz_widget.uil\fR, shown in Section 22.5.1.)
.mc
.LI
Main routine.
.LI
Arguments for the widgets.
.LI
Initializes MRM.
.LI
.mc |
Initializes the Motif Toolkit.
.LI
.mc
Registers the XYZ widget class with MRM.  This allows MRM to use standard
creation mechanisms to create the XYZ widget (see (13)).  
The arguments passed to the 
.mc |
\fBMrmRegisterClass\fR
.mc
routine are as follows:
.BL
.LI
.mc |
MRMwcUnknown -- Indicates that the class is user defined
.mc
.LI
XyzClassName -- Class name of XYZ widget, defined in 
.mc |
\fBXmWsXyz.h\fR.
.mc
.LI
"XyzCreate" -- Name of the creation routine
.LI
\fIXyzCreate\fP -- Address of the creation routine
.LI
&xyzwidgetclassrec -- Pointer to the widget class record
.LE
.LI
.mc *
.mc
Defines the UID hierarchy.
.LI
Registers callback routine names with MRM.
.LI
Fetches the interface (the main window widget with a menu bar widget and 
the XYZ widget
in the work area).  Note that the XYZ widget is treated like any Motif Toolkit
widget.  MRM calls the XYZ widget's creation routine (\fIXyzCreate\fP)
.mc |
and passes this routine the values for the 
XmNx, XmNy, XmNwidth, XmNheight, and \fIxyz_display_mode\fP) arguments as
.mc
specified in the UID file, using the standard creation routine format. 
.LE
