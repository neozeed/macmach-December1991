.mc |
.\"  (c) Copyright 1989, 1990 OPEN SOFTWARE FOUNDATION, INC.
.mc
.\"  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
.\"  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
.\"  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
.\"  (c) Copyright 1988 MICROSOFT CORPORATION
.\"  ALL RIGHTS RESERVED
.mc |
...\".nr !_ 1
...\".pn 1
...\".SK
.nr H1 3
.af H1 A
.H 1 "UIL Diagnostic Messages"
...\".nr !_ 0
.mc
.P
.iX "Diagnostic messages"
.mc |
This appendix lists the diagnostic messages produced by the UIL
compiler.  The severity, a description of the
message, and a suggestion for correcting the problem are listed for each message.
.mc
The following strings are used to represent data that varies
in the actual message you receive from the UIL compiler:
.TS H
tab(@);
l l
l l.
.mc |
.R
\fBString@Data Represented\fR
.mc
.sp 6p
.mc *
.mc
.sp 6p
.mc *
.mc
.TH
 %c@Character
 %d@Decimal number
 %s@String
.sp 6p
.mc *
.mc
.TE
.P
Messages are listed alphabetically by IDENT code.
.VL 1.5i
.LI "arg_count" 1.5i
procedure %s was previously declared with %d arguments
.mc |
.sp .75
.ne 4
.B Severity:  
Error
.sp .75
.mc
The declaration of the marked procedure specified a different number of
arguments than are present in this procedure reference.
.sp
.mc |
.ne 2i
.ne 4
.mc
.B "User Action:"
.sp
Check that you are calling the correct function.  If you intend to call the
procedure with a varying number of arguments, omit the argument list in the
procedure declaration.
.LI "arg_type" 1.5i
found %s value - procedure %s argument must be %s value
.sp
.mc |
.ne 4
.B Severity: 
Error
.mc
.sp
The declaration of the marked procedure specified a different type of argument
than is present in this procedure reference.
.sp
.mc |
.ne 4
.mc
.B "User Action:"
.sp
Check that you are passing the correct argument to the correct function.  If
you intend to call the procedure with varying argument types, declare the
procedure specifying 
\fBany\fP 
for the type of the argument.
.LI "backslash_ignored" 1.5i
unknown escape sequence "\\%c" -  ignored
.sp
.mc |
.ne 4
.B Severity: 
Error
.mc
.sp
.mc |
A backslash was followed by an unknown escape character. The \\ (backslash) is
.mc
the escape character in UIL.  A selected set of single characters can follow a
backslash such as \\n
for newline or \\\\
to insert a backslash.  The character
following the backslash was not one of the selected set.
.sp
.mc |
.ne 4
.mc
.B "User Action:"
.sp
If you want to add a backslash, use \\\\.
.mc |
See Section 2.4.1 for
.mc
a description of the supported escape sequences.
.LI "bug_check" 1.5i
internal error: %s
.sp
.mc |
.ne 4
.B Severity:  
Severe
.mc
.sp
The compiler diagnosed an internal error.
.sp
.mc |
.ne 4
.mc
.B "User Action:"
.sp
Submit a software problem report.
.LI "circular_def" 1.5i
widget %s is part of a circular definition
.sp
.mc |
.ne 4
.B Severity:  
Error
.mc
.sp
The indicated object is referenced as a descendant of itself, either within its
own definition or within the definition of one of the objects in the widget
.mc |
tree it controls.
.mc
.sp
.mc |
.ne 4
.mc
.B "User Action:"
.mc |
.sp .75
.mc
Change the definition of the indicated object so that it is not a descendant of
itself.
.LI "control_char" 1.5i
unprintable character \\%d\\ ignored
.mc |
.sp .75
.ne 4
.B Severity:  
Error
.sp .75
The compiler encountered an illegal control character in the UIL specification
file.  The decimal value of the character is given between the \\ (backslash)
.mc
characters.
.mc |
.sp .75
.ne 4
.mc
.B "User Action:"
.mc |
.sp .75
.mc
Replace the character with the sequence specified in  the message (for 
example, \\3 if the control character's internal value  is 3).  UIL provides 
several built-in control characters such as \\n  and \\r for newline and 
.mc |
carriage return.  See Section 2.4.1 for a complete list of
.ne 3
supported escape sequences.
.mc
.LI "create_proc" 1.5i
creation procedure is not supported by the %s widget
.mc |
.sp .75
.ne 4
.B Severity:  
Error
.sp .75
.mc
You specified a creation procedure for a Motif Toolkit widget.  You
can specify a creation procedure only for a user-defined widget.
.sp
.mc |
.ne 4
.mc
.B "User Action:"
.sp
Remove the procedure clause following the object type.
.LI "create_proc_inv" 1.5i
creation procedure is not allowed in a %s widget reference
.sp
.mc |
.ne 4
.B Severity:  
Error
.mc
.sp
You specified a creation procedure when referencing an object.  You can specify
a creation procedure only when you declare the object.
.sp
.mc |
.ne 4
.mc
.B "User Action:"
.sp
Remove the procedure clause following the object type.
.LI "create_proc_req" 1.5i
creation procedure is required in a %s widget declaration
.sp
.mc |
.ne 4
.B Severity:  
Error
.mc
.sp
When defining a user-defined widget, you must specify the name of the
creation function for creating an instance of this widget.
.sp
.mc |
.ne 4
.mc
.B "User Action:"
.sp
Insert a procedure clause following the widget type in the widget declaration. 
.mc |
You also need to declare the
.ne 4
creation procedure using a procedure declaration.  For
example:
.sp
.nf
.ft CW
.S -2
procedure my_list_box_creation_proc();
object list_box: 
 user_defined procedure my_list_box_creation_proc()
    { arguments ... };
.S
.sp .5
.fi
.ft R
.mc
.LI "ctx_req" 1.5i
context requires a %s - %s was specified
.sp
.mc |
.ne 4
.B Severity:  
Error
.mc
.sp
At the point marked in the specification, one type of object (such as a
widget) is required and your specification supplied a different type of object
(such as value).
.sp
.mc |
.ne 4
.mc
.B "User Action:"
.sp
Check for misspelling or that you have referred to the intended object.
.LI "d_add_source" 1.5i
additional UIL source file: %s was ignored
.sp
.mc |
.ne 4
.B Severity:  
Error
.mc
.sp
More than one source file was specified.  Only the first source file will be 
compiled.
.sp
.mc |
.ne 4
.mc
.B "User Action:"
.sp
Compile additional source files using separate invocations of the compiler.
.LI "d_dupl_opt" 1.5i
duplicate option \\ "%s" \\
was ignored
.sp
.mc |
.ne 4
.B Severity:  
Warning
.mc
.sp
The same command line option has been repeated more than once (for example, the
"-o" option or the "-v" option)
.sp
.mc |
.ne 4
.mc
.B "User Action:"
.sp
Remove duplicate command line option.
.LI "d_miss_opt_arg" 1.5i
%s missing following  \\
"%s" \\
option
.sp
.mc |
.ne 4
.B Severity:  
Error
.mc
.sp
You used a command line option that requires an argument and you did not
provide that argument.
.sp
.mc |
.ne 4
.mc
.B "User Action:"
.sp
Omit the option or provide the argument.
.LI "d_no_source" 1.5i
no source file specified
.sp
.mc |
.ne 4
.B Severity:  
Severe
.mc
.sp
No source file was specified to compile.  
.sp
.mc |
.ne 4
.mc
.B "User Action:"
.sp
Specify the name of a UIL specification file to compile.
.LI "d_unknown_opt" 1.5i
unknown option \\ "%s" \\
was ignored
.sp
.mc |
.ne 4
.B Severity:  
Warning
.mc
.sp
An unknown option has been used in the compiler command line.
.sp
.mc |
.ne 4
.mc
.B "User Action:"
.sp
Check what you typed on the command line.
.LI "dup_letter" 1.5i
color letter used for prior color in this table
.sp
.mc |
.ne 4
.B Severity:  
Error
.mc
.sp
Each of the letters used to represent a color in a color table must be unique. 
.mc |
If not, that letter in an icon would represent more than one color; each pixel
.mc
can have only one color associated with it at a time.  The letter marked has
been assigned to more than one color.
.sp
.mc |
.ne 4
.mc
.B "User Action:"
.sp
.mc |
Choose which color the letter is to represent and remove any duplicates or assign 
them a new character.
.mc
.LI "dup_list" 1.5i
%s %s already specified for this %s %s
.sp
.mc |
.ne 4
.B Severity:  
Error
.mc
.sp
A widget or gadget declaration can have at most one arguments list, one 
callbacks list, and one controls list.
.sp
.mc |
.ne 4
.mc
.B "User Action:"
.sp
If you want to specify multiple lists of arguments, controls, and callbacks,
you can do so within one list.  For example:
.mc |
.sp
.nf
.ft CW
.S -2
arguments { arguments_list1; arguments_list2; };
.S
.ft R
.fi
.sp .5
.mc
.LI "gadget_not_sup" 1.5i
%s gadget is not supported - %s widget will be used instead
.sp
.mc |
.ne 4
.B Severity:  
Warning
.mc
.sp
The indicated object type does not support a gadget variant; only a widget
variant is supported for this object type.  The UIL compiler ignores the gadget
indication, and creates widgets of this object type.
.sp
.mc |
.ne 4
.mc
.B "User Action:"
.sp
Specify that this object type is a widget instead of a gadget.
.LI "icon_letter" 1.5i
row %d, column %d: letter \\"%c"\\ not in color table
.sp
.mc |
.ne 4
.B Severity:  
Error
.mc
.sp
You have specified a color to be used in an icon that is not in that icon's
color table.  The invalid color is identified in the message by displaying
.mc |
the letter used to represent that color between the \\ (backslashes).   
.mc
This
letter was not defined in the specified color table.
.sp
.mc |
.ne 4
.mc
.B "User Action:"
.sp
Either add the color to the icon's color table or use a character representing
.mc |
a color in the color table.  The default color table  defines " "
(double quotation marks) as background
and * (asterisk) as foreground.
.mc
.LI "icon_width" 1.5i
row %d must have same width as row 1
.sp
.mc |
.ne 4
.B Severity:  
Error
.mc
.sp
The icons supported by UIL are rectangular (that is, x pixels wide by y pixels
high).  As a result, each of the strings used to represent a row of pixels in
an icon must have the same length.  The specified row does not have the same
length as the first row.
.sp
.mc |
.ne 4
.mc
.B "User Action:"
.sp
Make all the strings in the icon function the same length.
.LI "inv_module" 1.5i
invalid module structure - check UIL module syntax
.sp
.mc |
.ne 4
.B Severity:  
Error
.mc
.sp
The structure of the UIL module is incorrect.
.sp
.mc |
.ne 4
.mc
.B "User Action:"
.sp
If there are any syntax errors reported, fix them and recompile.  For example,
if the error occurs before the first object declaration (that is, before your
value and object declarations), check the syntax of the module header for
.mc |
unwanted ; (semicolons) after the module clauses.  If the error occurs at the
.mc
end of the module, check that the module concludes with the keywords "end
module;".
.LI "list_item" 1.5i
%s item not allowed in %s %s
.sp
.mc |
.ne 4
.B Severity:  
Error
.mc
.sp
The indicated list item is not of the type required by the list.  Arguments
lists must contain argument entries, callbacks lists must contain callback
entries, controls lists must contain control entries, and procedures
lists must contain callback entries.
.sp
.mc |
.ne 4
.mc
.B "User Action:"
.sp
Check the syntax for the type of list entry that is required in this context
and change the indicated list item.
.LI "listing_open" 1.5i
error opening listing file: %s
.sp
.mc |
.ne 4
.B Severity:  
Severe
.sp .75
.mc
The compiler could not create the listing file noted in the message.
.mc |
.sp .75
.ne 4
.mc
.B "User Action:"
.mc |
.sp .75
.mc
Check that you have write access to the directory you specified to hold the
listing file.
.LI "listing_write" 1.5i
error writing to listing file: %s
.mc |
.sp .75
.ne 4
.B Severity:  
Severe
.sp .75
.mc
The compiler could not write a line into the listing file noted in the message.
.mc |
.sp .75
.ne 4
.mc
.B "User Action:"
.mc |
.sp .75
.mc
Check to see that there is adequate space on the disk specified to hold the
listing file.
.LI "name_too_long" 1.5i
name exceeds 31 characters - truncated to: %s
.mc |
.sp .75
.ne 4
.B Severity:  
Error
.sp .75
.mc
The UIL compiler encountered a name longer than 31 characters. The compiler
truncated the name to the leftmost 31 characters.
.mc |
.sp .75
.ne 4
.mc
.B "User Action:"
.mc |
.sp .75
.mc
Shorten the name in the UIL module source.
.LI "names" 1.5i
place names clause before other module clauses
.mc |
.sp .75
.ne 4
.B Severity:  
Error
.sp .75
The case-sensitivity clause, if specified, must be the first clause following
.mc
the module's name.  You have inserted another module clause before this clause.
.mc |
.sp .75
.ne 4
.mc
.B "User Action:"
.mc |
.sp .75
Reorder the module clauses so that the case-sensitivity clause is first. (It is
acceptable to place the version clause ahead of the case-sensitivity clause;
.mc
this is the only exception.)
.LI "never_def" 1.5i
%s %s was never defined
.mc |
.sp .75
.ne 4
.B Severity:  
Error
.sp .75
.mc
Certain UIL objects such as gadgets and widgets can be referred to before they
.mc |
are defined.  The marked object is such an object.  However, the compiler never
.mc
found the object's declaration.
.mc |
.sp .75
.ne 4
.mc
.B "User Action:"
.mc |
.sp .75
.mc
Check for misspelling.   If the module is case sensitive, the spellings of
names in declarations and in references must match exactly.
.LI "no_uid" 1.5i
no UID file was produced
.mc |
.sp .75
.ne 4
.B Severity:  
Informational
.sp .75
.mc
If the compiler reported error or severe diagnostics (that is, any of the
diagnostic abbreviations starting with %UIL-E or %UIL-F), a UID file is not
created.  This diagnostic informs you that the compiler did not produce a UID
file.
.sp
.mc |
.ne 4
.mc
.B "User Action:"
.sp
Fix the problems reported by the compiler.
.LI "non_pvt" 1.5i
value used in this context must be private
.sp
.mc |
.ne 4
.B Severity:  
Error
.mc
.sp
A private value is one that is not imported or exported.  In the context marked
by the message, only a private value is legal.  Situations where this message
.mc |
is issued include defining one value in terms of another, and arguments to
.mc
functions.  In general, a value must be private when the compiler must know the
value at compilation time.  Exported values are disallowed in these contexts,
even though a value is present, because that value could be overridden at run
time.
.sp
.mc |
.ne 4
.mc
.B "User Action:"
.sp
Change the value to be private.
.LI "not_impl" 1.5i
%s is not implemented yet
.sp
.mc |
.ne 4
.B Severity:  
Error
.mc
.sp
You are using a feature of UIL that has not been implemented.
.sp
.mc |
.ne 4
.mc
.B "User Action:"
.sp
Try an alternate technique.
.LI "null" 1.5i
.mc |
a null character in a string is not supported
.mc
.sp
.mc |
.ne 4
.B Severity:  
Warning
.mc
.sp
You have created a string that has an embedded null character. Strings are
represented in a UID file and in many Motif Toolkit data structures
as null terminated strings.  So, although the embedded nulls will be placed in
.mc |
the UID file, Motif Toolkit functions may interpret an embedded null
.mc
as the terminator for the string.
.sp
.mc |
.ne 4
.mc
.B "User Action:"
.sp
Be very careful using embedded nulls.
.LI "obj_type" 1.5i
found %s %s when expecting %s %s
.sp
.mc |
.ne 4
.B Severity:  
Error
.mc
.sp
Most arguments take values of a specific type.  The value specified is not
correct for this argument.
.sp
.mc |
.ne 4
.mc
.B "User Action:"
.sp
The message indicates the expected type of argument. Check that you have
specified the intended value and that you specified the correct argument.
.LI "operand_type" 1.5i
%s type is not valid for %s
.sp
.mc |
.ne 4
.B Severity:  
Error
.mc
.sp
The indicated operand is not of a type that is supported by this operator.
.sp
.mc |
.ne 4
.mc
.B "User Action:"
.sp
Check the definition of the operator and make sure the type of the operand you
specify is supported by the operator.
.LI "out_of_memory" 1.5i
compiler ran out of virtual memory
.sp
.mc |
.ne 4
.B Severity:  
Severe
.mc
.sp
The compiler ran out of virtual memory.
.sp
.mc |
.ne 1i
.mc
.B "User Action:"
.sp
Reduce the size of your application.
.LI "out_range" 1.5i
value of %s is out of range %s
.sp
.mc |
.ne 4
.B Severity:  
Error
.mc
.sp
The value specified is outside the legal range of its type.
.sp
.mc |
.ne 4
.mc
.B "User Action:"
.sp
Change the UIL module source.
.LI "prev_error" 1.5i
compilation terminated - fix previous errors
.sp
.mc |
.ne 4
.B Severity:  
Severe
.mc
.sp
Errors encountered during the compilation have caused the compiler to abort.
.sp
.mc |
.ne 4
.mc
.B "User Action:"
.sp
Fix the errors already diagnosed by the compiler and recompile.
.LI "previous_def" 1.5i
name %s previously defined as %s
.sp
.mc |
.ne 4
.B Severity:  
Error
.mc
.sp
The name marked by the message was used in a previous declaration. UIL requires
that the names of all objects declared within a module be unique.
.sp
.mc |
.ne 4
.mc
.B "User Action:"
.sp
Check for a misspelling. If the module is case sensitive, the spellings of
names in declarations and in references must match exactly.
.LI "single_letter" 1.5i
color letter string must be a single character
.sp
.mc |
.ne 1i
.B Severity:  
Error
.mc
.sp
The string associated with each color in a color table must hold exactly one
character.  You have specified a string with either fewer or more characters.
.sp
.mc |
.ne 4
.mc
.B "User Action:"
.sp
Use a single character to represent each color in a color table.
.LI "single_occur" 1.5i
%s %s supports only a single %s %s
.sp
.mc |
.ne 4
.B Severity:  
Warning
.mc
.sp
You have specified a particular clause more than once in a context where that
.mc |
clause can occur only once.  For example, the version clause in the module can
.mc
only occur once.
.sp
.mc |
.ne 4
.mc
.B "User Action:"
.sp
Choose the correct clause and delete the others.
.LI "src_limit" 1.5i
too many source files open: %s
.sp
.mc |
.ne 4
.B Severity:  
Severe
.mc
.sp
The compiler has a fixed limit for the number of source and include files that
it can process.  This number is reported in the message.  
.sp
.mc |
.ne 4
.mc
.B "User Action:"
.sp
Use fewer include files.
.LI "src_null_char" 1.5i
source line contains a null character
.sp
.mc |
.ne 4
.B Severity:  
Error
.mc
.sp
The specified source line contains a null character.  The compiler ignores any
text following the null character.
.sp
.mc |
.ne 1i
.mc
.B "User Action:"
.sp
.mc |
Replace each null character with the escape sequence \\ (backslash).
.mc
.LI "src_open" 1.5i
error opening source file: %s
.sp
.mc |
.ne 4
.B Severity:  
Severe
.mc
.sp                 
The compiler could not open the UIL specification file listed in the message.
.sp
.mc |
.ne 4
.mc
.B "User Action:"
.sp
Check that the file listed in the message is the one you want to compile, that
it exists, and that you have read access to the file.  If you are using a large
number of include files, you may have exceeded your quota for open files.
.LI "src_read" 1.5i
error reading next line of source file: %s
.sp
.mc |
.ne 4
.B Severity:  
Severe
.mc
.sp
The compiler could not read a line of the UIL specification file listed in the
message.
.sp
.mc |
.ne 4
.mc
.B "User Action:"
.sp
In the listing file, this message should appear following the last line the
compiler read successfully.  First check that the file you are compiling is a
UIL specification file.  If it is, the file mostly likely contains corrupted
records.
.LI "src_truncate" 1.5i
line truncated at %d characters
.sp
.mc |
.ne 4
.B Severity:  
Error
.mc
.sp
The compiler encountered a source line greater than 132 characters. Characters
beyond the 132 character limit were ignored.
.sp
.mc |
.ne 1i
.mc
.B "User Action:"
.sp
Break each source line longer than 132 characters into several source lines. 
Long string literals can be created using the concatenation operator.
.LI "submit_spr" 1.5i
internal error - submit an SPR
.sp
.mc |
.ne 4
.B Severity:  
Severe
.mc
.sp
The compiler diagnosed an internal error.
.sp
.mc |
.ne 4
.mc
.B "User Action:"
.sp
Get a listing and look where the error is being issued.  Try fixing any faulty
syntax in this area.  If you are unable to prevent this error, submit a
software problem report.
.LI "summary" 1.5i
errors: %d  warnings: %d  informationals: %d
.sp
.mc |
.ne 4
.B Severity:  
Informational
.mc
.sp
This message lists a summary of the diagnostics issued by the compiler, and
appears only when diagnostics have been issued.
.sp
.mc |
.ne 4
.mc
.B "User Action:"
.sp
Fix the problems reported.  You can use the -I option qualifier to suppress
informational and warning diagnostics that you have determined to be harmless.
.LI "supersede" 1.5i
this %s %s supersedes a previous definition in this %s %s
.sp
.mc |
.ne 4
.B Severity:  
Informational
.mc
.sp
An argument or callback list has either a duplicate argument or duplicate
reason.  
.sp
.mc |
.ne 1i
.mc
.B "User Action:"
.sp
This is not necessarily an error.  The compiler is alerting you to make sure
.mc |
that you intended to override a prior argument's value. This informational
.mc
message can be suppressed using the -I option qualifier.
.LI "syntax" 1.5i
unexpected %s token seen - parsing resumes after 
\\"%c"\\
.sp
.mc |
.ne 4
.B Severity:  
Error
.mc
.sp
At the point marked in the module, the compiler found a construct such as a
punctuation mark, name, or keyword when it was expecting a different
construct.  The compiler continued analyzing the module at the next occurrence
of the construct stated in the message.
.sp
.mc |
.ne 4
.mc
.B "User Action:"
.sp
Check the syntax of your UIL module at the point marked by the compiler.  If
the module specifies case-sensitive names, check that your keywords are in
lowercase characters.
.LI "too_many" 1.5i
too many %ss in %s, limit is %d
.sp
.mc |
.ne 4
.B Severity:  
Error
.mc
.sp
You exceeded a compiler limit such as the number of fonts in a font table or
the number of strings in a translation table.  The message indicates the limit
imposed by the compiler.
.sp
.mc |
.ne 4
.mc
.B "User Action:"
.sp
Restructure your UIL module.
.LI "uid_open" 1.5i
error opening UID file: %s
.sp
.mc |
.ne 4
.B Severity:  
Severe
.mc
.sp
The compiler could not create the UID file noted in the message. A UID file
.mc |
holds the compiled user-interface specification.
.mc
.sp
.mc |
.ne 4
.mc
.B "User Action:"
.sp
Check that you have write access to the directory you specified to hold the UID
file.  If you have a large number of source and include files, check that you
have not exceeded your open file quota.
.LI "undefined" 1.5i
%s %s must be defined before this reference
.sp
.mc |
.ne 4
.B Severity:  
Error
.mc
.sp
The object pointed to in the message was either never defined or not defined
.mc |
prior to this point in the module.
The compiler requires the object to be
defined before
.ne 2
you refer to the object.
.mc
.sp
.mc |
.ne 4
.mc
.B "User Action:"
.sp
Check for a misspelling of the object's name, a missing declaration for the
object, or declaring the object after its first  reference.   If names in the
module are case sensitive, the spellings of the name in the declaration and in
the reference must match exactly.
.LI "unknown_charset" 1.5i
unknown character set
.mc |
.sp .75
.ne 4
.B Severity:  
Error
.sp .75
.mc
The message is pointing to a context where a character set name is required. 
You have not specified the name of a character set in that context.
.sp
.mc |
.ne 4
.mc
.B "User Action:"
.sp
Check for misspelling.  A list of the supported character sets is given in
.mc |
Section 2.1.  If you specified case-sensitive names in the
.mc
module, check that the character set name is in lowercase characters.
.LI "unknown_seq" 1.5i
unknown sequence \\"%s"\\ ignored
.sp
.mc |
.ne 4
.B Severity:  
Error
.mc
.sp
The compiler detected a sequence of printable characters it did not understand. 
.mc |
The compiler omitted the sequence of characters listed between the " "
(double quotation marks).
.mc
.sp
.mc |
.ne 4
.mc
.B "User Action:"
.sp
Fix the UIL module source.
.LI "unsupported" 1.5i
the %s %s is not supported for the %s %s
.sp
.mc |
.ne 4
.B Severity:  
Warning
.mc
.sp
Each widget or gadget supports a specific set of arguments, reasons, and
children.  The particular argument, reason, or child you specified is not
supported for this widget or gadget.
.sp
.mc |
.ne 4
.mc
.B "User Action:"
.sp
See the UIL built-in tables in Appendix B for the arguments,
reasons, and children  supported for each object.  If a widget creation
function accepts an  argument that UIL rejects, this does not necessarily
indicate that the UIL compiler is in error.  Widget creation
functions ignore arguments
that they do not support, without notifying you that the argument is being
ignored.
.LI "unterm_seq" 1.5i
%s not terminated %s
.sp
.mc |
.ne 4
.B Severity:  
Error
.mc
.sp
The compiler detected a sequence that was not properly terminated, such as a
string literal without the closing quotation mark.
.sp
.mc |
.ne 4
.mc
.B "User Action:"
.sp
Insert the proper termination characters.
.LI "wrong_type" 1.5i
found %s value when expecting %s value
.sp
.mc |
.ne 4
.B Severity:  
Error
.mc
.sp
The indicated value is not of the specific type required by UIL in this
context.
.sp
.mc |
.ne 4
.mc
.B "User Action:"
.sp
Check the definition of the function or clause.
.LE
