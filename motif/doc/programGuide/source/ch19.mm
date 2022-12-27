.mc |
.\"  (c) Copyright 1989, 1990 OPEN SOFTWARE FOUNDATION, INC.
.mc
.\"  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
.\"  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
.\"  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
.\"  (c) Copyright 1988 MICROSOFT CORPORATION
.\"  ALL RIGHTS RESERVED
.mc |
.nr H1 18
.mc
.H 1 "UIL Module Structure"
.P 0
This chapter describes how to build a UIL module using the language
elements described in the previous chapter.
This chapter also explains 
the scope of 
references to values and objects defined in UIL, and describes
the syntax and use of the following UIL module components:
.BL
.LI
Value section
.LI
Procedure section
.LI
List section
.LI
Object section
.LI
Identifier section
.LI
Include directive
.LE
.P 0
In addition, this chapter describes the following features of UIL:
.BL
.LI
Argument definitions for constraint widgets
.LI
Symbolic references to widget IDs
.LE
.P 0
.mc |
.ne 4
.mc
In this chapter, all examples assume case-insensitive mode.
Keywords are shown
.mc |
in uppercase to distinguish them from user-specified names,
.mc
which are
.mc |
shown in lowercase.  This use of uppercase is
.mc
not required in case-insensitive mode.  In case-sensitive mode, keywords
.mc |
must be in lowercase.
.mc
.P 0
.H 2 "Structure of a UIL Module"
.iX "UIL" "specification file structure"
.iX "UIL specification file" "structure of"
.iX "UIL module" "structure of"
.P 0
A UIL module contains definitions of objects that are to be stored in a
User Interface Definition (UID) file, the compiled output of the UIL
compiler.
A UIL module consists of a module block,
which contains a series of value, procedure, list, identifier, and
object
sections.
There can be any number of these sections in a UIL module. A UIL module 
can also contain include directives, which can 
be placed anywhere in the module, 
.mc |
except within a value, procedure, list, identifier, or object section.
.mc
.P 0
The structure of a UIL module is as follows:
.mc |
.P
.nf
.in 0
.ft CW
.S -2
.mc
uil_module ::=
        MODULE module_name
            [ version-clause ]
            [ case-sensitivity-clause ]
            [ default-character-set-clause ]
            [ default-object-variant-clause ]
            { value-section
            | procedure-section
            | list-section
            | object-section 
            | identifier-section
            | include-directive }...
        END MODULE ";"
version-clause ::=
        VERSION "=" character-expression
case-sensitivity-clause ::=
        NAMES "=" { CASE_SENSITIVE
                    CASE_INSENSITIVE }
default-character-set-clause ::=
        CHARACTER_SET "=" char-set
default-object-variant-clause ::=
        OBJECTS "=" "{" object-type "=" WIDGET | GADGET";" ... "}"
.mc |
.ft R
.S
.fi
.in
.mc
.P 0
.mc |
.ne 4
.mc
The module name is the name by which this UIL module is known
in the UID file.  This name is stored in the UID file for later
use in the retrieval of resources by the MRM.
This name is always stored in uppercase in the UID file. 
.P 0
The following is an example of the UIL module structure. 
.mc |
.P
.nf
.ft CW
.S -2
.\".in 0
.mc
!+
!        Sample UIL module
!-
MODULE example                ! module name
  VERSION = 'V1.0'            ! version
  NAMES = CASE_INSENSITIVE    ! keywords and names are 
                              ! not case sensitive 
  CHARACTER_SET = ISO_LATIN6  ! character set for compilation
                              ! is ISO_LATIN6
  OBJECTS = { XmPushButton = GADGET; }   ! push buttons are 
                                        ! gadgets by default
!+
!       Declare the VALUES, PROCEDURES, LISTS,
!       IDENTIFIERS, and OBJECTS here...
!-
END MODULE;
.mc |
.ft R
.fi
.S
.\".in
.br
.ne 1.2i
.mc
.H 3 "Version Clause"
.P 0
.iX "UIL" "version clause"
.iX "Version clause"
.iX "UIL specification file" "version clause"
.iX "UIL module" "version clause"
The version clause specifies the version number of the UIL module.  It is 
provided so that the programmer can verify the correct 
version of a UIL module is being accessed by MRM.
The character expression you use to 
specify the version can be up to 31 characters in length.
.mc |
.H 3 "Case-Sensitivity Clause"
.mc
.P 0
.iX "UIL" "case sensitivity clause"
.iX "Case sensitivity clause"
.iX "UIL specification file" "case sensitivity clause"
.iX "UIL module" "case sensitivity clause"
.mc |
The case-sensitivity clause indicates whether names
.mc
are to be treated as case sensitive or case
insensitive.
.mc |
The default is case sensitive. 
.mc
.P 0
.mc |
The case-sensitivity clause should be the
.mc
first clause in the module header, and in any case must precede any statement
that contains a name.
.P 0
If names are case sensitive in a UIL module, UIL keywords in that module
must be in lowercase.  Each name is stored in the UIL file in the same
case as it appears in the UIL module. 
If names are case insensitive, then keywords 
can be in uppercase, lowercase, or 
mixed case, and the uppercase equivalent of each
name is stored in the UID file.
.mc |
The following table summarizes these rules.
.P
.mc
.TB "Rules for Case Sensitivity in a UIL Module"
.P 0
.TS H
tab(@);
lBw(1i) lBw(1.75i) lBw(1.75i)
lBw(1i) lBw(1.75i) lBw(1.75i)
lw(1i) lw(1.75i) lw(1.75i).
.mc
_
.sp 6p
.mc |
@Keyword Treatment@Name Treatment
Case Sensitivity@in UIL Module@in UID File
.mc
.sp 6p
_
.sp 6p
.mc *
.mc
.TH
Case sensitive@T{
Must be entered in lowercase
T}@T{
Stored in the
same case as they appear in the UIL module
T}
.br
Case insensitive@T{
Can be entered in lowercase, uppercase, or mixed
case
T}@T{
Stored in uppercase
T}
.sp 6p
_
.TE
.H 3 "Default Character Set Clause"
.P 0
The default character set clause specifies the default character set for 
string literals in the module.  The use of the default character set 
clause is optional.
.P 0
If specified, the character set clause designates the character set used to
interpret an extended string literal if you did not specify a character
set for that literal. 
If you do not include the character set clause in
the module header, the default character
.mc |
set for the compilation is the codeset portion of the value of the
\fBLANG\fR environment variable if it is set or the value of
\fBXmFALLBACK_CHARSET\fP if LANG is not set or has no codeset component.
By default, \fBXmFALLBACK_CHARSET\fP is ISO8859-1 (equivalent to
ISO_LATIN1), but vendors may define a different default.
See Section 20.3 for more information.
.mc
.H 3 "Default Object Variant Clause"
.P
A gadget is a simplified version of a widget that consumes less resources (and
therefore enhances application performance) but offers limited customization. 
The following types of user interface object types have both a widget and a
gadget variant:
.mc |
.br
.ne 4
.br
.ne 4
.mc
.BL
.LI
Cascade button
.LI
Label
.LI
Push button
.LI
Separator
.LI
Toggle button
.LE
.P 0
For the most part, the widget and gadget variants are interchangeable.  To use
gadgets in your application, you need to specify to the UIL compiler that you
want that particular variant.  Otherwise, by default, the UIL compiler assumes
.mc |
you want to use widgets.  There are three ways to specify that you want to
use a gadget instead of a widget:
.BL
.LI
Add the default object-variant clause
to the module header.
.LI
Add the keyword GADGET to particular object
.mc
declarations.
.mc |
.LI
Use the Motif toolkit object name of the gadget variant (usually
\fBXm\fP<\fIobject\fP>\fBGadget\fP).
.LE
.mc
.P 0
.mc |
By using the default object-variant clause in the UIL module header, you can
.mc
declare all cascade buttons, labels, push buttons, separators, and
toggle buttons, or any combination of these types, to be gadgets.  In the
.mc |
sample module declaration shown previously in this section, only push buttons
are declared
.mc
as gadgets.
.P 0
To change these objects from one variant to the other, you need only change the
.mc |
default object-variant clause.  For example, suppose you used the default
object-variant clause to declare all push buttons as gadgets.  To change all
.mc
push button objects from gadgets to widgets, remove the type 
.mc |
\fBXmPushButton\fR
.mc
from the clause.
.P 0
Because you do not specify the variant for imported objects (discussed in the
next section), the variant (whether a widget or gadget) of imported objects 
is unknown until run time.
.P 0
.mc |
See Section 19.6 for more information on specifying the variant of objects in a
.mc
UIL module.
.mc |
.br
.ne 2i
.mc
.H 2 "Scope of References to Values and Objects"
.P 0
.iX "UIL" "scope of references"
.iX "Object" "scope of references"
.iX "Value" "scope of references"
.iX "IMPORTED"                                                  
.iX "EXPORTED"
.iX "PRIVATE"
.iX "Keywords" "IMPORTED"                                                  
.iX "Keywords" "EXPORTED"
.iX "Keywords" "PRIVATE"
.mc |
UIL values can have one of the following levels of scope of reference:
.mc
.BL
.LI
EXPORTED: A value that you define as exported is stored in the
UID file as a named resource, and therefore can be referenced by
name in other UID files.  When you define a value as exported, 
MRM looks outside the module in which the exported value
is declared to get its value at run time.
.LI
IMPORTED: A value that you define as imported is one that is 
defined as a named resource in a UID file.  MRM resolves this 
declaration with the corresponding exported declaration at 
application run time.
.LI
PRIVATE: A private value is a value that is not imported or exported.
A value that you define as private is
not stored as a distinct resource in the UID file.
You can reference a private value only in the UIL module containing 
the value declaration. The value or object is directly incorporated into
anything in the UIL module that references the declaration.
.LE
.P 0
EXPORTED, IMPORTED, and PRIVATE are reserved UIL keywords.  By default,
values and objects are private.
.H 2 "Structure of a Value Section"
.P 0
.iX "Value section"
.iX "UIL specification file" "value section"
.iX "UIL module" "value section"
.iX "UIL" "value section"
A value section consists of the keyword VALUE followed by a sequence of
value declarations.  It has the following syntax: 
.mc |
.P
.nf
\fIvalue-section\fP ::=
        \fBVALUE\fR \fIvalue-declaration\fP...
\fIvalue-declaration\fP ::=
        \fIvalue-name\fP ":"
                { \fBEXPORTED\fR \fIvalue-expression\fP
                | \fBPRIVATE\fR \fIvalue-expression\fP
                | \fIvalue-expression\fP
                | \fBIMPORTED\fR \fIvalue-type\fP } ";"
.fi
.ft R
.mc
.P 0
.iX "UIL specification file" "value declaration"
.iX "UIL module" "value declaration"
.iX "UIL" "value declaration"
.iX "Value declaration"
.mc |
.ne 3i
A value declaration provides a way to name a value expression or literal.
.mc
The value name can be referred to by declarations that occur
later in the UIL module in any context where a value can be used.
.mc |
Values can be forward referenced.
.mc
.P 0
Value sections can include a keyword defining the scope of references to
.mc |
the value (see Section 19.2).
The following table describes the supported value types in UIL.
.mc
.iX "UIL" "value types"
.iX "Value types"
.iX "UIL specification file" "value types"
.iX "UIL module" "value types"
.mc |
.br
.ne 2i
.P
.mc
.TB "Value Types"
.mc |
.sp .15
.in0
.mc
.TS H
tab(@);
lBw(1.5i) lBw(3.5i)
lw(1.5i) lw(3.5i).
.mc
_
.sp 6p
Value Type@Description
.sp 6p
_
.sp 6p
.mc *
.mc
.TH
ANY@T{
Prevents the UIL compiler from checking the type of an argument
value
T}
ARGUMENT@T{
Defines a value as a user-defined argument
T}
BOOLEAN@T{
.mc |
Defines a value as True (On) or False (Off)
.mc
T}
COLOR@Defines a value as a color
COLOR_TABLE@T{
Provides a device-independent way to define a set
of colors (usually for a pixmap)
T}
COMPOUND_STRING@Defines a value as a compound string
.mc |
FLOAT@T{
Defines a value as a double-precision, floating-point number
T}
.mc
FONT@Defines a value as a font
FONT_TABLE@T{
Defines a value as a sequence of font and character
set pairs
T}
ICON@T{
Describes a rectangular pixmap using a character to represent
each pixel
T}
INTEGER@Defines a value as an integer
INTEGER_TABLE@Defines a value as an array of integers
.mc |
KEYSYM@Defines a value as a keysym
.mc
REASON@T{
Defines a condition under which a widget is to call an
application function
T}
.mc |
SINGLE_FLOAT@T{
Defines a value as a single-precision, floating-point number
T}
.mc
STRING@Defines a value as a null-terminated (ASCIZ) string
STRING_TABLE@Defines a value as an array of compound strings
.mc |
TRANSLATION_TABLE@Defines an alternative set of events or actions for a widget
.sp 3p
.mc
_
.TE
.mc |
.in
.mc
.P 0
.mc |
.ne 3i
The following example shows how to declare values:
.mc
.P 0
.mc |
.nf
.ft CW
.S -2
.mc
VALUE
.mc |
   k_main             : EXPORTED 1;
   k_main_menu        : EXPORTED 2;
   k_main_command     : EXPORTED 3;
.mc
VALUE
.mc |
   white        : IMPORTED COLOR;
   blue         : IMPORTED COLOR;
   arg_name     : PRIVATE 'new_argument_name';
.mc
VALUE
.mc |
   main_prompt  : 'next command';  ! PRIVATE by default
   main_font    : IMPORTED FONT;
.S
.fi
.ft R
.mc
.P 0
.mc |
Because the values \fIk_main\fP \fIk_main_menu\fP and \fIk_main_command\fP
are defined as exported, you can use these values in another UIL 
module as follows:
.P
.nf
.ft CW
.mc
VALUE
.mc |
    k_main                : IMPORTED \fIinteger\fP;
.ft R
.fi
.mc
.H 2 "Structure of a Procedure Section"
.P 0
.iX "UIL" "procedure section"
.iX "UIL specification file" "procedure section"
.iX "UIL module" "procedure section"
.iX "PROCEDURE section"
.mc |
A procedure section consists of the keyword \fBPROCEDURE\fR followed by a
.mc
sequence of procedure declarations.  It has the following syntax:
.mc |
.P
.nf
\fIprocedure-section\fP ::=
        \fBPROCEDURE\fR \fIprocedure-declaration\fP...
\fIprocedure-declaration\fP ::=
        \fIprocedure-name\fP
            [ \fIformal-parameter-spec\fP ]";"
\fIformal-parameter-spec\fP ::=
            "(" [ \fIvalue-type\fP ] ")" ";"
.fi
.ft R
.mc
.P 0
.iX "UIL" "procedure declaration"
.iX "UIL module" "procedure declaration"
.iX "UIL specification file" "procedure declaration"
.iX "Procedure declaration"
Use a procedure declaration to declare the following:
.BL
.LI
A routine that can be used as a
callback routine for a widget
.LI
.mc |
The creation function for a user-defined widget
.mc
.LE
.P 0
.mc |
.ne 3i
.mc
You can reference a procedure name in
declarations that occur later in the UIL module in any context 
.mc |
where a procedure can be used.  Procedures can be forward referenced.
You cannot use a
.mc
name you used in another context as a procedure name.
.P 0
In a procedure declaration, you have the option
of specifying that a parameter will be
passed to the corresponding callback routine at run time.  This parameter is
called the callback tag.  You can specify the data type of the callback tag by
putting the data type in parentheses following the procedure name.  When you
compile the module, the UIL compiler checks that the argument you specify in
references to the procedure is of this type.  Note that the data type of the
.mc |
callback tag must be one of the valid UIL data types (see Table 19-2).
Note also that you cannot use a widget as a callback tag.
.mc
.P 0
.mc |
The following table summarizes how the UIL compiler checks
.mc
argument type and argument count, depending on the procedure declaration.
.mc |
.P
.mc
.TB "Rules for Checking Argument Type and Count"
.ne 7
.TS H
tab(@);
lB lBw(3.5i)
l lw(3.5i).
.mc
_
.sp 6p
Declaration@Rules
.sp 6p
_
.sp 6p
.mc *
.mc
.TH
No parameters@T{
No argument type or argument count checking occurs. 
You can supply any number of arguments in the procedure reference.
T}
( )@T{
.mc |
Checks that the argument count is 0.
.mc
T}
.mc |
(\f3any\fP)@T{
.mc
Checks that the argument count is 1.  Does
.mc |
not check the argument type.  Use the \f3any\fP type to prevent type 
checking on procedure tags.
.mc
T}
.mc |
(\f2type\fP)@T{
.mc
Checks for one argument of the specified type.
T}
.sp 6p
_
.TE
.iX "UIL" "procedure declaration"
.iX "UIL module" "procedure declaration"
.iX "UIL specification file" "procedure declaration"
.iX "Procedure declaration"
.iX "UIL procedure" "declaring"
.P 0
For example, in the following procedure declaration, the callback procedure
named
\fItoggle_proc\fP
will be passed an integer tag at run time.  The UIL compiler checks that the
parameter specified in any reference to procedure
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
.mc
.P 0
.mc |
.ne 3i
.mc
While it is possible to use any UIL data type to specify the type of a tag in a
procedure declaration, you must be able to represent that data type in the
.mc |
programming language you are using.  Some data types (such as integer, Boolean,
.mc
and string) are common data types recognized by most programming languages. 
Other UIL data types (such as string tables) are more complicated and may
require you to set up an appropriate corresponding data structure in the
application in order to pass a tag of that type to a callback routine.
.P 0
You can also use a procedure declaration to specify the creation function
for a user-defined widget.  In this case, you specify no formal parameters.
The procedure is invoked with the standard three arguments passed to
all widget creation functions.
.mc |
(See the Motif Toolkit documentation for more information 
.mc
about widget creation functions.)
.P 0
The following example
shows how to declare a procedure:
.mc |
.P
.nf
.ft CW
.mc
    PROCEDURE
        app_help (INTEGER);
        app_destroy (INTEGER);
.mc |
.fi
.ft R
.br
.ne 5i
.mc
.H 2 "Structure of a List Section"
.P 0
.iX "UIL" "list section"
.iX "UIL module" "list section"
.iX "UIL specification file" "list section"
.iX "List section"
A list section consists of the keyword LIST followed by a
sequence of list declarations.  It has the following syntax:
.mc |
.P
.nf
\fIlist-section\fP ::=
        \fBLIST\fR \fIlist-declaration\fP...
\fIlist-declaration\fP ::=
        \fIlist-name\fP ":" \fIlist-definitio\fPn ";"
\fIlist-definition\fP ::=
        \fIlist-type list-spec 
list-type\fP ::=
        { \fBARGUMENTS\fR
        | \fBCONTROLS\fR
        | \fBCALLBACKS\fR
        | \fBPROCEDURES\fR }
\fIlist-spec\fP ::=
        { \fIlist-name\fP
        | "{" \fIlist-entry\fP... "}" } 
\fIlist-entry\fP ::=
        { \fIlist-definition\fP
        | \fIargument-list-entry\fP
        | \fIcontrol-list-entry\fP
        | \fIcallback-list-entry\fP 
        | \fIprocedure-list-entry\fP } ";"
.fi
.ft R
.mc
.P 0
You use list sections to group together a set of arguments, controls
.mc |
(children),
callbacks, or procedures for later use in the UIL module.  Lists can contain other
.mc
lists, so that you can set up a hierarchy to clearly show which
.mc |
arguments, controls, callbacks, and procedures are common to which widgets.
.mc
You cannot mix the different types of lists; a list
of a particular type cannot contain entries of a different list type
or reference the name of a different list type.
.P 0
.mc |
.ne 3i
.mc
A list name is always private to the UIL module in which you declare 
the list and
cannot be stored as a named resource in a UID file.
.P 0
.iX "UIL" "list types"
.iX "UIL module" "list types"
.iX "UIL specification file" "list types"
.iX "List types"
There are four types of lists in UIL:
.BL
.LI
.mc |
Arguments list, having the list type \fBARGUMENTS\fR
.mc
.LI
.mc |
Callbacks list, having the list type \fBCALLBACKS\fR
.mc
.LI
.mc |
Controls list, having the list type \fBCONTROLS\fR
.mc
.LI
.mc |
Procedures list, having the list type \fBPROCEDURES\fR
.mc
.LE
.P 0
These list types are described in the following sections.
.H 3 "Arguments List Structure"
.P 0
.iX "List types" "argument"
.iX "UIL" "arguments list"
An arguments list defines which arguments are to be specified
.mc |
in the arguments-list parameter when the creation routine for a particular
.mc
object is called at run time.  
An arguments list also specifies
the values for those arguments.
Each entry in the arguments list has the following syntax:
.mc |
.P
.nf
\fIargument-list-entry\fP ::=
        \fIargument-name\fP "=" \fIvalue-expression\fP
.fi
.ft R
.mc
.P 0
The argument name must be either a built-in argument name or a
user-defined argument name that is specified with the
.mc |
\fBARGUMENT\fR function (see Section 18.5.13).
.mc
If you use a built-in argument name, the
type of the value expression must match the allowable type
for the argument.
.P 0
.mc |
If you use a built-in argument name as an arguments list entry in an object 
.mc
definition, the UIL compiler checks the argument name to be sure that it
is supported by the type of object that you are defining.  If the same argument
name appears more than once in a given arguments list, the last entry
that uses that argument name supersedes all previous entries
with that name, and the compiler issues a message. 
.P 0
.mc |
.ne 3i
.mc
Some arguments are coupled by the UIL compiler.  
When you specify one of the arguments, the compiler also sets the
.mc |
other.  The coupled
.mc
argument is not available to you.  The coupled arguments are listed
.mc |
in the following table.
.mc
.iX "UIL" "coupled arguments"
.iX "Arguments" "coupled in UIL"
.mc |
.P
.mc
.TB "Coupled Arguments"
.P 0
.TS H
tab(@);
lB lB
l l.
.mc
_
.sp 6p
Supported Argument@Coupled Argument
.sp 6p
_
.mc *
.mc
.TH
.mc |
XmNitems@XmNitemCount
XmNselectedItems@XmNselectedItemCount
.mc
.sp 6p
_
.TE
.P 0
See Appendix B 
for information about which arguments are supported by which widgets.  See 
Appendix C
for information about what the valid value type is for each built-in argument.
.P 0
.mc |
The following example shows how to declare and reference an arguments list:
.P
.nf
.ft CW
.mc
    LIST
        default_size: ARGUMENTS {
            XmNheight = 500;
            XmNwidth = 700;
        };
        default_args: ARGUMENTS {
            ARGUMENTS default_size;
            XmNforeground = white;
            XmNbackground = blue;
        };
.mc |
.fi
.ft R
.br
.ne 4i
.mc
.H 3 "Callbacks List Structure"
.mc |
.sp .35
.mc
.iX "UIL" "callbacks list"
.iX "List types" "callback"
Use a callbacks list to define which callback reasons are to
be processed by a particular widget at run time.
.mc |
Each callbacks list entry has the following syntax:
.P
.nf
\fIcallback-list-entry\fP ::=    
        \fIreason-name\fP "=" \fIprocedure-reference\fP
\fIprocedure-reference\fP ::=
        \fBPROCEDURE\fR \fIprocedure-name\fP
            [ "(" [ \fIvalue-expression\fP ] ")" ";"
               |\ \ \fIprocedure-list-specification\fP ] 
.fi
.ft R
.mc
.P 0
For Motif Toolkit widgets, the reason name must be a built-in
reason name.  For a user-defined widget, you can use a reason name
.mc |
that you previously specified using the \fBREASON\fR function (see
Section 18.5.14).
.mc
If you use a built-in reason in an object definition, 
the UIL compiler ensures that reason is supported by the type of object
you are defining.  Appendix B shows which reasons
each object supports.
.P 0
If the same reason appears more than once in a callbacks list, the last entry
referring to that name supersedes all previous entries using the same reason,
and the UIL compiler issues a diagnostic message.
.P 0
.mc |
If you specify a named value for the procedure argument (callback
.mc
tag), the data type of the value must match the type specified for the callback
.mc |
tag in the corresponding procedure declaration.  See Section 19.4 for a detailed
.mc
explanation of argument type checking for procedures.
.mc |
.P
The following example shows how to declare a callbacks list:
.P
.nf
.ft CW
.S -2
LIST
    default_callbacks : CALLBACKS {
        XmNdestroyCallback = PROCEDURE app_destroy (k_main);
        XmNhelpCallback = PROCEDURE app_help (k_main);
    };
.fi
.S
.ft R
.mc
.P 0
.mc |
.ne 8
.mc
The following lines of pseudocode show the interface to the callback 
procedure:
.mc |
.P
.ft CW
.nf
PROCEDURE procedure-name (widget by reference,
                          tag by reference,
                          reason by reference 
          RETURNS: no-value);
.fi
.ft R
.mc
.P 0
Because the UIL compiler produces
a UID file rather than an object module (.o), the binding of the UIL name to
the address of the entry point to the procedure is not done by the loader, but
is established at run time with the MRM function
.mc |
\fBMrmRegisterNames\fR.
.mc
You call this function
before fetching any objects,
giving it both the UIL names and the procedure addresses of each callback.  The
name you register with MRM in the application program must match the name you
specified for the procedure in the UIL module.
.P 0
Each callback procedure receives three arguments.  The first two arguments
have the same form for each callback.  The form of the third argument varies
from object to object.  
.P 0
The first argument is the address of the data structure maintained by the 
Motif Toolkit
for this object instance.  This address is called the widget ID for 
this object.
.P 0
The second argument is the address of the value you specified in the callbacks
list for this procedure.  
.mc |
The XmNdestroyCallback callback in this example has \fIapp_destroy\fP 
as its callback procedure.  The second argument is address of the integer
\fIk_main\fP.  If you do not specify an argument, the address is NULL.
.mc
.P 0
.mc |
Consult Part I of this guide
.mc
to find the structure of the third argument.
The reason name you specified in the UIL module is the first
field in this
structure.
.mc *
.mc
.H 3 "Controls List Structure"
.P 0
.iX "UIL" "controls list"
.iX "List types" "control"
A controls list defines which objects are children of, or controlled by, 
a particular object.
Each entry in a controls list has the following syntax:
.mc |
.P
.nf
\fIcontrol-list-entry\fP ::=
        [ \fBMANAGED\fR | \fBUNMANAGED\fR ] \fIobject-definition\fP 
.fi
.ft R
.mc
.P 0
.mc |
If you specify the keyword \fBMANAGED\fR at run time, the object is created and 
managed; if you specify \fBUNMANAGED\fR at run time, the object is only created.
.mc
Objects are managed by default.
.P 0
.mc |
.ne 3i
.mc
Unlike the arguments list and the callbacks list, a controls list entry that is
identical to a previous entry does not supersede the previous
entry.  At run time, 
each controls list entry causes a child to be created when 
the parent is created.  If the same
object definition is used for multiple children, multiple instances of the
child are created at run time. 
.mc *
.mc
See Appendix B
for a list of which widget types can be
controlled by which other widget types.
.P 0
The following example shows how to declare a controls list:
.P 0
.mc |
.nf
.ft CW
.mc
 LIST
            default_main_controls : CONTROLS {
                XmCommand main_command;
                XmMenuBar main_menu;
                UNMANAGED XmList file_menu;
                UNMANAGED XmOptionMenu edit_menu;
        };
.mc |
.fi
.ft R
.mc
.H 3 "Procedures List Structure"
.P 0
You can specify multiple procedures for a callback reason
in UIL by defining a procedures list.  Just as
with other list types, lists can be defined inline or in
a list section and referenced by name.
.P 0
If you define a reason more than once (for example, when the reason is
defined both in a referenced procedures list and in the callbacks list for the
object), previous definitions are overridden by the latest definition.
.mc *
.mc
The syntax for a procedures list is as follows:
.mc |
.P
.nf
\fIprocedure-list-specification\fP ::=
    \fBPROCEDURES\fR \fIprocedure-list-spec\fP
\fIprocedure-list-spec\fP ::=
    { \fIprocedure-list-name\fP 
    | "{" [\fIprocedure-list-clause\fP...] "}" }
\fIprocedure-list-clause\fP ::=
    { \fIprocedure-list-specification\fP | \fIprocedure-list-ref\fP }
\fIprocedure-list-ref\fP ::=
    \fIprocedure-list-name\fP [ "(" [ \fItag-value-expression\fP ] ")" ]
\fIcallback-list-entry\fP ::=
    \fIreason-name\fP "=" \fIprocedure-list-spec\fP
.fi
.ft R
.mc
.P 0
.mc |
You can specify multiple procedures for each callback reason in UIL by defining
the procedures as a type of list.  Just as with other list types, you can
define procedures lists either inline, or in a list section and referenced by
name.  If you define a reason more than once (for example, when the reason is
defined both in a referenced procedures list and in the callbacks list for the
object), previous definitions are overridden by the latest definition.
.P 0
The following example shows how to specify multiple procedures per callback
reason in an object declaration for the push button gadget.  In the example,
both functions \fIquit_proc\fP and \fIshutdown\fP
are called in response to the XmNactivateCallback callback reason.
.P
.ft CW
.nf
OBJECT m_quit_button: XmPushButton {
    ARGUMENTS {
                 .
                 .
                 .
              };
    CALLBACKS {
       XmNactivateCallback = PROCEDURES
              {
                 quit_proc ('normal exit');
                 shutdown ();
              };
    };
};
.ft R
.fi
.mc
.H 2 "Structure of an Object Section"
.P 0
.iX "UIL" "object section" 
.iX "UIL module" "object section" 
.iX "UIL specification file" "object section" 
.iX "Object section" 
.iX "Object" "defining in UIL"
An object section consists of the keyword OBJECT followed by a
sequence of object declarations.  It has the following syntax:
.mc |
.P
.nf
\fIobject-section\fP ::=
        \fBOBJECT\fR \fIobject-declaration\fP...
\fIobject-declaration\fP ::=
       \fIobject-name\fP ":"
               { \fBEXPORTED\fR \fIobject-definition\fP
               | \fBPRIVATE\fR \fIobject-definition\fP
               | \fIobject-definition\fP
               | \fBIMPORTED\fR \fIobject-type\fP } ";"
\fIobject-definition\fP ::=
       \fIobject-type\fP [ \fIprocedure-reference\fP ] \fIobject-spec\fP
\fIobject-spec\fP ::=
       { \fIobject-name\fP [\fBWIDGET\fR | \fBGADGET\fR]
       | "{" \fIlist-definition\fP... "}" } 
\fIprocedure-reference\fP ::=
       \fBPROCEDURE\fR \fIcreation_function\fP
.ft R
.fi
.mc
.P 0
.iX "UIL" "object declaration"
.iX "UIL module" "object declaration"
.iX "UIL specification file" "object declaration"
.iX "Object declaration"
Use an object declaration to define the objects that are to be stored in the
UID file. You can reference the object name in declarations that
.mc |
occur elsewhere in the UIL module in any context where an object name can 
be used (for example, in a controls list, as a symbolic reference
to a widget ID, or as the tag_value argument for a callback procedure). 
.mc
Objects can be forward referenced; that is, you can declare an object
name after you reference it.  All references to an object name must be
consistent with the type of the object, as specified in the object declaration. 
.mc *
.mc
You can specify an object as exported, imported, or private. 
.P 0
The object definition contains a sequence of lists that define 
the arguments, hierarchy, and callbacks for the widget.  
You can specify only one list of each type for an object.
If you want to specify more than one list of arguments,
controls,
or callbacks, you can do so within one list, as follows:
.mc |
.P
.nf
\fIobject some_widget\fP:
    \fIarguments\fP {
         \fIarguments_list1\fP;
         \fIarguments_list2\fP;
    };
.fi
.ft R
.mc
.P 0
When you declare a user-defined widget, you must include a reference to the
widget creation function for the user-defined widget.
.mc |
See Section 22.5 for more information.
.mc
.P 0
.mc |
In this example, \fIarguments_list1\fR and \fIarguments_list2\fR
are lists of arguments that were previously defined in a list
.mc
section.
.mc |
The following example shows how to declare an object:
.mc
.P 0
.mc |
.ft CW
.nf
OBJECT 
     app_main : EXPORTED XmMainWindow {
         ARGUMENTS {
             ARGUMENTS default_args;
             XmNheight = 1000; 
             XmNwidth = 800;   
         };
         CALLBACKS default_callbacks;
         CONTROLS {
             XmMenuBar main_menu;
             user_defined my_object;
         };
     };      
.fi
.ft R
.mc
.P 0
.mc *
.mc
The following sections detail UIL syntax for specifying object variants,
show an example of a UIL module in which gadgets are specified (using both
.mc |
the default object-variant clause and explicit declaration methods), and 
.mc
describe UIL compiler diagnostics related to gadgets.
.mc *
.mc
.H 3 "Specifying the Object Variant in the Module Header"
.P 0
.iX "Objects clause"
.iX "Module header" "objects clause"
.iX "Object type specification" "using objects clause"
.P 0
.mc |
You can include a default object-variant clause in the module header to 
.mc
specify the default
variant of objects defined in the module on a type-by-type basis.
The object type can be any user interface object type that has a gadget
variant (cascade button, label, push button, separator, or toggle button).
If you specify any other object type as a gadget, the UIL compiler issues a 
diagnostic.
.P 0
.mc |
When you include an object type in the default object-variant clause, all 
.mc
objects of that
type default to the variant you specified in the clause.  For example,
.mc |
the following default object-variant clause specifies that all 
.mc
push buttons in the module are gadgets:
.mc |
.P
.nf
.ft CW
.mc
OBJECTS = { XmPushButton = GADGET; }
.mc |
.fi
.ft R
.mc
.P 0
The UIL compiler issues an informational diagnostic if you attempt to specify
.mc |
an object type more than once in the default object-variant clause.
.mc
.P 0
.mc |
You can override the specification you made in the default object-variant 
.mc
clause when you
.mc |
declare a particular object. If you omit the default object-variant clause, 
.mc
or omit an object
type from the clause, the UIL compiler assumes you want the omitted
type to be a widget.  You can also explicitly override this default in an object
declaration. 
.mc |
The example in Section 19.6.2 shows how to use the default object-variant
.mc
clause and how to override the variant specification in an object declaration.
.P 0
.H 3 "Specifying the Object Variant in the Object Declaration"
.P 0
.iX "Object type specification" "in object declaration"
.iX "Gadgets" "specifying in an object declaration"
.iX "Object section" "declaring gadgets in"
.iX "User interface object specification" "in object declaration"
.P 0
You can use one of the keywords WIDGET or GADGET as an attribute of an object
declaration.  You include the keyword between the object type and
the left brace of the object specification.   Use the GADGET or WIDGET
keyword to specify the object type or to override the default variant for this
object type.
.P 0
The syntax of the object declaration is as follows: 
.mc |
.P
.nf
.mc
OBJECT
.mc |
     \fIobject-name\fP : \fIobject-type\fP GADGET | WIDGET { 
.mc
.br
\0\0\0.
.br
\0\0\0.
.br
\0\0\0.
.br
     };
.mc |
.fi
.ft R
.mc
.P 0
The object type can be any user interface object type that has a gadget
.mc |
variant (cascade button, label, push button, separator, or toggle button).
.mc
If you specify any other object type as a gadget, the UIL compiler issues a 
diagnostic.
.iX "Gadgets" "specifying in an object declaration"
.iX "Object section" "declaring gadgets in"
.iX "UIL module" "defining gadgets in"
.P 0
.mc |
You can use the Motif Toolkit name of an object type that has a gadget
variant (for example, \fBXmLabelGadget\fP) as an attribute of an object
declaration.  The syntax of the object declaration is as follows:
.P
.nf
\fBOBJECT\fR
     \fIobject-name\fP : \fIobject-type\fP { 
.br
\0\0\0.
.br
\0\0\0.
.br
\0\0\0.
.br
     };
.fi
.ft R
.P 0
The \fIobject_type\fP can be any object type, including gadgets.
.P 0
.ne 2i
The following example shows how to specify gadgets:
.P
.nf
.ft CW 0
.S -2
.in0
.mc
MODULE sample
    NAMES = case_insensitive
    OBJECTS =
        { XmSeparator = GADGET; XmPushButton = WIDGET; }
    OBJECT
         a_button : XmPushButton GADGET {
              ARGUMENTS { XmNlabelString = 'choice a'; };
         };
         a_menu : XmPulldownMenu {
             ARGUMENTS { XmNborderWidth = 2; };
             CONTROLS {
                 XmPushButton a_button;
                 XmSeparator GADGET {};
                 XmPushButton {
                     ARGUMENTS { XmNlabelString = 'choice b'; };
                 };
                 XmSeparator WIDGET {};
                 XmPushButton c_button;
                 XmSeparator {};
             };
         };
.mc |
         c_button : XmPushButtonGadget {
.mc
              ARGUMENTS { XmNlabelString = 'choice c'; }; 
         };
END MODULE;
.mc |
.in
.fi
.ft R
.S
.mc
.P 0
.mc |
.br
.ne 5
In this example, the default object-variant clause specifies that all
.mc
separator objects are gadgets and all push button objects are widgets, unless
.mc |
overridden.  Object \fIa_button\fP is explicitly specified as a gadget.
Object \fIa_menu\fP defaults to a widget. 
Object \fIc_button\fP is explicitly specified as a gadget using the
toolkit name.
.mc
.P 0
.mc |
Notice that the reference to \fIa_button\fP in the controls list of \fIa_menu\fP
refers to the \fIa_button\fP gadget; you need to
include the gadget attribute only on the declaration of \fIa_button\fP,
not on each reference to \fIa_button\fP.  The same holds true for \fIc_button\fP,
even though the reference to \fIc_button\fP in the controls list for \fIa_menu\fP
.mc
is a forward reference.  
.mc |
The unnamed push button definition in the controls list for \fIa_menu\fP
is a widget because of the default object-variant clause; the last separator 
.mc
is a gadget for the same reason. 
.P 0
You need to specify the GADGET or WIDGET keyword only in the
declaration of an object, not when you reference the object.  You cannot
specify the GADGET or WIDGET keyword for a user-defined object; user-defined
objects are always widgets.
.mc |
.br
.ne 3i
.mc
.H 2 "Structure of an Identifier Section"
.P 0
.iX "Identifiers"
.iX "UIL" "identifiers"
The identifier section allows you to define an identifier, a mechanism that
achieves run-time binding of data to names that appear in a UIL module. 
The identifier section consists of
the reserved keyword IDENTIFIER, followed by a list of names, each name followed
by a semicolon.  You can later use these names in the UIL module as
.mc |
either the value of an argument to a widget or the tag value to a callback
procedure. At run time, you use the MRM functions
\fBMrmRegisterNames\fP and \fBMrmRegisterNamesInHierarchy\fP
.mc
to bind
.mc |
the identifier name with the address of the data associated with the identifier.
(See Chapter 21 for information about MRM functions.)
.mc
.P 0
.mc |
Each UIL module has a single name space; therefore, you cannot use a
name you used for a value, object, or procedure as an identifier name in
the same module.
.mc
.P 0
The following example shows how to use
an identifier section in a UIL 
module:
.mc |
.P
.nf
.ft CW
.mc
IDENTIFIER
    my_x_id;
    my_y_id;
    my_destroy_id;
.mc |
.fi
.ft R
.mc
.P 0
The UIL compiler does not do any type checking on the use of identifiers in a
UIL module.  Unlike a UIL value, an identifier does not have a UIL type
associated with it.  Regardless of what particular type a widget argument or
callback procedure tag is defined to be, you can use an identifier in that
context instead of a value of the corresponding type.
.P 0
To reference these identifier names in a UIL module, you use the name of
the identifier wherever you want its value to be used.
Identifiers can be referenced in any context where a value can be
referenced in UIL, although the primary uses for identifiers are as callback
procedure tags and widget argument values.
.P 0
The UIL module in
the following example, the identifiers
\fImy_x_id\fP
and
\fImy_y_id\fP
are used as argument values for the main window widget,
\fImy_main\fP.
The position of the main window widget may depend on the screen size of the
terminal on which the interface is displayed.  Using identifiers, you can
provide the values of the
.mc |
XmNx
.mc
and
.mc |
XmNy
.mc
arguments at run time.
The identifier named
\fImy_destroy_id\fP
is specified as the tag to the callback procedure
\fImy_destroy_callback\fP.
In the application program, you could allocate a data structure and use
\fImy_destroy_id\fP
to store the address of the data structure.  When the 
.mc |
XmNdestroyCallback
.mc
reason occurs, the data structure is passed as the tag to procedure
\fImy_destroy_callback\fP.
.P 0
.mc |
.P
.nf
.ft CW 
.S -2
.mc
MODULE id_example
  NAMES = CASE_INSENSITIVE
    IDENTIFIER
        my_x_id;
        my_y_id;
        my_destroy_id;
    PROCEDURE
        my_destroy_callback ( STRING );
    OBJECT my_main : XmMainWindow {
        ARGUMENTS {
.mc |
          XmNx = my_x_id;
          XmNy = my_y_id;
.mc
        };
        CALLBACKS {
.mc |
          XmNdestroyCallback = PROCEDURE my_destroy_callback 
                  ( my_destroy_id );
.mc
        };
    };
END MODULE;
.mc |
.S
.fi
.ft R
.mc
.H 2 "Include Directive"
.P 0
.iX "UIL" "include directive"
.iX "UIL module" "include directive"
.iX "UIL specification file" "include directive"
.iX "Include directive"    
The include directive incorporates the contents of a specified
file into a UIL module.  This mechanism allows several UIL modules to share
common definitions.  The syntax for the include directive is as follows: 
.mc |
.P
.nf
\fIinclude-directive\fP ::=
        \fBINCLUDE FILE\fR \fIcharacter-expression\fP ";"
.fi
.ft R
.mc
.P 0
The file specified in the include directive is called an 
include file.  
The UIL 
compiler replaces the include directive with the contents of the include file
and processes it as if these contents had appeared in the current
UIL source file.  
.P 0
.mc |
.ne 3i
.mc
You can nest include files; that is, an include
file can contain include directives.
.mc |
The UIL compiler can process up to 100 references (including the file
containing the UIL module).  Therefore, you can include up to 99 files
.mc
in a single UIL module, including nested files.  Each time a file is
opened counts as a reference, so including the same file twice counts
as two references.
.P 0
The character expression is a file specification that identifies the
file to be included.  The rules for finding the specified file are
.mc |
similar to the rules for finding header, or \fB.h\fR files using
.mc
the include directive, 
.mc |
\fB#include\fR, 
.mc
with a quoted string in C.
.P 0
If you do
not supply a directory, the UIL compiler searches for the include file in the
directory of the main source file; if the compiler does not find the include
file there, the compiler looks in the same directory as the source
file. 
If you supply a directory, the UIL compiler searches
only that directory for the file. 
.P 0
.mc |
.ne 3
.mc
The following example shows how to use
the include directive:
.mc |
.P
.nf
.ft CW
.mc
    INCLUDE FILE 'constants';
.mc |
.fi
.ft R
.mc
.P 0
.mc |
.H 2 "Definitions for Constraint Arguments"
.mc
.P 0
.mc |
The Motif Toolkit and the X Toolkit (intrinsics) 
.mc
support constraint
.mc |
arguments.
A constraint argument is one that is passed
to children of an object, beyond those 
arguments normally available.
For example, the Form widget grants a set of constraint arguments to its
children.
These arguments control the position of the children within the Form.
.mc
.P 0
Unlike the arguments used to define the 
attributes of a particular
widget, constraint arguments are 
.mc |
used exclusively to define additional attributes of the children of a
particular widget.
These attributes affect the behavior of the children
within their parent.
To supply constraint arguments to the children,
you include the arguments 
in the arguments list for the
child, as shown in the following example:
.P
.nf
.ft CW
.S -2
.mc
OBJECT
.mc |
  my_form : XmForm {
    arguments {
        XmNx = 70;
        XmNy = 20;
        XmNrows = 35;
     };	  
.ne 1.5i
    CONTROLS {
        XmPushButton {
             ARGUMENTS {
                XmNleftAttachment = XmATTACH_WIDGET; !Constraint argument
                XmNleftOffset = 10;                  !Constraint argument
             };
         };   	 
     };
   };
.S
.fi
.ft R
.mc
.H 2 "Symbolic Referencing of Widget IDs"
.P 0
The UIL compiler allows you to reference a widget ID symbolically by 
using its name.  This mechanism addresses the problem that the UIL compiler 
views widgets by name and the Motif Toolkit views widgets by widget 
ID.  Widget IDs are defined at run time and are therefore unavailable for use
in a UIL module.
.P 0
When you need to supply an argument that requires a widget ID, 
you can use
the UIL name of that widget (and its object type) as the argument.  
.mc |
For example, the children of a Form widget can have an argument that
references a
widget as the anchor point for a top attachment.
.mc
You give the type and name of
the object you want to use for the top attachment as the value for this 
argument.
.P 0
The
widget name you reference must be a descendant of the widget being
fetched for MRM to find the referenced widget; you cannot 
reference an arbitrary widget.  MRM checks this at run time.   
For example, a practical use of symbolic references is to specify
the default push button (in a bulletin board or radio box). 
.P 0
.mc |
.ne 3i
.mc
The following example shows how to use
a symbolic reference:
.P 0
.mc |
.nf
.ft CW
.S -2
.mc
MODULE 
    NAMES = CASE_INSENSITIVE
        OBJECT my_dialog_box : XmBulletinBoard {
            ARGUMENTS {
                XmNdefaultButton = XmPushButton yes_button;
            };
            CONTROLS {
                XmPushButton yes_button;
                XmPushButton no_button;
            };
        };
        OBJECT yes_button : XmPushButton {
            ARGUMENTS {
                XmNlabelString = 'yes';
            };
        };
        OBJECT no_button : XmPushButton {
            ARGUMENTS {
                XmNlabelString = 'no';
            };
        };
END MODULE;
.mc |
.S
.fi
.ft R
.mc
.P
.mc |
.br
.ne 8
In this example, two PushButton widgets are defined, named \fIyes_button\fP
and \fIno_button\fP.  In the definition of the BulletinBoard widget, the name
\fIyes_button\fP is given as the value for the \fBXmNdefaultButton\fP
.mc
argument.  Usually, this argument accepts a widget ID.  When you use a symbolic
.mc |
reference (the object type and name of the \fIyes_button\fP
widget) as the value for the \fBXmNdefaultButton\fP
argument, MRM substitutes the widget ID of the \fIyes_button\fP
PushButton for its name at run time.
.mc
.P 0
.mc |
Symbolic referencing of widget IDs is acceptable for any argument whose
resource class is \fBXmCWidget\fP.
See the \fIOSF/Motif Programmer's Reference\fP for resource names and
classes.
.mc
