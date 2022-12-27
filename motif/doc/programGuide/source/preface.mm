.mc |
.\"  (c) Copyright 1989, 1990 OPEN SOFTWARE FOUNDATION, INC.
.mc
.\"  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
.\"  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
.\"  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
.\"  (c) Copyright 1988 MICROSOFT CORPORATION
.\"  ALL RIGHTS RESERVED
.mc |
...\".pn 21
...\".PH ""
...\".OH "'Revision 1.0''Programmer\'s Guide'"
...\".EH "'Programmer\'s Guide''Revision 1.0'"
...\".OP
...\".PF "''- \\\\nP -''"
...\".nr !- 21
...\".af P i
...\"
...\" book stuff:
...\".OF "'''\fH\\\\nP\fR'"
...\".EF "'\fH\\\\nP\fR'''"
...\".ds !! Preface
...\".ds !@ OSF/Motif Programmer's Guide
...\"
.ds bT "OSF/Motif Programmer's Guide
.mc
.nr Hu 1
.mc |
.nr `P 15
.mc
.HU "Preface"
.mc |
...\" book stuff:
...\".nr !_ 0
...\".nr P 15
.mc
.P
This manual is a guide to programming using the various components
.mc |
of the OSF/Motif\|\*(Tm environment; the toolkit, window manager, and 
.mc
user interface language.
.nr Hu 2
.HU "Audience"
.mc |
This document is written for programmers who want to create 
applications in the OSF/Motif environment.
.br
.ne 3i
.mc
.nr Hu 2
.HU "Contents"
.mc |
This document contains 23 chapters and 4 appendixes.
Chapters 1 through 9 discuss the Motif\*(Tm Toolkit.
Chapters 10 through 16 discuss the Motif Window Manager (MWM).
Chapters 17 through 23 discuss the User Interface Language (UIL) and
Motif Resource Manager (MRM).
.VL 1i
.LI "\fIChapter\ 1\fP"
.mc
provides introductory information on the Motif widget system.
.mc |
.LI "\fIChapter\ 2\fP"
.mc
provides a brief description of each widget, gadget, and convenience
function in the Motif system.
.mc |
.LI "\fIChapter\ 3\fP"
.mc
describes how to program using the Motif widget set.  Two sample 
programs are presented.
.mc |
.LI "\fIChapter\ 4\fP"
.mc
describes the various Shell widgets and how they are used.
.mc |
.LI "\fIChapter\ 5\fP"
.mc
describes Dialog Widgets and their associated convenience functions.
.mc |
.LI "\fIChapter\ 6\fP"
.mc
describes each of the various types of menus and presents a sample
program for each menu type.
.mc |
.LI "\fIChapter\ 7\fP"
.mc
describes in detail three widgets that are considered specialized:
List, RowColumn, and Text.
.mc |
.LI "\fIChapter\ 8\fP"
.mc
describes a number of additional features of the Motif system.
.mc |
.LI "\fIChapter\ 9\fP"
.mc
describes how to use the keyboard for traversal between widgets.
.mc |
.LI "\fIChapter\ 10\fP"
introduces the Motif Window Manager (MWM)
.LI "\fIChapter\ 11\fP"
explains the general principles and terminology of OSF/Motif window
management.
.LI "\fIChapter\ 12\fP"
.mc
provides the technical details programmers need to set up communications
between MWM and client applications.
.mc |
.LI "\fIChapter\ 13\fP"
.mc
discusses how the end user can set MWM resources to configure the window
environment.
.mc |
.LI "\fIChapter\ 14\fP"
.mc
discusses how the end user can use MWM functions to create or modify menus,
and to set 
mouse button bindings and keyboard bindings.
.mc |
.LI "\fIChapter\ 15\fP"
.mc
discusses how the end user can set MWM resources to configure icons and the
icon box. 
.mc |
.LI "\fIChapter\ 16\fP"
.mc
provides reference tables for quick access to MWM resources and functions.
.mc |
.LI "\fIChapter\ 17\fP"
is an introduction to the User Interface Language (UIL) and 
Motif Resource Manager (MRM). 
.LI "\fIChapter\ 18\fP"
describes the syntax of low-level elements of
.mc
UIL, including names, keywords, literals, functions, supported character
sets for string literals, and compile-time value expressions.
.mc |
.LI "\fIChapter\ 19\fP"
describes the syntax of UIL module
.mc
components (module header, value, procedure, identifier, and object
declarations; arguments, controls, callbacks, and procedures lists) 
and describes
additional features of UIL, including symbolic
references to widget IDs and UIL identifiers.
.mc |
.LI "\fIChapter\ 20\fP"
explains how to use the UIL compiler,
including command-line qualifiers, and how to interpret compiler
.mc
diagnostics.
.mc |
.LI "\fIChapter\ 21\fP"
provides reference information on the MRM functions.
.LI "\fIChapter\ 22\fP"
explains how to specify a user
.mc
interface for a Motif application, and describes how to access the compiled
interface specification at run time using MRM functions.
.mc |
.LI "\fIChapter\ 23\fP"
describes the Widget Meta-Language facility, that can be used to add
new widgets to the UIL compiler.
.LI "\fIAppendix\ A\fP"
lists constraint arguments that can be specified for children of
some widgets.
.LI "\fIAppendix\ B\fP"
lists the UIL built-in tables.  For each object
in the Motif Toolkit, this appendix lists the callback reasons and children
supported by the UIL compiler.
.LI "\fIAppendix\ C\fP"
provides an alphabetical listing of
.mc
the UIL arguments, their data type, and their equivalent name in the Motif 
Toolkit.
.mc |
.LI "\fIAppendix\ D\fP"
lists diagnostic messages issued by the UIL compiler and
.mc
provides corrective action.
.mc *
.mc
.LE
.nr Hu 2
.HU "Typographical Conventions"
.P
This volume uses the following typographical conventions: 
.BL
.LI
\fBBoldfaced\fP strings represent literals; type them exactly
as they appear.
.LI
\fIItalicized\fP strings represent variables (for example,
function or macro arguments).
.LI
Ellipses (...) indicate that additional arguments are optional. 
.LE
