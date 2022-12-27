.mc |
.\"  (c) Copyright 1989, 1990 OPEN SOFTWARE FOUNDATION, INC.
.mc
.\"  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
.\"  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
.\"  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
.\"  (c) Copyright 1988 MICROSOFT CORPORATION
.\"  ALL RIGHTS RESERVED
.nr H1 4
.mc |
.nr !_ 0
.mc
.H 1 "Dialog Widgets and Functions"
.iX "Widgets, dialog"
.iX "Dialog widgets"
.iX "Dialog functions"
Dialog widgets are container widgets that provide a means of communicating
between the user of an application and the application itself.  A Dialog
widget will normally ask a question or present some information to the
user.  In some cases, the application is suspended until the user provides
a response.
.P
This chapter explains the Dialog widgets that are available and how they
can be used in your application.
.H 2 "Dialog Widgets and Menus"
.iX "Widgets, dialog" "Single-reply"
.iX "Widgets, dialog" "multiple-reply"
.iX "Widgets, dialog" "and menus"
There are two types of Dialog widgets: single-reply and multiple-reply.  
A single-reply Dialog widget consists of a single question,  and a single
reply is expected.  A multiple-reply Dialog widget consists of a number
of questions that require a number of responses.
.iX "Modal dialog widget"
.iX "Modeless dialog widget"
Generally speaking,
.mc |
a single-reply Dialog widget is modal in nature.  This means that
.mc
a reply is required before the application can continue.  A multiple-reply
.mc |
Dialog widget is usually modeless.  It does not require a reply and
.mc
does not stop the progress of the application.
.P
.mc |
There are many similarities between Dialog widgets and menus, and knowing
when to use a Dialog widget and when to
use a menu may be difficult.  You should understand the differences between Dialog widgets
.mc
and menus in order to make this determination.
.P
A menu is short-lived.  It exists only while a selection is being made,
then it disappears.  A Dialog widget stays visible until it is told to
disappear.
.P
.mc |
A menu is usually modal.  Until the user of the application makes some
.mc
selection on the menu, interaction with any other part of the application
.mc |
is not possible.  Since multiple-reply Dialog widgets are modeless, 
.mc
interaction with other parts of the application is possible.  Thus, if a 
modeless state is required, a multiple-reply or modeless single-reply
Dialog widget should be used instead of a menu.  A menu is faster when
there is a need to identify current settings or make a single selection. 
When multiple selections are needed, a menu disappears after each
selection and has to be displayed over and over, so in this case
.mc |
a Dialog widget works better.
.mc
.H 2 "A List of the Dialog Widgets"
The following list identifies the Dialog widgets by name and provides a 
brief description of each widget's function.  Each widget will be described
in more detail in later sections of this chapter.  Additional information
.mc |
can be found in the manpage for the respective widget.  Manpages are
contained in the \fIOSF/Motif Programmer's Reference\fP.
.mc
.iX "XmDialogShell"
.iX "XmBulletinBoard"
.iX "XmCommand"
.iX "XmFileSelectionBox"
.iX "XmForm"
.iX "XmMessageBox"
.iX "XmSelectionBox"
.BL
.LI
\fBXmDialogShell\fP.  This widget is used as the parent for all Dialogs.
.mc |
It is automatically created by Dialog convenience functions (described
.mc
in a later section).
.LI
\fBXmBulletinBoard\fP.  This is a composite widget that provides simple
geometry management for its child widgets.  It is the base widget for most
Dialog widgets, but is also used as a container widget.
.LI
\fBXmCommand\fP.  This widget is a subclass of the \fBXmSelectionBox\fP
widget.  It includes a command line input text field, a command line
prompt, and a command history region.
.LI
\fBXmFileSelectionBox\fP.  This widget is used to traverse through
file system directories.  You can view the files in the directories and
then select a single file on which you intend to perform some action.
.mc |
.br
.ne 5
.mc
.LI
.mc |
\fBXmForm\fP.  This widget is a constraint-based Manager widget that
.mc
establishes and maintains spatial relationships between its children.  
These relationships are maintained even though the Form or any of its
children are resized, unmanaged, remanaged, or destroyed.
.LI
\fBXmMessageBox\fP.  This widget is used to pass information to the user
.mc |
of an application.  It contains up to three standard push buttons
(\fBOK\fP, \fBCancel\fP, and \fBHelp\fP by default), a message symbol, and the
.mc
message itself.
.LI
\fBXmSelectionBox\fP.  This widget provides the capability of selecting a
single item from a list of items.
.LE
.H 2 "Convenience Dialogs"
.iX "Convenience dialogs"
.mc |
In this guide, Dialog widget describes a particular type of
widget.  When used by itself, the word Dialog takes on a special
meaning.  A Dialog is a collection of widgets that are used for a
.mc
specific purpose.
A Dialog normally consists of a DialogShell, some BulletinBoard resources, and
various other widgets such as Label, PushButton, and Text.  The
collection of widgets that forms a Dialog can be built
.mc |
from scratch using argument lists and creating each individual widget
.mc
of the Dialog.  However, there is an easier, more convenient method to
create the Dialog.
.iX "Dialog convenience functions"
.mc |
Functions (called, appropriately, Dialog convenience
functions) exist that create the collection of widgets that make up the 
.mc
Dialog in one step.  The Dialogs that are created by these convenience
.mc |
functions are referred to as convenience Dialogs.
.mc
The following table identifies the convenience dialogs
that are available.
.mc |
.br
.ne 22
.mc
.iX "BulletinBoardDialog"
.iX "XmCreateBulletinBoardDialog"
.iX "ErrorDialog"
.iX "XmCreateErrorDialog"
.iX "FileSelectionDialog"
.iX "XmCreateFileSelectionDialog"
.iX "FormDialog"
.iX "XmCreateFormDialog"
.iX "InformationDialog"
.iX "XmCreateInformationDialog"
.iX "PromptDialog"
.iX "XmCreatePromptDialog"
.mc |
.sp
.mc
.TB "Convenience Dialogs"
.mc |
.in0
.TS H
box, tab (;);
lbw(1.125i) | lbw(2.00i) | lbw(1.625i)
l | l | lb.
.mc
.sp .2
Convenience Dialog;Definition;Convenience Function
_
.mc |
.TH
.mc
.sp .2
BulletinBoardDialog;T{
.mc |
Used for interactions that are not supported by the standard Dialog set.
.mc
T};XmCreateBulletinBoardDialog
_
.sp .2
ErrorDialog;T{
This Dialog instantiates a MessageBox and a DialogShell.
It uses a message and a symbol (circle with backslash) to warn the user
that an error has occurred.
.mc |

.mc
T};XmCreateErrorDialog
_
.sp .2
FileSelectionDialog;T{
This Dialog instantiates a FileSelectionBox and a DialogShell.
.mc |
T};XmCreateFileSelectionDialog
.mc
_
.sp .2
FormDialog;T{
This Dialog instantiates a Form and a DialogShell.
.mc |
T};XmCreateFormDialog
.mc
_
.sp .2
InformationDialog;T{
This Dialog instantiates a MessageBox and a
DialogShell.  This Dialog provides information to the user and it has a symbol
.mc |
that consists of a large lowercase ``i'' that is positioned on 
.mc
the left side of the MessageBox.
.mc |
T};XmCreateInformationDialog
.mc
_
.sp .2
PromptDialog;T{
This Dialog instantiates a SelectionBox and a
DialogShell.  It is used to prompt the user for input.
.mc |
T};XmCreatePromptDialog
.mc
.sp .2
.mc |
_
.mc
.iX "QuestionDialog"
.iX "XmCreateQuestionDialog"
.iX "SelectionDialog"
.iX "XmCreateSelectionDialog"
.iX "WarningDialog"
.iX "XmCreateWarningDialog"
.iX "WorkingDialog"
.iX "XmCreateWorkingDialog"
.mc *
.mc
.sp .2
.mc *
.mc
QuestionDialog;T{
This Dialog instantiates a MessageBox and a
DialogShell.  It is used to get an answer from the user.  It has
a symbol that consists of a large question mark that is positioned on the 
left side of the MessageBox.
.mc |
T};XmCreateQuestionDialog
.mc
_
.sp .2
SelectionDialog;T{
This Dialog instantiates a SelectionBox and a DialogShell.
.mc |
T};XmCreateSelectionDialog
.mc
_
.sp .2
WarningDialog;T{
This Dialog instantiates a MessageBox and a
DialogShell.  It is used to warn the user of some potential danger.
It has a symbol that consists of a large exclamation point contained
within a triangle.  The symbol is positioned on the left side of the
MessageBox.
.mc |
T};XmCreateWarningDialog
.mc
_
.sp .2
WorkingDialog;T{
This Dialog instantiates a MessageBox and a DialogShell.  It is used to
inform the user that a potentially time-consuming operation is in progress.
It has a symbol that is an hourglass positioned on the left side of the 
MessageBox.
.mc |
T};XmCreateWorkingDialog
.mc
.sp .2
.TE
.mc |
.in
.mc
.H 2 "Using Dialogs and Convenience Functions"
.iX "Using dialogs"
.iX "Using dialog convenience functions"
.iX "Dialogs, using"
.iX "Dialog convenience functions, using"
.mc |
Now that you have an idea of what Dialogs and convenience Dialogs are
.mc
available, you can learn how and when to use them in an application.
.mc |
This section explains each Dialog and its associated convenience functions.
.mc
Code segments are included to help you understand how to use them in your
.mc |
own applications.  More detailed information about these
widgets can be found in the respective manpages in the \fIOSF/Motif
Programmer's Reference .\fP
.mc
.H 3 "XmDialogShell"
.iX "XmDialogShell"
.mc |
\fBXmDialogShell\fP is the Shell parent widget for all Dialogs.  It provides
.mc
the necessary communication with the window manager to allow the Dialogs
.mc |
to be managed and unmanaged.  \fBXmDialogShell\fP is
.mc
automatically created by the Dialog convenience functions and is used as the
.mc |
parent widget for \fBXmBulletinBoard\fP or any subclass of \fBXmBulletinBoard\fP. 
.mc
It can also be directly instantiated by using either of the two create
functions that are available:
.iX "XtCreatePopupShell"
.iX "XmCreateDialogShell"
.P
.nf
.mc |
\f(CWWidget XtCreatePopupShell (name, 
	xmDialogShellWidgetClass, parent, 
	arglist, argcount)
.mc

.mc |
Widget XmCreateDialogShell (parent, name, arglist, 
	argcount)\fP
.br
.ne 7
.mc
.P
.fi
Both of these create functions create an instance of a DialogShell and return
the associated widget ID.  The following code segment shows you how to create
\fBXmDialogShell\fP using \fBXtCreatePopupShell\fP:
.P
.nf
\f(CWArg	args;
int	n;
Widget	dialog_shell;
Widget	parent_shell;

n = 0;
.mc |
dialog_shell = XtCreatePopupShell ("dialog_shell", 
xmDialogShellWidgetClass, parent_shell, args, n);\fP
.br
.ne 4
.mc
.P
.fi
The next code segment shows you how to create \fBXmDialogShell\fP
using \fBXmCreateDialogShell\fP:
.P
.nf
\f(CWArg	args;
int	n;
Widget	dialog_shell;
Widget	parent_shell;

n = 0;
.mc |
dialog_shell = XmCreateDialogShell (parent_shell, 
"dialog_shell", args, n);\fP
.mc
.P
.fi
.mc |
Remember that \fBXmDialogShell\fP is automatically created for you
.mc
when you create any of the convenience Dialogs.  You do not need to use
.mc |
either of the functions previously described when you create a convenience
.mc
Dialog.
.iX "XtManageChild"
.iX "XtCreateManagedWidget"
.mc |
Also, note that \fBXmDialogShell\fP should be popped up by using
\fBXtManageChild\fP on its Dialog child.  If the child is created using
\fBXtCreateManagedWidget\fP, it will try to pop up the shell before it has
.mc
been realized.  This will result in an error.
.H 3 "XmBulletinBoard"
.iX "XmBulletinBoard"
.iX "XmDialogShell"
\fBXmBulletinBoard\fP is a composite widget that can be instantiated alone.
Its main purpose with Dialog widgets, however, is as a superclass widget
that supplies resources
to the subclass Dialog widgets.  All of the other Dialog widgets except
\fBXmDialogShell\fP are built in part from \fBXmBulletinBoard\fP.  Refer
.mc |
to the \fBXmBulletinBoard\fP manpage in the \fIOSF/Motif Programmer's
Reference\fP for a description of the \fBXmBulletinBoard\fP
.mc
resources.
.mc |
.br
.ne 4
.mc
.P
\fBXmBulletinBoard\fP can be directly instantiated by using either
of the two create
functions that are available:
.iX "XtCreateWidget"
.iX "XmCreateBulletinBoard"
.P
.nf
.mc |
\f(CWWidget XtCreateWidget (name, 
	xmBulletinBoardWidgetClass, 
	parent, arglist, argcount)
.mc

.mc |
Widget XmCreateBulletinBoard (parent, name, 
	arglist, argcount)\fP
.ne 7
.mc
.P
.fi
Both of these create functions create an instance of a BulletinBoard and return
the associated widget ID.  The following code segment shows you how to create
an instance of \fBXmBulletinBoard\fP using \fBXmCreateWidget\fP:
.P
.nf
\f(CWArg	args;
int	n;
Widget	bboard;
Widget	parent_shell;

n = 0;
.mc |
bboard = XtCreateWidget ("bboard", 
	xmBulletinBoardWidgetClass, 
	parent_shell, args, n);
.mc
XtManageChild (bboard);\fP
.P
.fi
The next code segment shows you how to create an instance of 
\fBXmBulletinBoard\fP
using \fBXmCreateBulletinBoard\fP:
.P
.nf
\f(CWArg	args;
int	n;
Widget	bboard;
Widget	parent_shell;

n = 0;
.mc |
bboard = XmCreateBulletinBoard (parent_shell, 
	"bboard", args, n);
.mc
XtManageChild (bboard);\fP
.P
.fi
Remember that you do not need to use either of the create functions
.mc |
previously described if you are creating convenience Dialogs.
.mc
.H 3 "XmCommand"
.iX "XmCommand"
.iX "XmSelectionBox"
.iX "Command history region"
.iX "History region, Command"
\fBXmCommand\fP allows you to choose one selection from a list of
selections.  It is very much like the \fBXmSelectionBox\fP widget except
.mc |
that it can record selections in a history region.
.mc
The history region is accessible so that choices can be made from it as 
.mc |
well as entering a choice from the keyboard.  The following figure shows an
.mc
example of a Command widget. 
.ig ++
.br
.ie \nZ \{\
.br
.ne 18.5P
.P
.sC graphics/command1.tif tiff 300 13P 16.5P
\&
.P
.sp +1
.FG "XmCommand Widget"
.sp +1.5
.\}
.el \{\
.br
.ne 18.5P
\&
.sp 16.5P
.ce
illustration command1.eps goes here
.sp +1
.FG "XmCommand Widget"
.sp +1.5
.\}
.++
.br
.mc |
.ne 3.25i
.mc
\0
.mc *
.mc
.FG "XmCommand Widget"
.mc |
.sp 3i
.in +1.7i
.P! graphics/command1.ps 
.in -1.7i
.mc
.P
The history region is displayed in the
top box and the current selection is displayed in the command line box
at the bottom.  The history region will scroll automatically as the
.mc |
need arises.  In Figure 5-2 for example, more entries have been 
.mc
placed in the history region, causing the vertical ScrollBar to appear.
.ig ++
.br
.ie \nZ \{\
.br
.ne 14.5P
.C
.sC graphics/command2.tif tiff 300 14P 12.5P
\&
.P
.sp +1
.mc |
.FG "XmCommand Widget With Scrolled History Region"
.mc
.sp +1.5
.\}
.el \{\
.br
.ne 14.5P
\&
.sp 12.5P
.ce
illustration command2.eps goes here
.sp +1
.FG "XmCommand Widget with Scrolled History Region"
.sp +1.5
.\}
.++
.br
.mc |
.ne 2.5i
.mc
\0
.mc *
.mc
.FG "XmCommand Widget with Scrolled History Region"
.mc |
.sp 2.25i
.in +1.6i
.P! graphics/command2.ps 
.in -1.6i
.br
.ne 5
.mc
.P
\fBXmCommand\fP can be directly instantiated by using either
of the two create
functions that are available:
.iX "XtCreateWidget"
.iX "XmCreateCommand"
.P
.nf
.mc |
\f(CWWidget XtCreateWidget (name, 
	xmCommandWidgetClass, 
	parent, arglist, argcount)
.mc

.mc |
Widget XmCreateCommand (parent, name, arglist, 
	argcount)\fP
.br
.ne 7
.mc
.P
.fi
Both of these create functions create an instance of a Command widget and return
the associated widget ID.  The following code segment shows you how to create
an instance of \fBXmCommand\fP using \fBXmCreateWidget\fP:
.P
.nf
\f(CWArg	args;
int	n;
Widget	command;
Widget	parent_shell;

n = 0;
.mc |
command = XtCreateWidget ("command",
    xmCommandWidgetClass, parent_shell, args, n);
.mc
XtManageChild (command);\fP
.P
.fi
The next code segment shows you how to create an instance of 
\fBXmCommand\fP
using \fBXmCreateCommand\fP:
.iX "XmCreateCommand"
.P
.nf
\f(CWArg	args;
int	n;
Widget	command;
Widget	parent_shell;

n = 0;
.mc |
command = XmCreateCommand (parent_shell, "command", 
	args, n);
.mc
XtManageChild (command);\fP
.P
.fi
.mc |
.br
.ne 15
.mc
There are several other functions associated with \fBXmCommand\fP that 
perform certain operations to the command area string or the history
region string.  These functions are explained in the following list:
.BL
.LI
.iX "XmCommandAppendValue"
.mc |
\fBXmCommandAppendValue\fP
.mc
.nf
\f(CWvoid XmCommandAppendValue (widget, command)

Widget	widget;
XmString	command;\fP
.fi
.P
This function appends the passed, null-terminated command string to the end 
.mc |
of the string that is currently displayed in the command line.  The following figure shows
the resulting window from the \fBXmCommandAppendValue\fR call.
.mc
.ig ++
.br
.ie \nZ \{\
.br
.ne 14.5P
.C
.sC graphics/command3.tif tiff 300 14P 12.5P
\&
.P
.sp +1
.mc |
.FG "Results of XmCommandAppendValue Operation"
.mc
.sp +1.5
.\}
.el \{\
.br
.ne 14.5P
\&
.sp 12.5P
.ce
illustration command3.eps goes here
.sp +1
.mc |
.FG "Results of XmCommandAppendValue Operation"
.mc
.sp +1.5
.\}
.P
.++
.br
.mc |
.ne 2.5i
.mc
\0
.mc |
.FG "Results of XmCommandAppendValue Operation"
.sp 2.25i
.mc
.in +1.6i
.P! graphics/command3.ps 
.in -1.6i
.mc *
.mc
.P
The code segment to accomplish
.mc |
this is as follows.
.mc
.iX "XmCommandAppendValue"
.iX "XmStringCreateLtoR"
.P
.nf
\f(CWXmString	str;
Widget	w;

.mc |
str = XmStringCreateLtoR ("addValue", 
	XmSTRING_DEFAULT_CHARSET);
.mc
XmCommandAppendValue (w, str);\fP
.fi
.LI
.iX "XmCommandSetValue"
.mc |
\fBXmCommandSetValue\fP
.mc
.nf
\f(CWvoid XmCommandSetValue (widget, command)

Widget	widget;
XmString	command;\fP
.fi
.P
This function replaces the string that is currently displayed in the command
line with the passed, null-terminated string.
.LI
.iX "XmCommandError"
.mc |
\fBXmCommandError\fP
.mc
.nf
\f(CWvoid XmCommandError (widget, error)

Widget	widget;
XmString	error;\fP
.fi
.P
This function displays an error message in the history region.   
.LI
.iX "XmCommandGetChild"
.mc |
\fBXmCommandGetChild\fP
.mc
.nf
\f(CWvoid XmCommandGetChild (widget, child)

Widget	widget;
uns char	child;\fP
.fi
.P
This function returns the widget ID of the given child.  The function
takes these child types:
.BL
.LI
.mc |
\fBXmDIALOG_PROMPT_LABEL
.mc
.LI
.mc |
XmDIALOG_COMMAND_TEXT
.mc
.LI
.mc |
XmDIALOG_HISTORY_LIST\fP
.mc
.LE
.LE
.H 3 "XmFileSelectionBox"
.iX "XmFileSelectionBox"
\fBXmFileSelectionBox\fP is a widget very similar to \fBXmSelectionBox\fP.
The difference is that \fBXmFileSelectionBox\fP is used to traverse through
directories, viewing the names of the files and finally selecting a file on
.mc |
which to perform some action.  One thing to remember about this widget is that
you must link in the library PW (\-lPW) in order for FileSelectionBox
to work properly.  The follwoingfigure shows an example of a
.mc
FileSelectionBox.  
.ig ++
.br
.ie \nZ \{\
.br
.ne 21.5P
.P
.sC graphics/fselbox1.tif tiff 300 16P 19.5P
\&
.P
.sp +1
.FG "XmFileSelectionBox"
.sp +1.5
.\}
.el \{\
.br
.ne 21.5P
\&
.sp 19.5P
.ce
illustration fselbox1.eps goes here
.sp +1
.FG "XmFileSelectionBox"
.sp +1.5
.\}
.P
.++
.br
.mc |
.ne 3.75i
.mc
\0
.mc *
.mc
.FG "XmFileSelectionBox"
.mc |
.sp 5.5i
.in +.65i
.P! graphics/fileselbox.ps 
.in -.65i
.mc
.P
\fBXmFileSelectionBox\fP can be directly instantiated by using either
of the two create
functions that are available:
.iX "XmFileSelectionBox"
.iX "XtCreateWidget"
.iX "XmCreateFileSelectionBox"
.P
.nf
.mc |
\f(CWWidget XtCreateWidget (name, 
	xmFileSelectionBoxWidgetClass, 
	parent, arglist, argcount)
.mc

.mc |
Widget XmCreateFileSelectionBox (parent, name, 
	arglist, argcount)\fP
.br
.ne 21
.mc
.P
.fi
.mc |
Both of these create functions create an instance of a FileSelectionBox
.mc
widget and return
the associated widget ID.  The following code segment shows you how to create
an instance of \fBXmFileSelectionBox\fP using \fBXmCreateWidget\fP:
.P
.nf
\f(CWArg	args;
int	n;
Widget	fselbox;
Widget	parent_shell;

n = 0;
.mc |
fselbox = XtCreateWidget ("fselbox", 
	xmFileSelectionBoxWidgetClass,
	parent_shell, args, n);
.mc
XtManageChild (fselbox);\fP
.P
.fi
The next code segment shows you how to create an instance of 
\fBXmFileSelectionBox\fP
using \fBXmCreateFileSelectionBox\fP:
.P
.nf
\f(CWArg	args;
int	n;
Widget	fselbox;
Widget	parent_shell;

n = 0;
.mc |
fselbox = XmCreateFileSelectionBox (parent_shell, 
	"fselbox", args, n);
.mc
XtManageChild (fselbox);\fP
.fi
.P
.mc |
In the example shown in Figure 5-4, the Filter is an asterisk
following a directory name, 
.mc
indicating that all files in the directory should be listed.  Here, all files
.mc |
in the directory \fBxmsamplers\fP are listed.
The subdirectories appear in the Directories list, and the files appear
in the Files list.
.mc
The Selection window at the bottom of the \fBXmFileSelectionBox\fP
.mc |
specifies the directory.
Once you have selected a file from the Files list, you can press
the OK button and, depending on what the application has in the 
.mc
callback associated with this button, perform some action on the selected
file.  Note that you can select a file as many times as desired.  The
.mc |
operation on that file is not performed until you select the OK
push button.
.mc
.P
.mc |
You can change the Filter by moving the pointer into the Filter
window and clicking \fBBSelect\fP.
This activates the window and it
is highlighted.
You can then enter the desired filter by typing it in
from the keyboard.
You can also change the Filter by selecting a directory in the
Directories list.
This action replaces the directory portion of the Filter with the
directory you select from the Directories list, leaving the search
pattern in the Filter unchanged.
.mc
.P
.mc |
When you have finished entering the new filter,
press \fBKActivate\fP in the Filter or move
the pointer into the Filter button at the bottom of the main window
and press \fBBSelect\fP.
The subdirectories of the directory specified by the Filter are
displayed in the Directories list, and the files that match the search
pattern specified by the Filter are displayed in the Files list.
.mc
.P
.mc |
You can also generate a new file listing by double clicking on an item
in the Directories list.
This creates a new Filter, using the directory selected from the
Directories list and the search pattern from the existing Filter.
It then uses the new Filter to generate new lists of directories and
files.
.mc
.H 3 "XmForm"
.iX "XmForm"
.iX "Constraints"
The Form widget is a container widget that has no input characteristics of
its own.  Constraints are placed on the Form widget's children.  These
constraints define attachments for each of the four sides of each child,
and the attachments determine the layout of the Form widget when any
resizing occurs.  The child widgets' attachments can be to the Form widget,
to another child of the Form widget, to a relative position within the
Form widget, or to the initial position of the child.
.mc |
.br
.ne 10
.mc
.P
\fBXmForm\fP can be directly instantiated by using either
of the two create
functions that are available:
.P
.iX "XtCreateWidget"
.iX "XmCreateForm"
.nf
.mc |
\f(CWWidget XtCreateWidget (name, xmFormWidgetClass, 
	parent, arglist,argcount)
.mc

.mc |
Widget XmCreateForm (parent, name, arglist, 
	argcount)\fP
.br
.ne 7
.mc
.P
.fi
Both of these create functions create an instance of the Form
widget and return
the associated widget ID.  The following code segment shows you how to create
an instance of \fBXmForm\fP using \fBXmCreateWidget\fP:
.P
.nf
\f(CWArg	args;
int	n;
Widget	form;
Widget	parent_shell;

n = 0;
.mc |
form = XtCreateWidget ("form", xmFormWidgetClass, 
	parent_shell, args, n);
.mc
XtManageChild (form);\fP
.P
.fi
The next code segment shows you how to create an instance of 
\fBXmForm\fP
using \fBXmCreateForm\fP:
.P
.nf
\f(CWArg	args;
int	n;
Widget	form;
Widget	parent_shell;

n = 0;
form = XmCreateForm (parent_shell, "form", args, n);
XtManageChild (form);\fP
.fi
.mc |
.br
.ne 18
.mc
.P
You can create a Form Dialog by using the convenience function
\fBXmFormCreateDialog\fP.  This function creates and returns a Form
widget as a child of a DialogShell widget.  
.P
.iX "XmFormCreateDialog"
.nf
\f(CWArg	args;
int	n;
Widget	form;
Widget	parent_shell;

n = 0;
.mc |
form = XmCreateFormDialog (parent_shell, "form", 
	args, n);
.mc
XtManageChild (form);\fP
.fi
.P
.mc |
The following figure shows an example of a Form widget.
.mc
.ig ++
.br
.ie \nZ \{\
.br
.ne 14P
.C
.sC graphics/form1.tif tiff 300 16P 12P
\&
.P
.sp +1
.mc |
.FG "Form Widget With ArrowButtons"
.mc
.sp +1.5
.\}
.el \{\
.br
.ne 14P
\&
.sp 12.5P
.ce
illustration form1.eps goes here
.sp +1
.mc |
.FG "Form Widget With ArrowButtons"
.mc
.sp +1.5
.\}
.++
.br
.mc |
.ne 2.5i
.mc
\0
.mc |
.FG "Form Widget With ArrowButtons"
.sp 2.25i
.in +1.44i
.mc
.P! graphics/form1.ps 
.mc |
.in -1.44i
.mc
.P
.iX "ArrowButton widgets"
There are three ArrowButton
widgets positioned within the Form widget.  The ArrowButton on the left
.mc |
is set so that its left side position is offset an amount equal to 10
.mc
percent of the Form widget's width, its right side position is 
offset 30 percent of the Form widget's width, and the top is set to a
.mc |
fixed offset of 20 pixels from the top of the Form widget.  When the
.mc
Form widget is resized, the spatial relationships between the Form widget
.mc |
and its children remain the same.  This is shown in the following figure.
.mc
.ig ++
.br
.ie \nZ \{\
.br
.ne 19.5P
.C
.sC graphics/form2.tif tiff 300 24.5P 17.5P
\&
.P
.sp +1
.mc |
.FG "Form Widget After Resizing"
.mc
.sp +1.5
.\}
.el \{\
.br
.ne 19.5P
\&
.sp 17.5P
.ce
illustration form2.eps goes here
.sp +1
.mc |
.FG "Form Widget After Resizing"
.mc
.sp +1.5
.\}
.P
.++
.br
.mc |
.ne 3.25i
.mc
\0
.mc |
.FG "Form Widget After Resizing"
.sp 3.00i
.in +.45i
.mc
.P! graphics/form2.ps 
.mc |
.in -.45i
.mc
.P
.mc |
The code segment that positions the ArrowButton on the left is as follows: 
.mc
.P
.mc |
.in -.5i
.S -2
.mc
.iX "XtSetArg"
.nf
\f(CWWidget	arrow1;
int	n;

n = 0;
XtSetArg (args[n], XmNleftAttachment, XmATTACH_POSITION); n++;
XtSetArg (args[n], XmNleftPosition, 10); n++;
XtSetArg (args[n], XmNrightAttachment, XmATTACH_POSITION); n++;
XtSetArg (args[n], XmNrightPosition, 30); n++;
XtSetArg (args[n], XmNtopAttachment, XmATTACH_FORM); n++;
XtSetArg (args[n], XmNtopOffset, 20); n++;
.iX "XtCreateManagedWidget"
arrow1 = XtCreateManagedWidget("arrow1", xmArrowButtonWidgetClass,
   form, args, n);\fP
.fi
.mc |
.S
.in +.5i
.mc
.P
Note that the position of the top side of the ArrowButton is a constant 
value (20 pixels in this case), regardless of any resizing operations
.mc |
that may occur.  This is because the \fBXmNtopAttachment\fP resource is set
.mc
to \f(CWXmATTACH_FORM\fP as opposed to \f(CWXmATTACH_POSITION\fP.
.mc |
.br
.ne 20
.mc
.H 3 "XmMessageBox"
.iX "XmMessageBox"
.iX "MessageBox"
A MessageBox is used just as its name implies, to pass messages to the
user of an application.  
.mc |
\fBXmMessageBox\fP is a subclass of
\fBXmBulletinBoard\fP and inherits a large number of the BulletinBoard resources.
Convenience Dialogs based on \fBXmMessageBox\fP are provided for several
.mc
user-interaction functions: providing information, asking questions, and
.mc |
notifying the user if errors occur.  The figure below shows some examples of 
\fBXmMessageBox\fP.
.mc
.ig ++
.br
.ie \nZ \{\
.br
.ne 16P
.C
.sC graphics/msgboxes.tif tiff 300 25P 14P
\&
.P
.sp +1
.FG "MessageBox Examples"
.sp +1.5
.\}
.el \{\
.br
.ne 16P
\&
.sp 14P
.ce
illustration msgboxes.eps goes here
.sp +1
.FG "MessageBox Examples"
.sp +1.5
.\}
.sp 1.5
.P
.++
.br
.mc |
.ne 2.75i
.mc
\0
.mc *
.mc
.FG "MessageBox Examples"
.mc |
.sp 2.5i
.in +.69i
.P! graphics/msgboxes.ps 
.in -.69i
.mc
.P
.iX "MessageBox" "examples"
\fBXmMessageBox\fP can be directly instantiated by using either
of the two create
functions that are available:
.P
.iX "XtCreateWidget"
.iX "XmCreateMessageBox"
.nf
.mc |
\f(CWWidget XtCreateWidget (name, 
	xmMessageBoxWidgetClass, 
	parent, arglist, argcount)
.mc

.mc |
Widget XmCreateMessageBox (parent, name, arglist, 
	argcount)\fP
.mc
.fi
.mc |
.br
.ne 16
.P
.mc
Both of these create functions create an instance of the MessageBox
widget and return
the associated widget ID.  The following code segment shows you how to create
an instance of \fBXmMessageBox\fP using \fBXmCreateWidget\fP:
.P
.nf
\f(CWArg	args;
int	n;
Widget	msgbox;
Widget	parent_shell;

n = 0;
.mc |
msgbox = XtCreateWidget ("msgbox", 
	xmMessageBoxWidgetClass,
	parent_shell, args, n);
.mc
XtManageChild (msgbox);\fP
.mc |
.br
.ne 5
.mc
.P
.fi
The next code segment shows you how to create an instance of 
\fBXmMessageBox\fP
using \fBXmCreateMessageBox\fP:
.P
.nf
\f(CWArg	args;
int	n;
Widget	msgbox;
Widget	parent_shell;

n = 0;
.mc |
msgbox = XmCreateMessageBox (parent_shell,
	"msgbox", args, n);
.mc
XtManageChild (msgbox);\fP
.fi
.P
.iX "XmCreateMessageDialog"
You can create a MessageBox Dialog by using the convenience function
\fBXmMessageCreateDialog\fP.  This function creates and returns a MessageBox
widget as a child of a DialogShell widget.  
.P
.nf
\f(CWArg	args;
int	n;
Widget	msgbox;
Widget	parent_shell;

n = 0;
.mc |
msgbox = XmCreateMessageDialog (parent_shell, 
	"msgbox", args, n);
.mc
XtManageChild (msgbox);\fP
.fi
.mc |
.br
.ne 18
.mc
.P
A MessageBox can contain a message symbol, a message, and up to three
.mc |
standard PushButtons (the default buttons are OK,
Cancel, and Help).  The symbol (if any) appears in the upper
left part of the MessageBox (see Figure 5-7), the message appears in the
top and center-to-right side, and the buttons appear along the bottom edge.
.mc
.P
The defaults for the button labels and the message symbols can be changed.
The button labels can be changed by setting a resource in the 
program or in a defaults file.  For example, if you wanted to change the
.mc |
OK label to Close, you could use this code segment:
.ne 4
.mc
.P
.iX "XmStringCreateLtoR"
.iX "XmCreateMessageDialog"
.nf
\f(CWn = 0;
.mc |
XtSetArg (args[n], XmNokLabelString, 
	XmStringCreateLtoR ("Close",
	XmSTRING_DEFAULT_CHARSET)); n++;
messageD = XmCreateMessageDialog (parent, 
	"fontbox", args, n);\fP
.mc
.fi
.P
.mc |
\fBXmStringCreateLtoR\fP is a compound string function.  See Chapter 8,
.mc
``Additional Functionality,''
for more information on this function and compound strings in general.
You can make the same change by using the following statement in a 
defaults file:
.P
\f(CW*msgbox.okLabelString: Close\fP
.P
.mc |
Note that the preceding statement applies only to the MessageBox widget 
whose name is msgbox as specified in the \fBXmCreateMessageDialog\fP
.mc
or \fBXmCreateMessageBox\fP.
.P
The message label can be changed in the same manner as the button.  Use
the resource \f(CWXmNmessageString\fP instead of \f(CWXmNokLabelString\fP.
.P
.iX "XmCreateErrorDialog"
.iX "XmCreateInformationDialog"
.iX "XmCreateQuestionDialog"
.iX "XmCreateWarningDialog"
.iX "XmCreateWorkingDialog"
There are several other convenience dialogs that allow you to create 
.mc |
special versions of \fBXmMessageBox\fP.  These convenience dialogs are
.mc
\fBXmCreateErrorDialog, XmCreateInformationDialog, XmCreateQuestionDialog,
XmCreateWarningDialog\fP, and \fBXmCreateWorkingDialog\fP.  You use these
functions to create the appropriate dialogs in exactly the same manner 
as described earlier for the \fBXmMessageBoxDialog\fP.  
.mc |
.br
.ne 20
.mc
.H 3 "XmSelectionBox"
.iX "XmSelectionBox"
.iX "SelectionBox"
.mc |
A SelectionBox is a general Dialog widget that allows you to select an item
.mc
from a list of items.  SelectionBox can contain a label, a list of items
from which to choose, a selection text edit window that allows you to
enter a selection directly, and three
.mc |
PushButtons (OK, Cancel, and Help).  An example of a 
SelectionBox widget is shown in the following figure.
.mc
.ig ++
.br
.ie \nZ \{\
.br
.ne 19P
.P
.sC graphics/selbox.tif tiff 300 24.5P 17P
\&
.P
.sp +1
.FG "SelectionBox Widget"
.\}
.el \{\
.br
.ne 19P
\&
.sp 17P
.ce
illustration selbox.eps goes here
.sp +1
.FG "SelectionBox Widget"
.sp +1.5
.\}
.P
.++
.br
.mc |
.ne 3.25i
.mc
\0
.mc *
.mc
.FG "SelectionBox Widget"
.mc |
.sp 3i
.in +.8i
.P! graphics/selbox.ps 
.in -.8i
.mc
.P
You can select an item from the SelectionBox in either of two ways:
.BL
.LI
By scrolling through the list of items and selecting one.  The item you
select will be displayed in the SelectionBox text edit window.
.LI
By entering the item name directly into the text edit window.
.LE
.P
.mc |
You can select a new item as many times as you desire since no action is
taken until you move the pointer to the OK button and click mouse
.mc
button 1.
.mc |
.br
.ne 17
.mc
.P
.XmSelectionBox
\fBXmSelectionBox\fP can be directly instantiated by using either
of the two create
functions that are available:
.P
.nf
.iX "XtCreateWidget"
.mc |
\f(CWWidget XtCreateWidget (name, 
	xmSelectionBoxWidgetClass, parent,
	arglist,argcount)
.mc

.iX "XmCreateSelectionBox"
.mc |
Widget XmCreateSelectionBox (parent, name, 
	arglist, argcount)\fP
.mc
.P
.fi
Both of these create functions create an instance of the SelectionBox
widget and return
the associated widget ID.  The following code segment shows you how to create
an instance of \fBXmSelectionBox\fP using \fBXmCreateWidget\fP:
.P
.nf
\f(CWArg	args;
int	n;
Widget	selectbox;
Widget	parent_shell;

n = 0;
.mc |
selectbox = XtCreateWidget ("selectbox", 
	xmSelectionBoxWidgetClass, 
	parent_shell, args, n);
.mc
XtManageChild (selectbox);\fP
.P
.fi
The next code segment shows you how to create an instance of 
\fBXmSelectionBox\fP
using \fBXmCreateSelectionBox\fP:
.P
.nf
\f(CWArg	args;
int	n;
Widget	selectbox;
Widget	parent_shell;

n = 0;
.mc |
selectbox = XmCreateSelectionBox (parent_shell, 
	"selectbox", args, n);
.mc
XtManageChild (selectbox);\fP
.fi
.mc |
.br
.ne 13
.mc
.P
.iX "XmCreateSelectionBoxDialog"
You can create a SelectionBox Dialog by using the convenience function
\fBXmSelectionBoxCreateDialog\fP.  This function creates and returns
a SelectionBox
widget as a child of a DialogShell widget.  
.P
.nf
\f(CWArg	args;
int	n;
Widget	selectbox;
Widget	parent_shell;

n = 0;
.mc |
selectbox = XmCreateSelectionBoxDialog 
	(parent_shell, "selectbox", args, n);
.mc
XtManageChild (selectbox);\fP
.fi
.mc |
.br
.ne 7
.mc
.P
The defaults for the button labels and the list and text window
labels can be changed.
The button labels can be changed by setting a resource in the 
program or in a defaults file.  For example, if you wanted to change the
.mc |
OK label to Close, you could use this code segment:
.mc
.P
.nf
\f(CWn = 0;
.mc |
XtSetArg (args[n], XmNokLabelString, 
	XmStringCreateLtoR ("Close",
	XmSTRING_DEFAULT_CHARSET)); n++;
selectbox = XmCreateSelectionBoxDialog (parent, 
	"selectbox", args, n);\fP
.mc
.fi
.P
The following code segment shows how to set the list items that appear in
the list window.
.P
.mc |
.in -.5i
.S -2
.mc
.nf
\f(CWXmString	item[5];
	.
	.
item[0] = XmStringCreateLtoR ("one", XmSTRING_DEFAULT_CHARSET);
item[1] = XmStringCreateLtoR ("two", XmSTRING_DEFAULT_CHARSET);
item[2] = XmStringCreateLtoR ("three", XmSTRING_DEFAULT_CHARSET);
item[3] = NULL
	.
	.	

n = 0;
XtSetArg (args[n], XmNlistItems, item); n++;
XtSetArg (args[n], XmNlistItemCount, 3); n++;\fP
.fi
.mc |
.S
.in +.5i
.br
.ne 6
.mc
.P
.mc |
An array of type \fBXmString\fP is defined to hold the list items.  Since
.mc
only three of the five entries in the array are used, the fourth is set to
NULL to identify the end of the list.  Each entry is set manually, and then
the argument list is set in the normal manner.
