.\"  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  ALL RIGHTS RESERVED
.hR ErrorDialog
.hS "Description"
.iX "Error DialogBox"
.iX "DialogBox" "Error"
An ErrorDialog should be used to convey a message about
a user error.
It should stop user interaction with the application
until it is dismissed.
It should include an error symbol,
a message, and one of the following button arrangements.
.DS
\fBOK Cancel\fP
\fBOK Cancel Help\fP
.DE
.hS "Illustration"
.br
.ne 2.0i
\0
.sp 1.8i
.in +1.25i
.P! graphics/errorbox.ps
.in -1.25i
.hS "Related Information"
For more information,
see the reference page for DialogBox.
