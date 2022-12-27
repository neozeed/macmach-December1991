.\"  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  ALL RIGHTS RESERVED
.hR DialogBox
.hS "Description"
.iX "DialogBox"
A DialogBox should be used to group components
in a window secondary to the main tasks of the
application.
Although a DialogBox may contain any components,
a simple DialogBox is composed of a Label and 
PushButtons for supplying a response to the DialogBox.
The OSF/Motif toolkit provides a number of ready-designed
DialogBoxes for common uses: CommandDialog, FileSelectionDialog,
MessageDialog, PromptDialog, and
SelectionDialog.
There are also a number of different types of MessageDialogs:
ErrorDialog, InformationDialog, QuestionDialog, WorkingDialog,
and WarningDialog.
The
illustration on this reference page shows a typical DialogBox.
.P
.iX "common actions"
DialogBox PushButtons should use the following common
labels and actions.
The actions are in the approximate sequence in which they should
appear in DialogBoxes.
.VL .75i
.LI \fBYes\fP
.iX "Yes"
.mS
Must indicate an affirmative response to a
question posed in the DialogBox, and then close the window.
.mE
While
.B Yes
is not an active-voice label, it implies a positive response
to a question in a
.BR QuestionDialog .
Only use
.B Yes
if it is a clear answer to the question.
.LI \fBNo\fP
.iX "No"
.mS
Must indicate a negative response to a question posed in the DialogBox,
and then close the window.
.mE
While
.B No
is not an active-voice label, it implies a positive response
to a question in a
.BR QuestionDialog .
Only use
.B No
if it is a clear answer to the question.
.LI \fBOK\fP
.iX "OK"
.mS
Must cause the application to apply any changes made
in the DialogBox and dismiss the DialogBox.
.mE
While
.B OK
is not an active-voice label, its usage is too common to change.
.LI \fBApply\fP
.iX "Apply"
.mS
Must apply any changes made to components in the DialogBox.
.mE
.LI \fBRetry\fP
.iX "Retry"
.mS
Must cause the task in progress to be attempted again.
.mE
This action is commonly found in message boxes that report an
error.
.LI \fBStop\fP
.iX "Stop"
.mS
Must end the task in progress at the next possible breaking point.
.mE
This action is commonly found in progress message boxes.
.LI \fBReset\fP
.iX "Reset"
.mS
Must cancel any user changes that have not been applied to
the application.  It must also reset the status of the
DialogBox to the state since the last time the DialgoBox
action was applied or to the initial state of the DialogBox.
.mE
.LI \fBCancel\fP
.iX "Cancel"
.mS
Must close the DialogBox without performing any DialogBox
actions not yet applied to the application.
Pressing
.B KCancel
anywhere in the DialogBox, except during a cancelable drag
operation, must perform the action of this button.
.mE
.LI \fBHelp\ .\ .\ .\fP
.iX "Help"
.mS
Must provide any help for the DialogBox.
.mE
The ellipses should not be used if the help action is not
provided in a new DialogBox.
.LE
.P
You should arrange PushButton actions in
DialogBoxes like other PushButtons, according to
order and frequency of use.
The common action PushButtons should be ordered
as presented in the previous section.
Positive responses to the DialogBox should be presented
first, followed by negative responses and canceling
responses.
.B Help
should always be the last action on the right.
.br
.ne 4i
.hS "Illustration"
.br
.ne 3.7i
\0
.sp 3.5i
.in +.9i
.P! graphics/multirep.ps 
.in -.9i
.sp
.hS "Navigation"
.mS
A DialogBox must follow the navigation model described
in Chapter 2.
.mE
.hS "Related Information"
For more information,
see the reference pages for CommandDialog, ErrorDialog, FileSelectionDialog,
InformationDialog, MessageDialog, PromptDialog, QuestionDialog,
SelectionDialog, WarningDialog, and WorkingDialog.
