.\"  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  ALL RIGHTS RESERVED
.hR MessageDialogs
.hS "Description"
.iX "Message DialogBoxes"
.iX "DialogBox" "Message"
.iX "ErrorDialog"
.iX "InformationDialog"
.iX "QuestionDialog"
.iX "WorkingDialog"
MessageDialogs
should be used to convey a message to the user.
.mS
They must not interrupt the user's interaction with the application.
.mE
They should include a message, and one of the following button
arrangements.
.DS
\fBOK\fP
\fBOK Help\fP
\fBOK Cancel\fP
\fBOK Cancel Help\fP
\fBYes No\fP
\fBYes No Help\fP
\fBYes No Cancel\fP
\fBYes No Cancel Help\fP
\fBCancel\fP
\fBCancel Help\fP
\fBRetry Cancel\fP
\fBRetry Cancel Help\fP
.DE
There are a number of different types of MessageDialogs:
ErrorDialog, InformationDialog, QuestionDialog, WorkingDialog,
and WarningDialog.
.hS "Related Information"
For more information,
see the reference pages for DialogBox,
ErrorDialog, InformationDialog, QuestionDialog, WorkingDialog,
and WarningDialog.
