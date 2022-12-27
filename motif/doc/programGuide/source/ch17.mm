.mc |
.\"  (c) Copyright 1989, 1990 OPEN SOFTWARE FOUNDATION, INC.
.mc
.\"  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
.\"  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
.\"  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
.\"  (c) Copyright 1988 MICROSOFT CORPORATION
.\"  ALL RIGHTS RESERVED
.mc |
.nr H1 16
.H 1 "Introduction to the User Interface Language"
.mc
This chapter describes the features and benefits of the User 
Interface Language (UIL) and provides an overview of UIL and the Motif
Resource Manager (MRM).
.H 2 "Overview of UIL and MRM"
.P 0
.iX "UIL" "overview"
.iX "MRM" "overview"
UIL is a specification language for describing
the initial state of a user interface for a Motif application. The
specification describes the objects (for example, menus, form boxes, labels,
and push buttons) used in the interface and specifies the functions to be called
when the interface changes state as a result of user interaction. 
.P 0
To create a user interface with UIL and MRM, you perform the following:
.AL 1
.LI 
.mc |
Specify the user interface in a UIL module, which you store in a UIL
.mc
specification file.
.LI
.mc |
Compile the UIL specification file to generate a User Interface
.mc
Definition (UID) file.
.mc |
.br
.ne 3
.mc
.LI
.mc |
In your application program, use MRM run-time functions
to open the UID file and access the interface definitions.
.mc
MRM builds the necessary argument lists and calls widget creation
functions in the Motif Toolkit.
.LE
.P 0
Using UIL, you can specify the following:
.BL
.LI
Objects (widgets and gadgets) that comprise your interface
.LI
Arguments (attributes) of the widgets and gadgets you specify
.LI
Callback functions for each object
.LI
The widget tree for your application
.LI
.mc |
Literal values that can be fetched by the application at run time
.mc
.LE
.P 0
The UIL compiler has built-in tables containing information about widgets.
For every widget in the Motif Toolkit, the UIL compiler knows the
widgets that are valid children of the widget, the widget arguments,
and the valid callback reasons for the widget.  The UIL compiler uses this
information to check the validity of your interface specification at
compilation
time, to help you reduce run-time errors.
.P 0
.H 2 "Benefits of Using UIL and MRM"
.P 0
Creating a user interface for a Motif application using
.mc |
UIL and MRM offers the following advantages:
.mc
.BL
.LI
.mc |
Easier coding
.mc
.LI
.mc |
Earlier error detection
.mc
.LI
.mc |
Separation of form and function
.mc
.LI
.mc |
Faster prototype development
.mc
.LI
.mc |
Interface customization
.mc
.LE
.H 3 "Easier Coding"
.P 0
.mc |
You can specify an interface faster using UIL because you do not have to
.mc
know the specific widget creation functions
or the format of their arguments lists.  You
need to include only those object arguments you want to change.  In general,
you can specify these arguments in any order.
.P 0
Because UIL is a specification language that describes the characteristics of an
interface, it has no need for control flow.  Therefore, you can define
objects in your UIL specification in roughly the same order that the objects
are arranged in the widget tree for your application interface.  This
makes it easier for a programmer reading the UIL specification to interpret the
design of the interface. 
.P 0
At run time, when the interface objects are created, MRM performs some Motif
Toolkit
function calls for you as a convenience, simplifying your
programming tasks.
.H 3 "Earlier Error Detection"
.P 0
The UIL compiler does type checking for you that is not available with the 
Motif or the X Toolkits, so that the interface you specify has fewer errors. 
.P 0
The UIL compiler issues diagnostics if you specify any of the following:
.BL
.LI 
The wrong type of value for an argument
.LI 
An argument to an object that is not supported by that object
.LI 
A reason for an object that the object does not support
.LI 
A child of an object that the object does not support
.LE
.H 3 "Separation of Form and Function"
.P 0     
When you use UIL, you define your application interface in a separate UIL
module rather than by directly calling Motif Toolkit creation functions
in your application program. This lets you separate the form your interface
takes from the functions
provided by the application. By separating form and
function, you can design multiple interfaces that share a common
.mc |
set of functions.  This is useful, for example, in building an
.mc
international application for people who speak different languages.
.P 0
In general, you can freely change the appearance of the interface (for example,
by repositioning widgets or changing their borders or colors) without
recompiling the application program.
.H 3 "Faster Prototype Development"
.P 0
UIL helps you develop prototypes of user interfaces for an application.
You can create a variety of interfaces in a fairly short time, and
get an idea of the look of each interface before the functional routines
are written.
.P 0
The ability to specify the user interface separately lets designers work
with end users at the same time programmers are coding the functions
of the application.  Because both groups can work more or less independently,
the complete application can be delivered in less time than if the interface
design were part of the application code.
.H 3 "Interface Customization"
.P 0
.mc |
You can customize an interface by putting in place a hierarchy of UID
files, called a UID hierarchy. At run time, MRM searches this file hierarchy in
.mc
the sequence you specify to build the appropriate argument lists for the
widget creation functions.  
.P 0
One use of this feature would be to provide an interface in several
languages.  The text on title bars, menus, and so on, can be displayed in the
language of the end user without altering anything in the application.  In this
case, the files in the UID hierarchy represent alternative interfaces. 
.P 0
Another use of the UID hierarchy feature would be to isolate individual,
department, and division customizations to an interface.  In this case, you can
think of the files in the UID hierarchy as superimposed, with the definitions
in the first file listed in the array supplied to the MRM function 
.mc |
\fBMrmOpenHierarchy\fR
.mc
taking precedence.
.P 0
.H 2 "Features of UIL"
.P 0
UIL offers the following features to increase productivity and the flexibility
of your programs:
.BL
.LI
.mc |
Named values
.mc
.LI
.mc |
Compile-time expressions
.mc
.LI
Identifiers
.LI
Lists
.LI
.mc |
Support for compound strings
.mc
.LI
.mc |
Include files for useful constants
.mc
.LE
.H 3 "Named Values"
.P 0
Instead of directly specifying the values for widget and gadget attributes, you
can use named values, which are similar to variables in a programming language.
You give a literal value (such as an integer or string) a name and then use the
name in place of the value specification. Using named values makes your UIL
specification easier to understand and isolates changes.
.P 0
In addition, you can use MRM functions to fetch named values from the UID file
for use at run time.
.H 3 "Compile-Time Expressions"
.P 0
You can use expressions to specify values in UIL. A valid UIL expression can
contain integers, strings, floating-point numbers, Boolean values, named
values, and operators.  Using expressions can make values more descriptive (for
.mc |
example, \fIbulletin_board_width\fR/2) and can help you avoid 
.mc
recomputing values
(for example, if you needed to change the size or position of the
bulletin board).
.H 3 "Identifiers"
.P 0
Identifiers provide a mechanism for referencing values in UIL that are provided
by the application at run time.
In the application program,
you use an MRM function to associate a value with the identifier name. 
Unlike a named value, an identifier does not have an associated data type.  You
can use an identifier as an attribute value or callback procedure tag,
regardless of the data type specified in the object or procedure declaration.
Identifiers are useful for specifying position based on the type of terminal
on which the interface will be displayed or for passing a data structure (as
opposed to a constant) to a private callback function.
.H 3 "Lists"
.P 0
UIL allows you to create named lists of attributes, sibling widgets, and
callback procedures that you can later refer to by name.  This feature allows
you to easily reuse common definitions by simply referencing these definitions
by name.
.H 3 "Support for Compound Strings"
.P 0
Most Motif Toolkit widgets require strings used in the user interface (labels,
menu items, and so on) to be compound strings.  UIL fully supports the use of
compound strings, including left-to-right and right-to-left writing direction
and choice of fonts.
.mc *
.mc
