.\"  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  ALL RIGHTS RESERVED
.hR QuestionDialog
.hS "Description"
.iX "Question DialogBox"
.iX "DialogBox" "Question"
A QuestionDialog
should be used to get a user response to a question.
It should interrupt the user's interaction with the application.
It should include a question symbol, a message, and one
of the following button arrangements.
.DS
\fBYes No\fP
\fBYes No Help\fP
.DE
.P
It is possible that both the
.B Yes
and
.B No
actions of a Question DialogBox will perform an action.  If
this is the case the Question DialogBox should use one of the
following button arrangements: 
.DS
\fBYes No Cancel\fP
\fBYes No Cancel Help\fP
.DE
.hS "Illustration"
.br
.ne 2.1i
\0
.sp 1.9i
.in +1.3i
.P! graphics/questbox.ps
.in -1.3i
.hS "Related Information"
For more information,
see the reference page for DialogBox.
