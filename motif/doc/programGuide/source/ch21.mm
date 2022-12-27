.mc |
.\"  (c) Copyright 1989, 1990 OPEN SOFTWARE FOUNDATION, INC.
.mc
.\"  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
.\"  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
.\"  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
.\"  (c) Copyright 1988 MICROSOFT CORPORATION
.\"  ALL RIGHTS RESERVED
.mc |
.nr H1 20
.mc
.H 1 "Motif Resource Manager Functions"
.P 0
The functions discussed in this chapter define the application
interface to Motif Resource Manager (MRM).
MRM is responsible for creating widgets based 
on definitions contained
.mc |
in the UID
files created by the UIL compiler.
.mc
MRM interprets the output of the UIL compiler
and generates the appropriate argument lists for widget creation
functions. 
Specifically, the functions discussed in this chapter allow your
.mc |
application to
.mc
.BL
.LI
Initialize MRM.
.LI
Provide information required by MRM to successfully interpret
information contained in UID files.
.LI
Create widgets using UID file definitions.
.LI
.mc |
Read literal definitions from UID files.  Tthese definitions are created
.mc
by using the exported value definitions in the UIL, and 
the resulting literals
.mc |
may be used for any purpose the application requires.
.mc
.LE
.P 0
The representation of widgets in a UID file is not exposed in these
functions.
All management and translation of these representations is done
internally.
.P 0
All definitions required to use MRM facilities are 
.mc |
contained in the include file
\fB<Mrm/MrmPublic.h>\fP.
.mc
.H 2 "Setting Up Storage and Data Structures"
.P 0
To initialize the internal data structures needed by MRM, use
.mc |
\fBMrmInitialize\fR.
.P
.nf
.mc
.iX "MRM function" "MrmInitialize"
.iX "MrmInitialize" "definition"
.mc |
\fBvoid MrmInitialize\fR\^(\^)
.fi
.ft R
.mc
.P 0
.mc |
The \fBMrmInitialize\fR function must be called to prepare an application to use MRM
.mc
widget-fetching facilities.
You must call this function prior to fetching a widget.
.mc |
However, it is good programming practice to call \fBMrmInitialize\fR
prior to performing any MRM operations.
\fBMrmInitialize\fR initializes
.mc
.iX "MrmInitialize" "description"
.mc |
the internal data structures that MRM needs to successfully perform type
conversion on arguments and to successfully access widget creation facilities.
An application must call \fBMrmInitialize\fR before it uses other MRM functions.
.mc
.H 2 "Obtaining UID Database File IDs"
.P 0
A Motif application can access different UID files
based on the language preferences of the user.  
This capability is 
.mc |
provided by MRM in a way that is consistent 
.mc
with the existing NLS
standards as specified in the \fIXOpen Portability 
Guide\fP (issue 3, draft 2).  
In particular, the capability is compatible with the searching and
naming conventions used to access message catalogs.
To specify the UID files to be opened in Motif
.mc |
applications, use \fBMrmOpenHierarchy\fR:
.P
.nf
\fB#include\fR <\fBMrm/MrmPublic.h\fR>
.mc
.iX "MRM function" "MrmOpenHierarchy"
.iX "MrmOpenHierarchy" "definition"
.mc |
\fBCardinal MrmOpenHierarchy\fR\^(\^\fInum_files\fP, \fIfile_names_list\fP,
                  \fIancillary_structures_list\fP, \fIhierarchy_id\fP\^)
     \fBMrmCount\fR \fInum_files\fP\^;
     \fBString\fR \fIfile_names_list\fP []\^;
     \fBMrmOsOpenParamPtr\fR *\^\fIancillary_structures_list\fP\^;
     \fBMrmHierarchy\fR *\^\fIhierarchy_id\fP\^;
.fi
.ft R
.VL 1.5i
.LI "\fInum_files\fP"
.mc
Specifies the number of files in the name list.
.mc |
.LI "\fIfile_names_list\fP"
.mc
Specifies an array of pointers to character strings that identify 
.mc |
the .uid files.
.LI "\fIancillary_structures_list\fP"
.br
A list of operating system dependent ancillary structures corresponding
.mc
to such things as file names, clobber flag, and so forth.
This argument should be NULL for most operations.
If you need to reference this structure,
see the definition of
.mc |
MrmOsOpenParamPtr
.mc
in
.mc |
MrmPublic.h
.mc
for more information.
.mc |
.LI "\fIhierarchy_id\fP"
.mc
Returns the search hierarchy ID.
.mc |
The search hierarchy ID identifies the list of \fB.uid\fR files that
MRM searches (in order) when performing subsequent
.mc
fetch calls.
.mc |
.LE
.mc
.P 0
.mc |
The \fBMrmOpenHierarchy\fR function 
.mc
.iX "MrmOpenHierarchy" "description"
allows the user to specify the list of UID files that
.mc |
MRM searches in subsequent fetch operations.
All subsequent fetch operations return the first occurrence of the
.mc
named item encountered while traversing the UID hierarchy from
the first list element (UID file specification) to the last list
.mc |
element.  This function also
allocates a hierarchy ID and opens all the UID files in the hierarchy. 
.mc
It initializes the optimized search lists in the hierarchy.
.mc |
If \fBMrmOpenHierarchy\fR encounters any errors during its execution, it closes any 
files that were opened.
.P
Each UID file string in \fIfile_names_list\fP can specify either a full
pathname or a filename.
If a UID file string has a leading / (slash), it specifies a full
pathname, and MRM opens the file as specified.
Otherwise, the UID file string specifies a filename.
In this case MRM looks for the file along a search path specified by the
\fBUIDPATH\fP environment variable or by a default search path, which
varies depending on whether or not the \fBXAPPLRESDIR\fP environment
variable is set.
The filename is substituted for each occurrence of \fB%U\fR in the search
path.
.P
The \fBUIDPATH\fP environment variable specifies a search path and
naming conventions associated with UID files.
It can contain the substitution field \fB%U\fR, where the UID file string from
the \fIfile_names_list\fP argument to \fBMrmOpenHierarchy\fR is
substituted for \fB%U\fR.
It can also contain the substitution fields accepted by
\fBXtResolvePathname\fP.
For example, the following \fBUIDPATH\fP value and 
\fBMrmOpenHierarchy\fR
call cause MRM to open two separate UID files:
.P
.nf
.ft CW
.S -2
UIDPATH=/uidlib/%L/%U.uid:/uidlib/%U/%L
.mc
  static char *uid_files[] = {"/usr/users/me/test.uid", "test2"};
   MrmHierarchy  *Hierarchy_id;
   MrmOpenHierarchy((MrmCount)2,uid_files, NULL, Hierarchy_id)
.mc |
.S
.ft R
.fi
.P
MRM opens the first file, \fB/usr/users/me/test.uid\fP, as specified in
the \fIfile_names_list\fP argument to \fBMrmOpenHierarchy\fR because
the UID file string in the \fIfile_names_list\fP argument specifies a
full pathname.
MRM looks for the second file, \fBtest2\fP,
first as
\fB/uidlib/%L/test2.uid\fP
and second as
\fB/uidlib/test2/%L\fP,
where the current setting of the \fBxnlLanguage\fP resource or the
\fBLANG\fR environment variable is substituted for \fB%L\fR.
.P
If \fBUIDPATH\fP is not set but the environment variable
\fBXAPPLRESDIR\fP is set, MRM searches the following pathnames:
.P
\fB
%U
.br
$XAPPLRESDIR/%L/uid/%N/%U
.br
$XAPPLRESDIR/%l/uid/%N/%U
.br
$XAPPLRESDIR/uid/%N/%U
.br
$XAPPLRESDIR/%L/uid/%U
.br
$XAPPLRESDIR/%l/uid/%U
.br
$XAPPLRESDIR/uid/%U
.br
$HOME/uid/%U
.br
$HOME/%U
.br
/usr/lib/X11/%L/uid/%N/%U
.br
/usr/lib/X11/%l/uid/%N/%U
.br
/usr/lib/X11/uid/%N/%U
.br
/usr/lib/X11/%L/uid/%U
.br
/usr/lib/X11/%l/uid/%U
.br
/usr/lib/X11/uid/%U
.br
/usr/include/X11/uid/%U
.fi
\fP
.P
If neither \fBUIDPATH\fP nor \fBXAPPLRESDIR\fP is set, MRM searches the
following pathnames:
.P
\fB
%U
.br
$HOME/%L/uid/%N/%U
.br
$HOME/%l/uid/%N/%U
.br
$HOME/uid/%N/%U
.br
$HOME/%L/uid/%U
.br
$HOME/%l/uid/%U
.br
$HOME/uid/%U
.br
$HOME/%U
.br
/usr/lib/X11/%L/uid/%N/%U
.br
/usr/lib/X11/%l/uid/%N/%U
.br
/usr/lib/X11/uid/%N/%U
.br
/usr/lib/X11/%L/uid/%U
.br
/usr/lib/X11/%l/uid/%U
.br
/usr/lib/X11/uid/%U
.br
/usr/include/X11/uid/%U
.fi
\fP
.P
The following substitutions are used in these paths:
.VL 0.5i
.LI "\fB%U\fP"
The UID file string, from the \fIfile_names_list\fP argument
.LI "\fB%N\fP"
The class name of the application
.LI "\fB%L\fP"
The value of the \fBxnlLanguage\fP resource or the \fBLANG\fR
environment variable
.LI "\fB%l\fP"
The language component of the \fBxnlLanguage\fP resource or the
\fBLANG\fR environment variable
.LE
.mc
.P 0
.mc |
.ne 3i
After \fBMrmOpenHierarchy\fR
.mc
opens the UID hierarchy, you should not delete or modify the
.mc |
UID files until you close the UID hierarchy by calling \fBMrmCloseHierarchy\fR.
.mc
.P 0
.mc |
The \fBMrmOpenHierarchy\fR function returns one of these status return constants: 
.mc
.iX "MrmSUCCESS"
.iX "MrmNOT_FOUND"
.iX "MrmFAILURE"
.mc |
.P
.mc
.TS
tab(@);
l l
lw(1.25i) lw(3i).
.mc
.sp 6p
.mc |
.R
MrmSUCCESS@The function executed successfully.
.mc
.br
.mc |
MrmNOT_FOUND@File not found.
.mc
.br
.mc |
MrmFAILURE@The function failed.
.mc
.TE
.H 2 "Closing a MRM Search Hierarchy"
.P 0
.mc |
To close an MRM search hierarchy, use \fBMrmCloseHierarchy\fR:
.P
.nf
\fB#include\fR <\fBMrm/MrmPublic.h\fR>
.mc
.iX "MRM function" "MrmCloseHierarchy"
.iX "MrmCloseHierarchy" "definition"
.mc |
\fBCardinal MrmCloseHierarchy\fR\^(\^\fIhierarchy_id\fP\^)
     \fBMrmHierarchy\fR \fIhierarchy_id\fP\^;
.fi
.ft R
.sp .25
.VL 2i
.LI "\fIhierarchy_id\fP"
.mc
Specifies the ID of a previously opened UID hierarchy.
The
\fIhierarchy_id\fP
was returned in a previous call to
.mc |
\fBMrmOpenHierarchy\fR.
.LE
.mc
.P 0
The
.mc |
\fBMrmCloseHierarchy\fR
.mc
function
.iX "MrmCloseHierarchy" "description"
closes a UID hierarchy previously opened by
.mc |
\fBMrmOpenHierarchy\fR.
.mc
All files associated with the hierarchy are closed by
MRM and all associated memory is returned.
.iX "MrmSUCCESS"
.iX "MrmFAILURE"
.mc |
The \fBMrmCloseHierarchy\fR function returns one of these status return constants: 
.mc
.TS
tab(@);
l l
lw(1.25i) lw(3i).
.mc
.sp 6p
.mc |
.R
.mc
.br
.mc |
MrmSUCCESS@The function executed successfully.
.mc
.br
.mc |
MrmFAILURE@The function failed.
.mc
.TE
.H 2 "Registering MRM Information and Callbacks"
.P 0
.mc |
This section discusses the MRM functions you can use to
.mc
.BL
.LI
Save the information needed to access the widget creation function
.LI
Register a vector of callback functions
.LE
.H 3 "Registering MRM Information"
.P 0
To save the information needed to access the widget creation function,
use
.mc |
\fBMrmRegisterClass\fR:
.P
.nf
\fB#include\fR <\fBMrm/MrmPublic.h\fR>
.mc
.iX "MRM function" "MrmRegisterClass"
.iX "MrmRegisterClass" "definition"
.mc |
\fBCardinal MrmRegisterClass\fR\^(\^\fIclass_code\fP, \fIclass_name\fP, \fIcreate_name\fP,
                           \fIcreate_proc\fP, \fIclass_record\fP\^)
     \fBMrmType\fR \fIclass_code\fP\^;
     \fBString\fR \fIclass_name\fP\^;
     \fBString\fR \fIcreate_name\fP\^;
     \fBWidget\fR (* \fIcreate_proc\fP) ();
     \fBWidgetClass\fR \fIclass_record\fP\^;
.fi
.ft R
.sp .25
.VL 1.75i
.LI "\fIclass_code\fP"
.mc
Specifies the code name of the class.
.mc |
For all application-defined widgets, this code name is \fBMRMwcUnknown\fR.
For all Motif Toolkit widgets, each code name begins with the letters \fBMRMwc\fR.
The code names for all application widgets are defined in \fBMrm.h\fR.
.LI "\fIclass_name\fP"
.mc
Specifies the case-sensitive name of the class.
.mc |
The class names for all Motif Toolkit widgets are defined in \fBMrm.h\fR. 
Each class name begins with the letters \fBMRMwcn\fR.
.LI "\fIcreate_name\fP"
.mc
Specifies the case-sensitive name of the low-level widget creation
.mc |
function for the class.  An example from the Motif Toolkit is \fBXmCreateLabel\fR.
Arguments are \fIparent_widget\fP, \fIname\fP, \fIoverride_arglist\fP, and
.mc
\fIoverride_argcount\fP.
.mc |
.LE
.mc
.P 0
.mc |
For user-defined widgets, \fIcreate_name\fP
.mc
is the creation procedure in the UIL that defines this widget.
.mc |
.VL 1.75i
.LI "\fIcreate_proc\fP"
.mc
Specifies the address of the creation function that you named in
\fIcreate_name\fP. 
.mc |
.LI "\fIclass_record\fP"
.mc
Specifies a pointer to the class record.
.mc |
.LE
.mc
.P 0
.mc |
The \fBMrmRegisterClass\fR function 
.mc
.iX "MrmRegisterClass" "description"
allows MRM to access user-defined widget classes.
This function registers the necessary information for
.mc |
MRM to create widgets of this class.  You must call \fBMrmRegisterClass\fR
.mc
prior to fetching any user-defined class widget.
.P 0
.mc |
\fBMrmRegisterClass\fR
.mc
saves the information needed to access the widget creation
function and to do type conversion of argument lists by using the 
information in MRM databases.
.P 0
.iX "MrmSUCCESS"
.iX "MrmFAILURE"
This function returns one of these status return constants: 
.TS
tab(@);
l l
lw(1.25i) lw(3i).
.mc
.sp 6p
.br
.mc |
.R
MrmSUCCESS@The function executed successfully.
.mc
.br
.mc |
MrmFAILURE@The allocation of the class descriptor failed.
.mc
.TE
.mc |
.H 3 "Registering a Vector of Callback Functions"
.mc
.P 0
To register a vector of names of identifiers or callback functions for
access in MRM, use
.mc |
\fBMrmRegisterNames\fR or
\fBMrmRegisterNamesInHierarchy\fR:
.P
.nf
\fB#include\fR <\fBMrm/MrmPublic.h\fR>
.mc
.iX "MRM function" "MrmRegisterNames"
.iX "MrmRegisterNames" "definition"
.mc |
\fBCardinal MrmRegisterNames\fR\^(\^\fIregister_list\fP, \fIregister_count\fP\^)
     \fBMrmRegisterArglist\fR \fIregister_list\fP\^;
     \fBMrmCount\fR \fIregister_count\fP\^;
.fi
.ft R
.sp .25
.VL 2i
.LI "\fIregister_list\fP"
.mc
Specifies a list of name/value pairs for the names to be registered.
Each name is a case-sensitive, NULL-terminated ASCII string.
Each value is a 32-bit quantity, interpreted as a procedure address if
the name is a callback function, and uninterpreted otherwise.
.mc |
.LI "\fIregister_count\fP"
.mc
Specifies the number of entries in
\fIregister_list\fP.
.mc |
.LE
.mc
.P 0
The
.mc |
\fBMrmRegisterNames\fR
.mc
function
.iX "MrmRegisterNames" "description"
registers a vector of names and associated values
for access in MRM. 
The values can be callback functions, pointers
to user-defined data, or any other values. 
The information provided is used to resolve symbolic references
occurring in UID files to their run-time values.
For callbacks, this information provides the procedure address required
by the Motif Toolkit.
For names used as identifiers in UIL, this information 
provides any
run-time mapping the application needs.
.P 0
The names in the list are case-sensitive.
The list can be either ordered or unordered.
.P 0
Callback functions registered through 
.mc |
\fBMrmRegisterNames\fR
.mc
can be either
regular or creation callbacks.
Regular callbacks have declarations determined by Motif Toolkit and user
requirements.
Creation callbacks have the same format as any other callback:
.mc |
.P
.nf
\fBvoid CallBackProc\fR\^(\^\fIwidget_id\fP, \fItag\fP, \fIcallback_data\fP\^)
	\fBWidget\fR *\^\fIwidget_id\fP\^;
	\fBOpaque\fR \fItag\fP;
	\fBXmAnyCallbackStruct\fR *\^\fIcallback_data\fP\^;
.fi
.ft R
.VL 1.75i
.LI "\fIwidget_id\fP"
.mc
Specifies the widget ID associated with the widget performing the
callback (as in any callback function).
.mc |
.LI "\fItag\fP"
.mc
Specifies the tag value (as in any callback function).
.mc |
.LI "\fIcallback_data\fP"
.mc
Specifies a widget-specific data structure.
.mc |
This data structure has a minimum of two members, event and reason.
.mc
The reason member is always set to
.mc |
.BR MrmCR_CREATE .
.LE
.mc
.P 0
Note that the widget name and parent are available from the widget
.mc |
record accessible through \fIwidget_id\fP.
.mc
.P 0
.mc |
.ne 2i
.mc
.iX "MrmSUCCESS"
.iX "MrmFAILURE"
This function returns one of these status return constants: 
.TS
tab(@);
l l
lw(1.25i) lw(3i).
.mc
.sp 6p
.br
.mc |
.R
MrmSUCCESS@The function executed successfully.
.mc
.br
.mc |
MrmFAILURE@Memory allocation failed.
.mc
.TE
.mc |
.P
.nf
\fB#include\fR <\fBMrm/MrmPublic.h\fR>
.iX "MRM function" "MrmRegisterNamesInHierarchy"
.iX "MrmRegisterNamesInHierarchy" "definition"
\fBCardinal MrmRegisterNamesInHierarchy\fR\^(\^\fIhierarchy_id\fP, \fIregister_list\fP, \fIregister_count\fP\^)
     \fBMrmHierarchy\fR \fIhierarchy_id\fP\^;
     \fBMrmRegisterArglist\fR \fIregister_list\fP\^;
     \fBMrmCount\fR \fIregister_count\fP\^;
.fi
.ft R
.sp .25
.VL 2i
.LI "\fIhierarchy_id\fP"
Specifies the hierarchy with which the names are to be associated.
.LI "\fIregister_list\fP"
Specifies a list of name/value pairs for the names to be registered.
Each name is a case-sensitive, NULL-terminated ASCII string.
Each value is a 32-bit quantity, interpreted as a procedure address if
the name is a callback function, and uninterpreted otherwise.
.LI "\fIregister_count\fP"
Specifies the number of entries in
\fIregister_list\fP.
.mc
.LE
.mc *
.mc
.P 0
.mc *
.mc
The
.mc |
\fBMrmRegisterNamesInHierarchy\fR
.mc
function
.mc |
.iX "MrmRegisterNamesInHierarchy" "description"
registers a vector of names and associated values
for access in MRM. 
The values can be callback functions, pointers
to user-defined data, or any other values. 
The information provided is used to resolve symbolic references
occurring in UID files to their run-time values.
For callbacks, this information provides the procedure address required
by the Motif Toolkit.
For names used as identifiers in UIL, this information 
provides any
run-time mapping the application needs.
.P
This function is similar to
\fBMrmRegisterNames,\fR
except that the scope of the names registered by
\fBMrmRegisterNamesInHierarchy\fR
is limited to the hierarchy specified by
\fIhierarchy_id\fP,
whereas the names registered by
\fBMrmRegisterNames\fR
have global scope.
When MRM looks up a name, it first tries to find the name among those
registered for the given hierarchy.
If that lookup fails, it tries to find the name among those registered
globally.
.mc
.P 0
.mc |
.ne 2i
.mc
.iX "MrmSUCCESS"
.iX "MrmFAILURE"
.mc *
.mc
This function returns one of these status return constants: 
.TS
tab(@);
l l
lw(1.25i) lw(3i).
.mc
.sp 6p
.br
.mc |
.R
MrmSUCCESS@The function executed successfully.
.mc
.br
.mc |
MrmFAILURE@Memory allocation failed.
.mc
.TE
.mc |
.H 2 "Fetching Widgets"
.P 0
This section discusses the MRM functions you can use to
.BL
.LI
Fetch all the widgets defined in some interface
.LI
Fetch values stored in UID files
.LI
Fetch any indexed application widget
.LE
.mc
.H 3 "Fetching Values Stored in UID Files"
.P 0
To fetch the values to be set from literals stored in UID files, use
.mc |
\fBMrmFetchSetValues\fR:
.P
.nf
\fB#include\fR <\fBMrm/MrmPublic.h\fR>
.mc
.iX "MRM function" "MrmFetchSetValues"
.iX "MrmFetchSetValues" "definition"
.mc |
\fBCardinal MrmFetchSetValues\fR\^(\^\fIhierarchy_id\fP, \fIwidget\fP, \fIargs\fP, \fInum_args\fP\^)
     \fBMrmHierarchy\fR \fIhierarchy_id\fP\^;
     \fBWidget\fR \fIwidget\fP\^;
     \fBArgList\fR \fIargs\fP\^;
     \fBCardinal\fR \fInum_args\fP\^;
.fi
.ft R
.VL 1.75i
.LI "\fIhierarchy_id\fP"
.mc
Specifies the ID of the UID hierarchy that contains the
specified literal. 
The
\fIhierarchy_id\fP
was returned in a previous call to
.mc |
\fBMrmOpenHierarchy\fR.
.LI "\fIwidget\fP"
.mc
Specifies the widget that is modified.
.mc |
.LI "\fIargs\fP"
.mc
Specifies an argument list that identifies the widget arguments to be
modified as well as the index (UIL name) of the literal that
defines the value for that argument.
.mc |
The name part of each argument (\fIargs\fR[\fIn\fR]\fI.name\fR) must begin with the string 
\fBXmN\fR
.mc
followed
by the name that uniquely identifies this attribute tag.
For example,
.mc |
\fBXmNwidth\fR
.mc
is the attribute name associated with the core argument
\fIwidth\fP.
.mc |
The value part (\fIargs\fR[\fIn\fR]\fI.value\fR) must be a string
.mc
that gives the index (UIL name) of
the literal.
You must define all literals in UIL as exported values.
.mc |
.LI "\fInum_args\fP"
.mc
Specifies the number of entries in
\fIargs\fP.
.mc |
.LE
.mc
.P 0
The
.mc |
\fBMrmFetchSetValues\fR
.mc
function
is similar to
.mc |
\fBXtSetValues\fR,
.mc
except that the values to be set are defined by the UIL named
values that are stored in the UID hierarchy.
.iX "MrmFetchSetValues" "description"
.mc |
\fBMrmFetchSetValues\fR
.mc
fetches the values to be set from literals stored in UID files.
.P 0
This function
sets the values
on a widget, evaluating the
.mc |
values as literal references that can be resolved from
.mc
a UID hierarchy. 
Each literal is fetched from the hierarchy, and
its value is modified and converted as required. 
This value is
then placed in the argument list and used as the actual value for an
.mc |
\fBXtSetValues\fR
.mc
call. 
.mc |
\fBMrmFetchSetValues\fR
.mc
allows a widget to be modified
after creation using UID file values exactly as is done for
creation
values in 
.mc |
\fBMrmFetchWidget\fR.
.mc
.P 0
As in 
.mc |
\fBMrmFetchWidget\fR,
.mc
each argument whose value can be evaluated
from
the UID hierarchy is set in the widget. 
Values that are not
found
or values in which conversion errors occur are not modified.
.P 0
Each entry in the argument list identifies an argument to be modified
in the widget. 
The name part identifies the tag, which begins with 
.mc |
\fBXmN\fR.
.mc
The value part must be a string
whose value is the index of
the literal. 
.mc |
Thus, the following code modifies the XmLabel resource of the widget
.mc
to have the value of the literal accessed by the index
OK_button_label in the hierarchy:
.mc |
.P
.nf
.ft CW
.mc
args[n].name = XmNlabel;
args[n].value = "OK_button_label";
.mc |
.P
.fi
.ft R
.mc
.P 0
.iX "MrmSUCCESS"
.iX "MrmFAILURE"
This function returns one of these status return constants: 
.TS
tab(@);
l l
lw(1.25i) lw(3i).
.mc
.sp 6p
.mc |
.R
.mc
.br
.mc |
MrmSUCCESS@The function executed successfully.
.mc
.br
.mc |
MrmFAILURE@The function failed.
.mc
.TE
.mc |
.H 3 "Fetching Indexed Application Widgets"
.mc
.P 0
.mc |
To fetch an indexed application widget, use
\fBMrmFetchWidget\fR.
.P
.nf
\fB#include\fR <\fBMrm/MrmPublic.h\fR>
.mc
.br
.mc *
.mc
.iX "MRM function" "MrmFetchWidget"
.iX "MrmFetchWidget" "definition"
.mc |
\fBCardinal MrmFetchWidget\fR\^(\^\fIhierarchy_id\fP, \fIindex\fP, \fIparent_widget\fP,
                         \fIwidget\fP, \fIclass\fP\^)
     \fBMrmHierarchy\fR \fIhierarchy_id\fP\^;
     \fBString\fR \fIindex\fP\^;
     \fBWidget\fR \fIparent_widget\fP\^;
     \fBWidget\fR *\^\fIwidget\fP\^;
     \fBMrmType\fR *\^\fIclass\fP\^;
.P
.fi
.ft R
.VL 1.5i
.LI "\fIhierarchy_id\fP"
.mc
Specifies the ID of the UID hierarchy that contains the interface definition.
.mc |
The \fIhierarchy_id\fP was returned in a previous call to \fBMrmOpenHierarchy\fR.
.LI "\fIindex\fP"
.mc
Specifies the UIL name of the widget to fetch. 
.mc |
.LI "\fIparent_widget\fP"
.mc
Specifies the parent widget ID.
.mc |
.LI "\fIwidget\fP"
.mc
Returns the widget ID of the created widget.
If this is not NULL when
you call
.mc |
\fBMrmFetchWidgetOverride,\fR
.mc
MRM assumes that the widget has already been created
and 
.mc |
\fBMrmFetchWidgetOverride\fR
.mc
returns
.mc |
\fBMrmFAILURE\fR.
.LI "\fIclass\fP"
.mc
Returns the class code identifying MRM's widget class.
The widget class code for the main window widget, for example, is
.mc |
\fBMRMwcMainWindow\fR.
.mc
Literals identifying MRM widget class codes are defined in
.mc |
\fBMrm.h\fR.
.LE
.mc
.P 0
.mc |
.ne 3i
The \fBMrmFetchWidget\fR function
.mc
.iX "MrmFetchWidget" "description"
fetches and then creates an
indexed application widget and its children.
The indexed application widget is any widget that is named in UIL
and that is not the child of
any other widget in the UID hierarchy.
In fetch operations, the fetched widget's subtree is also
fetched and created.
This widget must not appear as the child of a widget within its own
subtree. 
.mc |
\fBMrmFetchWidget\fR
.mc
does not execute
.mc |
\fBXtManageChild\fR
.mc
for the newly created widget.
.P 0
.mc *
.mc
An application can fetch
any named widget in the UID hierarchy using
.mc |
\fBMrmFetchWidget\fR.
\fBMrmFetchWidget\fR
.mc
can be called at any time to fetch a widget that was not fetched at
.mc |
application startup.  \fBMrmFetchWidget\fR
.mc
determines if a widget has already been fetched by checking
.mc |
\fIwidget\fP for a NULL value.
Non-NULL values signify that the widget has already been fetched, and
\fBMrmFetchWidget\fR fails.  \fBMrmFetchWidget\fR
can be used to defer fetching pop-up widgets until they are first
referenced (presumably in a callback), and then used to fetch them once.
.mc
.P 0
.mc |
\fBMrmFetchWidget\fR can also create multiple instances of a widget (and its subtree).
.mc
In this case, the UID definition functions as a template;
a widget definition can be fetched any number of times. 
An application can use
this to make multiple instances of a widget, for example, in a
dialog box box or menu. 
.P 0
The index (UIL name) that identifies the widget must be 
known to the application. 
.P 0
.iX "MrmSUCCESS"
.iX "MrmFAILURE"
.iX "MrmNOT_FOUND"
This function returns one of these status return constants: 
.TS
tab(@);
l l
lw(1.25i) lw(3i).
.mc
.sp 6p
.mc |
.R
.mc
.br
.mc |
MrmSUCCESS@The function executed successfully.
.mc
.br
.mc |
MrmNOT_FOUND@Widget not found in UID hierarchy.
.mc
.br
.mc |
MrmFAILURE@The function failed.
.mc
.TE
.H 3 "Overriding MrmFetchWidget Arguments"
.P 0
To fetch any indexed application widget and override the
.mc |
\fBMrmFetchWidget\fR
.mc
arguments, use
.mc |
\fBMrmFetchWidgetOverride\fR:
.P
.nf
\fB#include\fR <\fBMrm/MrmPublic.h\fR>
.mc
.iX "MRM function" "MrmFetchWidgetOverride"
.iX "MrmFetchWidgetOverride" "definition"
.mc |
\fBCardinal MrmFetchWidgetOverride\fR\^(\^\fIhierarchy_id\fP, \fIindex\fP, \fIparent_widget\fP, 
                          \fIoverride_name\fP, \fIoverride_args\fP,
                          \fIoverride_num_args\fP, \fIwidget\fP, \fIclass\fP\^)
     \fBMrmHierarchy\fR \fIhierarchy_id\fP\^;
     \fBString\fR \fIindex\fP\^;
     \fBWidget\fR \fIparent_widget\fP\^;
     \fBString\fR \fIoverride_name\fP\^;
     \fBArgList\fR \fIoverride_args\fP\^;
     \fBCardinal\fR \fIoverride_num_args\fP\^;
     \fBWidget\fR *\^\fIwidget\fP\^;
     \fBMrmType\fR *\^\fIclass\fP\^;
.P
.fi
.ft R
.VL 2i
.LI "\fIhierarchy_id\fP"
.mc
Specifies the ID of the UID hierarchy that contains the interface definition.
.mc |
The \fIhierarchy_id\fP was returned in a previous call to \fBMrmOpenHierarchy\fB.
.LI "\fIindex\fP"
.mc
Specifies the UIL name of the widget to fetch. 
.mc |
.LI "\fIparent_widget\fP"
.mc
Specifies the parent widget ID.
.mc |
.LI "\fIoverride_name\fP"
.mc
Specifies the name to override the widget name. 
Use a NULL value if you do not want to override the widget name. 
.mc |
.LI "\fIoverride_args\fP"
Specifies the override argument list, exactly as given to \fBXtCreateWidget\fR
(conversion complete and so forth).  Use a
.mc
NULL value if you do not want to override the argument list.
.mc |
.LI "\fIoverride_num_args\fP"
Specifies the number of arguments in \fIoverride_args\fP.
.br
.ne 2i
.LI "\fIwidget\fP"
.mc
Returns the widget ID of the created widget.
.mc |
If this is not NULL when you call \fBMrmFetchWidgetOverride\fR,
MRM assumes that the widget has already been created and 
\fBMrmFetchWidgetOverride\fR returns MrmFAILURE.
.LI "\fIclass\fP"
.mc
Returns the class code identifying MRM's widget class.
.mc |
For example, the widget class code for the main window widget is
\fBMRMwcMainWindow\fR.
.mc
Literals identifying MRM widget class codes are defined in
.mc |
\fBMrm.h\fR.
.LE
.P
The \fBMrmFetchWidgetOverride\fR function
.mc
.iX "MrmFetchWidgetOverride" "description"
.mc |
is the extended version of \fBMrmFetchWidget\fR.
It is identical to \fBMrmFetchWidget\fR, except that it
.mc
allows the caller to override the widget's name and any
.mc |
arguments that \fBMrmFetchWidget\fR otherwise retrieves from the UID file or
one of the defaulting mechanisms.  That is, the override argument list is not
.mc
limited to those arguments in the UID file.
.P 0
.mc |
The override arguments apply only to the widget fetched and returned
by this function.  Its children (subtree) do not receive any override
.mc
parameters.
.P 0
.iX "MrmSUCCESS"
.iX "MrmFAILURE"
.iX "MrmNOT_FOUND"
This function returns one of these status return constants: 
.TS
tab(@);
l l
lw(1.25i) lw(3i).
.mc
.sp 6p
.mc |
.R
.mc
.br
.mc |
MrmSUCCESS@The function executed successfully.
.mc
.br
.mc |
MrmNOT_FOUND@Widget not found in UID hierarchy.
.mc
.br
.mc |
MrmFAILURE@The function failed.
.mc
.TE
.mc |
.br
.ne 4i
.mc
.H 2 "Fetching Literals"
.P 0
The Motif Toolkit provides functions with which you can fetch
literals from UID files.
Specifically, the section discusses how to fetch:
.BL
.LI
A named color literal
.LI
An icon literal
.LI
A literal value
.LE
.mc |
.H 3 "Fetching a Named Color Literal"
.mc
.P 0
.mc |
To fetch a named color literal, use \fBMrmFetchColorLiteral\fR:
.P
.nf
\fB#include\fR <\fBMrm/MrmPublic.h\fR>
.mc
.iX "MRM function" "MrmFetchColorLiteral"
.iX "MrmFetchColorLiteral" "definition"
.mc |
\fBint MrmFetchColorLiteral\fR\^(\^\fIhierarchy_id\fP, \fIindex\fP, \fIdisplay\fP,
                             \fIcolormap_id\fP, \fIpixel\fP\^)
     \fBMrmHierarchy\fR \fIhierarchy_id\fP\^;
     \fBString\fR \fIindex\fP\^;
     \fBDisplay\fR *\^\fIdisplay\fP\^;
     \fBColormap\fR \fIcolormap_id\fP\^;
     \fBPixel\fR *\^\fIpixel\fP\^;
.P
.fi
.ft R
.VL 2i
.LI "\fIhierarchy_id\fP"
Specifies the ID of the UID hierarchy that contains the specified literal. 
The \fIhierarchy_id\fP was returned in a previous call to \fBMrmOpenHierarchy\fR.
.LI "\fIindex\fP"
.mc
Specifies the UIL name of the color literal to fetch. 
You must define this name in UIL as an exported value.
.mc |
.LI "\fIdisplay\fP"
Specifies the display used for the pixmap.  The \fIdisplay\fP 
.mc
argument specifies the connection to the X server. 
For more information on the
.mc |
Display structure, see the Xlib function \fBXOpenDisplay\fR.
.LI "\fIcolormap_id\fP"
Specifies the ID of the colormap.
If NULL, the default colormap is used. 
.LI "\fIpixel\fP"
.mc
Returns the ID of the color literal.
.mc |
.LE
.mc
.P 0
.mc |
.ne 3i
The \fBMrmFetchColorLiteral\fR function
.mc
.iX "MrmFetchColorLiteral" "description"
fetches a named color literal from a UID file, and converts the
color literal to a pixel color value.
.P 0
.iX "MrmSUCCESS"
.iX "MrmFAILURE"
.iX "MrmNOT_FOUND"
This function returns one of these status return constants: 
.TS
tab(@);
l l
lw(1.25i) lw(3i).
.mc
.sp 6p
.br
.mc |
.R
MrmSUCCESS@The function executed successfully.
.mc
.br
.mc |
MrmNOT_FOUND@The color literal was not found in the UIL file.
.mc
.br
.mc |
MrmFAILURE@The function failed.
.mc
.TE
.mc |
.H 3 "Fetching an Icon Literal"
.mc
.P 0
To fetch an icon literal, use
.mc |
\fBMrmFetchIconLiteral\fR:
.P
.nf
\fB#include\fR <\fBMrm/MrmPublic.h\fR>
.mc
.iX "MRM function" "MrmFetchIconLiteral"
.iX "MrmFetchIconLiteral" "definition"
.mc |
\fBint MrmFetchIconLiteral\fR\^(\^\fIhierarchy_id\fP, \fIindex\fP, \fIscreen\fP,
                      \fIdisplay\fP, \fIfgpix\fP, \fIbgpix\fP, \fIpixmap\fP\^)
    \fBMrmHierarchy\fR \fIhierarchy_id\fP\^;
    \fBString\fR \fIindex\fP\^;
    \fBScreen\fR *\^\fIscreen\fP\^;
    \fBDisplay\fR *\^\fIdisplay\fP\^;
    \fBPixel\fR \fIfgpix\fP\^;
    \fBPixel\fR \fIbgpix\fP\^;
    \fBPixmap\fR *\^\fIpixmap\fP\^;
.P
.fi
.ft R
.VL 1.5i
.LI "\fIhierarchy_id\fP"
.mc
Specifies the ID of the UID hierarchy that contains the
specified icon literal. 
The
\fIhierarchy_id\fP
was returned in a previous call to
.mc |
\fBMrmOpenHierarchy\fR.
.LI "\fIindex\fP"
.mc
Specifies the UIL name of the icon literal to fetch. 
.mc |
.LI "\fIscreen\fP"
.mc
Specifies the screen used for the pixmap.
The
\fIscreen\fP
argument specifies a pointer to the
Xlib structure
.mc |
Screen
that contains the information about that screen and is linked to the
Display
.mc
structure.
For more information on the
.mc |
Display
.mc
and
.mc |
Screen
structures, see the Xlib function
\fBXOpenDisplay\fR
.mc
and the associated screen information macros.
.mc |
.br
.ne 3i
.LI "\fIdisplay\fP"
.mc
Specifies the display used for the pixmap.
.mc |
The \fIdisplay\fP argument specifies the connection to the X server. 
.mc
For more information on the
.mc |
Display structure, see the Xlib function \fBXOpenDisplay\fR.
.LI "\fIfgpix\fP"
.mc
Specifies the foreground color for the pixmap.
.mc |
.LI "\fIbgpix\fP"
.mc
Specifies the background color for the pixmap.
.mc |
.LI "\fIpixmap\fP"
.mc
Returns the resulting X pixmap value.
.mc |
.LE
.mc
.P 0
The
.mc |
\fBMrmFetchIconLiteral\fR
.mc
function
.iX "MrmFetchIconLiteral" "description"
fetches an icon literal from an Mrm.hierarchy, and converts the
icon literal to an X pixmap.
.P 0
.iX "MrmSUCCESS"
.iX "MrmFAILURE"
.iX "MrmNOT_FOUND"
This function returns one of these status return constants: 
.TS
tab(@);
l l
lw(1.25i) lw(3i).
.mc
.sp 6p
.mc |
.R
.mc
.br
.mc |
MrmSUCCESS@The function executed successfully.
.mc
.br
.mc |
MrmNOT_FOUND@The icon literal was not found in the hierarchy.
.mc
.br
.mc |
MrmFAILURE@The function failed.
.mc
.TE
.mc |
.br
.ne 10
.H 3 "Fetching a Literal Value"
.mc
.P 0
To fetch a literal value, use
.mc |
\fBMrmFetchLiteral\fR:
.P
.nf
\fB#include\fR <\fBMrm/MrmPublic.h\fR>
.mc
.iX "MRM function" "MrmFetchLiteral"
.iX "MrmFetchLiteral" "definition"
.mc |
\fBint MrmFetchLiteral\fR\^(\^\fIhierarchy_id\fP, \fIindex\fP, \fIdisplay\fP, \fIvalue\fP, \fItype\fP\^)
     \fBMrmHierarchy\fR \fIhierarchy_id\fP\^;
     \fBString\fR \fIindex\fP\^;
     \fBDisplay\fR *\^\fIdisplay\fP\^;
     \fBcaddr_t\fR *\^\fIvalue\fP\^;
     \fBMrmCode\fR *\^\fItype\fP\^;
.fi
.ft R
.sp .25
.VL 1.5i
.LI "\fIhierarchy_id\fP"
.mc
Specifies the ID of the UID hierarchy that contains the
specified literal. 
The
\fIhierarchy_id\fP
was returned in a previous call to
.mc |
\fBMrmOpenHierarchy\fR.
.br
.ne 3i
.LI "\fIindex\fP"
.mc
Specifies the UIL name of the literal (pixmap) to fetch. 
You must define this name in UIL as an exported value.
.mc |
.LI "\fIdisplay\fP"
.mc
Specifies the display used for the pixmap.
The
\fIdisplay\fP 
argument specifies the connection to the X server. 
For more information on the
.mc |
Display structure, see the Xlib function
\fBXOpenDisplay\fR.
.LI "\fIvalue\fP"
.mc
Returns the ID of the named literal's value.
.mc |
.LI "\fItype\fP"
.mc
Returns the named literal's data type.
.mc |
.LE
.mc
.P 0
The
.mc |
\fBMrmFetchLiteral\fR
.mc
function
.iX "MrmFetchLiteral" "description"
reads and returns the value and type of a literal (named value) 
that is stored as
a public resource in a single UID file.
This function returns a pointer to the value of the literal.
For example, an integer is always returned as a pointer to an integer,
and a string is always returned as a pointer to a string.
.P 0
Applications should not use
.mc |
\fBMrmFetchLiteral\fR
.mc
for fetching icon or color literals.
If this is attempted,
.mc |
\fBMrmFetchLiteral\fR
.mc
returns an error.
.P 0
.iX "MrmSUCCESS"
.iX "MrmFAILURE"
.iX "MrmNOT_FOUND"
.iX "MrmWRONG_TYPE"
This function returns one of these status return constants: 
.TS
tab(@);
l l
lw(1.25i) lw(3i).
.mc
.sp 6p
.mc |
.R
.mc
.br
.mc |
MrmSUCCESS@The function executed successfully.
.mc
.br
.mc |
MrmWRONG_TYPE@The operation encountered an unsupported literal type.
.mc
.br
.mc |
MrmNOT_FOUND@The literal was not found in the UID file.
.mc
.br
.mc |
MrmFAILURE@The function failed.
.mc
.TE
.mc |


.mc
