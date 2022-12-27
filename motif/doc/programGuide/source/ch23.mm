.mc *
.mc
.\"  (c) Copyright 1989, OPEN SOFTWARE FOUNDATION, INC.
.\"  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
.\"  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
.\"  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
.\"  (c) Copyright 1988 MICROSOFT CORPORATION
.\"  ALL RIGHTS RESERVED
.mc |
.nr H1 22
.H 1 "The Widget Meta-Language Facility"
.mc
The widget meta-language facility (WML) is used to generate the
components of the user interface language (UIL) compiler that 
can change depending on the widget set.  Using WML you can add support
in UIL for
.mc |
new widgets to the OSF/Motif widget set or for a totally new widget set.
.mc
.P
UIL is made up of:
.BL
.LI
.mc |
Static syntax
.mc
.LI
.mc |
Dynamic syntax
.mc
.LI
.mc |
Data types
.mc
.LE
.P
The static syntax elements are the basic syntax and keywords
of UIL.  The static elements do not change as you modify the
widget set.  The static syntax elements of UIL are defined in
.mc |
the file \fBUil.y\fP.
.mc
.P
The dynamic syntax elements are the parts of UIL
that change with the widget set.  The dynamic syntax elements
describe the widget and gadget classes supported by UIL
including their resources and hierarchy.  The dynamic elements
of UIL are defined in WML files.  The dynamic
elements of the OSF/Motif widget set are defined in the file \fBmotif.wml\fP.
.P
The data type elements describe the allowable data types for each
widget and gadget resource.  Although the data types do not change,
the resources that they are assigned to change with the widget set.
The data types are provided in UIL for better error checking.
The allowable data types for each resource are defined in
.mc |
the same file as the dynamic syntax elements.
.mc
.P
The WML facility combines the static syntax, dynamic syntax, and data
type elements to produce new source code for UIL.  This allows you to
modify the dynamic elements of your version of UIL; giving you the
ability to add resources, widgets, gadgets, or even new widget sets.
.P
Once you have modified your WML file, run the WML facility
with that file as input and compile the new UIL compiler.  A number of
useful reports are also created by the WML facility to help you analyze,
debug, and document your changes.
.mc |
.H 2 "Using WML"
.mc
.P
.mc |
Every time Motif is built,
.mc
UIL is built from the \fBmotif.wml\fP file using the WML facility.
You can create your own WML file
in the directory \fBtools/wml\fP to use in place of \fBmotif.wml\fP.
.mc |
By convention, WML files have a suffix of \fB.wml\fP.  Section 2
.mc
describes how to modify WML files.  After you have created your
new WML file, building a new UIL is a four step process:
.AL
.LI
Build WML.
.LI
Run WML with your WML file.
.LI
Install the UIL source files.
.LI
Build UIL.
.LE
.P
All four steps are done as needed each time Motif is built.  You
should follow your standard Motif build instructions to rebuild
UIL.  In most cases you will simply move to the top of your build
tree and enter \fBmake\fP.  By default, UIL is built using the
\fBmotif.wml\fP file from the \fBtools/wml\fP directory.  You can
specify a different WML file in the \fBtools/wml\fP directory
with the command line make variable \fBTABLE\fP as follows:
.DS
\f(CW
make TABLE=\fIanyfile.wml\fP
\fR
.DE
.P
Where \fIanyfile.wml\fP is the name of a WML file in the tools/wml
directory of your Motif build tree.
.P
You should refer to the \fIOSF/Motif Release Notes\fP for more
information about building Motif.
.P
The following sections describe how to do each of the four
steps independently.
.mc |
.H 3 "Building WML"
.mc
.P
WML is built by default when you build Motif.
You should only need to build WML if you want to use it without
building Motif.
The WML source is located in the subdirectory \fBtools/wml\fP.
Before WML is built, the directory should contain the files:
.P
.TS
center, tab(@);
l l l.
.mc |
Imakefile@wml.c@wmlparse.y
Makefile@wml.h@wmlresolve.c
Makefile.hp@wmllex.l@wmlsynbld.c
README@wmlouth.c@wmluiltok.l
Uil.y@wmloutkey.c@wmlutils.c
UilDBDef.h@wmloutmm.c
motif.wml@wmloutp1.c
.mc
.TE
.P
The files
.B Imakefile ,
.B Makefile ,
and
.B Makefile.hp
are used to build and run the WML facility.
The
.B README
file contains this technical bulletin.
The files
.mc |
.B Uil.y
.mc
and
.B motif.wml
are the data files for the static syntax, dynamic syntax and data type
elements of UIL.
The files with the
.B wml
prefix are the source files for the WML facility.
.P
To build WML, change to the directory \fBtools/wml\fP, build the
make file for your machine, and build WML using the following
commands:
.DS
\f(CW
cd tools/wml
make Makefile
make depend
make wmltools
\fP
.DE
.br
.ne 13
.P
The make file is built from the Imake facility using the \fBmake
Makefile\fP and \fBmake depend\fP commands.  The \fBmake Makefile\fP
command produces a machine dependent \fBMakefile\fP for your machine.
The \fBmake depend\fP command adds include file dependencies to the
new make file.
.P
After you have built the WML facility the \fBtools/wml\fP directory should
contain the following additional files:
.P
.TS
center, tab(@);
l l l.
.mc |
lex.yy.c@wmloutkey.o@wmlresolve.o
libwml.a@wmloutmm.o@wmlsynbld.o
wml@wmloutp1.o@wmluiltok
wml.o@wmlparse.c@wmlutils.o
wmllex.c@wmlparse.h
wmlouth.o@wmlparse.o
.mc
.TE
.mc |
.H 3 "Running WML"
.mc
.P
.mc |
You need to run WML separately only if you do not want to install
.mc
and build the new version of UIL.  Running WML automatically
builds the WML source files if necessary.
.P
The \fBmake runwml\fP command from the
.B tools/wml
directory runs the WML facility.
You can specify the WML file to use with the make variable
.B TABLE .
.DS
\f(CW
make runwml TABLE=\fIanyfile.wml\fP
\fR
.DE
.P
Where \fIanyfile.wml\fP is a WML file in the \fBtools/wml\fP
directory.  If you do not set the
.B TABLE
make variable, the
.B motif.wml
file is used by default.
.P
Running WML produces the following files:
.VL 15n
.LI "UIL source files"
.B "make copy"
copies these files to the
.B clients/uil
directory to be used in building UIL.
The UIL source files are
.mc *
.mc
.B UilConst.h ,
.mc |
.B UilDBDef.h ,
.mc
.B UilKeyTab.h ,
.B UilLexPars.c ,
.mc |
.B UilLexPars.h ,
.mc
.B UilSymArTa.h ,
.B UilSymArTy.h ,
.mc |
.B UilSymCSet.h ,
.mc
.B UilSymCtl.h ,
.mc |
.B UilSymEnum.h ,
.mc
.B UilSymGen.h ,
.B UilSymNam.h ,
.B UilSymRArg.h ,
.B UilSymReas.h ,
.B UilTokName.h ,
and
.B UilUrmClas.h .
.LI "wml.report"
This report describes the widget set supported by
the newly created UIL sources.
You can use it to help validate your WML source file.
It is organized so you can easily compare it to
reference documentation as follows:
.BL
.LI
Class names are ordered alphabetically by name.
.LI
Resources are ordered by ancestor(top down).
.LI
Resources are listed alphabetically, with datatype
and default.
.LI
Reasons are ordered by ancestor then alphabetically.
.LI
Controls are ordered alphabetically.
.LE
.LI "wml-uil.mm"
.mc |
This file contains the
.B "Appendix B"
of this manual.  You can process
.mc
this file using tbl, troff and the mm macro package to
produce two tables for each supported widget class.  The
first table lists the controls and reasons supported by the
class.  The second table lists the resources for the class,
their types and default values.
.LE
.P
These files overwrite any existing WML output files in
the
.B tools/wml
directory.  If you do not want to lose the existing
files, save them somewhere else.
.br
.ne 10
.mc |
.H 3 "Installing UIL"
.mc
.P
.mc |
You need to install the UIL source files separately only if
.mc
you do not want to build the new version of UIL.
Installing the UIL source files automatically builds
the WML source files and runs WML if necessary.
.P
The \fBmake\fP command from the
.B tools/wml
directory installs the UIL source files in the
.B clients/uil
directory.
The
.B "make all"
and
.B "make copy"
commands are synonyms for the
.B make
command.
This overwrites the existing source files in the
.B clients/uil
directory.  If you do not want to lose your existing
source files, save them somewhere else.
.P
You can specify the WML file on the
.B make
command line using the make variable
.B TABLE.
.DS
\f(CW
make TABLE=\fIanyfile.wml\fP
\fR
.DE
.P
Where \fIanyfile.wml\fP is the name of a WML file.  If you
do not specify
.B TABLE ,
.B motif.wml
is used by default.
.mc |
.H 3 "Building UIL"
.mc
.P
You only need to build UIL separately if
you do not want the new UIL to reflect the current WML tables.
To build UIL separately, move to the
.B clients/uil
directory and enter
.B make .
.DS
\f(CW
cd clients/uil
make
\fR
.DE
.P
You should refer to the \fIOSF/Motif Release Notes\fP for more information
about building UIL.
.mc |
.H 2 "Modifying WML files"
.mc
.P
WML files are ASCII files that you can modify with any standard
text editor.  They are accessed in the
.B tools/wml
directory by WML.  By convention WML files have the suffix
.B .wml .
The Motif widget set is described in the
.B motif.wml
file.
This is also the default WML file when using the WML facility.
.P
When adding new widgets or changing widget characteristics, you should
start with a copy of the
.B motif.wml
file.
If you are creating a new widget set for use with UIL, you should
start from scratch.
In either case the
.B motif.wml
file is a good example of WML syntax, and you should familiarize
yourself with it before writing your own WML file.
.mc |
.H 3 "WML Syntax"
.mc
.P
WML files have a simple syntax, similar in structure to UIL.
.mc |
It is made up of seven elements:
.mc
.BL
.LI
Comments
.LI
Data Type Definitions
.LI
.mc |
Character Set Definitions
.LI
Enumeration Set Definitions
.LI
.mc
Control List Definitions
.LI
Class Definitions
.LI
Resource Definitions
.LE
.P
You can use space, tabs, or newlines anywhere in the syntax,
as long as you do not split up keywords or strings, except that
comments end at a newline.
The order of elements is not important to the syntax.
.P
This section uses the following additional conventions
to describe the syntax of the widget meta-language:
.VL 7n
.LI "[\ \ ]"
Indicates optional elements.
.LI "\.\.\."
Indicates where an element of syntax can be repeated.
.LI "|"
Indicates a choice among multiple items.
.LE
.P
For example:
.DS
\fBExactlyTyped\fP [\fB{ONE\fP | \fBTWO\fP | \fIanything.else\fP [...]\fB}\fP];
.DE
.P
indicates that
.B "ExactlyTyped"
must be typed exactly as shown, and that any number of optional arguments of
.B ONE ,
.B TWO ,
or 
.I anything.else
must be inclosed in braces if used.
Any of the following lines would fit this sample syntax:
.DS
\f(CW
ExactlyTyped;
ExactlyTyped {ONE};
ExactlyTyped {  variable } ;
ExactlyTyped {ONE TWO variable};
\fR
.DE
.br
.ne 20
.mc |
.H 4 "Comments"
.mc
.P
You can include comments in the WML file.
Comments have the following syntax:
.DS
[\fIany.element\fP]!\fIany.comment\fP
.DE
.P
Comments begin with an exclamation point and extend to the
end of the line.  A comment can begin on a line by itself or
follow any part of another element.  A comment does not change
the meaning of any other element.
For example:
.DS
\f(CW
!This is a comment.

!This is a comment
!  that spans two lines.

DataType    !This is a comment following code.
\fR
.DE
.mc |
.H 4 "Data Type Definitions"
.mc
.P
Data type definitions register all the resource data
types used in the file.  You must register all the data types used
in your WML file.
Data type definitions have the following syntax:
.DS
\fBDataType\fP 
  \fIany.datatype\fP [\fB{ \fBInternalLiteral =\fP \fIinternal.name\fP | \fBDocName = "\fP\fIstring\fP\fB";\fP [...]\fB}\fP]\fB;\fP
  [...]
.DE
A data type definition begins with the keyword \fBDataType\fP.  Following
the
.B DataType
keyword is a list of data types that can be further modified with:
.VL 10n
.LI "\fBInternalLiteral\fP"
which forces the value of the internal symbol table literal definition
of the data type name.  This modifier is only used to get around
symbol table definitions hard coded into the UIL compiler.
It should rarely be used.
.LI "\fBDocName\fP"
which gives an arbitrary string for use in the documentation.
This string is meant to supply a different name for the data type for
use in the documentation, or a single name for the data type if the
data type has aliases.
.LE
.P
For example:
.DS
\f(CW
DataType OddNumber {DocName="\\fBOddNumber\\fP";};
         NewString;
\fR
.DE
.mc |
.H 4 "Character Set Definitions"
.mc
.P
.mc |
Character set definitions register the Motif Toolkit name and other
information for the character set names used in UIL.
Character set definitions have the following syntax:
.DS
.ta 0.25i 0.5i 0.6i 0.7i
\fBCharacterSet\fP
	\fIany.character.set\fP
		\fB{\fP		\fBXmStringCharsetName = "\fP\fIstring\fP\fB" ;\fP
			[	\fBAlias = "\fP\fIstring\fP\fB"\fP ... \fB;\fP |
				\fBDirection =\fP [ \fBLeftToRight\fP | \fBRightToLeft\fP ] \fB;\fP |
				\fBParseDirection =\fP [ \fBLeftToRight\fP | \fBRightToLeft\fP ] \fB;\fP |
				\fBCharacterSize =\fP [ \fBOneByte\fP | \fBTwoByte\fP ] \fB;\fP ]
			[ ... ] \fB} ;\fP
	[ ... ]
.DE
.P
A character set definition begins with the keyword \fBCharacterSet\fP.
Following the \fBCharacterSet\fP keyword is a list of character sets
that can be further modified with:
.VL 1.5i
.LI "\fBXmStringCharsetName\fP"
Specifies the name of the character set, which will become the character
set component of a compound string segment created using this
character set.
This modifier is required.
.LI "\fBAlias\fP"
Specifies one or more aliases for the character set name.
Each alias can be used within UIL to refer to the same character set.
.LI "\fBDirection\fP"
Specifies the direction of a compound string segment created using
this character set.
The default is \fBLeftToRight\fP.
.LI "\fBParseDirection\fP"
Specifies the direction in which an input string is parsed when a
compound string segment is created using this character set.
The default is whatever \fBDirection\fP is specified.
.LI "\fBCharacterSize\fP"
Specifies the number of bytes in each character of a compound string
segment created using this character set.
The default is \fBOneByte\fP.
.LE
.P
For example:
.DS
.ft CW
CharacterSet
  iso_latin1
    { XmStringCharsetName = "ISO8859-1";
      Alias = "ISOLatin1"; };
  iso_hebrew_lr
    { XmStringCharsetName = "ISO8859-8";
      Alias = "iso_latin8_lr";
      Direction = RightToLeft;
      ParseDirection = LeftToRight; };
  ksc_korean
    { XmStringCharsetName = "KSC5601.1987-0";
      CharacterSize = TwoByte; };
.ft R
.DE
.H 4 "Enumeration Set Definitions"
Enumeration set definitions register the named constants used in the
Motif Toolkit to specify some resource values.
Enumeration set definitions have the following syntax:
.DS
.ta 0.25i 0.5i
\fBEnumerationSet\fP
	\fIresource.name\fP \fB:\fP \fIresource.type\fP
		\fB{\fP \fIenum.value.name\fP \fB;\fP [ ... ] \fB} ;\fP
.DE
.P
An enumeration set definition begins with the keyword
\fBEnumerationSet\fP.
For each enumeration set defined, the name and type of the resource are
listed.
The resource name is the Motif Toolkit resource name, with the beginning
\fBXmN\fP removed and with the initial letter capitalized.
For example, the name of the Motif Toolkit resource
\fBXmNrowColumnType\fP is \fBRowColumnType\fP.
The resource type is the data type for the resource; for most resources,
this is \fBinteger\fP.
Following the resource name and type is a list of names of enumeration
values that can be used as settings for the resource.
These names are the same as those in the Motif Toolkit.
.P
For example:
.DS
.ft CW
EnumerationSet
  RowColumnType: integer
    { XmWORK_AREA; XmMENU_BAR; XmMENU_POPUP;
      XmMENU_PULLDOWN; XmMENU_OPTION; };
.ft R
.DE
.H 4 "Control List Definitions"
.P
.mc
Control list definitions assign a name to groups of controls.
You can use these control lists later in class definitions to simplify
the structure of your WML file.
Control list definitions have the following syntax:
.DS
\fBControlList\fP
  \fIany.control.list\fP [\fB{\fP \fIany.control\fP\fB;\fP [...]\fB}\fP]\fB;\fP
.DE
.P
A control list definition starts with the
.B ControlList
keyword.
Following the
.B ControlList
keyword are any number of control list definitions.  Control list
definitions are made up of a control list name followed by the
set of controls it represents.  For example:
.DS
\f(CW
ControlList
        Buttons {PushButton;
                 RadioButton;
                 CascadeButton;
                 NewCascadebutton;};
\fR
.DE
.P
Each control specified in the control list must be defined as
a class in the file.
.mc |
.H 4 "Class Definitions"
.mc
.P
Class definitions describe a particular widget class including
its position in the class hierarchy, toolkit convenience function,
resources, and controls.  There should be one class definition for
each widget or gadget in the widget set you want to support in UIL.
Class definitions have the following syntax:
.DS
\fBClass\fP \fIclass.name\fP \fB: MetaClass\fP | \fBWidget\fP | \fBGadget\fP
   [\fB{\fP[ \fBSuperClass =\fP \fIclass.name\fP\fB;\fP |
       \fBInternalLiteral =\fP \fIinternal.name\fP\fB;\fP |
       \fBAlias =\fP \fIalias\fP\fB;\fP |
       \fBConvenienceFunction =\fP \fIconvenience.function\fP\fB;\fP |
       \fBWidgetClass =\fP \fIwidget.class\fP\fB;\fP |
       \fBDocName = "\fP\fIstring\fP\fB";\fP |
       \fBDialogClass =\fP \fBTrue\fP | \fBFalse;\fP |
       \fBResources {\fP \fIany.resource.name\fP [\fB{Default =\fP \fInew.default.value\fP\fB;\fP |
                                            \fBExclude = True\fP | \fBFalse;\fP
                                            [...]\fB}\fP
                                         ]\fB;\fP
                    [...]\fB};\fP |
       \fBControls {\fP \fIany.control.name\fP\fB;\fP [...]}\fB;\fP
       [...]
   ]\fB}\fP]\fB;\fP
.DE
.P
Class definitions start with the
.B Class
keyword. 
For each class defined, the name of the class and whether the
class is a metaclass, widget, or gadget is listed.
Each class definition can be further modified with the following
keywords:
.VL 22n
.LI "\fBSuperClass\fP"
This indicates the name of the parent class.
Only the root of the hierarchy does not specify a SuperClass.
.LI "\fBInternalLiteral\fP"
which forces the value of the internal symbol table literal definition
of the class name.  This modifier is only used to get around
symbol table definitions hard coded into the UIL compiler.
It should rarely be used.
.LI "\fBAlias\fP"
This indicates alternate names for the class
for use in a UIL specification.
.mc *
.mc
.LI "\fBConvenienceFunction\fP"
This indicates the name of the creation convenience function
for this class.  All widget and gadget classes must have a
.B ConvenienceFunction .
.LI "\fBWidgetClass\fP"
This indicates the associated widget class of gadget type classes.
Presently, nothing is done with this value.
.LI "\fBDocName\fP"
This defines an arbitrary string for use in the documentation.
Presently, nothing is done with this value.
.LI "\fBDialogClass\fP"
This indicates whether the class is a dialog class.
Presently, nothing is done with this value.
.LI "\fBResources\fP"
This lists the resources of the widget class.  This keyword
can be further modified with:
.VL 10n
.LI "\fBDefault\fP"
This specifies a new default value for this resource.  Resource
default values are usually set in the resource definition.  If
an inherited resource's default value is changed by the class,
the new default value should be noted here.
.LI "\fBExclude\fP"
This specifies whether an inherited resource should be excluded from the
resource list of the class.  Exclude is
.B False
by default.
.LE
.LI "\fBControls\fP"
This lists the controls that the widget class allows.  The controls can
be other classes or a control list from the control list definition.
.LE
.P
The example below uses the examples from the data type definitions and
control list definitions above.
.DS
\f(CW
Class
     TopLevelWidget : MetaClass
          {
          Resources
               {
               XtbNfirstResource;
               XtbNsecondResource;
               };
          };
     NewWidget : Widget
          {
          SuperClass = TopLevelWidget;
          ConvenienceFunction = XtbCreateNewWidget;
          Resources
               {
               XtbNnewResource;
               XtbNfirstResource {Default="XtbNEW_VALUE";};
               XtbNsecondResource {Exclude=True;};
               };
          Controls
               {
               NewWidget;
               Buttons;
               };
          };
\fR
.DE
.br
.ne 20
.mc |
.H 4 "Resource Definitions"
.mc
.P
Resource definitions describe a particular resource including
its type, and default value.
There should be a resource definition for
.mc |
each new resource referenced in the class definitions.
.mc
Resource definitions have the following syntax:
.DS
\fBResource\fP
      \fIresource.name\fP \fB: Argument\fP | \fBReason\fP | \fBConstraint\fP | \fBSubResource\fP
            [\fB{\fP[ \fBType =\fP \fItype\fP \fB;\fP |
                 \fBResourceLiteral =\fP \fIresource.literal\fP \fB;\fP |
                 \fBInternalLiteral =\fP \fIinternal.name\fP\fB;\fP |
                 \fBAlias =\fP \fIalias\fP \fB;\fP |
                 \fBRelated =\fP \fIrelated\fP \fB;\fP |
                 \fBDefault =\fP \fIdefault\fP \fB;\fP |
                 \fBDocName =\fP \fIdoc.name\fP \fB;\fP ]
               [...]\fB}\fP]
      [...]
.DE
.P
Resource definitions start with the
.B Resource
keyword. 
For each resource definition,
the name of the resource and whether the resource is an argument, reason,
constraint or subresource is listed.
.VL 15n
.LI "\fBArgument\fP"
Indicates a standard resource.
.LI "\fBReason\fP"
Indicates a callback resource.
.LI "\fBConstraint\fP"
Indicates a constraint resource.
.LI "\fBSubResource\fP"
Presently, nothing is done with this value.
.LE
.P
The resource definition can be further modified with the following
keywords:
.VL 15n
.LI "\fBType\fP"
This indicates the data type of the resource.  It must be listed
in the data type definition.
.LI "\fBResourceLiteral\fP"
This indicates the keyword used in the UIL file to reference the
resource.  In Motif, the resource name is the same as the
.B ResourceLiteral .
.LI "\fBInternalLiteral\fP"
which forces the value of the internal symbol table literal definition
of the resource name.  This modifier is only used to get around
symbol table definitions hard coded into the UIL compiler.
It should rarely be used.
.LI "\fBAlias\fP"
This indicates alternate names for the resource
for use in a UIL specification.
.mc *
.mc
.LI "\fBRelated\fP"
This lists other related resources.  It is for
documentation purposes only.
Presently, nothing is done with this value.
.LI "\fBDefault\fP"
This indicates the default value of the resource.
.LI "\fBDocName\fP"
This defines an arbitrary string for use in the documentation.
Presently, nothing is done with this value.
.LE
.br
.ne 20
.P
The example below uses the examples from the data type definitions,
control list definitions and class definitions above.
.DS
\f(CW
Resource
     XtbNfirstResource : Argument
          { Type = OddNumber;
            Default = "XtbOLD_VALUE";};
     XtbNsecondResource : Argument
          { Type = NewString;
            Default = "XtbNEW_STRING"; };
     XtbNnewResource : Argument
          { Type = OddNumber;
            Default = "XtbODD_NUMBER"; };
\fR
.DE
