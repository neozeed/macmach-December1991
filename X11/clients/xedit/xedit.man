.TH XEDIT 1 "Release 4" "X Version 11"
.SH NAME
xedit - simple text editor for X
.SH SYNTAX
\fBxedit\fP [ \fI-toolkitoption\fP ...] [ filename ]
.SH OPTIONS
.I Xedit
accepts all of the standard X Toolkit command line
options (see \fIX(1)\fP).  The order of the command line options is
not important.
.TP 8
.I filename
Specifies the file that is to be loaded during start-up. This is the
file which will be edited. If a file is not specified,
.I xedit
lets you load a file or create a new file after it has started up.
.SH DESCRIPTION
.I Xedit
provides a window consisting of the following four areas:
.IP "Commands Section" 25
A set of commands that allow you to exit \fIxedit\fP, save the file, or
load a new file into the edit window.
.IP "Message Window" 25
Displays \fIxedit\fP messages. In addition, this window can be used as
a scratch pad. 
.IP "Filename Display"
Displays the name of the file currently being edited, and whether this file
is \fIRead - Write\fP or \fIRead Only\fP.
.IP "Edit Window" 25
Displays the text of the file that you are editing or creating.  
.SH EDITING
The Athena Text widget is used for the three sections of this
application that allow text input.  The characters typed will go to
the Text widget that the pointer cursor is currently over.  If the
pointer cursor is not over a text widget then the keypresses will have
no effect on the application.  This is also true for the special key
sequences that popup dialog widgets, so typing Control-S in the filename
widget will enable searching in that widget, not the edit widget.
.PP
Both the message window and the edit window will create a scrollbar if
the text to display is too large to fit in that window.  Horizontal scrolling
is not allowed by default, but can be turned on through the Text
widget's resources, see \fIAthena Widget Set\fP for the exact
resource definition.
.SH COMMANDS
.IP "Quit" 8
Quits the current editing session. If any changes have not been saved,
.I xedit 
displays a warning message, allowing the user to save the file.
.IP "Save" 
If file backups are enabled (see RESOURCES) xedit stores a copy of the
original, unedited file in <prefix>\fIfile\fP<suffix>,
then overwrites the \fIfile\fP with the contents of the edit window.  The
filename is retrieved from the Text widget directly to the right of
the \fILoad\fP button.
.IP "Load"
Loads the file named in the text widget immediately to the right
of the this button and displays it in the Edit window.  If the
currently displayed file has been modified a warning message will ask
the user to save the changes, or press \fIload\fP again.
.SH RESOURCES
For \fIxedit\fP the available resources are:
.TP 8
.B enableBackups (\fPClass\fB EnableBackups)
Specifies that, when edits made to an existing file are saved,
.I xedit
is to copy the original version of that file to <prefix>\fIfile\fP<suffix>
before it saves the changes.  The default value for this resource is
"off", stating that no backups should be created.
.TP 8
.B backupNamePrefix (\fPClass\fB BackupNamePrefix)
Specifies a string that is to be prepended to the backup filename.  The
default is that no string shall be prepended.
.TP 8
.B backupNameSuffix (\fPClass\fB BackupNameSuffix)
Specifies a string that is to be appended to the backup filename.  The
default is to append the string ".BAK".
.SH WIDGETS
In order to specify resources, it is useful to know the hierarchy of
the widgets which compose \fIxedit\fR.  In the notation below,
indentation indicates hierarchical structure.  The widget class name
is given first, followed by the widget instance name.
.sp
.nf
.TA .5i 1.0i 1.5i 2.0i
.ta .5i 1.0i 1.5i 2.0i
Xedit  xedit
	Paned  paned
		Paned  buttons
			Command  quit
			Command  save
			Command  load
			Text  filename
		Label  bc_label
		Text  messageWindow
		Label  labelWindow
		Text  editWindow
.fi
.sp
.SH ENVIRONMENT
.PP
.TP 8
.B DISPLAY
to get the default host and display number.
.TP 8
.B XENVIRONMENT
to get the name of a resource file that overrides the global resources
stored in the RESOURCE_MANAGER property.
.SH FILES
/usr/lib/X11/app-defaults/Xedit - specifies required resources
.SH SEE ALSO
X(1), xrdb(1), Athena Widget Set
.SH RESTRICTIONS
There is no \fIundo\fP function.
.SH COPYRIGHT
Copyright 1988, Digital Equipment Corporation.
.br
Copyright 1989, Massachusetts Institute of Technology.
.br
See \fIX(1)\fP for a full statement of rights and permissions.
.SH AUTHOR
Chris D. Peterson, MIT X Consortium

