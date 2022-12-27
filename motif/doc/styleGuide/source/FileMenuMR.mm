.\"  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  ALL RIGHTS RESERVED
.hR "File Menu"
.hS "Description"
.iX "file menu"
.iX "menus" "file"
The common File Menu contents are described as follows.
Note that you should only include those functions actually
supported by your application.
The illustration on this reference page shows a File Menu.
.VL 1i
.LI "\fB\o'N_'ew\fP"
.iX "New"
.mS
Must create a new file.  The
.B New
operation must clear existing data from the
client area.
If completion of the operation will obliterate current changes to the file, 
you must display a DialogBox, asking the user about saving changes.
This action must have the mnemonic
.BR N .
.mE
.LI "\fB\o'O_'pen\ .\ .\ .\fP"
.iX "Open"
.mS
Must open an existing file.  The
.B Open
operation must prompt the user for the
name of the file with a DialogBox.
.mE
The title bar should be updated with the name of the newly opened file.
.mS
If completion of the operation will obliterate current changes to the file, 
you must display a DialogBox, asking the user about saving changes.
.mE
This DialogBox should be displayed after the user selects a new file.
.mS
This action must have the mnemonic
.BR O .
.mE
.LI \fB\o'S_'ave\fP
.iX "Save"
.iX "file menu"
.mS
Must save the currently opened file without removing the
existing contents of the client area.
If the currently opened file has no name,
.B Save
must prompt for a filename with a DialogBox.
This action must have the mnemonic
.BR S .
.mE
.LI "\fBSave \o'A_'s\ .\ .\ .\fP"
.iX "Save As"
.mS
Must save the currently opened file under a new name
without removing the existing contents of the client area.
The
.B "Save As"
operation must prompt for the name of the file with a DialogBox.
If the user tries to save the new file under an existing name,
.B "Save As"
must alert the user with a DialogBox of a possible loss of data.
This action must have the mnemonic
.BR A .
.mE
.LI \fB\o'P_'rint\fP
.iX "Print"
.mS
Must schedule a file for printing.
If your application requires
specific printing information before printing, the operation must
first request that information with a DialogBox, and the
entry title must be followed by an ellipses.
.mE
Printing information can also be specified for the application
in the
.B Options
Menu.
.mS
This action must have the mnemonic
.BR P .
.mE
.LI "\fB\o'C_'lose\fP"
.iX "Close"
.iX "file menu"
Can be supplied in applications that have multiple independent
primary windows.
.mS
This action must not be supplied in applications with a single
primary window, or multiple dependent primary windows.
This action must only close the current primary
window and its associated secondary windows; that is, the window family.
This action must have the mnemonic
.BR C .
.mE
You can include this action even though it is similar to the
.B Close
action in the window Menu.
This ensures that users have a way to close the primary window even
if they are not running a compliant window manager.
Applications should prompt the user to save any unsaved changes
if the action would cause loss of data.
.LI "\fB\o'E_'xit\fP"
.iX "Exit"
.mS
Must end the current application and all windows associated with it.
.mE
This action is equivalent to closing all primary windows of the application. 
.mS
If completion of the operation will obliterate current changes to the file, 
you must display a DialogBox, asking the user about saving changes.
This action must have the mnemonic
.BR E .
.mE
You should include this action even though it is similar to the
.B Close
action in the window Menu.
This ensures that users have a way to end the application even
if they are not running a compliant window manager.
Applications should prompt the user to save any unsaved changes
if the action would cause loss of data.
If your application does not have a
.B File
Menu, put
.B Exit
at the end of the first PullDown Menu.
.LE
.ne 3.8i
.hS "Illustration"
.br
.ne 3.55i
\0
.sp 3.35i
.in +.9i
.P! graphics/filemenu.ps 
.in -.9i
.sp
.hS "Related Information"
See Chapter 4 for more information on the MenuBar system,
and for general information about Menu design.
